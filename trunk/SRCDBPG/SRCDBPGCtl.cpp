// SRCDBPGCtl.cpp : Implementation of the CSRCDBPagerCtrl ActiveX Control class.
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SRCDBPG.h"
#include "SRCDBPGCtl.h"
#include "SRCDBPGPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSRCDBPagerCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSRCDBPagerCtrl, COleControl)
	//{{AFX_MSG_MAP(CSRCDBPagerCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSRCDBPagerCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSRCDBPagerCtrl)
	DISP_PROPERTY_NOTIFY(CSRCDBPagerCtrl, "GotoFirstEnabled", m_bGotoFirstEnabled, OnGotoFirstEnabledChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSRCDBPagerCtrl, "GotoLastEnabled", m_bGotoLastEnabled, OnGotoLastEnabledChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSRCDBPagerCtrl, "GotoPrevEnabled", m_bGotoPrevEnabled, OnGotoPrevEnabledChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSRCDBPagerCtrl, "GotoNextEnabled", m_bGotoNextEnabled, OnGotoNextEnabledChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSRCDBPagerCtrl, "GotoNewEnabled", m_bGotoNewEnabled, OnGotoNewEnabledChanged, VT_BOOL)
	DISP_PROPERTY_EX(CSRCDBPagerCtrl, "CaptionWidth", GetCaptionWidth, SetCaptionWidth, VT_I2)
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_ENABLED()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSRCDBPagerCtrl, COleControl)
	//{{AFX_EVENT_MAP(CSRCDBPagerCtrl)
	EVENT_CUSTOM("GotoFirstClicked", FireGotoFirstClicked, VTS_NONE)
	EVENT_CUSTOM("GotoPrevClicked", FireGotoPrevClicked, VTS_NONE)
	EVENT_CUSTOM("GotoNextClicked", FireGotoNextClicked, VTS_NONE)
	EVENT_CUSTOM("GotoLastClicked", FireGotoLastClicked, VTS_NONE)
	EVENT_CUSTOM("GotoNewClicked", FireGotoNewClicked, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSRCDBPagerCtrl, 1)
	PROPPAGEID(CSRCDBPagerPropPage::guid)
END_PROPPAGEIDS(CSRCDBPagerCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSRCDBPagerCtrl, "SRCDBPG.SRCDBPGCtrl.1",
	0x5ee2fcd7, 0xf527, 0x4900, 0xaf, 0x7c, 0x33, 0xbc, 0xc6, 0x6, 0x27, 0xfb)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSRCDBPagerCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSRCDBPG =
		{ 0x2540d29a, 0x5fb9, 0x494d, { 0xa2, 0xa1, 0x7a, 0xd8, 0x5, 0x99, 0xe5, 0x82 } };
const IID BASED_CODE IID_DSRCDBPGEvents =
		{ 0x2b08b6fa, 0x7b7e, 0x43f4, { 0xa1, 0x7b, 0xba, 0x20, 0x4, 0x69, 0x93, 0xc0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSRCDBPGOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSRCDBPagerCtrl, IDS_SRCDBPG, _dwSRCDBPGOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::CSRCDBPagerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSRCDBPagerCtrl

BOOL CSRCDBPagerCtrl::CSRCDBPagerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SRCDBPG,
			IDB_SRCDBPG,
			afxRegApartmentThreading,
			_dwSRCDBPGOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::CSRCDBPagerCtrl - Constructor

const short nDefCaptionWidth = 60;
const UINT nTimerID = 1234;
const UINT uTimerDelay = 300;
const UINT uTimerRepeat = 15;

CSRCDBPagerCtrl::CSRCDBPagerCtrl() :
m_rcFirst(3, 2, 23, 17),
m_rcPrev(23, 2, 43, 17),
m_rcNext(117, 2, 137, 17),
m_rcLast(137, 2, 157, 17),
m_rcNew(157, 2, 177, 17),
m_nCaptionWidth(nDefCaptionWidth)
{
	InitializeIIDs(&IID_DSRCDBPG, &IID_DSRCDBPGEvents);

	// TODO: Initialize your control's instance data here.
	CAPTURED_BTN = NONE;
	DOWN_BTN     = NONE;

	m_rcNext.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);
	m_rcLast.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);
	m_rcNew.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);

	SetInitialSize(180, 18);
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::~CSRCDBPagerCtrl - Destructor

