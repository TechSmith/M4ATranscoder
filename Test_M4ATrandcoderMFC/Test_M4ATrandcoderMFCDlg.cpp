
// Test_M4ATrandcoderMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_M4ATrandcoderMFC.h"
#include "Test_M4ATrandcoderMFCDlg.h"
#include "afxdialogex.h"
#include "CWaveToM4A.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) delete (x); (x) = NULL;
#endif

#define TIMER_EVENT_PROGRESS 15358

#define DEFAULT_INFILE        NEVER_TRANSLATE("TestInput\\ExampleWaveFile.wav")
#define DEFAULT_OUTFILE       NEVER_TRANSLATE("TestOutput\\ExampleM4AFile.m4a")

#ifndef VERIFYHR
#define VERIFYHR(hr)          VERIFY(SUCCEEDED(hr))
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest_M4ATrandcoderMFCDlg dialog



CTest_M4ATrandcoderMFCDlg::CTest_M4ATrandcoderMFCDlg(CWnd* pParent /*=NULL*/)
   : CDialogEx(CTest_M4ATrandcoderMFCDlg::IDD, pParent), m_pTranscoder(NULL), m_nIDEvent(0), m_pTaskbar(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTest_M4ATrandcoderMFCDlg::~CTest_M4ATrandcoderMFCDlg()
{
   if (m_pTaskbar)
      m_pTaskbar->Release();
   SAFE_DELETE(m_pTranscoder);
}

void CTest_M4ATrandcoderMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CTest_M4ATrandcoderMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
   if (nIDEvent == m_nIDEvent)
   {
      if (m_pTranscoder != NULL)
      {
         CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
         int nProgress = (int)m_pTranscoder->GetProgress();
         pProgress->SetPos(nProgress);
         m_pTaskbar->SetProgressValue(GetSafeHwnd(), nProgress, 100);
      }
   }
}

BEGIN_MESSAGE_MAP(CTest_M4ATrandcoderMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_WM_DESTROY()
   ON_BN_CLICKED(IDC_BTN_GETFORMATS, OnBnClickedGetFormats)
   ON_CBN_SELCHANGE(IDC_COMBO_FORMATS, OnCbnSelchangeComboFormats)
   ON_BN_CLICKED(IDC_BTN_TRANSCODE, OnBnClickedBtnTranscode)
   ON_BN_CLICKED(IDC_BTN_CANCELTRANSCODE, OnBnClickedBtnCanceltranscode)
   ON_MESSAGE(WM_USER_NOTIFY_FINISH, OnTranscodeCompleted)
   ON_BN_CLICKED(IDC_BTN_BROWSE_INPUT, OnBnClickedBtnBrowseInput)
   ON_BN_CLICKED(IDC_BTN_BROWSE_OUTPUT, OnBnClickedBtnBrowseOutput)
   ON_WM_TIMER()
END_MESSAGE_MAP()


// CTest_M4ATrandcoderMFCDlg message handlers

BOOL CTest_M4ATrandcoderMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

   SetDlgItemText(IDC_EDIT_INPUT, DEFAULT_INFILE);
   SetDlgItemText(IDC_EDIT_OUTPUT, DEFAULT_OUTFILE);

   CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
   pProgress->SetRange32(0, 100);

   GetDlgItem(IDC_BTN_CANCELTRANSCODE)->EnableWindow(FALSE);

   HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

   VERIFYHR(CoCreateInstance(
      CLSID_TaskbarList, NULL, CLSCTX_ALL,
      IID_ITaskbarList3, (void**)&m_pTaskbar));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_M4ATrandcoderMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTest_M4ATrandcoderMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTest_M4ATrandcoderMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTest_M4ATrandcoderMFCDlg::OnDestroy()
{
   if (m_nIDEvent != 0)
   {
      KillTimer(m_nIDEvent);
      m_nIDEvent = 0;
   }
}

void CTest_M4ATrandcoderMFCDlg::OnBnClickedGetFormats()
{
   // TODO
}

void CTest_M4ATrandcoderMFCDlg::OnCbnSelchangeComboFormats()
{
   // TODO
}

void CTest_M4ATrandcoderMFCDlg::OnBnClickedBtnTranscode()
{
   CString strInput, strOutput;
   GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strInput);
   GetDlgItem(IDC_EDIT_OUTPUT)->GetWindowText(strOutput);

   CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
   pProgress->SetPos(0);
   m_pTaskbar->SetProgressState(GetSafeHwnd(), TBPF_NORMAL);
   m_pTaskbar->SetProgressValue(GetSafeHwnd(), 0, 100);

   if (!CWaveToM4A::PerformCheck(strInput, strOutput))
      return;

   m_pTranscoder = new CWaveToM4A(strInput, strOutput);
   (void)m_pTranscoder->Transcode(GetSafeHwnd());//Multi-threaded so ignoring return value here :)

   //Disable some UI controls
   GetDlgItem(IDC_BTN_TRANSCODE)->EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_CANCELTRANSCODE)->EnableWindow(TRUE);

   m_nIDEvent = SetTimer(TIMER_EVENT_PROGRESS, 100, NULL);
}


void CTest_M4ATrandcoderMFCDlg::OnBnClickedBtnCanceltranscode()
{
   if (m_pTranscoder == NULL)
      return;

   GetDlgItem(IDC_BTN_CANCELTRANSCODE)->EnableWindow(FALSE);

   m_pTranscoder->Cancel();
   SAFE_DELETE(m_pTranscoder);
}

LRESULT CTest_M4ATrandcoderMFCDlg::OnTranscodeCompleted(WPARAM wparam, LPARAM lparam)
{
   if (m_nIDEvent != 0)
   {
      KillTimer(m_nIDEvent);
      m_nIDEvent = 0;
   }

   WORD bSucceeded = LOWORD(wparam);
   if (bSucceeded)
   {
      CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
      pProgress->SetPos(100);
      m_pTaskbar->SetProgressState(GetSafeHwnd(), TBPF_NORMAL);
      m_pTaskbar->SetProgressValue(GetSafeHwnd(), 100, 100);
   }
   else
   {
      m_pTaskbar->SetProgressState(GetSafeHwnd(), TBPF_ERROR);
   }

   SAFE_DELETE(m_pTranscoder);

   GetDlgItem(IDC_BTN_TRANSCODE)->EnableWindow(TRUE);
   GetDlgItem(IDC_BTN_CANCELTRANSCODE)->EnableWindow(FALSE);

   return 0L;
}

void CTest_M4ATrandcoderMFCDlg::OnBnClickedBtnBrowseInput()
{
   CFileDialog dlg(TRUE/*Open*/, NEVER_TRANSLATE("wav"), NULL/*filename*/, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
      NEVER_TRANSLATE("Wave Files(*.wav)|*.wav|All Files(*.*)|*.*||"));
   if (IDOK == dlg.DoModal())
      GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(dlg.m_ofn.lpstrFile);
}


void CTest_M4ATrandcoderMFCDlg::OnBnClickedBtnBrowseOutput()
{
   CFileDialog dlg(FALSE/*Save*/, NEVER_TRANSLATE("m4a"), NULL/*filename*/, OFN_HIDEREADONLY,
      NEVER_TRANSLATE("M4A Files(*.m4a)|*.m4a|All Files(*.*)|*.*||"));
   if (IDOK == dlg.DoModal())
      GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(dlg.m_ofn.lpstrFile);
}

