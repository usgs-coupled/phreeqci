// KPRatesPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPRatesPg1.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "BasicObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPRatesPg1, baseKPRatesPg1)

//#define IDC_LB_ARGS IDC_TREE_ARGS

/////////////////////////////////////////////////////////////////////////////
// CKPRatesPg1 property page

CKPRatesPg1::CKPRatesPg1() : baseKPRatesPg1(CKPRatesPg1::IDD)
, m_basicDesc(GetDatabase(), IDC_LB_FUNCS, IDC_E_EXPLAN, IDC_TREE_ARGS)
{
	m_bRenumbering = false;
	//{{AFX_DATA_INIT(CKPRatesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPRatesPg1::~CKPRatesPg1()
{
}

void CKPRatesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPRatesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPRatesPg1)
	DDX_Control(pDX, IDC_TC_NAMES, m_tcNames);
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFG_NAMES, m_egNames);
	DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);
	//}}AFX_DATA_MAP
	m_basicDesc.DoDataExchange(pDX);
	DDX_RateList(pDX);
}

void CKPRatesPg1::DDX_RateList(CDataExchange *pDX)
{
	// validate  state
	ASSERT_VALID(this);

	// avoid flashing grid
	CDelayUpdate update(this, IDC_MSHFG_BASIC);

	if (pDX->m_bSaveAndValidate)
	{
		// determine which XGridTabItem is being validated
		int nCurSel = m_tcNames.GetCurSel();
		if (nCurSel == -1)
		{
			// no rates defined
			ASSERT(m_tcNames.GetItemCount() == 0);
			ASSERT(m_listItems.empty());

			// remove any current rates
			m_listRates.clear();
		}
		else
		{
			// rates have been defined
			ASSERT(m_tcNames.GetItemCount() != 0);
			ASSERT(!m_listItems.empty());

			// rate name already validated by OnEndCellEdit

			std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
			std::list<XGridTabItem>::iterator foundItem = m_listItems.end();

			for (; itemIter != foundItem; ++itemIter)
			{
				if (itemIter->nTabIndex == nCurSel)
				{
					foundItem = itemIter;
					break;
				}
			}

			ASSERT(foundItem != m_listItems.end()); // must be in the list

			// create object to check basic
			CBasicObj basicCheck;

			CRate updatedRate;
			updatedRate.m_strName = foundItem->rate.m_strName;

			// save view
			long nRowSave = m_egBasic.GetRow();
			long nTopRowSave = m_egBasic.GetTopRow();

			for (long nRow = m_egBasic.GetFixedRows(); nRow < m_egBasic.GetRows(); ++nRow)
			{
				basic_command command;
				basicCheck.DDX_BasicCommand(pDX, IDC_MSHFG_BASIC, nRow, 0, command);

				if (command.strCommand.IsEmpty())
				{
					// don't add empty commands to list
					continue;
				}
				updatedRate.m_listCommands.push_back(command);
			}

			// check by running
			basicCheck.DDV_BasicCommands(pDX, IDC_MSHFG_BASIC, updatedRate.m_listCommands, m_bRenumbering);

			// restore view
			m_egBasic.SetRow(nRowSave);
			m_egBasic.SetTopRow(nTopRowSave);

			if (!m_bRenumbering)
			{
				m_egBasic.SetRow(0);
				m_egBasic.SetTopRow(0);
				// make sure contains SAVE token
				basicCheck.DDV_ContainsSave(pDX, IDC_MSHFG_BASIC, updatedRate.m_listCommands); 
			}

			// restore view
			m_egBasic.SetRow(nRowSave);
			m_egBasic.SetTopRow(nTopRowSave);

			// update commands
			foundItem->rate = updatedRate;

			// copy rates to rate list
			m_listRates.clear();
			itemIter = m_listItems.begin();
			for (; itemIter != m_listItems.end(); ++itemIter)
			{
				m_listRates.push_back(itemIter->rate);
			}
		}
	}
	else
	{
		if (m_bFirstSetActive)
		{
			// layout basic grid
			CRect rect;
			CDC* pDC = GetDC();
			int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			m_egBasic.GetClientRect(&rect);
			m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - 425 - 3);

			std::list<CRate>::iterator rateIter = m_listRates.begin();
			for (int i = 0; rateIter != m_listRates.end(); ++rateIter, ++i)
			{
				InsertRate(i, (*rateIter));
			}
			if (m_listRates.size())
			{
				ASSERT(m_tcNames.GetItemCount() > 0);
				// enable and display basic input
				DisplayTab(0);
			}
			else
			{
				// disable basic input
				DisplayTab(-1);
			}
		}
	}

	// validate  state
	ASSERT_VALID(this);
}


