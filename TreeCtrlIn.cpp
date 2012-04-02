// TreeCtrlIn.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlIn.h"

//{{
#include "phrqtype.h"            // LDBLE
#include "CommonKeywordSheet.h"  // CCommonKeywordSheet
//}}

#include "RichEditLineParser.h"
#include "RichLogicalLineParser.h"
#include "RichKeywordParser.h"
#include "CStringLineParser.h"

#include "RichDocIn.h"
#include "RichViewIn.h"
#include "SuspendUndo.h"
#include "DelayRedraw.h"
#include "LockWindowUpdate.h"

#include "WorkspaceBar.h"
#include "Util.h"
#include "SaveCurrentDirectory.h"

#include "KeywordSheet.h"
#include "KSTitle.h"
#include "OCKSUse.h"

// keyword sheets
#include "CKSEquilibrium_Phases.h"
#include "CKSGas_Phase.h"
#include "OCKSSave.h"
#include "CKSExchange.h"
#include "CKSMix.h"
#include "CKSReaction.h"
#include "CKSReaction_Temperature.h"
#include "CKSSolution.h"
#include "CKSSolid_Solutions.h"
#include "OCKSRates.h"
#include "CKSInverse.h"
#include "OCKSUserPrint.h"
#include "OCKSUserPunch.h"
#include "SurfaceSheet.h"
#include "KSPrint.h"
#include "KSKinetics.h"
#include "KSIncrement.h"
#include "KSSelectedOutput.h"
#include "OCKSAdvection.h"
#include "OCKSTransport.h"
#include "OCKSSolution_Spread.h"
#include "KSSolutionSpecies.h"
#include "KSSurfaceSpecies.h"
#include "KSExchangeSpecies.h"
#include "KSPhases.h"
#include "KSSolutionMasterSpecies.h"
#include "KSExchangeMasterSpecies.h"
// COMMENT: {2/16/2012 4:52:00 PM}#include "KSSurfaceMasterSpecies.h"
// COMMENT: {2/16/2012 4:52:00 PM}#include "KSKnobs.h"
#include "OCKSCopy.h"
// COMMENT: {2/16/2012 4:52:00 PM}#include "KSPitzer.h"
// COMMENT: {2/16/2012 4:52:00 PM}#include "KSSIT.h"
//{{NEW KEYWORD HERE}}

#include <Htmlhelp.h>
#include <shlwapi.h>   // PathIsRelative PathAddBackslash PathAppend PathCanonicalize

#ifdef SAVE_OLD_IO
extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
#include "phreeqc/src/phqalloc.h"
	int read_number_description (char *ptr, int *n_user, int *n_user_end, char **description);
}
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CDiscardNotifications
{
public:
	CDiscardNotifications(CRichEditView* pView)
	: m_pView(pView)
	{
		ASSERT(pView);
		m_lEventMask = pView->SendMessage(EM_GETEVENTMASK);
		m_pView->SendMessage(EM_SETEVENTMASK, 0, ENM_NONE);
	}
	~CDiscardNotifications()
	{
		m_pView->SendMessage(EM_SETEVENTMASK, 0, m_lEventMask);
	}
protected:
	long m_lEventMask;
	CRichEditView* m_pView;
};

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlIn

IMPLEMENT_DYNAMIC(CTreeCtrlIn, CTreeCtrlEx)

CTreeCtrlIn::CTreeCtrlIn()
{
	// register clipboard format
	m_nIDClipFormat = (CLIPFORMAT)::RegisterClipboardFormat(_T("CTreeCtrlIn"));
	m_pDragImageList = NULL;
	m_hDragTreeItem = 0;
}

CTreeCtrlIn::~CTreeCtrlIn()
{
	//m_OleDropTarget.Revoke();
}


BEGIN_MESSAGE_MAP(CTreeCtrlIn, baseCTreeCtrlIn)
	//{{AFX_MSG_MAP(CTreeCtrlIn)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_DEBUG_SELECT, OnDebugSelect)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_EDIT_KEYWORD, OnUpdateEditKeyword)
	ON_COMMAND(ID_EDIT_KEYWORD, OnEditKeyword)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewProperties)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_DEBUG_INSERT_BEF, OnDebugInsertBef)
	ON_COMMAND(ID_DEBUG_INSERT_AFT, OnDebugInsertAft)
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_RUN_INPUT, OnRunInput)
	ON_UPDATE_COMMAND_UI(ID_RUN_INPUT, OnUpdateRunInput)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_CLOSE_ALL, OnFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALL, OnUpdateFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_RUN_ALL, OnUpdateRunAll)
	ON_COMMAND(ID_RUN_ALL, OnRunAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_COMMAND(ID_GOTO, OnGoto)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateNeedDocSel)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateNeedDocSel)
	// drag operations
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	// node expanding and collapsing
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	// keywords
//{{NEW KEYWORD HERE}}
	ON_UPDATE_COMMAND_UI_RANGE(ID_KEY_END, ID_KEY_SIT, OnUpdateKey)
	ON_COMMAND_RANGE(ID_KEY_END, ID_KEY_SIT, OnKey)
//{{NEW KEYWORD HERE}}
	ON_UPDATE_COMMAND_UI_RANGE(ID_KEY_END_A, ID_KEY_SIT_A, OnUpdateKey)
	ON_COMMAND_RANGE(ID_KEY_END_A, ID_KEY_SIT_A, OnKeyA)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlIn message handlers

int CTreeCtrlIn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseCTreeCtrlIn::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Add your specialized creation code here

	// Create the Image List
	if (!m_ctrlImage.Create(IDB_INPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}
	// set overlay images
	VERIFY(m_ctrlImage.SetOverlayImage(UnSyncOverlayImage, UnSyncState));
	VERIFY(m_ctrlImage.SetOverlayImage(ErrorOverlayImage, ErrorState));

	// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);

	// Create base node
	m_baseNode = InsertItem(_T("Input files"), workImage, workImage);

	// register as a drop target
	m_OleDropTarget.Register(this);

	return 0;
}

// static
//
//
enum CTreeCtrlIn::ImageIndex CTreeCtrlIn::GetImageIndex(enum CKeyword::type nType)
{
	ImageIndex index = genericKeyImage;

	switch (nType)
	{
	case CKeyword::K_END :
		index = endImage;
		break;

	case CKeyword::K_SOLUTION_SPECIES :
		index = solution_speciesImage;
		break;

	case CKeyword::K_SOLUTION_MASTER_SPECIES :
		index = solution_master_speciesImage;
		break;

	case CKeyword::K_SOLUTION :
		index = solutionImage;
		break;

	case CKeyword::K_PHASES :
		index = phasesImage;
		break;

	case CKeyword::K_REACTION :
		index = reactionImage;
		break;

	case CKeyword::K_MIX :
		index = mixImage;
		break;

	case CKeyword::K_USE :
		index = useImage;
		break;

	case CKeyword::K_SAVE :
		index = saveImage;
		break;

	case CKeyword::K_EXCHANGE_SPECIES :
		index = exchange_speciesImage;
		break;

	case CKeyword::K_EXCHANGE_MASTER_SPECIES :
		index = exchange_master_speciesImage;
		break;

	case CKeyword::K_EXCHANGE :
		index = exchangeImage;
		break;

	case CKeyword::K_SURFACE_SPECIES :
		index = surface_speciesImage;
		break;

	case CKeyword::K_SURFACE_MASTER_SPECIES :
		index = surface_master_speciesImage;
		break;

	case CKeyword::K_SURFACE :
		index = surfaceImage;
		break;

	case CKeyword::K_REACTION_TEMPERATURE :
		index = reaction_temperatureImage;
		break;

	case CKeyword::K_INVERSE_MODELING :
		index = inverse_modelingImage;
		break;

	case CKeyword::K_GAS_PHASE :
		index = gas_phaseImage;
		break;

	case CKeyword::K_TRANSPORT :
		index = transportImage;
		break;

	case CKeyword::K_SELECTED_OUTPUT :
		index = selected_outputImage;
		break;

	case CKeyword::K_KNOBS :
		index = knobsImage;
		break;

	case CKeyword::K_PRINT :
		index = printImage;
		break;

	case CKeyword::K_EQUILIBRIUM_PHASES :
		index = equilibrium_phasesImage;
		break;

	case CKeyword::K_TITLE :
		index = titleImage;
		break;

	//
	// New keywords added by version 2
	//

	case CKeyword::K_ADVECTION :
		index = advectionImage;
		break;

	case CKeyword::K_KINETICS :
		index = kineticsImage;
		break;

	case CKeyword::K_INCREMENTAL_REACTIONS :
		index = incremental_reactionsImage;
		break;

	case CKeyword::K_RATES :
		index = ratesImage;
		break;

	case CKeyword::K_USER_PRINT :
		index = user_printImage;
		break;

	case CKeyword::K_USER_PUNCH :
		index = user_punchImage;
		break;

	case CKeyword::K_SOLID_SOLUTIONS :
		index = solid_solutionsImage;
		break;

	case CKeyword::K_SOLUTION_SPREAD :
		index = solution_spreadImage;
		break;

	case CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS :
		index = llnl_aqueous_model_parametersImage;
		break;

	case CKeyword::K_NAMED_ANALYTICAL_EXPRESSION :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPES :
		index = genericKeyImage;
		break;

	case CKeyword::K_CALCULATE_VALUES :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPE_RATIOS :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPE_ALPHAS :
		index = genericKeyImage;
		break;

	case CKeyword::K_COPY :
		index = copyImage;
		break;

	case CKeyword::K_PITZER :
		index = pitzerImage;
		break;

	case CKeyword::K_SIT :
		index = sitImage;
		break;

	default :
// COMMENT: {2/16/2012 11:01:32 PM}		ASSERT(FALSE);	// undefined keyword
		break;
	}

	//// ASSERT(index != genericKeyImage);

	return index;
}

// static
//
//
enum CKeyword::type CTreeCtrlIn::GetKeywordType(enum CTreeCtrlIn::ImageIndex nImageID)
{
	CKeyword::type nType = CKeyword::K_NOT_KEYWORD;

	switch (nImageID)
	{
	case endImage :
		nType = CKeyword::K_END;
		break;

	case solution_speciesImage :
		nType = CKeyword::K_SOLUTION_SPECIES;
		break;

	case solution_master_speciesImage :
		nType = CKeyword::K_SOLUTION_MASTER_SPECIES;
		break;

	case solutionImage :
		nType = CKeyword::K_SOLUTION;
		break;

	case phasesImage :
		nType = CKeyword::K_PHASES;
		break;

	case reactionImage :
		nType = CKeyword::K_REACTION;
		break;

	case mixImage :
		nType = CKeyword::K_MIX;
		break;

	case useImage :
		nType = CKeyword::K_USE;
		break;

	case saveImage :
		nType = CKeyword::K_SAVE;
		break;

	case exchange_speciesImage :
		nType = CKeyword::K_EXCHANGE_SPECIES;
		break;

	case exchange_master_speciesImage :
		nType = CKeyword::K_EXCHANGE_MASTER_SPECIES;
		break;

	case exchangeImage :
		nType = CKeyword::K_EXCHANGE;
		break;

	case surface_speciesImage :
		nType = CKeyword::K_SURFACE_SPECIES;
		break;

	case surface_master_speciesImage :
		nType = CKeyword::K_SURFACE_MASTER_SPECIES;
		break;

	case surfaceImage :
		nType = CKeyword::K_SURFACE;
		break;

	case reaction_temperatureImage :
		nType = CKeyword::K_REACTION_TEMPERATURE;
		break;

	case inverse_modelingImage :
		nType = CKeyword::K_INVERSE_MODELING;
		break;

	case gas_phaseImage :
		nType = CKeyword::K_GAS_PHASE;
		break;

	case transportImage :
		nType = CKeyword::K_TRANSPORT;
		break;

	case selected_outputImage :
		nType = CKeyword::K_SELECTED_OUTPUT;
		break;

	case knobsImage :
		nType = CKeyword::K_KNOBS;
		break;

	case printImage :
		nType = CKeyword::K_PRINT;
		break;

	case equilibrium_phasesImage :
		nType = CKeyword::K_EQUILIBRIUM_PHASES;
		break;

	case titleImage :
		nType = CKeyword::K_TITLE;
		break;

	//
	// New keywords added by version 2
	//

	case advectionImage :
		nType = CKeyword::K_ADVECTION;
		break;

	case kineticsImage :
		nType = CKeyword::K_KINETICS;
		break;

	case incremental_reactionsImage :
		nType = CKeyword::K_INCREMENTAL_REACTIONS;
		break;

	case ratesImage :
		nType = CKeyword::K_RATES;
		break;

	case user_printImage :
		nType = CKeyword::K_USER_PRINT;
		break;

	case user_punchImage :
		nType = CKeyword::K_USER_PUNCH;
		break;

	case solid_solutionsImage :
		nType = CKeyword::K_SOLID_SOLUTIONS;
		break;

	case solution_spreadImage :
		nType = CKeyword::K_SOLUTION_SPREAD;
		break;

	case llnl_aqueous_model_parametersImage :
		nType = CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS;
		break;

	case copyImage :
		nType = CKeyword::K_COPY;
		break;

	case pitzerImage :
		nType = CKeyword::K_PITZER;
		break;

	case sitImage :
		nType = CKeyword::K_SIT;
		break;

	default :
		ASSERT(FALSE);
		break;
	}

	ASSERT(nType != CKeyword::K_NOT_KEYWORD);
	return nType;
}

void CTreeCtrlIn::OnContextMenu(CWnd* pWnd, CPoint point)
{
	ASSERT(pWnd == this);
	UNUSED(pWnd);  // unused in release builds

	// must override OnRButtonDown to recieve this message
	CTreeCtrlNode node = GetSelectedItem();
	{
		if (point.x == -1 && point.y == -1){
			// keystroke invocation
			node.EnsureVisible();
			CRect rect;
			node.GetRect(rect, TRUE);
			ClientToScreen(rect);
			point = rect.TopLeft();
			point.Offset(rect.Height(), rect.Height() / 2);
		}

		CMenu menu;
		VERIFY( menu.LoadMenu(IDR_POPUP_TREE_IN) );

		CMenu* pPopup = NULL;

		switch (node.GetLevel())
		{
		case FileLevel :
			pPopup = menu.GetSubMenu(0);
			ASSERT( pPopup != NULL );
			break;
		case SimLevel :
			pPopup = menu.GetSubMenu(1);
			ASSERT( pPopup != NULL );
			break;
		case KeywordLevel :
			pPopup = menu.GetSubMenu(2);
			ASSERT( pPopup != NULL );
			break;
			//{{
		case WorkLevel :
			pPopup = menu.GetSubMenu(3);
			ASSERT( pPopup != NULL );
			break;
			//}}
		default :
			// do nothing
			return;
			break;
		}

		ASSERT( pPopup != NULL );
		CWnd* pWndPopupOwner = this;

		//{{
		// The command routing doesn't work right when the workspace bar
		// is floating if the while-loop is used

		// while (pWndPopupOwner->GetStyle() & WS_CHILD)
		//	pWndPopupOwner = pWndPopupOwner->GetParent();
		//
		// ASSERT(pWndPopupOwner == AfxGetMainWnd());	// if docked
		//}}

		pWndPopupOwner = AfxGetMainWnd();
		ASSERT(pWndPopupOwner);

		if (pPopup)
		{
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
		}
		else
		{
			::MessageBeep(MB_ICONEXCLAMATION);
		}
	}
}

void CTreeCtrlIn::OnRButtonDown(UINT nFlags, CPoint point)
{
	UNUSED(nFlags);
	CTreeCtrlNode hitNode = HitTest(point);
	if (hitNode != NULL)
	{
		hitNode.Select();
		SetFocus();
	}
	else
	{
		// The next line will cause OnContextMenu to NOT be called
		baseCTreeCtrlIn::OnRButtonDown(nFlags, point);
	}
}

void CTreeCtrlIn::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	OnUpdateEditCopy(pCmdUI);
	OnUpdateEditClear(pCmdUI);
}


