// CKPKineticsPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPKineticsPg1.h"

#include "Util.h"
#include "DelayUpdate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPKineticsPg1, baseCKPKineticsPg1)
IMPLEMENT_DYNCREATE(CCKPKineticsPg2, baseCKPKineticsPg2)


/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg1 property page

CCKPKineticsPg1::CCKPKineticsPg1() : baseCKPKineticsPg1(CCKPKineticsPg1::IDD)
, m_gltDoc(&m_egNames, &m_clRates, &m_tcNames, 1), m_nextMapInt(1)
{
	m_bIgnoreCheckChange = FALSE;
	//{{AFX_DATA_INIT(CCKPKineticsPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPKineticsPg1::~CCKPKineticsPg1()
{
}

void CCKPKineticsPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPKineticsPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPKineticsPg1)
	DDX_Control(pDX, IDC_CL_RATES, m_clRates);
	DDX_Control(pDX, IDC_TC_NAMES, m_tcNames);
	DDX_Control(pDX, IDC_MSHFG_NAMES, m_egNames);
	DDX_Control(pDX, IDC_MSHFG_GENERAL, m_egGeneral);
	DDX_Control(pDX, IDC_MSHFG_FORMULA, m_egFormula);
	DDX_Control(pDX, IDC_MSHFG_USER, m_egUser);
	//}}AFX_DATA_MAP
	DDX_CompList(pDX);
}

void CCKPKineticsPg1::DDX_CompList(CDataExchange *pDX)
{
	// validate  state
	ASSERT_VALID(this);

	if (m_bFirstSetActive)
	{
		// Init grid controls
		InitGrids();

		// Init rate checklist
		CUtil::InsertRates(&m_clRates, GetDatabase());
	}

	// avoid flashing grid
	CDelayUpdate update1(this, IDC_MSHFG_GENERAL);
	CDelayUpdate update2(this, IDC_MSHFG_USER);
	CDelayUpdate update3(this, IDC_MSHFG_FORMULA);

	if (pDX->m_bSaveAndValidate)
	{
		// check if this tab has a CKineticComp associated with it
		int nTab = m_tcNames.GetCurSel();
		if (nTab == -1)
		{
			// no CKineticComps defined
			ASSERT(m_tcNames.GetItemCount() == 0);

			// remove any current CKineticComps
			m_listComps.clear();
		}
		else
		{
			// CKineticComps have been defined
			ASSERT(m_tcNames.GetItemCount() != 0);

			TCITEM tcItem;
			TCHAR buffer[121];
			tcItem.pszText = buffer;
			tcItem.cchTextMax = 120;
			tcItem.mask = TCIF_PARAM | TCIF_TEXT;

			VERIFY(m_tcNames.GetItem(nTab, &tcItem));

			if (!tcItem.lParam)
			{
				// this is a new tab with no associated CKineticComp
				tcItem.lParam = m_nextMapInt++;
				m_tcNames.SetItem(nTab, &tcItem);
				ASSERT(m_mapComps.find(tcItem.lParam) == m_mapComps.end());
			}

			CKineticComp kineticCompNew;

			// Rate name
			// m_strRateName already validated by OnEndCellEdit
			kineticCompNew.m_strRateName = tcItem.pszText;


			//
			// General parameters
			//

			// Initial moles (m0)
			DDX_GridText(pDX, IDC_MSHFG_GENERAL, 0, 1, kineticCompNew.m_dM0);

			// Moles (m)
			DDX_GridText(pDX, IDC_MSHFG_GENERAL, 1, 1, kineticCompNew.m_dM);

			// Tolerance (tol)
			DDX_GridText(pDX, IDC_MSHFG_GENERAL, 2, 1, kineticCompNew.m_dTol);

			//
			// User parameters
			//

			// Parm(1..N)
			CString strUserVal;
			double dUserVal;
			for (long nRow = 0; nRow < m_egUser.GetRows(); ++nRow)
			{
				DDX_GridText(pDX, IDC_MSHFG_USER, nRow, 1, strUserVal);
				if (strUserVal.IsEmpty()) continue;
				DDX_GridText(pDX, IDC_MSHFG_USER, nRow, 1, dUserVal);
				kineticCompNew.m_listDParams.push_back(dUserVal);
			}

			//
			// Formula
			//

			// Formula or phase name
			CNameCoef nameCoef;
			for (nRow = 1; nRow < m_egFormula.GetRows(); ++nRow)
			{
				DDX_GridText(pDX, IDC_MSHFG_FORMULA, nRow, 1, strUserVal);
				if (strUserVal.IsEmpty()) continue;
				nameCoef.m_strName = strUserVal;
				
				DDX_GridText(pDX, IDC_MSHFG_FORMULA, nRow, 2, strUserVal);
				if (strUserVal.IsEmpty())
				{
					nameCoef.m_dCoef = 1.0;
				}
				else
				{
					DDX_GridText(pDX, IDC_MSHFG_FORMULA, nRow, 2, nameCoef.m_dCoef);
				}
				kineticCompNew.m_listNameCoef.push_back(nameCoef);
			}

			// update map
			m_mapComps[tcItem.lParam] = kineticCompNew;

			// copy CKineticComps in map to list in tab order
			//TCITEM tcItem;
			tcItem.mask = TCIF_PARAM | TCIF_TEXT;
			m_listComps.clear();

			for (int i = 0; i < m_tcNames.GetItemCount(); ++i)
			{
				m_tcNames.GetItem(i, &tcItem);
				CKineticComp kineticComp = m_mapComps[tcItem.lParam];
				if (tcItem.lParam == 0)
				{
					ASSERT(kineticComp.m_strRateName.IsEmpty());
					ASSERT(tcItem.pszText != NULL);
					kineticComp.m_strRateName = tcItem.pszText;
				}
				else
				{
					ASSERT(!kineticComp.m_strRateName.IsEmpty());
				}

				// add CNameCoef if this rate name is a phase
				if (kineticComp.m_listNameCoef.size() == 0)
				{
					CDBPhase phase;
					phase.m_strName = kineticComp.m_strRateName;

					std::set<CDBPhase>::const_iterator phaseIter = GetDatabase().m_phaseSet.find(phase);
					if (phaseIter != GetDatabase().m_phaseSet.end())
					{
						CNameCoef nameCoef;
						nameCoef.m_strName = phaseIter->m_strFormula;
						nameCoef.m_dCoef = 1.0;
						kineticComp.m_listNameCoef.push_back(nameCoef);
					}
				}
				m_listComps.push_back(kineticComp);

#ifdef _DEBUG
				afxDump << "Item " << i << "\n";
				afxDump << "\tName = " << kineticComp.m_strRateName << "\n";
				afxDump << "\tInitial moles (m0) = " << kineticComp.m_dM0 << "\n";
				afxDump << "\tMoles (m) = " << kineticComp.m_dM << "\n";
				afxDump << "\tTolerance (tol) = " << kineticComp.m_dTol << "\n";

				afxDump << "\tPARM\n";
				std::list<double>::const_iterator iterDouble = kineticComp.m_listDParams.begin();
				for (; iterDouble != kineticComp.m_listDParams.end(); ++iterDouble)
				{
					afxDump << "\t\t" << *iterDouble << "\n";
				}

				afxDump << "\tFormula\n";
				std::list<CNameCoef>::const_iterator iterNameCoef = kineticComp.m_listNameCoef.begin();
				for (; iterNameCoef != kineticComp.m_listNameCoef.end(); ++iterNameCoef)
				{
					afxDump << "\t\t" << (*iterNameCoef).m_strName << ", " << (*iterNameCoef).m_dCoef << "\n";
				}
#endif
			}
		}
	}
	else
	{
		if (m_bFirstSetActive)
		{
			std::list<CKineticComp>::iterator compIter = m_listComps.begin();
			TCITEM tcItem;
			tcItem.mask = TCIF_PARAM;
			for (; compIter != m_listComps.end(); ++compIter)
			{
				CGridListItem glItem = m_gltDoc.AddItem(compIter->m_strRateName);
				tcItem.lParam = m_nextMapInt++;
				m_tcNames.SetItem(glItem.m_nTab, &tcItem);
				m_mapComps[tcItem.lParam] = (*compIter);
			}
			if (m_listComps.size() == 0)
			{
				DisplayTab(-1);
			}
			else
			{
				DisplayTab(0);
			}
		}
	}

	// validate  state
	ASSERT_VALID(this);
}

