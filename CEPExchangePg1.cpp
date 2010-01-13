// CEPExchangePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CEPExchangePg1.h"

#include "CKSExchange.h"
#include "Util.h"
#include "DDX_DDV.h"
#include "DelayUpdate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCommonExchangePage, baseCommonExchangePage)
IMPLEMENT_DYNCREATE(CCEPExchangePg1, baseCEPExchangePg1)
IMPLEMENT_DYNCREATE(CCEPExchangePg2, baseCEPExchangePg2)
IMPLEMENT_DYNCREATE(CCEPExchangePg3, baseCEPExchangePg3)

/////////////////////////////////////////////////////////////////////////////
// CCommonExchangePage property page

CCommonExchangePage::CCommonExchangePage() : baseCommonExchangePage(CCommonExchangePage::IDD)
, m_glDoc(&m_ctrlExSpecEditGrid, &m_ctrlExSpecCheckList, 1)
{
	//{{AFX_DATA_INIT(CCommonExchangePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCommonExchangePage::CCommonExchangePage(UINT nIDTemplate, UINT nIDCaption /*= 0*/)
: baseCommonExchangePage(nIDTemplate, nIDCaption)
, m_glDoc(&m_ctrlExSpecEditGrid, &m_ctrlExSpecCheckList, 1)
{
}

CCommonExchangePage::CCommonExchangePage(LPCTSTR lpszTemplateName, UINT nIDCaption /*= 0*/)
: baseCommonExchangePage(lpszTemplateName, nIDCaption)
, m_glDoc(&m_ctrlExSpecEditGrid, &m_ctrlExSpecCheckList, 1)
{
}

CCommonExchangePage::~CCommonExchangePage()
{
}

void CCommonExchangePage::DoDataExchange(CDataExchange* pDX)
{
	baseCommonExchangePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonExchangePage)
	DDX_Control(pDX, IDC_CL_EXCHANGE, m_ctrlExSpecCheckList);
	DDX_Control(pDX, IDC_S_DEF_EX_SPEC, m_sDefExSpec);
	DDX_Control(pDX, IDC_CB_SOLUTIONS, m_cboSolutions);
	DDX_Control(pDX, IDC_EQUILIBRATE, m_btnEquilibrate);	
	DDX_Control(pDX, IDC_PITZER, m_btnPitzer);
	DDX_Control(pDX, IDC_MSHFG_EXCHANGE, m_ctrlExSpecEditGrid);
	//}}AFX_DATA_MAP
	DDX_Equilibrate(pDX);
	DDX_Pitzer(pDX);
}

