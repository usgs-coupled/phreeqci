// RichOutCntrItem.h : interface of the CRichOutCntrItem class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHOUTCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_)
#define AFX_RICHOUTCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRichDocOut;
class CRichViewOut;

class CRichOutCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CRichOutCntrItem)

// Constructors
public:
	CRichOutCntrItem(REOBJECT* preo = NULL, CRichDocOut* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CRichDocOut* GetDocument()
		{ return (CRichDocOut*)CRichEditCntrItem::GetDocument(); }
	CRichViewOut* GetActiveView()
		{ return (CRichViewOut*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichOutCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CRichOutCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHOUTCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_)
