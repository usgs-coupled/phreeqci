// Include.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "Include.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInclude

IMPLEMENT_DYNAMIC(CInclude, baseInclude)

CInclude::CInclude(CWnd* pWndParent)
	 : baseInclude(IDS_PROPSHT_CAPTION_123, pWndParent)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CInclude::~CInclude()
{
}


BEGIN_MESSAGE_MAP(CInclude, baseInclude)
	//{{AFX_MSG_MAP(CInclude)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInclude message handlers

CString CInclude::GetString()
{
	// INCLUDE$ filename
	CString strLines = CKeyword::GetString(CKeyword::K_INCLUDE) + " " + this->m_Page1.m_strFileName;
	return strLines + s_strNewLine;
}

void CInclude::Edit(CString& rStr)
{
	try
	{
		std::string line(rStr);
		std::string token;
		std::string::iterator beg = line.begin();
		std::string::iterator end = line.end();
		CParser::copy_token(token, beg, end);
		std::transform(token.begin(), token.end(), token.begin(), ::tolower);

		ASSERT((strstr(token.c_str(),"include$") == token.c_str()) || (strstr(token.c_str(),"include_file") == token.c_str()));

		std::string file_name;
		file_name.assign(beg, end);
		file_name = trim(file_name);

		this->m_Page1.m_strFileName = file_name.c_str();		
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}
}
