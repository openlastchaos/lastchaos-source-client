#include "stdh.h"
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>


// ----------------------------------------------------------------------------
// Name : CUIComboBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIComboBox::CUIComboBox()
{
	m_bScrollBarExist = FALSE;
	m_bShowScrollBar = FALSE;
	m_bDropList = FALSE;
	m_nSelectList = 0;
	m_nOverList = -1;
	m_nLineHeight = 0;
	m_nMaxLine = 0;
	m_nComboTextSY = 0;
	m_nTextSX = 0;
	m_nTextSY = 0;
	m_nSelectOffsetY = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIComboBox()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIComboBox::~CUIComboBox()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nBtnSX, int nBtnSY, int nBtnWidth, int nBtnHeight,
							int nMaxLine, int nLineHeight, int nSpaceX, int nSpaceY )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nMaxLine = nMaxLine;
	m_nLineHeight = nLineHeight;

	m_rcBtn.Left = nBtnSX;
	m_rcBtn.Top = nBtnSY;
	m_rcBtn.Right = m_rcBtn.Left + nBtnWidth;
	m_rcBtn.Bottom = m_rcBtn.Top + nBtnHeight;

	m_rcDropList.Left = 0;
	m_rcDropList.Right = nWidth;
	m_rcDropList.Top = nHeight;

	m_nComboTextSY = ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;
	m_nTextSX = m_rcDropList.Left + 1 + nSpaceX;
	m_nTextSY = m_rcDropList.Top + 1 + nSpaceY;

	int	nOffset = nLineHeight - _pUIFontTexMgr->GetFontHeight();
	if( nOffset < 0 )
		nOffset = 0;
	m_nSelectOffsetY = nOffset / 2;
}

// ----------------------------------------------------------------------------
// Name : CreateScroll()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::CreateScroll( BOOL bRight, int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
								int nBarVertEdgeSize )
{
	m_bScrollBarExist = TRUE;

	int	nWidth = m_rcDropList.GetWidth() - 2;
	int	nSpaceY = ( m_nTextSY - m_rcDropList.Top - 1 ) * 2 - ( m_nLineHeight - _pUIFontTexMgr->GetFontHeight() );
	int	nHeight = nSpaceY + m_nLineHeight * m_nMaxLine;

	if( bRight )
	{
		m_sbScrollBar.Create( this, m_rcDropList.Right - 1 - nBtnWidth, m_rcDropList.Top + 1, nBtnWidth, nHeight );
		m_sbScrollBar.SetWheelRect( -( nWidth - nBtnWidth ), 0, nWidth - nBtnWidth, nHeight );
	}
	else
	{
		m_sbScrollBar.Create( this, m_rcDropList.Left + 1, m_rcDropList.Top + 1, nBtnWidth, nHeight );
		m_sbScrollBar.SetWheelRect( nBtnWidth, 0, nWidth - nBtnWidth, nHeight );
	}

	m_sbScrollBar.CreateButtons( TRUE, nBtnWidth, nBtnHeight, nBarWidthGap, nBarHeightGap, nBarVertEdgeSize );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetItemsPerPage( m_nMaxLine );
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::AddString( CTString &strString )
{
	if( !m_bScrollBarExist && m_vecString.size() >= m_nMaxLine )
		return;

	m_vecString.push_back( strString );
	int	nLines = m_vecString.size();

	if( m_sbScrollBar.GetCurItemCount() < nLines )
		m_sbScrollBar.SetCurItemCount( nLines );

	if( m_bScrollBarExist && nLines > m_nMaxLine )
	{
		m_bShowScrollBar = TRUE;
		return;
	}

	int	nSpaceY = ( m_nTextSY - m_rcDropList.Top - 1 ) * 2 - ( m_nLineHeight - _pUIFontTexMgr->GetFontHeight() ) + 2;
	m_rcDropList.Bottom = m_rcDropList.Top + nSpaceY + m_nLineHeight * nLines;
}

// ----------------------------------------------------------------------------
// Name : ResetStrings()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::ResetStrings()
{
	m_vecString.clear();
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_bShowScrollBar = FALSE;
	m_nSelectList = 0;
	m_nOverList = -1;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	if( m_bDropList )
	{
		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcBtn.Left, nY + m_rcBtn.Top,
											nX + m_rcBtn.Right, nY + m_rcBtn.Bottom,
											m_rtUp.U0, m_rtUp.V0, m_rtUp.U1, m_rtUp.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcDropList.Left, nY + m_rcDropList.Top,
											nX + m_rcDropList.Right, nY + m_rcDropList.Top + CMB_DROPLIST_VERT_GAP,
											m_rtDropListTop.U0, m_rtDropListTop.V0,
											m_rtDropListTop.U1, m_rtDropListTop.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcDropList.Left, nY + m_rcDropList.Top + CMB_DROPLIST_VERT_GAP,
											nX + m_rcDropList.Right, nY + m_rcDropList.Bottom - CMB_DROPLIST_VERT_GAP,
											m_rtDropListMiddle.U0, m_rtDropListMiddle.V0,
											m_rtDropListMiddle.U1, m_rtDropListMiddle.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcDropList.Left, nY + m_rcDropList.Bottom - CMB_DROPLIST_VERT_GAP,
											nX + m_rcDropList.Right, nY + m_rcDropList.Bottom,
											m_rtDropListBottom.U0, m_rtDropListBottom.V0,
											m_rtDropListBottom.U1, m_rtDropListBottom.V1,
											0xFFFFFFFF );

		// Scroll bar
		if( m_bShowScrollBar )
			m_sbScrollBar.Render();
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcBtn.Left, nY + m_rcBtn.Top,
											nX + m_rcBtn.Right, nY + m_rcBtn.Bottom,
											m_rtDown.U0, m_rtDown.V0, m_rtDown.U1, m_rtDown.V1,
											0xFFFFFFFF );
	}

	// Text
	if( m_vecString.size() > 0 )
		_pUIMgr->GetDrawPort()->PutTextEx( m_vecString[m_nSelectList],
											nX + m_nTextSX, nY + m_nComboTextSY, 0xF2F2F2FF );

	if( m_bDropList )
	{
		int	nTexX = nX + m_nTextSX;
		int	nTexY = nY + m_nTextSY;
		int	nRowS = m_sbScrollBar.GetScrollPos();
		int	nRowE = nRowS + m_nMaxLine;
		if( nRowE > m_vecString.size() )
			nRowE = m_vecString.size();

		for( int iString = nRowS; iString < nRowE; iString++ )
		{
			if( m_nOverList == iString )
				_pUIMgr->GetDrawPort()->PutTextEx( m_vecString[iString], nTexX, nTexY, 0xFF6107FF );
			else
				_pUIMgr->GetDrawPort()->PutTextEx( m_vecString[iString], nTexX, nTexY, 0xC5B083FF );

			nTexY += m_nLineHeight;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIComboBox::MouseMessage( MSG *pMsg )
{
	// If combo box is disabled
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
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				int	nAbsY = GetAbsPosY();
				m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
								m_sbScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}

			m_nOverList = -1;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				m_bDropList = !m_bDropList;

				return WMSG_SUCCESS;
			}
			else if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				m_bDropList = FALSE;

				int	nAbsY = GetAbsPosY();
				m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
								m_sbScrollBar.GetScrollPos();
				if( m_nOverList >= 0 && m_nOverList < m_vecString.size() )
				{
					m_nSelectList = m_nOverList;
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}

			m_bDropList = FALSE;
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
			else if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}
