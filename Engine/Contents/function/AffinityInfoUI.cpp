#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "AffinityInfoUI.h"
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIInventory.h>
#include <set>

#define DEF_REWARD_VIEWCOUNT	(5)

class CmdAffinityInfoEnterColor : public Command
{
public:
	CmdAffinityInfoEnterColor()	: m_pWnd(NULL), m_nIndex(-1), m_bEnter(false)	{}
	void setData(CAffinityInfoUI* pWnd, int idx, bool bEnter)	{ m_pWnd = pWnd; m_nIndex = idx; m_bEnter = bEnter;	}
	void execute()
	{
		if (m_pWnd != NULL && m_nIndex >= 0)
		{
			m_pWnd->ChangeEnterColor(m_bEnter, m_nIndex);
		}
	}
private:
	CAffinityInfoUI* m_pWnd;
	int m_nIndex;
	bool m_bEnter;
};

class CmdAffinityInfoSelectColor : public Command
{
public:
	CmdAffinityInfoSelectColor()	: m_pWnd(NULL), m_nIndex(-1)	{}
	void setData(CAffinityInfoUI* pWnd, int idx)	{ m_pWnd = pWnd; m_nIndex = idx; }
	void execute()
	{
		if (m_pWnd != NULL && m_nIndex >= 0)
		{
			m_pWnd->ChangeSelectColor(m_nIndex);
		}
	}
private:
	CAffinityInfoUI* m_pWnd;
	int m_nIndex;
};

class CmdAffinityInfoChangePage : public Command
{
public:
	CmdAffinityInfoChangePage() : m_pWnd(NULL), m_bPrev(false)	{}
	void setData(CAffinityInfoUI* pWnd, bool bPrev)	{m_pWnd = pWnd; m_bPrev = bPrev;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->ChangeRewardPage(m_bPrev);
		}
	}
private:
	CAffinityInfoUI* m_pWnd;
	bool m_bPrev;
};

CAffinityInfoUI::CAffinityInfoUI() 
	: m_ptdSignalTexture(NULL)
	, m_pListNpc(NULL)
	, m_pTabDonationList(NULL)
	, m_pTBAdminNpcName(NULL)
	, m_pTextAffinityPt(NULL)
	, m_pTextNpcName(NULL)
	, m_pTBNpcDesc(NULL)
	, m_pArrayReward(NULL)
	, m_pImgMap(NULL)
	, m_pTextAdminPos(NULL)
	, m_pTextPage(NULL)
	, m_bDrag(false)
	, m_nOldX(0)
	, m_nOldY(0)
	, m_nOldSelectIdx(-1)
	, m_nRewardCurPage(0)
	, m_nRewardNextPoint(0)
	, m_nRewardNpcIndex(0)
	, m_slRewardCheckSum(0)
{
	setInherit(false);

	int i;
	for (i = 0; i < UI_TAB_END; ++i)
	{
		m_pTPDonation[i] = NULL;
		m_pListDonation[i] = NULL;
	}
}

CAffinityInfoUI::~CAffinityInfoUI()
{
	AffinityReset();
	STOCK_RELEASE(m_ptdSignalTexture);
}

