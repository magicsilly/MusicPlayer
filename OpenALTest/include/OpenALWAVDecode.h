#ifndef	__OpenALWAVDecode_H__
#define __OpenALWAVDecode_H__

#pragma once

//
#include <ks.h>
#include <ksmedia.h>

//
#include "OpenALDecode.h"

//
namespace	OpenAL
{
	//
	enum WAVEFILETYPE
	{
		WF_EX  = 1,
		WF_EXT = 2
	};

	enum WAVERESULT
	{
		WR_OK = 0,
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
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
	typedef struct 
	{
		WAVEFORMATEX    Format;
		union 
		{
			WORD wValidBitsPerSample;       /* bits of precision  */
			WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
			WORD wReserved;                 /* If neither applies, set to zero. */
		} Samples;
		DWORD           dwChannelMask;      /* which channels are */
		/* present in stream  */
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

	typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );

	//
	class WAVDecode	: public Decode
	{
	public:
		WAVDecode( )
		{
		}
		virtual ~WAVDecode( )
		{
		}

	private:
		WAVEFILETYPE			m_waveFileType;
		WAVEFORMATEXTENSIBLE	m_waveFormatExtensible;

	private:
		WAVERESULT parseFile(const char *lpszFileName)
		{
			WAVEFILEHEADER	waveFileHeader;
			RIFFCHUNK		riffChunk;
			WAVEFMT			waveFmt;
			WAVERESULT		wr = WR_BADWAVEFILE;

			if (!lpszFileName){ return WR_INVALIDPARAM; }

			FILE*	pFile = NULL;
			// Open the wave file for reading
			fopen_s(&pFile, lpszFileName, "rb");
			if(pFile)
			{
				// Read Wave file header
				fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pFile);
				if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
				{
					while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), pFile) == sizeof(RIFFCHUNK))
					{
						if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
						{
							if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
							{
								fread(&waveFmt, 1, riffChunk.ulChunkSize, pFile);

								// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
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
						else
						{
							fseek(pFile, riffChunk.ulChunkSize, SEEK_CUR);
						}

						// Ensure that we are correctly aligned for next chunk
						if (riffChunk.ulChunkSize & 1)
						{ fseek(pFile, 1, SEEK_CUR); }
					}

					if (m_lLength && m_lOffset && 
						((m_waveFileType == WF_EX) || (m_waveFileType == WF_EXT)))
					{ wr = WR_OK; }
					else
					{ fclose(pFile); pFile = NULL; }
				}
			}
			else
			{
				wr = WR_INVALIDFILENAME;
			}

			if( SUCCEEDED( wr ) )
			{
				m_pData = new char[m_lLength];

				// Seek to start of audio data
				fseek(pFile, m_lOffset, SEEK_SET);

				// Read Sample Data
				if (fread(m_pData, 1, m_lLength, pFile) == m_lLength)
				{
				}
				else
				{
					delete m_pData; m_pData = NULL;
					wr = WR_BADWAVEFILE;
				}
			}

			if( pFile )
			{
				fclose(pFile);
				pFile = NULL;
			}
			m_lOffset	= 0;
			return wr;
		}

