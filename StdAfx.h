// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// $Date: 2001/08/10 01:12:00 $
// $Revision: 1.1.1.32 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__4CD13B05_A2D8_471E_A433_E2301893EAC7__INCLUDED_)
#define AFX_STDAFX_H__4CD13B05_A2D8_471E_A433_E2301893EAC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>        // MFC socket extensions
#include <afxrich.h>        // MFC rich edit classes
#include <afxmt.h>          // MFC Multithreaded Extensions (Syncronization Objects)

#pragma warning(push, 3)    // STL headers
#include <limits>           // Added for numeric_limits
#include <string>           // Added for string typedef
#include <algorithm>        // various template methods
#include <vector>           // std::vector
#include <list>             // std::list
#include <set>              // std::set
#include <map>              // std::map
#pragma warning(pop)

#include "Posea/sizecbar.h"   // CSizingControlBar classes
#include "Layout/ETSLayout.h" // ETSLayout classes
#include "tree.h"             // CTreeCtrlEx and CTreeCtrlNode classes
#include "TipDlg.h"           // CTipDlg class

#pragma warning(disable : 4100)  // Turn off unreferenced formal parameter
#include "mshflexgrid.h"         // CMSHFlexGrid class
#include "srcdbpg.h"            // CSRCDBPG class
#pragma warning(default : 4100)  // Turn on unreferenced formal parameter

const TWIPS_PER_INCH = 1440;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4CD13B05_A2D8_471E_A433_E2301893EAC7__INCLUDED_)
