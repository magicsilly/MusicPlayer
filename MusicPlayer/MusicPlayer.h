
// MusicPlayer.h : MusicPlayer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "PlayListTable.h"
#include "AudioDevice.h"


// CMusicPlayerApp:
// �йش����ʵ�֣������ MusicPlayer.cpp
//

class CMusicPlayerApp : public CWinAppEx
{
public:
	CMusicPlayerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	AudioDevice*	m_pAudioDevice;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMusicPlayerApp	theApp;
extern PlayListTable	g_PlayListTable;
