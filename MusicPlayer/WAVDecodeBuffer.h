#ifndef	__mtl_media_WAVDecodeBuffer_H__
#define __mtl_media_WAVDecodeBuffer_H__

#pragma once

//
#include "DecodeBuffer.h"
#include "WAVDecodeInfo.h"

//
namespace mtl{ namespace media{ namespace audio{


	//
	class WAVDecodeBuffer : public DecodeBuffer
	{
	public:
		WAVDecodeBuffer( )
		{

		}
		virtual ~WAVDecodeBuffer( )
		{

		}
	protected:

	public:
		virtual bool	DecodeFromFile(DecodeInfo* pDecodeInfo, char *pFileName, void** ppBuffer, int* pnLength)
		{
			FILE*	File = NULL;
			fopen_s(&File, pFileName, "rb");
			if( !File ){ return false; }

			//
			unsigned char* buffer = new unsigned char[pDecodeInfo->getLength()];
			fseek(File, pDecodeInfo->getOffset(), SEEK_SET);
			fread(buffer, pDecodeInfo->getLength(), 1, File);

			if(ppBuffer){ *ppBuffer = buffer; }
			if(pnLength){ *pnLength = pDecodeInfo->getLength(); }

			fclose(File);
			return true;
		}
	};



}; //namespace audio
}; //namespace media
}; //namespace mtl


#endif	//__mtl_media_WAVDecodeBuffer_H__