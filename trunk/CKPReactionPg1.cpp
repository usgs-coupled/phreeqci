// CKPReactionPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPReactionPg1.h"

#include "Util.h"
#include "DDX_DDV.h"
#include "DelayUpdate.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPReactionPg1, baseCKPReactionPg1)
IMPLEMENT_DYNCREATE(CCKPReactionPg2, baseCKPReactionPg2)


/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg1 property page

CCKPReactionPg1::CCKPReactionPg1() : baseCKPReactionPg1(CCKPReactionPg1::IDD)
, m_glDoc(&m_ctrlReactionEditGrid, &m_ctrlPhaseCheckList, 1)
{
	m_bComboFilledOnce = false;
	//{{AFX_DATA_INIT(CCKPReactionPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPReactionPg1::~CCKPReactionPg1()
{
}

void CCKPReactionPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPReactionPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPReactionPg1)
	DDX_Control(pDX, IDC_CL_PHASE, m_ctrlPhaseCheckList);
	DDX_Control(pDX, IDC_MSHFG_REACTION, m_ctrlReactionEditGrid);
	//}}AFX_DATA_MAP
	DDX_NameCoefList(pDX, IDC_MSHFG_REACTION);
	//{{NEW {6/21/2000 7:10:09 PM}
	// as last step remove all entries from m_glDoc;
	if (pDX->m_bSaveAndValidate)
	{
		m_glDoc.DeleteAllRows();
	}
	//}}NEW {6/21/2000 7:10:09 PM}
}

void CCKPReactionPg1::DDX_NameCoefList(CDataExchange* pDX, int nIDC)
{
	// reduce grid flicker
	CDelayUpdate update(this, nIDC);

	ASSERT(nIDC == IDC_MSHFG_REACTION);
	if (m_bFirstSetActive)
	{
		// Init phase checklist
		CUtil::InsertPhases(&m_ctrlPhaseCheckList, GetDatabase());

		// Init reaction edit grid
		InitGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CNameCoef> listNameCoef;
		for (long nRow = m_ctrlReactionEditGrid.GetFixedRows(); nRow < m_ctrlReactionEditGrid.GetRows(); ++nRow)
		{
			CNameCoef nameCoef;

			DDX_GridText(pDX, nIDC, nRow, 1, nameCoef.m_strName);
			if (nameCoef.m_strName.IsEmpty())
				continue;

			// phase name or formula
			if ( !(::_istalpha((int)nameCoef.m_strName[0]) || (nameCoef.m_strName[0] == _T('('))) )
			{
				// Phase name or formula must begin with a letter or an open parenthesis ""("".
				DDX_GridFail(pDX, IDS_STRING488, IDS_STRING489);
			}

			// relative stoichiometry
			CString strTemp;
			DDX_GridText(pDX, nIDC, nRow, 2, strTemp);
			if (strTemp.IsEmpty())
			{
				nameCoef.m_dCoef = 1.0;
			}
			else
			{
				DDX_GridText(pDX, nIDC, nRow, 2, nameCoef.m_dCoef);
			}
			// if here no exception has occured so add to list
			listNameCoef.push_back(nameCoef);
		}
		// if here listNameCoef is valid and can be assigned to the member variable
		m_listNameCoef.assign(listNameCoef.begin(), listNameCoef.end());
	}
	else
	{
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CNameCoef>::const_iterator iter = m_listNameCoef.begin();
		CString strFormat;
		for (; iter != m_listNameCoef.end(); ++iter)
		{
			CNameCoef nameCoef(*iter);

			int nRow = m_glDoc.AddItem(nameCoef.m_strName).m_nRow;

			// relative stoichiometry
			DDX_GridTextNaN(pDX, nIDC, nRow, 2, nameCoef.m_dCoef);
		}

	}
}

