#include "stdh.h"
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIManager.h>


// ----------------------------------------------------------------------------
// Name : CUIScrollBar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIScrollBar::CUIScrollBar()
{
	m_bScrollBarClick = FALSE;
	m_nScrollPos = 0;
	m_nScrollRange = 0;
	m_nCurItemCount = 0;
	m_nItemsPerPage = 0;
	m_rcWheel.SetRect( 0, 0, 0, 0 );
}

// ----------------------------------------------------------------------------
// Name : ~CUIScrollBar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIScrollBar::~CUIScrollBar()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : CreateButtons()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::CreateButtons( BOOL bVerticalScroll,	int nBtnWidth, int nBtnHeight,
									int nBarWidthGap, int nBarHeightGap, int nBarVertEdgeSize )
{
	m_bVertical = bVerticalScroll;
	m_nBarEdgeSize = nBarVertEdgeSize;

	// Create buttons
	if( bVerticalScroll )
	{
		m_nBarPosX = nBarWidthGap;
		m_nBarPosY = nBtnHeight + nBarHeightGap;
		m_nBarWidth = m_nWidth - nBarWidthGap * 2;
		m_nBarHeight = m_nHeight - nBarHeightGap * 2 - nBtnHeight * 2;

		m_btnUp.Create( this, CTString( "" ), 0, 0, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), 0, m_nHeight - nBtnHeight, nBtnWidth, nBtnHeight );
	}
	else
	{
		m_nBarPosX = nBtnWidth + nBarWidthGap;
		m_nBarPosY = nBarHeightGap;
		m_nBarWidth = m_nWidth - nBarWidthGap * 2 - nBtnWidth * 2;
		m_nBarHeight = m_nHeight - nBarHeightGap * 2;

		m_btnUp.Create( this, CTString( "" ), 0, 0, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), m_nWidth - nBtnWidth, 0, nBtnWidth, nBtnHeight );
	}

	m_rcScrolling.SetRect( m_nBarPosX, m_nBarPosY, m_nBarPosX + m_nBarWidth, m_nBarPosY + m_nBarHeight );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::Render()
{
	// Get position
	int	nAbsX, nAbsY;
	GetAbsPos( nAbsX, nAbsY );

	// Add render regions
	// Bar button
	int	nBarX = nAbsX + m_nBarPosX;
	int	nBarY = nAbsY + m_nBarPosY;
	if( m_bVertical )
	{
		_pUIMgr->GetDrawPort()->AddTexture( nBarX, nBarY,
											nBarX + m_nBarWidth, nBarY + m_nBarEdgeSize,
											m_rtBarTop.U0, m_rtBarTop.V0,
											m_rtBarTop.U1, m_rtBarTop.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nBarX, nBarY + m_nBarEdgeSize,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight - m_nBarEdgeSize,
											m_rcBarMiddle.U0, m_rcBarMiddle.V0,
											m_rcBarMiddle.U1, m_rcBarMiddle.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nBarX, nBarY + m_nBarHeight - m_nBarEdgeSize,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight,
											m_rtBarBottom.U0, m_rtBarBottom.V0,
											m_rtBarBottom.U1, m_rtBarBottom.V1,
											0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( nBarX, nBarY,
											nBarX + m_nBarEdgeSize, nBarY + m_nBarHeight,
											m_rtBarTop.U0, m_rtBarTop.V0,
											m_rtBarTop.U1, m_rtBarTop.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nBarX + m_nBarEdgeSize, nBarY,
											nBarX + m_nBarWidth - m_nBarEdgeSize, nBarY + m_nBarHeight,
											m_rcBarMiddle.U0, m_rcBarMiddle.V0,
											m_rcBarMiddle.U1, m_rcBarMiddle.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nBarX + m_nBarWidth - m_nBarEdgeSize, nBarY,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight,
											m_rtBarBottom.U0, m_rtBarBottom.V0,
											m_rtBarBottom.U1, m_rtBarBottom.V1,
											0xFFFFFFFF );
	}

	// Up button
	m_btnUp.Render();

	// Down button
	m_btnDown.Render();	
}

// ----------------------------------------------------------------------------
// Name : ChangeSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::ChangeSize( int ndSize )
{
	if( m_bVertical )
	{
		m_rcScrolling.Bottom += ndSize;
		m_nHeight += ndSize;
		m_rcWheel.Bottom += ndSize;
		m_btnDown.Move( 0, ndSize );
	}
	else
	{
		m_rcScrolling.Right += ndSize;
		m_nWidth += ndSize;
		m_rcWheel.Right += ndSize;
		m_btnDown.Move( ndSize, 0 );
	}

	UpdateScrollBarSize();
}

// ----------------------------------------------------------------------------
// Name : MoveScrollPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::MoveScrollPos( int ndPos )
{
	if( ndPos < 0 )
	{
		m_nScrollPos += ndPos;
		
		if( m_nScrollPos < 0 )
			m_nScrollPos = 0;
	}
	else
	{
		m_nScrollPos += ndPos;
		
		int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
		if( nScrollLine < 0 )
			m_nScrollPos = 0;
		else if( m_nScrollPos > nScrollLine )
			m_nScrollPos = nScrollLine;
	}

	UpdateScrollBarPos();
}

// ----------------------------------------------------------------------------
// Name : UpdateScrollBarSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::UpdateScrollBarSize()
{
	FLOAT	fSizeRatio = 1.0f;
	if( m_nCurItemCount > 0 && m_nItemsPerPage > 0 && m_nCurItemCount > m_nItemsPerPage )
		fSizeRatio = m_nItemsPerPage / (FLOAT)m_nCurItemCount;
	else
		m_nScrollPos = 0;

	int	nMinBarSize = m_nBarEdgeSize * 2;
	if( m_bVertical )
	{
		m_nBarHeight = m_rcScrolling.GetHeight() * fSizeRatio;
		if( m_nBarHeight < nMinBarSize )
			m_nBarHeight = nMinBarSize;
	}
	else
	{
		m_nBarWidth = m_rcScrolling.GetWidth() * fSizeRatio;
		if( m_nBarWidth < nMinBarSize )
			m_nBarWidth = nMinBarSize;
	}

	UpdateScrollBarPos();
}

// ----------------------------------------------------------------------------
// Name : UpdateScrollBarPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::UpdateScrollBarPos()
{
	FLOAT	fPosRatio = 0.0f;
	int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
	if( nScrollLine > 0 )
		fPosRatio = m_nScrollPos / (FLOAT)nScrollLine;

	if( m_bVertical )
		m_nBarPosY = m_rcScrolling.Top + ( m_rcScrolling.GetHeight() - m_nBarHeight ) * fPosRatio;
	else
		m_nBarPosX = m_rcScrolling.Left + ( m_rcScrolling.GetWidth() - m_nBarWidth ) * fPosRatio;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIScrollBar::MouseMessage( MSG *pMsg )
{
	// If scroll bar is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Messages
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Bar button
			if( m_bScrollBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	nAbsY, nAbsX;
				GetAbsPos( nAbsX, nAbsY );

				int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
				if( nScrollLine < 0 )
					nScrollLine = 0;

				if( m_bVertical )
				{
					if( m_nDragPosOffset != 0 )
					{
						BOOL	bOldNegative = m_nDragPosOffset < 0;
						m_nDragPosOffset += nY - m_nDragPos;
						BOOL	bNewNegative = m_nDragPosOffset < 0;

						if( bOldNegative != bNewNegative )
						{
							m_nBarPosY += m_nDragPosOffset;
							m_nDragPosOffset = 0;
						}
					}
					else
						m_nBarPosY += nY - m_nDragPos;

					if( m_nBarPosY < m_rcScrolling.Top )
					{
						m_nDragPosOffset += m_nBarPosY - m_rcScrolling.Top;
						m_nBarPosY = m_rcScrolling.Top;
					}
					else if( m_nBarPosY > m_rcScrolling.Bottom - m_nBarHeight )
					{
						m_nDragPosOffset += m_nBarPosY - ( m_rcScrolling.Bottom - m_nBarHeight );
						m_nBarPosY = m_rcScrolling.Bottom - m_nBarHeight;
					}

					FLOAT	fPosRatio = ( m_nBarPosY - m_rcScrolling.Top ) /
										(FLOAT)( m_rcScrolling.GetHeight() - m_nBarHeight );
					m_nScrollPos = nScrollLine * fPosRatio;

					m_nDragPos = nY;
				}
				else
				{
					if( m_nDragPosOffset != 0 )
					{
						BOOL	bOldNegative = m_nDragPosOffset < 0;
						m_nDragPosOffset += nX - m_nDragPos;
						BOOL	bNewNegative = m_nDragPosOffset < 0;

						if( bOldNegative != bNewNegative )
						{
							m_nBarPosX += m_nDragPosOffset;
							m_nDragPosOffset = 0;
						}
					}
					else
						m_nBarPosX += nX - m_nDragPos;

					if( m_nBarPosX < m_rcScrolling.Left )
					{
						m_nDragPosOffset += m_nBarPosX - m_rcScrolling.Left;
						m_nBarPosX = m_rcScrolling.Left;
					}
					else if( m_nBarPosX > m_rcScrolling.Right - m_nBarWidth )
					{
						m_nDragPosOffset += m_nBarPosX - ( m_rcScrolling.Right - m_nBarWidth );
						m_nBarPosX = m_rcScrolling.Right - m_nBarWidth;
					}

					FLOAT	fPosRatio = ( m_nBarPosX - m_rcScrolling.Left ) /
										(FLOAT)( m_rcScrolling.GetWidth() - m_nBarWidth );
					m_nScrollPos = nScrollLine * fPosRatio;

					m_nDragPos = nX;
				}

				return WMSG_COMMAND;
			}
			// Up button
			else if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Down button
			else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_nCurItemCount <= m_nItemsPerPage )
					return WMSG_SUCCESS;

				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					MoveScrollPos( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					MoveScrollPos( +1 );
					return WMSG_COMMAND;
				}
				// Bar button
				else if( IsInsideRect( nX, nY, m_rcScrolling ) )
				{
					m_nDragPosOffset = 0;

					int	nAbsY, nAbsX;
					GetAbsPos( nAbsX, nAbsY );
					nAbsX += m_nBarPosX;	nAbsY += m_nBarPosY;
					if( nX >= nAbsX && nY >= nAbsY && nX < nAbsX + m_nBarWidth && nY < nAbsY + m_nBarHeight )
					{
						m_bScrollBarClick = TRUE;
						if( m_bVertical )
							m_nDragPos = nY;
						else
							m_nDragPos = nX;
					}
					else
					{
						if( m_bVertical )
						{
							int	nCenterY = nAbsY + m_nBarHeight / 2;
							if( nY < nCenterY )
								MoveScrollPos( -m_nItemsPerPage );
							else
								MoveScrollPos( m_nItemsPerPage );
						}
						else
						{
							int	nCenterX = nAbsX + m_nBarWidth / 2;
							if( nX < nCenterX )
								MoveScrollPos( -m_nItemsPerPage );
							else
								MoveScrollPos( m_nItemsPerPage );
						}

						return WMSG_COMMAND;
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// Bar button
			if( m_bScrollBarClick )
			{
				m_bScrollBarClick = FALSE;
				m_nDragPosOffset = 0;
				return WMSG_SUCCESS;
			}
			// Up button
			else if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Down button
			else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveScrollPos( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveScrollPos( +1 );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) || IsInsideRect( nX, nY, m_rcWheel ) )
			{
				SWORD	swDelta = SWORD( HIWORD( pMsg->wParam ) ) / WHEEL_DELTA;
				MoveScrollPos( -swDelta );

				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}
