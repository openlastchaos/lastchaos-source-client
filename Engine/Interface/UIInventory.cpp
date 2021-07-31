#include "stdh.h"
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/PetInfo.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UISelChar.h>
#include <Engine/Interface/UITatoo.h>
#include <Engine/LocalDefine.h>

extern INDEX g_iCountry;

tm* g_tv_t = NULL; // 기간 표시용 전역 포인터 변수

#define IN_VALIDTM(a) if (a) {delete a; a=NULL;}

enum EInvenTex
{
	EInvenBg = 0,
	EInvenNormalSelect,
	EInvenWearSelect,
	EInvenMixing,
	EInvenSealed,
	EInvenLeaseMark,
	EInvenInfoUL,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoUM,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoUR,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoML,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoMM,			//////////////////아이템 정보 표시////////////////////////////////////////
	EInvenInfoMR,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoBL,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoBM,			//////////////////////////////////////////////////////////////////////////
	EInvenInfoBR,			//////////////////////////////////////////////////////////////////////////
	EInvenSimpleInfoL,
	EInvenSimpleInfoM,
	EInvenSimpleInfoR,
	EInvenTotal,
};

//#define ADJUST_MEMORIZE_SKILL 
// ----------------------------------------------------------------------------
// Name : CUIInventory()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIInventory::CUIInventory()
{
	m_bLockInven = FALSE;
	m_eLockInvenProcess = LOCK_NONE;
	m_nSelWearItemID = -1;

	int i;

	for (i=0; i<INVEN_TAB_TOTAL; ++i)
	{
		m_nSelSlotItemID[i] = -1;
	}

	m_bShowItemInfo = FALSE;
	m_bDetailItemInfo = FALSE;
	m_nCurInfoLines = 0;
	//m_strMyMoney = CTString( "999,999,999,999" );
	m_strMyMoney = CTString( "0" );
	//m_strWeight = CTString( "100.00%" );
	m_strWeight = CTString( "0.0%" );
	m_strNumItems = CTString( "100/100" );
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
	m_row = m_col =0; // wooss 050926

	m_iRareGrade =-1;
	m_NewInvenSurface.Clear();
	m_InvenCurrentTab = INVEN_TAB_NORMAL0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIInventory()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIInventory::~CUIInventory()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, nWidth, 36 );
	m_rcWearing.SetRect( 35, 45, 220, 183 );
	m_rcItemTypeTab[0].SetRect( 11, 214, 32, 361 );
	m_rcItemTypeTab[1].SetRect( 223, 214, 244, 338 );
	m_rcItemSlot.SetRect( 33, 214, 222, 400 );

	// Create inventory texture
	CTextureData* tmpTexture = CreateTexture(CTString("Data\\Interface\\NewInventory.tex"));
	FLOAT	fTexWidth = tmpTexture->GetPixWidth();
	FLOAT	fTexHeight = tmpTexture->GetPixHeight();

	m_NewInvenSurface.AddRectSurface(UIRect(0,0,255,407), UIRectUV(0,0,255,407,fTexWidth,fTexHeight)); // 배경
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(3,441,36,474,fTexWidth,fTexHeight)); // 인벤 아이템 공간 선택이미지
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(41,441,74,474,fTexWidth,fTexHeight)); // 인벤 장비 공간 선택이미지
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(79,441,112,474,fTexWidth,fTexHeight)); // 인벤 조합 중인 버튼 이미지
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(117,441,150,474,fTexWidth,fTexHeight)); // 인벤 봉인 중인 버튼 이미지
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(173,421,184,432,fTexWidth,fTexHeight)); // 인벤 대여 마크 이미지

	m_btnTrashcan.Create(this, _s(" "), 236, 193, 15, 19);
	m_btnTrashcan.SetNewType(TRUE);
	m_btnTrashcan.SetRTSurface(UBS_IDLE, UIRect(0,0,15,19), UIRectUV(3,413,17,431,fTexWidth,fTexHeight));
	m_btnTrashcan.SetRTSurface(UBS_CLICK, UIRect(0,0,15,19), UIRectUV(21,413,35,431,fTexWidth,fTexHeight));
	m_btnTrashcan.CopyRTSurface(UBS_IDLE, UBS_ON);
	m_btnTrashcan.CopyRTSurface(UBS_IDLE, UBS_DISABLE);

	tmpTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	fTexWidth = tmpTexture->GetPixWidth();
	fTexHeight = tmpTexture->GetPixHeight();

	m_btnTab[INVEN_TAB_NORMAL0].Create(this, _s(" "), 11, 214, 22, 34);
	m_btnTab[INVEN_TAB_NORMAL1].Create(this, _s(" "), 11, 252, 22, 34);
	m_btnTab[INVEN_TAB_NORMAL2].Create(this, _s(" "), 11, 290, 22, 34);
	m_btnTab[INVEN_TAB_NORMAL3].Create(this, _s(" "), 11, 328, 22, 34);
	m_btnTab[INVEN_TAB_NORMAL4].Create(this, _s(" "), 11, 336, 22, 34); // 현재 사용 안함

	UIRect top = UIRect(0,0,22,11);	// 11
	UIRect middle = UIRect(0,11,22,23); // 12
	UIRect bottom = UIRect(0,23,22,34);	// 11

	int i;
	for (i=0; i<INVEN_TAB_EVENT; ++i)
	{
		m_btnTab[i].SetNewType(TRUE);
		m_btnTab[i].SetRTSurface(UBS_IDLE, top, UIRectUV(86,0,107,10,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_IDLE, middle, UIRectUV(86,11,107,22,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_IDLE, bottom, UIRectUV(86,90,107,100,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_IDLE, UIRect(7,11,20,24), UIRectUV(154,91,167,104,fTexWidth,fTexHeight)); // 아이콘
		m_btnTab[i].SetRTSurface(UBS_CLICK, top, UIRectUV(59,0,80,10,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_CLICK, middle, UIRectUV(59,11,80,22,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_CLICK, bottom, UIRectUV(59,90,80,100,fTexWidth,fTexHeight));
		m_btnTab[i].SetRTSurface(UBS_CLICK, UIRect(7,11,20,24), UIRectUV(139,91,152,104,fTexWidth,fTexHeight)); // 아이콘
		m_btnTab[i].CopyRTSurface(UBS_CLICK, UBS_ON);
		m_btnTab[i].CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	}

	m_btnTab[INVEN_TAB_EVENT].Create(this, _s(" "), 223, 214, 22, 61);
	m_btnTab[INVEN_TAB_QUEST].Create(this, _s(" "), 223, 278, 22, 61);
	m_btnTab[INVEN_TAB_EVENT].SetNewType(TRUE);
	m_btnTab[INVEN_TAB_QUEST].SetNewType(TRUE);
	top = UIRect(0,0,22,11);	// 11	
	middle = UIRect(0,11,22,50);	// 39
	bottom = UIRect(0,50,22,61);	// 11
	// 버튼의 Idle, click 상태 이미지 설정
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_IDLE, top, UIRectUV(26,0,47,10,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_IDLE, middle, UIRectUV(26,11,47,49,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_IDLE, bottom, UIRectUV(26,90,47,100,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_CLICK, top, UIRectUV(0,0,21,10,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_CLICK, middle, UIRectUV(0,11,21,49,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_CLICK, bottom, UIRectUV(0,90,21,100,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_QUEST].GetRectSurface(UBS_IDLE) = m_btnTab[INVEN_TAB_EVENT].GetRectSurface(UBS_IDLE);
	m_btnTab[INVEN_TAB_QUEST].GetRectSurface(UBS_CLICK) = m_btnTab[INVEN_TAB_EVENT].GetRectSurface(UBS_CLICK);
	// 버튼이미지위의 아이콘 이미지 
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_IDLE, UIRect(3,23,16,36), UIRectUV(124,91,137,104,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_EVENT].SetRTSurface(UBS_CLICK, UIRect(3,23,16,36), UIRectUV(109,91,122,104,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_QUEST].SetRTSurface(UBS_IDLE, UIRect(3,23,16,36), UIRectUV(184,91,197,104,fTexWidth,fTexHeight));
	m_btnTab[INVEN_TAB_QUEST].SetRTSurface(UBS_CLICK, UIRect(3,23,16,36), UIRectUV(169,91,182,104,fTexWidth,fTexHeight));
	// Copy --> click => on, idle => disable
	m_btnTab[INVEN_TAB_EVENT].CopyRTSurface(UBS_CLICK, UBS_ON);
	m_btnTab[INVEN_TAB_EVENT].CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnTab[INVEN_TAB_QUEST].CopyRTSurface(UBS_CLICK, UBS_ON);
	m_btnTab[INVEN_TAB_QUEST].CopyRTSurface(UBS_IDLE, UBS_DISABLE);

	m_btnClose.Create(this, CTString(""), 232, 4, 16, 16);
	m_btnClose.SetUV(UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 229, 33, 244, 48, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	// Detail Info
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,137,19,156,fTexWidth,fTexHeight));	// 20 * 20
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,137,120,156,fTexWidth,fTexHeight));	// 101 * 20
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,137,140,156,fTexWidth,fTexHeight));	// 20 * 20

	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,157,19,206,fTexWidth,fTexHeight));	// 20 * 50
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,157,120,206,fTexWidth,fTexHeight)); // 101 * 50
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,157,140,206,fTexWidth,fTexHeight)); // 20 * 50

	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,207,19,226,fTexWidth,fTexHeight));	// 20 * 20
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,207,120,226,fTexWidth,fTexHeight)); // 101 * 20
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,207,140,226,fTexWidth,fTexHeight));	// 20 * 20
	
	// Simple Info 
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,229,11,250,fTexWidth,fTexHeight));	// 12 * 22
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(16,229,56,250,fTexWidth,fTexHeight)); // ?? * 22
	m_NewInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(73,229,84,250,fTexWidth,fTexHeight));	// 12 * 22

	m_abtnWearing[WEAR_HELMET].Create( this, 112, 46, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_JACKET].Create( this, 112, 81, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_WEAPON].Create( this, 77, 134, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );  
	m_abtnWearing[WEAR_PANTS].Create( this, 112, 116, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_SHIELD].Create( this, 147, 99, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_GLOVES].Create( this, 77, 99, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_BOOTS].Create( this, 112, 151, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );

	m_abtnWearing[WEAR_ACCESSORY1].Create( this, 36, 46, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_ACCESSORY2].Create( this, 36, 81, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_ACCESSORY3].Create( this, 36, 116, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );
	m_abtnWearing[WEAR_ACCESSORY4].Create( this, 188, 46, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM );

	// 이제 스크롤이 없어짐으로, 아이템 버튼의 위치를 미리 갱신하여 둔다.
	// 한 탭당 아이템 버튼은 5 * 5의 배치로 이루어 진다.
	const int tmpX = 40;
	const int tmpY = 220;
	const int btngap = 4;
	int iRow, iCol, iX=tmpX, iY=tmpY;

	for (iRow=0; iRow <INVEN_SLOT_ROW_TOTAL; ++iRow)
	{
		if ((iRow%INVEN_SLOT_ROW) == 0)
		{
			iY = tmpY;
		}
		
		for (iCol=0; iCol <INVEN_SLOT_COL; ++iCol)
		{
			m_abtnItems[0][iRow][iCol].Create(this, iX, iY, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM, 0, iRow, iCol);
			m_abtnItems[1][iRow][iCol].Create(this, iX, iY, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM, 1, iRow, iCol);
			m_abtnItems[2][iRow][iCol].Create(this, iX, iY, BTN_SIZE, BTN_SIZE, UI_INVENTORY, UBET_ITEM, 2, iRow, iCol);
			iX += (BTN_SIZE+btngap);
		}
		
		iX = tmpX;
		iY += (BTN_SIZE+btngap);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ShowLockErrorMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ShowLockErrorMessage()
{
	CTString	strError;
	switch( m_eLockInvenProcess )
	{
	case LOCK_UPGRADE_ITEM:
		strError = _S( 837, "아이템 업그레이드가 진행중입니다." );		
		break;
	case LOCK_BLOOD_OPTION:
		strError = _S( 838, "블러드 옵션 작업이 진행중입니다." );	
		break;
	case LOCK_REFINE:
		strError = _S( 839, "제련이 진행중입니다." );					
		break;
	case LOCK_COMPOUND:
		strError = _S( 840, "힘의 상자를 합성중입니다." );				
		break;
	case LOCK_MIX:
		strError = _S( 841, "재활의 상자를 조합중입니다." );			
		break;
	case LOCK_GW_MIX:
		strError = _S( 1928, "공성 아이템을 조합중입니다." );			
		break;
	case LOCK_PROCESS:
		strError = _S( 842, "가공 작업이 진행중입니다." );				
		break;
	case LOCK_PRODUCT:
		strError = _S( 843, "제작 작업이 진행중입니다." );				
		break;
	case LOCK_REMISSION:
		strError = _S( 844, "면죄부 작업이 진행중입니다." );			
		break;
	case LOCK_EXCHANGE:
		strError = _S( 845, "교환을 진행중입니다." );					
		break;
	case LOCK_SHOP:
		strError = _S( 846, "상점을 이용중입니다." );					
		break;
	case LOCK_PERSONAL_SHOP:
		strError = _S( 847, "개인 상점을 이용중입니다." );				
		break;
	case LOCK_WAREHOUSE:
		strError = _S( 848, "창고를 이용중입니다." );					
		break;
	case LOCK_CHANGEWEAPON:
		strError = _S( 1186, "무기 교체 중입니다." );			
		break;
	case LOCK_QUEST:
		strError = _S( 1669, "퀘스트 문서를 읽고 있는 중입니다." );			
		break;
	default:
		return;
	}

	_pUIMgr->GetChatting()->AddSysMessage( strError, SYSMSG_ERROR );
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
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

// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc :
// ----------------------------------------------------------------------------
// FIXME : 중복되는 코드가 너무 많음.
void CUIInventory::GetItemInfo( int nTab, int nRow, int nCol, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;
	int nItemleveldown = 0;

	CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;
	CItemRareOption rItemRareOption =_pNetwork->GetRareOptionData(0);

	if(  rItemData.GetFlag() & ITEM_FLAG_RARE )
	{
		m_bRareItem =TRUE;
		INDEX iRare =rItems.Item_RareIndex;
		rItemRareOption =_pNetwork->GetRareOptionData(iRare);
		m_iRareGrade = rItemRareOption.GetGrade();
	}
	else
	{
		m_bRareItem =FALSE;
		m_iRareGrade =-1;
	}


	const CTString strItemName =_pNetwork->GetItemName( rItemData.GetItemIndex() );

	CTString szItemName =strItemName;
	if( m_bRareItem )
	{
		CTString strPrefix = rItemRareOption.GetPrefix();
		if( strPrefix.Length() >0)
			szItemName.PrintF("%s %s", strPrefix, strItemName);
	}

	COLOR colNas = 0xF2F2F2FF;

	// Get item name
	if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
	{
		CTString	strCount;
		
		strCount.PrintF( "%I64d", rItems.Item_Sum );
		_pUIMgr->InsertCommaToString( strCount );
		strTemp.PrintF( "%s(%s)", szItemName, strCount );

		colNas = _pUIMgr->GetNasColor( rItems.Item_Sum );		

		AddItemInfoString( strTemp, colNas );
	}
	else
	{
		if( ( ( rItemData.GetType() == CItemData::ITEM_WEAPON && !rItemData.IsProductionItem() )||
			rItemData.GetType() == CItemData::ITEM_SHIELD ) )
		{
			if(rItems.IsFlag(PLATINUM_MAX_PLUS) || rItems.IsFlag(FLAG_ITEM_COMPOSITION) || rItemData.IsFlag(ITEM_FLAG_COMPOSITE)
				|| rItemData.IsFlag(ITEM_FLAG_CASH))
			{
			
				if(rItems.Item_Plus > 0){
					
					int plat_plus,diff;
					FLAG_ITEM_PLATINUM_GET(rItems.Item_Flag,plat_plus);
					diff = rItems.Item_Plus - plat_plus;

					strTemp.PrintF( "%s",szItemName);
					CTString strMsg;
					if(diff > 0)
					{
						strMsg.PrintF("+%d",diff) ;
						strTemp +=strMsg;
					}
					if(plat_plus > 0)
					{
						strMsg.PrintF("[+%d]",plat_plus) ;
						strTemp +=strMsg;
					}
						
					AddItemInfoString( strTemp, colNas );
								
				}
				else {
					strTemp.PrintF( "%s",szItemName);
					AddItemInfoString( strTemp );
				}
				
				CTString strTime1, strTime2;

				if (rItemData.IsFlag(ITEM_FLAG_COMPOSITE))
				{
					strTime1 = _S(3219, "의상만료 : %d년%d월%d일%d시");
				}

				if(rItems.IsFlag(FLAG_ITEM_COMPOSITION))
				{
					strTemp.PrintF( _S( 2706, "능력치 결합 효과"));
					AddItemInfoString( strTemp, 0xFFAA44FF );
					strTime2 = _S(3220, "결합만료 : %d년%d월%d일%d시");

/*					if(rItems.IsFlag(FLAG_ITEM_COMPOSITION) && !rItemData.IsFlag(ITEM_FLAG_COMPOSITE))
					{
						strTemp += strTemp + _S(3045,"(봉인)");
					}*/
				}
				else if(rItems.IsFlag(PLATINUM_MAX_PLUS)){
						strTemp.PrintF( _S( 2675, "플레티늄 제련 효과"));
						AddItemInfoString( strTemp, 0xFFAA44FF );

						strTime1 = _S(2525,"만료 : %d년%d월%d일%d시");
						strTime2 = CTString("");
				}else if(rItemData.IsFlag(ITEM_FLAG_CASH))
				{
					strTime1 = _S(2525,"만료 : %d년%d월%d일%d시");
					strTime2 = CTString("");
				}

				if (rItems.Item_Used > 0)
				{
					time_t tv_used = rItems.Item_Used - _pUIMgr->GetRadar()->GetStartTime();
					g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
					if(g_iCountry == BRAZIL) // 070531 ttos : 브라질 만료날짜 표기 수정 (일/월/년 시)
					{
						strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_yday+1,g_tv_t->tm_mon+1,g_tv_t->tm_year+1,g_tv_t->tm_hour);
					}else
					{
						strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
					}
					
					AddItemInfoString( strTemp, 0xFFAA44FF );
					IN_VALIDTM(g_tv_t)
				}

				// ITEM_FLAG_ABS 플래그가 있는 아이템은 Item_Used2에 남은시간이 있다.(기간이 아닌 시간(초))
				// 현재는 Item_Used만으로 표시한다.
				if (rItems.Item_Used2 > 0 && !rItemData.IsFlag(ITEM_FLAG_ABS))
				{
					if ((rItems.Item_Used2 - rItems.Item_Used) < 0)
					{
						time_t tv_used = rItems.Item_Used2 - _pUIMgr->GetRadar()->GetStartTime();
						g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);				
						strTemp.PrintF(strTime2,g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
						AddItemInfoString( strTemp, 0xFFAA44FF );
						IN_VALIDTM(g_tv_t)
					}
				}
			}
			else if(rItems.IsFlag(FLAG_ITEM_LENT))
			{
				CTString tv_str=_S(3046,"대여");
				tv_str+= CTString(" ")+szItemName;
				if(rItems.Item_Plus > 0){
					strTemp.PrintF( "%s +%d", tv_str, rItems.Item_Plus );
					AddItemInfoString( strTemp, colNas );
				}
				else {
					strTemp.PrintF( "%s",tv_str);
					AddItemInfoString( strTemp );
				}
				if(rItems.Item_Used > 0)
				{
					time_t tv_used = rItems.Item_Used - _pUIMgr->GetRadar()->GetStartTime();
					g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
					if(g_iCountry == BRAZIL) // 070531 ttos : 브라질 만료날짜 표기 수정 (일/월/년 시)
					{
						strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_yday+1,g_tv_t->tm_mon+1,g_tv_t->tm_year+1,g_tv_t->tm_hour);
					}else
					{
						strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
					}
			
					AddItemInfoString( strTemp, 0xFFAA44FF );
					
					IN_VALIDTM(g_tv_t)
				}
			}
			else if (rItems.IsFlag(FLAG_ITEM_LEVELDOWN))
			{
				strTemp = _S(3343, "아이템 레벨제한 감소효과");
				AddItemInfoString(strTemp, 0xFFAA44FF);
				nItemleveldown = 6;

				if (rItems.Item_Used2 > 0)
				{
					time_t tv_used = rItems.Item_Used2 - _pUIMgr->GetRadar()->GetStartTime();
					g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
					strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
			
					AddItemInfoString( strTemp, 0xFFAA44FF );
					
					IN_VALIDTM(g_tv_t)
				}
			}
			else {
				if(rItems.Item_Plus > 0){
					strTemp.PrintF( "%s +%d", szItemName, rItems.Item_Plus );
					AddItemInfoString( strTemp, colNas );
				}
				else {
					strTemp.PrintF( "%s",szItemName);
					AddItemInfoString( strTemp );
				}
			}
		}
		else if( rItemData.GetType() == CItemData::ITEM_ACCESSORY)
		{
#ifdef Pet_IMPROVEMENT_1ST
			// [070604: Su-won] 1차 펫 기능 개선
			// 펫의 진화단계 표시
			if( rItemData.GetSubType() ==CItemData::ACCESSORY_PET)
			{
				const INDEX iPetIndex = rItems.Item_Plus;
				SBYTE sbPetTypeGrade =_pUIMgr->GetPetInfo()->GetPetTypeGrade( iPetIndex );

				if( sbPetTypeGrade >0 )
				{
					INDEX iPetType;
					INDEX iPetAge;
					_pNetwork->CheckPetType( sbPetTypeGrade, iPetType, iPetAge );
					szItemName +=CTString(" - ")+PetInfo().GetName(iPetType, iPetAge);
				}
			}
#endif

			AddItemInfoString( szItemName, colNas );
		}
		else
		{
			if( rItems.IsFlag( FLAG_ITEM_BOOSTER_ADDED ) )
			{
				strTemp.PrintF(_S( 1929, "부스트 %s" ), szItemName);	
			}
			else if( rItems.IsFlag( FLAG_ITEM_SILVERBOOSTER_ADDED ) )
			{
				strTemp.PrintF(_S( 2429, "실버 부스트 %s" ), szItemName);	
			}
			else if( rItems.IsFlag( FLAG_ITEM_GOLDBOOSTER_ADDED ) )
			{
				strTemp.PrintF(_S( 2430,"골드 부스트 %s" ), szItemName);	
			}
			else if( rItems.IsFlag( FLAG_ITEM_PLATINUMBOOSTER_ADDED ) )
			{
				strTemp.PrintF(_S( 2639, "플래티늄 부스트 %s" ), szItemName);	
			}
			else
			{
				strTemp.PrintF("%s", szItemName);
			}
			
			AddItemInfoString( strTemp, colNas );
		}
	}
	
	// 기간제 악세사리와 광속 아이템의 경우 만료 일자 표시
	if(rItemData.GetType() == CItemData::ITEM_ACCESSORY
		|| rItemData.GetItemIndex()==2407 || rItemData.GetItemIndex()==2408 || rItemData.GetItemIndex()==2609)
	{
		if(rItems.Item_Used > 0)
		{
			time_t tv_used = rItems.Item_Used - _pUIMgr->GetRadar()->GetStartTime();
			g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
			if(g_iCountry == BRAZIL) // 070531 ttos : 브라질 만료날짜 표기 수정 (일/월/년 시)
			{
				strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_yday+1,g_tv_t->tm_mon+1,g_tv_t->tm_year+1,g_tv_t->tm_hour);
			}else
			{
				strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
			}
						
			AddItemInfoString( strTemp, 0xFFAA44FF );
		}
	}
	

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		COLOR colString =0xFFFFFFFF;
		
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				//초고급 제련석 여부
				//TEMP : 초고급 제련석 관련
				//대여 아이템 추가...
				if( rItems.IsFlag(FLAG_ITEM_LENT))
				{
					CTString strLeaseItem(_S(3047,"제련석 사용불가" ));
					AddItemInfoString( strLeaseItem, 0xFF4040FF);

				}
				else if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) ) // 업 그레이드 가능 아이템 
				{
					BOOL bCanUseSuperGOJE = !rItems.IsFlag( FLAG_ITEM_SUPER_STONE_USED );
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
				AddItemInfoString( strTemp, 0xFFFFFFFF );
#else
				AddItemInfoString( strTemp, 0x9E9684FF );
#endif
				// Level
				int	nItemLevel = rItemData.GetLevel();
				int nWearLevelReduction =rItems.GetWearLevelReduction();
				if( nWearLevelReduction >0 )
					strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction );
				else
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp,	0xFFFFFFFF );
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
				if( rItems.Item_Plus > 0 )
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalAttack() *
						//					pow( ITEM_PLUS_COFACTOR, rItems.Item_Plus ) ) - rItemData.GetPhysicalAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), rItems.Item_Plus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), nBasePhysicalAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack);
							
					
						AddItemInfoString( strTemp, colString );

						if( rItems.Item_Plus >= 15 )
						{
							strTemp.PrintF(_S( 1891, "물리 공격력 + 75"));
							AddItemInfoString( strTemp, colString );
						}
					}

					// Magic attack
					if( rItemData.GetMagicAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetMagicAttack() *
						//					pow( ITEM_PLUS_COFACTOR, rItems.Item_Plus ) ) - rItemData.GetMagicAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), rItems.Item_Plus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), nBaseMagicAttack, nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack);
															
						AddItemInfoString( strTemp, colString );

						if( rItems.Item_Plus >= 15 )
						{
							strTemp.PrintF(_S(1892,"마법 공격력 + 50"));
							AddItemInfoString( strTemp, colString );
						}
					}
				}
				else
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack);
						AddItemInfoString( strTemp, colString );		//흰색으로 표시
					}

					// Magic attack
					if( rItemData.GetMagicAttack() > 0 )
					{
						strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
						AddItemInfoString( strTemp, colString );
					}
				}

				// Penalty
				int	nPenalty = 0;
				int	nDiffLevel = nItemLevel - _pNetwork->MyCharacterInfo.level;
				if( nDiffLevel > 12 )
					nPenalty = 90;
				else if( nDiffLevel > 8 )
					nPenalty = 70;
				else if( nDiffLevel > 4 )
					nPenalty = 50;
				if( nPenalty > 0 && !m_bRareItem)
				{
					strTemp.PrintF( _S( 1029, "패널티 : 공격력 %d%% 감소" ), nPenalty );		
					AddItemInfoString( strTemp, 0xE53535FF );
				}
				
