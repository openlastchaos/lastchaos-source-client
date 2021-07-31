#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildBattleMatchChallengeUI.h"
#include "GuildBattleMatch.h"
#include <Engine/GameDataManager/GameDataManager.h>


GuildBattleMatchChallengeUI::GuildBattleMatchChallengeUI()
	: m_pTxtNas(NULL)
	, m_pTxtGP(NULL)
	, m_pTxtTime(NULL)
	, m_pTxtZone(NULL)
	, m_pTxtChallenge(NULL)
{
	setInherit(false);
}

void GuildBattleMatchChallengeUI::initialize()
{
	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pTxtNas = (CUIText*)findUI("txt_nas");
	m_pTxtGP = (CUIText*)findUI("txt_gp");
	m_pTxtTime = (CUIText*)findUI("txt_time");
	m_pTxtZone = (CUIText*)findUI("txt_zone");
	m_pTxtChallenge = (CUIText*)findUI("txt_challenge");

	CUITextBox* pTB = (CUITextBox*)findUI("tb_desc");

	if (pTB != NULL)
	{
		CTString strTmp;
		strTmp = _S(6490, "길드전투에서 승리하게 되면 양쪽 길드의 모든 배팅된 전투 신청금을 갖습니다.");
		strTmp += "\\n";
		strTmp += _S(6491, "단, 나스 5%, GP 10%는 수수료로 차감 후 지급됩니다.");
		strTmp += "\\n";
		strTmp += _S(6492, "[주의] 길드전투 챌린지에 등록하면 전투가 종료될 때 까지 창고 및 GP 사용이 중지됩니다.");

		pTB->SetText(strTmp);
	}

#ifndef		WORLD_EDITOR
	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchChallengeUI::close, this));

	pBtn = (CUIButton*)findUI("btn_close2");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchChallengeUI::close, this));

	pBtn = (CUIButton*)findUI("btn_reg");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchChallengeUI::_challenge, this));
#endif		// WORLD_EDITOR
}

void GuildBattleMatchChallengeUI::open( const char* guild_name, __int64 nas, int gp, int time, int zone )
{
	CUIManager* pUIMgr = UIMGR();
	CTString strTmp;

	if (m_pTxtNas != NULL)
	{
		strTmp.PrintF("%s", pUIMgr->IntegerToCommaString(nas));
		m_pTxtNas->SetText(strTmp);
	}

	if (m_pTxtGP != NULL)
	{
		strTmp.PrintF("%s", pUIMgr->IntegerToCommaString((__int64)gp));
		m_pTxtGP->SetText(strTmp);
	}

	if (m_pTxtTime != NULL)
	{
		strTmp.PrintF("%d", time);
		m_pTxtTime->SetText(strTmp);
	}

	if (m_pTxtZone != NULL)
	{
		strTmp = ZONEINFO()->GetZoneName(zone);
		m_pTxtZone->SetText(strTmp);
	}

	if (m_pTxtChallenge != NULL)
	{
		strTmp.PrintF(_S(6499, "[%s] 길드에게 도전하시겠습니까?"), guild_name);
		m_pTxtChallenge->SetText(strTmp);
	}

	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_CHALLENGE);
}

void GuildBattleMatchChallengeUI::close()
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_CHALLENGE, FALSE);

	GAMEDATAMGR()->GetGuildBattleMatch()->set_sel_list(-1);
}

WMSG_RESULT GuildBattleMatchChallengeUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_CHALLENGE);

	return CUIWindow::OnLButtonDown(x, y);
}

// -----------------------------------------------------------------
// PRIVATE

void GuildBattleMatchChallengeUI::_challenge()
{
	// send challenge
	GAMEDATAMGR()->GetGuildBattleMatch()->SendReqChallenge();

	close();
}
