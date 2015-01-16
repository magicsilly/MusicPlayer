#ifndef	__mtl_media_DecodeInfo_H__
#define __mtl_media_DecodeInfo_H__

#pragma once

namespace mtl{ namespace media{ namespace audio{

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
	class DecodeInfo
	{
	public:
		DecodeInfo( )
		{

		}
		virtual ~DecodeInfo( )
		{

		}
	protected:
		long		m_lLength;			//���ݳ���
		long		m_lOffset;			//����ƫ��

	protected:
		long		m_lChannels;		//������
		long		m_lSamplesPerSec;	//����Ƶ��
		long		m_lBitsPerSample;	//����λ��
		long		m_lAvgBytesPerSec;	//ÿ���ֽ�
		long		m_lBlockAlign;		//ÿ���ֽ�

	public:

		std::string		m_stringTracks;	//����
		std::string		m_stringTitle;	//����
		std::string		m_stringArtist;	//������
		std::string		m_stringAlbum;	//ר��
		std::string		m_stringGenres;	//����

	public:
		//���ļ���ʼ��������Ϣ,������̳�ʵ��
		virtual bool		initDecodeInfoFromFile( const wchar_t* lpszFileName )
		{
			char szFileNameA[MAX_PATH]; WideCharToMultiByte(CP_ACP, 0, lpszFileName, -1, szFileNameA, MAX_PATH,  NULL, NULL);
			return initDecodeInfoFromFile(szFileNameA);
		}
		virtual bool		initDecodeInfoFromFile( const char* lpszFileName )
		{
			return true;
		};
		//���ڴ��ʼ��������Ϣ,������̳�ʵ��
		virtual bool		initDecodeInfoFromMemory( void* lpBuffer, int nLength )
		{
			return true;
		};

	public:
		long				getLength( )
		{
			return m_lLength;
		}
		long				getOffset( )
		{
			return m_lOffset;
		}
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
	};



}; //namespace audio
}; //namespace media
}; //namespace mtl


#endif	//__mtl_media_DecodeInfo_H__