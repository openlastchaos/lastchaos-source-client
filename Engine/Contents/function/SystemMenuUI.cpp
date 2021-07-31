#include "stdh.h"
#include "SystemMenuUI.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Contents/function/TeleportUI.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Login/UIServerSelect.h>
#include <engine/Contents/Login/UILoginNew.h>
#include <Engine/Contents/Base/UIAuctionNew.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Contents/function/HelperUI.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Contents/function/AffinityInfoUI.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Contents/function/WareHouseUI.h>
#include <Engine/Contents/Login/UICharacterSelect.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Contents/function/HelpWebUI.h>
#include <Engine/Contents/function/News_Web_UI.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Interface/UIGuildBattle.h>

extern INDEX g_iXPosInSystemMenu;
extern INDEX g_iYPosInSystemMenu;

extern CDrawPort* _pdpMain;
extern BOOL g_bAutoRestart;

extern INDEX	g_iCountry;

// 유물 아이템 인덱스
#define	DEF_RELIC_ITEM1 10951
#define	DEF_RELIC_ITEM2 10952
#define	DEF_RELIC_ITEM3 10953

CSystemMenuUI::CSystemMenuUI()
	: m_pMoveArea(NULL)
	, m_pTextConnetingServerName(NULL)
{
	for (int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	if (IsGamigo(g_iCountry) == TRUE || g_iCountry == KOREA)
	{
		m_bCharacterMove = FALSE;
		m_llStartTime = 0;
	}

	setInherit(false);
}

void CSystemMenuUI::PressOption()
{
	UIMGR()->GetOption()->ToggleOption();
}

void CSystemMenuUI::PressHelp()
{
	UIMGR()->GetHelpWebUI()->OpenUI();
}

void CSystemMenuUI::PressGoCharSel()
{
	m_bCharacterMove = TRUE;
	_reStart();
}

void CSystemMenuUI::PressGoLogin()
{
	m_bCharacterMove = FALSE;
	_reStart();
}


ENGINE_API void CSystemMenuUI::Restart_Internal()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	closeUI();

	pUIManager->GetChattingUI()->ResetChatting();

	// Character state flags
	pUIManager->ResetCSFlag();

	_pNetwork->RestartGame();

	// 선물시스템 선물받음 초기화
	pUIManager->GetQuickSlot()->SetGiftRecv(FALSE);

	SlaveInfo().InitSkillIndex();

	// Reset buff
	_pUIBuff->ResetMyBuff();

	// Reset quest
	CQuestSystem::Instance().ClearAllDynamicData();
	CQuestSystem::Instance().ClearAllAllowList();
	//TODO : NewQuestSystem
	GAMEDATAMGR()->GetQuest()->ClearQuest();
	GAMEDATAMGR()->GetQuest()->ClearQuestList();

	// [090803 sora] 리스타트할때 선택했던 퀘스트 리스를 지워준다
	pUIManager->GetPlayerInfo()->ClearSelectedQuest();

	// Reset Notice
	pUIManager->GetNotice()->Clear();
	GAMEDATAMGR()->GetNotice()->clear();

	pUIManager->GetSiegeWarfare()->CloseSiegeWarfare();
	pUIManager->GetGuildBattle()->Clear();

	// 펫 AI 클리어
	pUIManager->GetWildPetInfoUI()->AIClear();

	// Reset Memory Scroll
	pUIManager->GetTeleport()->ClearTeleportList();
	pUIManager->GetTeleport()->SetUseTime(0);

	pUIManager->GetAffinityInfo()->AffinityReset();

	// clear all data in lacaball. [1/25/2011 rumist]
	pUIManager->GetLacaBall()->InitLacaballData();

	pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary(-1);	

	// [11/3/2010 kiny8216] Face Change 모드 초기화
	pUIManager->GetInitJob()->SetFaceDecoMode( FALSE );	

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReturnSorcerer();

	// 꽃놀이 나무 초기화
	pUIManager->GetFlowerTree()->ClearList();
	pUIManager->GetTargetInfoUI()->ShowPKTargetHP(FALSE);

	extern BOOL	_bLoginProcess;
	_bLoginProcess = TRUE;
	_pGameState->GetGameMode() = CGameState::GM_NONE;
	pUIManager->GetGame()->StopGame();

	pUIManager->GetSimplePlayerInfo()->StopDungeonTime();

	GameDataManager* pGameData = GameDataManager::getSingleton();

	pGameData->GetExpressData()->ClearNPCInfo();
	pGameData->GetExpressData()->SetNoticeFlag(0);

	if (pUIManager->GetRadar())
		pUIManager->GetRadar()->OffExpressNotice();

	// 구매대행 초기화
	pUIManager->GetAuction()->reset();
	pUIManager->GetAuction()->CloseAuction();
	pGameData->GetAuction()->reset();

	// New skill tree UI reset
	pUIManager->GetSkillNew()->InitArrayData();

	// 개인 창고
	pUIManager->GetWareHouseUI()->SetUseTime(0);
	pGameData->GetSyndicate()->ResetSyndiInfo();

	// 캐릭터 선택창 초기화 (간혈적으로 초기화 안되는 문제가 보고됨. 방어코드로 추가)
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		_pGameState->ClearCharacterSlot();
		_pGameState->SetCreatableNightShadow( FALSE );
	}

	pUIManager->ResetHoldBtn();

	for (int i = 0; i < _pNetwork->wo_iNumOfSkill; ++i)
	{
		CSkill* pSkill = &_pNetwork->GetSkillData(i);

		if (pSkill == NULL)
			continue;

		if (pSkill->GetFlag() & SF_TOGGLE)
			pSkill->SetToggle(false);
	}

	CItemCollectionData::ClearData();
	TOOLTIPMGR()->clearTooltip();

	// 커스텀 타이틀 정보 삭제
	CustomTitleData::clearCustomItemInfo();

	pUIManager->GetNewsWebUI()->clear_param();

	CNews* pNews = GAMEDATAMGR()->GetNews();

	if (pNews != NULL)
		pNews->ClearData();
}

