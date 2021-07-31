#include "StdH.h"
#include "GuildBattleMatch.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include "GuildBattleMatchUI.h"
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Entities/ZoneData.h>

int GuildBattleMatch::arr_time[GuildBattleMatch::time_max] = { 15, 30, 60, 90, 120, 180, 240, 300 };

GuildBattleMatch::GuildBattleMatch()
	: m_bNextPage(false)
	, m_bRegist(false)
	, m_nCurPage(0)
	, m_nSelList(0)
	, m_nCurTime(0)
	, m_nReqCharIdx(0)
{
}

GuildBattleMatch::~GuildBattleMatch()
{
	clear_data();
}

void GuildBattleMatch::clear_data()
{
	int i, max = m_data.size();

	for (i = 0; i < max; ++i)
		SAFE_DELETE(m_data[i]);

	m_data.clear();
}

void GuildBattleMatch::init_zone()
{
	// 특별한 조건이 없음. 
	// 기획에서 정해준 존에서만 가능.
	m_zone.push_back(0);
	m_zone.push_back(4);
	m_zone.push_back(7);
	m_zone.push_back(15);
	m_zone.push_back(23);
	m_zone.push_back(24);
	m_zone.push_back(32);
	m_zone.push_back(39);
	m_zone.push_back(40);
}

void GuildBattleMatch::RecvList( CNetworkMessage* istr )
{
	ResponseClient::GuildBattleList* pRecv = reinterpret_cast<ResponseClient::GuildBattleList*>(istr->GetBuffer());

	m_bNextPage = pRecv->isNext;
	m_bRegist = pRecv->isReg;

	clear_data();

	if (pRecv->count > 0)
	{
		int		i;
		ResponseClient::GuildBattleData* pData;

		for (i = 0; i < pRecv->count; ++i)
		{
			pData = new ResponseClient::GuildBattleData;
			memcpy(pData, &pRecv->data[i], sizeof(*pData));

			m_data.push_back(pData);
		}
	}

	UIMGR()->GetGuildBattleMatch()->Updatedata();
}

void GuildBattleMatch::RecvError( CNetworkMessage* istr )
{
	CTString strError;
	ResponseClient::GuildBattleErrorcode* pRecv = reinterpret_cast<ResponseClient::GuildBattleErrorcode*>(istr->GetBuffer());

	switch (pRecv->error_code)
	{
	case GUILD_BATTLE_ERROR_AREADY:					//이미 성사되었거나 취소된 길드입니다.
	case GUILD_BATTLE_FAIL_CHALLENGE:
		strError = _S(6496, "길드전투 챌린지에 도전할 수 없는 상태입니다.");
		break;
	case GUILD_BATTLE_ERROR_CONDITION_GUILD_MASTER:	// [1086]길드장만 길드전투를 신청 할 수 있습니다.
		strError = _S(1086, "길드장만 길드전투를 신청할 수 있습니다.");
		break;
	case GUILD_BATTLE_ERROR_CONDITION:				//길드전투 챌린지에 등록할 수 없는 상태입니다.
		strError = _S(6487, "길드전투 챌린지에 등록할 수 없는 상태입니다.");
		break;
	case GUILD_BATTLE_SUCCESS_REGIST:				//길드가 길드전투 챌린지에 등록되었습니다.
		strError.PrintF(_S(6494, "[%s]길드가 길드전투 챌린지에 등록되었습니다."), _pNetwork->MyCharacterInfo.strGuildName);
		SendReqList(m_nCurTime, 0);
		break;
	case GUILD_BATTLE_SUCCESS_CANCEL:				//길드의 길드전투 챌린지의 등록이 취소되었습니다.
		strError.PrintF(_S(6495, "[%s]길드의 길드전투 챌린지의 등록이 취소되었습니다."), _pNetwork->MyCharacterInfo.strGuildName);
		SendReqList(m_nCurTime, 0);
		break;
	}

	MSGBOX_OK(_S(191, "확인"), strError);
}

