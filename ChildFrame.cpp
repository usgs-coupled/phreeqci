// ChildFrame.cpp : implementation of the CChildFrame class
//
// $Date: 2001/08/18 00:19:42 $
// $Revision: 1.1.1.32 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "ChildFrame.h"

#include "RichDocIn.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
: m_workSpace(((CMainFrame*)AfxGetMainWnd())->m_wndWorkspaceBar)
{
	// TODO: add member initialization code here
	m_bFirstActivation = true;	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	// open maximized (requires Invalidate() in CView::OnInitialUpdate()
	cs.style |= WS_MAXIMIZE;

	return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.
	if (m_bFirstActivation)
	{
		nCmdShow = SW_SHOWMAXIMIZED;
		m_bFirstActivation  = false;
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// If the workspace is active and (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
	// let the workspace attempt to handle first
	//
	// Note: This is required in addition to CMainFrame::OnCmdMsg for WM_COMMAND
	// see CMDIFrameWnd::OnCommand()
	//
	if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
	{
		if (::IsChild(m_workSpace.m_hWnd, ::GetFocus()))
		{
			ASSERT(m_workSpace.IsVisible()  || m_workSpace.IsFloating());
			if (m_workSpace.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		}
	}	
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


