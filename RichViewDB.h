#pragma once


// CRichViewDB view

class CRichViewDB : public CRichEditView
{
	DECLARE_DYNCREATE(CRichViewDB)

protected:
	CRichViewDB();           // protected constructor used by dynamic creation
	virtual ~CRichViewDB();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	bool m_bOnInitialUpdateCalled;
	CHARFORMAT m_cf;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPasteSpecial(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOleEditProperties(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOleInsertNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAll(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
};


