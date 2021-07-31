#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/function/CompoundUI.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>

#define ITEM_ARCANE_MATERIAL_UPGRADE		0 // 비밀의 상자 재료 0번 제련석 
#define ITEM_ARCANE_MATERIAL_ACCESSORY		1 // 비밀의 상자 재료 1번 악세사리 
#define ITEM_ARCANE_MATERIAL_SAMPLE			2 // 비밀의 상자 재료 2번 시료 

static int	_iMaxMsgStringChar = 0;

extern INDEX g_iCountry; 

class CmdCompoundRemoveItem : public Command
{
public:
	CmdCompoundRemoveItem()	: m_pWnd(NULL), m_nIndex(-1)	{}
	void setData(CCompoundUI* pWnd,	int nIndex)	{ m_pWnd = pWnd;	m_nIndex = nIndex;	}
	void execute()
	{
		if (m_pWnd != NULL && m_nIndex >= 0)
			m_pWnd->RemoveItem(m_nIndex);
	}

private:
	CCompoundUI* m_pWnd;
	int m_nIndex;
};

CCompoundUI::CCompoundUI()
	: m_pBtnOk(NULL)
{
	setInherit(false);

	m_bWaitResult		= TRUE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;

	m_dwWndState		= UWS_ENABLE | UWS_VISIBLE;

	m_nTextItemIndex	= -1;						
	m_nTab				= -1;
	m_nInvenIdx			= -1;

	for (int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; ++i)
		m_pIcon[i] = NULL;
}

CCompoundUI::~CCompoundUI()
{
}

void CCompoundUI::initialize()
{
#ifndef		WORLD_EDITOR

	m_pBtnOk = (CUIButton*)findUI("btn_ok");

	if (m_pBtnOk != NULL)
		m_pBtnOk->SetCommandFUp(boost::bind(&CCompoundUI::SendCompoundReq, this));

	CUIBase* pBase = NULL;

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CCompoundUI::CloseCompound, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CCompoundUI::CloseCompound, this));

	if (pBase = findUI("text_title"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	CTString strItem[COMPOUND_ITEM_SLOT_COUNT] = {"ref", "acc", "sug"};

	for (int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; ++i)
	{
		m_pIcon[i] = (CUIIcon*)findUI(CTString("icon_" + strItem[i]).str_String);

		if (m_pIcon[i] == NULL)
			continue;

		m_pIcon[i]->clearIconData();

		CmdCompoundRemoveItem* pCmdD = new CmdCompoundRemoveItem;
		pCmdD->setData(this, i);
		m_pIcon[i]->SetCommandDBL(pCmdD);

		CmdDragIcon* pCmdDrag = new CmdDragIcon;
		pCmdDrag->setData(m_pIcon[i]);
		m_pIcon[i]->SetCommandDrag(pCmdDrag);
	}

	SetCommandFUp(boost::bind(&CCompoundUI::SetCompoundItem, this));

#endif		// WORLD_EDITOR
}

void CCompoundUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CCompoundUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CCompoundUI::OpenCompound ( int nItemIndex, SWORD nTab, SWORD inven_idx )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_COMPOUND_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_nTextItemIndex	= nItemIndex;						
	m_nTab				= nTab;
	m_nInvenIdx			= inven_idx;

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	pUIManager->RearrangeOrder( UI_COMPOUND, TRUE );
	Hide(FALSE);

	m_bWaitResult		= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	
	setBtnEnable(false);
}

void CCompoundUI::CloseCompound()
{
	// Reset slot item
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIcon[i] == NULL)
			continue;

		m_pIcon[i]->clearIconData();
	}

	m_nTextItemIndex	= -1;						
	m_nTab				= -1;
	m_nInvenIdx			= -1;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close Compound
	pUIManager->RearrangeOrder( UI_COMPOUND, FALSE );
	Hide(TRUE);

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_COMPOUND );

	m_bWaitResult = FALSE;
}

