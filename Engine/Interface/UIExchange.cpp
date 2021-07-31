#include "stdh.h"
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/PetInfo.h>
#include <Engine/LocalDefine.h>


extern INDEX g_iCountry;
extern INDEX g_iRejectExchange;	// [7/13/2009 rumist] reject request of exchange message.
// ----------------------------------------------------------------------------
// Name : CUIExchange()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIExchange::CUIExchange()
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
	m_strMyCurMoney = CTString( "0" );
	m_strOppExchMoney = CTString( "0" );
	m_strMyExchMoney = CTString( "0" );
}

// ----------------------------------------------------------------------------
// Name : ~CUIExchange()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIExchange::~CUIExchange()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcOppSlot.SetRect( 6, 27, 191, 142 );
	m_rcMySlot.SetRect( 6, 149, 191, 245 );
	m_rcInvenSlot.SetRect( 6, 253, 192, 417 );

	// Create exchange texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Exchange.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground.SetUV( 0, 0, 216, 452, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoL.SetUV( 0, 478, 7, 500, fTexWidth, fTexHeight );
	m_rtInfoM.SetUV( 10, 478, 12, 500, fTexWidth, fTexHeight );
	m_rtInfoR.SetUV( 15, 478, 22, 500, fTexWidth, fTexHeight );
	m_rtInfoUL.SetUV( 0, 478, 7, 485, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 10, 478, 12, 485, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 15, 478, 22, 485, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 488, 7, 490, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 10, 488, 12, 490, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 15, 488, 22, 490, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 493, 7, 500, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 10, 493, 12, 500, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 15, 493, 22, 500, fTexWidth, fTexHeight );

	// Outline of item that isn't exchangeable
	m_rtWearingOutline.SetUV( 218, 115, 250, 147, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Ready button
	m_btnReady.Create( this, _S( 148, "준비 완료" ), 11, 420, 63, 21 );
	m_btnReady.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnReady.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnReady.SetUV( UBS_DISABLE, 128, 454, 191, 475, fTexWidth, fTexHeight );
	m_btnReady.CopyUV( UBS_IDLE, UBS_ON );	

	// Exchange button
	m_btnExchange.Create( this, _S( 149, "교환" ), 76, 420, 63, 21 );
	m_btnExchange.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnExchange.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnExchange.SetUV( UBS_DISABLE, 128, 454, 191, 475, fTexWidth, fTexHeight );
	m_btnExchange.CopyUV( UBS_IDLE, UBS_ON );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 420, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar
	m_sbScrollBar.Create( this, 195, 249, 9, 137 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( EXCH_INVEN_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetCurItemCount( EXCH_INVEN_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetItemsPerPage( EXCH_INVEN_SLOT_ROW );
	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -193, -5, 192, 145 );
	

	// Opposite items
	int	iBtn;
	int	nBtnX = EXCH_SLOT_SX;
	int	nBtnY = EXCH_OPP_SLOT_SY;
	for( iBtn = 0; iBtn < EXCH_EXCH_SLOT_TOTAL; iBtn++ )
	{
		if( iBtn == EXCH_EXCH_SLOT_COL )
		{
			nBtnX = EXCH_SLOT_SX;	nBtnY += 35;
		}

		m_abtnOppItems[iBtn].Create( this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM );
		nBtnX += 35;
	}

	// Opposite items
	nBtnX = EXCH_SLOT_SX;
	nBtnY = EXCH_MY_SLOT_SY;
	for( iBtn = 0; iBtn < EXCH_EXCH_SLOT_TOTAL; iBtn++ )
	{
		if( iBtn == EXCH_EXCH_SLOT_COL )
		{
			nBtnX = EXCH_SLOT_SX;	nBtnY += 35;
		}

		m_abtnMyItems[iBtn].Create( this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM );
		nBtnX += 35;
	}

	// Slot items
	for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL ; iRow++ )
	{
		for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
		{
			m_abtnInvenItems[iRow][iCol].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM, 0, iRow, iCol );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : PrepareExchange()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::PrepareExchange()
{
	if(_pUIMgr->IsCSFlagOn(CSF_WAREHOUSE))
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(1890 ,"창고 사용중에는 교환을 할수 없습니다." ) );	
		_pUIMgr->GetWareHouse()->ResetWareHouse();
	}

	// Close message box of inventory
	_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_EXCHANGE );

	// Initialize inventory items of exchange
	int		iRow, iCol;
	for( iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
	{
		for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
		{
			CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			if( rItems.Item_Sum > 0 )
			{
				m_abtnInvenItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, rItems.Item_Index,
															rItems.Item_UniIndex, rItems.Item_Wearing );
				m_abtnInvenItems[iRow][iCol].SetItemCount( rItems.Item_Sum );
				m_abtnInvenItems[iRow][iCol].SetItemPlus( rItems.Item_Plus );
				m_abtnInvenItems[iRow][iCol].SetItemFlag( rItems.Item_Flag );
				m_abtnInvenItems[iRow][iCol].SetItemUsed( rItems.Item_Used );
				m_abtnInvenItems[iRow][iCol].SetItemUsed2( rItems.Item_Used2 );
				m_abtnInvenItems[iRow][iCol].SetItemRareIndex( rItems.Item_RareIndex);

				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
				{
					m_abtnInvenItems[iRow][iCol].SetItemOptionData( sbOption,
																	rItems.GetOptionType( sbOption ),
																	rItems.GetOptionLevel( sbOption ) );
				}
			}
		}
	}

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strMyCurMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strMyCurMoney );
	}

	// Set state of controls
	m_btnReady.SetEnable( TRUE );
	m_btnExchange.SetEnable( FALSE );

	_pUIMgr->RearrangeOrder( UI_EXCHANGE, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetExchange()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ResetExchange()
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
	m_strMyCurMoney = CTString( "0" );
	m_strOppExchMoney = CTString( "0" );
	m_strMyExchMoney = CTString( "0" );

	m_sbScrollBar.SetScrollPos( 0 );

	// Initialize buttons
	for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		m_abtnOppItems[iItem].InitBtn();
		m_abtnMyItems[iItem].InitBtn();
	}
	for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
			m_abtnInvenItems[iRow][iCol].InitBtn();
	}

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_EXCHANGE );

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_EXCHANGE );

	// Close message box of exchange
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );

	_pUIMgr->RearrangeOrder( UI_EXCHANGE, FALSE );
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return;

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
				for( iPos = 1; iPos < nTempLength; iPos++ )
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

