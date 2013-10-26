// SelectedOutputPg10.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "SelectedOutputPg10.h"

#include "Util.h"
#include "DelayUpdate.h"

// CSelectedOutputPg10 dialog

IMPLEMENT_DYNAMIC(CSelectedOutputPg10, baseCSelectedOutputPg10)

CSelectedOutputPg10::CSelectedOutputPg10() : baseCSelectedOutputPg10(CSelectedOutputPg10::IDD)
, glDoc(&egCalcValues, &clcCalcValues, 1)
{

}

CSelectedOutputPg10::~CSelectedOutputPg10()
{
}

void CSelectedOutputPg10::DoDataExchange(CDataExchange* pDX)
{
	baseCSelectedOutputPg10::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CL_CALCVALUES, this->clcCalcValues);
	DDX_Control(pDX, IDC_MSHFG_CALCVALUES, this->egCalcValues);

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_CALCVALUES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		this->m_egNumDesc.SetColWidth(0, 0, 1100);
		this->m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		this->m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertCalcValues(&this->clcCalcValues, GetDatabase());
		this->egCalcValues.AddRowHeaders();
		this->egCalcValues.SetTextMatrix(0, 1, _T("Name"));
		this->egCalcValues.SetColWidth(1, 0, 1400);

		// center align the column headers
		this->egCalcValues.SetFillStyle(flexFillRepeat);
		this->egCalcValues.SetRow(0);
		this->egCalcValues.SetCol(0);
		this->egCalcValues.SetColSel(this->egCalcValues.GetCols(0) - 1);
		this->egCalcValues.SetCellAlignment(flexAlignCenterCenter);
		this->egCalcValues.SetFillStyle(flexFillSingle);


		// set bold titles
		this->egCalcValues.SetRow(0), this->egCalcValues.SetCol(1);
		this->egCalcValues.SetCellFontBold(TRUE);

		// set initial cell
		this->egCalcValues.SetRow(1), this->egCalcValues.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listCV;
		for (long nRow = egCalcValues.GetFixedRows(); nRow < egCalcValues.GetRows(); ++nRow)
		{
			CString strCalcValues;

			DDX_GridText(pDX, IDC_MSHFG_CALCVALUES, nRow, 1, strCalcValues);
			if (strCalcValues.IsEmpty())
				continue;

			listCV.push_back(strCalcValues);
		}


		// if here list is valid and can be assigned to the member variable
		this->listCalcValues.assign(listCV.begin(), listCV.end());
	}
	else
	{
		// see OnKillActive
		ASSERT(this->glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = this->listCalcValues.begin();
		for (; iter != this->listCalcValues.end(); ++iter)
		{
			this->glDoc.AddItem(*iter);
		}
		ASSERT(this->listCalcValues.size() == this->glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CSelectedOutputPg10, baseCSelectedOutputPg10)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_CALCVALUES, OnItemchangedClCalcValues)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_CALCVALUES, OnSetfocusClCalcValues)
	ON_WM_HELPINFO()

	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()


// CSelectedOutputPg10 message handlers
BOOL CSelectedOutputPg10::OnKillActive() 
{
	if (baseCSelectedOutputPg10::OnKillActive())
	{
		// as last step remove all entries from glDoc;
		this->glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CSelectedOutputPg10::OnItemchangedClCalcValues(NMHDR* pNMHDR, LRESULT* pResult) 
{
	this->glDoc.OnItemChanged(pNMHDR, pResult);
}

void CSelectedOutputPg10::OnSetfocusClCalcValues(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING755);
	this->m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (this->clcCalcValues.GetItemCount() && this->clcCalcValues.GetFirstSelectedItemPosition() == NULL)
	{
		this->clcCalcValues.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CSelectedOutputPg10, baseCSelectedOutputPg10)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg9)
	ON_EVENT(CSelectedOutputPg10, IDC_MSHFG_CALCVALUES, -602 /* KeyDown */, OnKeyDownMshfgCalcValues, VTS_PI2 VTS_I2)
	ON_EVENT(CSelectedOutputPg10, IDC_MSHFG_CALCVALUES, 71 /* EnterCell */, OnEnterCellMshfgCalcValues, VTS_NONE)
	ON_EVENT(CSelectedOutputPg10, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSelectedOutputPg10::OnKeyDownMshfgCalcValues(short FAR* KeyCode, short Shift) 
{
	this->glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CSelectedOutputPg10::OnInitDialog() 
{
	baseCSelectedOutputPg10::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_CALCVALUES, GREEDY)
				<< item(IDC_MSHFG_CALCVALUES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CSelectedOutputPg10::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_CALCVALUES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertCalcValues(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

//{{
LRESULT CSelectedOutputPg10::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
	case IDC_MSHFG_CALCVALUES :
		return glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}
	return bMakeChange;
}

LRESULT CSelectedOutputPg10::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_CALCVALUES:
		OnEnterCellMshfgCalcValues();
		break;
	}
	return 0;
}

void CSelectedOutputPg10::OnEnterCellMshfgCalcValues() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING754);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CSelectedOutputPg10::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CL_CALCVALUES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING755);
		break;
	case IDC_MSHFG_CALCVALUES:
		strRes.LoadString(IDS_STRING754);
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
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
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

void CSelectedOutputPg10::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}
//}}
