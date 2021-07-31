#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include "PetItemMixUI.h"
#include <Engine/Interface/UIInventory.h>

enum eChangeSelection
{
	DRAGON_WEAPON = 2,		// 드래곤 무기
	KNIGHT_ARMOR,			// 나이트 방어
};

#define DEF_DRAGON_WEAPON_MAX 12
#define DEF_KNIGHT_ARMOR_MAX 5
#define DEF_UNI_KNIGHT_DEFENDSHIELD 1280

#define DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT 3
#define DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT 3

//--------------------------------------------------------------
// Make Item
class CmdPetItemMixMakeItem : public Command
{
public:
	CmdPetItemMixMakeItem() : pUI_(NULL), BtnType_(0) {}
	void setData(CPetItemMixUI* pUI, UBYTE btnType)	{ pUI_ = pUI; BtnType_ = btnType; }
	void execute() {
		if (pUI_)
			pUI_->SendPetItemMixReq(BtnType_);
	}
private:
	CPetItemMixUI* pUI_;
	UBYTE BtnType_;
};

//--------------------------------------------------------------
// Select Item
class CmdPetItemMixSelectItem : public Command
{
public:
	CmdPetItemMixSelectItem() : pUI_(NULL), nItemListIndex_(0), nOpenType_(0) {}
	void setData(CPetItemMixUI* pUI, int listIndex, int OpenType)	
	{ 
		pUI_ = pUI;
		nItemListIndex_ = listIndex;
		nOpenType_ = OpenType;
	}
	void execute() {
		if (pUI_)
			pUI_->SelectItem(nItemListIndex_, nOpenType_);
	}
private:
	CPetItemMixUI* pUI_;
	int nItemListIndex_;
	int nOpenType_;
};

CPetItemMixUI::CPetItemMixUI()
	: m_pMoveArea(NULL)
	, m_pTitle(NULL)
	, m_ptbGuide(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
	int i;
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eLIST_MAX; ++i)
		m_pList[i] = NULL;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	for (i = 0; i < T_KNIGHT_MAX; i++)
		memset ( &m_NStuffKnight[i], 0, sizeof(CNeedStuff) );

	setInherit(false);
}

void CPetItemMixUI::initialize()
{
#ifndef		WORLD_EDITOR

	int i;
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_cancel", "btn_first", "btn_middle" };

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CPetItemMixUI::closeUI, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CPetItemMixUI::closeUI, this));

	std::string strListID[eLIST_MAX] = { "list_makeitem", "list_metitem" };

	for (i = 0; i < eLIST_MAX; ++i)
		m_pList[i] = (CUIList*)findUI(strListID[i].c_str());

	m_pTitle = (CUIText*)findUI("str_title");
	m_pMoveArea = (CUIImage*)findUI("move_area");
	m_ptbGuide = (CUITextBox*)findUI("tb_firstguide");

	_InitNeedItem();

#endif		// WORLD_EDITOR
}

void CPetItemMixUI::openUI(int nType)
{
	if (GetHide() == FALSE)
		return;

	CUIManager* pUIMgr = UIMGR();

	Hide(FALSE);
	SetVisible(TRUE);

	switch(nType)
	{
	case DRAGON_WEAPON:
		_setWeaponUI();
		break;
	case KNIGHT_ARMOR:
		_setArmorUI();
		break;
	}

	pUIMgr->RearrangeOrder( UI_PET_ITEM_MIX, TRUE );
}

void CPetItemMixUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_PET_ITEM_MIX, FALSE );
}

WMSG_RESULT CPetItemMixUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();
	return WMSG_FAIL;
}

WMSG_RESULT CPetItemMixUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_PET_ITEM_MIX, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CPetItemMixUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->ResetHoldBtn();
	return WMSG_FAIL;
}

void CPetItemMixUI::_setWeaponUI()
{
	CTString strTemp;
	// Sub Title
	if (m_pTitle != NULL)
	{
		strTemp.PrintF(_S(2460, "드래곤 웨폰 제작"));
		m_pTitle->SetText(strTemp);
	}
	// Make Item List Set
	_setMakeItemListUI(DRAGON_WEAPON);

	// guide Set
	_setGuild();

	// Btn set
	if (m_pBtn[eBTN_FIRST] != NULL)
		m_pBtn[eBTN_FIRST]->Hide(TRUE);

	if (m_pBtn[eBTN_MIDDLE] != NULL)
	{
		strTemp.PrintF(_S(2459, "제작"));
		m_pBtn[eBTN_MIDDLE]->SetText(strTemp);
		m_pBtn[eBTN_MIDDLE]->SetEnable(FALSE);

		CmdPetItemMixMakeItem* pCmd = new CmdPetItemMixMakeItem;
		pCmd->setData(this, 0);
		m_pBtn[eBTN_MIDDLE]->SetCommandUp(pCmd);		
	}
}

