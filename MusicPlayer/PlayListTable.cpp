
#include "stdafx.h"
#include "MusicPlayer.h"
#include "PlayListTable.h"

//
PlayListTable::PlayListTable(void)
{
}

PlayListTable::~PlayListTable(void)
{
	for( std::vector<CPlayList*>::iterator it = m_PlayLists.begin();
		it != m_PlayLists.end(); it ++ )
	{
		delete *it; *it = NULL;
	}
	m_PlayLists.clear();
}

CPlayList*	PlayListTable::GetPlayList(int nIndex)
{
	if( nIndex >= (int)m_PlayLists.size() ){ return NULL; }
	return m_PlayLists[nIndex];
}

BOOL	PlayListTable::InsertPlayList(int nIndex, CPlayList* PlayList)
{
	if( nIndex > (int)m_PlayLists.size() ){ return FALSE; }
	m_PlayLists.insert(m_PlayLists.begin() + nIndex, PlayList);
	return TRUE;
}

BOOL	PlayListTable::DeletePlayList(int nIndex)
{
	if( nIndex >= (int)m_PlayLists.size() ){ return FALSE; }
	m_PlayLists.erase(m_PlayLists.begin() + nIndex);
	return TRUE;
}

//
class FindByNameProc
{
	CString		m_Name;
public:
	FindByNameProc(CString Name) : m_Name(Name){ }
	bool operator ()(CPlayList* PlayList){ return m_Name == PlayList->Name(); }
};


BOOL	PlayListTable::IsAlready(CString Name)
{
	std::vector<CPlayList*>::iterator it = std::find_if(m_PlayLists.begin(), m_PlayLists.end(), FindByNameProc(Name));
	if( it != m_PlayLists.end() ){ return TRUE; }
	return FALSE;
}

BOOL	PlayListTable::Load(CString File)
{
	CXmlFile	xmlFile;
	if(!xmlFile.Load((const TCHAR*)File))
	{ return FALSE; }

	CXmlElement	elemRoot;
	xmlFile.GetDocument().GetElementByDoc(elemRoot);
	if( elemRoot == NULL ){ return FALSE; }

	std::vector<CXmlElement>	elemPlayLists;
	elemRoot.GetElementByName(_T("PlayList"), elemPlayLists);

	CMusicPlayerApp* pApp = static_cast<CMusicPlayerApp*>(AfxGetApp());
	ASSERT(pApp != NULL);

	HKEY hKey = pApp->GetSectionKey(_T("PlayListTable"));
	ASSERT(hKey != NULL);

	int nIndex = pApp->GetProfileInt(_T("PlayListTable"), _T("Index"), -1);
	if( nIndex < 0 )
	{ pApp->WriteProfileInt(_T("PlayListTable"), _T("Index"), 0); }

	for( std::vector<CXmlElement>::iterator it = elemPlayLists.begin();
		it != elemPlayLists.end(); it ++ )
	{
		CXmlString	Name;
		(*it).GetAttributeByName(_T("Name"), Name);
		CXmlString	Guid;
		(*it).GetAttributeByName(_T("Guid"), Guid);

		CPlayList*	PlayList = new CPlayList(nIndex, CString(Name.c_str()), CString(Guid.c_str()));
		m_PlayLists.push_back(PlayList);

		pApp->WriteProfileInt(_T("PlayListTable"), _T("Index"), ++nIndex);

		LoadPlayList(m_PlayLists.back());
	}

	RegCloseKey(hKey);
	return TRUE;
}

BOOL	PlayListTable::LoadPlayList(CPlayList* PlayList)
{
	CString		File;
	File.Format(_T("./PlayList/%s.mpml"),  PlayList->Guid());

	CXmlFile	xmlFile;
	if(!xmlFile.Load((const TCHAR*)File))
	{ return FALSE; }

	CXmlElement	elemRoot;
	xmlFile.GetDocument().GetElementByDoc(elemRoot);
	if( elemRoot == NULL ){ return FALSE; }

	std::vector<CXmlElement>	elemPlayBlocks;
	elemRoot.GetElementByName(_T("PlayBlock"), elemPlayBlocks);

	for( std::vector<CXmlElement>::iterator it = elemPlayBlocks.begin();
		it != elemPlayBlocks.end(); it ++ )
	{
		CXmlString TempFile;
		(*it).GetText(TempFile);

		PlayList->AddMusicFile(CString(TempFile.c_str()));
	}

	return TRUE;
}


BOOL	PlayListTable::Save(CString File)
{
	CXmlFile	xmlFile;
	xmlFile.GetDocument().AddProcessingInstruction(_T("xml"), _T("version=\"1.0\" encoding=\"utf-8\""));
	xmlFile.GetDocument().AddComment(_T("This MusicPlayer add comment."));
	
	CXmlElement	elemRoot;
	xmlFile.GetDocument().CreateElement(_T("PlayListTable"), elemRoot);
	xmlFile.GetDocument().SetElementByDoc(elemRoot);

	for( std::vector<CPlayList*>::iterator it = m_PlayLists.begin();
		it != m_PlayLists.end(); it ++ )
	{
		CXmlElement	elemPlayList;
		xmlFile.GetDocument().CreateElement(_T("PlayList"), elemPlayList);
		elemPlayList.SetAttributeByName( _T("Name"), CXmlString((*it)->Name( )) );
		elemPlayList.SetAttributeByName( _T("Guid"), CXmlString((*it)->Guid( )) );

		elemRoot.AppendChild(elemPlayList);

		//
		SavePlayList(*it);
	}

	if(!xmlFile.Save((const TCHAR*)File))
	{
		::CreateDirectory(_T("./PlayList"), NULL);
		if(!xmlFile.Save((const TCHAR*)File))
		{ return FALSE; }
	}
	return TRUE;
}

BOOL	PlayListTable::SavePlayList(CPlayList* PlayList)
{
	CXmlFile	xmlFile;
	xmlFile.GetDocument().AddProcessingInstruction(_T("xml"), _T("version=\"1.0\" encoding=\"utf-8\""));
	xmlFile.GetDocument().AddComment(_T("This MusicPlayer add comment."));

	CXmlElement	elemRoot;
	xmlFile.GetDocument().CreateElement(_T("PlayList"), elemRoot);
	xmlFile.GetDocument().SetElementByDoc(elemRoot);
	elemRoot.SetAttributeByName(_T("Name"), CXmlString(PlayList->Name()));
	elemRoot.SetAttributeByName(_T("Guid"), CXmlString(PlayList->Guid()));
	elemRoot.SetAttributeByName(_T("Block"), PlayList->GetPlayBlockCount());

	int nCount = PlayList->GetPlayBlockCount();
	for (int n  = 0; n < nCount; n ++)
	{
		CPlayBlock*	pBlock = PlayList->GetPlayBlock(n);
		
		CXmlElement	elemPlayBlock;
		xmlFile.GetDocument().CreateElement(_T("PlayBlock"), elemPlayBlock);
		elemPlayBlock.SetText(CXmlString(pBlock->FileName()));

		elemRoot.AppendChild(elemPlayBlock);
	}

	CString		File;
	File.Format(_T("./PlayList/%s.mpml"),  PlayList->Guid());
	if(!xmlFile.Save((const TCHAR*)File))
	{
		::CreateDirectory(_T("./PlayList"), NULL);
		if(!xmlFile.Save((const TCHAR*)File))
		{ return FALSE; }
	}

	return TRUE;
}