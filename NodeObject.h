// NodeObject.h: interface for the CNodeObject class.
//
// $Date: 2000/10/04 20:17:49 $
// $Revision: 1.1.1.28 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEOBJECT_H__C78927DD_6AC9_4493_AB6B_2AB8FA0D55F6__INCLUDED_)
#define AFX_NODEOBJECT_H__C78927DD_6AC9_4493_AB6B_2AB8FA0D55F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodeObject : public CObject  
{
	DECLARE_DYNAMIC(CNodeObject)
public:

	CTreeCtrlNode m_node;
	CNodeObject(const CTreeCtrlNode node);
	virtual ~CNodeObject();


};

#endif // !defined(AFX_NODEOBJECT_H__C78927DD_6AC9_4493_AB6B_2AB8FA0D55F6__INCLUDED_)
