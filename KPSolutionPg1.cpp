// KPSolutionPg1.cpp : implementation file
//
// $Date: 2004/09/20 21:17:46 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSolutionPg1.h"

#include "Util.h"
#include "DelayUpdate.h"
#include "CommonKeywordSheet.h"
#include "SelectElementsDlg.h"
#include "phreeqci2.h"
#include "RichDocIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSolutionPg1, baseKPSolutionPg1)
IMPLEMENT_DYNCREATE(CKPSolutionPg2, baseKPSolutionPg2)
IMPLEMENT_DYNCREATE(CKPSolutionPg3, baseKPSolutionPg3)

// phreeqc routines
extern "C" {
	int parse_couple(char *token);
}



/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg1 property page

CKPSolutionPg1::CKPSolutionPg1() : baseKPSolutionPg1(CKPSolutionPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSolutionPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	InitValues();
}

void CKPSolutionPg1::InitValues()
{
	m_dPH = 7.0;
	m_strPH_Equil = _T("");
	m_dPH_SI = std::numeric_limits<double>::signaling_NaN();

	m_dPE = 4.0;
	m_strPE_Equil = _T("");
	m_dPE_SI = std::numeric_limits<double>::signaling_NaN();

	m_dTemp = 25.0;
	m_strRedox = _T("pe");
	m_dDensity = 1.0;
	m_dWaterMass = 1.0;
}

CKPSolutionPg1::~CKPSolutionPg1()
{
}

#ifdef _DEBUG
CCommonKeywordSheet* CKPSolutionPg1::GetSheet() // non-debug version is inline
{
	CCommonKeywordSheet* pSheet = (CCommonKeywordSheet*)GetParent();
	ASSERT_KINDOF(CCommonKeywordSheet, pSheet);
	return pSheet;
}
#endif

void CKPSolutionPg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolutionPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionPg1)
	DDX_Control(pDX, IDC_DESCRIPTION, m_ctrlDescription);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	//}}AFX_DATA_MAP

	DDX_NumDesc(pDX, IDC_MSHFLEXGRID1);
	DDX_Grid(pDX, IDC_MSHFLEXGRID1);
}

void CKPSolutionPg1::DDX_Grid(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFLEXGRID1);

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	long nCol = 2;

	if (pDX->m_bSaveAndValidate)
	{
		CString strValue;

		// pH
		DDX_GridText(pDX, nIDC, 3, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dPH = 7.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 3, nCol, m_dPH);
// COMMENT: {4/1/2002 12:05:48 PM}			if (!(2.0 <= m_dPH && m_dPH <= 15.))
// COMMENT: {4/1/2002 12:05:48 PM}			{
// COMMENT: {4/1/2002 12:05:48 PM}				DDX_GridFail(pDX, _T("pH must be between 2 and 15"));
// COMMENT: {4/1/2002 12:05:48 PM}			}
		}

		// pH equil
		DDX_GridText(pDX, nIDC, 4, nCol, m_strPH_Equil);
		if (m_strPH_Equil.IsEmpty())
		{
			m_dPH_SI = std::numeric_limits<double>::signaling_NaN();
		}
		else
		{
			// pH equil SI
			DDX_GridText(pDX, nIDC, 5, nCol, m_dPH_SI);
		}

		// pe
		DDX_GridText(pDX, nIDC, 6, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dPE = 4.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 6, nCol, m_dPE);
			if (!(-20.0 < m_dPE && m_dPE < 20.0))
			{
				DDX_GridFail(pDX, _T("pe must be between -20 and 20"));
			}
		}

		// pe equil
		DDX_GridText(pDX, nIDC, 7, nCol, m_strPE_Equil);
		if (m_strPE_Equil.IsEmpty())
		{
			m_dPE_SI = std::numeric_limits<double>::signaling_NaN();
		}
		else
		{
			// pe equil SI
			DDX_GridText(pDX, nIDC, 8, nCol, m_dPE_SI);
		}

		// temp
		DDX_GridText(pDX, nIDC, 9, nCol, m_dTemp);

		// redox
		DDX_GridText(pDX, nIDC, 10, nCol, m_strRedox);
		if (m_strRedox.IsEmpty())
		{
			m_strRedox = _T("pe");
		}
		else
		{
			if (!CUtil::IsValidRedoxCouple(m_strRedox))
			{
				DDX_GridFail(pDX, CUtil::GetLastErrorString());
			}
		}
		
		// density
		DDX_GridText(pDX, nIDC, 11, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dDensity = 1.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 11, nCol, m_dDensity);
			if (!(m_dDensity > 0.0))
			{
				DDX_GridFail(pDX, _T("Density must be a positive number."));
			}
		}

		// mass
		DDX_GridText(pDX, nIDC, 12, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dWaterMass = 1.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 12, nCol, m_dWaterMass);
			if (!(m_dWaterMass > 0.0))
			{
				DDX_GridFail(pDX, _T("Mass must be a positive number."));
			}
		}

	}
	else
	{
		CString strValue;

		// pH
		ASSERT(2.0 <= m_dPH && m_dPH <= 15.0);
		DDX_GridText(pDX, nIDC, 3, nCol, m_dPH);

		// pH equil
#ifdef _DEBUG
		if (m_strPH_Equil.IsEmpty())
			ASSERT(m_dPH_SI == std::numeric_limits<double>::signaling_NaN());
		else
			ASSERT(m_dPH_SI != std::numeric_limits<double>::signaling_NaN());
#endif
		DDX_GridText(pDX, nIDC, 4, nCol, m_strPH_Equil);
		if (!m_strPH_Equil.IsEmpty())
		{
			// expand rows
			m_ctrlGrid.SetTextMatrix(3, 0, _T("-"));
			m_ctrlGrid.SetRowHeight(4, m_ctrlGrid.GetRowHeight(0));
			m_ctrlGrid.SetRowHeight(5, m_ctrlGrid.GetRowHeight(0));
		}

		// pH equil SI
		DDX_GridTextNaN(pDX, nIDC, 5, nCol, m_dPH_SI);

		// pe
		ASSERT(-20.0 < m_dPE && m_dPE < 20.0);
		DDX_GridText(pDX, nIDC, 6, nCol, m_dPE);

		// pe equil
#ifdef _DEBUG
		if (m_strPE_Equil.IsEmpty())
			ASSERT(m_dPE_SI == std::numeric_limits<double>::signaling_NaN());
		else
			ASSERT(m_dPE_SI != std::numeric_limits<double>::signaling_NaN());
#endif
		DDX_GridText(pDX, nIDC, 7, nCol, m_strPE_Equil);
		if (!m_strPE_Equil.IsEmpty())
		{
			// expand rows
			m_ctrlGrid.SetTextMatrix(6, 0, _T("-"));
			m_ctrlGrid.SetRowHeight(7, m_ctrlGrid.GetRowHeight(0));
			m_ctrlGrid.SetRowHeight(8, m_ctrlGrid.GetRowHeight(0));
		}

		// pe equil SI
		DDX_GridTextNaN(pDX, nIDC, 8, nCol, m_dPE_SI);

		// temp
		DDX_GridText(pDX, nIDC, 9, nCol, m_dTemp);

		// redox
		ASSERT(!m_strRedox.IsEmpty());
		DDX_GridText(pDX, nIDC, 10, nCol, m_strRedox);
		
		// density
		ASSERT(m_dDensity > 0.0);
		DDX_GridText(pDX, nIDC, 11, nCol, m_dDensity);

		// mass
		ASSERT(m_dWaterMass > 0.0);
		DDX_GridText(pDX, nIDC, 12, nCol, m_dWaterMass);
	}

	// reset starting position
	m_ctrlGrid.SetRow(m_ctrlGrid.GetFixedRows());
	m_ctrlGrid.SetCol(m_ctrlGrid.GetFixedCols());
}

