#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "Product2UI.h"
#include <Engine/Interface/UIProduct.h>
#include <Engine/Contents/function/ProductNPCUI.h>
#include <Engine/Contents/function/ProductNPCCmd.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

// ----------------------------------------------------------------------------
// Name : CUIProduct2()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProduct2::CUIProduct2()
	: m_pTxtTitle(NULL)
	, m_pTxtSealExp(NULL)
	, m_pTreePos(NULL)
	, m_pTree(NULL)
	, m_pTreeTemp(NULL)
	, m_pTreeItemTemp(NULL)
	, m_pScroll(NULL)
	, m_pList(NULL)
	, m_pIconMake(NULL)
	, m_pTxtMakeName(NULL)
	, m_nSelFactoryIndex(-1)
	, m_nsel_sub(-1)
	, m_nsel_idx(-1)
{
	setInherit(false);
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct2()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct2::~CUIProduct2()
{
	Destroy();
}

void CUIProduct2::OpenProduct2(INDEX sealtype)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetProductNPC()->IsVisible())
	{
		pUIManager->GetProductNPC()->CloseProductNPC();
	}

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_nSealType = sealtype;
	
	if (m_pIconMake != NULL)
		m_pIconMake->clearIconData();

	set_data();

	// If this is already exist
	if( IsVisible() ) return;

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_PRODUCT2, TRUE );
	pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_PRODUCT_ITEM);

}

void CUIProduct2::CloseProduct2()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_PRODUCT2, FALSE );
	pUIManager->GetInventory()->Lock(FALSE, FALSE, LOCK_PRODUCT_ITEM);
	Hide(TRUE);
}

void CUIProduct2::ListReset()
{
	//m_vctList.clear();
	
	if (m_pIconMake != NULL)
		m_pIconMake->clearIconData();

	if (m_pTxtMakeName != NULL)
		m_pTxtMakeName->SetText(CTString(""));

	m_nSelFactoryIndex = -1;
	m_nsel_sub = -1;
	m_nsel_idx = -1;

	set_list_item();
}

void CUIProduct2::SetItemMaking(BOOL bMaking)
{
	m_bItemMaking = bMaking;
	SendMakeItem(MSG_FACTORY_ITEM_MAKE, m_nSelFactoryIndex);
}

void CUIProduct2::SendMakeItem(UBYTE ubType, ULONG lindex)
{
	CNetworkMessage nmMessage(MSG_FACTORY);

	nmMessage << ubType;
	nmMessage << lindex;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIProduct2::initialize()
{
#ifndef		WORLD_EDITOR

	CUIText* pTxt = (CUIText*)findUI("title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pTxtTitle = (CUIText*)findUI("txt_title");
	m_pTxtSealExp = (CUIText*)findUI("txt_exp");
	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
		m_pBtnClose->SetCommandFUp(boost::bind(&CUIProduct2::CloseProduct2, this));

	CUIButton* pBtn = (CUIButton*)findUI("btn_make");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProduct2::callback_make, this));

	m_pTreePos = findUI("pos");
	m_pTreeTemp = (CUITree*)findUI("tree");
	m_pScroll = (CUIScrollBar*)findUI("scroll");
	m_pTreeItemTemp = (CUITreeItem*)findUI("ti_stuff");

	m_pList = (CUIList*)findUI("list_material");
/*
	// 재료는 5개 이므로 미리 생성한다.

	if (m_pList != NULL)
	{
		CUIBase* ptmpl = m_pList->GetListItemTemplate();

		if (ptmpl != NULL)
		{
			for (int i = 0; i < MAX_STUFF_ITEM; ++i)
				m_pList->AddListItem(ptmpl->Clone());

			m_pList->UpdateList();
		}
	}
*/

	m_pIconMake = (CUIIcon*)findUI("icon_selected");
	m_pTxtMakeName = (CUIText*)findUI("txt_selected");

#endif		// WORLD_EDITOR
}

void CUIProduct2::callback_select( int nsub, int idx, int fac_idx, COLOR col )
{
	CUITree* ptree;

	// 같은 것을 선택하면 리턴
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	// 이전 선택을 되돌림.
	if (m_nsel_sub >= 0 && m_nsel_idx >= 0)
	{
		ptree = getTree(m_nsel_sub, m_nsel_idx);

		if (ptree != NULL)
		{
			set_color(ptree, m_colPrev);
			CUIBase* pimg = ptree->findUI("img_select");

			if (pimg != NULL)
				pimg->Hide(TRUE);
		}
	}

	ptree = getTree(nsub, idx);

	if (ptree == NULL)
		return;

	// select image
	{
		CUIBase* pimg = ptree->findUI("img_select");

		if (pimg != NULL)
			pimg->Hide(FALSE);
	}

	m_nsel_sub = nsub;
	m_nsel_idx = idx;
	m_nSelFactoryIndex = fac_idx;
	m_colPrev = col;

	CMakeItemData& Itemdata = _pNetwork->GetMakeItemData(m_nSelFactoryIndex);

	if (Itemdata.GetFactoryIndex() > 0)
	{
		// Make Item 및 재료 세팅
		if (m_pIconMake != NULL)
			m_pIconMake->setData(UBET_ITEM, Itemdata.GetItemButtonIndex());

		if (m_pTxtMakeName != NULL)
		{
			m_pTxtMakeName->SetText(CTString(_pNetwork->GetItemName(Itemdata.GetItemButtonIndex())));
		}

		set_list_item();
	}
}