void CSystemMenuUI::CancelRestart()
{
	_UIAutoHelp->ClearGMNNotice();
	ResetRestartTime();
	_pGameState->SetRestartGameValue(FALSE);
	resetUI();
}

void CSystemMenuUI::ReStartNow()
{
	if ( m_bCharacterMove == FALSE)
	{
		STAGEMGR()->SetNextStage(eSTAGE_GAMEEND);
		return;
	}

	_pNetwork->SendRestartGame();	
}

void CSystemMenuUI::LogoutRelicWarring()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_LOGOUT_RELIC_WARRING ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "시스템 메뉴" ), UMBS_YESNO,
		UI_SYSTEMMENU, MSGCMD_LOGOUT_RELIC_WARRING );
	MsgBoxInfo.AddString( _S( 6411, "로그아웃 하거나 캐릭터 변경 시 유물 아이템은 사라집니다. 그래도 나가시겠습니까??" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	_SetBtnEnable(eBTN_GO_CHAR_SEL_UI, FALSE);
	_SetBtnEnable(eBTN_GO_LOGIN_UI, FALSE);
}

bool CSystemMenuUI::IsHaveRelic()
{
	CUIInventory* pInven = UIMGR()->GetInventory();

	// 인벤토리에 가지고 있는지 체크
	int nTab, nInvenIdx;
	if (pInven->GetItemSlotInfo(DEF_RELIC_ITEM1, nTab, nInvenIdx) ||
		pInven->GetItemSlotInfo(DEF_RELIC_ITEM2, nTab, nInvenIdx) ||
		pInven->GetItemSlotInfo(DEF_RELIC_ITEM3, nTab, nInvenIdx) )
	{
		return true;
	}

	// 장착한 아이템중 유물을 가지고 있는지 체크
	if (pInven->IsWearingItem(DEF_RELIC_ITEM1) ||
		pInven->IsWearingItem(DEF_RELIC_ITEM2) ||
		pInven->IsWearingItem(DEF_RELIC_ITEM3))
	{
		return true;
	}

	return false;
}

void CSystemMenuUI::openUI()
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);
	resetUI();
	_setServerInfo();
	
	UIMGR()->RearrangeOrder( UI_SYSTEMMENU, TRUE );
}

void CSystemMenuUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	UIMGR()->RearrangeOrder( UI_SYSTEMMENU, FALSE );
}

void CSystemMenuUI::resetUI()
{
	_SetBtnEnable(eBTN_GO_CHAR_SEL_UI, TRUE);
	_SetBtnEnable(eBTN_GO_LOGIN_UI, TRUE);
}

