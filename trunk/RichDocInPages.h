// RichDocInPages.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __RICHDOCINPAGES_H__
#define __RICHDOCINPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// CRichDocInPage1 dialog

#include "RichDocIn.h"
#include "Layout/ETSLayout.h"

#ifndef baseRichDocInPage1
#define baseRichDocInPage1 ETSLayoutPropertyPage
#endif

class CRichDocInPage1 : public baseRichDocInPage1
{
	DECLARE_DYNCREATE(CRichDocInPage1)
	DECLARE_LAYOUT();

// Construction
public:
	CString m_strTitle;
	CString m_strPathName;
	CRichDocIn::XProperties m_props;
	CRichEditView* m_pRichView;
	CRichDocInPage1();
	~CRichDocInPage1();

// Dialog Data
	//{{AFX_DATA(CRichDocInPage1)
	enum { IDD = IDD_RICH_IN_PAGE1 };
	CButton	m_btnSetAsDefault;
	CComboBox	m_cboDatabase;
	BOOL	m_bSetAsDefault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRichDocInPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRichDocInPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnBOutput();
	afx_msg void OnBDb();
	afx_msg void OnSelchangeCbDb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateSetAsDefault();
};



#endif // __RICHDOCINPAGES_H__