BEGIN_MESSAGE_MAP(CCKPKineticsPg1, baseCKPKineticsPg1)
	//{{AFX_MSG_MAP(CCKPKineticsPg1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TC_NAMES, OnSelchangeTcNames)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TC_NAMES, OnSelchangingTcNames)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_RATES, OnItemchangedClRates)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_RATES, OnSetfocusClRates)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// ON_WM_CONTEXTMENU()
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg2 property page

CCKPKineticsPg2::CCKPKineticsPg2() : baseCKPKineticsPg2(CCKPKineticsPg2::IDD)
{
	//{{AFX_DATA_INIT(CCKPKineticsPg2)
	//}}AFX_DATA_INIT
	m_nLinearSteps = 1;
	m_nAmountType = TYPE_LINEAR;
	m_dLinearAmt = 1.0;
}

CCKPKineticsPg2::~CCKPKineticsPg2()
{
}

void CCKPKineticsPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPKineticsPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPKineticsPg2)
	DDX_Control(pDX, IDC_SPIN_STEPS, m_spinSteps);
	DDX_Control(pDX, IDC_E_STEPS, m_eSteps);
	DDX_Control(pDX, IDC_E_TOTAL_TIME, m_eTotalTime);
	DDX_Control(pDX, IDC_MSHFG_LIST, m_ctrlStepEditGrid);
	//}}AFX_DATA_MAP
	DDX_Steps(pDX, IDC_MSHFG_LIST);

}


BEGIN_MESSAGE_MAP(CCKPKineticsPg2, baseCKPKineticsPg2)
	//{{AFX_MSG_MAP(CCKPKineticsPg2)
	ON_BN_CLICKED(IDC_RADIO_LINEAR, OnRadioLinear)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_EN_SETFOCUS(IDC_E_TOTAL_TIME, OnSetfocusETotalTime)
	ON_EN_SETFOCUS(IDC_E_STEPS, OnSetfocusESteps)
	ON_CBN_SETFOCUS(IDC_CB_RK, OnSetfocusCbRk)
	ON_EN_SETFOCUS(IDC_E_STEP_DIVIDE, OnSetfocusEStepDivide)
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


BOOL CCKPKineticsPg1::OnInitDialog() 
{
	baseCKPKineticsPg1::OnInitDialog();

	// Set Layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_S_DEF_RATES, ABSOLUTE_VERT)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_RATES, GREEDY) // RELATIVE_VERT, 0, 33)
			<< item(IDC_MSHFG_NAMES, ABSOLUTE_HORZ)
			)
		<< (paneTab(&m_tcNames, VERTICAL, GREEDY, nDefaultBorder, 4, 15)
			<< (pane(HORIZONTAL, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< (pane(VERTICAL, GREEDY)
						<< item(IDC_S_GENERAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_MSHFG_GENERAL, NORESIZE)
						<< item(IDC_S_USER, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_MSHFG_USER, GREEDY)
						)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< (pane(VERTICAL, GREEDY)
						<< item(IDC_S_FORMULA, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_MSHFG_FORMULA, GREEDY) // RELATIVE_VERT, 0, 67)
						)
					)
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

void CCKPKineticsPg1::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//{{
	ASSERT(FALSE); // message map commented out
	//}}

	// TODO: Add your message handler code here
	if (pWnd == &m_egFormula)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_RICH_IN));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);

		if (pPopup)
		{
			if (point.x == -1 && point.y == -1)
			{
				// keystroke invocation
				point.x = point.y = 0;
				ClientToScreen(&point);
			}

			// Note: All commands except copy are grayed by default
			// disable copy item if there's no selection
// COMMENT: {7/26/2000 3:28:57 PM}			if (GetCurSel() == LB_ERR)
// COMMENT: {7/26/2000 3:28:57 PM}			{
// COMMENT: {7/26/2000 3:28:57 PM}				pPopup->EnableMenuItem((UINT)ID_COPY, MF_BYCOMMAND | MF_GRAYED);
// COMMENT: {7/26/2000 3:28:57 PM}			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

			//{{COPY TO CLIPBOARD
			if (::OpenClipboard(m_egFormula.m_hWnd))
			{
				// Retrieve string from ListBox
				CString strText = m_egFormula.GetClip();

				// Allocate global memory
				HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_DDESHARE, (strText.GetLength() + 1) * sizeof(TCHAR)); 

				if (hglbCopy)
				{
					// Lock it and get a pointer to it
					LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy);

					if (lptstrCopy)
					{
						// Empty existing clipboard contents
						VERIFY(::EmptyClipboard());

						// Copy string into memory
						VERIFY(::lstrcpy(lptstrCopy, (LPCTSTR)strText));

						// Unlock memory
						VERIFY(::GlobalUnlock(hglbCopy));

						// Send memory to clipboard
#ifdef UNICODE
						VERIFY(::SetClipboardData(CF_UNICODETEXT, hglbCopy));
#else
						VERIFY(::SetClipboardData(CF_TEXT, hglbCopy));
#endif

					}
					else
					{
						// Free memory if it couldn't be locked
						::GlobalFree(hglbCopy);
					}

				}
				// Close the clipboard. 
				VERIFY(::CloseClipboard());
			}
			//}}COPY TO CLIPBOARD
		}
	}	
}

