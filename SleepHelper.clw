; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=ShowRegister
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SleepHelper.h"

ClassCount=5
Class1=CSleepHelperApp
Class2=CSleepHelperDlg
Class3=CAboutDlg

ResourceCount=11
Resource1=IDD_SleepHelper_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_SN (English (U.S.))
Resource4=IDR_MNUTRAY
Resource5=IDD_ABOUTBOX
Resource6=IDR_MENUOP (English (U.K.))
Resource7=FileOpen
Class4=SoundFileOpenDlg
Class5=ShowRegister
Resource8=IDD_ABOUTBOX (English (U.S.))
Resource9=FileOpen (English (U.S.))
Resource10=IDD_SleepHelper_DIALOG (English (U.S.))
Resource11=IDR_MNUTRAY (English (U.S.))

[CLS:CSleepHelperApp]
Type=0
HeaderFile=SleepHelper.h
ImplementationFile=SleepHelper.cpp
Filter=N

[CLS:CSleepHelperDlg]
Type=0
HeaderFile=SleepHelperDlg.h
ImplementationFile=SleepHelperDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=AFX_ID_Delete

[CLS:CAboutDlg]
Type=0
HeaderFile=SleepHelperDlg.h
ImplementationFile=SleepHelperDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_EMAIL,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_Staic,static,1342308352

[DLG:IDD_SleepHelper_DIALOG]
Type=1
Class=CSleepHelperDlg
ControlCount=15
Control1=IDC_EDTMINCNT,edit,1350639746
Control2=IDC_SPNMINCNT,msctls_updown32,1342177322
Control3=IDC_LBLMIN,static,1342177280
Control4=IDC_EDTSECCNT,edit,1350639746
Control5=IDC_SPNSECCNT,msctls_updown32,1342177322
Control6=IDC_LBLSEC,static,1342177280
Control7=IDC_BTNPAUSE,button,1342242816
Control8=IDC_BTNAPPLY,button,1342242817
Control9=IDRegNow,button,1342242816
Control10=IDC_FRAOPT,button,1342177287
Control11=IDC_SLIDER,msctls_trackbar32,1342242840
Control12=IDC_OpenFile,button,1342242817
Control13=IDC_LIST_CTRL,SysListView32,1350647824
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352

[MNU:IDR_MNUTRAY]
Type=1
Class=CSleepHelperDlg
Command1=ID_POP_RESTORE
Command2=ID_POP_EXIT
CommandCount=2

[MNU:IDR_MNUTRAY (English (U.S.))]
Type=1
Class=CSleepHelperDlg
Command1=ID_POP_Restore
Command2=ID_POP_Play
Command3=ID_POP_Delete
Command4=ID_POP_EXIT
CommandCount=4

[CLS:SoundFileOpenDlg]
Type=0
HeaderFile=SoundFileOpenDlg.h
ImplementationFile=SoundFileOpenDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=SoundFileOpenDlg
VirtualFilter=dWC

[MNU:IDR_MENUOP (English (U.K.))]
Type=1
Class=?
Command1=AFX_ID_Delete
Command2=AFX_ID_Play
CommandCount=2

[CLS:ShowRegister]
Type=0
HeaderFile=ShowRegister.h
ImplementationFile=ShowRegister.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:FileOpen]
Type=1
Class=SoundFileOpenDlg
ControlCount=12
Control1=65535,static,1342308352
Control2=1088,static,1342308480
Control3=1121,listbox,1352732755
Control4=1089,static,1342308352
Control5=1136,combobox,1344340035
Control6=1091,static,1342308352
Control7=1137,combobox,1344340819
Control8=IDOK,button,1342373889
Control9=IDCANCEL,button,1342373888
Control10=1090,static,1342308352
Control11=1152,edit,1350632576
Control12=1120,listbox,1352732755

[DLG:FileOpen (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=65535,static,1342308352
Control2=1088,static,1342308480
Control3=1121,listbox,1352732755
Control4=1089,static,1342308352
Control5=1136,combobox,1344340035
Control6=1091,static,1342308352
Control7=1137,combobox,1344340819
Control8=IDOK,button,1342373889
Control9=IDCANCEL,button,1342373888
Control10=1090,static,1342308352
Control11=1152,edit,1350632576
Control12=1120,listbox,1352732755

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_EMAIL,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_Staic,static,1342308352

[DLG:IDD_SleepHelper_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=16
Control1=IDC_EDTMINCNT,edit,1350639746
Control2=IDC_SPNMINCNT,msctls_updown32,1342177322
Control3=IDC_LBLMIN,static,1342177280
Control4=IDC_EDTSECCNT,edit,1350639746
Control5=IDC_SPNSECCNT,msctls_updown32,1342177322
Control6=IDC_LBLSEC,static,1342177280
Control7=IDC_BTNPAUSE,button,1342242816
Control8=IDC_BTNAPPLY,button,1342242817
Control9=IDRegNow,button,1342242816
Control10=IDC_FRAOPT,button,1342177287
Control11=IDC_SLIDER,msctls_trackbar32,1342242840
Control12=IDC_OpenFile,button,1342242817
Control13=IDC_LIST_CTRL,SysListView32,1350647824
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDOnHelp,button,1342242816

[DLG:IDD_SN (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDC_ISN,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_UserName,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDOrder,button,1342242817