CSRCDBPagerCtrl::~CSRCDBPagerCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::OnDraw - Drawing function

void CSRCDBPagerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

	// fill background
	CBrush brBack(/*RGB(255, 0, 0)*/::GetSysColor(COLOR_3DFACE));
	pdc->FillRect(rcBounds, &brBack);

	//
	// draw GotoFirstButton
	//
	DrawButton(pdc, m_rcFirst.TopLeft(), DOWN_BTN == GOTO_FIRST_BTN);
	if (m_bGotoFirstEnabled && m_bEnabled)
	{
		DrawEndMark(pdc, m_rcFirst.TopLeft(), CSize(5, 4), DOWN_BTN == GOTO_FIRST_BTN);
		DrawLeftArrow(pdc, m_rcFirst.TopLeft(), CSize(8, 4), DOWN_BTN == GOTO_FIRST_BTN);
	}
	else
	{
		DrawDisabledEndMark(pdc, m_rcFirst.TopLeft(), CSize(5, 4), DOWN_BTN == GOTO_FIRST_BTN);
		DrawDisabledLeftArrow(pdc, m_rcFirst.TopLeft(), CSize(8, 4), DOWN_BTN == GOTO_FIRST_BTN);
	}

	//
	// draw GotoPrevButton
	//
	DrawButton(pdc, m_rcPrev.TopLeft(), DOWN_BTN == GOTO_PREV_BTN);
	if (m_bGotoPrevEnabled && m_bEnabled)
	{
		DrawLeftArrow(pdc, m_rcPrev.TopLeft(), CSize(6, 4), DOWN_BTN == GOTO_PREV_BTN);
	}
	else
	{
		DrawDisabledLeftArrow(pdc, m_rcPrev.TopLeft(), CSize(6, 4), DOWN_BTN == GOTO_PREV_BTN);
	}


	//
	// draw caption
	//
	CRect rcText(47, 1, 47 + m_nCaptionWidth + 6, 17);
	pdc->DrawEdge(rcText, EDGE_SUNKEN, BF_TOPLEFT|BF_RIGHT|BF_ADJUST);

	if (m_bEnabled)
	{
		pdc->FillRect(rcText, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_WINDOW)));
	}
	else
	{
		pdc->FillRect(rcText, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DFACE)));
	}

	// get stock property caption
	const CString& strCaption = InternalGetText();

	// create font and select it
	CFont font;
	VERIFY(font.CreatePointFont(80, _T("MS Sans Serif"), pdc));
	CFont* pOldFont = pdc->SelectObject(&font);

    // Set the ForeColor property color and transparent background mode into the device context
    pdc->SetBkMode(TRANSPARENT);

	rcText.InflateRect(-1, 2, -1, -1);
	if (!m_bEnabled)
	{
		pdc->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	}
	pdc->DrawText(strCaption, rcText, DT_SINGLELINE|DT_RIGHT|DT_BOTTOM|DT_END_ELLIPSIS);

