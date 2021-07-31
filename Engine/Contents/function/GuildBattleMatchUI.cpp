#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildBattleMatchUI.h"
#include <Engine/Interface/UIManager.h>
#include "GuildBattleMatchRegUI.h"
#include "GuildBattleMatchChallengeUI.h"
#include "GuildBattleMatch.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>


GuildBattleMatchUI::GuildBattleMatchUI()
	: m_nSelTime(0)
	, m_pCbTime(NULL)
	, m_pList(NULL)
	, m_pBtnRegist(NULL)
{
	setInherit(false);
}

void GuildBattleMatchUI::initialize()
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

#ifndef		WORLD_EDITOR

	m_pList = (CUIList*)findUI("list_");

	if (m_pList != NULL)
	{
		m_pList->SetCommandFSelect(boost::bind(&GuildBattleMatchUI::_ls_sel_callback, this));
	}

	m_pCbTime = (CUIComboBox*)findUI("cb_time");

	if (m_pCbTime != NULL)
	{
		m_pCbTime->setMaxLine(GuildBattleMatch::time_max + 1);

		CTString strTmp;

		strTmp = _S(6038, "모두");
		m_pCbTime->AddString(strTmp);

		for (int i = 0; i < GuildBattleMatch::time_max; ++i)
		{
			strTmp.PrintF("%d", GuildBattleMatch::arr_time[i]);
			m_pCbTime->AddString(strTmp);
		}

		m_pCbTime->SetCommandFUp(boost::bind(&GuildBattleMatchUI::_cb_sel_callback, this));
	}

	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchUI::close, this));

	pBtn = (CUIButton*)findUI("btn_close2");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchUI::close, this));

	m_pBtnRegist = (CUIButton*)findUI("btn_reg");

	if (m_pBtnRegist != NULL)
		m_pBtnRegist->SetCommandFUp(boost::bind(&GuildBattleMatchUI::_regist, this));

	pBtn = (CUIButton*)findUI("btn_challenge");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchUI::_challenge, this));

	pBtn = (CUIButton*)findUI("btn_prev");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchUI::_press_prev, this));

	pBtn = (CUIButton*)findUI("btn_next");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&GuildBattleMatchUI::_press_next, this));
#endif		// WORLD_EDITOR
}

void GuildBattleMatchUI::open()
{
	m_nSelTime = 0;

	// Req data
	GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

	pMatch->set_curpage(0);
	pMatch->set_sel_list(0);
	pMatch->SendReqList(m_nSelTime, 0);	

	if (m_pCbTime != NULL)
		m_pCbTime->SetCurSel(0);

	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH);
}

void GuildBattleMatchUI::close()
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH, FALSE);

	if (m_pCbTime != NULL)
		CUIFocus::getSingleton()->killFocus(m_pCbTime);
}

WMSG_RESULT GuildBattleMatchUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	UIMGR()->RearrangeOrder(UI_GUILD_BATTLE_MATCH);

	return CUIWindow::OnLButtonDown(x, y);
}

