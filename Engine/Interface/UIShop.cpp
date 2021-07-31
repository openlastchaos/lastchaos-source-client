#include "stdh.h"
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/network/MobTarget.h>
#include <Engine/Entities/Items.h>


extern INDEX g_iCountry;
#define ENABLE_GUILD_CUBE

// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;

enum eSelection
{
	SHOP_BUY,		// 상점 구매
	SHOP_SELL,		// 상점 판매
	SHOP_TALK,		// 이야기 하기
	SHOP_EVENT,		// 이벤트.
	SHOP_GW_MIX,	// 공성 조합 
	SHOP_SUPERGOJE,	// 초고급 제련석 받기, 특정 NPC 하드코딩

#ifdef HELP_SYSTEM_1
	SHOP_NPC_HELP,		//NPC 안내 시스템
#endif

	LEASE_WEAPON_TITAN,			// 무기 대여상
	LEASE_WEAPON_KNIGHT,
	LEASE_WEAPON_HEALER,
	LEASE_WEAPON_MAGE,
	LEASE_WEAPON_ROGUE,
	LEASE_WEAPON_SOCERER,

	//ttos: 몬스터 콤보
	COMBO_MAKE,
	COMBO_MOVE_LOBBY,
	COMBO_MOVE_STAGE,
	COMBO_MOVE_GO_LOBBY,
	COMBO_RECALL_LOBBY,
	COMBO_RECALL_STAGE,
	COMBO_GIVEUP,
	COMBO_COMPLETE,
	COMBO_START,

	//Cube
	CUBE_PARTY_ENTRANCE,
	CUBE_PARTY_GROUP_ENTRANCE,
	CUBE_PARTY_PERSON_ENTRANCE,
	CUBE_GUILD_GROUP_ENTRANCE,
	CUBE_GUILD_PERSON_ENTRANCE,
	CUBE_GUILD_ENTRANCE,
	CUBE_PLAYER_STATE,
	CUBE_GUILD_STATE,
};

// ----------------------------------------------------------------------------
// Name : CUIShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIShop::CUIShop()
{
	m_bBuyShop			= FALSE;
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelectedShopID	= -1;
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_bShowItemInfo		= FALSE;
	m_bDetailItemInfo	= FALSE;
	m_nCurInfoLines		= 0;
	m_strPlayerMoney	= CTString( "0" );
	m_strTotalPrice		= CTString( "0" );
	m_bIsLease			= FALSE;
	m_bIsEvent			= FALSE; //ttos : 2007 가정의 달 이벤트
	m_iEventjob			= -1;
	m_bIsFieldShop		= FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIShop::~CUIShop()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	//m_nBackSplitHeight = 50;
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	// Inventory region
	m_rcBuyShopSlot.SetRect( 4, 24, 190, 171 );
	m_rcBuyTradeSlot.SetRect( 4, 175, 190, 270 );
	m_rcSellShopSlot.SetRect( 4, 122, 190, 271 );
	m_rcSellTradeSlot.SetRect( 4, 24, 190, 118 );

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 29, 216, 62, fTexWidth, fTexHeight );

	// Inventory
	m_rtTopInven.SetUV( 0, 131, 216, 282, fTexWidth, fTexHeight );
	m_rtBottomInven.SetUV( 0, 283, 216, 381, fTexWidth, fTexHeight );
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );

	// for lease 
	m_rcBottom1.SetRect(4,175,190,270);
	m_rcBottom2.SetRect(4,270,190,291);
	m_rtBottom1.SetUV(0,284,216,286, fTexWidth, fTexHeight );
	m_rtBottom2.SetUV(0,358,216,381, fTexWidth, fTexHeight );
	m_rtBottom3.SetUV(26,42,97,48, fTexWidth, fTexHeight );
	m_rtleaseMark.SetUV(216,148,231,163,fTexWidth, fTexHeight );

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

	// Buy button of shop
	m_btnShopBuy.Create( this, _S( 253, "구매" ), 105, 277, 51, 21 );
	m_btnShopBuy.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Sell button of shop
	m_btnShopSell.Create( this, _S( 254, "판매" ), 105, 277, 51, 21 );
	m_btnShopSell.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopSell.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopSell.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopSell.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	m_btnShopCancel.Create( this, _S( 139, "취소" ), 159, 277, 51, 21 );
	m_btnShopCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Event_Weapon button of shop
	m_btnEvent_weapon.Create( this, _S( 2375, "무기" ), 24, 328, 51, 21 );
	m_btnEvent_weapon.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnEvent_weapon.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnEvent_weapon.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEvent_weapon.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Event_Shild button of shop
	m_btnEvent_shield.Create( this, _S( 2376, "방어구" ), 141, 328, 51, 21 );
	m_btnEvent_shield.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnEvent_shield.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnEvent_shield.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEvent_shield.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Top scroll bar
	m_sbTopScrollBar.Create( this, 194, 28, 9, 137 );
	m_sbTopScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbTopScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollRange( SHOP_SHOP_SLOT_ROW_TOTAL );
	m_sbTopScrollBar.SetCurItemCount( SHOP_SHOP_SLOT_ROW_TOTAL );
	m_sbTopScrollBar.SetItemsPerPage( SHOP_SHOP_SLOT_ROW );
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
	m_sbTopScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbTopScrollBar.SetWheelRect( -192, -6, 191, 148 );
	
	// Bottom scroll bar
	m_sbBottomScrollBar.Create( this, 194, 128, 9, 137 );
	m_sbBottomScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10);
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbBottomScrollBar.SetScrollRange( SHOP_SHOP_SLOT_ROW_TOTAL );
	m_sbBottomScrollBar.SetCurItemCount( SHOP_SHOP_SLOT_ROW_TOTAL );
	m_sbBottomScrollBar.SetItemsPerPage( SHOP_SHOP_SLOT_ROW );
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
	for( int iRow = 0; iRow < SHOP_SHOP_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
		{			
			m_abtnShopItems[iRow][iCol].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SHOP, UBET_ITEM, 0, iRow, iCol );
		}
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		m_abtnTradeItems[iItem].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SHOP, UBET_ITEM );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::OpenShop( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
		return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	ResetShop();

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData& MD = _pNetwork->GetMobData(iMobIndex);
	CShopData &SD = _pNetwork->GetShopData(iMobIndex);	
	//_pUIMgr->GetShop()->OpenShop( SD.GetIndex(), bHasQuest );
	const int iShopID = SD.GetIndex();


	switch( _pNetwork->MyCharacterInfo.sbJoinFlagMerac )
	{
		// 수성길드
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		{
			if( iMobIndex == 234 || iMobIndex == 235 ) // 공성측 상인이면 종료
				return;
		}
		break;
		
		// 공성측 길드
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		{
			if( iMobIndex == 232 || iMobIndex == 233 ) // 수성측 상인이면 종료 
				return;
		}
		break;
	}

	ASSERT(iShopID != -1 && "Invalid Shop ID");
	m_nSelectedShopID = iShopID;

	// Character state flags
	_pUIMgr->SetCSFlagOn( CSF_SHOP );

	_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

	// Create refine message box
	_pUIMgr->CreateMessageBoxL( _S( 263, "상점" ) , UI_SHOP, MSGLCMD_SHOP_REQ );

	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( iMobIndex == 272 ) // 무기 대여상
	{
		CTString tv_str;
		tv_str = _S(3054,"험난하고 위험한 아이리스를 여행하려면 이정도 무기 하나쯤은 가지고 계셔야 할 겁니다.");
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3055, "무기가 필요하시다고요? 파는건 아니지만 좋은 무기를 며칠간 빌려드릴 순 있는데 어떠세요?" ) , -1, 0xA3A1A3FF );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str = _S(3056,"대여해 드리는 무기는 +6으로 제련된 것으로 더 이상은 제련을 못하며 1일간 사용할 수 있습니다.");
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3057, "무기 종류를 골라보세요." ) , -1, 0xA3A1A3FF );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3058,"타이탄 무기류" ), LEASE_WEAPON_TITAN );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3059,"나이트 무기류" ), LEASE_WEAPON_KNIGHT  );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3060,"힐러 무기류" ), LEASE_WEAPON_HEALER  );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3061,"메이지 무기류" ), LEASE_WEAPON_MAGE  );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3062,"로그 무기류" ), LEASE_WEAPON_ROGUE  );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3063,"소서러 무기류" ), LEASE_WEAPON_SOCERER  );
		
		//대여상점으로...
		m_bIsLease = TRUE;
		SetHeight(m_nHeight+ 77);
		m_btnShopBuy.SetText(_S(3046,"대여"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()+77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()+77);

	}
	//------ 몬스터 콤보 NPC
	else if(iMobIndex == 488)
	{

		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4039, "몬스터 콤보 만들기" ), COMBO_MAKE );
	//	_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _s( "대기방 이동" ), COMBO_MOVE_LOBBY );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "상점 이용하기" ), SHOP_BUY );			
		
	}
	else if(iMobIndex == 489)
	{

		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4041, "몬스터 콤보 스테이지 이동" ), COMBO_MOVE_STAGE );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4042, "파티원 소환하기" ), COMBO_RECALL_LOBBY );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4043, "몬스터 콤보 나가기" ), COMBO_GIVEUP );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "상점 이용하기" ), SHOP_BUY );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "취소한다." ) );
		return;
		
	}
	else if(iMobIndex == 490)
	{

		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		if(_pUIMgr->GetCombo()->GetComboClear() == 1)
		{	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4044, "다음 스테이지로 이동" ), COMBO_MOVE_STAGE );
		}

		if(_pUIMgr->GetCombo()->GetComboClear() == 2)
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4045, "콤보 완료 필드로 돌아가기" ), COMBO_COMPLETE );
		}else{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4046, "몬스터 콤보 대기방 이동" ), COMBO_MOVE_GO_LOBBY );
		}
		
		return;
				
	}
	else if(iMobIndex >= 521 && iMobIndex <= 525)			//Cube NPCs
	{
		if( _pUIMgr->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			_pUIMgr->CloseMessageBoxL(MSGLCMD_CUBE_PARTY_ENTRANCE);

		if( _pUIMgr->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			_pUIMgr->CloseMessageBoxL(MSGLCMD_CUBE_GUILD_ENTRANCE);	
		
		if( _pUIMgr->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
			_pUIMgr->CloseMessageBox(MSGCMD_CUBE_STATE);
			
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4321, "이 큐브에 들어가기 위해서는 입장권이 필요합니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4322, "꼭 파티를 해서 들어가야 합니다. 파티에 속해 있지 않으면 들어갈 수 없습니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4323, "단체 입장을 하게 되면 파티원이 강제로 이동하게 됩니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4324, "그럼 행운을 빕니다."),-1,0xa3a1a3ff);
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4325, "입장큐브 구입" ), SHOP_BUY );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4326, "파티큐브입장 신청" ), CUBE_PARTY_ENTRANCE);
#ifdef ENABLE_GUILD_CUBE
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4394, "길드큐브입장 신청" ), CUBE_GUILD_ENTRANCE);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4390, "개인 큐브 포인트 현황" ), CUBE_PLAYER_STATE);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4388, "길드 큐브 포인트 현황" ), CUBE_GUILD_STATE);
#endif
	}
	else
	{
		if( iMobIndex == 233 || iMobIndex == 235 ) // 공성 조합상인
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1975, "전장에서 장사를 한다는 건 목숨을 내걸고 하는 일이지." ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1976, "그만큼 힘든 일 이기도 하고 그렇다고 바가지를 씌우거나 하지는 않네..." ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1977, "진귀한 물건들이 많이 있으니 골라보게.." ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1978, "난 조합 아이템만 취급하니 염두해 두게. 물론 아이템 조합은 서비스로 해주고 있지. " ) , -1, 0xA3A1A3FF );	
		}
		else
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 621, "어서 오시게." ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 622, "이 근방에서는 그래도 여기가 제일 가격도 싸고 종류도 다양 할 꺼야." ) , -1, 0xA3A1A3FF );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 623, "머, 영 고를 물건이 없다면 할 수 없지만 다른 곳을 가도 마찬가지야." ) , -1, 0xA3A1A3FF );	
		}


		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 624, "구매한다." ), SHOP_BUY  );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 625, "판매한다." ), SHOP_SELL  );		
	}
	
	if( iMobIndex == 233 || iMobIndex == 235 ) // 공성 조합상인
	{	
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1979, "아이템을 조합한다." ), SHOP_GW_MIX );	
	}

	if(bHasQuest)
	{
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SHOP_REQ);				
#else		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1053, "이야기한다." ), SHOP_TALK  );		
#endif
	}

	// FIXME : 퀘스트가 없을 경우에 문제가 됨.
	// FIXME : 고로, 이벤트 NPC는 되도록 퀘스트를 갖고 있는 형태로???
	if(MD.IsEvent())
	{
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 100, "이벤트" ), SHOP_EVENT  );		
	}

	if(iMobIndex == 226)
	{
		_pUIMgr->CloseMessageBox( MSGCMD_SUPERGOJE_REQUEST );
		_pUIMgr->CloseMessageBox( MSGCMD_SUPERGOJE_NOTIFY );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1782, "초 고급 제련석 받기" ), SHOP_SUPERGOJE  );
	}