BEGIN_MESSAGE_MAP(CKPRatesPg1, baseKPRatesPg1)
	//{{AFX_MSG_MAP(CKPRatesPg1)
	ON_LBN_SELCHANGE(IDC_LB_FUNCS, OnSelchangeLbFuncs)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TC_NAMES, OnSelchangeTcNames)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TC_NAMES, OnSelchangingTcNames)
	ON_WM_SIZE()
	ON_LBN_SETFOCUS(IDC_LB_FUNCS, OnSetfocusLbFuncs)
	ON_EN_SETFOCUS(IDC_E_EXPLAN, OnSetfocusEExplan)
	ON_LBN_SETFOCUS(IDC_LB_ARGS, OnSetfocusLbArgs)
	ON_BN_CLICKED(IDC_B_RENUMBER, OnBRenumber)
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_ARGS, OnSetfocusTreeArgs)
	//}}AFX_MSG_MAP
	// custom BN_SETFOCUS notifications
	ON_BN_SETFOCUS(IDC_B_RENUMBER, OnSetfocusBRenumber)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
// COMMENT: {9/5/2001 7:27:37 PM}	ON_MESSAGE(EGN_CHANGE, OnChange)
END_MESSAGE_MAP()

BOOL CKPRatesPg1::OnInitDialog() 
{
	baseKPRatesPg1::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, GREEDY)
			<< (pane(VERTICAL, ABSOLUTE_HORZ)
				<< item(IDC_ST_NAMES, NORESIZE | ALIGN_BOTTOM)
				<< item(IDC_MSHFG_NAMES, GREEDY)
				)
			<< (paneTab(&m_tcNames, VERTICAL, GREEDY, nDefaultBorder, 4, 15)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_ST_BASIC, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_B_RENUMBER, NORESIZE)
					)
				<< item(IDC_MSHFG_BASIC, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_ST_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_LB_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
						)
					<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_ST_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_E_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
						)
					<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_ST_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
						<< item(IDC_TREE_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
						)
					)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	
	// Add extra initialization here
	m_egBasic.SetTrimOnEntry(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// COMMENT: {9/5/2001 7:27:47 PM}LRESULT CKPRatesPg1::OnChange(WPARAM wParam, LPARAM lParam)
// COMMENT: {9/5/2001 7:27:47 PM}{
// COMMENT: {9/5/2001 7:27:47 PM}	TRACE("OnChange\n");
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}	NMEGINFO* pInfo = (NMEGINFO*)lParam;
// COMMENT: {9/5/2001 7:27:47 PM}	UINT nID = wParam;
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}	ASSERT(m_bIgnoreChanges == true); // should have vbeen set in OnBeginCellEdit
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}	switch (nID)
// COMMENT: {9/5/2001 7:27:47 PM}	{
// COMMENT: {9/5/2001 7:27:47 PM}	case IDC_MSHFG_NAMES :
// COMMENT: {9/5/2001 7:27:47 PM}		//{{
// COMMENT: {9/5/2001 7:27:47 PM}		CRate rate;
// COMMENT: {9/5/2001 7:27:47 PM}		rate.m_strName = pInfo->item.pszText;
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}		// replace spaces with underscores
// COMMENT: {9/5/2001 7:27:47 PM}		rate.m_strName.TrimLeft();
// COMMENT: {9/5/2001 7:27:47 PM}		rate.m_strName.TrimRight();
// COMMENT: {9/5/2001 7:27:47 PM}		rate.m_strName.Replace(_T(' '), _T('_'));
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}		InsertRate(pInfo->item.iRow, rate);
// COMMENT: {9/5/2001 7:27:47 PM}		return FALSE;	// let InsertRate make change
// COMMENT: {9/5/2001 7:27:47 PM}
// COMMENT: {9/5/2001 7:27:47 PM}		//}}
// COMMENT: {9/5/2001 7:27:47 PM}	}
// COMMENT: {9/5/2001 7:27:47 PM}	return 0;
// COMMENT: {9/5/2001 7:27:47 PM}}

