#if !defined(AFX_SRCDBPGPPG_H__8051EE96_B3FA_4523_A388_BE53C79C63F9__INCLUDED_)
#define AFX_SRCDBPGPPG_H__8051EE96_B3FA_4523_A388_BE53C79C63F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SRCDBPGPpg.h : Declaration of the CSRCDBPagerPropPage property page class.
// $Date: 2001/09/15 01:31:35 $
// $Revision: 1.1.1.6 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerPropPage : See SRCDBPGPpg.cpp.cpp for implementation.

class CSRCDBPagerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSRCDBPagerPropPage)
	DECLARE_OLECREATE_EX(CSRCDBPagerPropPage)

// Constructor
public:
	CSRCDBPagerPropPage();

// Dialog Data
	//{{AFX_DATA(CSRCDBPagerPropPage)
	enum { IDD = IDD_PROPPAGE_SRCDBPG };
	CString	m_strCaption;
	BOOL	m_bGotoFirstEnabled;
	BOOL	m_bGotoLastEnabled;
	BOOL	m_bGotoNewEnabled;
	BOOL	m_bGotoNextEnabled;
	BOOL	m_bGotoPrevEnabled;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSRCDBPagerPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCDBPGPPG_H__8051EE96_B3FA_4523_A388_BE53C79C63F9__INCLUDED)
