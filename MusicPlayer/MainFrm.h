
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

//
#include "PlayListTableView.h"
#include "PlayListView.h"

//
class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:
	CPlayListView* GetPlayListView( );
	CPlayListTableView* GetPlayListTableView( );

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա

	CMFCToolBarImages m_UserImages;

	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar		  m_wndToolBarPlay;
	CMFCStatusBar     m_wndStatusBar;
	CSplitterWndEx	  m_wndSplitter;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


//
__inline CPlayListView* CMainFrame::GetPlayListView( )
{
	CPlayListView* view = (CPlayListView*)m_wndSplitter.GetPane(0, 1);
	return view;
}
__inline CPlayListTableView* CMainFrame::GetPlayListTableView( )
{
	CPlayListTableView* view = (CPlayListTableView*)m_wndSplitter.GetPane(0, 0);
	return view;
}