void CCKPReactionPg1::InitGrid()
{
	ASSERT(::IsWindow(m_ctrlReactionEditGrid.m_hWnd));

	// set row heights
	m_ctrlReactionEditGrid.SetWordWrap(TRUE);
	m_ctrlReactionEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlReactionEditGrid.GetRowHeight(0));

	// set column titles
	m_ctrlReactionEditGrid.SetTextMatrix(0, 1, _T("Phase name or formula"));
	m_ctrlReactionEditGrid.SetTextMatrix(0, 2, _T("Relative stoichiometry"));

	// set bold titles
	m_ctrlReactionEditGrid.SetRow(0), m_ctrlReactionEditGrid.SetCol(1);
	m_ctrlReactionEditGrid.SetCellFontBold(TRUE);
	m_ctrlReactionEditGrid.SetRow(0), m_ctrlReactionEditGrid.SetCol(2);
	m_ctrlReactionEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlReactionEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlReactionEditGrid.SetRow(0);
	m_ctrlReactionEditGrid.SetCol(1);
	m_ctrlReactionEditGrid.SetColSel(m_ctrlReactionEditGrid.GetCols(0) - 1);
	m_ctrlReactionEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlReactionEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlReactionEditGrid.SetColWidth(1, 0, 1450);
	m_ctrlReactionEditGrid.SetColWidth(2, 0, 1450);

	// set row titles
	m_ctrlReactionEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlReactionEditGrid.SetTopRow(1); m_ctrlReactionEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlReactionEditGrid.SetRow(1); m_ctrlReactionEditGrid.SetCol(1);
}



BEGIN_MESSAGE_MAP(CCKPReactionPg1, baseCKPReactionPg1)
	//{{AFX_MSG_MAP(CCKPReactionPg1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PHASE, OnItemchangedClPhase)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PHASE, OnSetfocusClPhase)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg1 message handlers

BOOL CCKPReactionPg1::OnInitDialog() 
{
	baseCKPReactionPg1::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEF_PHASES, NORESIZE | ALIGN_BOTTOM)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_PHASE, GREEDY)
			<< itemFixed(HORIZONTAL, 1)
			<< item(IDC_MSHFG_REACTION, ABSOLUTE_HORZ)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CCKPReactionPg1::OnItemchangedClPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	long nRow = m_glDoc.OnItemChanged(pNMHDR, pResult);
	if (nRow != -1 && m_ctrlReactionEditGrid.GetTextMatrix(nRow, 2).IsEmpty())
	{
		m_ctrlReactionEditGrid.SetTextMatrix(nRow, 2, _T("1"));
	}
}

void CCKPReactionPg1::OnSetfocusClPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlPhaseCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;

	AfxFormatString1(strRes, IDS_CL_167, _T("phase(s)") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlPhaseCheckList.GetItemCount() && m_ctrlPhaseCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlPhaseCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

LRESULT CCKPReactionPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if (nID == IDC_MSHFG_REACTION)
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// set to phase list
			if (!m_bComboFilledOnce)
			{
				m_bComboFilledOnce = true;
				pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			}
			else
			{
				if (pInfo->item.hWndCombo)
					pInfo->item.bUseCombo = (::SendMessage(pInfo->item.hWndCombo, CB_GETCOUNT, 0, 0) > 0);
				else
					pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			}
			break;
		default :
			break;
		}
	}
	return bDenyEdit;
}

LRESULT CCKPReactionPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_REACTION :
		// fill in defaults
		if (m_ctrlReactionEditGrid.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
		{
			m_ctrlReactionEditGrid.SetTextMatrix(pInfo->item.iRow, 2, _T("1"));
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	case IDC_MSHFG_NUM_DESC :
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return bMakeChange;
}

LRESULT CCKPReactionPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_REACTION :
		OnEnterCellMshfgReaction();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}