/*
				if( !rItemData.IsFlag(ITEM_FLAG_COMPOSITE) && !rItems.IsFlag(PLATINUM_MAX_PLUS) && rItems.Item_Used > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), rItems.Item_Used);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}*/
				
				if(rItemData.GetSubType() == CItemData::ITEM_WEAPON_MINING 
					|| rItemData.GetSubType() == CItemData::ITEM_WEAPON_CHARGE
					|| rItemData.GetSubType() == CItemData::ITEM_WEAPON_GATHERING)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), rItems.Item_Used);		
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
					BOOL bCanUseSuperGOJE = !rItems.IsFlag( FLAG_ITEM_SUPER_STONE_USED );
					CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
#ifdef RARE_ITEM
					if( bCanUseSuperGOJE )
						AddItemInfoString(strCanUseSuperGOJE, 0xFFFF40FF );
#else
					CTString strCanNotUseSuperGOJE(_S( 1659, "초 고급 제련석 사용 불가능" ));
					AddItemInfoString(bCanUseSuperGOJE ? strCanUseSuperGOJE : strCanNotUseSuperGOJE
									, bCanUseSuperGOJE ? 0xFFFF40FF : 0xFF4040FF);
#endif
				}
				
				// Class
				CUIManager::GetClassOfItem( rItemData, strTemp );
#ifdef RARE_ITEM
				AddItemInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
#else
				AddItemInfoString( strTemp, 0x9E9684FF );
