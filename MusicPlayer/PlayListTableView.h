#pragma once



// CPlayListTableView

class CPlayListTableView :	public CListView
{
	DECLARE_DYNCREATE(CPlayListTableView)

public:
	CPlayListTableView();
	virtual ~CPlayListTableView();

	BOOL		LoadPlayListTable( );
	BOOL		ShowRenameEditBox( );
	BOOL		HideRenameEditBox( );

protected:
	CMenu		m_menuContext;
	CEdit		m_editRename;
	std::list<int>	m_listRename;

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnNewPlayList( );
	virtual void OnDelPlayList( );
	virtual void OnRenPlayList( );

	virtual void OnOpenFiles( );

public:
	virtual void OnInitialUpdate( );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


