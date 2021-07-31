// ----------------------------------------------------------------------------
//  File : UIComboBox.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICOMBOBOX_H_
#define	UICOMBOBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIScrollBar.h>
#include <vector>


#define	CMB_DROPLIST_VERT_GAP		4


// ----------------------------------------------------------------------------
// Name : CUIComboBox
// Desc :
// ----------------------------------------------------------------------------
class CUIComboBox : public CUIWindow
{
protected:
	BOOL					m_bScrollBarExist;				// If scroll bar is exist or not
	BOOL					m_bShowScrollBar;				// If scroll bar is shown or not
	CUIScrollBar			m_sbScrollBar;					// Scroll bar
	BOOL					m_bDropList;					// If list is drop or not
	int						m_nSelectList;					// Selected list
	int						m_nOverList;					// Over list
	std::vector<CTString>	m_vecString;					// Strings
	int						m_nMaxLine;						// Maximum count of line
	int						m_nLineHeight;					// Height of one line
	int						m_nComboTextSY;					// Position y of text in combo box
	int						m_nTextSX;						// Position x of text in drop list
	int						m_nTextSY;						// Position y of text in drop list
	int						m_nSelectOffsetY;				// Offset y of selection region

	// Region of each part
	UIRect					m_rcDropList;
	UIRect					m_rcBtn;

	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background
	UIRectUV				m_rtUp;							// UV of up button
	UIRectUV				m_rtDown;		             	// UV of down button
	UIRectUV				m_rtDropListTop;		        // UV of list
	UIRectUV				m_rtDropListMiddle;		        // UV of list
	UIRectUV				m_rtDropListBottom;		        // UV of list

public:
	CUIComboBox();
	~CUIComboBox();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					int nBtnSX, int nBtnSY, int nBtnWidth, int nBtnHeight,
					int nMaxLine, int nLineHeight, int nSpaceX, int nSpaceY );
	void	CreateScroll( BOOL bRight, int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
							int nBarVertEdgeSize );

	// Render
	void	Render();

	// Selected index
	int		GetCurSel() const { return m_nSelectList; }
	void	SetCurSel( int nCurSelection )
	{
		if( nCurSelection < m_vecString.size() )
			m_nSelectList = nCurSelection;
	}
	// Get string vector
	std::vector<CTString> GetVecString() { return m_vecString; }

	// Strings
	void	AddString( CTString &strString );
	void	ResetStrings();

	// UV of combo box
	void	SetBackUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBackground.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetUpBtnUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUp.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDownBtnUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtDown.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDropListUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtDropListTop.SetUV( fTx0, fTy0, fTx1, fTy0 + CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight );
		m_rtDropListMiddle.SetUV( fTx0, fTy0 + CMB_DROPLIST_VERT_GAP, fTx1, fTy1 - CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight );
		m_rtDropListBottom.SetUV( fTx0, fTy1 - CMB_DROPLIST_VERT_GAP, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	// UV of scroll abr
	void	SetScrollUpUV( UIBtnState bsState,
							FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetUpUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyScrollUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_sbScrollBar.CopyUpUV( bsSrcState, bsDstState );
	}
	void	SetScrollBarTopUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarTopUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollBarMiddleUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarMiddleUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollBarBottomUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarBottomUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollDownUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetDownUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyScrollDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_sbScrollBar.CopyDownUV( bsSrcState, bsDstState );
	}

	int GetMaxLine() { return m_nMaxLine; }

	//wooss 050829
	BOOL IsDropList() {return m_bDropList;}
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetDropList(BOOL bDrop) { m_bDropList = bDrop; }
};


#endif	// UICOMBOBOX_H_

