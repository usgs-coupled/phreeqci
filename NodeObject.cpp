// NodeObject.cpp: implementation of the CNodeObject class.
//
// $Date: 2000/10/04 20:17:49 $
// $Revision: 1.1.1.28 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "NodeObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CNodeObject, CObject)

CNodeObject::CNodeObject(const CTreeCtrlNode node)
: m_node(node)
{

}

CNodeObject::~CNodeObject()
{

}
