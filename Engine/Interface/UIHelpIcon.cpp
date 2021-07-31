#include "stdh.h"
#include <Engine/Interface/UIHelpIcon.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/LocalDefine.h>

extern INDEX g_iShowHelp1Icon;

// ----------------------------------------------------------------------------
// Name : CUIHelpIcon()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIHelpIcon::CUIHelpIcon()
{

}

// ----------------------------------------------------------------------------
// Name : ~CUIHelpIcon()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelpIcon::~CUIHelpIcon()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 14, 44 );
	m_rcBack.SetRect( 14, 0, 49, 44 );
	m_rcMark.SetRect( 0, 0, 49, 44 );
	m_rcLineV.SetRect( 49, 0, 52, 44 );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\QuickSlot.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Title UV
	m_rtTitle.SetUV( 0, 64, 13, 107, fTexWidth, fTexHeight );
	// Background UV
	m_rtBack.SetUV( 67, 64, 103, 107, fTexWidth, fTexHeight );
		
	// Help Button UV
	m_rtHelpIdle.SetUV( 70, 0, 104, 34, fTexWidth, fTexHeight );
	m_rtHelpClick.SetUV( 105, 0, 139, 34, fTexWidth, fTexHeight );

	// Vertical Line 
	m_rtLineV.SetUV( 474,64,477,107, fTexWidth, fTexHeight );

	// Help Button Create
	m_btnHelp.Create( this, CTString(""), 16, 6, 32, 32 );
	m_btnHelp.CopyUV( UBS_IDLE, m_rtHelpIdle );
	m_btnHelp.CopyUV( UBS_CLICK, m_rtHelpClick );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_bShowBack = FALSE;	

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth - RADAR_WIDTH, pixMinJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::Render()
{	
#ifdef	HELP_SYSTEM_1
	if( g_iShowHelp1Icon)
	{
		RenderIcon();
	}
#else
	// [070619: Su-won] TargetInfo창이 나타나지 않는 버그 수정
	// UIHelpIcon을 Disable을 시키고 RearrangeOrder 하지 않아서
	// 다른 UI가 RearrangeOrder 되면서 UITargetInfo 자리에 들어가서
	// UITargetInfo가 UI목록에서 없어져버렸음.
	//SetEnable(FALSE);
	//SetVisible(FALSE);
	if( this->IsEnabled() )
		_pUIMgr->RearrangeOrder( UI_HELP_ICON, FALSE );
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::RenderIcon()
{
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	if(m_bShowBack)
	{
	// Title region
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTitle.Left, m_nPosY +m_rcTitle.Top,
										m_nPosX + m_rcTitle.Right, m_nPosY +m_rcTitle.Bottom,
										m_rtTitle.U0, m_rtTitle.V0, m_rtTitle.U1, m_rtTitle.V1,
										0xFFFFFFFF );
	// Background region
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcBack.Left, m_nPosY + m_rcBack.Top,
										m_nPosX + m_rcBack.Right, m_nPosY + m_rcBack.Bottom,
										m_rtBack.U0, m_rtBack.V0, m_rtBack.U1, m_rtBack.V1,
										0xFFFFFFFF );
	// Vertical Line
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcLineV.Left, m_nPosY + m_rcLineV.Top,
										m_nPosX + m_rcLineV.Right, m_nPosY + m_rcLineV.Bottom,
										m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
										0xFFFFFFFF );
	}
	

	m_btnHelp.Render();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelpIcon::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				_pUIMgr->SetMouseCursorInsideUIs();
				_pUIMgr->RearrangeOrder( UI_HELP_ICON, TRUE );				
			}

			m_bShowBack = IsInsideRect(nX,nY,m_rcMark);

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}

				if(m_btnHelp.MouseMessage(pMsg) != WMSG_FAIL){
					// TODO : Nothing
				};

				_pUIMgr->RearrangeOrder( UI_HELP_ICON, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				if( IsInside( nX, nY ) )
				{
					if(m_btnHelp.MouseMessage(pMsg) != WMSG_FAIL && g_iShowHelp1Icon)
					{
						_pUIMgr->GetHelp()->OpenHelp();						
					};
					return WMSG_SUCCESS;
				}

				

			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}
