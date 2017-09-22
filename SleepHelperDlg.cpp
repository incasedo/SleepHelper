
// RSA  
//Author Paul Xiao


#include "stdafx.h"
#include "SleepHelper.h"
#include "SleepHelperDlg.h"
#include "ShowRegister.h"
#include "vfw.h"
#include "./gui/Hyperlink.h"
#include "md5.h"
#include "sha2.h"
#include "url.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



HICON g_hBkupIcon;

const char *pszTrayNfMsg = "Tray Message";

#define STAT_RESETACTION		0xff00
#define STAT_RESETRADIOBTN		0x00ff

#define STAT_TIMERACTION		0x00f0
#define STAT_DATETIMEACTION		0x000f

#define STAT_SWOFF				0x0001
#define STAT_LOGOFF				0x0002
#define STAT_RESTART			0x0004

#define STAT_SWOFFTIMER			0x0010
#define STAT_LOGOFFTIMER		0x0020
#define STAT_RESTARTTIMER		0x0040

#define STAT_RDODONOTHING		0x0100
#define STAT_RDOLOGOFF			0x0200
#define STAT_RDOSWOFF			0x0400
#define STAT_RDORESTART			0x0800
#define STAT_RDOLOGOFFTIMER		0x1000
#define STAT_RDOSWOFFTIMER		0x2000
#define STAT_RDORESTARTTIMER	0x4000

/*
0x 0 0 0 0
| | | |
| | | \---- represents on specified date & time to switch off, log off & restart
| | \------ represent after specified min & sec to switch off, log off & restart
| \--\
|     > represents each combo boxes
\----/
*/

#define WM_FIREEVENT	(WM_USER+0x00f2)

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	CHyperLink	m_cEmail;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EMAIL, m_cEmail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSleepHelperDlg dialog

CSleepHelperDlg::CSleepHelperDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSleepHelperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSleepHelperDlg)
	// NOTE: the ClassWizard will add member initialization here
	m_nItems = 0;
	m_nSelectItem = 0;
	m_bImage = FALSE;
	m_hMixer = NULL;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	//iPlaying = PLAY;
}

void CSleepHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSleepHelperDlg)
	DDX_Control(pDX, IDC_SLIDER, m_ctrlSlider);
	DDX_Control(pDX, IDC_LIST_CTRL, m_cListCtrl);
	DDX_Control(pDX, IDC_BTNAPPLY, m_start);
	DDX_Control(pDX, IDC_BTNPAUSE, m_pause);
	
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSleepHelperDlg, CDialog)
//{{AFX_MSG_MAP(CSleepHelperDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_RDOLOGOFF, OnRdoLogOff)
ON_BN_CLICKED(IDC_BTNAPPLY, OnBtnApply)
ON_COMMAND(ID_POP_Restore, OnPopRestore)
ON_COMMAND(ID_POP_EXIT, OnPopExit)
ON_BN_CLICKED(IDC_BTNPAUSE, OnBtnPause)
ON_WM_TIMER()
ON_MESSAGE(WM_FIREEVENT, OnAction)
ON_BN_CLICKED(IDC_OpenFile, OnOpenFile)
ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
ON_NOTIFY(NM_RCLICK, IDC_LIST_CTRL, OnRclickListCtrl)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL, OnDblclkListCtrl)
ON_COMMAND(ID_POP_Play, OnPOPPlay)
ON_COMMAND(ID_POP_Delete, OnPOPDelete)
ON_WM_DESTROY()
ON_WM_HSCROLL()
ON_MESSAGE(MM_MIXM_CONTROL_CHANGE, OnMixerCtrlChange)
ON_EN_CHANGE(IDC_EDTMINCNT, OnChangeEdtmincnt)
	ON_BN_CLICKED(IDRegNow, OnRegNow)