void CTreeCtrlIn::OnEditCut()
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode node = GetSelectedItem();
	if (node != NULL)
	{
		int nLevel = node.GetLevel();
		if (nLevel == KeywordLevel || nLevel == SimLevel || nLevel == FileLevel)
		{
			CopyNode(node);
			ClearNode(node);
		}
		else
		{
			// sound warning message
			::MessageBeep(MB_ICONEXCLAMATION);
		}
	}
}

CHARRANGE CTreeCtrlIn::GetRange(CTreeCtrlNode node)const
{
	ASSERT(node);
	ASSERT(node.GetLevel() > WorkLevel);	// must not be root

	CHARRANGE cr = { 0, 0 };
	CTreeCtrlNode fileNode, sim, key, option;
	CString strLabel;
// COMMENT: {6/28/2000 12:55:05 PM}	long cpMin = 0;
// COMMENT: {6/28/2000 12:55:05 PM}	long cpMax = 0;

	fileNode = GetFileNode(node);
	ASSERT(fileNode);
	
	// special case for file node
	if (fileNode == node)
	{
		cr.cpMax = -1;
		return cr;
	}

	// get pointer to document
	CRichEditView* pView = GetRichEditView(fileNode);
	ASSERT(pView);
	ASSERT_KINDOF(CRichEditView, pView);

	CRichEditLineParser richEditLineParser(pView->m_hWnd);
	CRichLogicalLineParser richLogicalLineParser(richEditLineParser);
	CRichKeywordParser richKeywordParser(richLogicalLineParser);

	while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);

	for (sim = fileNode.GetChild(); sim != NULL; sim = sim.GetNextSibling())
	{
		if ( node == sim)
		{
			cr = richKeywordParser.cr;
		}
		for (key = sim.GetChild(); key != NULL; key = key.GetNextSibling())
		{
			strLabel = key.GetText();
			ASSERT( strLabel.CompareNoCase(richKeywordParser.tree_line) == 0 );

			if (node == key)
			{
				cr = richKeywordParser.cr;
			}
			else if (node == sim)
			{
				cr.cpMax = richKeywordParser.cr.cpMax;
			}

			while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);
			for (option = key.GetChild(); option != NULL; option = option.GetNextSibling())
			{
				strLabel = option.GetText();
				ASSERT( strLabel.CompareNoCase(richKeywordParser.tree_line) == 0 );

				if (node == option)
				{
					cr = richKeywordParser.cr;
				}
				else if (node == key || node == sim)
				{
					cr.cpMax = richKeywordParser.cr.cpMax;
				}
				while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);
			}

		}
	}
	return cr;
}

void CTreeCtrlIn::OnDebugSelect()
{
	ASSERT( GetFocus() == this );

	CTreeCtrlNode nodeSelect = GetSelectedItem();
	CHARRANGE cr = GetRange(nodeSelect);

	CRichEditView* pView = GetRichEditDoc(nodeSelect)->GetView();
	ASSERT(pView);
	pView->GetRichEditCtrl().SetSel(cr);

	int nFirstVisible = pView->GetRichEditCtrl().GetFirstVisibleLine();
	int nFirstSelection = pView->GetRichEditCtrl().LineFromChar(cr.cpMin);
	pView->GetRichEditCtrl().LineScroll(nFirstSelection - nFirstVisible, 0);
	pView->GetParentFrame()->ActivateFrame();
	pView->SetFocus();
}

CRichEditDoc* CTreeCtrlIn::GetRichEditDoc(const CTreeCtrlNode &node)const
{
	CTreeCtrlNode fileNode = node;

	if (fileNode.GetLevel() == WorkLevel)
	{
		return NULL;
	}

	while (fileNode.GetLevel() > FileLevel)
	{
		fileNode = fileNode.GetParent();
	}

	ASSERT( fileNode.GetLevel() == FileLevel );

	CRichEditDoc* pDoc = (CRichEditDoc*)fileNode.GetData();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CRichEditDoc)) );

	return pDoc;
}

CRichEditView* CTreeCtrlIn::GetRichEditView(const CTreeCtrlNode &node)const
{
	CRichEditDoc* pDoc = GetRichEditDoc(node);
	if (pDoc == NULL)
	{
		return NULL;
	}
	return pDoc->GetView();
}

void CTreeCtrlIn::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	UINT uFlags;

	CTreeCtrlNode node = HitTest(point, &uFlags);

	if ((node != 0) && (TVHT_ONITEM & uFlags))
	{
		switch (node.GetLevel())
		{
		case KeywordOptionLevel :
			node.GetParent().Select();
			// fall through
		case KeywordLevel :
			OnEditKeyword();
			break;

		case FileLevel :
			GetRichEditDoc(node)->GetView()->GetParentFrame()->ActivateFrame();
			//{{ADDED June 9, 2000
			GetRichEditDoc(node)->GetView()->SetFocus();
			//}}ADDED June 9, 2000
			break;

		default :
			baseCTreeCtrlIn::OnLButtonDblClk(nFlags, point);
			break;
		}
	}
	else
	{
		baseCTreeCtrlIn::OnLButtonDblClk(nFlags, point);
	}
}

void CTreeCtrlIn::OnUpdateEditKeyword(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	if (GetFocus() == this)
	{
		CTreeCtrlNode node = GetSelectedItem();
		if (node != NULL && node.GetLevel() >= KeywordLevel)
		{
			bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CTreeCtrlIn::OnEditKeyword()
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode node = GetSelectedItem();
	
	// set active document
	// required if database is needed
	GetRichEditView(node)->GetParentFrame()->ActivateFrame();

	ASSERT(node.GetLevel() == KeywordLevel);
	if (node.GetLevel() != KeywordLevel) return;

	// set status bar
	CString strStatus;
	strStatus.Format(_T("Loading %s..."), node.GetText());
	CUtil::GetStatusBar().SetWindowText(strStatus);

	CString strNode = GetNodeString(node);
	CString strPrev;

	CKeywordSheet* pKeywordSheet = NULL;
	int imageID = node.GetImageID();
	switch (imageID)
	{
	case titleImage :
		pKeywordSheet = new CKSTitle();
		break;
	case useImage :
		ASSERT(node.GetParent());
		pKeywordSheet = new COCKSUse(NULL, node.GetParent());
		((COCKSUse*)pKeywordSheet)->Edit(node);
		break;
	case equilibrium_phasesImage :
		pKeywordSheet = new CCKSEquilibrium_Phases();
		break;
	case gas_phaseImage :
		ASSERT(node.GetParent());
		pKeywordSheet = new CCKSGas_Phase(NULL, node.GetParent());
		break;
	case saveImage :
		pKeywordSheet = new COCKSSave(NULL, node.GetParent());
		((COCKSSave*)pKeywordSheet)->Edit(node);
		break;
	case exchangeImage :
		pKeywordSheet = new CCKSExchange(NULL, node.GetParent());
		break;
	case mixImage :
		pKeywordSheet = new CCKSMix(NULL, node.GetParent());
		break;
	case reactionImage :
		pKeywordSheet = new CCKSReaction();
		break;
	case reaction_temperatureImage :
		pKeywordSheet = new CCKSReaction_Temperature();
		break;
	case solutionImage :
		pKeywordSheet = new CCKSSolution();
		break;
	case solid_solutionsImage :
		pKeywordSheet = new CCKSSolid_Solutions();
		break;
	case ratesImage :
		pKeywordSheet = new COCKSRates();
		break;
	case inverse_modelingImage :
		pKeywordSheet = new CCKSInverse(NULL, node.GetParent());
		break;
	case user_printImage :
		pKeywordSheet = new COCKSUserPrint();
		break;
	case user_punchImage :
		pKeywordSheet = new COCKSUserPunch();
		break;
	case surfaceImage :
		pKeywordSheet = new CSurfaceSheet(NULL, node.GetParent()); // new CCKSSurface(NULL, node.GetParent());
		break;
	case printImage :
		pKeywordSheet = new CKSPrint();
		break;
	case kineticsImage :
		pKeywordSheet = new CCKSKinetics();
		break;
	case incremental_reactionsImage :
		pKeywordSheet = new CKSIncrement();
		break;
	case selected_outputImage :
		pKeywordSheet = new CKSSelectedOutput();
		break;
	case advectionImage :
		pKeywordSheet = new COCKSAdvection();
		break;
	case transportImage :
		pKeywordSheet = new COCKSTransport();
		strPrev = GetPreviousString(node);
		break;
	case solution_spreadImage :
		pKeywordSheet = new COCKSSolution_Spread();
		break;
	case solution_speciesImage :
		pKeywordSheet = new CKSSolutionSpecies();
		break;
	case surface_speciesImage :
		pKeywordSheet = new CKSSurfaceSpecies();
		break;
	case exchange_speciesImage :
		pKeywordSheet = new CKSExchangeSpecies();
		break;
	case phasesImage :
		pKeywordSheet = new CKSPhases();
		break;
	case solution_master_speciesImage :
		pKeywordSheet = new CKSSolutionMasterSpecies();
		break;
	case exchange_master_speciesImage :
		pKeywordSheet = new CKSExchangeMasterSpecies();
		break;
// COMMENT: {2/16/2012 4:56:57 PM}	case surface_master_speciesImage :
// COMMENT: {2/16/2012 4:56:57 PM}		pKeywordSheet = new CKSSurfaceMasterSpecies();
// COMMENT: {2/16/2012 4:56:57 PM}		break;
// COMMENT: {2/16/2012 4:56:57 PM}	case knobsImage :
// COMMENT: {2/16/2012 4:56:57 PM}		pKeywordSheet = new CKSKnobs();
// COMMENT: {2/16/2012 4:56:57 PM}		break;
	case copyImage :
		pKeywordSheet = new COCKSCopy(NULL, node.GetParent());
		break;
// COMMENT: {2/16/2012 4:56:57 PM}	case pitzerImage :
// COMMENT: {2/16/2012 4:56:57 PM}		pKeywordSheet = new CKSPitzer();
// COMMENT: {2/16/2012 4:56:57 PM}		break;
// COMMENT: {2/16/2012 4:56:57 PM}	case sitImage :
// COMMENT: {2/16/2012 4:56:57 PM}		pKeywordSheet = new CKSSIT();
// COMMENT: {2/16/2012 4:56:57 PM}		break;
	//{{NEW KEYWORD HERE}}
	}

	// load database if needed
	if (pKeywordSheet && pKeywordSheet->bNeedDatabase)
	{
		// determine which simulation node is in (nSim = nIndex + 1)
		CTreeCtrlNode simNode = node.GetParent();
		int nIndex = simNode.GetParent().GetIndex(simNode);
		ASSERT(nIndex != -1);
		((CRichDocIn*)GetRichEditDoc(simNode))->UpdateDatabase(nIndex + 1);
	}

	if (pKeywordSheet)
	{
		if (imageID == transportImage)
		{
			ASSERT(pKeywordSheet->IsKindOf(RUNTIME_CLASS(COCKSTransport)));
			((COCKSTransport*)pKeywordSheet)->Edit2(strNode, strPrev);
		}
		else
		{
			pKeywordSheet->Edit(strNode);
		}

		// reset status bar
		::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

		if (pKeywordSheet->DoModal() == IDOK)
		{
			CTreeCtrlNode newNode;
			CString strReplace = pKeywordSheet->GetString();
			if (strReplace.IsEmpty())
			{
				ClearNode(node);
			}
			else
			{
				newNode = ReplaceNode(node, strReplace);
			}
			// special case for USE
			if (imageID == useImage)
			{
				COCKSUse* pUse = static_cast<COCKSUse*>(pKeywordSheet);
				ASSERT_VALID(pUse);
				std::list<CTreeCtrlNode>::const_iterator nodeIter = pUse->GetDeleteNodes().begin();
				for (; nodeIter != pUse->GetDeleteNodes().end(); ++nodeIter)
				{
					ClearNode((*nodeIter));
				}
			}
			// special case for SAVE
			if (imageID == saveImage)
			{
				COCKSSave* pSave = static_cast<COCKSSave*>(pKeywordSheet);
				ASSERT_VALID(pSave);
				std::list<CTreeCtrlNode>::const_iterator nodeIter = pSave->GetDeleteNodes().begin();
				for (; nodeIter != pSave->GetDeleteNodes().end(); ++nodeIter)
				{
					ClearNode((*nodeIter));
				}
			}
			// special case for COPY
			if (imageID == copyImage)
			{
				COCKSCopy* pCopy = static_cast<COCKSCopy*>(pKeywordSheet);
				ASSERT_VALID(pCopy);
				std::list<CTreeCtrlNode>::const_iterator nodeIter = pCopy->GetDeleteNodes().begin();
				for (; nodeIter != pCopy->GetDeleteNodes().end(); ++nodeIter)
				{
					ClearNode((*nodeIter));
				}
			}
			if (newNode)
			{
				// set selection to beginning of keyword
				CRichEditView* pView = GetRichEditView(newNode);
				long nPos = GetRange(newNode).cpMin;
				pView->GetRichEditCtrl().SetSel(nPos, nPos);
			}
		}
		delete pKeywordSheet;
	}
	else
	{
		// reset status bar
		::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

		CRichEditView* pView = GetRichEditView(node);
		if (pView)
		{
			// select text
			CRichEditCtrl& rich = pView->GetRichEditCtrl();
			CHARRANGE cr = GetRange(node);
			rich.SetSel(cr);

			// scroll into view
			rich.LineScroll(rich.LineFromChar(-1) - rich.GetFirstVisibleLine());
			pView->SetFocus();
		}
		if (imageID != endImage) {
			::AfxMessageBox(_T("This keyword cannot be edited at this time."));
		}
		else {
			::MessageBeep((UINT)-1);
		}
	}
}

void CTreeCtrlIn::OnUpdateViewProperties(CCmdUI* pCmdUI)
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	if (selectedNode && selectedNode != GetRootItem())
	{
		ASSERT(selectedNode.GetLevel() > 0);
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}

void CTreeCtrlIn::OnViewProperties()
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	if (selectedNode && selectedNode.GetLevel() > WorkLevel)
	{
		CRichDocIn* pDoc = (CRichDocIn*)GetRichEditDoc(selectedNode);
		ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)) );
		pDoc->OnViewProperties();
	}
}

