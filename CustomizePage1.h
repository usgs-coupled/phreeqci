// CustomizePage1.h : header file
//
// $Date: 2000/12/19 22:55:42 $
// $Revision: 1.1.1.24 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#ifndef __CUSTOMIZEPAGE1_H__
#define __CUSTOMIZEPAGE1_H__

/////////////////////////////////////////////////////////////////////////////
// CCustomizePage1 dialog

class CCustomizePage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomizePage1)

// Construction
public:
	CCustomizePage1();
	~CCustomizePage1();

// Dialog Data
	//{{AFX_DATA(CCustomizePage1)
	enum { IDD = IDD_CUSTOMIZE_PG1 };
	CComboBox	m_cboDatabase;
	CString	m_strDefaultDatabase;
	BOOL	m_bApplyOpen;
	BOOL	m_bApplyOnlyDef;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCustomizePage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomizePage1)
	afx_msg void OnBrowseDb();
	afx_msg void OnEditchangeCbDb();
	afx_msg void OnSelchangeCbDb();
	afx_msg void OnEditupdateCbDb();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnBApplyOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateButtons();
};


/////////////////////////////////////////////////////////////////////////////
// CCustomizePage2 dialog

class CCustomizePage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomizePage2)

// Construction
public:
	CCustomizePage2();
	~CCustomizePage2();

// Dialog Data
	//{{AFX_DATA(CCustomizePage2)
	enum { IDD = IDD_CUSTOMIZE_PG2 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCustomizePage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomizePage2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __CUSTOMIZEPAGE1_H__
