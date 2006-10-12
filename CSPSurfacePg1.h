// CSPSurfacePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CSPSURFACEPG1_H__
#define __CSPSURFACEPG1_H__

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"
#include "DefinedRanges.h"

// forward declaration
class CCKSSurface;

/////////////////////////////////////////////////////////////////////////////
// CCommonSurfacePage dialog

#include "CommonKeywordPage.h"

#ifndef baseCommonSurfacePage
#define baseCommonSurfacePage CCommonKeywordPage
#endif

class CCommonSurfacePage : public baseCommonSurfacePage
{
	DECLARE_DYNCREATE(CCommonSurfacePage)

// Construction
public:
	CCommonSurfacePage();
	CCommonSurfacePage(UINT nIDTemplate, UINT nIDCaption = 0);
	CCommonSurfacePage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

public:
	CCKSSurface* GetSheet();

	~CCommonSurfacePage();

// Dialog Data
	//{{AFX_DATA(CCommonSurfacePage)
	enum { IDD = IDD_KEY_SURFACE_COMMON };
	CComboBox	m_cboSolutions;
	CButton	m_btnEquilibrate;
	CButton m_btnDensity;
	CCheckListCtrl	m_clcSurface;
	CEditGrid	m_egSurfTypes;
	CEditGrid	m_egSurfaces;
	//}}AFX_DATA

	CGridListDoc m_glDoc;
	std::list<CSurfComp>m_listSurfComp;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCommonSurfacePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	
// COMMENT: {10/12/2000 3:57:58 PM}	void DDX_Equilibrate(CDataExchange* pDX);
	long AddSurface(const CString& strSurface);
	long GetSurface(const char *szFormula);
	void RemoveSurface(const CString& strSurface);

	BOOL InitCBOSolutions();
	BOOL InitEGDesc();

	void ExchangeEGDesc();
	void ExchangeCBOSolutions();
	void ExchangeDiffuseOptions();

	void ValidateGridDesc();
	void ValidateCboSolutions();
	void ValidateDiffuseOptions();


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCommonSurfacePage)
	afx_msg void OnEquilibrate();

	// No electrostatics
	afx_msg void OnRadioNoES();

	// D&M DDL
	afx_msg void OnRadioDDL();
		afx_msg void OnRadioDDLNoExpl();
		afx_msg void OnRadioDDLDiffuse();
		afx_msg void OnRadioDDLDonnan();
	
	// CD_MUSIC
	afx_msg void OnRadioCDMusic();
		afx_msg void OnRadioCDNoExpl();
		afx_msg void OnRadioCDDonnan();


	afx_msg void OnRadioNoEdl();
	afx_msg void OnRadioDiffuse();
	afx_msg void OnSetfocusCbSolutions();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnKeyDownMshfgSurftype(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyDownMshfgSurfaces(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClSurface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusEditThick();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	// custom setfocus handlers
	afx_msg void OnSetfocusBtnEquil();
	//{{
	afx_msg void OnSetfocusRadioDefault();
	afx_msg void OnSetfocusRadioNoEDL();
	afx_msg void OnSetfocusRadioDiffuse();
	afx_msg void OnSetfocusCheckCounterOnly();
	//}}

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in CEPExchangePg1.cpp
inline CCKSSurface* CCommonSurfacePage::GetSheet()
   { return (CCKSSurface*)GetParent(); }
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg1 dialog

#ifndef baseCSPSurfacePg1
#define baseCSPSurfacePg1 CCommonSurfacePage
#endif

class CCSPSurfacePg1 : public baseCSPSurfacePg1
{
	DECLARE_DYNCREATE(CCSPSurfacePg1)

// Construction
public:
	CCSPSurfacePg1();
	~CCSPSurfacePg1();

// Dialog Data
	//{{AFX_DATA(CCSPSurfacePg1)
	enum { IDD = IDD_KEY_SURFACE_PG1A };
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCSPSurfacePg1)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void ExchangeEG_CLC();

	void ValidateEGSurfaces(CDataExchange* pDX);
	void ValidateEGSurfTypes(CDataExchange* pDX);

	BOOL InitCLC();
	BOOL InitEGSurfTypes();
	BOOL InitEGSurfaces();

	void SetEGDefaults(long nRow);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCSPSurfacePg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnEquilibrate();
	afx_msg void OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgSurftype();
	afx_msg void OnEnterCellMshfgSurfaces();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	// No electrostatics
	afx_msg void OnRadioNoES();

	// D&M DDL
	afx_msg void OnRadioDDL();
	
	// CD_MUSIC
	afx_msg void OnRadioCDMusic();

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg2 dialog

#ifndef baseCSPSurfacePg2
#define baseCSPSurfacePg2 CCommonSurfacePage
#endif

class CCSPSurfacePg2 : public baseCSPSurfacePg2
{
	DECLARE_DYNCREATE(CCSPSurfacePg2)

// Construction
public:
	CCSPSurfacePg2();
	~CCSPSurfacePg2();

// Dialog Data
	//{{AFX_DATA(CCSPSurfacePg2)
	enum { IDD = IDD_KEY_SURFACE_PG1A };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCSPSurfacePg2)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL InitCLC();
	BOOL InitEGSurfaces();
	BOOL InitEGSurfTypes();

	void ExchangeEG_CLC(CDataExchange* pDX);

	void ValidateEGSurfaces(CDataExchange* pDX);
	void ValidateEGSurfTypes(CDataExchange* pDX);

	void SetEGDefaults(long nRow);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCSPSurfacePg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgSurftype();
	afx_msg void OnEnterCellMshfgSurfaces();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	// No electrostatics
	afx_msg void OnRadioNoES();

	// D&M DDL
	afx_msg void OnRadioDDL();
	
	// CD_MUSIC
	afx_msg void OnRadioCDMusic();

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg3 dialog

#ifndef baseCSPSurfacePg3
#define baseCSPSurfacePg3 CCommonSurfacePage
#endif

class CCSPSurfacePg3 : public baseCSPSurfacePg3
{
	DECLARE_DYNCREATE(CCSPSurfacePg3)

// Construction
public:
	CCSPSurfacePg3();
	~CCSPSurfacePg3();

// Dialog Data
	//{{AFX_DATA(CCSPSurfacePg3)
	enum { IDD = IDD_KEY_SURFACE_PG1A };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCSPSurfacePg3)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	void ExchangeEG_CLC(CDataExchange* pDX);

	BOOL InitCLC();
	BOOL InitEGSurfTypes();
	BOOL InitEGSurfaces();
	
	void ValidateEGSurfTypes(CDataExchange* pDX);
	void ValidateEGSurfaces(CDataExchange* pDX);

	void SetEGDefaults(long nRow);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCSPSurfacePg3)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgSurftype();
	afx_msg void OnEnterCellMshfgSurfaces();
	afx_msg void OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	// No electrostatics
	afx_msg void OnRadioNoES();

	// D&M DDL
	afx_msg void OnRadioDDL();
	
	// CD_MUSIC
	afx_msg void OnRadioCDMusic();

	DECLARE_MESSAGE_MAP()

};



#endif // __CSPSURFACEPG1_H__