void CCommonExchangePage::DDX_Equilibrate(CDataExchange* pDX)
{
	// initialize
	if (m_bFirstSetActive)
	{
		// fill defined solutions combo
		// std::set<CDBRange> setSolutions;
		// m_strNumFormat = CUtil::CreateRange(setSolutions, m_ranges[CKeyword::K_SOLUTION]);
		CUtil::InsertRange(m_cboSolutions.m_hWnd, GetSheet()->m_setSolutions);
	}

	if (pDX->m_bSaveAndValidate)
	{
		if (m_btnEquilibrate.GetCheck() == BST_CHECKED)
		{
			GetSheet()->m_bSolution_equilibria = true;
			DDX_Text(pDX, IDC_CB_SOLUTIONS, GetSheet()->m_nEquilSolutionNum);
			if (GetSheet()->m_nEquilSolutionNum < 0)
			{
				MessageBox(_T("Solution number must be greater than or equal to zero."), _T("Invalid solution number"), MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
		else
		{
			GetSheet()->m_bSolution_equilibria = false;
			GetSheet()->m_nEquilSolutionNum = CCKSExchange::N_NONE;
		}
	}
	else
	{
		CString strFormat(_T(""));
		if (GetSheet()->m_bSolution_equilibria)
		{
			m_btnEquilibrate.SetCheck(BST_CHECKED);
			// ASSERT(GetSheet()->m_nEquilSolutionNum !=  CCKSExchange::N_NONE);
			if (GetSheet()->m_nEquilSolutionNum > CCKSExchange::N_NONE)
			{
				strFormat.Format(GetSheet()->m_strNumFormat, GetSheet()->m_nEquilSolutionNum);
			}
		}
		else
		{
			m_btnEquilibrate.SetCheck(BST_UNCHECKED);
		}
		DDX_Text(pDX, IDC_CB_SOLUTIONS, strFormat);
		UpdateEquilState();
	}
}

void CCommonExchangePage::DDX_Pitzer(CDataExchange* pDX)
{
	if (pDX->m_bSaveAndValidate)
	{
		if (m_btnPitzer.GetCheck() == BST_CHECKED)
		{
			GetSheet()->m_bPitzer_exchange_gammas = true;
		}
		else
		{
			GetSheet()->m_bPitzer_exchange_gammas = false;
		}
	}
	else
	{
		if (GetSheet()->m_bPitzer_exchange_gammas)
		{
			m_btnPitzer.SetCheck(BST_CHECKED);
		}
		else
		{
			m_btnPitzer.SetCheck(BST_UNCHECKED);
		}
	}
}

BOOL CCommonExchangePage::InitExSpecCheckList()
{
	int nItem;

	std::set<CDBSpecies>::const_iterator specIter = GetDatabase().m_speciesExSet.begin();
	for (; specIter != GetDatabase().m_speciesExSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_type == CDBElement::typeEX);
		if ((nItem = m_ctrlExSpecCheckList.InsertItem(0, (*specIter).m_strName)) != -1)
		{
			// add to columns here
		}
	}
	return TRUE;
}


void CCommonExchangePage::OnEquilibrate() 
{
	int nState = m_btnEquilibrate.GetCheck();

	ASSERT(GetSheet()->m_Page1 != *this);
	//if (GetSheet()->m_Page1.m_glDoc.GetCount() > 0)
	if (GetSheet()->m_Page1.m_listExchComp.size() > 0)
	{
		if (AfxMessageBox(_T("This action will remove all selected components from the general tab.  Do you want to continue?"), MB_YESNO) == IDNO)
		{
			nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
			m_btnEquilibrate.SetCheck(nState);
			return;
		}
		else
		{
			// this is required to reset state of CheckListCtrl items
// COMMENT: {6/26/2000 7:06:38 PM}			GetSheet()->m_Page1.m_glDoc.DeleteAllRows();
			GetSheet()->m_Page1.m_listExchComp.clear();
		}
	}

	UpdateEquilState();
}

void CCommonExchangePage::UpdateEquilState()
{
	switch (m_btnEquilibrate.GetCheck())
	{
	case BST_CHECKED :
		GetSheet()->m_bSolution_equilibria = true;
		m_cboSolutions.EnableWindow(TRUE);
		break;
	case BST_UNCHECKED:
		GetSheet()->m_bSolution_equilibria = false;
		m_cboSolutions.EnableWindow(FALSE);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
}

#ifdef _DEBUG
CCKSExchange* CCommonExchangePage::GetSheet() // non-debug version is inline
{
	CCKSExchange* pSheet = (CCKSExchange*)GetParent();
	ASSERT_KINDOF(CCKSExchange, pSheet);
	return pSheet;
}
#endif

BEGIN_MESSAGE_MAP(CCommonExchangePage, baseCommonExchangePage)
	//{{AFX_MSG_MAP(CCommonExchangePage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EQUILIBRATE, OnEquilibrate)
	ON_CBN_SETFOCUS(IDC_CB_SOLUTIONS, OnSetfocusCbSolutions)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_EXCHANGE, OnSetfocusClExchange)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_EXCHANGE, OnItemchangedClExchange)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom setfocus notifications
	ON_BN_SETFOCUS(IDC_EQUILIBRATE, OnSetfocusBtnEquil)
	ON_BN_SETFOCUS(IDC_PITZER, OnSetfocusBtnPitzer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonExchangePage message handlers

BOOL CCommonExchangePage::OnInitDialog() 
{
	baseCommonExchangePage::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< (paneCtrl(IDC_GB_OPTIONS, HORIZONTAL, GREEDY, nDefaultBorder, 7, 10, 0)
			<< itemFixed(HORIZONTAL, 15)
			<< item(IDC_EQUILIBRATE, NORESIZE|ALIGN_CENTER)
			<< item(IDC_CB_SOLUTIONS, NORESIZE|ALIGN_CENTER)
			<< itemFixed(HORIZONTAL, 15)
			<< item(IDC_PITZER, NORESIZE|ALIGN_CENTER)			
			)

		<< item(IDC_S_DEF_EX_SPEC, NORESIZE | ALIGN_BOTTOM)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_EXCHANGE, GREEDY)
			<< itemFixed(HORIZONTAL, 1)
			<< item(IDC_MSHFG_EXCHANGE, ABSOLUTE_HORZ)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonExchangePage::OnSize(UINT nType, int cx, int cy) 
{
	baseCommonExchangePage::OnSize(nType, cx, cy);	
	// Add your message handler code here	
}

void CCommonExchangePage::OnSetfocusBtnEquil()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_EXCHANGE_165);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonExchangePage::OnSetfocusBtnPitzer()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING697);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonExchangePage::OnSetfocusCbSolutions() 
{
	CString strRes;
	strRes.LoadString(IDS_EXCHANGE_166);
	m_eInputDesc.SetWindowText(strRes);
}

