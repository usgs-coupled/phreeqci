// RichViewOut.cpp : implementation of the CRichViewOut class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "RichDocOut.h"
#include "RichOutCntrItem.h"
#include "RichViewOut.h"

#include "Tree.h"
#include "NodeObject.h"
#include "MainFrame.h"

#include "RichEditLineParserSafe.h"
#include "LocalException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const long CRichViewOut::viewAddedOrRemoved = 2;


/////////////////////////////////////////////////////////////////////////////
// CRichViewOut

IMPLEMENT_DYNCREATE(CRichViewOut, CRichEditView)

BEGIN_MESSAGE_MAP(CRichViewOut, CRichEditView)
	//{{AFX_MSG_MAP(CRichViewOut)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SPECIAL, OnUpdateEditPasteSpecial)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_PROPERTIES, OnUpdateOleEditProperties)
	ON_UPDATE_COMMAND_UI(ID_OLE_INSERT_NEW, OnUpdateOleInsertNew)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Thread messages
	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnUserThreadFinished)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichViewOut construction/destruction

CRichViewOut::CRichViewOut()
: m_workSpace(((CMainFrame*)AfxGetMainWnd())->m_wndWorkspaceBar),
  m_eventAbort(FALSE, TRUE) // bInitiallyOwn, bManualReset
{
	// add construction code here

	// turn off word wrap
	m_nWordWrap = WrapNone;

	// initilize timer
	m_uStatusTimer = 0;

	// initialize thread status
	m_bThreadStillActive = false;

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

CRichViewOut::~CRichViewOut()
{
	ASSERT(m_uStatusTimer == 0);
}

BOOL CRichViewOut::PreCreateWindow(CREATESTRUCT& cs)
{
	//  Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CRichViewOut::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	// Note: File has already been serialized by this point
	ASSERT( m_baseTreeNode == NULL );
	ASSERT( !GetDocument()->GetTitle().IsEmpty() );
	m_baseTreeNode = m_workSpace.AddOutputFileNode(GetDocument()->GetTitle());
	m_baseTreeNode.SetData((DWORD)GetDocument());

	// Determine if this is the first view
	POSITION pos = GetDocument()->GetFirstViewPosition();
	CView* pFirstView = GetDocument()->GetNextView(pos);
	if (this == pFirstView)
	{
		TRACE("First view");
	}
	else
	{
		TRACE("Not First view");
		int nViews = 0;
		POSITION pos = GetDocument()->GetFirstViewPosition();
		while (pos)
		{
			nViews++;
			CView* pView = GetDocument()->GetNextView(pos);
			if (this == pView)
			{
				break;
			}
		}

		CString str;
		str.Format(_T(":%d"), nViews);
		m_baseTreeNode.SetText(GetDocument()->GetTitle() + str);

		//{{ MODIFIED from CDocument::OnOpenDocument(LPCTSTR lpszPathName)
		CFileException fe;
		CFile* pFile = GetDocument()->GetFile(GetDocument()->GetPathName()/*lpszPathName*/,
			CFile::modeRead|CFile::shareDenyWrite, &fe);
		ASSERT( pFile != NULL );

		CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		loadArchive.m_pDocument = GetDocument() /*this*/;
		loadArchive.m_bForceFlat = FALSE;
		TRY
		{
			CWaitCursor wait;

			if (pFile->GetLength() != 0)
				Serialize(loadArchive);     // load me
			loadArchive.Close();
			GetDocument()->ReleaseFile(pFile, FALSE);
		}
		CATCH_ALL(e)
		{
			ASSERT(FALSE);
			GetDocument()->ReleaseFile(pFile, TRUE);
		}
		END_CATCH_ALL

		GetRichEditCtrl().SetModify(FALSE);
		//}} MODIFIED from CDocument::OnOpenDocument(LPCTSTR lpszPathName)
	}

	// Note: If this is a second view probably don't need to parse file
	// again just duplicate the tree

	CThreadParam* pParam = new CThreadParam(m_hWnd, m_baseTreeNode, m_eventAbort);
	m_bThreadStillActive = true;

	// set status timer
	m_uStatusTimer = SetTimer(1, 200, NULL); // set a timer for 200 milliseconds
	OnTimer(m_uStatusTimer);

	::AfxBeginThread(FillTreeThreadProc, pParam);	
}

/////////////////////////////////////////////////////////////////////////////
// CRichViewOut printing

BOOL CRichViewOut::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CRichViewOut::OnDestroy()
{
	// Note: If attempting to open an MRU which no longer exists
	// the thread has not started yet. This implys that it's possible for
	// OnDestroy to be called without OnInitialUpdate ever being called
	//

	// see FillTreeThreadProc and OnUserThreadFinished
	if (m_bThreadStillActive)
	{
		m_eventAbort.SetEvent();
		ASSERT(m_uStatusTimer != 0);
		VERIFY(KillTimer(m_uStatusTimer));
		m_uStatusTimer = 0;
	}
	else
	{
		ASSERT(m_uStatusTimer == 0);
		if (m_baseTreeNode)	// see note above
		{
			m_baseTreeNode.Delete();
		}
	}
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CRichEditView::OnDestroy();
#if _MSC_VER < 1400
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CRichViewOut diagnostics

#ifdef _DEBUG
void CRichViewOut::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CRichViewOut::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CRichDocOut* CRichViewOut::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRichDocOut)));
	return (CRichDocOut*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// inner class CRichViewOut::CThreadParam
CRichViewOut::CThreadParam::CThreadParam(HWND hwndRichEdit, CTreeCtrlNode node, HANDLE hAbort)
: m_hwndRichEdit(hwndRichEdit), m_baseNode(node)
{
	// duplicate handle to thread; this is necessary since the CRichViewOut class
	// may have already been destructed
	VERIFY( ::DuplicateHandle(::GetCurrentProcess(), hAbort, ::GetCurrentProcess(),
		&m_hAbortEvent, 0, FALSE, DUPLICATE_SAME_ACCESS) );

}

CRichViewOut::CThreadParam::~CThreadParam()
{
	// see ctor
	VERIFY( ::CloseHandle(m_hAbortEvent) );
}


/////////////////////////////////////////////////////////////////////////////
// CRichViewOut message handlers

int CRichViewOut::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add your specialized creation code here

	// Set default font
	SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cf);

	// Set readonly
	GetRichEditCtrl().SetOptions(ECOOP_OR, ECO_READONLY);
	
	return 0;
}