#endif

				// Level
				int	nItemLevel = rItemData.GetLevel();

				int nWearLevelReduction = rItems.GetWearLevelReduction();

				if( nWearLevelReduction >0 || nItemleveldown > 0 )
					strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction+nItemleveldown );
				else
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
#ifdef RARE_ITEM
				if (nItemleveldown > 0) { AddItemInfoString(strTemp, 0x1C54CC); }
				else { AddItemInfoString( strTemp,	0xFFFFFFFF ); }
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

				if( rItems.Item_Plus > 0 )
				{
					// Physical defense
					if( rItemData.GetPhysicalDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalDefence() *
						//					pow( ITEM_PLUS_COFACTOR, rItems.Item_Plus ) ) - rItemData.GetPhysicalDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalDefence(), rItems.Item_Plus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), nBasePhysicalDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );

						AddItemInfoString( strTemp, colString );

						if( rItems.Item_Plus >= 15 )
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
						//					pow( ITEM_PLUS_COFACTOR, rItems.Item_Plus ) ) - rItemData.GetMagicDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicDefence(), rItems.Item_Plus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), nBaseMagicDefence, nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
															
						AddItemInfoString( strTemp, colString );

						if( rItems.Item_Plus >= 15 )
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

				// Penalty
				int	nPenalty = 0;
				int	nDiffLevel = nItemLevel - _pNetwork->MyCharacterInfo.level;
				if( nDiffLevel > 15 )
					nPenalty = 90;
				else if( nDiffLevel > 10 )
					nPenalty = 70;
				else if( nDiffLevel > 5 )
					nPenalty = 50;
				if( nPenalty > 0 && !m_bRareItem)
				{
					strTemp.PrintF( _S( 1030, "패널티 : 방어력 %d%% 감소" ), nPenalty );		
					AddItemInfoString( strTemp, 0xE53535FF );
				}
/*
				if( !rItemData.IsFlag(ITEM_FLAG_COMPOSITE) && !rItems.IsFlag(PLATINUM_MAX_PLUS)&& rItems.Item_Used > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), rItems.Item_Used);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}*/
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
				if( rItemData.GetSubType() == CItemData::ACCESSORY_PET ) // 펫인 경우 
				{
					// 서버로 부터 읽어 드린 펫 교환 정보를 찾아서 출력
					SPetExchangeInfoString pPetExchangeInfo;
					// 서버로 부터 읽드린 펫 정보가 있는지 확인 있다면 참조로 데이터 전달.
					if( _pUIMgr->GetPetInfo()->GetPetExchangeInfo( rItems.Item_Plus, pPetExchangeInfo ) )
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

					if(_pUIMgr->GetWildPetInfo()->GetWildPetInfo(rItems.Item_Plus,pWildPetInfo))
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
					if(rItems.Item_Used > 0)
					{
						strTemp.PrintF(  _S( 510, "내구도 : %ld" ), rItems.Item_Used);		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
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
				
				if ( rItemData.GetSubType() == CItemData::ITEM_SUB_WARP || rItemData.GetSubType() == CItemData::ITEM_SUB_CASH_ITEM)
				{
					if(rItems.Item_Used > 0)
					{
						time_t tv_used = rItems.Item_Used - _pUIMgr->GetRadar()->GetStartTime();
						g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
				//		rItems.Item_Used*=24;
				//		tv_t = localtime((time_t*)&rItems.Item_Used);
						
						if(g_iCountry == BRAZIL) // 070531 ttos : 브라질 만료날짜 표기 수정 (일/월/년 시)
						{
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_yday+1,g_tv_t->tm_mon+1,g_tv_t->tm_year+1,g_tv_t->tm_hour);
						}else
						{
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
						}
				
						AddItemInfoString( strTemp, 0xDEC05BFF );
						IN_VALIDTM(g_tv_t)
					}
				}

			}
			break;

		case CItemData::ITEM_POTION:	// Date : 2005-01-07,   By Lee Ki-hwan
			{

				// Date : 2005-01-14,   By Lee Ki-hwan
				
				if ( rItemData.GetSubType() == CItemData::POTION_UP )
				{
					if( rItems.Item_Flag > 0 )
					{
						// Level
						strTemp.PrintF( _S( 160, "레벨: %d" ), rItems.Item_Flag );
						AddItemInfoString( strTemp, 0xD28060FF );

						// 향상 타입
						int nSkillType = rItemData.GetSkillType();
						CSkill	&rSkill = _pNetwork->GetSkillData( nSkillType );
						int Power = rSkill.GetPower( rItems.Item_Flag - 1);


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
						// Level
						if( rItems.Item_Flag > 0 )
						{
							strTemp.PrintF( _S( 160, "레벨: %d" ), rItems.Item_Flag );
							AddItemInfoString( strTemp, 0xD28060FF );
						}
					}
					break;
					// 블러드 아이템 & 정화석.
				case CItemData::ITEM_ETC_OPTION:
					{

					}
					break;
				case CItemData::ITEM_ETC_EVENT:
					{// Date : 2006-06-02(오후 2:27:41), By eons
						if( rItems.Item_Index == 1483 )
						{// 월드컵 카드
							ULONG ulTemp = rItems.Item_Plus;

							int nScoreA = static_cast<int>( ( ulTemp & 0xFFFF0000 ) >> 16 );
							int nScoreB = static_cast<int>( ulTemp & 0x0000FFFF );
							
							strTemp.PrintF( "%s (%d) VS (%d) %s", _pUIMgr->GetQuest()->GetStrTeamA(), nScoreA,
								nScoreB, _pUIMgr->GetQuest()->GetStrTeamB() );

							AddItemInfoString( strTemp, 0xFFAA44FF );
						}
					}
					break;
				case CItemData::ITEM_PET_AI:
					{
						if(rItems.Item_Used > 0)
						{
							time_t tv_used = rItems.Item_Used - _pUIMgr->GetRadar()->GetStartTime();
							g_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
							
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),g_tv_t->tm_year+1,g_tv_t->tm_mon+1,g_tv_t->tm_yday+1,g_tv_t->tm_hour);
							AddItemInfoString( strTemp, 0xDEC05BFF );
							IN_VALIDTM(g_tv_t)
						}

					}break;
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
			if( rItems.Item_RareIndex ==0 )
				AddItemInfoString( _S(3165, "<미공개 옵션>"), 0xFF4040FF );
		}

		// Options
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:
		case CItemData::ITEM_SHIELD:
		case CItemData::ITEM_ACCESSORY:
			{
				SBYTE	sbOptionType, sbOptionLevel;
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
				{
					sbOptionType = rItems.GetOptionType( sbOption );
					sbOptionLevel = rItems.GetOptionLevel( sbOption );

					if( sbOptionType == -1 || sbOptionLevel == 0 )
						break;

					COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType );
					switch(sbOptionType)
					{
					case 49:		//착용제한레벨 다운
						strTemp.PrintF( "%s : -%d", odItem.GetName(), odItem.GetValue( sbOptionLevel - 1 ) );
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
						strTemp.PrintF( "%s : %d%%", odItem.GetName(), odItem.GetValue( sbOptionLevel - 1 ) );
						break;
					case 53:		//암흑 공격
					case 54:		//독 공격
					default:
						strTemp.PrintF( "%s : %d", odItem.GetName(), odItem.GetValue( sbOptionLevel - 1 ) );
					}

					AddItemInfoString( strTemp, 0x94B7C6FF );
				}
				if( rItems.IsFlag( FLAG_ITEM_OPTION_ENABLE ) )
				{
					AddItemInfoString( _S( 511, "블러드 옵션 가능" ), 0xE53535FF );		
				}
				if( rItems.IsFlag( FLAG_ITEM_SEALED ) )
				{
					AddItemInfoString(  _S( 512, "봉인된 아이템" ), 0xE53535FF );		
				}
			}
			break;
		}

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( rItemData.GetItemIndex() );
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
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strItemInfo[0].Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 22;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nWear, int nRow, int nCol )
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
	int		Iv_Tab, Iv_Row;

	Iv_Tab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

	if (Iv_Tab > 0)
	{ Iv_Row = nRow; }
	else
	{ Iv_Row = nRow + (INVEN_SLOT_ROW*m_InvenCurrentTab); }

	// Wearing item
	if( nWear >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nUniIndex = m_abtnWearing[nWear].GetItemUniIndex();

		// Update item information
		if( nOldUniIndex != nUniIndex || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldUniIndex = nUniIndex;
			m_abtnWearing[nWear].GetAbsPos( nInfoPosX, nInfoPosY );

			int	nTab = m_abtnWearing[nWear].GetItemTab();
			nRow = m_abtnWearing[nWear].GetItemRow();
			nCol = m_abtnWearing[nWear].GetItemCol();

			// Get item information
			m_bDetailItemInfo = m_nSelWearItemID == nWear;
			GetItemInfo( nTab, nRow, nCol, nInfoWidth, nInfoHeight );
		}
	}
	// Slot items
	else if( nRow >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nUniIndex = m_abtnItems[Iv_Tab][Iv_Row][nCol].GetItemUniIndex();

		// Update item information
		if( nOldUniIndex != nUniIndex || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldUniIndex = nUniIndex;
			m_abtnItems[Iv_Tab][Iv_Row][nCol].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelSlotItemID[m_InvenCurrentTab] == ( nCol + nRow * INVEN_SLOT_COL );
			GetItemInfo( Iv_Tab, Iv_Row, nCol, nInfoWidth, nInfoHeight );
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
void CUIInventory::RenderItems()
{
	// Wearing items
	for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
	{
		if( m_abtnWearing[iWear].IsEmpty() )
			continue;

		m_abtnWearing[iWear].Render();
	}

	int iRow, iCol;
	int tv_posX,tv_posY;
	int nTab, nRow;

	nTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

	for (iRow=0; iRow < INVEN_SLOT_ROW; ++iRow)
	for (iCol=0; iCol < INVEN_SLOT_COL; ++iCol)
	{
		if (nTab > 0)
		{ nRow = iRow; }
		else
		{ nRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

		if (m_abtnItems[nTab][nRow][iCol].IsEmpty())
			continue;

		m_abtnItems[nTab][nRow][iCol].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Set inventory texture
	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\NewInventory.tex")));

	// Mark Lease Wearing items
	for( iWear = 0; iWear < WEAR_TOTAL; iWear++ )
	{
		if( m_abtnWearing[iWear].IsEmpty() )
			continue;
		// 대여 아이템 표시
		int tv_posX,tv_posY;
		//m_abtnWearing[iWear].GetAbsPos(tv_posX,tv_posY);
		m_abtnWearing[iWear].GetPos(tv_posX,tv_posY);
				
		if(m_abtnWearing[iWear].GetItemFlag()&FLAG_ITEM_LENT)
		{
			m_NewInvenSurface.m_RectSurfaceArray[EInvenLeaseMark].m_RT = UIRect(tv_posX+17, tv_posY+17,tv_posX+29,tv_posY+29);
			m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenLeaseMark);
		}
	}

	// Outline of wearing items in item slot
	for (iRow=0; iRow < INVEN_SLOT_ROW; ++iRow)
	for (iCol=0; iCol < INVEN_SLOT_COL; ++iCol)
	{
		if (nTab > 0)
		{ nRow = iRow; }
		else
		{ nRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

		CItems rItems = _pNetwork->MySlotItem[nTab][nRow][iCol];
		CItemData	&rItemData = rItems.ItemData;

		m_abtnItems[nTab][nRow][iCol].GetPos(tv_posX,tv_posY);
		// 봉인 아이템 표시
		if(rItems.IsFlag(FLAG_ITEM_COMPOSITION) && !rItemData.IsFlag(ITEM_FLAG_COMPOSITE)){
			m_NewInvenSurface.m_RectSurfaceArray[EInvenSealed].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
			m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenSealed);
		}
		// 대여 아이템 표시
		if(rItems.IsFlag(FLAG_ITEM_LENT))
		{
			m_NewInvenSurface.m_RectSurfaceArray[EInvenLeaseMark].m_RT = UIRect(tv_posX+17, tv_posY+17,tv_posX+29,tv_posY+29);
			m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenLeaseMark);
		}

		// [090608: selo] 퀘스트 제한 아이템의 경우 사용 가능한지 체크하여 비활성 랜더 처리
		if(IsRestritionItem(nTab, nRow, iCol))
		{
			m_NewInvenSurface.m_RectSurfaceArray[EInvenSealed].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
			m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenSealed);
		}

		if( m_abtnItems[nTab][nRow][iCol].IsEmpty() ||
			m_abtnItems[nTab][nRow][iCol].GetItemWearType() < 0 )
			continue;
		// 아이템 장비 착용 아웃 라인
		m_NewInvenSurface.m_RectSurfaceArray[EInvenWearSelect].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
		m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenWearSelect);
	}

	//  edit by cpp2angel (04.12.21) : 조합중인 아이템 표시
	
	if ( _pUIMgr->GetMix()->IsVisible() )
	{
		for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
		{
			CUIButtonEx* ButtonEx = _pUIMgr->GetMix()->GetItemSlot ( i );
			
			if ( !ButtonEx->IsEmpty() ) // 비어 있지 않다면....
			{
				m_abtnItems[ButtonEx->GetItemTab()][ButtonEx->GetItemRow()][ButtonEx->GetItemCol()].GetPos( tv_posX, tv_posY );

				m_NewInvenSurface.m_RectSurfaceArray[EInvenMixing].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
				m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenMixing);
			}

		}
	}
	
	if ( _pUIMgr->GetCompound()->IsVisible() )
	{
		for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
		{
			CUIButtonEx* ButtonEx = _pUIMgr->GetCompound()->GetItemSlot ( i );
			
			if ( !ButtonEx->IsEmpty() ) // 비어 있지 않다면....
			{
				m_abtnItems[ButtonEx->GetItemTab()][ButtonEx->GetItemRow()][ButtonEx->GetItemCol()].GetPos( tv_posX, tv_posY );

				m_NewInvenSurface.m_RectSurfaceArray[EInvenMixing].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
				m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenMixing);
			}

		}
	}

	// Outline of selected item
	if( m_nSelWearItemID >= 0 )
	{
		m_abtnWearing[m_nSelWearItemID].GetPos( tv_posX, tv_posY );

		m_NewInvenSurface.m_RectSurfaceArray[EInvenNormalSelect].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
		m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenNormalSelect);
	}

	if (m_nSelSlotItemID[m_InvenCurrentTab] >= 0)
	{
		int	nSelRow = m_nSelSlotItemID[m_InvenCurrentTab] / INVEN_SLOT_COL;
		int	nSelCol = m_nSelSlotItemID[m_InvenCurrentTab] % INVEN_SLOT_COL;

		m_abtnItems[nTab][nSelRow][nSelCol].GetPos( tv_posX, tv_posY );

		m_NewInvenSurface.m_RectSurfaceArray[EInvenNormalSelect].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
		m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenNormalSelect);
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{
		_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\CommonBtn.tex")));
		int tmpX1, tmpY1, tmpX2, tmpY2;
		int i;
		tmpX1 = m_rcItemInfo.Left-m_nPosX;
		tmpY1 = m_rcItemInfo.Top-m_nPosY;
		tmpX2 = m_rcItemInfo.Right-m_nPosX;
		tmpY2 = m_rcItemInfo.Bottom-m_nPosY;

		// Item information region
		if( m_bDetailItemInfo )
		{
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoUL].m_RT = UIRect(tmpX1,tmpY1,tmpX1+20,tmpY1+20);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoUM].m_RT = UIRect(tmpX1+20,tmpY1,tmpX2-20,tmpY1+20);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoUR].m_RT = UIRect(tmpX2-20,tmpY1,tmpX2,tmpY1+20);

			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoML].m_RT = UIRect(tmpX1,tmpY1+20,tmpX1+20,tmpY2-20);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoMM].m_RT = UIRect(tmpX1+20,tmpY1+20,tmpX2-20,tmpY2-20);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoMR].m_RT = UIRect(tmpX2-20,tmpY1+20,tmpX2,tmpY2-20);

			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoBL].m_RT = UIRect(tmpX1,tmpY2-20,tmpX1+20,tmpY2);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoBM].m_RT = UIRect(tmpX1+20,tmpY2-20,tmpX2-20,tmpY2);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenInfoBR].m_RT = UIRect(tmpX2-20,tmpY2-20,tmpX2,tmpY2);

			for (i=EInvenInfoUL; i<EInvenSimpleInfoL; ++i)
			{
				m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(),0xFFFFFFFF,i);
			}
		}
		else
		{
			m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoL].m_RT = UIRect(tmpX1,tmpY1,tmpX1+12,tmpY2);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoM].m_RT = UIRect(tmpX1+12,tmpY1,tmpX2-12,tmpY2);
			m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoR].m_RT = UIRect(tmpX2-12,tmpY1,tmpX2,tmpY2);

			for (i=EInvenSimpleInfoL; i<EInvenTotal; ++i)
			{
				m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(),0xFFFFFFFF,i);
			}
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
				if(iInfo==0 && m_bRareItem && m_iRareGrade>=0 && m_iRareGrade<5)
					_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, RareItem_Name_Color[m_iRareGrade] );
				else
					_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
				nInfoY += _pUIFontTexMgr->GetLineHeight();
			}
		}
		else
		{
			if( m_bRareItem && m_iRareGrade>=0 && m_iRareGrade<5)
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[0], m_rcItemInfo.Left + 8, m_rcItemInfo.Top + 3, RareItem_Name_Color[m_iRareGrade]);
			else
				_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[0], m_rcItemInfo.Left + 8, m_rcItemInfo.Top + 3, m_colItemInfo[0] );
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::UpdateWeight()
{
	FLOAT	fWeightRatio = (FLOAT)_pNetwork->MyCharacterInfo.weight / (FLOAT)_pNetwork->MyCharacterInfo.maxweight;
	FLOAT	fBarRatio;

	m_strWeight.PrintF( "%.2f%%", fWeightRatio * 100.0f );

	if( fWeightRatio > 1.0f )
	{
		fBarRatio = 1.0f;
		fWeightRatio = 1.0f - ( fWeightRatio - 1.0f ) * 2.0f;
		UBYTE	ubGreen = 0xE5 * fWeightRatio;
		m_colWeight = 0xE50000FF | ( ubGreen << 16 );
	}
	else
	{
		fBarRatio = fWeightRatio;
		fWeightRatio = 1.0f - fWeightRatio;
		UBYTE	ubBlue = 0xE5 * fWeightRatio;
		m_colWeight = 0xE5E500FF | ( ubBlue << 8 );
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::Render()
{
	// Set inventory texture
	int row;//, col;
	
	m_NewInvenSurface.SetPos(m_nPosX, m_nPosY);
	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\NewInventory.tex")));

	m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, EInvenBg);
	m_btnTrashcan.Render();
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\CommonBtn.tex")));

	if (m_btnTab[m_InvenCurrentTab].GetBtnState() != UBS_ON)
		m_btnTab[m_InvenCurrentTab].SetBtnState(UBS_ON);

	for (row=0; row<INVEN_TAB_TOTAL; ++row)
	{
		if (row == INVEN_TAB_NORMAL4)
			continue;

		m_btnTab[row].Render();
	}

	m_btnClose.Render();
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Text in inventory
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 166, "인벤토리" ), m_nPosX + 129, m_nPosY + 17, 0xFFFFFFFF );

	// Money
	COLOR colNas = _pUIMgr->GetNasColor( _pNetwork->MyCharacterInfo.money );
	_pUIMgr->GetDrawPort()->PutTextExRX( m_strMyMoney, m_nPosX + 229, m_nPosY + 196, colNas );

	// Weight
	UpdateWeight();
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strWeight, m_nPosX + 45, m_nPosY + 196, m_colWeight );

	m_strNumItems.PrintF("%d/100", GetMySlotItemsCount());
	_pUIMgr->GetDrawPort()->PutTextExCX( m_strNumItems, m_nPosX + 97, m_nPosY + 196, 0xFFFFFFFF );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Items
	RenderItems();

	// Tool tip
	if( m_bShowToolTip )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\CommonBtn.tex")));
		int tmpX1, tmpY1, tmpX2, tmpY2;
		int i;
		tmpX1 = m_rcToolTip.Left-m_nPosX;
		tmpY1 = m_rcToolTip.Top-m_nPosY;
		tmpX2 = m_rcToolTip.Right-m_nPosX;
		tmpY2 = m_rcToolTip.Bottom-m_nPosY;

		m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoL].m_RT = UIRect(tmpX1,tmpY1,tmpX1+12,tmpY2);
		m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoM].m_RT = UIRect(tmpX1+12,tmpY1,tmpX2-12,tmpY2);
		m_NewInvenSurface.m_RectSurfaceArray[EInvenSimpleInfoR].m_RT = UIRect(tmpX2-12,tmpY1,tmpX2,tmpY2);

		for (i=EInvenSimpleInfoL; i<EInvenTotal; ++i)
		{
			m_NewInvenSurface.RenderRectSurface(_pUIMgr->GetDrawPort(),0xFFFFFFFF,i);
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Text in tool tip
		_pUIMgr->GetDrawPort()->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		int	nInfoX, nInfoY, nX, nY;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		//case ITEM_TAB_NORMAL:
		case INVEN_TAB_NORMAL0:
		case INVEN_TAB_NORMAL1:
		case INVEN_TAB_NORMAL2:
		case INVEN_TAB_NORMAL3:
		case INVEN_TAB_NORMAL4:
			m_strToolTip = _S( 167, "일반" );
			break;
		case INVEN_TAB_EVENT:
			m_strToolTip = _S( 169, "이벤트" );
			break;
		case INVEN_TAB_QUEST:
			m_strToolTip = _S( 168, "퀘스트" );
			break;
		}

		int	nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		m_btnTab[nToolTipID].GetPos(nX, nY);
		nInfoX = m_nPosX + nX + m_btnTab[nToolTipID].GetWidth() + 2;
		nInfoY = m_nPosY + nY + (m_btnTab[nToolTipID].GetHeight() - _pUIFontTexMgr->GetLineHeight()) / 2;

		if( nInfoX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoX = _pUIMgr->GetMaxI() - nInfoWidth;

		m_rcToolTip.Left = nInfoX;
		m_rcToolTip.Top = nInfoY;
		m_rcToolTip.Right = m_rcToolTip.Left + nInfoWidth;
		m_rcToolTip.Bottom = m_rcToolTip.Top + nInfoHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : GetLocationOfNormalItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::GetLocationOfNormalItem( int nUniIndex, int &nRow, int &nCol )
{
	int	iRow, iCol;
	for( iRow = 0; iRow < INVEN_SLOT_ROW_TOTAL; iRow++ )
	{
		for( iCol = 0; iCol < INVEN_SLOT_COL; iCol++ )
		{
			if( m_abtnItems[ITEM_TAB_NORMAL][iRow][iCol].GetItemUniIndex() == nUniIndex )
			{
				nRow = iRow;
				nCol = iCol;
				return;
			}
		}
	}

	nRow = -1;
}

// ----------------------------------------------------------------------------
// Name : GetLocationOfNormalItem()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIInventory::GetItemCount( int iItemIndex )
{
	for(int tab=0; tab<INVEN_SLOT_TAB; ++tab)
	{
		for(int row=0; row<INVEN_SLOT_ROW_TOTAL; ++row)
		{
			for(int col=0; col<INVEN_SLOT_COL; ++col)
			{
				if( m_abtnItems[tab][row][col].GetItemIndex() == iItemIndex )
				{
					return _pNetwork->MySlotItem[tab][row][col].Item_Sum;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Name : GetItemName
// Desc :
// ----------------------------------------------------------------------------
CTString CUIInventory::GetItemName( int iItemIndex )
{
	CTString tv_str;
	for(int tab=0; tab<INVEN_SLOT_TAB; ++tab)
	{
		for(int row=0; row<INVEN_SLOT_ROW_TOTAL; ++row)
		{
			for(int col=0; col<INVEN_SLOT_COL; ++col)
			{
				if( m_abtnItems[tab][row][col].GetItemIndex() == iItemIndex )
				{
					CTString tv_str;
					tv_str.PrintF("%s",_pNetwork->MySlotItem[tab][row][col].ItemData.GetName());
					return tv_str;
				}
			}
		}
	}
	return tv_str;
}


// ----------------------------------------------------------------------------
// Name : GetItemAll()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIInventory::GetItemAll()
{
	SQUAD tv_num=0;
	for(int row=0; row<INVEN_SLOT_ROW_TOTAL; ++row)
	{
		for(int col=0; col<INVEN_SLOT_COL; ++col)
		{
			if( !m_abtnItems[0][row][col].IsEmpty() )
			{
				tv_num++;
			}
		}
	}

	return tv_num;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIInventory::MouseMessage( MSG *pMsg )
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

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			int nTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

			// Hold item button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				if( !m_bLockInven )
				{
					// Wearing items
					if( m_nSelWearItemID >= 0 )
					{
						// Close message box of inventory
						_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

						_pUIMgr->SetHoldBtn( m_abtnWearing[m_nSelWearItemID] );
						int	nOffset = BTN_SIZE / 2;
						_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

						m_abtnWearing[m_nSelWearItemID].SetBtnState( UBES_IDLE );
					}
					// Slot items
					else if( m_nSelSlotItemID[m_InvenCurrentTab] >= 0 )
					{
						// Close message box of inventory
						_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

						int	nSelRow = m_nSelSlotItemID[m_InvenCurrentTab] / INVEN_SLOT_COL;
						int	nSelCol = m_nSelSlotItemID[m_InvenCurrentTab] % INVEN_SLOT_COL;

						if (nTab == 0) {
							nSelRow += (m_InvenCurrentTab*INVEN_SLOT_ROW);
						}

						_pUIMgr->SetHoldBtn( m_abtnItems[nTab][nSelRow][nSelCol] );
						int	nOffset = BTN_SIZE / 2;
						_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

						m_abtnItems[nTab][nSelRow][nSelCol].SetBtnState( UBES_IDLE );
					}
				}

				bLButtonDownInItem = FALSE;
			}

			int i, nWhichTab = -1;
			for (i=0; i<INVEN_TAB_TOTAL; ++i)
			{
				if (i==INVEN_TAB_NORMAL4) // 현재 사용 안함
					continue;

				if (m_btnTab[i].MouseMessage(pMsg) != WMSG_FAIL)
				{
					nWhichTab = i;
				}
			}

			if (nWhichTab >= 0)
			{
				if( !_pUIMgr->GetHoldBtn().IsEmpty() &&
					_pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
					_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY &&
					_pUIMgr->GetHoldBtn().GetItemTab() == ITEM_TAB_NORMAL &&
					nWhichTab < INVEN_TAB_QUEST)
				{
					m_InvenCurrentTab = static_cast<InventoryTab>(nWhichTab);
				}

				ShowToolTip(TRUE, nWhichTab);
				ShowItemInfo(FALSE);
				return WMSG_SUCCESS;
			}

			// Move inventory
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if (m_btnTrashcan.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			// Wearing items
			else if( IsInsideRect( nX, nY, m_rcWearing ) )
			{
				int	nWhichWear = -1;
				for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
				{
					if( m_abtnWearing[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichWear = iWear;
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichWear );
				ShowToolTip( FALSE );

				return WMSG_SUCCESS;
			}
			// Slot items
			else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
			{
				int	iRow, iCol;
				int	nWhichRow = -1, nWhichCol = -1;
				int tmpRow;

				BOOL bFind = FALSE;

				for (iRow=0; iRow < INVEN_SLOT_ROW; ++iRow)
				{
					if (bFind) 
						break;

					for (iCol=0; iCol < INVEN_SLOT_COL; ++iCol)
					{
						if (nTab > 0)
						{ tmpRow = iRow; }
						else
						{ tmpRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

						if (m_abtnItems[nTab][tmpRow][iCol].MouseMessage(pMsg) != WMSG_FAIL)
						{
							nWhichRow = iRow;
							nWhichCol = iCol;
							bFind = TRUE;
							break;
						}
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichRow, nWhichCol );
				ShowToolTip( FALSE );

				return WMSG_SUCCESS;
			}

			// Hide tool tip
			ShowToolTip( FALSE );
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
				else if (m_btnTrashcan.MouseMessage(pMsg) != WMSG_FAIL)
				{

				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Wearing items
				else if( IsInsideRect( nX, nY, m_rcWearing ) )
				{
					m_nSelWearItemID = -1;
					m_nSelSlotItemID[m_InvenCurrentTab] = -1;

					for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelWearItemID = iWear;

							// Show tool tip
							ShowItemInfo( TRUE, TRUE, iWear );

							bLButtonDownInItem = TRUE;

							_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Slot items
				else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
				{
					//int	iRowS = m_aItemFirstRow[m_nCurItemTab];
					//int	iRowE = iRowS + INVEN_SLOT_ROW;
					int nTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;
					int	iRow, iCol, tmpRow;

					m_nSelWearItemID = -1;
					m_nSelSlotItemID[m_InvenCurrentTab] = -1;

					for( iRow=0; iRow < INVEN_SLOT_ROW; ++iRow )
					{
						for( iCol=0; iCol < INVEN_SLOT_COL; ++iCol )
						{
							if (nTab > 0)
							{ tmpRow = iRow; }
							else
							{ tmpRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

							// Update selected item
							if ( m_abtnItems[nTab][tmpRow][iCol].IsInside(nX, nY))
							{
								m_nSelSlotItemID[m_InvenCurrentTab] = iCol + iRow * INVEN_SLOT_COL;
							}

							if( m_abtnItems[nTab][tmpRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, iRow, iCol );

								bLButtonDownInItem = TRUE;
								
								if( _pUIMgr->GetCollectBox()->IsVisible() )
								{// 곤충 채집 상자는 무조건 닫는다.
									_pUIMgr->RearrangeOrder( UI_COLLECTBOX, FALSE );
								}

								_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Item type tab
				else if( IsInsideRect( nX, nY, m_rcItemTypeTab[0] ) || IsInsideRect( nX, nY, m_rcItemTypeTab[1] ) )
				{
					int i;

					for (i=0; i<INVEN_TAB_TOTAL; ++i)
					{
						if (i==INVEN_TAB_NORMAL4)
							continue;

						if (m_btnTab[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_InvenCurrentTab = static_cast<InventoryTab>(i);
							_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}

				_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
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

				// If inventory isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;				

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						_pUIMgr->RearrangeOrder( UI_INVENTORY, FALSE );	

					return WMSG_SUCCESS;
				}
				else if ((wmsgResult = m_btnTrashcan.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND && m_InvenCurrentTab < INVEN_TAB_QUEST && m_InvenCurrentTab >= 0)
					{
						if (m_nSelSlotItemID[m_InvenCurrentTab] >= 0)
						{
							int nRow = m_nSelSlotItemID[m_InvenCurrentTab] / INVEN_SLOT_COL;
							int nCol = m_nSelSlotItemID[m_InvenCurrentTab] % INVEN_SLOT_COL;
							nRow += (m_InvenCurrentTab*INVEN_SLOT_ROW);

							ShowItemDestroyMessage(m_abtnItems[0][nRow][nCol]);
						}
					}

					return WMSG_SUCCESS;
				}
				// Item type tab
				else if( IsInsideRect( nX, nY, m_rcItemTypeTab[0] ) || IsInsideRect( nX, nY, m_rcItemTypeTab[1] ) )
				{
					int i;

					for (i=0; i<INVEN_TAB_TOTAL; ++i)
					{
						if (i==INVEN_TAB_NORMAL4)
							continue;

						if (m_btnTab[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Wearing item
				else if( IsInsideRect( nX, nY, m_rcWearing ) )
				{
					for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Slot items
				else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
				{
					int	iRow, iCol, tmpRow;
					int nTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

					for( iRow = 0; iRow < INVEN_SLOT_ROW; ++iRow )
					{
						for( iCol = 0; iCol < INVEN_SLOT_COL; ++iCol )
						{
							if (nTab > 0)
							{ tmpRow = iRow; }
							else
							{ tmpRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

							if( m_abtnItems[nTab][tmpRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
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
					// If holding button is item and comes from inventory
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
					{
						// Wearing items
						if( IsInsideRect( nX, nY, m_rcWearing ) )
						{
							// If this item is not equal to wearing item
							if( m_nSelWearItemID < 0 ||
								m_abtnWearing[m_nSelWearItemID].GetItemUniIndex() != _pUIMgr->GetHoldBtn().GetItemUniIndex() )
							{
								int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
								int nRow = _pUIMgr->GetHoldBtn().GetItemRow();
								int nCol = _pUIMgr->GetHoldBtn().GetItemCol();
								int	nIndex = _pUIMgr->GetHoldBtn().GetItemIndex();
								int	nUniIndex = _pUIMgr->GetHoldBtn().GetItemUniIndex();
								CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
								CItemData	&rItemData = rItems.ItemData;

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								// If holding item is refining stone
								if( rItemData.GetType() == CItemData::ITEM_ETC &&
									rItemData.GetSubType() == CItemData::ITEM_ETC_REFINE )
								{
									for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
									{
										if( !m_abtnWearing[iWear].IsEmpty() &&
											m_abtnWearing[iWear].IsInside( nX, nY ) )
										{
											break;
										}
									}

									// If holding item is not inside weapon or shield
									if( iWear == WEAR_TOTAL )
										return WMSG_SUCCESS;

									// Upgrade item
									SLONG	slLevel = rItems.Item_Flag;
									UpgradeItem( iWear, nIndex, nUniIndex, nRow, nCol, slLevel );
								}
								// 블러드 아이템 & 정화석.
								else if( (rItemData.GetType() == CItemData::ITEM_ETC &&
									rItemData.GetSubType() == CItemData::ITEM_ETC_OPTION )
										|| nIndex==2361 )	//피와 땀의 결정
								{
									for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
									{
										if( !m_abtnWearing[iWear].IsEmpty() &&
											m_abtnWearing[iWear].IsInside( nX, nY ) )
										{
											break;
										}
									}

									// If holding item is not inside weapon or shield
									if( iWear == WEAR_TOTAL )
										return WMSG_SUCCESS;

									// Upgrade item
									SLONG	slLevel = rItems.Item_Flag;

									if(rItemData.GetOptionType() == CItemData::ITEM_OPTION_BLOOD
										|| nIndex==2361 )	//피와 땀의 결정
									{
										OptionAddItem( iWear, nIndex, nRow, nCol );
									}
									else if(rItemData.GetOptionType() == CItemData::ITEM_OPTION_CLEAR)
									{
										OptionDelItem( iWear, nIndex, nRow, nCol );
									}
								}
								else
								{
									// From item slot to wearing part ( only wearing items !!! )
									if( rItemData.GetWearingPosition() >= 0 )
									{
										// Check wearing
										for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
										{
											if( m_abtnWearing[iWear].GetItemUniIndex() == nUniIndex )
												break;
										}

										// Not wearing now
										if( iWear == WEAR_TOTAL )
										{
											SendUseSlotItem( nTab, nRow, nCol );
										}

										return WMSG_SUCCESS;
									}
								}
							}
						}
						// Slot items
						else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
						{
							// Unwear
							if( m_nSelWearItemID >= 0 &&
								m_abtnWearing[m_nSelWearItemID].GetItemUniIndex() == _pUIMgr->GetHoldBtn().GetItemUniIndex() )
							{
								SendUseSlotItem( _pUIMgr->GetHoldBtn().GetItemTab(),
													_pUIMgr->GetHoldBtn().GetItemRow(),
													_pUIMgr->GetHoldBtn().GetItemCol() );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
							// Swap
							else
							{
								int	iRow, iCol, tmpRow;
								int nTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

								for( iRow = 0; iRow < INVEN_SLOT_ROW; ++iRow )
								{
									for( iCol = 0; iCol < INVEN_SLOT_COL; ++iCol )
									{
										if (nTab > 0)
										{ tmpRow = iRow; }
										else
										{ tmpRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

										if( m_abtnItems[nTab][tmpRow][iCol].IsInside( nX, nY ) )
										{
											if( _pUIMgr->GetHoldBtn().GetItemUniIndex()
													!= m_abtnItems[nTab][tmpRow][iCol].GetItemUniIndex() )
											{
												SendSwapSlotItems( nTab,
																	_pUIMgr->GetHoldBtn().GetItemRow(),
																	_pUIMgr->GetHoldBtn().GetItemCol(),
																	tmpRow, iCol );
											}

											// Reset holding button
											_pUIMgr->ResetHoldBtn();

											return WMSG_SUCCESS;
										}
									}
								}
							} // If - Swap
						} // If - Slot items
						else if (IsInsideRect(nX, nY, m_rcItemTypeTab[0]))
						{
							int i;
							int tmpRow, tmpCol;

							if (_pUIMgr->GetHoldBtn().GetItemTab() == ITEM_TAB_NORMAL)
							{
								for (i=0; i<INVEN_TAB_NORMAL4; ++i)
								{
									if (m_btnTab[i].IsInside(nX, nY))
									{
										if (FindEmptyabtnItem(i, tmpRow, tmpCol))
										{
											SendSwapSlotItems( 0,
														_pUIMgr->GetHoldBtn().GetItemRow(),
														_pUIMgr->GetHoldBtn().GetItemCol(),
														tmpRow, tmpCol );
										}
										_pUIMgr->ResetHoldBtn();
										break;
									}
								}
							}
						}
						else if (m_btnTrashcan.IsInside(nX, nY))
						{
							ShowItemDestroyMessage(_pUIMgr->GetHoldBtn());
							_pUIMgr->ResetHoldBtn();
						}
					} // If - If holding button is item
					else if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_WAREHOUSE )
					{
						// Slot items
						if( IsInsideRect( nX, nY, m_rcItemSlot ) )
						{
							_pUIMgr->GetWareHouse()->DelWareHouseItemToInventory();							
						} // If - Slot items
					}
					else if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_WILDPET_INFO)
					{
						// Slot items
						if( IsInsideRect( nX, nY, m_rcItemSlot ) )
						{
							_pNetwork->SendUseWildPetItem(_pUIMgr->GetHoldBtn().GetItemWearType(),
								-1,-1,-1,_pUIMgr->GetHoldBtn().GetItemIndex());
						} // If - Slot items

					}
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				} // If - IsInside
			} // If - If holding button doesn't exist
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Wearing item(Wearing부분 더블 클릭시...)
				if( !m_bLockInven && IsInsideRect( nX, nY, m_rcWearing ) )
				{
					m_nSelWearItemID = -1;

					for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) == WMSG_COMMAND )
						{
							// Close message box of inventory
							_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

							SendUseSlotItem( m_abtnWearing[iWear].GetItemTab(),
												m_abtnWearing[iWear].GetItemRow(),
												m_abtnWearing[iWear].GetItemCol() );

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iWear );

							return WMSG_SUCCESS;
						}
					}
				}
				// Slot items(아이템 부분 더블 클릭시...)
				else if( !m_bLockInven && IsInsideRect( nX, nY, m_rcItemSlot ) )
				{
					m_nSelSlotItemID[m_InvenCurrentTab] = -1;

					int	iRow, iCol, tmpTab, tmpRow;
					tmpTab = ((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

					for( iRow = 0; iRow < INVEN_SLOT_ROW; ++iRow )
					{
						for( iCol = 0; iCol < INVEN_SLOT_COL; ++iCol )
						{
							if (tmpTab > 0)
							{ tmpRow = iRow; }
							else
							{ tmpRow = iRow + (m_InvenCurrentTab*INVEN_SLOT_ROW); }

							if( m_abtnItems[tmpTab][tmpRow][iCol].MouseMessage( pMsg ) == WMSG_COMMAND )
							{
								// Close message box of inventory
								_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );
								
								// [090609: selo] : 아이템의 Flag가 퀘스트Flag를 사용 한다면
								//					위치를 계산해 제한 사용 범위 안에 있지 않을 때에는 
								//					서버로 메시지를 보내지 않는다.								
								if(IsRestritionItem(tmpTab, tmpRow, iCol))
								{
									// [090707: selo] : 서버로 메시지를 보내지 않을 경우 아이템 사용 실패 메시지 박스를 띄움
									CTString strTitle;
									CTString strMessage1;
									CUIMsgBox_Info	MsgBoxInfo;
									
									strTitle	=	_S(191,"확인");
									strMessage1	=	_S( 4428, "여기서는 사용 할 수 없습니다" ); 
									MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
									MsgBoxInfo.AddString(strMessage1);
									_pUIMgr->CreateMessageBox(MsgBoxInfo);

									return WMSG_SUCCESS;
								}

								// 이기환 수정 (12.9) 조합창이 활성화 되어 있을 때는 조합창으로 아이템을 보냄 
								if( _pUIMgr->GetMix()->IsEnabled() && _pUIMgr->GetMix()->IsVisible() )
								{	
									_pUIMgr->SetHoldBtn( m_abtnItems[tmpTab][tmpRow][iCol] );
									int	nOffset = BTN_SIZE / 2;
									_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

									_pUIMgr->GetMix()->SetMixItem ();
								}
								else if( _pUIMgr->GetCompound()->IsEnabled() && _pUIMgr->GetCompound()->IsVisible() )
								{
									_pUIMgr->SetHoldBtn( m_abtnItems[tmpTab][tmpRow][iCol] );
									int	nOffset = BTN_SIZE / 2;
									_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

									_pUIMgr->GetCompound()->SetCompoundItem ();
								}
								else if( _pUIMgr->GetGWMix()->IsEnabled() && _pUIMgr->GetGWMix()->IsVisible() )
								{
									_pUIMgr->SetHoldBtn( m_abtnItems[tmpTab][tmpRow][iCol] );
									int	nOffset = BTN_SIZE / 2;
									_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

									_pUIMgr->GetGWMix()->SetMixItem();
								}
								else
								{
									SetUseItemSlotInfo(tmpTab, tmpRow, iCol);
									// 아이템 부분의 슬롯 더블 클릭시...
									SendUseSlotItem( tmpTab, tmpRow, iCol );

									// Show tool tup
									ShowItemInfo( TRUE, TRUE, -1, iRow, iCol );
								}

								return WMSG_SUCCESS;
							}
						}
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;	
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ToggleVisible()
{
	if( _pUIMgr->GetBilling()->IsLock() ) return;
	BOOL	bVisible = !IsVisible();
	_pUIMgr->RearrangeOrder( UI_INVENTORY, bVisible );
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static SBYTE	sbTempRow1, sbTempCol1, sbTempRow2, sbTempCol2, sbTempWearPos;
static SLONG	slTempPlus, slTempLevel;

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::UpgradeItem( SBYTE sbWearPos, SLONG slIndex, SLONG nUniIndex,
								SBYTE sbRefineRow, SBYTE sbRefineCol, SLONG slRefineLevel )
{
	// If inventory is already locked
	if( IsLocked() )
	{
		ShowLockErrorMessage();
		return;
	}

	// FIXME : 스킬사용중에 블러드 젬등을 사용하면, 응답이 안와서 인벤토리 락이 안풀어짐...
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 863, "스킬 사용중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 925, "순간 이동중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SBYTE	sbWearTab = m_abtnWearing[sbWearPos].GetItemTab();
	SBYTE	sbWearRow = m_abtnWearing[sbWearPos].GetItemRow();
	SBYTE	sbWearCol = m_abtnWearing[sbWearPos].GetItemCol();

	CItems		&rItems = _pNetwork->MySlotItem[sbWearTab][sbWearRow][sbWearCol];
	CItemData	&rItemData = rItems.ItemData;

	BOOL bBoost = ((slIndex == 786)||(slIndex == 1283)||(slIndex == 1284)||(slIndex == PLATINUM_BOOSTER_ITEM) ) ;//부스터,실버부스터,골드부스터 wooss 060221
	// If this item can not upgrade
	if( !bBoost 
		&& (!( rItemData.GetFlag() & ITEM_FLAG_UPGRADE ) 
		&& !rItems.IsFlag(FLAG_ITEM_COMPOSITION) 
		|| rItems.IsFlag(FLAG_ITEM_LENT)) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 170, "업그레이드 가능 아이템이 아닙니다." ), SYSMSG_ERROR );
		return;
	}

	SLONG	slWearLevel = rItemData.GetLevel();
	// Check level
	if( slRefineLevel > 0 )		// Normal refining stone
	{
		if( slRefineLevel < slWearLevel )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S( 171, "아이템 레벨이 맞지 않습니다." ), SYSMSG_ERROR );
			return;
		}
	}

	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_UPGRADE_REP );

	SLONG	slWearPlus = rItems.Item_Plus;
	sbTempRow1 = sbWearRow;
	sbTempCol1 = sbWearCol;
	slTempPlus = slWearPlus;
	sbTempRow2 = sbRefineRow;
	sbTempCol2 = sbRefineCol;
	slTempLevel = slRefineLevel;
	sbTempWearPos = sbWearPos;

	const char* szItemName = _pNetwork->GetItemName( rItemData.GetItemIndex() );

	CUIMsgBox_Info	MsgBoxInfo;
	CTString	strMessage, strTitle;
	BOOL bLock = TRUE;
	const BOOL bSuperGOJE = slIndex == 779;//초고제일때

	// ---- wooss 060518 --------------------------------------->>
	const BOOL bPlatinumStone = slIndex == 1419; // 

	// ---------------------------------------------------------<<
	const BOOL bItemLevelDown = slIndex == 2051; // 아이템 레벨 제한 감소 주문서

	const BOOL bChaosStone = slIndex == 2844;	// 카오스 제련석

	if(bSuperGOJE)
	{
		//TEMP:초고급 제련석 관련
		CTString	strMessage, strTitle;
		DWORD style;
		int commandCode;
		if(slWearPlus >= 14)
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 1671, "초 고급 제련석은 +14이상의 장비에는\n사용할 수 없습니다." ));
			style = UMBS_OK;
			commandCode = MSGCMD_INVEN_UPGRADE_REP;
			bLock = FALSE;
		}
		else if( rItems.IsFlag( FLAG_ITEM_BOOSTER_ADDED ) == 0 )
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 1672, "초 고급 제련석는 하나의 장비에\n한번만 사용할 수 있습니다.\n정말 사용하시겠습니까?" ));
			style = UMBS_YESNO;
			commandCode = MSGCMD_INVEN_UPGRADE_REQ;
			bLock = TRUE;
		}
		else
		{
			strTitle.PrintF(_S( 1673, "제련 불가!!!" ));
			strMessage.PrintF(_S( 1674, "초 고급 제련석을 이미\n사용한 장비입니다.\n다른 아이템에 사용해 주십시요." ));
			style = UMBS_OK;
			commandCode = MSGCMD_INVEN_UPGRADE_REP;
			bLock = FALSE;
		}

		MsgBoxInfo.SetMsgBoxInfo( strTitle, style, UI_INVENTORY, commandCode );
		MsgBoxInfo.AddString( strMessage );
	}
	else if(bBoost)
	{
		CTString	strMessage, strTitle;
		DWORD style;
		int commandCode;
		if(!(rItems.ItemData.GetType() == CItemData::ITEM_WEAPON
		&& (rItems.ItemData.GetSubType() == CItemData::ITEM_WEAPON_MINING
			|| rItems.ItemData.GetSubType() == CItemData::ITEM_WEAPON_GATHERING
			|| rItems.ItemData.GetSubType() == CItemData::ITEM_WEAPON_CHARGE)
			))
		{
			strTitle.PrintF(_S( 1673, "제련 불가!!!" ));
			strMessage.PrintF(_S( 1783, "부스터는 생산도구에만 사용할 수 있습니다." ) );
			style = UMBS_OK;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
			bLock = FALSE;
		}
		else if( rItems.IsFlag( FLAG_ITEM_PLATINUMBOOSTER_ADDED ) == 0 && slIndex == PLATINUM_BOOSTER_ITEM)
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 2640, "유료 아이템 플래티늄 부스터를 부착하시겠습니까?" ) );
			style = UMBS_YESNO;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REQ;
			bLock = TRUE;
			m_iItemServerIndex = nUniIndex;
		}
		else if( rItems.IsFlag( FLAG_ITEM_BOOSTER_ADDED ) == 0 && slIndex == 786)
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 1784, "정말 부스터를 사용하시겠습니까?" ) );
			style = UMBS_YESNO;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REQ;
			bLock = TRUE;
			m_iItemServerIndex = nUniIndex;
		}
		else if( rItems.IsFlag( FLAG_ITEM_SILVERBOOSTER_ADDED ) == 0 && slIndex == 1283)
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 2431,  "유료 아이템 실버 부스터를 부착하시겠습니까?" ) );
			style = UMBS_YESNO;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REQ;
			bLock = TRUE;
			m_iItemServerIndex = nUniIndex;
		}
		else if( rItems.IsFlag( FLAG_ITEM_GOLDBOOSTER_ADDED ) == 0 && slIndex == 1284)
		{
			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S( 2432, "유료 아이템 골드 부스터를 부착하시겠습니까?" ) );
			style = UMBS_YESNO;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REQ;
			bLock = TRUE;
			m_iItemServerIndex = nUniIndex;
		}
		else
		{
			strTitle.PrintF(_S( 1673, "제련 불가!!!" ));
			strMessage.PrintF(_S( 1785, "이미 부스터를 사용한 장비입니다.\n다른 아이템에 사용하여 주십시요." ) );
			style = UMBS_OK;
			commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
			bLock = FALSE;
		}
		MsgBoxInfo.SetMsgBoxInfo( strTitle, style, UI_INVENTORY, commandCode );
		MsgBoxInfo.AddString( strMessage );
	}

	else if(bPlatinumStone){
		CTString	strMessage, strTitle;
		strTitle.PrintF(_S( 191, "확인" ));
		if(rItems.IsFlag(PLATINUM_MAX_PLUS)){
			_pUIMgr->CloseMessageBox(MSGCMD_NULL);
			strMessage.PrintF(_S( 2641, "이미 플래티늄 제련석을 사용중입니다. 더 이상 사용할 수 없습니다." ) );
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( strMessage );
			bLock = FALSE;
		
		}
		else if(slWearPlus > 16) {
			_pUIMgr->CloseMessageBox(MSGCMD_NULL);
			strMessage.PrintF(_S( 2642, "더 이상 업그레이드 할 수 없습니다." ) );
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( strMessage );
			bLock = FALSE;

		}
		else {				
			if(!_pUIMgr->DoesMessageBoxExist(MSGCMD_INVEN_UPGRADE_REQ)){
				strMessage.PrintF(_S( 2643, "플래티늄 제련석을 통한 업그레이드는 최대 +16의 효과만 납니다. 사용하시겠습니까?" ) );
				bLock = TRUE;
				MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_INVENTORY, MSGCMD_INVEN_UPGRADE_REQ );
				MsgBoxInfo.AddString( strMessage );
			}
		}
	}
	else if (bItemLevelDown)
	{
		if(!_pUIMgr->DoesMessageBoxExist(MSGCMD_ITEM_LEVELDOWN_REQ))
		{
			strTitle.PrintF(_S( 191, "확인" ));
			strMessage.PrintF(_S(3979, "[유료아이템] '%s'에 아이템 레벨제한 감소를 사용하시겠습니까?"), szItemName);
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_INVENTORY, MSGCMD_ITEM_LEVELDOWN_REQ );
			MsgBoxInfo.AddString( strMessage );
			bLock = FALSE;
		}
	}
	else if(bChaosStone)
	{
		if( slWearPlus > 11 )
		{
			_pUIMgr->CloseMessageBox(MSGCMD_NULL);

			strTitle.PrintF(_S( 1670, "주의!!!" ));
			strMessage.PrintF(_S(4134, "카오스 제련석은 +12이상의 장비에는 사용할 수 없습니다." ) );
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( strMessage );
			bLock = FALSE;
		}
		else
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 175, "아이템 업그레이드" ), UMBS_OKCANCEL, UI_INVENTORY, MSGCMD_INVEN_UPGRADE_REQ );

			CTString strItemname;
			if(slWearPlus>0)
				strItemname.PrintF("[%s +%d]", szItemName, slWearPlus);
			else
				strItemname.PrintF("[%s]", szItemName);

			strMessage.PrintF( _S(4135, "%s<를> 카오스 제련석을 사용하여 업그레이드 하시겠습니까?" ), strItemname);
			MsgBoxInfo.AddString( strMessage );
		}
	}
	else
	{
		bLock = TRUE;
		CTString	strMessage;
		// +3 ~
		if( slWearPlus > 2 )
		{
			//const char	*szItemName = rItemData.GetName();
			strMessage.PrintF( _S2( 172, szItemName, "[%s +%d]<를> 업그레이드 하시겠습니까? 실패하면 아이템이 파괴될 수 있습니다." ), szItemName, slWearPlus );
		}
		// 0 ~ +2
		else if( slWearPlus > 0 )
		{
			//const char	*szItemName = rItemData.GetName();
			strMessage.PrintF( _S2( 173, szItemName, "[%s +%d]<를> 업그레이드 하시겠습니까? 실패하면 능력치가 상승하지 않습니다." ), szItemName, slWearPlus );
		}
		// 0
		else
		{
			//const char	*szItemName = rItemData.GetName();
			strMessage.PrintF( _S2( 174, szItemName, "[%s]<를> 업그레이드 하시겠습니까? 실패하면 능력치가 상승하지 않습니다." ), szItemName );
		}

		// [070824: Su-won] REFINE_PROTECT
#ifdef REFINE_PROTECT
		if( slWearPlus > 2 )
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 175, "아이템 업그레이드" ), UMBS_OKCANCEL | UMBS_BUTTONEX,
									UI_INVENTORY, MSGCMD_INVEN_UPGRADE_PROTECT_REQ );
			MsgBoxInfo.SetBtnType(UBET_ITEM, -1, -1);

			MsgBoxInfo.AddString( strMessage );
			MsgBoxInfo.AddString( _S(3660, "파괴되지 않으려면 제련보호석을 사용하세요.") );

			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			_pUIMgr->GetMessageBox(MSGCMD_INVEN_UPGRADE_PROTECT_REQ)->GetBtnOK().SetEnable(TRUE);

			// Lock inventory
			if(bLock)
				Lock( TRUE, TRUE, LOCK_UPGRADE_ITEM );

			return;
		}
		else
#endif
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 175, "아이템 업그레이드" ), UMBS_OKCANCEL,
										UI_INVENTORY, MSGCMD_INVEN_UPGRADE_REQ );
			MsgBoxInfo.AddString( strMessage );
		}
	}
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Lock inventory
	if(bLock)
		Lock( TRUE, FALSE, LOCK_UPGRADE_ITEM );
}

