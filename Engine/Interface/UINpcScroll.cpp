#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIInventory.h>
#include <engine/Contents/function/SystemMenuUI.h>
#include <Engine/Entities/MobData.h>

extern INDEX g_iXPosInNPCScroll;
extern INDEX g_iYPosInNPCScroll;


class CmdSelectNPC : public Command
{
public:
	CmdSelectNPC() : _npc(true), _idx(-1), _ui(NULL) {}
	void setData(CUINpcScroll* pui, int idx, bool bnpc)	{
		_npc = bnpc;
		_ui = pui; _idx = idx; 
	}
	void execute() {
		if (_idx < 0 || _ui == NULL)
			return;

		_ui->choose(_idx, _npc);
	}

private:
	bool _npc;
	int _idx;
	CUINpcScroll* _ui;

};

CUINpcScroll::CUINpcScroll() 
	: m_nCurRow(0)
	, m_nStringCount(0)
	, m_nCurNpcIndex(-1)
	, m_nCurMobIndex(-1)
	, m_bIsOpen(FALSE)
	, m_nCurZone(0)
	, m_pTbDesc(NULL)
	, m_pList(NULL)
	, m_pTxtJob(NULL)
	, m_pTbDescMove(NULL)
{
	setInherit(false);

	for (int i = 0; i < eSTATUS_MAX; ++i)
		m_pGroup[i] = NULL;
}

CUINpcScroll::~CUINpcScroll()
{
	cArrItemIdex.Clear();

	Destroy();
}

void CUINpcScroll::initialize()
{
#ifndef		WORLD_EDITOR

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

	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUINpcScroll::CloseNpcScroll, this));
	
	m_pGroup[0] = findUI("gro_base");
	m_pGroup[1] = findUI("gro_move");
	m_pGroup[2] = findUI("gro_confirm");

	m_pTbDesc = (CUITextBox*)findUI("tb_desc");
	m_pList = (CUIList*)findUI("list_npc");

	m_pTxtJob = (CUIText*)findUI("txt_name");
	m_pTbDescMove = (CUITextBox*)findUI("tb_desc_move");
	
	pBtn = (CUIButton*)findUI("btn_list");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUINpcScroll::press_list, this));

	pBtn = (CUIButton*)findUI("btn_move");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUINpcScroll::press_move, this));

	pBtn = (CUIButton*)findUI("btn_yes");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUINpcScroll::press_yes, this));

	pBtn = (CUIButton*)findUI("btn_no");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUINpcScroll::press_no, this));


#endif		// WORLD_EDITOR
}

void CUINpcScroll::OpenNPCScroll()
{
	if (IsVisible() != FALSE)
		return;

	m_vectorNpclist.clear();

	int ZoneNo = _pNetwork->MyCharacterInfo.zoneNo;
	INDEX index;

	CNpcHelp::_map::iterator	iter = CNpcHelp::_mapdata.begin();
	CNpcHelp::_map::iterator	eiter = CNpcHelp::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CNpcHelp* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		if (pNpc->zoneNum == ZoneNo)
		{
			index = pNpc->getNpcIndex();
			CMobData* MD = CMobData::getData( index );

			if (MD == NULL)
				continue;

			if( !MD->IsWarCastle() )
			{
				m_vectorNpclist.push_back(index);	 
			}		
		}		
	}

	m_vectorMobList.clear();

	CMobHelp::_map::iterator	iter1 = CMobHelp::_mapdata.begin();
	CMobHelp::_map::iterator	eiter1 = CMobHelp::_mapdata.end();

	for (;iter1 != eiter1; ++iter1)
	{
		CMobHelp* pMob = (*iter1).second;

		if (pMob == NULL)
			continue;

		if (pMob->zoneNum == ZoneNo)
		{
			index = pMob->getNpcIndex();
			CMobData* MD = CMobData::getData( index );

			if (MD == NULL)
				continue;

			m_vectorMobList.push_back(MD);
		}		
	}

	SetViewList();
	RefreshNpcList();

	m_bIsOpen = TRUE;
	SetPos( g_iXPosInNPCScroll, g_iYPosInNPCScroll );
	UIMGR()->RearrangeOrder(UI_NPC_SCROLL, TRUE);
	Hide(FALSE);

	// 상태를 초기화 한다.
	set_status(eSTATUS_LIST);
}

void CUINpcScroll::CloseNpcScroll()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsOpen = FALSE;
	// save client pos. [9/21/2009 rumist]
	g_iXPosInNPCScroll = GetPosX();
	g_iYPosInNPCScroll = GetPosY();
	pUIManager->m_nHelpNpc_Index = -1;
	pUIManager->RearrangeOrder(UI_NPC_SCROLL, FALSE);
	//vector 지우는거 추가 할 것
	m_vectorNpclist.clear();
	m_vectorMobList.clear();

	Hide(TRUE);
}

