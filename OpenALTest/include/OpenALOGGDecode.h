#ifndef	__OpenALOGGDecode_H__
#define __OpenALOGGDecode_H__

#pragma once

//
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#ifdef _DEBUG
#pragma comment(lib, "libogg_d.lib")
#pragma comment(lib, "libvorbis_d.lib")
#pragma comment(lib, "libvorbisfile_d.lib")
#else
#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libvorbis.lib")
#pragma comment(lib, "libvorbisfile.lib")
#endif

//
#include "OpenALDecode.h"

//
namespace	OpenAL
{
	//
	class OGGDecode	: public Decode
	{
	public:
		OGGDecode( )
		{
			m_bInitialize	= AL_FALSE;

			m_bInitialize	= AL_TRUE;
		}

		virtual ~OGGDecode( )
		{
		}

	private:
		ALboolean				m_bInitialize;
		OggVorbis_File			m_OggVorbisFile;
		vorbis_info*			m_pVorbisInfo;

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

			//
			this->clearData( );

			//
			int result = ov_fopen( lpszFileName, &m_OggVorbisFile );
			if( result != 0 ){ return AL_FALSE; }
			
			m_pVorbisInfo = ov_info( &m_OggVorbisFile, -1 );
			if( m_pVorbisInfo )
			{
				m_lSamplesPerSec	= m_pVorbisInfo->rate;
				m_lChannels			= m_pVorbisInfo->channels;
				m_lBitsPerSample	= 16; //Ä¬ÈÏ16Î»
				m_lAvgBytesPerSec	= m_lSamplesPerSec * m_lBitsPerSample / 8 * m_lChannels;
				m_lBlockAlign		= m_lBitsPerSample / 8 * m_lChannels;

				int buffer_len = 0;
				if (m_pVorbisInfo->channels == 1)
				{
					buffer_len = m_lSamplesPerSec > 1;
					buffer_len = buffer_len - (buffer_len % 2);
				}
				else if (m_pVorbisInfo->channels == 2)
				{
					buffer_len = m_lSamplesPerSec;
					buffer_len = buffer_len - (buffer_len % 4);
				}
				else if (m_pVorbisInfo->channels == 4)
				{
					buffer_len = m_lSamplesPerSec * 2;
					buffer_len = buffer_len - (buffer_len % 8);
				}
				else if (m_pVorbisInfo->channels == 6)
				{
					buffer_len = m_lSamplesPerSec * 3;
					buffer_len = buffer_len - (buffer_len % 12);
				}

				if( getBufferFormat( ) != 0 )
				{
					char* decode_buffer = (char*)malloc(buffer_len);

					while( TRUE )
					{
						int current_section = 0;
						long decode_len	= ov_read(&m_OggVorbisFile, decode_buffer, buffer_len, 0, 2, 1, &current_section );
						this->appendData( decode_buffer, decode_len );

						if( decode_len <= 0 )
						{
							break;
						}
					}

					free( decode_buffer );
				}

				//
			}

			//
			ov_clear( &m_OggVorbisFile );

			//
			m_lOffset	= 0;
			return AL_TRUE;
		}

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

#endif	//__OpenALOGGDecode_H__