// EditGrid.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITGRID_H__4058E9AA_07C1_11D3_BB09_02608C8F4400__INCLUDED_)
#define AFX_EDITGRID_H__4058E9AA_07C1_11D3_BB09_02608C8F4400__INCLUDED_

#include "mshflexgrid.h"
#include "DBObject.h"

#if defined(UNICODE) || defined(_UNICODE)
#error Unicode not supported
#endif

// Messages
#define EGM_FIRST             WM_APP + 1
#define EGM_EDITCELL          EGM_FIRST
#define EGM_ENDEDITCELLNOW    EGM_FIRST + 1

// Notifications
#define EGN_FIRST             WM_APP + 100
#define EGN_BEGINCELLEDIT     EGN_FIRST
#define EGN_ENDCELLEDIT       EGN_FIRST + 1
#define EGN_SETFOCUS          EGN_FIRST + 2
#define EGN_KILLFOCUS         EGN_FIRST + 3

#define EGN_ALLOW_INSERT_ROW  EGN_FIRST + 4
#define EGN_ALLOW_INSERT_COL  EGN_FIRST + 6

#define EGN_ALLOW_DELETE_ROW  EGN_FIRST + 7
#define EGN_ALLOW_DELETE_COL  EGN_FIRST + 8

#define EGN_ALLOW_SHIFT_RIGHT EGN_FIRST + 9
#define EGN_ALLOW_SHIFT_DOWN  EGN_FIRST + 10

#define EGN_DELETE_ROW        EGN_FIRST + 11
#define EGN_DELETE_COL        EGN_FIRST + 12

#define EGN_INSERT_ROW        EGN_FIRST + 13
#define EGN_INSERT_COL        EGN_FIRST + 14

#define EGN_CHANGE            EGN_FIRST + 15

#define EGN_RESIZE_ROWS       EGN_FIRST + 16
#define EGN_RESIZE_COLS       EGN_FIRST + 17

typedef struct tagEGITEM FAR* HGRIDITEM;

// Structures

typedef struct tagEGITEM {
	UINT mask;
	HGRIDITEM hItem;
	long iRow;
	long iCol;
	LPTSTR pszText;
	LPVOID pVoid;
	//{{NEW
	HWND hWndCombo;
	BOOL bUseCombo;
	//}}NEW
} EGITEM, FAR *LPEGITEM;

typedef struct tagNMEGINFO {
    NMHDR hdr;
    EGITEM item;
} NMEGINFO, FAR *LPNMEGINFO;

#define EGIF_TEXT               0x0001
#define EGIF_COMBOLIST          0x0002
//#define EGIF_PARAM              0x0004
//#define EGIF_STATE              0x0008
//#define EGIF_HANDLE             0x0010
//#define EGIF_SELECTEDIMAGE      0x0020
//#define EGIF_CHILDREN           0x0040


//// AddModeSettings Property
const int flexNoAddNew      = 0;	// The current cell is not in the last row, and no AddNew operation is pending. 
const int flexAddNewCurrent = 1;	// The current cell is in the last row, but no AddNew operation is pending. 
const int flexAddNewPending = 2;	// The current cell is in the next to last row. This is a result of either a pending AddNew operation initiated by the user through the user interface of the MSHFlexGrid, or setting a column’s Value or Text property programmatically.
							
//// AllowUserResizing Property
const int flexResizeNone    = 0;	// The user cannot resize with the mouse. This is the default. 
const int flexResizeColumns = 1;	// The user can resize columns using the mouse.
const int flexResizeRows    = 2;	// The user can resize rows using the mouse.
const int flexResizeBoth    = 3;	// The user can resize columns and rows using
								// the mouse.


//// Appearance Property
const int flexFlat = 0;	// The overall look of MSHFlexGrid is flat, or normal. 
const int flex3D   = 1;	// The overall look of MSHFlexGrid is three-dimensional. 


//// BorderStyle Property
const int flexBorderNone   = 0;	// There is no border. 
const int flexBorderSingle = 1;	// There is a single border. 

//// CellTextStyle, TextStyle, TextStyleFixed Properties
const int flexTextFlat        = 0;	// The text is normal, flat text. 
const int flexTextRaised      = 1;	// The text appears raised. 
const int flexTextInset       = 2;	// The text appears inset. 
const int flexTextRaisedLight = 3;	// The text appears slightly raised. 
const int flexTextInsetLight  = 4;	// The text appears slightly inset. 


//// CellType Properties (MSHFlexGrid)
const int flexCellTypeStandard    = 0;	// The cell is a standard cell. 
const int flexCellTypeFixed       = 1;	// The cell is contained in a fixed row or column. 
const int flexCellTypeHeader      = 2;	// The cell is a header cell for a band of data. 
const int flexCellTypeIndent      = 3;	// The cell is used in a column that indents a band of data. 
const int flexCellTypeUnpopulated = 4;	// The cell is an unpopulated cell. 