BOOL StringInAscendingOrder(const INDEX npc1, const INDEX npc2)
{
	int nReturn = strcmp(CMobData::getData(npc1)->GetName(), CMobData::getData(npc2)->GetName());
	
	if ( nReturn > 0) 
	{
		return FALSE;
	}
		
	return TRUE;
}	

BOOL MobLevInAscendingOrder(const CMobData* mob1, const CMobData* mob2)
{
	//int nResult = mob1.GetLevel() - mob2.GetLevel();

	if(mob1->GetLevel() >= mob2->GetLevel())
	{
		return FALSE;
	}

	return TRUE;
}

void CUINpcScroll::SetViewList()
{
	m_nCurZone = _pNetwork->MyCharacterInfo.zoneNo;

	CTString	tempDesc;
	tempDesc.PrintF(_S(5031,"%s 지역의 NPC들 앞으로 이동할 수 있는 포탈을 생성합니다. \
							리스트에서 이름을 클릭하면 위치 안내 또는 포탈을 사용 하실 수 있습니다."),
					CZoneInfo::getSingleton()->GetZoneName(m_nCurZone));

	if (m_pTbDesc != NULL)
		m_pTbDesc->SetText(tempDesc);	

	if (!m_vectorNpclist.empty())
	{
		std::sort(m_vectorNpclist.begin(), m_vectorNpclist.end(), StringInAscendingOrder);
	}

	if (!m_vectorMobList.empty())
	{
		std::sort(m_vectorMobList.begin(), m_vectorMobList.end(), MobLevInAscendingOrder); 
	}
}
void CUINpcScroll::RefreshNpcList()
{
	if (m_pList == NULL)
		return;

	m_pList->DeleteAllListItem();

	CTString strTemp;
	strTemp.PrintF("[%s]", _S(446, "NPC"));

	add_list(strTemp, 0xE36C0AFF);

	std::vector<int>::iterator it = m_vectorNpclist.begin();
	std::vector<int>::iterator itend = m_vectorNpclist.end();

	for( ; it != itend; ++it )
	{
		strTemp = CMobData::getData((*it))->GetName();
		if(CQuestSystem::Instance().TestNPCForQuest((*it)) == CQuestSystem::NQT_HAVE_QUEST)
		{
			strTemp += (CTString)(" [Q]");
		}
		else if(CQuestSystem::Instance().TestNPCForQuest((*it)) == CQuestSystem::NQT_CAN_PRIZE)
		{
			strTemp += (CTString)(" [A]");
		}
			
		add_list(strTemp, DEF_UI_FONT_COLOR, (*it), true);
	}

	strTemp.PrintF("[%s]", _S(448, "몬스터"));

	add_list(_s(""));
	add_list(strTemp, 0xC20000FF);

	vec_mob_iter it1 = m_vectorMobList.begin();
	vec_mob_iter iend1 = m_vectorMobList.end();

	int nLevelDiff	= 0;
	int nColIndex	= 0;

	
	for( ; it1 != iend1; ++it1)
	{
		int idx = (*it1)->GetMobIndex();

		if (idx <= 0)
			continue;

		CMobData* pData = CMobData::getData(idx);

		if (pData == NULL)
			continue;

		nLevelDiff = pData->GetLevel() - _pNetwork->MyCharacterInfo.level;

		if(nLevelDiff > 5)
			nColIndex = 0;
		else if (nLevelDiff > 2)
			nColIndex = 1;
		else if (nLevelDiff > -3)
			nColIndex = 2;
		else 
			nColIndex = 4;
			
		strTemp.PrintF("%s (Lv%d)", pData->GetName(), pData->GetLevel());		
 		add_list(strTemp, GetNameColor(nColIndex), pData->getindex(), false);
	}

	m_pList->UpdateList(true);
}

COLOR CUINpcScroll::GetNameColor(int nIndex)
{
	COLOR mobColor;

	switch(nIndex)
	{
		case 0:
			mobColor = 0xCE4900FF;
		break;

		case 1:
			mobColor = 0xD7AA00FF;
		break;

		case 2:
			mobColor = 0xC5C5C5FF;
		break;

		case 3:
			mobColor = 0x5B9900FF;
		break;

		case 4:
			mobColor = 0x008BAAFF;
		break;
	}

	return mobColor;
	
}

