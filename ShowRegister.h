#if !defined(AFX_SHOWREGISTER_H__E7D46362_9036_4566_A018_335295EA840A__INCLUDED_)
#define AFX_SHOWREGISTER_H__E7D46362_9036_4566_A018_335295EA840A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowRegister.h : header file
//
#include "sqlite./CppSQLite3.h"
#include "SleepHelperDlg.h"
/////////////////////////////////////////////////////////////////////////////
// ShowRegister dialog

class ShowRegister : public CDialog
{
// Construction
public:
	ShowRegister(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ShowRegister)
	enum { IDD = IDD_SN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	 CppSQLite3DB db;
	 CppSQLite3Query re;
	 CString    tempHash,uname,SN;
	 bool Register();
	CSleepHelperDlg sleepdlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ShowRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	private:

		CString Saveuname,SaveSN;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ShowRegister)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnOrder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWREGISTER_H__E7D46362_9036_4566_A018_335295EA840A__INCLUDED_)