// COMMENT: {7/24/2001 8:25:24 PM}	TRACE("Width = %d , Height = %d\n", rcText.Width(), rcText.Height());
// COMMENT: {7/24/2001 8:25:24 PM}	TRACE("Left = %d , Top = %d, Right = %d , Bottom = %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);

	// restore original font
	pdc->SelectObject(pOldFont);
	font.DeleteObject();

	//
	// draw GotoNextButton
	//
	DrawButton(pdc, m_rcNext.TopLeft(), DOWN_BTN == GOTO_NEXT_BTN);
	if (m_bGotoNextEnabled && m_bEnabled)
	{
		DrawRightArrow(pdc, m_rcNext.TopLeft(), CSize(6, 4), DOWN_BTN == GOTO_NEXT_BTN);
	}
	else
	{
		DrawDisabledRightArrow(pdc, m_rcNext.TopLeft(), CSize(6, 4), DOWN_BTN == GOTO_NEXT_BTN);
	}

	//
	// draw GotoLastButton
	//
	DrawButton(pdc, m_rcLast.TopLeft(), DOWN_BTN == GOTO_LAST_BTN);
	if (m_bGotoLastEnabled && m_bEnabled)
	{
		DrawRightArrow(pdc, m_rcLast.TopLeft(), CSize(5, 4), DOWN_BTN == GOTO_LAST_BTN);
		DrawEndMark(pdc, m_rcLast.TopLeft(), CSize(11, 4), DOWN_BTN == GOTO_LAST_BTN);
	}
	else
	{
		DrawDisabledRightArrow(pdc, m_rcLast.TopLeft(), CSize(5, 4), DOWN_BTN == GOTO_LAST_BTN);
		DrawDisabledEndMark(pdc, m_rcLast.TopLeft(), CSize(11, 4), DOWN_BTN == GOTO_LAST_BTN);
	}

	//
	// draw GotoNewButton
	//
	DrawButton(pdc, m_rcNew.TopLeft(), DOWN_BTN == GOTO_NEW_BTN);
	if (m_bGotoNewEnabled && m_bEnabled)
	{
		DrawRightArrow(pdc, m_rcNew.TopLeft(), CSize(3, 4), DOWN_BTN == GOTO_NEW_BTN);
		DrawStar(pdc, m_rcNew.TopLeft(), CSize(9, 4), DOWN_BTN == GOTO_NEW_BTN);
	}
	else
	{
		DrawDisabledRightArrow(pdc, m_rcNew.TopLeft(), CSize(3, 4), DOWN_BTN == GOTO_NEW_BTN);
		DrawDisabledStar(pdc, m_rcNew.TopLeft(), CSize(9, 4), DOWN_BTN == GOTO_NEW_BTN);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::DoPropExchange - Persistence support

void CSRCDBPagerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// Call PX_ functions for each persistent custom property.
	PX_Bool(pPX, _T("GotoFirstEnabled"), m_bGotoFirstEnabled, TRUE);
	PX_Bool(pPX, _T("GotoLastEnabled"), m_bGotoLastEnabled, TRUE);
	PX_Bool(pPX, _T("GotoPrevEnabled"), m_bGotoPrevEnabled, TRUE);
	PX_Bool(pPX, _T("GotoNextEnabled"), m_bGotoNextEnabled, TRUE);
	PX_Bool(pPX, _T("GotoNewEnabled"), m_bGotoNewEnabled, TRUE);

	PX_Short(pPX, _T("CaptionWidth"), m_nCaptionWidth, nDefCaptionWidth);

	if (pPX->IsLoading())
	{
		if (m_nCaptionWidth < 0)
		{
			m_nCaptionWidth = 0;
		}
		m_rcNext.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);
		m_rcLast.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);
		m_rcNew.OffsetRect(m_nCaptionWidth - nDefCaptionWidth, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::OnResetState - Reset control to default state

void CSRCDBPagerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl::AboutBox - Display an "About" box to the user

void CSRCDBPagerCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SRCDBPG);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl message handlers

BOOL CSRCDBPagerCtrl::OnSetExtent(LPSIZEL lpSizeL) 
{
	// Add your specialized code here and/or call the base class

	// This forces the control to be 18 pixels high

	// Use the desktop window to get a DC so we can use
	// CDC::HIMETRICtoDP and CDC::DPtoHIMETRIC
	CWnd *pWnd = CWnd::FromHandle(::GetDesktopWindow());
	CClientDC dc(pWnd);
	
	CSize sz(lpSizeL->cx, lpSizeL->cy);
	dc.HIMETRICtoDP(&sz); // Convert the size to pixels

	//{{BUGFIX 5/30/2002
	{
		// This fixes a bug which occured on \\Southpark which
		// caused the control to display at 22 rather than 18 pixels
		// 
		// And caused the control to be drawn on top of the grid

		CSize sz(0, 18);
		dc.DPtoHIMETRIC(&sz);

		SIZEL newSZ;
		newSZ.cx = lpSizeL->cx;
		newSZ.cy = sz.cy;
		return COleControl::OnSetExtent(&newSZ);
	}
	//}}BUGFIX 5/30/2002

	if (sz.cy != 18)
		return FALSE;

	return COleControl::OnSetExtent(lpSizeL);
}

