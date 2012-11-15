#include "stdafx.h"
#include "ModGridCtrlEx.h"
#include <uxtheme.h>

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif

#include "ListBoxDrop.h"

void AFXAPI DDX_GridCheck(CDataExchange* pDX, int nIDC, int nRow, int nCol, int& nState)
{
	pDX->PrepareCtrl(nIDC);
	CModGridCtrlEx* pGrid = static_cast<CModGridCtrlEx*>(pDX->m_pDlgWnd->GetDlgItem(nIDC));
	ASSERT_KINDOF(CModGridCtrlEx, pGrid);
	ASSERT(pGrid->IsCheckMarkCell(nRow, nCol));
	if (pDX->m_bSaveAndValidate)
	{
		nState = pGrid->GetCheck(nRow, nCol);
	}
	else
	{
		pGrid->SetCheck(nRow, nCol, nState);
	}
}

#ifdef XP_STYLE
CTheme CModGridCtrlEx::s_themeButton;
CTheme CModGridCtrlEx::s_themeCombo;
#endif /* XP_STYLE */

CModGridCtrlEx::CModGridCtrlEx(int nRows, int nCols, int nFixedRows, int nFixedCols)
: CModGridCtrl(nRows, nCols, nFixedRows, nFixedCols)
, m_bButtonDown(FALSE)
, m_bButtonCaptured(FALSE)
{
#ifdef XP_STYLE
	s_themeButton.Create(L"BUTTON");
	s_themeCombo.Create(L"COMBOBOX");
#endif /* XP_STYLE */
}

CModGridCtrlEx::~CModGridCtrlEx(void)
{
	std::map< std::vector<CString>, CListBox* >::iterator it = this->m_mapVectorToList.begin();
	for (; it != this->m_mapVectorToList.end(); ++it)
	{
		delete it->second;
	}
}

BOOL CModGridCtrlEx::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	BOOL bValue = CModGridCtrl::DrawCell(pDC, nRow, nCol, rect, bEraseBk);

	if (bValue)
	{
		GV_ITEM Item;
		Item.mask = GVIF_STATE;
		Item.row = nRow;
		Item.col = nCol;
		if (!GetItem(&Item))
			return FALSE;

		//{{
		bool bDrawHighLight = false;
		switch (this->GetHighLight())
		{
		case GV_HIGHLIGHT_NEVER:
			break;
		case GV_HIGHLIGHT_ALWAYS:
			bDrawHighLight = true;
			break;
		case GV_HIGHLIGHT_WITH_FOCUS:
			bDrawHighLight = (this->GetSafeHwnd() == ::GetFocus());
			break;
		}
		//}}

		// draw button
		if ((Item.state & GVIS_FOCUSED) && bDrawHighLight)
		{
			if (this->IsDropDownCell(nRow, nCol))
			{
				this->DrawButton2(pDC, nRow, nCol, rect, bEraseBk, this->m_bButtonDown);
				// redraw focus rect
				pDC->SelectStockObject(BLACK_PEN);
				pDC->SelectStockObject(NULL_BRUSH);
				pDC->Rectangle(rect);
			}
		}
		if (this->IsCheckMarkCell(nRow, nCol))
		{
			this->DrawButtonCheck(pDC, nRow, nCol, rect, bEraseBk, this->GetCheck(nRow, nCol), this->IsCellEnabled(nRow, nCol));
		}
	}
    return bValue;
}

void CModGridCtrlEx::DrawButton(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, BOOL bDown)
{
	UNREFERENCED_PARAMETER(bEraseBk);
	const int nButtonWidth = 17;
	const CPoint& ptPos = rcCell.TopLeft();
	if (bDown)
	{
		CRect rc(ptPos, CSize(nButtonWidth, this->GetRowHeight(nRow)));
		rc.OffsetRect(max(this->GetColumnWidth(nCol) - nButtonWidth, 0), 0);
		rc.DeflateRect(0, 1, 1, 1);
		this->m_rcButton = rc;

		CBrush brush;
		brush.CreateSysColorBrush(COLOR_3DFACE);
        pDC->FillRect(rc, &brush);

		pDC->DrawEdge(rc, EDGE_SUNKEN, BF_BOTTOMRIGHT);
		rc.DeflateRect(0, 0, 1, 1);
		pDC->DrawEdge(rc, BDR_SUNKENINNER, BF_TOPLEFT);
		rc.DeflateRect(1, 1, 1, 1);
		pDC->DrawEdge(rc, BDR_SUNKENOUTER, BF_TOPLEFT);
	}
	else
	{
		CRect rc(ptPos, CSize(nButtonWidth, this->GetRowHeight(nRow)));
		rc.OffsetRect(max(this->GetColumnWidth(nCol) - nButtonWidth, 0), 0);
		rc.DeflateRect(0, 1, 1, 1);
		this->m_rcButton = rc;

		CBrush brush;
		brush.CreateSysColorBrush(COLOR_3DFACE);
		pDC->FillRect(rc, &brush);

		pDC->DrawEdge(rc, EDGE_RAISED, BF_BOTTOMRIGHT);
		rc.DeflateRect(0, 0, 1, 1);
		pDC->DrawEdge(rc, BDR_RAISEDINNER, BF_TOPLEFT);
	}

	// draw down arrow
	CRect rc(this->m_rcButton.TopLeft(), CSize(7, 1));
	rc.OffsetRect(4, 7);
	if (bDown)
	{
		rc.OffsetRect(1, 1);
	}
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(0, 1);
	rc.DeflateRect(1, 0, 1, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(0, 1);
	rc.DeflateRect(1, 0, 1, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));

	rc.OffsetRect(0, 1);
	rc.DeflateRect(1, 0, 1, 0);
	pDC->FillRect(rc, CBrush::FromHandle((HBRUSH)::GetSysColorBrush(COLOR_BTNTEXT)));
}

CString CModGridCtrlEx::GetItemText(const CCellID& cell)
{
	return CGridCtrl::GetItemText(cell.row, cell.col);
}

CString CModGridCtrlEx::GetItemText(int nRow, int nCol)
{
	return CGridCtrl::GetItemText(nRow, nCol);
}