ON_MESSAGE(WM_DROPFILES,OnDropFiles)
	ON_BN_CLICKED(IDOnHelp, OnOnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSleepHelperDlg message handlers

BOOL CSleepHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);

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
	
	for(int i=0,j=IDI_ICON1;i<15;i++,j++)
	{
		m_hAnimIcon[i] = AfxGetApp()->LoadIcon(j);
	}
	m_hBlankIcon = AfxGetApp()->LoadIcon(IDI_ICONBLANK);
	
	m_NotifyData.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyData.hIcon = m_hIcon;
	m_NotifyData.hWnd = this->m_hWnd;
	m_NotifyData.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;
    _tcscpy(m_NotifyData.szTip, "SleepHelper");
	m_NotifyData.uID = 1010;
	m_NotifyData.uCallbackMessage = RegisterWindowMessage(pszTrayNfMsg);
	
	GetDlgItem(IDC_EDTSECCNT)->SetWindowText("0");
	
	
	m_pause.EnableWindow(false);
	
	
	m_cBrush.CreateSysColorBrush(HS_CROSS);//4
	
	g_hBkupIcon = m_hIcon;
	
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPNMINCNT))->SetBuddy(GetDlgItem(IDC_EDTMINCNT));
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPNMINCNT))->SetRange(0,9999);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPNSECCNT))->SetBuddy(GetDlgItem(IDC_EDTSECCNT));
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPNSECCNT))->SetRange(0,59);
	
	
	
	Reset();
	
	//create list
	DWORD dwStyle = m_cListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_cListCtrl.SetExtendedStyle(dwStyle);
	
	//	m_cListCtrl.ModifyStyle(0, LVS_REPORT);
	
	// Setup the column headings
	InitListCtrlCols();
	
	// Create the image list that is attached to the list control
	InitImageList();
	
	// Insert the default dummy items     
	//show bg image
	EnableBkImage( );
	m_cListCtrl.DeleteAllItems();
	CSleepHelperApp *app=(CSleepHelperApp*)AfxGetApp(); 

	//init db
	//open db & read db data then insert into play list
    dbfile =  app->m_sAppPath+_T("\\d.db3");
	db.open(dbfile);
	//get user info
	
	CString sql = "select * from user where uid=1";
	re = db.query(sql); 	
	soundname = re.fieldValue("soundname");
	md5code = re.fieldValue("md5");
	
	sleeptime = re.fieldValue("sleeptime");
	GetDlgItem(IDC_EDTMINCNT)->SetWindowText(sleeptime);
	
	


	
	
	//get user use count number
	volume = re.getIntField("volume");
	
	
	
	
	re.finalize();
	
    InsertIntoList();
	
    
	//init Volume
	if (this->amdInitialize())
	{
		
		
		// get the Control ID and the names
		if (this->amdGetMasterVolumeControl())
		{
			
			// Initialize the slider control. For a volume control,
			// the range of acceptable values is 0 through 65,535.
			m_ctrlSlider.SetRange(m_dwMinimum, m_dwMaximum);
			
			m_ctrlSlider.SetPos(volume);
			VERIFY(this->amdSetMasterVolumeValue(volume) );
			
		}
		else
		{
			//VERIFY(m_strStat3.LoadString(IDS_NOCONTROL));
			// disable the slider
			m_ctrlSlider.EnableWindow(FALSE);
		}
		
		VERIFY(this->UpdateData(FALSE));
	}
	else
	{
		ASSERT(FALSE);
	}
	
	
    //show time	
	OnRdoSwitchOffTimer();

	//	AfxMessageBox(filename);

	
	
	//添加网站到收藏夹中
	HKEY   hKey; 
	if(RegOpenKey(HKEY_CURRENT_USER,   _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"),   &hKey)   !=   ERROR_SUCCESS)   
	  {   
	  TRACE0("Favorites   folder   not   found\n");   
	  }   
	  TCHAR   sz[MAX_PATH];   
	  TCHAR   szPath[MAX_PATH];   
	  DWORD   dwSize   =   sizeof(sz);   
	  //得到   Favorites   环境变量   
	  RegQueryValueEx(hKey,   _T("Favorites"),   NULL,   NULL,   (LPBYTE)sz,   &dwSize);   
	  //得到真实的路径   
	  ExpandEnvironmentStrings(sz,   szPath,   MAX_PATH);   
	  RegCloseKey(hKey);   
	  //开始写   URL   参数   
	  CString   strPath   =   szPath;   
	  //假设它显示的名字为   OWDN     
	  strPath   =   strPath   +   "\\酷兔英语免费在线学习平台.url";   
	  TCHAR   buffer[2048];   
	  //其指向   http://www.owdn.com   这个位置   
	  strcpy(buffer,   "http://www.qeto.com");   
	  WritePrivateProfileString(_T("InternetShortcut"),   _T("URL"),   buffer,   strPath); 
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CSleepHelperDlg::OnDestroy() 
{
	VERIFY(this->amdUninitialize());
	
	CDialog::OnDestroy();
}
void CSleepHelperDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// set the Master Volume value according to the slider control
	int tmp;
	if (pScrollBar != NULL)
	{
		// This function should be called only if the control exists.
		ASSERT(m_ctrlSlider.IsWindowEnabled());
		tmp = reinterpret_cast<CSliderCtrl *>(pScrollBar)->GetPos();
		VERIFY(this->amdSetMasterVolumeValue(tmp) );
	}
	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
LRESULT CSleepHelperDlg::OnMixerCtrlChange(WPARAM wParam, LPARAM lParam)
{
	if (reinterpret_cast<HMIXER>(wParam) == m_hMixer &&
		static_cast<DWORD>(lParam) == m_dwVolumeControlID)
	{
		// This function should be called only if the control exists.
		ASSERT(m_ctrlSlider.IsWindowEnabled());
		
		// The state of the Master Volume control has changed. Refresh it.
		DWORD dwVal = 0;
		if (this->amdGetMasterVolumeValue(dwVal))
		{
			m_ctrlSlider.SetPos(dwVal);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	
	return 0;
}

BOOL CSleepHelperDlg::amdInitialize()
{
	ASSERT(m_hMixer == NULL);
	
	// get the number of mixer devices present in the system
	m_nNumMixers = ::mixerGetNumDevs();
	
	m_hMixer = NULL;
	::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
	
	m_strDstLineName.Empty();
	m_strVolumeControlName.Empty();
	m_dwMinimum = 0;
	m_dwMaximum = 0;
	m_dwVolumeControlID = 0;
	
	// open the first mixer
	// A "mapper" for audio mixer devices does not currently exist.
	if (m_nNumMixers != 0)
	{
		if (::mixerOpen(&m_hMixer,
			0,
			reinterpret_cast<DWORD>(this->GetSafeHwnd()),
			NULL,
			MIXER_OBJECTF_MIXER | CALLBACK_WINDOW)
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}
		
		if (::mixerGetDevCaps(reinterpret_cast<UINT>(m_hMixer),
			&m_mxcaps, sizeof(MIXERCAPS))
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}
BOOL CSleepHelperDlg::amdUninitialize()
{
	BOOL bSucc = TRUE;
	
	if (m_hMixer != NULL)
	{
		bSucc = (::mixerClose(m_hMixer) == MMSYSERR_NOERROR);
		m_hMixer = NULL;
	}
	
	return bSucc;
}

BOOL CSleepHelperDlg::amdGetMasterVolumeControl()
{
	if (m_hMixer == NULL)
	{
		return FALSE;
	}
	
	// get dwLineID
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (::mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(m_hMixer),
		&mxl,
		MIXER_OBJECTF_HMIXER |
		MIXER_GETLINEINFOF_COMPONENTTYPE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	
	// get dwControlID
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (::mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer),
		&mxlc,
		MIXER_OBJECTF_HMIXER |
		MIXER_GETLINECONTROLSF_ONEBYTYPE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	
	// store dwControlID
	m_strDstLineName = mxl.szName;
	m_strVolumeControlName = mxc.szName;
	m_dwMinimum = mxc.Bounds.dwMinimum;
	m_dwMaximum = mxc.Bounds.dwMaximum;
	m_dwVolumeControlID = mxc.dwControlID;
	
	return TRUE;
}
BOOL CSleepHelperDlg::amdGetMasterVolumeValue(DWORD &dwVal) const
{
	if (m_hMixer == NULL)
	{
		return FALSE;
	}
	
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	if (::mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
		&mxcd,
		MIXER_OBJECTF_HMIXER |
		MIXER_GETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	
	dwVal = mxcdVolume.dwValue;
	
	return TRUE;
}
BOOL CSleepHelperDlg::amdSetMasterVolumeValue(DWORD dwVal) const
{
	if (m_hMixer == NULL)
	{
		return FALSE;
	}
	
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume = { dwVal };
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	if (::mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
		&mxcd,
		MIXER_OBJECTF_HMIXER |
		MIXER_SETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	
	return TRUE;
}

void CSleepHelperDlg::InsertIntoList()
{
	CString sql = "select * from nlist order by Type desc";
	re = db.query(sql); 	
    int k=1;
	while (!re.eof())
    {
		k++;
		InsertItems(k,atoi(re.fieldValue("id")),re.fieldValue("vname"));	
		//	AfxMessageBox(re.fieldValue("id"));
        re.nextRow();
	}
	
	re.finalize();
	
}
CString CSleepHelperDlg::GetFileNameInPath(CString name)
{
	CString sql = "select * from nlist where vname='"+name+"' ";
	re = db.query(sql); 	
    CString result; 
	result = re.fieldValue("path");
	re.finalize();
	
	//check file wether exist.
	if( !finder.FindFile(result) ) 
	{	  
		CString strPath,m_sFile; 	
		TCHAR exeFullPath[MAX_PATH]; 
		GetModuleFileName(NULL,exeFullPath,MAX_PATH); 
		strPath=(CString)exeFullPath; 
		int position=strPath.ReverseFind('\\'); 
		strPath=strPath.Left(position+1); 	
		result = strPath +"\\sound\\" + name +".mp3";
	}
	//AfxMessageBox(result);
    return result;
}

void CSleepHelperDlg::Play(CString path,int vol)
{
	m_Video = NULL;
	if(m_Video == NULL)
	{
	/*The MCIWndCreate function registers the MCIWnd window class 
		and creates an MCIWnd window for using MCI services. */
		
		
		m_Video = MCIWndCreate(this->GetSafeHwnd(),
			AfxGetInstanceHandle(),
			WS_CHILD | MCIWNDF_NOMENU,path);
		
	}
	else
	{
		MCIWndHome(m_Video);		//go to the start
	}
	MCIWndPlay(m_Video);
	Pause = FALSE;
	
	MCIWndSetVolume(m_Video,vol) ;
	
	
	
	
}
void CSleepHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
		if (nID == SC_MINIMIZE) 
		{
			Shell_NotifyIcon(NIM_ADD,&m_NotifyData);
			ShowWindow(SW_HIDE);
			m_bMinimized = true;
		}
		else if (nID == SC_RESTORE)
		{
			Shell_NotifyIcon(NIM_DELETE,&m_NotifyData);
			m_bMinimized = false;
		}
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSleepHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
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
		CDialog::OnPaint();
	}
	
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSleepHelperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSleepHelperDlg::OnCancel() 
{
/*
if(MessageBox("Do you want to exit ?","SleepHelper",MB_YESNO|MB_ICONQUESTION) == IDNO)
return;
	*/
	GetDlgItem(IDC_FRAOPT)->SetWindowPos(NULL,11,0,457-11,242-0,SWP_SHOWWINDOW);
    int tmp;
	tmp = m_ctrlSlider.GetPos();
	CString tmp1;
	tmp1.Format("%d",tmp);
	
	
	CString sql = "update user set volume='"+ tmp1 +"' where uid=1";
	db.query(sql); 	

	Shell_NotifyIcon(NIM_DELETE,&m_NotifyData);
	CDialog::OnCancel();
}

/// need to handle tray icon
LRESULT CSleepHelperDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMenu cMenu, *pSubMenu;
	if(message == m_NotifyData.uCallbackMessage)
	{
		if (LOWORD(lParam) == WM_RBUTTONUP)
		{    
			if (!cMenu.LoadMenu(IDR_MNUTRAY)) return 0;
			if (!(pSubMenu = cMenu.GetSubMenu(0))) return 0;
			
			// Make chosen menu item the default (bold font)
			//::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);
			
			// Display and track the popup menu
			CPoint pos;
			GetCursorPos(&pos);
			
			SetForegroundWindow();  
			::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, GetSafeHwnd(), NULL);
			
			// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
			PostMessage(WM_NULL, 0, 0);
			
			cMenu.DestroyMenu();
		} 
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
		{
			// double click received, the default action is to execute default menu item
			SetForegroundWindow();  
			
			UINT uItem;
			if (!cMenu.LoadMenu(IDR_MNUTRAY)) return 0;
			if (!(pSubMenu = cMenu.GetSubMenu(0))) return 0;
			uItem = pSubMenu->GetMenuItemID(0);
			
			SendMessage(WM_COMMAND, uItem, 0);
			
			cMenu.DestroyMenu();
		}
		
	}
	return CDialog::WindowProc(message, wParam, lParam);	
}

// when DoNothing radio button clicked
void CSleepHelperDlg::OnRdoDoNothing() 
{
	GetDlgItem(IDC_EDTMINCNT)->ShowWindow(false);
	GetDlgItem(IDC_SPNMINCNT)->ShowWindow(false);
	GetDlgItem(IDC_LBLMIN)->ShowWindow(false);
	GetDlgItem(IDC_EDTSECCNT)->ShowWindow(false);
	GetDlgItem(IDC_SPNSECCNT)->ShowWindow(false);
	GetDlgItem(IDC_LBLSEC)->ShowWindow(false);
	
	
	
	if(!(m_dwAction&STAT_RDODONOTHING))
		Reset();
	
	m_dwAction &= STAT_RESETRADIOBTN;
	m_dwAction |= STAT_RDODONOTHING;
}


// when LogOff radio button clicked
void CSleepHelperDlg::OnRdoLogOff() 
{
	GetDlgItem(IDC_EDTMINCNT)->ShowWindow(false);
	GetDlgItem(IDC_SPNMINCNT)->ShowWindow(false);
	GetDlgItem(IDC_LBLMIN)->ShowWindow(false);
	GetDlgItem(IDC_EDTSECCNT)->ShowWindow(false);
	GetDlgItem(IDC_SPNSECCNT)->ShowWindow(false);
	GetDlgItem(IDC_LBLSEC)->ShowWindow(false);
	
	
	if(!(m_dwAction&STAT_RDOLOGOFF))
		Reset();
	
	m_dwAction &= STAT_RESETRADIOBTN;
	m_dwAction |= STAT_RDOLOGOFF;
}



// when SwitchOffAfter radio button clicked
void CSleepHelperDlg::OnRdoSwitchOffTimer() 
{
	
	
	
	if(!(m_dwAction&STAT_RDOSWOFFTIMER))
		Reset();
	
	m_dwAction &= STAT_RESETRADIOBTN;
	m_dwAction |= STAT_RDOSWOFFTIMER;
}




// when apply button clicked
bool CSleepHelperDlg::OnBtnApply() 
{
	
	m_dwAction &= STAT_RESETACTION;
	m_dwAction |= STAT_SWOFFTIMER;
	
	CString cszSec,cszMin,SaveSleepTime;
	
	GetDlgItem(IDC_EDTSECCNT)->GetWindowText(cszSec);
	GetDlgItem(IDC_EDTMINCNT)->GetWindowText(cszMin);
	m_dwSec = atol(cszSec.GetBuffer(0));
	m_dwMin = atol(cszMin.GetBuffer(0));
	
	CSleepHelperApp *app=(CSleepHelperApp*)AfxGetApp(); 




	SetTimer(101,1000,0); // for action
	SetTimer(102,30,0); // for animated icon
	
	m_bMinimized = m_bStarted = true;
	
	// after auccessful apply minimize the dialog



	Shell_NotifyIcon(NIM_ADD,&m_NotifyData);
	m_start.EnableWindow(false);
	m_pause.EnableWindow(true);
	
	
	GetDlgItem(IDC_EDTMINCNT)->GetWindowText(SaveSleepTime);
	CString sql = "update user set sleeptime='"+ SaveSleepTime +"' where uid=1";
	db.query(sql); 
	dtvol = m_ctrlSlider.GetPos();
	dttime = atoi(SaveSleepTime)*60;
	if (dttime ==0) dttime=1;
	rate = dtvol/dttime;
	
	//Starting Play sound
	sql = "select * from user where uid=1";
	re = db.query(sql); 	
	soundname = re.fieldValue("soundname");
	re.finalize();
	CString pString = GetFileNameInPath(soundname);//get path	
	MCIWndStop(m_Video);
	Play(pString,1000);
	//SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);

	return true;
}

// on the tay popup menu user cliecked restore
void CSleepHelperDlg::OnPopRestore() 
{
	ShowWindow(SW_SHOWNORMAL);
	m_bMinimized = false;
	Shell_NotifyIcon(NIM_DELETE,&m_NotifyData);
}

// on the tay popup menu user cliecked exit
void CSleepHelperDlg::OnPopExit() 
{
	PostQuitMessage(0);
}

// when pause button clicked 
void CSleepHelperDlg::OnBtnPause() 
{
	if(m_dwAction&STAT_TIMERACTION)
	{
		// if not paused, then pause and chamge the text to "Go"
		KillTimer(101);
		KillTimer(102);
		if(m_bMinimized)
		{
			m_NotifyData.hIcon = m_hIcon;
			_tcscpy(m_NotifyData.szTip, "SleepHelper");
			
			Shell_NotifyIcon(NIM_MODIFY,&m_NotifyData);
		}
		
		m_hIcon = m_hAnimIcon[m_nIdxIcon];
		
		
		m_bPaused = true;
		m_bStarted = false;
		m_pause.EnableWindow(false);
		m_start.EnableWindow(true);
		MCIWndPause(m_Video);
	}
}

// this is the timer starts ticking when applied
//    this also makes the icon blink
void CSleepHelperDlg::OnTimer(UINT nIDEvent) 
{/// two timer event comes here
	
	if(nIDEvent == 101)
	{// this event checks the time to perform 
		//    the action such like switch off, restart or log off
		//加入动态音量
		if (dtvol>0) {
			dtvol -= rate;
			m_ctrlSlider.SetPos(dtvol);
			ASSERT(m_ctrlSlider.IsWindowEnabled());
			VERIFY(this->amdSetMasterVolumeValue(dtvol));
		}
		//AfxMessageBox("dd");
		if(m_dwAction&STAT_TIMERACTION)
		{// execution comes here when minute or second specified
			CString cszSec, cszMin;
			
			if(m_dwSec <=0)
			{
				m_dwSec = 59;			
				if(m_dwMin <=0)
				{// now timer execution finished
					//   time to perform the action
					
					KillTimer(101);
					KillTimer(102);
					if(m_bMinimized)
					{
						m_NotifyData.hIcon = m_hIcon;
						_tcscpy(m_NotifyData.szTip, "SleepHelper");
						
						Shell_NotifyIcon(NIM_MODIFY,&m_NotifyData);
					}
					else
					{
						//GetDlgItem(IDC_FRAOPT)->GetDC()->FillRect(CRect(411,8,443,40),&m_cBrush);
						//GetDlgItem(IDC_FRAOPT)->GetDC()->DrawIcon(411,8,m_hIcon);
					}
					// sends a message to execute OnAction
					//   which will do switch off, restart or log off
					SendMessage(WM_FIREEVENT,0,0);
					m_dwSec = 0;
				}
				else
					m_dwMin--;
			}
			else
			{
				m_dwSec--;
			}
			
			cszSec.Format("%d",m_dwSec);
			cszMin.Format("%d",m_dwMin);
			
			GetDlgItem(IDC_EDTSECCNT)->SetWindowText(cszSec);
			GetDlgItem(IDC_EDTMINCNT)->SetWindowText(cszMin);
		}
		else
		{// execution comes here when date specified
			COleDateTime curTime = COleDateTime::GetCurrentTime();
			
			if(curTime >= m_oleDateTime)
			{// Now the time has come
				//  now the time to stop the timer and perform the action
				KillTimer(101);
				KillTimer(102);
				
				if(m_bMinimized)
				{
					m_NotifyData.hIcon = m_hIcon;
					_tcscpy(m_NotifyData.szTip, "SleepHelper");
					
					Shell_NotifyIcon(NIM_MODIFY,&m_NotifyData);
				}
				else
				{
					//GetDlgItem(IDC_FRAOPT)->GetDC()->FillRect(CRect(411,8,443,40),&m_cBrush);
					//GetDlgItem(IDC_FRAOPT)->GetDC()->DrawIcon(411,8,m_hIcon);
				}
				
				// sends a message to execute OnAction
				//   which will do switch off, restart or log off
				SendMessage(WM_FIREEVENT,0,0);
				
			}
		}
	}
	else if(nIDEvent == 102)
	{// this is the secon timer
		//   blinks the icon and displays the tool tip on the tray
		
		//whether loop
		char lp[3];
		long lMode;	
		lMode=MCIWndGetMode(m_Video,lp,sizeof(lp));
		if (lMode==MCI_MODE_STOP)
			::SendMessage(m_Video, MCI_PLAY, 0, 0);
		
		if(m_bMinimized)
		{
			if(m_bStarted)
			{
				m_NotifyData.hIcon = m_hAnimIcon[m_nIdxIcon];
				m_nIdxIcon++;
				if(m_nIdxIcon >=14)
					m_nIdxIcon = 0;
				
				if(m_dwAction&STAT_TIMERACTION)
				{
					sprintf(m_NotifyData.szTip,"Min: %d, Sec: %d",m_dwMin,m_dwSec);
				}
				else
				{
					strcpy(m_NotifyData.szTip,m_oleDateTime.Format(_T("%A, %B %d, %Y, %H:%M:%S")).GetBuffer(0));
				}
			}
			else
				m_NotifyData.hIcon = m_hIcon;
			Shell_NotifyIcon(NIM_MODIFY,&m_NotifyData);
		}
		else
		{
			if(m_bStarted)
			{
				m_nIdxIcon++;
				if(m_nIdxIcon >=14)
					m_nIdxIcon = 0;
				
				//GetDlgItem(IDC_FRAOPT)->GetDC()->FillRect(CRect(411,8,443,40),&m_cBrush);
				//	GetDlgItem(IDC_FRAOPT)->GetDC()->DrawIcon(411,8,m_hAnimIcon[m_nIdxIcon]);
			}
			else
			{
				//GetDlgItem(IDC_FRAOPT)->GetDC()->FillRect(CRect(411,8,443,40),&m_cBrush);
				//GetDlgItem(IDC_FRAOPT)->GetDC()->DrawIcon(411,8,m_hIcon);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

// resets any action going
//  if the user changes the combo the on going action will be reset
void CSleepHelperDlg::Reset()
{
	if(m_bPaused)
	{
		m_bPaused = false;
		GetDlgItem(IDC_BTNPAUSE)->SetWindowText("Pause");
	}
	m_dwAction &= STAT_RESETACTION;
	m_bStarted = false;
	
	// kill the timers
	KillTimer(101);
	KillTimer(102);
	
	m_NotifyData.hIcon = m_hIcon;
	_tcscpy(m_NotifyData.szTip, "SleepHelper");
	
	if(m_bMinimized)
	{
		Shell_NotifyIcon(NIM_MODIFY,&m_NotifyData);
	}
	
	m_hIcon = g_hBkupIcon;
	
	// incase the dialog is minimized restore it
	m_bMinimized = false;
	m_nIdxIcon = 0;
	m_NotifyData.hIcon = m_hIcon;
}

void CSleepHelperDlg::OnAction()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{// open and check the privileges for to perform the actions
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
		
		tkp.PrivilegeCount = 1; 
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
		
		if(AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		{/// adjust the privilege to perform the action
			if(m_dwAction&STAT_SWOFF)
				ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF|EWX_FORCE,0);
			else if(m_dwAction&STAT_SWOFFTIMER)
				ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF|EWX_FORCE,0);
			else if(m_dwAction&STAT_LOGOFF)
				ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
			else if(m_dwAction&STAT_LOGOFFTIMER)
				ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
			else if(m_dwAction&STAT_RESTART)
				ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
			else if(m_dwAction&STAT_RESTARTTIMER)
				ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
			
		}
	}
	
	int err = GetLastError();
	if(err)
	{// if any error occurs the report to the user
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL,err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox((LPTSTR)lpMsgBuf, "SleepHelper", MB_OK|MB_ICONERROR);
		LocalFree(lpMsgBuf);
	}
	
	// make sure you quit after issueing any commands
	PostQuitMessage(0);
	exit(0);
}
BOOL CSleepHelperDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE)
			return TRUE;
		if (pMsg->wParam==VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CSleepHelperDlg::OnOpenFile() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[]= _T("MP3 File (*.mp3)|*.mp3|");
	CFileDialog MediaFile(TRUE, _T("MP3 File"), _T("*.mp3"),
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES,
		szFilter);
	
	TCHAR szTitle[]=_T("Open File");
	MediaFile.m_ofn.lpstrTitle=szTitle;
	
	int iResult;
	iResult=MediaFile.DoModal();
	if (iResult==IDOK)
	{	
		
		InsertSound(MediaFile.GetPathName());	
	}	
}

void CSleepHelperDlg::EnableBkImage( )
{
	
	
/* 
There are two ways to include a background image:
(1) Specify a valid filepath and load the bitmap handle from there
(2) Specify a valid URL using the res:protocol (preferred).

		(2) Is preferable because you don't need to include a separate file 
		with your application.  You can embed the BMP in the resource fork
		and use the RES: protocol to have the ListControl load it.  See
		http://www.microsoft.com/mind/0199/cutting/cutting0199.asp for more
		information about the RES: syntax.  This idea was suggested by Nick Hodapp
		(http://www.codetools.com/script/Articles/list_articles.asp?userid=162)
		
		  Both methods are included for completeness, but the sample
		  application uses the embedded BMP.  Note that specifying a HBITMAP
		  in the struct is not currently supported.
		  
			
			  // Load the resource and apply it to the background
			  TCHAR szBuffer[_MAX_PATH]; 
			  VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
			  CString sPath;
			  sPath.Format(_T("res://%s/#2/#151"),szBuffer);
			  
				LVBKIMAGE bki;
				bki.ulFlags = LVBKIF_STYLE_TILE | LVBKIF_SOURCE_URL ;
				bki.pszImage = sPath.GetBuffer(sPath.GetLength());
				bki.cchImageMax = sPath.GetLength();
				VERIFY(m_cListCtrl.SetBkImage( &bki));
				
	*/
	
	
	/*
	Construct the path to the BMP.  Although the CListCtrl::SetBkImage docs
	indicate that you can use an HBITMAP, the latest docs on the LVBKIMAGE
	underlying struct indicates that the LVBKIMAGE.hbm member is not used.  
	*/
	AfxOleInit();
	TCHAR szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	CString sPath = (CString)szBuffer;
	sPath = sPath.Left(sPath.ReverseFind('\\') + 1);	
	sPath += "bk.bmp";
	//	AfxMessageBox(sPath);
	m_cListCtrl.SetBkImage( sPath.GetBuffer(sPath.GetLength()), TRUE);
	sPath.ReleaseBuffer();
	
	
	
}

/////////////////////////////////////////////////////////////////////////////
/*
InitListCtrlCols

  This is called when we first create the list control as well as part of the
  RecreateListControl(...) call.  The reason is that when we destroy
  and then create the list control in response to changing list control
  styles we lose the column information.
  
	Params
	None
	
	  Returns 
	  void
*/
/////////////////////////////////////////////////////////////////////////////
void CSleepHelperDlg::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	m_cListCtrl.GetClientRect(&rect);
	int nColInterval = rect.Width()/3;
	
	m_cListCtrl.InsertColumn(0, _T("ICON"), LVCFMT_LEFT, 40);
	m_cListCtrl.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 110);
	//m_cListCtrl.InsertColumn(2, _T("Description"), LVCFMT_LEFT, 270);
	
}

BOOL CSleepHelperDlg::InitImageList()
{
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(32,32, ILC_COLOR8 |ILC_MASK , 8, 1);
	m_cImageListNormal.Attach(hList);
	
	hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_cImageListSmall.Attach(hList);
	
	
	// Load the large icons
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_IMAGES_NORMAL);
	m_cImageListNormal.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();
	
	// Load the small icons
	cBmp.LoadBitmap(IDB_IMAGES_SMALL);
	m_cImageListSmall.Add(&cBmp, RGB(255,0, 255));
	
	// Attach them
	m_cListCtrl.SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	m_cListCtrl.SetImageList(&m_cImageListSmall, LVSIL_SMALL);
	
	return TRUE;
	
}

void CSleepHelperDlg::InsertItems(int j,int id,CString name)
{
	// Delete the current contents
	
	
	// Use the LV_ITEM structure to insert the items
	LVITEM lvi;
	CString strItem;
	
	// Insert the first item
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	strItem = name;
	lvi.iItem = id;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	lvi.iImage = j%8;		// There are 8 images in the image list
	m_cListCtrl.InsertItem(&lvi);
	
	
}

bool CSleepHelperDlg::InsertSound(CString path){
	
	//parse path
	//得到纯文件名（不带路径） strFilename
	CString strFilename(path);
	strFilename=strFilename.Right(strFilename.GetLength() - strFilename.ReverseFind('\\') - 1);
    //得到文件名
	int nlen=strFilename.ReverseFind('.');
	strFilename = strFilename.Left(nlen);
	//AfxMessageBox(strFilename);
	
	
	//insert into db
	
	
	CString sql;
	sql = "insert into nlist (vname,path) values ('"+strFilename+"','"+path+"')";
	re = db.query(sql); 	
	
	re.finalize();
	
	LVITEM lvi;
	CString strItem;
	// Insert the first item
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	
	strItem = strFilename;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	lvi.iImage = 0;		
	m_cListCtrl.InsertItem(&lvi);
	
	
	return 0;
}

void CSleepHelperDlg::OnRclickListCtrl(NMHDR* pNMHDR, LRESULT* pResult){
	
	// CListCtrl &CList = GetListCtrl();//获取当前列表控件的指针
	CMenu menu, * pSubMenu;//定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MNUTRAY);//装载自定义的右键菜单
	pSubMenu = menu.GetSubMenu(0);//获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CPoint oPoint;//定义一个用于确定光标位置的位置
	GetCursorPos( &oPoint);//获取当前光标的位置，以便使得菜单可以跟随光标
	int istat=m_cListCtrl.GetSelectionMark();//用istat存放当前选定的是第几项
	CString pString =m_cListCtrl.GetItemText(istat,0);//获取当前项中的数据，0代表是第0列
	pString="您选择的路径是:"+pString ;//显示当前选择项
	//MessageBox(pString);//显示当前选中的路径
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this); //在指定位置显示弹出菜单
}

void CSleepHelperDlg::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult){
	
	
	
}

void CSleepHelperDlg::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	OnPOPPlay() ;
	//AfxMessageBox(pString);
	
	*pResult = 0;
}



void CSleepHelperDlg::OnPOPPlay() 
{
	// TODO: Add your command handler code here
	int istat=m_cListCtrl.GetSelectionMark();//用istat存放当前选定的是第几项
	CString pString =m_cListCtrl.GetItemText(istat,0);//获取当前项中的数据，0代表是第0列
	//写入状态库
	CString sql = "update user set soundname='"+pString+"' where uid=1";
	db.query(sql); 	
	
	if (!pString.IsEmpty()) {
		SetTimer(102,30,0);
		pString = GetFileNameInPath(pString);//get path			
		
		MCIWndStop(m_Video);
		Play(pString,1000);

	}
}

void CSleepHelperDlg::OnPOPDelete() 
{
	// TODO: Add your command handler code here
	
	
	int istat1=m_cListCtrl.GetSelectionMark();//用istat存放当前选定的是第几项
	CString pString1 =m_cListCtrl.GetItemText(istat1,0);//获取当前项中的数据，0代表是第0列
	//	pString="您选择的路径是:"+pString ;//显示当前选择项
	
	CString sql;
	sql = "delete from nlist where vname='"+pString1+"' ";
	db.query(sql); 	
	
   	UpdateData(TRUE);
	
	m_cListCtrl.DeleteAllItems();
	InsertIntoList();
    m_cListCtrl.SetRedraw(TRUE);
	
    
	//MessageBox(pString);//显示当前选中的路径
}
LRESULT CSleepHelperDlg::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	char	szDroppedFile[MAX_PATH];
	HDROP	hDrop ;
	int nFiles;
	CString csString;
	
	hDrop = (HDROP)wParam;
	UpdateData();
	nFiles = DragQueryFile(hDrop,		// Struture Identifier
		0,			// -1 to Drop more than one file
		szDroppedFile,// Droped File Name
		MAX_PATH);
	
    //get programe path
	CString strPath,m_sFile; 	
	TCHAR exeFullPath[MAX_PATH]; 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH); 
    strPath=(CString)exeFullPath; 
    int position=strPath.ReverseFind('\\'); 
    strPath=strPath.Left(position+1); 	
	
	//Get source file path
	
	CString strFilename(szDroppedFile);
	
	strFilename=strFilename.Right(strFilename.GetLength() - strFilename.ReverseFind('\\') - 1);
    //得到扩搌名
	
	CString exp = strFilename.Right(3);
	exp.MakeUpper();
	if (exp != "MP3") {
		AfxMessageBox("Sorry,The file's format must be MP3");
		return 0;
		
	}
	
	//insert into db
	int nlen=strFilename.ReverseFind('.');
	strFilename = strFilename.Left(nlen);
	CString sql;
	sql = "insert into nlist (vname,path) values ('"+strFilename+"','"+szDroppedFile+"')";
	re = db.query(sql); 	
	
	re.finalize();
	
	LVITEM lvi;
	CString strItem;
	// Insert the first item
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	
	strItem = strFilename;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	lvi.iImage = 0;		
	m_cListCtrl.InsertItem(&lvi);
	
	
	
	//	csString  = "Drop file to sound list was succeed! ";
	
	//AfxMessageBox(csString);
	
	return 0;
	
}

void CSleepHelperDlg::OnChangeEdtmincnt() 
{
	
	
}

void CSleepHelperDlg::OnRegNow() 
{
	// TODO: Add your control notification handler code here
	CString infoOrderURL;
	infoOrderURL.LoadString(IDS_OrderURL);
	//CURL url;
//	url.Open(infoOrderURL);	

	::ShellExecute(NULL, NULL, _T(infoOrderURL), NULL, NULL, SW_SHOWNORMAL);
	   // ShowRegister dlg;	
	//	dlg.DoModal();

}

void CSleepHelperDlg::OnOnHelp() 
{
	CSleepHelperApp *app=(CSleepHelperApp*)AfxGetApp(); 
    CString helpfile =  app->m_sAppPath+_T("\\help.chm");
	HtmlHelp(NULL, (LPCSTR)helpfile, HH_DISPLAY_TOPIC, 0);
}