/**
Fills the grids within the tab control with the CKineticComp associated with nTabIndex

  nTabIndex == -1 signifies that no CKineticComps have been defined yet

*/
void CCKPKineticsPg1::DisplayTab(int nTabIndex)
{
	ASSERT_VALID(this);

	CKineticComp kineticComp;
	TCITEM tcItem;
	tcItem.pszText = NULL;

	if (nTabIndex >= 0)
	{
		TCHAR buffer[181];
		tcItem.pszText = buffer;
		tcItem.cchTextMax = 180;
		tcItem.mask = TCIF_PARAM | TCIF_TEXT;
		VERIFY(m_tcNames.GetItem(nTabIndex, &tcItem));

		// lookup CKineticComp by lParam
		kineticComp = m_mapComps[tcItem.lParam];
	}

	// General parameters:
	{
		// avoid flashing grid
		CDelayUpdate update(&m_egGeneral);

		m_egGeneral.SetFillStyle(flexFillRepeat);
		m_egGeneral.SetRow(0);
		m_egGeneral.SetCol(1);
		m_egGeneral.SetRowSel(m_egGeneral.GetRows() - 1);
		m_egGeneral.SetText(_T(""));
		m_egGeneral.SetFillStyle(flexFillSingle);

		CString str;
		str.Format(_T("%.*g"), DBL_DIG, kineticComp.m_dM0);
		m_egGeneral.SetTextMatrix(0, 1, str);
		str.Format(_T("%.*g"), DBL_DIG, kineticComp.m_dM);
		m_egGeneral.SetTextMatrix(1, 1, str);
		str.Format(_T("%.*g"), DBL_DIG, kineticComp.m_dTol);
		m_egGeneral.SetTextMatrix(2, 1, str);

		m_egGeneral.SetRow(0);
		m_egGeneral.SetCol(1);
	}

	// Formula:
	{
		// avoid flashing grid
		CDelayUpdate update(&m_egFormula);

		m_egFormula.SetFillStyle(flexFillRepeat);
		m_egFormula.SetRow(1);
		m_egFormula.SetCol(1);
		m_egFormula.SetRowSel(m_egFormula.GetRows() - 1);
		m_egFormula.SetColSel(m_egFormula.GetCols(0) - 1);
		m_egFormula.SetText(_T(""));
		m_egFormula.SetFillStyle(flexFillSingle);

		// add CNameCoef if this rate name is a predefined phase
		if (kineticComp.m_listNameCoef.size() == 0)
		{
			CDBPhase phase;
			phase.m_strName = tcItem.pszText ? tcItem.pszText : "";

			std::set<CDBPhase>::const_iterator phaseIter = GetDatabase().m_phaseSet.find(phase);
			if (phaseIter != GetDatabase().m_phaseSet.end())
			{
				CNameCoef nameCoef;
				nameCoef.m_strName = phaseIter->m_strFormula;
				nameCoef.m_dCoef = 1.0;
				kineticComp.m_listNameCoef.push_back(nameCoef);
			}
		}

		std::list<CNameCoef>::const_iterator iterNameCoef = kineticComp.m_listNameCoef.begin();
		for (long nRow = 1; iterNameCoef != kineticComp.m_listNameCoef.end(); ++iterNameCoef, ++nRow)
		{
			CString str;
			m_egFormula.SetTextMatrix(nRow, 1, iterNameCoef->m_strName);

			str.Format(_T("%.*g"), DBL_DIG, iterNameCoef->m_dCoef);
			m_egFormula.SetTextMatrix(nRow, 2, str);
		}
		m_egFormula.SetTopRow(1);
		m_egFormula.SetRow(1);
		m_egFormula.SetCol(1);
	}


	// User parameters:
	{
		// avoid flashing grid
		CDelayUpdate update(&m_egUser);

		m_egUser.SetFillStyle(flexFillRepeat);
		m_egUser.SetRow(0);
		m_egUser.SetCol(1);
		m_egUser.SetRowSel(m_egUser.GetRows() - 1);
		m_egUser.SetText(_T(""));
		m_egUser.SetFillStyle(flexFillSingle);

		std::list<double>::const_iterator iter = kineticComp.m_listDParams.begin();
		for (long nRow = 0; iter != kineticComp.m_listDParams.end(); ++iter, ++nRow)
		{
			CString str;
			str.Format(_T("%.*g"), DBL_DIG, *iter);
			m_egUser.SetTextMatrix(nRow, 1, str);
		}
		m_egUser.SetTopRow(0);
		m_egUser.SetRow(0);
		m_egUser.SetCol(1);
	}

	UpdateTabs();

	ASSERT_VALID(this);
}

void CCKPKineticsPg1::OnSelchangeTcNames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Note: OnSelchangingTcNames updates the rate
	// UNREFERENCED_PARAMETER(pNMHDR);
	DBG_UNREFERENCED_PARAMETER(pNMHDR);

	int nCurSel = m_tcNames.GetCurSel();
	ASSERT(nCurSel != -1 || m_tcNames.GetItemCount() == 0);

	DisplayTab(nCurSel);
	*pResult = 0;
}

void CCKPKineticsPg1::OnSelchangingTcNames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	ASSERT(m_tcNames.GetCurSel() != -1);

	// validate CKineticComp and disallow tab change if (UpdateData != TRUE)
	*pResult = !(UpdateData(TRUE));
}

// COMMENT: {12/12/2000 4:04:31 PM}BOOL CCKPKineticsPg1::GetComp(long nRowIndex, CKineticComp* pComp)const
// COMMENT: {12/12/2000 4:04:31 PM}{
// COMMENT: {12/12/2000 4:04:31 PM}	ASSERT(pComp != NULL);
// COMMENT: {12/12/2000 4:04:31 PM}	ASSERT_VALID(this);
// COMMENT: {12/12/2000 4:04:31 PM}	BOOL bItemFound = FALSE;
// COMMENT: {12/12/2000 4:04:31 PM}	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
// COMMENT: {12/12/2000 4:04:31 PM}	for (; itemIter != m_listItems.end(); ++itemIter)
// COMMENT: {12/12/2000 4:04:31 PM}	{
// COMMENT: {12/12/2000 4:04:31 PM}		if (itemIter->nRowIndex == nRowIndex)
// COMMENT: {12/12/2000 4:04:31 PM}		{
// COMMENT: {12/12/2000 4:04:31 PM}			// item found
// COMMENT: {12/12/2000 4:04:31 PM}			bItemFound = TRUE;
// COMMENT: {12/12/2000 4:04:31 PM}
// COMMENT: {12/12/2000 4:04:31 PM}			// private members
// COMMENT: {12/12/2000 4:04:31 PM}			// pComp->m_dInitialMoles = itemIter->kineticComp.m_dInitialMoles;
// COMMENT: {12/12/2000 4:04:31 PM}			// pComp->m_dMoles = itemIter->kineticComp.m_dMoles;
// COMMENT: {12/12/2000 4:04:31 PM}			// pComp->m_listCParams.assign(itemIter->kineticComp.m_listCParams.begin(), itemIter->kineticComp.m_listCParams.end());
// COMMENT: {12/12/2000 4:04:31 PM}
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_dM = itemIter->kineticComp.m_dM;
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_dM0 = itemIter->kineticComp.m_dM0;
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_dTol = itemIter->kineticComp.m_dTol;
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_listDParams.assign(itemIter->kineticComp.m_listDParams.begin(), itemIter->kineticComp.m_listDParams.end());
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_listNameCoef.assign(itemIter->kineticComp.m_listNameCoef.begin(), itemIter->kineticComp.m_listNameCoef.end());
// COMMENT: {12/12/2000 4:04:31 PM}			pComp->m_strRateName = itemIter->kineticComp.m_strRateName;
// COMMENT: {12/12/2000 4:04:31 PM}			break;
// COMMENT: {12/12/2000 4:04:31 PM}		}
// COMMENT: {12/12/2000 4:04:31 PM}	}
// COMMENT: {12/12/2000 4:04:31 PM}	ASSERT_VALID(this);
// COMMENT: {12/12/2000 4:04:31 PM}	return bItemFound;
// COMMENT: {12/12/2000 4:04:31 PM}}

// COMMENT: {12/12/2000 4:04:55 PM}CString CCKPKineticsPg1::GetRateName(long nRowIndex)const
// COMMENT: {12/12/2000 4:04:55 PM}{
// COMMENT: {12/12/2000 4:04:55 PM}	ASSERT_VALID(this);
// COMMENT: {12/12/2000 4:04:55 PM}	CString strName;
// COMMENT: {12/12/2000 4:04:55 PM}	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
// COMMENT: {12/12/2000 4:04:55 PM}	for (; itemIter != m_listItems.end(); ++itemIter)
// COMMENT: {12/12/2000 4:04:55 PM}	{
// COMMENT: {12/12/2000 4:04:55 PM}		if (itemIter->nRowIndex == nRowIndex)
// COMMENT: {12/12/2000 4:04:55 PM}		{
// COMMENT: {12/12/2000 4:04:55 PM}			strName = itemIter->kineticComp.m_strRateName;
// COMMENT: {12/12/2000 4:04:55 PM}			// item found
// COMMENT: {12/12/2000 4:04:55 PM}			break;
// COMMENT: {12/12/2000 4:04:55 PM}		}
// COMMENT: {12/12/2000 4:04:55 PM}	}
// COMMENT: {12/12/2000 4:04:55 PM}	ASSERT_VALID(this);
// COMMENT: {12/12/2000 4:04:55 PM}	return strName;
// COMMENT: {12/12/2000 4:04:55 PM}}

