#include "stdh.h"
// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include "ExChangeUI.h"

#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Contents/function/WareHouseUI.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Contents/Base/PersonalshopUI.h>

// Exchange request type
enum ExchangeReq
{
	EXCHREQ_REQ				= 0,
	EXCHREQ_REP				= 1,
	EXCHREQ_REJECT			= 2,
	EXCHREQ_REJECT_SRC		= 3,
	EXCHREQ_REJECT_DEST		= 4,
	EXCHREQ_READY			= 5,
	EXCHREQ_READY_SRC		= 6,
	EXCHREQ_READY_DST		= 7,
	EXCHREQ_FULL_SRC		= 8,
	EXCHREQ_FULL_DEST		= 9,
	EXCHREQ_REQ_OK			= 10,
	EXCHREQ_REQ_OK_SRC		= 11,
	EXCHREQ_REQ_OK_DEST		= 12
};


// Exchange item action type
enum ExchangeItemAction
{
	EXCHITEM_ADD			= 0,
	EXCHITEM_ADD_SRC		= 1,
	EXCHITEM_ADD_DEST		= 2,
	EXCHITEM_DEL			= 3,
	EXCHITEM_DEL_SRC		= 4,
	EXCHITEM_DEL_DEST		= 5,
	EXCHITEM_UPDATE_SRC		= 6,
	EXCHITEM_UPDATE_DEST	= 7
};


// Define exchange item slot
#define	EXCH_EXCH_SLOT_COL			5
#define EXCH_EXCH_SLOT_ROW			2

#define	EXCH_INVEN_SLOT_COL			5
#define	EXCH_INVEN_SLOT_ROW			4		

extern INDEX g_iCountry;
extern INDEX g_iRejectExchange;	// [7/13/2009 rumist] reject request of exchange message.

class CmdExChangeAddItem : public Command
{
public:
	CmdExChangeAddItem() : m_pWnd(NULL)	{}
	void setData(CExchangeUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CExchangeUI* m_pWnd;
};

class CmdExchangeAddItem : public Command
{
public:
	CmdExchangeAddItem() : m_pWnd(NULL)
						 , m_nTab(-1)
						 , m_nInvenIdx(-1)
						 , m_nUniIndex(-1)
						 , m_llCount(-1)	{};

	void setData(CExchangeUI* pWnd, int nTab, int nInvenIdx, int nUniIndex, SQUAD llCount)
	{
		m_pWnd = pWnd;
		m_nTab = nTab;
		m_nInvenIdx = nInvenIdx;
		m_nUniIndex = nUniIndex;
		m_llCount = llCount;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->AddExchItem(m_nTab, m_nInvenIdx, m_nUniIndex, m_llCount);
		}
	}
private:
	CExchangeUI* m_pWnd;
	int m_nTab;
	int m_nInvenIdx;
	int m_nUniIndex;
	SQUAD m_llCount;
};

CExchangeUI::CExchangeUI()
	: m_pBtnReady(NULL)
	, m_pBtnExchange(NULL)
	, m_pArrUser(NULL)
	, m_pArrMy(NULL)
	, m_pArrInven(NULL)
	, m_pTxtUserGold(NULL)
	, m_pTxtMyGold(NULL)
	, m_pTxtInvenGold(NULL)
	, m_pImgNoExchange(NULL)
	, m_pTxtTargetName(NULL)
{
	setInherit(false);
	m_nSelMyItemID = -1;
	m_nSelITab = -1;
	m_nSelInvenItemID = -1;
	m_bShowItemInfo = FALSE;
	m_bDetailItemInfo = FALSE;
	m_nCurInfoLines = 0;

	m_bAmISrc = FALSE;
	m_nSrcIndex = -1;
	m_nDestIndex = -1;
	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;
}