void CCommonExchangePage::OnSetfocusClExchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlExSpecCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;

	AfxFormatString1(strRes, IDS_CL_167, _T("exchange species") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlExSpecCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlExSpecCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CCommonExchangePage::OnItemchangedClExchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	ASSERT(pNMListView->hdr.hwndFrom == m_ctrlExSpecCheckList.m_hWnd);
	UNUSED(pNMListView);   // unused in release builds

	// Add your control notification handler code here
	long nRow = m_glDoc.OnItemChanged(pNMHDR, pResult);
	UNREFERENCED_PARAMETER(nRow);
}


BEGIN_EVENTSINK_MAP(CCommonExchangePage, baseCommonExchangePage)
    //{{AFX_EVENTSINK_MAP(CCommonExchangePage)
	ON_EVENT(CCommonExchangePage, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonExchangePage event handlers

void CCommonExchangePage::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		// Positive number to designate this %1 and its composition. Default is 1.
		AfxFormatString1(strRes, IDS_N_USER_140, _T("exchange assemblage") ); 
		break;
	case 1 :
		// Optional positive number defining range of %1 assemblage numbers
		// in which this %1 assemblage composition will be stored.  Number must
		// be greater than number on previous line.
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("exchange") ); 
		break;
	case 2 :
		// Optional comment that describes the %1.
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("exchanger") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg1 property page

CCEPExchangePg1::CCEPExchangePg1() : baseCEPExchangePg1(CCommonExchangePage::IDD, IDS_EXCHANGEPG1_168)
{
	//{{AFX_DATA_INIT(CCEPExchangePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCEPExchangePg1::~CCEPExchangePg1()
{
}

void CCEPExchangePg1::DoDataExchange(CDataExchange* pDX)
{
	baseCEPExchangePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCEPExchangePg1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_ExchCompList(pDX, IDC_MSHFG_EXCHANGE);
	//{{NEW {6/21/2000 7:10:09 PM}
	// as last step remove all entries from m_glDoc;
// COMMENT: {6/26/2000 7:03:11 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {6/26/2000 7:03:11 PM}	{
// COMMENT: {6/26/2000 7:03:11 PM}		m_glDoc.DeleteAllRows();
// COMMENT: {6/26/2000 7:03:11 PM}	}
	//}}NEW {6/21/2000 7:10:09 PM}
}

void CCEPExchangePg1::DDX_ExchCompList(CDataExchange* pDX, int nIDC)
{
	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_EXCHANGE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitExSpecEditGrid();
		InitExSpecCheckList();
	}

	// do everytime page is set active
	if (!pDX->m_bSaveAndValidate)
	{
		InitExSpecCheckList();
		// Update grid titles
		if (GetSheet()->m_bSolution_equilibria)
		{
			m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nsite"));
		}
		else
		{
			m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nformula"));
		}
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CExchComp> listExchComp;
		for (long nRow = m_ctrlExSpecEditGrid.GetFixedRows(); nRow < m_ctrlExSpecEditGrid.GetRows(); ++nRow)
		{
			// exchange formula, amount
			// exchange site, amount
			CExchComp exchComp;

			// exchange formula or exchange site
			DDX_GridText(pDX, nIDC, nRow, 1, exchComp.m_strFormula);
			if (exchComp.m_strFormula.IsEmpty())
				continue;

			// validate formula or site
			if (!CUtil::IsValidExchangerName(exchComp.m_strFormula))
			{
				DDX_GridFail(pDX, _T("Expected exchanger name to begin with a capital letter."),
					_T("Error"));
			}

			// amount
			CString strDummy;
			DDX_GridText(pDX, nIDC, nRow, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected concentration of exchanger."),
					_T("Missing data"));
			}
			DDX_GridText(pDX, nIDC, nRow, 2, exchComp.m_ldMoles);

			// if here no exception has occured so add to list
			listExchComp.push_back(exchComp);
		}
		// if here listExchComp is valid and can be assigned to the member variable
		m_listExchComp.empty();
		m_listExchComp.assign(listExchComp.begin(), listExchComp.end());
	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		CString strFormat;
		std::list<CExchComp>::iterator iter = m_listExchComp.begin();
		for (; iter != m_listExchComp.end(); ++iter)
		{
			// exchange formula, amount
			// exchange site, amount
			CExchComp exchComp(*iter);

			// exchange formula or exchange site
			// add to grid list doc
			int nRow = m_glDoc.AddItem(exchComp.m_strFormula).m_nRow;

			// amount
			DDX_GridText(pDX, nIDC, nRow, 2, (*iter).m_ldMoles);
		}
		ASSERT(m_listExchComp.size() == m_glDoc.GetCount());
	}	
}