BEGIN_EVENTSINK_MAP(CCKPReactionPg1, baseCKPReactionPg1)
    //{{AFX_EVENTSINK_MAP(CCKPReactionPg1)
	ON_EVENT(CCKPReactionPg1, IDC_MSHFG_REACTION, -602 /* KeyDown */, OnKeyDownMshfgReaction, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPReactionPg1, IDC_MSHFG_REACTION, 71 /* EnterCell */, OnEnterCellMshfgReaction, VTS_NONE)
	ON_EVENT(CCKPReactionPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg1 event handlers

void CCKPReactionPg1::OnEnterCellMshfgReaction() 
{
	CString strRes;

	switch (m_ctrlReactionEditGrid.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_RXN_179);	
		break;
	case 2 :
		strRes.LoadString(IDS_RXN_180);	
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(strRes, IDS_N_USER_140, _T("stoichiometric reaction") ); 
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("stoichiometric reaction") ); 
		break;
	case 2 :
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("stoichiometric reaction") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCKPReactionPg1::OnKeyDownMshfgReaction(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}


/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg2 property page

CCKPReactionPg2::CCKPReactionPg2() : baseCKPReactionPg2(CCKPReactionPg2::IDD)
{
	m_nType = TYPE_LINEAR;
	m_nUnits = UNITS_MOLES;
	m_dLinearAmt = 1.0;
	m_nLinearSteps = 1;
	//{{AFX_DATA_INIT(CCKPReactionPg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPReactionPg2::~CCKPReactionPg2()
{
}

void CCKPReactionPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPReactionPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPReactionPg2)
	DDX_Control(pDX, IDC_E_RXN_AMT, m_eRxnAmt);
	DDX_Control(pDX, IDC_SPIN_STEPS, m_spinSteps);
	DDX_Control(pDX, IDC_CBO_LIST_UNITS, m_cboUnitsSteps);
	DDX_Control(pDX, IDC_CBO_LINEAR_UNITS, m_cboUnitsLinear);
	DDX_Control(pDX, IDC_E_STEPS, m_eSteps);
	DDX_Control(pDX, IDC_MSHFG_LIST, m_ctrlStepEditGrid);
	//}}AFX_DATA_MAP
	DDX_Steps(pDX, IDC_MSHFG_LIST);
}

void CCKPReactionPg2::DDX_Steps(CDataExchange* pDX, int nIDC)
{

	if (m_bFirstSetActive)
	{
		InitCombos();
		InitSpin();
		InitStepEditGrid();
	}

	CButton* pLinearBtn = (CButton*)GetDlgItem(IDC_RADIO_LINEAR);
	CButton* pListBtn = (CButton*)GetDlgItem(IDC_RADIO_LIST);

	if (pDX->m_bSaveAndValidate)
	{
		switch (pLinearBtn->GetCheck())
		{
		case BST_CHECKED :
			ASSERT(pListBtn->GetCheck() == BST_UNCHECKED);
			m_nType = TYPE_LINEAR;
			DDX_CBIndex(pDX, IDC_CBO_LINEAR_UNITS, (int&)m_nUnits);
			break;

		case BST_UNCHECKED :
			ASSERT(pListBtn->GetCheck() == BST_CHECKED);
			m_nType = TYPE_LIST;
			DDX_CBIndex(pDX, IDC_CBO_LIST_UNITS, (int&)m_nUnits);
			break;
		default :
			ASSERT(FALSE);
			break;
		}

		if (m_nType == TYPE_LINEAR)
		{
			// amount
			DDX_Text(pDX, IDC_E_RXN_AMT, m_dLinearAmt);

			// steps
			DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);
			ASSERT(m_nLinearSteps > 0 && m_nLinearSteps <= INT_MAX);
		}
		else
		{
			ASSERT(m_nType == TYPE_LIST);

			// steps
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
		// set combos
		DDX_CBIndex(pDX, IDC_CBO_LIST_UNITS, (int&)m_nUnits);
		DDX_CBIndex(pDX, IDC_CBO_LINEAR_UNITS, (int&)m_nUnits);

		// exchange steps no mater what type
		ASSERT(0 < m_nLinearSteps && m_nLinearSteps <= INT_MAX);
		DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);

		// set radios
		if (m_nType == TYPE_LINEAR)
		{
			pLinearBtn->SetCheck(BST_CHECKED);

			// amount
			DDX_Text(pDX, IDC_E_RXN_AMT, m_dLinearAmt);
		}
		else if (m_nType == TYPE_LIST)
		{
			pListBtn->SetCheck(BST_CHECKED);

			// steps
			ASSERT(nIDC == m_ctrlStepEditGrid.GetDlgCtrlID());
			std::list<double>::const_iterator const_iter = m_listSteps.begin();
			for (long nRow = 1; const_iter != m_listSteps.end(); ++const_iter, ++nRow)
			{
				double value(*const_iter);
				DDX_GridText(pDX, nIDC, nRow, 1, value);
			}
		}
		else
		{
			ASSERT(FALSE); // invalid m_nType
		}

		// update radio controls
		UpdateRadioState();
	}
}


