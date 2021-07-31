#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetTarget.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3

#define INFOMARK_SIZE 16

#define	PETTARGETINFO_BAR_WIDTH			120

// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPetTargetInfo::CUIPetTargetInfo()
{	
	m_bShowTooltip = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetTargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPetTargetInfo::~CUIPetTargetInfo()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 140, 22 );
	m_rcHP.SetRect( 10, 27, 130, 35 );
	m_rcHPBack.SetRect( 5, 25, 135, 37 );
	m_rcHungry.SetRect( 10, 41, 130, 46 );	
	m_rcHungryBack.SetRect( 5, 40, 135, 48 );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TargetInfo.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground1.SetUV( 0, 0, 140, 26, fTexWidth, fTexHeight );
	m_rtBackground2.SetUV( 0, 27, 140, 43, fTexWidth, fTexHeight );

	// HP
	m_rtHP.SetUV( 1, 44, 2, 52, fTexWidth, fTexHeight );

	// Hungry
	m_rtHungry.SetUV( 6, 44, 8, 52, fTexWidth, fTexHeight );

	// HP background
	m_rtHPBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );

	// Hungry background
	m_rtHungryBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );	

	m_btnPetInfo.Create( this, CTString( "" ), INFOMARK_POS_X, INFOMARK_POS_Y, INFOMARK_SIZE, INFOMARK_SIZE );
	m_btnPetInfo.SetUV( UBS_IDLE, 159, 28, 175, 44, fTexWidth, fTexHeight );
	m_btnPetInfo.SetUV( UBS_CLICK, 159, 28, 175, 44, fTexWidth, fTexHeight );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Set tooptip UV
	m_rtToolTipL.SetUV(142,4,146,24,fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV(146,4,156,24,fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV(156,4,161,24,fTexWidth, fTexHeight );

	
}


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	//SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
	SetPos( pixMinI + pUIManager->GetPlayerInfo()->GetWidth() + 1, pixMinJ + (pUIManager->GetPlayerInfo()->GetHeight() - GetHeight()) );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::UpdateHPInfo()
{
	// HP
	// Modified by yjpark
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT	fHPRatio = Clamp( pInfo->GetMyPetInfo()->fHealth / pInfo->GetMyPetInfo()->fMaxHealth, 0.0f, 1.0f );
	// Modified by yjpark
	m_rcHP.Right = m_rcHP.Left + PETTARGETINFO_BAR_WIDTH * fHPRatio;
}

// ----------------------------------------------------------------------------
// Name : UpdateHungryInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::UpdateHungryInfo()
{
	// Hungry
	// Modified by yjpark
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT	fHungryRatio = Clamp( pInfo->GetMyPetInfo()->fHungry / pInfo->GetMyPetInfo()->fMaxHungry, 0.0f, 1.0f );
	// Modified by yjpark
	m_rcHungry.Right = m_rcHungry.Left + PETTARGETINFO_BAR_WIDTH * fHungryRatio;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::RenderTooltip()
{
	extern INDEX g_iEnterChat;
	int tv_strSize,tv_posX,tv_posY;
	m_strToolTip = _S(2173,"펫 정보창") ;
	if( g_iEnterChat )
		m_strToolTip+=CTString("(P,ALT+P)");
	else 
		m_strToolTip+=CTString("(ALT+P)");
	tv_posX =m_nPosX + m_btnPetInfo.GetPosX() -  m_btnPetInfo.GetWidth()/2 ;
	tv_posY = m_nPosY + m_btnPetInfo.GetPosY();
	tv_strSize = m_strToolTip.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
//	CTString tv_err;
//	tv_err.PrintF("%d %d %d\n",m_btnPetInfo.GetPosX(),m_btnPetInfo.GetPosY(),m_btnPetInfo.GetWidth());
//	OutputDebugString(tv_err);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture(	tv_posX, tv_posY - _pUIFontTexMgr->GetFontHeight() -5, tv_posX + 4, tv_posY - 2,
										m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( tv_posX +4, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize, tv_posY -2,
										m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( tv_posX +4 + tv_strSize, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize +5, tv_posY -2,
										m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
										0xFFFFFFFF );
	// show text
	pDrawPort->PutTextEx( m_strToolTip, tv_posX +4,
										tv_posY -_pUIFontTexMgr->GetFontHeight()-4, 0xFFFFFFFF );
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();	
	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Render()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if( !pInfo->GetMyPetInfo()->bIsActive || _pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == TRUE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 26,
		m_rtBackground1.U0, m_rtBackground1.V0, m_rtBackground1.U1, m_rtBackground1.V1,
		0xFFFFFFFF );

	pDrawPort->AddTexture( m_nPosX, m_nPosY + 26, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackground2.U0, m_rtBackground2.V0, m_rtBackground2.U1, m_rtBackground2.V1,
		0xFFFFFFFF );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	
	// Update info of target
	UpdateHPInfo();
	UpdateHungryInfo();
	
	// Background of HP
	pDrawPort->AddTexture( m_nPosX + m_rcHPBack.Left, m_nPosY + m_rcHPBack.Top,
		m_nPosX + m_rcHPBack.Right, m_nPosY + m_rcHPBack.Bottom,
		m_rtHPBack.U0, m_rtHPBack.V0, m_rtHPBack.U1, m_rtHPBack.V1,
		0xFFFFFFFF );

	// Background of Hungry
	pDrawPort->AddTexture( m_nPosX + m_rcHungryBack.Left, m_nPosY + m_rcHungryBack.Top,
		m_nPosX + m_rcHungryBack.Right, m_nPosY + m_rcHungryBack.Bottom,
		m_rtHungryBack.U0, m_rtHungryBack.V0, m_rtHungryBack.U1, m_rtHungryBack.V1,
		0xFFFFFFFF );
	
	// HP
	pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
		0xFFFFFFFF );

	// Hungry
	pDrawPort->AddTexture( m_nPosX + m_rcHungry.Left, m_nPosY + m_rcHungry.Top,
		m_nPosX + m_rcHungry.Right, m_nPosY + m_rcHungry.Bottom,
		m_rtHungry.U0, m_rtHungry.V0, m_rtHungry.U1, m_rtHungry.V1,
		0xFFFFFFFF );

	m_btnPetInfo.Render();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	// Pet name
	if( pInfo->GetMyPetInfo()->strNameCard.Length() >0)
	{
		pDrawPort->PutTextCharExCX( pInfo->GetMyPetInfo()->strNameCard, 0,
		m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
		0x008BAAFF );
	}
	else
	{
		pDrawPort->PutTextCharExCX( PetInfo().GetName(pInfo->GetMyPetInfo()->iType, pInfo->GetMyPetInfo()->iAge), 0,
			m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
			0x008BAAFF );
	}

	CTString strTemp;
	strTemp.PrintF("%d", pInfo->GetMyPetInfo()->iLevel);
	pDrawPort->PutTextEx( strTemp, m_nPosX + 18,
										m_nPosY + 5, 0xF2F2F2B2 );
	// Flush all render text queue
	pDrawPort->EndTextEx();		

	// SHOW TOOLTIP
	if(m_bShowTooltip)
	{
		RenderTooltip();
	}
}


// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetTargetInfo::MouseMessage( MSG *pMsg )
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	// If target is not exist
	if( !pInfo->GetMyPetInfo()->bIsActive )
		return WMSG_FAIL;

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
			m_bShowTooltip = FALSE;
			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !pInfo->GetMyPetInfo()->bIsActive )
					bTitleBarClick = FALSE;

				return WMSG_SUCCESS;
			}

			if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_bShowTooltip = TRUE;
				CUIManager::getSingleton()->RearrangeOrder( UI_PETTARGETINFO, TRUE );
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
				else
				{
					if( pInfo->GetMyPetInfo()->pen_pEntity )
					{
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( pInfo->GetMyPetInfo()->pen_pEntity );
					}
				}

				if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PETTARGETINFO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					// Close button
					if( m_btnPetInfo.MouseMessage( pMsg )  == WMSG_COMMAND )
					{					
						if( pUIManager->GetPetInfo()->IsVisible() )
						{
							pUIManager->GetPetInfo()->ClosePetInfo();								
						}
						else
						{
							pUIManager->GetPetInfo()->OpenPetInfo();
						}						
					
						return WMSG_SUCCESS;
					}
				}
			}
		}break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

