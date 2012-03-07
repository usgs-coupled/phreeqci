// KPSavePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSavePg1.h"

#include "DDX_DDV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSavePg1, baseKPSavePg1)

const CKeyword::type CKPSavePg1::s_arrKeyTypes[] =
{
	CKeyword::K_EQUILIBRIUM_PHASES,
	CKeyword::K_EXCHANGE,
	CKeyword::K_GAS_PHASE,
	CKeyword::K_SOLID_SOLUTIONS,
	CKeyword::K_SOLUTION,
	CKeyword::K_SURFACE,
};


/////////////////////////////////////////////////////////////////////////////
// CKPSavePg1 property page

CKPSavePg1::CKPSavePg1() : baseKPSavePg1(CKPSavePg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSavePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSavePg1::~CKPSavePg1()
{
}

void CKPSavePg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPSavePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSavePg1)
	DDX_Control(pDX, IDC_S_DESC_INPUT, m_btnInputDesc);
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFG_SAVE, m_egSave);
	//}}AFX_DATA_MAP
	DDX_Save(pDX, IDC_MSHFG_SAVE);
}


BEGIN_MESSAGE_MAP(CKPSavePg1, baseKPSavePg1)
	//{{AFX_MSG_MAP(CKPSavePg1)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// specialized grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPSavePg1, baseKPSavePg1)
    //{{AFX_EVENTSINK_MAP(CKPSavePg1)
	ON_EVENT(CKPSavePg1, IDC_MSHFG_SAVE, 71 /* EnterCell */, OnEnterCellMshfgSave, VTS_NONE)
	ON_EVENT(CKPSavePg1, IDC_MSHFG_SAVE, -602 /* KeyDown */, OnKeyDownMshfgSave, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSavePg1::DDX_Save(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_SAVE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitSave(pDX, nIDC);
	}

	if (pDX->m_bSaveAndValidate)
	{
		for (long nRow = FIRST_ROW; nRow < LAST_ROW; ++nRow)
		{
			CRange range;
			CString strTemp;
			DDX_GridText(pDX, nIDC, nRow, 1, strTemp);
			if (strTemp.IsEmpty())
			{
				range.nMin = N_EMPTY;
				range.nMax = N_EMPTY;
			}
			else
			{
				DDX_GridText(pDX, nIDC, nRow, 1, range.nMin);

				DDX_GridText(pDX, nIDC, nRow, 3, strTemp);
				if (strTemp.IsEmpty())
				{
					range.nMax = N_EMPTY;
				}
				else
				{
					DDX_GridText(pDX, nIDC, nRow, 3, range.nMax);
				}
			}
			m_arrRange[nRow] = range;
		}

	}
	else
	{
		for (long nRow = FIRST_ROW; nRow < LAST_ROW; ++nRow)
		{
			CRange range = m_arrRange[nRow];

			if (range.nMin != N_EMPTY)
			{
				DDX_GridText(pDX, nIDC, nRow, 1, range.nMin);
				if (range.nMax != N_EMPTY && range.nMax != range.nMin)
				{
					DDX_GridText(pDX, nIDC, nRow, 3, range.nMax);
				}
			}

		}
		// set initial position
		m_egSave.SetRow(0);
		m_egSave.SetCol(1);
	}

}