CTreeCtrlNode CTreeCtrlIn::ParseTree(CRichEditDoc *pDoc, int nErrors /*= 0*/)
{
	CSaveCurrentDirectory scd(pDoc->GetPathName());

	// set hourglass
	CWaitCursor wait;

	// update status bar
	CString strStatus;
	strStatus.Format(_T("Loading %s"), pDoc->GetTitle());
	CUtil::GetStatusBar().SetWindowText(strStatus);

	// check validity
	ASSERT(pDoc != NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)));
	ASSERT(pDoc->GetView() != NULL);
	ASSERT(pDoc->GetView()->IsKindOf(RUNTIME_CLASS(CRichViewIn)));
	ASSERT(::IsWindow(pDoc->GetView()->m_hWnd));

	// create and set file node
	CTreeCtrlNode fileNode = GetFileNode(pDoc);
	if (fileNode.HasChildren())
	{
		CTreeCtrlNode child = fileNode.GetChild();
		for (; child != NULL; child = fileNode.GetChild())
		{
			VERIFY(child.Delete());
		}
		ASSERT(!fileNode.HasChildren());
	}
	ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);

	if (nErrors == 0)
	{
		RemoveDatabaseKeyword(pDoc);
	}

	// create keyword parser
	CRichEditLineParser lineParser(pDoc->GetView()->m_hWnd);
	CRichLogicalLineParser logicalLineParser(lineParser);
	CRichKeywordParser keyParser(logicalLineParser);


	CString str;
	int nSim = 0;

	// add first sim node
	str.Format(IDS_SIM_FORMAT_135, ++nSim);
	CTreeCtrlNode simNode = fileNode.AddTail(str, simImage);

	if (nErrors != 0)
	{
		// file contains errors do not load
		fileNode.SetState(INDEXTOOVERLAYMASK(ErrorState), TVIS_OVERLAYMASK);
		fileNode.Expand(TVE_COLLAPSE);
		return fileNode;
	}
	//{{
	// This fixes a bug which occurred when opening an existing file
	// containing an error; fixing it and expanding the file node.
	// This caused a cascading event which, for some reason, doesn't
	// occur when the file has not been saved or when the file was 
	// opened successfully once
	else
	{
		fileNode.SetState(INDEXTOOVERLAYMASK(SyncState), TVIS_OVERLAYMASK);
	}
	//}}

	CTreeCtrlNode keyNode;

	while (keyParser.GetLine() != CKeywordParser::typeEof)
	{
		switch (keyParser.type)
		{
		case CKeywordParser::typeKeyword :
			ASSERT( !keyParser.tree_line.IsEmpty() );
			keyNode = simNode.AddTail(keyParser.tree_line, GetImageIndex(keyParser.last_keyword));
			if (keyParser.last_keyword == CKeyword::K_END)
			{
				str.Format(IDS_SIM_FORMAT_135, ++nSim);
				simNode = fileNode.AddTail(str, simImage);
				break;
			}
			//{{
			// break on single line keywords
			if (keyParser.last_keyword == CKeyword::K_SAVE) break;
			if (keyParser.last_keyword == CKeyword::K_USE) break;
			if (keyParser.last_keyword == CKeyword::K_INCREMENTAL_REACTIONS) break;
			//}}

#ifdef SUB_PARSE_SOLUTION_SPECIES
			ASSERT( FALSE ); // Need to fix OnDebugSelect
			if (keyParser.last_keyword == CKeyword::K_SOLUTION_SPECIES)
			{
				while (keyParser.GetLine() != CKeywordParser::typeEof)
				{
					if (keyParser.type == CKeywordParser::typeKeyword)
					{
						keyParser.PushBack();
						break;
					}
					if (keyParser.type != CKeywordParser::typeEmpty)
					{
						ASSERT( keyParser.type == CKeywordParser::typeOk || keyParser.type == CKeywordParser::typeOption);
						CTreeCtrlNode optionNode = keyNode.AddTail(keyParser.tree_line, CTreeCtrlIn::optionImage);
						if (!CKeywordParser::IsSolutionSpeciesOption(keyParser.line))
						{
							while (keyParser.GetLine() != CKeywordParser::typeEof)
							{
								if (keyParser.type == CKeywordParser::typeKeyword)
								{
									break;
								}
								if (keyParser.type != CKeywordParser::typeEmpty)
								{
									if (CKeywordParser::IsSolutionSpeciesOption(keyParser.line))
									{
										optionNode.AddTail(keyParser.tree_line, CTreeCtrlIn::optionImage);
									}
									else
									{
										break;
									}
								}
							}
							keyParser.PushBack();
						}
					}
				}
				break;
			}
#endif // SUB_PARSE_SOLUTION_SPECIES

			while (keyParser.GetLine() != CKeywordParser::typeEof)
			{
				if (keyParser.type == CKeywordParser::typeKeyword)
				{
					keyParser.PushBack();
					break;
				}
				if (keyParser.type != CKeywordParser::typeEmpty)
				{
					keyNode.AddTail(keyParser.tree_line, optionImage);
				}
			}
			break;

		case CKeywordParser::typeEmpty :
			break;

		default :
			ASSERT(!keyParser.tree_line.IsEmpty());
			simNode.AddTail(keyParser.tree_line, optionImage);
			break;
		}
	}

// SEE CTreeCtrlIn::UpdateAllViews
// COMMENT: {6/9/2000 5:16:48 PM}	fileNode.Select();

	fileNode.Expand();
	fileNode.EnsureVisible();

	// reset status bar to idle message
	::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

	return fileNode;
}

//
// returns the file node associated with pDoc
// if pDoc is not associated with any file nodes a file node
// is created
//
CTreeCtrlNode CTreeCtrlIn::GetFileNode(CRichEditDoc *pDoc)
{
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	CTreeCtrlNode fileNode;
	bool bFound = false;
	for (fileNode = m_baseNode.GetChild(); fileNode != NULL; fileNode = fileNode.GetNextSibling())
	{
		if (pDoc == (CRichEditDoc*)fileNode.GetData())
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		// node not found so add it
		fileNode = InsertItem(pDoc->GetTitle(), fileImage, fileImage, m_baseNode);
		fileNode.SetData((DWORD)pDoc);
		ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);
	}
	ASSERT(fileNode && fileNode.GetLevel() == FileLevel);
	return fileNode;
}

//
// returns the file node associated with childNode
//
CTreeCtrlNode CTreeCtrlIn::GetFileNode(const CTreeCtrlNode& childNode)const
{
	CTreeCtrlNode fileNode(childNode);

	if (fileNode == 0) return fileNode;

	if (fileNode.GetLevel() == WorkLevel)
	{
		return CTreeCtrlNode();
	}

	while (fileNode.GetLevel() > FileLevel)
	{
		fileNode = fileNode.GetParent();
	}

	ASSERT(fileNode && fileNode.GetLevel() == FileLevel);
	return fileNode;
}

//
//
void CTreeCtrlIn::OnUpdateNeedDocSel(CCmdUI* pCmdUI)
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	if (selectedNode != NULL && selectedNode != m_baseNode)
	{
		ASSERT(selectedNode.GetLevel() > WorkLevel);
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CTreeCtrlIn::OnFilePrint()
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	ASSERT(selectedNode && selectedNode.GetLevel() == FileLevel);

	CRichViewIn* pView = (CRichViewIn*)GetRichEditView(selectedNode);
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CRichViewIn)));

	// set active document
	pView->GetParentFrame()->ActivateFrame();
	pView->OnFilePrint();
}

void CTreeCtrlIn::OnFilePrintPreview()
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	if (selectedNode != NULL && selectedNode != m_baseNode)
	{
		CRichEditDoc* pDoc = GetRichEditDoc(selectedNode);
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)));

		CRichViewIn* pView = (CRichViewIn*)pDoc->GetView();
		ASSERT(pView->IsKindOf(RUNTIME_CLASS(CRichViewIn)));

		// set active document
		pView->GetParentFrame()->ActivateFrame();

		pView->OnFilePrintPreview();
	}
}

void CTreeCtrlIn::ClearNode(CTreeCtrlNode node)
{
	CRichEditView* pView;
	if (node == 0 || (pView = GetRichEditView(node)) == NULL)
	{
		// nothing to do
		return;
	}

	// turn off richedit notifications
	CDiscardNotifications discard(pView);

	// remove from rich edit control
	CHARRANGE cr = GetRange(node);
	pView->GetRichEditCtrl().SetSel(cr);
	pView->GetRichEditCtrl().Clear();

	CTreeCtrlNode fileNode = GetFileNode(node);

	CString strLabel;

	bool bRenumberSimulations = false;

	// remove from tree view
	switch (node.GetLevel())
	{
	case KeywordLevel:
		// check if node is an END keyword
		if (node.GetImageID() == endImage)
		{
			// get simulation node containing END keyword
			CTreeCtrlNode simNode = node.GetParent();

			CTreeCtrlNode deletedSimNode = simNode.GetNextSibling();

			// END keyword must be followed by a simulation node
			ASSERT(deletedSimNode != NULL);

			// move each keyword in next simulation to current
			// simulation node
			CTreeCtrlNode key = deletedSimNode.GetChild();
			for (; key != NULL; key = key.GetNextSibling())
			{
				CTreeCtrlNode copyKey = simNode.AddTail(key.GetText(), key.GetImageID());
				CTreeCtrlNode option = key.GetChild();
				for (; option != NULL; option = option.GetNextSibling())
				{
					copyKey.AddTail(option.GetText(), option.GetImageID());
				}
				// copy expanded state
				//{{FIX
				// copyKey.SetState(key.GetState(TVIS_EXPANDED), TVIS_EXPANDED);
				UINT uState = key.GetState(TVIS_EXPANDED);
				if (uState & TVIS_EXPANDED)
				{
					ASSERT(copyKey.HasChildren());
					copyKey.Expand(TVE_EXPAND);
				}
				//}}Fix
			}
			bRenumberSimulations = true;
			deletedSimNode.Delete();
		}
		node.Delete();
		break;

	case SimLevel:
		// check if this is the last simulation
		if (node.GetNextSibling() == 0)
		{
			CTreeCtrlNode fileNode = node.GetParent();
			CTreeCtrlNode prevSim = node.GetPrevSibling();

			// save sim info
			CString strLabel = node.GetText();
			int nImageID = node.GetImageID();

			// delete node
			node.Delete();

			// reinsert empty sim node
			fileNode.InsertAfter(strLabel, prevSim, nImageID);
		}
		else
		{
			bRenumberSimulations = true;
			node.Delete();
		}
		break;

	case FileLevel:
		// special case for fileNode level
		ASSERT(node == fileNode);

		// remove all keywords from first sim
		while (node.GetChild().GetChild())
		{
			node.GetChild().GetChild().Delete();
		}

		// remove Sims 2-n
		while(node.GetChild().GetNextSibling())
		{
			node.GetChild().GetNextSibling().Delete();
		}

		ASSERT(node.GetChild()); // verify sim 1
		ASSERT(!node.GetChild().HasChildren());	// verify sim 1 contains no keywords
		ASSERT(!node.GetChild().GetNextSibling());	// verify no more sims
		break;

	default:
		ASSERT(FALSE);
	}

	if (bRenumberSimulations)
	{
		RenumberSimulations(fileNode);
	}
}

//
// copies the text associated with node to the OLE clipboard
//
void CTreeCtrlIn::CopyNode(CTreeCtrlNode node)
{
	ASSERT(node.GetLevel() != WorkLevel);			// can't copy workspace
	ASSERT(node.GetLevel() != KeywordOptionLevel);	// can't copy keyword options

// COMMENT: {6/28/2000 12:55:49 PM}	CRichEditView* pView = GetRichEditView(node);

	CSharedFile globFile;
	CArchive ar(&globFile, CArchive::store);

	// get string from richedit control
	CString string = GetNodeString(node);

	ar << string;
	ar.Close();

	COleDataSource* pSrcItem = new COleDataSource();

	// copy CTreeCtrlIn clip format
	pSrcItem->CacheGlobalData(m_nIDClipFormat, globFile.Detach());

	// copy text clip format
	HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, string.GetLength() + 1);
	LPTSTR pData = (LPTSTR) ::GlobalLock(hData);
	::lstrcpy(pData, (LPCTSTR)string);
	::GlobalUnlock(hData);

#ifdef _UNICODE
	pSrcItem->CacheGlobalData(CF_UNICODETEXT, hData);
#else
	pSrcItem->CacheGlobalData(CF_TEXT, hData);
#endif

	pSrcItem->SetClipboard();
}

//
// pasting onto a keyword node inserts new keyword(s) before selected
// keyword
//
// pasting onto a simulation node inserts new keyword(s) at end of selected
// simulation (before END keyword)
//
// pasting onto a file node inserts new keyword(s) at end of file
//
// returns first keyword node added
//
CTreeCtrlNode CTreeCtrlIn::PasteString(CTreeCtrlNode nodeToPasteTo, CString& rStr)
{
	if (rStr.IsEmpty())
	{
		// nothing to do
		return CTreeCtrlNode();
	}

#ifdef _DEBUG
	CString strNodeToPasteTo = nodeToPasteTo.GetText();
#endif
	ASSERT(nodeToPasteTo);

	// get file node
	CTreeCtrlNode fileNode = GetFileNode(nodeToPasteTo);

	// get richedit view to paste to
	ASSERT(fileNode);
	CRichEditView* pView = GetRichEditView(fileNode);
	ASSERT_VALID(pView);


	// determine keyword node previous to paste node
	//		paste node may be FileNode, SimNode, or KeyNode
	//		cannot be WorkNode or KeyOptionNode
	ASSERT(nodeToPasteTo.GetLevel() > WorkLevel && nodeToPasteTo.GetLevel() < KeywordOptionLevel);
	CTreeCtrlNode prevKeyNode = GetPrevKeywordNode(nodeToPasteTo);
#ifdef _DEBUG
	CString strPrevKeyNode = prevKeyNode.GetText();
#endif

	// get position within richedit view to paste to
	long pos = 0;
	if (prevKeyNode)
	{
		pos = GetRange(prevKeyNode).cpMax;
		CTreeCtrlNode nextKeyNode = GetNextKeywordNode(prevKeyNode);
		if (nextKeyNode)
		{
#ifdef _DEBUG
			CString strNextKeyNode = nodeToPasteTo.GetText();
#endif
			ASSERT(nodeToPasteTo.GetLevel() != KeywordLevel || nextKeyNode == nodeToPasteTo);
			pos = GetRange(nextKeyNode).cpMin;
		}
	}

	// insert clipboard string into richedit view
	{	// avoiding notifications
		CDiscardNotifications discard(pView);
		pView->GetRichEditCtrl().SetSel(pos, pos);

		// this makes sure the pasted text is always
		// on its own line
		long nLine = pView->GetRichEditCtrl().LineFromChar(pos);
		long firstPos = pView->GetRichEditCtrl().LineIndex(nLine);
		if (firstPos == pos)
		{
			pView->GetRichEditCtrl().ReplaceSel(rStr);
		}
		else
		{
			TRACE0("PasteString: Added new line to rStr\n");
			pView->GetRichEditCtrl().ReplaceSel(_T("\r\n") + rStr);
		}
	}

	// determine starting points
	int nSim = 0;
	CTreeCtrlNode simNode;
	CTreeCtrlNode keyNode;
	CTreeCtrlNode optionNode;
	if (prevKeyNode == 0)
	{
		simNode = fileNode.GetChild();
		ASSERT(simNode);	// must be at least one sim
		keyNode = CTreeCtrlNode(TVI_FIRST, this);
	}
	else
	{
		keyNode = prevKeyNode;
		simNode = keyNode.GetParent();
	}
	if (prevKeyNode && prevKeyNode.GetImageID() == endImage)
	{
		keyNode = CTreeCtrlNode(TVI_FIRST, this);
		simNode = prevKeyNode.GetParent().GetNextSibling();
		ASSERT(simNode);	// must be sim following END
	}

	ASSERT(keyNode);
	ASSERT(simNode);

	// create string keyword parser using rStr
	CCStringLineParser cstringLineParser(rStr);
	CLogicalLineParser cstringLogicalLineParser(cstringLineParser);
	CKeywordParser cstringKeywordParser(cstringLogicalLineParser);

	CKeywordParser& keyParser = cstringKeywordParser;

	// now do as ParseTree
	// except where marked as MOD
	// TODO: Modify ParseTree to include MOD and call ParseTree here
	CString str;
	CTreeCtrlNode first;
	while (keyParser.GetLine() != CKeywordParser::typeEof)
	{
		switch (keyParser.type)
		{
		case CKeywordParser::typeKeyword :
			ASSERT( !keyParser.tree_line.IsEmpty() );
			//{{MOD
			// keyNode = simNode.AddTail(keyParser.tree_line, GetImageIndex(keyParser.last_keyword));
			keyNode = simNode.InsertAfter(keyParser.tree_line, keyNode, GetImageIndex(keyParser.last_keyword));
			if (!first)
			{
				first = keyNode;
			}
			//}}MOD
			if (keyParser.last_keyword == CKeyword::K_END)
			{
				str.Format(IDS_SIM_FORMAT_135, ++nSim);
				//{{MOD
				// simNode = fileNode.AddTail(str, simImage);
				UINT uState = simNode.GetState(TVIS_EXPANDED);
				simNode = fileNode.InsertAfter(keyParser.tree_line, simNode, simImage);
				//{{FIXMOD see ReplaceNode
				// simNode.SetState(uState, TVIS_EXPANDED);
				//}}FIXMOD see ReplaceNode
				//}}MOD

				//{{
				// move keywords following END to next simulation
				CTreeCtrlNode nextKeyNode = keyNode.GetNextSibling();
				for (; nextKeyNode != 0; nextKeyNode = keyNode.GetNextSibling())
				{
					CTreeCtrlNode movedKey = simNode.AddTail(nextKeyNode.GetText(), nextKeyNode.GetImageID());
					if (nextKeyNode == nodeToPasteTo)
					{
						nodeToPasteTo = movedKey;
					}
					CTreeCtrlNode nextOptionNode = nextKeyNode.GetChild();
					for (; nextOptionNode; nextOptionNode = nextOptionNode.GetNextSibling())
					{
						movedKey.AddTail(nextOptionNode.GetText(), nextOptionNode.GetImageID());
					}
					// copy expanded state
					//{{FIXMOD see ReplaceNode
					// movedKey.SetState(nextKeyNode.GetState(TVIS_EXPANDED), TVIS_EXPANDED);
					UINT uState = nextKeyNode.GetState(TVIS_EXPANDED);
					if ((uState & TVIS_EXPANDED) && movedKey.HasChildren())
					{
						movedKey.Expand(TVE_EXPAND);
					}
					//}}FIXMOD see ReplaceNode
					if (m_hDragTreeItem && m_hDragTreeItem == (HTREEITEM)nextKeyNode)
					{
						// need to reset m_hDragTreeItem
						m_hDragTreeItem = (HTREEITEM)movedKey;
					}
					nextKeyNode.Delete();
				}
				//{{FIXMOD
				if ((uState & TVIS_EXPANDED) && simNode.HasChildren())
				{
					simNode.Expand(TVE_EXPAND);
				}
				//}}FIXMOD

				// reset keyNode to be first child
				keyNode = CTreeCtrlNode(TVI_FIRST, this);
				RenumberSimulations(fileNode);
				//}}
				break;
			}

			//{{
			// break on single line keywords
			if (keyParser.last_keyword == CKeyword::K_SAVE) break;
			if (keyParser.last_keyword == CKeyword::K_USE) break;
			if (keyParser.last_keyword == CKeyword::K_INCREMENTAL_REACTIONS) break;
			//}}

			while (keyParser.GetLine() != CKeywordParser::typeEof)
			{
				if (keyParser.type == CKeywordParser::typeKeyword)
				{
					keyParser.PushBack();
					break;
				}
				if (keyParser.type != CKeywordParser::typeEmpty)
				{
					keyNode.AddTail(keyParser.tree_line, optionImage);
				}
			}
			break;

		case CKeywordParser::typeEmpty :
			break;

		default :
			ASSERT(!keyParser.tree_line.IsEmpty());
			//{{MOD
			// simNode.AddTail(keyParser.tree_line, optionImage);
			keyNode = simNode.InsertAfter(keyParser.tree_line, keyNode, optionImage);
			//}}MOD
			if (!first)
			{
				first = keyNode;
			}
			break;
		}
	}
	RenumberSimulations(fileNode);
	nodeToPasteTo.Select();
	RedrawWindow();
	ASSERT(first);
	return first;
}

