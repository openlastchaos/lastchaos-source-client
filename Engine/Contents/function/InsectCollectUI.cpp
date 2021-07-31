
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include "InsectCollectUI.h"

class CmdInsectCollectDrop : public Command
{
public:
	CmdInsectCollectDrop() : m_pWnd(NULL), m_pIcon(NULL)	{}
	void setData(CInsectCollectUI* pWnd, CUIIcon* pIcon)
	{
		m_pIcon = pIcon;
		m_pWnd = pWnd;
	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->DropInsectItem(m_pIcon);
	}
private:
	CInsectCollectUI* m_pWnd;
	CUIIcon* m_pIcon;
};

class CmdInsectCollectLDown : public Command
{
public:
	CmdInsectCollectLDown() : m_pWnd(NULL), m_nPos(-1)	{}
	void setData(CInsectCollectUI* pWnd, int nPos)	{ m_pWnd = pWnd; m_nPos = nPos;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->DropItemPos(m_nPos);
		}
	}
private:
	CInsectCollectUI* m_pWnd;
	int m_nPos;
};
CInsectCollectUI::CInsectCollectUI()
	: m_pArr(NULL)
	, m_pTextPoint(NULL)
{
	setInherit(false);
}

CInsectCollectUI::~CInsectCollectUI()
{

}

void CInsectCollectUI::initialize()
{
#ifndef		WORLD_EDITOR

	CUIBase* pTemp = NULL;

	if (pTemp = findUI("btn_close"))
		pTemp->SetCommandFUp(boost::bind(&CInsectCollectUI::CloseUI, this));

	if (pTemp = findUI("text_title"))
	{
		int l = pTemp->GetPosX();
		int t = pTemp->GetPosY();
		int r = l + pTemp->GetWidth();
		int b = t + pTemp->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pArr = (CUIArray*)findUI("array_item");
	m_pTextPoint = (CUIText*)findUI("text_point");

#endif		// WORLD_EDITOR
}

void CInsectCollectUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CInsectCollectUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );	
}

void CInsectCollectUI::OpenUI( int nTab, int inven_idx )
{
	if( IsVisible() || GetHide() == FALSE )	return;

	Init();

	m_nTab = nTab;
	m_nInvenIdx = inven_idx;

	UpDateItem();

	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder( UI_INSECTCOLLECT, TRUE );
}

void CInsectCollectUI::CloseUI()
{
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_INSECTCOLLECT, FALSE );
}

void CInsectCollectUI::Init( void )
{
	m_nDropPosition = -1;
	m_nTab = -1;
	m_nInvenIdx = -1;

	if (m_pArr == NULL)
		return;

	int i, max = m_pArr->GetArrayChildCount();
	CUIBase* pArrItem = NULL;
	CUIIcon* pIcon = NULL;

	for (i = 0; i < max; ++i)
	{
		pArrItem = m_pArr->GetArrayItem(i);

		if (pArrItem == NULL)
			continue;

		pIcon = (CUIIcon*)pArrItem->findUI("icon_item");

		if (pIcon == NULL)
			continue;

		pIcon->clearIconData();
	}
}

void CInsectCollectUI::SetBtnItem( int num, int nIndex )
{
	if (num < 1 || num > 11) 
		return; 

	if (num == 11) 
		num = 8;

	if (m_pArr == NULL)
		return;

	int nItemIndex = 1578;

	nItemIndex += num;
	// ( 1579 ~ 1588 );
	CItemData* pItemData = _pNetwork->GetItemData( nItemIndex );

	CUIBase* pArrItem = m_pArr->GetArrayItem(nIndex);

	if (pArrItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pArrItem->findUI("icon_item");

	if (pIcon == NULL)
		return;

	pIcon->setData(UBET_ITEM, nItemIndex);
	pIcon->SetWhichUI(UI_INSECTCOLLECT);
	CmdDragIcon* pCmd = new CmdDragIcon;
	pCmd->setData(pIcon);
	pIcon->SetCommandDrag(pCmd);

	CmdInsectCollectDrop* pCmdDBL = new CmdInsectCollectDrop;
	pCmdDBL->setData(this, pIcon);
	pIcon->SetCommandDBL(pCmdDBL);

	CmdInsectCollectLDown* pCmdDown = new CmdInsectCollectLDown;
	pCmdDown->setData(this, nIndex);
	pIcon->SetCommand(pCmdDown);
}

void CInsectCollectUI::DropInsectItem( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return;

	CItems* pItem = pIcon->getItems();

	if (pItem == NULL)
		return;

	CItemData* pID = pItem->ItemData;

	if (pID == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(_pNetwork->m_bSingleMode)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 529, "아이템을 버릴 수 없는 지역입니다." ), SYSMSG_ERROR );
		return;
	}

	if( pUIManager->DoesMessageBoxExist( MSGCMD_INSECTITEM_DROP ) )
	{
		return;
	}
	
	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2936, "곤충 채집 상자" ), UMBS_OKCANCEL, UI_INSECTCOLLECT, MSGCMD_INSECTITEM_DROP );
	strMessage.PrintF( _S( 2937, "%s<를> 버리면 파괴 됩니다. 계속 하시겠습니까?" ), pID->GetName() );
	MsgBoxInfo.AddString( strMessage );

	pUIManager->CreateMessageBox( MsgBoxInfo );	
}

