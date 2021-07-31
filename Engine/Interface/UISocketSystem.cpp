/********************************************************************
	UISocketSystem.cpp : implementation of the CUISocketSystem class.
*********************************************************************/

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Contents/function/SocketRemoveAllUI.h>
#include <Engine/Contents/function/SocketRemoveSingleUI.h>
#include <Engine/Contents/function/SocketCreateUI.h>
#include <Engine/Contents/function/SocketCombineUI.h>
#include <Engine/Contents/function/JewelComposUI.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Entities/InternalClasses.h>
#include <Common/Packet/ptype_old_do_exsocket.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Loading.h>
#include <Engine/Info/MyInfo.h>

enum eSocketSystemSelection
{
	SOCKET_CREATE,
	SOCKET_COMBINE,
	SOCKET_JEWEL_COMPOS,
	SOCKET_CHAOSJEWEL_COMPOS,
	SOCKET_EMPTY,
	SOCKET_ONCEEMPTY,
	SOCKET_CHANCECARD,
	SOCKET_SHOP,
	SOCKET_TALK,
	SOCKET_EVENT,
};

CUISocketSystem::CUISocketSystem()
:	m_euSocketStat( STAT_CREATE ),
	m_pIconEmpty(NULL)
{
}

CUISocketSystem::~CUISocketSystem()
{
	Destroy();
	SAFE_DELETE(m_pIconEmpty);
}

