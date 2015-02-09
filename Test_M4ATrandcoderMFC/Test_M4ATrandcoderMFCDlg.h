
// Test_M4ATrandcoderMFCDlg.h : header file
//

#pragma once


// CTest_M4ATrandcoderMFCDlg dialog
class CWaveToM4A;
class CTest_M4ATrandcoderMFCDlg : public CDialogEx
{
// Construction
public:
	CTest_M4ATrandcoderMFCDlg(CWnd* pParent = NULL);	// standard constructor
   ~CTest_M4ATrandcoderMFCDlg();

// Dialog Data
	enum { IDD = IDD_TEST_M4ATRANDCODERMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
   void OnTimer(UINT_PTR nIDEvent);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedGetFormats();
   afx_msg void OnCbnSelchangeComboFormats();
   afx_msg void OnBnClickedBtnTranscode();
   afx_msg void OnBnClickedBtnCanceltranscode();
   afx_msg LRESULT OnTranscodeCompleted(WPARAM wparam, LPARAM lparam);

protected:
   void CreateTranscoderIfNecessary();

protected:
   CWaveToM4A* m_pTranscoder;
   UINT_PTR m_nIDEvent;
   ITaskbarList3* m_pTaskbar;
public:
   afx_msg void OnBnClickedBtnBrowseInput();
   afx_msg void OnBnClickedBtnBrowseOutput();
};