BOOL CModGridCtrlEx::SetColumnOptions(int nCol, const std::vector<CString>& vecOptions)
{
	try
	{
		CString strWindowName;
#ifdef _DEBUG
		strWindowName.Format(_T("Col %d"), nCol);
#endif
		std::map< std::vector<CString>, CListBox* >::const_iterator item = this->FindOrCreateListBox(vecOptions, strWindowName);
		if (item != this->m_mapVectorToList.end())
		{
			for (int nRow = this->GetFixedRowCount(); nRow < this->GetRowCount(); ++nRow)
			{
				this->SetItemData(nRow, nCol, (LPARAM)&item->first);
				this->SetItemState(nRow, nCol, this->GetItemState(nRow, nCol) | GVIS_DROPDOWN);
			}
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL CModGridCtrlEx::SetCellOptions(int nRow, int nCol, const std::vector<CString>& vecOptions)
{
	try
	{
		CString strWindowName;
#ifdef _DEBUG
		strWindowName.Format(_T("Cell (%d,%d)"), nRow, nCol);
#endif
		std::map< std::vector<CString>, CListBox* >::const_iterator item = this->FindOrCreateListBox(vecOptions, strWindowName);
		if (item != this->m_mapVectorToList.end())
		{
			this->SetItemData(nRow, nCol, (LPARAM)&item->first);
			this->SetItemState(nRow, nCol, this->GetItemState(nRow, nCol) | GVIS_DROPDOWN);
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

bool CModGridCtrlEx::IsDropDownCell(const CCellID& cell)const
{
	return this->IsDropDownCell(cell.row, cell.col);
}

bool CModGridCtrlEx::IsDropDownCell(int nRow, int nCol)const
{
	if (nRow < this->GetFixedRowCount()) return false;
	if (nCol < this->GetFixedColumnCount()) return false;

	if (CGridCell *pCell = CGridCtrl::GetCell(nRow, nCol))
	{
		return ((pCell->state & GVIS_DROPDOWN) != 0);
	}
	return false;
}

bool CModGridCtrlEx::IsCheckMarkCell(const CCellID& cell)const
{
	return this->IsCheckMarkCell(cell.row, cell.col);
}

bool CModGridCtrlEx::IsCheckMarkCell(int nRow, int nCol)const
{
	if (nRow < this->GetFixedRowCount()) return false;
	if (nCol < this->GetFixedColumnCount()) return false;

	if (CGridCell *pCell = CGridCtrl::GetCell(nRow, nCol))
	{
		return ((pCell->state & GVIM_CHECKMARK) != 0);
	}
	return false;
}

int CModGridCtrlEx::GetCheck(const CCellID& cell)const
{
	return this->GetCheck(cell.row, cell.col);
}

int CModGridCtrlEx::GetCheck(int nRow, int nCol)const
{
	if (CGridCell *pCell = CGridCtrl::GetCell(nRow, nCol))
	{
		if (pCell->state & GVIM_CHECKMARK)
		{
			switch (pCell->state & GVIM_CHECKMARK)
			{
			case GVIS_UNCHECKED:
				return BST_UNCHECKED;
			case GVIS_CHECKED:
				return BST_CHECKED;
			case GVIS_INDETERMINATE:
				return BST_INDETERMINATE;
			default:
				return BST_UNCHECKED;
			}
		}
	}
	ASSERT(FALSE);
	return BST_UNCHECKED;
}

BOOL CModGridCtrlEx::SetCheck(const CCellID& cell, int nState)
{
	return this->SetCheck(cell.row, cell.col, nState);
}

BOOL CModGridCtrlEx::SetCheck(int nRow, int nCol, int nState)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
	{
        return FALSE;
	}

	// clear current
    pCell->state &= ~GVIM_CHECKMARK;

	switch (nState)
	{
	case BST_UNCHECKED:
		pCell->state |= GVIS_UNCHECKED;
// COMMENT: {4/29/2005 4:16:27 PM}		pCell->state |= GVIS_READONLY;
		break;
	case BST_CHECKED:
		pCell->state |= GVIS_CHECKED;
// COMMENT: {4/29/2005 4:16:31 PM}		pCell->state |= GVIS_READONLY;
		break;
	case BST_INDETERMINATE:
		pCell->state |= GVIS_INDETERMINATE;
// COMMENT: {4/29/2005 4:16:34 PM}		pCell->state |= GVIS_READONLY;
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
    return TRUE;
}

const std::vector<CString>* CModGridCtrlEx::GetOptionsVector(const CCellID& cell)const
{
	return this->GetOptionsVector(cell.row, cell.col);
}

const std::vector<CString>* CModGridCtrlEx::GetOptionsVector(int nRow, int nCol)const
{
	if (nRow < this->GetFixedRowCount()) return 0;
	if (nCol < this->GetFixedColumnCount()) return 0;

	if (this->IsDropDownCell(nRow, nCol) && this->GetItemData(nRow, nCol))
	{
		std::vector<CString> *pVector = (std::vector<CString>*)this->GetItemData(nRow, nCol);
		std::map< std::vector<CString>, CListBox* >::const_iterator iter = this->m_mapVectorToList.find(*pVector);
		ASSERT(iter != this->m_mapVectorToList.end());
		return pVector;
	}
	return 0;
}

CListBox* CModGridCtrlEx::GetListBox(const CCellID& cell)
{
	return this->GetListBox(cell.row, cell.col);
}

CListBox* CModGridCtrlEx::GetListBox(int nRow, int nCol)
{
	if (nRow < this->GetFixedRowCount()) return 0;
	if (nCol < this->GetFixedColumnCount()) return 0;

	//{{
	if (!this->IsCellEnabled(nRow, nCol)) return 0;
	//}}

	if (this->IsDropDownCell(nRow, nCol) && this->GetItemData(nRow, nCol))
	{
		std::vector<CString> *pVector = (std::vector<CString>*)this->GetItemData(nRow, nCol);
		std::map< std::vector<CString>, CListBox* >::iterator iter = this->m_mapVectorToList.find(*pVector);
		if (iter != this->m_mapVectorToList.end())
		{
			// set owner and font everytime
			// this is necessary in case the grid's OS window's
			// handle is no longer valid
			//
			iter->second->SetFont(&this->m_Font);
			iter->second->SetOwner(this);
			return (CListBox*)(iter->second);
		}
	}
	return 0;
}


BEGIN_MESSAGE_MAP(CModGridCtrlEx, CModGridCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_TIMER()
#ifdef XP_STYLE
	//ON_WM_THEMECHANGED()
	{ WM_THEMECHANGED, 0, 0, 0, AfxSig_l, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(void) > (OnThemeChanged)) },
#endif /* XP_STYLE */
END_MESSAGE_MAP()

void CModGridCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default

    //CCellID cell = this->GetCellFromPt(point);
	//if (!this->IsCheckMarkCell(cell))
	{
		CModGridCtrl::OnLButtonDown(nFlags, point);
		TraceMouseMode();
	}

	if (this->m_MouseMode == MOUSE_PREPARE_EDIT)
	{
		// This state occurs when user clicks within currently selected cell
		if (this->IsDropDownCell(this->m_idCurrentCell))
		{
			ASSERT(this->m_LeftClickDownCell == this->m_idCurrentCell);
			this->m_MouseMode = MOUSE_NOTHING;
			if (this->m_rcButton.PtInRect(point))
			{
				this->SetCapture();
				this->m_bButtonCaptured = TRUE;
				this->m_bButtonDown = TRUE;
				this->RedrawCell(this->m_idCurrentCell);

				CRect rc;
				if (!this->GetCellRect(this->m_idCurrentCell, &rc)) return;
				this->MapWindowPoints(CWnd::GetDesktopWindow(), &rc);

				GV_DISPINFO dispinfo;

				dispinfo.hdr.hwndFrom = GetSafeHwnd();
				dispinfo.hdr.idFrom   = GetDlgCtrlID();
				dispinfo.hdr.code     = LBDM_EDITLABEL;
				dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
				dispinfo.item.row     = this->m_idCurrentCell.row;
				dispinfo.item.col     = this->m_idCurrentCell.col;
				if (!this->GetItem(&dispinfo.item)) return;
				dispinfo.item.lParam  = (LPARAM)&rc; // screen coordinates

				if (CListBox* pListBox = this->GetListBox(this->m_idCurrentCell))
				{
					pListBox->SendMessage(LBDM_EDITLABEL, 0, (LPARAM)&dispinfo);
					pListBox->GetWindowRect(&this->m_rcListBox);
					this->ScreenToClient(&this->m_rcListBox);
				}
			}
		}
		else if (this->IsCheckMarkCell(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col))
		{
			ASSERT(CWnd::GetCapture() == 0);
			CRect rc = this->GetCheckRect(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col);
			if (rc.PtInRect(point))
			{
				TRACE("LButtonDown in Check 1\n");
				this->SetCapture();
				this->m_bButtonCaptured = TRUE;
				this->m_bButtonDown = TRUE;
				this->RedrawCell(this->m_LeftClickDownCell);
			}
			this->m_MouseMode = MOUSE_NOTHING;
		}
		else
		{
			// overide default behavior
			// normally a mouse down in a cell that already has the focus
			// causes an edit to begin
			//
			// this new behavior is more consistant
			// with excel -- Note: double-clicking an already focused cell
			// still causes an edit to begin
			if (this->m_LeftClickDownCell == this->m_idCurrentCell)
			{
				// this code mimics the state that a new cell received a mouse-down
				// event (see CGridCtrl::OnLButtonDown)

				ASSERT(CWnd::GetCapture() != this);
				SetCapture();

				// If Ctrl pressed, save the current cell selection. This will get added
				// to the new cell selection at the end of the cell selection process
				m_PrevSelectedCellMap.RemoveAll();
				if (nFlags & MK_CONTROL)
				{
					for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
					{
						DWORD key;
						CCellID cell;
						m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
						m_PrevSelectedCellMap.SetAt(key, cell);
					}
				}

				if (m_LeftClickDownCell.row < GetFixedRowCount())
					OnFixedRowClick(m_LeftClickDownCell);
				else if (m_LeftClickDownCell.col < GetFixedColumnCount())
					OnFixedColumnClick(m_LeftClickDownCell);
				else
				{
					m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
					OnSelecting(m_LeftClickDownCell);
				}

				ASSERT(m_nTimerID == 0);
				m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);

				m_LastMousePoint = point;
			}
		}
	}
	else if (this->m_MouseMode == MOUSE_SELECT_CELLS)
	{
		ASSERT(CWnd::GetCapture() == this); // captured in CGridCtrl::OnLButtonDown
		if (this->IsCheckMarkCell(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col))
		{
			CRect rc = this->GetCheckRect(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col);
			if (rc.PtInRect(point))
			{
				// kill timer if active
				// (disable dragging/autoscroll)
				if (this->m_nTimerID != 0)
				{
					this->KillTimer(this->m_nTimerID);
					this->m_nTimerID = 0;
				}
				ASSERT(CWnd::GetCapture() == this); // captured in CGridCtrl::OnLButtonDown
				TRACE("LButtonDown in Check 2\n");
				this->m_bButtonCaptured = TRUE;
				this->m_bButtonDown = TRUE;
				this->RedrawCell(this->m_LeftClickDownCell);
			}
		}
	}
}

void CModGridCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	if (this->m_MouseMode == MOUSE_NOTHING
		&&
		this->IsDropDownCell(this->m_idCurrentCell)
		&&
		this->m_LeftClickDownCell == this->m_idCurrentCell
		)
	{
		if (!this->IsCellEnabled(this->m_idCurrentCell)) return;

		const std::vector<CString>* pVec = this->GetOptionsVector(this->m_idCurrentCell);
		ASSERT(pVec);

		int nItemCount = (int)pVec->size();
		CString item = this->GetItemText(this->m_idCurrentCell);
		int nCurrent = -1;
		for (int i = 0; i < nItemCount; ++i)
		{
			if (item.Compare(pVec->at(i)) == 0)
			{
				nCurrent = i;
				break;
			}
		}
		int nNext = (nCurrent + 1) % nItemCount;
		// Note: OnEndEditCell doesn't check if cell is editable
		this->OnEndEditCell(this->m_idCurrentCell.row, this->m_idCurrentCell.col, pVec->at(nNext));
		this->InvalidateCellRect(this->m_idCurrentCell);
		this->SendMessageToParent(this->m_idCurrentCell.row, this->m_idCurrentCell.col, GVN_ENDLABELEDIT);
	}
	else
	{
		// Note: this->m_LeftClickDownCell could probably work here
		// since by the time this message is received an LButtonDown
		// and an LButtonUp have already been processed
		//
		CCellID dblClkCell = this->GetCellFromPt(point);
		if (this->IsCheckMarkCell(dblClkCell))
		{
			if (!this->m_idCurrentCell.IsValid()) return;
			if (!this->IsCellEnabled(this->m_idCurrentCell)) return;

			CRect rc = this->GetCheckRect(dblClkCell.row, dblClkCell.col);
			if (rc.PtInRect(point))
			{
				TRACE("LButtonDblClk in Check\n");
				switch (this->GetCheck(dblClkCell))
				{
				case BST_CHECKED:
					this->SetCheck(dblClkCell, BST_UNCHECKED);
					this->SetModified(TRUE, dblClkCell.row, dblClkCell.col);
					this->SendMessageToParent(dblClkCell.row, dblClkCell.col, GVN_CHECKCHANGED);
					this->RedrawCell(dblClkCell);
					break;
				case BST_UNCHECKED:
					this->SetCheck(dblClkCell, BST_CHECKED);
					this->SetModified(TRUE, dblClkCell.row, dblClkCell.col);
					this->SendMessageToParent(dblClkCell.row, dblClkCell.col, GVN_CHECKCHANGED);
					this->RedrawCell(dblClkCell);
					break;
				case BST_INDETERMINATE:
					break;
				default:
					ASSERT(FALSE);
				}
				return;
			}
			return; // don't start edit
		}
		CModGridCtrl::OnLButtonDblClk(nFlags, point);
	}
}

void CModGridCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	if (this->m_bButtonCaptured)
	{
		ASSERT(CWnd::GetCapture() == this);
		if (this->IsDropDownCell(this->m_idCurrentCell))
		{
			BOOL bLastButtonDown = this->m_bButtonDown;
			this->m_bButtonDown = this->m_rcButton.PtInRect(point);
			if (nFlags & MK_LBUTTON && this->m_rcListBox.PtInRect(point))
			{
				// transfer capture to listctrl
				this->m_bButtonDown = FALSE;
				this->m_bButtonCaptured = FALSE;
				if (CWnd::GetCapture() == this)
				{
					::ReleaseCapture();
				}
			}
			if (bLastButtonDown != this->m_bButtonDown)
			{
				this->RedrawCell(this->m_idCurrentCell);
			}
		}
		else if (this->IsCheckMarkCell(this->m_LeftClickDownCell))
		{
			this->RedrawCell(this->m_LeftClickDownCell);
		}
	}
	else
	{
		ASSERT(this->m_bButtonDown == FALSE);
		this->m_bButtonDown = FALSE;
#ifdef XP_STYLE
		if (this->s_themeButton || this->s_themeCombo)
		{
			if (this->IsValid(this->m_idLastHotCell))
			{
				this->RedrawCell(this->m_idLastHotCell);
				this->m_idLastHotCell.col = this->m_idLastHotCell.row = -1;
			}
			CCellID idHotCell = this->GetCellFromPt(point);
			if (this->IsCheckMarkCell(idHotCell))
			{
				this->RedrawCell(idHotCell);
				this->m_idLastHotCell = idHotCell;
			}
			if (idHotCell == this->m_idCurrentCell)
			{
				if (this->IsDropDownCell(idHotCell))
				{
					this->RedrawCell(idHotCell);
					this->m_idLastHotCell = idHotCell;
				}
			}
		}
#endif /* XP_STYLE */
		CModGridCtrl::OnMouseMove(nFlags, point);
	}
}

void CModGridCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	if (this->m_bButtonCaptured)
	{
		//// ASSERT(this->IsCheckMarkCell(this->m_idCurrentCell) ^ this->IsDropDownCell(this->m_idCurrentCell));

		ASSERT(CWnd::GetCapture() == this);
		::ReleaseCapture();
		this->m_bButtonCaptured = FALSE;
		this->m_bButtonDown = FALSE;
		this->RedrawCell(this->m_idCurrentCell);

		if (this->IsDropDownCell(this->m_idCurrentCell))
		{
			CListBox* pListBox = this->GetListBox(this->m_idCurrentCell);
			if (pListBox && pListBox->IsWindowVisible())
			{
				pListBox->SendMessage(LBDM_SETCAPTURE, 0, 0);
			}
		}
		else if (this->IsCheckMarkCell(this->m_LeftClickDownCell))
		{
			CRect rc = this->GetCheckRect(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col);
			if (rc.PtInRect(point))
			{
				TRACE("LButtonUp in Check\n");
				switch (this->GetCheck(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col))
				{
				case BST_CHECKED:
					this->SetCheck(this->m_LeftClickDownCell, BST_UNCHECKED);
					this->SetModified(TRUE, this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col);
					this->SendMessageToParent(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col, GVN_CHECKCHANGED);
					this->RedrawCell(this->m_LeftClickDownCell);
					break;
				case BST_UNCHECKED:
					this->SetCheck(this->m_LeftClickDownCell, BST_CHECKED);
					this->SetModified(TRUE, this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col);
					this->SendMessageToParent(this->m_LeftClickDownCell.row, this->m_LeftClickDownCell.col, GVN_CHECKCHANGED);
					this->RedrawCell(this->m_LeftClickDownCell);
					break;
				case BST_INDETERMINATE:
					break;
				default:
					ASSERT(FALSE);
				}
			}
		}
		CModGridCtrl::OnLButtonUp(nFlags, point);  // req'd to send GVN_SELCHANGED
	}
	else
	{
#if defined(_DEBUG)
		if (this->m_MouseMode == MOUSE_SIZING_COL)
		{
			if (m_LeftClickDownPoint != point)
			{
				CPoint start;
				if (this->GetCellOrigin(m_LeftClickDownCell, &start))
				{
					TRACE("SetColumnWidth(%d, %d)\n", m_LeftClickDownCell.col, point.x - start.x);
				}
			}
		}
		if (this->m_MouseMode == MOUSE_SIZING_ROW)
		{
			if (m_LeftClickDownPoint != point)
			{
				CPoint start;
				if (this->GetCellOrigin(m_LeftClickDownCell, &start))
				{
					TRACE("SetRowHeight(%d, %d)\n", m_LeftClickDownCell.row, point.y - start.y);
				}
			}
		}
#endif
		CModGridCtrl::OnLButtonUp(nFlags, point);
	}

	this->m_bButtonDown = FALSE;
}

void CModGridCtrlEx::ShowDropDown(BOOL bShowIt)
{
	if (bShowIt)
	{
		CRect rc;
		if (!this->GetCellRect(this->m_idCurrentCell, &rc)) return;

		if (CListBox *pListBox = this->GetListBox(this->m_idCurrentCell))
		{
			// determine size of dropdown list
			this->MapWindowPoints(CWnd::GetDesktopWindow(), &rc);
			rc.OffsetRect(0, rc.Height());

			int x, y, cx, cy;
			cx = rc.Width();
			x = rc.left;
			y = rc.top;

			pListBox->GetCurSel();
			pListBox->SetTopIndex(0);
			cy = pListBox->GetItemHeight(0) * pListBox->GetCount() + 2;

			this->m_rcListBox.SetRect(x, y, x+cx, y+cy);
			this->ScreenToClient(&this->m_rcListBox);

			// place in front of all windows
			pListBox->SetWindowPos(&wndTopMost, x, y, cx, cy, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
	}
	else
	{
		// hide DropDown
		if (CListBox *pListBox = this->GetListBox(this->m_idCurrentCell))
		{
			pListBox->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_HIDEWINDOW);
		}
	}
}

void CModGridCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Add your message handler code here and/or call default
	CListBox* pListBox = this->GetListBox(this->m_idCurrentCell);
	if (pListBox && pListBox->IsWindowVisible())
	{
		pListBox->SendMessage(WM_KEYDOWN, nChar, MAKEWORD(nRepCnt, nFlags));
		return;
		//switch (nChar) {
		//	case VK_PRIOR: // Page Down
		//	case VK_NEXT:  // Page Up
		//	case VK_END:
		//	case VK_HOME:
		//	case VK_LEFT:
		//	case VK_UP:
		//	case VK_RIGHT:
		//	case VK_DOWN:
		//		pListBox->SendMessage(WM_KEYDOWN, nChar, MAKEWORD(nRepCnt, nFlags));
		//		return;
		//		break;
		//}
	}
	CModGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CModGridCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	CListBox* pListBox = this->GetListBox(this->m_idCurrentCell);
	if (pListBox)
	{
		if (pListBox->IsWindowVisible())
		{
			// Esc
			if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
			{
				return pListBox->SendMessage(LBDM_ENDEDITLABELNOW, TRUE, 0);
			}

			// Enter
			if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
			{
				return pListBox->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
			}

			// F4
			if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4)
			{
				return pListBox->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
			}

			// Alt+Down
			if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_DOWN)
			{
				return pListBox->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
			}
		}
		else
		{
			// F4
			if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4)
			{
				this->OnLButtonDown(0, this->m_rcButton.CenterPoint());
				this->OnLButtonUp(0, this->m_rcButton.CenterPoint());
				return TRUE;
			}

			// Alt+Down
			if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_DOWN)
			{
				this->OnLButtonDown(0, this->m_rcButton.CenterPoint());
				this->OnLButtonUp(0, this->m_rcButton.CenterPoint());
				return TRUE;
			}
		}
	}
	//{{
	else
	{
		// F2
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2)
		{
			this->OnEditCell(this->m_idCurrentCell.row, this->m_idCurrentCell.col, VK_LBUTTON);
			return TRUE;
		}
	}
	//}}
	return CModGridCtrl::PreTranslateMessage(pMsg);
}

void CModGridCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//
	// Option list handling
	//
	CListBox* pListBox = this->GetListBox(this->m_idCurrentCell);
	if (pListBox)
	{
		if (pListBox->IsWindowVisible())
		{
			LRESULT lresult = pListBox->SendMessage(WM_CHAR, nChar, MAKEWORD(nRepCnt, nFlags));
			UNREFERENCED_PARAMETER(lresult);
			return;
		}
		else
		{
			if (!IsCTRLpressed() && this->m_MouseMode == MOUSE_NOTHING)
			{
				if (!this->m_bHandleTabKey || (this->m_bHandleTabKey && nChar != VK_TAB))
				{
					int nItemCount = pListBox->GetCount();
					int nItem = -1;
					CString str;
					for (int i = 0; i < nItemCount; ++i)
					{
						pListBox->GetText(i, str);
						UINT nFirstChar = str[0];
						if (::toupper(nFirstChar) == ::toupper(nChar))
						{
							nItem = i;
							break;
						}
					}
					if (nItem > -1)
					{
						this->OnEndEditCell(this->m_idCurrentCell.row, this->m_idCurrentCell.col, str);
						this->InvalidateCellRect(this->m_idCurrentCell);
						this->SendMessageToParent(this->m_idCurrentCell.row, this->m_idCurrentCell.col, GVN_ENDLABELEDIT);
					}
					CWnd::OnChar(nChar, nRepCnt, nFlags);
					return;
				}
			}
		}
	}

	//
	// Checkmark handling
	//
	if (this->IsCheckMarkCell(this->m_idCurrentCell) && nChar == _T(' '))
	{
		TRACE("CModGridCtrlEx::OnChar in Check\n");
		switch (this->GetCheck(this->m_idCurrentCell))
		{
		case BST_CHECKED:
			this->SetCheck(this->m_idCurrentCell, BST_UNCHECKED);
			this->SetModified(TRUE, this->m_idCurrentCell.row, this->m_idCurrentCell.col);
			this->SendMessageToParent(this->m_idCurrentCell.row, this->m_idCurrentCell.col, GVN_CHECKCHANGED);
			this->RedrawCell(this->m_idCurrentCell);
			break;
		case BST_UNCHECKED:
			this->SetCheck(this->m_idCurrentCell, BST_CHECKED);
			this->SetModified(TRUE, this->m_idCurrentCell.row, this->m_idCurrentCell.col);
			this->SendMessageToParent(this->m_idCurrentCell.row, this->m_idCurrentCell.col, GVN_CHECKCHANGED);
			this->RedrawCell(this->m_idCurrentCell);
			break;
		case BST_INDETERMINATE:
			break;
		default:
			ASSERT(FALSE);
		}
	}

	if (this->IsCheckMarkCell(this->m_idCurrentCell)) return;

	CModGridCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CModGridCtrlEx::DrawButton2(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, BOOL bDown)
{
	if (!this->IsCellEnabled(nRow, nCol)) return;

	UNREFERENCED_PARAMETER(bEraseBk);
	const int nButtonWidth = ::GetSystemMetrics(SM_CXVSCROLL);

	const CPoint& ptPos = rcCell.TopLeft();
	if (bDown)
	{
		CRect rc(ptPos, CSize(nButtonWidth, this->GetRowHeight(nRow)));
		rc.OffsetRect(max(this->GetColumnWidth(nCol) - nButtonWidth, 0), 0);
		rc.DeflateRect(-1, 1, 1, 1);
		this->m_rcButton = rc;
#ifdef XP_STYLE
		if (this->s_themeCombo)
		{
			rc.DeflateRect(0, 0, 0, 1);
			g_xpStyle.DrawThemeBackground(this->s_themeCombo, *pDC, CP_DROPDOWNBUTTON, CBXS_PRESSED, &rc, NULL);
		}
		else
#endif /* XP_STYLE */
		{
			pDC->DrawFrameControl(&rc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_PUSHED);
		}
		return;
	}
	else
	{
		CRect rc(ptPos, CSize(nButtonWidth, this->GetRowHeight(nRow)));
		rc.OffsetRect(max(this->GetColumnWidth(nCol) - nButtonWidth, 0), 0);
		rc.DeflateRect(-1, 1, 1, 1);
		this->m_rcButton = rc;
#ifdef XP_STYLE
		if (this->s_themeCombo)
		{
			rc.DeflateRect(0, 0, 0, 1);
			CPoint point;
			::GetCursorPos(&point);
			this->ScreenToClient(&point);
			if (rc.PtInRect(point))
			{
				g_xpStyle.DrawThemeBackground(this->s_themeCombo, *pDC, CP_DROPDOWNBUTTON, CBXS_HOT, &rc, NULL);
				return;
			}
			else
			{
				g_xpStyle.DrawThemeBackground(this->s_themeCombo, *pDC, CP_DROPDOWNBUTTON, CBXS_NORMAL, &rc, NULL);
			}
		}
		else
#endif /* XP_STYLE */
		{
			pDC->DrawFrameControl(&rc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
		}
	}
}

void CModGridCtrlEx::DrawButtonCheck(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, int nCheck, BOOL bEnabled)
{
	UNREFERENCED_PARAMETER(bEraseBk);

	// State of the check box: 0 for clear, 1 for checked, and 2 for indeterminate.
	UINT uState;
	switch (nCheck)
	{
	case BST_UNCHECKED:
		if (bEnabled) uState = DFCS_BUTTONCHECK | DFCS_FLAT;
		else          uState = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_INACTIVE;
		break;
	case BST_CHECKED:
		if (bEnabled) uState = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_CHECKED;
		else          uState = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_CHECKED | DFCS_INACTIVE;
		break;
	case BST_INDETERMINATE:
		uState = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_INACTIVE;
		break;
	default:
		uState = DFCS_BUTTONCHECK | DFCS_FLAT;
		break;
	}

	CRect rc = this->GetCheckRect(nRow, nCol);
	CPoint point;
	::GetCursorPos(&point);
	this->ScreenToClient(&point);

#ifdef XP_STYLE
	if (this->s_themeButton)
	{
		int iStateId;
		bool bHot = (rc.PtInRect(point) == TRUE);
		if (bHot && this->m_bButtonDown)
		{
			switch (nCheck)
			{
			case BST_UNCHECKED:
				iStateId = CBS_UNCHECKEDPRESSED;
				break;
			case BST_CHECKED:
				iStateId = CBS_CHECKEDPRESSED;
				break;
			case BST_INDETERMINATE:
				iStateId = CBS_UNCHECKEDDISABLED;
				break;
			default:
				ASSERT(FALSE);
				iStateId = CBS_UNCHECKEDPRESSED;
				break;
			}
			g_xpStyle.DrawThemeBackground(this->s_themeButton, *pDC, BP_CHECKBOX, iStateId, &rc, NULL);
			return;
		}
		if (!bHot && this->m_bButtonDown)
		{
			if (this->m_LeftClickDownCell.row == nRow && this->m_LeftClickDownCell.col == nCol)
			{
				//
				bHot = true;
			}
		}
		switch (nCheck)
		{
		case BST_UNCHECKED:
			iStateId = bHot ? CBS_UNCHECKEDHOT : CBS_UNCHECKEDNORMAL;
			break;
		case BST_CHECKED:
			iStateId = bHot ? CBS_CHECKEDHOT : CBS_CHECKEDNORMAL;
			break;
		case BST_INDETERMINATE:
			iStateId = CBS_UNCHECKEDDISABLED;
			break;
		default:
			iStateId = bHot ? CBS_UNCHECKEDHOT : CBS_UNCHECKEDNORMAL;
			break;
		}
		g_xpStyle.DrawThemeBackground(this->s_themeButton, *pDC, BP_CHECKBOX, iStateId, &rc, NULL);
	}
	else
#endif /* XP_STYLE */
	{
		if (this->m_bButtonDown && this->m_LeftClickDownCell.row == nRow && this->m_LeftClickDownCell.col == nCol)
		{
			if (rc.PtInRect(point))
			{
				uState |= DFCS_INACTIVE;
			}
		}
		pDC->DrawFrameControl(&rc, DFC_BUTTON, uState);
	}
	return;
}

CRect CModGridCtrlEx::GetCheckRect(int nRow, int nCol)const
{
	CRect rcCell;
	if (this->GetCellRect(nRow, nCol, &rcCell))
	{
		const int nCheckWidth = ::GetSystemMetrics(SM_CXMENUCHECK);

		CRect rc(rcCell.TopLeft(), CSize(nCheckWidth, this->GetRowHeight(nRow)));
		rc.OffsetRect(max(this->GetColumnWidth(nCol) - nCheckWidth, 0) / 2, 0);
		rc.DeflateRect(0, 1, 0, 1);
		return rc;
	}
	return CRect(0, 0, 0, 0);
}

BOOL CModGridCtrlEx::SetColumnCheck(int nCol, int nState)
{
	for (int i = this->GetFixedRowCount(); i < this->GetRowCount(); ++i)
	{
		this->SetCheck(i, nCol, nState);
	}
	return TRUE;
}

void CModGridCtrlEx::TraceMouseMode(void)const
{
	switch (this->m_MouseMode)
	{
	case MOUSE_NOTHING:
		TRACE("MOUSE_NOTHING\n");
		break;
	case MOUSE_SELECT_ALL:
		TRACE("MOUSE_SELECT_ALL\n");
		break;
	case MOUSE_SELECT_COL:
		TRACE("MOUSE_SELECT_COL\n");
		break;
	case MOUSE_SELECT_ROW:
		TRACE("MOUSE_SELECT_ROW\n");
		break;
	case MOUSE_SELECT_CELLS:
		TRACE("MOUSE_SELECT_CELLS\n");
		break;
	case MOUSE_SCROLLING_CELLS:
		TRACE("MOUSE_SCROLLING_CELLS\n");
		break;
	case MOUSE_OVER_ROW_DIVIDE:
		TRACE("MOUSE_OVER_ROW_DIVIDE\n");
		break;
	case MOUSE_SIZING_ROW:
		TRACE("MOUSE_SIZING_ROW\n");
		break;
	case MOUSE_OVER_COL_DIVIDE:
		TRACE("MOUSE_OVER_COL_DIVIDE\n");
		break;
	case MOUSE_SIZING_COL:
		TRACE("MOUSE_SIZING_COL\n");
		break;
	case MOUSE_PREPARE_EDIT:
		TRACE("MOUSE_PREPARE_EDIT\n");
		break;
	case MOUSE_PREPARE_DRAG:
		TRACE("MOUSE_PREPARE_DRAG\n");
		break;
	case MOUSE_DRAGGING:
		TRACE("MOUSE_DRAGGING\n");
		break;
	default:
		TRACE("Unknown mouse state\n");
		break;
	}
}

#ifdef XP_STYLE
LRESULT CModGridCtrlEx::OnThemeChanged(void)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// Add your message handler code here and/or call default
	this->s_themeButton.OnThemeChanged(L"BUTTON");
	this->s_themeCombo.OnThemeChanged(L"COMBOBOX");

	return 1;
}
#endif /* XP_STYLE */

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CModGridCtrlEx::CutSelectedText(void)
{
    if (!IsEditable())
        return;

    // Clear contents of selected cells.
    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        if (!IsCellEditable(cell))
		{
// COMMENT: {6/16/2009 10:20:08 PM}			if (!this->IsDropDownCell(cell))
			{
				continue;
			}
		}

        CGridCell* pCell = GetCell(cell.row, cell.col);
        if (pCell)
        {
		    SendMessageToParent(cell.row, cell.col, GVN_BEGINLABELEDIT);
			SetItemText(cell.row, cell.col, _T(""));
            SetModified(TRUE, cell.row, cell.col);
		    SendMessageToParent(cell.row, cell.col, GVN_ENDLABELEDIT);
			RedrawCell(cell);
        }
    }
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
BOOL CModGridCtrlEx::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPTSTR szBuffer = new TCHAR[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    CString strLine = strText;
    int nLine = 0;

#ifdef _DEBUG
	std::vector< std::vector< CString > > vvRows;
#endif
    // Find the end of the first line
	CCellRange PasteRange(cell.row, cell.col,-1,-1);
    int nIndex;
    do
    {
#ifdef _DEBUG
		std::vector< CString > vRow;
#endif
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        CString strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex + 1);

        // Remove all chars after the newline
        if (nIndex >= 0)
            strLine = strLine.Left(nIndex);

        int nLineIndex = strLine.FindOneOf(_T("\t,"));
        CString strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

        // skip hidden rows
        int iRowVis = cell.row + nLine;
        while( iRowVis < GetRowCount())
        {
            if( GetRowHeight( iRowVis) > 0)
                break;
            nLine++;
            iRowVis++;
        }

        while (!strLine.IsEmpty())
        {
            // skip hidden columns
            int iColVis = cell.col + nColumn;
            while( iColVis < GetColumnCount())
            {
                if( GetColumnWidth( iColVis) > 0)
                    break;
                nColumn++;
                iColVis++;
            }

            CCellID TargetCell(iRowVis, iColVis);
            if (IsValid(TargetCell))
            {
                strCellText.TrimLeft();
                strCellText.TrimRight();
#ifdef _DEBUG
				vRow.push_back(strCellText);
#endif

                ValidateAndModifyCellContents(TargetCell.row, TargetCell.col, strCellText);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                    GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);

				if (iRowVis > PasteRange.GetMaxRow()) PasteRange.SetMaxRow(iRowVis);
				if (iColVis > PasteRange.GetMaxCol()) PasteRange.SetMaxCol(iColVis);
            }

            strLine = (nLineIndex >= 0)? strLine.Mid(nLineIndex + 1) : _T("");
            nLineIndex = strLine.FindOneOf(_T("\t,"));
            strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

            nColumn++;
        }

        strLine = strNext;
        nLine++;
