// EqnChecker.cpp: implementation of the CEqnChecker2 class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "EqnChecker2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
#include "phreeqc/src/output.h"
#include "phreeqc/src/phrqproto.h"
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEqnChecker2::CEqnChecker2()
// COMMENT: {10/20/2004 8:08:56 PM}: m_pPhreeqcCallback(NULL)
{
// COMMENT: {10/20/2004 8:08:37 PM}	ASSERT(sizeof(DWORD) >= sizeof(this));
// COMMENT: {10/20/2004 8:08:37 PM}	GUICALLBACK gui = {(DWORD)this, NULL/*LineCallBack*/, ErrorCallBack, NULL/*WarnCallBack*/};
// COMMENT: {10/20/2004 8:08:37 PM}
// COMMENT: {10/20/2004 8:08:37 PM}	m_pPhreeqcCallback = new CPhreeqcCallback(gui);

	::add_output_callback(WriteCallBack, this);	
	::initialize();

// COMMENT: {10/20/2004 8:08:39 PM}	m_pPhreeqcCallback->Go(set_callback_gui);

}

CEqnChecker2::~CEqnChecker2()
{
	::clean_up();
// COMMENT: {10/20/2004 8:09:05 PM}	if (m_pPhreeqcCallback) delete m_pPhreeqcCallback;
}

bool CEqnChecker2::CheckRxn(LPCTSTR pstr, bool bAssociation, bool bCheck)
{
	m_strErrors = _T("");
	m_nErrors = 0;

	char *buffer = new char[strlen(pstr) + 4];
	strcpy(buffer, pstr);

	try
	{
		struct elt_list *next_elt;

		struct species *s_ptr = NULL;

		if ( parse_eq(buffer, &next_elt, bAssociation ) == ERROR)
		{
			throw ERROR;
		}

		//
		// Get pointer to each species in the reaction, store new species if necessary
		//
		trxn.token[0].s = s_store( trxn.token[0].name, trxn.token[0].z, TRUE);
		for (int i=1; i<count_trxn; ++i)
		{
			trxn.token[i].s = s_store( trxn.token[i].name, trxn.token[i].z, FALSE);
		}

		//
		// Save element list and carbon, hydrogen, and oxygen in species
		//
		trxn.token[0].s->next_elt=next_elt;
		trxn.token[0].s->next_secondary = NULL; 
		for ( ; next_elt->elt != NULL; ++next_elt)
		{
			if ( strcmp (next_elt->elt->name,"C") == 0 )
			{
				trxn.token[0].s->carbon = next_elt->coef;
			}
			if ( strcmp (next_elt->elt->name,"H") == 0 )
			{
				trxn.token[0].s->h = next_elt->coef;
			}
			if ( strcmp (next_elt->elt->name,"O") == 0 )
			{
				trxn.token[0].s->o = next_elt->coef;
			}
		}

		//
		// Malloc space for species reaction
		//
		trxn.token[0].s->rxn = rxn_alloc (count_trxn+1);
		//
		// Copy reaction to reaction for species
		//
		trxn_copy(trxn.token[0].s->rxn);
		s_ptr=trxn.token[0].s;

		if (bCheck && check_eqn(TRUE) == ERROR)
		{
			throw ERROR;
		}
	}
	catch (int)
	{
		ASSERT(m_nErrors != 0);
		TRACE("Error caught in CEqnChecker2::CheckAssociationRxn()\n");
	}

	delete[] buffer;

	return (m_nErrors == 0);
}

bool CEqnChecker2::CheckAssociationRxn(LPCTSTR pstr, bool bCheck)
{
	return CheckRxn(pstr, true, bCheck);
}

bool CEqnChecker2::CheckDissociationRxn(LPCTSTR pstr, bool bCheck)
{
	return CheckRxn(pstr, false, bCheck);
}

CString CEqnChecker2::GetLastError()
{
	return m_strErrors;
}

// COMMENT: {10/20/2004 8:09:18 PM}// static
// COMMENT: {10/20/2004 8:09:18 PM}void CALLBACK CEqnChecker2::ErrorCallBack(DWORD dwCookie, LPCSTR lpcstr)
// COMMENT: {10/20/2004 8:09:18 PM}{
// COMMENT: {10/20/2004 8:09:18 PM}	ASSERT(dwCookie);
// COMMENT: {10/20/2004 8:09:18 PM}	CEqnChecker2* pThis = (CEqnChecker2*) dwCookie;
// COMMENT: {10/20/2004 8:09:18 PM}
// COMMENT: {10/20/2004 8:09:18 PM}	++(pThis->m_nErrors);
// COMMENT: {10/20/2004 8:09:18 PM}	pThis->m_strErrors += lpcstr;
// COMMENT: {10/20/2004 8:09:18 PM}	pThis->m_strErrors += _T("\n");
// COMMENT: {10/20/2004 8:09:18 PM}}
int CEqnChecker2::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
	UNREFERENCED_PARAMETER(cookie);
	UNREFERENCED_PARAMETER(format);
	UNREFERENCED_PARAMETER(args);

	static CString str;

	if (action == ACTION_OUTPUT)
	{

		ASSERT(cookie);
		CEqnChecker2 *pThis = (CEqnChecker2*)cookie;

		switch (type)
		{
		case OUTPUT_ERROR:
			++(pThis->m_nErrors);
			pThis->m_strErrors += err_str;
			pThis->m_strErrors += _T("\n");
			break;
		}

		if (stop == STOP)
		{
			::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
		}
	}
	return 0;
}
