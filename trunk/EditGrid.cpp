// EditGrid.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "phreeqci2.h"
#include "EditGrid.h"

#include "font.h"
#ifndef _DEBUG
#pragma warning(disable : 4097)   // typedef-name 'istream' used as synonym for class-name 'basic_istream<char,struct std::char_traits<char> >'
#endif
#include <strstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class XSafeDelayRedraw
{
public:
	XSafeDelayRedraw(CEditGrid* pEditGrid) : m_pEditGrid(pEditGrid), bRedraw(pEditGrid->GetRedraw()) {pEditGrid->SetRedraw(FALSE);}
	~XSafeDelayRedraw() {if (bRedraw) m_pEditGrid->SetRedraw(TRUE);}
	const BOOL bRedraw;
private:
	CEditGrid* m_pEditGrid;
};

class XSafeSaveSelection
{
public:
	XSafeSaveSelection(CEditGrid* pEditGrid) : m_pEditGrid(pEditGrid), nRow(pEditGrid->GetRow()), nCol(pEditGrid->GetCol()), nRowSel(pEditGrid->GetRowSel()), nColSel(pEditGrid->GetColSel()) {}
	~XSafeSaveSelection() {
		m_pEditGrid->SetRow(nRow); 
		m_pEditGrid->SetCol(nCol); 
		m_pEditGrid->SetRowSel(nRowSel); 
		m_pEditGrid->SetColSel(nColSel); 
		m_pEditGrid->OnReflectSelChange();
	}
public:
	const long nRow;
	const long nCol;
	const long nRowSel;
	const long nColSel;
private:
	CEditGrid* m_pEditGrid;
};


/////////////////////////////////////////////////////////////////////////////
// CEditGrid

CEditGrid::CEditGrid()
{
	m_currentMode = readyMode;

	m_bComboListDown = false;
	m_bComboTextValid = false;

	m_pEditWnd = NULL;
	m_nEditRow = -1;
	m_nEditCol = -1;

	m_bTrimOnEntry = true;

	// colors
	m_bMoreThan256Colors = false;
	m_clrBackColorSel = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrForeColorSel = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	m_clrBackColorFixed = ::GetSysColor(COLOR_3DFACE);
	m_clrForeColorFixed = ::GetSysColor(COLOR_BTNTEXT);

	m_clrBackColorFixedSel = m_clrBackColorFixed;
	m_clrForeColorFixedSel = m_clrForeColorFixed;

	m_bShowRowSelection = true;
	m_bShowColSelection = true;
}

CEditGrid::~CEditGrid()
{
}


BEGIN_MESSAGE_MAP(CEditGrid, CMSHFlexGrid)
	//{{AFX_MSG_MAP(CEditGrid)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_HELPINFO()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	// Custom messages
	ON_MESSAGE(EGM_EDITCELL, OnEditCell)
	ON_MESSAGE(EGM_ENDEDITCELLNOW, OnEndEditCellNow)
	// Edit Control Notifications
	ON_EN_KILLFOCUS(IDC_MSHFLEXGRIDEDIT, On_EN_KillFocus)
	ON_EN_CHANGE(IDC_MSHFLEXGRIDEDIT, On_EN_Change)
	// Combo Box Notifications
	ON_CBN_CLOSEUP(IDC_MSHFLEXGRIDCOMBO, On_CBN_CloseUp)
	ON_CBN_DROPDOWN(IDC_MSHFLEXGRIDCOMBO, On_CBN_DropDown)
	ON_CBN_KILLFOCUS(IDC_MSHFLEXGRIDCOMBO, On_CBN_KillFocus)
	ON_CBN_SELENDCANCEL(IDC_MSHFLEXGRIDCOMBO, On_CBN_SelEndCancel)
	ON_CBN_SELENDOK(IDC_MSHFLEXGRIDCOMBO, On_CBN_SelEndOK)
	ON_CBN_EDITCHANGE(IDC_MSHFLEXGRIDCOMBO, On_CBN_EditChange)


	// Context Menu Commands
	ON_COMMAND(ID_PICK_FROM_LIST, OnPickFromList)
	ON_COMMAND(ID_CLEAR_CONTENTS, OnClearContents)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_CUT, OnCut)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_INSERT, OnInsert)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CEditGrid, CMSHFlexGrid)
    //{{AFX_EVENTSINK_MAP(CEditGrid)
	//}}AFX_EVENTSINK_MAP

	//ON_EVENT_REFLECT(CEditGrid, -600 /* Click */, OnReflectClick, VTS_NONE)
	ON_EVENT_REFLECT(CEditGrid, -601 /* DblClick */, OnReflectDblClick, VTS_NONE)
	//ON_EVENT_REFLECT(CEditGrid, -602 /* KeyDown */, OnReflectKeyDown, VTS_PI2 VTS_I2)
	//ON_EVENT_REFLECT(CEditGrid, -603 /* KeyPress */, OnReflectKeyPress, VTS_PI2)
	//ON_EVENT_REFLECT(CEditGrid, -604 /* KeyUp */, OnReflectKeyUp, VTS_PI2 VTS_I2)
	ON_EVENT_REFLECT(CEditGrid, -605 /* MouseDown */, OnReflectMouseDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT_REFLECT(CEditGrid, -606 /* MouseMove */, OnReflectMouseMove, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//ON_EVENT_REFLECT(CEditGrid, -607 /* MouseUp */, OnReflectMouseUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT_REFLECT(CEditGrid, 69 /* SelChange */, OnReflectSelChange, VTS_NONE)
	//ON_EVENT_REFLECT(CEditGrid, 70 /* RowColChange */, OnReflectRowColChange, VTS_NONE)
	//ON_EVENT_REFLECT(CEditGrid, 71 /* EnterCell */, OnReflectEnterCell, VTS_NONE)
	ON_EVENT_REFLECT(CEditGrid, 72 /* LeaveCell */, OnReflectLeaveCell, VTS_NONE)
	ON_EVENT_REFLECT(CEditGrid, 73 /* Scroll */, OnReflectScroll, VTS_NONE)
	//ON_EVENT_REFLECT(CEditGrid, 74 /* Compare */, OnReflectCompare, VTS_I4 VTS_I4 VTS_PI2)
	//ON_EVENT_REFLECT(CEditGrid, 140 /* Collapse */, OnReflectCollapse, VTS_PBOOL)
	//ON_EVENT_REFLECT(CEditGrid, 141 /* Expand */, OnReflectExpand, VTS_PBOOL)
	//ON_EVENT_REFLECT(CEditGrid, 1553 /* OLECompleteDrag */, OnReflectOLECompleteDrag, VTS_PI4)
	//ON_EVENT_REFLECT(CEditGrid, 1555 /* OLEDragDrop */, OnReflectOLEDragDrop, VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4)
	//ON_EVENT_REFLECT(CEditGrid, 1554 /* OLEDragOver */, OnReflectOLEDragOver, VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4 VTS_PI2)
	//ON_EVENT_REFLECT(CEditGrid, 1551 /* OLEGiveFeedback */, OnReflectOLEGiveFeedback, VTS_PI4 VTS_PBOOL)
	//ON_EVENT_REFLECT(CEditGrid, 1552 /* OLESetData */, OnReflectOLESetData, VTS_PDISPATCH VTS_PI2)
	ON_EVENT_REFLECT(CEditGrid, 1550 /* OLEStartDrag */, OnReflectOLEStartDrag, VTS_PDISPATCH VTS_PI4)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditGrid message handlers
BOOL CEditGrid::OnReflectOLEStartDrag(LPDISPATCH FAR* Data, long FAR* AllowedEffects)
{
	UNREFERENCED_PARAMETER(Data);
	UNREFERENCED_PARAMETER(AllowedEffects);
	
	TRACE("CEditGrid::OnReflectOLEStartDrag\n");
	return FALSE;
}

BOOL CEditGrid::OnReflectMouseMove(short Button, short Shift, long x, long y)
{
	UNREFERENCED_PARAMETER(Shift);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	if ((Button & 1) > 0)
	{
		::OutputDebugString(_T("CEditGrid::OnReflectMouseMove() IN\n"));
		// if (::rand() % 7) OnReflectSelChange();
		//if (GetMouseRow(), GetMouseCol())
		::OutputDebugString(_T("CEditGrid::OnReflectMouseMove() OUT\n"));
		// OnReflectSelChange();
	}
	return FALSE;
}

/**
	Make Right Mouse Button act like Left
*/
BOOL CEditGrid::OnReflectMouseDown(short Button, short Shift, long x, long y)
{
	UNREFERENCED_PARAMETER(Shift);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	if (Button == 1) // left mouse button
	{
		///OnReflectSelChange();
	}
	if (Button == 2) // right mouse button
	{
		// if editing end now
		VERIFY(SendMessage(EGM_ENDEDITCELLNOW, (WPARAM)(BOOL)FALSE/*fCancel*/, 0));

// COMMENT: {8/6/2001 8:21:44 PM}		//{{
// COMMENT: {8/6/2001 8:21:44 PM}		// force selection change
// COMMENT: {8/6/2001 8:21:44 PM}		SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
// COMMENT: {8/6/2001 8:21:44 PM}		return FALSE;
// COMMENT: {8/6/2001 8:21:44 PM}		//}}

		long nMouseRow  = GetMouseRow();
		long nFixedRows = GetFixedRows();
		long nMouseCol  = GetMouseCol();
		long nFixedCols = GetFixedCols();
		long nRows      = GetRows();
		long nCols      = GetCols(0);
		long nRow       = GetRow();
		long nRowSel    = GetRowSel();
		long nCol       = GetCol();
		long nColSel    = GetColSel();

		if (__min(nRow, nRowSel) <= nMouseRow && nMouseRow <= __max(nRow, nRowSel) &&
		__min(nCol, nColSel) <= nMouseCol && nMouseCol <= __max(nCol, nColSel))
		{
			// don't change selection
		}
		else if (nMouseRow < nFixedRows && nMouseCol < nFixedCols) // column and row header
		{
// COMMENT: {8/6/2001 8:23:19 PM}			// select all
// COMMENT: {8/6/2001 8:23:19 PM}			SetRow(nFixedRows);
// COMMENT: {8/6/2001 8:23:19 PM}			SetCol(nFixedCols);
// COMMENT: {8/6/2001 8:23:19 PM}			SetRowSel(nRows - 1);
// COMMENT: {8/6/2001 8:23:19 PM}			SetColSel(nCols - 1);
// COMMENT: {8/6/2001 8:23:19 PM}
// COMMENT: {8/6/2001 8:23:19 PM}			OnReflectSelChange();
			//{{
			// force selection change
// COMMENT: {9/6/2001 2:06:40 PM}			SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
// COMMENT: {9/6/2001 2:06:40 PM}			SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
			//{{ NEW {9/6/2001 2:06:40 PM}
			SetFocus();
			//}} NEW {9/6/2001 2:06:40 PM}
			PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
			PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
			return FALSE;
			//}}
		}
		else if (nMouseRow < nFixedRows) // column header
		{
			if ((__min(nCol, nColSel) <= nMouseCol && nMouseCol <= __max(nCol, nColSel)) &&
				__min(nRow, nRowSel) == nFixedRows && __max(nRow, nRowSel) == nRows - 1)
			{
				// don't change selection
			}
			else
			{
// COMMENT: {8/6/2001 8:24:12 PM}				// select col
// COMMENT: {8/6/2001 8:24:12 PM}				SetRow(nFixedRows);
// COMMENT: {8/6/2001 8:24:12 PM}				SetCol(nMouseCol);
// COMMENT: {8/6/2001 8:24:12 PM}				SetRowSel(nRows - 1);
// COMMENT: {8/6/2001 8:24:12 PM}				SetColSel(nMouseCol);
// COMMENT: {8/6/2001 8:24:12 PM}				OnReflectSelChange();
				//{{
				// force selection change
// COMMENT: {9/6/2001 2:07:02 PM}				SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
// COMMENT: {9/6/2001 2:07:02 PM}				SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				//{{ NEW {9/6/2001 2:06:40 PM}
				SetFocus();
				//}} NEW {9/6/2001 2:06:40 PM}
				PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
				PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				return FALSE;
				//}}
			}
		}
		else if (nMouseCol < nFixedCols) // row header
		{
			if ((__min(nRow, nRowSel) <= nMouseRow && nMouseRow <= __max(nRow, nRowSel)) &&
				__min(nCol, nColSel) == nFixedCols && __max(nCol, nColSel) == nCols - 1)
			{
				// don't change selection
			}
			else
			{
// COMMENT: {8/6/2001 8:24:55 PM}				// select row
// COMMENT: {8/6/2001 8:24:55 PM}				SetRow(nMouseRow);
// COMMENT: {8/6/2001 8:24:55 PM}				SetCol(nFixedCols);
// COMMENT: {8/6/2001 8:24:55 PM}				SetRowSel(nMouseRow);
// COMMENT: {8/6/2001 8:24:55 PM}				SetColSel(nCols - 1);
// COMMENT: {8/6/2001 8:24:55 PM}				OnReflectSelChange();
				//{{
				// force selection change
// COMMENT: {9/6/2001 2:07:06 PM}				SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
// COMMENT: {9/6/2001 2:07:06 PM}				SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				//{{ NEW {9/6/2001 2:06:40 PM}
				SetFocus();
				//}} NEW {9/6/2001 2:06:40 PM}
				PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
				PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				return FALSE;
				//}}
			}
		}
		else // no headers
		{
// COMMENT: {8/6/2001 8:24:47 PM}			// select cell
// COMMENT: {8/6/2001 8:24:47 PM}			SetRow(nMouseRow);
// COMMENT: {8/6/2001 8:24:47 PM}			SetCol(nMouseCol);
// COMMENT: {8/6/2001 8:24:47 PM}			SetRowSel(nMouseRow);
// COMMENT: {8/6/2001 8:24:47 PM}			SetColSel(nMouseCol);
// COMMENT: {8/6/2001 8:24:47 PM}			OnReflectSelChange();
			//{{
			// force selection change
// COMMENT: {9/6/2001 2:07:09 PM}			SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
// COMMENT: {9/6/2001 2:07:09 PM}			SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
			//{{ NEW {9/6/2001 2:06:40 PM}
			SetFocus();
			//}} NEW {9/6/2001 2:06:40 PM}
			PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
			PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
			return FALSE;
			//}}
		}
// COMMENT: {8/6/2001 8:24:41 PM}		SetFocus();
	}
	return FALSE;
}

void CEditGrid::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	ASSERT(pWnd && pWnd == this);
	if (pWnd && pWnd == this)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_GRID_POPUP));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);

		if (pPopup)
		{
			if (point.x == -1 && point.y == -1)
			{
				// keystroke invocation
				point.x = MulDiv(GetCellLeft(), m_nLogX, TWIPS_PER_INCH) + MulDiv(GetCellWidth(), m_nLogX, TWIPS_PER_INCH) - 3;
				point.y = MulDiv(GetCellTop(),  m_nLogY, TWIPS_PER_INCH) + MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH) - 3;

				ClientToScreen(&point);
			}

