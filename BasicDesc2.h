// BasicDesc2.h: interface for the CBasicDesc2 class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICDESC2_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_)
#define AFX_BASICDESC2_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClipListBox.h"
#include "ClipTreeCtrl.h"
class CDatabase;

class CBasicDesc2  
{
public:
	CBasicDesc2(const CDatabase& m_rDatabase, int nIDFuncs, int nIDExplan, int nIDArgs, bool bUserGraph = false);
	virtual ~CBasicDesc2();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	// message map functions
	void OnSelchangeLbFuncs();

protected:
	void LoadMap();
	void FillFuncs();


	// Database used to fill m_lbArgs
	const CDatabase& m_rDatabase;

	// Control IDs
	int m_nIDFuncs;
	int m_nIDExplan;
	int m_nIDArgs;

	// Controls
	CClipListBox m_lbFuncs;
	CClipTreeCtrl m_treeArgs;
	CEdit m_eExplan;

	// map loaded flag
	bool m_bMapLoaded;

	// USER_GRAPH
	bool m_bUserGraph;

	// previous seletected string in m_lbFuncs
	CString m_strPrev;


	// map used to hold functions and their descriptions
	std::map<CString, CString> m_mapFuncs;


};

#endif // !defined(AFX_BASICDESC2_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_)
