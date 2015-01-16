#pragma once

//
#include "PlayBlock.h"

//
class CPlayList
{
public:
	CPlayList(int nIndex, CString& Name, BOOL bNewGuid = TRUE);
	CPlayList(int nIndex, CString& Name, CString& Guid);
	~CPlayList(void);

	int				Index( ){ return m_nIndex; }
	const TCHAR*	Name( ){ return m_Name; }
	const TCHAR*	Guid( ){ return m_Guid; }

	void			Name(CString& Name){ m_Name = Name; }

protected:
	int			m_nIndex;
	CString		m_Name;
	CString		m_Guid;

private:
	std::vector<CPlayBlock*>	m_PlayBlocks;

public:
	void		RemoveAllPlayBlock( );

	CPlayBlock*	GetPlayBlock(int nIndex);
	int			GetPlayBlockCount( );

public:
	BOOL		AddMusicFile(CString& File);
};
