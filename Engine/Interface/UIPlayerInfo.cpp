#include "stdh.h"
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Base/KeyNames.h>

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIPetInfo.h>

#include <Engine/LocalDefine.h>

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIPlayerInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPlayerInfo::CUIPlayerInfo()
{
	m_fHPRatio = 0.0f;
	m_fMPRatio = 0.0f;
	m_fEXPRatio = 0.0f;

	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
}

// ----------------------------------------------------------------------------
// Name : ~CUIPlayerInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPlayerInfo::~CUIPlayerInfo()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part +44
	//m_rcHP.SetRect( 10, 55, 10, 63 );
	//m_rcMP.SetRect( 10, 69, 10, 77 );
	//m_rcEXP.SetRect( 10, 83, 10, 91 );

	// UI_REFORM :Su-won
	m_rcHP.SetRect( 111, 31, 111, 38 );
	m_rcMP.SetRect( 111, 44, 111, 51 );
	m_rcEXP.SetRect( 111, 57, 111, 64 );


	// Create inventory texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\PlayerInfo.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtBackground.SetUV( 0, 0, 140, 98, fTexWidth, fTexHeight );
	m_rtBackground.SetUV( 0, 0, 275, 90, fTexWidth, fTexHeight );			// UI_REFORM :Su-won

	// Tool tip
	//m_rtToolTipL.SetUV( 142, 53, 149, 76, fTexWidth, fTexHeight );
	//m_rtToolTipM.SetUV( 152, 53, 154, 76, fTexWidth, fTexHeight );
	//m_rtToolTipR.SetUV( 157, 53, 164, 76, fTexWidth, fTexHeight );
	// UI_REFORM :Su-won
	m_rtToolTipL.SetUV( 239, 253, 273, 272, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 274, 253, 318, 272, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 319, 253, 339, 272, fTexWidth, fTexHeight );


	// HP, MP, EXP, SP
	//m_rtHP.SetUV( 143, 0, 144, 8, fTexWidth, fTexHeight );
	//m_rtMP.SetUV( 148, 0, 149, 8, fTexWidth, fTexHeight );
	//m_rtEXP.SetUV( 153, 0, 154, 8, fTexWidth, fTexHeight );
	// UI_REFORM :Su-won
	m_rtHP.SetUV( 286, 220, 289, 228, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 291, 220, 294, 228, fTexWidth, fTexHeight );
	m_rtEXP.SetUV( 296, 220, 299, 228, fTexWidth, fTexHeight );

	// Web board button
	m_btnBoard.Create( this, CTString( "" ), 7, 27, 18, 18 );
	m_btnBoard.SetUV( UBS_IDLE, 142, 14, 160, 32, fTexWidth, fTexHeight );
	m_btnBoard.SetUV( UBS_CLICK, 161, 14, 179, 32, fTexWidth, fTexHeight );
	m_btnBoard.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBoard.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Messanger button
	m_btnMessanger.Create( this, CTString( "" ), 31, 27, 18, 18 );
	m_btnMessanger.SetUV( UBS_IDLE, 180, 14, 198, 32, fTexWidth, fTexHeight );
	m_btnMessanger.SetUV( UBS_CLICK, 199, 14, 217, 32, fTexWidth, fTexHeight );
	m_btnMessanger.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMessanger.CopyUV( UBS_IDLE, UBS_DISABLE );

	// System menu button
	m_btnSysMenu.Create( this, CTString( "" ), 61, 27, 18, 18 );
	m_btnSysMenu.SetUV( UBS_IDLE, 218, 14, 236, 32, fTexWidth, fTexHeight );
	m_btnSysMenu.SetUV( UBS_CLICK, 218, 33, 236, 51, fTexWidth, fTexHeight );
	m_btnSysMenu.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSysMenu.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Inventory button
	m_btnInven.Create( this, CTString( "" ), 91, 27, 18, 18 );
	m_btnInven.SetUV( UBS_IDLE, 142, 33, 160, 51, fTexWidth, fTexHeight );
	m_btnInven.SetUV( UBS_CLICK, 161, 33, 179, 51, fTexWidth, fTexHeight );
	m_btnInven.CopyUV( UBS_IDLE, UBS_ON );
	m_btnInven.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Character info button
	m_btnCharInfo.Create( this, CTString( "" ), 115, 27, 18, 18 );
	m_btnCharInfo.SetUV( UBS_IDLE, 180, 33, 198, 51, fTexWidth, fTexHeight );
	m_btnCharInfo.SetUV( UBS_CLICK, 199, 33, 217, 51, fTexWidth, fTexHeight );
	m_btnCharInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCharInfo.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnBoard.SetEnable(FALSE);
	m_btnMessanger.SetEnable(FALSE);
	m_btnSysMenu.SetEnable(FALSE);
	m_btnInven.SetEnable(FALSE);
	m_btnCharInfo.SetEnable(FALSE);


	// UI_REFORM :Su-won
	m_rcFace.SetRect(2, 8, 79, 85);
	m_rcQuickMenu.SetRect( 0, 0, 322, 27 );
	
	m_rtQuickMenu.SetUV( 0, 306, 321, 334, fTexWidth, fTexHeight );

	m_rtFace[TITAN].SetUV( 925, 428, 1003, 506, fTexWidth, fTexHeight );
	m_rtFace[KNIGHT].SetUV( 837, 344, 915, 422, fTexWidth, fTexHeight );
	m_rtFace[HEALER].SetUV( 750, 427, 828, 505, fTexWidth, fTexHeight );
	m_rtFace[MAGE].SetUV( 837, 428, 915, 506, fTexWidth, fTexHeight );
	m_rtFace[ROGUE].SetUV( 925, 256, 1003, 334, fTexWidth, fTexHeight );
	m_rtFace[SORCERER].SetUV( 925, 342, 1003, 420, fTexWidth, fTexHeight );
	m_rtFace[TOTAL_JOB].SetUV( 751, 341, 828, 418, fTexWidth, fTexHeight );

	for(int i=0; i<9; ++i)
	{
		m_btnQuickMenu[i].Create( this, CTString( "" ), 46 +i*26, 0, 22, 22);
		m_btnQuickMenu[i].SetUV( UBS_IDLE, i*26, 222, 21 +i*26 +1, 244, fTexWidth, fTexHeight );
		m_btnQuickMenu[i].SetUV( UBS_CLICK, i*26, 248, 21 +i*26 +1, 270, fTexWidth, fTexHeight );
		m_btnQuickMenu[i].CopyUV( UBS_IDLE, UBS_ON );
		m_btnQuickMenu[i].CopyUV( UBS_IDLE, UBS_DISABLE );
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMinI, pixMinJ );
	_pUIBuff->SetMyGoodBuffPos( m_nPosX + m_nWidth + 2, 2 );

	// UI_REFORM :Su-won
	ResetQuickMenuPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::Render()
{
	// Set player information texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_rcHP.Left, m_rcHP.Top, m_rcHP.Right, m_rcHP.Bottom,
										m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcMP.Left, m_rcMP.Top, m_rcMP.Right, m_rcMP.Bottom,
										m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcEXP.Left, m_rcEXP.Top, m_rcEXP.Right, m_rcEXP.Bottom,
										m_rtEXP.U0, m_rtEXP.V0, m_rtEXP.U1, m_rtEXP.V1,
										0xFFFFFFFF );

	// UI_REFORM :Su-won

	if( _pNetwork->MyCharacterInfo.hp<=0)
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_rcFace.Left, m_rcFace.Top, m_rcFace.Right, m_rcFace.Bottom,
										m_rtFace[TOTAL_JOB].U0, m_rtFace[TOTAL_JOB].V0, m_rtFace[TOTAL_JOB].U1, m_rtFace[TOTAL_JOB].V1,
										0xFFFFFFFF );
	}
	else
	{
		
		int iJob =_pNetwork->MyCharacterInfo.job;
		_pUIMgr->GetDrawPort()->AddTexture( m_rcFace.Left, m_rcFace.Top, m_rcFace.Right, m_rcFace.Bottom,
											m_rtFace[iJob].U0, m_rtFace[iJob].V0, m_rtFace[iJob].U1, m_rtFace[iJob].V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuickMenu.Left, m_rcQuickMenu.Top, m_rcQuickMenu.Right, m_rcQuickMenu.Bottom,
											m_rtQuickMenu.U0, m_rtQuickMenu.V0, m_rtQuickMenu.U1, m_rtQuickMenu.V1,
											0xFFFFFFFF );
	}

	/**********************
	// Web board button
	m_btnBoard.Render();

	// Messanger button
	m_btnMessanger.Render();

	// System menu button
	m_btnSysMenu.Render();

	// Inventory button
	m_btnInven.Render();

	// Character info button
	m_btnCharInfo.Render();
	**********************/

	// UI_REFORM :Su-won
	m_btnQuickMenu[0].Render();	m_btnQuickMenu[1].Render();	m_btnQuickMenu[2].Render();
	m_btnQuickMenu[3].Render();	m_btnQuickMenu[4].Render();	m_btnQuickMenu[5].Render();
	m_btnQuickMenu[6].Render();	m_btnQuickMenu[7].Render();	m_btnQuickMenu[8].Render();


	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Text in player information
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strLevel, m_nPosX + PLAYERINFO_LEVEL_CX,
										m_nPosY + PLAYERINFO_LEVEL_SY, 0xFFD3A7FF );
	_pUIMgr->GetDrawPort()->PutTextEx( _pNetwork->MyCharacterInfo.name,
										m_nPosX + PLAYERINFO_NAME_SX, m_nPosY + PLAYERINFO_NAME_SY );
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "HP" ), m_nPosX + PLAYERINFO_HP_SX,
										m_nPosY + PLAYERINFO_HP_SY, 0xF2F2F2B2 );
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "MP" ), m_nPosX + PLAYERINFO_HP_SX,
										m_nPosY + PLAYERINFO_MP_SY, 0xF2F2F2B2 );
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "EXP" ), m_nPosX + PLAYERINFO_HP_SX,
										m_nPosY + PLAYERINFO_EXP_SY, 0xF2F2F2B2 );
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strHP, m_nPosX + PLAYERINFO_HP_CX,
										m_nPosY + PLAYERINFO_HP_SY, 0xF2F2F2B2 );
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strMP, m_nPosX + PLAYERINFO_HP_CX,
										m_nPosY + PLAYERINFO_MP_SY, 0xF2F2F2B2 );
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strEXP, m_nPosX + PLAYERINFO_HP_CX,
										m_nPosY + PLAYERINFO_EXP_SY, 0xF2F2F2B2 );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render my buff
	_pUIBuff->RenderMyBuff();

	// Tool tip
	if( m_bShowToolTip )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top,
											m_rcToolTip.Left + 7, m_rcToolTip.Bottom,
											m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Left + 7, m_rcToolTip.Top,
											m_rcToolTip.Right - 7, m_rcToolTip.Bottom,
											m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Right - 7, m_rcToolTip.Top,
											m_rcToolTip.Right, m_rcToolTip.Bottom,
											m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Text in tool tip
		_pUIMgr->GetDrawPort()->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	
	// [090602: selo] 전체화면에 보여질 선택된 퀘스트를 랜더링 한다.
	RenderSelectedQuest();
	// [090608: selo] 전체화면에 보여질 시간제한 퀘스트의 남은시간을 랜더링 한다.
	RenderTimeAttackRemainTime();
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		extern INDEX	g_iEnterChat;
		int	nInfoX, nInfoY, nWidth;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case 0:		// Web board
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(B,Alt+B)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(Alt+B)" );

				m_btnBoard.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnBoard.GetWidth();
			}
			break;

		case 1:		// Messanger
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(F,Alt+F)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(Alt+F)" );

				m_btnMessanger.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnMessanger.GetWidth();
			}
			break;

		case 2:		// System menu
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Z,Alt+Z)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Z)" );

				m_btnSysMenu.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnSysMenu.GetWidth();
			}
			break;

		case 3:		// Inventory
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(E,Alt+E)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(Alt+E)" );

				m_btnInven.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnInven.GetWidth();
			}
			break;

		case 4:		// Character information
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(T,Alt+T)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(Alt+T)" );

				m_btnCharInfo.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnCharInfo.GetWidth();
			}
			break;
		}

		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY -= 1;

		if( nInfoX < _pUIMgr->GetMinI() )
			nInfoX = _pUIMgr->GetMinI();

		m_rcToolTip.SetRect( nInfoX, nInfoY - nInfoHeight, nInfoX + nInfoWidth, nInfoY );
	}
}

