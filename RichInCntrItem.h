// RichInCntrItem.h : interface of the CRichInCntrItem class
//
// $Date: 2000/10/04 20:17:53 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHINCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_)
#define AFX_RICHINCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRichDocIn;
class CRichViewIn;

class CRichInCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CRichInCntrItem)

// Constructors
public:
	CRichInCntrItem(REOBJECT* preo = NULL, CRichDocIn* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CRichDocIn* GetDocument()
		{ return (CRichDocIn*)CRichEditCntrItem::GetDocument(); }
	CRichViewIn* GetActiveView()
		{ return (CRichViewIn*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichInCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CRichInCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHINCNTRITEM_H__3E5408F7_AABA_4BC7_8E0C_B4B632672EA9__INCLUDED_)
