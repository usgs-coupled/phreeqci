// KPKnobsPg1.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KPKnobsPg1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPKnobsPg1, baseCKPKnobsPg1)


/////////////////////////////////////////////////////////////////////////////
// CKPKnobsPg1 property page

CKPKnobsPg1::CKPKnobsPg1() : baseCKPKnobsPg1(CKPKnobsPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPKnobsPg1)
	m_nItmax                = 100;
	m_dConvergenceTolerance = 1e-008;
	m_dIneqTol              = 1e-015;
	m_dStepSize             = 100.0;
	m_dPeStepSize           = 10.0;
	//}}AFX_DATA_INIT
#ifdef ENABLE_SCALE_PURE_PHASES
	m_dPPScale              = 1.0;
#endif
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_IS;
	}
}

CKPKnobsPg1::~CKPKnobsPg1()
{
}

void CKPKnobsPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPKnobsPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPKnobsPg1)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_SPIN_ITER, m_spinIterations);
	DDX_Text(pDX, IDC_EDIT_ITER, m_nItmax);
	DDV_MinMaxUInt(pDX, m_nItmax, 1, INT_MAX);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		UDACCEL uda[7];
		uda[0].nSec = 0;
		uda[0].nInc = 1;
		uda[1].nSec = 2;
		uda[1].nInc = 2;
		uda[2].nSec = 4;
		uda[2].nInc = 10;
		uda[3].nSec = 8;
		uda[3].nInc = 100;
		uda[4].nSec = 16;
		uda[4].nInc = 1000;
		uda[5].nSec = 32;
		uda[5].nInc = 10000;
		uda[6].nSec = 64;
		uda[6].nInc = 100000;

		// init spinners
		m_spinIterations.SetRange32(1, INT_MAX - 1);
		m_spinIterations.SetAccel(7, uda);
	}

	// -convergence_tolerance
	DDX_Text(pDX, IDC_EDIT_CONV_TOL, m_dConvergenceTolerance);
	if (pDX->m_bSaveAndValidate && m_dConvergenceTolerance < 0.0)
	{
		// Convergence tolerance must be greater than zero.
		::AfxMessageBox(IDS_STRING594);
		pDX->Fail();
	}

	// -tolerance
	DDX_Text(pDX, IDC_EDIT_TOL, m_dIneqTol);
	if (pDX->m_bSaveAndValidate && m_dIneqTol < 0.0)
	{
		// Tolerance must be greater than zero.
		::AfxMessageBox(IDS_STRING595);
		pDX->Fail();
	}

	// -step_size
	DDX_Text(pDX, IDC_EDIT_STEP_SIZE, m_dStepSize);
	if (pDX->m_bSaveAndValidate && m_dStepSize < 0.0)
	{
		// Step size must be greater than zero.
		::AfxMessageBox(IDS_STRING596);
		pDX->Fail();
	}

	// -pe_step_size 
	DDX_Text(pDX, IDC_EDIT_PE_STEP_SIZE, m_dPeStepSize);
	if (pDX->m_bSaveAndValidate && m_dPeStepSize < 0.0)
	{
		// pE step size must be greater than zero.
		::AfxMessageBox(IDS_STRING597);
		pDX->Fail();
	}

#ifdef ENABLE_SCALE_PURE_PHASES
	// -scale_pure_phases 
	DDX_Text(pDX, IDC_EDIT_PP_SCALE, m_dPPScale);
	if (pDX->m_bSaveAndValidate && m_dPPScale < 0.0)
	{
		// Scale pure phases must be greater than zero.
		::AfxMessageBox(IDS_STRING598);
		pDX->Fail();
	}
#endif


	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		int nValue = static_cast<int>(m_arrValue[i]);
		DDX_CBIndex(pDX, IDC_CBO_00 + i, nValue);
		if (pDX->m_bSaveAndValidate)
		{
			m_arrValue[i] = static_cast<ValueType>(nValue);
			ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		}
	}
}


BEGIN_MESSAGE_MAP(CKPKnobsPg1, baseCKPKnobsPg1)
	//{{AFX_MSG_MAP(CKPKnobsPg1)
	ON_BN_CLICKED(IDC_B_AS_IS, OnBAsIs)
	ON_BN_CLICKED(IDC_B_TRUE, OnBTrue)
	ON_BN_CLICKED(IDC_B_FALSE, OnBFalse)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_ITER, IDC_EDIT_PP_SCALE, OnSetFocusEdit)
	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_CBO_00, IDC_CBO_07, OnSetfocusCbo)