//============================================================================================================
// CUICollectBox::MsgBoxCommand
// Desc:
//============================================================================================================
void CInsectCollectUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_INSECTITEM_DROP:
		if( bOK )
		{
			if (m_nDropPosition < 0)
				return;

			_pNetwork->SendDropInsect( m_nDropPosition
				, UIMGR()->GetInventory()->GetItemUniIndex(m_nTab, m_nInvenIdx));
		}
		break;
	}
}

//============================================================================================================
// CUICollectBox::GetInsectName
// Desc :
//============================================================================================================
CTString CInsectCollectUI::GetInsectName( int n )
{
	CItemData* pItemData = _pNetwork->GetItemData( n );

	CTString strMessage;
	strMessage.PrintF( _S( 2938, "%s를(을) 채집하였습니다." ), pItemData->GetName() );

	return strMessage;
}

//============================================================================================================
// CUICollectBox::UpDateItem
// Desc :
//============================================================================================================
void CInsectCollectUI::UpDateItem( void )
{
	CItems*		pItems = &_pNetwork->MySlotItem[m_nTab][m_nInvenIdx];

	if (m_pTextPoint != NULL)
	{
		CTString strPoint;
		strPoint.PrintF( _S( 2939, "합계: %d 포인트" ), pItems->Item_Flag );
		m_pTextPoint->SetText(strPoint);
	}

	ULONG ulTemp;
	ULONG ulHiBit = pItems->Item_Plus;
	ULONG ulLowBit = pItems->Item_Used;

	ulTemp = ( ulHiBit & 0xF0000000 ) >> 28;
	SetBtnItem( ulTemp, 0 );
	ulTemp = ( ulHiBit & 0x0F000000 ) >> 24;
	SetBtnItem( ulTemp, 1 );
	ulTemp = ( ulHiBit & 0x00F00000 ) >> 20;
	SetBtnItem( ulTemp, 2 );
	ulTemp = ( ulHiBit & 0x000F0000 ) >> 16;
	SetBtnItem( ulTemp, 3 );
	ulTemp = ( ulHiBit & 0x0000F000 ) >> 12;
	SetBtnItem( ulTemp, 4 );
	ulTemp = ( ulHiBit & 0x00000F00 ) >> 8;
	SetBtnItem( ulTemp, 5 );
	ulTemp = ( ulHiBit & 0x000000F0 ) >> 4;
	SetBtnItem( ulTemp, 6 );
	ulTemp = ( ulHiBit & 0x0000000F );
	SetBtnItem( ulTemp, 7 );

	ulTemp = ( ulLowBit & 0xF0000000 ) >> 28;
	SetBtnItem ( ulTemp , 8 );
	ulTemp = ( ulLowBit & 0x0F000000 ) >> 24;
	SetBtnItem ( ulTemp , 9 );
	ulTemp = ( ulLowBit & 0x00F00000 ) >> 20;
	SetBtnItem ( ulTemp , 10 );
	ulTemp = ( ulLowBit & 0x000F0000 ) >> 16;
	SetBtnItem ( ulTemp , 11 );
	ulTemp = ( ulLowBit & 0x0000F000 ) >> 12;
	SetBtnItem ( ulTemp , 12 );
	ulTemp = ( ulLowBit & 0x00000F00 ) >> 8;
	SetBtnItem ( ulTemp , 13 );
	ulTemp = ( ulLowBit & 0x000000F0 ) >> 4;
	SetBtnItem ( ulTemp , 14 );
	ulTemp = ( ulLowBit & 0x0000000F );
	SetBtnItem ( ulTemp , 15 );	
}

void CInsectCollectUI::DeleteItem()
{
	if (m_pArr == NULL)
		return;

	CUIBase* pArrItem = m_pArr->GetArrayItem(m_nDropPosition);

	if (pArrItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pArrItem->findUI("icon_item");

	if (pIcon == NULL)
		return;

	pIcon->clearIconData();
	m_nDropPosition = -1;
}

void CInsectCollectUI::DropItemPos( int nPos )
{
	if (UIMGR()->DoesMessageBoxExist(MSGCMD_INSECTITEM_DROP))
		return;

	m_nDropPosition = nPos;
}
