// DDX_DDV.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "DDX_DDV.h"

#include "mshflexgrid.h"
#include "EditGrid.h"

UINT BasicCheckProc(LPVOID pParam);
struct BasicCheckProcStruct
{
	struct rate* rate_ptr;
	HANDLE hInfiniteLoop;
};
enum
{
	BCP_OK,
	BCP_ERRORS,
};

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
	//{{
	int basic_renumber_1(char *commands, void **lnbase, void **vbase, void **lpbase);

	int P_escapecode;
	UINT g_nIDErrPrompt;
	int g_nErrLineNumber;

	int basic_compile_1(char *commands, void **lnbase, void **vbase, void **lpbase, int parse_whole_program_flag);

	int basic_run_1(char *commands, void *lnbase, void *vbase, void *lpbase, int parse_whole_program_flag, HANDLE hInfiniteLoop);
	void basic_free(void *lnbase, void *vbase, void *lpbase);

	void cmd_initialize_1(void);
	void cmd_free_1(void);

	void initialize(void);
	int clean_up(void);
	//}}
}

// prepares editgrid control for data transfer
// this is required since the editgrid doesn't recieve a WM_KILLFOCUS notification
// before DoDataExchange is called
CWnd* PASCAL PrepareEditGridCtrl(CDataExchange* pDX, int nIDC)
{
	CWnd* pWndCtrl = pDX->PrepareOleCtrl(nIDC);
	if (pWndCtrl)
	{
		// force the grid to finish the edit routine if editing or not
		// the fCancel set to FALSE sets the grid to the current value
		// of the edit window
		VERIFY(pWndCtrl->SendMessage(EGM_ENDEDITCELLNOW, (WPARAM)(BOOL)FALSE/*fCancel*/, 0));
	}
	return pWndCtrl;
}


// Taken from DLGDATA.CPP
// only supports '%d', '%u', '%sd', '%su', '%ld' and '%lu'
AFX_STATIC BOOL AFXAPI _AfxSimpleScanf(LPCTSTR lpszText,
	LPCTSTR lpszFormat, va_list pData)
{
	ASSERT(lpszText != NULL);
	ASSERT(lpszFormat != NULL);

	ASSERT(*lpszFormat == '%');
	lpszFormat++;        // skip '%'

	BOOL bLong = FALSE;
	BOOL bShort = FALSE;
	if (*lpszFormat == 'l')
	{
		bLong = TRUE;
		lpszFormat++;
	}
	else if (*lpszFormat == 's')
	{
		bShort = TRUE;
		lpszFormat++;
	}

	ASSERT(*lpszFormat == 'd' || *lpszFormat == 'u');
	ASSERT(lpszFormat[1] == '\0');

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	TCHAR chFirst = lpszText[0];
	long l, l2;
	if (*lpszFormat == 'd')
	{
		// signed
		l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (int)l;
	}
	else
	{
		// unsigned
		if (*lpszText == '-')
			return FALSE;
		l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (unsigned int)l;
	}
	if (l == 0 && chFirst != '0')
		return FALSE;   // could not convert

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	if (bShort)
	{
		if ((short)l != l)
			return FALSE;   // too big for short
		*va_arg(pData, short*) = (short)l;
	}
	else
	{
		ASSERT(sizeof(long) == sizeof(int));
		ASSERT(l == l2);
		*va_arg(pData, long*) = l;
	}

	// all ok
	return TRUE;
}

