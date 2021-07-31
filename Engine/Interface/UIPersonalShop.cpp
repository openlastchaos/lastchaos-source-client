#include "stdh.h"
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIFiltering.h>

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Classes/PlayerEntity.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Entities/Items.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/PetInfo.h>
#include <Engine/LocalDefine.h>

#define PACKAGE_ITEM_INDEX		(506)

#define PERSONAL_SELL_TOP_SLOT_SX			11		// Sell Mode(Top)
#define PERSONAL_SELL_TOP_SLOT_SY			47
#define PERSONAL_SELL_BOTTOM_SLOT_SX		11		// (Bottom)
#define PERSONAL_SELL_BOTTOM_SLOT_SY		217		// 238
#define PERSONAL_SELL_PACKAGE_SLOT_SX		11		// (Package)
#define PERSONAL_SELL_PACKAGE_SLOT_SY		146		// 167

extern INDEX g_iCountry;

// ----------------------------------------------------------------------------
// Name : CUIPersonalShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPersonalShop::CUIPersonalShop()
{
	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPackageBuy		= FALSE;
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelPackageItemID = -1;
	m_nSelectedChaID	= -1;	
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_bShowItemInfo		= FALSE;
	m_bDetailItemInfo	= FALSE;
	m_bPremium			= FALSE;
	m_nCurInfoLines		= 0;
	m_strPlayerMoney	= CTString( "0" );
	m_strTotalPrice		= CTString( "0" );
	m_strPackagePrice	= CTString( "0" );
	m_llPackagePrice	= 0;
	m_bCashPersonShop	= FALSE;
	m_bCashPersonShop_open = FALSE;
	memset(&m_aiTradeItemCount, 0, sizeof(m_aiTradeItemCount));
}