BOOL CCEPExchangePg1::InitExSpecCheckList()
{
	int nItem;

	m_ctrlExSpecCheckList.DeleteAllItems();

	if (GetSheet()->m_bSolution_equilibria)
	{
		// set static control
		m_sDefExSpec.SetWindowText(_T("&Defined exchange sites"));

		std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
		for (; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
		{
			if ((*elemIter).m_type == CDBElement::typeEX)
			{
				if ((nItem = m_ctrlExSpecCheckList.InsertItem(0, (*elemIter).m_strName)) != -1)
				{
					// add to columns here
				}
			}
		}
	}
	else 
	{
		// set static control
		m_sDefExSpec.SetWindowText(_T("&Defined exchange species"));

		std::set<CDBSpecies>::const_iterator specIter = GetDatabase().m_speciesExSet.begin();
		for (; specIter != GetDatabase().m_speciesExSet.end(); ++specIter)
		{
			ASSERT((*specIter).m_type == CDBElement::typeEX);
			if ((nItem = m_ctrlExSpecCheckList.InsertItem(0, (*specIter).m_strName)) != -1)
			{
				// add to columns here
			}
		}
	}

	return TRUE;
}

BOOL CCEPExchangePg1::InitExSpecEditGrid()
{
	// set rows, cols
	m_ctrlExSpecEditGrid.SetRows(21);
	m_ctrlExSpecEditGrid.SetCols(0, 3);

	// set header row height
	m_ctrlExSpecEditGrid.SetWordWrap(TRUE);
	m_ctrlExSpecEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlExSpecEditGrid.GetRowHeight(1));


	// set column titles
	if (GetSheet()->m_bSolution_equilibria)
	{
		m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nsite"));
		m_ctrlExSpecEditGrid.SetTextMatrix(0, 2, _T("Amount\r\n(moles)"));
	}
	else
	{
		m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nformula"));
		m_ctrlExSpecEditGrid.SetTextMatrix(0, 2, _T("Amount\r\n(moles)"));
	}

	// set bold titles
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(2);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlExSpecEditGrid.SetRow(0);
	m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetColSel(m_ctrlExSpecEditGrid.GetCols(0) - 1);
	m_ctrlExSpecEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlExSpecEditGrid.SetColWidth(0, 0, 500);
	m_ctrlExSpecEditGrid.SetColWidth(1, 0, 1200);
	m_ctrlExSpecEditGrid.SetColWidth(2, 0, 1600);

	// set row titles
	m_ctrlExSpecEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlExSpecEditGrid.SetTopRow(1); m_ctrlExSpecEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlExSpecEditGrid.SetRow(1); m_ctrlExSpecEditGrid.SetCol(1);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCEPExchangePg1, baseCEPExchangePg1)
	//{{AFX_MSG_MAP(CCEPExchangePg1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EQUILIBRATE, OnEquilibrate)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_EXCHANGE, OnSetfocusClExchange)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg1 message handlers

LRESULT CCEPExchangePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_EXCHANGE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			if (GetSheet()->m_bSolution_equilibria)
			{
				pInfo->item.bUseCombo = (CUtil::InsertExchangeSites(pInfo->item.hWndCombo, GetDatabase()) > 0);
			}
			else
			{
				pInfo->item.bUseCombo = (CUtil::InsertExchangeSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
			}
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCEPExchangePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
	case IDC_MSHFG_EXCHANGE :
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}
	return bMakeChange;
}


void CCEPExchangePg1::OnEquilibrate() 
{
	int nState = m_btnEquilibrate.GetCheck();

	if (m_glDoc.GetCount() > 0)
	{
		if (AfxMessageBox(_T("This action will remove all selected components.  Do you want to continue?"), MB_YESNO) == IDNO)
		{
			nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
			m_btnEquilibrate.SetCheck(nState);
			return;
		}
		else
		{
			// this is required to reset state of CheckListCtrl items
			m_glDoc.DeleteAllRows();
			m_listExchComp.clear();
		}
	}
	UpdateEquilState();
	UpdateData(FALSE);
}

BOOL CCEPExchangePg1::OnKillActive() 
{
	if (baseCEPExchangePg1::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}


void CCEPExchangePg1::OnSetfocusClExchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlExSpecCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;

	if (GetSheet()->m_bSolution_equilibria)
	{
		AfxFormatString1(strRes, IDS_CL_167, _T("exchange site") ); 
	}
	else
	{
		AfxFormatString1(strRes, IDS_CL_167, _T("exchange species") ); 
	}

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlExSpecCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlExSpecCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}


