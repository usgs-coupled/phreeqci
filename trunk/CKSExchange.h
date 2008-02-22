// CKSExchange.h : header file
//
// This class defines custom modal property sheet 
// CCKSExchange.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSEXCHANGE_H__
#define __CKSEXCHANGE_H__

#include "CEPExchangePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSExchange

#include "CommonKeywordSheet.h"

#ifndef baseCKSExchange
#define baseCKSExchange CCommonKeywordSheet
#endif

class CCKSExchange : public baseCKSExchange
{
	DECLARE_DYNAMIC(CCKSExchange)

// Construction
public:
	CCKSExchange(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	enum EquilSolutionNum
	{
		N_NONE = -1,
	};
	CCEPExchangePg1 m_Page1;
	CCEPExchangePg2 m_Page2;
	CCEPExchangePg3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSExchange)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bSolution_equilibria;
	int m_nEquilSolutionNum;
	std::set<CDBRange> m_setSolutions;
	CString m_strNumFormat;
	virtual ~CCKSExchange();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

protected:
	CDefinedRanges m_ranges;


// Generated message map functions
protected:
	//{{AFX_MSG(CCKSExchange)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSEXCHANGE_H__
