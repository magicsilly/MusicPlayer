// PlayListTableView.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer.h"
#include "MainFrm.h"
#include "PlayListTableView.h"
#include "PlayListView.h"

// CPlayListTableView

IMPLEMENT_DYNCREATE(CPlayListTableView, CListView)

CPlayListTableView::CPlayListTableView()
{

}

CPlayListTableView::~CPlayListTableView()
{
}


BEGIN_MESSAGE_MAP(CPlayListTableView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CPlayListTableView::OnNMRClick)
	ON_COMMAND(ID_MENU_NEWPLAYLIST, &CPlayListTableView::OnNewPlayList)
	ON_COMMAND(ID_MENU_DELPLAYLIST, &CPlayListTableView::OnDelPlayList)
	ON_COMMAND(ID_MENU_RENPLAYLIST, &CPlayListTableView::OnRenPlayList)
	ON_COMMAND(ID_MENU_ADDFILE_TO_PLAYLIST, &CPlayListTableView::OnOpenFiles)
	ON_NOTIFY_REFLECT(NM_CLICK, &CPlayListTableView::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CPlayListTableView::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CPlayListTableView::OnLvnItemchanged)
END_MESSAGE_MAP()



// CPlayListTableView 消息处理程序
void CPlayListTableView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
}

int CPlayListTableView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
	{ return -1; }

	// TODO:  在此添加您专用的创建代码

	//
	CListCtrl&	list_ctrl = this->GetListCtrl();
	list_ctrl.ModifyStyle(0, LVS_REPORT| LVS_SHOWSELALWAYS);
	list_ctrl.SetExtendedStyle(list_ctrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	list_ctrl.InsertColumn(0, _T("播放列表"), LVCFMT_LEFT, 150);

	//
	m_menuContext.CreatePopupMenu( );
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_NEWPLAYLIST, _T("新建列表"));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_DELPLAYLIST, _T("删除列表"));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_RENPLAYLIST, _T("重命名列表"));
	m_menuContext.AppendMenu(MF_SEPARATOR, 0, _T(""));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_ADDFILE_TO_PLAYLIST, _T("添加文件"));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_ADDFOLD_TO_PLAYLIST, _T("添加目录"));

	// 创建个编辑框,供输入重命名
	if( !m_editRename.Create(WS_VISIBLE| WS_CHILD| WS_BORDER| ES_AUTOHSCROLL, CRect(0, 0, 150, 20), this, 0) )
	{ return -1; }
	m_editRename.SetFont(list_ctrl.GetFont());
	HideRenameEditBox( );

	//
	LoadPlayListTable( );

	//	
	return 0;
}

void CPlayListTableView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//
	CPoint point;
	GetCursorPos(&point);
	m_menuContext.TrackPopupMenu(TPM_LEFTALIGN| TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CPlayListTableView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if(m_editRename.IsWindowEnabled())
	{ HideRenameEditBox( ); }
}

void CPlayListTableView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if(m_editRename.IsWindowEnabled())
	{ HideRenameEditBox( ); }
	ShowRenameEditBox( );
}


void CPlayListTableView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pNMLV->uNewState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_FOCUSED))
	{
		//
		CPlayList* pPlayList= g_PlayListTable.GetPlayList(pNMLV->iItem);
		if( !pPlayList ){ return; }

		CPlayListView* view = static_cast<CMainFrame*>(AfxGetMainWnd())->GetPlayListView( );
		view->UpdatePlayList(pPlayList);
	}
}

void CPlayListTableView::OnNewPlayList( )
{
	CMusicPlayerApp* pApp = static_cast<CMusicPlayerApp*>(AfxGetApp());
	ASSERT(pApp != NULL);

	HKEY hKey = pApp->GetSectionKey(_T("PlayListTable"));
	ASSERT(hKey != NULL);

	int nIndex = pApp->GetProfileInt(_T("PlayListTable"), _T("Index"), -1);
	if( nIndex < 0 )
	{ pApp->WriteProfileInt(_T("PlayListTable"), _T("Index"), 0); }

	//
	CString Name;
	Name.Format(_T("新建列表%u"), nIndex);
	int nItem = this->GetListCtrl().GetItemCount();
	if(nItem < 0){ nItem = 0; }
	this->GetListCtrl().InsertItem(nItem, Name);

	//
	CPlayList*	PlayList = new CPlayList(nIndex, Name);
	g_PlayListTable.InsertPlayList(nItem, PlayList);

	//
	pApp->WriteProfileInt(_T("PlayListTable"), _T("Index"), ++nIndex);
	RegCloseKey(hKey);
}

void CPlayListTableView::OnDelPlayList( )
{
	int			nCount	= this->GetListCtrl().GetSelectedCount();
	POSITION	Pos		= this->GetListCtrl().GetFirstSelectedItemPosition();
	for( ;Pos = this->GetListCtrl().GetFirstSelectedItemPosition(); )
	{
		int nItem = this->GetListCtrl().GetNextSelectedItem(Pos);
		this->GetListCtrl().DeleteItem(nItem); 
		g_PlayListTable.DeletePlayList(nItem);
	}
}

