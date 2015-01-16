#ifndef	__OpenALMP3Decode_H__
#define __OpenALMP3Decode_H__

#pragma once

//
#include <mp3/mpg123.h>
#ifdef _DEBUG
#pragma comment(lib, "libmpg123_d.lib")
#else
#pragma comment(lib, "libmpg123.lib")
#endif

//
#include "OpenALDecode.h"

//
namespace	OpenAL
{
	//
	class MP3Decode	: public Decode
	{
	public:
		MP3Decode( )
		{
			m_bInitialize	= AL_FALSE;

			//
			mpg123_init();

			int	result = 0;
			m_pHandler = mpg123_new(NULL, &result);
			if(m_pHandler == NULL)
			{
				printf("Unable to create mpg123 handle: %s\n", mpg123_plain_strerror(result));
				return ;
			}

			m_bInitialize	= AL_TRUE;
		}
		virtual ~MP3Decode( )
		{
			mpg123_delete( m_pHandler );
			mpg123_exit( );
		}
	private:
		ALboolean				m_bInitialize;
		mpg123_handle			*m_pHandler;

	private:
		ALboolean				appendData( void* lpData, ALuint nLength )
		{
			//
			int src_len = m_lLength;
			int mem_num	= src_len / 0x100000;
			if( src_len % 0x100000 > 0 ){ mem_num += 1; }

			
			if( mem_num * 0x100000 >= (int)(src_len + nLength) )
			{
				memcpy( &m_pData[src_len], lpData, nLength );
			}
			else
			{
				mem_num = (src_len + nLength) / 0x100000;
				if( (src_len + nLength) % 0x100000 > 0 ){ mem_num += 1; }

				char*	p	= new char[mem_num * 0x100000];
				if( m_pData )
				{
					memcpy( &p[0], m_pData, src_len );
					delete [] m_pData; m_pData = NULL;
				}

				memcpy( &p[src_len], lpData, nLength );
				m_pData	= p;
			}

			m_lLength += nLength;
			return AL_TRUE;
		}
		void					clearData( )
		{
			if( m_pData ){ delete [] m_pData; m_pData = NULL; }
			m_lLength	= 0;
		}
	public:

		virtual ALboolean		loadFile( const char* lpszFileName )
		{
			if( !m_bInitialize ){ return AL_FALSE; }

			int result = mpg123_open(m_pHandler, lpszFileName);
			if(result != MPG123_OK){ return AL_FALSE; }

			long rate;
			int channels, enc;
			mpg123_getformat(m_pHandler, &rate, &channels, &enc);
			m_lChannels			= channels;
			m_lSamplesPerSec	= rate;
			m_lLength			= 0;

			if(enc & MPG123_ENC_FLOAT_64)
			{
				m_lBitsPerSample = 64;
			}
			else if(enc & MPG123_ENC_FLOAT_32)
			{
				m_lBitsPerSample = 32;
			}
			else if(enc & MPG123_ENC_16)
			{
				m_lBitsPerSample = 16;
			}
			else
			{
				m_lBitsPerSample = 8;
			}
			m_lAvgBytesPerSec	= m_lSamplesPerSec * m_lBitsPerSample / 8 * m_lChannels;
			m_lBlockAlign		= m_lBitsPerSample / 8 * m_lChannels;

			//
			this->clearData( );

			//
			int buflen = 16384 * 2 * 2;
			unsigned char buf[16384*2*2];
			while( TRUE )
			{
				size_t len = 0;
				result = mpg123_read(m_pHandler, buf, buflen, &len);
				if((int)len <= 0){ break; }

				if(result == MPG123_NEW_FORMAT)
				{	
				}
				else if(result == MPG123_OK)
				{
					this->appendData( buf, len );
				}
				else if(result == MPG123_ERR)
				{
					printf("Error: %s", mpg123_strerror(m_pHandler));
					break; 
				}
			};

			mpg123_close(m_pHandler);

			//
			m_lOffset	= 0;
			return AL_TRUE;
		}

/*		
		virtual ALboolean		loadFile( const char* lpszFileName )
		{
			if( !m_bInitialize ){ return AL_FALSE; }

			mpg123_param(m_pHandler, MPG123_VERBOSE, 4, 0);

			int result = mpg123_param(m_pHandler, MPG123_FLAGS, MPG123_FUZZY | MPG123_SEEKBUFFER | MPG123_GAPLESS, 0);
			if(result != MPG123_OK)
			{
				printf("Unable to set library options: %s\n", mpg123_plain_strerror(result));
				return AL_FALSE;
			}

			result = mpg123_param(m_pHandler, MPG123_INDEX_SIZE, -1, 0);
			if(result != MPG123_OK)
			{
				printf("Unable to set index size: %s\n", mpg123_plain_strerror(result));
				return AL_FALSE;
			}

			result = mpg123_format_none(m_pHandler);
			if(result != MPG123_OK)
			{
				printf("Unable to disable all output formats: %s\n", mpg123_plain_strerror(result));
				return AL_FALSE;
			}

			//
			size_t nrates;
			const long *rates;
			mpg123_rates(&rates, &nrates);
			for(int i=0; i< (int)nrates; i++)
			{
				result = mpg123_format(m_pHandler, rates[i], MPG123_MONO | MPG123_STEREO,  MPG123_ENC_FLOAT_32);
				if(result != MPG123_OK)
				{
					printf("Unable to set float output formats: %s\n", mpg123_plain_strerror(result));
					return AL_FALSE;
				}
			}

			result = mpg123_open_feed(m_pHandler);
			if(result != MPG123_OK)
			{
				printf("Unable open feed: %s\n", mpg123_plain_strerror(result));
				return AL_FALSE;
			}

			//
			FILE *in;
			fopen_s(&in, lpszFileName, "rb");
			if(in == NULL)
			{
				printf("Unable to open input file %s\n", lpszFileName);
				return AL_FALSE;
			}

			//
			off_t	inoffset;
			size_t	inc = 0;
			int buflen = 16384 * 2 * 2;
			unsigned char buf[16384*2*2];
			while(result = mpg123_feedseek(m_pHandler, 95000, SEEK_SET, &inoffset) == MPG123_NEED_MORE)
			{
				int len = fread(buf, sizeof(unsigned char), buflen, in);
				if(len <= 0){ break; }
				inc += len;

				int state = mpg123_feed(m_pHandler, buf, len);
				if(state == MPG123_ERR)
				{
					printf("Error: %s", mpg123_strerror(m_pHandler));
					fclose(in);
					return AL_FALSE; 
				}
			}

			long rate;
			int channels, enc;
			mpg123_getformat(m_pHandler, &rate, &channels, &enc);
			m_lChannels			= channels;
			m_lSamplesPerSec	= rate;
			m_lLength			= 0;

			if(enc & MPG123_ENC_FLOAT_64)
			{
				m_lBitsPerSample = 64;
			}
			else if(enc & MPG123_ENC_FLOAT_32)
			{
				m_lBitsPerSample = 32;
			}
			else if(enc & MPG123_ENC_16)
			{
				m_lBitsPerSample = 16;
			}
			else
			{
				m_lBitsPerSample = 8;
			}
			m_lAvgBytesPerSec	= m_lSamplesPerSec * m_lBitsPerSample / 8 * m_lChannels;
			m_lBlockAlign		= m_lBitsPerSample / 8 * m_lChannels;

			this->clearData( );

			fseek(in, inoffset, SEEK_SET);
			while(1)
			{
				int len = fread(buf, sizeof(unsigned char), buflen, in);
				if(len <= 0){ break; }
				inc += len;

				result = mpg123_feed(m_pHandler, buf, len);
				while(result != MPG123_ERR && result != MPG123_NEED_MORE)
				{
					off_t	num;
					size_t	bytes;
					unsigned char* audio;
					result = mpg123_decode_frame(m_pHandler, &num, &audio, &bytes);
					if(result == MPG123_NEW_FORMAT)
					{	
					}
					else if(result == MPG123_OK)
					{
						this->appendData( audio, bytes );
					}
				}

				if(result == MPG123_ERR)
				{
					printf("Error: %s", mpg123_strerror(m_pHandler));
					break; 
				}
			};

			//
			fclose(in);

			m_lOffset	= 0;
			return AL_TRUE;
		}
*/

