#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildTaxHistory.h"

GuildTaxHistoryUI::GuildTaxHistoryUI()
	: m_pList(NULL)
{
	setInherit(false);
}

void GuildTaxHistoryUI::initialize()
{
	m_pList = (CUIList*)findUI("list_history");

#ifndef		WORLD_EDITOR
	CUIButton* pBtn = (CUIButton*)findUI("btn_ok");

	if (pBtn != NULL)
	{
		pBtn->SetCommandFUp(boost::bind(&GuildTaxHistoryUI::close, this));
	}
#endif		// WORLD_EDITOR
}

void GuildTaxHistoryUI::open()
{
	update_list();
	UIMGR()->RearrangeOrder(UI_GUILDTAXHISTORY, TRUE);
}

void GuildTaxHistoryUI::close()
{
	UIMGR()->RearrangeOrder(UI_GUILDTAXHISTORY, FALSE);
}

void GuildTaxHistoryUI::AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney )
{
	STakeInfo TakeInfo;
	TakeInfo.SetData( nMonth, nDay, llMoney );

	m_vTakeInfo.push_back(TakeInfo);
}

void GuildTaxHistoryUI::ResetTakeInfo()
{
	m_vTakeInfo.clear();

	if (m_pList == NULL)
		return;

	update_list();
}

void GuildTaxHistoryUI::update_list()
{
	if (m_pList == NULL)
		return;

	m_pList->DeleteAllListItem();

	VTake_iter iter = m_vTakeInfo.begin();
	VTake_iter eiter = m_vTakeInfo.end();

	CUIListItem* pTempl = m_pList->GetListItemTemplate();
	CUIListItem* pClone = NULL;
	CUIText* pTxt;

	CUIManager* pUIManager = UIMGR();

	for (VTake_iter it = iter; it != eiter; ++it)
	{
		CTString strtmp;

		if( it->m_nMonth == 0 ) break;

		pClone = (CUIListItem*)pTempl->Clone();
		m_pList->AddListItem(pClone);

		pTxt = (CUIText*)pClone->findUI("txt_date");
		if (pTxt != NULL)
		{
			strtmp.PrintF( "%02d / %02d", it->m_nMonth, it->m_nDay );
			pTxt->SetText(strtmp);
		}

		pTxt = (CUIText*)pClone->findUI("txt_gold");
		if (pTxt != NULL)
		{
			strtmp.PrintF("%I64d", it->m_llMoney);
			pUIManager->InsertCommaToString(strtmp);
			pTxt->SetText(strtmp);
			pTxt->setFontColor(pUIManager->GetNasColor(it->m_llMoney));
		}
	}

	m_pList->UpdateList();
}