//////////////////////////////////////////////////////////////////////////
// Protect functions.
void CUISocketSystem::_toggleVisible( const _SOCKET_STAT _status /* = STAT_CREATE  */)
{
	m_euSocketStat = _status;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( _status == STAT_CREATE )
	{
		CSocketCreateUI* pUI = pUIManager->GetSocketCreate();

		if (pUI != NULL)
		{
			if (pUI->GetHide() == TRUE)
				pUI->openUI();
			else
				pUI->closeUI();
		}
		return;
	}
	else if( _status == STAT_COMBINE )
	{
		CSocketCombineUI* pUI = pUIManager->GetSocketCombine();

		if (pUI != NULL)
		{
			if (pUI->GetHide() == TRUE)
				pUI->openUI();
			else
				pUI->closeUI();
		}
	}
	else if( _status == STAT_JEWEL_COMPOS || _status == STAT_CHAOSJEWEL_COMPOS)	
	{
		CJewelComposUI* pUI = pUIManager->GetSocketJewelCompos();

		if (pUI != NULL)
		{
			if (pUI->GetHide() == TRUE)
			{
				int nOpenType;
				if (_status == STAT_JEWEL_COMPOS)
					nOpenType = eJEWEL_COMPOS_OPEN_NORMAL;
				else
					nOpenType = eJEWEL_COMPOS_OPEN_CHAOS;

				pUI->openUI(nOpenType);
			}
			else
				pUI->closeUI();
		}
	}
	else if( _status == STAT_EMPTY )
	{
		CSocketRemoveAllUI* pUI = pUIManager->GetSocketRemoveAll();

		if (pUI != NULL)
		{
			if (pUI->GetHide() == TRUE)
				pUI->openUI();
			else
				pUI->closeUI();
		}
	}
	else if ( _status == STAT_ONCEEMPTY )
	{
		CSocketRemoveSingleUI* pUI = pUIManager->GetSocketRemoveSingle();

		if (pUI != NULL)
		{
			if (pUI->GetHide() == TRUE)
				pUI->openUI();
			else
				pUI->closeUI();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_close()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_euSocketStat == STAT_EMPTY)
		pUIManager->GetSocketRemoveAll()->closeUI();
	else if (m_euSocketStat == STAT_CREATE)
		pUIManager->GetSocketCreate()->closeUI();
 	else if (m_euSocketStat == STAT_COMBINE)
 		pUIManager->GetSocketCombine()->closeUI();
	else if (m_euSocketStat == STAT_ONCEEMPTY)
		pUIManager->GetSocketRemoveSingle()->closeUI();
	else
		pUIManager->GetSocketJewelCompos()->closeUI();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL) )
		pUIManager->CloseMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::showErrorMessage( const _SOCKET_ERROR_TYPE type /*=SOCKET_ERROR_INEXACT*/)
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	if( type == SOCKET_ERROR_INEXACT )
		MsgInfo.AddString( _S( 4981, "올바른 아이템이 아닙니다. 확인 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_EMPTY )
		MsgInfo.AddString( _S( 4982, "소켓 아이템을 먼저 장착한 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_EXCEED )
		MsgInfo.AddString( _S( 4983, "소캣 개수를 초과하였습니다. 확인 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_ITEM_LACK )
		MsgInfo.AddString( _S( 5011,"아이템 개수가 부족합니다. 아이템 개수를 확인해 주세요.") );
	else if( type == SOCKET_ERROR_JEWEL_OVERLAP )
		MsgInfo.AddString( _S( 5012, "같은 종류의 보석은 더 이상 결합할 수 없습니다.") );
	else if( type == SOCKET_ERROR_COMBINE_TERMS )
		MsgInfo.AddString( _S( 5924, "결합 조건에 맞지 않은 보석입니다.") );
	else if( type == SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT )
		MsgInfo.AddString( _S( 4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));

	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void CUISocketSystem::_showErrorSystemMessage( const _JEWEL_ERROR_TYPE type)
{
	CTString strMessage;
	
	if ( type == JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL )
		strMessage.PrintF( _S( 5931, "일반보석을 등록하세요.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL )
		strMessage.PrintF( _S( 5929, "카오스보석을 등록하세요.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL)
		strMessage.PrintF( _S( 4346, "등록할 수 없는 아이템 입니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_INVENTORY)
		strMessage.PrintF( _S( 5934, "합성하는 보석이 없습니다.") );
	else if (type == JEWEL_COMPOS_ERROR_JEWEL_LACK)
		strMessage.PrintF( _S( 5935, "보석이 부족합니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NAS_LACK)
		strMessage.PrintF( _S( 306, "나스가 부족합니다.") );
	else if (type == JEWEL_COMPOS_ERROR_COMPOS_SCROLL)
		strMessage.PrintF( _S( 5936, "보석 합성스크롤이 잘못 되었습니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY)
		strMessage.PrintF( _S( 265, "인벤토리 공간이 부족합니다.") );

	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}

void CUISocketSystem::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_pIconEmpty = new CUIIcon;
	m_pIconEmpty->Create(NULL, 0, 0, BTN_SIZE, BTN_SIZE, UI_SOCKETSYSTEM, UBET_ITEM);
}

void CUISocketSystem::ToggleVisible( const _SOCKET_STAT _status /* = STAT_CREATE  */)
{
	_toggleVisible( _status );
}

void CUISocketSystem::OpenSocketSystem(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM_EMPTY ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxExist( MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS ) || IsVisible() )
		return;

	CMobData* MD = CMobData::getData( iMobIndex );

	m_nNPCIndex = iMobIndex;
	m_fNPCPosX = fX;
	m_fNPCPosZ = fZ;

	if (CEntity* pEntity = INFO()->GetTargetEntity(eTARGET))
		m_nNPCVIdx = pEntity->GetNetworkID();

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 4989, "소켓 생성 및 보석 관리" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM );
	CTString strNpcName = CMobData::getData( iMobIndex )->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, TRUE, _S( 4990, "아이템을 가져오면 소켓을 뚫어주겠네." 
																	"단 소켓 가공을 이미 끝낸 아이템은 다시 "
																	"작업하기 힘들어 가공이 안되니 주의하게. "
																	"그리고 소켓 뚫린 아이템과 보석을 가져다 주면, "
																	"내가 소켓 홈에다 가 보석을 결합시켜 주겠네."), -1, 0xA3A1A3FF );

	CTString strMessage;
	strMessage.PrintF( _S( 4973, "소켓 생성" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_CREATE );
	strMessage.PrintF( _S( 4976, "보석 결합" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_COMBINE );
	strMessage.PrintF( _S( 5925, "보석 합성" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_JEWEL_COMPOS );
	strMessage.PrintF( _S( 4978, "보석 제거" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_EMPTY );
	strMessage.PrintF( _S( 4991, "재료 구매" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_SHOP );
	// quest
	if( bHasQuest )
	{
		strMessage.PrintF( _S( 1053, "이야기 한다." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_TALK );
	}
	// event
	if( MD->IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_EVENT );
	}
	// cancel
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage );
}

void CUISocketSystem::MsgBoxLCommand(int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (nResult >= 0)	// 소켓 UI를 열려고 할때.
	{
		if (pUIManager->GetInventory()->IsLocked() == TRUE ||
			pUIManager->GetInventory()->IsLockedArrange() == TRUE)
		{
			// 이미 Lock 인 창이 있을 경우 열지 못한다.
			pUIManager->GetInventory()->ShowLockErrorMessage();
			return;
		}
	}
	switch( nCommandCode )
	{
		case MSGLCMD_SOCKET_SYSTEM:
		{
			if( nResult == SOCKET_CREATE )
			{
				ToggleVisible( STAT_CREATE );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_COMBINE )
			{
				ToggleVisible( STAT_COMBINE );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_JEWEL_COMPOS )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				pUIManager->CreateMessageBoxL( _S( 5925, "보석 합성" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS );
				CTString strNpcName = CMobData::getData( m_nNPCIndex )->GetName();
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, TRUE, _S( 5926, "보석 3개를 합성하여 새로운 보석을 만들어줄수 있지."
					"아주 가끔은 카오스 보석이 나올수도 있다네."
					"마법 스크롤을 사용하면 좋은 보석을 얻을수 있다네."), -1, 0xA3A1A3FF );
				
				CTString strMessage;
				strMessage.PrintF( _S( 5927, "일반보석 합성" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage, SOCKET_JEWEL_COMPOS );
				strMessage.PrintF( _S( 5928, "카오스보석 합성" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage, SOCKET_CHAOSJEWEL_COMPOS );
				strMessage.PrintF( _S( 1220, "취소한다." ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage );
			}
			else if( nResult == SOCKET_EMPTY )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				pUIManager->CreateMessageBoxL( _S( 4978, "보석 제거" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM_EMPTY );
				CTString strNpcName = CMobData::getData( m_nNPCIndex )->GetName();
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, TRUE, _S( 5895, "보석이 결합된 장비에서 보석을 제거하는"
					"작업은 매우 어려운데다가 원하는 보석만 제거하려면 특별한 마법 스크롤도 필요하지. "
					"게다가 제거 작업 중에 보석이 파괴되는 경우가 많으니 이점도 꼭 명심하게."), -1, 0xA3A1A3FF );

				CTString strMessage;
				strMessage.PrintF( _S( 5896, "보석 일괄 제거" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage, SOCKET_EMPTY );
				strMessage.PrintF( _S( 5897, "보석 개별 제거" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage, SOCKET_ONCEEMPTY );
				strMessage.PrintF( _S( 1220, "취소한다." ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage );
			}
			else if( nResult == SOCKET_SHOP )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
					return;

				pUIManager->GetShop()->OpenShop( m_nNPCIndex, -1, 0, m_fNPCPosX, m_fNPCPosZ );
			}
			else if( nResult == SOCKET_TALK )
			{

			}
			else if( nResult == SOCKET_EVENT )
			{

			}
			else
			{

			}
		}// end case MSGLCMD
		break;
		case MSGLCMD_SOCKET_SYSTEM_EMPTY:
		{
			if( nResult == SOCKET_EMPTY )
			{
				ToggleVisible( STAT_EMPTY );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_ONCEEMPTY )
			{
				ToggleVisible( STAT_ONCEEMPTY );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
		}
		break;
		case MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS:
			{
				if( nResult == SOCKET_JEWEL_COMPOS )
				{
					ToggleVisible( STAT_JEWEL_COMPOS );

					pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
				}
				else if( nResult == SOCKET_CHAOSJEWEL_COMPOS )
				{
					ToggleVisible( STAT_CHAOSJEWEL_COMPOS );

					pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
				}
			}
			break;
	} // end switch.
}

//////////////////////////////////////////////////////////////////////////
// user define network message functions.
void CUISocketSystem::SendCreateReq(SWORD item_Tab1, SWORD invenIdx1, LONG itemVirIdx1, SWORD item_Tab2, SWORD invenIdx2, LONG itemVirIdx2)
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketMake* packet = reinterpret_cast<RequestClient::doExSocketMake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_MAKE_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = item_Tab1;
	packet->invenIndex_1 = invenIdx1;
	packet->virualIndex_1 = itemVirIdx1;
	packet->tab_2 = item_Tab2;
	packet->invenIndex_2 = invenIdx2;
	packet->virualIndex_2 = itemVirIdx2;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}
//-----------------------------------------------------------------
//	Jewel Index
//					2
//				7		3
//					1
//				6		4
//					5
//					|
//					|--	8 (파괴방지석)
//					0

void CUISocketSystem::SendCombineReq(stPACKET_ITEM Item, stPACKET_ITEM Juwel, stPACKET_ITEM Scroll)
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCombineJewel* packet = reinterpret_cast<RequestClient::doExSocketCombineJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_COMBINE_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;

	packet->tab_1 = Item.Item_Tab;
	packet->invenIndex_1 = Item.InvenIdx;
	packet->virualIndex_1 = Item.ItemVirIdx;

	packet->tab_2 = Juwel.Item_Tab;
	packet->invenIndex_2 = Juwel.InvenIdx;
	packet->virualIndex_2 = Juwel.ItemVirIdx;
	packet->pos = Juwel.SlotIdx;

	packet->tab_3 = Scroll.Item_Tab;
	packet->invenIndex_3 = Scroll.InvenIdx;
	packet->virualIndex_3 = Scroll.ItemVirIdx;

	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendUpgradeReq(bool bChaosJewel, stPACKET_ITEM Jewel1, stPACKET_ITEM Jewel2, stPACKET_ITEM Jewel3, stPACKET_ITEM Scroll, int nScrollCnt)
{
	// doExSocketUpgradeChaosJewel 공용 사용

	CNetworkMessage nmMessage;
	RequestClient::doExSocketUpgradeJewel* packet = reinterpret_cast<RequestClient::doExSocketUpgradeJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);

	if (bChaosJewel == true)
		packet->thirdType = MSG_EX_SOCKET_UPGRADE_JEWEL_CHAOS_REQ;
	else
		packet->thirdType = MSG_EX_SOCKET_UPGRADE_JEWEL_REQ;

	packet->npcIndex = m_nNPCVIdx;

	packet->tab_1 = Jewel1.Item_Tab;
	packet->invenIndex_1 = Jewel1.InvenIdx;
	packet->virualIndex_1 = Jewel1.ItemVirIdx;

	packet->tab_2 = Jewel2.Item_Tab;
	packet->invenIndex_2 = Jewel2.InvenIdx;
	packet->virualIndex_2 = Jewel2.ItemVirIdx;

	packet->tab_3 = Jewel3.Item_Tab;
	packet->invenIndex_3 = Jewel3.InvenIdx;
	packet->virualIndex_3 = Jewel3.ItemVirIdx;

	packet->tab_4 = Scroll.Item_Tab;
	packet->invenIndex_4 = Scroll.InvenIdx;
	packet->virualIndex_4 = Scroll.ItemVirIdx;

	packet->itemCount = nScrollCnt;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendCleanAllJewelReq(SWORD item_Tab, SWORD invenIdx, LONG itemVirIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCleanJewel* packet = reinterpret_cast<RequestClient::doExSocketCleanJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_CLEAN_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab = item_Tab;
	packet->invenIndex = invenIdx;
	packet->virualIndex = itemVirIdx;

	nmMessage.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendCleanOneJewelReq(SWORD tab, SWORD inven_idx, SLONG index)
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCleanOneJewel* packet = reinterpret_cast<RequestClient::doExSocketCleanOneJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_CLEAN_ONE_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = m_pIconEmpty->getItems()->Item_Tab;
	packet->invenIndex_1 = m_pIconEmpty->getItems()->InvenIndex;
	packet->virualIndex_1 = m_pIconEmpty->getItems()->Item_UniIndex;

	packet->tab_2 = tab;
	packet->invenIndex_2 = inven_idx;
	packet->virualIndex_2 = index;

	packet->pos = m_nEmptyJewelPos;

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

const BOOL CUISocketSystem::IsOpenedUI() const
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	BOOL bOpen = IsVisible() || pUIMgr->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM ) ||
		pUIMgr->GetSocketRemoveSingle()->IsVisible() ||
		pUIMgr->GetSocketRemoveAll()->IsVisible() ||
		pUIMgr->GetSocketCreate()->IsVisible() || 
		pUIMgr->GetSocketCombine()->IsVisible() ||
		pUIMgr->GetSocketJewelCompos()->IsVisible();

	return bOpen;
}

void CUISocketSystem::CloseAllUI()
{
	CloseSocketSystem();
	CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_SOCKET_SYSTEM );	
}

void CUISocketSystem::absCloseAllUI()
{
	CloseSocketSystem();
	CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_SOCKET_SYSTEM );	
}

int CUISocketSystem::IsEmptyInvenSlotCount()
{
	if( CUIManager::getSingleton()->GetInventory()->IsInventoryFull() )
		return 0;

	int	  nInvenSlotMax =  INVEN_SLOT_COL * INVEN_SLOT_ROW_TOTAL;
	SQUAD count = CUIManager::getSingleton()->GetInventory()->GetItemAll();

	return nInvenSlotMax - count;
}

void CUISocketSystem::SetRemoveSingleItem( CUIIcon* pItem )
{
	if (pItem == NULL)
		return;

	m_pIconEmpty->clearIconData();
	m_pIconEmpty->copyItem(pItem);
}

void CUISocketSystem::SetRemoveSingleJewelPos( int nJewelPos )
{
	if (nJewelPos < 0 || nJewelPos >= JEWEL_MAX_COUNT)
		return;

	m_nEmptyJewelPos = nJewelPos;
}