		virtual void*		getData( )
		{
			return m_pData;
		}
		virtual ALuint		getDataLength( )
		{
			return (ALuint)m_lLength;
		}
		virtual ALuint		getBufferFormat( )
		{
			ALuint	nFormat = 0;
			if (m_lChannels == 1)
			{
				switch (m_lBitsPerSample)
				{
				case 4:
					nFormat = alGetEnumValue("AL_FORMAT_MONO_IMA4");
					break;
				case 8:
					nFormat = alGetEnumValue("AL_FORMAT_MONO8");
					break;
				case 16:
					nFormat = alGetEnumValue("AL_FORMAT_MONO16");
					break;
				}
			}
			else if (m_lChannels == 2)
			{
				switch (m_lBitsPerSample)
				{
				case 4:
					nFormat = alGetEnumValue("AL_FORMAT_STEREO_IMA4");
					break;
				case 8:
					nFormat = alGetEnumValue("AL_FORMAT_STEREO8");
					break;
				case 16:
					nFormat = alGetEnumValue("AL_FORMAT_STEREO16");
					break;
				}
			}
			else if ((m_lChannels == 4) && 
				(m_lBitsPerSample == 16))
			{	nFormat = alGetEnumValue("AL_FORMAT_QUAD16"); }

			return (ALuint)nFormat;
		}

		virtual ALuint		getBufferLength( )
		{
			return ( m_lAvgBytesPerSec >> 2 ) - 
				( ( m_lAvgBytesPerSec >> 2 ) % m_lBlockAlign );;
		}

		virtual ALuint		getFrequency( )
		{
			return (ALuint)m_lSamplesPerSec;
		}

		virtual void		setReadOffset(long lOffset)
		{
			m_lOffset	= lOffset;
		}

		virtual long		getReadOffset( )
		{
			return m_lOffset;
		}
		virtual ALuint		readData(void *pData, long lDataLength)
		{
			if( m_lOffset + lDataLength > m_lLength )
			{
				lDataLength = m_lLength - m_lOffset;
			}
			memcpy( pData, &m_pData[m_lOffset], lDataLength );
			m_lOffset += lDataLength;
			return (ALuint)lDataLength;
		}
	};
}; //namespace	OpenAL

#endif	//__OpenALMP3Decode_H__