//
// Modified from DLGDATA.CPP
//
AFX_STATIC void AFX_CDECL _Afx_DDX_TextWithFormat(CDataExchange* pDX, int nIDC,
	long nRow, long nCol,
	LPCTSTR lpszFormat, UINT nIDPrompt, ...)
	// only supports windows output formats - no floating point
{
	va_list pData;
	va_start(pData, nIDPrompt);

	CWnd* pWnd = PrepareEditGridCtrl(pDX, nIDC);

	// validate grid cell
	ASSERT_KINDOF(CMSHFlexGrid, pWnd);	// only valid for grid control
	ASSERT(nRow < ((CMSHFlexGrid*)pWnd)->GetRows());
	ASSERT(nRow >= ((CMSHFlexGrid*)pWnd)->GetFixedRows());
	ASSERT(nCol < ((CMSHFlexGrid*)pWnd)->GetCols(0));
	ASSERT(nCol >= ((CMSHFlexGrid*)pWnd)->GetFixedCols());

	static BYTE parms[] = VTS_I4;

	// void CMSHFlexGrid::SetRow(long nNewValue)
	pWnd->InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nRow);

	// void CMSHFlexGrid::SetCol(long nNewValue)
	pWnd->InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nCol);

	TCHAR szT[32];
	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		// CString CMSHFlexGrid::GetTextMatrix(long Row, long Col)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
			nRow, nCol);

		if (!_AfxSimpleScanf(result, lpszFormat, pData))
		{
			DDX_GridFail(pDX, nIDPrompt);
		}
	}
	else
	{
		wvsprintf(szT, lpszFormat, pData);
			// does not support floating point numbers - see dlgfloat.cpp
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR;
		// void CMSHFlexGrid::SetTextMatrix(long Row, long Col, LPCTSTR lpszNewValue)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
			 nRow, nCol, szT);
	}

	va_end(pData);
}

//
// Modified from:
// void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, int& value)
// (DLGDATA.CPP)
//
void AFXAPI DDX_GridText(CDataExchange* pDX, int nIDC,
					 long nRow, long nCol,
					 int& value)
{
	if (pDX->m_bSaveAndValidate)
		_Afx_DDX_TextWithFormat(pDX, nIDC, nRow, nCol, _T("%d"), AFX_IDP_PARSE_INT, &value);
	else
		_Afx_DDX_TextWithFormat(pDX, nIDC, nRow, nCol, _T("%d"), AFX_IDP_PARSE_INT, value);
}

void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC,
						 long nRow, long nCol,
						 long& value)
{
	if (pDX->m_bSaveAndValidate)
		_Afx_DDX_TextWithFormat(pDX, nIDC, nRow, nCol, _T("%ld"), AFX_IDP_PARSE_INT, &value);
	else
		_Afx_DDX_TextWithFormat(pDX, nIDC, nRow, nCol, _T("%ld"), AFX_IDP_PARSE_INT, value);
}


//
// Modified from:
// void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, CString& value)
// (DLGDATA.CPP)
//
void AFXAPI DDX_GridText(CDataExchange* pDX, int nIDC,
					 long nRow, long nCol,
					 CString& value)
{
	CWnd* pWnd = PrepareEditGridCtrl(pDX, nIDC);

	// validate grid cell
	ASSERT_KINDOF(CMSHFlexGrid, pWnd);	// only valid for grid control
	ASSERT(nRow < ((CMSHFlexGrid*)pWnd)->GetRows());
	ASSERT(nRow >= ((CMSHFlexGrid*)pWnd)->GetFixedRows());
	ASSERT(nCol < ((CMSHFlexGrid*)pWnd)->GetCols(0));
	ASSERT(nCol >= ((CMSHFlexGrid*)pWnd)->GetFixedCols());

	if (pDX->m_bSaveAndValidate)
	{
		static BYTE parms[] = VTS_I4 VTS_I4;

		// CString CMSHFlexGrid::GetTextMatrix(long Row, long Col)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&value, parms,
			nRow, nCol);

		static BYTE parm[] = VTS_I4;

		// void CMSHFlexGrid::SetRow(long nNewValue)
		pWnd->InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parm, nRow);

		// void CMSHFlexGrid::SetCol(long nNewValue)
		pWnd->InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parm, nCol);
	}
	else
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR;
		// void CMSHFlexGrid::SetTextMatrix(long Row, long Col, LPCTSTR lpszNewValue)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
			 nRow, nCol, (LPCTSTR)value);
	}
}

void PASCAL DDV_MinEqualInt(CDataExchange* pDX, int value, int minVal, UINT nIDPrompt /*= IDS_INT_MIN_EQUAL_137*/)
{
	if (value < minVal)
	{
		if (!pDX->m_bSaveAndValidate)
		{
			TRACE0(_T("Warning: initial dialog data is out of range.\n"));
			return;     // don't stop now
		}
		TCHAR szMin[32];
		wsprintf(szMin, _T("%ld"), minVal);
		CString prompt;
		AfxFormatString1(prompt, nIDPrompt, szMin);
		DDX_GridFail(pDX, prompt);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Extra data validation procs for float/double support
/////////////////////////////////////////////////////////////////////////////

AFX_STATIC BOOL AFXAPI _AfxSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == _T(' ') || *lpszText == _T('\t'))
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != _T('0'))
		return FALSE;   // could not convert
	while (*lpszText == _T(' ') || *lpszText == _T('\t'))
		lpszText++;

	if (*lpszText != _T('\0'))
		return FALSE;   // not terminated properly

	return TRUE;
}