void GuildBattleMatch::RecvChallenge( CNetworkMessage* istr )
{
	UpdateClient::GuildBattleChallengeAgreeUp* pRecv = reinterpret_cast<UpdateClient::GuildBattleChallengeAgreeUp*>(istr->GetBuffer());

	show_challenge(pRecv->guild_name, pRecv->guild_level, 
		pRecv->guild_member_count, pRecv->guild_online_member_count, 
		pRecv->guile_member_level_aver, pRecv->guile_online_member_level_aver,
		pRecv->guild_battle_count, pRecv->guild_win_count, pRecv->guild_nas, pRecv->guild_point);

	m_nReqCharIdx = pRecv->char_index;
}

void GuildBattleMatch::RecvChallengeRes( CNetworkMessage* istr )
{
	CTString strError;
	ResponseClient::GuildBattleChallenge* pRecv = reinterpret_cast<ResponseClient::GuildBattleChallenge*>(istr->GetBuffer());

	switch (pRecv->error_code)
	{
	case GUILD_BATTLE_SUCCESS_CHALLENGE:			//길드전투를 신청하였습니다.
		strError.PrintF(_S(6500, "[%s]과의 길드전투를 신청하였습니다."), pRecv->guild_name);
		break;
	}

	MSGBOX_OK(_S(191, "확인"), strError);
}

void GuildBattleMatch::RecvGuildBattleChallengeAgree( CNetworkMessage* istr )
{
	CTString strError;
	ResponseClient::GuildBattleChallengeAgree* pRecv = reinterpret_cast<ResponseClient::GuildBattleChallengeAgree*>(istr->GetBuffer());

	switch (pRecv->error_code)
	{
	case GUILD_BATTLE_SUCCESS_AGREE:				//길드전투 신청을 받아드렸습니다. 즉시 길드전투가 시작됩니다.
		{			
			strError.PrintF(_S(6506, "[%s]이 길드전투 신청을 받아드렸습니다. [%s]에서 즉시 길드전투가 시작됩니다."), pRecv->guild_name, ZONEINFO()->GetZoneName(pRecv->zone_index));
			UIMGR()->GetGuildBattleMatch()->close();
		}
		break;
	case GUILD_BATTLE_SUCCESS_REFUSE:				//길드전투 신청을 거부하였습니다.
		strError.PrintF(_S(6507, "[%s]이 길드전투 신청을 거부하였습니다."), pRecv->guild_name);
		break;
	}

	MSGBOX_OK(_S(191, "확인"), strError);
}

void GuildBattleMatch::SendReqList( int time, int gap )
{
	int page = m_nCurPage + gap;

	if (page < 0)
		return;

	CNetworkMessage nmMessage;
	RequestClient::GuildBattleList* packet = reinterpret_cast<RequestClient::GuildBattleList*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD_BATTLE;
	packet->subType = MSG_SUB_GUILD_BATTLE_LIST;
	packet->guild_battle_time = time;
	packet->page_num = page;

	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );

	m_nCurTime = time;
	m_nCurPage = page;

	// 다음 페이지 요청 시 리스트를 받기 전에 Next 를 false 로 해놓으면 잠금이 된다.
	if (gap > 0)
		m_bNextPage = false;
}

void GuildBattleMatch::SendReqReg( __int64 nas, int gp, int time, int zone )
{
	CNetworkMessage nmMessage;
	RequestClient::GuildBattleRegist* packet = reinterpret_cast<RequestClient::GuildBattleRegist*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD_BATTLE;
	packet->subType = MSG_SUB_GUILD_BATTLE_REGIST;
	packet->stake_nas = nas;
	packet->stake_gp = gp;
	packet->guild_battle_time = time;
	packet->zone_index = zone;

	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );
}

