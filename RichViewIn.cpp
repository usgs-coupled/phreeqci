// RichViewIn.cpp : implementation of the CRichViewIn class
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "RichDocIn.h"
#include "RichInCntrItem.h"
#include "RichViewIn.h"

#include "Tree.h"
#include "MainFrame.h"
#include "NodeObject.h"
#include <Htmlhelp.h>

#include "KeywordParser.h"

#include "RichDocInSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichViewIn

IMPLEMENT_DYNCREATE(CRichViewIn, CRichEditView)

BEGIN_MESSAGE_MAP(CRichViewIn, CRichEditView)
	//{{AFX_MSG_MAP(CRichViewIn)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	// user messages
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichViewIn construction/destruction

CRichViewIn::CRichViewIn()
: m_workSpace(((CMainFrame*)AfxGetMainWnd())->m_wndWorkspaceBar)
{
	// add construction code here

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
	//{{
	// register clipboard format
	m_nTreeCtrlInClipFormat = (CLIPFORMAT)::RegisterClipboardFormat(_T("CTreeCtrlIn"));
	//}}
	m_bDraggingText = false;
}

CRichViewIn::~CRichViewIn()
{
}

BOOL CRichViewIn::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CRichViewIn::OnInitialUpdate()
{
	m_bOnInitialUpdateCalled = true;

	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));


	ASSERT_VALID(GetDocument());
	CWaitCursor wait;
	// see CChildFrame::PreCreateWindow
	Invalidate(FALSE);
	// update workspace
	m_workSpace.AddInputDoc(GetDocument());
}

/////////////////////////////////////////////////////////////////////////////
// CRichViewIn printing

BOOL CRichViewIn::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CRichViewIn::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CRichEditView::OnDestroy();
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}

	// update workspace
	m_workSpace.RemoveInputDoc(GetDocument());
}


/////////////////////////////////////////////////////////////////////////////
// CRichViewIn diagnostics

#ifdef _DEBUG
void CRichViewIn::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CRichViewIn::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CRichDocIn* CRichViewIn::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRichDocIn)));
	return (CRichDocIn*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichViewIn message handlers

int CRichViewIn::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add your specialized creation code here

	// Set default font
	SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cf);

	VERIFY(GetRichEditCtrl().SetOLECallback(&m_xMyRichEditOleCallback));

	// Don't allow files to be dropped as icons into the richedit control
	::DragAcceptFiles(m_hWnd, FALSE);
	
	return 0;
}

HMENU CRichViewIn::GetContextMenu( WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg )
{
	UNUSED(seltyp);
	UNUSED(lpoleobj);
	UNUSED(lpchrg);

	//CRichCntrItem* pItem = GetSelectedItem();
	//if (pItem == NULL || !pItem->IsInPlaceActive())
	//{
		CMenu menuText;
		menuText.LoadMenu(IDR_POPUP_RICH_IN);
		CMenu* pMenuPopup = menuText.GetSubMenu(0);
		menuText.RemoveMenu(0, MF_BYPOSITION);
		return pMenuPopup->Detach();
	//}
	//return NULL;
}

HRESULT CRichViewIn::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat, DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile)
{
	// Called on a paste or drag to determine if the data pasted/dragged should be accepted

	UNUSED(dwReco);
	UNUSED(bReally);
	UNUSED(hMetaFile);
	TRACE("QueryAcceptData In\n");

	ASSERT(lpdataobj != NULL);
	COleDataObject dataobj;
	dataobj.Attach(lpdataobj, FALSE);
	m_bDraggingText = false;
	if (dataobj.IsDataAvailable(CF_TEXT))
	{
		m_bDraggingText = true;
	}
	
	// accept only text
	*lpcfFormat = CF_TEXT;
	return S_OK;
}


void CRichViewIn::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNUSED(pSender);

	//
	// OnInitialUpdate should be called before OnUpdate (OnInitialUpdate calls OnUpdate)
	// If OnInitialUpdate has not been called this OnUpdate is ignored
	//
	if (!m_bOnInitialUpdateCalled)
	{
		// GetDocument()->SetPathName() is called before this view has
		// been initialized by OnInitialUpdate
		ASSERT(lHint == RICHDOCIN_SET_PATH_NAME);
		return;
	}

	switch (lHint)
	{
	// handled locally
	case NODEOBJECT_JUMPTOERROR:
		JumpToError(pHint);
		break;

	// handled by the workspace
	case RICHVIEWIN_EN_CHANGE:
	case RICHDOCIN_LOADERRORFILE:
	case RICHDOCIN_SET_PATH_NAME:
		m_workSpace.UpdateAllInViews(GetDocument(), lHint, pHint);
		break;
	}

}