#ifdef UNICODE
			// enable/disable paste
			VERIFY( pPopup->EnableMenuItem((UINT)ID_PASTE,
				::IsClipboardFormatAvailable(CF_UNICODETEXT) ? 
				MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED ) != -1);
#else
			// enable/disable paste
			VERIFY( pPopup->EnableMenuItem((UINT)ID_PASTE,
				::IsClipboardFormatAvailable(CF_TEXT) ? 
				MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED ) != -1);
#endif

			NMEGINFO info = {0};
			info.hdr.code       = EGN_BEGINCELLEDIT;
			info.hdr.hwndFrom   = GetSafeHwnd();
			info.hdr.idFrom     = GetDlgCtrlID();	
			info.item.iRow      = GetRow();
			info.item.iCol      = GetCol();
			// MAYBE: set hWndCombo to NULL to signify just checking if going to use a combobox
			// most if not all OnBeginEdit's will need to be modified
			//{{ NEW {8/6/2001 3:12:06 PM}
			info.item.hWndCombo = NULL; // m_ctrlCombo.m_hWnd;
			//}} NEW {8/6/2001 3:12:06 PM}

			if (!GetCellEnabled(info.item.iRow, info.item.iCol))
			{
				return;
			}

			// see if allow edit
			if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				return;
			}

			// determine if rows are selected
			long nCol = GetCol();
			long nColSel = GetColSel();
			long nFixedCols = GetFixedCols();
			long nCols = GetCols(0);
			bool bRowsSelected = false;
			if (__min(nCol,nColSel) == nFixedCols && __max(nCol,nColSel) == nCols - 1)
			{
				// whole rows are selected
				VERIFY( pPopup->ModifyMenu(ID_INSERT, MF_BYCOMMAND | MF_GRAYED, ID_INSERT, "&Insert"));
				VERIFY( pPopup->ModifyMenu(ID_DELETE, MF_BYCOMMAND | MF_GRAYED, ID_DELETE, "&Delete"));
				bRowsSelected = true;
			}
			// determine if cols are selected
			long nRow = GetRow();
			long nRowSel = GetRowSel();
			long nFixedRows = GetFixedRows();
			long nRows = GetRows();
			bool bColsSelected = false;
			if (__min(nRow,nRowSel) == nFixedRows && __max(nRow,nRowSel) == nRows - 1)
			{
				// whole cols are selected
				VERIFY( pPopup->ModifyMenu(ID_INSERT, MF_BYCOMMAND | MF_GRAYED, ID_INSERT, "&Insert"));
				VERIFY( pPopup->ModifyMenu(ID_DELETE, MF_BYCOMMAND | MF_GRAYED, ID_DELETE, "&Delete"));
				bColsSelected = true;
			}

			if (!(bRowsSelected && bColsSelected)) // leave disabled if ALL selected
			{
				if (bRowsSelected)
				{
					info.item.iRow = __min(nRow,nRowSel);
					info.hdr.code = EGN_ALLOW_INSERT_ROW;
					if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
					{
						// enable Insert (disabled by default)
						VERIFY( pPopup->EnableMenuItem((UINT)ID_INSERT, MF_BYCOMMAND | MF_ENABLED) != -1);
					}
					info.hdr.code = EGN_ALLOW_DELETE_ROW;
					if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
					{
						// enable Delete (disabled by default)
						VERIFY( pPopup->EnableMenuItem((UINT)ID_DELETE, MF_BYCOMMAND | MF_ENABLED) != -1);
					}
				}
				if (bColsSelected)
				{
					info.item.iCol = __min(nCol,nColSel);
					info.hdr.code = EGN_ALLOW_INSERT_COL;
					if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
					{
						// enable Insert (disabled by default)
						VERIFY( pPopup->EnableMenuItem((UINT)ID_INSERT, MF_BYCOMMAND | MF_ENABLED) != -1);
					}
					info.hdr.code = EGN_ALLOW_DELETE_COL;
					if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
					{
						// enable Insert (disabled by default)
						VERIFY( pPopup->EnableMenuItem((UINT)ID_DELETE, MF_BYCOMMAND | MF_ENABLED) != -1);
					}
				}
			}
			else // entire spreadsheet is selected
			{
				// leave Insert disabled
				// leave Delete disabled
			}

			// enable/disable Pick From List...
			VERIFY( pPopup->EnableMenuItem((UINT)ID_PICK_FROM_LIST,
				info.item.bUseCombo ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED ) != -1);

			// show menu
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
}

void CEditGrid::OnCut() 
{
	OnCopy();
	OnClearContents();
}

void CEditGrid::OnInsert()
{
	NMEGINFO info = {0};
	info.hdr.hwndFrom   = GetSafeHwnd();
	info.hdr.idFrom     = GetDlgCtrlID();

	// save selection
	XSafeSaveSelection save(this);
	long nFixedRows = GetFixedRows();
	long nFixedCols = GetFixedCols();
	long nRows      = GetRows();
	long nCols      = GetCols(0);

	// determine what's selected
	bool bRowsSelected = (__min(save.nCol, save.nColSel) == nFixedCols && __max(save.nCol, save.nColSel) == nCols - 1);
	bool bColsSelected = (__min(save.nRow, save.nRowSel) == nFixedRows && __max(save.nRow, save.nRowSel) == nRows - 1);

	XSafeDelayRedraw redraw(this);

	if (bRowsSelected)
	{
		// determine how many rows to delete
		long cSelectedRows = labs(save.nRow - save.nRowSel) + 1;

		// determine first row to delete
		long iStartRow = __min(save.nRow, save.nRowSel);

		// delete each row
		for (long i = 0; i < cSelectedRows; ++i)
		{
			info.hdr.code  = EGN_ALLOW_INSERT_ROW;
			info.item.iRow = iStartRow;
			if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				info.hdr.code  = EGN_INSERT_ROW;
				if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info) == FALSE)
				{
					InsertRow(iStartRow);
				}
			}
		}
	}

	if (bColsSelected)
	{
		// determine how many cols to insert
		long cSelectedCols = labs(save.nCol - save.nColSel) + 1;

		// determine first col to insert
		long iStartCol = __min(save.nCol, save.nColSel);

		// insert each col
		for (long i = 0; i < cSelectedCols; ++i)
		{
			info.hdr.code  = EGN_ALLOW_INSERT_COL;
			info.item.iCol = iStartCol;
			if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				info.hdr.code  = EGN_INSERT_COL;
				if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info) == FALSE)
				{
					SetCols(0, GetCols(0) + 1);
					SetColPosition(GetCols(0) - 1, 0, iStartCol);
				}
			}
		}
	}
}

void CEditGrid::OnDelete()
{
	NMEGINFO info = {0};
	info.hdr.hwndFrom = GetSafeHwnd();
	info.hdr.idFrom   = GetDlgCtrlID();

	// save selection
	XSafeSaveSelection save(this);
	long nFixedRows = GetFixedRows();
	long nFixedCols = GetFixedCols();
	long nRows      = GetRows();
	long nCols      = GetCols(0);

	// determine what's selected
	bool bRowsSelected = (__min(save.nCol, save.nColSel) == nFixedCols && __max(save.nCol, save.nColSel) == nCols - 1);
	bool bColsSelected = (__min(save.nRow, save.nRowSel) == nFixedRows && __max(save.nRow, save.nRowSel) == nRows - 1);

	XSafeDelayRedraw redraw(this);

	if (bRowsSelected)
	{
		// determine how many rows to delete
		long cSelectedRows = abs(save.nRow - save.nRowSel) + 1;

		// determine first row to delete
		long iStartRow = __min(save.nRow, save.nRowSel);

		// delete each row
		for (long i = 0; i < cSelectedRows; ++i)
		{
			info.hdr.code  = EGN_ALLOW_DELETE_ROW;
			info.item.iRow = iStartRow;
			if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				info.hdr.code  = EGN_DELETE_ROW;
				if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info) == FALSE)
				{
					DeleteRow(iStartRow);
				}
			}
		}
	}
	if (bColsSelected)
	{
		// determine how many cols to delete
		long cSelectedCols = abs(save.nCol - save.nColSel) + 1;

		// determine first col to delete
		long iStartCol = __min(save.nCol, save.nColSel);

		// delete each col
		for (long i = 0; i < cSelectedCols; ++i)
		{
			info.hdr.code  = EGN_ALLOW_DELETE_COL;
			info.item.iCol = iStartCol;
			if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				info.hdr.code  = EGN_DELETE_COL;
				if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info) == FALSE)
				{
					DeleteCol(iStartCol);
				}
			}
		}
	}
}

