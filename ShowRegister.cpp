// ShowRegister.cpp : implementation file
//

#include "stdafx.h"
#include "SleepHelper.h"
#include "SleepHelperDlg.h"
#include "ShowRegister.h"
#include "sha2.h"
#include "url.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ShowRegister dialog


ShowRegister::ShowRegister(CWnd* pParent /*=NULL*/)
	: CDialog(ShowRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(ShowRegister)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ShowRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ShowRegister)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ShowRegister, CDialog)
	//{{AFX_MSG_MAP(ShowRegister)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOrder, OnOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ShowRegister message handlers
bool ShowRegister::Register() {
	//Hash with SHA-2
	
	// if(uname.IsEmpty() )
	//   return false;
	
	//start sha-a
	
	int sha2Strength=256;//定义长度可定义256,384,512
	// The outputted hash
	CString outHash,hashString,OKoutHash;
//	CString    tempHash,uname,SN;
	//Get user Input
	this->GetDlgItemText(IDC_UserName, uname);
	this->GetDlgItemText(IDC_ISN, SN);

	hashString = uname;//转义
	// Temporary working buffers
    
	unsigned char* tempOut = new unsigned char[256];

	sha2_ctx sha;
	// Work out hash length
	int hashLength = sha2Strength / 8;
	// Hashing a string, open the object, give it the text, and finalise it
	sha2_begin(sha2Strength, &sha);
	sha2_hash(reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength(), &sha);
	sha2_end(tempOut, &sha);

	for (int i = 0; i < hashLength; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp, 16);
		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}
		tempHash += tmp;		
	}

	delete[] tempOut;

	outHash = tempHash;//伪装
	OKoutHash = tempHash;//正确的

	//SHA encode has been finished. ok,let's compare SN with uname.
	//split code
	outHash = outHash.Right(32);
	outHash = outHash.Left(8);

	OKoutHash = OKoutHash.Right(31);
	OKoutHash = OKoutHash.Left(7);
//	AfxMessageBox(outHash);

    outHash.MakeUpper();
	OKoutHash.MakeUpper();

	if (SN == outHash ) {
		Saveuname=uname;
		SaveSN=outHash;
		return true;
	}else if (SN == OKoutHash) {		
		Saveuname=uname;
		SaveSN=OKoutHash;

		return true;
	}
	return false;
}

void ShowRegister::OnOK() 
{
	// TODO: Add extra validation here

	if (!Register() || uname.IsEmpty() ||  SN.IsEmpty()) {

		CString infoRegWrong;
		infoRegWrong.LoadString(IDS_STRINGRegWrong);
		AfxMessageBox(infoRegWrong);

	}else{

		    CString infoRegSuccessed;
			infoRegSuccessed.LoadString(IDS_STRINGRegSuccessed);
			AfxMessageBox(infoRegSuccessed);
	
		//	sleepdlg.isreg=1;
			
			CString sql;
			
			 const unsigned char * tmpval;
			 const  char * tmpval1 =( LPCTSTR)SaveSN;
			 tmpval = (const unsigned char *) tmpval1;


			SaveSN = re.Qencode(tmpval).c_str();
			sql = "update user set uname ='"+Saveuname+"',SN = '"+SaveSN+"' ";
			
			CSleepHelperApp *app=(CSleepHelperApp*)AfxGetApp(); 
			CString dbfile =  app->m_sAppPath+_T("\\e.db3");
			db.open(dbfile);
			db.query(sql);
			db.close();
		//	CString 

			CDialog::OnOK();
	}
	
}

void ShowRegister::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();

}

void ShowRegister::OnOrder() 
{
	CString infoOrderURL;
	infoOrderURL.LoadString(IDS_OrderURL);
	::ShellExecute(NULL, NULL, _T(infoOrderURL), NULL, NULL, SW_SHOWNORMAL);

//	CURL url;
//	url.Open(infoOrderURL);	
	
}