void GuildBattleMatch::SendReqRegCancel()
{
	CNetworkMessage nmMessage;
	RequestClient::GuildBattleRegCancel* packet = reinterpret_cast<RequestClient::GuildBattleRegCancel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD_BATTLE;
	packet->subType = MSG_SUB_GUILD_BATTLE_CANCEL;
	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );
}

bool GuildBattleMatch::SendReqChallenge()
{
	if (m_nSelList < 0 || m_data.size() <= m_nSelList)
		return false;

	CNetworkMessage nmMessage;
	RequestClient::GuildBattleChallenge* packet = reinterpret_cast<RequestClient::GuildBattleChallenge*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD_BATTLE;
	packet->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE;
	packet->guild_index = m_data[m_nSelList]->guild_index;

	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );

	return true;
}

void GuildBattleMatch::SendResChallenge( bool bYes )
{
	CNetworkMessage nmMessage;
	RequestClient::GuildBattleChallengeAgree* packet = reinterpret_cast<RequestClient::GuildBattleChallengeAgree*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD_BATTLE;
	packet->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE;
	packet->char_index = m_nReqCharIdx;
	packet->isAgree = bYes;
	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );
}

ResponseClient::GuildBattleData* GuildBattleMatch::get_data( int idx )
{
	if (idx < 0 || idx >= m_data.size())
		return NULL;

	return m_data[idx];
}

void GuildBattleMatch::show_challenge(const char* guild_name, int glv, 
										int mem_total, int mem_online, int avg_lv, int avg_lv_online,
										int war_cnt, int win_cnt, __int64 nas, int gp)
{
	CUIManager* pUIMgr = UIMGR();
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage, strTitle;

	if (pUIMgr->DoesMessageBoxExist(MSGCMD_GUILDBATTLE_CHALLENGE) == TRUE)
		pUIMgr->CloseMessageBox(MSGCMD_GUILDBATTLE_CHALLENGE);

	strTitle.PrintF(_S(6501, "[%s] 길드로 부터 길드전투 도전장이 도착하였습니다."), guild_name);	
	MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_YESNO, UI_NONE, MSGCMD_GUILDBATTLE_CHALLENGE);

	MsgBoxInfo.m_nWidth = 350;

	strMessage.PrintF("%s : %d", _S(3861, "길드레벨"), glv);
	MsgBoxInfo.AddString(strMessage);

	//strMessage.PrintF("%s : %d (%s : %d)", _S(893, "길드원"), mem_total, _S(895, "온라인"), mem_online);
	strMessage.PrintF("%s : %d", _S(893, "길드원"), mem_total);
	MsgBoxInfo.AddString(strMessage);

	//strMessage.PrintF("%s : %d (%s : %d)", _S(3863, "길드 평균 레벨"), avg_lv, _S(895, "온라인"), avg_lv_online);
	strMessage.PrintF("%s : %d", _S(3863, "길드 평균 레벨"), avg_lv);
	MsgBoxInfo.AddString(strMessage);

	strMessage.PrintF("%s : %d", _S(6502, "총 길드전투 진행 횟수"), war_cnt);
	MsgBoxInfo.AddString(strMessage);

	strMessage.PrintF("%s : %d", _S(6503, "승리한 길드전투 수"), win_cnt);
	MsgBoxInfo.AddString(strMessage);

	strMessage = " ";
	MsgBoxInfo.AddString(strMessage);

	strMessage.PrintF(_S(6504, "이 길드전투의 신청금은 [%s] 나스와 [%s] 길드포인트 입니다."), 
		pUIMgr->IntegerToCommaString(nas), pUIMgr->IntegerToCommaString((__int64)gp));
	MsgBoxInfo.AddString(strMessage);

	strMessage = _S(6505, "도전 신청을 받겠습니까?");
	MsgBoxInfo.AddString(strMessage);

	pUIMgr->CreateMessageBox(MsgBoxInfo);
}



// PRIVATE
