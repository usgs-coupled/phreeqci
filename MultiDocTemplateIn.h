// MultiDocTemplateIn.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIDOCTEMPLATEIN_H__BAD9C513_B493_4402_9102_6D640592B8A9__INCLUDED_)
#define AFX_MULTIDOCTEMPLATEIN_H__BAD9C513_B493_4402_9102_6D640592B8A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplateIn command target

#ifndef baseMultiDocTemplateIn
#define baseMultiDocTemplateIn CMultiDocTemplate
#endif

class CMultiDocTemplateIn : public baseMultiDocTemplateIn
{
	DECLARE_DYNAMIC(CMultiDocTemplateIn)

// Constructors
public:
	CMultiDocTemplateIn( UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass );

// Attributes
public:

// Operations
public:

// Overrides
public:
	//{{CDocTemplate Virtual Functions
	virtual Confidence MatchDocType(LPCTSTR lpszPathName,
					CDocument*& rpDocMatch);
	//virtual CDocument* CreateNewDocument();
	//virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);
	//virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc,
	//	BOOL bMakeVisible = TRUE);
	//virtual BOOL SaveAllModified();     // for all documents
	//virtual void CloseAllDocuments(BOOL bEndSession);
	//virtual CDocument* OpenDocumentFile(
	//	LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) = 0;
	//				// open named file
	//				// if lpszPathName == NULL => create new file with this type
	//virtual void SetDefaultTitle(CDocument* pDocument) = 0;
	//}}CDocTemplate Virtual Functions

	//{{CMultiDocTemplate Virtual Functions
	//virtual void LoadTemplate();
	//virtual void AddDocument(CDocument* pDoc);
	//virtual void RemoveDocument(CDocument* pDoc);
	//virtual POSITION GetFirstDocPosition() const;
	//virtual CDocument* GetNextDoc(POSITION& rPos) const;
	//virtual CDocument* OpenDocumentFile(
	//	LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	//virtual void SetDefaultTitle(CDocument* pDocument);
	//}}CMultiDocTemplate Virtual Functions

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiDocTemplateIn)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMultiDocTemplateIn();

	// Generated message map functions
	//{{AFX_MSG(CMultiDocTemplateIn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIDOCTEMPLATEIN_H__BAD9C513_B493_4402_9102_6D640592B8A9__INCLUDED_)
