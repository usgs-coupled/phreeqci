// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// COMMENT: {10/28/2009 6:59:57 PM}#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// COMMENT: {10/28/2009 6:59:57 PM}// Windows Header Files:
// COMMENT: {10/28/2009 6:59:57 PM}#include <windows.h>
// COMMENT: {10/28/2009 6:59:57 PM}#include <strsafe.h>
// COMMENT: {10/28/2009 6:59:57 PM}#include <msiquery.h>
// COMMENT: {10/28/2009 6:59:57 PM}
// COMMENT: {10/28/2009 6:59:57 PM}// WiX Header Files:
// COMMENT: {10/28/2009 6:59:57 PM}#include <wcautil.h>


// TODO: reference additional headers your program requires here
#if _WIN32_MSI < 150
#define _WIN32_MSI 150
#endif

#include <windows.h>
#include <msiquery.h>
#include <msidefs.h>
#include <shlobj.h>
#include <richedit.h>
#include <msxml2.h>
#include <shobjidl.h>
#include <intshcut.h>


//{{ changed " to <
#include "C:\Documents and Settings\All Users\Documents\Downloads\sourceforge.net\wix3.0.5419.0-sources\wix3.0.5419.0-sources\inc\wixstrsafe.h"
#include <wcautil.h>
#include <wcawow64.h>
#include <aclutil.h>
#include <dirutil.h>
#if _MSC_VER > 1400

#include <fileutil.h> // FileRead FileExistsEx

#else

// HACK to declare fileutil.h
// since fileutil.h contains unsupported __out_range
#ifdef __cplusplus
extern "C" {
#endif

#define ReleaseFile(h) if (INVALID_HANDLE_VALUE != h) { ::CloseHandle(h); h = INVALID_HANDLE_VALUE; }

BOOL DAPI FileExistsEx(
    __in LPCWSTR wzPath, 
    __out_opt DWORD *pdwAttributes
    );
HRESULT DAPI FileRead(
    __deref_out_bcount_full(*pcbDest) LPBYTE* ppbDest,
    __out DWORD* pcbDest,
    __in LPCWSTR wzSrcPath
    );
HRESULT DAPI FileGetTime(
    __in LPCWSTR wzFile,  
    __out_opt  LPFILETIME lpCreationTime,
    __out_opt  LPFILETIME lpLastAccessTime,
    __out_opt  LPFILETIME lpLastWriteTime
    );
HRESULT DAPI FileSetTime(
    __in LPCWSTR wzFile,  
    __in_opt  const FILETIME *lpCreationTime,
    __in_opt  const FILETIME *lpLastAccessTime,
    __in_opt  const FILETIME *lpLastWriteTime
    );

#ifdef __cplusplus
}
#endif

#endif
//}}
#include <memutil.h>
#include <pathutil.h>
#include <procutil.h>
#include <strutil.h>
#include <xmlutil.h>
#include <wiutil.h>

#include <C:\Documents and Settings\All Users\Documents\Downloads\sourceforge.net\wix3.0.5419.0-sources\wix3.0.5419.0-sources\src\ca\wcautil\custommsierrors.h>
#include <C:\Documents and Settings\All Users\Documents\Downloads\sourceforge.net\wix3.0.5419.0-sources\wix3.0.5419.0-sources\src\ext\UIExtension\ca\cost.h>