void CKPSolutionPg1::DDX_NumDesc(CDataExchange *pDX, int nIDC)
{
	// Modified from CCommonKeywordSheet::DDX_NumDesc()
	ASSERT(nIDC == IDC_MSHFLEXGRID1);

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		// Handled by InitGrid
		// InitNumDesc();
	}

	long nCol = 1 + 1;

	if (pDX->m_bSaveAndValidate)
	{
		CString strValue;

		// lower range (m_n_user)
		DDX_GridText(pDX, nIDC, 0, nCol, strValue);
		if (strValue.IsEmpty())
		{
			GetSheet()->m_n_user = 1;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 0, nCol, GetSheet()->m_n_user);

			// Please enter an integer greater than or equal to %1.
			// DDV_MinEqualInt(pDX, GetSheet()->m_n_user, 0);

			// The starting number must be an integer greater than or equal to zero.
			DDV_MinEqualInt(pDX, GetSheet()->m_n_user, 0, IDS_STRING139);
		}

		// upper range (m_n_user_end)
		DDX_GridText(pDX, nIDC, 1, nCol, strValue);
		if (strValue.IsEmpty())
		{
			GetSheet()->m_n_user_end = CCommonKeywordSheet::N_EMPTY;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 1, nCol, GetSheet()->m_n_user_end);

			// Please enter an integer greater than or equal to %1.
			// DDV_MinEqualInt(pDX, GetSheet()->m_n_user_end, GetSheet()->m_n_user);

			// The ending number must be an integer greater than or equal to the starting number.
			DDV_MinEqualInt(pDX, GetSheet()->m_n_user_end, GetSheet()->m_n_user, IDS_STRING138);
		}
	}
	else
	{
		CString strEmpty(_T(""));

		// lower range (m_n_user)
		if (GetSheet()->m_n_user != CCommonKeywordSheet::N_EMPTY)
		{
			DDX_GridText(pDX, nIDC, 0, nCol, GetSheet()->m_n_user);
		}
		else
		{
			DDX_GridText(pDX, nIDC, 0, nCol, strEmpty);
		}

		// upper range (m_n_user_end)
		if (GetSheet()->m_n_user_end != CCommonKeywordSheet::N_EMPTY
			&& 
			GetSheet()->m_n_user != GetSheet()->m_n_user_end)
		{
			DDX_GridText(pDX, nIDC, 1, nCol, GetSheet()->m_n_user_end);
		}
		else
		{
			DDX_GridText(pDX, nIDC, 1, nCol, strEmpty);
		}

		// set intitial starting position
		CMSHFlexGrid* pWnd = (CMSHFlexGrid*)pDX->PrepareOleCtrl(nIDC);
		ASSERT_KINDOF(CMSHFlexGrid, pWnd);
		pWnd->SetRow(0);
		pWnd->SetCol(nCol);

	}

	// description (m_strDesc)
	DDX_GridText(pDX, nIDC, 2, nCol, GetSheet()->m_strDesc);

}

void CKPSolutionPg1::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set title column widths
	m_ctrlGrid.SetColWidth(0, 0, 200);
	m_ctrlGrid.SetColWidth(1, 0, 1550);

	// set edit column width
	CRect rect;
	CDC* pDC = GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_ctrlGrid.GetClientRect(&rect);
	m_ctrlGrid.SetColWidth(2, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) 
		- m_ctrlGrid.GetColWidth(0, 0)  //200
		- m_ctrlGrid.GetColWidth(1, 0)  //1550
		);
	

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, 1, _T("Starting number"));
    m_ctrlGrid.SetTextMatrix( 1, 1, _T("Ending number"));
    m_ctrlGrid.SetTextMatrix( 2, 1, _T("Description"));
	m_ctrlGrid.SetTextMatrix( 3, 0, _T("+"));
    m_ctrlGrid.SetTextMatrix( 3, 1, _T("pH"));
    m_ctrlGrid.SetTextMatrix( 4, 1, _T("  [Equilibrium]"));
    m_ctrlGrid.SetTextMatrix( 5, 1, _T("  [Sat. index]"));
	m_ctrlGrid.SetTextMatrix( 6, 0, _T("+"));
    m_ctrlGrid.SetTextMatrix( 6, 1, _T("pe"));
    m_ctrlGrid.SetTextMatrix( 7, 1, _T("  [Equilibrium]"));
    m_ctrlGrid.SetTextMatrix( 8, 1, _T("  [Sat. index]"));
    m_ctrlGrid.SetTextMatrix( 9, 1, _T("Temperature (°C)"));
    m_ctrlGrid.SetTextMatrix(10, 1, _T("Redox"));
    m_ctrlGrid.SetTextMatrix(11, 1, _T("Density (g/cm^3)"));
	m_ctrlGrid.SetTextMatrix(12, 1, _T("Water mass (kg)"));

	// set alignment
	m_ctrlGrid.SetColAlignment(0, flexAlignCenterCenter);
	m_ctrlGrid.SetRow(3), m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(6), m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRowHeight(4, 0);
	m_ctrlGrid.SetRowHeight(5, 0);
	m_ctrlGrid.SetRowHeight(7, 0);
	m_ctrlGrid.SetRowHeight(8, 0);

	// set bold titles
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(3), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(6), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(9), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);


	// set starting position
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(2);
}

BEGIN_MESSAGE_MAP(CKPSolutionPg1, baseKPSolutionPg1)
	//{{AFX_MSG_MAP(CKPSolutionPg1)
	ON_BN_CLICKED(IDC_RESET, OnResetPressed)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom grid messages
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	// custom setfocus handlers
	ON_BN_SETFOCUS(IDC_RESET, OnSetfocusReset)
END_MESSAGE_MAP()

BOOL CKPSolutionPg1::OnInitDialog() 
{
	baseKPSolutionPg1::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFLEXGRID1, GREEDY)
			<< item(IDC_RESET, NORESIZE)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSolutionPg1::OnSize(UINT nType, int cx, int cy) 
{
	baseKPSolutionPg1::OnSize(nType, cx, cy);
	
	// Add your message handler code here
	// resize the columns within IDC_MSHFLEXGRID1
	if (m_ctrlGrid.GetSafeHwnd())
	{
		long nCol0 = m_ctrlGrid.GetColWidth(0, 0);
		long nCol1 = m_ctrlGrid.GetColWidth(1, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_ctrlGrid.GetClientRect(&rect);
		m_ctrlGrid.SetColWidth(2, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0 - nCol1);
	}	
}

void CKPSolutionPg1::OnSetfocusReset()
{
	// in order for a button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING465);
	m_ctrlDescription.SetWindowText(strRes);
}


void CKPSolutionPg1::OnResetPressed() 
{
	InitValues();
	UpdateData(FALSE);	
}

LRESULT CKPSolutionPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	UNUSED(pInfo);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iRow)
	{
	case 4: case 7:
		pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;

	case 10:
		pInfo->item.bUseCombo = (CUtil::InsertRedox(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;
	}
	return bDenyEdit;
}

LRESULT CKPSolutionPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL) return bMakeChange;

	ASSERT(pInfo->item.iCol == 2);
	switch (pInfo->item.iRow)
	{
	case 4 :	// pH phase name
		if (_tcslen(pInfo->item.pszText) && m_ctrlGrid.GetTextMatrix(4, 2).IsEmpty())
		{
			// set default SI
			m_ctrlGrid.SetTextMatrix(5, 2, _T("0.0"));
		}
		break;
	case 7 :	// pe phase name
		if (_tcslen(pInfo->item.pszText) && m_ctrlGrid.GetTextMatrix(8, 2).IsEmpty())
		{
			// set default SI
			m_ctrlGrid.SetTextMatrix(8, 2, _T("0.0"));
		}
		break;
	}

	// later
	return bMakeChange;
}

LRESULT CKPSolutionPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	ASSERT((int)wParam == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(wParam); // unused in release builds
	UNUSED_ALWAYS(lParam);

	OnEnterCellGrid();
	return 0;
}