void GuildBattleMatchUI::Updatedata()
{
	if (m_bHide == TRUE)
		return;

	if (m_pList == NULL)
		return;

	m_pList->DeleteAllListItem();

	GuildBattleMatch* pGBM = GAMEDATAMGR()->GetGuildBattleMatch();

	if (pGBM == NULL)
		return;

	// list 갱신
	CUIListItem* pClone, *pTemp = m_pList->GetListItemTemplate();
	int i, max = pGBM->get_data_cnt();	
	ResponseClient::GuildBattleData* pData;
	CUIText* pTxt;
	CTString strTmp;
	CUIManager* pUIMgr = UIMGR();

	for (i = 0; i < max; ++i)
	{
		pData = pGBM->get_data(i);

		if (pData == NULL)
			continue;

		pClone = (CUIListItem*)pTemp->Clone();
		m_pList->AddListItem(pClone);

		pTxt = (CUIText*)pClone->findUI("name");
		if (pTxt != NULL)
			pTxt->SetText(CTString(pData->guild_name));

		pTxt = (CUIText*)pClone->findUI("master");
		if (pTxt != NULL)
			pTxt->SetText(CTString(pData->guild_master_name));

		pTxt = (CUIText*)pClone->findUI("num");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%d", pData->guild_member_count);
			pTxt->SetText(strTmp);
		}

		pTxt = (CUIText*)pClone->findUI("avg_lv");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%d", pData->guild_member_level_aver);
			pTxt->SetText(strTmp);
		}

		pTxt = (CUIText*)pClone->findUI("win");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%d / %d", pData->guild_win_count, pData->guild_battle_count);
			pTxt->SetText(strTmp);
		}

		pTxt = (CUIText*)pClone->findUI("nas");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%s", pUIMgr->IntegerToCommaString(pData->stake_nas));
			pTxt->SetText(strTmp);
		}

		pTxt = (CUIText*)pClone->findUI("gp");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%s", pUIMgr->IntegerToCommaString((__int64)pData->stake_gp));
			pTxt->SetText(strTmp);
		}

		pTxt = (CUIText*)pClone->findUI("time");
		if (pTxt != NULL)
		{
			strTmp.PrintF("%d", pData->guild_battle_time);
			pTxt->SetText(strTmp);
		}
	}

	m_pList->UpdateList();

	if (m_pBtnRegist != NULL)
	{
		if (pGBM->isRegist() == true)
		{
			strTmp = _S(4292, "등록취소");
		}
		else
		{
			strTmp = _S(6488, "길드전투 챌린지 등록");
		}

		m_pBtnRegist->SetText(strTmp);
	}
}





// -----------------------------------------------------------------
// PRIVATE

void GuildBattleMatchUI::_cb_sel_callback()
{
	if (m_pCbTime == NULL)
		return;

	int nSel = m_pCbTime->GetCurSel();

	if (nSel > 0)
		nSel = GuildBattleMatch::arr_time[nSel - 1];

	if (nSel != m_nSelTime)
	{
		GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

		m_nSelTime = nSel;
		// 선택된 시간이 바뀌었기 때문에 페이지 번호를 초기화 한다.
		pMatch->set_curpage(0);

		pMatch->SendReqList(m_nSelTime, 0);
	}
}

void GuildBattleMatchUI::_ls_sel_callback()
{
	if (m_pList == NULL)
		return;

	GAMEDATAMGR()->GetGuildBattleMatch()->set_sel_list(m_pList->getCurSel());
}

void GuildBattleMatchUI::_regist()
{
	GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

	bool bReg = pMatch->isRegist();

	// 길드 마스터 검사
	if (_pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS)
	{
		MSGBOX_OK(_S(191, "확인"), _S(1086, "길드장만 길드전투를 신청할 수 있습니다."));
		return;
	}

	if (bReg == false)
	{
		if (_pNetwork->MyCharacterInfo.lGuildLevel < GuildBattleMatch::LIMIT_LV)
		{
			MSGBOX_OK(_S(191, "확인"), _S(6487, "길드전투 챌린지에 등록할 수 없는 상태입니다."));
		}


		// open 등록 창
		UIMGR()->GetGuildBattleMatchReg()->open();
	}
	else
	{
		// 취소 요청
		pMatch->SendReqRegCancel();
	}
}

void GuildBattleMatchUI::_challenge()
{
	if (m_pList == NULL)
		return;

	int nCur = m_pList->getCurSel();
	GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();
	ResponseClient::GuildBattleData* pData = pMatch->get_data(nCur);

	if (pData != NULL)
	{
		pMatch->set_sel_list(nCur);

		UIMGR()->GetGuildBattleMatchChallenge()->open(
			pData->guild_name, pData->stake_nas, pData->stake_gp, 
			pData->guild_battle_time, pData->zone_index);
	}
}

void GuildBattleMatchUI::_press_prev()
{
	GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

	int nCur = pMatch->get_curpage();

	if (nCur > 0)
		pMatch->SendReqList(m_nSelTime, -1);
}

void GuildBattleMatchUI::_press_next()
{
	GuildBattleMatch* pMatch = GAMEDATAMGR()->GetGuildBattleMatch();

	bool bNext = pMatch->isNextPage();

	if (bNext == true)
		pMatch->SendReqList(m_nSelTime, +1);
}