CExchangeUI::~CExchangeUI()
{
}
void CExchangeUI::initialize()
{
#ifndef		WORLD_EDITOR
	CUIBase* pBase = NULL;

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CExchangeUI::SendExchangeReq_Rej, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CExchangeUI::SendExchangeReq_Rej, this));

	if (m_pTxtInvenGold = (CUIText*)findUI("text_inven_gold"))
		m_pTxtInvenGold->SetCommandFUp(boost::bind(&CExchangeUI::ExchangeNas, this));

	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pTxtUserGold = (CUIText*)findUI("text_user_gold");
	m_pTxtMyGold = (CUIText*)findUI("text_my_gold");
	m_pTxtTargetName = (CUIText*)findUI("text_id");

	if (m_pBtnReady = (CUIButton*)findUI("btn_ready"))
		m_pBtnReady->SetCommandFUp(boost::bind(&CExchangeUI::SendExchangeReq_Ready, this));

	if (m_pBtnExchange = (CUIButton*)findUI("btn_trade"))
		m_pBtnExchange->SetCommandFUp(boost::bind(&CExchangeUI::SendExchangeReq_Ok, this));

	m_pImgNoExchange = (CUIImage*)findUI("img_no_exchange");

	if (m_pArrUser = (CUIArray*)findUI("arr_user_item"))
	{
		m_pArrUser->SetRowCol(EXCH_EXCH_SLOT_ROW, EXCH_EXCH_SLOT_COL);
		InitArrayItem(m_pArrUser, m_pIconsOppItems, EXCH_EXCH_SLOT_ROW, EXCH_EXCH_SLOT_COL);
	}

	if (m_pArrMy = (CUIArray*)findUI("arr_my_item"))
	{
		m_pArrMy->SetRowCol(EXCH_EXCH_SLOT_ROW, EXCH_EXCH_SLOT_COL);
		InitArrayItem(m_pArrMy, m_pIconsMyItems, EXCH_EXCH_SLOT_ROW, EXCH_EXCH_SLOT_COL);
	}

	if (m_pArrInven = (CUIArray*)findUI("arr_inven"))
	{
		int row = DEF_INVENTORY_MAX / EXCH_INVEN_SLOT_COL;
		m_pArrInven->SetRowCol(row, EXCH_INVEN_SLOT_COL);
		InitArrayItem(m_pArrInven, m_pIconsInvenItems, row, EXCH_INVEN_SLOT_COL);
	}
#endif		// WORLD_EDITOR
}

void CExchangeUI::InitArrayItem( CUIArray* pArr, CUIIcon** pPtr, int nRow, int nCol )
{
	if (pArr == NULL)
		return;

	int i, arr_max, item_max = nRow * nCol;
	CUIArrayItem* pItem = NULL;
	CUIArrayItem* pItemTmp = NULL;

	pItemTmp = pArr->GetArrayItemTemplate();
	arr_max = pArr->GetArrayChildCount();

	for (i = 0; i < item_max; ++i)
	{
		if (arr_max <= i)
			pArr->AddArrayItem((CUIArrayItem*)pItemTmp->Clone());

		pItem = pArr->GetArrayItem(i);

		if (pItem == NULL)
			continue;

		pPtr[i] = (CUIIcon*)pItem->findUI("icon_item");

		if (pPtr[i] == NULL)
			continue;

		pPtr[i]->clearIconData();
	}
}

void CExchangeUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CExchangeUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CExchangeUI::PrepareExchange()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_WAREHOUSE))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S(1890 ,"창고 사용중에는 교환을 할수 없습니다." ) );	
		pUIManager->GetWareHouseUI()->closeUI();
	}

	// Close message box of inventory
	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_EXCHANGE );

	// Initialize inventory items of exchange
	int		i;
	int		tab, idx;

	for (i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			tab = INVENTORY_TAB_CASH_2;
			idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			tab = INVENTORY_TAB_CASH_1;
			idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			tab = INVENTORY_TAB_NORMAL;
			idx = i;
		}

		CItems*		pItems = &_pNetwork->MySlotItem[tab][idx];

		if( pItems->Item_Sum > 0 )
		{
			CItems* pClone = new CItems;
			memcpy(pClone, pItems, sizeof(CItems));
			m_pIconsInvenItems[i]->setData(pClone, false);
			m_pIconsInvenItems[i]->setCount(pItems->Item_Sum);

			CUIBase* pBase = NULL;
			if (UTIL_HELP()->IsAvailable4Sale(pItems, UI_EXCHANGE) == false)
			{
				// 교환 불가 이미지가 있다면 보여주기.
				pBase = m_pIconsInvenItems[i]->findUI("img_no_exchange");

				if (pBase == NULL)// 없다면 클론
				{
					pBase = m_pImgNoExchange->Clone();
					m_pIconsInvenItems[i]->addChild(pBase);
				}

				pBase->Hide(FALSE);
			}
			else
			{
				// 교환 가능인데 교환 불가 이미지가 있는 경우 하이드
				if (pBase = m_pIconsInvenItems[i]->findUI("img_no_exchange"))
					pBase->Hide(TRUE);

				// 교환 가능한 아이템만 이동 가능 하도록 커멘드 연결.
				CmdExchangeAddItem* pCmd = new CmdExchangeAddItem;
				pCmd->setData(this
					, pItems->Item_Tab
					, pItems->InvenIndex
					, pItems->Item_UniIndex
					, pItems->Item_Sum);
				m_pIconsInvenItems[i]->SetCommandDBL(pCmd);

				CmdDragIcon* pDrag = new CmdDragIcon;
				pDrag->setData(m_pIconsInvenItems[i]);
				m_pIconsInvenItems[i]->SetCommandDrag(pDrag);
			}
		}
	}

	// Set money
	if (m_pTxtInvenGold != NULL)
	{
		CTString strGold;
		if (_pNetwork->MyCharacterInfo.money > 0)
		{
			strGold.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
			pUIManager->InsertCommaToString( strGold );
		}
		else
		{
			strGold = "0";
		}

		m_pTxtInvenGold->SetText(strGold);
		m_pTxtInvenGold->setFontColor(pUIManager->GetNasColor(strGold));
	}

	// Set state of controls
	m_pBtnReady->SetEnable( TRUE );
	m_pBtnExchange->SetEnable( FALSE );

	if( m_bAmISrc )
		m_pTxtTargetName->SetText(m_strDestName);
	else
		m_pTxtTargetName->SetText(m_strSrcName);

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
}

