#include "stdh.h"
#include <Engine/Interface/UIPetTarget.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/PetInfo.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>

#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3

#define INFOMARK_SIZE 16

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
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

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
	//SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
	SetPos( pixMinI + _pUIMgr->GetPlayerInfo()->GetWidth() + 1, pixMinJ + (_pUIMgr->GetPlayerInfo()->GetHeight() - GetHeight()) );
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
	FLOAT	fHPRatio = Clamp( _pNetwork->_PetTargetInfo.fHealth / _pNetwork->_PetTargetInfo.fMaxHealth, 0.0f, 1.0f );
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
	FLOAT	fHungryRatio = Clamp( _pNetwork->_PetTargetInfo.fHungry / _pNetwork->_PetTargetInfo.fMaxHungry, 0.0f, 1.0f );
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
	m_strToolTip = _S(2173,"Ćę Á¤ş¸Ă˘") ;
	if( g_iEnterChat )
		m_strToolTip+=CTString("(D,ALT+D)");
	else 
		m_strToolTip+=CTString("(ALT+D)");
	tv_posX =m_nPosX + m_btnPetInfo.GetPosX() -  m_btnPetInfo.GetWidth()/2 ;
	tv_posY = m_nPosY + m_btnPetInfo.GetPosY();
	tv_strSize = m_strToolTip.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
