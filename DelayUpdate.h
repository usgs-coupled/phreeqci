// DelayUpdate.h: interface for the CDelayUpdate class.
//
// $Date: 2000/10/04 20:17:40 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYUPDATE_H__A1234274_9D40_11D3_BB49_02608C8F4400__INCLUDED_)
#define AFX_DELAYUPDATE_H__A1234274_9D40_11D3_BB49_02608C8F4400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMSHFlexGrid;

class CDelayUpdate  
{
public:
	CDelayUpdate(CMSHFlexGrid* pGrid);
	CDelayUpdate(CWnd* pDlgWnd, int nIDC);
	virtual ~CDelayUpdate();

private:
	CMSHFlexGrid* m_pGrid;
protected:
	BOOL m_bRedraw;
};

#endif // !defined(AFX_DELAYUPDATE_H__A1234274_9D40_11D3_BB49_02608C8F4400__INCLUDED_)