void CExchangeUI::ResetExchange()
{
	m_nSelMyItemID = -1;
	m_nSelInvenItemID = -1;
	m_bShowItemInfo = FALSE;
	m_bDetailItemInfo = FALSE;
	m_nCurInfoLines = 0;

	m_bAmISrc = FALSE;
	m_nSrcIndex = -1;
	m_nDestIndex = -1;
	m_strSrcName = CTString( "" );
	m_strDestName = CTString( "" );
	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;
	m_nMyNas = 0;
	m_nTradeNas = 0;

	CUIBase* pBase = NULL;
	// Initialize buttons
	for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		m_pIconsOppItems[iItem]->clearIconData();
		m_pIconsMyItems[iItem]->clearIconData();

		// 교환 불가 이미지 하이드 처리.
		if (pBase = m_pIconsOppItems[iItem]->findUI("img_no_exchange"))
			pBase->Hide(TRUE);

		if (pBase = m_pIconsMyItems[iItem]->findUI("img_no_exchange"))
			pBase->Hide(TRUE);
	}

	
	for (int i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		m_pIconsInvenItems[i]->clearIconData();

		// 커멘드 해제.
		m_pIconsInvenItems[i]->SetCommandDrag(NULL);
		m_pIconsInvenItems[i]->SetCommandDBL(NULL);

		// 교환 불가 이미지 하이드 처리.
		if (pBase = m_pIconsInvenItems[i]->findUI("img_no_exchange"))
			pBase->Hide(TRUE);
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pTxtUserGold != NULL &&
		m_pTxtMyGold != NULL &&
		m_pTxtInvenGold != NULL)
	{
		m_pTxtUserGold->SetText(CTString("0"));
		m_pTxtMyGold->SetText(CTString("0"));
		m_pTxtInvenGold->SetText(CTString("0"));

		m_pTxtUserGold->setFontColor(DEF_UI_COLOR_WHITE);
		m_pTxtMyGold->setFontColor(DEF_UI_COLOR_WHITE);
		m_pTxtInvenGold->setFontColor(DEF_UI_COLOR_WHITE);
	}

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_EXCHANGE );

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_EXCHANGE );

	// Close message box of exchange
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
	pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );

	Hide(TRUE);
	pUIManager->RearrangeOrder( UI_EXCHANGE, FALSE );
}

static int		nTempUniIndex;
static SQUAD	llTempCount;

void CExchangeUI::AddExchItem( int nTab, int inven_idx, int nUniIndex, SQUAD llCount )
{
	UIMGR()->GetMsgBoxNumOnly()->CloseBox();

	if (m_bExchReady == TRUE)
		return;

	if(nUniIndex == 19)
	{
		SendExchangeItem_Add( nUniIndex, llCount );
		return;
	}

	nTempUniIndex = nUniIndex;
	llTempCount = llCount;

	CItems*		pItems = &_pNetwork->MySlotItem[nTab][inven_idx];
	CItemData*	pItemData = pItems->ItemData;

	if (pItemData == NULL)
		return;

	if( ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 )
	{
		CTString		strMessage;
		const char	*szItemName = _pNetwork->GetItemName(pItemData->GetItemIndex());
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdExChangeAddItem* pCmd = new CmdExChangeAddItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 149, "교환" ), strMessage, 1, pItems->Item_Sum);
	}
	else
	{
		SendExchangeItem_Add( nTempUniIndex, llTempCount);
	}
}

void CExchangeUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_EXCH_REQ_SRC:
		if( !bOK )
		{
			m_nSrcIndex = -1;
			m_nDestIndex = -1;
			SendExchangeReq_Rej();
		}

		break;

	case MSGCMD_EXCH_REQ_DEST:
		if( bOK )
			SendExchangeReq_Rep();
		else
			SendExchangeReq_Rej();

		break;

	case MSGCMD_EXCH_ADD_NAS:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}
			
			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				//if( llCount > 0 && llCount <= llTempCount )
				if( (llCount+m_nMyNas) > 0 && (llCount+m_nMyNas) <= _pNetwork->MyCharacterInfo.money)
				{
					CTString strGold;
					m_nMyNas		+= llCount;
					strGold.PrintF( "%I64d", m_nMyNas );
					CUIManager::getSingleton()->InsertCommaToString( strGold );
					AddExchItem(-1, -1, 19, m_nMyNas);

					if (m_pTxtMyGold != NULL)
					{
						m_pTxtMyGold->setFontColor(UIMGR()->GetNasColor(strGold));
						m_pTxtMyGold->SetText(strGold);
					}
				}
				else
				{
					CTString strInNasError;
					strInNasError.PrintF( _S( 5902, "현재 가지고 있는 나스 보다 많은 나스를 교환 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strInNasError );
				}
			}
		}
		
		break;
	}
}