//	CTString tv_err;
//	tv_err.PrintF("%d %d %d\n",m_btnPetInfo.GetPosX(),m_btnPetInfo.GetPosY(),m_btnPetInfo.GetWidth());
//	OutputDebugString(tv_err);
	// Set target information texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	_pUIMgr->GetDrawPort()->AddTexture(	tv_posX, tv_posY - _pUIFontTexMgr->GetFontHeight() -5, tv_posX + 4, tv_posY - 2,
										m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( tv_posX +4, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize, tv_posY -2,
										m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( tv_posX +4 + tv_strSize, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize +5, tv_posY -2,
										m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
										0xFFFFFFFF );
	// show text
	_pUIMgr->GetDrawPort()->PutTextEx( m_strToolTip, tv_posX +4,
										tv_posY -_pUIFontTexMgr->GetFontHeight()-4, 0xFFFFFFFF );
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();	
	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Render()
{
	if( !_pNetwork->_PetTargetInfo.bIsActive )
		return;

	// Set target information texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 26,
		m_rtBackground1.U0, m_rtBackground1.V0, m_rtBackground1.U1, m_rtBackground1.V1,
		0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + 26, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackground2.U0, m_rtBackground2.V0, m_rtBackground2.U1, m_rtBackground2.V1,
		0xFFFFFFFF );
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	
	// Update info of target
	UpdateHPInfo();
	UpdateHungryInfo();
	
	// Background of HP
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHPBack.Left, m_nPosY + m_rcHPBack.Top,
		m_nPosX + m_rcHPBack.Right, m_nPosY + m_rcHPBack.Bottom,
		m_rtHPBack.U0, m_rtHPBack.V0, m_rtHPBack.U1, m_rtHPBack.V1,
		0xFFFFFFFF );

	// Background of Hungry
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHungryBack.Left, m_nPosY + m_rcHungryBack.Top,
		m_nPosX + m_rcHungryBack.Right, m_nPosY + m_rcHungryBack.Bottom,
		m_rtHungryBack.U0, m_rtHungryBack.V0, m_rtHungryBack.U1, m_rtHungryBack.V1,
		0xFFFFFFFF );
	
	// HP
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
		0xFFFFFFFF );

	// Hungry
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHungry.Left, m_nPosY + m_rcHungry.Top,
		m_nPosX + m_rcHungry.Right, m_nPosY + m_rcHungry.Bottom,
		m_rtHungry.U0, m_rtHungry.V0, m_rtHungry.U1, m_rtHungry.V1,
		0xFFFFFFFF );

	m_btnPetInfo.Render();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	// Pet name
	if( _pNetwork->_PetTargetInfo.strNameCard.Length() >0)
	{
		_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_PetTargetInfo.strNameCard, 0,
		m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
		0x008BAAFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->PutTextCharExCX( PetInfo().GetName(_pNetwork->_PetTargetInfo.iType, _pNetwork->_PetTargetInfo.iAge), 0,
			m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
			0x008BAAFF );
	}

	CTString strTemp;
	strTemp.PrintF("%d", _pNetwork->_PetTargetInfo.iLevel);
	_pUIMgr->GetDrawPort()->PutTextEx( strTemp, m_nPosX + 18,
										m_nPosY + 5, 0xF2F2F2B2 );
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();		

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
	// If target is not exist
	if( !_pNetwork->_PetTargetInfo.bIsActive )
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
				_pUIMgr->SetMouseCursorInsideUIs();
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !_pNetwork->_PetTargetInfo.bIsActive )
					bTitleBarClick = FALSE;

				return WMSG_SUCCESS;
			}

			if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_bShowTooltip = TRUE;
				_pUIMgr->RearrangeOrder( UI_PETTARGETINFO, TRUE );
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
					if( _pNetwork->_PetTargetInfo.pen_pEntity )
					{
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( _pNetwork->_PetTargetInfo.pen_pEntity );
					}
				}

				if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}

				_pUIMgr->RearrangeOrder( UI_PETTARGETINFO, TRUE );
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
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					// Close button
					if( m_btnPetInfo.MouseMessage( pMsg )  == WMSG_COMMAND )
					{					
						if( _pUIMgr->GetPetInfo()->IsVisible() )
						{
							_pUIMgr->GetPetInfo()->ClosePetInfo();								
						}
						else
						{
							_pUIMgr->GetPetInfo()->OpenPetInfo();
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


// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIWildPetTargetInfo::CUIWildPetTargetInfo()
{	
	m_bShowTooltip = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetTargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIWildPetTargetInfo::~CUIWildPetTargetInfo()
{
	Destroy();
}
// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	//SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
	SetPos( pixMinI + _pUIMgr->GetPlayerInfo()->GetWidth() + 1, pixMinJ + (_pUIMgr->GetPlayerInfo()->GetHeight() - GetHeight()) );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_PetInfo.tex") );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_bxTitle.SetBoxUV(m_ptdBaseTexture,10,WRect(4,429,166,505));
	m_bxTitle.SetBoxPos(WRect( 0, 0, 160, 78 ));

	m_rcTitle.SetRect( 0, 0, 160, 22 );
	
	m_rtHP.SetUV(175,415,178,422,fTexWidth,fTexHeight);
	m_rtMP.SetUV(180,415,183,422,fTexWidth,fTexHeight);
	m_rtEXP.SetUV(185,415,188,422,fTexWidth,fTexHeight);

	m_rcHP.SetRect( 16, 33, 143, 40 );
	m_rcMP.SetRect( 16, 47, 143, 54 );
	m_rcEXP.SetRect(16, 61, 143, 68 );

	m_btnPetInfo.Create( this, CTString( "" ), INFOMARK_POS_X+25, INFOMARK_POS_Y+3, INFOMARK_SIZE, INFOMARK_SIZE );
	m_btnPetInfo.SetUV( UBS_IDLE, 137, 408, 152, 423, fTexWidth, fTexHeight );
	m_btnPetInfo.SetUV( UBS_CLICK, 155, 408, 170, 423, fTexWidth, fTexHeight );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_bxTooltip.SetBoxUV(m_ptdButtonTexture,10,WRect(0,229,84,250));

}

void CUIWildPetTargetInfo::Render()
{
	if(_pNetwork->_WildPetInfo.bIsActive == FALSE) return;
	// Set target information texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	m_bxTitle.Render(m_nPosX, m_nPosY);

	UpdateHPInfo();
	UpdateMPInfo();
	UpdateEXPInfo();

	// HP
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,0xFFFFFFFF );


	// MP
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcMP.Left, m_nPosY + m_rcMP.Top,
		m_nPosX + m_rcMP.Right, m_nPosY + m_rcMP.Bottom,
		m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,0xFFFFFFFF );

	// EXP
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcEXP.Left, m_nPosY + m_rcEXP.Top,
			m_nPosX + m_rcEXP.Right, m_nPosY + m_rcEXP.Bottom,
			m_rtEXP.U0, m_rtEXP.V0, m_rtEXP.U1, m_rtEXP.V1,0xFFFFFFFF );

	CTString tempString;

	tempString.PrintF("%d",_pNetwork->_WildPetInfo.m_nLevel);
	_pUIMgr->GetDrawPort()->PutTextEx(tempString, m_nPosX+15, m_nPosY+8);

	_pUIMgr->GetDrawPort()->PutTextExCX(_pNetwork->_WildPetInfo.m_strName, m_nPosX+82, m_nPosY+8);

	tempString.PrintF("%d/%d",_pNetwork->_WildPetInfo.m_nHP,_pNetwork->_WildPetInfo.m_nMaxHP);
	_pUIMgr->GetDrawPort()->PutTextExCX(tempString, m_nPosX+82, m_nPosY+33);

	tempString.PrintF("%d/%d",_pNetwork->_WildPetInfo.m_nMP,_pNetwork->_WildPetInfo.m_nMaxMP);
	_pUIMgr->GetDrawPort()->PutTextExCX(tempString, m_nPosX+82, m_nPosY+47);
	
	tempString.PrintF("%.2f%%",((FLOAT)_pNetwork->_WildPetInfo.m_exp/(FLOAT)_pNetwork->_WildPetInfo.m_next_exp)*100.0f);
	_pUIMgr->GetDrawPort()->PutTextExCX(tempString, m_nPosX+82, m_nPosY+61);

	m_btnPetInfo.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

		// SHOW TOOLTIP
	if(m_bShowTooltip)
	{
		RenderTooltip();
	}
}