// COMMENT: {12/11/2000 5:32:21 PM}BOOL CCKPKineticsPg1::SetCompName(long nRowIndex, LPCTSTR lpszName)
// COMMENT: {12/11/2000 5:32:21 PM}{
// COMMENT: {12/11/2000 5:32:21 PM}	ASSERT_VALID(this);
// COMMENT: {12/11/2000 5:32:21 PM}	ASSERT(lpszName && lstrlen(lpszName));
// COMMENT: {12/11/2000 5:32:21 PM}	BOOL bItemFound = FALSE;
// COMMENT: {12/11/2000 5:32:21 PM}	std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
// COMMENT: {12/11/2000 5:32:21 PM}	for (; itemIter != m_listItems.end(); ++itemIter)
// COMMENT: {12/11/2000 5:32:21 PM}	{
// COMMENT: {12/11/2000 5:32:21 PM}		if (itemIter->nRowIndex == nRowIndex)
// COMMENT: {12/11/2000 5:32:21 PM}		{
// COMMENT: {12/11/2000 5:32:21 PM}			// item found
// COMMENT: {12/11/2000 5:32:21 PM}			bItemFound = TRUE;
// COMMENT: {12/11/2000 5:32:21 PM}
// COMMENT: {12/11/2000 5:32:21 PM}			// update list
// COMMENT: {12/11/2000 5:32:21 PM}			itemIter->kineticComp.m_strRateName = lpszName;
// COMMENT: {12/11/2000 5:32:21 PM}
// COMMENT: {12/11/2000 5:32:21 PM}			// update grid
// COMMENT: {12/11/2000 5:32:21 PM}			m_egNames.SetTextMatrix(itemIter->nRowIndex, 0, lpszName);
// COMMENT: {12/11/2000 5:32:21 PM}
// COMMENT: {12/11/2000 5:32:21 PM}			// update tab
// COMMENT: {12/11/2000 5:32:21 PM}			TCITEM item;
// COMMENT: {12/11/2000 5:32:21 PM}			item.mask = TCIF_TEXT;
// COMMENT: {12/11/2000 5:32:21 PM}			item.pszText = (LPTSTR)lpszName;
// COMMENT: {12/11/2000 5:32:21 PM}			m_tcNames.SetItem(itemIter->nTabIndex, &item);
// COMMENT: {12/11/2000 5:32:21 PM}
// COMMENT: {12/11/2000 5:32:21 PM}			break;
// COMMENT: {12/11/2000 5:32:21 PM}		}
// COMMENT: {12/11/2000 5:32:21 PM}	}
// COMMENT: {12/11/2000 5:32:21 PM}	ASSERT_VALID(this);
// COMMENT: {12/11/2000 5:32:21 PM}	return bItemFound;
// COMMENT: {12/11/2000 5:32:21 PM}}

BOOL CCKPKineticsPg2::OnInitDialog() 
{
	baseCKPKineticsPg2::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< (paneCtrl(IDC_GB_TYPE_OF_INPUT, VERTICAL, ABSOLUTE_HORZ, nDefaultBorder, 10, 10)
				<< item(IDC_RADIO_LINEAR, NORESIZE)
				<< itemFixed(VERTICAL, 40)
				<< item(IDC_RADIO_LIST, NORESIZE)
				)
			<< (pane(VERTICAL, GREEDY)
				<< (paneCtrl(IDC_GB_LINEAR, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
					<< itemFixed(HORIZONTAL, 10)
					<< (pane(VERTICAL, NORESIZE)
						<< item(IDC_STATIC_LINEAR1, NORESIZE)
						<< item(IDC_E_TOTAL_TIME, NORESIZE)
						)
					<< itemFixed(HORIZONTAL, 50)
					<< (pane(VERTICAL, NORESIZE)
						<< item(IDC_STATIC_LINEAR3, NORESIZE)
						<< (pane(HORIZONTAL, NORESIZE, 0, 0, 0)
							<< item(IDC_E_STEPS, NORESIZE)
							<< item(IDC_SPIN_STEPS, NORESIZE)
							)
						)
					<< itemGrowing(HORIZONTAL)
					)
				<< (paneCtrl(IDC_GB_STEPS, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_MSHFG_LIST, ABSOLUTE_HORZ)
					<< itemGrowing(HORIZONTAL)
					)
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

LRESULT CCKPKineticsPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit


	if (nID == IDC_MSHFG_NAMES)
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// set to rate list
			pInfo->item.bUseCombo = (CUtil::InsertRates(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

void CCKPKineticsPg1::OnItemchangedClRates(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		// do nothing if ignoring
		if (m_bIgnoreCheckChange) return;

		int cTabsBefore = m_tcNames.GetItemCount();

		m_gltDoc.OnItemChanged(pNMHDR, pResult);

		if (cTabsBefore > 0 && m_tcNames.GetItemCount() == 0)
		{
			// no tabs; previously one or more tabs
			DisplayTab(-1);
		}
		else if (cTabsBefore > 1 && m_tcNames.GetCurSel() == -1)
		{
			// current tab has been deleted
			m_tcNames.SetCurSel(0);
			DisplayTab(m_tcNames.GetCurSel());
		}
		else if (cTabsBefore == 0 && m_tcNames.GetItemCount() > 0)
		{
			// new tab added; previously no tabs
			m_tcNames.SetCurSel(0);
			DisplayTab(m_tcNames.GetCurSel());
		}
		else if (m_tcNames.GetCurSel() != -1)
		{
			if (m_tcNames.GetItemCount() == cTabsBefore)
			{
				// this should not occur
				ASSERT(FALSE);
			}
			else
			{
				// a tab was added or removed but the current selection has not changed
				// do nothing
			}
		}
		else
		{
			// unknown state
			ASSERT(FALSE);
		}
	}

}

BEGIN_EVENTSINK_MAP(CCKPKineticsPg1, baseCKPKineticsPg1)
    //{{AFX_EVENTSINK_MAP(CCKPKineticsPg1)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_NAMES, -602 /* KeyDown */, OnKeyDownMshfgNames, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_GENERAL, -602 /* KeyDown */, OnKeyDownMshfgGeneral, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_GENERAL, 71 /* EnterCell */, OnEnterCellMshfgGeneral, VTS_NONE)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_USER, 71 /* EnterCell */, OnEnterCellMshfgUser, VTS_NONE)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_USER, -602 /* KeyDown */, OnKeyDownMshfgUser, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_FORMULA, -602 /* KeyDown */, OnKeyDownMshfgFormula, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_FORMULA, 71 /* EnterCell */, OnEnterCellMshfgFormula, VTS_NONE)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPKineticsPg1, IDC_MSHFG_NAMES, 71 /* EnterCell */, OnEnterCellMshfgNames, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCKPKineticsPg1::OnKeyDownMshfgFormula(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		// just fill with empty strings
		m_egFormula.SetFillStyle(flexFillRepeat);
		m_egFormula.SetText(_T(""));
		m_egFormula.SetFillStyle(flexFillSingle);
		break;
	}	
}

void CCKPKineticsPg1::OnKeyDownMshfgGeneral(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		// just fill with empty strings
		m_egGeneral.SetFillStyle(flexFillRepeat);
		m_egGeneral.SetText(_T(""));
		m_egGeneral.SetFillStyle(flexFillSingle);
		break;
	}	
}

void CCKPKineticsPg1::OnKeyDownMshfgNames(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		{
			long nCol = m_egNames.GetCol();
			long nColSel = m_egNames.GetColSel();

			// check if associated col is included (m_nCol)
			if ((min(nCol, nColSel) <= 1) && (1 <= max(nCol, nColSel)))
			{
				long nRow = m_egNames.GetRow();
				long nRowSel = m_egNames.GetRowSel();

				// determine how many rows to delete
				long cSelectedRows = abs(nRow - nRowSel) + 1;

				// determine first row to delete
				long iStartRow = min(nRow, nRowSel);

				// delete each row
				for (long i = 0; i < cSelectedRows; ++i)
				{
					CString strRateName = m_egNames.GetTextMatrix(iStartRow, 1);
					if (!strRateName.IsEmpty())
					{
						CString strMsg;
						::AfxFormatString1(strMsg, IDS_REMOVE_COMP_346, strRateName);
						CGridListItem item;
						switch (AfxMessageBox(strMsg, MB_YESNOCANCEL))
						{
						case IDYES :
							{
								int nCurSelTab = m_tcNames.GetCurSel();

								// don't allow OnItemChanged to handle
								ASSERT(m_bIgnoreCheckChange == FALSE);
								m_bIgnoreCheckChange = TRUE;

								item = m_gltDoc.DeleteRow(iStartRow);

								ASSERT(m_bIgnoreCheckChange == TRUE);
								m_bIgnoreCheckChange = FALSE;

								if (nCurSelTab == item.m_nTab)
								{
									// update tabs only if current tab is removed
									m_tcNames.SetCurSel(0);
									DisplayTab(m_tcNames.GetCurSel());
								}
							}
							break;
						case IDNO :
							iStartRow++;
							break;
						case IDCANCEL :
							i = cSelectedRows; // break for loop
							break;
						}
					}
				}
			}
		}
		break;
	}
}