void CRichViewIn::OnFilePrint() 
{
	// Need to override in order to let friend class CTreeCtrlIn access
	CRichEditView::OnFilePrint();
}

void CRichViewIn::OnFilePrintPreview() 
{
	// Need to override in order to let friend class CTreeCtrlIn access
	CRichEditView::OnFilePrintPreview();
}

void CRichViewIn::OnEditChange()
{
	GetDocument()->UpdateAllViews(this, RICHVIEWIN_EN_CHANGE, NULL);
	m_workSpace.UpdateAllInViews(GetDocument(), RICHVIEWIN_EN_CHANGE, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CRichViewIn::XMyRichEditOleCallback
//
// TODO: this may not be nec 

BEGIN_INTERFACE_MAP(CRichViewIn, CRichEditView)
	// we use IID_IUnknown because richedit doesn't define an IID
	INTERFACE_PART(CRichViewIn, IID_IUnknown, MyRichEditOleCallback)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CRichViewIn::XMyRichEditOleCallback::AddRef()
{
	METHOD_PROLOGUE_EX_(CRichViewIn, MyRichEditOleCallback)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CRichViewIn::XMyRichEditOleCallback::Release()
{
	METHOD_PROLOGUE_EX_(CRichViewIn, MyRichEditOleCallback)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CRichViewIn, MyRichEditOleCallback)
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::GetNewStorage(LPSTORAGE* ppstg)
{
	METHOD_PROLOGUE_EX_(CRichViewIn, MyRichEditOleCallback)

	// Create a flat storage and steal it from the client item
	// the client item is only used for creating the storage
	COleClientItem item;
	item.GetItemStorageFlat();
	*ppstg = item.m_lpStorage;
	HRESULT hRes = E_OUTOFMEMORY;
	if (item.m_lpStorage != NULL)
	{
		item.m_lpStorage = NULL;
		hRes = S_OK;
	}
	pThis->GetDocument()->InvalidateObjectCache();
	return hRes;
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::GetInPlaceContext(
	LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	return pThis->GetWindowContext(lplpFrame, lplpDoc, lpFrameInfo);
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	return pThis->ShowContainerUI(fShow);
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::QueryInsertObject(
	LPCLSID /*lpclsid*/, LPSTORAGE /*pstg*/, LONG /*cp*/)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	pThis->GetDocument()->InvalidateObjectCache();
	return S_OK;
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::DeleteObject(LPOLEOBJECT /*lpoleobj*/)
{
	METHOD_PROLOGUE_EX_(CRichViewIn, MyRichEditOleCallback)
	pThis->GetDocument()->InvalidateObjectCache();
	return S_OK;
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::QueryAcceptData(
	LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD reco,
	BOOL fReally, HGLOBAL hMetaPict)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	return pThis->QueryAcceptData(lpdataobj, lpcfFormat, reco,
		fReally, hMetaPict);
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::GetClipboardData(
	CHARRANGE* lpchrg, DWORD reco, LPDATAOBJECT* lplpdataobj)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	LPDATAOBJECT lpOrigDataObject = NULL;

	// get richedit's data object
	if (FAILED(pThis->m_lpRichEditOle->GetClipboardData(lpchrg, reco,
		&lpOrigDataObject)))
	{
		return E_NOTIMPL;
	}

	// allow changes
	HRESULT hRes = pThis->GetClipboardData(lpchrg, reco, lpOrigDataObject,
		lplpdataobj);

	// if changed then free original object
	if (SUCCEEDED(hRes))
	{
		if (lpOrigDataObject!=NULL)
			lpOrigDataObject->Release();
		return hRes;
	}
	else
	{
		// use richedit's data object
		*lplpdataobj = lpOrigDataObject;
		return S_OK;
	}
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::GetDragDropEffect(
	BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!fDrag) // allowable dest effects
	{
		DWORD dwEffect;
		// check for force link
		if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
			dwEffect = DROPEFFECT_LINK;
		// check for force copy
		else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
			dwEffect = DROPEFFECT_COPY;
		// check for force move
		else if ((grfKeyState & MK_ALT) == MK_ALT)
			dwEffect = DROPEFFECT_MOVE;
		// default -- recommended action is move
		else
			dwEffect = DROPEFFECT_MOVE;
		if (dwEffect & *pdwEffect) // make sure allowed type
			*pdwEffect = dwEffect;
		//{{
		// METHOD_PROLOGUE_EX is needed in order to get pThis
		METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
		if (!pThis->IsDraggingText())
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
		//}}
	}
	return S_OK;
}

STDMETHODIMP CRichViewIn::XMyRichEditOleCallback::GetContextMenu(
	WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg,
	HMENU* lphmenu)
{
	METHOD_PROLOGUE_EX(CRichViewIn, MyRichEditOleCallback)
	HMENU hMenu = pThis->GetContextMenu(seltype, lpoleobj, lpchrg);
	if (hMenu == NULL)
		return E_NOTIMPL;
	*lphmenu = hMenu;
	return S_OK;
}

void CRichViewIn::OnHelp() 
{
	CHARRANGE cr;
	GetRichEditCtrl().GetSel(cr);
	CString strSelection;
	if (cr.cpMin == cr.cpMax)
	{
		// no selection so just get current line
		long nLine = GetRichEditCtrl().LineFromChar(cr.cpMin);
		int nChars = GetRichEditCtrl().LineLength(nLine);

		if (nChars > 0)
		{
			TCHAR *buffer = new TCHAR[nChars + 4];
			nChars = GetRichEditCtrl().GetLine(nLine, buffer, nChars);
			buffer[nChars] = NULL;
			strSelection = buffer;
			delete[] buffer;
			ASSERT(strSelection.GetLength() == nChars);
		}
	}
	else
	{
		strSelection = GetRichEditCtrl().GetSelText();
	}

	// get first token
	strSelection.TrimLeft();
	strSelection = strSelection.SpanExcluding(_T(" \t#\r\n"));

	CString strIndex = CKeyword::GetHelpIndex(CKeyword::GetKeywordType(strSelection));
	if (strIndex.IsEmpty())
	{
		VERIFY(HtmlHelp(::GetDesktopWindow(), _T("phreeqci.chm"), HH_DISPLAY_TOPIC, (DWORD)NULL));
	}
	else
	{
		VERIFY(HtmlHelp(::GetDesktopWindow(), _T("phreeqci.chm"), HH_DISPLAY_TOPIC, (DWORD)(LPCTSTR)strIndex));
	}
}

void CRichViewIn::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (bActivate)
	{
		m_workSpace.UpdateAllInViews(GetDocument(), RICHDOCIN_ONACTIVATE, NULL);
	}
	
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CRichViewIn::JumpToError(CObject *pHint)
{
	ASSERT( pHint );
	CNodeObject* pNode = (CNodeObject*) pHint;
	ASSERT( pNode->IsKindOf(RUNTIME_CLASS(CNodeObject)) );

	CRichEditCtrl& rich = GetRichEditCtrl();
	int nFirstVisible = rich.GetFirstVisibleLine();
	if (pNode->m_node.GetLevel() == 1)
	{
		rich.LineScroll(0 - nFirstVisible, 0);
		rich.SetSel(0, 0);
		SetFocus();
	}
	else
	{
		// select line
		if (pNode->m_node.GetData())
		{
			int nLine = pNode->m_node.GetData();
			int nStart = rich.LineIndex(nLine);
			int nEnd = rich.LineIndex(nLine + 1);
			rich.SetSel(nStart, nEnd);
			SetFocus();
		}
		else
		{
			::MessageBeep(MB_ICONASTERISK);
		}
		rich.LineScroll(pNode->m_node.GetData() - nFirstVisible, 0);
	}
	GetParentFrame()->ActivateFrame();
}

void CRichViewIn::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// overide CRichEditView::OnUpdateEditPaste to only
	// allow text to be pasted
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
}

bool CRichViewIn::IsDraggingText()
{
	return m_bDraggingText;
}