void CKPSavePg1::InitSave(CDataExchange *pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == IDC_MSHFG_SAVE);
	UNUSED(nIDC);  // unused in release builds

	CString strKey;
	ASSERT(m_egSave.GetRows() == LAST_ROW);
	for (long nRow = FIRST_ROW; nRow < LAST_ROW; ++nRow)
	{
		// set titles
		strKey = CKeyword::GetString(s_arrKeyTypes[nRow]);
		strKey.MakeLower();
		m_egSave.SetTextMatrix(nRow, 0, strKey);

		// bold and centered
		m_egSave.SetCol(0);
		m_egSave.SetRow(nRow);
		m_egSave.SetCellFontBold(TRUE);

		// add through column
		m_egSave.SetTextMatrix(nRow, 2, _T("through"));
		m_egSave.SetCol(2);
		m_egSave.SetRow(nRow);
		m_egSave.SetCellFontBold(TRUE);
		m_egSave.DisableCell(nRow, 2);
	}

	// set through alignment
	m_egSave.SetColAlignment(2, flexAlignCenterCenter);

	// set col widths
	const long col_0 = 2047;
	const long col_2 = 960;
	m_egSave.SetColWidth(0, 0, col_0);
	m_egSave.SetColWidth(2, 0, col_2);

	// determine edit widths
	CRect rect;
	CDC* pDC = m_egSave.GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_egSave.GetClientRect(&rect);
	long nWidth = MulDiv(rect.right, TWIPS_PER_INCH, nLogX);
	long col1_3 = ((nWidth - col_0 - col_2) / 2) - 4;
	m_egSave.SetColWidth(1, 0, col1_3);
	m_egSave.SetColWidth(3, 0, col1_3);

	// set initial position
	m_egSave.SetRow(0);
	m_egSave.SetCol(1);
	OnEnterCellMshfgSave();
}

void CKPSavePg1::OnSize(UINT nType, int cx, int cy) 
{
	baseKPSavePg1::OnSize(nType, cx, cy);
	
	// resize the columns within the grid
	if (m_egSave.GetSafeHwnd())
	{
		long nCol0 = m_egSave.GetColWidth(0, 0);
		long nCol2 = m_egSave.GetColWidth(2, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egSave.GetClientRect(&rect);
		m_egSave.SetColWidth(1, 0, (MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0 - nCol2) / 2);
		m_egSave.SetColWidth(3, 0, (MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0 - nCol2) / 2);
	}	
	
}

BOOL CKPSavePg1::OnInitDialog() 
{
	baseKPSavePg1::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< item(IDC_MSHFG_SAVE, ABSOLUTE_VERT)

		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSavePg1::OnEnterCellMshfgSave() 
{
	CString strRes;

	switch (m_egSave.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_SAVE_158);
		break;
	case 3 :
		strRes.LoadString(IDS_SAVE_159);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSavePg1::OnKeyDownMshfgSave(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	m_egSave.SetRedraw(FALSE);

	// save selection
	long nSaveRow = m_egSave.GetRow();
	long nSaveCol = m_egSave.GetCol();
	long nSaveRowSel = m_egSave.GetRowSel();
	long nSaveColSel = m_egSave.GetColSel();

	CString strClip;

	long i, j;

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		for (i = nSaveRow; i <= nSaveRowSel; ++i)
		{
			for (j = nSaveCol; j <= nSaveColSel; ++j)
			{
				if (m_egSave.GetCellEnabled(i, j))
					m_egSave.SetTextMatrix(i, j, _T(""));
			}
		}
		break;
	}

	// reset selection
	m_egSave.SetRow(nSaveRow);
	m_egSave.SetCol(nSaveCol);
	m_egSave.SetRowSel(nSaveRowSel);
	m_egSave.SetColSel(nSaveColSel);	

	m_egSave.SetRedraw(TRUE);
}

LRESULT CKPSavePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bDenyEdit = FALSE;	// allow edit	

	ASSERT(nID == IDC_MSHFG_SAVE);
	return bDenyEdit;
}

LRESULT CKPSavePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	ASSERT(nID == IDC_MSHFG_SAVE);

	return bMakeChange;
}

BOOL CKPSavePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_SAVE:
		if (!IsContextHelp())
		{
			if (!(m_egSave.GetRowIsVisible(m_egSave.GetRow()) && m_egSave.GetColIsVisible(m_egSave.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSave.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSave.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSave.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSave.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSave.GetMouseCol() : m_egSave.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_SAVE_158);
			break;
		case 3 :
			strRes.LoadString(IDS_SAVE_159);
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
