// TreeCtrlIn.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_)
#define AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlIn window

#include "Tree.h"

#ifndef baseCTreeCtrlIn
#define baseCTreeCtrlIn CTreeCtrlEx
#endif

#include "Keyword.h"
#include "InputTreeOleDropTarget.h"	// Added by ClassView

// forward declaration
class CWorkspaceBar;

class CTreeCtrlIn : public baseCTreeCtrlIn
{
	DECLARE_DYNAMIC(CTreeCtrlIn)
// Construction
public:
	CTreeCtrlIn();

// Attributes
public:

// Operations
public:
	/***
		K_SOLUTION_SPECIES        =  2,
		K_SOLUTION_MASTER_SPECIES =  3,
		K_PHASES                  =  5,
		K_EXCHANGE_SPECIES        = 11,
		K_EXCHANGE_MASTER_SPECIES = 12,
		K_SURFACE_SPECIES         = 14,
		K_SURFACE_MASTER_SPECIES  = 15,
		K_INVERSE_MODELING        = 18,
		K_TRANSPORT               = 20,
		K_SELECTED_OUTPUT         = 22,
		K_KNOBS                   = 24,
		K_PRINT                   = 25,
		//      New keywords added by version 2
		K_ADVECTION               = 32,
		K_KINETICS                = 33,
		K_INCREMENTAL_REACTIONS   = 34,
		K_RATES                   = 36,
		K_USER_PRINT              = 38,
		K_USER_PUNCH              = 39,
		K_SOLUTION_SPREAD         = 42,
		K_USER_GRAPH              = 46,
	***/
	enum ImageIndex
	{
		simImage,
		unusedImage,
		workImage,
		fileImage,
		endImage,                     // K_END
		solutionImage,                // K_SOLUTION
		exchangeImage,                // K_EXCHANGE
		gas_phaseImage,               // K_GAS_PHASE
		equilibrium_phasesImage,      // K_EQUILIBRIUM_PHASES
		solid_solutionsImage,         // K_SOLID_SOLUTIONS
		surfaceImage,                 // K_SURFACE
		useImage,                     // K_USE
		mixImage,                     // K_MIX
		reactionImage,                // K_REACTION
		reaction_temperatureImage,    // K_REACTION_TEMPERATURE
		saveImage,                    // K_SAVE
		titleImage,                   // K_TITLE
		genericKeyImage,
		optionImage,
		UnSyncOverlayImage,
		ErrorOverlayImage,
		solution_speciesImage,        // K_SOLUTION_SPECIES
		solution_master_speciesImage, // K_SOLUTION_MASTER_SPECIES
		phasesImage,                  // K_PHASES
		exchange_speciesImage,        // K_EXCHANGE_SPECIES
		exchange_master_speciesImage, // K_EXCHANGE_MASTER_SPECIES
		surface_speciesImage,         // K_SURFACE_SPECIES
		surface_master_speciesImage,  // K_SURFACE_MASTER_SPECIES
		inverse_modelingImage,        // K_INVERSE_MODELING
		transportImage,               // K_TRANSPORT
		selected_outputImage,         // K_SELECTED_OUTPUT
		knobsImage,                   // K_KNOBS
		printImage,                   // K_PRINT
		advectionImage,               // K_ADVECTION
		kineticsImage,                // K_KINETICS
		incremental_reactionsImage,   // K_INCREMENTAL_REACTIONS
		ratesImage,                   // K_RATES
		user_printImage,              // K_USER_PRINT
		user_punchImage,              // K_USER_PUNCH
		solution_spreadImage,         // K_SOLUTION_SPREAD
		user_graphImage,              // K_USER_GRAPH
		llnl_aqueous_model_parametersImage,  // K_LLNL_AQUEOUS_MODEL_PARAMETERS
		copyImage,                    // K_COPY
		pitzerImage,                  // K_PITZER
		sitImage,                     // K_SIT
		reaction_pressureImage,       // K_REACTION_PRESSURE
	};
	enum NodeLevel
	{
		WorkLevel          = 0,
		FileLevel          = 1,
		SimLevel           = 2,
		KeywordLevel       = 3,
		KeywordOptionLevel = 4,
	};
	enum OverlayState
	{
		SyncState   = 0,
		UnSyncState = 1,
		ErrorState  = 2,
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlIn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetNextNum(const CTreeCtrlNode& node, enum CTreeCtrlIn::ImageIndex nImageIndex);
	CTreeCtrlNode GetNextKeywordNode(const CTreeCtrlNode& rNode)const;
	CTreeCtrlNode GetPrevKeywordNode(const CTreeCtrlNode& rNode)const;
	bool GetClipBoardData(CString& rStr);
	long GetInsertPosBeforeKeyword(const CTreeCtrlNode& node);
	long GetInsertPosAfterKeyword(const CTreeCtrlNode& node);
	CTreeCtrlNode PasteString(CTreeCtrlNode nodeToPasteTo, CString& rStr);
	CWorkspaceBar& GetWorkspaceBar();
	void UpdateAllViews(CDocument *pDoc, LPARAM lHint, CObject *pHint);
	void RenumberSimulations(CTreeCtrlNode fileNode, CTreeCtrlNode firstSimNode = CTreeCtrlNode());
	CTreeCtrlNode GetFileNode(CRichEditDoc* pDoc);
	CTreeCtrlNode GetFileNode(const CTreeCtrlNode& childNode)const;
	CTreeCtrlNode m_baseNode;
	CTreeCtrlNode ParseTree(CRichEditDoc* pDoc, int nErrors = 0);
	CRichEditDoc* GetRichEditDoc(const CTreeCtrlNode& node)const;
	CRichEditView* GetRichEditView(const CTreeCtrlNode& node)const;
	CHARRANGE GetRange(CTreeCtrlNode node)const;
	static enum CTreeCtrlIn::ImageIndex GetImageIndex(enum CKeyword::type nType);
	static enum CKeyword::type GetKeywordType(enum CTreeCtrlIn::ImageIndex nImageID);
	CImageList m_ctrlImage;

