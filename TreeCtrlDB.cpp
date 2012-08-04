// TreeCtrlDB.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlDB.h"

#include "TreeCtrlIn.h"

// CTreeCtrlDB

IMPLEMENT_DYNAMIC(CTreeCtrlDB, baseCTreeCtrlDB)

CTreeCtrlDB::CTreeCtrlDB()
{

}

CTreeCtrlDB::~CTreeCtrlDB()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlDB, baseCTreeCtrlDB)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CTreeCtrlDB message handlers

int CTreeCtrlDB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseCTreeCtrlDB::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the Image List
	if (!m_ctrlImage.Create(IDB_INPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}
	// set overlay images
	VERIFY(m_ctrlImage.SetOverlayImage(UnSyncOverlayImage, UnSyncState));
	VERIFY(m_ctrlImage.SetOverlayImage(ErrorOverlayImage, ErrorState));

	// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);

	this->InsertItem(_T("Database files"), CTreeCtrlKeyword::workImageDB, CTreeCtrlKeyword::workImageDB);

	return 0;
}

void CTreeCtrlDB::RemoveDatabaseKeyword(CRichEditDoc*)
{
	/* no-op */
}

//
// returns the file node associated with pDoc
// if pDoc is not associated with any file nodes a file node
// is created
//
CTreeCtrlNode CTreeCtrlDB::GetFileNode(CRichEditDoc *pDoc)
{
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	CTreeCtrlNode fileNode;
	bool bFound = false;
	for (fileNode =  this->GetRootItem().GetChild(); fileNode != NULL; fileNode = fileNode.GetNextSibling())
	{
		if (pDoc == (CRichEditDoc*)fileNode.GetData())
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		// node not found so add it
		fileNode = InsertItem(pDoc->GetTitle(), CTreeCtrlKeyword::fileImage, CTreeCtrlKeyword::fileImage, this->GetRootItem());
		fileNode.SetData((DWORD)pDoc);
		ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);
	}
	ASSERT(fileNode && fileNode.GetLevel() == CTreeCtrlKeyword::FileLevel);
	return fileNode;
}
