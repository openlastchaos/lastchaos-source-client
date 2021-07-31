#include "stdh.h"
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Entities/InternalClasses.h>


// ----------------------------------------------------------------------------
// Name : CUIButton()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIButton::CUIButton()
{
	m_bUseNewType = FALSE;
	m_bLButtonDown = FALSE;
	m_bsState = UBS_IDLE;
	m_strText = CTString( "" );
	m_bValidText = FALSE;
	m_bOnlyText = FALSE;
	m_colText[UBS_IDLE] = 0xF2F2F2FF;
	m_colText[UBS_ON] = 0xFFFFFFFF;
	m_colText[UBS_CLICK] = 0xFFFFFFFF;
	m_colText[UBS_DISABLE] = 0xB3B3B3FF;
	m_bVerticality = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIButton()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIButton::~CUIButton()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::Create( CUIWindow *pParentWnd, CTString &strText, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	m_strText = strText;
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText && nWidth == 0 && nHeight == 0 )
	{
		int	nTextWidth = nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nTextHeight = _pUIFontTexMgr->GetFontHeight();
		SetSize( nTextWidth, nTextHeight );
		m_bOnlyText = TRUE;
	}
}

// ----------------------------------------------------------------------------
// Name : SetText()
// Desc :
// ----------------------------------------------------------------------------
int CUIButton::SetText( CTString &strText, BOOL bVerticality )
{
	m_strText = strText;
	m_bVerticality = bVerticality;
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText )
	{
		if( m_bOnlyText )
		{
			int	nTextWidth = nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			int	nTextHeight = _pUIFontTexMgr->GetFontHeight();
			SetSize( nTextWidth, nTextHeight );
		}
	}
	else
		m_bOnlyText = FALSE;

	return nLength;
}

// ----------------------------------------------------------------------------
// Name : RenderHighlight()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::RenderHighlight( const COLOR colHighlight )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );
	
	// Add render regions
	if (m_bUseNewType)
	{
		m_rtSurface[m_bsState].SetPos(nX, nY);
		m_rtSurface[m_bsState].RenderRectSurface(_pUIMgr->GetDrawPort(), colHighlight);
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
												m_rtUV[m_bsState].U0, m_rtUV[m_bsState].V0,
												m_rtUV[m_bsState].U1, m_rtUV[m_bsState].V1,
												colHighlight );
	}
	
	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::Render( COLOR textureColor )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	if( !m_bOnlyText )
	{
		if (m_bUseNewType)
		{
			m_rtSurface[m_bsState].SetPos(nX, nY);
			m_rtSurface[m_bsState].RenderRectSurface(_pUIMgr->GetDrawPort(), textureColor);
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
												m_rtUV[m_bsState].U0, m_rtUV[m_bsState].V0,
												m_rtUV[m_bsState].U1, m_rtUV[m_bsState].V1,
												textureColor );
		}
	}
	int nLength = m_strText.Length();
	int PosCont=0;
	for(int i = 0; i < nLength; i++)
	{
		if(m_strText[i]&0x80)
		{
			i++;
		}
		
		PosCont++;

	}
	

	// Text
	if( m_bValidText )
	{
		GetAbsPos( nX, nY );
		if( m_bOnlyText )
		{
			if(m_bVerticality)
			{
				int yPos = nY - ((12 * (PosCont-2))/2-6);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{
					if( m_strText[con] & 0x80 )
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';
						temStr.PrintF("%s",chatext);
						con++;
					}else temStr.PrintF("%c",m_strText[con]);

					_pUIMgr->GetDrawPort()->PutTextEx( temStr, nX-5, yPos+(FontGap*12), m_colText[m_bsState] );
					FontGap++;
					
				}
			}
			else _pUIMgr->GetDrawPort()->PutTextEx( m_strText, nX, nY, m_colText[m_bsState] );
		}
		else
		{
			nX += m_nWidth >> 1;
			nY += ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) >> 1;
			if( m_bsState == UBS_CLICK )
			{
				++nX;	++nY;
			}

			extern INDEX g_iCountry; 
			extern BOOL g_bIsMalEng;
			if(g_iCountry == MALAYSIA && g_bIsMalEng) nX +=_pUIFontTexMgr->GetFontWidth();
		//	_pUIMgr->GetDrawPort()->PutTextExCX( m_strText, nX, nY, m_colText[m_bsState] );
			if(m_bVerticality)
			{
				int yPos = nY - ((12 * (PosCont-2))/2-6);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{
					if( m_strText[con] & 0x80 )
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';	
						temStr.PrintF("%s",chatext);
						con++;
					}else temStr.PrintF("%c",m_strText[con]);

					_pUIMgr->GetDrawPort()->PutTextEx( temStr, nX-5, yPos+(FontGap*12), m_colText[m_bsState] );
					FontGap++;

				}
			}
			else _pUIMgr->GetDrawPort()->PutTextExCX( m_strText, nX, nY, m_colText[m_bsState] );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIButton::MouseMessage( MSG *pMsg )
{
	// If button is disabled
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
			if( IsInside( nX, nY ) )
			{
				if( !( pMsg->wParam & MK_LBUTTON ) )
					m_bLButtonDown = FALSE;

				if( m_bsState != UBS_CLICK )
				{
					if( m_bLButtonDown )
						m_bsState = UBS_CLICK;
					else
						m_bsState = UBS_ON;
				}

				return WMSG_SUCCESS;
			}

			m_bsState = UBS_IDLE;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CEntity			*penPlEntity;
				CPlayerEntity	*penPlayerEntity;

				penPlEntity = CEntity::GetPlayerEntity( 0 );
				penPlayerEntity = (CPlayerEntity *)penPlEntity;
				penPlayerEntity->PlayButtonSound();

				m_bsState = UBS_CLICK;
				m_bLButtonDown = TRUE;
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			m_bLButtonDown = FALSE;

			if( IsInside( nX, nY ) )
			{
				if( m_bsState == UBS_CLICK )
				{
					m_bsState = UBS_ON;
					return WMSG_COMMAND;
				}
				else
				{
					m_bsState = UBS_ON;
					return WMSG_SUCCESS;
				}
			}

			m_bsState = UBS_IDLE;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				m_bsState = UBS_ON;
				return WMSG_COMMAND;
			}
		}
		break;
	}

	return WMSG_FAIL;
}
