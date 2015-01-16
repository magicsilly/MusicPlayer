#ifndef	__mtl_media_WAVDecodeInfo_H__
#define __mtl_media_WAVDecodeInfo_H__

#pragma once

//
#include <ks.h>
#include <ksmedia.h>

//
#include "DecodeInfo.h"


namespace mtl{ namespace media{ namespace audio{
	
	//
	enum WAVEFILETYPE
	{
		WF_EX  = 1,
		WF_EXT = 2
	};

	//
	enum WAVERESULT
	{
		WR_OK								= 0,
		WR_INVALIDFILENAME					= - 1,
		WR_BADWAVEFILE						= - 2,
		WR_INVALIDPARAM						= - 3,
		WR_INVALIDWAVEID					= - 4,
		WR_NOTSUPPORTEDYET					= - 5,
		WR_WAVEMUSTBEMONO					= - 6,
		WR_WAVEMUSTBEWAVEFORMATPCM			= - 7,
		WR_WAVESMUSTHAVESAMEBITRESOLUTION	= - 8,
		WR_WAVESMUSTHAVESAMEFREQUENCY		= - 9,
		WR_WAVESMUSTHAVESAMEBITRATE			= -10,
		WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT	= -11,
		WR_OFFSETOUTOFDATARANGE				= -12,
		WR_FILEERROR						= -13,
		WR_OUTOFMEMORY						= -14,
		WR_INVALIDSPEAKERPOS				= -15,
		WR_INVALIDWAVEFILETYPE				= -16,
		WR_NOTWAVEFORMATEXTENSIBLEFORMAT	= -17
	};

#ifndef WAVE_FORMAT_PCM
	#define WAVE_FORMAT_PCM 1

	typedef struct waveformat_tag 
	{
		WORD    wFormatTag; 
		WORD    nChannels;    
		DWORD   nSamplesPerSec;   
		DWORD   nAvgBytesPerSec; 
		WORD    nBlockAlign;  
	} WAVEFORMAT;

	typedef struct pcmwaveformat_tag 
	{
		WAVEFORMAT  wf;
		WORD        wBitsPerSample;
	} PCMWAVEFORMAT;
#endif //WAVE_FORMAT_PCM

#ifndef WAVE_FORMAT_EXTENSIBLE
	#define WAVE_FORMAT_EXTENSIBLE 0xFFFE
#endif //WAVE_FORMAT_EXTENSIBLE


	//
#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
	typedef struct tWAVEFORMATEX
	{
		WORD    wFormatTag;
		WORD    nChannels;
		DWORD   nSamplesPerSec;
		DWORD   nAvgBytesPerSec;
		WORD    nBlockAlign;
		WORD    wBitsPerSample;
		WORD    cbSize;
	} WAVEFORMATEX;
#endif // !_WAVEFORMATEX_

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
	typedef struct tWAVEFORMATEXTENSIBLE
	{
		WAVEFORMATEX    Format;
		union 
		{
			WORD wValidBitsPerSample;
			WORD wSamplesPerBlock;
			WORD wReserved;
		} Samples;
		DWORD           dwChannelMask;
		GUID            SubFormat;
	} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_


#pragma pack(push, 4)

	typedef struct WAVEFILEHEADER
	{
		char			szRIFF[4];
		unsigned long	ulRIFFSize;
		char			szWAVE[4];
	} *LPWAVEFILEHEADER;

	typedef struct RIFFCHUNK
	{
		char			szChunkName[4];
		unsigned long	ulChunkSize;
	} *LPRIFFCHUNK;

	typedef struct WAVEFMT
	{
		unsigned short	usFormatTag;
		unsigned short	usChannels;
		unsigned long	ulSamplesPerSec;
		unsigned long	ulAvgBytesPerSec;
		unsigned short	usBlockAlign;
		unsigned short	usBitsPerSample;
		unsigned short	usSize;
		unsigned short  usReserved;
		unsigned long	ulChannelMask;
		GUID            guidSubFormat;
	} *LPWAVEFMT;

#pragma pack(pop)


	//
	class WAVDecodeInfo : public DecodeInfo
	{
	public:
		WAVDecodeInfo( )
		{

		}
		virtual ~WAVDecodeInfo( )
		{

		}
	private:
		WAVEFILETYPE			m_waveFileType;
		WAVEFORMATEXTENSIBLE	m_waveFormatExtensible;