// ----------------------------------------------------------------------------
// Name : OptionAddItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::OptionAddItem( SBYTE sbWearPos, SLONG slIndex,
								SBYTE sbItemRow, SBYTE sbItemCol )
{
	// If inventory is already locked
	if( IsLocked() )
	{
		ShowLockErrorMessage();
		return;
	}

	SBYTE	sbWearTab = m_abtnWearing[sbWearPos].GetItemTab();
	SBYTE	sbWearRow = m_abtnWearing[sbWearPos].GetItemRow();
	SBYTE	sbWearCol = m_abtnWearing[sbWearPos].GetItemCol();

	CItems		&rItems = _pNetwork->MySlotItem[sbWearTab][sbWearRow][sbWearCol];
	CItemData	&rItemData = rItems.ItemData;

	// FIXME : 스킬사용중에 블러드 젬등을 사용하면, 응답이 안와서 인벤토리 락이 안풀어짐...
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 794, "스킬 사용중에는 옵션을 붙일 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 926, "순간 이동중에는 옵션을 붙일 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SLONG	slWearLevel = rItemData.GetLevel();

	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_OPTION_ADD_REP );

	SLONG	slWearPlus = rItems.Item_Plus;
	sbTempRow1 = sbWearRow;
	sbTempCol1 = sbWearCol;
	slTempPlus = slWearPlus;
	sbTempRow2 = sbItemRow;
	sbTempCol2 = sbItemCol;
	sbTempWearPos = sbWearPos;

	CTString	strMessage;

	const char* szItemName = _pNetwork->GetItemName( rItemData.GetItemIndex() );
	// +1 ~
	if( slWearPlus > 0 )
	{
		if( slIndex == 2361 ) //'피와 땀의 결정' 아이템일 때
			strMessage.PrintF( _S(3533, "[유료아이템]피와 땀의 결정을 사용해 [%s +%d]에 옵션을 붙이시겠습니까?" ), szItemName, slWearPlus );		
		else
			strMessage.PrintF( _S( 513, "[%s +%d]에 옵션을 붙이시겠습니까?" ), szItemName, slWearPlus );		
	}
	// 0
	else
	{
		if( slIndex == 2361 ) //'피와 땀의 결정' 아이템일 때
			strMessage.PrintF( _S(3534, "[유료아이템]피와 땀의 결정을 사용해 [%s]에 옵션을 붙이시겠습니까?" ), szItemName, slWearPlus );		
		else
			strMessage.PrintF( _S( 514, "[%s]에 옵션을 붙이시겠습니까?" ), szItemName );		
	}

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 515, "블러드 옵션" ), UMBS_OKCANCEL,			
								UI_INVENTORY, MSGCMD_INVEN_OPTION_ADD_REQ );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Lock inventory
	Lock( TRUE, FALSE, LOCK_BLOOD_OPTION );
}

