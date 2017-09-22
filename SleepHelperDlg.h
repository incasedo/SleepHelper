// SleepHelperDlg.h : header file
//// Author: Paul Xiao

#if !defined(AFX_SleepHelperDLG_H__F96B2364_EC17_4359_B825_8B6B07DA569A__INCLUDED_)
#define AFX_SleepHelperDLG_H__F96B2364_EC17_4359_B825_8B6B07DA569A__INCLUDED_

#include "sqlite./CppSQLite3.h"
#include "afx.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




/////////////////////////////////////////////////////////////////////////////
// CSleepHelperDlg dialog

class CSleepHelperDlg : public CDialog
{
// Construction
public:
	CSleepHelperDlg(CWnd* pParent = NULL);	// standard constructor



	CListCtrl	m_cListCtrl;

	int		m_nItems;
	int		m_nSelectItem;
	BOOL	m_bImage;
	bool     isreg;
    CButton m_start;
	CButton m_pause;


	HWND m_Video;
	BOOL Pause;
	//user information
	CString uname,SN,soundname,sleeptime,usecount,md5code,setusecount;
	DWORD volume,dtvol,dttime,rate;


	CString m_Path,dbfile;

	//sqlite connecter
    CppSQLite3DB db,db1;//sqlite 
	CppSQLite3Query re,re1;//sqlite Query data and also show results of inserts into auto-increment field
	

// Dialog Data
	//{{AFX_DATA(CSleepHelperDlg)
	enum { IDD = IDD_SleepHelper_DIALOG };
	CSliderCtrl	m_ctrlSlider;


		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSleepHelperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
    //ctrl list
	void InitListCtrlCols();
	void InsertItems(int j,int id,CString name);
	BOOL InitImageList();
	CImageList	m_cImageListNormal, m_cImageListSmall, m_cImageListState;
	LRESULT  OnDropFiles(WPARAM wParam,LPARAM lParam);

     CFileFind finder;


	BOOL m_bMinimized;
	void Reset();
	BOOL m_bStarted;
	BOOL m_bPaused;
	DWORD m_dwAction;
	HICON m_hIcon;

	HICON m_hAnimIcon[16], m_hBlankIcon;
	DWORD m_nIdxIcon;
	CBrush m_cBrush; 				

	NOTIFYICONDATA m_NotifyData;

	DWORD m_dwSec,m_dwMin;

	COleDateTime m_oleDateTime;

	// Generated message map functions
	//{{AFX_MSG(CSleepHelperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnRdoDoNothing();
	afx_msg void OnRdoSwitchOffTimer();
	afx_msg void OnRdoLogOff();
	afx_msg bool OnBtnApply();
	afx_msg void OnPopRestore();
	afx_msg void OnPopExit();
	afx_msg void OnBtnPause();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAction();
	afx_msg void OnOpenFile();
	afx_msg void Play(CString path,int vol);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void EnableBkImage( );
	afx_msg bool InsertSound(CString path);
	afx_msg void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPOPPlay();
	afx_msg void OnPOPDelete();
	afx_msg	void InsertIntoList();
	afx_msg CString GetFileNameInPath(CString name);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnMixerCtrlChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeEdtmincnt();
	afx_msg void Loop();
	afx_msg void OnRegNow();
	afx_msg void OnOnHelp();
	//}}AFX_MSG

	private:
	UINT m_nNumMixers;
	HMIXER m_hMixer;
	MIXERCAPS m_mxcaps;

	DWORD m_dwMinimum, m_dwMaximum;
	DWORD m_dwVolumeControlID;
	CString m_strDstLineName, m_strVolumeControlName;
	BOOL amdUninitialize();
	BOOL amdInitialize();
	BOOL amdGetMasterVolumeControl();
	BOOL amdGetMasterVolumeValue(DWORD &dwVal) const;
	BOOL amdSetMasterVolumeValue(DWORD dwVal) const;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SleepHelperDLG_H__F96B2364_EC17_4359_B825_8B6B07DA569A__INCLUDED_)
