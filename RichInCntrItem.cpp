// RichInCntrItem.cpp : implementation of the CRichInCntrItem class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "RichDocIn.h"
#include "RichViewIn.h"
#include "RichInCntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichInCntrItem implementation

IMPLEMENT_SERIAL(CRichInCntrItem, CRichEditCntrItem, 0)

CRichInCntrItem::CRichInCntrItem(REOBJECT* preo, CRichDocIn* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CRichInCntrItem::~CRichInCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CRichInCntrItem diagnostics

#ifdef _DEBUG
void CRichInCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CRichInCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