END_MESSAGE_MAP()

void CKPKnobsPg1::OnSetfocusCbo(UINT nID) 
{
	CString strRes;
	CString strRes2;
	switch (nID)
	{
	case IDC_CBO_00: // -diagonal_scale
		VERIFY(strRes.LoadString(IDS_STRING604));
		break;

	case IDC_CBO_01: // -debug_model
		VERIFY(strRes.LoadString(IDS_STRING607));
		VERIFY(strRes2.LoadString(IDS_STRING608));
		strRes += strRes2;
		break;

	case IDC_CBO_02: // -debug_prep
		VERIFY(strRes.LoadString(IDS_STRING609));
		break;

	case IDC_CBO_03: // -debug_set
		VERIFY(strRes.LoadString(IDS_STRING610));
		break;

	case IDC_CBO_04: // -debug_inverse
		VERIFY(strRes.LoadString(IDS_STRING606));
		break;

	case IDC_CBO_05: // -logfile
		VERIFY(strRes.LoadString(IDS_STRING611));
		VERIFY(strRes2.LoadString(IDS_STRING612));
		strRes += strRes2;
		break;

	case IDC_CBO_06: // -debug_diffuse_layer
		VERIFY(strRes.LoadString(IDS_STRING605));
		break;

	case IDC_CBO_07: // -delay_mass_water
		// TODO
		VERIFY(strRes.LoadString(IDS_STRING614));
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPKnobsPg1::OnSetFocusEdit(UINT nID) 
{
	CString strRes;
	switch (nID)
	{
	case IDC_EDIT_ITER:
		VERIFY(strRes.LoadString(IDS_STRING599));
		break;

	case IDC_EDIT_CONV_TOL:
		VERIFY(strRes.LoadString(IDS_STRING600));
		break;

	case IDC_EDIT_TOL:
		VERIFY(strRes.LoadString(IDS_STRING601));
		break;

	case IDC_EDIT_STEP_SIZE:
		VERIFY(strRes.LoadString(IDS_STRING602));
		break;

	case IDC_EDIT_PE_STEP_SIZE:
		VERIFY(strRes.LoadString(IDS_STRING603));
		break;

	case IDC_EDIT_PP_SCALE:
		// TODO
		VERIFY(strRes.LoadString(IDS_STRING613));
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}


void CKPKnobsPg1::OnBAsIs() 
{
	CComboBox* pCBO = NULL;
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		pCBO = (CComboBox*)GetDlgItem(IDC_CBO_00 + i);
		if (pCBO) pCBO->SetCurSel(AS_IS);
	}
}

void CKPKnobsPg1::OnBTrue() 
{
	CComboBox* pCBO = NULL;
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		pCBO = (CComboBox*)GetDlgItem(IDC_CBO_00 + i);
		if (pCBO) pCBO->SetCurSel(AS_TRUE);
	}
}

void CKPKnobsPg1::OnBFalse() 
{
	CComboBox* pCBO = NULL;
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		pCBO = (CComboBox*)GetDlgItem(IDC_CBO_00 + i);
		if (pCBO) pCBO->SetCurSel(AS_FALSE);
	}
}

BOOL CKPKnobsPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	CString strRes2;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_B_AS_IS:
		VERIFY(strRes.LoadString(IDS_PRINT_B_0));
		break;

	case IDC_B_TRUE:
		VERIFY(strRes.LoadString(IDS_PRINT_B_1));
		break;

	case IDC_B_FALSE:
		VERIFY(strRes.LoadString(IDS_PRINT_B_2));
		break;

	case IDC_EDIT_ITER: case IDC_STATIC_ITER: case IDC_SPIN_ITER:
		VERIFY(strRes.LoadString(IDS_STRING599));
		break;

	case IDC_EDIT_CONV_TOL: case IDC_STATIC_CONV_TOL:
		VERIFY(strRes.LoadString(IDS_STRING600));
		break;

	case IDC_EDIT_TOL: case IDC_STATIC_TOL:
		VERIFY(strRes.LoadString(IDS_STRING601));
		break;

	case IDC_EDIT_STEP_SIZE: case IDC_STATIC_STEP_SIZE:
		VERIFY(strRes.LoadString(IDS_STRING602));
		break;

	case IDC_EDIT_PE_STEP_SIZE: case IDC_STATIC_PE_STEP_SIZE:
		VERIFY(strRes.LoadString(IDS_STRING603));
		break;

