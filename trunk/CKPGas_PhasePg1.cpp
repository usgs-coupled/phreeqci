// CKPGas_PhasePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPGas_PhasePg1.h"

#include "Util.h"
#include "DDX_DDV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPGas_PhasePg1, baseCKPGas_PhasePg1)


/////////////////////////////////////////////////////////////////////////////
// CCKPGas_PhasePg1 property page

CCKPGas_PhasePg1::CCKPGas_PhasePg1(CTreeCtrlNode simNode) : baseCKPGas_PhasePg1(CCKPGas_PhasePg1::IDD)
, m_glDoc(&m_ctrlGasEditGrid, &m_ctrlGasCheckList, 1), m_ranges(simNode, true)
{
	ASSERT(simNode);
	//{{AFX_DATA_INIT(CCKPGas_PhasePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType        = pressureType;
	m_dTempC       = 25.0;
	m_dVolumeL     = 1.0;
	m_dPressureAtm = 1.0;
	m_nSolution    = NONE;
	m_strNumFormat = _T("%d");
}

CCKPGas_PhasePg1::~CCKPGas_PhasePg1()
{
}

void CCKPGas_PhasePg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPGas_PhasePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPGas_PhasePg1)
	DDX_Control(pDX, IDC_CBO_EQUIL, m_cboSolutions);
	DDX_Control(pDX, IDC_BTN_EQUIL, m_btnEquilbrate);
	DDX_Control(pDX, IDC_CL_GASPHASE, m_ctrlGasCheckList);
	DDX_Control(pDX, IDC_MSHFG_GASPHASE, m_ctrlGasEditGrid);
	//}}AFX_DATA_MAP
	DDX_GridTextDefault(pDX, IDC_MSHFG_NUM_DESC, 3, 1, m_dPressureAtm, 1.0);
	DDX_GridTextDefault(pDX, IDC_MSHFG_NUM_DESC, 4, 1, m_dVolumeL, 1.0);
	DDX_GridTextDefault(pDX, IDC_MSHFG_NUM_DESC, 5, 1, m_dTempC, 25.0);
	DDX_Type(pDX);
	DDX_GasCompList(pDX, IDC_MSHFG_GASPHASE);
}


