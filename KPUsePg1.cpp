// KPUsePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPUsePg1.h"

#include "TreeCtrlIn.h"
#include "Util.h"
#include "DDX_DDV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPUsePg1, baseKPUsePg1)

const CString CKPUsePg1::s_arrStrKeys[] =
{
	_T("equilibrium_phases"),
	_T("exchange"),
	_T("gas_phase"),
	_T("kinetics"),
	_T("mix"),
	_T("reaction"),
	_T("reaction_temperature"),
	_T("solid_solutions"),
	_T("solution"),
	_T("surface"),
};

const CKeyword::type CKPUsePg1::s_arrKeyTypes[] =
{
	CKeyword::K_EQUILIBRIUM_PHASES,
	CKeyword::K_EXCHANGE,
	CKeyword::K_GAS_PHASE,
	CKeyword::K_KINETICS,
	CKeyword::K_MIX,
	CKeyword::K_REACTION,
	CKeyword::K_REACTION_TEMPERATURE,
	CKeyword::K_SOLID_SOLUTIONS,
	CKeyword::K_SOLUTION,
	CKeyword::K_SURFACE,
};

/////////////////////////////////////////////////////////////////////////////
// CKPUsePg1 property page

CKPUsePg1::CKPUsePg1(CTreeCtrlNode simNode) : baseKPUsePg1(CKPUsePg1::IDD)
, m_ranges(simNode, true)
{
	// init to empty
	for (int i = FIRST_ROW; i < LAST_ROW; ++i)
	{
		m_arrNums[i] = N_EMPTY;
		CUtil::CreateRange(m_arrRangeSet[i], m_ranges[s_arrKeyTypes[i]]);
		m_arrRangeSet[i].insert(m_arrRangeSet[i].begin(), _T("none"));
	}

	//{{AFX_DATA_INIT(CKPUsePg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPUsePg1::~CKPUsePg1()
{
}

void CKPUsePg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPUsePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPUsePg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eDesc);
	DDX_Control(pDX, IDC_MSHFG_USE, m_egUse);
	//}}AFX_DATA_MAP
	DDX_Grid(pDX);
}

BEGIN_MESSAGE_MAP(CKPUsePg1, baseKPUsePg1)
	//{{AFX_MSG_MAP(CKPUsePg1)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// specialized grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	// ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)
	// ON_MESSAGE(EGN_KILLFOCUS, OnKillfocusEG)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPUsePg1, baseKPUsePg1)
    //{{AFX_EVENTSINK_MAP(CKPUsePg1)
	ON_EVENT(CKPUsePg1, IDC_MSHFG_USE, 71 /* EnterCell */, OnEnterCellMshfgUse, VTS_NONE)
	ON_EVENT(CKPUsePg1, IDC_MSHFG_USE, -602 /* KeyDown */, OnKeyDownMshfgUse, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPUsePg1::DDX_Grid(CDataExchange *pDX)
{
	 // do one time initialize
	if (m_bFirstSetActive && !pDX->m_bSaveAndValidate)
	{
		InitGrid();
	}

	// get and format data from control
	if (pDX->m_bSaveAndValidate)
	{
		ValidateGrid(pDX);
	}
	// load data to control
	else		
	{
		ExchangeGrid(pDX);
	}
}

void CKPUsePg1::InitGrid()
{
	ASSERT(m_egUse.GetRows() == LAST_ROW);
	// set titles
	for (long i = FIRST_ROW; i < LAST_ROW; ++i)
	{
		m_egUse.SetTextMatrix(i, 0, s_arrStrKeys[i]);
	}

	// bold and centered
	m_egUse.SetCol(0);
	for (long nRow = 0; nRow < m_egUse.GetRows(); ++nRow)
	{
		m_egUse.SetRow(nRow);
		m_egUse.SetCellFontBold(TRUE);
	}

	// set col widths
	const long col0Width = 2047;
	m_egUse.SetColWidth(0, 0, col0Width);

	// set edit column width
	CRect rect;
	CDC* pDC = m_egUse.GetDC();
	int nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_egUse.GetClientRect(&rect);
	// The MulDiv converts rect.right from units of pixels to units of twips
	m_egUse.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nPixelsPerInchX) - col0Width);

	// set initial position
	m_egUse.SetRow(0); m_egUse.SetCol(1);

	// update m_eDesc
	OnEnterCellMshfgUse();
}