void CKPRatesPg1::OnSize(UINT nType, int cx, int cy) 
{
	baseKPRatesPg1::OnSize(nType, cx, cy);
	
	// resize the columns within the grid
	if (m_egBasic.GetSafeHwnd())
	{
		long nCol0 = m_egBasic.GetColWidth(0, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egBasic.GetClientRect(&rect);
		m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0/* - 50*/);
	}	
}

void CKPRatesPg1::OnSelchangeLbFuncs() 
{
	m_basicDesc.OnSelchangeLbFuncs();
}

void CKPRatesPg1::OnSelchangeTcNames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Note: OnSelchangingTcNames updates the rate
	UNUSED(pNMHDR);

	int nCurSel = m_tcNames.GetCurSel();
	ASSERT(nCurSel != -1 || m_tcNames.GetItemCount() == 0);

	DisplayTab(nCurSel);
	*pResult = 0;
}

LRESULT CKPRatesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	//DDX_Control(pDX, IDC_MSHFG_NAMES, m_egNames);
	//DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);

	if (nID == IDC_MSHFG_NAMES)
	{
		// do nothing
	}

	return bDenyEdit;
}

LRESULT CKPRatesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NAMES :
		{
			CRate rate;
			rate.m_strName = pInfo->item.pszText;

			// replace spaces with underscores
			rate.m_strName.TrimLeft();
			rate.m_strName.TrimRight();
			rate.m_strName.Replace(_T(' '), _T('_'));

// COMMENT: {5/17/2001 6:05:12 PM}			if (rate.m_strName.IsEmpty())
// COMMENT: {5/17/2001 6:05:12 PM}			{
// COMMENT: {5/17/2001 6:05:12 PM}				// TODO: remove rate
// COMMENT: {5/17/2001 6:05:12 PM}				ASSERT(FALSE);
// COMMENT: {5/17/2001 6:05:12 PM}				DeleteRate(pInfo->item.iRow);
// COMMENT: {5/17/2001 6:05:12 PM}			}
// COMMENT: {5/17/2001 6:05:12 PM}			else
// COMMENT: {5/17/2001 6:05:12 PM}			{
				InsertRate(pInfo->item.iRow, rate);
				return FALSE;	// let InsertRate make change
// COMMENT: {5/17/2001 6:05:15 PM}			}
		}
		break;

	case IDC_MSHFG_BASIC :
		// make sure grid has at least one empty row
		if (pInfo->item.iRow >= m_egBasic.GetRows() - m_egBasic.GetFixedRows() - 1)
		{
			m_egBasic.SetRows(m_egBasic.GetRows() + 5);
			// MAYBE number added lines
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

void CKPRatesPg1::OnSelchangingTcNames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	ASSERT(m_tcNames.GetCurSel() != -1);

	// validate basic and disallow tab change if (UpdateData != TRUE)
	*pResult = !(UpdateData(TRUE));
}