void CPetItemMixUI::_setArmorUI()
{
	CTString strTemp;
	// Sub Title
	if (m_pTitle != NULL)
	{
		strTemp.PrintF(_S(2461, "나이트 메어 방어구 제작"));
		m_pTitle->SetText(strTemp);
	}

	// Make Item List Set
	_setMakeItemListUI(KNIGHT_ARMOR);

	// guide Set
	_setGuild();

	// Btn set
	if (m_pBtn[eBTN_FIRST] != NULL)
	{
		m_pBtn[eBTN_FIRST]->Hide(FALSE);
		strTemp.PrintF(_S(2457, "제작 A"));
		m_pBtn[eBTN_FIRST]->SetText(strTemp);
		m_pBtn[eBTN_FIRST]->SetEnable(FALSE);

		CmdPetItemMixMakeItem* pCmd = new CmdPetItemMixMakeItem;
		pCmd->setData(this, 0);
		m_pBtn[eBTN_FIRST]->SetCommandUp(pCmd);	
	}

	if (m_pBtn[eBTN_MIDDLE] != NULL)
	{
		strTemp.PrintF(_S(2458, "제작 B"));
		m_pBtn[eBTN_MIDDLE]->SetText(strTemp);
		m_pBtn[eBTN_MIDDLE]->SetEnable(FALSE);

		CmdPetItemMixMakeItem* pCmd = new CmdPetItemMixMakeItem;
		pCmd->setData(this, 1);
		m_pBtn[eBTN_MIDDLE]->SetCommandUp(pCmd);	
	}
}

