// RichOutCntrItem.cpp : implementation of the CRichOutCntrItem class
//
// $Date: 2000/10/04 20:17:54 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "RichDocOut.h"
#include "RichViewOut.h"
#include "RichOutCntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichOutCntrItem implementation

IMPLEMENT_SERIAL(CRichOutCntrItem, CRichEditCntrItem, 0)

CRichOutCntrItem::CRichOutCntrItem(REOBJECT* preo, CRichDocOut* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CRichOutCntrItem::~CRichOutCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CRichOutCntrItem diagnostics

#ifdef _DEBUG
void CRichOutCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CRichOutCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