// ----------------------------------------------------------------------------
// Name : UpdateAllInfos()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::UpdateAllInfos()
{
	m_fHPRatio = _pNetwork->MyCharacterInfo.hp / (FLOAT)( _pNetwork->MyCharacterInfo.maxHP );
	if( m_fHPRatio > 1.0f ) m_fHPRatio = 1.0f;
	m_fMPRatio = _pNetwork->MyCharacterInfo.mp / (FLOAT)( _pNetwork->MyCharacterInfo.maxMP );
	if( m_fMPRatio > 1.0f ) m_fMPRatio = 1.0f;
	m_fEXPRatio = _pNetwork->MyCharacterInfo.curExp / (FLOAT)( _pNetwork->MyCharacterInfo.needExp );
	FLOAT	fEXPBarRatio = m_fEXPRatio;
	if( fEXPBarRatio > 1.0f ) fEXPBarRatio = 1.0f;
	else if( fEXPBarRatio < 0.0f ) fEXPBarRatio = 0.0f;

	m_rcHP.Right = m_rcHP.Left + PLAYERINFO_BAR_WIDTH * m_fHPRatio;
	m_rcMP.Right = m_rcMP.Left + PLAYERINFO_BAR_WIDTH * m_fMPRatio;
	m_rcEXP.Right = m_rcEXP.Left + PLAYERINFO_BAR_WIDTH * fEXPBarRatio;

	m_strLevel.PrintF( "%d", _pNetwork->MyCharacterInfo.level );
	m_strHP.PrintF( "%d/%d", _pNetwork->MyCharacterInfo.hp, _pNetwork->MyCharacterInfo.maxHP );
	m_strMP.PrintF( "%d/%d", _pNetwork->MyCharacterInfo.mp, _pNetwork->MyCharacterInfo.maxMP );
	m_strEXP.PrintF( "%.2f%%", m_fEXPRatio * 100.0f );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::UpdateHPInfo()
{
	m_fHPRatio = _pNetwork->MyCharacterInfo.hp / (FLOAT)( _pNetwork->MyCharacterInfo.maxHP );
	if( m_fMPRatio > 1.0f )
	{
		m_fMPRatio = 1.0f;
		_pNetwork->MyCharacterInfo.mp = _pNetwork->MyCharacterInfo.maxMP;
	}

	m_rcHP.Right = m_rcHP.Left + PLAYERINFO_BAR_WIDTH * m_fHPRatio;
	m_strHP.PrintF( "%d/%d", _pNetwork->MyCharacterInfo.hp, _pNetwork->MyCharacterInfo.maxHP );
}

// ----------------------------------------------------------------------------
// Name : UpdateEXPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::UpdateEXPInfo()
{
	m_fEXPRatio = _pNetwork->MyCharacterInfo.curExp / (FLOAT)( _pNetwork->MyCharacterInfo.needExp );
	FLOAT	fEXPBarRatio = m_fEXPRatio;
	if( fEXPBarRatio > 1.0f ) fEXPBarRatio = 1.0f;
	else if( fEXPBarRatio < 0.0f ) fEXPBarRatio = 0.0f;

	m_rcEXP.Right = m_rcEXP.Left + PLAYERINFO_BAR_WIDTH * fEXPBarRatio;

	m_strEXP.PrintF( "%.2f%%", m_fEXPRatio * 100.0f );
}

// ----------------------------------------------------------------------------
// Name : ToggleUIWebBoard()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIWebBoard()
{
	_pUIMgr->GetWebBoard()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUIMessanger()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIMessanger()
{
	_pUIMgr->GetMessenger()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUISysMenu()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUISysMenu()
{
	_pUIMgr->GetSystemMenu()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUIInventory()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIInventory()
{
	_pUIMgr->GetInventory()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUICharacterInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUICharacterInfo()
{
	_pUIMgr->GetCharacterInfo()->ToggleVisible();
}

// ----------------------------------------------------------------------------
//  [6/2/2009 selo]
// Name : RenderSelectedQuest()
// Desc : 전체화면에 표시할 퀘스트 내용 랜더한다.
//		  정보는 QuestBookList 에서 얻어온다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderSelectedQuest()
{	
	int nPosX = _pUIMgr->GetDrawPort()->GetWidth() - 20;
	int nPosY = 280;
	CTString strTemp;

	std::list<int>::const_iterator iter = _pUIMgr->GetQuestBookList()->GetSelectedQuestList().begin();
	while( iter != _pUIMgr->GetQuestBookList()->GetSelectedQuestList().end() )
	{
		CQuestDynamicData *pQuestDD = NULL;
		if( !_pUIMgr->GetQuestBookList()->isRaidMessage(*iter) )
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(*iter);	
		}
		else
		{
			CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(*iter));
			pQuestDD = &qdd;	
		}
		
		strTemp.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), pQuestDD->GetTitleDesc( 0 ));
		_pUIMgr->GetDrawPort()->PutTextExRX(strTemp, nPosX, nPosY, pQuestDD->GetColorTitleDesc( 0 ));
		nPosY += 12;			
		strTemp.PrintF("%s %s", pQuestDD->GetStatusDesc(0), pQuestDD->GetStatusDesc(1));
		_pUIMgr->GetDrawPort()->PutTextExRX(strTemp, nPosX, nPosY, pQuestDD->GetColorStatusDesc(0));
		nPosY += 12;		

		++iter;
	}	
	
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
//  [6/8/2009 selo]
// Name : RenderTimeAttackRemainTime()
// Desc : 전제화면에 표시할 타임어택 남은시간 랜더한다.
//		  정보는 QuestBookList 에서 얻어온다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderTimeAttackRemainTime()
{
	DOUBLE dRemainTime = _pUIMgr->GetQuestBookList()->GetTimeAttackRemainTime();

	if( dRemainTime >= 0 )
	{
		int nPosX = _pUIMgr->GetDrawPort()->GetWidth() - 20;
		int nPosY = 240;
		CTString strTemp;

		strTemp.PrintF(_S(2514, "%d초"), (int)dRemainTime);
		_pUIMgr->GetDrawPort()->PutTextExRX(strTemp, nPosX, nPosY, 0xFFFFFFFF);
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPlayerInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Buff mouse message
	if( ( wmsgResult = _pUIBuff->MouseMessageMyBuff( pMsg ) ) != WMSG_FAIL )
	{
		if( wmsgResult == WMSG_COMMAND )
			_pUIMgr->RearrangeOrder( UI_PLAYERINFO, TRUE );

		return wmsgResult;
	}

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// Web board button
			if( m_btnBoard.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 0 );
				return WMSG_SUCCESS;
			}
			// Messanger button
			else if( m_btnMessanger.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 1 );
				return WMSG_SUCCESS;
			}
			// System menu button
			else if( m_btnSysMenu.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 2 );
				return WMSG_SUCCESS;
			}
			// Inventory button
			else if( m_btnInven.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 3 );
				return WMSG_SUCCESS;
			}
			// Character info button
			else if( m_btnCharInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 4 );
				return WMSG_SUCCESS;
			}

			for(int i=0; i<9; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ShowQuickMenuToolTip( TRUE, i);
					return WMSG_SUCCESS;
				}
			}

			// Hide tool tip
			ShowToolTip( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				// Web board button
				if( m_btnBoard.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Messanger button
				else if( m_btnMessanger.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// System menu button
				else if( m_btnSysMenu.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Character info button
				else if( m_btnCharInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Inventory button
				else if( m_btnInven.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else
				{
					//0806 kwon
					CEntity			*penPlEntity;
					CPlayerEntity	*penPlayerEntity;
					penPlEntity = CEntity::GetPlayerEntity( 0 );
					penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
					penPlayerEntity->SetTargetMe();
				}

				_pUIMgr->RearrangeOrder( UI_PLAYERINFO, TRUE );
				return WMSG_SUCCESS;
			}

			for(int i=0; i<9; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					_pUIMgr->RearrangeOrder( UI_PLAYERINFO, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{
				// If holding button doesn't exist
				if( _pUIMgr->GetHoldBtn().IsEmpty() )
				{
					// If player info isn't focused
					if( !IsFocused() )
						return WMSG_FAIL;

					// Web board button
					if( ( wmsgResult = m_btnBoard.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult = WMSG_COMMAND )
						{
							ToggleUIWebBoard();
						}

						return WMSG_SUCCESS;
					}
					// Messanger button
					else if( ( wmsgResult = m_btnMessanger.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult = WMSG_COMMAND )
						{
							ToggleUIMessanger();
						}

						return WMSG_SUCCESS;
					}
					// System menu button
					else if( ( wmsgResult = m_btnSysMenu.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							ToggleUISysMenu();
						}

						return WMSG_SUCCESS;
					}
					// Character info button
					else if( ( wmsgResult = m_btnCharInfo.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							ToggleUICharacterInfo();
						}

						return WMSG_SUCCESS;
					}
					// Inventory button
					else if( ( wmsgResult = m_btnInven.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							ToggleUIInventory();
						}

						return WMSG_SUCCESS;
					}
				}
				// If holding button exists
				else
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();
				}

				return WMSG_SUCCESS;
			}

			for(int i=0; i<9; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					OpenQuickMenu(i);
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;

			for(int i=0; i<9; ++i)
			{
				//QuickMenu 범위로 체크
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ProcessShortCut()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIPlayerInfo::ProcessShortCut( MSG *pMsg )
{
	if( _pUIMgr->GetBilling()->IsLock() ) return FALSE ; 
	if( pMsg->wParam == VK_TAB )
	{
		_pUIMgr->GetInventory()->ToggleVisible();
		return TRUE;
	}
	else
	{
		INDEX			iScanCode = ( pMsg->lParam >> 16 ) & 0x1FF;
		extern INDEX	g_iEnterChat;
		extern UBYTE	_abKeysPressed[256];

		// Quick slot
		if( _abKeysPressed[KID_LALT] )
		{
#ifndef Pet_IMPROVEMENT_1ST
			//Alt+F1~F3을 눌렀을 때 퀵슬롯 페이지 전환
			if( iScanCode >= 59 && iScanCode <= 61 )
			{
				_pUIMgr->GetQuickSlot()->ChangePage( iScanCode - 59 );
				return TRUE;
			}
#endif
			//Alt+1~3을 눌렀을 때 퀵슬롯 페이지 전환
#ifdef Pet_IMPROVEMENT_1ST
			//[070604: Su-won] 1차 펫 기능 개선
			//Enter 채팅과 상관없이 항상 Alt+숫자로 퀵슬롯 페이지 전환
			if( iScanCode >= 2 && iScanCode <= 4 )
#else
			if( g_iEnterChat && iScanCode >= 2 && iScanCode <= 4 )
#endif
			{
				_pUIMgr->GetQuickSlot()->ChangePage( iScanCode - 2 );
				return TRUE;
			}
		}
		else if( _abKeysPressed[KID_LCONTROL] )
		{
			switch( iScanCode )
			{
			case 2:
			case 3:
			case 4:
			case 5:			
				_pUIMgr->GetSummonFirst()->SetCommand( iScanCode - 2 );
				break;			
			case 6:
				_pUIMgr->GetSummonFirst()->UseSkill();
				break;
/*
			case 7:
			case 8:
			case 9:			
			case 10:			
				_pUIMgr->GetSummonSecond()->SetCommand( iScanCode - 7 );
				break;			
			case 11:
				_pUIMgr->GetSummonSecond()->UseSkill();
				break;
*/
			case 16:  // 'q'    
			case 17:  // 'w'    
			case 18:  // 'e'    
			case 19:  // 'r'
				_pUIMgr->GetSummonSecond()->SetCommand( iScanCode - 16 );
				break;       
			case 20:  // 't'
				_pUIMgr->GetSummonSecond()->UseSkill();
				break;

			default:
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			if(g_iCountry == JAPAN)
			{
				if( !g_iEnterChat)
				{
					if( iScanCode >= 59 && iScanCode <= 68 )
					{
						_pUIMgr->GetQuickSlot()->UseQuickSlot( iScanCode - 59 );
						return TRUE;
					}
				} 
				
			}
			else if( iScanCode >= 59 && iScanCode <= 68 )
			{
				_pUIMgr->GetQuickSlot()->UseQuickSlot( iScanCode - 59 );
				return TRUE;
			}
			else if( iScanCode == 87 || iScanCode == 88 )
			{
				_pUIMgr->GetQuickSlot()->UseQuickSlot( iScanCode - 77 );
				return TRUE;
			}

			//if( g_iEnterChat && iScanCode >= 2 && iScanCode <= 11 )
			if( g_iEnterChat && iScanCode >= 2 && iScanCode <= 13 )
			{
				int	nUIIndex = _pUIMgr->GetUIIndexByOrder( 0 );
				if( _pUIMgr->GetUI( nUIIndex )->IsEditBoxFocused() )
					return FALSE;

				_pUIMgr->GetQuickSlot()->UseQuickSlot( iScanCode - 2 );
				return TRUE;
			}
		}

		// Short cut
		if( _abKeysPressed[KID_LALT] || g_iEnterChat )
		{
			if( !_abKeysPressed[KID_LALT] )
			{
				int	nUIIndex = _pUIMgr->GetUIIndexByOrder( 0 );
				if( _pUIMgr->GetUI( nUIIndex )->IsEditBoxFocused() )
					return FALSE;
			}

			switch( iScanCode )
			{
			case 18:		// 'e' - inventory
				_pUIMgr->GetInventory()->ToggleVisible();
				break;

			case 17:		// 'w' - map
				_pUIMgr->GetMap()->ToggleVisible();
				break;

			case 20:		// 't' - status
				_pUIMgr->GetCharacterInfo()->ToggleVisibleStatus();
				break;

			case 31:		// 's' - skill
				_pUIMgr->GetCharacterInfo()->ToggleVisibleSkill();
				break;

			case 30:		// 'a' - action
				_pUIMgr->GetCharacterInfo()->ToggleVisibleAction();
				break;

			case 46:		// 'c' - social
				_pUIMgr->GetCharacterInfo()->ToggleVisibleSocial();
				break;

			case 34:		// 'g' - group
				_pUIMgr->GetCharacterInfo()->ToggleVisibleGroup();
				break;

			case 16:		// 'q' - quest
				_pUIMgr->GetQuestBookList()->ToggleVisible();
				break;

			case 44:		// 'z' - system menu
				_pUIMgr->GetSystemMenu()->ToggleVisible();
				break;

			case 48:		// 'b' - web board
				_pUIMgr->GetWebBoard()->ToggleVisible();
				break;

			case 33:		// 'f' - messanger
				_pUIMgr->GetMessenger()->ToggleVisible();
				break;

			case 45:		// 'x' - all UIs
				_pUIMgr->ToggleVisibleUIs();
				break;

			case 32:		// 'd' - pet info
				_pUIMgr->GetPetInfo()->ToggleVisible();
				break;

			case 25:		// 'P' - WildPet info
				if(_pNetwork->_WildPetInfo.bIsActive) {
					_pUIMgr->GetWildPetInfo()->ToggleVisible();
				}
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI_REFORM :Su-won
void CUIPlayerInfo::ResetQuickMenuPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	m_rcQuickMenu.SetRect( 0, pixMaxJ-28, 322, pixMaxJ );

	for(int i=0; i<9; ++i)
	{
		m_btnQuickMenu[i].SetPosY(pixMaxJ -25);
	}
}

void CUIPlayerInfo::ShowQuickMenuToolTip(BOOL bShow, int nToolTipID)
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		extern INDEX	g_iEnterChat;
		int	nInfoX, nInfoY, nWidth;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case 0:		// Character information
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(T,Alt+T)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(Alt+T)" );
			}
			break;
		case 1:		// Inventory
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(E,Alt+E)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(Alt+E)" );
			}
			break;
		case 2:		// Quest Book
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S(1700, "퀘스트북" ), "(Q,Alt+Q)" );
				else
					m_strToolTip.PrintF( "%s %s", _S(1700, "퀘스트북" ), "(Alt+Q)" );
			}
			break;
		case 3:		// Pet Info
			{
				if(_pNetwork->_WildPetInfo.bIsActive)
				{
					if( g_iEnterChat )
						m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(D,Alt+P)" );
					else
						m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(Alt+P)" );

				}else
				{
					if( g_iEnterChat )
						m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(D,Alt+D)" );
					else
						m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(Alt+D)" );
				}
			}
			break;
		case 4:		// Messanger
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(F,Alt+F)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(Alt+F)" );
			}
			break;
		case 5:		// Web board
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(B,Alt+B)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(Alt+B)" );
			}
			break;
		case 6:		// System menu
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Z,Alt+Z)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Z)" );
			}
			break;
		case 7:		// Help
			{
				m_strToolTip.PrintF( "%s", _S(284, "도움말"));
			}
			break;
		case 8:		// Cash Shop
			{
				m_strToolTip.PrintF( "%s", _S(2572, "아이리스 상점"));
			}
			break;

		}

		m_btnQuickMenu[nToolTipID].GetAbsPos( nInfoX, nInfoY );
		nWidth = m_btnQuickMenu[nToolTipID].GetWidth();

		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY -= 1;

		if( nInfoX < _pUIMgr->GetMinI() )
			nInfoX = _pUIMgr->GetMinI();

		m_rcToolTip.SetRect( nInfoX, nInfoY - nInfoHeight, nInfoX + nInfoWidth, nInfoY );
	}
}