void CUINpcScroll::ReceiveNPCData(CNetworkMessage* istr)
{
	
	SLONG slMobCount, slMobIndex;
	SLONG slNpcCount, slNpcIndex;

	(*istr) >> slNpcCount;

	m_vectorNpclist.clear();

	int		i;
	for( i = 0; i < slNpcCount; i++)
	{
		(*istr) >> slNpcIndex;
	}

	int n = m_vectorNpclist.size();
	
	(*istr) >> slMobCount;

	m_vectorMobList.clear();

	for( i = 0; i < slMobCount; i++)
	{
		(*istr) >> slMobIndex;

		CMobData* tempMob = CMobData::getData(slMobIndex);

		if (tempMob == NULL)
			continue;

		m_vectorMobList.push_back(tempMob);
	}

	if (m_eStatus == eSTATUS_LIST)
	{	
		SetViewList();
		RefreshNpcList();
		OpenNPCScroll();
	}

}

void CUINpcScroll::ReceiveMobLocation(CNetworkMessage* istr)
{
	(*istr) >> fX;
	(*istr) >> fZ;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->m_fHelpMobX = fX;
	pUIManager->m_fHelpMobZ = fZ;
}

void CUINpcScroll::OpenNPCScrollInfo(SLONG slIndex)
{
	CTString strMessage;
	CUIMsgBox_Info	ErrorMsgBoxInfo;

	switch(slIndex)
	{
		case MSG_NPC_PORTAL_ERROR_MOB:
			{
				strMessage.PrintF(_S(4741,"현재 필드에 해당 몬스터가 존재하지 않습니다."));
			}
			break;

		case MSG_NPC_PORTAL_ERROR_LIST:
			{
				strMessage.PrintF(_S(4742,"이 아이템은 현재 지역에서 사용 할 수 없습니다."));
			}
			break;
		default:	// 전혀 모르는 에러 정보 이다. [eons]
			{
				CPrintF("OpenNPCScrollInfo() : default message\n");
				return;
			}
	}

	// error 정보는 그때 그때 마다 정확히 뿌려 주어야 한다.  return; -> ClosemessageBox [eons]
	// 메세지 타입을 잘못 사용하였다. 알림 메세지 박스는 MSGLCMD가 아니라 MSGCMD이다.
	//단순히 확인 창이므로 MSGLCMD_NPC_SCROLL_MESSAGE를 MSGCMD_NULL으로 처리한다. UI_NPC_SCROLL또한 UI_NONE으로
	ErrorMsgBoxInfo.SetMsgBoxInfo(_S(1748, "안내"), UMBS_OK, UI_NONE, MSGCMD_NULL);
	ErrorMsgBoxInfo.AddString(strMessage, 0xA3A1A3FF);
	CUIManager::getSingleton()->CreateMessageBox(ErrorMsgBoxInfo);
}

BOOL CUINpcScroll::IsNPC(int nIndex)
{
	if(0 < nIndex && nIndex <= m_vectorNpclist.size())			
		return TRUE;

	return FALSE;
}

// check & toggling function bodys. [9/21/2009 rumist]
BOOL CUINpcScroll::IsOpened() const 
{
	return m_bIsOpen;
}

void CUINpcScroll::ToggleNPCScroll()
{
	if( IsOpened() )
		CloseNpcScroll();
	else
		OpenNPCScroll();
}

