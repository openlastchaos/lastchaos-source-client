#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "AffinityUI.h"
#include "AffinityCmd.h"

#include <Engine/Interface/UIShop.h>
#include <vector>
#include <Common/Packet/ptype_old_do_affinity.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

#define AFFINITY_POINT_LIMIT (50000)

enum eAffinitySelection
{
	AFFINITY_CONTRACT,
	AFFINITY_DONATE,
	AFFINITY_SHOP,
	AFFINITY_PRESENT,
	AFFINITY_TALK,
	AFFINITY_EVENT,
};

CAffinityUI::CAffinityUI()
	: m_pListAffinity(NULL)
	, m_pWant2DonationPoint(NULL)
	, m_pTextDonationPoint(NULL)
	, m_pDragArea(NULL)
	, m_slNPCIdx(-1)
	, m_slNPCVIdx(-1)
	, m_nAffinityPoint(0)
	, m_nAffinityIndex(0)
	, m_bDrag(false)
	, m_nOldX(0)
	, m_nOldY(0)
{
	setInherit(false);
}

CAffinityUI::~CAffinityUI()
{
	m_pWant2DonationPoint = NULL;
	m_pTextDonationPoint = NULL;
	m_pListAffinity = NULL;
	m_pDragArea = NULL;

	DonationReset();
	Destroy();
}

void CAffinityUI::initialize()
{
	CUIBase* pBtn = NULL;

	if (pBtn = findUI("btn_close"))
	{
		CmdAffinityClose* pCmd = new CmdAffinityClose();
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	if (pBtn = findUI("btn_cancel"))
	{
		CmdAffinityClose* pCmd = new CmdAffinityClose();
		pCmd->setData(this);
		pBtn->SetCommand(pCmd);
	}

	if (pBtn = findUI("btn_ok"))
	{
		CmdAffinityConfirm* pCmd = new CmdAffinityConfirm();
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	m_pWant2DonationPoint = (CUIText*)findUI("text_want2donation_point");
	m_pTextDonationPoint = (CUIText*)findUI("text_donation_point");
	m_pListAffinity = (CUIList*)findUI("list_affinity");
	m_pDragArea = findUI("base_drag");
}

void CAffinityUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CAffinityUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CAffinityUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_AFFINITY_CONTRACT:
		{
			if( bOK )
			{
				CPrintF("Mob Index %d\n", INFO()->GetTargetEntity(eTARGET)->GetNetworkID() );
				_pNetwork->SendAffinityConnectReq( m_slNPCVIdx );
			}
		}
		break;
	case MSGCMD_AFFINITY_TAKEPRESENT:
		{
			if( bOK )
			{
				_pNetwork->SendAffinityGiftReq( m_slNPCVIdx );
			}
		}
		break;
	}
}

void CAffinityUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_AFFINITY_DONATE_REQ:
		{
			if( nResult == AFFINITY_CONTRACT )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(4464, "친구 맺기"), UMBS_OKCANCEL, UI_NPC_AFFINITY, MSGCMD_AFFINITY_CONTRACT );

				CTString strTemp;

				CAffinityData* pData = _pNetwork->GetAffinityData();

				strTemp.PrintF( _S( 5297, "%s 세력과 친구 맺기를 하시겠습니까?"), 
					_pNetwork->GetAffinityData()->GetAffinityName( m_slNPCIdx ) );	

				MsgBoxInfo.AddString( strTemp );

				int needLevel = pData->GetAffinityData( m_slNPCIdx )->needLevel;

				if( needLevel > 0 )
				{
					strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), needLevel );
					MsgBoxInfo.AddString( strTemp );
				}

				int needItemCount = pData->GetAffinityData( m_slNPCIdx )->needItemCount;

				if( needItemCount > 0 )	// 필요 아이템이 있다
				{
					CTString strItemName;
					INDEX needItemIndex = pData->GetAffinityData( m_slNPCIdx )->needItemIndex;

					CTString strNas;
					strNas.PrintF("%d", needItemCount);
					pUIManager->InsertCommaToString(strNas);

					if( needItemIndex == 19 )	// 나스
						strItemName.PrintF(_S( 836,"%s 나스"), strNas );
					else
					{
						CTString strCount = pUIManager->IntegerToCommaString(needItemCount);
						strItemName.PrintF( _S( 61, "%s %s개" ), _pNetwork->GetItemName(needItemIndex), strCount );
					}

					strTemp.PrintF( _s("%s : %s"), _S( 351, "필요 아이템" ), strItemName );
					MsgBoxInfo.AddString( strTemp );
				}

				int needAffinityPoint = pData->GetAffinityData( m_slNPCIdx )->needAffinityPoint;

				if( needAffinityPoint > 0 )
				{
					int needAffinityIndex = pData->GetAffinityData( m_slNPCIdx )->needAffinityIndex;

					CTString strPoint;
					strPoint.PrintF("%d", needAffinityPoint);
					pUIManager->InsertCommaToString(strPoint);

					strTemp.PrintF( _S( 5316, "필요 친화도 : %s 세력 %s" ), 
						_pNetwork->GetAffinityData()->GetAffinityNameByIndex( needAffinityIndex ),
						strPoint);

					MsgBoxInfo.AddString( strTemp );
				}
				CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
			}
			else if( nResult == AFFINITY_DONATE )
			{
				// [2013/02/05] sykim70 친화도 기부 UI 개선
				CNetworkLibrary::_AffinityInfo* pInfo = _pNetwork->GetAffinityPointer();
				if (pInfo->count == 0 || (pInfo->mapAffinityList.find(m_nAffinityIndex) == pInfo->mapAffinityList.end()))
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(4678, "아직 친구를 맺지 않았습니다. 먼저 친구를 맺어주세요."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
					return;
				}

				DonationReset();

				int i = 0, nItemCount = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.size();
				
				UpdateListItem();
				OpenDonation();
				UpdateDonationPoint();
			}
			else if( nResult == AFFINITY_SHOP )
			{
				// shop open code.
				_pNetwork->SendAffinityShopReq( m_slNPCVIdx );
			}
			else if( nResult == AFFINITY_PRESENT )
			{
				//take a present from friendly force.
				_pNetwork->SendAffinityGiftInfoReq( m_slNPCVIdx );

			}
			else if( nResult == AFFINITY_TALK )
			{
				CUIQuestBook::TalkWithNPC();
			}
			else if( nResult == AFFINITY_EVENT )
			{
			}
			else
			{
			}
		}
	}
}

