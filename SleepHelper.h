// SleepHelper.h : main header file for the SleepHelper application
//

#if !defined(AFX_SleepHelper_H__D24A3CC0_2329_43D6_A125_2D390A838A76__INCLUDED_)
#define AFX_SleepHelper_H__D24A3CC0_2329_43D6_A125_2D390A838A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "sqlite./CppSQLite3.h"
/////////////////////////////////////////////////////////////////////////////
// CSleepHelperApp:
// See SleepHelper.cpp for the implementation of this class
//

class CSleepHelperApp : public CWinApp
{
public:
	CSleepHelperApp();
    CString m_sAppPath,md5code;

    CppSQLite3DB db,db1;//sqlite 
	CppSQLite3Query re,re1;//sqlite Query 



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSleepHelperApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSleepHelperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SleepHelper_H__D24A3CC0_2329_43D6_A125_2D390A838A76__INCLUDED_)
