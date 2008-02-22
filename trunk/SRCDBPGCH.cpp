// SRCDBPGCH.cpp : implementation file
//
// $Id$
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