void CAffinityUI::OpenAffinity( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is alreay exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_AFFINITY_DONATE_REQ )|| IsVisible() )
		return;

	pUIManager->CloseMessageBox(MSGCMD_AFFINITY_CONTRACT);

	// Set position of target npc.
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_slNPCVIdx = INFO()->GetTargetEntity(eTARGET)->GetNetworkID();
	m_slNPCIdx = iMobIndex;
	m_nAffinityIndex = _pNetwork->GetAffinityData()->GetAffinityIndex(iMobIndex);
	CMobData* MD = CMobData::getData( iMobIndex );

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 4462,"친화도" ), UI_NPC_AFFINITY, MSGLCMD_AFFINITY_DONATE_REQ );

	CTString strNpcName = CMobData::getData(iMobIndex )->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	// 친화도 개편2 NPC 대화 얻기 [2/5/2013 Ranma]
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, TRUE, 
		pUIManager->GetString(_pNetwork->GetAffinityData()->GetAffinityStringIndex(iMobIndex)),
		-1, 0xA3A1A3FF );

	CTString strMessage;
	// [100323: selo] 플래그에 따라 메뉴를 보여준다
	CTString strAffinityName = _pNetwork->GetAffinityData()->GetAffinityName( iMobIndex );

	if( _pNetwork->GetAffinityData()->IsContract(iMobIndex) )
	{	
		// contract to force.
		strMessage.PrintF( _S( 5298, "%s 세력과 친구 맺기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_CONTRACT );
	}
	if( _pNetwork->GetAffinityData()->IsDonate(iMobIndex) )
	{	
		// donate item to force.
		strMessage.PrintF( _S( 5299, "%s 세력에 아이템 기부하기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_DONATE );
	}
	if( _pNetwork->GetAffinityData()->IsShop(iMobIndex) )
	{	
		// using force shop.
		strMessage.PrintF( _S( 5300, "%s 세력의 상점 이용하기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_SHOP );
	}
	if( _pNetwork->GetAffinityData()->IsPresent(iMobIndex) )
	{	
		// take a present.
		strMessage.PrintF( _S( 5301, "%s 세력에게 선물 받기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_PRESENT );
	}
	// quest
	if( bHasQuest )
	{
		strMessage.PrintF( _S( 1053, "이야기한다." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_TALK );
	}
	// event
	if( MD->IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_EVENT );
	}
	// cancel
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage );
}

void CAffinityUI::ToggleVisible()
{
	BOOL bVisible = !( IsVisible() );

	if( bVisible )
	{
		OpenDonation();
	}
	else
	{
		CloseDonation();
	}
}

void CAffinityUI::OpenDonation()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if( IsVisible() == TRUE || GetHide() == FALSE)
		return;

	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITY, TRUE );
	CUIManager::getSingleton()->GetInventory()->Lock(TRUE, TRUE, LOCK_AFFINITY_DONATION);
}

void CAffinityUI::CloseDonation()
{
	DonationReset();
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITY, FALSE );
	CUIManager::getSingleton()->GetInventory()->Lock(FALSE, FALSE, LOCK_AFFINITY_DONATION);
}

void CAffinityUI::DonationReset()
{
	m_nAffinityPoint = 0;

	m_vecItem.clear();
}

void CAffinityUI::OpenShop(const int npcIdx)
{
	CUIManager::getSingleton()->GetShop()->OpenShop(npcIdx, -1, 0, m_fNpcX, m_fNpcZ);
}

void CAffinityUI::CloseAllUI()
{
	CloseDonation();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL( MSGLCMD_AFFINITY_DONATE_REQ );
	pUIManager->CloseMessageBox( MSGCMD_AFFINITY_CONTRACT );
	pUIManager->CloseMessageBox( MSGCMD_AFFINITY_TAKEPRESENT );
}

const BOOL CAffinityUI::IsOpenUI() const
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	return IsVisible() ||
		pUIManager->DoesMessageBoxLExist( MSGLCMD_AFFINITY_DONATE_REQ ) ||
		pUIManager->DoesMessageBoxExist( MSGCMD_AFFINITY_CONTRACT ) || pUIManager->DoesMessageBoxExist( MSGCMD_AFFINITY_TAKEPRESENT );
}

void CAffinityUI::_sendDonateMessage()
{
	int nItemCnt = 0, nAffinityPoint = 0;
	std::vector<Item>::iterator b;
	for (b = m_vecItem.begin(); b != m_vecItem.end(); b++)
	{
		if (b->count > 0)
		{
			nItemCnt++;
			nAffinityPoint += b->count * _pNetwork->GetAffinityData()->GetAffinityPoint(m_slNPCIdx, b->index);
		}
	}
	if (nItemCnt > 0)
	{
		CNetworkMessage nm;

		RequestClient::doAffinityContribute* packet = reinterpret_cast<RequestClient::doAffinityContribute*>(nm.nm_pubMessage);

		packet->type = MSG_EXTEND;
		packet->subType = htonl(MSG_EX_AFFINITY);
		packet->thirdType = MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REQ;
		packet->npcIndex = m_slNPCVIdx;
		packet->clientPoint = nAffinityPoint;
		packet->listCount = nItemCnt;

		CPrintF("Donation Item count is %d\n", nItemCnt);

		int idx = 0;
		CUIInventory* pInven = CUIManager::getSingleton()->GetInventory();
		for (b = m_vecItem.begin(); b != m_vecItem.end(); b++)
		{
			if (b->count > 0 && 
				ItemHelp::HaveItem(b->index) &&
				pInven->GetItemCount(b->index, false))
			{
				packet->list[idx].itemDBIndex = b->index;
				packet->list[idx].count = b->count;

				++idx;
			}
		}

		nm.setSize(sizeof(*packet) + (sizeof(packet->list[0]) * nItemCnt));

		_pNetwork->SendToServerNew(nm);
	}

	CloseDonation();
}

void CAffinityUI::Donation_Sum(int nSelect)
{
	Item* pItem = GetDonationItem(nSelect);

	if (pItem == NULL)
		return;

	int nInvenItemCnt = UIMGR()->GetInventory()->GetItemCount(pItem->index);

	if (nInvenItemCnt <= 0)
		return;

	int nTempCnt = pItem->count;
	++nTempCnt;

	if (nInvenItemCnt < nTempCnt)
		return;

	UpdateWant2DonationPoint(nSelect, pItem->count, nTempCnt);
	pItem->count = nTempCnt;
	UpdateItemCount(nSelect, nTempCnt);
}

void CAffinityUI::Donation_Sub(int nSelect)
{
	Item* pItem = GetDonationItem(nSelect);

	if (pItem == NULL)
		return;

	int nTempCnt = pItem->count;
	--nTempCnt;

	if (nTempCnt < 0)
		return;

	UpdateWant2DonationPoint(nSelect, pItem->count, nTempCnt);
	pItem->count = nTempCnt;
	UpdateItemCount(nSelect, nTempCnt);
}

void CAffinityUI::Donation_Max(int nSelect)
{
	Item* pItem = GetDonationItem(nSelect);

	if (pItem == NULL)
		return;

	int nInvenItemCnt = UIMGR()->GetInventory()->GetItemCount(pItem->index);
	UpdateWant2DonationPoint(nSelect, pItem->count, nInvenItemCnt);
	pItem->count = nInvenItemCnt;
	UpdateItemCount(nSelect, nInvenItemCnt);
}

CUIListItem* CAffinityUI::GetListItem(int idx)
{
	if (m_pListAffinity == NULL)
		return NULL;

	return (CUIListItem*)m_pListAffinity->GetListItem(idx);
}

CAffinityUI::Item* CAffinityUI::GetDonationItem( int idx )
{
	if (m_vecItem.size() < 0 || idx >= m_vecItem.size())
		return NULL;

	return &m_vecItem[idx];
}

void CAffinityUI::UpdateItemCount( int nSelect, int nCnt )
{
	CUIListItem* pItem = GetListItem(nSelect);

	if (pItem == NULL)
		return;

	CUIText* pCur = (CUIText*)pItem->findUI("text_cur");

	if (pCur == NULL)
		return;

	char pBuff[10];
	itoa(nCnt, pBuff, 10);
	pCur->SetText(CTString(pBuff));
}

void CAffinityUI::UpdateWant2DonationPoint(int nSelect, int nOld, int nCur)
{
	Item* pItem = GetDonationItem(nSelect);

	if (pItem == NULL)
		return;

	int nPoint = _pNetwork->GetAffinityData()->GetAffinityPoint(m_slNPCIdx, pItem->index);
	m_nAffinityPoint += (nCur - nOld) * nPoint;	

	if (m_pWant2DonationPoint == NULL)
		return;

	CTString strTemp;
	strTemp.PrintF("%d", m_nAffinityPoint);
	m_pWant2DonationPoint->SetText(strTemp);
}

void CAffinityUI::UpdateListItem()
{
	if (m_pListAffinity == NULL)
		return;

	CUIInventory* pInven = UIMGR()->GetInventory();

	CTString strTemp;
	std::vector<CAffinityData::_DonationItemInfo>::iterator b, e;
	b = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.begin();
	e = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.end();

	CUIListItem* pItemTmp = m_pListAffinity->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	CUIButton* pBtn = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;

	m_pListAffinity->DeleteAllListItem();

	int nCnt = 0;
	for (nCnt = 0; b != e; b++, nCnt++)
	{
		m_pListAffinity->AddListItem(pItemTmp->Clone());		

		pItem = (CUIListItem*)m_pListAffinity->GetListItem(nCnt);

		if (pItem == NULL)
			continue;

		if (pIcon = (CUIIcon*)pItem->findUI("icon_item"))
		{
			pIcon->setAffinity(eETC_TYPE_AFFINITY_ITEM, b->index, b->point);
		}

		if (pText = (CUIText*)pItem->findUI("text_name"))
		{
			pText->SetText(CTString(_pNetwork->GetItemName(b->index)));
		}

		if (pText = (CUIText*)pItem->findUI("text_max"))
		{
			strTemp.PrintF("%d", (int)pInven->GetItemCount(b->index, false));
			pText->SetText(strTemp);
		}
		
		if (pBtn = (CUIButton*)pItem->findUI("btn_sum"))
		{
			CmdAffinitySum* pCmd = new CmdAffinitySum();
			pCmd->setData(this, nCnt);
			pBtn->SetCommandUp(pCmd);
		}

		if (pBtn = (CUIButton*)pItem->findUI("btn_sub"))
		{
			CmdAffinitySub* pCmd = new CmdAffinitySub();
			pCmd->setData(this, nCnt);
			pBtn->SetCommandUp(pCmd);
		}

		if (pBtn = (CUIButton*)pItem->findUI("btn_max"))
		{
			CmdAffinityMax* pCmd = new CmdAffinityMax();
			pCmd->setData(this, nCnt);
			pBtn->SetCommandUp(pCmd);
		}

		m_vecItem.push_back(Item(b->index));
		UpdateItemCount(nCnt, 0);
	}

	if (m_pListAffinity->GetScroll() != NULL)
	{
		m_pListAffinity->GetScroll()->SetScrollPos(0);
		m_pListAffinity->UpdateScroll(nCnt);
	}
	
	m_pListAffinity->UpdateList();

	if (m_pWant2DonationPoint != NULL)
		m_pWant2DonationPoint->SetText(CTString("0"));
}

void CAffinityUI::UpdateDonationPoint()
{
	if (m_pTextDonationPoint == NULL)
		return;

	int nCur = 0;
	int nMax = 0;
	CTString strTemp, strPoint;
	CTString strCurPoint;
	CTString strMaxPoint;
	CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
	if (iterPoint == _pNetwork->AffinityInfo.mapAffinityList.end())
		return;

	nCur = iterPoint->second.current;
	nMax = iterPoint->second.max;

	strTemp.PrintF(_S(5861, "친화도 수치"));

	strCurPoint = UIMGR()->IntegerToCommaString(nCur);
	strMaxPoint = UIMGR()->IntegerToCommaString(nMax);
	strPoint.PrintF(" %s/%s", strCurPoint, strMaxPoint);
	strTemp += strPoint;

	m_pTextDonationPoint->SetText(strTemp);
}

WMSG_RESULT CAffinityUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CAffinityUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if ( IsInside(x, y) )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		pUIManager->RearrangeOrder(UI_NPC_AFFINITY, TRUE);

		m_nOldX = x;
		m_nOldY = y;

		if ( m_pDragArea != NULL && m_pDragArea->IsInside(x, y) == TRUE )
		{
			m_bDrag = true;
			return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT CAffinityUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return WMSG_FAIL;
}