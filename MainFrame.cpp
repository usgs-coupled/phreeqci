// MainFrame.cpp : implementation of the CMainFrame class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "MainFrame.h"
#include <Htmlhelp.h>

#include "KeywordSheet.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR BARSTATE = _T("Control");

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_HELP_USER, OnHelpUser)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_WARNOVERWRITEINPUT, OnUpdateOptionsWarnoverwriteinput)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_WARNOVERWRITEOUTPUT, OnUpdateOptionsWarnoverwriteoutput)
	ON_COMMAND(ID_OPTIONS_WARNOVERWRITEINPUT, OnOptionsWarnoverwriteinput)
	ON_COMMAND(ID_OPTIONS_WARNOVERWRITEOUTPUT, OnOptionsWarnoverwriteoutput)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_COMMAND(ID_HELP_FACTSHEETFS, OnHelpFactsheet)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	///ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	///ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	///ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INIT_COND, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_INIT_COND, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORWARD, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_FORWARD, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRINT, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_PRINT, OnBarCheck)

	ON_UPDATE_COMMAND_UI(ID_VIEW_STOICH, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STOICH, OnBarCheck)

	// View->Keywords detail level
	ON_UPDATE_COMMAND_UI(ID_KD_NOVICE, OnUpdateKeywordDetail)
	ON_UPDATE_COMMAND_UI(ID_KD_INTERMEDIATE, OnUpdateKeywordDetail)
	ON_UPDATE_COMMAND_UI(ID_KD_PHREAK, OnUpdateKeywordDetail)
	ON_COMMAND_RANGE(ID_KD_NOVICE, ID_KD_PHREAK, OnKeywordDetail)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	if (!m_wndToolBarRun.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect(0,0,0,0), ID_VIEW_RUN) ||
		!m_wndToolBarRun.LoadToolBar(IDR_TB_RUN))
	{
		TRACE0("Failed to create run toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBarRun.SetBarStyle(m_wndToolBarRun.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	if (!m_wndToolInitCond.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect(0,0,0,0), ID_VIEW_INIT_COND) ||
		!m_wndToolInitCond.LoadToolBar(IDR_INIT_COND))
	{
		TRACE0("Failed to create Initial Conditions toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolInitCond.SetBarStyle(m_wndToolInitCond.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	if (!m_wndToolForward.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect(0,0,0,0), ID_VIEW_FORWARD) ||
		!m_wndToolForward.LoadToolBar(IDR_FORWARD))
	{
		TRACE0("Failed to create Forward and Inverse Modeling toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolForward.SetBarStyle(m_wndToolForward.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	if (!m_wndToolPrint.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect(0,0,0,0), ID_VIEW_PRINT) ||
		!m_wndToolPrint.LoadToolBar(IDR_PRINT))
	{
		TRACE0("Failed to create Printing and numerical method toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolPrint.SetBarStyle(m_wndToolPrint.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	//{{
	if (!m_wndToolStoichTherm.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect(0,0,0,0), ID_VIEW_STOICH) ||
		!m_wndToolStoichTherm.LoadToolBar(IDR_STOICH_THERM))
	{
		TRACE0("Failed to create Stoichiometry and thermodynamic data toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolStoichTherm.SetBarStyle(m_wndToolStoichTherm.GetBarStyle() & ~CBRS_HIDE_INPLACE);
	//}}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndToolBarRun) ||
		!m_wndReBar.AddBar(&m_wndToolInitCond, "Initial conditions", NULL, RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP) ||
		!m_wndReBar.AddBar(&m_wndToolForward, "Forward and inverse modeling", NULL, RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP) ||
		!m_wndReBar.AddBar(&m_wndToolPrint, "Printing and numerical method", NULL, RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP) ||
		!m_wndReBar.AddBar(&m_wndToolStoichTherm, "Stoichiometry and thermodynamic data", NULL, RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP)
		)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	// position m_wndToolBarRun next to m_wndToolBar
	m_wndReBar.GetReBarCtrl().MaximizeBand(1);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// see TN031: Control Bars
	ASSERT(AFX_IDW_CONTROLBAR_FIRST + 33 < AFX_IDW_CONTROLBAR_LAST);
	// res 1280 x 1024 CSize(280, 280) 1000
	// res 1024 x 768 CSize(230, 230) 794
	// res 800 x 600 CSize(200, 200) 600
	if (!m_wndWorkspaceBar.Create(_T("Workspace"), this, CSize(200, 200),
		TRUE, AFX_IDW_CONTROLBAR_FIRST + 33))
	{
		TRACE0("Failed to create Workspace\n");
		return -1;      // fail to create
	}

	// Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBarRun.SetBarStyle(m_wndToolBarRun.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolInitCond.SetBarStyle(m_wndToolInitCond.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolForward.SetBarStyle(m_wndToolForward.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolPrint.SetBarStyle(m_wndToolPrint.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolStoichTherm.SetBarStyle(m_wndToolStoichTherm.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	// CBRS_SIZE_DYNAMIC allows the bar to be resized when floating
	m_wndWorkspaceBar.SetBarStyle(m_wndWorkspaceBar.GetBarStyle() |
		/*CBRS_TOOLTIPS | CBRS_FLYBY |*/ CBRS_SIZE_DYNAMIC);

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndWorkspaceBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndWorkspaceBar, AFX_IDW_DOCKBAR_LEFT);

	// by default don't display Print toolbar
	ShowControlBar(&m_wndToolPrint, FALSE, FALSE);

	// by default don't display Stoich/Therm toolbar
	ShowControlBar(&m_wndToolStoichTherm, FALSE, FALSE);

	// load previous state of workspace
	ASSERT( BARSTATE != NULL );
	CSizingControlBar::GlobalLoadState(BARSTATE);
	LoadBarState(BARSTATE);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	// Add your command update UI handler code here

	pCmdUI->SetCheck(m_wndWorkspaceBar.IsVisible() ? BST_CHECKED : BST_UNCHECKED);
}

void CMainFrame::OnViewWorkspace() 
{
	// Add your command handler code here

	ShowControlBar(&m_wndWorkspaceBar, !m_wndWorkspaceBar.IsVisible(), FALSE);
	if (!m_wndWorkspaceBar.IsVisible())
	{
		if(::IsChild(m_wndWorkspaceBar.m_hWnd, ::GetFocus()))
		{
			MDIGetActive()->SetFocus();
		}
		ASSERT(!::IsChild(m_wndWorkspaceBar.m_hWnd, ::GetFocus()));
	}
}

BOOL CMainFrame::DestroyWindow() 
{
	// Add your specialized code here and/or call the base class

	// save state of workspace bar
	ASSERT( BARSTATE != NULL );
	CSizingControlBar::GlobalSaveState(BARSTATE);
	SaveBarState(BARSTATE);
	
	return CMDIFrameWnd::DestroyWindow();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// If the workspace is active and (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
	// let the workspace attempt to handle first
	if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
	{
		if (::IsChild(m_wndWorkspaceBar.m_hWnd, ::GetFocus()))
		{
			ASSERT(m_wndWorkspaceBar.IsVisible() || m_wndWorkspaceBar.IsFloating());
			if (m_wndWorkspaceBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		}
		// FIX: If a keyword sheet is currently displayed let
		// workspace handle
		CWnd* pWnd = GetFocus();
		CWnd* pOwner = pWnd ? pWnd->GetOwner() : NULL;
		if (pOwner && pOwner->IsKindOf(RUNTIME_CLASS(CKeywordSheet)))
		{
			if (m_wndWorkspaceBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		}
	}	
	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);	
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	// Note: This fix doesn't work when the app is minimized
	// but neither does Dev Studio
	static CWnd* pLastFocus = NULL;

	if (nState == WA_INACTIVE)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && !pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pLastFocus = pWnd;
		}
		else
		{
			pLastFocus = NULL;
		}
	}

	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if ((nState == WA_ACTIVE || nState == WA_CLICKACTIVE) && pLastFocus)
	{
		if (::IsWindow(pLastFocus->GetSafeHwnd()))
		{
			pLastFocus->SetFocus();
		}
		else
		{
			pLastFocus = NULL;
		}
	}	
}

void CMainFrame::OnHelpUser() 
{
	// create path to phreeqci.chm
	//
	CPhreeqciApp* pApp = (CPhreeqciApp*)::AfxGetApp();
	CString chm = pApp->m_settings.m_strHelpDirectory;
	chm.Append(_T("phreeqci.chm"));

	HWND hwnd = ::HtmlHelp(::GetDesktopWindow(), chm, HH_DISPLAY_TOPIC, NULL);
	if (!hwnd)
	{
		::AfxMessageBox(_T("Unable to display help."), MB_OK);
	}
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == &m_wndReBar)
	{
		CMenu menu;
		VERIFY( menu.LoadMenu(IDR_REBAR) );

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else
	{
		CMDIFrameWnd::OnContextMenu(pWnd, point);
	}
}

void CMainFrame::OnUpdateOptionsWarnoverwriteinput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteInput);
}

