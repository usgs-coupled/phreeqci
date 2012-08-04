#pragma once

// CRichDocDB document

class CRichDocDB : public CRichEditDoc
{
protected:
	CRichDocDB();
	DECLARE_DYNCREATE(CRichDocDB)

public:
	virtual ~CRichDocDB();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
};
