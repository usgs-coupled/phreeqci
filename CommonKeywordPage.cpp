// CommonKeywordPage.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "CommonKeywordPage.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordPage property page

IMPLEMENT_DYNCREATE(CCommonKeywordPage, baseCommonKeywordPage)

CCommonKeywordPage::CCommonKeywordPage() : baseCommonKeywordPage(CCommonKeywordPage::IDD)
{
	//{{AFX_DATA_INIT(CCommonKeywordPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCommonKeywordPage::CCommonKeywordPage(UINT nIDTemplate, UINT nIDCaption /*= 0*/)
: baseCommonKeywordPage(nIDTemplate, nIDCaption)
{
}

CCommonKeywordPage::CCommonKeywordPage(LPCTSTR lpszTemplateName, UINT nIDCaption /*= 0*/)
: baseCommonKeywordPage(lpszTemplateName, nIDCaption)
{
}

CCommonKeywordPage::~CCommonKeywordPage()
{
}

#ifdef _DEBUG
CCommonKeywordSheet* CCommonKeywordPage::GetSheet() // non-debug version is inline
{
	CCommonKeywordSheet* pSheet = (CCommonKeywordSheet*)GetParent();
	ASSERT_KINDOF(CCommonKeywordSheet, pSheet);
	return pSheet;
}
#endif

void CCommonKeywordPage::DoDataExchange(CDataExchange* pDX)
{
	baseCommonKeywordPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonKeywordPage)
	DDX_Control(pDX, IDC_S_DESC_INPUT, m_btnInputDesc);
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFG_NUM_DESC, m_egNumDesc);
	//}}AFX_DATA_MAP
	DDX_NumDesc(pDX, IDC_MSHFG_NUM_DESC);
}

void CCommonKeywordPage::DDX_NumDesc(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_NUM_DESC);

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		InitNumDesc();
	}

	long nCol = 1;

	if (pDX->m_bSaveAndValidate)
	{
		CString strValue;

		// lower range (m_n_user)
		DDX_GridText(pDX, nIDC, 0, nCol, strValue);
		if (strValue.IsEmpty())
		{
			GetSheet()->m_n_user = 1;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 0, nCol, GetSheet()->m_n_user);

			// Please enter an integer greater than or equal to %1.
			// DDV_MinEqualInt(pDX, GetSheet()->m_n_user, 0);

			// The starting number must be an integer greater than or equal to zero.
			DDV_MinEqualInt(pDX, GetSheet()->m_n_user, 0, IDS_STRING139);
		}

		// upper range (m_n_user_end)
		DDX_GridText(pDX, nIDC, 1, nCol, strValue);
		if (strValue.IsEmpty())
		{
			GetSheet()->m_n_user_end = CCommonKeywordSheet::N_EMPTY;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 1, nCol, GetSheet()->m_n_user_end);

			// Please enter an integer greater than or equal to %1.
			// DDV_MinEqualInt(pDX, GetSheet()->m_n_user_end, GetSheet()->m_n_user);

			// The ending number must be an integer greater than or equal to the starting number.
			DDV_MinEqualInt(pDX, GetSheet()->m_n_user_end, GetSheet()->m_n_user, IDS_STRING138);
		}
	}
	else
	{
		CString strEmpty(_T(""));

		// lower range (m_n_user)
		if (GetSheet()->m_n_user != CCommonKeywordSheet::N_EMPTY)
		{
			DDX_GridText(pDX, nIDC, 0, nCol, GetSheet()->m_n_user);
		}
		else
		{
			DDX_GridText(pDX, nIDC, 0, nCol, strEmpty);
		}

		// upper range (m_n_user_end)
		if (GetSheet()->m_n_user_end != CCommonKeywordSheet::N_EMPTY
			&& 
			GetSheet()->m_n_user != GetSheet()->m_n_user_end)
		{
			DDX_GridText(pDX, nIDC, 1, nCol, GetSheet()->m_n_user_end);
		}
		else
		{
			DDX_GridText(pDX, nIDC, 1, nCol, strEmpty);
		}

		// set intitial starting position
		CMSHFlexGrid* pWnd = (CMSHFlexGrid*)pDX->PrepareOleCtrl(nIDC);
		ASSERT_KINDOF(CMSHFlexGrid, pWnd);
		pWnd->SetRow(0);
		pWnd->SetCol(nCol);

	}

	// description (m_strDesc)
	DDX_GridText(pDX, nIDC, 2, nCol, GetSheet()->m_strDesc);
}

void CCommonKeywordPage::InitNumDesc()
{
	ASSERT( m_egNumDesc.GetSafeHwnd() != NULL );

	// set title column widths
	const long nCol1 = 1500;
	m_egNumDesc.SetColWidth(0, 0, nCol1);

	// set edit column width
	CRect rect;
	CDC* pDC = GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_egNumDesc.GetClientRect(&rect);
	m_egNumDesc.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol1);

	// set grid titles
    m_egNumDesc.SetTextMatrix(0, 0, _T("Starting number"));
    m_egNumDesc.SetTextMatrix(1, 0, _T("Ending number"));
    m_egNumDesc.SetTextMatrix(2, 0, _T("Description"));

	// set bold titles
	m_egNumDesc.SetRow(0), m_egNumDesc.SetCol(0);
	m_egNumDesc.SetCellFontBold(TRUE);
}

BEGIN_MESSAGE_MAP(CCommonKeywordPage, baseCommonKeywordPage)
	//{{AFX_MSG_MAP(CCommonKeywordPage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordPage message handlers

void CCommonKeywordPage::OnSize(UINT nType, int cx, int cy) 
{
	baseCommonKeywordPage::OnSize(nType, cx, cy);
	
	// resize the columns within the num desc grid
	if (m_egNumDesc.GetSafeHwnd())
	{
		long nCol0 = m_egNumDesc.GetColWidth(0, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egNumDesc.GetClientRect(&rect);
		m_egNumDesc.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0);
	}	
}

BEGIN_EVENTSINK_MAP(CCommonKeywordPage, baseCommonKeywordPage)
    //{{AFX_EVENTSINK_MAP(CCommonKeywordPage)
	ON_EVENT(CCommonKeywordPage, IDC_MSHFG_NUM_DESC, -602 /* KeyDown */, OnKeyDownMshfgNumDesc, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordPage event handlers

void CCommonKeywordPage::OnKeyDownMshfgNumDesc(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		m_egNumDesc.SetRedraw(FALSE);
		m_egNumDesc.SetFillStyle(flexFillRepeat);
		m_egNumDesc.SetText(_T(""));
		m_egNumDesc.SetFillStyle(flexFillSingle);
		m_egNumDesc.SetRedraw(TRUE);
		break;
	}
}