BEGIN_EVENTSINK_MAP(CKPSolutionPg1, baseKPSolutionPg1)
    //{{AFX_EVENTSINK_MAP(CKPSolutionPg1)
	ON_EVENT(CKPSolutionPg1, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSolutionPg1, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSolutionPg1, IDC_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPSolutionPg1, IDC_MSHFLEXGRID1, -605 /* MouseDown */, OnMouseDownGrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolutionPg1::OnEnterCellGrid() 
{
	CString strRes;
	switch (m_ctrlGrid.GetRow())
	{
	case 0:
		strRes.LoadString(IDS_STRING452);
		break;
	case 1:
		strRes.LoadString(IDS_STRING453);
		break;
	case 2:
		strRes.LoadString(IDS_STRING454);
		break;
	case 3:
		strRes.LoadString(IDS_STRING455);
		break;		
	case 4:
		strRes.LoadString(IDS_STRING456);
		break;
	case 5:
		strRes.LoadString(IDS_STRING457);
		break;
	case 6:	// pe
		strRes.LoadString(IDS_STRING458);
		break;
	case 7:	// pe phase equil
		strRes.LoadString(IDS_STRING459);
		break;
	case 8:	// pe S.I.
		strRes.LoadString(IDS_STRING460);
		break;
	case 9:
		strRes.LoadString(IDS_STRING461);
		break;
	case 10:
		strRes.LoadString(IDS_STRING462);
		break;
	case 11:	// Density
		strRes.LoadString(IDS_STRING463);
		break;
	case 12:
		strRes.LoadString(IDS_STRING464);
		break;
	}
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	const int PH_ROW = 3;
	const int PE_ROW = 6;

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.SetFillStyle(flexFillRepeat);
		m_ctrlGrid.SetText(_T(""));
		m_ctrlGrid.SetFillStyle(flexFillSingle);
		break;

	//{{
	case VK_RIGHT:
		if (m_ctrlGrid.GetRow() == PH_ROW) {
			CString str = m_ctrlGrid.GetTextMatrix(PH_ROW, 0);
			if (str[0] == _T('+'))
			{
				m_ctrlGrid.SetTextMatrix(PH_ROW, 0, _T("-"));
				m_ctrlGrid.SetRowHeight(PH_ROW + 1, m_ctrlGrid.GetRowHeight(PH_ROW));
				m_ctrlGrid.SetRowHeight(PH_ROW + 2, m_ctrlGrid.GetRowHeight(PH_ROW));
			}
		}
		if (m_ctrlGrid.GetRow() == PE_ROW) {
			CString str = m_ctrlGrid.GetTextMatrix(PE_ROW, 0);
			if (str[0] == _T('+'))
			{
				m_ctrlGrid.SetTextMatrix(PE_ROW, 0, _T("-"));
				m_ctrlGrid.SetRowHeight(PE_ROW + 1, m_ctrlGrid.GetRowHeight(PE_ROW));
				m_ctrlGrid.SetRowHeight(PE_ROW + 2, m_ctrlGrid.GetRowHeight(PE_ROW));
			}
		}

		break;
	case VK_LEFT:
		if (m_ctrlGrid.GetRow() == PH_ROW) {
			CString str = m_ctrlGrid.GetTextMatrix(PH_ROW, 0);
			if (str[0] == _T('-'))
			{
				m_ctrlGrid.SetTextMatrix(PH_ROW, 0, _T("+"));
				m_ctrlGrid.SetRowHeight(PH_ROW + 1, 0);
				m_ctrlGrid.SetRowHeight(PH_ROW + 2, 0);
			}
		}
		if (m_ctrlGrid.GetRow() == PE_ROW) {
			CString str = m_ctrlGrid.GetTextMatrix(PE_ROW, 0);
			if (str[0] == _T('-'))
			{
				m_ctrlGrid.SetTextMatrix(PE_ROW, 0, _T("+"));
				m_ctrlGrid.SetRowHeight(PE_ROW + 1, 0);
				m_ctrlGrid.SetRowHeight(PE_ROW + 2, 0);
			}
		}
		break;
	//}}
	}	
}

void CKPSolutionPg1::OnDblClickGrid() 
{
	if (GetKeyState(VK_LBUTTON) < 0)
		OnMouseDownGrid(1, 0, 0, 0);
}

void CKPSolutionPg1::OnMouseDownGrid(short Button, short Shift, long x, long y) 
{
	// Add your control notification handler code here
	UNUSED(Shift);
	UNUSED(x);
	UNUSED(y);
	if (Button == 1 )
	{
		long nCol = m_ctrlGrid.GetMouseCol();
		if (nCol == 0)
		{
			CString str;
			long nRow = m_ctrlGrid.GetMouseRow();
			switch (nRow)
			{
			case 3: // pH
				str = m_ctrlGrid.GetTextMatrix(nRow, nCol);
				if (str[0] == _T('+'))
				{
					m_ctrlGrid.SetTextMatrix(nRow, nCol, _T("-"));
					m_ctrlGrid.SetRowHeight(4, m_ctrlGrid.GetRowHeight(3));
					m_ctrlGrid.SetRowHeight(5, m_ctrlGrid.GetRowHeight(3));
				}
				else
				{
					m_ctrlGrid.SetTextMatrix(nRow, nCol, _T("+"));
					m_ctrlGrid.SetRowHeight(4, 0);
					m_ctrlGrid.SetRowHeight(5, 0);
				}
				break;
			case 6: // pe
				str = m_ctrlGrid.GetTextMatrix(nRow, nCol);
				if (str[0] == _T('+'))
				{
					m_ctrlGrid.SetTextMatrix(nRow, nCol, _T("-"));
					m_ctrlGrid.SetRowHeight(7, m_ctrlGrid.GetRowHeight(6));
					m_ctrlGrid.SetRowHeight(8, m_ctrlGrid.GetRowHeight(6));
				}
				else
				{
					m_ctrlGrid.SetTextMatrix(nRow, nCol, _T("+"));
					m_ctrlGrid.SetRowHeight(7, 0);
					m_ctrlGrid.SetRowHeight(8, 0);
				}
				break;
			}
		}
	}	
}


/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg2 property page