void CSRCDBPagerCtrl::DrawButton(CDC *pDC, CPoint ptPos, BOOL bDown)
{
	if (bDown)
	{
		CRect rc(ptPos, CSize(20, 15));
		pDC->DrawEdge(rc, EDGE_SUNKEN, BF_BOTTOMRIGHT);
		rc.DeflateRect(0, 0, 1, 1);
		pDC->DrawEdge(rc, BDR_SUNKENINNER, BF_TOPLEFT);
		rc.DeflateRect(1, 1, 1, 1);
		pDC->DrawEdge(rc, BDR_SUNKENOUTER, BF_TOPLEFT);
	}
	else
	{
		CRect rc(ptPos, CSize(20, 15));
		pDC->DrawEdge(rc, EDGE_RAISED, BF_BOTTOMRIGHT);
		rc.DeflateRect(0, 0, 1, 1);
		pDC->DrawEdge(rc, BDR_RAISEDINNER, BF_TOPLEFT);
	}
}

void CSRCDBPagerCtrl::DrawEndMark(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	CRect rc(ptPos, CSize(2, 7));
	rc.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));
}

void CSRCDBPagerCtrl::DrawDisabledEndMark(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	//
	// Draw disabled shadow
	//
	CRect rc0(ptPos, CSize(2, 7));
	rc0.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc0.OffsetRect(1, 1);
	}
	rc0.OffsetRect(1, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	//
	// Draw disabled end mark
	//
	CRect rc(ptPos, CSize(2, 7));
	rc.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));
}

void CSRCDBPagerCtrl::DrawLeftArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	rc.OffsetRect(4, 0);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));
}

void CSRCDBPagerCtrl::DrawDisabledLeftArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	//
	// Draw disabled shadow
	//
	CRect rc0(ptPos, CSize(1, 7));
	rc0.OffsetRect(sizeOffset);
	rc0.OffsetRect(1, 1);
	rc0.OffsetRect(4, 0);
	if (bDown)
	{
		rc0.OffsetRect(1, 1);
	}
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(-1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(-1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(-1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	//
	// Draw disabled arrow
	//
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	rc.OffsetRect(4, 0);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(-1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));
}


void CSRCDBPagerCtrl::DrawRightArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));
}

void CSRCDBPagerCtrl::DrawDisabledRightArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	//
	// Draw disabled shadow
	//
	CRect rc0(ptPos, CSize(1, 7));
	rc0.OffsetRect(sizeOffset);
	rc0.OffsetRect(1, 1);
	if (bDown)
	{
		rc0.OffsetRect(1, 1);
	}
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(1, 0);
	rc0.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));


	//
	// Draw disabled arrow
	//
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 1, 0, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));
}


void CSRCDBPagerCtrl::DrawStar(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	rc.OffsetRect(3, 0);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 2, 0, 2);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(-2, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(-2, 0);
	rc.DeflateRect(0, 1, -1, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(5, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(0, -2);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_BTNTEXT));

	rc.OffsetRect(-4, 0);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_BTNTEXT));

	rc.OffsetRect(0, 4);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_BTNTEXT));

	rc.OffsetRect(4, 0);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_BTNTEXT));
}

