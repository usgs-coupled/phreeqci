// CKSReaction_Temperature.h : header file
//
// This class defines custom modal property sheet 
// CCKSReaction_Temperature.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSREACTION5F_TEMPERATURE_H__
#define __CKSREACTION5F_TEMPERATURE_H__

#include "CKPReaction_TemperaturePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSReaction_Temperature

#include "CommonKeywordSheet.h"

#ifndef baseCKSReaction_Temperature
#define baseCKSReaction_Temperature CCommonKeywordSheet
#endif

class CCKSReaction_Temperature : public baseCKSReaction_Temperature
{
	DECLARE_DYNAMIC(CCKSReaction_Temperature)

// Construction
public:
	CCKSReaction_Temperature(CWnd* pWndParent = NULL);

// Attributes
public:
	CCKPReaction_TemperaturePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSReaction_Temperature)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSReaction_Temperature();

	virtual CString GetString();
	virtual void Edit(CString& rStr);


// Generated message map functions
protected:
	//{{AFX_MSG(CCKSReaction_Temperature)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSREACTION5F_TEMPERATURE_H__