CKPSolutionPg2::CKPSolutionPg2() : baseKPSolutionPg2(CKPSolutionPg2::IDD)
, m_glDoc(&m_ctrlGrid, &m_ctrlElements, 1)
{
	m_strChargeBalance = _T("(none)");
	m_strDefaultUnits = _T("mmol/kgw");

	//{{AFX_DATA_INIT(CKPSolutionPg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSolutionPg2::~CKPSolutionPg2()
{
}

void CKPSolutionPg2::DoDataExchange(CDataExchange* pDX)
{
	TRACE("CKPSolutionPg2::DoDataExchange\n");
	baseKPSolutionPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionPg2)
	DDX_Control(pDX, IDC_CBO_CHARGE, m_ctrlChargeBalance);
	DDX_Control(pDX, IDC_CBO_DEF_UNITS, m_ctrlDefaultUnits);
	DDX_Control(pDX, IDC_ELEMENT_LIST, m_ctrlElements);
	DDX_Control(pDX, IDC_DESCRIPTION, m_ctrlDescription);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	//}}AFX_DATA_MAP

	// avoid flashing grid
	CDelayUpdate update(this, IDC_MSHFLEXGRID1);

	if (m_bFirstSetActive)
	{
		CUtil::InsertAqElements(&m_ctrlElements, GetDatabase());
		CUtil::InsertAqElements(m_ctrlChargeBalance.m_hWnd, GetDatabase());
		m_ctrlChargeBalance.AddString(_T("(none)"));
		m_ctrlChargeBalance.AddString(_T("(pH)"));
		m_ctrlChargeBalance.AddString(_T("(pe)"));
		CUtil::InsertConcUnits(m_ctrlDefaultUnits.m_hWnd);

		InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	if (pDX->m_bSaveAndValidate)
	{
		// default units
		int nIndex = m_ctrlDefaultUnits.GetCurSel();
		m_ctrlDefaultUnits.GetLBText(nIndex, m_strDefaultUnits);
		m_strDefaultUnits.Replace("kg solution", "kgs");
		m_strDefaultUnits.Replace("kg water", "kgw");

		// charge balance
		nIndex = m_ctrlChargeBalance.GetCurSel();
		m_ctrlChargeBalance.GetLBText(nIndex, m_strChargeBalance);

		bool bChargeBalanceFound = false;
		if (m_strChargeBalance[0] == _T('('))
		{
			bChargeBalanceFound = true;
		}

		LVFINDINFO info;
		info.flags = LVFI_STRING;

		std::list<CConc> listConc;
		for (long nRow = m_ctrlGrid.GetFixedRows(); nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			CString str;

			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 1, str);
			if (str.IsEmpty())
				continue;

			CConc conc;

			//
			// element
			//

			// Find proper name in list
			info.psz = str.GetBuffer(str.GetLength() + 1);
			int nItem = m_ctrlElements.FindItem(&info, -1);
			if (nItem != -1)
			{
				conc.m_strDesc = m_ctrlElements.GetItemText(nItem, 1);
			}
			else
			{
				conc.m_strDesc = info.psz;
				TRACE(_T("WARNING: %s: not in list \n"), conc.m_strDesc);
			}
			str.ReleaseBuffer();

			//
			// Conc.
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 2, str);
			if (str.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Concentration data missing in solution input."));
			}
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 2, conc.m_dInputConc);
			if (conc.m_dInputConc < 0.0)
			{
				DDX_GridFail(pDX, _T("Concentration must be greater than or equal to 0."));
			}

			//
			// Units
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 3, conc.m_strUnits);
			if (conc.m_strUnits.IsEmpty() || conc.m_strUnits.Find(_T("Def")) != -1 || conc.m_strUnits.Find(_T("def")) != -1)
			{
				conc.m_strUnits = _T("(Default)");
			}
			if (conc.m_strUnits.Compare(_T("(Default)")) != 0)
			{
				// check for valid units
				CString strTemp = conc.m_strUnits;

				strTemp.Replace(_T("kgs"), _T("kg solution"));
				strTemp.Replace(_T("kgw"), _T("kg water"));

				// test for alk
				if (conc.m_strDesc.Left(3).CompareNoCase(_T("alk")) == 0)
				{
					strTemp.Replace(_T("eq"), _T("mol"));
				}
				if (m_ctrlDefaultUnits.FindStringExact(0, strTemp) == CB_ERR)
				{
					TRACE("WARNING: didn't find %s units\n", strTemp);
					DDX_GridFail(pDX, _T("Invalid concentration units."));
				}

				int nDefUnits = m_ctrlDefaultUnits.GetCurSel();
				int nConcUnits = m_ctrlDefaultUnits.FindStringExact(0, strTemp);

				bool bValid = true;
				// per liter
				if (0 <= nDefUnits && nDefUnits <= 5 && !(0 <= nConcUnits && nConcUnits <= 5))
					bValid = false;
				// per kg solution
				if (6 <= nDefUnits && nDefUnits <= 14 && !(6 <= nConcUnits && nConcUnits <= 14))
					bValid = false;
				// per kg water
				if (15 <= nDefUnits && nDefUnits <= 20 && !(15 <= nConcUnits && nConcUnits <= 20))
					bValid = false;
				if (!bValid)
				{
					CString strMsg;
					CString strDefUnits = m_strDefaultUnits;
					strDefUnits.Replace(_T("kgs"), _T("kg solution"));
					strDefUnits.Replace(_T("kgw"), _T("kg water"));
					strMsg.Format(_T("Units for master species, %s are not compatible with default units, %s."), strTemp, strDefUnits);
					DDX_GridFail(pDX, strMsg);
				}
			}
			// convert to standard unit
			conc.m_strUnits.Replace(_T("kg solution"), _T("kgs"));
			conc.m_strUnits.Replace(_T("kg water"), _T("kgw"));
			if (conc.m_strUnits.Compare(m_strDefaultUnits) == 0)
			{
				conc.m_strUnits = _T("(Default)");
			}

			//
			// Phase
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 4, conc.m_strPhase);

			if (m_strChargeBalance.Compare(conc.m_strDesc) == 0)
			{
				if (!conc.m_strPhase.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Phase name may not be used if charge has been specified for this element"));
				}
				else
				{
					conc.m_strPhase = _T("charge");
					bChargeBalanceFound = true;
				}
			}
			else if (!conc.m_strPhase.IsEmpty())
			{
				//
				// S.I.
				//
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 5, str);
			
				if (!conc.m_strPhase.IsEmpty() && str.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected saturation index."));
				}
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 5, conc.m_dPhaseSI);
			}

			//
			// As / GFW   ([as formula] or [gfw gfw])
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 6, conc.m_strAs);
			if (!conc.m_strAs.IsEmpty() && !::_istalpha(conc.m_strAs[0]))
			{
				conc.m_strAs.Empty();
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 6, conc.m_dGFW);
			}

			//
			// Redox
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 7, conc.m_strRedox);

			// if here no exception has occured so add to list
			listConc.push_back(conc);
		}

		// Add charge balance if not found
		if (!bChargeBalanceFound)
		{
			CConc conc;
			conc.m_strDesc = m_strChargeBalance;
			conc.m_dInputConc = 0.0;
			conc.m_strPhase = _T("charge");
			listConc.push_front(conc);
		}

		// if here listNameCoef is valid and can be assigned to the member variable
		m_listConc.assign(listConc.begin(), listConc.end());
	}
	else
	{
		/////m_glDoc.DeleteAllRows();
		ASSERT(m_glDoc.GetCount() == 0);

		// default units
		CString strTemp = m_strDefaultUnits;
		strTemp.Replace(_T("s"), _T(" solution"));
		strTemp.Replace(_T("w"), _T(" water"));

		VERIFY(m_ctrlDefaultUnits.SelectString(-1, strTemp) != CB_ERR);

		// charge balance
		VERIFY(m_ctrlChargeBalance.SelectString(-1, m_strChargeBalance) != CB_ERR);

		std::list<CConc>::const_iterator iter = m_listConc.begin();
		CString strFormat;
		int nRow;
		for (; iter != m_listConc.end(); ++iter)
		{
			CConc conc(*iter);

			//
			// element
			//
			if (conc.m_strDesc.Find(_T("(")) == -1)
			{
				CGridListItem item = m_glDoc.AddItem(conc.m_strDesc);
				nRow = item.m_nRow;
				UpdateElementList(item.m_nItem);

			}
			else
			{
				CGridListItem item = m_glDoc.AddSubItem(1, conc.m_strDesc);
				nRow = item.m_nRow;
				UpdateElementList(item.m_nItem);
			}
			if (nRow == -1) continue;	// invalid CConc

			//
			// Conc.
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 2, conc.m_dInputConc);

			//
			// Units
			//
			conc.m_strUnits.Replace(_T("s"), _T(" solution"));
			conc.m_strUnits.Replace(_T("w"), _T(" water"));
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 3, conc.m_strUnits);

			//
			// Phase
			//
			if (conc.m_strPhase.CompareNoCase(_T("charge")) != 0)
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 4, conc.m_strPhase);

				// S.I.
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, 5, conc.m_dPhaseSI);
			}

			//
			// As / GFW		([as formula] or [gfw gfw])
			//
			if (!conc.m_strAs.IsEmpty())
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 6, conc.m_strAs);
			}
			else
			{
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, 6, conc.m_dGFW);
			}

			//
			// Redox
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, 7, conc.m_strRedox);

		}
		ASSERT(m_listConc.size() == m_glDoc.GetCount());
	}
}

void CKPSolutionPg2::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	m_ctrlGrid.SetTextMatrix(0, 1, _T("Element"));
	m_ctrlGrid.SetTextMatrix(0, 2, _T("Conc."));
	m_ctrlGrid.SetTextMatrix(0, 3, _T("[Units]"));

	m_ctrlGrid.SetTextMatrix(0, 4, _T("[Phase]"));
	m_ctrlGrid.SetTextMatrix(0, 5, _T("[S.I.]"));

	m_ctrlGrid.SetTextMatrix(0, 6, _T("[As / GFW]"));
	m_ctrlGrid.SetTextMatrix(0, 7, _T("[Redox]"));

	m_ctrlGrid.AddRowHeaders();

	// set bold titles
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(2);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(3);
	m_ctrlGrid.SetCellFontBold(TRUE);

	// Left Align the units column
	m_ctrlGrid.SetColAlignment(3, flexAlignLeftCenter);

	// Center Align the column headers
	m_ctrlGrid.SetFillStyle(flexFillRepeat);
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetColSel(m_ctrlGrid.GetCols(0) - 1);
	m_ctrlGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlGrid.SetFillStyle(flexFillSingle);


	// set initial row, col
	m_ctrlGrid.SetRow(m_ctrlGrid.GetFixedRows());
	m_ctrlGrid.SetCol(m_ctrlGrid.GetFixedCols());
}

