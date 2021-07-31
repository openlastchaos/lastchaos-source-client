#include "stdh.h"
#include <Engine/Interface/UIProduct.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Itemdata.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UISkillLearn.h>

extern INDEX g_iCountry;
// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProduct()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProduct::CUIProduct()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct::~CUIProduct()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PRODUCT_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = PRODUCT_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );
	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );

	// Product Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PRODUCT_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PRODUCT_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoUL.SetUV( 218, 113, 225, 120, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 228, 113, 230, 120, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 233, 113, 240, 120, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 218, 123, 225, 125, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 228, 123, 230, 125, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 233, 123, 240, 125, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 218, 128, 225, 135, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 228, 128, 230, 135, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 233, 128, 240, 135, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Product button
	m_btnOK.Create( this, _S( 191, "확인" ), 70, 372-diff, 63, 21 );			
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 372-diff, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of Product Item
	m_sbProductItem.Create( this, 193, 51, 9, 153 );
	m_sbProductItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProductItem.SetScrollPos( 0 );
	m_sbProductItem.SetScrollRange( PRODUCT_SLOT_ROW_TOTAL );
	m_sbProductItem.SetCurItemCount( 0 );
	m_sbProductItem.SetItemsPerPage( PRODUCT_SLOT_ROW );	
	// Up button
	m_sbProductItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProductItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProductItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProductItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProductItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProductItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProductItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProductItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProductItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProductItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProductItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProductItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbPreconditionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbPreconditionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbPreconditionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPreconditionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPreconditionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_bSatisfied = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Clear()
{
	m_nSelectProductItem	= -1;
	m_bWaitProductResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_nProductText			= -1;				
	m_nRow					= -1;
	m_nCol					= -1;
	
	m_nNeedItemCount		= 0;
	m_nMakeItemCount		= 0;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	m_StrProductType.Clear ();

	m_bWaitProductResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();

	m_btnProductItems.clear ();
}

// ----------------------------------------------------------------------------
// Name : InitProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::InitProduct()
{	
	CItemData &TextItemData = _pNetwork->GetItemData ( m_nProductText );
	const char* szItemName = _pNetwork->GetItemName( m_nProductText );
	
	// 임시 처리 
	int nProductType = TextItemData.GetProcessType();
	int nProductSubType = TextItemData.GetProcessSubType();

	m_StrProductType = szItemName;
		
	CUIButtonEx UIButtonEx;
	
	m_btnProductItems.clear ();

	int SubTypeBit;
	
	// 제조 문서로 제조 할수 있는 아이템 검색 
	for( int nIndex = 0; nIndex <= _pNetwork->wo_iNumOfItem; nIndex++ )
	{
		CItemData& ItemData = _pNetwork->GetItemData ( nIndex );

		if( ItemData.GetItemIndex() == -1 ) continue;
		
		if ( ItemData.GetFlag() & ITEM_FLAG_MADE )
		{
			SubTypeBit = 0x00000001 << ItemData.GetSubType();		

			if ( ( ItemData.GetType () == nProductType ) && ( SubTypeBit & nProductSubType ) )
			{
				// Date : 2005-01-07,   By Lee Ki-hwan 
				// 포션인 경우에는 등급을 체크 하고, 다른 아이템인 경우에는 레벨을 체크 한다. 
				if ( ItemData.GetType () == CItemData::ITEM_POTION )
				{
					if ( ItemData.GetGrade() != TextItemData.GetGrade() ) continue;
				}
				else
				{
					if ( ItemData.GetLevel() != TextItemData.GetLevel() ) continue;
				}
				
				{
					UIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PRODUCT ); 
					UIButtonEx.SetItemInfo ( 0, 0, 0, ItemData.GetItemIndex(), -1, -1 );
				
					m_btnProductItems.push_back ( UIButtonEx );
				}
			}
		}
	}		

	m_nMakeItemCount = TextItemData.GetMakePotionCount();
		 
	AddString ( _S( 572, "아이템을 선택해 주십시오." ), COLOR_TEXT );		

	m_sbProductItem.SetScrollPos( 0 );
	m_sbProductItem.SetCurItemCount( m_btnProductItems.size()  );

	m_btnOK.SetEnable ( m_bSatisfied );

	m_bShowItemInfo = false;
	m_nCurInfoLines = 0;
	m_bDetailItemInfo = TRUE;
}

