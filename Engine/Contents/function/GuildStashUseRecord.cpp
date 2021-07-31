
#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "GuildStashUseRecord.h"

CGuildStashUseRecordUI::CGuildStashUseRecordUI()
	: m_pListRecord(NULL)
	, m_pTitle(NULL)
	, m_nLastLogIdx(0)
{
	setInherit(false);
}

CGuildStashUseRecordUI::~CGuildStashUseRecordUI()
{

}

void CGuildStashUseRecordUI::initialize()
{
#ifndef WORLD_EDITOR
	m_pListRecord = (CUIList*)findUI("list_record");
	m_pListRecord->SetKeepSelect(true);

	m_pTitle = (CUIText*)findUI("text_title");

	CUIBase* pBase = NULL;

	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	if (pBase = findUI("btn_more"))
		pBase->SetCommandFUp(boost::bind(&CGuildStashUseRecordUI::GSUseRecordListReq, this));

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CGuildStashUseRecordUI::CloseUI, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CGuildStashUseRecordUI::CloseUI, this));

#endif // WORLD_EDITOR
}

void CGuildStashUseRecordUI::OpenUI(CTString& strGuildName)
{
	CTString strTemp;
	strTemp.PrintF(_S(5544, "[%s]길드 창고 이용 기록"), strGuildName);
	m_pTitle->SetText(strTemp);

	m_pListRecord->DeleteAllListItem();
	m_pListRecord->UpdateScroll(0);

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_GS_USE_RECORD, TRUE);
}

void CGuildStashUseRecordUI::CloseUI()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_GS_USE_RECORD, FALSE);
}

void CGuildStashUseRecordUI::GSUseRecordListRecv( CNetworkMessage* istr )
{
	INDEX		logCnt;
	CTString	logTime;
	CTString	logUserName;
	SBYTE		sbAction;
	INDEX		itemIdx;
	INDEX		itemPlus;
	SQUAD		itemCnt;

	CUIBase* pTmp = m_pListRecord->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CTString strTemp;

	(*istr) >> logCnt;
	for( int i = 0; i < logCnt; ++i )
	{
		(*istr) >> m_nLastLogIdx 
			>> logTime 
			>> logUserName 
			>> sbAction 
			>> itemIdx 
			>> itemPlus 
			>> itemCnt;
		
		pItem = pTmp->Clone();

		if (pItem == NULL)
			continue;

		// 시간
		if (pText = (CUIText*)pItem->findUI("txt_tm"))
		{
			pText->SetText(logTime);
		}

		// 사용한 캐릭터 이름
		if (pText = (CUIText*)pItem->findUI("txt_name"))
		{
			pText->SetText(logUserName);
		}

		// 입/출 여부
		if (pText = (CUIText*)pItem->findUI("txt_inout"))
		{
			if (sbAction > 0)
				strTemp = _S(5556, "출고");
			else
				strTemp = _S(5557, "입고");

			pText->SetText(strTemp);
		}

		// 입/출 아이템 이름.
		if (pText = (CUIText*)pItem->findUI("txt_item_name"))
		{
			CTString strPlus;

			if (itemPlus > 0)
				strPlus.PrintF("+%d  ", itemPlus);

			strTemp = _pNetwork->GetItemName(itemIdx);

			pText->SetText(strPlus + strTemp);
		}

		// 입/출 아이템 수량
		if (pText = (CUIText*)pItem->findUI("txt_cnt"))
		{
			strTemp.PrintF("%I64d", itemCnt);
			UIMGR()->InsertCommaToString(strTemp);
			pText->SetText(strTemp);
		}

		m_pListRecord->AddListItem(pItem);
	}

	m_pListRecord->UpdateList();
	m_pListRecord->UpdateScroll(m_pListRecord->getListItemCount());
}

void CGuildStashUseRecordUI::GSUseRecordListReq()
{
	_pNetwork->SendGuildStashUseRecord(m_nLastLogIdx);
}
