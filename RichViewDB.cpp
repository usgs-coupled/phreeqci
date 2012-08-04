// RichViewDB.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichViewDB.h"

#include "MainFrame.h"

// CRichViewDB

IMPLEMENT_DYNCREATE(CRichViewDB, CRichEditView)

CRichViewDB::CRichViewDB()
{
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
}

CRichViewDB::~CRichViewDB()
{
}

BEGIN_MESSAGE_MAP(CRichViewDB, CRichEditView)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CRichViewDB::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CRichViewDB::OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CRichViewDB::OnUpdateEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SPECIAL, &CRichViewDB::OnUpdateEditPasteSpecial)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_PROPERTIES, &CRichViewDB::OnUpdateOleEditProperties)
	ON_UPDATE_COMMAND_UI(ID_OLE_INSERT_NEW, &CRichViewDB::OnUpdateOleInsertNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, &CRichViewDB::OnUpdateFileSaveAll)
	ON_WM_DESTROY()
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

	// Add your specialized creation code here

	// Set default font
	this->SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cf);

	// Force read-only
	this->GetRichEditCtrl().SetReadOnly(TRUE);

	return 0;
}

void CRichViewDB::OnInitialUpdate()
{
	m_bOnInitialUpdateCalled = true;

	CRichEditView::OnInitialUpdate();

	// Add your specialized code here and/or call the base class

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	ASSERT_VALID(GetDocument());
	CWaitCursor wait;

	// see CChildFrame::PreCreateWindow
	Invalidate(FALSE);

	// update workspace
	((CMainFrame*)AfxGetMainWnd())->GetWorkspaceBar().AddDatabaseDoc(GetDocument());
}

void CRichViewDB::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateEditReplace(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateEditPasteSpecial(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateOleEditProperties(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateOleInsertNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnUpdateFileSaveAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CRichViewDB::OnDestroy()
{
	CRichEditView::OnDestroy();

	// Add your message handler code here
	// update workspace
	((CMainFrame*)AfxGetMainWnd())->GetWorkspaceBar().RemoveDatabaseDoc(this->GetDocument());
}
