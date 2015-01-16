// PlayListView.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer.h"
#include "PlayListView.h"

//
using namespace mtl::media;



// CPlayListView

IMPLEMENT_DYNCREATE(CPlayListView, CListView)

CPlayListView::CPlayListView()
{
	m_pPlayList		= NULL;
	m_pAudioBuffer	= NULL;
}

CPlayListView::~CPlayListView()
{
}

BEGIN_MESSAGE_MAP(CPlayListView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CPlayListView::OnNMRClick)
	ON_COMMAND(ID_MENU_PLAY, &CPlayListView::OnMenuPlay)
	ON_COMMAND(ID_MENU_PAUSE, &CPlayListView::OnMenuPause)
	ON_COMMAND(ID_MENU_STOP, &CPlayListView::OnMenuStop)
END_MESSAGE_MAP()


// CPlayListView 诊断

#ifdef _DEBUG
void CPlayListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlayListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlayListView 消息处理程序

int CPlayListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
	{ return -1; }

	// TODO:  在此添加您专用的创建代码
	
	//
	CListCtrl&	list_ctrl = this->GetListCtrl();
	list_ctrl.ModifyStyle(0, LVS_REPORT);
	list_ctrl.SetExtendedStyle(list_ctrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	list_ctrl.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	list_ctrl.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 150);
	list_ctrl.InsertColumn(2, _T("标题"), LVCFMT_LEFT, 150);
	list_ctrl.InsertColumn(3, _T("艺术家"), LVCFMT_LEFT, 100);
	list_ctrl.InsertColumn(4, _T("时长"), LVCFMT_LEFT, 80);
	list_ctrl.InsertColumn(5, _T("码率"), LVCFMT_LEFT, 100);

	//
	m_menuContext.CreatePopupMenu( );
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_PLAY, _T("播放"));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_PAUSE, _T("暂停"));
	m_menuContext.AppendMenu(MF_STRING, ID_MENU_STOP, _T("停止"));

	//
	return 0;
}

void CPlayListView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//
	CPoint point;
	GetCursorPos(&point);
	m_menuContext.TrackPopupMenu(TPM_LEFTALIGN| TPM_RIGHTBUTTON, point.x, point.y, this);
}


BOOL	CPlayListView::UpdatePlayList(CPlayList* pPlayList)
{
	if(!pPlayList){ return FALSE; }
	m_pPlayList		= pPlayList;

	this->GetListCtrl().DeleteAllItems();

	//
	int nCount = pPlayList->GetPlayBlockCount();
	for (int n = 0; n < nCount; n ++)
	{
		CPlayBlock*	pBlock = pPlayList->GetPlayBlock(n);
		ASSERT(pBlock);

		int nItem = this->GetListCtrl().GetItemCount();
		if( nItem < 0 ){ nItem = 0; }

		LV_ITEMA	item = {0};
		item.mask		= LVIF_TEXT;
		item.iItem		= nItem;
		item.iSubItem	= 0;
		item.pszText	= (char*)pBlock->DecodeInfo()->m_stringTracks.c_str();
		SendMessageA(this->GetListCtrl().GetSafeHwnd(), LVM_INSERTITEMA, 0, (LPARAM)&item);

		CString	FileName= pBlock->FileName();
		FileName = FileName.Mid(FileName.ReverseFind('\\')+1);
		this->GetListCtrl().SetItemText(nItem, 1, FileName);

		item.iSubItem	= 2;
		item.pszText	= (char*)pBlock->DecodeInfo()->m_stringTitle.c_str();
		SendMessageA(this->GetListCtrl().GetSafeHwnd(), LVM_SETITEMTEXTA, nItem, (LPARAM)&item);

		item.iSubItem	= 3;
		item.pszText	= (char*)pBlock->DecodeInfo()->m_stringArtist.c_str();
		SendMessageA(this->GetListCtrl().GetSafeHwnd(), LVM_SETITEMTEXTA, nItem, (LPARAM)&item);

		audio::Time MaxTime = pBlock->DecodeInfo()->getMaxTime();
		CString Text;
		Text.Format(_T("%02d:%02d:%02d"), MaxTime.h, MaxTime.m, MaxTime.s);
		this->GetListCtrl().SetItemText(nItem, 4, Text);

		audio::AvgBytesPerSec	xbps;
		pBlock->DecodeInfo()->getAvgBytesPerSec(&xbps);

		Text.Format(_T("%.2f kbps"), xbps.kbps);
		this->GetListCtrl().SetItemText(nItem, 5, Text);
	}

	//
	return TRUE;
}

void CPlayListView::OnMenuPlay()
{
	// TODO: 在此添加命令处理程序代码
	if( !m_pPlayList ){ return; }
	if( m_pAudioBuffer )
	{
		if( !m_pAudioBuffer->IsPlaying(1) )
		{ m_pAudioBuffer->Play(1); }
		return;
	}

	//
	POSITION	Pos		= this->GetListCtrl().GetFirstSelectedItemPosition();
	int			nItem	= this->GetListCtrl().GetNextSelectedItem(Pos);
	if( nItem < 0 ){ return ; }

	CPlayBlock* pBlock	= m_pPlayList->GetPlayBlock(nItem);
	if( !pBlock ){ return; }

	CMusicPlayerApp*	pApp	= static_cast<CMusicPlayerApp*>(AfxGetApp());
	m_pAudioBuffer				= pApp->m_pAudioDevice->CreateAudioBuffer( );

	AudioFormat			Format;
	Format.m_lChannels			= pBlock->DecodeInfo()->getChannels();
	Format.m_lBitsPerSample		= pBlock->DecodeInfo()->getBitsPerSample();
	Format.m_lSamplesPerSec		= pBlock->DecodeInfo()->getSamplesPerSec();
	Format.m_lBlockAlign		= (WORD)(Format.m_lBitsPerSample / 8 * Format.m_lChannels);
	Format.m_lAvgBytesPerSec	= (DWORD)(Format.m_lSamplesPerSec * Format.m_lBlockAlign);
	if( !m_pAudioBuffer->Init(&Format) )
	{
		pApp->m_pAudioDevice->DestroyAudioBuffer(m_pAudioBuffer);
		m_pAudioBuffer = NULL;
		return ;
	}

	void* pDecodeBuffer	= NULL;
	int	  nDecodeLength	= 0;
	if( !pBlock->DecodeBuffer(&pDecodeBuffer, &nDecodeLength) )
	{
		pApp->m_pAudioDevice->DestroyAudioBuffer(m_pAudioBuffer);
		m_pAudioBuffer = NULL;
		return ;
	}

	int nFragmentIndex = m_pAudioBuffer->CreateFragmentBuffer(pDecodeBuffer, nDecodeLength);
	if( nFragmentIndex > 0 )
	{
		m_pAudioBuffer->Play( nFragmentIndex );
	}

	//
	delete [] pDecodeBuffer;
	nDecodeLength = 0;
}

void CPlayListView::OnMenuPause()
{
	// TODO: 在此添加命令处理程序代码
	
	if( !m_pAudioBuffer ){ return; }
	m_pAudioBuffer->Stop(1);
}


void CPlayListView::OnMenuStop()
{
	// TODO: 在此添加命令处理程序代码

	if( !m_pAudioBuffer ){ return; }

	if(m_pAudioBuffer->IsPlaying(1))
	{
		m_pAudioBuffer->Stop(1);
	}


	CMusicPlayerApp*	pApp	= static_cast<CMusicPlayerApp*>(AfxGetApp());
	pApp->m_pAudioDevice->DestroyAudioBuffer(m_pAudioBuffer);
	m_pAudioBuffer = NULL;
}

