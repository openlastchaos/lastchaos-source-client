#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameState.h>

//강동민 수정 시작 클로즈 준비 작업	08.10
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Interface/UINotice.h>
#include <Engine/SlaveInfo.h>
//강동민 수정 끝 클로즈 준비 작업		08.10

#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UISummon.h>


// ----------------------------------------------------------------------------
// Name : CUISystemMenu()
// Desc : Constructor
// ----------------------------------------------------------------------------
#ifdef RESTART_GAME
CUISystemMenu::CUISystemMenu() : m_bRestartGame(FALSE), m_llStartTime(0)
#else
CUISystemMenu::CUISystemMenu()
#endif
{
}

// ----------------------------------------------------------------------------
// Name : ~CUISystemMenu()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISystemMenu::~CUISystemMenu()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );

	// Create inventory texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SystemMenu.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewOption.tex" ) );
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtBackground.SetUV( 0, 0, 216, 114, fTexWidth, fTexHeight );
	m_rtBackground.SetUV( 270, 104, 448, 295, fTexWidth, fTexHeight );

	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// Close button
	//m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	//m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	//m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.Create( this, CTString( "" ), 153, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Option button
	//m_btnOption.Create( this, _S( 283, "옵션" ), 31, 39, 63, 21 );
	//m_btnOption.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnOption.Create( this, _S( 283, "옵션" ), 9, 44, 78, 22);
	m_btnOption.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Help button
	//m_btnHelp.Create( this, _S( 284, "도움말" ), 121, 39, 63, 21 );
	//m_btnHelp.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnHelp.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnHelp.Create( this, _S( 284, "도움말" ), 91, 44, 78, 22 );
	m_btnHelp.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnHelp.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_DISABLE );

	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 146, 21);
	rcRight.SetRect(146, 0, 160, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);

	// Restart button
	//m_btnRestart.Create( this, _S( 285, "리스타트" ), 31, 73, 63, 21 );
	//m_btnRestart.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnRestart.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnRestart.Create( this, _S(4195, "서버 선택 창으로"), 9, 130, 160, 22);
	m_btnRestart.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnRestart.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnRestart.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnRestart.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnRestart.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnRestart.SetNewType(TRUE);

	// Exit button
	//m_btnExit.Create( this, _S( 286, "게임종료" ), 121, 73, 63, 21 );
	//m_btnExit.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnExit.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnExit.Create( this, _S( 286, "게임종료" ), 9, 158, 160, 22 );
	m_btnExit.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnExit.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnExit.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnExit.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnExit.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnExit.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnExit.SetNewType(TRUE);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - _pUIMgr->GetRadar()->GetWidth() - GetWidth(),
			pixMinJ + _pUIMgr->GetRadar()->GetHeight() + GetHeight() );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Render()
{
	// Set system menu texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Set Button texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );

	// Close button
	m_btnClose.Render();

	// Option button
	m_btnOption.Render();

	// Help button
	m_btnHelp.Render();

	// Restart button
	m_btnRestart.Render();

	// Exit button
	m_btnExit.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Text in system menu
	//_pUIMgr->GetDrawPort()->PutTextEx( _S( 287, "시스템 메뉴" ), m_nPosX + SYSMENU_TITLE_TEXT_OFFSETX,
	//									m_nPosY + SYSMENU_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 287, "시스템 메뉴" ), m_nPosX + m_nWidth/2,
										m_nPosY + 18, 0xFFFFFFFF );


	CTString strServerInfo;
	strServerInfo.PrintF("%s - %d", _pUIMgr->GetSelServer()->GetServerGroupName(_pNetwork->m_iServerGroup), _pNetwork->m_iServerCh );

	_pUIMgr->GetDrawPort()->PutTextExCX( _S(4167, "접속중인 서버"), m_nPosX + m_nWidth/2,
										m_nPosY + 82, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->PutTextExCX( strServerInfo, m_nPosX +m_nWidth/2,
										m_nPosY +99 , 0xFFFFFFFF );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::ToggleVisible()
{
	BOOL	bVisible = !IsVisible();
	_pUIMgr->RearrangeOrder( UI_SYSTEMMENU, bVisible );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISystemMenu::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

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
				_pUIMgr->SetMouseCursorInsideUIs();

			// Move system menu
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Option button
			else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Help button
			else if( m_btnHelp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Restart button
			else if( m_btnRestart.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Exit button
			else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Option button
				if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Help button
				if( m_btnHelp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Restart button
				if( m_btnRestart.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Exit button
				if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_SYSTEMMENU, TRUE );
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

				// If system menu isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						_pUIMgr->RearrangeOrder( UI_SYSTEMMENU, FALSE );

					return WMSG_SUCCESS;
				}
				// Option button
				else if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						OpenOption();
					}

					return WMSG_SUCCESS;
				}
				// Help button
				else if( ( wmsgResult = m_btnHelp.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						OpenHelp();
					}

					return WMSG_SUCCESS;
				}
				// Restart button
				else if( ( wmsgResult = m_btnRestart.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						Restart();
					}

					return WMSG_SUCCESS;
				}
				// Exit button
				else if( ( wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						Exit();
					}

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
			{
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : OpenOption()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::OpenOption()
{
	_pUIMgr->GetOption()->OpenOption();
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc : 도움말 열기.
// ----------------------------------------------------------------------------
void CUISystemMenu::OpenHelp()
{	
#ifdef HELP_SYSTEM_1
	_pUIMgr->GetHelp()->OpenHelp();		
#else
	_pUIMgr->GetHelpOld()->OpenHelp();
#endif
}

// ----------------------------------------------------------------------------
// Name : Restart()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Restart()
{
	if( _pUIMgr->DoesMessageBoxExist( MSGCMD_SYSMENU_RESTART ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "시스템 메뉴" ), UMBS_YESNO,
								UI_SYSTEMMENU, MSGCMD_SYSMENU_RESTART );
	MsgBoxInfo.AddString( _S( 451, "게임을 재시작 하시겠습니까?" ) );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

extern BOOL bCalcNotChecked;

// ----------------------------------------------------------------------------
// Name : Restart_Internal()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Restart_Internal()
{
	_pUIMgr->SetUIGameState( UGS_GAMELOADING );

	// Reset chatting
	_pUIMgr->GetChatting()->ResetChatting();

	// Character state flags
	_pUIMgr->ResetCSFlag();

	_pNetwork->MyCharacterInfo.money = 0;
	_pNetwork->MyCharacterInfo.bExtension = FALSE;
	_pNetwork->MyCharacterInfo.sbEvocationType = -1;

	// Reset inventory
	for( int i = 0; i < WEAR_TOTAL; ++i )
		_pNetwork->pMyCurrentWearing[i] = NULL;

	for(int tab = 0; tab < INVEN_SLOT_TAB; ++tab )
	{
		for(int row = 0; row < INVEN_SLOT_ROW_TOTAL; ++row )
		{
			for( int col = 0; col < INVEN_SLOT_COL; ++col )
			{
				_pNetwork->MySlotItem[tab][row][col].Init();
				_pUIMgr->GetInventory()->InitInventory( tab, row, col, -1, -1, -1 );
			}
		}
	}

	// 선물시스템 선물받음 초기화
	_pUIMgr->GetQuickSlot()->SetGiftRecv(FALSE);

	_pUIMgr->GetPersonalShop()->ResetShop();
	_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
	_pUIMgr->SetCSFlagOff( CSF_MOONSTONE );
	_pUIMgr->SetCSFlagOff( CSF_MOUNT_HUNGRY );

	_pUIMgr->GetSummonFirst()->ResetSummon();
	_pUIMgr->GetSummonSecond()->ResetSummon();
	SlaveInfo().InitSkillIndex();

	// Reset buff
	_pUIBuff->ResetMyBuff();

	// Reset party
	_pUIMgr->GetParty()->Init();

	// Reset quest
	CQuestSystem::Instance().ClearAllDynamicData();
	CQuestSystem::Instance().ClearAllAllowList();
	//TODO : NewQuestSystem
	_pUIMgr->GetQuestBookNew()->ClearQuestList();
	_pUIMgr->GetQuestBookList()->ClearQuestList();
	_pUIMgr->GetQuestBookComplete()->ClearQuestList();
	_pUIMgr->GetQuestBookContent()->ClearQuestList();

	// Reset Notice
	_pUIMgr->GetNotice()->Clear();

	// Reset Guild
	_pUIMgr->GetGuild()->ResetGuild();
	_pUIMgr->GetGuild()->ClearMemberList();
	_pUIMgr->GetSiegeWarfare()->CloseSiegeWarfare();

	_pUIMgr->GetHelper()->ResetHelper();
	_pUIMgr->GetHelper()->ClearHelperList();
	_pUIMgr->GetWildPetInfo()->AIClear();

	_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
	_pNetwork->ClearPetList();	
	_pNetwork->_PetTargetInfo.Init();
	_pNetwork->_SlaveTargetInfo[0].Init();
	_pNetwork->_SlaveTargetInfo[1].Init();
	_pNetwork->_WildPetInfo.Init();

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearMultiTargets();
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReturnSorcerer();

	extern BOOL	_bLoginProcess;
	_bLoginProcess = TRUE;
	_pGameState->GetGameMode() = CGameState::GM_NONE;
	_pUIMgr->GetGame()->StopGame();

	// 이기환 수정 ( 12. 1 ) : 로딩화면 변경에 따라 존 값 변경 ( -1 : [re]start )
	g_slZone = -1; 
	g_bFirstIntoWorld = TRUE;
	// ... cpp2angel End 

	_pUIMgr->GetGame()->LoginGame( LOGIN_WORLD );

	_pGameState->SelectedSlot()		= 0;
	_pGameState->m_pEntModels[0]	= NULL;
	_pGameState->m_pEntModels[1]	= NULL;
	_pGameState->m_pEntModels[2]	= NULL;
	_pGameState->m_pEntModels[2]	= NULL;
	_pNetwork->m_ubGMLevel			= 0;
	_pNetwork->m_bSingleMode		= FALSE;
	
	const int	iCameraEntityID	= 1628;
	CEntity		*pCameraEntity = NULL;
	CEntity		*pEntity = NULL;
	BOOL bExist = _pNetwork->ga_World.EntityExists( iCameraEntityID, pCameraEntity );
	if(bExist)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
	}

	// 꽃놀이 나무 초기화
	_pUIMgr->GetFlowerTree()->ClearList();
	_pUIMgr->GetTargetInfo()->ShowPKTargetHP(FALSE);
	
	_pUIMgr->SetUIGameState( UGS_LOGIN );	
	_pUIMgr->GetLogin()->SetPWFocus();

	//구매 대행 정산 리스트 체크 플래그
	bCalcNotChecked = TRUE;
}

// ----------------------------------------------------------------------------
// Name : Exit()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Exit()
{
	if( _pUIMgr->DoesMessageBoxExist( MSGCMD_SYSMENU_EXIT ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "시스템 메뉴" ), UMBS_YESNO,
								UI_SYSTEMMENU, MSGCMD_SYSMENU_EXIT );
	MsgBoxInfo.AddString( _S( 288, "게임을 종료하시겠습니까?" ) );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SYSMENU_EXIT:
		if( bOK )
		{
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;	
		}
		break;

	case MSGCMD_SYSMENU_RESTART:
		if( bOK )
		{
#ifdef RESTART_GAME
			SetRestartGameValue(TRUE);
			m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
#else
			Restart_Internal();
#endif
		}
		break;
	}
}