LRESULT CCEPExchangePg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_EXCHANGE :
		OnEnterCellMshfgExchange();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CCEPExchangePg1, baseCEPExchangePg1)
    //{{AFX_EVENTSINK_MAP(CCEPExchangePg1)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CCEPExchangePg1, IDC_MSHFG_EXCHANGE, -602 /* KeyDown */, OnKeyDownMshfgExchange, VTS_PI2 VTS_I2)
	ON_EVENT(CCEPExchangePg1, IDC_MSHFG_EXCHANGE, 71 /* EnterCell */, OnEnterCellMshfgExchange, VTS_NONE)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg1 event handlers

void CCEPExchangePg1::OnEnterCellMshfgExchange() 
{
	CString strRes;
	switch (m_ctrlExSpecEditGrid.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_STRING470);
		break;
	case 2:
		strRes.LoadString(IDS_STRING471);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCEPExchangePg1::OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}


/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg2 property page

CCEPExchangePg2::CCEPExchangePg2() : baseCEPExchangePg2(CCommonExchangePage::IDD, IDS_EXCHANGEPG2_169)
{
	//{{AFX_DATA_INIT(CCEPExchangePg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCEPExchangePg2::~CCEPExchangePg2()
{
}

void CCEPExchangePg2::DoDataExchange(CDataExchange* pDX)
{
	baseCEPExchangePg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCEPExchangePg2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_ExchCompList(pDX, IDC_MSHFG_EXCHANGE);
	//{{NEW {6/21/2000 7:10:09 PM}
	// as last step remove all entries from m_glDoc;
	if (pDX->m_bSaveAndValidate)
	{
		m_glDoc.DeleteAllRows();
	}
	//}}NEW {6/21/2000 7:10:09 PM}
}

void CCEPExchangePg2::DDX_ExchCompList(CDataExchange* pDX, int nIDC)
{
	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_EXCHANGE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitExSpecEditGrid();
		InitExSpecCheckList();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CExchComp> listExchComp;
		for (long nRow = m_ctrlExSpecEditGrid.GetFixedRows(); nRow < m_ctrlExSpecEditGrid.GetRows(); ++nRow)
		{
			// exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
			CExchComp exchComp;

			// exchange formula
			DDX_GridText(pDX, nIDC, nRow, 1, exchComp.m_strFormula);
			if (exchComp.m_strFormula.IsEmpty())
				continue;

			// validate formula or site
			if (!CUtil::IsValidExchangerName(exchComp.m_strFormula))
			{
				DDX_GridFail(pDX, _T("Expected exchanger name to begin with a capital letter."),
					_T("Error"));
			}

			// name
			DDX_GridText(pDX, nIDC, nRow, 2, exchComp.m_strPhase_name);
			if (exchComp.m_strPhase_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected mineral name."),
					_T("Missing data"));
			}

			// exchange_per_mole
			CString strDummy;
			DDX_GridText(pDX, nIDC, nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate exchanger to mineral."),
					_T("Missing data"));
			}
			DDX_GridText(pDX, nIDC, nRow, 3, exchComp.m_dPhase_proportion);

			// OK
			listExchComp.push_back(exchComp);
		}
		// all OK
		m_listExchComp.assign(listExchComp.begin(), listExchComp.end());
	}
	else
	{
		//{{NEW {6/21/2000 7:10:09 PM}
		// this is required to reset state of CheckListCtrl items
		//// SEE DoDataExchange
		//// m_glDoc.DeleteAllRows();
		ASSERT(m_glDoc.GetCount() == 0);
		//}}NEW {6/21/2000 7:10:09 PM}

		std::list<CExchComp>::const_iterator iter = m_listExchComp.begin();
		for (; iter != m_listExchComp.end(); ++iter)
		{
			// exchange formula, name, [(equilibrium_phase or kinetic_reactant)], sites_per_mole
			CExchComp exchComp(*iter);

			// exchange formula
			// add to grid list doc
			int nRow = m_glDoc.AddItem(exchComp.m_strFormula).m_nRow;

			// name
			ASSERT(!exchComp.m_strPhase_name.IsEmpty());
			ASSERT(exchComp.m_strRate_name.IsEmpty());
			DDX_GridText(pDX, nIDC, nRow, 2, exchComp.m_strPhase_name);

			// sites_per_mole
			DDX_GridText(pDX, nIDC, nRow, 3, exchComp.m_dPhase_proportion);
		}
		ASSERT(m_listExchComp.size() == m_glDoc.GetCount());
	}	
}