WMSG_RESULT CUIWildPetTargetInfo::MouseMessage(MSG *pMsg )
{
		// If target is not exist
	if( !_pNetwork->_WildPetInfo.bIsActive)
		return WMSG_FAIL;

//	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleClick = FALSE;

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
				_pUIMgr->SetMouseCursorInsideUIs();
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move target information
			if( bTitleClick && ( pMsg->wParam & MK_LBUTTON ) )
			{

				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if(!_pNetwork->_WildPetInfo.bIsActive )
					bTitleClick = FALSE;

				return WMSG_SUCCESS;
			}

			if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_bShowTooltip = TRUE;
				_pUIMgr->RearrangeOrder( UI_WILDPETTARGETINFO, TRUE );
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
				if( IsInsideRect( nX, nY, m_rcTitle ))
				{
					bTitleClick = TRUE;
				}
				else
				{
					if( _pNetwork->_WildPetInfo.pet_pEntity )
					{
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( _pNetwork->_WildPetInfo.pet_pEntity );
					}
				}
				
				if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				_pUIMgr->RearrangeOrder( UI_WILDPETTARGETINFO, TRUE );
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
				bTitleClick = FALSE;
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					// Close button
					if( m_btnPetInfo.MouseMessage( pMsg ) == WMSG_COMMAND )
					{
						if( _pUIMgr->GetWildPetInfo()->IsVisible() )
						{
							_pUIMgr->GetWildPetInfo()->CloseWildPetInfo();								
						}
						else
						{
							_pUIMgr->GetWildPetInfo()->OpenWildPetInfo();
						}							
					}
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

void CUIWildPetTargetInfo::UpdateHPInfo()
{
	FLOAT	fHPRatio = Clamp( (FLOAT)_pNetwork->_WildPetInfo.m_nHP / (FLOAT)_pNetwork->_WildPetInfo.m_nMaxHP, 0.0f, 1.0f );
	
	m_rcHP.Right = m_rcHP.Left + (PETTARGETINFO_BAR_WIDTH+10) * fHPRatio;

}

void CUIWildPetTargetInfo::UpdateMPInfo()
{
	FLOAT fMPRatio = Clamp( (FLOAT)_pNetwork->_WildPetInfo.m_nMP / (FLOAT)_pNetwork->_WildPetInfo.m_nMaxMP, 0.0f, 1.0f);

	m_rcMP.Right = m_rcMP.Left + (PETTARGETINFO_BAR_WIDTH+10) * fMPRatio;
}

void CUIWildPetTargetInfo::UpdateEXPInfo()
{
	FLOAT fExpRatio = Clamp( (FLOAT)_pNetwork->_WildPetInfo.m_exp / (FLOAT)_pNetwork->_WildPetInfo.m_next_exp, 0.0f, 1.0f);

	m_rcEXP.Right = m_rcEXP.Left + (PETTARGETINFO_BAR_WIDTH+10) * fExpRatio;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::RenderTooltip()
{
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );
	extern INDEX g_iEnterChat;
	int tv_strSize,tv_posX,tv_posY;
	m_strToolTip = _S(2173,"Ćę Á¤ş¸Ă˘") ;
	if( g_iEnterChat )
		m_strToolTip+=CTString("(P,ALT+P)");
	else 
		m_strToolTip+=CTString("(ALT+P)");
	tv_posX =m_nPosX + m_btnPetInfo.GetPosX() -  m_btnPetInfo.GetWidth()/2 ;
	tv_posY = m_nPosY + m_btnPetInfo.GetPosY();
	tv_strSize = m_strToolTip.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());

	m_bxTooltip.SetBoxPos(WRect(0,0,tv_strSize+10,20));

	m_bxTooltip.Render(tv_posX,tv_posY-20);
	// show text
	_pUIMgr->GetDrawPort()->PutTextEx( m_strToolTip, tv_posX +4,
										tv_posY -_pUIFontTexMgr->GetFontHeight()-4, 0xFFFFFFFF );
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();	
	
}