// CSPSurfacePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CSPSurfacePg1.h"

#include "CKSSurface.h"
#include "Util.h"
#include "DDX_DDV.h"
#include "DelayUpdate.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/*
IDC_MSHFG_NUM_DESC,
IDC_EQUILIBRATE,
IDC_CB_SOLUTIONS,
IDC_CHECK_DENSITY,
IDC_CHECK_MOBILE,
IDC_CHECK_RETARD,
IDC_GB_DIFF_OPTS,
	IDC_RADIO_NO_ES,
	IDC_GB_DM_DDL,
	IDC_RADIO_DM_DDL,
		IDC_GB_NO_EDL,
		IDC_R_DM_NO_EXPL,
		IDC_GB_BORK,
		IDC_R_DM_DL,
			IDC_STATIC_THICK,
			IDC_EDIT_THICK,
		IDC_GB_DONNAN,
		IDC_R_DM_DONNAN,
			IDC_RADIO_DDL_THICK,
				IDC_EDIT_DDL_THICK,
			IDC_RADIO_DDL_DEBYE_L,
				IDC_EDIT_DDL_DEBYE_L,
				IDC_STATIC_DDL_LIMIT,
				IDC_EDIT_DDL_LIMIT,
		IDC_GB_CIO_V,
			IDC_CHECK_COUNTER_ONLY,
			IDC_ST_DM_DDL_VISC,
			IDC_EDIT_DM_DDL_VISC,
	IDC_GB_CD_MUSIC,
	IDC_RADIO_CD_MUSIC,
		IDC_GB_CD_NO_EDL,
		IDC_R_CD_NO_EXPL,
		IDC_GB_CD_DDL
		IDC_R_CD_DONNAN,
			IDC_R_CD_DONNAN_TH,
				IDC_EDIT_CD_DDL_THICK,
			IDC_R_CD_DONNAN_DB,
				IDC_EDIT_CD_DDL_LENGTH,
				IDC_ST_CD_DDL_LIMIT,
				IDC_EDIT_CD_DDL_LIMIT,
			IDC_CHECK_CD_DDL_CIO,
			IDC_ST_CD_DDL_VISC,
			IDC_EDIT_CD_DDL_VISC,
			
IDC_CL_SURFACE,
IDC_MSHFG_SURFTYPE,
IDC_MSHFG_SURFACES,
IDC_S_DESC_INPUT,
	IDC_E_DESC_INPUT,	
*/


IMPLEMENT_DYNCREATE(CCommonSurfacePage, baseCommonSurfacePage)
IMPLEMENT_DYNCREATE(CCSPSurfacePg1, baseCSPSurfacePg1)
IMPLEMENT_DYNCREATE(CCSPSurfacePg2, baseCSPSurfacePg2)
IMPLEMENT_DYNCREATE(CCSPSurfacePg3, baseCSPSurfacePg3)

/////////////////////////////////////////////////////////////////////////////
// CCommonSurfacePage property page

CCommonSurfacePage::CCommonSurfacePage() : baseCommonSurfacePage(CCommonSurfacePage::IDD)
, m_glDoc(&m_egSurfTypes, &m_clcSurface, 1)
{
	//{{AFX_DATA_INIT(CCommonExchangePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCommonSurfacePage::CCommonSurfacePage(UINT nIDTemplate, UINT nIDCaption /*= 0*/)
: baseCommonSurfacePage(nIDTemplate, nIDCaption)
, m_glDoc(&m_egSurfTypes, &m_clcSurface, 1)
{
}

CCommonSurfacePage::CCommonSurfacePage(LPCTSTR lpszTemplateName, UINT nIDCaption /*= 0*/)
: baseCommonSurfacePage(lpszTemplateName, nIDCaption)
, m_glDoc(&m_egSurfTypes, &m_clcSurface, 1)
{
}

CCommonSurfacePage::~CCommonSurfacePage()
{
}

void CCommonSurfacePage::DoDataExchange(CDataExchange* pDX)
{

	if (!pDX->m_bSaveAndValidate)
	{
		ASSERT(m_glDoc.GetCount() == 0);
	}

	baseCommonSurfacePage::DoDataExchange(pDX);

	
	//{{AFX_DATA_MAP(CCommonSurfacePage)
	DDX_Control(pDX, IDC_CB_SOLUTIONS, m_cboSolutions);
	DDX_Control(pDX, IDC_EQUILIBRATE, m_btnEquilibrate);
	DDX_Control(pDX, IDC_CHECK_DENSITY, m_btnDensity);
	
	DDX_Control(pDX, IDC_CL_SURFACE, m_clcSurface);
	DDX_Control(pDX, IDC_MSHFG_SURFTYPE, m_egSurfTypes);
	DDX_Control(pDX, IDC_MSHFG_SURFACES, m_egSurfaces);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		InitCBOSolutions();
	}

	if (pDX->m_bSaveAndValidate)
	{
		ASSERT(this->GetSheet());
		this->GetSheet()->m_bRetard = (this->IsDlgButtonChecked(IDC_CHECK_RETARD) == BST_CHECKED);
		ValidateGridDesc();
		ValidateCboSolutions();
		ValidateDiffuseOptions();
	}
	else
	{
		// If any other pages might modify
		// any of the variables set through these exchanges
		// they should be handled in this spot
		// otherwise place in OnInitDialog
		ExchangeEGDesc();
		ExchangeCBOSolutions();
		ExchangeDiffuseOptions();

		ASSERT(this->GetSheet());
		if (this->GetSheet()->m_bRetard)
		{
			this->CheckDlgButton(IDC_CHECK_RETARD, BST_CHECKED);
			double viscosity = this->GetSheet()->m_dDDL_viscosity;
			DDX_Text(pDX, IDC_EDIT_DM_DDL_VISC, viscosity);
			DDX_Text(pDX, IDC_EDIT_CD_DDL_VISC, viscosity);
		}
		else
		{
			this->CheckDlgButton(IDC_CHECK_RETARD, BST_UNCHECKED);
		}
		this->OnBnClickedCheckRetard();
	}
}


#ifdef _DEBUG
CCKSSurface* CCommonSurfacePage::GetSheet() // non-debug version is inline
{
	CCKSSurface* pSheet = (CCKSSurface*)GetParent();
	ASSERT_KINDOF(CCKSSurface, pSheet);
	return pSheet;
}
#endif