BEGIN_MESSAGE_MAP(CCKPReactionPg2, baseCKPReactionPg2)
	//{{AFX_MSG_MAP(CCKPReactionPg2)
	ON_EN_SETFOCUS(IDC_E_RXN_AMT, OnSetfocusERxnAmt)
	ON_CBN_SETFOCUS(IDC_CBO_LINEAR_UNITS, OnSetfocusCboLinearUnits)
	ON_EN_SETFOCUS(IDC_E_STEPS, OnSetfocusESteps)
	ON_CBN_SETFOCUS(IDC_CBO_LIST_UNITS, OnSetfocusCboListUnits)
	ON_BN_CLICKED(IDC_RADIO_LINEAR, OnRadioLinear)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
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

/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg2 message handlers

BOOL CCKPReactionPg2::OnInitDialog() 
{
	baseCKPReactionPg2::OnInitDialog();

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
			<< (paneCtrl(IDC_GB_LINEAR, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_STATIC_LINEAR1, NORESIZE)
					<< item(IDC_E_RXN_AMT, NORESIZE)
					)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_STATIC_LINEAR2, NORESIZE)
					<< item(IDC_CBO_LINEAR_UNITS, NORESIZE)
					)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_STATIC_LINEAR3, NORESIZE)
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
			<< (paneCtrl(IDC_GB_STEPS, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
				<< item(IDC_MSHFG_LIST, ABSOLUTE_HORZ)
				<< (pane(VERTICAL, NORESIZE)
					<< item(IDC_STATIC_STEPS1, NORESIZE)
					<< item(IDC_CBO_LIST_UNITS, NORESIZE)
					)
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

LRESULT CCKPReactionPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);
	UNUSED(nID);

	BOOL bDenyEdit = FALSE;	// allow edit

	return bDenyEdit;
}

LRESULT CCKPReactionPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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

LRESULT CCKPReactionPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC:
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_LIST:
		OnEnterCellMshfgList();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return 0;
}


BEGIN_EVENTSINK_MAP(CCKPReactionPg2, baseCKPReactionPg2)
    //{{AFX_EVENTSINK_MAP(CCKPReactionPg2)
	ON_EVENT(CCKPReactionPg2, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPReactionPg2, IDC_MSHFG_LIST, 71 /* EnterCell */, OnEnterCellMshfgList, VTS_NONE)
	ON_EVENT(CCKPReactionPg2, IDC_MSHFG_LIST, -602 /* KeyDown */, OnKeyDownMshfgList, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg2 event handlers


void CCKPReactionPg2::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0:
		AfxFormatString1(strRes, IDS_N_USER_140, _T("stoichiometric reaction") ); 
		break;
	case 1:
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("stoichiometric reaction") ); 
		break;
	case 2:
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("stoichiometric reaction") ); 
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

void CCKPReactionPg2::OnEnterCellMshfgList() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING487);
	m_eInputDesc.SetWindowText(strRes);	
}