void CEditGrid::OnPaste() 
{
	CWaitCursor wait;

#ifdef UNICODE
	if (::IsClipboardFormatAvailable(CF_UNICODETEXT))
#else
	if (::IsClipboardFormatAvailable(CF_TEXT))
#endif
	{
		if (!::OpenClipboard(m_hWnd))
			return;

#ifdef UNICODE
		HGLOBAL hglb = ::GetClipboardData(CF_UNICODETEXT); 
#else
		HGLOBAL hglb = ::GetClipboardData(CF_TEXT); 
#endif
        if (hglb != NULL) 
        { 
            LPTSTR lptstr = (LPTSTR)::GlobalLock(hglb); 
            if (lptstr != NULL) 
            {
				// copy clipboard into string
				CString str(lptstr);  // throws CMemoryException

				// no longer need clipboard
                ::GlobalUnlock(hglb);
				::CloseClipboard();

				// replace CRLF with CR
				str.Replace(_T("\r\n"), _T("\r"));

				// allocate space to hold SRC
#ifdef UNICODE
				std::vector< std::vector< std::wstring > > vvsCols;
#else
				std::vector< std::vector< std::string > > vvsCols;
#endif

				// determine size and shape of SRC
				CSize sizeSrc(0, 0);
				{
#ifdef UNICODE
					std::wistream issRows(str);
					std::wstring sRow;
#else
					std::istrstream issRows(str);
					std::string sRow;
#endif

					while (issRows.good())
					{
						std::getline(issRows, sRow, _T('\r'));
						if (sRow.empty() && issRows.eof())
							break;
						sizeSrc.cx++;
						std::istrstream issCells(sRow.c_str());
						std::string sCell;
						int nCol = 0;
#ifdef UNICODE
						std::vector<std::wstring> vsRow;
#else
						std::vector<std::string> vsRow;
#endif
						while (issCells.good())
						{
							std::getline(issCells, sCell, _T('\t'));
							nCol++;
							//{{
							if (m_bTrimOnEntry)
							{
								std::string::size_type stBegin = sCell.find_first_not_of(" ");
								if (stBegin != std::string::npos)
								{
									std::string::size_type stEnd = sCell.find_last_not_of(" ");
									if (stEnd != std::string::npos)
									{
										sCell = sCell.substr(stBegin, stEnd - stBegin + 1);
									}
									else
									{
										sCell = sCell.substr(stBegin);
									}
								}
							}
							//}}
							vsRow.push_back(sCell);
						}
						if (nCol > sizeSrc.cy) sizeSrc.cy = nCol;
						vvsCols.push_back(vsRow);
					}
				}

				// save selection
				CPoint ptBegin(GetRow(), GetCol());
				CPoint ptEnd(GetRowSel(), GetColSel());

				// determine size and shape of dest
				CSize sizeDst(__max(ptBegin.x, ptEnd.x) - __min(ptBegin.x, ptEnd.x) + 1, __max(ptBegin.y, ptEnd.y) - __min(ptBegin.y, ptEnd.y) + 1);

				CString strErrMsg(_T("The information cannot be pasted because the copy area and the paste area are not the same size and shape."));

				//{{{
				{
					NMEGINFO info = {0};
					info.hdr.hwndFrom = GetSafeHwnd();
					info.hdr.idFrom   = GetDlgCtrlID();	

					// Check for enough rows
					if (__min(ptBegin.x, ptEnd.x) + sizeSrc.cx - 1 > GetRows())
					{
						info.hdr.code = EGN_RESIZE_ROWS;
						if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
						{
							long nOldLastRow = GetRows() - 1;
							SetRows(__min(ptBegin.x, ptEnd.x) + sizeSrc.cx);
							CString strLabel;
							for (long row = nOldLastRow; row < GetRows(); ++row)
							{
								strLabel.Format(_T("%d"), row);
								SetTextMatrix(row, 0, strLabel);
							}
						}
					}

					// Check for enough cols
					if (__min(ptBegin.y, ptEnd.y) + sizeSrc.cy - 1 > GetCols(0))
					{
						info.hdr.code = EGN_RESIZE_COLS;
						if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
						{
							SetCols(0, __min(ptBegin.y, ptEnd.y) + sizeSrc.cy);
						}
					}
				}
				//}}}

				// check if enough dst space
				//{{ MOD 8/7/2001 4:14:03 PM
				if (!(__min(ptBegin.x, ptEnd.x) + sizeSrc.cx - 1 < GetRows() && __min(ptBegin.y, ptEnd.y) + sizeSrc.cy - 1 < GetCols(0)))
				{
					::AfxMessageBox(strErrMsg);
					return;
				}
				//}} MOD 8/7/2001 4:14:03 PM

				// check for compatible shapes and sizes
				if ((sizeDst.cx % sizeSrc.cx == 0 || sizeSrc.cx % sizeDst.cx == 0) && 
					(sizeDst.cy % sizeSrc.cy == 0 || sizeSrc.cy % sizeDst.cy == 0))
				{
					// OK to paste
				}
				else
				{
					::AfxMessageBox(strErrMsg);
					return;
				}

				// resize dst and selection if nec.
				if (sizeDst.cx < sizeSrc.cx)
				{
					ptEnd.x += sizeSrc.cx - sizeDst.cx;
					sizeDst.cx = sizeSrc.cx;
				}
				if (sizeDst.cy < sizeSrc.cy)
				{
					ptEnd.y += sizeSrc.cy - sizeDst.cy;
					sizeDst.cy = sizeSrc.cy;
				}


				CPoint ptUpperLeft(__min(ptBegin.x, ptEnd.x), __min(ptBegin.y, ptEnd.y));

				NMEGINFO info = {0};
				info.hdr.code     = EGN_ENDCELLEDIT;
				info.hdr.hwndFrom = GetSafeHwnd();
				info.hdr.idFrom   = GetDlgCtrlID();	
				info.item.mask    = EGIF_TEXT;

				// foreach row
				for (info.item.iRow = ptUpperLeft.x; info.item.iRow < ptUpperLeft.x + sizeDst.cx; ++info.item.iRow)
				{
					// foreach col
					for (info.item.iCol = ptUpperLeft.y; info.item.iCol < ptUpperLeft.y + sizeDst.cy; ++info.item.iCol)
					{
						if (GetCellEnabled(info.item.iRow, info.item.iCol))
						{
							info.item.pszText = _tcsdup(vvsCols[(info.item.iRow - ptUpperLeft.x) % sizeSrc.cx][(info.item.iCol - ptUpperLeft.y) % sizeSrc.cy].c_str());
							if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
							{
								str = info.item.pszText;
								if (m_bTrimOnEntry)
								{
									str.TrimLeft();
									str.TrimRight();
								}
								if (info.item.pszText)
								{
									SetTextMatrix(info.item.iRow, info.item.iCol, str);
								}
							}
							free(info.item.pszText);
						}
					}
				}

				// set selection
				SetRow(ptBegin.x);
				SetCol(ptBegin.y);
				SetRowSel(ptEnd.x);
				SetColSel(ptEnd.y);
            }
			else
			{
				::GlobalUnlock(hglb);
				::CloseClipboard();
			}
        } 
	}
}

void CEditGrid::OnCopy() 
{
	if (::OpenClipboard(m_hWnd))
	{
		// Get String from selection
		CString strText = GetClip();

		// make compatible w/ Excell
		strText += _T("\r");

		// Allocate global memory
		HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_DDESHARE, (strText.GetLength() + 1) * sizeof(TCHAR)); 

		if (hglbCopy)
		{
			// Lock it and get a pointer to it
			LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy);

			if (lptstrCopy)
			{
				// Empty existing clipboard contents
				VERIFY(::EmptyClipboard());

				// Copy string into memory
				VERIFY(::lstrcpy(lptstrCopy, (LPCTSTR)strText));

				// Unlock memory
				VERIFY(::GlobalUnlock(hglbCopy));

				// Send memory to clipboard
#ifdef UNICODE
				VERIFY(::SetClipboardData(CF_UNICODETEXT, hglbCopy));
#else
				VERIFY(::SetClipboardData(CF_TEXT, hglbCopy));
#endif

			}
			else
			{
				// Free memory if it couldn't be locked
				::GlobalFree(hglbCopy);
			}

		}
		// Close the clipboard. 
		VERIFY(::CloseClipboard());
	}
}

void CEditGrid::OnPickFromList() 
{
	VERIFY(SendMessage(EGM_EDITCELL, (WPARAM)(BOOL)FALSE/*fCancel*/, (LPARAM)(BOOL)TRUE/*fForceList*/));	
}