void CExchangeUI::SendExchangeReq_Req( CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Already exchange
	if( m_nSrcIndex != -1 && m_nDestIndex != -1 )
	{
		CTString	strSysMessage;
		if( m_bAmISrc )
			strSysMessage.PrintF( _S2( 422, m_strDestName, "이미 %s<과> 교환중입니다." ), m_strDestName );
		else
			strSysMessage.PrintF( _S2( 422, m_strSrcName, "이미 %s<과> 교환중입니다." ), m_strSrcName );

		pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

		return;
	}
	
	// Find index of dest character
	SLONG	slDestIndex = -1;	
	INDEX	iSyndicateType = 0;


	ObjectBase* pObject = ACTORMGR()->GetObjectByName(strDestName.str_String, eOBJ_CHARACTER);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		if (pTarget->cha_sbShopType != PST_NOSHOP)
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 737, "개인 상점과는 교환을 할 수 없습니다." ), SYSMSG_ERROR );		
			return;
		}

		slDestIndex = pTarget->m_nIdxServer;
		iSyndicateType = pTarget->cha_iSyndicateType;

		if (_pNetwork->IsRvrZone() == TRUE)
		{
			if (iSyndicateType != _pNetwork->MyCharacterInfo.iSyndicateType)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다."), SYSMSG_ERROR);
				return;
			}
		}
	}

	// If dest character doesn't exist
	if( slDestIndex == -1 )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 151, "교환 대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	_pNetwork->ExchangeReq_Req( slDestIndex, strDestName );

	m_bAmISrc = TRUE;
	m_nSrcIndex = _pNetwork->MyCharacterInfo.index;
	m_nDestIndex = slDestIndex;
	m_strSrcName = _pNetwork->MyCharacterInfo.name;
	m_strDestName = strDestName;

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );

	// Create message box
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_CANCEL,
								UI_EXCHANGE, MSGCMD_EXCH_REQ_SRC );
	strMessage.PrintF( _S( 152, "%s에게 교환을 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_EXCHANGE );
}

void CExchangeUI::SendExchangeReq_Rep()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		_pNetwork->ExchangeReq_Rej();
		return;
	}

	_pNetwork->ExchangeReq_Rep();
}

void CExchangeUI::SendExchangeReq_Rej()
{
	_pNetwork->ExchangeReq_Rej();
}

void CExchangeUI::SendExchangeReq_Ready()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of exchange
	pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

	_pNetwork->ExchangeReq_Ready();
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}

void CExchangeUI::SendExchangeReq_Ok()
{
	_pNetwork->ExchangeReq_Ok();
}

void CExchangeUI::SendExchangeItem_Add( int nUniIndex, SQUAD llCount)
{

	_pNetwork->ExchangeItem_Add( nUniIndex, llCount );
}

void CExchangeUI::ExchangeReq_Req( int nSrcIndex, CTString &strSrcName,
									int nDestIndex, CTString &strDestName )
{
	m_bAmISrc = FALSE;
	m_nSrcIndex = nSrcIndex;
	m_nDestIndex = nDestIndex;
	m_strSrcName = strSrcName;
	m_strDestName = strDestName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );
	// rejection bug fix [8/5/2010 rumist]
	if( g_iRejectExchange )
	{
		// [7/13/2009 rumist] if i want reject request message, 
		// call send exchange request message reject function. 
		SendExchangeReq_Rej();
		return;
	}

	// Create message box
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_YESNO,
								UI_EXCHANGE, MSGCMD_EXCH_REQ_DEST );
	strMessage.PrintF( _S2( 153, m_strSrcName, "%s<가> 교환을 신청했습니다. 수락하시겠습니까?" ), m_strSrcName );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_EXCHANGE );
}

void CExchangeUI::ExchangeReq_Rep()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();

		SendExchangeReq_Rej();
		return;
	}

	if( m_bAmISrc )
	{
		pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		strMessage.PrintF( _S2( 154, m_strDestName, "%s<과> 교환을 시작합니다." ), m_strDestName );
	}
	else
		strMessage.PrintF( _S2( 154, m_strSrcName, "%s<과> 교환을 시작합니다." ), m_strSrcName );

	// Add system message
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	// Prepare exchange
	PrepareExchange();
}