void CMainFrame::OnUpdateOptionsWarnoverwriteoutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteOutput);
}

void CMainFrame::OnOptionsWarnoverwriteinput() 
{
	((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteInput = !((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteInput;
}

void CMainFrame::OnOptionsWarnoverwriteoutput() 
{
	((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteOutput = !((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteOutput;
}

CWorkspaceBar& CMainFrame::GetWorkspaceBar()
{
	return m_wndWorkspaceBar;
}

void CMainFrame::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);	
}

void CMainFrame::OnFileSaveAll() 
{
	// Add your command handler code here
	m_wndWorkspaceBar.GetTreeCtrlIn().SendMessage(WM_COMMAND, ID_FILE_SAVE_ALL, 0);	
}

void CMainFrame::OnHelpFactsheet() 
{
	// create path to fs-031-02.chm
	//
	CPhreeqciApp* pApp = (CPhreeqciApp*)::AfxGetApp();
	CString chm = pApp->m_settings.m_strHelpDirectory;
	chm.Append(_T("fs-031-02.chm"));

	HWND hwnd = ::HtmlHelp(::GetDesktopWindow(), chm, HH_DISPLAY_TOPIC, NULL);
	if (!hwnd)
	{
		::AfxMessageBox(_T("Unable to open fs-031-02.chm."), MB_OK);
	}
}

void CMainFrame::OnUpdateKeywordDetail(CCmdUI* pCmdUI) 
{
	switch (this->m_wndWorkspaceBar.GetKeywordDetail())
	{
	case CTreeCtrlPfw::KD_NOVICE:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_KD_NOVICE ? 1 : 0);
		break;
	case CTreeCtrlPfw::KD_INTERMEDIATE:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_KD_INTERMEDIATE ? 1 : 0);
		break;
	case CTreeCtrlPfw::KD_PHREAK:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_KD_PHREAK ? 1 : 0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CMainFrame::OnKeywordDetail(UINT id) 
{
	switch (id)
	{
	case ID_KD_NOVICE:
		this->m_wndWorkspaceBar.SetKeywordDetail(CTreeCtrlPfw::KD_NOVICE);
		break;
	case ID_KD_INTERMEDIATE:
		this->m_wndWorkspaceBar.SetKeywordDetail(CTreeCtrlPfw::KD_INTERMEDIATE);
		break;
	case ID_KD_PHREAK:
		this->m_wndWorkspaceBar.SetKeywordDetail(CTreeCtrlPfw::KD_PHREAK);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