BOOL CKPSolutionPg2::OnInitDialog() 
{
	baseKPSolutionPg2::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_S_DEF_UNITS, NORESIZE|ALIGN_VCENTER)
			<< item(IDC_CBO_DEF_UNITS, NORESIZE|ALIGN_VCENTER)
			<< itemSpaceBetween(HORIZONTAL, IDC_CBO_DEF_UNITS, IDC_S_CHARGE)
			<< item(IDC_S_CHARGE, NORESIZE|ALIGN_VCENTER)
			<< item(IDC_CBO_CHARGE, NORESIZE|ALIGN_VCENTER)
			)

		<< item(IDC_S_SELECT, NORESIZE)

		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MAJORS, NORESIZE)
			<< item(IDC_TRACE, NORESIZE)
			<< item(IDC_CUSTOM, NORESIZE)
			<< item(IDC_BTN_MOD_CUST, NORESIZE)
			)

		<< item(IDC_ELEMENT_LIST, RELATIVE_VERT, 0, 33)
		<< item(IDC_MSHFLEXGRID1, RELATIVE_VERT, 0, 67)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CKPSolutionPg2, baseKPSolutionPg2)
	//{{AFX_MSG_MAP(CKPSolutionPg2)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ELEMENT_LIST, OnItemchangedElementList)
	ON_BN_CLICKED(IDC_MAJORS, OnMajors)
	ON_BN_CLICKED(IDC_TRACE, OnTrace)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_CBN_SELCHANGE(IDC_CBO_DEF_UNITS, OnSelchangeCboDefUnits)
	ON_NOTIFY(NM_SETFOCUS, IDC_ELEMENT_LIST, OnSetfocusElementList)
	ON_CBN_SETFOCUS(IDC_CBO_DEF_UNITS, OnSetfocusCboDefUnits)
	ON_CBN_SETFOCUS(IDC_CBO_CHARGE, OnSetfocusCboCharge)
	ON_WM_KEYDOWN()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BTN_MOD_CUST, OnBtnModCust)
	//}}AFX_MSG_MAP

	// custom setfocus messages
	ON_BN_SETFOCUS(IDC_MAJORS, OnSetfocusMajors)
	ON_BN_SETFOCUS(IDC_TRACE, OnSetfocusTrace)
	ON_BN_SETFOCUS(IDC_CUSTOM, OnSetfocusCustom)
	ON_BN_SETFOCUS(IDC_BTN_MOD_CUST, OnSetfocusBtnModCust)

	// custom grid messages
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	// context menu check list commands
	ON_COMMAND_RANGE(ID_VIEW_AS_LIST, ID_LIST_CLEAR_ALL, OnViewChange)
END_MESSAGE_MAP()

void CKPSolutionPg2::OnSetfocusMajors() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING468);
	m_ctrlDescription.SetWindowText(strRes);
}
void CKPSolutionPg2::OnSetfocusTrace() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING469);
	m_ctrlDescription.SetWindowText(strRes);
}
void CKPSolutionPg2::OnSetfocusCustom() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING567);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg2::OnSetfocusBtnModCust() 
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING569);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg2::OnSetfocusCboDefUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_SOL_204);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg2::OnSetfocusCboCharge() 
{
	CString strRes;
	strRes.LoadString(IDS_SOL_205);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg2::OnSetfocusElementList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlElements.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;
	AfxFormatString1(strRes, IDS_CL_167, _T("element(s)") );
	strRes += _T(" Right-click to display details");
	m_ctrlDescription.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlElements.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlElements.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}

LRESULT CKPSolutionPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	int nID = wParam;
	ASSERT( nID == m_ctrlGrid.GetDlgCtrlID() );
	OnEnterCellGrid();
	return 0;
}

LRESULT CKPSolutionPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	// check if valid entry
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	int nID = wParam;
	ASSERT( nID == m_ctrlGrid.GetDlgCtrlID() );

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL) return bMakeChange;

	LVFINDINFO info;

	if (pInfo->item.iCol == 1)
	{
		CString strNew = pInfo->item.pszText;

		info.flags = LVFI_STRING;
		info.psz   = strNew.GetBuffer(strNew.GetLength());

		int nItem = m_ctrlElements.FindItem(&info);
		if (nItem == -1)
		{
			nItem = m_ctrlElements.FindSubItem(1, strNew);
		}

		if (nItem != -1)
		{
			if (m_ctrlElements.GetCheckBox(nItem) == BST_INDETERMINATE)
			{
				CString strElement = m_ctrlElements.GetItemText(nItem, 1);
				if (strElement.Find("(") != -1)
				{
					CString strMsg = CString("Cannot enter both total ") + strElement.SpanExcluding("(") + " and " + strElement + ".";
					::AfxMessageBox(strMsg, MB_ICONEXCLAMATION);
				}
				else
				{
					// Find first checked redox item
					CString strRedox;
					for (int j = nItem + 1; ; ++j)
					{
						if (m_ctrlElements.GetCheckBox(j) == BST_CHECKED)
						{
							strRedox = m_ctrlElements.GetItemText(j, 1);
							break;
						}
					}
					ASSERT( strRedox.Find("(") != -1 );
					CString strMsg = CString("Cannot enter both total ") + strElement + " and " + strRedox + ".";
					::AfxMessageBox(strMsg, MB_ICONEXCLAMATION);
				}
				return FALSE;
			}
		}
		// fill in defaults
		if (m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
			m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, 2, _T("0.0"));
		if (m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, 3).IsEmpty())
			m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, 3, _T("(Default)"));
	}
	return m_glDoc.OnEndCellEdit(wParam, lParam, 1);
}


LRESULT CKPSolutionPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iCol)
	{
	case 1:	// element
		pInfo->item.bUseCombo = (CUtil::InsertAqElements(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;

	case 3: // units
		if (pInfo->item.hWndCombo)
		{
			CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);

			// clear combo
			pCombo->ResetContent();

			// keep track of widest string
			CDC* pDC = pCombo->GetDC();
			CSize size(0, 0);
			pCombo->AddString(_T("(Default)"));
			long nWidest = pDC->GetTextExtent("(Default)").cx;

			// get current default units
			int nCurrent = m_ctrlDefaultUnits.GetCurSel();

			// check for alkalinity
			CString str = m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, 1);
			bool bAlk = (str.Left(3).CompareNoCase("alk") == 0);

			int i;
			if (0 <= nCurrent && nCurrent <= 5)
			{
				// per liter
				for (i = 0; i <= 5; ++i)
				{
					m_ctrlDefaultUnits.GetLBText(i, str);
					if (bAlk)
					{
						str.Replace("mol", "eq");
					}
					pCombo->AddString(str);
					size = pDC->GetTextExtent(str);
					if (nWidest < size.cx )
					{
						nWidest = size.cx;
					}
				}
			}
			else if (6 <= nCurrent && nCurrent <= 14)
			{
				// per kg solution
				for (i = 6; i <= 14; ++i)
				{
					m_ctrlDefaultUnits.GetLBText(i, str);
					if (bAlk)
					{
						str.Replace("mol", "eq");
					}
					pCombo->AddString(str);
					size = pDC->GetTextExtent(str);
					if (nWidest < size.cx )
					{
						nWidest = size.cx;
					}
				}
			}
			else if (15 <= nCurrent && nCurrent <= 20)
			{
				// per kg water
				for (i = 15; i <= 20; ++i)
				{
					m_ctrlDefaultUnits.GetLBText(i, str);
					if (bAlk)
					{
						str.Replace("mol", "eq");
					}
					pCombo->AddString(str);
					size = pDC->GetTextExtent(str);
					if (nWidest < size.cx )
					{
						nWidest = size.cx;
					}
				}
			}
			pCombo->SetDroppedWidth(nWidest);
			pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
		}
		else
		{
			pInfo->item.bUseCombo = TRUE;
		}
		break;

	case 4:	// phase
		pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;

	case 7:	// redox
		pInfo->item.bUseCombo = (CUtil::InsertRedox(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;
	}
	return bDenyEdit;	// allow edit
}


void CKPSolutionPg2::OnItemchangedElementList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Note:
	// This function IS called when items are checked from the 
	// OnInitDialog function
	//
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	//{{ FIX
	static bool bIgnoreNotifications;
	//}} FIX

	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		UpdateElementList(pNMListView->iItem);
	}

	// do default
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	if (row != -1) {
		if (m_ctrlGrid.GetTextMatrix(row, 2).IsEmpty())
			m_ctrlGrid.SetTextMatrix(row, 2, _T("0.0"));
		if (m_ctrlGrid.GetTextMatrix(row, 3).IsEmpty())
			m_ctrlGrid.SetTextMatrix(row, 3, _T("(Default)"));
	}
	return;
}