extern tm* g_tv_t; // 기간 표시용 전역 포인터 변수

#define IN_VALIDTM(a) if (a) {delete a; a=NULL;}

// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc : nWhichSlot - 0: opposite slot, 1: my slot, 2: inventory slot
// ----------------------------------------------------------------------------
// FIXME : 중복되는 코드가 너무 많음.
void CUIExchange::GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight,
								int nOppItem, int nMyItem, int nRow, int nCol )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex;
	SQUAD		llCount;
	ULONG		ulPlus, ulFlag;
	LONG		ulUsed;
	LONG		lRareIndex;
	SBYTE		sbOptionType[MAX_ITEM_OPTION], sbOptionLevel[MAX_ITEM_OPTION];

	switch( nWhichSlot )
	{
	case 0:
		{
			nIndex = m_abtnOppItems[nOppItem].GetItemIndex();
			llCount = m_abtnOppItems[nOppItem].GetItemCount();
			ulPlus = m_abtnOppItems[nOppItem].GetItemPlus();
			ulFlag = m_abtnOppItems[nOppItem].GetItemFlag();
			ulUsed = m_abtnOppItems[nOppItem].GetItemUsed();
			lRareIndex = m_abtnOppItems[nOppItem].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnOppItems[nOppItem].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnOppItems[nOppItem].GetItemOptionLevel( sbOption );
			}
		}
		break;

	case 1:
		{
			nIndex = m_abtnMyItems[nMyItem].GetItemIndex();
			llCount = m_abtnMyItems[nMyItem].GetItemCount();
			ulPlus = m_abtnMyItems[nMyItem].GetItemPlus();
			ulFlag = m_abtnMyItems[nMyItem].GetItemFlag();
			ulUsed = m_abtnMyItems[nMyItem].GetItemUsed();
			lRareIndex = m_abtnMyItems[nMyItem].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnMyItems[nMyItem].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnMyItems[nMyItem].GetItemOptionLevel( sbOption );
			}
		}
		break;

	case 2:
		{
			nIndex = m_abtnInvenItems[nRow][nCol].GetItemIndex();
			llCount = m_abtnInvenItems[nRow][nCol].GetItemCount();
			ulPlus = m_abtnInvenItems[nRow][nCol].GetItemPlus();
			ulFlag = m_abtnInvenItems[nRow][nCol].GetItemFlag();
			ulUsed = m_abtnInvenItems[nRow][nCol].GetItemUsed();
			lRareIndex = m_abtnInvenItems[nRow][nCol].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnInvenItems[nRow][nCol].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnInvenItems[nRow][nCol].GetItemOptionLevel( sbOption );
			}
		}
		break;
	}

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
		if( strPrefix.Length() >0 )
			szItemName.PrintF("%s %s", strPrefix, strItemName);
	}
	
	COLOR colNas = 0xF2F2F2FF;

	// Get item name
	if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
	{
		CTString	strCount;

		strCount.PrintF( "%I64d", llCount );
		_pUIMgr->InsertCommaToString( strCount );		
		strTemp.PrintF( "%s(%s)", szItemName, strCount );
		
		colNas = _pUIMgr->GetNasColor( llCount );
	}
	else
	{
		if( ( rItemData.GetType() == CItemData::ITEM_WEAPON ||
			rItemData.GetType() == CItemData::ITEM_SHIELD ) &&
			ulPlus > 0 )
			strTemp.PrintF( "%s +%d", szItemName, ulPlus );
#ifdef Pet_IMPROVEMENT_1ST
		// [070604: Su-won] 1차 펫 기능 개선
		// 펫의 진화단계 표시
		else if( _pUIMgr->IsPet(rItemData) )
		{
			const INDEX iPetIndex = ulPlus;
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
			strTemp = szItemName;
	}

	AddItemInfoString( strTemp, colNas );
	

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		COLOR colString =0xFFFFFFFF;
		CTString strTime1, strTime2;

		if(rItemData.IsFlag(ITEM_FLAG_CASH))
		{

			if (ulUsed > 0)
			{
				time_t tv_used = ulUsed - _pUIMgr->GetRadar()->GetStartTime();
				g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
				strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
				
				AddItemInfoString( strTemp, 0xFFAA44FF );
				IN_VALIDTM(g_tv_t)
			}
		}

		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_ACCESSORY:
			{
				if( rItemData.GetSubType() == CItemData::ACCESSORY_PET ) // 펫인 경우 
				{
					// 서버로 부터 읽어 드린 펫 교환 정보를 찾아서 출력
					SPetExchangeInfoString pPetExchangeInfo;
					// 서버로 부터 읽드린 펫 정보가 있는지 확인 있다면 참조로 데이터 전달.
					if( _pUIMgr->GetPetInfo()->GetPetExchangeInfo( ulPlus, pPetExchangeInfo ) )
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

					if(_pUIMgr->GetWildPetInfo()->GetWildPetInfo(ulPlus,pWildPetInfo))
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
				/*****
				else 
				{
					if(ulUsed > 0)
					{
						strTemp.PrintF(  _S( 510, "내구도 : %ld" ), ulUsed);		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
				*****/		
			}
			break;
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				//초 고급 제련석 여부
				if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) ) // 업 그레이드 가능 아이템 
				{
					BOOL bCanUseSuperGOJE = ( ulFlag & FLAG_ITEM_SUPER_STONE_USED ) ? FALSE : TRUE ;
					CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
#ifdef RARE_ITEM
					if( bCanUseSuperGOJE )
						AddItemInfoString(strCanUseSuperGOJE, 0xFFFF40FF);
#else
					CTString strCanNotUseSuperGOJE(_S( 1659, "초 고급 제련석 사용 불가능" ));
					AddItemInfoString(bCanUseSuperGOJE ? strCanUseSuperGOJE : strCanNotUseSuperGOJE
									, bCanUseSuperGOJE ? 0xFFFF40FF : 0xFF4040FF);
#endif
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0x9E9684FF );
#else
				AddItemInfoString( strTemp, 0xFFFFFFFF );
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



				int		nPlusValue;
				int nBasePhysicalAttack =rItemData.GetPhysicalAttack() +rItemRareOption.GetPhysicalAttack();
				int nBaseMagicAttack =rItemData.GetMagicAttack() +rItemRareOption.GetMagicAttack();

