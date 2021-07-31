// ----------------------------------------------------------------------------
//  File : UIHelp.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIHELPOLD_H_
#define	UIHELPOLD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UIMultiEditBox.h>
#include <string>
#include <vector>

// Section of web board
enum UIHelpSection
{
	HELPS_HELP		= 0,	
	HELPS_TOTAL,
};

// Type of web board
enum UIHelpType
{
	HELPT_LIST		= 0,
	HELPT_READ,	
};

#define	HELPOLD_TAB_WIDTH					95

// Count
#define	HELPOLD_LIST_SUBJECT_MAX_CHAR		60
#define	HELPOLD_MAX_LIST					10
#define	HELPOLD_READ_MAX_CHAR				88

// Define text position
#define	HELPOLD_TITLE_OFFSETX				25
#define	HELPOLD_TITLE_OFFSETY				5
#define	HELPOLD_LIST_TITLE_SY				39
#define	HELPOLD_LIST_TITLE_NO_SX			46
#define	HELPOLD_LIST_TITLE_SUBJECT_SX		187
#define	HELPOLD_LIST_TITLE_NAME_SX			365
#define	HELPOLD_LIST_MAIN_SY				83
#define	HELPOLD_LIST_MAIN_OFFSETY			29
#define	HELPOLD_LIST_MAIN_NO_CX			36
#define	HELPOLD_LIST_MAIN_SUBJECT_SX		69
#define	HELPOLD_LIST_MAIN_NAME_CX			368
#define	HELPOLD_LIST_MAIN_NOTICE_NAME_CX	419
#define	HELPOLD_READ_SUBJECT_SY			67
#define	HELPOLD_READ_NO_SX					35
#define	HELPOLD_READ_SUBJECT_SX			96
#define	HELPOLD_READ_NAME_RX				561
#define	HELPOLD_READ_CONTENT_SX			36
#define	HELPOLD_READ_CONTENT_SY			141

// Splitter
#define	HELPOLD_SPLIT_WIDTH					551
#define	HELPOLD_LIST_SPLIT_SX				27
#define	HELPOLD_LIST_SPLITS_SY				104 
#define	HELPOLD_LIST_SPLITL_SY				366
#define	HELPOLD_READ_SPLIT_SX				25
#define	HELPOLD_READ_SPLITS_SY				93
#define	HELPOLD_READ_SPLITL_SY				384

// Buttons
#define	HELPOLD_PAGE_BUTTON_SY				373

// Define size of web board
#define	HELPOLD_WIDTH						600
#define	HELPOLD_HEIGHT						450

// ----------------------------------------------------------------------------
// Name : CUIHelp
// Desc :
// ----------------------------------------------------------------------------
class CUIHelpOld : public CUIWindow
{
public:
	enum	eHelpType
	{
		HELP_IMAGE	= 0,
		HELP_TEXT	= 1,
		HELP_LIST	= 2,
	};

protected:
	struct sPageItem
	{
		sPageItem()
		{
			eType	= HELP_TEXT;
			strText	= "";
			crText	= 0xFFFFFFFF;
			nX		= 0;
			nY		= 0;
			nWidth	= 0;
			nHeight	= 0;
		}

		eHelpType	eType;
		int			nX;
		int			nY;
		int			nWidth;
		int			nHeight;

		CTString	strText;
		COLOR		crText;
		CUIListBox	lbListBox;
		UIRectUV	rtImage;
	};

	struct sPage
	{
		sPage()
		{
			iIndex		= -1;
			strTitle	= "";
		}

		int						iIndex;
		CTString				strTitle;
		std::vector<sPageItem>	vectorPageItem;
	};

