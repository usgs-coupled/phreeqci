// BasicDesc.h: interface for the CBasicDesc class.
//
// $Date: 2000/10/04 20:17:29 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICDESC_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_)
#define AFX_BASICDESC_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClipListBox.h"
class CDatabase;

class CBasicDesc  
{
public:
	CBasicDesc(const CDatabase& m_rDatabase, int nIDFuncs, int nIDExplan, int nIDArgs);
	virtual ~CBasicDesc();

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
	CClipListBox m_lbArgs;
	CEdit m_eExplan;

	// map loaded flag
	bool m_bMapLoaded;

	// map used to hold functions and their descriptions
	std::map<CString, CString> m_mapFuncs;


};

#endif // !defined(AFX_BASICDESC_H__E15C5EFB_E98B_4E1D_8511_75F83A9A067F__INCLUDED_)