void CCKPKineticsPg1::OnKeyDownMshfgUser(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		// just fill with empty strings
		m_egUser.SetFillStyle(flexFillRepeat);
		m_egUser.SetText(_T(""));
		m_egUser.SetFillStyle(flexFillSingle);
		break;
	}	
}

LRESULT CCKPKineticsPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	CString strRateName;

	switch ( nID )
	{
	case IDC_MSHFG_NAMES :
		{
			// replace spaces with underscores
			strRateName = pInfo->item.pszText;
			if (strRateName.Replace(_T(" "), _T("_")))
			{
				// see CEditGrid::EndEdit
				free(pInfo->item.pszText);
				pInfo->item.pszText = _tcsdup(strRateName);
			}

			int cTabsBefore = m_tcNames.GetItemCount();

			ASSERT(m_bIgnoreCheckChange == FALSE);
			m_bIgnoreCheckChange = TRUE;

			bMakeChange = m_gltDoc.OnEndCellEdit(wParam, lParam);

			ASSERT(m_bIgnoreCheckChange == TRUE);
			m_bIgnoreCheckChange = FALSE;

			if (cTabsBefore > 0 && m_tcNames.GetItemCount() == 0)
			{
				// no tabs; previously one or more tabs
				DisplayTab(-1);
			}
			else if (cTabsBefore > 1 && m_tcNames.GetCurSel() == -1)
			{
				// current tab has been deleted
				m_tcNames.SetCurSel(0);
				DisplayTab(m_tcNames.GetCurSel());
			}
			else if (cTabsBefore == 0 && m_tcNames.GetItemCount() > 0)
			{
				// new tab added; previously no tabs
				m_tcNames.SetCurSel(0);
				DisplayTab(m_tcNames.GetCurSel());
			}
			else if (m_tcNames.GetCurSel() != -1)
			{
				ASSERT(m_tcNames.GetItemCount() > 0);
			}
			else
			{
				// no tabs before or after
				ASSERT(cTabsBefore == 0 && m_tcNames.GetItemCount() == 0);
			}
		}
		break;

	case IDC_MSHFG_GENERAL :
		// nothing to do
		break;

	case IDC_MSHFG_USER :
		// nothing to do
		break;

	case IDC_MSHFG_FORMULA :
		// nothing to do
		break;

	case IDC_MSHFG_NUM_DESC :
		// nothing to do
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

LRESULT CCKPKineticsPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_NAMES :
		OnEnterCellMshfgNames();
		break;
	case IDC_MSHFG_GENERAL :
		OnEnterCellMshfgGeneral();
		break;
	case IDC_MSHFG_FORMULA :
		OnEnterCellMshfgFormula();
		break;
	case IDC_MSHFG_USER :
		OnEnterCellMshfgUser();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CCKPKineticsPg2::OnRadioLinear() 
{
	OnSetfocusRadioLinear();
	UpdateRadioState();
}

void CCKPKineticsPg2::OnRadioList() 
{
	OnSetfocusRadioList();
	UpdateRadioState();
}

void CCKPKineticsPg2::UpdateRadioState()
{
	CButton* pLinearBtn = (CButton*)GetDlgItem(IDC_RADIO_LINEAR);
	CButton* pListBtn = (CButton*)GetDlgItem(IDC_RADIO_LIST);

	switch (pLinearBtn->GetCheck())
	{
	case BST_CHECKED :
		ASSERT(pListBtn->GetCheck() == BST_UNCHECKED);

		m_nAmountType = TYPE_LINEAR;
		EnableLinear(TRUE);
		EnableList(FALSE);	

		break;
	case BST_UNCHECKED :
		ASSERT(pListBtn->GetCheck() == BST_CHECKED);

		m_nAmountType = TYPE_LIST;
		EnableLinear(FALSE);
		EnableList(TRUE);

		break;
	default :
		ASSERT(FALSE);
		break;
	}
}

void CCKPKineticsPg2::EnableLinear(BOOL bEnable) 
{
	CWnd* pWnd;

	// IDC_GB_LINEAR
	pWnd = GetDlgItem(IDC_GB_LINEAR);
	pWnd->EnableWindow(bEnable);

	// IDC_STATIC_LINEAR1
	pWnd = GetDlgItem(IDC_STATIC_LINEAR1);
	pWnd->EnableWindow(bEnable);

	// IDC_E_RXN_AMT
	m_eTotalTime.EnableWindow(bEnable);

	// IDC_STATIC_LINEAR2
// COMMENT: {11/28/2000 12:09:01 PM}	pWnd = GetDlgItem(IDC_STATIC_LINEAR2);
// COMMENT: {11/28/2000 12:09:01 PM}	pWnd->EnableWindow(bEnable);


	// IDC_STATIC_LINEAR3
	pWnd = GetDlgItem(IDC_STATIC_LINEAR3);
	pWnd->EnableWindow(bEnable);

	// IDC_E_STEPS
	m_eSteps.EnableWindow(bEnable);

	// IDC_SPIN_STEPS
	m_spinSteps.EnableWindow(bEnable);
}

void CCKPKineticsPg2::EnableList(BOOL bEnable) 
{
	CWnd* pWnd;

	// IDC_GB_STEPS
	pWnd = GetDlgItem(IDC_GB_STEPS);
	pWnd->EnableWindow(bEnable);

	// IDC_MSHFG_LIST
	m_ctrlStepEditGrid.EnableWindow(bEnable);

// COMMENT: {11/28/2000 12:11:09 PM}	// IDC_STATIC_STEPS1
// COMMENT: {11/28/2000 12:11:09 PM}	pWnd = GetDlgItem(IDC_STATIC_STEPS1);
// COMMENT: {11/28/2000 12:11:09 PM}	pWnd->EnableWindow(bEnable);
}

void CCKPKineticsPg2::OnSetfocusRadioLinear()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING509);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg2::OnSetfocusRadioList()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING510);
	m_eInputDesc.SetWindowText(strRes);
}

// COMMENT: {4/29/2004 7:29:49 PM}void CCKPKineticsPg2::InitCombos()
// COMMENT: {4/29/2004 7:29:49 PM}{
// COMMENT: {4/29/2004 7:29:49 PM}	ASSERT(m_cboRKOpt.GetCount() == 0);
// COMMENT: {4/29/2004 7:29:49 PM}	// fill RK option combo
// COMMENT: {4/29/2004 7:29:49 PM}	CString strRes;
// COMMENT: {4/29/2004 7:29:49 PM}	VERIFY( strRes.LoadString(IDS_RK_OPTS_347) );
// COMMENT: {4/29/2004 7:29:49 PM}	LPTSTR lpszOpt;
// COMMENT: {4/29/2004 7:29:49 PM}	lpszOpt = _tcstok(strRes.LockBuffer(), _T(";"));
// COMMENT: {4/29/2004 7:29:49 PM}	while (lpszOpt)
// COMMENT: {4/29/2004 7:29:49 PM}	{
// COMMENT: {4/29/2004 7:29:49 PM}		m_cboRKOpt.AddString(lpszOpt);
// COMMENT: {4/29/2004 7:29:49 PM}		lpszOpt = _tcstok(NULL, _T(";"));
// COMMENT: {4/29/2004 7:29:49 PM}	}
// COMMENT: {4/29/2004 7:29:49 PM}	strRes.UnlockBuffer();	
// COMMENT: {4/29/2004 7:29:49 PM}	ASSERT(m_cboRKOpt.GetCount() == 4);   // 1, 2, 3, 6 (this may change)
// COMMENT: {4/29/2004 7:29:49 PM}}

