#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildBattleMatchRegUI.h"
#include "GuildBattleMatch.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>

GuildBattleMatchRegUI::GuildBattleMatchRegUI()
	: m_nNas(0)
	, m_nGP(0)
	, m_nTime(0)
	, m_nZone(-1)
	, m_pEdNas(NULL)
	, m_pEdGP(NULL)
	, m_pCbTime(NULL)
	, m_pCbZone(NULL)
{
	setInherit(false);
}

void GuildBattleMatchRegUI::initialize()
{
	CUIManager* pUIMgr = UIMGR();

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

	CUITextBox* pTB = (CUITextBox*)findUI("tb_desc");

	if (pTB != NULL)
	{
		CTString strTmp;
		strTmp = _S(6489, "길드전투 챌린지 목록에 등록을 신청합니다.");
		strTmp += "\\n";
		strTmp += _S(1386, "전투 신청금과 전투시간을 입력하세요.");
		strTmp += "\\n";
		strTmp += _S(6490, "길드전투에서 승리하게 되면 양쪽 길드의 모든 배팅된 전투 신청금을 갖습니다.");
		strTmp += "\\n";
		strTmp += _S(6491, "단, 나스 5%, GP 10%는 수수료로 차감 후 지급됩니다.");

		pTB->SetText(strTmp);
	}

	pTxt = (CUIText*)findUI("txt_nas");

	if (pTxt != NULL)
	{
		CTString strTmp;
		strTmp.PrintF("%s ~ %s", 
			pUIMgr->IntegerToCommaString(GuildBattleMatch::NAS_MIN),
			pUIMgr->IntegerToCommaString(GuildBattleMatch::NAS_MAX));

		pTxt->SetText(strTmp);
	}

	pTxt = (CUIText*)findUI("txt_gp");

	if (pTxt != NULL)
	{
		CTString strTmp;
		strTmp.PrintF("%s ~ %s", 
			pUIMgr->IntegerToCommaString(GuildBattleMatch::GP_MIN),
			pUIMgr->IntegerToCommaString(GuildBattleMatch::GP_MAX));

		pTxt->SetText(strTmp);
	}

	m_pEdNas = (CUIEditBox*)findUI("ed_nas");
	m_pEdGP = (CUIEditBox*)findUI("ed_gp");
	m_pCbTime = (CUIComboBox*)findUI("cb_time");

	if (m_pCbTime != NULL)
	{
		m_pCbTime->setMaxLine(GuildBattleMatch::time_max);		

		CTString strTmp;

		for (int i = 0; i < GuildBattleMatch::time_max; ++i)
		{
			strTmp.PrintF("%d", GuildBattleMatch::arr_time[i]);
			m_pCbTime->AddString(strTmp);
		}
	}

	m_pCbZone = (CUIComboBox*)findUI("cb_zone");

#ifndef		WORLD_EDITOR
	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchRegUI::close, this));

	pBtn = (CUIButton*)findUI("btn_close2");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchRegUI::close, this));

	pBtn = (CUIButton*)findUI("btn_reg");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchRegUI::_regist, this));
#endif		// WORLD_EDITOR
}

void GuildBattleMatchRegUI::init_zone()
{
	if (m_pCbZone != NULL)
	{
		GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

		pMatch->init_zone();

		CTString strTmp;
		int i, zone, max = pMatch->get_zone_cnt();

		m_pCbZone->setMaxLine(max);

		for (i = 0; i < max; ++i)
		{
			zone = pMatch->get_zone_idx(i);
			strTmp = ZONEINFO()->GetZoneName(zone);
			m_pCbZone->AddString(strTmp);
		}
	}
}


void GuildBattleMatchRegUI::open()
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_REG);
}

void GuildBattleMatchRegUI::close()
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_REG, FALSE);

	clear_value();

	if (m_pCbTime != NULL)
		CUIFocus::getSingleton()->killFocus(m_pCbTime);

	if (m_pCbZone != NULL)
		CUIFocus::getSingleton()->killFocus(m_pCbZone);
}

WMSG_RESULT GuildBattleMatchRegUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH_REG);

	return CUIWindow::OnLButtonDown(x, y);
}

BOOL GuildBattleMatchRegUI::IsEditBoxFocused()
{
	if (m_pEdNas != NULL && m_pEdNas->IsFocused())
		return TRUE;

	if (m_pEdGP != NULL && m_pEdGP->IsFocused())
		return TRUE;

	return FALSE;
}





// -----------------------------------------------------------------
// PRIVATE

void GuildBattleMatchRegUI::clear_value()
{
	m_nNas = 0;
	m_nGP = 0;
	m_nTime = 0;
	m_nZone = -1;

	m_pEdNas->SetString("");
	m_pEdGP->SetString("");
	m_pCbTime->SetCurSel(0);
	m_pCbZone->SetCurSel(0);
}

void GuildBattleMatchRegUI::_regist()
{
	eCHECK_VALUE eCHECK = (eCHECK_VALUE)_check_value();

 	if (eCHECK != eCHECK_VALUE_OK)
 	{
 		// msg
		MSGBOX_OK(_S(3877, "입력 오류"), _S(6487, "길드전투 챌린지에 등록할 수 없는 상태입니다."));
 		return;
 	}

	// send challenge
	GAMEDATAMGR()->GetGuildBattleMatch()->SendReqReg(m_nNas, m_nGP, m_nTime, m_nZone);

	close();
}

int GuildBattleMatchRegUI::_check_value()
{
	if (m_pEdNas == NULL || m_pEdGP == NULL || 
		m_pCbTime == NULL || m_pCbZone == NULL)
		return false;

	{
		char* pStr = m_pEdNas->GetString();
		m_nNas = _atoi64(pStr);

		// min, max
		if (m_nNas < GuildBattleMatch::NAS_MIN || m_nNas > GuildBattleMatch::NAS_MAX)
			return eCHECK_VALUE_ERR_NAS_RANGE;

		// 백만 단위 배팅 가능

		if (m_nNas % 1000000)
			return eCHECK_VALUE_ERR_NAS_UNIT;
	}

	{
		char* pStr = m_pEdGP->GetString();
		m_nGP = atoi(pStr);

		// min, max
		if (m_nGP < GuildBattleMatch::GP_MIN || m_nGP > GuildBattleMatch::GP_MAX)
			return eCHECK_VALUE_ERR_GP_RANGE;

		// 천단위 배팅
		if (m_nGP % 1000)
			return eCHECK_VALUE_ERR_GP_UNIT;
	}

	{
		int nCur = m_pCbTime->GetCurSel();

		// 선택 되었는지
		if (nCur < 0)
			return eCHECK_VALUE_ERR_NO_SEL_TIME;

		if (nCur >= GuildBattleMatch::time_max)
			return eCHECK_VALUE_ERR_OVER_TIME_RANGE;

		m_nTime = GuildBattleMatch::arr_time[nCur];
	}

	{
		GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();
		int nCur = m_pCbZone->GetCurSel();

		if (nCur < 0)
			return eCHECK_VALUE_ERR_NO_SEL_ZONE;

		if (nCur >= pMatch->get_zone_cnt())
			return eCHECK_VALUE_ERR_OVER_ZONE_RANGE;

		m_nZone = pMatch->get_zone_idx(nCur);
	}

	return eCHECK_VALUE_OK;
}