void CRichViewOut::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint)
	{
		if (lHint == NODEOBJECT_JUMPTOOUT)
		{
			ASSERT( pHint );
			CNodeObject* pNode = (CNodeObject*) pHint;
			ASSERT( pNode->IsKindOf(RUNTIME_CLASS(CNodeObject)) );
#ifndef USE_ORIGINAL
			// determine if the message is meant for this view
			CTreeCtrlNode node = pNode->m_node;
			while (node.GetLevel() > 1)
			{
				node = node.GetParent();
			}
			ASSERT( node.GetLevel() == 1 );
			if ( node != m_baseTreeNode )
				return;
#endif

			CRichEditCtrl& rich = GetRichEditCtrl();
			int nFirstVisible = rich.GetFirstVisibleLine();
			if (pNode->m_node.GetLevel() == 1)
			{
				rich.LineScroll(0 - nFirstVisible, 0);
			}
			else
			{
				rich.LineScroll(pNode->m_node.GetData() - nFirstVisible, 0);
			}
			GetParentFrame()->ActivateFrame();
			SetFocus();
		}
		if (lHint == CRichViewOut::viewAddedOrRemoved)
		{
			if (m_baseTreeNode == NULL)
				return;
			ASSERT( pSender == NULL );
			UNUSED(pSender);  // unused in release builds

			// Note: If a new window has been added it will not recieve this notification
			// determine which view this is
			int nViews = 0;
			int nThisView = 0;
			POSITION pos = GetDocument()->GetFirstViewPosition();
			while (pos)
			{
				nViews++;
				CView* pView = GetDocument()->GetNextView(pos);
				if (this == pView)
				{
					nThisView = nViews;
				}
			}
			if (nViews == 1)
			{
				m_baseTreeNode.SetText(GetDocument()->GetTitle());
			}
			else
			{
				ASSERT( nViews > 1 );
				CString str;
				str.Format(_T(":%d"), nThisView);
				m_baseTreeNode.SetText(GetDocument()->GetTitle() + str);
			}
		}
	}	
}