void CKPSolutionPg2::UpdateElementList(int nItem) 
{
	if (nItem == -1) return;

	CString str = m_ctrlElements.GetItemText(nItem, 1);
	ASSERT( !str.IsEmpty() );

	LVFINDINFO info;

	switch (m_ctrlElements.GetCheckBox(nItem))
	{
	case BST_UNCHECKED:

		// check if redox state
		if (str.Find(_T("("), 0) != -1)
		{
			CString strTotalElem = str.SpanExcluding(_T("("));

			// Find total element state (nTotalElem)
			info.flags = LVFI_STRING;
			info.psz   = strTotalElem.GetBuffer(strTotalElem.GetLength());
			int nTotalElem = m_ctrlElements.FindItem(&info, -1);

			if (nTotalElem != -1)
			{
				// Note: This assumes that col 1 is sorted
				CString strRedox;
				for (int i = nTotalElem + 1; ; ++i)
				{
					strRedox = m_ctrlElements.GetItemText(i, 1);
					if (strRedox.Find(strTotalElem + _T("("), 0) != 0)
					{
						m_ctrlElements.SetCheckBox(nTotalElem, BST_UNCHECKED);
						break;
					}
					if (m_ctrlElements.GetCheckBox(i) == BST_CHECKED)
						break;
				}
			}
		}
		else
		{
			// Note: This assumes that col 1 is sorted
			CString strRedox;
			for (int i = nItem + 1; ; ++i)
			{
				strRedox = m_ctrlElements.GetItemText(i, 1);
				if (strRedox.Find(str + _T("("), 0) != 0)
					break;
				m_ctrlElements.SetCheckBox(i, BST_UNCHECKED);
			}
		}
		break;

	case BST_CHECKED:

		// check if redox state
		if (str.Find(_T("("), 0) != -1)
		{
			// turn off total element
			CString strElem = str.SpanExcluding(_T("("));

			info.flags = LVFI_STRING;
			info.psz = strElem.GetBuffer(strElem.GetLength());
			nItem = m_ctrlElements.FindItem(&info);

			if (nItem != -1)
			{
				m_ctrlElements.SetCheckBox(nItem, BST_INDETERMINATE);
			}
		}
		else
		{
			// Note: This assumes that col 1 is sorted
			CString strRedox;
			for (int i = nItem + 1; ; ++i)
			{
				strRedox = m_ctrlElements.GetItemText(i, 1);
				if (strRedox.Find(str + _T("("), 0) != 0)
					break;
				m_ctrlElements.SetCheckBox(i, BST_INDETERMINATE);
			}
		}
		break;
	}
}

void CKPSolutionPg2::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == &m_ctrlElements) 
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;

			pWnd->GetClientRect(rect);
			pWnd->ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_CHECKLIST));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pPopup->CheckMenuRadioItem(ID_VIEW_AS_LIST, ID_VIEW_AS_REPORT,
			(m_ctrlElements.GetStyle() & LVS_TYPEMASK) == LVS_LIST ? ID_VIEW_AS_LIST : ID_VIEW_AS_REPORT,
			MF_BYCOMMAND );
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			this/*pWndPopupOwner*/);
	}
}

void CKPSolutionPg2::OnViewChange(UINT uID)
{
	int i, nElements;
	
	switch (uID)
	{
	case ID_VIEW_AS_LIST :
		m_ctrlElements.ModifyStyle(LVS_REPORT, LVS_LIST);
		break;

	case ID_VIEW_AS_REPORT :
		m_ctrlElements.ModifyStyle(LVS_LIST, LVS_REPORT);
		m_ctrlElements.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.RedrawWindow();
		break;

	case ID_LIST_CLEAR_ALL :
		nElements = m_ctrlElements.GetItemCount();
		for (i = 0; i < nElements; ++i)
		{
			if (m_ctrlElements.GetCheckBox(i) == BST_CHECKED)
			{
				m_ctrlElements.SetCheckBox(i, BST_UNCHECKED);
			}
		}
		break;

	default :
		ASSERT(FALSE);

	}
}

void CKPSolutionPg2::OnMajors() 
{
	CString strMajors[] = {_T("Alkalinity"), _T("Ca"), _T("Cl"), _T("K"), _T("Mg"), _T("Na"), _T("SO4-2")};

	// find in list
	LVFINDINFO info;
	info.flags = LVFI_STRING;

	for (int i = 0; i < sizeof(strMajors)/sizeof(strMajors[0]); ++i)
	{
		info.psz = strMajors[i];

		int nItem = m_ctrlElements.FindItem(&info);

		// set check mark
		if (nItem != -1)
		{
			m_ctrlElements.SetCheck(nItem, TRUE);
		}
	}
}

void CKPSolutionPg2::OnTrace() 
{
	CString strTrace[] = {_T("Al"), _T("F"), _T("Fe"), _T("Mn"), _T("NO3-"), _T("NH4+"), _T("P"), _T("Si")};

	// Find in list
	LVFINDINFO info;
	info.flags = LVFI_STRING;

	int cMajors = sizeof(strTrace)/sizeof(strTrace[0]);

	for (int i = 0; i < cMajors; ++i)
	{
		info.psz = strTrace[i];

		int nItem = m_ctrlElements.FindItem(&info);

		// Set check mark
		if (nItem != -1)
		{
			m_ctrlElements.SetCheck(nItem, TRUE);
		}
	}	
}

void CKPSolutionPg2::OnCustom() 
{
	CString strElements = AfxGetApp()->GetProfileString(_T("Settings"), _T("CustomList"));
	if (strElements.IsEmpty())
	{
		if (::AfxMessageBox("You have not selected any elements for the current database.  Do you want to define then now?", MB_YESNO) == IDYES)
		{
			CSelectElementsDlg dlg(((CPhreeqciApp*)AfxGetApp())->GetMergeDatabase(GetActiveDocument()->m_props.m_strDBPathName), this);
			if (dlg.DoModal() == IDOK)
			{
				std::list<CString>::iterator iter = dlg.m_listElements.begin();
				for (; iter != dlg.m_listElements.end(); ++iter)
				{
					strElements += *iter + _T(";");
				}
				AfxGetApp()->WriteProfileString(_T("Settings"), _T("CustomList"), strElements);
			}
		}
	}

	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = _tcstok(strElements.LockBuffer(), _T(";"));
	int nItem;
	while (info.psz)
	{
		nItem = m_ctrlElements.FindItem(&info);
		// set check mark
		if (nItem != -1)
		{
			if (m_ctrlElements.GetCheckBox(nItem) == BST_UNCHECKED)
			{
				m_ctrlElements.SetCheckBox(nItem, BST_CHECKED);
			}
		}
		info.psz = _tcstok(NULL, _T(";"));
	}
	strElements.UnlockBuffer();
}