// ----------------------------------------------------------------------------
// Name : OpenProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::OpenProduct( int nItemIndex, int nRow, int nCol )
{
	// If this is already exist
	if( IsVisible() ) return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear();

	if( !_pUIMgr->GetInventory()->IsVisible() )
		_pUIMgr->GetInventory()->ToggleVisible();

	m_nProductText	= nItemIndex;
	m_nRow			= nRow;
	m_nCol			= nCol;

	_pUIMgr->RearrangeOrder( UI_PRODUCT, TRUE );
	
	InitProduct ();

	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_PRODUCT );
}

// ----------------------------------------------------------------------------
// Name : CloseProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::CloseProduct()
{
	// Close refine
	_pUIMgr->RearrangeOrder( UI_PRODUCT, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PRODUCT );

	Clear();		
	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Render()
{
// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PRODUCT_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY+PRODUCT_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 571, "제작" ), m_nPosX + PRODUCT_TITLE_TEXT_OFFSETX,				
										m_nPosY + PRODUCT_TITLE_TEXT_OFFSETY, COLOR_TEXT );

	// Render Product Type
	_pUIMgr->GetDrawPort()->PutTextExCX( m_StrProductType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	

	// Product Item Scroll bar
	m_sbProductItem.Render();

	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// Product button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	// Render Product Item
	CTString m_strShortDesc;
	
	
	int	nX = SLEARN_SLOT_SX;
	int nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbProductItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_btnProductItems.size() )
		iEnd = m_btnProductItems.size();
	
	
	CTString strMakePotionCount;

	strMakePotionCount.PrintF ( _S( 766, "%d 개" ), m_nMakeItemCount );	
	
	int OffSetY = 8;

	if ( m_nMakeItemCount > 0 )
	{
		OffSetY -= 5;
	}

	for( int i = m_sbProductItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_btnProductItems[i].IsEmpty() )
		{
			// render Item
			m_btnProductItems[i].SetPos ( nX, nY );
			m_btnProductItems[i].Render();
			
			// render Item desc 
			CItemData& ItemData = _pNetwork->GetItemData ( m_btnProductItems[i].GetIndex() );
			const char* szItemName = _pNetwork->GetItemName( m_btnProductItems[i].GetIndex() );
			
			_pUIMgr->GetDrawPort()->PutTextExCX( szItemName, m_nPosX + 122, m_nPosY + nY + OffSetY, 0xC3C3C3ff );
			
			if ( m_nMakeItemCount > 0 )
				_pUIMgr->GetDrawPort()->PutTextExCX( strMakePotionCount, m_nPosX + 172, m_nPosY + nY + OffSetY + 18, 0xC3C3C3ff );
			
			nY += SLEARN_SLOT_OFFSETY;
		}
	}
	
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
	
	

	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// render sel outline
	if ( m_nSelectProductItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProductItem - m_sbProductItem.GetScrollPos();

		if ( nOffset >= 0 && nOffset < PRODUCT_SLOT_ROW )
		{
			BoxX = m_nPosX + nX - 1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) - 1 ;


			_pUIMgr->GetDrawPort()->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
											m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xffffffff );
		}
	}
	
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	

	RenderItemInfo ();

}


// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::RenderItemInfo ()
{
	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{

		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

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
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}




// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProduct::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

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

				int	iRowS = m_sbProductItem.GetScrollPos();
				int	iRowE = iRowS + PRODUCT_SLOT_ROW;

				if ( m_btnProductItems.size() < iRowE )
				{
					iRowE = m_btnProductItems.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProductItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bShowItem = true;
							ShowItemInfo( true, iRow ) ;					
						}
					}

					if ( !bShowItem ) 
						ShowItemInfo( false, -1 );
				}
				else
					ShowItemInfo( false, -1 );

			// Move refine
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
				// List box of skill desc
			else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
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
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if ( m_lbPreconditionDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}

				_pUIMgr->RearrangeOrder( UI_PRODUCT, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProduct();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendProductReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProduct();

					return WMSG_SUCCESS;
				}
				else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if ( m_lbPreconditionDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProductItem.GetScrollPos();
					int	iRowE = iRowS + PRODUCT_SLOT_ROW;

					if ( m_btnProductItems.size() < iRowE )
					{
						iRowE = m_btnProductItems.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProductItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectProductItem = iRow;

							SelectItem ( m_nSelectProductItem );
												
							return WMSG_SUCCESS;
						}
					}
				}
			}
		
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{ 
					return m_sbProductItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbPreconditionDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
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
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
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
				for( iPos = 1; iPos < nTempLength; iPos++ )
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


// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew )
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

	
	if( nItemIndex >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_btnProductItems[nItemIndex].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_btnProductItems[nItemIndex].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
		//	m_bDetailItemInfo = m_nSelectProductItem == nItemIndex;
			
			if( !GetItemInfo(  nItemIndex, nInfoWidth, nInfoHeight ) )
				m_bShowItemInfo = FALSE;
		}
	}

	// Update item information box
	if( m_bShowItemInfo && bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX = _pUIMgr->GetMaxI() - nInfoWidth;

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
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
// Name : GetItemInfo()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIProduct::GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex = m_btnProductItems[nItemIndex].GetItemIndex();
	
	if( nIndex < 0 )
		return FALSE;

	CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( nIndex );

	// Get item name
	strTemp = szItemName;
	AddItemInfoString( strTemp ); // 리스트에 추가 

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				// Level
				int	nItemLevel = rItemData.GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				// Physical attack
				if( rItemData.GetPhysicalAttack() > 0 )
				{
					strTemp.PrintF( _S( 161, "공격력 : %d" ), rItemData.GetPhysicalAttack() );
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}

				// Magic attack
				if( rItemData.GetMagicAttack() > 0 )
				{
					strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), rItemData.GetMagicAttack() );
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
				// Level
				int	nItemLevel = rItemData.GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				// Physical defense
				if( rItemData.GetPhysicalDefence() > 0 )
				{
					strTemp.PrintF( _S( 163, "방어력 : %d" ), rItemData.GetPhysicalDefence() );
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}

				// Magic defense
				if( rItemData.GetMagicDefence() > 0 )
				{
					strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), rItemData.GetMagicDefence() );
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;
		}

		// Weight
		strTemp.PrintF( _S( 165, "무게 : %d" ), rItemData.GetWeight() );
		AddItemInfoString( strTemp, 0xDEC05BFF );

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( nIndex );
		if( pDesc[0] != NULL )
		{
			strTemp.PrintF( "%s", pDesc );
			AddItemInfoString( strTemp, 0x9E9684FF );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}
	else 
	{
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strItemInfo[0]);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strItemInfo[0].Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 30;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :!!
// ----------------------------------------------------------------------------
void CUIProduct::SelectItem ( int _nIndex )
{
 
	/*  cpp2angel
		제조창을 띄운 상태에서 아이템을 습득 하였을 때 SelectItem()을 호출 하여서 
		현재 선택된 아이템으로 필요 조건을 다시 습득 하기 위한 루틴
	*/
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProductItem;	// 현재 선택된 아이템으로 셋팅

		if ( _nIndex == -1 ) return;	// 그래도 -1이면 Return...
		
	}
	
	BOOL bNeedItem[MAX_MAKE_ITEM_MATERIAL];
	BOOL bNeedItemCount[MAX_MAKE_ITEM_MATERIAL];
	
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;

	/*
	BOOL bNeedSSkill2		= FALSE;
	BOOL bNeedSSkillLevel2	= FALSE;
	*/

	// 데이터 초기화
	memset ( bNeedItem, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );
	memset ( bNeedItemCount, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );

	m_nNeedItemCount = 0;
	m_lbPreconditionDesc.ResetAllStrings();
	
	// 생산 하고자 하는 아이템 모록
	if ( m_btnProductItems[_nIndex].GetIndex() == -1 ) return;
	CItemData& ProductItemData	= _pNetwork->GetItemData ( m_btnProductItems[_nIndex].GetIndex() );
	CItemData& TextItemData		= _pNetwork->GetItemData ( m_nProductText );
	
	if ( ProductItemData.GetNeedSSkillIndex () == -1 ) return;
	CSpecialSkill NeedSSkill	= _pNetwork->GetSSkillData ( ProductItemData.GetNeedSSkillIndex () );

	if ( ProductItemData.GetNeedSSkillCount () == -1 ) return;
	int			nSSkillLevel	= ProductItemData.GetNeedSSkillCount ();