#ifndef RARE_ITEM
				colString = 0xDEC05BFF;
#endif
				if( ulPlus > 0 )
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulPlus ) ) - rItemData.GetPhysicalAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), ulPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), nBasePhysicalAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack );

						AddItemInfoString( strTemp, colString );

						if( ulPlus >= 15 )
						{
							strTemp.PrintF(_S( 1891, "물리 공격력 + 75" ));		
							AddItemInfoString( strTemp, colString );
						}
					}

					// Magic attack
					if( rItemData.GetMagicAttack() )
					{
						//nPlusValue = (int)( rItemData.GetMagicAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulPlus ) ) - rItemData.GetMagicAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), ulPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), nBaseMagicAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
															
						AddItemInfoString( strTemp, colString );

						if( ulPlus >= 15 )
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
					if( rItemData.GetMagicAttack() )
					{
						strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
						AddItemInfoString( strTemp, colString );
					}
				}

				if(ulUsed > 0)
				{
					strTemp.PrintF( _S( 510, "내구도 : %ld" ), ulUsed);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
				//초고급 제련석 제련석 여부
				//TEMP : 초고급 제련석 관련
				if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) ) // 업 그레이드 가능 아이템 
				{
					BOOL bCanUseSuperGOJE = ( ulFlag & FLAG_ITEM_SUPER_STONE_USED ) ? FALSE : TRUE ;
					CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
#ifdef RARE_ITEM
					if( bCanUseSuperGOJE )
						AddItemInfoString(strCanUseSuperGOJE, 0xFFFF40FF);
#else
					CTString strCanNotUseSuperGOJE(_S( 1659, "초 고급 제련석 사용 불가능" ));
					AddItemInfoString(bCanUseSuperGOJE ? strCanUseSuperGOJE : strCanNotUseSuperGOJE
									, bCanUseSuperGOJE ? 0xFFFF40FF : 0xFF4040FF);
#endif
				}

				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0x9E9684FF );
#else
				AddItemInfoString( strTemp, 0xFFFFFFFF );
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
				AddItemInfoString( strTemp, 0xFFFFFFFF);