void CCKPKineticsPg2::InitSpin()
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

void CCKPKineticsPg2::InitStepEditGrid()
{
	// set column titles
	m_ctrlStepEditGrid.SetTextMatrix(0, 1, _T("Time step (sec)"));

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

void CCKPKineticsPg2::DDX_Steps(CDataExchange* pDX, int nIDC)
{
	if (m_bFirstSetActive)
	{
		InitSpin();
		InitStepEditGrid();
	}

	CButton* pLinearBtn = (CButton*)GetDlgItem(IDC_RADIO_LINEAR);
	CButton* pListBtn = (CButton*)GetDlgItem(IDC_RADIO_LIST);

	if (pDX->m_bSaveAndValidate)
	{
		// Type of input
		switch (pLinearBtn->GetCheck())
		{
		case BST_CHECKED :
			ASSERT(pListBtn->GetCheck() == BST_UNCHECKED);
			m_nAmountType = TYPE_LINEAR;

			break;

		case BST_UNCHECKED :
			ASSERT(pListBtn->GetCheck() == BST_CHECKED);
			m_nAmountType = TYPE_LIST;

			break;

		default :
			ASSERT(FALSE);
			break;
		}

		if (m_nAmountType == TYPE_LINEAR)
		{
			// amount
			DDX_Text(pDX, IDC_E_TOTAL_TIME, m_dLinearAmt);

			// steps
			DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);
			ASSERT(m_nLinearSteps > 0 && m_nLinearSteps <= INT_MAX);
		}
		else
		{
			ASSERT(m_nAmountType == TYPE_LIST);

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
// COMMENT: {11/28/2000 12:27:39 PM}		DDX_CBIndex(pDX, IDC_CBO_LIST_UNITS, (int&)m_nUnits);
// COMMENT: {11/28/2000 12:27:39 PM}		DDX_CBIndex(pDX, IDC_CBO_LINEAR_UNITS, (int&)m_nUnits);

		// exchange steps no mater what type
		ASSERT(0 < m_nLinearSteps && m_nLinearSteps <= INT_MAX);
		DDX_Text(pDX, IDC_E_STEPS, m_nLinearSteps);

		// set radios
		if (m_nAmountType == TYPE_LINEAR)
		{
			pLinearBtn->SetCheck(BST_CHECKED);

			// amount
			DDX_Text(pDX, IDC_E_TOTAL_TIME, m_dLinearAmt);
		}
		else if (m_nAmountType == TYPE_LIST)
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
			ASSERT(FALSE); // invalid m_nAmountType
		}

		// update radio controls
		UpdateRadioState();
	}
}

void CCKPKineticsPg2::OnSetfocusETotalTime() 
{
	CString strRes;
	VERIFY( strRes.LoadString(IDS_KINETICS_348) );
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg2::OnSetfocusESteps() 
{
	CString strRes;
	VERIFY( strRes.LoadString(IDS_KINETICS_349) );
	m_eInputDesc.SetWindowText(strRes);
}

BEGIN_EVENTSINK_MAP(CCKPKineticsPg2, baseCKPKineticsPg2)
    //{{AFX_EVENTSINK_MAP(CCKPKineticsPg2)
	ON_EVENT(CCKPKineticsPg2, IDC_MSHFG_LIST, 71 /* EnterCell */, OnEnterCellMshfgList, VTS_NONE)
	ON_EVENT(CCKPKineticsPg2, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPKineticsPg2, IDC_MSHFG_LIST, -602 /* KeyDown */, OnKeyDownMshfgList, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCKPKineticsPg2::OnEnterCellMshfgList() 
{
	CString strRes;
	VERIFY( strRes.LoadString(IDS_KINETICS_350) );
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg2::OnSetfocusCbRk() 
{
	CString strRes;
	VERIFY( strRes.LoadString(IDS_KINETICS_351) );
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg2::OnSetfocusEStepDivide() 
{
	CString strRes, strRes2;
	VERIFY( strRes.LoadString(IDS_KINETICS_352) );
	VERIFY( strRes2.LoadString(IDS_KINETICS_354) );
	m_eInputDesc.SetWindowText(strRes + strRes2);
}

void CCKPKineticsPg1::UpdateTabs()
{
	UpdateLayout();
	CWnd* pWnd = NULL;
	BOOL bEnable = (m_tcNames.GetItemCount() != 0);

	m_egFormula.EnableWindow(bEnable);
	m_egGeneral.EnableWindow(bEnable);
	m_egUser.EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_S_GENERAL);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_S_USER);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_S_FORMULA);
	pWnd->EnableWindow(bEnable);
}

void CCKPKineticsPg1::InitGrids()
{
	CRect rect;
	CDC* pDC = GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);


	//{{ Rate names
	m_egNames.AddRowHeaders();
	long nCol0 = m_egNames.GetColWidth(0, 0);
	m_egNames.GetClientRect(&rect);
	m_egNames.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0);
	m_egNames.SetColAlignmentFixed(1, flexAlignCenterCenter);
	m_egNames.SetTextMatrix(0, 1, _T("Rate name"));
	//}} Rate names


	//{{General Parameters
	m_egGeneral.SetColWidth(0, 0, 1400);
	m_egGeneral.GetClientRect(&rect);
	m_egGeneral.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - 1400);

    m_egGeneral.SetTextMatrix(0, 0, _T("Initial moles (m0)"));
    m_egGeneral.SetTextMatrix(1, 0, _T("Moles (m)"));
    m_egGeneral.SetTextMatrix(2, 0, _T("Tolerance (tol)"));
	//}}General Parameters


	//{{ Formula:
	m_egFormula.AddRowHeaders();
	nCol0 = m_egFormula.GetColWidth(0, 0);
	m_egFormula.SetColWidth(1, 0, 1050);
	m_egFormula.GetClientRect(&rect);
	m_egFormula.SetColWidth(2, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0 - 1050);

	// set row heights
	m_egFormula.SetWordWrap(TRUE);
	m_egFormula.SetRowHeight(0, (long) 2.5 * m_egFormula.GetRowHeight(0));

	m_egFormula.SetColAlignmentFixed(1, flexAlignCenterCenter);
	m_egFormula.SetColAlignmentFixed(2, flexAlignCenterCenter);

    m_egFormula.SetTextMatrix(0, 1, _T("Formula or\r\nphase name"));
    m_egFormula.SetTextMatrix(0, 2, _T("Stoich.\r\ncoef."));
	//}} Formula:

	//{{ User parameters
	m_egUser.SetColAlignmentFixed(0, flexAlignCenterCenter);
	for (long nRow = m_egUser.GetFixedRows(); nRow < m_egUser.GetRows(); ++nRow)
	{
		CString str;
		str.Format(_T("PARM(%d)"), nRow + 1);
		m_egUser.SetTextMatrix(nRow, 0, str);
	}

	m_egUser.SetColWidth(1, 0, 500);
	nCol0 = m_egUser.GetColWidth(0, 0);
	m_egUser.SetColWidth(1, 0, 1050);
	m_egUser.GetClientRect(&rect);
	m_egUser.SetColWidth(2, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0 - 1050);
	//}} User parameters
}