/*
	Notes: Emulates EndEdit(TRUE) with an empty string
*/
void CEditGrid::OnClearContents() 
{
	CWaitCursor wait;

	NMEGINFO info = {0};
	info.hdr.code     = EGN_ENDCELLEDIT;
	info.hdr.hwndFrom = GetSafeHwnd();
	info.hdr.idFrom   = GetDlgCtrlID();	
	info.item.mask    = EGIF_TEXT;

	CString str;
	long nRow    = GetRow();
	long nCol    = GetCol();
	long nRowSel = GetRowSel();
	long nColSel = GetColSel();
	for (info.item.iRow = __min(nRow, nRowSel); info.item.iRow <= __max(nRow, nRowSel); ++info.item.iRow)
	{
		for (info.item.iCol = __min(nCol, nColSel); info.item.iCol <= __max(nCol, nColSel); ++info.item.iCol)
		{
			info.item.pszText = _tcsdup(_T(""));
			if (GetCellEnabled(info.item.iRow, info.item.iCol) && GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
			{
				str = info.item.pszText;
				if (m_bTrimOnEntry)
				{
					str.TrimLeft();
					str.TrimRight();
				}
				if (info.item.pszText)
				{
					SetTextMatrix(info.item.iRow, info.item.iCol, str);
				}
			}
			free(info.item.pszText);
		}
	}
}

LRESULT CEditGrid::OnEditCell(WPARAM wParam, LPARAM lParam)
{
	// EGM_EDITCELL 
    // wParam = (WPARAM) (BOOL) fCancel; 
	// lParam = (LPARAM) (BOOL) fForceList
	UNREFERENCED_PARAMETER(wParam);

	if (m_currentMode != readyMode) CancelEdit();

	StartEditMode((BOOL)lParam);
	return (LRESULT)m_pEditWnd;
}


LRESULT CEditGrid::OnEndEditCellNow(WPARAM wParam, LPARAM lParam)
{
	// EGM_ENDEDITCELLNOW 
    // wParam = (WPARAM) (BOOL) fCancel; 
    // lParam = 0; 
	UNREFERENCED_PARAMETER(lParam);

	if (m_currentMode == readyMode)
	{
		ASSERT(m_pEditWnd == NULL);
		return TRUE;
	}

	if ((BOOL)wParam) 
	{
		ASSERT(m_pEditWnd != NULL);
		CancelEdit();
	}
	else
	{
		ASSERT(m_pEditWnd != NULL);
		EndEdit();
	}

	return TRUE;
}


void CEditGrid::PreSubclassWindow() 
{
	ASSERT(::IsWindow(m_hWnd));

	CDC* pDC = GetDC();
	m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

	// colors
	m_bMoreThan256Colors = (pDC->GetDeviceCaps(BITSPIXEL) > 8);
	// m_bMoreThan256Colors = (pDC->GetDeviceCaps(BITSPIXEL) > 16);

	if (m_bMoreThan256Colors)
	{
		m_clrBackColorSel = pDC->GetNearestColor(RGB(165, 165, 210));
		m_clrForeColorSel = pDC->GetNearestColor(RGB(0, 0, 0));

		SetBackColorSel(m_clrBackColorSel);
		SetForeColorSel(m_clrForeColorSel);

		m_clrBackColorFixed = ::GetSysColor(COLOR_3DFACE);
		m_clrForeColorFixed = ::GetSysColor(COLOR_BTNTEXT);

		m_clrBackColorFixedSel = pDC->GetNearestColor(RGB(43, 47, 55));
		m_clrForeColorFixedSel = pDC->GetNearestColor(RGB(255, 255, 255));
	}


	// save state of scrollbars
	m_nScrollBars = CMSHFlexGrid::GetScrollBars();

	//{{TEST
// COMMENT: {7/20/2000 2:31:42 PM}	CFont tempFont;
// COMMENT: {7/20/2000 2:31:42 PM}	VERIFY(tempFont.CreateStockObject(ANSI_VAR_FONT));
// COMMENT: {7/20/2000 2:31:42 PM}	// CFont* pGridFont = CFont::CreateStockObject(GetStockObject(ANSI_VAR_FONT);
// COMMENT: {7/20/2000 2:31:42 PM}	CFont* pGridFont = pDC->SelectObject(&tempFont);
// COMMENT: {7/20/2000 2:31:42 PM}	ASSERT_VALID(pGridFont);
// COMMENT: {7/20/2000 2:31:42 PM}	LOGFONT lf;
// COMMENT: {7/20/2000 2:31:42 PM}	pGridFont->GetLogFont(&lf);
// COMMENT: {7/20/2000 2:31:42 PM}	m_font.CreateFontIndirect(&lf);
	//}}TEST
	ReleaseDC(pDC);

	m_ctrlEdit.Create(WS_CHILD|ES_MULTILINE|ES_WANTRETURN|WS_BORDER|ES_AUTOHSCROLL,
		CRect(0, 0, 0, 0),
		this,
		IDC_MSHFLEXGRIDEDIT
		);


	m_ctrlCombo.Create(WS_CHILD|WS_BORDER|/*CBS_DROPDOWNLIST*/CBS_DROPDOWN|WS_VSCROLL|CBS_AUTOHSCROLL /*|CBS_SORT*/,
		CRect(0, 0, 0, 0),
		this,
		IDC_MSHFLEXGRIDCOMBO
		);


	ASSERT_VALID(GetParent());

	//{{NEW{7/20/2000 12:06:57 PM}
	COleFont gridFont = GetFont();
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	_tcsncpy(lf.lfFaceName, gridFont.GetName(), LF_FACESIZE);
	lf.lfWeight = gridFont.GetWeight();
	lf.lfHeight = -MulDiv(gridFont.GetSize().Lo, m_nLogY, 720000);
	lf.lfCharSet = (unsigned char)gridFont.GetCharset();
	lf.lfItalic = (unsigned char)gridFont.GetItalic();
	lf.lfStrikeOut = (unsigned char)gridFont.GetStrikethrough();
	// for some reason CEdit doesn't support underline
	lf.lfUnderline = (unsigned char)gridFont.GetUnderline();

	m_font.CreateFontIndirect(&lf);
#ifdef _DEBUG
	LOGFONT lfDebug;
	m_font.GetLogFont(&lfDebug);
	ASSERT(lfDebug.lfUnderline == lf.lfUnderline);
#endif
	m_ctrlEdit.SetFont(&m_font/*GetParent()->GetFont()*/);
#ifdef _DEBUG
	m_ctrlEdit.GetFont()->GetLogFont(&lfDebug);
	ASSERT(lfDebug.lfUnderline == lf.lfUnderline);
#endif
	//}}NEW{7/20/2000 12:06:57 PM}

	m_ctrlCombo.SetFont(GetParent()->GetFont());

	CMSHFlexGrid::PreSubclassWindow();
}


BOOL CEditGrid::PreTranslateMessage(MSG* pMsg) 
{
	// Return
	// TRUE if the message was translated and should not be dispatched;
	// FALSE if the message was not translated and should be dispatched.

	//NMHDR nmhdr;
	
// COMMENT: {7/24/2000 2:31:55 PM}	//{{NEW
// COMMENT: {7/24/2000 2:31:55 PM}	if (pMsg->message == WM_RBUTTONDOWN)
// COMMENT: {7/24/2000 2:31:55 PM}	{
// COMMENT: {7/24/2000 2:31:55 PM}		pMsg->message = WM_LBUTTONDOWN;
// COMMENT: {7/24/2000 2:31:55 PM}	}
// COMMENT: {7/24/2000 2:31:55 PM}	if (pMsg->message == WM_RBUTTONUP)
// COMMENT: {7/24/2000 2:31:55 PM}	{
// COMMENT: {7/24/2000 2:31:55 PM}		pMsg->message = WM_LBUTTONUP;
// COMMENT: {7/24/2000 2:31:55 PM}	}
// COMMENT: {7/24/2000 2:31:55 PM}
// COMMENT: {7/24/2000 2:31:55 PM}	//}}NEW

	switch (m_currentMode)
	{
	case readyMode:
		return FilterReady(pMsg); 
	case enterMode:
		return FilterEnter(pMsg); 
	case editMode:
		return FilterEdit(pMsg); 
	default:
		ASSERT(FALSE);
		return CMSHFlexGrid::PreTranslateMessage(pMsg);
	}

}

BOOL CEditGrid::OnReflectLeaveCell()
{
	switch (m_currentMode)
	{
	case readyMode:
		break;

	case enterMode: case editMode:
		ASSERT(m_pEditWnd);
		EndEdit();
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

BOOL CEditGrid::OnReflectSelChange()
{
	//{{FIX
	return FALSE;
	//}}FIX
	::OutputDebugString(_T("CEditGrid::OnReflectSelChange() IN\n"));
	//{{
	if (!(m_bShowRowSelection | m_bShowColSelection))
	{
		// nothing to do
		::OutputDebugString(_T("CEditGrid::OnReflectSelChange() OUT\n"));
		return FALSE;
	}
	//}}

	long nFixedRows = GetFixedRows();
	long nFixedCols = GetFixedCols();
	if (nFixedCols == 0 && nFixedRows == 0)
	{
		// nothing to do
		::OutputDebugString(_T("CEditGrid::OnReflectSelChange() OUT\n"));
		return FALSE;
	}

	// save state
	long nRow       = GetRow();
	long nCol       = GetCol();
	long nRowSel    = GetRowSel();
	long nColSel    = GetColSel();
	long nRows      = GetRows();
	long nCols      = GetCols(0);

	bool bRowsSelected = (__min(nCol,nColSel) == nFixedCols && __max(nCol,nColSel) == nCols - 1);
	bool bColsSelected = (__min(nRow,nRowSel) == nFixedRows && __max(nRow,nRowSel) == nRows - 1);

	if (!(bRowsSelected | bColsSelected))
	{
		ClearSelectedRowsAndCols();
		::OutputDebugString(_T("CEditGrid::OnReflectSelChange() OUT\n"));
		return FALSE;
	}

	XSafeDelayRedraw redraw(this);

	long nFillStyle = GetFillStyle();
	SetFillStyle(flexFillRepeat);

	if (bRowsSelected && bColsSelected)
	{
		// all selected
		SetRow(0);
		SetCol(0);
		if (nFixedCols > 0 && m_bShowRowSelection)
		{
			SetRowSel(nRows - 1);
			SetColSel(nFixedCols - 1);
			SetCellBackColor(m_clrBackColorFixedSel);
			SetCellForeColor(m_clrForeColorFixedSel);
		}
		if (nFixedRows > 0 && m_bShowColSelection)
		{
			SetRowSel(nFixedRows - 1);
			SetColSel(nCols - 1);
			SetCellBackColor(m_clrBackColorFixedSel);
			SetCellForeColor(m_clrForeColorFixedSel);
		}
		SetFillStyle(nFillStyle);

		// reset selection
		SetRow(nRow);
		SetCol(nCol);
		SetRowSel(nRowSel);
		SetColSel(nColSel);

		::OutputDebugString(_T("CEditGrid::OnReflectSelChange() OUT\n"));
		return FALSE;
	}

	ClearSelectedRowsAndCols();

	if (bRowsSelected && nFixedCols > 0 && m_bShowRowSelection)
	{
		SetRow(nRow);
		SetCol(0);
		SetRowSel(nRowSel);
		SetColSel(nFixedCols - 1);
		SetCellBackColor(m_clrBackColorFixedSel);
		SetCellForeColor(m_clrForeColorFixedSel);
	}

	if (bColsSelected && nFixedRows > 0 && m_bShowColSelection)
	{
		// whole col selection
		SetRow(0);
		SetCol(nCol);
		SetRowSel(nFixedRows - 1);
		SetColSel(nColSel);
		SetCellBackColor(m_clrBackColorFixedSel);
		SetCellForeColor(m_clrForeColorFixedSel);
	}

	// restore state
	SetFillStyle(nFillStyle);


	// reset selection
	SetRow(nRow);
	SetCol(nCol);
	SetRowSel(nRowSel);
	SetColSel(nColSel);

	::OutputDebugString(_T("CEditGrid::OnReflectSelChange() OUT\n"));
	return FALSE;
}

BOOL CEditGrid::OnReflectScroll()
{
	// NOTE:
	// GetCellLeft() and GetCellTop() may cause a scroll event
	// even when GetRowIsVisible() and GetColIsVisible() are true
	//
	// The extra scroll event causes the m_pEditWnd to be set to
	// NULL if it is a combobox
	//
	// This is inconsistant compared to when m_pEditWnd is a textbox
	//

	TRACE("In CEditGrid::OnReflectScroll\n");
	// this moves the edit window if a cell is currently being
	// edited during a scroll
	if (m_currentMode != readyMode)
	{
		ASSERT(m_pEditWnd);
		if (GetRowIsVisible(GetRow()) && GetColIsVisible(GetCol()))
		{
			int left   = MulDiv(GetCellLeft(),   m_nLogX, TWIPS_PER_INCH);
			int top    = MulDiv(GetCellTop(),    m_nLogY, TWIPS_PER_INCH);
			int width  = MulDiv(GetCellWidth(),  m_nLogX, TWIPS_PER_INCH);
			int height = MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH);

			if (m_pEditWnd == NULL) return false;

			if (GetBorderStyle())
			{
				ASSERT(m_pEditWnd);
				m_pEditWnd->MoveWindow(left - 3, top - 3, width, height);
				ASSERT(m_pEditWnd);
			}
			else
			{
				ASSERT(m_pEditWnd);
				m_pEditWnd->MoveWindow(left, top, width, height);
				ASSERT(m_pEditWnd);
			}
			ASSERT(m_pEditWnd);
			m_pEditWnd->ShowWindow(SW_SHOWNORMAL);
			ASSERT(m_pEditWnd);
		}
		else
		{
			// Note: this causes the edit control to lose focus
			// see OnEnKillFocus
			m_pEditWnd->ShowWindow(SW_HIDE);
		}
		m_pEditWnd->SetFocus();
		RedrawWindow();
	}
#ifdef _DEBUG
	else
	{
		ASSERT(m_pEditWnd == NULL);
		ASSERT(m_ctrlEdit.IsWindowVisible() == FALSE);
		ASSERT(m_ctrlCombo.IsWindowVisible() == FALSE);
	}
#endif
	TRACE("Out CEditGrid::OnReflectScroll\n");
	return FALSE;
}

void CEditGrid::OnSetFocus(CWnd* pOldWnd) 
{
	CMSHFlexGrid::OnSetFocus(pOldWnd);

	NMHDR hdr = {0};
	hdr.code     = EGN_SETFOCUS;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom   = GetDlgCtrlID();


	OnReflectSelChange();

	GetParent()->SendMessage(hdr.code, (WPARAM)hdr.idFrom);	

	// Note: This does not let grid get the focus
	// ever!
	//if (m_pEditWnd) {
	//	m_pEditWnd->SetFocus();
	//}
	
}


void CEditGrid::CancelEdit()
{
	ASSERT(m_currentMode != readyMode);
	ASSERT(m_nEditRow == GetRow());
	ASSERT(m_nEditCol == GetCol());

	m_currentMode = readyMode;

	// parent must be a dialog
	ASSERT(GetParent()->IsKindOf(RUNTIME_CLASS(CDialog)) == TRUE);

	// SetFocus() does not always work
	// see WM_NEXTDLGCTL
	((CDialog*)GetParent())->GotoDlgCtrl(this);

	m_pEditWnd->ShowWindow(SW_HIDE);
	m_pEditWnd = NULL;


	NMEGINFO info = {0};
	info.hdr.code     = EGN_ENDCELLEDIT;
	info.hdr.hwndFrom = GetSafeHwnd();
	info.hdr.idFrom   = GetDlgCtrlID();	
	info.item.mask    = EGIF_TEXT;
	info.item.iRow    = m_nEditRow = GetRow();
	info.item.iCol    = m_nEditCol = GetCol();
	info.item.pszText = NULL;

	GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info);

	SetAllowUserResizing(m_nAllowUserResizing);

#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	SetHighLight(m_nHighLight);
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
}

BOOL CEditGrid::FilterReady(MSG *pMsg)
{
	long r1, r2;
	long c1, c2;

	// sanity check
	ASSERT(m_currentMode == readyMode);
	ASSERT(m_pEditWnd == NULL);

	switch (pMsg->message)
	{
	case WM_CHAR :
		// Deny if cell is disabled
		if (GetCellEnabled(GetRow(), GetCol()) == FALSE)
		{
			// notify user
			MessageBeep(0);
			break;
		}
		if (pMsg->wParam == 24) // Ctrl+X
		{
			OnCut();
			break;
		}
		if (pMsg->wParam == 3) // Ctrl+C
		{
			OnCopy();
			break;
		}
		if (pMsg->wParam == 22) // Ctrl+V
		{
			OnPaste();
			break;
		}
		// Deny if any other control character (except backspace ^H)
		if (pMsg->wParam < VK_SPACE && pMsg->wParam != VK_BACK)
		{
			// notify user
			MessageBeep(0);
			break;
		}

		switch (pMsg->wParam)
		{
		case VK_SPACE :
			// Shift+Space
			if (GetKeyState(VK_SHIFT) < 0)
			{
				//
				// Select entire row on Shift+Space
				//
				r1 = min(GetRow(), GetRowSel());
				r2 = max(GetRow(), GetRowSel());

				SetCol(GetFixedCols());
				SetRow(r1);

				SetColSel(GetCols(0) - 1);
				SetRowSel(r2);
				OnReflectSelChange();
				break;
			}
			// Ctrl+Space
			if (GetKeyState(VK_CONTROL) < 0)
			{
				//
				// Select entire col on Ctrl+Space
				//
				c1 = min(GetCol(), GetColSel());
				c2 = max(GetCol(), GetColSel());

				SetCol(c1);
				SetRow(GetFixedRows());

				SetColSel(c2);
				SetRowSel(GetRows() - 1);
				OnReflectSelChange();
				break;
			}
			// fall through (let space w/o shift or ctrl go though)
		default :
			//
			// Forward message to current edit control
			//
			StartEnterMode(pMsg);
			break;
		}
		break;

	case WM_KEYDOWN :
		switch (pMsg->wParam)
		{
		case VK_TAB :
			if (GetKeyState(VK_CONTROL) < 0)
			{
				// ignore Ctrl+Tab and Ctrl+Shift+Tab
				break;
			}

			if (GetAsyncKeyState(VK_SHIFT) < 0) 
			{
				MoveCurSel(VK_LEFT);
			}
			else
			{
				MoveCurSel(VK_RIGHT);
			}
			return TRUE; // handled
			break;

		case VK_RETURN :
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
			if (GetKeyState(VK_CONTROL) < 0)
			{
				// do nothing on Ctrl+Enter in ReadyMode
				return TRUE;
			}
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */

			if (GetAsyncKeyState(VK_SHIFT) < 0) 
			{
				MoveCurSel(VK_UP);
			}
			else
			{
				MoveCurSel(VK_DOWN);
			}
			return TRUE; // handled
			break;

		case VK_F2 :
			StartEditMode();
			//{{ NEW July 20, 2000
			if (m_pEditWnd && m_pEditWnd == &m_ctrlEdit)
			{
				// scroll caret position
				m_ctrlEdit.SendMessage(EM_SCROLLCARET, 0, 0);

			}
			//}} NEW July 20, 2000

			break;

		case VK_F4 :
			//pMsg->message = CB_SHOWDROPDOWN;
			//pMsg->wParam = TRUE;
			//pMsg->lParam = 0;
			
			//StartEnterMode(pMsg);
			StartEditMode(TRUE);
			return TRUE; // handled
			break;

		case VK_INSERT :
			if (GetKeyState(VK_CONTROL) < 0)
			{
				OnCopy();
			}
			if (GetKeyState(VK_SHIFT) < 0)
			{
				OnPaste();
			}
			break;
		case VK_DELETE :
			if (GetKeyState(VK_SHIFT) < 0)
			{
				OnCut();
			}
			break;
		case VK_LEFT :
			if (!(GetKeyState(VK_SHIFT) < 0))
			{
				if (GetCol() == GetFixedCols())
				{
					SetRowSel(GetRow());
					SetColSel(GetCol());
					OnReflectSelChange();
				}
			}
			break;
		case VK_RIGHT :
			if (!(GetKeyState(VK_SHIFT) < 0))
			{
				if (GetCol() == GetCols(0) - 1)
				{
					SetRowSel(GetRow());
					SetColSel(GetCol());
					OnReflectSelChange();
				}
			}
			break;

		}
		break;

	case WM_SYSKEYDOWN :
		switch (pMsg->wParam)
		{
		case VK_DOWN : case VK_UP :
			StartEditMode(TRUE);
			return TRUE;
			break;
		}
		break;

	}
	return CMSHFlexGrid::PreTranslateMessage(pMsg);
}

BOOL CEditGrid::FilterEnter(MSG *pMsg)
{
	ASSERT(m_pEditWnd != NULL);
	ASSERT(m_nEditRow == GetRow());
	ASSERT(m_nEditCol == GetCol());

	switch (pMsg->message)
	{
	case WM_KEYDOWN :
		switch (pMsg->wParam)
		{
		case VK_ESCAPE :
			//{{NEW {May 15, 2001}
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				// implicit CBN_SelEndCancel
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			//}}NEW {May 15, 2001}

			//{{{6/29/2000 11:44:30 AM}
			if (m_pEditWnd != &m_ctrlCombo || m_bComboListDown == false)
			{
				CancelEdit();
				return TRUE;
			}
			break;
			//}}{6/29/2000 11:44:30 AM}

		case VK_RETURN :
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				// Implicit CBN_SelEndOK()
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			else
			{
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
				if (GetKeyState(VK_CONTROL) < 0)
				{
					EndEdit(true);
				}
				else
				{
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
					EndEdit();
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
// COMMENT: {8/7/2001 3:56:29 PM}					long nSaveRowSel = GetRowSel();
// COMMENT: {8/7/2001 3:56:29 PM}					if (GetRows() > GetRow() + 1)
// COMMENT: {8/7/2001 3:56:29 PM}					{
// COMMENT: {8/7/2001 3:56:29 PM}						SetRow(GetRow() + 1);
// COMMENT: {8/7/2001 3:56:29 PM}						if (GetRow() < nSaveRowSel)
// COMMENT: {8/7/2001 3:56:29 PM}						{
// COMMENT: {8/7/2001 3:56:29 PM}							SetRowSel(nSaveRowSel);
// COMMENT: {8/7/2001 3:56:29 PM}						}
// COMMENT: {8/7/2001 3:56:29 PM}					}
// COMMENT: {8/7/2001 3:56:29 PM}					return TRUE;
				}
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
			}
			// don't move current selection if Ctrl key is down
			if (!(GetKeyState(VK_CONTROL) < 0))
			{
				if (GetKeyState(VK_SHIFT) < 0)
				{
					MoveCurSel(VK_UP);
				}
				else
				{
					MoveCurSel(VK_DOWN);
				}
			}
			return TRUE;

		case VK_F2 :
			// toggle edit mode
			m_currentMode = editMode;
			break;

		case VK_DOWN :
			//{{NEW{6/28/2000 7:43:46 PM}
			if (m_pEditWnd == &m_ctrlCombo)
			{
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			//}}NEW{6/28/2000 7:43:46 PM}
// COMMENT: {6/28/2000 7:43:46 PM}			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
// COMMENT: {6/28/2000 7:43:46 PM}			{
// COMMENT: {6/28/2000 7:43:46 PM}				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
// COMMENT: {6/28/2000 7:43:46 PM}			}

			if (GetRow() == GetRows() - 1)
			{
				EndEdit();				
			}
			break;

		case VK_UP :
			//{{NEW{6/28/2000 7:43:46 PM}
			if (m_pEditWnd == &m_ctrlCombo)
			{
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			//}}NEW{6/28/2000 7:43:46 PM}
// COMMENT: {6/28/2000 7:45:09 PM}			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
// COMMENT: {6/28/2000 7:45:09 PM}			{
// COMMENT: {6/28/2000 7:45:09 PM}				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
// COMMENT: {6/28/2000 7:45:09 PM}			}

			if (GetRow() == GetFixedRows())
			{
				EndEdit();
			}
			break;

		case VK_LEFT :
			if (GetCol() == GetFixedCols())
			{
				EndEdit();
			}
			break;

		case VK_RIGHT :
			if (GetCol() == GetCols(0) - 1)
			{
				EndEdit();
			}
			break;

		case VK_PRIOR : /*Page Up*/ case VK_NEXT : /*Page Down*/
			//{{NEW{6/28/2000 7:43:46 PM}
			if (m_pEditWnd == &m_ctrlCombo)
			{
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
// COMMENT: {6/28/2000 7:46:24 PM}			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
// COMMENT: {6/28/2000 7:46:24 PM}			{
// COMMENT: {6/28/2000 7:46:24 PM}				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
// COMMENT: {6/28/2000 7:46:24 PM}			}
			break;

		case VK_TAB :
			//{{ NEW 5/15/2001
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				// EAT TAB
				return TRUE;
			}
			//}} NEW 5/15/2001
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				m_ctrlCombo.ShowDropDown(FALSE);
			}

			if (GetKeyState(VK_CONTROL) < 0)
				// ignore Ctrl+Tab and Ctrl+Shift+Tab
				break;

			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				MoveCurSel(VK_LEFT);
			}
			else
			{
				MoveCurSel(VK_RIGHT);
			}
			return TRUE;
			break;
		}

		break;
	}
	return CMSHFlexGrid::PreTranslateMessage(pMsg);
}

BOOL CEditGrid::FilterEdit(MSG *pMsg)
{
	ASSERT(m_pEditWnd != NULL);
	ASSERT(m_nEditRow == GetRow());
	ASSERT(m_nEditCol == GetCol());

	switch (pMsg->message)
	{
	case WM_KEYDOWN :
		switch (pMsg->wParam)
		{
		case VK_TAB :
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				//{{ NEW 5/15/2001
				// EAT TAB
				return TRUE;
				//}} NEW 5/15/2001
			}

			if (GetKeyState(VK_CONTROL) < 0)
				// ignore Ctrl+Tab and Ctrl+Shift+Tab
				break;

			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				MoveCurSel(VK_LEFT);
			}
			else
			{
				MoveCurSel(VK_RIGHT);
			}
			return TRUE;

			break;

		case VK_ESCAPE :
			//{{NEW {May 15, 2001}
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				// implicit CBN_SelEndCancel
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			//}}NEW {May 15, 2001}
			if (m_pEditWnd != &m_ctrlCombo)
			{
				CancelEdit();
				return TRUE;
			}
			//{{NEW{7/22/2000}
			else if (!m_bComboListDown)
			{
				CancelEdit();
				return TRUE;
			}
			//}}NEW{7/20/2000}
			break;

		case VK_RETURN :
			//{{NEW{6/28/2000 7:29:32 PM}
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				// Implicit CBN_SelEndOK()
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			//}}NEW{6/28/2000 7:29:32 PM}
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				m_ctrlCombo.ShowDropDown(FALSE);
			}

			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState() && m_ctrlCombo.GetCurSel() != LB_ERR)
			{
				m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				// this implicitly calls On_CBN_SelEndOK()
			}
			if (m_pEditWnd == &m_ctrlCombo)
			{
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
				if (GetKeyState(VK_CONTROL) < 0)
				{
					EndEdit(true);
				}
				else
				{
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
					EndEdit();
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
				}
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
			}
			if (m_pEditWnd == &m_ctrlEdit)
			{
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
				if (GetKeyState(VK_CONTROL) < 0)
				{
					EndEdit(true);
				}
				else
				{
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
					EndEdit();
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
				}
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
			}

			if (!(GetKeyState(VK_CONTROL) < 0))
			{
				if (GetKeyState(VK_SHIFT) < 0)
				{
					MoveCurSel(VK_UP);
				}
				else
				{
					MoveCurSel(VK_DOWN);
				}
			}
			return TRUE;

		case VK_F2 :
			// toggle edit mode
			m_currentMode = enterMode;
			break;

		// forward movement keys to edit control
		case VK_DOWN :      // fall through
		case VK_UP :        // fall through
		case VK_RIGHT :     // fall through
		case VK_LEFT :      // fall through
		case VK_HOME :      // fall through
		case VK_END :
			//{{ NEW 5/15/2001
			if (m_pEditWnd == &m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				if (pMsg->wParam == VK_RIGHT)
				{
					return m_pEditWnd->SendMessage(pMsg->message, VK_DOWN, pMsg->lParam);
				}
				if (pMsg->wParam == VK_LEFT)
				{
					return m_pEditWnd->SendMessage(pMsg->message, VK_UP, pMsg->lParam);
				}
			}
			//}} NEW 5/15/2001

			return m_pEditWnd->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			break;

		case VK_PRIOR : /*Page Up*/ case VK_NEXT : /*Page Down*/
			if (m_pEditWnd == & m_ctrlCombo && m_ctrlCombo.GetDroppedState())
			{
				return m_ctrlCombo.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			break;

		}
		break;
	case EM_SETSEL:
		if (m_pEditWnd == &m_ctrlEdit)
		{
			return m_ctrlEdit.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		}
		break;
	//{{NEW {6/28/2000 3:52:00 PM}
	// Fixes Alt+VK_DOWN Alt+VK_DOWN causing EndCellEdit message bug 
	case WM_SYSKEYDOWN :