CTreeCtrlNode CTreeCtrlIn::ReplaceNode(CTreeCtrlNode nodeToReplace, CString& rStr)
{
	// make sure string ends with a newline
	int nLen = rStr.GetLength();
	ASSERT(nLen > 1 && rStr[nLen - 1] == _T('\n') && rStr[nLen - 2] == _T('\r'));

	CRichEditView* pView = GetRichEditView(nodeToReplace);

	// Don't record undo/redo
	CSuspendUndo suspend(pView->GetRichEditCtrl());

	// delay tree rendering
	CLockWindowUpdate lwu(this);

	// delay richedit rendering
	CDelayRedraw dr(pView);
	
	// paste node
	CTreeCtrlNode newNode = PasteString(nodeToReplace, rStr);

	// clear node saving expanded state
	UINT uState = nodeToReplace.GetState(TVIS_EXPANDED);
	ClearNode(nodeToReplace);

	if (newNode)
	{
		newNode.Select();
	}
	else
	{
		return newNode;
	}

	// copy expanded state
	if ((uState & TVIS_EXPANDED) && newNode.HasChildren())
	{
		newNode.Expand(TVE_EXPAND);
		// Note:
		// SetState(XX, TVIS_EXPANDED) causes problems:
		// the child items and items below them cannot be selected
		// until newNode is collapsed and expanded this may have
		// something to do with TVIS_EXPANDEDONCE state
		//
		// newNode.SetState(uState, TVIS_EXPANDED);
		// VERIFY(newNode.SetState(uState, TVIS_EXPANDED);		
	}

	return newNode;
}

void CTreeCtrlIn::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	CTreeCtrlNode selectedNode = GetSelectedItem();

	BOOL bEnable = FALSE;

	switch (selectedNode.GetLevel())
	{
	case KeywordOptionLevel:
		// cannot delete keyword options
		break;

	case KeywordLevel:
		bEnable = TRUE;
		break;

	case SimLevel:
		// cannot delete empty simulation
		if (selectedNode.HasChildren())
		{
			bEnable = TRUE;
		}
		break;

	case FileLevel:
		// cannot delete empty file
		ASSERT(selectedNode.HasChildren());
		if (selectedNode.HasChildren())
		{
			bEnable = selectedNode.GetChild().HasChildren();
		}
		break;

	case WorkLevel:
		// cannot delete workspace
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	pCmdUI->Enable(bEnable);
}

void CTreeCtrlIn::OnEditClear()
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode selectedNode = GetSelectedItem();
	ClearNode(selectedNode);
}

void CTreeCtrlIn::RenumberSimulations(CTreeCtrlNode fileNode, CTreeCtrlNode firstSimNode /*= CTreeCtrlNode()*/)
{
	ASSERT(fileNode.GetLevel() == FileLevel);
	ASSERT(!firstSimNode || firstSimNode.GetLevel() == SimLevel);
	UNUSED(firstSimNode);	// not implemented yet

	int nSim = 1;
	CString label;

	CTreeCtrlNode simNode = fileNode.GetChild();
	for (; simNode != NULL; simNode = simNode.GetNextSibling())
	{
		label.Format(IDS_SIM_FORMAT_135, nSim);
		// avoid flashing
		if (label.Compare(simNode.GetText()) != 0)
		{
			simNode.SetText(label);
		}
		++nSim;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlIn drag and drop operations

void CTreeCtrlIn::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;

	ASSERT(m_hDragTreeItem == 0);
	ASSERT(m_pDragImageList == NULL);

	CTreeCtrlNode dragNode(pNMTreeView->itemNew.hItem, this);
	dragNode.Select();

	// only files, simulations, and keywords may be dragged 
	int nLevel = dragNode.GetLevel();
	if (nLevel == FileLevel && dragNode.GetChild().GetChild() // file must contain keyword nodes
		||
		nLevel == SimLevel && dragNode.GetChild() // sim must contain keywords
		||
		nLevel == KeywordLevel)
	{
		// set drag node member variable
		ASSERT(m_hDragTreeItem == 0);
		m_hDragTreeItem = (HTREEITEM)dragNode;

		//{{MOVED
		// create the drag image
		ASSERT(m_pDragImageList == NULL);
		m_pDragImageList = dragNode.CreateDragImage();
		m_pDragImageList->BeginDrag(0, CPoint(10,10));

		// map drag point to desktop point
		MapWindowPoints(GetDesktopWindow(), &pNMTreeView->ptDrag, 1);
		//}}MOVED

		// create a shared memory file
		CSharedFile globFile;

		// create an archive to serialize to
		CArchive ar(&globFile, CArchive::store);

		// get string from richedit control
		CString string = GetNodeString(dragNode);

		// serialize the string
		ar << string;
		ar.Close();

		// create ole data source and add string as CTreeCtrlIn clip format
		COleDataSource oleDataSource;
		oleDataSource.CacheGlobalData(m_nIDClipFormat, globFile.Detach());

		// add text clip format
		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, string.GetLength() + 1);
		LPTSTR pData = (LPTSTR) ::GlobalLock(hData);
		::lstrcpy(pData, (LPCTSTR)string);
		::GlobalUnlock(hData);
#ifdef _UNICODE
		oleDataSource.CacheGlobalData(CF_UNICODETEXT, hData);
#else
		oleDataSource.CacheGlobalData(CF_TEXT, hData);
#endif

		DROPEFFECT dwDropEffect = oleDataSource.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

		if ((dwDropEffect & DROPEFFECT_MOVE) == DROPEFFECT_MOVE)
		{
			// see PasteString
			// remove the node being dragged
			ASSERT(m_hDragTreeItem != 0);
			ClearNode(CTreeCtrlNode(m_hDragTreeItem, this));
		}
		else if ((dwDropEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY)
		{
			// do nothing
		}
		else if ((dwDropEffect & DROPEFFECT_NONE) == DROPEFFECT_NONE)
		{
			// do nothing
		}

		// next line required if dropped onto a different instance
		SelectDropTarget(0);

		m_pDragImageList->DragLeave(GetDesktopWindow());
		m_pDragImageList->EndDrag();
		delete m_pDragImageList;
		m_pDragImageList = NULL;
		m_hDragTreeItem = 0;

		RedrawWindow();
	}

	ASSERT(m_hDragTreeItem == 0);
	ASSERT(m_pDragImageList == NULL);

	*pResult = 0;
}

DROPEFFECT CTreeCtrlIn::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT de = DROPEFFECT_NONE;

	// only allow CTreeCtrlIn clip format
	if (pDataObject->IsDataAvailable(m_nIDClipFormat) == TRUE)
	{
		// Note: the drag image won't be displayed in other instances
		if (m_pDragImageList)
		{
			// display drag image
			ASSERT(m_hDragTreeItem);
			CPoint ptDesktopWindow(point);
			MapWindowPoints(GetDesktopWindow(), &ptDesktopWindow, 1);
			VERIFY(m_pDragImageList->DragEnter(GetDesktopWindow(), ptDesktopWindow));
		}

		SelectDropNode(point);

		de = GetDropEffect(pDataObject, dwKeyState, point);
	}
	return de;
}

DROPEFFECT CTreeCtrlIn::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT de = DROPEFFECT_NONE;

	if (pDataObject->IsDataAvailable(m_nIDClipFormat) == TRUE)
	{
		// if (m_pDragImageList == NULL) we are dragging from another instance
		// and there is no drag image list and (m_hDragTreeItem == 0)
		if (m_pDragImageList != NULL)
		{
			ASSERT(m_hDragTreeItem != 0);
			CPoint ptDesktop(point);
			MapWindowPoints(GetDesktopWindow(), &ptDesktop, 1);
			VERIFY(m_pDragImageList->DragMove(ptDesktop));
		}

		SelectDropNode(point);

		de = GetDropEffect(pDataObject, dwKeyState, point);
	}
	return de;
}

BOOL CTreeCtrlIn::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	UNUSED(point);
	// display hourglass
	CWaitCursor wait;

	TRACE0("CTreeCtrlIn::OnDrop\n");
	ASSERT(dropEffect != DROPEFFECT_NONE);
	UNUSED(dropEffect);  // unused in release builds

	// verify clipboard format
	ASSERT(pDataObject->IsDataAvailable(m_nIDClipFormat));

	HGLOBAL hGlob = pDataObject->GetGlobalData(m_nIDClipFormat);
	if (hGlob != NULL)
	{
		CSharedFile globFile;
		globFile.SetHandle(hGlob, FALSE);
		CArchive ar(&globFile, CArchive::load);

		CString strDrop;

		ar >> strDrop;

		ar.Close();

		CTreeCtrlNode destNode = GetDropHilightItem();
		SelectDropTarget(0);
		ASSERT(destNode != 0);
		PasteString(destNode, strDrop);
		return TRUE;
	}

	return FALSE;
}
void CTreeCtrlIn::OnDragLeave()
{
	TRACE0("CTreeCtrlIn::OnDragLeave\n");
	SelectDropTarget(0);
	if (m_pDragImageList != NULL)
	{
		ASSERT(m_hDragTreeItem != 0);
		VERIFY(m_pDragImageList->DragLeave(GetDesktopWindow()));
	}
}

void CTreeCtrlIn::SelectDropNode(CPoint point)
{
	CTreeCtrlNode hitNode = HitTest(point);
	if (hitNode != NULL)
	{
		if (m_pDragImageList)
		{
			ASSERT(m_hDragTreeItem);
			// unlock window updates (allow window to redraw drop target)
			VERIFY(m_pDragImageList->DragShowNolock(FALSE));
		}

		// can't drop onto workspace level
		if (hitNode.GetLevel() == WorkLevel)
		{
			hitNode.GetChild().SelectDropTarget();
		}
		// can't drop onto keyword option
		else if (hitNode.GetLevel() == KeywordOptionLevel)
		{
			hitNode.GetParent().SelectDropTarget();
		}
		else
		{
			hitNode.SelectDropTarget();
		}

		if (hitNode == GetFirstVisibleItem())
		{
			// scroll up if necessary
			CTreeCtrlNode prevItem =  hitNode.GetPrevVisible();
			if (prevItem)
			{
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			}
		}
		else
		{
			// scroll down if necessary
			// TODO: this needs work
			HTREEITEM hVisibleItem = GetFirstVisibleItem();
			HTREEITEM hNextItem = GetNextVisibleItem((HTREEITEM)hitNode);
			UINT nVisible = GetVisibleCount();
			while (nVisible--)
			{
				if (hVisibleItem == hNextItem) break;
				hVisibleItem = GetNextVisibleItem(hVisibleItem);
				if (hVisibleItem == 0) break;
			}
			if (!nVisible)
			{
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			}
		}

		if (m_pDragImageList)
		{
			ASSERT(m_hDragTreeItem);
			// lock window updates (this reduces flicker)
			VERIFY(m_pDragImageList->DragShowNolock(TRUE));
		}
	}
}

