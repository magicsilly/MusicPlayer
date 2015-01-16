
#include "stdafx.h"
#include "PlayList.h"


//
CPlayList::CPlayList(int nIndex, CString& Name, BOOL bNewGuid)
{
	m_nIndex	= nIndex;
	m_Name		= Name;

	if( bNewGuid )
	{
		GUID	guid= {0};
		HRESULT hResult = CoCreateGuid(&guid);
		if(SUCCEEDED(hResult))
		{
			wchar_t text[100] = {0};
			if( StringFromGUID2(guid, text, 100) > 0 )
			{
#ifdef _UNICODE
				m_Guid = text;
#else
				char textA[100] = {0}
				WideCharToMultiByte(CP_ACP, 0, text, 100, textA, 100, NULL, NULL);
				m_Guid = textA;
#endif
			}
		}
	}

}

CPlayList::CPlayList(int nIndex, CString& Name, CString& Guid)
{
	m_nIndex	= nIndex;
	m_Name		= Name;
	m_Guid		= Guid;
}

CPlayList::~CPlayList(void)
{
	RemoveAllPlayBlock( );
}

void		CPlayList::RemoveAllPlayBlock( )
{
	for( std::vector<CPlayBlock*>::iterator i = m_PlayBlocks.begin();
		i != m_PlayBlocks.end(); i ++ )
	{ delete (*i); (*i) = NULL; }
	m_PlayBlocks.clear( );
}

CPlayBlock*	CPlayList::GetPlayBlock(int nIndex)
{
	if( nIndex >= (int)m_PlayBlocks.size()){ return NULL; }
	return m_PlayBlocks[nIndex];
}

int			CPlayList::GetPlayBlockCount( )
{
	return (int)m_PlayBlocks.size();
}

//
class FindByFileNameProc
{
	CString		m_FileName;
public:
	FindByFileNameProc(CString FileName) : m_FileName(FileName){ }
	bool operator ()(CPlayBlock* pBlock){ return m_FileName == pBlock->FileName(); }
};

BOOL	CPlayList::AddMusicFile(CString& File)
{
	//
	std::vector<CPlayBlock*>::iterator i = std::find_if(m_PlayBlocks.begin(), m_PlayBlocks.end(), FindByFileNameProc(File));
	if( i != m_PlayBlocks.end() ){ return FALSE; }

	//
	CPlayBlock*	pBlock	= new CPlayBlock(File);
	if( !pBlock->IsLoad( ) )
	{
		delete pBlock;
		return FALSE;
	}

	//
	m_PlayBlocks.push_back(pBlock);
	return TRUE;
}