BEGIN_MESSAGE_MAP(CCommonSurfacePage, baseCommonSurfacePage)
	//{{AFX_MSG_MAP(CCommonSurfacePage)
	ON_BN_CLICKED(IDC_EQUILIBRATE, OnEquilibrate)

	// No electrostatics
	ON_BN_CLICKED(IDC_RADIO_NO_ES, OnRadioNoES)

	// D&M DDL
	ON_BN_CLICKED(IDC_RADIO_DM_DDL, OnRadioDDL)
		ON_BN_CLICKED(IDC_R_DM_NO_EXPL, OnRadioDDLNoExpl)
		ON_BN_CLICKED(IDC_R_DM_DL, OnRadioDDLDiffuse)
		ON_BN_CLICKED(IDC_R_DM_DONNAN, OnRadioDDLDonnan)
	
	// CD_MUSIC
	ON_BN_CLICKED(IDC_RADIO_CD_MUSIC, OnRadioCDMusic)
		ON_BN_CLICKED(IDC_R_CD_NO_EXPL, OnRadioCDNoExpl)
		ON_BN_CLICKED(IDC_R_CD_DONNAN, OnRadioCDDonnan)


	ON_CBN_SETFOCUS(IDC_CB_SOLUTIONS, OnSetfocusCbSolutions)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_SURFACE, OnSetfocusClSurface)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_EDIT_THICK, OnSetfocusEditThick)
	ON_EN_SETFOCUS(IDC_EDIT_THICK_CD, OnSetfocusEditThick)
	//}}AFX_MSG_MAP

	// custom setfocus notifications
	ON_BN_SETFOCUS(IDC_EQUILIBRATE, OnSetfocusBtnEquil)
	ON_BN_SETFOCUS(IDC_RADIO_DEFAULT, OnSetfocusRadioDefault)
	ON_BN_SETFOCUS(IDC_RADIO_NO_EDL, OnSetfocusRadioNoEDL)

	ON_BN_SETFOCUS(IDC_RADIO_NO_ES, OnSetfocusRadioNoEDL)


	ON_BN_SETFOCUS(IDC_RADIO_DIFFUSE, OnSetfocusRadioDiffuse)
	ON_BN_SETFOCUS(IDC_CHECK_COUNTER_ONLY, OnSetfocusCheckCounterOnly)

	// custom grid notifications
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	// ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)

	//{{
	ON_WM_SIZE()
	//}}
	ON_BN_CLICKED(IDC_RADIO_DDL_THICK, &CCommonSurfacePage::OnBnClickedRadioDdlThick)
	ON_BN_CLICKED(IDC_RADIO_DDL_DEBYE_L, &CCommonSurfacePage::OnBnClickedRadioDdlDebyeL)
	ON_BN_CLICKED(IDC_R_CD_DONNAN_TH, &CCommonSurfacePage::OnBnClickedRCdDonnanTh)
	ON_BN_CLICKED(IDC_R_CD_DONNAN_DB, &CCommonSurfacePage::OnBnClickedRCdDonnanDb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonSurfacePage message handlers
void CCommonSurfacePage::OnSetfocusCheckCounterOnly()
{
	// in order to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING486);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonSurfacePage::OnSetfocusBtnEquil()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_SURFACE_304);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonSurfacePage::OnSetfocusRadioDefault() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING484);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonSurfacePage::OnSetfocusRadioNoEDL() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING482);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonSurfacePage::OnSetfocusRadioDiffuse() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING483);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonSurfacePage::OnSetfocusEditThick() 
{
	// Add your control notification handler code here
	CString strRes;
	strRes.LoadString(IDS_STRING485);
	m_eInputDesc.SetWindowText(strRes);	
}


void CCommonSurfacePage::OnEquilibrate() 
{
	ASSERT( !IsKindOf(RUNTIME_CLASS(CCSPSurfacePg1)) );

	int nState = m_btnEquilibrate.GetCheck();

	ASSERT_KINDOF(CCKSSurface, GetSheet());

	if (GetSheet()->m_Page1.m_listSurfComp.size() > 0)
	{
		if (AfxMessageBox(IDS_SURFACE_301, MB_YESNO) == IDNO)
		{
			nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
			m_btnEquilibrate.SetCheck(nState);
			return;
		}
		else
		{
			GetSheet()->m_Page1.m_listSurfComp.clear();
		}
	}

	m_cboSolutions.EnableWindow(nState == BST_CHECKED);		
}

void CCommonSurfacePage::OnRadioNoES() 
{
	ASSERT(this->GetCheckedRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC) == IDC_RADIO_NO_ES);

	static int Ids[] = 
	{
		IDC_GB_DM_DDL,
			IDC_R_DM_NO_EXPL,
			IDC_R_DM_DL,
				IDC_STATIC_THICK,
				IDC_EDIT_THICK,
			IDC_R_DM_DONNAN,
				IDC_RADIO_DDL_THICK,
				IDC_EDIT_DDL_THICK,
				IDC_RADIO_DDL_DEBYE_L,
				IDC_EDIT_DDL_DEBYE_L,
				IDC_STATIC_DDL_LIMIT,
				IDC_EDIT_DDL_LIMIT,
			IDC_CHECK_COUNTER_ONLY,
			IDC_ST_DM_DDL_VISC,
			IDC_EDIT_DM_DDL_VISC,
		IDC_GB_CD_MUSIC,
			IDC_R_CD_NO_EXPL,
			IDC_R_CD_DONNAN,
				IDC_R_CD_DONNAN_TH,
					IDC_EDIT_CD_DDL_THICK,
				IDC_R_CD_DONNAN_DB,
					IDC_EDIT_CD_DDL_LENGTH,
					IDC_ST_CD_DDL_LIMIT,
					IDC_EDIT_CD_DDL_LIMIT,
				IDC_CHECK_CD_DDL_CIO,
				IDC_ST_CD_DDL_VISC,
				IDC_EDIT_CD_DDL_VISC
	};

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioDDL() 
{
	static int enableIds[] = 
	{
		IDC_GB_DM_DDL,
			IDC_R_DM_NO_EXPL,
			IDC_R_DM_DL,
			IDC_R_DM_DONNAN,
	};

	static int disableIds[] = 
	{
		IDC_GB_CD_MUSIC,
			IDC_R_CD_NO_EXPL,
			IDC_R_CD_DONNAN,
				IDC_R_CD_DONNAN_TH,
					IDC_EDIT_CD_DDL_THICK,
				IDC_R_CD_DONNAN_DB,
					IDC_EDIT_CD_DDL_LENGTH,
					IDC_ST_CD_DDL_LIMIT,
					IDC_EDIT_CD_DDL_LIMIT,
				IDC_CHECK_CD_DDL_CIO,
				IDC_ST_CD_DDL_VISC,
				IDC_EDIT_CD_DDL_VISC
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	ASSERT(IDC_R_DM_NO_EXPL < IDC_R_DM_DL);
	ASSERT(IDC_R_DM_DL < IDC_R_DM_DONNAN);

	int id = GetCheckedRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN);
	switch(id)
	{
	case IDC_R_DM_NO_EXPL:
		OnRadioDDLNoExpl();
		break;
	case IDC_R_DM_DL:
		this->OnRadioDDLDiffuse();
		break;
	case IDC_R_DM_DONNAN:
		this->OnRadioDDLDonnan();
		break;
	default:
		CheckRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN, IDC_R_DM_NO_EXPL);
		OnRadioDDLNoExpl();
		break;
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioDDLNoExpl(void)
{
	static int disableIds[] = 
	{
		IDC_STATIC_THICK,
		IDC_EDIT_THICK,
		IDC_CHECK_COUNTER_ONLY,
		IDC_ST_DM_DDL_VISC,
		IDC_EDIT_DM_DDL_VISC,
		//{{
		IDC_RADIO_DDL_THICK,
			IDC_EDIT_DDL_THICK,
		IDC_RADIO_DDL_DEBYE_L,
			IDC_EDIT_DDL_DEBYE_L,
			IDC_STATIC_DDL_LIMIT,
			IDC_EDIT_DDL_LIMIT,
		//}}
	};

	int i;

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioDDLDiffuse(void)
{
	static int enableIds[] = 
	{
		IDC_STATIC_THICK,
		IDC_EDIT_THICK,
		IDC_CHECK_COUNTER_ONLY,
	};

	static int disableIds[] = 
	{
		IDC_RADIO_DDL_THICK,
			IDC_EDIT_DDL_THICK,
		IDC_RADIO_DDL_DEBYE_L,
			IDC_EDIT_DDL_DEBYE_L,
			IDC_STATIC_DDL_LIMIT,
			IDC_EDIT_DDL_LIMIT,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	// update viscosity
	BOOL bEnableVisc = (this->IsDlgButtonChecked(IDC_CHECK_RETARD) == BST_CHECKED);
	if (CWnd *pWnd = this->GetDlgItem(IDC_ST_DM_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_DM_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioDDLDonnan(void)
{
	static int enableIds[] = 
	{
		IDC_RADIO_DDL_THICK,
		IDC_RADIO_DDL_DEBYE_L,
		IDC_CHECK_COUNTER_ONLY,
	};

	static int disableIds[] = 
	{
		IDC_STATIC_THICK,
		IDC_EDIT_THICK,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	//{{
	int id = GetCheckedRadioButton(IDC_RADIO_DDL_THICK, IDC_RADIO_DDL_DEBYE_L);
	switch(id)
	{
	case IDC_RADIO_DDL_THICK:
		this->OnBnClickedRadioDdlThick();
		break;
	case IDC_RADIO_DDL_DEBYE_L:
		this->OnBnClickedRadioDdlDebyeL();
		break;
	default:
		this->CheckRadioButton(IDC_RADIO_DDL_THICK, IDC_RADIO_DDL_DEBYE_L, IDC_RADIO_DDL_THICK);
		this->OnBnClickedRadioDdlThick();
		break;
	}
	//}}

	// update viscosity
	BOOL bEnableVisc = (this->IsDlgButtonChecked(IDC_CHECK_RETARD) == BST_CHECKED);
	if (CWnd *pWnd = this->GetDlgItem(IDC_ST_DM_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_DM_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}

	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
}

void CCommonSurfacePage::OnRadioCDMusic(void)
{
	static int enableIds[] = 
	{
		IDC_GB_CD_MUSIC,
			IDC_R_CD_NO_EXPL,
			IDC_R_CD_DONNAN,
	};

	static int disableIds[] = 
	{
		IDC_GB_DM_DDL,
			IDC_R_DM_NO_EXPL,
			IDC_R_DM_DL,
				IDC_STATIC_THICK,
				IDC_EDIT_THICK,
			IDC_R_DM_DONNAN,
				IDC_RADIO_DDL_THICK,
					IDC_EDIT_DDL_THICK,
				IDC_RADIO_DDL_DEBYE_L,
					IDC_EDIT_DDL_DEBYE_L,
					IDC_STATIC_DDL_LIMIT,
					IDC_EDIT_DDL_LIMIT,
				IDC_CHECK_COUNTER_ONLY,
				IDC_ST_DM_DDL_VISC,
				IDC_EDIT_DM_DDL_VISC,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	ASSERT(IDC_R_CD_NO_EXPL < IDC_R_CD_DONNAN);

	int id = GetCheckedRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN);
	switch(id)
	{
	case IDC_R_CD_NO_EXPL:
		OnRadioCDNoExpl();
		break;
	case IDC_R_CD_DONNAN:
		this->OnRadioCDDonnan();
		break;
	default:
		CheckRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN, IDC_R_CD_NO_EXPL);
		OnRadioCDNoExpl();
		break;
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}


void CCommonSurfacePage::OnRadioCDNoExpl(void)
{
	static int disableIds[] = 
	{
		IDC_R_CD_DONNAN_TH,
		IDC_EDIT_CD_DDL_THICK,
		IDC_R_CD_DONNAN_DB,
		IDC_EDIT_CD_DDL_LENGTH,
		IDC_ST_CD_DDL_LIMIT,
		IDC_EDIT_CD_DDL_LIMIT,
		IDC_CHECK_CD_DDL_CIO,
		IDC_ST_CD_DDL_VISC,
		IDC_EDIT_CD_DDL_VISC,
	};

	int i;

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioCDDonnan(void)
{
	static int enableIds[] = 
	{
		IDC_R_CD_DONNAN_TH,
			IDC_EDIT_CD_DDL_THICK,
		IDC_R_CD_DONNAN_DB,
			IDC_EDIT_CD_DDL_LENGTH,
		IDC_ST_CD_DDL_LIMIT,
		IDC_EDIT_CD_DDL_LIMIT,
		IDC_CHECK_CD_DDL_CIO,
		/*IDC_ST_CD_DDL_VISC*/
		/*IDC_EDIT_CD_DDL_VISC*/
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	//{{
	int id = GetCheckedRadioButton(IDC_R_CD_DONNAN_TH, IDC_R_CD_DONNAN_DB);
	switch(id)
	{
	case IDC_R_CD_DONNAN_TH:
		this->OnBnClickedRCdDonnanTh();
		break;
	case IDC_R_CD_DONNAN_DB:
		this->OnBnClickedRCdDonnanDb();
		break;
	default:
		this->CheckRadioButton(IDC_R_CD_DONNAN_TH, IDC_R_CD_DONNAN_DB, IDC_R_CD_DONNAN_TH);
		this->OnBnClickedRCdDonnanTh();
		break;
	}
	//}}

	// update viscosity
	BOOL bEnableVisc = (this->IsDlgButtonChecked(IDC_CHECK_RETARD) == BST_CHECKED);
	if (CWnd *pWnd = this->GetDlgItem(IDC_ST_CD_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CD_DDL_VISC))
	{
		pWnd->EnableWindow(bEnableVisc);
	}

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

void CCommonSurfacePage::OnRadioNoEdl() 
{
	ASSERT(FALSE); // no longer used?
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_THICK);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_THICK);
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_COUNTER_ONLY);

	pEdit->EnableWindow(FALSE);
	pStatic->EnableWindow(FALSE);
	pButton->EnableWindow(FALSE);

	//{{
	// Refresh radios
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		pWnd->RedrawWindow();
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		pWnd->RedrawWindow();
	}
	//}}
}

// COMMENT: {12/17/2009 10:45:03 PM}void CCommonSurfacePage::OnRadioDiffuse() 
// COMMENT: {12/17/2009 10:45:03 PM}{
// COMMENT: {12/17/2009 10:45:03 PM}	ASSERT(FALSE); // no longer used?
// COMMENT: {12/17/2009 10:45:03 PM}
// COMMENT: {12/17/2009 10:45:03 PM}	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_THICK);
// COMMENT: {12/17/2009 10:45:03 PM}	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_THICK);
// COMMENT: {12/17/2009 10:45:03 PM}	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_COUNTER_ONLY);
// COMMENT: {12/17/2009 10:45:03 PM}
// COMMENT: {12/17/2009 10:45:03 PM}	pEdit->EnableWindow(TRUE);
// COMMENT: {12/17/2009 10:45:03 PM}	pStatic->EnableWindow(TRUE);
// COMMENT: {12/17/2009 10:45:03 PM}	pButton->EnableWindow(TRUE);
// COMMENT: {12/17/2009 10:45:03 PM}
// COMMENT: {12/17/2009 10:45:03 PM}	//{{
// COMMENT: {12/17/2009 10:45:03 PM}	// Refresh radios
// COMMENT: {12/17/2009 10:45:03 PM}	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_NO_ES))
// COMMENT: {12/17/2009 10:45:03 PM}	{
// COMMENT: {12/17/2009 10:45:03 PM}		pWnd->RedrawWindow();
// COMMENT: {12/17/2009 10:45:03 PM}	}
// COMMENT: {12/17/2009 10:45:03 PM}	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
// COMMENT: {12/17/2009 10:45:03 PM}	{
// COMMENT: {12/17/2009 10:45:03 PM}		pWnd->RedrawWindow();
// COMMENT: {12/17/2009 10:45:03 PM}	}
// COMMENT: {12/17/2009 10:45:03 PM}	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
// COMMENT: {12/17/2009 10:45:03 PM}	{
// COMMENT: {12/17/2009 10:45:03 PM}		pWnd->RedrawWindow();
// COMMENT: {12/17/2009 10:45:03 PM}	}
// COMMENT: {12/17/2009 10:45:03 PM}	//}}
// COMMENT: {12/17/2009 10:45:03 PM}}

long CCommonSurfacePage::AddSurface(const CString& strSurface)
{
	long row;
	CString str;
	CString strSurf = strSurface.SpanExcluding(_T("_"));
	for (row = 1; row < m_egSurfaces.GetRows() - 1; ++row)
	{
		str = m_egSurfaces.GetTextMatrix(row, 0);
		if (str.Compare(strSurf) == 0)
		{
			// already in list
			break;
		}
		else if (str.IsEmpty())
		{
			m_egSurfaces.SetTextMatrix(row, 0, strSurf);
			m_egSurfaces.SetRow(row), m_egSurfaces.SetCol(0);
			// m_egSurfaces.SetCellFontBold(TRUE);
			m_egSurfaces.SetCellAlignment(flexAlignCenterCenter);
			m_egSurfaces.SetCol(1);
			break;
		}
	}
	return row;
}

void CCommonSurfacePage::RemoveSurface(const CString& strSurface)
{
	long row;
	CString str, strSurf2;
	CString strSurf = strSurface.SpanExcluding(_T("_"));
	BOOL bDoNothing = FALSE;
	for (row = 1; row < m_egSurfTypes.GetRows() - 1; ++row)
	{
		str = m_egSurfTypes.GetTextMatrix(row, 1);
		strSurf2 = str.SpanExcluding(_T("_"));
		if (strSurf2.Compare(strSurf) == 0)
		{
			// surface still in list
			bDoNothing = TRUE;
			break;
		}
	}
	if (bDoNothing == FALSE)
	{
		for (row = 1; row < m_egSurfaces.GetRows() - 1; ++row)
		{
			str = m_egSurfaces.GetTextMatrix(row, 0);
			if (str.Compare(strSurf) == 0)
			{
				// delete row doesn't remove header column
				m_egSurfaces.SetTextMatrix(row, 0, _T(""));
				//m_egSurfaces.DeleteRow(row);
				m_egSurfaces.DeleteRowAndHeader(row);
				break;
			}
			else if (str.IsEmpty())
			{
				ASSERT(FALSE);
				break;
			}
		}
	}
}

long CCommonSurfacePage::GetSurface(const char *szFormula)
{
	long row;
	CString strSurf = CString(szFormula).SpanExcluding(_T("_"));
	CString str;
	for (row = 1; row < m_egSurfaces.GetRows() - 1; ++row)
	{
		str = m_egSurfaces.GetTextMatrix(row, 0);
		if (str.Compare(strSurf) == 0)
		{
			return row;
			break;
		}
		if (str.IsEmpty())
		{
			break;
		}
	}
	return -1;
}

BOOL CCommonSurfacePage::InitCBOSolutions()
{
#ifdef _DEBUG
	CString str;
	m_cboSolutions.GetWindowText(str);
	ASSERT(str.IsEmpty());
	ASSERT(m_cboSolutions.GetCount() == 0);
#endif

	CUtil::InsertRange(m_cboSolutions.m_hWnd, GetSheet()->m_setSolutions);
	return TRUE;
}

BOOL CCommonSurfacePage::InitEGDesc()
{
	//
	// Handled by base class
	//
	return TRUE;
}

LRESULT CCommonSurfacePage::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}

	return bMakeChange;
}


void CCommonSurfacePage::ExchangeEGDesc()
{
	//
	// This is handled by CCommonKeywordPage::DDX_NumDesc
	//
}

void CCommonSurfacePage::ExchangeCBOSolutions()
{
	CCKSSurface* pSheet = GetSheet();
	ASSERT_KINDOF(CCKSSurface, pSheet);

	if (pSheet->m_nEquilSolutionNum != CCKSSurface::N_NONE)
	{
		m_cboSolutions.EnableWindow(TRUE);
		ASSERT( pSheet->m_nEquilSolutionNum >= 0 );
		CString strSolNum;

		if (pSheet->m_strNumFormat.IsEmpty())
		{
			pSheet->m_strNumFormat = _T("%d");
		}
		strSolNum.Format(pSheet->m_strNumFormat, pSheet->m_nEquilSolutionNum);
		int nFind = m_cboSolutions.SelectString(-1, strSolNum);
		if (nFind == -1)
		{
			m_cboSolutions.SetWindowText(strSolNum);
		}
		m_btnEquilibrate.SetCheck(BST_CHECKED);
	}
	else 
	{
		m_cboSolutions.EnableWindow(FALSE);
		m_btnEquilibrate.SetCheck(BST_UNCHECKED);
	}
}

void CCommonSurfacePage::ExchangeDiffuseOptions()
{
	// thickness
	//
	static int thicknessIds[] = 
	{
		IDC_EDIT_THICK,
		IDC_EDIT_THICK_CD,
	};
	for (int i = 0; i < sizeof(thicknessIds) / sizeof(thicknessIds[0]); ++i)
	{
		CString thickness;
		thickness.Format("%g", this->GetSheet()->m_dThickness);
		if (CEdit *pEdit = (CEdit*)this->GetDlgItem(thicknessIds[i]))
		{
			pEdit->SetWindowText(thickness);
		}
	}


	// type
	//
	if (this->GetSheet()->m_surfaceType == NO_EDL)
	{
		this->CheckRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC, IDC_RADIO_NO_ES);
		this->OnRadioNoES();
	}
	else if (this->GetSheet()->m_surfaceType == DDL)
	{
		this->CheckRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC, IDC_RADIO_DM_DDL);
		this->OnRadioDDL();

		switch(this->GetSheet()->m_dlType)
		{
		case NO_DL:
			this->CheckRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN, IDC_R_DM_NO_EXPL);
			this->OnRadioDDLNoExpl();
			break;

		case BORKOVEK_DL:
			this->CheckRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN, IDC_R_DM_DL);
			this->OnRadioDDLDiffuse();
			if (CButton* pButton = (CButton*)this->GetDlgItem(IDC_CHECK_COUNTER_ONLY))
			{
				if (this->GetSheet()->m_bOnlyCounterIons)
				{
					pButton->SetCheck(BST_CHECKED);
				}
				else
				{
					pButton->SetCheck(BST_UNCHECKED);
				}
			}
			break;

		case DONNAN_DL:
			this->CheckRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN, IDC_R_DM_DONNAN);
			this->OnRadioDDLDonnan();
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}
	else if (this->GetSheet()->m_surfaceType == CD_MUSIC)
	{
		this->CheckRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC, IDC_RADIO_CD_MUSIC);
		this->OnRadioCDMusic();

		switch(this->GetSheet()->m_dlType)
		{
		case NO_DL:
			this->CheckRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN, IDC_R_CD_NO_EXPL);
			this->OnRadioCDNoExpl();
			break;

		case BORKOVEK_DL:
			ASSERT(FALSE);
			break;

		case DONNAN_DL:
			this->CheckRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN, IDC_R_CD_DONNAN);
			this->OnRadioCDDonnan();
			break;

		default:
			ASSERT(FALSE);
			break;
		}

	}
}

