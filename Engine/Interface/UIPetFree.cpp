#include "stdh.h"
#include <Engine/Interface/UIPetFree.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/ItemData.h>
#include <algorithm>

extern INDEX g_iCountry;

static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIPetFree()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPetFree::CUIPetFree()
{
	m_nStringCount			= 0;
	m_strPetFreeMoney	= CTString( "" );
	m_llPetFreeMoney	= 0;

	m_slLevel				= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetFree()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPetFree::~CUIPetFree()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = 190 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcItemSlot.SetRect( 97, 0, 131, 0 );
	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create refine texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtMiddle3.SetUV( 36, 86, 252, 104, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );

	// Input box
	m_rtInputBoxL.SetUV( 205, 46, 209, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 211, 46, 213, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 215, 46, 219, 59, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 2440, "봉인해제" ), 78, 244, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 146, 244, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_rcItemSlot.Top		= 51;
	m_rcItemSlot.Bottom		= m_rcItemSlot.Top + 34;

	m_rcInsertItem.Top		= 33;
	m_rcInsertItem.Bottom	= m_rcItemSlot.Bottom + 33;

	// Slot item button
	if( g_iCountry == TAIWAN || g_iCountry == TAIWAN2 || g_iCountry == JAPAN || g_iCountry == HONGKONG ) // 대만 일때
	{
		m_rcItemSlot.Left += 15;
		m_rcItemSlot.Right += 15;
	}

	m_btnSlotItem.Create( this, m_rcItemSlot.Left + 1, m_rcItemSlot.Top + 1, BTN_SIZE, BTN_SIZE, UI_CHANGEWEAPON, UBET_ITEM );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenPetFree()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::OpenPetFree()
{
	// If this is already exist
	if( IsVisible() )
		return;

	m_btnOK.SetEnable( FALSE );

	// Set position of target npc
	_pUIMgr->RearrangeOrder( UI_PETFREE, TRUE );

}

// ----------------------------------------------------------------------------
// Name : ClosePetFree()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::ClosePetFree()
{
	// Reset slot item
	m_btnSlotItem.InitBtn();
	m_slLevel				= -1;

	// Close refine
	_pUIMgr->RearrangeOrder( UI_PETFREE, FALSE );

	m_strPetFreeMoney.Clear();
	m_btnOK.SetEnable( FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::Render()
{
	// Set refine texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
		m_rtTop.U0, m_rtTop.V0,
		m_rtTop.U1, m_rtTop.V1,
		0xFFFFFFFF );
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 113,
		m_rtMiddle1.U0, m_rtMiddle1.V0,
		m_rtMiddle1.U1, m_rtMiddle1.V1,
		0xFFFFFFFF );

	nY += 113;
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 11,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );

	nY += 11;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 90,
		m_rtMiddle3.U0, m_rtMiddle3.V0,
		m_rtMiddle3.U1, m_rtMiddle3.V1,
		0xFFFFFFFF );

	nY += 90;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 4,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );
	
	nY += 4;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 10,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );
	
	nY += 10;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 30,
		m_rtBottom.U0, m_rtBottom.V0,
		m_rtBottom.U1, m_rtBottom.V1,
		0xFFFFFFFF );


	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcItemSlot.Left, m_nPosY + m_rcItemSlot.Top,
										m_nPosX + m_rcItemSlot.Right, m_nPosY + m_rcItemSlot.Bottom,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
										0xFFFFFFFF );


	nX	= m_nPosX + 95;
	nX2	= nX + 84;
	nY	= m_nPosY + 102;
	nY2 = nY + 13;

	// Point    금액 박스 wooss 051011
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 4, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	//Lower middle
		_pUIMgr->GetDrawPort()->AddTexture( nX + 4, nY, nX2 - 4, nY2,
			m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
			0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 4, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );
	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Item
	if( !m_btnSlotItem.IsEmpty() )
	{
		m_btnSlotItem.Render();
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	CTString szItemName = CTString(	_S( 2441, "봉인된 펫을 부활시킵니다." ) );
	_pUIMgr->GetDrawPort()->PutTextEx( szItemName, m_nPosX+15, m_nPosY+162, 0xFFFFFFFF );
	szItemName = CTString( _S( 2442, "레벨에 따라 부활비용을 지불해야" ) );
	_pUIMgr->GetDrawPort()->PutTextEx( szItemName, m_nPosX+15, m_nPosY+182, 0xFFFFFFFF );
	szItemName = CTString( _S( 2443, "합니다." ) );
	_pUIMgr->GetDrawPort()->PutTextEx( szItemName, m_nPosX+15, m_nPosY+202, 0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->PutTextEx( _S( 2444, "펫 봉인해제" ), m_nPosX + UI_PETFREE_TITLE_TEXT_OFFSETX,		
										m_nPosY + UI_PETFREE_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->PutTextEx( _S( 2444, "펫 봉인해제" ), m_nPosX + 20,		
										m_nPosY + 60, 0xFFFFFFFF );

	// cash item check wooss 051011
	nY = m_nPosY + 104;
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1058, "필요나스" ), m_nPosX + 40,		
									nY, 0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->PutTextEx( m_strPetFreeMoney,	m_nPosX + 95, m_nPosY + 102, 0xE1B300FF );				

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetFree::MouseMessage( MSG *pMsg )
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

				_pUIMgr->RearrangeOrder( UI_PETFREE, TRUE );
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
						ClosePetFree();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						SendPetFreeReq();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ClosePetFree();
					}

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and comes from inventory
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
					{
						if( IsInsideRect( nX, nY, m_rcInsertItem ) )
							SetFreePetItem();  // 
					}

					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( IsInsideRect( nX, nY, m_rcItemSlot ) )
				{
					m_btnSlotItem.InitBtn();
					m_strPetFreeMoney.Clear();
					m_btnOK.SetEnable( FALSE );
				}
			}
				return WMSG_SUCCESS;
		}
		break;
		
	case WM_MOUSEWHEEL:
		{

		}
		break;
	}

	return WMSG_FAIL;
}

// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
// ----------------------------------------------------------------------------
// Name : SetFreePetItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::SetFreePetItem()
{
	int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();

	CItems		&m_selItem = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = m_selItem.ItemData;
	if( !(rItemData.GetSubType() == CItemData::ACCESSORY_PET || rItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2445, "펫 아이템만 가능합니다." ), SYSMSG_ERROR );		
		return;
	}

	if(rItemData.GetSubType() == CItemData::ACCESSORY_PET )
	{
		m_nPettype = CItemData::ACCESSORY_PET;
		CNetworkLibrary::sPetInfo	TempPet;
		TempPet.lIndex				= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus; // 펫 식별인덱스
		m_slPetIndex = TempPet.lIndex; // 펫 식별 인덱스 저장
		std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		if( iter != _pNetwork->m_vectorPetList.end() )	// 펫을 찾았다.
		{
			m_slLevel = (*iter).lLevel;
			m_llPetFreeMoney = CalculateNeedNas( m_slLevel );
			
		}
		else
			return;


	}else if(rItemData.GetSubType() == CItemData::ACCESSORY_WILDPET )
	{
		m_nPettype = CItemData::ACCESSORY_WILDPET;
		sPetItem_Info temPet;
		_pUIMgr->GetWildPetInfo()->GetWildPetInfo(_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus,temPet);
		m_slLevel = temPet.pet_level;
		m_llPetFreeMoney = (m_slLevel-1)*((m_slLevel*m_slLevel*8)+20000);
	}
/*	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus; // 펫 식별인덱스
	m_slPetIndex = TempPet.lIndex; // 펫 식별 인덱스 저장
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != _pNetwork->m_vectorPetList.end() )	// 펫을 찾았다.
	{
		m_slLevel = (*iter).lLevel;
	}
	else
		return;*/

	// Insert upgrade slot
	m_btnSlotItem.Copy( _pUIMgr->GetHoldBtn() );
	
	
	m_strPetFreeMoney.PrintF( "%I64d", m_llPetFreeMoney );
	m_btnOK.SetEnable( TRUE );
}

// ----------------------------------------------------------------------------
// Name : CalculateNeedNas()
// Desc : 봉인 해제를 위한 나스 계산
// ----------------------------------------------------------------------------
SQUAD CUIPetFree::CalculateNeedNas( int iPetLevel )
{
	if( iPetLevel < 0 )
		return 0;

	SQUAD llPrice = 0;
	llPrice = ( iPetLevel - 1 ) * ( iPetLevel * iPetLevel * 7 );

	return llPrice;
}

// ----------------------------------------------------------------------------
// Name : SendPetFreeReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::SendPetFreeReq()
{
	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2446, "스킬 사용중에는 펫의 봉인을 해제 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2447, "순간 이동중에는 봉인을 해제 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( m_btnSlotItem.IsEmpty() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2448, "봉인 해제 할 펫이 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if(m_nPettype == CItemData::ACCESSORY_PET)
	{// 네트 워크 메세지 ~~
		_pNetwork->SendPetRebirth( m_slPetIndex );
	}else if(m_nPettype == CItemData::ACCESSORY_WILDPET)
	{
		int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
		int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();
		_pNetwork->SendWildPetRebirth( nRow, nCol);
	}
}

// ----------------------------------------------------------------------------
// Name : PetFreeError()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetFree::PetFreeError( SLONG PetIndex, SBYTE sbResult )
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	if( PetIndex != m_slPetIndex )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_PETFREE, UI_NONE );
		strMessage.PrintF( _S( 2449, "봉인해제를 요청하신 펫과 슬롯의 펫이 다릅니다.") );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		return;
	}

	switch( sbResult )
	{
	case MSG_EX_PET_REBIRTH_ERROR_OK:			// 성공
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_PETFREE, UI_NONE );
		strMessage.PrintF( _S( 2450, "펫의 봉인을 해제하는데 성공하였습니다.") );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		ClosePetFree();
		break;
	case MSG_EX_PET_REBIRTH_ERROR_NOMONEY:		// 나스 부족
		_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2451, "펫의 봉인을 해제하기 위한 비용이 부족합니다." ), SYSMSG_ERROR );		
		break;
	case MSG_EX_PET_REBIRTH_ERROR_NOPET:		// 펫 존재 안함
		_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2452, "봉인을 해제할 펫이 존재하지 않습니다." ), SYSMSG_ERROR );				
		break;
	case MSG_EX_PET_REBIRTH_ERROR_NODEAD:		// 펫 살아있음
		_pUIMgr->GetChatting()->AddSysMessage( 
						_S( 2453, "펫이 봉인되지 않았습니다." ), SYSMSG_ERROR );
		break;
	}
}