void CSystemMenuUI::initialize()
{
#ifndef		WORLD_EDITOR
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	// Btn
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_option", "btn_help"
	, "btn_goCharSel", "btn_goLogin"};

	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSystemMenuUI::closeUI, this));

	if (m_pBtn[eBTN_OPTION] != NULL)
		m_pBtn[eBTN_OPTION]->SetCommandFUp(boost::bind(&CSystemMenuUI::PressOption, this));

	if (m_pBtn[eBTN_HELP] != NULL)
		m_pBtn[eBTN_HELP]->SetCommandFUp(boost::bind(&CSystemMenuUI::PressHelp, this));

	if (m_pBtn[eBTN_GO_CHAR_SEL_UI] != NULL)
		m_pBtn[eBTN_GO_CHAR_SEL_UI]->SetCommandFUp(boost::bind(&CSystemMenuUI::PressGoCharSel, this));

	if (m_pBtn[eBTN_GO_LOGIN_UI] != NULL)
		m_pBtn[eBTN_GO_LOGIN_UI]->SetCommandFUp(boost::bind(&CSystemMenuUI::PressGoLogin, this));

	m_pTextConnetingServerName = (CUIText*)findUI("str_connectingServerName");
#endif	//	WORLD_EDITOR
}

void CSystemMenuUI::ToggleVisible()
{
	BOOL	bHide = GetHide();

	ResetPosition( _pdpMain->dp_MinI, _pdpMain->dp_MinJ,
		_pdpMain->dp_MaxI, _pdpMain->dp_MaxJ );

	if (bHide == TRUE)
		openUI();
	else
		closeUI();
}

void CSystemMenuUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetPos( pixMaxI - pUIManager->GetRadar()->GetWidth() - GetWidth(),
		pixMinJ + pUIManager->GetRadar()->GetHeight() + GetHeight() );
}

void CSystemMenuUI::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInSystemMenu = GetPosX();
	g_iYPosInSystemMenu = GetPosY();
}

void CSystemMenuUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CSystemMenuUI::_reStart()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SYSMENU_RESTART ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "시스템 메뉴" ), UMBS_YESNO,
		UI_SYSTEMMENU, MSGCMD_SYSMENU_RESTART );

	if( _pUIBuff->IsBuffBySkill( 1060 ) != NULL )	// 출석대기 버프 index가 1060임
	{
		MsgBoxInfo.AddString( _S( 5592, "출석체크를 하지 않은 상태입니다." ) );
	}

	MsgBoxInfo.AddString( _S( 451, "게임을 재시작 하시겠습니까?" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CSystemMenuUI::_setServerInfo()
{
	if (m_pTextConnetingServerName == NULL)
		return;

	CTString strServerInfo;
	strServerInfo.PrintF("%s - %d", UIMGR()->GetServerSelect()->GetServerName(_pNetwork->m_iServerGroup), _pNetwork->m_iServerCh );

	m_pTextConnetingServerName->SetText(strServerInfo);
}

void CSystemMenuUI::_SetBtnEnable( int nBtnType, BOOL bEnable )
{
	if (nBtnType < 0 || nBtnType >= eBTN_MAX)
		return;

	if (m_pBtn[nBtnType] != NULL)
		m_pBtn[nBtnType]->SetEnable(bEnable);
}

void CSystemMenuUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SYSMENU_EXIT:
		if( bOK )
		{
			if(IsHaveRelic() == true)
			{
				LogoutRelicWarring();
				return;
			}
			g_iXPosInSystemMenu = GetPosX();
			g_iYPosInSystemMenu = GetPosY();

			_pNetwork->SendClickObject(-1);
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );

			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;	
		}
		break;

	case MSGCMD_SYSMENU_RESTART:
		if( bOK )
		{
			if(IsHaveRelic() == true)
			{
				LogoutRelicWarring();
				return;
			}

			g_iXPosInSystemMenu = GetPosX();
			g_iYPosInSystemMenu = GetPosY();

			_pNetwork->SendClickObject(-1);
			_pGameState->SetRestartGameValue(TRUE);
			m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			_SetBtnEnable(eBTN_GO_CHAR_SEL_UI, FALSE);
			_SetBtnEnable(eBTN_GO_LOGIN_UI, FALSE);
		}
		else
		{
			g_bAutoRestart = FALSE;
		}
		break;

	case MSGCMD_LOGOUT_RELIC_WARRING:
		{
			if (bOK)
			{
				if (m_bCharacterMove == TRUE)
				{
					g_iXPosInSystemMenu = GetPosX();
					g_iYPosInSystemMenu = GetPosY();

					_pNetwork->SendClickObject(-1);
					_pGameState->SetRestartGameValue(TRUE);
					m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				}
				else
				{
					g_iXPosInSystemMenu = GetPosX();
					g_iYPosInSystemMenu = GetPosY();

					_pNetwork->SendClickObject(-1);
					_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );

					_pGameState->Running() = FALSE;
					_pGameState->QuitScreen() = FALSE;	
				}
			}
			else
			{
				resetUI();
			}
		}
		break;
	}
}

WMSG_RESULT CSystemMenuUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->RearrangeOrder( UI_SYSTEMMENU, TRUE );
	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSystemMenuUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