CString CKPRatesPg1::GetRateName(long nRowIndex)const
{
	ASSERT_VALID(this);
	CString strName;
	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		if (itemIter->nRowIndex == nRowIndex)
		{
			strName = itemIter->rate.m_strName;
			// item found
			break;
		}
	}
	ASSERT_VALID(this);
	return strName;
}

BOOL CKPRatesPg1::SetRateName(long nRowIndex, LPCTSTR lpszRateName)
{
	ASSERT_VALID(this);
	ASSERT(lpszRateName && lstrlen(lpszRateName));
	BOOL bItemFound = FALSE;
	std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		if (itemIter->nRowIndex == nRowIndex)
		{
			// item found
			bItemFound = TRUE;

			// update list
			itemIter->rate.m_strName = lpszRateName;

			// update grid
			m_egNames.SetTextMatrix(itemIter->nRowIndex, 0, lpszRateName);

			// update tab
			TCITEM item;
			item.mask = TCIF_TEXT;
			item.pszText = (LPTSTR)lpszRateName;
			m_tcNames.SetItem(itemIter->nTabIndex, &item);

			break;
		}
	}
	ASSERT_VALID(this);
	return bItemFound;
}

BOOL CKPRatesPg1::GetRate(long nRowIndex, CRate* pRate)const
{
	ASSERT(pRate != NULL);
	ASSERT_VALID(this);
	BOOL bItemFound = FALSE;
	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		if (itemIter->nRowIndex == nRowIndex)
		{
			// item found
			bItemFound = TRUE;
			pRate->m_strName = itemIter->rate.m_strName;
			pRate->m_listCommands.assign(itemIter->rate.m_listCommands.begin(), itemIter->rate.m_listCommands.end());
			break;
		}
	}
	ASSERT_VALID(this);
	return bItemFound;
}

BOOL CKPRatesPg1::DeleteRate(long nRowIndex)
{
	ASSERT_VALID(this);
	BOOL bItemFound = FALSE;
	XGridTabItem foundItem;
	std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		if (itemIter->nRowIndex == nRowIndex)
		{
			foundItem = (*itemIter);
			// item found
			bItemFound = TRUE;

			// update grid
			m_egNames.DeleteRow(itemIter->nRowIndex);

			// update tab
// COMMENT: {7/13/2000 11:39:03 AM}			//{{
// COMMENT: {7/13/2000 11:39:03 AM}			int nCurSel = m_tcNames.GetCurSel();
// COMMENT: {7/13/2000 11:39:03 AM}			VERIFY(m_tcNames.DeleteItem(itemIter->nTabIndex));
// COMMENT: {7/13/2000 11:39:03 AM}			if (nCurSel == itemIter->nTabIndex)
// COMMENT: {7/13/2000 11:39:03 AM}			{
// COMMENT: {7/13/2000 11:39:03 AM}				if (m_tcNames.GetItemCount())
// COMMENT: {7/13/2000 11:39:03 AM}				{
// COMMENT: {7/13/2000 11:39:03 AM}					m_tcNames.SetCurSel(0);
// COMMENT: {7/13/2000 11:39:03 AM}				}
// COMMENT: {7/13/2000 11:39:03 AM}				OnSelchangeTcNames(NULL, NULL);
// COMMENT: {7/13/2000 11:39:03 AM}			}
// COMMENT: {7/13/2000 11:39:03 AM}			//}}

			// remove from list
			itemIter = m_listItems.erase(itemIter);
			break;
		}
	}
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		// update rates following the deleted rate
		itemIter->nRowIndex--;
		itemIter->nTabIndex--;
	}

	// update tab
	//{{
	int nCurSel = m_tcNames.GetCurSel();
	VERIFY(m_tcNames.DeleteItem(foundItem.nTabIndex));
	if (nCurSel == foundItem.nTabIndex)
	{
		if (m_tcNames.GetItemCount())
		{
			m_tcNames.SetCurSel(0);
		}
		// LRESULT* pResult
		LRESULT result;
		OnSelchangeTcNames(NULL, &result);
	}
	//}}

	// Recalc Tab area
	UpdateLayout();

	ASSERT_VALID(this);
	return bItemFound;
}