	afx_msg void OnUpdateKey(CCmdUI* pCmdUI);
	afx_msg void OnKey(UINT nID);
	afx_msg void OnKeyA(UINT nID);

	virtual ~CTreeCtrlIn();

	// Generated message map functions
protected:
	void RemoveDatabaseKeyword(CRichEditDoc *pDoc);

	CTreeCtrlNode ReplaceNode(CTreeCtrlNode nodeToReplace, CString& rStr);
	void ClearNode(CTreeCtrlNode node);
	void CopyNode(CTreeCtrlNode node);
	CString GetNodeString(const CTreeCtrlNode& rNode)const;
	CString GetTransportNodeString(const CTreeCtrlNode& rNode)const;
	CString CTreeCtrlIn::GetPreviousString(const CTreeCtrlNode& rNode)const;

	void DoKeyword(UINT nID, CTreeCtrlNode nodeToPasteTo);

	HTREEITEM m_hDragTreeItem;
	CLIPFORMAT m_nIDClipFormat;
	CInputTreeOleDropTarget m_OleDropTarget;
	CImageList* m_pDragImageList;
	friend class CWorkspaceBar;

	//{{AFX_MSG(CTreeCtrlIn)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnDebugSelect();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditKeyword(CCmdUI* pCmdUI);
	afx_msg void OnEditKeyword();
	afx_msg void OnUpdateViewProperties(CCmdUI* pCmdUI);
	afx_msg void OnViewProperties();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnFileClose();
	afx_msg void OnDebugInsertBef();
	afx_msg void OnDebugInsertAft();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnPopupClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRunInput();
	afx_msg void OnUpdateRunInput(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunAll(CCmdUI* pCmdUI);
	afx_msg void OnRunAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnGoto();
	//}}AFX_MSG

	// keywords
// COMMENT: {10/14/2009 4:03:20 PM}	afx_msg void OnUpdateKey(CCmdUI* pCmdUI);
// COMMENT: {10/14/2009 4:03:20 PM}	afx_msg void OnKey(UINT nID);
// COMMENT: {10/14/2009 4:03:20 PM}	afx_msg void OnKeyA(UINT nID);

	afx_msg void OnUpdateNeedDocSel(CCmdUI* pCmdUI);

	// drag and drop operations
	DROPEFFECT GetDropEffect(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	friend class CInputTreeOleDropTarget;
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void OnDragLeave();
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	void SelectDropNode(CPoint point);

	// tree notifications
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_)
