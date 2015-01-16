#ifndef	__OpenALFLACDecode_H__
#define __OpenALFLACDecode_H__

#pragma once

//
#define  FLAC__NO_DLL

//
#include <ogg/ogg.h>
#include <flac/all.h>
#include <flacplus/all.h>

#ifdef _DEBUG
#pragma comment(lib, "libogg_d.lib")
#pragma comment(lib, "libflac_d.lib")
#pragma comment(lib, "libflacplus_d.lib")
#else
#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libflac.lib")
#pragma comment(lib, "libflacplus.lib")
#endif

//
#include "OpenALDecode.h"

//
namespace	OpenAL
{
	//
	class FLACDecode;

	//
	class FLACFile		: public FLAC::Decoder::File
	{
	public:
		FLACFile( FLACDecode* decode ) : FLAC::Decoder::File( ), m_pDecode( decode )
		{}
		virtual ~FLACFile( )
		{}
	private:
		FLACDecode*		m_pDecode;

	public:
		virtual FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[]);
		virtual void error_callback(FLAC__StreamDecoderErrorStatus status)
		{
			printf("Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
		}
		virtual void metadata_callback(const ::FLAC__StreamMetadata *metadata)
		{
			if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) 
			{
				FLAC__uint64	total_samples	= metadata->data.stream_info.total_samples;
				FLAC__uint32	sample_rate		= metadata->data.stream_info.sample_rate;
				FLAC__uint32	channels		= metadata->data.stream_info.channels;
				FLAC__uint32	bps				= metadata->data.stream_info.bits_per_sample;

				printf("sample rate    : %u Hz\n", sample_rate);
				printf("channels       : %u\n", channels);
				printf("bits per sample: %u\n", bps);
				printf("total samples  : %I64u\n", total_samples);
			}
		}
	};

	//
	class FLACDecode	: public Decode
	{
	public:
		FLACDecode( )
		{
			m_bInitialize	= AL_FALSE;

			m_pFile			= new FLACFile( this );
			m_bInitialize	= AL_TRUE;
		}

		virtual ~FLACDecode( )
		{
			delete m_pFile; m_pFile = NULL;
		}

	private:
		ALboolean				m_bInitialize;
		FLACFile*				m_pFile;

	public:
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
			m_pFile->set_md5_checking(true);
			FLAC__StreamDecoderInitStatus init_status = m_pFile->init(lpszFileName);
			if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) 
			{
				printf("ERROR: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
				return AL_FALSE;
			}

			//
			if( !m_pFile->process_until_end_of_stream() )
			{
				printf("FAILD: process until end of stream: state %s \n", m_pFile->get_state( ).resolved_as_cstring( *m_pFile ) );
				return AL_FALSE;
			}

			m_lSamplesPerSec	= m_pFile->get_sample_rate( );
			m_lChannels			= m_pFile->get_channels( );
			m_lBitsPerSample	= m_pFile->get_bits_per_sample( );
			m_lAvgBytesPerSec	= m_lSamplesPerSec * m_lBitsPerSample / 8 * m_lChannels;
			m_lBlockAlign		= m_lBitsPerSample / 8 * m_lChannels;

			m_pFile->finish( );

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

	FLAC__StreamDecoderWriteStatus FLACFile::write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[])
	{
		if( frame->header.number.sample_number == 0 )
		{
		}
		else
		{
			if( m_pDecode )
			{
				for(unsigned int i = 0; i < frame->header.blocksize; i++) 
				{
					// left channel
					m_pDecode->appendData( (void*)&(buffer[0][i]), sizeof(FLAC__int16) );
					// right channel
					m_pDecode->appendData( (void*)&(buffer[1][i]), sizeof(FLAC__int16) );
				}
			}
		}
		return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
	}

}; //namespace	OpenAL

#endif	//__OpenALFLACDecode_H__