// CKPMixPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPMixPg1.h"

#include "CKSMix.h"
#include "Util.h"
#include "DDX_DDV.h"
#include "DelayUpdate.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPMixPg1, baseCKPMixPg1)


/////////////////////////////////////////////////////////////////////////////
// CCKPMixPg1 property page

CCKPMixPg1::CCKPMixPg1() : baseCKPMixPg1(CCKPMixPg1::IDD)
, m_glDoc(&m_ctrlMixEditGrid, &m_ctrlSolCheckList, 1)
{
	//{{AFX_DATA_INIT(CCKPMixPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPMixPg1::~CCKPMixPg1()
{
}

void CCKPMixPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPMixPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPMixPg1)
	DDX_Control(pDX, IDC_CL_SOLS, m_ctrlSolCheckList);
	DDX_Control(pDX, IDC_MSHFG_MIX, m_ctrlMixEditGrid);
	//}}AFX_DATA_MAP
	DDX_MixCompList(pDX, IDC_MSHFG_MIX);
}

void CCKPMixPg1::DDX_MixCompList(CDataExchange* pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_MIX);

	// reduce grid flicker
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc (Mix doesn't support a range)
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		//
		// Init checklist
		// 

		CUtil::InsertRange(&m_ctrlSolCheckList, ((CCKSMix*)GetSheet())->m_setSolutions);

		//
		// Init edit grid
		//

		// set header row height
		m_ctrlMixEditGrid.SetRowHeight(0, (long) 2.5 * m_ctrlMixEditGrid.GetRowHeight(1));

		// set column titles
		m_ctrlMixEditGrid.SetTextMatrix(0, 1, _T("Solution number"));
		m_ctrlMixEditGrid.SetTextMatrix(0, 2, _T("Mixing fraction"));

		// set bold titles
		m_ctrlMixEditGrid.SetRow(0), m_ctrlMixEditGrid.SetCol(1);
		m_ctrlMixEditGrid.SetCellFontBold(TRUE);
		m_ctrlMixEditGrid.SetRow(0), m_ctrlMixEditGrid.SetCol(2);
		m_ctrlMixEditGrid.SetCellFontBold(TRUE);

		// center align the column headers
		m_ctrlMixEditGrid.SetFillStyle(flexFillRepeat);
		m_ctrlMixEditGrid.SetRow(0);
		m_ctrlMixEditGrid.SetCol(0);
		m_ctrlMixEditGrid.SetColSel(m_ctrlMixEditGrid.GetCols(0) - 1);
		m_ctrlMixEditGrid.SetCellAlignment(flexAlignCenterCenter);
		m_ctrlMixEditGrid.SetFillStyle(flexFillSingle);

		// set row titles
		m_ctrlMixEditGrid.AddRowHeaders();


		// set column widths
		m_ctrlMixEditGrid.SetColWidth(1, 0, 1100);
		m_ctrlMixEditGrid.SetColWidth(2, 0, 1100);

		// scroll to top of grid
		m_ctrlMixEditGrid.SetTopRow(1); m_ctrlMixEditGrid.SetLeftCol(1);

		// set initial cell
		m_ctrlMixEditGrid.SetRow(1); m_ctrlMixEditGrid.SetCol(1);
	}


	// do exchange
	if (pDX->m_bSaveAndValidate)
	{
		std::list<mix_comp> listMixComp;
		for (long nRow = m_ctrlMixEditGrid.GetFixedRows(); nRow < m_ctrlMixEditGrid.GetRows(); ++nRow)
		{
			CString str;

			DDX_GridText(pDX, nIDC, nRow, 1, str);
			if (str.IsEmpty())
				continue;

			struct mix_comp comp;

			// check solution number
			if (::_stscanf(str, _T("%d"), &comp.n_solution) != 1 || comp.n_solution < 0)
			{
				DDX_GridFail(pDX, _T("Solution number must be a positive integer."),
					_T("Invalid solution number"));
			}

			// check fraction
			DDX_GridText(pDX, nIDC, nRow, 2, comp.fraction, IDS_MIX_176);

			// ok add to list
			listMixComp.push_back(comp);
		}

		// Must define at least one solution number and mixing fraction for MIX input.
		if (listMixComp.empty())
		{
			CString str;
			DDX_GridText(pDX, nIDC, 1, 1, str);
			DDX_GridFail(pDX, IDS_MIX_177);
		}
		// ok if here
		m_listMixComp.assign(listMixComp.begin(), listMixComp.end());
	}
	else
	{
		std::list<mix_comp>::const_iterator iter = m_listMixComp.begin();
		CString strFormat;
		for (; iter != m_listMixComp.end(); ++iter)
		{
			// solution number
			strFormat.Format(((CCKSMix*)GetSheet())->m_strNumFormat, (*iter).n_solution);
			int nRow = m_glDoc.AddItem(strFormat).m_nRow;

			// fraction
			double value = (*iter).fraction;
			DDX_GridText(pDX, nIDC, nRow, 2, value);
		}
	}

}


