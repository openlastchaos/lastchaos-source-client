#include "StdH.h"
#include <Engine/Interface/UICollectBox.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>

//============================================================================================================
// Name : CUICollectBox()
// Desc :
//============================================================================================================
CUICollectBox::CUICollectBox()
{
	m_nSelectItem = -1;
	m_nDropPosition = -1;
	m_nTab = -1;
	m_nRow = -1;
    m_nCol = -1;
}

CUICollectBox::~CUICollectBox()
{
	Destroy();
}

//============================================================================================================
// Name : Create ()
// Desc :
//============================================================================================================
void CUICollectBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_rcTitle.SetRect( 0, 0, UICOLLECTBOX_WIDTH-1, 22 );
	m_rcbtnItems.SetRect( 5, 27, 164, 166 );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtTitleL.SetUV( 0, 0, 21, 21, fTexWidth, fTexHeight );
	m_rtTitleM.SetUV( 22, 0, 51, 21, fTexWidth, fTexHeight );
	m_rtTitleR.SetUV( 198, 0, 215, 21, fTexWidth, fTexHeight );

	m_rtItemSlotL.SetUV( 0, 119, 21, 128, fTexWidth, fTexHeight );
	m_rtItemSlotM.SetUV( 10, 131, 148, 279, fTexWidth, fTexHeight );
	m_rtItemSlotR.SetUV( 194, 119, 215, 128, fTexWidth, fTexHeight );

	m_rtPointDescL.SetUV( 0, 103, 15, 118, fTexWidth, fTexHeight );
	m_rtPointDescM.SetUV( 16, 103, 147, 118, fTexWidth, fTexHeight );
	m_rtPointDescR.SetUV( 200, 103, 215, 118, fTexWidth, fTexHeight );

	m_rtBottomL.SetUV( 0, 94, 4, 96, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 5, 94, 178, 96, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 211, 94, 215, 96, fTexWidth, fTexHeight );

	m_btnClose.Create( this, CTString( "" ), 153, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 1, 230, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 1, 245, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_rtSelectOutline.SetUV( 218, 51, 250, 118, fTexWidth, fTexHeight );

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			m_abtnInsectItem[iRow*4+iCol].Create( this, 22+35*iCol, 27+35*iRow, BTN_SIZE, BTN_SIZE, UI_COLLECTBOX, UBET_ITEM );
		}
	}

	//Su-won		|----->
	m_lbItemInfo.Create( this, 0, 0, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, FALSE );
	m_lbItemInfo.SetEnable(TRUE);
	m_lbItemInfo.SetVisible(FALSE);

	m_iDetailInfo =-1;
	m_bDetailInfo =FALSE;

	m_ptdInfoTexture = CreateTexture( CTString( "Data\\Interface\\Inventory.tex" ) );
	fTexWidth = m_ptdInfoTexture->GetPixWidth();
	fTexHeight = m_ptdInfoTexture->GetPixHeight();

	m_rtInfoL.SetUV( 133, 404, 140, 426, fTexWidth, fTexHeight );
	m_rtInfoM.SetUV( 143, 404, 145, 426, fTexWidth, fTexHeight );
	m_rtInfoR.SetUV( 148, 404, 155, 426, fTexWidth, fTexHeight );
	m_rtInfoUL.SetUV( 133, 404, 140, 411, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 143, 404, 145, 411, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 148, 404, 155, 411, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 133, 414, 140, 416, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 143, 414, 145, 416, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 148, 414, 155, 416, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 133, 419, 140, 426, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 143, 419, 145, 426, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 148, 419, 155, 426, fTexWidth, fTexHeight );
	//Su-won		<-----|
}

//============================================================================================================
// Name : ResetPosition()
// Desc :
//============================================================================================================
void CUICollectBox::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//============================================================================================================
// Name : AdjustPosition()
// Desc :
//============================================================================================================
void CUICollectBox::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );	
}