void CAffinityInfoUI::initialize()
{
	m_nAffinityIndex = -1;
	m_nAdminNPCIndex = -1;
	m_nPositionNPCIndex = -1;
	m_nPositionZoneNo = -1;

	m_ptdSignalTexture = CreateTexture(CTString("Data\\Interface\\Map.tex"));

	m_pListNpc = (CUIList*)findUI("list_npc");

	CTString strName[UI_TAB_END] = {"item", "quest", "monster"};
	CTString strTmp;

	if (m_pTabDonationList = (CUITab*)findUI("tab_donation_list"))
	{
		for (int i = 0; i < UI_TAB_END; ++i)
		{
			strTmp.PrintF("tp_%s", strName[i]);
			m_pTPDonation[i] = (CUITabPage*)findUI(strTmp.str_String);

			if (m_pTPDonation[i] == NULL)
				continue;

			strTmp.PrintF("list_%s", strName[i]);
			m_pListDonation[i] = (CUIList*)m_pTPDonation[i]->findUI(strTmp);
		}
	}
#ifndef		WORLD_EDITOR
	if (CUIBase* pClose = findUI("btn_close"))
	{
		pClose->SetCommandFUp(boost::bind(&CAffinityInfoUI::CloseAffinityInfo, this));
	}

	if (m_pTBNpcDesc = (CUITextBox*)findUI("tb_npc_desc"))
		m_pTextNpcName = (CUIText*)findUI("text_npc_name");

	if (m_pTBAdminNpcName = (CUITextBox*)findUI("tb_admin_npc_name"))
	{
		m_pTBAdminNpcName->SetCommandFUp(boost::bind(&CAffinityInfoUI::OpenMap, this));
	}
#endif	// WORLD_EDITOR

	m_pTextAffinityPt = (CUIText*)findUI("text_affinity_point");
	m_pArrayReward = (CUIArray*)findUI("array_reward");
	m_pImgMap = (CUIImage*)findUI("img_map");
	m_pTextAdminPos = (CUIText*)findUI("text_admin_pos");
	m_pDrag = findUI("base_drag");

	if (CUIBase* pRewardGroup = findUI("base_reward_group"))
	{
		if (CUIBase* pNext = findUI("btn_next"))
		{
			CmdAffinityInfoChangePage* pCmd = new CmdAffinityInfoChangePage();
			pCmd->setData(this, false);
			pNext->SetCommandUp(pCmd);
		}

		if (CUIBase* pPrev = findUI("btn_prev"))
		{
			CmdAffinityInfoChangePage* pCmd = new CmdAffinityInfoChangePage();
			pCmd->setData(this, true);
			pPrev->SetCommandUp(pCmd);
		}

		m_pTextPage = (CUIText*)findUI("text_reward_page");
	}
}

void CAffinityInfoUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CAffinityInfoUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CAffinityInfoUI::_toggleVisible()
{
	BOOL bVisible = !( IsVisible() );

	if( bVisible )
	{
		OpenAffinityInfo();
	}
	else
	{
		CloseAffinityInfo();
	}
}

void CAffinityInfoUI::OpenAffinityInfo()
{
	if( IsVisible() || GetHide() == FALSE)
	{
		CloseAffinityInfo();
		return;
	}

	_setAffinityData();
	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITYINFO, TRUE );
}

void CAffinityInfoUI::CloseAffinityInfo()
{
	AffinityReset();
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITYINFO, FALSE );
}

void CAffinityInfoUI::AffinityReset()
{	
	m_AffinityList.clear();
	m_nAffinityIndex = -1;
	m_nAdminNPCIndex = -1;
	m_nPositionNPCIndex = -1;
	m_nPositionZoneNo = -1;
	m_nOldSelectIdx = -1;

	m_nRewardNextPoint = 0;
	m_nRewardNpcIndex = 0;
	m_nRewardCurPage = 0;
	m_slRewardCheckSum = 0;

	m_pListNpc->DeleteAllListItem();
}