void CPlayListTableView::OnRenPlayList( )
{
	//
	ShowRenameEditBox( );

}

void CPlayListTableView::OnOpenFiles( )
{
	CFileDialog		dlg(true, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT);
	dlg.m_ofn.lpstrTitle	= _T("添加音乐:");
	dlg.m_ofn.lpstrFilter	= _T("Music Files(*.wav,*.mp3,*.flac,*.ape)\0*.wav;*.mp3;*.flac;*.ape");
	dlg.m_ofn.nMaxFile		= 100*MAX_PATH;
	dlg.m_ofn.lpstrFile		= new TCHAR[dlg.m_ofn.nMaxFile];
	ZeroMemory(dlg.m_ofn.lpstrFile, dlg.m_ofn.nMaxFile*sizeof(TCHAR));
	
	std::list<CString>	listFileName;
	if( dlg.DoModal() == IDOK )
	{
		POSITION Pos = dlg.GetStartPosition();
		while( Pos )
		{
			CString FileName = dlg.GetNextPathName(Pos);
			listFileName.push_back(FileName);
		}
	}
	delete [] dlg.m_ofn.lpstrFile;
	dlg.m_ofn.lpstrFile = NULL;

	//
	POSITION	Pos		= this->GetListCtrl().GetFirstSelectedItemPosition();
	int			nItem	= this->GetListCtrl().GetNextSelectedItem(Pos);
	if( nItem < 0 )
	{ return ; }

	//
	CPlayList* pPlayList= g_PlayListTable.GetPlayList(nItem);
	if( !pPlayList ){ return; }

	for (std::list<CString>::iterator i = listFileName.begin();
		i != listFileName.end(); i ++)
	{
		pPlayList->AddMusicFile(*i);
	}

	//
	CPlayListView* view = static_cast<CMainFrame*>(AfxGetMainWnd())->GetPlayListView( );
	view->UpdatePlayList(pPlayList);
}


BOOL CPlayListTableView::LoadPlayListTable( )
{
	this->GetListCtrl().DeleteAllItems();

	int nCount = g_PlayListTable.GetPlayListCount( );
	for (int n = 0; n < nCount; n ++)
	{
		CPlayList*	pPlayList	= g_PlayListTable.GetPlayList(n);
		if(pPlayList)
		{	
			int nItem = this->GetListCtrl().GetItemCount();
			if(nItem < 0){ nItem = 0; }
			this->GetListCtrl().InsertItem(nItem, pPlayList->Name());
		}
	}

	//
	return TRUE;
}

BOOL CPlayListTableView::ShowRenameEditBox( )
{
	int			nCount	= this->GetListCtrl().GetSelectedCount();
	if( nCount <= 0 ){ return FALSE; }

	POSITION	Pos		= this->GetListCtrl().GetFirstSelectedItemPosition();
	int			nItem	= this->GetListCtrl().GetNextSelectedItem(Pos);


	CRect		rect;
	this->GetListCtrl().GetItemRect(nItem, &rect, LVIR_BOUNDS);
	m_editRename.MoveWindow(&rect);

	m_listRename.clear();
	for (int n = 0 ; n < nCount ; n++)
	{
		m_listRename.push_back(nItem);
		nItem = this->GetListCtrl().GetNextSelectedItem(Pos);
	}

	//显示编辑框供用户输入
	m_editRename.SetWindowText(_T(""));
	m_editRename.EnableWindow(TRUE);
	m_editRename.ShowWindow(SW_SHOWNORMAL);
	m_editRename.SetFocus();

	return TRUE;
}


BOOL CPlayListTableView::HideRenameEditBox( )
{
	if( m_editRename.IsWindowEnabled() )
	{ m_editRename.EnableWindow(FALSE); }
	m_editRename.ShowWindow(SW_HIDE);

	CString Text;
	m_editRename.GetWindowText(Text);
	
	int			nCount	= (int)m_listRename.size();
	if( nCount <= 0 ){ return FALSE; }

	if( Text == _T("") ||
		g_PlayListTable.IsAlready(Text) )
	{ return FALSE; }

	int n = 0;
	for( std::list<int>::iterator i = m_listRename.begin(); 
		i != m_listRename.end(); i ++, n++ )
	{
		int	nItem	= *i; CString aa = Text;
		if(n > 0)
		{ TCHAR tt[50]; _itot_s(n, tt, 10); aa += tt; }
		this->GetListCtrl().SetItemText(nItem, 0, aa);
		
		CPlayList* pPlayList = g_PlayListTable.GetPlayList(nItem);
		pPlayList->Name(aa);
	}

	return TRUE;
}