// COMMENT: {6/28/2000 7:15:14 PM}		switch (pMsg->wParam)
// COMMENT: {6/28/2000 7:15:14 PM}		{
// COMMENT: {6/28/2000 7:15:14 PM}		case VK_DOWN:
// COMMENT: {6/28/2000 7:15:14 PM}			// act as if escape was pressed
// COMMENT: {6/28/2000 7:15:14 PM}			CancelEdit();
// COMMENT: {6/28/2000 7:15:14 PM}			return TRUE;
// COMMENT: {6/28/2000 7:15:14 PM}			break;
// COMMENT: {6/28/2000 7:15:14 PM}		}
		break;
	//}}NEW {6/28/2000 3:52:00 PM}
	}
	return CMSHFlexGrid::PreTranslateMessage(pMsg);
}

void CEditGrid::StartEnterMode(MSG *pMsg)
{
	TRACE("In StartEnterMode\n");
	ASSERT(m_currentMode == readyMode);

	ASSERT_VALID(GetParent());

	NMEGINFO info = {0};
	info.hdr.code       = EGN_BEGINCELLEDIT;
	info.hdr.hwndFrom   = GetSafeHwnd();
	info.hdr.idFrom     = GetDlgCtrlID();	
	info.item.mask      = 0;
	info.item.iRow      = m_nEditRow = GetRow();
	info.item.iCol      = m_nEditCol = GetCol();
	info.item.hWndCombo = m_ctrlCombo.m_hWnd;
	info.item.bUseCombo = FALSE;

	if (!GetCellEnabled(info.item.iRow, info.item.iCol))
	{
		return;
	}
	if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
	{
		return;
	}

#ifndef TURNOFF_SCROLL_FIX_04_10_2001
	GetCellLeft();
	GetCellTop();
#endif /* TURNOFF_SCROLL_FIX_04_10_2001 */

	m_currentMode = enterMode;
	m_nAllowUserResizing = GetAllowUserResizing();
	SetAllowUserResizing(flexResizeNone);


#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	m_nHighLight = GetHighLight();
	SetHighLight(flexHighlightAlways);
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */

	if (info.item.bUseCombo)
	{
		m_pEditWnd = &m_ctrlCombo;
	}
	else
	{
		m_pEditWnd = &m_ctrlEdit;
	}

	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->SetWindowText(_T(""));

	ShowEditWindow();

	// forward message to edit control
	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	TRACE("Out StartEnterMode\n");
}