void CKPRatesPg1::InsertRate(long nRowIndex, CRate& rate)
{
	ASSERT_VALID(this);

	// validate row
	ASSERT(nRowIndex < m_egNames.GetRows());
	ASSERT(nRowIndex >= m_egNames.GetFixedRows());

	int nTabsBefore = m_tcNames.GetItemCount();

	// find first item after new rate
	std::list<XGridTabItem>::iterator itemIter       = m_listItems.begin();
	std::list<XGridTabItem>::iterator firstItemAfter = m_listItems.end();
	std::list<XGridTabItem>::iterator oldItem        = m_listItems.end();
	for (; itemIter != firstItemAfter; ++itemIter)
	{
		if (itemIter->nRowIndex == nRowIndex)
		{
			oldItem = itemIter;
			break;
		}
		if (itemIter->nRowIndex > nRowIndex)
		{
			firstItemAfter = itemIter;
			break;
		}
	}

	if (oldItem != m_listItems.end())
	{
		if (rate.m_strName.IsEmpty())
		{
			// save tab info before deleting
			int nTabIndex = oldItem->nTabIndex;

			// remove from list
			itemIter = m_listItems.erase(oldItem);

			// update items following the deleted rate
			for (; itemIter != m_listItems.end(); ++itemIter)
			{
				itemIter->nTabIndex--;
			}

			// remove tab
			int nCurSel = m_tcNames.GetCurSel();
			VERIFY(m_tcNames.DeleteItem(nTabIndex));
			if (nCurSel == nTabIndex)
			{
				if (m_tcNames.GetItemCount())
				{
					m_tcNames.SetCurSel(0);
				}
				LRESULT result;
				OnSelchangeTcNames(NULL, &result);
			}

			// update grid
			m_egNames.SetTextMatrix(nRowIndex, 0, _T(""));
		}
		else
		{
			// item already exists just update rate name
			// update grid
			m_egNames.SetTextMatrix(nRowIndex, 0, rate.m_strName);

			// update tab
			TCITEM item;
			item.mask = TCIF_TEXT;
			item.pszText = (LPTSTR)(LPCTSTR)rate.m_strName;
			m_tcNames.SetItem(oldItem->nTabIndex, &item);

			// update list
			oldItem->rate.m_strName = rate.m_strName;
		}
	}
	else
	{
		if (!rate.m_strName.IsEmpty())
		{
			// this is a new rate add to list
			XGridTabItem item;
			item.rate = rate;
			item.nRowIndex = nRowIndex;

			// make sure grid has at least one empty row
			if (nRowIndex >= m_egNames.GetRows() - m_egNames.GetFixedRows() - 1)
			{
				m_egNames.SetRows(m_egNames.GetRows() + 1);
			}

			// add to grid
			m_egNames.SetTextMatrix(nRowIndex, 0, item.rate.m_strName);

			// add to list
			if (firstItemAfter == m_listItems.end())
			{
				// add tab 
				item.nTabIndex = m_tcNames.InsertItem(item.nRowIndex, item.rate.m_strName);
				m_listItems.push_back(item);

				// should be last tab
				ASSERT(item.nTabIndex + 1 == m_tcNames.GetItemCount());

				// if this is the first tab display it
				if (item.nTabIndex == 0)
				{
					DisplayTab(0);
				}
			}
			else
			{
				if (firstItemAfter == m_listItems.begin())
				{
					item.nTabIndex = m_tcNames.InsertItem(0, item.rate.m_strName);

					// should be first tab
					ASSERT(item.nTabIndex == 0);
				}
				else
				{
					// get prev item
					std::list<XGridTabItem>::iterator itemPrev = firstItemAfter;
					itemPrev--;

					// add tab 
					item.nTabIndex = m_tcNames.InsertItem(itemPrev->nTabIndex + 1, item.rate.m_strName);

					// shouldn't be first tab
					ASSERT(item.nTabIndex > 0);

					// should be adjacent to previous tab
					ASSERT(item.nTabIndex == itemPrev->nTabIndex + 1);
				}

				// shouldn't be last tab
				ASSERT(item.nTabIndex + 1 < m_tcNames.GetItemCount());

				// update tabs after this one
				itemIter = m_listItems.insert(firstItemAfter, item);
				for (itemIter++; itemIter != m_listItems.end(); ++itemIter)
				{
					itemIter->nTabIndex++;
				}
			}
		}
	}

	if (nTabsBefore == 0 && m_tcNames.GetItemCount() > 0 ||
		nTabsBefore != 0 && m_tcNames.GetItemCount() == 0)
	{
		// Recalc Tab area
		UpdateLayout();
	}

	ASSERT_VALID(this);
}