void CCKPKineticsPg1::OnEnterCellMshfgGeneral() 
{
	CString strRes;
	switch (m_egGeneral.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING503);
		break;
	case 1 :
		strRes.LoadString(IDS_STRING504);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING505);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1::OnEnterCellMshfgNames() 
{
	CString strRes;
	switch (m_egNames.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_STRING502);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1::OnEnterCellMshfgUser() 
{
	CString strRes;
	switch (m_egUser.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_STRING508);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}



void CCKPKineticsPg1::OnEnterCellMshfgFormula() 
{
	CString strRes;
	switch (m_egFormula.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_STRING506);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING507);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING500);
		break;
	case 1 :
		strRes.LoadString(IDS_STRING501);
		break;
	case 2 :
		// Optional comment that describes the %1.
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1::OnSetfocusClRates(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_clRates.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;
	
	strRes.LoadString(IDS_KINETICS_353);

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clRates.GetFirstSelectedItemPosition() == NULL)
	{
		m_clRates.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CCKPKineticsPg2::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING500);
		break;
	case 1 :
		strRes.LoadString(IDS_STRING501);
		break;
	case 2 :
		// Optional comment that describes the %1.
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg2::OnKeyDownMshfgList(short FAR* KeyCode, short Shift) 
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		// just fill with empty strings
		m_ctrlStepEditGrid.SetFillStyle(flexFillRepeat);
		m_ctrlStepEditGrid.SetText(_T(""));
		m_ctrlStepEditGrid.SetFillStyle(flexFillSingle);
		break;
	}	
}

LRESULT CCKPKineticsPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UNREFERENCED_PARAMETER(pInfo);

	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bDenyEdit = FALSE;	// allow edit

	switch (nID)
	{
	case IDC_MSHFG_LIST :
		// allow edit
		break;

	case IDC_MSHFG_NUM_DESC:
		// allow edit
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bDenyEdit;
}

LRESULT CCKPKineticsPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_LIST :
		// nothing to do
		break;
	case IDC_MSHFG_NUM_DESC :
		// nothing to do
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

LRESULT CCKPKineticsPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_LIST :
		OnEnterCellMshfgList();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}


BOOL CCKPKineticsPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
		case 0 :
			strRes.LoadString(IDS_STRING500);
			break;
		case 1 :
			strRes.LoadString(IDS_STRING501);
			break;
		case 2 :
			// Optional comment that describes the %1.
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
			break;
		}
		break;
	case IDC_MSHFG_NAMES :
		//{{NEW
		if (!IsContextHelp())
		{
			if (!(m_egNames.GetRowIsVisible(m_egNames.GetRow()) && m_egNames.GetColIsVisible(m_egNames.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNames.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNames.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNames.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNames.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNames.GetMouseCol() : m_egNames.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_STRING502);
			break;
		}
		break;
	case IDC_MSHFG_GENERAL :
		//{{NEW
		if (!IsContextHelp())
		{
			if (!(m_egGeneral.GetRowIsVisible(m_egGeneral.GetRow()) && m_egGeneral.GetColIsVisible(m_egGeneral.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egGeneral.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egGeneral.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egGeneral.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egGeneral.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egGeneral.GetMouseRow() : m_egGeneral.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING503);
			break;
		case 1 :
			strRes.LoadString(IDS_STRING504);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING505);
			break;
		}
		break;
	case IDC_MSHFG_FORMULA :
		//{{NEW
		if (!IsContextHelp())
		{
			if (!(m_egFormula.GetRowIsVisible(m_egFormula.GetRow()) && m_egFormula.GetColIsVisible(m_egFormula.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egFormula.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egFormula.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egFormula.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egFormula.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egFormula.GetMouseCol() : m_egFormula.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_STRING506);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING507);
			break;
		}
		break;
	case IDC_MSHFG_USER :
		if (!IsContextHelp())
		{
			if (!(m_egUser.GetRowIsVisible(m_egUser.GetRow()) && m_egUser.GetColIsVisible(m_egUser.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egUser.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egUser.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egUser.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egUser.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egUser.GetMouseCol() : m_egUser.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_STRING508);
			break;
		}
		break;
	case IDC_CL_RATES:
		strRes.LoadString(IDS_KINETICS_353);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CCKPKineticsPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
		strRes.LoadString(IDS_STRING510);
		break;
	case IDC_RADIO_LINEAR:
		strRes.LoadString(IDS_STRING509);
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
		case 0 :
			strRes.LoadString(IDS_STRING500);
			break;
		case 1 :
			strRes.LoadString(IDS_STRING501);
			break;
		case 2 :
			// Optional comment that describes the %1.
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
			break;
		}
		break;
	case IDC_MSHFG_LIST :
		strRes.LoadString(IDS_KINETICS_350);
		break;
	case IDC_E_TOTAL_TIME: case IDC_STATIC_LINEAR1:
		strRes.LoadString(IDS_KINETICS_348);
		break;
	case IDC_E_STEPS: case IDC_STATIC_LINEAR3: case IDC_SPIN_STEPS:
		strRes.LoadString(IDS_KINETICS_349);
		break;
	case IDC_E_STEP_DIVIDE: case IDC_S_STEP_DIVIDE:
		strRes.LoadString(IDS_KINETICS_352);
		strRes += CString((LPCSTR)IDS_KINETICS_354);
		break;
	case IDC_CB_RK: case IDC_S_RUNGE:
		strRes.LoadString(IDS_KINETICS_351);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg1A property page

IMPLEMENT_DYNCREATE(CCKPKineticsPg1A, baseCKPKineticsPg1A)

CCKPKineticsPg1A::CCKPKineticsPg1A() : baseCKPKineticsPg1A(CCKPKineticsPg1A::IDD)
	, m_nRKType(CCKPKineticsPg1A::RK_3)
	, m_nODEMethodType(CCKPKineticsPg1A::ODE_RUNGA_KUTTA)
{
	//{{AFX_DATA_INIT(CCKPKineticsPg1A)
	m_dStepDivide    = 1.0;
	m_nRKMaxBadSteps = 200;
	m_nCVMaxBadSteps = 200;
	//}}AFX_DATA_INIT
}

CCKPKineticsPg1A::~CCKPKineticsPg1A()
{
}

void CCKPKineticsPg1A::DoDataExchange(CDataExchange* pDX)
{
	baseCKPKineticsPg1A::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPKineticsPg1A)
	DDX_Control(pDX, IDC_CB_RK, m_cboRKOpt);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		InitCombos();
	}
	ASSERT(m_cboRKOpt.GetCount() == 4);   // 1, 2, 3, 6 (this may change)
	ASSERT(m_nRKType >= RK_1);
	ASSERT(m_nRKType <= RK_6);
	DDX_CBIndex(pDX, IDC_CB_RK, (int&)m_nRKType);

	if (pDX->m_bSaveAndValidate)
	{
		switch (this->GetCheckedRadioButton(IDC_RUNGE_RADIO, IDC_CVODE_RADIO))
		{
		case IDC_RUNGE_RADIO:
			m_nODEMethodType = CCKPKineticsPg1A::ODE_RUNGA_KUTTA;
			DDX_Text(pDX, IDC_E_STEP_DIVIDE, m_dStepDivide);
			DDX_Text(pDX, IDC_E_RUNGE_MAX_BAD, m_nRKMaxBadSteps);
			break;
		case IDC_CVODE_RADIO:
			m_nODEMethodType = CCKPKineticsPg1A::ODE_CVODE;
			DDX_Text(pDX, IDC_E_CVODE_MAX_BAD, m_nCVMaxBadSteps);
			break;
		default:
			ASSERT(FALSE);
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_STEP_DIVIDE, m_dStepDivide);
		DDX_Text(pDX, IDC_E_RUNGE_MAX_BAD, m_nRKMaxBadSteps);
		DDX_Text(pDX, IDC_E_CVODE_MAX_BAD, m_nCVMaxBadSteps);
		switch (this->m_nODEMethodType)
		{
		case CCKPKineticsPg1A::ODE_RUNGA_KUTTA:
			this->CheckRadioButton(IDC_RUNGE_RADIO, IDC_CVODE_RADIO, IDC_RUNGE_RADIO);
			break;

		case CCKPKineticsPg1A::ODE_CVODE:
			this->CheckRadioButton(IDC_RUNGE_RADIO, IDC_CVODE_RADIO, IDC_CVODE_RADIO);
			break;

		default:
			ASSERT(FALSE);
			this->CheckRadioButton(IDC_RUNGE_RADIO, IDC_CVODE_RADIO, IDC_RUNGE_RADIO);
		}

		// update radio controls
		this->UpdateRadioState();
	}
}


BEGIN_MESSAGE_MAP(CCKPKineticsPg1A, baseCKPKineticsPg1A)
	//{{AFX_MSG_MAP(CCKPKineticsPg1A)
	ON_CBN_SETFOCUS(IDC_CB_RK, OnSetfocusCbRk)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_RUNGE_RADIO, OnRungeRadio)
	ON_BN_CLICKED(IDC_CVODE_RADIO, OnCvodeRadio)
	ON_EN_SETFOCUS(IDC_E_STEP_DIVIDE, OnSetfocusEStepDivide)
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	// custom radio button set focus notifications
	ON_BN_SETFOCUS(IDC_RUNGE_RADIO, OnSetfocusRungeRadio)
	ON_BN_SETFOCUS(IDC_CVODE_RADIO, OnSetfocusCvodeRadio)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCKPKineticsPg1A, baseCKPKineticsPg1A)
    //{{AFX_EVENTSINK_MAP(CCKPKineticsPg1A)
	ON_EVENT(CCKPKineticsPg1A, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg1A message handlers

BOOL CCKPKineticsPg1A::OnInitDialog() 
{
	baseCKPKineticsPg1A::OnInitDialog();

	// IDC_S_RUNGE_MAX_BAD IDC_E_RUNGE_MAX_BAD
	// IDC_S_CVODE_MAX_BAD IDC_E_CVODE_MAX_BAD
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (paneCtrl(IDC_GB_STEP_OPTIONS, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_RUNGE_RADIO, NORESIZE|ALIGN_CENTER)
					)
				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemFixed(HORIZONTAL, 30)
					<< item(IDC_S_RUNGE, NORESIZE|ALIGN_CENTER)
					<< item(IDC_CB_RK, NORESIZE|ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 30)
					<< item(IDC_S_STEP_DIVIDE, NORESIZE|ALIGN_CENTER)
					<< item(IDC_E_STEP_DIVIDE, NORESIZE|ALIGN_CENTER)
					)
				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemFixed(HORIZONTAL, 30)
					<< item(IDC_S_RUNGE_MAX_BAD, NORESIZE|ALIGN_CENTER)
					<< item(IDC_E_RUNGE_MAX_BAD, NORESIZE|ALIGN_CENTER)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CVODE_RADIO, NORESIZE|ALIGN_CENTER)
					)
				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemFixed(HORIZONTAL, 30)
					<< item(IDC_S_CVODE_MAX_BAD, NORESIZE|ALIGN_CENTER)
					<< item(IDC_E_CVODE_MAX_BAD, NORESIZE|ALIGN_CENTER)
					)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CCKPKineticsPg1A::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CCKPKineticsPg1A::OnSetfocusCbRk() 
{
	CString strRes;
	VERIFY( strRes.LoadString(IDS_KINETICS_351) );
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1A::OnSetfocusCvodeRadio()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING618);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1A::OnSetfocusRungeRadio()
{
	// in order for a radio button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING619);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPKineticsPg1A::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING500);
		break;
	case 1 :
		strRes.LoadString(IDS_STRING501);
		break;
	case 2 :
		// Optional comment that describes the %1.
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CCKPKineticsPg1A::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		// nothing to do
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

BOOL CCKPKineticsPg1A::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_RUNGE_RADIO:
		strRes.LoadString(IDS_STRING617);
		break;
	case IDC_CVODE_RADIO:
		strRes.LoadString(IDS_STRING616);
		break;

// COMMENT: {4/29/2004 7:16:39 PM}	case IDC_RADIO_LIST:
// COMMENT: {4/29/2004 7:16:39 PM}		strRes.LoadString(IDS_STRING510);
// COMMENT: {4/29/2004 7:16:39 PM}		break;
// COMMENT: {4/29/2004 7:16:39 PM}	case IDC_RADIO_LINEAR:
// COMMENT: {4/29/2004 7:16:39 PM}		strRes.LoadString(IDS_STRING509);
// COMMENT: {4/29/2004 7:16:39 PM}		break;
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
		case 0 :
			strRes.LoadString(IDS_STRING500);
			break;
		case 1 :
			strRes.LoadString(IDS_STRING501);
			break;
		case 2 :
			// Optional comment that describes the %1.
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("kinetic reactions") ); 
			break;
		}
		break;
