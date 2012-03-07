
#include "stdafx.h"


// COMMENT: {10/28/2009 6:51:23 PM}UINT __stdcall CustomAction1(MSIHANDLE hInstall)
// COMMENT: {10/28/2009 6:51:23 PM}{
// COMMENT: {10/28/2009 6:51:23 PM}	HRESULT hr = S_OK;
// COMMENT: {10/28/2009 6:51:23 PM}	UINT er = ERROR_SUCCESS;
// COMMENT: {10/28/2009 6:51:23 PM}
// COMMENT: {10/28/2009 6:51:23 PM}	hr = WcaInitialize(hInstall, "CustomAction1");
// COMMENT: {10/28/2009 6:51:23 PM}	ExitOnFailure(hr, "Failed to initialize");
// COMMENT: {10/28/2009 6:51:23 PM}
// COMMENT: {10/28/2009 6:51:23 PM}	WcaLog(LOGMSG_STANDARD, "Initialized.");
// COMMENT: {10/28/2009 6:51:23 PM}
// COMMENT: {10/28/2009 6:51:23 PM}	// TODO: Add your custom action code here.
// COMMENT: {10/28/2009 6:51:23 PM}
// COMMENT: {10/28/2009 6:51:23 PM}
// COMMENT: {10/28/2009 6:51:23 PM}LExit:
// COMMENT: {10/28/2009 6:51:23 PM}	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
// COMMENT: {10/28/2009 6:51:23 PM}	return WcaFinalize(er);
// COMMENT: {10/28/2009 6:51:23 PM}}


// DllMain - Initialize and cleanup WiX custom action utils.
extern "C" BOOL WINAPI DllMain(
	__in HINSTANCE hInst,
	__in ULONG ulReason,
	__in LPVOID
	)
{
	switch(ulReason)
	{
	case DLL_PROCESS_ATTACH:
		WcaGlobalInitialize(hInst);
		break;

	case DLL_PROCESS_DETACH:
		WcaGlobalFinalize();
		break;
	}

	return TRUE;
}
