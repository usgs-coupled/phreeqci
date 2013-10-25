// KSSelectedOutput.h : header file
//
//
// $Id$
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSSelectedOutput.
 
#ifndef __KSSELECTEDOUTPUT_H__
#define __KSSELECTEDOUTPUT_H__

#include "KPSelectedOutputPg1.h"
#include "KPSelectedOutputPg8.h"
#include "KPSelectedOutputPg9.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSelectedOutput

#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSelectedOutput
#define baseCKSSelectedOutput CCommonKeywordSheet
#endif

class CKSSelectedOutput : public baseCKSSelectedOutput
{
	DECLARE_DYNAMIC(CKSSelectedOutput)

// Construction
public:
	CKSSelectedOutput(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSelectedOutputPg1 m_Page1;
	CKPSelectedOutputPg2 m_Page2;
	CKPSelectedOutputPg3 m_Page3;
	CKPSelectedOutputPg4 m_Page4;
	CKPSelectedOutputPg5 m_Page5;
	CKPSelectedOutputPg6 m_Page6;
	CKPSelectedOutputPg7 m_Page7;
	CKPSelectedOutputPg8 m_Page8;
	CKPSelectedOutputPg9 m_Page9;

	std::list<CString> listIsotopes;
	std::list<CString> listCalcValues;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSelectedOutput)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSSelectedOutput();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSelectedOutput)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSELECTEDOUTPUT_H__