void CPetItemMixUI::_setMakeItemListUI( int nType )
{
	if (m_pList[eLIST_MAKE_ITEM] == NULL)
		return;

	m_pList[eLIST_MAKE_ITEM]->DeleteAllListItem();

	CUIListItem* pTemp = m_pList[eLIST_MAKE_ITEM]->GetListItemTemplate();

	if (pTemp == NULL)
		return;

	CUIListItem* pItem = NULL;
	CUIIcon* pItemIcon = NULL;
	CUIText* pItemText = NULL;

	if( DRAGON_WEAPON == nType )
	{
		int nWeaPonIndex[DEF_DRAGON_WEAPON_MAX] = 
		{
			887,	// 드래곤 소드
			907,	// 듀얼 드래곤 소드
			908,	// 드래곤 블레이드
			909,	// 드래곤 액스
			910,	// 드래곤 완드
			911,	// 드래곤 보우
			912,	// 드래곤 스틱
			913,	// 드래곤 스테프
			914,	// 드래곤 대거
			915,	// 드래곤 암릿 보우
			916,	// 드래곤 사이드
			917,	// 드래곤 로드
		};

		for( int i = 0; i < DEF_DRAGON_WEAPON_MAX; i++ )
		{
			CItemData* pItemData = _pNetwork->GetItemData( nWeaPonIndex[i] );
			if( pItemData->GetItemIndex() == -1 ) 
				continue;

			m_pList[eLIST_MAKE_ITEM]->AddListItem(pTemp->Clone());

			pItem = (CUIListItem*)m_pList[eLIST_MAKE_ITEM]->GetListItem(i);

			if (pItem == NULL)
				continue;

			pItemIcon = (CUIIcon*)pItem->findUI("icon_makeitem");

			if (pItemIcon != NULL)
			{
				pItemIcon->setData(UBET_ITEM, nWeaPonIndex[i]);
				pItemIcon->SetWhichUI(UI_PET_ITEM_MIX);
			}

			pItemText = (CUIText*)pItem->findUI("strMakename");

			if (pItemText != NULL)
				pItemText->SetText(CTString(pItemData->GetName()));

			CmdPetItemMixSelectItem* pCmd = new CmdPetItemMixSelectItem;
			pCmd->setData(this, i, DRAGON_WEAPON);
			pItem->SetCommand(pCmd);
		}
	}
	else if( KNIGHT_ARMOR == nType )
	{	
		UBYTE ubJob = _pNetwork->MyCharacterInfo.job;

		if (ubJob == NIGHTSHADOW)
			return;

		int nArmorIndex[TOTAL_JOB] = 
		{
			918, // 타이탄
			923, // 기사
			938, // 힐러
			928, // 메이지
			933, // 로그
			943, // 소서러
			0,	 // 나이트 쉐도우는 이용불가.
#ifdef CHAR_EX_ROGUE
			933, // EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
			928, // EX메이지 추가 
#endif
		};

		for( int i = 0; i < DEF_KNIGHT_ARMOR_MAX; i++ )
		{
			CItemData* pItemData = _pNetwork->GetItemData(nArmorIndex[ubJob] + i);

			if( pItemData->GetItemIndex() == -1 ) 
				continue;

			m_pList[eLIST_MAKE_ITEM]->AddListItem(pTemp->Clone());

			pItem = (CUIListItem*)m_pList[eLIST_MAKE_ITEM]->GetListItem(i);

			if (pItem == NULL)
				continue;

			pItemIcon = (CUIIcon*)pItem->findUI("icon_makeitem");

			if (pItemIcon != NULL)
			{
				pItemIcon->setData(UBET_ITEM, nArmorIndex[ubJob] + i);
				pItemIcon->SetWhichUI(UI_PET_ITEM_MIX);
			}

			pItemText = (CUIText*)pItem->findUI("strMakename");

			if (pItemText != NULL)
				pItemText->SetText(CTString(pItemData->GetName()));

			CmdPetItemMixSelectItem* pCmd = new CmdPetItemMixSelectItem;
			pCmd->setData(this, i, KNIGHT_ARMOR);
			pItem->SetCommand(pCmd);
		}

		if( ubJob == KNIGHT )
		{
			CItemData* pItemData = _pNetwork->GetItemData(DEF_UNI_KNIGHT_DEFENDSHIELD);
			if( pItemData->GetItemIndex() == -1 ) 
				return;

			m_pList[eLIST_MAKE_ITEM]->AddListItem(pTemp->Clone());

			int nSize = m_pList[eLIST_MAKE_ITEM]->getListItemCount() - 1;

			pItem = (CUIListItem*)m_pList[eLIST_MAKE_ITEM]->GetListItem(nSize);

			if (pItem == NULL)
				return;

			pItemIcon = (CUIIcon*)pItem->findUI("icon_makeitem");

			if (pItemIcon != NULL)
			{
				pItemIcon->setData(UBET_ITEM, DEF_UNI_KNIGHT_DEFENDSHIELD);
				pItemIcon->SetWhichUI(UI_PET_ITEM_MIX);
			}

			pItemText = (CUIText*)pItem->findUI("strMakename");

			if (pItemText != NULL)
				pItemText->SetText(CTString(pItemData->GetName()));

			CmdPetItemMixSelectItem* pCmd = new CmdPetItemMixSelectItem;
			pCmd->setData(this, nSize, KNIGHT_ARMOR);
			pItem->SetCommand(pCmd);
		}
	}

	int nItemCount = m_pList[eLIST_MAKE_ITEM]->getListItemCount();
	m_pList[eLIST_MAKE_ITEM]->setCurSel(-1);
	CUIScrollBar* pScroll = m_pList[eLIST_MAKE_ITEM]->GetScroll();
	if (pScroll != NULL)
		pScroll->SetScrollCurPos(0);

	m_pList[eLIST_MAKE_ITEM]->UpdateScroll(nItemCount);
	m_pList[eLIST_MAKE_ITEM]->UpdateList();
}

void CPetItemMixUI::_setGuild()
{
	if (m_ptbGuide != NULL)
		m_ptbGuide->Hide(FALSE);

	if (m_pList[eLIST_METERIAL_ITEM] != NULL)
		m_pList[eLIST_METERIAL_ITEM]->Hide(TRUE);
}

void CPetItemMixUI::PetItemMixRep( SBYTE sbResult )
{
	// Show result
	CTString	strMessage;

	switch( sbResult )
	{
	case MSG_EX_PET_MIX_ITEM_ERROR_OK:
		strMessage = _S( 580, "제작에 성공 하였습니다." );										
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_FAIL:
		strMessage = _S( 577, "제작에 실패 하였습니다." );
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_NOITEM:
		strMessage = _S( 2465, "제작에 필요한 재료가 부족합니다." );
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_NOMIX:
		strMessage = _S( 2466, "제작에 적합하지 않은 조합입니다." );
		break;
	}

	closeUI();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2459, "제작" ), UMBS_OK, UI_PET_ITEM_MIX, MSG_EX_PET_MIX_ITEM );		
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

