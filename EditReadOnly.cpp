// EditReadOnly.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "EditReadOnly.h"


// CEditReadOnly

IMPLEMENT_DYNAMIC(CEditReadOnly, CEdit)

CEditReadOnly::CEditReadOnly()
{

}

CEditReadOnly::~CEditReadOnly()
{
}


BEGIN_MESSAGE_MAP(CEditReadOnly, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CEditReadOnly message handlers

HBRUSH CEditReadOnly::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// Return a non-NULL brush if the parent's handler should not be called
	return GetSysColorBrush(COLOR_WINDOW); 
}
