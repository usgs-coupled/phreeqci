// OCKSTransport.h : header file
//
// This class defines custom modal property sheet 
// COCKSTransport.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSTRANSPORT_H__
#define __OCKSTRANSPORT_H__

#include "KPTransportPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSTransport

#include "OleCtlKeywordSheet.h"

#ifndef baseCOCKSTransport
#define baseCOCKSTransport COleCtlKeywordSheet
#endif

class COCKSTransport : public baseCOCKSTransport
{
	DECLARE_DYNAMIC(COCKSTransport)

// Construction
public:
	COCKSTransport(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPTransportPg1 m_Page1;
	CKPTransportPg2 m_Page2;
	CKPTransportPg3 m_Page3;
	CKPTransportPg4 m_Page4;
	CKPTransportPg5 m_Page5;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSTransport)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COCKSTransport();

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSTransport)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSTRANSPORT_H__