//// CellAlignment, ColAlignment, CellPictureAlignment Properties
const int flexAlignLeftTop      = 0;
const int flexAlignLeftCenter   = 1;
const int flexAlignLeftBottom   = 2;
const int flexAlignCenterTop    = 3;
const int flexAlignCenterCenter = 4;
const int flexAlignCenterBottom = 5;
const int flexAlignRightTop     = 6;
const int flexAlignRightCenter  = 7;
const int flexAlignRightBottom  = 8;
const int flexAlignGeneral      = 9;


//// ColHeader Property (MSHFlexGrid)
const int flexColHeaderOn   = 0;	// The headers display for each band. 
const int flexColHeaderOff  = 1;	// No headers display for the band. 
const int flexColHeaderOnce = 2;	// The headers display for the band. If the band is collapsed, only one header displays. Duplicate headers don’t display. 


//// DisplayBandSettings Property (MSHFlexGrid)
const int flexBandDisplayHorizontal = 0;	// The bands within the MSHFlexGrid display horizontally (across). This is the default. 
const int flexBandDisplayVertical   = 1;	// The bands within the MSHFlexGrid display vertically (up and down). 


//// DisplayCellSettings Property (MSHFlexGrid)
const int flexCellDisplayHorizontal = 0;	// The bands within the MSHFlexGrid display horizontally (across). 
const int flexCellDisplayVertical   = 1;	// The bands within the MSHFlexGrid display vertically (up and down). 


//// FillStyle Property
const int flexFillSingle = 0;	// Single. Changing Text or any of the cell properties only affects the active cell. This is the default. 
const int flexFillRepeat = 1;	// Repeat. Changing the Text or any of the cell properties affects all selected cells. 


//// FocusRect Property
const int flexFocusNone  = 0;	// There is no focus rectangle around the current cell. 
const int flexFocusLight = 1;	// There is a light focus rectangle around the current cell. This is the default. 
const int flexFocusHeavy = 2;	// There is a heavy focus rectangle around the current cell. 


//// GridLines, GridLinesFixed Properties
const int flexGridNone   = 0;	// No Lines. There are no lines between cells. In the MSFlexGrid, this is the default for GridLines. 
const int flexGridFlat   = 1;	// Lines. The line style between cells is set to normal, flat lines. In the MSHFlexGrid, this is the default for GridLines. 
const int flexGridInset  = 2;	// Inset Lines. The line style between cells is set to inset lines. In the MSFlexGrid, this is the default for GridLinesFixed. 
const int flexGridRaised = 3;	// Raised Lines. The line style between cells is set to raised lines. In the MSHFlexGrid, this is the default for GridLinesFixed. 


//// HighLight Property
const int flexHighlightNever     = 0;	// There is no highlight on the selected cells. 
const int flexHighlightAlways    = 1;	// The selected cells are always highlighted. This is the default. 
const int flexHighlightWithFocus = 2;	// The highlight appears only when the control has focus. 


//// MergeCells Property
const int flexMergeNever            = 0;	// Never. The cells containing identical content are not grouped. This is the default. 
const int flexMergeFree             = 1;	// Free. Cells with identical content always merge. 
const int flexMergeRestrictRows     = 2;	// Restrict Rows. Only adjacent cells (to the left) within the row containing identical content merge. 
const int flexMergeRestrict_Columns = 3;	// Restrict Columns. Only adjacent cells (to the top) within the column containing identical content merge. 
const int flexMergeRestrictBoth     = 4;	// Restrict Both. Only adjacent cells within the row (to the left) or column (to the top) containing identical content merge. 

//// MousePointer Property
const int flexDefault   = 0; 	// The outline of MSHFlexGrid. 
const int flexArrow     = 1; 	// An arrow pointer. 
const int flexCross     = 2; 	// A set of crosshairs. 
const int flexIBeam     = 3; 	// An "I"-shaped beam to allow for text entry. 
const int flexIcon      = 4; 	// A square within a square. 
const int flexSize      = 5; 	// A resizing pointer with arrows facing up, down, left, and right. 
const int flexSizeNESW  = 6; 	// A resizing pointer with arrows pointing to the upper right and lower left. 
const int flexSizeNS    = 7; 	// A resizing pointer with arrows pointing up and down. 
const int flexSizeNWSE  = 8; 	// A resizing pointer with arrows pointing to the upper left and lower right. 
const int flexSizeEW    = 9; 	// A resizing pointer with arrows pointing left and right. 
const int flexUpArrow   = 10;	// An arrow pointing up. 
const int flexHourGlass = 11;	// An hourglass symbol. 
const int flexNoDrop    = 12;	// A no drop pointer. 
const int flexCustom    = 13;	// A custom style pointer. 