/*
	CSpecialSkill NeedSSkill2;	
	int			nSSkillLevel2;


	if ( ProductItemData.GetType() == CItemData::ITEM_SHIELD )
	{
		if ( ProductItemData.GetNeedSSkillIndex2 () == -1 ) return;
		NeedSSkill2		= _pNetwork->GetSSkillData ( ProductItemData.GetNeedSSkillIndex2 () );
		
		if ( ProductItemData.GetNeedSSkillCount2 () == -1 ) return;
		nSSkillLevel2	= ProductItemData.GetNeedSSkillCount2 ();
	}
*/
// 조건 검색 
	
	// 필요 스킬 ( Preference 채크 바람 )
	bNeedSSkill =  _pUIMgr->GetCharacterInfo()->CheckSSkillByIndex ( NeedSSkill.GetIndex(), nSSkillLevel, &bNeedSSkillLevel );
		
/*
	if ( ProductItemData.GetType() == CItemData::ITEM_SHIELD )
	{
		// second Need Skill Index Check ...
		bNeedSSkill2 =  _pUIMgr->GetCharacterInfo()->CheckSSkillByIndex ( NeedSSkill2.GetIndex(), nSSkillLevel2, &bNeedSSkillLevel2 );	
	}
*/
// 필요 아이템 
	int nIndexTemp = 0;
	CItemData InvenItem;

	// 필요 아이템 종류와 갯수 얻기 
	for ( int nIndex = 0; nIndex < MAX_MAKE_ITEM_MATERIAL; nIndex++ )
	{	
		nIndexTemp = ProductItemData.GetNeedItemIndex( nIndex );

		if( nIndexTemp == -1 )	continue;
		
		m_NeedItems[m_nNeedItemCount].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
		m_NeedItems[m_nNeedItemCount].llCount	= ProductItemData.GetNeedItemCount( nIndex ); 
	
		m_nNeedItemCount++;	
	}

	if ( m_nNeedItemCount == 0 ) return;

	// 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )
	for ( int nTab = 0; nTab < INVEN_SLOT_TAB; nTab++ )
	{
		for ( int nRow = 0 ; nRow < INVEN_SLOT_ROW_TOTAL; nRow++ )
		{
			for ( int nCol = 0 ; nCol < INVEN_SLOT_COL; nCol++ )
			{
				InvenItem = _pNetwork->MySlotItem[nTab][nRow][nCol].ItemData;
				
				for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
				{
					if ( InvenItem.GetItemIndex() == m_NeedItems[nIndex].ItemData.GetItemIndex() )
					{
						bNeedItem[nIndex] = TRUE;
						
						if ( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum >= m_NeedItems[nIndex].llCount )
						{
							bNeedItemCount[nIndex] = TRUE;
						
							m_NeedItems[nIndex].sbMatTab = nTab;
							m_NeedItems[nIndex].sbMatRow = nRow;
							m_NeedItems[nIndex].sbMatCol = nCol;
						}
						
					}
									
				}
			}		
		}
	}


// 조건 출력 
	CTString strTitle;
	m_bSatisfied = TRUE;
	
	// 필요스킬 -------------------------------------------------------------------------------	
	AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE );		
	AddString ( CTString ( NeedSSkill.GetName() ), bNeedSSkill? COLOR_TEXT : COLOR_NONE );

	strTitle.PrintF ( _S( 573, "%s : %d " ), _S( 574, "레벨" ), nSSkillLevel ); 
	AddString ( strTitle, bNeedSSkillLevel? COLOR_TEXT : COLOR_NONE );

