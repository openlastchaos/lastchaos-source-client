#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include "StageGamePlay.h"
#include <Engine/GameState.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Contents/function/SummonUI.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/Party.h>
#include <engine/Contents/function/SystemMenuUI.h>
#include <Engine/Contents/Base/UIExpedition.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Contents/function/PremiumCharUI.h>
#include <Engine/Contents/function/ItemCollectionUI.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/function/NewsUI.h>
#include <Engine/Contents/function/News_Web_UI.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Contents/Base/PersonalshopUI.h>
#include <Engine/Contents/function/PetTargetUI.h>
#include <Engine/Contents/function/CompoundUI.h>
#include <Engine/Contents/function/GuildWarMixUI.h>
#include <Engine/Contents/function/HelperManager.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/function/RoyalrumbleUI.h>
#include <Engine/Contents/function/TatooUI.h>
#include <Engine/Contents/function/GuildBattleMatchRegUI.h>

CStageGamePlay::CStageGamePlay()
	: m_bFirst(true)
{

}

void CStageGamePlay::Init()
{
	extern INDEX g_iCountry;
	
	switch(g_iCountry)
	{
	case KOREA:
		m_dwRemainTime = 1;
		break;
	case USA:
	case BRAZIL:
	case MEXICO:
	case ENGLAND:
	case GERMANY:
	case SPAIN:
	case FRANCE:
	case POLAND:
	case ITALY:	
	case RUSSIA:
		m_dwRemainTime = 10;
		break;
	default:
		m_dwRemainTime = 0;
		break;
	}
	m_bSendRestart = false;
	UIMGR()->SetCSFlagOff(CSF_TELEPORT);

	if (m_bFirst == true)
	{
		// ???? ?? ?? ?????? ??????
		m_bFirst = false;

		UIMGR()->GetGuildBattleMatchReg()->init_zone();
	}
}

void CStageGamePlay::Release()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	GameDataManager* pGameData = GameDataManager::getSingleton();

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //?????? ???? ????
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

#ifndef	VER_TEST
	_pNetwork->m_ubGMLevel = 0;