void CEditGrid::StartEditMode(BOOL bForceList /* FALSE */)
{
	ASSERT(m_currentMode == readyMode);

	ASSERT_VALID(GetParent());

	NMEGINFO info = {0};
	info.hdr.code       = EGN_BEGINCELLEDIT;
	info.hdr.hwndFrom   = GetSafeHwnd();
	info.hdr.idFrom     = GetDlgCtrlID();	
	info.item.mask      = 0;
	info.item.iRow      = m_nEditRow = GetRow();
	info.item.iCol      = m_nEditCol = GetCol();
	info.item.hWndCombo = m_ctrlCombo.m_hWnd;
	info.item.bUseCombo = FALSE;

	if (!GetCellEnabled(info.item.iRow, info.item.iCol))
	{
		return;
	}
	if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
	{
		return;
	}
	//{{ NEW {6/20/2000 1:06:01 PM}
	if (bForceList && info.item.bUseCombo == FALSE)
	{
		MessageBeep(0);
		return;
	}
	//}} NEW {6/20/2000 1:06:01 PM}

#ifndef TURNOFF_SCROLL_FIX_04_10_2001
	GetCellLeft();
	GetCellTop();
#endif /* TURNOFF_SCROLL_FIX_04_10_2001 */

	m_currentMode = editMode;
	m_nAllowUserResizing = GetAllowUserResizing();
	SetAllowUserResizing(flexResizeNone);

	//{{ NEW Ctrl+Enter {4/9/2001}
	m_nHighLight = GetHighLight();
	SetHighLight(flexHighlightAlways);
	//}} NEW Ctrl+Enter {4/9/2001}

	if (info.item.bUseCombo || bForceList)
	{
		m_pEditWnd = &m_ctrlCombo;
	}
	else
	{
		m_pEditWnd = &m_ctrlEdit;
	}

	CString str = GetText();
	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->SetWindowText(str);

	if (m_pEditWnd == &m_ctrlEdit && str.GetLength())
	{
		//{{NEW {May 15, 2001}
		if (isdigit(str.GetAt(0)))
		{
			// Note: Cannot change alignment on the fly
			// but could create two controls and switch between
			// them
		}
		//}}NEW {May 15, 2001}

		// place caret at end of text w/o scrolling caret
		m_ctrlEdit.SetSel(str.GetLength(), str.GetLength(), TRUE);
	}
	ShowEditWindow();
	if (bForceList && info.item.bUseCombo == FALSE)
	{
		CRect rect;
		m_ctrlCombo.GetClientRect(rect);
		m_ctrlCombo.SetDroppedWidth(rect.Width());
	}
	if (str.IsEmpty())
	{
		bForceList = TRUE;
	}
	if (m_pEditWnd == &m_ctrlCombo/* && bForceList*/)
	{
		if (bForceList)
		{
			m_ctrlCombo.ShowDropDown();
		}
		else
		{
			m_ctrlCombo.SetEditSel(str.GetLength(), str.GetLength());
		}
	}
}