#ifdef _DEBUG
		vvRows.push_back(vRow);
#endif
    } while (nIndex >= 0);

    strText.UnlockBuffer();

// COMMENT: {4/29/2005 3:56:53 PM}	if (bSelectPastedCells)
		SetSelectedRange(PasteRange, TRUE);
// COMMENT: {4/29/2005 3:56:56 PM}	else
// COMMENT: {4/29/2005 3:56:56 PM}	{
// COMMENT: {4/29/2005 3:56:56 PM}		ResetSelectedRange();
// COMMENT: {4/29/2005 3:56:56 PM}		Refresh();
// COMMENT: {4/29/2005 3:56:56 PM}	}
#ifdef _DEBUG
	// dump paste cells
	//vvsCols[(info.item.iRow - ptUpperLeft.x) % sizeSrc.cx][(info.item.iCol - ptUpperLeft.y) % sizeSrc.cy]
	std::vector<CString>::size_type row, col;
	for (row = 0; row < vvRows.size(); ++row)
	{
		TRACE("Row %d\n", row);
		for (col = 0; col < vvRows[row].size(); ++col)
		{
			TRACE("%s ", vvRows[row][col]);
		}
		TRACE("\n");
	}
#endif

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
COleDataSource* CModGridCtrlEx::CopyTextFromGrid()
{
    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection)) return NULL;

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCell *pCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
			if (this->IsCheckMarkCell(row, col))
			{
				// may want to change this so that szText contains
				// FALSE or TRUE but DrawCell doesn't display it
				//
				switch (this->GetCheck(row, col))
				{
				case BST_CHECKED:
					str += _T("TRUE");
					break;
				case BST_UNCHECKED:
					str += _T("FALSE");
					break;
				default:
					str += _T(" ");
					break;
				}
			}
			else
			{
				pCell = GetCell(row,col);
				if (pCell && (pCell->state  & GVIS_SELECTED))
				{
					if (pCell->szText.IsEmpty())
						str += _T(" ");
					else
						str += pCell->szText;
				}
			}
            if (col != Selection.GetMaxCol())
                str += _T("\t");
        }
        if (row != Selection.GetMaxRow())
            str += _T("\n");

        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }

    char c = '\0';
    sf.Write(&c, 1);

    DWORD dwLen = (DWORD)sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem)
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    if (!hMem)
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}
#endif


