// UpdateObject.h: interface for the CUpdateObject class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEOBJECT_H__A865136C_423A_48BB_B9A7_25895E212614__INCLUDED_)
#define AFX_UPDATEOBJECT_H__A865136C_423A_48BB_B9A7_25895E212614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUpdateObject : public CObject  
{
public:
	DECLARE_DYNAMIC(CUpdateObject)
	CString m_str;
	CUpdateObject();
	virtual ~CUpdateObject();

};

#endif // !defined(AFX_UPDATEOBJECT_H__A865136C_423A_48BB_B9A7_25895E212614__INCLUDED_)