BEGIN_MESSAGE_MAP(CCKPGas_PhasePg1, baseCKPGas_PhasePg1)
	//{{AFX_MSG_MAP(CCKPGas_PhasePg1)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_GASPHASE, OnSetfocusClGasPhase)
	ON_BN_CLICKED(IDC_RADIO_FIX_PRESS, OnRadioFixPress)
	ON_BN_CLICKED(IDC_RADIO_FIX_VOL, OnRadioFixVol)
	ON_BN_CLICKED(IDC_BTN_EQUIL, OnBtnEquil)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_GASPHASE, OnItemchangedClGasphase)
	ON_WM_HELPINFO()
	ON_CBN_SETFOCUS(IDC_CBO_EQUIL, OnSetfocusCboEquil)
	//}}AFX_MSG_MAP
	// custom set focus notifications
	ON_BN_SETFOCUS(IDC_RADIO_FIX_PRESS, OnSetfocusRadioFixPress)
	ON_BN_SETFOCUS(IDC_RADIO_FIX_VOL, OnSetfocusRadioFixVol)
	ON_BN_SETFOCUS(IDC_BTN_EQUIL, OnSetfocusBtnEquil)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCKPGas_PhasePg1, baseCKPGas_PhasePg1)
    //{{AFX_EVENTSINK_MAP(CCKPGas_PhasePg1)
	ON_EVENT(CCKPGas_PhasePg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPGas_PhasePg1, IDC_MSHFG_GASPHASE, 71 /* EnterCell */, OnEnterCellMshfgGasphase, VTS_NONE)
	ON_EVENT(CCKPGas_PhasePg1, IDC_MSHFG_NUM_DESC, -602 /* KeyDown */, OnKeyDownMshfgNumDesc, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPGas_PhasePg1, IDC_MSHFG_GASPHASE, -602 /* KeyDown */, OnKeyDownMshfgGasphase, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CCKPGas_PhasePg1::OnInitDialog() 
{
	baseCKPGas_PhasePg1::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< (paneCtrl(IDC_GB_TYPE, HORIZONTAL, GREEDY, nDefaultBorder, 4, 15, 0)
			<< (pane(VERTICAL, NORESIZE)
				<< (pane(HORIZONTAL, NORESIZE)
					<< itemFixed(HORIZONTAL, 15)
					<< item(IDC_RADIO_FIX_PRESS, NORESIZE)
					)
				<< (pane(HORIZONTAL, NORESIZE)
					<< itemFixed(HORIZONTAL, 15)
					<< item(IDC_RADIO_FIX_VOL, NORESIZE)
					<< item(IDC_BTN_EQUIL, NORESIZE)
					<< item(IDC_CBO_EQUIL, NORESIZE)
					)
				<< itemFixed(VERTICAL, 1)
				)
			)
		<< itemFixed(VERTICAL, 1)

		<< item(IDC_S_DEF_GAS, NORESIZE | ALIGN_BOTTOM)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_GASPHASE, GREEDY)
			//<< itemSpaceBetween(HORIZONTAL, IDC_CL_GASPHASE, IDC_MSHFG_GASPHASE)
			<< itemFixed(HORIZONTAL, 1)
			<< item(IDC_MSHFG_GASPHASE, ABSOLUTE_HORZ)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();

	// set radio button
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCKPGas_PhasePg1::DDX_Type(CDataExchange *pDX)
{
	// initialize
	if (m_bFirstSetActive)
	{
		// fill defined solutions combo
		std::set<CDBRange> setSolutions;
		m_strNumFormat = CUtil::CreateRange(setSolutions, m_ranges[CKeyword::K_SOLUTION]);
		CUtil::InsertRange(m_cboSolutions.m_hWnd, setSolutions);
	}

	CButton* pFixPressBtn = (CButton*)GetDlgItem(IDC_RADIO_FIX_PRESS);
	CButton* pFixVolBtn = (CButton*)GetDlgItem(IDC_RADIO_FIX_VOL);

	if (pDX->m_bSaveAndValidate)
	{
		switch (pFixPressBtn->GetCheck())
		{
		case BST_CHECKED :
			ASSERT(pFixVolBtn->GetCheck() == BST_UNCHECKED);
			m_nType = pressureType;
			break;

		case BST_UNCHECKED :
			ASSERT(pFixVolBtn->GetCheck() == BST_CHECKED);
			m_nType = volumeType;

			if (m_btnEquilbrate.GetCheck() == BST_CHECKED)
			{
				ASSERT(m_cboSolutions.IsWindowEnabled());
				DDX_Text(pDX, IDC_CBO_EQUIL, m_nSolution);
				DDV_MinMaxInt(pDX, m_nSolution, 0, INT_MAX);
			}
			else
			{
				ASSERT(!m_cboSolutions.IsWindowEnabled());
				m_nSolution = NONE;
			}
			break;

		default :
			ASSERT(FALSE);
			break;
		}
	}
	else
	{
		if (m_nType == pressureType)
		{
			pFixPressBtn->SetCheck(BST_CHECKED);
		}
		else if (m_nType == volumeType)
		{
			pFixVolBtn->SetCheck(BST_CHECKED);

			if (m_nSolution == NONE)
			{
				m_btnEquilbrate.SetCheck(BST_UNCHECKED);
			}
			else
			{
				CString strSolNum;
				strSolNum.Format(m_strNumFormat, m_nSolution);
				int nFind = m_cboSolutions.FindStringExact(-1, strSolNum);
				if (nFind == CB_ERR)
				{
					m_cboSolutions.SetWindowText(strSolNum);
				}
				else
				{
					m_cboSolutions.SetCurSel(nFind);
				}
				m_btnEquilbrate.SetCheck(BST_CHECKED);
				m_cboSolutions.EnableWindow(TRUE);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
		UpdateRadioState();
	}
}

void CCKPGas_PhasePg1::DDX_GasCompList(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_GASPHASE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitNumDesc();
		InitGasCheckList();
		InitGasEditGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CGasComp> listGasComp;
		for (long nRow = m_ctrlGasEditGrid.GetFixedRows(); nRow < m_ctrlGasEditGrid.GetRows(); ++nRow)
		{
			CGasComp gasComp;

			// gas phase name
			DDX_GridText(pDX, nIDC, nRow, 1, gasComp.m_strName);
			if (gasComp.m_strName.IsEmpty())
				continue;

			// partial pressure
			DDX_GridTextNaN(pDX, nIDC, nRow, 2, gasComp.m_dP_Read);

			// if here no exception has occured so add to list
			listGasComp.push_back(gasComp);
		}
		// if here listGasComp is valid and can be assigned to the member variable
		m_listGasComp.assign(listGasComp.begin(), listGasComp.end());
	}
	else
	{
		std::list<CGasComp>::const_iterator iter = m_listGasComp.begin();
		for (; iter != m_listGasComp.end(); ++iter)
		{
			// phase name [saturation index [(alternative formula or alternative phase) [amount]]]
			CGasComp gasComp(*iter);

			// gas phase name
			// add to grid list doc
			int nRow = m_glDoc.AddItem(gasComp.m_strName).m_nRow;

			// partial pressure
			DDX_GridTextNaN(pDX, nIDC, nRow, 2, gasComp.m_dP_Read);
		}
	}
}

BOOL CCKPGas_PhasePg1::InitNumDesc()
{
	// must be valid window
	ASSERT(::IsWindow(m_egNumDesc.m_hWnd));

	// should only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set title column widths
	m_egNumDesc.SetColWidth(0, 0, 2100);

	// additional row headings
    m_egNumDesc.SetTextMatrix(3, 0, _T("Pressure (atm)"));
    m_egNumDesc.SetTextMatrix(4, 0, _T("Initial volume (liters)"));
    m_egNumDesc.SetTextMatrix(5, 0, _T("Initial temperature (°C)"));

	// set additional bold headings
	m_egNumDesc.SetRow(3), m_egNumDesc.SetCol(0);
	m_egNumDesc.SetCellFontBold(TRUE);
	m_egNumDesc.SetRow(4), m_egNumDesc.SetCol(0);
	m_egNumDesc.SetCellFontBold(TRUE);
	m_egNumDesc.SetRow(5), m_egNumDesc.SetCol(0);
	m_egNumDesc.SetCellFontBold(TRUE);

	// set starting position
	m_egNumDesc.SetRow(0);
	m_egNumDesc.SetCol(1);

	return TRUE;
}

BOOL CCKPGas_PhasePg1::InitGasCheckList()
{
	// must be valid window
	ASSERT(::IsWindow(m_ctrlGasEditGrid.m_hWnd));
	// should only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	CUtil::InsertGases(&m_ctrlGasCheckList, GetDatabase());

	return TRUE;
}

BOOL CCKPGas_PhasePg1::InitGasEditGrid()
{
	// must be valid window
	ASSERT(::IsWindow(m_ctrlGasEditGrid.m_hWnd));

	// should only be called once
	ASSERT( m_bFirstSetActive == TRUE );

	// set header row height
	m_ctrlGasEditGrid.SetWordWrap(TRUE);
	m_ctrlGasEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlGasEditGrid.GetRowHeight(1));

	// set column titles
	m_ctrlGasEditGrid.SetTextMatrix(0, 1, _T("Gas"));
	m_ctrlGasEditGrid.SetTextMatrix(0, 2, _T("Partial pressure\r\n(atm)"));

	// set bold titles
	m_ctrlGasEditGrid.SetRow(0), m_ctrlGasEditGrid.SetCol(1);
	m_ctrlGasEditGrid.SetCellFontBold(TRUE);
	m_ctrlGasEditGrid.SetRow(0), m_ctrlGasEditGrid.SetCol(2);
	m_ctrlGasEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlGasEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlGasEditGrid.SetRow(0);
	m_ctrlGasEditGrid.SetCol(1);
	m_ctrlGasEditGrid.SetColSel(m_ctrlGasEditGrid.GetCols(0) - 1);
	m_ctrlGasEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlGasEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlGasEditGrid.SetColWidth(0, 0, 500);
	m_ctrlGasEditGrid.SetColWidth(1, 0, 1000);
	m_ctrlGasEditGrid.SetColWidth(2, 0, 1450);

	// set row titles
	m_ctrlGasEditGrid.AddRowHeaders();

	// center align row titles
	m_ctrlGasEditGrid.SetColAlignmentFixed(0, flexAlignCenterCenter);

	// scroll to top of grid
	m_ctrlGasEditGrid.SetTopRow(1); m_ctrlGasEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlGasEditGrid.SetRow(1); m_ctrlGasEditGrid.SetCol(1);

	return TRUE;
}

void CCKPGas_PhasePg1::OnSetfocusClGasPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlGasCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString str;
	
	str.LoadString(IDS_GAS_155);

	m_eInputDesc.SetWindowText(str);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlGasCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlGasCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}