void CKPSolutionPg2::OnSelchangeCboDefUnits() 
{
	int i;
	int nStart, nEnd;
	CString str;

	int nCurrent = m_ctrlDefaultUnits.GetCurSel();
	ASSERT(nCurrent != LB_ERR);

	nStart = m_ctrlGrid.GetFixedRows();
	nEnd = m_ctrlGrid.GetRows() - 1;

	// (0)mol/L; (1)mmol/L; (2)umol/L; (3)g/L; (4)mg/L; (5)ug/L;

	// (6)mol/kg solution; (7)mmol/kg solution; (8)umol/kg solution;
	// (9)g/kg solution; (10)mg/kg solution; (11)ug/kg solution;
	// (12)ppt; (13)ppm; (14)ppb;


	// (15)mol/kg water; (16)mmol/kg water; (17)umol/kg water;
	// (18)g/kg water; (19)mg/kg water; (20)ug/kg water

	if (0 <= nCurrent && nCurrent <= 5)
	{
		// per liter
		for (i = nStart; i < nEnd; ++i)
		{
			str = m_ctrlGrid.GetTextMatrix(i, 3);
			// replace parts per units
			str.Replace("ppt", "g/kg solution") ||
			str.Replace("ppm", "mg/kg solution") ||
			str.Replace("ppb", "ug/kg solution");

			if (str.Replace("/kg solution", "/L") ||
				str.Replace("/kg water", "/L"))
			{
				m_ctrlGrid.SetTextMatrix(i, 3, str);
			}
		}

	}
	else if (6 <= nCurrent && nCurrent <= 14)
	{
		// per kg solution
		for (i = nStart; i < nEnd; ++i)
		{
			str = m_ctrlGrid.GetTextMatrix(i, 3);
			if (str.Replace("/L", "/kg solution") ||
				str.Replace("/kg water", "/kg solution"))
			{
				str.Replace("mg/kg solution", "ppm");
				str.Replace("ug/kg solution", "ppb");
				str.Replace("g/kg solution", "ppt");
				m_ctrlGrid.SetTextMatrix(i, 3, str);
			}
		}
	}
	else if (15 <= nCurrent && nCurrent <= 20)
	{
		// per kg water
		for (i = nStart; i < nEnd; ++i)
		{
			str = m_ctrlGrid.GetTextMatrix(i, 3);

			// replace parts per units
			str.Replace("ppt", "g/kg solution") ||
			str.Replace("ppm", "mg/kg solution") ||
			str.Replace("ppb", "ug/kg solution");

			if (str.Replace("/kg solution", "/kg water") || 
				str.Replace("/L", "/kg water"))
			{
				m_ctrlGrid.SetTextMatrix(i, 3, str);
			}
		}
	}	
}

BEGIN_EVENTSINK_MAP(CKPSolutionPg2, baseKPSolutionPg2)
    //{{AFX_EVENTSINK_MAP(CKPSolutionPg2)
	ON_EVENT(CKPSolutionPg2, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSolutionPg2, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolutionPg2::OnEnterCellGrid() 
{
	CString strRes;
	CString strResEx;

	switch (m_ctrlGrid.GetCol())
	{
	case 1: // elem
		strRes.LoadString(IDS_ELEM_195);
		break;
	case 2: // conc
		strRes.LoadString(IDS_ELEM_196);
		break;
	case 3: // units
		strRes.LoadString(IDS_ELEM_197);
		break;
	case 4: // phase
		strRes.LoadString(IDS_ELEM_198);
		break;		
	case 5: // S.I.
		strRes.LoadString(IDS_ELEM_199);
		break;
	case 6: // As / GFW
		strRes.LoadString(IDS_ELEM_200);
		strResEx.LoadString(IDS_ELEM_201);
		strRes += strResEx;
		break;
	case 7: // Redox
		strRes.LoadString(IDS_ELEM_202);
		strResEx.LoadString(IDS_ELEM_203);
		strRes += strResEx;
		break;
	}
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolutionPg2::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg3 property page

CKPSolutionPg3::CKPSolutionPg3() : baseKPSolutionPg3(CKPSolutionPg3::IDD)
, m_glDoc(&m_ctrlGrid, &m_ctrlIsotopes, 1)
{
	//{{AFX_DATA_INIT(CKPSolutionPg3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSolutionPg3::~CKPSolutionPg3()
{
}

void CKPSolutionPg3::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolutionPg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionPg3)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_ISOTOPE_LIST, m_ctrlIsotopes);
	DDX_Control(pDX, IDC_ISOTOPE_GRID, m_ctrlGrid);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		// add isotopes to check list control
		CUtil::InsertIsotopes(&m_ctrlIsotopes);
	}
	DDX_IsotopeList(pDX, IDC_ISOTOPE_GRID);
}

void CKPSolutionPg3::DDX_IsotopeList(CDataExchange* pDX, int nIDC)
{
	if (m_bFirstSetActive)
	{
		InitGrid(pDX, nIDC);
	}

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CIsotope> listIsotopes;
		for (long nRow = m_ctrlGrid.GetFixedRows(); nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			CIsotope isotope;

			//
			// Name
			//
			DDX_GridText(pDX, nIDC, nRow, 1, isotope.m_strName);
			if (isotope.m_strName.IsEmpty())
				continue;

			if (!(::_istdigit((int)isotope.m_strName[0]))) // TODO DAVID 
			{
				DDX_GridFail(pDX, _T("Expected isotope name to begin with an isotopic number."));
			}

			TCHAR* next_char;
			isotope.m_dIsotopeNumber = strtod(isotope.m_strName, &next_char);

			if (*next_char == _T('\0') || !::_istupper(*next_char)) // TODO DAVID 
			{
				DDX_GridFail(pDX, _T("Expected element name to follow isotopic number."));
			}

			//
			// Value
			//
			DDX_GridText(pDX, nIDC, nRow, 2, isotope.m_dRatio);

			//
			// Uncertainty limit
			// 
			DDX_GridTextNaN(pDX, nIDC, nRow, 3, isotope.m_dRatioUncertainty);

			// OK
			listIsotopes.push_back(isotope);
		}
		// All OK
		m_listIsotopes.assign(listIsotopes.begin(), listIsotopes.end());
	}
	else
	{
		// reset each time
		m_glDoc.DeleteAllRows();

		std::list<CIsotope>::const_iterator iter = m_listIsotopes.begin();
		for (; iter != m_listIsotopes.end(); ++iter)
		{
			CIsotope isotope(*iter);

			//
			// Name
			//
			long nRow = m_glDoc.AddItem(isotope.m_strName).m_nRow;

			//
			// Value
			//
			DDX_GridText(pDX, nIDC, nRow, 2, isotope.m_dRatio);

			//
			// Uncertainty limit
			// 
			DDX_GridTextNaN(pDX, nIDC, nRow, 3, isotope.m_dRatioUncertainty);
		}
	}

	// reset initial pos
	m_ctrlGrid.SetRow(m_ctrlGrid.GetFixedRows());
	m_ctrlGrid.SetCol(m_ctrlGrid.GetFixedCols());

}


void CKPSolutionPg3::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set row heights
	m_ctrlGrid.SetWordWrap(TRUE);
	m_ctrlGrid.SetRowHeight(0, (long) 2.5 * m_ctrlGrid.GetRowHeight(0));

	// Add column labels
	m_ctrlGrid.SetTextMatrix(0, 1, _T("Name"));
	m_ctrlGrid.SetTextMatrix(0, 2, _T("Value"));
	m_ctrlGrid.SetTextMatrix(0, 3, _T("Uncertainty\r\nlimit"));

	// set bold titles
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetCellFontBold(TRUE);
	m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(2);
	m_ctrlGrid.SetCellFontBold(TRUE);

	// Center Align the column headers
	m_ctrlGrid.SetFillStyle(flexFillRepeat);
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(1);
	m_ctrlGrid.SetColSel(m_ctrlGrid.GetCols(0) - 1);
	m_ctrlGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlGrid.SetFillStyle(flexFillSingle);

	// set row titles
	m_ctrlGrid.AddRowHeaders();
}

