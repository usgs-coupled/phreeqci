#include "stdafx.h"
#include "PitzParam.h"

CPitzParam::CPitzParam(pitz_param_type type)
: type(type)
{
	for (int i = 0; i < 6; ++i)
	{
		this->a[i] = std::numeric_limits<double>::signaling_NaN();
	}
}

CPitzParam::CPitzParam(const struct pitz_param* p_pitz_param)
: type(p_pitz_param->type)
{
	if (this->type == TYPE_B0 || this->type == TYPE_B1 || this->type == TYPE_B2  || this->type == TYPE_C0 || this->type == TYPE_ALPHAS)
	{
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		if (first.Find(_T('+')) != -1)
		{
			ASSERT(second.Find(_T('-')) != -1);
			this->species[0] = first;
			this->species[1] = second;
		}
		else
		{
			ASSERT(second.Find(_T('+')) != -1);
			this->species[0] = second;
			this->species[1] = first;
		}
	}
	else if (this->type == TYPE_PSI)
	{
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		CString third(p_pitz_param->species[2]);
		if (first.Find(_T('+')) != -1)
		{
			if (second.Find(_T('+')) != -1)
			{
				// ++-
				ASSERT(third.Find(_T('-')) != -1);
				// no changes nec
				this->species[0] = first;  // +
				this->species[1] = second; // +
				this->species[2] = third;  // -
			}
			else if (third.Find(_T('+')) != -1)
			{
				// ++-
				ASSERT(second.Find(_T('-')) != -1);
				this->species[0] = first;  // +
				this->species[1] = third;  // +
				this->species[2] = second; // -
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			ASSERT(first.Find(_T('-')) != -1);
			if (second.Find(_T('-')) != -1)
			{
				// --+
				ASSERT(third.Find(_T('+')) != -1);
				// no changes nec
				this->species[0] = first;  // -
				this->species[1] = second; // -
				this->species[2] = third;  // +
			}
			else if (third.Find(_T('-')) != -1)
			{
				// --+
				ASSERT(second.Find(_T('+')) != -1);
				this->species[0] = first;  // -
				this->species[1] = third;  // -
				this->species[2] = second; // +
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else if (this->type == TYPE_THETA)
	{
		this->species[0] = p_pitz_param->species[0];
		this->species[1] = p_pitz_param->species[1];
	}
	else if (this->type == TYPE_LAMDA)
	{
		// CN or AN
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		if (first.FindOneOf(_T("+-")) != -1)
		{
			ASSERT(second.FindOneOf(_T("+-")) == -1);
			this->species[0] = first;  // +/-
			this->species[1] = second; // 0
		}
		else
		{
			ASSERT(second.FindOneOf(_T("+-")) != -1);
			this->species[0] = second; // +/-
			this->species[1] = first;  // 0
		}
	}
	else if (this->type == TYPE_ZETA)
	{
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		CString third(p_pitz_param->species[2]);
		if (first.Find(_T('+')) != -1)
		{
			if (second.Find(_T('-')) != -1)
			{
				ASSERT(third.FindOneOf(_T("+-")) == -1);
				this->species[0] = first;  // +
				this->species[1] = second; // -
				this->species[2] = third;  // 0
			}
			else
			{
				ASSERT(second.Find(_T('+')) != -1);
				ASSERT(third.Find(_T('-')) != -1);
				this->species[0] = first;  // +
				this->species[1] = third;  // -
				this->species[2] = second; // 0
			}
		}
		else if (first.Find(_T('-')) != -1)
		{
			if (second.Find(_T('+')) != -1)
			{
				ASSERT(third.FindOneOf(_T("+-")) == -1);
				this->species[0] = second; // +
				this->species[1] = first;  // -
				this->species[2] = third;  // 0
			}
			else
			{
				ASSERT(second.Find(_T('+')) != -1);
				ASSERT(third.Find(_T('+')) != -1);
				this->species[0] = third;  // +
				this->species[1] = first;  // -
				this->species[2] = second; // 0
			}
		}
		else
		{
			if (second.Find(_T('+')) != -1)
			{
				ASSERT(third.Find(_T('-')) != -1);
				this->species[0] = second; // +
				this->species[1] = third;  // -
				this->species[2] = first;  // 0
			}
			else
			{
				// first is neutral
				ASSERT(second.Find(_T('-')) != -1);
				ASSERT(third.Find(_T('+')) != -1);
				this->species[0] = third;  // +
				this->species[1] = second; // -
				this->species[2] = first;  // 0
			}
		}
	}
	else if (this->type == TYPE_MU)
	{
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		CString third(p_pitz_param->species[2]);
		if (first.FindOneOf(_T("+-")) == -1)
		{
			if (second.Find(_T("+-")) == -1)
			{
				ASSERT(third.FindOneOf(_T("+-")) == -1);
				this->species[0] = first;  // 0
				this->species[1] = second; // 0
				this->species[2] = third;  // V
			}
			else
			{
				ASSERT(third.FindOneOf(_T("+-")) == -1);
				this->species[0] = first;  // 0
				this->species[1] = third;  // 0
				this->species[2] = second; // V
			}
		}
		else
		{
			ASSERT(second.Find(_T("+-")) == -1);
			ASSERT(third.Find(_T("+-")) == -1);
			this->species[0] = second;  // 0
			this->species[1] = third;  // 0
			this->species[2] = first; // V
		}
	}
	else if (this->type == TYPE_ETA)
	{
		CString first(p_pitz_param->species[0]);
		CString second(p_pitz_param->species[1]);
		CString third(p_pitz_param->species[2]);
		if (first.FindOneOf(_T("+-")) == -1)
		{
			this->species[0] = first;  // 0
			this->species[1] = second; // V
			this->species[2] = third;  // V
		}
		else if (second.Find(_T("+-")) == -1)
		{
			this->species[0] = second; // 0
			this->species[1] = first;  // V
			this->species[2] = third;  // V
		}
		else if (third.Find(_T("+-")) == -1)
		{
			this->species[0] = third;  // 0
			this->species[1] = first;  // V
			this->species[2] = second; // V
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	for (size_t i = 0; i < 6; ++i)
	{
		this->a[i] = p_pitz_param->a[i];
	}
}