void CExchangeUI::ExchangeReq_Rej( BOOL bSrcReject )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;

	if( m_bAmISrc )
	{
		if( bSrcReject )
			strMessage.PrintF( _S2( 155, m_strDestName, "%s<과>의 교환을 취소하셨습니다." ), m_strDestName );
		else
		{
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
			strMessage.PrintF( _S2( 156, m_strDestName, "%s<가> 교환을 취소했습니다." ), m_strDestName );
		}
	}
	else
	{
		if( bSrcReject )
		{
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );
			strMessage.PrintF( _S2( 156, m_strSrcName, "%s<가> 교환을 취소했습니다." ), m_strSrcName );
		}
		else
			strMessage.PrintF( _S2( 155, m_strSrcName, "%s<과>의 교환을 취소하셨습니다." ), m_strSrcName );
	}

	// Add system message
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	// Reset exchange
	ResetExchange();
}

void CExchangeUI::ExchangeReq_Ready( BOOL bSrcReady )
{
	CUIBase* pBase = NULL;
	if( ( m_bAmISrc && bSrcReady ) || ( !m_bAmISrc && !bSrcReady ) )
	{
		m_bExchReady = TRUE;
		m_pBtnReady->SetEnable( FALSE );

		SetNoExchangeImg(m_pIconsMyItems, EXCH_EXCH_SLOT_TOTAL);
	}
	else
	{
		m_bExchReadyDest = TRUE;

		SetNoExchangeImg(m_pIconsOppItems, EXCH_EXCH_SLOT_TOTAL);
	}
}

void CExchangeUI::ExchangeReq_BothReady()
{
	if (m_bExchReadyDest == FALSE)
		SetNoExchangeImg(m_pIconsOppItems, EXCH_EXCH_SLOT_TOTAL);

	if (m_bExchReady == FALSE)
		SetNoExchangeImg(m_pIconsMyItems, EXCH_EXCH_SLOT_TOTAL);

	m_bExchReady = TRUE;
	m_bExchReadyDest = TRUE;

	// Disable ready button
	m_pBtnReady->SetEnable( FALSE );

	// Enable exchange button
	m_pBtnExchange->SetEnable( TRUE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 157, "교환 준비가 완료되었습니다." ), SYSMSG_ERROR );
}

void CExchangeUI::ExchangeReq_Full( BOOL bSrcFull )
{
	CTString	strMessage;

	if( m_bAmISrc )
	{
		if( bSrcFull )
			strMessage = _S( 265, "인벤토리 공간이 부족합니다." );
		else
			strMessage.PrintF( _S( 158, "%s의 인벤토리 공간이 부족합니다." ), m_strDestName );
	}
	else
	{
		if( bSrcFull )
			strMessage.PrintF( _S( 158, "%s의 인벤토리 공간이 부족합니다." ), m_strSrcName );
		else
			strMessage = _S( 265, "인벤토리 공간이 부족합니다." );
	}

	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;

	// Enable ready button
	m_pBtnReady->SetEnable( TRUE );

	// Disable exchange button
	m_pBtnExchange->SetEnable( FALSE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
}

void CExchangeUI::ExchangeReq_Ok( BOOL bSrcOk )
{
	if( m_bAmISrc )
	{
		if( bSrcOk )
			m_pBtnExchange->SetEnable( FALSE );
	}
	else
	{
		if( !bSrcOk )
			m_pBtnExchange->SetEnable( FALSE );
	}
}

void CExchangeUI::ExchangeReq_BothOk()
{
	// Disable exchange button
	m_pBtnExchange->SetEnable( FALSE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 159, "교환이 성공적으로 이루어졌습니다." ) );

	ResetExchange();
}

void CExchangeUI::ExchangeReq_Pet_Err(BOOL petErr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Disable exchange button
	m_pBtnExchange->SetEnable( FALSE );

	if(!petErr){
		// Add system message
		pUIManager->GetChattingUI()->AddSysMessage( _S(2221, "펫을 더 이상 소유할 수 없습니다." ),SYSMSG_ERROR );
	}
	else{
		CTString	strMessage;
		strMessage.PrintF(_S(2555, "%s님은 펫을 더 이상 소유할 수 없는 상태 입니다." ) ,m_strDestName);
		pUIManager->GetChattingUI()->AddSysMessage(strMessage,SYSMSG_ERROR); 
	}
		
	ResetExchange();
}

