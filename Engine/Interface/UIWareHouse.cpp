#include "stdh.h"
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISecurity.h>
#include <Engine/Entities/Items.h>
#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/PetInfo.h>

// Define item slot
#define WAREHOUSE_WAREHOUSE_WIDTH			216
#define WAREHOUSE_TOP_HEIGHT				74
#define WAREHOUSE_MIDDLE_HEIGHT				34
#define WAREHOUSE_BOTTOM_HEIGHT				41
#define WAREHOUSE_TRADE_HEIGHT				75
#define WAREHOUSE_DAYS_HEIGHT				27		// wooss 050811
#define WAREHOUSE_BUY_TOP_SLOT_SX			11		// Buy Mode(Top)
#define WAREHOUSE_BUY_TOP_SLOT_SY			55		// wooss 28 -> 55	
#define WAREHOUSE_BUY_BOTTOM_SLOT_SX		11		// (Bottom)
#define WAREHOUSE_BUY_BOTTOM_SLOT_SY		241		// wooss 214 -> 241 	

#define	ITEM_LIST_START		(1 << 0)
#define ITEM_LIST_END		(1 << 1)
#define ITEM_LIST_EMPTY		(1 << 2)

enum eSelection
{
	WAREHOUSE_IN,
	WAREHOUSE_OUT,
	WAREHOUSE_CHANGEPW,
	WAREHOUSE_TALK,
};

extern INDEX g_iCountry;

// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;


// ----------------------------------------------------------------------------
// Name : CUIWareHouse()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIWareHouse::CUIWareHouse()
{
	m_bStorageWareHouse		= FALSE;
	m_nSelWareHouseItemID	= -1;
	m_nSelTradeItemID		= -1;
	m_nSelectedWareHouseID	= -1;
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_nTex				= 1;
	m_bShowItemInfo		= FALSE;
	m_bDetailItemInfo	= FALSE;
	m_nCurInfoLines		= 0;
	m_strTotalPrice		= CTString( "0" );
	m_bHasPassword		= FALSE;
	m_bSealed			= FALSE;
	m_strPW				= CTString( "0" );
	m_nNpcIndex			= -1;

	m_useTime			= 0; //wooss 050817
}

// ----------------------------------------------------------------------------
// Name : ~CUIWareHouse()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIWareHouse::~CUIWareHouse()
{
	if( !m_vectorStorageItems.empty() )
		m_vectorStorageItems.clear();
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	//wooss 050811
	//extend inventory rect
	m_rcExInvenRect.SetRect( 0, 23, 216,50 );
	
	// Inventory region
	m_rcTop.SetRect( 4, 48, 190, 229 );			//wooss 24 -> 48 , 205 -> 229
	m_rcBottom.SetRect( 4, 232, 190, 306 );		//wooss 209 -> 232 , 282 -> 306

	// Create warehouse texture
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottomLine.SetUV( 0, 59, 216, 61, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );	

	// wooss 050811
	// extend inventory remain days
	m_rtExInven.SetUV( 0, 98, 216, 129, fTexWidth, fTexHeight);

	// Inventory
	m_rtTopTopInven.SetUV( 0, 131, 216, 205, fTexWidth, fTexHeight );		// 74
	m_rtTopMiddleInven.SetUV( 0, 206, 216, 240, fTexWidth, fTexHeight );	// 34
	m_rtTopBottomInven.SetUV( 0, 241, 216, 282, fTexWidth, fTexHeight );	// 41
	m_rtBottomInven.SetUV( 0, 283, 216, 358, fTexWidth, fTexHeight );
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );
	m_rtBlankInven.SetUV( 0, 387, 216, 393, fTexWidth, fTexHeight );
	m_rtPriceInven.SetUV( 0, 359, 216, 381, fTexWidth, fTexHeight );

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

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buy button of warehouse
	m_btnWareHouseKeep.Create( this, _S( 812, "보관" ), 105, 339, 51, 21 );			//wooss 312->339		
	m_btnWareHouseKeep.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseKeep.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseKeep.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseKeep.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Sell button of warehouse
	m_btnWareHouseTake.Create( this, _S( 813, "찾기" ), 105, 339, 51, 21 );			//wooss 312->339
	m_btnWareHouseTake.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseTake.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseTake.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseTake.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of warehouse
	m_btnWareHouseCancel.Create( this, _S( 139, "취소" ), 159, 339, 51, 21 );		//wooss 312->339
	m_btnWareHouseCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Top scroll bar
	m_sbTopScrollBar.Create( this, 194, 54, 9, 171 );			//wooss 28 -> 54
	m_sbTopScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbTopScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollRange( WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL );
	m_sbTopScrollBar.SetCurItemCount( WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL );	 //wooss
	m_sbTopScrollBar.SetItemsPerPage( WAREHOUSE_WAREHOUSE_SLOT_ROW );
	// Up button
	m_sbTopScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbTopScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbTopScrollBar.SetBarTopUV( 217, 16, 226, 24, fTexWidth, fTexHeight );	//wooss 25 -> 24
	m_sbTopScrollBar.SetBarMiddleUV( 217, 27, 226, 29, fTexWidth, fTexHeight );	//wooss 30 -> 29 
	m_sbTopScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );	//wooss
	// Wheel region
	m_sbTopScrollBar.SetWheelRect( -192, -6, 191, 148 );

	// Slot items
	// WareHouse Slot(5x4)
	for( int iRow = 0; iRow < WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
		{			
			m_abtnWareHouseItems[iRow][iCol].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM, 0, iRow, iCol );
		}
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
		m_abtnTradeItems[iItem].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CheckHasPassWord()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::CheckHasPassWord( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_WAREHOUSE_REQ ) || IsVisible())
		return;

	m_fNpcX			= fX;
	m_fNpcZ			= fZ;
	m_nNpcIndex		= iMobIndex;
	m_bHasQuest		= bHasQuest;
	_pNetwork->SendWareHouseIsSetPassword();
}