DROPEFFECT CTreeCtrlIn::GetDropEffect(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	UNUSED(point);
	ASSERT(pDataObject->IsDataAvailable(m_nIDClipFormat) == TRUE);
	UNUSED(pDataObject);  // unused in release builds

	// see "Handling a Drag-and-Drop Operation" in MSDN Library
	DROPEFFECT de = DROPEFFECT_NONE;

	if (m_hDragTreeItem)
	{
		// do not allow a file node to be dragged onto itself or any descendants
		CTreeCtrlNode dragItem(m_hDragTreeItem, this);
		if (dragItem.GetLevel() == FileLevel && dragItem.IsNodeDescendant(GetDropHilightItem()))
		{
			return (de = DROPEFFECT_NONE);
		}
	}

	// do not allow a node to be dragged onto itself
	if (m_hDragTreeItem && GetDropHilightItem() == m_hDragTreeItem)
	{
		de = DROPEFFECT_NONE;
	}
	// check for force link
	else if ((dwKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
	{
		de = DROPEFFECT_NONE;	// link not supported
	}
	// check for force copy
	else if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
	{
		de = DROPEFFECT_COPY;
	}
	// check for force move
	else if ((dwKeyState & MK_ALT) == MK_ALT)
	{
		de = DROPEFFECT_MOVE;
	}
	else
	{
		if (m_hDragTreeItem)
		{
			// if the node being dragged is in the same file as the current drop node
			// default to move otherwise default to copy
			ASSERT(m_pDragImageList);
			CTreeCtrlNode dragItem(m_hDragTreeItem, this);
			if (dragItem.IsNodeAncestor(GetFileNode(GetDropHilightItem())))
			{
				de = DROPEFFECT_MOVE;
			}
			else
			{
				de = DROPEFFECT_COPY;
			}
		}
		else
		{
			// the drop source is in another instance default to copy
			de = DROPEFFECT_COPY;
		}
	}
	return de;
}

void CTreeCtrlIn::UpdateAllViews(CDocument *pDoc, LPARAM lHint, CObject *pHint)
{
	UNUSED(pHint);
	ASSERT_KINDOF(CRichDocIn, pDoc);
	if (lHint == RICHVIEWIN_EN_CHANGE)
	{
		CTreeCtrlNode fileNode = GetFileNode((CRichDocIn*)pDoc);
		ASSERT(fileNode);
		fileNode.Expand(TVE_COLLAPSE);
		fileNode.SetState(INDEXTOOVERLAYMASK(UnSyncState), TVIS_OVERLAYMASK);
	}
	if (lHint == RICHDOCIN_ONACTIVATE)
	{
		CTreeCtrlNode selNode = GetSelectedItem();
		CTreeCtrlNode fileNode = GetFileNode((CRichDocIn*)pDoc);
		ASSERT(fileNode);
		if (selNode != fileNode)
		{
			fileNode.Select();
		}
	}
	if (lHint == RICHDOCIN_UPDATE_FRAME_COUNTS)
	{
		// do nothing
	}
	if (lHint == RICHDOCIN_SET_PATH_NAME)
	{
		CTreeCtrlNode fileNode = GetFileNode((CRichDocIn*)pDoc);
		ASSERT(fileNode);
		fileNode.SetText(pDoc->GetTitle());
	}
}

// TVN_ITEMEXPANDING notification
void CTreeCtrlIn::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;

	CTreeCtrlNode node(pNMTreeView->itemNew.hItem, this);

	*pResult = FALSE;	// allow expansion

	if (node.GetLevel() == FileLevel)
	{
		UINT nState = node.GetState(TVIS_OVERLAYMASK);

		switch (nState >> 8)
		{
		case ErrorState :
			TRACE0(_T("ErrorState\n"));
			::MessageBox(m_hWnd, _T("Use the line editor to fix these errors."), _T("File contains errors"), MB_OK);
			*pResult = TRUE;	// don't allow expansion
			break;

		case UnSyncState :
			TRACE0(_T("UnSyncState\n"));
			{
				CRichEditDoc* pDoc = (CRichEditDoc*)node.GetData();
				ASSERT_KINDOF(CRichEditDoc, pDoc);
				int nErrors = GetWorkspaceBar().SyncInputTrees(pDoc);
				if (nErrors == 0)
				{
					node.SetState(INDEXTOOVERLAYMASK(SyncState), TVIS_OVERLAYMASK);
				}
				else
				{
					::MessageBox(m_hWnd, _T("Use the line editor to fix these errors."), _T("File contains errors"), MB_OK);
					*pResult = TRUE;		// don't allow expansion
				}
			}
			break;

		case SyncState:
			TRACE0(_T("SyncState\n"));
			break;

		default:
			TRACE0(_T("Unknown state\n"));
			ASSERT(FALSE);
			break;
		}
	}
}

CWorkspaceBar& CTreeCtrlIn::GetWorkspaceBar()
{
	CWorkspaceBar* pBar = (CWorkspaceBar*)GetParent()->GetParent();
	ASSERT_KINDOF(CWorkspaceBar, pBar);
	return (*pBar);
}


void CTreeCtrlIn::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	CTreeCtrlNode selectedNode = GetSelectedItem();

	BOOL bEnable = FALSE;

	switch (selectedNode.GetLevel())
	{
	case KeywordOptionLevel:
		// cannot copy keyword options
		break;

	case KeywordLevel:
		bEnable = TRUE;
		break;

	case SimLevel:
		// cannot copy empty simulation
		if (selectedNode.HasChildren())
		{
			bEnable = TRUE;
		}
		break;

	case FileLevel:
		ASSERT(selectedNode.HasChildren());
		// cannot copy empty file
		if (selectedNode.HasChildren())
		{
			// check if first sim contains keywords
			bEnable = selectedNode.GetChild().HasChildren();
		}
		break;

	case WorkLevel:
		// cannot copy workspace
		break;

	}
	pCmdUI->Enable(bEnable);
}

void CTreeCtrlIn::OnEditCopy()
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode node = GetSelectedItem();
	if (node != NULL)
	{
		int nLevel = node.GetLevel();
		if (nLevel == KeywordLevel || nLevel == SimLevel || nLevel == FileLevel)
		{
			CopyNode(node);
		}
		else
		{
			// sound warning message
			::MessageBeep(MB_ICONEXCLAMATION);
		}
	}
}

void CTreeCtrlIn::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	COleDataObject dataObject;

	VERIFY( dataObject.AttachClipboard() == TRUE );

	CTreeCtrlNode node = GetSelectedItem();

	int nLevel = node.GetLevel();

	pCmdUI->Enable(
		dataObject.IsDataAvailable(m_nIDClipFormat)
		&&
		(nLevel == FileLevel || nLevel == SimLevel || nLevel == KeywordLevel)
		);
}

//
// pasting onto a keyword node inserts new keyword(s) before selected
// keyword
//
// pasting onto a simulation node inserts new keyword(s) at end of selected
// simulation (before END keyword)
//
// pasting onto a file node inserts text at end of file
void CTreeCtrlIn::OnEditPaste()
{
	// display hourglass
	CWaitCursor wait;

	// get node to paste to
	CTreeCtrlNode pasteNode = GetSelectedItem();
	if (pasteNode == 0) return;

	// get data from clipboard
	CString strClipboard;
	if (!GetClipBoardData(strClipboard))
	{
		// nothing to do
		return;
	}
	PasteString(pasteNode, strClipboard);
}

long CTreeCtrlIn::GetInsertPosBeforeKeyword(const CTreeCtrlNode &rNode)
{
	CTreeCtrlNode node(rNode);

	// only keywords
	ASSERT(node.GetLevel() == KeywordLevel);
	return GetRange(node).cpMin;
}

long CTreeCtrlIn::GetInsertPosAfterKeyword(const CTreeCtrlNode &rNode)
{
	CTreeCtrlNode node(rNode);

	// only keywords
	ASSERT(node.GetLevel() == KeywordLevel);

	// determine next keyword node
	CTreeCtrlNode nextKeywordNode = node.GetNextSibling();
	if (nextKeywordNode == 0)
	{
		ASSERT(node.GetParent());
		nextKeywordNode = node.GetParent().GetNextSibling();
		if (nextKeywordNode != 0)
		{
			nextKeywordNode = nextKeywordNode.GetChild();
		}
	}
	if (nextKeywordNode)
	{
		return GetRange(nextKeywordNode).cpMin;
	}
	else
	{
		// end of file
		return -1;
	}

}

bool CTreeCtrlIn::GetClipBoardData(CString &rStr)
{
	COleDataObject dataObject;

	rStr.Empty();

	if (!dataObject.AttachClipboard() && !dataObject.IsDataAvailable(m_nIDClipFormat))
	{
		return false;
	}

	HGLOBAL hGlob = dataObject.GetGlobalData(m_nIDClipFormat, NULL);
	if (hGlob != NULL)
	{
		CSharedFile globFile;
		globFile.SetHandle(hGlob, FALSE);
		CArchive ar(&globFile, CArchive::load);
		ar >> rStr;
		ar.Close();
	}

	if (rStr.IsEmpty())
	{
		return false;
	}

	// make sure string ends with LF or logical line terminator(;)
	int nLen = rStr.GetLength();
#if _MSC_VER >= 1400
	switch (rStr[nLen - 1])
	{
	case _T('\r') :
		break;
	case _T('\n') :
		break;
	case _T(';') :
		break;
	default :
		rStr += _T("\r");
		break;
	}
#else
	switch (rStr[nLen - 1])
	{
	case _T('\n') :
		break;
	case _T(';') :
		break;
	default :
		rStr += _T("\r\n");
		break;
	}
#endif
	return true;
}

// if rNode is a keyword node
//		returns the keyword node before rNode
//
// if rNode is a simulation node
// and rNode contains keywords
//		returns the last non-END keyword node
// and rNode doesn't contain keywords
//		returns the previous simulation's last keyword node(END)
//
// if rNode is a file node
//		returns the last keyword node of all simulations
//
// Note: If there is no previous keyword the return value is 0
CTreeCtrlNode CTreeCtrlIn::GetPrevKeywordNode(const CTreeCtrlNode &rNode) const
{
	CTreeCtrlNode prevKeywordNode;
	CTreeCtrlNode copyNode(rNode);
	ASSERT(copyNode);

	int nLevel = copyNode.GetLevel();

	ASSERT(copyNode.GetLevel() != WorkLevel);
	ASSERT(copyNode.GetLevel() != KeywordOptionLevel);

	switch (nLevel)
	{
	case FileLevel :
		//
		// return last keyword node 
		//

		ASSERT(copyNode.GetLastChild());    // must be at least one sim
		prevKeywordNode = GetPrevKeywordNode(copyNode.GetLastChild());

		break;

	case SimLevel :
		//
		// return last keyword node of simulation (before END)
		// or last keyword of previous simulation (END)

		prevKeywordNode = copyNode.GetLastChild();
		if (prevKeywordNode)
		{
			if (prevKeywordNode.GetImageID() == endImage)
			{
				ASSERT(copyNode.GetNextSibling());	// END keyword must be followed by a simulation
				prevKeywordNode = GetPrevKeywordNode(prevKeywordNode);
			}
		}
		else if (copyNode.GetPrevSibling())
		{
			prevKeywordNode = copyNode.GetPrevSibling().GetLastChild();
			ASSERT(prevKeywordNode.GetImageID() == endImage);	// must be END
		}

		// prevKeywordNode is 0 only if the file contains no keywords

		// verify first sim
		ASSERT(prevKeywordNode || (copyNode.GetPrevSibling() == 0));
		break;

	case KeywordLevel :
		//
		// return keyword node just before rNode
		// 
		prevKeywordNode = copyNode.GetPrevSibling();
		if (prevKeywordNode == 0)
		{
			prevKeywordNode = copyNode.GetParent().GetPrevSibling();
			if (prevKeywordNode != 0)
			{
				//prevKeywordNode = prevKeywordNode.GetChild();
				prevKeywordNode = prevKeywordNode.GetLastChild();
				ASSERT(prevKeywordNode);
			}
		}

		// prevKeywordNode is 0 if rNode is the first keyword
		// in the file

		// verify first keyword
		ASSERT(prevKeywordNode || (copyNode.GetParent().GetChild() == copyNode));
		// verify first sim
		ASSERT(prevKeywordNode || (copyNode.GetParent().GetPrevSibling() == 0));
		break;

	case KeywordOptionLevel :
		ASSERT(FALSE);	// cannot paste to option
		break;

	default :
		ASSERT(FALSE);	// no more levels
		break;
	}

	// verify 0 or keyword level
	ASSERT(!prevKeywordNode || prevKeywordNode.GetLevel() == KeywordLevel);

	return prevKeywordNode;	// 0 signifies no previous keyword
}

void CTreeCtrlIn::OnFileClose() 
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	CTreeCtrlNode fileNode = GetFileNode(selectedNode);
	ASSERT(fileNode.GetLevel() == FileLevel);
	//GetRichEditDoc(fileNode)->OnCloseDocument();
	((CRichDocIn*)GetRichEditDoc(fileNode))->OnFileClose();
}

void CTreeCtrlIn::OnPopupClose() 
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	CTreeCtrlNode fileNode = GetFileNode(selectedNode);
	ASSERT(fileNode.GetLevel() == FileLevel);
	//GetRichEditDoc(fileNode)->OnCloseDocument();
	((CRichDocIn*)GetRichEditDoc(fileNode))->OnFileClose();
}

void CTreeCtrlIn::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedItem().GetLevel() == FileLevel);
}

void CTreeCtrlIn::OnDebugInsertBef() 
{
	CTreeCtrlNode node = GetSelectedItem();
	ASSERT(node.GetLevel() == KeywordLevel);

	CTreeCtrlNode prev = GetPrevKeywordNode(node);

	VERIFY(SetInsertMark(node, FALSE));
}

void CTreeCtrlIn::OnDebugInsertAft() 
{
	CTreeCtrlNode node = GetSelectedItem();
	ASSERT(node.GetLevel() == KeywordLevel);

	CTreeCtrlNode next = GetNextKeywordNode(node);

	if (next)
	{
		VERIFY(SetInsertMark(next, FALSE/*bAfter*/));
		TRACE("next=%s\n", next.GetText());
	}
	else
	{
		VERIFY(SetInsertMark(node.GetParent().GetNextSibling(), TRUE/*bAfter*/));
	}
}

CTreeCtrlNode CTreeCtrlIn::GetNextKeywordNode(const CTreeCtrlNode &rNode) const
{
	CTreeCtrlNode nextKeywordNode;
	CTreeCtrlNode copyNode(rNode);
	int nLevel = copyNode.GetLevel();

	ASSERT(copyNode.GetLevel() != WorkLevel);

	CTreeCtrlNode simNode;

	switch (nLevel)
	{
	case FileLevel :
		ASSERT(FALSE);	// NOT YET
		break;

	case SimLevel :
		ASSERT(FALSE);	// NOT YET
		break;

	case KeywordLevel :
		nextKeywordNode = copyNode.GetNextSibling();
		if (nextKeywordNode == 0)
		{
			nextKeywordNode = copyNode.GetParent().GetNextSibling();
			if (nextKeywordNode != 0)
			{
				nextKeywordNode = nextKeywordNode.GetChild();
			}
		}
		break;

	case KeywordOptionLevel :
		ASSERT(FALSE);	// cannot paste to option
		break;

	default :
		ASSERT(FALSE);	// no more levels
		break;
	}

	return nextKeywordNode;	// 0 signifies no next keyword
}

void CTreeCtrlIn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Add your message handler code here and/or call default

	// is OnDragEnter called before or after
	// is OnMouseMove called when dragging?

	// NOTE OnMouseMove is not called while dragging
	////TRACE("OnMouseMove\n");
	if (m_pDragImageList)
	{
		ASSERT(FALSE);
		CPoint pt (point);
		ClientToScreen (&pt);

		// move the drag image
		VERIFY (m_pDragImageList->DragMove(pt));
	}
	
	baseCTreeCtrlIn::OnMouseMove(nFlags, point);
}

void CTreeCtrlIn::OnKillFocus(CWnd* pNewWnd) 
{
	baseCTreeCtrlIn::OnKillFocus(pNewWnd);
	
	// Add your message handler code here
	
}

void CTreeCtrlIn::OnRunInput() 
{
	CTreeCtrlNode node = GetSelectedItem();
	ASSERT_KINDOF(CRichDocIn, GetRichEditDoc(node));
	((CRichDocIn*)GetRichEditDoc(node))->OnRunInput();	
}

void CTreeCtrlIn::OnUpdateRunInput(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedItem().GetLevel() == FileLevel);
}

//
// retrieves the string associated with rNode without
// modifying the richedit control (ie doesn't change the current
// selection)
//
CString CTreeCtrlIn::GetNodeString(const CTreeCtrlNode& rNode)const
{
	TEXTRANGE tr;
	tr.chrg = GetRange(rNode);
	CRichEditView* pView = GetRichEditView(rNode);
	ASSERT(pView);
	ASSERT_KINDOF(CRichEditView, pView);
	ASSERT(::IsWindow(pView->m_hWnd));

	if (tr.chrg.cpMax == -1)
	{
		ASSERT(tr.chrg.cpMin == 0);
		tr.chrg.cpMax = ::GetWindowTextLength(pView->m_hWnd);
	}
	ASSERT(tr.chrg.cpMax > tr.chrg.cpMin);

#if _MSC_VER >= 1400
	// see CString CRichEditCtrl::GetTextRange() const
	int nLength = int(tr.chrg.cpMax - tr.chrg.cpMin + 1);
	ASSERT(nLength > 0);

	CString strText;
	tr.lpstrText = strText.GetBuffer(nLength);
	nLength = (int)::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr);
	strText.ReleaseBuffer(nLength);
#if 1
	strText.Replace('\r', '\n');
#endif
	return CString(strText);
#else
	// see CString CRichEditCtrl::GetSelText() const
	LPSTR lpsz = (char*)_alloca((tr.chrg.cpMax - tr.chrg.cpMin + 1)*2);
	lpsz[0] = NULL;
	tr.lpstrText = lpsz;
	::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	return lpsz;
#endif
}