void CExchangeUI::ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
									int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex )
{
	// My items
	int		iItem;

	if ((m_bAmISrc && bSrcAdd) || (!m_bAmISrc && !bSrcAdd))
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->IsEmpty() )
				break;
		}
		
		CItems* pItems = new CItems;
		pItems->Item_Index = nIndex;
		pItems->Item_UniIndex = nUniIndex;
		pItems->Item_Sum = llCount;
		pItems->Item_Plus = ulPlus;
		pItems->Item_Used = ulUsed;
		pItems->Item_Used2 = ulUsed2;
		pItems->Item_Flag = ulFlag;
		pItems->SetRareIndex(lRareIndex);
		pItems->ItemData = CItemData::getData(nIndex);

		m_pIconsMyItems[iItem]->setData(pItems, false);

		CUIManager* pUIManager = CUIManager::getSingleton();

		if (ulFlag & FLAG_ITEM_SEALED)
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
			m_pIconsMyItems[iItem]->clearIconData();
			return;
		}

		// Update inventory of exchange
		for (int nIdx = 0; nIdx < DEF_INVENTORY_MAX; ++nIdx)
		{
			if (m_pIconsInvenItems[nIdx]->getItems() != NULL && 
				m_pIconsInvenItems[nIdx]->getItems()->Item_UniIndex == nUniIndex)
			{
				// Set item location of my item
				pItems->Item_Tab = 0;
				pItems->InvenIndex = nIdx;

				// Update count
				SQUAD	llNewCount = m_pIconsInvenItems[nIdx]->getItems()->Item_Sum;
				llNewCount -= llCount;
				m_pIconsInvenItems[nIdx]->setCount(llNewCount);
				
				if( llNewCount <= 0 )
				{
					m_pIconsInvenItems[nIdx]->clearIconData();

					// Unselect item
					if( m_nSelInvenItemID == nIdx )
						m_nSelInvenItemID = -1;
				}

				// If item is money
				CItemData* pItemData = pItems->ItemData;

				if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
				pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,ulPlus);
				}

				return;
			}
		}
	}
	// Opposite items
	else
	{
		int		iItem;
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->IsEmpty() )
				break;
		}

		CItems* pItems = new CItems;
		pItems->Item_Index = nIndex;
		pItems->Item_UniIndex = nUniIndex;
		pItems->Item_Sum = llCount;
		pItems->Item_Plus = ulPlus;
		pItems->Item_Used = ulUsed;
		pItems->Item_Used2 = ulUsed2;
		pItems->Item_Flag = ulFlag;
		pItems->SetRareIndex(lRareIndex);
		pItems->ItemData = CItemData::getData(nIndex);

		m_pIconsOppItems[iItem]->setData(pItems, false);

		// If item is money
		CItemData* pItemData = _pNetwork->GetItemData( nIndex );

		if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
			pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(m_bAmISrc ? m_nDestIndex : m_nSrcIndex, ulPlus);
		}
	}
}

void CExchangeUI::ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
												SBYTE sbOptionType, LONG lOptionLevel, LONG lRareIndex, LONG lOriginOptionVar )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		CItems* pItems = m_pIconsMyItems[iItem]->getItems();

		pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		pItems->SetRareIndex(lRareIndex);
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		CItems* pItems = m_pIconsOppItems[iItem]->getItems();

		pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		pItems->SetRareIndex(lRareIndex);
	}
}

void CExchangeUI::ExchangeItem_Del( BOOL bSrcDel, int nUniIndex )
{
	// My items			// 아이템 삭제 막음
	/*if( ( m_bAmISrc && bSrcDel ) || ( !m_bAmISrc && !bSrcDel ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		SQUAD	llMySlotCount = m_abtnMyItems[iItem].GetItemCount();

		// Rearrange buttons
		for( int iArrItem = iItem; iArrItem < EXCH_EXCH_SLOT_TOTAL; iArrItem++ )
		{
			if( ( iArrItem + 1 ) == EXCH_EXCH_SLOT_TOTAL )
			{
				m_abtnMyItems[iArrItem].InitBtn();
				break;
			}

			if( m_abtnMyItems[iArrItem + 1].IsEmpty() )
			{
				m_abtnMyItems[iArrItem].InitBtn();
				break;
			}

			m_abtnMyItems[iArrItem].SetItemInfo( 0, 0, 0, m_abtnMyItems[iArrItem + 1].GetItemIndex(),
														m_abtnMyItems[iArrItem + 1].GetItemUniIndex(), -1 );
			m_abtnMyItems[iArrItem].SetItemCount( m_abtnMyItems[iArrItem + 1].GetItemCount() );
			m_abtnMyItems[iArrItem].SetItemPlus( m_abtnMyItems[iArrItem + 1].GetItemPlus() );
			m_abtnMyItems[iArrItem].SetItemUsed( m_abtnMyItems[iArrItem + 1].GetItemUsed() );
			m_abtnMyItems[iArrItem].SetItemFlag( m_abtnMyItems[iArrItem + 1].GetItemFlag() );
		}

		// Unselect item
		if( iItem == m_nSelMyItemID )
			m_nSelMyItemID = -1;

		// Update inventory of exchange
		for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
		{
			for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
			{
				if( m_abtnInvenItems[iRow][iCol].GetItemUniIndex() == nUniIndex )
				{
					SQUAD	llInvenCount = m_abtnInvenItems[iRow][iCol].GetItemCount() + llMySlotCount;
					m_abtnInvenItems[iRow][iCol].SetItemCount( llInvenCount );
					if( llInvenCount > 0 )
						m_abtnInvenItems[iRow][iCol].SetEmpty( FALSE );

					// If item is money
					int			nIndex = m_abtnInvenItems[iRow][iCol].GetItemIndex();
					CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
					if( pItemData->GetType() == CItemData::ITEM_ETC &&
						pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
					{
						m_strMyExchMoney.PrintF( "%d", 0 );
					}

					return;
				}
			}
		}
	}
	// Opposite items
	else
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnOppItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		// If item is money
		int			nIndex = m_abtnOppItems[iItem].GetItemIndex();
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			m_strOppExchMoney.PrintF( "%d", 0 );
		}

		m_abtnOppItems[iItem].InitBtn();
	}*/
}

