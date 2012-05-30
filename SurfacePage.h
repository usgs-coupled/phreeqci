#pragma once
#include "afxcmn.h"

#include "GridListDoc.h"

// CSurfacePage dialog

#include "CommonKeywordPage.h"
// COMMENT: {12/28/2009 9:28:48 PM}#include "DefinedRanges.h"
#include "afxwin.h"

#ifndef baseCSurfacePage
#define baseCSurfacePage CCommonKeywordPage
#endif

class CSurfacePage : public baseCSurfacePage
{
	DECLARE_DYNAMIC(CSurfacePage)

public:
	CSurfacePage();
	virtual ~CSurfacePage();

// Dialog Data
	enum { IDD = IDD_KEY_SURFACE_PAGE };

public:
	class CSurfaceSheet* GetSheet();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CTabCtrl             TabCtrlSurfType;
	CListCtrl            ListCtrlDefined;
	CGridListDoc         GridListDocGen;      // m_glDoc
	CGridListDoc         GridListDocEqu;      // m_glDoc
	CGridListDoc         GridListDocKin;      // m_glDoc
	CEditGrid	         GridSurfTypes;       // m_egSurfTypes (IDC_MSHFG_SURFTYPE)
	CEditGrid	         GridSurfaces;        // m_egSurfaces  (IDC_MSHFG_SURFACES)
	CCheckListCtrl	     ListCtrlSurface;     // m_clcSurface  (IDC_CL_SURFACE)
	CButton              CheckBoxEquilibrate; // m_btnEquilibrate (IDC_CHECK_EQUILIBRATE/IDC_EQUILIBRATE)
	CComboBox            ComboSolutions;      // m_cboSolutions (IDC_CB_SOLUTIONS/IDC_CBO_SOLUTIONS)

	BOOL InitComboSolutions();
	void ExchangeComboSolutions();
	void ValidateComboSolutions();

	void ExchangeSCM(CDataExchange* pDX);
	void ValidateSCM(CDataExchange* pDX);

	void ExchangeEG_CLC(CDataExchange* pDX);
	void ExchangeEG_CLCGen(CDataExchange* pDX);
	void ExchangeEG_CLCEqu(CDataExchange* pDX);
	void ExchangeEG_CLCKin(CDataExchange* pDX);

	void ValidateEGSurfaces(CDataExchange* pDX);
	void ValidateEGSurfacesGen(CDataExchange* pDX);
	void ValidateEGSurfacesEqu(CDataExchange* pDX);
	void ValidateEGSurfacesKin(CDataExchange* pDX);

	void ValidateEGSurfTypes(CDataExchange* pDX);
	void ValidateEGSurfTypesGen(CDataExchange* pDX);
	void ValidateEGSurfTypesEqu(CDataExchange* pDX);
	void ValidateEGSurfTypesKin(CDataExchange* pDX);

	long AddSurface(const CString& strSurface);
	long GetSurface(const char *szFormula);
	void RemoveSurface(const CString& strSurface);	

	//{{
	BOOL InitCLC();
	BOOL InitCLCGen();
	BOOL InitCLCEqu();
	BOOL InitCLCKin();	

	BOOL InitEGSurfTypes();
	BOOL InitEGSurfTypesGen();
	BOOL InitEGSurfTypesEqu();
	BOOL InitEGSurfTypesKin();

	BOOL InitEGSurfaces();
	BOOL InitEGSurfacesGen();
	BOOL InitEGSurfacesEqu();
	BOOL InitEGSurfacesKin();
	//}}

	// radio updates
	void UpdateRadioSCM();
	void UpdateRadioDLO();
	void UpdateRadioDDL(BOOL bEnabled);
	void UpdateRetard(BOOL bEnabled);
	void UpdateMobileSurfaces();

	void UpdateListCtrlDefined(void);

	void SetEGDefaults(long nRow);
	void SetEGDefaultsGen(long nRow);
	void SetEGDefaultsEqu(long nRow);
	void SetEGDefaultsKin(long nRow);

public:
	enum DonnanType
	{
		DT_THICKNESS,
		DT_DEBYE_LENGTHS
	}DT;