//
// retrieves the string associated with rNode without
// modifying the richedit control (ie doesn't change the current
// selection)
//
CString CTreeCtrlIn::GetTransportNodeString(const CTreeCtrlNode& rNode)const
{
	TEXTRANGE tr;
	tr.chrg = GetRange(rNode);
	//{{
	tr.chrg.cpMin = 0;
	//}}
	CRichEditView* pView = GetRichEditView(rNode);
	ASSERT(pView);
	ASSERT_KINDOF(CRichEditView, pView);
	ASSERT(::IsWindow(pView->m_hWnd));

	if (tr.chrg.cpMax == -1)
	{
		ASSERT(tr.chrg.cpMin == 0);
		tr.chrg.cpMax = ::GetWindowTextLength(pView->m_hWnd);
	}
	ASSERT(tr.chrg.cpMax > tr.chrg.cpMin);

#if _MSC_VER >= 1400
	// see CString CRichEditCtrl::GetTextRange() const
	int nLength = int(tr.chrg.cpMax - tr.chrg.cpMin + 1);
	ASSERT(nLength > 0);

	CString strText;
	tr.lpstrText = strText.GetBuffer(nLength);
	nLength = (int)::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr);
	strText.ReleaseBuffer(nLength);
#if 1
	strText.Replace('\r', '\n');
#endif
	return CString(strText);
#else
	// see CString CRichEditCtrl::GetSelText() const
	LPSTR lpsz = (char*)_alloca((tr.chrg.cpMax - tr.chrg.cpMin + 1)*2);
	lpsz[0] = NULL;
	tr.lpstrText = lpsz;
	::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	return lpsz;
#endif
}

//
// retrieves the string associated with rNode without
// modifying the richedit control (ie doesn't change the current
// selection)
//
CString CTreeCtrlIn::GetPreviousString(const CTreeCtrlNode& rNode)const
{
	TEXTRANGE tr;
	tr.chrg = GetRange(rNode);
	//{{
	tr.chrg.cpMax = tr.chrg.cpMin;
	tr.chrg.cpMin = 0;
	//}}
	CRichEditView* pView = GetRichEditView(rNode);
	ASSERT(pView);
	ASSERT_KINDOF(CRichEditView, pView);
	ASSERT(::IsWindow(pView->m_hWnd));

	if (tr.chrg.cpMax == -1)
	{
		ASSERT(tr.chrg.cpMin == 0);
		tr.chrg.cpMax = ::GetWindowTextLength(pView->m_hWnd);
	}
	ASSERT(tr.chrg.cpMax >= tr.chrg.cpMin);

#if _MSC_VER >= 1400
	// see CString CRichEditCtrl::GetTextRange() const
	int nLength = int(tr.chrg.cpMax - tr.chrg.cpMin + 1);
	ASSERT(nLength > 0);

	CString strText;
	tr.lpstrText = strText.GetBuffer(nLength);
	nLength = (int)::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr);
	strText.ReleaseBuffer(nLength);
#if 1
	strText.Replace('\r', '\n');
#endif
	return CString(strText);
#else
	// see CString CRichEditCtrl::GetSelText() const
	LPSTR lpsz = (char*)_alloca((tr.chrg.cpMax - tr.chrg.cpMin + 1)*2);
	lpsz[0] = NULL;
	tr.lpstrText = lpsz;
	::SendMessage(pView->m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	return lpsz;
#endif
}

void CTreeCtrlIn::OnUpdateKey(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	switch (pCmdUI->m_nID)
	{
	case ID_KEY_TITLE                   : case ID_KEY_TITLE_A                   :
	case ID_KEY_END                     : case ID_KEY_END_A                     :
	case ID_KEY_USE                     : case ID_KEY_USE_A                     :
	case ID_KEY_EQUILIBRIUM_PHASES      : case ID_KEY_EQUILIBRIUM_PHASES_A      :
	case ID_KEY_GAS_PHASE               : case ID_KEY_GAS_PHASE_A               :
	case ID_KEY_SAVE                    : case ID_KEY_SAVE_A                    :
	case ID_KEY_EXCHANGE                : case ID_KEY_EXCHANGE_A                :
	case ID_KEY_MIX                     : case ID_KEY_MIX_A                     :
	case ID_KEY_REACTION                : case ID_KEY_REACTION_A                :
	case ID_KEY_REACTION_TEMPERATURE    : case ID_KEY_REACTION_TEMPERATURE_A    :
	case ID_KEY_SOLUTION                : case ID_KEY_SOLUTION_A                :
	case ID_KEY_SOLID_SOLUTIONS         : case ID_KEY_SOLID_SOLUTIONS_A         :
	case ID_KEY_RATES                   : case ID_KEY_RATES_A                   :
	case ID_KEY_INVERSE_MODELING        : case ID_KEY_INVERSE_MODELING_A        :
	case ID_KEY_USER_PRINT              : case ID_KEY_USER_PRINT_A              :
	case ID_KEY_USER_PUNCH              : case ID_KEY_USER_PUNCH_A              :
	case ID_KEY_SURFACE                 : case ID_KEY_SURFACE_A                 :
	case ID_KEY_PRINT                   : case ID_KEY_PRINT_A                   :
	case ID_KEY_KINETICS                : case ID_KEY_KINETICS_A                :
	case ID_KEY_INCREMENTAL_REACTIONS   : case ID_KEY_INCREMENTAL_REACTIONS_A   :
	case ID_KEY_SELECTED_OUTPUT         : case ID_KEY_SELECTED_OUTPUT_A         :
	case ID_KEY_ADVECTION               : case ID_KEY_ADVECTION_A               :
	case ID_KEY_TRANSPORT               : case ID_KEY_TRANSPORT_A               :
	case ID_KEY_SOLUTION_SPREAD         : case ID_KEY_SOLUTION_SPREAD_A         :
	case ID_KEY_SOLUTION_SPECIES        : case ID_KEY_SOLUTION_SPECIES_A        :
	case ID_KEY_SURFACE_SPECIES         : case ID_KEY_SURFACE_SPECIES_A         :
	case ID_KEY_EXCHANGE_SPECIES        : case ID_KEY_EXCHANGE_SPECIES_A        :
	case ID_KEY_PHASES                  : case ID_KEY_PHASES_A                  :
	case ID_KEY_SOLUTION_MASTER_SPECIES : case ID_KEY_SOLUTION_MASTER_SPECIES_A :
	case ID_KEY_EXCHANGE_MASTER_SPECIES : case ID_KEY_EXCHANGE_MASTER_SPECIES_A :
// COMMENT: {2/16/2012 4:58:31 PM}	case ID_KEY_SURFACE_MASTER_SPECIES  : case ID_KEY_SURFACE_MASTER_SPECIES_A  :
// COMMENT: {2/16/2012 4:58:31 PM}	case ID_KEY_KNOBS                   : case ID_KEY_KNOBS_A                   :		
	case ID_KEY_COPY                    : case ID_KEY_COPY_A                    :
// COMMENT: {2/16/2012 4:58:31 PM}	case ID_KEY_PITZER                  : case ID_KEY_PITZER_A                  :
// COMMENT: {2/16/2012 4:58:31 PM}	case ID_KEY_SIT                     : case ID_KEY_SIT_A                     :
	//{{NEW KEYWORD HERE}}
		bEnable = TRUE;
		break;
	default :
		break;
	}
	// check error status
	CTreeCtrlNode node = GetFileNode(GetSelectedItem());
	if (node)
	{
		UINT nState = node.GetState(TVIS_OVERLAYMASK);
		if (nState >> 8 != SyncState)
		{
			bEnable = FALSE;
		}
	}
	else
	{
		bEnable = FALSE;
	}
	pCmdUI->Enable(bEnable);
}