void CCKPGas_PhasePg1::OnRadioFixPress() 
{
	OnSetfocusRadioFixPress();
	UpdateRadioState();
}

void CCKPGas_PhasePg1::OnRadioFixVol() 
{
	OnSetfocusRadioFixVol();
	UpdateRadioState();
}

void CCKPGas_PhasePg1::UpdateRadioState()
{
	CButton* pFixPressBtn = (CButton*)GetDlgItem(IDC_RADIO_FIX_PRESS);
	CButton* pFixVolBtn = (CButton*)GetDlgItem(IDC_RADIO_FIX_VOL);

	switch (pFixPressBtn->GetCheck())
	{
	case BST_CHECKED :
		ASSERT(pFixVolBtn->GetCheck() == BST_UNCHECKED);
		m_btnEquilbrate.EnableWindow(FALSE);
		m_cboSolutions.EnableWindow(FALSE);
		break;
	case BST_UNCHECKED :
		ASSERT(pFixVolBtn->GetCheck() == BST_CHECKED);
		m_btnEquilbrate.EnableWindow(TRUE);
		if (m_btnEquilbrate.GetCheck() == BST_CHECKED)
		{
			m_cboSolutions.EnableWindow(TRUE);
		}
		break;
	default :
		ASSERT(FALSE);
		break;
	}
}