//============================================================================================================
// Name : Render
// Desc :
//============================================================================================================
void CUICollectBox::Render()
{
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	int nX, nY, nX1, nY1;
	
	nX1 = m_nPosX + 21;
	nY1 = m_nPosY + 21;
	nX = m_nPosX + UICOLLECTBOX_WIDTH - 17;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX1, nY1,
		m_rtTitleL.U0, m_rtTitleL.V0, m_rtTitleL.U1, m_rtTitleL.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX1, m_nPosY, nX, nY1,
		m_rtTitleM.U0, m_rtTitleM.V0, m_rtTitleM.U1, m_rtTitleM.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX, m_nPosY, m_nPosX+UICOLLECTBOX_WIDTH-1, nY1,
		m_rtTitleR.U0, m_rtTitleR.V0, m_rtTitleR.U1, m_rtTitleR.V1, 0xFFFFFFFF );

	nY = nY1;
	nY1 = nY + 149;
	nX = m_nPosX+22;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY1,
		m_rtItemSlotL.U0, m_rtItemSlotL.V0, m_rtItemSlotL.U1, m_rtItemSlotL.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX+139, nY1,
		m_rtItemSlotM.U0, m_rtItemSlotM.V0, m_rtItemSlotM.U1, m_rtItemSlotM.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX+139, nY, m_nPosX+UICOLLECTBOX_WIDTH-1, nY1,
		m_rtItemSlotR.U0, m_rtItemSlotR.V0, m_rtItemSlotR.U1, m_rtItemSlotR.V1, 0xFFFFFFFF );

	nY = nY1;
	nY1 = nY + 19;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+15, nY1,
		m_rtPointDescL.U0, m_rtPointDescL.V0, m_rtPointDescL.U1, m_rtPointDescL.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+15, nY, m_nPosX+UICOLLECTBOX_WIDTH-16, nY1,
		m_rtPointDescM.U0, m_rtPointDescM.V0, m_rtPointDescM.U1, m_rtPointDescM.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+UICOLLECTBOX_WIDTH-16, nY, m_nPosX+UICOLLECTBOX_WIDTH-1, nY1,
		m_rtPointDescR.U0, m_rtPointDescR.V0, m_rtPointDescR.U1, m_rtPointDescR.V1, 0xFFFFFFFF );

	nY = nY1;
	nY1 = nY + 2;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+4, nY1,
		m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+4, nY, m_nPosX+4+174, nY1,
		m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+4+174, nY, m_nPosX+UICOLLECTBOX_WIDTH-1, nY1,
		m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1, 0xFFFFFFFF );

	m_btnClose.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	for( int iRow = 0; iRow < 4; iRow++ )
	for( int iCol = 0; iCol < 4; iCol++ )
	{
		if( m_abtnInsectItem[(iRow*4)+iCol].IsEmpty() ) continue;

		m_abtnInsectItem[(iRow*4)+iCol].Render();
	}

	if( m_nSelectItem >= 0 )
	{
		m_abtnInsectItem[m_nSelectItem].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX+BTN_SIZE, nY+BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );	

	_pUIMgr->GetDrawPort()->PutTextExCX( m_strTitle, m_nPosX+(UICOLLECTBOX_WIDTH/2), m_nPosY+5, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strPoint, m_nPosX+(UICOLLECTBOX_WIDTH/2), m_nPosY+173, 0x6BD2FFFF );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	if( m_lbItemInfo.IsVisible() )
		RenderInfo();	
}

//============================================================================================================
// CUICollectBox::OpenCollectBox
// Desc :
//============================================================================================================
void CUICollectBox::OpenCollectBox( int nTab, int nRow, int nCol )
{
	if( IsVisible() )	return;

	Init();

	m_nTab = nTab;
	m_nRow = nRow;
    m_nCol = nCol;

	UpDateItem();

	_pUIMgr->RearrangeOrder( UI_COLLECTBOX, TRUE );
}

//============================================================================================================
// CUICollectBox::Init
// Desc :
//============================================================================================================
void CUICollectBox::Init( void )
{
	for( int iRow = 0; iRow < 4; iRow++ )
	for( int iCol = 0; iCol < 4; iCol++ )
	{
		m_abtnInsectItem[iRow*4+iCol].InitBtn();
	}

	m_nSelectItem = -1;
	m_nDropPosition = -1;
	m_nTab = -1;
	m_nRow = -1;
    m_nCol = -1;
}

//============================================================================================================
// CUICollectBox::SetBtnItem
// Desc :
//============================================================================================================
void CUICollectBox::SetBtnItem( int num, int nIndex )
{
	if( num < 1 || num > 11 ) { return; }
	if ( num == 11 ) { num = 8; }

	int nItemIndex = 1578;
	
	nItemIndex += num;
	// ( 1579 ~ 1588 );
	CItemData& ItemData = _pNetwork->GetItemData( nItemIndex );

	m_abtnInsectItem[nIndex].SetItemInfo( 0, 0, 0, nItemIndex, -1, -1, -1, -1, ItemData.GetName(),
		ItemData.GetDesc(), -1, -1 );
}