void CModGridCtrlEx::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
    if (!IsCellEditable(nRow, nCol))
        return;

    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
		if (this->IsCheckMarkCell(nRow, nCol))
		{
			// yes, no, false, true
			CString strValue(strText);
			strValue.MakeUpper();

			if (strValue.Compare("TRUE") == 0 || strValue.Compare("YES") == 0)
			{
				this->SetCheck(nRow, nCol, BST_CHECKED);
			}
			else if (strValue.Compare("FALSE") == 0 || strValue.Compare("NO") == 0)
			{
				this->SetCheck(nRow, nCol, BST_UNCHECKED);
			}
			return;
		}

		if (this->IsDropDownCell(nRow, nCol))
		{
			const std::vector<CString>* pVec = this->GetOptionsVector(nRow, nCol);
			ASSERT(pVec);
			if (pVec)
			{
				int nItemCount = (int)pVec->size();
				CString item = strText;
				int nCurrent = -1;
				for (int i = 0; i < nItemCount; ++i)
				{
					if (item.Compare(pVec->at(i)) == 0)
					{
						nCurrent = i;
						break;
					}
				}
				if (nCurrent == -1) return;
			}
		}

		CString strCurrentText = GetItemText(nRow, nCol);
        if (strCurrentText != strText)
        {
            SetItemText(nRow, nCol, strText);
            if (ValidateEdit(nRow, nCol, strText) &&
                SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
            {
                SetModified(TRUE, nRow, nCol);
                RedrawCell(nRow, nCol);
            }
            else
            {
                SetItemText(nRow, nCol, strCurrentText);
            }
        }
    }
}