void CCommonSurfacePage::ValidateCboSolutions()
{
	int nState = m_btnEquilibrate.GetCheck();	

	CCKSSurface* pSheet = GetSheet();
	ASSERT_KINDOF(CCKSSurface, pSheet);

	if (nState == BST_UNCHECKED)
	{
		pSheet->m_nEquilSolutionNum = -1;
		return;
	}

	CString strText;
	m_cboSolutions.GetWindowText(strText);
	strText.TrimLeft();
	char* pEnd;

	int nCurSel = m_cboSolutions.GetCurSel();
	if (nCurSel != -1)
	{
		pSheet->m_nEquilSolutionNum = strtol(strText, &pEnd, 10);
	}
	else
	{
		if (strText.IsEmpty())
		{
			pSheet->m_nEquilSolutionNum = -1;
			CString str;
			str = _T("Equilibration solution must be a positive integer.");
			OnSetfocusCbSolutions();
			m_cboSolutions.SetFocus();
			MessageBox(str, "Missing solution number", MB_OK);
			AfxThrowUserException();
		}
		else
		{
			pSheet->m_nEquilSolutionNum = strtol(strText, &pEnd, 10);
			if (*pEnd != 0
				||
				pSheet->m_nEquilSolutionNum < 0 
				||
				(pSheet->m_nEquilSolutionNum == 0 && *pEnd != 0))
			{
				CString str;
				str = _T("Equilibration solution must be a positive integer.");
				OnSetfocusCbSolutions();
				m_cboSolutions.SetFocus();
				MessageBox(str, "Invalid solution number", MB_OK);
				AfxThrowUserException();
			}
		}
	}	
}

void CCommonSurfacePage::ValidateGridDesc()
{
	//
	// This is handled by CCommonKeywordPage::DDX_NumDesc
	//
}

void CCommonSurfacePage::ValidateDiffuseOptions()
{
	// init
	//
	int thicknessID = 0;
	this->GetSheet()->m_dlType = NO_DL;
	this->GetSheet()->m_bOnlyCounterIons = FALSE;

	// type
	//
	switch(this->GetCheckedRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC))
	{
	case IDC_RADIO_NO_ES:
		this->GetSheet()->m_surfaceType = NO_EDL;
		break;

	case IDC_RADIO_DM_DDL:
		this->GetSheet()->m_surfaceType = DDL;
		switch(this->GetCheckedRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN))
		{
		case IDC_R_DM_DL:
			this->GetSheet()->m_dlType = BORKOVEK_DL;
			break;
		case IDC_R_DM_DONNAN:
			this->GetSheet()->m_dlType = DONNAN_DL;
			break;
		}
		thicknessID = IDC_EDIT_THICK;
		break;

	case IDC_RADIO_CD_MUSIC:
		this->GetSheet()->m_surfaceType = CD_MUSIC;
		switch(this->GetCheckedRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN))
		{
		case IDC_R_CD_DONNAN:
			this->GetSheet()->m_dlType = DONNAN_DL;
			break;
		}
		thicknessID = IDC_EDIT_THICK_CD;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	// thickness
	//
	if (this->GetSheet()->m_dlType == BORKOVEK_DL || this->GetSheet()->m_dlType == DONNAN_DL)
	{
		ASSERT(thicknessID);
		if (CEdit* pEdit = (CEdit*)GetDlgItem(thicknessID))
		{
			CString str;
			pEdit->GetWindowText(str);
			if (::sscanf(str, "%lf", &GetSheet()->m_dThickness) < 1)
			{
				this->MessageBox("Expected thickness of diffuse layer.", "Invalid thickness", MB_OK);
				pEdit->SetFocus();
				GetFocus()->SendMessage(EM_SETSEL, 0, -1);
				AfxThrowUserException();
			}
		}

		// counter only
		//
		if (this->GetSheet()->m_surfaceType == DDL)
		{

			if (CButton* pCounterOnly = (CButton*)this->GetDlgItem(IDC_CHECK_COUNTER_ONLY))
			{
				if (pCounterOnly->GetCheck() == BST_CHECKED)
				{
					this->GetSheet()->m_bOnlyCounterIons = TRUE;
				}
			}
		}
	}
}

