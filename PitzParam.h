#pragma once

#include "phreeqc.h"
#include "global_structures.h"

class CPitzParam
{
public:
	CPitzParam(pitz_param_type type);
	CPitzParam(const struct pitz_param* p_pitz_param);

public:
	CString species[3];
	pitz_param_type type;
	union
	{
		LDBLE b0;
		LDBLE b1;
		LDBLE b2;
		LDBLE c0;
		LDBLE theta;
		LDBLE lamda;
		LDBLE zeta;
		LDBLE psi;
		LDBLE alphas;
		LDBLE mu;
		LDBLE eta;
		LDBLE eps;
		LDBLE eps1;
	} U;

	double a[6];
};
