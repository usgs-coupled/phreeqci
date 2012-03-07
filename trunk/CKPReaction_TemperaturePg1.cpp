// CKPReaction_TemperaturePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPReaction_TemperaturePg1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPReaction_TemperaturePg1, baseCKPReaction_TemperaturePg1)


/////////////////////////////////////////////////////////////////////////////
// CCKPReaction_TemperaturePg1 property page

CCKPReaction_TemperaturePg1::CCKPReaction_TemperaturePg1() : baseCKPReaction_TemperaturePg1(CCKPReaction_TemperaturePg1::IDD)
{
	m_nType = TYPE_LIST;
	m_nLinearSteps = 1;
	//{{AFX_DATA_INIT(CCKPReaction_TemperaturePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPReaction_TemperaturePg1::~CCKPReaction_TemperaturePg1()
{
}

void CCKPReaction_TemperaturePg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPReaction_TemperaturePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPReaction_TemperaturePg1)
	DDX_Control(pDX, IDC_SPIN_STEPS, m_spinSteps);
	DDX_Control(pDX, IDC_E_STEPS, m_eSteps);
	DDX_Control(pDX, IDC_E_TEMP2, m_eTemp2);
	DDX_Control(pDX, IDC_E_TEMP1, m_eTemp1);
	DDX_Control(pDX, IDC_MSHFG_LIST, m_ctrlStepEditGrid);
	//}}AFX_DATA_MAP
	DDX_Steps(pDX, IDC_MSHFG_LIST);
}

void CCKPReaction_TemperaturePg1::DDX_Steps(CDataExchange* pDX, int nIDC)
{
	if (m_bFirstSetActive)
	{
		InitSpin(pDX);
		InitStepEditGrid(pDX);
	}

	if (pDX->m_bSaveAndValidate)
	{
		if (m_nType == TYPE_LINEAR)
		{
			// validate Temp1
			DDX_Text(pDX, IDC_E_TEMP1, m_dTemp1);

			// validate Temp2
			DDX_Text(pDX, IDC_E_TEMP2, m_dTemp2);

			// validate steps
			DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);
			if (m_nLinearSteps < 0)
			{
				DDX_GridFail(pDX, _T("Steps must be a positive integer."));
			}
			ASSERT(0 < m_nLinearSteps && m_nLinearSteps <= INT_MAX);
		}
		else
		{
			ASSERT(m_nType == TYPE_LIST);

			// validate steps
			ASSERT(nIDC == m_ctrlStepEditGrid.GetDlgCtrlID());
			CString strValue;
			double dValue;

			std::list<double> listSteps;
			for (long nRow = m_ctrlStepEditGrid.GetFixedRows(); nRow < m_ctrlStepEditGrid.GetRows(); ++nRow)
			{
				DDX_GridText(pDX, nIDC, nRow, 1, strValue);
				if (strValue.IsEmpty())
					continue;
				DDX_GridText(pDX, nIDC, nRow, 1, dValue);

				listSteps.push_back(dValue);
			}
			// if here list is OK
			m_listSteps.assign(listSteps.begin(), listSteps.end());
		}
	}
	else
	{
		// exchange steps no mater what type
		ASSERT(0 < m_nLinearSteps && m_nLinearSteps <= INT_MAX);
		DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);

		if (m_nType == TYPE_LINEAR)
		{
			// set radio state
			CheckRadioButton(IDC_RADIO_LINEAR, IDC_RADIO_LIST, IDC_RADIO_LINEAR);
			UpdateRadioState();

			// exchange Temp1
			DDX_Text(pDX, IDC_E_TEMP1, m_dTemp1);

			// exchange Temp2
			DDX_Text(pDX, IDC_E_TEMP2, m_dTemp2);
		}
		else
		{
			ASSERT(m_nType == TYPE_LIST);
			
			// set radio state
			CheckRadioButton(IDC_RADIO_LINEAR, IDC_RADIO_LIST, IDC_RADIO_LIST);
			UpdateRadioState();

			// exchange list of steps
			ASSERT(nIDC == m_ctrlStepEditGrid.GetDlgCtrlID());
			std::list<double>::const_iterator const_iter = m_listSteps.begin();
			for (long nRow = 1; const_iter != m_listSteps.end(); ++const_iter, ++nRow)
			{
				double value(*const_iter);
				DDX_GridText(pDX, nIDC, nRow, 1, value);
			}
		}
	}
}

void CCKPReaction_TemperaturePg1::UpdateRadioState()
{
	CButton* pLinearBtn = (CButton*)GetDlgItem(IDC_RADIO_LINEAR);
	CButton* pListBtn = (CButton*)GetDlgItem(IDC_RADIO_LIST);

	switch (pLinearBtn->GetCheck())
	{
	case BST_CHECKED :
		ASSERT(pListBtn->GetCheck() == BST_UNCHECKED);

		m_nType = TYPE_LINEAR;
		EnableLinear(TRUE);
		EnableList(FALSE);	

		break;
	case BST_UNCHECKED :
		ASSERT(pListBtn->GetCheck() == BST_CHECKED);

		m_nType = TYPE_LIST;
		EnableLinear(FALSE);
		EnableList(TRUE);

		break;
	default :
		ASSERT(FALSE);
		break;
	}
}