void CCommonSurfacePage::OnSetfocusCbSolutions() 
{
	CString strRes;
	strRes.LoadString(IDS_SURFACE_303);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CCommonSurfacePage::OnInitDialog() 
{
	baseCommonSurfacePage::OnInitDialog();

// COMMENT: {12/15/2009 10:01:53 PM}	// set layout
// COMMENT: {12/15/2009 10:01:53 PM}	CreateRoot(VERTICAL, 5, 6) 
// COMMENT: {12/15/2009 10:01:53 PM}		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}			)
// COMMENT: {12/15/2009 10:01:53 PM}		<< itemFixed(VERTICAL, 2)
// COMMENT: {12/15/2009 10:01:53 PM}		<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}			<< itemFixed(HORIZONTAL, 15)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_EQUILIBRATE, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_CB_SOLUTIONS, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}			<< itemFixed(HORIZONTAL, 50)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_CHECK_DENSITY, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}			)
// COMMENT: {12/15/2009 10:01:53 PM}		<< (paneCtrl(IDC_STATIC_DIFF_OPTS, VERTICAL, GREEDY, nDefaultBorder, 4, 15, 0)
// COMMENT: {12/15/2009 10:01:53 PM}			<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}				<< itemFixed(HORIZONTAL, 15)
// COMMENT: {12/15/2009 10:01:53 PM}				<< item(IDC_RADIO_NO_ES, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}				)
// COMMENT: {12/15/2009 10:01:53 PM}			<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}				<< itemFixed(HORIZONTAL, 15)
// COMMENT: {12/15/2009 10:01:53 PM}				<< item(IDC_RADIO_DM_DDL, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}				<< (paneCtrl(IDC_STATIC_DM_DDL, VERTICAL, GREEDY, nDefaultBorder, 4, 8, 0)
// COMMENT: {12/15/2009 10:01:53 PM}					<< item(IDC_R_DM_NO_EXPL, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}					<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_R_DM_DL, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_STATIC_THICK, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_EDIT_THICK, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_CHECK_COUNTER_ONLY, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}						)
// COMMENT: {12/15/2009 10:01:53 PM}// COMMENT: {12/11/2009 10:36:43 PM}					<< item(IDC_R_DM_DONNAN, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}					//{{
// COMMENT: {12/15/2009 10:01:53 PM}					<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_R_DM_DONNAN, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_STATIC_VISC, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						<< item(IDC_EDIT_VISC, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}						)
// COMMENT: {12/15/2009 10:01:53 PM}					//}}
// COMMENT: {12/15/2009 10:01:53 PM}					)
// COMMENT: {12/15/2009 10:01:53 PM}				)
// COMMENT: {12/15/2009 10:01:53 PM}			<< (pane(HORIZONTAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}				<< itemFixed(HORIZONTAL, 15)
// COMMENT: {12/15/2009 10:01:53 PM}				<< item(IDC_RADIO_CD_MUSIC, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}				<< (paneCtrl(IDC_STATIC_CD_MUSIC, VERTICAL, GREEDY, nDefaultBorder, 4, 8, 0)
// COMMENT: {12/15/2009 10:01:53 PM}					<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}						<< (pane(VERTICAL, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}							<< item(IDC_R_CD_NO_EXPL, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}							<< item(IDC_R_CD_DONNAN, NORESIZE)
// COMMENT: {12/15/2009 10:01:53 PM}							)
// COMMENT: {12/15/2009 10:01:53 PM}						<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}							<< item(IDC_STATIC_THICK_CD, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}							<< item(IDC_EDIT_THICK_CD, NORESIZE | ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}							<< item(paneNull, ALIGN_VCENTER)
// COMMENT: {12/15/2009 10:01:53 PM}							)
// COMMENT: {12/15/2009 10:01:53 PM}						)
// COMMENT: {12/15/2009 10:01:53 PM}					)
// COMMENT: {12/15/2009 10:01:53 PM}				)
// COMMENT: {12/15/2009 10:01:53 PM}			)
// COMMENT: {12/15/2009 10:01:53 PM}		<< itemFixed(VERTICAL, 4)
// COMMENT: {12/15/2009 10:01:53 PM}		<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_CL_SURFACE, RELATIVE_HORZ, 40)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_MSHFG_SURFTYPE, RELATIVE_HORZ, 20)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_MSHFG_SURFACES, RELATIVE_HORZ, 40)
// COMMENT: {12/15/2009 10:01:53 PM}			)
// COMMENT: {12/15/2009 10:01:53 PM}
// COMMENT: {12/15/2009 10:01:53 PM}		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {12/15/2009 10:01:53 PM}			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
// COMMENT: {12/15/2009 10:01:53 PM}			);


	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (pane(HORIZONTAL, GREEDY, 5, 0, 0)
			<< item(IDC_EQUILIBRATE, NORESIZE)
			<< item(IDC_CB_SOLUTIONS, NORESIZE)
			<< itemFixed(HORIZONTAL, 133)
			<< item(IDC_CHECK_DENSITY, NORESIZE)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CHECK_MOBILE, NORESIZE)
			<< itemSpaceLike(HORIZONTAL, IDC_CB_SOLUTIONS)
			<< itemFixed(HORIZONTAL, 133)
			<< item(IDC_CHECK_RETARD, NORESIZE)
			)
		<< (paneCtrl(IDC_GB_DIFF_OPTS, VERTICAL, GREEDY, nDefaultBorder, 10, 30, 10)
			<< itemFixed(VERTICAL, 3)
			<< (paneCtrl(IDC_GB_DM_DDL, HORIZONTAL, GREEDY, 10, 10, 8, 0)
				<< (paneCtrl(IDC_GB_NO_EDL, VERTICAL, /*ABSOLUTE_VERT*/ NORESIZE, nDefaultBorder, 4, 15, 0)				
					<< itemFixed(HORIZONTAL, 140)
					)
				<< (pane(VERTICAL, GREEDY)
					<< (pane(HORIZONTAL, GREEDY)
						<< (paneCtrl(IDC_GB_BORK, VERTICAL, ABSOLUTE_VERT, /*16*/8, 8, 15, /*0*/20 /*nDefaultBorder, 4, 15, 0*/)
							<< (pane(HORIZONTAL, GREEDY)
								<< item(IDC_STATIC_THICK, NORESIZE)
								<< item(IDC_EDIT_THICK, ABSOLUTE_VERT)
								)								
							)
						<< (paneCtrl(IDC_GB_DONNAN, VERTICAL, ABSOLUTE_VERT, 4, 2, 15, 20)
							<< itemFixed(VERTICAL, 0)
							<< (pane(HORIZONTAL, GREEDY, 0)
								<< itemFixed(HORIZONTAL, 15)
								<< item(IDC_RADIO_DDL_THICK, NORESIZE | ALIGN_VCENTER)
								<< item(IDC_EDIT_DDL_THICK, ABSOLUTE_VERT | ALIGN_VCENTER)
								<< itemFixed(HORIZONTAL, 8)
								)								
							<< (pane(HORIZONTAL, GREEDY, 0)
								<< itemFixed(HORIZONTAL, 15)
								<< item(IDC_RADIO_DDL_DEBYE_L, NORESIZE | ALIGN_VCENTER)
								<< item(IDC_EDIT_DDL_DEBYE_L, ABSOLUTE_VERT | ALIGN_VCENTER)
								<< itemFixed(HORIZONTAL, 8)
								)								
							<< (pane(HORIZONTAL, GREEDY, 0)
								<< itemFixed(HORIZONTAL, 15)
								<< item(IDC_STATIC_DDL_LIMIT, NORESIZE | ALIGN_VCENTER)
								<< item(IDC_EDIT_DDL_LIMIT, ABSOLUTE_VERT | ALIGN_VCENTER)
								<< itemFixed(HORIZONTAL, 8)
								)								
							//<< itemFixed(VERTICAL, 0)
							)
						)
					<< (paneCtrl(IDC_GB_CIO_V, HORIZONTAL, GREEDY, nDefaultBorder, /*4*/8, /*15*/10, 0)				
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_CHECK_COUNTER_ONLY, NORESIZE | ALIGN_VCENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_ST_DM_DDL_VISC, NORESIZE | ALIGN_VCENTER)
						<< item(IDC_EDIT_DM_DDL_VISC, NORESIZE | ALIGN_VCENTER)
						<< itemGrowing(HORIZONTAL)
						)
					)
				)
			<< (paneCtrl(IDC_GB_CD_MUSIC, HORIZONTAL, GREEDY, 10, 10, 8, 0)
				<< (paneCtrl(IDC_GB_CD_NO_EDL, VERTICAL, NORESIZE, nDefaultBorder, 4, 15, 0)
					<< itemFixed(HORIZONTAL, 140)
					)
				<< (paneCtrl(IDC_GB_CD_DDL, HORIZONTAL, GREEDY, 8, 8, 15, 20)
					<< itemFixed(HORIZONTAL, 0)
					<< (pane(VERTICAL, GREEDY)
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_R_CD_DONNAN_TH, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_CD_DDL_THICK, ABSOLUTE_VERT | ALIGN_VCENTER)
							)								
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_R_CD_DONNAN_DB, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_CD_DDL_LENGTH, ABSOLUTE_VERT | ALIGN_VCENTER)
							)								
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_ST_CD_DDL_LIMIT, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_CD_DDL_LIMIT, ABSOLUTE_VERT | ALIGN_VCENTER)
							)
						)
					<< itemFixed(HORIZONTAL, 4)
					<< (pane(VERTICAL, GREEDY)
						<< itemGrowing(VERTICAL)
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_CHECK_CD_DDL_CIO, NORESIZE | ALIGN_VCENTER)
							)								
						<< itemFixed(VERTICAL, 0)
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_ST_CD_DDL_VISC, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_CD_DDL_VISC, ABSOLUTE_VERT | ALIGN_VCENTER)
							)								
						<< itemGrowing(VERTICAL)
						)
					<< itemFixed(HORIZONTAL, 0)
					)
				)
			)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_SURFACE, RELATIVE_HORZ, 40)
			<< item(IDC_MSHFG_SURFTYPE, RELATIVE_HORZ, 20)
			<< item(IDC_MSHFG_SURFACES, RELATIVE_HORZ, 40)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			)
	;

	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg1 property page

CCSPSurfacePg1::CCSPSurfacePg1() : baseCSPSurfacePg1(CCSPSurfacePg1::IDD)
{
	//{{AFX_DATA_INIT(CCSPSurfacePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCSPSurfacePg1::~CCSPSurfacePg1()
{
}

void CCSPSurfacePg1::DoDataExchange(CDataExchange* pDX)
{
	baseCSPSurfacePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSPSurfacePg1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		ValidateEGSurfaces(pDX);
		ValidateEGSurfTypes(pDX);
	}
	else
	{
		InitCLC();
		InitEGSurfTypes();
		InitEGSurfaces();
		ExchangeEG_CLC();
	}

	if (m_btnDensity.GetSafeHwnd())
	{
		if (pDX->m_bSaveAndValidate)
		{
			if (m_btnDensity.GetCheck() == BST_CHECKED)
			{
				this->GetSheet()->m_sitesUnits = SITES_DENSITY;
			}
			else
			{
				this->GetSheet()->m_sitesUnits = SITES_ABSOLUTE;
			}
		}
		else
		{
			if (this->GetSheet()->m_sitesUnits == SITES_DENSITY)
			{
				m_btnDensity.SetCheck(BST_CHECKED);
			}
			else
			{
				m_btnDensity.SetCheck(BST_UNCHECKED);
			}
		}
	}

}


BEGIN_MESSAGE_MAP(CCSPSurfacePg1, baseCSPSurfacePg1)
	//{{AFX_MSG_MAP(CCSPSurfacePg1)
	ON_BN_CLICKED(IDC_EQUILIBRATE, OnEquilibrate)
	ON_BN_CLICKED(IDC_CHECK_DENSITY, OnCheckDensity)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SURFACE, OnItemchangedClSurface)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)

	// No electrostatics
	ON_BN_CLICKED(IDC_RADIO_NO_ES, OnRadioNoES)

	// D&M DDL
	ON_BN_CLICKED(IDC_RADIO_DM_DDL, OnRadioDDL)
	
	// CD_MUSIC
	ON_BN_CLICKED(IDC_RADIO_CD_MUSIC, OnRadioCDMusic)

	// custom setfocus notifications
	ON_BN_SETFOCUS(IDC_CHECK_DENSITY, OnSetfocusCheckDensity)

	ON_BN_CLICKED(IDC_CHECK_RETARD, &CCommonSurfacePage::OnBnClickedCheckRetard)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg2 property page

CCSPSurfacePg2::CCSPSurfacePg2() : baseCSPSurfacePg2(CCSPSurfacePg2::IDD)
{
	//{{AFX_DATA_INIT(CCSPSurfacePg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCSPSurfacePg2::~CCSPSurfacePg2()
{
}

void CCSPSurfacePg2::DoDataExchange(CDataExchange* pDX)
{
	baseCSPSurfacePg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSPSurfacePg2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		ValidateEGSurfaces(pDX);
		ValidateEGSurfTypes(pDX);
	}
	else
	{
		if (m_bFirstSetActive)
		{
			InitCLC();
			InitEGSurfTypes();
			InitEGSurfaces();
		}
		ExchangeEG_CLC(pDX);
	}
}

void CCSPSurfacePg2::ValidateEGSurfaces(CDataExchange* pDX)
{
	CString strDummy;

	for (long row = 1; row < m_egSurfaces.GetRows(); ++row)
	{
		strDummy = m_egSurfaces.GetTextMatrix(row, 0);
		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{
			// Read surface area (m^2/g)
			double dArea;
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, "Expected specific area in m^2/g.", "Invalid specific area");
			}
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, "Specific area must be greater than 0.", "Invalid specific area");
			}

			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 2, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 2, dCap);

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), row, 3, dCap);
			}
		}
	}
}

void CCSPSurfacePg2::ValidateEGSurfTypes(CDataExchange* pDX)
{
	CString strDummy;

	std::list<CSurfComp> listSurfComp;

	ASSERT( m_egSurfTypes.GetSafeHwnd() != NULL );
	
	for (long nRow = m_egSurfTypes.GetFixedRows(); nRow < m_egSurfTypes.GetRows(); ++nRow)
	{
		// Line 2: exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 1, strDummy);

		if ( !strDummy.IsEmpty() )
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// mineral name
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 2, loSurfComp.m_strPhase_name);
			if (loSurfComp.m_strPhase_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected mineral name."), _T("Invalid mineral name"));
			}

			// read proportion
			// str = m_egSurfTypes.GetTextMatrix(row, 3);
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate surface to mineral."), _T("Missing ratio"));
			}
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, loSurfComp.m_dPhase_proportion);

			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			// validated in ValidateEGSurfaces()
			ASSERT( nRow2 != -1 );
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);

			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance1);
			}

			// all ok add to list
			listSurfComp.push_back(loSurfComp);			
		}
	}

	m_listSurfComp.assign(listSurfComp.begin(), listSurfComp.end());
}