void CUINpcScroll::UseNPCScroll()
{
	int nTab = 0;
	int nIdx = 0;
	int	i;
//	int size = sizeof( cArrItemIdex ) / sizeof(int);
// 	if( size < _pNetwork->MyCharacterInfo.zoneNo )
// 		return;

	if(cArrItemIdex.Count() == 0)
	{
		// NPC 스크롤 아이템 인덱스 정보 세팅
		cArrItemIdex.New( CZoneInfo::getSingleton()->GetZoneCount() );

		for( i = 0; i < cArrItemIdex.Count(); ++i )
		{
			cArrItemIdex[i] = 0;
		}

		CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
		CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

		for (; iter != eiter; ++iter)
		{	
			CItemData* pItemData = (*iter).second;

			if (pItemData == NULL)
				continue;

			if( pItemData->GetType() == CItemData::ITEM_POTION && pItemData->GetSubType() == CItemData::POTION_POTAL_SCROLL )
			{
				cArrItemIdex[pItemData->GetNum0()] = pItemData->GetItemIndex();
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetQuickSlot()->IsInQuickSolt(UBET_ITEM, cArrItemIdex[_pNetwork->MyCharacterInfo.zoneNo]))
	{ // 퀵슬롯에 등록되어 있다면 사용한다. [eons]
		pUIManager->GetInventory()->GetItemSlotInfo( cArrItemIdex[_pNetwork->MyCharacterInfo.zoneNo], nTab, nIdx );
		pUIManager->GetInventory()->SendUseSlotItem( nTab, nIdx );
	}

	CPrintF("Use Item!!!\n");
}

BOOL CUINpcScroll::IsInPeaceArea()
{
	if (_pNetwork->MyCharacterInfo.sbAttributePos & MATT_PEACE)
		return TRUE;

	return CUIManager::getSingleton()->GetMap()->IsPlayerInPeaceArea();
}

BOOL CUINpcScroll::IsOpenDetailedMap(int nZone, int nNpcIndex)
{
	BOOL bIsNpcInField = CUIManager::getSingleton()->GetMap()->IsNpcInField(m_nCurZone, m_nCurNpcIndex);

	if( IsInPeaceArea()==TRUE && bIsNpcInField==FALSE )
		return TRUE;
	
	return FALSE;
}

void CUINpcScroll::choose( int idx, bool bnpc )
{
	CUIManager* pUIManager = UIMGR();
	
	if (bnpc == true)
	{
		m_nCurNpcIndex = idx;
		pUIManager->m_nHelpNpc_Index = idx;

		if (IsOpenDetailedMap(m_nCurZone, m_nCurNpcIndex))
		{
			pUIManager->m_IsInField = FALSE;
		}
		else
		{
			pUIManager->m_IsInField = TRUE;
		}

		set_status(eSTATUS_DESC);

		CMobData* pMob = CMobData::getData(idx);

		if (pMob != NULL)
		{
			if (m_pTxtJob != NULL)
				m_pTxtJob->SetText((CTString)pMob->GetName());

			if (m_pTbDescMove != NULL)
				m_pTbDescMove->SetText((CTString)pMob->GetDesc());
		}
	}
	else
	{
		m_nCurMobIndex = idx;
		_pNetwork->SendNPCPortalLocationReq(m_nCurMobIndex);
		pUIManager->m_IsHelpMob = TRUE;

		pUIManager->m_NPCScrollZoneNo = m_nCurZone;
		pUIManager->m_nHelpNpc_Index = -1;
		pUIManager->m_IsInField = TRUE;
	}

	pUIManager->GetMap()->ManipulateNpcScroll(TRUE);

	pUIManager->RearrangeOrder(UI_NPC_SCROLL, TRUE);
}

//----------------------------------------------------------------------------

bool CUINpcScroll::add_list( CTString& str, COLOR col, int idx, bool bnpc )
{
	if (m_pList == NULL)
		return false;

	CUIListItem* ptemp = m_pList->GetListItemTemplate();

	if (ptemp == NULL)
		return false;

	CUIListItem* pClone = (CUIListItem*)ptemp->Clone();
	CUIText* pTxt;

	pTxt = (CUIText*)pClone->findUI("txt_npc");

	if (pTxt != NULL)
	{
		pTxt->SetText(str);
		
		if (idx < 0)
			pTxt->setFontColor(col);
		else
			pTxt->setFontColor(col, eSTATE_IDLE);
			//pTxt->setFontColor(0xF0A769FF, eSTATE_ENTER);

		CmdSelectNPC* pcmd = new CmdSelectNPC;
		pcmd->setData(this, idx, bnpc);
		pTxt->SetCommand(pcmd);

		m_pList->AddListItem(pClone);

		return true;
	}

	return false;
}

void CUINpcScroll::set_status( eSTATUS status )
{
	int		i;
	BOOL	bHide;

	for (i = 0; i < eSTATUS_MAX; ++i)
	{
		if (m_pGroup[i] == NULL)
			continue;

		bHide = (i == status) ? FALSE : TRUE;

		m_pGroup[i]->Hide(bHide);
	}
}

void CUINpcScroll::press_list()
{
	set_status(eSTATUS_LIST);
	UIMGR()->GetMap()->ManipulateNpcScroll(FALSE);
}

void CUINpcScroll::press_move()
{
	set_status(eSTATUS_YESNO);
}

void CUINpcScroll::press_yes()
{
	// Move
	CUIManager* pUIManager = UIMGR();

	CloseNpcScroll();
	//로그아웃 도중 포탈 이동을 실행할 경우 로그아웃 취소
	pUIManager->GetSystemMenu()->CancelRestart();

	_pNetwork->SendNPCPortalGoReq(m_nCurNpcIndex);
	pUIManager->GetMap()->ManipulateNpcScroll(FALSE);
}

void CUINpcScroll::press_no()
{
	set_status(eSTATUS_DESC);
	UIMGR()->GetMap()->ManipulateNpcScroll(FALSE);
}