void CCKPReaction_TemperaturePg1::EnableLinear(BOOL bEnable)
{
	CWnd* pWnd;

	// IDC_GB_LINEAR
	pWnd = GetDlgItem(IDC_GB_LINEAR);
	pWnd->EnableWindow(bEnable);

	// IDC_S_TEMP1
	pWnd = GetDlgItem(IDC_S_TEMP1);
	pWnd->EnableWindow(bEnable);

	// IDC_E_TEMP1
	m_eTemp1.EnableWindow(bEnable);

	// IDC_S_TEMP2
	pWnd = GetDlgItem(IDC_S_TEMP2);
	pWnd->EnableWindow(bEnable);

	// IDC_E_TEMP2
	m_eTemp2.EnableWindow(bEnable);

	// IDC_S_STEPS
	pWnd = GetDlgItem(IDC_S_STEPS);
	pWnd->EnableWindow(bEnable);

	// IDC_E_STEPS
	m_eSteps.EnableWindow(bEnable);

	// IDC_SPIN_STEPS
	m_spinSteps.EnableWindow(bEnable);
}

void CCKPReaction_TemperaturePg1::EnableList(BOOL bEnable)
{
	CWnd* pWnd;

	// IDC_GB_STEPS
	pWnd = GetDlgItem(IDC_GB_STEPS);
	pWnd->EnableWindow(bEnable);

	// IDC_MSHFG_LIST
	m_ctrlStepEditGrid.EnableWindow(bEnable);
}

void CCKPReaction_TemperaturePg1::InitSpin(CDataExchange* pDX)
{
	UNUSED(pDX);
	m_spinSteps.SetRange32(1, INT_MAX);

	UDACCEL uda[6];
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
	m_spinSteps.SetAccel(6, uda);
}

void CCKPReaction_TemperaturePg1::InitStepEditGrid(CDataExchange* pDX)
{
	UNUSED(pDX);

	// set column titles
	m_ctrlStepEditGrid.SetTextMatrix(0, 1, _T("Temperature (°C)"));

	// center align the column headers
	m_ctrlStepEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlStepEditGrid.SetRow(0);
	m_ctrlStepEditGrid.SetCol(1);
	m_ctrlStepEditGrid.SetColSel(m_ctrlStepEditGrid.GetCols(0) - 1);
	m_ctrlStepEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlStepEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlStepEditGrid.SetColWidth(1, 0, 1450);

	// set row titles
	m_ctrlStepEditGrid.SetRows(max(m_ctrlStepEditGrid.GetRows(), (long)m_listSteps.size() + 2));
	m_ctrlStepEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlStepEditGrid.SetTopRow(1); m_ctrlStepEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlStepEditGrid.SetRow(1); m_ctrlStepEditGrid.SetCol(1);
}