BOOL CCEPExchangePg2::InitExSpecEditGrid()
{
	// set rows, cols
	m_ctrlExSpecEditGrid.SetRows(21);
	m_ctrlExSpecEditGrid.SetCols(0, 4);

	// set header row height
	m_ctrlExSpecEditGrid.SetWordWrap(TRUE);
	m_ctrlExSpecEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlExSpecEditGrid.GetRowHeight(1));


	// set column titles
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nformula"));
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 2, _T("Equilibrium\r\nphase"));
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 3, _T("Sites/mole"));

	// set bold titles
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(2);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(3);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlExSpecEditGrid.SetRow(0);
	m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetColSel(m_ctrlExSpecEditGrid.GetCols(0) - 1);
	m_ctrlExSpecEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlExSpecEditGrid.SetColWidth(0, 0, 500);
	m_ctrlExSpecEditGrid.SetColWidth(1, 0, 1200);
	m_ctrlExSpecEditGrid.SetColWidth(2, 0, 1200);
	m_ctrlExSpecEditGrid.SetColWidth(3, 0, 1200);

	// set row titles
	m_ctrlExSpecEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlExSpecEditGrid.SetTopRow(1); m_ctrlExSpecEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlExSpecEditGrid.SetRow(1); m_ctrlExSpecEditGrid.SetCol(1);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCEPExchangePg2, baseCEPExchangePg2)
	//{{AFX_MSG_MAP(CCEPExchangePg2)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg2 message handlers

LRESULT CCEPExchangePg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_EXCHANGE :
		OnEnterCellMshfgExchange();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

LRESULT CCEPExchangePg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_EXCHANGE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertExchangeSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
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

LRESULT CCEPExchangePg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
	case IDC_MSHFG_EXCHANGE :
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}
	return bMakeChange;
}

BEGIN_EVENTSINK_MAP(CCEPExchangePg2, baseCEPExchangePg2)
    //{{AFX_EVENTSINK_MAP(CCEPExchangePg2)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CCEPExchangePg2, IDC_MSHFG_EXCHANGE, -602 /* KeyDown */, OnKeyDownMshfgExchange, VTS_PI2 VTS_I2)
	ON_EVENT(CCEPExchangePg2, IDC_MSHFG_EXCHANGE, 71 /* EnterCell */, OnEnterCellMshfgExchange, VTS_NONE)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg2 event handlers

void CCEPExchangePg2::OnEnterCellMshfgExchange() 
{
	CString strRes;
	switch (m_ctrlExSpecEditGrid.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_STRING472);
		break;
	case 2:
		strRes.LoadString(IDS_STRING473);
		break;
	case 3:
		strRes.LoadString(IDS_STRING474);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCEPExchangePg2::OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}


/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg3 property page

CCEPExchangePg3::CCEPExchangePg3() : baseCEPExchangePg3(CCommonExchangePage::IDD, IDS_EXCHANGEPG3_170)
{
	//{{AFX_DATA_INIT(CCEPExchangePg3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCEPExchangePg3::~CCEPExchangePg3()
{
}

void CCEPExchangePg3::DoDataExchange(CDataExchange* pDX)
{
	baseCEPExchangePg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCEPExchangePg3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_ExchCompList(pDX, IDC_MSHFG_EXCHANGE);
	//{{NEW {6/21/2000 7:10:09 PM}
	// as last step remove all entries from m_glDoc;
	if (pDX->m_bSaveAndValidate)
	{
		m_glDoc.DeleteAllRows();
	}
	//}}NEW {6/21/2000 7:10:09 PM}
}

void CCEPExchangePg3::DDX_ExchCompList(CDataExchange* pDX, int nIDC)
{
	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_EXCHANGE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitExSpecEditGrid();
		InitExSpecCheckList();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CExchComp> listExchComp;
		for (long nRow = m_ctrlExSpecEditGrid.GetFixedRows(); nRow < m_ctrlExSpecEditGrid.GetRows(); ++nRow)
		{
			// exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
			CExchComp exchComp;

			// exchange formula
			DDX_GridText(pDX, nIDC, nRow, 1, exchComp.m_strFormula);
			if (exchComp.m_strFormula.IsEmpty())
				continue;

			// validate formula or site
			if (!CUtil::IsValidExchangerName(exchComp.m_strFormula))
			{
				DDX_GridFail(pDX, _T("Expected exchanger name to begin with a capital letter."),
					_T("Error"));
			}

			// name
			DDX_GridText(pDX, nIDC, nRow, 2, exchComp.m_strRate_name);
			if (exchComp.m_strRate_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected kinetic reaction name."),
					_T("Missing data"));
			}

			// exchange_per_mole
			CString strDummy;
			DDX_GridText(pDX, nIDC, nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate exchanger to kinetic reaction."),
					_T("Missing data"));
			}
			DDX_GridText(pDX, nIDC, nRow, 3, exchComp.m_dPhase_proportion);

			// if here no exception has occured so add to list
			listExchComp.push_back(exchComp);
		}
		// if here listExchComp is valid and can be assigned to the member variable
		m_listExchComp.empty();
		m_listExchComp.assign(listExchComp.begin(), listExchComp.end());
	}
	else
	{
		//{{NEW {6/21/2000 7:10:09 PM}
		// this is required to reset state of CheckListCtrl items
		//// SEE DoDataExchange
		//// m_glDoc.DeleteAllRows();
		ASSERT(m_glDoc.GetCount() == 0);
		//}}NEW {6/21/2000 7:10:09 PM}

		std::list<CExchComp>::const_iterator iter = m_listExchComp.begin();
		for (; iter != m_listExchComp.end(); ++iter)
		{
			// exchange formula, name, [(equilibrium_phase or kinetic_reactant)], sites_per_mole
			CExchComp exchComp(*iter);

			// exchange formula
			// add to grid list doc
			int nRow = m_glDoc.AddItem(exchComp.m_strFormula).m_nRow;

			// name
			ASSERT(!exchComp.m_strRate_name.IsEmpty());
			ASSERT(exchComp.m_strPhase_name.IsEmpty());
			DDX_GridText(pDX, nIDC, nRow, 2, exchComp.m_strRate_name);

			// sites_per_mole
			DDX_GridText(pDX, nIDC, nRow, 3, exchComp.m_dPhase_proportion);
		}
		ASSERT(m_listExchComp.size() == m_glDoc.GetCount());
	}	
}