void CUIProduct2::callback_over( int nsub, int idx, bool bover, COLOR col )
{
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	CUITree* ptree = getTree(nsub, idx);

	if (bover == true)
	{
		set_color(ptree, DEF_UI_COLOR_OVER);
	}
	else
	{
		set_color(ptree, col);
	}
}

// -----------------------------------------------------------------
// PRIVATE

bool CUIProduct2::set_data()
{
	if (m_pTreeTemp == NULL || m_pTreePos == NULL ||
		m_pTreeItemTemp == NULL)
		return false;

	if (m_nSealType < 0)
		return false;

	int nSealtypenum = m_nSealType - SEALTYPE_START_INDEX;
	//nSealSubCount는 List 저장을 위해 subtype 갯수를 저장함
	int nSealSubCount = SealTypeCount[nSealtypenum+1] - SealTypeCount[nSealtypenum];	

	MyInfo* pMyinfo = MY_INFO();

	// Title 세팅
	const int title_string_idx[] = { 4615, 4616, 4617, 4618, 4619 };

	if (m_pTxtTitle != NULL)
		m_pTxtTitle->SetText(_S(title_string_idx[nSealtypenum], ""));

	if (m_pTxtSealExp != NULL)
	{
		CTString str;
		str.PrintF("%I64d", pMyinfo->GetSkillExp(m_nSealType));
		UIMGR()->InsertCommaToString(str);
		m_pTxtSealExp->SetText(str);
	}

	std::vector<CMakeItemData*>	vecList;

	int		i;
	//Seal type에 따른 Sub타입에 맞게 저장 시킴
	for( i = 0; i < _pNetwork->wo_iNomOfMakeItemData; ++i )
	{
		CMakeItemData* temMakeItem =  &_pNetwork->wo_aMakeItemData[i];

		if (temMakeItem != NULL && temMakeItem->GetMakeItemType() == m_nSealType)
		{
			{
				// 배우지 않은 스킬 리스트 저장
				if (pMyinfo->find_makeitem(temMakeItem->GetFactoryIndex()) == true)
				{
					vecList.push_back(temMakeItem);
				}

			}

		}
	}

	std::sort(vecList.begin(), vecList.end(), comp);

	// 기존 트리에 데이터가 있다면 삭제한다.
	if (m_pTree != NULL)
	{
		m_pTreePos->deleteChild(m_pTree);
		m_pTree = NULL;
	}

	// tree root 처리
	{
		m_pTree = (CUITree*)m_pTreeTemp->Clone();
		m_pTree->setRoot();
		m_pTreePos->addChild(m_pTree);

		m_pScroll->Hide(FALSE);
		m_pTree->setScroll(m_pScroll);
	}

	CUITree*		ptree_clone = NULL;
	CUITree*		ptree_child = NULL;
	CUIBase*		pclone = NULL;
	CUIText*		pText = NULL;

	int temStartpos = SealTypeCount[nSealtypenum];
	CTString strTemp;
	UQUAD nSealExp = MY_INFO()->GetSkillExp(m_nSealType);

	std::vector<CMakeItemData*>::iterator endIter = vecList.end();
	std::vector<CMakeItemData*>::iterator startIter;

	int		idx;
	// List에 SubType대로 저장함
	for( i = 0; i < nSealSubCount; i++)
	{
		strTemp = _S(sub_type_string_idx[temStartpos + i], "");

		// 서브 메뉴 추가
		{
			ptree_clone = (CUITree*)m_pTreeTemp->Clone();

			pclone = m_pTreeItemTemp->Clone();
			pclone->Hide(FALSE);
			pText = (CUIText*)pclone->findUI("txt_");
			pText->SetText(strTemp);

			ptree_clone->addChild(pclone);		// Child 를 붙인 후 			
		}

		startIter = vecList.begin();

		for(; startIter != endIter; ++startIter )
		{
			CMakeItemData* tempdata = (*startIter);
			COLOR colTemp = 0xC0C0C0FF;

			if (SealSubType[temStartpos+i] == tempdata->GetMakeItemSubType())
			{
				strTemp = tempdata->GetName();
				SQUAD temResult = nSealExp - tempdata->GetNeedExp();

				if (temResult < 10)
				{
					colTemp = 0xFF8000FF;
				}else if (temResult < 20)
				{
					colTemp = 0xFFFF00FF;
				}else if (temResult < 30)
				{
					colTemp = 0x008000FF;
				}

				ptree_child = (CUITree*)m_pTreeTemp->Clone();
				ptree_child->SetHeight(20);			

				pclone = m_pTreeItemTemp->Clone();
				pclone->Hide(FALSE);
				pText = (CUIText*)pclone->findUI("txt_");
				pText->SetText(strTemp);
				pText->setFontColor(colTemp);

				ptree_child->addChild(pclone);

				ptree_clone->addTree(ptree_child, false);
				idx = ptree_clone->getChildTreeCount() - 1;

				// cmd 등록
				CmdProductNPCSelect<CUIProduct2>* pcmd = new CmdProductNPCSelect<CUIProduct2>;
				pcmd->setData(this, i, idx, tempdata->GetFactoryIndex(), colTemp);
				ptree_child->SetCommandUp(pcmd);

				CmdProductNPCOver<CUIProduct2>* pcmd_over = new CmdProductNPCOver<CUIProduct2>;
				pcmd_over->setData(this, i, idx, true, DEF_UI_COLOR_OVER);
				ptree_child->SetCommandEnter(pcmd_over);

				CmdProductNPCOver<CUIProduct2>* pcmd_leave = new CmdProductNPCOver<CUIProduct2>;
				pcmd_leave->setData(this, i, idx, false, colTemp);
				ptree_child->SetCommandLeave(pcmd_leave);
			}
		}

		m_pTree->addTree(ptree_clone);		// Tree 에 등록해야 높이가 잡힌다.
		ptree_clone->setUncollapse(TRUE);
		ptree_clone->updateTree();
	}

	vecList.clear();

	m_pTree->SetScrollPos(0);
	m_pTree->updateTree();

	return true;
}