// ----------------------------------------------------------------------------
// Name : ~CUIPersonalShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPersonalShop::~CUIPersonalShop()
{
	if(!m_vectorSellItemList.empty())
	{
		m_vectorSellItemList.clear();
	}
	if(!m_vectorSellPackageList.empty())
	{
		m_vectorSellPackageList.clear();
	}
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	//m_nBackSplitHeight = 50;
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	// Inventory region
	m_rcTop.SetRect( 4, 214, 190, 365 );
	m_rcMiddle.SetRect( 4, 144, 190, 182 );
	m_rcBottom.SetRect( 4, 48, 190, 115 );

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 29, 216, 62, fTexWidth, fTexHeight );	
	m_rtBackSellShop.SetUV( 0, 64, 216, 96, fTexWidth, fTexHeight );	

	// Inventory
	m_rtTopInven.SetUV( 0, 131, 216, 282, fTexWidth, fTexHeight );
	//m_rtBottomInven.SetUV( 0, 283, 216, 381, fTexWidth, fTexHeight );	
	m_rtBottomInven.SetUV( 0, 283, 216, 360, fTexWidth, fTexHeight );	
	m_rtMiddleInven.SetUV( 0, 387, 216, 451, fTexWidth, fTexHeight );	
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );
	m_rtTitleInven.SetUV( 0, 393, 216, 411, fTexWidth, fTexHeight );
	m_rtNameInven.SetUV( 0, 98, 216, 130, fTexWidth, fTexHeight );
	m_rtPackageBar.SetUV( 0, 30, 216, 60, fTexWidth, fTexHeight );
	m_rtBlankBar.SetUV( 0, 387, 216, 393, fTexWidth, fTexHeight );
	m_rtPriceBar.SetUV( 0, 361, 216, 380, fTexWidth, fTexHeight );

	// Outline of unmovable item
	m_rtUnmovableOutline.SetUV( 218, 86, 250, 118, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 51, 250, 83, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoUL.SetUV( 0, 476, 7, 483, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 10, 476, 12, 483, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 15, 476, 22, 483, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 486, 7, 488, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 10, 486, 12, 488, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 15, 486, 22, 488, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 491, 7, 498, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 10, 491, 12, 498, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 15, 491, 22, 498, fTexWidth, fTexHeight );

	// Lease Item UV
	m_rtleaseMark.SetUV(216,148,231,163,fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buy button of shop
	m_btnShopBuy.Create( this, _S( 253, "구매" ), 105, 419, 51, 21 );
	m_btnShopBuy.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	m_btnShopCancel.Create( this, _S( 139, "취소" ), 159, 419, 51, 21 );
	m_btnShopCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Start button of shop
	m_btnShopStart.Create( this, _S( 742, "판매개시" ), 148, 419, 62, 21 );		
	m_btnShopStart.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnShopStart.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnShopStart.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopStart.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Stop button of shop
	m_btnShopStop.Create( this, _S( 743, "판매중지" ), 148, 419, 62, 21 );		
	m_btnShopStop.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnShopStop.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnShopStop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopStop.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// Package OK of shop
	m_btnPackageOK.Create( this, _S( 191, "확인" ), 105, 186, 51, 21 );		
	m_btnPackageOK.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnPackageOK.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnPackageOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPackageOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnPackageOK.SetEnable( m_bPremium );

	// Package Cancel of shop
	m_btnPackageCancel.Create( this, _S( 139, "취소" ), 159, 186, 51, 21 );		
	m_btnPackageCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnPackageCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnPackageCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPackageCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnPackageCancel.SetEnable( m_bPremium );

	// Premium check button
	int	nStrWidth = ( strlen( _S( 744, "프리미엄" ) ) + 1 ) *				
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	m_cbtnPremium.Create( this, 193, 128, 11, 11, _S( 744, "프리미엄" ), TRUE, nStrWidth, nStrWidth );		
	m_cbtnPremium.SetUV( UCBS_NONE, 230, 119, 241, 130, fTexWidth, fTexHeight );
	m_cbtnPremium.SetUV( UCBS_CHECK, 218, 119, 229, 130, fTexWidth, fTexHeight );	
	m_cbtnPremium.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnPremium.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnPremium.SetCheck( m_bPremium );
	//m_cbtnPremium.SetEnable( m_bPremium );
	//m_cbtnPremium.SetVisible( m_bPremium );

	// Input Box for Package Price
	m_ebPackagePrice.Create( this, 26, 191, 72, 13, 12 );
	m_ebPackagePrice.SetFocus( FALSE );
	m_ebPackagePrice.SetReadingWindowUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	m_ebPackagePrice.SetCandidateUV( 218, 132, 235, 148, fTexWidth, fTexHeight );

	// Input Box for Shop Name
	m_ebShopName.Create( this, 11, 388, 195, 13, 80 );
	m_ebShopName.SetFocus( FALSE );
	m_ebShopName.SetReadingWindowUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	m_ebShopName.SetCandidateUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	
	// Bottom scroll bar
	m_sbBottomScrollBar.Create( this, 194, 218, 9, 137 );
	m_sbBottomScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10);
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbBottomScrollBar.SetScrollRange( PERSONAL_SHOP_SLOT_ROW_TOTAL );
	m_sbBottomScrollBar.SetCurItemCount( PERSONAL_SHOP_SLOT_ROW_TOTAL );
	m_sbBottomScrollBar.SetItemsPerPage( PERSONAL_SHOP_SLOT_ROW );
	// Up button
	m_sbBottomScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbBottomScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbBottomScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbBottomScrollBar.SetWheelRect( -192, -6, 191, 148 );

	// Slot items
	// Shop Slot(5x4)
	for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW_TOTAL; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			m_abtnShopItems[iRow][iCol].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM, 0, iRow, iCol );
		}
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		m_abtnTradeItems[iItem].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM );

	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		m_abtnPackageItems[iCol].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM );
	}	

	if(!m_vectorSellItemList.empty())
	{
		m_vectorSellItemList.clear();
	}
	if(!m_vectorSellPackageList.empty())
	{
		m_vectorSellPackageList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIPersonalShop::SetFocus
// Explain:  
// Date : 2005-03-10(오후 1:03:58) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPersonalShop::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
	m_ebShopName.SetFocus( bVisible );

	if( !bVisible )
	{
		m_ebPackagePrice.SetFocus( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : OpenPersonalShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::OpenPersonalShop( BOOL bSellShoop )
{
	if( _pUIMgr->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		// FIXME : 코드가 중복되는 곳이 너무 많음.
		if(!m_bBuyShop && m_bShopStart)
		{
			//_pUIMgr->GetInventory()->Lock( m_bShopStart );

			//BOOL	bVisible = !IsVisible();
			//_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, bVisible );
			_pUIMgr->GetChatting()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
			return;
		}
		else
		{
			ResetShop();
			_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		return;
	}

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
		return;
	}

	m_btnShopCancel.SetVisible(FALSE);
	m_btnShopCancel.SetEnable(FALSE);
	m_btnShopBuy.SetVisible(FALSE);
	m_btnShopBuy.SetEnable(FALSE);
	m_cbtnPremium.SetEnable( TRUE );
	//m_btnPackageOK.SetEnable( FALSE );
	//m_btnPackageCancel.SetEnable( FALSE );
	m_ebPackagePrice.SetEnable( m_bPremium );

	// 판매 상점 창의 경우 초기화 될 필요 없음.
	//ResetShop();
	
	m_nSelectedChaID	= -1;

	_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareSellShop();
}

// ----------------------------------------------------------------------------
// Name : TradePersonalShop()
// Desc : 개인 상점에서 아이템 사기 & 팔기
// ----------------------------------------------------------------------------
void CUIPersonalShop::TradePersonalShop( INDEX iChaIndex, FLOAT fX, FLOAT fZ, BOOL bBuy )
{	
	// FIXME : 중복코드 제거
	if( _pUIMgr->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		return;
	}

	if( (m_nSelectedChaID == iChaIndex) && IsVisible() )
	{
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 746, "이동중에는 개임상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(_pUIMgr->IsCSFlagOn( CSF_TELEPORT ))
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	_pUIMgr->SetCSFlagOn( CSF_PERSONALSHOP );
	//m_bBuyShop = TRUE;

	ResetShop(TRUE);

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	if(_pNetwork->_TargetInfo.dbIdx == 482)
	{
		m_bCashPersonShop_open = TRUE;
		SendCashPersonOpenShop( iChaIndex );
	}else
	{
		SendPersonalOpenShop( iChaIndex );
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::PrepareBuyShop()
{
	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP );

	m_bBuyShop			= TRUE;
	RefreshUserItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strPlayerMoney );
	}

	// Set money
	if( m_llPackagePrice > 0 )
	{
		m_strPackagePrice.PrintF( "%I64d", m_llPackagePrice );
		_pUIMgr->InsertCommaToString( m_strPackagePrice );
	}

	// Set position of trade items
	int nX = PERSONAL_SELL_TOP_SLOT_SX;
	int nY = PERSONAL_SELL_TOP_SLOT_SY;
	for( int nItem = 0; nItem < PERSONAL_TRADE_SLOT_TOTAL; ++nItem )
	{
		if( nItem == PERSONAL_TRADE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	// Set position of trade items
	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
	nY = PERSONAL_SELL_PACKAGE_SLOT_SY;
	for( nItem = 0; nItem < PERSONAL_PACKAGE_SLOT_COL; ++nItem )
	{
		if( nItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
		}
		m_abtnPackageItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::PrepareSellShop()
{
	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP );

	m_bBuyShop			= FALSE;
	RefreshPlayerItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strPlayerMoney );
	}

	// Set money
	if( m_llPackagePrice > 0 )
	{
		m_strPackagePrice.PrintF( "%I64d", m_llPackagePrice );
		_pUIMgr->InsertCommaToString( m_strPackagePrice );
	}

	// Set position of trade items
	int nX = PERSONAL_SELL_TOP_SLOT_SX;
	int nY = PERSONAL_SELL_TOP_SLOT_SY;
	for( int nItem = 0; nItem < PERSONAL_TRADE_SLOT_TOTAL; ++nItem )
	{
		if( nItem == PERSONAL_TRADE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	// Set position of trade items
	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
	nY = PERSONAL_SELL_PACKAGE_SLOT_SY;
	for( nItem = 0; nItem < PERSONAL_PACKAGE_SLOT_COL; ++nItem )
	{
		if( nItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
		}
		m_abtnPackageItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::RefreshPlayerItem()
{
	int		iRow, iCol;
	for( iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW_TOTAL; ++iRow )
	{
		for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{
			CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			if( rItems.Item_Sum > 0 )
			{
				m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, rItems.Item_Index,
															rItems.Item_UniIndex, rItems.Item_Wearing );
				m_abtnShopItems[iRow][iCol].SetItemPlus( rItems.Item_Plus );
				m_abtnShopItems[iRow][iCol].SetItemCount( rItems.Item_Sum );
				m_abtnShopItems[iRow][iCol].SetItemFlag( rItems.Item_Flag );
				m_abtnShopItems[iRow][iCol].SetItemUsed( rItems.Item_Used );
				m_abtnShopItems[iRow][iCol].SetItemUsed2( rItems.Item_Used2 );
				m_abtnShopItems[iRow][iCol].SetItemRareIndex( rItems.Item_RareIndex );

				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					m_abtnShopItems[iRow][iCol].SetItemOptionData( sbOption,
																	rItems.GetOptionType( sbOption ),
																	rItems.GetOptionLevel( sbOption ) );
				}

				if(rItems.ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
					rItems.ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, rItems.Item_Plus);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::RefreshUserItem()
{
	int iPos = 0;
	std::vector<CItems>::const_iterator end = m_vectorSellItemList.end();
	std::vector<CItems>::iterator pos;
	for(pos = m_vectorSellItemList.begin(); pos != end; ++pos, ++iPos)
	{		
		if( (*pos).Item_Sum > 0 )
		{
			const int iRow = iPos / PERSONAL_TRADE_SLOT_COL;
			const int iCol = iPos % PERSONAL_TRADE_SLOT_COL;

			m_abtnTradeItems[iPos].SetItemInfo( 0, iRow, iCol, (*pos).Item_Index,
														(*pos).Item_UniIndex, (*pos).Item_Wearing );
			m_abtnTradeItems[iPos].SetItemPlus( (*pos).Item_Plus );
			m_abtnTradeItems[iPos].SetItemCount( (*pos).Item_Sum );
			m_abtnTradeItems[iPos].SetItemPrice( (*pos).Item_Price );
			m_abtnTradeItems[iPos].SetItemFlag( (*pos).Item_Flag );
			m_abtnTradeItems[iPos].SetItemUsed( (*pos).Item_Used );
			m_abtnTradeItems[iPos].SetItemUsed2( (*pos).Item_Used2 );
			m_abtnTradeItems[iPos].SetItemRareIndex( (*pos).Item_RareIndex);

			m_aiTradeItemCount[iPos] = (*pos).Item_Sum;

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{
				m_abtnTradeItems[iPos].SetItemOptionData( sbOption,
																(*pos).GetOptionType( sbOption ),
																(*pos).GetOptionLevel( sbOption ) );
			}
		}
	}

	{
		int iPos = 0;
		std::vector<CItems>::const_iterator end = m_vectorSellPackageList.end();
		std::vector<CItems>::iterator pos;
		for(pos = m_vectorSellPackageList.begin(); pos != end; ++pos, ++iPos)
		{		
			if( (*pos).Item_Sum > 0 )
			{
				const int iRow = iPos / PERSONAL_PACKAGE_SLOT_COL;
				const int iCol = iPos % PERSONAL_PACKAGE_SLOT_COL;

				m_abtnPackageItems[iPos].SetItemInfo( 0, iRow, iCol, (*pos).Item_Index,
					(*pos).Item_UniIndex, (*pos).Item_Wearing );
				m_abtnPackageItems[iPos].SetItemPlus( (*pos).Item_Plus );
				m_abtnPackageItems[iPos].SetItemCount( (*pos).Item_Sum );
				m_abtnPackageItems[iPos].SetItemFlag( (*pos).Item_Flag );
				m_abtnPackageItems[iPos].SetItemUsed( (*pos).Item_Used );
				m_abtnPackageItems[iPos].SetItemUsed2( (*pos).Item_Used2 );				
				m_abtnPackageItems[iPos].SetItemRareIndex( (*pos).Item_RareIndex );
				
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					m_abtnPackageItems[iPos].SetItemOptionData( sbOption,
															(*pos).GetOptionType( sbOption ),
															(*pos).GetOptionLevel( sbOption ) );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ResetShop(BOOL bOpen)
{
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelPackageItemID	= -1;
	m_nSelectedChaID	= -1;
	m_strTotalPrice		= CTString( "0" );
	m_strPlayerMoney	= CTString( "0" );
	m_strPackagePrice	= CTString( "0" );
	m_nTotalPrice		= 0;
	m_nNumOfItem		= 0;
	m_llPackagePrice	= 0;
	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPremium			= FALSE;
	m_bPackageBuy		= FALSE;
	m_ebPackagePrice.ResetString();
	m_ebShopName.ResetString();

	ChangeShopState( m_bShopStart );
	m_cbtnPremium.SetCheck( m_bPremium );

	m_btnShopCancel.SetVisible(FALSE);
	m_btnShopCancel.SetEnable(FALSE);
	m_btnShopBuy.SetVisible(FALSE);
	m_btnShopBuy.SetEnable(FALSE);
	//m_cbtnPremium.SetEnable( m_bPremium );
	m_btnPackageOK.SetEnable( m_bPremium );
	m_btnPackageCancel.SetEnable( m_bPremium );
	m_ebPackagePrice.SetEnable( m_bPremium );

	ClearItems();

	if( !bOpen )
	{
		_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, FALSE );

		// Unlock inventory
		_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PERSONAL_SHOP );

		// Close message box of shop
		_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
		_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
		_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
		_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
		_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
		_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
	}

	m_nCurRow = 0;
	m_sbBottomScrollBar.SetScrollPos( 0 );

	m_bCashPersonShop = FALSE;
	m_bCashPersonShop_open = FALSE;

	// Character state flags
	//_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ClearItems()
{
	for(int i = 0; i < PERSONAL_TRADE_SLOT_TOTAL; ++i)
		m_abtnTradeItems[i].InitBtn();			

	for(i = 0; i < PERSONAL_SHOP_SLOT_ROW_TOTAL; ++i)
	{
		for(int j = 0; j < PERSONAL_SHOP_SLOT_COL; ++j)
		{
			m_abtnShopItems[i][j].InitBtn();
		}
	}

	memset(&m_aiTradeItemCount, 0, sizeof(m_aiTradeItemCount));

	// Package Slot(5x1)	
	for(i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		m_abtnPackageItems[i].InitBtn();
	}

	// 판매 물품 비우기(구매 중일때...)
	if(!m_vectorSellItemList.empty())
	{
		m_vectorSellItemList.clear();
	}

	if(!m_vectorSellPackageList.empty())
	{
		m_vectorSellPackageList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return ;

	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strItemInfo);
		INDEX	nChatMax= (MAX_ITEMINFO_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			m_strItemInfo[m_nCurInfoLines] = strItemInfo;
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_ITEMINFO_CHAR;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strItemInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; ++iPos )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddItemInfoString( strTemp, colItemInfo );

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= MAX_ITEMINFO_CHAR )
		{
			m_strItemInfo[m_nCurInfoLines] = strItemInfo;
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		}
		
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_ITEMINFO_CHAR;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; ++iPos )
			{
				if( strItemInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; ++iPos )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddItemInfoString( strTemp, colItemInfo );
		}
	}
}

extern tm* g_tv_t; // 기간 표시용 전역 포인터 변수

#define IN_VALIDTM(a) if (a) {delete a; a=NULL;}

// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc : nWhichSlot - 0: opposite slot, 1: my slot, 2: inventory slot
// ----------------------------------------------------------------------------
BOOL CUIPersonalShop::GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight, int nTradeItem, int nPackageItem, int nRow, int nCol )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex = -1;
	SQUAD		llCount;
	ULONG		ulItemPlus, ulFlag;
	LONG		lItemUsed;
	LONG		lRareIndex;
	SBYTE		sbOptionType[MAX_ITEM_OPTION], sbOptionLevel[MAX_ITEM_OPTION];
	__int64		iPrice = 0;

	switch( nWhichSlot )
	{
	case 0:
		{
			nIndex		= m_abtnTradeItems[nTradeItem].GetItemIndex();
			llCount		= m_abtnTradeItems[nTradeItem].GetItemCount();
			ulItemPlus	= m_abtnTradeItems[nTradeItem].GetItemPlus();
			iPrice		= m_abtnTradeItems[nTradeItem].GetItemPrice();
			ulFlag		= m_abtnTradeItems[nTradeItem].GetItemFlag();
			lItemUsed	= m_abtnTradeItems[nTradeItem].GetItemUsed();
			lRareIndex	= m_abtnTradeItems[nTradeItem].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{
				sbOptionType[sbOption] = m_abtnTradeItems[nTradeItem].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnTradeItems[nTradeItem].GetItemOptionLevel( sbOption );
			}
		}
		break;

	case 1:
		{
			nIndex		= m_abtnShopItems[nRow][nCol].GetItemIndex();
			llCount		= m_abtnShopItems[nRow][nCol].GetItemCount();
			ulItemPlus	= m_abtnShopItems[nRow][nCol].GetItemPlus();
			iPrice		= m_abtnShopItems[nRow][nCol].GetItemPrice();
			ulFlag		= m_abtnShopItems[nRow][nCol].GetItemFlag();
			lItemUsed	= m_abtnShopItems[nRow][nCol].GetItemUsed();
			lRareIndex	= m_abtnShopItems[nRow][nCol].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{
				sbOptionType[sbOption]	= m_abtnShopItems[nRow][nCol].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnShopItems[nRow][nCol].GetItemOptionLevel( sbOption );
			}
		}
		break;
	case 2:
		{
			nIndex		= m_abtnPackageItems[nPackageItem].GetItemIndex();
			llCount		= m_abtnPackageItems[nPackageItem].GetItemCount();
			ulItemPlus	= m_abtnPackageItems[nPackageItem].GetItemPlus();
			iPrice		= m_abtnPackageItems[nPackageItem].GetItemPrice();
			ulFlag		= m_abtnPackageItems[nPackageItem].GetItemFlag();
			lItemUsed	= m_abtnPackageItems[nPackageItem].GetItemUsed();
			lRareIndex	= m_abtnPackageItems[nPackageItem].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{
				sbOptionType[sbOption]	= m_abtnPackageItems[nPackageItem].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnPackageItems[nPackageItem].GetItemOptionLevel( sbOption );
			}
		}
	}
	
	if( nIndex < 0 )
		return FALSE;

	CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
	CItemRareOption rItemRareOption =_pNetwork->GetRareOptionData(0);

	if(  rItemData.GetFlag() & ITEM_FLAG_RARE )
	{
		m_bRareItem =TRUE;
		rItemRareOption =_pNetwork->GetRareOptionData(lRareIndex);
		m_iRareGrade =rItemRareOption.GetGrade();
	}
	else
	{
		m_bRareItem =FALSE;
		m_iRareGrade =-1;
	}

	const CTString strItemName =_pNetwork->GetItemName( nIndex );

	CTString szItemName =strItemName;
	if( m_bRareItem )
	{
		CTString strPrefix = rItemRareOption.GetPrefix();
		if( strPrefix.Length() >0)
			szItemName.PrintF("%s %s", strPrefix, strItemName);
	}

	/*
	// Get item name
	if( !m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) )
	{
		CTString	strCount;
		strCount.PrintF( "%I64d", llCount );
		_pUIMgr->InsertCommaToString( strCount );		
		strTemp.PrintF( "%s(%s)", rItemData.GetName(), strCount );
	}
	else
	{
	*/
		COLOR colNas = 0xF2F2F2FF;

		if( ulItemPlus > 0 && !(_pUIMgr->IsPet(rItemData) || _pUIMgr->IsWildPet(rItemData)) && !(rItemData.GetFlag() & ITEM_FLAG_COUNT) )
		{
			if (rItemData.GetType() == CItemData::ITEM_ONCEUSE &&
				rItemData.GetSubType() == CItemData::ITEM_SUB_BOX)
			{
				strTemp = szItemName;
			}
			else
			{
				strTemp.PrintF( "%s +%d", szItemName, ulItemPlus );
			}			
		}
		else
		{
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				CTString	strCount;
				
				strCount.PrintF( "%I64d", llCount );
				_pUIMgr->InsertCommaToString( strCount );
				strTemp.PrintF( "%s(%s)", szItemName, strCount );

				colNas = _pUIMgr->GetNasColor( llCount );
			}
#ifdef Pet_IMPROVEMENT_1ST
			// [070604: Su-won] 1차 펫 기능 개선
			// 펫의 진화상태 표시
			else if( _pUIMgr->IsPet(rItemData) )
			{
				const INDEX iPetIndex = ulItemPlus;
				SBYTE sbPetTypeGrade =_pUIMgr->GetPetInfo()->GetPetTypeGrade( iPetIndex );

				if( sbPetTypeGrade >0 )
				{
					INDEX iPetType;
					INDEX iPetAge;
					_pNetwork->CheckPetType( sbPetTypeGrade, iPetType, iPetAge );
					strTemp = szItemName +CTString(" - ")+PetInfo().GetName(iPetType, iPetAge);
				}
				else
					strTemp = szItemName;
			}
#endif
			else
			{
				strTemp = szItemName;
			}
		}
	//}

	AddItemInfoString( strTemp, colNas );

	//const __int64	iPrice = CalculateItemPrice( m_nSelectedShopID, rItemData, 1, m_bBuyShop );	

	// Get item information in detail
	if( m_bDetailItemInfo )
	{

		if(rItemData.IsFlag(ITEM_FLAG_CASH))
		{
			if (lItemUsed > 0)
			{
				time_t tv_used = lItemUsed - _pUIMgr->GetRadar()->GetStartTime();
				g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
				strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
				
				AddItemInfoString( strTemp, 0xFFAA44FF );
				IN_VALIDTM(g_tv_t)
			}
		}

		// Price - except money
		if( nWhichSlot == 0 && (rItemData.GetType() != CItemData::ITEM_ETC ||
			rItemData.GetSubType() != CItemData::ITEM_ETC_MONEY ))
		{
			CTString	strMoney;
			
			strMoney.PrintF( "%I64d", iPrice );
			_pUIMgr->InsertCommaToString( strMoney );
			strTemp.PrintF( _S( 255, "가격 : %I64d" ), strMoney );
			
			AddItemInfoString( strTemp, _pUIMgr->GetNasColor( iPrice ) );
		}
		//AddItemInfoString( CTString( " " ) );

		COLOR colString =0xFFFFFFFF;

		switch( rItemData.GetType() )
		{
			
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				
				//초 고급 제련석 여부		PERSONALSHOP_1658_TEXT_SHOW_NA_20081020		// 업 그레이드 가능 아이템 
				if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) && 
					(g_iCountry == HONGKONG || g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)) 
				{
					BOOL bCanUseSuperGOJE = ( ulFlag & FLAG_ITEM_SUPER_STONE_USED ) ? FALSE : TRUE ;
					if( bCanUseSuperGOJE )
					{
						CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
						AddItemInfoString(strCanUseSuperGOJE, 0xFFFF40FF);
					}
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0xFFFFFFFF );
#else
				AddItemInfoString( strTemp, 0x9E9684FF );
#endif
				// Level
				int	nItemLevel = rItemData.GetLevel();
				int nWearLevelReduction =0;
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					if( sbOptionType[sbOption] == -1 || sbOptionType[sbOption] == 0 )
						break;

					//착용제한레벨 다운 옵션
					if( sbOptionType[sbOption]==49)
					{
						COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType[sbOption] );
						nWearLevelReduction = odItem.GetValue( sbOptionLevel[sbOption] - 1 );
						break;
					}
				}
				if( nWearLevelReduction >0 )
					strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction );
				else
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
#ifdef RARE_ITEM
					AddItemInfoString( strTemp, 0xFFFFFFFF );
#else

					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
#endif

				int	nPlusValue;
				int nBasePhysicalAttack =rItemData.GetPhysicalAttack() +rItemRareOption.GetPhysicalAttack();
				int nBaseMagicAttack =rItemData.GetMagicAttack() +rItemRareOption.GetMagicAttack();
				
#ifndef RARE_ITEM
				colString =0xDEC05BFF;
#endif
				if( ulItemPlus > 0 )
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetPhysicalAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), nBasePhysicalAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack );
															
						AddItemInfoString( strTemp, colString );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1891, "물리 공격력 + 75" ));		
							AddItemInfoString( strTemp, colString );
						}
					}

					// Magic attack
					if( rItemData.GetMagicAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetMagicAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetMagicAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), nBaseMagicAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
															
						AddItemInfoString( strTemp, colString );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1892, "마법 공격력 + 50" ));		
							AddItemInfoString( strTemp, colString );
						}
					}
				}
				else
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack );
						AddItemInfoString( strTemp, colString );
					}

					// Magic attack
					if( rItemData.GetMagicAttack() > 0 )
					{
						strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
						AddItemInfoString( strTemp, colString );
					}
				}

				if( lItemUsed > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), lItemUsed);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
				//초 고급 제련석 여부			PERSONALSHOP_1658_TEXT_SHOW_NA_20081020		// 업 그레이드 가능 아이템 
				if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) && 
					(g_iCountry == HONGKONG || g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)) 
				{
					BOOL bCanUseSuperGOJE = ( ulFlag & FLAG_ITEM_SUPER_STONE_USED ) ? FALSE : TRUE ;
					if( bCanUseSuperGOJE )
					{
						CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
						AddItemInfoString(strCanUseSuperGOJE, 0xFFFF40FF);
					}
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0xFFFFFFFF );	//흰색으로 표시
#else
				AddItemInfoString( strTemp, 0x9E9684FF );