/*
	if ( ProductItemData.GetType() ==  CItemData::ITEM_SHIELD )
	{
		AddString ( CTString ( NeedSSkill2.GetName() ), bNeedSSkill2? COLOR_TEXT : COLOR_NONE );

		strTitle.PrintF ( _S( 573, "%s : %d " ), _S( 574, "레벨" ), nSSkillLevel2 );		
		AddString ( strTitle, bNeedSSkillLevel2? COLOR_TEXT : COLOR_NONE );

		m_bSatisfied &= ( bNeedSSkill | bNeedSSkill2 );
	}
	else 
*/
	{
		m_bSatisfied &= bNeedSSkill;		
	}


	// 필요 물품 -------------------------------------------------------------------------------
	AddString ( CTString ( "" ) );		
	AddString ( _S( 575, "필요 물품" ), COLOR_SUB_TITLE );		
		
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		m_bSatisfied &= bNeedItem[nIndex] && bNeedItemCount[nIndex];
	}
	
	
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		const char* szItemName = _pNetwork->GetItemName( m_NeedItems[nIndex].ItemData.GetItemIndex() );
		strTitle.PrintF ( _S( 576, "%s : %d개" ), szItemName, m_NeedItems[nIndex].llCount );
		AddString ( strTitle, bNeedItemCount[nIndex]? COLOR_TEXT : COLOR_NONE );
	}

	m_btnOK.SetEnable ( m_bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProduct::AddString ( CTString& strText, COLOR colText )
{
	int nLength =  strText.Length();

	if( nLength >= _iMaxMsgStringChar )
	{
		nLength -= nLength - _iMaxMsgStringChar;
		
		do
		{
			if ( strText[nLength] == ' ' )
			{
				break;
			}

		} while ( nLength-- );

		CTString strTemp2, strTemp;

		strText.Split( nLength, strTemp2, strTemp );
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbPreconditionDesc.AddString ( 0, strText, colText );
	}

}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProductReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::SendProductReq()
{
	if( m_bWaitProductResult )
		return;

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage(  _S( 943, "스킬 사용중에는 제작을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage(  _S( 944, "순간 이동중에는 제작을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	CItemData& ProductItemData	= _pNetwork->GetItemData ( m_btnProductItems[m_nSelectProductItem].GetIndex() );

	_pNetwork->ProductReq ( m_nRow, m_nCol, m_btnProductItems[m_nSelectProductItem].GetIndex(), m_nNeedItemCount, 
						m_NeedItems );

	m_bWaitProductResult = TRUE;
	
	m_btnOK.SetEnable ( !m_bWaitProductResult );
	//SetEnable ( !m_bWaitProductResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProductRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ProductRep( SBYTE sbResult )
{
	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_PRODUCT_REP );

	// Show result
	CTString	strMessage;

	switch( sbResult )
	{
	case MSG_MAKE_FAIL_SYSTEM:				// 있어서는 안되는 이유로 실패
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_FAIL_MATERIAL:			// 재료 정보 오류
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_FAIL_NO_SSKILL:			// 제작 스킬 없음
		strMessage = _S( 578, "제작에 필요한 제작 스킬이 없습니다." );							
		break;
	case MSG_MAKE_FAIL_ITEM_LEVEL :			// 현재 스킬로 제작 불가능 아이템 (레벨)
		strMessage = _S( 579, "제작에 필요한 스킬 레벨이 부족합니다." );						
		break;
	case MSG_MAKE_FAIL_OVER_WEIGHT:			// 무게 초과  
		strMessage = _S( 705, "무게가 초과하여 제작을 할 수 없습니다." );	
		break;
	case MSG_MAKE_FAIL_PROB:				// 확률 실패
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_SUCCESS:
		strMessage = _S( 580, "제작에 성공 하였습니다." );										
		break;
	}

	CloseProduct ();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 571, "제작" ), UMBS_OK, UI_PRODUCT, MSGCMD_PRODUCT_REP );		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	
	m_bWaitProductResult = FALSE;	
}