//
// nTabIndex == -1 signifies no commands
//
void CKPRatesPg1::DisplayTab(int nTabIndex)
{
	ASSERT_VALID(this);

	std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
	std::list<XGridTabItem>::iterator foundItem = m_listItems.end();

	// find XGridTabItem
	for (; itemIter != foundItem; ++itemIter)
	{
		if (itemIter->nTabIndex == nTabIndex)
		{
			foundItem = itemIter;
			break;
		}
	}

	if (nTabIndex != -1)
	{
		ASSERT(foundItem != m_listItems.end()); // must exist
	}

	// set line number column width
	m_egBasic.SetColWidth(0, 0, 625);

	if (nTabIndex != -1)
	{
		// init m_egBasic rows
		long size = foundItem->rate.m_listCommands.size();
		if (m_egBasic.GetRows() < size + 5)
		{
			m_egBasic.SetRows(size + 5);
		}
	}

	CString str;
	long nLastLineNumber = 0;
	long nRow = 0;

	if (nTabIndex != -1)
	{
		std::list<basic_command>::iterator cmdIter = foundItem->rate.m_listCommands.begin();
		for (; cmdIter != foundItem->rate.m_listCommands.end(); ++cmdIter, ++nRow)
		{
			nLastLineNumber = cmdIter->nLine;
			str.Format(_T("%d"), cmdIter->nLine);
			m_egBasic.SetTextMatrix(nRow, 0, str);
			m_egBasic.SetTextMatrix(nRow, 1, cmdIter->strCommand);
		}
	}
	// determine next line number
	long nLineNumber = ((nLastLineNumber / 10) + 1) * 10;
	for (; nRow < m_egBasic.GetRows(); ++nRow, nLineNumber += 10)
	{
		str.Format(_T("%d"), nLineNumber);
		m_egBasic.SetTextMatrix(nRow, 0, str);
		m_egBasic.SetTextMatrix(nRow, 1, _T(""));
	}

	// reset grid
	m_egBasic.SetTopRow(0);
	m_egBasic.SetRow(0);
	m_egBasic.SetCol(1);

	// if nTabIndex == -1 then there shouldn't be any items
	ASSERT(nTabIndex != -1 || m_listItems.size() == 0);

	// if nTabIndex == -1 then there shouldn't be any tabs
	ASSERT(nTabIndex != -1 || m_tcNames.GetItemCount() == 0);

	m_egBasic.EnableWindow(nTabIndex != -1);
	CWnd* pWnd = GetDlgItem(IDC_B_RENUMBER);
	pWnd->EnableWindow(nTabIndex != -1);
	pWnd = GetDlgItem(IDC_ST_BASIC);
	pWnd->EnableWindow(nTabIndex != -1);

	ASSERT_VALID(this);
}
/////////////////////////////////////////////////////////////////////////////
// CKPRatesPg1 diagnostics