BEGIN_MESSAGE_MAP(CCSPSurfacePg2, baseCSPSurfacePg2)
	//{{AFX_MSG_MAP(CCSPSurfacePg2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SURFACE, OnItemchangedClSurface)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)

	// No electrostatics
	ON_BN_CLICKED(IDC_RADIO_NO_ES, OnRadioNoES)

	// D&M DDL
	ON_BN_CLICKED(IDC_RADIO_DM_DDL, OnRadioDDL)
	
	// CD_MUSIC
	ON_BN_CLICKED(IDC_RADIO_CD_MUSIC, OnRadioCDMusic)

	ON_BN_CLICKED(IDC_CHECK_RETARD, &CCommonSurfacePage::OnBnClickedCheckRetard)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCSPSurfacePg3 property page

CCSPSurfacePg3::CCSPSurfacePg3() : baseCSPSurfacePg3(CCSPSurfacePg3::IDD)
{
	//{{AFX_DATA_INIT(CCSPSurfacePg3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCSPSurfacePg3::~CCSPSurfacePg3()
{
}

void CCSPSurfacePg3::DoDataExchange(CDataExchange* pDX)
{
	baseCSPSurfacePg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSPSurfacePg3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		ValidateEGSurfaces(pDX);
		ValidateEGSurfTypes(pDX);
	}
	else
	{
		if (m_bFirstSetActive)
		{
			InitCLC();
			InitEGSurfTypes();
			InitEGSurfaces();
		}
		ExchangeEG_CLC(pDX);
	}
}

void CCSPSurfacePg3::ValidateEGSurfaces(CDataExchange* pDX)
{
	CString strDummy;
	for (long nRow = 1; nRow < m_egSurfaces.GetRows(); ++nRow)
	{
		strDummy = m_egSurfaces.GetTextMatrix(nRow, 0);
		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{		
			// Read surface area (m^2/g)
			double dArea;
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected specific area in m^2/g."), _T("Invalid specific area"));
			}
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, _T("Specific area must be greater than 0."), _T("Invalid specific area"));
			}

			//{{
			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 2, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 2, dCap);

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 3, dCap);
			}
			//}}
		}
	}
}

void CCSPSurfacePg3::ValidateEGSurfTypes(CDataExchange* pDX)
{
	CString strDummy;

	std::list<CSurfComp> listSurfComp;

	ASSERT( m_egSurfTypes.GetSafeHwnd() != NULL );
	
	for (long nRow = m_egSurfTypes.GetFixedRows(); nRow < m_egSurfTypes.GetRows(); ++nRow)
	{
		// Line 2: exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 1, strDummy);
		if (!strDummy.IsEmpty())
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// rate name
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 2, loSurfComp.m_strRate_name);
			if (loSurfComp.m_strRate_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected kinetic reaction name."), _T("Invalid kinetic reaction name"));
			}

			// read proportion
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate surface to kinetic reaction."), _T("Missing ratio"));
			}
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, loSurfComp.m_dPhase_proportion);

			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			// validated in ValidateEGSurfaces()
			ASSERT( nRow2 != -1 );
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);

			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance1);
			}

			// all ok add to list
			listSurfComp.push_back(loSurfComp);
		}
	}
	m_listSurfComp.assign(listSurfComp.begin(), listSurfComp.end());
}



BEGIN_MESSAGE_MAP(CCSPSurfacePg3, baseCSPSurfacePg3)
	//{{AFX_MSG_MAP(CCSPSurfacePg3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SURFACE, OnItemchangedClSurface)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)

	// No electrostatics
	ON_BN_CLICKED(IDC_RADIO_NO_ES, OnRadioNoES)

	// D&M DDL
	ON_BN_CLICKED(IDC_RADIO_DM_DDL, OnRadioDDL)
	
	// CD_MUSIC
	ON_BN_CLICKED(IDC_RADIO_CD_MUSIC, OnRadioCDMusic)
	ON_BN_CLICKED(IDC_CHECK_RETARD, &CCommonSurfacePage::OnBnClickedCheckRetard)
END_MESSAGE_MAP()

LRESULT CCSPSurfacePg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = m_clcSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = m_clcSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}
					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (m_clcSurface.GetItemCount() > 0);
			}
			break;

		case 2 :
			// set to rate list
			pInfo->item.bUseCombo = (CUtil::InsertRates(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCSPSurfacePg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)m_egSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if (pInfo->item.pszText == NULL) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			else if (str.IsEmpty())
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)m_egSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (m_egSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else
	{
		return baseCSPSurfacePg3::OnEndCellEdit(wParam, lParam);
	}
}

void CCSPSurfacePg3::SetEGDefaults(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= m_egSurfTypes.GetFixedRows() );

	if (m_egSurfTypes.GetTextMatrix(nRow, 3).IsEmpty())
	{
		m_egSurfTypes.SetTextMatrix(nRow, 3, _T("0.0"));
	}

	long nRow2 = AddSurface(m_egSurfTypes.GetTextMatrix(nRow, 1));

	if (m_egSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 1, _T("0.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 2, _T("1.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 3, _T("5.0"));
	}
}

LRESULT CCSPSurfacePg3::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);
	UINT nID = wParam;

	switch (nID)
	{
	case IDC_MSHFG_SURFTYPE :
		OnEnterCellMshfgSurftype();
		break;
	case IDC_MSHFG_SURFACES :
		OnEnterCellMshfgSurfaces();
		break;
	default :
		OnEnterCellMshfgNumDesc();
		break;
	}

	return 0;
}

BOOL CCSPSurfacePg1::OnInitDialog() 
{
	baseCSPSurfacePg1::OnInitDialog();
	
	// Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCSPSurfacePg2::OnInitDialog() 
{
	baseCSPSurfacePg2::OnInitDialog();

	m_btnDensity.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCSPSurfacePg2::InitCLC()
{
	int nItem;

	ASSERT(m_clcSurface.GetItemCount() == 0);
	std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
	for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
	{
		if ( (*elemIter).m_type == CDBElement::typeSURF )
		{
			if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
			{
				// add to columns here
			}
		}
	}

	m_clcSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CCSPSurfacePg2::InitEGSurfaces()
{
	// must only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set rows, cols
	m_egSurfaces.SetRows(21);
	m_egSurfaces.SetCols(0, 4);

	// set column titles
	m_egSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	m_egSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/mol)"));
	m_egSurfaces.SetTextMatrix(0, 2, _T("Cap. 0-1 plane"));
	m_egSurfaces.SetTextMatrix(0, 3, _T("Cap. 1-2 plane"));

	// set bold titles
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(0);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(1);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(2);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(3);
	m_egSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfaces.SetFillStyle(flexFillRepeat);
	m_egSurfaces.SetRow(0);
	m_egSurfaces.SetCol(1);
	m_egSurfaces.SetColSel(m_egSurfaces.GetCols(0) - 1);
	m_egSurfaces.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfaces.SetColWidth(0, 0, 800);
	m_egSurfaces.SetColWidth(1, 0, 1500);
	if (this->GetSheet()->m_surfaceType == CD_MUSIC)
	{
		m_egSurfaces.SetColWidth(2, 0, 1500);
		m_egSurfaces.SetColWidth(3, 0, 1500);
	}
	else
	{
		m_egSurfaces.SetColWidth(2, 0, 0);
		m_egSurfaces.SetColWidth(3, 0, 0);
	}

	// scroll to top of grid
	m_egSurfaces.SetTopRow(1); m_egSurfaces.SetLeftCol(1);

	// set initial cell
	m_egSurfaces.SetRow(1); m_egSurfaces.SetCol(1);

	return TRUE;
}

BOOL CCSPSurfacePg2::InitEGSurfTypes()
{
	// must only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set rows, cols
	m_egSurfTypes.SetRows(21);
	m_egSurfTypes.SetCols(0, 4);

	// set column titles
	m_egSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	m_egSurfTypes.SetTextMatrix(0, 2, _T("Equil. phase"));
	m_egSurfTypes.SetTextMatrix(0, 3, _T("Sites/mol"));

	// set bold titles
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetCellFontBold(TRUE);
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(2);
	m_egSurfTypes.SetCellFontBold(TRUE);
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(3);
	m_egSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfTypes.SetFillStyle(flexFillRepeat);
	m_egSurfTypes.SetRow(0);
	m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetColSel(m_egSurfTypes.GetCols(0) - 1);
	m_egSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfTypes.SetColWidth(1, 0, 1050);
	m_egSurfTypes.SetColWidth(2, 0, 1200);
	m_egSurfTypes.SetColWidth(3, 0, 1100);

	// set row titles
	m_egSurfTypes.AddRowHeaders();

	// scroll to top of grid
	m_egSurfTypes.SetTopRow(1); m_egSurfTypes.SetLeftCol(1);

	// set initial cell
	m_egSurfTypes.SetRow(1); m_egSurfTypes.SetCol(1);

	return TRUE;
}

void CCSPSurfacePg2::ExchangeEG_CLC(CDataExchange* pDX)
{
	long nRow = 1;
	CString format;

	ASSERT( m_clcSurface.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfaces.GetSafeHwnd() != NULL );
	ASSERT( m_glDoc.GetCount() == 0 );

	std::list<CSurfComp>::iterator iter = m_listSurfComp.begin();
	for (; iter != m_listSurfComp.end(); ++iter, ++nRow)
	{
		nRow = m_glDoc.AddItem((*iter).m_strFormula).m_nRow;
	
		// name
		ASSERT((*iter).m_strRate_name.GetLength() == 0);
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 2, (*iter).m_strPhase_name);

		// sites_per_mole
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, (*iter).m_dPhase_proportion);

		// surfaces
		long nRow2 = AddSurface((*iter).m_strFormula);
		if (nRow2 != -1)
		{
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 1, (*iter).m_dSpecific_area);
		}
	}
}

BOOL CCSPSurfacePg3::OnInitDialog() 
{
	baseCSPSurfacePg3::OnInitDialog();

	m_btnDensity.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCSPSurfacePg3::InitCLC()
{
	int nItem;

	ASSERT(m_clcSurface.GetItemCount() == 0);

	std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
	for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
	{
		if ( (*elemIter).m_type == CDBElement::typeSURF )
		{
			if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
			{
				// add to columns here
			}
		}
	}

	m_clcSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CCSPSurfacePg3::InitEGSurfTypes()
{
	// must only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set rows, cols
	m_egSurfTypes.SetRows(21);
	m_egSurfTypes.SetCols(0, 4);

	// set column titles
	m_egSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	m_egSurfTypes.SetTextMatrix(0, 2, _T("Kin. reactant"));
	m_egSurfTypes.SetTextMatrix(0, 3, _T("Sites/mol"));

	// set bold titles
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetCellFontBold(TRUE);
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(2);
	m_egSurfTypes.SetCellFontBold(TRUE);
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(3);
	m_egSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfTypes.SetFillStyle(flexFillRepeat);
	m_egSurfTypes.SetRow(0);
	m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetColSel(m_egSurfTypes.GetCols(0) - 1);
	m_egSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfTypes.SetColWidth(1, 0, 1050);
	m_egSurfTypes.SetColWidth(2, 0, 1300);
	m_egSurfTypes.SetColWidth(3, 0, 1000);

	// set row titles
	m_egSurfTypes.AddRowHeaders();

	// scroll to top of grid
	m_egSurfTypes.SetTopRow(1); m_egSurfTypes.SetLeftCol(1);

	// set initial cell
	m_egSurfTypes.SetRow(1); m_egSurfTypes.SetCol(1);

	return TRUE;
}

BOOL CCSPSurfacePg3::InitEGSurfaces()
{
	// must only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set rows, cols
	m_egSurfaces.SetRows(21);
	m_egSurfaces.SetCols(0, 4);

	// set column titles
	m_egSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	m_egSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/mol)"));
	m_egSurfaces.SetTextMatrix(0, 2, _T("Cap. 0-1 plane"));
	m_egSurfaces.SetTextMatrix(0, 3, _T("Cap. 1-2 plane"));

	// set bold titles
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(0);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(1);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(2);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(3);
	m_egSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfaces.SetFillStyle(flexFillRepeat);
	m_egSurfaces.SetRow(0);
	m_egSurfaces.SetCol(1);
	m_egSurfaces.SetColSel(m_egSurfaces.GetCols(0) - 1);
	m_egSurfaces.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfaces.SetColWidth(0, 0, 800);
	m_egSurfaces.SetColWidth(1, 0, 1500);
	if (this->GetSheet()->m_surfaceType == CD_MUSIC)
	{
		m_egSurfaces.SetColWidth(2, 0, 1500);
		m_egSurfaces.SetColWidth(3, 0, 1500);
	}
	else
	{
		m_egSurfaces.SetColWidth(2, 0, 0);
		m_egSurfaces.SetColWidth(3, 0, 0);
	}

	// scroll to top of grid
	m_egSurfaces.SetTopRow(1); m_egSurfaces.SetLeftCol(1);

	// set initial cell
	m_egSurfaces.SetRow(1); m_egSurfaces.SetCol(1);

	return TRUE;
}

void CCSPSurfacePg3::ExchangeEG_CLC(CDataExchange* pDX)
{
	CString format;

	ASSERT( m_clcSurface.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfaces.GetSafeHwnd() != NULL );
	ASSERT( m_glDoc.GetCount() == 0 );

	std::list<CSurfComp>::iterator iter = m_listSurfComp.begin();
	for (long nRow = 1; iter != m_listSurfComp.end(); ++iter, ++nRow)
	{
		nRow = m_glDoc.AddItem((*iter).m_strFormula).m_nRow;

		// name
		ASSERT((*iter).m_strPhase_name.IsEmpty());
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 2, (*iter).m_strRate_name);

		// sites_per_mole
		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), nRow, 3, (*iter).m_dPhase_proportion);

		// surfaces
		long nRow2 = AddSurface((*iter).m_strFormula);
		if (nRow2 != -1)
		{
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 1, (*iter).m_dSpecific_area);
		}
	}	
}