#endif

				// Level
				int	nItemLevel = rItemData.GetLevel();
				int nWearLevelReduction =0;
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					if( sbOptionType[sbOption] == -1 || sbOptionType[sbOption] == 0 )
						break;

					//착용제한레벨 다운 옵션
					if( sbOptionType[sbOption]==49)
					{
						COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType[sbOption] );
						nWearLevelReduction = odItem.GetValue( sbOptionLevel[sbOption] - 1 );
						break;
					}
				}
				if( nWearLevelReduction >0 )
					strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction );
				else
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0xFFFFFFFF );	//흰색으로 표시
#else	
				AddItemInfoString( strTemp,
									nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
#endif

				int	nPlusValue;
				int nBasePhysicalDefence =rItemData.GetPhysicalDefence() +rItemRareOption.GetPhysicalDefence();
				int nBaseMagicDefence =rItemData.GetMagicDefence() +rItemRareOption.GetMagicDefence();
				
#ifndef RARE_ITEM
				colString =0xDEC05BFF;
#endif
				if( ulItemPlus > 0 )
				{
					// Physical defense
					if( rItemData.GetPhysicalDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetPhysicalDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalDefence(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), nBasePhysicalDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );
															
						AddItemInfoString( strTemp, colString );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, colString );

							strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
							AddItemInfoString( strTemp, colString );
						}
					}

					// Magic defense
					if( rItemData.GetMagicDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetMagicDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetMagicDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicDefence(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), nBaseMagicDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
															
						AddItemInfoString( strTemp, colString );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, colString );

							strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
							AddItemInfoString( strTemp, colString );
						}
					}
				}
				else
				{
					// Physical defense
					if( rItemData.GetPhysicalDefence() > 0 )
					{
						strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );
						AddItemInfoString( strTemp, colString );
					}

					// Magic defense
					if( rItemData.GetMagicDefence() > 0 )
					{
						strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
						AddItemInfoString( strTemp, colString );
					}
				}

				if(lItemUsed > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), lItemUsed);
					AddItemInfoString( strTemp, colString );
				}
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
				if( rItemData.GetSubType() == CItemData::ACCESSORY_PET ) // 펫인 경우 
				{
					// 서버로 부터 읽어 드린 펫 교환 정보를 찾아서 출력
					SPetExchangeInfoString pPetExchangeInfo;
					// 서버로 부터 읽드린 펫 정보가 있는지 확인 있다면 참조로 데이터 전달.
					if( _pUIMgr->GetPetInfo()->GetPetExchangeInfo( ulItemPlus, pPetExchangeInfo ) )
					{
						if( pPetExchangeInfo.strNameCard.Length() >0)
							AddItemInfoString( pPetExchangeInfo.strNameCard, 0xF2F2F2FF );
						AddItemInfoString( pPetExchangeInfo.strLevel, 0xDEC05BFF );		
						AddItemInfoString( pPetExchangeInfo.strHP, 0xDEC05BFF );		
						AddItemInfoString( pPetExchangeInfo.strlExp, 0xDEC05BFF );		
						AddItemInfoString( pPetExchangeInfo.strHungry, 0xDEC05BFF );		
						AddItemInfoString( pPetExchangeInfo.strSympathy, 0xDEC05BFF );		
						AddItemInfoString( pPetExchangeInfo.strAbility, 0xDEC05BFF );		
#ifdef PET_SEAL_TIME
						AddItemInfoString( pPetExchangeInfo.strRemainTime, 0xDEC05BFF );
#endif
					}
					
				}
				else if(rItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					sPetItem_Info pWildPetInfo;

					if(_pUIMgr->GetWildPetInfo()->GetWildPetInfo(ulItemPlus,pWildPetInfo))
					{
						strTemp.PrintF(_S(4215, "이름: %s"),pWildPetInfo.pet_name);
						AddItemInfoString(strTemp,  0xF2F2F2FF );
						strTemp.PrintF(_S(4216, "펫 레벨: %d"),pWildPetInfo.pet_level);
						AddItemInfoString(strTemp, 0xDEC05BFF);
						strTemp.PrintF(_S(4217, "힘: %d"),pWildPetInfo.pet_str);
						AddItemInfoString(strTemp, 0xDEC05BFF);
						strTemp.PrintF(_S(4218, "민첩: %d"),pWildPetInfo.pet_dex);
						AddItemInfoString(strTemp, 0xDEC05BFF);
						strTemp.PrintF(_S(4219, "지혜: %d"),pWildPetInfo.pet_int);
						AddItemInfoString(strTemp, 0xDEC05BFF);
						strTemp.PrintF(_S(4220, "체력: %d"),pWildPetInfo.pet_con);
						AddItemInfoString(strTemp, 0xDEC05BFF);
					}
				}
				else 
				{
					if( lItemUsed > 0)
					{
						strTemp.PrintF(  _S( 510, "내구도 : %ld" ), lItemUsed );		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}

			}
			break;

				// 일회용
		case CItemData::ITEM_ONCEUSE:
			{
				// 퀘스트 정보 표시.
				if ( rItemData.GetSubType() == CItemData::ITEM_SUB_QUEST_SCROLL )
				{	
					const int iQuestIndex = rItemData.GetNum0();

					if( iQuestIndex != -1 )
					{
						// 퀘스트 이름 출력
						strTemp.PrintF( "%s", CQuestSystem::Instance().GetQuestName( iQuestIndex ) );
						AddItemInfoString( strTemp, 0xDEC05BFF );
						
						const int iMinLevel = CQuestSystem::Instance().GetQuestMinLevel( iQuestIndex );
						const int iMaxLevel = CQuestSystem::Instance().GetQuestMaxLevel( iQuestIndex );

						// 레벨 제한 출력.
						strTemp.PrintF( _S( 1660, "레벨 제한 : %d ~ %d" ), iMinLevel, iMaxLevel );		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
			}
			break;

		case CItemData::ITEM_POTION:	// Date : 2005-01-07,   By Lee Ki-hwan
			{

				// Date : 2005-01-14,   By Lee Ki-hwan
				
				if ( rItemData.GetSubType() == CItemData::POTION_UP )
				{
					if( ulFlag > 0 )
					{
						// Level
						strTemp.PrintF( _S( 160, "레벨: %d" ), ulFlag );
						AddItemInfoString( strTemp, 0xD28060FF );

						// 향상 타입
						int nSkillType = rItemData.GetSkillType();
						CSkill	&rSkill = _pNetwork->GetSkillData( nSkillType );
						int Power = rSkill.GetPower( ulFlag - 1);

						if(  rItemData.GetNum1() == CItemData::POTION_UP_PHYSICAL ) // 물리
						{
							if(  rItemData.GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 790, "물리 공격력 +%d 상승" ), Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( rItemData.GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 791, "물리 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}

						}
						else if( rItemData.GetNum1() == CItemData::POTION_UP_MAGIC ) // 마법
						{
							if(  rItemData.GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 792, "마법 공격력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( rItemData.GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 793, "마법 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
						
						}
					}
						
				}
			}

		case CItemData::ITEM_BULLET:		// Bullet item
			{
			}
			break;

		case CItemData::ITEM_ETC:			// Etc item
			{
				switch( rItemData.GetSubType() )
				{
				case CItemData::ITEM_ETC_REFINE:
					{
						// FIXME : 레벨 표시가 안된다구 해서...
						// 블러드라고 표시가 되어있다면, 표시를 없애준다.
						if(ulFlag & FLAG_ITEM_OPTION_ENABLE)
						{
							ulFlag ^= FLAG_ITEM_OPTION_ENABLE;
						}

						// Level
						if( ulFlag > 0 )
						{
							strTemp.PrintF( _S( 160, "레벨: %d" ), ulFlag );
							AddItemInfoString( strTemp, 0xD28060FF );
						}
					}
					break;
					// 블러드 아이템 & 정화석.
				case CItemData::ITEM_ETC_OPTION:
					{

					}
					break;
				}
			}
			break;
		}

		// Weight
		strTemp.PrintF( _S( 165, "무게 : %d" ), rItemData.GetWeight() );
		AddItemInfoString( strTemp, 0xDEC05BFF );
		
		const int iFame = rItemData.GetFame();
		if( iFame > 0 )
		{
			strTemp.PrintF( _S( 1096, "명성 %d 필요" ), iFame );		
			AddItemInfoString( strTemp, 0xDEC05BFF );
		}

		if( m_bRareItem )
		{
			//if( lRareIndex <=0)
			if( m_iRareGrade <0)
				AddItemInfoString( _S(3165, "<미공개 옵션>"), 0xFF4040FF );
		}

		// Options
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:
		case CItemData::ITEM_SHIELD:
		case CItemData::ITEM_ACCESSORY:
			{
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					if( sbOptionType[sbOption] == -1 || sbOptionLevel[sbOption] == 0 )
						break;

					COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType[sbOption] );
					switch(sbOptionType[sbOption])
					{
					case 49:		//착용제한레벨 다운
						strTemp.PrintF( "%s : -%d", odItem.GetName(), odItem.GetValue( sbOptionLevel[sbOption] - 1 ) );
						break;
					case 50:		//소지량 증가
					case 51:		//마나 흡수
					case 52:		//생명력 흡수
					case 55:		//크리티컬확률 증가
					case 56:		//HP회복력 상승
					case 57:		//MP회복력 상승
					case 58:		//스킬쿨타임 감소
					case 59:		//MP소모량 감소
					case 60:		//스톤 내성 증가
					case 61:		//스턴 내성 증가
					case 62:		//침묵 내성 증가
					case 63:		//블록률 증가
					case 64:		//이동속도 향상
						strTemp.PrintF( "%s : %d%%", odItem.GetName(), odItem.GetValue( sbOptionLevel[sbOption] - 1 ) );
						break;
					case 53:		//암흑 공격
					case 54:		//독 공격
					default:
						strTemp.PrintF( "%s : %d", odItem.GetName(), odItem.GetValue( sbOptionLevel[sbOption] - 1 ) );
					}
					AddItemInfoString( strTemp, 0x94B7C6FF );
				}

				if( ulFlag & FLAG_ITEM_OPTION_ENABLE )
				{
					AddItemInfoString( _S( 511, "블러드 옵션 가능" ), 0xE53535FF );	
				}
				if( ulFlag & FLAG_ITEM_SEALED )
				{
					AddItemInfoString(  _S( 512, "봉인된 아이템" ), 0xE53535FF );
				}
			}
			break;
		}

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( nIndex );
		if( pDesc[0] != NULL )
		{
			//AddItemInfoString( CTString( " " ) );
			strTemp.PrintF( "%s", pDesc );
			AddItemInfoString( strTemp, 0x9E9684FF );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}
	else
	{
		// Price - except money
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( nWhichSlot == 0 && (ItemData.GetType() != CItemData::ITEM_ETC ||
			ItemData.GetSubType() != CItemData::ITEM_ETC_MONEY ))
		{
			CTString	strMoney;

			strMoney.PrintF( "%I64d", iPrice );
			_pUIMgr->InsertCommaToString( strMoney );
			strTemp.PrintF( _S( 255, "가격 : %I64d" ), strMoney );

			AddItemInfoString( strTemp, _pUIMgr->GetNasColor( iPrice ) );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nTradeItem, int nPackageItem, int nRow, int nCol )
{
	static int	nOldBtnID = -1;
	int			nBtnID;

	m_bShowItemInfo = FALSE;

	// Hide item information
	if( !bShowInfo )
	{
		nOldBtnID = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	// Trade
	if( nTradeItem >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID		= m_abtnTradeItems[nTradeItem].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnTradeItems[nTradeItem].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelTradeItemID == nTradeItem;
			if( !GetItemInfo( 0, nInfoWidth, nInfoHeight, nTradeItem ) )
				m_bShowItemInfo = FALSE;
		}
	}
	// Package
	else if( nPackageItem >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID		= m_abtnPackageItems[nPackageItem].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnPackageItems[nPackageItem].GetAbsPos( nInfoPosX, nInfoPosY );

			if(m_bBuyShop)
			{
				// Get item information
				m_bDetailItemInfo = TRUE;
			}
			else
			{
			// Get item information
			m_bDetailItemInfo = m_nSelPackageItemID == nPackageItem;
			}			
			if( !GetItemInfo( 2, nInfoWidth, nInfoHeight, -1, nPackageItem ) )
				m_bShowItemInfo = FALSE;
		}
	}
	// Shop
	else if( nRow >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_abtnShopItems[nRow][nCol].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnShopItems[nRow][nCol].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelShopItemID == ( nCol + nRow * PERSONAL_SHOP_SLOT_COL );;
			if( !GetItemInfo( 1, nInfoWidth, nInfoHeight, -1, -1, nRow, nCol ) )
				m_bShowItemInfo = FALSE;
		}
	}

	// Update item information box
	if( m_bShowItemInfo && bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		if( nInfoPosX < _pUIMgr->GetDrawPort()->dp_MinI )
			nInfoPosX = _pUIMgr->GetDrawPort()->dp_MinI;
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetDrawPort()->dp_MaxI )
			nInfoPosX = _pUIMgr->GetDrawPort()->dp_MaxI - nInfoWidth;

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetDrawPort()->dp_MinJ )
		{
			nInfoPosY += BTN_SIZE;
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowItemInfo )
		nOldBtnID = -1;
}

