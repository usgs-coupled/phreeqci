// CKSReaction_Temperature.h : header file
//
// This class defines custom modal property sheet 
// CCKSReaction_Temperature.
//
// $Id: CKSReaction_Temperature.h 138 2005-02-10 02:58:28Z charlton $
//////////////////////////////////////////////////////////////////////
 
#ifndef __REACTION_PRESSURE_H__
#define __REACTION_PRESSURE_H__

#include "Reaction_PressurePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSReaction_Temperature

#include "CommonKeywordSheet.h"

#ifndef baseCReaction_Pressure
#define baseCReaction_Pressure CCommonKeywordSheet
#endif

class CReaction_Pressure : public baseCReaction_Pressure
{
	DECLARE_DYNAMIC(CReaction_Pressure)

// Construction
public:
	CReaction_Pressure(CWnd* pWndParent = NULL);

// Attributes
public:
	CReaction_PressurePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSReaction_Temperature)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReaction_Pressure();

	virtual CString GetString();
	virtual void Edit(CString& rStr);


// Generated message map functions
protected:
	//{{AFX_MSG(CReaction_Pressure)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __REACTION_PRESSURE_H__
