// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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

#pragma warning(push)
#pragma warning(disable: 6001 6011 6029 6031 6053 6054 6057 6059 6063 6064 6066 6067 6200 6201 6202 6203 6204 6209 6211 6214 6215 6216 6217 6219 6220 6221 6225 6226 6230 6235 6236 6237 6239 6240 6242 6244 6246 6248 6250 6255 6258 6259 6260 6262 6263 6268 6269 6270 6271 6272 6273 6274 6276 6277 6278 6279 6280 6281 6282 6283 6284 6285 6286 6287 6288 6289 6290 6291 6292 6293 6294 6295 6296 6297 6298 6299 6302 6303 6305 6306 6308 6309 6310 6312 6313 6314 6315 6316 6317 6318 6319 6320 6322 6323 6324 6326 6327 6328 6329 6331 6332 6333 6334 6335 6336 6381 6383 6384 6385 6386 6387 6388 6400 6401 6500 6501 6503 6504 6505 6506 6507 6508 6509 6510 6511 6512 6513 6514 6515 6516 6517 6518 6521 6522 6523 6525 6526 6530 6535 6540 6990 6991 6992 6993 6994 6995)
#include <strsafe.h>
#pragma warning(pop)

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

// these come from the wix3.0.5419.0 sources
// changed to wix36-sources
#include <src/ca/wcautil/custommsierrors.h>
#include <src/ext/UIExtension/ca/cost.h>
