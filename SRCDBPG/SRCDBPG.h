#if !defined(AFX_SRCDBPG_H__F41A2064_285E_498A_BFF3_1246455D1EB7__INCLUDED_)
#define AFX_SRCDBPG_H__F41A2064_285E_498A_BFF3_1246455D1EB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SRCDBPG.h : main header file for SRCDBPG.DLL
// $Date: 2001/09/15 01:31:34 $
// $Revision: 1.1.1.6 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPGApp : See SRCDBPG.cpp for implementation.

class CSRCDBPGApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCDBPG_H__F41A2064_285E_498A_BFF3_1246455D1EB7__INCLUDED)