#ifdef _DEBUG
void CKPRatesPg1::AssertValid() const
{
	baseKPRatesPg1::AssertValid();

	if (!m_tcNames.GetSafeHwnd())
		return;

	// verify state of tabctrl
	ASSERT((int)m_listItems.size() == m_tcNames.GetItemCount());


	// verify state of list
	long nRowIndexPrev = -1;
	int nTabIndexPrev = -1;
	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		// Note: the list should be in the same order as tabs and the grid
		ASSERT(itemIter->nRowIndex > nRowIndexPrev);
		ASSERT(itemIter->nTabIndex > nTabIndexPrev);

		// check tab
		TCITEM item;
		TCHAR buffer[80];
		item.mask = TCIF_TEXT;
		item.cchTextMax = 80;
		item.pszText = buffer;
		ASSERT(m_tcNames.GetItem(itemIter->nTabIndex, &item));
		ASSERT(item.pszText);
		ASSERT(itemIter->rate.m_strName == item.pszText);

		// can't check grid since GetTextMatrix is not const
		// ASSERT(itemIter->rate.m_strName == m_egNames.GetTextMatrix(itemIter->nRowIndex, 0));

		nRowIndexPrev = itemIter->nRowIndex;
		nTabIndexPrev = itemIter->nTabIndex;
	}
}

void CKPRatesPg1::Dump(CDumpContext& dc) const
{
	baseKPRatesPg1::Dump(dc);
	dc << "Item list contains " << m_listItems.size() << " items\n";

	std::list<XGridTabItem>::const_iterator itemIter = m_listItems.begin();
	for (; itemIter != m_listItems.end(); ++itemIter)
	{
		dc << "Row = " << itemIter->nRowIndex << ", Tab = " << itemIter->nTabIndex <<
			", Rate name = " << itemIter->rate.m_strName << "\n";
	}

}
#endif //_DEBUG


//////////////////////////////////////////////////////////////////////
// CKPRatesPg1::XGridTabItem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKPRatesPg1::XGridTabItem::XGridTabItem()
{
	CRate rate;
	nRowIndex = -1;
	nTabIndex = -1;
}


BEGIN_EVENTSINK_MAP(CKPRatesPg1, baseKPRatesPg1)
    //{{AFX_EVENTSINK_MAP(CKPRatesPg1)
	ON_EVENT(CKPRatesPg1, IDC_MSHFG_NAMES, -602 /* KeyDown */, OnKeyDownMshfgNames, VTS_PI2 VTS_I2)
	ON_EVENT(CKPRatesPg1, IDC_MSHFG_NAMES, 71 /* EnterCell */, OnEnterCellMshfgNames, VTS_NONE)
	ON_EVENT(CKPRatesPg1, IDC_MSHFG_BASIC, 71 /* EnterCell */, OnEnterCellMshfgBasic, VTS_NONE)
	ON_EVENT(CKPRatesPg1, IDC_MSHFG_BASIC, -602 /* KeyDown */, OnKeyDownMshfgBasic, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPRatesPg1::OnKeyDownMshfgNames(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		{
			CString strRateName = m_egNames.GetTextMatrix(m_egNames.GetRow(), m_egNames.GetCol());
			if (!strRateName.IsEmpty())
			{
				CString strMsg;
				::AfxFormatString1(strMsg, IDS_REMOVE_RATE, strRateName);
				if (AfxMessageBox(strMsg, MB_YESNOCANCEL) == IDYES)
				{
					CRate rate;
					DeleteRate(m_egNames.GetRow());
					//AfxMessageBox("%1 and the associated BASIC statements will be deleted.");
				}
				else
				{
					//AfxMessageBox("%1 and the associated BASIC statements will NOT be deleted.");
				}
			}
		}
		break;
	}
}

