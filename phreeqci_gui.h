// phreeqci_gui.h :
//
// $Date: 2000/10/04 20:18:05 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHREEQCI_GUI_H__A62CB467_D42F_4F92_A796_A026F88518B3__INCLUDED_)
#define AFX_PHREEQCI_GUI_H__A62CB467_D42F_4F92_A796_A026F88518B3__INCLUDED_

#include <windows.h>
#include <crtdbg.h>

struct h_status {
	HWND hText;
	HWND hSim;
	HWND hTrans;
};

extern struct h_status g_status;
extern char g_szLineBuf[160];
extern HANDLE g_hKill;
extern BOOL g_bKill;
extern int linenr;

// returns	TRUE if the line was copied
//			FALSE if a larger buffer is required to copy the entire line
//				pcb returns the buffer size required to copy the entire line
typedef BOOL (CALLBACK *LINECALLBACK)(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

typedef void (CALLBACK *ERRORCALLBACK)(DWORD dwCookie, LPCSTR lpcstr);

typedef void (CALLBACK *WARNCALLBACK)(DWORD dwCookie, LPCSTR lpcstr);

typedef struct tagGUICALLBACK
{
	DWORD dwCookie;		/* user value passed to callback as first parameter */
	LINECALLBACK pfnLine;
	ERRORCALLBACK pfnError;
	WARNCALLBACK pfnWarn;
} GUICALLBACK;

typedef void (*LPFNPHREEQC)(GUICALLBACK gui);
typedef void (*LPFNPHREEQC2)(GUICALLBACK gui, int nValue);


// see winerror.h
#define MAKESOFTWAREEXCEPTION(Severity, Facility, Exception) \
	((DWORD) ( \
	/* Severity code */      (Severity       ) |   \
	/* MS(0) or Cust(1) */   (1         << 29) |   \
	/* Reserved(0) */        (0         << 28) |   \
	/* Facility code */      (Facility  << 16) |   \
	/* Exception code */     (Exception <<  0)))

#define USER_CANCELED_RUN MAKESOFTWAREEXCEPTION(ERROR_SEVERITY_ERROR, FACILITY_NULL, 1)

#define INPUT_CONTAINS_ERRORS MAKESOFTWAREEXCEPTION(ERROR_SEVERITY_ERROR, FACILITY_NULL, 2)

#endif // !defined(AFX_PHREEQCI_GUI_H__A62CB467_D42F_4F92_A796_A026F88518B3__INCLUDED_)