// COMMENT: {4/29/2004 7:17:05 PM}	case IDC_MSHFG_LIST :
// COMMENT: {4/29/2004 7:17:05 PM}		strRes.LoadString(IDS_KINETICS_350);
// COMMENT: {4/29/2004 7:17:05 PM}		break;
// COMMENT: {4/29/2004 7:17:05 PM}	case IDC_E_TOTAL_TIME: case IDC_STATIC_LINEAR1:
// COMMENT: {4/29/2004 7:17:05 PM}		strRes.LoadString(IDS_KINETICS_348);
// COMMENT: {4/29/2004 7:17:05 PM}		break;
	case IDC_E_STEPS: case IDC_STATIC_LINEAR3: case IDC_SPIN_STEPS:
		strRes.LoadString(IDS_KINETICS_349);
		break;
	case IDC_E_STEP_DIVIDE: case IDC_S_STEP_DIVIDE:
		strRes.LoadString(IDS_KINETICS_352);
		strRes += CString((LPCSTR)IDS_KINETICS_354);
		break;
	case IDC_CB_RK: case IDC_S_RUNGE:
		strRes.LoadString(IDS_KINETICS_351);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CCKPKineticsPg1A::InitCombos()
{
	ASSERT(m_cboRKOpt.GetCount() == 0);
	// fill RK option combo
	CString strRes;
	VERIFY( strRes.LoadString(IDS_RK_OPTS_347) );
	LPTSTR lpszOpt;
	lpszOpt = _tcstok(strRes.LockBuffer(), _T(";"));
	while (lpszOpt)
	{
		m_cboRKOpt.AddString(lpszOpt);
		lpszOpt = _tcstok(NULL, _T(";"));
	}
	strRes.UnlockBuffer();	
	ASSERT(m_cboRKOpt.GetCount() == 4);   // 1, 2, 3, 6 (this may change)
}

void CCKPKineticsPg1A::OnRungeRadio() 
{
	OnSetfocusRungeRadio();
	UpdateRadioState();
}

void CCKPKineticsPg1A::OnCvodeRadio() 
{
	OnSetfocusCvodeRadio();
	UpdateRadioState();
}

void CCKPKineticsPg1A::UpdateRadioState()
{
	BOOL bEnableRunge = (IDC_RUNGE_RADIO == this->GetCheckedRadioButton(IDC_RUNGE_RADIO, IDC_CVODE_RADIO));
	if (CWnd* pWnd = this->GetDlgItem(IDC_S_RUNGE)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_CB_RK)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_S_STEP_DIVIDE)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_E_STEP_DIVIDE)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_E_STEP_DIVIDE)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_S_RUNGE_MAX_BAD)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_E_RUNGE_MAX_BAD)) {
		pWnd->EnableWindow(bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_S_CVODE_MAX_BAD)) {
		pWnd->EnableWindow(!bEnableRunge);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_E_CVODE_MAX_BAD)) {
		pWnd->EnableWindow(!bEnableRunge);
	}

}

void CCKPKineticsPg1A::OnSetfocusEStepDivide() 
{
	CString strRes, strRes2;
	VERIFY( strRes.LoadString(IDS_KINETICS_352) );
	VERIFY( strRes2.LoadString(IDS_KINETICS_354) );
	m_eInputDesc.SetWindowText(strRes + strRes2);
}
