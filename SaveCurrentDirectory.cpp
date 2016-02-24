#include "StdAfx.h"
#include "SaveCurrentDirectory.h"

CSaveCurrentDirectory::CSaveCurrentDirectory(void)
: path(0)
{
	DWORD len = ::GetCurrentDirectory(0, NULL);
	if (len)
	{
		this->path = new TCHAR[len];
		if (::GetCurrentDirectory(len, this->path) == 0)
		{
			delete this->path;
			this->path = NULL;
			ASSERT(FALSE);
		}
	}
}

CSaveCurrentDirectory::CSaveCurrentDirectory(LPCTSTR lpszNewPath)
: path(0)
{
	DWORD len = ::GetCurrentDirectory(0, NULL);
	if (len)
	{
		this->path = new TCHAR[len];
		if (::GetCurrentDirectory(len, this->path) == 0)
		{
			delete this->path;
			this->path = NULL;
			ASSERT(FALSE);
		}
	}

	if (lpszNewPath)
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];
		VERIFY(::_tsplitpath_s(lpszNewPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFName, _MAX_FNAME, szExt, _MAX_EXT) == 0);

		TCHAR szNewDir[_MAX_PATH];
		VERIFY(::_tmakepath_s(szNewDir, _MAX_PATH, szDrive, szDir, NULL, NULL) == 0);
		if (::_tcslen(szNewDir))
		{
			VERIFY(this->SetCurrentDirectory(szNewDir));
		}
	}
}

CSaveCurrentDirectory::~CSaveCurrentDirectory(void)
{
	if (this->path)
	{
		BOOL bRet = ::SetCurrentDirectory(this->path);
		if (bRet == 0)
		{
			TRACE("::SetCurrentDirectory(%s) failed (%d)\n", this->path, ::GetLastError());
			ASSERT(FALSE);
		}
	}
	delete this->path;
	this->path = NULL;
}

BOOL CSaveCurrentDirectory::SetCurrentDirectory(LPCTSTR lpPathName)
{
	BOOL bRet = FALSE;
	if (lpPathName)
	{
		CString strPathName(lpPathName);

		DWORD len = ::GetCurrentDirectory(0, NULL);
		TCHAR *current = new TCHAR[len];
		if (current)
		{
			::GetCurrentDirectory(len, current);
			if(::_tcscmp(current, lpPathName) != 0)
			{
				bRet = ::SetCurrentDirectory(lpPathName);
				if (bRet == 0)
				{
					TRACE("::SetCurrentDirectory failed (%d)\n", ::GetLastError());
					ASSERT(FALSE);
				}
			}
			delete current;
		}
		else
		{
			bRet = ::SetCurrentDirectory(lpPathName);
			if (bRet == 0)
			{
				TRACE("::SetCurrentDirectory failed (%d)\n", ::GetLastError());
				ASSERT(FALSE);
			}
		}
	}
	return bRet;
}