LRESULT CRichViewOut::OnUserThreadFinished(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( wParam );
	ASSERT( GetRichEditCtrl().m_hWnd == (HWND)lParam );
	m_bThreadStillActive = false;
	m_baseTreeNode.EnsureVisible();
	m_baseTreeNode.Expand();
	m_baseTreeNode.Select();

	ASSERT(m_uStatusTimer != 0);
	VERIFY(KillTimer(m_uStatusTimer));
	m_uStatusTimer = 0;

	// reset status bar
	::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	return lParam;
}

UINT CRichViewOut::FillTreeThreadProc(LPVOID pParam)
{
	ASSERT( pParam );

	CRichViewOut::CThreadParam*	p = (CRichViewOut::CThreadParam*)pParam;

	CRichViewOut::CThreadParam threadParam(p->m_hwndRichEdit, p->m_baseNode, p->m_hAbortEvent);

	delete pParam;

	try
	{
		Parse(&threadParam);
	}
	catch (CLocalException* pLocalException)
	{
		delete pLocalException;
		TRACE(_T("FillTreeThreadProc aborted\n"));
		threadParam.m_baseNode.Delete();
		return 1; // aborted
	}

	TRACE("sending message\n");
	//
	// SendMessage waits for the window to respond while PostMessage returns immediately
	//
	LRESULT msgReturn = ::SendMessage(threadParam.m_hwndRichEdit, WM_USER_THREAD_FINISHED, 0,
		(LPARAM)threadParam.m_hwndRichEdit);

	if (msgReturn == (LRESULT)threadParam.m_hwndRichEdit)
	{
		//
		// Since the window received the message ok it is now responsible for removing
		// the base node
		//
		TRACE("message recieved\n");
		ASSERT( ::IsWindow(threadParam.m_hwndRichEdit) );
	}
	else
	{
		//
		// Since the window did not received the message we are now responsible for
		// removing the base node
		//
		TRACE("message not recieved\n");
		ASSERT( !::IsWindow(threadParam.m_hwndRichEdit) );
		DWORD dw = ::WaitForSingleObject(threadParam.m_hAbortEvent, 0);
		ASSERT( dw == WAIT_OBJECT_0 );
		threadParam.m_baseNode.Delete();
		return 1;
	}

	TRACE(_T("FillTreeThreadProc finished\n"));
	return 0; // finished
}