#ifdef HELP_SYSTEM_1
	_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC 안내" ), SHOP_NPC_HELP); //ttos : 안내시스템 추가시
#endif
	_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "취소한다." ) );	
}

// ttos : 가정의 달 이벤트(어린이날) 추가
void CUIShop::EventOpenShop( int iMobIndex, BOOL bHasQuest,FLOAT fX, FLOAT fZ )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
		return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	ResetShop();

	_pUIMgr->SetCSFlagOn( CSF_SHOP );

	_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

	_pUIMgr->CreateMessageBoxL(_S(169,"이벤트"), UI_SHOP, MSGLCMD_SHOP_REQ ); // 랜디 이벤트 타이틀바
	
	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( iMobIndex == 254 /*&& IS_EVENT_ON(TEVENT_MAY)*/)
	{
		CTString tv_str;
		tv_str = _S(3401, "아직 많은 모험을 하기에는 장비가 부족한 것 같은데 제가 +3장비들을 선물로 드릴 수 있는데요 절 좀 도와주실래요?");
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str = _S(3402, "제가 요즘 너무 바빠서 좋아하는 노란 개나리꽃을 구해다 주시면 제가 장비를 드릴께요 한번 받은 장비는 또 다시 주어지지 않습니다.");
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str.PrintF("%s %s",_S(43,"타이탄"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_TITAN );
		tv_str.PrintF("%s %s",_S(44,"기사"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_KNIGHT  );
		tv_str.PrintF("%s %s",_S(45,"힐러"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_HEALER  );
		tv_str.PrintF("%s %s",_S(46,"메이지"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_MAGE  );
		tv_str.PrintF("%s %s",_S(47,"로그"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_ROGUE  );
		tv_str.PrintF("%s %s",_S(48,"소서러"),_S(2736,"장비 아이템"));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_SOCERER  );
		
		//대여상점과 가정의 달 이벤트로...
		m_bIsLease = TRUE;
		m_bIsEvent = TRUE;
		SetHeight(m_nHeight+ 77);
		m_btnShopBuy.SetText(_S(191,"확인"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()+77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()+77);

	}
#ifdef HELP_SYSTEM_1
	_pUIMgr->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC 안내" ), SHOP_NPC_HELP); //ttos : 안내시스템 추가시
#endif
	_pUIMgr->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "취소한다." ) );

}

//---------------------------------------------------------------------->>
// Name : PrepareLeaseShop()
// Desc : 
// Date : [6/29/2006] , Wooss
//----------------------------------------------------------------------<<
void CUIShop::PrepareLeaseShop(int jobIdx, int iType)
{
	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	if(!m_bIsEvent)
		_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcBuyShopSlot );
	m_rcBottom.CopyRect( m_rcBuyTradeSlot );

	m_bBuyShop			= TRUE;

	int	ctShopItems;
	if(m_bIsEvent) // ttos : 가정의 달 이벤트
	{
		ClearItems();
		m_iEventjob = jobIdx;
		ctShopItems = _pNetwork->RefreshEventItem( m_iEventjob , iType); // ttos : 이벤트 아이템 적용
	}else
	{
		ctShopItems = _pNetwork->RefreshLeaseItem( jobIdx ); // 대여 아이템 적용
	}
	
	
	int	nScrollItem = ( ctShopItems + SHOP_SHOP_SLOT_COL - 1 ) / SHOP_SHOP_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade itemstest5	
	nX = SHOP_BUY_BOTTOM_SLOT_SX;
	nY = SHOP_BUY_BOTTOM_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_BUY_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::PrepareBuyShop()
{
	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcBuyShopSlot );
	m_rcBottom.CopyRect( m_rcBuyTradeSlot );

	m_bBuyShop			= TRUE;
	int	ctShopItems = _pNetwork->RefreshShopItem( m_nSelectedShopID );
	int	nScrollItem = ( ctShopItems + SHOP_SHOP_SLOT_COL - 1 ) / SHOP_SHOP_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade itemstest5	
	nX = SHOP_BUY_BOTTOM_SLOT_SX;
	nY = SHOP_BUY_BOTTOM_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_BUY_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );
	
#ifdef HELP_SYSTEM_1
	if(g_iShowHelp1Icon)
	{
		_pUIMgr->GetHelp3()->ClearHelpString();
		_pUIMgr->GetHelp3()->AddHelpString(_S(3301, "상단의 상품 중 구입의사가 있는 상품을 더블 클릭하거나 하단의 빈칸에 드래그 한 후 구매 버튼을 클릭하면 구매가 이루어 집니다."));
		_pUIMgr->GetHelp3()->AddHelpString(_S(3302, "※ 최대 10개의 아이템을 한번에 구매할 수 있습니다."));
		_pUIMgr->GetHelp3()->AddHelpString(_S(3303, "※ 아이템 구매 시 자동으로 표시 된 나스가 지불되고 구입한 아이템은 캐릭터의 인벤토리로 들어옵니다."));
		_pUIMgr->GetHelp3()->OpenHelp(this);
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::PrepareSellShop()
{
	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcSellShopSlot );
	m_rcBottom.CopyRect( m_rcSellTradeSlot );

	m_bBuyShop			= FALSE;
	RefreshPlayerItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		_pUIMgr->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade items
	nX = SHOP_SELL_TOP_SLOT_SX;
	nY = SHOP_SELL_TOP_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_SELL_BOTTOM_SLOT_SX;
		}
		m_abtnTradeItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );

#ifdef HELP_SYSTEM_1
// [KH_07044] 3차 도움말 관련 추가
	if(g_iShowHelp1Icon)
	{
		_pUIMgr->GetHelp3()->ClearHelpString();
		_pUIMgr->GetHelp3()->AddHelpString(_S(3304, "하단의 내 인벤토리에서 상점에 판매하려는 물품을 더블클릭 하거나 상단의 빈칸에 드래그 한 후 판매 버튼을 클릭하면 판매가 이루어 집니다."));
		_pUIMgr->GetHelp3()->AddHelpString(_S(3305, "※ 최대 10개의 아이템을 한번에 판매할 수 있습니다."));
		_pUIMgr->GetHelp3()->AddHelpString(_S(3306, "※ 흰색테두리가 둘러진 아이템은 상인에게 판매할 수 없는 아이템입니다."));
		_pUIMgr->GetHelp3()->AddHelpString(_S(3307, "※ 한 번 판매한 아이템은 다시 복구되지 않으니 주의하시기 바랍니다."));
		_pUIMgr->GetHelp3()->OpenHelp(this);
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::RefreshPlayerItem()
{
	int		iRow, iCol;
	for( iRow = 0; iRow < SHOP_SHOP_SLOT_ROW_TOTAL; iRow++ )
	{
		for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
		{
			CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			if( rItems.Item_Sum > 0 )
			{
				m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, rItems.Item_Index,
															rItems.Item_UniIndex, rItems.Item_Wearing );
				m_abtnShopItems[iRow][iCol].SetItemPlus( rItems.Item_Plus );
				m_abtnShopItems[iRow][iCol].SetItemCount( rItems.Item_Sum );

				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
				{
					m_abtnShopItems[iRow][iCol].SetItemOptionData( sbOption,
																	rItems.GetOptionType( sbOption ),
																	rItems.GetOptionLevel( sbOption ) );
				}
			}
		}
	}
}