// ----------------------------------------------------------------------------
// Name : OpenWareHouse()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::OpenWareHouse( SBYTE sbSetPW )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_WAREHOUSE_REQ ) || IsVisible())
		return;
	
	if(_pUIMgr->GetSecurity()->IsVisible() )
	{
		return;
	}
	
	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}
	
	_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_PASSWORD);
	_pUIMgr->CloseMessageBox( MSGCMD_CONFIRM_PASSWORD);
	_pUIMgr->CloseMessageBox( MSGCMD_OLD_PASSWORD);	

	BOOL bHasQuest = m_bHasQuest;
	ResetWareHouse();

	m_bHasPassword	= sbSetPW;

	// Set position of target npc
	//m_fNpcX = fX;
	//m_fNpcZ = fZ;

	// Character state flags
	_pUIMgr->SetCSFlagOn( CSF_WAREHOUSE );

	_pUIMgr->CloseMessageBoxL( MSGLCMD_WAREHOUSE_REQ );

	// Create refine message box
	_pUIMgr->CreateMessageBoxL( _S( 814, "보관소" ) , UI_WAREHOUSE, MSGLCMD_WAREHOUSE_REQ );	

	if( m_nNpcIndex != -1 )
	{
		CTString	strNpcName = _pNetwork->GetMobName(m_nNpcIndex);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	}

	_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, _S( 815, "물품 보관을 원하십니까?" ) , -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, _S( 816, "저희 물품 보관소에서는 여행자님의 물품을 안전하게 보관하고 언제 어디서든지 저희 물품 보관소가 있는 곳이라면 물품을 찾으실 수 있습니다." ) , -1, 0xA3A1A3FF );		

	CTString strMessage;	
	strMessage.PrintF( _S( 817, "물품을 맡긴다." ) );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage, WAREHOUSE_IN );

	strMessage.PrintF( _S( 818, "물품을 찾는다." ) );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage, WAREHOUSE_OUT );

	if( g_iCountry == KOREA )
		_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, _S( 819, "암호를 변경한다." ), WAREHOUSE_CHANGEPW );		

	if( bHasQuest )
	{
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_WAREHOUSE_REQ);				
#else
		strMessage.PrintF( _S( 1053, "이야기한다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage, WAREHOUSE_TALK );		
#endif
	}

	strMessage.PrintF( _S( 1220, "취소한다." ) );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::PrepareWareHouse()
{
	// Already exchange
	if( _pUIMgr->IsCSFlagOn( CSF_EXCHANGE ) )
	{
		_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
		_pUIMgr->GetChatting()->AddSysMessage( _S( 821, "교환중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// Already personal shop
	if( _pUIMgr->IsCSFlagOn( CSF_PERSONALSHOP ) )
	{
		_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
		_pUIMgr->GetChatting()->AddSysMessage( _S( 822, "개인 상점 사용중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// Already shop
	if( _pUIMgr->IsCSFlagOn( CSF_SHOP ) )
	{
		_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
		_pUIMgr->GetChatting()->AddSysMessage( _S( 807, "상점 거래중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	RefreshWareHouse();

	/*
	int	iRow, iCol;
	int	nX = WAREHOUSE_BUY_TOP_SLOT_SX;
	int	nY = WAREHOUSE_BUY_TOP_SLOT_SY;
	int	iRowS = m_nCurRow;
	int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++, nX += 35 )
		{
			m_abtnWareHouseItems[iRow][iCol].SetPos( nX, nY );
		}
		nX = WAREHOUSE_BUY_TOP_SLOT_SX;	nY += 35;
	}
	*/

	int nX = WAREHOUSE_BUY_BOTTOM_SLOT_SX;
	int nY = WAREHOUSE_BUY_BOTTOM_SLOT_SY;
	// ---Trade slot items---
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{	
		if( iItem == WAREHOUSE_TRADE_SLOT_COL )
		{
			nY += 35;	nX = WAREHOUSE_BUY_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[iItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_WAREHOUSE, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::ResetWareHouse()
{
	m_nSelWareHouseItemID	= -1;
	m_nSelTradeItemID		= -1;
	m_nSelectedWareHouseID	= -1;
	m_strTotalPrice = CTString( "0" );
	m_strPW			= CTString("");
	m_bSealed		= FALSE;
	m_bHasPassword	= FALSE;
	m_nTotalPrice	= 0;
	m_nNumOfItem	= 0;
	m_bHasQuest		= FALSE;

	ClearItems();

	_pUIMgr->RearrangeOrder( UI_WAREHOUSE, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_WAREHOUSE );

	// Close message box of warehouse
	_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );

	m_nCurRow = 0;
	m_sbTopScrollBar.SetScrollPos( 0 );

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::RefreshWareHouse()
{
	for(int i = 0; i < WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL; i++)
	{
		for(int j = 0; j < WAREHOUSE_WAREHOUSE_SLOT_COL; j++)
		{
			m_abtnWareHouseItems[i][j].InitBtn();
		}
	}

	ASSERT(m_vectorStorageItems.size() <= WAREHOUSE_WAREHOUSE_SLOT_COL * WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL && 
		"Invalid Storge Count" );

	int t = 0;
	std::vector<CItems>::const_iterator end = m_vectorStorageItems.end();
	for(std::vector<CItems>::iterator pos = m_vectorStorageItems.begin(); pos != end; ++pos)
	{
		if( (*pos).Item_Sum > 0 )
		{
			const int iRow = t / WAREHOUSE_WAREHOUSE_SLOT_COL;
			const int iCol = t % WAREHOUSE_WAREHOUSE_SLOT_COL;

			ASSERT( iRow <= WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL && "Invalid Row Column" );
			
			m_abtnWareHouseItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, (*pos).Item_Index,
				(*pos).Item_UniIndex, (*pos).Item_Wearing );
			m_abtnWareHouseItems[iRow][iCol].SetItemPlus( (*pos).Item_Plus );
			m_abtnWareHouseItems[iRow][iCol].SetItemFlag( (*pos).Item_Flag );
			m_abtnWareHouseItems[iRow][iCol].SetItemUsed( (*pos).Item_Used );
			m_abtnWareHouseItems[iRow][iCol].SetItemUsed2( (*pos).Item_Used2 );
			m_abtnWareHouseItems[iRow][iCol].SetItemCount( (*pos).Item_Sum );
			m_abtnWareHouseItems[iRow][iCol].SetItemRareIndex( (*pos).Item_RareIndex );
			
			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				m_abtnWareHouseItems[iRow][iCol].SetItemOptionData( sbOption,
					(*pos).GetOptionType( sbOption ),
					(*pos).GetOptionLevel( sbOption ) );
			}
			
			t++;
		}
	}
// wooss 050812
// 아이템 25개를 넘으면 스크롤 확장되던 것을 아이템 개수에 상관없이 스크롤되게 변형
// wooss 060807 다시 풀어 줌....
	const int	ctWareHouseItems	= m_vectorStorageItems.size();	
	int	nScrollItem = ( ctWareHouseItems + WAREHOUSE_WAREHOUSE_SLOT_COL - 1 ) / WAREHOUSE_WAREHOUSE_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::ClearItems()
{
	if( !m_vectorStorageItems.empty() )
	{ 
		m_vectorStorageItems.clear();
	}
	for(int i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; i++)
		m_abtnTradeItems[i].InitBtn();			

	for(i = 0; i < WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL; i++)
	{
		for(int j = 0; j < WAREHOUSE_WAREHOUSE_SLOT_COL; j++)
		{
			m_abtnWareHouseItems[i][j].InitBtn();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
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
// Name : GetItemInfo()
// Desc : nWhichSlot - 0: opposite slot, 1: my slot, 2: inventory slot
// ----------------------------------------------------------------------------
BOOL CUIWareHouse::GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight, int nTradeItem, int nRow, int nCol )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex;
	SQUAD		llCount;
	ULONG		ulItemPlus, ulFlag;
	LONG		lItemUsed;
	LONG		lRareIndex;
	SBYTE		sbOptionType[MAX_ITEM_OPTION], sbOptionLevel[MAX_ITEM_OPTION];

	switch( nWhichSlot )
	{
	case 0:
		{
			nIndex		= m_abtnTradeItems[nTradeItem].GetItemIndex();
			llCount		= m_abtnTradeItems[nTradeItem].GetItemCount();
			ulItemPlus	= m_abtnTradeItems[nTradeItem].GetItemPlus();
			ulFlag		= m_abtnTradeItems[nTradeItem].GetItemFlag();
			lItemUsed	= m_abtnTradeItems[nTradeItem].GetItemUsed();
			lRareIndex	= m_abtnTradeItems[nTradeItem].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnTradeItems[nTradeItem].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnTradeItems[nTradeItem].GetItemOptionLevel( sbOption );
			}
		}
		break;

	case 1:
		{
			nIndex		= m_abtnWareHouseItems[nRow][nCol].GetItemIndex();
			llCount		= m_abtnWareHouseItems[nRow][nCol].GetItemCount();
			ulItemPlus	= m_abtnWareHouseItems[nRow][nCol].GetItemPlus();
			ulFlag		= m_abtnWareHouseItems[nRow][nCol].GetItemFlag();
			lItemUsed	= m_abtnWareHouseItems[nRow][nCol].GetItemUsed();
			lRareIndex	= m_abtnWareHouseItems[nRow][nCol].GetItemRareIndex();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnWareHouseItems[nRow][nCol].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnWareHouseItems[nRow][nCol].GetItemOptionLevel( sbOption );
			}
		}
		break;
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

	COLOR colNas = 0xF2F2F2FF;

	// Get item name	
	if( ulItemPlus > 0 && !(_pUIMgr->IsPet(rItemData) || _pUIMgr->IsWildPet(rItemData)))
		strTemp.PrintF( "%s +%d", szItemName, ulItemPlus );
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
		// 펫 진화 상태 표시
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
	AddItemInfoString( strTemp, colNas );

	const __int64	iPrice = CalculateStoragePrice();

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		COLOR colString = 0xFFFFFFFF;

		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
				//AddItemInfoString( strTemp, 0x9E9684FF );
				AddItemInfoString( strTemp, 0xFFFFFFFF );

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

				if(lItemUsed > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), lItemUsed);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
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
				AddItemInfoString( strTemp, 0xFFFFFFFF );	//흰색으로 표시
#else
				AddItemInfoString( strTemp,
									nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
#endif

				int	nPlusValue;
				int nBasePhysicalDefence =rItemData.GetPhysicalDefence() +rItemRareOption.GetPhysicalDefence();
				int nBaseMagicDefence =rItemData.GetMagicDefence() +rItemRareOption.GetMagicDefence();
				
#ifdef RARE_ITEM
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
							AddItemInfoString( strTemp, colString);

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
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
				/*****
				if(lItemUsed > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), lItemUsed);
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
				*****/
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
void CUIWareHouse::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nTradeItem, int nRow, int nCol )
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
	// WareHouse
	else if( nRow >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_abtnWareHouseItems[nRow][nCol].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnWareHouseItems[nRow][nCol].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelWareHouseItemID == ( nCol + nRow * WAREHOUSE_WAREHOUSE_SLOT_COL );;
			if( m_bStorageWareHouse )
				m_bDetailItemInfo = TRUE;
			
			if( !GetItemInfo( 1, nInfoWidth, nInfoHeight, -1, nRow, nCol ) )
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
// Name : RenderWareHouseItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIWareHouse::RenderWareHouseItems()
{
	int	nX = WAREHOUSE_BUY_TOP_SLOT_SX;
	int	nY = WAREHOUSE_BUY_TOP_SLOT_SY;

	int	iRow, iCol;
	int	iRowS = m_nCurRow;
	int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; ++iRow )
	{
		for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; ++iCol, nX+= 35)
		{			
			m_abtnWareHouseItems[iRow][iCol].SetPos( nX, nY );
			if( m_abtnWareHouseItems[iRow][iCol].IsEmpty() )
				continue;
			
			m_abtnWareHouseItems[iRow][iCol].Render();
		}
		nX = WAREHOUSE_BUY_TOP_SLOT_SX;	nY += 35;
	}

	// ---Trade slot items---
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; ++iItem )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
				continue;
		
		m_abtnTradeItems[iItem].Render();		
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	_pUIMgr->GetDrawPort()->EndTextEx();

	// Set warehouse texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );	


	// Outline of selected item
	if( m_nSelTradeItemID >= 0 )
	{
		m_abtnTradeItems[m_nSelTradeItemID].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelWareHouseItemID >= 0 )
	{
		int	nSelRow = m_nSelWareHouseItemID / WAREHOUSE_WAREHOUSE_SLOT_COL;
		if( nSelRow >= iRowS && nSelRow < iRowE )
		{
			int	nSelCol = m_nSelWareHouseItemID % WAREHOUSE_WAREHOUSE_SLOT_COL;

			m_abtnWareHouseItems[nSelRow][nSelCol].GetAbsPos( nX, nY );
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
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
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
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		ResetWareHouse();

	// Set warehouse texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	int	nX, nY;

	// Add render regions
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 38,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 38,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 2,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 2,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackBottomLine.U0, m_rtBackBottomLine.V0,
										m_rtBackBottomLine.U1, m_rtBackBottomLine.V1,
										0xFFFFFFFF );	

	// Buy
	nX = m_nPosX;
	nY = m_nPosY + 22;

	//wooss 050811 ExInventory window
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_DAYS_HEIGHT,
		m_rtExInven.U0, m_rtExInven.V0,
		m_rtExInven.U1, m_rtExInven.V1,
		0xFFFFFFFF );
	nY += WAREHOUSE_DAYS_HEIGHT;
	
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_TOP_HEIGHT,
		m_rtTopTopInven.U0, m_rtTopTopInven.V0,
		m_rtTopTopInven.U1, m_rtTopTopInven.V1,
		0xFFFFFFFF );

	nY += WAREHOUSE_TOP_HEIGHT;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_MIDDLE_HEIGHT,
		m_rtTopMiddleInven.U0, m_rtTopMiddleInven.V0,
		m_rtTopMiddleInven.U1, m_rtTopMiddleInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_MIDDLE_HEIGHT;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_MIDDLE_HEIGHT,
		m_rtTopMiddleInven.U0, m_rtTopMiddleInven.V0,
		m_rtTopMiddleInven.U1, m_rtTopMiddleInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_MIDDLE_HEIGHT;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_BOTTOM_HEIGHT,
		m_rtTopBottomInven.U0, m_rtTopBottomInven.V0,
		m_rtTopBottomInven.U1, m_rtTopBottomInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_BOTTOM_HEIGHT;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 1,
		m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
		m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
		0xFFFFFFFF );
	
	nY += 1;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_TRADE_HEIGHT,
		m_rtBottomInven.U0, m_rtBottomInven.V0,
		m_rtBottomInven.U1, m_rtBottomInven.V1,
		0xFFFFFFFF );

	nY += WAREHOUSE_TRADE_HEIGHT;

	if( m_bStorageWareHouse )
	{
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 23,
		m_rtPriceInven.U0, m_rtPriceInven.V0,
		m_rtPriceInven.U1, m_rtPriceInven.V1,
		0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 23,
		m_rtBlankInven.U0, m_rtBlankInven.V0,
		m_rtBlankInven.U1, m_rtBlankInven.V1,
		0xFFFFFFFF );
	}
	nY += 23;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 1,
		m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
		m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
		0xFFFFFFFF );

	// Bottom scroll bar		
	m_sbTopScrollBar.Render();

	// Close button
	m_btnClose.Render();

	if( m_bStorageWareHouse )
	{
		// Buy & sell button of warehouse
		m_btnWareHouseKeep.Render();
	}
	else
	{
		m_btnWareHouseTake.Render();
	}
	
	// Cancel button of warehouse
	m_btnWareHouseCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Render item information
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 823, "창고" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,	
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );

	if(m_bStorageWareHouse)
	{
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 824, "보관료" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY );	
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strTotalPrice, m_nPosX + WAREHOUSE_TRADEPRICE_POSX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY, _pUIMgr->GetNasColor( m_strTotalPrice ) );
	}

//	char tv_str[20]={'\0',};
//	itoa(m_useTime,tv_str,10);
	
	// ------만료기간 라카력으로 ---------wooss-060421--->>
//	tm* tv_t = new tm;
	int tv_time = m_useTime;
	int tv_year,tv_month,tv_day;
	CTString tv_str,tv_str2;
//	tv_time *=24;
	tv_year = tv_time/360;
	tv_time -= tv_year*360;
	tv_month = (tv_time)/30;
	tv_time -= tv_month*30;
	tv_day = tv_time;
//		tv_t = _pUIMgr->LClocaltime((time_t*)&m_useTime);
	if(tv_year >0 ){
		tv_str2.PrintF(  _S(2623, "%d년"),tv_year);
		tv_str+=tv_str2+" ";
	}
	if(tv_month >0 ){
		tv_str2.PrintF(  _S(2624, "%d개월"),tv_month);
		tv_str+=tv_str2+" ";
	}
	if(tv_day >0 ){
		tv_str2.PrintF(  _S(2625, "%d일"),tv_day);
		tv_str+=tv_str2+" ";
	}

	// -----------------------------------------------------------<<
#ifdef CHARGE_ITEM 
	//wooss 050811 050917 060207_thai 
	_pUIMgr->GetDrawPort()->PutTextEx( _S(2150, "확장 기간 : "), m_nPosX + WAREHOUSE_EXTEND_TEXT_POSX, m_nPosY + WAREHOUSE_EXTEND_TEXT_POSY ); 	
	_pUIMgr->GetDrawPort()->PutTextExRX(tv_str, m_nPosX + WAREHOUSE_EXTEND_DAYS_POSX+30, m_nPosY + WAREHOUSE_EXTEND_TEXT_POSY ,0xFFFFFFF);
#endif
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render Items
	RenderWareHouseItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIWareHouse::MouseMessage( MSG *pMsg )
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

			// Move warehouse
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
				// WareHouse items
				if( m_nSelWareHouseItemID >= 0 )
				{
					int	nSelRow = m_nSelWareHouseItemID / WAREHOUSE_WAREHOUSE_SLOT_COL;
					int	nSelCol = m_nSelWareHouseItemID % WAREHOUSE_WAREHOUSE_SLOT_COL;
					
					// Close message box of warehouse
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
					
					_pUIMgr->SetHoldBtn( m_abtnWareHouseItems[nSelRow][nSelCol] );
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
					
					m_abtnWareHouseItems[nSelRow][nSelCol].SetBtnState( UBES_IDLE );					
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of warehouse
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );

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
			// Buy button of warehouse
			else if( m_bStorageWareHouse && m_btnWareHouseKeep.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Sell button of warehouse
			else if( !m_bStorageWareHouse && m_btnWareHouseTake.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of warehouse
			else if( m_btnWareHouseCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Top Scroll bar
			else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}			
			// WareHouse
			else if( IsInsideRect( nX, nY, m_rcTop ) )
			{
				int	iRow, iCol;
				int	iRowS = m_nCurRow;								// Start Row
				int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;	// End Row
				int	nWhichRow = -1, nWhichCol = -1;
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
					{
						if( m_abtnWareHouseItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichRow = iRow;	
							nWhichCol = iCol;
						}
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichRow, nWhichCol );

				return WMSG_SUCCESS;
			}
			// Trade
			else if( IsInsideRect( nX, nY, m_rcBottom ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;	
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichItem );

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
				// Buy button of warehouse
				else if( m_bStorageWareHouse && m_btnWareHouseKeep.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Sell button of warehouse
				else if( !m_bStorageWareHouse && m_btnWareHouseTake.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of warehouse
				else if( m_btnWareHouseCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// WareHouse items
				else if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelWareHouseItemID = -1;
					m_nSelTradeItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;			// 4줄~
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
						{
							if( m_abtnWareHouseItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected item
								m_nSelWareHouseItemID = iCol + iRow * WAREHOUSE_WAREHOUSE_SLOT_COL;	// 선택된 슬롯의 아이템 ID

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, iRow, iCol );

								bLButtonDownInItem	= TRUE;

								_pUIMgr->RearrangeOrder( UI_WAREHOUSE, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					m_nSelTradeItemID = -1;
					m_nSelWareHouseItemID = -1;

					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							bLButtonDownInItem	= TRUE;

							_pUIMgr->RearrangeOrder( UI_WAREHOUSE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}				

				_pUIMgr->RearrangeOrder( UI_WAREHOUSE, TRUE );
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

				// If warehouse isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Buy button of warehouse
				else if( m_bStorageWareHouse && ( wmsgResult = m_btnWareHouseKeep.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 KeepItems();
						 
						 // Clear Member Value
						 //ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Sell button of warehouse
				else if( !m_bStorageWareHouse && ( wmsgResult = m_btnWareHouseTake.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 TakeItems();

						 // Clear Member Value
						 //ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of warehouse
				else if( ( wmsgResult = m_btnWareHouseCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Top Scroll bar
				else if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//wooss 050812
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					// Nothing
					return WMSG_SUCCESS;
				}				
				// WareHouse items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
						{
							if( m_abtnWareHouseItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
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
					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
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
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and is from warehouse
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_WAREHOUSE )
					{
						// Trade items
						if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from warehouse slot
							if( m_nSelTradeItemID < 0 ||
								m_abtnTradeItems[m_nSelTradeItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								AddWareHouseItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount() );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// WareHouse items
						else if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcTop ) )
						{
							int	nSelRow = m_nSelWareHouseItemID / WAREHOUSE_WAREHOUSE_SLOT_COL;
							int	nSelCol = m_nSelWareHouseItemID % WAREHOUSE_WAREHOUSE_SLOT_COL;
							// If this item is moved from trade slot
							if( m_nSelWareHouseItemID < 0 ||
								m_abtnWareHouseItems[nSelRow][nSelCol].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								DelWareHouseItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount(),
												m_nSelTradeItemID );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item
					else if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY && m_bStorageWareHouse)
					{
						// Trade items
						if( IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from warehouse slot
							if( m_nSelTradeItemID < 0 ||
								m_abtnTradeItems[m_nSelTradeItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								AddWareHouseItemFromInventory( );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					}

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
				if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	nTradeItemID = m_nSelTradeItemID;
					m_nSelTradeItemID = -1;

					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Close message box of warehouse
							_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );

							DelWareHouseItem( m_abtnTradeItems[iItem].GetItemRow(),
											m_abtnTradeItems[iItem].GetItemCol(),
											m_abtnTradeItems[iItem].GetItemUniIndex(),
											m_abtnTradeItems[iItem].GetItemCount(),
											nTradeItemID );

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// WareHouse items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelWareHouseItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
						{
							if( m_abtnWareHouseItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								if( !m_bStorageWareHouse )
								{
									// Not wearing, not refine stone, can trade
									CItems		&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
									CItemData	&rItemData = rItems.ItemData;
									if( rItems.Item_Wearing == -1 &&
										( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
										rItemData.GetFlag() & ITEM_FLAG_TRADE )
									{
										// Close message box of warehouse
										_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );

										AddWareHouseItem( iRow, iCol,
														m_abtnWareHouseItems[iRow][iCol].GetItemUniIndex(),
														m_abtnWareHouseItems[iRow][iCol].GetItemCount() );
									}
								}

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, iRow, iCol );

								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Top Scroll bar
				else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Top Scroll bar
				if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}				
			}
		}
		break;
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
static ULONG	ulItemPlus;
static ULONG	ulItemFlag;
static LONG		lItemUsed;
static SQUAD	llTempPrice;
static SBYTE	TempsbOptionType[MAX_ITEM_OPTION];
static SBYTE	TempsbOptionLevel[MAX_ITEM_OPTION];
static SQUAD	llMaxTempCount = -1;
static LONG		lRareIndex;


// ----------------------------------------------------------------------------
// Name : AddWareHouseItem()
// Desc : From warehouse to trade
// ----------------------------------------------------------------------------
void CUIWareHouse::AddWareHouseItem( int nRow, int nCol, int nUniIndex, SQUAD llCount )
{
	nTempRow		= nRow;
	nTempCol		= nCol;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempIndex		= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemIndex();
	ulItemPlus		= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemPlus();
	ulItemFlag		= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemFlag();
	lItemUsed		= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemUsed();
	llMaxTempCount	= -1;
	lRareIndex		= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemRareIndex();

	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		TempsbOptionType[i] = -1;
		TempsbOptionLevel[i] = 0;
	}

	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{	
		TempsbOptionType[sbOption]	= m_abtnWareHouseItems[nTempRow][nTempCol].GetItemOptionType( sbOption );
		TempsbOptionLevel[sbOption] = m_abtnWareHouseItems[nTempRow][nTempCol].GetItemOptionLevel( sbOption );
	}

	CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );

	// Ask quantity
	if( ( m_bStorageWareHouse && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !m_bStorageWareHouse && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_ITEM );
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );

		if( rItemData.GetType() == CItemData::ITEM_ETC &&
			rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			strMessage.PrintF( _S( 1323, "몇 나스를 옮기시겠습니까?" ) );		
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		}
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
		CTString strItemCount;
		strItemCount.PrintF( "%I64d", m_abtnWareHouseItems[nTempRow][nTempCol].GetItemCount() );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
	}
	else if( ulItemPlus > 0 && !(_pUIMgr->IsPet(rItemData) || _pUIMgr->IsWildPet(rItemData)))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_YESNO,		
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		WareHouseToTrade( llTempCount, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : AddWareHouseItemFromInventory()
// Desc : From inventory to trade
// ----------------------------------------------------------------------------
void CUIWareHouse::AddWareHouseItemFromInventory( )
{
	int nTab, nRow, nCol;
	nTempUniIndex	= _pUIMgr->GetHoldBtn().GetItemUniIndex();
	_pUIMgr->GetInventory()->GetLocationOfNormalItem( nTempUniIndex, nRow, nCol );

	if( nRow == -1 )
		return;
	
	nTab					= _pUIMgr->GetHoldBtn().GetItemTab();
	if(_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Wearing != -1)
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 825, "장착하고 있는 아이템은 보관할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	nTempIndex				= _pUIMgr->GetHoldBtn().GetItemIndex();
	CItemData	&rItemData	= _pNetwork->GetItemData( nTempIndex );
	const char* szItemName	= _pNetwork->GetItemName( nTempIndex );
	ulItemPlus				= _pUIMgr->GetHoldBtn().GetItemPlus();
	ulItemFlag				= _pUIMgr->GetHoldBtn().GetItemFlag();
	lItemUsed				= _pUIMgr->GetHoldBtn().GetItemUsed();
	nTempRow				= nRow;
	nTempCol				= nCol;
	llTempCount				= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum;
	llMaxTempCount			= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum;
	lRareIndex				= _pUIMgr->GetHoldBtn().GetItemRareIndex();

	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		TempsbOptionType[i] = -1;
		TempsbOptionLevel[i] = 0;
	}

	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{	
		TempsbOptionType[sbOption]	= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_OptionType[sbOption];
		TempsbOptionLevel[sbOption] = _pNetwork->MySlotItem[nTab][nRow][nCol].Item_OptionLevel[sbOption];
	}

	// Ask quantity
	if( ( m_bStorageWareHouse && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !m_bStorageWareHouse && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;

		// 나스의 경우.
		if( rItemData.GetType() == CItemData::ITEM_ETC &&
			rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
				UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_MONEY );
			//strMessage.PrintF( _S( 826, "보관하실 나스를 천 단위로 입력해 주십시요. 보관료는 %d%%입니다." ), m_nTex );		
			strMessage.PrintF(  _S( 960, "보관하실 나스를 입력해 주십시오." )  );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		else
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_ITEM );
			//const char	*szItemName = rItemData.GetName();
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
			CTString strItemCount;
			strItemCount.PrintF( "%I64d", llMaxTempCount );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
		}		
	}
	else if( ulItemPlus > 0 && !(_pUIMgr->IsPet(rItemData) || _pUIMgr->IsWildPet(rItemData)))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_YESNO,		
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		//const char	*szItemName = rItemData.GetName();
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		WareHouseToTrade( llTempCount, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : DelWareHouseItemToInventory()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::DelWareHouseItemToInventory( )
{
	if(m_nSelTradeItemID < 0)
		return;

	// FIXME : 수정이 필요한 부분.
	nTempRow		= _pUIMgr->GetHoldBtn().GetItemRow();
	nTempCol		= _pUIMgr->GetHoldBtn().GetItemCol();
	nTempUniIndex	= _pUIMgr->GetHoldBtn().GetItemUniIndex();
	llTempCount		= _pUIMgr->GetHoldBtn().GetItemCount();
	nTempTradeItemID= m_nSelTradeItemID;
	nTempIndex		= _pUIMgr->GetHoldBtn().GetItemIndex();
	ulItemPlus		= _pUIMgr->GetHoldBtn().GetItemPlus();
	ulItemFlag		= _pUIMgr->GetHoldBtn().GetItemFlag();
	lItemUsed		= _pUIMgr->GetHoldBtn().GetItemUsed();
	lRareIndex		= _pUIMgr->GetHoldBtn().GetItemRareIndex();

	CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
	const char* szItemName = _pNetwork->GetItemName( nTempIndex );
	
	// Ask quantity
	if( llTempCount > 1 )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,		
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_DEL_ITEM );
		//const char	*szItemName = rItemData.GetName();
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
		CTString strItemCount;
		strItemCount.PrintF( "%I64d", _pUIMgr->GetHoldBtn().GetItemCount() );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
	}
	else
	{
		TradeToWareHouse( llTempCount, ulItemPlus, ulItemFlag, lItemUsed );
	}
}

// ----------------------------------------------------------------------------
// Name : DelWareHouseItem()
// Desc : From trade to warehouse
// ----------------------------------------------------------------------------
void CUIWareHouse::DelWareHouseItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nTradeItemID )
{
	nTempRow		= nRow;
	nTempCol		= nCol;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempTradeItemID= nTradeItemID;
	nTempIndex		= m_abtnTradeItems[nTempTradeItemID].GetItemIndex();

	CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
	
	// Ask quantity
	if( llTempCount > 1 )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_DEL_ITEM );

		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		if( rItemData.GetType() == CItemData::ITEM_ETC &&
			rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			strMessage.PrintF( _S( 1323, "몇 나스를 옮기시겠습니까?" ) );		
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		}
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
		CTString strItemCount;
		strItemCount.PrintF( "%I64d", m_abtnTradeItems[nTempTradeItemID].GetItemCount() );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
	}
	else
	{
		TradeToWareHouse( llTempCount, ulItemPlus, ulItemFlag, lItemUsed );
	}
}

// ----------------------------------------------------------------------------
// Name : KeepItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::KeepItems()
{
	// Buy nothing
	if( m_nTotalPrice < 0 )
		return;
	// Can't stash items
	for( int i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; i++ )
	{
		// Add system message
		CTString tv_str;
		CItemData	&rItemData = _pNetwork->GetItemData( m_abtnTradeItems[i].GetItemIndex());
		if( rItemData.GetFlag()&ITEM_FLAG_NO_STASH){
			tv_str.PrintF(_S(2578, "저장할 수 없는 아이템[%s] 입니다." ),rItemData.GetName() );
			_pUIMgr->GetChatting()->AddSysMessage( tv_str, SYSMSG_ERROR );
			return;
		}
			
	}

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		_pUIMgr->GetChatting()->AddSysMessage( _S( 827, "나스가 부족하여 물품을 보관할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	// Check inventory space
	int	ctEmptySlot = 0;
	int	ctTradeItems = m_nNumOfItem;
	for( int iRow = 0; iRow < WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++ )
		{
			if( m_abtnWareHouseItems[iRow][iCol].IsEmpty() )
			{
				ctEmptySlot++;
				continue;
			}

			int	nIndex = m_abtnWareHouseItems[iRow][iCol].GetItemIndex();
			CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
			if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
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

	//_pNetwork->BuyItem( m_nSelectedWareHouseID, m_nNumOfItem, m_nTotalPrice );
	SendWareHouseKeepReq();

}

// ----------------------------------------------------------------------------
// Name : TakeItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::TakeItems()
{
	// Sell nothing
	if( m_nTotalPrice < 0 )
		return;

	//_pNetwork->SellItem( m_nSelectedWareHouseID, m_nNumOfItem, m_nTotalPrice );
	SendWareHouseTakeReq();
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
//		cpp2angel edit (04.12.18)
// ----------------------------------------------------------------------------
void CUIWareHouse::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_WAREHOUSE_REQ:
		{
			// [090527: selo] 확장팩 퀘스트 이야기 한다 처리 수정을 위한 루틴
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}

			switch ( nResult ) 
			{
			case WAREHOUSE_IN: // 물품을 맡긴다
				{
					m_bStorageWareHouse			= TRUE;
					_pUIMgr->GetSecurity()->OpenSecurity( m_bHasPassword );

#ifdef HELP_SYSTEM_1
// [KH_07043] 3차 도움말 관련 추가
					if(g_iShowHelp1Icon)
					{
						_pUIMgr->GetHelp3()->ClearHelpString();
						_pUIMgr->GetHelp3()->AddHelpString(_S(3313, "캐릭터의 인벤토리 창에서 창고에 보관 하고자 하는 물품을 클릭 후 창고의 하단 빈칸에 드래그 하여 올려 놓습니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3314, "보관버튼을 누르면 보관료만큼 자신의 인벤토리에서 나스가 빠져나가고 물품은 창고에 보관 됩니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3315, "※ 보관료는 맡기려는 물품 무게당 1나스 입니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3316, "※ 흰색 테두리가 둘러진 아이템은 창고에 보관이 불가능합니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3317, "※ 창고에 보관 가능한 공간은 기본 50칸이며, 창고확장카드를 사용하면 150칸으로 확장할 수 있습니다."));
						_pUIMgr->GetHelp3()->OpenHelp(this);
					}		
#endif
				}
				break;

			case WAREHOUSE_OUT: // 물품을 찾는다
				{	
					m_bStorageWareHouse			= FALSE;
					_pUIMgr->GetSecurity()->OpenSecurity( m_bHasPassword );
#ifdef HELP_SYSTEM_1
// [KH_07043] 3차 도움말 관련 추가
					if(g_iShowHelp1Icon)
					{
						_pUIMgr->GetHelp3()->ClearHelpString();
						_pUIMgr->GetHelp3()->AddHelpString(_S(3318, "창고에 보관된 물품 중 찾으려는 물품을 창고 창 아래의 빈칸으로 옮기고 찾기 버튼을 클릭 합니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3319, "찾은 물품은 자신의 인벤토리에 옮겨지게 됩니다."));
						_pUIMgr->GetHelp3()->AddHelpString(_S(3320, "※ 최대 10개의 아이템을 한번에 찾을 수 있습니다."));
						_pUIMgr->GetHelp3()->OpenHelp(this);
					}
#endif
				}
				break;

			case WAREHOUSE_TALK:
				{
					// 퀘스트 창 띄우기
					//_pUIMgr->GetQuest()->OpenQuest( _pUIMgr->GetCharacterInfo()->GetMobIndex(), m_fNpcX, m_fNpcZ );
					CUIQuestBook::TalkWithNPC();
				}
				break;
				
			case WAREHOUSE_CHANGEPW : // 암호를 변경한다.
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_SECURITY_REQ );
					
					_pUIMgr->CreateMessageBoxL( _S( 1743, "창고 보안설정" ) , UI_SECURITY, MSGLCMD_SECURITY_REQ );		
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, _S( 1778, "창고 보안을 위하여 암호를 설정 하실 수 있습니다." ), -1, 0xA3A1A3FF );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, CTString("  "), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, _S( 1779, "암호를 설정하면 창고를 이용할 때 설정하신 암호를 입력하여야만 창고를 이용할 수 있습니다." ), -1, 0xA3A1A3FF );	
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1780, "암호 설정" ), CUISecurity::SET_PASSWORD );
					if( m_bHasPassword )
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1781, "암호 분실" ), CUISecurity::UNSET_PASSWORD );
					}
					_pUIMgr->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1220, "취소한다." ) );
				}
				break;	
				
				// [090527: selo] 확장팩 퀘스트 수정
			case ciQuestClassifier:
				{
					// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;

			default:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
				}
				break;
			}
		}
		break;
	}
}

static SQUAD	llTempNas = 0;

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIWareHouse::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_WAREHOUSE_ADD_ITEM:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			for( int iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount > 0 && llCount <= llTempCount )
					WareHouseToTrade( llCount, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
				
			}
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_PLUSITEM:
		{
			WareHouseToTrade( 1, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_MONEY:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			for( int iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				//const int iUnit	= 1000;
				SQUAD	llCount = _atoi64( pcInput );
				//SQUAD	llNas	= (llCount / 1000) * 1000;				

				if( llCount > 0 && llCount <= llTempCount )
				{
					llTempNas		= llCount;
					//if( llCount % 1000 == 0 )
					//{
						WareHouseToTrade( llCount, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
						/*
					}
					else if( llNas > 0 )
					{
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL,	
							UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_REQ );
						strMessage.PrintF( _S( 828, "%d 나스 단위로만 보관이 가능합니다.  %I64d 나스를 보관하시겠습니까?" ), iUnit, llNas);	
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
						llTempNas = llNas;
					}
					else
					{
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OK,	
							UI_WAREHOUSE, MSGCMD_WAREHOUSE_ERROR );
						strMessage.PrintF( _S( 829, "%d 나스 단위로만 보관이 가능합니다." ), iUnit);	
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					*/
				}
			}
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_REQ:
		{
			WareHouseToTrade( llTempNas, ulItemPlus, ulItemFlag, lItemUsed, lRareIndex );
		}
		break;

	case MSGCMD_WAREHOUSE_ERROR:
		{
		}
		break;

	case MSGCMD_WAREHOUSE_DEL_ITEM:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			for( int iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount > 0 && llCount <= llTempCount )
					TradeToWareHouse( llCount, ulItemPlus, ulItemFlag, lItemUsed );
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::WareHouseToTrade( SQUAD llCount, ULONG ulPlus, ULONG ulFlag, LONG lUsed, LONG lRareIndex )
{	
	// Find same item in trade slot
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnTradeItems[iItem].GetItemUniIndex() == nTempUniIndex )
			break;
	}

	// If there is same item
	if( iItem < WAREHOUSE_TRADE_SLOT_TOTAL )
	{
		// Check if item is countable
		CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
		if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
		{
			// Update count of trade item
			SQUAD	llNewCount	= m_abtnTradeItems[iItem].GetItemCount();
			llNewCount			+= llCount;

			// 인벤에 가고 있는 갯수보다 많은 갯수의 아이템을 보관하려 할때...
			if(m_bStorageWareHouse && llMaxTempCount > 0)
			{
				if( llNewCount > llMaxTempCount )
					return;
			}
			
			m_abtnTradeItems[iItem].SetItemCount( llNewCount );
												
			if( !m_bStorageWareHouse )
			{
				// Update count of warehouse item
				llNewCount = m_abtnWareHouseItems[nTempRow][nTempCol].GetItemCount();
				llNewCount -= llCount;
				m_abtnWareHouseItems[nTempRow][nTempCol].SetItemCount( llNewCount );
				if( llNewCount <= 0 )
				{
					m_abtnWareHouseItems[nTempRow][nTempCol].SetEmpty( TRUE );

					// Unselect item
					if( m_nSelWareHouseItemID == ( nTempCol + nTempRow * WAREHOUSE_WAREHOUSE_SLOT_COL ) )
						m_nSelWareHouseItemID = -1;
				}
			}

			m_nTotalPrice = CalculateStoragePrice();
			m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
			_pUIMgr->InsertCommaToString( m_strTotalPrice );
			return;
		}
		else
		{
			if(m_bStorageWareHouse)
			{
				m_nSelWareHouseItemID = -1;
				return;
			}
		}
	}

	// Find empty slot
	for( iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			break;
	}

	// If there is not empty slot
	if( iItem == WAREHOUSE_TRADE_SLOT_TOTAL )
	{
		CTString	strMessage;

		// Add system message
		if( m_bStorageWareHouse )
			strMessage.PrintF( _S( 831, "한번에 최대 %d개의 아이템을 보관하실 수 있습니다." ), WAREHOUSE_TRADE_SLOT_TOTAL );	
		else
			strMessage.PrintF( _S( 832, "한번에 최대 %d개의 아이템을 찾으실 수 있습니다." ), WAREHOUSE_TRADE_SLOT_TOTAL );		

		_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	m_abtnTradeItems[iItem].SetItemInfo( 0, nTempRow, nTempCol, nTempIndex, nTempUniIndex, -1 );
	m_abtnTradeItems[iItem].SetItemCount( llCount );
	m_abtnTradeItems[iItem].SetItemPlus( ulPlus );
	m_abtnTradeItems[iItem].SetItemFlag( ulFlag );
	m_abtnTradeItems[iItem].SetItemUsed( lUsed );
	m_abtnTradeItems[iItem].SetItemRareIndex( lRareIndex );
	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{	
		m_abtnTradeItems[iItem].SetItemOptionData(sbOption, TempsbOptionType[sbOption], TempsbOptionLevel[sbOption]);				
	}
	
	if( !m_bStorageWareHouse )
	{												
		// Update count of warehouse item
		SQUAD	llNewCount = m_abtnWareHouseItems[nTempRow][nTempCol].GetItemCount();
		llNewCount -= llCount;
		m_abtnWareHouseItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount <= 0 )
		{
			m_abtnWareHouseItems[nTempRow][nTempCol].SetEmpty( TRUE );

			// Unselect item
			if( m_nSelWareHouseItemID == ( nTempCol + nTempRow * WAREHOUSE_WAREHOUSE_SLOT_COL ) )
				m_nSelWareHouseItemID = -1;
		}
	}

	m_nTotalPrice = CalculateStoragePrice();
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::TradeToWareHouse( SQUAD llCount, ULONG ulPlus, ULONG ulFlag, LONG lUsed )
{
	// Update count of trade item
	SQUAD	llNewCount = m_abtnTradeItems[nTempTradeItemID].GetItemCount();
	llNewCount -= llCount;
	m_abtnTradeItems[nTempTradeItemID].SetItemCount( llNewCount );

	if( llNewCount <= 0 )
	{
		// Rearrange buttons
		for( int iArrItem = nTempTradeItemID; iArrItem < WAREHOUSE_TRADE_SLOT_TOTAL; iArrItem++ )
		{
			if( ( iArrItem + 1 ) == WAREHOUSE_TRADE_SLOT_TOTAL )
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

	if( !m_bStorageWareHouse )
	{
		// Update count of warehouse item
		llNewCount = m_abtnWareHouseItems[nTempRow][nTempCol].GetItemCount();
		llNewCount += llCount;
		m_abtnWareHouseItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount > 0 )
			m_abtnWareHouseItems[nTempRow][nTempCol].SetEmpty( FALSE );
	}

	m_nTotalPrice = CalculateStoragePrice();
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 보관료를 계산합니다.
// Input  :
//-----------------------------------------------------------------------------
SQUAD CUIWareHouse::CalculateStoragePrice()
{
	const int iTexPerWeight	= 1;	// 무게 1당 세금.
	SQUAD llTotalPrice		= 0;
	SQUAD llTotalWeight		= 0;
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{	
		if(m_abtnTradeItems[iItem].IsEmpty())
			continue;

		const int iIndex	= m_abtnTradeItems[iItem].GetItemIndex();
		const SQUAD llCount	= m_abtnTradeItems[iItem].GetItemCount();
		CItemData& ID = _pNetwork->GetItemData(iIndex);
		if(ID.GetWeight() != 0)
		{
			llTotalWeight += ID.GetWeight() * llCount;
		}
	}

	// 돈을 따로 계산할것.

	// 무게 1당 3나스.
	llTotalPrice += llTotalWeight * iTexPerWeight;
	return llTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseKeepReq
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::SendWareHouseKeepReq()
{
	LONG lKeepCount = 0;
	for(int i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
	{
		if(m_abtnTradeItems[i].IsEmpty())
			continue;
		lKeepCount++;
	}

	if(lKeepCount <= 0)
	{
		return;
	}

	// password(str) keepcount(n) [row(c) col(c) item_idx(n) count(ll)]:keepcount
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_KEEP_REQ;
	nmWareHouse << m_strPW;

	nmWareHouse << lKeepCount;

	for(i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
	{
		if(m_abtnTradeItems[i].IsEmpty())
			continue;
		SBYTE sbRow		= m_abtnTradeItems[i].GetItemRow();
		SBYTE sbCol		= m_abtnTradeItems[i].GetItemCol();
		LONG lIndex		= m_abtnTradeItems[i].GetItemUniIndex();
		SQUAD llCount	= m_abtnTradeItems[i].GetItemCount();

		nmWareHouse << sbRow;
		nmWareHouse << sbCol;
		nmWareHouse << lIndex;
		nmWareHouse << llCount;
	}
	
	_pNetwork->SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseTakeReq
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::SendWareHouseTakeReq()
{
	LONG lTakeCount = 0;
	for(int i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
	{
		if(m_abtnTradeItems[i].IsEmpty())
			continue;
		lTakeCount++;
	}

	if(lTakeCount <= 0)
	{
		return;
	}

	// 찾기 요청				: password(str) takecount(n) [item_idx(n) count(ll)]:takecount
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_TAKE_REQ;
	nmWareHouse << m_strPW;

	nmWareHouse << lTakeCount;

	for(i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
	{
		if(m_abtnTradeItems[i].IsEmpty())
			continue;		
		LONG lIndex		= m_abtnTradeItems[i].GetItemUniIndex();
		SQUAD llCount	= m_abtnTradeItems[i].GetItemCount();
		
		nmWareHouse << lIndex;
		nmWareHouse << llCount;
	}
	
	_pNetwork->SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: ReceiveWareHouseListRep
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::ReceiveWareHouseListRep(CNetworkMessage *istr)
{
	// 창고 아이템 리스트		: listflag(c) item_idx(n) plus(n) flag(n) used(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)
	SBYTE	sbListFlag;
	LONG	lIndex;
	LONG	lUniIndex;
	LONG	lPlus;
	LONG	lFlag;
	LONG	lUsed;
	LONG	lUsed2;
	SBYTE	sbOptionCount;
	SQUAD	llCount;
	LONG	useTime;

	(*istr) >> sbListFlag;

	if( sbListFlag & ITEM_LIST_EMPTY )	// Empty
	{
		_pUIMgr->GetSecurity()->ResetSecurity();
		_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_WAREHOUSE );
		_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
		//_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
		PrepareWareHouse();
		return;
	}

	(*istr) >> lUniIndex;
	(*istr) >> lIndex;
	(*istr)	>> lPlus;
	(*istr)	>> lFlag;
	(*istr)	>> lUsed;
	(*istr) >> lUsed2;
	(*istr) >> sbOptionCount;

	CItems Item;
	Item.InitOptionData();

		SBYTE sbOptionType;
		SBYTE sbOptionLevel;
	//레어 아이템이면...

	if( _pNetwork->GetItemData(lIndex).GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if( sbOptionCount ==0)
			Item.SetRareIndex(0);
		//감정된 레어 아이템
		else
			_pUIMgr->SetRareOption(istr, Item);
	}
	//레어 아이템이 아니면...
	else
	{
		for(int i = 0; i < sbOptionCount; ++i)
		{
			(*istr) >> sbOptionType;
			(*istr) >> sbOptionLevel;
			Item.SetOptionData( i, sbOptionType, sbOptionLevel );
		}
	}

	(*istr) >> llCount;	
	
	(*istr) >> useTime;
	_pUIMgr->GetWareHouse()->SetUseTime(useTime);	//wooss 050817

	Item.SetData( lIndex, lUniIndex, -1, -1, -1, lPlus, lFlag, lUsed, lUsed2, -1, llCount);
	
	m_vectorStorageItems.push_back(Item);

	if( sbListFlag & ITEM_LIST_END )	// End
	{
		_pUIMgr->GetSecurity()->ResetSecurity();
		_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_WAREHOUSE );
		_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
		PrepareWareHouse();
	}
}