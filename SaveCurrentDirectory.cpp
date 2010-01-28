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
		}
	}

	if (lpszNewPath && ::_tcslen(lpszNewPath))
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];
		::_tsplitpath(lpszNewPath, szDrive, szDir, szFName, szExt);

		TCHAR szNewDir[MAX_PATH];
		::_tmakepath(szNewDir, szDrive, szDir, NULL, NULL);
		this->SetCurrentDirectory(szNewDir);
	}
}

CSaveCurrentDirectory::~CSaveCurrentDirectory(void)
{
	this->SetCurrentDirectory(this->path);
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
			}
			delete current;
		}
		else
		{
			bRet = ::SetCurrentDirectory(lpPathName);
		}
	}
	return bRet;
}