void CUIPlayerInfo::OpenQuickMenu(int nMenu)
{
	switch(nMenu)
	{
		//스테이터스
	case 0:
		ToggleUICharacterInfo();
		break;
		//인벤토리
	case 1:
		ToggleUIInventory();
		break;
		//퀘스트북
	case 2:
		_pUIMgr->GetQuestBookList()->ToggleVisible();
		break;
		//펫 정보창
	case 3:
		{
			if(_pNetwork->_WildPetInfo.bIsActive)
			{
				_pUIMgr->GetWildPetInfo()->ToggleVisible();
			}else
			{
				_pUIMgr->GetPetInfo()->ToggleVisible();
			}
		}
		
		break;
		//메신저
	case 4:
		ToggleUIMessanger();
		break;
		//게시판
	case 5:
		ToggleUIWebBoard();
		break;
		//시스템
	case 6:
		ToggleUISysMenu();
		break;
		//도움말
	case 7:
		_pUIMgr->GetHelp()->OpenHelp();
		break;
		//아이리스 상점
	case 8:
		{
			if( g_iCountry == TAIWAN )
			{
				if( !_pUIMgr->GetBilling()->IsVisible() )
					_pNetwork->BillInfoUserInfoReq();
			}
			else if( g_iCountry == TAIWAN2 )
			{
				if( !_pUIMgr->GetBillItem()->IsVisible() )							
					_pNetwork->BillInfoUserInfoReq();
			}
			else
			{
				if(_pUIMgr->GetCashShop()->IsEnabled()&&_pUIMgr->GetCashShop()->IsVisible()) _pUIMgr->GetCashShop()->CloseCashShop();
				else{
					_pUIMgr->GetCashShop()->OpenCashShop();
					_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_BALANCE_REQ);														
				}
			}
		}
		break;
	}
}
// UI_REFORM :Su-won
/////////////////////////////////////////////////////////////////////////////////////////////////////////