void CCSPSurfacePg1::OnEquilibrate() 
{
	int nItem;

	int nState = m_btnEquilibrate.GetCheck();

	if (m_glDoc.GetCount() > 0)
	{
		if (AfxMessageBox(IDS_SURFACE_302, MB_YESNO) == IDNO)
		{
			nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
			m_btnEquilibrate.SetCheck(nState);
			return;
		}
		else
		{
			m_glDoc.DeleteAllRows();
			m_egSurfaces.ClearWorkSpace();
			for ( long row = m_egSurfaces.GetFixedRows(); row < m_egSurfaces.GetRows(); ++row)
			{
				m_egSurfaces.SetTextMatrix(row, 0, _T(""));
			}
		}
	}

	switch (nState)
	{
	case BST_CHECKED :
		{
			m_cboSolutions.EnableWindow(TRUE);
			m_clcSurface.DeleteAllItems();
			std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
			for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
			{
				if ( (*elemIter).m_type == CDBElement::typeSURF )
				{
					if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strName)) != -1)
					{
						// add to columns here
					}
				}
			}
		}
		break;

	case BST_UNCHECKED :
		{
			m_cboSolutions.EnableWindow(FALSE);
			m_clcSurface.DeleteAllItems();
			std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
			for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
			{
				if ( (*elemIter).m_type == CDBElement::typeSURF )
				{
					if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
					{
						// add to columns here
					}
				}
			}
		}
		break;

	default :
		ASSERT( FALSE );
		break;
	}	
}

void CCSPSurfacePg1::OnCheckDensity() 
{
	if (m_btnDensity.GetSafeHwnd())
	{
		if (m_btnDensity.GetCheck() == BST_CHECKED)
		{
			m_egSurfTypes.SetTextMatrix(0, 2, _T("D(sites/nm^2)"));

		}
		else
		{
			m_egSurfTypes.SetTextMatrix(0, 2, _T("Sites (moles)"));
		}
	}
}

void CCSPSurfacePg1::OnSetfocusCheckDensity()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_SURFACE_629);
	m_eInputDesc.SetWindowText(strRes);
}

BEGIN_EVENTSINK_MAP(CCommonSurfacePage, baseCommonSurfacePage)
    //{{AFX_EVENTSINK_MAP(CCommonSurfacePage)
	ON_EVENT(CCommonSurfacePage, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCommonSurfacePage, IDC_MSHFG_SURFTYPE, -602 /* KeyDown */, OnKeyDownMshfgSurftype, VTS_PI2 VTS_I2)
	ON_EVENT(CCommonSurfacePage, IDC_MSHFG_SURFACES, -602 /* KeyDown */, OnKeyDownMshfgSurfaces, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonSurfacePage::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(strRes, IDS_N_USER_140, _T("surface") ); 
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("surface") ); 
		break;
	case 2 :
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("surface") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

BEGIN_EVENTSINK_MAP(CCSPSurfacePg1, baseCSPSurfacePg1)
    //{{AFX_EVENTSINK_MAP(CCSPSurfacePg1)
	ON_EVENT(CCSPSurfacePg1, IDC_MSHFG_SURFTYPE, 71 /* EnterCell */, OnEnterCellMshfgSurftype, VTS_NONE)
	ON_EVENT(CCSPSurfacePg1, IDC_MSHFG_SURFACES, 71 /* EnterCell */, OnEnterCellMshfgSurfaces, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BEGIN_EVENTSINK_MAP(CCSPSurfacePg3, baseCSPSurfacePg3)
    //{{AFX_EVENTSINK_MAP(CCSPSurfacePg3)
	ON_EVENT(CCSPSurfacePg3, IDC_MSHFG_SURFTYPE, 71 /* EnterCell */, OnEnterCellMshfgSurftype, VTS_NONE)
	ON_EVENT(CCSPSurfacePg3, IDC_MSHFG_SURFACES, 71 /* EnterCell */, OnEnterCellMshfgSurfaces, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonSurfacePage::OnKeyDownMshfgSurfaces(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		m_egSurfaces.SetFillStyle(flexFillRepeat);
		m_egSurfaces.SetText(_T(""));
		m_egSurfaces.SetFillStyle(flexFillSingle);
		break;
	}	
}

void CCommonSurfacePage::OnKeyDownMshfgSurftype(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	int iStartRow;
	int cSelectedRows;

	switch (*KeyCode)
	{
	case VK_DELETE :
		// check if col 1 is included
		if (min(m_egSurfTypes.GetCol(), m_egSurfTypes.GetColSel()) == 1)
		{
			// determine how many rows to delete
			cSelectedRows = abs(m_egSurfTypes.GetRow() - m_egSurfTypes.GetRowSel()) + 1;

			// determine first row to delete
			iStartRow = min(m_egSurfTypes.GetRow(), m_egSurfTypes.GetRowSel());

			// delete each row
			for (int i = 0; i < cSelectedRows; ++i)
			{
				CString strRemove = m_egSurfTypes.GetTextMatrix(iStartRow, 1);
				m_glDoc.DeleteRow(iStartRow);
				RemoveSurface(strRemove);
			}

		}
		else
		{
			// just fill with empty strings
			m_egSurfTypes.SetFillStyle(flexFillRepeat);
			m_egSurfTypes.SetText(_T(""));
			m_egSurfTypes.SetFillStyle(flexFillSingle);
		}
		break;
	}	
}


void CCommonSurfacePage::OnSetfocusClSurface(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER( pNMHDR );

	CString strRes;
	strRes.LoadString(IDS_STRING481);
	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	if (m_clcSurface.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CCSPSurfacePg1::ExchangeEG_CLC()
{
	CString format;

	ASSERT( m_clcSurface.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( m_egSurfaces.GetSafeHwnd() != NULL );


	// verify state of gridlistdoc
	ASSERT( m_glDoc.GetCount() == 0 );

	std::list<CSurfComp>::const_iterator const_iter = m_listSurfComp.begin();
	for (long row = 1; const_iter != m_listSurfComp.end(); ++const_iter, ++row)
	{
		// exchange formula
		row = m_glDoc.AddItem((*const_iter).m_strFormula).m_nRow;		

		// amount
		format.Format("%g", (*const_iter).m_dMoles);
		m_egSurfTypes.SetTextMatrix(row, 2, format);

		// surfaces
		long row2 = AddSurface((*const_iter).m_strFormula);
		if (row2 != -1)
		{
			format.Format("%g", (*const_iter).m_dSpecific_area);
			m_egSurfaces.SetTextMatrix(row2, 1, format);
			format.Format("%g", (*const_iter).m_dGrams);
			m_egSurfaces.SetTextMatrix(row2, 2, format);

			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				format.Format("%g", (*const_iter).m_dCapacitance0);
				m_egSurfaces.SetTextMatrix(row2, 3, format);

				format.Format("%g", (*const_iter).m_dCapacitance1);
				m_egSurfaces.SetTextMatrix(row2, 4, format);
			}
		}
	}

	// verify state of gridlistdoc
	ASSERT( m_listSurfComp.size() == m_glDoc.GetCount() );
}

BOOL CCSPSurfacePg1::InitCLC()
{
	int nItem;

	CCKSSurface* pSheet = GetSheet();

	m_clcSurface.DeleteAllItems();

	if (pSheet->m_nEquilSolutionNum == -1)
	{
		//{{
		ASSERT(m_clcSurface.GetItemCount() == 0);
		// m_clcSurface.DeleteAllItems();
		std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
		for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
		{
			if ( (*elemIter).m_type == CDBElement::typeSURF )
			{
				if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
				{
					// add to columns here
				}
			}
		}
		//}}
	}
	else 
	{
		ASSERT(m_clcSurface.GetItemCount() == 0);
		// m_clcSurface.DeleteAllItems();
		std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
		for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
		{
			if ( (*elemIter).m_type == CDBElement::typeSURF )
			{
				if ((nItem = m_clcSurface.InsertItem(0, (*elemIter).m_strName)) != -1)
				{
					// add to columns here
				}
			}
		}
	}

	m_clcSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CCSPSurfacePg1::InitEGSurfaces()
{
	if ( m_bFirstSetActive == FALSE )
	{
		m_egSurfaces.ClearWorkSpace();
		for ( long row = m_egSurfaces.GetFixedRows(); row < m_egSurfaces.GetRows(); ++row)
		{
			m_egSurfaces.SetTextMatrix(row, 0, _T(""));
		}
		return TRUE;
	}

	// set rows, cols
	m_egSurfaces.SetRows(21);
	m_egSurfaces.SetCols(0, 5);

	// set column titles
	m_egSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	m_egSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/g)"));
	m_egSurfaces.SetTextMatrix(0, 2, _T("Mass (g)"));
	m_egSurfaces.SetTextMatrix(0, 3, _T("Cap. 0-1 plane"));
	m_egSurfaces.SetTextMatrix(0, 4, _T("Cap. 1-2 plane"));

	// set bold titles
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(0);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(1);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(2);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(3);
	m_egSurfaces.SetCellFontBold(TRUE);
	m_egSurfaces.SetRow(0), m_egSurfaces.SetCol(4);
	m_egSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfaces.SetFillStyle(flexFillRepeat);
	m_egSurfaces.SetRow(0);
	m_egSurfaces.SetCol(0);
	m_egSurfaces.SetColSel(m_egSurfaces.GetCols(0) - 1);
	m_egSurfaces.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfaces.SetColWidth(0, 0, 800);
	m_egSurfaces.SetColWidth(1, 0, 1300);
	m_egSurfaces.SetColWidth(2, 0, 920);
	if (this->GetSheet()->m_surfaceType == CD_MUSIC)
	{
		m_egSurfaces.SetColWidth(3, 0, 1500);
		m_egSurfaces.SetColWidth(4, 0, 1500);
	}
	else
	{
		m_egSurfaces.SetColWidth(3, 0, 0);
		m_egSurfaces.SetColWidth(4, 0, 0);
	}

	// scroll to top of grid
	m_egSurfaces.SetTopRow(1); m_egSurfaces.SetLeftCol(1);

	// set initial cell
	m_egSurfaces.SetRow(1); m_egSurfaces.SetCol(1);

	return TRUE;
}

BOOL CCSPSurfacePg1::InitEGSurfTypes()
{
	if (m_bFirstSetActive == FALSE)
	{
		return TRUE;
	}

	// set rows, cols
	m_egSurfTypes.SetRows(20);
	m_egSurfTypes.SetCols(0, 3);

	// set column titles
	m_egSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	if (this->GetSheet()->m_sitesUnits == SITES_DENSITY)
	{
		m_egSurfTypes.SetTextMatrix(0, 2, _T("D(sites/nm^2)"));
	}
	else
	{
		m_egSurfTypes.SetTextMatrix(0, 2, _T("Sites (moles)"));
	}

	// set bold titles
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetCellFontBold(TRUE);
	m_egSurfTypes.SetRow(0), m_egSurfTypes.SetCol(2);
	m_egSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	m_egSurfTypes.SetFillStyle(flexFillRepeat);
	m_egSurfTypes.SetRow(0);
	m_egSurfTypes.SetCol(1);
	m_egSurfTypes.SetColSel(m_egSurfTypes.GetCols(0) - 1);
	m_egSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	m_egSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	m_egSurfTypes.SetColWidth(0, 0, 500);
	m_egSurfTypes.SetColWidth(1, 0, 900);
	m_egSurfTypes.SetColWidth(2, 0, 1300);

	// set row titles
	m_egSurfTypes.AddRowHeaders();

	// scroll to top of grid
	m_egSurfTypes.SetTopRow(1); m_egSurfTypes.SetLeftCol(1);

	// set initial cell
	m_egSurfTypes.SetRow(1); m_egSurfTypes.SetCol(1);

	return TRUE;
}

void CCSPSurfacePg1::OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// do default
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (m_clcSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = m_clcSurface.GetItemText(pNMListView->iItem, 0);
			RemoveSurface(str);
		}
	}

}

void CCSPSurfacePg1::SetEGDefaults(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= m_egSurfTypes.GetFixedRows() );

	if (m_egSurfTypes.GetTextMatrix(nRow, 2).IsEmpty())
	{
		m_egSurfTypes.SetTextMatrix(nRow, 2, _T("0.0"));
	}

	long nRow2 = AddSurface(m_egSurfTypes.GetTextMatrix(nRow, 1));

	if (m_egSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 1, _T("600.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 2, _T("0.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 3, _T("1.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 4).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 4, _T("5.0"));
	}

}

BOOL CCSPSurfacePg1::OnKillActive() 
{
	if (baseCSPSurfacePg1::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CCSPSurfacePg1::ValidateEGSurfaces(CDataExchange* pDX)
{
	CString strDummy;

	for (long nRow = 1; nRow < m_egSurfaces.GetRows(); ++nRow)
	{
		// Line 2: surface binding-site name, sites, specific_area_per_gram, mass
		strDummy = m_egSurfaces.GetTextMatrix(nRow, 0);
		if (!strDummy.IsEmpty())
		{			
			// Read surface area (m^2/g)
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected specific area in m^2/g."), _T("Invalid specific area"));
			}
			double dArea;
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, _T("Specific area must be greater than 0."), _T("Invalid specific area"));
			}

			// Read grams of solid (g)
			double dGrams;
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected mass of solid in grams."), _T("Invalid mass"));
			}
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 2, dGrams);

			if (dGrams <= 0.0)
			{
				DDX_GridFail(pDX, _T("Mass of solid must be greater than 0."), _T("Invalid mass"));
			}

			//{{
			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 3, dCap);

				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 4, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow, 4, dCap);
			}
			//}}
		}
	}
}