// WSS_SHOP_BUGFIX 070530 -------------------------------------->>
void CUIShop::CloseShop()
{
	_pUIMgr->RearrangeOrder( UI_SHOP, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SHOP );

	// Close message box of shop
	_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
	_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
}
// -------------------------------------------------------------<<

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::ResetShop()
{
	m_nSelShopItemID = -1;
	m_nSelTradeItemID = -1;
	m_nSelectedShopID = -1;
	m_strTotalPrice = CTString( "0" );
	m_strPlayerMoney = CTString( "0" );
	m_nTotalPrice = 0;
	m_nNumOfItem = 0;
	
	ClearItems();	
	
	m_nCurRow = 0;
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollPos( 0 );

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_SHOP );

	// Lease shop 
	if(m_bIsLease)
	{
		m_bIsLease	= FALSE;
		m_bIsEvent	= FALSE; // ttos : 2007 가정의 달 이벤트
		SetHeight(m_nHeight - 77);
		m_btnShopBuy.SetText(_S(253,"구매"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()-77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()-77);
	}

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::ClearItems()
{
	for(int i = 0; i < SHOP_TRADE_SLOT_TOTAL; i++)
		m_abtnTradeItems[i].InitBtn();			

	for(i = 0; i < SHOP_SHOP_SLOT_ROW_TOTAL; i++)
	{
		for(int j = 0; j < SHOP_SHOP_SLOT_COL; j++)
		{
			m_abtnShopItems[i][j].InitBtn();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
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
BOOL CUIShop::GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight, int nTradeItem, int nRow, int nCol )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex;
	SQUAD		llCount;
	ULONG		ulItemPlus, ulFlag;
	SBYTE		sbOptionType[MAX_ITEM_OPTION], sbOptionLevel[MAX_ITEM_OPTION];

	switch( nWhichSlot )
	{
	case 0:
		{
			nIndex		= m_abtnTradeItems[nTradeItem].GetItemIndex();
			llCount		= m_abtnTradeItems[nTradeItem].GetItemCount();
			ulItemPlus	= m_abtnTradeItems[nTradeItem].GetItemPlus();
			ulFlag		= m_abtnTradeItems[nTradeItem].GetItemFlag();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
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
			ulFlag		= m_abtnShopItems[nRow][nCol].GetItemFlag();

			for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnShopItems[nRow][nCol].GetItemOptionType( sbOption );
				sbOptionLevel[sbOption] = m_abtnShopItems[nRow][nCol].GetItemOptionLevel( sbOption );
			}
		}
		break;
	}
	
	if( nIndex < 0 )
		return FALSE;

	CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( nIndex );

	COLOR colNas = 0xF2F2F2FF;

	// Get item name
	if( !m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) )
	{
		CTString	strCount;
		
		strCount.PrintF( "%I64d", llCount );
		_pUIMgr->InsertCommaToString( strCount );		
		strTemp.PrintF( "%s(%s)", szItemName, strCount );
		colNas = _pUIMgr->GetNasColor( llCount );
	}
	else
	{
		if( !m_bBuyShop && ulItemPlus > 0 )
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
			if( nWhichSlot == 0 && rItemData.GetFlag() & ITEM_FLAG_COUNT )
			{
				CTString	strCount;
				strCount.PrintF( "%I64d", llCount );
				_pUIMgr->InsertCommaToString( strCount );
				strTemp.PrintF( "%s(%s)", szItemName, strCount );
				colNas = _pUIMgr->GetNasColor( llCount );
			}
			else
			{
				strTemp = szItemName;
			}
		}		
	}
	
	AddItemInfoString( strTemp, colNas );

	const __int64	iPrice = CalculateItemPrice( m_nSelectedShopID, rItemData, 1, m_bBuyShop );

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		// Price - except money
		if( rItemData.GetType() != CItemData::ITEM_ETC ||
			rItemData.GetSubType() != CItemData::ITEM_ETC_MONEY )
		{
			CTString	strMoney;
			strMoney.PrintF( "%I64d", iPrice );
			_pUIMgr->InsertCommaToString( strMoney );
			strTemp.PrintF( _S( 255, "가격 : %I64d" ), strMoney );
			AddItemInfoString( strTemp, _pUIMgr->GetNasColor( iPrice ) );
		}
		//AddItemInfoString( CTString( " " ) );

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

				int	nPlusValue;
				if( ulItemPlus > 0 )
				{
					// Physical attack
					if( rItemData.GetPhysicalAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetPhysicalAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), rItemData.GetPhysicalAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), rItemData.GetPhysicalAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1891, "물리 공격력 + 75" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}

					// Magic attack
					if( rItemData.GetMagicAttack() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetMagicAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetMagicAttack();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), rItemData.GetMagicAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), rItemData.GetMagicAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S(1892 , "마법 공격력 + 50" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}
				}
				else
				{
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

				int	nPlusValue;
				if( ulItemPlus > 0 )
				{
					// Physical defense
					if( rItemData.GetPhysicalDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetPhysicalDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetPhysicalDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalDefence(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), rItemData.GetPhysicalDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), rItemData.GetPhysicalDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S(1893 , "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );

							strTemp.PrintF(_S(1894 , "마법 방어력 + 50" ));			
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}

					// Magic defense
					if( rItemData.GetMagicDefence() > 0 )
					{
						//nPlusValue = (int)( rItemData.GetMagicDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - rItemData.GetMagicDefence();
						nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicDefence(), ulItemPlus );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), rItemData.GetMagicDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), rItemData.GetMagicDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S(1893 , "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );

							strTemp.PrintF(_S(1894 , "마법 방어력 + 50" ));			
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}
				}
				else
				{
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
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
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
					strTemp.PrintF( "%s : %d", odItem.GetName(), odItem.GetValue( sbOptionLevel[sbOption] - 1 ) ); 
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
		if( ItemData.GetType() != CItemData::ITEM_ETC ||
			ItemData.GetSubType() != CItemData::ITEM_ETC_MONEY )
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
void CUIShop::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nTradeItem, int nRow, int nCol )
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
			m_bDetailItemInfo = m_nSelShopItemID == ( nCol + nRow * SHOP_SHOP_SLOT_COL );;
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
// Name : RenderSellItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIShop::RenderShopItems()
{
	int iShopX, iShopY;
	if(m_bBuyShop)
	{
		iShopX = SHOP_BUY_TOP_SLOT_SX;
		iShopY = SHOP_BUY_TOP_SLOT_SY;
	}
	else
	{
		iShopX = SHOP_SELL_BOTTOM_SLOT_SX;
		iShopY = SHOP_SELL_BOTTOM_SLOT_SY;
	}

	int	iRow, iCol;
	int	nX = iShopX;
	int	nY = iShopY;
	int	iRowS = m_nCurRow;
	int	iRowE = iRowS + SHOP_SHOP_SLOT_ROW;

	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++, nX += 35 )
		{
			m_abtnShopItems[iRow][iCol].SetPos( nX, nY );

			if( m_abtnShopItems[iRow][iCol].IsEmpty() )
				continue;
			
			m_abtnShopItems[iRow][iCol].Render();

/*			CItems rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			if(m_bIsLease || rItems.IsFlag(FLAG_ITEM_LENT))
			{
				// Mark lease item
				int tv_posX,tv_posY;
				m_abtnShopItems[iRow][iCol].GetAbsPos( tv_posX, tv_posY );
				_pUIMgr->GetDrawPort()->AddTexture( tv_posX+17, tv_posY+17, tv_posX+32, tv_posY+32,
													m_rtleaseMark.U0, m_rtleaseMark.V0,
													m_rtleaseMark.U1, m_rtleaseMark.V1,
													0xFFFFFFFF );
			}
*/						
		}
		nX = iShopX;	nY += 35;
	}

	// ---Trade slot items---
	for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			continue;
			
		m_abtnTradeItems[iItem].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );


	// Set shop texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Outline of items in shop slot ( unmovable )
	if( !m_bBuyShop )
	{
		for( iRow = iRowS; iRow < iRowE; iRow++ )
		{
			for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
			{
				// If button is empty
				if( m_abtnShopItems[iRow][iCol].IsEmpty() )
					continue;

				// Not wearing, not refine stone, can trade
				int			nIndex = m_abtnShopItems[iRow][iCol].GetItemIndex();
				CItems		&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
				CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
				m_abtnShopItems[iRow][iCol].GetAbsPos( nX, nY );
				if( m_abtnShopItems[iRow][iCol].GetItemWearType() < 0 &&
					( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
					rItemData.GetFlag() & ITEM_FLAG_TRADE &&!rItems.IsFlag(FLAG_ITEM_LENT))
					continue;
				_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
													m_rtUnmovableOutline.U0, m_rtUnmovableOutline.V0,
													m_rtUnmovableOutline.U1, m_rtUnmovableOutline.V1,
													0xFFFFFFFF );
				
			}
		}
	}

	// Mark Lease Items
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++)
		{
			if( m_abtnShopItems[iRow][iCol].IsEmpty() || m_bIsEvent)
				continue;
			// Mark lease item
			CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
			if(m_bIsLease|| rItems.IsFlag(FLAG_ITEM_LENT)) //m_abtnShopItems[iRow][iCol].GetItemFlag()&FLAG_ITEM_LENT)
			{
				int tv_posX,tv_posY;
				m_abtnShopItems[iRow][iCol].GetAbsPos( tv_posX, tv_posY );
				_pUIMgr->GetDrawPort()->AddTexture( tv_posX+17, tv_posY+17, tv_posX+32, tv_posY+32,
													m_rtleaseMark.U0, m_rtleaseMark.V0,
													m_rtleaseMark.U1, m_rtleaseMark.V1,
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
	if( m_nSelShopItemID >= 0 )
	{
		int	nSelRow = m_nSelShopItemID / SHOP_SHOP_SLOT_COL;
		if( nSelRow >= iRowS && nSelRow < iRowE )
		{
			int	nSelCol = m_nSelShopItemID % SHOP_SHOP_SLOT_COL;

			m_abtnShopItems[nSelRow][nSelCol].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelectOutline.U0, m_rtSelectOutline.V0,
												m_rtSelectOutline.U1, m_rtSelectOutline.V1,
												0xFFFFFFFF );
		}
	}

	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo && !m_bIsLease)
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
void CUIShop::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		ResetShop();
		CloseShop();
	}

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
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	
	// Inventory
	if(m_bBuyShop)
	{
		// Buy
		nX = m_nPosX;
		nY = m_nPosY + 22;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_SHOP_HEIGHT,
											m_rtTopInven.U0, m_rtTopInven.V0,
											m_rtTopInven.U1, m_rtTopInven.V1,
											0xFFFFFFFF );

		nY += SHOP_SHOP_HEIGHT;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 1,
											m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
											m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
											0xFFFFFFFF );

		nY += 1;
		if(m_bIsLease)
		{
			// 대여상점 아랫부분
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_LEASE_HEIGHT+77 ,
											m_rtBottom1.U0, m_rtBottom1.V0,
											m_rtBottom1.U1, m_rtBottom1.V1,
											0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX+7, nY+4,
											nX + SHOP_SHOP_WIDTH -7, nY + SHOP_LEASE_HEIGHT+77-4,
											m_rtBottom3.U0, m_rtBottom3.V0,
											m_rtBottom3.U1, m_rtBottom3.V1,
											0xFFFFFFFF );
			nY += SHOP_LEASE_HEIGHT + 77;

			if(m_bIsEvent)
			{
				m_btnEvent_weapon.Render();
				m_btnEvent_shield.Render();

			}else{
				_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 21,
											m_rtBottom2.U0, m_rtBottom2.V0,
											m_rtBottom2.U1, m_rtBottom2.V1,
											0xFFFFFFFF );
			}
			
		}
		else 
		{
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_TRADE_HEIGHT,
											m_rtBottomInven.U0, m_rtBottomInven.V0,
											m_rtBottomInven.U1, m_rtBottomInven.V1,
											0xFFFFFFFF );			
		}
		// Bottom scroll bar		
		m_sbTopScrollBar.Render();
	}
	else
	{
		// Sell
		nX = m_nPosX;
		nY = m_nPosY + 22;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_TRADE_HEIGHT,
											m_rtBottomInven.U0, m_rtBottomInven.V0,
											m_rtBottomInven.U1, m_rtBottomInven.V1,
											0xFFFFFFFF );

		nY += SHOP_TRADE_HEIGHT;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 1,
											m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
											m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
											0xFFFFFFFF );

		nY += 1;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_SHOP_HEIGHT,
											m_rtTopInven.U0, m_rtTopInven.V0,
											m_rtTopInven.U1, m_rtTopInven.V1,
											0xFFFFFFFF );

		// Bottom scroll bar		
		m_sbBottomScrollBar.Render();		
	}

	// Close button
	m_btnClose.Render();

	// Buy & sell button of shop
	if(m_bBuyShop)
		m_btnShopBuy.Render();
	else
		m_btnShopSell.Render();
	
	// Cancel button of shop
	m_btnShopCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Render item information
	if(m_bIsEvent) // ttos : 가정의 달 이벤트
	{
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 100, "이벤트" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	}else
	{
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 263, "상점" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	}
	

	COLOR colNas,colNasP;
	colNas = _pUIMgr->GetNasColor( m_strTotalPrice );
	colNasP = _pUIMgr->GetNasColor( m_strPlayerMoney );
	
	if(m_bIsLease)
	{
		int tv_posY = m_nPosY + LEASE_INFO_TEXT_OFFSETX + 10;
		int tv_gap	= _pUIFontTexMgr->GetLineHeight() +2;
		
		CTString tv_str;
		int tv_int=0;
		COLOR tv_clr = 0xFFF377FF;

		tv_posY+= tv_gap;
		if(m_nSelShopItemID < 0)
		{
			_pUIMgr->GetDrawPort()->PutTextEx( _S(3065, "무기이름" ), m_nPosX + 10, tv_posY,tv_clr);
		}
		tv_posY+= tv_gap;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(3066, "무기레벨" ), m_nPosX + 10, tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(3067, "사용클래스" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(85, "공격력" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(86, "마법공격력" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(1070, "무게" ), m_nPosX + 10, tv_posY,tv_clr );
		tv_posY+= tv_gap;

		if(m_bIsEvent) // ttos : 가정의 달 이벤트
		{
		//	tv_str = CTString("개나리 개수 ");
			tv_str.PrintF("%s %s", _pNetwork->GetItemName(2344), _S(2396,"수량"));
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 10, tv_posY ,tv_clr);
		}else
		{
			_pUIMgr->GetDrawPort()->PutTextEx( _S(3071, "대여기간" ), m_nPosX + 10, tv_posY ,tv_clr);
		}
		tv_posY+= tv_gap;
		
		if(m_nSelShopItemID >= 0)
		{
			//WSS_SHOP_BUGFIX_70530 --->><<
			int tv_itemIdx = (m_abtnShopItems[0]+m_nSelShopItemID)->GetItemIndex();
			CItemData	&rItemData = _pNetwork->GetItemData( tv_itemIdx );

			tv_posY = m_nPosY + LEASE_INFO_TEXT_OFFSETX + 10;
			// Weapon Name
			const char* szItemName = _pNetwork->GetItemName( tv_itemIdx );
			
			if(m_bIsEvent) tv_str = CTString("+3 ");
			else tv_str = CTString("+6 ");

			tv_str +=szItemName;
			_pUIMgr->GetDrawPort()->PutTextExCX( tv_str, m_nPosX + 108, tv_posY, tv_clr);
			tv_posY+= tv_gap*2;
			
			int iExtraPos =0;
			if( g_iCountry ==THAILAND)
				iExtraPos =52;
			if( g_iCountry == MALAYSIA)
				iExtraPos = 82;

			// Weapon Level
			tv_int =rItemData.GetLevel();
			tv_str.PrintF("%d",tv_int);
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// Class 
			tv_int = rItemData.GetJob();
			switch(tv_int)
			{
			case 1:
					tv_str = _S(43,"타이탄");
					break;
			case 2:
					tv_str = _S(44,"기사");
					break;
			case 4:
					tv_str = _S(45,"힐러");
					break;
			case 8:
					tv_str = _S(46,"메이지");
					break;
			case 16:
					tv_str = _S(47,"로그");
					break;
			case 32:
					tv_str = _S(48,"소서러");
					break;
			default : 
				tv_str = CTString("Alien");
				break;
			}		
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			
			int tv_plus = m_bIsEvent ? 3 : 6 ;  // +6으로 고정
			// attack ability
			tv_int = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), tv_plus );
			if( tv_int > 0 )
				tv_str.PrintF( CTString("%d + %d" ), rItemData.GetPhysicalAttack(), tv_int );
			else
				tv_str.PrintF( CTString( "%d" ), rItemData.GetPhysicalAttack() );
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// magic attack ability
			tv_int = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), tv_plus );
			if( tv_int > 0 )
				tv_str.PrintF( CTString("%d+%d" ), rItemData.GetMagicAttack(), tv_int );
			else
				tv_str.PrintF( CTString( "%d" ), rItemData.GetMagicAttack() );
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// Weight
			tv_int =rItemData.GetWeight();
			tv_str.PrintF("%d",tv_int);
			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// Lease period
			if(m_bIsEvent) //ttos : 가정의 달 이벤트
			{
				tv_int = 1;
				tv_str.PrintF(_S(766,"%d 개"),(int)((rItemData.GetLevel()/2.0f)+0.5f));
				_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
				tv_posY+= tv_gap;

			}else
			{
				tv_int = 1;
				tv_str.PrintF(_S(3079,"%d일(게임시간%d일)"),tv_int,tv_int*24);
				_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
				tv_posY+= tv_gap;

			}
			
			if(!m_bIsEvent)
			{
				// Lease Price
				tv_int =(rItemData.GetPrice()*1.5) + 50000;
				tv_str.PrintF("%d",tv_int);
				_pUIMgr->InsertCommaToString( tv_str );
	//			_pUIMgr->GetDrawPort()->PutTextEx( tv_str, m_nPosX + 80, tv_posY);
			}
			
		}
		// Set Nas
		if(!m_bIsEvent) // ttos : 가정의 달 이벤트에 적용 되지 않음
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( tv_str, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_BUY_TRADEPRICE_POSY + 77, colNas );
		}		
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strPlayerMoney, m_nPosX + SHOP_MYMONEY_POSX, m_nPosY + SHOP_MYMONEY_POSY + 77, colNasP );
		
	}
	else
	{
		if(m_bBuyShop)
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strTotalPrice, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_BUY_TRADEPRICE_POSY, colNas );
		else
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strTotalPrice, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_SELL_TRADEPRICE_POSY, colNas );

		
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strPlayerMoney, m_nPosX + SHOP_MYMONEY_POSX, m_nPosY + SHOP_MYMONEY_POSY, colNasP );
		
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render Items
	RenderShopItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIShop::MouseMessage( MSG *pMsg )
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

			if(m_bIsLease) break;

			// Hold item button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ))
			{
				// Shop items
				if( m_nSelShopItemID >= 0 )
				{
					int	nSelRow = m_nSelShopItemID / SHOP_SHOP_SLOT_COL;
					int	nSelCol = m_nSelShopItemID % SHOP_SHOP_SLOT_COL;

					if( m_bBuyShop )
					{
						// Close message box of shop
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
						_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

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
							( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
							rItemData.GetFlag() & ITEM_FLAG_TRADE && !rItems.IsFlag(FLAG_ITEM_LENT))
						{
							// Close message box of shop
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

							_pUIMgr->SetHoldBtn( m_abtnShopItems[nSelRow][nSelCol] );
							int	nOffset = BTN_SIZE / 2;
							_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

							m_abtnShopItems[nSelRow][nSelCol].SetBtnState( UBES_IDLE );
						}
					}
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of shop
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
					_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

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
			// Sell button of shop
			else if( !m_bBuyShop && m_btnShopSell.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of shop
			else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Top Scroll bar
			else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			// Bottom Scroll bar
			else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
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
				int	iRowE = iRowS + SHOP_SHOP_SLOT_ROW;			// End Row
				int	nWhichRow = -1, nWhichCol = -1;
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
					{
						if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
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
				for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
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
				// Buy button of shop
				else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Sell button of shop
				else if( !m_bBuyShop && m_btnShopSell.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;
					m_nSelTradeItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + SHOP_SHOP_SLOT_ROW;			// 4줄~
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
						{
							if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected item
								m_nSelShopItemID = iCol + iRow * SHOP_SHOP_SLOT_COL;	// 선택된 슬롯의 아이템 ID

								// Show tool tup
								ShowItemInfo( TRUE, TRUE, -1, iRow, iCol );

								bLButtonDownInItem	= TRUE;

								_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) && !m_bIsLease )
				{
					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;

					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							bLButtonDownInItem	= TRUE;

							_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				_pUIMgr->RearrangeOrder( UI_SHOP, TRUE );
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
						ResetShop();
						//WSS_SHOP_BUGFIX_70530 --->><<
						CloseShop();
					
					}

					return WMSG_SUCCESS;
				}
				// Buy button of shop
				else if( m_bBuyShop && ( wmsgResult = m_btnShopBuy.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_bIsLease)
						{
							if(m_nSelShopItemID >= 0)
							{								
								CTString	strMessage;
								CUIMsgBox_Info	MsgBoxInfo;
								if(m_bIsEvent)
								{
									int iItemidx = (m_abtnShopItems[0]+m_nSelShopItemID)->GetItemIndex();
									CItemData &ID = _pNetwork->GetItemData(iItemidx);

									if( ID.GetJob()&(1<<_pNetwork->MyCharacterInfo.job))
									{
										MsgBoxInfo.SetMsgBoxInfo( _S(169,"이벤트"),UMBS_YESNO,UI_SHOP,MSGLCMD_EVENT_MAY);
										strMessage.PrintF(_S(3403, "%s 아이템을 선택하였습니다."),ID.GetName());

									}else
									{
										MsgBoxInfo.SetMsgBoxInfo( _S(169,"이벤트"),UMBS_NO,UI_SHOP,MSGLCMD_EVENT_MAY);
										strMessage = _S(3404, "선택하신 장비는 자신에 맞는 장비가 아닙니다. 확인하고 다시 시도하여 주십시오");
									}
									

								}else
								{
									MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_YESNO,
															UI_SHOP, MSGCMD_SHOP_LEASE_ITEM );
									strMessage = _S(3080, "대여한 무기는 대여한 날로부터 1일간의 대여기간이 만료되면 자동으로 소멸됩니다.")
										+ _S(3081,"선택하신 무기를 대여하시겠습니까?");
								}
								
								MsgBoxInfo.AddString( strMessage );
								_pUIMgr->CreateMessageBox( MsgBoxInfo );	
							}
							return WMSG_SUCCESS;							
						}
						// NOTE : 거래 슬롯에 하나라도 아이템이 있으면 구입을 하도록 하고,
						// NOTE : 없으면 메세지 자체를 안보내도록 함...
						for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
						{
							if( !m_abtnTradeItems[i].IsEmpty() )
							{
								// Send Network Message
								BuyItems();
								
								// Clear Member Value
								ResetShop();

								//WSS_SHOP_BUGFIX_70530 --->><<
								CloseShop();
								
							}
						}
						return WMSG_SUCCESS;
					}

					return WMSG_SUCCESS;
				}
				// Sell button of shop
				else if( !m_bBuyShop && ( wmsgResult = m_btnShopSell.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// NOTE : 거래 슬롯에 하나라도 아이템이 있으면 구입을 하도록 하고,
						// NOTE : 없으면 메세지 자체를 안보내도록 함...
						for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
						{
							if( !m_abtnTradeItems[i].IsEmpty() )
							{
								// Send Network Message
								SellItems();
								
								// Clear Member Value
								ResetShop();

								//WSS_SHOP_BUGFIX_70530 --->><<
								CloseShop();

								return WMSG_SUCCESS;
							}
						}
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of shop
				else if( ( wmsgResult = m_btnShopCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetShop();
						//WSS_SHOP_BUGFIX_70530 --->><<
						CloseShop();
					}

					return WMSG_SUCCESS;
				}
				// Event button of change weapon
				else if(m_bIsEvent && m_btnEvent_weapon.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PrepareLeaseShop(m_iEventjob, CItemData::ITEM_WEAPON);
					return WMSG_SUCCESS;
				}
				// Event button of change shild
				else if(m_bIsEvent && m_btnEvent_shield.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PrepareLeaseShop( m_iEventjob, CItemData::ITEM_SHIELD);
					return WMSG_SUCCESS;
				}
				// Top Scroll bar
				else if( m_bBuyShop && m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
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
					int	iRowE = iRowS + SHOP_SHOP_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
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
				else if( IsInsideRect( nX, nY, m_rcBottom ) &&!m_bIsLease)
				{
					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
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
					// If holding button is item and is from shop
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_SHOP )
					{
						// Trade items
						if( IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from shop slot
							if( m_nSelTradeItemID < 0 ||
								m_abtnTradeItems[m_nSelTradeItemID].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								AddShopItem( _pUIMgr->GetHoldBtn().GetItemRow(),
												_pUIMgr->GetHoldBtn().GetItemCol(),
												_pUIMgr->GetHoldBtn().GetItemUniIndex(),
												_pUIMgr->GetHoldBtn().GetItemCount() );

								// Reset holding button
								_pUIMgr->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Shop items
						else if( IsInsideRect( nX, nY, m_rcTop ) )
						{
							int	nSelRow = m_nSelShopItemID / SHOP_SHOP_SLOT_COL;
							int	nSelCol = m_nSelShopItemID % SHOP_SHOP_SLOT_COL;
							// If this item is moved from trade slot
							if( m_nSelShopItemID < 0 ||
								m_abtnShopItems[nSelRow][nSelCol].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
							{
								DelShopItem( _pUIMgr->GetHoldBtn().GetItemRow(),
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
			if(m_bIsLease) 
				return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				// Trade items
				if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	nTradeItemID = m_nSelTradeItemID;
					m_nSelTradeItemID = -1;

					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnTradeItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Close message box of shop
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
							_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

							DelShopItem( m_abtnTradeItems[iItem].GetItemRow(),
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
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;

					int	iRow, iCol;
					int	iRowS = m_nCurRow;
					int	iRowE = iRowS + SHOP_SHOP_SLOT_ROW;
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						for( iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
						{
							if( m_abtnShopItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								if( m_bBuyShop )
								{
									// Close message box of shop
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
									_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

									AddShopItem( iRow, iCol,
													m_abtnShopItems[iRow][iCol].GetItemUniIndex(),
													m_abtnShopItems[iRow][iCol].GetItemCount() );

								}
								else
								{
									// Not wearing, not refine stone, can trade
									CItems		&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
									CItemData	&rItemData = rItems.ItemData;
									if( rItems.Item_Wearing == -1 &&
										( rItemData.GetType() != CItemData::ITEM_ETC || rItemData.GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
										rItemData.GetFlag() & ITEM_FLAG_TRADE && !rItems.IsFlag(FLAG_ITEM_LENT))
									{
										// Close message box of shop
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_DEL_ITEM );
										_pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );

										AddShopItem( iRow, iCol,
														m_abtnShopItems[iRow][iCol].GetItemUniIndex(),
														m_abtnShopItems[iRow][iCol].GetItemCount() );
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
				else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
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
				// Top Scroll bar
				if( m_bBuyShop && m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
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


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempIndex;
static int		nTempUniIndex;
static int		nTempRow, nTempCol;
static SQUAD	llTempCount;
static int		nTempTradeItemID;

// ----------------------------------------------------------------------------
// Name : AddShopItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIShop::AddShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount )
{
	nTempRow		= nRow;
	nTempCol		= nCol;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempIndex		= m_abtnShopItems[nTempRow][nTempCol].GetItemIndex();
	ULONG ulItemPlus= m_abtnShopItems[nTempRow][nTempCol].GetItemPlus();

	CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
	const char* szItemName = _pNetwork->GetItemName( nTempIndex );

	// Ask quantity
	if( ( m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !m_bBuyShop && ( rItemData.GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
									UI_SHOP, MSGCMD_SHOP_ADD_ITEM );
		//const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		if( !m_bBuyShop )
		{
			CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_ADD_ITEM );
			CTString strItemCount;
			strItemCount.PrintF( "%I64d", m_abtnShopItems[nTempRow][nTempCol].GetItemCount() );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
		}
	}
	else if( ulItemPlus > 0 )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_YESNO,
									UI_SHOP, MSGCMD_SHOP_ADD_PLUSITEM );
		//const char	*szItemName = rItemData.GetName();
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		ShopToTrade( llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIShop::DelShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nTradeItemID )
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
		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
									UI_SHOP, MSGCMD_SHOP_DEL_ITEM );
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_SHOP_DEL_ITEM );
		CTString strItemCount;
		strItemCount.PrintF( "%I64d", m_abtnTradeItems[nTempTradeItemID].GetItemCount() );
		ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
		pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
	}
	else
	{
		TradeToShop( llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::BuyItems()
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
	for( int iRow = 0; iRow < SHOP_SHOP_SLOT_ROW_TOTAL; iRow++ )
	{
		for( int iCol = 0; iCol < SHOP_SHOP_SLOT_COL; iCol++ )
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
				for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
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

	if(m_bIsLease)
	{
		if(m_bIsEvent)
		{
			int iItemidx = (m_abtnShopItems[0]+m_nSelShopItemID)->GetItemIndex();
			CItemData &ID = _pNetwork->GetItemData(iItemidx);
			_pNetwork->SendEventItem(iItemidx,(int)((ID.GetLevel()/2.0f)+0.5));
		}else
		{
			_pNetwork->SendLeaseItem( (m_abtnShopItems[0]+m_nSelShopItemID)->GetItemIndex());
		}			
	}
	else
	{
		if(m_bIsFieldShop)
		{
			_pNetwork->FieldShopBuyItem(m_nNumOfItem, m_nTotalPrice);
			m_bIsFieldShop = FALSE;
		}else
		{
			_pNetwork->BuyItem( m_nSelectedShopID, m_nNumOfItem, m_nTotalPrice );
		}
		
	}
}

// ----------------------------------------------------------------------------
// Name : SellItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::SellItems()
{
	// Sell nothing
	if( m_nTotalPrice < 0 )
		return;

	if(m_bIsFieldShop)
	{
		_pNetwork->FieldShopSellItem(m_nNumOfItem, m_nTotalPrice);
		m_bIsFieldShop = FALSE;
	}else
	{
		_pNetwork->SellItem( m_nSelectedShopID, m_nNumOfItem, m_nTotalPrice );
	}
		
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
//		cpp2angel edit (04.12.18)
// ----------------------------------------------------------------------------
void CUIShop::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_SHOP_REQ:
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
			case SHOP_BUY : //  Buy
				{
					PrepareBuyShop();
				}
				break;

			case SHOP_SELL : // Sell
				{	
					PrepareSellShop();
				}
				break;
			case SHOP_GW_MIX: // 공성 조합
				{
					_pUIMgr->GetGWMix()->OpenGWMix();
				}
				break;
			case SHOP_TALK :// Quest ...
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_SHOP );

					//TODO : NewQuestSystem
					// Open quest
					CUIQuestBook::TalkWithNPC();
				}
				break;
				// [090527: selo] 확장팩 퀘스트 수정
			case ciQuestClassifier:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_SHOP );

					// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;
			case SHOP_SUPERGOJE:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_SHOP );

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  CTString(""), UMBS_YESNO, UI_SHOP, MSGCMD_SUPERGOJE_REQUEST);
					MsgBoxInfo.AddString( _S( 1739, "초고급 제련석을 지급 받겠습니까?" ) );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				} break;

			case LEASE_WEAPON_TITAN:
			case LEASE_WEAPON_KNIGHT:
			case LEASE_WEAPON_HEALER:
			case LEASE_WEAPON_MAGE:
			case LEASE_WEAPON_ROGUE:
			case LEASE_WEAPON_SOCERER:
				{
					int tv_job = nResult - LEASE_WEAPON_TITAN;
					PrepareLeaseShop( tv_job );
				}
				break;
#ifdef HELP_SYSTEM_1
			case SHOP_NPC_HELP:
				{
					_pUIMgr->RearrangeOrder( UI_NPCHELP, TRUE );
				}
				break;
#endif
			case COMBO_MAKE:
				{
					//콤보 편집으로 이동시 CSF_SHOP 플래그를 제거한다
					_pUIMgr->SetCSFlagOff( CSF_SHOP );
					
					_pUIMgr->GetCombo()->OpenMonsterCombo(TRUE,m_fNpcX,m_fNpcZ);
				}
				break;
			case COMBO_MOVE_LOBBY:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GOTO_COMBO);
				}
			case COMBO_MOVE_STAGE:
				{
					_pUIMgr->GetCombo()->SendComboMessage( MSG_EX_MONSTERCOMBO_GOTO_STAGE);
				}
				break;
			case COMBO_MOVE_GO_LOBBY:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GOTO_WAITROOM);

				}
				break;
			case COMBO_RECALL_LOBBY:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO);
				}
				break;
			case COMBO_RECALL_STAGE:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_STAGE);
				}
				break;
			case COMBO_GIVEUP:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_SHOP );

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  _S(4047,"몬스터 콤보"), UMBS_YESNO, UI_SHOP, MSGCMD_COMBO_GIVEUP);
					MsgBoxInfo.AddString( _S(4048, "드라탄으로 이동되며 다시 입장하기 위해서는 입장료를 지불해야 합니다. \n 그래도 몬스터 콤보를 포기하시겠습니까?" ) );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );

				//	_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP);
				}
				break;
			case COMBO_START:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_START);
				}
				break;
			case COMBO_COMPLETE:
				{
					_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP);
				}
				break;
			///////////////////////////////////////////////////////////////////////////////////
			// Cube
			case CUBE_PARTY_ENTRANCE:
				{
					_pUIMgr->CreateMessageBoxL( _S(4327, "큐브입장선택" ) , UI_SHOP, MSGLCMD_CUBE_PARTY_ENTRANCE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4328, "큐브입장방법을 선택하여 주십시오."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4329, "주의사항"),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4330, "단체입장시 파티원 모두가 강제이동됩니다."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4331, "개인입장시 조건이 맞아야 입장이 가능합니다."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4332, "단체입장을 추천드립니다."),-1,0xa3a1a3ff);
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4333, "단체입장" ), CUBE_PARTY_GROUP_ENTRANCE);
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4334, "개인입장" ), CUBE_PARTY_PERSON_ENTRANCE);
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(139, "취소" ) );
				}
				break;
			case CUBE_GUILD_ENTRANCE:
				{
					_pUIMgr->CreateMessageBoxL( _S(4327, "큐브입장선택" ) , UI_SHOP, MSGLCMD_CUBE_GUILD_ENTRANCE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4328, "큐브입장방법을 선택하여 주십시오."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4329, "주의사항"),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4335, "길드입장시 길드원 모두가 강제이동됩니다."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4331, "개인입장시 조건이 맞아야 입장이 가능합니다."),-1,0xa3a1a3ff);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4332, "단체입장을 추천드립니다."),-1,0xa3a1a3ff);
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4336, "길드입장" ), CUBE_GUILD_GROUP_ENTRANCE);
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4334, "개인입장" ), CUBE_GUILD_PERSON_ENTRANCE);
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(139, "취소" ) );
				}
				break;
			case CUBE_PLAYER_STATE:
			case CUBE_GUILD_STATE:
				{
					CNetworkMessage nmMessage(MSG_EXTEND);
			
					nmMessage << (LONG)MSG_EX_EXTREME_CUBE;

					if (nResult == CUBE_GUILD_STATE)
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_REQ; // 길드 큐브 현황
					else
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REQ; // 개인 큐브 현황

					_pNetwork->SendToServerNew(nmMessage);
				}
				break;
			// Cube
			///////////////////////////////////////////////////////////////////////////////////			
			default:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_SHOP );
				}
				break;
		
			}
		}  // case MSGLCMD_SHOP_REQ
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGLCMD_CUBE_PARTY_ENTRANCE:
		{
			switch ( nResult )
			{
			case CUBE_PARTY_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4339, "단체입장을 통해서 큐브에 입장하시겠습니까? 파티큐브가 없으면 입장할 수 없습니다.") );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_PARTY_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "개인입장을 통해서 큐브에 입장하시겠습니까?") );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	case MSGLCMD_CUBE_GUILD_ENTRANCE:
		{
		switch( nResult )
			{
			case CUBE_GUILD_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4337, "길드입장을 통해서 큐브에 입장하시겠습니까? 길드큐브가 없으면 입장할 수 없습니다.") );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_GUILD_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "개인입장을 통해서 큐브에 입장하시겠습니까?") );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	
	}// switch( nCommandCode )
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_ITEM:
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
				if( m_bBuyShop )
				{
					if( llCount > 0 )
						ShopToTrade( llCount );
				}
				else
				{
					if( llCount > 0 && llCount <= llTempCount )
						ShopToTrade( llCount );
				}
			}
		}
		break;

	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			ShopToTrade( 1 );
		}
		break;

	case MSGCMD_SHOP_DEL_ITEM:
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
					TradeToShop( llCount );
			}
		}
		break;
	case MSGCMD_SUPERGOJE_REQUEST:
		{
			_pNetwork->SendSuperStoneReq();			
		} break;
	case MSGCMD_SUPERGOJE_NOTIFY:
		{
		} break;

	case MSGCMD_SHOP_LEASE_ITEM :
	case MSGLCMD_EVENT_MAY:
		{
			// Send Network Message
			BuyItems();
			// Clear Member Value
			ResetShop();
			//WSS_SHOP_BUGFIX_70530 --->><<
			CloseShop();
		}
		break;
	case MSGCMD_COMBO_GIVEUP:
		{
			_pUIMgr->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP);
		}
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGCMD_CUBE_PARTY_GROUP_ENTRANCE:
		{
			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_PARTY_PERSON_ENTRANCE:
		{
			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_GROUP_ENTRANCE:
		{
			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_PERSON_ENTRANCE:
		{
			CNetworkMessage nmMessage(MSG_EXTEND);
			
			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_STATE:
		{
			_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->InitMessageBox();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIShop::ShopToTrade( SQUAD llCount )
{
	if( m_bBuyShop )
	{
		CItemData	&rItemData = _pNetwork->GetItemData( nTempIndex );
		SQUAD	llPrice = m_nTotalPrice +
							CalculateItemPrice( m_nSelectedShopID, rItemData, llCount, m_bBuyShop );

		// Not enough money
		if(_pNetwork->MyCharacterInfo.money < llPrice )
		{
			// Add system message
			_pUIMgr->GetChatting()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
			return;
		}

		const int iFame = rItemData.GetFame();

		// Not enough fame
		if( iFame > 0 && iFame > _pNetwork->MyCharacterInfo.fame )
		{
			// Add system message
			_pUIMgr->GetChatting()->AddSysMessage( _S( 1217, "명성치가 부족합니다." ), SYSMSG_ERROR );		
			return;
		}
	}

	// Find same item in trade slot
	for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnTradeItems[iItem].GetItemIndex() == nTempIndex )
			break;
	}

	// If there is same item
	if( iItem < SHOP_TRADE_SLOT_TOTAL )
	{
		// Check if item is countable
		CItemData&	rItemData = _pNetwork->GetItemData( nTempIndex );
		if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
		{
			// Update count of trade item
			SQUAD	llNewCount = m_abtnTradeItems[iItem].GetItemCount();
			llNewCount += llCount;
			m_abtnTradeItems[iItem].SetItemCount( llNewCount );
												
			if( !m_bBuyShop )
			{
				// Update count of shop item
				llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
				llNewCount -= llCount;
				m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
				if( llNewCount <= 0 )
				{
					m_abtnShopItems[nTempRow][nTempCol].SetEmpty( TRUE );

					// Unselect item
					if( m_nSelShopItemID == ( nTempCol + nTempRow * SHOP_SHOP_SLOT_COL ) )
						m_nSelShopItemID = -1;
				}
			}

			m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
			m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
			_pUIMgr->InsertCommaToString( m_strTotalPrice );
			return;
		}
	}

	// Find empty slot
	for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnTradeItems[iItem].IsEmpty() )
			break;
	}

	// If there is not empty slot
	if( iItem == SHOP_TRADE_SLOT_TOTAL )
	{
		// Add system message
		if( m_bBuyShop )
			_pUIMgr->GetChatting()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
		else
			_pUIMgr->GetChatting()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );

		return;
	}

	m_abtnTradeItems[iItem].SetItemInfo( 0, nTempRow, nTempCol, nTempIndex, nTempUniIndex, -1 );
	m_abtnTradeItems[iItem].SetItemCount( llCount );
	
	if( !m_bBuyShop )
	{												
		// Update count of shop item
		SQUAD	llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
		llNewCount -= llCount;
		m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount <= 0 )
		{
			m_abtnShopItems[nTempRow][nTempCol].SetEmpty( TRUE );

			// Unselect item
			if( m_nSelShopItemID == ( nTempCol + nTempRow * SHOP_SHOP_SLOT_COL ) )
				m_nSelShopItemID = -1;
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIShop::TradeToShop( SQUAD llCount )
{
	// Update count of trade item
	SQUAD	llNewCount = m_abtnTradeItems[nTempTradeItemID].GetItemCount();
	llNewCount -= llCount;
	m_abtnTradeItems[nTempTradeItemID].SetItemCount( llNewCount );

	if( llNewCount <= 0 )
	{
		// Rearrange buttons
		for( int iArrItem = nTempTradeItemID; iArrItem < SHOP_TRADE_SLOT_TOTAL; iArrItem++ )
		{
			if( ( iArrItem + 1 ) == SHOP_TRADE_SLOT_TOTAL )
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
		}

		// Unselect item
		if( nTempTradeItemID == m_nSelTradeItemID )
			m_nSelTradeItemID = -1;
	}

	if( !m_bBuyShop )
	{
		// Update count of shop item
		llNewCount = m_abtnShopItems[nTempRow][nTempCol].GetItemCount();
		llNewCount += llCount;
		m_abtnShopItems[nTempRow][nTempCol].SetItemCount( llNewCount );
		if( llNewCount > 0 )
			m_abtnShopItems[nTempRow][nTempCol].SetEmpty( FALSE );
	}

	m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	_pUIMgr->InsertCommaToString( m_strTotalPrice );
}


//-----------------------------------------------------------------------------
// Purpose: 가격을 계산합니다.
// Input  : iShopID - 
//			bSell - 
// Output : __int64
//-----------------------------------------------------------------------------
__int64 CUIShop::CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell)
{
	if(iShopID	<= 0)								return 0;

	__int64	iTotalPrice	= 0;
	iCount				= 0;

	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; i++ )
	{
		if( m_abtnTradeItems[i].IsEmpty() )
			continue;

		const int iIndex		= m_abtnTradeItems[i].GetItemIndex();
		SQUAD iItemCount		= m_abtnTradeItems[i].GetItemCount();
		CItemData& ID			= _pNetwork->GetItemData(iIndex);
		if( ID.GetItemIndex() == -1 )
			continue;

		iTotalPrice				+= CalculateItemPrice( iShopID, ID, iItemCount, bSell );
		iCount++;
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
__int64	CUIShop::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell)
{
	if(iShopID <= 0)
		return 0;
	
	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
	const int iShopSellRate	= SD.GetSellRate();
	const int iShopBuyRate	= SD.GetBuyRate();
	__int64	iPrice		= 0;

	if( bSell )
		//iPrice = (ID.GetPrice() * iShopSellRate) / 100;
		iPrice = (ID.GetPrice() * iShopBuyRate) / 100;
	else
	{
		//iPrice = (ID.GetPrice() * iShopBuyRate) / 100;
		iPrice = (ID.GetPrice() * iShopSellRate) / 100;

		// [090617: selo] 미국의 경우 아이템 레벨이 97이상이면 아이템의 가격을 반값으로 한다.		
		if( USA == g_iCountry )
		{
			if( 97 <= ID.GetLevel() )
				iPrice /= 2;
		}		
	}

	iPrice *= iNumOfItem;
	return iPrice;
}
//강동민 수정 끝 테스트 클라이언트 작업		06.16

void CUIShop::CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild)
{
	if( _pUIMgr->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
		_pUIMgr->CloseMessageBox(MSGCMD_CUBE_STATE);

	LONG lMyPoint = 0, lInfoCubePoint = 0; // 자신이 소속된 길드 및 개인 포인트 , 각 리스트의 큐브 포인트
	BYTE InfoCount = 0, InfoRank = 0; // 리스트 갯수, 랭크 정보

	CTString strGuildName, strMyName; // 길드 이름, 길드 마스터 및 캐릭터 이름

	(*istr) >> lMyPoint;
	(*istr) >> InfoCount;

	CUIMsgBox_Info MsgBoxInfo;
	CTString strMsg, strMsgEx;

	if (bGuild)
	{
		strMsg = _S(4388, "길드 큐브 포인트 현황");
		strMsgEx = _S(4389, "자신이 소속된 길드 큐브 포인트 : %d");
		strGuildName = _S(3451,"길드명");
		strMyName = _S(4344, "길드 마스터");
	}
	else
	{
		strMsg = _S(4390, "개인 큐브 포인트 현황");
		strMsgEx = _S(4391, "자신의 개인 큐브 포인트 : %d");
		strGuildName = _S(4392, "소속 길드");
		strMyName = _S(4393, "캐릭터 이름");
	}

	MsgBoxInfo.SetMsgBoxInfo(strMsg, UMBS_OK | UMBS_LISTBOX, UI_SHOP, MSGCMD_CUBE_STATE);

	strMsg.PrintF(strMsgEx, lMyPoint);
	MsgBoxInfo.AddStringEx(strMsg, 0, 1);
	MsgBoxInfo.AddStringEx(_S(4343, "큐브포인트 현황"), 2, 1);
	MsgBoxInfo.AddStringEx(_S(1714, "순위"), 4, 1);
	MsgBoxInfo.AddStringEx( strGuildName, 4, 9 );
	MsgBoxInfo.AddStringEx( strMyName, 4, 28 );
	MsgBoxInfo.AddStringEx( _S(4345, "큐브포인트"), 4, 47 );
	MsgBoxInfo.SetListBoxPlacement(20, 113, 350, 84, 5, 4); // 173 - 118 = 55
	MsgBoxInfo.m_nColorBoxCount = 9;
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetWidth(400);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().ResetAllStrings();
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(0, 6);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(1, 42);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(2, 156);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(3, 335, TEXT_RIGHT);

	int i;

	for (i=0; i<InfoCount; ++i)
	{
		(*istr) >> InfoRank >> strGuildName >> strMyName >> lInfoCubePoint;

		strMsg.PrintF("%d", InfoRank);
		_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(0, strMsg, 0xF2F2F2FF, FALSE);
		_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(1, strGuildName, 0xF2F2F2FF, FALSE);
		_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(2, strMyName, 0xF2F2F2FF, FALSE);
		strMsg.PrintF("%d", lInfoCubePoint);
		_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(3, strMsg, 0xF2F2F2FF, FALSE );
	}

	WRect rcHorizonalLine, rcVerticalLine0, rcVerticalLine1;
	// 전체적으로 높이 -48 , 추가 -12
	rcHorizonalLine.SetRect( 15, 58, 386, 58);
	rcVerticalLine0.SetRect( 15, 58, 15, 204);
	rcVerticalLine1.SetRect( 51, 82, 51, 204);

	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(0, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 24);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(1, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 25);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(2, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 95);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(3, 0, rcHorizonalLine);

	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(4, 0, rcVerticalLine0);
	rcVerticalLine0.Offset(370, 0);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(5, 0, rcVerticalLine0);

	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(6, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(7, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	_pUIMgr->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(8, 0, rcVerticalLine1);
}