void CCKPGas_PhasePg1::OnBtnEquil() 
{
	int nState = m_btnEquilbrate.GetCheck();
	
	switch (nState)
	{
	case BST_CHECKED :
		m_cboSolutions.EnableWindow(TRUE);
		break;
	case BST_UNCHECKED :
		m_cboSolutions.EnableWindow(FALSE);
		break;
	case BST_INDETERMINATE :
		ASSERT( FALSE );
		break;
	}	
}

void CCKPGas_PhasePg1::OnEnterCellMshfgNumDesc() 
{
	CString str;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(str, IDS_N_USER_140, _T("gas phase") ); 
		break;
	case 1 :
		AfxFormatString1(str, IDS_N_USER_END_141, _T("gas phase") ); 
		break;
	case 2 :
		AfxFormatString1(str, IDS_DESC_STR_142, _T("gas phase") ); 
		break;
	case 3 :
		str.LoadString(IDS_GAS_149);
		break;
	case 4 :
		str.LoadString(IDS_GAS_150);
		break;
	case 5 :
		str.LoadString(IDS_GAS_151);
		break;
	}

	m_eInputDesc.SetWindowText(str);
}

void CCKPGas_PhasePg1::OnEnterCellMshfgGasphase() 
{
	CString str;
	switch (m_ctrlGasEditGrid.GetCol())
	{
	case 1 :
		str.LoadString(IDS_GAS_153);
		break;
	case 2 :
		str.LoadString(IDS_GAS_154);
		break;
	}

	m_eInputDesc.SetWindowText(str);
}

LRESULT CCKPGas_PhasePg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_GASPHASE :
		OnEnterCellMshfgGasphase();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CCKPGas_PhasePg1::OnSetfocusRadioFixPress()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString str;
	str.LoadString(IDS_GAS_156);
	m_eInputDesc.SetWindowText(str);
}

void CCKPGas_PhasePg1::OnSetfocusRadioFixVol()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString str;
	str.LoadString(IDS_GAS_157);
	m_eInputDesc.SetWindowText(str);
}

void CCKPGas_PhasePg1::OnSetfocusBtnEquil()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString str;
	str.LoadString(IDS_GAS_152);
	m_eInputDesc.SetWindowText(str);
}

void CCKPGas_PhasePg1::OnItemchangedClGasphase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	ASSERT(pNMListView->hdr.hwndFrom == m_ctrlGasCheckList.m_hWnd);
	UNUSED(pNMListView);   // unused in release builds
	// Add your control notification handler code here
	long nRow = m_glDoc.OnItemChanged(pNMHDR, pResult);
	UNUSED(nRow);
}

LRESULT CCKPGas_PhasePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_GASPHASE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertGases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCKPGas_PhasePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
	case IDC_MSHFG_GASPHASE :
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}

	return bMakeChange;
}

void CCKPGas_PhasePg1::OnKeyDownMshfgGasphase(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

BOOL CCKPGas_PhasePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CBO_EQUIL:
		strRes.LoadString(IDS_STRING478);
		break;
	case IDC_RADIO_FIX_VOL:
		strRes.LoadString(IDS_GAS_157);
		break;
	case IDC_RADIO_FIX_PRESS:
		strRes.LoadString(IDS_GAS_156);
		break;
	case IDC_CL_GASPHASE: case IDC_S_DEF_GAS:
		strRes.LoadString(IDS_GAS_155);
		break;
	case IDC_BTN_EQUIL:
		strRes.LoadString(IDS_GAS_152);
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
			AfxFormatString1(strRes, IDS_N_USER_140, _T("gas phase") ); 
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("gas phase") ); 
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("gas phase") ); 
			break;
		case 3 :
			strRes.LoadString(IDS_GAS_149);
			break;
		case 4 :
			strRes.LoadString(IDS_GAS_150);
			break;
		case 5 :
			strRes.LoadString(IDS_GAS_151);
			break;
		default:
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;

	case IDC_MSHFG_GASPHASE:
		if (!IsContextHelp())
		{
			if (!(m_ctrlGasEditGrid.GetRowIsVisible(m_ctrlGasEditGrid.GetRow()) && m_ctrlGasEditGrid.GetColIsVisible(m_ctrlGasEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlGasEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlGasEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlGasEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlGasEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlGasEditGrid.GetMouseCol() : m_ctrlGasEditGrid.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_GAS_153);
			break;
		case 2 :
			strRes.LoadString(IDS_GAS_154);
			break;
		default:
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
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CCKPGas_PhasePg1::OnSetfocusCboEquil() 
{
	// TODO: Add your control notification handler code here
	// IDS_STRING478
	CString strRes;
	strRes.LoadString(IDS_STRING478);
	m_eInputDesc.SetWindowText(strRes);	
}
