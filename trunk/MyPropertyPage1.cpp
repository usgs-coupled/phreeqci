// MyPropertyPage1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MyPropertyPage1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage1, baseCMyPropertyPage1)
IMPLEMENT_DYNCREATE(CMyPropertyPage2, baseCMyPropertyPage2)


/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 property page

CMyPropertyPage1::CMyPropertyPage1() : baseCMyPropertyPage1(CMyPropertyPage1::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPropertyPage1::~CMyPropertyPage1()
{
}

void CMyPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	baseCMyPropertyPage1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage1, baseCMyPropertyPage1)
	//{{AFX_MSG_MAP(CMyPropertyPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 property page

CMyPropertyPage2::CMyPropertyPage2() : baseCMyPropertyPage2(CMyPropertyPage2::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPropertyPage2::~CMyPropertyPage2()
{
}

void CMyPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	baseCMyPropertyPage2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage2, baseCMyPropertyPage2)
	//{{AFX_MSG_MAP(CMyPropertyPage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CMyPropertyPage1::OnInitDialog() 
{
	baseCMyPropertyPage1::OnInitDialog();

	/*
	enum layResizeMode {
		GREEDY				= 0,		// Will eat up as much as it can
		ABSOLUTE_HORZ		= 1 << 0,	// Horizontal size is absolute
		RELATIVE_HORZ		= 1 << 1,	// Horizontal size in percent
		ABSOLUTE_VERT		= 1 << 2,	// Vertical size is absolute
		RELATIVE_VERT		= 1 << 3,	// Vertical size in percent

		NORESIZE			= ABSOLUTE_HORZ | ABSOLUTE_VERT,

		SIZE_MASK			= NORESIZE,

		ALIGN_LEFT			= 1 << 4,   // following only for NORESIZE
		ALIGN_RIGHT			= 1 << 5,
		ALIGN_TOP			= 1 << 6,
		ALIGN_BOTTOM		= 1 << 7,

		ALIGN_HCENTER		= ALIGN_LEFT    | ALIGN_RIGHT,	
		ALIGN_VCENTER		= ALIGN_TOP     | ALIGN_BOTTOM,

		ALIGN_CENTER		= ALIGN_HCENTER | ALIGN_VCENTER,

		ALIGN_FILL_HORZ		= 1 << 8,
		ALIGN_FILL_VERT		= 1 << 9,
		ALIGN_FILL			= ALIGN_FILL_HORZ | ALIGN_FILL_VERT,	
	};
	*/
	/*
	enum layOrientation {
		HORIZONTAL,
		VERTICAL
	};
	*/

	/**
	paneCtrl Notes:
		For a HORIZONTAL paneCtrl(GROUPBOX)
		
		if (layResizeMode == GREEDY)
		paneCtrl.nHeight = 2*sizeExtraBorder + sizeTopExtra + MAX(item.nHeight)
		

		if (layResizeMode == RELATIVE_VERT && sizeSecondary != 0)
		paneCtrl.nHeight = 2*sizeExtraBorder + sizeTopExtra + MAX(item.nHeight)

		if (layResizeMode == RELATIVE_VERT && sizeSecondary == 0)
		paneCtrl.nHeight = 2*sizeExtraBorder + sizeTopExtra + MAX(item.nHeight)

        if (layResizeMode == ABSOLUTE_VERT || layResizeMode == NORESIZE)
          if (sizeSecondary == 0)
            paneCtrl.nHeight = 2*sizeExtraBorder + sizeTopExtra + MAX(item.nHeight)
          else
            paneCtrl.nHeight = sizeSecondary
	*/

	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< ( paneCtrl(IDC_S_1,        // UINT nID
					  HORIZONTAL,     // enum layOrientation orientation
					  NORESIZE,  // enum layResizeMode modeResize      (default = GREEDY)
					  53,              // int sizeBorder      (default = nDefaultBorder = 5)
					  18,              // int sizeExtraBorder (default = 0)
					  26,              // int sizeTopExtra    (default = 0)
					  0               // int sizeSecondary   (default = 0)
					  )
			<< item(IDC_BUTTON1,   // UINT nID
					NORESIZE,      // enum layResizeMode modeResize (default = GREEDY)
					0,             // int sizeX                     (default = 0)
					0,             // int sizeY                     (default = 0)
					-1,            // int sizeXMin                  (default = -1)
					-1             // int sizeYMin                  (default = -1)
					)
			//{{
			// << itemGrowing(HORIZONTAL)
			// << itemSpaceBetween(HORIZONTAL, IDC_BUTTON1, IDC_BUTTON2)
			//}}
			<< item(IDC_BUTTON2,   // UINT nID
					NORESIZE,      // enum layResizeMode modeResize (default = GREEDY)
					0,             // int sizeX                     (default = 0)
					0,             // int sizeY                     (default = 0)
					-1,            // int sizeXMin                  (default = -1)
					-1             // int sizeYMin                  (default = -1)
					)
			
		)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMyPropertyPage2::OnInitDialog() 
{
	baseCMyPropertyPage2::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