//// PictureType Property
const int flexPictureColor      = 0;	// This produces a high-quality full-color image.  
const int flexPictureMonochrome = 1;	// This produces a lower-quality, monochrome, image that consumes less memory. 


//// RowSizingSettings Property (MSHFlexGrid)
const int flexRowSizeIndividual = 0;	// Resizing a row changes only the height of the row sized. This is the default. 
const int flexRowSizeAll        = 1;	// Resizing a row changes the height of all rows in the MSHFlexGrid. 


//// SelectionMode Property
const int flexSelectionFree     = 0;	// Free. This allows individual cells in the MSHFlexGrid to be selected, spreadsheet-style. (Default) 
const int flexSelectionByRow    = 1;	// By Row. This forces selections to span entire rows, as in a multi-column list box or record-based display. 
const int flexSelectionByColumn = 2;	// By Column. This forces selections to span entire columns, as if selecting ranges for a chart or fields for sorting. 


//// ScrollBars Property
const int flexScrollNone       = 0;	// The MSHFlexGrid has no scroll bars. 
const int flexScrollHorizontal = 1;	// The MSHFlexGrid has a horizontal scroll bar. 
const int flexScrollVertical   = 2;	// The MSHFlexGrid has a vertical scroll bar. 
const int flexScrollBoth       = 3;	// The MSHFlexGrid has horizontal and vertical scroll bars. This is the default. 


//// Sort Property
const int flexSortNone                 = 0;	// None. No sorting is performed. 
const int flexSortGenericAscending     = 1;	// Generic Ascending. An ascending sort, which estimates whether text is string or number, is performed. 
const int flexSortGenericDescending    = 2;	// Generic Descending. A descending sort, which estimates whether text is string or number, is performed. 
const int flexSortNumericAscending     = 3;	// Numeric Ascending. An ascending sort, which converts strings to numbers, is performed. 
const int flexSortNumericDescending    = 4;	// Numeric Descending. A descending sort, which converts strings to numbers, is performed. 
const int flexSortStringNoCaseAsending = 5;	// String Ascending. An ascending sort using case-insensitive string comparison is performed. 
const int flexSortNoCaseDescending     = 6;	// String Descending. A descending sort using case-insensitive string comparison is performed. 
const int flexSortStringAscending      = 7;	// String Ascending. An ascending sort using case-sensitive string comparison is performed. 
const int flexSortStringDescending     = 8;	// String Descending. A descending sort using case-sensitive string comparison is performed. 
const int flexSortCustom               = 9;	// Custom. This uses the Compare event to compare rows. 



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEditGrid window

class CEditGrid : public CMSHFlexGrid
{
// Construction
public:
	CEditGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditGrid)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// PreTranslateMessage functions based on m_currentMode
	BOOL FilterEdit(MSG* pMsg);
	BOOL FilterEnter(MSG* pMsg);
	BOOL FilterReady(MSG* pMsg);

// Implementation
public:
	void ClearContents();
	void AddRowHeaders();
	void ClearWorkSpace();
	void DeleteCol(long nCol);
	void DeleteRow(long nRow);
	void DeleteRowAndHeader(long nRow);
	void DisableCell(long nRow, long nCol);
	void EnableCell(long nRow, long nCol);
	BOOL EnableWindow(BOOL bEnable = TRUE);
	void Fail(long nRow, long nCol, LPCTSTR szText, LPCTSTR szCaption); // deprecated
	BOOL GetCellEnabled(long nRow, long nCol);
	bool GetTrimOnEntry(){return m_bTrimOnEntry;}
	void InsertRow(long nRow);
	void MoveCurSel(int VK_CODE);
	void SetHeaderCell(long nRow, long nCol);
	void SetHeaderCol(long nCol);
	void SetTrimOnEntry(bool bTrim){m_bTrimOnEntry = bTrim;}

	bool GetShowRowSelection()const {return m_bShowRowSelection;}
	bool GetShowColSelection()const {return m_bShowColSelection;}

	void SetShowRowSelection(bool bShow) {m_bShowRowSelection = bShow;}
	void SetShowColSelection(bool bShow) {m_bShowColSelection = bShow;}

	long GetScrollBars();
	void SetScrollBars(long nNewValue);

	void SetSortedCombos(bool bSort)  {m_bSortCombos = bSort;}

#ifdef _DEBUG
// COMMENT: {7/7/2000 1:31:06 PM}	CString GetTextMatrix(long Row, long Col)const;
#endif

	virtual ~CEditGrid();

	// Generated message map functions
