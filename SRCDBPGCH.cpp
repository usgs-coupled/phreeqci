// SRCDBPGCH.cpp : implementation file
//
// $Date: 2001/08/20 20:36:17 $
// $Revision: 1.1.1.8 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "SRCDBPGCH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPGCH


BEGIN_MESSAGE_MAP(CSRCDBPGCH, CSRCDBPG)
	//{{AFX_MSG_MAP(CSRCDBPGCH)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPGCH message handlers

BOOL CSRCDBPGCH::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	pHelpInfo->iCtrlId = GetDlgCtrlID();
	return Default();	
}