// ----------------------------------------------------------------------------
// Name : OptionAddItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::OptionDelItem( SBYTE sbWearPos, SLONG slIndex,
								SBYTE sbItemRow, SBYTE sbItemCol )
{
	// If inventory is already locked
	if( IsLocked() )
	{
		ShowLockErrorMessage();
		return;
	}

	SBYTE	sbWearTab = m_abtnWearing[sbWearPos].GetItemTab();
	SBYTE	sbWearRow = m_abtnWearing[sbWearPos].GetItemRow();
	SBYTE	sbWearCol = m_abtnWearing[sbWearPos].GetItemCol();

	CItems		&rItems = _pNetwork->MySlotItem[sbWearTab][sbWearRow][sbWearCol];
	CItemData	&rItemData = rItems.ItemData;

	// FIXME : 스킬사용중에 블러드 젬등을 사용하면, 응답이 안와서 인벤토리 락이 안풀어짐...
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 795, "스킬 사용중에는 옵션을 제거할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 927, "순간 이동중에는 옵션을 제거할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SLONG	slWearLevel = rItemData.GetLevel();

	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_OPTION_DEL_REP );

	SLONG	slWearPlus = rItems.Item_Plus;
	sbTempRow1 = sbWearRow;
	sbTempCol1 = sbWearCol;
	slTempPlus = slWearPlus;
	sbTempRow2 = sbItemRow;
	sbTempCol2 = sbItemCol;
	sbTempWearPos = sbWearPos;

	CTString	strMessage;

	const char* szItemName = _pNetwork->GetItemName( rItemData.GetItemIndex() );
	// +1 ~
	if( slWearPlus > 0 )
	{
		strMessage.PrintF( _S( 516, "[%s +%d]의 옵션을 제거하시겠습니까?" ), szItemName, slWearPlus );		
	}
	// 0
	else
	{
		strMessage.PrintF( _S( 517, "[%s]의 옵션을 제거하시겠습니까?" ), szItemName );	
	}

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 515, "블러드 옵션" ), UMBS_OKCANCEL,		
								UI_INVENTORY, MSGCMD_INVEN_OPTION_DEL_REQ );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Lock inventory
	Lock( TRUE, FALSE, LOCK_BLOOD_OPTION );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_BOOSTER_UPGRADE_REQ:
		if( bOK )
		{
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 863, "스킬 사용중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );
				Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
			}
			else if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 925, "순간 이동중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );	
				Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
			}
			else
				_pNetwork->SendBoosterUpgrade(m_iItemServerIndex);
		}
		else
			Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
		break;

	case MSGCMD_INVEN_UPGRADE_REQ:
		if( bOK )
		{
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 863, "스킬 사용중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );
				Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
			}
			else if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 925, "순간 이동중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );	
				Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
			}
			else
				SendUpgradeItem();
		}
		else
			Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
		break;

	case MSGCMD_INVEN_OPTION_ADD_REQ:
		if( bOK )
		{
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 794, "스킬 사용중에는 옵션을 붙일 수 없습니다." ), SYSMSG_ERROR );
				Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
			}
			else if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 926, "순간 이동중에는 옵션을 붙일 수 없습니다." ), SYSMSG_ERROR );	
				Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
			}
			else
				SendOptionAddItem();
		}
		else
			Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
		break;

	case MSGCMD_INVEN_OPTION_DEL_REQ:
		if( bOK )
		{
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 795, "스킬 사용중에는 옵션을 제거할 수 없습니다." ), SYSMSG_ERROR );
				Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
			}
			else if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 927, "순간 이동중에는 옵션을 제거할 수 없습니다." ), SYSMSG_ERROR );	
				Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
			}
			else
				SendOptionDelItem();
		}
		else
			Lock( FALSE, FALSE, LOCK_BLOOD_OPTION );
		break;


	// wooss 070305 --------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	case MSGCMD_WHITEDAY_BLUELETTER:
		if( bOK )
		{
			// TODO : Send blue,pink letter message
			COLOR colMsg(0x5EB9FFFF);
			CTString tStr = _pUIMgr->GetMessageBox(nCommandCode)->GetInputBox().GetString();
			_pNetwork->SendWhiteday2007Letter(BLUE_LETTER_ITEM,colMsg,tStr);
			
		}
		break;
	case MSGCMD_WHITEDAY_PINKLETTER:
		if( bOK )
		{
			// TODO : Send blue,pink letter message
			COLOR colMsg(0xFF96C2FF);
			CTString tStr = _pUIMgr->GetMessageBox(nCommandCode)->GetInputBox().GetString();
			_pNetwork->SendWhiteday2007Letter(PINK_LETTER_ITEM,colMsg,tStr);
			
		}
		break;
	
	
	//------------------------------------------------------------------<<
	case MSGCMD_ITEM_LEVELDOWN_REQ:
		if (bOK)
		{
			if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
			{
				_pUIMgr->GetChatting()->AddSysMessage(_S(3980, "스킬 사용중에는 아이템 레벨제한 감소를 할 수 없습니다."), SYSMSG_ERROR);
				Lock(FALSE, FALSE, LOCK_UPGRADE_ITEM);
			}
			else if (_pUIMgr->IsCSFlagOn(CSF_TELEPORT))
			{
				_pUIMgr->GetChatting()->AddSysMessage(_S(3981, "순간 이동중에는 아이템 레벨제한 감소를 할 수 없습니다."), SYSMSG_ERROR);
				Lock(FALSE, FALSE, LOCK_UPGRADE_ITEM);
			}
			else
				SendItemLvDown();
		}
		else
			Lock(FALSE, FALSE, LOCK_UPGRADE_ITEM);
		break;
		
		// [070820: Su-won] REFINE_PROTECT
	case MSGCMD_INVEN_UPGRADE_PROTECT_REQ:
		{
			if( bOK )
			{
				if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
				{
					_pUIMgr->GetChatting()->AddSysMessage( _S( 863, "스킬 사용중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );
					Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
				}
				else if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
				{
					_pUIMgr->GetChatting()->AddSysMessage( _S( 925, "순간 이동중에는 아이템을 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );	
					Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );
				}
				else
				{
					if( !_pUIMgr->GetMessageBox(MSGCMD_INVEN_UPGRADE_PROTECT_REQ)->GetBtnEx(0).IsEmpty() )
					{
						int iItemIndex =_pUIMgr->GetMessageBox(MSGCMD_INVEN_UPGRADE_PROTECT_REQ)->GetBtnEx(0).GetItemIndex();
						SendUpgradeItem( iItemIndex );
					}
					else
						SendUpgradeItem();
				}
			}
			else
				Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM );

			break;
		}
		// [071122: Su-won] NEW_MOONSTONE
	case MSGCMD_MOONSTONE_MIX:
		{
			if( bOK )
			{
				_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->GetColorBox().Start(0, 3);
			}
			else
				_pUIMgr->CloseMessageBox(MSGCMD_MOONSTONE_MIX);
		}
		break;
	case MSGCMD_PETTRANSFORM_RU:
		{
			if (bOK)
			{
				int nUniIndex;
				int tv_tab, tv_row, tv_col;

				_pUIMgr->GetMessageBox(MSGCMD_PETTRANSFORM_RU)->GetBtnUseItemInfo( tv_tab, tv_row, tv_col, nUniIndex );
				CItems& ItemsBtn = _pNetwork->MySlotItem[tv_tab][tv_row][tv_col];

				_pNetwork->SendItemUse(ItemsBtn.Item_Tab, ItemsBtn.Item_Row, ItemsBtn.Item_Col, ItemsBtn.Item_UniIndex, 0 );
			}

			_pUIMgr->CloseMessageBox(MSGCMD_PETTRANSFORM_RU);
		}
		break;

	case MSGCMD_PRESSCORPS:
		{
			int tv_tab, tv_row, tv_col;
			_pUIMgr->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_row,tv_col);
			CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_row][tv_col];
			_pNetwork->SendPresscorpsMessage(Items.Item_UniIndex, strInput);

			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PRESSCORPS );
		}
		break;
	case MSGCMD_ITEMDELETE:
		{
			if (bOK)
			{
				_pNetwork->SendItemDelete(m_abtnDestroyItem.GetItemTab(), m_abtnDestroyItem.GetItemRow(), m_abtnDestroyItem.GetItemCol(),
											m_abtnDestroyItem.GetItemUniIndex(), m_abtnDestroyItem.GetItemCount());
			}

			_pUIMgr->CloseMessageBox(MSGCMD_ITEMDELETE);
		}
		break;
	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendArrangeItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendArrangeItem()
{
	// If inventory is locked
	if( IsLocked() )
		return;

	// If arrange button is locked
	if( m_bLockArrange )
		return;

	// If current tab is not normal item
//	if( m_nCurItemTab != ITEM_TAB_NORMAL )
//		return;

	if( _pUIMgr->GetCollectBox()->IsVisible() )
	{// 곤충 채집 상자는 무조건 닫는다.
		_pUIMgr->RearrangeOrder( UI_COLLECTBOX, FALSE );
	}

	// Initialize selected items
//	m_nSelWearItemID = -1;
//	m_nSelSlotItemID[m_nCurItemTab] = -1;

//	_pNetwork->ArrangeItem( m_nCurItemTab );
}