//============================================================================================================
// CUICollectBox::MouseMessage
// Desc :
//============================================================================================================
WMSG_RESULT CUICollectBox::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT wmsg_Result;

	static BOOL bTitleBarClick = FALSE;
	static BOOL bLButtonDownInItem = FALSE;

	// Mouse point
	static int nOldX, nOldY;
	int nX = LOWORD( pMsg->lParam );
	int nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int ndX = nX - nOldX;
			int ndY = nY - nOldY;

			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				if( m_nSelectItem >= 0 )
				{
					_pUIMgr->CloseMessageBox( MSGCMD_INSECTITEM_DROP );

					_pUIMgr->SetHoldBtn( m_abtnInsectItem[m_nSelectItem] );
					int nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnInsectItem[m_nSelectItem].SetBtnState( UBES_IDLE );
				}

				bLButtonDownInItem = FALSE;
			}

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX; nOldY = nY;
				Move( ndX, ndY );
				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( IsInsideRect( nX, nY, m_rcbtnItems ) )
			{
				//Su-won
				for( int i=0; i<16; i++ )
				{
					if( m_abtnInsectItem[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_lbItemInfo.ResetAllStrings();
						m_lbItemInfo.AddString( 0, m_abtnInsectItem[i].GetCashName(), 0xF2F2F2FF, FALSE);
						
						if( m_iDetailInfo ==i )
						{
							m_bDetailInfo =TRUE;
							CTString strWeight;
							strWeight.PrintF( _S(165, "무게 : %d"), 0);
							m_lbItemInfo.AddString( 0, strWeight, 0xDEC05BFF, FALSE);
							m_lbItemInfo.AddString( 0, m_abtnInsectItem[i].GetCashDesc(), 0x9E9684FF, FALSE);
						}
						else
							m_bDetailInfo =FALSE;

						m_lbItemInfo.SetPos( m_abtnInsectItem[i].GetPosX(), m_abtnInsectItem[i].GetPosY() -m_lbItemInfo.GetCurItemCount(0)*m_lbItemInfo.GetLineHeight() -5 );

						m_lbItemInfo.SetVisible(TRUE);
						return WMSG_SUCCESS;
					}
				}
				m_lbItemInfo.SetVisible(FALSE);
				return WMSG_SUCCESS;
			}
			else
				m_lbItemInfo.SetVisible(FALSE);

		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX; nOldY = nY;

				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{

				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcbtnItems ) )
				{
					for( int i=0; i<16; i++ )
					{
						if( m_abtnInsectItem[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectItem = i;
							bLButtonDownInItem = TRUE;
							_pUIMgr->RearrangeOrder( UI_COLLECTBOX, TRUE );

							if( m_iDetailInfo !=i )
							{
								m_iDetailInfo =i;	//Su-won
								m_bDetailInfo =TRUE;
								CTString strWeight;
								strWeight.PrintF( _S(165, "무게 : %d"), 0);
								m_lbItemInfo.AddString( 0, strWeight, 0xDEC05BFF, FALSE);
								m_lbItemInfo.AddString( 0, m_abtnInsectItem[i].GetCashDesc(), 0x9E9684FF, FALSE);

								m_lbItemInfo.SetPos( m_abtnInsectItem[i].GetPosX(), m_abtnInsectItem[i].GetPosY() -m_lbItemInfo.GetCurItemCount(0)*m_lbItemInfo.GetLineHeight() -5 );
							}

							return WMSG_SUCCESS;
						}
					}
				}

				_pUIMgr->RearrangeOrder( UI_COLLECTBOX, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bLButtonDownInItem = FALSE;

			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				bTitleBarClick = FALSE;

				if( !IsFocused() ) return WMSG_FAIL;

				if( ( wmsg_Result = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsg_Result == WMSG_COMMAND )
						_pUIMgr->RearrangeOrder( UI_COLLECTBOX, FALSE );
					
					return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcbtnItems ) )
				{
					for( int i=0; i<16; i++ )
					{
						if( m_abtnInsectItem[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
				}
			}
			else
			{
				if( !IsInside( nX, nY ) )
				{
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_COLLECTBOX )
					{
						DropInsectItem( _pUIMgr->GetHoldBtn().GetCashName(), m_nSelectItem );
					}
				}
				
				_pUIMgr->ResetHoldBtn();
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( IsInsideRect( nX, nY, m_rcbtnItems ) )
				{
					for( int i=0; i<16; i++ )
					{
						if( m_abtnInsectItem[i].MouseMessage( pMsg ) == WMSG_COMMAND )
						{
							DropInsectItem( m_abtnInsectItem[i].GetCashName(), i );
							return WMSG_SUCCESS;
						}
					}
				}
			}
			
			return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

//============================================================================================================
// CUICollectBox::DropInsectItem
// Desc :
//============================================================================================================
void CUICollectBox::DropInsectItem( CTString Name, int nPosition )
{
	if(_pNetwork->m_bSingleMode)
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 529, "아이템을 버릴 수 없는 지역입니다." ), SYSMSG_ERROR );
		return;
	}

	m_nDropPosition = nPosition;

	if( _pUIMgr->DoesMessageBoxExist( MSGCMD_INSECTITEM_DROP ) )
	{
		return;
	}

	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2936, "곤충 채집 상자" ), UMBS_OKCANCEL, UI_COLLECTBOX, MSGCMD_INSECTITEM_DROP );
	strMessage.PrintF( _S( 2937, "%s<를> 버리면 파괴 됩니다. 계속 하시겠습니까?" ), Name );
	MsgBoxInfo.AddString( strMessage );
	
	_pUIMgr->CreateMessageBox( MsgBoxInfo );	
}