#endif	// VER_TEST

	// [090826: selo] ?????? ?????? ???? ??????
	pUIManager->GetChattingUI()->SaveSysMessageComplete(1);

	// Date : 2006-04-26(???? 11:48:00), By eons
	if( penPlayerEntity->IsPolymophing() )	// ???? ????????
	{
		penPlayerEntity->ReturnChange( TRUE );
		const int iStopChangeItem = 521;

		Notice* pNotice = pGameData->GetNotice();

		if (pNotice != NULL)
			pNotice->DelFromNoticeList(iStopChangeItem, Notice::NOTICE_POLYMOPH);
	}

	// ?????? ????
	pUIManager->GetSummonFirst()->ResetSummon();
	pUIManager->GetSummonSecond()->ResetSummon();
	// ???????? ??????
	pUIManager->GetPersonalShop()->ResetShop();
	pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );

	// ?????? ??????
	pUIManager->SetCSFlagOff( CSF_MOONSTONE );

	// ?????? ??????
	pUIManager->SetCSFlagOff( CSF_MOUNT_HUNGRY );

	// ???? ????
	CUIGuild* pGuild = pUIManager->GetGuild();

	if (pGuild != NULL)
	{
		pGuild->ResetGuild();
		pGuild->ClearMemberList();
		pGuild->ClearGuildSkill();
	}

	// ???????? ??????
	pGameData->GetPartyInfo()->Init();
	// ?????? ??????
	pGameData->GetPartyInfo()->InitExpedition();
	pUIManager->GetExpedition()->closeUI();

	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	// ?? ???? ??????
	_pNetwork->LeavePet( (CPlayerEntity*)penPlEntity );
	pInfo->GetMyPetInfo()->Init();
	pUIManager->GetPetTargetUI()->closeUI();

	pUIManager->GetTatoo()->Close();
	// FIXED : wild pet mount bug [12/12/2010 rumist]
	// ?????? ?????? ?????? ?????? ?? ????.
	static_cast<CPlayerEntity*>(penPlEntity)->LeavingWildPet( static_cast<CPlayerEntity*>(penPlEntity) );
	pInfo->SetMyApet(NULL);
	// royal rumble init [5/11/2011 rumist]
	pUIManager->GetRadar()->ResetRoyalRumbleStat();
	pUIManager->GetRoyalRumbleIcon()->ShowRoyalRumbleIcon( FALSE );
	// ITS#3867 : ???? ?? ?????? ?????????? ????. [2/13/2012 rumist]
	static_cast<CPlayerEntity*>(penPlEntity)->CancelProduct();

	pInfo->GetMySlaveInfo(0)->Init();
	pInfo->GetMySlaveInfo(1)->Init();

	pGameData->GetGPS()->RelicPosClear();

	// guardian reset
	pGameData->GetHelperManager()->ClearHelperList();

	// NPC ?????? ????
	pUIManager->GetNpcScroll()->CloseNpcScroll();

	penPlayerEntity->ClearMultiTargets();

	_pNetwork->DeleteAllMob();
	_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL; // ??????

	if (pGameData)
	{
		PetStash* pPetStash = pGameData->GetStashData();

		if (pPetStash)
		{
			pPetStash->SetEffectModelNo(-1);
		}
	}

	int i;
	// Reset inventory
	for( i = 0; i < WEAR_TOTAL; ++i )
	{
		_pNetwork->MyWearItem[i].Init();
		pUIManager->GetInventory()->InitWearBtn(-1, -1, i);
	}
	// Reset Costume Wearing Info

	for( i = 0; i < WEAR_COSTUME_TOTAL; ++i)
	{
		_pNetwork->MyWearCostItem[i].Init();
		pUIManager->GetInventory()->InitCostumeInventory(-1, -1, i);
	}

	_pNetwork->MyCharacterInfo.bOneSuit = FALSE;
	_pNetwork->MyCharacterInfo.iOneSuitDBIndex	= -1;
	_pNetwork->MyCharacterInfo.iOneSuitUniIndex	= -1;

	int		t, each;
	for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
	{
		if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
		else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
		else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

		for (i = 0 ; i < each; ++i)
		{
			_pNetwork->MySlotItem[t][i].Init();
			pUIManager->GetInventory()->InitInventory( t, i, -1, -1, -1 );
		}

		if( t >= INVENTORY_TAB_CASH_1 && INVENTORY_TAB_CASH_2 <= t )
			pUIManager->GetInventory()->CashInvenLockOn();
	}

	// ?????? ?????? ?? ???? ???? GMNotice ?? ??????!
	_UIAutoHelp->ClearGMNNotice();
	pUIManager->ResetHoldBtn();

	MY_INFO()->ClearPetSkill();
	MY_INFO()->clear();

	pUIManager->GetMap()->ClearPlayerEntity();

	CPremiumChar* pChar = pGameData->GetPremiumChar();

	if (pChar != NULL)
		pChar->Clear();

	pUIManager->GetPremiumChar()->CloseUI();
	pUIManager->GetItemCollection()->CloseUI();

	pUIManager->GetNewsUI()->CloseUI();
	pUIManager->GetNewsWebUI()->CloseUI();

	ExpressSystem* pLce = pGameData->GetExpressData();

	if (pLce != NULL)
		pLce->UnLock();

	MSGBOXMGR()->DeleteAll();

	pUIManager->GetChatFilter()->CloseChatFilter();
	pUIManager->GetCompound()->CloseCompound();
	pUIManager->GetGWMix()->CloseGWMix();
}

void CStageGamePlay::Run()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (_pGameState->IsRestartGame() && m_bSendRestart == false)
	{
		DWORD	tmpTime			= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		DWORD	IntervalTime	= 0;
		int		iShowTime		= 0;
		CTString strtime;
		
		IntervalTime = tmpTime - pUIManager->GetSystemMenu()->GetRestartTime();
		iShowTime = m_dwRemainTime - (IntervalTime / 1000);

		strtime.PrintF(_S(4201,"%d ?? ???? ??????????????."), iShowTime);

		if (m_dwRemainTime > 0)
			_UIAutoHelp->SetGMNotice(strtime);

		if (IntervalTime >= (m_dwRemainTime * 1000)) // 15??  15000 -> 10???? ???? 
		{	// ??????
			_UIAutoHelp->ClearGMNNotice();
			pUIManager->GetSystemMenu()->ResetRestartTime();
			pUIManager->GetSystemMenu()->ReStartNow();
			m_bSendRestart = true;
		}
	}
}