	private:
		WAVERESULT		parseDecodeInfoFromFile(const char *lpszFileName)
		{
			if (!lpszFileName){ return WR_INVALIDPARAM; }

			FILE*	pFile = NULL;
			fopen_s(&pFile, lpszFileName, "rb");
			if(!pFile){ return WR_INVALIDFILENAME; }

			WAVERESULT		wr = WR_BADWAVEFILE;
			
			WAVEFILEHEADER	waveFileHeader;
			fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pFile);
			if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
			{
				RIFFCHUNK		riffChunk;
				WAVEFMT			waveFmt;
				
				while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), pFile) == sizeof(RIFFCHUNK))
				{
					if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
					{
						if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
						{
							fread(&waveFmt, 1, riffChunk.ulChunkSize, pFile);
							if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
							{
								m_waveFileType = WF_EX;
								memcpy(&m_waveFormatExtensible.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
							}
							else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
							{
								m_waveFileType = WF_EXT;
								memcpy(&m_waveFormatExtensible, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
							}
						}
						else
						{
							fseek(pFile, riffChunk.ulChunkSize, SEEK_CUR);
						}
					}
					else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
					{
						m_lLength = riffChunk.ulChunkSize;
						m_lOffset = ftell(pFile);
						fseek(pFile, riffChunk.ulChunkSize, SEEK_CUR);
					}
					else if(!_strnicmp(riffChunk.szChunkName, "list", 4))
					{
						long seek = ftell(pFile);

						unsigned char* buffer = new unsigned char[riffChunk.ulChunkSize];
						fread(buffer, riffChunk.ulChunkSize, 1, pFile);
						for (unsigned int n = 0; n < riffChunk.ulChunkSize; )
						{
							if(!_strnicmp((char*)&buffer[n], "infoiart", 8))
							{
								n += 8;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{
									std::string text((char*)&buffer[n], listLength-1);
									m_stringArtist = text;

									n += listLength;
								}
							}
							else if(!_strnicmp((char*)&buffer[n], "inam", 4))
							{
								n += 4;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{									
									std::string text((char*)&buffer[n], listLength-1);
									m_stringTitle	= text;

									n += listLength;
								}
							}
							else if(!_strnicmp((char*)&buffer[n], "iprd", 4))
							{
								n += 4;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{
									std::string text((char*)&buffer[n], listLength-1);
									m_stringAlbum	= text;

									n += listLength;
								}
							}
							else if(!_strnicmp((char*)&buffer[n], "ignr", 4))
							{
								n += 4;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{
									std::string text((char*)&buffer[n], listLength-1);
									m_stringGenres = text;

									n += listLength;
								}
							}
							else if(!_strnicmp((char*)&buffer[n], "itoc", 4))
							{
								n += 4;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{
									std::string text((char*)&buffer[n], listLength-1);

									n += listLength;
								}
							}
							else if(!_strnicmp((char*)&buffer[n], "itrk", 4))
							{
								n += 4;
								unsigned long listLength = *(unsigned long*)&buffer[n];
								n += 4;
								if( listLength > 0)
								{
									std::string text((char*)&buffer[n], listLength-1);
									m_stringTracks	= text;
									
									n += listLength;
								}
							}
							else
							{
								n ++;
							}
						}
						delete [] buffer; buffer = NULL;

						fseek(pFile, seek, SEEK_SET);
						fseek(pFile, riffChunk.ulChunkSize, SEEK_CUR);
					}
					else
					{
						fseek(pFile, riffChunk.ulChunkSize, SEEK_CUR);
					}

					if (riffChunk.ulChunkSize & 1)
					{ fseek(pFile, 1, SEEK_CUR); }
				}

				if (m_lLength && m_lOffset && 
					((m_waveFileType == WF_EX) || (m_waveFileType == WF_EXT)))
				{ wr = WR_OK; }
			}

			fclose(pFile);
			pFile = NULL;
			return wr;
		}
	public:
		// 从文件初始化信息头
		virtual bool	initDecodeInfoFromFile( const wchar_t* lpszFileName )
		{
			return DecodeInfo::initDecodeInfoFromFile(lpszFileName);
		}
		virtual bool	initDecodeInfoFromFile( const char* lpszFileName )
		{
			WAVERESULT wr = parseDecodeInfoFromFile( lpszFileName );
			if( FAILED( wr ) )
			{ return false; }

			//
			m_lChannels			= m_waveFormatExtensible.Format.nChannels;
			m_lSamplesPerSec	= m_waveFormatExtensible.Format.nSamplesPerSec;
			m_lBitsPerSample	= m_waveFormatExtensible.Format.wBitsPerSample;
			m_lAvgBytesPerSec	= m_waveFormatExtensible.Format.nAvgBytesPerSec;
			m_lBlockAlign		= m_waveFormatExtensible.Format.nBlockAlign;

			//
			return true;
		}
	};

}; //namespace audio
}; //namespace media
}; //namespace mtl


#endif	//__mtl_media_WAVDecodeInfo_H__