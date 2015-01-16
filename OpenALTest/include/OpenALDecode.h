#ifndef	__OpenALDecode_H__
#define __OpenALDecode_H__

#pragma once

//
namespace	OpenAL
{
	//
	struct Time
	{
	public:
		short	h;
		short	m;
		short	s;
		short	ms;
	};

	//
	struct AvgBytesPerSec 
	{
	public:
		float	bps;
		float	kbps;
		float	mbps;
	};

	//
	class Decode
	{
	public:
		Decode( )
		{
			m_pData			= NULL;
			m_lLength		= 0;
			m_lOffset		= 0;
		}
		virtual ~Decode( )
		{
			if( m_pData )
			{ delete [] m_pData; m_pData = NULL; }
		}

	protected:
		char*		m_pData;
		long		m_lLength;
		long		m_lOffset;

	protected:
		long		m_lChannels;		//声道数
		long		m_lSamplesPerSec;	//采样频率
		long		m_lBitsPerSample;	//采样位数
		long		m_lAvgBytesPerSec;	//每秒字节
		long		m_lBlockAlign;		//每样字节

	public:
		virtual ALboolean	loadFile( const char* lpszFileName ) = 0;
		virtual void*		getData( ) = 0;
		virtual ALuint		getDataLength( ) = 0;
		virtual ALuint		getBufferFormat( ) = 0;
		virtual ALuint		getBufferLength( ) = 0;
		virtual ALuint		getFrequency( ) = 0;
		virtual void		setReadOffset(long lOffset) = 0;
		virtual long		getReadOffset( ) = 0;
		virtual ALuint		readData(void *pData, long lDataLength) = 0;

	public:
		long				getChannels( )
		{
			return m_lChannels;
		}
		long				getSamplesPerSec( )
		{
			return m_lSamplesPerSec;
		}
		long				getBitsPerSample( )
		{
			return m_lBitsPerSample;
		}
		long				getAvgBytesPerSec( AvgBytesPerSec* abps = NULL )
		{
			if( abps )
			{
				abps->bps	= (float)m_lAvgBytesPerSec * 8;
				abps->kbps	= abps->bps / 1000.0f;
				abps->mbps	= abps->kbps/ 1000.0f;
			}
			return m_lAvgBytesPerSec;
		}

	public:
		Time				getMaxTime( )
		{
			Time	result;
			result.ms	= (short)(((m_lLength % m_lAvgBytesPerSec) * 1000) / m_lAvgBytesPerSec);
			long s		= m_lLength / m_lAvgBytesPerSec;
			result.s	= s % 60; s = s / 60;
			result.m	= s % 60; s = s / 60;
			result.h	= s % 60;
			return result;
		}
		Time				getCurTime( )
		{
			Time	result;
			result.ms	= (short)(((m_lOffset % m_lAvgBytesPerSec) * 1000) / m_lAvgBytesPerSec);
			long s		= m_lOffset / m_lAvgBytesPerSec;
			result.s	= s % 60; s = s / 60;
			result.m	= s % 60; s = s / 60;
			result.h	= s % 60;
			return result;
		}
	};
}; //namespace	OpenAL

#endif	//__OpenALDecode_H__