void CSRCDBPagerCtrl::DrawDisabledStar(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown)
{
	//
	// Draw disabled shadow
	//
	CRect rc0(ptPos, CSize(1, 7));
	rc0.OffsetRect(sizeOffset);
	rc0.OffsetRect(1, 1);
	if (bDown)
	{
		rc0.OffsetRect(1, 1);
	}
	rc0.OffsetRect(3, 0);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(1, 0);
	rc0.DeflateRect(0, 2, 0, 2);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(-2, 0);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(-2, 0);
	rc0.DeflateRect(0, 1, -1, 1);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(5, 0);
	pDC->FillRect(rc0, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_3DHILIGHT)));

	rc0.OffsetRect(0, -2);
	pDC->SetPixel(rc0.TopLeft(), ::GetSysColor(COLOR_3DHILIGHT));

	rc0.OffsetRect(-4, 0);
	pDC->SetPixel(rc0.TopLeft(), ::GetSysColor(COLOR_3DHILIGHT));

	rc0.OffsetRect(0, 4);
	pDC->SetPixel(rc0.TopLeft(), ::GetSysColor(COLOR_3DHILIGHT));

	rc0.OffsetRect(4, 0);
	pDC->SetPixel(rc0.TopLeft(), ::GetSysColor(COLOR_3DHILIGHT));

	//
	// Draw disabled star
	//
	CRect rc(ptPos, CSize(1, 7));
	rc.OffsetRect(sizeOffset);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	rc.OffsetRect(3, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(1, 0);
	rc.DeflateRect(0, 2, 0, 2);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(-2, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(-2, 0);
	rc.DeflateRect(0, 1, -1, 1);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(5, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_GRAYTEXT)));

	rc.OffsetRect(0, -2);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_GRAYTEXT));

	rc.OffsetRect(-4, 0);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_GRAYTEXT));

	rc.OffsetRect(0, 4);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_GRAYTEXT));

	rc.OffsetRect(4, 0);
	pDC->SetPixel(rc.TopLeft(), ::GetSysColor(COLOR_GRAYTEXT));
}

void CSRCDBPagerCtrl::OnGotoFirstEnabledChanged() 
{
    // force the control to redraw itself
    InvalidateControl(m_rcFirst);

	SetModifiedFlag();
}

void CSRCDBPagerCtrl::OnGotoLastEnabledChanged() 
{
    // force the control to redraw itself
    InvalidateControl(m_rcLast);

	SetModifiedFlag();
}

void CSRCDBPagerCtrl::OnGotoPrevEnabledChanged() 
{
    // force the control to redraw itself
    InvalidateControl(m_rcPrev);

	SetModifiedFlag();
}

void CSRCDBPagerCtrl::OnGotoNextEnabledChanged() 
{
    // force the control to redraw itself
    InvalidateControl(m_rcNext);

	SetModifiedFlag();
}

void CSRCDBPagerCtrl::OnGotoNewEnabledChanged() 
{
    // force the control to redraw itself
    InvalidateControl(m_rcNew);

	SetModifiedFlag();
}

void CSRCDBPagerCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnLButtonDown()\n"));
	// Add your message handler code here and/or call default
	ASSERT(CAPTURED_BTN == NONE);
	ASSERT(DOWN_BTN == NONE);
	CAPTURED_BTN = HitTest(point);
	if (CAPTURED_BTN != NONE)
	{
		LPCRECT lpRect = NULL;
		switch (CAPTURED_BTN)
		{
		case GOTO_FIRST_BTN:
			lpRect = m_rcFirst;
			if (!m_bGotoFirstEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_PREV_BTN:
			lpRect = m_rcPrev;
			if (!m_bGotoPrevEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_NEXT_BTN:
			lpRect = m_rcNext;
			if (!m_bGotoNextEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_LAST_BTN:
			lpRect = m_rcLast;
			if (!m_bGotoLastEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_NEW_BTN:
			lpRect = m_rcNew;
			if (!m_bGotoNewEnabled)
				CAPTURED_BTN = NONE;
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		HWND hWndSave = m_hWnd;

		if (CAPTURED_BTN != NONE)
		{
			SetFocus();

			SetCapture();
			DOWN_BTN = CAPTURED_BTN;
			InvalidateControl(lpRect);

			// Note: Must fire events after SetCapture otherwise
			// WM_CANCELMODE won't be sent if FireEvent displays a MessageBox

			switch (CAPTURED_BTN)
			{
			case GOTO_PREV_BTN:
				FireGotoPrevClicked();
				break;
			case GOTO_NEXT_BTN:
				FireGotoNextClicked();
				break;
			}

			switch (CAPTURED_BTN)
			{
			case GOTO_PREV_BTN:
			case GOTO_NEXT_BTN:
				{
					MSG msg;
					while(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
					{
						TRACE(_T("WM_TIMER removed in OnLButtonDown\n"));
					}
					UINT nID = SetTimer(nTimerID, uTimerDelay, NULL);
					if (nID == nTimerID)
					{
						TRACE(_T("Timer set to uTimerDelay ms\n"));
					}
					else
					{
						TRACE(_T("Unable to set timer to uTimerDelay ms\n"));
					}
				}
				break;
			}
			if (m_hWnd != hWndSave)
			{
				return;
			}
		}
		Default();
		return;
	}

	COleControl::OnLButtonDown(nFlags, point);
}

void CSRCDBPagerCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnLButtonUp()\n"));
	// Add your message handler code here and/or call default
	if (CAPTURED_BTN != NONE)
	{
		DOWN_BTN = NONE;
		ReleaseCapture();
		if (CAPTURED_BTN == GOTO_PREV_BTN || CAPTURED_BTN == GOTO_NEXT_BTN)
		{
			if (KillTimer(nTimerID))
			{
				TRACE(_T("Killed timer in OnLButtonUp\n"));
			}
			else
			{
				TRACE(_T("Unable to kill timer in OnLButtonUp\n"));
			}
			MSG msg;
			while(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
			{
				TRACE(_T("WM_TIMER removed in OnLButtonUp\n"));
			}
		}

		if (CAPTURED_BTN == HitTest(point))
		{
			switch (CAPTURED_BTN)
			{
			case GOTO_FIRST_BTN:
				InvalidateControl(m_rcFirst);
				FireGotoFirstClicked();
				break;

			case GOTO_PREV_BTN:
				InvalidateControl(m_rcPrev);
				// Note: Event fired on mousedown and ontimer
				break;

			case GOTO_NEXT_BTN:
				InvalidateControl(m_rcNext);
				// Note: Event fired on mousedown and ontimer
				break;

			case GOTO_LAST_BTN:
				InvalidateControl(m_rcLast);
				FireGotoLastClicked();
				break;

			case GOTO_NEW_BTN:
				InvalidateControl(m_rcNew);
				FireGotoNewClicked();
				break;

			default:
				ASSERT(FALSE);
				break;
			}
		}
		CAPTURED_BTN = NONE;
	}
	
	COleControl::OnLButtonUp(nFlags, point);
}

void CSRCDBPagerCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	//TRACE("CSRCDBPagerCtrl::OnMouseMove()\n");
	// Add your message handler code here and/or call default
	if (CAPTURED_BTN != NONE)
	{
		if (CAPTURED_BTN == HitTest(point))
		{
			// invalidate rect only if different
			// from previous state
			if (DOWN_BTN == NONE)
			{
				DOWN_BTN = CAPTURED_BTN;
				LPCRECT lpRect = NULL;
				switch (CAPTURED_BTN)
				{
				case GOTO_FIRST_BTN:
					lpRect = m_rcFirst;
					break;
				case GOTO_PREV_BTN:
					lpRect = m_rcPrev;
					break;
				case GOTO_NEXT_BTN:
					lpRect = m_rcNext;
					break;
				case GOTO_LAST_BTN:
					lpRect = m_rcLast;
					break;
				case GOTO_NEW_BTN:
					lpRect = m_rcNew;
					break;
				}
				InvalidateControl(lpRect);
			}
		}
		else
		{
			// invalidate rect only if different
			// from previous state
			if (DOWN_BTN != NONE)
			{
				DOWN_BTN = NONE;
				LPCRECT lpRect = NULL;
				switch (CAPTURED_BTN)
				{
				case GOTO_FIRST_BTN:
					lpRect = m_rcFirst;
					break;
				case GOTO_PREV_BTN:
					lpRect = m_rcPrev;
					break;
				case GOTO_NEXT_BTN:
					lpRect = m_rcNext;
					break;
				case GOTO_LAST_BTN:
					lpRect = m_rcLast;
					break;
				case GOTO_NEW_BTN:
					lpRect = m_rcNew;
					break;
				}
				InvalidateControl(lpRect);
			}
		}
	}
	
	COleControl::OnMouseMove(nFlags, point);
}

void CSRCDBPagerCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnLButtonDblClk() IN\n"));

	// Add your message handler code here and/or call default
	ASSERT(CAPTURED_BTN == NONE);
	CAPTURED_BTN = HitTest(point);
	if (CAPTURED_BTN != NONE)
	{
		LPCRECT lpRect = NULL;
		switch (CAPTURED_BTN)
		{
		case GOTO_FIRST_BTN:
			lpRect = m_rcFirst;
			if (!m_bGotoFirstEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_PREV_BTN:
			lpRect = m_rcPrev;
			if (!m_bGotoPrevEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_NEXT_BTN:
			lpRect = m_rcNext;
			if (!m_bGotoNextEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_LAST_BTN:
			lpRect = m_rcLast;
			if (!m_bGotoLastEnabled)
				CAPTURED_BTN = NONE;
			break;

		case GOTO_NEW_BTN:
			lpRect = m_rcNew;
			if (!m_bGotoNewEnabled)
				CAPTURED_BTN = NONE;
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		HWND hWndSave = m_hWnd;

		if (CAPTURED_BTN != NONE)
		{

			SetFocus();
			TRACE(_T("SetFocus in OnLButtonDblClk\n"));

			SetCapture();
			TRACE(_T("SetCapture in OnLButtonDblClk\n"));

			DOWN_BTN = CAPTURED_BTN;
			InvalidateControl(lpRect);
			TRACE(_T("InvalidateControl in OnLButtonDblClk\n"));

			// Note: Must fire events after SetCapture otherwise
			// WM_CANCELMODE won't be sent if FireEvent displays a MessageBox

			switch (CAPTURED_BTN)
			{
			case GOTO_PREV_BTN:
				FireGotoPrevClicked();
				TRACE(_T("FireGotoPrevClicked in OnLButtonDblClk\n"));
				break;
			case GOTO_NEXT_BTN:
				FireGotoNextClicked();
				TRACE(_T("FireGotoNextClicked in OnLButtonDblClk\n"));
				break;
			}

			switch (CAPTURED_BTN)
			{
			case GOTO_PREV_BTN:
			case GOTO_NEXT_BTN:
				{
					MSG msg;
					while(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
					{
						TRACE(_T("WM_TIMER removed in OnLButtonDown\n"));
					}
					UINT nID = SetTimer(nTimerID, uTimerDelay, NULL);
					if (nID == nTimerID)
					{
						TRACE(_T("Timer set to uTimerDelay ms\n"));
					}
					else
					{
						TRACE(_T("Unable to set timer to uTimerDelay ms\n"));
					}
				}
				break;
			}
			if (m_hWnd != hWndSave)
			{
				TRACE(_T("CSRCDBPagerCtrl::OnLButtonDblClk() OUT(m_hWnd != hWndSave)\n"));
				return;
			}
		}
// COMMENT: {8/1/2001 2:33:52 PM}		Default();
		TRACE(_T("CSRCDBPagerCtrl::OnLButtonDblClk() OUT(Default())\n"));
		return;
	}

	
	COleControl::OnLButtonDblClk(nFlags, point);
	TRACE(_T("CSRCDBPagerCtrl::OnLButtonDblClk() OUT(COleControl::OnLButtonDblClk(nFlags, point))\n"));
}

enum CSRCDBPagerCtrl::BUTTON_TYPE CSRCDBPagerCtrl::HitTest(const CPoint &point)
{
	if (m_rcFirst.PtInRect(point))
	{
		return GOTO_FIRST_BTN;
	}
	if (m_rcPrev.PtInRect(point))
	{
		return GOTO_PREV_BTN;
	}
	if (m_rcNext.PtInRect(point))
	{
		return GOTO_NEXT_BTN;
	}
	if (m_rcLast.PtInRect(point))
	{
		return GOTO_LAST_BTN;
	}
	if (m_rcNew.PtInRect(point))
	{
		return GOTO_NEW_BTN;
	}
	return NONE;
}

short CSRCDBPagerCtrl::GetCaptionWidth() 
{
	InvalidateControl();

	return m_nCaptionWidth;
}

void CSRCDBPagerCtrl::SetCaptionWidth(short nNewValue) 
{
	TRACE(_T("CSRCDBPagerCtrl::SetCaptionWidth(nNewValue = %d)\n"), nNewValue);

	if (nNewValue >= 0 && nNewValue != m_nCaptionWidth)
	{
		m_rcNext.OffsetRect(nNewValue - m_nCaptionWidth, 0);
		m_rcLast.OffsetRect(nNewValue - m_nCaptionWidth, 0);
		m_rcNew.OffsetRect(nNewValue - m_nCaptionWidth, 0);

		m_nCaptionWidth = nNewValue;

		InvalidateControl();
		SetModifiedFlag();
	}
}

void CSRCDBPagerCtrl::OnTimer(UINT nIDEvent) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnTimer(nIDEvent = %d)\n"), nIDEvent);
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == nTimerID)
	{
		if (CAPTURED_BTN == NONE)
		{
			// ignore
			ASSERT(FALSE);
			return;
		}
		else
		{
			// reset timer to uTimerRepeat msec intervals
			UINT nID = SetTimer(nTimerID, uTimerRepeat, NULL);
			if (nID == nTimerID)
			{
				TRACE(_T("Reset timer to uTimerRepeat ms\n"));
			}
			else
			{
				TRACE(_T("Unable to reset timer to uTimerRepeat ms\n"));
			}
		}

		POINT p;
		GetCursorPos(&p);

		ScreenToClient(&p);
		CPoint point(p);
		if (DOWN_BTN != NONE)
		{
			if (CAPTURED_BTN == HitTest(point))
			{
				switch (CAPTURED_BTN)
				{
				case GOTO_PREV_BTN:
					TRACE(_T("FireGotoPrevClicked\n"));
					FireGotoPrevClicked();
					break;

				case GOTO_NEXT_BTN:
					TRACE(_T("FireGotoNextClicked\n"));
					FireGotoNextClicked();
					break;
				}
// COMMENT: {8/1/2001 12:52:32 PM}				//{{
// COMMENT: {8/1/2001 12:52:32 PM}				MSG msg;
// COMMENT: {8/1/2001 12:52:32 PM}				while(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
// COMMENT: {8/1/2001 12:52:32 PM}				{
// COMMENT: {8/1/2001 12:52:32 PM}					TRACE("WM_TIMER removed in switch\n");
// COMMENT: {8/1/2001 12:52:32 PM}				}
// COMMENT: {8/1/2001 12:52:32 PM}				//}}
			}
		}
	}
	COleControl::OnTimer(nIDEvent);
}

void CSRCDBPagerCtrl::OnCancelMode() 
{
	TRACE(_T("CSRCDBPagerCtrl::OnCancelMode()\n"));
	COleControl::OnCancelMode();
	
	// Add your message handler code here
	ASSERT(GetCapture() == NULL); // should have been cancelled in base class

	CAPTURED_BTN = NONE;
	DOWN_BTN     = NONE;
	KillTimer(nTimerID);

	// remove any remaining WM_TIMER msg
	MSG msg;
	while(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
	{
		TRACE(_T("WM_TIMER removed in OnCancelMode\n"));
	}
	InvalidateControl();
}

void CSRCDBPagerCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnKillFocus()\n"));
	COleControl::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	
}

void CSRCDBPagerCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	TRACE(_T("CSRCDBPagerCtrl::OnSetFocus()\n"));
	COleControl::OnSetFocus(pOldWnd);

// COMMENT: {8/1/2001 7:03:40 PM}	CWnd::OnSetFocus(pOldWnd);
// COMMENT: {8/1/2001 7:03:40 PM}
// COMMENT: {8/1/2001 7:03:40 PM}	if (m_pControlSite != NULL)
// COMMENT: {8/1/2001 7:03:40 PM}		m_pControlSite->OnFocus(TRUE);

	
	// TODO: Add your message handler code here
// COMMENT: {8/1/2001 5:11:25 PM}	ASSERT(CAPTURED_BTN == NONE);
// COMMENT: {8/1/2001 5:11:25 PM}	ASSERT(DOWN_BTN == NONE);
	
}
