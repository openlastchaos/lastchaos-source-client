#include "stdh.h"
#include <Engine/Interface/UISlideBar.h>
#include <Engine/Interface/UIManager.h>


// ----------------------------------------------------------------------------
// Name : CUISlideBar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISlideBar::CUISlideBar()
{
	m_bSlideBarClick = FALSE;
	m_nCurPos = 0;
	m_nRange = 0;
	m_nMinPos = 0;
	m_nMaxPos = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUISlideBar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISlideBar::~CUISlideBar()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nBarWidth, int nBarHeight,
							int nCurPos, int nRange, int nMinPos, int nMaxPos )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_rcBar.Top = ( m_nHeight - nBarHeight ) / 2;
	m_rcBar.Bottom = m_rcBar.Top + nBarHeight;
	m_rcBar.Left = 0;
	m_rcBar.Right = nBarWidth;

	m_rcSlide.SetRect( 0, m_rcBar.Top, m_nWidth, m_rcBar.Bottom );

	m_nCurPos = nCurPos;
	m_nRange = nRange;
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;

	SetCurPos( nCurPos );
}

// ----------------------------------------------------------------------------
// Name : AdjustSlidePos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::AdjustSlidePos( int nX )
{
	int	nAbsX = GetAbsPosX();

	FLOAT	fCellWidth = ( m_nWidth - m_rcBar.GetWidth() ) / (FLOAT)( m_nRange - 1 );
	int		nSX = nAbsX + ( m_rcBar.GetWidth() - fCellWidth ) / 2;
	int		nCurPos = ( nX - nSX ) / fCellWidth;

	SetCurPos( nCurPos );
}

// ----------------------------------------------------------------------------
// Name : SetCurPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetCurPos( int nCurPos )
{
	if( nCurPos >= m_nMinPos && nCurPos <= m_nMaxPos )
	{
		m_nCurPos = nCurPos;
		int		nNewWidth = m_nWidth - m_rcBar.GetWidth();
		FLOAT	fOffset = m_nCurPos * (FLOAT)( nNewWidth / (FLOAT)( m_nRange - 1 ) );
		int		nBarWidth = m_rcBar.GetWidth();
		m_rcBar.Left = fOffset;
		m_rcBar.Right = m_rcBar.Left + nBarWidth;
	}
}

// ----------------------------------------------------------------------------
// Name : SetRange()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetRange( int nRange )
{
	if( m_nMinPos + nRange <= m_nMaxPos )
		m_nRange = nRange;
}

// ----------------------------------------------------------------------------
// Name : SetMinPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetMinPos( int nMinPos )
{
	m_nMinPos = nMinPos;
	if( m_nCurPos < m_nMinPos )
	{
		m_nCurPos = m_nMinPos;
		SetCurPos( m_nCurPos );
	}
}

// ----------------------------------------------------------------------------
// Name : SetMaxPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetMaxPos( int nMaxPos )
{
	m_nMaxPos = nMaxPos;
	if( m_nCurPos > m_nMaxPos )
	{
		m_nCurPos = m_nMaxPos;
		SetCurPos( m_nCurPos );
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0,
										m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Bar
	_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcBar.Left, nY + m_rcBar.Top,
										nX + m_rcBar.Right, nY + m_rcBar.Bottom,
										m_rtBar.U0, m_rtBar.V0, m_rtBar.U1, m_rtBar.V1,
										0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISlideBar::MouseMessage( MSG *pMsg )
{
	// If slide bar is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Bar button
			if( m_bSlideBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				AdjustSlidePos( nX );

				return WMSG_COMMAND;
			}

			if( IsInsideRect( nX, nY, m_rcSlide ) )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInsideRect( nX, nY, m_rcSlide ) )
			{
				// If slide bar is clicked
				if( IsInsideRect( nX, nY, m_rcBar ) )
					m_bSlideBarClick = TRUE;
				// If sliding region is clicked
				else
				{
					AdjustSlidePos( nX );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bSlideBarClick )
			{
				m_bSlideBarClick = FALSE;
				return WMSG_SUCCESS;
			}

			m_bSlideBarClick = FALSE;
		}
		break;
	}

	return WMSG_FAIL;
}