void CKPRatesPg1::OnEnterCellMshfgNames() 
{
	CString strRes;
	strRes.LoadString(IDS_RATE_254);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPRatesPg1::OnEnterCellMshfgBasic() 
{
	CString strRes;
	switch (m_egBasic.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_RATE_261);
		break;
	case 1 :
		strRes.LoadString(IDS_RATE_262);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPRatesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	int nID = wParam;
	switch (nID)
	{
	case IDC_MSHFG_NAMES :
		OnEnterCellMshfgNames();
		break;
	case IDC_MSHFG_BASIC :
		OnEnterCellMshfgBasic();
		break;
	}
	return 0;
}

void CKPRatesPg1::OnSetfocusLbFuncs() 
{
	CString strRes;
	strRes.LoadString(IDS_RATE_258);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPRatesPg1::OnSetfocusEExplan() 
{
	CString strRes;
	strRes.LoadString(IDS_RATE_259);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPRatesPg1::OnSetfocusLbArgs() 
{
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPRatesPg1::OnSetfocusBRenumber()
{
	CString strRes;
	strRes.LoadString(IDS_STRING512);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPRatesPg1::OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		{
			CWaitCursor wait;	// display hourglass cursor
			m_egBasic.SetRedraw(FALSE);
			long nCol    = m_egBasic.GetCol();
			long nColSel = m_egBasic.GetColSel();

			// check if entire row is selected
			if ((min(nCol, nColSel) == 0) && (1 == max(nCol, nColSel)))
			{
				long nRow    = m_egBasic.GetRow();
				long nRowSel = m_egBasic.GetRowSel();

				// determine how many rows to delete
				long cSelectedRows = abs(nRow - nRowSel) + 1;

				// determine first row to delete
				long iStartRow = min(nRow, nRowSel);

				// delete each row
				for (long i = 0; i < cSelectedRows; ++i)
				{
					m_egBasic.DeleteRow(iStartRow);
				}
			}
			else 
			{
				// just fill with empty strings
				m_egBasic.SetFillStyle(flexFillRepeat);
				m_egBasic.SetText(_T(""));
				m_egBasic.SetFillStyle(flexFillSingle);
			}
			m_egBasic.SetRedraw(TRUE);
		}
		break;
	}
}

void CKPRatesPg1::OnBRenumber() 
{
	m_bRenumbering = true;

	int nRate = m_tcNames.GetCurSel();
	ASSERT(nRate != -1);

	if (UpdateData(TRUE))
	{
		// BASIC Ok
		std::list<XGridTabItem>::iterator itemIter = m_listItems.begin();
		for (int i = 0; i < nRate; ++i)
		{
			ASSERT(itemIter != m_listItems.end());
			itemIter++;
		}
		ASSERT(itemIter != m_listItems.end());

		CBasicObj basicCheck;
		if (basicCheck.Renumber((*itemIter).rate.m_listCommands))
		{
			// update grid
			DisplayTab(nRate);
		}
	}
	else
	{
		// do nothing BASIC contains errors
	}
	m_bRenumbering = false;
}

BOOL CKPRatesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_B_RENUMBER:
		strRes.LoadString(IDS_STRING512);
		break;
	case IDC_LB_FUNCS: case IDC_ST_FUNCS:
		strRes.LoadString(IDS_RATE_258);
		break;
	case IDC_TREE_ARGS: case IDC_ST_ARGS:
		strRes.LoadString(IDS_RATE_260);
		break;		
	case IDC_MSHFG_NAMES :
		strRes.LoadString(IDS_RATE_254);
		break;
	case IDC_MSHFG_BASIC :
		if (!IsContextHelp())
		{
			if (!(m_egBasic.GetRowIsVisible(m_egBasic.GetRow()) && m_egBasic.GetColIsVisible(m_egBasic.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egBasic.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egBasic.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egBasic.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egBasic.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egBasic.GetMouseCol() : m_egBasic.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_RATE_261);
			break;
		case 1 :
			strRes.LoadString(IDS_RATE_262);
			break;
		}
		break;
	case IDC_E_EXPLAN: case IDC_ST_EXPLAN:
		strRes.LoadString(IDS_RATE_259);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPRatesPg1::OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);	
	
	*pResult = 0;
}