void CRichViewOut::Parse(CRichViewOut::CThreadParam* pParam)
{
	ASSERT( pParam );

	CRichEditLineParserSafe parser(pParam);

	CTreeCtrlNode baseNode = pParam->m_baseNode;

	CString strLine;

	for (; parser.GetNextLine(strLine); )
	{
		if (_tcsncmp(LPCTSTR(strLine), _T("Reading data base."), 18) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			break;
		}
	}
	for (; parser.GetNextLine(strLine); )
	{
		if (_tcsncmp(LPCTSTR(strLine), _T("Reading input data for simulation "), 34) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::UNUSED00Node);
			node.SetData((DWORD)parser.GetLineNumber());
			baseNode = node;
			for (; parser.GetNextLine(strLine); )
			{
				if (strLine.IsEmpty())
				{
					continue;
				}
				if (_tcsncmp(LPCTSTR(strLine), _T("End of simulation."), 18) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::lineNode);
					node.SetData((DWORD)parser.GetLineNumber());
					baseNode = pParam->m_baseNode;
					break;
				}
				if (strLine.Compare(_T("End of run.")) == 0)
				{
					break;
				}
				if (strLine.Left(9).Compare(_T("Beginning")) != 0)
				{
					// User print
					if (strLine.GetLength() > 34 && _tcsncmp(LPCTSTR(strLine) + 34, _T("User print"), 10) == 0)
					{
						parser.PushBack();
						ParsePrintAll(parser, node);
					}
					// Description of Solid Solution
					if (_tcsncmp(LPCTSTR(strLine), _T("Description of Solid Solution "), 30) == 0)
					{
						CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::solidSolutionsNode);
						node.SetData((DWORD)parser.GetLineNumber());
					}
					//{{NEW
					// ERROR:
					if (_tcsncmp(LPCTSTR(strLine), _T("ERROR:"), 6) == 0)
					{
						CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::errorNode);
						node.SetData((DWORD)parser.GetLineNumber());
					}
					//}}NEW
					continue;
				}

				//
				// Everything below here starts with "Beginning of ..."
				//

				// Beginning of initial solution calculations.
				if (strLine.GetLength() > 21 && _tcsncmp(LPCTSTR(strLine) + 21, _T("so"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::solutionNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Initial solution "), CTreeCtrlOut::solutionNode, true);
					continue;
				}

				// Beginning of initial exchange-composition calculations.
				if (strLine.GetLength() > 21 && _tcsncmp(LPCTSTR(strLine) + 21, _T("ex"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::exchangeNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Exchange "), CTreeCtrlOut::exchangeNode, false);
					continue;
				}

				// Beginning of initial surface-composition calculations.
				if (strLine.GetLength() > 21 && _tcsncmp(LPCTSTR(strLine) + 21, _T("su"), 2) == 0)
				{
					// Surface 
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::surfaceNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Surface "), CTreeCtrlOut::surfaceNode, true);
					continue;
				}

				// Beginning of initial gas_phase-composition calculations.
				if (strLine.GetLength() > 21 && _tcsncmp(LPCTSTR(strLine) + 21, _T("ga"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::lineNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Gas_Phase "), CTreeCtrlOut::lineNode, false);
					continue;
				}

				// Beginning of batch-reaction calculations.
				if (strLine.GetLength() > 13 && _tcsncmp(LPCTSTR(strLine) + 13, _T("ba"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::reactionNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Reaction step "), CTreeCtrlOut::reactionNode, true);
					continue;
				}

				// Beginning of inverse modeling calculations.
				if (strLine.GetLength() > 13 && _tcsncmp(LPCTSTR(strLine) + 13, _T("in"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::inverseNode);
					node.SetData((DWORD)parser.GetLineNumber());
					// Nothing else to do for now
					// TODO: should there be a pushback here???
					//{{NEW{6/29/2000 1:09:14 PM}
					// ParseBeginning(parser, node, _T("Solution "), CTreeCtrlOut::solutionNode, false);
					//}}NEW{6/29/2000 1:09:14 PM}
					ParseInverse(parser, node);
					continue;
				}

				// Beginning of advection calculations.
				if (strLine.GetLength() > 13 && _tcsncmp(LPCTSTR(strLine) + 13, _T("ad"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::advectionNode);
					node.SetData((DWORD)parser.GetLineNumber());
					ParseBeginning(parser, node, _T("Beginning of advection time step "), CTreeCtrlOut::advectionNode, true);
					continue;
				}

				// Beginning of transport calculations.
				if (strLine.GetLength() > 13 && _tcsncmp(LPCTSTR(strLine) + 13, _T("tr"), 2) == 0)
				{
					CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::transportNode /*lineNode*/);
					node.SetData((DWORD)parser.GetLineNumber());

					// Only one of these
					ParseBeginning(parser, node, _T("Equilibrating initial solutions"), CTreeCtrlOut::transportNode, true);

					ParseBeginning(parser, node, _T("Transport step "), CTreeCtrlOut::transportNode, true);
					continue;
				}

				// Beginning of advection calculations.
				if (strLine.GetLength() == 26 && _tcsncmp(LPCTSTR(strLine) + 13, _T("ru"), 2) == 0)
				{
					continue;
				}

				// all "Beginning of" should have been caught
				ASSERT( FALSE );
			}

		}
		if (strLine == _T("End of run."))
		{
			CTreeCtrlNode node = pParam->m_baseNode.AddTail(strLine, CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			break;
		}
	}
	TRACE("Thread Finished\n");

	return;
}

void CRichViewOut::ParsePrintAll(CRichEditLineParserSafe &parser, CTreeCtrlNode& baseNode)
{
	CString strLine;

	for (; parser.GetNextLine(strLine); )
	{
		if ( strLine.IsEmpty() )
			continue;

		switch ( strLine[0] )
		{
		case _T('B') :
			if(_tcsncmp(strLine, _T("Beginning"), 9) == 0)
			{
				parser.PushBack();
				return;
			}
			continue;
			break;

		case _T('T') :
			if(_tcsncmp(strLine, _T("Transport step "), 15) == 0)
			{
				//{{ NEW {5/3/2001 5:27:34 PM}
				if (strLine.Find(_T("Mixrun"), 15) == -1)
				{
				//}} NEW {5/3/2001 5:27:34 PM}
					parser.PushBack();
					return;
				//{{ NEW {5/3/2001 5:27:34 PM}
				}
				//}} NEW {5/3/2001 5:27:34 PM}
			}
			continue;
			break;

		case _T('E') :
			if(_tcsncmp(strLine, _T("End of simulation."), 18) == 0)
			{
				parser.PushBack();
				return;
			}
			//{{NEW
			if(_tcsncmp(strLine, _T("ERROR:"), 6) == 0)
			{
				parser.PushBack();
				return;
			}
			//}}NEW
			continue;
			break;
		case _T('-') :
			break;
		default :
			continue;
			break;
		}
		
		// User print
		if(strLine.GetLength() > 34 && _tcsncmp(LPCTSTR(strLine) + 34, _T("User print"), 10) == 0)
		{ 
			CTreeCtrlNode node = baseNode.AddTail(_T("User print"), CTreeCtrlOut::userPrintNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Gas phase
		if(strLine.GetLength() > 35 && _tcsncmp(LPCTSTR(strLine) + 35, _T("Gas phase"), 9) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Gas phase"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Phase assemblage
		if(strLine.GetLength() > 31 && _tcsncmp(LPCTSTR(strLine) + 31, _T("Phase assemblage"), 16) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Phase assemblage"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Solid solutions
		if(strLine.GetLength() > 32 && _tcsncmp(LPCTSTR(strLine) + 32, _T("Solid solutions"), 15) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Solid solutions"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Surface composition
		if(strLine.GetLength() > 30 && _tcsncmp(LPCTSTR(strLine) + 30, _T("Surface composition"), 19) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Surface composition"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Exchange composition
		if(strLine.GetLength() > 29 && _tcsncmp(LPCTSTR(strLine) + 29, _T("Exchange composition"), 20) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Exchange composition"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Solution composition
		if(strLine.GetLength() > 29 && _tcsncmp(LPCTSTR(strLine) + 29, _T("Solution composition"), 20) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Solution composition"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Description of solution
		if(strLine.GetLength() > 28 && _tcsncmp(LPCTSTR(strLine) + 28, _T("Description of solution"), 23) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Description of solution"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Redox couples
		if(strLine.GetLength() > 33 && _tcsncmp(LPCTSTR(strLine) + 33, _T("Redox couples"), 13) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Redox couples"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Distribution of species
		if(strLine.GetLength() > 28 && _tcsncmp(LPCTSTR(strLine) + 28, _T("Distribution of species"), 23) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Distribution of species"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			continue;
		}

		// Saturation indices
		if(strLine.GetLength() > 30 && _tcsncmp(LPCTSTR(strLine) + 30, _T("Saturation indices"), 18) == 0)
		{
			CTreeCtrlNode node = baseNode.AddTail(_T("Saturation indices"), CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			break;	// last
		}
	}
}

void CRichViewOut::ParseInverse(CRichEditLineParserSafe &parser, CTreeCtrlNode &baseNode)
{
	CString strLine;
	for (; parser.GetNextLine(strLine); )
	{
		if (_tcsncmp(strLine, _T("Summary of inverse modeling:"), 28) == 0)
		{
			strLine.Replace(_T("\t"), _T("    "));
			CTreeCtrlNode node = baseNode.AddTail(strLine, CTreeCtrlOut::lineNode);
			node.SetData((DWORD)parser.GetLineNumber());
			break;
		}
		else if (_tcsncmp(strLine, _T("End of simulation."), 18) == 0)
		{
			break;
		}
		else if (_tcsncmp(strLine, _T("Beginning"), 9) == 0)
		{
			break;
		}
		else if (_tcsncmp(strLine, _T("Transport step "), 15) == 0)
		{
			break;
		}
		if(_tcsncmp(strLine, _T("ERROR:"), 6) == 0)
		{
			break;
		}
	}
	parser.PushBack();
}

void CRichViewOut::ParseBeginning(CRichEditLineParserSafe &parser, CTreeCtrlNode &baseNode, LPCTSTR lpsz/*CString rStr*/, enum CTreeCtrlOut::ImageIndex image, bool bCallParsePrintAll)
{
	CString strLine;
	/*int nLen = rStr.GetLength();*/
	ASSERT(lpsz != NULL);
	if (lpsz == NULL) return;

	int nLen = lstrlen(lpsz);
	for (; parser.GetNextLine(strLine); )
	{
		/*if (_tcsncmp(strLine, rStr, nLen) == 0)*/
		if (_tcsncmp(strLine, lpsz, nLen) == 0)
		{
			strLine.Replace(_T("\t"), _T("    "));
			CTreeCtrlNode node = baseNode.AddTail(strLine, image);
			node.SetData((DWORD)parser.GetLineNumber());
			if (bCallParsePrintAll)
			{
				ParsePrintAll(parser, node);
				//{{ NEW {5/3/2001 5:27:34 PM}
				if (!node.HasChildren() && (image == CTreeCtrlOut::advectionNode || image == CTreeCtrlOut::transportNode))
				{
					node.Delete();
				}
				//}} NEW {5/3/2001 5:27:34 PM}
			}
		}
		else if (_tcsncmp(strLine, _T("End of simulation."), 18) == 0)
		{
			break;
		}
		else if (_tcsncmp(strLine, _T("Beginning"), 9) == 0)
		{
			break;
		}
		//{{ NEW {5/3/2001 5:27:34 PM}
		else if (_tcsncmp(strLine, _T("Transport step "), 15) == 0)
		{
			break;
		}
		//}} NEW {5/3/2001 5:27:34 PM}
		//{{NEW
		if(_tcsncmp(strLine, _T("ERROR:"), 6) == 0)
		{
			break;
		}
		//}}NEW
	}
	parser.PushBack();
}

void CRichViewOut::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnUpdateEditPasteSpecial(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnUpdateOleEditProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnUpdateOleInsertNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CRichViewOut::OnTimer(UINT nIDEvent) 
{
	// Add your message handler code here and/or call default
	if (nIDEvent == m_uStatusTimer)
	{
		// set status
		CString strStatus;
		strStatus.Format(_T("Loading %s"), GetDocument()->GetPathName());
		CUtil::GetStatusBar().SetWindowText(strStatus);
	}
	
	CRichEditView::OnTimer(nIDEvent);
}