void CAffinityInfoUI::_setAffinityData()
{
	AffinityReset();

	if (m_pListNpc == NULL)
		return;

	int cnt = _pNetwork->GetAffinityPointer()->count;
	std::set<int> OnAffinityList;

	CNetworkLibrary::_AffinityInfo* pInfo = _pNetwork->GetAffinityPointer();
	CNetworkLibrary::_AffinityInfo::mapAffIter iter;

	CUIIcon* pIcon = NULL;
	CUIBase* pItem = NULL;
	CUIBase* pItemTmp = m_pListNpc->GetListItemTemplate();
	int nCurItem = 0;

	for( iter = pInfo->mapAffinityList.begin(); iter != pInfo->mapAffinityList.end(); ++iter )
	{
 		OnAffinityList.insert( iter->first );
		m_AffinityList.push_back( std::make_pair( iter->first, true) );
	}

	CAffinityData* pTempAffinityData = _pNetwork->GetAffinityData();

	// 맵은 키값으로 소팅이 되기 때문에 값이 뒤에서부터 들어가 있다. 그래서 뒤에서 부터 값을 가져도록 하자 최대한 근접하게 [2/7/2013 Ranma]
	CAffinityData::mapAffReverseIter itermapBegin = pTempAffinityData->mapAffinity_Data.rbegin();
	CAffinityData::mapAffReverseIter itermapEnd = pTempAffinityData->mapAffinity_Data.rend();
	CAffinityData::mapAffReverseIter itermapP = itermapBegin;

	for (; itermapP != itermapEnd; itermapP++)
	{
		int pInfoIndex = itermapP->first;

		if (OnAffinityList.find(pInfoIndex) == OnAffinityList.end())
		{
			// 친화도 개편2 친구로 맺지 못한 세력 셋팅 [2/7/2013 Ranma]
			m_AffinityList.push_back( std::make_pair( pInfoIndex, false ) );
		}
	}

	AffintyList_vec::iterator ListIterBegin = m_AffinityList.begin();
	AffintyList_vec::iterator ListIterEnd = m_AffinityList.end();
	AffintyList_vec::iterator ListIter = ListIterBegin;

	CTString szTextTitle;
	CUIText* pText = NULL;
	COLOR colText = 0xFFFFFFFF;

	for (; ListIter != ListIterEnd; ListIter++)
	{
		m_pListNpc->AddListItem(pItemTmp->Clone());
		pItem = m_pListNpc->GetListItem(nCurItem);

#ifndef		WORLD_EDITOR
		pItem->SetCommandF(boost::bind(&CAffinityInfoUI::_SetAffinityListData, this));
#endif	// WORLD_EDITOR

		if (pIcon = (CUIIcon*)pItem->findUI("icon_npc"))
		{
			pIcon->setAffinity(ListIter->first);
			pIcon->SetCustomTooltip(TRUE);
		}

		if (pText = (CUIText*)pItem->findUI("text_name"))
		{
			szTextTitle.PrintF( "%s", _pNetwork->GetAffinityData()->GetAffinityDataByIndex(ListIter->first)->GetName());
		
			if (ListIter->second)
				colText = 0xFFFFFFFF;
			else
				colText = 0x4C4C4CFF;

			pText->SetText(szTextTitle, "..");
			pText->setFontColor(colText);

			CmdAffinityInfoEnterColor* pCmdE = new CmdAffinityInfoEnterColor();
			pCmdE->setData(this, nCurItem, true);
			pItem->SetCommandEnter(pCmdE);

			CmdAffinityInfoEnterColor* pCmdL = new CmdAffinityInfoEnterColor();
			pCmdL->setData(this, nCurItem, false);
			pItem->SetCommandLeave(pCmdL);

			CmdAffinityInfoSelectColor * pCmdS = new CmdAffinityInfoSelectColor();
			pCmdS->setData(this, nCurItem);
			pItem->SetCommand(pCmdS);

			++nCurItem;
		}
	}

	m_pListNpc->UpdateScroll(m_pListNpc->getListItemCount());
	m_pListNpc->UpdateList();

	ChangeSelectColor(0);
	m_pListNpc->setCurSel(0);
	_SetAffinityListData();
}