void CCSPSurfacePg1::ValidateEGSurfTypes(CDataExchange* pDX)
{
	std::list<CSurfComp> listSurfComp;

	CString strDummy;
	for (long row = 1; row < m_egSurfTypes.GetRows(); ++row)
	{
		// Line 2: surface binding-site name, sites, specific_area_per_gram, mass

		DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), row, 1, strDummy);

		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// sites 
			// Read surface concentration
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), row, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected number of surface sites in moles.."), _T("Invalid surface sites"));
			}
			DDX_GridText(pDX, m_egSurfTypes.GetDlgCtrlID(), row, 2, loSurfComp.m_dMoles);


			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			ASSERT( nRow2 != -1 );
			// validated in ValidateEGSurfaces()
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);


			// Read grams of solid (g)
			// validated in ValidateEGSurfaces()
			DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dGrams);

			if (this->GetSheet()->m_surfaceType == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, m_egSurfaces.GetDlgCtrlID(), nRow2, 4, loSurfComp.m_dCapacitance1);
			}

			listSurfComp.push_back(loSurfComp);
		}
	}
	m_listSurfComp.assign(listSurfComp.begin(), listSurfComp.end());
}

LRESULT CCSPSurfacePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch (pInfo->item.iCol)
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = m_clcSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = m_clcSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}

					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (m_clcSurface.GetItemCount() > 0);
			}
			break;

		default :
			break;

		}
	}
	return bDenyEdit;
}

LRESULT CCSPSurfacePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)m_egSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if ( pInfo->item.pszText == NULL ) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			else if (str.IsEmpty())
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)m_egSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (m_egSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else
	{
		return baseCSPSurfacePg1::OnEndCellEdit(wParam, lParam);
	}
}

LRESULT CCSPSurfacePg1::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);
	UINT nID = wParam;

	switch (nID)
	{
	case IDC_MSHFG_SURFTYPE :
		OnEnterCellMshfgSurftype();
		break;

	case IDC_MSHFG_SURFACES :
		OnEnterCellMshfgSurfaces();
		break;

	default :
		OnEnterCellMshfgNumDesc();
		break;

	}
	return 0;
}

void CCSPSurfacePg1::OnEnterCellMshfgSurftype() 
{
	CString strRes;

	switch (m_egSurfTypes.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_SURFACE_305);
		break;

	case 2:
		if (m_btnDensity.GetCheck() == BST_CHECKED)
		{
			strRes.LoadString(IDS_SURFACE_626);
		}
		else
		{
			strRes.LoadString(IDS_SURFACE_306);
		}
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCSPSurfacePg1::OnEnterCellMshfgSurfaces() 
{
	CString strRes;

	switch (m_egSurfaces.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_SURFACE_307);
		break;

	case 2:
		strRes.LoadString(IDS_SURFACE_308);
		break;

	case 3:
		strRes.LoadString(IDS_SURFACE_627);
		break;

	case 4:
		strRes.LoadString(IDS_SURFACE_628);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCSPSurfacePg1::OnRadioNoES() 
{
	baseCSPSurfacePg1::OnRadioNoES();

	this->m_egSurfaces.SetColWidth(3, 0, 0);
	this->m_egSurfaces.SetColWidth(4, 0, 0);
}

void CCSPSurfacePg1::OnRadioDDL() 
{
	baseCSPSurfacePg1::OnRadioDDL();

	this->m_egSurfaces.SetColWidth(3, 0, 0);
	this->m_egSurfaces.SetColWidth(4, 0, 0);
}

void CCSPSurfacePg1::OnRadioCDMusic(void)
{
	baseCSPSurfacePg1::OnRadioCDMusic();

	this->m_egSurfaces.SetColWidth(3, 0, 1500);
	this->m_egSurfaces.SetColWidth(4, 0, 1500);
}

void CCSPSurfacePg2::OnRadioNoES() 
{
	baseCSPSurfacePg2::OnRadioNoES();

	this->m_egSurfaces.SetColWidth(2, 0, 0);
	this->m_egSurfaces.SetColWidth(3, 0, 0);
}

void CCSPSurfacePg2::OnRadioDDL() 
{
	baseCSPSurfacePg2::OnRadioDDL();

	this->m_egSurfaces.SetColWidth(2, 0, 0);
	this->m_egSurfaces.SetColWidth(3, 0, 0);
}

void CCSPSurfacePg2::OnRadioCDMusic() 
{
	baseCSPSurfacePg2::OnRadioCDMusic();

	this->m_egSurfaces.SetColWidth(2, 0, 1500);
	this->m_egSurfaces.SetColWidth(3, 0, 1500);
}

void CCSPSurfacePg3::OnRadioNoES() 
{
	baseCSPSurfacePg3::OnRadioNoES();

	this->m_egSurfaces.SetColWidth(2, 0, 0);
	this->m_egSurfaces.SetColWidth(3, 0, 0);
}

void CCSPSurfacePg3::OnRadioDDL() 
{
	baseCSPSurfacePg3::OnRadioDDL();

	this->m_egSurfaces.SetColWidth(2, 0, 0);
	this->m_egSurfaces.SetColWidth(3, 0, 0);
}

void CCSPSurfacePg3::OnRadioCDMusic() 
{
	baseCSPSurfacePg3::OnRadioCDMusic();

	this->m_egSurfaces.SetColWidth(2, 0, 1500);
	this->m_egSurfaces.SetColWidth(3, 0, 1500);
}

void CCSPSurfacePg2::OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// do default
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (m_clcSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = m_clcSurface.GetItemText(pNMListView->iItem, 0);
			RemoveSurface(str);
		}
	}
}

void CCSPSurfacePg2::SetEGDefaults(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= m_egSurfTypes.GetFixedRows() );

	if (m_egSurfTypes.GetTextMatrix(nRow, 3).IsEmpty())
	{
		m_egSurfTypes.SetTextMatrix(nRow, 3, _T("0.0"));
	}

	long nRow2 = AddSurface(m_egSurfTypes.GetTextMatrix(nRow, 1));

	if (m_egSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 1, _T("0.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 2, _T("1.0"));
	}
	if (m_egSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		m_egSurfaces.SetTextMatrix(nRow2, 3, _T("5.0"));
	}
}

LRESULT CCSPSurfacePg2::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);
	UINT nID = wParam;

	switch (nID)
	{
	case IDC_MSHFG_SURFTYPE :
		OnEnterCellMshfgSurftype();
		break;
	case IDC_MSHFG_SURFACES :
		OnEnterCellMshfgSurfaces();
		break;
	default :
		OnEnterCellMshfgNumDesc();
		break;
	}

	return 0;
}

BEGIN_EVENTSINK_MAP(CCSPSurfacePg2, baseCSPSurfacePg2)
    //{{AFX_EVENTSINK_MAP(CCSPSurfacePg2)
	ON_EVENT(CCSPSurfacePg2, IDC_MSHFG_SURFTYPE, 71 /* EnterCell */, OnEnterCellMshfgSurftype, VTS_NONE)
	ON_EVENT(CCSPSurfacePg2, IDC_MSHFG_SURFACES, 71 /* EnterCell */, OnEnterCellMshfgSurfaces, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCSPSurfacePg2::OnEnterCellMshfgSurftype() 
{
	CString strRes;

	switch (m_egSurfTypes.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_SURFACE_309);
		break;

	case 2 :
		strRes.LoadString(IDS_SURFACE_310);
		break;

	case 3 :
		strRes.LoadString(IDS_SURFACE_311);
		break;

	case 4 :
		strRes.LoadString(IDS_SURFACE_312);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCSPSurfacePg2::OnEnterCellMshfgSurfaces() 
{
	CString strRes;

	switch (m_egSurfaces.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_SURFACE_313);
		break;

	case 2:
		strRes.LoadString(IDS_SURFACE_627);
		break;

	case 3:
		strRes.LoadString(IDS_SURFACE_628);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

LRESULT CCSPSurfacePg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = m_clcSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = m_clcSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}

					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (m_clcSurface.GetItemCount() > 0);
			}
			break;

		case 2 :
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCSPSurfacePg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)m_egSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if ( pInfo->item.pszText == NULL ) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			//{{
			else if (str.IsEmpty())
			{
				CString strSave = m_egSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				m_egSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			//}}
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)m_egSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (m_egSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else
	{
		return baseCSPSurfacePg2::OnEndCellEdit(wParam, lParam);
	}
}

void CCSPSurfacePg3::OnEnterCellMshfgSurftype() 
{
	CString strRes;

	switch (m_egSurfTypes.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_SURFACE_309);
		break;

	case 2 :
		strRes.LoadString(IDS_SURFACE_310);
		break;

	case 3 :
		strRes.LoadString(IDS_SURFACE_311);
		break;

	case 4 :
		strRes.LoadString(IDS_SURFACE_314);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);		
}

void CCSPSurfacePg3::OnEnterCellMshfgSurfaces() 
{
	CString strRes;

	switch (m_egSurfaces.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_SURFACE_313);
		break;

	case 2:
		strRes.LoadString(IDS_SURFACE_627);
		break;

	case 3:
		strRes.LoadString(IDS_SURFACE_628);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCSPSurfacePg3::OnItemchangedClSurface(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// do default
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (m_clcSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = m_clcSurface.GetItemText(pNMListView->iItem, 0);
			RemoveSurface(str);
		}
	}
}