BOOL CCEPExchangePg3::InitExSpecEditGrid()
{
	// set rows, cols
	m_ctrlExSpecEditGrid.SetRows(21);
	m_ctrlExSpecEditGrid.SetCols(0, 4);

	// set header row height
	m_ctrlExSpecEditGrid.SetWordWrap(TRUE);
	m_ctrlExSpecEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlExSpecEditGrid.GetRowHeight(1));


	// set column titles
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 1, _T("Exchange\r\nformula"));
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 2, _T("Kinetic\r\nreactant"));
	m_ctrlExSpecEditGrid.SetTextMatrix(0, 3, _T("Sites/mole"));

	// set bold titles
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(2);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);
	m_ctrlExSpecEditGrid.SetRow(0), m_ctrlExSpecEditGrid.SetCol(3);
	m_ctrlExSpecEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlExSpecEditGrid.SetRow(0);
	m_ctrlExSpecEditGrid.SetCol(1);
	m_ctrlExSpecEditGrid.SetColSel(m_ctrlExSpecEditGrid.GetCols(0) - 1);
	m_ctrlExSpecEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlExSpecEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
	m_ctrlExSpecEditGrid.SetColWidth(0, 0, 500);
	m_ctrlExSpecEditGrid.SetColWidth(1, 0, 1200);
	m_ctrlExSpecEditGrid.SetColWidth(2, 0, 1200);
	m_ctrlExSpecEditGrid.SetColWidth(3, 0, 1200);

	// set row titles
	m_ctrlExSpecEditGrid.AddRowHeaders();

	// scroll to top of grid
	m_ctrlExSpecEditGrid.SetTopRow(1); m_ctrlExSpecEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlExSpecEditGrid.SetRow(1); m_ctrlExSpecEditGrid.SetCol(1);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCEPExchangePg3, baseCEPExchangePg3)
	//{{AFX_MSG_MAP(CCEPExchangePg3)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg3 message handlers

LRESULT CCEPExchangePg3::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_EXCHANGE :
		OnEnterCellMshfgExchange();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

LRESULT CCEPExchangePg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_EXCHANGE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertExchangeSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		case 2 :
			pInfo->item.bUseCombo = (CUtil::InsertRates(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCEPExchangePg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
	case IDC_MSHFG_EXCHANGE :
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}
	return bMakeChange;
}

BEGIN_EVENTSINK_MAP(CCEPExchangePg3, baseCEPExchangePg3)
    //{{AFX_EVENTSINK_MAP(CCEPExchangePg3)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CCEPExchangePg3, IDC_MSHFG_EXCHANGE, -602 /* KeyDown */, OnKeyDownMshfgExchange, VTS_PI2 VTS_I2)
	ON_EVENT(CCEPExchangePg3, IDC_MSHFG_EXCHANGE, 71 /* EnterCell */, OnEnterCellMshfgExchange, VTS_NONE)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg3 event handlers