void CCKPReactionPg2::OnKeyDownMshfgList(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlStepEditGrid.SetRedraw(FALSE);
		// just fill with empty strings
		m_ctrlStepEditGrid.SetFillStyle(flexFillRepeat);
		m_ctrlStepEditGrid.SetText(_T(""));
		m_ctrlStepEditGrid.SetFillStyle(flexFillSingle);
		m_ctrlStepEditGrid.SetRedraw(TRUE);
		break;
	}
}


void CCKPReactionPg2::OnSetfocusERxnAmt() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING492);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg2::OnSetfocusCboLinearUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING493);
	m_eInputDesc.SetWindowText(strRes);	
}

void CCKPReactionPg2::OnSetfocusESteps() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING494);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg2::OnSetfocusCboListUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING493);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg2::OnRadioLinear() 
{
	OnSetfocusRadioLinear();
	UpdateRadioState();
}

void CCKPReactionPg2::OnRadioList() 
{
	OnSetfocusRadioList();
	UpdateRadioState();
}

void CCKPReactionPg2::UpdateRadioState()
{
	CButton* pLinearBtn = (CButton*)GetDlgItem(IDC_RADIO_LINEAR);
	CButton* pListBtn = (CButton*)GetDlgItem(IDC_RADIO_LIST);

	switch (pLinearBtn->GetCheck())
	{
	case BST_CHECKED:
		ASSERT(pListBtn->GetCheck() == BST_UNCHECKED);
		m_nType = TYPE_LINEAR;
		EnableLinear(TRUE);
		EnableList(FALSE);
		break;
	case BST_UNCHECKED:
		ASSERT(pListBtn->GetCheck() == BST_CHECKED);
		m_nType = TYPE_LIST;
		EnableLinear(FALSE);
		EnableList(TRUE);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CCKPReactionPg2::EnableLinear(BOOL bEnable) 
{
	CWnd* pWnd;

	// IDC_GB_LINEAR
	pWnd = GetDlgItem(IDC_GB_LINEAR);
	pWnd->EnableWindow(bEnable);

	// IDC_STATIC_LINEAR1
	pWnd = GetDlgItem(IDC_STATIC_LINEAR1);
	pWnd->EnableWindow(bEnable);

	// IDC_E_RXN_AMT
	m_eRxnAmt.EnableWindow(bEnable);

	// IDC_STATIC_LINEAR2
	pWnd = GetDlgItem(IDC_STATIC_LINEAR2);
	pWnd->EnableWindow(bEnable);

	// IDC_CBO_LINEAR_UNITS
	m_cboUnitsLinear.EnableWindow(bEnable);

	// IDC_STATIC_LINEAR3
	pWnd = GetDlgItem(IDC_STATIC_LINEAR3);
	pWnd->EnableWindow(bEnable);

	// IDC_E_STEPS
	m_eSteps.EnableWindow(bEnable);

	// IDC_SPIN_STEPS
	m_spinSteps.EnableWindow(bEnable);
}

void CCKPReactionPg2::EnableList(BOOL bEnable) 
{
	CWnd* pWnd;

	// IDC_GB_STEPS
	pWnd = GetDlgItem(IDC_GB_STEPS);
	pWnd->EnableWindow(bEnable);

	// IDC_MSHFG_LIST
	m_ctrlStepEditGrid.EnableWindow(bEnable);

	// IDC_STATIC_STEPS1
	pWnd = GetDlgItem(IDC_STATIC_STEPS1);
	pWnd->EnableWindow(bEnable);

	// IDC_CBO_LIST_UNITS
	m_cboUnitsSteps.EnableWindow(bEnable);
}

void CCKPReactionPg2::OnSetfocusRadioLinear()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING490);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg2::OnSetfocusRadioList()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING491);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPReactionPg2::InitCombos()
{
	// fill default units combos
	CString strRes;
	VERIFY( strRes.LoadString(IDS_REACTION_UNITS_181) );
	LPTSTR lpszUnits;
	lpszUnits = _tcstok(strRes.LockBuffer(), _T(";"));
	while (lpszUnits)
	{
		m_cboUnitsLinear.AddString(lpszUnits);
		m_cboUnitsSteps.AddString(lpszUnits);
		lpszUnits = _tcstok(NULL, _T(";"));
	}
	strRes.UnlockBuffer();
	m_cboUnitsLinear.SetCurSel(m_nUnits);
	m_cboUnitsSteps.SetCurSel(m_nUnits);
}

