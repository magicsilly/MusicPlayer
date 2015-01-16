#pragma once

#include "XmlFile.h"
#include "PlayList.h"



//
class PlayListTable
{
public:
	PlayListTable(void);
	virtual ~PlayListTable(void);

protected:
	std::vector<CPlayList*>	m_PlayLists;

public:
	int		GetPlayListCount(){ return (int)m_PlayLists.size(); }
	CPlayList*	GetPlayList(int nIndex);

	BOOL	InsertPlayList(int nIndex, CPlayList* PlayList);
	BOOL	DeletePlayList(int nIndex);

	BOOL	IsAlready(CString Name);

public:
	BOOL	Load(CString File);
	BOOL	Save(CString File);

	BOOL	LoadPlayList(CPlayList* PlayList);
	BOOL	SavePlayList(CPlayList* PlayList);
};