	// Function Object
	struct FindPage
	{
		int iPageIndex;
		FindPage(sPage& info) : iPageIndex(info.iIndex) {};
		bool operator() (const sPage& info)
		{
			if(info.iIndex == iPageIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

protected:	
	CTextureData		*m_ptdImageTexture;			// Texture of window

	BOOL				m_bWaitResponseMode;
	BOOL				m_bVisible;	
	
	int					m_nWantPage;						//CCommandList에서 사용
	int					m_nWantWrite;						//CCommandView에서 사용
	int					m_nCurSection;						// Section of web board
	int					m_nCurBoardType;					// Type of web board

	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnPrev;							// Prev button
	CUIButton			m_btnNext;							// Next button
	CUIButton			m_btnPrevContent;					// Prev Content button
	CUIButton			m_btnNextContent;					// Next Content button
	CUIButton			m_btnList;							// List button
	CUIButton			m_btnPage[10];						// Buttons of page

	int					m_nValidPageBtnCount;				// Count of pages
	int					m_nCurrentPage;						// Index of current page
	int					m_nCurPage;
	int					m_nCurrentFirstPage;				// First index of current page
	int					m_nCurrentLastPage;					// Last index of current page
	int					m_nTotalPage;						// Count of total page

	// List type
	std::vector<sPage>	m_vectorPages;
	CUIListBox			m_lbListContent;					// Listbox of list content ( col : 0-No, 1-Subject, 2-Writer, 3-Date, 4-Hit )
	int					m_nListIndex[HELPOLD_MAX_LIST];
	int					m_nListDepth[HELPOLD_MAX_LIST];

	// Read type
	CUIListBox			m_lbReadContent;					// Listbox of reading content
	CTString			m_strReadNo;
	CTString			m_strReadSubject;
	CTString			m_strReadName;

	// Region of each part
	UIRect				m_rcTitle;							// Region of title	
	UIRect				m_rcListTitle;						// Region of list title
	UIRect				m_rcListContent;					// Region of list content
	UIRect				m_rcReadSubject;					// Region of read subject
	UIRect				m_rcReadContent;					// Region of read content	

	// UV of each part
	UIRectUV			m_rtTitleUL;						// UV of upper left background
	UIRectUV			m_rtTitleUM;						// UV of upper middle background
	UIRectUV			m_rtTitleUR;						// UV of upper right background

	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	UIRectUV			m_rtTab;							// UV of section tab
	UIRectUV			m_rtSplitterS;						// UV of small splitter
	UIRectUV			m_rtSplitterL;						// UV of large splitter
	UIRectUV			m_rtBackSub;						// UV of sub background	

	UIRectUV			m_rtEditBoxL;						// UV of left region of edit box
	UIRectUV			m_rtEditBoxM;						// UV of middle region of edit box
	UIRectUV			m_rtEditBoxR;						// UV of right region of edit box
	UIRectUV			m_rtEditBoxUL;						// UV of upper left region of edit box
	UIRectUV			m_rtEditBoxUM;						// UV of upper middle region of edit box
	UIRectUV			m_rtEditBoxUR;						// UV of upper right region of edit box
	UIRectUV			m_rtEditBoxML;						// UV of middle left region of edit box
	UIRectUV			m_rtEditBoxMM;						// UV of middle middle region of edit box
	UIRectUV			m_rtEditBoxMR;						// UV of middle right region of edit box
	UIRectUV			m_rtEditBoxLL;						// UV of lower left region of edit box
	UIRectUV			m_rtEditBoxLM;						// UV of lower middle region of edit box
	UIRectUV			m_rtEditBoxLR;						// UV of lower right region of edit box

protected:
	// Internal functions
	void	RenderListCommon();
	void	RenderReadCommon();	
	void	RenderPage();
	
	void	SetListContent( int iPage );
	void	SetViewContent( int iIndex );

	// 페이지 추가.
	int		AddPage( const CTString &strPageTitle );	

	// 페이지 아이템 추가.
	void	AddPageItem( int iPage, sPageItem& NewPageItem );

	void	SetPageButtons( int nTotalWrite, int nCurPage );

	void	AddListSubjectString( CTString &strSubject );
	void	AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar = HELPOLD_READ_MAX_CHAR );

public:
	CUIHelpOld();
	~CUIHelpOld();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );		
	}

	// Toggle visible of web board
	void	ToggleVisible();

	// Open & close web board
	void	OpenHelp();
	void	CloseHelp();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIBUTTON_H_