void CUIProduct2::callback_make()
{
	if (m_nSelFactoryIndex < 0)
		return;

	SendMakeItem(MSG_FACTORY_ITEM_MAKE_REQ, m_nSelFactoryIndex);
}

CUITree* CUIProduct2::getTree( int nsub, int idx )
{
	CUITree* ret = NULL;

	if (m_pTree == NULL)
		return ret;

	CUITree* psub = m_pTree->getChildTree(nsub);

	if (psub != NULL)
	{
		ret = psub->getChildTree(idx);
	}

	return ret;
}

void CUIProduct2::set_color( CUITree* pTree, COLOR col )
{
	if (pTree == NULL)
		return;

	CUIText* ptxt = (CUIText*)pTree->findUI("txt_");

	if (ptxt == NULL)
		return;

	ptxt->setFontColor(col);
}

void CUIProduct2::set_list_item()
{
	int		i, stuff_idx;
	LONGLONG llstuff_count;
	CUIBase* pChild;
	CUIIcon* pIcon;
	CUIText* pTxt;
	CTString tmpStr;

	CMakeItemData* pItemdata = NULL;
	
	if (m_nSelFactoryIndex > 0)
		pItemdata = &_pNetwork->GetMakeItemData(m_nSelFactoryIndex);

	for (i = 0; i < MAX_STUFF_ITEM; ++i)
	{		
		pChild = m_pList->GetListItem(i);

		if (pChild == NULL)
			return;

		pIcon = (CUIIcon*)pChild->findUI("icon");

		if (pIcon != NULL)
		{
			if (m_nSelFactoryIndex > 0)
			{
				stuff_idx = pItemdata->GetStuffItemIndex(i);
				llstuff_count = pItemdata->GetStuffItemCount(i);
				pIcon->setData(UBET_ITEM, pItemdata->GetStuffItemIndex(i));
				pIcon->setCount(llstuff_count);
			}
			else
			{
				pIcon->clearIconData();
			}
		}

		pTxt = (CUIText*)pChild->findUI("name");

		if (pTxt != NULL)
		{
			if (m_nSelFactoryIndex > 0)
				tmpStr = _pNetwork->GetItemName(stuff_idx);
			else
				tmpStr = "";

			pTxt->SetText(tmpStr);
		}

		pTxt = (CUIText*)pChild->findUI("count");

		if (pTxt != NULL)
		{
			if (m_nSelFactoryIndex > 0)
			{
				CTString strCount;
				strCount.PrintF("%I64d", llstuff_count);
				UIMGR()->InsertCommaToString(strCount);
				tmpStr.PrintF(_S(4809, "%s 개"), strCount);
			}
			else
				tmpStr = "";

			
			pTxt->SetText(tmpStr);
		}
	}
}
