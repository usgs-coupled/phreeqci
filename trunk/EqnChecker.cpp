// EqnChecker.cpp: implementation of the CEqnChecker class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "EqnChecker.h"

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

CEqnChecker::CEqnChecker()
{
	::initialize();
}

CEqnChecker::~CEqnChecker()
{
	::clean_up();
}

bool CEqnChecker::CheckRxn(LPCTSTR pstr, bool bAssociation, bool bCheck)
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
		TRACE("Error caught in CEqnChecker::CheckAssociationRxn()\n");
	}

	delete[] buffer;

	return (m_nErrors == 0);
}

bool CEqnChecker::CheckAssociationRxn(LPCTSTR pstr, bool bCheck)
{
	return CheckRxn(pstr, true, bCheck);
}

bool CEqnChecker::CheckDissociationRxn(LPCTSTR pstr, bool bCheck)
{
	return CheckRxn(pstr, false, bCheck);
}

CString CEqnChecker::GetLastError()
{
	return m_strErrors;
}

int CEqnChecker::WriteCallBack(const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
	UNREFERENCED_PARAMETER(cookie);
	UNREFERENCED_PARAMETER(format);
	UNREFERENCED_PARAMETER(args);

	static CString str;

	ASSERT(cookie);
	CEqnChecker *pThis = (CEqnChecker*)cookie;

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
	return 0;
}