// If this returns FALSE then the editing isn't allowed
// virtual
BOOL CModGridCtrlEx::ValidateEdit(int nRow, int nCol, LPCTSTR str)
{
	return TRUE;
}

std::map< std::vector<CString>, CListBox* >::const_iterator CModGridCtrlEx::FindOrCreateListBox(const std::vector<CString>& vecOptions, LPCTSTR lpszWindowName)
{
	CString strWindowName;
#ifdef _DEBUG
	if (lpszWindowName != NULL)
	{
		strWindowName.Format(_T("%s"), lpszWindowName);
	}
#endif

	std::map< std::vector<CString>, CListBox* >::const_iterator item = this->m_mapVectorToList.find(vecOptions);
	if (item == this->m_mapVectorToList.end())
	{
		// this insert is ok
		item = (this->m_mapVectorToList.insert(std::map< std::vector<CString>, CListBox* >::value_type(vecOptions, new CListBoxDrop()))).first;

		ASSERT(item == this->m_mapVectorToList.find(vecOptions));
		ASSERT(item != this->m_mapVectorToList.end());
		ASSERT(item->first.size() == vecOptions.size());
		ASSERT(&item->first != &vecOptions);

		CListBox* pListBox = item->second;

		CRect rect(0, 0, 0, 0);
		VERIFY(pListBox->CreateEx(
			WS_EX_TOPMOST|WS_EX_TOOLWINDOW,
			_T("LISTBOX"),
			strWindowName,
			WS_CHILDWINDOW|WS_CLIPSIBLINGS|WS_BORDER,
			rect,
			CWnd::GetDesktopWindow(),
			IDC_INPLACE_CONTROL,
			NULL
			));

		pListBox->SetFont(&this->m_Font);
		pListBox->SetOwner(this);

		ASSERT(this->m_mapVectorToList[vecOptions]->GetOwner() == this);
		ASSERT(this->m_mapVectorToList[vecOptions]->GetParent() == CWnd::GetDesktopWindow());

		std::vector<CString>::const_iterator opt = vecOptions.begin();
		for (; opt != vecOptions.end(); ++opt)
		{
			pListBox->AddString(*opt);
		}
		ASSERT(pListBox->GetCount() == (int)vecOptions.size());
	}
#ifdef _DEBUG
	else
	{
		if (lpszWindowName != NULL)
		{
			CString str;
			CListBox *pListBox = item->second;
			pListBox->GetWindowText(str);
			strWindowName.Format(_T("%s, %s"), str, lpszWindowName);
			pListBox->SetWindowText(strWindowName);
		}
	}
#endif
	return item;
}

//
// Modified: SetFocusCell is not called therefore fixing bug that sometimes caused
// the focus cell to move when the mouse was held down on a fixed row or col
// Note: this is mostly just a combined CGridCtrl::OnTimer and CGridCtrl::OnKeyDown
//
void CModGridCtrlEx::OnTimer(UINT nIDEvent)
{
    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN)
        return;

    CPoint pt, origPt;

#ifdef _WIN32_WCE
    if (m_MouseMode == MOUSE_NOTHING)
        return;
    origPt = GetMessagePos();
#else
    if (!GetCursorPos(&origPt))
        return;
#endif

    ScreenToClient(&origPt);

    CRect rect;
    GetClientRect(rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColumnWidth();

    pt = origPt;
    if (pt.y > rect.bottom)
    {
        if (m_idCurrentCell.row < (GetRowCount() - 1))
		{
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		}

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        if (m_idCurrentCell.row > m_nFixedRows)
		{
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		}

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        if (m_idCurrentCell.col < (GetColumnCount() - 1))
		{
			SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
		}

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.x < nFixedColWidth)
    {
        if (m_idCurrentCell.col > m_nFixedCols)
		{
			SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
		}

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }
}
