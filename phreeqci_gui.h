// phreeqci_gui.h :
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHREEQCI_GUI_H__A62CB467_D42F_4F92_A796_A026F88518B3__INCLUDED_)
#define AFX_PHREEQCI_GUI_H__A62CB467_D42F_4F92_A796_A026F88518B3__INCLUDED_

#include <windows.h>

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

