// RichViewDB.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichViewDB.h"


// CRichViewDB

IMPLEMENT_DYNCREATE(CRichViewDB, CRichEditView)

CRichViewDB::CRichViewDB()
{
	//{{
	// initialize OnInitialUpdate flag
	m_bOnInitialUpdateCalled = false;

	// turn off word wrap
	m_nWordWrap = WrapNone;

	// set default font
	m_cf.cbSize = sizeof(CHARFORMAT);
	m_cf.dwMask = CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_OFFSET|CFM_COLOR;
	m_cf.dwEffects = CFE_AUTOCOLOR;
	m_cf.yHeight = 200;	// 10 pooints
	m_cf.yOffset = 0;
	m_cf.crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	m_cf.bCharSet = 0;
	m_cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	// Note: CHARFORMAT.szFaceName is char[] even when _UNICODE is defined
	strncpy(m_cf.szFaceName, "Courier New", LF_FACESIZE);
// COMMENT: {7/27/2012 9:26:14 PM}	m_bDraggingText = false;
	//}}
}

CRichViewDB::~CRichViewDB()
{
}

BEGIN_MESSAGE_MAP(CRichViewDB, CRichEditView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CRichViewDB diagnostics

#ifdef _DEBUG
void CRichViewDB::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CRichViewDB::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRichViewDB message handlers

int CRichViewDB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//{{
	// Set default font
	SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cf);
	//}}

	return 0;
}

void CRichViewDB::OnInitialUpdate()
{
	//{{
	m_bOnInitialUpdateCalled = true;
	//}}

	CRichEditView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	//{{
	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
	//}}
}