	enum EquilSolutionNum
	{
		N_NONE = -1
	};

	enum TabIndex
	{
		TI_GENERAL = 0,
		TI_EQUILIBRIUM_PHASES = 1,
		TI_KINETIC_REACTANTS = 2
	};

	int n_solution;
	double thickness;
	double debye_lengths;
	double DDL_limit;
	double DDL_viscosity;
	int only_counter_ions;
	enum cxxSurface::SURFACE_TYPE type;
	enum cxxSurface::DIFFUSE_LAYER_TYPE dl_type;
	enum cxxSurface::SITES_UNITS sites_units;
	std::list<CSurfComp> ListSurfComp[3];        // m_listSurfComp;
	int transport;

// COMMENT: {12/29/2009 2:57:20 PM}	CDefinedRanges      DefinedRanges;
// COMMENT: {12/29/2009 2:57:20 PM}	std::set<CDBRange>  SetSolutions;
// COMMENT: {12/29/2009 2:57:20 PM}	CString             SolutionNumberFormat;

public:
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginCellEditGen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginCellEditEqu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginCellEditKin(WPARAM wParam, LPARAM lParam);	

	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEditGen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEditEqu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEditKin(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedCheckEquilibrate();
	afx_msg void OnBnClickedRadioNoEdl2();
	afx_msg void OnBnClickedRadioDdl2();
	afx_msg void OnBnClickedRadioCdMusic2();
	afx_msg void OnBnClickedRadioDlTypeN();
	afx_msg void OnBnClickedRadioDlTypeD();
	afx_msg void OnBnClickedRadioDlTypeB();
	afx_msg void OnBnClickedCheckRetard2();
	afx_msg void OnBnClickedRadioThick();
	afx_msg void OnBnClickedRadioDebye();
	afx_msg void OnCbnSetfocusCboSolutions();
	afx_msg void OnTcnSelchangingTabSurfType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabSurfType(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_EVENTSINK_MAP()
	void EnterCellMshfgSurftype();
	void EnterCellMshfgSurfaces();
	void EnterCellMshfgNumDesc();
	afx_msg void OnLvnItemchangedClSurface(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedClSurfaceGen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedClSurfaceEqu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedClSurfaceKin(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheckSiteDensity();
	afx_msg void OnBnSetfocusCheckEquilibrate();
	afx_msg void OnBnSetfocusCheckDonnanCio();
	afx_msg void OnBnSetfocusCheckBorkCio();
	afx_msg void OnNMSetfocusClSurface(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnSetfocusEditDonnanThickness();
	afx_msg void OnBnClickedCheckMobile();
	void KeyDownMshfgSurftype(short* KeyCode, short Shift);
	void KeyDownMshfgSurfaces(short* KeyCode, short Shift);
	afx_msg void OnBnSetfocusRadioNoEdl2();
	afx_msg void OnBnSetfocusRadioDdl2();
	afx_msg void OnBnSetfocusRadioCdMusic2();
	afx_msg void OnBnSetfocusRadioDlTypeD();
	afx_msg void OnBnSetfocusRadioThick();
	afx_msg void OnBnSetfocusRadioDebye();
	afx_msg void OnEnSetfocusEditDebyeLength();
	afx_msg void OnEnSetfocusEditDebLenLimit();
	afx_msg void OnEnSetfocusEditViscosity();
	afx_msg void OnBnSetfocusCheckRetard2();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnSetfocusCheckSiteDensity();
	afx_msg void OnBnSetfocusCheckMobile();
	afx_msg void OnEnSetfocusEditBorkThickness();
	afx_msg void OnBnSetfocusRadioDlTypeB();
	afx_msg void OnBnSetfocusRadioDlTypeN();
};

#ifndef _DEBUG
inline CSurfaceSheet* CSurfacePage::GetSheet()
   { return (CSurfaceSheet*)GetParent(); }
#endif