// ----------------------------------------------------------------------------
// Name : SendUseSlotItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendUseSlotItem( int nTab, int nRow, int nCol, SBYTE sbWearType )
{
	// If arrange button is locked
	if( m_bLockArrange )
		return;

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
		return;

	SQUAD llTerm = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - m_abtnItems[nTab][nRow][nCol].GetUseItemStartTime();

	if( llTerm > 500 )
	{
		m_abtnItems[nTab][nRow][nCol].SetUseItemStartTime( _pTimer->GetHighPrecisionTimer().GetMilliseconds() );
	}
	else 
	{ return; }// 0.5초 사이에 같은 버튼을 누른다면 return;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity( 0 );
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
	if( penPlayerEntity->IsSkilling() )
		return;

	if( penPlayerEntity->IsSocialActing() )
	{
		if( m_abtnItems[nTab][nRow][nCol].GetItemWearType() == WEAR_WEAPON )
			return;
	}

	CItems	&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&ItemData = _pNetwork->MySlotItem[nTab][nRow][nCol].ItemData;

	const int iFame = ItemData.GetFame();		// 명성치를 갖고 있는 아이템.
	if( iFame > 0 && iFame > _pNetwork->MyCharacterInfo.fame )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1187,  "명성치가 부족하여 아이템을 사용할 수 없습니다."  ), SYSMSG_ERROR );	
		return;
	}

	if( (penPlayerEntity->IsTransforming() || penPlayerEntity->IsPolymophing() || penPlayerEntity->IsSocialActing()) && 
		( ItemData.GetType() == CItemData::ITEM_WEAPON || ItemData.GetType() == CItemData::ITEM_SHIELD ))//변신상태인데 무기,갑옷을 착용하려고 한다면,
	{
		return;
	}

	// 공격형 펫의 아이템 job 플래그가 Total_job 위치에 되어있음
	if(( ItemData.GetType() == CItemData::ITEM_WEAPON || ItemData.GetType() == CItemData::ITEM_SHIELD ) && ItemData.CanUse(WILDPET_JOB))
	{
		_pNetwork->SendUseWildPetItem(ItemData.GetWearingPosition(),  nTab,  nRow,  nCol,  ItemData.GetItemIndex() );
		return;
	}
	
	if( ItemData.IsFlag(ITEM_FLAG_RARE) )
	{
		int nNeedLevel =  ItemData.GetLevel()-_pNetwork->MySlotItem[nTab][nRow][nCol].GetWearLevelReduction();
		if( _pNetwork->MyCharacterInfo.level < nNeedLevel)
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3183, "레벨이 부족하여 아이템을 착용할 수 없습니다."  ), SYSMSG_ERROR );	
			return;
		}
	}
	
	// Date : 2006-06-28(오후 4:40:57), By eons
	// 이벤트 아이템 사용 체크( 곤충 채집 상자 )
	if( ItemData.GetItemIndex() == 1577 || ItemData.GetItemIndex() == 1578 )
	{
		_pUIMgr->GetCollectBox()->OpenCollectBox( nTab, nRow, nCol );
		return;
	}

	if ( ItemData.GetItemIndex() == 2047 || ItemData.GetItemIndex() == 2633) // 초코 수집상자 , 새해 소망상자
	{
		_pUIMgr->GetBingoBox()->OpenBingoBox( nTab, nRow, nCol );
		return;
	}
	
	if (_pUIMgr->GetPortal()->IsWarpItem(ItemData.GetItemIndex()))
	{ // 존이동 주문서
		_pUIMgr->GetPortal()->SetUseItem(TRUE);
		_pUIMgr->GetPortal()->SetUniItemIndex(rItems.Item_UniIndex);
		_pUIMgr->GetPortal()->AddToZoneList(ItemData.GetNum0(), 0);
		_pUIMgr->GetPortal()->SetNpcIdx(ItemData.GetNum2());
		_pUIMgr->GetPortal()->OpenPortal(_pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.z);
		return;
	}

	if ( ItemData.GetItemIndex() == 2882 )
	{

		
		if(_pUIMgr->DoesMessageBoxExist(MSGCMD_PRESSCORPS)) return;

		CTString strSysMessage;
		CUIMsgBox_Info	MsgBoxInfo;

		MsgBoxInfo.SetMsgBoxInfo( _S(4143, "라카 기자단 확성기"), UMBS_USER_12 | UMBS_INPUT_MASK | UMBS_ALIGN_RIGHT, UI_INVENTORY, MSGCMD_PRESSCORPS, 300 ); 
		MsgBoxInfo.SetUserBtnName( _S(191, "확인"), _S(139, "취소") ); 
		MsgBoxInfo.SetInputBox( 2, 2, 80, 235 );	
	
		strSysMessage.PrintF( _S(4144, "외치기 할 내용을 입력하세요.") ); 
		MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
		
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_PRESSCORPS );

		return;
	}

	// wooss 050806
	// 유료 아이템 사용 체크 
	int tv_index = ItemData.GetItemIndex();
	CUIMsgBox_Info	MsgBoxInfo;
	CTString		strMessage[MAX_MSGSTRING];
	int tv_t1,tv_t2; // 확장슬롯 임시 변수
	switch(tv_index)
	{
		case MEMSCROLL_EX_ITEM:
		case MEMSCROLL_EX_ITEM_7DAYS:
			
			if((_pUIMgr->GetTeleport()->GetUseTime())>0) {
//			//메모리 스크롤을 이미 사용하고 있을때
//			//wooss 050816
//			strMessage[0]	=	"사용 확인";
//			strMessage[1]	=	"이미 메모리 스크롤이 확장되어 있는 상태 입니다.";
//			strMessage[2]   =   "기간을 연장하시겠습니까?";	
//			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_PROLONG_MEMSCROLL_EX);
//			MsgBoxInfo.AddString(strMessage[1]);
//			MsgBoxInfo.AddString(strMessage[2]);
//			_pUIMgr->CreateMessageBox(MsgBoxInfo);
//			MSG_ITME_PROLONG에서 처리하기로 함
			}
			else {
			// 메모리 스크롤을 사용하고 있지 않을 때

			//wooss 050816
			strMessage[0]	=_S( 2088, 	"사용 확인" );	
			strMessage[1]	=_S( 2089, 	"메모리 스크롤을 확장하시겠습니까?" );	
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_MEMSCROLL_EX);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;
			}
			break;

		case WAREHOUSE_EX_ITEM:
		case WAREHOUSE_EX_ITEM_7DAYS:			
//			if((_pUIMgr->GetWareHouse()->GetUseTime())<=0)
			{
				//wooss 050816
				strMessage[0]	=_S( 2088, 	"사용 확인" ); 
				strMessage[1]	=_S( 2090, 	"창고를 확장하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_WAREHOUSE_EX);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				return;
			}
			break;
		case CHANGE_MY_NAME_ITEM:
		case CHANGE_MY_NAME_ITEM_UE:

			//wooss 050812
			strMessage[0]	=_S( 2088, 	"사용 확인" ); 
			strMessage[1]	=_S( 2091, 	"개명카드를 사용하시겠습니까?" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_CHANGE_MY_NAME);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;

		case CHANGE_GUILD_NAME_ITEM:

			//wooss 050812
			strMessage[0]	=_S( 2088, 	"사용 확인" );
			strMessage[1]	=_S( 2092, 	"길드 개명카드를 사용하시겠습니까?" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_CHANGE_GUILD_NAME);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;
			
		case RECOVER_HEXP_ITEM:
		case RECOVER_HEXP_ITEM_LUCKY:
			{
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);				
				
				if(_pUIBuff->IsBuff(RECOVER_HEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_HEXP_ITEM_LUCKY)) 
				{
					strMessage[0] = _S( 2093, "이미 경험치 복구 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
				// wooss 050926
				if(_pUIBuff->IsBuff(RECOVER_AEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_AEXP_ITEM_LUCKY) )
				{
					
					strMessage[0] = _S(2095 , "이미 숙련도 복구 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
				if(_pUIBuff->IsBuff(REBIRTH_ITEM)){
					strMessage[0] = _S( 2094, "이미 부활 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
			}

			break;

		case RECOVER_AEXP_ITEM:
		case RECOVER_AEXP_ITEM_LUCKY:
			{
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);	

				if(_pUIBuff->IsBuff(RECOVER_AEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_AEXP_ITEM_LUCKY))
				{
					
					strMessage[0] = _S(2095 , "이미 숙련도 복구 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
				// wooss 050926
				if(_pUIBuff->IsBuff(RECOVER_HEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_HEXP_ITEM_LUCKY)) 
				{
					strMessage[0] = _S( 2093, "이미 경험치 복구 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
				if(_pUIBuff->IsBuff(REBIRTH_ITEM)){
					
					strMessage[0] = _S( 2094, "이미 부활 주문서를 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;

				}
			}

			break;

		case REBIRTH_ITEM:
		case REBIRTH_ITEM_NEWBIE:
			{
				_pUIMgr->CloseMessageBox(MSGCMD_NULL);	

				if(_pUIBuff->IsBuff(RECOVER_HEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_HEXP_ITEM_LUCKY) )
				{
					strMessage[0] = _S( 2093, "이미 경험치 복구 주문서를 사용중입니다." );
					strMessage[1] = _S( 2096, "부활 주문서를 사용하실 경우 기존 주문서의 효력은 사라집니다." );
					strMessage[2] = _S( 2097, "사용하시겠습니까?" );	 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_YESNO,
							UI_NONE, MSGCMD_CHECK_ITEM_HEXP );
					MsgBoxInfo.AddString( strMessage[0] );
					MsgBoxInfo.AddString( strMessage[1] );
					MsgBoxInfo.AddString( strMessage[2] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}

				if(_pUIBuff->IsBuff(RECOVER_AEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_AEXP_ITEM_LUCKY) )
				{
					strMessage[0] = _S( 2095, "이미 숙련도 주문서를 사용중입니다." );
					strMessage[1] = _S( 2096, "부활 주문서를 사용하실 경우 기존 주문서의 효력은 사라집니다." );
					strMessage[2] = _S( 2097, "사용하시겠습니까?" ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_YESNO,
							UI_NONE, MSGCMD_CHECK_ITEM_AEXP );
					MsgBoxInfo.AddString( strMessage[0] );
					MsgBoxInfo.AddString( strMessage[1] );
					MsgBoxInfo.AddString( strMessage[2] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;

				}

				if(_pUIBuff->IsBuff(REBIRTH_ITEM))
				{					
					strMessage[0] = _S( 2094, "이미 부활 주문서를 사용중입니다" ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
			
			}

			break;

		case REBIRTH_ITEM_PHOENIX:
			{
				if(_pUIBuff->IsBuff(REBIRTH_ITEM_PHOENIX))
				{					
					strMessage[0] = _S( 4258, "이미 피닉스의 부활을 사용중입니다." ); 
					MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK,
							UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage[0] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					return;
				}
			}
			break;

		case WARP_ITEM:
			// Date : 2006-05-30(오전 11:39:42), By eons
			if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 2739, "다른 행동 중에는 순간이동 마법서를 사용할 수 없습니다." ) );
				return;
			}
			
			//wooss 050808
			strMessage[0]	=_S( 2098, 	"순간이동 마법서" );	
			strMessage[1]	=_S( 2099, 	"순간 이동을 할 위치에 있는 캐릭터의 이름을 입력해 주세요" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_INPUTBOX|UMBS_YESNO,UI_NONE,MSGCMD_USE_WARP_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo); // UIManager에서 MSGCMD_USE_WARP_ITEM를 확인하고 처리
			
			return;	
			
		case PC_SUMMON_ITEM:
			// Date : 2006-05-30(오전 11:39:42), By eons
			if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 2740, "다른 행동 중에는 소환 마법서를 사용할 수 없습니다." ) );
				return;
			}

			//wooss 050817
			strMessage[0]	=_S( 2100, 	"소환 마법서" );	
			strMessage[1]	=_S( 2101, 	"소환할 캐릭터의 이름을 입력해 주세요" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_INPUTBOX|UMBS_YESNO,UI_NONE,MSGCMD_USE_PC_SUMMON_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo); // UIManager에서 MSGCMD_USE_PC_SUMMON_ITEM를 확인하고 처리
			
			return;	

		case BAAL_SUMMON_ITEM :
			//wooss 050818
			if(_pUIMgr->DoesMessageBoxExist(BAAL_SUMMON_ITEM)) return;
			strMessage[0]	=_S( 2100, "소환 마법서" );	
			strMessage[1]	=_S( 2102, 	"보스 몬스터 소환서를 " );
			strMessage[2]	=_S( 2097,  "사용하시겠습니까?" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_USE_BOSS_SUMMON_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			MsgBoxInfo.AddString(strMessage[2]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo); // UIManager에서 MSGCMD_USE_BOSS_SUMMON_ITEM를 확인하고 처리
			return;
		
		case DEATHNIGHT_SUMMON_ITEM :
			//wooss 050818
			if(_pUIMgr->DoesMessageBoxExist(DEATHNIGHT_SUMMON_ITEM)) return;
			strMessage[0]	=_S( 2100, 	"소환 마법서" );	
			strMessage[1]	=_S( 2102, 	"보스 몬스터 소환서를 " ); 
			strMessage[2]	=_S( 2097,   "사용하시겠습니까?" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_USE_BOSS_SUMMON_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			MsgBoxInfo.AddString(strMessage[2]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo); // UIManager에서 MSGCMD_USE_BOSS_SUMMON_ITEM를 확인하고 처리
			return;
			
		case INIT_STATUS_ITEM :
			//wooss 050818
			strMessage[0]	=_S( 2103, 	"스테이터스 초기화 카드" );	
			strMessage[1]	=_S( 2104, 	"카드를 사용하면 일부  필요한 스테이터스가 있는 스킬은 사용하지 못할 수 있습니다" ); 
			
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_USE_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			
			_pUIMgr->CreateMessageBox(MsgBoxInfo); // UIManager에서 MSGCMD_USE_BOSS_SUMMON_ITEM를 확인하고 처리
			return;
		case INIT_COMBAT_SKILL_ITEM:
			strMessage[0]	=_S( 2105, 	"전투스킬 초기화 카드" ); 
			strMessage[1]	=_S( 2106, 	"카드를 사용하면 전직 포기와 함께, 일반, 강화 스킬 모드가 초기화되며 스킬을 배우는데 사용된 숙련도는 모두 환원 됩니다" );
			
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_USE_ITEM );
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;
		case INIT_SPECIAL_SKILL_ITEM:
			strMessage[0]	=_S( 2107, 	"특수스킬 초기화 카드" );
			strMessage[1]	=_S( 2108, 	"특수스킬 초기화 카드를 사용하면 생산, 가공, 제작 스킬 모두가 초기화되며 스킬을 배우는데 사용된 숙련도는 모두 환원 됩니다" ); 
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0], UMBS_YESNO,UI_NONE,MSGCMD_USE_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;	
		case INIT_ALL_ITEM:
			strMessage[0]	=_S( 2109, 	"능력치 초기화 카드" );
			strMessage[1]	=_S( 2110, 	"캐릭터의 스테이터스, 일반, 강화, 특수 스킬, 초기화, 전직 포기가 되며 스킬을 배우는데 사용된 숙련도는 모두 환원 됩니다" );
			MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_USE_ITEM);
			MsgBoxInfo.AddString(strMessage[1]);
			_pUIMgr->CreateMessageBox(MsgBoxInfo);
			return;
		//wooss 050820
		case EX_SLOT_ITEM :
			tv_t1=_pUIMgr->GetSelChar()->GetExSlotTime(1);
			tv_t2=_pUIMgr->GetSelChar()->GetExSlotTime(2);
			strMessage[0] =_S(191,"확인");
			if(tv_t1||tv_t2)
			{// 이미 슬롯이 확장된 상태 -> 연장
				strMessage[1] =_S( 2111,  "현재 이미 확장된 슬롯" );		
				if(tv_t1) strMessage[2] =_S( 2112,  "확장슬롯 1 ");		
				if(tv_t2) strMessage[3] = _S( 2113, "확장슬롯 2 " );	
//				strMessage[4] =("입니다");
				strMessage[2] += strMessage[3];//+strMessage[4];
				strMessage[3] = _S( 2116, "확장 또는 연장 하시겠습니까?" ) ;
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_SLOT_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				MsgBoxInfo.AddString(strMessage[2]);
				MsgBoxInfo.AddString(strMessage[3]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			else 
			{// 어느 슬롯도 확장이 안 된 상태
				strMessage[1] =_S( 2117,  "캐릭터 슬롯 확장 카드를 사용하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_SLOT_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;
		// wooss 051011	
		// [070613: Su-won] 무기 교환 카드
		case EXCHANGE_WEAPON_ITEM :
			if(_pUIMgr->GetChangeWeapon()->GetCashItem()) return;
			else {
				SetUseItemSlotInfo(nTab, nRow, nCol);
				_pUIMgr->GetChangeWeapon()->SetCashItem(TRUE);
				_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, TRUE );
				_pUIMgr->GetChangeWeapon()->MsgBoxLCommand(MSGLCMD_CHANGEWEAPON_REQ, 0);
				// Lock inventory
				_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_CHANGEWEAPON );
			}			
			return;	
			
		// [070613: Su-won] 방어구 교환 카드
		case EXCHANGE_ARMOR_ITEM:
			if(_pUIMgr->GetChangeWeapon()->GetCashItem()) return;
			else {
				SetUseItemSlotInfo(nTab, nRow, nCol);
				_pUIMgr->GetChangeWeapon()->SetCashItem(TRUE);
				_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, TRUE );
				_pUIMgr->GetChangeWeapon()->MsgBoxLCommand(MSGLCMD_CHANGEWEAPON_REQ, 3);
				// Lock inventory
				_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_CHANGEWEAPON );
			}			
			return;	
			
		case PARTY_RECALL_ITEM1 :
		case PARTY_RECALL_ITEM2 :
		case PARTY_RECALL_ITEM3 :
		case PARTY_RECALL_ITEM4 :
		case PARTY_RECALL_ITEM5 :
		case PARTY_RECALL_ITEM6 :

			{// 파티 리콜 확인 
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_CONFIRM_PARTY_RECALL_ITEM)) return;
				if(_pUIMgr->GetParty()->GetMemberCount() == 0)
				{
					_pNetwork->ClientSystemMessage(_S(3408, "소환할 파티원들이 없습니다."), SYSMSG_ERROR);
					return;
				}

				strMessage[1] =_S(2515, "파티원들을 소환 하시겠습니까?" );
				strMessage[2].PrintF(_S(3286, "[유료아이템] %s을 개봉하시겠습니까?"), ItemData.GetName());

				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_PARTY_RECALL_ITEM);

				if (rItems.Item_Used < 0) { MsgBoxInfo.AddString(strMessage[2]); }
				else { MsgBoxInfo.AddString(strMessage[1]); }

				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;

		case PLATINUM_UPGRADESTONE_ITEM1 :		//		1418	
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_CONFIRM_UPGRADESTONE_ITEM)) return;
				strMessage[1] =_S( 2707, "플래티늄 제련석을 감정하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_UPGRADESTONE_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				
			}
			return;

		case COMPOSITE_ITEM1 :					//		1421
		case COMPOSITE_ITEM_EVENT1:
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_CONFIRM_MIXNEW_ITEM)) return;
				strMessage[1] =_S( 2714, "결합 주문서를 감정하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_MIXNEWREQ_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;
			
		case COMPOSITE_ITEM2 :					//		1421
		case COMPOSITE_ITEM_EVENT2:
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_CONFIRM_MIXNEW_ITEM)) return;
				strMessage[1] =_S( 2734, "결합 주문서를 사용하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_MIXNEW_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;
		
		case PET_MOUNT_CANCEL_CARD :
			{
				if( _pUIMgr->DoesMessageBoxExist( MSGCMD_PET_MOUNT_CANCEL_CARD ) ) return;

				int nTab, nRow, nCol;
				_pUIMgr->GetInventory()->GetUseItemSlotInfo(nTab,nRow,nCol);

				strMessage[0] = _S( 2896, "펫 마운트 훈련 취소" );
				MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGCMD_PET_MOUNT_CANCEL_CARD );
				strMessage[1] = _S( 2897, "탈 것(마운트) 훈련을 취소할 애완동물을 올려 주세요.\n단, 애완 동물의 스킬은 초기화된 상태여야 합니다." );
				MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ACCESSORY, CItemData::ACCESSORY_PET );
				MsgBoxInfo.AddString( strMessage[1] );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );		
				_pUIMgr->GetMessageBox(MSGCMD_PET_MOUNT_CANCEL_CARD)->SetInvenUseItemInfo( nTab, nRow, nCol );
			}
			return;
		case UNCOMPOSITE_ITEM :					//		1540
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_CONFIRM_UNMIX_ITEM)) return;
				strMessage[1] =_S(2928, "결합 분리 주문서를 사용하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_UNMIX_ITEM);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;
		case PACKING_PAPER_ITEM :
		case SMALL_CORD_ITEM :
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_USE_PACKING_PAPER) || 
					_pUIMgr->DoesMessageBoxExist(MSGCMD_USE_SMALL_CORD))
					return;
				
				int nTab, nRow, nCol;
				_pUIMgr->GetInventory()->GetUseItemSlotInfo(nTab,nRow,nCol);				

				if (tv_index == SMALL_CORD_ITEM)
				{
					strMessage[0] = _S(3993, "복주머니 노끈");
					strMessage[1] = _S(3994, "복주머니를 올린 후 확인버튼을 클릭하세요");

					MsgBoxInfo.SetMsgBoxInfo(strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGCMD_USE_SMALL_CORD);
					MsgBoxInfo.SetBtnType(UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_EVENT);
					MsgBoxInfo.AddString(strMessage[1]);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
					_pUIMgr->GetMessageBox(MSGCMD_USE_SMALL_CORD)->SetInvenUseItemInfo(nTab, nRow, nCol);
				}
				else
				{
					strMessage[0] = _S(3199, "초코포장지" );
					strMessage[1] = _S(3200, "초코렛을 올린 후 확인버튼을 클릭하세요");

					MsgBoxInfo.SetMsgBoxInfo(strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGCMD_USE_PACKING_PAPER);
					MsgBoxInfo.SetBtnType(UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_EVENT);
					MsgBoxInfo.AddString(strMessage[1]);
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
					_pUIMgr->GetMessageBox(MSGCMD_USE_PACKING_PAPER)->SetInvenUseItemInfo(nTab, nRow, nCol);
				}
			}
			return;

		// wooss 070305 --------------------------------------------------------------------------------->>
		// kw : WSS_WHITEDAY_2007
		case BLUE_LETTER_ITEM :					//		2135
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WHITEDAY_BLUELETTER)) return;
				strMessage[0] =_S( 3224, "하늘색 편지지" );
				strMessage[1] =_S( 3225, "메시지 내용을 입력하세요.(50자)" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_OKCANCEL|UMBS_INPUTBOX,UI_INVENTORY,MSGCMD_WHITEDAY_BLUELETTER);
				MsgBoxInfo.AddString(strMessage[1]); // BLUE
				MsgBoxInfo.SetInputBox(3,3,50,150);
				MsgBoxInfo.m_nMaxRow = 5;
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
		//		_pUIMgr->GetMessageBox(MSGCMD_WHITEDAY_BLUELETTER)->GetInputBox().SetMaxChar(256);
				
			}
			return;
		case PINK_LETTER_ITEM :					//		2136
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_WHITEDAY_PINKLETTER)) return;
				strMessage[0] =_S( 3226, "분홍색 편지지" );
				strMessage[1] =_S( 3225, "메시지 내용을 입력하세요.(50자)" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_OKCANCEL|UMBS_INPUTBOX,UI_INVENTORY,MSGCMD_WHITEDAY_PINKLETTER);
				MsgBoxInfo.AddString(strMessage[1]); // PINK
				MsgBoxInfo.SetInputBox(3,3,50,150);
				MsgBoxInfo.m_nMaxRow = 5;
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			//	_pUIMgr->GetMessageBox(MSGCMD_WHITEDAY_BLUELETTER)->GetInputBox().SetMaxChar(256);
			}
			return;
		// -----------------------------------------------------------------------------------------------<<