// ----------------------------------------------------------------------------
// Name : RenderShopItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIPersonalShop::RenderShopItems()
{
	int iShopX, iShopY;	
	iShopX = PERSONAL_SELL_BOTTOM_SLOT_SX;
	iShopY = PERSONAL_SELL_BOTTOM_SLOT_SY;

	int	iRow, iCol;
	int	nX = iShopX;
	int	nY = iShopY;
	int	iRowS = m_nCurRow;
	int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; ++iRow )
	{
		for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol, nX += 35 )
		{
			m_abtnShopItems[iRow][iCol].SetPos( nX, nY );

			if( m_abtnShopItems[iRow][iCol].IsEmpty() )
				continue;
			
			m_abtnShopItems[iRow][iCol].Render();
		}
		nX = iShopX;	nY += 35;
	}

	// ---Trade slot items---
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			continue;
			
		m_abtnTradeItems[iItem].Render();
	}

	// Package Slot(5x1)	
	for(iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if( m_abtnPackageItems[iCol].IsEmpty() )
			continue;
		
		m_abtnPackageItems[iCol].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Set shop texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Outline of items in shop slot ( unmovable )
	if( !m_bBuyShop )
	{
		for( iRow = iRowS; iRow < iRowE; ++iRow )
		{
			for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
			{
				// If button is empty
				if( m_abtnShopItems[iRow][iCol].IsEmpty() )
					continue;

				int			nIndex = m_abtnShopItems[iRow][iCol].GetItemIndex();
				CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
				m_abtnShopItems[iRow][iCol].GetAbsPos( nX, nY );

				// 대여 아이템 표시
				CItems		&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
				if(rItems.IsFlag(FLAG_ITEM_LENT))
				{
					_pUIMgr->GetDrawPort()->AddTexture( nX+17, nY+17, nX+32, nY+32,
														m_rtleaseMark.U0, m_rtleaseMark.V0,
														m_rtleaseMark.U1, m_rtleaseMark.V1,
														0xFFFFFFFF );			
				}

				// Not wearing, not refine stone, can trade
				if( m_abtnShopItems[iRow][iCol].GetItemWearType() < 0 &&
					// Date : 2005-01-15,   By Lee Ki-hwan
					//( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
					rItemData.GetFlag() & ITEM_FLAG_EXCHANGE && !rItems.IsFlag(FLAG_ITEM_LENT) && !rItems.IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED))
					continue;

				
				_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
													m_rtUnmovableOutline.U0, m_rtUnmovableOutline.V0,
													m_rtUnmovableOutline.U1, m_rtUnmovableOutline.V1,
													0xFFFFFFFF );
				
			}
		}
	}

	// Outline of selected item
	if( m_nSelTradeItemID >= 0 )
	{
		m_abtnTradeItems[m_nSelTradeItemID].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelPackageItemID >= 0 )
	{
		m_abtnPackageItems[m_nSelPackageItemID].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelShopItemID >= 0 )
	{
		int	nSelRow = m_nSelShopItemID / PERSONAL_SHOP_SLOT_COL;
		if( nSelRow >= iRowS && nSelRow < iRowE )
		{
			int	nSelCol = m_nSelShopItemID % PERSONAL_SHOP_SLOT_COL;

			m_abtnShopItems[nSelRow][nSelCol].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelectOutline.U0, m_rtSelectOutline.V0,
												m_rtSelectOutline.U1, m_rtSelectOutline.V1,
												0xFFFFFFFF );
		}
	}

	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{
		// Item information region
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right, m_rcItemInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render item information
		int	nInfoX = m_rcItemInfo.Left + 12;
		int	nInfoY = m_rcItemInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurInfoLines; ++iInfo )
		{
			if(iInfo==0 && m_bRareItem && m_iRareGrade>=0)
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, RareItem_Name_Color[m_iRareGrade] );
			else
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	else
	{
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}

	// Reading window
	if( m_ebShopName.DoesShowReadingWindow() )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebShopName.RenderReadingWindow();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::Render()
{
	// FIXME : 각각 상태에 따른 정리가 필요함.
	//ResetShop();
	COLOR colNas;

	if(m_bPremium != m_cbtnPremium.IsChecked())
	{
		m_bPremium = m_cbtnPremium.IsChecked();
		//m_btnPackageOK.SetEnable( m_bBuyShop & m_bPremium & !m_bPackageBuy);
		//m_btnPackageCancel.SetEnable( m_bBuyShop & m_bPremium & m_bPackageBuy);
		m_ebPackagePrice.SetEnable( m_bPremium );
		m_ebPackagePrice.SetFocus( m_bPremium );
		m_ebShopName.SetFocus(FALSE);

	}

	m_btnPackageOK.SetEnable( m_bBuyShop & m_bPremium & !m_bPackageBuy);
	m_btnPackageCancel.SetEnable( m_bBuyShop & m_bPremium & m_bPackageBuy);
	
	// Set shop texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	int	nX, nY;

	// Add render regions
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 33,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	if(m_bBuyShop)
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackBottom.U0, m_rtBackBottom.V0,
											m_rtBackBottom.U1, m_rtBackBottom.V1,
											0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackSellShop.U0, m_rtBackSellShop.V0,
											m_rtBackSellShop.U1, m_rtBackSellShop.V1,
											0xFFFFFFFF );
	}
	
	// 판매 상점 & 구매 시...
	nX = m_nPosX;
	nY = m_nPosY + 22;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 18,
										m_rtTitleInven.U0, m_rtTitleInven.V0,
										m_rtTitleInven.U1, m_rtTitleInven.V1,
										0xFFFFFFFF );										

	nY += 18;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + PERSONAL_TRADE_HEIGHT,
										m_rtBottomInven.U0, m_rtBottomInven.V0,
										m_rtBottomInven.U1, m_rtBottomInven.V1,
										0xFFFFFFFF );

	nY += PERSONAL_TRADE_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 1,
										m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
										m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
										0xFFFFFFFF );
	
	nY += 1;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 64,
										m_rtMiddleInven.U0, m_rtMiddleInven.V0,
										m_rtMiddleInven.U1, m_rtMiddleInven.V1,
										0xFFFFFFFF );

	nY += 64;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 30,
										m_rtPackageBar.U0, m_rtPackageBar.V0,
										m_rtPackageBar.U1, m_rtPackageBar.V1,
										0xFFFFFFFF );

	nY += 30;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 1,
										m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
										m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
										0xFFFFFFFF );

	nY += 1;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + PERSONAL_SHOP_HEIGHT,
										m_rtTopInven.U0, m_rtTopInven.V0,
										m_rtTopInven.U1, m_rtTopInven.V1,
										0xFFFFFFFF );

	if(!m_bBuyShop)
	{
		nY += PERSONAL_SHOP_HEIGHT;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 18,
											m_rtTitleInven.U0, m_rtTitleInven.V0,
											m_rtTitleInven.U1, m_rtTitleInven.V1,
											0xFFFFFFFF );

		nY += 18;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 32,
											m_rtNameInven.U0, m_rtNameInven.V0,
											m_rtNameInven.U1, m_rtNameInven.V1,
											0xFFFFFFFF );
	}
	else
	{
		nY += PERSONAL_SHOP_HEIGHT;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 19,
											m_rtPriceBar.U0, m_rtPriceBar.V0,
											m_rtPriceBar.U1, m_rtPriceBar.V1,
											0xFFFFFFFF );

		nY += 19;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 31,
											m_rtBlankBar.U0, m_rtBlankBar.V0,
											m_rtBlankBar.U1, m_rtBlankBar.V1,
											0xFFFFFFFF );
	}

	// Bottom scroll bar		
	m_sbBottomScrollBar.Render();

	// Close button
	m_btnClose.Render();

	// Buy & sell button of shop
	if(m_bBuyShop)
	{
		m_btnShopBuy.Render();
	
		// Cancel button of shop
		m_btnShopCancel.Render();
	}

	m_btnPackageOK.Render();
	m_btnPackageCancel.Render();

	// Start & Stop Button of Shop
	if( !m_bBuyShop )
	{
		if(m_bShopStart)
		{
			m_btnShopStop.Render();			
		}
		else
		{	
			m_btnShopStart.Render();
		}		
		m_cbtnPremium.Render();
		m_ebPackagePrice.Render();
		m_ebShopName.Render();
	}	

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	if( !m_bBuyShop )
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 753, "상점 이름" ), m_nPosX + 55, m_nPosY + 368 );		
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 754, "물품 등록" ), m_nPosX + 55, m_nPosY + 26 );		
	}
	else
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 755, "판매 물품" ), m_nPosX + 55, m_nPosY + 26 );		
	}
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 756, "패키지 판매" ), m_nPosX + 55, m_nPosY + 128 );		

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render item information
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 757, "개인상점" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,	
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	

	if( m_bBuyShop )
	{
		colNas = _pUIMgr->GetNasColor( m_strTotalPrice );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strTotalPrice, m_nPosX + PERSONAL_TRADEPRICE_POSX, m_nPosY + PERSONAL_SELL_TRADEPRICE_POSY, colNas );
	}

	if( m_bBuyShop )
	{
		colNas = _pUIMgr->GetNasColor( m_strPlayerMoney );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strPlayerMoney, m_nPosX + PERSONAL_MYMONEY_POSX, m_nPosY + PERSONAL_MYMONEY_POSY, colNas );

		if(m_bPremium)
		{
			colNas = _pUIMgr->GetNasColor( m_strPackagePrice );	
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strPackagePrice, m_nPosX + PERSONAL_MYMONEY_POSX, m_nPosY + 191, colNas );
		}
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render Items
	RenderShopItems();

	// Reading window
	if( !m_bBuyShop )
	{
		if( m_ebPackagePrice.DoesShowReadingWindow() )
		{
			// Set texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebPackagePrice.RenderReadingWindow();

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		else if( m_ebShopName.DoesShowReadingWindow() )
		{
			// Set texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebShopName.RenderReadingWindow();

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPersonalShop::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Hold item button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				// Shop items
				if( m_nSelShopItemID >= 0 )
				{
					int	nSelRow = m_nSelShopItemID / PERSONAL_SHOP_SLOT_COL;
					int	nSelCol = m_nSelShopItemID % PERSONAL_SHOP_SLOT_COL;

					if( m_bBuyShop )
					{
						// Close message box of shop
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
						_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
						
						_pUIMgr->SetHoldBtn( m_abtnShopItems[nSelRow][nSelCol] );
						int	nOffset = BTN_SIZE / 2;
						_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
						
						m_abtnShopItems[nSelRow][nSelCol].SetBtnState( UBES_IDLE );
					}
					else
					{
						// Not wearing, not refine stone, can trade
						CItems		&rItems = _pNetwork->MySlotItem[0][nSelRow][nSelCol];
						CItemData	&rItemData = rItems.ItemData;
						if( rItems.Item_Wearing == -1 &&
							//( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
							rItemData.GetFlag() & ITEM_FLAG_EXCHANGE && !rItems.IsFlag(FLAG_ITEM_LENT) && !rItems.IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED))
						{
							// Close message box of shop
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
							_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
							
							_pUIMgr->SetHoldBtn( m_abtnShopItems[nSelRow][nSelCol] );
							int	nOffset = BTN_SIZE / 2;
							_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
							
							m_abtnShopItems[nSelRow][nSelCol].SetBtnState( UBES_IDLE );
						}
					}
				}
				else if( m_nSelPackageItemID >= 0 )
				{
					// Close message box of shop
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
					_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
					
					_pUIMgr->SetHoldBtn( m_abtnPackageItems[m_nSelPackageItemID] );
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnPackageItems[m_nSelPackageItemID].SetBtnState( UBES_IDLE );
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of shop
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
					_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
					
					_pUIMgr->SetHoldBtn( m_abtnTradeItems[m_nSelTradeItemID] );
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnTradeItems[m_nSelTradeItemID].SetBtnState( UBES_IDLE );
				}

				bLButtonDownInItem	= FALSE;
			}

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Buy button of shop
			else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of shop
			else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Start button of shop
			else if( m_btnShopStart.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Stop button of shop
			else if( m_btnShopStop.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPackageOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPackageCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_cbtnPremium.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
			// Bottom Scroll bar
			if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			// Shop
			else if( IsInsideRect( nX, nY, m_rcTop ) )
			{
				int	iRow, iCol;
				int	iRowS = m_nCurRow;							// Start Row
				int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;			// End Row
				int	nWhichRow = -1, nWhichCol = -1;
				for( iRow = iRowS; iRow < iRowE; ++iRow )
				{
					for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
					{
						if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichRow = iRow;	
							nWhichCol = iCol;
						}
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, -1, nWhichRow, nWhichCol );

				return WMSG_SUCCESS;
			}
			// Trade
			else if( IsInsideRect( nX, nY, m_rcBottom ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
				{
					if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;	
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichItem );

				return WMSG_SUCCESS;
			}
			// Package
			else if( m_bPremium && IsInsideRect( nX, nY, m_rcMiddle ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
				{
					if( m_abtnPackageItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;	
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichItem );

				return WMSG_SUCCESS;
			}

			// Hide tool tip
			ShowItemInfo( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Buy button of shop
				else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Start Button of shop
				else if( m_btnShopStart.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Stop Button of shop
				else if( m_btnShopStop.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnPackageOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					PackageToShop( TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_btnPackageCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					PackageToShop( FALSE );
					return WMSG_SUCCESS;
				}
				// Premium Check Box of shop
				else if( !m_bShopStart && m_cbtnPremium.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					ClearPackageSlot();
					return WMSG_SUCCESS;
				}
				else if( m_ebShopName.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	m_ebShopName.SetFocus(TRUE);
					m_ebPackagePrice.SetFocus(FALSE);
					_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_ebPackagePrice.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	m_ebPackagePrice.SetFocus(TRUE);
					m_ebShopName.SetFocus(FALSE);
					_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
					return WMSG_SUCCESS;
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;
					m_nSelTradeItemID = -1;
					m_nSelPackageItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;			// 4줄~
					for( iRow = iRowS; iRow < iRowE; ++iRow )
					{
						for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
						{
							if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected item
								m_nSelShopItemID = iCol + iRow * PERSONAL_SHOP_SLOT_COL;	// 선택된 슬롯의 아이템 ID

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, -1, iRow, iCol );

								bLButtonDownInItem	= TRUE;

								_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Package Items
				else if( m_bPremium && !m_bBuyShop && IsInsideRect( nX, nY, m_rcMiddle ) )
				{
					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;
					m_nSelPackageItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
					{
						if( m_abtnPackageItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelPackageItemID = iItem;			// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							bLButtonDownInItem	= TRUE;

							_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;
					m_nSelPackageItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							bLButtonDownInItem	= TRUE;

							_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}				
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInItem = FALSE;

			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If shop isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// FIXME : 코드가 중복되는 곳이 너무 많음.
						if(!m_bBuyShop && m_bShopStart)
						{
							//_pUIMgr->GetInventory()->Lock( m_bShopStart );
							//_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, FALSE );
							_pUIMgr->GetChatting()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
						}
						else
						{
							ResetShop();
							_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
						}
					}
					return WMSG_SUCCESS;
				}
				// Buy button of shop
				else if( m_bBuyShop && ( wmsgResult = m_btnShopBuy.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 BuyItems();
						 
						 // Clear Member Value
						 ResetShop();

						 _pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of shop
				else if( ( wmsgResult = m_btnShopCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// FIXME : 코드가 중복되는 곳이 너무 많음.
						if(!m_bBuyShop && m_bShopStart)
						{
							//_pUIMgr->GetInventory()->Lock( m_bShopStart );
							//_pUIMgr->RearrangeOrder( UI_PERSONALSHOP, FALSE );
							_pUIMgr->GetChatting()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
						}
						else
						{
							ResetShop();
							_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
						}
					}
					return WMSG_SUCCESS;
				}
				// Premium OK button of shop
				else if( ( wmsgResult = m_btnPackageOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				// Premium Cancel button of shop
				else if( ( wmsgResult = m_btnPackageCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ClearPackageSlot();
					}
					return WMSG_SUCCESS;
				}
				// Start button of shop
				else if( ( wmsgResult = m_btnShopStart.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
						//m_bShopStart = TRUE;
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OKCANCEL,		
							UI_PERSONALSHOP, MSGCMD_SHOP_START );						
						int iRatio = 2;
						if(m_bPremium)
						{
							if( g_iCountry == JAPAN )
								iRatio = 10;
							else
								iRatio = 2;

							strMessage.PrintF( _S( 758, "프리미엄 서비스의 사용료는 판매된 아이템 가격의 %d 퍼센트입니다.  계속 진행하시겠습니까?" ), iRatio );
						}
						else
						{
							strMessage.PrintF( _S( 759, "개인상점을 개설합니다.  계속 진행하시겠습니까?" ) );		
						}
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					return WMSG_SUCCESS;
				}
				// Stop button of shop
				else if( ( wmsgResult = m_btnShopStop.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{						
						SendPersonalShopStop();
					}
					return WMSG_SUCCESS;
				}			
				// Bottom Scroll bar
				else if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; ++iRow )
					{
						for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
						{
							if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Nothing
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Package items
				else if( m_bPremium && IsInsideRect( nX, nY, m_rcMiddle ) )
				{
					for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
					{
						if( m_abtnPackageItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
			}		
			else
			{
				//////////////////////////////////////////////////////////////////////////
				if( (m_bBuyShop || !m_bShopStart) && IsInside( nX, nY ) )
				{
					// If holding button is item and is from shop
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_PERSONALSHOP )
					{
						// Trade items
						if( m_nSelPackageItemID == -1 && IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from shop slot
							if( m_nSelTradeItemID < 0 ||
								m_abtnTradeItems[m_nSelTradeItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								AddShopItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount(), SLOT_TRADE );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Package Items
						else if( m_nSelTradeItemID == -1 && IsInsideRect( nX, nY, m_rcMiddle ) )
						{
							// If this item is moved from shop slot
							if( m_nSelPackageItemID < 0 ||
								m_abtnPackageItems[m_nSelPackageItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								if(m_bPremium)
								{
								AddShopItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount(), SLOT_PACKAGE );
								}
								else
								{
									
									_pUIMgr->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
										UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
									CTString	strMessage = _S( 760, "패키지 판매는 프리미엄 서비스를 사용해야만 가능합니다." );		
									MsgBoxInfo.AddString( strMessage );
									_pUIMgr->CreateMessageBox( MsgBoxInfo );									
								}

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Shop items
						else if( IsInsideRect( nX, nY, m_rcTop ) )
						{
							int	nSelRow = m_nSelShopItemID / PERSONAL_SHOP_SLOT_COL;
							int	nSelCol = m_nSelShopItemID % PERSONAL_SHOP_SLOT_COL;
							// If this item is moved from trade slot
							if( m_nSelShopItemID < 0 ||
								m_abtnShopItems[nSelRow][nSelCol].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								DelShopItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount(),
												m_nSelTradeItemID, m_nSelPackageItemID, SLOT_SHOP );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item

					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				} // If - IsInside
				//////////////////////////////////////////////////////////////////////////
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Trade items
				if( (m_bBuyShop || !m_bShopStart) && IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	nTradeItemID = m_nSelTradeItemID;
					m_nSelTradeItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Close message box of shop
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
							_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
							
							DelShopItem( m_abtnTradeItems[iItem].GetItemRow(),
								m_abtnTradeItems[iItem].GetItemCol(),
								m_abtnTradeItems[iItem].GetItemUniIndex(),
											m_abtnTradeItems[iItem].GetItemCount(),
											nTradeItemID, -1, SLOT_TRADE );

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// Package items
				if( (!m_bBuyShop && !m_bShopStart) && m_bPremium && IsInsideRect( nX, nY, m_rcMiddle ) )
				{
					int	nPackageItemID = m_nSelPackageItemID;
					m_nSelPackageItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
					{
						if( m_abtnPackageItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Close message box of shop
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
							_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
							
							DelShopItem( m_abtnPackageItems[iItem].GetItemRow(),
								m_abtnPackageItems[iItem].GetItemCol(),
								m_abtnPackageItems[iItem].GetItemUniIndex(),
											m_abtnPackageItems[iItem].GetItemCount(),
											-1, nPackageItemID, SLOT_PACKAGE );

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// Shop items
				else if( (m_bBuyShop || !m_bShopStart) && IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; ++iRow )
					{
						for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
						{
							if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								if( m_bBuyShop )
								{
									// Close message box of shop
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
									_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
									
									AddShopItem( iRow, iCol,
										m_abtnShopItems[iRow][iCol].GetItemUniIndex(),
										m_abtnShopItems[iRow][iCol].GetItemCount(), SLOT_TRADE );
									
								}
								else
								{
									// Not wearing, not refine stone, can trade
									CItems		&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
									CItemData	&rItemData = rItems.ItemData;
									if( rItems.Item_Wearing == -1 &&
										//( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
										rItemData.GetFlag() & ITEM_FLAG_EXCHANGE )
									{
										// Close message box of shop
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_PRICE );
										_pUIMgr->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_START );
										
										AddShopItem( iRow, iCol,
											m_abtnShopItems[iRow][iCol].GetItemUniIndex(),
														m_abtnShopItems[iRow][iCol].GetItemCount(), SLOT_TRADE);
									}
								}

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, -1, iRow, iCol );

								return WMSG_SUCCESS;
							}
						}
					}
				}				
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				
				// Bottom Scroll bar
				if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::KeyMessage( MSG *pMsg )
{
	// If login is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( m_ebPackagePrice.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebShopName.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::IMEMessage( MSG *pMsg )
{
	WMSG_RESULT wmsgResult;

	if( ( wmsgResult = m_ebShopName.IMEMessage( pMsg ) ) != WMSG_FAIL )
	{
		return wmsgResult;
	}
	else 
	{
		return m_ebPackagePrice.IMEMessage( pMsg );
	}
	// 이기환 수정 끝
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::CharMessage( MSG *pMsg )
{
	if( m_ebPackagePrice.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebShopName.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempIndex;
static int		nTempUniIndex;
static int		nTempRow, nTempCol;
static SQUAD	llTempCount;
static int		nTempTradeItemID;
static int		nTempPackageItemID;
static ULONG	ulItemPlus;
static ULONG	ulItemFlag;
static SQUAD	llTempPrice;
static LONG		lItemUsed;
static int		nTempSlot;
static LONG		lTempRareIndex;
static SBYTE	TempsbOptionType[MAX_ITEM_OPTION];
static SBYTE	TempsbOptionLevel[MAX_ITEM_OPTION];

// ----------------------------------------------------------------------------
// Name : AddShopItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIPersonalShop::AddShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nWhichSlot )
{
	nTempRow		= nRow;
	nTempCol		= nCol;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempIndex		= m_abtnShopItems[nTempRow][nTempCol].GetItemIndex();
	ulItemPlus		= m_abtnShopItems[nTempRow][nTempCol].GetItemPlus();
	ulItemFlag		= m_abtnShopItems[nTempRow][nTempCol].GetItemFlag();
	lItemUsed		= m_abtnShopItems[nTempRow][nTempCol].GetItemUsed();
	nTempSlot		= nWhichSlot;
	lTempRareIndex	= m_abtnShopItems[nTempRow][nTempCol].GetItemRareIndex();
	
	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		TempsbOptionType[i] = -1;
		TempsbOptionLevel[i] = 0;
	}

	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{	
		TempsbOptionType[sbOption] = m_abtnShopItems[nTempRow][nTempCol].GetItemOptionType( sbOption );
		TempsbOptionLevel[sbOption] = m_abtnShopItems[nTempRow][nTempCol].GetItemOptionLevel( sbOption );
	}

	if( nTempIndex == -1 )
	{
		return;
	}

	CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );

	if (g_iCountry == HONGKONG && _pUIMgr->IsPet(rItemData))
	{
		return;
	}

	if((rItemData.GetType() == CItemData::ITEM_ETC && 
		rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY))
		return;

	// 대여 아이템 판매 불가
	int	nSelRow = m_nSelShopItemID / PERSONAL_SHOP_SLOT_COL;
	int	nSelCol = m_nSelShopItemID % PERSONAL_SHOP_SLOT_COL;
	CItems		&rItems = _pNetwork->MySlotItem[0][nSelRow][nSelCol];
	if(rItems.IsFlag(FLAG_ITEM_LENT) || rItems.IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED))
	{
		_pUIMgr->CloseMessageBox(MSGCMD_NULL);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
		CTString strMessage(_S(3052,"판매할 수 없는 아이템 입니다."));
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		return;
	}

	if( !m_bBuyShop && nWhichSlot != SLOT_PACKAGE )
	{
		// Ask Price
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,		
			UI_PERSONALSHOP, MSGCMD_SHOP_PRICE );
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
	
		if( ulItemPlus > 0 &&  !_pUIMgr->IsPet(rItemData) && !_pUIMgr->IsWildPet(rItemData) )
		{
			strMessage.PrintF( _S( 761, "%s +%d의 개당 가격을 입력해 주십시오." ), szItemName, ulItemPlus );		
		}
		else 
		{
			strMessage.PrintF( _S( 762, "%s의 개당 가격을 입력해 주십시오." ), szItemName );		
		}
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		/*
		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_PRICE );
		CTString strItemCount;
		SQUAD llPrice = 1;			// Default Price
		strItemCount.PrintF( "%I64d", llPrice );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().SetString( strItemCount.str_String );		
		*/
	}	
	else
	{
		AskQuantity();
	}
}

// ----------------------------------------------------------------------------
// Name : AskQuantity()
// Desc : 
// ----------------------------------------------------------------------------
void CUIPersonalShop::AskQuantity()
{
	CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
	const char	*szItemName = _pNetwork->GetItemName( nTempIndex );

	// Ask quantity
	if( ( m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,		
									UI_PERSONALSHOP, MSGCMD_SHOP_ADD_ITEM );
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_ADD_ITEM );
		CTString strItemCount;
		strItemCount.PrintF( "%I64d", llTempCount );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
	}
	else if( ulItemPlus > 0 && !(_pUIMgr->IsPet(rItemData)||_pUIMgr->IsWildPet(rItemData)))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_YESNO,
									UI_PERSONALSHOP, MSGCMD_SHOP_ADD_PLUSITEM );
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
	}
	else
	{
		//ShopToTrade( llTempCount, llTempPrice, 0, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex );
		ShopToTrade( llTempCount, llTempPrice, ulItemPlus, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIPersonalShop::DelShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nTradeItemID, int nPackageItemID, int nWhichSlot )
{
	nTempRow			= nRow;
	nTempCol			= nCol;
	nTempUniIndex		= nUniIndex;
	llTempCount			= llCount;
	nTempTradeItemID	= nTradeItemID;
	nTempPackageItemID	= nPackageItemID;
	nTempSlot			= nWhichSlot;

	if(nTempTradeItemID >= 0)
	{
		nTempIndex			= m_abtnTradeItems[nTempTradeItemID].GetItemIndex();
		
		CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
		
		// Ask quantity
		if( llTempCount > 1 )
		{
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
				UI_PERSONALSHOP, MSGCMD_SHOP_DEL_ITEM );
			const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			if( !m_bBuyShop )
			{
				CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_DEL_ITEM );
				CTString strItemCount;
				strItemCount.PrintF( "%I64d", llCount );
				ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
				pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
			}
		}
		else
		{
			TradeToShop( llTempCount, nWhichSlot );
		}
	}
	else if(nPackageItemID >= 0)
	{
		nTempIndex			= m_abtnPackageItems[nTempPackageItemID].GetItemIndex();
		
		CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
		
		// Ask quantity
		if( llTempCount > 1 )
		{
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
				UI_PERSONALSHOP, MSGCMD_SHOP_DEL_ITEM );
			const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_DEL_ITEM );
			CTString strItemCount;
			strItemCount.PrintF( "%I64d", m_abtnPackageItems[nTempPackageItemID].GetItemCount() );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
		}
		else
		{
			TradeToShop( llTempCount, nWhichSlot );
		}
	}
	else
	{
		TradeToShop( llTempCount, nWhichSlot );
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::BuyItems()
{
	// Buy nothing
	if( m_nTotalPrice < 0 )
		return;

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		_pUIMgr->GetChatting()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	/*
	// Check inventory space
	int	ctEmptySlot = 0;
	int	ctTradeItems = m_nNumOfItem;
	for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW_TOTAL; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{
			if( m_abtnShopItems[iRow][iCol].IsEmpty() )
			{
				ctEmptySlot++;
				continue;
			}

			int	nIndex = m_abtnShopItems[iRow][iCol].GetItemIndex();
			CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
				{
					if( nIndex == m_abtnTradeItems[iItem].GetItemIndex() )
						ctTradeItems--;
				}
			}
		}
	}

	// Not enough space
	if( ctEmptySlot < ctTradeItems )
	{
		// Add system message
		_pUIMgr->GetChatting()->AddSysMessage( _S( 265, "인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
		return;
	}
	*/

	//_pNetwork->BuyItem( m_nSelectedShopID, m_nNumOfItem, m_nTotalPrice );
	SendPersonalShopBuy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIPersonalShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_ITEM:
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			for( int iChar = 0; iChar < nLength; ++iChar )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( m_bBuyShop )
				{
					if( llCount > 0 )
					{
						//ShopToTrade( llCount, llTempPrice, 0, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex);
						ShopToTrade( llCount, llTempPrice, ulItemPlus, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex);
					}
				}
				else
				{
					if( llCount > 0 && llCount <= llTempCount )
					{
						//ShopToTrade( llCount, llTempPrice, 0, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex );
						ShopToTrade( llCount, llTempPrice, ulItemPlus, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex);
					}
				}
			}
		}
		break;

	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			ShopToTrade( 1, llTempPrice, ulItemPlus, ulItemFlag, lItemUsed, nTempSlot, lTempRareIndex );
		}
		break;

	case MSGCMD_SHOP_DEL_ITEM:
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			for( int iChar = 0; iChar < nLength; ++iChar )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount > 0 && llCount <= llTempCount )
					TradeToShop( llCount, nTempSlot );
			}
		}
		break;
	case MSGCMD_SHOP_PRICE:		// 가격 입력.
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			for( int iChar = 0; iChar < nLength; ++iChar )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				llTempPrice		= llCount;
				AskQuantity();
			}
		}
		break;
	case MSGCMD_PERSONALSHOP_ERROR:
		{
			// Nothing
		}
		break;
	case MSGCMD_SHOP_START:		// 판매 개시
		{			
			SendPersonalShopStart();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::ShopToTrade( SQUAD llCount, SQUAD llPrice, ULONG ulPlus, ULONG ulFlag, LONG lUsed, int iSlot, LONG lRareIndex )
{
	/*
	if( m_bBuyShop )
	{
		CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
		SQUAD		llPrice = m_nTotalPrice;// +
							//CalculateItemPrice( m_nSelectedShopID, rItemData, llCount, m_bBuyShop );

		// Not enough money
		if(_pNetwork->MyCharacterInfo.money < llPrice )
		{
			// Add system message
			_pUIMgr->GetChatting()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
			return;
		}
	}
	*/

	// My Inventory -> Package Slot
	if(iSlot == SLOT_PACKAGE)
	{
		// Find same item in trade slot
		for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
		{
			if( m_abtnPackageItems[iItem].GetItemUniIndex() == nTempUniIndex )
				break;
		}
		
		// If there is same item
		if( iItem < PERSONAL_PACKAGE_SLOT_COL )
		{
			// Check if item is countable
			CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				if( !m_bBuyShop )
				{
					// Update count of trade item
					SQUAD	llNewCount = m_abtnPackageItems[iItem].GetItemCount();
					llNewCount += llCount;
					m_abtnPackageItems[iItem].SetItemCount( llNewCount );
					
					// Update count of shop item
					llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
					llNewCount -= llCount;
					m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
					if( llNewCount <= 0 )
					{
						m_abtnShopItems[nTempRow][nTempCol].SetEmpty( TRUE );
						
						// Unselect item
						if( m_nSelShopItemID == ( nTempCol + nTempRow * PERSONAL_SHOP_SLOT_COL ) )
							m_nSelShopItemID = -1;
					}
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					_pUIMgr->InsertCommaToString( m_strTotalPrice );
					return;
				}
			}
		}
		
		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
		{
			if( m_abtnPackageItems[iItem].IsEmpty() )
				break;
		}
		
		// If there is not empty slot
		if( iItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			// Add system message
			if( m_bBuyShop )
				_pUIMgr->GetChatting()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				_pUIMgr->GetChatting()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		if( !m_bBuyShop )
		{
			m_abtnPackageItems[iItem].SetItemInfo( 0, nTempRow, nTempCol, nTempIndex, nTempUniIndex, -1 );
			m_abtnPackageItems[iItem].SetItemCount( llCount );
			m_abtnPackageItems[iItem].SetItemPrice( llPrice );
			m_abtnPackageItems[iItem].SetItemPlus( ulPlus );
			m_abtnPackageItems[iItem].SetItemFlag( ulFlag );
			m_abtnPackageItems[iItem].SetItemUsed( lUsed );
			m_abtnPackageItems[iItem].SetItemRareIndex( lRareIndex );
			
			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{	
				m_abtnPackageItems[iItem].SetItemOptionData(sbOption, TempsbOptionType[sbOption], TempsbOptionLevel[sbOption]);
			}
		}
	}
	// My Inventory -> Trade Slot
	else if(nTempIndex != PACKAGE_ITEM_INDEX && iSlot == SLOT_TRADE)
	{
		/*
		// NOTE : 패키지 아이템에 대한 처리...
		if( nTempIndex == PACKAGE_ITEM_INDEX)
		{
			m_nSelShopItemID = -1;
			m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
			m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
			_pUIMgr->InsertCommaToString( m_strTotalPrice );
			return;
		}
		*/

		// Find same item in trade slot
		for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if( m_abtnTradeItems[iItem].GetItemUniIndex() == nTempUniIndex )
				break;
		}
		
		// If there is same item
		if( iItem < PERSONAL_TRADE_SLOT_TOTAL )
		{
			m_aiTradeItemCount[iItem] += llCount;
			// Check if item is countable
			CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{				
				if( !m_bBuyShop )		// 개인상점 구매중 중첩 아이템의 경우 [최대 10개...]란 메시지가 뜨는 버그로 삭제함
										// 이 부분을 막을 경우에. 개인 상점 구매중에 물품 목록의 갯수가 계속 늘어나게 됨.
				{
					// Update count of trade item
					SQUAD	llNewCount = m_abtnTradeItems[iItem].GetItemCount();
					llNewCount += llCount;
					m_abtnTradeItems[iItem].SetItemCount( llNewCount );				
					
					
					// Update count of shop item
					llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
					llNewCount -= llCount;
					m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
					if( llNewCount <= 0 )
					{
						m_abtnShopItems[nTempRow][nTempCol].SetEmpty( TRUE );
						
						// Unselect item
						if( m_nSelShopItemID == ( nTempCol + nTempRow * PERSONAL_SHOP_SLOT_COL ) )
							m_nSelShopItemID = -1;
					}
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					_pUIMgr->InsertCommaToString( m_strTotalPrice );
					return;
				}
			}
		}
		
		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if( m_abtnTradeItems[iItem].IsEmpty() )
				break;
		}
		
		// If there is not empty slot
		if( iItem == PERSONAL_TRADE_SLOT_TOTAL )
		{
			// Add system message
			if( m_bBuyShop )
				_pUIMgr->GetChatting()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				_pUIMgr->GetChatting()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		if( !m_bBuyShop )
		{
			m_abtnTradeItems[iItem].SetItemInfo( 0, nTempRow, nTempCol, nTempIndex, nTempUniIndex, -1 );
			m_abtnTradeItems[iItem].SetItemCount( llCount );
			m_abtnTradeItems[iItem].SetItemPrice( llPrice );
			m_abtnTradeItems[iItem].SetItemPlus( ulPlus );
			m_abtnTradeItems[iItem].SetItemFlag( ulFlag );
			m_abtnTradeItems[iItem].SetItemUsed( lUsed );
			m_abtnTradeItems[iItem].SetItemRareIndex( lRareIndex );
			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
			{	
				m_abtnTradeItems[iItem].SetItemOptionData(sbOption, TempsbOptionType[sbOption], TempsbOptionLevel[sbOption]);				
			}
		}
	}
	
	//if( !m_bBuyShop )
	{
		// Update count of shop item
		SQUAD	llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
		if(llNewCount < llCount)
			return;
		llNewCount -= llCount;
		m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount <= 0 )
		{
			// 패키지 아이템의 경우, 패키지 상태를 해제하는 것임?
			if(nTempIndex == PACKAGE_ITEM_INDEX)
			{
				m_bPackageBuy	= FALSE;
				m_abtnShopItems[nTempRow][nTempCol].InitBtn();
			}

			//m_abtnShopItems[nTempRow][nTempCol].InitBtn();
			m_abtnShopItems[nTempRow][nTempCol].SetEmpty( TRUE );

			// Unselect item
			if( m_nSelShopItemID == ( nTempCol + nTempRow * PERSONAL_SHOP_SLOT_COL ) )
				m_nSelShopItemID = -1;
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: Trade 슬롯에서 상점 슬롯으로.
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::TradeToShop( SQUAD llCount, int iSlot )
{
	SQUAD	llNewCount;
	SBYTE	nTab; 
	SBYTE	nRow; 
	SBYTE	nCol; 
	int		nIndex;
	int		nUniIndex;
	SBYTE	nWearingType;
	ULONG	ulPlus;
	ULONG	ulFlag;
	LONG	lUsed;
	LONG	lRareIndex;
	SQUAD	llPrice = 0;

	// 선택된 아이템이 Trade 창에 있던것.
	if( nTempTradeItemID >= 0 )
	{
		// Update count of trade item
		llNewCount	= m_abtnTradeItems[nTempTradeItemID].GetItemCount();
		nTab		= m_abtnTradeItems[nTempTradeItemID].GetItemTab(); 
		nRow		= m_abtnTradeItems[nTempTradeItemID].GetItemRow(); 
		nCol		= m_abtnTradeItems[nTempTradeItemID].GetItemCol();
		nIndex		= m_abtnTradeItems[nTempTradeItemID].GetItemIndex();
		nUniIndex	= m_abtnTradeItems[nTempTradeItemID].GetItemUniIndex();
		nWearingType = m_abtnTradeItems[nTempTradeItemID].GetItemWearType();
		ulPlus		= m_abtnTradeItems[nTempTradeItemID].GetItemPlus();
		ulFlag		= m_abtnTradeItems[nTempTradeItemID].GetItemFlag();
		lUsed		= m_abtnTradeItems[nTempTradeItemID].GetItemUsed();
		llPrice		= m_abtnTradeItems[nTempTradeItemID].GetItemPrice();
		lRareIndex	= m_abtnTradeItems[nTempTradeItemID].GetItemRareIndex();
		
		SQUAD llOld = m_aiTradeItemCount[nTempTradeItemID];
		m_aiTradeItemCount[nTempTradeItemID] -= llCount;		
		if( m_bBuyShop && m_aiTradeItemCount[nTempTradeItemID] < 0)
		{
			m_aiTradeItemCount[nTempTradeItemID] = llOld;
			nTempTradeItemID = -1;
			return;
		}
		
		if( !m_bBuyShop )
		{
			llNewCount -= llCount;
			m_abtnTradeItems[nTempTradeItemID].SetItemCount( llNewCount );
			
			if( llNewCount <= 0 )
			{
				// Rearrange buttons
				for( int iArrItem = nTempTradeItemID; iArrItem < PERSONAL_TRADE_SLOT_TOTAL; ++iArrItem )
				{
					if( ( iArrItem + 1 ) == PERSONAL_TRADE_SLOT_TOTAL )
					{
						m_abtnTradeItems[iArrItem].InitBtn();
						break;
					}
					
					if( m_abtnTradeItems[iArrItem + 1].IsEmpty() )
					{
						m_abtnTradeItems[iArrItem].InitBtn();
						break;
					}
					
					m_abtnTradeItems[iArrItem].SetItemInfo( 0,
						m_abtnTradeItems[iArrItem + 1].GetItemRow(),
						m_abtnTradeItems[iArrItem + 1].GetItemCol(),
						m_abtnTradeItems[iArrItem + 1].GetItemIndex(),
						m_abtnTradeItems[iArrItem + 1].GetItemUniIndex(),
						-1 );
					m_abtnTradeItems[iArrItem].SetItemCount( m_abtnTradeItems[iArrItem + 1].GetItemCount() );
					m_abtnTradeItems[iArrItem].SetItemFlag( m_abtnTradeItems[iArrItem + 1].GetItemFlag() );
					m_abtnTradeItems[iArrItem].SetItemPlus( m_abtnTradeItems[iArrItem + 1].GetItemPlus() );
					m_abtnTradeItems[iArrItem].SetItemPrice( m_abtnTradeItems[iArrItem + 1].GetItemPrice() );
					m_abtnTradeItems[iArrItem].SetItemUsed( m_abtnTradeItems[iArrItem + 1].GetItemUsed() );
					m_abtnTradeItems[iArrItem].SetItemRareIndex( m_abtnTradeItems[iArrItem + 1].GetItemRareIndex() );

					for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
					{
						SBYTE sbOptionType	= m_abtnTradeItems[iArrItem + 1].GetItemOptionType(sbOption);
						SBYTE sbOptionLevel	= m_abtnTradeItems[iArrItem + 1].GetItemOptionLevel(sbOption);
						m_abtnTradeItems[iArrItem].SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
					}
				}
				
				// Unselect item
				if( nTempTradeItemID == m_nSelTradeItemID )
					m_nSelTradeItemID = -1;
			}
		}
	}
	// 패키지에서 선택한 경우. m_bBuyShop일 경우 필요 없음)
	else if( !m_bBuyShop && nTempPackageItemID >= 0 )
	{
		// Update count of Package item
		llNewCount	= m_abtnPackageItems[nTempPackageItemID].GetItemCount();
		nTab		= m_abtnPackageItems[nTempPackageItemID].GetItemTab(); 
		nRow		= m_abtnPackageItems[nTempPackageItemID].GetItemRow(); 
		nCol		= m_abtnPackageItems[nTempPackageItemID].GetItemCol();
		nIndex		= m_abtnPackageItems[nTempPackageItemID].GetItemIndex();
		nUniIndex	= m_abtnPackageItems[nTempPackageItemID].GetItemUniIndex();
		nWearingType = m_abtnPackageItems[nTempPackageItemID].GetItemWearType();
		ulPlus		= m_abtnPackageItems[nTempPackageItemID].GetItemPlus();
		lUsed		= m_abtnPackageItems[nTempPackageItemID].GetItemUsed();
		ulFlag		= m_abtnPackageItems[nTempPackageItemID].GetItemFlag();

		//if( !m_bBuyShop )
		//{
			llNewCount -= llCount;
			m_abtnPackageItems[nTempPackageItemID].SetItemCount( llNewCount );
			
			if( llNewCount <= 0 )
			{
				// Rearrange buttons
				for( int iArrItem = nTempPackageItemID; iArrItem < PERSONAL_PACKAGE_SLOT_COL; ++iArrItem )
				{
					if( ( iArrItem + 1 ) == PERSONAL_PACKAGE_SLOT_COL )
					{
						m_abtnPackageItems[iArrItem].InitBtn();
						break;
					}
					
					if( m_abtnPackageItems[iArrItem + 1].IsEmpty() )
					{
						m_abtnPackageItems[iArrItem].InitBtn();
						break;
					}
					
					m_abtnPackageItems[iArrItem].SetItemInfo( 0,
						m_abtnPackageItems[iArrItem + 1].GetItemRow(),
						m_abtnPackageItems[iArrItem + 1].GetItemCol(),
						m_abtnPackageItems[iArrItem + 1].GetItemIndex(),
						m_abtnPackageItems[iArrItem + 1].GetItemUniIndex(),
						-1 );
					m_abtnPackageItems[iArrItem].SetItemCount( m_abtnPackageItems[iArrItem + 1].GetItemCount() );
					m_abtnPackageItems[iArrItem].SetItemFlag( m_abtnPackageItems[iArrItem + 1].GetItemFlag() );
					m_abtnPackageItems[iArrItem].SetItemPrice( m_abtnPackageItems[iArrItem + 1].GetItemPrice() );
					m_abtnPackageItems[iArrItem].SetItemPlus( m_abtnPackageItems[iArrItem + 1].GetItemPlus() );
					m_abtnPackageItems[iArrItem].SetItemUsed( m_abtnPackageItems[iArrItem + 1].GetItemUsed() );
					m_abtnPackageItems[iArrItem].SetItemRareIndex( m_abtnPackageItems[iArrItem + 1].GetItemRareIndex() );

					for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
					{
						SBYTE sbOptionType	= m_abtnPackageItems[iArrItem + 1].GetItemOptionType(sbOption);
						SBYTE sbOptionLevel	= m_abtnPackageItems[iArrItem + 1].GetItemOptionLevel(sbOption);
						m_abtnPackageItems[iArrItem].SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
					}
				}
				
				// Unselect item
				if( nTempPackageItemID == m_nSelPackageItemID )
					m_nSelPackageItemID = -1;
			}
		//}
	}

	// 판매 모드 일때...
	if( !m_bBuyShop )
	{
		// Update count of shop item
		llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
		llNewCount += llCount;
		/*
		if(m_bBuyShop)
		{
			m_abtnShopItems[nTempRow][nTempCol].SetItemInfo( 0,
					nRow,
					nCol,
					nIndex,
					nUniIndex,
					nWearingType );
			m_abtnShopItems[nTempRow][nTempCol].SetItemPlus( ulPlus );
			m_abtnShopItems[nTempRow][nTempCol].SetItemFlag( ulFlag );
		}
		*/
		m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount > 0 )
			m_abtnShopItems[nTempRow][nTempCol].SetEmpty( FALSE );
	}
	// 구매 모드일때...
	else
	{
		int iRow = -1;
		int iCol = -1;

		FindShopSlot( iRow, iCol, nIndex, ulFlag );

		if( iRow < PERSONAL_SHOP_SLOT_ROW && iCol < PERSONAL_SHOP_SLOT_COL )
		{
			// Check if item is countable
			CItemData&	rItemData = _pNetwork->GetItemData( nIndex );
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				// Update count of trade item
				SQUAD	llNewCount = m_abtnShopItems[iRow][iCol].GetItemCount();
				llNewCount += llCount;
				m_abtnShopItems[iRow][iCol].SetItemCount( llNewCount );
				
				if( !m_bBuyShop )
				{
					// Update count of shop item
					llNewCount = m_abtnShopItems[iRow][iCol].GetItemCount();
					llNewCount -= llCount;
					m_abtnShopItems[iRow][iCol].SetItemCount( llNewCount );
					if( llNewCount <= 0 )
					{
						m_abtnShopItems[iRow][iCol].SetEmpty( TRUE );
						
						// Unselect item
						if( m_nSelShopItemID == ( iCol + iRow * PERSONAL_SHOP_SLOT_COL ) )
							m_nSelShopItemID = -1;
					}
				}
				
				m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
				m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
				_pUIMgr->InsertCommaToString( m_strTotalPrice );
				return;
			}
		}

		int iEmptyRow = 0;
		int iEmptyCol = 0;

		//FindShopSlot( iEmptyRow, iEmptyCol, -1 );
		FindEmptySlot( iEmptyRow, iEmptyCol );

		// If there is not empty slot
		if( iEmptyRow == PERSONAL_SHOP_SLOT_ROW && iEmptyCol == PERSONAL_SHOP_SLOT_COL )
		{
			// Add system message
			if( m_bBuyShop )
				_pUIMgr->GetChatting()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				_pUIMgr->GetChatting()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		// Update count of shop item
		llNewCount = m_abtnShopItems[iEmptyRow][iEmptyCol].GetItemCount();
		llNewCount += llCount;
		
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemInfo( 0,
			nRow,
			nCol,
			nIndex,
			nUniIndex,
			nWearingType );
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemPlus( ulPlus );
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemFlag( ulFlag );					
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemCount( llNewCount );
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemPrice( llPrice );
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemUsed( lUsed );
		m_abtnShopItems[iEmptyRow][iEmptyCol].SetItemRareIndex( lRareIndex );
		if( llNewCount > 0 )
			m_abtnShopItems[iEmptyRow][iEmptyCol].SetEmpty( FALSE );
		
		m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
		m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
		_pUIMgr->InsertCommaToString( m_strTotalPrice );
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::PackageToShop( BOOL bAdd )
{
	/*
	SQUAD	llNewCount;
	SBYTE	nTab; 
	SBYTE	nRow; 
	SBYTE	nCol; 
	int		nIndex;
	int		nUniIndex;
	SBYTE	nWearingType;
	ULONG	ulPlus;
	ULONG	ulFlag;
	*/

	if(bAdd)
	{		
		for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
		{
			for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
			{			
				if( m_abtnShopItems[iRow][iCol].IsEmpty() )
				{	
					m_abtnShopItems[iRow][iCol].SetItemInfo( 0,
						-1,
						-1,
						PACKAGE_ITEM_INDEX,
						-1,
						-1 );								
					m_abtnShopItems[iRow][iCol].SetItemCount( 1 );				
					m_abtnShopItems[iRow][iCol].SetEmpty( FALSE );
					
					m_bPackageBuy	= bAdd;
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					_pUIMgr->InsertCommaToString( m_strTotalPrice );					
					return;
				}
			}
		}
	}
	else
	{
		for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
		{
			for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
			{			
				if( m_abtnShopItems[iRow][iCol].IsEmpty() )
					continue;
				else
				{
					const int iIndex = m_abtnShopItems[iRow][iCol].GetItemIndex();
					if(iIndex == PACKAGE_ITEM_INDEX)
					{
						m_abtnShopItems[iRow][iCol].InitBtn();						
						
						m_bPackageBuy	= bAdd;
						m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
						m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
						_pUIMgr->InsertCommaToString( m_strTotalPrice );						
						return;
					}
				}
			}
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 가격을 계산합니다.
// Input  : iShopID - 
//			bSell - 
// Output : __int64
//-----------------------------------------------------------------------------
__int64 CUIPersonalShop::CalculateTotalPrice(int& iCount)
{
	__int64	iTotalPrice	= 0;
	iCount				= 0;
	
	for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			if( m_abtnShopItems[iRow][iCol].IsEmpty() )
				continue;
			
			const int iIndex		= m_abtnShopItems[iRow][iCol].GetItemIndex();
			const int iUniIndex		= m_abtnShopItems[iRow][iCol].GetItemUniIndex();
			const SQUAD llItemCount	= m_abtnShopItems[iRow][iCol].GetItemCount();
			const SQUAD llItemPrice	= m_abtnShopItems[iRow][iCol].GetItemPrice();
			
			if(iIndex == -1 || iUniIndex == -1)
				continue;
			
			iTotalPrice					+= llItemPrice * llItemCount;
			iCount++;
		}
	}
	if( m_bPackageBuy )
	{
		iTotalPrice		+= m_llPackagePrice;
	}
	
	return iTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iShopID - 
//			&ID - 
//			iNumOfItem - 
//			bSell - 
//-----------------------------------------------------------------------------
__int64	CUIPersonalShop::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell)
{
	if(iShopID <= 0)
		return 0;
	
	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
	const int iShopSellRate	= SD.GetSellRate();
	const int iShopBuyRate	= SD.GetBuyRate();
	__int64	iPrice		= 0;

	if( bSell )
		iPrice = (ID.GetPrice() * iShopBuyRate) / 100;
	else
		iPrice = (ID.GetPrice() * iShopSellRate) / 100;

	iPrice *= iNumOfItem;
	return iPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 판매 개시
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopStart()
{
	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 746, "이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if(_pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 763, "서 있는 상태에서만 개인상점거래를 개설 할 수 있습니다." ), SYSMSG_ERROR );	
		return;
	}

	// 판매시작					: charindex(n) shoptype(c) shopname(str) [normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:client
	// normal_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	// pack_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	SBYTE sbShopType = PST_NOSHOP;

	if( m_ebShopName.GetString() )
	{
		m_strShopName = m_ebShopName.GetString();

		if(m_strShopName.Length() < 4)
		{
			_pUIMgr->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 764, "상점 이름이 너무 짧습니다.\n(한글 2자이상, 영문 4자이상)" );		
			
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			m_ebShopName.ResetString();
			m_ebShopName.SetFocus( TRUE );
			return;
		}
		
		int nIndexBuffer[32];
		char szBuffer[256];

		strcpy ( szBuffer,m_strShopName.str_String );

		// Date : 2005-01-15,   By Lee Ki-hwan
		if( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE )
		{
			_pUIMgr->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			
			CTString	strMessage = _S ( 751, "상점 이름에 잘못된 문자가 포함되어 있습니다." );

			//strMessage += _UIFiltering.GetString ( nIndex );
			/* // Date : 2005-01-15,   By Lee Ki-hwan : 넣어 줘도 되고 안 넣어 줘도 되고
			strMessage += "\n";
			
			for ( int i = 1; i <= nIndexBuffer[0]; i++ )
			{	
				strMessage += "[";	
				strMessage += _UIFiltering.GetString( nIndexBuffer[i] );
				strMessage += "] ";
			}
			*/


			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
					
			m_ebShopName.SetFocus( TRUE );

			return;
		}
		
		/*
		// 공백 체크.
		for(const char *chr = m_strShopName.str_String; *chr != 0; chr++)
		{
			if( (*chr) == ' ' || (*chr) == '\t' || (*chr) == '\n' || (*chr) == '\r' || 
				(*chr) == '%' || (*chr) == '#' || (*chr) == '&' || (*chr) == '?' || (*chr) == '+' || (*chr) == '=')
			{
				_pUIMgr->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
					UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
				CTString	strMessage = _S( 764, "상점 이름이 올바르지 않습니다." );		
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				
				m_ebShopName.ResetString();
				return;
			}
		}
		*/
	}

	int iNormalItemCount	= 0;
	int iPackageItemCount	= 0;

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			continue;
		iNormalItemCount++;
	}

	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if( m_abtnPackageItems[iCol].IsEmpty() )
			continue;
		iPackageItemCount++;
	}

	if( iPackageItemCount > 0 && m_bPremium && m_ebPackagePrice.GetString() )
	{
		CTString strPrice;
		strPrice = m_ebPackagePrice.GetString();

		if(strPrice == "" || strPrice.Length() <= 0)
		{
			_pUIMgr->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
				UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 765, "패키지 가격을 입력해주십시오." );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			m_ebPackagePrice.ResetString();
			return;
		}
		char	*pcInput	= strPrice.str_String;
		int		nLength		= strPrice.Length();
		for( int iChar = 0; iChar < nLength; ++iChar )
		{
			if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
				break;
		}
		
		if( iChar == nLength )
		{
			SQUAD	llCount		= _atoi64( pcInput );
			m_llPackagePrice	= llCount;
		}
	}

	sbShopType |= PST_SELL;

	if(m_bPremium)
	{
		sbShopType |= PST_PREMIUM;	
	}

	if(iPackageItemCount > 0)
	{
		sbShopType |= PST_PACKAGE;	
	}
	
//	UBYTE Maintype;
	
//	Maintype = m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP;

	CNetworkMessage nmPersonalShop(m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP);

		// 판매대행 상인
	if(m_bCashPersonShop)
	{
		nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
		nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_START;
	
	}else
	{
//		Maintype = MSG_PERSONALSHOP;
		nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_START;

	}	

//	if(m_bCashPersonShop) nmPersonalShop << MSG_EX_ALTERNATE_MERCHANT;
	
//	nmPersonalShop << Subtype;
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)sbShopType;
	nmPersonalShop << m_strShopName;

	// Normal Item List
	nmPersonalShop << (SBYTE)iNormalItemCount;	

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			continue;
		nmPersonalShop << (SBYTE)m_abtnTradeItems[iItem].GetItemRow();
		nmPersonalShop << (SBYTE)m_abtnTradeItems[iItem].GetItemCol();
		nmPersonalShop << (LONG)m_abtnTradeItems[iItem].GetItemUniIndex();
		nmPersonalShop << (LONG)m_abtnTradeItems[iItem].GetItemIndex();
		nmPersonalShop << m_abtnTradeItems[iItem].GetItemCount();
		nmPersonalShop << m_abtnTradeItems[iItem].GetItemPrice();
	}
	
	// Package Item List
	nmPersonalShop << m_llPackagePrice;
	nmPersonalShop << (SBYTE)iPackageItemCount;

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	for( iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if( m_abtnPackageItems[iCol].IsEmpty() )
			continue;

		nmPersonalShop << (SBYTE)m_abtnPackageItems[iCol].GetItemRow();
		nmPersonalShop << (SBYTE)m_abtnPackageItems[iCol].GetItemCol();
		nmPersonalShop << (LONG)m_abtnPackageItems[iCol].GetItemUniIndex();
		nmPersonalShop << (LONG)m_abtnPackageItems[iCol].GetItemIndex();
		nmPersonalShop << m_abtnPackageItems[iCol].GetItemCount();
	}

	_pNetwork->SendToServerNew(nmPersonalShop);	
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 중단.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopStop()
{
	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsActionSitting())
	{
		//_pUIMgr->GetChatting()->AddSysMessage( _S( 746, "이동중에는 상점을 닫을 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_CHANGE;	
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)PST_NOSHOP;
	_pNetwork->SendToServerNew(nmPersonalShop);	
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;

	for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			if( m_abtnShopItems[iRow][iCol].IsEmpty() ||
				m_abtnShopItems[iRow][iCol].GetItemIndex() == -1 || 
				m_abtnShopItems[iRow][iCol].GetItemUniIndex() == -1)
				continue;
			iNormalItemCount++;
		}
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			if( m_abtnShopItems[iRow][iCol].IsEmpty() ||
				m_abtnShopItems[iRow][iCol].GetItemIndex() == -1 || 
				m_abtnShopItems[iRow][iCol].GetItemUniIndex() == -1)
				continue;

			nmPersonalShop << (LONG)m_abtnShopItems[iRow][iCol].GetItemUniIndex();
			nmPersonalShop << m_abtnShopItems[iRow][iCol].GetItemCount();
		}
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}
//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendCashPersonShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;

	for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			if( m_abtnShopItems[iRow][iCol].IsEmpty() ||
				m_abtnShopItems[iRow][iCol].GetItemIndex() == -1 || 
				m_abtnShopItems[iRow][iCol].GetItemUniIndex() == -1)
				continue;
			iNormalItemCount++;
		}
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{			
			if( m_abtnShopItems[iRow][iCol].IsEmpty() ||
				m_abtnShopItems[iRow][iCol].GetItemIndex() == -1 || 
				m_abtnShopItems[iRow][iCol].GetItemUniIndex() == -1)
				continue;

			nmPersonalShop << (LONG)m_abtnShopItems[iRow][iCol].GetItemUniIndex();
			nmPersonalShop << m_abtnShopItems[iRow][iCol].GetItemCount();
		}
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ReceivePersonalShopItemList(int iChaIndex, CNetworkMessage *istr) 
{
	// 판매리스트 요청			: charindex(n) [ shoptype(c) normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:server
	// normal_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) price(ll)
	// pack_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)

	//memset(&m_aiTradeItemCount, 0, sizeof(m_aiTradeItemCount));
	ClearItems();

	SBYTE	sbShopType;
	SBYTE	sbNormalCount;
	SBYTE	sbPackageCount;

	//m_nSelectedChaID = iChaIndex;

	(*istr) >> sbShopType
			>> sbNormalCount;

	if(sbShopType & PST_PREMIUM)
	{
		m_bPremium = TRUE;
		m_cbtnPremium.SetCheck(TRUE);
	}
	
	for(int i = 0; i < sbNormalCount; ++i)
	{
		LONG	lIndex;
		LONG	lUniIndex;
		ULONG	ulPlus;
		ULONG	ulFlag;
		SLONG	slUsed;
		SLONG	slUsed2;
		SBYTE	sbOptionCount;
		SBYTE	sbOptionType, sbOptionLevel;
		SQUAD	llCount;
		SQUAD	llPrice;
		CItems	TempItem;

		(*istr) >> lUniIndex;
		(*istr) >> lIndex;		
		(*istr) >> ulPlus;
		(*istr) >> ulFlag;
		(*istr) >> sbOptionCount;

		TempItem.InitOptionData();

		//레어 아이템이면...
		if( _pNetwork->GetItemData(lIndex).GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if( sbOptionCount ==0)
				TempItem.SetRareIndex(0);
			//감정된 레어 아이템
			else
				_pUIMgr->SetRareOption(istr, TempItem);
		}
		//레어 아이템이 아니면...
		else
		{
			for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
			{
				(*istr) >> sbOptionType;
				(*istr) >> sbOptionLevel;
			
				TempItem.SetOptionData( sbOption, sbOptionType, sbOptionLevel );
			}
		}
		(*istr) >> llCount;
		(*istr) >> llPrice;
		(*istr) >> slUsed;
		(*istr) >> slUsed2;

		TempItem.SetData(lIndex, lUniIndex, -1, -1, -1, ulPlus, ulFlag, slUsed, slUsed2, -1, llCount);
		TempItem.SetPrice(llPrice);

		if(_pNetwork->GetItemData(lIndex).GetType() == CItemData::ITEM_ACCESSORY &&
			_pNetwork->GetItemData(lIndex).GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
		}
		
		m_vectorSellItemList.push_back(TempItem);
	}

	if(sbShopType & PST_PACKAGE)
	{
		(*istr) >> m_llPackagePrice;
		(*istr) >> sbPackageCount;	
		for(i = 0; i < sbPackageCount; ++i)
		{
			LONG	lIndex;
			LONG	lUniIndex;
			ULONG	ulPlus;
			ULONG	ulFlag;
			SLONG	slUsed;
			SLONG	slUsed2;
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType, sbOptionLevel;
			SQUAD	llCount;
			CItems	TempItem;
			
			(*istr) >> lUniIndex;
			(*istr) >> lIndex;		
			(*istr) >> ulPlus;
			(*istr) >> ulFlag;
			(*istr) >> sbOptionCount;
			
			TempItem.InitOptionData();
			
			if( _pNetwork->GetItemData(lIndex).GetFlag() & ITEM_FLAG_RARE )
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					TempItem.SetRareIndex(0);
				//감정된 레어 아이템
				else
					_pUIMgr->SetRareOption(istr, TempItem);
			}
			else
			{
				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> sbOptionLevel;
				
					TempItem.SetOptionData( sbOption, sbOptionType, sbOptionLevel );
				}
			}
			(*istr) >> llCount;
			(*istr) >> slUsed;
			(*istr) >> slUsed2;
			
			TempItem.SetData(lIndex, lUniIndex, -1, -1, -1, ulPlus, ulFlag, slUsed, slUsed2, -1, llCount);
			if(_pNetwork->GetItemData(lIndex).GetType() == CItemData::ITEM_ACCESSORY &&
				_pNetwork->GetItemData(lIndex).GetSubType() == CItemData::ACCESSORY_WILDPET)
			{
				_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
			}
			m_vectorSellPackageList.push_back(TempItem);
		}
	}

	m_btnShopCancel.SetVisible(TRUE);
	m_btnShopCancel.SetEnable(TRUE);
	m_btnShopBuy.SetVisible(TRUE);
	m_btnShopBuy.SetEnable(TRUE);
	m_cbtnPremium.SetVisible( FALSE );
	m_cbtnPremium.SetEnable( FALSE );
	//m_btnPackageOK.SetEnable( m_bPremium );
	//m_btnPackageCancel.SetEnable( m_bPremium );
	m_ebShopName.SetEnable( FALSE );
	m_ebPackagePrice.SetEnable( FALSE );

	//m_nSelectedShopID = iShopID;
	m_nSelectedChaID = iChaIndex;

	// Character state flags
	//_pUIMgr->SetCSFlagOn( CSF_PERSONALSHOP );

	_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareBuyShop();
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 갱신
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ReceivePersonalShopItemUpdate(CNetworkMessage *istr)
{
	SBYTE	sbBuyPack		= 0;
	LONG	lChaIndex		= 0;
	SBYTE	sbNormalCount	= 0;

	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	(*istr) >> lChaIndex;	
	(*istr) >> sbBuyPack;
	(*istr) >> sbNormalCount;

	// Package 상품을 팔았을때...
	if(sbBuyPack == 1)
	{
		for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
		{
			m_abtnPackageItems[iCol].InitBtn();
		}

		m_llPackagePrice	= 0;
		m_strPackagePrice	= CTString( "0" );
	}

	RefreshPlayerItem();

	LONG	lItemIndex		= -1;
	SQUAD	llItemCount		= 0;
	for( int i = 0; i < sbNormalCount; ++i)
	{
		(*istr) >> lItemIndex;
		(*istr) >> llItemCount;

		for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if(m_abtnTradeItems[iItem].IsEmpty())
				continue;

			if(m_abtnTradeItems[iItem].GetItemUniIndex() == lItemIndex)
			{
				SQUAD llNewItemCount	= m_abtnTradeItems[iItem].GetItemCount();
				llNewItemCount			-= llItemCount;

				if(llNewItemCount <= 0)
				{
					m_abtnTradeItems[iItem].InitBtn();
				}
				else
				{
					m_abtnTradeItems[iItem].SetItemCount(llNewItemCount);
				}
				break;
			}
		}
	}

	// 상점 윗부분과 아래 인벤토리에 해당하는 부분의 중복을 제거하기 위한 부분.
	// FIXME : 느릴거 같은 코드.  최적화 필요.
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if(m_abtnTradeItems[iItem].IsEmpty())
			continue;

		SQUAD llItemCount	= m_abtnTradeItems[iItem].GetItemCount();
		int iItemUniIndex	= m_abtnTradeItems[iItem].GetItemUniIndex();
		
		// 빈 공간에 추가함.
		for( int iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
		{
			for( int iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
			{
				if( m_abtnShopItems[iRow][iCol].IsEmpty())
					continue;
				
				if( m_abtnShopItems[iRow][iCol].GetItemUniIndex() == iItemUniIndex )
				{
					SQUAD llNewItemCount	= m_abtnShopItems[iRow][iCol].GetItemCount();					
					llNewItemCount			-= llItemCount;
					
					if(llNewItemCount <= 0)
					{
						m_abtnShopItems[iRow][iCol].InitBtn();
					}
					else
					{
						m_abtnShopItems[iRow][iCol].SetItemCount(llNewItemCount);
					}
					break;
				}
			}
		}
	}

	//RefreshPlayerItem();

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendCashPersonOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 패키지 슬롯을 클리어함.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ClearPackageSlot()
{
	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if(m_abtnPackageItems[iCol].IsEmpty())
			continue;

		const SQUAD llOldCount	= m_abtnPackageItems[iCol].GetItemCount();
		const int iOldRow		= m_abtnPackageItems[iCol].GetItemRow();
		const int iOldCol		= m_abtnPackageItems[iCol].GetItemCol();
		SQUAD llNew				= m_abtnShopItems[iOldRow][iOldCol].GetItemCount() + llOldCount;
		m_abtnShopItems[iOldRow][iOldCol].SetItemCount(llNew);
		if( llNew > 0 )
			m_abtnShopItems[iOldRow][iOldCol].SetEmpty( FALSE );

		m_abtnPackageItems[iCol].InitBtn();
	}
	m_ebPackagePrice.ResetString();
	m_llPackagePrice = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 상점의 상태를 변경합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ChangeShopState(BOOL bShopStart)
{
	m_bShopStart = bShopStart;
	m_btnShopStart.SetVisible(!m_bShopStart);
	m_btnShopStart.SetEnable(!m_bShopStart);
	m_btnShopStop.SetVisible(m_bShopStart);						
	m_btnShopStop.SetEnable(m_bShopStart);
	//m_btnPackageOK.SetEnable(m_bPremium & !m_bShopStart);
	//m_btnPackageCancel.SetEnable(m_bPremium & !m_bShopStart);
	//m_cbtnPremium.SetEnable(!m_bShopStart);
	m_ebPackagePrice.SetEnable(!m_bShopStart);
	m_ebShopName.SetEnable(!m_bShopStart);
}

//-----------------------------------------------------------------------------
// Purpose: 인덱스에 해당하는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::FindShopSlot( int& iRow, int& iCol, int iIndex, ULONG ulFlag )
{
	// 빈 공간에 추가함.
	for( iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{
			if( !m_abtnShopItems[iRow][iCol].IsEmpty() && 
				m_abtnShopItems[iRow][iCol].GetItemIndex() == iIndex && 
				m_abtnShopItems[iRow][iCol].GetItemFlag() == ulFlag )
				return;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 비어있는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::FindEmptySlot( int& iRow, int& iCol )
{
	// 빈 공간에 추가함.
	for( iRow = 0; iRow < PERSONAL_SHOP_SLOT_ROW; ++iRow )
	{
		for( iCol = 0; iCol < PERSONAL_SHOP_SLOT_COL; ++iCol )
		{
			if( m_abtnShopItems[iRow][iCol].IsEmpty())
				return;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 구매중에 판매자가 개인상점 닫았다. 구매창을 닫는다.
// Input  : 
//-----------------------------------------------------------------------------
BOOL CUIPersonalShop::EndBuyShop(int nChaID)
{
	if (_pUIMgr->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible())
	{ //구매자 상태이고
		if (m_nSelectedChaID == nChaID)
		{ // 구매하려는 판매 상점이 닫히면 구매창을 닫자
			ResetShop();
			_pUIMgr->SetCSFlagOff( CSF_PERSONALSHOP );
			return TRUE;
		}
	}

	return FALSE;
}