void CAffinityInfoUI::_SetAffinityListData()
{
	if (m_pListNpc == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nSel = m_pListNpc->getCurSel();

	if ( (nSel >= 0 && nSel < m_AffinityList.size()))
	{
		int nAffinityIndex = 0;

		if (nSel >= 0 && nSel < m_AffinityList.size())
			nAffinityIndex = m_AffinityList[nSel].first;

		if (m_nAffinityIndex != nAffinityIndex)
		{
			m_pTBNpcDesc->SetText(CTString(""));

			CTString strTmp = "";
			strTmp.PrintF("%d/%d", 0, 0);

			if (m_pTextAffinityPt != NULL)
				m_pTextAffinityPt->SetText(strTmp);

			m_nAffinityIndex = nAffinityIndex;
			m_nAdminNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(m_nAffinityIndex);

			strTmp.PrintF("%s", CMobData::getData( m_nAdminNPCIndex )->GetName());
			if (m_pTextNpcName != NULL)
				m_pTextNpcName->SetText(strTmp);

			strTmp = pUIManager->GetString(_pNetwork->GetAffinityData()->GetAffinityStringIndex(m_nAdminNPCIndex));
			if (m_pTBNpcDesc != NULL)
				m_pTBNpcDesc->SetText(strTmp);
			
			int nCur = 0;
			int nMax = 0;	
			CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
			if (iterPoint != _pNetwork->AffinityInfo.mapAffinityList.end())
			{
				nCur = iterPoint->second.current;
				nMax = iterPoint->second.max;
			}

			CTString strCurPoint = UIMGR()->IntegerToCommaString(nCur);
			CTString strMaxPoint = UIMGR()->IntegerToCommaString(nMax);
			strTmp.PrintF("%s/%s", strCurPoint, strMaxPoint);
			
			if (m_pTextAffinityPt != NULL)
				m_pTextAffinityPt->SetText(strTmp);

			_SetAffinityTab(m_nAffinityIndex);

			// [2013/02/12] sykim70 미니맵 추가
			if (m_nAffinityIndex == 14)	// 흑의 추적자연맹
				m_nPositionNPCIndex = 54;	// 공간술사 리안
			else
				m_nPositionNPCIndex = m_nAdminNPCIndex;

			m_nPositionZoneNo = UTIL_HELP()->GetZoneByNpcIndex(m_nPositionNPCIndex);

			if (m_pTBAdminNpcName != NULL)
			{
				strTmp = CMobData::getData(m_nPositionNPCIndex)->GetName();
				m_pTBAdminNpcName->SetText(strTmp);
			}

			SetMiniMap();
		}
	}
}

void CAffinityInfoUI::_SetAffinityTab(int affinityIdx)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ClearDonationList();

	_pNetwork->SendAffinityInfoTabReq(affinityIdx);

	int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(affinityIdx);
	int i = 0;

	// item info
	CUIInventory* pInven = pUIManager->GetInventory();
	COLOR color = 0xFFFFFFFF;

	std::vector<CAffinityData::_DonationItemInfo>::iterator b, e;
	b = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecDonateList.begin();
	e = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecDonateList.end();

	for (; b != e; ++b)
	{		
		if (pInven->GetItemCount(b->index))
			color = 0x11FF11FF;
		else
			color = 0xFFFFFFFF;

		AddDonationList(UI_TAB_ITEM, eETC_TYPE_AFFINITY_ITEM, b->index, b->point, CTString(_pNetwork->GetItemName(b->index)), color);
	}

	// quest info
	std::vector<CAffinityData::_AffinityQuestInfo>::iterator b1, e1;
	b1 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecQuestList.begin();
	e1 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecQuestList.end();

	for (; b1 != e1; ++b1)
	{
		AddDonationList(UI_TAB_QUEST, eETC_TYPE_AFFINITY_QUEST, affinityIdx, b1->point, CTString(CQuestSystem::Instance().GetQuestName(b1->index)));
	}

	// monster info
	std::vector<CAffinityData::_AffinityMonsterInfo>::iterator b2, e2;
	b2 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.begin();
	e2 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.end();

	for (; b2 != e2; ++b2)
	{
		//AddDonationList(UI_TAB_MONSTER, eETC_TYPE_AFFINITY_MONSTER, b2->index, b2->point, CTString(CMobData::getData(b2->index)->GetName()));
		SetDonationMonsterList(&(*b2));
	}

	for (i = 0; i < UI_TAB_END; ++i)
	{
		if (m_pListDonation[i]->GetScroll() != NULL)
		{
			m_pListDonation[i]->GetScroll()->SetScrollCurPos(0);
			m_pListDonation[i]->UpdateScroll(m_pListDonation[i]->getListItemCount());
		}

		m_pListDonation[i]->UpdateList();
	}
}

void CAffinityInfoUI::RecvAffinity_RewardInfo(CNetworkMessage* istr)
{
	if (m_pArrayReward == NULL)
		return;

	(*istr) >> m_nRewardNextPoint; // 다음 보상 받을 포인트 [2/13/2013 Ranma]
	(*istr) >> m_nRewardNpcIndex;
	(*istr) >> m_slRewardCheckSum;

	m_nRewardCurPage = 0;
	SetRewardList();
}