		WAVERESULT getBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat)
		{
			WAVERESULT wr = WR_OK;

			if (TRUE)
			{
				if (pfnGetEnumValue && pulFormat)
				{
					*pulFormat = 0;

					if (m_waveFileType == WF_EX)
					{
						if (m_waveFormatExtensible.Format.nChannels == 1)
						{
							switch (m_waveFormatExtensible.Format.wBitsPerSample)
							{
							case 4:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
								break;
							case 8:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8");
								break;
							case 16:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16");
								break;
							}
						}
						else if (m_waveFormatExtensible.Format.nChannels == 2)
						{
							switch (m_waveFormatExtensible.Format.wBitsPerSample)
							{
							case 4:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
								break;
							case 8:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8");
								break;
							case 16:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16");
								break;
							}
						}
						else if ((m_waveFormatExtensible.Format.nChannels == 4) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16))
						{	*pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16"); }
					}
					else if (m_waveFileType == WF_EXT)
					{
						if ((m_waveFormatExtensible.Format.nChannels == 1) &&
							((m_waveFormatExtensible.dwChannelMask == SPEAKER_FRONT_CENTER) ||
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)) ||
							(m_waveFormatExtensible.dwChannelMask == 0)))
						{
							switch (m_waveFormatExtensible.Format.wBitsPerSample)
							{
							case 4:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
								break;
							case 8:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8");
								break;
							case 16:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16");
								break;
							}
						}
						else if ((m_waveFormatExtensible.Format.nChannels == 2) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
						{
							switch (m_waveFormatExtensible.Format.wBitsPerSample)
							{
							case 4:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
								break;
							case 8:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8");
								break;
							case 16:
								*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16");
								break;
							}
						}
						else if ((m_waveFormatExtensible.Format.nChannels == 2) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
							*pulFormat =  pfnGetEnumValue("AL_FORMAT_REAR16");
						else if ((m_waveFormatExtensible.Format.nChannels == 4) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
							*pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16");
						else if ((m_waveFormatExtensible.Format.nChannels == 6) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
							*pulFormat = pfnGetEnumValue("AL_FORMAT_51CHN16");
						else if ((m_waveFormatExtensible.Format.nChannels == 7) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_BACK_CENTER)))
							*pulFormat = pfnGetEnumValue("AL_FORMAT_61CHN16");
						else if ((m_waveFormatExtensible.Format.nChannels == 8) && 
							(m_waveFormatExtensible.Format.wBitsPerSample == 16) && 
							(m_waveFormatExtensible.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
							*pulFormat = pfnGetEnumValue("AL_FORMAT_71CHN16");
					}

					if (*pulFormat == 0)
						wr = WR_INVALIDWAVEFILETYPE;
				}
				else
				{
					wr = WR_INVALIDPARAM;
				}
			}
			else
			{
				wr = WR_INVALIDWAVEID;
			}

			return wr;
		}

		WAVERESULT GetType(WAVEFILETYPE *pwfType)
		{
			if (!pwfType)
				return WR_INVALIDPARAM;

			*pwfType = m_waveFileType;

			return WR_OK;
		}

		WAVERESULT GetFormatExHeader(WAVEFORMATEX *pWFEX)
		{
			if (!pWFEX)
				return WR_INVALIDPARAM;

			memcpy(pWFEX, &(m_waveFormatExtensible.Format), sizeof(WAVEFORMATEX));

			return WR_OK;
		}

		WAVERESULT GetFormatExtensibleHeader(WAVEFORMATEXTENSIBLE *pWFEXT)
		{
			if (m_waveFileType != WF_EXT)
				return WR_NOTWAVEFORMATEXTENSIBLEFORMAT;

			if (!pWFEXT)
				return WR_INVALIDPARAM;

			memcpy(pWFEXT, &(m_waveFormatExtensible), sizeof(WAVEFORMATEXTENSIBLE));

			return WR_OK;
		}

	public:
		virtual ALboolean		loadFile( const char* lpszFileName )
		{
			WAVERESULT wr = parseFile( lpszFileName );
			if( FAILED( wr ) )
			{
				return AL_FALSE;
			}

			//
			m_lChannels			= m_waveFormatExtensible.Format.nChannels;
			m_lSamplesPerSec	= m_waveFormatExtensible.Format.nSamplesPerSec;
			m_lBitsPerSample	= m_waveFormatExtensible.Format.wBitsPerSample;
			m_lAvgBytesPerSec	= m_waveFormatExtensible.Format.nAvgBytesPerSec;
			m_lBlockAlign		= m_waveFormatExtensible.Format.nBlockAlign;

			//
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
			unsigned long format = 0;
			getBufferFormat( alGetEnumValue, (unsigned long*)&format );
			return (ALuint)format;
		}

		virtual ALuint		getBufferLength( )
		{
			return ( m_waveFormatExtensible.Format.nAvgBytesPerSec >> 2 ) - 
				( ( m_waveFormatExtensible.Format.nAvgBytesPerSec >> 2 ) % m_waveFormatExtensible.Format.nBlockAlign );
		}

		virtual ALuint		getFrequency( )
		{
			return (ALuint)m_waveFormatExtensible.Format.nSamplesPerSec;
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

#endif	//__OpenALWAVDecode_H__