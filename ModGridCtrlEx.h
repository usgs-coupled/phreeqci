#pragma once
#include "ModGridCtrl.h"

//#define GVN_ITEMCHANGED          LVN_ITEMCHANGED
#define GVN_CHECKCHANGED          (LVN_FIRST-109)

// checkmark states
#define GVIS_UNCHECKED            0x00100000
#define GVIS_CHECKED              0x00200000
#define GVIS_INDETERMINATE        0x00400000

// checkmark masks
#define GVIM_CHECKMARK            0x00700000 // (GVIS_UNCHECKED|GVIS_CHECKED|GVIS_INDETERMINATE)

// list states
#define GVIS_DROPDOWN             0x01000000


#include <map>
#include <vector>
#include "../VisualStylesXP/VisualStylesXP.h"

void AFXAPI DDX_GridCheck(CDataExchange* pDX, int nIDC, int nRow, int nCol, int& nState);

class CTheme
{
public:
	CTheme(void) : m_hTheme(0)
	{
	}
	~CTheme(void)
	{
		if (m_hTheme)
		{
			g_xpStyle.CloseThemeData(m_hTheme);
		}
	}
	void Create(LPCWSTR pszClassList)
	{
		if (!m_hTheme)
		{
			if (g_xpStyle.IsAppThemed())
			{
				m_hTheme = g_xpStyle.OpenThemeData(NULL, pszClassList);
			}
		}
	}
	operator HTHEME() const
	{
		return this == NULL ? NULL : m_hTheme;
	}
	void OnThemeChanged(LPCWSTR pszClassList)
	{
		if (m_hTheme)
		{
			g_xpStyle.CloseThemeData(m_hTheme);
			m_hTheme = 0;
		}
		if (g_xpStyle.IsAppThemed())
		{
			m_hTheme = g_xpStyle.OpenThemeData(NULL, pszClassList);
		}
	}
protected:
	HTHEME m_hTheme;
};


class CModGridCtrlEx : public CModGridCtrl
{
public:
	CModGridCtrlEx(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CModGridCtrlEx(void);

	// Methods
	BOOL SetCellOptions(int nRol, int nCol, const std::vector<LPCTSTR>& vecOptions);
	BOOL SetColumnOptions(int nCol, const std::vector<LPCTSTR>& vecOptions);
	bool IsDropDownCell(int nRow, int nCol)const;
	bool IsDropDownCell(const CCellID& cell)const;

	bool IsCheckMarkCell(int nRow, int nCol)const;
	bool IsCheckMarkCell(const CCellID& cell)const;
	int GetCheck(const CCellID& cell)const;
	int GetCheck(int nRow, int nCol)const;
	BOOL SetCheck(const CCellID& cell, int nState);
	BOOL SetCheck(int nRow, int nCol, int nState);
	BOOL SetColumnCheck(int nCol, int nState);
	CRect GetCheckRect(int nRow, int nCol)const;

	const std::vector<LPCTSTR>* GetOptionsVector(int nRow, int nCol)const;
	const std::vector<LPCTSTR>* GetOptionsVector(const CCellID& cell)const;
	CListBox* GetListBox(int nRow, int nCol);
	CListBox* GetListBox(const CCellID& cell);
    CString GetItemText(const CCellID& cell);
	CString GetItemText(int nRow, int nCol);

	void TraceMouseMode(void)const;

	void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);
	virtual BOOL ValidateEdit(int nRow, int nCol, LPCTSTR str);


protected:
	// Virtual overrides
    virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
	virtual void  DrawButton(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, BOOL bDown);
	virtual void  DrawButton2(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, BOOL bDown);
	virtual void  DrawButtonCheck(CDC* pDC, int nRow, int nCol, CRect rcCell, BOOL bEraseBk, int nCheck);	

#ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void CutSelectedText();
    virtual BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);
    virtual COleDataSource* CopyTextFromGrid();
#endif

	void ShowDropDown(BOOL bShowIt);

protected:
	BOOL                                 m_bButtonDown;
	BOOL                                 m_bButtonCaptured;
	CRect                                m_rcButton;
	CRect                                m_rcListBox;
	CCellID m_idLastHotCell;
	std::map< std::vector<LPCTSTR>, CListBox* > m_mapVectorToList;
	std::map< std::vector<LPCTSTR>, CListBox* >::const_iterator 
		FindOrCreateListBox(const std::vector<LPCTSTR>& vecOptions, LPCTSTR lpszWindowName = NULL);

	static CTheme s_themeButton;
	static CTheme s_themeCombo;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	LRESULT OnThemeChanged();
	afx_msg void OnTimer(UINT nIDEvent);
};