void CCEPExchangePg3::OnEnterCellMshfgExchange() 
{
	CString strRes;
	switch (m_ctrlExSpecEditGrid.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_STRING475);
		break;

	case 2:
		strRes.LoadString(IDS_STRING476);
		break;

	case 3:
		strRes.LoadString(IDS_STRING477);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCEPExchangePg3::OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

BOOL CCEPExchangePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	//{{ HANDLED BY CCommonExchangePage
	case IDC_MSHFG_NUM_DESC: case IDC_CB_SOLUTIONS:
	case IDC_EQUILIBRATE:
		return CCommonExchangePage::OnHelpInfo(pHelpInfo);
	//}} HANDLED BY CCommonExchangePage
		break;

	case IDC_MSHFG_EXCHANGE:
		if (!IsContextHelp())
		{
			if (!(m_ctrlExSpecEditGrid.GetRowIsVisible(m_ctrlExSpecEditGrid.GetRow()) && m_ctrlExSpecEditGrid.GetColIsVisible(m_ctrlExSpecEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlExSpecEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlExSpecEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlExSpecEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlExSpecEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlExSpecEditGrid.GetMouseCol() : m_ctrlExSpecEditGrid.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_STRING470);
			break;
		case 2:
			strRes.LoadString(IDS_STRING471);
			break;
		}
		break;
	case IDC_CL_EXCHANGE:
		if (GetSheet()->m_bSolution_equilibria)
		{
			AfxFormatString1(strRes, IDS_CL_167, _T("exchange site") ); 
		}
		else
		{
			AfxFormatString1(strRes, IDS_CL_167, _T("exchange species") ); 
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

BOOL CCommonExchangePage::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_EQUILIBRATE:
		strRes.LoadString(IDS_EXCHANGE_165);
		break;
	case IDC_PITZER:
		strRes.LoadString(IDS_STRING697);
		break;
	case IDC_CL_EXCHANGE:
		AfxFormatString1(strRes, IDS_CL_167, _T("exchange species") );
		break;
	case IDC_CB_SOLUTIONS:
		strRes.LoadString(IDS_EXCHANGE_166);
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
			// Positive number to designate this %1 and its composition. Default is 1.
			AfxFormatString1(strRes, IDS_N_USER_140, _T("exchange assemblage") ); 
			break;
		case 1 :
			// Optional positive number defining range of %1 assemblage numbers
			// in which this %1 assemblage composition will be stored.  Number must
			// be greater than number on previous line.
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("exchange") ); 
			break;
		case 2 :
			// Optional comment that describes the %1.
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("exchanger") ); 
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

BOOL CCEPExchangePg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	//{{ HANDLED BY CCommonExchangePage
	case IDC_MSHFG_NUM_DESC: case IDC_CB_SOLUTIONS: case IDC_CL_EXCHANGE:
	case IDC_EQUILIBRATE:
		return CCommonExchangePage::OnHelpInfo(pHelpInfo);
	//}} HANDLED BY CCommonExchangePage
		break;
	case IDC_MSHFG_EXCHANGE:
		if (!IsContextHelp())
		{
			if (!(m_ctrlExSpecEditGrid.GetRowIsVisible(m_ctrlExSpecEditGrid.GetRow()) && m_ctrlExSpecEditGrid.GetColIsVisible(m_ctrlExSpecEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlExSpecEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlExSpecEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlExSpecEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlExSpecEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlExSpecEditGrid.GetMouseCol() : m_ctrlExSpecEditGrid.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_STRING472);
			break;
		case 2:
			strRes.LoadString(IDS_STRING473);
			break;
		case 3:
			strRes.LoadString(IDS_STRING474);
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

BOOL CCEPExchangePg3::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_NUM_DESC: case IDC_CB_SOLUTIONS: case IDC_CL_EXCHANGE:
	case IDC_EQUILIBRATE:
		return CCommonExchangePage::OnHelpInfo(pHelpInfo);
		break;

	case IDC_MSHFG_EXCHANGE:
		if (!IsContextHelp())
		{
			if (!(m_ctrlExSpecEditGrid.GetRowIsVisible(m_ctrlExSpecEditGrid.GetRow()) && m_ctrlExSpecEditGrid.GetColIsVisible(m_ctrlExSpecEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlExSpecEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlExSpecEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlExSpecEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlExSpecEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlExSpecEditGrid.GetMouseCol() : m_ctrlExSpecEditGrid.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_STRING475);
			break;
		case 2:
			strRes.LoadString(IDS_STRING476);
			break;
		case 3:
			strRes.LoadString(IDS_STRING477);
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
