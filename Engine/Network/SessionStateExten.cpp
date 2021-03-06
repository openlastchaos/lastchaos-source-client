#include "stdh.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Contents/function/PetTrainingUI.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Contents/function/SummonUI.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Contents/Base/UIPetStash.h>
#include <Engine/Interface/UICashShopEX.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIPartyAuto.h>
#include <Engine/Contents/Base/PartyAutoUIInviteList.h>
#include <Engine/Contents/Base/PartyAutoUIPartyList.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Contents/function/UIPortalNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIRanking.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Contents/function/PetFreeUI.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Contents/function/SocketCreateUI.h>
#include <Engine/Contents/function/SocketCombineUI.h>
#include <Engine/Contents/function/JewelComposUI.h>
#include <Engine/Contents/function/PetItemMixUI.h>
#include <Engine/Contents/function/AffinityUI.h>
#include <Engine/Contents/function/AffinityInfoUI.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UIRankingSystem.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Contents/Login/BackImageManager.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Info/ServerInfo.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/function/PetTargetUI.h>
#include <Engine/Contents/function/CubeRankingUI.h>
#include <Engine/Contents/function/RoyalrumbleUI.h>

// socket system. [5/11/2010 rumist]
//#include <Engine/Interface/UISocketSystem.h>
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")
// #include <map>
// #include <algorithm>
//#define XTRAP_LOG_ENABLE
// ----------------------------------------------------------------------------
// Name : CheckDratanSiegewarfareError()
// Desc : 
// ----------------------------------------------------------------------------
void CheckDratanSiegewarfareError(UBYTE errcode)
{
	CTString tStr;
	switch(errcode)
	{
		case MSG_EX_CASTLE_ERROR_NOT_JOIN:    // ???? ???????? ????
				tStr = _S( 3739,"???? ???????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_WAR:    // ?????? ???? ????
				tStr = _S( 3740,"???????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_POS_FAIL:    // ???? ????
				tStr = _S( 3741,"???? ?????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_MEM_5_FAIL:    // ???? ???? ???? ????
				tStr = _S( 3742,"???? ?????? ??????????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_OWNER:    // ???? ???? ????
				tStr = _S( 3743,"???? ?????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND:   // ???? ???? ????
				tStr = _S( 3744,"???????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME:  // ???? ???? ????
				tStr = _S( 3745,"???? ?????? ??????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_MONEY:    // ?? ????
				tStr = _S(306,"?????? ??????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_ATTACK:    // ???? ???? ????
				tStr = _S( 3746,"???? ?????? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_DATA:    // ?????? ????
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_DIFFER:    // ???? ???????? ????				
				tStr = _S( 3747,"???? ?????? ??????????.");				
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY:    // ???????? ??????				
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC:     // ???? ????
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC:    // ???? ???? 
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD:   // ???? ???? ????
				tStr = _S(985,"???? ???????? ???? ?? ????????.");
				break;				
		case MSG_EX_CASTLE_ERROR_USE_NPC:    // ?????? ???? ????
				tStr = _S(3749,"???????? ??????????????.");
				break;
		case MSG_EX_CASTLE_ERROR_USE_NPC_OTHER:   // ???? ???? ???????? ??????
				tStr = _S(3750,"???? ?????????? ????????????.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC2:     // ???? ???? ????
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC2:    // ???? ????????
				tStr.PrintF("Error %d",errcode);
				break;				
		case MSG_EX_CASTLE_ERROR_SAME_DATA:	   // ???? ?????? - ?????? ?????? ????????...
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY_NPC:  // ?????? NPC?? ???? ???? ????????,
				tStr = _S(3812, "?????? ?????? ?? ????????.");
				break;
		case MSG_EX_CASTLE_ERROR_PERMISSION_DENIED: // ?????? ????????.
				tStr = _S( 973, "?????? ????????.");
				break;
	}
	
	if(tStr.Length()>0 )
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( tStr , SYSMSG_ERROR );	
	
}

// ----------------------------------------------------------------------------
// Name : CheckCashMoonStoneMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CheckCashMoonStone(UBYTE errcode)
{
	if( errcode == MSG_EX_CASHITEM_MOONSTONE_ERROR_SUCCESS )	// ????
		return TRUE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(errcode)
	{
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM:		
			pUIManager->GetChattingUI()->AddSysMessage( _S(2908, "?????? ???????? ????????." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN:		
			pUIManager->GetChattingUI()->AddSysMessage( _S(265, "?????????? ??????????." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON:			
			pUIManager->GetChattingUI()->AddSysMessage( _S(2907, "???? ?????? ???????? ????????." ), SYSMSG_ERROR );	
			break;
	}

	return FALSE;

}

// ----------------------------------------------------------------------------
// Name : CheckCashItemMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CheckCashItemMessage(UBYTE errcode)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(errcode)
	{
		case MSG_EX_CASHITEM_ERROR_SUCCESS:				// ????
			pUIManager->GetChattingUI()->AddSysMessage( _S(2408,  "?????? ?????? ??????????????." ), SYSMSG_ERROR );	
			return TRUE;
			
		case MSG_EX_CASHITEM_ERROR_LACKCASH:			// ???????? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2409, "???? ?????? ???? ??????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTUSER:				// ???????? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2410, "???? ?????? ???????? ????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTITEM:				// ???????? ???? ??????????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2411, "???????? ???? ??????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_DB:					// DB????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(1843, "?????? ?????? ?? ????????.(B)"), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_PACKET:				// ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(854, "???? ?????? ??????????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_ETC:					// ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(1094, "?? ?? ???? ?????? ??????????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_OVERITEM:				// ???? ???? ???? ????(10??)
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2413, "???? ?????? 10???? ??????????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOSPACE:				// ???????? ???????? ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2414, "???????? ???????? ?????? ??????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_LACKINVEN:			// ?????? ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(265, "?????? ???????? ?????? ??????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_CONN:					// ?????? ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2415, "?????? ???? ??????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_BILL:					// ???? ???? ????
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2416, "???? ???? ??????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTHAVECT :
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "????"), _S(2769, "?????? ???? ??????."), UMBS_OK);
			break;


		// ???? ???? :Su-won	|---------->
		case MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3102, "???? ??????"), _S(3106, "???? ???????? ??????????????."), UMBS_OK);
			return TRUE;
		case MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3107, "???? ?????? ????"), _S(3108, "???? ???????????? ?????? ???? ?? ????????. ?????? ???? ???????? ?????? ??????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_CANT_GIFT:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3107, "???? ?????? ????"), _S(5366, "?????? ?????? ?????? ????????????."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3109, "???? ????"), _S(3110, "???? ???? ???????? ?????? ?????? ?????????? ????????????."), UMBS_OK);
			pUIManager->GetCashShopEX()->SetLoadDataState(FALSE);
			return TRUE;
		// ???? ???? :Su-won	<----------|
	}

	pUIManager->GetCashShopEX()->SetLoadDataState(FALSE);
	return FALSE;

}
void CheckSocketSystemError(UBYTE errcode);

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    #include <Engine/XTrapInterface/XTrapInterface.h>
#endif

void CheckTimerEvent()
{
	if (IS_EVENT_ON(A_EVENT_SONGKRAN))
	{
		_pNetwork->ga_World.loadTradeItem();
	}

	if (IS_EVENT_ON(A_EVENT_HOLLOWEEN))
	{
		g_fGWTime = 30*3600; // ??????????2007???????????? ???? ???? ???? ????.
		g_fGWTimeMul = 0.0f;
	}
	else
	{
		g_fGWTime = CUIManager::getSingleton()->GetRadar()->GetHour() * 3600;
		g_fGWTimeMul = 1.0f;
	}

	FOREACHINDYNAMICCONTAINER( _pNetwork->ga_World.wo_cenEntities, CEntity, iten) 
	{
		if( iten->GetName() == CTString("Color controller") || iten->GetName() == CTString("SunMoon") )
		{
			iten->Initialize();
		}
	}

	if (!_pUIBuff->IsBuffBySkill(564))
	{
		if (CUIManager::getSingleton()->GetInventory()->GetWearingBtn(WEAR_HELMET)->IsEmpty() == false)
		{
			int iItemIndex = CUIManager::getSingleton()->GetInventory()->GetWearingBtn(WEAR_HELMET)->getIndex();
			if ((( ( iItemIndex >= 4927 && iItemIndex <= 4932 ) || iItemIndex == 6228) && IS_EVENT_ON(A_EVENT_HOLLOWEEN))// ?????? ???????? ??????????.
				|| (iItemIndex >= 7253 && iItemIndex <= 7259)) // [ldy1978220 2011/5/31] ?????? 10???? ?????? ???? ???? ??????
			{ // ???????? hidden ?????? npc?? ?? ?? ????.
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
			}
			else
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOff(ENF_SHOWHIDDEN);
			}
		}
	}


	LoginNew* pInfo = GAMEDATAMGR()->GetLoginData();

	if (pInfo != NULL)
	{
		if (IS_EVENT_ON(A_EVENT_HOLLOWEEN))
			pInfo->SetLogoType(eLOGO_HOLLOWEEN);
		else if (IS_EVENT_ON(TEVENT_XMAS_2007))
			pInfo->SetLogoType(eLOGO_XMAS);
		else
			pInfo->SetLogoType(eLOGO_NORMAL);
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveExtendMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExtendMessage( CNetworkMessage *istr )
{
	int		i;
	LONG	lType;
	UBYTE	errcode;
	UBYTE	exType;
	SLONG	nServerTime;
	(*istr) >> lType;
	CTString		strTitle,strMessage,newName;
	ULONG			nIndex;
	UBYTE			nJob;
	BYTE			bIsGuildName;
	CUIMsgBox_Info	MsgBoxInfo;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(lType)
	{
	case MSG_EX_PET_STATUS:
		ReceivePetStatusMessage(istr);
		break;
	case MSG_EX_PET_MOUNT:
		ReceivePetMountMessage(istr);
		break;
	case MSG_EX_PET_LEARN:			// ?? ????			: skillindex(n) level(c:server) errorcode(n:server)
		ReceivePetLearnMessage(istr);
		break;
	case MSG_EX_PET_SKILLLIST:		// ?? ???? ??????	: index(n) count(n) [skillindex(n) skilllevel(c)] ...
		ReceivePetSkillListMessage(istr);
		break;
	case MSG_EX_PET_RESET_SKILL:	// ?? ???? ??????
		ReceivePetResetSkillMessage(istr);
		break;
	case MSG_EX_PET_SELL_INFO:		// ?? ????/???? ????: ownerindex(n) petindex(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) ability(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n)
		ReceivePetSellInfoMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_MOUNT:	// ?? ???? ????		: errorcode(n:server)
		ReceivePetChangeMountMessage(istr);
		break;	
	
	case MSG_EX_PET_COMMAND:		// ?? ????????		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Pet(MSG_EX_PET_COMMAND, &(*istr));			
		}
		break;
	case MSG_EX_PET_LEVELUP: // ?? ?????? petindex(n) typegrade(c) level(n)
		ReceivePetLevelUpMessage(istr);
		break;
	case MSG_EX_PET_MIX_ITEM:		// ?? ?????? ????	: 
		ReceviePetItemMixMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_ITEM:	// ?? ?????? ????  
		ReceviePetItemChangeMessage(istr);
		break;
	case MSG_EX_PET_REBIRTH:
		ReceviePetRebirthMessage(istr); // ?? ???? ????
		break;
	
	case MSG_EX_PARTY_RECALL:
		ReceivePartyRecall(istr);	// CashItem 060306
		break;

	case MSG_EX_NAMECHANGE :
		(*istr) >> errcode;
		switch(errcode)
		{
			case MSG_EX_NAMECHANGE_ERROR_SUCCESS:
				(*istr) >> newName;
				(*istr) >> bIsGuildName;
				if(bIsGuildName)
				{
					_pNetwork->MyCharacterInfo.strGuildName=newName;
					if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"????");
					strMessage	=_S( 2131, 	"???????? ???? ???? ?????? ??????????????" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
					
				}
				else 
				{
					_pNetwork->MyCharacterInfo.name=newName;
					if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"????");
					strMessage	=_S( 2132, 	"?????? ?????????? ??????????????" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
					
					//wooss 051004 ?????? ???????? ?????? ????
					pUIManager->GetMessenger()->SetMyInfo(-1,newName);
					
				}
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_DUPLICATE:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_DUPLICATE)) return;
				strTitle	=	_S(191,"????");
				strMessage	=_S(12,"?????? ???? ??????."); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_HELPER:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_HELPER)) return;
				strTitle	=	_S(191,"????");
				strMessage	=	_S( 2133, "???????? ???? ??????" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_INVALID:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_INVALID)) return;
				strTitle	=	_S(191,"????");
				strMessage	=_S( 2134, 	"?????? ?????? ?????? ????????" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS)) return;
				strTitle	=	_S(191,"????");
				strMessage	=_S( 2136, 	"?????????? ?????????? ?????? ?? ????????" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;	

			case MSG_EX_NAMECHANGE_ERROR_PARTY:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_PARTY)) return;
				strTitle	=	_S(191,"????");
				strMessage	=_S(4718, "?????????? ?????? ?????? ?? ????????." ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
			}
			break;

		case MSG_EX_CASHITEM:			// ?????? ????		: subtype(uc) ...
			{
				ULONG tv_cash, /*nCtid,*/ nCount;
				(*istr) >> exType;
				
				switch(exType)
				{
				case MSG_EX_CASHITEM_TYPE_LIST_REP:
					{ // max type count?? ???????? ???????? index?? ???? ???? index?? ????????.
						pUIManager->GetCashShopEX()->SetCashType(istr);
					}
					break;
				case MSG_EX_CASHITEM_LIST_REP:
					{
						pUIManager->GetCashShopEX()->SetGoodsList(istr);
					}
					break;
				case MSG_EX_CASHITEM_RECOMMAND_REP:
					{
						pUIManager->GetCashShopEX()->SetRecommandList(istr);
					}
					break;
				case MSG_EX_CASHITEM_WISHLIST_REP:
					{
						pUIManager->GetCashShopEX()->SetWishList(istr);
					}
					break;

					case MSG_EX_CASHITEM_BALANCE_REP :		// ????				: errorCode(uc) cashBalance(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr) >> 	tv_cash;
								pUIManager->GetCashShopEX()->SetMyCash(static_cast<SQUAD>(tv_cash));
							}
						}
						break;			
					case MSG_EX_CASHITEM_PURCHASE_REP :		// ????				: errorCode(uc) cashBalance(n)
					case MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REP : // ???? ???? ???? : errorCode(uc) cashBalance(n)
						{							
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr) >> 	tv_cash;
								pUIManager->GetCashShopEX()->SetMyCash(static_cast<SQUAD>(tv_cash));
								pUIManager->GetCashShopEX()->CompletePurchase();
							}
						}
						break;
					case MSG_EX_CASHITEM_BRING_REP:			// ????				: errorCode(uc) 
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								pUIManager->GetCashShopEX()->CompleteBringItems();
							}
						}
						break;	
					case MSG_EX_CASHITEM_PURCHASELIST_REP:   // ????				: count(n) idx(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->SetPurchaseItemList(istr);
							}
						}
						break;
					case MSG_EX_CASHITEM_PURCHASEHISTORY_REP:// ????				: errorcode(uc) count(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr >> nCount);
								if( nCount > 0){
// 									pUIManager->GetCashShop()->m_abtnHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbHistory.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbHistory.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbHistory.SetCurItemCount(nCount);
// 									for( int i=0; i < nCount ;i++){
// 										CUIButtonEx tv_btn;
// 										ULONG nItemCnt;
// 										(*istr) >> nItemCnt;
// 										(*istr) >> nCtid;
// 										pUIManager->GetCashShop()->m_abtnHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn)){
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i].Copy(tv_btn);
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i].SetItemCount(nItemCnt);
// 										}
// 									}
								}
							}						
						}
						break;
					case MSG_EX_CASHITEM_MOONSTONE_START :
						{
							(*istr) >> errcode;
							if(CheckCashMoonStone(errcode)){
								UBYTE grade;
								(*istr) >> grade;
								grade =5-grade;
								pUIManager->GetGamble()->SetSelectedMarker( grade, grade );
								pUIManager->GetGamble()->Start();
							}						
						}
						break;

					// ???? ???? :Su-won	|--------------------------------------------------->
					case MSG_EX_CASHITEM_GIFT_REP:					//????	: errcode(uc) 
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->CompleteBringItems();
							}
						}
						break;
					case MSG_EX_CASHITEM_WISHLIST_SAVE_REP: // ???????? ?????? ???? ????
						{
							//(*istr) >> errcode;
							pUIManager->GetCashShopEX()->RevWishList(istr, TRUE);
						}
						break;
					case MSG_EX_CASHITEM_WISHLIST_DEL_REP:
						{
							pUIManager->GetCashShopEX()->RevWishList(istr, FALSE);
						}
						break;
					case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP:		// ????	: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
