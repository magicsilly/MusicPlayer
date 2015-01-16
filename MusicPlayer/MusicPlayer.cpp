
// MusicPlayer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MusicPlayer.h"
#include "MainFrm.h"

#include "DSoundDevice.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicPlayerApp

BEGIN_MESSAGE_MAP(CMusicPlayerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMusicPlayerApp::OnAppAbout)
END_MESSAGE_MAP()


// CMusicPlayerApp 构造

CMusicPlayerApp::CMusicPlayerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_pAudioDevice	= NULL;
}

// 唯一的一个 CMusicPlayerApp 对象

CMusicPlayerApp theApp;
PlayListTable	g_PlayListTable;

// CMusicPlayerApp 初始化

BOOL CMusicPlayerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("XSoftware"));

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);


	//
	g_PlayListTable.Load(_T("PlayList/PlayList.mppl"));

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//
	m_pAudioDevice	= new DSoundDevice( );
	m_pAudioDevice->Initialize(pFrame->GetSafeHwnd());

	//
	CPlayListTableView* view = pFrame->GetPlayListTableView();
	if(view && view->GetListCtrl().GetItemCount() > 0)
	{ view->GetListCtrl().SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED); }

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}


// CMusicPlayerApp 消息处理程序




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CMusicPlayerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMusicPlayerApp 自定义加载/保存方法

void CMusicPlayerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMusicPlayerApp::LoadCustomState()
{
}

void CMusicPlayerApp::SaveCustomState()
{
}

// CMusicPlayerApp 消息处理程序




int CMusicPlayerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	//
	g_PlayListTable.Save(_T("PlayList/PlayList.mppl"));

	if( m_pAudioDevice )
	{ m_pAudioDevice->Release(); }

	return CWinAppEx::ExitInstance();
}