void CPetItemMixUI::SendPetItemMixReq(int nBtnType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 941, "스킬 사용중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 942, "순간 이동중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	if (m_pList[eLIST_MAKE_ITEM] == NULL)
		return;

	int nSelItemIdx = m_pList[eLIST_MAKE_ITEM]->getCurSel();

	if (nSelItemIdx < 0)
		return;

	CUIListItem* pItem = (CUIListItem*)m_pList[eLIST_MAKE_ITEM]->GetListItem(nSelItemIdx);

	if (pItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_makeitem");

	if (pIcon == NULL)
		return;

	int nItemIdx = pIcon->getIndex();

	if (nItemIdx <= 0)
		return;

	_pNetwork->SendPetItemMix ( nItemIdx, nBtnType );

	if (m_pBtn[eBTN_FIRST] != NULL)
		m_pBtn[eBTN_FIRST]->SetEnable(FALSE);

	if (m_pBtn[eBTN_MIDDLE] != NULL)
		m_pBtn[eBTN_MIDDLE]->SetEnable(FALSE);	
}

void CPetItemMixUI::_InitNeedItem( void )
{
	m_NStuffKnight[T_HELMET].nNeedA[0] = 886; // 25%	투구
	m_NStuffKnight[T_HELMET].nNeedA[1] = 889;
	m_NStuffKnight[T_HELMET].nNeedA[2] = 894;
	m_NStuffKnight[T_HELMET].nNeedB[0] = 888; // 75%	
	m_NStuffKnight[T_HELMET].nNeedB[1] = 891;
	m_NStuffKnight[T_HELMET].nNeedB[2] = 894;

	m_NStuffKnight[T_COAT].nNeedA[0] = 891; // 25%		상의
	m_NStuffKnight[T_COAT].nNeedA[1] = 892;
	m_NStuffKnight[T_COAT].nNeedA[2] = 894;
	m_NStuffKnight[T_COAT].nNeedB[0] = 893; // 75%
	m_NStuffKnight[T_COAT].nNeedB[1] = 889;
	m_NStuffKnight[T_COAT].nNeedB[2] = 894;

	m_NStuffKnight[T_PANTS].nNeedA[0] = 886; // 25%		하의
	m_NStuffKnight[T_PANTS].nNeedA[1] = 891;
	m_NStuffKnight[T_PANTS].nNeedA[2] = 894;
	m_NStuffKnight[T_PANTS].nNeedB[0] = 890; // 75%
	m_NStuffKnight[T_PANTS].nNeedB[1] = 892;
	m_NStuffKnight[T_PANTS].nNeedB[2] = 894;

	m_NStuffKnight[T_BOOTS].nNeedA[0] = 889; // 25%		부츠
	m_NStuffKnight[T_BOOTS].nNeedA[1] = 892;
	m_NStuffKnight[T_BOOTS].nNeedA[2] = 894;
	m_NStuffKnight[T_BOOTS].nNeedB[0] = 890; // 75%
	m_NStuffKnight[T_BOOTS].nNeedB[1] = 886;
	m_NStuffKnight[T_BOOTS].nNeedB[2] = 894;

	m_NStuffKnight[T_GLOVE].nNeedA[0] = 889; // 25%		장갑
	m_NStuffKnight[T_GLOVE].nNeedA[1] = 892;
	m_NStuffKnight[T_GLOVE].nNeedA[2] = 894;
	m_NStuffKnight[T_GLOVE].nNeedB[0] = 888; // 75%
	m_NStuffKnight[T_GLOVE].nNeedB[1] = 886;
	m_NStuffKnight[T_GLOVE].nNeedB[2] = 894;

	m_NStuffKnight[T_SHIELD].nNeedA[0] = 886; // 25%	방패
	m_NStuffKnight[T_SHIELD].nNeedA[1] = 892;
	m_NStuffKnight[T_SHIELD].nNeedA[2] = 894;
	m_NStuffKnight[T_SHIELD].nNeedB[0] = 893; // 75%
	m_NStuffKnight[T_SHIELD].nNeedB[1] = 891;
	m_NStuffKnight[T_SHIELD].nNeedB[2] = 894;
}

void CPetItemMixUI::SelectItem( int nIndex, int nOpenType )
{
	if ( nIndex < 0 )
		return;

	if (m_pList[eLIST_METERIAL_ITEM] == NULL)
		return;

	if (m_pList[eLIST_MAKE_ITEM] == NULL)
		return;

	int	 i;
	BOOL bNeedItemA			= FALSE;	
	BOOL bNeedItemB			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	BOOL bNeedItems[6];
	for( i = 0; i < 6; i++ )
		bNeedItems[i] = FALSE;

	CUIListItem* pItem = (CUIListItem*)m_pList[eLIST_MAKE_ITEM]->GetListItem(nIndex);

	if (pItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_makeitem");

	if (pIcon == NULL)
		return;

	int nItemIdx = pIcon->getIndex();

	if (nItemIdx <= 0)
		return;

	// 생산 하고자 하는 아이템 목록
	CItemData* pProcessItemData = _pNetwork->GetItemData(nItemIdx);

	// 필요 아이템 
	int nIndexTemp = 0;
	int NeedCntA[MAX_MAKE_ITEM_MATERIAL] = {0,};
	int NeedCntB[MAX_MAKE_ITEM_MATERIAL] = {0,};
	int nResultA = 0, nResultB = 0;
	CItemData* pInvenItem = NULL;

	int		nCnt;
	//필요 아이템 종류와 갯수 얻기 
	if( nOpenType == DRAGON_WEAPON )
	{
		for( i = 0; i < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; i++ )
		{
			nIndexTemp = pProcessItemData->GetNeedItemIndex( i );

			if( nIndexTemp == -1 ) return;

			m_NeedItems[i].ItemData	= _pNetwork->GetItemData( nIndexTemp );
			m_NeedItems[i].llCount	= 1; 
		}

		int		t, i, each;

		for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
		{
			if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
			else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
			else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

			for (i = 0 ; i < each; ++i)
			{

				pInvenItem = _pNetwork->MySlotItem[t][i].ItemData;

				for( nCnt = 0; nCnt < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt].ItemData != NULL &&
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt].ItemData->GetItemIndex() )
					{
						NeedCntA[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt].MatTab = t;
						m_NeedItems[nCnt].inven_idx = i;
						bNeedItems[nCnt] = TRUE;
					}
				}
			}
		}


		for ( i = 0; i < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; i++ )
		{
			nResultA += NeedCntA[i];
		}

		if( nResultA == DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT )	bNeedItemA = TRUE;	// 조합 아이템 조건 만족

	}
	else if( nOpenType == KNIGHT_ARMOR )
	{
		int nJob = _pNetwork->MyCharacterInfo.job;

		for( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++ )
		{
			m_NeedItems[i].ItemData = _pNetwork->GetItemData( m_NStuffKnight[nIndex].nNeedA[i] );
			m_NeedItems[i].llCount	= 1; 
		}
		for( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++ )
		{
			m_NeedItems[i+3].ItemData = _pNetwork->GetItemData( m_NStuffKnight[nIndex].nNeedB[i] );
			m_NeedItems[i+3].llCount	= 1; 
		}

		int		t, i, each;

		for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
		{
			if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
			else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
			else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

			for (i = 0 ; i < each; ++i)
			{

				pInvenItem = _pNetwork->MySlotItem[t][i].ItemData;

				for( nCnt = 0; nCnt < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt].ItemData != NULL && 
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt].ItemData->GetItemIndex() )
					{
						NeedCntA[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt].MatTab = t;
						m_NeedItems[nCnt].inven_idx = i;
						bNeedItems[nCnt] = TRUE;
					}
				}

				for( nCnt = 0; nCnt < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt+3].ItemData !=NULL && 
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt+3].ItemData->GetItemIndex())
					{
						NeedCntB[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt+3].MatTab = t;
						m_NeedItems[nCnt+3].inven_idx = i;
						bNeedItems[nCnt+3] = TRUE;
					}
				}
			}
		}

		for ( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++)
		{
			nResultA += NeedCntA[i];
			nResultB += NeedCntB[i];
		}

		if( nResultA == DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT )	bNeedItemA = TRUE;	// 조합 아이템 조건 만족
		if( nResultB == DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT )	bNeedItemB = TRUE;	// 조합 아이템 조건 만족
	}

	// 재료 출력 초기화
	m_pList[eLIST_METERIAL_ITEM]->DeleteAllListItem();

	m_pList[eLIST_METERIAL_ITEM]->Hide(FALSE);

	if (m_ptbGuide != NULL)
		m_ptbGuide->Hide(TRUE);

	CUIListItem* pTemp = m_pList[eLIST_METERIAL_ITEM]->GetListItemTemplate();

	if (pTemp == NULL)
		return;

	CUIListItem* pMetItem = NULL;
	CUIText* pTextTemp = NULL;
	// 조건 출력 
	CTString strTitle;
	CTString strSpace = "";
	CTString strJobNames[TOTAL_JOB+1] =
	{
		_S( 44, "기사" ), _S( 43, "타이탄" ), _S( 45, "힐러" ),
		_S( 46, "메이지" ), _S( 47, "로그" ), _S( 48,"소서러" ), "",
	};

	std::string strMeterilID[3] = {"str_met0", "str_met1", "str_met2"};
	CTString strCount;
	// 필요 스킬 출력 
	if( nOpenType == DRAGON_WEAPON )
	{
		m_pList[eLIST_METERIAL_ITEM]->AddListItem(pTemp->Clone());

		pMetItem = (CUIListItem*)m_pList[eLIST_METERIAL_ITEM]->GetListItem(0);

		if (pMetItem == NULL)
			return;

		pTextTemp = (CUIText*)pMetItem->findUI("str_subtitle");

		if (pTextTemp != NULL)
		{
			strTitle.PrintF( _S( 2462, "필요 재료" ) );
			strTitle = strTitle + "(" + strJobNames[nIndex/2] + ")";
			
			pTextTemp->SetText(strTitle);
		}		
		
		for( i = 0; i < 3; ++i )
		{
			const char* szItemName = NULL;
			if (m_NeedItems[i].ItemData != NULL)
			{
				szItemName = m_NeedItems[i].ItemData->GetName();
				strCount = UIMGR()->IntegerToCommaString(m_NeedItems[i].llCount);
				strTitle.PrintF ( _S( 576, "%s : %s개" ), strSpace + szItemName, strCount );
			}

			pTextTemp = (CUIText*)pMetItem->findUI(strMeterilID[i].c_str());

			if (pTextTemp != NULL)
			{
				pTextTemp->SetText(strTitle);
				pTextTemp->setFontColor(bNeedItems[i]? COLOR_TEXT : COLOR_NONE);
			}
		}

		if (m_pBtn[eBTN_MIDDLE])
			m_pBtn[eBTN_MIDDLE]->SetEnable(bNeedItemA);
	}
	else if( nOpenType == KNIGHT_ARMOR )
	{
		int subTitleId[2] = { 2463, 2464 };

		for (int j = 0; j < 2; ++j)
		{
			m_pList[eLIST_METERIAL_ITEM]->AddListItem(pTemp->Clone());

			pMetItem = (CUIListItem*)m_pList[eLIST_METERIAL_ITEM]->GetListItem(j);

			if (pMetItem == NULL)
				return;

			pTextTemp = (CUIText*)pMetItem->findUI("str_subtitle");

			if (pTextTemp != NULL)
			{
				strTitle.PrintF( _S( subTitleId[j], "필요 재료" ) );
				pTextTemp->SetText(strTitle);
			}		

			for( i = 0; i < 3; ++i )
			{
				const char* szItemName = NULL;
				if (m_NeedItems[i + (j * 3)].ItemData != NULL)
				{
					szItemName = m_NeedItems[i + (j * 3)].ItemData->GetName();
					strCount = UIMGR()->IntegerToCommaString(m_NeedItems[i + (j * 3)].llCount);
					strTitle.PrintF ( _S( 576, "%s : %s개" ), strSpace + szItemName, strCount );
				}

				pTextTemp = (CUIText*)pMetItem->findUI(strMeterilID[i].c_str());

				if (pTextTemp != NULL)
				{
					pTextTemp->SetText(strTitle);
					pTextTemp->setFontColor(bNeedItems[i + (j * 3)]? COLOR_TEXT : COLOR_NONE);
				}
			}
		}

		m_pList[eLIST_METERIAL_ITEM]->UpdateList();
	
		if (m_pBtn[eBTN_FIRST])
			m_pBtn[eBTN_FIRST]->SetEnable(bNeedItemA);

		if (m_pBtn[eBTN_MIDDLE])
			m_pBtn[eBTN_MIDDLE]->SetEnable(bNeedItemB);
	}
}
