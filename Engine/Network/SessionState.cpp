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

#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include <Engine/Petinfo.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIPetTarget.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UISelectWord.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UISingleBattle.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIGWMix.h>
#include <Engine/Interface/UINotice.h>
#include <Engine/Interface/UIQuestBook.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UISelChar.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/LocalDefine.h>

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
	#include <NPGameLib.h>
#endif
// ---------------------------------------------------<<


#define SESSIONSTATEVERSION_OLD 1
#define SESSIONSTATEVERSION_WITHBULLETTIME 2
#define SESSIONSTATEVERSION_CURRENT SESSIONSTATEVERSION_WITHBULLETTIME

//---------------------------------------------------------------------

#define MODEL_MONEY		("Data\\Item\\Common\\item_money_01.smc")
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")
#define MODEL_GIFT		("Data\\Item\\Common\\item_gift.smc")


#define MODEL_DROPIEM_TI_BU		("Data\\Item\\Common\\ti_bu_drop.smc")
#define MODEL_DROPIEM_TI_BD		("Data\\Item\\Common\\ti_bd_dorp.smc")
#define MODEL_DROPIEM_TI_FT		("Data\\Item\\Common\\ti_ft_drop.smc")
#define MODEL_DROPIEM_TI_HN		("Data\\Item\\Common\\ti_hn_drop.smc")

#define MODEL_DROPIEM_NI_BU		("Data\\Item\\Common\\ni_bu_drop.smc")
#define MODEL_DROPIEM_NI_BD		("Data\\Item\\Common\\ni_bd_drop.smc")
#define MODEL_DROPIEM_NI_FT		("Data\\Item\\Common\\ni_ft_drop.smc")
#define MODEL_DROPIEM_NI_HN		("Data\\Item\\Common\\ni_hn_drop.smc")

#define MODEL_DROPIEM_HW_BU		("Data\\Item\\Common\\hw_bu_drop.smc")
#define MODEL_DROPIEM_HW_BD		("Data\\Item\\Common\\hw_bd_drop.smc")
#define MODEL_DROPIEM_HW_FT		("Data\\Item\\Common\\hw_ft_drop.smc")
#define MODEL_DROPIEM_HW_HN		("Data\\Item\\Common\\hw_hn_drop.smc")

#define MODEL_DROPIEM_MA_BU		("Data\\Item\\Common\\ma_bu_drop.smc")
#define MODEL_DROPIEM_MA_BD		("Data\\Item\\Common\\ma_bd_drop.smc")
#define MODEL_DROPIEM_MA_FT		("Data\\Item\\Common\\ma_ft_drop.smc")
#define MODEL_DROPIEM_MA_HN		("Data\\Item\\Common\\ma_hn_drop.smc")

#define MODEL_DROPIEM_RO_BU		("Data\\Item\\Common\\ro_bu_drop.smc")
#define MODEL_DROPIEM_RO_BD		("Data\\Item\\Common\\ro_bd_drop.smc")
#define MODEL_DROPIEM_RO_FT		("Data\\Item\\Common\\ro_ft_drop.smc")
#define MODEL_DROPIEM_RO_HN		("Data\\Item\\Common\\ro_hn_drop.smc")

#define MODEL_DROPIEM_SO_BU		("Data\\Item\\Common\\so_bu_drop.smc")
#define MODEL_DROPIEM_SO_BD		("Data\\Item\\Common\\so_bd_drop.smc")
#define MODEL_DROPIEM_SO_FT		("Data\\Item\\Common\\so_ft_drop.smc")
#define MODEL_DROPIEM_SO_HN		("Data\\Item\\Common\\so_hn_drop.smc")

#define MODEL_DROPIEM_HW_BOW		("Data\\Item\\Common\\hw_bow_drop.smc")
#define MODEL_DROPIEM_HW_STAFF		("Data\\Item\\Common\\hw_staff_drop.smc")
#define MODEL_DROPIEM_MA_WAND		("Data\\Item\\Common\\ma_wand_drop.smc")
#define MODEL_DROPIEM_MA_STAFF		("Data\\Item\\Common\\ma_staff_drop.smc")
#define MODEL_DROPIEM_NI_SHIELD		("Data\\Item\\Common\\ni_shield_drop.smc")
#define MODEL_DROPIEM_NI_SWORD		("Data\\Item\\Common\\ni_sword_drop.smc")
#define MODEL_DROPIEM_RO_CBOW		("Data\\Item\\Common\\ro_crossbow_drop.smc")
#define MODEL_DROPIEM_RO_DAGGER		("Data\\Item\\Common\\ro_dagger_drop.smc")
#define MODEL_DROPIEM_TI_GSWORD		("Data\\Item\\Common\\ti_gsword_drop.smc")
#define MODEL_DROPIEM_TI_AXE		("Data\\Item\\Common\\ti_axe_drop.smc")
#define MODEL_DROPIEM_SO_SCYTHE		("Data\\Item\\Common\\so_scythe_001_drop.smc")
#define MODEL_DROPIEM_SO_STAFF		("Data\\Item\\Common\\so_staff_001_drop.smc")

//---------------------------------------------------------------------

// ------------------------------------------------------------------->>
// wooss 060529
//#define SUMMON_SIZE

//--------------------------------------------------------------------<<

// Check Attack Flag
#define ENF_EX2_PVP				(1L<<0)
#define ENF_EX2_LEGIT			(1L<<1)
#define ENF_EX2_MYPARTY			(1L<<2)
#define ENF_EX2_MYGUILD			(1L<<3)
#define ENF_EX2_ENEMYGUILD		(1L<<4)
#define ENF_EX2_WAR_OURFORCE	(1L<<5)
#define ENF_EX2_WAR_ENEMY		(1L<<6)
#define ENF_EX2_SAFEGUARD		(1L<<7)
//#define MOB_MOVE	1
//#define MOB_ATTACK	2
//#define MOB_DIE		3
//#define MOB_STATUS  4
//#define PLAYER_MOVE 11
//#define PLAYER_ATTACK 12
//#define PLAYER_STOP 13
//#define PLAYER_ROTATE 14
//#define DEBUG_LERPING 1

enum e24Event
{
	EXCH_FIVE,						// 맞고 머니 교환
	EXCH_FOUR,						// 포커 머니 교환 
	OPEN_TREA,						// 보물 상자 열기
};

// 보물 상자에서 지급하는 아이템 갯수
#define TREASUREBOX_ITEM_COUNT	3

#define	ISNPCLIVE	1
//#define TEST_SERVER

extern INDEX net_bLerping;
extern FLOAT net_tmConnectionTimeout;
extern FLOAT net_tmProblemsTimeOut;
extern FLOAT net_tmDisconnectTimeout;
extern INDEX net_bReportClientTraffic;

extern CClientInterface cm_ciLocalClient;
extern TIME _tmLocalTick = -1.0f;
extern INDEX g_iCountry;

FLOAT3D _vBasePosition;
BOOL    _bBasePositionSet;

#if DEBUG_LERPING

FLOAT avfStats[1000][4];
INDEX ctCounter=0;
INDEX ctMax = sizeof(avfStats)/sizeof(avfStats[0]);

#endif // DEBUG_LERPING

// 플레이 타임 연산 관련 /////////////////////////////////////////////////
DWORD	_SetPlayStartTime = 0;
const DWORD	_IntervalPlayCountTime = 1000 * 60 * 10; // 10분간격
//////////////////////////////////////////////////////////////////////////
BOOL bShowMessage = TRUE;
BOOL bCalcNotChecked = TRUE;


// WSS_GUILDMASTER 070416 ------------------------------------------>>
int GuildMasterErrChk(SBYTE errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
		case MSG_ERROR_PARENTSDAY_2007_FULLINVEN:
			strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
			break;
		case MSG_ERROR_PARENTSDAY_2007_NO_EXCHAGNE_ITEM:
			strMessage =_S( 2236, "이벤트 아이템이 존재하지 않습니다.");
			break;		
		case MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER:
			strMessage =_S(3470,"길드 마스터가 아닙니다.");
			break;
		case MSG_ERROR_ACCUMULATEPOINT_NOT_RANKING:
			strMessage =_S(3471, "랭킹 데이터가 없습니다.");
			break;

		case MSG_ERROR_ADD_POINT:					
			strMessage =_S(3472,"포인트를 적립하지 못했습니다.");
			break;
		case MSG_ERROR_NOT_ENOUGH_POINT:
			strMessage =_S(3473, "포인트가 부족 합니다.");
			break;
		case MSG_ERROR_NOT_FOUND_ACCUMULATEPOINT_LIST:
			strMessage =_S(3474, "누적 포인트 정보가 없습니다.");
			break;
		case MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM :
			strMessage =_S(3475,"교환권이 존재하지 않습니다.");
			break;
		case MSG_PARENTSDAY_2007_SUCCESS:
			return TRUE;
		default :
			nResult = TRUE;
			break;
	}
	if(strMessage.str_String!=NULL)
		_pNetwork->ClientSystemMessage(strMessage,SYSMSG_ERROR);

	return nResult;

}

// -----------------------------------------------------------------<<

// wooss 070307 ---------------------------------------------------->>
// kw WSS_WHITEDAY_2007 
int Whiteday2007ErrChk(ULONG errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
		case MSG_EVENT_WHITEDAY_2007_EXCHANGE_OK:
		//  성공시 메시지 출력이 필요 없을거 같음...
		//	strMessage =_s( "요청한 작업이 완료 되었습니다.");
			nResult = TRUE;
			break;
		case MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED:
			strMessage =_S( 3257, "요청한 작업이 실패 하였습니다.");
			break;
		case MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM:
			strMessage =_S( 3258, "보상을 받기 위한 사탕의 개수가 부족합니다.");
			break;		
		case MSG_EVENT_WHITEDAY_2007_EXCHANGE_FULLINVEN:
			strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
			break;
		case MSG_EVENT_WHITEDAY_2007_EXCHANGE_MAXITEM:
			strMessage =_S( 3260, "편지지는 최대 5개까지 소유할 수 있습니다.");
			break;
	}
	
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;

}

// [KH_070326] 부활절 이벤트 관련
int EasterEggs2007ErrChk(ULONG errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_EVENT_EGGS_HUNT_2007_GIFT_REP_OK:
		nResult = TRUE;
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST:
		strMessage =_S( 3265, "1개 이상의 부활절 달걀을 소유할 수 없습니다.");
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_GIFT_FULLINVEN:
		strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_NOITEM:
		strMessage =_S( 3257, "요청한 작업이 실패 하였습니다.");
		break;
	}
	
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;

}

// -----------------------------------------------------------------<<

int LeaseWeaponErrChk(ULONG errType) 		
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
		case MSG_ITEM_LEND_WEAPON_ERROR_OK:
			strMessage =_S(3082, "선택한 장비가 대여 되었습니다.");
			nResult = TRUE;
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_FULLINVEN:
			strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_NOMONEY:
			strMessage =_S( 306,"나스가 부족합니다.");
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_FAIL:
			strMessage =_S( 16,"알수 없는 오류가 발생했습니다.");
			break;
		case MSG_ITEM_LEND_TIMEOUT:
			strMessage =_S( 3064,"대여 기간이 종료되었습니다.");
			break;
	}
	
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;
}

//wooss 060601 inner Function
// worldcup event
int WorldCupErrChk(ULONG errType) 		
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_OK :			// 승리 국가 응모 : 성공
			strMessage =_S( 2835, "우승컵이 선택한 국가의 국기로 교환되었습니다.");
			nResult = TRUE;
			break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM :		// 승리 국가 응모 : FIFA 컵 부족
			strMessage =_S( 2836, "우승컵이 부족합니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_NOTCHANGE :	// 승리 국가 응모 : 응모 기간 아님
			strMessage =_S( 2837, "우승컵 교환기간이 아닙니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_FULLINVEN :	// 승리 국가 응모 : 인벤 공간 부족
			strMessage =_S( 2838, "승리 국가 응모 : 인벤 공간이 부족합니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_ALREADY :		// 승리 국가 응모 : 이미 응모한 국가
			strMessage =_S( 2839, "승리 국가 응모 당첨 확인 : 이미 응모한 국가입니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_OK :			// 승리 국가 응모 당첨 확인 : 지급 성공
			strMessage =_S( 2840, "우승국가 맞추기 이벤트 보상품이 모두 지급되었습니다. 참여해 주셔서 감사합니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_NOTCHANGE :	// 승리 국가 응모 당첨 확인 : 교환 기간 아님
			strMessage =_S( 2841, "보상기간이 아닙니다. 월드컵이 종료된 이후에 확인하시기 바랍니다.");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM :		// 승리 국가 응모 당첨 확인 : 국기 아이템 없음
			strMessage =_S( 2842, "해당 순위의 국기를 가지고 있지 않습니다. 다시 확인해 주세요");
			break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_FULLINVEN :	// 승리 국가 응모 당첨 확인 : 인벤 공간 부족
			strMessage =_S( 2843, "승리 국가 응모 당첨 확인 : 인벤공간이 부족합니다.");
			break;
	
	}
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;
}
	
//------------------------------------------------------------------------------
// CSlaveInfo::GetOwnner()
// Explain:  
// Date : wooss 060529
//------------------------------------------------------------------------------

CCharacterTarget GetOwnner(int ownIdx)
{
	INDEX cnt = _pNetwork->ga_srvServer.srv_actCha.Count();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(_pNetwork->ga_srvServer.srv_actCha[i].cha_Index == ownIdx)
		{
			if(_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity != NULL
				&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance != NULL
				&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance->GetName() != "")
			{
				
				return _pNetwork->ga_srvServer.srv_actCha[i];
			}
		}
	}
	CCharacterTarget tv_target;
	return tv_target;
	
}

//wooss 060201 inner Function
void FindFriendErrChk1(ULONG errType) 		
{
	CTString strMessage;
	
	switch(errType)
	{
		case MSG_EVENT_SEARCHFRIEND_ERROR_OK :				 //정상.(친구등록OK, 삭제 OK)
			strMessage=_S( 2495, "친구등록에 성공하였습니다.");
			_pUIMgr->GetSelectList()->ResetSelectList();
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_PACKET :			 //패킷오류
			strMessage =_S(854,"패킷 오류가 발생하였습니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_NOT_EXIST :			 //존재하지 않는다.
			strMessage =_S(11, "존재하지 않는 캐릭터 입니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_FULLMEMBER :		 //친구 정원 초과.
			strMessage =_S( 2496, "등록인원이 초과 되었습니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_ALREADY_EXIST :		 //이미 친구리스트에 존재.
			strMessage =_S( 2497, "이미 등록된 친구입니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_NOT_MATCH_INDEX :	 //서로간  요청,승인 아이디가 일치하면 안됨.
			strMessage = _S( 2498, "같은 계정의 캐릭터는 등록할 수 없습니다.  ");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_HELPER_SERVER :		 //헬퍼서버와의 접속오류.
			strMessage = _S(2133,"헬퍼서버 오류 입니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER :		 //기간이 지났음.
			strMessage = _S( 2499, "한달 이상 게임에 접속을 안 한 캐릭터만 등록이 가능합니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME :		 //이름이 잘못되었다.
			strMessage = _S( 2500, "잘못된 이름입니다.");
			break;							
	}
	_pNetwork->ClientSystemMessage(strMessage);
}

void FindFriendErrChk2(ULONG errType) 		
{
	CTString strMessage;
	
	switch(errType)
	{
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK :				// 지급 성공
			{
				_pUIMgr->CloseMessageBox( MSGCMD_NULL );
				CTString strSysMessage;
				strSysMessage = _S(1840,"아이템이 성공적으로 지급되었습니다.");
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S( 169, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				
				strMessage = CTString("");
				_pUIMgr->GetSelectList()->ResetSelectList();
			}
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TIME :			// 지급 실패 : 시간 부족
			strMessage = _S( 2501, "시간이 부족합니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_END :				// 지급 실패 : 기간 아님
			strMessage = _S(945,"지금은 이벤트 기간이 아닙니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_SOMEREMAIN :		// 지급 성공, 일부 아이템 미지급
			strMessage = _S(2235,"이벤트 아이템을 일부 지급 받았습니다.");
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOGOODS :			// 지급 실패, 받을 아이템 없음
			strMessage = _S( 2502,"지급 받을 아이템이 없습니다." );
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOSPACE :			// 지급 실패 : 아이템 칸이 모자름.
			strMessage = _S(116,"인벤토리 공간이 부족하여 보상을 받을 수 없습니다.");
			break;
	}
	_pNetwork->ClientSystemMessage(strMessage);
}

//wooss 050808 inner function
void ConfirmMsgProc(UBYTE subType,UBYTE errorType = NULL,CTString charName = CTString("PLAYER"))
{
	CTString		strTitle= _S(191,"확인");
	CTString		strMsg;
	CUIMsgBox_Info	msgInfo;	
	switch(subType)
	{
	
		case MSG_WARP_TO_REQ:
			strMsg.PrintF(_S( 2124, "%s님께서 당신에게 순간이동을 하려고 합니다. 승인 하시겠습니까?" ),charName); 
			msgInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_WARP_TO_REQ_CALL);
			msgInfo.AddString(strMsg);
			_pUIMgr->CreateMessageBox(msgInfo);
			_pUIMgr->GetMessageBox(MSGCMD_WARP_TO_REQ_CALL)->GetInputBox().SetString(charName.str_String);
			

			break;
		
		case MSG_WARP_TAKE_REQ:
			strMsg.PrintF(_S( 2125, "%s님께서 당신의 소환을 요청하셨습니다. 승인 하시겠습니까?" ),charName); 
			msgInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_WARP_TAKE_REQ_CALL);
			msgInfo.AddString(strMsg);
			_pUIMgr->CreateMessageBox(msgInfo);
			_pUIMgr->GetMessageBox(MSGCMD_WARP_TAKE_REQ_CALL)->GetInputBox().SetString(charName.str_String);
			

			break;

		
		case MSG_WARP_ERROR:
			
			switch(errorType)
			{
				case MSG_WARP_ERROR_NOTALLOW:
					strMsg.PrintF(_S( 2126, "%s님께서 이동 요청을 거절하였습니다. 순간 이동이 취소됩니다." ),charName); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					_pUIMgr->CreateMessageBox(msgInfo);
					_pNetwork->SendWarpCancel();
					break;

				case MSG_WARP_ERROR_NOTALLOWZONE:
					strMsg =_S( 2127,  "순간 이동 가능 지역이 아닙니다.(다른 존이거나 퍼스널던전에서는 사용할 수 없습니다.)" ); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					_pUIMgr->CreateMessageBox(msgInfo);
					break;

				case MSG_WARP_ERROR_NOTCHAR:
					strMsg = _S(1628,"해당 케릭터가 존재 하지 않거나 [오프라인] 상태입니다."); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					_pUIMgr->CreateMessageBox(msgInfo);
					break;

				case MSG_WARP_ERROR_CANCEL:
					if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TO_REQ_CALL)) 
						_pUIMgr->CloseMessageBox(MSGCMD_WARP_TO_REQ_CALL);
					if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TAKE_REQ_CALL)) 		
						_pUIMgr->CloseMessageBox(MSGCMD_WARP_TAKE_REQ_CALL);
					_pUIMgr->GetChatting()->AddSysMessage( _S( 340, "이동이 취소되었습니다." ) );
					break;
			}

			break;

	}
}

// FIXME : 드롭될 SMC 명이 하드 코딩되어있는데...
ENGINE_API void SetDropItemModel(CEntity *penEntity, CItemData& ItemData, CItemTarget &it)
{
	penEntity->InitAsSkaModel();
	char *szEffectName = NULL;
	INDEX animID = -1;

	if(ItemData.GetType() == CItemData::ITEM_ETC)
	{
		if(ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY)			// 돈이라면,//1018
		{
			penEntity->SetSkaModel(MODEL_MONEY);
		}
		else if(ItemData.GetSubType() == CItemData::ITEM_ETC_EVENT)		// 이벤트 아이템이라면,
		{
			if(ItemData.GetItemIndex() == 1393){
				penEntity->SetSkaModel(ItemData.GetItemSmcFileName());
				szEffectName = "server_event_drop";				
			}
			else penEntity->SetSkaModel(MODEL_GIFT);
		}	
		else
		{

			if(ItemData.GetItemIndex() == 723)//문스톤
			{
				penEntity->SetSkaModel("data\\item\\common\\item_moonstone.smc");
				animID = penEntity->GetModelInstance()->FindFirstAnimationID();
				szEffectName = "Item MoonStone";
			}
			else if (ItemData.GetItemIndex() == 2844) // 카오스 제련석
			{
				penEntity->SetSkaModel("data\\item\\common\\item_goje_twn.smc");
				szEffectName = "Item GOJE twn";
			}
			else if(ItemData.GetItemIndex() == 85)//고급제련석
			{
				/*if(g_iCountry == TAIWAN || g_iCountry == TAIWAN2 ||)
				{
					penEntity->SetSkaModel("data\\item\\common\\item_goje_twn.smc");
					szEffectName = "Item GOJE twn";
				}
				else {*/
				penEntity->SetSkaModel("data\\item\\common\\item_goje.smc");
				szEffectName = "Item GOJE";
				//}
			}
			else if(ItemData.GetItemIndex() == 779)//초고급제련석
			{
				penEntity->SetSkaModel("Data\\Item\\Common\\item_lightstone.smc");
				szEffectName = "Item SGOJE";
			}
			else if(ItemData.GetItemIndex() == 974)//행운의 제련석 wooss 051026 -> 소스 머지 대만 공개(060320)
			{
				penEntity->SetSkaModel("Data\\Item\\Common\\item_goje_lucky.smc");
				szEffectName = "Item LGOJE";
			}
			// 1482 : 축구공 1484 :우승컵
			else if(ItemData.GetItemIndex() == 1482 || ItemData.GetItemIndex() == 1484 ){
				penEntity->SetSkaModel(ItemData.GetItemSmcFileName());
			}
			else if( ItemData.GetItemIndex() >= 1579 && ItemData.GetItemIndex() <= 1588 )
			{// 곤충 박스
				penEntity->SetSkaModel( ItemData.GetItemSmcFileName() );
			}
			// WSS_MINIGAME 070423 -------------------------------------------->>
			// 2329 아이템부터 smc 파일대로 드랍시킴
			else if( ItemData.GetItemIndex() >= 2329 )
			{
				penEntity->SetSkaModel( ItemData.GetItemSmcFileName() );
			}
			// ----------------------------------------------------------------<<
			else
			{
				penEntity->SetSkaModel(MODEL_TREASURE);
			}
		}
	}
	else if(ItemData.GetType() == CItemData::ITEM_WEAPON)
	{
#ifdef NEW_WEAPONDROP
		if( ItemData.GetSubType() == CItemData::ITEM_WEAPON_TWOSWORD || ItemData.GetSubType() == CItemData::ITEM_WEAPON_DAGGER )
		{
			CTString tStr;
			tStr.PrintF("%s",ItemData.GetItemSmcFileName());
			tStr.ReplaceSubstr(CTString("Weapon"),CTString("Weapon\\Drop"));
			penEntity->SetSkaModel( tStr );
		}
		else 
			penEntity->SetSkaModel( ItemData.GetItemSmcFileName() );
#else
		switch(ItemData.GetSubType()) 
		{
		case CItemData::ITEM_WEAPON_KNIFE :
		case CItemData::ITEM_WEAPON_TWOSWORD :
			penEntity->SetSkaModel(MODEL_DROPIEM_NI_SWORD);
			break;
		case CItemData::ITEM_WEAPON_CROSSBOW :
			penEntity->SetSkaModel(MODEL_DROPIEM_RO_CBOW);
			break;
		case CItemData::ITEM_WEAPON_STAFF :
			penEntity->SetSkaModel(MODEL_DROPIEM_HW_STAFF);
			break;
		case CItemData::ITEM_WEAPON_BIGSWORD :
			penEntity->SetSkaModel(MODEL_DROPIEM_TI_GSWORD);
			break;
		case CItemData::ITEM_WEAPON_AXE	:
			penEntity->SetSkaModel(MODEL_DROPIEM_TI_AXE);
			break;		
		case CItemData::ITEM_WEAPON_WAND:
			//penEntity->SetSkaModel(MODEL_DROPIEM_MA_WAND);
			penEntity->SetSkaModel(MODEL_DROPIEM_MA_STAFF);
			break;
		case CItemData::ITEM_WEAPON_SSTAFF :
			//penEntity->SetSkaModel(MODEL_DROPIEM_MA_STAFF);
			penEntity->SetSkaModel(MODEL_DROPIEM_MA_WAND);
			break;
		case CItemData::ITEM_WEAPON_BOW	:
			penEntity->SetSkaModel(MODEL_DROPIEM_HW_BOW);
			break;
		case CItemData::ITEM_WEAPON_DAGGER :
			penEntity->SetSkaModel(MODEL_DROPIEM_RO_DAGGER);
			break;
		case CItemData::ITEM_WEAPON_SCYTHE:
			penEntity->SetSkaModel(MODEL_DROPIEM_SO_SCYTHE);
			break;
		case CItemData::ITEM_WEAPON_POLEARM	:
			penEntity->SetSkaModel(MODEL_DROPIEM_SO_STAFF);
			break;
		default:
			penEntity->SetSkaModel(MODEL_TREASURE);
			break;
		}
#endif
	}
	else if	(ItemData.GetType() == CItemData::ITEM_SHIELD)
	{
		switch(ItemData.GetSubType()) 
		{
		case CItemData::ITEM_SHIELD_COAT :
			if(ItemData.GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_BU);
			}
			else if(ItemData.GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_BU);
			}
			else if(ItemData.GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_BU);
			}
			else if(ItemData.GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BU);
			}
			else if(ItemData.GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BU);
			}
			else if(ItemData.GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_BU);
			}
			else if(ItemData.GetJob()&(1<<WILDPET_JOB))
			{
				penEntity->SetSkaModel( ItemData.GetItemSmcFileName() );
			}
			break;
		case CItemData::ITEM_SHIELD_PANTS :
			if(ItemData.GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_BD);
			}
			else if(ItemData.GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_BD);
			}
			else if(ItemData.GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_BD);
			}
			else if(ItemData.GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BD);
			}
			else if(ItemData.GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BD);
			}
			else if(ItemData.GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_BD);
			}
			break;
		case CItemData::ITEM_SHIELD_GLOVE :
			if(ItemData.GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_HN);
			}
			else if(ItemData.GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_HN);
			}
			else if(ItemData.GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_HN);
			}
			else if(ItemData.GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_HN);
			}
			else if(ItemData.GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_HN);
			}
			else if(ItemData.GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_HN);
			}
			break;
		case CItemData::ITEM_SHIELD_SHOES :
			if(ItemData.GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_FT);
			}
			else if(ItemData.GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_FT);
			}
			else if(ItemData.GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_FT);
			}
			else if(ItemData.GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_FT);
			}
			else if(ItemData.GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_FT);
			}
			else if(ItemData.GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_FT);
			}
			break;
		case CItemData::ITEM_SHIELD_SHIELD :
			penEntity->SetSkaModel(MODEL_DROPIEM_NI_SHIELD);
			break;
		default:
			penEntity->SetSkaModel(MODEL_TREASURE);
			break;
		}
	}
	else if(ItemData.GetType() == CItemData::ITEM_ONCEUSE)
	{
// [KH_070328] 달걀 이벤트 관련 추가
		if(ItemData.GetItemIndex() == EASTER_EGGS)
		{
			penEntity->SetSkaModel( ItemData.GetItemSmcFileName() );
			szEffectName = "Item LGOJE";
		}
		else
		{
			penEntity->SetSkaModel(MODEL_TREASURE);
		}
	}
	else
	{
		penEntity->SetSkaModel(MODEL_TREASURE);
	}

	penEntity->Initialize();
	penEntity->FallDownToFloor();
	if(penEntity->GetModelInstance())
	{
		if(szEffectName)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			penEntity->GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			it.item_pDropItemEffect = StartEffectGroup(szEffectName
				, &penEntity->GetModelInstance()->m_tmSkaTagManager
				, _pTimer->GetLerpedCurrentTick());
		}
		if(animID != -1)
		{
			penEntity->GetModelInstance()->AddAnimation(
				animID, AN_LOOPING|AN_CLEAR|AN_NORESTART, 1.0f, 0);
		}
	}
}

//0105
//CEntity* m_penEntity;

#define TOTAL_TAB	3
#define TOTAL_ROW	20
#define TOTAL_COL	5

#define ADDSERVERVALUE	1999

#define		ITEM_ARRANGE_START			(1 << 0)	// 아이템 정렬의 시작.
#define		ITEM_ARRANGE_END			(1 << 1)	// 아이템 정렬의 끝.

#define TEACHER_PRIZE_EVENT_2PAN4PAN				// 후견인 명성 보상 이벤트

//#define RESTRICT_SOUND

// get a pseudo-random number (safe for network gaming)
ULONG CSessionState::Rnd(void) {
//  ASSERTMSG(ses_bAllowRandom, "Use RND only in entity AI!");
	// NOTE: 
	// using multiplicative congruent method with Greanberger's lambda = 2^18+3
	ses_ulRandomSeed = ses_ulRandomSeed*262147;
	ASSERT(ses_ulRandomSeed!=0);
	return ses_ulRandomSeed;
}

// reset random number generator (always randomizes to same sequence!)
void CSessionState::ResetRND(void)
{
	BOOL bOldAllow = ses_bAllowRandom;
	ses_bAllowRandom = TRUE;
	// random must start at a number different than zero!
	ses_ulRandomSeed = 0x87654321;
	// run rnd a few times to make it go random
	for(INDEX i=0; i<32; i++) {
		Rnd();
	}
	ses_bAllowRandom = bOldAllow;
}

/*
 * Constructor.
 */
CSessionState::CSessionState(void)
{
	TRACKMEM(Mem,"Network");
	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	ses_bAllowRandom = TRUE;  // random allowed when not in game

	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bGameFinished = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_bWaitAllPlayers = FALSE;
	ses_fRealTimeFactor = 1.0f;
	ses_ubNumLevelChanges = 0;

	// reset random number generator
	ResetRND();

	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;
	ses_bRestartLocalTime = FALSE;

	_tmLocalTick = -1.0f;
//0105
		m_commIn[0] = NULL;
	m_commOut[0] = NULL;
	m_commInTmp[0] = NULL;
	m_pCmd = new CCmd(this);

	m_iRecentServer = -1;
	m_iRecentGroup	= -1;

}

/*
 * Destructor.
 */
CSessionState::~CSessionState()
{
//0105
	delete m_pCmd;
}

/*
 * Clear the object.
 */
void CSessionState::Stop(void)
{

	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	ses_bAllowRandom = TRUE;  // random allowed when not in game
	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bGameFinished = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_fRealTimeFactor = 1.0f;
	ses_bWaitAllPlayers = FALSE;
	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;

	// disable lerping
	_pTimer->DisableLerp();

#if DEBUG_LERPING

	CTFileStream f;
	f.Create_t(CTFILENAME("Temp\\Lerp.stats"), CTStream::CM_TEXT);
	for (INDEX i=0; i<ctCounter; i++) {
		char strBuffer[256];
		sprintf(strBuffer, "%6.2f %6.2f %6.2f %6.2f",
			avfStats[i][0],
			avfStats[i][1],
			avfStats[i][2],
			avfStats[i][3]);
		f.PutLine_t(strBuffer);
	}
	f.Close();

#endif // DEBUG_LERPING
/* //0522 kwon 삭제.
	CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);
	if (_cmiComm.cci_bClientInitialized) {
	  _pNetwork->SendToServerReliable(nmConfirmDisconnect);
	}
*/
	_cmiComm.Client_Close();

	// clear all old levels
	ForgetOldLevels();

	_pNetwork->ga_srvServer.srv_apltPlayers.Clear();
	_pNetwork->ga_srvServer.srv_apltPlayers.New(NET_MAXGAMEPLAYERS);

	_pNetwork->ga_srvServer.ReAllocEntities();	
}

/*
 * Initialize session state and wait for game to be started.
 */
void CSessionState::Start_t(INDEX ctLocalPlayers) 
{
	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	// clear message stream
	ses_nsGameStream.Clear();
	ses_bAllowRandom = FALSE;  // random not allowed in game
	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_bGameFinished = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_fRealTimeFactor = 1.0f;
	ses_bWaitAllPlayers = FALSE;
	ses_iMissingSequence = -1;
	ses_tvMessageReceived.Clear();
	_pNetwork->ga_strRequiredMod = "";
	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;

	// reset random number generator
	ResetRND();

	// clear all old levels
	ForgetOldLevels();

#if DEBUG_LERPING
	// clear lerp stats
	ctCounter = 0;
#endif // DEBUG_LERPING

	// ses_LastProcessedTick and ses_LastReceivedTick tick counters are
	// irrelevant now, will be initialized when initialization message
	// from server is received, no need to set them here

	// if this computer is server
	if (_pNetwork->IsServer()) {
		// initialize local client
		_cmiComm.Client_Init_t(0UL);
		// connect as main session state
		try {
			Start_AtServer_t();
		} catch(char *) {
			_cmiComm.Client_Close();
			throw;
		}

	// if this computer is client
	} else {
		// connect client to server computer
			_cmiComm.Client_Init_t((char*)(const char*)_pNetwork->ga_strServerAddress);
		// connect as remote session state
		try {
			Start_AtClient_t(ctLocalPlayers);
		} catch(char *) {
			// if failed due to wrong mod
			if (strncmp(ses_strDisconnected, "MOD:", 4)==0) {
				// remember the mod
				_pNetwork->ga_strRequiredMod = ses_strDisconnected+4;
				// make sure that the string is never empty
				if (_pNetwork->ga_strRequiredMod=="") {
					_pNetwork->ga_strRequiredMod=" ";
				}
			}
			_cmiComm.Client_Close();
			throw;
		}
	}
	ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
}
//! 서버 스타트.
void CSessionState::Start_AtServer_t(void)     // throw char *
{
	//! 로컬 클라이언트가 접속을 요청한다.
	// send registration request
	CNetworkMessage nmRegisterMainSessionState(MSG_REQ_CONNECTLOCALSESSIONSTATE);
	ses_sspParams.Update();
	nmRegisterMainSessionState<<ses_sspParams;
	//! 이함수 안에서 서버 업데이트를 하지..
	_pNetwork->SendToServerReliable(nmRegisterMainSessionState);

	TIME tmTimeout = net_tmConnectionTimeout;
	if (_pNetwork->IsServer()) {
		tmTimeout = 5.0f;
	}
//! 5초동안 0.05초 간격으로 
	for(TIME tmWait=0; tmWait<tmTimeout*1000; 
	Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) { 
	  
	  //! 서버 업데이트.
	  // handle server messages
	  _cmiComm.Server_Update();
	  //! 서버가 스타트 되었다면,
	  if (_pNetwork->ga_srvServer.srv_bActive) {
		  // handle all incoming messages
		  _pNetwork->ga_srvServer.HandleAll();
	  }
	  //! 서버 메인 loop
	  _pNetwork->ga_srvServer.ServerLoop();
	  //! 또 서버 업데이트
	  _cmiComm.Server_Update();
	  //! 로컬 클라이언트 업데이트.
	  if (_cmiComm.Client_Update() == FALSE) {
		  break;
	  }
	  
	  //! 로컬클라이언트가 서버로 부터의 릴라이어블 메시지를 기다린다.
	  // wait for message to come
	  CNetworkMessage nmReceived;
	  if (!_pNetwork->ReceiveFromServerReliable(nmReceived)) {
		  continue;
	  }
	  //! 그 메시지가 초기화 메시지라면,
	  // if this is the init message
	  if (nmReceived.GetType() == MSG_REP_CONNECTLOCALSESSIONSTATE) {
		  // just adjust your tick counters
		  nmReceived>>ses_tmLastProcessedTick;
		  nmReceived>>ses_iLastProcessedSequence;
		  ses_tmInitializationTick  = -1.0f;
		  ses_tmInitializationTick2 = -1.0f;
		  ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
		  //! for문 빠져나감.
		  // finish waiting
		  return;
		  // otherwise
	  } else {
		  // it is invalid message
		  ThrowF_t(TRANS("Invalid message while waiting for server session registration"));
	  }
	  
	  // if client is disconnected
	  if (!_cmiComm.Client_IsConnected()) {
		  // quit
		  ThrowF_t(TRANS("Client disconnected"));
	  }
	}
	ThrowF_t(TRANS("Timeout while waiting for server session registration"));
}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
BOOL PCStartEffectGroup(const char *szEffectGroupName, SLONG slPCIndex, CEntity *penPC, float fDelayTime )
{
	float fStartTime = _pTimer->GetLerpedCurrentTick() +fDelayTime;
	
	//내 캐릭이면
	if(_pNetwork->MyCharacterInfo.index == slPCIndex)
	{
		if(CEntity::GetPlayerEntity(0) != NULL
			&& CEntity::GetPlayerEntity(0)->en_pmiModelInstance != NULL)
		{
			StartEffectGroup(szEffectGroupName
				, &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager
				//, _pTimer->GetLerpedCurrentTick());
				, fStartTime);
			return TRUE;
		}
	}
	//내 캐릭이 아니면
	else
	{
		if( penPC == NULL )
		{
			INDEX cnt = _pNetwork->ga_srvServer.srv_actCha.Count();
			for(INDEX i=0; i<cnt; ++i)
			{
				if(_pNetwork->ga_srvServer.srv_actCha[i].cha_Index == slPCIndex)
				{
					if(_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity != NULL
						&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance != NULL
						&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance->GetName() != "")
					{
						StartEffectGroup(szEffectGroupName
										, &(_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance->m_tmSkaTagManager)
										//,_pTimer->GetLerpedCurrentTick());
										, fStartTime);
						return TRUE;
					}
				}
			}
		}
		else
		{
			if( penPC != NULL && penPC->en_pmiModelInstance != NULL && penPC->en_pmiModelInstance->GetName() != "" )
			{
				StartEffectGroup( szEffectGroupName, &(penPC->en_pmiModelInstance->m_tmSkaTagManager)
									//, _pTimer->GetLerpedCurrentTick() );
									, fStartTime);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
BOOL ENGINE_API WildPetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet )
{
	if( penPet == NULL )
	{
		INDEX cnt = _pNetwork->ga_srvServer.srv_actWildPet.Count();
		
		for( INDEX i = 0; i < cnt; ++i )
		{
			if(_pNetwork->ga_srvServer.srv_actWildPet[i].m_nNetIndex == slPetIndex )
			{
				if(_pNetwork->ga_srvServer.srv_actWildPet[i].pet_pEntity != NULL
					&& _pNetwork->ga_srvServer.srv_actWildPet[i].pet_pEntity->en_pmiModelInstance != NULL
					&& _pNetwork->ga_srvServer.srv_actWildPet[i].pet_pEntity->en_pmiModelInstance->GetName() != "")
				{
					StartEffectGroup(szEffectGroupName
									, &(_pNetwork->ga_srvServer.srv_actWildPet[i].pet_pEntity->en_pmiModelInstance->m_tmSkaTagManager)
									,_pTimer->GetLerpedCurrentTick());
					return TRUE;
				}
			}
		}
	}
	else
	{
		if( penPet != NULL && penPet->en_pmiModelInstance != NULL && penPet->en_pmiModelInstance->GetName() != "" )
		{
			StartEffectGroup( szEffectGroupName, &(penPet->en_pmiModelInstance->m_tmSkaTagManager),
								_pTimer->GetLerpedCurrentTick() );
			return TRUE;
		}
	}
	
	return FALSE;
}


void CSessionState::Start_AtClient_t(INDEX ctLocalPlayers)     // throw char *
{
/* //0522 kwon 삭제.
	ses_dtmOldTickTime = -1;     
	ses_dtmOldRealTime = -1;
	ses_dtmAccumulatedDelta = 0; // accumulated difference between server real time and server tick time (detects server stalls)

	//! 현재 서버로부터 접속승인 패킷을 받은 상태이다. 우선 클라이언트가 살아있다는걸 알리자.타입만 보낸다.
	// send one unreliable packet to server to make the connection up and running
	CNetworkMessage nmKeepAlive(MSG_KEEPALIVE);
	_pNetwork->SendToServer(nmKeepAlive);

//! 등록 요청 메시지를 보낸다.릴라이어블 메시지.
	// send registration request
	CNetworkMessage nmRegisterSessionState(MSG_REQ_CONNECTREMOTESESSIONSTATE);
	nmRegisterSessionState<<INDEX('VTAG')<<INDEX(_SE_BUILD_MAJOR)<<INDEX(_SE_BUILD_MINOR);
	nmRegisterSessionState<<_strModName;
	extern CTString net_strConnectPassword;
	extern CTString net_strVIPPassword;
	extern CTString net_strObserverPassword;
	CTString strPasw = net_strConnectPassword;
	if (strPasw=="") {
		strPasw = net_strVIPPassword;
	}
	if (strPasw=="") {
		strPasw = net_strObserverPassword;
	}

	nmRegisterSessionState<<strPasw;
	nmRegisterSessionState<<ctLocalPlayers;
	ses_sspParams.Update();
	nmRegisterSessionState<<ses_sspParams;
	_pNetwork->SendToServerReliable(nmRegisterSessionState);

	// prepare file or memory stream for state
	CTFileStream strmStateFile; CTMemoryStream strmStateMem;
	{
	  //! 서버의 응답을 기다린다.
		// wait for server's response
		CTMemoryStream strmMessage;
		WaitStream_t(strmMessage, "reply", MSG_REP_CONNECTREMOTESESSIONSTATE);
	//! 서버 응답이 왔다! 메시지에서 정보를 읽어들인다. 월드에 대한것...
		// get the number of server level changes, and store it
		strmMessage>>ses_ubNumLevelChanges;

		// get motd
		strmMessage>>ses_strMOTD;
		// get info for creating default state
		CTFileName fnmWorld;
		strmMessage>>fnmWorld;
		ULONG ulSpawnFlags;
		strmMessage>>ulSpawnFlags;
		UBYTE aubProperties[NET_MAXSESSIONPROPERTIES];
		strmMessage.Read_t(aubProperties, NET_MAXSESSIONPROPERTIES);


		_pNetwork->ga_sesSessionState.ses_ulSpawnFlags = ulSpawnFlags;
		memcpy(_pNetwork->ga_aubProperties, aubProperties, NET_MAXSESSIONPROPERTIES);
		_pNetwork->ga_fnmWorld = fnmWorld;
		_pNetwork->ga_fnmNextLevel = CTString("");

		// flush stale caches
		extern void FreeUnusedStock(void);
		FreeUnusedStock();
	
		try {
	  //! 월드를 로드한다.
			// load the world
			_pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading
			_pNetwork->ga_World.Load_t(fnmWorld);
	  //! 스폰 플래그에 맞지 않는 모든 엔티티를 삭제한다.
			// delete all entities that don't fit given spawn flags
			_pNetwork->ga_World.FilterEntitiesBySpawnFlags(_pNetwork->ga_sesSessionState.ses_ulSpawnFlags);
		} catch(char *) {
			_pNetwork->ga_World.Clear();
			FreeUnusedStock();
			throw;
		}
		// remember the world filename
		_pNetwork->ga_fnmWorld = fnmWorld;
		_pNetwork->ga_fnmNextLevel = CTString("");
		// remember the world pointer
		_pShell->SetINDEX("pwoCurrentWorld", (INDEX)&_pNetwork->ga_World);

		// reset random number generator
		_pNetwork->ga_sesSessionState.ResetRND();
	//! 왜 하는지 모르겠다..ㅡㅡ;
		// warmup the world
		_pNetwork->ga_sesSessionState.WarmUpWorld();
	}


	// send one unreliable packet to server to make the connection up and running
	{CNetworkMessage nmKeepAlive(MSG_KEEPALIVE);
	_pNetwork->SendToServer(nmKeepAlive); } 
	
	//! 풀 게임정보를 서버에 요청한다.
	// send data request
	CPrintF(TRANS("Sending full game state request\n"));
	CNetworkMessage nmRequestGameState(MSG_REQ_GAMESTATE);
	_pNetwork->SendToServerReliable(nmRequestGameState);

	{
	//! 서버의 응답을 기다린다.
		// wait for server's response
		CTMemoryStream strmMessage;
		WaitStream_t(strmMessage, "data", MSG_REP_GAMESTATE);
		
		// send one unreliable packet to server to make the connection up and running
		{CNetworkMessage nmKeepAlive(MSG_KEEPALIVE);
		_pNetwork->SendToServer(nmKeepAlive); }

	//! 받은 메시지 언팩 하기.
		CTMemoryStream strmGameState;
		CzlibCompressor comp;
		comp.UnpackStream_t(strmMessage, strmGameState);

	//! 서버 레벨이 몇번 바뀌었는지 얻어낸다.
		// get the number of server level changes
		UBYTE ubNumLevelChanges;
		strmGameState>>ubNumLevelChanges;
	//! 처음에 받은 레벨과 다르다면 다음 레벨로 바꾼다.
		// if it doesn't match the locally stored number
		if (ses_ubNumLevelChanges != ubNumLevelChanges) {
			// bail out
			ThrowF_t(TRANS(" level change is in progress\n"));
		}

		// set proper ID sequence for entities created by this machine - used to separate 
		// server and client generated IDs
		if (!_pNetwork->IsServer()) {
			_pNetwork->ga_World.wo_ulNextEntityID = 0x10000001;
		}
		Read_t(&strmGameState,TRUE);

		// send one unreliable packet to server to make the connection up and running
		{CNetworkMessage nmKeepAlive(MSG_KEEPALIVE);
		_pNetwork->SendToServer(nmKeepAlive); }

	//! 서버에서 받은 값으로 crc를 생성해서 서버로 보낸다.
		CNetworkMessage nmCRC(MSG_REP_CRCCHECK);
//0130
//    nmCRC<<CRCT_MakeCRCForFiles_t(strmGameState);	
		CPrintF(TRANS("Sending CRC to server\n"));
		_pNetwork->SendToServerReliable(nmCRC);
		

	}      

	// send one unreliable packet to server to make the connection up and running
	{CNetworkMessage nmKeepAlive(MSG_KEEPALIVE);
	_pNetwork->SendToServer(nmKeepAlive); }
*/
}

// notify entities of level change
void CSessionState::SendLevelChangeNotification(CEntityEvent &ee)
{
	// for each entity in the world
	{FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
		// if it should be notified
		if (iten->en_ulFlags&ENF_NOTIFYLEVELCHANGE) {
			// send the event
			iten->SendEvent(ee);
		}
	}}
}
//! 서버로부터 오는 스트림을 기다린다.
// wait for a stream to come from server
void CSessionState::WaitStream_t(CTMemoryStream &strmMessage, const CTString &strName, 
																 INDEX iMsgCode)
{

	// start waiting for server's response
	SetProgressDescription(TRANS("waiting for ")+strName);
	CallProgressHook_t(0.0f);
	SLONG slReceivedLast = 0;

	// repeat until timed out
	for(TIME tmWait=0; tmWait<net_tmConnectionTimeout*1000/3;
		Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) {
		//!서버에서 답변이 올때까지 보낸다.
		// update network connection sockets
		if (_cmiComm.Client_Update() == FALSE) {
			break;
		}



		// check how much is received so far
		SLONG slExpectedSize; 
		SLONG slReceivedSize;
		//! 패킷에 적혀있는 전송사이즈(slExpectedSize)와 
	//! 실제로 버퍼에 존재하는 릴라이어블 패킷의 사이즈(slReceivedSize)를 얻어낸다.	
		_cmiComm.Client_PeekSize_Reliable(slExpectedSize,slReceivedSize);
	//! 아직 패킷이 버퍼에 안들어왔다면,
		// if nothing received yet
		if (slExpectedSize==0) {
			// progress with waiting
	  //! 네트워크 업데이트 함수였다!!!
			CallProgressHook_t(tmWait/(net_tmConnectionTimeout*1000));
	  //! 무언가 패킷이 들어왔는데,
			// if something received
		} else {
	  //! 새로운 데이타를 받았다면,
			// if some new data received
			if (slReceivedSize!=slReceivedLast) {
				slReceivedLast = slReceivedSize;
				// reset timeout
				tmWait=0;
			}
			// progress with receiving
			SetProgressDescription(TRANS("receiving ")+strName+"  ");
			CallProgressHook_t((float)slReceivedSize/slExpectedSize);
		}

	//! 만약에 제대로 다 받지 못했다면, continue.
		// if not everything received yet
		if (!_pNetwork->ReceiveFromServerReliable(strmMessage)) {
			// continue waiting
			continue;
		}
	//! 이제 메시지를 받았다.메시지의 첫번째 위치로 이동하여 아이디를 얻어낸다.
		// read message identifier
		strmMessage.SetPos_t(0);
		INDEX iID;
		strmMessage>>iID;
	//! 우리가 기다리면 메시지 코드이냐...처음엔 MSG_REP_CONNECTREMOTESESSIONSTATE
		// if this is the message
		if (iID == iMsgCode) {
			// all ok
			CallProgressHook_t(1.0f);
			return;
		// if disconnected
		}
/* //0522 kwon 삭제.		
		else if (iID == MSG_INF_DISCONNECTED) {
		//! 디스커넥트 확인 메시지 보냄.
		// confirm disconnect
		CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
		_pNetwork->SendToServerReliable(nmConfirmDisconnect);
		//! 디스커넥트 이유
		// report the reason
		CTString strReason;
		strmMessage>>strReason;
		ses_strDisconnected = strReason;
		ThrowF_t(TRANS("Disconnected: %s\n"), strReason);
		// otherwise
		} */else {
			 //! 이상한 메시지가 날라온것임.
			// it is invalid message
			ThrowF_t(TRANS("Invalid stream while waiting for %s"), strName);
		}

		// if client is disconnected
		if (!_cmiComm.Client_IsConnected()) {
			// quit
			ThrowF_t(TRANS("Client disconnected"));
		}
	}

//	CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
//	_pNetwork->SendToServerReliable(nmConfirmDisconnect);

	
	ThrowF_t(TRANS("Timeout while waiting for %s"), strName);
}

// check if disconnected
BOOL CSessionState::IsDisconnected(void)
{
	return ses_strDisconnected!="";
}

// print an incoming chat message to console
void CSessionState::PrintChatMessage(ULONG ulFrom, const CTString &strFrom, const CTString &strMessage)
{
	CTString strSender;
	// if no sender players
	if (ulFrom==0) {
		// take symbolic sender string
		strSender = strFrom;
	// if there are sender players
	} else {
		// for each sender player
		for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ipl++) {
			CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[ipl];
			if (plt.IsActive() && (ulFrom & (1UL<<ipl)) ) {
				// add its name to the sender list
				if (strSender!="") {
					strSender+=", ";
				}
				strSender+=plt.plt_penPlayerEntity->GetPlayerName();
			}
		}
	}

	// let eventual script addon process the message
	extern CTString cmd_strChatSender ;
	extern CTString cmd_strChatMessage;
	extern CTString cmd_cmdOnChat;
	cmd_strChatSender = strSender;
	cmd_strChatMessage = strMessage;
	if (cmd_cmdOnChat!="") {
		_pShell->Execute(cmd_cmdOnChat);
	}

	// if proccessing didn't kill it
	if (cmd_strChatSender!="" && cmd_strChatMessage!="") {
		// print the message
		CPrintF("%s: ^o^cFFFFFF%s^r\n", (const char*)RemoveSpecialCodes(cmd_strChatSender), (const char*)RemoveSpecialCodes(cmd_strChatMessage));
	}
	extern INDEX net_ctChatMessages;
	net_ctChatMessages++;
}

/* NOTES:
1) New thinkers might be added by current ones, but it doesn't matter,
since they must be added forward in time and the list is sorted, so they
cannot be processed in this tick.
2) Thinkers/Movers can be removed during iteration, but the CEntityPointer
guarantee that they are not freed from memory.
*/

// do physics for a game tick
void CSessionState::HandleMovers(void)
{
//  CPrintF("---- tick %g\n", _pTimer->CurrentTick());
	// put all movers in active list, pushing ones first
	CListHead lhActiveMovers, lhDoneMovers, lhDummyMovers;
	{FORDELETELIST(CMovableEntity, en_lnInMovers, _pNetwork->ga_World.wo_lhMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		pen->en_lnInMovers.Remove();
		if (!(pen->en_ulFlags&ENF_DELETED)) {
			if ((pen->en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
				lhActiveMovers.AddHead(pen->en_lnInMovers);
			} else {
				lhActiveMovers.AddTail(pen->en_lnInMovers);
			}
		}
	}}


	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it clear its temporary variables    

		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->ClearMovingTemp();
	}}

	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it calculate its wanted parameters for this tick
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->PreMoving();
	}}

 

	// while there are some active movers
	while(!lhActiveMovers.IsEmpty()) {
		// get first one
		CMovableEntity *penMoving = LIST_HEAD(lhActiveMovers, CMovableEntity, en_lnInMovers);
		CEntityPointer penCurrent = penMoving;  // just to keep it alive around the loop
		// first move it to done list (if not done, it will move back to world's movers)
		penMoving->en_lnInMovers.Remove();
		lhDoneMovers.AddTail(penMoving->en_lnInMovers);

		// let it do its own physics
		if (penMoving->IsPlayer() && !penMoving->IsActionApplied()) {
//      lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
			continue;
		}

		penMoving->DoMoving();

		// if any mover is re-added, put it to the end of active list
		lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
	}


	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		// let it calculate its parameters after all movement has been resolved
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->PostMoving();
	}}

 

	// for each done mover

	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		// if marked for removing from list of movers
		if (pen->en_ulFlags&ENF_INRENDERING) {
			// remove it
			pen->en_ulFlags&=~ENF_INRENDERING;
			pen->en_lnInMovers.Remove();

			// this means that the entity has stopped moving, so 
			// force sending an update to the client
			if (pen->IsSentOverNet() && !pen->IsPlayer()) {
				ses_emEntityMessage.WritePlacementNotify(pen->en_ulID,TRUE);      
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			}

		}
		// let it clear its temporary variables
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		pen->ClearMovingTemp();
	}}
	
	// return all done movers to the world's list
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDummyMovers);
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDoneMovers);

	// handle all the sent events
	CEntity::HandleSentEvents();
}


// let the client move the entities around
void CSessionState::HandleMovers_client(void)
{
	ASSERT(!_pNetwork->IsServer());

//  CPrintF("---- tick %g\n", _pTimer->CurrentTick());
	// put all movers in active list, pushing ones first
	CListHead lhActiveMovers, lhDoneMovers, lhDummyMovers;
	{FORDELETELIST(CMovableEntity, en_lnInMovers, _pNetwork->ga_World.wo_lhMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		pen->en_lnInMovers.Remove();
		if (!(pen->en_ulFlags&ENF_DELETED)) {
			if ((pen->en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
				lhActiveMovers.AddHead(pen->en_lnInMovers);
			} else {
				lhActiveMovers.AddTail(pen->en_lnInMovers);
			}
		}
	}}
	
	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it clear its temporary variables
		itenMover->ClearMovingTemp();
	}}

	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// if this is the client and this is a locally created entity
		if (itenMover->IsSentOverNet()) {
			continue;
		}
		itenMover->PreMoving();
	}}

	// while there are some active movers
	while(!lhActiveMovers.IsEmpty()) {
		// get first one
		CMovableEntity *penMoving = LIST_HEAD(lhActiveMovers, CMovableEntity, en_lnInMovers);
		CEntityPointer penCurrent = penMoving;  // just to keep it alive around the loop
		// first move it to done list (if not done, it will move back to world's movers)
		penMoving->en_lnInMovers.Remove();
		lhDoneMovers.AddTail(penMoving->en_lnInMovers);
		
		//lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);

		// let it do its own physics                            
		if (penMoving->IsSentOverNet()) {
			penMoving->ApplyDeadReckoning();     
			continue;
		}

		penMoving->DoMoving();

		// if any mover is re-added, put it to the end of active list
		lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
	}

	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		// let it calculate its parameters after all movement has been resolved
		if (itenMover->IsSentOverNet()) {
			continue;
		}
		itenMover->PostMoving();
	}}


	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		// if marked for removing from list of movers
		if (pen->en_ulFlags&ENF_INRENDERING) {
			// remove it
			pen->en_ulFlags&=~ENF_INRENDERING;
			pen->en_lnInMovers.Remove();
		}
		// let it clear its temporary variables
		pen->ClearMovingTemp();
	}}
	
	// return all done movers to the world's list
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDummyMovers);
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDoneMovers);
}


// do thinking for a game tick
void CSessionState::HandleTimers(TIME tmCurrentTick)
{
#define TIME_EPSILON 0.0001f
	IFDEBUG(TIME tmLast = 0.0f);

	// repeat
	CListHead &lhTimers = _pNetwork->ga_World.wo_lhTimers;
	FOREVER {
		// no entity found initially
		CRationalEntity *penTimer = NULL;
		// for each entity in list of timers
		FOREACHINLIST(CRationalEntity, en_lnInTimers, lhTimers, iten) {
			// if due after current time
			if(iten->en_timeTimer>tmCurrentTick+TIME_EPSILON) {
				// stop searching
				break;
			}
			// remember found entity, and stop searching
			penTimer = iten;
			break;
		}

		// if no entity is found
		if (penTimer==NULL) {
			// stop
			break;
		}
		
		// if this entity isn't locally created, remove it from the timer list and skip it
		if (!cm_ciLocalClient.ci_bClientLocal && cm_ciLocalClient.ci_bUsed && penTimer->IsSentOverNet() && penTimer->IsFlagOff(ENF_CLIENTHANDLING)) {
			// remove the timer from the list
			penTimer->en_timeTimer = THINKTIME_NEVER;
			penTimer->en_lnInTimers.Remove();
			continue;
		}
		 
		extern INDEX net_bReportMiscErrors;
		// check that timers are propertly handled - client doesn't do thinking, so it doesn't check
//안태훈 수정 시작	//(Zone Change System)(0.1)
		//if (_pNetwork->IsServer()) {
			//ASSERT(penTimer->en_timeTimer>tmCurrentTick-_pTimer->TickQuantum-TIME_EPSILON);
		//}
//안태훈 수정 끝	//(Zone Change System)(0.1)
		//ASSERT(penTimer->en_timeTimer>=tmLast);
		IFDEBUG(tmLast=penTimer->en_timeTimer);

		// remove the timer from the list
		penTimer->en_timeTimer = THINKTIME_NEVER;
		penTimer->en_lnInTimers.Remove();
		// send timer event to the entity
		penTimer->SendEvent(ETimer());
	}

	// handle all the sent events
	CEntity::HandleSentEvents();
}


// do a warm-up run of the world for a few ticks
void CSessionState::WarmUpWorld(void)
{
#define WORLD_WORMUP_COUNT 20   // run 20 ticks to get stable
	ses_tmLastProcessedTick = _pNetwork->ga_srvServer.srv_tmLastProcessedTick = 0;
	ses_iLastProcessedSequence = _pNetwork->ga_srvServer.srv_iLastProcessedSequence = -1;
	// add a few empty all-action messages to the game stream
	for (INDEX iTick=0; iTick<WORLD_WORMUP_COUNT; iTick++) {
		_pNetwork->ga_srvServer.srv_tmLastProcessedTick += _pTimer->TickQuantum;
		_pNetwork->ga_srvServer.srv_iLastProcessedSequence++;
		CNetworkStreamBlock nsbAllActions(MSG_SEQ_ALLACTIONS, _pNetwork->ga_srvServer.srv_iLastProcessedSequence);
		nsbAllActions<<_pNetwork->ga_srvServer.srv_tmLastProcessedTick;
		nsbAllActions.Rewind();
		ses_nsGameStream.AddBlock(nsbAllActions);
	}

	// process the blocks
	ProcessGameStream();
}


/*
 * Process a game tick.
 */

void CSessionState::ProcessGameTick(CNetworkMessage &nmMessage, TIME tmCurrentTick)
{
	ASSERT(this!=NULL);

	// copy the tick to process into tick used for all tasks
	_pTimer->SetCurrentTick(tmCurrentTick);
	_pfNetworkProfile.IncrementAveragingCounter();

	// random is allowed only here, during entity ai
	ses_bAllowRandom = TRUE;

	// clear the applied action flag for all players
	for (INDEX ctPlayers=0;ctPlayers<NET_MAXGAMEPLAYERS;ctPlayers++) {
		if (_pNetwork->ga_srvServer.srv_apltPlayers[ctPlayers].plt_penPlayerEntity != NULL) {
			_pNetwork->ga_srvServer.srv_apltPlayers[ctPlayers].plt_penPlayerEntity->ClearActionApplied();
		}
	}

	// for all players in game
	INDEX iPlayer = 0;
	CPlayerAction paAction;
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itplt) 
	{
		// if player is active
		if (itplt->IsActive()) 
		{
			// while there are actions in this player's buffer
			while(itplt->HasBufferedActions()) 
			{
				itplt->GetActionPacket(paAction);

				CPlayerTarget *pptTarget = itplt;
				// if this is action number two for a certian player, perform it's moving operations      
				if (pptTarget->plt_penPlayerEntity->IsActionApplied()) 
				{
					pptTarget->plt_penPlayerEntity->ClearMovingTemp();
					pptTarget->plt_penPlayerEntity->PreMoving();
					pptTarget->plt_penPlayerEntity->DoMoving(); 
					pptTarget->plt_penPlayerEntity->PostMoving();
					pptTarget->plt_penPlayerEntity->ClearMovingTemp();
					CEntity::HandleSentEvents();
				}

				//Sleep(1);
				// apply the new action and mark it
				pptTarget->ApplyActionPacket(paAction);
				pptTarget->plt_penPlayerEntity->SetActionApplied();

				ses_emEntityMessage.WritePlayerAction(pptTarget->plt_penPlayerEntity->en_ulID,paAction);
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			}
		}
		iPlayer++;
	}

	// handle all the sent events
	CEntity::HandleSentEvents();

	if (_pNetwork->IsServer() || _pNetwork->ga_bDemoPlay) {
		// do thinking
		HandleTimers(tmCurrentTick);

		// do physics
		HandleMovers();
	} 

	// notify all entities of level change as needed
	if (_lphCurrent==LCP_INITIATED) {
		EPreLevelChange ePreChange;
		ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePreChange);
		CEntity::HandleSentEvents();
		_lphCurrent=LCP_SIGNALLED;
	}
	if (_lphCurrent==LCP_CHANGED) {
		EPostLevelChange ePostChange;
		ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePostChange);
		CEntity::HandleSentEvents();
		_lphCurrent=LCP_NOCHANGE;
	}

	// let the worldbase execute its tick function
	if (_pNetwork->ga_World.wo_pecWorldBaseClass!=NULL
		&&_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass!=NULL
		&&_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldTick!=NULL) {
		_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass->
			dec_OnWorldTick(&_pNetwork->ga_World);
	}
	// handle all the sent events
	CEntity::HandleSentEvents();

	ses_bAllowRandom = FALSE;

	// let eventual script do something on each tick
	extern FLOAT cmd_tmTick;
	extern CTString cmd_cmdOnTick;
	if (cmd_cmdOnTick!="") {
		cmd_tmTick = tmCurrentTick;
		_pShell->Execute(cmd_cmdOnTick);
	}
}

//! 게임스트림을 처리한다.
/*
 * Process all eventual available gamestream blocks.
 */
void CSessionState::ProcessGameStream(void)
{

// this is actually a compiler kludge - it messes up the stack pointer when compiling debug code 
// this kludge gets rid of it 
#ifndef NDEBUG
	CSetFPUPrecision FPUPrecision(FPT_64BIT);  
#endif 

	// repeat
	FOREVER {
		// calculate index of next expected sequence
		INDEX iSequence = ses_iLastProcessedSequence+1;
		// get the stream block with that sequence
		CNetworkStreamBlock *pnsbBlock;
	//서버로부터 온 게임스트림에서 순서대로 블록을 읽어들인다.
		CNetworkStream::Result res = ses_nsGameStream.GetBlockBySequence(iSequence, pnsbBlock);
		// if it is found
		if (res==CNetworkStream::R_OK) 
		{
			// remember the message type
			int iMsgType=pnsbBlock->GetType();
			// remember the processed sequence
			ses_iLastProcessedSequence = iSequence;
			// process the stream block
			ProcessGameStreamBlock(*pnsbBlock);
			// remove the block from the stream
			pnsbBlock->RemoveFromStream();
			// delete network stream block
			DeleteNetworkStreamBlock(pnsbBlock);
			// remove eventual resent blocks that have already been processed
			ses_nsGameStream.RemoveOlderBlocksBySequence(ses_iLastProcessedSequence-2);

			// if the message is all actions
			if (iMsgType==MSG_SEQ_ALLACTIONS) {
				// if playing a demo
				if (_pNetwork->ga_bDemoPlay) {
					// step demo sequence
					ses_tmLastDemoSequence+=_pTimer->TickQuantum;
				}
			}

		// if it is not avaliable yet
		} if (res==CNetworkStream::R_BLOCKNOTRECEIVEDYET) {
			// finish
			return;
	//! 여긴 클라이언트만 들어온다.
		// if it is missing
		} else if (res==CNetworkStream::R_BLOCKMISSING) {
			
			// if it is a new sequence
			if (iSequence>ses_iMissingSequence) {
				ses_iMissingSequence = iSequence;
				// setup timeout
				ses_tvResendTime = _pTimer->GetHighPrecisionTimer();
				ses_tmResendTimeout = 0.1f;
			}

			// if timeout occured
			if (_pTimer->GetHighPrecisionTimer()>ses_tvResendTime+CTimerValue(ses_tmResendTimeout)) {

				_pNetwork->AddNetGraphValue(NGET_MISSING, 1.0f); // missing sequence

				// find how many are missing
				INDEX iNextValid = ses_nsGameStream.GetOldestSequenceAfter(iSequence);
				INDEX ctSequences = Max(iNextValid-iSequence, INDEX(1));

					// create a request for resending the missing packet
				CNetworkMessage nmResendRequest(MSG_REQUESTGAMESTREAMRESEND);
				nmResendRequest<<iSequence;
				nmResendRequest<<ctSequences;
				// send the request to the server
				_pNetwork->SendToServer(nmResendRequest);

				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Session State: Missing sequences %d-%d(%d) timeout %g\n"), 
						iSequence, iSequence+ctSequences-1, ctSequences, ses_tmResendTimeout);
				}

				// increase the timeout
				ses_tvResendTime = _pTimer->GetHighPrecisionTimer();
				ses_tmResendTimeout *= 2.0f;
			}

			// finish
			return;
		}
	}

}

//! 게임스트림 블록을 처리.
/*
 * Process a gamestream block.
 */
void CSessionState::ProcessGameStreamBlock(CNetworkMessage &nmMessage)
{
	// copy the tick to process into tick used for all tasks
	_pTimer->SetCurrentTick(ses_tmLastProcessedTick);

	// check message type
	switch (nmMessage.GetType()) 
	{
/* //0522 kwon 삭제.
	// if removing a player
	case MSG_SEQ_REMPLAYER: {
			_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence
			INDEX iPlayer;
			nmMessage>>iPlayer;      // player index

			if (_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity == NULL) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: trying to remove a non-existing player, index: %d\n"),iPlayer);
				}
				break;
			}

			// inform entity of disconnnection
			CPrintF(TRANS("%s left\n"), _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->GetPlayerName());
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->Disconnect();
			// deactivate the player
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
			// handle all the sent events
			ses_bAllowRandom = TRUE;
			CEntity::HandleSentEvents();
			ses_bAllowRandom = FALSE;

		} break;
*/
	// if changing character
	case MSG_SEQ_CHARACTERCHANGE: 
		{
			_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence
			INDEX iPlayer;
			CPlayerCharacter pcCharacter;
			nmMessage>>iPlayer>>pcCharacter;

			// change the character
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->CharacterChanged(pcCharacter);

			// handle all the sent events
			ses_bAllowRandom = TRUE;
			CEntity::HandleSentEvents();
			ses_bAllowRandom = FALSE;
			
			ses_emEntityMessage.WriteCharacterChange(_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->en_ulID,pcCharacter);
			_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			
		} break;

	  //! 클라이언트 액션을 받았다면,
	// if receiving client actions
	case MSG_SEQ_ALLACTIONS: 
		{
			// read time from packet
			TIME tmPacket;
			nmMessage>>tmPacket;    // packet time

			// time must be greater by one than that on the last packet received
			TIME tmTickQuantum = _pTimer->TickQuantum;
			TIME tmPacketDelta = tmPacket-ses_tmLastProcessedTick;
			if(! (Abs(tmPacketDelta-tmTickQuantum) < (tmTickQuantum/10.0f)) ) {
				// report debug info
				
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Session state: Mistimed MSG_ALLACTIONS: Last received tick %g, this tick %g\n"),
					ses_tmLastProcessedTick, tmPacket);
				}
			}
			// remember the received tick
			ses_tmLastProcessedTick = tmPacket;
			
/* //0522 kwon 삭제.
			if (_pNetwork->IsServer() && _pNetwork->ga_srvServer.srv_bSendPause && _pNetwork->ga_srvServer.srv_bPauseValue) {
				ses_emEntityMessage.WritePause(_pNetwork->ga_srvServer.srv_bPauseValue,_pNetwork->ga_srvServer.srv_strPauser);
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
				_pNetwork->ga_srvServer.srv_bSendPause = FALSE;
				_pNetwork->ga_srvServer.srv_bPause = _pNetwork->ga_srvServer.srv_bPauseValue;
				CNetworkStreamBlock nsbPause(MSG_SEQ_PAUSE, ++_pNetwork->ga_srvServer.srv_iLastProcessedSequence);
				nsbPause<<(INDEX&)_pNetwork->ga_srvServer.srv_bPause;
				nsbPause<<_pNetwork->ga_srvServer.srv_strPauser;        
				// put the message in buffer
				_pNetwork->ga_srvServer.srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbPause);
			}
*/
			INDEX iActiveSessions = 0;
			for (INDEX iSession=1;iSession < _pNetwork->ga_srvServer.srv_assoSessions.Count();iSession++) {
				if (_pNetwork->ga_srvServer.srv_assoSessions[iSession].sso_bTrackData) {
					iActiveSessions++;
				}
			}

			_pNetwork->ga_srvServer.StartNewTick(ses_tmLastProcessedTick);
			if (_pNetwork->ga_srvServer.srv_bRunning) {       
				_pNetwork->ga_srvServer.srv_embSendBuffer.RequestTickAcknowledge(ses_tmLastProcessedTick,iActiveSessions);
			}

			// NOTE: if we got a tick, it means that all players have joined
			// don't wait for new players any more
			ses_bWaitAllPlayers = FALSE;

			// process the tick
			ProcessGameTick(nmMessage, tmPacket);
	
		} break;
/* //0522 kwon 삭제.
	// if receiving pause message
	case MSG_SEQ_PAUSE: 
	{
		_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence

		BOOL bPauseBefore = ses_bPause;
		// read the pause state and pauser from it
		nmMessage>>(INDEX&)ses_bPause;
		CTString strPauser;
		nmMessage>>strPauser;
		// if paused by some other machine
		if (strPauser!=TRANS("Local machine")) {
			// report who paused
			if (ses_bPause!=bPauseBefore) {
				if (ses_bPause) {
					CPrintF(TRANS("Paused by '%s'\n"), strPauser);
				} else {
					CPrintF(TRANS("Unpaused by '%s'\n"), strPauser);
				}
			}
		}
		// must keep wanting current state
		ses_bWantPause = ses_bPause;
													} break;
*/
	// otherwise
	default:
		// error
		ASSERT(FALSE);
	}
}

//-----------------------------------------------------------------------------
// Purpose: attach a player entity to this session 
// Input  : iPlayerIndex - 
//			&pcCharacter - 
//			&ulEntityID - 
//			&ulWeaponsID - 
//			&ulAnimatorID - 
//			iClient - 
//-----------------------------------------------------------------------------
void CSessionState::AddPlayer(
							  INDEX iPlayerIndex, 
							  CPlayerCharacter &pcCharacter, 
							  ULONG &ulEntityID, 
							  ULONG &ulWeaponsID, 
							  ULONG &ulAnimatorID, 
							  INDEX iClient) 
{
	ASSERT(iPlayerIndex >=0);

	INDEX iPlayerWithName = 0;
	BOOL bFound = FALSE;
	CPlayerEntity *penNewPlayer = NULL;
	

	// if this is single player
	if (ses_ctMaxPlayers == 1) 
	{
		// get the first player entity in the world
		penNewPlayer = _pNetwork->GetNthPlayerEntity(0);
		_pNetwork->ga_srvServer.srv_apltPlayers[0].plt_bActive = FALSE;
	} 
		// if multiplayer
	else 
	{
		// try to find a player entity with the same name as this player character
		// 같은 이름의 플레이어 엔티티가 있는지 찾아보기.
		do 
		{
			penNewPlayer = _pNetwork->GetNthPlayerEntityWithName(pcCharacter.pc_strName, iPlayerWithName);
			bFound = FALSE;
			if (penNewPlayer != NULL) 
			{
				// if the names match, check if it's already active
				for (INDEX iTarget=0;iTarget<NET_MAXGAMEPLAYERS;iTarget++) 
				{
					// and if it is, skip it (someone is already playing with it)
					if (_pNetwork->ga_srvServer.srv_apltPlayers[iTarget].IsActive() &&
							_pNetwork->ga_srvServer.srv_apltPlayers[iTarget].plt_penPlayerEntity == penNewPlayer)
					{
						bFound = TRUE;
						break;
					}
				}
			}
			iPlayerWithName++;
		} while (bFound);
	}

	// activate the player target
	_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].Activate(iClient);
	pcCharacter.pc_iPlayerIndex = iPlayerIndex;

	// if there is no entity with that character in the world
	// 월드내에 캐릭터 엔티티가 존재하지 않는 경우.
	if (penNewPlayer==NULL) 
	{
		// create an entity for it
		CPlacement3D pl(FLOAT3D(0.0f,0.0f,0.0f), ANGLE3D(0,0,0));
		try 
		{
			CTFileName fnmPlayer = CTString("Classes\\Player.ecl"); // this must not be a dependency!
			penNewPlayer = (CPlayerEntity*)(_pNetwork->ga_World.CreateEntity_t(pl, fnmPlayer,ulEntityID,FALSE));
			// attach entity to client data
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].AttachEntity(penNewPlayer);
			// attach the character to it
			penNewPlayer->en_pcCharacter = pcCharacter;
			// initialize the player      
			penNewPlayer->InitializePlayer(ulWeaponsID,ulAnimatorID);      
		} 
		catch (char *strError) 
		{
			(void)strError;
			FatalError(TRANS("Cannot load Player class:\n%s"), strError);
		}
		if (!_pNetwork->IsPlayerLocal(penNewPlayer)) 
		{
			CPrintF(TRANS("%s joined\n"), (const char*) RemoveSpecialCodes(penNewPlayer->en_pcCharacter.GetNameForPrinting()));
		}
	} 
	else 
	{
		// attach entity to client data
		_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].AttachEntity(penNewPlayer);
		// make it update its character
		penNewPlayer->CharacterChanged(pcCharacter);

		if (!_pNetwork->IsPlayerLocal(penNewPlayer)) 
		{
			CPrintF(TRANS("%s rejoined\n"), (const char*) RemoveSpecialCodes(penNewPlayer->en_pcCharacter.GetNameForPrinting()));
		}
	}

	ulEntityID = penNewPlayer->en_ulID;

}




// Set lerping factor for current frame.
void CSessionState::SetLerpFactor(CTimerValue tvNow)
{
	TIME tmLastTick;
	if (_pNetwork->IsServer()) 
	{
		tmLastTick = ses_tmLastProcessedTick;
	} 
	else 
	{
		tmLastTick = _tmLocalTick;
	} 
	
//강동민 수정 시작 로그인 처리 작업	07.13
#ifndef FINALVERSION
	if (_pNetwork->IsPlayingDemo()) 
	{
		tvNow = CTimerValue(_pNetwork->ga_fDemoTimer);
		tmLastTick = _pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum;
	}
#endif	// FINALVERSION
//강동민 수정 끝 로그인 처리 작업		07.13
	
	// if no lerping
	if (!net_bLerping) 
	{
		// set lerping factor without lerping
		_pTimer->SetLerp(1.0f);
		_pTimer->SetCurrentTick(tmLastTick);
		return;
	}
	
	FLOAT fFactor  = 0.0f;
	FLOAT fFactor2 = 0.0f;
	
	// ---- primary factor - used for prediction
	{
		// if lerping was never set before
		if (ses_tmInitializationTick<0) {
			// initialize it
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick;
		}

		// get passed time from session state starting in precise time and in ticks
		FLOAT tmRealDelta = FLOAT((tvNow-ses_tvInitialization).GetSeconds())
			*_pNetwork->ga_fGameRealTimeFactor*_pNetwork->ga_sesSessionState.ses_fRealTimeFactor;
		FLOAT tmTickDelta = tmLastTick-ses_tmInitializationTick;
		// calculate factor
		fFactor = 1.0f-(tmTickDelta-tmRealDelta)/_pTimer->TickQuantum;

		// if the factor starts getting below zero
		if (fFactor<0) {
			//CPrintF("Lerp=%.2f <0 @ %.2fs\n", fFactor, tmLastTick);
			// clamp it
			fFactor = 0.0f;
			// readjust timers so that it gets better
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick-_pTimer->TickQuantum;
		}
		if (fFactor>1) {
			//CPrintF("Lerp=%.2f >1 @ %.2fs\n", fFactor, tmLastTick);
			// clamp it
			fFactor = 1.0f;
			// readjust timers so that it gets better
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick;
		}

		#if DEBUG_LERPING

			avfStats[ctCounter][0] = tmRealDelta/_pTimer->TickQuantum;
			avfStats[ctCounter][1] = tmTickDelta/_pTimer->TickQuantum;
			avfStats[ctCounter][2] = fFactor;
			avfStats[ctCounter][3] = (tmLastTick/_pTimer->TickQuantum-1.0f)+fFactor;
			ctCounter++;
			if (ctCounter>=ctMax) {
				ctCounter = 0;
			}
		#endif // DEBUG_LERPING
	}

	// set lerping
	_pTimer->SetLerp(fFactor);
	_pTimer->SetLerp2(fFactor);
	_pTimer->SetCurrentTick(tmLastTick);

	static TIME tmOld = -1.0f;
	if (tmOld>0 && fabs(tmLastTick - tmOld)>0.01) {
//    CPrintF("New tick: %7.4f\n",tmLastTick);
	}
	tmOld = tmLastTick;

}




/*
 * Read session state state from a stream.
 */
void CSessionState::Read_t(CTStream *pstr,BOOL bNetwork)  // throw char *
{
	// read time information and random seed

	//! 버전 읽기
	INDEX iVersion = SESSIONSTATEVERSION_OLD;
	if (pstr->PeekID_t()==CChunkID("SESV")) {
		pstr->ExpectID_t("SESV");
		(*pstr)>>iVersion;
	}
	(*pstr)>>ses_tmLastProcessedTick;
	(*pstr)>>ses_iLastProcessedSequence;
	(*pstr)>>ses_ulRandomSeed;
	(*pstr)>>ses_ulSpawnFlags;
	(*pstr)>>ses_ctMaxPlayers;
	(*pstr)>>ses_bWaitAllPlayers;
	(*pstr)>>ses_bPause;
	(*pstr)>>ses_bGameFinished;
	if (iVersion>=SESSIONSTATEVERSION_WITHBULLETTIME) {
		(*pstr)>>ses_fRealTimeFactor;
	}
	ses_bWaitingForServer = FALSE;
	ses_bWantPause = ses_bPause;
	ses_strDisconnected = "";
	_pTimer->SetCurrentTick(ses_tmLastProcessedTick);

	// read session properties from stream
	(*pstr)>>_pNetwork->ga_strSessionName;
	pstr->Read_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);

	//! bNetwork에 FALSE는 안들어오는데? ㅡㅡ?
	if (!bNetwork) {
		// read world and its state
		ReadWorldAndState_t(pstr);
	} else {
//0130 //0203 ska스트링 테이블땜에 잠시 다시 씀.
		ReadState_t(pstr);

	}

}


void CSessionState::ReadState_t(CTStream *pstr)   // throw char *
{

	//! 모든 엔티티에 lock건다.
	// prepare the world for loading 
	_pNetwork->ga_World.LockAll();
	//! 현재의 월드 스테이트를 읽어들인다.
	// read world situation
	_pNetwork->ga_World.ReadState_net_t(pstr);
	//! 플레이어 수를 읽는다.
		// read number of players
	INDEX ctPlayers;
	(*pstr)>>ctPlayers;
	ASSERT(ctPlayers==_pNetwork->ga_srvServer.srv_apltPlayers.Count());
	//! 플레이어 수만큼 스트림에서 플레이어타겟 정보를 읽는다.
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {    
		// read from stream
		itclt->Read_t(pstr);
	}
 //! lock을 해제한다.
	_pNetwork->ga_World.UnlockAll();

};

void CSessionState::ReadWorldAndState_t(CTStream *pstr)   // throw char *
{
	// check engine build disallowing reinit
	BOOL bNeedsReinit;
	_pNetwork->CheckVersion_t(*pstr, FALSE, bNeedsReinit);
	ASSERT(!bNeedsReinit);

	// read world filename from stream
	(*pstr)>>_pNetwork->ga_fnmWorld;

	if (CTFileName(pstr->GetDescription()).FileExt()==".dem" &&
		GetFileTimeStamp_t(pstr->GetDescription())<=GetFileTimeStamp_t(_pNetwork->ga_fnmWorld)) {
		ThrowF_t(
			TRANS("Cannot play demo because file '%s'\n"
			"is older than file '%s'!\n"),
			CTString(pstr->GetDescription()),
			CTString(_pNetwork->ga_fnmWorld));
	}

	// prepare the world for loading
	_pNetwork->ga_World.Clear();
	_pNetwork->ga_World.LockAll();
	// load the world brushes from the world file
	_pNetwork->ga_World.LoadBrushes_t(_pNetwork->ga_fnmWorld);
	// read world situation
	_pNetwork->ga_World.ReadState_t(pstr);

	// create an empty list for relinking timers
	CListHead lhNewTimers;
	// read number of entities in timer list
	pstr->ExpectID_t("TMRS");   // timers
	INDEX ctTimers;
	*pstr>>ctTimers;
//  ASSERT(ctTimers == _pNetwork->ga_World.wo_lhTimers.Count());
	// for each entity in the timer list
	{for(INDEX ienTimer=0; ienTimer<ctTimers; ienTimer++) {
		// read its index in container of all entities
		INDEX ien;
		*pstr>>ien;
		// get the entity
		CRationalEntity *pen = (CRationalEntity*)_pNetwork->ga_World.EntityFromID(ien);
		// remove it from the timer list and add it at the end of the new timer list
		if (pen->en_lnInTimers.IsLinked()) {
			pen->en_lnInTimers.Remove();
			lhNewTimers.AddTail(pen->en_lnInTimers);
		}
	}}
	// use the new timer list instead the old one
	ASSERT(_pNetwork->ga_World.wo_lhTimers.IsEmpty());
	_pNetwork->ga_World.wo_lhTimers.MoveList(lhNewTimers);

	// create an empty list for relinking movers
	CListHead lhNewMovers;
	// read number of entities in mover list
	pstr->ExpectID_t("MVRS");   // movers
	INDEX ctMovers;
	*pstr>>ctMovers;
	ASSERT(ctMovers == _pNetwork->ga_World.wo_lhMovers.Count());
	// for each entity in the mover list
	{for(INDEX ienMover=0; ienMover<ctMovers; ienMover++) {
		// read its index in container of all entities
		INDEX ien;
		*pstr>>ien;
		// get the entity
		CMovableEntity *pen = (CMovableEntity*)_pNetwork->ga_World.EntityFromID(ien);
		// remove it from the mover list and add it at the end of the new mover list
		if (pen->en_lnInMovers.IsLinked()) {
			pen->en_lnInMovers.Remove();
		}
		lhNewMovers.AddTail(pen->en_lnInMovers);
	}}
	// use the new mover list instead the old one
	ASSERT(_pNetwork->ga_World.wo_lhMovers.IsEmpty());
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhNewMovers);

	// read number of players
	INDEX ctPlayers;
	(*pstr)>>ctPlayers;
	ASSERT(ctPlayers==_pNetwork->ga_srvServer.srv_apltPlayers.Count());
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// read from stream
		itclt->Read_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::ReadRememberedLevels_t(CTStream *pstr)
{
	pstr->ExpectID_t("RLEV"); // remembered levels
	// read count of remembered levels
	INDEX ctLevels;
	(*pstr)>>ctLevels;
	// for each level
	for(INDEX iLevel=0; iLevel<ctLevels; iLevel++) {
		// create it
		CRememberedLevel *prl = new CRememberedLevel;
		// read it
		(*pstr)>>prl->rl_strFileName;
		//prl->rl_strmSessionState.
		// use readstream() !!!! @@@@

	}
	
};

//! 스트림에 세션 스테이트를 쓴다.
/*
 * Write session state state into a stream.
 */
void CSessionState::Write_t(CTStream *pstr,BOOL bNetwork)  // throw char *
{
	pstr->WriteID_t("SESV");
	(*pstr)<<INDEX(SESSIONSTATEVERSION_WITHBULLETTIME);
	// write time information and random seed
	(*pstr)<<ses_tmLastProcessedTick;
	(*pstr)<<ses_iLastProcessedSequence;
	(*pstr)<<ses_ulRandomSeed;
	(*pstr)<<ses_ulSpawnFlags;
	(*pstr)<<ses_ctMaxPlayers;
	(*pstr)<<ses_bWaitAllPlayers;
	(*pstr)<<ses_bPause;
	(*pstr)<<ses_bGameFinished;
	(*pstr)<<ses_fRealTimeFactor;
	// write session properties to stream
	(*pstr)<<_pNetwork->ga_strSessionName;
	pstr->Write_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);


	if (!bNetwork) {
		// read world and its state
		WriteWorldAndState_t(pstr);
	} else {
		WriteState_t(pstr);
	}

}

//! 클라이언트로 보낼 게임 스테이트를 쓴다.
void CSessionState::WriteState_t(CTStream *pstr)   // throw char *
{
	// write world situation
	_pNetwork->ga_World.LockAll();
	//! 월드 스테이트를 쓴다.
	_pNetwork->ga_World.WriteState_net_t(pstr);

	//! 플레이어 수를 쓴다.
	// write players - their number and data for each one
	// write number of clients
	(*pstr)<<_pNetwork->ga_srvServer.srv_apltPlayers.Count();
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write to stream
		itclt->Write_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::WriteWorldAndState_t(CTStream *pstr)   // throw char *
{
	// save engine build
	_pNetwork->WriteVersion_t(*pstr);

	// write world filename to stream
	(*pstr)<<_pNetwork->ga_fnmWorld;

	// write world situation
	_pNetwork->ga_World.LockAll();
	_pNetwork->ga_World.WriteState_t(pstr, TRUE);

	// write number of entities in timer list
	pstr->WriteID_t("TMRS");   // timers
	CListHead &lhTimers = _pNetwork->ga_World.wo_lhTimers;
	*pstr<<lhTimers.Count();
	// for each entity in the timer list
	{FOREACHINLIST(CRationalEntity, en_lnInTimers, lhTimers, iten) {
		// save its index in container
		*pstr<<iten->en_ulID;
	}}

	// write number of entities in mover list
	pstr->WriteID_t("MVRS");   // movers
	CListHead &lhMovers = _pNetwork->ga_World.wo_lhMovers;
	*pstr<<lhMovers.Count();
	// for each entity in the mover list
	{FOREACHINLIST(CMovableEntity, en_lnInMovers, lhMovers, iten) {
		// save its index in container
		*pstr<<iten->en_ulID;
	}}

	// write number of clients
	(*pstr)<<_pNetwork->ga_srvServer.srv_apltPlayers.Count();
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write to stream
		itclt->Write_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::WriteRememberedLevels_t(CTStream *pstr)
{
		// use writestream() !!!! @@@@

};

// remember current level
void CSessionState::RememberCurrentLevel(const CTString &strFileName)
{
	// if level is already remembered
	for(;;) {
		CRememberedLevel *prlOld = FindRememberedLevel(strFileName);
		if (prlOld==NULL) {
			break;
		}
		// remove it
		prlOld->rl_lnInSessionState.Remove();
		delete prlOld;
	}

	// create new remembered level
	CRememberedLevel *prlNew = new CRememberedLevel;
	ses_lhRememberedLevels.AddTail(prlNew->rl_lnInSessionState);
	// remember it
	prlNew->rl_strFileName = strFileName;
	WriteWorldAndState_t(&prlNew->rl_strmSessionState);
}

// find a level if it is remembered
CRememberedLevel *CSessionState::FindRememberedLevel(const CTString &strFileName)
{
	{FOREACHINLIST(CRememberedLevel, rl_lnInSessionState, ses_lhRememberedLevels, itrl) {
		CRememberedLevel &rl = *itrl;
		if (rl.rl_strFileName==strFileName) {
			return &rl;
		}
	}}
	return NULL;
}

// restore some old level
void CSessionState::RestoreOldLevel(const CTString &strFileName)
{
	// find the level
	CRememberedLevel *prlOld = FindRememberedLevel(strFileName);
	// it must exist
	ASSERT(prlOld!=NULL);
	// restore it
	try {
		prlOld->rl_strmSessionState.SetPos_t(0);
		_pTimer->SetCurrentTick(0.0f);
		ReadWorldAndState_t(&prlOld->rl_strmSessionState);
		_pTimer->SetCurrentTick(ses_tmLastProcessedTick);
	} catch (char *strError) {
		FatalError(TRANS("Cannot restore old level '%s':\n%s"), prlOld->rl_strFileName, strError);
	}
	// delete it
	delete prlOld;
}


// forget all remembered levels
void CSessionState::ForgetOldLevels(void)
{
	{FORDELETELIST(CRememberedLevel, rl_lnInSessionState, ses_lhRememberedLevels, itrl) {
		delete &*itrl;
	}}
}


//! 클라이언트에 로컬 타임을 적용한다.그리고 클라이언트 업데이트를 한다. 사용안함.
// this is used on client side of network game
// adjust the client's local game time and run client updates
void CSessionState::AdjustLocalTime() {
	static CTimerValue tvOld = _pTimer->GetHighPrecisionTimer(); 
	BOOL bPaused = _pNetwork->IsWaitingForPlayers() || ses_bPause;   
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	//! 데모 플레이 때만 TRUE
	if (ses_bRestartLocalTime) {
		ses_bRestartLocalTime = FALSE;
		tvOld = tvNow;
	}

	if (_pNetwork->IsGameFinished()) {
		return;
	}
	// how many ticks we have to run?
	ULONG ulNumTicks = (tvNow.GetSeconds() - tvOld.GetSeconds())/_pTimer->TickQuantum;
	//! ClampUp:높은것을 압박하다.즉, 둘중에 작은것을 선택한다.
	ULONG ulClampedTicks = ClampUp(ulNumTicks,(ULONG)10);
	//! 새로운 시간은 기억.
	// remember the new time
	tvOld += _pTimer->TickQuantum*ulNumTicks;

	if (ulClampedTicks>0 && !bPaused) {
		// advance local time for that many ticks
		_tmLocalTick += _pTimer->TickQuantum*ulNumTicks;
		// update the current tick
		if (_pNetwork->IsPlayingDemo()) {
			_pTimer->SetCurrentTick(_pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum);
		} else {
			_pTimer->SetCurrentTick(_tmLocalTick);
		}
		
		if (_cmiComm.cci_bClientInitialized) {
			INDEX ctTicks = 0;
			// for each tick that has passed
			while (ctTicks < ulClampedTicks && !_pNetwork->IsPlayingDemo()) {
				 //! 모든 로컬 플레이어에 대한 액션 패킷을 만들고 서버로 보낸다.
				// make actions packet for all local players and send to server
				_pNetwork->SendActionsToServer();
				ctTicks++;
			}
		}
		extern INDEX cli_iFPSLimit;
		extern INDEX  cli_iCurrentFPS;
		if (cli_iFPSLimit && (cli_iCurrentFPS < cli_iFPSLimit)) {
			SetLerpFactor(_pTimer->GetHighPrecisionTimer());
		}
		// do time events
		if (!_pNetwork->IsPlayingDemo()) {
			HandleTimers(_tmLocalTick);
			// do movement - dead reckoning
			HandleMovers_client();
		}
	}

}


void Cludge_SetLerpFactor() 
{
	// update the current tick
	{CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	if (!_pNetwork->ga_bDemoPlay) {
		_pNetwork->ga_sesSessionState.SetLerpFactor(tvNow);
	} else {
		_pNetwork->ga_sesSessionState.SetLerpFactor(CTimerValue(_pNetwork->ga_fDemoTimer));
	}}
}

/*
void CSessionState::RunDemoTick()
{
	static UBYTE aubData[SERVER_MAX_MESSAGE_SIZE];
	SLONG slSize,slCompressedSize;

	ses_bWaitingForServer = FALSE;
	 
	// if this is not an empty tick, read the data to the buffer
	extern UBYTE _aubCompData[SERVER_MAX_MESSAGE_SIZE];
	try {
		_pNetwork->ga_strmDemoPlay.ExpectID_t("DTCK");// demo tick       
		_pNetwork->ga_strmDemoPlay >> slCompressedSize;        
		if (slCompressedSize > 0) {          
			_pNetwork->ga_strmDemoPlay.Read_t(_aubCompData,slCompressedSize);
		}
		_pNetwork->ga_bDemoPlayFinished = FALSE;
	} catch(char *strError) {
		_pNetwork->ga_bDemoPlayFinished = TRUE;
		CPrintF("Error while playing demo: %s\n",strError);
		return;
	}
	slSize = SERVER_MAX_MESSAGE_SIZE;
	CzlibCompressor comp;
	BOOL bOK = comp.Unpack(_aubCompData,slCompressedSize,aubData,slSize);
	ASSERT(bOK);         

	// the compiler messes up the stack in debug if 64 bit integers and 
	// try-catch are used in the same function (stack scope). This is a fix-up cludge. 
	Cludge_SetLerpFactor();

	if (slCompressedSize > 0) {          
		// clear the input buffer
		ses_embReceiveBuffer.Clear();

		// put the tick data into the buffer
		ReceiveTick(_pNetwork->ga_tmNextDemoTick,aubData,slSize);
	
		_vBasePosition    = FLOAT3D(0,0,0);
		_bBasePositionSet = FALSE;

		// if the tick received isn't empty
		if (slSize > 0) {          
			// read and apply all received messages
			while (ReceiveMessage()){};
		}
	}
	
	// update session time
	ses_tmLastProcessedTick = _pNetwork->ga_fDemoTimer;//_tmLocalTick;         
	ses_tmLastTickTime = _pNetwork->ga_fDemoTimer;//_tmLocalTick;

	try {
		CChunkID cid = _pNetwork->ga_strmDemoPlay.PeekID_t();
		if (cid == CChunkID("DTTM")) {
			_pNetwork->ga_strmDemoPlay.ExpectID_t("DTTM");
			_pNetwork->ga_strmDemoPlay>>_pNetwork->ga_tmNextDemoTick;
		} else {
			_pNetwork->ga_strmDemoPlay.ExpectID_t("DEND"); // demo end
			_pNetwork->ga_bDemoPlayFinished = TRUE;
			return;
		}
	} catch(char *strError) {
		_pNetwork->ga_bDemoPlayFinished = TRUE;
		CPrintF("Error while playing demo: %s\n",strError);
	}
};
*/

//0105 함수 추가하기.
void CSessionState::ProcessOneSentence(char *msg)
{
	static char cmd[100];

	msg = AnyOneArg(msg, cmd);
	int cmd_num = m_pCmd->Find(cmd);
	if (cmd_num < 0)
		return;

	m_pCmd->Run(cmd_num, msg);

	return;
}

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))
char* CSessionState::AnyOneArg(char *argument, char *first_arg)
{
	SkipSpaces(&argument);

	while (*argument && !is_space(*argument)) {
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}


void CSessionState::SkipSpaces(char **string)
{
	for (; **string && is_space(**string); (*string)++);
}


void CSessionState::StartGame()
{

	char Buf[1024];

	sprintf(Buf, "start 0 0\n");

	_tcpip.SendNetMessage(Buf, TRUE);

	return;
}
//..

/* Session state loop. */
void CSessionState::SessionStateLoop(void)
{
	_pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SESSIONSTATE_LOOP);
	// while there is something to do
	BOOL bSomethingToDo = TRUE;
	while (bSomethingToDo && !IsDisconnected()) 
	{
		
		//Sleep(0);				// deleted by seo - 40830
		bSomethingToDo = FALSE;
		//! 클라이언트 업데이트
		_cmiComm.Client_Update();					
		CNetworkMessage nmMessage;
		static FLOAT ResponseTime = _pTimer->CurrentTick();

		extern BOOL _bLoginProcess;
		
		// ---- Add national code ----------060711------------->>
		extern INDEX g_iCountry;
		INDEX tv_idx;
		switch(g_iCountry)
		{
		case KOREA :
			tv_idx = 0;
			break;
		case TAIWAN : 
			tv_idx = 1;
			break;
		case CHINA : 
			tv_idx = 3;
			break;
		case THAILAND :
			tv_idx = 4;
			break;
		case TAIWAN2 :
			tv_idx = 2;
			break;
		case JAPAN : 
			tv_idx = 6;
			break;
		case MALAYSIA :
			tv_idx = 7;
			break;
		case USA :
			tv_idx = 9;
			break;
		case BRAZIL:
			tv_idx = 10;
			break;
		case HONGKONG:
			tv_idx = 11;
			break;
		case GERMANY:
			tv_idx = 13;
			break;
		case SPAIN:
			tv_idx = 14;
			break;
		case FRANCE:
			tv_idx = 15;
			break;
		case POLAND:
			tv_idx = 16;
			break;
		case RUSSIA:
			tv_idx = 17;
			break;
		case TURKEY:
			tv_idx = 18;
			break;
		}
		// ---- Add national code ----------060711-------------<<

		if(!_bLoginProcess)
		{
			static DWORD AliveTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static DWORD SendTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static BOOL bIsFirst = TRUE;
			
			DWORD tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			
			if(bIsFirst)
			{
				CNetworkMessage nmAliveMsg(MSG_PULSE); 	
				nmAliveMsg << (SLONG)(AliveTime/100);
				// ---- Add national code ----------060711------------->>
				nmAliveMsg << (UBYTE)tv_idx;			
				// ---- Add national code ----------060711-------------<<
				nmAliveMsg << (FLOAT)_pTimer->tmTimeDelay;
				_pNetwork->SendToServerNew(nmAliveMsg);	
				
				bIsFirst = FALSE;
			}
			
			if(tmpTime - AliveTime >= 60000) 
			{
				CNetworkMessage nmAliveMsg(MSG_PULSE); 	
				nmAliveMsg << (SLONG)(tmpTime/100);
				// ---- Add national code ----------060711------------->>
				nmAliveMsg << (UBYTE)tv_idx;			
				// ---- Add national code ----------060711-------------<<
				nmAliveMsg << (FLOAT)_pTimer->tmTimeDelay;
				_pNetwork->SendToServerNew(nmAliveMsg);	
				CPrintF("Send Alive MSG...%d \n",tmpTime);
				AliveTime = tmpTime;
			}
#ifdef RESTART_GAME
		   if (_pUIMgr->GetSystemMenu()->IsRestartGame())
		   {
				CTString strtime;
				DWORD IntervalTime;
				int iShowTime;
    
				IntervalTime = tmpTime - _pUIMgr->GetSystemMenu()->GetRestartTime();
				//iShowTime = 15 - IntervalTime/1000;
				iShowTime = 10 - IntervalTime/1000;

				//strtime.PrintF(_S(2514,"%d초"), iShowTime);
				strtime.PrintF(_S(4201,"%d 초 후에 접속종료됩니다."), iShowTime);

				_UIAutoHelp->SetGMNotice(strtime);

				if (IntervalTime >= 10000) // 15초  15000 -> 10초로 수정 
				{	// 재시작
					 _pUIMgr->GetSystemMenu()->SetRestartGameValue(FALSE);
					 _pUIMgr->GetSystemMenu()->ResetRestartTime();
					 //_pNetwork->SendRestartGame();
					 _pUIMgr->GetSystemMenu()->Restart_Internal();
				}
		   }
#endif
#ifdef UPDATE_PLAYTIME
			DWORD tmpDeffTime = tmpTime - _SetPlayStartTime;

			if (tmpDeffTime >= _IntervalPlayCountTime)
			{
				// 플레이 타임 업데이트
				++_PlayTimeCount;
				_pNetwork->SendUpdatePlayTime(_PlayTimeCount * _IntervalTime);
				_SetPlayStartTime = tmpTime;
			}
#endif
			// 싱글던젼 모드의 경우에...
			if( _pNetwork->m_bSingleMode )
			{
				if( tmpTime - SendTime >= 1000 )
				{
					CPrintF( "%ld SendAttack\n", tmpTime );
					//_pNetwork->SendMoveList();
					_pNetwork->SendAttackList();
					SendTime = tmpTime;
				}
			}

//			if (_pTimer->tmTimeDelay > 0.07f)
//			{ // 스피드 핵 사용자 알림 (TickCount는 0.05f가 기본이므로 0.05f보다 빨라진다면 스피드핵 사용자로 본다.
//				_pNetwork->SendUsingSpeedHack();
//			}

#ifdef AUCTION_SYSTEM
			if(bCalcNotChecked)
			{
				if(_pUIMgr->GetUIGameState() == UGS_GAMEON)
				{
					//정산 리스트 조회 요청
					_pNetwork->SendTradeAgentCheckCalcReq();
					bCalcNotChecked = FALSE;
				}
			}
#endif

		}

		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			bSomethingToDo = TRUE;//0820 이놈의 플래그 m때문에 네트워크가 늦었군..ㅡㅡ;
			ResponseTime = _pTimer->CurrentTick();
/*			//0818			
			static FLOAT AliveTime = _pTimer->CurrentTick();
			if(_pTimer->CurrentTick() - AliveTime > 60)
			{
				CNetworkMessage nmAliveMsg(MSG_TEST2); 					 
				_pNetwork->SendToServerNew(nmAliveMsg);	
				CPrintF("Send Alive MSG...%f \n",_pTimer->CurrentTick());
				AliveTime = _pTimer->CurrentTick();
			}
*/		

//			extern BOOL _bLoginProcess;
			if(_bLoginProcess)
			{				
				// NOTE : 로그인 시에는 무조건 아래 함수로 들어감.
				// NOTE : 캐릭터 생성 실패시 MSG_FAIL 다음에 캐릭터 슬롯 정보가 바로 오기 때문에
				// NOTE : 서브 타입까지 다 읽은 다음에 처리해야 함.
				ReceiveLoginMessage(&nmMessage);
			}
			else
			{
				switch(nmMessage.GetType())		
				{
				case MSG_APPEAR:
					ReceiveAppearMessage(&nmMessage);
					break;
				case MSG_DISAPPEAR:
					ReceiveDisappearMessage(&nmMessage);
					break;
				case MSG_AT:
					ReceiveAtMessage(&nmMessage);
					break;					
				case MSG_STATUS:
					ReceiveStatusMessage(&nmMessage);
					break;
				case MSG_MOVE:
					ReceiveMoveMessage(&nmMessage);
					break;
				case MSG_ATTACK:
					ReceiveAttackMessage(&nmMessage);
					break;
				case MSG_DAMAGE:
					ReceiveDamageMessage(&nmMessage);
					break;
				case MSG_CHAT:
					ReceiveChatMessage(&nmMessage);
					break;
				case MSG_SKILL:
					ReceiveSkillMessage(&nmMessage);
					break;
				case MSG_SSKILL:			// 특수 스킬
					ReceiveSSkillMessage(&nmMessage);
					break;
				case MSG_QUICKSLOT:
					ReceiveQuickSlotMessage(&nmMessage);
					break;
				case MSG_INVENTORY:
					ReceiveInventoryMessage(&nmMessage);
					break;
				case MSG_WEARING:
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_WEARING, &nmMessage);
					break;
				case MSG_ITEM:
					ReceiveItemMessage(&nmMessage);
					break;
				case MSG_GOTO:
					ReceiveGoToMessage(&nmMessage);
					break;
				case MSG_MEMPOS:
// [KH_070316] 프리미엄 메모리 관련 추가 및 변경
				case MSG_MEMPOSPLUS:
					ReceiveMemPosMessage(nmMessage.GetType(), &nmMessage);
					break;
				case MSG_GM:
					ReceiveGmMessage(&nmMessage);
					break;
				case MSG_GO_ZONE:
					ReceiveGoZoneMessage(&nmMessage);
					break;
				case MSG_PARTY:
					ReceivePartyMessage(&nmMessage);
					break;
				case MSG_EXCHANGE:
					ReceiveExchangeMessage(&nmMessage);
					break;
				case MSG_QUEST:
					ReceiveQuestMessage(&nmMessage);
					break;
				case MSG_STATPOINT:
					ReceiveStatMessage(&nmMessage);
					break;
				case MSG_UI:
					ReceiveUIMessage(&nmMessage);
					break;
				case MSG_NPC_REGEN:
					ReceiveNpcRegenMessage(&nmMessage);
					break;
				case MSG_ASSIST:
					ReceiveAssistMessage(&nmMessage);
					break;
				case MSG_CHAR_STATUS:
					ReceiveMobStatusMessage(&nmMessage);//1013
					break;
				case MSG_PC_REBIRTH:
					ReceiveRebirthMessage(&nmMessage);
					break;
				case MSG_EFFECT:
					ReceiveEffectMessage(&nmMessage);
					break;
				case MSG_SYS:
					ReceiveSystemMessage(&nmMessage);
					break;
				case MSG_EXP_SP:
					ReceiveEXPMessage(&nmMessage);
					break;
				case MSG_ACTION:
					ReceiveActionMessage(&nmMessage);
					break;
				case MSG_ENV://1013
					ReceiveEnvMessage(&nmMessage);
					break;
				case MSG_WARP:
					ReceiveWarpMessage(&nmMessage);
					break;
				case MSG_PK:
					ReceivePkMessage(&nmMessage);
					break;
				case MSG_EVENT:
					ReceiveEventMessage(&nmMessage);
					break;
				case MSG_PERSONALSHOP:
					ReceivePersonalShopMessage(&nmMessage);
					break;
				case MSG_RIGHT_ATTACK:
					ReceiveLegitimateMessage(&nmMessage);
					break;
				case MSG_STASH:
					ReceiveWareHouseMessage(&nmMessage);
					break;
				case MSG_GUILD:
					ReceiveGuildMessage(&nmMessage);
					break;
				case MSG_TEACH:
					ReceiveTeachMessage(&nmMessage);
					break;
				case MSG_CHANGEJOB:
					ReceiveChangeJobMessage(&nmMessage);
					break;
				case MSG_CHANGE:
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_CHANGE, &nmMessage);
					break;
				case MSG_BILLINFO:// Date : 2005-05-06(오후 9:21:44), By Lee Ki-hwan
					ReceiveBillingMessage(&nmMessage);
					break;
				case MSG_FRIEND:
					ReceiveMessengerMessage( &nmMessage );
					break;
				case MSG_TRADEAGENT:
					ReceiveTradeAgentMessage( &nmMessage );
					break;
				//wooss 050818
				case MSG_EXTEND:
					ReceiveExtendMessage( &nmMessage);
					break;
				case MSG_FAIL:
					ReceiveFailMessage(&nmMessage);
					break;

				default:
					CPrintF("Undefined data received!! msgType=%d \n" , nmMessage.GetType());
					break;
				}
			}
		}
		else
		{
			extern BOOL _bLoginProcess;
			if( _pNetwork->m_bSendMessage && timeGetTime() - _pNetwork->m_uiSendedTime > 20000 )
			{
				if( _bLoginProcess )
				{
					if( _pUIMgr->GetUIGameState() == UGS_LOGIN && !_pUIMgr->GetSelServer()->IsEmptyGroup() )
					{
						_pUIMgr->GetSelServer()->SetRecentServer(GetRecentGroup(), GetRecentServer());
						_pUIMgr->SetUIGameState( UGS_SELSERVER );
						_pNetwork->m_bSendMessage = FALSE;
					}
					else
					//else if(_cmiComm.IsNetworkOn())
					{
						if( !_tcpip.IsConnected(_tcpip.Socket) )
						{
							_pGameState->DisplayErrorMessage( MSG_FAIL_CONNECT_SERVER, UI_NONE, MSGCMD_LOGIN_ERROR );
						}
						else
						{
							_pGameState->DisplayErrorMessage( MSG_TIME_OUT, UI_NONE, MSGCMD_LOGIN_ERROR );
						}
						_pNetwork->m_bSendMessage = FALSE;
					}
				}
			}
		}

		// if there is some unreliable message
		if (_pNetwork->ReceiveFromServer(nmMessage))
		{
			bSomethingToDo = TRUE;
			//! 게임 스트림 메시지라면, 언팩후에 읽어들인다.로컬만 받는다.
			// if it is a gamestream message
			if (nmMessage.GetType() == MSG_GAMESTREAMBLOCKS)
			{
				ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
				ses_bWaitingForServer = FALSE;
				
				// unpack the message
				CNetworkMessage nmUnpackedBlocks(MSG_GAMESTREAMBLOCKS);
				nmMessage.UnpackDefault(nmUnpackedBlocks);
				
				// while there are some more blocks in the message
				while (!nmUnpackedBlocks.EndOfMessage()) 
				{
					// read a block to the gamestream
					ses_nsGameStream.ReadBlock(nmUnpackedBlocks);
				}			
			}
			else 
			{
				CPrintF(TRANS("Session state: Unexpected unreliable message during game: %s(%d)\n"),
					ErrorDescription(&MessageTypes, nmMessage.GetType()), nmMessage.GetType());
			}
		}
	}
	_pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SESSIONSTATE_LOOP);
}

/* Get number of active players. */
INDEX CSessionState::GetPlayersCount(void)
{
	INDEX ctPlayers = 0;
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itplt) {
		if (itplt->IsActive()) {
			ctPlayers++;
		}
	}
	return ctPlayers;
}


// probability (0.0 - 1.0) that the client will drop the tick data (bad network conditions emulation)
extern FLOAT cli_fTickDropRate;


// receive a message from the incoming tick data
BOOL CSessionState::ReceiveMessage() 
{
	ULONG ulEntityID;
	UWORD aubData[1024];
	int   iErr;
	
	extern INDEX net_bReportClientTraffic;
	
	// read the message from the input buffer
	iErr = ses_embReceiveBuffer.ReadMessage(ses_emEntityMessage);
	
	if (iErr != EMB_SUCCESS_OK) 
	{
		return FALSE;
	} 
	
	// if this message is older than the last fully processed server tick, do not process it
	if ((ses_tmLastTickTime > (ses_emEntityMessage.em_tmMessageTime - (_pTimer->TickQuantum/2))) && !_pNetwork->IsPlayingDemo()) 
	{
		if (net_bReportClientTraffic) 
		{
			//0203 
			//	CPrintF(TRANS("MESSAGE REJECTED! Type: %d, Session time: %5.2f, Message time: %5.2f\n"),ses_emEntityMessage.em_ulType,ses_tmLastTickTime,ses_emEntityMessage.em_tmMessageTime);
		}
		return TRUE;
	}
	
	switch (ses_emEntityMessage.em_ulType) 
	{
		// if adding a new player
	case EMT_ADDPLAYER: 
		{
			INDEX iPlayerIndex,iClient;
			ULONG ulWeaponsID,ulAnimatorID;
			CPlayerCharacter pcCharacter;
			ses_ulMsgId++;
			CEntity* penPlayer;        
			
			// extract neccessary data from the message
			ses_emEntityMessage.ReadAddPlayer(ulEntityID,iPlayerIndex,pcCharacter,ulWeaponsID,ulAnimatorID,iClient);    
			
			if (_pNetwork->ga_World.EntityExists(ulEntityID,penPlayer)) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF("Warning: Repeated AddPlayer, EntityID: 0x%X\n",ulEntityID);
				}
				break;
			}
			
			// add the player to the game, forcing it to have a received entity ID for CPlayer, CPlayerWeapons and CPlayerAnimator
			// this is because we will need to process messages specifying those ID's later
			AddPlayer(iPlayerIndex,pcCharacter,ulEntityID,ulWeaponsID,ulAnimatorID,iClient);              
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				CPrintF("ADDPLAYER:     %s  EntityID: 0x%X   %6.3f\n  ",(const char*)pcCharacter.pc_strName,ulEntityID,ses_emEntityMessage.em_tmMessageTime);
			}
			
			break;
		}
		//0203 
		/*
		// new placement received for an entity
		case EMT_SETPLACEMENT: 
		{
		CEntity* penEntity;
		CPlacement3D plPlacement,plSpeed;			
		ses_ulMsgId++;		
		//        ASSERT(_bBasePositionSet);
		
		  // extract neccessary data from the message
		  ses_emEntityMessage.ReadPlacement(ulEntityID,plPlacement,plSpeed);      
		  plPlacement.pl_PositionVector += _vBasePosition;
		  
			// if placement received is for an entity that does not exist on the client, 
			// print a warning to the console, and return
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
			CPrintF("Warning: SetPlacement for non existing entity, EntityID: 0x%X\n",ulEntityID);
			}
			break;
			}
			
			  // get the pointer to the specified entity
			  penEntity = _pNetwork->ga_World.EntityFromID(ulEntityID);       
			  
				// it it's a movable entity, adjust it's dead reckoning parameters - position, speed and extrapolation start time
				if (penEntity->en_ulPhysicsFlags & EPF_MOVABLE) {
				CMovableEntity* penMovable = (CMovableEntity*) penEntity;
				if (plSpeed.pl_OrientationAngle.Length() > 0.01 || plSpeed.pl_PositionVector.Length() > 0.01) {
				penMovable->AddToMovers();
				} else {
				if (penMovable->en_lnInMovers.IsLinked()) {
				penMovable->en_lnInMovers.Remove();
				}
				}
				penMovable->AdjustDeadReckoning(plPlacement,plSpeed,ses_emEntityMessage.em_tmMessageTime);
				// if it's not a movable entity, just put it in it's place
				} else {
				penEntity->SetPlacement(plPlacement);
				}
				
				  // report to console
				  if (net_bReportClientTraffic) {
				  CPrintF("%lu: SETPLACEMENT:     EntityID: 0x%X     Position: %f, %f, %f     ",ses_ulMsgId,ulEntityID,plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
				  CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
				  }
				  break;
	}*/
		// a new entity should be created on the client
	case EMT_CREATE: 
		{
			UWORD uwClassID;																								 
			CPlacement3D plPlacement;
			CEntity* penEntity;
			EntityClassEntry* peceEntry;
			ses_ulMsgId++;
			
			// get the entity id, placement and class id
			ses_emEntityMessage.ReadEntityCreate(ulEntityID,plPlacement,uwClassID);
			
			// get the pointer to the entity class entry in the class container
			peceEntry = FindClassInContainer(uwClassID);
			// it really should be there
			ASSERT(peceEntry != NULL);
			if (peceEntry == NULL) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF(TRANS("Warning: Create entity received for an unknown entity class, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			// create the entity with specified data
			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement,peceEntry->ece_fnmEntityClass,ulEntityID);
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				CPrintF("Class name: %s\n",peceEntry->ece_fnmEntityClass);          
				CPrintF("%lu: CREATE:           EntityID: 0x%X     EntityClass: %ld",ses_ulMsgId,ulEntityID,uwClassID);
				CPrintF("     Position: %f, %f, %f     ",plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
				CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
			}
			
			break;
			
		}
		// copy one entity to another
	case EMT_COPY: 
		{
			ULONG ulTargetID;
			CPlacement3D plPlacementCopy;
			UBYTE ubWithDescentants = 0;
			CEntity* penOriginal;
			//        CEntity* penCopy;
			
			ses_ulMsgId++;
			
			// extract neccessary data from the message
			ses_emEntityMessage.ReadEntityCopy(ulEntityID,ulTargetID,plPlacementCopy,ubWithDescentants);
			
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penOriginal)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF("Warning: Copy from non existing entity, EntityID: 0x%X\n",ulEntityID);
				}
				break;
			}
			ASSERT(penOriginal!=NULL);
			if (penOriginal == NULL) {
				break;
			}
			
			_pNetwork->ga_World.CopyEntityInWorld(*penOriginal,plPlacementCopy,ubWithDescentants,ulTargetID);
			
			// report to console
			if (net_bReportClientTraffic) {
				CPrintF("%lu: COPY:           SourceEntityID: 0x%X     TargetEntityID: 0x%X     ",ses_ulMsgId,ulEntityID,ulTargetID);
				CPrintF("     Position: %f, %f, %f     ",plPlacementCopy.pl_PositionVector(1),plPlacementCopy.pl_PositionVector(2),plPlacementCopy.pl_PositionVector(3));
				CPrintF("Angle: %f, %f, %f\n",plPlacementCopy.pl_OrientationAngle(1),plPlacementCopy.pl_OrientationAngle(2),plPlacementCopy.pl_OrientationAngle(3));
			}
			
			break;
		}
		// entity needs to be destroyed
	case EMT_DESTROY: 
		{
			CEntity* penEntity;      
			
			ses_ulMsgId++;
			
			// get the entity id from themessage
			ses_emEntityMessage.ReadEntityDestroy(ulEntityID);
			
			// get the pointer to the entity
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: destroy message received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			ASSERT(penEntity != NULL);
			if (penEntity == NULL) break;
			// destroy the entity - this might not delete it from memory, depending on the number of references to it
			penEntity->Destroy();
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				//0205
				//			CPrintF("%lu: DESTROY:        EntityID: 0x%X\n", ses_ulMsgId,ulEntityID);
			}
			
			break;
		}
		
		// an event has been received for an entity (i.e. Explode, Damage, Die...)
	case EMT_EVENT: 
		{
			SLONG slDataSize;
			SLONG slEventCode;				
			UWORD uwClassID;
			UWORD uwEventID;
			EntityClassEntry* peceEntry;
			CEntityEvent  *peeEvent;
			CEntity* penEntity;
			
			ses_ulMsgId++;
			// get the entity ID, event code and event data from the message
			ses_emEntityMessage.ReadEntityEvent(ulEntityID,slEventCode,(void*) aubData,slDataSize);
			// extract the entity class id and event id
			// an event is uniquely identified only if both of these ids are supplied
			uwEventID = slEventCode & 0x0000FFFF;
			uwClassID = (slEventCode & 0xFFFF0000) >> 16; 
			
			// get the pointer to the entity class entry in the class container
			peceEntry = FindClassInContainer(uwClassID);
			// it really should be there
			ASSERT(peceEntry != NULL);
			if (peceEntry==NULL) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: Event received for an unknown entity class, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			// report to console
			if (net_bReportClientTraffic) {
				//			CPrintF("%lu: EVENT:            EntityID: 0x%X     EventCode: 0x%X     DataSize: %d\n",ses_ulMsgId,ulEntityID,slEventCode,slDataSize);
			}
			
			CEntityClass *pecClass;
			
			// obtain the class from the stock
			try 
			{
				pecClass = _pEntityClassStock->Obtain_t(peceEntry->ece_fnmEntityClass);
			} 
			catch (char * strError) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF(TRANS("Warning: Unable to obtain entity class from stock, class name: %s\n"),peceEntry->ece_fnmEntityClass);
					CPrintF("%s\n",strError);
				}
				break;
			}
			BOOL bFound = FALSE;
			
			// try to find the event in the event table for this class, and when found, create a new empty event
			for (int iEvent=0;iEvent<pecClass->ec_pdecDLLClass->dec_ctEvents;iEvent++) {
				if (slEventCode == pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_slEvent) {
					peeEvent = pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_New();
					bFound=TRUE;
					break;
				}
			}
			// if must be there
			ASSERT (bFound);
			
			if (!bFound) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: Unable to find requested entity event, class name: %s, event: %d\n"),peceEntry->ece_fnmEntityClass,uwEventID);
				}
				_pEntityClassStock->Release(pecClass);
				break;
			}
			
			// fill in the event data
			memcpy(((UBYTE*)peeEvent) + sizeof(CEntityEvent),aubData,slDataSize);
			
			// get the pointer to the entity
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
				}
				_pEntityClassStock->Release(pecClass);
				delete (UBYTE*)peeEvent;
				break;
			}
			
			if (penEntity->IsFlagOn(ENF_DELETED)) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: event received for deleted entity, EntityID: 0x%X\n"),ulEntityID);
				}
				_pEntityClassStock->Release(pecClass);
				delete (UBYTE*)peeEvent;
				break;
			}
			
			// sent the event to the entity
			penEntity->SendEvent(*peeEvent);
			
			// handle the event
			CEntity::HandleSentEvents();
			
			
			
			// release the entity class from the stock (remove a reference)
			_pEntityClassStock->Release(pecClass);
			
			delete (UBYTE*)peeEvent;
			
			break;
			}
			// an entity should be initialized
		case EMT_INITIALIZE: 
			{
				SLONG slDataSize;
				SLONG slEventCode;
				EntityClassEntry* peceEntry;
				CEntityEvent  *peeEvent;
				CEntity* penEntity;
				UWORD uwClassID;
				UWORD uwEventID;
				
				ses_ulMsgId++;
				
				// get the entity ID, event code and event data from the message
				ses_emEntityMessage.ReadEntityInitialize(ulEntityID,slEventCode,(void*) aubData,slDataSize);
				// extract the entity class id and event id
				// an event is uniquely identified only if both of these ids are supplied
				uwEventID = slEventCode & 0x0000FFFF;
				uwClassID = (slEventCode & 0xFFFF0000) >> 16; 
				// get the pointer to the entity class entry in the class container
				peceEntry = FindClassInContainer(uwClassID);
				// it isn't neccessary to get a non-null pointer here - it will be null for an event
				// from 'BaseEvents.h'
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("%lu: INITIALIZE:            EntityID: 0x%X     EntityClass: %d      EventID: %d     DataSize: %d\n",ses_ulMsgId,ulEntityID,uwClassID,uwEventID,slDataSize);
				}
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: initialize received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				EVoid eVoidEvent;
				// if this is en EVoid, initialize with default
				if (slEventCode == eVoidEvent.ee_slEvent) {
					penEntity->Initialize();
				} 
				else 
				{
					// if it's not an EVoid, check if we got a valid class entry
					ASSERT(peceEntry != NULL);
					
					CEntityClass *pecClass;
					
					try 
					{
						pecClass = _pEntityClassStock->Obtain_t(peceEntry->ece_fnmEntityClass);
					} catch (char * strError){
						extern INDEX net_bReportMiscErrors;
						if (net_bReportMiscErrors) {
							CPrintF(TRANS("Warning: Unable to obtain entity class from stock, class name: %s\n"),peceEntry->ece_fnmEntityClass);
							CPrintF("%s\n",strError);
						}
						break;
					}
					
					BOOL bFound = FALSE;
					
					// find the event in this class, and create a new empty event
					for (int iEvent=0;iEvent<pecClass->ec_pdecDLLClass->dec_ctEvents;iEvent++) {
						if (slEventCode == pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_slEvent) {
							peeEvent = pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_New();
							bFound=TRUE;              
							break;
						}
					}
					// it must be there
					ASSERT (bFound);
					
					if (!bFound) 
					{
						extern INDEX net_bReportMiscErrors;
						if (net_bReportMiscErrors) {
							CPrintF(TRANS("Warning: Unable to find requested entity event, class name: %s, event: %d\n"),peceEntry->ece_fnmEntityClass,uwEventID);
						}
						_pEntityClassStock->Release(pecClass);
						break;
					}
					
					// fill in the data
					memcpy(((UBYTE*)peeEvent) + sizeof(CEntityEvent),aubData,slDataSize);
					
					// initialize the entity
					penEntity->Initialize(*peeEvent);
					
					// release the class (remove reference)
					_pEntityClassStock->Release(pecClass);
					
					delete (UBYTE*)peeEvent;
				}
				
				break;
			}
			// make an entity start playing an animation
		case EMT_STARTANIM: 
			{
				CEntity* penEntity;
				INDEX iNewModelAnim;
				ULONG ulFlags;
				
				ses_ulMsgId++;
				// get the entity id, animation index and flags
				ses_emEntityMessage.ReadStartAnim(ulEntityID,iNewModelAnim,ulFlags);
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Start anim (not ska) received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				// do not do this for local players
				if (penEntity->IsPlayer() && (_pNetwork->IsPlayerLocal(penEntity))) { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Start anim (not ska) received for local player, EntityID: 0x%X\n"),ulEntityID);
					}
					break; 
				};
				
				// play the animation 
				penEntity->StartModelAnim(iNewModelAnim,ulFlags);
				
				// report to the console
				if (net_bReportClientTraffic) {
					CPrintF("STARTANIM:            EntityID: 0x%X     NewAnim: %ld     Flags: 0x%X\n",ulEntityID,iNewModelAnim,ulFlags);
				}
				
				break;
			}
			// update for player data
		case EMT_PLAYERUPDATE: 
			{
				CPlayerEntity* penPlayerEntity;        
				CNetworkMessage nmMessage;
				CEntity* penEntity;
				
				ses_ulMsgId++;
				// get the player entity id
				ses_emEntityMessage.ReadPlayerUpdate(ulEntityID,nmMessage);
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: update received for non existing player!\n"));
					}
					break;
				}
				ASSERT (penEntity->IsPlayer());
				if (!penEntity->IsPlayer()) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player update received with non-player entity id !\n"));
					}
					break;
				}
				
				// the pointer to the player entity
				penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// report to the console
				if (net_bReportClientTraffic) {
					//0203
					//				CPrintF("PLAYERUPDATE:            EntityID: 0x%X \n",ulEntityID);
				}      
				
				// find the PlayerSource for this entity - will produce a non null pointer only for local players
				CPlayerSource* pplsSource = _pNetwork->GetPlayerSource(penPlayerEntity);
				// if it's a local player and the game isn't paused
				// if it's a local player, read it's data and reapply local prediction
				if (_pNetwork->IsPlayerLocal(penEntity) || _pNetwork->IsPlayingDemo()) {
					if (!_pNetwork->IsPlayingDemo()) {
						ASSERT(pplsSource != NULL);
						if (pplsSource == NULL) {
							extern INDEX net_bReportMiscErrors;
							if (net_bReportMiscErrors) {
								CPrintF(TRANS("Warning: player source for local player does not exist!\n"));
							}
							break;
						}
					}//0202*/
					//0130 5line 지우기
					/*				
					penPlayerEntity->Read_net(&nmMessage,ses_emEntityMessage.em_tmMessageTime);
					
					  if (!_pNetwork->IsPlayingDemo()) {
					  // apply all actions not executed by the server - local prediction
					  pplsSource->ApplyNewerActions(penPlayerEntity->m_iLastActionTime);          
					  }
					*/				
					// if it's no a local player, read data and apply dead reckoning
					//0202/*	
				}
				/*
				else 
				{		
				static bFirst=TRUE;
				if(bFirst){				
				penPlayerEntity->Read_net_brief(&nmMessage,ses_emEntityMessage.em_tmMessageTime);			
				bFirst=FALSE;
				CPrintF(TRANS("Read_net_brief!\n"));
				}
				
				  }
				*/
				
				
				break;
			}
			/*
			// read base placement
			case EMT_BASEPLACEMENT: 
			{
			ses_emEntityMessage.ReadBasePosition(_vBasePosition);
			_bBasePositionSet = TRUE;
			
			  // report to console
			  if (net_bReportClientTraffic) {
			  CPrintF("BASEPLACEMENT:           Position: %f, %f, %f\n",_vBasePosition(1),_vBasePosition(2),_vBasePosition(3));
			  }
			  
				break;
								}*/
			
			// ska animation event
		case EMT_SKA_ANIM: 
			{
				CEntity* penEntity;        
				UBYTE ubSkaCode;
				SLONG slDataSize = ESKA_MAX_DATA_SIZE;
				static UBYTE aubSkaData[ESKA_MAX_DATA_SIZE];
				UBYTE* pubData = aubSkaData;
				
				ses_ulMsgId++;
				// get the entity id
				ses_emEntityMessage.ReadSka(ulEntityID,ubSkaCode,aubSkaData,slDataSize);
				// the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity))  { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: SKA event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);            
					}
					break;
				}
				//penEntity = _pNetwork->ga_World.EntityFromID(ulEntityID);
				// if it fails in the release version, don't crash ;)
				if (penEntity == NULL) break;
				if (penEntity->en_RenderType != CEntity::RT_SKAMODEL && penEntity->en_RenderType != CEntity::RT_SKAEDITORMODEL) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: SKA event received for non SKA entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				switch (ubSkaCode) 
				{
				case EM_SKA_ADD: 
					{
						INDEX iAnimID,iLocalAnimID,iGroupID,iLocalGroupID,iModelInstanceID,iLocalModelInstanceID;
						ULONG ulFlags;
						FLOAT fStrength,fSpeedMul;
						extern CStaticStackArray<INDEX> _aiNetSkaIDConversion;
						
						//				ska_StringToID("Windwalker");
						
						iAnimID   = *(INDEX*)pubData; pubData += sizeof(iAnimID);
						ulFlags   = *(ULONG*)pubData; pubData += sizeof(ulFlags);
						fStrength = *(FLOAT*)pubData; pubData += sizeof(fStrength);
						iGroupID  =  *(INDEX*)pubData; pubData += sizeof(iGroupID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						fSpeedMul = *(FLOAT*)pubData;
						
						iLocalAnimID = _aiNetSkaIDConversion[iAnimID];
						iLocalGroupID = _aiNetSkaIDConversion[iGroupID];
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->AddAnimation(iLocalAnimID,ulFlags,fStrength,iLocalGroupID,iLocalModelInstanceID,fSpeedMul);
						
						break;
					}
				case EM_SKA_REM: 
					{
						INDEX iAnimID,iLocalAnimID,iModelInstanceID,iLocalModelInstanceID;
						
						iAnimID   = *(INDEX*)pubData; pubData += sizeof(iAnimID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						iLocalAnimID = _aiNetSkaIDConversion[iAnimID];              
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->RemAnimation(iLocalAnimID,iLocalModelInstanceID);
						
						break;
					}
				case EM_SKA_REMID: 
					{
						INDEX iGroupID,iModelInstanceID,iLocalModelInstanceID;
						
						iGroupID   = *(INDEX*)pubData; pubData += sizeof(iGroupID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->RemAnimsWithID(iGroupID,iLocalModelInstanceID);
						
						break;
					}
				case EM_SKA_NEWCLONED: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT fFadeTime;
						
						fFadeTime = *(FLOAT*)pubData; pubData += sizeof(fFadeTime);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->NewClonedState(fFadeTime,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_NEWCLEAR: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT fFadeTime;
						
						fFadeTime = *(FLOAT*)pubData; pubData += sizeof(fFadeTime);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->NewClearState(fFadeTime,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_STRETCH: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT3D vStretch;
						
						vStretch(1) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(2) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(3) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->StretchModel(vStretch,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_STRETCHSINGLE: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT3D vStretch;
						
						vStretch(1) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(2) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(3) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->StretchSingleModel(vStretch,iLocalModelInstanceID);    
						
						break;
					}
				default: 
					{
						ASSERT(FALSE);
					}					
				}
				break;
			}
			// pause/unpause game
		case EMT_PAUSE : 
			{
				CTString strPauser;
				BOOL bPauseBefore = ses_bPause;
				
				ses_ulMsgId++;
				// read the new pause state
				ses_emEntityMessage.ReadPause(ses_bPause,strPauser);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("PAUSE.\n");
				}
				
				// report who paused
				if (ses_bPause!=bPauseBefore) {
					if (ses_bPause) {
						CPrintF(TRANS("Paused by '%s'\n"), strPauser);
					} else {
						CPrintF(TRANS("Unpaused by '%s'\n"), strPauser);
					}
				}
				
				// must keep wanting current state
				ses_bWantPause = ses_bPause;
				
				break;
			}///*
		case EMT_REMPLAYER: 
			{
				INDEX iPlayer;
				
				ses_ulMsgId++;
				// read the new pause state
				ses_emEntityMessage.ReadRemovePlayer(iPlayer);
				
				// report to console
				//if (net_bReportClientTraffic) {
				CPrintF("REMPLAYER:     %s   index: %ld\n",(const char*)_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_pcCharacter.pc_strName,iPlayer);
				//}
				
				CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];
				if (plt.plt_penPlayerEntity == NULL) { 
					if (plt.IsActive()) {
						_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
					}
					return SUCCESS_OK; 
				}
				
				// inform entity of disconnnection
				CPrintF(TRANS("%s left\n"), _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->GetPlayerName());
				_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->Disconnect();
				// deactivate the player
				_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
				// handle all the sent events
				ses_bAllowRandom = TRUE;
				CEntity::HandleSentEvents();
				ses_bAllowRandom = FALSE;				
				
				break;
			}
			//0205	
			/*
			case EMT_PLAYER_MOVE:{
			
			  
				CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];	
				
				  plt->plt_penPlayerEntity->ClearMovingTemp();
				  plt->plt_penPlayerEntity->PreMoving();
				  plt->plt_penPlayerEntity->DoMoving();
				  plt->plt_penPlayerEntity->PostMoving();
				  plt->plt_penPlayerEntity->ClearMovingTemp();
				  CEntity::HandleSentEvents();
				  
					plt->ApplyActionPacket(paAction);
					plt->plt_penPlayerEntity->SetActionApplied();
					
					  break;
					  }
			*/
			////////////////////////////////						
			
		case EMT_CHANGECHARACTER: 
			{
				CPlayerCharacter pcCharacter;
				CEntity* penEntity;
				
				ses_ulMsgId++;
				
				// extract neccessary data from the message
				ses_emEntityMessage.ReadcharacterChange(ulEntityID,pcCharacter);    
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: Change character received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				CPlayerEntity* penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// change the character
				penPlayerEntity->CharacterChanged(pcCharacter);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("%lu: CHARACTERCHANGE:     EntityID: 0x%X\n",ulEntityID);
				}
				
				break;
			}
			
			// sound animation event
		case EMT_SOUND: 
			{
				CEntity* penEntity;        
				UBYTE ubSoundCode;
				SLONG slDataSize = ESOUND_MAX_DATA_SIZE;
				static UBYTE aubSoundData[ESOUND_MAX_DATA_SIZE];
				UBYTE* pubData = aubSoundData;
				ULONG ulComponentOffset;
				CSoundObject *psoSoundObject;
				
				ses_ulMsgId++;
				// get the entity id
				ses_emEntityMessage.ReadSound(ulEntityID,ubSoundCode,aubSoundData,slDataSize);
				// the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity))  { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Sound event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);            
					}
					break;
				}
				
				
				
				switch( ubSoundCode)
				{
				case EM_SOUND_PLAY: 
					{
						SLONG idSoundComponent;
						SLONG slPlayType;
						UBYTE ubThis;
						ULONG ulOwnerID;
						CEntity* penSoOwner = penEntity;
						
						ubThis = *(UBYTE*)pubData; pubData += sizeof(ubThis);
						if (!ubThis) {
							ulOwnerID = *(ULONG*)pubData; pubData += sizeof(ulOwnerID);
							if (!_pNetwork->ga_World.EntityExists(ulOwnerID,penSoOwner))  { 
								extern INDEX net_bReportMiscErrors;
								if (net_bReportMiscErrors) {
									CPrintF(TRANS("Warning: Sound event received for non existing entity, OwnerID: 0x%X\n"),ulOwnerID);            
								}
								break;
							}
						}
						if (penEntity->IsPlayer() && _pNetwork->IsPlayerLocal(penEntity)) {
							break;
						}
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						idSoundComponent = *(SLONG*)pubData;   pubData += sizeof(idSoundComponent);
						slPlayType = *(SLONG*)pubData; pubData += sizeof(slPlayType);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penSoOwner) + ulComponentOffset);
						penEntity->PlaySound(*psoSoundObject,idSoundComponent,slPlayType); 
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: SOUND_PLAY:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_STOP: 
					{
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->Stop();
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_Stop:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETVOLUME: 
					{
						ulComponentOffset = *(ULONG*)pubData;    pubData += sizeof(ulComponentOffset);
						FLOAT fVolume = ((FLOAT)(*(UBYTE*)pubData))/127.0f;  pubData += sizeof(UBYTE);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetVolume(fVolume);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETVOLUME:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETPITCH: 
					{
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						FLOAT fPitch = ((FLOAT)(*(UBYTE*)pubData))/63.0f;  pubData += sizeof(UBYTE);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetPitch(fPitch);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETPITCH:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETRANGE: 
					{
						ulComponentOffset = *(ULONG*)pubData;   pubData += sizeof(ulComponentOffset);
						FLOAT fHotSpot = ((FLOAT)(*(UWORD*)pubData))/5.0f;  pubData += sizeof(UWORD);
						FLOAT fFallOff = ((FLOAT)(*(UWORD*)pubData))/5.0f;  pubData += sizeof(UWORD);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetRange( fHotSpot, fFallOff);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETRANGE:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_FADE: 
					{
						ulComponentOffset = *(ULONG*)pubData;    pubData += sizeof(ulComponentOffset);
						FLOAT fVolume = ((FLOAT)(*(UBYTE*)pubData))/127.0f;  pubData += sizeof(UBYTE);
						TIME tmPeriod = *((TIME*)pubData);  pubData += sizeof(TIME);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->Fade(fVolume,tmPeriod);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_FADE:     EntityID: 0x%X, period: %7.4f, volume: %7.4f\n",ses_ulMsgId,ulEntityID,tmPeriod,fVolume);
						}
						break;
					}
					
				default: 
					{
						ASSERT(FALSE);
					}
				}
				
				break;
			}
			// add a new string to ska string table & net coversion table
		case EMT_ADDSKASTRING : 
			{
				CTString strString;
				INDEX iServerIndex;
				
				ses_ulMsgId++;
				// read server ska ID and ska string to be added
				ses_emEntityMessage.ReadAddSkaString(iServerIndex,strString);
				
				// add it to the local table
				INDEX iLocalIndex = ska_GetIDFromStringTable(strString);
				
				ASSERT(_aiNetSkaIDConversion.Count() == iServerIndex);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("EMT_ADDSKASTRING:  String  '%s' added to ska table, Local ID is:  %ld, Server ID is: %ld\n",(const char*)strString,iLocalIndex,iServerIndex);
				}
				
				// expand the conversion array, and make a new entry
				INDEX &iNewIndex = _aiNetSkaIDConversion.Push();
				iNewIndex = iLocalIndex;
				
				break;
			}
		case EMT_SETPLACEMENT_NOTIFY: 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: server received EMT_SETPLACEMENT_NOTIFY\n"));
				}
				break;
			}
			// update for player data
		case EMT_PLAYERACTION: 
			{
				CPlayerEntity* penPlayerEntity;        
				CPlayerAction paAction;
				CEntity* penEntity;
				
				ASSERT(_pNetwork->IsPlayingDemo());
				
				ses_ulMsgId++;
				// get the player entity id
				ses_emEntityMessage.ReadPlayerAction(ulEntityID,paAction);
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player action for non existing player!\n"));
					}
					break;
				}
				ASSERT (penEntity->IsPlayer());
				if (!penEntity->IsPlayer()) 
				{
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player action received with non-player entity id !\n"));
					}
					break;
				}
				
				// the pointer to the player entity
				penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// report to the console
				if (net_bReportClientTraffic) {
					CPrintF("PLAYERACTION:            EntityID: 0x%X \n",ulEntityID);
				}      
				
				// apply this action
				penPlayerEntity->ApplyAction(paAction,0);
				
				// run all player updates
				penPlayerEntity->ClearMovingTemp();
				penPlayerEntity->PreMoving();
				penPlayerEntity->DoMoving(); 
				penPlayerEntity->PostMoving();
				penPlayerEntity->ClearMovingTemp();
				
				if (penPlayerEntity->en_penParent!=NULL && penPlayerEntity->en_penParent->GetPhysicsFlags()&EPF_MOVABLE) 
				{
					penPlayerEntity->en_plLastPlacement.pl_PositionVector =     penPlayerEntity->en_plPlacement.pl_PositionVector - 
						((CMovableEntity*)(penPlayerEntity->en_penParent))->en_vCurrentTranslationAbsolute*_pTimer->TickQuantum;
				}
				
				break;				
			}
			//    default: {
			//        ASSERT(FALSE);
			//        break;
			//			 }
	}	
	return TRUE;	
};


// receive new tick data from the server
void CSessionState::ReceiveTick(TIME tmTickTime,UBYTE* pubData,SLONG slSize) 
{
	
	ASSERT (pubData !=NULL);
	ASSERT (slSize >= 0);
	float fRandom;
	
	extern INDEX net_bReportClientTraffic;
	
	// emulate bad network conditions
	fRandom = rand() / (float) RAND_MAX;  
	// if tick data is newer than the last data received, and it shouldn't be dropped
	if (ses_tmLastTickTime < tmTickTime && fRandom >= cli_fTickDropRate) {
		// report to console
		if (net_bReportClientTraffic) {
			//0203      CPrintF(TRANS("Client reading tick, time: %5.2f\n"),tmTickTime);
		}
		// put the data into the input buffer
		ses_embReceiveBuffer.WriteTick(tmTickTime,pubData,slSize);       
		// send tick acknowledge to server
		if (!_pNetwork->IsPlayingDemo()) {
			SendAcknowledge(tmTickTime);
		}
		// if tick data is older than last received, just send the acknowledge
	} else if (fRandom >= cli_fTickDropRate) { 
		if (!_pNetwork->IsPlayingDemo()) {
			SendAcknowledge(tmTickTime);
		}
	}
	
};


// acknowledge that tick data for a certian tick time has been received
void CSessionState::SendAcknowledge(float fTickTime)
{
	extern INDEX net_bReportClientTraffic;

	// report to console
	if (net_bReportClientTraffic) {
//0203    CPrintF(TRANS("Send acknowledge: %5.2f\n"), fTickTime);
	}
/* //0522 kwon 삭제.
	// create a message, and send it to the server
	CNetworkMessage nmMessage(MSG_TICK_ACKNOWLEDGE);
	nmMessage << fTickTime;
	_pNetwork->SendToServer(nmMessage);
*/
};

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::SwapItem( int tab, int row, int col, int tab2, int row2, int col2 )
{
	CItems ItemsTmp;
	CItems& Items	= _pNetwork->MySlotItem[tab][row][col];
	CItems& Items2	= _pNetwork->MySlotItem[tab2][row2][col2];

	ItemsTmp.ItemData = Items.ItemData;
	ItemsTmp.Item_Index = Items.Item_Index;
	ItemsTmp.Item_UniIndex = Items.Item_UniIndex;
	ItemsTmp.Item_Tab = tab2;
	ItemsTmp.Item_Row = row2;
	ItemsTmp.Item_Col = col2;
	ItemsTmp.Item_Plus = Items.Item_Plus;
	ItemsTmp.Item_Flag = Items.Item_Flag;						
	ItemsTmp.Item_Used = Items.Item_Used;
	ItemsTmp.Item_Used2 = Items.Item_Used2;
	ItemsTmp.Item_Wearing = Items.Item_Wearing;
	ItemsTmp.Item_Sum = Items.Item_Sum;
	ItemsTmp.Item_RareIndex =Items.Item_RareIndex;
	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
	{
		ItemsTmp.SetOptionData( sbOption, Items.GetOptionType( sbOption ),
								Items.GetOptionLevel( sbOption ) );
	}

	Items.ItemData = Items2.ItemData;
	Items.Item_Index = Items2.Item_Index;
	Items.Item_UniIndex = Items2.Item_UniIndex;
	Items.Item_Tab = tab;
	Items.Item_Row = row;
	Items.Item_Col = col;
	Items.Item_Plus = Items2.Item_Plus;
	Items.Item_Flag = Items2.Item_Flag;						
	Items.Item_Used = Items2.Item_Used;
	Items.Item_Used2 = Items2.Item_Used2;
	Items.Item_Wearing = Items2.Item_Wearing;
	Items.Item_Sum = Items2.Item_Sum;
	Items.Item_RareIndex =Items2.Item_RareIndex;
	for( sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
	{
		Items.SetOptionData( sbOption, Items2.GetOptionType( sbOption ),
								Items2.GetOptionLevel( sbOption ) );
	}

	Items2.ItemData = ItemsTmp.ItemData;
	Items2.Item_Index = ItemsTmp.Item_Index;
	Items2.Item_UniIndex = ItemsTmp.Item_UniIndex;
	Items2.Item_Tab = ItemsTmp.Item_Tab;
	Items2.Item_Row = ItemsTmp.Item_Row;
	Items2.Item_Col = ItemsTmp.Item_Col;
	Items2.Item_Plus = ItemsTmp.Item_Plus;
	Items2.Item_Flag = ItemsTmp.Item_Flag;						
	Items2.Item_Used = ItemsTmp.Item_Used;
	Items2.Item_Used2 = ItemsTmp.Item_Used2;
	Items2.Item_Wearing = ItemsTmp.Item_Wearing;
	Items2.Item_Sum = ItemsTmp.Item_Sum;
	Items2.Item_RareIndex =ItemsTmp.Item_RareIndex;
	for( sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
	{
		Items2.SetOptionData( sbOption, ItemsTmp.GetOptionType( sbOption ),
								ItemsTmp.GetOptionLevel( sbOption ) );
	}

	if(Items.Item_Wearing != -1)
	{
		//_pNetwork->MyCurrentWearing[Items.Item_Wearing] = Items; 
		_pNetwork->pMyCurrentWearing[Items.Item_Wearing] = &Items; 
	}
	if(Items2.Item_Wearing != -1)
	{
		//_pNetwork->MyCurrentWearing[Items2.Item_Wearing] = Items2; 
		_pNetwork->pMyCurrentWearing[Items2.Item_Wearing] = &Items2; 
	}
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveChatMessage(CNetworkMessage *istr)
{
	CTString str;
	CTString strSendName;
	CTString strRecvName;
	unsigned char chattype;
	SLONG 	 sendindex;
				
	(*istr) >> chattype; //0528 kwon
	(*istr) >> sendindex;
	(*istr) >> strSendName;
	(*istr) >> strRecvName;
	(*istr) >> str;

	// 랭커 입장 메세지 표시.
	if( chattype == MSG_CHAT_RANKER_CONN )
	{
		SBYTE sbJob = atoi(str);
		CTString	strMessage;
		strMessage.PrintF(_S(2358,"%s클래스 랭커 %s님이 입장하였습니다."), JobInfo().GetName(sbJob, 0), strSendName );

		_UIAutoHelp->SetGMNotice(strMessage, 0x38FFD7FF);
		return;
	}

	// Date : 2005-05-25(오전 11:41:19), By Lee Ki-hwan
	if( chattype == MSG_CHAT_MESSENGER )
	{
		/***
		if( !_pUIMgr->GetMessenger()->IsUseLevel() ) return;
		_pUIMgr->GetMessenger()->RevTalk( sendindex, strSendName, strRecvName, str );
		return;
		***/
	}

	_pUIMgr->GetChatting()->AddChatMessage( chattype, sendindex, strSendName, str );
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveLoginMessage(CNetworkMessage *istr)
{
	if((*istr).GetType() == MSG_LOGINSERV_PLAYER)
	{
		_pSound->Mute();
		ReceiveServerListMessage(&(*istr));	
		//_pNetwork->m_bSendMessage = FALSE;
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
			_pSound->UpdateSounds();
#endif		
	}
	else if((*istr).GetType() == MSG_EXTEND)
	{
		LONG	ubType;
		(*istr) >> ubType;
	
		//if( ubType == MSG_EX_SECURE_CARD)
		{
			UBYTE cSecurityCode[4];

			(*istr) >> cSecurityCode[0];
			(*istr)	>> cSecurityCode[1];
			(*istr)	>> cSecurityCode[2];
			(*istr)	>> cSecurityCode[3];

			_pUIMgr->GetLogin()->SecurityCardSet(cSecurityCode);
		}

		_pUIMgr->Lock(FALSE);
		_pNetwork->m_bSendMessage = FALSE;
	}	
	else
	{
		unsigned char dbtype;
		(*istr) >> dbtype;

		// FIXME : bFail로 매번 체크하는게 맘에 들지 않음...
		static BOOL bFail	= FALSE;

		if((*istr).GetType() == MSG_FAIL)
		{	
			//wooss 050824
			// ----------->
			
			if(dbtype == MSG_FAIL_NOTLEVEL_FORDELETE )
			{	
				_pGameState->DisplayErrorMessage( dbtype, UI_NONE, MSGCMD_NULL);
				// WSS_TIMEOUT_BUGFIX 070607 ------->><<
				_pNetwork->m_bSendMessage = FALSE;
				return;
			} 
			// <----------
			else if(dbtype == MSG_FAIL_WRONG_IDENTIFICATION)			// 주민번호 틀림
			{
				_pUIMgr->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK,
											UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(2359,"주민등록 번호가 일치하지 않아 캐릭터를 삭제할 수 없습니다.") );

				_pUIMgr->CreateMessageBox( MsgBoxInfo);
				_pUIMgr->Lock(FALSE);
				// WSS_TIMEOUT_BUGFIX 070607 ------->><<
				_pNetwork->m_bSendMessage = FALSE;				
				return;
			}
			else if( dbtype == MSG_FAIL_SCARD_NOT_MATCHING )	//보안카드 인증 번호 틀림
			{
				_pUIMgr->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 3667, "보안 카드" ), UMBS_OK,
											UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(3669,"보안 카드 인증 실패") );

				_pUIMgr->CreateMessageBox( MsgBoxInfo);
				_pUIMgr->Lock(FALSE);
				_pNetwork->m_bSendMessage = FALSE;
				
				_pUIMgr->GetLogin()->SecurityCardSet(NULL);
				return;
			}


			_pGameState->DisplayErrorMessage( dbtype, UI_NONE, MSGCMD_LOGIN_ERROR);

			// NOTE : 캐릭터 생성 실패시에는 다음 화면으로 넘어가면 안되기 때문에 플래그 체크함.
			// NOTE : 캐릭터 생성 실패시에 먼저 MSG_FAIL이 온후 MSG_DB_CHAR_EXIST가 온다...
			if(_pUIMgr->GetUIGameState() == UGS_CREATECHAR)
			{
				bFail = TRUE;
			}
			_pNetwork->m_bSendMessage = FALSE;
			return;
		}
		else
		{
				_pSound->Mute();
			static BOOL bSlotInfo = FALSE;
			if(dbtype == MSG_DB_SUCCESS)
			{
				if(!bSlotInfo)
				{
					_pGameState->ClearCharacterSlot();
					bSlotInfo = TRUE;
				}
				bFail = FALSE;
			}
			else if(dbtype == MSG_DB_CHAR_EXIST)
			{
				if(!bSlotInfo)
				{
					_pGameState->ClearCharacterSlot();
					bSlotInfo = TRUE;
				}

				_pGameState->ReceiveCharSlot((*istr));		// 캐릭터 생성 후
			}
			else if(dbtype == MSG_DB_CHAR_END)
			{
				bSlotInfo = FALSE;
				_pGameState->SelectedSlot() = 0;

				//wooss 050820
				ULONG slotTime;
				(*istr) >> slotTime;
				_pUIMgr->GetSelChar()->SetExSlotTime(1,slotTime);
				(*istr) >> slotTime;
				_pUIMgr->GetSelChar()->SetExSlotTime(2,slotTime);
				
				// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
				ULONG ulSeed;
				(*istr) >> ulSeed;
				CNM_MakeKeyFromSeed(&_pNetwork->cnmKeyServer, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // ifdef CRYPT_NET_MSG

//#define AUTO_LOGIN
#ifndef AUTO_LOGIN
				_pUIMgr->GetSelChar()->ShowCharacter();
#endif
				_pNetwork->m_bSendMessage = FALSE;

#ifndef AUTO_LOGIN
				if(_pUIMgr->GetUIGameState() == UGS_SELCHAR ||
					_pUIMgr->GetUIGameState() == UGS_SELSERVER)
				{
					_pGameState->BackToSelChar();
					// 캐릭터 선택 페이지로 넘어감.
					_pUIMgr->SetUIGameState(UGS_SELCHAR);				
					_pUIMgr->Lock(FALSE);
					_pUIMgr->GetSelServer()->ResetServerList();
#else
					_pGameState->SelectedSlot() = 1;
					_pUIMgr->GetSelChar()->StartGame();
#endif
#ifndef AUTO_LOGIN
				}
				else 
#endif
					if(_pUIMgr->GetUIGameState() == UGS_CREATECHAR)
				{
					if(bFail)
					{
						bFail = FALSE;
						
#ifdef RESTRICT_SOUND
						extern BOOL _bNTKernel;
						//if( _bNTKernel )
							_pSound->UpdateSounds();
						#endif
						return;
					}
					// 생성 성공시!!!
					_pUIMgr->SetUIGameState(UGS_SELCHAR);
					_pGameState->BackToSelChar();
				}
			}
			else if(dbtype == MSG_DB_OK)
			{				
				bFail = FALSE;
				_pNetwork->m_bSendMessage = FALSE;
				// Game Start
				ULONG zone;
				(*istr) >> zone;
				g_slZone = zone;//0616 kwon			
				
				//_pNetwork->m_bSingleMode = FALSE;
#ifndef AUTO_LOGIN
				_pNetwork->TogglePause();

				if(ZoneInfo().GetZoneType(zone) == ZONE_SDUNGEON)
				{
					CPrintF("===Start Single Mode===\n");
					_pNetwork->m_bSingleMode = TRUE;
					_pNetwork->wo_dwEnemyCount		= 0;
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SingleModeOn();
				}
				else
				{
					CPrintF("===End Single Mode===\n");
					_pNetwork->m_bSingleMode = FALSE;
					_pUIMgr->GetSingleBattle()->Close();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->FieldModeOn();
				}
				
				_pNetwork->ChangeLevel(ZoneInfo().GetZoneWldFile(zone), FALSE, 0);
#else
				_pUIMgr->SetUIGameState( UGS_GAMEON );		
				_pNetwork->_TargetInfo.Init();
				_pNetwork->_TargetInfoReal.Init();
#endif
				
				extern ENGINE_API BOOL _bLoginProcess;
				_bLoginProcess					= FALSE;
				_SetPlayStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
								
				_pNetwork->SetLoadingZone( g_slZone );
				
				_pGameState->GetGameMode()		= CGameState::GM_NETWORK;
				_pUIMgr->GetGame()->gm_bGameOn	= TRUE;
				
				/*
				if(_pNetwork && _pNetwork->m_bSingleMode && _pNetwork->ga_World.wo_iNumOfNPC)
				{
					CEntity* penPlayer = CEntity::GetPlayerEntity(0);
					_pNetwork->ga_World.m_vectorTargetNPC.push_back(penPlayer);
				}
				*/
			}

			// EDIT : BS
			else if (dbtype == MSG_DB_OTHER_SERVER)
			{
				ULONG zone;
				CTString ip;
				ULONG port;

				(*istr) >> zone;
				(*istr) >> ip;
				(*istr) >> port;

				MoveOtherServer(zone, ip, port);
			}
			// --- EDIT : BS
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 서버 목록을 가져옵니다.
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveServerListMessage(CNetworkMessage *istr)
{
	LONG lRecentGroup;
	LONG lRecentServer;
	LONG lSubServerCount;
	LONG lGroupCount;
	LONG lSeqGroupNumber;
	LONG lGroupNumber;
	UBYTE bIsRecommend = 0;
	
	(*istr) >> lRecentGroup;		// 최근 접속한 서버 군
	(*istr) >> lRecentServer;		// 최근 접속한 서버 번호
	(*istr) >> lGroupCount;			// 서버 목록의 갯수.
	(*istr) >> lSeqGroupNumber;		// 서버 목록의 갯수.
	(*istr) >> lGroupNumber;		// 서버 목록의 갯수.
	(*istr) >> lSubServerCount;		// 서버 목록의 갯수.

#ifdef RECOMMEND_SERVER_EVENT	
	// --- wooss-060426---추천서버이벤트-------------------->>
	(*istr) >> bIsRecommend;
	// -----------------------------------------------------<<
#endif
	// 꽃놀이 이벤트 서버 리스트 초기화
	_pUIMgr->GetFlowerTree()->ClearList();

	for(int i = 0; i < lSubServerCount; ++i)
	{
		LONG		lSubNum;		// Sub 번호
		LONG		lPlayerNum;		// 플레이어 수
		CTString	strAddress;		// IP Address
		LONG		lPortNum;		
		
		(*istr) >> lSubNum;		// Sub 번호 
		(*istr) >> lPlayerNum;	// 플레이어 수
		(*istr) >> strAddress;	// IP Address
		(*istr) >> lPortNum;		// Port Num
		
		lPlayerNum -= ADDSERVERVALUE;
		
		_pUIMgr->GetSelServer()->AddToServerList(lGroupNumber, lSubNum, lPlayerNum, strAddress, lPortNum, bIsRecommend);		
	}
	
	SetRecentServer(lRecentGroup, lRecentServer);
	
	if(lSeqGroupNumber == lGroupCount)
	{
		_pUIMgr->GetSelServer()->SetRecentServer(lRecentGroup, lRecentServer);
		_pUIMgr->SetUIGameState( UGS_SELSERVER );
		_pNetwork->m_bSendMessage = FALSE;
		_cmiComm.Disconnect();
	}
}

#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & ((1L)<<(flagVal)) )
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *istr - 
//-----------------------------------------------------------------------------
//안태훈 수정 시작	//(5th Closed beta)(0.2)
void CSessionState::ReceiveAppearMessage(CNetworkMessage *istr)
{	
	SBYTE			isNew;		// 0 or 1
	SBYTE			type;		// MSG_CHAR_PC or MSG_CHAR_NPC

	(*istr) >> isNew;
	(*istr) >> type;
	
	if(type == MSG_CHAR_NPC)
	{
		ReceiveMobAppearMessage(istr);
	}
	else if(type == MSG_CHAR_PC) //PC인 경우 
	{
		ReceiveCharacterAppearMessage(istr);
	}
	else if(type == MSG_CHAR_PET)
	{
		ReceivePetAppearMessage(istr,(BOOL)isNew);
	}
	else if(type == MSG_CHAR_ELEMENTAL)
	{
		ReceiveSummonAppearMessage(istr);
	}
	else if(type == MSG_CHAR_WILDPET)
	{
		ReceiveWildPetAppearMessage(istr);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveMobAppearMessage(CNetworkMessage *istr)
{
	CPlacement3D	plPlacement;	
	SLONG			index;
	CTString		name;
	FLOAT			x, z, h, r;
	SBYTE			yLayer;
	SLONG			hp, maxHP;
	SLONG			idNum;		// 1015
	SBYTE			assist_count;
	SLONG			assist_state;
	UBYTE			sbAttributePos;
	INDEX			Ownerindex = -1;
	CTString		Ownername = "";
	CTString		Ownershopname = "";

	CEntity* penEntity;
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	(*istr) >> index;
	(*istr) >> idNum;		
	(*istr) >> x >> z >> h;
	(*istr) >> r >> yLayer;
	(*istr) >> hp >> maxHP;	
	
	plPlacement.pl_PositionVector(1) = x;
	plPlacement.pl_PositionVector(2) = h + 1.0f;
	plPlacement.pl_PositionVector(3) = z;
	plPlacement.pl_OrientationAngle(1) = r;
	plPlacement.pl_OrientationAngle(2) = 0;
	plPlacement.pl_OrientationAngle(3) = 0;
	
	//		CPrintF("Send Mob Create Message : h : %f\n", h);		
	// FIXME : CMobTarget 부분을 서버 ID와 매핑하는 식으로 처리하려 했으나, 고쳐야 될 부분이 너무 많아서 포기...ㅡ.ㅡ
	CMobData& MD = _pNetwork->GetMobData(idNum);
	if(MD.GetMobIndex() == -1)
	{
		OutputDebugString("Cannot find Mob Data!!!\n");
		//			ASSERTALWAYS("Cannot find Mob Data!!!\n");
		return;
	}
	
	CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob.Push();
	
	penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ENEMY, -1,TRUE);	
	penEntity->SetNetworkID(index);
	
	BOOL bNpc = FALSE;
	if( MD.IsSkillMaster() || MD.IsSSkillMaster() || MD.IsNPC() )
	{
		bNpc = TRUE;
	}
	
	(*istr) >> assist_state >> assist_count;
	
	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	ASSERT( mt.mob_BuffCount == 0 );
	mt.mob_BuffCount = 0;
	for( int i = 0; i < assist_count; i++ )
	{
		(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;
		mt.AddBuff( sBuff );
	}
	
	(*istr) >> sbAttributePos;

#ifdef CASH_PERSON_SHOP			// ttos : 판매대행 상인
	if(idNum == 482){
		(*istr) >> Ownerindex;
		(*istr) >> Ownername;
		(*istr) >> Ownershopname;
	}
#endif	//CASH_PERSON_SHOP

	if(sbAttributePos != ATTC_UNWALKABLE )
	{
		mt.mob_sbAttributePos = sbAttributePos;
	}
	
	const char *szMobName = _pNetwork->GetMobName(idNum);
	CTString strMobName;
	
	if(Ownerindex > 0) 
	{
		strMobName.PrintF("%s %s",Ownername,szMobName);
		MD.SetShopmsg( Ownershopname );
		MD.SetOwnerIndex(Ownerindex);
	}else strMobName = szMobName;

	mt.SetData(index, idNum, strMobName, MD.GetLevel(), penEntity, bNpc, yLayer );
	penEntity->InitAsSkaModel();
	mt.mob_iClientIndex	= penEntity->en_ulID;
	
	// 성주의 권좌일때....
	if( idNum == LORD_SYMBOL_INDEX )
	{
		INDEX iCurHealth = hp;
		float fMaxHealth = maxHP;
		if( iCurHealth <= fMaxHealth * 0.25f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword04.smc");			
		}
		else if( iCurHealth <= fMaxHealth * 0.50f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword03.smc");			
		}
		else if( iCurHealth <= fMaxHealth * 0.75f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword02.smc");			
		}			
		else
		{
			penEntity->SetSkaModel(MD.GetMobSmcFileName());
		}
	}
	else
	{
		penEntity->SetSkaModel(MD.GetMobSmcFileName());
	}
	//0807
	
	// FIXME : Factory로 바꿀까???
	penPlayerEntity->SetMobData(penEntity,hp,maxHP,MD.GetLevel(),bNpc,idNum);				
	CMobData::SetMobDataToNPC( penEntity, MD, szMobName );
	penEntity->Initialize();
	penEntity->FallDownToFloor();
	
	// 공성 관련되서... hp가 0이라면...
	// 죽은 NPC이지만 사라지지는 않는 NPC로...(그리고 타겟팅도 안되도록...)
	if( (MD.IsCastleTower()) && hp <= 0 )
	{
		penEntity->SetFlagOff(ENF_ALIVE);
	}

	if( mt.mob_iType == 491)
	{
		penEntity->SetFlagOff(ENF_ALIVE);
	}
	
	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		mt.ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, assist_state);
	}
	else
	{
		ASSERTALWAYS("Mob must have ska model and tag manager.");
	}
	
	if(bNpc)
	{
		CQuestSystem::Instance().RefreshNPCQuestMark(idNum);
	}
	
	//hardcoding effect
	if( penEntity->en_pmiModelInstance == NULL ) return;
	
	if (idNum == MOB_FLOWERTREE_INDEX || idNum == MOB_XMAS_TREE_CENTER || idNum == MOB_XMAS_TREE_DECO)
	{ //꽃나무, 크리스마스 트리(장식용, 광장용)
		_pUIMgr->GetFlowerTree()->SetMobFlowerTree(mt); // 꽃나무 MobTarget정보 저장
		_pUIMgr->GetFlowerTree()->FlowerTreeUpdate(hp); // 꽃나무 모델 포인트에 따른 업데이트
	}

	CTString strStateEffectName;

	switch( mt.mob_iType )
	{
	case 190: {	strStateEffectName = "DK Normal State";	} break; //데스나이트
	case 236: { strStateEffectName = "npcGolem01"; } break; //아케인 자이언트
	case 454: { strStateEffectName = "halloween_mon"; penEntity->SetFlagOn(ENF_HIDDEN); } break; // 호박 귀신
	case 455: { strStateEffectName = "halloween_crow"; } break; // 호박 광대 허수아비
	case 504: // 안개그림자 드래곤
	case 505: // 핏빛그림자 드래곤
		{
			penEntity->SetFlagOn(ENF_HIDDEN);
		}
	default:
		return;
	}

	mt.mob_pNormalEffect = StartEffectGroup( strStateEffectName, &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveCharacterAppearMessage(CNetworkMessage *istr)
{
	CPlacement3D	plPlacement;	
	SLONG			index;
	CTString		name;
	SBYTE			job;
	SBYTE			job2;
	FLOAT			x, z, h, r;
	SBYTE			yLayer;
	SLONG			hp, maxHP;
	SBYTE			hairstyle;	// 헤어 스타일 //1013
	SBYTE			facestyle;	// 얼굴 스타일
	UWORD			state;		// WSS_DRATAN_SIEGEWARFARE 2007/10/12
	SBYTE			assist_count;
	SLONG			assist_state;
	SBYTE			pktitle;
	SLONG			pkstate;
	SLONG			change_mobid;
	UBYTE			sbAttributePos;
	SBYTE			sbPetTypeGrade;
	SBYTE			sbPetColor =0;
	SBYTE			sbEvocationType;
	SBYTE			sbPesscorps = 0;

	CEntity* penEntity;
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	(*istr) >> index;
	(*istr) >> name;
	(*istr) >> job;
	(*istr) >> job2;
	(*istr) >> hairstyle;//1013
	(*istr) >> facestyle;		
	(*istr) >> x >> z >> h;
	(*istr) >> r >> yLayer;
	(*istr) >> hp >> maxHP >> state >> pkstate >> pktitle;
	
	SBYTE	sbItemPlusEffect;
	(*istr) >> sbItemPlusEffect;
	
	plPlacement.pl_PositionVector(1) = x;
	plPlacement.pl_PositionVector(2) = h + 1.0f;
	plPlacement.pl_PositionVector(3) = z;
	plPlacement.pl_OrientationAngle(1) = r;
	plPlacement.pl_OrientationAngle(2) = 0;
	plPlacement.pl_OrientationAngle(3) = 0;
	
	// FIXME : Factory로 바꿀까???
	CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha.Push();
	penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement,CTFILENAME("Classes\\Character.ecl"),-1,TRUE);
	ct.SetData( index, job, name, penEntity, yLayer,state,pkstate );//0826
	ct.cha_iJob2 = job2;
	penEntity->InitAsSkaModel();
	ct.cha_iClientIndex = penEntity->en_ulID;
	penPlayerEntity->SetChaData(penEntity->en_ulID, job, hp, maxHP, hairstyle, facestyle, name, state,pktitle, pkstate );
	ct.SetItemEffectOption( sbItemPlusEffect );
	//penPlayerEntity->SetChaWearing(penEntity, &nmMessage);
	penEntity->Initialize();
	penEntity->SetNetworkID(index);
	penEntity->FallDownToFloor();
	penEntity->en_pCharacterTarget	= &ct;
	//CPrintF(TRANS("Cha_Create pos:%f %f %f index: %d \n"),plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3),index);
	penPlayerEntity->SetChaWearing(ct, penEntity, &(*istr));

	(*istr) >> assist_state >> assist_count; 
	
	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	ASSERT( ct.cha_BuffCount == 0 );
	ct.cha_BuffCount = 0;

	CEffectGroup* pEG=NULL;
	for( int k = 0; k < assist_count; k++ )
	{
		(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;
		
		// wooss 070305 ------------------------------------>>
		// kw : WSS_WHITEDAY_2007
		// 6종 러브매직 스킬
		// 423 424 425 426 427 428
		if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
		{
			// 처음이면 이펙트를 생성함...
			if(pEG==NULL)
			{
				// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
				pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
		//		penEntity->GetModelInstance()->m_pEG = pEG;
			}

			sBuff.m_pEG =  pEG;
		}
		// -------------------------------------------------<<
		
		// wooss 070310 ----------------------->><<
		// kw : WSS_WHITEDAY_2007
		// Add lovemagic buff (wind - skill index 423 )
		if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex == 1226||sBuff.m_slItemIndex ==1227 || sBuff.m_slSkillIndex == 423	//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
			|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
			)	
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, ct.cha_iClientIndex, TRUE);
		}
		
		ct.AddBuff( sBuff );
	}

	if (_pNetwork->SearchLegitList(index))
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetRepairLegit(index, &ct);
	}
	
	// 강동민 수정 시작		// 개인 상점
	// shottype(c) shopname(str)
	SBYTE sbShopType;
	CTString strShopName;
	(*istr) >> sbShopType;
	ct.cha_sbShopType = sbShopType;		
	(*istr) >> strShopName;
	
	if(sbShopType != PST_NOSHOP)
	{
		ct.ShopMsg.SetChatMsg( strShopName );
		penPlayerEntity->SetShopData( ct.cha_iClientIndex, sbShopType );
	}
	// 강동민 수정 끝		// 개인 상점
	
	// 길드 관련.
	//guildindex(n) guildname(str)
	LONG		lGuildIndex;
	CTString	strGuildName;
	LONG		lGuildPosition;
	(*istr) >> lGuildIndex;
	(*istr) >> strGuildName;
	(*istr) >> lGuildPosition;
// WSS_NEW_GUILD_SYSTEM 070731
//#ifdef DISPLAY_GUILD_RANK	
//	(*istr) >> ct.cha_sbGuildRank;			
//#endif
	ct.cha_lGuildIndex		= lGuildIndex;
	ct.cha_strGuildName		= strGuildName;
	ct.cha_lGuildPosition	= lGuildPosition;

	// 같은 길드일 경우.
	if( lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
	{

	}
	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/01
	// 교감 애니 적용
	if(state & PLAYER_STATE_CRISTAL_RESPOND)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetConsensus(ct.cha_Index);
		_pUISWDoc->StartConsensusEffect(ct.cha_Index,ct.cha_pEntity); // WSS_DRATAN_SIEGEWARFARE 2007/10/14		
	}
	

	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		CStatusEffect::eRelation rel = CStatusEffect::R_NONE;
		if(_pUIMgr->GetParty()->IsPartyMember(ct.cha_Index))
		{
			// 051203 파티 멤버일 경우.  플래그를 켜줌.
			penEntity->SetSecondExtraFlagOn( ENF_EX2_MYPARTY );
			rel = CStatusEffect::R_PARTY;
		}
		ct.ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, assist_state, rel);			
	}
	else
	{
		ASSERTALWAYS("Character must have ska model and tag manager.");
	}
	
	if(IS_FLAG_ON(assist_state, EST_ASSIST_INVISIBLE) && _pNetwork->m_ubGMLevel < 2)
	{
		penEntity->SetFlags(penEntity->GetFlags() | ENF_HIDDEN);
	}
	else
	{
		penEntity->SetFlags(penEntity->GetFlags()&~ENF_HIDDEN);
	}

	if(IS_FLAG_ON(assist_state, EST_ASSIST_FAKEDEATH))
	{
		INDEX animID = ska_StringToID("ro_die_01");//Hardcoding
		penEntity->GetModelInstance()->AddAnimation(animID, AN_CLEAR, 1.0f, 0);
		//hardcoding, CCharacterBase의 m_bIdleAnim property를 가져온다.
		if(penEntity->en_pecClass)
		{
			CDLLEntityClass *pdecDLLBaseClass	= penEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
			CEntityProperty *pEP = pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 250);
			if(pEP)
			{
				ENTITYPROPERTY( &*penEntity, pEP->ep_slOffset, BOOL) = FALSE;
			}
		}
	}
	//0221 변신루틴
	
	(*istr) >> change_mobid;
	ct.cha_nChangeMobId = change_mobid;
	
	if(change_mobid != -1)
	{
		penPlayerEntity->AppearChaPolymoph(penEntity, change_mobid);
	}
	
	SLONG fame;
	(*istr) >> fame;
	ct.cha_nFame = fame;
	
	(*istr) >> sbAttributePos;
	if( sbAttributePos != ATTC_UNWALKABLE )
	{
		ct.cha_sbAttributePos = sbAttributePos;
	}
	
	SBYTE sbJoinFlagMerac;
	(*istr) >> sbJoinFlagMerac;
	ct.cha_sbJoinFlagMerac = sbJoinFlagMerac;
// WSS_DRATAN_SIEGEWARFARE 070720
#ifdef DRATAN_SIEGEWARFARE
	SBYTE sbJoinFlagDratan;
	(*istr) >> sbJoinFlagDratan;
	ct.cha_sbJoinFlagDratan = sbJoinFlagDratan;
#endif

	(*istr) >> sbPetTypeGrade;

	// [070824: Su-won] PET_COLOR_CHANGE
#ifdef PET_COLOR_CHANGE
	(*istr) >> sbPetColor;
#endif

	(*istr) >> sbEvocationType;
	ct.cha_nTransformID = sbEvocationType - 1;

	if(ct.cha_nTransformID != -1)
	{
		penPlayerEntity->AppearChaTransform(penEntity, ct.cha_nTransformID);
	}

	if( sbPetTypeGrade )
	{		
		INDEX iPetType;
		INDEX iPetAge;
		_pNetwork->CheckPetType( sbPetTypeGrade, iPetType, iPetAge );				
		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
		
		ct.cha_iPetType = iPetType;
		ct.cha_iPetAge	= iPetAge;
		if( ct.cha_bPetRide != bPetRide )
		{
			// 펫을 타도록 설정해야 하는 경우...
			if( iPetType != -1 && iPetAge != -1 && bPetRide )			
			{
				// [070824: Su-won] PET_COLOR_CHANGE
				INDEX iPetColoredType = iPetType | (sbPetColor<<8);
				_pNetwork->RidePet( ct.cha_pEntity, NULL, iPetColoredType );
			}
			ct.cha_bPetRide = bPetRide;
		}
	}
	
#ifdef SUPPORTER_EVENT // 게임 서포터 이벤트 
	SBYTE			sbSupporter;

	(*istr) >> sbSupporter;
	// 서포터 상태 일때
	if ( sbSupporter )
	{// Effect 적용
		INDEX Cnt = _pNetwork->ga_srvServer.srv_actCha.Count()-1;
		penPlayerEntity->PlAddAccessoryEffect(Cnt, penEntity, FALSE);
	}
#endif

	// 라카 기자단
#ifdef LAKA_Presscorp
	(*istr) >> sbPesscorps; // 미국 라카 기자단 없음
	ct.cha_sbPresscorps = sbPesscorps;
#endif

	if ( sbJoinFlagMerac > 0 )
	{
		// Date : 2005-07-13(오후 5:29:49), By Lee Ki-hwan
		// 공성 Effect
		_pUISWDoc->StartEffect( index, penEntity, sbJoinFlagMerac, lGuildIndex );
		
		// 길드장 이펙트 추가 // Date : 2005-11-18(오후 3:43:49), By Lee Ki-hwan
		_pUISWDoc->StartGuildMasterEffect( index, penEntity, sbJoinFlagMerac, lGuildIndex, lGuildPosition );
	}	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/29
	// 공성 이펙트 처리
#ifdef DRATAN_SIEGEWARFARE
	if( sbJoinFlagDratan > 0 )
	{
		// TODO :: 이펙트 나오면 하위 처리...
		_pUISWDoc->StartEffect( index, penEntity, sbJoinFlagDratan, lGuildIndex );
		_pUISWDoc->StartGuildMasterEffect( index, penEntity, sbJoinFlagDratan, lGuildIndex, lGuildPosition );
	}
#endif

	// Date : 2005-04-06(오후 5:04:54), By Lee Ki-hwan
	// 길드 전투 중이라면 길드 Index를 체크해서 Effect를 뿌려줌
	if ( _pUIMgr->GetGuildBattle()->IsInBattle() )
	{
		int nEnemyGuild = _pUIMgr->GetGuildBattle()->IsEnemyGuild( lGuildIndex );
		if( nEnemyGuild == 1 ) // 적군 (적색)
		{
			_pUIMgr->GetGuildBattle()->StartGuildEffect( index, penEntity, TRUE );
		}
		else if( nEnemyGuild == -1 ) // 아군 (청색)
		{
			_pUIMgr->GetGuildBattle()->StartGuildEffect( index, penEntity, FALSE );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Pet Appear에 대해서 처리함(애완동물을 타고 있지 않을 경우에...)
// Input  : *istr - 
//-----------------------------------------------------------------------------
// FIXME : ReceivePetAppearMessage, ReceiveStatusMessage, ReceivePetMountMessage 중복되는 코드 많음 정리 필요.
void CSessionState::ReceivePetAppearMessage(CNetworkMessage *istr,BOOL isNew)
{
	CPlacement3D	plPlacement;	
	SLONG			index;
	SLONG			lOwnerIndex;
	CTString		strOwnerName;	
	SBYTE			sbTypeGrade;
	FLOAT			x, z, h, r;
	SBYTE			yLayer;
	SLONG			hp, maxHP;
	UBYTE			sbAttributePos;		// 속성 맵 값.
	CTString		strNameCard ="";
	SLONG			slPetTransformIndex, slPetTransformSize; // 펫 변신 Npc Index, 펫 변신 Npc size

	(*istr) >> index;
	(*istr) >> lOwnerIndex;
	(*istr) >> strOwnerName;	
	(*istr) >> sbTypeGrade;	
	(*istr) >> x >> z >> h;
	(*istr) >> r >> yLayer;
	(*istr) >> hp >> maxHP >> sbAttributePos;
#ifdef PET_NAMECARD
	(*istr) >> strNameCard;
#endif
	// [070824: Su-won] PET_COLOR_CHANGE
	UBYTE			sbPetColor = 0;
#ifdef PET_COLOR_CHANGE
	(*istr) >> sbPetColor;
#endif

	slPetTransformIndex = 0;
	slPetTransformSize = 10;
#ifdef PET_TRANSFORM	
	(*istr) >> slPetTransformIndex;
	(*istr) >> slPetTransformSize;
#endif

	plPlacement.pl_PositionVector(1)	= x;
	plPlacement.pl_PositionVector(2)	= h + 1.0f;
	plPlacement.pl_PositionVector(3)	= z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f ;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;

	//-----------------------------------------------------------	
	INDEX iPetType	= -1;
	INDEX iPetAge	= -1;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );

	// NOTE : 탈수 있는 애완동물인지 판단하여, 캐릭터를 태웁니다.
	// NOTE : 애완동물을 타고 있을 경우에는, Pet타겟 정보를 추가하지 않음(내 애완동물일때는 펫 타겟 정보창 설정할것...)
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);	
	
	//-----------------------------------------------------------
	CEntity* penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_PET, -1, TRUE);
	if( !bPetRide )
	{
		// NOTE : 애완동물을 타고있는 경우에는, 그 엔티티는 캐릭터로 취급되기 때문에,
		// NOTE : 펫타겟을 추가하면 안된다.
		// NOTE : 만약에 추가하게 되면, DISAPPEAR에서 엔티티를 Destroy하기 때문에
		// NOTE : 애니메이션시 엔티티가 유효하지 않게 되어서 뻑나게 됨.
		// FIXME : Factory로 바꿀까???
		CPetTarget &pt = _pNetwork->ga_srvServer.srv_actPet.Push();
		CTString strPetName = strOwnerName + _S(2228,"의 ") + CTString(" ") + PetInfo().GetName(iPetType, iPetAge);		
		pt.SetData( index, strPetName, strOwnerName, lOwnerIndex, iPetType, iPetAge, penEntity, yLayer, strNameCard);	
		pt.pet_iClientIndex = penEntity->en_ulID;
		penEntity->en_pPetTarget	= &pt;
	}
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetPetData( penEntity, hp, maxHP );

	BOOL bUseAI		= FALSE;

	// 내 펫이며, 인벤토리에 장착해서 데리구 다니는 펫일 경우.
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{
		// 펫 타겟 정보창 정보를 설정합니다.
		bUseAI		= TRUE;
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_PET);		
		_pNetwork->_PetTargetInfo.lIndex		= index;
		_pNetwork->_PetTargetInfo.bIsActive		= TRUE;
		_pNetwork->_PetTargetInfo.TransformIndex = slPetTransformIndex; // 펫 변신 NPC 인덱스
		_pNetwork->_PetTargetInfo.pen_pEntity	= penEntity;
		
		_pNetwork->UpdatePetTargetInfo( index );
	}	

	penEntity->InitAsSkaModel();
	penEntity->SetSkaModel(PetInfo().GetFileName(iPetType, iPetAge));
	PetInfo().SetPetDataToEntity(iPetType, iPetAge, penEntity, bUseAI);
	penEntity->SetNetworkID(index);
	penEntity->Initialize();
	penEntity->FallDownToFloor();
	
	// [070824: Su-won] PET_COLOR_CHANGE
	// 펫 색깔 변경 아이템을 사용한 상태라면 텍스쳐 파일을 바꿔줌...
	if( sbPetColor >0 )
	{
		CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, sbPetColor);
		penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
	}

	// eons 펫 변신
	// 일단 펫 모델 설정을 거친 후 변신을 하자.
	if (slPetTransformIndex > 0)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(penEntity, slPetTransformIndex, slPetTransformSize);
	}
	
	if(g_iCountry == KOREA)
	{
		if( sbAttributePos == ATTC_WAR)
			penEntity->SetFlagOn(ENF_HIDDEN);
	}
	else {
		if( sbAttributePos == ATTC_WAR || sbAttributePos == ATTC_PEACE)
			penEntity->SetFlagOn(ENF_HIDDEN);
	}
	
	if(isNew){
		CTString strEffect;
		strEffect.PrintF( "pet_%c%d_summon", (iPetType==0)?'h':'d', iPetAge+1 );
		PetStartEffectGroup( strEffect, index );
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: Pet Appear에 대해서 처리함.
// Input  : *istr - 
//-----------------------------------------------------------------------------
// FIXME : ReceivePetAppearMessage, ReceiveStatusMessage, ReceivePetMountMessage 중복되는 코드 많음 정리 필요.
void CSessionState::ReceiveSummonAppearMessage(CNetworkMessage *istr)
{
	//					: 소환수: isNew(uc) type(c) index(n) elemental_type(c) owner_index(n) owner_name(str) x(f) z(f) h(f) r(f) yLayer(n) hp(n) maxHP(n) assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count map_attr(uc)
	CPlacement3D	plPlacement;
	SLONG			index;
	SBYTE			sbElementalType;
	SLONG			lOwnerIndex;
	CTString		strOwnerName;		
	FLOAT			x, z, h, r;
	SBYTE			yLayer;
	SLONG			hp, maxHP;
	SBYTE			assist_count;
	SLONG			assist_state;
	UBYTE			sbAttributePos;		// 속성 맵 값.

	(*istr) >> index;
	(*istr) >> sbElementalType;
	(*istr) >> lOwnerIndex;
	(*istr) >> strOwnerName;
	(*istr) >> x >> z >> h;
	(*istr) >> r >> yLayer;
	(*istr) >> hp >> maxHP;

	plPlacement.pl_PositionVector(1)	= x;
	plPlacement.pl_PositionVector(2)	= h + 1.0f;
	plPlacement.pl_PositionVector(3)	= z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;	

	(*istr) >> assist_state >> assist_count; 
	CEntity* penEntity = NULL;
	if( _pNetwork->m_bSingleMode )
	{
		penEntity = _pNetwork->ga_World.CreateEntity_t(CEntity::GetPlayerEntity(0)->GetPlacement(), CLASS_SUMMON, -1, TRUE);
		if( penEntity )
			_pNetwork->ga_World.m_vectorTargetNPC.push_back(penEntity);
	}
	else
	{
		penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_SUMMON, -1, TRUE);
	}
	penEntity->InitAsSkaModel();	
	penEntity->SetSkaModel( SlaveInfo().GetFileName(sbElementalType) );		
	SlaveInfo().SetSlaveDataToEntity( sbElementalType, penEntity, lOwnerIndex == _pNetwork->MyCharacterInfo.index );
	penEntity->SetNetworkID(index);
	penEntity->Initialize();
	//penEntity->FallDownToFloor();	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalData(penEntity, hp, maxHP);
	
	// 내 소환수라고 표시해줌.
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE);
		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
			if( !pUISummon->GetSummonEntity() )
			{
				pUISummon->SetSummonIndex(index);
				pUISummon->SetSummonEntity( penEntity );
				pUISummon->SetSummonType( sbElementalType );
				//pUISummon->SetCommand(CSlaveInfo::COMMAND_PROTECTION);
				_pNetwork->_SlaveTargetInfo[i - UI_SUMMON_START].fHealth	= hp;
				_pNetwork->_SlaveTargetInfo[i - UI_SUMMON_START].fMaxHealth = maxHP;						
				break;
			}
		}
	}

	// NOTE : 애완동물을 타고있는 경우에는, 그 엔티티는 캐릭터로 취급되기 때문에,
	// NOTE : 펫타겟을 추가하면 안된다.
	// NOTE : 만약에 추가하게 되면, DISAPPEAR에서 엔티티를 Destroy하기 때문에
	// NOTE : 애니메이션시 엔티티가 유효하지 않게 되어서 뻑나게 됨.
	// FIXME : Factory로 바꿀까???
	CSlaveTarget &st = _pNetwork->ga_srvServer.srv_actSlave.Push();
	CTString strSlaveName = strOwnerName + _S(2228,"의 ") + CTString(" ") + SlaveInfo().GetName(sbElementalType);		// 번역
	st.SetData( index, strSlaveName, strOwnerName, lOwnerIndex, sbElementalType, penEntity, yLayer );	
	st.slave_iClientIndex = penEntity->en_ulID;
	penEntity->en_pSlaveTarget	= &st;
	
	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	ASSERT( st.slave_BuffCount == 0 );
	st.slave_BuffCount = 0;
	for( int i = 0; i < assist_count; i++ )
	{
		(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;
		st.AddBuff( sBuff );
	}

	(*istr) >> sbAttributePos;

	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		st.ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, assist_state);
	}
	else
	{
		ASSERTALWAYS("Character must have ska model and tag manager.");
	}

	SlaveInfo().StartIdleEffect( index, sbElementalType, penEntity );		
#ifdef SUMMON_SIZE
	BYTE	summonSize;
	(*istr) >> summonSize;
	// 소환수 사이즈 조절 ( 캐릭 전직전 작게 )
	if(!summonSize)
	{
		penEntity->en_pmiModelInstance->StretchModel(FLOAT3D( 0.7f,0.7f,0.7f ));
/*		// Start Effect
		penEntity->GetModelInstance()->m_tmSkaTagManager.SetOwner(penEntity);
		CEffectGroup *pEG = StartEffectGroup( "STATE_SUMMON_SIZE"
			, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
			,_pTimer->GetLerpedCurrentTick());*/
	}
	
	/*
	if(_pNetwork->MyCharacterInfo.index == lOwnerIndex )
	{
		if(_pNetwork->MyCharacterInfo.job2 != 1)
		{
			// Start Effect
			CEffectGroup *pEG = StartEffectGroup( "STATE_SUMMON_SIZE"
				, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
				,_pTimer->GetLerpedCurrentTick());
			
	//		std::map<SLONG, CEffectGroup *>::value_type tmpVal( index, pEG);
	//		SlaveInfo().m_mapEG.insert(tmpVal);
		}

	}else {
	
		CCharacterTarget tv_target = GetOwnner(lOwnerIndex);
		if(tv_target.cha_Index != -1)
		{
			if( tv_target.cha_iJob2 != 1)
			{			
				// Start Effect
				CEffectGroup *pEG = StartEffectGroup( "STATE_SUMMON_SIZE"
					, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
					,_pTimer->GetLerpedCurrentTick());
				
		//		std::map<SLONG, CEffectGroup *>::value_type tmpVal( index, pEG);
		//		SlaveInfo().m_mapEG.insert(tmpVal);
			}
		}
	}*/
#endif 
#ifdef SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008//IsFlagOn(ENF_HIDDEN)
	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)	
	{
		if (sbAttributePos == ATTC_PEACE)
			penEntity->SetFlagOn(ENF_HIDDEN);
	}	
#endif	
}

void CSessionState::ReceiveWildPetAppearMessage(CNetworkMessage *istr)
{
	CPlacement3D	plPlacement;	
	INDEX			iNetID,index,iLevel;
	SLONG			lOwnerIndex;
	CTString		strOwnerName;	
	INDEX			nPetType;
	FLOAT			x, z, h, r;
	SBYTE			yLayer;
	SLONG			hp, maxHP;
	UBYTE			sbAttributePos;		// 속성 맵 값.
	CTString		strPetName;
	INDEX			nItemIndex[4],nItemPlus[4];

	(*istr) >> iNetID;
	(*istr) >> index;
	(*istr) >> iLevel;
	(*istr) >> strPetName;
	(*istr) >> lOwnerIndex;
	(*istr) >> strOwnerName;	
	(*istr) >> nPetType;	
	(*istr) >> x >> z >> h;
	(*istr) >> r >> yLayer;
	for (int i = 0; i < 4; i++)
	{
		(*istr) >> nItemIndex[i];
		(*istr) >> nItemPlus[i];
	}

	(*istr) >> hp >> maxHP >> sbAttributePos;

	plPlacement.pl_PositionVector(1)	= x;
	plPlacement.pl_PositionVector(2)	= h + 1.0f;
	plPlacement.pl_PositionVector(3)	= z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f ;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;
	
	for( i = 0; i < _pNetwork->ga_srvServer.srv_actWildPet.Count(); i++)
	{
		if(_pNetwork->ga_srvServer.srv_actWildPet[i].m_nNetIndex == iNetID)
			return;		
	}

	CEntity* penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_WILDPET, -1, TRUE);

	CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet.Push();
	CTString strWildPetName = strPetName;
	pt.m_nIndex = index;
	pt.m_nLevel = iLevel;
	pt.m_strName = strWildPetName;
	pt.m_nHP = hp;
	pt.m_nMaxHP = maxHP;
	pt.m_nNetIndex = iNetID;
	pt.m_nType = nPetType;
	pt.m_nOwnerIndex = lOwnerIndex;
	pt.m_sbYlayer = yLayer;
	pt.m_sbAttributePos = sbAttributePos;
	pt.pet_iClientIndex = penEntity->en_ulID;
	penEntity->en_pWildPetInfo	= &pt;	

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetWildPetData( penEntity, hp, maxHP );

	penEntity->InitAsSkaModel();
	penEntity->SetSkaModel(_pNetwork->wo_aWildPetData[index-1].m_WildPetData.strFileName);
	penEntity->SetNetworkID(iNetID);
	pt.SetWildPetToEntity(penEntity, index-1, nItemIndex, nItemPlus);
	penEntity->Initialize();
	penEntity->FallDownToFloor();
	for ( i = 0; i < 4; i++)
	{
		pt.m_nPetWearIndex[i] = nItemIndex[i];
		pt.m_nPetWearPlus[i] = nItemPlus[i];

		_pGameState->WearingArmorTest(penEntity->GetModelInstance(), pt.m_nPetWearIndex[i]);
	}
	
	if(pt.m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
	{
		pt.bIsActive = TRUE;
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET);
		_pNetwork->_WildPetInfo = pt;
		_pUIMgr->RearrangeOrder(UI_WILDPETTARGETINFO,TRUE);
		/*_pUIMgr->GetWildPetInfo()->sendSkillList();*/
	}
	

}

//안태훈 수정 끝	//(5th Closed beta)(0.2)
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveNpcRegenMessage(CNetworkMessage *istr)
{
	INDEX iIndex;
	SBYTE sbYLayer;//1013
	INDEX iMobType;
	INDEX iEntityID;
	CPlacement3D plPlacement = CPlacement3D(FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D(0.0f, 0.0f, 0.0f));
	CEntity* penEntity = NULL;

	(*istr) >> iIndex;								// Index
	(*istr) >> iMobType;							// DB Index
	(*istr) >> plPlacement.pl_PositionVector(1);	// Pos X
	(*istr) >> plPlacement.pl_PositionVector(3);	// Pos Z
	(*istr) >> plPlacement.pl_PositionVector(2);	// Pos H
	(*istr) >> plPlacement.pl_OrientationAngle(1);	// Angle
	(*istr) >> sbYLayer;							// Y Layer
	(*istr) >> iEntityID;							// Entity ID

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	CMobData& MD = _pNetwork->GetMobData(iMobType);
	if(MD.GetMobIndex() == -1)
	{
		OutputDebugString("Cannot find Mob Data!!!\n");
		ASSERTALWAYS("Cannot find Mob Data!!!\n");
		return;
	}

	CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob.Push();
		
	penEntity = _pNetwork->ga_World.EntityFromID(iEntityID);
	penEntity->SetNetworkID(iIndex);

	BOOL bNpc = FALSE;
	if(penEntity->IsFirstExtraFlagOn(ENF_EX1_NPC))
	{
		bNpc = TRUE;
	}

	const char *szMobName = _pNetwork->GetMobName(iMobType);
	CTString strMobName = szMobName;
	mt.SetData( iIndex, iMobType, strMobName, MD.GetLevel(), penEntity, bNpc, sbYLayer );//0826

	if(penEntity != NULL)
	{
		// FIXME : 에너미 Spawner과 중복되는 부분.
		penEntity->SetSkaModel( MD.GetMobSmcFileName() );
		CMobData::SetMobDataToNPC( penEntity, MD, szMobName );
		// 싱글모드일때 생성된 엔티티의 갯수를 저장함.
		if(_pNetwork->m_bSingleMode && 
			!penEntity->IsFirstExtraFlagOn( ENF_EX1_NPC ))
		{
			_pNetwork->wo_dwEnemyCount++;
			if(_pNetwork->m_ubGMLevel > 1)
			{
				CTString strMessage;
				strMessage.PrintF("=====spawned enemy : %d=====\n", _pNetwork->wo_dwEnemyCount);
				_pNetwork->ClientSystemMessage(strMessage);
			}
		}

		// 생성될때 애니메이션이 필요한 에너미들...
		const int iWanderingZombie	= 35;		// 방황하는 좀비
		const int iZombie			= 51;		// 좀비
		const int iDamd				= 142;		// 뎀드(스펠링 맞나???)

		// FIXME : 임시로 하드 코딩한 부분.
		// FIXME : 맘에 들지 않음.
		if( iMobType == iWanderingZombie || iMobType == iZombie || iMobType == iDamd )		// 좀비 & 뎀드
		{
			CEntityProperty &epPropertyStart = *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 99));	// Walk Animation
			ENTITYPROPERTY( &*penEntity, epPropertyStart.ep_slOffset, INDEX)		= ska_GetIDFromStringTable("m_zm_up01");
		}
		mt.mob_iClientIndex	= penEntity->en_ulID;
		penEntity->Initialize();
		penEntity->en_pMobTarget = &mt;
		
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetMobData(penEntity, MD.GetHealth(), MD.GetHealth(), MD.GetLevel(),FALSE,-1);
		penEntity->FallDownToFloor();
		CPrintF("---Mob Created! S Index : %d, C Index : %d\n", iIndex, mt.mob_iClientIndex);
	}
//안태훈 수정 끝	//(5th Closed beta)(0.2)ㅇ
	// NOTE : 11번 던젼의 경우에만...
	if( g_slZone == 11 )
	{
		// NOTE : 밴더스내치 일경우에는 인터페이스를 띄우지 않음.
		if( iMobType != 220 )
			_pUIMgr->GetSingleBattle()->OpenSingleBattle();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveDisappearMessage(CNetworkMessage *istr)
{
	SBYTE	type; //MSG_CHAR_PC or MSG_CHAR_NPC
	ULONG	index;
				
	(*istr) >> type;
	(*istr) >> index;

	if(type == MSG_CHAR_PC)
	{
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ipl++ )
		{
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];					

			if( ct.cha_Index == index )
			{
				// wooss 070305 ----------------------------------->>
				// kw : WSS_WHITEDAY_2007
				// 6종 러브매직 스킬
				// 423 424 425 426 427 428
				for(int i=0;i<20;i++)
				{				
					if( ct.cha_Buff[i].m_pEG !=NULL)
					{
						DestroyEffectGroup(ct.cha_Buff[i].m_pEG);
					}
				}
				
				// -------------------------------------------------<<

				// Date : 2005-07-13(오후 5:30:06), By Lee Ki-hwan
				// 공성 Effect Stop
				_pUISWDoc->StopEffect( index );
				_pUISWDoc->StopGuildMasterEffect( index );

				// Date : 2005-04-06(오후 5:05:02), By Lee Ki-hwan
				// 해당 캐릭터의 Index 바탕으로 Guild Effect를 삭제해줌
				_pUIMgr->GetGuildBattle()->StopGuildEffect ( index );

#ifdef TARGET_MARK
				// 해당 캐릭터의 타겟 이펙트를 없앰...
				_pUIMgr->StopTargetEffect( index );
#endif
#ifdef DRATAN_SIEGEWARFARE
				// WSS_DRATAN_SIEGEWARFARE 2007/10/14
				_pUISWDoc->StopConsensusEffect(ct.cha_Index); // WSS_DRATAN_SIEGEWARFARE 2007/10/14		
#endif
				ct.cha_itemEffect.DelAccessoryEffect();	// 서포터 이펙트 등, 악세사리 이펙트 삭제
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(ct.cha_pEntity);

				ct.Init();
				_pNetwork->ga_srvServer.srv_actCha.SwapAndPop(ipl);
				break;
			}
		}
	}
	else if(type == MSG_CHAR_NPC)
	{					
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_amtMob.Count(); ipl++ )
		{
			CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[ipl];
			if( mt.mob_Index == index )
			{  
				// wooss 060908
				// 전사의 손길 시스템 추가로...
				if(		mt.mob_iType == 310 ||	mt.mob_iType == 311 
					||	mt.mob_iType == 312 ||	mt.mob_iType == 313 )
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DropDeathItem(mt.mob_pEntity);

#ifdef TARGET_MARK
				//해당 NPC의 타겟 이펙트를 없앰...
				_pUIMgr->StopTargetEffect( index );
#endif

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(mt.mob_pEntity);

				if (mt.mob_iType == MOB_FLOWERTREE_INDEX)
				{ // 꽃놀이 나무 이펙트 정보 삭제
					_pUIMgr->GetFlowerTree()->ClearEffect();
				}

				mt.Init();
				_pNetwork->ga_srvServer.srv_amtMob.SwapAndPop(ipl);

				break;
			}
		}
	}
	// 애완동물 DISAPPEAR처리...
	else if(type == MSG_CHAR_PET)
	{
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actPet.Count(); ipl++ )
		{
			CPetTarget	&pt = _pNetwork->ga_srvServer.srv_actPet[ipl];

			if( pt.pet_Index == index )
			{
				// 내 애완동물인 경우...
				if( pt.pet_OwnerIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->_PetTargetInfo.Init();
				}

#ifdef TARGET_MARK				
				//해당 펫의 타겟 이펙트를 없앰...
				_pUIMgr->StopTargetEffect( index );
#endif

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pt.pet_pEntity);

				pt.Init();
				_pNetwork->ga_srvServer.srv_actPet.SwapAndPop(ipl);
				break;
			}
		}
	}
	// 소환수 DISAPPEAR처리...
	else if(type == MSG_CHAR_ELEMENTAL)
	{
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actSlave.Count(); ipl++ )
		{
			CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[ipl];

			if( st.slave_Index == index )
			{
				// 내 소환수인 경우...
				if( st.slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
				{
					for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
					{
						CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
						if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == index )
						{
							pUISummon->WarpOfResetSummon();							
						}
					}
				}
				
				SlaveInfo().StopIdleEffect( index );		

#ifdef TARGET_MARK
				//해당 소환수의 타겟 이펙트를 없앰...
				_pUIMgr->StopTargetEffect( index );
#endif				

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(st.slave_pEntity);

				st.Init();
				_pNetwork->ga_srvServer.srv_actSlave.SwapAndPop(ipl);
				break;
			}
		}
	}
	else if(type == MSG_CHAR_WILDPET)
	{
		for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl++ )
		{
			CWildPetInfo	&pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl];

			if( pt.m_nNetIndex == index )
			{
#ifdef TARGET_MARK				
				//해당 펫의 타겟 이펙트를 없앰...
				_pUIMgr->StopTargetEffect( index );
#endif

			//	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pt.pet_pEntity);

				if (pt.m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
				{
					_pUIMgr->RearrangeOrder(UI_WILDPETTARGETINFO,FALSE);
					_pNetwork->_WildPetInfo.Init();
					_pUIMgr->GetWildPetInfo()->AIClear();
					_pUIMgr->GetQuickSlot()->RemoveWildPetSkill();
				}
				pt.Init();
				_pNetwork->ga_srvServer.srv_actWildPet.SwapAndPop(ipl);
				break;
			}
		}

	}

}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveAtMessage(CNetworkMessage *istr)
{
	SLONG	 index;
	CTString name;
	SBYTE	 job;
	SBYTE	 job2;
	SLONG	 zoneNo;
	SLONG	 areaNo;
	FLOAT	 x;
	FLOAT	 z;
	FLOAT	 h;
	FLOAT	 r; //방향
	SBYTE	 yLayer;//1013
	SBYTE	hairstyle;//헤어 스타일 //1013
	SBYTE	facestyle;//얼굴 스타일
	SLONG	userIndex;
	LONG	lGuildOutDate;
	SBYTE	sbItemPlusEffect;
	SBYTE	sbAttributePos;
	LONG	lchatflag = 0;
	
	(*istr) >> index;
	(*istr) >> name;
	(*istr) >> job;
	(*istr) >> job2;
	(*istr) >> hairstyle;//1013
	(*istr) >> facestyle;			
	(*istr) >> zoneNo;
	(*istr) >> areaNo;
	(*istr) >> x;
	(*istr) >> z;
	(*istr) >> h;
	(*istr) >> r;	
	(*istr) >> yLayer;
	(*istr) >> userIndex;
	(*istr) >> lGuildOutDate;	
	(*istr) >> sbItemPlusEffect;
	(*istr) >> sbAttributePos;

	//0707
#ifdef SECRET_KEY
	ULONG secretkey;
	(*istr) >> secretkey;

	secretkey = (secretkey >> 1) - index; 

	_pNetwork->MyCharacterInfo.secretkey = (SLONG)secretkey;
#endif

//[ttos_2009_1_23]: 채팅 금지
#ifdef CHATTING_BAN
	(*istr) >> lchatflag;
#endif

	_pNetwork->MyCharacterInfo.index = index;
	_pNetwork->MyCharacterInfo.name = name;
	_pNetwork->MyCharacterInfo.job = job;
	_pNetwork->MyCharacterInfo.job2 = job2;
	_pNetwork->MyCharacterInfo.zoneNo = zoneNo;
	_pNetwork->MyCharacterInfo.areaNo = areaNo;
	_pNetwork->MyCharacterInfo.x = x;
	_pNetwork->MyCharacterInfo.z = z; 
	_pNetwork->MyCharacterInfo.h = h;
	_pNetwork->MyCharacterInfo.r = r;	
	_pNetwork->MyCharacterInfo.yLayer = yLayer;
	_pNetwork->MyCharacterInfo.userIndex = userIndex;
	_pNetwork->MyCharacterInfo.hairStyle = hairstyle;
	_pNetwork->MyCharacterInfo.faceStyle = facestyle;
	_pNetwork->MyCharacterInfo.pk_mode   = 0; 
	_pNetwork->MyCharacterInfo.sbShopType		= PST_NOSHOP;
	_pNetwork->MyCharacterInfo.lGuildIndex		= -1;
	_pNetwork->MyCharacterInfo.lGuildLevel		= -1;
	_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;
#ifdef DISPLAY_GUILD_RANK
	_pNetwork->MyCharacterInfo.sbGuildRank		= 0;
#endif
	_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;	
	_pNetwork->MyCharacterInfo.fame				= 0;	
	_pNetwork->MyCharacterInfo.strTeacherName.Clear();
	_pNetwork->MyCharacterInfo.bExtension = FALSE;
	// 존 이동을 했을때 캐릭터의 상태중에서 몇가지 정보를 초기화하지 않고 유지하도록 함...
//	_pNetwork->MyCharacterInfo.bExtension		= FALSE;
	_pNetwork->MyCharacterInfo.sbItemEffectOption = sbItemPlusEffect;	
//	_pNetwork->MyCharacterInfo.sbEvocationType	= -1;
	if(sbAttributePos != ATTC_UNWALKABLE )
	{
		_pNetwork->MyCharacterInfo.sbAttributePos = sbAttributePos;
	}
//[ttos_2009_1_23]:채팅 금지
#ifdef CHATTING_BAN
	_pNetwork->MyCharacterInfo.ChatFlag = lchatflag;
#endif
//안태훈 수정 시작	//(Zone Change System)(0.1)
	g_slZone = zoneNo;
	_pNetwork->ga_sesSessionState.ses_bWantPause = FALSE;
//안태훈 수정 끝	//(Zone Change System)(0.1)

	CEntity::GetPlayerEntity(0)->SetNetworkID(index);
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_AT, &(*istr));	 

	_pUIMgr->GetMap()->SetCurrentWorldMap( zoneNo, yLayer );		// yjpark
	_pUIMgr->GetCharacterInfo()->RegisterActions();					// yjpark

	_pNetwork->SendRegenList();

	// Date : 2005-11-08(오후 6:39:20), By Lee Ki-hwan
	_pUIMgr->SetUIGameState( UGS_GAMEON );

	if( _pUIMgr->IsCSFlagOn( CSF_PARTY ) )
	{
		_pUIMgr->RearrangeOrder( UI_PARTY, TRUE );
	}

	// [090715: selo] 레이드에서 나갈때는 퀘스트 북의 레이드 메시지를 비운다
	if(!_pUIMgr->IsPlayInZone()) //인스턴트존내부에서 나갈경우
	{		
		_pUIMgr->GetQuestBookList()->RemoveRaidMessageAll();		
	}
//	CPrintF(TRANS("MSG_AT : (name)%s (job)%d (zoneNo)%d (areaNo)%d (x)%f (z)%f (h)%f (r)%f (yLayer)%d \n"), name,job,zoneNo,areaNo,x,z,h,r,yLayer);	
}

void CSessionState::ReceiveStatusMessage(CNetworkMessage *istr)
{
	SLONG level, hp, maxHP, mp, maxMP, str, dex, _int, con, opt_str, opt_dex, opt_int, opt_con;
	SLONG attack, magic, defense, resist;
	SLONG sp, weight, maxweight;
	FLOAT walkspeed, runspeed,attackrange;
	SBYTE attackspeed, magicspeed;
	SQUAD curExp, needExp;
	SBYTE pktitle;
	SLONG pkpenalty, pkcount;
	SLONG fame;
	SBYTE sbJoinFlagMerac;
// WSS_DRATAN_SIEGEWARFARE 070720
#ifdef DRATAN_SIEGEWARFARE
	SBYTE sbJoinFlagDratan;	
#endif
	SLONG skillSpeed;
	UBYTE	ubAttributePos;
	SBYTE	sbMountPet;
	SBYTE	sbPetColor =0;
	SBYTE	sbEvocationType;
	SLONG	lEvocationRemain;
	SBYTE	sBPresscorps = 0;
	//[ttos_2009_1_15]:쿨타임 감소 및 MP량 감소 옵션을 클라에서 계산하지 않고 서버에서 받음
	INDEX	nCoolTimeReductionRate = 0;
	INDEX	nMPReductionRate = 0;

// 080901 UI개편
#ifdef NEW_USER_INTERFACE
		SLONG			baseHP, baseMP, addedAttack, addedMagic, addedDefense, addedResist; //기본HP, 기본 MP, 추가공격력, 추가마법력, 추가방어력, 추가저항력
		SLONG			critical, baseCritical, deadly, baseDeadly; // 크리티컬, 기본 크리티컬, 데들리, 기본 데들리
		SLONG			dodgeRate, baseDodgeRate, magicDodgeRate, baseMagicDodgeRate, hitRate, baseHitRate, magicHitRate, baseMagicHitRate;
		FLOAT			baseRunSpeed;
		SBYTE           baseAttackSpeed;
		// 물리회피, 기본물리회피, 마법회피, 기본마법회피, 물리명중, 기본물리명중, 마법명중, 기본마법명중
#endif


	(*istr) >> level
			>> curExp
			>> needExp
			>> hp
			>> maxHP
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> baseHP	// 기본 HP
	#endif
#endif
			>> mp
			>> maxMP
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> baseMP // 기본 MP
	#endif
#endif
			>> str
			>> dex
			>> _int
			>> con

			>> opt_str
			>> opt_dex
			>> opt_int
			>> opt_con

			>> attack
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> addedAttack // 추가공격력
	#endif
#endif
			>> magic
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> addedMagic // 추가마법력
	#endif
#endif
			>> defense
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> addedDefense // 추가방어력
	#endif
#endif
			>> resist
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> addedResist // 추가 저항력

			>> dodgeRate	// 물리 회피도
			>> baseDodgeRate // 기본 물리 회피도
			>> magicDodgeRate // 매직 회피도
			>> baseMagicDodgeRate // 기본매직 회피도
			>> hitRate // 물리명중도
			>> baseHitRate // 기본물리명중도
			>> magicHitRate // 마법명중도
			>> baseMagicHitRate // 기본 마법명중도
			>> critical // 크리티컬 타격
			>> baseCritical // 기본 크리티컬 타격
			>> deadly // 데들리 타격
			>> baseDeadly // 기본 데들리 타격
	#endif
#endif
			>> sp
			>> weight
			>> maxweight
			>> walkspeed
			>> runspeed
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> baseRunSpeed // 기본 이속
	#endif
#endif
			>> attackspeed	
#ifdef NEW_USER_INTERFACE
	#ifdef NEW_USER_INTERFACE_MESSAGE
			>> baseAttackSpeed // 기본 공속
	#endif
#endif
			>> magicspeed
			>> pktitle  
			>> pkpenalty
			>> pkcount
			>> fame
			>> attackrange
			>> sbJoinFlagMerac
// WSS_DRATAN_SIEGEWARFARE 070720
#ifdef DRATAN_SIEGEWARFARE
			>> sbJoinFlagDratan
#endif
			>> skillSpeed
			>> ubAttributePos
			>> sbMountPet
		// [070824: Su-won] PET_COLOR_CHANGE
#ifdef PET_COLOR_CHANGE
			>> sbPetColor
#endif
			>> sbEvocationType
			>> lEvocationRemain
			
			// 080623 라카 기자단 혜택
#ifdef LAKA_Presscorp
			>> sBPresscorps // 미국은 라카 기자단 없음
#endif
			>> nCoolTimeReductionRate
			>> nMPReductionRate
			;

	// edit by cpp2angel (044.12.20) : 자동 도움말
	if ( _pNetwork->MyCharacterInfo.sp < sp )
	{
		_UIAutoHelp->SetInfo ( AU_GET_SKILL_POINT );
	}

	_pNetwork->MyCharacterInfo.level = level;
	static int iOldLevel = -1;
	if(iOldLevel != level)
	{
		//모든 npc의 quest mark 갱신.
		for(int i=0; i<_pNetwork->ga_srvServer.srv_amtMob.Count(); ++i)
		{
			CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[i];
			if(mt.mob_Index != -1 && mt.IsNPC())
			{
				CQuestSystem::Instance().RefreshNPCQuestMark(mt.mob_iType);
			}
		}
		iOldLevel = level;
	}
	_pNetwork->MyCharacterInfo.curExp = curExp; 
	_pNetwork->MyCharacterInfo.needExp = needExp; 
	_pNetwork->MyCharacterInfo.hp = hp; 
	_pNetwork->MyCharacterInfo.maxHP = maxHP;
	_pNetwork->MyCharacterInfo.mp = mp; 
	_pNetwork->MyCharacterInfo.maxMP = maxMP; 
	_pNetwork->MyCharacterInfo.str = str; 
	_pNetwork->MyCharacterInfo.dex = dex;
	_pNetwork->MyCharacterInfo.intel = _int; 
	_pNetwork->MyCharacterInfo.con = con; 
	_pNetwork->MyCharacterInfo.opt_str = opt_str; 
	_pNetwork->MyCharacterInfo.opt_dex = opt_dex;
	_pNetwork->MyCharacterInfo.opt_intel = opt_int; 
	_pNetwork->MyCharacterInfo.opt_con = opt_con; 
	_pNetwork->MyCharacterInfo.sp = sp; 
	_pNetwork->MyCharacterInfo.weight = weight; 
	_pNetwork->MyCharacterInfo.maxweight = maxweight; 
	_pNetwork->MyCharacterInfo.walkspeed = walkspeed; 
	_pNetwork->MyCharacterInfo.runspeed = runspeed; 
	_pNetwork->MyCharacterInfo.attackspeed = attackspeed; 
	_pNetwork->MyCharacterInfo.magicspeed = magicspeed;
	_pNetwork->MyCharacterInfo.pktitle			= pktitle; 
	_pNetwork->MyCharacterInfo.pkpenalty		= pkpenalty; 
	_pNetwork->MyCharacterInfo.pkcount			= pkcount;
	_pNetwork->MyCharacterInfo.fame				= fame;
	_pNetwork->MyCharacterInfo.attackrange		= attackrange;
	_pNetwork->MyCharacterInfo.sbJoinFlagMerac	= sbJoinFlagMerac;
#ifdef DRATAN_SIEGEWARFARE
	_pNetwork->MyCharacterInfo.sbJoinFlagDratan	= sbJoinFlagDratan; // WSS_DRATAN_SIEGEWARFARE 070720
#endif
	_pNetwork->MyCharacterInfo.skillSpeed		= skillSpeed;
	_pNetwork->MyCharacterInfo.sbAttributePos	= ubAttributePos;
	_pNetwork->MyCharacterInfo.sbMountPet		= sbMountPet;
	_pNetwork->MyCharacterInfo.sbEvocationType	= sbEvocationType - 1;

	// 080623 라카 기자단 혜택
	_pNetwork->MyCharacterInfo.sbPresscorps		= sBPresscorps;

#ifdef NEW_USER_INTERFACE
	_pNetwork->MyCharacterInfo.baseHP		= baseHP;
	_pNetwork->MyCharacterInfo.baseMP 		= baseMP;
	_pNetwork->MyCharacterInfo.addedAttack 		= addedAttack;
	_pNetwork->MyCharacterInfo.addedMagic 		= addedMagic;
	_pNetwork->MyCharacterInfo.addedDefense 		= addedDefense;
	_pNetwork->MyCharacterInfo.addedResist 		= addedResist;

	_pNetwork->MyCharacterInfo.dodgeRate		= dodgeRate;
	_pNetwork->MyCharacterInfo.baseDodgeRate 		= baseDodgeRate;
	_pNetwork->MyCharacterInfo.magicDodgeRate 		= magicDodgeRate;
	_pNetwork->MyCharacterInfo.baseMagicDodgeRate 		= baseMagicDodgeRate;
	_pNetwork->MyCharacterInfo.hitRate 		= hitRate;
	_pNetwork->MyCharacterInfo.baseHitRate 		= baseHitRate;

	_pNetwork->MyCharacterInfo.magicHitRate		= magicHitRate;
	_pNetwork->MyCharacterInfo.baseMagicHitRate 		= baseMagicHitRate;
	_pNetwork->MyCharacterInfo.critical 		= critical;
	_pNetwork->MyCharacterInfo.baseCritical 		= baseCritical;
	_pNetwork->MyCharacterInfo.deadly 		= deadly;
	_pNetwork->MyCharacterInfo.baseDeadly 		= baseDeadly;

	_pNetwork->MyCharacterInfo.baseRunSpeed 		= baseRunSpeed;
	_pNetwork->MyCharacterInfo.baseAttackSpeed 		= baseAttackSpeed;
#endif

	_pUIMgr->GetPlayerInfo()->UpdateAllInfos();			// yjpark
	_pUIMgr->GetCharacterInfo()->UpdateStatus( attack, magic, defense, resist);		// yjpark

	//[ttos_2009_1_15]:쿨타임 감소 및 MP량 감소 옵션을 클라에서 계산하지 않고 서버에서 받음
	_pUIMgr->SetCoolTimeReductionRate(nCoolTimeReductionRate);
	_pUIMgr->SetNeedMPReductionRate(nMPReductionRate);

	//CPrintF(TRANS("MSG_STATUS1: (level)%d (curExp)%I64d (needExp)%I64d (HP)%d/%d (MP)%d/%d (str)%d (dex)%d\n"), level, curExp, needExp, hp, maxHP, mp, maxMP, str, dex);
	//CPrintF(TRANS("MSG_STATUS2: (int)%d (con)%d (opt_str)%d (opt_dex)%d (opt_int)%d (opt_con)%d (attack)%d (magic)%d\n"), _int, con, opt_str, opt_dex, opt_int, opt_con, attack, magic);
	//CPrintF(TRANS("MSG_STATUS3: (defence)%d (resist)%d (sp)%d (weight)%d (maxweight)%d (walkspd)%f (runspd)%f (attackspd)%d  \n"), defense, resist, sp, weight, maxweight, walkspeed, runspeed, attackspeed);

	
	// Date : 2005-09-07(오후 5:31:54), By Lee Ki-hwan
	// sbMountPet : 팻을 타고있는지에 대한 정보
	// 0 타고 있지 않음, 
	// 그외 PetType과 Age가 bit정보로 옴.
	if( sbMountPet ) 
	{
		//-----------------------------------------------------------	
		INDEX iPetType	= -1;
		INDEX iPetAge	= -1;
		_pNetwork->CheckPetType( sbMountPet, iPetType, iPetAge );
		
		// NOTE : 탈수 있는 애완동물인지 판단하여, 캐릭터를 태웁니다.
		// NOTE : 애완동물을 타고 있을 경우에는, Pet타겟 정보를 추가하지 않음(내 애완동물일때는 펫 타겟 정보창 설정할것...)
		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);		
		
		if( bPetRide )
		{
			//-----------------------------------------------------------
			if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsRidePet(CEntity::GetPlayerEntity(0)) )
			{				
				_pNetwork->_PetTargetInfo.bIsActive		= TRUE;			
				_pNetwork->_PetTargetInfo.pen_pEntity	= NULL;
				_pNetwork->_PetTargetInfo.iType			= iPetType;
				_pNetwork->_PetTargetInfo.iAge			= iPetAge;				
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= _pNetwork->_PetTargetInfo.lIndex;				
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					_pNetwork->_PetTargetInfo.iLevel		= (*iter).lLevel;
					_pNetwork->_PetTargetInfo.fHealth		= (*iter).lHP;
					_pNetwork->	_PetTargetInfo.fMaxHealth	= (*iter).lMaxHP;
					_pNetwork->_PetTargetInfo.fMaxHungry	= (*iter).lMaxHungry;
					_pNetwork->_PetTargetInfo.fHungry		= (*iter).lHungry;
					_pNetwork->_PetTargetInfo.lAbility		= (*iter).lAbility;
					_pNetwork->_PetTargetInfo.bIsActive		= TRUE;
					_pNetwork->_PetTargetInfo.lIndex		= TempPet.lIndex;
					_pNetwork->_PetTargetInfo.iType			= iPetType;
					_pNetwork->_PetTargetInfo.iAge			= iPetAge;						
					_pNetwork->_PetTargetInfo.strNameCard	= (*iter).strNameCard;
				}

				// [070824: Su-won] PET_COLOR_CHANGE
				INDEX iPetColoredType = iPetType | (sbPetColor<<8);
				_pNetwork->RidePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0), NULL, iPetColoredType );
			}
		}
		//_pNetwork->MyCharacterInfo.bPetRide = bPetRide;
	}

	if( sbEvocationType != -1)
	{
		_pUIMgr->GetNotice()->SetLeftTime(lEvocationRemain);
	}

	if(_pNetwork->MyCharacterInfo.hp==0)
	{		
		CEntity* penPlEntity;
		CPlayerEntity* penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
		penPlayerEntity = (CPlayerEntity*) penPlEntity;
		if(!penPlayerEntity->IsAlreadyDie())
		{
		//	penPlayerEntity->DeathYou();
			penPlayerEntity->SetDie();
			_pNetwork->MyCharacterInfo.statusEffect.Reset();
		}
		// WSS_DRATAN_SIEGEWARFARE 2007/10/17
		// 공성 부활대기시 죽었을 경우 처리		
		if( !_pUIMgr->GetSiegeWarfareNew()->GetWarState()&&
			(_pUIMgr->GetSiegeWarfareNew()->GetWaitTime()>=0))
		{			
			penPlayerEntity->DeathYou();
		}
	}
}

void CSessionState::ReceiveMoveMessage(CNetworkMessage *istr)
{
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

	SBYTE	type;							
	(*istr) >> type;

	if(type == MSG_CHAR_PC)
	{
		penPlayerEntity->Read_net_Character(MSG_MOVE, &(*istr));	
	}
	else if(type == MSG_CHAR_NPC)
	{
		penPlayerEntity->Read_net_Mob(MSG_MOVE, &(*istr));	
	}
	else if(type == MSG_CHAR_PET)
	{
		penPlayerEntity->Read_net_Pet(MSG_MOVE, &(*istr));
	}
	else if(type == MSG_CHAR_ELEMENTAL)
	{
		penPlayerEntity->Read_net_Summon(MSG_MOVE, &(*istr));
	}else if(type == MSG_CHAR_WILDPET)
	{
		penPlayerEntity->Read_net_WildPet(MSG_MOVE, &(*istr));
	}
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
// 서버간에 이동이 있을때 쓰는 게 GO_ZONE
// 이동이 없을때 쓰는 게 GO_TO
// FiXME : 아래 함수 정리 필요함.
// FIXME : Restart_Internal()부분과 중복되는 부분이 너무 많음.  초기화하는 부분만 따로 빼낼것.
void CSessionState::ReceiveGoToMessage(CNetworkMessage *istr)
{
	SLONG zone;
//	FLOAT x, z, h, r, yLayer;

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	//0917
	_pNetwork->_TargetInfo.Init();
	_pNetwork->_TargetInfoReal.Init();

	(*istr) >> zone;
	if(zone == g_slZone)	//현재 world내 이동
	{
/*
		(*istr) >> x;
		(*istr) >> z;
		(*istr) >> h;
		(*istr) >> r;
		(*istr) >> yLayer;

		CPlacement3D plWhere=CPlacement3D(FLOAT3D(x,h,z),ANGLE3D(r,0,0));
		penPlayerEntity->Teleport(plWhere, FALSE);
*/
		penPlayerEntity->Read_net_Character(MSG_GOTO, &(*istr));
	}
	else	//다른 world(zone)로 이동, 재접필요없이 클라이언트 내부 처리.
	{
		g_slZone = zone;//0921

		if(ZoneInfo().GetZoneType(zone) == ZONE_SDUNGEON)
		{
			CPrintF("===Start Single Mode===\n");
			_pNetwork->m_bSingleMode = TRUE;
			_pNetwork->wo_dwEnemyCount		= 0;
			_pNetwork->wo_dwKilledEnemyCount= 0;
			penPlayerEntity->SingleModeOn();
		}
		else
		{
			CPrintF("===End Single Mode===\n");
			_pNetwork->m_bSingleMode = FALSE;
			_pUIMgr->GetSingleBattle()->Close();
			penPlayerEntity->FieldModeOn();
		}

		_pUIMgr->SetUIGameState( UGS_GAMELOADING );			// yjpark

		_pNetwork->m_ubGMLevel				= 0;
		_pNetwork->TogglePause();
		_pNetwork->ChangeLevel(ZoneInfo().GetZoneWldFile(zone), FALSE, 0);

		// Date : 2006-04-26(오전 11:48:00), By eons
		if( penPlayerEntity->IsPolymophing() )	// 변신 중이라면
		{
			penPlayerEntity->ReturnChange( TRUE );
			const int iStopChangeItem = 521;
			_pUIMgr->GetNotice()->DelFromNoticeList(iStopChangeItem, NOTICE_POLYMOPH);
			_pUIMgr->GetNotice()->RefreshNoticeList();			
		}

		// 인벤 비우기.
		for( int a = 0; a < INVEN_SLOT_TAB; a++ )
		{
			for( int b = 0; b < INVEN_SLOT_ROW_TOTAL; b++ )
			{
				for( int c = 0; c < INVEN_SLOT_COL; c++ )
				{
					_pNetwork->MySlotItem[a][b][c].Init();
				}
			}
		}

		/*
		for( int i = 0; i < WEAR_TOTAL; ++i )
		{			
			//_pNetwork->MyCurrentWearing[i].Init();
			_pNetwork->pMyCurrentWearing[i] = NULL;
			penPlayerEntity->DeleteDefaultArmor(i);//0914 안전코드.
			penPlayerEntity->DeleteCurrentArmor(i);//0922
			_pNetwork->DeleteMyCurrentWearing(i);
		}
		*/

		_pUIMgr->GetSummonFirst()->ResetSummon();
		_pUIMgr->GetSummonSecond()->ResetSummon();

		_pUIMgr->GetPersonalShop()->ResetShop();
		_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
		_pUIMgr->SetCSFlagOff( CSF_MOONSTONE );
		_pUIMgr->SetCSFlagOff( CSF_MOUNT_HUNGRY );
		_pUIMgr->GetGuild()->ResetGuild();
		_pUIMgr->GetGuild()->ClearMemberList();
		_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );

//		Bug Fix 060530 wooss
//		_pNetwork->ClearPetList();
//		_pNetwork->_PetTargetInfo.Init();
		_pNetwork->_SlaveTargetInfo[0].Init();
		_pNetwork->_SlaveTargetInfo[1].Init();
		
		_pUIMgr->GetHelper()->ResetHelper();
		_pUIMgr->GetHelper()->ClearHelperList();
		
		penPlayerEntity->ClearMultiTargets();

		_pNetwork->DeleteAllMob();
		_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL; // 초기화

		EPreLevelChange ePreChange;
		ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePreChange);
		CEntity::HandleSentEvents();
		_pNetwork->ChangeLevel_internal();
		EPostLevelChange ePostChange;
		ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePostChange);
		CEntity::HandleSentEvents();
		_lphCurrent=LCP_NOCHANGE;
		CNetworkMessage nmStartGame(MSG_START_GAME); 					 
		_pNetwork->SendToServerNew(nmStartGame,TRUE);	
		_cmiComm.Server_Update();
//강동민 수정 시작 싱글던젼 스토리	10.07
		// TargetNPC는 대부분 미리 생성되야할 엔티티들임.
		for(std::vector<CEntity*>::iterator iter = _pNetwork->ga_World.m_vectorPreCreateNPC.begin(); iter != _pNetwork->ga_World.m_vectorPreCreateNPC.end(); ++iter)
		{
			(*iter)->Reinitialize();
		}
		_pNetwork->ga_World.m_vectorPreCreateNPC.clear();
		_cmiComm.Server_Update();

		/*
		if(_pNetwork && _pNetwork->m_bSingleMode && _pNetwork->ga_World.wo_iNumOfNPC)
		{
			CEntity* penPlayer = CEntity::GetPlayerEntity(0);
			_pNetwork->ga_World.m_vectorTargetNPC.push_back(penPlayer);
		}
		*/
//강동민 수정 끝 싱글던젼 스토리	10.07
	}
}

// [KH_070316] 변경 프리미엄 메모리 관련
void CSessionState::ReceiveMemPosMessage(UBYTE recvMSG, CNetworkMessage *istr)
{
	UBYTE		ubMemposType, ubSlot;
	SLONG		slZone;
	FLOAT		fX, fZ;
	CTString	strComment;
	ULONG		useTime;			

	(*istr) >> ubMemposType;

	if( ubMemposType == MSG_MEMPOS_LIST )
	{	
// [KH_070316] 변경 프리미엄 메모리 관련
		if(recvMSG == MSG_MEMPOS)
			_pUIMgr->GetTeleport()->ClearTeleportList();
		else if(recvMSG == MSG_MEMPOSPLUS)
			_pUIMgr->GetTeleportPrimium()->ClearTeleportList();

		UBYTE	ubCount;
		(*istr) >> ubCount;
				
		for( UBYTE i = 0; i < ubCount; ++i )
		{
			(*istr) >> ubSlot;
			(*istr) >> slZone;
			(*istr) >> fX;
			(*istr) >> fZ;
			(*istr) >> strComment;

// [KH_070316] 변경 프리미엄 메모리 관련
			if(recvMSG == MSG_MEMPOS)
				_pUIMgr->GetTeleport()->SetTeleportInfo( ubSlot, slZone, fX, fZ, strComment );
			else if(recvMSG == MSG_MEMPOSPLUS)
				_pUIMgr->GetTeleportPrimium()->SetTeleportInfo( ubSlot, slZone, fX, fZ, strComment );
		}
		(*istr)	>> useTime;
		_pUIMgr->GetTeleport()->SetUseTime(useTime);	//wooss
	}
	else if(ubMemposType == MSG_MEMPOS_WRITE)
	{
		(*istr) >> ubSlot;
		(*istr) >> strComment;
		(*istr) >> slZone;
		(*istr) >> fX;
		(*istr) >> fZ;

// [KH_070316] 변경 프리미엄 메모리 관련
		if(recvMSG == MSG_MEMPOS)
			_pUIMgr->GetTeleport()->SetTeleportInfo( ubSlot, slZone, fX, fZ, strComment );
		else if(recvMSG == MSG_MEMPOSPLUS)
			_pUIMgr->GetTeleportPrimium()->SetTeleportInfo( ubSlot, slZone, fX, fZ, strComment );
	}
}
void CSessionState::ReceiveGmMessage(CNetworkMessage *istr)
{
	UBYTE ubGmSubType;
	(*istr) >> ubGmSubType;
	if(ubGmSubType == MSG_GM_WHOAMI)
	{
		(*istr) >> _pNetwork->m_ubGMLevel;
		CPrintF("I'm a GM, level %d.", _pNetwork->m_ubGMLevel);
	}
}
void CSessionState::ReceiveInventoryMessage(CNetworkMessage *istr)
{
	SBYTE	bArrange;	//정렬시 1
	SBYTE	tabId;
	SBYTE	rowId;
	ULONG	uniIndex;
	ULONG	index;
	SBYTE	Wearing;
	ULONG	plus;
	ULONG	flag;
	ULONG	used;
	ULONG	used2;
	SQUAD	count;
	SBYTE	sbOptionCount;
	SBYTE	sbOptionType, sbOptionLevel;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	(*istr) >> bArrange;
	(*istr) >> tabId;
	(*istr) >> rowId;

	//CPrintF(TRANS("MSG_INVENTORY : (TAB)%d (ROW)%d \n"), tabId,rowId);				
	for( int i = 0;i < 5; i++ )
	{
		(*istr) >> uniIndex;

		if( uniIndex == -1 )
		{
			_pNetwork->MySlotItem[tabId][rowId][i].Init();
			continue;
		}

		(*istr) >> index;
		(*istr) >> Wearing;
		(*istr) >> plus;
		(*istr) >> flag; 
		(*istr) >> used; 

		(*istr) >> used2;

		(*istr) >> count;
		(*istr) >> sbOptionCount;

		//CPrintF(TRANS("MSG_INVENTORY(%d): (uniIndex)%d (index)%d (wear)%d (plus)%d (flag)%d (count)%ld \n"), i+1,uniIndex,index,Wearing,plus,flag,count);
		// eons 2006.11.1
		CItemData& ItemData	= _pNetwork->GetItemData(index);

		_pNetwork->MySlotItem[tabId][rowId][i].Item_Wearing = Wearing;
		_pNetwork->MySlotItem[tabId][rowId][i].ItemData		= ItemData;
		_pNetwork->MySlotItem[tabId][rowId][i].SetData( index, uniIndex, tabId, rowId, i,
														plus, flag, used, used2, Wearing, count );

		// 펫을 장착한 경우....
		if( ItemData.GetType() == CItemData::ITEM_ACCESSORY && 
			ItemData.GetSubType() == CItemData::ACCESSORY_PET && 
			Wearing != -1 )
		{
			const INDEX iPetIndex = _pNetwork->MySlotItem[tabId][rowId][i].Item_Plus;
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= iPetIndex;
			_pNetwork->_PetTargetInfo.lIndex		= iPetIndex;
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
				_pNetwork->_PetTargetInfo.strNameCard	= (*iter).strNameCard;

				INDEX iPetType;
				INDEX iPetAge;
				_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );

				_pNetwork->_PetTargetInfo.iType			= iPetType;
				_pNetwork->_PetTargetInfo.iAge			= iPetAge;
			}
		}		

		_pNetwork->MySlotItem[tabId][rowId][i].InitOptionData();

		//레어 아이템일때...
		if( ItemData.GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if( sbOptionCount ==0)
				_pNetwork->MySlotItem[tabId][rowId][i].SetRareIndex(0);
			//감정된 레어아이템이면...
			else
			{
				_pUIMgr->SetRareOption(istr, _pNetwork->MySlotItem[tabId][rowId][i]);
			}
		}
		//레어 아이템이 아니면.....
		else
		{
			for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
			{
				(*istr) >> sbOptionType;
				(*istr) >> sbOptionLevel;

				_pNetwork->MySlotItem[tabId][rowId][i].SetOptionData( sbOption, sbOptionType, sbOptionLevel );
			}
		}

		_pUIMgr->GetInventory()->InitInventory( tabId, rowId, i, uniIndex, index, Wearing );

		//INDEX PlayerType = _pNetwork->ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->en_pcCharacter.pc_iPlayerType;//타이탄:0,나이트:1,healer:2 메이지:3	
		if(Wearing != -1 && _pNetwork->MyCharacterInfo.sbEvocationType == -1)
		{
//			if(bArrange!=1)
//			{
				//_pNetwork->DeleteMyCurrentWearing(Wearing);
#ifdef HEAD_CHANGE
				if(!(ItemData.GetWearingPosition() == WEAR_HELMET && (CTString)ItemData.GetItemSmcFileName() == MODEL_TREASURE))
				{
					penPlayerEntity->DeleteCurrentArmor(Wearing);//1005 아이템 깨지는 버그수정
					penPlayerEntity->DeleteDefaultArmor(Wearing);
				}
#else
				penPlayerEntity->DeleteCurrentArmor(Wearing);//1005 아이템 깨지는 버그수정
				penPlayerEntity->DeleteDefaultArmor(Wearing);
#endif

				CModelInstance *pMI			= NULL;
				if( _pNetwork->MyCharacterInfo.bPetRide )
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
				else
				{
					pMI	= penPlayerEntity->GetModelInstance();
				}

				//penPlayerEntity->WearingArmor(tabId, rowId, i);
#ifdef HEAD_CHANGE
				if(!(ItemData.GetWearingPosition() == WEAR_HELMET && (CTString)ItemData.GetItemSmcFileName() == MODEL_TREASURE))
				{
					_pGameState->WearingArmor( pMI, ItemData );
				}
#else
				_pGameState->WearingArmor( pMI, ItemData );
#endif

				_pNetwork->SetMyCurrentWearing(tabId, rowId, i);

				// Date : 2005-04-07(오후 3:39:57), By Lee Ki-hwan
				_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
					, &ItemData
					, Wearing
					, plus
					, &pMI->m_tmSkaTagManager
					, 1, ItemData.GetSubType() );

//			}
//			else if(bArrange==1)//0611 kwon 정렬시에 장착슬롯 배열에 데이타 넣기.
//			{
//				penPlayerEntity->DeleteCurrentArmor(Wearing);//1231 아이템 정렬후 아이템 벗을때 안지워지던 버그수정
//				_pNetwork->pMyCurrentWearing[Wearing] = NULL;
//				penPlayerEntity->DeleteDefaultArmor(Wearing);
//				penPlayerEntity->WearingArmor(tabId, rowId, i);
//			}
		}
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&penPlayerEntity->GetModelInstance()->m_tmSkaTagManager,
														1);
	}


	if(bArrange & ITEM_ARRANGE_END)
	{
		for(int row=rowId+1;row<TOTAL_ROW;++row)
		{
			for(int col=0;col<TOTAL_COL;++col)
			{
				_pNetwork->MySlotItem[tabId][row][col].Init();
			}
		}
	}
/*
ITEM_ARRANGE_START			(1 << 0)	// 아이템 정렬의 시작.
#define		ITEM_ARRANGE_END
*/

}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
// 서버간에 이동이 있을때 쓰는 게 GO_ZONE
// 이동이 없을때 쓰는 게 GO_TO
// FIXME : 아래 함수 정리가 필요함.
void CSessionState::ReceiveGoZoneMessage(CNetworkMessage *istr)
{
	ULONG	zone;
	ULONG	extra;
	CTString	ip;				
	ULONG	port;
 
	(*istr) >> zone;			
	(*istr) >> extra;			
	(*istr) >> ip;			
	(*istr) >> port;

	// EDIT : BS
	MoveOtherServer(zone, ip, port);
	// --- EDIT : BS

	CPrintF(TRANS("MSG_GO_ZONE zone:%d extra:%d ip:%s port:%d \n"),zone,extra,ip,port);	

}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveItemMessage(CNetworkMessage *istr)
{
	SBYTE	type;
	SBYTE	wear_type;								// 입는 위치 
	SBYTE	wear_tab, wear_row, wear_col;			// 입는 아이템 위치.			 
	SBYTE	takeoff_tab, takeoff_row, takeoff_col;	// 벗는 아이템 위치.
	SLONG	wear_index, takeoff_index;				// 인덱스
	ULONG	item_uniindex;
	ULONG	item_index;
	ULONG	plus, flag;
	LONG	used, used2;
	SQUAD	count;
	FLOAT	x, h, z, r;
	SBYTE	ylayer;
	SBYTE	cha_type;
	ULONG	cha_index;
	int		ipl;
	SBYTE	tab,row,col;				
	SBYTE	row2,col2;	
	SQUAD	delta;
	
	//wooss 050818
	int commandCode = -1;

	//wooss 050818
	CTString strTitle;
	CTString strMessage1,strMessage2;
	CUIMsgBox_Info	MsgBoxInfo;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	(*istr) >> type;

	switch(type)
	{
	case MSG_ITEM_DISAPPEAR: //0607 kwon
		{
			(*istr) >> item_uniindex;
			for( ipl = 0; ipl < _pNetwork->ga_srvServer.srv_aitItem.Count(); ipl++ ) 
			{
				CItemTarget	&it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
				if (it.item_Index == item_uniindex)
				{
					it.Init();
//안태훈 수정 시작	//(5th Closed beta)(0.2)
					_pNetwork->ga_srvServer.srv_aitItem.SwapAndPop(ipl);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
				}						 												
			}
		}
		break;

	case MSG_ITEM_APPEAR: //0607 kwon
		{
			(*istr) >> item_uniindex;
			(*istr) >> item_index;
			(*istr) >> count;
			(*istr) >> x;
			(*istr) >> z;
			(*istr) >> h;
			(*istr) >> r;
			(*istr) >> ylayer;

			CItemData &ItemData	= _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName(item_index);
			CItemTarget &it = _pNetwork->ga_srvServer.srv_aitItem.Push();

			CEntity* penEntity;
			CPlacement3D plPlacement;
			plPlacement.pl_PositionVector(1) = x;
			plPlacement.pl_PositionVector(2) = h + 1.0f;
			plPlacement.pl_PositionVector(3) = z;	
			plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
			
			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM, -1, TRUE);
			it.SetData( item_uniindex, szItemName, penEntity, ylayer, count,FLOAT3D(x,h,z));	//0은 dummy
			if(penEntity)
			{
				it.item_iClientIndex	= penEntity->en_ulID;
				penEntity->en_strItemName = szItemName;
				SetDropItemModel(penEntity, ItemData, it);
			}
		}
		break;

	case MSG_ITEM_DROP:
		{
			(*istr) >> item_uniindex;
			(*istr) >> item_index;
			(*istr) >> count;
			(*istr) >> x;
			(*istr) >> z;
			(*istr) >> h;
			(*istr) >> r;
			(*istr) >> ylayer;
			(*istr) >> cha_type;
			(*istr) >> cha_index;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
			CItemData &ItemData	= _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName(item_index);
			CItemTarget &it = _pNetwork->ga_srvServer.srv_aitItem.Push();
			it.SetData( item_uniindex, szItemName, NULL, ylayer,count,FLOAT3D(x,h,z) );//0826
			if(cha_type == MSG_CHAR_NPC)
			{
				BYTE bLive = 0;

				(*istr) >> bLive; // NPC가 살아 있는가? 1(LIVE), 0(DEATH)

				// edit by cpp2angel (044.12.20) : 자동 도움말
				if ( cha_index == _UIAutoHelp->GetKillMonIndex () )
				{
					_UIAutoHelp->SetInfo ( AU_ITEM_DROP );
				}
				// ... End Edit 

				INDEX cntNPC = _pNetwork->ga_srvServer.srv_amtMob.Count();
				INDEX i = 0;

				if (bLive < ISNPCLIVE)
				{
					for(i=0; i<cntNPC; ++i)
					{
						if(_pNetwork->ga_srvServer.srv_amtMob[i].mob_Index == cha_index)
						{
							CItemTarget it;
							it.SetData(item_uniindex, szItemName, NULL, ylayer,count,FLOAT3D(x,h,z) );
							((CPlayerEntity*)CEntity::GetPlayerEntity(0))->AddDeathItem(
								_pNetwork->ga_srvServer.srv_amtMob[i].mob_pEntity
								, FLOAT3D(x,h+1,z), &it, &ItemData);
							break;
						}
					}
				}

				// FIXME : 아이템 표시하는 부분 중에서 중복되는 부분이 너무 많음.
				if(i == cntNPC || bLive == ISNPCLIVE)
				{
					char *szEffectName = NULL;
					CEntity* penEntity;
					CPlacement3D plPlacement;
					plPlacement.pl_PositionVector(1) = x;
					plPlacement.pl_PositionVector(2) = h + 1.0f;
					plPlacement.pl_PositionVector(3) = z;	
					plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
					penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
					if(penEntity)
					{
						it.item_iClientIndex	= penEntity->en_ulID;
						it.item_pEntity			= penEntity;
						penEntity->en_strItemName = szItemName;
						SetDropItemModel(penEntity, ItemData, it);
					}
				}
			}
			else	//MSG_CHA_PC
			{
				if( cha_index == _pNetwork->MyCharacterInfo.index )
				{

					CTString	strSysMessage;

					if( ItemData.GetType() == CItemData::ITEM_ETC &&
						ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )	// 0829 드롭사운드
					{
						penPlayerEntity->PlayItemSound( FALSE, TRUE );
						
						CTString strCount;
						strCount.PrintF( "%I64d", count );
						_pUIMgr->InsertCommaToString( strCount );

						strSysMessage.PrintF( _S( 301, "%s 나스를 버렸습니다." ), strCount );	// 번역 수정
						_pNetwork->ClientSystemMessage( strSysMessage );
					}
					else
					{
						penPlayerEntity->PlayItemSound( FALSE, FALSE );

						strSysMessage.PrintF( _S( 302, "%s %d개를 버렸습니다." ), szItemName, count );
						_pNetwork->ClientSystemMessage( strSysMessage );		
					}
				}
				CEntity* penEntity;
				CPlacement3D plPlacement;
				plPlacement.pl_PositionVector(1) = x;
				plPlacement.pl_PositionVector(2) = h + 1.0f;
				plPlacement.pl_PositionVector(3) = z;	
				plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
				penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
				if(penEntity)
				{
					it.item_iClientIndex	= penEntity->en_ulID;
					it.item_pEntity			= penEntity;
					penEntity->en_strItemName = szItemName;

					SetDropItemModel(penEntity, ItemData, it);
				}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
			}
		}
		break;

	case MSG_ITEM_UPDATE:
		{

			SBYTE	sbOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;

			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> item_uniindex;
			(*istr) >> plus;
			(*istr) >> flag;				
			(*istr) >> used;
			(*istr) >> used2;
			(*istr) >> count;
			(*istr) >> delta;
			(*istr) >> sbOptionCount;

			CItems& Items = _pNetwork->MySlotItem[tab][row][col];
			Items.Item_Plus = plus;
			Items.Item_Flag = flag;
			Items.Item_Used = used;
			Items.Item_Used2 = used2;
			Items.Item_Sum  = count;

			_pNetwork->MySlotItem[tab][row][col].InitOptionData();

			if( Items.ItemData.GetFlag() & ITEM_FLAG_RARE )
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					_pNetwork->MySlotItem[tab][row][col].SetRareIndex(0);
				//감정된 레어아이템이면...
				else
					_pUIMgr->SetRareOption(istr, _pNetwork->MySlotItem[tab][row][col]);
			}
			else
			{
				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> sbOptionLevel;

					_pNetwork->MySlotItem[tab][row][col].SetOptionData( sbOption, sbOptionType, sbOptionLevel );
				}
			}

			_pUIMgr->GetInventory()->InitInventory( tab, row, col, item_uniindex, Items.Item_Index,
													Items.Item_Wearing );

			if( delta > 0 )
			{
				CTString	strSysMessage;
				CItemData	&ItemData	= Items.ItemData;
				const char* szItemName	= _pNetwork->GetItemName(Items.Item_Index);

				if( ItemData.GetType() == CItemData::ITEM_ETC &&
					ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					CTString strCount;
					strCount.PrintF( "%I64d", delta );
					_pUIMgr->InsertCommaToString( strCount );

					strSysMessage.PrintF( _S( 416, "%s 나스를 얻었습니다." ), strCount ); // 번역 수정
					_pNetwork->ClientSystemMessage( strSysMessage );
				}
				else
				{						
					strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, delta );
					_pNetwork->ClientSystemMessage( strSysMessage );
				}
			}

			// edit by cpp2angel (044.12.20) : 
			if ( _pUIMgr->GetProcess()->IsVisible () )
			{
				_pUIMgr->GetProcess()->SelectItem ();
			}

			if ( _pUIMgr->GetProduct()->IsVisible () )
			{
				_pUIMgr->GetProduct()->SelectItem ();
			}

			// Date : 2005-04-07(오후 3:40:01), By Lee Ki-hwan
			_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
				, &Items.ItemData
				, Items.Item_Wearing
				, Items.Item_Plus
				, &penPlayerEntity->GetModelInstance()->m_tmSkaTagManager
				, 1, Items.ItemData.GetSubType() );
		}
		break;
		
	case MSG_ITEM_ADD:
		{
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;

			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> item_uniindex;					
			(*istr) >> item_index;
			(*istr) >> wear_type;
			(*istr) >> plus;
			(*istr) >> flag;
			(*istr) >> used;
			(*istr) >> used2;
			(*istr) >> count;
			(*istr) >> sbOptionCount;

			// 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 949 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 872 ))
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4001, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}

			// 판의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 948 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 871 ) )
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4000, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}
			
			// wooss 060810
			// 핑크 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1706 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 1711 ) )
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4004, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}
			// 불가사의한 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1709 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 1712 ) )
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4006, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}
			// 파란 판의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1707 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 1710 ) )
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4003, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}
			// 불가사의한의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1708 )
			{
				if( !_pNetwork->HasItem( item_index ) && !_pNetwork->HasItem( 1713 ) )
				{
					_pUIMgr->GetNotice()->AddToNoticeList(4005, NOTICE_EVENT);
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
			}

			CItemData& ItemData = _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName( item_index );

			_pNetwork->MySlotItem[tab][row][col].ItemData = ItemData;
			_pNetwork->MySlotItem[tab][row][col].SetData( item_index, item_uniindex, tab, row, col,
															plus, flag, used, used2, wear_type, count );

			_pNetwork->MySlotItem[tab][row][col].InitOptionData();

			if( ItemData.GetFlag() & ITEM_FLAG_RARE)
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					_pNetwork->MySlotItem[tab][row][col].SetRareIndex(0);
				//감정된 레어아이템이면...
				else
					_pUIMgr->SetRareOption(istr, _pNetwork->MySlotItem[tab][row][col]);
			}
			else
			{
				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> sbOptionLevel;

					_pNetwork->MySlotItem[tab][row][col].SetOptionData( sbOption, sbOptionType, sbOptionLevel );
				}
			}

			if(wear_type != -1)
			{								
				penPlayerEntity->DeleteDefaultArmor(wear_type);
				//penPlayerEntity->WearingArmor(tab, row, col);
				CModelInstance *pMI			= NULL;
				if( _pNetwork->MyCharacterInfo.bPetRide )
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
				else
				{
					pMI	= penPlayerEntity->GetModelInstance();
				}			
				_pGameState->WearingArmor( pMI, ItemData );				
				_pNetwork->SetMyCurrentWearing(tab, row, col);
			}

			_pUIMgr->GetInventory()->InitInventory( tab, row, col, item_uniindex, item_index, wear_type );	
			
			CTString	strSysMessage;

			if( ItemData.GetType() == CItemData::ITEM_ETC &&
				ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
			{
				CTString strCount;
				strCount.PrintF( "%I64d", count );
				_pUIMgr->InsertCommaToString( strCount );
				strSysMessage.PrintF( _S( 416, "%s 나스를 얻었습니다." ), strCount ); // 번역 수정
			}
			else
			{
				if( count > 0 )
					strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, count );
				else
					strSysMessage.PrintF( _S2( 303, szItemName, "%s<를> 얻었습니다." ),
											szItemName );
			}
			_pNetwork->ClientSystemMessage( strSysMessage );
			
			// edit by cpp2angel (044.12.20) : 
			if ( _pUIMgr->GetProcess()->IsVisible () )
			{
				_pUIMgr->GetProcess()->SelectItem ();
			}

			if ( _pUIMgr->GetProduct()->IsVisible () )
			{
				_pUIMgr->GetProduct()->SelectItem ();
			}

			if(_pUIMgr->GetChangeWeapon()->GetCashItem()){ //wooss 051011
				CUIMsgBox_Info	MsgBoxInfo;
				if( ItemData.GetType() == CItemData::ITEM_WEAPON)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(1049,"무기 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(2229, "무기가 성공적으로 교체되었습니다.") );
				}
				else if(ItemData.GetType() == CItemData::ITEM_SHIELD)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(3536, "방어구 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3537, "방어구가 성공적으로 교체되었습니다.") );
				}
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				_pUIMgr->GetChangeWeapon()->CloseChangeWeapon();												
			}
	
		}						
		break;
		
	case MSG_ITEM_SWAP:
		{
			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> row2;
			(*istr) >> col2;

			SwapItem( tab, row, col, tab, row2, col2 );
			_pUIMgr->GetInventory()->SwapSlotItems( tab, row, col, tab, row2, col2 );
		}
		break;

	case MSG_ITEM_USE :
		{
			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> item_index;

			LONG lItemIndex = _pUIMgr->GetInventory()->GetItemIndex( tab, row, col );

			CItemData	&rItemData = _pNetwork->GetItemData( lItemIndex );
			
			//071009_ttos: 창고 이용 주문서, 잡화상 이용 주문서
			if(item_index == 2455 || item_index == 2456 || item_index == 2457 || item_index == 2607)	//창고 이용 주문서
			{
				_pNetwork->MyCharacterInfo.bOtherZone = FALSE;
				_pUIMgr->GetWareHouse()->CheckHasPassWord(149, FALSE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
				return;
			}
			
			if(item_index == 2458 || item_index == 2459 || item_index == 2460 || item_index == 2608)	//잡화상 이용 주문서
			{
				_pUIMgr->GetShop()->SetFieldShopChack(TRUE);
				_pUIMgr->GetShop()->OpenShop(130, FALSE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
				return;
			}
			
			if(item_index == 2565 || item_index == 2566 || item_index == 2567)
			{
				_pUIMgr->GetPersonalShop()->SetCashPersonShop(TRUE);
				_pUIMgr->GetPersonalShop()->OpenPersonalShop( TRUE );
				return;
			}

			if (item_index == 2867) // 애완동물 조련사 NPC 이용 카드
			{
				_pUIMgr->GetPetTraining()->OpenPetTraining(rItemData.GetNum0(), FALSE, _pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
				return;
			}

			if (item_index == 2868) // 스킬 마스터 NPC 이용카드
			{
				_pUIMgr->GetSkillLearn()->PriorityOpen(rItemData.GetNum0(), TRUE);
				return;
			}

			// 포션이 아니라면 CoolTime 처리를 하지 않음.
			if( rItemData.GetType() != CItemData::ITEM_POTION 
				&& (rItemData.GetSubType() != CItemData::ITEM_SUB_TARGET) 
				//광속 아이템의 경우 사용한 시간을 저장해서 버튼의 쿨타임을 그리는데 사용하기 위해 리턴하지 않음...
				&&(item_index!=2407 && item_index!=2408 && item_index!=2609) )
				return;
				
			if( !_pUIMgr->GetQuickSlot()->StartSkillDelay( rItemData.GetNum0() ) )
			{
				if( !_pUIMgr->GetInventory()->StartSkillDelay( rItemData.GetNum0() ) )
					return;
			}
			
			_pUIMgr->GetInventory()->StartSkillDelay( rItemData.GetNum0() ); 		
			

			rItemData.StartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		}
		break;

	case MSG_ITEM_TAKE:
		{
			penPlayerEntity->Read_net_Character(MSG_ITEM_TAKE, &(*istr));	
/*
			(*istr) >> cha_index;
			(*istr) >> item_uniindex;		

			for( ipl = 0; ipl < _pNetwork->ga_srvServer.srv_aitItem.Count(); ipl++ ) 
			{
				CItemTarget	&it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
				if( it.item_Index == item_uniindex )
				{
					it.Init();
//안태훈 수정 시작	//(5th Closed beta)(0.2)
					_pNetwork->ga_srvServer.srv_aitItem.SwapAndPop(ipl);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
				}						 												
			}
*/
		}
		break;
	case MSG_ITEM_GET:
		{
			penPlayerEntity->Read_net_Character(MSG_ITEM_GET, &(*istr));	
		}
		break;

	case MSG_ITEM_DELETE:
		{
			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;

			CItemData& rItemData = _pNetwork->MySlotItem[tab][row][col].ItemData;			
			
			// 펫인 경우 리스트에서 제거합니다.
			if( rItemData.GetType() == CItemData::ITEM_ACCESSORY && 
				(rItemData.GetSubType() == CItemData::ACCESSORY_PET || rItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
			{
				const INDEX iPetIndex = _pNetwork->MySlotItem[tab][row][col].Item_Plus;
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= iPetIndex;
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					_pNetwork->m_vectorPetList.erase( iter );
				}
			}

			_pNetwork->MySlotItem[tab][row][col].Init();
			_pUIMgr->GetInventory()->InitInventory( tab, row, col, -1, -1, -1 );
		}
		break;

	case MSG_ITEM_WEAR:
		{
			CTString strMessage1;		
			CTString strMessage2;	

			(*istr) >> wear_type;
			(*istr) >> wear_tab;
			(*istr) >> wear_row;
			(*istr) >> wear_col;
			(*istr) >> wear_index;
			(*istr) >> takeoff_tab;
			(*istr) >> takeoff_row;
			(*istr) >> takeoff_col;
			(*istr) >> takeoff_index;

			CItemData& WearItemData	= _pNetwork->MySlotItem[wear_tab][wear_row][wear_col].ItemData;			
			CItemData& OffItemData	= _pNetwork->MySlotItem[takeoff_tab][takeoff_row][takeoff_col].ItemData;

			//CModelInstance *pMI			= penPlayerEntity->GetModelInstance();
			CModelInstance *pMI			= NULL;
			if( _pNetwork->MyCharacterInfo.bPetRide )
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
			else
			{
				pMI	= penPlayerEntity->GetModelInstance();
			}			
			const int iJob = _pNetwork->MyCharacterInfo.job;

			ASSERT( pMI != NULL && "Invalid Model Instance!!!" );

			// 아이템을 착용한다.
			if(takeoff_index == -1)
			{
				_pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Wearing = wear_type;

				// 펫을 장착한 경우....
				if( WearItemData.GetType() == CItemData::ITEM_ACCESSORY && 
					(WearItemData.GetSubType() == CItemData::ACCESSORY_PET || WearItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
				{
					const INDEX iPetIndex = _pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Plus;
					if( WearItemData.GetSubType() != CItemData::ACCESSORY_WILDPET)
					{
						_pNetwork->_PetTargetInfo.lIndex		= iPetIndex;					
						_pNetwork->_PetTargetInfo.bIsActive		= TRUE;
						_pNetwork->UpdatePetTargetInfo( iPetIndex );

					}
					
				}				

#ifdef HEAD_CHANGE
				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 && (CTString)WearItemData.GetItemSmcFileName() != MODEL_TREASURE)
#else
				if(_pNetwork->MyCharacterInfo.sbEvocationType == -1)
#endif
				{	
					penPlayerEntity->DeleteDefaultArmor(wear_type);
					_pGameState->WearingArmor( pMI, WearItemData );

				}
				_pNetwork->SetMyCurrentWearing(wear_tab, wear_row, wear_col);				

				_pUIMgr->GetInventory()->InitInventory( wear_tab, wear_row, wear_col, wear_index, WearItemData.GetItemIndex(), wear_type );

				//const char	*szItemName = WearItemData.GetName();
				const char* szItemName = _pNetwork->GetItemName(WearItemData.GetItemIndex());
				strMessage1.PrintF( _S2( 387, szItemName, "%s<를> 착용했습니다." ), szItemName );
				_pUIMgr->GetChatting()->AddSysMessage( strMessage1 );

				_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
					, &WearItemData
					, wear_type
					, _pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Plus
					, &pMI->m_tmSkaTagManager
					, 1, WearItemData.GetSubType() );
			}
			else if(wear_index == -1)//갑옷을 벗는거다.
			{
				// 펫을 장착한 경우....
				if( OffItemData.GetType() == CItemData::ITEM_ACCESSORY && 
					OffItemData.GetSubType() == CItemData::ACCESSORY_PET )
				{					
					// 펫을 타고 있으면, 펫에서 내리도록 처리.
					_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
					_pNetwork->_PetTargetInfo.Init();
					_pUIMgr->GetPetInfo()->ClosePetInfo();
				}

				if(OffItemData.GetType() == CItemData::ITEM_ACCESSORY && OffItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->_WildPetInfo.Init();
					_pUIMgr->GetWildPetInfo()->CloseWildPetInfo();					
				}

				// 강신중이고, 무기를 벗으려고 한다면...
				if( _pNetwork->MyCharacterInfo.sbEvocationType != -1 && 
					OffItemData.GetType() == CItemData::ITEM_WEAPON )
				{
					_pNetwork->SendStopEvocation();
				}
				
				// 생산중에 장비를 벗으면 생산을 중단 한다.
				if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				}
				
				_pNetwork->MySlotItem[takeoff_tab][takeoff_row][takeoff_col].Item_Wearing = -1;
				
#ifdef HEAD_CHANGE
				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1
					&& !(OffItemData.GetWearingPosition() == WEAR_HELMET
					&& (CTString)OffItemData.GetItemSmcFileName() == MODEL_TREASURE))
				{
					penPlayerEntity->DeleteCurrentArmor(wear_type);
					penPlayerEntity->WearingDefaultArmor(wear_type);
				}

				_pNetwork->DeleteMyCurrentWearing(wear_type);
#else
				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 )
				{
					penPlayerEntity->DeleteCurrentArmor(wear_type);
				}

				_pNetwork->DeleteMyCurrentWearing(wear_type);

				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 )
					penPlayerEntity->WearingDefaultArmor(wear_type);
#endif
				_pUIMgr->GetInventory()->InitInventory( takeoff_tab, takeoff_row, takeoff_col, takeoff_index, OffItemData.GetItemIndex(), -1 );
				
				const char* szItemName = _pNetwork->GetItemName(OffItemData.GetItemIndex());
				strMessage1.PrintF( _S2( 388, szItemName, "%s<를> 벗었습니다." ), szItemName );
				_pUIMgr->GetChatting()->AddSysMessage( strMessage1 );

				_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
					, &OffItemData
					, wear_type
					, -1
					, &pMI->m_tmSkaTagManager
					, 1, OffItemData.GetSubType() );
			}
			else //입고 있던 갑옷을 벗고, 새로운 갑옷을 입는다.
			{
				_pNetwork->MySlotItem[takeoff_tab][takeoff_row][takeoff_col].Item_Wearing = -1;
				_pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Wearing = wear_type;

				// 펫을 장착한 경우....
				if( OffItemData.GetType() == CItemData::ITEM_ACCESSORY && 
					(OffItemData.GetSubType() == CItemData::ACCESSORY_PET || OffItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
				{					
					// 펫을 타고 있으면, 펫에서 내리도록 처리.
					if(OffItemData.GetSubType() == CItemData::ACCESSORY_PET)
					{
						_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
						_pNetwork->_PetTargetInfo.Init();
						_pUIMgr->GetPetInfo()->ClosePetInfo();
					}else if(OffItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
					{
						_pNetwork->_WildPetInfo.Init();
						_pUIMgr->GetWildPetInfo()->CloseWildPetInfo();						
					}
				}

				// 강신중이고, 무기를 벗으려고 한다면...
				if( _pNetwork->MyCharacterInfo.sbEvocationType != -1 && 
					OffItemData.GetType() == CItemData::ITEM_WEAPON )
				{
					_pNetwork->SendStopEvocation();
				}

				// 펫을 장착한 경우....
				if( WearItemData.GetType() == CItemData::ITEM_ACCESSORY && 
					(WearItemData.GetSubType() == CItemData::ACCESSORY_PET || WearItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
				{
					const INDEX iPetIndex = _pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Plus;
					if(WearItemData.GetSubType() == CItemData::ACCESSORY_PET)
					{
						_pNetwork->_PetTargetInfo.lIndex		= iPetIndex;
						_pNetwork->_PetTargetInfo.bIsActive		= TRUE;
						_pNetwork->UpdatePetTargetInfo( iPetIndex );

					}else if(WearItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
					{
						// 서버에서 Appear정보를 다시 받아 갱신한다.
					}
					
				}

				// 생산중에 장비를 교체하면 생산을 중단 한다.
				if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				}

		#ifdef HEAD_CHANGE
				if((CTString)WearItemData.GetItemSmcFileName() == MODEL_TREASURE && wear_type == WEAR_HELMET)
 				{
 					penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
 				}

				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 
					&& !((CTString)WearItemData.GetItemSmcFileName() == MODEL_TREASURE && (CTString)OffItemData.GetItemSmcFileName() == MODEL_TREASURE))
				{
					penPlayerEntity->DeleteCurrentArmor(wear_type);

					if((CTString)WearItemData.GetItemSmcFileName() == MODEL_TREASURE)
						penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
					else
						_pGameState->WearingArmor( pMI, WearItemData );
				}

				_pNetwork->DeleteMyCurrentWearing(wear_type);
		#else
				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 )
				{
					penPlayerEntity->DeleteCurrentArmor(wear_type);
				}

				_pNetwork->DeleteMyCurrentWearing(wear_type);

				if( _pNetwork->MyCharacterInfo.sbEvocationType == -1 )
					_pGameState->WearingArmor( pMI, WearItemData );

		#endif
				_pNetwork->SetMyCurrentWearing(wear_tab, wear_row, wear_col);

				_pUIMgr->GetInventory()->InitInventory( takeoff_tab, takeoff_row, takeoff_col, takeoff_index, OffItemData.GetItemIndex(), -1 );
				_pUIMgr->GetInventory()->InitInventory( wear_tab, wear_row, wear_col, wear_index, WearItemData.GetItemIndex(), wear_type );						

				const char	*szItemName1 = _pNetwork->GetItemName(OffItemData.GetItemIndex());
				const char	*szItemName2 = _pNetwork->GetItemName(WearItemData.GetItemIndex());
				strMessage1.PrintF( _S3( 389, szItemName1, szItemName2, "%s<를> 벗고, %s<를> 착용했습니다." ), szItemName1, szItemName2 );
				_pUIMgr->GetChatting()->AddSysMessage( strMessage1 );

				_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
					, &WearItemData
					, wear_type
					, _pNetwork->MySlotItem[wear_tab][wear_row][wear_col].Item_Plus
					, &pMI->m_tmSkaTagManager
					, 1, WearItemData.GetSubType() );
			}
		}
		break;

	case MSG_ITEM_UPGRADE_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			_pUIMgr->GetInventory()->UpgradeItem( sbResult );
		}
		break;

	case MSG_ITEM_REFINE_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			_pUIMgr->GetRefine()->RefineRep( sbResult );
		}
		break;

	case MSG_ITEM_PROCESS_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			
			
			_pUIMgr->GetProcess()->ProcessRep ( sbResult ); // 이기환 수정 ( 12.3 )
		}
		break;

	// 이기환 수정 ( 12.6 ) 추가 
	case MSG_ITEM_MAKE_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			
			
			_pUIMgr->GetProduct()->ProductRep ( sbResult ); 
		}
		break;
	// 이기환 수정 끝 
	
	// 이기환 추가 (12.8)
	case MSG_ITEM_MIX_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			
			
			_pUIMgr->GetMix()->MixRep ( sbResult ); 
		}
		break;
	// 이기환 추가 끝
	case MSG_ITEM_ARCANE_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			
			
			_pUIMgr->GetCompound()->CompoundRep ( sbResult ); 
		}
		break;



	case MSG_ITEM_OPTION_ADD_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			_pUIMgr->GetInventory()->OptionAddRep( sbResult );
		}
		break;
	case MSG_ITEM_OPTION_DEL_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			_pUIMgr->GetInventory()->OptionDelRep( sbResult );
		}
		break;
	case MSG_ITEM_CHANGEWEAPON_REP:
		{
			// 무기 교환 응답		: old_itemdbindex(n) new_itemdbindex(n)
			LONG lOldItemIndex;
			LONG lNewItemIndex;
			(*istr) >> lOldItemIndex;
			(*istr) >> lNewItemIndex;

			// 돈이 모자른 경우.
			if( lOldItemIndex == -1 )
			{	
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1328, "나스가 부족하여 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
			}
			// 잘못된 교환을 요청한 경우.
			else if( lNewItemIndex == -1 )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1329, "무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
			}
			_pUIMgr->GetChangeWeapon()->CloseChangeWeapon();
		}
		break;
	case MSG_ITEM_PROCESS_NPC:
		{
			//processItemDBIndex(n) count(n) errcode(n:s)
			LONG processItemDBIndex;
			LONG count;
			LONG errcode;
			(*istr) >> processItemDBIndex;
			(*istr) >> count;
			(*istr) >> errcode;

			_pUIMgr->GetProcessNPC()->ProcessNPCRep(processItemDBIndex, count, errcode);
		} break;
	case MSG_ITEM_ADD_BOOSTER:
		{
			LONG flag;
			(*istr) >> flag;
			CTString strTitle;
			CTString strContent;

			int commandCode = -1;
			switch(flag)
			{
			case MSG_ITEM_ADD_BOOSTER_ERROR_OK:		// 성공
				{
					strTitle = _S( 1825, "업그레이드 성공" );
					strContent = _S( 1826, "부스터 업그레이드에 성공했습니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			case MSG_ITEM_ADD_BOOSTER_ERROR_USED:	// 실패
				{
					strTitle = _S( 1827, "업그레이드 불가" );
					strContent = _S( 1828, "내구도가 부족합니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			case MSG_ITEM_ADD_BOOSTER_ERROR_ALREADY:	// 카운트 오류
				{
					strTitle = _S( 1827, "업그레이드 불가" );
					strContent = _S( 1829, "이미 부스터 업그레이드가 되어 있는 아이템입니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			}
			
			_pUIMgr->CloseMessageBox(MSGCMD_INVEN_UPGRADE_REP);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_INVENTORY, commandCode );
			MsgBoxInfo.AddString( strContent );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
		} break;
	//wooss 050816
	//item prolong message
	case MSG_ITEM_USE_PROLONG :
		
		ULONG useTime;
		UBYTE prolong;
	
		(*istr) >> item_index;
		(*istr) >> useTime;
		(*istr) >> prolong;
		
		if(item_index==MEMSCROLL_EX_ITEM || item_index == MEMSCROLL_EX_ITEM_7DAYS)
		{
			_pUIMgr->GetTeleport()->SetUseTime(useTime);
			
			if(prolong&&useTime>0){
				//메모리 스크롤을 이미 사용하고 있을때
				//wooss 050816
				strTitle	=	_S(2088 , "사용 확인" );
				strMessage1	=_S( 2128, 	"이미 메모리 스크롤이 확장되어 있는 상태 입니다." ); 
				strMessage2 =   _S(2148 , "기간을 연장하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_PROLONG_MEMSCROLL_EX);
				MsgBoxInfo.AddString(strMessage1);
				MsgBoxInfo.AddString(strMessage2);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		else if(item_index==WAREHOUSE_EX_ITEM || item_index ==WAREHOUSE_EX_ITEM_7DAYS )
		{
			_pUIMgr->GetWareHouse()->SetUseTime(useTime);

			if(prolong&&useTime>0){
				//확장창고를 이미 사용하고 있을때
				//wooss 050816
				strTitle	=_S( 2088, 	"사용 확인" );
				strMessage1	=_S(2129 , 	"이미 개인 창고가 확장되어 있는 상태 입니다." );
				strMessage2 =   _S(2148 , "기간을 연장하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_PROLONG_WAREHOUSE_EX);
				MsgBoxInfo.AddString(strMessage1);
				MsgBoxInfo.AddString(strMessage2);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		if(item_index==EX_SLOT_ITEM)
		{
				strTitle	=	_S(191,"확인");   
				strMessage1	=	_S( 2149, "캐릭터 슬롯이 확장 되었습니다." );
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YES,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage1);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
		

	case MSG_ITEM_USE_ERROR :
	
		ULONG errType;
		(*istr) >> errType;
		switch(errType)
		{
			case MSG_ITEM_USE_ERROR_REVIVAL_EXP	:		// 부활 주문서 때문에 경험치 복구 주문서 사용 할수 없음
				//Client 자체적으로 처리함 buff의 상태를 체크하여 처리
				break;	
			case MSG_ITEM_USE_ERROR_REVIVAL_SP :			// 부활 주문서 때문에 숙련도 복구 주문서를 사용할수 없음
				//Client 자체적으로 처리함 buff의 상태를 체크하여 처리
				break;
			case MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE : // 소환 불가능한 상태나 지역입니다.
				strTitle	=	_S(191,"확인");
				strMessage1	=	_S( 2130, "소환 불가능한 지역 또는 상태입니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage1);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
//			case MSG_ITEM_USE_ERROR_EXP_SP : //
//				break;
//			case MSG_ITEM_USE_ERROR_SP_EXP :
//				break;
			case MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL :
				strTitle	=	_S(191,"확인");
				strMessage1	=	_S( 2728, "플래티늄 제련석 감정이 완료 되었습니다." ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage1);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
			case MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE :
				strTitle	=	_S(191,"확인");
				LONG tv_idx;
				(*istr) >> tv_idx;
				strMessage1.PrintF( _S( 2738, "[%s]플래티늄 아이템의 사용기간이 만료되어 효과가 사라집니다."),_pUIMgr->GetInventory()->GetItemName(tv_idx));
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage1);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				break;
			case MSG_ITEM_USE_ERROR_EXPUP: // 같은 스킬의 아이템 사용시 사용 확인
				{
					SBYTE sbTab, sbRow, sbCol;
					LONG lIndex;

					(*istr) >> sbTab >> sbRow >> sbCol;
					(*istr) >> lIndex;
					
					CItems& Items = _pNetwork->MySlotItem[sbTab][sbRow][sbCol];

					if( lIndex == Items.Item_UniIndex )
					{// 아이리스의 축복
						_pUIMgr->GetInventory()->SetUseItemSlotInfo( (int)sbTab, (int)sbRow, (int)sbCol );

						strTitle = _S( 2088, "사용 확인" );
						strMessage1.PrintF( _S( 2638, "현재 경험의묘약을 복용중입니다.만약 아이리스의 축복을 사용하면 그 시간동안 경험의 묘약 효과가 적용되지 않습니다" ) );
						
						MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_YESNO, UI_NONE, MSGCMD_COMFIRM_USE_ITEM );
						MsgBoxInfo.AddString( strMessage1 );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
				}
				break;
			case MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK:
				{
					strTitle	=	_S(191,"확인");
					strMessage1	=	_S(3163, "옵션이 성공적으로 감정되었습니다." ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage1);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_ITEM_USE_ERROR_IDENTIFY_RARE_FAIL:
				{
					strTitle	=	_S(191,"확인");
					strMessage1	=	_S(3164, "옵션 감정이 실패하였습니다." ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage1);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_ITEM_USE_ERROR_WARN_PARTYRECALL:
				{
					strTitle = _S(191, "확인");
					strMessage1 = _S(3069, "공성중에 성안이나 성인근에서는 같은 길드원만 리콜할 수 있습니다.");
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage1);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_ITEM_USE_ERROR_SUMMON_GUARD:
				{
					_pUIMgr->GetChatting()->AddSysMessage( _S( 3596, "소환수가 이미 소환되어 있어 소환수를 소환할 수 없습니다." ), SYSMSG_ERROR );
				}
				break;
		}
		break;
	case MSG_ITEM_LEND_WEAPON :
		{
			ULONG idx;
			ULONG errType;
			(*istr) >> idx; 
			(*istr) >> errType;
			LeaseWeaponErrChk(errType);
		}
		break;
		
	// 공성 아이템 조합 
	case MSG_ITEM_MIX_WARITEM:
		{
			LONG nErrorCode;
			(*istr) >> nErrorCode;	
			_pUIMgr->GetGWMix()->MixRep( nErrorCode );
		}
		break;

	case MSG_ITEM_LEVELDOWN:
		{
			LONG nErrorCode;
			(*istr) >> nErrorCode;
			
			_pUIMgr->CloseMessageBox(MSGCMD_ITEM_LEVELDOWN_REP);
			CUIMsgBox_Info MsgBoxInfo;

			switch (nErrorCode)
			{
			case MSG_ITEM_LEVELDOWN_ERROR_OK:			// 성공
				{
					strTitle = _S(1970, "성공");
					strMessage1 = _S(3982, "아이템의 레벨제한이 감소되었습니다.");
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_ALREADY: // 이미 레벨 하락
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3983, "이미 레벨제한 감소가 적용되어 있습니다." );
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP: // 장비 아님
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3984, "레벨제한 감소를 적용할 수 있는 장비 아이템이 아닙니다." );
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_SCROLL: // 주문서 아님
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3985, "아이템 레벨제한 감소 주문서가 아닙니다." );
				}
				break;
			}

			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( strMessage1 );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
		}
		break;
	}
}

void CSessionState::ReceiveAttackMessage(CNetworkMessage *istr)
{
	SBYTE	type;							
	(*istr) >> type;				
	
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	if(type == MSG_CHAR_PC)
	{
		penPlayerEntity->Read_net_Character(MSG_ATTACK, &(*istr));	
	}
	else if(type == MSG_CHAR_NPC)
	{
		penPlayerEntity->Read_net_Mob(MSG_ATTACK, &(*istr));						
	}
	else if(type == MSG_CHAR_PET)
	{
		penPlayerEntity->Read_net_Pet(MSG_ATTACK, &(*istr));						
	}
	else if(type == MSG_CHAR_ELEMENTAL)
	{
		penPlayerEntity->Read_net_Summon(MSG_ATTACK, &(*istr));
	}
}

void CSessionState::ReceiveDamageMessage(CNetworkMessage *istr)
{
	SBYTE	type;							
	(*istr) >> type;				
	
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	if(type == MSG_CHAR_PC)
	{
		penPlayerEntity->Read_net_Character(MSG_DAMAGE, &(*istr));		
	}
	else
	{
		// 플레이어를 제외한 다른 엔티티의 경우.
		penPlayerEntity->Read_net_Damage(type, &(*istr));		
	}	
	
	// 길드 전쟁중 포탈을 타려고 하는데 공격을 받으면 취소 
	_pUIMgr->GetGuildWarPortal()->PortalCancel();
}

void CSessionState::ReceiveExchangeMessage(CNetworkMessage *istr)
{
	BYTE	ExchType;
	(*istr) >> ExchType;

	if( ExchType == MSG_EXCHANGE_REQ )
	{
		BYTE		bExchReqType;
		(*istr) >> bExchReqType;

		switch( bExchReqType )
		{
			case MSG_EXCHANGE_REQ_REQ:
				{
					LONG		lSrcIndex;
					LONG		lDestIndex;
					CTString	strSrcName;
					CTString	strDestName;
					(*istr) >> lSrcIndex;
					(*istr) >> strSrcName;
					(*istr) >> lDestIndex;
					(*istr) >> strDestName;
					_pUIMgr->GetExchange()->ExchangeReq_Req( lSrcIndex, strSrcName, lDestIndex, strDestName );
				}
				break;
			case MSG_EXCHANGE_REQ_REP:
				_pUIMgr->GetExchange()->ExchangeReq_Rep();
				break;
			case MSG_EXCHANGE_REQ_REJECT_SRC:
				_pUIMgr->GetExchange()->ExchangeReq_Rej( TRUE );
				break;
			case MSG_EXCHANGE_REQ_REJECT_DEST:
				_pUIMgr->GetExchange()->ExchangeReq_Rej( FALSE );
				break;
			case MSG_EXCHANGE_REQ_READY_SRC:
				_pUIMgr->GetExchange()->ExchangeReq_Ready( TRUE );
				break;
			case MSG_EXCHANGE_REQ_READY_DEST:
				_pUIMgr->GetExchange()->ExchangeReq_Ready( FALSE );
				break;
			case MSG_EXCHANGE_REQ_READY:
				_pUIMgr->GetExchange()->ExchangeReq_BothReady();
				break;
			case MSG_EXCHANGE_REQ_FULL_SRC:
				_pUIMgr->GetExchange()->ExchangeReq_Full( TRUE );
				break;
			case MSG_EXCHANGE_REQ_FULL_DEST:
				_pUIMgr->GetExchange()->ExchangeReq_Full( FALSE );
				break;
			case MSG_EXCHANGE_REQ_OK_SRC:
				_pUIMgr->GetExchange()->ExchangeReq_Ok( TRUE );
				break;
			case MSG_EXCHANGE_REQ_OK_DEST:
				_pUIMgr->GetExchange()->ExchangeReq_Ok( FALSE );
				break;
			case MSG_EXCHANGE_REQ_OK:
				_pUIMgr->GetExchange()->ExchangeReq_BothOk();
				break;
			case MSG_EXCHANGE_REQ_SRC_PET_ANYMORE :
				_pUIMgr->GetExchange()->ExchangeReq_Pet_Err(FALSE);
				break;
			case MSG_EXCHANGE_REQ_DEST_PET_ANYMORE : 
				_pUIMgr->GetExchange()->ExchangeReq_Pet_Err(TRUE);
				break;

		}
	}
	else if( ExchType == MSG_EXCHANGE_ITEM )
	{
		BYTE	bExchItemType;
		SLONG	slUniIndex;
		SQUAD	llCount;
		SLONG	slIndex;
		SLONG	slPlus;
		SLONG	slFlag;
		SLONG	slUsed;
		SLONG	slUsed2;
		SBYTE	sbOptionCount;
		SBYTE	sbOptionType, sbOptionLevel;

		(*istr) >> bExchItemType;

		switch( bExchItemType )
		{
		case MSG_EXCHANGE_ITEM_ADD_SRC:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> slIndex;
				(*istr) >> slPlus;
				(*istr) >> slFlag;
				(*istr) >> slUsed;
				(*istr) >> slUsed2;
				(*istr) >> sbOptionCount;

				_pUIMgr->GetExchange()->ExchangeItem_Add( TRUE, slUniIndex, llCount, slIndex, slPlus, slFlag, slUsed, slUsed2 );

				//레어 아이템일때...
				if( _pNetwork->GetItemData(slIndex).GetFlag() & ITEM_FLAG_RARE )
				{ 
					if( sbOptionCount >0)
						_pUIMgr->GetExchange()->ExchangeItem_SetRareOption(istr, TRUE, slUniIndex);	
				}
				//레어 아이템이 아니면.....
				else
				{
					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> sbOptionLevel;

						_pUIMgr->GetExchange()->ExchangeItem_SetOptionData( TRUE, slUniIndex, sbOption,
																		sbOptionType, sbOptionLevel );
					}
				}
			}
			break;

		case MSG_EXCHANGE_ITEM_ADD_DEST:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> slIndex;
				(*istr) >> slPlus;
				(*istr) >> slFlag;
				(*istr) >> slUsed;
				(*istr) >> slUsed2;
				(*istr) >> sbOptionCount;

				_pUIMgr->GetExchange()->ExchangeItem_Add( FALSE, slUniIndex, llCount, slIndex, slPlus, slFlag, slUsed, slUsed2 );

				//레어 아이템일때...
				if( _pNetwork->GetItemData(slIndex).GetFlag() & ITEM_FLAG_RARE )
				{
					if( sbOptionCount >0)
						_pUIMgr->GetExchange()->ExchangeItem_SetRareOption( istr, FALSE, slUniIndex);
				}
				//레어 아이템이 아니면.....
				else
				{
					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> sbOptionLevel;

						_pUIMgr->GetExchange()->ExchangeItem_SetOptionData( FALSE, slUniIndex, sbOption,
																		sbOptionType, sbOptionLevel );
					}
				}
			}
			break;

		case MSG_EXCHANGE_ITEM_DEL_SRC:
			{
				(*istr) >> slUniIndex;
				_pUIMgr->GetExchange()->ExchangeItem_Del( TRUE, slUniIndex );
			}
			break;

		case MSG_EXCHANGE_ITEM_DEL_DEST:
			{
				(*istr) >> slUniIndex;
				_pUIMgr->GetExchange()->ExchangeItem_Del( FALSE, slUniIndex );
			}
			break;

		case MSG_EXCHANGE_ITEM_UPDATE_SRC:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				_pUIMgr->GetExchange()->ExchangeItem_Update( TRUE, slUniIndex, llCount );
			}
			break;

		case MSG_EXCHANGE_ITEM_UPDATE_DEST:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				_pUIMgr->GetExchange()->ExchangeItem_Update( FALSE, slUniIndex, llCount );
			}
			break;
		}
	}
}

void CSessionState::ReceivePartyMessage( CNetworkMessage *istr )
{
	SBYTE	sbPartyType;
	(*istr) >> sbPartyType;

	switch( sbPartyType )
	{
	case MSG_PARTY_MEMBER_INFO:
		{
			SLONG	slIndex, slZone;
			SLONG   swLevel, swHP, swMaxHP, swMP, swMaxMP;
			FLOAT	fX, fZ;
			SBYTE	sbLayer;

			(*istr) >> slIndex >> swLevel >> swHP >> swMaxHP >> swMP >> swMaxMP;
			(*istr) >> fX >> fZ >> sbLayer >> slZone;

			_pUIMgr->GetParty()->PartyMemberInfo( slIndex, swLevel, swHP, swMaxHP, swMP, swMaxMP, fX, fZ, sbLayer, slZone );
		}
		break;

	case MSG_PARTY_INVITE:
		{
			SBYTE		sbType;
			SLONG		slSrcIndex;
			CTString	strSrcName;

			(*istr) >> sbType;
			(*istr) >> slSrcIndex;
			(*istr) >> strSrcName;

			_pUIMgr->GetParty()->PartyInvite( sbType, slSrcIndex, strSrcName );
		}
		break;

	case MSG_PARTY_REJECT_SRC:
		_pUIMgr->GetParty()->PartyReject( TRUE );
		break;

	case MSG_PARTY_REJECT_DEST:
		_pUIMgr->GetParty()->PartyReject( FALSE );
		break;

	case MSG_PARTY_ADD:
		{
			SBYTE		sbLeader, sbJob, sbJob2, sbLayer;
			SLONG		slIndex, slZone;
			SLONG		swLevel, swHP, swMaxHP, swMP, swMaxMP;
			FLOAT		fX, fZ;
			CTString	strName;

			(*istr) >> sbLeader >> slIndex >> strName >> sbJob >> sbJob2 >> swLevel;
			(*istr) >> swHP >> swMaxHP >> swMP >> swMaxMP;
			(*istr) >> fX >> fZ >> sbLayer >> slZone;

			_pUIMgr->GetParty()->PartyAddMember( sbLeader, slIndex, strName, sbJob, sbJob2, swLevel,
													swHP, swMaxHP, swMP, swMaxMP, fX, fZ, sbLayer, slZone );
		}
		break;

	case MSG_PARTY_QUIT:
		{
			SLONG	slIndex;
			(*istr) >> slIndex;

			_pUIMgr->GetParty()->PartyQuit( slIndex );
		}
		break;

	case MSG_PARTY_KICK:
		{
			SLONG	slIndex;
			(*istr) >> slIndex;

			_pUIMgr->GetParty()->PartyKick( slIndex );
		}
		break;

	case MSG_PARTY_END:
		_pUIMgr->GetParty()->PartyEnd();
		break;

	case MSG_PARTY_ASSISTINFO:
		break;
	case MSG_PARTY_CHANGEBOSS: // 파티 장 위임 
		{	
			CTString	strBossName;
			CTString	strManadateChaName;
			SLONG		slManadateIndex;
			SBYTE		sbMandate;	// 0: 파티장 탈되, 1:위임
			
			(*istr) >> strBossName;
			(*istr) >> slManadateIndex;
			(*istr) >> strManadateChaName;
			(*istr) >> sbMandate;	
			
			_pUIMgr->GetParty()->MandateBoss( strBossName, slManadateIndex, strManadateChaName, sbMandate );

		}
		break;
	case MSG_PARTY_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			_pUIMgr->GetParty()->PartyError( sbError );
		}
		break;
	case MSG_PARTY_EMPTY: // 퍼스널 던전 입장시 파티 초기화 [1/11/2007 Theodoric]
		if( _pUIMgr->GetParty()->GetMemberCount() )
		{
			_pUIMgr->GetParty()->PartyEnd();
		}
		break;
	case MSG_PARTY_CHANGETYPE:
		{
			SBYTE	sbPartyType;
			(*istr) >> sbPartyType;

			_pUIMgr->GetParty()->SetPartyType(sbPartyType);			
			
			CTString strMessage, strPartyType;

			switch(sbPartyType)
			{
				case PT_PEACEEVER:
					strPartyType = _S(2660, "균등분배 파티");
					break;
				case PT_SURVIVAL:
					strPartyType = _S(2661, "입수우선 파티");
					break;
				case PT_ATTACK:
					strPartyType = _S(2662, "전투형 파티");
					break;
			}

			strMessage.PrintF(_S(4197, "파티 종류가 %s로 변경되었습니다."), strPartyType);

			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
	}
}

void CSessionState::ReceiveQuickSlotMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;//1013
	SBYTE	sbPage, sbSlotType;
	(*istr) >> ubType;
	(*istr) >> sbPage;

	switch( ubType )
	{
	case MSG_QUICKSLOT_LIST:
		{
			_pUIMgr->GetQuickSlot()->ClearAllBtns( sbPage );

			//for( int iSlot = 0; iSlot < 10; iSlot++ )
			for( int iSlot = 0; iSlot < QSLOT_BTN_COUNT; iSlot++ )
			{
				(*istr) >> sbSlotType;
				if( sbSlotType == -1 )
				{
					_pUIMgr->GetQuickSlot()->AddBtn( sbPage, iSlot );
				}
				else if( sbSlotType == 2 )
				{
					SBYTE	sbRow, sbCol;
					(*istr) >> sbRow;
					(*istr) >> sbCol;
					
					_pUIMgr->GetQuickSlot()->AddBtn( sbPage, iSlot, sbSlotType, sbRow, sbCol );
				}
				else
				{
					SLONG	slIndex; // Date : 2005-03-02,   By Lee Ki-hwan
					(*istr) >> slIndex;

					_pUIMgr->GetQuickSlot()->AddBtn( sbPage, iSlot, sbSlotType, slIndex );
				}
			}
		}
		break;

	case MSG_QUICKSLOT_ADD:
		{
			SBYTE	sbSlotNum;
			(*istr) >> sbSlotNum;
			(*istr) >> sbSlotType;

			if( sbSlotType == -1 )
			{
				_pUIMgr->GetQuickSlot()->AddBtn( sbPage, sbSlotNum );
			}
			else if( sbSlotType == 2 )
			{
				SBYTE	sbRow, sbCol;
				(*istr) >> sbRow;
				(*istr) >> sbCol;
				
				_pUIMgr->GetQuickSlot()->AddBtn( sbPage, sbSlotNum, sbSlotType, sbRow, sbCol );
			}
			else
			{
				SLONG	slIndex;		// Date : 2005-03-02,   By Lee Ki-hwan
				(*istr) >> slIndex;

				_pUIMgr->GetQuickSlot()->AddBtn( sbPage, sbSlotNum, sbSlotType, slIndex );
			}
		}
		break;

	case MSG_QUICKSLOT_SWAP:
		{
			SBYTE	sbSlotNum1, sbSlotNum2;
			(*istr) >> sbSlotNum1;
			(*istr) >> sbSlotNum2;

			_pUIMgr->GetQuickSlot()->SwapBtns( sbPage, sbSlotNum1, sbSlotNum2 );
		}
		break;
	}
}

void CSessionState::ReceiveSkillMessage( CNetworkMessage *istr )
{
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	UBYTE	ubType;//1013
	(*istr) >> ubType;

	switch( ubType )
	{
	case MSG_SKILL_LIST:
		{
			_pUIMgr->GetCharacterInfo()->ClearSkills();

			SLONG	slIndex;
			SBYTE	sbCount, sbLevel;//1013
			(*istr) >> sbCount;

			for( int iSkill = 0; iSkill < sbCount; iSkill++ )
			{
				(*istr) >> slIndex;
				(*istr) >> sbLevel;

				_pUIMgr->GetCharacterInfo()->AddSkill( slIndex, sbLevel );
			}
		}
		break;

	case MSG_SKILL_LEARN:
		{
			SLONG	slIndex;
			SBYTE	sbIsNew;
			SBYTE	sbLevel;

			(*istr) >> slIndex;
			(*istr) >> sbIsNew;
			(*istr) >> sbLevel;

			_pUIMgr->GetSkillLearn()->LearnSkill( slIndex, sbIsNew, sbLevel );
		}
		break;

	case MSG_SKILL_READY:
		{
			//스킬 시전.

			penPlayerEntity->Read_net_Character(MSG_SKILL_READY, istr);				
		}
		break;

	case MSG_SKILL_FIRE:
		{
			//스킬 발동.
			penPlayerEntity->Read_net_Character(MSG_SKILL_FIRE, istr);				
		}
		break;

	case MSG_SKILL_CANCEL:
		{
			//SLONG	slChaIndex;
			//(*istr) >> slChaIndex;
			penPlayerEntity->Read_net_Character(MSG_SKILL_CANCEL, istr);		//1013	
			//스킬취소.
		}
		break;

	case MSG_SKILL_LEARN_ERROR:
		{
			UBYTE	ubError;
			(*istr) >> ubError;

			_pUIMgr->GetSkillLearn()->LearnSkillError( ubError );
		}
		break;
	case MSG_SKILL_AUTO_USE:
		{
			LONG	nSkillIndex;
			(*istr) >> nSkillIndex;
			_pUIMgr->GetCharacterInfo()->UseSkill( nSkillIndex );
		}
		break;

	case MSG_SKILL_ERROR:
		{
			BYTE	bError;	
			LONG	nSkillIndex;	
			LONG	nExtraInfo;

			(*istr) >> bError;
			(*istr) >> nSkillIndex;			
			(*istr) >> nExtraInfo;
				
			switch( bError )
			{
				// FIXME : Read_net_Character()에 NULL을 넣는 이상한 루틴...너무 엉망인듯...
			case MSG_SKILL_ERROR_CANNOT_SPELL:
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1438, "스킬 사용이 불가능한 상태입니다." ), SYSMSG_ERROR );	
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_CANNOT_SPELL, NULL );
				break;
			case MSG_SKILL_ERROR_ALREADY_USE:	// 다른 스킬 사용중
				_pUIMgr->GetChatting()->AddSysMessage( _S( 315, "다른 스킬을 사용중입니다." ), SYSMSG_ERROR );
				//penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_ALREADY_USE, NULL );
				break;
			case MSG_SKILL_ERROR_NOTFOUND:		// 그런 스킬 없어요
				_pUIMgr->GetChatting()->AddSysMessage( _S( 316, "존재하지 않는 스킬입니다." ), SYSMSG_ERROR );
				//penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_NOTFOUND, NULL );
				break;
			case MSG_SKILL_ERROR_NOTREADY:		// 스킬 준비 중... 즉, 이전 스킬 시전후 다음 스킬 가능 시간이 안되었을때
				_pUIMgr->GetChatting()->AddSysMessage( _S( 317, "스킬을 준비중입니다." ), SYSMSG_ERROR );
				//penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_NOTREADY, NULL );
				// NOTE : 스킬 시간이 안됐다구 중지시키면서 스킬을 Cancel하면 안됨.
				break;				
			case MSG_SKILL_ERROR_INVAL_TARGET:	// 타겟이 잘못되었음
				_pUIMgr->GetChatting()->AddSysMessage( _S( 318, "타겟이 잘못되었습니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_TARGET, NULL );
				break;
			case MSG_SKILL_ERROR_INSUFF_MP:		// 조건이 안 맞아 사용 못함(MP)
				_pUIMgr->GetChatting()->AddSysMessage( _S( 320, "MP가 부족합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INSUFF_MP, NULL );
				break;
			case MSG_SKILL_ERROR_INSUFF_HP:		// 조건이 안 맞아 사용 못함(HP)
				_pUIMgr->GetChatting()->AddSysMessage( _S( 319, "HP가 부족합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INSUFF_HP, NULL );
				break;
			case MSG_SKILL_ERROR_INSUFF_ITEM:	// 조건이 안 맞아 사용 못함(Item)
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1330, "아이템이 부족합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INSUFF_ITEM, NULL );
				break;
			case MSG_SKILL_ERROR_LONG:	// 멀어서 안됨
				_pUIMgr->GetChatting()->AddSysMessage( _S( 322, "거리가 멀어서 사용할 수 없습니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_LONG, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_DEATH:		// 상태 불만족 : 죽어서만 가능
				_pUIMgr->GetChatting()->AddSysMessage( _S( 390, "죽었을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_DEATH, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_WEAPON:		// 상태 불만족 : 무기 장비시만 가능
				_pUIMgr->GetChatting()->AddSysMessage( _S( 391, "무기를 장착해야 사용가능 합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_WEAPON, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_SITDOWN:	// 상태 불만족 : 앉아서만 가능
				_pUIMgr->GetChatting()->AddSysMessage( _S( 392, "앉아있을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_SITDOWN, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE:	// 상태 불만족 : 피스존 불가능
				_pUIMgr->GetChatting()->AddSysMessage( _S( 393, "평화존에서는 사용 불가능합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_STAND:		// 상태 불만족 : 서서만 가능
				_pUIMgr->GetChatting()->AddSysMessage( _S( 394, "서있을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_STAND, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_SHIELD:	// 상태 불만족 : 방패 미착용
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1331,  "방패를 착용해야만 사용합니다." ), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_SHIELD, NULL );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_MAGIC:		// 상태 불만족 : 필요 버프 불충분
				{
					CSkill &skill = _pNetwork->GetSkillData( nSkillIndex );
					CTString strMessage;
					switch( nSkillIndex )
					{
					case 236: // 하이드 스템 
					case 237: // 하이드 샷 
						strMessage.PrintF(_S( 2063,  "%s을 사용하기 위해서는 [인비저빌리티]스킬이 필요합니다." ), skill.GetName() ); 
						break;
					default :
						strMessage = _S( 1332, "특정 버프 스킬이 필요합니다." );
						break;
					}
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
					//penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_INVAL_STATE_MAGIC, NULL );
				}
				break;
			case MSG_SKILL_ERROR_STATPOINT_STR:			// 힘 부족
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1333, "힘이 부족합니다."), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_STATPOINT_STR, NULL );
				break;
			case MSG_SKILL_ERROR_STATPOINT_DEX:			// 민첩 부족
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1334, "민첩이 부족합니다."), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_STATPOINT_DEX, NULL );
				break;
			case MSG_SKILL_ERROR_STATPOINT_INT:			// 지혜 부족
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1335, "지혜가 부족합니다."), SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_STATPOINT_INT, NULL );
				break;
			case MSG_SKILL_ERROR_STATPOINT_CON:			// 체질 부족
				_pUIMgr->GetChatting()->AddSysMessage( _S( 1336, "체질이 부족합니다."), SYSMSG_ERROR ); 
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_STATPOINT_CON, NULL );
				break;
			case MSG_SKILL_ERROR_TARGET_PKLIMITLEVEL: // 15레벨 이하는 pvp를 할 수 없습니다.
				_pUIMgr->GetChatting()->AddSysMessage( _S( 2230,"15레벨 이하의 케릭터는 공격할 수 없습니다." ), SYSMSG_ERROR ); 
				_pUIMgr->LostTarget();
				penPlayerEntity->Read_net_Character( MSG_SKILL_ERROR_STATPOINT_CON, NULL );
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_TIME:			// 소환 불가능 : 소환수 사망후 5분동안 소환 불가
				{
					CTString strMessage;
					int iMin, iSec;
					if( nExtraInfo == 0 )
					{
						iMin = 0;
						iSec = 1;
					}
					else
					{							
						iMin = nExtraInfo / 60;
						iSec = nExtraInfo % 60;
					}
					strMessage.PrintF( _S(2360, "[%d분 %d초] 후에 소환수가 소환 가능합니다." ), iMin, iSec );						
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				}				
				//_pUIMgr->GetChatting()->AddSysMessage( _S(2361, "소환수가 사망하고 5분동안은 소환이 불가능합니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_FIRE:			// 소환 불가능 : 불의정령과 중복 불가
				_pUIMgr->GetChatting()->AddSysMessage( _S(2362, "불의 정령이 이미 소환되어 있거나, 불의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_WIND:			// 소환 불가능 : 바람의정령과 중복 불가
				_pUIMgr->GetChatting()->AddSysMessage( _S(2363, "바람의 정령이 이미 소환되어 있거나, 바람의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_EARTH:		// 소환 불가능 : 대지의정령과 중복 불가
				_pUIMgr->GetChatting()->AddSysMessage( _S(2364, "대지의 정령이 이미 소환되어 있거나, 대지의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_WATER:		// 소환 불가능 : 물의정령과 중복 불가
				_pUIMgr->GetChatting()->AddSysMessage( _S(2365, "물의 정령이 이미 소환되어 있거나, 물의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_EVOCATION_ALREADY:
				_pUIMgr->GetChatting()->AddSysMessage( _S(2366, "이미 강신한 상태입니다." ), SYSMSG_ERROR ); 
				//_pUIMgr->CancelSkill();
				break;
			case MSG_SKILL_ERROR_EVOCATION_TIME:
				{
					CTString strMessage;
					int iMin, iSec;
					if( nExtraInfo == 0 )
					{
						iMin = 0;
						iSec = 1;
					}
					else
					{							
						iMin = nExtraInfo / 60;
						iSec = nExtraInfo % 60;
					}
					strMessage.PrintF( _S(2367, "[%d분 %d초] 후에 강신 가능합니다." ), iMin, iSec );
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				}				
				break;
			case MSG_SKILL_ERROR_CANNOT_INVISIBLE:
				{
					_pUIMgr->GetChatting()->AddSysMessage( _S(3167, "몬스터에게 타켓팅이 되어 있을 경우 스킬을 사용할 수 없습니다." ), SYSMSG_ERROR ); 
				}
				break;
			case MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC:
				{
					_pUIMgr->GetChatting()->AddSysMessage( _S(3168, "반경 15M이내 당신이 모르는 유저가 존재하여 스킬을 사용할 수 없습니다." ), SYSMSG_ERROR ); 
				}
				break;
			case MSG_SKILL_ERROR_MISS_ROGUE_SKILL:		// 070725_ttos: 와일드 라쏘 스킬(index 125) 실패
				_pUIMgr->GetChatting()->AddSysMessage( _S(3595, "스킬 사용에 실패하였습니다." ), SYSMSG_ERROR ); 
				break;
			
			case MSG_SKILL_ERROR_COMPETITION:
				_pUIMgr->GetChatting()->AddSysMessage( _S(4264, "쿨타임이 적용 중입니다." ), SYSMSG_ERROR ); 
				break;
			}

			// NOTE : 스킬 스펠하고 사용을 못하면 락이 걸리기 때문에...
			_pUIMgr->CancelSkill(FALSE, TRUE);
		}
	}
}

// 생산 관련 특수 스킬...(Passive Skill임)
//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveSSkillMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;//1013	
	(*istr) >> ubType;

	switch(ubType)
	{
	case MSG_SSKILL_LIST:
		{
			_pUIMgr->GetCharacterInfo()->ClearSSkills();
			
			SLONG	slIndex;
			SBYTE	sbCount, sbLevel;//1013
			(*istr) >> sbCount;
			
			for( int iSkill = 0; iSkill < sbCount; iSkill++ )
			{
				(*istr) >> slIndex;
				(*istr) >> sbLevel;
				
				_pUIMgr->GetCharacterInfo()->AddSkill( slIndex, sbLevel, TRUE );
			}
		}
		break;
	case MSG_SSKILL_LEARN:
		{
			SLONG	slIndex;
			SBYTE	sbIsNew;
			SBYTE	sbLevel;
			
			(*istr) >> slIndex;
			(*istr) >> sbIsNew;
			(*istr) >> sbLevel;
			
			_pUIMgr->GetSkillLearn()->LearnSkill( slIndex, sbIsNew, sbLevel, TRUE );
		}
		break;
	case MSG_SSKILL_REMOVE:
		{
			SLONG	slIndex;
			(*istr) >> slIndex;
			_pUIMgr->GetCharacterInfo()->RemoveSkill( slIndex, TRUE );
		}
		break;
	case MSG_SSKILL_LEARN_ERROR:		// 에러 발생.
		{
			UBYTE ubError;
			(*istr) >> ubError;

			_pUIMgr->GetSkillLearn()->LearnSSkillError( ubError );			
		}
		break;
	}
}

void CSessionState::ReceiveStatMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;//1013
	SLONG   swRemainedStat;
	(*istr) >> ubType;

	if( ubType == MSG_STATPOINT_REMAIN )
	{
		(*istr) >> swRemainedStat;

		_pNetwork->MyCharacterInfo.StatPoint = swRemainedStat;
		_pUIMgr->GetCharacterInfo()->NotifyStatPoint();
	}
	else if( ubType == MSG_STATPOINT_USE )
	{
		UBYTE	ubStatType;
		SLONG	slStatChange;
		(*istr) >> ubStatType;
		(*istr) >> slStatChange;
		(*istr) >> swRemainedStat;

		_pNetwork->MyCharacterInfo.StatPoint = swRemainedStat;
		_pUIMgr->GetCharacterInfo()->UseStatPoint( ubStatType, slStatChange );
	}
	else if( ubType == MSG_STATPOINT_ERROR )
	{
		SBYTE	sbError;
		(*istr) >> sbError;

		_pUIMgr->GetCharacterInfo()->StatPointError( sbError );
	}
	// 스탯 초기화시.
	else if( ubType == MSG_STATPOINT_RESET )
	{
		LONG	lStr;
		LONG	lDex;
		LONG	lInt;
		LONG	lCon;
		int		iTotalStat = 0;

		(*istr) >> lStr;
		(*istr) >> lDex;
		(*istr) >> lInt;
		(*istr) >> lCon;
		iTotalStat += lStr;
		iTotalStat += lDex;
		iTotalStat += lInt;
		iTotalStat += lCon;

		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1337, "총 %d 스탯이 초기화되었습니다." ), iTotalStat );		
		_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_NORMAL );
		_pUIMgr->GetInitJob()->CloseInitJob();
	}
}

void CSessionState::ReceiveUIMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;

	if( ubType == MSG_UI_PLUS_EFFECT_REP )
	{
		SBYTE	sbItemPlusEffect;
		SLONG	slCharIndex;

		(*istr) >> sbItemPlusEffect;
		(*istr) >> slCharIndex;

		if( slCharIndex == _pNetwork->MyCharacterInfo.index )
		{
			_pNetwork->MyCharacterInfo.sbItemEffectOption = sbItemPlusEffect;
		}
		else
		{
			INDEX cnt = _pNetwork->ga_srvServer.srv_actCha.Count();
			for( INDEX i = 0; i < cnt; ++i )
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
				if( ct.cha_Index == slCharIndex )
				{
					ct.SetItemEffectOption( sbItemPlusEffect );
					ct.cha_itemEffect.Refresh(&(ct.cha_pEntity->GetModelInstance()->m_tmSkaTagManager), sbItemPlusEffect );
				}
			}
		}
	}
}

void CSessionState::ReceiveSystemMessage( CNetworkMessage *istr )
{
	SBYTE		sbType;
	SBYTE		sbTab;	// 1013
	CTString	strSysMessage;

	(*istr) >> sbType;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	switch( sbType )
	{
		case MSG_SYS_WHISPER_NOTFOUND:	// 귓말 대상 없음	:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 304, "귓속말 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_TIMEOUT:			// 타임 아웃 발생	:
			strSysMessage = "MSG_SYS_TIMEOUT";
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;

		case MSG_SYS_CANNOT_WEAR:		// 착용 오류		:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 305, "장비를 착용할 수 없습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_FULL_INVENTORY:	// 인벤토리 가득 참	: tab_index(uc)
			(*istr) >> sbTab;//1013
			_pUIMgr->GetChatting()->AddSysMessage( _S( 265, "인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_CUSTOM:			// 커스텀 메시지	: msg(str)
			{
				CTString	strMsg;//1013
				(*istr) >> strMsg;//1013
				strSysMessage = "MSG_SYS_CUSTOM";
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;

		case MSG_SYS_SHORT_MONEY:		// 돈 사용시 모질람	:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 306, "나스가 부족합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_SHOP_CANCEL:		// 상점 거래 취소	:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 307, "상점 거래를 취소하셨습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_EXCHANGE_NOTFOUND:	// 교환 대상 없음	:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 308, "교환 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
			
			_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		    if (_pUIMgr->IsCSFlagOn( CSF_EXCHANGE ))
			{
				_pUIMgr->SetCSFlagOff( CSF_EXCHANGE );
			}
			break;

		case MSG_SYS_EXCHANGE_ALREADY:	// 이미 교환 중		: 
			_pUIMgr->GetChatting()->AddSysMessage( _S( 309, "이미 교환중입니다." ), SYSMSG_ERROR );
			
			_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		    if (_pUIMgr->IsCSFlagOn( CSF_EXCHANGE ))
			{
				_pUIMgr->SetCSFlagOff( CSF_EXCHANGE );
			}
			break;

		/*
		case MSG_SYS_SKILL_ALREADY_USE:	// 다른 스킬 사용중
			_pUIMgr->GetChatting()->AddSysMessage( _S( 315, "다른 스킬을 사용중입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_ALREADY_USE, NULL );
			break;
		
		case MSG_SYS_SKILL_NOTFOUND:		// 그런 스킬 없어요
			_pUIMgr->GetChatting()->AddSysMessage( _S( 316, "존재하지 않는 스킬입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_NOTFOUND, NULL );
			break;

		case MSG_SYS_SKILL_NOTREADY:		// 스킬 준비 중... 즉, 이전 스킬 시전후 다음 스킬 가능 시간이 안되었을때
			_pUIMgr->GetChatting()->AddSysMessage( _S( 317, "스킬을 준비중입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_NOTREADY, NULL );
			break;

		case MSG_SYS_SKILL_INVAL_TARGET:	// 타겟이 잘못되었음
			_pUIMgr->GetChatting()->AddSysMessage( _S( 318, "타겟이 잘못되었습니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_INVAL_TARGET, NULL );
			break;
		
		case MSG_SYS_SKILL_INSUFF:		// 조건이 안 맞아 사용 못함(HP, MP, 아이템 부족)
			{
				SBYTE	sbErrType;
				(*istr) >> sbErrType;

				switch( sbErrType )
				{
				case 0:		// HP
					strSysMessage = _S( 319, "HP가 부족합니다." );
					break;
				case 1:		// MP
					strSysMessage = _S( 320, "MP가 부족합니다." );
					break;
				case 2:		// Item
					strSysMessage = _S( 321, "답글" );
					break;
				}
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_INSUFF, NULL );
			}
			break;

		case MSG_SYS_SKILL_LONG:			// 멀어서 안됨
			_pUIMgr->GetChatting()->AddSysMessage( _S( 322, "거리가 멀어서 사용할 수 없습니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_LONG, NULL );
			break;
*/
		case MSG_SYS_NOT_OWNER_ITEM:		// 아이템 우선권이 없음
			_pUIMgr->GetChatting()->AddSysMessage( _S( 323, "아이템 권한이 없습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_OVER_WEIGHT:			// 무게가 오바 瑛?
			_pUIMgr->GetChatting()->AddSysMessage( _S( 327, "무게가 초과되었습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_UPGRADE_NOCONDITION:	// 조건이 안맞아 업그레이드 못함 (일반제련석 레벨이 맞지 않음)
			_pUIMgr->GetChatting()->AddSysMessage( _S( 328, "레벨이 맞지 않아 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );			
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			break;

		case MSG_SYS_MAKE_REFINE_KIND:		// 무기나 방어구가 아닌 다른걸 제련석으로 전환하려할 때
			_pUIMgr->GetChatting()->AddSysMessage( _S( 329, "무기와 방어구만 업그레이드 할 수 있습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MAKE_REFINE_WEARING:	// 입고 있는 아이템으로 제련석 전환 불가능
			_pUIMgr->GetChatting()->AddSysMessage( _S( 330, "착용된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MAKE_REFINE_PLUS:		// Plus 값이 셋팅되어 있는 아이템 제련석 전환 불가능
			_pUIMgr->GetChatting()->AddSysMessage( _S( 331, "업그레이드 된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MEMPOS_CANT_WRITE:		// 장소 기억 장치 사용 불가능 존
			_pUIMgr->GetChatting()->AddSysMessage( _S( 333, "메모리 스크롤을 사용할 수 없는 지역입니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_LAYERDUNGEON_FULL:		// 레이어 존 가득 차서 입장 불가
			strSysMessage = _S( 1338, "레이어 존이 가득 차서 입장 할 수 없습니다." );		
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;

		case MSG_SYS_CANT_WARP_STATE:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 415, "이동할 수 없는 상태입니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_CANT_WARP_OTHER:		// 다른 이동 장치 사용중으로 이동 장치 사용 불가
			_pUIMgr->GetChatting()->AddSysMessage( _S( 334, "다른 이동 장치를 사용중입니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MEMPOS_OTHERZONE:		// 다른 존으로는 장소 기억 이동 불가
		case MSG_SYS_MEMPOS_CASTLE:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 335, "메모리 스크롤로 이동할 수 있는 지역이 아닙니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_SINGLEDUNGEON_FULL:	// 싱글 던젼이 꽉 차있음.
			strSysMessage = _S( 1339, "싱글 던젼 인원 초과로 입장 할 수 없습니다." );			
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;
/*
		case MSG_SYS_SKILL_INVAL_STATE:		// 상태 불만족			: flag(c: 0 - 죽었을때만 가능, 1 - 무기 장비시만 가능, 2 - 앉아서만 가능, 3 - 피스존 불가능, 4 - 서서만 가능
			{
				SBYTE	sbErrType;
				(*istr) >> sbErrType;

				switch( sbErrType )
				{
					case 0:		
						strSysMessage = _S( 390, "죽었을 때만 사용가능 합니다." );
						break;
					case 1:		
						strSysMessage = _S( 391, "무기를 장착해야 사용가능 합니다." );
						break;
					case 2:	
						strSysMessage = _S( 392, "앉아있을 때만 사용가능 합니다." );
						break;
					case 3:		
						strSysMessage = _S( 393, "평화존에서는 사용 불가능합니다." );
						break;
					case 4:		
						strSysMessage = _S( 394, "서있을 때만 사용가능 합니다." );
						break;
				}
				
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
*/
		case MSG_SYS_MAKE_REFINE_SHORT:		// 제련석 만들때 돈 모질람
			_pUIMgr->GetChatting()->AddSysMessage( _S( 369, "제련 비용이 부족합니다." ), SYSMSG_ERROR );
			break;
		case MSG_SYS_PRODUCE_NOTOOL:			// 생산도구가 없음
			_pUIMgr->GetChatting()->AddSysMessage( _S( 657, "생산에 필요한 도구가 없습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_WEAR_TOOL:		// 생산도구를 착용하시오
			_pUIMgr->GetChatting()->AddSysMessage( _S( 658, "생산 도구를 착용하시오." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MATCH_TOOL:		// 맞는 생산도구를 착용하시오
			_pUIMgr->GetChatting()->AddSysMessage( _S( 659, "적절하지 않은 생산 도구입니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MAXUSE_TOOL:	// 생산도구 다 썼음
			_pUIMgr->GetChatting()->AddSysMessage( _S( 660, "생산 도구가 낡아서 파괴되었습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MAX_ACCEPTABLE:	// 생산 가능 인원 초과
			_pUIMgr->GetChatting()->AddSysMessage( _S( 661, "생산 가능 인원을 초과했습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_NO_SSKILL:			// 해당하는 생산 스킬이 없다!!
			_pUIMgr->GetChatting()->AddSysMessage( _S( 662, "해당하는 생산 스킬이 없습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PROCESS_DOC_NOT_SSKILL:// 가공문서 쓰는데 가공스킬이 없음
			_pUIMgr->GetChatting()->AddSysMessage( _S( 663, "생산품을 가공하기 위한 가공스킬이 없습니다" ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_WARNING_WEIGHT:// 무게 초과 경고메시지.
			_pUIMgr->GetChatting()->AddSysMessage( _S( 664, "한계 무게를 초과하여, 민첩이 감소합니다." ), SYSMSG_ERROR );		
			break;
		case MSG_SYS_BLOODITEM:
			{
				SLONG	itemindex;
				(*istr) >> itemindex;
				
				//CItemData &ItemData	= _pNetwork->GetItemData(itemindex);
				//CTString strSysMessage;
				strSysMessage.PrintF( _S( 703, "%s의 아이템에 블러드 옵션을 사용할 수 있습니다." ), _pNetwork->GetItemName(itemindex) );
				_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				
				//_pUIMgr->GetChatting()->AddSysMessage( CTString(_S( 665, "블러드 옵션을 사용할수 있습니다." )) );		
				break;
			}
		case MSG_SYS_MAKE_REFINE_CANNOT:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 666, "해당 아이템은 제련석으로 교환할 수 없는 아이템입니다." ), SYSMSG_ERROR );	
			break;

		case MSG_SYS_PKMODE_LIMITLEVEL:		// PK 선언이 불가능한 레벨
			_pUIMgr->GetChatting()->AddSysMessage(  _S( 806, "15레벨 이하는 PVP를 할 수가 없습니다." ) , SYSMSG_ERROR );	
			break;

		case MSG_SYS_SHORT_FAME:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 1340, "명성치가 부족합니다." ), SYSMSG_ERROR );		
			break;

		case MSG_SYS_FRIEND_NOTFOUND:
			{
				// 현재 활성화 되어 있는 메제 창으로 에러 메세지를 보내자 
				_pUIMgr->GetMessenger()->TalkErrorMessage(_S( 2064,  "대화상대가 오프라인이여서 메세지를 전달할 수 없습니다." ) );	
			}
			break;
		//TEMP: 초고급 제련석
		case MSG_SYS_UPGRADE_CANT_SUPERSTONE:
			{
				strSysMessage = _S( 1746, "초 고급 제련석을 이미 사용한 장비입니다. 다른 아이템에 사용해 주십시요." );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1673, "제련 불가!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				if(_pUIMgr->GetInventory()->IsLocked()) //wooss 051024
					_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			} break;
		case MSG_SYS_UPGRADE_CANT_14LEVEL:
			{
				strSysMessage = _S( 1747, "초 고급 제련석은 +14장비에는 사용할 수 없습니다." );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1670, "주의!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				if(_pUIMgr->GetInventory()->IsLocked()) //wooss 051024
					_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			} break;
		case MSG_SYS_CAN_RECEIVE_SUPERSTONE:
			{
				SLONG fame;
				(*istr) >> fame;
				strSysMessage.PrintF(_S( 1749, "축하합니다.\n명성치가 %d이 되었습니다.\n마법 잡화상 코엔을 통해\n초 고급 제련석을 지급받을 수\n있습니다." ), fame);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1748, "안내" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			} break;
		case MSG_SYS_FAME_NOT_ENOUGH:
			{
				strSysMessage = _S( 1750, "명성치가 부족하여 초 고급 제련석을 지급 받을 수 없습니다." );
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_INVEN_NOT_ENOUGH:
			{
				strSysMessage = _S( 1751, "사용중인 인벤토리에 빈공간이 부족하여 초 고급 제련석을 지급할 수 없습니다. 인벤토리를 비워 주십시오." );
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_NO_MORE_SUPERSTONE:
			{
				strSysMessage = _S( 1752, "초 고급 제련석을 모두 지급 받아 더이상 지급 받을 수 없습니다." );
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_APPEAR_NOVICE:
			{
				CTString strName;
				(*istr) >> strName;
				strSysMessage.PrintF(_S( 1753, "신규 유저 %s 님이 지금 게임에 접속했습니다. 후견인 제도를 통한 많은 지원 부탁드립니다." ), strName.str_String);
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_NOTIFY );
			} break;
		case MSG_SYS_NOTICE:
			{
				LONG lIndex;
				(*istr) >> lIndex;
				
				// 이벤트는 이벤트 인덱스 + 2000입니다.
				_pUIMgr->GetNotice()->AddToNoticeList(2000 + lIndex, NOTICE_EVENT);						
				_pUIMgr->GetNotice()->RefreshNoticeList();				
			} break;
		
		case MSG_SYS_PET_CANT_SUMMON:		// 소환 불가능 지역
			{
				strSysMessage = _S( 2130, "소환이 불가능한 지역입니다." );	
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;

		case MSG_SYS_PET_CANT_MOUNT:		// 마운트 불가능 지역
			{
				strSysMessage = _S( 2065, "마운트가 불가능한 지역입니다." );
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
		case MSG_SYS_PRODUCE_NO_MONEY:		// 돈없어서 생산 불가
			{
				strSysMessage = _S( 2066, "나스가 부족하여 생산이 불가능합니다. " );	
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
		case MSG_SYS_PRODUCE_CANNT_BOOST:	// 부스터로는 랜덤 생산 불가
			{
				_pUIMgr->CancelSkill();
				_pUIMgr->CloseMessageBox(MSGCMD_BOOST_RANDON_PRODUCT_ERROR);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				strSysMessage = _S( 2067, "부스터를 착용한 장비로는 선택 생산만 가능합니다." );	
						
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(2068, "생산" ), UMBS_OK, NULL, MSGCMD_BOOST_RANDON_PRODUCT_ERROR );
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );

			}
			break;
		case MSG_SYS_CANNOT_DUPLICATION:	// 부스터로는 랜덤 생산 불가
			{
				SLONG	nNewItmeIndex;
				SLONG	nCurItemIndex;

				(*istr) >> nNewItmeIndex;
				(*istr) >> nCurItemIndex;
				
				CTString strNewItmeName;
				CTString strCurItmeName;
							
				
				strNewItmeName = _pNetwork->GetItemName( nNewItmeIndex );
				strCurItmeName = _pNetwork->GetItemName( nCurItemIndex );
				
				if( nNewItmeIndex == nCurItemIndex )
				{
					strSysMessage.PrintF( _S2(2069,strNewItmeName,"%s<는> 중복하여 사용할 수 없습니다."), strNewItmeName ); 
				}
				else
				{
					strSysMessage.PrintF( _S3(2070,strNewItmeName,strCurItmeName,"%s<는> %s<과> 중복하여 사용할 수 없습니다."), strNewItmeName, strCurItmeName ); 
				}
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;		
		case MSG_SYS_UPGRADE_CANT_6LEVEL_LUCKY :	// 행운의 제력석 레벨 에러 wooss 051010
			{
				strSysMessage = _S(2151,"행운의 제련석을 사용할 수 없습니다."); 
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1673, "제련 불가!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				if(_pUIMgr->GetInventory()->IsLocked()) //wooss 051024
					_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			}
			break;
			
	case MSG_SYS_TARGET_PKLIMITLEVEL:
			{
				_pUIMgr->LostTarget();

				strSysMessage = _S(2231, "15레벨 이하의 케릭터는 공격할 수 없습니다." );
				_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
	case MSG_SYS_NOT_ADULT : 
		{
			ULONG remTime;
			(*istr)>>remTime;
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2261,"%d분후에 접속이 끊깁니다. 내일 오전 6시 이후에 접속가능합니다."), remTime);
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;

	case MSG_SYS_NOTTIME_GOZONE :
		{
			CTString strSysMessage=_S(2279, "이동 가능한 시간이 아닙니다."); // wooss 051212 번역 
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;

	case MSG_SYS_CANNOT_WEAR_DEADPET:
		{
			// MSG_SYS_CANNOT_WEAR_DEADPET,	// 060221 : bs : 사망한 펫 착용 불가 : petindex(n) remainRebirth(n)
			SLONG	remainRebirth;
			SLONG	lPetIndex;
			(*istr) >> lPetIndex;
			(*istr) >> remainRebirth;
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2508,"펫 봉인해제 시간이 %d시간 %d분 남았습니다."), remainRebirth/3600, (remainRebirth%3600)/60 );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
		
	case MSG_SYS_ITEMCOMPOSITED :			// 아이템 합성 성공
		{
			CTString strSysMessage=_S( 2730, "아이템 합성이 성공하였습니다."); // wooss 051212 번역 
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}
		break;
	
	case MSG_SYS_EXPIRE_ITEMCOMPOSITE :		// 아이템 합성 만료 : itemdbindex(n)
		{
			CTString strSysMessage;
			LONG tv_idx;
			(*istr) >> tv_idx;
		
			strSysMessage.PrintF( _S( 3041, "[%s]아이템 합성 사용이 만료 되었습니다." ),
				_pUIMgr->GetInventory()->GetItemName(tv_idx)); // wooss 051212 번역 ")
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CANNOT_RESET_MOUNT:	// 펫 마운트 타입 취소 불가
		{
			CTString strSysMessage = _S( 2900, "펫의 마운트 훈련을 취소 할 수 없습니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}	
		break;
	case MSG_SYS_RESET_MOUNT:	// 펫 마운트 타입 취소
		{
			CTString strSysMessage = _S( 2901, "애완동물의 상태 변환이 완료 되었습니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_CANNOT_SEPARATE_ITEM:
		{
			CTString strSysMessage = _S(2934, "능력치가 결합된 유료 장비 아이템만 결합 해제가 가능합니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}	
		break;
	case MSG_SYS_SEPARATE_ITEM :
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(2935, "결합된 장비[%s] 분리가 성공하였습니다."), _pNetwork->GetItemName(tv_idx));
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}
		break;

	case MSG_SYS_RAIDMOB_REGEN:
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			
			CTString strNoticeMsg;
			strNoticeMsg.PrintF("[ %s %s ]", _pNetwork->GetMobName(tv_idx), _S(3158, "몬스터가 출현하였습니다.") );
			
			_UIAutoHelp->SetGMNotice( strNoticeMsg, 0xFF5014FF);
			
		}
		break;
	case MSG_SYS_TIMEOUT_LUCKY_ACCESSORY:
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(3188, "%s의 사용기간이 만료되어 아이템이 사라집니다."), _pNetwork->GetItemName(tv_idx));
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CHANGE_LUCKY_BOX:
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(3187, "럭키드로우상자에서 %s 아이템이 나왔습니다."), _pNetwork->GetItemName(tv_idx));
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			_pUIMgr->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CANNOT_HAVE_DUPLICATE:
		{
			CTString strSysMessage = _S(3218, "중복해서 소유할 수 없는 아이템 입니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	// wooss 070307----------------------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	case MSG_SYS_MATCHSEX:
		{
			CTString strSysMessage = _S( 3256, "성별이 맞지 않습니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	// -----------------------------------------------------------------------------<<
	case MSG_SYS_NO_AUTO_ITEM:
		{
			CTString strSysMessage = _S( 3666, "더 이상 사용할 물약이 없습니다." );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
		
	case MSG_SYS_EGEHA_MINLEVEL:
		{
			CTString strSysMessage = _S( 2712, "레벨 제한 조건을 만족하지 못합니다" );
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

		}
		break;
		
		default:
			strSysMessage = "Undefined Message..";
			_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
												
			break;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	}
}

void CSessionState::ReceiveRebirthMessage( CNetworkMessage *istr )
{	
/*
	SLONG  cha_index
	SWORD  hp,maxHp,mp,maxMp;
		
	(*istr) >> cha_index;
	(*istr) >> hp;
	(*istr) >> maxHp;
	(*istr) >> mp;
	(*istr) >> maxMp;
	
	if(cha_index == _pNetwork->MyCharacterInfo.index)
	{
		_pNetwork->MyCharacterInfo.hp = hp; 
		_pNetwork->MyCharacterInfo.maxHP = maxHP;
		_pNetwork->MyCharacterInfo.mp = mp; 
		_pNetwork->MyCharacterInfo.maxMP = maxMP; 
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Rebirth();
		return;
	}
*/
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_PC_REBIRTH, &(*istr));

}


void CSessionState::ReceiveEffectMessage( CNetworkMessage *istr )
{
	//COMMON
	SBYTE	sbType;
	//SKILL
	SLONG	slSkillId;
	SBYTE	sbTargetType;
	SLONG	slTargetId;
	//ETC
	SBYTE	sbEtcType;
	SBYTE	sbChaType;
	SLONG	slChaIndex;	
		
	(*istr) >> sbType;

	switch(sbType)
	{
	case MSG_EFFECT_SKILL:
		{
			(*istr) >> slSkillId;
			(*istr) >> sbTargetType;
			(*istr) >> slTargetId;

			INDEX	iWeapon = 0;
			CEntity	*penTarget = NULL;
			if( sbTargetType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slTargetId )	//내 캐릭이면
				{
					penTarget = CEntity::GetPlayerEntity(0);
					iWeapon = _pNetwork->MyCharacterInfo.bExtension;
				}
				else													//내 캐릭이 아니면
				{
					INDEX cnt = _pNetwork->ga_srvServer.srv_actCha.Count();
					for( INDEX i = 0; i < cnt; ++i )
					{
						if( _pNetwork->ga_srvServer.srv_actCha[i].cha_Index == slTargetId )
						{
							penTarget = _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity;
							iWeapon = _pNetwork->ga_srvServer.srv_actCha[i].cha_bExtension;
							break;
						}
					}
				}

				CSkill &skill = _pNetwork->GetSkillData( slSkillId );
				if( slSkillId == 33	//Hardcording, 파티힐만, 파티클 이펙트가 잘 안나올 경우 INDEX를 의심할 것.
				 || (slSkillId == 114 && skill.GetMissileType( 0 ) == 21)//Hardcoding, baal skill 1
				 || slSkillId == 115 //Hardcoding, baal skill 2
				 || slSkillId == 149 //Hardcoding, death knight skill 1
				 || slSkillId == 150 //Hardcoding, death knight skill 2
				 || slSkillId == 169 //Hardcoding, knight telekinesis
				 || slSkillId == 226 //Hardcoding, mage flame storm
				 || skill.GetTargetType() == CSkill::STT_PARTY_ALL // 파티원 들에게 들어갈 효과
				 )
				{
					PCStartEffectGroup( skill.GetFireEffect3( iWeapon ), slTargetId, penTarget );
				}
			}
			// Date : 2005-10-28(오후 1:42:53), By Lee Ki-hwan
			// 몬스터가 몬스터에게 이펙트를 사용할때는 예외처리됨
			// 타격 이팩트 처리 됨
			else if( sbTargetType == MSG_CHAR_NPC )
			{
				if( slSkillId == 289 || slSkillId == 290 )
				{
					CPrintF( "Target = NPC, Skill ID : %d, TargetID: %d\n", slSkillId, slTargetId );

					SLONG	mob_index = -1;
					for(INDEX ipl=0; ipl< _pNetwork->ga_srvServer.srv_amtMob.Count(); ++ipl) 
					{
						CMobTarget &mt =  _pNetwork->ga_srvServer.srv_amtMob[ipl];
						if (mt.mob_Index == slTargetId ) 
						{																			
							penTarget = _pNetwork->ga_srvServer.srv_amtMob[ipl].mob_pEntity;
							iWeapon = 0;
							break;
						}
					}
				
					CSkill &skill = _pNetwork->GetSkillData( slSkillId );
					PCStartEffectGroup( skill.GetFireEffect3( iWeapon ), slTargetId, penTarget );
				}
			}
		} break;
	case MSG_EFFECT_ETC:
		{
			(*istr) >> sbChaType;
			(*istr) >> slChaIndex;
			(*istr) >> sbEtcType;
			
			if(sbEtcType==MSG_EFFECT_ETC_LEVELUP) //레벨업.
			{
				if(sbChaType != MSG_CHAR_PC) break;	//현재 몹의 레벨없은 없음.

				PCStartEffectGroup("LevelUp", slChaIndex);		//Hardcording
				
				//내 캐릭이면
				if(_pNetwork->MyCharacterInfo.index == slChaIndex)
				{
					_pNetwork->ClientSystemMessage( _S( 336, "레벨업 하였습니다." ) );
				}
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP );
			//	_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST2 );
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST8 );
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST9 );
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST10 );
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST16 );
				_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST24 );

			}
			else if(sbEtcType==MSG_EFFECT_ETC_GOZONE) //GO_World
			{
				if(sbChaType != MSG_CHAR_PC) break;	//현재 몹의 존이동은 없음.
									
				PCStartEffectGroup("Teleport End", slChaIndex);		//Hardcording
			}
			else if(sbEtcType== MSG_EFFECT_ETC_PRODUCE_MINING
				|| sbEtcType== MSG_EFFECT_ETC_PRODUCE_GATHERING
				|| sbEtcType== MSG_EFFECT_ETC_PRODUCE_CHARGE
				|| sbEtcType== MSG_EFFECT_ETC_ITEM_PROCESS
				|| sbEtcType== MSG_EFFECT_ETC_ITEM_MAKE)
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemEffect(-1, sbEtcType);
			}
			// HP 스틸 포션 이펙트
			else if(sbEtcType==MSG_EFFECT_ETC_HP_STEAL)
			{
				if(sbChaType != MSG_CHAR_PC) break;
									
				PCStartEffectGroup("HP_STEAL", slChaIndex);
			}
			// MP 스틸 포션 이펙트
			else if(sbEtcType==MSG_EFFECT_ETC_MP_STEAL)
			{
				if(sbChaType != MSG_CHAR_PC) break;
									
				PCStartEffectGroup("MP_STEAL", slChaIndex);
			}
/*
			else if(sbEtcType== MSG_EFFECT_ETC_PRODUCE_GATHERING)
			{
			}
			else if(sbEtcType== MSG_EFFECT_ETC_PRODUCE_CHARGE)
			{
			}
			else if(sbEtcType== MSG_EFFECT_ETC_ITEM_PROCESS)
			{
			}
			else if(sbEtcType== MSG_EFFECT_ETC_ITEM_MAKE)
			{
			}
*/
			// [070824: Su-won] ITEM_FIRECRACKER
			// 폭죽 아이템을 사용했을 때 나오는 이펙트
			else if(sbEtcType==MSG_EFFECT_ETC_FIRECRACKER)
			{
				if(sbChaType != MSG_CHAR_PC) break;
									
				PCStartEffectGroup("squid", slChaIndex);
			}else if(sbEtcType == MGS_EFFECT_ETC_COMPETITION)
			{
				if(sbChaType != MSG_CHAR_PC) break;
				PCStartEffectGroup("STATE_UP", slChaIndex);
			}
		} break;
	case MSG_EFFECT_PRODUCE:		// 생산 이펙트
		{
			SLONG	targetindex;
			SLONG	targethp;
			SBYTE	targettype;
			(*istr) >> sbEtcType;
			(*istr) >> sbChaType;		
			(*istr) >> slChaIndex;		
			(*istr) >> targettype;
			(*istr) >> targetindex;
			(*istr) >> targethp;

			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetProduct(slChaIndex, sbEtcType);
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ProcessProduce(sbChaType, slChaIndex, targettype, targetindex, targethp);
		}
		// FIXME : 생산 이펙트를 일단은 레벨업으로...
		//PCStartEffectGroup("LevelUp", slChaIndex);		//Hardcording
		break;		
	
	case MSG_EFFECT_ITEM:
		{		
			SLONG	slItemIndex;
			(*istr) >> sbChaType;
			(*istr) >> slChaIndex;
			(*istr) >> slItemIndex;

			if( sbChaType != MSG_CHAR_PC )
				break;
			if( slItemIndex == -1 )
				break;
			
			CItemData	&rItemData = _pNetwork->GetItemData( slItemIndex );

	#define INDEX_INVISIBLE_CURE		677
	#define INDEX_STONE_CURE			873
	#define INDEX_BLIND_CURE			874
	#define INDEX_SILENT_CURE			875	
	#define INDEX_RECOMMEND_EVENT		1393	
 
			switch( slItemIndex )
			{
			case INDEX_INVISIBLE_CURE:
				PCStartEffectGroup( "InVisibleCure", slChaIndex );
				return;
			case INDEX_STONE_CURE:
			case INDEX_BLIND_CURE:
			case INDEX_SILENT_CURE:
				PCStartEffectGroup( "CurePosion", slChaIndex );
				return;
			case INDEX_RECOMMEND_EVENT:
				PCStartEffectGroup( "server_event_using", slChaIndex );
				return;
			}
			if( rItemData.GetType() == CItemData::ITEM_POTION )
			{
				if( rItemData.GetSubType() == CItemData::POTION_HP )
				{
					PCStartEffectGroup( "potionHP", slChaIndex );
				}
				else if( rItemData.GetSubType() == CItemData::POTION_MP )
				{
					PCStartEffectGroup( "potionMP", slChaIndex );
				}

				if( slChaIndex == _pNetwork->MyCharacterInfo.index )
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemEffect(slItemIndex, -1);
				}
			}
		}
		break;
	case MSG_EFFECT_FIRE:
		{
			INDEX nExtra;
			INDEX nCount;
			INDEX index;
			INDEX nMtIndex;
			INDEX animID[5];
			FLOAT x,z,h;
//			SBYTE yLayer;
			CPlacement3D vDesiredPosition;
			CEntity* penEntity = NULL;
			//CTString strEffectname;

			(*istr) >> nExtra;
			(*istr) >> nCount;

			int i;
			CPrintF("MSG_EFFECT_FIRE\n");
			for (i=0; i<nCount; i++)
			{
				(*istr) >> index;
				(*istr) >> x;
				(*istr) >> z;
				(*istr) >> h;
		//		(*istr) >> yLayer;

				vDesiredPosition.pl_PositionVector(1) = x;
				vDesiredPosition.pl_PositionVector(2) = h;
				vDesiredPosition.pl_PositionVector(3) = z;				
/*
				INDEX j;

				for (j=0; j<_pNetwork->ga_srvServer.srv_amtMob.Count(); j++)
				{
					CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[j];
					
					if (mt.mob_Index == index)
					{
						if (_pNetwork->ga_World.EntityExists(mt.mob_iClientIndex, penEntity))
						{
							vDesiredPosition.pl_PositionVector(1) = x;
							vDesiredPosition.pl_PositionVector(2) = h;
							vDesiredPosition.pl_PositionVector(3) = z;

							penEntity->Teleport(vDesiredPosition);

							CMobData& MD = _pNetwork->GetMobData(mt.mob_iType);

							animID[0] = ska_StringToID(MD.GetAnimWalkName());		// 화염 이펙트
							animID[1] = ska_StringToID(MD.GetAnimAttackName());		// 바위 이펙트
							animID[2] = ska_StringToID(MD.GetAnimRunName());		// 번개 이펙트
							animID[3] = ska_StringToID(MD.GetAnimDefault2Name());	// 바람 이펙트
							animID[4] = ska_StringToID(MD.GetAnimAttack2Name());	// 얼음 이펙트

							if (nExtra > 0 && nExtra < 6)
								penEntity->GetModelInstance()->AddAnimation(animID[nExtra-1], AN_CLEAR, 1.0f, 0);
						}
					}
				}
*/
				// 네트워크 아이디를 서버에서 받아서 Entity를 받아오자.
				if (_pNetwork->SearchEntityByNetworkID(index, MSG_CHAR_NPC, penEntity, nMtIndex))
				{
					CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[nMtIndex];
					penEntity->Teleport(vDesiredPosition, FALSE);	

					CMobData& MD = _pNetwork->GetMobData(mt.mob_iType);
					CPrintF("NPC index : %d \n", index);

					animID[0] = ska_StringToID(MD.GetAnimWalkName());		// 화염 이펙트
					animID[1] = ska_StringToID(MD.GetAnimAttackName());		// 바위 이펙트
					animID[2] = ska_StringToID(MD.GetAnimRunName());		// 번개 이펙트
					animID[3] = ska_StringToID(MD.GetAnimDefault2Name());	// 바람 이펙트
					animID[4] = ska_StringToID(MD.GetAnimAttack2Name());	// 얼음 이펙트

					if (nExtra > 0 && nExtra < 6)
						penEntity->GetModelInstance()->AddAnimation(animID[nExtra-1], AN_CLEAR, 1.0f, 0);
				}
			}
		}
		break;
	}
}

void CSessionState::ReceiveEXPMessage( CNetworkMessage *istr )
{
	SQUAD Exp; //1013
	SLONG Sp;

	(*istr) >> Exp;
	(*istr) >> Sp;

	_pNetwork->MyCharacterInfo.curExp += Exp; 	
	_pNetwork->MyCharacterInfo.sp += Sp;

	CTString strSysMessage;
	if( Exp > 0 && Sp > 0 )
		strSysMessage.PrintF( _S( 337, "%I64d의 경험치와 %d의 SP를 얻었습니다." ), Exp, Sp );
	else if( Exp > 0 )
		strSysMessage.PrintF( _S( 427, "%I64d의 경험치를 얻었습니다." ), Exp );
	else
		strSysMessage.PrintF( _S( 428, "%d의 SP를 얻었습니다." ), Sp );
	
	_pUIMgr->GetChatting()->AddSysMessage( strSysMessage );
	_pUIMgr->GetPlayerInfo()->UpdateEXPInfo();
	_pUIMgr->GetCharacterInfo()->UpdateExpAndSp();
}
void CSessionState::ReceiveActionMessage( CNetworkMessage *istr )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_ACTION, &(*istr));
}

void CSessionState::ReceiveEnvMessage( CNetworkMessage *istr )//1013
{
	UBYTE subType;
	(*istr) >> subType;

	SLONG year, zone, sellrate, buyrate;
	SBYTE month, day, hour;
	UBYTE weather;

	// -------------------------------------->>
	LONG startWorldTime;
	// --------------------------------------<<

	if(subType == MSG_ENV_TAX_CHANGE)
	{
		(*istr) >> zone
				>> sellrate
				>> buyrate;
	}
	else if(subType == MSG_ENV_WEATHER)
	{
		(*istr) >> weather;
	}
	else if(subType == MSG_ENV_TIME)
	{
		(*istr) >> year;
		(*istr) >> month;
		(*istr) >> day;
		(*istr) >> hour;
		(*istr) >> startWorldTime;

//		if(_pNetwork->m_ubGMLevel < 2)//GM이라면 시간의 흐름을 멈춘다.
		{	
			_pUIMgr->GetRadar()->SetTime(year,month,day,hour,startWorldTime);

			if (IS_EVENT_ON(TEVENT_HOLLOWEEN2007))
			{
				g_fGWTime = 30*3600; // 할로윈데이2007이벤트에서는 계속 밤을 유지 한다.
				g_fGWTimeMul = 0.0f;
			}
			else
			{
				g_fGWTime = hour*3600;
			}
		}
	}
}

void CSessionState::ReceiveWarpMessage(CNetworkMessage *istr)
{
	UBYTE subtype;

	(*istr) >> subtype;
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	static std::map<SLONG, CEffectGroup *> mapEG;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	if(subtype == MSG_WARP_START)
	{
		SLONG	char_index;
		(*istr) >> char_index;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
		CTagManager *pTM = NULL;
		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			_pUIMgr->GetTeleport()->CloseTeleport();

			// Character state flags
			_pUIMgr->SetCSFlagOn( CSF_TELEPORT );
			_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_WARP );

			if(CEntity::GetPlayerEntity(0) != NULL
				&& CEntity::GetPlayerEntity(0)->en_pmiModelInstance != NULL)
			{
				pTM = &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager;
			}
		}
		else
		{
			INDEX cnt = _pNetwork->ga_srvServer.srv_actCha.Count();
			for(INDEX i=0; i<cnt; ++i)
			{
				if(_pNetwork->ga_srvServer.srv_actCha[i].cha_Index == char_index)
				{
					if(_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity != NULL
						&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance != NULL
						&& _pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance->GetName() != "")
					{
						pTM = &_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity->en_pmiModelInstance->m_tmSkaTagManager;
					}
					break;
				}
			}
		}
		CEffectGroup *pEG = StartEffectGroup("Teleport Start"		//Hardcording
						, pTM
						,_pTimer->GetLerpedCurrentTick());
		std::map<SLONG, CEffectGroup *>::value_type tmpVal(char_index, pEG);
		mapEG.insert(tmpVal);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	}
	else if(subtype == MSG_WARP_CANCEL)
	{
		SLONG	char_index;
		(*istr) >> char_index;

		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S( 340, "이동이 취소되었습니다." ) );

			if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TO_CANCEL)) _pUIMgr->CloseMessageBox(MSGCMD_WARP_TO_CANCEL); //wooss 050808
			if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TAKE_CANCEL)) _pUIMgr->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL); //wooss 050808
			// Character state flags
			_pUIMgr->SetCSFlagOff( CSF_TELEPORT );
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_WARP );
		}
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		std::map<SLONG, CEffectGroup *>::iterator it = mapEG.find(char_index);
		if(it != mapEG.end())
		{
			if((*it).second != NULL && CEffectGroupManager::Instance().IsValidCreated((*it).second))
				(*it).second->Stop(0);
			mapEG.erase( it );
		}
	}
	else if(subtype == MSG_WARP_END)
	{
		SLONG char_index;
		(*istr) >> char_index;

		std::map<SLONG, CEffectGroup *>::iterator it = mapEG.find(char_index);
		if(it != mapEG.end())
		{
			if((*it).second != NULL && CEffectGroupManager::Instance().IsValidCreated((*it).second))
				(*it).second->Stop(0);
			mapEG.erase( it );
		}

		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S( 395, "이동이 완료되었습니다." ) );

			if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TO_CANCEL)) _pUIMgr->CloseMessageBox(MSGCMD_WARP_TO_CANCEL); //wooss 050808
			if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WARP_TAKE_CANCEL)) _pUIMgr->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL); //wooss 050808
			// Character state flags
			_pUIMgr->SetCSFlagOff( CSF_TELEPORT );
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_WARP );
		}
	}
	else if(subtype == MSG_WARP_TELEPORT)
	{
		// pos(n) x(f:s) z(f:s) h(f:s) r(f:s) ylayer(c:s)
		LONG	lPos;
		float	fPosX;
		float	fPosZ;
		float	fPosH;
		float	fPosR;
		SBYTE	sbYLayer;

		(*istr) >> lPos;
		(*istr) >> fPosX;
		(*istr) >> fPosZ;
		(*istr) >> fPosH;
		(*istr) >> fPosR;
		(*istr) >> sbYLayer;
		
		CEntity* penPlEntity;
		CPlayerEntity* penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
		penPlayerEntity = (CPlayerEntity*) penPlEntity;
		penPlayerEntity->WarpTeleport( fPosX, fPosZ, fPosH, fPosR, sbYLayer );
	}
	//wooss 050808 
	//유료아이템 소환마법서 메세지 처리
	
	else if(subtype == MSG_WARP_TO_REQ)
	{
		CTString charName;
		(*istr) >> charName;
		ConfirmMsgProc(MSG_WARP_TO_REQ,NULL,charName);			
	}else if(subtype == MSG_WARP_TAKE_REQ)
	{
		CTString charName;
		(*istr) >> charName;
		ConfirmMsgProc(MSG_WARP_TAKE_REQ,NULL,charName);			
	}

		
	
	
	//에러처리 
	else if(subtype == MSG_WARP_ERROR)
	{
		UBYTE errType;
		CTString strName;
		(*istr) >>errType;
		
		_pUIMgr->CloseMessageBox(MSGCMD_WARP_TO_CANCEL);
		_pUIMgr->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL);
		switch(errType)
		{
			case MSG_WARP_ERROR_NOTCHAR:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTCHAR);
				break;

			case MSG_WARP_ERROR_NOTALLOW:
				(*istr) >> strName;
				ASSERT(strName!="");
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTALLOW,strName);
				break;

			case MSG_WARP_ERROR_NOTALLOWZONE:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTALLOWZONE);
				break;
			case MSG_WARP_ERROR_CANCEL:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_CANCEL);
				break;
		}

		
								
	}
	else if( subtype == MSG_WARP_PROMPT ) // 공성 시작 시 공성지역으로 이동 할 것인지 확인!!
	{		
		LONG	lZone;
		LONG	lPos;
		
		(*istr) >> lZone;
		(*istr) >> lPos;
		
		_pNetwork->MyCharacterInfo.lWarpZone = lZone;
		_pNetwork->MyCharacterInfo.lWarpPos = lPos;
		
		_pUIMgr->CloseMessageBox( MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM );
				
		_pUIMgr->m_nShowMessageBox = MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM;
		CUIMsgBox_Info	MsgBoxInfo;	
		CTString		strMessage;
		MsgBoxInfo.SetMsgBoxInfo(  _S( 2071, "공성지역 이동" ), UMBS_YESNO, UI_NONE, MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM );		
		strMessage.PrintF( _S( 2072, "공성 시작 지점으로 이동하시겠습니까?" ) );	
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	} 
		
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

void CSessionState::ReceiveMobStatusMessage(CNetworkMessage *istr)//1013
{	
	SBYTE sbType;
	(*istr) >> sbType;
	if(sbType == MSG_CHAR_NPC)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Mob(MSG_CHAR_STATUS, &(*istr));	
	}
	else if(sbType == MSG_CHAR_PC)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_CHAR_STATUS, &(*istr));	
	}
	else if(sbType == MSG_CHAR_PET)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Pet(MSG_CHAR_STATUS, &(*istr));	
	}
	else if(sbType == MSG_CHAR_ELEMENTAL)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Summon(MSG_CHAR_STATUS, &(*istr));	
	}
}

void CSessionState::ReceiveAssistMessage( CNetworkMessage *istr )
{
	SBYTE	sbType;
	(*istr) >> sbType;

	switch( sbType )
	{
	case MSG_ASSIST_ADD:
		{
			SBYTE		sbCharType;
			SLONG		slCharIndex;

			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			(*istr) >> sbCharType >> slCharIndex;
			(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;


			CEntity* pDestEntity;

			if( sbCharType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slCharIndex )
				{
				// wooss 070305 --------------------------------------------------------------------------------->>
				// kw : WSS_WHITEDAY_2007
				// 6종 러브매직 스킬
				// 423 424 425 426 427 428
				
					CEffectGroup* pEG = NULL;

					if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
					{				
						for(int i=423,j=0;i<=428;i++)
						{
							if( (_pUIBuff->IsBuffBySkill(i))!=NULL ){
								j++;
								break;
							}					
						}
						
						CEntity* penEntity = _pNetwork->GetEntityByTypeIndex(sbCharType,slCharIndex);

						// 처음이면 이펙트를 생성함...
						if(j==0)
						{					
							// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
							pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else
						{
							pEG = _pUIBuff->GetEGFromSkillIndex(i);
						}
						// 처음이 아니면 이전 CEffectGroup의 포인터를 넘겨줌...
						// 이후 버프가 지워질때 최종 하나가 지워질때 이펙트도 지워준다...
						sBuff.m_pEG =  pEG;
											
						// 추가 이펙트
						StartEffectGroup( "STATE_LOVEMAGIC", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());				
					}
				// ------------------------------------------------------------------------------------------------<<
					else if (sBuff.m_slSkillIndex == 564)	// 그림자 제거 마법가루 효과
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
					}

					_pUIBuff->AddMyBuff( sBuff );

					pDestEntity =CEntity::GetPlayerEntity( 0 );

					//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
					// wooss 070310 ----------------------->><<
					// kw : WSS_WHITEDAY_2007
					// Add lovemagic buff (wind - skill index 423 )
					if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex ==1226||sBuff.m_slItemIndex ==1227||sBuff.m_slSkillIndex == 423
						|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845 //광속 아이템, 바람의 속도 물약
						)
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, TRUE);
					}						
					CEntity	*penPlayer = CEntity::GetPlayerEntity( 0 );
					if( _pNetwork->_TargetInfo.pen_pEntity == penPlayer )
						_pUIBuff->AddTargetBuff( sBuff );
				}
				else
				{
					for( INDEX iChar = 0; iChar < _pNetwork->ga_srvServer.srv_actCha.Count(); iChar++ )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iChar];
						if( ct.cha_Index == slCharIndex )
						{

							// wooss 070305 --------------------------------------------------------------------------------->>
							// kw : WSS_WHITEDAY_2007
							// 6종 러브매직 스킬
							// 423 424 425 426 427 428
							
							CEffectGroup* pEG = NULL;

							if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
							{				
								for(int i=423,j=0;i<=428;i++)
								{
									if( (ct.IsBuffBySkill(i)) != NULL){
										j++;
										break;
									}								
								}

								CEntity* penEntity = _pNetwork->GetEntityByTypeIndex(sbCharType,slCharIndex);								
								// 처음이면 이펙트를 생성함...
								if(j==0)
								{									
									// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
									pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else 
								{
									pEG = ct.IsBuffBySkill(i);	
								}
								// 처음이 아니면 이전 CEffectGroup의 포인터를 넘겨줌...
								// 이후 버프가 지워질때 최종 하나가 지워질때 이펙트도 지워준다...
								if( pEG != NULL)
								{
									sBuff.m_pEG =  pEG;
								}
								
								// 추가 이펙트
								StartEffectGroup( "STATE_LOVEMAGIC", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());				
							}
							// ------------------------------------------------------------------------------------------------<<
							ct.AddBuff( sBuff );
							_pUIBuff->AddPartyBuff( slCharIndex, sBuff );

							pDestEntity =ct.cha_pEntity;

							//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
							// wooss 070310 ----------------------->><<
							// kw : WSS_WHITEDAY_2007
							// Add lovemagic buff (wind - skill index 423 )
							if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex ==1226||sBuff.m_slItemIndex ==1227||sBuff.m_slSkillIndex == 423
								|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
								)
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, ct.cha_iClientIndex, TRUE);
							}
							if( _pNetwork->_TargetInfo.pen_pEntity == ct.cha_pEntity )
								_pUIBuff->AddTargetBuff( sBuff );
							break;
						}
					}
				}
			}
			else if( sbCharType == MSG_CHAR_NPC )
			{
				for( INDEX iMob = 0; iMob < _pNetwork->ga_srvServer.srv_amtMob.Count(); iMob++ )
				{
					CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iMob];
					if( mt.mob_Index == slCharIndex )
					{
						mt.AddBuff( sBuff );

						pDestEntity =mt.mob_pEntity;

						if( _pNetwork->_TargetInfo.pen_pEntity == mt.mob_pEntity )
							_pUIBuff->AddTargetBuff( sBuff );
						break;
					}
				}
			}
			else if( sbCharType == MSG_CHAR_ELEMENTAL )
			{
				for( INDEX iSlave = 0; iSlave < _pNetwork->ga_srvServer.srv_actSlave.Count(); iSlave++ )
				{
					CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[iSlave];
					if( st.slave_Index == slCharIndex )
					{
						st.AddBuff( sBuff );

						pDestEntity =st.slave_pEntity;

						if( _pNetwork->_TargetInfo.pen_pEntity == st.slave_pEntity )
							_pUIBuff->AddTargetBuff( sBuff );
						break;
					}
				}
			}

			// 절망의 부적일 경우 타겟에 이펙트 생성
			if( sBuff.m_slItemIndex ==2382 )
			{
				CTString strEffect = _pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetFireEffect1(0);
				StartEffectGroup( strEffect, &pDestEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			}
		}
		break;

	case MSG_ASSIST_DEL:
		{
			SBYTE	sbCharType;
			SLONG	slCharIndex, slItemIndex, slSkillIndex;

			(*istr) >> sbCharType >> slCharIndex >> slItemIndex >> slSkillIndex;

			if( sbCharType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slCharIndex )
				{
					// wooss 070305 ----------------------------------->>
					// kw : WSS_WHITEDAY_2007
					// 6종 러브매직 스킬
					// 423 424 425 426 427 428
					if( slSkillIndex >= 423 && slSkillIndex <= 428 )
					{
						for(int i=423,j=0;i<=428;i++)
						{
							if(_pUIBuff->IsBuffBySkill(i)) j++;
						}
						
						// 마지막 버프가 사라질때 이펙트를 지워줌...
						if(j==1)
						{
							CEffectGroup* pEG = _pUIBuff->GetEGFromSkillIndex(slSkillIndex);
							if(pEG!=NULL)
							{
								DestroyEffectGroup(pEG);
							}
						}
					}
					// -------------------------------------------------<<
					else if (slSkillIndex == 564) // 그림자 제거 마법 가루 효과 삭제
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOff(ENF_SHOWHIDDEN);
						
						if (_pNetwork->_TargetInfo.dbIdx == 504 || _pNetwork->_TargetInfo.dbIdx == 505) //안개그림자 드래곤,핏빛그림자 드래곤
						{
							if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
								_pUIMgr->StopTargetEffect(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID()); // 타겟 이펙트 종료

							((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(_pNetwork->_TargetInfo.pen_pEntity);
						}
					}

					_pUIBuff->RemoveMyBuff( slItemIndex, slSkillIndex );

					
					// wooss 070310 ----------------------->><<
					// kw : WSS_WHITEDAY_2007
					// Add lovemagic buff (wind - skill index 423 )
					if( !_pUIBuff->IsBuff( 391 ) && !_pUIBuff->IsBuff( 1226 ) && !_pUIBuff->IsBuff( 1227 ) && !_pUIBuff->IsBuffBySkill(423) //이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
						&& !_pUIBuff->IsBuff( 2407 ) && !_pUIBuff->IsBuff( 2408 ) && !_pUIBuff->IsBuff(2609) && !_pUIBuff->IsBuff(2845)		//광속 아이템, 바람의 속도 물약
						)
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, FALSE);
					}

					CEntity	*penPlayer = CEntity::GetPlayerEntity( 0 );
					if( _pNetwork->_TargetInfo.pen_pEntity == penPlayer )
						_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
				}
				else
				{
					for( INDEX iChar = 0; iChar < _pNetwork->ga_srvServer.srv_actCha.Count(); iChar++ )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iChar];
						if( ct.cha_Index == slCharIndex )
						{
							// wooss 070312 ------------------------------------>>
							// kw : WSS_WHITEDAY_2007
							if( slSkillIndex >= 423 && slSkillIndex <= 428 )
							{
								for(int i=423,j=0;i<=428;i++)
								{
									if(ct.IsBuffBySkill(i)) j++;
								}
								
								// 마지막 버프가 사라질때 이펙트를 지워줌...
								if(j==1)
								{
									CEffectGroup* pEG = ct.IsBuffBySkill(slSkillIndex);
									if(pEG!=NULL)
									{
										DestroyEffectGroup(pEG);
									}
								}
							}					
							// -------------------------------------------------<<

							ct.RemoveBuff( slItemIndex, slSkillIndex );
							_pUIBuff->RemovePartyBuff( slCharIndex, slItemIndex, slSkillIndex );

							
							// wooss 070310 ----------------------->><<
							// kw : WSS_WHITEDAY_2007
							// Add lovemagic buff (wind - skill index 423 )
							//if( !_pUIBuff->IsBuff( 391 ) && !_pUIBuff->IsBuff( 1226 ) && !_pUIBuff->IsBuff( 1227 ) &&!_pUIBuff->IsBuffBySkill(423) )//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
							if( !ct.IsBuff( 391 ) && !ct.IsBuff( 1226 ) && !ct.IsBuff( 1227 ) &&!ct.IsBuffBySkill(423) //이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
								&&!ct.IsBuff( 2407 ) && !ct.IsBuff( 2408 ) && !ct.IsBuff( 2609 ) && !ct.IsBuff(2845)	//광속 아이템
								)
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, ct.cha_iClientIndex, FALSE);
							}

							if( _pNetwork->_TargetInfo.pen_pEntity == ct.cha_pEntity )
								_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
							break;
						}
					}
				}
			}
			else if( sbCharType == MSG_CHAR_NPC )
			{
				for( INDEX iMob = 0; iMob < _pNetwork->ga_srvServer.srv_amtMob.Count(); iMob++ )
				{
					CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iMob];
					if( mt.mob_Index == slCharIndex )
					{
						mt.RemoveBuff( slItemIndex, slSkillIndex );
						if( _pNetwork->_TargetInfo.pen_pEntity == mt.mob_pEntity )
							_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
						break;
					}
				}
			}
			else if( sbCharType == MSG_CHAR_ELEMENTAL )
			{
				for( INDEX iSlave = 0; iSlave < _pNetwork->ga_srvServer.srv_actSlave.Count(); iSlave++ )
				{
					CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[iSlave];
					if( st.slave_Index == slCharIndex )
					{
						st.RemoveBuff( slItemIndex, slSkillIndex );
						if( _pNetwork->_TargetInfo.pen_pEntity == st.slave_pEntity )
							_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
						break;
					}
				}
			}
		}
		break;

	case MSG_ASSIST_LIST:
		{
			SLONG	slAssistState;
			SBYTE	sbAssistCount;
			(*istr) >> slAssistState >> sbAssistCount;

			_pUIBuff->ResetMyBuff();			

			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			CEffectGroup* pEG = NULL;

			for( int i = 0; i < sbAssistCount; i++ )
			{
				(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;
							
				// wooss 070305 ------------------------------------>>
				// kw : WSS_WHITEDAY_2007
				// 6종 러브매직 스킬
				// 423 424 425 426 427 428
				if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
				{				
						
					for(int i=423,j=0;i<=428;i++)
					{
						if(_pUIBuff->IsBuffBySkill(i)) j++;
					}
					
					// 처음이면 이펙트를 생성함...
					if(j==0)
					{
						CEntity* penEntity =(CPlayerEntity*)CEntity::GetPlayerEntity(0);
						pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				//		penEntity->GetModelInstance()->pEG = StartEffectGroup( 
				//			"STATE_LOVE", 
				//			&penEntity->en_pmiModelInstance->m_tmSkaTagManager, 
				//			_pTimer->GetLerpedCurrentTick());
					}

					sBuff.m_pEG =  pEG;							
				}
				// -------------------------------------------------<<
				else if (sBuff.m_slSkillIndex == 564)	// 그림자 제거 마법가루 효과
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
				}
				
				// wooss 070310 ----------------------->><<
				// kw : WSS_WHITEDAY_2007
				// Add lovemagic buff (wind - skill index 423 )
				if(sBuff.m_slItemIndex ==391 ||sBuff.m_slItemIndex ==1226 ||sBuff.m_slItemIndex ==1227 ||sBuff.m_slSkillIndex ==423//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
					|| sBuff.m_slItemIndex ==2407 ||sBuff.m_slItemIndex ==2408 ||sBuff.m_slItemIndex == 2609 ||	sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
					)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, TRUE);
				}

				_pUIBuff->AddMyBuff( sBuff );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveQuestMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveQuestMessage(CNetworkMessage *istr)
{
	UBYTE ubType;
	(*istr) >> ubType;

	MSG_QUEST_TYPE msgQuestType = MSG_QUEST_TYPE(ubType);
	switch(msgQuestType)
	{
	case MSG_QUEST_PC_LIST:
		{
			INDEX questCount;
			INDEX questIndex;
			INDEX questCurCount[QUEST_MAX_CONDITION];

			(*istr) >> questCount;			// 수행중인 퀘스트 수

			CUIQuestBook::UnlockQuest();
			_pUIMgr->GetQuestBookList()->ClearQuestList();
			CQuestSystem::Instance().ClearAllDynamicData();
			for( INDEX iQuest = 0; iQuest < questCount; ++iQuest )
			{
				(*istr) >> questIndex;		// 퀘스트 번호
				for(int i = 0; i < QUEST_MAX_CONDITION; ++i)
				{
					(*istr) >> questCurCount[i];
				}

				CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( questIndex );
				if( pQuestDD == NULL )
				{
					pQuestDD = CQuestSystem::Instance().Create( questIndex );
					if( pQuestDD != NULL )
					{
						CQuestSystem::Instance().AddCurrentRequest();
						pQuestDD->SetCurrentConditionValueStatus( questCurCount );
						BOOL bInserted = _pUIMgr->GetQuestBookList()->AddToQuestList( questIndex, pQuestDD->IsQuestComplete() );
						ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
						CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_DOING);
					}
					else
					{
						ASSERTALWAYS("MSG_QUEST_LIST : Quest Index is abnormal. Server or Client has wrong quest index.");
					}
				}
			}
			_pUIMgr->GetQuestBookList()->RefreshQuestList();
		}
		break;
	case MSG_QUEST_NPC_LIST:
		{
			//TODO : NewQuestSystem
			INDEX iQuestCount;
			INDEX iQuestIndex;
			SBYTE sbQuestFlag;

			(*istr) >> iQuestCount;

			CUIQuestBook::UnlockQuest();
			_pUIMgr->GetQuestBookNew()->ClearQuestList();
			_pUIMgr->GetQuestBookComplete()->ClearQuestList();

			INDEX iQuestValid = 0;
			for( INDEX iQuest = 0; iQuest < iQuestCount; ++iQuest )
			{
				(*istr) >> iQuestIndex;
				(*istr) >> sbQuestFlag;
				if(sbQuestFlag == QUEST_FLAG_CAN)
				{
					BOOL bInserted = _pUIMgr->GetQuestBookNew()->AddToQuestList(iQuestIndex, FALSE);
					ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
					++iQuestValid;
				}
				else if(sbQuestFlag == QUEST_FLAG_COMPLETE)
				{
					BOOL bInserted = _pUIMgr->GetQuestBookComplete()->AddToQuestList(iQuestIndex, TRUE);
					ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
					++iQuestValid;
				}
			}

			_pUIMgr->GetQuestBookList()->SetTargetIndex(_pUIMgr->GetQuestBookNew()->GetTargetIndex());
			_pUIMgr->GetQuestBookNew()->OpenWindow( iQuestValid > 0 ? TRUE : FALSE );
		}
		break;

	case MSG_QUEST_PD4_RANK_VIEW_REP:		// 싱글던젼 랭킹 뷰 응답.
		{
			// 랭크 뷰응답	: multicount(c) charname(str) bclear(c) deadmon(n) time(ll)
			SBYTE		sbMultiCount;
			CTString	strName;
			SBYTE		sbSuccess;
			INDEX		iNumOfDeadMonster;
			__int64		llTime;

			(*istr) >> sbMultiCount;

			_pUIMgr->GetRanking()->ClearRankingList();

			for( int i = 0; i < sbMultiCount; ++i )
			{
				(*istr) >> strName;
				(*istr) >> sbSuccess;
				(*istr) >> iNumOfDeadMonster;
				(*istr) >> llTime;
				_pUIMgr->GetRanking()->AddToRankingList( strName, sbSuccess, iNumOfDeadMonster, llTime );
			}
			
			if( !_pUIMgr->GetRanking()->IsVisible() )
				_pUIMgr->GetRanking()->OpenRankingList();
			else
				_pUIMgr->GetRanking()->RefreshRankingList();
		}
		break;

	case MSG_QUEST_PD4_RANK_REWARD_RANK_REP:	// 싱글던젼 보상 응답.
		{
			// 보상 뷰응답	: multicount(c) charname(str) itemindex(n) itemnum(c) breward(c)			
			SBYTE		sbMultiCount;
			CTString	strName;
			INDEX		iItemIndex;
			SBYTE		sbNumOfItem;
			SBYTE		sbReward;

			(*istr) >> sbMultiCount;

			for( int i = 0; i < sbMultiCount; ++i )
			{
				(*istr) >> strName;
				(*istr) >> iItemIndex;
				(*istr) >> sbNumOfItem;
				(*istr) >> sbReward;
				_pUIMgr->GetRanking()->AddToPrizeList( strName, iItemIndex, sbNumOfItem, sbReward );
			}

			_pUIMgr->GetRanking()->OpenRankingPrize();
		}
		break;

	case MSG_QUEST_PD4_REWARD:	// 보상을 받은 경우.
		{
			_pUIMgr->GetRanking()->ResetRanking();
		}
		break;

	case MSG_QUEST_PD4_ERROR:	// 에러 발생.
		{
			SBYTE	sbErrType;
			(*istr) >> sbErrType;
			CTString strMessage;

			switch( sbErrType )
			{
			case MSG_QUEST_ERR_REWARD_NOT_RANK:		// 보상시 랭커가 아님.
				strMessage = _S( 1754, "보상을 받을 수 있는 조건이 아닙니다." );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_QUEST_ERR_BREWARD:				// 이미 보상 받았음.
				strMessage = _S( 1755, "이미 보상을 받았습니다." );						
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			}
			_pUIMgr->GetRanking()->ResetRanking();
		}
		break;

	case MSG_QUEST_CREATE_LIST://자동 퀘스트 부분
		{
			//TODO : NewQuestSystem
			INDEX iQuestCount;
			INDEX iQuestIndex;
			
			(*istr) >> iQuestCount;
			
			for( INDEX iQuest = 0; iQuest < iQuestCount; ++iQuest )
			{
				(*istr) >> iQuestIndex;
				_pUIMgr->GetNotice()->AddToNoticeList(iQuestIndex, NOTICE_QUEST);
			}
			if(iQuestCount > 0)
			{
				CTString strLevelQuest;
				strLevelQuest.PrintF( _S( 770, "레벨 퀘스트를 %d개 받았습니다." ), iQuestCount);		
				_pNetwork->ClientSystemMessage( strLevelQuest );

				_pUIMgr->GetNotice()->RefreshNoticeList();

				//_pUIMgr->GetNotice()->InitNotice();
				//_pUIMgr->GetNotice()->OpenNotice();
			}
		}
		break;

	case MSG_QUEST_START:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			INDEX remainTime = -1;		// [090609: selo] 타임어택에서의 남은 시간
			(*istr) >> questIndex;
#ifdef NEW_QUESTBOOK			
			(*istr) >> remainTime;
#endif
			//Check Point Init
			_pNetwork->SetCheckPointData(0,0,0);

			CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create(questIndex);
			//_pUIMgr->GetCharacterInfo()->OpenQuestIntro( pQuestDD );

			//TODO : NewQuestSystem
			ASSERT( CQuestSystem::Instance().GetCurrentRequest() != NULL &&
					questIndex == CQuestSystem::Instance().GetCurrentRequest()->GetQuestIndex() );
			CQuestSystem::Instance().AddCurrentRequest();

			BOOL bInserted = _pUIMgr->GetQuestBookList()->AddToQuestList( questIndex, FALSE );
			ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");

			// [090609: selo] 타임어택의 남은 시간을 설정한다.
			_pUIMgr->GetQuestBookList()->SetQuestRemainTime(questIndex, remainTime);

			_pUIMgr->GetQuestBookList()->RefreshQuestList();
			_pUIMgr->GetQuestBookList()->OpenQuestBook();

			CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_DOING);
			INDEX iTargetIndex = _pUIMgr->GetQuestBookNew()->GetTargetIndex();
			CQuestSystem::Instance().RefreshNPCQuestMark(iTargetIndex);

			_pUIMgr->GetQuestBookNew()->CloseQuestBook();
			
			if( pQuestDD->GetQuestType1() == QTYPE_KIND_SAVE)			// 싱글던젼 퀘스트의 경우(구출 퀘스트의 경우)
			{
				// 일단 던젼으로 이동 시키고나서...
				// 무조건 0으로 판단함.				
				const int iWorldNum = pQuestDD->GetConditionData(0, 0);						// 월드 번호.
				const int iExtraNum = pQuestDD->GetConditionData(0, 1);						// Extra 번호.
				_pNetwork->GoZone(iWorldNum, iExtraNum,pQuestDD->GetPrizeNPCIndex());
			}
			else if( pQuestDD->GetQuestType1() == QTYPE_KIND_DEFEAT )	// 싱글던젼 퀘스트의 경우(격파 퀘스트의 경우)
			{
				// 일단 던젼으로 이동 시키고나서...
				// 무조건 0으로 판단함.
				const int iWorldNum = pQuestDD->GetConditionData(0, 0);						// 월드 번호.
				const int iExtraNum = pQuestDD->GetConditionData(0, 1);						// Extra 번호.
				_pNetwork->GoZone(iWorldNum, iExtraNum,pQuestDD->GetPrizeNPCIndex());
			}		
		}
		break;

	case MSG_QUEST_STATUS:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			INDEX currentConditionValue[3];

			(*istr) >> questIndex;
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			int changeIndex = -1;
			for(int i = 0; i < QUEST_MAX_CONDITION; ++i)
			{
				(*istr) >> currentConditionValue[i];
				if(pData->GetCurrentConditionValueStatus(i) != currentConditionValue[i]
				&& (pData->GetConditionType(i) == QCONDITION_ITEM
					|| pData->GetConditionType(i) == QCONDITION_ITEM_NORMAL
					|| pData->GetConditionType(i) == QCONDITION_NPC)
				)
					changeIndex = i;
			}

			if(pData != NULL)
			{
				pData->SetCurrentConditionValueStatus(currentConditionValue);
				_pUIMgr->GetQuestBookContent()->RefreshQuestContent(questIndex);

				if( changeIndex != -1
				&& (_UIAutoHelp->GetGMNoticeColor() == 0xFFFFFFFF || !_UIAutoHelp->IsShowGMNotice()) )
				{
					CTString strCurrentChangedStatus = CQuestSystem::Instance().MakeInfoForCondition(
						questIndex
						, pData->GetConditionType(changeIndex)
						, pData->GetConditionIndex(changeIndex)
						, currentConditionValue[changeIndex]
						, pData->GetConditionNum(changeIndex));
					if(strCurrentChangedStatus.Length() != 0)
						_UIAutoHelp->SetGMNotice(strCurrentChangedStatus, 0xFFFFFFFF);
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_STATUS : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		}
		break;

	case MSG_QUEST_COMPLETE:
		{
			INDEX questIndex;
			(*istr) >> questIndex;

			//TODO : NewQuestSystem
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);

			if(pData)
			{
				pData->MakeQuestComplete();
				BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, FALSE);
				if(!bDeleted)//뻑나는거 방지용
				{
					bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, TRUE);
					ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
					if(!bDeleted)
					{
						ASSERTALWAYS("Somthing is wrong, unknown quest index");
					}
				}
				BOOL bInserted = _pUIMgr->GetQuestBookList()->AddToQuestList(questIndex, TRUE);
				ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
				_pUIMgr->GetQuestBookList()->RefreshQuestList();
				_pUIMgr->GetQuestBookContent()->RefreshQuestContent(questIndex);

				CQuestSystem::Instance().RefreshNPCQuestMark(pData->GetPrizeNPCIndex());

				// 싱글던젼 격파형 퀘스트의 경우.
				if(_pNetwork->m_bSingleMode)
				{
					if(pData->GetQuestType1() == QTYPE_KIND_DEFEAT || pData->GetQuestType1() == QTYPE_KIND_SAVE )
					{
						_pUIMgr->CloseMessageBox(MSGCMD_QUEST_COMPLETE);
						CTString		strMessage;
						CUIMsgBox_Info	MsgBoxInfo;	
						MsgBoxInfo.SetMsgBoxInfo(  _S( 99, "퀘스트" ), UMBS_OK, UI_CHARACTERINFO, MSGCMD_QUEST_COMPLETE);		
						strMessage.PrintF( _S( 667, "[%s] 퀘스트를 완료 했습니다." ), pData->GetName());	
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );

						// 공주 구출 후 파티를 끊는다. [12/26/2006 Theodoric]
						if( _pNetwork->m_bSingleMode && _pUIMgr->GetParty()->GetMemberCount())
						{
							_pUIMgr->GetParty()->PartyEnd();
						}
						// 스타트 존으로 이동시킴.
						//_pNetwork->GoZone(0, 0);
					}
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_COMPLETE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		}
		break;

	case MSG_QUEST_UNCOMPLETE:
		{
			INDEX questIndex;
			(*istr) >> questIndex;
			//TODO : NewQuestSystem
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				pData->MakeQuestUncomplete();
				BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, TRUE);
				if(!bDeleted)//뻑나는거 방지용
				{
					bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, FALSE);
					ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
					if(!bDeleted)
					{
						ASSERTALWAYS("Somthing is wrong, unknown quest index");
					}
				}
				BOOL bInserted = _pUIMgr->GetQuestBookList()->AddToQuestList(questIndex, FALSE);
				ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
				_pUIMgr->GetQuestBookList()->RefreshQuestList();
				_pUIMgr->GetQuestBookContent()->RefreshQuestContent(questIndex);

				CQuestSystem::Instance().RefreshNPCQuestMark(pData->GetPrizeNPCIndex());
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_UNCOMPLETE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		} break;

	case MSG_QUEST_GIVEUP:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;

			if( CQuestSystem::Instance().GetCurrentRequest() != NULL &&
				questIndex == CQuestSystem::Instance().GetCurrentRequest()->GetQuestIndex() )
			{
				CQuestSystem::Instance().DeleteCurrentRequest();
				CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
				CQuestSystem::Instance().RefreshNPCQuestMark(_pUIMgr->GetQuestBookNew()->GetTargetIndex());
				_pUIMgr->GetQuestBookNew()->CloseQuestBook();
			}
			else
			{
				CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
				if( pData != NULL )
				{
					if(_pUIMgr->GetQuestBookComplete()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
					if(_pUIMgr->GetQuestBookContent()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookContent()->CloseQuestBook();
					BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList( questIndex, pData->IsQuestComplete() );
					if(!bDeleted)//뻑나는거 방지용
					{
						bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, !pData->IsQuestComplete());
						ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
						if(!bDeleted)
						{
							ASSERTALWAYS("Somthing is wrong, unknown quest index");
						}
					}
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);

					INDEX startType = pData->GetStartType();
					INDEX startData = pData->GetStartData();
					INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
					CQuestSystem::Instance().Remove( questIndex );
					if(startType == QSTART_NPC)
						CQuestSystem::Instance().RefreshNPCQuestMark(startData);
					CQuestSystem::Instance().RefreshNPCQuestMark(prizeNPCIndex);

					// 공주구출 퀘수행도중 포기하고 나와도 공주 인포창이 그대로 있는 버그 [12/27/2006 Theodoric]
					if( questIndex == 14 ) // 공주구출
					{	_pNetwork->EndRescueNPC();	}
				}
				else
				{
					CQuestSystem::Instance().DeleteCurrentRequest();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
					CQuestSystem::Instance().RefreshNPCQuestMark(_pUIMgr->GetQuestBookNew()->GetTargetIndex());
					_pUIMgr->GetQuestBookNew()->CloseQuestBook();
				}
			}
			_pUIMgr->GetQuestBookList()->RefreshQuestList();
		}
		break;

	case MSG_QUEST_PRIZE:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;
			
			CUIQuestBook::UnlockQuest();
			_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
			if(_pUIMgr->GetQuestBookContent()->GetCurrentQuest() == questIndex)
			{
				_pUIMgr->GetQuestBookContent()->CloseQuestBook();
			}
			
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				if(pData->GetQuestType1() != QTYPE_KIND_TUTORIAL )
				{
					CTString		strMessage;
					strMessage.PrintF("%s", pData->GetName());
					
					INDEX iPrizeNPCIndex = pData->GetPrizeNPCIndex();
					if( CQuestSystem::Instance().Remove( questIndex ) )
					{
						BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, TRUE);
						if(!bDeleted)//뻑나는거 방지용
						{
							bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, FALSE);
							ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
							if(!bDeleted)
							{
								ASSERTALWAYS("Somthing is wrong, unknown quest index");
							}
						}
					}

					_pUIMgr->GetQuestBookList()->RefreshQuestList();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALREADY_DONE);
					CQuestSystem::Instance().RefreshNPCQuestMark(iPrizeNPCIndex);

					INDEX iContactNpc = CQuestSystem::Instance().SearchContactQuestIndex(questIndex);

					if (iContactNpc>0 && iContactNpc!=iPrizeNPCIndex)
					{
						CQuestSystem::Instance().RefreshNPCQuestMark(iContactNpc);
					}
				}
				// FIXME : 튜토리얼 퀘스트만 특이해서 이렇게 처리함...
				else
				{
					CTString		strMessage;
					strMessage.PrintF("%s", pData->GetName());
					
					// Create skill learn message box
					_pUIMgr->CreateMessageBoxL( strMessage, UI_CHARACTERINFO, MSGLCMD_QUEST_TUTORIAL );
					
					strMessage.PrintF("%s", pData->GetDesc2());
					_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL, TRUE, strMessage );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL, FALSE, _S( 191, "확인" ), 0 );
					_pUIMgr->GetQuest()->LockQuest(TRUE);
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_PRIZE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		}
		break;

	case MSG_QUEST_ERROR:
		{
			//TODO : NewQuestSystem
			SBYTE	sbErrType;
			(*istr) >> sbErrType;

			if( CQuestSystem::Instance().GetCurrentRequest() )
				CQuestSystem::Instance().DeleteCurrentRequest();

			CUIQuestBook::UnlockQuest();
			_pUIMgr->GetQuest()->QuestError(sbErrType);
			//_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
			//_pUIMgr->GetQuestBookNew()->CloseQuestBook();
			//_pUIMgr->GetQuestBookContent()->CloseQuestBook();
			//_pUIMgr->GetQuestBookList()->CloseQuestBook();
		}
		break;

	case MSG_QUEST_FAIL:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;
			
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				_pUIMgr->GetQuest()->SetCurQuest(questIndex);
				INDEX startType = pData->GetStartType();
				INDEX startData = pData->GetStartData();
				INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
				//CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
				if(_pNetwork->MyCharacterInfo.hp != 0)
				{					
					_pUIMgr->CloseMessageBox(MSGCMD_QUEST_FAIL);
					CTString		strMessage;
					CUIMsgBox_Info	MsgBoxInfo;	
					MsgBoxInfo.SetMsgBoxInfo(  _S( 99, "퀘스트" ), UMBS_OK, UI_QUEST, MSGCMD_QUEST_FAIL);		
					strMessage.PrintF( _S( 669, "[%s] 퀘스트를 실패했습니다." ), pData->GetName());	
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );		
				}
				else
				{
					CTString strQuestFail;// = "퀘스트를 실패했습니다.";		
					strQuestFail.PrintF( _S( 669, "[%s] 퀘스트를 실패했습니다." ), pData->GetName()); 
					_pNetwork->ClientSystemMessage( strQuestFail );
				}
				
				// 싱글던젼의 경우에는 버튼을 누른다음에 제거해줌.
				if( !_pNetwork->m_bSingleMode )
				{
					BOOL isComplete = pData->IsQuestComplete();
					if( CQuestSystem::Instance().Remove( questIndex ) )
					{
						BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, isComplete);
						if(!bDeleted)//뻑나는거 방지용
						{
							bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, !isComplete);
							ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
							if(!bDeleted)
							{
								ASSERTALWAYS("Somthing is wrong, unknown quest index");
							}
						}
					}
					_pUIMgr->GetQuestBookList()->RefreshQuestList();
					
					if(_pUIMgr->GetQuestBookContent()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookContent()->CloseQuestBook();
					if(_pUIMgr->GetQuestBookComplete()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALLOW);
					
					if(startType == QSTART_NPC)
						CQuestSystem::Instance().RefreshNPCQuestMark(startData);
					CQuestSystem::Instance().RefreshNPCQuestMark(prizeNPCIndex);
				}				

				if( questIndex == 14 ) // 공주 구출
				{
					_pNetwork->EndRescueNPC();
				}
			}
		}
		break;
		
	case MSG_QUEST_DONE_LIST:
		{
			INDEX completeQuestCount;
			(*istr) >> completeQuestCount;
			for(int i=0; i<completeQuestCount; ++i)
			{
				INDEX completeQuestIndex;
				(*istr) >> completeQuestIndex;
				CQuestSystem::Instance().SetQuestAllow(completeQuestIndex, CQuestSystem::QAT_ALREADY_DONE);
			}
		} break;
	case MSG_QUEST_ABANDON_LIST:
		{
			INDEX giveupQuestCount;
			(*istr) >> giveupQuestCount;
			for(int i=0; i<giveupQuestCount; ++i)
			{
				INDEX giveupQuestIndex;
				(*istr) >> giveupQuestIndex;
				CQuestSystem::Instance().SetQuestAllow(giveupQuestIndex, CQuestSystem::QAT_GIVEUP);
			}
		} break;
	case MSG_QUEST_FORCE_GIVEUP:
		{
			INDEX questIndex;
			(*istr) >> questIndex;
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if( pData != NULL )
			{
				//확인창
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_LIST, MSGCMD_QUEST_NOTIFY );
				CTString strMessage;
				strMessage.PrintF( _S( 1756, "%s 퀘스트가 적정 수행 레벨을 지나 삭제됩니다." ), pData->GetName() );
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );

				//관련해서 열려있는 창 닫기 및 퀘스트 상태 변경
				if(_pUIMgr->GetQuestBookContent()->GetCurrentQuest() == questIndex)
					_pUIMgr->GetQuestBookContent()->CloseQuestBook();
				if(_pUIMgr->GetQuestBookComplete()->GetCurrentQuest() == questIndex)
					_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
				BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList( questIndex, pData->IsQuestComplete() );
				if(!bDeleted)//뻑나는거 방지용
				{
					bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, !pData->IsQuestComplete());
					ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
					if(!bDeleted)
					{
						ASSERTALWAYS("Somthing is wrong, unknown quest index");
					}
				}
				CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);

				//npc의 퀘스트 마크 갱신
				INDEX startType = pData->GetStartType();
				INDEX startData = pData->GetStartData();
				INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
				CQuestSystem::Instance().Remove( questIndex );
				if(startType == QSTART_NPC)
					CQuestSystem::Instance().RefreshNPCQuestMark(startData);
				CQuestSystem::Instance().RefreshNPCQuestMark(prizeNPCIndex);
			}
			//퀘스트 리스트 갱신
			_pUIMgr->GetQuestBookList()->RefreshQuestList();
		} break;
		
	case MSG_QUEST_COLLECT :
		{
			LONG npcIdx,charIdx,itemIdx;
			(*istr) >> npcIdx>>charIdx>>itemIdx;
			
			// 	>	0 : NPC는 사망하고 itemdbindex를 지급함
			//	==	0 : NPC는 사망하고 아이템 지급 없음
			//	<   0 : NPC는 그대로 아이템 지급 없음

			if(itemIdx >= 0)
			{	
				// npc를 죽임 ㅡㅡ;;
				CEntity* penEntity = NULL;
				CEntity *penTargetEntity = NULL;
			
				if( _pNetwork->SearchEntityByNetworkID( npcIdx, MSG_CHAR_NPC, penTargetEntity ) )
				{
					((CMovableModelEntity*)penTargetEntity)->DeathNow();
				}
				
				//  수집 중지, 탈착한 무기 장착
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->AppearWearingWeapon();
				
			}

		}
		break;

	default:
		{
			//ASSERTALWAYS("Not defined MSG_QUEST_TYPE.");
		}
		break;
	}
}
			
// ----------------------------------------------------------------------------
// Name : ReceivePkMessage()
// Desc : 
// ----------------------------------------------------------------------------			
void CSessionState::ReceivePkMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;
	
	switch(ubType)
	{
	case MSG_PK_PENALTY_HPMP:
		{
			ULONG ulHPCount;
			ULONG ulMPCount;
			(*istr) >> ulHPCount;
			(*istr) >> ulMPCount;
			_pNetwork->MyCharacterInfo.hpcount	= ulHPCount;
			_pNetwork->MyCharacterInfo.mpcount	= ulMPCount;
		}
		break;
	case MSG_PK_ITEMSEAL:
		{
			SBYTE sbTab;
			SBYTE sbRow;
			SBYTE sbCol;
			ULONG ulIndex;
			ULONG ulFlag;
			(*istr) >> sbTab;
			(*istr) >> sbRow;
			(*istr) >> sbCol;
			(*istr) >> ulIndex;
			(*istr) >> ulFlag;
			
			const int iIndex = _pNetwork->MySlotItem[sbTab][sbRow][sbCol].Item_Index;
			//CItemData& ID = _pNetwork->GetItemData(iIndex);
			CTString strItemSealed;
			strItemSealed.PrintF( _S2( 670, _pNetwork->GetItemName(iIndex), "[%s]<가> 봉인되었습니다." ), _pNetwork->GetItemName(iIndex) );		
			_pNetwork->ClientSystemMessage( strItemSealed );
		}
		break;	
	case MSG_PK_RECOVER_HPMP:		// hp, mp 면죄 응답
		{
			SBYTE sbHPCount;
			SBYTE sbMPCount;
			(*istr) >> sbHPCount;
			(*istr) >> sbMPCount;
			CTString strItemSealed;
			strItemSealed.PrintF( _S( 671, "HP : %d, MP : %d 가 면죄되었습니다." ), sbHPCount, sbMPCount);	
			_pNetwork->ClientSystemMessage( strItemSealed );
			_pUIMgr->GetRemission()->InitRemission();
		}
		break;
	case MSG_PK_RECOVER_ITEMSEAL:	// item 봉인 응답
		{
			SBYTE sbTab;
			SBYTE sbRow;
			SBYTE sbCol;
			SLONG slIndex;
			SLONG slFlag;
			(*istr) >> sbTab;
			(*istr) >> sbRow;
			(*istr) >> sbCol;
			(*istr) >> slIndex;
			(*istr) >> slFlag;

			const int iIndex = _pNetwork->MySlotItem[sbTab][sbRow][sbCol].Item_Index;
			//CItemData& ID = _pNetwork->GetItemData(iIndex);
			CTString strItemSealed;
			strItemSealed.PrintF( _S2( 672, _pNetwork->GetItemName(iIndex), "[%s]<가> 봉인이 해제되었습니다." ), _pNetwork->GetItemName(iIndex) );	
			_pNetwork->ClientSystemMessage( strItemSealed );
			_pUIMgr->GetRemission()->InitRemission();
		}
		break;
	case MSG_PK_TARGET_SHOWHP: // WSS_PK_TARGET_HP 070726
		{
			SBYTE	sbON;
			(*istr) >> sbON;
			_pUIMgr->GetTargetInfo()->ShowPKTargetHP( (BOOL)sbON );
		}
		break;
	case MSG_PK_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			_pUIMgr->GetRemission()->RemissionError( sbError );
		}
		break;
	default:
		{
			ASSERTALWAYS("Not defined MSG_QUEST_TYPE.");
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveEventMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveEventMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	
	
	switch(ubType)
	{
	case MSG_EVENT_2PAN4PAN_LETTER:
		{
			LONG lError;
			(*istr) >> lError;
			
			_pUIMgr->GetSelectWord()->ResultProcess( lError );
						
		}
		break;
	case MSG_EVENT_ERROR:
		{
			SBYTE	sbError;
			ULONG	tv_index;
			(*istr) >> sbError;
			
			_pNetwork->m_iLastError = sbError;
			CTString strMessage;
			switch(sbError)
			{
			case MSG_EVENT_ERROR_ALREADY_GIVE:
				strMessage.PrintF( _S( 673, "이벤트 아이템이 이미 지급되었습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_INSUFF_CONDITION:
				strMessage.PrintF( _S( 674, "이벤트 아이템을 받을 수 없습니다." ) );			
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_FULLINVENTORY:
				strMessage.PrintF( _S( 675, "인벤토리가 가득차서 이벤트 아이템을 받을 수 없습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_WRONG_PACKET:
				strMessage.PrintF( _S( 1341, "잘못된 패킷입니다." ) );				
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_NO_MATCH:
				strMessage.PrintF( _S( 1342, "같은 레벨의 아이템이 없습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CANT_OPTION:
				strMessage.PrintF( _S( 1343, "아이템 옵션을 옮길 수 없습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;

			case MSG_EVENT_ERROR_CHUSEOK_NO_CROP:
				strMessage.PrintF( _S( 1879, "작물이 없습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
		
			case MSG_EVENT_ERROR_CHUSEOK_NO_SOIL:
				if(g_iCountry == THAILAND ){
					(*istr) >> tv_index;
						strMessage.PrintF( _S( 1880, "작물을 업그레이드 하기 위해서는 [%s] 10개가 필요합니다." ), 
						_pNetwork->wo_aItemName[tv_index].GetName());	

				} else 	strMessage.PrintF( _S( 1880, "작물을 업그레이드 하기 위해서는 '배양토'가 필요합니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;

			case MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE:
				strMessage.PrintF( _S( 1881, "이미 작물의 업그레이드 최종단계인 열매단계까지 도달하셨습니다. 보상품으로 교환하십시오." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK:
				strMessage.PrintF( _S( 1882, "작물 업그레이드에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL:
				strMessage.PrintF( _S( 1883, "작물 업그레이드에 실패하였습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK:
				strMessage.PrintF( _S( 1884, "추석 이벤트 보상품이 지급되었습니다. 이벤트에 참여해 주셔서 감사합니다." ) );	
			//	_pNetwork->ClientSystemMessage( strMessage );
				break;

			// 060118 wooss japan package item...
			// 말레이시아 프로모 패키지 이벤트
			case MSG_EVENT_ERROR_NOT_COUPON :					// 패키지 아이템 지급, 없는 시리얼
				//strMessage.PrintF( _S( 2405, "요청한 쿠폰 번호가 존재하지 않습니다." ) );	
				strMessage.PrintF( _S(3153, "올바른 인증Key가 아닙니다." ) );	
				break;
			
			case MSG_EVENT_ERROR_ALREADY_USE_COUNPON :		// 패키지 아이템 지급, 이미 지급 받은 쿠폰
				//strMessage.PrintF( _S( 2406, "이미 지급받은 쿠폰 번호 입니다." ) );	
				strMessage.PrintF( _S( 3152, "이미 등록된 인증key 입니다." ) );	
				break;
			
			case MSG_EVENT_ERROR_COUPON_SUCCESS :				// 패키지 아이템 지급, 지급 성공
				strMessage.PrintF( _S( 1840, "아이템이 성공적으로 지급 되었습니다" ) );	
				break;		
			case MSG_EVENT_ERROR_SAKURABUD_SUCCESS: // 060322 eons 일본 전통의상 교환 이벤트
				strMessage.PrintF( _S( 2545, "벚꽃 봉우리 교환 아이템이 성공적으로 지급 되었습니다." ) );
				break;
			case MSG_EVENT_ERROR_SAKURABUD_LEAKITEM: // 060322 eons
				strMessage.PrintF( _S( 2546, "벚꽃 봉우리가 부족합니다. 확이 후 다시 시도해 주세요." ) );
				break;

			case MSG_EVENT_ERROR_SEPTEMBER_NAS:
				{
					strMessage.PrintF( _S( 2073, "나스 당첨(사운드)" ) );	
					//_pNetwork->ClientSystemMessage( strMessage );
					
					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_DROP:
				{
					strMessage.PrintF( _S( 2074, "드롭율 당첨(사운드)" ) );	
				//	_pNetwork->ClientSystemMessage( strMessage );
					
					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_EXP:
				{	
					strMessage.PrintF( _S( 2075, "경험치 당첨(사운드)" ) );	
				//	_pNetwork->ClientSystemMessage( strMessage );
						
					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_SP:
				{
					strMessage.PrintF( _S( 2076, "숙련도 당첨(사운드)" ) );	
				//	_pNetwork->ClientSystemMessage( strMessage );

						CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;				
			}			
			
			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_PREOPEN_GIFT:
		{
			SLONG slIndex;
			(*istr) >> slIndex;

			_pUIMgr->CloseMessageBox(MSGCMD_EVENT_PRIZE);
			CTString		strMessage;
			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			//CItemData& ID = _pNetwork->GetItemData(slIndex);
			strMessage.PrintF( _S( 676, "[%s] 이벤트 아이템을 얻었습니다." ), _pNetwork->GetItemName(slIndex) );	
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	// Date : 2005-01-28,   By Lee Ki-hwan
	case MSG_EVENT_LATTO:
		{
			unsigned char ucIndex;
			(*istr) >> ucIndex;
			
			CTString		strMessage;
				
			switch ( ucIndex )
			{
			case MSG_EVENT_LATTO_UPGRADE_SUCCESS: // "인첸트 역전 라또 아이템 +2에 당첨 되었습니다"
				strMessage = "인첸트 역전 라또 아이템 +2에 당첨 되었습니다";
				_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
				break;
			case MSG_EVENT_LATTO_BLOOD_SUCCESS:	// "블러드 역전 라또 아이템 옵션에 당첨 되었습니다"
				strMessage = "블러드 역전 라또 아이템 옵션에 당첨 되었습니다";
				_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_BLOOD_OPTION  );
				break;
			case MSG_EVENT_LATTO_PROCESS_SUCCESS:// "가공 역전 라또 아이템에 당첨 되었습니다"
				strMessage = "가공 역전 라또 아이템에 당첨 되었습니다";
				_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESS  );
				// 가공창 닫기 
				_pUIMgr->GetProcess()->CloseProcess();
				break;	
			case MSG_EVENT_LATTO_EXPSP_SUCCESS:	// "레벨 역전 라또 (경험치 *%: 숙련도 *)에 당첨 되었습니다" : exp(n) sp(n)
				{
					SLONG Exp; //1013
					SLONG Sp;

					(*istr) >> Exp;
					(*istr) >> Sp;
					strMessage.PrintF ( "레벨 역전 라또 (경험치 %ld%%, 숙련도 %ld)에 당첨 되었습니다", Exp, Sp );
				}
				break;
			
			case MSG_EVENT_LATTO_PARTY_SUCCESS:	// "파티 역전 라또 아이템 복주머니에 당첨 되었습니다" : 시스템 창으로 msg 처리
				{
					strMessage = "파티 역전 라또 아이템 복주머니에 당첨 되었습니다";
					_pUIMgr->GetChatting()->AddSysMessage( strMessage );
					return;
				}
				break;


			case MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP : 	// 복주머니 교환 응답		: type (c)
				{
					UBYTE	ubType;
					(*istr) >> ubType;

					switch ( ubType )
					{
					case MSG_CHANGE_26LEVEL_GENERAL_STONE:			// 26레벨 일반 제련석
						strMessage = "26레벨 일반 제련석을 얻었습니다.";
						break;
					case MSG_CHANGE_SPECIAL_STONE:					// 고급제련석
						strMessage = "고급 제련석을 얻었습니다.";	
						break;
					case MSG_CHANGE_22LEVEL_EVENT_WEAPON:			// 22레벨 이벤트 무기 (종족별)
						strMessage = "22레벨 이벤트 무기을 얻었습니다.";
						break;
					case MSG_CHANGE_31LEVEL_ARMOR:					// 31레벨 방어구 상의 (종족별)
						strMessage = "31레벨 방어구 상의을 얻었습니다.";
						break;
					case MSG_CHANGE_ATTACK_UP:						// 공격력 향상제
						strMessage = "공격력 향상제를 얻었습니다.";
						break;
					case MSG_CHANGE_DEFENSE_UP:						// 방어력 향상제
						strMessage = "방어력 향상제를 얻었습니다.";
						break;
					case MSG_CHANGE_50000NAS:						// 5만 나스
						strMessage = "5만 나스를 얻었습니다.";
						break;
					case MSG_CHANGE_NO_LUCKYBAG:					// 교환할 복주머니가 없다
						strMessage = "교환할 복 주머니가 없습니다.";
						break;
					}

				}
				break;
			}
			
			_pUIMgr->CloseMessageBox(MSGCMD_EVENT_PRIZE);
			
			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		}
		break;
	case MSG_EVENT_LETTER:
		{
			unsigned char ucIndex;
			(*istr) >> ucIndex;
			
			CTString		strMessage;
			switch( ucIndex )
			{
			case MSG_EVENT_LETTER_LASTCHAOS:
				strMessage	= "[라스트 카오스] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_NAKO:						// 나코 당첨										: S -> C
				strMessage	= "[나코] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_KNIGHT:					// 나이트 당첨										: S -> C
				strMessage	= "[나이트] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_MERAC:					// 메라크 당첨										: S -> C
				strMessage	= "[메라크] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_CRIS:						// 크리스 당첨										: S -> C
				strMessage	= "[크리스] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_LAMIAN:					// 라미안 당첨										: S -> C
				strMessage	= "[라미안] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_ARCHWAND:					// 아크완드 당첨									: S -> C
				strMessage	= "[아크완드] 낱말을 맞추셨습니다.";
				break;
			case MSG_EVENT_LETTER_NOMATCH:					// 낱말 맞추기 실패									: S -> C
				strMessage	= "낱말 맞추기에 실패하셨습니다.";
				break;
			case MSG_EVENT_LETTER_WRONGPACKET:				// 이상한 패킷 보냄
				{
					ASSERTALWAYS( "Invalid Packet!!!" );
				}
				break;
			}

			_pUIMgr->GetQuiz()->ResetQuiz();

			_pUIMgr->CloseMessageBox(MSGCMD_EVENT_PRIZE);
			
			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);			
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

		}
		break;
	case MSG_EVENT_CHANGEWEAPON:		// 무기 교체 이벤트
		{
			LONG lOldIndex;
			LONG lNewIndex;

			(*istr) >> lOldIndex;
			(*istr) >> lNewIndex;

			CTString strOldItem = _pNetwork->GetItemName(lOldIndex);
			CTString strNewItem = _pNetwork->GetItemName(lNewIndex);
			
			CTString strMessage;
			strMessage.PrintF( _S( 1344, "[%s] 무기가 [%s] 무기로 교체되었습니다." ), strOldItem, strNewItem );		
			_pUIMgr->GetChatting()->AddSysMessage( strMessage );			
		}
		break;
	case MSG_EVENT_MOONSTONE:				// 문스톤 이벤트...
		{
			unsigned char ucIndex;
			(*istr) >> ucIndex;
				
			switch ( ucIndex )
			{
			case MSG_EVENT_MOONSTONE_START_REP:
				{
					SBYTE sbType;
					LONG lAccumulate;
					(*istr) >> sbType;
					(*istr) >> lAccumulate;
					_pUIMgr->GetGamble()->SetAccumulate( lAccumulate );
					_pUIMgr->GetGamble()->ShowGamble();
					_pUIMgr->SetCSFlagOn( CSF_MOONSTONE );
				}
				break;

			case MSG_EVENT_MOONSTONE_TRY_REP:		// 문스톤 복권 응답
				{
					unsigned char ucSubType;
					(*istr) >> ucSubType;
					switch( ucSubType )
					{
					case MSG_CHANGE_NO_MOONSTONE:
						{
							CTString strMessage;
							strMessage.PrintF( _S( 1439, "문스톤이 없습니다." ) );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							_pUIMgr->GetGamble()->ResetGamble();
						}
						break;					
					case MSG_CHANGE_MOONSTONE_RULLET:
						{
							LONG lSelectedMarker;
							LONG lSelectedCount;
							LONG lAccumulate;
							(*istr) >> lSelectedMarker;
							(*istr) >> lSelectedCount;
							(*istr) >> lAccumulate;
							
							_pUIMgr->GetGamble()->SetSelectedMarker( lSelectedMarker, lSelectedCount );
							_pUIMgr->GetGamble()->Start();
							_pUIMgr->GetGamble()->SetAccumulate( lAccumulate );
							//_pUIMgr->SetCSFlagOn( CSF_MOONSTONE );
							//_pUIMgr->GetGamble()->Stop();
						}
						break;
					}
				}
				break;
			case MSG_EVENT_MOONSTONE_RESULT_REP:	// 문스톤 복권 결과 응답.ㄴ
				{
					SBYTE	sbPrizeType;					
					(*istr) >> sbPrizeType;
					_pUIMgr->GetGamble()->Stop();
					switch( sbPrizeType )
					{										
					case MSG_CHANGE_150000_EXP:					    // 경험치 150,000
						{
							CTString strMessage;
							strMessage.PrintF( _S( 1442, "경험치 150,000을 얻었습니다." ) );		
						//	_pNetwork->ClientSystemMessage( strMessage );
							_pUIMgr->GetGamble()->ShowPrizeEffect();
						}
						break;
					case MSG_CHANGE_70000_EXP:					    // 경험치 70,000	
						{
							CTString strMessage;
							strMessage.PrintF(_S( 1443, "경험치 70,000을 얻었습니다." ) );
						//	_pNetwork->ClientSystemMessage( strMessage );
							_pUIMgr->GetGamble()->ShowPrizeEffect();
						}
						break;
					case MSG_CHANGE_25000_EXP:					    // 경험치 25,000							
						{
							CTString strMessage;
							strMessage.PrintF(_S( 1444, "경험치 25,000을 얻었습니다." ));		
						//	_pNetwork->ClientSystemMessage( strMessage );
							_pUIMgr->GetGamble()->ShowPrizeEffect();
						}
						break;

					case MSG_CHANGE_JACK_POT:						// 잭팟.
					case MSG_CHANGE_5PLUS_41LEVEL_WEAPON:			// 41레벨 +5 무기(종족별)	
					case MSG_CHANGE_3PLUS_41LEVEL_WEAPON:			// 41레벨 +3 무기(종족별)	
					case MSG_CHANGE_41LEVEL_WEAPON:					// 41레벨 무기 (종족별)
					case MSG_CHANGE_3_SPECIAL_STONE:				// 고급제련석 3개
					case MSG_CHANGE_2_SPECIAL_STONE:				// 고급제련석 2개
					case MSG_CHANGE_1_SPECIAL_STONE:				// 고급제련석 1개						
					case MSG_CHANGE_CRYSTAL_OF_EFFORT:				// 노력의 결정.
					case MSG_CHANGE_HIGH_CRYSTAL_OF_EXPERIMENCE:	// 상급 경험의 결정.					
					case MSG_CHANGE_MIDDLE_CRYSTAL_OF_EXPERIMENCE:	// 중급 경험의 결정
					case MSG_CHANGE_10_PURE_STONE:
					case MSG_CHANGE_3_MIDDLE_CRYSTAL_OF_EXPERIMENCE:
					case MSG_CHANGE_5_12GRADE_RED_KER:				//12레벨 붉은 케르 5개
					case MSG_CHANGE_5_12GRADE_RED_NEN:				//12레벨 붉은 넨 5개
					case MSG_CHANGE_5_12GRADE_BLUE_KER:				//12레벨 푸른 케르 5개
					case MSG_CHANGE_5_BIG_MP_POTION:				//대형 MP물약 5개
					case MSG_CHANGE_5_12GRADE_BLUE_NEN:				//12레벨 푸른 넨 5개
					case MSG_CHANGE_5_MOONSTONE:					//문스톤 5개
					case MSG_CHANGE_3_6GRADE_RED_KER:				//6레벨 붉은 케르 3개 
					case MSG_CHANGE_3_6GRADE_RED_NEN:				//6레벨 붉은 넨 3개
					case MSG_CHANGE_3_6GRADE_BLUE_KER:				//6레벨 푸른 케르 3개
					case MSG_CHANGE_3_BIG_HEAL_POTION:				//대형 HP물약 3개
					case MSG_CHANGE_3_6GRADE_BLUE_NEN:				//6레벨 푸른넨 3개
					case MSG_CHANGE_3_MIDDLE_MP_POTION:				//중형 MP물약 3개
					case MSG_CHANGE_3_MEMORY_SCROLL:				//메모리 스크롤 3개
					case MSG_CHANGE_3_C_GRADE_ELEMENTAL:			//c등급 원소 3개
					case MSG_CHANGE_3_CRAK_BLUE_LEAF:			 	//크락 파란잎 3개
					case MSG_CHANGE_3_STONE_WEAVER:					//스톤 웨버 3개
					case MSG_CHANGE_3_D_GRADE_ELEMENTAL:			//D등급 원소 3개
					case MSG_CHANGE_3_MIDIUM_HEAL_POTION:			//중형회복물약 5개
					case MSG_CHANGE_3_CRAK_BRANCH:					//크락 줄기 3개
					case MSG_CHANGE_3_STONELISE:					//스톤 리스
						{
							_pUIMgr->GetGamble()->ShowPrizeEffect();
							//CTString strMessage;
							//strMessage.PrintF( _S( 1452, "당첨 되었습니다!!!" ));		
							//_pNetwork->ClientSystemMessage( strMessage );							
						}
						break;
					}					
				}
				break;
			}
		}
		break;
		
	case MSG_EVENT_TREASUREBOX:
		{
			unsigned char ucSubType;
			(*istr) >> ucSubType;
			switch( ucSubType )
			{
			case MSG_EVENT_TREASUREBOX_TRY_REP:
				{
					int iTreasureBoxLevel	= -1;
					CTString strMessage;
					LONG lTreasureBoxType;
					(*istr) >> lTreasureBoxType;

					switch( lTreasureBoxType )
					{
					case MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL:
						{
							strMessage = _S( 1757, "레벨이 부족하여 보물 상자를 열수 없습니다." );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EVENT_TREASUREBOX_ERROR_NO_BOX:
						{
							strMessage = _S( 1758, "보물 상자가 없습니다." );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EVENT_TREASUREBOX_10LEVEL:
						{
							iTreasureBoxLevel	= 10;
						}
						break;
					case MSG_EVENT_TREASUREBOX_14LEVEL:
						{
							iTreasureBoxLevel	= 14;
						}
						break;
					case MSG_EVENT_TREASUREBOX_18LEVEL:
						{
							iTreasureBoxLevel	= 18;
						}
						break;
					case MSG_EVENT_TREASUREBOX_22LEVEL:
						{
							iTreasureBoxLevel	= 22;
						}
						break;
					case MSG_EVENT_TREASUREBOX_26LEVEL:
						{
							iTreasureBoxLevel	= 26;
						}
						break;
					case MSG_EVENT_TREASUREBOX_30LEVEL:
						{
							iTreasureBoxLevel	= 30;
						}
						break;
					}
					
					CTString	strItemName[TREASUREBOX_ITEM_COUNT];
					LONG		lItemCount[TREASUREBOX_ITEM_COUNT];

					ASSERT( iTreasureBoxLevel != -1 && "Invalid Treasure Box Level!" );
					for( int i = 0; i < TREASUREBOX_ITEM_COUNT ; ++i )
					{
						SBYTE sbTreasureItem;						
						(*istr) >> sbTreasureItem;
						(*istr) >> lItemCount[i];

						switch( sbTreasureItem )
						{
						case MSG_EVENT_CHANGE_13LEVEL_WEAPON:		//+4 13레벨 무기
							strItemName[i] = _S( 1759, "+4 13레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_21LEVEL_WEAPON:		//+4 21레벨 무기
							strItemName[i] = _S( 1760, "+4 21레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_29LEVEL_WEAPON:		//+4 29레벨 무기
							strItemName[i] = _S( 1761, "+4 29레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_NAS:					//나스
							strItemName[i] =_S( 1762,  "나스" );	
							break;
						case MSG_EVENT_CHANGE_CANDY:				//사탕
							strItemName[i] = _S( 1763, "사탕" );	
							break;
						case MSG_EVENT_CHANGE_ATTACK_POTION:		//공격력향상물약
							strItemName[i] = _S( 1764, "공격력 향상 물약" );	
							break;
						case MSG_EVENT_CHANGE_DEFENSE_POTION:		//방어력향상물약
							strItemName[i] = _S( 1765, "방어력 향상 물약" );	
							break;
						case MSG_EVENT_CHANGE_NO_TREASUREBOX:		//보물상자가 없음
							break;
						}
					}
					_pUIMgr->CloseMessageBoxL( MSGLCMD_TREASUREBOX_EVENT );
					
					_pUIMgr->CreateMessageBoxL( _S( 1766, "보물 상자" ), UI_QUEST, MSGLCMD_TREASUREBOX_EVENT );			
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, _S( 1766, "보물 상자" ), -1, 0xE18600FF );	
					strMessage.PrintF( _S( 1767, "%d Lv 보물상자를 열면 아래 아이템 항목중 1가지가 지급됩니다.  %d Lv 보물상자를 열어보시겠습니까?" ), iTreasureBoxLevel, iTreasureBoxLevel );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, CTString("  "), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, _S( 1768, "아이템 지급 품목" ), -1, 0xE18600FF );			

					for( i = 0; i < TREASUREBOX_ITEM_COUNT; ++i )
					{
						strMessage.PrintF( _S( 61, "%s %d개" ), strItemName[i], lItemCount[i] );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );			
					}					
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, FALSE, _S( 1769, "보물상자를 연다." ), 1 );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, FALSE, _S( 1220, "취소한다." ) );	
				}
				break;
			case MSG_EVENT_TREASUREBOX_OPEN_REP:
				{
					LONG lTreasureItem;
					(*istr) >> lTreasureItem;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_SUPERSTONE:
		{
			unsigned char ucSubType;
			(*istr) >> ucSubType;
			switch(ucSubType)
			{
			case MSG_EVENT_SUPERSTONE_REP:
				{
				} break;
			}
		} break;

	case MSG_EVENT_CHANGE_WITHOUT_OPTION:
		{			
			// MSG_EVENT_CHANGE_WITHOUT_OPTION,			// 2판4판 리뉴얼 무기 교체 이벤트 : old_itemindex(n) new_subtype(n)	
			LONG lOldItemIndex;
			LONG lNewSubType;
			(*istr) >> lOldItemIndex;
			(*istr) >> lNewSubType;
			_pUIMgr->GetChangeWeapon()->CloseChangeWeapon();
		}
		break;

	case MSG_EVENT_2PAN4PAN_GOODS_CHECK:
		{
			ULONG count;
			CTString strMessage;
			(*istr) >> count;
			if(count>0){
				strMessage.PrintF( _S(2232, "보상품을 요청 중입니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				
				_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_GOODS_REQ);
				
			} else {				
				strMessage.PrintF( _S(2233, "요청한 보상품이 존재하지 않습니다" ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}
			
		}
		break;
		
	case MSG_EVENT_2PAN4PAN_GOODS_REP:
		{
			ULONG errType,count;
			CTString strMessage;
			(*istr) >> errType;
			(*istr) >> count;

			switch(errType)
			{
				case MSG_EVENT_2PAN4PAN_GOODS_ERROR_OK :
						strMessage.PrintF( _S(2234, "이벤트 아이템을 지급 받았습니다." ));
						_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
						break;
				case MSG_EVENT_2PAN4PAN_GOODS_ERROR_SOMEREMAIN:
					strMessage.PrintF( _S(2235, "이벤트 아이템을 일부 지급 받았습니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
				case MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOGOODS:
					strMessage.PrintF( _S(2236, "이벤트 아이템이 존재하지 않습니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
				case MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOSPACE:
					strMessage.PrintF( _S(2237, "지급받을 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
			}
		}
		break;
	case MSG_EVENT_2PAN4PAN_MONEY_REP:
		{
			ULONG success;
			CTString strMessage;
			(*istr) >> success;

			if(success==1 ){ //1 맞고 
				strMessage.PrintF( _S(2238, "맞고머니를 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				
			
			} 
			else if(success==2 ){ //2 포커 
				strMessage.PrintF( _S(2239, "포커머니를 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				
						
			} 
			
			else {				
				strMessage.PrintF( _S(2240, "이벤트 아이템이 존재하지 않습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}

		}
		break;
	case MSG_EVENT_2PAN4PAN_BOX_CHECK:
		{
			ULONG flag;
			CTString strMessage;
			(*istr) >> flag;
			if(flag>0){
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_2PAN4PAN_TWO );
				_pUIMgr->CreateMessageBoxL( _S( 1875, "이벤트 선택창" ),UI_QUEST, MSGLCMD_EVENT_2PAN4PAN_TWO);						
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, 
											_S(2241,"2pan4pan 보물상자를 열어보면 아래의 3가지 물품 중 한가지가 지급됩니다."), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, CTString("\n\n"), -1,0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2242, "지급 물품" ), -1, 0xFFA41CFF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2243, "부스터 5개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2244, "문스톤 5개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2245, "2pan4pan 물약 10개" ), -1, 0xA3A1A3FF );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, FALSE, _S(2246,"2pan4pan 보물상자 열기"),OPEN_TREA);
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, FALSE,_S( 1220, "취소한다." ));

			} else {				
				strMessage.PrintF( _S(2247, "보물상자가 존재하지 않습니다" ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}

		}
		break;
	case MSG_EVENT_2PAN4PAN_BOX_REP:
		{
			ULONG errType,count;
			CTString strMessage;
			(*istr) >> errType;
			(*istr) >> count;

			switch(errType)
			{
				case MSG_EVENT_2PAN4PAN_BOX_ERROR_OK :
					strMessage.PrintF( _S(2234, "이벤트 아이템을 지급 받았습니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
				case MSG_EVENT_2PAN4PAN_BOX_ERROR_SOMEREMAIN:
					strMessage.PrintF( _S(2235, "이벤트 아이템을 일부 지급 받았습니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
				case MSG_EVENT_2PAN4PAN_BOX_ERROR_NOBOX:
					strMessage.PrintF( _S(2236, "이벤트 아이템이 존재하지 않습니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
				case MSG_EVENT_2PAN4PAN_BOX_ERROR_NOSPACE:
					strMessage.PrintF( _S(2237, "지급받을 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
					break;
			}
				
		}
		break;
	case MSG_EVENT_XMAS_2005 :
		LONG errType1;
		(*istr)>>errType1;
		if(errType1==MSG_EVENT_XMAS_2005_ERROR_OK){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2282,"축하합니다. 2005 크리스마스 이벤트 보상품을 획득 하셨습니다.") ; // 번역 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		else if(errType1==MSG_EVENT_XMAS_2005_ERROR_NO_ITEM){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2283,"조합할 꿈과 희망 아이템이 부족합니다.") ; //번역 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

		}
		break;

	case MSG_EVENT_NEWYEAR_2006_CHECK:
		{
			LONG tv_time,hour,min;
			(*istr)>>tv_time;
			hour=tv_time/3600;
			min=(tv_time%3600)/60;
			
				
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2304,"현재 접속 시간은 %d시간 %d분 입니다."),hour,min) ; 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 2287, "2006 인내의 신년 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}		
		break;


	case MSG_EVENT_NEWYEAR_2006_GOODS:
		LONG errType2;
		(*istr)>>errType2;
		if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_OK){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2284,"축하합니다. 2006 신년 이벤트 보상품을 획득 하셨습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TIME){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2285,"이벤트 시간이 부족 합니다.(10시간 이상)") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );


		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_END){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2285,"인내의 열매 이벤트 보상품은 2006 신년 이벤트가 종료 되어야 지급 받으실 수 있습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_ALREADY){
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(673,"이벤트 아이템이 이미 지급 되었습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

		}
		
		break;

	case MSG_EVENT_SEARCHFRIEND_ADD_CHECK :
	case MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK :
		{
			ULONG errType;
			(*istr) >> errType;
			FindFriendErrChk1(errType);
		}
		break;
		
	case MSG_EVENT_SEARCHFRIEND_ADDSELECT_LIST :
		{
			ULONG listCnt,startIdx,listAll;
			SEL_LIST tv_char;
			LONG tv_tmp;
					
			(*istr) >> listCnt;
			(*istr) >> startIdx;
			(*istr) >> listAll;

			if( startIdx==0) 
				_pUIMgr->GetSelectList()->ResetSelectList();

			tv_tmp = startIdx+listCnt;
			for( int i=startIdx; i<tv_tmp; i++){
				(*istr) >> tv_char.m_nIndex;
				(*istr) >> tv_char.m_sName;
		
				tv_char.m_nState = LS_WAIT ;

				_pUIMgr->GetSelectList()->GetList()->push_back(tv_char);
			}
			if(tv_tmp>=listAll)
				_pUIMgr->GetSelectList()->CreateSelectList(SLS_REGIST|SLS_CANCEL|SLS_PREV|SLS_NEXT ,10, _S( 2503, "나를 찾기 원하는 친구"));
		}		
		break;
	case MSG_EVENT_SEARCHFRIEND_TIME_CHECK :
		{
			CTString strMessage;
			ULONG tv_time;
			(*istr) >> tv_time;
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			strMessage.PrintF(_S( 2504, "현재 %d시간 %d분 사냥하였습니다."),tv_time/3600,(tv_time%3600)/60 );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 2433, "친구 찾기 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_ONETIME_CHECK :
		{
			CTString strMessage;
			ULONG tv_time;
			int tv_sum;
			(*istr) >> tv_time;
			tv_sum = tv_time/3600;
			strMessage.PrintF(_S( 2505, "현재 %d시간 사냥하였습니다."),tv_sum);
			_pNetwork->ClientSystemMessage(strMessage);
			if(tv_sum == 30)  
			{
				strMessage.PrintF(_S( 2506, "이제 랜디에게서 보상품을 지급 받을 수 있습니다."));
				_pNetwork->ClientSystemMessage(strMessage);
			}
			if(tv_sum == 60)  
			{
				strMessage.PrintF(_S( 2507, "경험치 두배 이벤트가 종료 되었습니다."));
				_pNetwork->ClientSystemMessage(strMessage);
			}
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_LIST_GOODS :
		{
			ULONG listCnt,startIdx,listAll;
			SEL_LIST tv_char;
			LONG tv_tmp,listState;
					
			(*istr) >> listCnt;
			(*istr) >> startIdx;
			(*istr) >> listAll;

			if( startIdx==0) 
				_pUIMgr->GetSelectList()->ResetSelectList();

			if(listAll <= 0){
				_pUIMgr->CloseMessageBox( MSGCMD_NULL );
				CTString strSysMessage = _S( 2493, "등록된 친구가 없습니다.") ;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S( 2433, "친구 찾기 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				break;
			}
			tv_tmp = startIdx+listCnt;
			for( int i=startIdx; i<tv_tmp; i++){
				(*istr) >> tv_char.m_nIndex;
				(*istr) >> tv_char.m_sName;
			//	(*istr) >> requestTime;
				(*istr) >> listState;
				if(listState < 0) tv_char.m_nState = LS_WAIT;
				else if(listState == 0) tv_char.m_nState = LS_REWARDX;
				else if(listState == 3) tv_char.m_nState = LS_RECEIVED;
				else if(listState > 0 ) tv_char.m_nState = LS_REWARD;

				_pUIMgr->GetSelectList()->GetList()->push_back(tv_char);
			}
			if(tv_tmp == listAll)
				_pUIMgr->GetSelectList()->CreateSelectList(SLS_REWARD|SLS_PREV|SLS_NEXT|SLS_CANCEL,10,_S( 2503, "내가 찾기 원하는 친구"));
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_CHECK :
		{
			ULONG errType;
			(*istr) >> errType;
			FindFriendErrChk2(errType);
		}
		break;
	case MSG_EVENT_OXQUIZ:
		{
			ULONG QuizType;
			ULONG QuizNo;
			ULONG Limitsec;
			CTString strQuizMessage;
			UBYTE isAnswer;

			(*istr) >> QuizType;
			
			if( QuizType == MSG_EVENT_OXQUIZ_INVALID_TIME )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_NULL );
				CTString strSysMessage = _S(2626, "O.X 이벤트 입장 가능 시간이 아닙니다.") ;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S(2627, "O.X 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( strSysMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				break;				
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_START )
			{
				CTString strStart = _S(2628, "O.X 퀴즈를 시작 합니다." );
				_pNetwork->ClientSystemMessage( strStart );
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_QUIZ )
			{
				(*istr) >> QuizNo;
				(*istr) >> Limitsec;
				(*istr) >> strQuizMessage;

				_pUIMgr->GetOXQuizEvent()->ReceiveQuiz( QuizNo, Limitsec, strQuizMessage );
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_ANSWER )
			{

				(*istr) >> QuizNo;
				(*istr) >> isAnswer;

				CTString tv_str;
				tv_str.PrintF(_S(2629,"%d번 문제의 정답은"), QuizNo );

				if( isAnswer == 1 )
				{
					tv_str += _S(2630, "O 였습니다." );	
				}
				else
				{
					tv_str += _S(2631, "X 였습니다." );	
				}

				_pNetwork->ClientSystemMessage( tv_str );
			}
		}
		break;
	case MSG_EVENT_RECOMMEND :
		{
			ULONG subType;
			(*istr) >> subType;
		
			switch(subType){
			case MSG_EVENT_RECOMMEND_SET :
				
				break;
			case MSG_EVENT_RECOMMEND_POTION :
			
				break;
			case MSG_EVENT_RECOMMEND_MOONSTONE :
				ULONG tv_idx;
				CTString tv_str,tv_str2;
				(*istr) >> tv_idx;
				(*istr) >> tv_str;
				tv_str2.PrintF(_S(2632,"%s님께서 추천서버 이벤트에 당첨 되셨습니다."),tv_str.str_String);
				PCStartEffectGroup("server_event_using", tv_idx);
				_UIAutoHelp->SetGMNotice ( tv_str2, 0xFFAA33FF );
				break;
				
			}
		}
		break;
	case MSG_EVENT_PCBANG:	// NetCafe 보물상자 지급, (태국, 일본)NetCafe 캠페인 이벤트
		{
			ULONG ItemIndex;
			(*istr) >> ItemIndex;

			if( ItemIndex != -1 )
			{
				if( ItemIndex == 1394 || ItemIndex == 1395 || ItemIndex == 1396 || ItemIndex == 1397 )
				{// 보물 상자를 받을 때만
					_pUIMgr->GetNotice()->AddToNoticeList( 4002, NOTICE_EVENT );
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
				else
				{
					_pUIMgr->CloseMessageBox( MSGCMD_NULL );
					CTString strSysMessage = _S( 1840, "아이템이 성공적으로 지급 되었습니다." );
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(   _S( 169, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strSysMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
			}
			else
			{
				_pNetwork->ClientSystemMessage( _S( 1758, "보물 상자가 없습니다." ) );
			}
		}
		break;
	case MSG_EVENT_WORLDCUP :
		{
			ULONG subType,nIdx,errCode;
			(*istr) >> subType;
			(*istr) >> nIdx;
			(*istr) >> errCode;

			WorldCupErrChk( errCode) ;

		}
		break;
	case MSG_EVENT_GOLDENBALL:
		{
			ULONG subType, errType;
			CTString strTeamA, strTeamB, strMessage;
			ULONG ulTeamAScore, ulTeamBScore;
			ULONG ulYear, ulMonth, ulDay, ulHour, ulMinute;
			SYSTEMTIME GoldenTime;

			ZeroMemory( &GoldenTime, sizeof( GoldenTime ) );

			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >>subType;

			switch( subType )
			{
			case MSG_EVENT_GOLDENBALL_VOTE_START:
				{
					(*istr) >> strTeamA;
					(*istr) >> strTeamB;
					(*istr) >> ulYear >> ulMonth >> ulDay >> ulHour >> ulMinute;

					GoldenTime.wYear = static_cast<WORD>(ulYear);
					GoldenTime.wMonth = static_cast<WORD>(ulMonth);
					GoldenTime.wDay = static_cast<WORD>(ulDay);
					GoldenTime.wHour = static_cast<WORD>(ulHour);
					GoldenTime.wMinute = static_cast<WORD>(ulMinute);

					_pUIMgr->GetQuest()->SetGoldenBallEntry( TRUE );
					_pUIMgr->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, TRUE );
					_pUIMgr->GetNotice()->AddToNoticeList( 2000+100, NOTICE_EVENT );
					_pUIMgr->GetNotice()->RefreshNoticeList();
				}
				break;
			case MSG_EVENT_GOLDENBALL_VOTE_END:
				{
					(*istr) >> strTeamA;
					(*istr) >> strTeamB;				

					_pUIMgr->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, FALSE );
					_pUIMgr->GetQuest()->SetGoldenBallEntry( FALSE );
					_pNetwork->ClientSystemMessage( _S( 2844, "골든 볼 이벤트의 응모를 마감합니다." ) );
				}
				break;
			case MSG_EVENT_GOLDENBALL_GIFT_START:
				{
					(*istr) >> strTeamA;
					(*istr) >> ulTeamAScore;
					(*istr) >> strTeamB;
					(*istr) >> ulTeamBScore;
					(*istr) >> ulYear >> ulMonth >> ulDay >> ulHour >> ulMinute;

					GoldenTime.wYear = static_cast<WORD>(ulYear);
					GoldenTime.wMonth = static_cast<WORD>(ulMonth);
					GoldenTime.wDay = static_cast<WORD>(ulDay);
					GoldenTime.wHour = static_cast<WORD>(ulHour);
					GoldenTime.wMinute = static_cast<WORD>(ulMinute);

					_pUIMgr->GetQuest()->SetGoldenBallRequital( TRUE );
					_pUIMgr->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, TRUE );
					_pUIMgr->GetQuest()->SetTeamScore( static_cast<int>( ulTeamAScore ), static_cast<int>( ulTeamBScore ) );
					_pUIMgr->GetNotice()->AddToNoticeList( 2000+101, NOTICE_EVENT );
					_pUIMgr->GetNotice()->RefreshNoticeList();

					_pUIMgr->GetQuest()->SetGoldenBallRequital( TRUE );
			
				}	
				break;
			case MSG_EVENT_GOLDENBALL_GIFT_END:
				{
					_pUIMgr->GetQuest()->SetGoldenBallRequital( FALSE );
					_pNetwork->ClientSystemMessage( _S( 2845, "골든 볼 이벤트의 보상시간이 끝났습니다." ), SYSMSG_NOTIFY );
				}
				break;
			case MSG_EVENT_GOLDENBALL_VOTE:
				{
					(*istr) >> ulTeamAScore;
					(*istr) >> ulTeamBScore;

					_pUIMgr->GetQuest()->SetTeamScore( static_cast<int>( ulTeamAScore ), static_cast<int>( ulTeamBScore ) );

					(*istr) >> errType;

					switch( errType )
					{
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_OK:
						{
							strMessage = _S( 2846, "골든볼 이벤트에 응모되었습니다" );
							_pNetwork->ClientSystemMessage( _S( 2847, "골든 볼 이벤트 보상이 시작되면 랜디를 다시 찾아오셔서 보상을 받으시기 바랍니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_ALREADY:
						{
							strMessage = _S( 2848, "골든 볼 이벤트에 이미 응모 하셨습니다." );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM:
						{
							strMessage = _S( 2849, "이벤트 응모를 위해서는 팀가이스트 축구공 10개가 필요합니다.\n확인 후 다시 시도해 주세요" );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_FULLINVEN:
						{
							strMessage = _S( 2850, "인벤토리가 가득 찼습니다." );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_TIME:
						{
							strMessage = _S( 2851, "골든 볼 이벤트 응모 시간이 아닙니다." );
						}
						break;
					}

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case MSG_EVENT_GOLDENBALL_GIFT:
				{
					(*istr) >> errType;

					switch( errType )
					{
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_EXACT:
						{
							_pNetwork->ClientSystemMessage( _S( 2852, "골든 볼 이벤트 승리국 및 점수를 맞추어 보상을 획득 하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_WINNER:
						{
							_pNetwork->ClientSystemMessage( _S( 2853, "골든 볼 이벤트 승리국 적중하여 보상을 획득하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_NONE:
						{
							_pNetwork->ClientSystemMessage( _S( 2854, "골든 볼 이벤트 참가상 획득하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_FULLINVEN:
						{
							_pNetwork->ClientSystemMessage( _S( 2855, "인벤토리가 가득 찼습니다." ), SYSMSG_ERROR );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM:
						{
							_pNetwork->ClientSystemMessage( _S( 2856, "골든 볼 이벤트 응모 카드가 없습니다." ), SYSMSG_ERROR );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_TIME:
						{
							_pNetwork->ClientSystemMessage( _S( 2857, "골든 볼 이벤트 보상 기간이 아닙니다." ), SYSMSG_ERROR );
						}
						break;
					}
				}
				break;
			}
			break;
		}
		break;
	case MSG_EVENT_RAIN_2006 : 
		{
			ULONG errType;
			(*istr) >> errType;
			CTString tv_str;
			switch(errType)
			{
				case MSG_EVENT_RAIN_2006_ERROR_OK :				// 보상 성공
					tv_str=_S( 159, "보상이 성공적으로 지급되었습니다.");
					break;
				case MSG_EVENT_RAIN_2006_ERROR_NOITEM :			// 빗방울/빗물병 없음
					tv_str=_S(2767, "빗방울이 부족하여 보상을 받을 수 없습니다. 확인 후 다시 시도하여 주세요.");
					break;
				case MSG_EVENT_RAIN_2006_ERROR_FULLINVEN :		// 인벤토리 가득참
					tv_str=_S( 2768,"인벤토리가 가득차서 보상을 받을 수 없습니다.");
					break;
			}
			_pNetwork->ClientSystemMessage(tv_str);
		}
		break;
	case MSG_EVENT_TLD_BUDDHIST:
		{
			BYTE ItemType;
			ULONG errType;
			(*istr) >> ItemType;
			(*istr) >> errType;

			switch( errType )
			{
			case MSG_EVENT_TLD_BUDDHIST_ERROR_OK:
				_pNetwork->ClientSystemMessage( _S( 2926, "성공적으로 보상품을 수령하였습니다." ), SYSMSG_NOTIFY );
				break;
			case MSG_EVENT_TLD_BUDDHIST_ERROR_FULLINVEN:
				_pNetwork->ClientSystemMessage( _S( 2768, "인벤토리가 가득차서 보상을 받을 수 없습니다."), SYSMSG_ERROR );
				break;
			case MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM:
				_pNetwork->ClientSystemMessage( _S( 2927, "모래 봉투가 부족하여 보상을 받을 수 없습니다. 확인 후 다시 시도하여 주세요." ), SYSMSG_ERROR );
				break;
			}
		}
		break;
	case MSG_EVENT_COLLECT_BUG:
		{
			ULONG subType;
			ULONG ulTemp;
			CTString strMessage;
			
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> subType;

			switch( subType )
			{
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_OK:
				{
					(*istr) >> ulTemp;
					if( ulTemp == 1 )
					{
						strMessage = _S( 2972, "축하합니다! 30포인트가 추가된 골든 곤충 채집상자를 구입하셨습니다" );
					}
					else if( ulTemp == 0 )
					{ 
						strMessage = _S( 2973, "곤충 채집상자 1개를 구입하였습니다." );
					}
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_DUPLICATE:
				{
					_pNetwork->ClientSystemMessage( _S( 2974, "이미 곤충 채집상자를 가지고 있어 더 이상 구입할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage( _S( 2855, "인벤토리가 가득 찼습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_MONEY:
				{
					_pNetwork->ClientSystemMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_OK:
				{
					(*istr) >> ulTemp;
					
					strMessage = _S( 2975, "곤충 채집 이벤트 보상이 완료 되었습니다." );
					if ( _pUIMgr->GetCollectBox()->IsVisible() )
						_pUIMgr->RearrangeOrder(UI_COLLECTBOX, FALSE);
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage( _S( 116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOITEM:
				{
					_pNetwork->ClientSystemMessage( _S( 2976, "곤충 채집 상자를 가지고 있지 않습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOTFULL:
				{
					_pNetwork->ClientSystemMessage( _S( 2977, "곤충 채집상자가 전부 채워지지 않아 보상 받을 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_OK:
				{
					(*istr) >> ulTemp;

					strMessage = _pUIMgr->GetCollectBox()->GetInsectName( ulTemp );
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_NOTIFY );					
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_NOITEM:
				{
					_pNetwork->ClientSystemMessage( _S( 2978, "곤충 채집 상자를 가지고 있지 않습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_FULL:
				{
					_pNetwork->ClientSystemMessage( _S( 2979, "채집상자가 가득 찼습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_DROP:
				{// 메세지 처리 안함 (eons)
					return;
				}
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
		
	case MSG_EVENT_CHUSEOK_2006:
		{
			ULONG subType;
			CTString strMessage;

			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> subType;
			switch(subType)
			{
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_OK:
				strMessage.PrintF( _S(3138, "송편 제작에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FAIL:
				strMessage.PrintF( _S(3139, "송편 제작에 실패하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM:
				strMessage.PrintF( _S(3140, "송편 재료가 부족합니다." ) );
				break;
			
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_OK:			// 오색 송편 만들기 성공
				strMessage.PrintF( _S(3141, "오색송편 제작에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FAIL:			// 오색 송편 만들기 실패
				strMessage.PrintF( _S(3142, "오색송편 제작에 실패하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM:		// 오색 송편 재료 부족
				strMessage.PrintF( _S(3143, "오색송편 재료가 부족합니다." ) );
				break;

			case MSG_EVENT_CHUSEOK_2006_GIFT_OK:							// 오색 송편 교환 성공
				strMessage.PrintF( _S( 1884, "추석 이벤트 보상품이 지급되었습니다. 이벤트에 참여해 주셔서 감사합니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM:					// 오색 송편 부족
				strMessage.PrintF( _S(3144, "오색송편이 없어 보상을 받을 수 없습니다." ) );	
				break;

			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FULLINVEN:
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FULLINVEN:		// 인벤토리 부족
				strMessage.PrintF( _S(265, "인벤토리 공간이 부족합니다." ) );
				break;
			case MSG_EVENT_CHUSEOK_2006_GIFT_OK_FULLINVEN:				// 인벤토리 부족
				strMessage.PrintF( _S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다." ) );
				break;
			}
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_XMAS_2006:		// 2006 X-Mas event [12/12/2006 Theodoric]
		{
			SLONG nCakeCount;
			ULONG subType;
			
			CTString strMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> nCakeCount;
			(*istr) >> subType;	
			
			switch(subType)
			{
			case MSG_EVENT_XMAS_2006_ERROR_OK:	// 성공
				strMessage.PrintF( _S(159, "교환이 성공적으로 이루어졌습니다." ) );
				break;
			case MSG_EVENT_XMAS_2006_ERROR_FULLINVEN:	// 인벤 가득 참
				strMessage.PrintF( _S(265, "인벤토리 공간이 부족합니다." ) );
				break;
			case MSG_EVENT_XMAS_2006_ERROR_NOITEM:	// 아이템 없음
				strMessage.PrintF( _S(2908, "교환할 아이템이 없습니다." ) );
				break;
			}
			
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL, MSGBOX_WIDTH + 20 );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_OPEN_BETA_GIFT:
		{
			CUIMsgBox_Info	MsgBoxInfo;
			CTString strMessage;

			strMessage.PrintF(_S(1840, "아이템이 성공적으로 지급 되었습니다."));
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_VALENTINE_2007: // 러브러브 이벤트
		{
			_pUIMgr->GetBingoBox()->ErrorMessage(&(*istr));
		}
		break;

	// wooss 070307 ---------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007 
	case MSG_EVENT_WHITEDAY_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if( Whiteday2007ErrChk(subType) )
			{
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString tMsg,tStr;
				
				UBYTE exType;
				(*istr) >> exType;
				switch(exType)
				{
				case 0:
					{
						ULONG iIdx;
						(*istr) >> iIdx;						
						tStr = CTString(_pNetwork->ga_World.wo_aSkillData[iIdx].GetName());
						tMsg.PrintF(_S(3261, "로레인으로부터 %s 주문을 받았습니다."), tStr.str_String );
						MsgBoxInfo.AddString( tMsg );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					break;
				case 1:
					{
						tMsg = _S(3262,"로레인으로부터 하늘색 편지지 1장을 받았습니다.");
						MsgBoxInfo.AddString( tMsg );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );					
					}					
					break;
				case 2:
					{
						tMsg =_S(3263,"로레인으로부터 분홍색 편지지 1장을 받았습니다.");
						MsgBoxInfo.AddString( tMsg );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );					
					}
					break;
				}
			}
			
			if( subType ==  MSG_EVENT_WHITEDAY_2007_LETTER_REP )
			{
				COLOR tCol;
				CTString tStr;
				(*istr) >> tCol;
				(*istr) >> tStr;
				_pUIMgr->GetChatting()->AddSysMessage( tStr, SYSMSG_USER,tCol );
			}				
		}
		break;
// [KH_070417] 스승의날 이벤트 관련 추가
	case MSG_EVENT_TEACH_2007:
		ReceiveEventMaster(istr);
		break;
	// ------------------------------------------------------------------<<
	case MSG_EVENT_CHILDRENSDAY_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if(subType == MSG_EVENT_CHILDRENSDAY_2007_REP)
			{				
				UBYTE exType;
				(*istr) >> exType;

				CTString tMsg;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			
				switch(exType)
				{
				case 0:
					{
						ULONG iItemidx;
						(*istr) >> iItemidx;
						CItemData &ID = _pNetwork->GetItemData(iItemidx);
						tMsg.PrintF(_S(3405, "%s 아이템을 획득 하였습니다."),ID.GetName());
						MsgBoxInfo.AddString(tMsg);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
						
					}break;
				case 1:
					{
						tMsg = _S(3406, "보유하고 있는 노란 개나리 꽃의 개수가 부족합니다. 확인하고 다시 시도하여 주십시오");
						MsgBoxInfo.AddString(tMsg);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );

					}break;
				case 2:
					{
						tMsg = _S(3407, "한번 받은 장비는 두 번 받을 수 없습니다. 확인하고 다시 시도 하여 주십시오");
						MsgBoxInfo.AddString(tMsg);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}break;
				default:
					{
						tMsg.PrintF(_S(1844,"지급된 아이템을 정상적으로 처리하지 못하였습니다. 다시 시도하여 주십시오."));
						MsgBoxInfo.AddString(tMsg);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}break;

				}

			}
		}break;
// [KH_070326] 부활절 이벤트 관련
	case MSG_EVENT_EGGS_HUNT_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if( EasterEggs2007ErrChk(subType) )
			{
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString tMsg,tStr;
				
				INDEX exItemType;	// [KH_070329] 아이템 서버 인덱스
				INDEX exItemNum;	// [KH_070329] 아이템 갯수
				(*istr) >> exItemType;
				(*istr) >> exItemNum;

				if(exItemNum != 1) // [KH_070329] 한개가 아니면 숫자 표시
					tStr.PrintF("%d", exItemNum);
				else // [KH_070329] 하나가 아니면 표시 안함
				{
					tStr = CTString("");
				}

				tMsg.PrintF( _S( 3266, "부활절 달걀 보상품 %s%s를 획득하였습니다."), tStr, _pNetwork->GetItemName( exItemType ) );
						
				MsgBoxInfo.AddString( tMsg );
				
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;

		// ------------------------------------------------------------------<<
	// WSS_GUILDMASTER -------------------------------------------------->>
	case MSG_EVENT_PARENTSDAY_2007:
		{
			UBYTE subType;
			SBYTE errCode;  
			(*istr) >> subType;
			switch(subType)
			{
					case MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST:
						{
							(*istr)>>errCode;
							if(GuildMasterErrChk(errCode))
							{						
								ULONG tCnt;
								ULONG tRank,tPoint;
								CTString gName,strRank,strPoint;
								CUIListBox* tBox = &_pUIMgr->GetMessageBox(MSGCMD_GUILDMASTER_1)->GetListBox();

								// Init string
								ASSERT(tBox!=NULL);
								tBox->ResetAllStrings();
								tBox->AddString( 0, _S(1714, "순위"),0xFFF244FF, FALSE );
								tBox->AddString( 1, _S(3450, "포인트"),0xFFF244FF, FALSE );
								tBox->AddString( 2, _S(3451, "길드명"),0xFFF244FF, FALSE );	
																
								(*istr) >> tCnt;
								for(int i=0; i<tCnt;i++)
								{
									(*istr) >> tRank >> tPoint >> gName;
									strRank.PrintF("%d",tRank);
									strPoint.PrintF("%d",tPoint);
									tBox->AddString(0,strRank,0xF2F2F2FF,FALSE);
									tBox->AddString(1,strPoint,0xF2F2F2FF,FALSE);
									tBox->AddString(2,gName,0xF2F2F2FF,FALSE);							
								}									
							}						
						}	
						break;
					case MSG_EVENT_PARENTSDAY_2007_ADD_POINT:			// s->c : errorcode(c) point(n) totalpoint(n)
						{
							(*istr)>>errCode;
							if( GuildMasterErrChk(errCode))
							{
								CTString tMsg;
								CUIMsgBox_Info	MsgBoxInfo;
								ULONG tInc,tPnt;
								(*istr) >> tInc >> tPnt;
								MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
								tMsg.PrintF(_S(3476,"길드의 포인트가 %d만큼 증가하여 %d점이 되었습니다."),tInc,tPnt);
								MsgBoxInfo.AddString(tMsg);
								_pUIMgr->CreateMessageBox( MsgBoxInfo );										
							}
						}
						break;
					case MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET:		// s->c : errorcode(c) changeTicketCount(n)
						{
							(*istr)>>errCode;
							if(GuildMasterErrChk(errCode))
							{
								CTString tMsg;
								CUIMsgBox_Info	MsgBoxInfo;
								ULONG tCnt;
								(*istr) >> tCnt;
								MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
								tMsg.PrintF(_S(3477,"누적된 교환권 %d장을 지급 받았습니다."),tCnt);
								MsgBoxInfo.AddString(tMsg);
								_pUIMgr->CreateMessageBox( MsgBoxInfo );										
							}
						}
						break;
					case MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM:		// s->c: errcode(c)					
						(*istr)>>errCode;
						GuildMasterErrChk(errCode);
						break;
					case MSG_EVENT_PARENTSDAY_2007_NOTICE:
						{
							CTString strMessage,tGuild,tItem;
							(*istr)>>tGuild>>tItem;
							strMessage.PrintF(_S(3478,"[%s]길드의 길드 마스터가 [%s]아이템에 당첨 되었습니다."),tGuild,tItem);
							_UIAutoHelp->SetGMNotice ( strMessage );
						}
						break;
			}
		}
		break;
	// ------------------------------------------------------------------<<
	
	// WSS_MINIGAME 070420 ---------------------------------------------->>
	case MSG_EVENT_GOMDORI_2007 :
		{
			UBYTE subType;
			SBYTE wincount; 
			SBYTE usersel;  
			SBYTE npcsel;  
			CUIMsgBox_Info msgInfo;
			CTString strMessage=_S(191,"확인");
			CUIMinigame* pMinigame=((CUIMinigame*)_pUIMgr->GetUI(UI_MINIGAME));
			(*istr) >> subType;
			switch(subType)
			{
					case MSG_EVENT_GOMDORI_2007_CHECKITEM:
						// 에코시 버튼을 풀어 준다.
						pMinigame->OpenMinigame();
						break;
					case MSG_EVENT_GOMDORI_2007_START:
						// 에코시 버튼을 풀어 준다.
						pMinigame->SetButtonState(BTN_STATE_START_CONTINUE);
						break;
					case MSG_EVENT_GOMDORI_2007_CONTINUE:
						// 에코시 버튼을 풀어 준다.
						pMinigame->SetButtonState(BTN_STATE_START_CONTINUE);
						break;

					case MSG_EVENT_GOMDORI_2007_RESULT_WIN:
						// TODO : 이겼을 경우 처리(UIMinigame내 함수 호출)
						(*istr)>>wincount>>usersel>>npcsel;
						pMinigame->SetMinigameResult(wincount,usersel,npcsel,2);						
						break;
					case MSG_EVENT_GOMDORI_2007_RESULT_LOSE:
						// TODO : 졌을 경우 처리(UIMinigame내 함수 호출)
						(*istr)>>wincount>>usersel>>npcsel;
						pMinigame->SetMinigameResult(wincount,usersel,npcsel,0);									
						break;
					case MSG_EVENT_GOMDORI_2007_RESULT_DRAW:	
						// TODO : 비겼을 경우 처리(UIMinigame내 함수 호출)
						(*istr)>>wincount>>usersel>>npcsel;						
						pMinigame->SetMinigameResult(wincount,usersel,npcsel,1);						
						break;						
					case MSG_EVENT_GOMDORI_2007_END:
						{
							UBYTE wincount,prompttype;
							ULONG extra1,extra2;							
							
							(*istr)>>wincount>>prompttype>>extra1>>extra2;
							if(prompttype== 0)
							{							
								msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
							//	strMessage.PrintF( _s( "보상아이템[%s] %d개가 지급 되었습니다." ), _pNetwork->GetItemName(extra1), extra2);		
								strMessage.PrintF( _S(1840, "보상아이템이 지급 되었습니다." ));		
								msgInfo.AddString(strMessage);
								_pUIMgr->CreateMessageBox(msgInfo);																		
							}
							else if(prompttype== 1)
							{
								msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
								strMessage.PrintF( _S(3479, "체험 교복이 지급되었습니다." ));
								msgInfo.AddString(strMessage);
								_pUIMgr->CreateMessageBox(msgInfo);
							}
							else if(prompttype== 2)
							{
								CTString strName1,strName2;
								strName1.PrintF("%s",_pNetwork->GetItemName(extra1));
								strName2.PrintF("%s",_pNetwork->GetItemName(extra2));

								// 저장
								pMinigame->m_extra1 = extra1;
								pMinigame->m_extra2 = extra2;

								strMessage= _S(3480, "보상 무기 선택" );
								msgInfo.SetMsgBoxInfo(strMessage,UMBS_SELECTBOX,UI_MINIGAME,MSGCMD_MINIGAME_SELECT_ITEM);
								msgInfo.m_nHeight =200;
								_pUIMgr->CreateMessageBox(msgInfo);
								_pUIMgr->GetMessageBox(MSGCMD_MINIGAME_SELECT_ITEM)->addSelectContext(strName1);
								_pUIMgr->GetMessageBox(MSGCMD_MINIGAME_SELECT_ITEM)->addSelectContext(strName2);																						

							}

							// 게임 창 닫기 
							pMinigame->Close();
						}
																				
						break;
					case MSG_EVENT_GOMDORI_2007_VIEW_STATUS:
						{
							ULONG serverCnt,itemCnt;
							ULONG itemAll=0;
							(*istr)>>serverCnt;
							strMessage= _S(3481, "곰돌이 인형 웬디 찾기 현황");

							msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL,300);
							
							strMessage=_S(3482, "♣완성된 곰돌이 웬디의 전체 누적 수에 따라 실제 곰 인형과 학용품 셋트를 어린이날 천사의 집 어린이들에게 선물하게 됩니다." );
							msgInfo.AddString(strMessage);
							
							for(int i=0;i<serverCnt;i++)
							{
								(*istr)>>itemCnt;
								strMessage.PrintF( _S(576, "%s : %d개" ),_pUIMgr->GetSelServer()->GetServerGroupName(i+1),itemCnt);
								msgInfo.AddString(strMessage);

								itemAll+=itemCnt;
							}
							msgInfo.AddString(CTString(" "));
							strMessage.PrintF( _S(3483, "전체 서버에서 찾은 완성품 수 : %d개" ),itemAll);
							msgInfo.AddString(strMessage);

							_pUIMgr->CreateMessageBox(msgInfo);
						}
						break;

					case MSG_EVENT_GOMDORI_2007_ERROR_NOITEM:
						{
							if(_pUIMgr->DoesMessageBoxExist(MSGCMD_NULL)) return;							
							msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
							strMessage.PrintF( _S(3484, "어라, 곰돌이가 없네요? 곰돌이가 없으면 가위바위보를 할 수 없어요. 게임을 하고 싶다면, 곰돌이조각 1번부터 9번까지를 모두 모아 완성된 곰돌이를 가져와야 해요." ));
							msgInfo.AddString(strMessage);
							_pUIMgr->CreateMessageBox(msgInfo);						
						}
						break;

					case MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY:
						strMessage=_S(675, "인벤토리가 가득 차서 아이템 지급이 불가능합니다." );			
						_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
						break;
			}
		}
		break;
	// ------------------------------------------------------------------<<
	case MSG_EVENT_FLOWERTREE_2007: // 2007 꽃놀이 이벤트
		{
			UBYTE subtype, errortype;
			SLONG SvCount, SvGroup;
			ULONG SvPoint, SvPoint1;

			CUIMsgBox_Info MsgBoxInfo;

			(*istr) >> subtype;

			switch (subtype)
			{
			case MSG_EVENT_FLOWERTREE_2007_POINT:
				{
					(*istr) >> SvPoint;
					(*istr) >> SvGroup;

					_pUIMgr->GetFlowerTree()->AddServerNameList(SvPoint, SvGroup);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_POINT_RANK:
				{
					(*istr) >> SvCount; // 서버군 갯수

					SLONG i;

					for (i=0; i<SvCount; i++)
					{
						(*istr) >> SvPoint; // 포인트
						(*istr) >> SvGroup; // 서버군 번호

						_pUIMgr->GetFlowerTree()->AddServerNameList(SvPoint, SvGroup);
					}
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_GROWUP:
				{
					(*istr) >> SvPoint; // 증가 포인트

					CTString strMessage;

					if (SvPoint > 0)
					{
						strMessage.PrintF(_S(3494, "이 나무의 개화를 도와주어 기여도가 %d증가 하였습니다."), SvPoint);
					}
					else
					{
						strMessage.PrintF(_S(3495, "나무에게 줄 퇴비와 정화수가 없습니다."));						
					}

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_MY_POINT:
				{
					(*istr) >> SvPoint; // 유저가 기여한 포인트
					(*istr) >> SvPoint1;
					
					if (_pUIMgr->DoesMessageBoxExist(MSGCMD_FLOWERTREE_SUPPORT))
						_pUIMgr->CloseMessageBox(MSGCMD_FLOWERTREE_SUPPORT);
					
					if (_pUIMgr->GetFlowerTree()->IsVisible())
						_pUIMgr->RearrangeOrder(UI_FLOWERTREE, FALSE);

					MsgBoxInfo.SetMsgBoxInfo(_S(169, "이벤트"), UMBS_USER_12, UI_FLOWERTREE, MSGCMD_FLOWERTREE_TICKET);
					MsgBoxInfo.SetUserBtnName(_S(3498, "교환권 받기"), _S(3499, "교환권 확인"));
					MsgBoxInfo.AddString( _S(3500, "꽃나무 기여포인트"), 0xF3F3F3FF, TEXT_CENTER );
					CTString strPoint, strPoint1;
					strPoint.PrintF(_S(3511, "나의 총 기여포인트: %d"), SvPoint);
					strPoint1.PrintF(_S(3512, "현재 교환권을 받을 갯수: %d"), SvPoint1);
					MsgBoxInfo.AddString(CTString(" "));
					MsgBoxInfo.AddString( strPoint, 0x6BD2FFFF, TEXT_CENTER );
					MsgBoxInfo.AddString( strPoint1, 0x6BD2FFFF, TEXT_CENTER );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_RECV_TICKET:
				{
					(*istr) >> SvCount; // 교환권 개수

					CTString strTicket;					

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					if (SvCount > 0)
						strTicket.PrintF(_S(3496, "누적된 교환권 %d장을 지급 받았습니다"), SvCount);
					else
						strTicket.PrintF(_S(3497, "지급받을 교환권이 없습니다."));

					MsgBoxInfo.AddString(strTicket);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET:
				{
					(*istr) >> errortype;
					
					switch (errortype)
					{
					case 0: // 실패
						{
							MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
							MsgBoxInfo.AddString(_S(3502, "현재 보유하고 있는 교환권이 없습니다. 확인하고 다시 시도하여 주십시요"));
							_pUIMgr->CreateMessageBox(MsgBoxInfo);
						}
						break;
					}
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_PRICE_MSG:
				{ // 보상 메세지
					CTString CharName, strMessage;
					SLONG nItemIndex;

					(*istr) >> CharName;
					(*istr) >> nItemIndex;

					strMessage.PrintF(_S(3503, "%s님이 %s 아이템에 당첨되었습니다."), CharName, _pNetwork->GetItemData(nItemIndex).GetName());
					_UIAutoHelp->SetGMNotice ( strMessage );
				}
				break;
			}
		}
		break;
		////////////////////////////////////////////////////////////////
		// [070705: Su-won] EVENT_SUMMER_2007						|---------->
	case MSG_EVENT_SUMMER_VACATION_2007:
		{
			LONG subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			switch (subtype)
			{
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_OK:
				{
					LONG nResult;
					LONG nReward;
					LONG nCount;
					(*istr) >> nResult;
					(*istr) >> nReward;
					(*istr) >> nCount;

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					
					CTString strColor;
					if( nResult == 1 )
						strColor = _S(3576, "그린");
					else if( nResult<=4)
						strColor = _S(3577, "레드");
					else if( nResult<=7)
						strColor = _S(3578, "실버");
					else if( nResult<=10)
						strColor = _S(3579, "골드");

					if( nResult == 1 )
						strMessage.PrintF( _S(3580, "색종이로 %s 꽃을 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==2) 
						strMessage.PrintF( _S(3581, "색종이로 %s 개구리를 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==0) 
						strMessage.PrintF( _S(3582, "색종이로 %s 종이학을 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==1) 
						strMessage.PrintF( _S(3583, "색종이로 %s 드래곤을 접으셨네요! 축하합니다!"), strColor);
					MsgBoxInfo.AddString(strMessage);
					
					if( nReward == 19 )
						strMessage.PrintF( _S(3584, "보상으로 %d 나스를 드리겠습니다."), nCount );
					else
						strMessage.PrintF( _S(3585, "보상으로 %s %d개를 드리겠습니다."), _pNetwork->GetItemName(nReward), nCount );
					MsgBoxInfo.AddString(strMessage);

					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK:
				{
					LONG nResult;
					(*istr) >> nResult;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(558, "조합에 성공하였습니다."));
					MsgBoxInfo.AddString(strMessage);
					strMessage.PrintF( _S(3586, "%s 1장이 생성되었습니다."), _pNetwork->GetItemName(nResult));
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(3588, "종이 접기에 가능한 조합이 아닙니다."));
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL:
				{
					LONG nResult;
					(*istr) >> nResult;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(557, "조합에 실패하였습니다."));
					MsgBoxInfo.AddString(strMessage);
					if( nResult != 2370 )
					{
						strMessage.PrintF( _S(3587, "%s 4장이 사라졌습니다."), _pNetwork->GetItemName(nResult));
						MsgBoxInfo.AddString(strMessage);
					}
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM:
				{
					strMessage.PrintF(_S( 1330 ,"아이템이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY:
				{
					strMessage.PrintF(_S( 266 ,"소지금이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			}
		}
		break;
		// [070703: Su-won] EVENT_SUMMER_2007						<----------|
		////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
	case MSG_EVENT_OPEN_ADULT_SERVER:
		{
			LONG subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			_pUIMgr->CloseMessageBox( MSGCMD_NULL );

			switch(subtype)
			{
			//////////////////////////////////////////////
			// 매직카드 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_OK:
				{
					UBYTE ubBuffType;
					(*istr) >> ubBuffType;

					CTString strBuff;
					switch(ubBuffType)
					{
					case 0:
						strBuff = CTString(_pNetwork->GetSkillData(423).GetName());	//바람의 러브매직
						break;
					case 1:
						strBuff = CTString(_pNetwork->GetSkillData(424).GetName()); //힘의 러브매직
						break;
					case 2:
						strBuff = CTString(_pNetwork->GetSkillData(425).GetName()); //민첩의 러브매직
						break;
					case 3:
						strBuff = CTString(_pNetwork->GetSkillData(426).GetName()); //지혜의 러브매직
						break;
					case 4:
						strBuff = CTString(_pNetwork->GetSkillData(427).GetName()); //체질의 러브매직
						break;
					case 5:
						strBuff = CTString(_pNetwork->GetSkillData(428).GetName()); //로레인의 러브매직
						break;
					}

					strMessage.PrintF( _S(3637, "[성인서버 오픈 이벤트] 이벤트 진행요원으로부터 %s 마법주문을 받았습니다."), strBuff);

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED:
				{
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM:
				{
					strMessage.PrintF(_S( 1330 ,"아이템이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			// 매직카드 이벤트
			//////////////////////////////////////////////
			

			//////////////////////////////////////////////
			// 전직 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE_OK:
				{
					LONG lRank, lCount;
					(*istr) >> lRank;
					(*istr) >> lCount;

					if( lRank <=100 )
						strMessage.PrintF( _S(3638, "%d등! 축하합니다. 상품으로 절망의 부적 %d개를 지급받으셨습니다."), lRank, lCount);
					else
						strMessage.PrintF( _S(3639, "축하합니다. 상품으로 절망의 부적 %d개를 지급받으셨습니다."), lCount);

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_ALREADY:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3640, "이미 전직 달성 상품을 받으셨습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FAILED:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3641, "전직을 하지 않아 상품을 받을 수 없습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FULLINVEN:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			// 전직 이벤트
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			// 연금술 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_OK:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3642, "축하합니다! 장비조합 연금술이 성공하였습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_NOT_FOUND:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3643, "연금술을 시도하기 위해 필요한 재료가 부족합니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3644, "이럴수가...장비조합 연금술이 실패하여 사용한 재료가 사라졌습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FULLINVEN:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			// 연금술 이벤트
			//////////////////////////////////////////////
			}
		}
		break;
		// '거침없이 쏜다!' 이벤트
	case MSG_EVENT_GIFT_2007:
		{
			BYTE subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			_pUIMgr->CloseMessageBox( MSGCMD_NULL );

			switch(subtype)
			{
				//성공
			case MSG_CONN_EVENT_GIFT_2007_GIVE:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(2234, "이벤트 아이템을 지급받았습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
				//이벤트 DB 에러(?)
			case MSG_CONN_EVENT_GIFT_2007_CHECK_DBERROR:
				break;
				//이미 보상 받았음
			case MSG_CONN_EVENT_GIFT_2007_CHECK_DENY:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(673, "이벤트 아이템이 이미 지급되었습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
				//DB 없음(?)
			case MSG_CONN_EVENT_GIFT_2007_GIVE_DBERROR:
				break;
				// 인벤 만땅
			case MSG_CONN_EVENT_GIFT_2007_GIVE_FULL:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다.") );
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;
	// [070807: Su-won] EVENT_ADULT_OPEN
	//////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_EVENT_BJMONO_2007:
		{
			BYTE subtype;

			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			ULONG ulCount;

			switch (subtype)
			{
			case MSG_EVENT_BJMONO_2007_TICKET_REQ:
				{
					(*istr) >> ulCount; // 응모권 갯수

					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_s("%d장의 응모권을 받았습니다."), ulCount);
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ:
				{
					_pUIMgr->CloseMessageBox(MSGCMD_BJMONO_2007_CHANGE_REQ);

					strMessage.PrintF(_s("응모권 교환 성공"));
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_BJMONO_2007_NO_TICKET:
				{
					_pNetwork->ClientSystemMessage(_s("받으실 응모권이 없습니다."), SYSMSG_ERROR);					
				}
				break;
			case MSG_EVENT_BJMONO_2007_DONT_CHANGE_TICKET:
				{
					_pNetwork->ClientSystemMessage( _s("교환 받을 응모권이 없습니다."), SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage(_s("인벤토리가 가득차서 보상을 받을 수 없습니다."), SYSMSG_ERROR);
				}
			}
		}
		break;		
	case MSG_EVENT_SSHOT_2007 : // WSS_TG2007 2007/09/14
		{
			UBYTE subtype;
			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;			

			switch (subtype)
			{
				case MSG_EVENT_SSHOT_2007_RECIVE:
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_s("송편 10개를 받았습니다."));
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
					break;
				case MSG_EVENT_SSHOT_2007_ALREADY:
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_S( 673,"이벤트 아이템이 이미 지급되었습니다."));
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
					break;
				case MSG_EVENT_SSHOT_2007_NOSSHOT:					// 응모한적 없음
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_s("송편받기 이벤트에 해당되지 않습니다. 확인하고 다시 시도하여 주십시오."));
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
					break;
				case MSG_EVENT_SSHOT_2007_FULLINVEN:				// 인벤이 가득 참
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_S( 116,"인벤토리 공간이 부족하여 보상을 받을 수 없습니다."));
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
					break;
			}

		}
		break;
	case MSG_EVENT_RICHYEAR : // WSS_TG2007 2007/09/14
		{
			UBYTE subtype;
			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;			
			MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch (subtype)
			{				
				case MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM:
					strMessage.PrintF(_S(1799,"재료가 부족합니다. 확인하고 다시 시도하여 주십시오."));
					break;
				case MSG_EVENT_RICHYEAR_ERROR_UPGRADE_FAILED:
					strMessage.PrintF(_S(1883,"작물 업그레이드에 실패하였습니다."));
					break;
				case MSG_EVENT_RICHYEAR_ERROR_NOCROP:
					strMessage.PrintF(_S(1879,"작물이 없습니다."));
					break;
				case MSG_EVENT_RICHYEAR_ERROR_ALREADY_UPGRADE:
					strMessage.PrintF(_S(2642,"더 이상 업그레이드 할 수 없습니다."));					
					break;
				case MSG_EVENT_RICHYEAR_ERROR_FULLINVEN:
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));					
					break;
				case MSG_EVENT_RICHYEAR_UPGRADE_OK:
					strMessage.PrintF(_S(1882,"작물 업그레이드에 성공하였습니다."));										
					break;
				case MSG_EVENT_RICHYEAR_EXCHAGE_OK:
					strMessage.PrintF(_S(1840,"아이템이 성공적으로 지급 되었습니다"));										
					break;

			}		
			
			MsgBoxInfo.AddString(strMessage);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_LC_1000DAY: // 라스트 카오스 1000일 이벤트
		{
			BYTE subType, noItem;

			(*istr) >> subType;
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;

			switch (subType)
			{
			case MSG_EVENT_LC_1000DAY_HAT_UPDATE_SUCCESS: // 모자 옵션 업데이트 성공
				{
					_pNetwork->ClientSystemMessage(_S(3833, "업그레이드를 성공하였습니다."), SYSMSG_ERROR);
				}
				break;
			case MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL: // 모자 옵션 업데이트 실패
				{
					(*istr) >> noItem;

					if (noItem == 1)
					{
						_pNetwork->ClientSystemMessage(_S(3834, "업그레이드할 기념모자 아이템이 없습니다." ), SYSMSG_ERROR);
					}
					else
					{
						_pNetwork->ClientSystemMessage(_S(3835, "업그레이드를 실패하였습니다." ), SYSMSG_ERROR);
						_pNetwork->ClientSystemMessage(_S(3836, "기념모자가 파괴되었습니다." ), SYSMSG_ERROR);
					}
				}
				break;
			case MSG_EVENT_LC_1000DAY_FIRECRACKER_DUPLICATION: // 폭죽 아이템이 인벤에 있음
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(3837, "폭죽을 다 사용하고 나서 다시 시도하여 주십시오."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_LC_1000DAY_NOITEM: // 아이템 부족
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(3838, "이벤트 아이템이 부족하여 기념모자를 받을 수 없습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_LC_1000DAY_FULLINVEN: // 인벤이 가득 참
				{
					_pNetwork->ClientSystemMessage(_S( 265 ,"인벤토리 공간이 부족합니다." ), SYSMSG_ERROR);
				}
				break;
			}
		}
		break;
	case MSG_EVENT_HALLOWEEN_2007:	// 할로윈 이벤트 2007
		{
			BYTE subType, subResult;

			(*istr) >> subType;
			(*istr) >> subResult;
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, 확인), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch (subType)
			{
			case MSG_EVENT_TAKE_DEVILHEAIR:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3781, "악마 날개 머리띠를 지급 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3782, "악마 날개 머리띠를 더이상 받을 수 없습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_CANDYBASKET:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3783, "사탕을 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3784, "바구니가 없어 사탕을 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_PUMKINHEAD:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3785, "호박 머리탈을 교환 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3786, "사탕의 종류가 모자라 교환 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_WITCHHAT:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3787, "마녀(마법사) 모자를 교환 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3788, "증표가 없어 교환을 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_PUMKIN:
				{
					switch (subResult)
					{
					case 0:
						{
							SLONG slItemIndex;
							(*istr) >> slItemIndex;
							strMessage.PrintF(_S(3789, "호박을 쪼개었더니 %s이(가) 나왔습니다!"), _pNetwork->GetItemName(slItemIndex));
							MsgBoxInfo.AddString(strMessage);
						}
						break;
					case 1: MsgBoxInfo.AddString(_S(3790, "너에게 볼일은 없어 썩 꺼져!")); break;
					}
				}
				break;
			case MSG_EVENT_TASTE_CANDY:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3791, "절망적인 끔직한 맛! 잠시 지옥을 본 것 같아!!")); break;
					case 1: MsgBoxInfo.AddString(_S(3792, "우~웩! 정말! 토 나오는 맛이군!!")); break;
					case 2: MsgBoxInfo.AddString(_S(3793, "그럭저럭 먹을 만 하군!!")); break;
					case 3: MsgBoxInfo.AddString(_S(3794, "오! 의외로 괜찮은 맛 이군!!")); break;
					case 4: MsgBoxInfo.AddString(_S(3795, "아! 천상의 맛이야! 아 날개 달린 당신은...천..사!!")); break;
					}
				}
				break;
			case MSG_EVENT_FULL_INVEN:
				{
					_pNetwork->ClientSystemMessage(_S(3796, "인벤토리가 가득차서 아이템을 받을 수 없습니다."), SYSMSG_ERROR);
					return;
				}
				break;
			}

			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;

	/////////////////////////////////////////////////////////////////////////
	// [071122: Su-won] NEW_MOONSTONE
	case MSG_EVENT_NEW_MOONSTONE:
		{
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			unsigned char ucIndex;
			(*istr) >> ucIndex;
				
			switch ( ucIndex )
			{
			case MSG_EVENT_NEW_MOONSTONE_START_REP:
				{
					LONG lAccumulate;
					(*istr) >> lAccumulate;
					_pUIMgr->GetGamble()->SetAccumulate( lAccumulate );
					_pUIMgr->GetGamble()->ShowGamble();
					_pUIMgr->SetCSFlagOn( CSF_MOONSTONE );
				}
				break;

			case MSG_EVENT_NEW_MOONSTONE_TRY_REP:		// 문스톤 복권 응답
				{
					LONG lRewardItem;
					LONG lAccumulate;
					(*istr) >> lRewardItem;
					(*istr) >> lAccumulate;
					
					_pUIMgr->GetGamble()->SetSelectedItem( lRewardItem );
					_pUIMgr->GetGamble()->Start();
					_pUIMgr->GetGamble()->SetAccumulate( lAccumulate );
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_RESULT_REP:	// 문스톤 복권 결과 응답.
				{
					LONG	lRewardItem;					
					(*istr) >> lRewardItem;
					_pUIMgr->GetGamble()->Stop();
					
					_pUIMgr->GetGamble()->ShowPrizeEffect();
				}
				break;

			case MSG_EVENT_NEW_MOONSTONE_JACK_POT:
				{
					_pUIMgr->GetGamble()->ShowPrizeEffect();
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_OK:
				{
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE:
				{
					MsgBoxInfo.AddString(_S(1439, "문스톤이 없습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

					_pUIMgr->GetGamble()->ResetGamble();
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_FULLINVEN:
				{
					MsgBoxInfo.AddString(_S(2850, "인벤토리가 가득 찼습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_SUCCESS:
				{
					MsgBoxInfo.AddString(_S(558, "조합에 성공하였습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_FAILED:
				{
					MsgBoxInfo.AddString(_S(557, "조합에 실패하였습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_MISS:
				{
					MsgBoxInfo.AddString(_S(2466, "제작에 적합하지 않은 조합입니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;
		// [071122: Su-won] NEW_MOONSTONE
		/////////////////////////////////////////////////////////////////////////
	case MSG_EVENT_XMAS_2007: // X-MAS 2007 eons 071204
		{
			UBYTE ubSucess;
			UBYTE ubType;
			SLONG slRemain;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			(*istr) >> ubType;

			switch (ubType)
			{
			case MSG_EVENT_XMASTREE_ADD_POINT: // 장식
				{
					(*istr) >> ubSucess;

					if (ubSucess == 1)
					{
						MsgBoxInfo.AddString(_S(3954, "크리스마스 트리를 장식할 아이템이 없습니다"));
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
				}
				break;
			case MSG_EVENT_XMASTREE_GET_BLESS: // 축복
				{
					(*istr) >> ubSucess;

					if (ubSucess == 1)
					{
						MsgBoxInfo.AddString(_S(3955, "지금은 크리스마스 축복을 받을 수 없습니다. 잠시 후 다시 시도하여주십시오"));
						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}
				}
				break;
			case MSG_EVENT_XMASTREE_DROP_ITEM: // 선물 공지
				{
					(*istr) >> slRemain;

					strMessage.PrintF(_S(3956, "%d분 뒤 크리스마스 트리주위에 크리스마스 선물이 놓여지게 됩니다"), slRemain);
					
					_UIAutoHelp->SetGMNotice ( strMessage, 0xFFAA33FF );
				}
				break;
			}
		}
		break;

	// [071217: Su-won] EVENT_NEWYEAR_2008
	case MSG_EVENT_NEWYEAR_2008:
		{
			INDEX iChar;
			CTString strCharName;

			(*istr) >> iChar;
			(*istr) >> strCharName;

			CTString strMsg;
			strMsg.PrintF(_S(3965, "%s님으로부터 새해복 많이 받으세요 새해인사가 도착하였습니다."), strCharName);
			_pNetwork->ClientSystemMessage( strMsg, SYSMSG_ERROR );

			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OKCANCEL, UI_NONE, MSGCMD_EVENT_NEWYEAR_2008);
			MsgBoxInfo.AddString(_S(3966, "해맞이 버프가 시전되었습니다. 수락하시겠습니까?"));
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_SAKURA_2008:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			switch (ubType)
			{
			case MSG_EVENT_SAKURAEVENT_2008_ERROR:
				{
					UBYTE ubError;
					(*istr) >> ubError;

					if( ubError == 0 )
					{

					}
					else if( ubError == 1 )
					{
						CUIMsgBox_Info MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

						MsgBoxInfo.AddString(_S(4114, "퇴비를 가지고 있지 않습니다."));
						_pUIMgr->CreateMessageBox(MsgBoxInfo);

						return;
					}
				}
				break;
			case MSG_EVENT_SAKURAEVENT_2008_ITEMCHANGE:
				{
					CTString strName;
					INDEX iItemIndex;

					(*istr) >> strName;
					(*istr) >> iItemIndex;

					CTString strNotice;
					strNotice.PrintF( _S(4115, "%s님이 벚꽃선물 상자를 열어 %s을 획득하였습니다."), strName, _pNetwork->GetItemName(iItemIndex));

					_UIAutoHelp->SetGMNotice( strNotice );

					return;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_PHOENIX:
		{
			UBYTE ubType;
			(*istr) >> ubType;
			switch(ubType)
			{
			case MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4235, "이미 피닉스로 등록되어 있습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_ERROR_REQUIREMENT:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4236, "피닉스가 될 자격이 되지 않습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4237, "인벤토리가 부족하거나 한계 무게를 초과하였습니다.. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_OK:
				{
					if(_pUIMgr->GetQuest()->GetNpcIndex() == 834)
					{
						FLOAT fNPCPosX = _pUIMgr->GetQuest()->GetNpcPosX();
						FLOAT fNPCPosZ = _pUIMgr->GetQuest()->GetNpcPosZ();

						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OKCANCEL, UI_NONE, MSGCMD_EVENT_PHOENIX, -1, fNPCPosX, fNPCPosZ);
						MsgBoxInfo.AddString(_S(4238, "피닉스가 될 자격이 충분합니다. 가입하시겠습니까?"));
						_pUIMgr->CreateMessageBox( MsgBoxInfo );		
					}
					return;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_PHOENIX_SUCCESS:
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(_S(4239, "피닉스로 등록되었습니다."));
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			return;			
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePersonalShopMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePersonalShopMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	
	
	switch(ubType)
	{
	case MSG_PERSONALSHOP_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			//case MSG_PERSONALSHOP_ERROR_OK:
			//	strMessage.PrintF(_S( 771, "정상" ));		
			//	break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_SKILL:		// 스킬 사용
				strMessage.PrintF(_S( 772, "거래를 요청한 캐릭터가 스킬을 사용 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_WARP:		// 워프
				strMessage.PrintF(_S( 773, "거래를 요청한 캐릭터가 워프(이동장치) 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE:	// 교환 중
				strMessage.PrintF(_S( 774, "거래를 요청한 캐릭터가 교환 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY:		// 상점 개설중
				strMessage.PrintF(_S( 775, "거래를 요청한 캐릭터가 이미 개인상점 거래중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_MONEY:		// 나스를 개인상점으로 거래하고자 할때
				strMessage.PrintF(_S( 776, "나스는 개인상점에서 거래할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_ETC:			// 상점을 열수 없는 상태(스킬사용/워프/패킷내용오류 등등)
				strMessage.PrintF(_S( 777, "개인상점 거래가 취소되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_DUPLICATION:		// 중복 등록
				strMessage.PrintF(_S( 778, "아이템이 중복 등록 되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTSELL:			// 대상 캐릭터가 판매 상점이 아닐 때
				strMessage.PrintF(_S( 779, "대상 캐릭터가 판매 상점이 아닙니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTPACKAGE:			// 대상 캐릭터가 패키지 판매가 아닐 때
				strMessage.PrintF(_S( 780, "대상 캐릭터가 패키지 판매중이 아닙니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTFOUND:			// 대상 캐릭터가 없을 때
				strMessage.PrintF(_S( 781, "대상 캐릭터가 존재하지 않습니다." ));		
				break;				
			case MSG_PERSONALSHOP_ERROR_MONEY:				// 소지금 부족
				strMessage.PrintF(_S( 782, "소지금 부족하여 거래를 할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_COUNT:			// 대상 개인상점보다 요청 판매/구매 카운트가 클때
				strMessage.PrintF(_S( 783, "요청한 아이템 개수가 올바르지 않습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_CANTPREMIUM:	// 소지금 부족으로 패키지 상점 개설 불가
				strMessage.PrintF(_S( 784, "소지금이 부족하여 패키지 상점을 개설할 수 없습니다." ));		
				break;
			// Date : 2005-02-01,   By Lee Ki-hwan
			case MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY:	// 인벤토리가 가득 차서 물건을 구입할 수 없습니다.
				strMessage.PrintF( _S( 862, "인벤토리가 가득 차서 물건을 구입할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_AREA:
				strMessage.PrintF(_S( 785, "개인상점을 개설할 수 없는 지역입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_ANYMORE_PET :
				strMessage.PrintF(_S( 2582,"펫 소유 제한이 넘어서 취소되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOSPACE :
				strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
				break;

			}
			_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		break;
	case MSG_PERSONALSHOP_CHANGE:
		{
			LONG	lChaIndex;
			SBYTE	sbShopType;
			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;

			//// 상점 상태 변화			: charindex(n) shoptype(c)
			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				if(sbShopType == PST_NOSHOP)
				{
					CTString strSysMessage;
					strSysMessage.PrintF(_S( 786, "상점을 닫았습니다." ));		
					_pNetwork->ClientSystemMessage( strSysMessage );
					
					_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
					_pUIMgr->GetPersonalShop()->ChangeShopState(FALSE);
					_pNetwork->MyCharacterInfo.ShopMsg.Reset();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( FALSE );
				}
			}
			// 타캐릭터의 상점 상태 변화.
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lChaIndex )
					{
						ct.cha_sbShopType = sbShopType;
						if( sbShopType == PST_NOSHOP )
						{
							ct.ShopMsg.Reset();
							if (_pUIMgr->GetPersonalShop()->EndBuyShop(lChaIndex))
							{
								CTString strSysMessage;
								strSysMessage.PrintF(_S(3075, "%s 님이 개인상점을 닫았습니다."), ct.cha_strName);
								_pNetwork->ClientSystemMessage(strSysMessage);
							}
						}

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
						break;
					}
				}				
			}
		}
		break;
	case MSG_PERSONALSHOP_SELL_LIST:
		{
			LONG	lChaIndex;
			(*istr) >> lChaIndex;			
			_pUIMgr->GetPersonalShop()->ReceivePersonalShopItemList(lChaIndex, &(*istr));
		}
		break;	
	case MSG_PERSONALSHOP_BUY:
		{
			_pUIMgr->GetPersonalShop()->ReceivePersonalShopItemUpdate(&(*istr));
		}
		break;
	case MSG_PERSONALSHOP_SELL_START:
		{
			// charindex(n) shoptype(c) shopname(str)
			LONG lChaIndex;
			SBYTE sbShopType;
			CTString strShopName;

			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;
			(*istr) >> strShopName;

			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pUIMgr->GetPersonalShop()->ChangeShopState(TRUE);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( TRUE );
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				_pNetwork->MyCharacterInfo.ShopMsg.SetChatMsg( strShopName );

				CTString strSysMessage;
				strSysMessage.PrintF( _S( 1037, "[%s] 상점을 열었습니다." ), strShopName);
				_pNetwork->ClientSystemMessage( strSysMessage );
				_pUIMgr->SetCSFlagOn( CSF_PERSONALSHOP );
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lChaIndex )
					{
						ct.cha_sbShopType = sbShopType;
						ct.ShopMsg.SetChatMsg( strShopName );

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
						break;
					}
				}
			}
		}
		break;
	}
}
// ----------------------------------------------------------------------------
// Name : ReceivePersonalShopMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveCashPersonShopMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	
	
	switch(ubType)
	{
	case MSG_ALTERNATEMERCHANT_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			//case MSG_PERSONALSHOP_ERROR_OK:
			//	strMessage.PrintF(_S( 771, "정상" ));		
			//	break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL:		// 스킬 사용
				strMessage.PrintF(_S( 772, "거래를 요청한 캐릭터가 스킬을 사용 중입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_WARP:		// 워프
				strMessage.PrintF(_S( 773, "거래를 요청한 캐릭터가 워프(이동장치) 중입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE:	// 교환 중
				strMessage.PrintF(_S( 774, "거래를 요청한 캐릭터가 교환 중입니다." ));		
				break;
//			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ALREADY:		// 상점 개설중
//				strMessage.PrintF(_S( 775, "거래를 요청한 캐릭터가 이미 개인상점 거래중입니다." ));		
//				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_MONEY:		// 나스를 개인상점으로 거래하고자 할때
				strMessage.PrintF(_S( 776, "나스는 개인상점에서 거래할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC:			// 상점을 열수 없는 상태(스킬사용/워프/패킷내용오류 등등)
				strMessage.PrintF(_S( 777, "개인상점 거래가 취소되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_DUPLICATION:		// 중복 등록
				strMessage.PrintF(_S( 778, "아이템이 중복 등록 되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTSELL:			// 대상 캐릭터가 판매 상점이 아닐 때
				strMessage.PrintF(_S( 779, "대상 캐릭터가 판매 상점이 아닙니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTPACKAGE:			// 대상 캐릭터가 패키지 판매가 아닐 때
				strMessage.PrintF(_S( 780, "대상 캐릭터가 패키지 판매중이 아닙니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND:			// 대상 캐릭터가 없을 때
				strMessage.PrintF(_S( 781, "대상 캐릭터가 존재하지 않습니다." ));		
				break;				
			case MSG_ALTERNATEMERCHANT_ERROR_MONEY:				// 소지금 부족
				strMessage.PrintF(_S( 782, "소지금 부족하여 거래를 할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_COUNT:			// 대상 개인상점보다 요청 판매/구매 카운트가 클때
				strMessage.PrintF(_S( 783, "요청한 아이템 개수가 올바르지 않습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_CANTPREMIUM:	// 소지금 부족으로 패키지 상점 개설 불가
				strMessage.PrintF(_S( 784, "소지금이 부족하여 패키지 상점을 개설할 수 없습니다." ));		
				break;
			// Date : 2005-02-01,   By Lee Ki-hwan
			case MSG_ALTERNATEMERCHANT_ERROR_BUY_FULLINVENTORY:	// 인벤토리가 가득 차서 물건을 구입할 수 없습니다.
				strMessage.PrintF( _S( 862, "인벤토리가 가득 차서 물건을 구입할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_AREA:
				strMessage.PrintF(_S( 785, "개인상점을 개설할 수 없는 지역입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_ANYMORE_PET :
				strMessage.PrintF(_S( 2582,"펫 소유 제한이 넘어서 취소되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOSPACE :
				strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOCOLLECT_ITEM:
				strMessage.PrintF(_s("회수 되지 않은 아이템이 존재합니다."));
				break;
			case  MSG_ALTERNATEMERCHANT_ERROR_TIMEOVER:
				strMessage.PrintF(_s("판매 대행 시간이 만료 되었습니다. 판매중인 아이템은 판매 대행협회 회장님께 회수 하시기 바랍니다."));
				break;

			}
			_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		break;
	case MSG_ALTERNATEMERCHANT_CHANGE:
		{
			LONG	lChaIndex;
			SBYTE	sbShopType;
			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;

			//// 상점 상태 변화			: charindex(n) shoptype(c)
			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				if(sbShopType == PST_NOSHOP)
				{
					CTString strSysMessage;
					strSysMessage.PrintF(_S( 786, "상점을 닫았습니다." ));		
					_pNetwork->ClientSystemMessage( strSysMessage );
					
					_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
					_pUIMgr->GetPersonalShop()->ChangeShopState(FALSE);
					_pNetwork->MyCharacterInfo.ShopMsg.Reset();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( FALSE );
				}
			}
			// 타캐릭터의 상점 상태 변화.
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lChaIndex )
					{
						ct.cha_sbShopType = sbShopType;
						if( sbShopType == PST_NOSHOP )
						{
							ct.ShopMsg.Reset();
							if (_pUIMgr->GetPersonalShop()->EndBuyShop(lChaIndex))
							{
								CTString strSysMessage;
								strSysMessage.PrintF(_S(3075, "%s 님이 개인상점을 닫았습니다."), ct.cha_strName);
								_pNetwork->ClientSystemMessage(strSysMessage);
							}
						}

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
						break;
					}
				}				
			}
		}
		break;
	case MSG_ALTERNATEMERCHANT_SELL_LIST:
		{
			LONG	lChaIndex;
			(*istr) >> lChaIndex;			
			_pUIMgr->GetPersonalShop()->ReceivePersonalShopItemList(lChaIndex, &(*istr));
		}
		break;	
	case MSG_ALTERNATEMERCHANT_BUY:
		{
			_pUIMgr->GetPersonalShop()->ReceivePersonalShopItemUpdate(&(*istr));
		}
		break;
	case MSG_ALTERNATEMERCHANT_SELL_START:
		{
			// charindex(n) shoptype(c) shopname(str)
			LONG lChaIndex;
			SBYTE sbShopType;
			CTString strShopName;

			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;
			(*istr) >> strShopName;

			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pUIMgr->GetPersonalShop()->ChangeShopState(TRUE);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( TRUE );
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				_pNetwork->MyCharacterInfo.ShopMsg.SetChatMsg( strShopName );

				CTString strSysMessage;
				strSysMessage.PrintF( _S( 1037, "[%s] 상점을 열었습니다." ), strShopName);
				_pNetwork->ClientSystemMessage( strSysMessage );
				_pUIMgr->SetCSFlagOn( CSF_PERSONALSHOP );
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lChaIndex )
					{
						ct.cha_sbShopType = sbShopType;
						ct.ShopMsg.SetChatMsg( strShopName );

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
						break;
					}
				}
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SetRecentServer()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::SetRecentServer(int iRecentGroup, int iRecentServer)
{
	m_iRecentGroup	= iRecentGroup;
	m_iRecentServer = iRecentServer;
}

// ----------------------------------------------------------------------------
// Name : ReceiveLegitimateMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveLegitimateMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;		
	SBYTE	targettype;
	SLONG	targetindex;	

	(*istr) >> ubType;
	(*istr) >> targettype;
	(*istr) >> targetindex;	

	if(targettype != MSG_CHAR_PC)
	{
		return;
	}	

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetLegit(ubType,targetindex);
}

// ----------------------------------------------------------------------------
// Name : ReceiveWareHouseMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveWareHouseMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	
	
	switch(ubType)
	{
	case MSG_STASH_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			//case MSG_STASH_ERROR_OK:						// 정상
			//	strMessage.PrintF( _S( 771, "정상" ));		
			//	break;
			case MSG_STASH_ERROR_CANNOT_STATE:				
				strMessage.PrintF(_S( 850, "창고 이용을 할 수 없는 상태입니다." ));		
				break;
			case MSG_STASH_ERROR_NOTCOMPLETE:				// 이전 창고 요청이 마무리 되지 않았음
				strMessage.PrintF(_S( 851, "이전의 창고 요청이 마무리 되지 않았습니다." ));		
				break;
			case MSG_STASH_ERROR_SEALED:					// 창고 긴급 잠김 상태
				strMessage.PrintF(_S( 852, "창고가 긴급 잠김 상태입니다." ));	
				break;
			case MSG_STASH_ERROR_WRONG_PASSWORD:			// 암호 틀림
				strMessage.PrintF(_S( 853, "암호가 올바르지 않습니다." ));	
				break;
			//case MSG_STASH_ERROR_WRONG_PACKET:				// 패킷 내용 오류
				//strMessage.PrintF( _S( 961, "패킷 내용에 오류가 발생했습니다." ) );	
				//break;
			case MSG_STASH_ERROR_MONEY:						// 돈 부족
				strMessage = _S( 306, "나스가 부족합니다." );
				break;
			case MSG_STASH_ERROR_STASH_FULL:				// 창고 가득 참
				strMessage.PrintF(_S( 855, "창고가 가득 찼습니다." ));	
				break;
			case MSG_STASH_ERROR_WEIGHT_OVER:				// 무게 초과로 찾을 수 없음
				strMessage.PrintF(_S( 856, "무게를 초과로 아이템을 찾을 수 없습니다." ));		
				break;
			case MSG_STASH_ERROR_INVENTORY_FULL:			// 인벤토리 개수 초과로 찾을 수 없음
				strMessage.PrintF(_S( 857, "인벤토리가 꽉 차서 찾을 수 없습니다." ));		
				break;
			case MSG_STASH_ERROR_WRONG_CHAR:				// 암호에 잘못된 문자가 들어감
				strMessage.PrintF(_S( 809, "암호에 잘못된 문자가 들어가 있습니다." ));	
				break;
			case MSG_STASH_ERROR_CANNOT_SEAL:				// 잠글수 없음
				strMessage.PrintF(_S( 810, "잠글 수 없습니다." ));		
				break;
			case MSG_STASH_ERROR_ALREADY_SEAL:				// 이미 잠김
				strMessage.PrintF(_S( 811, "이미 잠겨있습니다." ));	
				break;
			}

			_pUIMgr->CloseMessageBox(MSGCMD_WAREHOUSE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OK, UI_WAREHOUSE, MSGCMD_WAREHOUSE_ERROR );	
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );			

			_pUIMgr->GetWareHouse()->ResetWareHouse();
			_pUIMgr->GetSecurity()->ResetSecurity();
		}
		break;

	case MSG_STASH_ISSETPASSWORD:
		{
			SBYTE sbPW;
			(*istr) >> sbPW;

			_pUIMgr->GetWareHouse()->OpenWareHouse( sbPW );
		}
		break;

	case MSG_STASH_LIST_REP:
		{
			// 창고에 보관한 아이템 목록...
			_pUIMgr->GetWareHouse()->ReceiveWareHouseListRep( istr );			
		}
		break;

	case MSG_STASH_KEEP_REP:
		{
			// 맡기기 결과				: keepcount(n) [itemdbindex(n) count(ll)]:keepcount
			LONG lKeepCount;
			CTString strMessage;
			(*istr) >> lKeepCount;
			for(int i = 0; i < lKeepCount; ++i)
			{
				LONG	lItemDBIndex;
				SQUAD	llCount;
				(*istr) >> lItemDBIndex;
				(*istr) >> llCount;
				//const char* szName = _pNetwork->GetItemData( lItemDBIndex ).GetName();
				const char* szName = _pNetwork->GetItemName( lItemDBIndex );
				CItemData& rItemData = _pNetwork->GetItemData( lItemDBIndex );
				if( rItemData.GetType() == CItemData::ITEM_ETC &&
					rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					strMessage.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), llCount);		
				}
				else
				{
					strMessage.PrintF(_S( 808, "%s를 %I64d개 보관하였습니다." ), szName, llCount);
				}
				_pNetwork->ClientSystemMessage( strMessage );
			}
			//_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
			_pUIMgr->GetWareHouse()->ResetWareHouse();
		}
		break;

	case MSG_STASH_TAKE_REP:
		{
			// 찾기 결과				: takecount(n) [itemdbindex(n) count(ll)]:keepcount
			LONG lTakeCount;
			CTString strMessage;
			(*istr) >> lTakeCount;
			for(int i = 0; i < lTakeCount; ++i)
			{
				LONG	lItemDBIndex;
				SQUAD	llCount;
				(*istr) >> lItemDBIndex;
				(*istr) >> llCount;
				//const char* szName = _pNetwork->GetItemData( lItemDBIndex ).GetName();
				//strMessage.PrintF("%s를 %I64d개 찾았습니다.", szName, llCount);
				//_pNetwork->ClientSystemMessage( strMessage );

			}
			//_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
			_pUIMgr->GetWareHouse()->ResetWareHouse();
		}
		break;

	case MSG_STASH_CHANGE_PASSWORD_REP:
		{
			// 잠금 요청/결과
			SBYTE sbPW;
			(*istr) >> sbPW;

			if( sbPW == 1)
			{
				CTString strMessage = _S( 962, "암호가 변경되었습니다." );		

				// FIXME : 에러는 아니지만....
				_pUIMgr->CloseMessageBox(MSGCMD_WAREHOUSE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OK, UI_WAREHOUSE, MSGCMD_WAREHOUSE_ERROR );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			_pUIMgr->GetSecurity()->ResetSecurity();
		}
		break;
		
	case MSG_STASH_DELETE_PASSWORD_REP:
		{
			// 암호 제거 요청.
			SBYTE sbResult;
			(*istr) >> sbResult;

			_pUIMgr->GetSecurity()->ResetSecurity();

			if(sbResult == 0)		// 성공
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1770, "암호 삭제" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1771, "암호가 삭제되었습니다.  새 암호를 설정하십시오." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if(sbResult == 1)	// 틀린 주민등록 번호
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1770, "암호 삭제" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1772, "주민등록 번호가 일치하지 않습니다.  확인 후 다시 시도하여 주십시오." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );				
			}
			else if(sbResult == 2)	// 기타
			{				
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1770, "암호 삭제" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1773, "오류가 발생했습니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;

	case MSG_STASH_CHECK_PASSWORD_REP:		// 암호 확인 결과.
		{
			// 암호가 맞음...
			_pUIMgr->CloseMessageBox( MSGCMD_NEW_PASSWORD );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 1727, "보안 입력" ), UMBS_OK | UMBS_INPUTPASSWORD, UI_SECURITY, MSGCMD_NEW_PASSWORD );		
			CTString	strMessage =  _S( 1738, "새 암호를 입력하여 주십시오." );	
			MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
			strMessage =  _S( 1729, "8자(영문 숫자조합)." );	
			MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_NEW_PASSWORD );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			//pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
			pMsgBox->GetInputBox().SetMaxChar(8);
		}
		break;

	case MSG_STASH_SETTING_NEWPASSWORD_REP:	// 암호 설정 결과.
		{
			_pUIMgr->GetSecurity()->ResetSecurity();

			// 암호 설정 결과.
			SBYTE sbResult;
			(*istr) >> sbResult;
			if(sbResult == 0)
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1774, "암호 설정" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1775, "암호가 설정되었습니다. 창고 보안 기능이 활성화됩니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if(sbResult == 1)
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1774, "암호 설정" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1776, "적절하지 않은 암호입니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if(sbResult == 2)
			{
			}
		}
		break;	

	case MSG_STASH_SEAL:
		{
			_pUIMgr->GetSecurity()->ResetSecurity();
		}
		break;	
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveGuildMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveGuildMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch(ubType)
	{
	case MSG_GUILD_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;
			
			CTString strMessage;
			switch(sbError)
			{
			//case MSG_GUILD_ERROR_OK:
			//	strMessage.PrintF( _S( 963, "정상" ) );		
			//	break;
			
			// 길드 배틀 관련 에러 처리 부분
			// UIGuildBattle에서 한꺼번에 처리 합니다.
			case MSG_GUILD_ERROR_BATTLE:
				{
					SBYTE	sbError;
					(*istr) >> sbError;	
					_pUIMgr->GetGuildBattle()->GBErrorProcess( sbError );
				}
				return;
			case MSG_GUILD_ERROR_PACKET:
				ASSERTALWAYS("호출되면 안되는 부분");
				break;
			case MSG_GUILD_ERROR_CHARLEVEL:
				strMessage.PrintF(  _S( 965, "레벨이 부족합니다." )  );		
				break;
			case MSG_GUILD_ERRRO_SKILLPOINT:
				strMessage.PrintF(  _S( 966, "SP가 부족합니다." )  );		
				break;
			case MSG_GUILD_ERROR_MONEY:
				strMessage.PrintF(  _S( 967, "나스가 부족합니다." )  );		
				break;
			case MSG_GUILD_ERROR_ALREADYBOSS:
				strMessage.PrintF(  _S( 968, "다른 길드의 (부)장이기 때문에 길드를 생성할 수 없습니다." ) );		
				break;			
			case MSG_GUILD_ERROR_DUPNAME:
				strMessage.PrintF(  _S( 969, "이미 존재하는 길드 이름입니다." )  );		
				break;
			case MSG_GUILD_ERROR_GUILDNAME:
				strMessage.PrintF(  _S( 970, "길드 이름에 오류가 있습니다." )  );		
				break;
			case MSG_GUILD_ERROR_GAMESERVER:
				strMessage.PrintF(  _S( 971, "게임 서버의 오류로 인해서 길드를 생성할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOGUILD:
				strMessage.PrintF(  _S( 972, "길드에 속해있지 않습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOTBOSS:
				strMessage.PrintF(  _S( 973, "권한이 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_MAXLEVEL:
				strMessage.PrintF(  _S( 974, "더 이상 길드의 등급을 높일 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CREATE_OK:
				strMessage.PrintF(  _S( 975, "길드가 생성 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_LEVELUP_OK:
				strMessage.PrintF(  _S( 976, "길드가 승급 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_BREAKUP_OK:
				strMessage.PrintF(  _S( 977, "길드가 해산 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_TARGET_NOBOSS:
				strMessage.PrintF(  _S( 978, "대상이 길드장이 아니어서 가입 신청을 할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_TARGET_BUSY:
				strMessage.PrintF(  _S( 979, "길드장이 다른 요청을 처리하고 있습니다. 다시 시도하십시요." )  );		
			_pNetwork->GuildApplicantReject( FALSE );
				break;
			case MSG_GUILD_ERROR_SOURCE_BUSY:
				strMessage.PrintF(  _S( 980, "길드 가입 신청중에 다른 길드에 가입 신청을 할 수 없습니다." )  );		
				_pNetwork->GuildApplicantReject( FALSE );
				break;
			case MSG_GUILD_ERROR_FULLMEMBER:					// 길드 정원이 초과
				strMessage.PrintF( _S( 981, "길드에 정원이 초과 하여 가입을 할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_REGIST_OK:					// 길드원 가입 완료
				{				
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
				strMessage.PrintF( _S( 982, "길드에 가입되었습니다." )  );		
				break;
				}
				//return;
			case MSG_GUILD_ERROR_OUTBOSS:					// 길드장이 탈퇴하려면 紵?
				strMessage.PrintF( _S( 983, "길드장은 탈퇴할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_OUT_OK:						// 탈퇴 성공
				strMessage.PrintF( _S( 984, "길드 탈퇴가 완료되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_NOTFOUNDMEMBER:				// 강퇴, 임명, 이임등에서 대상 길원이 없을때
				strMessage.PrintF( _S( 985, "해당 길드원을 찾을 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANTKICKBOSS:				// 길장은 당퇴 불능
				strMessage.PrintF( _S( 986, "길드(부)장은 퇴출 대상이 아닙니다." ) );		
				break;
			case MSG_GUILD_ERROR_KICK_OK:					// 퇴출 성공
				strMessage.PrintF( _S( 987, "길드원이 퇴출 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_CHANGE_BOSS_OK:
				strMessage.PrintF( _S( 988, "길드장이 이임 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_CANTOFFICER:				// 길드장이나 부관을 다시 부관으로 임명시
				strMessage.PrintF( _S( 989, "해당 길드원를 길드부장으로 임명할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_APPOINT_OFFICER_OK:		// 부관 임명 성공
				strMessage.PrintF( _S( 990, "길드부장이 임명 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_FULLOFFICER:				// 부관 가득 참
				strMessage.PrintF( _S( 991, "더 이상 길드부장을 임명할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOTOFFICER:					// 부관이 아니라서 해임 불가
				strMessage.PrintF( _S( 992, "길드부장이 아니라서 해임할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_FIRE_OFFICER_OK:			// 해임 성공S
				strMessage.PrintF( _S( 993, "해임이 성공하였습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_REGDELAY:					// 탈퇴후 7일이 지나지 않아 가입 불능
				strMessage.PrintF( _S( 994, "탈퇴 후 7일이 지나지 않아서 길드에 가입할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANTKICK:
				strMessage.PrintF( _S( 995, "길드부장이 길드부장을 퇴출할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE:	// 길드전 중에는 가입신청 불가 
				strMessage.PrintF( _S( 1830, "가입하려는 길드가 길드 전쟁중일 때는 가입신청을 할 수 없습니다." )  ); 
				break;
			case MSG_GUILD_ERROR_CANNOT_BREAK_BATTLE:		// 갈드전 - 길드전 중 길드 해체 불가
				strMessage.PrintF( _S( 1831, "길드 전쟁중일 때는 길드를 해체할 수 없습니다." )  );	
				break;
			case MSG_GUILD_ERROR_CANNOT_KICK_BATTLE:		// 갈드전 - 길드전 중 퇴출 불가   
				strMessage.PrintF( _S( 1832, "길드 전쟁중일 때는 길드원을 퇴출할 수 없습니다." )  ); 
				break;
			case MSG_GUILD_ERROR_CANNOT_OUT_BATTLE:
				strMessage.PrintF( _S( 1833, "길드 전쟁중일 때는 길드를 탈퇴할 수 없습니다." )  );	 
				break;
			case MSG_GUILD_ERROR_CANNOT_BREAK_WAR:
				strMessage.PrintF( _S( 2077, "성주 길드는 길드체를 할 수 없습니다." )  );		
				break;
			// WSS_NEW_GUILD_SYSTEM 070704
			// ---------------------------------------------------------------->>
			case MSG_NEW_GUILD_ERRROR_GUILDPOINT:				// 길드 레벨업시 길드 포인트 부족 
				strMessage.PrintF( _S(3884, "길드 포인트가 부족합니다." )  );		
				break;

			case MSG_NEW_GUILD_ERROR_SAME_INCLNE:				// 길드 성향이 같음
				strMessage.PrintF( _S(3885, "길드 성향이 같습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_CHANG_INCLINE:				// 길드 성향 변경 실패
				strMessage.PrintF( _S(3886, "길드 성향 변경에 실패하였습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_INCLINE_OK:				// 길드 성향 변경 성공
				strMessage.PrintF( _S(3887, "길드 성향이 변경되었습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_ADJUST_FAIL:				// 길드 멤버들의  설정 변경 실패
				strMessage.PrintF( _S(3888, "길드 설정 적용에 실패하였습니다." )  );
				break;
			case MSG_NEW_GUILD_ERROR_ADJUST_OK:	
				_pUIMgr->GetGuild()->AdjustGuildMemberInfo();
				strMessage.PrintF( _S(3889, "길드 설정이 적용되었습니다." )  );
				break;		
			case MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_OK:			// 새로운 길드 공지 업데이트 성공
				strMessage.PrintF( _S(3890, "길드 공지 업데이트가 완료되었습니다." )  );		
				break;
			case MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_FAIL:		// 새로운 길드 공지 업데이트 실패
				strMessage.PrintF( _S(3891, "길드 공지 업데이트에 실패하였습니다." )  );		
				break;
			case MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_OK:	
				strMessage.PrintF( _S(3892, "길드 공지 메시지를 보냈습니다." )  );
				break;
			case MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL:
				strMessage.PrintF( _S(3893 , "길드 공지 메시지 전달에 실패하였습니다." )  );
				break;		
			case MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST:	
				strMessage.PrintF( _S(3894, "길드 스킬이 없습니다." )  );
				break;
			case MSG_NEW_GUILD_NOTICE_ERROR_NOT_MESSAGE:			
				strMessage.PrintF( _S(3895, "전달 메시지가 없습니다." )  );
				break;			
			case MSG_NEW_GUILD_SKILL_LEARN_ERROR_OK:
				strMessage.PrintF( _S(3896, "길드 스킬을 습득하였습니다." )  );
				break;			
			case MSG_NEW_GUILD_SKILL_LEARN_ERROR_LEVEL:  
				strMessage.PrintF( _S(3897, "길드 레벨이 부족합니다." )  );
				break;
			case MSG_NEW_GUILD_SKILL_ALEADY_LEARN: 
				strMessage.PrintF( _S(3897, "길드 레벨이 부족합니다." )  );
				break;

			// ----------------------------------------------------------------<<
			}

			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_KICK);						// 멤버 퇴출
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ADD_VICE_BOSS);				// 부단장 임명
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DEL_VICE_BOSS);				// 부단장 퇴출
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_CHANGE_BOSS);					// 단장 이임
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );		
			//_pUIMgr->GetGuild()->ResetGuild();
		}
		break;

	case MSG_GUILD_CREATE:		// 길드 생성
		{
			CPrintF("길드 생성\n");
		}
		break;

	case MSG_GUILD_INFO:		// 내 길드 정보 보내기
		{
			//MSG_GUILD_INFO,	// 내 길드 정보 보내기	: guildindex(n) guildname(str) guildlevel(n) mypos(n)
								// 길드 없으면 guildindex == -1, 이후 항목 생략
			LONG		lGuildIndex;			
			(*istr) >> lGuildIndex;

			_pNetwork->MyCharacterInfo.lGuildIndex = lGuildIndex;

			// 길드에 속해있지 않음.
			if( lGuildIndex == -1 )
			{
			}
			else
			{
				CTString	strGuildName, strGuildClassName;
				LONG		lGuildLevel;
				LONG		lMyPos;
				(*istr) >> strGuildName;
				(*istr) >> lGuildLevel;
				(*istr) >> lMyPos;
#ifdef DISPLAY_GUILD_RANK
				(*istr) >> _pNetwork->MyCharacterInfo.sbGuildRank;	
#endif
				(*istr) >> strGuildClassName;
// WSS_NEW_GUILD_SYSTEM 070731
#ifdef NEW_GUILD_SYSTEM
				SBYTE		sbGuildSkill444;	// 길드원 확장 스킬
				(*istr) >> sbGuildSkill444;
				_pUIMgr->GetGuild()->ResetGuildMaxMember((int)sbGuildSkill444);				
#endif

				_pNetwork->MyCharacterInfo.lGuildLevel		= lGuildLevel;
				_pNetwork->MyCharacterInfo.lGuildPosition	= lMyPos;
				_pNetwork->MyCharacterInfo.strGuildName		= strGuildName;
				_pNetwork->MyCharacterInfo.guildPosName		= strGuildClassName;
			}
		}
		break;
		
	case MSG_GUILD_MEMBERLIST:	// 길드원 리스트
		{
			// 길원 리스트			: guildindex(n) count(n) [index(n) name(str) position(n) online(c)]:count
			LONG lGuildIndex;
			LONG lCount;
			(*istr) >> lGuildIndex;
			(*istr) >> lCount;
			
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				_pUIMgr->GetGuild()->ResetGuild();
				_pUIMgr->GetGuild()->ClearMemberList();
			}
			
			for(int i = 0; i < lCount; ++i)
			{
				LONG		lIndex;
				CTString	strName;
				LONG		lPosition;
				SBYTE		sbOnline;
				
				(*istr) >> lIndex;
				(*istr) >> strName;
				(*istr) >> lPosition;
				(*istr) >> sbOnline;
				
				if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
				{
					_pUIMgr->GetGuild()->AddToMemberList( lIndex, strName, lPosition, sbOnline );
				}
				else
				{
					for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						if( ct.cha_Index == lIndex )
						{
							//ct.cha_sbShopType = sbShopType;
							//if( sbShopType == PST_NOSHOP )
							//	ct.ShopMsg.Reset();
							
							//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
							break;
						}
					}
				}
			}
		}
		break;
		
	case MSG_GUILD_ONLINE:		// 길드원 온라인 상태
		{
			LONG		lGuildIndex;
			LONG		lIndex;
			CTString	strName;
			//LONG		lPosition;
			SBYTE		sbOnline;
			
			(*istr) >> lGuildIndex;
			(*istr) >> lIndex;
			(*istr) >> strName;
			//(*istr) >> lPosition;
			(*istr) >> sbOnline;
			
			// 우리 길드원.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				if( _pNetwork->MyCharacterInfo.index != lIndex )
				{
					CTString strMessage;
					if( sbOnline == TRUE )
						strMessage.PrintF( _S( 996, "[%s] 길드원이 접속하셨습니다." ), strName);		
					else
						strMessage.PrintF( _S( 997, "[%s] 길드원이 접속종료 하셨습니다." ), strName);		
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				
				_pUIMgr->GetGuild()->ChangeMemberOnline( lIndex, sbOnline );

			// WSS_NEW_GUILD_SYSTEM 2007/08/07--------->><<
#ifdef NEW_GUILD_SYSTEM
				if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
				{
#endif
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						_pUIMgr->GetGuild()->RefreshMemberList( TRUE );
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						_pUIMgr->GetGuild()->RefreshApplicantList();
					}
#ifdef NEW_GUILD_SYSTEM
				}
#endif
			}
			// 다른 길드원.
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lIndex )
					{
						//ct.cha_sbShopType = sbShopType;
						//if( sbShopType == PST_NOSHOP )
						//	ct.ShopMsg.Reset();
						
						//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( ct.cha_iClientIndex, sbShopType );
						break;
					}
				}
			}
		}
		break;
		
	case MSG_GUILD_LEVELUP:		// 길드 승급
		{
		}
		break;
		
	case MSG_GUILD_LEVELINFO:	// 길드 레벨 정보
		{
			LONG lGuildIndex;
			LONG lGuildLevel;
			(*istr) >> lGuildIndex;
			(*istr) >> lGuildLevel;
			
			// my Character
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				_pNetwork->MyCharacterInfo.lGuildLevel = lGuildLevel;
			}
			// other Characters
			else
			{
				// 다른 캐릭터들의 길드 레벨을 변경시킴.
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_lGuildIndex == lGuildIndex )
					{
						ct.cha_lGuildLevel = lGuildLevel;
					}
				}
			}
			_pUIMgr->GetGuild()->ResetGuild();
		}
		break;
		
	case MSG_GUILD_BREAKUP:		// 길드 해체
		{
			//_pUIMgr->GetGuild()->ResetGuild();
			//_pUIMgr->GetGuild()->ClearMemberList();
		}
		break;
		
	case MSG_GUILD_BREAKUP_NOTIFY:
		{
			LONG		lGuildIndex;
			CTString	strGuildName;
			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
			
			for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
				if( ct.cha_lGuildIndex == lGuildIndex )
				{
					ct.cha_lGuildIndex		= -1;		// 임시	길드의 인덱스
					ct.cha_lGuildLevel		= -1;		// 임시 길드의 레벨
					ct.cha_lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
					ct.cha_strGuildName		= "";
				}
			}
			
			if( lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex)
			{			
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;
				
				CTString strMessage;
				strMessage.PrintF( _S( 998, "[%s] 길드가 해체되었습니다." ), strGuildName );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_KICK);						// 멤버 퇴출
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ADD_VICE_BOSS);				// 부단장 임명
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DEL_VICE_BOSS);				// 부단장 퇴출
				_pUIMgr->CloseMessageBox(MSGCMD_GUILD_CHANGE_BOSS);				// 단장 이임
				
				_pUIMgr->GetGuild()->ResetGuild();
				_pUIMgr->GetGuild()->ClearMemberList();

				((CUIWebBoard*)_pUIMgr->GetUI(UI_WEBBOARD))->NotifyGuildChange();
			}
		}
		break;
		
	case MSG_GUILD_REGIST_REQ:	// 가입 신청을 함.
		{
			LONG lGuildIndex;
			LONG lBossIndex;
			LONG lRequesterIndex;
			
			(*istr) >> lGuildIndex;
			(*istr) >> lBossIndex;
			(*istr) >> lRequesterIndex;
			
			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				_pUIMgr->GetGuild()->ApplicantAccept( lRequesterIndex );
		}
		break;
		
	case MSG_GUILD_REGIST_ALLOW:	// 길드 신청 확인
		{
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
		}
		break;
		
	case MSG_GUILD_REGIST_CANCEL:	// 길드 신청 취소
		{
			//m_bGuildJoinReq;
			
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
			CTString strMessage;
			strMessage.PrintF( _S( 999, "길드 가입 신청이 취소되었습니다." ) );			
			_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
		}
		break;
		
	case MSG_GUILD_CHANGE_BOSS:
		{
			// 단장 이임			: guildindex(n) current(n) newboss(n)
			LONG lGuildIndex;
			LONG lCharIndex;
			LONG lBossIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> lBossIndex;
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
#ifdef NEW_GUILD_SYSTEM
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				

				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
					_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );				
				}

				if( lBossIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_BOSS;		// 임시 길드에서의 캐릭터의 레벨
					_pUIMgr->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );
					CTString strMessage;
					strMessage.PrintF( _S( 1000, "길드장에 임명되었습니다." ) );			
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					
				}
				break;					
			}
#endif
			// ------------------------------------------------------------------------------<<
			// 보스 변경.
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );				
				const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
				_pUIMgr->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
						ct.cha_lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨						
						break;
					}
				}
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
			}
			
			if( lBossIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_BOSS;		// 임시 길드에서의 캐릭터의 레벨
				_pUIMgr->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );				
				const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
				_pUIMgr->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}

				CTString strMessage;
				strMessage.PrintF( _S( 1000, "길드장에 임명되었습니다." ) );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lBossIndex )
					{
						ct.cha_lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
						ct.cha_lGuildPosition	= GUILD_MEMBER_BOSS;		// 임시 길드에서의 캐릭터의 레벨
						break;
					}
				}
				_pUIMgr->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );
			}

			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
			{
				_pUIMgr->GetGuild()->RefreshMemberList( TRUE );
			}
			else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
			{
				_pUIMgr->GetGuild()->RefreshApplicantList();
			}

		}
		break;

	case MSG_GUILD_APPOINT_OFFICER:
		{
			// 부단장 임명			: guildindex(n) charindex(n)
			LONG lGuildIndex;
			LONG lCharIndex;			
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;

			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
#ifdef NEW_GUILD_SYSTEM
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨				

					CTString strMessage;
					strMessage.PrintF( _S( 1001, "길드부장에 임명되었습니다." ) );				
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				else 
				{
					for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						if( ct.cha_Index == lCharIndex )
						{
							ct.cha_lGuildIndex		= lGuildIndex;					// 임시	길드의 인덱스
							ct.cha_lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨						
							break;
						}
					}					
				}
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );
				break;
			}
#endif
			// ------------------------------------------------------------------------------<<
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨				
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );
				const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
				_pUIMgr->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}
				
				CTString strMessage;
				strMessage.PrintF( _S( 1001, "길드부장에 임명되었습니다." ) );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= lGuildIndex;					// 임시	길드의 인덱스
						ct.cha_lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨						
						break;
					}
				}
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );
			}

			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
			{
				_pUIMgr->GetGuild()->RefreshMemberList( TRUE );
			}
			else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
			{
				_pUIMgr->GetGuild()->RefreshApplicantList();
			}
		}
		break;

	case MSG_GUILD_FIRE_OFFICER:
		{
			// 부단장 해임			: guildindex(n) charindex(n)
			LONG lGuildIndex;
			LONG lCharIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
#ifdef NEW_GUILD_SYSTEM
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
					CTString strMessage;
					strMessage.PrintF( _S( 1002, "길드부장에서 해임되었습니다." ) );		
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				else
				{
					for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						if( ct.cha_Index == lCharIndex )
						{
							ct.cha_lGuildIndex		= lGuildIndex;					// 임시	길드의 인덱스
							ct.cha_lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨						
							break;
						}
					}
				}			
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
				break;
			}
#endif
			// ------------------------------------------------------------------------------<<

			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
				const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
				_pUIMgr->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}

				CTString strMessage;
				strMessage.PrintF( _S( 1002, "길드부장에서 해임되었습니다." ) );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= lGuildIndex;					// 임시	길드의 인덱스
						ct.cha_lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨						
						break;
					}
				}
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
			}

			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
			{
				_pUIMgr->GetGuild()->RefreshMemberList( TRUE );
			}
			else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
			{
				_pUIMgr->GetGuild()->RefreshApplicantList();
			}
		}
		break;	
	case MSG_GUILD_MEMBER_ADD:		// 멤버 추가
		{
			LONG lGuildIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
#ifdef NEW_GUILD_SYSTEM
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{	
				CTString strMessage;
				strMessage.PrintF( _S( 1003, "[%s] 길드원이 가입하였습니다." ), strCharName );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				break;
			}
#endif
			// 내 길드에 추가되는 경우.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				_pUIMgr->GetGuild()->AddToMemberList( lCharIndex, strCharName, GUILD_MEMBER_MEMBER, TRUE );			

				if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
				{
					_pUIMgr->GetGuild()->RefreshApplicantList();
				}
				else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
					_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				{
					_pUIMgr->GetGuild()->RefreshMemberList();
				}
				
				CTString strMessage;
				strMessage.PrintF( _S( 1003, "[%s] 길드원이 가입하였습니다." ), strCharName );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_MEMBER_OUT:		// 멤버 탈퇴
		{
			LONG lGuildIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;	
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;		// 임시 길드의 레벨
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= "";

				((CUIWebBoard*)_pUIMgr->GetUI(UI_WEBBOARD))->NotifyGuildChange();
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= -1;		// 임시	길드의 인덱스
						ct.cha_lGuildLevel		= -1;		// 임시 길드의 레벨
						ct.cha_lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
						ct.cha_strGuildName		= "";
						break;
					}
				}
			}			
						
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
#ifdef NEW_GUILD_SYSTEM
				if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
				{	
#endif
					_pUIMgr->GetGuild()->DelFromMemberList( lCharIndex );
					
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						_pUIMgr->GetGuild()->RefreshApplicantList();
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						_pUIMgr->GetGuild()->RefreshMemberList();
					}
#ifdef NEW_GUILD_SYSTEM
				}
#endif
				CTString strMessage;
				strMessage.PrintF( _S( 1004, "[%s] 길드원이 탈퇴하였습니다." ), strCharName );			

				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_MEMBER_KICK:		// 멤버 강퇴
		{
			// guildindex(n) bossindex(n) charindex(n) charname(str)
			
			LONG lGuildIndex;
			LONG lBossIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lBossIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				// 자신이 퇴출된거라면...				
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;		// 임시 길드의 레벨
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= "";
				_pUIMgr->GetGuild()->ResetGuild();
				_pUIMgr->GetGuild()->ClearMemberList();

				CTString strMessage;
				strMessage.PrintF( _S( 1005, "길드에서 퇴출되었습니다." ) );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );

				((CUIWebBoard*)_pUIMgr->GetUI(UI_WEBBOARD))->NotifyGuildChange();
				return;
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= -1;		// 임시	길드의 인덱스
						ct.cha_lGuildLevel		= -1;		// 임시 길드의 레벨
						ct.cha_lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
						ct.cha_strGuildName		= "";
						break;
					}
				}
			}
			
			// 내가 갖고 있는 길드 정보에서 해당 캐릭터 정보를 제거함.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{

			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
#ifdef NEW_GUILD_SYSTEM
				if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
				{		
#endif
					_pUIMgr->GetGuild()->DelFromMemberList( lCharIndex );
					
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						_pUIMgr->GetGuild()->RefreshApplicantList();
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						_pUIMgr->GetGuild()->RefreshMemberList();
					}
#ifdef NEW_GUILD_SYSTEM
				}
#endif
				CTString strMessage;
				strMessage.PrintF( _S( 1006, "[%s] 길드원을 퇴출하였습니다." ), strCharName );			
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_INFO_CHANGE:		// 길드 정보 변경.
		{
			LONG		lCharIndex;
			LONG		lGuildIndex;
			CTString	strGuildName;
			LONG		lCharPos;			
			(*istr) >> lCharIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> lCharPos;
#ifdef DISPLAY_GUILD_RANK
			SBYTE		sbGuildRank;
			(*istr) >> sbGuildRank;
#endif
			
			LONG		lOldPos = _pNetwork->MyCharacterInfo.lGuildPosition;
			
			// 길드 상에서의 내 위치나 길드명이 변경된 경우...
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스				
				_pNetwork->MyCharacterInfo.lGuildPosition	= lCharPos;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= strGuildName;
#ifdef DISPLAY_GUILD_RANK
				_pNetwork->MyCharacterInfo.sbGuildRank		= sbGuildRank;
#endif
			}
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
						ct.cha_lGuildPosition	= lCharPos;			// 임시 길드에서의 캐릭터의 레벨
						ct.cha_strGuildName		= strGuildName;
#ifdef DISPLAY_GUILD_RANK
						ct.cha_sbGuildRank		= sbGuildRank;
#endif
						break;
					}
				}
			}
			
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				_pUIMgr->GetGuild()->ChangeMemberLevel( lCharIndex, lCharPos );				
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
#ifdef NEW_GUILD_SYSTEM
			if(_pNetwork->MyCharacterInfo.lGuildLevel < LIMIT_GUILD_LEVEL)
			{		
#endif				
				// 상태에 변경이 없음.
				if( lOldPos == _pNetwork->MyCharacterInfo.lGuildPosition )
				{
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						_pUIMgr->GetGuild()->RefreshApplicantList( );
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						_pUIMgr->GetGuild()->RefreshMemberList( );
					}
				}
				else
				{
					// 멤버에서 단장 혹은 부단장으로 변경.
					if( lOldPos == GUILD_MEMBER_MEMBER && 
						( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS || 
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS ) )
					{
						const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
						_pUIMgr->GetGuild()->ResetGuild();
						if( bVisible )
						{								
							_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
						}
					}
					else if( ( lOldPos == GUILD_MEMBER_BOSS || lOldPos == GUILD_MEMBER_VICE_BOSS ) )						
					{
						// 단장 혹은 부단장에서 일반 멤버로...
						if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
						{
							const BOOL bVisible = _pUIMgr->GetGuild()->IsVisible();
							_pUIMgr->GetGuild()->ResetGuild();
							if( bVisible )
							{								
								_pUIMgr->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
							}
						}
					}
				}
#ifdef NEW_GUILD_SYSTEM
			}
#endif
			}
		}
		break;
		// Guild Battle 
	// 길드전 신청 요청 받음
	case MSG_GUILD_BATTLE_REQ_REQ: 
		{
			LONG		lGuildIndex;
			CTString	strGuildName;
			LONG		lPrize;
			LONG		lTime;

			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> lPrize;
			(*istr) >> lTime;

			
			_pUIMgr->GetGuildBattle()->GBReq( lGuildIndex, strGuildName, lPrize, lTime );
		}
		break;

	// 길드전 신청 거절 받음
	case MSG_GUILD_BATTLE_REQ_REJECT: 
		{
			LONG lRejectCharIndex;
			(*istr) >> lRejectCharIndex;
			
			_pUIMgr->GetGuildBattle()->GBReqReject( lRejectCharIndex );
	
		}
		break;
		
	// 길드전 신청 허락	받음
	case MSG_GUILD_BATTLE_REQ_ACCEPT:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
			LONG		lZone;
			LONG		lTime;
			
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
			(*istr) >> lZone;
			(*istr) >> lTime;

					
			_pUIMgr->GetGuildBattle()->GBReqAccept( lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize, lZone, lTime );
		}
		break;

	// 길드전 중단 요청	받음 
	case MSG_GUILD_BATTLE_STOP_REQ:
		{
			LONG		lGuildIndex;
			CTString	strGuildName;

			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
						
			_pUIMgr->GetGuildBattle()->GBStopReqRev( lGuildIndex, strGuildName );
		}
		break;

	// 길드전 중단 거절 받음 
	case MSG_GUILD_BATTLE_STOP_REJECT:
		{
			LONG lRejectCharIndex;
			(*istr) >> lRejectCharIndex;
			
			_pUIMgr->GetGuildBattle()->GBStopReqReject( lRejectCharIndex );
		}
		break;

	// 길드전 중단 허락 받음 
	case MSG_GUILD_BATTLE_STOP_ACCEPT:
		{
			_pUIMgr->GetGuildBattle()->GBStopReqAccept();
		}
		break;

	// 길드전 시작 알림
	case MSG_GUILD_BATTLE_START:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
			LONG		lZone;
			LONG		lTime;
			
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
			(*istr) >> lZone;
			(*istr) >> lTime;

			_pUIMgr->GetGuildBattle()->GBStart ( lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize, lZone, lTime );
			
		}
		break;

	// 길드전 현황			
	case MSG_GUILD_BATTLE_STATUS:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lCount1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lCount2;
			LONG		lTime;
			LONG		lZone;

			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lCount1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lCount2;			
			(*istr) >> lTime;
			(*istr) >> lZone;
			
			_pUIMgr->GetGuildBattle()->GBStatus( lGuildIndex1, strGuildName1, lCount1, 
												lGuildIndex2, strGuildName2, lCount2, lTime, lZone );
		}
		break;

	// 길드전 종료 	
	case MSG_GUILD_BATTLE_END:
		{
			LONG		lWinnerGuildIndex;
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
	
			(*istr) >> lWinnerGuildIndex;
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
		
			_pUIMgr->GetGuildBattle()->GBEnd( lWinnerGuildIndex, lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize );
		}
		break;

// Date : 2005-07-06(오후 2:41:11), By Lee Ki-hwan
// 공성전 관련 메세지 처리 
	case MSG_GUILD_WAR_JOIN_ATTACK_GUILD:
	case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD:
	case MSG_GUILD_WAR_JOIN_ATTACK_CHAR:	
	case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR:
		{
			LONG		nErrorCode;
			(*istr) >> nErrorCode;
			_pUISWDoc->ErrorProc( nErrorCode, ubType );
		}
		break;
	case MSG_GUILD_WAR_GET_TIME:
		{
			LONG		nErrorCode;
			LONG		nGuildIndex;
			CTString	strGuildName;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nErrorCode;
			(*istr) >> nGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;
			
			if( nErrorCode != MSG_GUILD_ERROR_OK )	_pUISWDoc->ErrorProc( nErrorCode );
			else
			{
				_pUISWDoc->GetTime( nGuildIndex, strGuildName, nMonth, nDay, nHour, nMin );
			}
			
		}
		break;
	case MSG_GUILD_WAR_START:
		{
			LONG		nZoneIndex;
			LONG		nRemainSec;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;
			
			_pUISWDoc->GuildWarStart( nZoneIndex, nRemainSec );	
		}
		break;
	case MSG_GUILD_WAR_NOTICE_START_CASTLE:
		{
			LONG		nZoneIndex;
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;
			(*istr) >> nGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> nGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> nGuildIndex3;
			(*istr) >> strGuildName3;

			_pUISWDoc->StartCastle( nZoneIndex, nRemainSec, nGuildIndex1, strGuildName1, nGuildIndex2, strGuildName2, nGuildIndex3, strGuildName3 );
		}
		break;
	case MSG_GUILD_WAR_POINT:
		{
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildPoint1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildPoint2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;
			LONG		nGuildPoint3;
			LONG		nMyPoint;
			LONG		nDefensPoint;

			(*istr) >> nRemainSec;		
			(*istr) >> nGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> nGuildPoint1;
			(*istr) >> nGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> nGuildPoint2;
			(*istr) >> nGuildIndex3;
			(*istr) >> strGuildName3;
			(*istr) >> nGuildPoint3;
			(*istr) >> nMyPoint;
			(*istr) >> nDefensPoint;

			SGuild Guild1, Guild2, Guild3;

			Guild1.SetGuildInfo( nGuildIndex1, strGuildName1, nGuildPoint1 );
			Guild2.SetGuildInfo( nGuildIndex2, strGuildName2, nGuildPoint2 );
			Guild3.SetGuildInfo( nGuildIndex3, strGuildName3, nGuildPoint3 );

			_pUISWDoc->UpdatePoint( nRemainSec, Guild1, Guild2, Guild3, nMyPoint, nDefensPoint );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME:
		{
			LONG		nRemainSec;
			LONG		nZoneIndex;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;					
	
			_pUISWDoc->RemainFiledTime( nZoneIndex, nRemainSec );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_TIME:
		{
			LONG		nZoneIndex;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nZoneIndex;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;
			
			_pUISWDoc->TimeConfirm( nZoneIndex, nMonth, nDay, nHour, nMin );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_TIME_REMAIN:
		{
			LONG		nZoneIndex;
			SBYTE		nMin;
		
			(*istr) >> nZoneIndex;
			(*istr) >> nMin;		

			_pUISWDoc->RemainStartTime( nZoneIndex, nMin );
		}
		break;
	case MSG_GUILD_WAR_CASTLE_STATE:
		{
			LONG		nZoneIndex;
			LONG		nState;
			LONG		nGateState;
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildPoint1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildPoint2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;
			LONG		nGuildPoint3;
			LONG		nMyPoint;
			LONG		nDefensPoint;

			(*istr) >> nZoneIndex;		
			(*istr) >> nState;		
			(*istr) >> nGateState;	
			(*istr) >> nRemainSec;
// WSS_DRATAN_SIEGEWARFARE 070723 ------------------>><<
// 드라탄 공성은 포인트 관련 데이터를 받지 않는다.
			if( nZoneIndex == 7)
			{			
				(*istr) >> nGuildIndex1;
				(*istr) >> strGuildName1;
				(*istr) >> nGuildPoint1;
				(*istr) >> nGuildIndex2;
				(*istr) >> strGuildName2;
				(*istr) >> nGuildPoint2;
				(*istr) >> nGuildIndex3;
				(*istr) >> strGuildName3;
				(*istr) >> nGuildPoint3;
				(*istr) >> nMyPoint;
				(*istr) >> nDefensPoint;

				SGuild Guild1, Guild2, Guild3;
				
				Guild1.SetGuildInfo( nGuildIndex1, strGuildName1, nGuildPoint1 );
				Guild2.SetGuildInfo( nGuildIndex2, strGuildName2, nGuildPoint2 );
				Guild3.SetGuildInfo( nGuildIndex3, strGuildName3, nGuildPoint3 );

				_pUISWDoc->CastleState( nZoneIndex, nState, nGateState, nRemainSec, Guild1, Guild2, Guild3, nMyPoint, nDefensPoint );
			}
			else if( nZoneIndex ==4 )
			{
				_pUISWDoc->CastleState( nZoneIndex, nState, nGateState, nRemainSec);
			}


		}
		break;
	case MSG_GUILD_WAR_GATE_STATE:
		{
			LONG lOldGate;
			LONG lNewGate;
			(*istr) >> lOldGate;
			(*istr) >> lNewGate;

			_pUISWDoc->SetGateState( lOldGate, lNewGate );
		
		}
		break;

	case MSG_GUILD_WAR_ATTACK_SYMBOL:
		{
			LONG lChaIndex;
			(*istr) >> lChaIndex;
		}
		break;
		
	case MSG_GUILD_WAR_END:
		{
			LONG		nZoneIndex; 
			BYTE		nWinDefense; 
			LONG		nOwnerGuildIndex; 
			CTString	strOwnerGuildName; 
			LONG		nOwnerIndex; 
			CTString	strOwnerName; 
			LONG		nNextMonth; 
			LONG		nNextDay; 
			LONG		nNextWeek; 
			LONG		nNextHour;

			(*istr) >> nZoneIndex; 
			(*istr) >> nWinDefense; 
			(*istr) >> nOwnerGuildIndex;
			(*istr) >> strOwnerGuildName;
			(*istr) >> nOwnerIndex; 
			(*istr) >> strOwnerName; 
			(*istr) >> nNextMonth; 
			(*istr) >> nNextDay; 
			(*istr) >> nNextHour;
			(*istr) >> nNextWeek; 

			_pUISWDoc->EndWar( nZoneIndex, nWinDefense, nOwnerGuildIndex, strOwnerGuildName, nOwnerIndex, strOwnerName, nNextMonth, nNextDay, nNextWeek, nNextHour );
		}
		break;
	case MSG_GUILD_WAR_SET_TIME_REP:
		{
			LONG		nErrorCode;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nErrorCode;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;


			if( nErrorCode != MSG_GUILD_ERROR_OK ) _pUISWDoc->ErrorProc( nErrorCode );
			else
			{
				_pUISWDoc->SetTimeRep( nMonth, nDay, nHour, nMin );
			}

		}
		break;
	case MSG_GUILD_WAR_INVALID_COMMAND:
		{
			BYTE		nReqMsgType;
			(*istr) >> nReqMsgType;
						
			_pUISWDoc->InvalidCommand( nReqMsgType );
		}
		break;

	// Date : 2005-09-06(오전 11:03:59), By Lee Ki-hwan 
	case MSG_GUILD_STASH_HISTORY_REP:
		{
			LONG	nErrorCode;

			LONG		lMonth, lDay;
			LONGLONG	llMoney;

			(*istr) >> nErrorCode;
			
			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				_pUIMgr->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}
			
			_pUIMgr->GetGuildStash()->ResetTakeInfo();
			for( int i = 0; i < 7; i++ )
			{
				(*istr) >> lMonth;
				(*istr) >> lDay;
				(*istr) >> llMoney;

				_pUIMgr->GetGuildStash()->AddTakeInfo( lMonth, lDay, llMoney );					
			}
			_pUIMgr->GetGuildStash()->OpenView();

		}
		break;

	case MSG_GUILD_STASH_VIEW_REP:
		{
			
			LONG	nErrorCode;
			LONGLONG	llMoney;

			(*istr) >> nErrorCode;

			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				_pUIMgr->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}

			(*istr) >> llMoney;

			_pUIMgr->GetGuildStash()->OpenTake( llMoney );
		}
		break;

	case MSG_GUILD_STASH_TAKE_REP:
		{
			LONG	nErrorCode;
			(*istr) >> nErrorCode;

			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				_pUIMgr->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}

			_pUIMgr->GetGuildStash()->Message( MSGCMD_GUILDSTASH_INFO, _S( 2078, "금액이 성공적으로 인출 되었습니다. "), UMBS_OK ); 

		}
		break;

	case MSG_NEW_GUILD_INCLINE_ESTABLISH: 		
	case MSG_NEW_GUILD_INFO:
	case MSG_NEW_GUILD_MEMBERLIST:
	case MSG_NEW_GUILD_SKILL:	
	case MSG_NEW_GUILD_NOTICE:
	case MSG_NEW_GUILD_NOTICE_TRANSMISSION:
	case MSG_NEW_GUILD_MANAGE:
	case MSG_NEW_GUILD_SKILL_LEARN:
	case MSG_NEW_GUILD_POINT_RANKING:
		_pUIMgr->GetGuild()->ReceiveNewGuildMessage(ubType,istr);
		break;

	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveTeachMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveTeachMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch(ubType)
	{
	case MSG_TEACH_TEACHER_LIST:		// 선생 리스트
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_TEACH_TEACHER_LIST_UP:		// 나를 선생 목록에 올림
				{
					_pUIMgr->GetHelper()->SetRegistredTeacher( TRUE );

					CTString strMessage;
					strMessage.PrintF( _S( 1347, "[%s]님이 후견인 목록에 추가되었습니다." ), _pNetwork->MyCharacterInfo.name );		
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				break;
			case MSG_TEACH_TEACHER_LIST_DN:		// 나를 선생 목록에서 내림
				{
					_pUIMgr->GetHelper()->SetRegistredTeacher( FALSE );

					CTString strMessage;
					strMessage.PrintF( _S( 1348, "[%s]님이 후견인 목록에서 삭제되었습니다." ), _pNetwork->MyCharacterInfo.name );		
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				break;
			case MSG_TEACH_TEACHER_LIST_SHOW:
				{
					LONG		lCharIndex;
					CTString	strCharName;
					SBYTE		sbCharJob;
					SBYTE		sbCharJob2;

					_pUIMgr->GetHelper()->ClearHelperList();

					for( int i = 0; i < HELPER_MAX_STUDENTS; ++i )
					{
						(*istr) >> lCharIndex;

						if( lCharIndex != -1 )
						{
							(*istr) >> strCharName;
							(*istr) >> sbCharJob;
							(*istr) >> sbCharJob2;

							SLONG fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent;
							(*istr) >> fame;
							(*istr) >> cntTeachingStudent;
							(*istr) >> cntCompleteStudent;
							(*istr) >> cntFailStudent;

							_pUIMgr->GetHelper()->AddToHelperList( lCharIndex, strCharName, -1, sbCharJob, sbCharJob2 );
							_pUIMgr->GetHelper()->AddToTeacherInfoList(lCharIndex, fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent);
						}
					}
					_pUIMgr->GetHelper()->RefreshTeacherList();
				}
				break;
			}
		}
		break;

	case MSG_TEACH_TEACHER_REQ:		// 선생 요청 과정
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_TEACH_TEACHER_REQ_REQ:
				{
					LONG		lCharIndex;
					CTString	strCharName;

					(*istr) >> lCharIndex;
					(*istr) >> strCharName;
					_pUIMgr->GetHelper()->TeacherAccept( lCharIndex, strCharName );
				}
				break;
			case MSG_TEACH_TEACHER_REQ_REJECT:
				{
					//UBYTE		ubCharType;
					LONG		lRejectCharIndex;
					CTString	strCharName;

					_pUIMgr->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );
					_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

					//(*istr) >> ubCharType;
					(*istr) >> lRejectCharIndex;
					(*istr) >> strCharName;
					// 선생 요청 거절	: char_type(uc) reject_charindex(n) reject_charname(str)

					if( lRejectCharIndex == _pNetwork->MyCharacterInfo.index )	// 내가 거절했을때...
					{
					}
					else		// 다른 사람이 거절했을때...
					{
						CTString strMessage;
						strMessage.PrintF( _S( 1349, "[%s]님이 거절하였습니다." ), strCharName );		
						_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
					
					//_pNetwork->MyCharacterInfo.lTeacherIndex = -1;
				}
				break;

			case MSG_TEACH_TEACHER_REQ_ACCEPT:
				{
					LONG		lTeacherIndex;
					CTString	strTeacherName;
					LONG		lStudentIndex;
					CTString	strStudentName;

					_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

					(*istr) >> lTeacherIndex;
					(*istr) >> strTeacherName;
					(*istr) >> lStudentIndex;
					(*istr) >> strStudentName;
					// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)

					CTString strMessage;
					if( lStudentIndex == _pNetwork->MyCharacterInfo.index )
					{
						// 내가 학생인데, 선생이 생긴경우...
						strMessage.PrintF( _S( 1350, "[%s]님이 [%s]님의 후견인이 되었습니다." ), strTeacherName, strStudentName );		
						_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
					else
					{						
						strMessage.PrintF( _S( 1351, "[%s]님이 [%s]님의 견습생이 되었습니다." ), strStudentName, strTeacherName );		
						_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
				}
				break;
			case MSG_TEACH_TEACHER_GIVEUP:
				{
					// 학생 포기 요청	: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
					LONG		lTeacherIndex;
					CTString	strTeacherName;
					LONG		lStudentIndex;
					CTString	strStudentName;
					(*istr) >> lTeacherIndex;
					(*istr) >> strTeacherName;
					(*istr) >> lStudentIndex;
					(*istr) >> strStudentName;

					if( lStudentIndex == _pNetwork->MyCharacterInfo.index )
					{
						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
						_pUIMgr->GetHelper()->ClearHelperList();
					}

					CTString strMessage;
					strMessage.PrintF( _S( 1352, "[%s]님이 [%s]님의 견습생을 포기하였습니다." ), strStudentName, strTeacherName );		
					_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );

					_pUIMgr->GetHelper()->DelFromHelperList( lStudentIndex );
					_pUIMgr->GetHelper()->RefreshStudentList();
				}
				break;
			}
		}
		break;
	case MSG_TEACH_END:			// 종료
		{
			LONG		lTeacherIndex;
			CTString	strTeacherName;
			LONG		lStudentIndex;
			CTString	strStudentName;
			UBYTE		ubEndType;
			SBYTE		sbEvent;

			(*istr) >> lTeacherIndex;
			(*istr) >> strTeacherName;
			(*istr) >> lStudentIndex;
			(*istr) >> strStudentName;
			(*istr) >> ubEndType;
			(*istr) >> sbEvent;

			switch( ubEndType )
			{
			case MSG_TEACH_END_FAIL:		// 실패
				{
					CTString strMessage;
					
					_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
					
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 1353, "견습생 양성 실패" ), UMBS_OK, UI_HELPER, MSGCMD_HELPER_NOTIFY );	
					strMessage.PrintF( _S( 1354, "10일동안 [%s]님의 레벨이 %d이 되지못해 후견인 관계가 강제 종료되었습니다." ), strStudentName, _pUIMgr->GetHelper()->GetLimitLevel() );	
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					
					// 내가 선생이었을 때...
					if( lTeacherIndex == _pNetwork->MyCharacterInfo.index )
					{
						_pUIMgr->GetHelper()->DelFromHelperList( lStudentIndex );
						_pUIMgr->GetHelper()->RefreshStudentList();
					}
					// 내가 학생이었을 때...
					else
					{
						_pUIMgr->GetHelper()->DelFromHelperList( lTeacherIndex );
						_pUIMgr->GetHelper()->RefreshTeacherList();
						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
					}
				}
				break;

			case MSG_TEACH_END_SUCCESS:		// 성공
				{
					CTString strMessage;

					_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );

					CUIMsgBox_Info	MsgBoxInfo;

// [KH_070417] 스승의날 이벤트 관련 추가
					if(IS_EVENT_ON(TEVENT_TEACHER_2007))
						MsgBoxInfo.SetMsgBoxInfo(_S(3344, "스승의 은혜 이벤트"), UMBS_OK, UI_NONE,MSGCMD_NULL);
					else
						MsgBoxInfo.SetMsgBoxInfo( _S( 1355, "견습생 양성 성공" ), UMBS_OK, UI_HELPER, MSGCMD_HELPER_NOTIFY );	
					
					// 내가 선생이었을 때...
					if( lTeacherIndex == _pNetwork->MyCharacterInfo.index )
					{	
						int iPlusFame	= 0;
						// FIXME : 이렇게 엔진 버젼 정보를 체크하는거 맘에 안듬...						
						extern UINT g_uiEngineVersion;
						if( g_uiEngineVersion >= 10000 )
							iPlusFame = 20;
						else						
							iPlusFame = 10;

						if( sbEvent == 1 )
						{
#ifndef TEACHER_PRIZE_EVENT_2PAN4PAN		// 후견인 명성 보상 이벤트가 아닌경우.
							strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다.  명성치가 +%d 이 됩니다." ), strStudentName, _pUIMgr->GetHelper()->GetLimitLevel(), iPlusFame*2 );	
#else
							iPlusFame = 20;
// [KH_070417] 스승의날 이벤트 관련 추가 (혹시 몰라 넣음)
							if(IS_EVENT_ON(TEVENT_TEACHER_2007))
								MsgBoxInfo.AddString(_S(3346, "축하합니다. 견습생 양성을 성공적으로 이루어 내 노란 카네이션을 획득 하였습니다."));
							else
								strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다.  명성치가 +%d 이 됩니다." ), strStudentName, _pUIMgr->GetHelper()->GetLimitLevel(), iPlusFame );	
#endif							
						}
						else
						{
// [KH_070417] 스승의날 이벤트 관련 추가
							if(IS_EVENT_ON(TEVENT_TEACHER_2007))
								MsgBoxInfo.AddString(_S(3346, "축하합니다. 견습생 양성을 성공적으로 이루어 내 노란 카네이션을 획득 하였습니다."));
							else
								strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다.  명성치가 +%d 이 됩니다." ), strStudentName, _pUIMgr->GetHelper()->GetLimitLevel(), iPlusFame );	
						}
						
						_pUIMgr->GetHelper()->DelFromHelperList( lStudentIndex );
						_pUIMgr->GetHelper()->RefreshStudentList();
					}
					// 내가 학생이었을 때...
					else
					{	
#ifndef TEACHER_PRIZE_EVENT_2PAN4PAN		// 후견인 명성 보상 이벤트가 아닌경우.
						if( sbEvent == 1 )
						{
							// NOTE : 이벤트일때...
							strMessage.PrintF( _S(2079, "후견인 시스템을 통하여 성공적으로 성장하였으므로 이벤트검이 지급됩니다.") );		
						}
						else
						{
							strMessage.PrintF( _S( 1357, "레벨이 %d이 되어 [%s]님과의 후견인 관계가 종료 되었습니다." ), _pUIMgr->GetHelper()->GetLimitLevel(), strTeacherName );	
						}
#else
// [KH_070417] 스승의날 이벤트 관련 추가
						if(IS_EVENT_ON(TEVENT_TEACHER_2007))
							MsgBoxInfo.AddString(_S(3347, "축하합니다. 레벨 20을 달성하여 스승의날 이벤트 노란 견장과 이벤트검을 획득 하였습니다."));
						else
							strMessage.PrintF( _S(2252,"축하합니다.  후견인 시스템을 성공적으로 완수하셨습니다.  보상으로 이벤트검이 지급됩니다."), _pUIMgr->GetHelper()->GetLimitLevel(), strTeacherName );	
#endif
						_pUIMgr->GetHelper()->DelFromHelperList( lTeacherIndex );
						_pUIMgr->GetHelper()->RefreshTeacherList();
						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
					}
					
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	case MSG_TEACH_INFO:		// 사제 정보
		{
			// 자신이 선생인지 학생인지에 따라서...
			SBYTE		sbRegistered;
			SBYTE		sbCharType;
			LONG		lTeacherIndex;
			LONG		lTeacherLevel;
			CTString	strTeacherName;
			SBYTE		sbTeacherJob;
			SBYTE		sbTeacherJob2;
			
			(*istr) >> sbRegistered;
			(*istr) >> sbCharType;			
							
			// 후견인으로 등록되어 있는 경우.				
			_pUIMgr->GetHelper()->SetRegistredTeacher( sbRegistered );
			
			// 자신이 학생일 경우, 이미 후견인 있음.
			if( sbCharType == MSG_TEACH_STUDENT_TYPE )
			{
				(*istr) >> lTeacherIndex;
				(*istr) >> lTeacherLevel;
				(*istr) >> strTeacherName;
				(*istr) >> sbTeacherJob;
				(*istr) >> sbTeacherJob2;
				
				_pNetwork->MyCharacterInfo.lTeacherIndex	= lTeacherIndex;
				_pNetwork->MyCharacterInfo.strTeacherName	= strTeacherName;

				SLONG fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent;
				(*istr) >> fame;
				(*istr) >> cntTeachingStudent;
				(*istr) >> cntCompleteStudent;
				(*istr) >> cntFailStudent;

				_pUIMgr->GetHelper()->ClearHelperList();
				_pUIMgr->GetHelper()->AddToHelperList( lTeacherIndex, strTeacherName, lTeacherLevel, sbTeacherJob, sbTeacherJob2 );
				_pUIMgr->GetHelper()->AddToTeacherInfoList(lTeacherIndex, fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent);
				_pUIMgr->GetHelper()->RefreshTeacherList();
			}
			// 자신이 선생일 경우.
			else if( sbCharType == MSG_TEACH_TEACHER_TYPE )
			{
				// 학생들 목록을 가져옴.
				//_pUIMgr->GetHelper()->ClearHelperList();
				
				for( int i = 0; i < 8; ++i )
				{
					LONG		lStudentIndex;
					LONG		lStudentLevel;
					CTString	strStudentName;
					SBYTE		sbStudentJob;
					SBYTE		sbStudentJob2;
					
					(*istr) >> lStudentIndex;
					
					if( lStudentIndex != -1 )
					{
						(*istr) >> lStudentLevel;
						(*istr) >> strStudentName;
						(*istr) >> sbStudentJob;
						(*istr) >> sbStudentJob2;
						CTString strFirstDate, strFinalDate;
						(*istr) >> strFirstDate;
						(*istr) >> strFinalDate;
						//char firstDate[50], finalDate[50];
						//istr->Read(firstDate, 50);
						//istr->Read(finalDate, 50);
						_pUIMgr->GetHelper()->AddToHelperList( lStudentIndex, strStudentName, lStudentLevel, sbStudentJob, sbStudentJob2 );
						_pUIMgr->GetHelper()->AddToStudentInfoList(lStudentIndex, strFirstDate.str_String, strFinalDate.str_String);
					}
				}
				_pUIMgr->GetHelper()->RefreshStudentList();
			}
		}
		break;
	case MSG_TEACH_STUDENT_LEVELUP:		// 학생 레벨업
		{
			// FIXME : 레벨만 변경되는게 보장 되어야 함.
			LONG		lStudentIndex;
			LONG		lStudentLevel;
			CTString	strStudentName;
			SBYTE		sbStudentJob;
			SBYTE		sbStudentJob2;

			(*istr) >> lStudentIndex;
			(*istr) >> lStudentLevel;
			(*istr) >> strStudentName;
			(*istr) >> sbStudentJob;
			(*istr) >> sbStudentJob2;

			_pUIMgr->GetHelper()->ChangeHelperLevel( lStudentIndex, lStudentLevel );
			_pUIMgr->GetHelper()->RefreshStudentList( );

			// 내가 해당 학생인경우...
			if( lStudentIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(2080,"후견인 보상"), UMBS_OK, UI_HELPER, MSGCMD_HELPER_NOTIFY );			
				MsgBoxInfo.AddString( _S(2081,"후견인 시스템을 통해 레벨 업을 하여 10,000나스를 지급 받았습니다.") );	
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
	case MSG_TEACH_ERR:			// 에러
		{
			UBYTE ubErrorType;
			CTString strMessage;

			_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
			_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
			_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

			(*istr) >> ubErrorType;

			switch( ubErrorType )
			{
			case MSG_TEACH_ERR_SYSTEM:					// 나오면 안되는 오류
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			case MSG_TEACH_ERR_TEACHER_LEVEL:			// 선생 가능레벨이 아님
				strMessage.PrintF( _S( 1358, "후견인으로 등록 가능한 레벨이 아닙니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_LIST_ALREADY:	// 이미 선생 리스트에 있음
				strMessage.PrintF( _S( 1359, "이미 후견인 리스트에 등록되어 있습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_LIST_NOT:		// 선생 리스트에 있지 않음
				strMessage.PrintF( _S( 1360, "후견인 리스트에 등록 되어 있지 않습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_REQ_ALREADY:		// 이미 신청중
				strMessage.PrintF( _S( 1361, "이미 신청중입니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_STUDENT_LEVEL:			// 학생 가능레벨이 아님
				strMessage.PrintF( _S( 1362, "견습생이 가능한 레벨이 아닙니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_HAVE_TEACHER:			// 이미 선생이 있음
				strMessage.PrintF( _S( 1363, "이미 후견인이 있습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_NOT_CONNECT:		// 선생이 접속중이 아님
				strMessage.PrintF( _S( 1364, "후견인이 접속중이 아닙니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_STUDENT_NOT_CONNECT:		// 학생이 접속중이 아님
				strMessage.PrintF( _S( 1365, "견습생이 접속중이 아닙니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_FULL:			// 제자가 꽉 차 있음
				strMessage.PrintF( _S( 1366, "더 이상 견습생을 받을 수 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_NOE_REQ:					// 신청한 적 없음
				strMessage.PrintF( _S( 1367, "신청한 적이 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_NOMORE_STUDENT:			// 더이상 학생 할 수 없음
				strMessage.PrintF( _S( 1368, "더 이상 견습생을 할 수 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			default:
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			}
		}
		break;
	}	
}

// ----------------------------------------------------------------------------
// Name : ReceiveChangeJobMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveChangeJobMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch(ubType)
	{
	case MSG_CHANGEJOB_REQ:			// 전직 요청		: job2(c)
		{
			SBYTE sbJob2;
			(*istr) >> sbJob2;
		}
		break;
	case MSG_CHANGEJOB_REP:			// 전직 알림		: charindex(n) job(c) job2(c)
		{
			LONG	lCharIndex;
			SBYTE	sbJob;
			SBYTE	sbJob2;
			(*istr) >> lCharIndex;
			(*istr) >> sbJob;
			(*istr) >> sbJob2;

			// 나
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.job				= sbJob;
				_pNetwork->MyCharacterInfo.job2				= sbJob2;

				CTString strMessage;
				strMessage.PrintF( _S( 1369, "[%s]로 전직 되었습니다." ), JobInfo().GetName( sbJob, sbJob2 ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );

				// 스페셜 리스트 일때만...
				if( sbJob == SORCERER && 
					sbJob2 == 2 )
				{
					_pUIMgr->GetCharacterInfo()->RemoveSummonSkill();
					_pUIMgr->GetQuickSlot()->RemoveSummonSkill();
				}
			}
			// 타 캐릭터
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ipl++ )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_iType	= sbJob;
						ct.cha_iJob2	= sbJob2;
						break;
					}
				}
			}
		}
		break;
	case MSG_CHANGEJOB_RESET_REQ:	// 직업 포기 요청	: 
		{
		}
		break;
	case MSG_CHANGEJOB_RESET_REP:	// 직업 포기 알림	: charindex(n) job(c)
		{
			LONG	lCharIndex;
			SBYTE	sbJob;
			(*istr) >> lCharIndex;
			(*istr) >> sbJob;
		
			// 나
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				CTString strMessage;
				strMessage.PrintF( _S( 1370, "[%s] 직업이 포기 되었습니다." ),  JobInfo().GetName( _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2 ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				
				// 스페셜 리스트 일때만...
				if( _pNetwork->MyCharacterInfo.job == SORCERER && 
					_pNetwork->MyCharacterInfo.job2 == 1 )
				{
					_pUIMgr->GetQuickSlot()->RemoveElementalSkill();
				}
				
				_pNetwork->MyCharacterInfo.job				= sbJob;
				_pNetwork->MyCharacterInfo.job2				= 0;

				_pUIMgr->GetInitJob()->CloseInitJob();
			}
			// 타 캐릭터.
			else
			{
				for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ipl++ )
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
					if( ct.cha_Index == lCharIndex )
					{
						ct.cha_iType			= sbJob;
						ct.cha_iJob2			= 0;
						break;
					}
				}
			}
		}
		break;
	case MSG_CHANGEJOB_ERROR:			// 에러
		{
			UBYTE ubErrorType;
			CTString strMessage;

			(*istr) >> ubErrorType;

			switch( ubErrorType )
			{			
			case MSG_CHANGEJOB_ERROR_OK:				
				break;			
			case MSG_CHANGEJOB_ERROR_WRONGMSG:
				strMessage.PrintF( _S( 1371, "잘못 된 메세지입니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_ALREADY:
				strMessage.PrintF( _S( 1372, "이미 전직했습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_LEVEL:
				strMessage.PrintF( _S( 1373, "레벨이 부족하여 전직할 수 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL:
				strMessage.PrintF( _S( 1374, "스킬이 없어서 전직할 수 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_SKILLLEVEL:
				strMessage.PrintF( _S( 1375, "스킬 레벨이 부족하여 전직할 수 없습니다."  ) );				
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOMONEY:
				strMessage.PrintF( _S( 1376, "돈이 부족하여 전직할 수 없습니다."  ) );		
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			default:
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			}
		}
		break;
	}
}

#define BILL_FLAG_START			0x0001	
#define BILL_FLAG_END			0x0002
#define BILL_FLAG_EMPTY			0x0004
#define BILL_FLAG_WARR			0x0008	// 시간 만료 경고 메세지 
#define BILL_FLAG_POST_USERINFO	0x0010	// 사용자 정보 ( 아이콘 클릭)

//------------------------------------------------------------------------------
// CSessionState::ReceiveBillingMessage
// Explain:  
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSessionState::ReceiveBillingMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;
	
	static BILLINFO_VEC	vecBillInfo;
	CBillInfo			BillInfo;
	CBillInfo			biReserve;

	switch(ubType)
	{
	case MSG_BILLINFO_USERINFO:
		{
			CTString	 strBillGuid;
			CTString	strBillViewName;
			LONG		lBillSection;		// 시간제인가? 4 시간제 6월정제
			LONG		lSurplusPoint;		// 잔액 
			LONG		lYear;				// 종료 날짜
			LONG 		lMonth; 
			LONG 		lDay; 
			LONG 		lHour; 
			LONG 		lMin; 
			LONG 		lRemaintime;		// 남은 시간
			LONG 		lExistReserve;		// 예약 결재
			CTString	strReserveGuid;
			LONG		lReserveValue;
			CTString	strReserveName;
			LONG 		lIsWarn; 
			LONG 		lIsTimeOut;
			LONG 		lFirst;
			SBYTE 		nServer;
						
			(*istr) >> strBillGuid;
			(*istr) >> strBillViewName;
			(*istr) >> lBillSection;
			(*istr) >> lSurplusPoint;
			(*istr) >> lYear;
			(*istr) >> lMonth;
			(*istr) >> lDay;
			(*istr) >> lHour;
			(*istr) >> lMin;
			(*istr) >> lRemaintime;
			(*istr) >> lExistReserve;
			(*istr) >> strReserveName;
			(*istr) >> lReserveValue;
			(*istr) >> strReserveName;
			(*istr) >> lIsWarn; 
			(*istr) >> lIsTimeOut;
			(*istr) >> lFirst;
			(*istr) >> nServer;
			
			if( lBillSection >= 50 )
			{

				_pUIMgr->GetBilling()->SetLock( FALSE );
				
				if( nServer >= 11 ) // 11 번 서버 군이상은 무료 사용자로 설정 
				{
					// 천하대란 서버의 경우에는 처음에는 아무것도 안 뜨우고 아이콘 클릭으로 요청 아이템 목록을 띄웁니다.
					if( !lFirst )
					{
						_pNetwork->BillItemListReq();
					}
				}
				else
				{
					_pUIMgr->GetBilling()->OpenInfo();
				}
				break;
			}

			CBillInfo BillInfo( strBillGuid, strBillViewName, (CHARGE_TYPE)lBillSection );
			
			_pUIMgr->GetBilling()->SetUsedBilling( BillInfo );
			_pUIMgr->GetBilling()->SetMoneyLeftOver( lSurplusPoint );
			_pUIMgr->GetBilling()->SetLeftTime( lRemaintime );
			_pUIMgr->GetBilling()->SetEndTimeLimit( CDate( lYear, lMonth, lDay, lHour, lMin ) );
			
			if( lExistReserve ) // 예약 결재가 존재한 다면 
			{
				vecBillInfo.clear();
				biReserve.SetBillInfo( strReserveGuid, strReserveName, CHARGE_TIME, 0, lReserveValue );
				vecBillInfo.push_back( biReserve );
				_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
			}
			else
			{
				vecBillInfo.clear();

				if( BillInfo.m_eSection == CHARGE_TIME )
				{
					vecBillInfo.push_back( BillInfo );
				}
				
				_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
			}
	
			if( lFirst ) // 처음이야~
			{
				if( lRemaintime <= 0 ) // 남은 시간이 0 이라면
				{
					_pUIMgr->GetBilling()->SetLock( TRUE );

					if( lExistReserve ) // 선결재가 있다면
					{
						_pUIMgr->GetBilling()->OpenBeforeUse(); // Date : 2005-05-16(오후 4:41:20), By Lee Ki-hwan
					}
					else
					{
						if( lBillSection == CHARGE_TIME )  // 시간제(화면6)
						{
							_pUIMgr->GetBilling()->OpenBeforeUseNow();
							
						}
						else //if( lBillSection == CHARGE_MONTHLY )  // 월정제
						{
							// 사용권한 없음 표시
							 _pUIMgr->GetBilling()->MessageNotAllowUse();
						}
					}
				}
				else
				{
					_pUIMgr->GetBilling()->SetLock( FALSE );
					_pUIMgr->GetBilling()->OpenTime();
				}
			}	
			else // 시간 만료이거나 사용자 정보 요청 
			{
				if( lIsWarn || lIsTimeOut ) // 시간 만료, 경고인가?
				{
					if( lRemaintime <= 0 ) // 남은 시간이 0 이라면
					{
						_pUIMgr->GetBilling()->SetLock( TRUE );
					}
					else
					{
						_pUIMgr->GetBilling()->SetLock( FALSE );
					}

					if( lExistReserve )
					{
						_pUIMgr->GetBilling()->OpenTime();
					}
					else
					{
						if( lBillSection == CHARGE_TIME ) // 시간제 인가?
						{
							// 결재 가능액 확인				
							if( _pUIMgr->GetBilling()->GetMoneyLeftOver() - _pUIMgr->GetBilling()->GetUsedBilling().m_lValue >= 0 )
								_pUIMgr->GetBilling()->OpenAutoChargeInfo();	
							else
								_pUIMgr->GetBilling()->OpenChargeConfirm();
						}
						else
						{
							_pUIMgr->GetBilling()->OpenTimeNow();

						}
					}
				
				}
				else // 아이콘에 의한 정보 요청
				{
					_pUIMgr->GetBilling()->OpenTime();
				}
			}
		}
		break;
	
	case MSG_BILLINFO_SECTION_LIST_REP:
		{
			LONG		lFlag;
			CTString	strbill_guid;
			LONG		lbill_section;
			LONG		lbill_hour;
			LONG		lbill_value;
			CTString	strBillViewName;
					
			(*istr) >> lFlag;
			(*istr) >> strbill_guid;
			(*istr) >> lbill_section;
			(*istr) >> lbill_hour;
			(*istr) >> lbill_value;
			(*istr) >> strBillViewName;


			if ( ( BILL_FLAG_EMPTY & lFlag ) && ( BILL_FLAG_END & lFlag ) && ( BILL_FLAG_START & lFlag ) )
			{
				vecBillInfo.clear();
				_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
				return;
			}
			

			if( BILL_FLAG_START & lFlag )
			{
				// 받기 시작 
				vecBillInfo.clear();
				
				if( !( BILL_FLAG_END & lFlag) )
				{
					BillInfo.SetBillInfo( strbill_guid, strBillViewName, (CHARGE_TYPE)lbill_section, lbill_hour, lbill_value );
					vecBillInfo.push_back( BillInfo );
				}		

			}
			if ( BILL_FLAG_END & lFlag )
			{
				BillInfo.SetBillInfo( strbill_guid, strBillViewName, (CHARGE_TYPE)lbill_section, lbill_hour, lbill_value );
				vecBillInfo.push_back( BillInfo );
				
				_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
				
				_pUIMgr->GetBilling()->OpenDefault();
				
			}
			
			if ( !( ( BILL_FLAG_EMPTY & lFlag ) || ( BILL_FLAG_END & lFlag ) || ( BILL_FLAG_START & lFlag ) ) )
			{
				BillInfo.SetBillInfo( strbill_guid, strBillViewName, (CHARGE_TYPE)lbill_section, lbill_hour, lbill_value );
				vecBillInfo.push_back( BillInfo );				
			}
		}
		break;
	
	case MSG_BILLINFO_PAY_REP:	// 결제 응답
		{
			LONG		lErrCode;  
			CTString	strBillGuid; 
			CTString	strBillViewName; 
			LONG		lBillSection;
			LONG		lSurplusPoint;
			LONG		lYear;
			LONG		lMonth;
			LONG		lDay;
			LONG		lHour;
			LONG		lMin;
			LONG		lRemaintime;
			LONG 		lExistReserve;	
			CTString	strReserveGuid;
			LONG		lReserveValue;
			CTString	strReserveName;
			LONG 		lFromAuto;


			(*istr) >> lErrCode;
			(*istr) >> strBillGuid; 
			(*istr) >> strBillViewName; 
			(*istr) >> lBillSection;
			(*istr) >> lSurplusPoint;
			(*istr) >> lYear;
			(*istr) >> lMonth;
			(*istr) >> lDay;
			(*istr) >> lHour;
			(*istr) >> lMin;
			(*istr) >> lRemaintime;
			(*istr) >> lExistReserve;
			(*istr) >> strReserveName;
			(*istr) >> lReserveValue;
			(*istr) >> strReserveName;
			(*istr) >> lFromAuto; 
			
			if ( lErrCode == MSG_BILLINFO_PAY_ERROR_OK )
			{
				_pUIMgr->GetBilling()->Close();
				if( lRemaintime > 0 ) _pUIMgr->GetBilling()->SetLock( FALSE );

				CBillInfo BillInfo( strBillGuid, strBillViewName, (CHARGE_TYPE)lBillSection );
				
				_pUIMgr->GetBilling()->SetUsedBilling( BillInfo );
				_pUIMgr->GetBilling()->SetMoneyLeftOver( lSurplusPoint );
				_pUIMgr->GetBilling()->SetLeftTime( lRemaintime );
				_pUIMgr->GetBilling()->SetEndTimeLimit( CDate( lYear, lMonth, lDay, lHour, lMin ) );

				//_pUIMgr->GetBilling()->Close();
				if( lExistReserve ) // 예약 결재가 존재한 다면 
				{
					vecBillInfo.clear();	
					CBillInfo biReserve( strReserveGuid, strReserveName, CHARGE_TIME, 0, lReserveValue );
					vecBillInfo.push_back( biReserve );
					_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
					_pUIMgr->GetBilling()->OpenTime();
					break;
				}
				else
				{
					vecBillInfo.clear();	
					if( BillInfo.m_eSection == CHARGE_TIME )
					{
						vecBillInfo.push_back( BillInfo );
					}
	
					_pUIMgr->GetBilling()->SetBillList( vecBillInfo );
				
					if( !lFromAuto ) _pUIMgr->GetBilling()->OpenTime();
					else
					{
						_pUIMgr->GetBilling()->Close();
					}
				}

			}
			else
			{
				_pUIMgr->GetBilling()->ErrorProcess( lErrCode );
			}

		}
		break;
	case MSG_BILLINFO_ITEM_LIST_REP: // 유료화 지급 아이템 목록
		{
	/*		LONG		nBillCount;  	
			CTString	strbill_guid;
			LONG		nItemCount;
			LONG		nItemIndex;
			LONG		nItemPlus;	
			LONG		nItemFlag;
			LONG		nItemNumber;
			
			LONG		lAllItemCount = 0;
	
			CBillItemData		BillItemData;

			int i, j;

			(*istr) >> nBillCount;
		
			
			for( i = 0; i < nBillCount; i++ )
			{
				(*istr) >> strbill_guid;
				(*istr) >> nItemCount;

				for( j = 0; j < nItemCount; j++ )
				{
					CBillItemData		BillItemData;

					(*istr) >> nItemIndex;
					(*istr) >> nItemPlus;	
					(*istr) >> nItemFlag;
					(*istr) >> nItemNumber;
					BillItemData.SetBillItem( nItemIndex, nItemPlus, nItemFlag, nItemNumber );
				
					// Add ItemList;
					lAllItemCount++;
					_pUIMgr->GetBillItem()->AddBillItemData( BillItemData );
				}
			}
			// 화면에 ItemList 표시 
			_pUIMgr->GetBillItem()->OpenBillItem();
					
		}*/
			LONG	listFlag;
			LONG	itemCount;
			LONG	index;
			LONG	itemIndex;
			LONG	itemPlus;
			LONG	itemFlag;
			LONG	itemNum;
			CBillItemData		BillItemData;
			
			(*istr) >> listFlag;
			(*istr) >> itemCount;
			for(int i=0; i<itemCount ;i++)
			{
				(*istr) >> index;
				(*istr) >> itemIndex;
				(*istr) >> itemPlus;
				(*istr) >> itemFlag;
				(*istr) >> itemNum;

				BillItemData.SetBillItem( index, itemIndex, itemPlus, itemFlag, itemNum );
				// Add ItemList;
				_pUIMgr->GetBillItem()->AddBillItemData( BillItemData );
			}
			if( listFlag&0x02 ) 
				// 마지막 데이터를 읽으면 화면에 ItemList 표시 
				_pUIMgr->GetBillItem()->OpenBillItem();
		}
		break;

	case MSG_BILLINFO_ITEM_REP: // 유료화 지급 아이템 요청 확인
		{
			SBYTE		nErrCode;  
			(*istr) >> nErrCode;
			_pUIMgr->GetBillItem()->BillItemRep( nErrCode );
		}
		break;

	}

}


//------------------------------------------------------------------------------
// CSessionState::ReceiveMessengerMessage
// Explain:  메신저 관련 메세지 처리
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSessionState::ReceiveMessengerMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch(ubType)
	{
	case MSG_FRIEND_ERROR:
		{
			SBYTE	lErrCode;  
			(*istr) >> lErrCode;
			_pUIMgr->GetMessenger()->ErrorMsg( lErrCode ); 
		}
		break;

	// 친구 리스트를 받는다.
	case MSG_FRIEND_MEMBERLIST:
		{
			LONG		lCharIndex;  
			LONG		lGroupIndex;  
			CTString	strReqCharName;
			LONG		lJob;
			LONG		lCondition;

			(*istr) >> lCharIndex;
			(*istr) >> strReqCharName;
			(*istr) >> lJob;
			(*istr) >> lCondition;
			(*istr) >> lGroupIndex;

			_pUIMgr->GetMessenger()->AddFriendList( lCharIndex, lGroupIndex, strReqCharName, (eJob)lJob, (eCondition)lCondition, false);

		}
		break;
	
	// 새로운 친구 등록
	case MSG_FRIEND_REGIST_MEMBER_NOTIFY:	
		{
			LONG		lCharIndex;  
			CTString	strReqCharName;
			LONG		lJob;  
			LONG		lCondition;  

			(*istr) >> lCharIndex;
			(*istr) >> strReqCharName;
			(*istr) >> lJob;  
			(*istr) >> lCondition;  

			_pUIMgr->GetMessenger()->AddFriendList( lCharIndex, 0, strReqCharName, (eJob)lJob, (eCondition)lCondition, false );
		}
		break;
	// 친구 리스트에서 삭제
	case MSG_FRIEND_DELETE_MEMBER_NOTIFY:	
		{
			LONG		lCharIndex;  
			
			(*istr) >> lCharIndex;
			_pUIMgr->GetMessenger()->DeleteMember( lCharIndex );
		}
		break;

	// 친구 등록 요청 받음 
	case MSG_FRIEND_REGIST_REQUEST: 
		{
			if( !_pUIMgr->GetMessenger()->IsUseLevel() ) break;
			_pUIMgr->RearrangeOrder( UI_MESSENGER, TRUE );

			LONG		lCharIndex;  
			CTString	strReqCharName;
			(*istr) >> lCharIndex;
			(*istr) >> strReqCharName;
			

			if ( _pNetwork->MyCharacterInfo.index == lCharIndex )
			{
				CTString strMessage;
				strMessage.PrintF( _S( 1622, "%s님께 친구등록을 요청하고 있습니다. 취소하시겠습니까?" ), strReqCharName );
				_pUIMgr->GetMessenger()->Message( MSGCMD_MESSENGER_ADD_WAIT, _S(2996, "친구등록 요청"), strMessage, UMBS_CANCEL );
			}
			else
			{
				_pUIMgr->GetMessenger()->RegistMemberReq( lCharIndex, strReqCharName ); 
			}
		}
		break;
	case MSG_FRIEND_CHATTING:
		{

		}
		break;
	case MSG_FRIEND_REGIST_CANCEL:
		{
			_pUIMgr->CloseMessageBox( MSGCMD_MESSENGER_ADD_REP  );
			_pUIMgr->CloseMessageBox( MSGCMD_MESSENGER_ADD_WAIT  );
			_pUIMgr->GetChatting()->AddSysMessage( _S(2082,"상대방이 친구등록을 취소하였습니다." ));			
		}
		break;
	case MSG_FRIEND_SET_CONDITION :
		{
			LONG		lCharIndex;  
			LONG		lCondition;

			(*istr) >> lCharIndex;
			(*istr) >> lCondition;
			_pUIMgr->GetMessenger()->SetMemberCondition( lCharIndex, (eCondition)lCondition );
		
		}
		break;
	}
}

void CSessionState::ReceiveMessengerExMessage( CNetworkMessage *istr )
{
	UBYTE	exType;
	(*istr) >> exType;

	switch(exType)
	{
		case MSG_EX_MESSENGER_GROUP_LIST :
		{
			LONG		nCount;
			LONG		nGroupIndex;  
			CTString	strGroupName;

			(*istr) >> nCount;
			for(int i=0; i<nCount; ++i)
			{
				(*istr) >> nGroupIndex;
				(*istr) >> strGroupName;
				_pUIMgr->GetMessenger()->AddGroup(nGroupIndex, strGroupName);
			}

			_pUIMgr->GetMessenger()->SetDefaultGroupName(_S(2986, "라스트카오스친구"));
		
		}
		break;

		case MSG_EX_MESSENGER_GROUP_ADD :
		{
			LONG		nGroupIndex;  
			CTString	strName;

			(*istr) >> nGroupIndex;
			(*istr) >> strName;
			_pUIMgr->GetMessenger()->AddGroup(nGroupIndex, strName);
		
		}
		break;

		case MSG_EX_MESSENGER_GROUP_DEL :
		{
			//LONG		nGroupIndex;  

			//(*istr) >> nGroupIndex;
		}
		break;

		case MSG_EX_MESSENGER_GROUP_CHANGE :
		{
			LONG		nGroupIndex;
			CTString	strName;
			
			(*istr) >> nGroupIndex;
			(*istr) >> strName;

			_pUIMgr->GetMessenger()->RenameGroup(nGroupIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_INVITE :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex;  
			CTString	strName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> strName;

			_pUIMgr->GetMessenger()->SetChat(lMakeCharIndex, lChatIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_OUT :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex;  
			CTString	strName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> strName;

			_pUIMgr->GetMessenger( )->DeleteChatMember( lMakeCharIndex, lChatIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_CHAT:
		{
			LONG		lMakeCharIndex;
			LONG		lChatIndex;
			LONG		lColIndex;
			CTString	strName;
			CTString	strChat;			
			
			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> lColIndex;
			(*istr) >> strName;
			(*istr) >> strChat;			

			_pUIMgr->GetMessenger()->RevTalk( lMakeCharIndex, lChatIndex, strName, strChat, lColIndex);
		}


		case MSG_EX_MESSENGER_CHARLIST :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex; 
			LONG		lCount;
			CTString	strCharName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> lCount;

			for(int i=0; i<lCount; ++i)
			{
				(*istr) >> strCharName;
				_pUIMgr->GetMessenger()->AddChatMember(lMakeCharIndex, lChatIndex, strCharName);
			}
		}
		break;

		case MSG_EX_MESSENGER_BLOCK :
		{
			BYTE		cError;  
			LONG		lCharIndex; 
			CTString	strCharName;

			(*istr) >> cError;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;

			_pUIMgr->GetMessenger()->Block(cError, lCharIndex, strCharName);
		}
		break;

		case MSG_EX_MESSENGER_RELEASE_BLOCK :
		{
			BYTE		cError;  
			LONG		lCharIndex; 
			CTString	strCharName;

			(*istr) >> cError;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;

			_pUIMgr->GetMessenger()->UnBlock(cError, lCharIndex, strCharName);
		}
		break;


		case MSG_EX_MESSENGER_BLOCK_LIST :
		{
			LONG		lCount;
			LONG		lCharIndex;  
			CTString	strCharName;

			(*istr) >> lCount;
			
			
			for(int i=0; i<lCount; ++i)
			{
				(*istr) >> lCharIndex;
				(*istr) >> strCharName;
				_pUIMgr->GetMessenger()->AddFriendList(lCharIndex, -1, strCharName, (eJob)0, (eCondition)0, true);
			}
		}
		break;

		case MSG_EX_MESSENGER_ONE_VS_ONE: // 서버의 인덱스가 클라이언트에 없는 1:1 채팅
		{
			LONG		lsIndex;
			CTString	strName;
			LONG		lrIndex;
			LONG		lColIndex;
			CTString	strChat;			
			
			(*istr) >> lsIndex;
			(*istr) >> strName;
			(*istr) >> lrIndex;
			(*istr) >> lColIndex;
			(*istr) >> strChat;			

			_pUIMgr->GetMessenger()->RevOneVsOneTalk( lsIndex, lrIndex, strName, strChat, lColIndex);

		}
		break;

		case MSG_EX_MESSENGER_ONE_OFF_LINE :	// 1:1대화에서 상대가 응답할 수 없는 경우
		{
			LONG		lsIndex; 
			LONG		lrIndex;			

			(*istr) >> lsIndex;
			(*istr) >> lrIndex;

			// 다시돌아오는 에러 메세지이기 때문에 sender와 resiver를 바꿨다
			_pUIMgr->GetMessenger()->RevOneVsOneTalk( lrIndex, lsIndex, CTString(""),
				_S(2064,"대화상대가 오프라인이여서 메세지를 전달할 수 없습니다."), 7 /*RED*/ );  

		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveFailMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveFailMessage(CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;
	
	CTString		strTitle,strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	switch(ubType)
	{

		case MSG_FAIL_WRONG_CHAR :
			_pUIMgr->CloseMessageBox(MSG_FAIL_WRONG_CHAR);
			strTitle	=	_S(191,"확인");
			strMessage	=_S(883,"길드 이름에 잘못된 문자가 포함되어 있습니다."); 
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		
			break;
	}

}

// EDIT : BS
void CSessionState::MoveOtherServer(ULONG zone, CTString ip, ULONG port)
{
	extern BOOL _bLoginProcess;
	_bLoginProcess					= TRUE;
	//extern BOOL _bUseSocket;
	//_bUseSocket = TRUE;

	// 소켓의 연결을 끊었다가, 다시 연결함.
	_cmiComm.Reconnect(ip, port);

	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 패킷 암호화 : 암호화 키 초기화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&_pNetwork->cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&_pNetwork->cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	_pNetwork->cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG
	
	// 로그인 서버에 접속 후, 새로운 서버에 접속을 시도합니다.
	CNetworkMessage nmLoginNew(MSG_LOGIN);			// 로그인 메시지 보내기.	
	ULONG version;	 
	version = 700;
	nmLoginNew << version;	    
	nmLoginNew << (unsigned char)MSG_LOGIN_RE;		// 서버 이동
	
	CTString _strUserID;
	CTString _strUserPW;

	_strUserID = _pNetwork->m_strUserID;
	_strUserPW = _pNetwork->m_strUserPW;
	nmLoginNew << _strUserID;
	nmLoginNew << _strUserPW;
	
#ifdef CRYPT_NET_MSG
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// new Version serialization 060710
	// national code 
	extern INDEX g_iCountry;
	INDEX tv_idx;
	switch(g_iCountry)
	{
	case KOREA :
		tv_idx = 0;
		break;
	case TAIWAN : 
		tv_idx = 1;
		break;
	case CHINA : 
		tv_idx = 3;
		break;
	case THAILAND :
		tv_idx = 4;
		break;
	case TAIWAN2 :
		tv_idx = 2;
		break;
	case JAPAN : 
		tv_idx = 6;
		break;
	case MALAYSIA :
		tv_idx = 7;
		break;
	}

	nmLoginNew << (UBYTE)tv_idx;

#ifndef CRYPT_NET_MSG_MANUAL
	ULONG ulSeed = (ULONG)CNM_MakeSeedForClient(_strUserID, _strUserPW, GetTickCount());
	nmLoginNew << ulSeed;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	_pNetwork->SendToServerNew(nmLoginNew, TRUE);
	_cmiComm.Server_Update();
	
	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_MakeKeyFromSeed(&_pNetwork->cnmKey, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	unsigned int StartTime = timeGetTime(); 
	StartTime = timeGetTime(); 
	
	for(;;)
	{		
		_cmiComm.Server_Update();
		CNetworkMessage nmMessage;	  
		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			if (nmMessage.GetType() == MSG_DB)
			{
				unsigned char dbtype;
				nmMessage >> dbtype;
				
				if(dbtype == MSG_DB_CHAR_END)
				{
#ifdef CRYPT_NET_MSG
					// EDIT : BS : 070413 : 신규 패킷 암호화
					//wooss 050820
					ULONG slotTime;
					nmMessage >> slotTime;
					nmMessage >> slotTime;

#ifndef CRYPT_NET_MSG_MANUAL
					ULONG ulSeed;
					nmMessage >> ulSeed;
					CNM_MakeKeyFromSeed(&_pNetwork->cnmKeyServer, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
					break;					
				}
			}
		}

		// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
		// FIXME : 접속 종료 처리를 이 안에서 하는듯...
		// FIXME : 수정해야 함.
		if(timeGetTime() - StartTime > 10000)
		{
			// FIXME : 중복되는 코드가 많음.
			_pUIMgr->CloseMessageBox(MSGCMD_DISCONNECT);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
			MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			//cci_bInitUpdateMasterBuffer = FALSE;
			_pNetwork->m_bSendMessage = FALSE;
			_cmiComm.Disconnect();
			break;
		}
	}

	CNetworkMessage nmChaStart(MSG_MENU); 
	nmChaStart << (unsigned char)MSG_MENU_START;
	nmChaStart << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew(nmChaStart,TRUE);	
	
	StartTime = timeGetTime(); 

	for(;;)
	{	
		_cmiComm.Server_Update();
		CNetworkMessage nmMessage;	  
		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			if (nmMessage.GetType() == MSG_DB)
			{
				unsigned char dbtype;
				nmMessage >> dbtype;
				
				if(dbtype == MSG_DB_OK)
				{				
					ULONG zone;
					nmMessage >> zone;
					g_slZone = zone;
				}
				
				_bLoginProcess					= FALSE;
				_SetPlayStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

				///////////
				CEntity* penPlEntity;
				CPlayerEntity* penPlayerEntity;
				penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
				penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);
				
				g_slZone = zone;//0921

				if(ZoneInfo().GetZoneType(zone) == ZONE_SDUNGEON)
				{
					CPrintF("===Start Single Mode===\n");
					_pNetwork->m_bSingleMode = TRUE;
					_pNetwork->wo_dwEnemyCount		= 0;
					_pNetwork->wo_dwKilledEnemyCount= 0;
					penPlayerEntity->SingleModeOn();
				}
				else
				{
					CPrintF("===End Single Mode===\n");
					_pNetwork->m_bSingleMode = FALSE;
					penPlayerEntity->FieldModeOn();
					_pUIMgr->GetSingleBattle()->Close();
				}
				_pNetwork->m_ubGMLevel				= 0;
				
				_pNetwork->TogglePause();
				_pNetwork->ChangeLevel(ZoneInfo().GetZoneWldFile(zone), FALSE, 0);
				
				_pNetwork->DeleteAllMob();
				_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL; // 초기화
				
				EPreLevelChange ePreChange;
				ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
				SendLevelChangeNotification(ePreChange);
				CEntity::HandleSentEvents();
				_pNetwork->ChangeLevel_internal();
				EPostLevelChange ePostChange;
				ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
				SendLevelChangeNotification(ePostChange);
				CEntity::HandleSentEvents();
				_lphCurrent=LCP_NOCHANGE;
				CNetworkMessage nmStartGame(MSG_START_GAME); 					 
				_pNetwork->SendToServerNew(nmStartGame,TRUE);	
				_cmiComm.Server_Update();

				//_bUseSocket = FALSE;
				//////////
				break;
			}
			// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
			// FIXME : 접속 종료 처리를 이 안에서 하는듯...
			// FIXME : 수정해야 함.
			else if( nmMessage.GetType() == MSG_FAIL)
			{
				// FIXME : 중복되는 코드가 많음.
				_pUIMgr->CloseMessageBox(MSGCMD_DISCONNECT);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
				MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				//cci_bInitUpdateMasterBuffer = FALSE;
				_pNetwork->m_bSendMessage = FALSE;
				_cmiComm.Disconnect();
				break;
			}
		}

		// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
		// FIXME : 접속 종료 처리를 이 안에서 하는듯...
		// FIXME : 수정해야 함.
		if(timeGetTime() - StartTime > 10000)
		{
			// FIXME : 중복되는 코드가 많음.
			_pUIMgr->CloseMessageBox(MSGCMD_DISCONNECT);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
			MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			//cci_bInitUpdateMasterBuffer = FALSE;
			_pNetwork->m_bSendMessage = FALSE;
			_cmiComm.Disconnect();
			break;
		}
	}
}
// --- EDIT : BS

// ----------------------------------------------------------------------------
// Name : ReceiveExHairChange()
// Desc : 산타 모자 관련( 헤어 변경 메세지 처리 )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExHairChange(CNetworkMessage *istr)
{
	ULONG ulCharIndex;
	UBYTE ubHairStyle;

	(*istr) >> ulCharIndex >> ubHairStyle;

	CEntity *penEntity = NULL;
	CEntity* penPlEntity = NULL;
	CModelInstance *pMI = NULL;
	CPlayerEntity* penPlayerEntity = NULL;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity; // SetCharacterHairChange() 호출을 위해 사용

	if ( ulCharIndex == _pNetwork->MyCharacterInfo.index ) // 자기 캐릭터
	{
		penPlayerEntity->SetCharacterHairChange(penPlEntity, ubHairStyle, 0, TRUE);
	}
	else // 타 캐릭터
	{
		INDEX Cnt = 0;

		// 캐릭터를 찾아라
		if ( _pNetwork->SearchEntityByNetworkID(ulCharIndex, MSG_CHAR_PC, penEntity, Cnt) )
		{
			if ( Cnt < 0 || penEntity == NULL ) return;

			penPlayerEntity->SetCharacterHairChange(penEntity, ubHairStyle, Cnt, FALSE);
		}
	}
}
// ----------------------------------------------------------------------------
// Name : ReceiveExPlayerStateChange()
// Desc : 게임 서포터 관련( 특정 아이템 착용시 이펙트 적용 )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExPlayerStateChange(CNetworkMessage *istr)
{
	ULONG ulCharIndex;
	ULONG ulPlayerState;

	(*istr) >> ulCharIndex >> ulPlayerState;

	CEntity *penEntity = NULL;
	CEntity* penPlEntity = NULL;
	CModelInstance *pMI = NULL;
	BOOL bAdd = FALSE;
	CPlayerEntity* penPlayerEntity = NULL;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity; // PlAddAccessoryEffect() 호출을 위해 사용

	if ( ulPlayerState & PLAYER_STATE_SUPPORTER ) bAdd = TRUE;
	
	if ( ulCharIndex == _pNetwork->MyCharacterInfo.index )
	{ // 자기 캐릭터
		if ( bAdd )
		{ // 적용
			penPlayerEntity->PlAddAccessoryEffect(0, penPlEntity, TRUE);
		}
		else
		{ // 해제
			_pNetwork->MyCharacterInfo.itemEffect.DelAccessoryEffect();
		}
	}
	else
	{ // 타 캐릭터
		// 캐릭터를 찾아라.
		INDEX Cnt=0;

		if ( _pNetwork->SearchEntityByNetworkID(ulCharIndex, MSG_CHAR_PC, penEntity, Cnt) )
		{
			if ( Cnt < 0 || penEntity == NULL ) return;

			if ( bAdd )
			{ // 적용
				_pNetwork->ga_srvServer.srv_actCha[Cnt].cha_state = ulPlayerState;
				penPlayerEntity->PlAddAccessoryEffect(Cnt, penEntity, FALSE);
			}
			else
			{ // 적용
				_pNetwork->ga_srvServer.srv_actCha[Cnt].cha_state = ulPlayerState;
				_pNetwork->ga_srvServer.srv_actCha[Cnt].cha_itemEffect.DelAccessoryEffect();
			}
		}
	}
}

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
void CSessionState::ReceiveExnProtect(CNetworkMessage *istr)
{
	extern ENGINE_API CNPGameLib npgl;
	GG_AUTH_DATA m_AuthData;
	(*istr) >>m_AuthData.dwIndex
		>>m_AuthData.dwValue1
		>>m_AuthData.dwValue2
		>>m_AuthData.dwValue3;
	
	// Debug...
//	CPrintF("Recv Index : %x\n", m_AuthData.dwIndex);
//	CPrintF("Recv Value1 : %x\n", m_AuthData.dwValue1);
//	CPrintF("Recv Value2 : %x\n", m_AuthData.dwValue2);
//	CPrintF("Recv Value3 : %x\n", m_AuthData.dwValue3);
	
//	CPrintF("[ ---->> GameGuard Auth2...Start <<---- ] - %lu\n",timeGetTime());
	npgl.Auth2(&m_AuthData);
//	CPrintF("[ ---->> GameGuard Auth2...End   <<---- ] - %lu\n",timeGetTime());
}
#endif
// ---------------------------------------------------<<
// [KH_070413] 스승의날 이벤트 관련 추가
void CSessionState::ReceiveEventMaster(CNetworkMessage *istr)
{
	INDEX iValue;
	(*istr) >> iValue;

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo(_S(3344, "스승의 은혜 이벤트"),UMBS_OK,UI_NONE,MSGCMD_NULL);

	if(iValue < 10)
		MsgBoxInfo.AddString(_S(3348, "어려운 모험에 작은 힘이 되도록 25레벨 일반제련석 10개를 드리겠습니다."));
	else
	{
		CTString strMsg;
		strMsg.PrintF(_S(3349, "견습생을 잘 양성하여 명성치가 +%d이 되었습니다. 앞으로도 많은 견습생 양성에 힘써주시기 바랍니다."), iValue);
		MsgBoxInfo.AddString(strMsg);
	}

	_pUIMgr->CreateMessageBox(MsgBoxInfo);
}	
	
	// [070613: Su-won] 펫 명찰 아이템 사용 결과 메시지 처리
void CSessionState::ReceiveExPetNameChange(CNetworkMessage *istr)
{
	UBYTE lResult;
	(*istr) >> lResult;
	
	switch(lResult)
	{
	case MSG_EX_PET_CHANGE_NAME_ERROR_OK:	// 이름 변경 성공:
		{
			ULONG lPetIndex;
			CTString strPetName;
			ULONG lOwnerIndex=0;

			(*istr) >> lPetIndex;
			(*istr) >> strPetName;

			//펫 정보 업데이트
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= lPetIndex;
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		
			if( iter != _pNetwork->m_vectorPetList.end() )
			{		
				(*iter).strNameCard		= strPetName;
			}

			for(int i=0; i<_pNetwork->ga_srvServer.srv_actPet.Count(); ++i)
			{
				if( _pNetwork->ga_srvServer.srv_actPet[i].pet_Index == lPetIndex)
				{
					_pNetwork->ga_srvServer.srv_actPet[i].pet_strNameCard =strPetName;
					lOwnerIndex =_pNetwork->ga_srvServer.srv_actPet[i].pet_OwnerIndex;
					break;
				}
			}

			_pNetwork->UpdatePetTargetInfo( lPetIndex );

			if( lOwnerIndex ==_pNetwork->MyCharacterInfo.index)
			{
				CUIMsgBox_Info	MsgBoxInfo;
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);
				MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(3531, "펫의 명찰이 변경되었습니다.") );
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_FAIL:	// Pet 이름 변경 실패
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_ITEM:	// 아이템이 없음
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_NOPET: // Pet 존재 안함
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_DUPLICATE:	// Pet 이름 중복
		{
			CUIMsgBox_Info	MsgBoxInfo;
			_pUIMgr->CloseMessageBox(MSGCMD_NULL);
			MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(3532, "입력한 이름은 존재하는 이름입니다. 다시 입력해 주세요.") );
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
		}
		break;
	}
}

// [070824: Su-won] PET_TEX_CHANGE
void CSessionState::ReceiveExPetColorChange(CNetworkMessage *istr)
{
	ULONG lPetIndex;			// 펫 인덱스
	SBYTE sbPetColor;			// 펫 색깔
	ULONG lOwnerIndex;			// 펫 주인 인덱스
	UBYTE sbTypeGrade;			// 펫 타입

	(*istr) >> lPetIndex;
	(*istr) >> sbPetColor;
	(*istr) >> lOwnerIndex;
	(*istr) >> sbTypeGrade;

	INDEX iPetType	= -1;
	INDEX iPetAge	= -1;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);

	// 변경할 텍스쳐 파일
	CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, sbPetColor);

	CEntity* penEntity =NULL;

	//마운트 상태이면...
	if( bPetRide )
	{
		//내 펫이면...
		if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
		{
			penEntity = CEntity::GetPlayerEntity(0);
		}
		// 내 펫이 아니면...
		else
		{
			for(int i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				if( _pNetwork->ga_srvServer.srv_actCha[i].cha_Index == lOwnerIndex)
				{
					penEntity =_pNetwork->ga_srvServer.srv_actCha[i].cha_pEntity;
				}
			}
		}
	}
	//마운트 상태가 아니면...
	else
	{
		for(int i=0; i<_pNetwork->ga_srvServer.srv_actPet.Count(); ++i)
		{
			if( _pNetwork->ga_srvServer.srv_actPet[i].pet_Index == lPetIndex)
			{
				penEntity =_pNetwork->ga_srvServer.srv_actPet[i].pet_pEntity;
			}
		}
	}

	if( penEntity == NULL )
		return;

	// 펫 모델의 텍스쳐를 바꿔줌...
	penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );


	// 이펙트 뿌려줌...
	{
		CTString strEffect;
		//말이면...
		if( iPetType%6<3 )
			strEffect = CTString("pet_dye");
		//용이면...
		else
			strEffect = CTString("pet_dye_1");

		if( bPetRide )
			PCStartEffectGroup( strEffect, lOwnerIndex );
		else
			PetStartEffectGroup( strEffect, lPetIndex );
	}


	// 내 펫의 색깔이 변경되었을 때만 메시지창 띄워줌...
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		_pUIMgr->CloseMessageBox(MSGCMD_NULL);
//		MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD)), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxInfo.SetMsgBoxInfo( _S( 1970, "성공" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxInfo.AddString( _S(3665, "펫 색상변경이 성공적으로 이루어졌습니다.") );
		_pUIMgr->CreateMessageBox(MsgBoxInfo);
	}
}


void CSessionState::ReceiveTradeAgentMessage(CNetworkMessage *istr )
{
	BOOL bShowMessage = TRUE;
	ULONG	ubType;
	(*istr) >> ubType;

	switch(ubType)
	{
		case MSG_TRADEAGENT_SEARCH_REP:
		{
			SLONG nCharIndex, nMaxPageNo, nPageNo, nItemCount;

			(*istr) >> nCharIndex;
			(*istr) >> nMaxPageNo;
			(*istr) >> nPageNo;

			_pUIMgr->GetAuction()->SetMaxPage(nMaxPageNo);
			_pUIMgr->GetAuction()->SetCurrentPage(nPageNo);
			_pUIMgr->GetAuction()->InitSlotItem();

			(*istr) >> nItemCount;

			CUIButtonEx btnAuctionItem;
			SLONG nAutionIndex, nItemIndex, nCount, nLevel, nItemPlus, nItemFlag, nOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;
			CTString strCharName = "";

			SQUAD nTotalNas;
			for(SLONG i=0; i<nItemCount; ++i)
			{
				btnAuctionItem.InitBtn();

				(*istr) >> nAutionIndex;
				(*istr) >> nItemIndex;
				(*istr) >> nItemPlus;
				(*istr) >> nItemFlag;
				btnAuctionItem.SetItemInfo(-1, -1, -1, nItemIndex, -1, -1);
				btnAuctionItem.SetItemPlus(nItemPlus);
				btnAuctionItem.SetItemFlag(nItemFlag);

				(*istr) >> nOptionCount;

				CItemData& ItemData	= _pNetwork->GetItemData(nItemIndex);
				CItems temItem;

				//레어 아이템일때...
				if( ItemData.GetFlag() & ITEM_FLAG_RARE )
				{
					//옵션 개수가 0이면 미감정 레어아이템
					if( nOptionCount ==0)
						temItem.SetRareIndex(0);
					//감정된 레어아이템이면...
					else
					{
						_pUIMgr->SetRareOption(istr, temItem);
					}

					btnAuctionItem.SetItemRareIndex(temItem.Item_RareIndex);

					for(SBYTE i = 0; i < MAX_ITEM_OPTION; i++)
					{
						btnAuctionItem.SetItemOptionData(i,temItem.GetOptionType(i),temItem.GetOptionLevel(i));
					}
				}
				//레어 아이템이 아니면.....
				else
				{
					for( SBYTE sbOption = 0; sbOption < nOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> sbOptionLevel;

						btnAuctionItem.SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
					}
				}

				(*istr) >> nCount;
				(*istr) >> nTotalNas;
				(*istr) >> nLevel;
				(*istr) >> strCharName;

				btnAuctionItem.SetItemCount(nCount);
				btnAuctionItem.SetCharName(CHAR_NAME_SELLER, strCharName);
				_pUIMgr->GetAuction()->SetSlotItem(i, nAutionIndex, btnAuctionItem, nTotalNas, nLevel);
			}

			_pUIMgr->GetAuction()->EnableSortBtn();
			_pUIMgr->GetAuction()->EnableSearchBtn();

		}
		break;
		
		case MSG_TRADEAGENT_REG_REP:
		{
			CTString strItemName;
			SLONG nCharIndex, nCount;
			SQUAD nTotalNas, nDepositMoney;

			(*istr) >> nCharIndex;
			(*istr) >> strItemName;
			(*istr) >> nCount;
			(*istr) >> nTotalNas;
			(*istr) >> nDepositMoney;

			_pUIMgr->GetAuction()->SetRegistering(FALSE); // 등록 요청 상태 취소

			CUIMsgBox_Info MsgBoxInfo;
			CTString strTemp;
			MsgBoxInfo.SetMsgBoxInfo( _S(4303, "등록 완료"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );
			MsgBoxInfo.AddString(_S(4304, "거래 대행 서비스를 이용해 주셔서 감사합니다."));
			
			strTemp.PrintF(_S(4305, "%s %ld개를 %I64d나스로 등록하셨습니다."),strItemName, nCount, nTotalNas);
			MsgBoxInfo.AddString(strTemp);

			strTemp.PrintF(_S(4306, "보증금으로 %I64d나스를 지불하셨습니다."), nDepositMoney);
			MsgBoxInfo.AddString(strTemp);

			_pUIMgr->CreateMessageBox(MsgBoxInfo);		

			_pUIMgr->GetAuction()->SetDepositMoney(nDepositMoney + 1000);

			_pNetwork->ClientSystemMessage(_S(4360, "아이템이 정상적으로 등록되었습니다."), SYSMSG_NOTIFY);

			int nPage = _pUIMgr->GetAuction()->GetCurrentPage();

			if(_pUIMgr->GetAuction()->GetCurrentAuctionItemCount() < AUCTION_SLOT_MAX)
			{
				_pNetwork->SendTradeAgentRegListReq(nPage);
			}
			else
			{
				_pNetwork->SendTradeAgentRegListReq(nPage+1);
			}
		}
		break;

		case MSG_TRADEAGENT_REG_CANCEL_REP:
		{
			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(4292, "등록취소"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );
			MsgBoxInfo.AddString(_S(4307, "등록이 취소되었습니다."));	
			
			_pUIMgr->CreateMessageBox(MsgBoxInfo);	
			
			int nPage = _pUIMgr->GetAuction()->GetCurrentPage();

			if(_pUIMgr->GetAuction()->GetCurrentAuctionItemCount() < AUCTION_SLOT_MAX)
			{
				_pNetwork->SendTradeAgentRegListReq(nPage);
			}
			else
			{
				_pNetwork->SendTradeAgentRegListReq(nPage+1);
			}
			
		}
			break;

		case MSG_TRADEAGENT_REG_LIST_REP:
		{

			SLONG nCharIndex, nCharRegTotCount, nMaxPageNo, nPageNo, nItemCount;

			(*istr) >> nCharIndex;
			(*istr) >> nCharRegTotCount;
			(*istr) >> nMaxPageNo;
			(*istr) >> nPageNo;

			_pUIMgr->GetAuction()->SetTab(AUCTION_TAB_REGISTER);
			_pUIMgr->GetAuction()->SetDepositMoney((nCharRegTotCount+1) * 1000);
			_pUIMgr->GetAuction()->SetMaxPage(nMaxPageNo);
			_pUIMgr->GetAuction()->SetCurrentPage(nPageNo);

			(*istr) >> nItemCount;
			
			CUIButtonEx btnAuctionItem;
			SLONG nAutionIndex, nItemIndex, nCount, nFinishDay, nFinishDayUnit, nItemPlus, nItemFlag, nOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;
			SQUAD nTotalNas;
			for(SLONG i=0; i<nItemCount; ++i)
			{
				btnAuctionItem.InitBtn();

				(*istr) >> nAutionIndex;
				(*istr) >> nItemIndex;
				(*istr) >> nItemPlus;
				(*istr) >> nItemFlag;
				btnAuctionItem.SetItemInfo(-1, -1, -1, nItemIndex, -1, -1);
				btnAuctionItem.SetItemPlus(nItemPlus);
				btnAuctionItem.SetItemFlag(nItemFlag);

				(*istr) >> nOptionCount;
/*
				for(int j=0; j<nOptionCount; j++)
				{
					(*istr) >> nOptionType;
					(*istr) >> nOptionLevel;
					btnAuctionItem.SetItemOptionData(j, nOptionType, nOptionLevel);
				}
*/
				CItemData& ItemData	= _pNetwork->GetItemData(nItemIndex);
				CItems temItem;

				//레어 아이템일때...
				if( ItemData.GetFlag() & ITEM_FLAG_RARE )
				{
					//옵션 개수가 0이면 미감정 레어아이템
					if( nOptionCount ==0)
						temItem.SetRareIndex(0);
					//감정된 레어아이템이면...
					else
					{
						_pUIMgr->SetRareOption(istr, temItem);
					}

					btnAuctionItem.SetItemRareIndex(temItem.Item_RareIndex);

					for(SBYTE i = 0; i < MAX_ITEM_OPTION; i++)
					{
						btnAuctionItem.SetItemOptionData(i,temItem.GetOptionType(i),temItem.GetOptionLevel(i));
					}
				}
				//레어 아이템이 아니면.....
				else
				{
					for( SBYTE sbOption = 0; sbOption < nOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> sbOptionLevel;

						btnAuctionItem.SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
					}
				}

				(*istr) >> nCount;
				(*istr) >> nTotalNas;
				(*istr) >> nFinishDay;
				(*istr) >> nFinishDayUnit;

				btnAuctionItem.SetItemCount(nCount);
				_pUIMgr->GetAuction()->SetSlotItem(i, nAutionIndex, btnAuctionItem, nTotalNas, nFinishDay, nFinishDayUnit);
			}

		}
		break;

		case MSG_TRADEAGENT_BUY_REP:
		{
			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(4308, "구입완료"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );
			MsgBoxInfo.AddString(_S(4309, "아이템 구입을 완료하였습니다."));	
			
			_pUIMgr->CreateMessageBox(MsgBoxInfo);	

			CTString strTemp, strTemp2, strItemName;
			SQUAD nTotalNas, nItemCount;

			_pUIMgr->GetAuction()->GetRegisterItemData(strItemName, nItemCount, nTotalNas);

			strTemp2.PrintF("%I64d",nTotalNas);
			_pUIMgr->InsertCommaToString( strTemp2 );
			strTemp.PrintF(_S(4361, "%s나스로 %s 아이템 %I64d개를 구매하셨습니다. 정산 페이지를 확인하십시오."), strTemp2, strItemName, nItemCount);
			_pNetwork->ClientSystemMessage(strTemp, SYSMSG_NOTIFY);
			
			int nPage = _pUIMgr->GetAuction()->GetCurrentPage();

			_pUIMgr->GetAuction()->GoPage(nPage);
			_pUIMgr->GetAuction()->SetSettling(TRUE);
		}
		break;

		case MSG_TRADEAGENT_CALCLIST_REP:
		{
			
			SLONG nCharIndex, nMaxPageNo, nPageNo, nItemCount;

			(*istr) >> nCharIndex;
			(*istr) >> nMaxPageNo;
			(*istr) >> nPageNo;

			_pUIMgr->GetAuction()->SetSettling(TRUE);
			_pUIMgr->GetAuction()->SetTab(AUCTION_TAB_SETTLEMENT);
			_pUIMgr->GetAuction()->SetMaxPage(nMaxPageNo);
			_pUIMgr->GetAuction()->SetCurrentPage(nPageNo);

			(*istr) >> nItemCount;

			CUIButtonEx btnAuctionItem;
			SLONG nItemIndex, nCount, nState, nPassDay, nItemPlus, nItemFlag, nOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;
			CTString strCharName = "";
			SQUAD nTotalNas;
			for(SLONG i=0; i<nItemCount; ++i)
			{
				btnAuctionItem.InitBtn();

				(*istr) >> nItemIndex;
				(*istr) >> nItemPlus;
				(*istr) >> nItemFlag;
				btnAuctionItem.SetItemInfo(-1, -1, -1, nItemIndex, -1, -1);
				btnAuctionItem.SetItemPlus(nItemPlus);
				btnAuctionItem.SetItemFlag(nItemFlag);

				(*istr) >> nOptionCount;
/*
				for(int j=0; j<nOptionCount; j++)
				{
					(*istr) >> nOptionType;
					(*istr) >> nOptionLevel;
					btnAuctionItem.SetItemOptionData(j, nOptionType, nOptionLevel);
				}
*/
				CItemData& ItemData	= _pNetwork->GetItemData(nItemIndex);
				CItems temItem;

				//레어 아이템일때...
				if( ItemData.GetFlag() & ITEM_FLAG_RARE )
				{
					//옵션 개수가 0이면 미감정 레어아이템
					if( nOptionCount ==0)
						temItem.SetRareIndex(0);
					//감정된 레어아이템이면...
					else
					{
						_pUIMgr->SetRareOption(istr, temItem);
					}

					btnAuctionItem.SetItemRareIndex(temItem.Item_RareIndex);

					for(SBYTE i = 0; i < MAX_ITEM_OPTION; i++)
					{
						btnAuctionItem.SetItemOptionData(i,temItem.GetOptionType(i),temItem.GetOptionLevel(i));
					}
				}
				//레어 아이템이 아니면.....
				else
				{
					for( SBYTE sbOption = 0; sbOption < nOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> sbOptionLevel;

						btnAuctionItem.SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
					}
				}

				(*istr) >> nCount;
				(*istr) >> nTotalNas;
				(*istr) >> nState;
				(*istr) >> nPassDay;
				(*istr) >> strCharName;

				btnAuctionItem.SetItemCount(nCount);

				if(nState == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
				{
					btnAuctionItem.SetCharName(CHAR_NAME_BUYER, strCharName);
				}

				if(nState == MSG_TRADEAGENT_STATE_BUY)
				{
					btnAuctionItem.SetCharName(CHAR_NAME_SELLER, strCharName);
				}

				_pUIMgr->GetAuction()->SetSlotItem(i, 0,btnAuctionItem, nTotalNas, nPassDay, nState);
			}

		}
		break;

		case MSG_TRADEAGENT_CALCULATE_REP:
		{
			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(4313, "정산완료"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );
			MsgBoxInfo.AddString(_S(4315, "정산을 완료하였습니다."));	
			
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			_pUIMgr->GetAuction()->SetSettling(FALSE);
			_pUIMgr->GetAuction()->SetSettleRequest(FALSE);
			_pNetwork->SendTradeAgentCalcListReq(1);
		}
		break;

		case MSG_TRADEAGENT_CHECKCALC_REP:
		{
			SLONG nCharIndex, nNeedCalc;
			
			(*istr) >> nCharIndex;
			(*istr) >> nNeedCalc;

			if(nNeedCalc)
			{
				_pUIMgr->GetAuction()->SetSettling(TRUE);
				_pUIMgr->GetNotice()->AddToNoticeList(0, NOTICE_AUCTION);
				_pUIMgr->GetNotice()->RefreshNoticeList();
			}
		}
		break;

		case MSG_TRADEAGENT_NEEDCALC_NOTIFY:
		{
			SLONG nCharIndex, nItemCount;
			UBYTE ucState;
			CTString stItemname, strTemp;

			_pUIMgr->GetAuction()->SetSettling(TRUE);
			_pUIMgr->GetNotice()->AddToNoticeList(0, NOTICE_AUCTION);
			_pUIMgr->GetNotice()->RefreshNoticeList();

			(*istr) >> nCharIndex;
			(*istr) >> ucState;
			(*istr) >> stItemname;
			(*istr) >> nItemCount;

			if(ucState == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
			{
				strTemp.PrintF(_S(4362, "거래 대행중인 %s 아이템 %d개에 대한 판매가 이루어졌습니다."), stItemname, nItemCount);
			}
			else if(ucState == MSG_TRADEAGENT_STATE_SELL_RETURNED)
			{
				strTemp.PrintF(_S(4363, "%s 아이템 %d개가 반송되었습니다. 정산 페이지를 확인하십시오."), stItemname, nItemCount);
			}
			_pNetwork->ClientSystemMessage(strTemp, SYSMSG_NOTIFY);
		}
		break;

		case MSG_TRADEAGENT_ERROR:
		{	
			UBYTE ubErrorType;
			SBYTE sbErrorSubType;
			CTString strError;
			BOOL bShowMessage = TRUE;
			
			(*istr) >> ubErrorType;
			(*istr) >> sbErrorSubType;

			if (sbErrorSubType == MSG_TRADEAGENT_ERROR_PART_REGIST)
			{
				_pUIMgr->GetAuction()->SetRegistering(FALSE); // 등록 요청상태 취소
			}
			else if (sbErrorSubType == MSG_TRADEAGENT_ERROR_PART_CALCULATE)
			{
				_pUIMgr->GetAuction()->SetSettleRequest(FALSE);
			}

			switch(ubErrorType)
			{
				case MSG_TRADEAGENT_ERROR_NORMAL:
					strError = _S(4364, "서비스 이용중 문제가 발생하였습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_TRADEITEM:
					strError = _S(4365, "거래할 수 없는 아이템입니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_ITEMCOUNT:
					strError = _S(4366, "아이템의 개수가 잘못되었습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_SELL_TOTALMONEY:
					strError = _S(4367, "판매금액이 잘못되었습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_ENUGH_GUARANTY:
					strError = _S(4368, "보증금이 부족합니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_ENUGH_MONEY:
					strError = _S(306, "나스가 부족합니다.");
					break;
				case MSG_TRADEAGENT_ERROR_ALREADY_SELLITEM:
					strError = _S(4369, "이미 판매된 아이템입니다.");
					break;
				case MSG_TRADEAGENT_ERROR_FULL_INVENTORY:
					strError = _S(2850, "인벤토리가 가득 찼습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_MAXREGCOUNT_FULL:
					strError = _S(4378, "대행중인 아이템의 최대수를 초과하였습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_CHAR_MAXREGCOUNT_FULL:
					strError = _S(1629, "더 이상 등록할 수 없습니다.");
					break;
				case MSG_TRADEAGENT_ERROR_NOT_BE_ITEM:
					{
						int nSelTab;
						bShowMessage = FALSE;

						switch (sbErrorSubType)
						{
						case MSG_TRADEAGENT_ERROR_PART_SEARCH: // 조회
							{
								strError = _S(4371, "조회된 아이템이 없습니다.");
								nSelTab = AUCTION_TAB_REFER;

								_pUIMgr->GetAuction()->EnableSearchBtn();	// [090611: selo] 서버 응답 처리후 검색 버튼 활성화
							}
							break;
						case MSG_TRADEAGENT_ERROR_PART_REGIST: // 등록
							{
								strError = _S(4370, "등록된 아이템이 없습니다.");
								_pUIMgr->GetAuction()->SetDepositMoney(1000);
								nSelTab = AUCTION_TAB_REGISTER;
							}
							break;
						case MSG_TRADEAGENT_ERROR_PART_CALCULATE:
							{
								strError = _S(4372, "정산할 아이템이 없습니다.");

								_pUIMgr->GetNotice()->DelFromNoticeList(0, NOTICE_AUCTION);
								_pUIMgr->GetNotice()->RefreshNoticeList();
								nSelTab = AUCTION_TAB_SETTLEMENT;
							}
							break;
						}

						_pUIMgr->GetAuction()->SetTab(nSelTab);
						_pNetwork->ClientSystemMessage(strError, SYSMSG_ERROR);
					}
					break;
			}

			if(bShowMessage)
			{
				_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_INFO);
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(4287, "거래 대행 서비스"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );

				MsgBoxInfo.AddString(strError);		
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	}
}