void CEditGrid::EndEdit(bool bFillSelection/* = false */)
{
	// This makes sure that this function does not 
	static int nCalls = 0;
	nCalls++;
	ASSERT(nCalls == 1);		// must not be called recursively

	ASSERT(m_currentMode != readyMode);
	ASSERT(m_nEditRow == GetRow());		// must use appropriate DDX_DDV routines
	ASSERT(m_nEditCol == GetCol());

	CString str;

	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->GetWindowText(str);

	if (m_bTrimOnEntry)
	{
		str.TrimLeft();
		str.TrimRight();
	}	

	// parent must be a dialog
	ASSERT_KINDOF(CDialog, GetParent());

	// SetFocus() does not always work
	// see WM_NEXTDLGCTL
	if (m_pEditWnd == &m_ctrlCombo)
	{
		((CDialog*)GetParent())->GotoDlgCtrl(this);
	}
	// NOTE: This causes a CBN_SELENDOK notification if
	// the list is showing and the edit box has the focus
	// and enter is pressed

	//NEW{{ May 30, 2001
	if (m_pEditWnd == &m_ctrlEdit)
	{
// COMMENT: {9/5/2001 12:05:41 PM}		((CDialog*)GetParent())->GotoDlgCtrl(this);
	}
	//NEW}} May 30, 2001

	NMEGINFO info = {0};
	info.hdr.code     = EGN_ENDCELLEDIT;
	info.hdr.hwndFrom = GetSafeHwnd();
	info.hdr.idFrom   = GetDlgCtrlID();	
	info.item.mask    = EGIF_TEXT;
	info.item.iRow    = m_nEditRow = GetRow();
	info.item.iCol    = m_nEditCol = GetCol();

	//{{NEW{6/29/2000 11:25:02 AM}
	if (str.IsEmpty())
	{
		//{{NEW{9/12/2000 1:08:38 PM}
		// info.item.pszText = NULL;
		info.item.pszText = _tcsdup(str);
		//}}NEW{9/12/2000 1:08:38 PM}
	}
	else
	{
		info.item.pszText = _tcsdup(str);
	}
	//}}NEW{6/29/2000 11:25:02 AM}

#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	if (bFillSelection)
	{
		XSafeDelayRedraw redraw(this);

		CPoint ptBegin(__min(GetRow(), GetRowSel()), __min(GetCol(), GetColSel()));
		CPoint ptEnd(__max(GetRow(), GetRowSel()), __max(GetCol(), GetColSel()));

		for (info.item.iRow = ptBegin.x; info.item.iRow <= ptEnd.x; ++info.item.iRow)
		{
			for (info.item.iCol = ptBegin.y; info.item.iCol <= ptEnd.y; ++info.item.iCol)
			{
				if (GetCellEnabled(info.item.iRow, info.item.iCol) && GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
				{
					str = info.item.pszText;
					if (m_bTrimOnEntry)
					{
						str.TrimLeft();
						str.TrimRight();
					}
					if (info.item.pszText)
					{
						SetTextMatrix(info.item.iRow, info.item.iCol, str);
					}
				}
			}
		}
	}
	else
	{
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
		if (GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info))
		{
			str = info.item.pszText;
			if (m_bTrimOnEntry)
			{
				str.TrimLeft();
				str.TrimRight();
			}
			if (info.item.pszText)
			{
				SetTextMatrix(info.item.iRow, info.item.iCol, str);
			}
		}
#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	}
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */

	free(info.item.pszText);

	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->ShowWindow(SW_HIDE);
	m_currentMode = readyMode;
	TRACE("Setting m_pEditWnd to NULL in EndEdit()\n");
	m_pEditWnd = NULL;

	SetAllowUserResizing(m_nAllowUserResizing);

#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	SetHighLight(m_nHighLight);
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
	nCalls--;
}

void CEditGrid::ShowEditWindow()
{
	if (GetBorderStyle())
	{
		if (m_pEditWnd == &m_ctrlEdit)
		{
			m_pEditWnd->MoveWindow(
				MulDiv(GetCellLeft(),   m_nLogX, TWIPS_PER_INCH) - 3,
				MulDiv(GetCellTop(),    m_nLogY, TWIPS_PER_INCH) - 3,
				MulDiv(GetCellWidth(),  m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH)
				);
		}
		else
		{
			m_pEditWnd->MoveWindow(
				MulDiv(GetCellLeft(),   m_nLogX, TWIPS_PER_INCH) - 3,
				MulDiv(GetCellTop(),    m_nLogY, TWIPS_PER_INCH) - 3,
				MulDiv(GetCellWidth(),  m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH / 6)
				);
		}
	}
	else
	{
		if (m_pEditWnd == &m_ctrlEdit)
		{
			m_pEditWnd->MoveWindow(
				MulDiv(GetCellLeft(),   m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellTop(),    m_nLogY, TWIPS_PER_INCH),
				MulDiv(GetCellWidth(),  m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH)
				);
		}
		else
		{
			m_pEditWnd->MoveWindow(
				MulDiv(GetCellLeft(),   m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellTop(),    m_nLogY, TWIPS_PER_INCH),
				MulDiv(GetCellWidth(),  m_nLogX, TWIPS_PER_INCH),
				MulDiv(GetCellHeight(), m_nLogY, TWIPS_PER_INCH / 6)
				);
		}
	}

	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->ShowWindow(SW_SHOW);

	ASSERT(m_pEditWnd != NULL);
	m_pEditWnd->SetFocus();
}

void CEditGrid::MoveCurSel(int VK_CODE) 
{
	BYTE pbKeyState[256];

	GetKeyboardState(pbKeyState);

	// turn off shift key
	if (pbKeyState[VK_SHIFT] == 129)
	{
		pbKeyState[VK_SHIFT] = 1;
	}
	if (pbKeyState[VK_SHIFT] == 128)
	{
		pbKeyState[VK_SHIFT] = 0;
	}
	SetKeyboardState(pbKeyState);

	PostMessage(WM_KEYDOWN, VK_CODE);
}

void CEditGrid::DeleteCol(long nCol)
{
	CWaitCursor wait();
	long cCols = GetCols(0);
	long cRows = GetRows();

	XSafeDelayRedraw redraw(this);

	for (long iCol = nCol; iCol < cCols - 1; ++iCol)
	{
		for (long iRow = GetFixedRows(); iRow < cRows; ++iRow)
		{	
			SetTextMatrix(iRow, iCol, GetTextMatrix(iRow, iCol + 1));
		}
	}

	for (long iRow = GetFixedRows(); iRow < cRows; ++iRow)
	{	
		SetTextMatrix(iRow, cCols - 1, _T(""));
	}
}

void CEditGrid::InsertRow(long nRowInsert)
{
	XSafeDelayRedraw redraw(this);

	// save selection
	long nRow       = GetRow();
	long nCol       = GetCol();
	long nRowSel    = GetRowSel();
	long nColSel    = GetColSel();	
	// long nFixedRows = GetFixedRows();
	long nFixedCols = GetFixedCols();
	long nRows      = GetRows();
	// long nCols      = GetCols(0);


	// select fixed cols
	SetRow(nRowInsert);
	SetCol(0);
	SetRowSel(nRows - 1);
	SetColSel(nFixedCols - 1);

	CString strClip = GetClip();

	SetRows(GetRows() + 1);
	SetRowPosition(GetRows() - 1, nRowInsert);

	SetRow(nRowInsert);
	SetCol(0);
	SetRowSel(nRows - 1);
	SetColSel(nFixedCols - 1);

	SetClip(strClip);

	CString strPerCentOne;
	strPerCentOne.Format(_T("%ld"), GetRows() - GetFixedRows());

	CString strLabel;
	AfxFormatString1(strLabel, IDS_GRID_ROW_FORMAT531, strPerCentOne);
	SetTextMatrix(GetRows() - 1, 0, strLabel);

	// restore selection
	SetRow(nRow);
	SetCol(nCol);
	SetRowSel(nRowSel);
	SetColSel(nColSel);
	OnReflectSelChange();
}

void CEditGrid::DeleteRow(long nRow)
{
	long cCols = GetCols(0);
	long cRows = GetRows();
	long cFixedCols = GetFixedCols();

	// Note: may want to use GetClip and SetClip w/ selections to avoid
	// notifications using GetTextMatrix or use CMSFlexGrid::SetTextMatrix()
	// instead

	CStringArray strFixed;
	strFixed.SetSize(cFixedCols);

	// save fixed columns of last row
	for (long iFixedCol = 0; iFixedCol < cFixedCols; ++iFixedCol)
	{	
		strFixed[iFixedCol] = GetTextMatrix(cRows - 1, iFixedCol);
	}

	// translate fixed columns
	for (long iRow = cRows - 1; iRow > nRow ; --iRow)
	{
		for (long iFixedCol = 0; iFixedCol < cFixedCols; ++iFixedCol)
		{	
			CMSHFlexGrid::SetTextMatrix(iRow, iFixedCol, GetTextMatrix(iRow - 1, iFixedCol));
		}
	}

	// restore fixed columns of last row
	for (iFixedCol = 0; iFixedCol < cFixedCols; ++iFixedCol)
	{	
		CMSHFlexGrid::SetTextMatrix(nRow, iFixedCol, strFixed[iFixedCol]);
	}

	// make nRow last row
	SetRowPosition(nRow, cRows - 1);

	// clear row
	for (long iCol = cFixedCols; iCol < cCols; ++iCol)
	{	
		CMSHFlexGrid::SetTextMatrix(cRows - 1, iCol, _T(""));
	}
}

void CEditGrid::DeleteRowAndHeader(long nRow)
{
	long cCols = GetCols(0);
	long cRows = GetRows();
	long cFixedCols = GetFixedCols();

	// make nRow last row
	SetRowPosition(nRow, cRows - 1);

	// clear row
	for (long iCol = cFixedCols; iCol < cCols; ++iCol)
	{	
		CMSHFlexGrid::SetTextMatrix(cRows - 1, iCol, _T(""));
	}
}


void CEditGrid::On_EN_KillFocus()
{
	TRACE("CEditGrid::On_EN_KillFocus\n");
	if (m_ctrlEdit.GetFocus() != this)
	{
		//{{NEW May 30, 2001
		if (m_pEditWnd == &m_ctrlEdit)
		//}}NEW May 30, 2001
		EndEdit();
	}
}

void CEditGrid::On_EN_Change()
{
	TRACE("CEditGrid::On_EN_Change\n");
	NotifyChange();
}

void CEditGrid::NotifyChange()
{
	NMEGINFO info = {0};
	info.hdr.code     = EGN_CHANGE;
	info.hdr.hwndFrom = GetSafeHwnd();
	info.hdr.idFrom   = GetDlgCtrlID();	
	info.item.mask    = EGIF_TEXT;
	info.item.iRow    = GetRow();
	info.item.iCol    = GetCol();

	CString str;
	m_pEditWnd->GetWindowText(str);
	info.item.pszText = _tcsdup(str);

	GetParent()->SendMessage(info.hdr.code, (WPARAM)info.hdr.idFrom, (LPARAM)&info);
	free(info.item.pszText);
}

void CEditGrid::OnKillFocus(CWnd* pNewWnd) 
{
	TRACE("CEditGrid::OnKillFocus\n");

	if (m_bShowRowSelection || m_bShowColSelection)
	{
		if (GetHighLight() == flexHighlightWithFocus)
		{
			ClearSelectedRowsAndCols();
		}
	}


	CMSHFlexGrid::OnKillFocus(pNewWnd);

	NMHDR hdr = {0};
	hdr.code     = EGN_KILLFOCUS;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom   = GetDlgCtrlID();

	GetParent()->SendMessage(hdr.code, (WPARAM)hdr.idFrom);

}

void CEditGrid::AddRowHeaders()
{
	CString strLabel;
	int i = 0;
	for (long row = GetFixedRows(); row < GetRows(); ++row)
	{
		strLabel.Format(_T("%d"), ++i);
		SetTextMatrix(row, 0, strLabel);
	}

	SetColWidth(0, 0, 500);
	SetColAlignmentFixed(0, flexAlignCenterCenter);
}

BOOL CEditGrid::OnReflectDblClick()
{
	long nCol = GetMouseCol();
	long nRow = GetMouseRow();
	if (nCol >= GetFixedCols() && nRow >= GetFixedRows())
	{
		if (m_currentMode != readyMode) CancelEdit();
		StartEditMode();
		//{{ NEW July 18, 2000
		if (m_pEditWnd && m_pEditWnd == &m_ctrlEdit)
		{
			// place selection by mouse position
			CPoint pt(::GetMessagePos());
			m_ctrlEdit.ScreenToClient(&pt);		
			int nCharIndex = LOWORD(m_ctrlEdit.CharFromPos(pt));
			m_ctrlEdit.SetSel(nCharIndex, nCharIndex);
		}
		//}} NEW July 18, 2000
		return TRUE;
	}
	return FALSE;
}

void CEditGrid::ClearWorkSpace()
{
	SetRow(GetFixedRows());
	SetCol(GetFixedCols());

	SetRowSel(GetRows() - 1);
	SetColSel(GetCols(0) - 1);

	SetFillStyle(flexFillRepeat);
	SetText(_T(""));
	SetFillStyle(flexFillSingle);

	SetRow(GetFixedRows());
	SetCol(GetFixedCols());
}

void CEditGrid::DisableCell(long nRow, long nCol)
{
	ASSERT(nRow >= GetFixedRows());
	ASSERT(nRow < GetRows());
	ASSERT(nCol >= GetFixedCols());
	ASSERT(nCol < GetCols(0));

	XSafeDelayRedraw redraw(this);

	long nSaveRow = GetRow();
	long nSaveCol = GetCol();

	SetRow(nRow);
	SetCol(nCol);
		
	SetCellBackColor(::GetSysColor(COLOR_3DFACE));

	SetRow(nSaveRow);
	SetCol(nSaveCol);
}

void CEditGrid::EnableCell(long nRow, long nCol)
{
	ASSERT(nRow >= this->GetFixedRows());
	ASSERT(nRow < this->GetRows());
	ASSERT(nCol >= this->GetFixedCols());
	ASSERT(nCol < this->GetCols(0));

	XSafeDelayRedraw redraw(this);

	long nSaveRow = GetRow();
	long nSaveCol = GetCol();

	SetRow(nRow);
	SetCol(nCol);

	SetCellBackColor(::GetSysColor(COLOR_WINDOW));

	SetRow(nSaveRow);
	SetCol(nSaveCol);
}

BOOL CEditGrid::GetCellEnabled(long nRow, long nCol)
{
	BOOL bEnabled;

	XSafeDelayRedraw redraw(this);

	long nSaveRow = GetRow();
	long nSaveCol = GetCol();

#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	long nSaveRowSel = GetRowSel();
	long nSaveColSel = GetColSel();
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */

	SetRow(nRow);
	SetCol(nCol);	

	unsigned long COLOR = GetCellBackColor();

	if (COLOR == ::GetSysColor(COLOR_3DFACE))
	{
		SetRow(nSaveRow);
		SetCol(nSaveCol);
		bEnabled = FALSE;
	}
	else
	{
		SetRow(nSaveRow);
		SetCol(nSaveCol);
		bEnabled = TRUE;
	}

#ifndef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
	SetRowSel(nSaveRowSel);
	SetColSel(nSaveColSel);
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */

	return bEnabled;
}

void CEditGrid::SetHeaderCell(long nRow, long nCol)
{
	ASSERT(nRow >= GetFixedRows());
	ASSERT(nRow < GetRows());
	ASSERT(nCol >= GetFixedCols());
	ASSERT(nCol < GetCols(0));

	XSafeDelayRedraw redraw(this);

	DisableCell(nRow, nCol);

	long nSaveRow = GetRow();
	long nSaveCol = GetCol();

	SetRow(nRow);
	SetCol(nCol);	

	SetCellBackColor(::GetSysColor(COLOR_3DFACE));
	SetCellFontBold(TRUE);
	SetCellAlignment(flexAlignCenterCenter);

	SetRow(nSaveRow);
	SetCol(nSaveCol);
}

void CEditGrid::SetHeaderCol(long nCol)
{
	ASSERT(nCol < GetCols(0));

	XSafeDelayRedraw redraw(this);

	SetRow(GetFixedRows());
	SetCol(nCol);

	SetRowSel(GetRows() - 1);
	SetColSel(nCol);

	SetFillStyle(flexFillRepeat);
	//{{
	SetCellBackColor(::GetSysColor(COLOR_3DFACE));
	SetCellFontBold(TRUE);
	SetCellAlignment(flexAlignCenterCenter);
	//}}
	SetFillStyle(flexFillSingle);
}

/////////////////////////////////////////////////////////////////////////////
// Combo Box Notification handlers

void CEditGrid::On_CBN_CloseUp()
{
	TRACE("CEditGrid::On_CBN_CloseUp()\n");

	if (m_bComboTextValid)
	{
		TRACE("CEditGrid::On_CBN_CloseUp() finish edit\n");

		CString str;
		int nIndex = m_ctrlCombo.GetCurSel();
		if (nIndex == CB_ERR)
		{
			CancelEdit();
		}
		else
		{
			m_ctrlCombo.GetLBText(nIndex, str);
			m_ctrlCombo.SetWindowText(str);
#ifdef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
			EndEdit();
#else  /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
			EndEdit(GetKeyState(VK_CONTROL) < 0);
#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
		}
	}
	else
	{
		TRACE("CEditGrid::On_CBN_CloseUp() cancel edit\n");
		CancelEdit();
	}


// COMMENT: {5/15/2001 5:13:35 PM}	ASSERT(m_bComboListDown == true);
// COMMENT: {5/15/2001 5:13:35 PM}	m_bComboListDown = false;
// COMMENT: {5/15/2001 5:13:35 PM}	
// COMMENT: {5/15/2001 5:13:35 PM}	if (m_bComboTextValid)
// COMMENT: {5/15/2001 5:13:35 PM}	{
// COMMENT: {5/15/2001 5:13:35 PM}		TRACE("CEditGrid::On_CBN_CloseUp() finish edit\n");
// COMMENT: {5/15/2001 5:13:35 PM}
// COMMENT: {5/15/2001 5:13:35 PM}		CString str;
// COMMENT: {5/15/2001 5:13:35 PM}		int nIndex = m_ctrlCombo.GetCurSel();
// COMMENT: {5/15/2001 5:13:35 PM}		if (nIndex == CB_ERR)
// COMMENT: {5/15/2001 5:13:35 PM}		{
// COMMENT: {5/15/2001 5:13:35 PM}			CancelEdit();
// COMMENT: {5/15/2001 5:13:35 PM}		}
// COMMENT: {5/15/2001 5:13:35 PM}		else
// COMMENT: {5/15/2001 5:13:35 PM}		{
// COMMENT: {5/15/2001 5:13:35 PM}			m_ctrlCombo.GetLBText(nIndex, str);
// COMMENT: {5/15/2001 5:13:35 PM}			m_ctrlCombo.SetWindowText(str);
// COMMENT: {5/15/2001 5:13:35 PM}#ifdef TURNOFF_CTRL_ENTER_HANDLER_04_09_2001
// COMMENT: {5/15/2001 5:13:35 PM}			EndEdit();
// COMMENT: {5/15/2001 5:13:35 PM}#else  /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
// COMMENT: {5/15/2001 5:13:35 PM}			EndEdit(GetKeyState(VK_CONTROL) < 0);
// COMMENT: {5/15/2001 5:13:35 PM}#endif /* TURNOFF_CTRL_ENTER_HANDLER_04_09_2001 */
// COMMENT: {5/15/2001 5:13:35 PM}		}
// COMMENT: {5/15/2001 5:13:35 PM}	}
// COMMENT: {5/15/2001 5:13:35 PM}	else
// COMMENT: {5/15/2001 5:13:35 PM}	{
// COMMENT: {5/15/2001 5:13:35 PM}		TRACE("CEditGrid::On_CBN_CloseUp() cancel edit\n");
// COMMENT: {5/15/2001 5:13:35 PM}		CancelEdit();
// COMMENT: {5/15/2001 5:13:35 PM}	}

// COMMENT: {6/28/2000 4:21:25 PM}	CString str;
// COMMENT: {6/28/2000 4:21:25 PM}	int nIndex = m_ctrlCombo.GetCurSel();
// COMMENT: {6/28/2000 4:21:25 PM}	if (m_currentMode != readyMode && nIndex != CB_ERR)
// COMMENT: {6/28/2000 4:21:25 PM}	{
// COMMENT: {6/28/2000 4:21:25 PM}		m_ctrlCombo.GetLBText(nIndex, str);
// COMMENT: {6/28/2000 4:21:25 PM}		m_ctrlCombo.SetWindowText(str);
// COMMENT: {6/28/2000 4:21:25 PM}		EndEdit();
// COMMENT: {6/28/2000 4:21:25 PM}	}
// COMMENT: {6/28/2000 4:21:25 PM}	else
// COMMENT: {6/28/2000 4:21:25 PM}	{
// COMMENT: {6/28/2000 4:21:25 PM}		if (m_currentMode != readyMode)
// COMMENT: {6/28/2000 4:21:25 PM}		{
// COMMENT: {6/28/2000 4:21:25 PM}			EndEdit();
// COMMENT: {6/28/2000 4:21:25 PM}		}
// COMMENT: {6/28/2000 4:21:25 PM}	}
}

void CEditGrid::On_CBN_DropDown()
{
	TRACE("CEditGrid::On_CBN_DropDown()\n");

// COMMENT: {5/15/2001 5:13:44 PM}	ASSERT(m_bComboListDown == false);
// COMMENT: {5/15/2001 5:13:44 PM}	m_bComboListDown = true;
// COMMENT: {5/15/2001 5:13:44 PM}	m_bComboTextValid = false;
}

void CEditGrid::On_CBN_KillFocus()
{
	TRACE("CEditGrid::On_CBN_KillFocus\n");

// COMMENT: {5/15/2001 5:13:50 PM}	ASSERT(m_bComboListDown == false); // cannot lose focus if combo list is displayed
// COMMENT: {5/15/2001 5:13:50 PM}
// COMMENT: {5/15/2001 5:13:50 PM}	if (m_ctrlCombo.GetFocus() != this)
// COMMENT: {5/15/2001 5:13:50 PM}	{
// COMMENT: {5/15/2001 5:13:50 PM}		EndEdit();
// COMMENT: {5/15/2001 5:13:50 PM}	}
}

void CEditGrid::On_CBN_SelEndCancel()
{
	TRACE("CEditGrid::On_CBN_SelEndCancel()\n");

	m_bComboTextValid = false;


// COMMENT: {6/28/2000 6:55:57 PM}	//{{NEW {6/28/2000 3:52:00 PM}
// COMMENT: {6/28/2000 6:55:57 PM}	// Fixes StartEditMode and clicking outside of combobox causing EndCellEdit message bug
// COMMENT: {6/28/2000 6:55:57 PM}	if (m_currentMode != readyMode)
// COMMENT: {6/28/2000 6:55:57 PM}	{
// COMMENT: {6/28/2000 6:55:57 PM}		SendMessage(EGM_ENDEDITCELLNOW, (WPARAM)(BOOL)FALSE, (LPARAM)0);
// COMMENT: {6/28/2000 6:55:57 PM}		//OnEndEditCellNow((WPARAM)(BOOL)TRUE, (LPARAM)0);
// COMMENT: {6/28/2000 6:55:57 PM}	}
// COMMENT: {6/28/2000 6:55:57 PM}	//}}NEW {6/28/2000 3:52:00 PM}
}

void CEditGrid::On_CBN_SelEndOK()
{
	TRACE("CEditGrid::On_CBN_SelEndOK()\n");

	m_bComboTextValid = true;

// COMMENT: {6/28/2000 6:56:44 PM}	//{{NEW {6/28/2000 3:52:00 PM}
// COMMENT: {6/28/2000 6:56:44 PM}	// Fixes StartEditMode and clicking outside of combobox causing EndCellEdit message bug
// COMMENT: {6/28/2000 6:56:44 PM}	if (m_currentMode != readyMode)
// COMMENT: {6/28/2000 6:56:44 PM}	{
// COMMENT: {6/28/2000 6:56:44 PM}// COMMENT: {6/28/2000 4:31:42 PM}		SendMessage(EGM_ENDEDITCELLNOW, (WPARAM)(BOOL)FALSE, (LPARAM)0);
// COMMENT: {6/28/2000 6:56:44 PM}		// OnEndEditCellNow((WPARAM)(BOOL)FALSE, (LPARAM)0);
// COMMENT: {6/28/2000 6:56:44 PM}	}
// COMMENT: {6/28/2000 6:56:44 PM}	//}}NEW {6/28/2000 3:52:00 PM}
}

void CEditGrid::On_CBN_EditChange()
{
	TRACE("CEditGrid::On_CBN_EditChange\n");
	NotifyChange();
}


void CEditGrid::Fail(long nRow, long nCol, LPCTSTR szText, LPCTSTR szCaption)
{
	ASSERT(FALSE);	// use DDX_GridFail() instead
	SetRow(nRow);
	SetCol(nCol);
	SetHighLight(flexHighlightAlways);
	MessageBox(szText, szCaption, MB_OK);
	SetHighLight(flexHighlightWithFocus);
	SetFocus();
	SendMessage(EGM_EDITCELL);
	AfxThrowUserException();
}

BOOL CEditGrid::EnableWindow(BOOL bEnable)
{
	const long DEFAULT_FORECOLOR       = 2147483656;
	const long DEFAULT_FORECOLOR_FIXED = 2147483666;
	const long DEFAULT_BACKCOLOR       = 2147483653;

	if (bEnable)
	{
		SetForeColor(DEFAULT_FORECOLOR);
		SetForeColorFixed(DEFAULT_FORECOLOR_FIXED);
		SetBackColor(DEFAULT_BACKCOLOR);
	}
	else
	{
		SetForeColor(::GetSysColor(COLOR_GRAYTEXT));
		SetForeColorFixed(::GetSysColor(COLOR_GRAYTEXT));
		SetBackColor(::GetSysColor(COLOR_BTNFACE));
	}
	return CMSHFlexGrid::EnableWindow(bEnable);

#ifdef SAVE_OLD
	BOOL bRedraw = GetRedraw();
	SetRedraw(FALSE);

	long nSaveRow = GetRow();
	long nSaveCol = GetCol();
	long nSaveRowSel = GetRowSel();
	long nSaveColSel = GetColSel();

	SetRow(0);
	SetCol(0);
	SetRowSel(GetRows() - 1);
	SetColSel(GetCols(0) - 1);

	SetFillStyle(flexFillRepeat);
	if (bEnable)
	{
		SetCellForeColor(::GetSysColor(COLOR_BTNTEXT));

		SetRow(GetFixedRows());
		SetCol(GetFixedCols());
		SetRowSel(GetRows() - 1);
		SetColSel(GetCols(0) - 1);
		SetCellBackColor(::GetSysColor(COLOR_WINDOW));

		CMSHFlexGrid::SetScrollBars(m_nScrollBars);
	}
	else
	{
		SetCellForeColor(::GetSysColor(COLOR_GRAYTEXT));

		SetRow(GetFixedRows());
		SetCol(GetFixedCols());
		SetRowSel(GetRows() - 1);
		SetColSel(GetCols(0) - 1);
		SetCellBackColor(::GetSysColor(COLOR_3DFACE));

		CMSHFlexGrid::SetScrollBars(flexScrollNone);
	}


	SetFillStyle(flexFillSingle);

	SetRow(nSaveRow);
	SetCol(nSaveCol);
	SetRowSel(nSaveRowSel);
	SetColSel(nSaveColSel);

	// avoid redraw if already off
	if (bRedraw == TRUE)
	{
		SetRedraw(TRUE);
	}


	return CMSHFlexGrid::EnableWindow(bEnable);
#endif // SAVE_OLD
}

long CEditGrid::GetScrollBars()
{
	ASSERT(m_nScrollBars == CMSHFlexGrid::GetScrollBars());
	m_nScrollBars = CMSHFlexGrid::GetScrollBars();
	return m_nScrollBars;
}

void CEditGrid::SetScrollBars(long nNewValue)
{
	CMSHFlexGrid::SetScrollBars(nNewValue);
	m_nScrollBars = CMSHFlexGrid::GetScrollBars();
}

BOOL CEditGrid::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// BUG FIX
	// for some reason this works
	pHelpInfo->iCtrlId = GetDlgCtrlID();
	return Default();	
	//return CMSHFlexGrid::OnHelpInfo(pHelpInfo);
}