BEGIN_MESSAGE_MAP(CCKPMixPg1, baseCKPMixPg1)
	//{{AFX_MSG_MAP(CCKPMixPg1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SOLS, OnItemchangedClSols)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_SOLS, OnSetfocusClSols)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKPMixPg1 message handlers

BOOL CCKPMixPg1::OnInitDialog() 
{
	baseCKPMixPg1::OnInitDialog();
	
	// TODO: Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< itemFixed(VERTICAL, 1)

		<< item(IDC_S_DEF_SOL, NORESIZE | ALIGN_BOTTOM)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_SOLS, GREEDY)
			//<< itemSpaceBetween(HORIZONTAL, IDC_CL_GASPHASE, IDC_MSHFG_GASPHASE)
			<< itemFixed(HORIZONTAL, 1)
			<< item(IDC_MSHFG_MIX, ABSOLUTE_HORZ)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCKPMixPg1::OnItemchangedClSols(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// do default
	long nRow = m_glDoc.OnItemChanged(pNMHDR, pResult);
	if (nRow != -1 && m_ctrlMixEditGrid.GetTextMatrix(nRow, 2).IsEmpty())
	{
		m_ctrlMixEditGrid.SetTextMatrix(nRow, 2, _T("1"));
	}
}

LRESULT CCKPMixPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_MIX )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// only fill if less than 500 items
			if (((CCKSMix*)GetSheet())->m_setSolutions.size() < 501)
			{
				pInfo->item.bUseCombo = (CUtil::InsertRange(pInfo->item.hWndCombo, ((CCKSMix*)GetSheet())->m_setSolutions) > 0);
			}
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCKPMixPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_MIX :
		{
			// fill defaults
			long col = pInfo->item.iCol;
			if (col == 1)
			{
				int n;
				// reformat original string to match 
				// string contained in CheckListCtrl
				CString strOld = m_ctrlMixEditGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				if (!strOld.IsEmpty() && ::_stscanf(strOld, "%d", &n))
				{
					CString strFormat;
					strFormat.Format(((CCKSMix*)GetSheet())->m_strNumFormat, n);
					m_ctrlMixEditGrid.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strFormat);
				}

				// check if number was entered
				// if it was, reformat it to match
				// string contained in CheckListCtrl
				if (::_stscanf(pInfo->item.pszText, "%d", &n) == 1)
				{
					CString strFormat;
					strFormat.Format(((CCKSMix*)GetSheet())->m_strNumFormat, n);
					free(pInfo->item.pszText);
					pInfo->item.pszText = _tcsdup(strFormat);
				}

				CString str(pInfo->item.pszText);
				str.TrimLeft();
				if (!str.IsEmpty() && m_ctrlMixEditGrid.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
				{
					m_ctrlMixEditGrid.SetTextMatrix(pInfo->item.iRow, 2, _T("1"));
				}
			}

			return m_glDoc.OnEndCellEdit(wParam, lParam);
		}
		break;
	}

	return bMakeChange;
}

void CCKPMixPg1::OnSetfocusClSols(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlSolCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;

	AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlSolCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlSolCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

LRESULT CCKPMixPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_MIX :
		OnEnterCellMshfgMix();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CCKPMixPg1, baseCKPMixPg1)
    //{{AFX_EVENTSINK_MAP(CCKPMixPg1)
	ON_EVENT(CCKPMixPg1, IDC_MSHFG_MIX, -602 /* KeyDown */, OnKeyDownMshfgMix, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPMixPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPMixPg1, IDC_MSHFG_MIX, 71 /* EnterCell */, OnEnterCellMshfgMix, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKPMixPg1 event handlers

void CCKPMixPg1::OnKeyDownMshfgMix(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

void CCKPMixPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_MIX_NUM_173);
		break;
	case 2 :
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("mixture") ); 
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPMixPg1::OnEnterCellMshfgMix() 
{
	CString strRes;

	switch (m_ctrlMixEditGrid.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_MIX_SOL_NUM_174);
		break;
	case 2 :
		strRes.LoadString(IDS_MIX_FRAC_175);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CCKPMixPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CL_SOLS: case IDC_S_DEF_SOL:
		AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 
		break;
	case IDC_MSHFG_MIX:
		if (!IsContextHelp())
		{
			if (!(m_ctrlMixEditGrid.GetRowIsVisible(m_ctrlMixEditGrid.GetRow()) && m_ctrlMixEditGrid.GetColIsVisible(m_ctrlMixEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlMixEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlMixEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlMixEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlMixEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlMixEditGrid.GetMouseCol() : m_ctrlMixEditGrid.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_MIX_SOL_NUM_174);
			break;
		case 2 :
			strRes.LoadString(IDS_MIX_FRAC_175);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
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
		case 0 :
			strRes.LoadString(IDS_MIX_NUM_173);
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("mixture") ); 
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
