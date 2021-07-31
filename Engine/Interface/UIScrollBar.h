// ----------------------------------------------------------------------------
//  File : UIScrollBar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISCROLLBAR_H_
#define	UISCROLLBAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>


// ----------------------------------------------------------------------------
// Name : CUIScrollBar
// Desc :
// ----------------------------------------------------------------------------
class CUIScrollBar : public CUIWindow
{
protected:
	BOOL			m_bVertical;				// Vertical scrolling
	int				m_nScrollPos;				// Position of scroll bar
	int				m_nScrollRange;				// Range of scroll bar	
	int				m_nCurItemCount;			// Current count of items
	int				m_nItemsPerPage;			// Count of items per one page
	CUIButton		m_btnUp;					// Up button of scroll bar
	CUIButton		m_btnDown;					// Down button of scroll bar
	BOOL			m_bScrollBarClick;			// If bar button is clicked or not
	int				m_nDragPos;					// Position x or y for dragging
	int				m_nDragPosOffset;			// Offset x or y for dragging
	int				m_nBarPosX, m_nBarPosY;		// Position of bar button
	int				m_nBarWidth, m_nBarHeight;	// Size of bar button
	int				m_nBarEdgeSize;				// Edge size of bar button

	// Region
	UIRect			m_rcScrolling;				// Region of scrolling
	UIRect			m_rcWheel;					// Region of wheel

	// UV
	UIRectUV		m_rtBarTop;					// UV of top
	UIRectUV		m_rcBarMiddle;				// UV of middle
	UIRectUV		m_rtBarBottom;				// UV of bottom

protected:
	// Internal functions
	void	MoveScrollPos( int ndPos );
	void	UpdateScrollBarSize();
	void	UpdateScrollBarPos();

public:
	CUIScrollBar();
	~CUIScrollBar();

	// Create buttons
	void	CreateButtons( BOOL bVerticalScroll, int nBtnWidth, int nBtnHeight,
							int nBarWidthGap, int nBarHeightGap, int nBarVertEdgeSize );

	// Render
	void	Render();

	// Seet wheel region
	void	SetWheelRect( int nLeft, int nTop, int nWidth, int nHeight )
	{
		m_rcWheel.SetRect( nLeft, nTop, nLeft + nWidth, nTop + nHeight );
	}

	// UV
	void	SetUpUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnUp.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnUp.CopyUV( bsSrcState, bsDstState );
	}
	void	SetBarTopUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBarTop.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetBarMiddleUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rcBarMiddle.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetBarBottomUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBarBottom.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDownUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnDown.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnDown.CopyUV( bsSrcState, bsDstState );
	}

	// Size
	void	ChangeSize( int ndSize );

	// Scroll information
	void	SetScrollPos( int nScrollPos )
	{
		m_nScrollPos = nScrollPos;
		UpdateScrollBarPos();
	}
	void	ChangeScrollPos( int ndPos )
	{
		MoveScrollPos( ndPos );
	}
	void	SetScrollRange( int nScrollRange )
	{
		m_nScrollRange = nScrollRange;
		UpdateScrollBarSize();
	}
	void	SetCurItemCount( int nCount )
	{
		m_nCurItemCount = nCount;
		UpdateScrollBarSize();
	}
	void	SetItemsPerPage( int nCount )
	{
		m_nItemsPerPage = nCount;
		if( m_nScrollRange < m_nItemsPerPage )
			m_nScrollRange = m_nItemsPerPage;

		UpdateScrollBarSize();
	}
	void	ChangeItemsPerPage( int ndCount )
	{
		m_nItemsPerPage += ndCount;
		UpdateScrollBarSize();
	}

	int		GetScrollPos() const { return m_nScrollPos; }
	int		GetScrollRange() const { return m_nScrollRange; }
	int		GetItemsPerPage() const { return m_nItemsPerPage; }
	int		GetCurItemCount() const { return m_nCurItemCount; }

	// Message
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UISCROLLBAR_H_

