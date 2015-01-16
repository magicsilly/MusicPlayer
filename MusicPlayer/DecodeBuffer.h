#ifndef	__mtl_media_DecodeBuffer_H__
#define __mtl_media_DecodeBuffer_H__

#pragma once

namespace mtl{ namespace media{ namespace audio{

	//
	class DecodeInfo;

	//
	class DecodeBuffer
	{
	public:
		DecodeBuffer( )
		{

		}
		virtual ~DecodeBuffer( )
		{

		}
	protected:

	public:
		virtual bool	DecodeFromFile(DecodeInfo* pDecodeInfo, wchar_t *pFileName, void** ppBuffer, int* pnLength)
		{
			char szFileNameA[MAX_PATH]; WideCharToMultiByte(CP_ACP, 0, pFileName, -1, szFileNameA, MAX_PATH,  NULL, NULL);
			return DecodeFromFile(pDecodeInfo, szFileNameA, ppBuffer, pnLength);
		}
		virtual bool	DecodeFromFile(DecodeInfo* pDecodeInfo, char *pFileName, void** ppBuffer, int* pnLength) = 0;
	};



}; //namespace audio
}; //namespace media
}; //namespace mtl


#endif	//__mtl_media_DecodeBuffer_H__