// [KH_070326] 달걀 이벤트 관련
		case EASTER_EGGS:
			{
				strMessage[0]	=_S( 2088, "사용 확인" );	
				strMessage[1]	=_S( 3267, "선택한 부활절 달걀을 개봉 하시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0],UMBS_YESNO,UI_NONE,MSGCMD_CONFIRM_EASTER_EGGS);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;
		// [070613: Su-won] 펫 명찰 아이템
		case PET_NAMECARD_ITEM:
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_PET_NAMECARD_USE)) 
					return;
				strMessage[1] =_S(3527, "[유료아이템] 펫의 명찰을 사용하시겠습니까?");
				MsgBoxInfo.SetMsgBoxInfo(CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)) ,UMBS_YESNO,UI_NONE,MSGCMD_PET_NAMECARD_USE);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);	
			}
			return;			

		// [070824: Su-won] PET_COLOR_CHANGE
		case PET_COLOR_RED:
		case PET_COLOR_ORNAGE:
		case PET_COLOR_YELLOW:
		case PET_COLOR_GREEN:
		case PET_COLOR_BLUE:
		case PET_COLOR_INDIGO:
		case PET_COLOR_VIOLET:
		case PET_COLOR_WHITE:
		case PET_COLOR_BLACK:
		case PET_COLOR_PHOENIX:
		case PET_COLOR_BLACK_HEART:
		case PET_COLOR_WHITE_HEART:
		case PET_COLOR_YELLOW_HEART:
			{
				if(_pUIMgr->DoesMessageBoxExist(MSGCMD_PET_COLORCHANGE_USE))
					_pUIMgr->CloseMessageBox(MSGCMD_PET_COLORCHANGE_USE);
//					return;

				if( !_pNetwork->_PetTargetInfo.bIsActive )
				{
					_pNetwork->ClientSystemMessage( _S(2189,"애완동물을 슬롯에 장착하여야만 합니다."), SYSMSG_ERROR );
					return;
				}
				
				if (_pNetwork->_PetTargetInfo.TransformIndex > 0) // 펫이 변신하고 있을 경우
				{
					_pNetwork->ClientSystemMessage( _S(3968, "변신중인 펫에게는 사용할 수 없습니다."));
					return;
				}

				strMessage[1].PrintF( _S(3661, "%s을 사용하여 펫을 결정색으로 물들이시겠습니까?"), _pNetwork->GetItemName(tv_index));
				MsgBoxInfo.SetMsgBoxInfo(CTString(_pNetwork->GetItemName(tv_index)), UMBS_YESNO, UI_NONE, MSGCMD_PET_COLORCHANGE_USE);
				MsgBoxInfo.AddString(strMessage[1]);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			return;

		// [090710: selo] 펫 문양 시스템
		case PET_TATOO_HORSE:
		case PET_TATOO_DRAGON:
			{
				if( !_pNetwork->_PetTargetInfo.bIsActive )
				{
					_pNetwork->ClientSystemMessage( _S(2189,"애완동물을 슬롯에 장착하여야만 합니다."), SYSMSG_ERROR );
					return;
				}
				
				if (_pNetwork->_PetTargetInfo.TransformIndex > 0) // 펫이 변신하고 있을 경우
				{
					_pNetwork->ClientSystemMessage( _S(3968, "변신중인 펫에게는 사용할 수 없습니다."));
					return;
				}
				
				_pUIMgr->GetTatoo()->OpenTatoo();
			}
			return;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// [071122: Su-won] NEW_MOONSTONE
		case MOONSTONE_POOR_MIX_BOX:
		case MOONSTONE_USEFUL_MIX_BOX:
		case MOONSTONE_SOLID_MIX_BOX:
		case MOONSTONE_STRONG_MIX_BOX:
		case MOONSTONE_TRUSTY_MIX_BOX:
			{
				if( _pUIMgr->DoesMessageBoxExist( MSGCMD_MOONSTONE_MIX ) )
					_pUIMgr->CloseMessageBox(MSGCMD_MOONSTONE_MIX);

				int nTab, nRow, nCol;
				_pUIMgr->GetInventory()->GetUseItemSlotInfo(nTab,nRow,nCol);

				int iMoonStoneBox =_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Index;

				strMessage[0] = _pNetwork->GetItemName(iMoonStoneBox);
				MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_OKCANCEL | UMBS_BUTTONEX | UMBS_INPUTBOX, UI_INVENTORY, MSGCMD_MOONSTONE_MIX );
				MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ACCESSORY, CItemData::ACCESSORY_PET );
				MsgBoxInfo.AddString( _S(3900, "조합할 문스톤을 올려 주세요." ) );

				MsgBoxInfo.SetInputBox( 2, 0, 0, MsgBoxInfo.m_nWidth-MSGBOX_MESSAGE_OFFSETX*2 );

				MsgBoxInfo.m_nMaxRow +=4;
				MsgBoxInfo.AddString( _s("X 3") +_s("        "), 0xF2F2F2Ff, TEXT_RIGHT );
				MsgBoxInfo.m_nMaxRow -=4;
				
				MsgBoxInfo.m_nColorBoxCount =1;

				_pUIMgr->CreateMessageBox( MsgBoxInfo );		
				_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->SetInvenUseItemInfo( nTab, nRow, nCol );

				_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->GetInputBox().SetFocus(FALSE);
				_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->GetInputBox().SetInValidEditBox(TRUE);

				int iPosX, iPosY;
				int iWidth, iHeight;

				iPosX =MsgBoxInfo.m_nInputPosX+2;
				iPosY =MsgBoxInfo.m_nInputPosY +2;

				iWidth =(MsgBoxInfo.m_nInputWidth-4);
				iHeight =9;

				WRect rcBox;

				rcBox.SetRect( iPosX, iPosY, iPosX+iWidth, iPosY+iHeight);

				_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->SetColorBox(0, 6, rcBox);
			}
			break;
		// [071122: Su-won] NEW_MOONSTONE
		////////////////////////////////////////////////////////////////////////////////////////////////////
		case PET_TRANSFORM_RU: // 펫 루돌프 변신 아이템
			{
				if (_pUIMgr->DoesMessageBoxExist(MSGCMD_PETTRANSFORM_RU))
					_pUIMgr->CloseMessageBox(MSGCMD_PETTRANSFORM_RU);

				if( !_pNetwork->_PetTargetInfo.bIsActive )
				{
					_pNetwork->ClientSystemMessage( _S(2189,"애완동물을 슬롯에 장착하여야만 합니다."), SYSMSG_ERROR );
					return;
				}

				strMessage[0] = _pNetwork->GetItemName(PET_TRANSFORM_RU);
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0], UMBS_YESNO, UI_INVENTORY, MSGCMD_PETTRANSFORM_RU);
				MsgBoxInfo.AddString(_S(3967, "착용중인 펫을 이벤트 기간 동안만 루돌프로 변신시키겠습니까?"));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
				_pUIMgr->GetMessageBox(MSGCMD_PETTRANSFORM_RU)->SetInvenUseItemInfo( nTab, nRow, nCol );
				return;
			}
			break;
	}
	
	// 일회용-기타일회용 아이템들...
	if( ItemData.GetType() == CItemData::ITEM_ONCEUSE )
	{
		if( ItemData.GetSubType() == CItemData::ITEM_SUB_ETC )
		{
			int tv_index = ItemData.GetItemIndex();
			switch(tv_index)
			{
				case RARE_IDENTITY_ITEM1 :
				case RARE_IDENTITY_ITEM2 :
				case RARE_IDENTITY_ITEM_NEWBIE:
				{
					if( _pUIMgr->DoesMessageBoxExist( MSGCMD_RARE_IDENTIFY ) ) return;

					int nTab, nRow, nCol;
					_pUIMgr->GetInventory()->GetUseItemSlotInfo(nTab,nRow,nCol);

					strMessage[0] = _S(3160, "옵션 감정 아이템" );
					MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGCMD_RARE_IDENTIFY );
					strMessage[1] = _S(3161, "장비에 붙어 있는 미공개 옵션을 감정해주는 아이템입니다. 단, 옵션이 미공개인 장비만 감정할 수 있습니다." );
					MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ACCESSORY, CItemData::ACCESSORY_PET );
					MsgBoxInfo.AddString( strMessage[1] );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );		
					_pUIMgr->GetMessageBox(MSGCMD_RARE_IDENTIFY)->SetInvenUseItemInfo( nTab, nRow, nCol );
				}
				return;
			}
		}
	}
	
	_pNetwork->UseSlotItem( nTab, nRow, nCol, sbWearType );
}

// ----------------------------------------------------------------------------
// Name : SendSwapSlotItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendSwapSlotItems( int nTab0, int nRow0, int nCol0, int nRow1, int nCol1 )
{
	// If arrange button is locked
//	if( m_bLockArrange )
//		return;
	_pNetwork->SwapItem( nTab0, nRow0, nCol0, nRow1, nCol1 );
}

// ----------------------------------------------------------------------------
// Name : SendUpgradeItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendUpgradeItem( INDEX iProtect )
{
	// 제련보호석을 사용했을 때...
	if( iProtect >=0 )
	{
		// 보호제련석의 인벤 위치
		int nRow, nCol;

		for( int iRow = 0; iRow < INVEN_SLOT_ROW_TOTAL; iRow++ )
		{
			for( int iCol = 0; iCol < INVEN_SLOT_COL; iCol++ )
			{
				if( m_abtnItems[ITEM_TAB_NORMAL][iRow][iCol].GetIndex() == iProtect )
				{
					nRow = iRow;
					nCol = iCol;

					_pNetwork->UpgradeItem( sbTempRow1, sbTempCol1, slTempPlus,
							sbTempRow2, sbTempCol2, slTempLevel, iProtect, nRow, nCol );
					return;
				}
			}
		}
	}
	// 제련보호석을 사용하지 않을 때...
	else
	{
		_pNetwork->UpgradeItem( sbTempRow1, sbTempCol1, slTempPlus,
								sbTempRow2, sbTempCol2, slTempLevel );
	}
}

// ----------------------------------------------------------------------------
// Name : SendItemLvDown()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendItemLvDown()
{
	_pNetwork->ItemLevelDown(sbTempRow1, sbTempCol1, sbTempRow2, sbTempCol2);
}