protected:
	//
	// State variables
	//

	// Used to hold current grid edit state
	enum modeType
	{
		readyMode = 1,
		enterMode = 2,
		editMode  = 3,
	} m_currentMode;

	// Used to hold state of combo box
	bool m_bComboListDown;
	bool m_bComboTextValid;

	// Used to hold edit cell coordinates
	long m_nEditCol;
	long m_nEditRow;

	// Used to restore user resizing during edit
	long m_nAllowUserResizing;

	// Used to restore highlight
	long m_nHighLight;

	// Number of pixels per logical inch along the screen height
	int m_nLogY;
	// Number of pixels per logical inch along the screen width
	int m_nLogX;

	// colors
	bool m_bMoreThan256Colors;

	COLORREF m_clrBackColorSel;
	COLORREF m_clrForeColorSel;

	COLORREF m_clrBackColorFixedSel;
	COLORREF m_clrForeColorFixedSel;

	COLORREF m_clrBackColorFixed;
	COLORREF m_clrForeColorFixed;


	// Used to determine if text entered into the grid should be trimmed of whitespace
	// true by default
	bool m_bTrimOnEntry;

	// Used to determine if a row should be highlighted when it is selected
	// true by default
	bool m_bShowRowSelection;

	// Used to determine if a column should be highlighted when it is selected
	// true by default
	bool m_bShowColSelection;


	// Used to hold state of scrollbars
	long m_nScrollBars;

	// Sort 
	bool m_bSortCombos;

	CFont m_font;

protected:
	//
	// Controls
	//
	CEdit     m_ctrlEdit;
	CComboBox m_ctrlCombo;
	CWnd*     m_pEditWnd; // holds current edit control

	//{{AFX_MSG(CEditGrid)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	friend class XSafeSaveSelection;

	void ShowEditWindow();
	void EndEdit(bool bFillSelection = false);
	void StartEnterMode(MSG *pMsg);
	void StartEditMode(BOOL bForceList = FALSE);
	void CancelEdit();
	void ClearSelectedRowsAndCols();
	void NotifyChange();


	afx_msg LRESULT OnEditCell(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndEditCellNow(WPARAM wParam, LPARAM lParam);

	// Context Menu Commands
	afx_msg void OnPickFromList();
	afx_msg void OnClearContents();
	afx_msg void OnCopy();
	afx_msg void OnCut();
	afx_msg void OnPaste();

	afx_msg void OnDelete();
	afx_msg void OnInsert();

	// Edit Control Notifications
	afx_msg void On_EN_KillFocus();
	afx_msg void On_EN_Change();


	// Combo Box Notifications
	afx_msg void On_CBN_CloseUp();	
	afx_msg void On_CBN_DropDown();	
	afx_msg void On_CBN_KillFocus();
	afx_msg void On_CBN_SelEndCancel();
	afx_msg void On_CBN_SelEndOK();
	afx_msg void On_CBN_EditChange();

	// Reflected Messages
	//afx_msg BOOL OnReflectClick();
	//afx_msg BOOL OnReflectCollapse(BOOL FAR* Cancel);
	//afx_msg BOOL OnReflectCompare(long Row1, long Row2, short FAR* Cmp);
	afx_msg BOOL OnReflectDblClick();
	//afx_msg BOOL OnReflectEnterCell();
	//afx_msg BOOL OnReflectExpand(BOOL FAR* Cancel);
	//afx_msg BOOL OnReflectKeyDown(short FAR* KeyCode, short Shift);
	//afx_msg BOOL OnReflectKeyPress(short FAR* KeyAscii);
	//afx_msg BOOL OnReflectKeyUp(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnReflectLeaveCell();
	afx_msg BOOL OnReflectMouseDown(short Button, short Shift, long x, long y);
	afx_msg BOOL OnReflectMouseMove(short Button, short Shift, long x, long y);
	//afx_msg BOOL OnReflectMouseUp(short Button, short Shift, long x, long y);
	//afx_msg BOOL OnReflectOLECompleteDrag(long FAR* Effect);
	//afx_msg BOOL OnReflectOLEDragDrop(LPDISPATCH FAR* Data, long FAR* Effect, short FAR* Button, short FAR* Shift, float FAR* x, float FAR* y);
	//afx_msg BOOL OnReflectOLEDragOver(LPDISPATCH FAR* Data, long FAR* Effect, short FAR* Button, short FAR* Shift, float FAR* x, float FAR* y, short FAR* State);
	//afx_msg BOOL OnReflectOLEGiveFeedback(long FAR* Effect, BOOL FAR* DefaultCursors);
	//afx_msg BOOL OnReflectOLESetData(LPDISPATCH FAR* Data, short FAR* DataFormat);
	afx_msg BOOL OnReflectOLEStartDrag(LPDISPATCH FAR* Data, long FAR* AllowedEffects);
	//afx_msg BOOL OnReflectRowColChange();
	afx_msg BOOL OnReflectScroll();
	afx_msg BOOL OnReflectSelChange();
	DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITGRID_H__4058E9AA_07C1_11D3_BB09_02608C8F4400__INCLUDED_)
