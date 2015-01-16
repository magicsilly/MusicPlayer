
#pragma once

//
#include "DecodeInfo.h"
#include "DecodeBuffer.h"

//
using namespace mtl::media;


//
enum PlayBlockType
{
	PlayBlock_Null,
	PlayBlock_Wav,
	PlayBlock_MP3,
	PlayBlock_Flac,
	PlayBlock_APE,
};

//
class CPlayBlock
{
public:
	CPlayBlock(const TCHAR* pFileName);
	virtual ~CPlayBlock(void);

	BOOL			IsLoad( ){ return m_bIsLoad; }
	const TCHAR*	FileName( ){ return m_FileName.c_str(); }
	audio::DecodeInfo*		DecodeInfo( ){ return m_pDecodeInfo; }

private:
	BOOL		m_bIsLoad;
	_tstring	m_FileName;

	PlayBlockType	m_BlockType;

	audio::DecodeInfo*		m_pDecodeInfo;
	audio::DecodeBuffer*	m_pDecodeBuffer;

public:
	BOOL	DecodeBuffer(void** ppBuffer, int* pnLength);
};