void AFXAPI AfxTextFloatFormat(CDataExchange* pDX, int nIDC,
							   long nRow, long nCol,
							   void* pData, double value, int nSizeGcvt,
							   UINT nIDPrompt)
{
	ASSERT(pData != NULL);

	CWnd* pWnd = PrepareEditGridCtrl(pDX, nIDC);
	TCHAR szBuffer[32];
	static BYTE parms[] = VTS_I4;

	// validate grid cell
	ASSERT_KINDOF(CMSHFlexGrid, pWnd);	// only valid for grid control
	ASSERT(nRow < ((CMSHFlexGrid*)pWnd)->GetRows());
	ASSERT(nRow >= ((CMSHFlexGrid*)pWnd)->GetFixedRows());
	ASSERT(nCol < ((CMSHFlexGrid*)pWnd)->GetCols(0));
	ASSERT(nCol >= ((CMSHFlexGrid*)pWnd)->GetFixedCols());

	// void CMSHFlexGrid::SetRow(long nNewValue)
	pWnd->InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nRow);

	// void CMSHFlexGrid::SetCol(long nNewValue)
	pWnd->InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nCol);

	if (pDX->m_bSaveAndValidate)
	{
		// ::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		// CString CMSHFlexGrid::GetTextMatrix(long Row, long Col)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
			nRow, nCol);

		double d;
		if (!_AfxSimpleFloatParse(result, d))
		{
			DDX_GridFail(pDX, nIDPrompt);	// throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR;
		// void CMSHFlexGrid::SetTextMatrix(long Row, long Col, LPCTSTR lpszNewValue)
		pWnd->InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
			 nRow, nCol, szBuffer);
	}
}

void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC,
					 long nRow, long nCol,
					 float& value, UINT nIDPrompt /* = AFX_IDP_PARSE_REAL */)
{
	AfxTextFloatFormat(pDX, nIDC, nRow, nCol, &value, value, FLT_DIG, nIDPrompt);
}

void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC,
					 long nRow, long nCol,
					 double& value, UINT nIDPrompt /* = AFX_IDP_PARSE_REAL */)
{
	AfxTextFloatFormat(pDX, nIDC, nRow, nCol, &value, value, DBL_DIG, nIDPrompt);
}

void PASCAL DDX_GridText(CDataExchange* pDX, int nIDC,
					 long nRow, long nCol,
					 long double& value, UINT nIDPrompt /* = AFX_IDP_PARSE_REAL */)
{
	ASSERT(DBL_DIG == LDBL_DIG);
	ASSERT(sizeof(double) == sizeof(long double));
	double dValue = (double) value;
	AfxTextFloatFormat(pDX, nIDC, nRow, nCol, &dValue, dValue, LDBL_DIG, nIDPrompt);
	value = (long double) dValue;
}

//
// Dialog Data Exchange Routine for MSHFlexGrid
//
// if (m_bSaveAndValidate == TRUE)
//     If cell defined by nRow and nCol is empty the value is set to the
//     default value of dDefault
//
// if (m_bSaveAndValidate == FALSE)
//     The cell defined by nRow and nCol is set to the value
//
void PASCAL DDX_GridTextDefault(CDataExchange* pDX, int nIDC,
								long nRow, long nCol,
								double& value, const double dDefault)
{
	if (pDX->m_bSaveAndValidate)
	{
		CString str;
		DDX_GridText(pDX, nIDC, nRow, nCol, str);
		if (str.IsEmpty())
		{
			value = dDefault;
		}
		else
		{
			DDX_GridText(pDX, nIDC, nRow, nCol, value);
		}
	}
	else
	{
		DDX_GridText(pDX, nIDC, nRow, nCol, value);
	}
}

