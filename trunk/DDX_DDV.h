// DDX_DDV.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDX_DDV_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_)
#define AFX_DDX_DDV_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_

#include "KeywordPageListItems.h"


//void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, double& value);
CWnd* PASCAL PrepareEditGridCtrl(CDataExchange* pDX, int nIDC);

//
// Exchange routines
//

// integral routines
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, int& value);
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, long& value);

// floating point routines
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, float& value, UINT nIDPrompt = AFX_IDP_PARSE_REAL);
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, double& value, UINT nIDPrompt = AFX_IDP_PARSE_REAL);
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, long double& value, UINT nIDPrompt = AFX_IDP_PARSE_REAL);

// string routines
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, CString& value);

// non-standard type routines
void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC, long nRow, long nCol, basic_command& value);
void PASCAL DDV_Basic(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands);


//
// Validation routines
//


void PASCAL DDV_MinEqualInt(CDataExchange* pDX, int value, int minVal, UINT nIDPrompt = IDS_INT_MIN_EQUAL_137);

void PASCAL DDX_GridTextDefault(CDataExchange* pDX, int nIDC, long nRow, long nCol, double& value, const double dDefault);
void PASCAL DDX_GridTextNaN(CDataExchange* pDX, int nIDC, long nRow, long nCol, double& value);

void PASCAL DDX_GridFail(CDataExchange* pDX, UINT nIDText, UINT nIDCaption = IDR_MAINFRAME);
void PASCAL DDX_GridFail(CDataExchange* pDX, LPCTSTR lpText, LPCTSTR lpCaption = NULL);

#endif // !defined(AFX_DDX_DDV_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_)
