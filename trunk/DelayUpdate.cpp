// DelayUpdate.cpp: implementation of the CDelayUpdate class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "DelayUpdate.h"

#include "mshflexgrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelayUpdate::CDelayUpdate(CMSHFlexGrid* pGrid) : m_pGrid(pGrid)
{
	ASSERT(m_pGrid);
	m_bRedraw = m_pGrid->GetRedraw();
	pGrid->SetRedraw(FALSE);
}

CDelayUpdate::CDelayUpdate(CWnd* pDlgWnd, int nIDC) : m_pGrid(NULL)
{
	ASSERT(nIDC != 0);
	ASSERT(nIDC != -1); // not allowed
	m_pGrid = (CMSHFlexGrid*)pDlgWnd->GetDlgItem(nIDC);
	ASSERT(m_pGrid);
	ASSERT_KINDOF(CMSHFlexGrid, m_pGrid);

	m_bRedraw = m_pGrid->GetRedraw();
	m_pGrid->SetRedraw(FALSE);
}

CDelayUpdate::~CDelayUpdate()
{
	if (m_bRedraw)
		m_pGrid->SetRedraw(TRUE);
}