void CEditGrid::OnSysColorChange() 
{
	CDC* pDC = NULL;
	if (GetSafeHwnd() && ((pDC = GetDC()) != NULL))
	{
		bool bMoreThan256Colors = (pDC->GetDeviceCaps(BITSPIXEL) > 8);
		if (bMoreThan256Colors)
		{
			m_clrBackColorSel = pDC->GetNearestColor(RGB(165, 165, 210));
			m_clrForeColorSel = pDC->GetNearestColor(RGB(0, 0, 0));

			m_clrBackColorFixed = ::GetSysColor(COLOR_3DFACE);
			m_clrForeColorFixed = ::GetSysColor(COLOR_BTNTEXT);

			m_clrBackColorFixedSel = pDC->GetNearestColor(RGB(43, 47, 55));
			m_clrForeColorFixedSel = pDC->GetNearestColor(RGB(255, 255, 255));
		}
		else
		{
			m_clrBackColorSel = ::GetSysColor(COLOR_HIGHLIGHT);
			m_clrForeColorSel = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

			m_clrBackColorFixed = ::GetSysColor(COLOR_3DFACE);
			m_clrForeColorFixed = ::GetSysColor(COLOR_BTNTEXT);

			m_clrBackColorFixedSel = m_clrBackColorFixed;
			m_clrForeColorFixedSel = m_clrForeColorFixed;

			ClearSelectedRowsAndCols();
		}
		SetBackColorSel(m_clrBackColorSel);
		SetForeColorSel(m_clrForeColorSel);
		if (m_bMoreThan256Colors ^ bMoreThan256Colors)
		{
			OnReflectSelChange();
		}
		m_bMoreThan256Colors = bMoreThan256Colors;
	}

	if (GetFocus() == this)
	{
		OnReflectSelChange();
	}
	else
	{
		ClearSelectedRowsAndCols();
	}
	CMSHFlexGrid::OnSysColorChange();	
	TRACE("CEditGrid::OnSysColorChange()\n");	
}

void CEditGrid::ClearSelectedRowsAndCols() 
{
	::OutputDebugString(_T("CEditGrid::ClearSelectedRowsAndCols() IN\n"));
	XSafeDelayRedraw redraw(this);

	long nFixedRows = GetFixedRows();
	long nFixedCols = GetFixedCols();
	if (nFixedCols == 0 && nFixedRows == 0)
	{
		// nothing to do
		TRACE("CEditGrid::ClearSelectedRowsAndCols() OUT\n");
		::OutputDebugString(_T("CEditGrid::ClearSelectedRowsAndCols() OUT\n"));
		return;
	}

	// save state
	// XSafeSaveSelection save(this); // this causes stack overflow on row or column selection
	const long nRow       = GetRow();
	const long nCol       = GetCol();
	const long nRowSel    = GetRowSel();
	const long nColSel    = GetColSel();
	const long nFillStyle = GetFillStyle();

	SetFillStyle(flexFillRepeat);
	SetRow(0);
	SetCol(0);

	if (nFixedCols > 0)
	{
		// clear col selection
		SetRowSel(GetRows() - 1);
		SetColSel(nFixedCols - 1);
		SetCellBackColor(m_clrBackColorFixed);
		SetCellForeColor(m_clrForeColorFixed);
	}
	if (nFixedRows > 0)
	{
		// clear row selection
		SetRowSel(nFixedRows - 1);
		SetColSel(GetCols(0) - 1);
		SetCellBackColor(m_clrBackColorFixed);
		SetCellForeColor(m_clrForeColorFixed);
	}

	// restore state
	SetRow(nRow);
	SetCol(nCol);
	SetRowSel(nRowSel);
	SetColSel(nColSel);
	SetFillStyle(nFillStyle);
	::OutputDebugString(_T("CEditGrid::ClearSelectedRowsAndCols() OUT\n"));
}

void CEditGrid::ClearContents()
{
	OnClearContents();
}
