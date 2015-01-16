
// MainFrm.h : CMainFrame 类的接口
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

// 属性
public:

// 操作
public:
	CPlayListView* GetPlayListView( );
	CPlayListTableView* GetPlayListTableView( );

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员

	CMFCToolBarImages m_UserImages;

	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar		  m_wndToolBarPlay;
	CMFCStatusBar     m_wndStatusBar;
	CSplitterWndEx	  m_wndSplitter;

// 生成的消息映射函数
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