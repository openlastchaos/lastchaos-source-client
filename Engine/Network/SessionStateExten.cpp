#include "stdh.h"

#include <Engine/Build.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/Diff.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/ProgressHook.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Base/Shell.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Math/Float.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Network/Compression.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Base/Console.h>
#include <Engine/Light/LightSource.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Network/LevelChange.h>
#include <Engine/World/WorldCollision.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Network/MessageDefine.h>

#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/CRC.h>
#include <Engine/Base/CRCTable.h>
//0105
#include <Engine//Network/tcpipconnection.h>
#include <Engine//Network/TxtQueue.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIChatting.h>
#include <Engine/World/World.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIGWMix.h>

#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIPetTarget.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Petinfo.h>

//051226 wooss
#include <Engine/SlaveInfo.h>
#include <Engine/LocalDefine.h>
#include <Engine/Entities/MobData.h> // WSS_DRATAN_SEIGEWARFARE 2007/08/23

// ----------------------------------------------------------------------------
// Name : CheckDratanSiegewarfareError()
// Desc : 
// ----------------------------------------------------------------------------
void CheckDratanSiegewarfareError(UBYTE errcode)
{
	CTString tStr;
	switch(errcode)
	{
		case MSG_EX_CASTLE_ERROR_NOT_JOIN:    // 공성 참가주이 아님
				tStr = _S( 3739,"공성 참가중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_WAR:    // 공성전 중이 아님
				tStr = _S( 3740,"공성중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_POS_FAIL:    // 위치 이상
				tStr = _S( 3741,"공성 지역이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_MEM_5_FAIL:    // 교감 인원 자리 없음
				tStr = _S( 3742,"교감 인원이 초과되었습니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_OWNER:    // 성주 길드 아님
				tStr = _S( 3743,"성주 길드가 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND:   // 교감 중이 아님
				tStr = _S( 3744,"교감중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME:  // 교감 시간 부족
				tStr = _S( 3745,"교감 시간이 부족합니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_MONEY:    // 돈 부족
				tStr = _S(306,"나스가 부족합니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_ATTACK:    // 공성 길드 아님
				tStr = _S( 3746,"공성 길드가 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_DATA:    // 데이터 이상
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_DIFFER:    // 이전 데이터와 동일				
				_S( 3747,"이미 강화된 단계입니다.");				
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY:    // 구매하지 않았음				
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC:     // 없는 타워
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC:    // 죽은 타워 
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD:   // 길드 원이 아님
				tStr = _S(985,"해당 길드원을 찾을 수 없습니다.");
				break;				
		case MSG_EX_CASTLE_ERROR_USE_NPC:    // 사용중 부활 진지
				tStr = _S(3749,"사용중인 부활진지입니다.");
				break;
		case MSG_EX_CASTLE_ERROR_USE_NPC_OTHER:   // 이미 다른 부활진지 사용중
				tStr = _S(3750,"다른 부활진지를 사용중입니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC2:     // 없는 부활 진지
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC2:    // 죽은 부활진지
				tStr.PrintF("Error %d",errcode);
				break;				
		case MSG_EX_CASTLE_ERROR_SAME_DATA:	   // 같은 데이터 - 타입과 단계가 안맞을때...
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY_NPC:  // 더이상 NPC를 구매 할수 없습니다,
				tStr = _S(3812, "더이상 구입할 수 없습니다.");
				break;

	}
	
	if(tStr.Length()>0 )
		_pUIMgr->GetChatting()->AddSysMessage( tStr , SYSMSG_ERROR );	
	
}

// ----------------------------------------------------------------------------
// Name : CheckCashMoonStoneMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CheckCashMoonStone(UBYTE errcode)
{
	switch(errcode)
	{
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_SUCCESS:				// 성공

			return TRUE;
			
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM:		
			_pUIMgr->GetChatting()->AddSysMessage( _S(2908, "교환할 아이템이 없습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN:		
			_pUIMgr->GetChatting()->AddSysMessage( _S(265, "인벤토리가 부족합니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON:			
			_pUIMgr->GetChatting()->AddSysMessage( _S(2907, "이용 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );	
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
	switch(errcode)
	{
		case MSG_EX_CASHITEM_ERROR_SUCCESS:				// 성공
	//		_pUIMgr->GetChatting()->AddSysMessage( _S(2408,  "요청한 작업이 성공하였습니다." ), SYSMSG_ERROR );	
			return TRUE;
			
		case MSG_EX_CASHITEM_ERROR_LACKCASH:			// 보유캐쉬 부족
			_pUIMgr->GetChatting()->AddSysMessage( _S(2409,  "보유 캐쉬가 부족 합니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_NOTUSER:				// 해당유저 없음
			_pUIMgr->GetChatting()->AddSysMessage( _S(2410,  "해당 유저가 존재하지 않습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_NOTITEM:				// 존재하지 않는 상품입니다
			_pUIMgr->GetChatting()->AddSysMessage( _S(2411,   "존재하지 않는 상품입니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_DB:					// DB오류
			_pUIMgr->GetChatting()->AddSysMessage( _S( 1843, "서버에 연결할 수 없습니다.(B)" ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_PACKET:				// 패킷 오류
			_pUIMgr->GetChatting()->AddSysMessage( _S( 854 ,  "패킷 오류가 발생하였습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_ETC:					// 기타 오류
			_pUIMgr->GetChatting()->AddSysMessage( _S(1094,   "알 수 없는 오류가 발생하였습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_OVERITEM:				// 구입 상품 개수 초과(10개)
			_pUIMgr->GetChatting()->AddSysMessage( _S( 2413,   "구입 상품이 10개를 초과하였습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_NOSPACE:				// 구매물품 보관소의 공간 부족
			_pUIMgr->GetChatting()->AddSysMessage( _S( 2414,  "구매물품 보관소의 공간이 부족합니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_LACKINVEN:			// 케릭터 인벤 부족
			_pUIMgr->GetChatting()->AddSysMessage( _S( 265,  "캐릭터 인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_CONN:					// 커넥터 서버 에러
			_pUIMgr->GetChatting()->AddSysMessage( _S( 2415,  "커넥터 서버 에러입니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_BILL:					// 빌링 서버 에러
			_pUIMgr->GetChatting()->AddSysMessage( _S( 2416,  "빌링 서버 에러입니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_ERROR_NOTHAVECT :
			_pUIMgr->GetChatting()->AddSysMessage( _S( 2769, "품절된 상품 입니다." ), SYSMSG_ERROR );	
			break;


		// 선물 관련 :Su-won	|---------->
		case MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS:
			_pUIMgr->GetCashShop()->Message(MSGCMD_GIFT_ERROR, _S(3102, "선물 보내기"), _S(3106, "선물 보내기가 완료되었습니다."), UMBS_OK);
			return TRUE;
		case MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR:
			_pUIMgr->GetCashShop()->Message(MSGCMD_GIFT_ERROR, _S(3107, "선물 보내기 에러"), _S(3108, "해당 캐릭터에게로 선물을 보낼 수 없습니다. 캐릭터 명을 정확하게 입력해 주세요."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS:
			_pUIMgr->GetCashShop()->IsEmptyRecvGift();
			_pUIMgr->GetCashShop()->Message(MSGCMD_GIFT_ERROR, _S(3109, "선물 받기"), _S(3110, "선물 받은 물품들이 캐릭터 아이템 인벤토리로 옮겨졌습니다."), UMBS_OK);
			return TRUE;
		// 선물 관련 :Su-won	<----------|
	}
	return FALSE;

}

// ----------------------------------------------------------------------------
// Name : ReceiveExtendMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExtendMessage( CNetworkMessage *istr )
{
	LONG	ubType;
	UBYTE	errcode;
	UBYTE	exType;
	(*istr) >> ubType;
	CTString		strTitle,strMessage,newName;
	ULONG			nIndex;
	UBYTE			nJob;
	BYTE			bIsGuildName;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(ubType)
	{
	case MSG_EX_PET_STATUS:
		ReceivePetStatusMessage(istr);
		break;
	case MSG_EX_PET_MOUNT:
		ReceivePetMountMessage(istr);
		break;
	case MSG_EX_PET_LEARN:			// 펫 교육			: skillindex(n) level(c:server) errorcode(n:server)
		ReceivePetLearnMessage(istr);
		break;
	case MSG_EX_PET_SKILLLIST:		// 펫 스킬 리스트	: index(n) count(n) [skillindex(n) skilllevel(c)] ...
		ReceivePetSkillListMessage(istr);
		break;
	case MSG_EX_PET_RESET_SKILL:	// 펫 스킬 초기화
		ReceivePetResetSkillMessage(istr);
		break;
	case MSG_EX_PET_SELL_INFO:		// 펫 교환/판매 정보: ownerindex(n) petindex(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) ability(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n)
		ReceivePetSellInfoMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_MOUNT:	// 펫 타입 변경		: errorcode(n:server)
		ReceivePetChangeMountMessage(istr);
		break;	
	
	case MSG_EX_PET_COMMAND:		// 펫 사교동작		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Pet(MSG_EX_PET_COMMAND, &(*istr));			
		}
		break;
	case MSG_EX_PET_LEVELUP: // 펫 레벨업 petindex(n) typegrade(c) level(n)
		ReceivePetLevelUpMessage(istr);
		break;
	case MSG_EX_PET_MIX_ITEM:		// 펫 이아템 조합	: 
		ReceviePetItemMixMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_ITEM:	// 펫 아이템 교환  
		ReceviePetItemChangeMessage(istr);
		break;
	case MSG_EX_PET_REBIRTH:
		ReceviePetRebirthMessage(istr); // 펫 봉인 해제
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
					if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"확인");
					strMessage	=_S( 2131, 	"길드장에 의해 길드 이름이 변경되었습니다" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
					
				}
				else 
				{
					_pNetwork->MyCharacterInfo.name=newName;
					if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"확인");
					strMessage	=_S( 2132, 	"이름이 성공적으로 변경되었습니다" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
					
					//wooss 051004 개명시 메신저상 이름도 변경
					_pUIMgr->GetMessenger()->SetMyInfo(-1,newName);
					
				}
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_DUPLICATE:
				if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_DUPLICATE)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S(12,"중복된 이름 입니다."); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_HELPER:
				if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_HELPER)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=	_S( 2133, "헬퍼서버 오류 입니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_INVALID:
				if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_INVALID)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S( 2134, 	"올바른 형식의 이름이 아닙니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS:
				if(_pUIMgr->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S( 2136, 	"길드이름은 길드장만이 변경할 수 있습니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;			
			}
			break;

		case MSG_EX_CASHITEM:			// 아이템 구매		: subtype(uc) ...
			{
				ULONG tv_cash,nCtid,nCount;
				
				ULONG tv_remainCnt;
				ULONG tv_listCnt;
				ULONG tv_cashIdx;
				int i;

			//	ULONG test;
				(*istr) >> exType;
				switch(exType)
				{
					case MSG_EX_CASHITEM_BALANCE_REP :		// 캐쉬				: errorCode(uc) cashBalance(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr) >> 	tv_cash;			
								_pUIMgr->GetCashShop()->SetMyCash(tv_cash);
								(*istr) >> 	tv_listCnt;
								if (tv_listCnt >0)
								{
									CCashShopData& CS = _pNetwork->GetCashShopData(UCSS_PLATINUM);
									for(i=0;i<tv_listCnt;i++ ){
										(*istr) >> tv_cashIdx;
										(*istr) >> tv_remainCnt;
										if(tv_cashIdx == CS.m_vShopItemArray[i].m_shopItemIndex)
											CS.m_vShopItemArray[i].m_limitCnt = tv_remainCnt;
//										CTString tv_str;
//										tv_str.PrintF("%d %d %d",tv_cashIdx,CS.m_vShopItemArray[i].m_shopItemIndex,tv_remainCnt,CS.m_vShopItemArray[i].m_limitCnt);
									}
								}
							}
							
						}
						break;			
					case MSG_EX_CASHITEM_PURCHASE_REP :		// 응답				: errorCode(uc) cashBalance(n)
						{							
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr) >> 	tv_cash;			
								_pUIMgr->GetCashShop()->SetMyCash(tv_cash);
								_pUIMgr->GetCashShop()->ClearBtnItems(_pUIMgr->GetCashShop()->m_abtnTradeItems,MAX_KIT_SIZE);
								(*istr) >> 	tv_listCnt;
								if (tv_listCnt >0)
								{
									CCashShopData& CS = _pNetwork->GetCashShopData(UCSS_PLATINUM);
									for(i=0;i<tv_listCnt;i++ ){
										(*istr) >> tv_cashIdx;
										(*istr) >> tv_remainCnt;
										if(tv_cashIdx == CS.m_vShopItemArray[i].m_shopItemIndex)
											CS.m_vShopItemArray[i].m_limitCnt = tv_remainCnt;
									}
								}
							}
						}
						break;			
					case MSG_EX_CASHITEM_BRING_REP:			// 응답				: errorCode(uc) 
						{
							(*istr) >> errcode;

							_pUIMgr->GetCashShop()->m_bBringItem = FALSE; // 아이템 가져오기 완료

							if(CheckCashItemMessage(errcode)){
								_pUIMgr->GetCashShop()->ClearBtnItems(_pUIMgr->GetCashShop()->m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);
							}

						}
						break;	
					case MSG_EX_CASHITEM_PURCHASELIST_REP:   // 응답				: count(n) idx(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr) >> nCount;
								nCount = (nCount>KIT_SLOT_TOTAL)?KIT_SLOT_TOTAL:nCount;
								for( int i = 0 ; i < nCount  ; i++){
									CUIButtonEx tv_btn;
									(*istr) >> nIndex;
									(*istr) >> nCtid;
									if(_pUIMgr->GetCashShop()->SetCashIndexToBtn(nCtid,nIndex,tv_btn)){
										_pUIMgr->GetCashShop()->m_abtnKitItems[i] = tv_btn;
										_pUIMgr->GetCashShop()->m_abtnKitItems[i].Copy(tv_btn);
									}

								}
							}
							
						}
						break;	
					case MSG_EX_CASHITEM_PURCHASEHISTORY_REP:// 응답				: errorcode(uc) count(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr >> nCount);
								if( nCount > 0){
									_pUIMgr->GetCashShop()->m_abtnHistoryItems.New(nCount);
									_pUIMgr->GetCashShop()->m_sbHistory.SetScrollPos( 0 );
									_pUIMgr->GetCashShop()->m_sbHistory.SetScrollRange(nCount);
									_pUIMgr->GetCashShop()->m_sbHistory.SetCurItemCount(nCount);
									for( int i=0; i < nCount ;i++){
										CUIButtonEx tv_btn;
										ULONG nItemCnt;
										(*istr) >> nItemCnt;
										(*istr) >> nCtid;
										_pUIMgr->GetCashShop()->m_abtnHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
										if(_pUIMgr->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn)){
											_pUIMgr->GetCashShop()->m_abtnHistoryItems[i] = tv_btn;
											_pUIMgr->GetCashShop()->m_abtnHistoryItems[i].Copy(tv_btn);
											_pUIMgr->GetCashShop()->m_abtnHistoryItems[i].SetItemCount(nItemCnt);
										}
									}
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
								_pUIMgr->GetGamble()->SetSelectedMarker( grade, grade );
								_pUIMgr->GetGamble()->Start();
							}						
						}
						break;

					// 선물 관련 :Su-won	|--------------------------------------------------->
					case MSG_EX_CASHITEM_GIFT_REP:					//응답	: errcode(uc) 
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								_pUIMgr->GetCashShop()->ClearBtnItems(_pUIMgr->GetCashShop()->m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);

								_pUIMgr->GetCashShop()->m_bShowSendGift =FALSE;
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP:		// 응답	: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
									_pUIMgr->GetCashShop()->m_abtnSendHistoryItems.New(nCount);
									_pUIMgr->GetCashShop()->m_sbGift.SetScrollPos( 0 );
									_pUIMgr->GetCashShop()->m_sbGift.SetScrollRange(nCount);
									_pUIMgr->GetCashShop()->m_sbGift.SetCurItemCount(nCount);

									_pUIMgr->GetCashShop()->m_astrSendChar.New(nCount);
									for( int i=0; i < nCount ;i++)
									{
										CUIButtonEx tv_btn;
										//ULONG nItemCnt;
										(*istr) >> nCtid;
										(*istr) >> newName;
										_pUIMgr->GetCashShop()->m_abtnSendHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
										if(_pUIMgr->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
										{
											_pUIMgr->GetCashShop()->m_abtnSendHistoryItems[i] = tv_btn;
											_pUIMgr->GetCashShop()->m_abtnSendHistoryItems[i].Copy(tv_btn);
											//_pUIMgr->GetCashShop()->m_abtnSendHistoryItems[i].SetItemCount(nItemCnt);
											_pUIMgr->GetCashShop()->m_astrSendChar[i] =newName;
										}
									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP:// 응답				: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
									_pUIMgr->GetCashShop()->m_abtnRecvHistoryItems.New(nCount);
									_pUIMgr->GetCashShop()->m_sbGift.SetScrollPos( 0 );
									_pUIMgr->GetCashShop()->m_sbGift.SetScrollRange(nCount);
									_pUIMgr->GetCashShop()->m_sbGift.SetCurItemCount(nCount);

									_pUIMgr->GetCashShop()->m_astrReceiveChar.New(nCount);
									for( int i=0; i < nCount ;i++)
									{
										CUIButtonEx tv_btn;
										//ULONG nItemCnt;
										//(*istr) >> nItemCnt;
										(*istr) >> nCtid;
										(*istr) >> newName;
										_pUIMgr->GetCashShop()->m_abtnRecvHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
										if(_pUIMgr->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
										{
											_pUIMgr->GetCashShop()->m_abtnRecvHistoryItems[i] = tv_btn;
											_pUIMgr->GetCashShop()->m_abtnRecvHistoryItems[i].Copy(tv_btn);
											//_pUIMgr->GetCashShop()->m_abtnReceiveHistoryItems[i].SetItemCount(nItemCnt);
											_pUIMgr->GetCashShop()->m_astrReceiveChar[i] =newName;
										}										
									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVLIST_REP:// 응답
						{
							//listflag(c) count(n) idx(n) ctid(n) date(un) sendcharName(str) msg(str)
							BYTE cListFlag;
							ULONG nDate;

							(*istr) >> cListFlag;

							int nPrevCount;
							(*istr) >> nCount;

							if( nCount ==0)
							{
								_pUIMgr->GetCashShop()->m_sbReceive.SetCurItemCount(0);
								break;
							}

							if( cListFlag & 0x1 )
							{
								nPrevCount=0;
								_pUIMgr->GetCashShop()->m_sbReceive.SetCurItemCount(0);

								_pUIMgr->GetCashShop()->m_abtnRecvGift.New(nCount);
								_pUIMgr->GetCashShop()->m_astrSend.New(nCount);
								_pUIMgr->GetCashShop()->m_astrGiftMessage.New(nCount);
								_pUIMgr->GetCashShop()->m_anDate.New(nCount);
								_pUIMgr->GetCashShop()->m_anRecvOrder.New(nCount);
							}
							else
							{
								nPrevCount =_pUIMgr->GetCashShop()->m_abtnRecvGift.sa_Count;

								_pUIMgr->GetCashShop()->m_abtnRecvGift.Expand(nPrevCount+nCount);
								_pUIMgr->GetCashShop()->m_astrSend.Expand(nPrevCount+nCount);
								_pUIMgr->GetCashShop()->m_astrGiftMessage.Expand(nPrevCount+nCount);
								_pUIMgr->GetCashShop()->m_anDate.Expand(nPrevCount+nCount);
								_pUIMgr->GetCashShop()->m_anRecvOrder.Expand(nPrevCount+nCount);
							}

							for(int i=nPrevCount; i < nPrevCount+nCount ; i++)
							{
								CUIButtonEx tv_btn;
								(*istr) >> nIndex;
								(*istr) >> nCtid;
								if(_pUIMgr->GetCashShop()->SetCashIndexToBtn(nCtid,nIndex,tv_btn))
								{
									_pUIMgr->GetCashShop()->m_abtnRecvGift[i] = tv_btn;
									_pUIMgr->GetCashShop()->m_abtnRecvGift[i].Copy(tv_btn);
								}
								(*istr) >> nDate;
								(*istr) >> newName;
								(*istr) >> strMessage;

								_pUIMgr->GetCashShop()->m_anDate[i] =nDate;
								_pUIMgr->GetCashShop()->m_astrSend[i] =newName;
								_pUIMgr->GetCashShop()->m_astrGiftMessage[i] =strMessage;
								_pUIMgr->GetCashShop()->m_anRecvOrder[i] =i;
							}
							int nCurItemCount =_pUIMgr->GetCashShop()->m_sbReceive.GetCurItemCount();
							_pUIMgr->GetCashShop()->m_sbReceive.SetCurItemCount( nCurItemCount+nCount);
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECV_REP:		// 응답				: errCode(uc)
						{
							(*istr) >> errcode;

							_pUIMgr->GetCashShop()->m_bBringItem = FALSE; // 아이템 가져오기 완료

							if(CheckCashItemMessage(errcode))
							{
								_pUIMgr->GetCashShop()->ClearBtnItems(_pUIMgr->GetCashShop()->m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);
							}
							else if( errcode == MSG_EX_CASHITEM_ERROR_LACKINVEN)
								_pUIMgr->GetCashShop()->Message(MSGCMD_GIFT_ERROR, _S(3111, "선물 받기 에러"), _S( 265,  "캐릭터 인벤토리 공간이 부족합니다." ), UMBS_OK);

						}
						break;
					case MSG_EX_CASHITEM_GIFT_NOTICE:
						{
							BYTE bGift;

							(*istr) >> bGift;
							_pUIMgr->GetQuickSlot()->SetGiftRecv(bGift);
						}
						break;
					// 선물 관련 :Su-won	<---------------------------------------------------|
				}
			}
			break;

		// wooss 051004 개명후 메신저 등록
		case MSG_EX_FRIENDNAMECHANGE :
			(*istr) >> nIndex;
			(*istr) >> newName;
			(*istr) >> nJob;
			_pUIMgr->GetMessenger()->DeleteMember(nIndex);
			_pUIMgr->GetMessenger()->AddFriendList(nIndex, 0, newName,(eJob)nJob);
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
					_pUIMgr->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 1 );
					_pUIMgr->GetMap()->ReSetData();
				}
				else
				{
					_pUIMgr->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 0 );
					return;
				}

				_pUIMgr->GetMap()->InitCastleData();
				// 권좌
				(*istr) >> fLordX;
				(*istr) >> fLordZ;
				(*istr) >> nLordHP;
				(*istr) >> nLordMaxHP;
				
				_pUIMgr->GetMap()->AddCastleData( CASTLE_LORD, fLordX, fLordZ, -1, nLordHP, nLordMaxHP );

				// 공성 탑 
				(*istr) >> lTowerCount;

				for( int i = 0; i < lTowerCount; i++ )
				{
					(*istr) >> lTowerIndex;
					(*istr) >> fTowerX;
					(*istr) >> fTowerZ;
					(*istr) >> nTowerHP;
					(*istr) >> nTowerMaxHP;

					_pUIMgr->GetMap()->AddCastleData( CASTLE_TOWER, fTowerX, fTowerZ, lTowerIndex, nTowerHP, nTowerMaxHP );
				}

				// 리젠 포인트
				(*istr) >> fRegenX;
				(*istr) >> fRegenY;
				
				_pUIMgr->GetMap()->AddCastleData( CASTLE_REGEN, fRegenX, fRegenY );
				
				// Final Signal... 
				(*istr) >> nSenderFlag;
				(*istr) >> nSenderIndex;
				(*istr) >> fSignalX;
				(*istr) >> fSignalY;
					
				_pUIMgr->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );

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
				
				_pUIMgr->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );
			}
			break;

		case MSG_EX_ELEMENTAL_STATUS:
			{
				ReceiveElementalStatusMessage( istr );
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
				_pUIMgr->GetGuild()->SetMemberName(charIndex,newName);

			}
			break;
		case MSG_EX_PARTY_MATCH :		// Date : 2006-05-09(오후 5:43:02), By eons
			{
				ULONG	ErrorType;
				ULONG	MatchType;
				(*istr) >> MatchType;

				switch( MatchType )
				{
				case MSG_EX_PARTY_MATCH_REG_MEMBER_REP:
					{
						(*istr) >> ErrorType;
						
						_pUIMgr->GetPartyAuto()->MatchRegMemberRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_REG_PARTY_REP:
					{
						(*istr) >> ErrorType;

						_pUIMgr->GetPartyAuto()->MatchRegPartyRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_MEMBER_LIST_REP:
					{
						_pUIMgr->GetPartyAuto()->ReceiveMemberData( istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_PARTY_LIST_REP:
					{
						_pUIMgr->GetPartyAuto()->ReceivePartyData( istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_DEL_REP:
					{
						strMessage = _S( 2732, "등록이 삭제 되었습니다." );
						MsgBoxInfo.SetMsgBoxInfo( _S( 2733, "등록 삭제" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );						
					}
					break;
				case MSG_EX_PARTY_MATCH_INVITE_REP:
					{
						(*istr) >> ErrorType;

						_pUIMgr->GetPartyAuto()->ReceivePartyInviteMessage( ErrorType, istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_JOIN_REP:
					{
						(*istr) >> ErrorType;

						_pUIMgr->GetPartyAuto()->ReceivePartyJoinMessage( ErrorType, istr );
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
				else strMessage=_S(strIdx,"서버 호출 스트링");

				switch(outType)
				{
					case MSG_EX_STRING_OUTPUT_WINDOW :		// 창
						{
							MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							MsgBoxInfo.AddString(strMessage);
							_pUIMgr->CreateMessageBox(MsgBoxInfo);

						}
						break;
					case MSG_EX_STRING_OUTPUT_SYS :			// 시스템창
						_pUIMgr->GetChatting()->AddSysMessage(strMessage);
						break;
					case MSG_EX_STRING_OUTPUT_NOTICE :		// 공지
						_UIAutoHelp->SetGMNotice ( strMessage );
						break;
					case MSG_EX_STRING_OUTPUT_CHAT :		// 챗창
						 _pUIMgr->GetChatting()->AddChatMessage(MSG_CHAT_SAY,
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
					_pUIMgr->GetSiegeWarfareNew()->StartConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_END:	// 교감 완료								
					(*istr) >> charIdx;
					(*istr) >> tStrtmp;
					tStr.PrintF(_S(3898,"[%s]님이 크리스탈 교감에 성공하였습니다."), tStrtmp);
					_pUIMgr->GetChatting()->AddSysMessage( tStr, SYSMSG_ERROR );
					_pUIMgr->GetSiegeWarfareNew()->CompleteConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_FAIL:	// 교감 실패
					(*istr) >> charIdx;
					if( _pNetwork->MyCharacterInfo.index == charIdx)
					{
						_pUIMgr->GetChatting()->AddSysMessage( _S( 3751,"교감에 실패하였습니다."), SYSMSG_ERROR );
					}
					_pUIMgr->GetSiegeWarfareNew()->StopConsensus(charIdx);					
					break;
				case MSG_CASTLE_TOWER_CONTRAL:
					{
						_pUIMgr->GetChatting()->AddSysMessage( _S( 3752,"정상적으로 가동되었습니다." ), SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_CONTRAL_LIST:		// 마스트 타워 설정 리스트
					{
						UBYTE tSet[DRATAN_TOWER_KIND_MAX];
						for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
						{
							(*istr) >> tSet[i];
							_pUIMgr->GetSiegeWarfareNew()->SetTowerSet(i,tSet[i]);
						}
						
						_pUIMgr->GetSiegeWarfareNew()->SetCBFromTowerSet();
						_pUIMgr->GetSiegeWarfareNew()->OpenCheckTower();						
						
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
							tStr = _S( 3753,"공격형 타워");
							break;
						case 1:
							tStr = _S( 3754,"가드형 타워");
							break;
						case 2:
							tStr = _S( 3755,"성문");
							break;
						}						
						tStr2.PrintF("%d %s",(int)tLevel, _S( 3756,"단계가 정상적으로 강화되었습니다." ));
						tStr += tStr2;
						_pUIMgr->GetChatting()->AddSysMessage( tStr, SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_REINFORCE_LIST:
					{					
						UBYTE tType,tLevel;
						(*istr) >> tType >> tLevel;		
						if( (int)tType == _pUIMgr->GetSiegeWarfareNew()->GetUpgradeType())
						{								
							
							_pUIMgr->GetSiegeWarfareNew()->SetUpgradedLevel(int(tLevel));
							_pUIMgr->GetSiegeWarfareNew()->SetUpgradePos(int(tLevel));
							_pUIMgr->GetSiegeWarfareNew()->SetUpgradeLevel((int)tLevel );
							_pUIMgr->GetSiegeWarfareNew()->OpenUpgradeTower();
						}
						else 
							_pUIMgr->GetChatting()->AddSysMessage( _S( 3757,"강화할 타워가 올바르지 않습니다." ), SYSMSG_ERROR );	
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE:			// 공성 타워 수리    cl->gs idx(n) | gs->cl idx(n)
					{
						ULONG tIdx;
						(*istr) >> tIdx;
						_pUIMgr->GetChatting()->AddSysMessage( _S( 3758,"정상적으로 수리되었습니다." ), SYSMSG_ERROR );	
						
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE_LIST:		// 공성 타워 수리 상태   cl->gs idx(n) | gs->cl money(n)
					{
						ULONG tNpcIdx;
						SQUAD tMoney;

						(*istr) >> tNpcIdx >> tMoney;
						_pUIMgr->GetSiegeWarfareNew()->SetRepairMoney( tNpcIdx,tMoney );
						_pUIMgr->GetSiegeWarfareNew()->OpenRepairTower();
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
							_pUIMgr->GetGuildWarPortal()->SetDratanPortalFlag((int)tNpcIdx);
						}
						_pUIMgr->GetGuildWarPortal()->OpenGuildWarPortalDratan();
					}
					break;				
				case MSG_CASTLE_QUARTERS_INSTALL:		// 부활진지 설치 cl->gs idx(n) | gs->cl  idx(n), gidx(n)
					{
						CTString tStr,tGuildName;
						ULONG tNpcIdx,tGuildIdx;
						SLONG tNpcUniIndex;
						(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName >> tNpcUniIndex;
						tStr.PrintF(_S( 3759,"[%s]길드가 [%d]번 부활진지를 점령했습니다." ),tGuildName,tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;
						_pUIMgr->GetChatting()->AddSysMessage( tStr, SYSMSG_ERROR );

						// 몹 이름 변경
						CMobData& MD = _pNetwork->GetMobData(tNpcIdx);
						if(MD.GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
						{							
							tStr = tGuildName + CTString(" ") + _S( 3760,"공성 부활진지");;
							MD.SetMobName(tStr);

							tStr = MD.GetName();
							MD = _pNetwork->GetMobData(tNpcIdx);
							tStr = MD.GetName();
						}

						if (!MD.IsNPC())
						{
							CEntity* pEntity = NULL;
							if (_pNetwork->SearchEntityByNetworkID(tNpcUniIndex, MSG_CHAR_NPC, pEntity))
							{	// 일반 npc에서 몬스터로 설정 변경(수성 측이 공격 가능한 npc로 )
								if (pEntity != NULL)
								{
									pEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
								}
							}
						}
					}
					break;
				case MSG_CASTLE_QUARTERS_CRUSH:			// 부활 진지 파괴 gs->cl idx(n)
					{
						CTString tStr;
						ULONG tNpcIdx;
						(*istr) >> tNpcIdx;
						tStr.PrintF(_S( 3705,"%d번 부활진지가 파괴되었습니다." ),tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = -1;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = CTString("");
						_pUIMgr->GetChatting()->AddSysMessage( tStr, SYSMSG_ERROR );

						// Hard Cording ^^;; 어쩔수 없이....
						// 몹 이름 변경
						CMobData& MD = _pNetwork->GetMobData(tNpcIdx);
						if(MD.GetMobIndex()>0)
						{
							tStr.PrintF(_S( 3685,"공성 부활진지%d"),tNpcIdx-389);
							MD.SetMobName(tStr);
						}						
					}
					break;
				case MSG_CASTLE_QUARTERS_LIST:			// 부활 진지 리스트 gs->cl 7*(nindex(n), gindex(n))
					{		
						CTString tGuildName;
						LONG tNpcIdx,tGuildIdx;

						for(int i=0;i<7;i++)
						{	
							(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName;						
							// 점령 길드가 존재하면...
							if (tGuildIdx!=-1)
							{
								_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
								_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;

								// 몹 이름 변경
								CMobData& MD = _pNetwork->GetMobData(tNpcIdx);
								if(MD.GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
								{
									tStr = tGuildName +CTString(" ") +_S( 3760,"공성 부활진지");
									MD.SetMobName(tStr);
								}
							}
							// 점령 길드가 없다면...
							else 
							{
								// Hard Cording ^^;; 어쩔수 없이....
								// 몹 이름 변경
								CMobData& MD = _pNetwork->GetMobData(tNpcIdx);
								if(MD.GetMobIndex()>0)
								{
									tStr.PrintF(_S( 3685,"공성 부활진지%d"),tNpcIdx-389);
									MD.SetMobName(tStr);
								}						
							}
						}
					}
					break;

				case MSG_CASTLE_WAIT_TIME:				// 부활 대기 시간
					{
						LONG tWaitTime;
						(*istr) >> tWaitTime;						
						_pUIMgr->GetSiegeWarfareNew()->SetWaitTime(tWaitTime);
						_pUIMgr->GetSiegeWarfareNew()->SetTimeReply(TRUE);

					}
					break;
				case MSG_EX_CASTLE_ERROR:				// 공성 에러
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
// EDIT : BS : 070413 : 신규 패킷 암호화			
#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
	case MSG_EX_KEYCHANGE:		// EDIT : BS : 패킷 암호화 : 키 변경
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

					_pUIMgr->GetPortal()->Create_SiegeDungeon_State_MsgBox( ulEnvRate, ulMonRate);
				}
				break;
			case MSG_MANAGEMENT:
				{
					UBYTE sbSubSubType, ubError;

					(*istr) >> sbSubSubType;
					switch( sbSubSubType )
					{
					case MSG_MANAGEMENT_MANAGER_CONFIRM:					//관리 요청
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								_pUIMgr->CreateMessageBoxL( _S(3908, "던전 관리"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								_pUIMgr->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3940, "안녕하십니까? 성주님."),-1,0xa3a1a3ff);
								_pUIMgr->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3941, "던전의 어떤 부분을 관리하시겠습니까?"),-1,0xa3a1a3ff);
								
								_pUIMgr->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3921, "던전 내부 환경 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT);
								_pUIMgr->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3926, "던전 내부 몬스터 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER);
								_pUIMgr->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3931, "던전 입장료 조정" ), DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE);
								_pUIMgr->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3935, "던전 수렵세율 조정" ), DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE);
								_pUIMgr->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 1220, "취소한다." ) );
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3942, "성주 길드의 마스터, 또는 부길드마스터만 접근할 수 있습니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_INFO:				//던전 내부 환경 정보 요청
						{
							ULONG ulCurRate;
							(*istr) >> ulCurRate;

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "더이상 상승시킬 수 없습니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );

								return;
							}

							_pUIMgr->GetPortal()->Create_SiegeDungeon_EnvCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_MONSTER_INFO:				//던전 내부 몬스터 정보 요청
						{
							ULONG ulCurRate;
							(*istr) >> ulCurRate;

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "더이상 상승시킬 수 없습니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );

								return;
							}

							_pUIMgr->GetPortal()->Create_SiegeDungeon_MonCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_TAX_INFO:				//입장료 세금 정보 요청
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							_pUIMgr->GetPortal()->Create_SiegeDungeon_AdmissionCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_INFO:			//수렵세 정보 요청
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							_pUIMgr->GetPortal()->Create_SiegeDungeon_HuntingCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_CHANGE:				//던전 내부환경 변경
					case MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE:			//던전 내부 몬스터 상태 변경
					case MSG_MANAGEMENT_MANAGER_TAX_CHANGE:				//입장료 세금 변경
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE:		//수렵세 변경
						{
							(*istr) >> ubError;
							if( ubError == MSG_DVD_ERROR_OK )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3944, "던전의 상태가 변경되었습니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_MONEY)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "나스가 부족합니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
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

							if( _pUIMgr->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ENTER) )
								_pUIMgr->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENTER);
							
							strMessage.PrintF( _S( 191, "확인" ) );
							MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_ENTER);
							strMessage.PrintF( _S(3945, "던전으로 이동하기 위해서는 %d나스가 필요합니다. 이동 하시겠습니까?" ), ulNeedNas );
							MsgBoxInfo.AddString(strMessage);

							_pUIMgr->CreateMessageBox(MsgBoxInfo);
						}
						break;
					case MSG_DUNGEON_GO:
						{
							UBYTE ubError;
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_MONEY )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "나스가 부족합니다.") );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
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
						for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
						{
							CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
							if( ct.cha_Index == ulCharIndex )
							{	
								ct.cha_ubGuildNameColor =ubCol;
								break;
							}
						}
					}
				}
				break;
			}
		}
		break;
		//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_EX_PET_TURNTO_NPC: // 펫 변신
		{
			SLONG slPetIndex;
			SLONG slNpcIndex;
			SLONG slCharIndex;
			SLONG slNpcSize;

			(*istr) >> slPetIndex >> slNpcIndex >> slCharIndex >> slNpcSize;

			CEntity* penEntity = NULL;

			for(int i=0; i<_pNetwork->ga_srvServer.srv_actPet.Count(); ++i)
			{
				if( _pNetwork->ga_srvServer.srv_actPet[i].pet_Index == slPetIndex)
				{
					penEntity =_pNetwork->ga_srvServer.srv_actPet[i].pet_pEntity;
				}
			}

			if (penEntity != NULL)
			{
				if (penEntity == _pNetwork->_PetTargetInfo.pen_pEntity) // 자신이 장착한 펫일 경우
					_pNetwork->_PetTargetInfo.TransformIndex = slNpcIndex;

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(penEntity, slNpcIndex, slNpcSize); // 10은 사이즈 1배
			}
		}
		break;
	//ttos : 몬스터 콤보
	case MSG_EX_MONSTERCOMBO:
		{			
			UBYTE SubMessage;

			(*istr) >> SubMessage;
			switch(SubMessage)
			{
			case  MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP:
				{
					INDEX nComboCount;
					(*istr) >> nComboCount;
					INDEX tempIndex;
					for(int i = 0; i < nComboCount; i++)
					{
						(*istr) >> tempIndex;
						_pUIMgr->GetCombo()->SetComboList(i,tempIndex);
					}
					_pUIMgr->GetCombo()->SetComboCount(nComboCount);
					_pUIMgr->GetCombo()->SetActionChack(TRUE);
					_pUIMgr->GetCombo()->OpenMonsterCombo(TRUE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
					//_pUIMgr->RearrangeOrder( UI_MONSTER_COMBO, TRUE);

				}
				break;
			case MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT:
				{
					CTString tv_str;
					INDEX nBoss,nNas;
					(*istr) >> nBoss;
					(*istr) >> nNas;
					_pUIMgr->GetCombo()->SetBossIndex(nBoss);
					if(_pUIMgr->DoesMessageBoxExist(MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT)) return ;
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT );
					tv_str.PrintF(_S(4049,"몬스터 콤보 %d Nas 입장료를 내고 입장하시겠습니까?"),nNas);
					MsgBoxInfo.AddString(tv_str);
					_pUIMgr->CreateMessageBox( MsgBoxInfo );

				}break;
			case MSG_EX_MONSTERCOMBO_MISSION_COMPLETE:
				{
					INDEX nNextStagenum;
					UBYTE bComplete;
					(*istr) >> bComplete;
					(*istr) >> nNextStagenum;
					
					
					_pUIMgr->GetCombo()->StageComplete(nNextStagenum,bComplete);
					
				}
				break;
			case MSG_EX_MONSTERCOMBO_NOTICE_STAGE:  // stage(n) 시작시 stage 넘버메시지 전달
				{
					INDEX nStagenum;
					(*istr) >> nStagenum;

					_pUIMgr->GetCombo()->SetStageNum(nStagenum);
					_pUIMgr->GetCombo()->SetSysImage(SYS_STAGE,TRUE);				
				}break;
			case MSG_EX_MONSTERCOMBO_ERROR:
				{
					UBYTE ubError;
					(*istr) >> ubError;
					_pUIMgr->GetCombo()->RecComboErrorMessage(ubError);
				}
				break;
			}
			
		}
		break;
#ifdef RESTART_GAME
	case MSG_EX_RESTART:// 게임 재시작 처리
			_pUIMgr->GetSystemMenu()->Restart_Internal();
		break;
#endif		
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
				_pUIMgr->GetChatting()->AddSysMessage( _S( 265,  "캐릭터 인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
				break;
			case MSG_CHAOSBALL_ERROR_ITEM_FAULT:
				break;
			}			
		}
		break;
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
					_pUIMgr->GetShop()->CreateCubeStateMsgBox(istr, TRUE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP:
				{
					_pUIMgr->GetShop()->CreateCubeStateMsgBox(istr, FALSE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_COMPLETE:
				{
					UBYTE ubCnt;
					(*istr) >> ubCnt;

					_pUIMgr->GetCombo()->SetComboCount(ubCnt);
					_pUIMgr->GetCombo()->SetSysImage(SYS_STAGE, TRUE);
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
							strMessage = _S(1330, "아이템이 부족합니다.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT:
						{
							strMessage = _S(4374, "현재 들어갈 수 있는 방이 존재하지 않습니다. 잠시 후 다시 시도하여 주십시오.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE:
						{
							strMessage = _S(4375, "현재 큐브에 들어갈 수 없습니다. 잠시 후 다시 시도하여 주십시오.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_CAHNGE_RATE:
						{
							strMessage = _S(4376, "입장큐브 세율을 변경할 수 없습니다.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME:
						{
							strMessage = _S(4377, "길드 큐브 입장 시간이 아닙니다.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME:
						{
							strMessage = _S(4397, "파티 큐브 입장 시간이 아닙니다.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_PARTYCUBE:
						{ // 파티 큐브 입장
							strMessage = _S(4398, "파티 큐브에 입장하였습니다.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_PARTYCUBE;
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_GUILDCUBE:
						{ // 길드 큐브 입장
							strMessage = _S(4399, "길드 큐브에 입장하였습니다.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_GUILDCUBE;
							bShowBox = FALSE;
						}
						break;
					}
					
					if (bShowBox)
					{
						MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
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

			MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubSubType)
			{
			case MSG_EX_INIT_SSkill_ERROR_SUCCESS:
				{
					MsgBoxInfo.AddString(_S(4247, "스페셜 스킬 초기화에 성공하였습니다."));
				}
				break;
			case MSG_EX_INIT_SSkill_ERROR_FAIL:
				{
					MsgBoxInfo.AddString(_S(4248, "스페셜 스킬 초기화에 실패하였습니다."));
				}
				break;
			}

			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	}
}

void CSessionState::ReceiveExWildPetMessage(UBYTE index, CNetworkMessage *istr)
{	
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

	switch(index)
	{
	case MSG_SUB_STAT:
		{
			ULONG	pet_index;
			CTString Pet_Name;
			INDEX   pet_level;
			INDEX	pet_type;
			__int64	pet_exp;			
			__int64 pet_next_exp;
			INDEX	pet_hp,pet_mp;
			INDEX	pet_maxhp,pet_maxmp;
			INDEX	pet_faith,pet_max_faith;
			INDEX	pet_stm,pet_max_stm,pet_point;
			INDEX	pet_str,pet_str_plus;
			INDEX	pet_con,pet_con_plus;
			INDEX	pet_dex,pet_dex_plus;
			INDEX	pet_int,pet_int_plus;
			INDEX	pet_state[STATE_END];
			FLOAT	x;
			FLOAT	z;
			FLOAT	h;
			FLOAT	r;
			SBYTE	yLayer;
			UBYTE	sbAttributePos;

			(*istr) >> pet_index;
			(*istr) >> Pet_Name;
			(*istr) >> pet_level;
			(*istr) >> pet_type;
			(*istr) >> pet_exp;
			(*istr) >> pet_next_exp;
			(*istr) >> pet_hp; 
			(*istr) >> pet_maxhp;
			(*istr) >> pet_mp;
			(*istr) >> pet_maxmp;
			(*istr) >> pet_faith >> pet_max_faith;
			(*istr) >> pet_stm >> pet_max_stm;
			(*istr)	>> pet_str >> pet_str_plus;
			(*istr)	>> pet_con >> pet_con_plus;
			(*istr)	>> pet_dex >> pet_dex_plus;
			(*istr)	>> pet_int >> pet_int_plus;
			(*istr)	>> pet_point;
			for(int i = 0; i < STATE_END; i++)
			{
				(*istr) >> pet_state[i];
			}
			(*istr) >> x >> z >> h >> r;
			(*istr) >> yLayer;
			(*istr) >> sbAttributePos;

			for(INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl++)
			{
				CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl];

				if(pet_index == pt.m_nNetIndex)
				{
					pt.m_strName = Pet_Name;
					pt.m_nLevel = pet_level;
					pt.m_nType = pet_type;
					pt.m_exp = pet_exp;
					pt.m_next_exp = pet_next_exp;
					pt.m_nHP = pet_hp;
					pt.m_nMaxHP = pet_maxhp;
					pt.m_nMP = pet_mp;
					pt.m_nMaxMP = pet_maxmp;
					pt.m_nFaith = pet_faith;
					pt.m_nMaxFaith = pet_max_faith;
					pt.m_nStm = pet_stm;
					pt.m_nMaxStm = pet_max_stm;
					pt.m_nLevelupPoint = pet_point;
					pt.m_sbAttributePos = sbAttributePos;
					pt.m_sbYlayer = yLayer;
					pt.m_nStr = pet_str;
					pt.m_nStrPlus = pet_str_plus;
					pt.m_nCon = pet_con;
					pt.m_nConPlus = pet_con_plus;
					pt.m_nDex = pet_dex;
					pt.m_nDexPlus = pet_dex_plus;
					pt.m_nInt = pet_int;
					pt.m_nIntPlus = pet_int_plus;
					for(int i = 0; i < STATE_END; i++)
					{
						pt.m_nWildPetState[i] = pet_state[i];
					}			


					penPlayerEntity->SetWildPetData(pt.pet_pEntity,pet_hp,pet_maxhp);

					if(pt.m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
					{
						_pNetwork->_WildPetInfo = pt;

						sPetItem_Info TemPet;

						TemPet.pet_index = pet_index;
						TemPet.pet_name = Pet_Name;
						TemPet.pet_level = pet_level;
						TemPet.pet_str = pet_str;
						TemPet.pet_con = pet_con;
						TemPet.pet_dex = pet_dex;
						TemPet.pet_int = pet_int;

						_pUIMgr->GetWildPetInfo()->AddWildPetInfo(TemPet);
					}
				}

				

			}
			
		}
		break;
	case MSG_SUB_ITEM_WEAR:
		{
			SBYTE wear_pos;
			INDEX wear_item,takeoff_item;
			int CurPetid;

			(*istr) >> wear_pos;
			(*istr) >> wear_item;
			(*istr) >> takeoff_item;

			for(INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl++)
			{
				CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl];
				if(_pNetwork->MyCharacterInfo.index == pt.m_nOwnerIndex)
				{
					CurPetid = ipl;
				}
			}

			if(_pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos] == takeoff_item)
			{
				_pGameState->TakeOffArmorTest(_pNetwork->_WildPetInfo.pet_pEntity->GetModelInstance(), takeoff_item);
				_pNetwork->ga_srvServer.srv_actWildPet[CurPetid].m_nPetWearIndex[wear_pos] = -1;
				_pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos] = -1;
			}

			if (_pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos] > 0)
			{
				_pGameState->TakeOffArmorTest(_pNetwork->_WildPetInfo.pet_pEntity->GetModelInstance(), _pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos]);
			}
			_pNetwork->ga_srvServer.srv_actWildPet[CurPetid].m_nPetWearIndex[wear_pos] = wear_item;
			_pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos] = wear_item;
			_pGameState->WearingArmorTest(_pNetwork->_WildPetInfo.pet_pEntity->GetModelInstance(), wear_item);
			
			_pUIMgr->GetWildPetInfo()->PetWearItemReSet();

		}
		break;
	case MSG_SUB_ITEM_WEARING:
		{
			INDEX OwenerIndex,petID;
			INDEX wearIndex[WILDPET_WEAR_TOTAL],wearPlus[WILDPET_WEAR_TOTAL];

			(*istr) >> OwenerIndex;
			(*istr) >> petID;

			for (int i = 0; i < WILDPET_WEAR_TOTAL; i++)
			{
				(*istr) >> wearIndex[i];
				(*istr) >> wearPlus[i];
			}

			for(INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl++)
			{
				CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl];
				if (pt.m_nNetIndex == petID)
				{
					for (i = 0; i < WILDPET_WEAR_TOTAL; i++)
					{
						_pGameState->TakeOffArmorTest(pt.pet_pEntity->GetModelInstance(),pt.m_nPetWearIndex[i]);

						pt.m_nPetWearIndex[i] = wearIndex[i];
						pt.m_nPetWearPlus[i] = wearPlus[i];

						_pGameState->WearingArmorTest(pt.pet_pEntity->GetModelInstance(),pt.m_nPetWearIndex[i]);
					}
					break;
				}
			}

			_pUIMgr->GetWildPetInfo()->PetWearItemReSet();


		}break;
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

					_pUIMgr->GetPetTraining()->LearnSkill( nSkillIndex, nSkillLevel );

					CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );
					strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), rSkill.GetName() );

				}break;
			case 1:
				{
					//착용 펫 없음
					strMessage = _S(2203, "애완동물을 착용하지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 2:
				{
					//레벨이 아님
					strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
				}break;
			case 3:
				{
					//스킬 부족
					strMessage = _S( 281, "스킬 조건이 맞지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 4:
				{
					//필요 아이템 없음
					strMessage = _S( 280, "아이템이 존재하지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 5:
				{
					//필요 아이템 부족
					strMessage = _S(4214, "필요 아이템의 부족으로 스킬을 습득할 수 없습니다. ");
					
				}break;

			}
			// Close message box of skill learn
		_pUIMgr->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

		// Create message box of skill learn
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
									UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );


		}break;
	case MSG_SUB_SKILLLIST:
		{
			INDEX nSkillCont;
			INDEX nSkillIndex;
			UBYTE nSkillLevel;
			(*istr) >> nSkillCont;
			
			_pUIMgr->GetWildPetInfo()->SkillClear();

			for(int con = 0; con < nSkillCont; con++)
			{
				(*istr) >> nSkillIndex;
				(*istr) >> nSkillLevel;
				_pUIMgr->GetWildPetInfo()->AddSkill(nSkillIndex,nSkillLevel);
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
					_pUIMgr->GetPetFree()->ClosePetFree(); //성공
					MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_PETFREE, UI_NONE );
					strMessage.PrintF( _S( 2450, "펫의 봉인을 해제하는데 성공하였습니다.") );
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}break;
			case 1:
				{
					//알맞지 않은 아이템
					_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2452, "봉인을 해제할 펫이 존재하지 않습니다." ), SYSMSG_ERROR );
				}break;
			case 2:
				{
					//나스가 모자람
					_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2451, "펫의 봉인을 해제하기 위한 비용이 부족합니다." ), SYSMSG_ERROR );
				}break;
			case 3:
				{
					//봉인되지 않은 아이템
					_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2452, "봉인을 해제할 펫이 존재하지 않습니다." ), SYSMSG_ERROR );
				}break;
			}
		}break;
	case MSG_SUB_LEVELUP:
		{
			_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2253, "펫이 레벨업 하였습니다." ), SYSMSG_NORMAL );
			WildPetStartEffectGroup("pet_levelup",_pNetwork->_WildPetInfo.m_nNetIndex,_pNetwork->_WildPetInfo.pet_pEntity);

		}break;
	case MSG_SUB_DELETE_EQUIP:
		{
			CTString strPetname,strmassage;
			INDEX nItem_Index;
			SBYTE wear_pos;

			(*istr) >> strPetname;
			(*istr) >> nItem_Index;
			(*istr) >> wear_pos;
	
			_pGameState->TakeOffArmorTest(_pNetwork->_WildPetInfo.pet_pEntity->GetModelInstance(),nItem_Index);
			_pNetwork->_WildPetInfo.m_nPetWearIndex[wear_pos] = -1;
			_pUIMgr->GetWildPetInfo()->PetWearItemReSet();

			strmassage.PrintF(_S(4213, "%s 의 %s 아이템이 기간 만료로 사라집니다." ),strPetname,_pNetwork->GetItemName(nItem_Index));

			_pUIMgr->GetChatting()->AddSysMessage( strmassage, SYSMSG_NORMAL );

		}break;
	case MSG_SUB_APET_INFO:
		{
			INDEX nPetIndex;
			INDEX nlevel,nstr,ncon,ndex,nint;
			CTString petname;
			sPetItem_Info temPetInfo;
			(*istr) >> nPetIndex;
			(*istr) >> petname >> nlevel;
			(*istr) >> nstr >> ncon >> ndex >> nint;

			temPetInfo.pet_index = nPetIndex;
			temPetInfo.pet_name = petname;
			temPetInfo.pet_level = nlevel;
			temPetInfo.pet_str = nstr;
			temPetInfo.pet_con = ncon;
			temPetInfo.pet_dex = ndex;
			temPetInfo.pet_int = nint;

			_pUIMgr->GetWildPetInfo()->AddWildPetInfo(temPetInfo);

		}break;
	case MSG_SUB_ADDITEM_MSG:
		{
			_pUIMgr->GetWildPetInfo()->ReceiveWearItemData(istr);	
		}break;

	case MSG_SUB_AI_LIST_NTF:
		{
			INDEX nPetAICount;
			INDEX nAIMaxCount;
			INDEX nSlotnum;
			UBYTE bActive;
			sWildPet_AI_Slot temAI;

			_pUIMgr->GetWildPetInfo()->AIClear();

			(*istr) >> nAIMaxCount;

			_pUIMgr->GetWildPetInfo()->SetPetAISlotCount(nAIMaxCount);

			(*istr) >> nPetAICount;

			for (int ncnt = 0; ncnt < nPetAICount; ncnt++)
			{
				(*istr) >> nSlotnum;
				(*istr)	>> temAI.m_nTargetSlot;
				(*istr)	>> temAI.m_nActionSlot;
				(*istr)	>> temAI.m_nSillType;
				(*istr)	>> bActive;

				temAI.m_bActive = bActive;

				_pUIMgr->GetWildPetInfo()->AddAIData(nSlotnum, temAI);

			}

			_pUIMgr->GetWildPetInfo()->PetAIReSet();
			_pUIMgr->GetWildPetInfo()->AIPopupClose();


		}break;
	case MSG_SUB_AI_UPDATE_REP:
		{
			UBYTE ubErrorcode;
			(*istr) >> ubErrorcode;

			_pUIMgr->GetWildPetInfo()->ErrorMassage(ubErrorcode);

		}break;
	case MSG_SUB_AI_ONOFF_REP:
		{
			UBYTE bActive;
			UBYTE ubErrorCode;

			(*istr) >> bActive;
			(*istr) >> ubErrorCode;

			_pUIMgr->GetWildPetInfo()->SetPetAIActive(bActive);
			_pUIMgr->GetWildPetInfo()->ErrorMassage(ubErrorCode);			

		}break;


	}

}

// ----------------------------------------------------------------------------
// Name : ReceivePetStatusMessage()
// Desc : 애완동물 상태.
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetStatusMessage( CNetworkMessage *istr )
{
	// 애완동물 상태	: index(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n) x(f) z(f) h(f) r(f) y(c) mapatt(uc)
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
	SBYTE		sbMapAttribute;
	LONG		remainRebirth;
	CTString	strNameCard ="";

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

#ifdef PET_NAMECARD
	(*istr) >> strNameCard;
#endif

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
	
	// 펫 정보를 못 찾으면 추가함.
	if( iter == _pNetwork->m_vectorPetList.end() )
	{
		_pNetwork->m_vectorPetList.push_back(TempPet);
	}	
	// 펫 정보를 갱신함.
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
	_pUIMgr->GetPetInfo()->GetPetDesc();
	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetMountMessage()
// Desc : 애완동물 타기.
// ----------------------------------------------------------------------------
// FIXME : ReceivePetAppearMessage, ReceiveStatusMessage, ReceivePetMountMessage 중복되는 코드 많음 정리 필요.
void CSessionState::ReceivePetMountMessage( CNetworkMessage *istr )
{
	// 애완동물에 타기	: owner_index(n) mountPetTypeGrade(c)
	LONG	lOwnerIndex;
	SBYTE	sbMountPetTypeGrade;

	(*istr) >> lOwnerIndex;
	(*istr) >> sbMountPetTypeGrade;

	// [070824: Su-won] PET_COLOR_CHANGE
	SBYTE	sbPetColor =0;
#ifdef PET_COLOR_CHANGE
	(*istr) >> sbPetColor;
#endif
	//sbPetColor =3;


	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbMountPetTypeGrade, iPetType, iPetAge );
	
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
	
	// [070824: Su-won] PET_COLOR_CHANGE
	INDEX iPetColoredType = iPetType | (sbPetColor<<8);
	
	// 내 펫인경우...
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{			
		// 펫을 타도록 설정해야 하는 경우...
		if( iPetType != -1 && 
			iPetAge != -1 && 
			bPetRide )
		{
			
			if( _pNetwork->pMyCurrentWearing[WEAR_PET] )
			{
				const INDEX iPetIndex = _pNetwork->pMyCurrentWearing[WEAR_PET]->Item_Plus;
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= iPetIndex;
				
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					_pNetwork->_PetTargetInfo.iLevel		= (*iter).lLevel;
					_pNetwork->_PetTargetInfo.fHealth		= (*iter).lHP;
					_pNetwork->_PetTargetInfo.fMaxHealth	= (*iter).lMaxHP;
					_pNetwork->_PetTargetInfo.fMaxHungry	= (*iter).lMaxHungry;
					_pNetwork->_PetTargetInfo.fHungry		= (*iter).lHungry;
					_pNetwork->_PetTargetInfo.lAbility		= (*iter).lAbility;
					_pNetwork->_PetTargetInfo.bIsActive		= TRUE;				
					_pNetwork->_PetTargetInfo.iType			= iPetType;
					_pNetwork->_PetTargetInfo.iAge			= iPetAge;
					_pNetwork->_PetTargetInfo.lIndex		= iPetIndex;
					_pNetwork->_PetTargetInfo.strNameCard	= (*iter).strNameCard;
					
					_pUIMgr->GetPetInfo()->GetPetDesc();
				}
			}

			// 기존의 정보랑 비교를 해서, 펫을 제거해야 하는지? 아니면 생성해야 하는지 판단...
			_pNetwork->RidePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0), NULL, iPetColoredType );
		}
		// 펫에서 내리도록 설정해야 하는 경우...
		else
		{
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
		}
		//		_pNetwork->MyCharacterInfo.bPetRide = bPetRide;		
	}
	// 타 캐릭터의 펫인 경우...
	else
	{
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ipl++ )
		{
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
			if( ct.cha_Index == lOwnerIndex )
			{
				ct.cha_iPetType = iPetType;
				ct.cha_iPetAge	= iPetAge;
				if( ct.cha_bPetRide != bPetRide )
				{
					// 펫을 타도록 설정해야 하는 경우...
					if( iPetType != -1 && 
						iPetAge != -1 && 
						bPetRide )
					{	
						_pNetwork->RidePet( ct.cha_pEntity, NULL, iPetColoredType );
					}
					// 펫에서 내리도록 설정해야 하는 경우...
					else
					{
						_pNetwork->LeavePet( ct.cha_pEntity );
					}
					ct.cha_bPetRide = bPetRide;
				}
				break;
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
	
	if( nErrorcode == MSG_EX_PET_LEARN_ERROR_OK || nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO  )
	{	
		_pUIMgr->GetPetTraining()->LearnSkill( nSkillIndex, nSkillLevel );
	}
	else if( nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO ) // 다종 스킬 습득 
	{
		_pUIMgr->GetPetTraining()->LearnSkill( nSkillIndex, nSkillLevel, TRUE );
	}
	else 
	{
		_pUIMgr->GetPetTraining()->LearnSkillError( nErrorcode );
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

	(*istr) >> nPetIndex;
	(*istr) >> nCount;

	_pUIMgr->GetPetInfo()->ClearSkills( nPetIndex );
	
	for( int i = 0; i < nCount; i++ )
	{
		(*istr) >> nSkillIndex;
		(*istr) >> nSkillLevel;
		
		_pUIMgr->GetPetInfo()->AddSkill( nPetIndex, nSkillIndex, nSkillLevel );
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePetResetSkillMessage()
// Desc : 현재 착용하고 있는 펫의 스킬 초기화
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetResetSkillMessage( CNetworkMessage *istr )
{
	_pUIMgr->GetPetInfo()->ClearSkills( _pNetwork->_PetTargetInfo.lIndex );
	
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	strMessage.PrintF( _S(2565,"애완동물 스킬 초기화가 완료 되었습니다." ) );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
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

#ifdef PET_NAMECARD
	(*istr) >> temp.strNameCard;
#endif

	_pUIMgr->GetPetInfo()->AddPetExchangeInfo( temp );
	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetChangeMountMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetChangeMountMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	_pUIMgr->GetPetInfo()->PetExchangeMount( lResult );
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
#ifdef PET_COLOR_CHANGE
	(*istr) >> sbPetColor;
#endif

	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );

	for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_actPet.Count(); ipl++) 
	{
		CPetTarget &pt = _pNetwork->ga_srvServer.srv_actPet[ipl];					
		
		if (pt.pet_Index == lPetIndex )
		{
			if( pt.pet_Index == _pNetwork->_PetTargetInfo.lIndex )
			{
				_pNetwork->ClientSystemMessage( _S(2253, "펫이 레벨업 하였습니다." ) );
			}

			// 펫의 나이가 변했을때...
			if( pt.pet_iAge != iPetAge )
			{
				CEntity	*penEntity = NULL;
				if( _pNetwork->ga_World.EntityExists( pt.pet_iClientIndex, penEntity ) ) 
				{
					pt.pet_iType	= iPetType;
					pt.pet_iAge		= iPetAge;

					// Date : 2005-11-08(오후 4:04:21), By Lee Ki-hwan
					// _s 로 처리가 안되나요?ㅋㅋ 
					CTString strPetName = pt.pet_OwnerName + _S( 2228, "의" ) + PetInfo().GetName(iPetType, iPetAge);		
					pt.pet_Name		= strPetName;
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
			break;
		}
	}

	CTString strEffect;
	strEffect = "pet_levelup";
	
	if( iPetAge >= 2 )
		strEffect = "pet_levelup_mount";
	
	// 펫 모양 변경 ...?? 
	PetStartEffectGroup( strEffect, lPetIndex );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemMixMessage()
// Desc : 유니크 아이템 조합 eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemMixMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	_pUIMgr->GetPetItemMix()->PetItemMixRep( lResult );	
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemChangeMessage()
// Desc : 펫 유니크 재료 아이템으로 교환 eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemChangeMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	_pUIMgr->GetPetTraining()->PetChangeItemError( lResult );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetRebirthMessage()
// Desc : eons( 봉인 해제 )
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetRebirthMessage( CNetworkMessage *istr )
{
	LONG	lPetIndex;
	LONG	lResult;
	(*istr) >> lPetIndex;
	(*istr) >> lResult;

	_pUIMgr->GetPetFree()->PetFreeError( lPetIndex, lResult );
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
	switch(subType)
	{

		case MSG_EX_PARTY_RECALL_NOTICE :
			_pUIMgr->CloseMessageBox(MSGCMD_NULL);
			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(2516, "파티원에게 소환 메시지를 보냈습니다." ) );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			break;
		case  MSG_EX_PARTY_RECALL_PROMPT :
			{
				CTString tv_str;
				(*istr) >> _pNetwork->m_tmp_idx;
				(*istr) >> _pNetwork->m_tmp_str;
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_EX_PARTY_RECALL_PROMPT)) return ;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_PARTY_RECALL_PROMPT );
				tv_str.PrintF(_S(2517,"%s파티원님께서 소환메시지를 보내 왔습니다. 소환에 응하시겠습니까?"),_pNetwork->m_tmp_str.str_String);
				MsgBoxInfo.AddString(tv_str);
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
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
			   		_pUIMgr->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2518, "%s파티원님이 소환을 거절하였습니다."), strTgt_char);
					MsgBoxInfo.AddString(tv_str);
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
			   }
			   else {
				   _pUIMgr->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2519, "%s파티원님의 소환을 거절하였습니다."), strReq_char);
					MsgBoxInfo.AddString( tv_str );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );				   
			   }
			}
			break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveElementalStatusMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveElementalStatusMessage( CNetworkMessage *istr )
{
	// MSG_EX_ELEMENTAL_STATUS,	// 소환수 정보		: index(n) elementaltype(c) remain(n) hp(n) maxhp(n) attackspeed(c) magicspeed(c) 
								// skillspeed(n) walkspeed(f) runspeed(f) attackrange(f) x(f) z(f) h(f) r(f) ylayer(c) mapattr(uc) assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count	
	LONG	lIndex;
	SBYTE	sbElementalType;
	LONG	lRemain;
	LONG	lHP;
	LONG	lMaxHP;
	SBYTE	sbAttackSpeed;
	SBYTE	sbMagicSpeed;
	LONG	lSkillSpeed;
	FLOAT	fWalkSpeed;
	FLOAT	fRunSpeed;	
	FLOAT	fAttackRange;	
	FLOAT	fX;
	FLOAT	fZ;
	FLOAT	fH;
	FLOAT	fR;
	SBYTE	sbYLayer;
	SBYTE	sbMapAttr;
	SBYTE	assist_count;
	SLONG	assist_state;

	(*istr) >> lIndex;
	(*istr) >> sbElementalType;
	(*istr) >> lRemain;
	(*istr) >> lHP;
	(*istr) >> lMaxHP;
	(*istr) >> sbAttackSpeed;
	(*istr) >> sbMagicSpeed;
	(*istr) >> lSkillSpeed;
	(*istr) >> fWalkSpeed;
	(*istr) >> fRunSpeed;	
	(*istr) >> fAttackRange;	
	(*istr) >> fX;
	(*istr) >> fZ;
	(*istr) >> fH;
	(*istr) >> fR;
	(*istr) >> sbYLayer;
	(*istr) >> sbMapAttr;
	(*istr) >> assist_state >> assist_count; 

	for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actSlave.Count(); ipl++ )
	{
		CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[ipl];
		
		if( st.slave_Index == lIndex )
		{
			if( st.slave_pEntity )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalData(st.slave_pEntity, lHP, lMaxHP);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalStatus(
					st.slave_pEntity, sbAttackSpeed, sbMagicSpeed, lSkillSpeed, fWalkSpeed, fRunSpeed, fAttackRange);
			}
			
			st.slave_yLayer = sbYLayer;

			// 내 소환수인 경우...
			if( st.slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
			{
				for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
				{
					CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
					if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == lIndex )
					{
						pUISummon->SetMaxTime(lRemain);		// MaxTime은 한번만 설정됩니다.
						_pNetwork->_SlaveTargetInfo[i - UI_SUMMON_START].fHealth	= lHP;
						_pNetwork->_SlaveTargetInfo[i - UI_SUMMON_START].fMaxHealth = lMaxHP;
						pUISummon->SetLeftTime(lRemain);
					}
				}
			}
			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();	
			
			st.slave_BuffCount = 0;
			for( int i = 0; i < assist_count; i++ )
			{
				(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;
				st.AddBuff( sBuff );
			}
		}

	
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveElementalDeleteMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveElementalDeleteMessage( CNetworkMessage *istr )
{
	// MSG_EX_ELEMENTAL_DELETE,	// 소환수 제거		: index(n)
	LONG lIndex;
	(*istr) >> lIndex;
	
	for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actSlave.Count(); ipl++ )
	{
		CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[ipl];
		
		if( st.slave_Index == lIndex )
		{
			// 내 소환수인 경우...
			if( st.slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
			{
				for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
				{
					CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
					if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == lIndex )
					{
						pUISummon->ResetSummon();						
						break;
					}
				}
				//_pNetwork->_PetTargetInfo.Init();
			}

			SlaveInfo().StopIdleEffect( lIndex );	
			
#ifdef TARGET_MARK			
			//해당 소환수의 타겟 이펙트를 없앰...
			_pUIMgr->StopTargetEffect( lIndex );
#endif

			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(st.slave_pEntity);
			
			st.Init();
			_pNetwork->ga_srvServer.srv_actSlave.SwapAndPop(ipl);
			break;
		}
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
	SBYTE	sbType;
	(*istr) >> lCharIndex;
	(*istr) >> sbType;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->EvocationStart( lCharIndex, sbType - 1);	
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