//
// Dialog Data Exchange Routine for MSHFlexGrid
//
// if (m_bSaveAndValidate == TRUE)
//     if cell defined by nRow and nCol is empty value is to
//     std::numeric_limits<double>::signaling_NaN()
//
// if (m_bSaveAndValidate == FALSE)
//     if value == std::numeric_limits<double>::signaling_NaN()
//     the cell defined by nRow and nCol is set to empty
//
void PASCAL DDX_GridTextNaN(CDataExchange* pDX, int nIDC,
							long nRow, long nCol,
							double& value)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
	CString str(_T(""));
	if (pDX->m_bSaveAndValidate)
	{
		DDX_GridText(pDX, nIDC, nRow, nCol, str);
		if (str.IsEmpty())
		{
			value = std::numeric_limits<double>::signaling_NaN();
		}
		else
		{
			DDX_GridText(pDX, nIDC, nRow, nCol, value);
		}
	}
	else
	{
		if (value == std::numeric_limits<double>::signaling_NaN())
		{
			DDX_GridText(pDX, nIDC, nRow, nCol, str);
		}
		else
		{
			DDX_GridText(pDX, nIDC, nRow, nCol, value);
		}
	}
}

//
// Display MessageBox containing string table nIDText and string table nIDCaption
// and highlight the bad cell
//
void PASCAL DDX_GridFail(CDataExchange* pDX, UINT nIDText, UINT nIDCaption /* = IDR_MAINFRAME*/)
{
	if (!pDX->m_bSaveAndValidate)
	{
		TRACE0("Warning: CDataExchange::Fail called when not validating.\n");
		// throw the exception anyway
	}
	else if (pDX->m_hWndLastControl != NULL)
	{
		CMSHFlexGrid* pGrid = (CMSHFlexGrid*)CWnd::FromHandle(pDX->m_hWndLastControl);
		ASSERT_KINDOF(CMSHFlexGrid, pGrid);	// last control not CMSHFlexGrid

		// This causes the current cell to be visible
		pGrid->GetCellTop();

		// let user see which cell contains error while msgbox is modal
		ASSERT(pGrid->GetHighLight() == flexHighlightWithFocus);
		pGrid->SetHighLight(flexHighlightAlways);
		pGrid->SetRedraw(TRUE);

		// display error msg
		CString strText;
		CString strCaption;
		VERIFY(strText.LoadString(nIDText));
		VERIFY(strCaption.LoadString(nIDCaption));
		MessageBox(pGrid->m_hWnd, strText, strCaption, MB_ICONEXCLAMATION);

		// reset grid
		pGrid->SetHighLight(flexHighlightWithFocus);
		pDX->Fail(); // throws exception
	}
	else
	{
		TRACE0("Error: fail validation with no control to restore focus to.\n");
		// do nothing more
	}
}

//
// Display MessageBox containing lpText and lpCaption
// and highlight the bad cell
//
void PASCAL DDX_GridFail(CDataExchange* pDX, LPCTSTR lpText, LPCTSTR lpCaption /* = NULL*/)
{
	if (!pDX->m_bSaveAndValidate)
	{
		TRACE0("Warning: CDataExchange::Fail called when not validating.\n");
		// throw the exception anyway
	}
	else if (pDX->m_hWndLastControl != NULL)
	{
		CMSHFlexGrid* pGrid = (CMSHFlexGrid*)CWnd::FromHandle(pDX->m_hWndLastControl);
		ASSERT_KINDOF(CMSHFlexGrid, pGrid);	// last control not CMSHFlexGrid

		// This causes the current cell to be visible
		pGrid->GetCellTop();

		// let user see which cell contains error while msgbox is modal
		ASSERT(pGrid->GetHighLight() == flexHighlightWithFocus);
		pGrid->SetHighLight(flexHighlightAlways);
		pGrid->SetRedraw(TRUE);

		// display error msg
		CString strCaption;
		if (lpCaption == NULL)
		{
			VERIFY(strCaption.LoadString(IDR_MAINFRAME));
			lpCaption = strCaption;
		}
		MessageBox(pGrid->m_hWnd, lpText, lpCaption, MB_ICONEXCLAMATION);

		/// reset grid
		pGrid->SetHighLight(flexHighlightWithFocus);
		pDX->Fail(); // throws exception
	}
	else
	{
		TRACE0("Error: fail validation with no control to restore focus to.\n");
		// do nothing more
	}
}

