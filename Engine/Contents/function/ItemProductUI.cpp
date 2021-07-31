#include "stdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "ItemProductUI.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Info/MyInfoSkill.h>

CItemProductUI::CItemProductUI()
	: m_pList(NULL)
{
	setInherit(false);
}

void CItemProductUI::initialize()
{
#ifndef		WORLD_EDITOR
	CUIBase* pBase = NULL;
	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CItemProductUI::CloseUI, this));

	m_pList = (CUIList*)findUI("_list");
#endif // WORLD_EDITOR
}

void CItemProductUI::OpenUI()
{
	if (GetHide() == FALSE || IsVisible() == TRUE)
		return;

	m_pList->DeleteAllListItem();

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_ITEMPRODUCT, TRUE);
}

void CItemProductUI::CloseUI()
{
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_ITEMPRODUCT, FALSE);
}

void CItemProductUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CItemProductUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CItemProductUI::AddData( int idx )
{
	if (m_pList == NULL || idx <= 0)
		return;

	CUIBase* pTmp = m_pList->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;

	m_pList->AddListItem(pTmp->Clone());

	pItem = m_pList->GetListItem(m_pList->getListItemCount() - 1);

	if (pItem == NULL)
		return;

	pIcon = (CUIIcon*)pItem->findUI("icon_");

	if (pIcon == NULL)
		return;

	pIcon->clearIconData();
	pIcon->setData(UBET_SKILL, idx);
	pIcon->SetWhichUI(UI_ITEMPRODUCT);

#ifndef		WORLD_EDITOR
	pIcon->SetCommandFUp(boost::bind(&CItemProductUI::SendMakeItemList, this, idx));
#endif		// WORLD_EDITOR

	pText = (CUIText*)pItem->findUI("text_name");
	if (pText == NULL)
		return;

	CSkill& rData = _pNetwork->GetSkillData(idx);
	CTString strTemp;

	strTemp.PrintF("%s", rData.GetName());
	pText->SetText(strTemp);

	pText = (CUIText*)pItem->findUI("text_exp");
	if (pText == NULL)
		return;

	strTemp.PrintF("%I64d", MY_INFO()->GetSkillExp(idx));
	UIMGR()->InsertCommaToString(strTemp);
	pText->SetText(strTemp);

	m_pList->UpdateList();
	m_pList->GetScroll()->SetScrollCurPos(m_pList->getListItemCount());
}

void CItemProductUI::SendMakeItemList( int idx )
{
	if (UIMGR()->GetInventory()->IsLocked() == TRUE ||
		UIMGR()->GetInventory()->IsLockedArrange() == TRUE)
	{
		UIMGR()->GetInventory()->ShowLockErrorMessage();
		CloseUI();
		return;
	}

	_pNetwork->SendMakeItemList(idx, TRUE);
	CloseUI();
}