#ifdef ENABLE_SCALE_PURE_PHASES
	case IDC_EDIT_PP_SCALE: case IDC_STATIC_PP_SCALE:
		VERIFY(strRes.LoadString(IDS_STRING613));
		break;
#endif

	case IDC_CBO_00: case IDC_STATIC_00: // -diagonal_scale
		VERIFY(strRes.LoadString(IDS_STRING604));
		break;

	case IDC_CBO_01: case IDC_STATIC_01: // -debug_model
		VERIFY(strRes.LoadString(IDS_STRING607));
		VERIFY(strRes2.LoadString(IDS_STRING608));
		strRes += strRes2;
		break;

	case IDC_CBO_02: case IDC_STATIC_02: // -debug_prep
		VERIFY(strRes.LoadString(IDS_STRING609));
		break;

	case IDC_CBO_03: case IDC_STATIC_03: // -debug_set
		VERIFY(strRes.LoadString(IDS_STRING610));
		break;

	case IDC_CBO_04: case IDC_STATIC_04: // -debug_inverse
		VERIFY(strRes.LoadString(IDS_STRING606));
		break;

	case IDC_CBO_05: case IDC_STATIC_05: // -logfile
		VERIFY(strRes.LoadString(IDS_STRING611));
		VERIFY(strRes2.LoadString(IDS_STRING612));
		strRes += strRes2;
		break;

	case IDC_CBO_06: case IDC_STATIC_06: // -debug_diffuse_layer
		VERIFY(strRes.LoadString(IDS_STRING605));
		break;

	case IDC_CBO_07: case IDC_STATIC_07: // -delay_mass_water
		// TODO
		VERIFY(strRes.LoadString(IDS_STRING614));
		break;

	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}


	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPKnobsPg1::OnInitDialog() 
{
	baseCKPKnobsPg1::OnInitDialog();
	
	CreateRoot(VERTICAL, 5, 6) 
		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, 0, 10, 4/*16*/, 0)
			<< (pane(VERTICAL, GREEDY, 0, 0, 0)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_ITER, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_ITER, NORESIZE | ALIGN_CENTER)
					<< item(IDC_SPIN_ITER, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_CONV_TOL, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_CONV_TOL, NORESIZE | ALIGN_CENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_ITER)
					<< itemGrowing(HORIZONTAL)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_TOL, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_TOL, NORESIZE | ALIGN_CENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_ITER)
					<< itemGrowing(HORIZONTAL)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_STEP_SIZE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_STEP_SIZE, NORESIZE | ALIGN_CENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_ITER)
					<< itemGrowing(HORIZONTAL)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_PE_STEP_SIZE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_PE_STEP_SIZE, NORESIZE | ALIGN_CENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_ITER)
					<< itemGrowing(HORIZONTAL)
					)
#ifdef ENABLE_SCALE_PURE_PHASES
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_PP_SCALE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_EDIT_PP_SCALE, NORESIZE | ALIGN_CENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_ITER)
					<< itemGrowing(HORIZONTAL)
					)
#endif
				<< itemFixed(VERTICAL, 7)
				<< (paneCtrl(IDC_STATIC_OUT_OPTS, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< item(IDC_STATIC_00, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_00, NORESIZE | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_STATIC_04, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_04, NORESIZE | ALIGN_CENTER)
						)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< item(IDC_STATIC_01, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_01, NORESIZE | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_STATIC_05, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_05, NORESIZE | ALIGN_CENTER)
						)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< item(IDC_STATIC_02, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_02, NORESIZE | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_STATIC_06, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_06, NORESIZE | ALIGN_CENTER)
						)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< item(IDC_STATIC_03, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_03, NORESIZE | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_STATIC_07, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_CBO_07, NORESIZE | ALIGN_CENTER)
						)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_B_AS_IS, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_B_TRUE, NORESIZE | ALIGN_CENTER)
						<< itemFixed(HORIZONTAL, 10)
						<< item(IDC_B_FALSE, NORESIZE | ALIGN_CENTER)
						)
					)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