BOOL CKPUsePg1::OnInitDialog() 
{
	baseKPUsePg1::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< item(IDC_MSHFG_USE, ABSOLUTE_VERT)

		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPUsePg1::OnSize(UINT nType, int cx, int cy) 
{
	baseKPUsePg1::OnSize(nType, cx, cy);
	
	// resize the columns within the grid
	if (m_egUse.GetSafeHwnd())
	{
		long nCol0 = m_egUse.GetColWidth(0, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egUse.GetClientRect(&rect);
		m_egUse.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0);
	}	
}

void CKPUsePg1::ValidateGrid(CDataExchange* pDX)
{
	UNUSED(pDX);
	CString str;
	int n_user;

	for (long row = m_egUse.GetFixedRows(); row < m_egUse.GetRows(); ++row)
	{
		//{{ NEW 6/12/2000 2:32:05 PM
		DDX_GridText(pDX, IDC_MSHFG_USE, row, 1, str);
		// Note: grid trims white space on entry

		n_user = N_EMPTY;
		if (!str.IsEmpty())
		{
			if (str[0] == _T('N') || str[0] == _T('n'))
			{
				n_user = N_NONE;
			}
			else if (isdigit(str[0]))
			{
				if (::_stscanf(str, _T("%d"), &n_user) < 1)
				{
					DDX_GridFail(pDX, _T("Expected positive number or \"none\"."));
				}
			}
			else
			{
				DDX_GridFail(pDX, _T("Expected positive number or \"none\"."));
			}
		}
		//}} NEW 6/12/2000 2:32:05 PM
		m_arrNums[row] = n_user;
	}
}

void CKPUsePg1::SetRowValue(enum RowIndex ri, int& value)
{
	CString str;
	if (value != N_EMPTY)
	{
		if (value == N_NONE)
		{
			str =  _T("none");
		}
		else
		{
			str.Format("%d", value);
		}
		m_egUse.SetTextMatrix(ri, 1, str);
	}
	else
	{
		m_egUse.SetTextMatrix(ri, 1, _T(""));
	}
}

void CKPUsePg1::ExchangeGrid(CDataExchange* pDX)
{
	UNUSED(pDX);
	for (int i = FIRST_ROW; i < LAST_ROW; ++i)
	{
		SetRowValue(static_cast<RowIndex>(i), m_arrNums[i]);
	}
}

LRESULT CKPUsePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_USE )
	{
		if (FIRST_ROW <= pInfo->item.iRow && pInfo->item.iRow < LAST_ROW)
		{
			pInfo->item.bUseCombo = (CUtil::InsertRange(pInfo->item.hWndCombo, m_arrRangeSet[pInfo->item.iRow]) > 0);
		}
	}
	return bDenyEdit;
}

LRESULT CKPUsePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_USE :
		break;
	}

	return bMakeChange;
}

void CKPUsePg1::OnEnterCellMshfgUse() 
{
	CString strRes;
	switch (m_egUse.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_USE_136);
		break;
	}
	m_eDesc.SetWindowText(strRes);		
}

void CKPUsePg1::OnKeyDownMshfgUse(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		// just fill with empty strings
		m_egUse.SetFillStyle(flexFillRepeat);
		m_egUse.SetText(_T(""));
		m_egUse.SetFillStyle(flexFillSingle);
		break;
	}
}


BOOL CKPUsePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_USE:
		if (!IsContextHelp())
		{
			if (!(m_egUse.GetRowIsVisible(m_egUse.GetRow()) && m_egUse.GetColIsVisible(m_egUse.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egUse.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egUse.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egUse.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egUse.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egUse.GetMouseCol() : m_egUse.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_USE_136);
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