// 									pUIManager->GetCashShop()->m_abtnSendHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetCurItemCount(nCount);
// 
// 									pUIManager->GetCashShop()->m_astrSendChar.New(nCount);
// 									for( int i=0; i < nCount ;i++)
// 									{
// 										CUIButtonEx tv_btn;
// 										//ULONG nItemCnt;
// 										(*istr) >> nCtid;
// 										(*istr) >> newName;
// 										pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
// 										{
// 											pUIManager->GetCashShop()->m_abtnSendHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].Copy(tv_btn);
// 											//pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].SetItemCount(nItemCnt);
// 											pUIManager->GetCashShop()->m_astrSendChar[i] =newName;
// 										}
// 									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP:// ????				: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
// 									pUIManager->GetCashShop()->m_abtnRecvHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetCurItemCount(nCount);
// 
// 									pUIManager->GetCashShop()->m_astrReceiveChar.New(nCount);
// 									for( int i=0; i < nCount ;i++)
// 									{
// 										CUIButtonEx tv_btn;
// 										//ULONG nItemCnt;
// 										//(*istr) >> nItemCnt;
// 										(*istr) >> nCtid;
// 										(*istr) >> newName;
// 										pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
// 										{
// 											pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i].Copy(tv_btn);
// 											//pUIManager->GetCashShop()->m_abtnReceiveHistoryItems[i].SetItemCount(nItemCnt);
// 											pUIManager->GetCashShop()->m_astrReceiveChar[i] =newName;
// 										}										
// 									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVLIST_REP:// ????
						{
							pUIManager->GetCashShopEX()->RevGiftList(istr);
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECV_REP:		// ????				: errCode(uc)
						{
							(*istr) >> errcode;
							if (CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->RevResultGetGift();
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_NOTICE: // ???? ?????? ???? ???? ??????
						{
							BYTE bGift;

							(*istr) >> bGift;
							// MSG_EX_CASHITEM_GIFT_NOTICE?? Party ?? ?????? ???? ???????? ???? ???? ???? ?????????? ????????
							// ?? ???? ?? ???? ?? ???????? ???? ?????? ?? ?????? ???? ?????? ???? ?????? ???????? ???? ????
							Party* pParty = GAMEDATAMGR()->GetPartyInfo();

							if (pParty == NULL)
								return;

							if( pParty->GetIsPartyPlay() == FALSE)
							{
								if( pUIManager->IsCSFlagOn(CSF_EXPEDITION) )
								{
									pParty->ExpeditionEnd();
									pUIManager->GetChattingUI()->AddSysMessage( _S( 4665, "???????? ??????????????." ) );
								}
								
								if( pUIManager->IsCSFlagOn(CSF_PARTY) )
									pParty->PartyEnd();
							}
							//////////////////////////////////////////////////////////////////////////
							pUIManager->GetQuickSlot()->SetGiftRecv(bGift);
						}
						break;
					// ???? ???? :Su-won	<---------------------------------------------------|
					case MSG_EX_CASHITEM_CUPON_REP: // ???? ???? ?????? ????
						{
							(*istr) >> errcode;

							if (CheckCashItemMessage(errcode))
							{ // pUIManager->GetCashShop()->m_pConfirmInfo NULL ???? ???? 
								// ???? ???????? ????
								SLONG slCuponCout;
								(*istr) >> slCuponCout;

								//pUIManager->GetCashShop()->m_pConfirmInfo->ClearCuponData(); // ?????? ?????? ??????

								if (slCuponCout > 0) // ???? ??????
								{
									int i;

									for (i=0; i<slCuponCout; i++)
									{
										CTString strCuponName;
										SLONG slCuponID;
										SLONG slDCPoint;
										SLONG slLimitPoint = 0;

										(*istr) >> strCuponName; // ???? ????
										(*istr) >> slCuponID; // ???? ??????
										(*istr) >> slDCPoint; // ???? ????
										(*istr) >> slLimitPoint; // ???? ???? ????

										//pUIManager->GetCashShop()->m_pConfirmInfo->AddCuponData(strCuponName, slCuponID, slDCPoint, slLimitPoint);
									}
								}

								//pUIManager->GetCashShop()->m_pConfirmInfo->SetEnable(TRUE);
								//pUIManager->GetCashShop()->m_pConfirmInfo->SetVisible(TRUE);
							}
						}
					case MSG_EX_CASHITEM_SHOP_LOCK:
						{
							pUIManager->GetCashShopEX()->ForcedExit();
						}
						break;
					case MSG_EX_CASHITEM_SHOP_UNLOCK:
						{
							pUIManager->GetCashShopEX()->SetCashShopLock(FALSE);
						}
						break;
				}
			}
			break;

		// wooss 051004 ?????? ?????? ????
		case MSG_EX_FRIENDNAMECHANGE :
			(*istr) >> nIndex;
			(*istr) >> newName;
			(*istr) >> nJob;
			pUIManager->GetMessenger()->DeleteMember(nIndex);
			pUIManager->GetMessenger()->AddFriendList(nIndex, 0, newName,(eJob)nJob);
			break;

		case MSG_EX_CASTLE_MAP_RECENT:
			{
				FLOAT	fLordX, fLordZ;
				FLOAT	nLordHP, nLordMaxHP;
				
				LONG	lTowerCount;
				
				LONG	lTowerIndex;
				FLOAT	fTowerX, fTowerZ;
				LONG	nTowerHP, nTowerMaxHP;

				FLOAT	fRegenX, fRegenY;
				LONG	nSenderFlag;
				LONG	nSenderIndex;
				FLOAT	fSignalX, fSignalY;
				
				extern SLONG	g_slZone;

				if( _pUISWDoc->IsWar() ) 
				{
					pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 1 );
					pUIManager->GetMap()->ReSetData();
				}
				else
				{
					pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 0 );
					return;
				}

				pUIManager->GetMap()->InitCastleData();
				// ????
				(*istr) >> fLordX;
				(*istr) >> fLordZ;
				(*istr) >> nLordHP;
				(*istr) >> nLordMaxHP;
				
				pUIManager->GetMap()->AddCastleData( CASTLE_LORD, fLordX, fLordZ, -1, nLordHP, nLordMaxHP );

				// ???? ?? 
				(*istr) >> lTowerCount;

				for( int i = 0; i < lTowerCount; i++ )
				{
					(*istr) >> lTowerIndex;
					(*istr) >> fTowerX;
					(*istr) >> fTowerZ;
					(*istr) >> nTowerHP;
					(*istr) >> nTowerMaxHP;

					pUIManager->GetMap()->AddCastleData( CASTLE_TOWER, fTowerX, fTowerZ, lTowerIndex, nTowerHP, nTowerMaxHP );
				}

				// ???? ??????
				(*istr) >> fRegenX;
				(*istr) >> fRegenY;
				
				pUIManager->GetMap()->AddCastleData( CASTLE_REGEN, fRegenX, fRegenY );
				
				// Final Signal... 
				(*istr) >> nSenderFlag;
				(*istr) >> nSenderIndex;
				(*istr) >> fSignalX;
				(*istr) >> fSignalY;
					
				pUIManager->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );

			}
			break;

		case MSG_EX_CASTLE_MAP_SIGNAL:
			{
				LONG	nSenderFlag;
				LONG	nSenderIndex;
				FLOAT	fSignalX, fSignalY;
								 
				(*istr) >> fSignalX;
				(*istr) >> fSignalY;
				(*istr) >> nSenderFlag;
				(*istr) >> nSenderIndex;
				
				pUIManager->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );
			}
			break;

		case MSG_EX_ELEMENTAL_DELETE:
			{
				ReceiveElementalDeleteMessage( istr );
			}
			break;

		case MSG_EX_EVOCATION_START:
			{
				ReceiveEvocationStart( istr );
			}
			break;

		case MSG_EX_EVOCATION_STOP:
			{
				ReceiveEvocationStop( istr );
			}
			break;
			
		case MSG_EX_GUILDMEMBERNAMECHANGE :
			{
				ULONG	 charIndex; 
				(*istr) >> charIndex;
				(*istr) >> newName;
			//	if(_pNetwork->MyCharacterInfo.index == charIndex)
				pUIManager->GetGuild()->SetMemberName(charIndex,newName);

			}
			break;
		case MSG_EX_PARTY_MATCH :		// Date : 2006-05-09(???? 5:43:02), By eons
			{
				ULONG	ErrorType;
				ULONG	MatchType;
				(*istr) >> MatchType;

				switch( MatchType )
				{
				case MSG_EX_PARTY_MATCH_REG_MEMBER_REP:
					{
						(*istr) >> ErrorType;
						
						pUIManager->GetPartyAuto()->MatchRegMemberRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_REG_PARTY_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAuto()->MatchRegPartyRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_MEMBER_LIST_REP:
					{
						pUIManager->GetPartyAutoInvite()->ReceiveMemberData(istr);						
					}
					break;
				case MSG_EX_PARTY_MATCH_PARTY_LIST_REP:
					{
						pUIManager->GetPartyAutoParty()->ReceivePartyData(istr);
					}
					break;
				case MSG_EX_PARTY_MATCH_DEL_REP:
					{
						strMessage = _S( 2732, "?????? ???? ??????????." );
						MsgBoxInfo.SetMsgBoxInfo( _S( 2733, "???? ????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );						
					}
					break;
				case MSG_EX_PARTY_MATCH_INVITE_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAutoInvite()->ReceivePartyInviteMessage(ErrorType, istr);
					}
					break;
				case MSG_EX_PARTY_MATCH_JOIN_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAutoParty()->ReceivePartyJoinMessage(ErrorType, istr);
					}
					break;
				}
			}
			break;
		case MSG_EX_STRING :
			{
				BYTE	outType;
				LONG	strIdx,argCnt,argType;
				(*istr) >> outType >> strIdx >> argCnt;

				if(argCnt>0)
				{
					for(int i=0;i<argCnt;i++)
					{
						(*istr) >> argType;

						// arg 1
						if(argCnt==1){
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										LONG arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										LONGLONG arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										CTString arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
							}

						}
				/*		// arg 2
						if(argCnt==2){
							if(i==1)
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg1;
										arg1 = new BYTE;
										(*istr) >> ((BYTE)(*arg1));
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										static LONG arg1;
										(*istr) >> arg1;
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										static LONGLONG arg1;
										(*istr) >> arg1;
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										static CTString arg1;
										(*istr) >> arg1;
									}
									break;
							}
							if(i==2)
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										LONG arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										LONGLONG arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										CTString arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
							}

							
						}*/
						
					}
				}
				else strMessage=_S(strIdx,"???? ???? ??????");

				switch(outType)
				{
					case MSG_EX_STRING_OUTPUT_WINDOW :		// ??
						{
							MsgBoxInfo.SetMsgBoxInfo( _S(191,"????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							MsgBoxInfo.AddString(strMessage);
							pUIManager->CreateMessageBox(MsgBoxInfo);

						}
						break;
					case MSG_EX_STRING_OUTPUT_SYS :			// ????????
						pUIManager->GetChattingUI()->AddSysMessage(strMessage);
						break;
					case MSG_EX_STRING_OUTPUT_NOTICE :		// ????
						_UIAutoHelp->SetGMNotice ( strMessage );
						break;
					case MSG_EX_STRING_OUTPUT_CHAT :		// ????
						 pUIManager->GetChattingUI()->AddChatMessage(MSG_CHAT_SAY,
							 _pNetwork->MyCharacterInfo.index,_pNetwork->MyCharacterInfo.name,strMessage);
						break;
				}
				
			}
			break;

		case MSG_EX_MESSENGER:
				ReceiveMessengerExMessage( istr );
				break;
		
		case MSG_EX_HAIR_CHANGE:
			{
				ReceiveExHairChange(istr);
			}
			break;
		case MSG_EX_PLAYER_STATE_CHANGE:
			{
				ReceiveExPlayerStateChange(istr);
			}
			break;
		case MSG_EX_NPROTECT:
			{
#ifndef NO_GAMEGUARD
				ReceiveExnProtect(istr);
#endif
			}
			
			break;
		case MSG_EX_PET_CHANGE_NAME:
			{
				ReceiveExPetNameChange(istr);
			}
			break;
			
		case MSG_EX_CASTLE_WAR: // WSS_DRATAN_SEIGEWARFARE 2007/07/30
		{
			UBYTE bType;
			ULONG charIdx;
			CTString tStr, tStrtmp;
			(*istr) >> bType;
			switch (bType)
			{
				case MSG_CASTLE_CRISTAL_RESPOND_START:
					(*istr) >> charIdx;
					pUIManager->GetSiegeWarfareNew()->StartConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_END:	// ???? ????								
					(*istr) >> charIdx;
					(*istr) >> tStrtmp;
					tStr.PrintF(_S(3898,"[%s]???? ???????? ?????? ??????????????."), tStrtmp);
					pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );
					pUIManager->GetSiegeWarfareNew()->CompleteConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_FAIL:	// ???? ????
					(*istr) >> charIdx;
					if( _pNetwork->MyCharacterInfo.index == charIdx)
					{
						//pUIManager->GetChattingUI()->AddSysMessage( _S( 3751,"?????? ??????????????."), SYSMSG_ERROR );
						//CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(_S(3751,"?????? ??????????????."), 0xE28769FF);
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
					pUIManager->GetSiegeWarfareNew()->StopConsensus(charIdx);					
					break;
				case MSG_CASTLE_TOWER_CONTRAL:
					{
						pUIManager->GetChattingUI()->AddSysMessage( _S( 3752,"?????????? ??????????????." ), SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_CONTRAL_LIST:		// ?????? ???? ???? ??????
					{
						UBYTE tSet[DRATAN_TOWER_KIND_MAX];
						for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
						{
							(*istr) >> tSet[i];
							pUIManager->GetSiegeWarfareNew()->SetTowerSet(i,tSet[i]);
						}
						
						pUIManager->GetSiegeWarfareNew()->SetCBFromTowerSet();
						pUIManager->GetSiegeWarfareNew()->OpenCheckTower();						
						
					}
					break;
				case MSG_CASTLE_TOWER_REINFORCE:
					{
						CTString tStr,tStr2;
						UBYTE tType,tLevel;
						(*istr) >> tType >> tLevel;
						switch(tType)
						{
						case 0:
							tStr = _S( 3753,"?????? ????");
							break;
						case 1:
							tStr = _S( 3754,"?????? ????");
							break;
						case 2:
							tStr = _S( 3755,"????");
							break;
						}						
						tStr2.PrintF("%d %s",(int)tLevel, _S( 3756,"?????? ?????????? ??????????????." ));
						tStr += tStr2;
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_REINFORCE_LIST:
					{					
						UBYTE tType,tLevel;
						(*istr) >> tType >> tLevel;		
						if( (int)tType == pUIManager->GetSiegeWarfareNew()->GetUpgradeType())
						{								
							
							pUIManager->GetSiegeWarfareNew()->SetUpgradedLevel(int(tLevel));
							pUIManager->GetSiegeWarfareNew()->SetUpgradePos(int(tLevel));
							pUIManager->GetSiegeWarfareNew()->SetUpgradeLevel((int)tLevel );
							pUIManager->GetSiegeWarfareNew()->OpenUpgradeTower();
						}
						else 
							pUIManager->GetChattingUI()->AddSysMessage( _S( 3757,"?????? ?????? ???????? ????????." ), SYSMSG_ERROR );	
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE:			// ???? ???? ????    cl->gs idx(n) | gs->cl idx(n)
					{
						ULONG tIdx;
						(*istr) >> tIdx;
						pUIManager->GetChattingUI()->AddSysMessage( _S( 3758,"?????????? ??????????????." ), SYSMSG_ERROR );	
						
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE_LIST:		// ???? ???? ???? ????   cl->gs idx(n) | gs->cl money(n)
					{
						ULONG tNpcIdx;
						SQUAD tMoney;

						(*istr) >> tNpcIdx >> tMoney;
						pUIManager->GetSiegeWarfareNew()->SetRepairMoney( tNpcIdx,tMoney );
						pUIManager->GetSiegeWarfareNew()->OpenRepairTower();
					}
					break;
				case MSG_CASTLE_TOWER_WARP_LIST:
					{
						UBYTE tCnt;
						ULONG tNpcIdx;

						(*istr) >> tCnt;
						for( int i=0;i<tCnt ;i++)
						{
							(*istr) >> tNpcIdx;
							pUIManager->GetGuildWarPortal()->SetDratanPortalFlag((int)tNpcIdx);
						}
						pUIManager->GetGuildWarPortal()->OpenGuildWarPortalDratan();
					}
					break;				
				case MSG_CASTLE_QUARTERS_INSTALL:		// ???????? ???? cl->gs idx(n) | gs->cl  idx(n), gidx(n)
					{
						CTString tStr,tGuildName;
						ULONG tNpcIdx,tGuildIdx;
						SLONG tNpcUniIndex;
						(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName >> tNpcUniIndex;
						tStr.PrintF(_S( 3759,"[%s]?????? [%d]?? ?????????? ????????????." ),tGuildName,tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;						
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );

						// ?? ???? ????
						CMobData* MD = CMobData::getData(tNpcIdx);
						if(MD->GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
						{							
							tStr = tGuildName + CTString(" ") + _S( 3760,"???? ????????");;
							MD->SetName(tStr);

							tStr = MD->GetName();
							MD = CMobData::getData(tNpcIdx);
							tStr = MD->GetName();
						}

						if (!MD->IsNPC())
						{
							CEntity* pEntity = NULL;

							int		i;

							for (i = 0; i < eOBJ_MAX; ++i)
							{
								pEntity = ACTORMGR()->GetEntityByIndexServer((eOBJ_TYPE)i, tNpcUniIndex);
								// ???? npc???? ???????? ???? ????(???? ???? ???? ?????? npc?? )
								if (pEntity != NULL)
								{
									pEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
									break;
								}
							}
						}
					}
					break;
				case MSG_CASTLE_QUARTERS_CRUSH:			// ???? ???? ???? gs->cl idx(n)
					{
						CTString tStr;
						ULONG tNpcIdx;
						(*istr) >> tNpcIdx;
						tStr.PrintF(_S( 3705,"%d?? ?????????? ??????????????." ),tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = -1;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = CTString("");
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );

						// Hard Cording ^^;; ?????? ????....
						// ?? ???? ????
						CMobData* MD = CMobData::getData(tNpcIdx);
						if(MD->GetMobIndex()>0)
						{
							tStr.PrintF(_S( 3685,"???? ????????%d"),tNpcIdx-389);
							MD->SetName(tStr);
						}						
					}
					break;
				case MSG_CASTLE_QUARTERS_LIST:			// ???? ???? ?????? gs->cl 7*(nindex(n), gindex(n))
					{		
						CTString tGuildName;
						LONG tNpcIdx,tGuildIdx;

						for(int i=0;i<7;i++)
						{	
							(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName;						
							// ???? ?????? ????????...
							if (tGuildIdx!=-1)
							{
								_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
								_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;

								// ?? ???? ????
								CMobData* MD = CMobData::getData(tNpcIdx);
								if(MD->GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
								{
									tStr = tGuildName +CTString(" ") +_S( 3760,"???? ????????");
									MD->SetName(tStr);
								}
							}
							// ???? ?????? ??????...
							else 
							{
								// Hard Cording ^^;; ?????? ????....
								// ?? ???? ????
								CMobData* MD = CMobData::getData(tNpcIdx);
								if(MD->GetMobIndex()>0)
								{
									tStr.PrintF(_S( 3685,"???? ????????%d"),tNpcIdx-389);
									MD->SetName(tStr);
								}						
							}
						}
					}
					break;

				case MSG_CASTLE_WAIT_TIME:				// ???? ???? ????
					{
						LONG tWaitTime;
						(*istr) >> tWaitTime;						
						pUIManager->GetSiegeWarfareNew()->SetWaitTime(tWaitTime);
						pUIManager->GetSiegeWarfareNew()->SetTimeReply(TRUE);

					}
					break;
				case MSG_EX_CASTLE_ERROR:				// ???? ????
					{
						UBYTE errcode;
						(*istr) >> errcode;
						CheckDratanSiegewarfareError(errcode);
					}

			}
		}
		break;
	
		case MSG_EX_PET_CHANGE_COLOR:
			{
				ReceiveExPetColorChange(istr);
			}
			break;
// EDIT : BS : 070413 : ???? ???? ??????			
#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
	case MSG_EX_KEYCHANGE:		// EDIT : BS : ???? ?????? : ?? ????
		{
			ULONG nKey;
			(*istr) >> nKey;
			CNM_CopyKey(&_pNetwork->cnmKey, (CNM_KEY_TYPE*)&nKey);
		}
		break;
#endif // CRYPT_NET_MSG_MANUAL
#else
	case MSG_EX_KEYCHANGE:
		{
			ULONG nKey;
			(*istr) >> nKey;
			_pNetwork->cnmKey = nKey;
		}
#endif // #ifdef CRYPT_NET_MSG

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	case MSG_EX_DVD:
		{
			UBYTE sbSubType;
			
			(*istr) >> sbSubType;

			switch(sbSubType)
			{
			case MSG_DUNGEON_INFO:
				{
					ULONG ulEnvRate, ulMonRate;
					
					(*istr) >> ulEnvRate;
					(*istr) >> ulMonRate;

					pUIManager->GetPortal()->Create_SiegeDungeon_State_MsgBox( ulEnvRate, ulMonRate);
				}
				break;
			case MSG_MANAGEMENT:
				{
					UBYTE sbSubSubType, ubError;

					(*istr) >> sbSubSubType;
					switch( sbSubSubType )
					{
					case MSG_MANAGEMENT_MANAGER_CONFIRM:				//???? ????
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CreateMessageBoxL( _S(3908, "???? ????"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3940, "????????????? ??????."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3941, "?????? ???? ?????? ?????????????????"),-1,0xa3a1a3ff);

								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3921, "???? ???? ???? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3926, "???? ???? ?????? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3931, "???? ?????? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3935, "???? ???????? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 1220, "????????." ) );

								SERVER_INFO()->SetDG_Mode(eDUNGEON_MODE_LORD);
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3942, "???? ?????? ??????, ???? ?????????????? ?????? ?? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_ZONE)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(6275, "???? ?????????? ?????? ??????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_NORMAL_MODE)
							{
								LONGLONG llNas = 0;
								(*istr) >> llNas;
								CTString strString;
								CTString strTemp;
								strTemp.PrintF("%I64d", llNas);
								pUIManager->InsertCommaToString(strTemp);

								strString.PrintF(_S(7065, "?????????? ?????? ?????? ???? ???? ???? ??????. %s ?????? ??????"), strTemp);
								pUIManager->CreateMessageBoxL( _S(3908, "???? ????"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, strString, -1, 0xa3a1a3ff);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(191, "????" ), DRATAN_SIEGE_DUNGEON_CHANGE_LORDMODE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 139, "????" ) );
							}
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_INFO:				//???? ???? ???? ???? ????
						{
							ULONG ulCurRate;
							LONGLONG llNas;
							(*istr) >> ulCurRate;
							(*istr) >> llNas;

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "?????? ???????? ?? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								return;
							}
														
							SERVER_INFO()->SetDG_EnvNas(llNas / 10); // 10% ?????? ???????? ?????? ?????? ????
							pUIManager->GetPortal()->Create_SiegeDungeon_EnvCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_MONSTER_INFO:			//???? ???? ?????? ???? ????
						{
							ULONG ulCurRate;
							LONGLONG llNas;
							(*istr) >> ulCurRate;
							(*istr) >> llNas;

							SERVER_INFO()->SetDG_MonsterNas(llNas / 10); // 10% ?????? ???????? ?????? ?????? ????

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "?????? ???????? ?? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								return;
							}

							pUIManager->GetPortal()->Create_SiegeDungeon_MonCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_TAX_INFO:				//?????? ???? ???? ????
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							pUIManager->GetPortal()->Create_SiegeDungeon_AdmissionCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_INFO:		//?????? ???? ????
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							pUIManager->GetPortal()->Create_SiegeDungeon_HuntingCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_CHANGE:				//???? ???????? ????
					case MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE:			//???? ???? ?????? ???? ????
					case MSG_MANAGEMENT_MANAGER_TAX_CHANGE:				//?????? ???? ????
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE:		//?????? ????
						{
							(*istr) >> ubError;
							if( ubError == MSG_DVD_ERROR_OK )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3944, "?????? ?????? ??????????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_MONEY)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "?????? ??????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					case MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL:
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CreateMessageBoxL( _S(3908, "???? ????"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(7060, "???? ?????? ?????? ... ???? ???? ?????? ???? ???? ???? ?????? ????????????."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3921, "???? ???? ???? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3926, "???? ???? ?????? ????" ), DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 1220, "????????." ) );

								SERVER_INFO()->SetDG_Mode(eDUNGEON_MODE_NOMAL);
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7074, "?????? ???? ?????? ?? ?????????? ???? ??????, ???? ???? ?????? ???????? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_NORMAL_MODE )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7071, "?????? ???? ???? ?????? ???? ?????? ???????? ???? ?????? ???????? ???????????? ???? ?????? ???? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}							
							else if (ubError == MSG_DVD_ERROR_ZONE)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(6275, "???? ?????????? ?????? ??????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					case MSG_NAMAGEMENT_CHANGE_OWNER_MODE:
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7069, "?????????? ?????? ?????????? ???????? ???????? ???? ???? ?? ??????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3942, "???? ?????? ??????, ???? ?????????????? ?????? ?? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_MONEY)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7070, "?????? ???? ???? ?????? ???? ?? ????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					}
				}
				break;
			case MSG_DUNGEON_ENTER:
				{
					UBYTE ubSubSubType;
					(*istr) >> ubSubSubType;

					switch( ubSubSubType )
					{
					case MSG_DUNGEON_ENTER_FEE:
						{
							__int64 ulNeedNas;
							(*istr) >> ulNeedNas;

							if( ulNeedNas <= 0 )
							{
								_pNetwork->SendDratanSiegeDungeonMSG(MSG_DUNGEON_ENTER, MSG_DUNGEON_GO);
								return;
							}

							if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ENTER) )
								pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENTER);
							
							strMessage.PrintF( _S( 191, "????" ) );
							MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_ENTER);

							CTString strNas;
							strNas.PrintF("%I64d", ulNeedNas);
							pUIManager->InsertCommaToString(strNas);
							strMessage.PrintF( _S(3945, "???????? ???????? ???????? %s?????? ??????????. ???? ?????????????" ), strNas );
							MsgBoxInfo.AddString(strMessage);

							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
						break;
					case MSG_DUNGEON_GO:
						{
							UBYTE ubError;
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_MONEY )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "?????? ??????????.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								pUIManager->SetCSFlagOff(CSF_TELEPORT);
							}
						}
					}
				}
				break;
			case MSG_GUILD_NAME_COLOR:
				{
					ULONG ulCharIndex;
					UBYTE ubCol;

					(*istr) >> ulCharIndex;
					(*istr) >> ubCol;

					if( ulCharIndex == _pNetwork->MyCharacterInfo.index)
					{				
						_pNetwork->MyCharacterInfo.ubGuildNameColor = ubCol;		
					}
					else
					{
						ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, ulCharIndex);

						if (pObject != NULL)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							pTarget->cha_ubGuildNameColor = ubCol;
						}
					}
				}
				break;

			case MSG_CHANGE_NORMAL_NOTICE: // ???????? ????
				{
					strMessage = _S(7067, "???????? ???????? ?????? ???? ???????? ???? ?????? ???????? ?????? ???? ?? ?????? ???????? ?????? ???? ???? ?? ????????. ");
					_UIAutoHelp->SetGMNotice(strMessage);
					_pNetwork->ClientSystemMessage(strMessage);
				}
				break;

			case MSG_CHANGE_NORMAL_NOTICE_TO_OWNER:
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(7068, "???????? ???????? ?????? ???? ?????? ???????? ???? ???? ???? ???????? ?????? ???? ???? ??????, ???????? ???? ???? ????????. ?????? ?????? ???? ???????????? ???? ???????? ?????? ?????????? ???? ?????? ???????? ?????? ???? ?? ????????.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
		//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_EX_PET_TURNTO_NPC: // ?? ????
		{
			SLONG slPetIndex;
			SLONG slNpcIndex;
			SLONG slCharIndex;
			SLONG slNpcSize;

			(*istr) >> slPetIndex >> slNpcIndex >> slCharIndex >> slNpcSize;

			CEntity* penEntity = NULL;

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, slPetIndex);
			CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();

			if (pObject != NULL)
			{
				penEntity = pObject->GetEntity();
			}

			if (penEntity != NULL)
			{
				if (penEntity == pPetInfo->pen_pEntity) // ?????? ?????? ???? ????
					pPetInfo->TransformIndex = slNpcIndex;

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(penEntity, slNpcIndex, slNpcSize); // 10?? ?????? 1??
			}
		}
		break;
	
	// [080422: Su-won] EVENT_CHAOSBALL
	case MSG_EX_CHAOSBALL:
		{
			(*istr) >> errcode;

			switch(errcode)
			{
			case MSG_CHAOSBALL_ERROR_SUCCESS:
				break;
			case MSG_CHAOSBALL_ERROR_ITEM_COUNT:
				break;
			case MSG_CHAOSBALL_ERROR_NOSPACE:
				pUIManager->GetChattingUI()->AddSysMessage( _S( 265,  "?????? ???????? ?????? ??????????." ), SYSMSG_ERROR );
				break;
			case MSG_CHAOSBALL_ERROR_ITEM_FAULT:
				break;
			}			
		}
	case MSG_EX_ATTACK_PET:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			ReceiveExWildPetMessage(ubSubType, &(*istr));

		}break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSG_EX_EXTREME_CUBE:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_EX_EXTREME_CUBE_STATE_REP:
				{
					pUIManager->GetCubeRank()->CreateCubeState(istr, TRUE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP:
				{
					pUIManager->GetCubeRank()->CreateCubeState(istr, FALSE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_COMPLETE:
				{
					INDEX ubCnt;
					(*istr) >> ubCnt;

					//pUIManager->GetCombo()->SetComboCount(ubCnt);
					pUIManager->GetCombo()->StageComplete(ubCnt,1);
					pUIManager->GetCombo()->SetSysImage(SYS_STAGE, TRUE, CUBE_TYPE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_ERROR:
				{
					UBYTE ubErrorType;
					(*istr) >> ubErrorType;

					BOOL bShowBox = TRUE;

					switch( ubErrorType )
					{
					case MSG_EX_EXTREME_CUBE_ERROR_NOITEM:
						{
							strMessage = _S(1330, "???????? ??????????.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT:
						{
							strMessage = _S(4374, "???? ?????? ?? ???? ???? ???????? ????????. ???? ?? ???? ???????? ????????.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE:
						{
							strMessage = _S(4375, "???? ?????? ?????? ?? ????????. ???? ?? ???? ???????? ????????.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_CAHNGE_RATE:
						{
							strMessage = _S(4376, "???????? ?????? ?????? ?? ????????.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME:
						{
							strMessage = _S(4377, "???? ???? ???? ?????? ????????.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME:
						{
							strMessage = _S(4397, "???? ???? ???? ?????? ????????.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_PARTYCUBE:
						{ // ???? ???? ????
							strMessage = _S(4398, "???? ?????? ??????????????.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_PARTYCUBE;
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_GUILDCUBE:
						{ // ???? ???? ????
							strMessage = _S(4399, "???? ?????? ??????????????.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_GUILDCUBE;
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE: // ???????? ??????????
						{
							strMessage = _S(4401, "?????????? ???????? ?????????? ??????????????.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE: // ???????? ??????????
						{
							strMessage = _S(4402, "?????????? ???????? ?????????? ??????????????.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME: // ???????? ???? 1???? ?????? 
						{
							strMessage = _S(4403, "1?? ?? ?????????? ???????? 1?? ?? ?????????? ??????????.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME: // ???????? ???? 1???? ??????
						{
							strMessage = _S(4404, "1?? ?? ?????????? ???????? 1?? ?? ?????????? ??????????.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_INVEN_NOSPACE:		// ???? ????????
						{
							strMessage = _S(265, "???????? ?????? ??????????.");
						}break;
					case MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_CANNOT:	// ???????? ???? ??????
						{
							strMessage = _S(1722, "?????? ?????? ?? ???? ?????? ????????");
						}break;
					case MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_ALREADY:	// ???????? ???? ??????	
						{
							strMessage = _S(1755, "???? ?????? ??????????.");
						}break;
					}
					
					if (bShowBox)
					{
						MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
					else
					{
						_pNetwork->ClientSystemMessage(strMessage);
					}
				}
				break;
			}
		}
		break;
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	
	case MSG_EX_INIT_SSKILL:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubSubType)
			{
			case MSG_EX_INIT_SSkill_ERROR_SUCCESS:
				{
					MsgBoxInfo.AddString(_S(4247, "?????? ???? ???????? ??????????????."));
				}
				break;
			case MSG_EX_INIT_SSkill_ERROR_FAIL:
				{
					MsgBoxInfo.AddString(_S(4248, "?????? ???? ???????? ??????????????."));
				}
				break;
			}

			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EX_TRIGGER_EVENT:
		{
			UBYTE ubSubType;
			INDEX iTouchID, iPlayID, iContinued;
			(*istr) >> ubSubType;
			(*istr) >> iTouchID >> iPlayID >> iContinued;

			switch(ubSubType)
			{
			case MSG_EX_TRIGGER_EVENT_ERROR: // ???? ????
				break;
			case MSG_EX_TRIGGER_EVENT_ALREADY_PLAY: // ???? ?????? ??
				break;
			case MSG_EX_TRIGGER_EVENT_NO_CONDITION: // ?????? ?????? ???? (???? ?????? ???????? ?????? ???????? ?????)
				break;
			case MSG_EX_TRIGGER_EVENT_TRIGER_EXIST_AREA: // ?? ?????? ???? ?????? ???? ?????????? ?????? ???????? ????.(Sub Info : 1. Obj count, 2. Objects ID
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, istr);
				break;
			case MSG_EX_TRIGGER_EVENT_ITEM_COMPOSITION: // ?????? ???? ????
				pUIManager->GetChattingUI()->AddSysMessage(_S(4669, "?????? ???? ????"));
				break;
			case MSG_EX_TRIGGER_EVENT_ITEM_DROP: // ???????? ??????( ???? ???????? ?????? ?????? ?????? ?????? ?????????)
				pUIManager->GetChattingUI()->AddSysMessage(_S(4670, "?????? ???? ????"));
				break;
			case MSG_EX_TRIGGER_EVENT_OPEN_THE_DOOR: // Door object?? ???? ????.(????????)
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, NULL);
				break;
			case MSG_EX_TRIGGER_EVENT_START_RAID: // ?????? ?????? ?????? ????
				{				
					//pUIManager->GetChattingUI()->AddSysMessage(_s("?????? ?????? ????"));					
				}
				break;
			case MSG_EX_TRIGGER_EVENT_MESSAGE_PRINT: // ???? ???????? ????(???? ???????? ???????? ?????? ?????????? ??)
				{				
					// [090709: selo] ?????? ???????? ???? ?????????? ?????? ???????? ????????
					INDEX iStringIndex = 0;
					(*istr) >> iStringIndex;
					
					if (iStringIndex > 0)
					{
						GAMEDATAMGR()->GetQuest()->AddRaidMessage(iStringIndex);
					}
				}
				break;
			case MSG_EX_TRIGGER_EVENT_SUMMON_NPC: // npc?? ????????.(???? npc?? ???????? ?????? ?????? ???? ????.)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("npc???? ????"));
				break;
			case MSG_EX_TRIGGER_EVENT_USE_DEBUF: // ???????? ???????? ??????.(?????? ????????)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("?? ???????? ??????~"));
				break;
			case MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT: // ?????????? ??????(???????????? ????????.)
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, NULL);
				break;
			case MSG_EX_TRIGGER_EVENT_REMOVE_NPC: // npc ???? ???? (npc?? ????????. ?? ?????????)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("npc ????~"));
				break;
			case MSG_EX_TRIGGER_EVENT_TIME_LIMIT:
				{
					// [090707: selo] 
					// ???? ?????? ?????? 0 ???????? ???? ?????? ????????.
					INDEX iRemainTime = 0;
					(*istr) >> iRemainTime;

					// 0 ???? ?????? ?????? ???? ??
					if( 0 == iRemainTime )
					{
						GAMEDATAMGR()->GetQuest()->RaidEnd();
					}
					// -1 ???? ?????? ???????? ?????? ???? ??
					else if( -1 == iRemainTime )
					{
						GAMEDATAMGR()->GetQuest()->RaidEnd();
					}
					else
					{
						// [090907: selo] ?????? ???? ?????? ??????.
						GAMEDATAMGR()->GetQuest()->SetRaidRemainTime(iRemainTime);	
				}	}
				break;
			case MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT:
				{
					CEntity *tmpEntity = NULL;

					tmpEntity = _pNetwork->ga_World.EntityFromID(iPlayID);

					if (tmpEntity != NULL)
					{
						tmpEntity->Precache();
					}
				}
				break;
			}
		}
		break;
	case MSG_EX_AFFINITY:
		{
			ReceiveAffinityMessage( istr );
		}
		break;
		
	case MSG_EX_TUTORIAL:
		{
			pUIManager->GetHelp()->OpenTutorial();
		}	
		break;
		
	case MSG_EX_LOADING_END:
		{
			ReceiveExLoadingEndMessage( istr );
		}
		break;

	case MSG_EX_NPC_PORTAL_SCROLL:
		{
			ReceiveNPCPortalMessage( istr);
		}
		break;
		
	case MSG_EX_RAID_INFO:
		{
			SLONG nRaidCount = 0;
			SLONG nZoneNum = 0;
			SLONG nRoomNum = 0;

			(*istr) >> nRaidCount;

			if(pUIManager->DoesMessageBoxExist(MSGCMD_RAID_INFO))
				pUIManager->CloseMessageBox(MSGCMD_RAID_INFO);

			MsgBoxInfo.SetMsgBoxInfo( _S(4754, "?????? ?????? ?? ????"), UMBS_OK, UI_NONE, MSGCMD_RAID_INFO);

			if(nRaidCount > 0)
			{
				for( i = 0; i < nRaidCount; i++ )
				{
					(*istr) >> nZoneNum;
					(*istr) >> nRoomNum;

					strMessage.PrintF(_S(4755, "?????? ?????? ?? ???? : %s"),CZoneInfo::getSingleton()->GetZoneName(nZoneNum));
					MsgBoxInfo.AddString(strMessage);
					strMessage.PrintF(_S(4756, "?????? ?????? ?? ID : %d"), nRoomNum);
					MsgBoxInfo.AddString(strMessage);
				}
			}
			else
			{
				MsgBoxInfo.AddString(_S(4757, "?????? ???? ????????."));
			}

			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;	
		
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM:
		{
			ReceiveTakeAgainQuestItem( istr);						
		}
		break;
	case MSG_EX_COSTUME2:
		{
			UCHAR ucType;
			(*istr) >> ucType;
			if ( ucType == MSG_EX_COSTUME2_SYSTEM_ERROR )
			{
				RecieveCostume2Message( istr );
			}
			else if ( ucType == MSG_EX_COSTUEM2_WEARING )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingCostumeArmor( istr );
			}
			// added by sam 11/02/01 [SAM]
			else if ( ucType == MSG_EX_COSTUME2_WEARING_SUIT )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingCostumeArmorOneSuit( istr );
			}
			else if ( ucType == MSG_EX_COSTUME2_PREINVENINFO )
			{
				SBYTE	wear_type;	
				INDEX	uni_index;
				INDEX	item_index;
				CEntity			*penPlEntity;
				CPlayerEntity	*penPlayerEntity;

				penPlEntity = CEntity::GetPlayerEntity(0);		// ?????? ???? ????
				penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
				
				CModelInstance *pMI	= penPlayerEntity->GetModelInstance();

				if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
				{
					CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();
					
					INDEX ctmi = pMITemp->mi_cmiChildren.Count();
					if( ctmi > 0 )
					{
						pMI = &pMITemp->mi_cmiChildren[0];
					}
					else
					{
						pMI	= penPlayerEntity->GetModelInstance();
					}
				}
				
				for (int i=0; i<WEAR_COSTUME_TOTAL; ++i)
				{
					(*istr) >> wear_type; // 0 ~ 6, 11
					(*istr) >> uni_index;
					(*istr) >> item_index;
					
					if ( item_index >= 0 )
					{	// ?????? ????
						_pNetwork->MyWearCostItem[i].Item_Index = item_index;
						_pNetwork->MyWearCostItem[i].Item_UniIndex = uni_index;
						
						// BUGFIX : ITS(#0002472) ?????? 2 ???? ?? ???? -> ???????? ???? ???? ???? ????. [6/9/2011 rumist]
						if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 )
						{
							if ( !( wear_type == WEAR_HELMET && (CTString)_pNetwork->GetItemData(item_index)->GetItemSmcFileName() == MODEL_TREASURE) )
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DeleteDefaultArmor(wear_type);
								_pGameState->WearingArmor(pMI, *_pNetwork->GetItemData(item_index));
							}
							if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
								_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job, _pNetwork->GetItemData(item_index), wear_type,
								0, &pMI->m_tmSkaTagManager, 1, _pNetwork->GetItemData(item_index)->GetSubType());
						}
					}
				}
			}
		}
		break;
	case MSG_EX_DUNGEONTIME:
		{
			UCHAR ucType;
			SLONG slStart, slEnd, slAddExp;
			CTString strMessage;
			(*istr) >> ucType;

			if (ucType == MSG_EX_DUNGEONTIME_NOTICE)
			{
				(*istr) >> slStart >> slEnd >> slAddExp;
				strMessage.PrintF(_S(4956, "???? ?????? ??????????????. (%d?? ~ %d??, ???? ?????? %d%%???? ????)"), slStart, slEnd, slAddExp);
				_UIAutoHelp->SetGMNotice(strMessage);
				pUIManager->GetSimplePlayerInfo()->StartDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(slAddExp);
			}
			else if (ucType == MSG_EX_DUNGEONTIME_START)
			{
				(*istr) >> slStart >> slEnd >> slAddExp;
			//TO.DO ???? ????
				strMessage.PrintF(_S(4957, "???? ?????? ???? ????????. (%d?? ~ %d??, ???? ?????? %d%%???? ????)"), slStart, slEnd, slAddExp);
				_UIAutoHelp->SetGMNotice(strMessage);
				pUIManager->GetSimplePlayerInfo()->StartDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(slAddExp);
			}
			else if (ucType == MSG_EX_DUNGEONTIME_END)
			{
				_UIAutoHelp->SetGMNotice( _S(4958, "?????????? ???? ??????????.") );
				pUIManager->GetSimplePlayerInfo()->StopDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(0);
			}
		}
		break;
	case MSG_EX_SOCKET:
		{
			RecieveSocketSystemMessage( istr );
		}
		break;
#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    case MSG_EX_XTRAP:
        {
            if (g_pXTrap_CS_Step2Func)
            {
                CNetworkMessage nmMessageTest(MSG_EXTEND);
                #define XTRAP_CMD_STEP_TWO  0x002
                nmMessageTest << (ULONG)MSG_EX_XTRAP;
                nmMessageTest << (BYTE)XTRAP_CMD_STEP_TWO;
                char szBuf[128] , szSendBuf[128];
                
				istr->Read(szBuf, 128);
#ifdef XTRAP_LOG_ENABLE
				g_pHexaDump4XTrap_V1("TestXtrapLog.txt", szBuf, 128, "XTrap_stap1_RecBufData");
#endif
                g_pXTrap_CS_Step2Func(szBuf, szSendBuf);

                nmMessageTest.Write(szSendBuf , 128 );
#ifdef XTRAP_LOG_ENABLE
				g_pHexaDump4XTrap_V1("TestXtrapLog.txt", szSendBuf, 128, "XTrap_stap2_SendBufData");
#endif
                _pNetwork->SendToServerNew( nmMessageTest );
            }
        }
        break;
#endif
		case MSG_EX_TUTORIAL_RENEWER_2010:
		{
			pUIManager->GetHelp()->RepBeginnerTitle( istr );
		}
		break;
	case MSG_EX_RAID_SCENE: // ???????? ??????
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveRaidScene(istr);
		}
		break;

	case MSG_EX_CASTLLAN:
		{
			(*istr) >> errcode;
			if ( errcode == MSG_EX_CASTLLAN_ERROR )
			{
				RecieveLordCostumeMessage( istr );
			}
		}
		break;

	case MSG_EX_SUBJOB:	// [2010/08/25 : Sora] ADD_SUBJOB ???? ???? ??????
		{
			RecieveSubJobMessage(istr);
		}
		break;
	case MSG_EX_RANKING_SYSTEM_EX:		// ???? ?????? ???? [trylord : 110825]
		{
			RecieveRankingListExMessage(istr);
		}
	// [2010/10/20 : Sora] ?????? ???? ????
	case MSG_EX_MONSTER_MERCENARY:
		{
			RecieveMonsterMercenaryCardMessage( istr );
		}
		break;
	case MSG_EX_FACEOFF:
		{
			(*istr) >> errcode;
			if ( errcode == MSG_EX_FACEOFF_REP )
			{
				RecieveFaceOffMessage( istr );
			}
		}
		break;
	case MSG_EX_LUCKYDRAW_BOX:
		{
			RecieveLuckyDrawBoxMessage( istr );
		}
		break;
		// ?????? ???? ???? ?????? ???? added by sam 11/01/04
	case MSG_EX_MSGBOX:
		{	
			RecieveMsgBoxShow( istr );					
		}
		break;

	case MSG_EX_LCBALL:
		{
			pUIManager->GetLacaBall()->ReceiveLcBallMessage( istr );
		}
		break;
	// royal rumble [4/19/2011 rumist]
	case MSG_EX_ROYAL_RUMBLE:
		{
			RecieveRoyalrumbleMessage( istr );
		}
		break;
	case MSG_EX_CASH_AUTHENTICATION_CODE:
		{
			CTString strUserID;
			CTString strCode;
			UBYTE nGrupID,nAuthCode;
			(*istr) >> nAuthCode;
			

			if (nAuthCode)
			{
				(*istr) >> strUserID;
				(*istr) >> nGrupID;
				(*istr) >> strCode;

				CTString temURL;
				extern ENGINE_API INDEX sam_bFullScreenActive;				
				
				if ( IsFullScreen( sam_bFullScreenActive))
				{												 
					 pUIManager->GetOption()->ChangeWindowMode();
					 pUIManager->DestroyRenderTarget();
					 pUIManager->InitRenderTarget();
				}
				
				temURL.PrintF("http://shop.gamigo.com/game_%d/login/user/%s/hash/%s/", nGrupID, strUserID, strCode);
				ShellExecute( NULL, "open", temURL, NULL,NULL, SW_SHOWNORMAL );
			}else
			{
				MsgBoxInfo.SetMsgBoxInfo( _S( 191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 3257, "?????? ?????? ???? ??????????." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			
		}break;
	case MSG_EX_TREASURE_MAP_SYSTEM_KEY_EXCHANGE:
		{
			UBYTE ubErrorcode;
			(*istr) >> ubErrorcode;
			
			if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_SUC) // ????
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 159, "?????? ?????????? ??????????????." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_FAIL) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 3257, "?????? ?????? ???? ??????????." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_ITEM) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5099, "???? ?????? ???????? ????????" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_COUNT) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5100, "???? ?????? ???????? ??????????" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}

		}break;
	case MSG_EX_PKPENALTY_REFORM:
		{
			UBYTE ubinx;
			CTString strSysMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> ubinx;
			if(ubinx == MSG_EX_PKPENALTY_REFORM_REWARD_INFO)
			{
				ULONG pkSysRewardFlag;
				(*istr) >> pkSysRewardFlag;
				_pNetwork->MyCharacterInfo.pkSysRewardFlag = pkSysRewardFlag;
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= FALSE;
			}
			else if(ubinx == MSG_EX_PKPENALTY_REFORM_REWARD_REP)
			{
				INDEX pkSysRewardFlag;
				UBYTE errcode;
				(*istr) >> pkSysRewardFlag;
				(*istr) >> errcode;
				switch(errcode)
				{
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_SUC:
					_pNetwork->MyCharacterInfo.bpkSysRewardLate= FALSE;
					break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_COUNT:
					strSysMessage = _S(2237, "???????? ?????? ??????????.");
					break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_ONCE:
					//strSysMessage = _S(4827, "???? ???????? ???? ??????????.");
				    break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL:
					strSysMessage = _S(1745, "?????? ???? ????????.");
				    break;
				default:
				    break;
				}
				
			}
			if (strSysMessage != CTString(""))
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strSysMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}break;

	case MSG_EX_SHUTDOWN:
		{
			UCHAR type;
			SLONG arg;
			CTString strMessage;
			(*istr) >> type;
			(*istr) >> arg;
			
			switch( type )
			{
				case MSG_EX_SHUTDOWN_REMAIN:
					{
						strMessage.PrintF( _s("Shut Down ???????? %d?? ??????????."), arg );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_COUNTDOWN:
					{
						strMessage.PrintF( _s("Shut Down ???????? %d?? ??????????. ?????????? ???? ?????? ???????? ?????? ???????? ?????????? ????????."), arg );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_START:
					{
						strMessage.PrintF( _s("Shut Down?? ???? ??????????.") );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_END:
					{
						strMessage.PrintF( _s("Shut Down?? ???? ??????????.") );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_DISCONNECT:
					{
						CUIMsgBox_Info	MsgBoxInfo;
						
						strMessage.PrintF( _s("???????? ??????????????. ?????? ???? ???? ?????? ???? ????????????. ?????? ???? ???????????? ?????????? ????????. (?????? ???????? : 24:00-06:00)") );
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
					break;
			}

		}break;
#ifdef	IMPROV1107_NOTICESYSTEM
	case MSG_EX_USER_NOTICE:
		{
			LONG	retCode;

			(*istr) >> retCode;
			switch(retCode)
			{
			case MSG_EX_USER_NOTICE_REG_REP:		// ????
				{
					pUIManager->GetChattingUI()->AddSysMessage(_S( 5611 ,"?????????? ??????????????."), SYSMSG_NORMAL);
				}
				break;

			case MSG_EX_USER_NOTICE_PRINT:
				{
					CTString	strName;
					CTString	strMessage;

					(*istr) >> strName;
					(*istr) >> strMessage;
					pUIManager->GetChattingUI()->AddUserNotice(strName, strMessage);
				}
				break;

			case MSG_EX_USER_NOTICE_ERROR:
				{
					LONG		errcode;
					CTString	strSysMessage;

					(*istr) >> errcode;
					if(errcode == MSG_EX_USER_NOTICE_ERROR_NOITEM)
					{
						strSysMessage	= _S( 1330 ,"???????? ??????????." );
					}
					else if(errcode == MSG_EX_USER_NOTICE_ERROR_LENGTH)
					{
						strSysMessage	= _S( 5609 ,"?????? ?????? ??????????." );
					}

					if (strSysMessage.Length() > 0)
					{
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
						MsgBoxInfo.AddString(strSysMessage);
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}

				}
				break;
			}
		}
		break;
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

//////////////////////////////////////////////////////////////////////////
// ?????? ?????? ????
//////////////////////////////////////////////////////////////////////////
	case MSG_EX_EVENT_AUTOMATION_SYSTEM:
		{
			extern ENGINE_API std::map<int,int> g_mapEvent;

			UCHAR type;
			SLONG nEventIndex;
			(*istr) >> type;

			switch( type )
			{
			case MSG_EX_EVENT_AUTOMATION_LIST:
				{
					SLONG nCount;
					(*istr) >> nCount;

					for( int i=0; i<nCount; i++ )
					{
						(*istr) >> nEventIndex;
						g_mapEvent[nEventIndex] = 1;
					}

					CheckTimerEvent();
				}
				break;
			case MSG_EX_EVENT_AUTOMATION_ADD:
				(*istr) >> nEventIndex;
				g_mapEvent[nEventIndex] = 1;

				CheckTimerEvent();
				break;
			case MSG_EX_EVENT_AUTOMATION_REMOVE:
				(*istr) >> nEventIndex;
				g_mapEvent.erase(nEventIndex);

				CheckTimerEvent();
				break;
			}
		}
		break;
	case MSG_EX_JUMPING_SYSTEM:
		{
			UCHAR type;
			CTString	strSysMessage;
			INDEX nJumpingLevel;

			(*istr) >> type;

			switch( type )
			{
			case MSG_EX_JUMPING_SUCCESS:
				{
					strSysMessage.PrintF(_S(5689, "?????????? ???? ??????????."));
				}break;
			case MSG_EX_JUMPING_ERROR_LEVEL:
				{
					strSysMessage.PrintF(_S(5685, "?????? ???? ???? ???????? ?? ????????."));
				}break;
			case MSG_EX_JUMPING_ERROR_NOT_EXIST_CHAR:
				{
					(*istr) >> nJumpingLevel;
					strSysMessage.PrintF(_S(5690, "???? ???? ???? ???? ???? ???? %d ???? ?????? ???????? ???????? ????????."), nJumpingLevel);
				}break;
			}

			MsgBoxInfo.SetMsgBoxInfo( _S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString(strSysMessage);
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;

	//REFORM_SYSTEM
	case MSG_EX_REFORMER_SYSTEM:
		{
			pUIManager->GetReformSystem()->RecvReformSystemResult(istr);
		}break;

	case MSG_EX_SERVER_TIME:
		{
			(*istr) >> nServerTime;

			_pNetwork->slServerTime = nServerTime;
			_pNetwork->slServerTimeGap = (SLONG)time(NULL) - nServerTime;
		}break;
	}
}

void CSessionState::ReceiveExWildPetMessage(UBYTE index, CNetworkMessage *istr)
{	
//	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
// 	penPlEntity = CEntity::GetPlayerEntity(0); //?????? ???? ????
// 	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	penPlayerEntity = static_cast<CPlayerEntity*>( CEntity::GetPlayerEntity(0) );

	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	switch(index)	
	{
	case MSG_SUB_ITEM_WEAR:
		{
			SBYTE wear_pos;
			INDEX wear_item,takeoff_item;

			(*istr) >> wear_pos;
			(*istr) >> wear_item;
			(*istr) >> takeoff_item;

			// ?? ???? ?????? ???? ????????.
			if (pInfo->GetMyApetInfo() == NULL)
				return;

			CWildPetTarget* pTarget = pInfo->GetMyApetInfo();
			
			if (pTarget->bIsActive == FALSE)
			{
				break;
			}

			if(pTarget->m_nPetWearIndex[wear_pos] == takeoff_item)
			{
				if (pTarget->GetEntity() != NULL)
					_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), takeoff_item);
				
				pTarget->m_nPetWearIndex[wear_pos] = -1;
			}

			if (pTarget->m_nPetWearIndex[wear_pos] > 0)
			{
				if (pTarget->GetEntity() != NULL)
					_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[wear_pos]);
			}			
						
			pTarget->m_nPetWearIndex[wear_pos] = wear_item;

			if (pTarget->GetEntity() != NULL)
				_pGameState->WearingArmorTest(pTarget->GetEntity()->GetModelInstance(), wear_item);
			
			pUIManager->GetWildPetInfoUI()->PetWearItemReSet();
			pUIManager->GetWildPetInfoUI()->updateBaseTab();
		}
		break;
	case MSG_SUB_ITEM_WEARING:
		{
			INDEX OwenerIndex,petID;
			INDEX wearIndex[WILDPET_WEAR_TOTAL] = {0,};
			INDEX wearPlus[WILDPET_WEAR_TOTAL] = {0,};

			(*istr) >> OwenerIndex;
			(*istr) >> petID;

			for (int i = 0; i < WILDPET_WEAR_TOTAL; i++)
			{
				(*istr) >> wearIndex[i];
				(*istr) >> wearPlus[i];
			}

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, petID);

			if (pObject != NULL)
			{
				CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

				for (int i = 0; i < WILDPET_WEAR_TOTAL; i++)
				{
					if (pTarget->GetEntity() != NULL)
						_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[i]);

					pTarget->m_nPetWearIndex[i] = wearIndex[i];
					pTarget->m_nPetWearPlus[i] = wearPlus[i];

					if (pTarget->GetEntity() != NULL)
						_pGameState->WearingArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[i]);
				}

				// ?? ???????? (&_WildPetInfo != &_pNetwork->ga_srvServer.srv_actWildPet)
				if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
				{
					pInfo->SetMyApet(pTarget);
					pUIManager->GetWildPetInfoUI()->PetWearItemReSet();
					pUIManager->GetWildPetInfoUI()->updateBaseTab();
				}
			}
		}
		break;

	case MSG_SUB_SKILLLEAN:
		{
			CTString	strMessage;
			UBYTE code;
			(*istr) >> code;

			switch(code)
			{
			case 0:
				{	LONG nSkillIndex;
					UBYTE nSkillLevel;
				
					(*istr) >> nSkillIndex;
					(*istr) >> nSkillLevel;
					
					pUIManager->GetPetTrainingUI()->LearnSkill( 0, nSkillIndex, nSkillLevel );

					CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );
					strMessage.PrintF( _S( 277, "%s ?????? ??????????????" ), rSkill.GetName() );

				}break;
			case 1:
				{
					//???? ?? ????
					strMessage = _S(2203, "?????????? ???????? ???? ?????? ?????? ?? ????????." );
				}break;
			case 2:
				{
					//?????? ????
					strMessage = _S( 278, "?????? ???????? ?????? ?????? ?? ????????." );
				}break;
			case 3:
				{
					//???? ????
					strMessage = _S( 281, "???? ?????? ???? ???? ?????? ?????? ?? ????????." );
				}break;
			case 4:
				{
					//???? ?????? ????
					strMessage = _S( 280, "???????? ???????? ???? ?????? ?????? ?? ????????." );
				}break;
			case 5:
				{
					//???? ?????? ????
					strMessage = _S(4214, "???? ???????? ???????? ?????? ?????? ?? ????????. ");
					
				}break;

			}
			// Close message box of skill learn
		pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

		// Create message box of skill learn
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "????" ), UMBS_OK,
									UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );


		}break;
	case MSG_SUB_SKILLLIST:
		{
			INDEX nSkillCont;
			INDEX nSkillIndex;
			UBYTE nSkillLevel;
			int nSkillTime;
			(*istr) >> nSkillCont;
			
			pUIManager->GetWildPetInfoUI()->SkillClear();
			
			for(int con = 0; con < nSkillCont; con++)
			{
				(*istr) >> nSkillIndex;
				(*istr) >> nSkillLevel;
				(*istr) >> nSkillTime;

				pUIManager->GetPetTrainingUI()->LearnSkill(0, nSkillIndex, nSkillLevel, FALSE, false);

				CSkill& rSkill = _pNetwork->GetSkillData(nSkillIndex);
				if ( nSkillTime <= 0 )
				{
					rSkill.ResetStartTime();
					continue;
				}
				SLONG slRemainTime = ((SLONG)time(NULL) - _pNetwork->slServerTimeGap) - nSkillTime; 

				if ( slRemainTime > 0 )
					rSkill.SetStartTime(slRemainTime);
			}

		}break;
	case MSG_SUB_REBIRTH:
		{
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			UBYTE code;
			(*istr) >> code;
			switch(code)
			{
			case 0:
				{
					pUIManager->GetPetFree()->closeUI(); //????
					MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_PETFREE, UI_NONE );
					strMessage.PrintF( _S( 2450, "???? ?????? ?????????? ??????????????.") );
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}break;
			case 1:
				{
					//?????? ???? ??????
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2452, "?????? ?????? ???? ???????? ????????." ), SYSMSG_ERROR );
					pUIManager->GetPetFree()->ResetUI();
				}break;
			case 2:
				{
					//?????? ??????
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2451, "???? ?????? ???????? ???? ?????? ??????????." ), SYSMSG_ERROR );
				}break;
			case 3:
				{
					//???????? ???? ??????
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2452, "?????? ?????? ???? ???????? ????????." ), SYSMSG_ERROR );
					pUIManager->GetPetFree()->ResetUI();
				}break;
			}
		}break;
	case MSG_SUB_LEVELUP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2253, "???? ?????? ??????????." ), SYSMSG_NORMAL );
			
			if (pInfo->GetMyApetInfo() != NULL)
				WildPetStartEffectGroup("pet_levelup", pInfo->GetMyApetInfo()->m_nIdxServer, pInfo->GetMyApetInfo()->m_pEntity);

		}break;
	case MSG_SUB_DELETE_EQUIP:
		{
			pUIManager->GetWildPetInfoUI()->ReceiveDeleteEquip(istr);
		}break;

	case MSG_SUB_ADDITEM_MSG: // ???? ???? ???? ?? ?????? ?????? ?????? UI Update?? ?????? ????.
		{
			pUIManager->GetWildPetInfoUI()->ReceiveWearItemData(istr);	
		}break;

	case MSG_SUB_AI_LIST_NTF:
		{
			INDEX nPetAICount;
			INDEX nAIMaxCount;
			INDEX nSlotnum;
			UBYTE bActive;
			sWildPet_AI_Slot temAI;

			pUIManager->GetWildPetInfoUI()->AIClear();

			(*istr) >> nAIMaxCount;

			pUIManager->GetWildPetInfoUI()->SetPetAISlotCount(nAIMaxCount);

			(*istr) >> nPetAICount;

			for (int ncnt = 0; ncnt < nPetAICount; ncnt++)
			{
				(*istr) >> nSlotnum;
				(*istr)	>> temAI.m_nTargetSlot;
				(*istr)	>> temAI.m_nActionSlot;
				(*istr)	>> temAI.m_nSillType;
				(*istr)	>> bActive;

				temAI.m_bActive = bActive;

				pUIManager->GetWildPetInfoUI()->AddAIData(nSlotnum, temAI);

			}

			pUIManager->GetWildPetInfoUI()->PetAIReSet();
			pUIManager->GetWildPetInfoUI()->CloseAISetUI();

		}break;
	case MSG_SUB_AI_UPDATE_REP:
		{
			UBYTE ubErrorcode;
			(*istr) >> ubErrorcode;

			pUIManager->GetWildPetInfoUI()->ErrorMessage(ubErrorcode);

		}break;
	case MSG_SUB_AI_ONOFF_REP:
		{
			UBYTE bActive;
			UBYTE ubErrorCode;

			(*istr) >> bActive;
			(*istr) >> ubErrorCode;

			pUIManager->GetWildPetInfoUI()->SetPetAIActive(bActive);
			pUIManager->GetWildPetInfoUI()->ErrorMessage(ubErrorCode);			

		}break;
	case MSG_SUB_EVOLUTION: // ???? ???? ?? ???? ?????? ????
		{
			UBYTE ubErrorCode;

			(*istr) >> ubErrorCode;

			pUIManager->GetPetTrainingUI()->EvolutionError(ubErrorCode);
		}break;
	case MSG_SUB_MOUNT_REP:
		{
			UBYTE ubErrorCode;	// 0: ????, 1: ????

			(*istr) >> ubErrorCode;

			pUIManager->SetCSFlagOff(CSF_PETRIDING);
		}
		break;
	case MSG_SUB_SUMMON_ERROR:
		{
			UBYTE ubErrorCode;
			CTString strMessage;

			(*istr) >> ubErrorCode;

			strMessage.PrintF( _S(5212, "???????????? %d?? ??????????." ), ubErrorCode );						
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}break;
		// cash item ???? : p2??. [1/19/2011 rumist]
	case MSG_SUB_STATINIT:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;
			
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			switch( ubErrorCode )
			{
				case 0:
					MsgBoxInfo.AddString( _S( 5322, "?????? ???????? ???? ??????????." ) );
				break;
				case 1:
					MsgBoxInfo.AddString( _S( 2189,"?????????? ?????? ???????????? ??????.") );
				break;
				case 2:
					MsgBoxInfo.AddString( _S( 2444, "?? ???? ????." ) );
				break;
				case 3:
					MsgBoxInfo.AddString( _S( 5320, "?????? ?? ???????? ???????? ????????." ) );
				break;
			}
			
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}break;
	case MSG_SUB_EXPUSE:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			switch( ubErrorCode )
			{
			case 0:
				MsgBoxInfo.AddString( _S( 5651, "?????? ???????? ?????????? ??????????????." ) );
				break;
			case 1:
				MsgBoxInfo.AddString( _S( 5673,"???? ?? ???????? ???? ?? ?? ???? ???? ??????.") );
				break;
			case 2:
				MsgBoxInfo.AddString( _S( 4981, "???????? ????????????, ?????? ???????? ????????. ???? ?? ???? ?????? ?????? ????????." ) );
				break;
			case 3:
				MsgBoxInfo.AddString( _S( 2445, "?? ???????? ??????????." ) );
				break;
			}
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;
	case MSG_SUB_ERROR_NOT_EXIST_NPC:
		{
			CTString strMessage;
			strMessage.PrintF( _S( 5913, "?????????? NPC?? ?????? ???????? ????????.") );
			_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
		}
		break;
	case MSG_SUB_INIT_COOLTIME:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;

			MsgBoxInfo.SetMsgBoxInfo(_S(191, "????"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubErrorCode)
			{
			case 0:
				MsgBoxInfo.AddString(_S(6288, "???? ???? ?????? ???? ???????? ???????? ?????? ?????? ??????????."));
				break;
			case 1:
			case 2:
				MsgBoxInfo.AddString(_S(6289, "???????? ?????????? ???? ?????? ???? ???????? ?????? ????????."));
				break;
			default:
				return;
			}

			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_SUB_DELETE_ITEM:
		{
			UBYTE	error;
			ULONG	wearpos;
			(*istr) >> error;
			(*istr) >> wearpos;

			pUIManager->GetWildPetInfoUI()->DeleteEquip(wearpos);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePetStatusMessage()
// Desc : ???????? ????.
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetStatusMessage( CNetworkMessage *istr )
{
	// ???????? ????	: index(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n) x(f) z(f) h(f) r(f) y(c) mapatt(uc)
	LONG		lIndex;
	SBYTE		sbPetTypeGrade;
	LONG		lLevel;
	__int64		llExp;
	__int64		llNeedExp;
	LONG		lHP;
	LONG		lMaxHP;
	LONG		lAbility;
	LONG		lHungry;
	LONG		lMaxHungry;
	LONG		lSympathy;
	LONG		lMaxSympathy;
	FLOAT		fX;
	FLOAT		fZ;
	FLOAT		fH;
	FLOAT		fR;	
	SBYTE		sbYLayer;
	UWORD		sbMapAttribute;
	LONG		remainRebirth;
	CTString	strNameCard;

	(*istr) >> lIndex;
	(*istr) >> sbPetTypeGrade;
	(*istr) >> lLevel;
	(*istr) >> llExp;
	(*istr) >> llNeedExp;
	(*istr) >> lHP;
	(*istr) >> lMaxHP;
	(*istr) >> lAbility;
	(*istr) >> lHungry;
	(*istr) >> lMaxHungry;
	(*istr) >> lSympathy;
	(*istr) >> lMaxSympathy;
	(*istr) >> fX;
	(*istr) >> fZ;
	(*istr) >> fH;
	(*istr) >> fR;
	(*istr) >> sbYLayer;
	(*istr) >> sbMapAttribute;
#ifdef PET_SEAL_TIME
	(*istr) >> remainRebirth;
#endif

	(*istr) >> strNameCard;

	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= lIndex;
	TempPet.sbPetTypeGrade		= sbPetTypeGrade;
	TempPet.lLevel				= lLevel;
	TempPet.llExp				= llExp;
	TempPet.llNeedExp			= llNeedExp;
	TempPet.lHP					= lHP;
	TempPet.lMaxHP				= lMaxHP;
	TempPet.lAbility			= lAbility;
	TempPet.lHungry				= lHungry;
	TempPet.lMaxHungry			= lMaxHungry;
	TempPet.lSympathy			= lSympathy;
	TempPet.lMaxSympathy		= lMaxSympathy;
	TempPet.fX					= fX;
	TempPet.fZ					= fZ;
	TempPet.fH					= fH;
	TempPet.fR					= fR;	
	TempPet.sbMapAttribute		= sbMapAttribute;
#ifdef PET_SEAL_TIME
	TempPet.lRemainRebirth		= remainRebirth;
#endif
	TempPet.strNameCard			= strNameCard;

	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	
	// ?? ?????? ?? ?????? ??????.
	if( iter == _pNetwork->m_vectorPetList.end() )
	{
		_pNetwork->m_vectorPetList.push_back(TempPet);
	}	
	// ?? ?????? ??????.
	else
	{		
		(*iter).sbPetTypeGrade	= TempPet.sbPetTypeGrade;
		(*iter).lLevel			= TempPet.lLevel;
		(*iter).llExp			= TempPet.llExp;
		(*iter).llNeedExp		= TempPet.llNeedExp;
		(*iter).lHP				= TempPet.lHP;
		(*iter).lMaxHP			= TempPet.lMaxHP;
		(*iter).lHungry			= TempPet.lHungry;
		(*iter).lMaxHungry		= TempPet.lMaxHungry;
		(*iter).lSympathy		= TempPet.lSympathy;
		(*iter).lMaxSympathy	= TempPet.lMaxSympathy;
		(*iter).fX				= TempPet.fX;
		(*iter).fZ				= TempPet.fZ;
		(*iter).fH				= TempPet.fH;
		(*iter).fR				= TempPet.fR;		
		(*iter).sbMapAttribute	= TempPet.sbMapAttribute;
		(*iter).lAbility		= TempPet.lAbility;
#ifdef PET_SEAL_TIME
		(*iter).lRemainRebirth	= TempPet.lRemainRebirth;
#endif
		(*iter).strNameCard		= TempPet.strNameCard;
	}
	_pNetwork->UpdatePetTargetInfo( TempPet.lIndex );
	CUIManager::getSingleton()->GetPetInfo()->GetPetDesc();	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetMountMessage()
// Desc : ???????? ????.
// ----------------------------------------------------------------------------
// FIXME : ReceivePetAppearMessage, ReceiveStatusMessage, ReceivePetMountMessage ???????? ???? ???? ???? ????.
void CSessionState::ReceivePetMountMessage( CNetworkMessage *istr )
{
	// ?????????? ????	: owner_index(n) mountPetTypeGrade(c)
	LONG	lOwnerIndex;
	SBYTE	sbMountPetTypeGrade;

	(*istr) >> lOwnerIndex;
	(*istr) >> sbMountPetTypeGrade;

	// [070824: Su-won] PET_COLOR_CHANGE
	SBYTE	sbPetColor =0;
	(*istr) >> sbPetColor;

	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbMountPetTypeGrade, iPetType, iPetAge );
	
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
	
	// [070824: Su-won] PET_COLOR_CHANGE
	INDEX iPetColoredType = iPetType | (sbPetColor<<8);
	
	// ?? ????????...
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{			
		// ???? ?????? ???????? ???? ????...
		if( iPetType != -1 && 
			iPetAge != -1 && 
			bPetRide )
		{
			
			if( _pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE )
			{
				const INDEX iPetIndex = _pNetwork->MyWearItem[WEAR_PET].Item_Plus;
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= iPetIndex;

				ObjInfo* pInfo = ObjInfo::getSingleton();
				CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();
				
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					pPetInfo->iLevel		= (*iter).lLevel;
					pPetInfo->fHealth		= (*iter).lHP;
					pPetInfo->fMaxHealth	= (*iter).lMaxHP;
					pPetInfo->fMaxHungry	= (*iter).lMaxHungry;
					pPetInfo->fHungry		= (*iter).lHungry;
					pPetInfo->lAbility		= (*iter).lAbility;
					pPetInfo->bIsActive		= TRUE;				
					pPetInfo->iType			= iPetType;
					pPetInfo->iAge			= iPetAge;
					pPetInfo->lIndex		= iPetIndex;
					pPetInfo->strNameCard	= (*iter).strNameCard;
					
					// ?? ???? ????. [11/18/2010 rumist]
					pPetInfo->pen_pEntity	= NULL;
					
					CUIManager* pUIMgr = CUIManager::getSingleton();
					pUIMgr->GetPetInfo()->GetPetDesc();
					pUIMgr->GetPetTargetUI()->openUI();
				}
			}

			// ?????? ?????? ?????? ????, ???? ???????? ??????? ?????? ???????? ?????? ????...
			_pNetwork->RidePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0), NULL, iPetColoredType );
		}
		// ?????? ???????? ???????? ???? ????...
		else
		{
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
		}
		//		_pNetwork->MyCharacterInfo.bPetRide = bPetRide;		
	}
	// ?? ???????? ???? ????...
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lOwnerIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_iPetType = iPetType;
			pTarget->cha_iPetAge	= iPetAge;
			if( pTarget->cha_bPetRide != bPetRide )
			{
				// ???? ?????? ???????? ???? ????...
				if( iPetType != -1 && 
					iPetAge != -1 && 
					bPetRide )
				{	
					_pNetwork->RidePet( pTarget->GetEntity(), NULL, iPetColoredType );
				}
				// ?????? ???????? ???????? ???? ????...
				else
				{
					_pNetwork->LeavePet( pTarget->GetEntity() );
				}
				pTarget->cha_bPetRide = bPetRide;
			}
		}
	}	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetLearnMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetLearnMessage( CNetworkMessage *istr )
{
	LONG nSkillIndex;
	BYTE nSkillLevel;
	LONG nErrorcode;

	(*istr) >> nSkillIndex;
	(*istr) >> nSkillLevel;
	(*istr) >> nErrorcode;

	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if( nErrorcode == MSG_EX_PET_LEARN_ERROR_OK || nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO  )
	{	
		pUIManager->GetPetTrainingUI()->LearnSkill( pPetInfo->lIndex, nSkillIndex, nSkillLevel );
	}
	else if( nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO ) // ???? ???? ???? 
	{
		pUIManager->GetPetTrainingUI()->LearnSkill( pPetInfo->lIndex, nSkillIndex, nSkillLevel, TRUE );
	}
	else 
	{
		pUIManager->GetPetTrainingUI()->LearnSkillError( nErrorcode );
	}

}

// ----------------------------------------------------------------------------
// Name : ReceivePetSkillListMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetSkillListMessage( CNetworkMessage *istr )
{
	LONG nPetIndex, nCount;
	LONG nSkillIndex;
	BYTE nSkillLevel;
	int nSkillTime;

	(*istr) >> nPetIndex;
	(*istr) >> nCount;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetPetInfo()->ClearSkills( nPetIndex );
	
	for( int i = 0; i < nCount; i++ )
	{
		(*istr) >> nSkillIndex;
		(*istr) >> nSkillLevel;
		(*istr) >> nSkillTime;

		if (MY_PET_INFO()->lIndex < 0)
		{
			MY_INFO()->SetPetSkill(nPetIndex, nSkillIndex, nSkillLevel);
			pUIManager->GetPetInfo()->AddSkill(nPetIndex, nSkillIndex, nSkillLevel);

			CSkill& rSkill = _pNetwork->GetSkillData(nSkillIndex);
			if ( nSkillTime <= 0 )
			{
				rSkill.ResetStartTime();
				continue;
			}
			SLONG slRemainTime = ((SLONG)time(NULL) - _pNetwork->slServerTimeGap) - nSkillTime; 

			if ( slRemainTime > 0 )
				rSkill.SetStartTime(slRemainTime);

		}
		else
		{
			pUIManager->GetPetTrainingUI()->LearnSkill(nPetIndex, nSkillIndex, nSkillLevel, FALSE, false);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePetResetSkillMessage()
// Desc : ???? ???????? ???? ???? ???? ??????
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetResetSkillMessage( CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetPetInfo()->ClearSkills( MY_PET_INFO()->lIndex );
	
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(191,"????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	strMessage.PrintF( _S(2565,"???????? ???? ???????? ???? ??????????." ) );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : ReceivePetSellInfoMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetSellInfoMessage( CNetworkMessage *istr )
{
	SPetExchangInfo temp;

	(*istr) >> temp.lOwnerIndex;
	(*istr) >> temp.lPetIndex;
	(*istr) >> temp.sbPetTypeGrade;
	(*istr) >> temp.lLevel;
	(*istr) >> temp.llExp;
	(*istr) >> temp.llNeedExp;
	(*istr) >> temp.lHP;
	(*istr) >> temp.lMaxHP;
	(*istr) >> temp.lAbility;
	(*istr) >> temp.lHungry;
	(*istr) >> temp.lMaxHungry;
	(*istr) >> temp.lSympathy;
	(*istr) >> temp.lMaxSympathy;
#ifdef PET_SEAL_TIME
	(*istr) >> temp.lRemainRebirth;	
#endif

	(*istr) >> temp.strNameCard;

	CUIManager::getSingleton()->GetPetInfo()->AddPetExchangeInfo( temp );
	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetChangeMountMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetChangeMountMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetInfo()->PetExchangeMount( lResult );
}
//------------------------------------------------------------------------------
// CSessionState::ReceivePetLevelUpMessage
// Explain:  
// Date : 2005-10-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSessionState::ReceivePetLevelUpMessage( CNetworkMessage *istr )
{
	LONG	lPetIndex;
	SBYTE	sbTypeGrade;
	LONG	lLevel;

	(*istr) >> lPetIndex;
	(*istr) >> sbTypeGrade;
	(*istr) >> lLevel;

	// [070824: Su-won] PET_COLOR_CHANGE
	SBYTE	sbPetColor =0;
	(*istr) >> sbPetColor;

	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, lPetIndex);

	if (pObject != NULL)
	{
		CPetTarget* pTarget = static_cast< CPetTarget* >(pObject);

		if (pTarget->m_nIdxServer == MY_PET_INFO()->lIndex )
		{
			_pNetwork->ClientSystemMessage( _S(2253, "???? ?????? ??????????." ) );
		}

		// ???? ?????? ????????...
		if (pTarget->pet_iAge != iPetAge )
		{
			CEntity	*penEntity = pTarget->GetEntity();

			if (penEntity != NULL)
			{
				pTarget->m_nType	= iPetType;
				pTarget->pet_iAge	= iPetAge;

				// Date : 2005-11-08(???? 4:04:21), By Lee Ki-hwan
				// _s ?? ?????? ????????????? 
				CTString strPetName = pTarget->pet_OwnerName + _S( 2228, "??" ) + CTString(" ") + PetInfo().GetName(iPetType, iPetAge);	
				pTarget->m_strName	= strPetName;
				penEntity->SetSkaModel(PetInfo().GetFileName(iPetType, iPetAge));					
				//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PetIdleAnim( penEntity );
				PetInfo().SetPetDataToEntity(iPetType, iPetAge, penEntity, FALSE);
				penEntity->GetModelInstance()->RefreshTagManager();
				//penEntity->GetModelInstance()->StopAllAnimations(0.1f);

				if( sbPetColor > 0 )
				{
					CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, sbPetColor);
					penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
				}
			}
		}
	}

	CTString strEffect;
	strEffect = "pet_levelup";
	
	if( iPetAge >= 2 )
		strEffect = "pet_levelup_mount";
	
	// ?? ???? ???? ...?? 
	PetStartEffectGroup( strEffect, lPetIndex );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemMixMessage()
// Desc : ?????? ?????? ???? eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemMixMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetItemMixUI()->PetItemMixRep( lResult );	
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemChangeMessage()
// Desc : ?? ?????? ???? ?????????? ???? eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemChangeMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetTrainingUI()->PetChangeItemError( lResult );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetRebirthMessage()
// Desc : eons( ???? ???? )
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetRebirthMessage( CNetworkMessage *istr )
{
	LONG	lPetIndex;
	LONG	lResult;
	(*istr) >> lPetIndex;
	(*istr) >> lResult;

	CUIManager::getSingleton()->GetPetFree()->PetFreeError( lPetIndex, lResult );
}


// ----------------------------------------------------------------------------
// Name : ReceivePartyRecall()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePartyRecall( CNetworkMessage *istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	LONG subType;
	(*istr) >> subType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(subType)
	{

		case MSG_EX_PARTY_RECALL_NOTICE :
			pUIManager->CloseMessageBox(MSGCMD_NULL);
			MsgBoxInfo.SetMsgBoxInfo( _S(191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(2516, "?????????? ???? ???????? ??????????." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			break;
		case  MSG_EX_PARTY_RECALL_PROMPT :
			{
				CTString tv_str;
				(*istr) >> _pNetwork->m_tmp_idx;
				(*istr) >> _pNetwork->m_tmp_str;

				if( pUIManager->DoesMessageBoxExist(MSGCMD_EX_PARTY_RECALL_PROMPT) )
					pUIManager->CloseMessageBox(MSGCMD_EX_PARTY_RECALL_PROMPT); // ?????? ?????? ?????? ?????? ?????? ???????? ????
				
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_PARTY_RECALL_PROMPT );
				tv_str.PrintF(_S(2517,"%s???????????? ???????????? ???? ????????. ?????? ???????????????"),_pNetwork->m_tmp_str.str_String);
				MsgBoxInfo.AddString(tv_str);
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
	
		case MSG_EX_PARTY_RECALL_CANCEL :
			{
			   LONG nReq_char,nTgt_char;
			   CTString strReq_char,strTgt_char,tv_str;
			   (*istr) >> nReq_char;
			   (*istr) >> strReq_char;
			   (*istr) >> nTgt_char;
			   (*istr) >> strTgt_char;
			   if(nReq_char == _pNetwork->MyCharacterInfo.index)
			   {
			   		pUIManager->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2518, "%s?????????? ?????? ??????????????."), strTgt_char);
					MsgBoxInfo.AddString(tv_str);
					pUIManager->CreateMessageBox( MsgBoxInfo );
			   }
			   else {
				   pUIManager->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2519, "%s?????????? ?????? ??????????????."), strReq_char);
					MsgBoxInfo.AddString( tv_str );
					pUIManager->CreateMessageBox( MsgBoxInfo );				   
			   }
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_NOT_FOUND_PARTY_PLAYER :
			{
				_pNetwork->ClientSystemMessage(_S(4802, "???? ?????? ???? ???? ?????? ????????."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_INSTANT_DUNGEON :
			{
				_pNetwork->ClientSystemMessage(_S(4504, "?????????? ?????????? ?????? ?? ????????."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_THIS_CONTINENT :
			{
				_pNetwork->ClientSystemMessage(_S(4803, "???? ???????? ?????? ?? ????????."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_NOT_PARTY:
			{
				_pNetwork->ClientSystemMessage(_S(3408, "?????? ?????????? ????????."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_EXPEND:
			{
				_pNetwork->ClientSystemMessage(_S(4752, "???????????? ?????? ?? ????????."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_CONFIRM_FAIL:
			{// ???? ???? ?? ?????? ?? ???? ???? ???? ???? ???? ???? ??????
				pUIManager->CloseMessageBox(MSGCMD_NULL);
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(340, "?????? ??????????????." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				if(pUIManager->IsCSFlagOn(CSF_TELEPORT))
				{
					pUIManager->SetCSFlagOff( CSF_TELEPORT );
				}			
			}
			break;
		case MSG_EX_PARTY_RECALL_AREADY:
			{
				_pNetwork->ClientSystemMessage(_S(556, "???? ???????? ?????? ??????."), SYSMSG_ERROR);
			}
			break;

	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveElementalDeleteMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveElementalDeleteMessage( CNetworkMessage *istr )
{
	// MSG_EX_ELEMENTAL_DELETE,	// ?????? ????		: index(n)
	LONG lIndex;
	(*istr) >> lIndex;

	CUIManager* pUIManager = CUIManager::getSingleton();

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, lIndex);

	if (pObject != NULL)
	{
		CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

		// ?? ???????? ????...
		if( pTarget->slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
		{
			for (int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i)
			{
				CSummonUI* pSummonUI = (CSummonUI*)pUIManager->GetUI(i);

				if( pSummonUI->GetSummonEntity() && pSummonUI->GetSummonIndex() == lIndex )
				{
					pSummonUI->ResetSummon();
					break;
				}
			}			
		}

		SlaveInfo().StopIdleEffect( lIndex );

		//???? ???????? ???? ???????? ????...
		pUIManager->StopTargetEffect( lIndex );

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pTarget->GetEntity());

		ACTORMGR()->RemoveObject(eOBJ_SLAVE, lIndex);
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveEvocationStart()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveEvocationStart( CNetworkMessage *istr )
{
	// charindex(n) type(c)
	LONG	lCharIndex;
	INDEX	iType;

	(*istr) >> lCharIndex;
	(*istr) >> iType;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->EvocationStart( lCharIndex, iType);	
}

// ----------------------------------------------------------------------------
// Name : ReceiveEvocationStop()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveEvocationStop( CNetworkMessage *istr )
{
	// charindex(n)
	LONG	lCharIndex;	
	(*istr) >> lCharIndex;	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->EvocationStop( lCharIndex );	
}


//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
//  Name 	: 
//  Desc 	: 
// ----------------------------------------------------------------------------
void AffinityErrChk( UBYTE errType )
{
	CUIMsgBox_Info MsgBoxInfo;
	CTString		MsgBoxMsg;
	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( errType )
	{
		case MSG_EX_AFFINITY_ERROR_CONNECT_SUCCESS:
		{
			MsgBoxMsg.PrintF( _S( 4671, "?????????? ?????? ??????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_NOMONEY:			// ????????
		{
			MsgBoxMsg.PrintF( _S(306, "?????? ??????????.") );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_ALREADY:			// ???????? ????????
		{
			MsgBoxMsg.PrintF( _S( 4672, "???? ?????? ?????? ??????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MAX:		// ?????? ?????? ?????? ????
		{
			MsgBoxMsg.PrintF( _S( 4673, "???????? ???? ????????. ?????? ???????? ???? ?? ????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MISSMATCH:	// ???? ?????? ??????
		{
			MsgBoxMsg.PrintF( _S( 4674, "?????? ?? ???? ????????????. ???????? ???? ???????? ????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_USESHOP_POINT:			// ???????? ?????? ????
		{
			MsgBoxMsg.PrintF( _S( 4675, "???? ?????????? ?? ?????? ????????. ???? %s ?? ?????? ?????? ?????? ?? ????????." ), 
			_pNetwork->GetAffinityData()->GetAffinityName( pUIManager->GetAffinity()->GetNPCIndex() ) );	// [2010/12/07 : Sora] ?????? ???? 2??
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFTINFO_NOITEM:			// ???????? ???????? ????
		{
			MsgBoxMsg.PrintF( _S( 4676, "???? ?????? ??????????. ?????? ???????? ???????? ????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFTINFO_NOPOINT:			// ?????????????? ???? ??????
		{
			MsgBoxMsg.PrintF( _S( 4677, "???????? ??????????." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFT_NOSPACE:			// ?????? ?????? ???? ?????? ????.
		{
			MsgBoxMsg = _S(2850, "?????????? ???? ????????.");
		}
			break;
		case MSG_EX_AFFINITY_ERROR_NOT_CONNECTED:
		{
			MsgBoxMsg = _S( 4678, "???? ?????? ???? ??????????. ???? ?????? ??????????." );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_PCLEVEL:
		{
			MsgBoxMsg = _S( 965, "?????? ??????????." );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM:
		{
			MsgBoxMsg = _S( 1330 ,"???????? ??????????." );
		}
			break;
	}
	MsgBoxInfo.AddString( MsgBoxMsg );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
//  Name 	: ReceiveAffinityMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveAffinityMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	UBYTE	errCode;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( ubType )
	{
		case MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REP:
		{
			CPrintF("Affinity Contribute success!!\n" );
		}
			break;
		case MSG_EX_AFFINITY_SHOPUSE_REP:
		{
			SLONG		npc_idx = 0;
			(*istr) >> npc_idx;
			pUIManager->GetAffinity()->OpenShop( npc_idx );
		}
			break;
		case MSG_EX_AFFINITY_GIFT_REP:
		{
			// [100324: selo] ???? ???? npc ?? ???? ???????? ???? ????.
			SLONG npc_idx = 0;
			(*istr) >> npc_idx;

			_pNetwork->RemoveAffinityRewardNPC(npc_idx);

			if( npc_idx > 0)
			{
				Notice* pNotice = GAMEDATAMGR()->GetNotice();

				if (pNotice != NULL)
					pNotice->DelFromNoticeList(npc_idx, Notice::NOTICE_AFFINITY);
			}
		}
			break;
		case MSG_EX_AFFINITY_GIFTINFO_REP:
		{
			SLONG		gift_point = 0;
			SLONG		gift_item_index = 0;
			SLONG		gift_item_count = 0;
			CTString	MsgTemp;
			CUIMsgBox_Info	Msg_Info;

			(*istr) >> gift_point;
			(*istr) >> gift_item_index;
			(*istr) >> gift_item_count;

			CTString strTemp;
			strTemp.PrintF("%d", gift_point);
			pUIManager->InsertCommaToString(strTemp);

			MsgTemp.PrintF( _S( 5302, "%s ?????? ???? ???? ?????? ???????? ???????? %s???? ???? ???? ?????? ????????????. \n%s  %d???? ???????????????" ),
				_pNetwork->GetAffinityData()->GetAffinityName( pUIManager->GetAffinity()->GetNPCIndex() ), strTemp, _pNetwork->GetItemData( gift_item_index )->GetName(), gift_item_count );
			
			Msg_Info.SetMsgBoxInfo( _S(4680, "???? ????"), UMBS_OKCANCEL, UI_NPC_AFFINITY, MSGCMD_AFFINITY_TAKEPRESENT );
			Msg_Info.AddString( MsgTemp );
			pUIManager->CreateMessageBox( Msg_Info );
		}
			break;
		case MSG_EX_AFFINITY_UPDATE_POINT:
		{
			SLONG	affinity_index = 0;
			SLONG	affinity_point = 0;
			SLONG	affinity_point_bonus = 0;
			
			(*istr) >> affinity_index;
			(*istr) >> affinity_point; // total (?????????? ?????????? ????????)
			(*istr) >> affinity_point_bonus; // ?????????? ?????? ??

			CNetworkLibrary::_AffinityInfo::mapAffIter iter;

			iter = _pNetwork->AffinityInfo.mapAffinityList.find( affinity_index );
			if( iter != _pNetwork->AffinityInfo.mapAffinityList.end() )
			{
				CTString MsgTemp;
				int ndelta = affinity_point - (*iter).second.current; // ???? ?????????? ?????? ????
				
				CTString strAp;
				
				// [2010/12/07 : Sora] ?????? ???? 2??
				if (affinity_point_bonus > 0)
				{
					CTString strBonus;

					strAp.PrintF("%d", ndelta - affinity_point_bonus);
					pUIManager->InsertCommaToString(strAp);

					strBonus.PrintF("%d", affinity_point_bonus);
					pUIManager->InsertCommaToString(strBonus);

					MsgTemp.PrintF( _S(6445, "%s(+%s)???? %s???????? ??????????????."), strAp, strBonus,
						_pNetwork->GetAffinityData()->GetAffinityNameByIndex(affinity_index) );
				}
				else
				{
					strAp.PrintF("%d", ndelta);
					pUIManager->InsertCommaToString(strAp);

					MsgTemp.PrintF( _S(4681, "%s???? %s???????? ??????????????."), strAp,
						_pNetwork->GetAffinityData()->GetAffinityNameByIndex(affinity_index) );
				}				

				((*iter).second).current = affinity_point;
				pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );

				//2012/11/09 jeil ?????? ?????? ?????????? ???? ???? ?????? ???? ???? 
				if(_pUIBuff->IsBuff(9189)){	//???? ???? ???? 
					MsgTemp.PrintF(_S(5815, "(?????? ?????? ????)"));	//???????? ?????? ????
					pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );	//?????? ???????? ????
				}
				//2012/11/09 jeil ???? ?????? ?????? ?????? ???????? ?????? ???? 
				//?????? ?????? ?????? ?????????? ???? ???? -> ????????
				//?????? ?????? ???? ?????? ???? ->???????? 
//				MsgTemp.PrintF("(?????? ?????? ????)");	//???????? ?????? ????
//				pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );	//?????? ???????? ????

			}
			else
				CPrintF("Affinity Index Error !!\n" );
		}
			break;
		case MSG_EX_AFFINITY_LISTINFO:
		{
			SLONG	affinity_cnt = 0;
			SLONG	affinity_index = 0;
			SLONG	affinity_point = 0;
			SLONG	affinity_max = 0;
			CNetworkLibrary::_AffinityInfo::_AffinityPoint AfPoint;
			(*istr) >> affinity_cnt;

			_pNetwork->AffinityInfo.count = affinity_cnt;

			for( int i = 0; i < affinity_cnt; ++i )
			{
				(*istr) >> affinity_index;
				(*istr) >> affinity_point;
				(*istr) >> affinity_max;

				AfPoint.current = affinity_point;
				AfPoint.max		= affinity_max;

				_pNetwork->AffinityInfo.mapAffinityList.insert( std::make_pair( affinity_index, AfPoint ) );
			}
		}
			break;
		case MSG_EX_AFFINITY_ADDINFO:
		{
			// [6/8/2009 rumist] if server was accept that connect to npc forces, called this message.
			SLONG affinity_index;
			SLONG affinity_max = 0;
			CNetworkLibrary::_AffinityInfo::_AffinityPoint AfPoint;
			AfPoint.current = 0;
			(*istr) >> affinity_index;

			(*istr) >> affinity_max;
			AfPoint.max = affinity_max;

			// ?????? ????2 ?????? ???? ?????? ?????? ???????? [2/14/2013 Ranma]
			CNetworkLibrary::_AffinityInfo::mapAffIter iter = _pNetwork->AffinityInfo.mapAffinityList.find( affinity_index );
		
			if ( iter == _pNetwork->AffinityInfo.mapAffinityList.end() )
			{
				_pNetwork->AffinityInfo.mapAffinityList.insert( std::make_pair( affinity_index, AfPoint ) );
			}
			else
			{
				((*iter).second).max = affinity_max;
			}
			
			// [2013/02/05] sykim70 ?????? ???? UI ????
			_pNetwork->AffinityInfo.count = _pNetwork->AffinityInfo.mapAffinityList.size();
		}
			break;
		case MSG_EX_AFFINITY_ERROR:
		{
			(*istr) >> errCode;
			AffinityErrChk( errCode );
		}	
			break;
			
		// [100322: selo] ?????? ???? ????		
		case MSG_EX_AFFINITY_REWARD_NOTICE:
		{					 
			INDEX noticecount = 0;
			INDEX npcidx = 0;

			(*istr) >> noticecount;

			if( noticecount > 0 )
			{
				Notice* pNotice = GAMEDATAMGR()->GetNotice();

				for( int i = 0; i < noticecount; i++ )
				{
					(*istr) >> npcidx;
					_pNetwork->AddAffinityRewardNPC(npcidx);

					if (pNotice != NULL && npcidx > 0)
						pNotice->AddToNoticeList(npcidx, Notice::NOTICE_AFFINITY);
				}
			}
		}
		break;

		// ?????? ????2 ???? ???? ???? [2/13/2013 Ranma]		
		case MSG_EX_AFFINITY_INFOTAB_REP:
			{					 
				pUIManager->GetAffinityInfo()->RecvAffinity_RewardInfo(istr);
			}
		break;
	}
}

// ----------------------------------------------------------------------------
//  Name 	: ReceiveExLoadingEndMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExLoadingEndMessage(CNetworkMessage* istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch( ubType )
	{
		case MSG_EX_LODING_END_NPC_PORTAL_USE:
		{
			CUIManager::getSingleton()->GetNpcScroll()->UseNPCScroll();
		}
			break;
	}
}

// ----------------------------------------------------------------------------
// [100208: selo] 
//  Name 	: ReceiveTakeAgainQuestItem()
//  Desc 	: ?????? ?????? ???? ???? ( ?????? ???? ???? )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveTakeAgainQuestItem(CNetworkMessage* istr)
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	SLONG	slType;
	(*istr) >> slType;

	switch( slType )
	{
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_SUCCESS:
		{
			strMessage.PrintF( _S( 4820, "?????????? ???????? ??????????????.") );
		}
		break;
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_ALREADY_EXIST:
		{
			strMessage.PrintF( _S( 4821, "?????????? ???????? ??????????.") );
		}
		break;
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_FULL_INVENTORY:
		{
			strMessage.PrintF( _S( 3796, "?????????? ???????? ???????? ???? ?? ????????.") );
		}
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxExist(MSGCMD_QUEST_RESTORE_ITEM_RESULT))
		pUIManager->CloseMessageBox(MSGCMD_QUEST_RESTORE_ITEM_RESULT);

	MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_QUEST, MSGCMD_QUEST_RESTORE_ITEM_RESULT );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );	
}

// ----------------------------------------------------------------------------
//  [4/15/2010 kiny8216] ?????? ??????2 ??????
//  Name 	: RecieveCostume2Message()
//  Desc 	: ?????? ??????2 ??????
// ----------------------------------------------------------------------------
void CSessionState::RecieveCostume2Message(CNetworkMessage* istr)
{
	UCHAR ucType;
	CTString	strMessage;

	(*istr) >> ucType;

	switch( ucType )
	{
	case 0:
		strMessage.PrintF( _S( 4868, "?????????? ?????? ?????? ?????? ?????? ????????.") );
		break;
	case 1:
	case 3:
		strMessage.PrintF( _S( 4869, "?????? ?????? ?????????? ?????? ?? ????????.") );
		break;
	case 2:
		strMessage.PrintF( _S( 4870, "?????????? ?????? ?????? ?????? ?????? ????????.") );
		break;
	case 4:
		strMessage.PrintF( _S( 4871, "?????? ?????????? ?????? ?????????? ?????? ?????? ?? ????????.") );
		break;
	case 5:
		strMessage.PrintF( _S( 4872, "?????????? ?????? ?????????? ?????? ?????? ?????? ?? ????????.") );
		break;
	}

	if(strMessage.Length()>0 )
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage , SYSMSG_ERROR );	

	// ???? ???? ???? ???? ????
	CUIManager::getSingleton()->SetCSFlagOffElapsed( CSF_ITEMWEARING );
}

// SOCKET_SYSTEM_S2 ???? ?????? ???? ?????? ???? ???? [4/3/2013 Ranma]
void CSessionState::RecieveSocketSystemMessage(CNetworkMessage* istr )
{
	UBYTE	slType;
	(*istr) >> slType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( slType )
	{
		case MSG_EX_SOCKET_MAKE_REP:
			{
				SLONG idx;
				SBYTE socketCount;
				(*istr) >> idx;
				(*istr) >> socketCount;

				// ?????? ui ?????????? ??????.
				// ?????? ?????? ?????? ?????????? idx?? ??????.
				pUIManager->GetSocketCreate()->ShowResultSuccess(idx);
			}
			break;
		case MSG_EX_SOCKET_COMBINE_JEWEL_REP:
			{
				SLONG idx;
				(*istr) >> idx;
				pUIManager->GetSocketCombine()->ShowResultSuccess(idx);
			}
			break;

		case MSG_EX_SOCKET_UPGRADE_JEWEL_REP:
		case MSG_EX_SOCKET_UPGRADE_JEWEL_CHAOS_REP:
			{
				SLONG jewelIdx;
				(*istr) >> jewelIdx;
				pUIManager->GetSocketJewelCompos()->SetResultJewelUI(jewelIdx);
			}
			break;

		case MSG_EX_SOCKET_INFO_REP:
			{
			}
			break;

		case MSG_EX_SOCKET_CLEAN_JEWEL_REP:
			{
				SLONG idx;
				(*istr) >> idx;
				CUIMsgBox_Info MsgInfo;
				MsgInfo.SetMsgBoxInfo( _S( 191, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgInfo.AddString( _S( 5000, "???? ?????? ?????? ?????????? ??????????????.") );
				pUIManager->CreateMessageBox( MsgInfo );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
			}
			break;

		case MSG_EX_SOCKET_ERROR:
			{
				UBYTE errcode;
				(*istr) >> errcode;
				CheckSocketSystemError( errcode );
			}
			break;
	}
}	

// ???? ?????? ???????? ?????? ?????? ?????????? ???? ????. [5/4/2010 rumist]	
void CheckSocketSystemError(UBYTE errcode)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString tStr;

	switch(errcode)
	{
		case MSG_EX_SOCKET_ERROR_ITEM_MISSMATCH:    // ?????? ???????? ????????.
				tStr = _S( 4981, "?????? ???????? ????????. ???? ?? ???? ?????? ?????? ????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_FAILED_MAKE:		// ???? ???? ????
				tStr = _S( 5001, "???? ???? ????." );
				// ?????????? ???????? ?????????? ?????? update?? ????.
				pUIManager->GetSocketCreate()->ShowResultFail();
				break;
		case MSG_EX_SOCKET_ERROR_NOMONEY:			// ?? ????	
				tStr = _S( 306, "?????? ??????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_MAX_SOCKET:		// ???? ?????? ???? ???? ????
				tStr = _S( 5002, "???? ?????? ??????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_INVEN_NOSPACE:		// ???????? ????
				tStr = _S( 265, "???????? ?????? ??????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOWEARITEM:		// ???? ?????????? ?? ???? ??????????.
				tStr = _S( 520, "?????? ???????? ??????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOSPACE:			// ?????? ???? ???????????? ?????? ?????? ?? ????????
				tStr = _S( 4998, "?????? ???????? ???? ????????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOJEWEL:			// ?????? ???????? ???????? ?????? ?????? ?? ????????
				tStr = _S( 5003, "???? ?????? ???????? ???? ????????????." );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;

		case MSG_EX_SOCKET_ERROR_FAILED_COMBINE: // ???? ???? ????
				pUIManager->GetSocketCombine()->ShowResultFail();
				break;
	}
	
	if(tStr.Length()>0 )
		pUIManager->GetChattingUI()->AddSysMessage( tStr , SYSMSG_ERROR );	
}


// ----------------------------------------------------------------------------
// [9/29/2010 kiny8216] ???? ??????
//  Name 	: RecieveCostume2Message()
//  Desc 	: ???? ?????? ??????
// ----------------------------------------------------------------------------
void CSessionState::RecieveLordCostumeMessage( CNetworkMessage* istr )
{
	UBYTE ubType;
	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;

	(*istr) >> ubType;
	switch( ubType )
	{
	case MSG_EX_CASTLLAN_ERROR_GIVE_ITEM_ERROR:
		{	// ?????? ???????? ????
			strMessage = _S(2502, "???? ???? ???????? ????????.");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_GIVE_ITEM_SUC:
		{	// ???? ????
			strMessage = _S(1840, "???????? ?????????? ???? ??????????");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN:
		{	// ?????? ????
			strMessage = _S(1722, "?????? ?????? ?? ???? ?????? ????????");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN_GUILD:
		{
			// ?????????? ????
			strMessage = _S( 3743, "???? ?????? ????????." );
		}break;
	}

	if ( strMessage.Length() > 0)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
//  [2010/08/25 : Sora] ADD_SUBJOB
//  Name 	: RecieveSubJobMessage()
//  Desc 	: ???????? ?????? ????
// ----------------------------------------------------------------------------
void CSessionState::RecieveSubJobMessage(CNetworkMessage* istr)
{
	SLONG	slType;
	(*istr) >> slType;

	switch( slType )
	{
		case MSG_EX_SUBJOB_ERROR_SUCESS:
			{
				SLONG subJobCode;
				(*istr) >> subJobCode;
				CTString strMessage;
				strMessage.PrintF( _S( 5083, "[%s]???? ???? ??????????." ), CUIManager::getSingleton()->GetSubJobName(subJobCode) );
				_pNetwork->ClientSystemMessage(strMessage, SYSMSG_NORMAL);
				_pNetwork->MyCharacterInfo.slSubJob = subJobCode;
			}
			break;
		case MSG_EX_SUBJOB_ERROR_FAIL:
			{
				SLONG errcode;
				(*istr) >> errcode;
				switch( errcode )
				{
					case 1:
						{
							_pNetwork->ClientSystemMessage(_S( 965, "?????? ??????????." ), SYSMSG_ERROR);
						}
						break;
					case 2:
						{
							_pNetwork->ClientSystemMessage(_S( 1217, "???????? ??????????." ), SYSMSG_ERROR);
						}
						break;
					case 3:
						{
							_pNetwork->ClientSystemMessage(_S( 966, "SP?? ??????????." ), SYSMSG_ERROR);
						}
						break;
					case 4:
						{
							_pNetwork->ClientSystemMessage(_S( 967, "?????? ??????????." ), SYSMSG_ERROR);
						}
						break;
				}
			}		
			break;
		case MSG_EX_SUBJOB_ERROR_NOT_TRADER:
			{
				CUIMsgBox_Info MsgInfo;
				MsgInfo.SetMsgBoxInfo( _S( 1748, "????" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgInfo.AddString( _S( 5050, "???????? ???????? ???? ????????.") );
				CUIManager::getSingleton()->CreateMessageBox( MsgInfo );			
			}
			break;
	}
}

// ----------------------------------------------------------------------------
// [2010/10/20 : Sora] ?????? ???? ????
//  Name 	: RecieveMonsterMercenaryCardMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::RecieveMonsterMercenaryCardMessage(CNetworkMessage* istr)
{
	UBYTE ubType;
	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox(MSGCMD_MONSTER_MERCENARY_CARD_ERROR);

	(*istr) >> ubType;

	switch( ubType )
	{
		case MSG_EX_MERCENARY_SUMMON:		// ????/ ???? ????
			{
				SLONG uniqItemIndex;
				SBYTE toggle;
				//?????? ????
				(*istr) >> uniqItemIndex;
				(*istr) >> toggle;

				if( toggle > 0 )
				{
					CUIIcon* pIcon = pUIManager->GetInventory()->GetItemIcon(uniqItemIndex);

					if (pIcon != NULL)
						pUIManager->GetMonsterMercenary()->SetUseItemData(pIcon->getIndex());
				}
				pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary( toggle );
			}
			break;
		case MSG_EX_MERCENARY_LIMIT_TIME:	// ???? ???? ????
			{
				SLONG time;
				SLONG tab, nInvenIdx;

				(*istr) >> tab;
				(*istr) >> nInvenIdx;
				(*istr) >> time;

				CItems	&rItems = _pNetwork->MySlotItem[tab][nInvenIdx];

				strMessage.PrintF(_S( 5158, "[%s]%s???? ???? ???? ???????? %d?? ??????????."), 
									pUIManager->GetMonsterMercenary()->GetMonsterGrade(rItems.Item_Used2),
									CMobData::getData(rItems.Item_Plus)->GetName(), time);

				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}
			break;
		case MSG_EX_MERCENARY_ERROR:
			{
				UBYTE errcode;
				
				(*istr) >> errcode;

				switch( errcode )
				{
					case MSG_EX_MERCENARY_ERROR_USE_FAIL:		// ???? ????
						{
							strMessage = _S( 5159, "?????? ???? ?? ?? ????????. ???? ?? ???? ????????????.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_SUMMON_ALREADY:	// ???? ???????? ???????? ???? ?????? ?? ????
						{
							strMessage = _S( 5160, "???? ?????? ???????? ????????.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_DONT_SUMMON:	// ???? ????
						{
							strMessage = strMessage = _S( 5159, "?????? ???? ?? ?? ????????. ???? ?? ???? ????????????.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL:	// ?????? ???? ????(?????? ?????????? ?????? ????????)
						{
							pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary( -1 );

							strMessage = _S( 5161, "???? ?????? ??????????????.");

							pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EX_MERCENARY_ERROR_AREA:	// ?? ?????????? ?????? ???? ?? ?? ????????. 
						{
							// ?????? ????
							strMessage = _S( 5162, "???? ?????????? ?????? ???? ?? ?? ????????.");
						}
						break;
					default:
						{
							strMessage = _S( 5159, "?????? ???? ?? ?? ????????. ???? ?? ???? ????????????.");
						}
						break;
				}
			}
			break;
	}

	if ( strMessage.Length() > 0)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_MONSTER_MERCENARY_CARD_ERROR);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void CheckFaceOffError(UBYTE ubError)
{
	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch( ubError )
	{
	case MSG_EX_FACEOFF_ERROR_FAIL:
		{
			strMessage.PrintF( _S( 5186, "?????????? ?????? ?????? ??????????????. ???? ?? ???? ?????? ??????."));
		}
		break;
	case MSG_EX_FACEOFF_ERROR_PLZ_CHOISE:
		{
			strMessage.PrintF( _S( 5187, "???????? ?????? ???? ?????? ??????????."));
		}
		break;
	default:
		{
			strMessage.PrintF( _S( 5188, "???? ?????? ??????????????."));
		}
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetInitJob()->CloseInitJob();

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}
void CSessionState::RecieveFaceOffMessage( CNetworkMessage* istr )
{
	UBYTE ubType;
	INDEX iCharIndex;
	UBYTE ubFaceStyle, ubHairStyle;
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;

	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( ubType )
	{
	case MSG_EX_FACEOFF_ERROR_SUC:
		{
			(*istr) >> ubHairStyle;
			(*istr) >> ubFaceStyle;
			(*istr) >> iCharIndex;

			if ( iCharIndex == _pNetwork->MyCharacterInfo.index )
			{	// ?? ???????? face, hair change?? ?????? ????,
				_pNetwork->MyCharacterInfo.faceStyle = ubFaceStyle;
				_pNetwork->MyCharacterInfo.hairStyle = ubHairStyle;
				pUIManager->GetInitJob()->CloseInitJob();
				strMessage.PrintF( _S( 5185, "???? ?????? ????????????."));
			}
			else
			{	// ???? ???????? face, hair change?? ?????? ????,
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

				if (pObject != NULL)
				{
					if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsHelmet(iCharIndex) == TRUE)
						return;

					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					CModelInstance* pMI = pTarget->GetEntity()->en_pmiModelInstance;

					INDEX iJob = pTarget->GetType();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetAppearanceData( pTarget->GetEntity()->en_ulID, ubHairStyle, ubFaceStyle);
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh( pMI, iJob, ubHairStyle - 1);
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh( pMI, iJob, ubFaceStyle - 1);
					pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
					pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			PCStartEffectGroup("squid", iCharIndex);
		}
		break;
	default :
		{
			CheckFaceOffError( ubType );
		}
		break;
	}
	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void CSessionState::RecieveLuckyDrawBoxMessage( CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	SLONG slErrorType;

	(*istr) >> slErrorType;

	switch( slErrorType )
	{
		case MSG_LUCKYDRAWBOX_ERROR_SUCESS:
			return;
		case MSG_LUCKYDRAWBOX_ERROR_NEED:
			{
				strMessage.PrintF( _S( 5311, "?????? ???? ???????? ???????? ?????????? ???? ?? ???? ???????? ????????."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_INVEN:
			{
				strMessage.PrintF( _S(4237, "?????????? ?????? ???? ??????. ?????????? ???? ?? ???? ???????????? ????????."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_USE:
			{
				strMessage.PrintF( _S(5312, "???????? ?????? ?? ????????."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_OVER_WEIGHT:
			{
				strMessage.PrintF( _S(4237, "?????????? ?????? ???? ??????. ?????????? ???? ?? ???? ???????????? ????????."));
			}
			break;
		default:
			{
				strMessage.PrintF( _S(5312, "???????? ?????? ?? ????????."));
			}
	}

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}


//added by sam 11/01/04 ?????? ?????? ???? ?? ???????? ???????? ????????. 
void CSessionState::RecieveMsgBoxShow ( CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	UCHAR chrErrorType;
	(*istr) >> chrErrorType;

	INDEX nTitle = 191;
	switch ( chrErrorType )
	{
		case MSG_EX_MSGBOX_INVEN_FULL:				// ???????? ??????
			{
				strMessage.PrintF( _S( 265, "???????? ?????? ??????????." ));			
				
				break;			
			}
		case MSG_EX_MSGBOX_CHANGE_START_POINT:
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(nTitle,"????"), UMBS_YESNO, UI_NONE, MSGCMD_CHANGE_START_POINT, 200);
				MsgBoxInfo.AddString( _S(5376, "?????? ???????? ???? ???? ??????????.") );
				MsgBoxInfo.AddString( _S(5377, "?????? ???? ?????? ???? ?? ??????.") );
				MsgBoxInfo.AddString( _S(5378, "?????????? ???? ??????????, ?????? ???? ?????? ????????.") );
				MsgBoxInfo.AddString( _S(5379, "?????? ???? ?????? ???????????? ?????? ???????? ???? ?????? ??????????.") );
				MsgBoxInfo.AddString( _S(5380, "(??, ?????? ???? ?? ???????????? ???? ?? ?? ????????.)") );
				MsgBoxInfo.AddString( _S(5381, "???????????? ???? ?????????????") );
				CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
				return;
			}
	}	

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(nTitle,"????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

void	CSessionState::RecieveUsedPartyItemMessage(CNetworkMessage* istr )
{
	INDEX		iItemIdx = -1;
	CTString	strNickName;

	(*istr) >> iItemIdx;
	(*istr) >> strNickName;

	CUIManager::getSingleton()->GetNotice()->ShowUsedPartyItemNotice( iItemIdx, strNickName );
}


void	CSessionState::RecieveRoyalrumbleMessage(CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	UBYTE msgIdx				= 0;
	(*istr) >> msgIdx;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( msgIdx )
	{
		case MSG_EX_ROYAL_RUMBLE_NOTICE:						// ????
			{
				UBYTE uResult = 0;
				(*istr) >> uResult;
				pUIManager->GetNotice()->ShowRoyalRumbleNotice( uResult );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_REG_MENU:						// ???????? ??????.
			{
				UBYTE uType = 0;
				(*istr) >> uType;
				pUIManager->GetRadar()->SetRoyalRumbleStatus( uType );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_REP:					// ???? ???? ????.
			{
				MsgBoxInfo.SetMsgBoxInfo( _s(""), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5414, "???? ?????? ?????????? ??????????????.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5415, "?????? ???????? ?????????? ???????? ?? ????????.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5416, "?????????? ????????????.") );
				}
				else if( uResult == 3 )
				{
					MsgBoxInfo.AddString( _S(5417, "???? ??????????????.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_UNREGIST_REP:			// ???? ????.
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5405, "???????? ???? ????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5418, "?????????? ??????????????.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5419, "?????? ?? ???? ?????? ????????.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5420, "?????? ?????? ????????.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_REG_RESULT:				// ???? ???? ????.
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5407, "???????? ????????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _s("?????? ?? ????") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5421, "????????????. ?????????? ??????????????. ?????? ???????? ?????? ?????? ??????.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5422, "?????????? ?????????? ???????? ????????????. ???? ?????????? ?????? ??????.") );
				}
				else if( uResult == 3 )
				{
					MsgBoxInfo.AddString( _S(5423, "??????????. ?????? ??????????????.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_GO_ZONE:						// ????????
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5424, "?????? ????"), UMBS_OK|UMBS_USE_TIMER, UI_RADAR, MSGCMD_ROYALRUMBLE_GO_ZONE);
								
				MsgBoxInfo.AddString( _S(5425,	"???????? ???????? ??????????."
												"?????????? ???? ?????? ????????."
												"(?????????? ?????? ???????? ??????.)" ) );
// 				MsgBoxInfo.AddString( _s("?????????? ???? ?????? ????????.") );
// 				MsgBoxInfo.AddString( _s("(?????????? ?????? ???????? ??????.)") );
				MsgBoxInfo.SetMsgBoxTimer( 30, FALSE );			// 30 seconds.
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_ENTER_NEXT_PLAYER:				// ???? ???? ???? ???? ????
			{
				INDEX iLeftTime = 0;
				(*istr) >> iLeftTime;
				pUIManager->GetNotice()->ShowRoyalRumbleNextPlayerTime( iLeftTime );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_HOLD:							// ???? ???? 
			{
				;
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_WINNER:						// ?????? ???? ????
			{
				INDEX iLevelType = 0;
				INDEX iCharIdx = 0;
				CTString strNickName;
				(*istr) >> iLevelType;
				(*istr) >> iCharIdx;
				(*istr) >> strNickName;
				pUIManager->GetNotice()->ShowRoyalRumbleWinner( iLevelType, strNickName );
				if( _pNetwork->MyCharacterInfo.index == iCharIdx )
				{
					pUIManager->GetRadar()->ShowRoyalRumbleTrophy();
				}
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_REWARD_REP:					// ???? ????
			{
				UBYTE uResult = 0;
				(*istr) >> uResult;
				MsgBoxInfo.SetMsgBoxInfo( _s(""), UMBS_OK, UI_NONE, MSGCMD_NULL);
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5427, "???? ???????? ????????.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(1755, "???? ?????? ??????????.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_POINT_REP:						// ?????????? ???? ????.
			{
				INDEX iWarPoint = 0, iWarAccPoint = 0;
				(*istr) >> iWarPoint;
				(*istr) >> iWarAccPoint;
				MsgBoxInfo.SetMsgBoxInfo( _S(5412, "???? ?????? ????"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				CTString strTemp;
				MsgBoxInfo.AddString( _S(5428, "???? ???? ??????"), 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				strTemp.PrintF( "%d", iWarPoint );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				MsgBoxInfo.AddString( _s(" ") );
				MsgBoxInfo.AddString( _S(5429, "???? ???? ??????"), 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				strTemp.PrintF( "%d", iWarAccPoint );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_TIME_REP:
			{
				INDEX iTime = -1;
				(*istr) >> iTime;
				MsgBoxInfo.SetMsgBoxInfo( _S(5430, "???????? ???? ????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString strTemp;
				if( iTime < 0 )
				{
					strTemp.PrintF( _S(5431, "???? ?????? ????????????.") );
				}
				else
				{
					strTemp.PrintF( _S(5432, "???? ?????? %d?? 00???? ??????????."), iTime );
				}
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_LEFT_COUNT:
			{
				INDEX iLeftCount = 0;
				(*istr) >> iLeftCount;
				pUIManager->GetRoyalRumbleIcon()->SetLeftCount( iLeftCount );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_TIME_NOTICE:
			{
				UBYTE uType = 0;
				INDEX iLeftTime = -1;
				(*istr) >> uType;
				(*istr) >> iLeftTime;

				pUIManager->GetRadar()->SetRoyalRumbleSandglass(uType, iLeftTime );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_NOTFOUND_ITEM:
			{	
				MsgBoxInfo.SetMsgBoxInfo( _S( 5576, "???????? ??????"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString strTemp;
				strTemp.PrintF( _S( 5577, "???? ???????? ?????? ???????? ???????? ????????.") );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_KILL_PLAYER:
			{
				CTString strAttacker;
				CTString strVictim;
				CTString strTemp;
				(*istr) >> strAttacker >> strVictim;
				// ADD-Contents : ITS#3620 ?????? ?????? ?????? ?????? ????.  [8/22/2011 rumist]
				if( strAttacker == strVictim )
					strTemp.PrintF( _S( 5580, "%???? ??????????????."), strAttacker );
				else
					strTemp.PrintF( _S( 5578, "%s???? %s???? ??????????????."), strAttacker, strVictim );
				pUIManager->GetChattingUI()->AddSysMessage( strTemp, SYSMSG_ERROR );
			}
			break;
	}
}

void CSessionState::RecieveMasterStoneMessage( CNetworkMessage* istr )
{
	ResponseClient::doItemUseMasterStoneUSA* pPacket = reinterpret_cast<ResponseClient::doItemUseMasterStoneUSA*>(istr->GetBuffer());

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetMixNew()->MixNewMasterStoneRep(pPacket->error, pPacket->success_type);
}

void CSessionState::RecieveRankingListExMessage( CNetworkMessage* istr )
{
	CUIManager::getSingleton()->GetRankingViewEx()->RecvRankingList(*istr);
}