void CCKPReactionPg2::InitSpin()
{
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

void CCKPReactionPg2::InitStepEditGrid()
{
	// set column titles
	m_ctrlStepEditGrid.SetTextMatrix(0, 1, _T("Reaction amount"));

	// center align the column headers
	m_ctrlStepEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlStepEditGrid.SetRow(0);
	m_ctrlStepEditGrid.SetCol(1);
	m_ctrlStepEditGrid.SetColSel(m_ctrlStepEditGrid.GetCols(0) - 1);
	m_ctrlStepEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlStepEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlStepEditGrid.SetColWidth(1, 0, 1450);

	// set row heights
	m_ctrlStepEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlStepEditGrid.GetRowHeight(0));


	// set row titles
	m_ctrlStepEditGrid.SetRows(max(m_ctrlStepEditGrid.GetRows(), (long)m_listSteps.size() + 2));
	m_ctrlStepEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlStepEditGrid.SetTopRow(1); m_ctrlStepEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlStepEditGrid.SetRow(1); m_ctrlStepEditGrid.SetCol(1);
}

BOOL CCKPReactionPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CL_PHASE: case IDC_S_DEF_PHASES:
		AfxFormatString1(strRes, IDS_CL_167, _T("phase(s)") ); 
		break;
	case IDC_MSHFG_REACTION:
		if (!IsContextHelp())
		{
			if (!(m_ctrlReactionEditGrid.GetRowIsVisible(m_ctrlReactionEditGrid.GetRow()) && m_ctrlReactionEditGrid.GetColIsVisible(m_ctrlReactionEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlReactionEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlReactionEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlReactionEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlReactionEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlReactionEditGrid.GetMouseCol() : m_ctrlReactionEditGrid.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_RXN_179);	
			break;
		case 2:
			strRes.LoadString(IDS_RXN_180);	
			break;
		default:
			ASSERT(FALSE);
			break;
		}
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
		case 0:
			AfxFormatString1(strRes, IDS_N_USER_140, _T("stoichiometric reaction") ); 
			break;
		case 1:
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("stoichiometric reaction") ); 
			break;
		case 2:
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("stoichiometric reaction") ); 
			break;
		default:
			ASSERT(FALSE);
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

BOOL CCKPReactionPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_LIST:
		strRes.LoadString(IDS_STRING487);
		break;
	case IDC_RADIO_LIST:
		strRes.LoadString(IDS_STRING491);
		break;
	case IDC_RADIO_LINEAR:
		strRes.LoadString(IDS_STRING490);
		break;
	case IDC_E_STEPS: case IDC_SPIN_STEPS: case IDC_STATIC_LINEAR3:
		strRes.LoadString(IDS_STRING494);
		break;
	case IDC_E_RXN_AMT: case IDC_STATIC_LINEAR1:
		strRes.LoadString(IDS_STRING492);
		break;
	case IDC_CBO_LINEAR_UNITS: case IDC_CBO_LIST_UNITS:
	case IDC_STATIC_LINEAR2: case IDC_STATIC_STEPS1:
		strRes.LoadString(IDS_STRING493);
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
			AfxFormatString1(strRes, IDS_N_USER_140, _T("stoichiometric reaction") ); 
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("stoichiometric reaction") ); 
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("stoichiometric reaction") ); 
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
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