void CCompoundUI::SetCompoundItem ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	if (pDrag->getBtnType() != UBET_ITEM &&
		pDrag->GetWhichUI() != UI_INVENTORY)
	{
		pUIManager->ResetHoldBtn();
		return;
	}

	if (pDrag->GetWhichUI() == UI_COMPOUND)
	{
		RemoveItem(pDrag);
		pUIManager->ResetHoldBtn();
		return;
	}

	// If this is wearing item
	if (pDrag->IsWearTab() == true)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 728, "착용한 아이템은 합성할 수 없습니다." ), SYSMSG_ERROR );	
		pUIManager->ResetHoldBtn();
		return;
	}

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;
	
	ResetCompoundItem();	// 버튼이 들려 있으면 삭제 하고
	
	// 아이템이 현재 사용중인지 확인 한다.
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIcon[i] == NULL)
			return;

		if (m_pIcon[i]->IsEmpty() == false)
		{
			if (pItems->Item_UniIndex == m_pIcon[i]->getItems()->Item_UniIndex)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 556, "이미 사용중인 아이템 입니다." ), SYSMSG_ERROR );	
				pUIManager->ResetHoldBtn();
				return;
			}
		}
	}

	int nSlotIndex = 0;

	// 해당 아이템의 위치를 찾아 준다.
	if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::ITEM_ETC_REFINE &&
		pItemData->GetRefineType() == CItemData::REFINE_GENERAL )  // 일반제련석
	{
		nSlotIndex = ITEM_ARCANE_MATERIAL_UPGRADE;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ETC && 
			pItemData->GetSubType() == CItemData::ITEM_ETC_SAMPLE ) // 시료
	{
		nSlotIndex = ITEM_ARCANE_MATERIAL_SAMPLE;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ACCESSORY ) // 악세서리
	{
		nSlotIndex = ITEM_ARCANE_MATERIAL_ACCESSORY;
	}
	else
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 729, "합성 할 수 있는 아이템이 아닙니다." ), SYSMSG_ERROR ); 
		pUIManager->ResetHoldBtn();
		return;
	}
	
	// 사용중이 아니라면 입력 해 준다.
	if (m_pIcon[nSlotIndex]->IsEmpty() == true)
	{
		m_nCurItemCount++;	
	}

	CItems* pClone = new CItems;
	memcpy(pClone, pItems, sizeof(CItems));
	pClone->Item_Sum = 1;

	m_pIcon[nSlotIndex]->setData(pClone, false);
	m_pIcon[nSlotIndex]->SetWhichUI(UI_COMPOUND);
	
	if ( m_nCurItemCount >= COMPOUND_ITEM_SLOT_COUNT )
		setBtnEnable(true);
	else 
		setBtnEnable(false);
	
	pUIManager->ResetHoldBtn();

	// Lock arrange of inventory
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_COMPOUND );
}

void CCompoundUI::ResetCompoundItem ()
{
	if ( m_nCurItemSlot != -1 )
	{
		if (m_pIcon[m_nCurItemSlot] == NULL)
			return;

		m_pIcon[m_nCurItemSlot]->clearIconData();
		m_nCurItemSlot = -1;
		m_nCurItemCount--;
		setBtnEnable(false);
	}
}

void CCompoundUI::SendCompoundReq()
{
	if( m_bWaitResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 868, "스킬 사용중에는 힘의 상자를 합성할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 869, "순간 이동중에는 힘의 상자를 합성할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SWORD	arrTab[COMPOUND_ITEM_SLOT_COUNT];
	SWORD	nIdx[COMPOUND_ITEM_SLOT_COUNT];
	
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIcon[i] == NULL)
			continue;

		arrTab[i] = m_pIcon[i]->getItems()->Item_Tab;
		nIdx[i] = m_pIcon[i]->getItems()->InvenIndex;
	}

	_pNetwork->CompoundReq( m_nTab, m_nInvenIdx, arrTab, nIdx );
	setBtnEnable(false);
	m_bWaitResult = TRUE;

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_COMPOUND );
}

void CCompoundUI::CompoundRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_COMPOUND_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_ARCANE_FAIL_SYSTEM: 			// 있어서는 안되는 오류
		strMessage = _S( 730, "합성에 실패하였습니다." );
		break;
	case MSG_ARCANE_FAIL_ARCANEBOX:			// 힘의상자 정보 오류
		strMessage = _S( 731, "힘의상자 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_MATERIAL:			// 재료 정보 오류 (재료가 위치에 없거나 착용중)
		strMessage = _S( 732, "합성할 아이템이 작용중이거나 아이템 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_UPGRADE:			// 제련석 오류
		strMessage = _S( 733, "일반제련석의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_ACCESSORY:			// 악세사리 오류
		strMessage = _S( 734, "악세사리의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_SAMPLE:			// 시료 오류
		strMessage = _S( 735, "마법가루의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_SUCCESS:		
		strMessage = _S( 736, "합성에 성공하였습니다." );
		break;
	}

	CloseCompound();
	
	m_bWaitResult = FALSE;

//	if ( strMessage.Length() )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 723, "힘의 상자" ), UMBS_OK, UI_COMPOUND, MSGCMD_COMPOUND_REP ); 
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}	
}

void CCompoundUI::RemoveItem( int nIndex )
{
	if (nIndex < 0)
		return;

	m_nCurItemSlot = nIndex;
	ResetCompoundItem();
}

void CCompoundUI::RemoveItem( CUIIcon* pIcon )
{
	if (pIcon == NULL || pIcon->IsEmpty() == true)
		return;

	for (int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; ++i)
	{
		if (m_pIcon[i] == NULL || m_pIcon[i]->IsEmpty() == true)
			continue;

		if (m_pIcon[i]->getItems()->Item_Index == pIcon->getItems()->Item_Index)
		{
			RemoveItem(i);
			break;
		}
	}
}

WMSG_RESULT CCompoundUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (GetHide() == TRUE || IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->RearrangeOrder(UI_COMPOUND, TRUE);

	return CUIWindow::OnLButtonDown(x, y);
}
