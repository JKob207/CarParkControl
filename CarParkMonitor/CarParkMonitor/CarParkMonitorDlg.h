
// CarParkMonitorDlg.h : header file
//

#pragma once


// CCarParkMonitorDlg dialog
class CCarParkMonitorDlg : public CDialogEx
{
// Construction
public:
	CCarParkMonitorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARPARKMONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_VideoName;
	afx_msg void OnBnClickedSelectvideo();
	afx_msg void OnBnClickedSelectspots();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_SaveToFile;
	CString m_FilePath;
	afx_msg void OnBnClickedSelectfile();
	CString m_FileToLoad;
	afx_msg void OnBnClickedSelectfiletoload();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelectoneplace();
	CString m_SavingName;
};
