#pragma once

class CSaveCurrentDirectory
{
public:
	CSaveCurrentDirectory(void);
	CSaveCurrentDirectory(LPCTSTR lpszNewPath);
	~CSaveCurrentDirectory(void);

	BOOL SetCurrentDirectory(LPCTSTR lpPathName);
protected:
	TCHAR *path;
};