// if selected node is keyword or keyword option:
//     Insert keyword before selected keyword
//
// if selected node is sim:
//     Insert keyword as last keyword before END
//
// if selected node is file:
//     Insert keyword as last keyword of file
//
void CTreeCtrlIn::OnKey(UINT nID) 
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode nodeSelected = GetSelectedItem();

	//{{
	// set active document
	// required if database is needed
	GetRichEditView(nodeSelected)->GetParentFrame()->ActivateFrame();
	//}}

	ASSERT(nodeSelected);

	CTreeCtrlNode nodeSimToAddTo;
	CTreeCtrlNode nodeToInsertAfter;

	switch (nodeSelected.GetLevel())
	{
	case KeywordOptionLevel :
		nodeSelected = nodeSelected.GetParent();
		// fall through

	case KeywordLevel :
		nodeSimToAddTo = nodeSelected.GetParent();
		nodeToInsertAfter = nodeSelected.GetPrevSibling();
		break;

	case SimLevel :
		nodeSimToAddTo = nodeSelected;
		nodeToInsertAfter = nodeSelected.GetLastChild();
		if (nodeToInsertAfter && nodeToInsertAfter.GetImageID() == endImage)
		{
			nodeToInsertAfter = nodeToInsertAfter.GetPrevSibling();
		}
		break;

	case FileLevel :
		nodeSimToAddTo = nodeSelected.GetLastChild();
		nodeToInsertAfter = nodeSimToAddTo.GetLastChild();
		break;

	case WorkLevel :
		ASSERT(FALSE);
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	if (!nodeToInsertAfter)
	{
		nodeToInsertAfter = CTreeCtrlNode(TVI_FIRST, this);
	}

	CKeywordSheet* pKeywordSheet = NULL;
	CString strLabel;
	//int nImageIndex;
	CTreeCtrlIn::ImageIndex nImageIndex = unusedImage;
	
	switch (nID)
	{
	case ID_KEY_END :
		strLabel = _T("END\r\n");
		PasteString(nodeSelected, strLabel);
		break;
	case ID_KEY_USE :
		strLabel = _T("USE...");
		nImageIndex = useImage;
		pKeywordSheet = new COCKSUse(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_TITLE :
		strLabel = _T("TITLE...");
		nImageIndex = titleImage;
		pKeywordSheet = new CKSTitle();
		break;
	case ID_KEY_EQUILIBRIUM_PHASES :
		strLabel = _T("EQUILIBRIUM_PHASES...");
		nImageIndex = equilibrium_phasesImage;
		pKeywordSheet = new CCKSEquilibrium_Phases();
		break;
	case ID_KEY_GAS_PHASE :
		strLabel = _T("GAS_PHASE...");
		nImageIndex = gas_phaseImage;
		pKeywordSheet = new CCKSGas_Phase(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_SAVE :
		strLabel = _T("SAVE...");
		nImageIndex = saveImage;
		pKeywordSheet = new COCKSSave(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_EXCHANGE :
		strLabel = _T("EXCHANGE...");
		nImageIndex = exchangeImage;
		pKeywordSheet = new CCKSExchange(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_MIX :
		strLabel = _T("MIX...");
		nImageIndex = mixImage;
		pKeywordSheet = new CCKSMix(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_REACTION :
		strLabel = _T("REACTION...");
		nImageIndex = reactionImage;
		pKeywordSheet = new CCKSReaction();
		break;
	case ID_KEY_REACTION_TEMPERATURE :
		strLabel = _T("REACTION_TEMPERATURE...");
		nImageIndex = reaction_temperatureImage;
		pKeywordSheet = new CCKSReaction_Temperature();
		break;
	case ID_KEY_SOLUTION :
		strLabel = _T("SOLUTION...");
		nImageIndex = solutionImage;
		pKeywordSheet = new CCKSSolution();
		break;
	case ID_KEY_SOLID_SOLUTIONS :
		strLabel = _T("SOLID_SOLUTIONS...");
		nImageIndex = solid_solutionsImage;
		pKeywordSheet = new CCKSSolid_Solutions();
		break;
	case ID_KEY_RATES :
		strLabel = _T("RATES...");
		nImageIndex = ratesImage;
		pKeywordSheet = new COCKSRates();
		break;
	case ID_KEY_INVERSE_MODELING :
		strLabel = _T("INVERSE_MODELING...");
		nImageIndex = inverse_modelingImage;
		pKeywordSheet = new CCKSInverse(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_USER_PRINT :
		strLabel = _T("USER_PRINT...");
		nImageIndex = user_printImage;
		pKeywordSheet = new COCKSUserPrint();
		break;
	case ID_KEY_USER_PUNCH :
		strLabel = _T("USER_PUNCH...");
		nImageIndex = user_punchImage;
		pKeywordSheet = new COCKSUserPunch();
		break;
	case ID_KEY_SURFACE :
		strLabel = _T("SURFACE...");
		nImageIndex = surfaceImage;
		pKeywordSheet = new CSurfaceSheet(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_PRINT :
		strLabel = _T("PRINT...");
		nImageIndex = printImage;
		pKeywordSheet = new CKSPrint();
		break;
	case ID_KEY_KINETICS :
		strLabel = _T("KINETICS...");
		nImageIndex = kineticsImage;
		pKeywordSheet = new CCKSKinetics();
		break;
	case ID_KEY_INCREMENTAL_REACTIONS :
		strLabel = _T("INCREMENTAL_REACTIONS...");
		nImageIndex = incremental_reactionsImage;
		pKeywordSheet = new CKSIncrement();
		break;
	case ID_KEY_SELECTED_OUTPUT :
		strLabel = _T("SELECTED_OUTPUT...");
		nImageIndex = selected_outputImage;
		pKeywordSheet = new CKSSelectedOutput();
		break;
	case ID_KEY_ADVECTION :
		strLabel = _T("ADVECTION...");
		nImageIndex = advectionImage;
		pKeywordSheet = new COCKSAdvection();
		break;
	case ID_KEY_TRANSPORT :
		strLabel = _T("TRANSPORT...");
		nImageIndex = transportImage;
		pKeywordSheet = new COCKSTransport();
		break;
	case ID_KEY_SOLUTION_SPREAD :
		strLabel = _T("SOLUTION_SPREAD...");
		nImageIndex = solution_spreadImage;
		pKeywordSheet = new COCKSSolution_Spread();
		break;
	case ID_KEY_SOLUTION_SPECIES :
		strLabel = _T("SOLUTION_SPECIES...");
		nImageIndex = solution_speciesImage;
		pKeywordSheet = new CKSSolutionSpecies();
		break;
	case ID_KEY_SURFACE_SPECIES :
		strLabel = _T("SURFACE_SPECIES...");
		nImageIndex = surface_speciesImage;
		pKeywordSheet = new CKSSurfaceSpecies();
		break;
	case ID_KEY_EXCHANGE_SPECIES :
		strLabel = _T("EXCHANGE_SPECIES...");
		nImageIndex = exchange_speciesImage;
		pKeywordSheet = new CKSExchangeSpecies();
		break;
	case ID_KEY_PHASES :
		strLabel = _T("PHASES...");
		nImageIndex = phasesImage;
		pKeywordSheet = new CKSPhases();
		break;
	case ID_KEY_SOLUTION_MASTER_SPECIES :
		strLabel = _T("SOLUTION_MASTER_SPECIES...");
		nImageIndex = solution_master_speciesImage;
		pKeywordSheet = new CKSSolutionMasterSpecies();
		break;
	case ID_KEY_EXCHANGE_MASTER_SPECIES :
		strLabel = _T("EXCHANGE_MASTER_SPECIES...");
		nImageIndex = exchange_master_speciesImage;
		pKeywordSheet = new CKSExchangeMasterSpecies();
		break;
// COMMENT: {2/16/2012 4:59:15 PM}	case ID_KEY_SURFACE_MASTER_SPECIES :
// COMMENT: {2/16/2012 4:59:15 PM}		strLabel = _T("SURFACE_MASTER_SPECIES...");
// COMMENT: {2/16/2012 4:59:15 PM}		nImageIndex = surface_master_speciesImage;
// COMMENT: {2/16/2012 4:59:15 PM}		pKeywordSheet = new CKSSurfaceMasterSpecies();
// COMMENT: {2/16/2012 4:59:15 PM}		break;
// COMMENT: {2/16/2012 4:59:15 PM}	case ID_KEY_KNOBS :
// COMMENT: {2/16/2012 4:59:15 PM}		strLabel = _T("KNOBS...");
// COMMENT: {2/16/2012 4:59:15 PM}		nImageIndex = knobsImage;
// COMMENT: {2/16/2012 4:59:15 PM}		pKeywordSheet = new CKSKnobs();
// COMMENT: {2/16/2012 4:59:15 PM}		break;
	case ID_KEY_COPY :
		strLabel = _T("COPY...");
		nImageIndex = copyImage;
		pKeywordSheet = new COCKSCopy(NULL, nodeSimToAddTo);
		break;
// COMMENT: {2/16/2012 4:59:15 PM}	case ID_KEY_PITZER :
// COMMENT: {2/16/2012 4:59:15 PM}		strLabel = _T("PITZER...");
// COMMENT: {2/16/2012 4:59:15 PM}		nImageIndex = pitzerImage;
// COMMENT: {2/16/2012 4:59:15 PM}		pKeywordSheet = new CKSPitzer();
// COMMENT: {2/16/2012 4:59:15 PM}		break;
// COMMENT: {2/16/2012 4:59:15 PM}	case ID_KEY_SIT :
// COMMENT: {2/16/2012 4:59:15 PM}		strLabel = _T("SIT...");
// COMMENT: {2/16/2012 4:59:15 PM}		nImageIndex = sitImage;
// COMMENT: {2/16/2012 4:59:15 PM}		pKeywordSheet = new CKSSIT();
// COMMENT: {2/16/2012 4:59:15 PM}		break;
	//{{NEW KEYWORD HERE}}
	}

	if (pKeywordSheet == NULL) return;

	if (pKeywordSheet->IsKindOf(RUNTIME_CLASS(CCommonKeywordSheet)))
	{
		((CCommonKeywordSheet*)pKeywordSheet)->m_n_user = GetNextNum(nodeSelected, nImageIndex);
	}

	//{{
	// special case for TRANSPORT
	if (nImageIndex == transportImage)
	{
		ASSERT(pKeywordSheet->IsKindOf(RUNTIME_CLASS(COCKSTransport)));

		CString strPrev(_T(""));
		CString strNode(_T(""));
		CTreeCtrlNode prevNode = nodeToInsertAfter;
		if ((HTREEITEM)prevNode == TVI_FIRST)
		{
			CTreeCtrlNode simNode = nodeSimToAddTo;
			simNode = simNode.GetPrevSibling();
			if ((HTREEITEM)simNode)
			{
				prevNode = simNode.GetLastChild();
				if ((HTREEITEM)prevNode)
				{
					strPrev = this->GetPreviousString(prevNode);
					strPrev += this->GetNodeString(prevNode);
				}
			}
		}
		else
		{
			strPrev = this->GetPreviousString(prevNode);
			strPrev += this->GetNodeString(prevNode);
		}
		((COCKSTransport*)pKeywordSheet)->Edit2(strNode, strPrev);
	}
	//}}

	ASSERT(nodeSimToAddTo);
	CTreeCtrlNode nodeTemp = nodeSimToAddTo.InsertAfter(strLabel, nodeToInsertAfter, nImageIndex);
	nodeTemp.Select();

	// set active document
	// required if database is needed
	GetRichEditView(nodeSelected)->GetParentFrame()->ActivateFrame();
	if (pKeywordSheet && pKeywordSheet->bNeedDatabase)
	{
		int nIndex = nodeSimToAddTo.GetParent().GetIndex(nodeSimToAddTo);
		ASSERT(nIndex != -1);
		((CRichDocIn*)GetRichEditDoc(nodeSelected))->UpdateDatabase(nIndex + 1);
	}

	if (pKeywordSheet && pKeywordSheet->DoModal() == IDOK)
	{
		VERIFY(nodeTemp.Delete());
		ASSERT(nodeSelected);

		// verify nodeSelected still exists
		char szLabel[20];
		TVITEM tvi;
		tvi.hItem		= nodeSelected;
		tvi.mask		= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText		= szLabel;
		tvi.cchTextMax	= 20;
		ASSERT(GetItem(&tvi));

		CString str = pKeywordSheet->GetString();

		CTreeCtrlNode newNode = PasteString(nodeSelected, str);
		nodeSelected.Select();

		// special case for USE
		if (nImageIndex == useImage)
		{
			COCKSUse* pUse = static_cast<COCKSUse*>(pKeywordSheet);
			ASSERT_VALID(pUse);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pUse->GetDeleteNodes().begin();
			for (; nodeIter != pUse->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
		// special case for SAVE
		if (nImageIndex == saveImage)
		{
			COCKSSave* pSave = static_cast<COCKSSave*>(pKeywordSheet);
			ASSERT_VALID(pSave);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pSave->GetDeleteNodes().begin();
			for (; nodeIter != pSave->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
		// special case for COPY
		if (nImageIndex == copyImage)
		{
			COCKSCopy* pCopy = static_cast<COCKSCopy*>(pKeywordSheet);
			ASSERT_VALID(pCopy);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pCopy->GetDeleteNodes().begin();
			for (; nodeIter != pCopy->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
	}
	else
	{
		VERIFY(nodeSelected.Select());
		VERIFY(nodeTemp.Delete());
	}
	delete pKeywordSheet;
	SetFocus();
}

// if selected node is keyword or keyword option:
//     insert keyword after selected keyword
//
//     if selected node is END then insert keyword
//     as first keyword of next sim
//
// if selected node is sim:
//     Insert keyword as first keyword of the sim
//
// if selected node is file:
//     Insert keyword as first keyword of the file
//
void CTreeCtrlIn::OnKeyA(UINT nID) 
{
	// display hourglass
	CWaitCursor wait;

	CTreeCtrlNode nodeSelected = GetSelectedItem();

	ASSERT(nodeSelected);
	//{{
	// set active document
	// required if database is needed
	GetRichEditView(nodeSelected)->GetParentFrame()->ActivateFrame();
	//}}

	CTreeCtrlNode nodeSimToAddTo;
	CTreeCtrlNode nodeToInsertAfter;

	switch (nodeSelected.GetLevel())
	{
	case KeywordOptionLevel :
		nodeSelected = nodeSelected.GetParent();
		// fall through
	case KeywordLevel :
		if (nodeSelected.GetImageID() == endImage)
		{
			nodeSimToAddTo = nodeSelected.GetParent().GetNextSibling();
			ASSERT(nodeSimToAddTo); // must be sim folling END
		}
		else
		{
			nodeSimToAddTo = nodeSelected.GetParent();
			nodeToInsertAfter = nodeSelected;
		}
		break;

	case SimLevel :
		nodeSimToAddTo = nodeSelected;
		break;

	case FileLevel :
		nodeSimToAddTo = nodeSelected.GetChild();
		break;

	case WorkLevel :
		ASSERT(FALSE);
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	if (!nodeToInsertAfter)
	{
		nodeToInsertAfter = CTreeCtrlNode(TVI_FIRST, this);
	}

	// determine node to paste to
	CTreeCtrlNode nodePaste;
	if (nodeToInsertAfter != (HTREEITEM)TVI_FIRST)
	{
		nodePaste = nodeToInsertAfter.GetNextSibling();
		if (nodePaste == 0)
		{
			nodePaste = nodeSimToAddTo;
		}
	}
	else
	{
		nodePaste = nodeSimToAddTo.GetChild();
		if (nodePaste == 0)
		{
			nodePaste = nodeSimToAddTo;
			ASSERT(nodePaste.HasChildren() == FALSE);
		}
	}
	ASSERT(nodePaste);

	//{{COMMON
	// determine which keyword to create
	CKeywordSheet* pKeywordSheet = NULL;
	CString strLabel;
	CTreeCtrlIn::ImageIndex nImageIndex = unusedImage;
	
	switch (nID)
	{
	case ID_KEY_END_A :
		strLabel = _T("END\r\n");
		ASSERT(nodePaste);
		PasteString(nodePaste, strLabel);
		break;
	case ID_KEY_USE_A :
		strLabel = _T("USE...");
		nImageIndex = useImage;
		pKeywordSheet = new COCKSUse(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_TITLE_A :
		strLabel = _T("TITLE...");
		nImageIndex = titleImage;
		pKeywordSheet = new CKSTitle();
		break;
	case ID_KEY_EQUILIBRIUM_PHASES_A :
		strLabel = _T("EQUILIBRIUM_PHASES...");
		nImageIndex = equilibrium_phasesImage;
		pKeywordSheet = new CCKSEquilibrium_Phases();
		break;
	case ID_KEY_GAS_PHASE_A :
		strLabel = _T("GAS_PHASE...");
		nImageIndex = gas_phaseImage;
		pKeywordSheet = new CCKSGas_Phase(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_SAVE_A :
		strLabel = _T("SAVE...");
		nImageIndex = saveImage;
		pKeywordSheet = new COCKSSave(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_EXCHANGE_A :
		strLabel = _T("EXCHANGE...");
		nImageIndex = exchangeImage;
		pKeywordSheet = new CCKSExchange(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_MIX_A :
		strLabel = _T("MIX...");
		nImageIndex = mixImage;
		pKeywordSheet = new CCKSMix(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_REACTION_A :
		strLabel = _T("REACTION...");
		nImageIndex = reactionImage;
		pKeywordSheet = new CCKSReaction();
		break;
	case ID_KEY_REACTION_TEMPERATURE_A :
		strLabel = _T("REACTION_TEMPERATURE...");
		nImageIndex = reaction_temperatureImage;
		pKeywordSheet = new CCKSReaction_Temperature();
		break;
	case ID_KEY_SOLUTION_A :
		strLabel = _T("SOLUTION...");
		nImageIndex = solutionImage;
		pKeywordSheet = new CCKSSolution();
		break;
	case ID_KEY_SOLID_SOLUTIONS_A :
		strLabel = _T("SOLID_SOLUTIONS...");
		nImageIndex = solid_solutionsImage;
		pKeywordSheet = new CCKSSolid_Solutions();
		break;
	case ID_KEY_RATES_A :
		strLabel = _T("RATES...");
		nImageIndex = ratesImage;
		pKeywordSheet = new COCKSRates();
		break;
	case ID_KEY_INVERSE_MODELING_A :
		strLabel = _T("INVERSE_MODELING...");
		nImageIndex = inverse_modelingImage;
		pKeywordSheet = new CCKSInverse(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_USER_PRINT_A :
		strLabel = _T("USER_PRINT...");
		nImageIndex = user_printImage;
		pKeywordSheet = new COCKSUserPrint();
		break;
	case ID_KEY_USER_PUNCH_A :
		strLabel = _T("USER_PUNCH...");
		nImageIndex = user_punchImage;
		pKeywordSheet = new COCKSUserPunch();
		break;
	case ID_KEY_SURFACE_A :
		strLabel = _T("SURFACE...");
		nImageIndex = surfaceImage;
		pKeywordSheet = new CSurfaceSheet(NULL, nodeSimToAddTo);
		break;
	case ID_KEY_PRINT_A :
		strLabel = _T("PRINT...");
		nImageIndex = printImage;
		pKeywordSheet = new CKSPrint();
		break;
	case ID_KEY_KINETICS_A :
		strLabel = _T("KINETICS...");
		nImageIndex = kineticsImage;
		pKeywordSheet = new CCKSKinetics();
	case ID_KEY_INCREMENTAL_REACTIONS_A :
		strLabel = _T("INCREMENTAL_REACTIONS...");
		nImageIndex = incremental_reactionsImage;
		pKeywordSheet = new CKSIncrement();
	case ID_KEY_SELECTED_OUTPUT_A :
		strLabel = _T("SELECTED_OUTPUT...");
		nImageIndex = selected_outputImage;
		pKeywordSheet = new CKSSelectedOutput();
		break;
	case ID_KEY_ADVECTION_A :
		strLabel = _T("ADVECTION...");
		nImageIndex = advectionImage;
		pKeywordSheet = new COCKSAdvection();
		break;
	case ID_KEY_TRANSPORT_A :
		strLabel = _T("TRANSPORT...");
		nImageIndex = transportImage;
		pKeywordSheet = new COCKSTransport();
		break;
	case ID_KEY_SOLUTION_SPREAD_A :
		strLabel = _T("SOLUTION_SPREAD...");
		nImageIndex = solution_spreadImage;
		pKeywordSheet = new COCKSSolution_Spread();
		break;
	case ID_KEY_SOLUTION_SPECIES_A :
		strLabel = _T("SOLUTION_SPECIES...");
		nImageIndex = solution_speciesImage;
		pKeywordSheet = new CKSSolutionSpecies();
		break;
	case ID_KEY_SURFACE_SPECIES_A :
		strLabel = _T("SURFACE_SPECIES...");
		nImageIndex = surface_speciesImage;
		pKeywordSheet = new CKSSurfaceSpecies();
		break;
	case ID_KEY_EXCHANGE_SPECIES_A :
		strLabel = _T("EXCHANGE_SPECIES...");
		nImageIndex = exchange_speciesImage;
		pKeywordSheet = new CKSExchangeSpecies();
		break;
	case ID_KEY_PHASES_A :
		strLabel = _T("PHASES...");
		nImageIndex = phasesImage;
		pKeywordSheet = new CKSPhases();
		break;
	case ID_KEY_SOLUTION_MASTER_SPECIES_A :
		strLabel = _T("SOLUTION_MASTER_SPECIES...");
		nImageIndex = solution_master_speciesImage;
		pKeywordSheet = new CKSSolutionMasterSpecies();
		break;
	case ID_KEY_EXCHANGE_MASTER_SPECIES_A :
		strLabel = _T("EXCHANGE_MASTER_SPECIES...");
		nImageIndex = exchange_master_speciesImage;
		pKeywordSheet = new CKSExchangeMasterSpecies();
		break;
// COMMENT: {2/16/2012 4:59:47 PM}	case ID_KEY_SURFACE_MASTER_SPECIES_A :
// COMMENT: {2/16/2012 4:59:47 PM}		strLabel = _T("SURFACE_MASTER_SPECIES...");
// COMMENT: {2/16/2012 4:59:47 PM}		nImageIndex = surface_master_speciesImage;
// COMMENT: {2/16/2012 4:59:47 PM}		pKeywordSheet = new CKSSurfaceMasterSpecies();
// COMMENT: {2/16/2012 4:59:47 PM}		break;
// COMMENT: {2/16/2012 4:59:47 PM}	case ID_KEY_KNOBS_A :
// COMMENT: {2/16/2012 4:59:47 PM}		strLabel = _T("KNOBS...");
// COMMENT: {2/16/2012 4:59:47 PM}		nImageIndex = knobsImage;
// COMMENT: {2/16/2012 4:59:47 PM}		pKeywordSheet = new CKSKnobs();
// COMMENT: {2/16/2012 4:59:47 PM}		break;
	case ID_KEY_COPY_A :
		strLabel = _T("COPY...");
		nImageIndex = copyImage;
		pKeywordSheet = new COCKSCopy(NULL, nodeSimToAddTo);
		break;
// COMMENT: {2/16/2012 4:59:47 PM}	case ID_KEY_PITZER_A :
// COMMENT: {2/16/2012 4:59:47 PM}		strLabel = _T("PITZER...");
// COMMENT: {2/16/2012 4:59:47 PM}		nImageIndex = pitzerImage;
// COMMENT: {2/16/2012 4:59:47 PM}		pKeywordSheet = new CKSPitzer();
// COMMENT: {2/16/2012 4:59:47 PM}		break;
// COMMENT: {2/16/2012 4:59:47 PM}	case ID_KEY_SIT_A :
// COMMENT: {2/16/2012 4:59:47 PM}		strLabel = _T("SIT...");
// COMMENT: {2/16/2012 4:59:47 PM}		nImageIndex = sitImage;
// COMMENT: {2/16/2012 4:59:47 PM}		pKeywordSheet = new CKSSIT();
// COMMENT: {2/16/2012 4:59:47 PM}		break;
	//{{NEW KEYWORD HERE}}
	}

	if (pKeywordSheet == NULL) return;

	if (pKeywordSheet->IsKindOf(RUNTIME_CLASS(CCommonKeywordSheet)))
	{
		((CCommonKeywordSheet*)pKeywordSheet)->m_n_user = GetNextNum(nodeSelected, nImageIndex);
	}



	ASSERT(nodeSimToAddTo);
	CTreeCtrlNode nodeTemp = nodeSimToAddTo.InsertAfter(strLabel, nodeToInsertAfter, nImageIndex);
	nodeTemp.Select();

	// set active document
	// required if database is needed
	GetRichEditView(nodeSelected)->GetParentFrame()->ActivateFrame();
	if (pKeywordSheet && pKeywordSheet->bNeedDatabase)
	{
		int nIndex = nodeSimToAddTo.GetParent().GetIndex(nodeSimToAddTo);
		ASSERT(nIndex != -1);
		((CRichDocIn*)GetRichEditDoc(nodeSelected))->UpdateDatabase(nIndex + 1);
	}

	if (pKeywordSheet && pKeywordSheet->DoModal() == IDOK)
	{
		VERIFY(nodeTemp.Delete());
		ASSERT(nodePaste);
		
		// verify nodePaste still exists
		char szLabel[20];
		TVITEM tvi;
		tvi.hItem		= nodePaste;
		tvi.mask		= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText		= szLabel;
		tvi.cchTextMax	= 20;
		ASSERT(GetItem(&tvi));

		CString str = pKeywordSheet->GetString();
		ASSERT(!str.IsEmpty());

		CTreeCtrlNode newNode = PasteString(nodePaste, str);
		nodeSelected.Select();

		// special case for USE
		if (nImageIndex == useImage)
		{
			COCKSUse* pUse = static_cast<COCKSUse*>(pKeywordSheet);
			ASSERT_VALID(pUse);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pUse->GetDeleteNodes().begin();
			for (; nodeIter != pUse->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
		// special case for SAVE
		if (nImageIndex == saveImage)
		{
			COCKSSave* pSave = static_cast<COCKSSave*>(pKeywordSheet);
			ASSERT_VALID(pSave);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pSave->GetDeleteNodes().begin();
			for (; nodeIter != pSave->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
		// special case for COPY
		if (nImageIndex == copyImage)
		{
			COCKSCopy* pCopy = static_cast<COCKSCopy*>(pKeywordSheet);
			ASSERT_VALID(pCopy);
			std::list<CTreeCtrlNode>::const_iterator nodeIter = pCopy->GetDeleteNodes().begin();
			for (; nodeIter != pCopy->GetDeleteNodes().end(); ++nodeIter)
			{
				ClearNode((*nodeIter));
			}
		}
	}
	else
	{
		VERIFY(nodeSelected.Select());
		VERIFY(nodeTemp.Delete());
	}
	delete pKeywordSheet;
	SetFocus();
	//}}COMMON
}

void CTreeCtrlIn::DoKeyword(UINT nID, CTreeCtrlNode nodeToPasteTo)
{
	// verify command ID
	ASSERT(ID_KEY_END <= nID && nID <= ID_KEY_SOLUTION_SPREAD);

	// validate nodeToPasteTo
	ASSERT(nodeToPasteTo);
	TVITEM tvi     = {0};
	tvi.hItem		= (HTREEITEM)nodeToPasteTo;
	tvi.mask		= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;
	ASSERT(GetItem(&tvi));

	// determine sim to add to and keyword before
	// required to insert temp keyword node
	CTreeCtrlNode nodeSimToAddTo;
	CTreeCtrlNode nodeKeyBefore;
	int nLevel = nodeToPasteTo.GetLevel();
	if (nLevel == KeywordLevel)
	{
		nodeSimToAddTo = nodeToPasteTo.GetParent();
		nodeKeyBefore = nodeToPasteTo.GetPrevSibling();
		if (nodeKeyBefore == 0)
		{
			nodeKeyBefore = CTreeCtrlNode(TVI_FIRST, this);
		}
	}
	else if (nLevel == SimLevel)
	{
		nodeSimToAddTo = nodeToPasteTo;
		nodeKeyBefore = nodeSimToAddTo.GetLastChild();
		if (nodeKeyBefore == 0)
		{
			nodeKeyBefore = CTreeCtrlNode(TVI_FIRST, this);
		}
		else
		{
			// if pasting to sim the sim must not have an END keyword
			ASSERT(nodeKeyBefore.GetImageID() != endImage);
		}
	}
	else
	{
		ASSERT(FALSE);	// nodeToPasteTo must be keyword or simulation
	}

	// verify sim
	ASSERT(nodeSimToAddTo && nodeSimToAddTo.GetLevel() == SimLevel);
	
	// verify key before
	ASSERT(nodeKeyBefore &&
		((HTREEITEM)nodeKeyBefore == TVI_FIRST || nodeKeyBefore.GetLevel() == KeywordLevel));


	// determine which keyword sheet to display
	CKeywordSheet* pKeywordSheet = NULL;
	CString strLabel;
	int nImageIndex;
	
	switch (nID)
	{
	case ID_KEY_USE :
		strLabel = _T("USE...");
		nImageIndex = useImage;
		pKeywordSheet = new COCKSUse(NULL, nodeSimToAddTo);
		break;

	case ID_KEY_TITLE :
		strLabel = _T("TITLE...");
		nImageIndex = titleImage;
		pKeywordSheet = new CKSTitle();
		break;

	case ID_KEY_END :
// COMMENT: {5/24/2000 6:49:53 PM}		strLabel = _T("END\r\n");
// COMMENT: {5/24/2000 6:49:28 PM}		ASSERT(nodePaste);
// COMMENT: {5/24/2000 6:49:28 PM}		//{{CHECK
// COMMENT: {5/24/2000 6:49:28 PM}		PasteString(nodePaste, strLabel).Select();
// COMMENT: {5/24/2000 6:49:28 PM}		//}}CHECK
		break;
	}


}

void CTreeCtrlIn::OnHelp() 
{
	// Add your command handler code here
	CTreeCtrlNode node = GetSelectedItem();

	if (node.GetLevel() == KeywordOptionLevel)
		node = node.GetParent();

	// create path to phreeqci.chm
	//
	CPhreeqciApp* pApp = (CPhreeqciApp*)::AfxGetApp();
	CString chm = pApp->m_settings.m_strHelpDirectory;
	chm.Append(_T("phreeqci.chm"));

	if (node.GetLevel() == KeywordLevel)
	{
		CKeyword::type nType = GetKeywordType(static_cast<CTreeCtrlIn::ImageIndex>(node.GetImageID()));
		CString strIndex = CKeyword::GetHelpIndex(nType);

		if (strIndex.IsEmpty())
		{
			VERIFY(::HtmlHelp(::GetDesktopWindow(), chm, HH_DISPLAY_TOPIC, (DWORD)NULL));
		}
		else
		{
			VERIFY(::HtmlHelp(::GetDesktopWindow(), chm, HH_DISPLAY_TOPIC, (DWORD)(LPCTSTR)strIndex));
		}
	}
	else
	{	
		VERIFY(::HtmlHelp(::GetDesktopWindow(), chm, HH_DISPLAY_TOPIC, (DWORD)NULL));
	}
}

int CTreeCtrlIn::GetNextNum(const CTreeCtrlNode& node, enum CTreeCtrlIn::ImageIndex nImageIndex)
{
	int nNext = 0;

	CTreeCtrlNode fileNode = GetFileNode(node);
	ASSERT(fileNode);

	int n_user, n_user_end;
	char * description;
	CString strKey;

	Phreeqc i;
	CTreeCtrlNode simNode = fileNode.GetChild();
	for (; simNode != 0; simNode = simNode.GetNextSibling())
	{
		CTreeCtrlNode keyNode = simNode.GetChild();
		for (; keyNode != 0; keyNode = keyNode.GetNextSibling())
		{
			if (nImageIndex != static_cast<enum CTreeCtrlIn::ImageIndex>(keyNode.GetImageID()))
				continue;

			strKey = keyNode.GetText();

			i.read_number_description(strKey.GetBuffer(strKey.GetLength() + 1),
				&n_user, &n_user_end, &description);
			i.PHRQ_free(description);
			nNext = (nNext > n_user_end) ? nNext : n_user_end;
		}
	}

	return nNext + 1;
}

void CTreeCtrlIn::OnSetFocus(CWnd* pOldWnd) 
{
	baseCTreeCtrlIn::OnSetFocus(pOldWnd);
	
	// Add your message handler code here
	
}

void CTreeCtrlIn::OnFileCloseAll() 
{
	// make list of files to close
	std::list<CRichDocIn*> listInputFiles;
	CTreeCtrlNode fileNode = m_baseNode.GetChild();
	ASSERT(fileNode);
	
	while (fileNode)
	{
		ASSERT(fileNode.GetLevel() == FileLevel);
		listInputFiles.push_back((CRichDocIn*)GetRichEditDoc(fileNode));
		fileNode = fileNode.GetNextSibling();
	}

	std::list<CRichDocIn*>::iterator iterFile = listInputFiles.begin();
	for (; iterFile != listInputFiles.end(); ++iterFile)
	{
		(*iterFile)->OnFileClose();
	}
}

void CTreeCtrlIn::OnUpdateFileCloseAll(CCmdUI* pCmdUI) 
{	
	pCmdUI->Enable(TRUE);	
}

/**
Removes the DATABASE keyword line from the input file and sets
the database appropriately if it exists.

  Assumes that the DATABASE keyword must be the first keyword
  in the file.

  Assumes that the file is error free.
*/
void CTreeCtrlIn::RemoveDatabaseKeyword(CRichEditDoc *pDoc)
{
	CRichEditLineParser lineParser(pDoc->GetView()->m_hWnd);
	CRichLogicalLineParser logicalLineParser(lineParser);
	CRichKeywordParser keyParser(logicalLineParser);

	// skip non-keyword lines
	while (keyParser.GetLine() != CKeywordParser::typeKeyword)
	{
		if (keyParser.type == CKeywordParser::typeEof)
		{
			break;
		}
	}
	if (keyParser.last_keyword == CKeyword::K_DATABASE)
	{
		BOOL bModified = pDoc->IsModified();

		CString strDBPathName = keyParser.line.Mid(8);
		strDBPathName.TrimLeft();
		strDBPathName.TrimRight();

		// convert unix paths to dos paths
		strDBPathName.Replace(_T('/'), _T('\\'));

		if (::PathIsRelative(strDBPathName))
		{
			// convert strDBPathName to absolute path
			TCHAR szPath[MAX_PATH];
			TCHAR szOut[MAX_PATH];
			VERIFY(::GetCurrentDirectory(MAX_PATH, szPath));
			VERIFY(::PathAddBackslash(szPath));
			VERIFY(::PathAppend(szPath, strDBPathName));
			VERIFY(::PathCanonicalize(szOut, szPath));
			strDBPathName = szOut;
		}

#ifdef SAVE_EXPAND_ENVIR
		TCHAR infoBuf[32767];
		DWORD bufCharCount = 32767;
		bufCharCount = ExpandEnvironmentStrings(strDBPathName, infoBuf, 32767); 
		strDBPathName = infoBuf;
#endif
		// check if database exists
		CPhreeqciApp* pApp = (CPhreeqciApp*)AfxGetApp();
		if (!CUtil::FileExists(strDBPathName))
		{
			ASSERT(CUtil::FileExists(pApp->m_settings.m_strDefDBPathName));
			CString warning;
			AfxFormatString2(warning, IDS_MISSING_DB_171, strDBPathName, pApp->m_settings.m_strDefDBPathName);

			// Note: MessageBox is now modal.  This fixes
			// a bug that occured if user attempted to 
			// exit program without responding to the
			// MessageBox,
			//
			// If opening more than one file this will not
			// allow any remaining files to be opened
			HWND hWndTop;
			HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);
			::MessageBox(hWnd, warning, _T("Missing database"), MB_OK | MB_ICONWARNING);
			bModified = TRUE;

			strDBPathName = pApp->m_settings.m_strDefDBPathName;
		}
		else
		{
			try
			{
				pApp->PreLoadDatabase(strDBPathName);
			}
			catch (CException* p)
			{
				// TODO
				throw p;
			}
		}

		ASSERT_KINDOF(CRichDocIn, pDoc);
		((CRichDocIn*)pDoc)->m_props.m_strDBPathName = strDBPathName;

		CHARRANGE cr = keyParser.cr;
		pDoc->GetView()->GetRichEditCtrl().SetSel(cr);
		pDoc->GetView()->GetRichEditCtrl().Clear();
		if (!bModified)
		{
			pDoc->GetView()->GetRichEditCtrl().EmptyUndoBuffer();
			pDoc->SetModifiedFlag(FALSE);
		}
	}
}

BOOL CTreeCtrlIn::PreTranslateMessage(MSG* pMsg) 
{
	// Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// for some reason NM_RETURN is not recieved
		// nor is OnKeyDown(VK_RETURN)
		CTreeCtrlNode node = GetSelectedItem();
		CRichEditView* pView = GetRichEditView(node);
		if (pView)
		{
			CWaitCursor wait;

			// set active document
			pView->GetParentFrame()->ActivateFrame();

			// select text
			CRichEditCtrl& rich = pView->GetRichEditCtrl();
			CHARRANGE cr = GetRange(node);
			rich.SetSel(cr.cpMin, cr.cpMin);

			// scroll into view
			rich.LineScroll(rich.LineFromChar(-1) - rich.GetFirstVisibleLine());
			pView->SetFocus();
		}
	}
	
	return baseCTreeCtrlIn::PreTranslateMessage(pMsg);
}

void CTreeCtrlIn::OnUpdateRunAll(CCmdUI* pCmdUI) 
{
	// Add your command update UI handler code here
	pCmdUI->Enable(TRUE);	
}

void CTreeCtrlIn::OnRunAll() 
{
	// make list of files to close
	std::list<CRichDocIn*> listInputFiles;
	CTreeCtrlNode fileNode = m_baseNode.GetChild();
	ASSERT(fileNode);
	
	while (fileNode)
	{
		ASSERT(fileNode.GetLevel() == FileLevel);
		listInputFiles.push_back((CRichDocIn*)GetRichEditDoc(fileNode));
		fileNode = fileNode.GetNextSibling();
	}

	std::list<CRichDocIn*>::iterator iterFile = listInputFiles.begin();
	for (; iterFile != listInputFiles.end(); ++iterFile)
	{
		 if (!(*iterFile)->DoFileSave()) return;
	}

	iterFile = listInputFiles.begin();
	for (; iterFile != listInputFiles.end(); ++iterFile)
	{
		 (*iterFile)->OnRunInput();
	}

}

void CTreeCtrlIn::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CTreeCtrlIn::OnFileSaveAll() 
{
	// make list of files to close
	std::list<CRichDocIn*> listInputFiles;
	CTreeCtrlNode fileNode = m_baseNode.GetChild();
	ASSERT(fileNode);
	
	while (fileNode)
	{
		ASSERT(fileNode.GetLevel() == FileLevel);
		listInputFiles.push_back((CRichDocIn*)GetRichEditDoc(fileNode));
		fileNode = fileNode.GetNextSibling();
	}

	std::list<CRichDocIn*>::iterator iterFile = listInputFiles.begin();
	for (; iterFile != listInputFiles.end(); ++iterFile)
	{
		 if (!(*iterFile)->DoFileSave()) break;
	}
}

void CTreeCtrlIn::OnGoto() 
{
	CTreeCtrlNode node = GetSelectedItem();
	CRichEditView* pView = GetRichEditView(node);
	if (pView)
	{
		CWaitCursor wait;

		// set active document
		pView->GetParentFrame()->ActivateFrame();

		// select text
		CRichEditCtrl& rich = pView->GetRichEditCtrl();
		CHARRANGE cr = GetRange(node);
		rich.SetSel(cr.cpMin, cr.cpMin);

		// scroll into view
		rich.LineScroll(rich.LineFromChar(-1) - rich.GetFirstVisibleLine());
		pView->SetFocus();
	}
}