BEGIN_MESSAGE_MAP(CCKPReaction_TemperaturePg1, baseCKPReaction_TemperaturePg1)
	//{{AFX_MSG_MAP(CCKPReaction_TemperaturePg1)
	ON_BN_CLICKED(IDC_RADIO_LINEAR, OnRadioLinear)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_EN_SETFOCUS(IDC_E_TEMP1, OnSetfocusETemp1)
	ON_EN_SETFOCUS(IDC_E_TEMP2, OnSetfocusETemp2)
	ON_EN_SETFOCUS(IDC_E_STEPS, OnSetfocusESteps)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom radio button set focus notifications
	ON_BN_SETFOCUS(IDC_RADIO_LINEAR, OnSetfocusRadioLinear)
	ON_BN_SETFOCUS(IDC_RADIO_LIST, OnSetfocusRadioList)

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BOOL CCKPReaction_TemperaturePg1::OnInitDialog() 
{
	baseCKPReaction_TemperaturePg1::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< (pane(HORIZONTAL, GREEDY)
			<< (pane(VERTICAL, NORESIZE)
				<< itemFixed(VERTICAL, 20)
				<< item(IDC_RADIO_LINEAR, NORESIZE)
				)
			// linear steps
			<< (paneCtrl(IDC_GB_LINEAR, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_S_TEMP1, NORESIZE)
					<< item(IDC_E_TEMP1, NORESIZE)
					)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_S_TEMP2, NORESIZE)
					<< item(IDC_E_TEMP2, NORESIZE)
					)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_S_STEPS, NORESIZE)
					<< (pane(HORIZONTAL, NORESIZE, 0, 0, 0)
						<< item(IDC_E_STEPS, NORESIZE)
						<< item(IDC_SPIN_STEPS, NORESIZE)
						)
					)
				<< itemGrowing(HORIZONTAL)
				)
			)

		<< (pane(HORIZONTAL, GREEDY)
			<< (pane(VERTICAL, NORESIZE)
				<< itemFixed(VERTICAL, 20)
				<< item(IDC_RADIO_LIST, NORESIZE)
				)
			// explicit steps
			<< (paneCtrl(IDC_GB_STEPS, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
				<< item(IDC_MSHFG_LIST, ABSOLUTE_HORZ)
				<< itemGrowing(HORIZONTAL)
				)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCKPReaction_TemperaturePg1::OnSetfocusRadioLinear()
{
	CString strRes;
	strRes.LoadString(IDS_RXN_TEMP_183);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReaction_TemperaturePg1::OnSetfocusRadioList()
{
	CString strRes;
	strRes.LoadString(IDS_RXN_TEMP_184);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReaction_TemperaturePg1::OnRadioLinear() 
{
	OnSetfocusRadioLinear();
	UpdateRadioState();
}

void CCKPReaction_TemperaturePg1::OnRadioList() 
{
	OnSetfocusRadioList();
	UpdateRadioState();
}

void CCKPReaction_TemperaturePg1::OnSetfocusETemp1() 
{
	CString strRes;
	strRes.LoadString(IDS_RXN_TEMP_186);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReaction_TemperaturePg1::OnSetfocusETemp2() 
{
	CString strRes;
	strRes.LoadString(IDS_RXN_TEMP_187);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReaction_TemperaturePg1::OnSetfocusESteps() 
{
	CString strRes;
	strRes.LoadString(IDS_RXN_TEMP_188);
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CCKPReaction_TemperaturePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);
	UNUSED(nID);

	BOOL bDenyEdit = FALSE;	// allow edit

	return bDenyEdit;
}

LRESULT CCKPReaction_TemperaturePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_LIST :
		if (pInfo->item.iCol == 1 && (pInfo->item.iRow == m_ctrlStepEditGrid.GetRows() - 1))
		{
			// add more rows if nec
			m_ctrlStepEditGrid.SetRows(m_ctrlStepEditGrid.GetRows() + 1);
			long nRow = pInfo->item.iRow + 1;
			CString strFormat;
			strFormat.Format("%d", nRow);
			m_ctrlStepEditGrid.SetTextMatrix(nRow, 0, strFormat);
		}
		break;
	}
	return bMakeChange;
}

LRESULT CCKPReaction_TemperaturePg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	CString strRes;

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_LIST :
		strRes.LoadString(IDS_RXN_TEMP_185);
		m_eInputDesc.SetWindowText(strRes);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return 0;
}



BEGIN_EVENTSINK_MAP(CCKPReaction_TemperaturePg1, baseCKPReaction_TemperaturePg1)
    //{{AFX_EVENTSINK_MAP(CCKPReaction_TemperaturePg1)
	ON_EVENT(CCKPReaction_TemperaturePg1, IDC_MSHFG_LIST, -602 /* KeyDown */, OnKeyDownMshfgList, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPReaction_TemperaturePg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCKPReaction_TemperaturePg1::OnKeyDownMshfgList(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		m_ctrlStepEditGrid.SetRedraw(FALSE);
		// just fill with empty strings
		m_ctrlStepEditGrid.SetFillStyle(flexFillRepeat);
		m_ctrlStepEditGrid.SetText(_T(""));
		m_ctrlStepEditGrid.SetFillStyle(flexFillSingle);
		m_ctrlStepEditGrid.SetRedraw(TRUE);
		break;
	}
}

void CCKPReaction_TemperaturePg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0:
		strRes.LoadString(IDS_STRING495);
		break;
	case 1:
		strRes.LoadString(IDS_STRING496);
		break;
	case 2:
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("temperature data") ); 
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CCKPReaction_TemperaturePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_RADIO_LIST:
		strRes.LoadString(IDS_RXN_TEMP_184);
		break;
	case IDC_RADIO_LINEAR:
		strRes.LoadString(IDS_RXN_TEMP_183);
		break;
	case IDC_MSHFG_NUM_DESC :
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
		case 0:
			strRes.LoadString(IDS_STRING495);
			break;
		case 1:
			strRes.LoadString(IDS_STRING496);
			break;
		case 2:
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("temperature data") ); 
			break;
		}
		break;
	case IDC_MSHFG_LIST :
		strRes.LoadString(IDS_RXN_TEMP_185);
		break;
	case IDC_E_TEMP2: case IDC_S_TEMP2:
		strRes.LoadString(IDS_RXN_TEMP_187);
		break;
	case IDC_E_TEMP1: case IDC_S_TEMP1:
		strRes.LoadString(IDS_RXN_TEMP_186);
		break;
	case IDC_E_STEPS: case IDC_S_STEPS: case IDC_SPIN_STEPS:
		strRes.LoadString(IDS_RXN_TEMP_188);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