void CExchangeUI::ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount,SQUAD llNas )
{
	// My items
	int		iItem;
	SQUAD	llCountDiff;

	if( ( m_bAmISrc && bSrcUpdate ) || ( !m_bAmISrc && !bSrcUpdate ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsMyItems[iItem]->IsEmpty() == false && 
				m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if (iItem < EXCH_EXCH_SLOT_TOTAL)
		{
			llCountDiff = m_pIconsMyItems[iItem]->getItems()->Item_Sum - llCount;
			m_pIconsMyItems[iItem]->setCount(llCount);
		}

		// Update inventory of exchange
		for (int nIdx = 0; nIdx < DEF_INVENTORY_MAX; ++nIdx)
		{
			if (m_pIconsInvenItems[nIdx]->getItems() == NULL)
				continue;

			if( m_pIconsInvenItems[nIdx]->getItems()->Item_UniIndex == nUniIndex )
			{
				SQUAD	llNewCount = m_pIconsInvenItems[nIdx]->getItems()->Item_Sum + llCountDiff;
				m_pIconsInvenItems[nIdx]->setCount(llNewCount);
				
				if( llNewCount <= 0 )
				{
					m_pIconsInvenItems[nIdx]->clearIconData();

					// Unselect item
					if( m_nSelInvenItemID == nIdx )
						m_nSelInvenItemID = -1;
				}

				// If item is money
				int			nIndex = m_pIconsInvenItems[nIdx]->getIndex();
				CItemData* pItemData = _pNetwork->GetItemData( nIndex );

				if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
					pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, m_pIconsMyItems[iItem]->getItems()->Item_Plus);
				}

				return;
			}
		}
	}
	// Opposite items
	else
	{
		CTString strGold;
		m_nTradeNas = llNas;
		strGold.PrintF( "%I64d", m_nTradeNas );
		UIMGR()->InsertCommaToString(strGold);

		if (m_pTxtUserGold != NULL)
		{
			m_pTxtUserGold->SetText(strGold);
			m_pTxtUserGold->setFontColor(UIMGR()->GetNasColor(strGold));
		}

		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsOppItems[iItem]->IsEmpty() == false && 
				m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if (iItem >= EXCH_EXCH_SLOT_TOTAL)
			return;
		
		SQUAD	llCountDiff = m_pIconsOppItems[iItem]->getItems()->Item_Sum - llCount;
		m_pIconsOppItems[iItem]->setCount(llCount);

		// If item is money
		int			nIndex = m_pIconsOppItems[iItem]->getIndex();
		CItemData* pItemData = _pNetwork->GetItemData( nIndex );

		if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
			pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, m_pIconsOppItems[iItem]->getItems()->Item_Plus);
		}
	}
}

void CExchangeUI::ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex)
	{
		SBYTE	sbOptionType;
		LONG	lOptionLevel;

		//레어 옵션 인덱스
		(*istr) >> sbOptionType;
		(*istr) >> lOptionLevel;
		UWORD iRareIndex = lOptionLevel;

		//레어 옵션 종류
		(*istr) >> sbOptionType;
		(*istr) >> lOptionLevel;
	
		WORD iRareOption = lOptionLevel;
		WORD wCBit =1;
		SBYTE sbOption =-1;
		for(int iBit=0; iBit<10; ++iBit)
		{
			if( iRareOption & wCBit)
			{
				CItemRareOption* pItem = CItemRareOption::getData(iRareIndex);

				if (pItem == NULL)
					continue;

				if (pItem->GetIndex() < 0)
					continue;

				int OptionType = pItem->rareOption[iBit].OptionIdx;
				int OptionLevel = pItem->rareOption[iBit].OptionLevel;

				this->ExchangeItem_SetOptionData(bSrcAdd, nUniIndex, ++sbOption, OptionType, OptionLevel, iRareIndex, ORIGIN_VAR_DEFAULT );
			}
			wCBit <<=1;
	}
}