const	ULONG	CAffinityInfoUI::_getUIType() const
{	
	return UI_NPC_AFFINITYINFO;	
}

void CAffinityInfoUI::AddDonationList( _eAffinityTab_UI tab, eUIBTN_ETC_TYPE btn_type, int idx, int point, CTString& str, COLOR col /*= DEF_UI_COLOR_WHITE*/ )
{
	if (m_pListDonation[tab] == NULL)
		return;

	CUIBase* pBase = m_pListDonation[tab]->GetListItemTemplate();
	if (pBase == NULL)
		return;

	CUIBase* pClone = pBase->Clone();
	m_pListDonation[tab]->AddListItem(pClone);

	CUIIcon* pIcon = (CUIIcon*)pClone->findUI("icon_item");
	CUIText* pText = (CUIText*)pClone->findUI("text_name");

	if (pIcon == NULL || pText == NULL)
		return;

	int nTexIndex = idx;
	if (btn_type == eETC_TYPE_AFFINITY_QUEST)
		nTexIndex = 6088;

	pIcon->setAffinity(btn_type, idx, point, nTexIndex);

	pText->SetText(str, "..");
	pText->setFontColor(col);
}

void CAffinityInfoUI::SetDonationMonsterList( CAffinityData::_AffinityMonsterInfo* pData )
{
	if (m_pListDonation[UI_TAB_MONSTER] == NULL || pData == NULL)
		return;

	CUIBase* pBase = m_pListDonation[UI_TAB_MONSTER]->GetListItemTemplate();
	if (pBase == NULL)
		return;

	CUIBase* pClone = pBase->Clone();
	m_pListDonation[UI_TAB_MONSTER]->AddListItem(pClone);

	CUIIcon* pIcon = (CUIIcon*)pClone->findUI("icon_item");
	CUIText* pText = (CUIText*)pClone->findUI("text_name");

	if (pIcon == NULL || pText == NULL)
		return;

	pIcon->setAffinityM(pData->index, pData->point,
		pData->TextureID, pData->TextureRow, pData->TextureCol);

	CTString strTmp = CMobData::getData(pData->index)->GetName();
	pText->SetText(strTmp, "..");
}

void CAffinityInfoUI::ClearDonationList()
{
	int i;
	for (i = 0; i < UI_TAB_END; ++i)
	{
		if (m_pListDonation[i] == NULL)
			continue;

		m_pListDonation[i]->DeleteAllListItem();
	}
}