// ----------------------------------------------------------------------------
// Name : SendOptionItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendOptionAddItem()
{
	_pNetwork->OptionAddItem( sbTempRow1, sbTempCol1, sbTempWearPos, sbTempRow2, sbTempCol2 );
}

// ----------------------------------------------------------------------------
// Name : SendOptionDelItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SendOptionDelItem()
{
	_pNetwork->OptionDelItem( sbTempRow1, sbTempCol1, sbTempWearPos, sbTempRow2, sbTempCol2 );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : InitInventory()
// Desc : For initializing inventory ( start game, arrange item ) and
//        deleting item
// ----------------------------------------------------------------------------
void CUIInventory::InitInventory( int nTab, int nRow, int nCol, int nUniIndex, int nIndex, int nWearingType )
{
	// Empty slot
	if( nUniIndex == -1 )
	{
		// Unwear
		nUniIndex = m_abtnItems[nTab][nRow][nCol].GetItemUniIndex();
		for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
		{
			if( m_abtnWearing[iWear].GetItemUniIndex() == nUniIndex )
			{
				m_abtnWearing[iWear].InitBtn();
				if( m_nSelWearItemID == iWear )
					m_nSelWearItemID = -1;

				break;
			}
		}

		// Initialize item
		int	nItemIndex = m_abtnItems[nTab][nRow][nCol].GetItemIndex();
		m_abtnItems[nTab][nRow][nCol].InitBtn();

		int tmpTab, tmpRow;

		if (nTab > 0)
		{
			tmpTab = nTab + INVEN_TAB_NORMAL4;
		}
		else
		{
			tmpTab = nRow / INVEN_SLOT_ROW;
		}

		tmpRow = nRow % INVEN_SLOT_ROW;

		//if( m_nSelSlotItemID[tmpTab] == ( nCol + nRow * INVEN_SLOT_COL ) )
		if( m_nSelSlotItemID[tmpTab] == (nCol * tmpRow) )
			m_nSelSlotItemID[tmpTab] = -1;

		// Money
		if( nItemIndex != -1 )
		{
			CItemData	&ItemData = _pNetwork->GetItemData( nItemIndex );
			if( ItemData.GetType() == CItemData::ITEM_ETC &&
				ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
			{
				m_strMyMoney = CTString( "0" );
				
				// Date : 2005-11-10(오후 5:20:23), By Lee Ki-hwan
				_pNetwork->MyCharacterInfo.money = 0;
			}
		}

		return;
	}

	m_abtnItems[nTab][nRow][nCol].SetItemInfo( nTab, nRow, nCol, nIndex, nUniIndex, nWearingType );
	m_abtnItems[nTab][nRow][nCol].SetItemFlag( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Flag );
	m_abtnItems[nTab][nRow][nCol].SetItemPlus( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus );
	m_abtnItems[nTab][nRow][nCol].SetItemUsed( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Used );
	m_abtnItems[nTab][nRow][nCol].SetItemUsed2( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Used2 );
	m_abtnItems[nTab][nRow][nCol].SetItemRareIndex( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_RareIndex );
	
	m_abtnItems[nTab][nRow][nCol].SetItemCount(_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum);
	
	CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
	m_abtnItems[nTab][nRow][nCol].SetItemPrice(ItemData.GetPrice());

	// Money
	if( nIndex != -1 )
	{
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( ItemData.GetType() == CItemData::ITEM_ETC &&
			ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			CItems	&rItems = _pNetwork->MySlotItem[0][nRow][nCol];
			_pNetwork->MyCharacterInfo.money = rItems.Item_Sum;
			m_strMyMoney.PrintF( "%I64d", rItems.Item_Sum );
			_pUIMgr->InsertCommaToString( m_strMyMoney );
		}
		if(ItemData.GetType() == CItemData::ITEM_ACCESSORY &&
			ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus);
		}
	}

	// Wear
	if( nWearingType != -1 )
	{
		m_abtnWearing[nWearingType].SetItemInfo( nTab, nRow, nCol, nIndex, nUniIndex, nWearingType );
		m_abtnWearing[nWearingType].SetItemFlag(m_abtnItems[nTab][nRow][nCol].GetItemFlag());
		m_abtnWearing[nWearingType].SetItemRareIndex(m_abtnItems[nTab][nRow][nCol].GetItemRareIndex() );
	}
	// Unwear
	else
	{
		for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
		{
			if( m_abtnWearing[iWear].GetItemUniIndex() == nUniIndex )
			{
				m_abtnWearing[iWear].InitBtn();
				if( m_nSelWearItemID == iWear )
					m_nSelWearItemID = -1;

				break;
			}
		}
	}
	
	if( nIndex == 1577 || nIndex == 1578 )
	{// 곤충 박스 갱신
		if( _pUIMgr->GetCollectBox()->IsVisible() )
			_pUIMgr->GetCollectBox()->UpDateItem();
	}	
}

// ----------------------------------------------------------------------------
// Name : SwapSlotItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::SwapSlotItems( int nTab0, int nRow0, int nCol0, int nTab1, int nRow1, int nCol1 )
{
	int tmpRow, tmpCol;
	tmpRow = nRow1 - (m_InvenCurrentTab * INVEN_SLOT_ROW);
	tmpCol = nCol1;

	m_abtnItems[nTab0][nRow0][nCol0].Swap( m_abtnItems[nTab1][nRow1][nCol1] );
	m_nSelSlotItemID[m_InvenCurrentTab] = tmpCol + tmpRow * INVEN_SLOT_COL; // 이걸 왜 설정 해주지~?

	// Refresh tool tip
	ShowItemInfo( TRUE, TRUE, -1, tmpRow, tmpCol );

	// Refresh wearing slot
	SBYTE	sbWearingType = m_abtnItems[nTab0][nRow0][nCol0].GetItemWearType();
	if( sbWearingType != -1 )
		m_abtnWearing[sbWearingType].SetItemLocation( nTab0, nRow0, nCol0 );

	sbWearingType = m_abtnItems[nTab1][nRow1][nCol1].GetItemWearType();
	if( sbWearingType != -1 )
		m_abtnWearing[sbWearingType].SetItemLocation( nTab1, nRow1, nCol1 );

	// Refresh quick slot
	int	nUniIndex = m_abtnItems[nTab0][nRow0][nCol0].GetItemUniIndex();
	if( nUniIndex != -1 )
		_pUIMgr->GetQuickSlot()->RefreshBtn( UBET_ITEM, nUniIndex );
	
	nUniIndex = m_abtnItems[nTab1][nRow1][nCol1].GetItemUniIndex();
	if( nUniIndex != -1 )
		_pUIMgr->GetQuickSlot()->RefreshBtn( UBET_ITEM, nUniIndex );
}

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::UpgradeItem( SBYTE sbResult )
{
	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_UPGRADE_REP );

	CTString	strMessage;
	switch( sbResult )
	{
	case 0:
		strMessage = _S( 176, "업그레이드를 성공하였습니다. +1 상승하였습니다." );
		break;

	case 1:
		strMessage = _S( 177, "업그레이드를 실패하였습니다. -1 하강하였습니다." );
		break;

	case 2:
		strMessage = _S( 178, "업그레이드를 실패하였습니다. 아이템에 변화가 없습니다." );
		break;

	case 3:
		strMessage = _S( 179, "업그레이드를 실패하였습니다. 아이템이 파괴되었습니다." );
		break;
	case 4:
		strMessage = _S( 2644, "플래티늄 제련 효과가 적용되었습니다." );
		break;

	// [070824: Su-won] REFINE_PROTECT
	case 5:
		strMessage = _S(3662, "업그레이드에 실패하여 아이템이 +10이 되었습니다." );
		break;
	}

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 175, "아이템 업그레이드" ), UMBS_OK,
								UI_INVENTORY, MSGCMD_INVEN_UPGRADE_REP );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	
	// Unlock inventory
	Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
}

// ----------------------------------------------------------------------------
// Name : OptionAddRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::OptionAddRep( SBYTE sbResult )
{
	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_OPTION_ADD_REP );
	
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_OPTION_ADD_ERROR_SYSTEM:		// 있어서는 안되는 오류
		strMessage = _S( 518, "옵션 붙이기에 실패 하였습니다." );		
		break;
	case MSG_OPTION_ADD_ERROR_WEAR:			// 착용한 아이템어야지
		strMessage = _S( 519, "착용한 아이템이어야 합니다." );		
		break;
	case MSG_OPTION_ADD_ERROR_KIND:			// 무기나 방어구만 가능하다
		strMessage = _S( 520, "무기나 방어구만 가능합니다." );		
		break;
	case MSG_OPTION_ADD_ERROR_ENABLE:		// 옵션을 붙일 수 있는 아이템이 아니다
		strMessage = _S( 521, "옵션을 붙일 수 있는 아이템이 아닙니다." );		
		break;
	case MSG_OPTION_ADD_ERROR_MAX:			// 최대 옵션 수 초과
		strMessage = _S( 522,  "최대 옵션 수를 초과 하였습니다." );		
		break;
	case MSG_OPTION_ADD_SUCCESS:				// 옵션 붙이기 성공
		strMessage = _S( 523, "옵션 붙이기에 성공하였습니다." );		
		break;
	case MSG_OPTION_BLOOD_SWEAT_FRUIT_ADD_SUCCESS:
		strMessage = _S(3535, "[유료아이템]피와 땀의 결정을 사용하여 블러드 옵션 붙이기에 성공하였습니다." );		
		break;

	}
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 515, "블러드 옵션" ), UMBS_OK,		
								UI_INVENTORY, MSGCMD_INVEN_OPTION_ADD_REP );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	
	// Unlock inventory
	Lock( FALSE, FALSE, LOCK_BLOOD_OPTION  );
}

// ----------------------------------------------------------------------------
// Name : OptionDelRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::OptionDelRep( SBYTE sbResult )
{
	// Create message box
	_pUIMgr->CloseMessageBox( MSGCMD_INVEN_OPTION_DEL_REP );
	
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_OPTION_DEL_ERROR_SYSTEM:		// 있어서는 안되는 오류
		strMessage = _S( 524, "옵션 지우기에 실패 하였습니다." );		
		break;
	case MSG_OPTION_DEL_ERROR_WEAR:			// 착용한 아이템어야지
		strMessage = _S( 525, "착용한 아이템이어야 합니다." );		
		break;
	case MSG_OPTION_DEL_ERROR_KIND:			// 무기나 방어구만 가능하다
		strMessage = _S( 520, "무기나 방어구만 가능합니다." );		
		break;
	case MSG_OPTION_DEL_ERROR_NUM:			// 옵션3개 이상 있어야 지울수 있다
		strMessage = _S( 526, "옵션이 3개 이상 있어야 옵션을 지울 수 있습니다." );		
		break;
	case MSG_OPTION_DEL_SUCCESS:				// 옵션 지우기 성공
		strMessage = _S( 527, "옵션 지우기 성공하였습니다." );		
		break;
	}
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 175, "아이템 업그레이드" ), UMBS_OK,
								UI_INVENTORY, MSGCMD_INVEN_OPTION_DEL_REP );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	
	// Unlock inventory
	Lock( FALSE, FALSE, LOCK_BLOOD_OPTION  );
}

// ----------------------------------------------------------------------------
// Name : ClearAllItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIInventory::ClearAllItems( SBYTE sbTab )
{
	if( sbTab == ITEM_TAB_NORMAL )
	{
		// Initialize wearing items
		for( int iWear = 0; iWear < WEAR_TOTAL; iWear++ )
		{
			m_abtnWearing[iWear].InitBtn();
		}
	}

	// Initialize slot items
	for( int iRow = 0; iRow < INVEN_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < INVEN_SLOT_COL; iCol++ )
		{
			m_abtnItems[sbTab][iRow][iCol].InitBtn();
		}
	}

	// Reset scroll bar
/*	m_aItemFirstRow[sbTab] = 0;
	if( m_nCurItemTab == sbTab )
		m_sbScrollBar.SetScrollPos( 0 );*/
}

// ----------------------------------------------------------------------------
// Name : GetUseItemInfo() wooss 050806
// Desc : 현재 사용하려는 아이템의 정보를 얻는다
// ----------------------------------------------------------------------------

void CUIInventory::GetUseItemSlotInfo(int &nTab,int&nRow,int&nCol)
{
	nTab=((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;
	nRow=m_row;
	nCol=m_col;
}
void CUIInventory::SetUseItemSlotInfo(int nTab,int nRow,int nCol)
{
	m_InvenCurrentTab = static_cast<InventoryTab>((nTab>0) ? (nTab+INVEN_TAB_NORMAL4) : (nRow/INVEN_SLOT_ROW));
	m_row = nRow;		
	m_col = nCol;
}


//------------------------------------------------------------------------------
// CUIInventory::StartSkillDelay
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIInventory::StartSkillDelay( int nIndex )
{
	int nTab=((m_InvenCurrentTab-INVEN_TAB_NORMAL4) > 0) ? (m_InvenCurrentTab-INVEN_TAB_NORMAL4) : 0;

	CItems		&tv_rItems = _pNetwork->MySlotItem[nTab][m_row][m_col];

	for(int tab=0; tab<INVEN_SLOT_TAB; ++tab)
	{
		for(int row=0; row<INVEN_SLOT_ROW_TOTAL; ++row)
		{
			for(int col=0; col<INVEN_SLOT_COL; ++col)
			{
				if( m_abtnItems[tab][row][col].GetBtnType() == UBET_ITEM )
				{
					int nItemIndex = m_abtnItems[tab][row][col].GetItemIndex();
					if( nItemIndex == -1 ) continue;

					CItems	  &rItems = _pNetwork->MySlotItem[tab][row][col];

					if( tv_rItems.Item_UniIndex == rItems.Item_UniIndex )
					{
						if( m_abtnItems[tab][row][col].GetSkillDelay() )
						{
							return FALSE;
						}
						else
						{
							m_abtnItems[tab][row][col].SetSkillDelay( TRUE );
							return TRUE;
						}
					}
					
				}
			}
		}
	}
	return FALSE;
}

BOOL CUIInventory::GetSkillDelay( int nIndex )
{
	for(int tab=0; tab<INVEN_SLOT_TAB; ++tab)
	{
		for(int row=0; row<INVEN_SLOT_ROW_TOTAL; ++row)
		{
			for(int col=0; col<INVEN_SLOT_COL; ++col)
			{
				if( m_abtnItems[tab][row][col].GetBtnType() == UBET_ITEM )
				{
					int nItemIndex = m_abtnItems[tab][row][col].GetItemIndex();
					
					if( nItemIndex == nIndex )
					{
						if( m_abtnItems[tab][row][col].GetSkillDelay() )
						{
							return TRUE;
						}
						else FALSE;
						
					}
					
				}
			}
		}
	}
	return FALSE;
}


//------------------------------------------------------------------------------
// CUIInventory::GetItemIndex
// Explain: 인벤토리에서 아이템의 Index를 얻어오기 위한 함수 
// Date : 2005-11-02,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
LONG CUIInventory::GetItemIndex( int nTab, int nRow, int nCol )
{
	return m_abtnItems[nTab][nRow][nCol].GetItemIndex();
}

// [070824: Su-won] REFINE_PROTECT
SLONG CUIInventory::GetUpgradeItemPlus()
{
	return slTempPlus;
}

INDEX CUIInventory::GetMySlotItemsCount(void)
{
	INDEX Cnt=0;

	int i, j;

	for (i=0; i<INVEN_SLOT_ROW_TOTAL; ++i)
	for (j=0; j<INVEN_SLOT_COL; ++j)
	{
		if (!_pNetwork->MySlotItem[ITEM_TAB_NORMAL][i][j].IsEmptyItem())
		{
			++Cnt;
		}
	}

	return Cnt;
}

void CUIInventory::ShowItemDestroyMessage(CUIButtonEx& abtn)
{
	if (_pUIMgr->DoesMessageBoxExist(MSGCMD_ITEMDELETE))
	{
		return;
	}

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo(_S(4202, "아이템 파괴(휴지통)"), UMBS_OKCANCEL, UI_INVENTORY, MSGCMD_ITEMDELETE);
	MsgBoxInfo.AddString(_S(4203, "아이템을 휴지통에 넣으면 파괴되어 다시 복구될 수 없습니다. 그래도 파괴 하시겠습니까?"));
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	m_abtnDestroyItem.Copy(abtn);
}

BOOL CUIInventory::FindEmptyabtnItem(int nTab, int &nRow, int &nCol)
{
	int iRow, iCol, tmpRow;

	for (iRow=0; iRow < INVEN_SLOT_ROW; ++iRow)
	for (iCol=0; iCol < INVEN_SLOT_COL; ++iCol)
	{
		tmpRow = iRow + (nTab*INVEN_SLOT_ROW);

		if (m_abtnItems[0][tmpRow][iCol].IsEmpty())
		{
			nRow = tmpRow;
			nCol = iCol;
			return TRUE;
		}
	}

	return FALSE;
}
void CUIInventory::SetPetItem(int nTab, int nRow, int nCol)
{
	m_abtnItems[nTab][nRow][nCol].SetPetDataInfo(_pNetwork->MySlotItem[nTab][nRow][nCol].Item_PetInfo);
}

// ===================================================================
//  [6/8/2009 selo]
// Name : IsRestritionItem()
// Desc : 퀘스트 아이템 사용제한 체크
//		  해당 위치의 아이템이 퀘스트에 의해 사용제한 된 아이템인지 확인
// ===================================================================
BOOL CUIInventory::IsRestritionItem(int nTab, int nRow, int nCol)
{
	CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;
	
	if( rItems.ItemData.GetFlag() & ITEM_FLAG_QUEST )		
	{
		int	nZoneNo = rItemData.GetRestrictionZoneNo();

		if( nZoneNo == _pNetwork->MyCharacterInfo.zoneNo )
		{
			FLOAT3D		restrictionPos;
			FLOAT		fRadius = (FLOAT)rItemData.GetRestrictionRadius();
			if( 0 == fRadius )
				fRadius = 20.0f;

			restrictionPos(1) = (FLOAT)rItemData.GetRestrictionPosX();
			restrictionPos(2) = (FLOAT)rItemData.GetRestrictionPosY();
			restrictionPos(3) = (FLOAT)rItemData.GetRestrictionPosZ();

			FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - restrictionPos(1);
			FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - restrictionPos(3);
			if( fDiffX * fDiffX + fDiffZ * fDiffZ > fRadius )
				return TRUE;
		}
		else
			return TRUE;
	}
	
	return FALSE;
}