#else
				AddItemInfoString( strTemp,
									nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
#endif

				int	nPlusValue;
				int nBasePhysicalDefence =rItemData.GetPhysicalDefence() +rItemRareOption.GetPhysicalDefence();
				int nBaseMagicDefence =rItemData.GetMagicDefence() +rItemRareOption.GetMagicDefence();
				
#ifndef RARE_ITEM
				colString = 0xDEC05BFF;
#endif
				if( ulPlus > 0 )
				{
					// Physical defense
					if( rItemData.GetPhysicalDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulPlus ) ) - rItemData.GetPhysicalDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalDefence(), ulPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), nBasePhysicalDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );
															
						AddItemInfoString( strTemp, colString );

						if( ulPlus >= 15 )
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
						//					pow( ITEM_PLUS_COFACTOR, ulPlus ) ) - rItemData.GetMagicDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicDefence(), ulPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), nBaseMagicDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
															
						AddItemInfoString( strTemp, colString );

						if( ulPlus >= 15 )
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
						AddItemInfoString( strTemp, colString);
					}

					// Magic defense
					if( rItemData.GetMagicDefence() > 0 )
					{
						strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
						AddItemInfoString( strTemp, colString );
					}
				}

				if(ulUsed > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), ulUsed);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
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

		if( rItemData.IsFlag(ITEM_FLAG_RARE) )
		{
			//if( sbOptionType[0] == -1 || sbOptionLevel[0] == 0 )
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
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
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
					AddItemInfoString( _S( 512, "봉인된 아이템" ), 0xE53535FF );		
				}
			}
			break;
		}

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
		nInfoHeight = 22;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nOppItem, int nMyItem, int nRow, int nCol )
{
	static int	nOldUniIndex = -1;
	int			nUniIndex;

	m_bShowItemInfo = FALSE;

	// Hide item information
	if( !bShowInfo )
	{
		nOldUniIndex = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	// Opposite items
	if( nOppItem >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nUniIndex = m_abtnOppItems[nOppItem].GetItemUniIndex();

		// Update item information
		if( nOldUniIndex != nUniIndex || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldUniIndex = nUniIndex;
			m_abtnOppItems[nOppItem].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = TRUE;
			GetItemInfo( 0, nInfoWidth, nInfoHeight, nOppItem );
		}
	}
	// My items
	else if( nMyItem >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nUniIndex = m_abtnMyItems[nMyItem].GetItemUniIndex();

		// Update item information
		if( nOldUniIndex != nUniIndex || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldUniIndex = nUniIndex;
			m_abtnMyItems[nMyItem].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = TRUE;	//m_nSelMyItemID == nMyItem;	// 아이템 삭제 막음
			GetItemInfo( 1, nInfoWidth, nInfoHeight, -1, nMyItem );
		}
	}
	// Inventory items of exchange
	else if( nRow >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nUniIndex = m_abtnInvenItems[nRow][nCol].GetItemUniIndex();

		// Update item information
		if( nOldUniIndex != nUniIndex || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldUniIndex = nUniIndex;
			m_abtnInvenItems[nRow][nCol].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelInvenItemID == ( nCol + nRow * EXCH_INVEN_SLOT_COL );
			GetItemInfo( 2, nInfoWidth, nInfoHeight, -1, -1, nRow, nCol );
		}
	}

	// Update item information box
	if( bUpdateInfo )
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
	{
		nOldUniIndex = -1;
		m_bShowItemInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::RenderItems()
{
	// Opposite items
	int	iItem;
	for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnOppItems[iItem].IsEmpty() )
			continue;

		m_abtnOppItems[iItem].Render();
	}

	// My items
	for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnMyItems[iItem].IsEmpty() )
			continue;

		m_abtnMyItems[iItem].Render();
	}

	// Inven items
	int	nX = EXCH_SLOT_SX, nY = EXCH_INVEN_SLOT_SY;
	int	iRow, iCol;
	int	iRowS = m_sbScrollBar.GetScrollPos();
	int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++, nX += 35 )
		{
			m_abtnInvenItems[iRow][iCol].SetPos( nX, nY );

			if( m_abtnInvenItems[iRow][iCol].IsEmpty() )
				continue;

			m_abtnInvenItems[iRow][iCol].Render();
		}
		nX = EXCH_SLOT_SX;	nY += 35;
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	_pUIMgr->GetDrawPort()->EndTextEx();

	// Set exchange texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Outline of ready items ( not movable )
	// Opposite items
	if( m_bExchReadyDest )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnOppItems[iItem].IsEmpty() )
				continue;

			m_abtnOppItems[iItem].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtWearingOutline.U0, m_rtWearingOutline.V0,
												m_rtWearingOutline.U1, m_rtWearingOutline.V1,
												0xFFFFFFFF );
		}
	}
	// My items
	if( m_bExchReady )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].IsEmpty() )
				continue;

			m_abtnMyItems[iItem].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtWearingOutline.U0, m_rtWearingOutline.V0,
												m_rtWearingOutline.U1, m_rtWearingOutline.V1,
												0xFFFFFFFF );
		}
	}

	// Outline of items in inventory slot of exchange ( not exchangeable )
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
		{
			// If button is empty or not weraing item
			if( m_abtnInvenItems[iRow][iCol].IsEmpty() ||
				m_abtnInvenItems[iRow][iCol].GetItemWearType() < 0 )
				continue;

			// Not exchangeable
			CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			CItemData	&rItemData = rItems.ItemData;
			if( !( rItemData.GetFlag() & ITEM_FLAG_EXCHANGE ) )
				continue;

			m_abtnInvenItems[iRow][iCol].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtWearingOutline.U0, m_rtWearingOutline.V0,
												m_rtWearingOutline.U1, m_rtWearingOutline.V1,
												0xFFFFFFFF );
		}
	}

	// Outline of selected item
	if( m_nSelMyItemID >= 0 )
	{
		m_abtnMyItems[m_nSelMyItemID].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelInvenItemID >= 0 )
	{
		int	nSelRow = m_nSelInvenItemID / EXCH_EXCH_SLOT_COL;
		if( nSelRow >= iRowS && nSelRow < iRowE )
		{
			int	nSelCol = m_nSelInvenItemID % EXCH_EXCH_SLOT_COL;

			m_abtnInvenItems[nSelRow][nSelCol].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelectOutline.U0, m_rtSelectOutline.V0,
												m_rtSelectOutline.U1, m_rtSelectOutline.V1,
												0xFFFFFFFF );
		}
	}

	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{
		// Item information region
		if( m_bDetailItemInfo )
		{
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
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
												m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
												m_rtInfoL.U0, m_rtInfoL.V0, m_rtInfoL.U1, m_rtInfoL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
												m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
												m_rtInfoM.U0, m_rtInfoM.V0, m_rtInfoM.U1, m_rtInfoM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
												m_rcItemInfo.Right, m_rcItemInfo.Bottom,
												m_rtInfoR.U0, m_rtInfoR.V0, m_rtInfoR.U1, m_rtInfoR.V1,
												0xFFFFFFFF );
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render item information
		if( m_bDetailItemInfo )
		{
			int	nInfoX = m_rcItemInfo.Left + 12;
			int	nInfoY = m_rcItemInfo.Top + 8;
			for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
			{
				if(iInfo==0 && m_bRareItem && m_iRareGrade>=0)
					_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, RareItem_Name_Color[m_iRareGrade] );
				else
					_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
				nInfoY += _pUIFontTexMgr->GetLineHeight();
			}
		}
		else
		{
			if(m_bRareItem && m_iRareGrade>=0)
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[0], m_rcItemInfo.Left + 8, m_rcItemInfo.Top + 3, RareItem_Name_Color[m_iRareGrade] );
			else
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[0], m_rcItemInfo.Left + 8, m_rcItemInfo.Top + 3, m_colItemInfo[0] );
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	else
	{
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::Render()
{
	// Set exchange texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Close button
	m_btnClose.Render();

	// Ready button
	m_btnReady.Render();

	// Exchange button
	m_btnExchange.Render();

	// Cancel button
	m_btnCancel.Render();

	// Scroll bar
	m_sbScrollBar.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Text in exchange
	// Title
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 149, "교환" ), m_nPosX + EXCH_TITLE_TEXT_OFFSETX,
										m_nPosY + EXCH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// Opposite name
	if( m_bAmISrc )
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( m_strDestName, m_nPosX + EXCH_OPP_NAME_OFFSETX,
												m_nPosY + EXCH_OPP_NAME_OFFSETY );
	}
	else
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( m_strSrcName, m_nPosX + EXCH_OPP_NAME_OFFSETX,
												m_nPosY + EXCH_OPP_NAME_OFFSETY );
	}

	// Date : 2005-11-11(오전 10:48:01), By Lee Ki-hwan 
	// 나스 색 추가 
	// Money
	COLOR colNas = 0;

	colNas = _pUIMgr->GetNasColor( m_strOppExchMoney );
	_pUIMgr->GetDrawPort()->PutTextExRX( m_strOppExchMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_OPP_MONEY_OFFSETY, colNas );
	
	colNas = _pUIMgr->GetNasColor( m_strMyExchMoney );
	_pUIMgr->GetDrawPort()->PutTextExRX( m_strMyExchMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_MY_MONEY_OFFSETY, colNas );

	colNas = _pUIMgr->GetNasColor( m_strMyCurMoney );
	_pUIMgr->GetDrawPort()->PutTextExRX( m_strMyCurMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_MYCUR_MONEY_OFFSETY, colNas );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Items
	RenderItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIExchange::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

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

			// Hold item button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				// My items
				/*if( !m_bExchReady && m_nSelMyItemID >= 0 )		// 아이템 삭제 막음
				{
					// Close message box of exchange
					_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

					_pUIMgr->SetHoldBtn( m_abtnMyItems[m_nSelMyItemID] );
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnMyItems[m_nSelMyItemID].SetBtnState( UBES_IDLE );
				}
				// Inven items
				else*/ if( !m_bExchReady && m_nSelInvenItemID >= 0 )
				{
					int	nSelRow = m_nSelInvenItemID / EXCH_INVEN_SLOT_COL;
					int	nSelCol = m_nSelInvenItemID % EXCH_INVEN_SLOT_COL;

					// Exchangeable
					CItems	&rItems = _pNetwork->MySlotItem[0][nSelRow][nSelCol];
					CItemData	&rItemData = rItems.ItemData;
					if( rItemData.GetFlag() & ITEM_FLAG_EXCHANGE )
					{
						// Close message box of exchange
						_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

						_pUIMgr->SetHoldBtn( m_abtnInvenItems[nSelRow][nSelCol] );
						int	nOffset = BTN_SIZE / 2;
						_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

						m_abtnInvenItems[nSelRow][nSelCol].SetBtnState( UBES_IDLE );
					}
				}

				bLButtonDownInItem = FALSE;
			}

			// Move exchange
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Ready button
			else if( m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Exchange button
			else if( m_btnExchange.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Scroll bar
			else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Opposite items
			else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnOppItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichItem );

				return WMSG_SUCCESS;
			}
			// My items
			else if( IsInsideRect( nX, nY, m_rcMySlot ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichItem );

				return WMSG_SUCCESS;
			}
			// Inven items
			else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
			{
				int	iRow, iCol;
				int	iRowS = m_sbScrollBar.GetScrollPos();
				int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
				int	nWhichRow = -1, nWhichCol;
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
					{
						if( m_abtnInvenItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichRow = iRow;	nWhichCol = iCol;
						}
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, -1, nWhichRow, nWhichCol );

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
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Opposite items
				else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
				{
					_pUIMgr->RearrangeOrder( UI_EXCHANGE, TRUE );
					return WMSG_SUCCESS;
				}
				// My items
				/*else if( IsInsideRect( nX, nY, m_rcMySlot ) )		// 아이템 삭제 막음
				{
					m_nSelMyItemID = -1;
					m_nSelInvenItemID = -1;

					for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelMyItemID = iItem;

							// Show tool tip
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							bLButtonDownInItem = TRUE;

							_pUIMgr->RearrangeOrder( UI_EXCHANGE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}*/
				// Inventory items of exchange
				else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					m_nSelInvenItemID = -1;
					m_nSelMyItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
						{
							if( m_abtnInvenItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected item
								m_nSelInvenItemID = iCol + iRow * EXCH_INVEN_SLOT_COL;

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, -1, iRow, iCol );

								bLButtonDownInItem = TRUE;

								_pUIMgr->RearrangeOrder( UI_EXCHANGE, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Ready button
				else if( m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Exchange button
				else if( m_btnExchange.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_EXCHANGE, TRUE );
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

				// If exchange isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Rej();

					return WMSG_SUCCESS;
				}
				// Ready button
				else if( ( wmsgResult = m_btnReady.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Ready();

					return WMSG_SUCCESS;
				}
				// Exchange button
				else if( ( wmsgResult = m_btnExchange.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Ok();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Rej();

					return WMSG_SUCCESS;
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Opposite item
				else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// My item
				else if( IsInsideRect( nX, nY, m_rcMySlot ) )
				{
					for( int iWear = 0; iWear < EXCH_EXCH_SLOT_TOTAL; iWear++ )
					{
						if( m_abtnMyItems[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Inventory items of exchange
				else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					int	iRow, iCol;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
						{
							if( m_abtnInvenItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Nothing
								return WMSG_SUCCESS;
							}
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and is from exchange
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_EXCHANGE )
					{
						// My items
						if( IsInsideRect( nX, nY, m_rcMySlot ) )
						{
							// If this item is moved from inventory slot of exchange
							/*if( m_nSelMyItemID < 0 ||
								m_abtnMyItems[m_nSelMyItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )*/	// 아이템 삭제 막음
							{
								AddExchItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount() );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Inventory items of exchange
						/*else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )		// 아이템 삭제 막음
						{
							int	nSelRow = m_nSelInvenItemID / EXCH_INVEN_SLOT_COL;
							int	nSelCol = m_nSelInvenItemID % EXCH_INVEN_SLOT_COL;
							// If this item is moved from my slot
							if( m_nSelInvenItemID < 0 ||
								m_abtnInvenItems[nSelRow][nSelCol].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								DelExchItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount() );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}*/
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
				// My items
				/*if( IsInsideRect( nX, nY, m_rcMySlot ) )		// 아이템 삭제 막음
				{
					m_nSelMyItemID = -1;

					for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if( !m_bExchReady )
							{
								// Close message box of exchange
								_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
								_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

								DelExchItem( m_abtnMyItems[iItem].GetItemRow(),
												m_abtnMyItems[iItem].GetItemCol(),
												m_abtnMyItems[iItem].GetItemUniIndex(),
												m_abtnMyItems[iItem].GetItemCount() );
							}

							// Show tool tip
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// Inventory items of exchange
				else*/ if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					m_nSelInvenItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
						{
							if( m_abtnInvenItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								if( !m_bExchReady )
								{
									// Exchangeable
									CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
									CItemData	&rItemData = rItems.ItemData;
									if( rItemData.GetFlag() & ITEM_FLAG_EXCHANGE )
									{
										// Close message box of exchange
										_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

										AddExchItem( iRow, iCol,
														m_abtnInvenItems[iRow][iCol].GetItemUniIndex(),
														m_abtnInvenItems[iRow][iCol].GetItemCount() );
									}
								}

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, -1, iRow, iCol );

								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Scroll bar
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempUniIndex;
static SQUAD	llTempCount;

// ----------------------------------------------------------------------------
// Name : AddExchItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::AddExchItem( int nRow, int nCol, int nUniIndex, SQUAD llCount )
{
	nTempUniIndex = nUniIndex;
	llTempCount = llCount;

	CItems	&rItems = _pNetwork->MySlotItem[0][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;

	// Ask quantity
	if( ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 )
	{
		CTString		strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
									UI_EXCHANGE, MSGCMD_EXCH_ADD_ITEM );
		const char	*szItemName = _pNetwork->GetItemName(rItemData.GetItemIndex());
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		SendExchangeItem_Add( nTempUniIndex, llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : DelExchItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::DelExchItem( int nRow, int nCol, int nUniIndex, SQUAD llCount )
{
	/*nTempUniIndex = nUniIndex;		// 아이템 삭제 막음
	llTempCount = llCount;

	CItems	&rItems = _pNetwork->MySlotItem[0][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;

	// Ask quantity
	if( llTempCount > 1 )
	{
		CTString		strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
									UI_EXCHANGE, MSGCMD_EXCH_DEL_ITEM );
		//const char	*szItemName = rItemData.GetName();
		const char	*szItemName = _pNetwork->GetItemName(rItemData.GetItemIndex());
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		SendExchangeItem_Del( nTempUniIndex, llTempCount );
	}*/
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
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

	case MSGCMD_EXCH_ADD_ITEM:
		if( bOK )
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			for( int iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount > 0 && llCount <= llTempCount )
					SendExchangeItem_Add( nTempUniIndex, llCount );
			}
		}
		break;

	case MSGCMD_EXCH_DEL_ITEM:
		/*if( bOK )		// 아이템 삭제 막음
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			for( int iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount > 0 && llCount <= llTempCount )
					SendExchangeItem_Del( nTempUniIndex, llCount );
			}
		}*/
		break;
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Req()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Req( CTString &strDestName )
{
	// Already exchange
	if( m_nSrcIndex != -1 && m_nDestIndex != -1 )
	{
		CTString	strSysMessage;
		if( m_bAmISrc )
			strSysMessage.PrintF( _S2( 422, m_strDestName, "이미 %s<과> 교환중입니다." ), m_strDestName );
		else
			strSysMessage.PrintF( _S2( 422, m_strSrcName, "이미 %s<과> 교환중입니다." ), m_strSrcName );

		_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

		return;
	}
	
	// Find index of dest character
	SLONG	slDestIndex = -1;
	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iPlyaer = 0; iPlyaer < ctCha; iPlyaer++ ) 
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iPlyaer];
		if( ct.cha_strName == strDestName )
		{
// 강동민 수정 시작		// 개인상점
			if( ct.cha_sbShopType != PST_NOSHOP )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 737, "개인 상점과는 교환을 할 수 없습니다." ), SYSMSG_ERROR );		
				return;
			}
// 강동민 수정 끝		// 개인상점
			slDestIndex = ct.cha_Index;
			break;
		}
	}

	// If dest character doesn't exist
	if( slDestIndex == -1 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 151, "교환 대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	_pNetwork->ExchangeReq_Req( slDestIndex, strDestName );

	m_bAmISrc = TRUE;
	m_nSrcIndex = _pNetwork->MyCharacterInfo.index;
	m_nDestIndex = slDestIndex;
	m_strSrcName = _pNetwork->MyCharacterInfo.name;
	m_strDestName = strDestName;

	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );

	// Create message box
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_CANCEL,
								UI_EXCHANGE, MSGCMD_EXCH_REQ_SRC );
	strMessage.PrintF( _S( 152, "%s에게 교환을 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	_pUIMgr->SetCSFlagOn( CSF_EXCHANGE );
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Rep()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Rep()
{
	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		_pNetwork->ExchangeReq_Rej();
		return;
	}

	_pNetwork->ExchangeReq_Rep();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Rej()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Rej()
{
	_pNetwork->ExchangeReq_Rej();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Ready()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Ready()
{
	// Close message box of exchange
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

	_pNetwork->ExchangeReq_Ready();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Ok()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Ok()
{
	_pNetwork->ExchangeReq_Ok();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeItem_Add()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeItem_Add( int nUniIndex, SQUAD llCount )
{
	_pNetwork->ExchangeItem_Add( nUniIndex, llCount );
}

// ----------------------------------------------------------------------------
// Name : SendExchangeItem_Del()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeItem_Del( int nUniIndex, SQUAD llCount )
{
	//_pNetwork->ExchangeItem_Del( nUniIndex, llCount );		// 아이템 삭제 막음
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Req()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Req( int nSrcIndex, CTString &strSrcName,
									int nDestIndex, CTString &strDestName )
{
	m_bAmISrc = FALSE;
	m_nSrcIndex = nSrcIndex;
	m_nDestIndex = nDestIndex;
	m_strSrcName = strSrcName;
	m_strDestName = strDestName;

	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );

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
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	_pUIMgr->SetCSFlagOn( CSF_EXCHANGE );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Rep()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Rep()
{
	CTString	strMessage;
	if( m_bAmISrc )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		strMessage.PrintF( _S2( 154, m_strDestName, "%s<과> 교환을 시작합니다." ), m_strDestName );
	}
	else
		strMessage.PrintF( _S2( 154, m_strSrcName, "%s<과> 교환을 시작합니다." ), m_strSrcName );

	// Add system message
	_pUIMgr->GetChatting()->AddSysMessage( strMessage );

	// Prepare exchange
	PrepareExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Rej()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Rej( BOOL bSrcReject )
{
	CTString	strMessage;
	if( m_bAmISrc )
	{
		if( bSrcReject )
			strMessage.PrintF( _S2( 155, m_strDestName, "%s<과>의 교환을 취소하셨습니다." ), m_strDestName );
		else
		{
			_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
			strMessage.PrintF( _S2( 156, m_strDestName, "%s<가> 교환을 취소했습니다." ), m_strDestName );
		}
	}
	else
	{
		if( bSrcReject )
		{
			_pUIMgr->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );
			strMessage.PrintF( _S2( 156, m_strSrcName, "%s<가> 교환을 취소했습니다." ), m_strSrcName );
		}
		else
			strMessage.PrintF( _S2( 155, m_strSrcName, "%s<과>의 교환을 취소하셨습니다." ), m_strSrcName );
	}

	// Add system message
	_pUIMgr->GetChatting()->AddSysMessage( strMessage );

	// Reset exchange
	ResetExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Ready()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Ready( BOOL bSrcReady )
{
	if( ( m_bAmISrc && bSrcReady ) || ( !m_bAmISrc && !bSrcReady ) )
	{
		m_bExchReady = TRUE;
		m_btnReady.SetEnable( FALSE );
	}
	else
	{
		m_bExchReadyDest = TRUE;
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_BothReady()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_BothReady()
{
	m_bExchReady = TRUE;
	m_bExchReadyDest = TRUE;

	// Disable ready button
	m_btnReady.SetEnable( FALSE );

	// Enable exchange button
	m_btnExchange.SetEnable( TRUE );

	// Add system message
	_pUIMgr->GetChatting()->AddSysMessage( _S( 157, "교환 준비가 완료되었습니다." ), SYSMSG_ERROR );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Full()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Full( BOOL bSrcFull )
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
	m_btnReady.SetEnable( TRUE );

	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	// Add system message
	_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Ok()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Ok( BOOL bSrcOk )
{
	if( m_bAmISrc )
	{
		if( bSrcOk )
			m_btnExchange.SetEnable( FALSE );
	}
	else
	{
		if( !bSrcOk )
			m_btnExchange.SetEnable( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_BothOk()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_BothOk()
{
	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	// Add system message
	_pUIMgr->GetChatting()->AddSysMessage( _S( 159, "교환이 성공적으로 이루어졌습니다." ) );

	ResetExchange();
}


// ----------------------------------------------------------------------------
// Name : ExchangeReq_Pet_Err()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Pet_Err(BOOL petErr)
{
	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	if(!petErr){
		// Add system message
		_pUIMgr->GetChatting()->AddSysMessage( _S(2221, "펫을 더 이상 소유할 수 없습니다." ),SYSMSG_ERROR );
	}
	else{
		CTString	strMessage;
		strMessage.PrintF(_S(2555, "%s님은 펫을 더 이상 소유할 수 없는 상태 입니다." ) ,m_strDestName);
		_pUIMgr->GetChatting()->AddSysMessage(strMessage,SYSMSG_ERROR); 
	}
		
	ResetExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Add()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
									int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex )
{
	// My items
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].IsEmpty() )
				break;
		}

		m_abtnMyItems[iItem].SetItemInfo( 0, 0, 0, nIndex, nUniIndex, -1 );
		m_abtnMyItems[iItem].SetItemCount( llCount );
		m_abtnMyItems[iItem].SetItemPlus( ulPlus );
		m_abtnMyItems[iItem].SetItemUsed( ulUsed );
		m_abtnMyItems[iItem].SetItemUsed2( ulUsed2 );
		m_abtnMyItems[iItem].SetItemFlag( ulFlag );
		m_abtnMyItems[iItem].SetItemRareIndex( lRareIndex );

		if( m_abtnMyItems[iItem].GetItemFlag()&FLAG_ITEM_SEALED){
			_pUIMgr->GetChatting()->AddSysMessage( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
			m_abtnMyItems[iItem].InitBtn();
			return;
		}

		// Update inventory of exchange
		for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
		{
			for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
			{
				if( m_abtnInvenItems[iRow][iCol].GetItemUniIndex() == nUniIndex )
				{
					// Set item location of my item
					m_abtnMyItems[iItem].SetItemLocation( 0, iRow, iCol );
					m_abtnMyItems[iItem].SetItemRareIndex( lRareIndex );

					// Update count
					SQUAD	llNewCount = m_abtnInvenItems[iRow][iCol].GetItemCount();
					llNewCount -= llCount;
					m_abtnInvenItems[iRow][iCol].SetItemCount( llNewCount );
					if( llNewCount <= 0 )
					{
						m_abtnInvenItems[iRow][iCol].SetEmpty( TRUE );

						// Unselect item
						if( m_nSelInvenItemID == ( iCol + iRow * EXCH_INVEN_SLOT_COL ) )
							m_nSelInvenItemID = -1;
					}

					// If item is money
					CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
					if( ItemData.GetType() == CItemData::ITEM_ETC &&
						ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
					{
						m_strMyExchMoney.PrintF( "%I64d", llCount );
						_pUIMgr->InsertCommaToString( m_strMyExchMoney );
					}

					if(ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
					ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
					{
						_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,ulPlus);
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
			if( m_abtnOppItems[iItem].IsEmpty() )
				break;
		}

		m_abtnOppItems[iItem].SetItemInfo( 0, 0, 0, nIndex, nUniIndex, 0 );
		m_abtnOppItems[iItem].SetItemCount( llCount );
		m_abtnOppItems[iItem].SetItemPlus( ulPlus );
		m_abtnOppItems[iItem].SetItemFlag( ulFlag );
		m_abtnOppItems[iItem].SetItemUsed( ulUsed );
		m_abtnOppItems[iItem].SetItemUsed2( ulUsed2 );
		m_abtnOppItems[iItem].SetItemRareIndex( lRareIndex );

		// If item is money
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( ItemData.GetType() == CItemData::ITEM_ETC &&
			ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			m_strOppExchMoney.PrintF( "%I64d", llCount );
			_pUIMgr->InsertCommaToString( m_strOppExchMoney );
		}
		if(ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
			ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(m_nSrcIndex,ulPlus);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_SetOptionData()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
												SBYTE sbOptionType, SBYTE sbOptionLevel, LONG lRareIndex )
{
	// My items
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_abtnMyItems[iItem].SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
		m_abtnMyItems[iItem].SetItemRareIndex( lRareIndex);
	}
	// Opposite items
	else
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnOppItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_abtnOppItems[iItem].SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
		m_abtnOppItems[iItem].SetItemRareIndex( lRareIndex);
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Del()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Del( BOOL bSrcDel, int nUniIndex )
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
					if( ItemData.GetType() == CItemData::ITEM_ETC &&
						ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
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
		if( ItemData.GetType() == CItemData::ITEM_ETC &&
			ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			m_strOppExchMoney.PrintF( "%d", 0 );
		}

		m_abtnOppItems[iItem].InitBtn();
	}*/
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Update()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount )
{
	// My items
	if( ( m_bAmISrc && bSrcUpdate ) || ( !m_bAmISrc && !bSrcUpdate ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		SQUAD	llCountDiff = m_abtnMyItems[iItem].GetItemCount() - llCount;
		m_abtnMyItems[iItem].SetItemCount( llCount );

		// Update inventory of exchange
		for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
		{
			for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
			{
				if( m_abtnInvenItems[iRow][iCol].GetItemUniIndex() == nUniIndex )
				{
					SQUAD	llNewCount = m_abtnInvenItems[iRow][iCol].GetItemCount() + llCountDiff;
					m_abtnInvenItems[iRow][iCol].SetItemCount( llNewCount );
					if( llNewCount <= 0 )
					{
						m_abtnInvenItems[iRow][iCol].SetEmpty( TRUE );

						// Unselect item
						if( m_nSelInvenItemID == ( iCol + iRow * EXCH_INVEN_SLOT_COL ) )
							m_nSelInvenItemID = -1;
					}
					else
						m_abtnInvenItems[iRow][iCol].SetEmpty( FALSE );

					// If item is money
					int			nIndex = m_abtnInvenItems[iRow][iCol].GetItemIndex();
					CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
					if( ItemData.GetType() == CItemData::ITEM_ETC &&
						ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
					{
						m_strMyExchMoney.PrintF( "%I64d", llCount );
						_pUIMgr->InsertCommaToString( m_strMyExchMoney );
					}
					if(ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
						ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
					{
						_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,m_abtnMyItems[iItem].GetItemPlus());
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

		SQUAD	llCountDiff = m_abtnOppItems[iItem].GetItemCount() - llCount;
		m_abtnOppItems[iItem].SetItemCount( llCount );

		// If item is money
		int			nIndex = m_abtnOppItems[iItem].GetItemIndex();
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( ItemData.GetType() == CItemData::ITEM_ETC &&
			ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			m_strOppExchMoney.PrintF( "%I64d", llCount );
			_pUIMgr->InsertCommaToString( m_strOppExchMoney );
		}
		if(ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
			ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,m_abtnOppItems[iItem].GetItemPlus());
		}
	}
}

void CUIExchange::ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex)
	{
		SBYTE	sbOptionType, sbOptionLevel;

		//레어 옵션 인덱스
		(*istr) >> sbOptionType;
		(*istr) >> sbOptionLevel;
		UWORD iRareIndex = (static_cast<WORD>(sbOptionType) <<8) +sbOptionLevel;

		//레어 옵션 종류
		(*istr) >> sbOptionType;
		(*istr) >> sbOptionLevel;
	
		WORD iRareOption = (static_cast<WORD>(sbOptionType) <<8) +sbOptionLevel;
		WORD wCBit =1;
		SBYTE sbOption =-1;
		for(int iBit=0; iBit<10; ++iBit)
		{
			if( iRareOption & wCBit)
			{
				int OptionType =_pNetwork->wo_vecItemRareOption[iRareIndex].m_Option[iBit].index;
				int OptionLevel =_pNetwork->wo_vecItemRareOption[iRareIndex].m_Option[iBit].level;
				this->ExchangeItem_SetOptionData(bSrcAdd, nUniIndex, ++sbOption, OptionType, OptionLevel, iRareIndex );
			}
			wCBit <<=1;
	}
}