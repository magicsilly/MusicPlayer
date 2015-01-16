#pragma once

//
#include "PlayList.h"
#include "AudioDevice.h"

// CPlayListView 视图

class CPlayListView : public CListView
{
	DECLARE_DYNCREATE(CPlayListView)

protected:
	CPlayListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPlayListView();

	CMenu			m_menuContext;
	CPlayList*		m_pPlayList;
	AudioBuffer*	m_pAudioBuffer;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	BOOL	UpdatePlayList(CPlayList* pPlayList);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuPlay();
	afx_msg void OnMenuPause();
	afx_msg void OnMenuStop();
};


