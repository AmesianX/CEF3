
// CefMfcDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CefMfcDemo.h"
#include "CefMfcDemoDlg.h"
#include "afxdialogex.h"

#include "include/cef_app.h"

#include "include/cef_sandbox_win.h"

#pragma comment(lib, ".\\cef_sandbox.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCefMfcDemoDlg �Ի���

#define CEF_USE_SANDBOX 560

CCefMfcDemoDlg::CCefMfcDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCefMfcDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCefMfcDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCefMfcDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GO, &CCefMfcDemoDlg::OnBnClickedBtnGo)
END_MESSAGE_MAP()


// CCefMfcDemoDlg ��Ϣ�������

BOOL CCefMfcDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	void *sandbox_info = NULL;
#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	CefRefPtr<CSimpleClient> client(new CSimpleClient());
	m_simpleClient = client;

	CefMainArgs mainArgs;
	
	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	settings.single_process = true;
	
	//CefRefPtr<CefApp> cefApp;
	
#if !defined(CEF_USE_SANDBOX)  
	settings.no_sandbox = true;
#endif  

	int exit_code = CefExecuteProcess(mainArgs, m_simpleClient.get(), sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.  
		return exit_code;
	}

	CefInitialize(mainArgs, settings, m_simpleClient, sandbox_info);

	RECT rect;
	GetClientRect(&rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + 50;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), rectnew);

	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(winInfo, client, _T("chrome://version"), browserSettings, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCefMfcDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCefMfcDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCefMfcDemoDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CefShutdown();

	CDialogEx::OnClose();
}



void CCefMfcDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	RECT rect;
	GetClientRect(&rect);

	if (m_simpleClient.get())
	{
		CefRefPtr<CefBrowser> browser = m_simpleClient->GetBrowser();
		if (browser)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			::MoveWindow(hwnd, 0, 50, rect.right - rect.left, rect.bottom - 50, true);
		}
	}
}


void CCefMfcDemoDlg::OnBnClickedBtnGo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(strUrl);
	if (strUrl.Trim().IsEmpty())
	{
		AfxMessageBox(_T("��������ַ"));
		return;
	}
	const CefString cefStrUrl(strUrl);
	m_simpleClient->GetBrowser()->GetMainFrame()->LoadURL(cefStrUrl);
}
