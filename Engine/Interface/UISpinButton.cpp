#include "stdh.h"
#include <Engine/Interface/UISpinButton.h>
#include <Engine/Interface/UITextureManager.h>

#define SPIN_TITLE_GAP			3	// 제목과 DataBack사이
#define SPIN_DATABACK_GAP		5	// DataBack과 Data사이 Gap(오른쪽 기준)
#define SPIN_BUTTON_OFFSET_X	0	// Control Button의 상하위치
#define SPIN_BUTTON_OFFSET_Y	3	// Control Button의 상하위치

//------------------------------------------------------------------------------
// CUISpinButton::CUISpinButton
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISpinButton::CUISpinButton()
{
	m_bDataBack	= FALSE;
	m_strTitle	= "";	
	m_nSelectIndex = -1;
	m_bVertical = FALSE;
}

//------------------------------------------------------------------------------
// CUISpinButton::Create
// Explain:  MultiList생성시 추가 작업 필요로 
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, CTString strTitle, int nDataBackWidth )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_strTitle = strTitle;
	m_nDataBackWidth = nDataBackWidth;
	m_nStrWidth = ( m_strTitle.Length() ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 3;
}

//------------------------------------------------------------------------------
// CUISpinButton::CreateButtons
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::CreateButtons( int nBtnWidth, int nBtnHeight, int nButtonGap, BOOL bVertical )
{
	m_bVertical = bVertical;
	
	int nX = m_nStrWidth + m_nDataBackWidth + SPIN_BUTTON_OFFSET_X;

	// Create buttons
	if( m_bVertical )
	{
		m_btnUp.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y + nBtnHeight - nButtonGap, nBtnWidth, nBtnHeight );
	}
	else
	{
		m_btnUp.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), nX + nBtnWidth + nButtonGap, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
	}
}

//------------------------------------------------------------------------------
// CUISpinButton::Render
// Explain:  기본 루틴 그대로 가져옴.. 일부 수정
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::Render()
{
	if( !IsVisible() ) return;

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Data 배경 
	if( m_bDataBack )
	{
		//RenderRectUV3( 4, nX + m_nStrWidth, nY + m_nHeight - _pUIFontTexMgr->GetFontHeight(), m_nDataBackWidth, m_nHeight, m_rt3Data );
		RenderRectUV3( 4, nX + m_nStrWidth, nY +1, m_nDataBackWidth, m_nHeight, m_rt3Data );
	}

	// Title : 나중에 색정할수 있게 하면 좋겠다.
	_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, nX, nY + SPIN_TITLE_GAP );


	// Data 
	if( m_nSelectIndex >= 0 )
	{
		CTString strData = m_vecData[m_nSelectIndex];
		int nOffsetX, nOffsetY;

		nOffsetX = nX + m_nStrWidth + m_nDataBackWidth // 여기가 맨 왼쪽 
					- strData.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - SPIN_DATABACK_GAP;

		
		nOffsetY = nY + 2;

		_pUIMgr->GetDrawPort()->PutTextEx( strData, nOffsetX, nOffsetY ); 
		
	}
	
	// Render UP/DOWN Button
	m_btnUp.Render();
	m_btnDown.Render();
}

//------------------------------------------------------------------------------
// CUISpinButton::MouseMessage
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUISpinButton::MouseMessage( MSG *pMsg )
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
			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	MoveDataIndex( -1 );
					MoveDataIndex( +1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	MoveDataIndex( +1 );
					MoveDataIndex( -1 );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveDataIndex( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveDataIndex( +1 );
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
				MoveDataIndex( -swDelta );

				return WMSG_SUCCESS;
			}
		}
		break;
	}	
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISpinButton::AddData
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::AddData( CTString strData )
{
	m_nSelectIndex = 0;
	m_vecData.push_back( strData );
}

//------------------------------------------------------------------------------
// CUISpinButton::GetDataToNumber
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CUISpinButton::GetDataToNumber( int nIndex )
{
	if( nIndex < -1 || nIndex > (int)m_vecData.size() - 1 ) return -1;

	if( nIndex == -1 ) nIndex = m_nSelectIndex;

	int nData = atoi( m_vecData[nIndex].str_String );
	return nData;
}

//------------------------------------------------------------------------------
// CUISpinButton::GetDataToString
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISpinButton::GetDataToString( int nIndex )
{
	if( nIndex < -1 || nIndex > (int)m_vecData.size() - 1 ) return "NULL";
	
	if( nIndex == -1 ) nIndex = m_nSelectIndex;

	return m_vecData[nIndex];
}

//------------------------------------------------------------------------------
// CUISpinButton::MoveDataIndex
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::MoveDataIndex( int nIndexDelta )
{
	m_nSelectIndex += nIndexDelta;

	if( m_nSelectIndex < 0 ) m_nSelectIndex = 0;
	if( m_nSelectIndex > (int)m_vecData.size() - 1 ) m_nSelectIndex = (int)m_vecData.size() - 1;

}

//------------------------------------------------------------------------------
// CUISpinButton::RenderRectUV3
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 )
{
	int nX2 = nX + nWidth;
	int nY2 = nY + nHeight;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + nUnitSize, nY2,
										rtRectUV3.rtL.U0, rtRectUV3.rtL.V0, rtRectUV3.rtL.U1, rtRectUV3.rtL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX + nUnitSize, nY, nX2 - nUnitSize, nY2,
										rtRectUV3.rtM.U0, rtRectUV3.rtM.V0, rtRectUV3.rtM.U1, rtRectUV3.rtM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - nUnitSize, nY, nX2, nY2,
										rtRectUV3.rtR.U0, rtRectUV3.rtR.V0, rtRectUV3.rtR.U1, rtRectUV3.rtR.V1,
										0xFFFFFFFF );
}

void CUISpinButton::SetTitle( CTString strTitle )
{ 
	m_strTitle =strTitle;
	m_nStrWidth = ( m_strTitle.Length() ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 3;

	int nX = m_nStrWidth + m_nDataBackWidth + SPIN_BUTTON_OFFSET_X;
	
	m_btnUp.SetPosX( nX );
	m_btnDown.SetPosX( nX );
}