void CExchangeUI::ExchangeItem_SetSkill( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex)
{
	SBYTE sbBelong, sbSkillcont;
	LONG lSkillIndex,lplus;
	SBYTE sbSkillLevel;

	// My items
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
			{
				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				m_pIconsMyItems[iItem]->getItems()->Item_Belong = sbBelong;

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					m_pIconsMyItems[iItem]->getItems()->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}

				(*istr) >> lplus;
				m_pIconsMyItems[iItem]->getItems()->SetItemPlus2(lplus);
				break;
			}
				
		}
	}
	// Opposite items
	else
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
			{
				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				m_pIconsOppItems[iItem]->getItems()->Item_Belong = sbBelong;

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					m_pIconsOppItems[iItem]->getItems()->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}

				(*istr) >> lplus;
				m_pIconsOppItems[iItem]->getItems()->SetItemPlus2(lplus);
				break;
			}
		}
	}

}

void CExchangeUI::ExchangeItem_InitSocket(BOOL bSrcAdd, int nUniIndex )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->InitSocketInfo();
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->InitSocketInfo();
	}
		
}

void CExchangeUI::ExchangeItem_SetSocketData( BOOL bSrcAdd, int nUniIndex, SBYTE sbSocketCreateCount,
												SBYTE sbSlotIndex, LONG lJewelIndex )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->SetSocketCount(sbSocketCreateCount);
		m_pIconsMyItems[iItem]->getItems()->SetSocketOption( sbSlotIndex, lJewelIndex );
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->SetSocketCount( sbSocketCreateCount );
		m_pIconsOppItems[iItem]->getItems()->SetSocketOption( sbSlotIndex, lJewelIndex );
	}
}

void CExchangeUI::ExchangeItem_SetPlus2( BOOL bSrcAdd, int nUniIndex, LONG plus2 )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->SetItemPlus2( plus2 );
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->SetItemPlus2( plus2 );
	}	
}

void CExchangeUI::SetDurability( BOOL bSrcAdd, int nUniIndex, LONG nDuraNow, LONG nDuraMax )
{
	// My items
	int			iItem;
	CItems*		pItems = NULL;

	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		pItems = m_pIconsMyItems[iItem]->getItems();

		if (pItems != NULL)
			pItems->SetDurability(nDuraNow, nDuraMax);

	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		pItems = m_pIconsOppItems[iItem]->getItems();

		if (pItems != NULL)
			pItems->SetDurability(nDuraNow, nDuraMax);
	}	
}

//2013/04/03 jeil 나스 아이템 제거 
//나스 교환 팝업 
void CExchangeUI::ExchangeNas()
{
	if (m_bExchReady == TRUE)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_ADD_NAS))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_EXCHANGE, MSGCMD_EXCH_ADD_NAS );
	strMessage.PrintF( _S( 5903, "몇 개의 나스를 교환 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;

}

void CExchangeUI::AddItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		SendExchangeItem_Add( nTempUniIndex, UIMGR()->GetMsgBoxNumOnly()->GetData());
}

void CExchangeUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	int i;
	for (i = 0; i < EXCH_EXCH_SLOT_TOTAL; ++i)
	{
		if (m_pIconsOppItems[i]->IsEmpty() == false)
			m_pIconsOppItems[i]->Update(fDeltaTime, ElapsedTime);
		if (m_pIconsMyItems[i]->IsEmpty() == false)
			m_pIconsMyItems[i]->Update(fDeltaTime, ElapsedTime);
	}

	for (i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		if (m_pIconsInvenItems[i]->IsEmpty() == false)
			m_pIconsInvenItems[i]->Update(fDeltaTime, ElapsedTime);
	}
}

void CExchangeUI::DragItem()
{
	CUIIcon* pIcon = UIMGR()->GetDragIcon();
	if (pIcon == NULL)
		return;

	CItems* pItem = pIcon->getItems();

	if (pItem == NULL)
		return;

	AddExchItem(pItem->Item_Tab
			  , pItem->InvenIndex
			  , pItem->Item_UniIndex
			  , pItem->Item_Sum);
}

WMSG_RESULT CExchangeUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (CUIWindow::OnLButtonUp(x, y) == WMSG_OB)
		return WMSG_OB;

	if (m_pArrMy != NULL && m_pArrMy->IsInside(x, y) == TRUE)
		DragItem();

	UIMGR()->ResetHoldBtn();

	return WMSG_FAIL;
}

void CExchangeUI::SetNoExchangeImg( CUIIcon** ptr, int nCnt )
{
	CUIBase* pBase = NULL;
	for (int i = 0; i < nCnt; ++i)
	{
		if (ptr[i] == NULL || ptr[i]->IsEmpty() == true)
			continue;

		pBase = ptr[i]->findUI("img_no_exchange");

		if (pBase == NULL)
		{
			pBase = m_pImgNoExchange->Clone();
			ptr[i]->addChild(pBase);
		}

		pBase->Hide(FALSE);
	}
}