void CAffinityInfoUI::SetMiniMap()
{
	if (m_nAffinityIndex == -1)
		return;

	if (m_pImgMap == NULL || m_pTextAdminPos == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	{
		BOOL bWorldMap;
		int nDetail, nX, nZ = 0;

		MapData* md = pUIManager->GetMap()->GetCurMapData(m_nPositionZoneNo);
		for (int i = 0; i < md->vecNpc.size(); i++)
		{
			NpcData& npcData = md->vecNpc[i];
			if (npcData.nIndex != m_nPositionNPCIndex)
				continue;

			if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
			{
				if (npcData.nIndex == 454 ||
					npcData.nIndex == 455)
					continue;
			}

			nDetail = -1;
			for (int j = 0; j < md->vecDetail.size(); j++)
			{
				DetailMapData& dmd = md->vecDetail[j];
				nX = (npcData.fX - dmd.fOffsetX) * dmd.fRatio;
				nZ = (npcData.fZ - dmd.fOffsetZ) * dmd.fRatio;
				if (nX >= dmd.rcMapSize.Left && nX < dmd.rcMapSize.Right &&
					nZ >= dmd.rcMapSize.Top && nZ < dmd.rcMapSize.Bottom)
				{
					bWorldMap = FALSE;
					nDetail = j;
					break;
				}
			}
			if (nDetail == -1)
			{
				bWorldMap = TRUE;
				nX = (npcData.fX - md->World.lOffsetX) * md->World.fRatio;
				nZ = (npcData.fZ - md->World.lOffsetZ) * md->World.fRatio;
			}

			nX -= 113/2;
			nZ -= 113/2;

			// [2013/02/13] sykim70 관리자 NPC 정보 표시
			CTString strTmp;
			strTmp.PrintF("%d, %d", (int)npcData.fX, (int)npcData.fZ);
			m_pTextAdminPos->SetText(strTmp);
			break;
		}

		CTString strFileName;
		if (bWorldMap)
			strFileName.PrintF("Data\\Interface\\Map_World%d0.tex", m_nPositionZoneNo);
		else
			strFileName.PrintF("Data\\Interface\\Map_World%d0_%d.tex", m_nPositionZoneNo, nDetail);

		m_pImgMap->ReleaseTexData();
		m_pImgMap->setTexData(CreateTexture(strFileName));

		UIRectUV uv;
		uv.SetUV(nX, nZ, nX+113, nZ+113);
		m_pImgMap->SetUV(uv);
	}
}

void CAffinityInfoUI::OpenMap()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIMap *pUIMap = pUIManager->GetMap();
	pUIMap->SetCurrentWorldMap(m_nPositionZoneNo, 0);
	pUIManager->m_nHelpNpc_Index = m_nPositionNPCIndex;
	pUIManager->m_IsHelpMob = FALSE;
	pUIManager->m_IsInField = pUIMap->IsNpcInField(m_nPositionZoneNo, m_nPositionNPCIndex);
	pUIMap->ManipulateNpcScroll(TRUE);
}

WMSG_RESULT CAffinityInfoUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	int ndX = x - m_nOldX;
	int ndY = y - m_nOldY;

	if (m_bDrag)
	{
		m_nOldX = x; m_nOldY = y;
		Move(ndX, ndY);
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();
	return WMSG_FAIL;
}

WMSG_RESULT CAffinityInfoUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if ( IsInside(x, y) )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		pUIManager->RearrangeOrder(UI_NPC_AFFINITYINFO, TRUE);

		m_nOldX = x;
		m_nOldY = y;

		if ( m_pDrag != NULL && m_pDrag->IsInside(x, y) == TRUE )
		{
			m_bDrag = true;
			return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT CAffinityInfoUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	m_bDrag = false;
	return WMSG_FAIL;
}

void CAffinityInfoUI::ChangeEnterColor( bool bEnter, int idx )
{
	if (m_nOldSelectIdx == idx)
		return;

	if (idx < 0 || idx >= m_pListNpc->getListItemCount())
		return;

	CUIBase* pItem = m_pListNpc->GetListItem(idx);

	if (pItem == NULL)
		return;

	CUIText* pText = (CUIText*)pItem->findUI("text_name");

	if (pText == NULL)
		return;

	if (bEnter == true)
	{
		pText->setFontColor(0xFEFEBAFF);
	}
	else
	{
		if (m_nOldSelectIdx == idx)
		{
			pText->setFontColor(0xFFEF19FF);
		}
		else
		{
			if (idx >= m_AffinityList.size())
				return;
	
			if (m_AffinityList[idx].second == true)
				pText->setFontColor(0xFFFFFFFF);
			else
				pText->setFontColor(0x4C4C4CFF);
		}
	}
}

void CAffinityInfoUI::ChangeSelectColor( int idx )
{
	if (m_pListNpc == NULL)
		return;

	CUIText* pText = NULL;
	CUIBase* pItem = NULL;

	pItem = m_pListNpc->GetListItem(idx);

	if (pItem == NULL)
		return;

	pText = (CUIText*)pItem->findUI("text_name");

	if (pText == NULL)
		return;

	pText->setFontColor(0xFFEF19FF);

	int nOld = m_nOldSelectIdx;
	m_nOldSelectIdx = idx;

	if (nOld >= 0)
		ChangeEnterColor(false, nOld);
}