//============================================================================================================
// CUICollectBox::MsgBoxCommand
// Desc:
//============================================================================================================
void CUICollectBox::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_INSECTITEM_DROP:
		if( bOK )
		{
			m_abtnInsectItem[m_nDropPosition].InitBtn();
			_pNetwork->SendDropInsect( m_nDropPosition );
		}
		break;
	}
}

//============================================================================================================
// CUICollectBox::GetInsectName
// Desc :
//============================================================================================================
CTString CUICollectBox::GetInsectName( int n )
{
	CItemData& ItemData = _pNetwork->GetItemData( n );

	CTString strMessage;
	strMessage.PrintF( _S( 2938, "%s를(을) 채집하였습니다." ), ItemData.GetName() );

	return strMessage;
}

//============================================================================================================
// CUICollectBox::UpDateItem
// Desc :
//============================================================================================================
void CUICollectBox::UpDateItem( void )
{
	CItems		&rItems = _pNetwork->MySlotItem[m_nTab][m_nRow][m_nCol];	

	m_strTitle = rItems.ItemData.GetName();

	m_strPoint.PrintF( _S( 2939, "합계: %d 포인트" ), rItems.Item_Flag );

	ULONG ulTemp;
	ULONG ulHiBit = rItems.Item_Plus;
	ULONG ulLowBit = rItems.Item_Used;

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

void CUICollectBox::RenderInfo()
{
	int Left =m_lbItemInfo.GetAbsPosX();
	int Right =Left +m_lbItemInfo.GetWidth();
	int Top = m_lbItemInfo.GetAbsPosY();
	int Bottom = Top +m_lbItemInfo.GetCurItemCount(0)*m_lbItemInfo.GetLineHeight() +9;
	

	//_pUIMgr->GetDrawPort()->AddTexture( AbsPosX, AbsPosY,
	//										AbsPosX +Width, AbsPosY +Height,
	//										0, 0, 1, 1, 0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdInfoTexture );
	if( m_bDetailInfo )
		{
			_pUIMgr->GetDrawPort()->AddTexture( Left, Top,
												Left + 7, Top + 7,
												m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left + 7, Top,
												Right - 7, Top + 7,
												m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Right - 7, Top,
												Right, Top + 7,
												m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left, Top + 7,
												Left + 7, Bottom - 7,
												m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left + 7, Top + 7,
												Right - 7, Bottom - 7,
												m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Right - 7, Top + 7,
												Right, Bottom - 7,
												m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left, Bottom - 7,
												Left + 7, Bottom,
												m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left + 7, Bottom - 7,
												Right - 7, Bottom,
												m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Right - 7, Bottom - 7,
												Right, Bottom,
												m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
												0xFFFFFFFF );
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( Left, Top,
												Left + 7, Bottom,
												m_rtInfoL.U0, m_rtInfoL.V0, m_rtInfoL.U1, m_rtInfoL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Left + 7, Top,
												Right - 7, Bottom,
												m_rtInfoM.U0, m_rtInfoM.V0, m_rtInfoM.U1, m_rtInfoM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( Right - 7, Top,
												Right, Bottom,
												m_rtInfoR.U0, m_rtInfoR.V0, m_rtInfoR.U1, m_rtInfoR.V1,
												0xFFFFFFFF );
		}
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	m_lbItemInfo.Render();

	_pUIMgr->GetDrawPort()->EndTextEx();
}