BOOL CKPSolutionPg3::OnInitDialog() 
{
	baseKPSolutionPg3::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_S_ISOTOPES, NORESIZE)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_ISOTOPE_LIST, GREEDY)
			<< item(IDC_ISOTOPE_GRID, GREEDY)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, ABSOLUTE_VERT)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CKPSolutionPg3, baseKPSolutionPg3)
	//{{AFX_MSG_MAP(CKPSolutionPg3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ISOTOPE_LIST, OnItemchangedIsotopeList)
	ON_NOTIFY(NM_SETFOCUS, IDC_ISOTOPE_LIST, OnSetfocusIsotopeList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom grid messages
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

LRESULT CKPSolutionPg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_ISOTOPE_GRID);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iCol)
	{
	case 1:	// name
		pInfo->item.bUseCombo = (CUtil::InsertIsotopes(pInfo->item.hWndCombo) > 0);
		break;
	}

	return bDenyEdit;	// allow edit
}

LRESULT CKPSolutionPg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_ISOTOPE_GRID :
		// set defaults
		if (pInfo->item.iCol == 1 && m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
		{
			if (_tcslen(pInfo->item.pszText))
			{
				m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, 2, _T("0.0"));
			}
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}
	return bMakeChange;
}

LRESULT CKPSolutionPg3::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	ASSERT((int)wParam == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(wParam);  // unused in release builds
	UNUSED_ALWAYS(lParam);
	OnEnterCellIsotopeGrid();
	return 0;
}

void CKPSolutionPg3::OnItemchangedIsotopeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	if (row != -1)
	{
		if (m_ctrlGrid.GetTextMatrix(row, 2).IsEmpty())
			m_ctrlGrid.SetTextMatrix(row, 2, _T("0.0"));
	}
	return;
}

BEGIN_EVENTSINK_MAP(CKPSolutionPg3, baseKPSolutionPg3)
    //{{AFX_EVENTSINK_MAP(CKPSolutionPg3)
	ON_EVENT(CKPSolutionPg3, IDC_ISOTOPE_GRID, 71 /* EnterCell */, OnEnterCellIsotopeGrid, VTS_NONE)
	ON_EVENT(CKPSolutionPg3, IDC_ISOTOPE_GRID, -602 /* KeyDown */, OnKeyDownIsotopeGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolutionPg3::OnEnterCellIsotopeGrid() 
{
	CString strRes;

	switch (m_ctrlGrid.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_ISOTOPE_192);
		break;
	case 2:
		strRes.LoadString(IDS_ISOTOPE_193);
		break;
	case 3:
		strRes.LoadString(IDS_ISOTOPE_194);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPSolutionPg3::OnKeyDownIsotopeGrid(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

void CKPSolutionPg3::OnSetfocusIsotopeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlIsotopes.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlIsotopes.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}

BOOL CKPSolutionPg2::OnKillActive() 
{
	if (baseKPSolutionPg2::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CKPSolutionPg2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

#ifdef _DEBUG
	// reset dialog on Ctrl-Z
	if (nChar == _T('Z') && GetKeyState(VK_CONTROL) < 0)
	{
		UpdateData(FALSE);
		MessageBeep(0);
	}
#endif // _DEBUG
	
	baseKPSolutionPg2::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CKPSolutionPg2::PreTranslateMessage(MSG* pMsg) 
{
#ifdef _DEBUG
	if (pMsg->message == WM_CHAR)
	{
		if (pMsg->wParam == 0x1A)
		{
			UpdateData(FALSE);
		}
	}
#endif // _DEBUG
	
	return baseKPSolutionPg2::PreTranslateMessage(pMsg);
}

BOOL CKPSolutionPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_RESET:
		strRes.LoadString(IDS_STRING465);
		break;
	case IDC_MSHFLEXGRID1:
		{
			if (!IsContextHelp())
			{
				if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_ctrlGrid.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_ctrlGrid.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			long nCol = IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol();
			switch (IsContextHelp() ? m_ctrlGrid.GetMouseRow() : m_ctrlGrid.GetRow())
			{
			case 0:
				strRes.LoadString(IDS_STRING452);
				break;
			case 1:
				strRes.LoadString(IDS_STRING453);
				break;
			case 2:
				strRes.LoadString(IDS_STRING454);
				break;
			case 3:
				strRes.LoadString((nCol == 0) ? IDS_STRING466 : IDS_STRING455);
				break;		
			case 4:
				strRes.LoadString(IDS_STRING456);
				break;
			case 5:
				strRes.LoadString(IDS_STRING457);
				break;
			case 6:	// pe
				strRes.LoadString((nCol == 0) ? IDS_STRING467 : IDS_STRING458);
				break;
			case 7:	// pe phase equil
				strRes.LoadString(IDS_STRING459);
				break;
			case 8:	// pe S.I.
				strRes.LoadString(IDS_STRING460);
				break;
			case 9:
				strRes.LoadString(IDS_STRING461);
				break;
			case 10:
				strRes.LoadString(IDS_STRING462);
				break;
			case 11:	// Density
				strRes.LoadString(IDS_STRING463);
				break;
			case 12:
				strRes.LoadString(IDS_STRING464);
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPSolutionPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	CString strResEx;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_ELEMENT_LIST:
		AfxFormatString1(strRes, IDS_CL_167, _T("element(s)") );
		strRes += _T(" Right-click to display details");
		break;
	case IDC_TRACE:
		strRes.LoadString(IDS_STRING469);
		break;
	case IDC_MAJORS:
		strRes.LoadString(IDS_STRING468);
		break;
	case IDC_CUSTOM:
		strRes.LoadString(IDS_STRING567);
		break;
	case IDC_BTN_MOD_CUST:
		strRes.LoadString(IDS_STRING569);
		break;
	case IDC_CBO_DEF_UNITS: case IDC_S_DEF_UNITS:
		strRes.LoadString(IDS_SOL_204);
		break;
	case IDC_CBO_CHARGE: case IDC_S_CHARGE:
		strRes.LoadString(IDS_SOL_205);
		break;
	case IDC_MSHFLEXGRID1:
		{
			if (!IsContextHelp())
			{
				if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_ctrlGrid.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_ctrlGrid.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
			{
			case 1: // elem
				strRes.LoadString(IDS_ELEM_195);
				break;
			case 2: // conc
				strRes.LoadString(IDS_ELEM_196);
				break;
			case 3: // units
				strRes.LoadString(IDS_ELEM_197);
				break;
			case 4: // phase
				strRes.LoadString(IDS_ELEM_198);
				break;		
			case 5: // S.I.
				strRes.LoadString(IDS_ELEM_199);
				break;
			case 6: // As / GFW
				strRes.LoadString(IDS_ELEM_200);
				strResEx.LoadString(IDS_ELEM_201);
				strRes += strResEx;
				break;
			case 7: // Redox
				strRes.LoadString(IDS_ELEM_202);
				strResEx.LoadString(IDS_ELEM_203);
				strRes += strResEx;
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
			}
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPSolutionPg3::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_ISOTOPE_LIST:
		AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
		break;
	case IDC_ISOTOPE_GRID:
		if (!IsContextHelp())
		{
			if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_ISOTOPE_192);
			break;
		case 2:
			strRes.LoadString(IDS_ISOTOPE_193);
			break;
		case 3:
			strRes.LoadString(IDS_ISOTOPE_194);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSolutionPg2::OnBtnModCust() 
{
	CString strElements = AfxGetApp()->GetProfileString(_T("Settings"), _T("CustomList"));

	CSelectElementsDlg dlg(((CPhreeqciApp*)AfxGetApp())->GetMergeDatabase(GetActiveDocument()->m_props.m_strDBPathName), this);

	LPCTSTR psz = _tcstok(strElements.LockBuffer(), _T(";"));
	while (psz)
	{
		dlg.m_listElements.push_back(psz);
		psz = _tcstok(NULL, _T(";"));
	}
	strElements.UnlockBuffer();

	if (dlg.DoModal() == IDOK)
	{
		strElements.Empty();
		std::list<CString>::iterator iter = dlg.m_listElements.begin();
		for (; iter != dlg.m_listElements.end(); ++iter)
		{
			strElements += *iter + _T(";");
		}
		AfxGetApp()->WriteProfileString(_T("Settings"), _T("CustomList"), strElements);
	}
}
