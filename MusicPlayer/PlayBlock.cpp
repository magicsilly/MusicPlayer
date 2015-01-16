
#include "stdafx.h"
#include "PlayBlock.h"

#include "WAVDecodeInfo.h"
#include "WAVDecodeBuffer.h"



//
CPlayBlock::CPlayBlock(const TCHAR* pFileName)
		:	m_bIsLoad ( FALSE ),
			m_FileName( pFileName ),
			m_BlockType( PlayBlock_Null )
{
	m_pDecodeInfo	= NULL;

	_tstring Ext = m_FileName.substr(m_FileName.length()-4, 4);
	if( Ext == _T(".wav") )
	{
		m_BlockType		= PlayBlock_Wav;
		m_pDecodeInfo	= new audio::WAVDecodeInfo;
		m_pDecodeBuffer = new audio::WAVDecodeBuffer;
	}
	else if ( Ext == _T(".mp3") )
	{
		m_BlockType = PlayBlock_MP3;
	}
	else if ( Ext == _T("flac") )
	{
		m_BlockType = PlayBlock_Flac;
	}
	else if ( Ext == _T(".ape") )
	{
		m_BlockType = PlayBlock_APE;
	}

	if( !m_pDecodeInfo )
	{
		return ;
	}
	if( !m_pDecodeInfo->initDecodeInfoFromFile(pFileName) )
	{
		return ;
	}

	m_bIsLoad	= TRUE;
}

CPlayBlock::~CPlayBlock(void)
{
	if( m_pDecodeInfo ){ delete m_pDecodeInfo; m_pDecodeInfo = NULL; }
	if( m_pDecodeBuffer ){ delete m_pDecodeBuffer; m_pDecodeBuffer = NULL; }
}

BOOL	CPlayBlock::DecodeBuffer(void** ppBuffer, int* pnLength)
{
	if( !m_pDecodeInfo || !m_pDecodeBuffer ){ return FALSE; }
	if( !m_pDecodeBuffer->DecodeFromFile(m_pDecodeInfo, (TCHAR*)m_FileName.c_str(), ppBuffer, pnLength) )
	{ return FALSE; }
	return TRUE;
}