void CAffinityInfoUI::SetRewardList( int nPage )
{
	//////////////////// 보상 목록 text [2/13/2013 Ranma]/////////////////////////////////////////////////////
	int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(m_nRewardNpcIndex);
	CTString textReward;
	int i = 0;

	int nRewardIdx = -1;
	int nArrayItemCnt = m_pArrayReward->GetArrayChildCount();
	CUIBase* pItemTmp = m_pArrayReward->GetArrayItemTemplate();
	CUIBase* pItem = NULL;
	CUIIcon* pIcon = NULL;

	// 보유한 세력 포인트 [2/13/2013 Ranma]
	int nCur = 0;
	CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
	if (iterPoint != _pNetwork->AffinityInfo.mapAffinityList.end())
	{
		nCur = iterPoint->second.current;
	}

	std::vector<CAffinityData::_AffinityRewardInfo> vec_data = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList;
	
	int nDataCnt = vec_data.size();
	int nTmpCnt = nDataCnt;

	if (nTmpCnt <= 0)
		return;

	int nStart = nPage * DEF_REWARD_VIEWCOUNT;

	nTmpCnt -= nStart;

	if (nTmpCnt > DEF_REWARD_VIEWCOUNT)
		nTmpCnt = DEF_REWARD_VIEWCOUNT;

	if ((nStart + nTmpCnt) > nDataCnt)
		return;

	CTString strTemp;

	for (i = 0; i < nTmpCnt; ++i)
	{
		if (m_slRewardCheckSum & (1<<0)) // 받을수 없다
		{
			nRewardIdx = 2555;	
		}
		else if (m_slRewardCheckSum & (1<<1)) // 모든 보상을 받았다.
		{
			nRewardIdx = 497;
		}
		else
		{
			if (vec_data[nStart + i].point <= nCur)
			{
				if ( vec_data[nStart + i].point < m_nRewardNextPoint ) // 받았음.
					nRewardIdx = 497;
				else // 받을수 있음.
					nRewardIdx = 6227;
			}
			else // 받을수 없음.
			{
				nRewardIdx = 2555;
			}
		}

		if (i >= nArrayItemCnt)
			m_pArrayReward->AddArrayItem((CUIArrayItem*)pItemTmp->Clone());

		pItem = m_pArrayReward->GetArrayItem(i);
		pIcon = (CUIIcon*)pItem->findUI("icon_reward");

		pItem->Hide(FALSE);

		if (pIcon == NULL)
			continue;

		pIcon->setAffinity(eETC_TYPE_AFFINITY_REWARD, vec_data[nStart + i].index, vec_data[nStart + i].point, nRewardIdx);
	}

	nArrayItemCnt = m_pArrayReward->GetArrayChildCount();

	for (i = nTmpCnt; i < nArrayItemCnt; ++i)
	{
		pItem = m_pArrayReward->GetArrayItem(i);

		if (pItem == NULL)
			continue;

		pItem->Hide(TRUE);
	}

	m_nRewardCurPage = nPage;
	int nMax = nDataCnt / DEF_REWARD_VIEWCOUNT;

	if ((nDataCnt % DEF_REWARD_VIEWCOUNT) != 0)
		++nMax;

	SetPage(m_nRewardCurPage + 1, nMax);
}

void CAffinityInfoUI::ChangeRewardPage( bool bPrev )
{
	int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(m_nRewardNpcIndex);
	std::vector<CAffinityData::_AffinityRewardInfo> vec_data = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList;

	int nPage = m_nRewardCurPage;

	if (bPrev == true)
	{
		--nPage;

		if (nPage < 0)
			return;
	}
	else
	{
		int nItemCnt = vec_data.size();
		if (nItemCnt <= 0)
			return;

		++nPage;

		int nMax = nItemCnt / DEF_REWARD_VIEWCOUNT;

		if ((nItemCnt % DEF_REWARD_VIEWCOUNT) != 0)
			++nMax;

		if (nPage >= nMax)
			return;
	}

	SetRewardList(nPage);
}

void CAffinityInfoUI::SetPage( int nCur, int nMax )
{
	if (m_pTextPage == NULL)
		return;

	CTString strTemp;
	strTemp.PrintF("%d / %d", nCur, nMax);

	m_pTextPage->SetText(strTemp);
}