BOOL CCSPSurfacePg2::OnKillActive() 
{
	if (baseCSPSurfacePg2::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;	
}

BOOL CCSPSurfacePg3::OnKillActive() 
{
	if (baseCSPSurfacePg3::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;	
}

BOOL CCommonSurfacePage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.pt.x             = pHelpInfo->MousePos.x;
	myPopup.pt.y             = pHelpInfo->MousePos.y;
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CHECK_COUNTER_ONLY:
		strRes.LoadString(IDS_STRING486);
		break;
	case IDC_EDIT_THICK: case IDC_STATIC_THICK:
		strRes.LoadString(IDS_STRING485);
		break;
// COMMENT: {10/18/2006 8:40:32 PM}	case IDC_RADIO_NO_EDL: // no_edl
// COMMENT: {10/18/2006 8:40:32 PM}		strRes.LoadString(IDS_STRING482);
// COMMENT: {10/18/2006 8:40:32 PM}		break;
	//{{NEW
	case IDC_RADIO_NO_ES:
		strRes.LoadString(IDS_STRING482);
		break;
	case IDC_R_DM_DL:
		strRes.LoadString(IDS_STRING483);
		break;
	case IDC_R_DM_NO_EXPL:
		strRes.LoadString(IDS_STRING484);
		break;
	//}}NEW
	case IDC_CL_SURFACE:
		strRes.LoadString(IDS_STRING481);
		break;
	case IDC_CB_SOLUTIONS:
		strRes.LoadString(IDS_SURFACE_303);
		break;
	case IDC_EQUILIBRATE:
		strRes.LoadString(IDS_SURFACE_304);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			AfxFormatString1(strRes, IDS_N_USER_140, _T("surface") ); 
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("surface") ); 
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("surface") ); 
			break;
		default :
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CCSPSurfacePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	//{{ HANDLED BY CCommonSurfacePage
	case IDC_STATIC_THICK:
	case IDC_CHECK_COUNTER_ONLY:
	case IDC_EDIT_THICK:
	case IDC_RADIO_DEFAULT:
	case IDC_RADIO_NO_EDL:
	case IDC_RADIO_DIFFUSE:
	case IDC_CL_SURFACE:
	case IDC_CB_SOLUTIONS:
	case IDC_EQUILIBRATE:
	case IDC_MSHFG_NUM_DESC:
	//{{NEW
	case IDC_RADIO_NO_ES:
	case IDC_R_DM_DL:
	case IDC_R_DM_NO_EXPL:
	//}}NEW
		return CCommonSurfacePage::OnHelpInfo(pHelpInfo);
		break;
	//}} HANDLED BY CCommonSurfacePage
	case IDC_MSHFG_SURFTYPE:
		if (!IsContextHelp())
		{
			if (!(m_egSurfTypes.GetRowIsVisible(m_egSurfTypes.GetRow()) && m_egSurfTypes.GetColIsVisible(m_egSurfTypes.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfTypes.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfTypes.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfTypes.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfTypes.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfTypes.GetMouseCol() : m_egSurfTypes.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_305);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_306);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_MSHFG_SURFACES:
		if (!IsContextHelp())
		{
			if (!(m_egSurfaces.GetRowIsVisible(m_egSurfaces.GetRow()) && m_egSurfaces.GetColIsVisible(m_egSurfaces.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfaces.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfaces.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfaces.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfaces.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfaces.GetMouseCol() : m_egSurfaces.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_307);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_308);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CCSPSurfacePg2::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure

	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	//{{ HANDLED BY CCommonSurfacePage
	case IDC_STATIC_THICK:
	case IDC_CHECK_COUNTER_ONLY:
	case IDC_EDIT_THICK:
	case IDC_RADIO_DEFAULT:
	case IDC_RADIO_NO_EDL:
	case IDC_RADIO_DIFFUSE:
	case IDC_CL_SURFACE:
	case IDC_CB_SOLUTIONS:
	case IDC_EQUILIBRATE:
	case IDC_MSHFG_NUM_DESC:
	//{{NEW
	case IDC_RADIO_NO_ES:
	case IDC_R_DM_DL:
	case IDC_R_DM_NO_EXPL:
	//}}NEW
		return CCommonSurfacePage::OnHelpInfo(pHelpInfo);
		break;
	//}} HANDLED BY CCommonSurfacePage
	case IDC_MSHFG_SURFTYPE:
		if (!IsContextHelp())
		{
			if (!(m_egSurfTypes.GetRowIsVisible(m_egSurfTypes.GetRow()) && m_egSurfTypes.GetColIsVisible(m_egSurfTypes.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfTypes.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfTypes.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfTypes.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfTypes.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfTypes.GetMouseCol() : m_egSurfTypes.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_SURFACE_309);
			break;
		case 2 :
			strRes.LoadString(IDS_SURFACE_310);
			break;
		case 3 :
			strRes.LoadString(IDS_SURFACE_311);
			break;
		case 4 :
			strRes.LoadString(IDS_SURFACE_312);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_MSHFG_SURFACES:
		if (!IsContextHelp())
		{
			if (!(m_egSurfaces.GetRowIsVisible(m_egSurfaces.GetRow()) && m_egSurfaces.GetColIsVisible(m_egSurfaces.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfaces.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfaces.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfaces.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfaces.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfaces.GetMouseCol() : m_egSurfaces.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_SURFACE_313);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CCSPSurfacePg3::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	//{{ HANDLED BY CCommonSurfacePage
	case IDC_STATIC_THICK:
	case IDC_CHECK_COUNTER_ONLY:
	case IDC_EDIT_THICK:
	case IDC_RADIO_DEFAULT:
	case IDC_RADIO_NO_EDL:
	case IDC_RADIO_DIFFUSE:
	case IDC_CL_SURFACE:
	case IDC_CB_SOLUTIONS:
	case IDC_EQUILIBRATE:
	case IDC_MSHFG_NUM_DESC:
	//{{NEW
	case IDC_RADIO_NO_ES:
	case IDC_R_DM_DL:
	case IDC_R_DM_NO_EXPL:
	//}}NEW
		return CCommonSurfacePage::OnHelpInfo(pHelpInfo);
		break;
	//}} HANDLED BY CCommonSurfacePage
	case IDC_MSHFG_SURFTYPE:
		if (!IsContextHelp())
		{
			if (!(m_egSurfTypes.GetRowIsVisible(m_egSurfTypes.GetRow()) && m_egSurfTypes.GetColIsVisible(m_egSurfTypes.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfTypes.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfTypes.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfTypes.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfTypes.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfTypes.GetMouseCol() : m_egSurfTypes.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_SURFACE_309);
			break;
		case 2 :
			strRes.LoadString(IDS_SURFACE_310);
			break;
		case 3 :
			strRes.LoadString(IDS_SURFACE_311);
			break;
		case 4 :
			strRes.LoadString(IDS_SURFACE_314);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_MSHFG_SURFACES:
		if (!IsContextHelp())
		{
			if (!(m_egSurfaces.GetRowIsVisible(m_egSurfaces.GetRow()) && m_egSurfaces.GetColIsVisible(m_egSurfaces.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSurfaces.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSurfaces.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSurfaces.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSurfaces.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSurfaces.GetMouseCol() : m_egSurfaces.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_SURFACE_313);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CCommonSurfacePage::OnSize(UINT nType, int cx, int cy) 
{
	baseCommonSurfacePage::OnSize(nType, cx, cy);

	const int x_offset = 7;
	const int y_offset = 0;

	// Dzombak && Morel DDL
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DM_DDL))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_DM_DDL)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);


		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd); // screen coord
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// CD_MUSIC
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_CD_MUSIC))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_CD_MUSIC)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);


		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd); // screen coord
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// Dzombak && Morel DDL -- No explicit diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_R_DM_NO_EXPL))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_NO_EDL)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// Dzombak && Morel DDL -- Borkovec diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_R_DM_DL))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_BORK)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// Dzombak && Morel DDL -- Donnan diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_R_DM_DONNAN))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_DONNAN)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// CD_MUSIC -- No explicit diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_R_CD_NO_EXPL))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_CD_NO_EDL)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// CD_MUSIC -- Donnan diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_R_CD_DONNAN))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_CD_DDL)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}
}

void CCommonSurfacePage::OnBnClickedCheckRetard()
{
	static int DMIds[] = 
	{
		IDC_ST_DM_DDL_VISC,
		IDC_EDIT_DM_DDL_VISC,
	};

	static int CDIds[] = 
	{
		IDC_ST_CD_DDL_VISC,
		IDC_EDIT_CD_DDL_VISC,
	};
	
	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHECK_RETARD) == BST_CHECKED);

	switch (this->GetCheckedRadioButton(IDC_RADIO_NO_ES, IDC_RADIO_CD_MUSIC))
	{
	case IDC_RADIO_NO_ES:
		break;
	case IDC_RADIO_DM_DDL:
		switch (this->GetCheckedRadioButton(IDC_R_DM_NO_EXPL, IDC_R_DM_DONNAN))
		{
		case IDC_R_DM_NO_EXPL:
			// do nothing
			ASSERT(!this->GetDlgItem(IDC_ST_DM_DDL_VISC)->IsWindowEnabled());
			ASSERT(!this->GetDlgItem(IDC_EDIT_DM_DDL_VISC)->IsWindowEnabled());
			break;
		case IDC_R_DM_DL: case IDC_R_DM_DONNAN:
			for (int i = 0; i < sizeof(DMIds) / sizeof(DMIds[0]); ++i)
			{
				if (CWnd *pWnd = this->GetDlgItem(DMIds[i]))
				{
					pWnd->EnableWindow(bEnable);
				}
			}
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;
	case IDC_RADIO_CD_MUSIC:
		switch (this->GetCheckedRadioButton(IDC_R_CD_NO_EXPL, IDC_R_CD_DONNAN))
		{
		case IDC_R_CD_NO_EXPL:
			// do nothing
			ASSERT(!this->GetDlgItem(IDC_ST_CD_DDL_VISC)->IsWindowEnabled());
			ASSERT(!this->GetDlgItem(IDC_EDIT_CD_DDL_VISC)->IsWindowEnabled());
			break;
		case IDC_R_CD_DONNAN:
			for (int i = 0; i < sizeof(CDIds) / sizeof(CDIds[0]); ++i)
			{
				if (CWnd *pWnd = this->GetDlgItem(CDIds[i]))
				{
					pWnd->EnableWindow(bEnable);
				}
			}
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

}

void CCommonSurfacePage::OnBnClickedRadioDdlThick()
{
	static int enableIds[] = 
	{
		IDC_EDIT_DDL_THICK,
	};

	static int disableIds[] = 
	{
		IDC_EDIT_DDL_DEBYE_L,
		IDC_STATIC_DDL_LIMIT,
		IDC_EDIT_DDL_LIMIT,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}
}

void CCommonSurfacePage::OnBnClickedRadioDdlDebyeL()
{
	static int enableIds[] = 
	{
		IDC_EDIT_DDL_DEBYE_L,
		IDC_STATIC_DDL_LIMIT,
		IDC_EDIT_DDL_LIMIT,
	};

	static int disableIds[] = 
	{
		IDC_EDIT_DDL_THICK,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}
}

void CCommonSurfacePage::OnBnClickedRCdDonnanTh()
{
	static int enableIds[] = 
	{
		IDC_EDIT_CD_DDL_THICK,
	};

	static int disableIds[] = 
	{
		IDC_EDIT_CD_DDL_LENGTH,
		IDC_ST_CD_DDL_LIMIT,
		IDC_EDIT_CD_DDL_LIMIT,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}
}

void CCommonSurfacePage::OnBnClickedRCdDonnanDb()
{
	static int enableIds[] = 
	{
		IDC_EDIT_CD_DDL_LENGTH,
		IDC_ST_CD_DDL_LIMIT,
		IDC_EDIT_CD_DDL_LIMIT,
	};

	static int disableIds[] = 
	{
		IDC_EDIT_CD_DDL_THICK,
	};

	int i;

	for (i = 0; i < sizeof(enableIds) / sizeof(enableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(enableIds[i]))
		{
			pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < sizeof(disableIds) / sizeof(disableIds[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(disableIds[i]))
		{
			pWnd->EnableWindow(FALSE);
		}
	}
}
