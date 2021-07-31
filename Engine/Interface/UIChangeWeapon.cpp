#include "stdh.h"
#include <Engine/Interface/UIChangeWeapon.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/ItemData.h>
#include <algorithm>

extern INDEX g_iCountry;

// [KH_07044] 3차 도움말 관련 추가
#ifdef HELP_SYSTEM_1
extern INDEX g_iShowHelp1Icon;
#endif

enum eSelection
{
	CHANGEWEAPON_OK,
	CHANGEWEAPON_TALK,
	CHANGEWEAPON_EVENT,
	CHANGESHIELD_OK,

};

static int	_iMaxMsgStringChar = 0;

#define DESC_LIST_BOX_HEIGHT			(130)
#define CRITERION_LEVEL					(28)
#define WEAPON_LIST_START_X				(21)
#define WEAPON_INFO_START_X				(21)
#define WEAPON_DESC_START_X				(190)
#define WEAPON_LISTBOX_WIDTH			(193)

#define EXCHANGE_LIMIT_LEVEL			(29)

//#define CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
//#define CHANGE_WEAPON_SHIELD_EVENT			// 무기 방어구 교체 이벤트

// NOTE : 서버측에 하드코딩 되어 있는 무기 교체 가능 아이템 인덱스.
// 레벨, 한손검, 석궁, 스태프, 대검, 도끼, 숏스테프, 활, 단검, -1, -1, -1, 이도류, 완드, 사이드, 폴암 : 할일 : 소서러
const static matchTable[12][16] = {
	{1,48,530,600,12,558,356,50,528,-1,-1,-1,459,599,-1,-1},
	{5,53,666,628,51,601,357,56,529,-1,-1,-1,610,619,-1,-1},
	{9,55,637,629,52,602,358,57,532,-1,-1,-1,611,620,-1,-1},
	{13,107,638,630,105,603,359,106,533,-1,-1,-1,612,621,-1,-1},
	{17,180,639,631,306,604,360,185,534,-1,-1,-1,613,622,-1,-1},
	{21,322,640,632,307,605,361,341,535,-1,-1,-1,614,623,-1,-1},
	{25,323,641,633,308,606,362,342,536,-1,-1,-1,615,624,-1,-1},
	{29,324,642,634,309,607,363,343,537,-1,-1,-1,616,625,-1,-1},
	{33,325,643,635,310,608,364,344,538,-1,-1,-1,617,626,-1,-1},
	{37,326,644,636,311,609,365,345,539,-1,-1,-1,618,627,-1,-1},
	{41,681,721,719,715,645,720,718,722,-1,-1,-1,716,717,-1,-1},
	{45,800,823,816,793,794,815,808,822,-1,-1,-1,801,809,-1,-1}
};

// ----------------------------------------------------------------------------
// Name : IsChangeableWeapon()
// Desc : 바꿀수 있는 아이템인지 체크합니다.
// ----------------------------------------------------------------------------
BOOL CUIChangeWeapon::IsChangeableWeapon( int iWeaponIndex )
{
	if( iWeaponIndex == -1 )
		return FALSE;
	// 아이템 목록을 서버측에서 긁어다가 쓸수 있게...
	for( int i = 1; i < 12; ++i )
	{
		for( int j = 0; j < 16; ++j )
		{
			if( matchTable[i][j] == iWeaponIndex )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : CUIChangeWeapon()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIChangeWeapon::CUIChangeWeapon()
{
	m_eChangeWeaponState	= CHANGEWEAPON_REQ;
	m_nStringCount			= 0;
	m_strChangeWeaponMoney	= CTString( "" );
	m_llChangeWeaponMoney	= 0;

	m_strWeaponName			= CTString( "" );
	m_strWeaponLevel		= CTString( "" );
	m_strWeaponClass		= CTString( "" );
	m_strWeaponAttack		= CTString( "" );
	m_strWeaponDefence		= CTString( "" );
	m_strWeaponMagicAttack	= CTString( "" );
	m_strWeight				= CTString( "" );

	m_slLevel				= -1;
	m_bCashItemChk			= FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIChangeWeapon()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIChangeWeapon::~CUIChangeWeapon()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
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
	m_btnOK.Create( this, _S( 191, "확인" ), 78, 371, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 146, 371, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box of guild description
	m_lbWeaponDesc.Create( this, 8, 263, WEAPON_LISTBOX_WIDTH, 101, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );
	m_lbWeaponDesc.CreateScroll( TRUE, 0, 0, 9, 101, 9, 7, 0, 0, 10 );
	m_lbWeaponDesc.SetSelBar( 190, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	// Up button
	m_lbWeaponDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbWeaponDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbWeaponDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbWeaponDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbWeaponDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbWeaponDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbWeaponDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbWeaponDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbWeaponDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbWeaponDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbWeaponDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// List box of guild description
	m_lbWeaponList.Create( this, 8, 158, WEAPON_LISTBOX_WIDTH, 101, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, TRUE );
	m_lbWeaponList.CreateScroll( TRUE, 0, 0, 9, 101, 9, 7, 0, 0, 10 );
	m_lbWeaponList.SetSelBar( WEAPON_LISTBOX_WIDTH, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbWeaponList.SetOverColor( 0xF8E1B5FF );
	m_lbWeaponList.SetSelectColor( 0xF8E1B5FF );	
	// Up button
	m_lbWeaponList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbWeaponList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbWeaponList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbWeaponList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbWeaponList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbWeaponList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbWeaponList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbWeaponList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbWeaponList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbWeaponList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbWeaponList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	m_rcItemSlot.Top		= 51;
	m_rcItemSlot.Bottom		= m_rcItemSlot.Top + 34;

	m_rcInsertItem.Top		= 33;
	m_rcInsertItem.Bottom	= m_rcItemSlot.Bottom + 33;

	// Slot item button
	m_btnSlotItem.Create( this, m_rcItemSlot.Left + 1, m_rcItemSlot.Top + 1, BTN_SIZE, BTN_SIZE, UI_CHANGEWEAPON, UBET_ITEM );
/*
	AddWeaponInfo( CItemData::ITEM_WEAPON_KNIFE,	_S( 1038, "기사도" ),	TITAN );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_CROSSBOW, _S( 1039, "석궁" ),		ROGUE );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_STAFF,	_S( 1040, "스태프" ),	MAGE );			
	AddWeaponInfo( CItemData::ITEM_WEAPON_BIGSWORD, _S( 1041, "대검" ),		TITAN );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_AXE,		_S( 1042, "도끼" ),		TITAN );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_SSTAFF,	_S( 1043, "숏스태프" ), MAGE );			
	AddWeaponInfo( CItemData::ITEM_WEAPON_BOW,		_S( 1044, "활" ),		HEALER );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_DAGGER,	_S( 1045, "단검" ),		ROGUE );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_TWOSWORD, _S( 1046, "이도류" ),	KNIGHT );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_WAND,		_S( 1047, "힐러완드" ),		HEALER );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_SCYTHE,	_S( 2306,"사이드" ),		SORCERER );		
	AddWeaponInfo( CItemData::ITEM_WEAPON_POLEARM,		_S(2307, "폴암" ),		SORCERER );		

	RefreshWeaponList();
	*/
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenChangeWeapon()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::OpenChangeWeapon(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_CHANGEWEAPON_REQ ) || IsVisible() )
		return;

	// 전직 캐릭터인지를 체크함!!!
	// 전직 캐릭터가 아니라면 return;
	//_pUIMgr->GetChatting()->AddSysMessage( _S( 1048, "전직 캐릭터만 무기 교체를 할 수 있습니다." ), SYSMSG_ERROR );		

	_pUIMgr->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	_pUIMgr->CloseMessageBox( MSGCMD_CHANGEWEAPON_NOTIFY );

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData& MD = _pNetwork->GetMobData(iMobIndex);

	// Create refine message box
	_pUIMgr->CreateMessageBoxL( _S(2308, "무기및 방어구 교체" ), UI_CHANGEWEAPON, MSGLCMD_CHANGEWEAPON_REQ );		

	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, strNpcName, -1, 0xE18600FF );

#ifndef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S( 1050, "지금 사용하고 있는 무기나 방어구가 마음에 들지 않는겐가?" ), -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S( 1051, "그럼 내가 자네를 도와줄 수 있겠구만." ), -1, 0xA3A1A3FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2309,"하지만 주의해야할 사항이 있네"), -1, 0xA3A1A3FF );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2310,"교체되는 장비는 제련된 수치만 남고 블러드 옵션값이 삭제되니까 잘 생각해 보시고 교체게나."), -1, 0xF3BA0CFF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2311,"그래도 이렇게 쉽게 새로운 장비를 구할 수 있는 곳이 어디 있겠나..."), -1, 0xA3A1A3FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2312,"자 빨리 교환하게나...  사람들이 더 몰리기 전에..."), -1, 0xA3A1A3FF );
#else
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2152,"당신도 무기 교체를 하러 오신 것 맞죠?"), -1, 0xA3A1A3FF );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2153,"사용하던 무기를 무료로 다른 종류의 무기로 바꿔 준다는 소문이 어찌나 빠르게 퍼졌던지 쉴세없이 사람들이 몰려와서 정신이 없네요."), -1, 0xA3A1A3FF );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2154,"하지만 주의해야할 사항이 있어요!"), -1, 0xA3A1A3FF );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2155,"교체되는 무기는 제련된 수치만 남고 블러드 옵션값이 삭제되니까 잘 생각해 보시고 교체하세요."), -1, 0xF3BA0CFF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2156,"그래도 이렇게 쉽게 새로운 무기를 구할 수 있는 곳이 어디 있겠어요..."), -1, 0xA3A1A3FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2157,"자 빨리 교환하세요.  사람들이 더 몰리기 전에요..."), -1, 0xA3A1A3FF );
	
#endif

	CTString strMessage;	
	strMessage.PrintF( _S( 1049, "무기 교체" ) );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_OK );	
	
	// wooss add shield item exchange
	strMessage= _S(2313, "방어구 교체" );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGESHIELD_OK );	

	if( bHasQuest )
	{
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_CHANGEWEAPON_REQ);				
#else
		strMessage.PrintF( _S( 1053, "이야기한다." ) );	
		_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_TALK );
#endif

	}
	
	if( MD.IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_EVENT );
	}

	strMessage.PrintF( _S( 1220, "취소한다." ) );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage );	

}

// ----------------------------------------------------------------------------
// Name : CloseChangeWeapon()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::CloseChangeWeapon()
{
	// Reset slot item
	m_btnSlotItem.InitBtn();

	// Reset ListBox
	m_lbWeaponDesc.ResetAllStrings();
	m_lbWeaponList.ResetAllStrings();

	m_strWeaponName.Clear();
	m_strWeaponLevel.Clear();
	m_strWeaponClass.Clear();
	m_strWeaponAttack.Clear();
	m_strWeaponDefence.Clear();
	m_strWeaponMagicAttack.Clear();
	m_strWeight.Clear();
	m_slLevel				= -1;

	// Close refine
	_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, FALSE );

	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_CHANGEWEAPON );

	m_strChangeWeaponMoney.Clear();

	//wooss 051011
	if(m_bCashItemChk) m_bCashItemChk = FALSE;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	// cash item check wooss 051011
	if(!m_bCashItemChk)
		if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
			CloseChangeWeapon();


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
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 103,
		m_rtMiddle1.U0, m_rtMiddle1.V0,
		m_rtMiddle1.U1, m_rtMiddle1.V1,
		0xFFFFFFFF );

	nY += 103;
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 11,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );

	nY += 11;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 18,
		m_rtMiddle3.U0, m_rtMiddle3.V0,
		m_rtMiddle3.U1, m_rtMiddle3.V1,
		0xFFFFFFFF );

	nY += 18;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 101,
		m_rtMiddle1.U0, m_rtMiddle1.V0,
		m_rtMiddle1.U1, m_rtMiddle1.V1,
		0xFFFFFFFF );

	nY += 101;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 4,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );
	
	nY += 4;
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 101,
		m_rtMiddle1.U0, m_rtMiddle1.V0,
		m_rtMiddle1.U1, m_rtMiddle1.V1,
		0xFFFFFFFF );

	nY += 101;

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );
	
	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
		m_rtBottom.U0, m_rtBottom.V0,
		m_rtBottom.U1, m_rtBottom.V1,
		0xFFFFFFFF );

	// Slot item region
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcItemSlot.Left, m_nPosY + m_rcItemSlot.Top,
										m_nPosX + m_rcItemSlot.Right, m_nPosY + m_rcItemSlot.Bottom,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
										0xFFFFFFFF );

	nX	= m_nPosX + 95;
	nX2	= nX + 84;
	nY	= m_nPosY + 102;
	nY2 = nY + 13;

	if(!m_bCashItemChk){
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
	}
	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// List Box
	m_lbWeaponDesc.Render();
	m_lbWeaponList.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Item
	if( !m_btnSlotItem.IsEmpty() )
	{
		m_btnSlotItem.Render();
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	if( m_eChangeWeaponState == CHANGEWEAPON_STAT ){
	
		// Text in refine
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1049, "무기 교체"  ), m_nPosX + CHANGEWEAPON_TITLE_TEXT_OFFSETX,		
											m_nPosY + CHANGEWEAPON_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1057, "교체무기" ), m_nPosX + 40,		
											m_nPosY + 60, 0xFFFFFFFF );

		// cash item check wooss 051011
		if(!m_bCashItemChk) {
			nY = m_nPosY + 104;
			_pUIMgr->GetDrawPort()->PutTextEx( _S( 1058, "필요나스" ), m_nPosX + 40,		
										nY, 0xFFFFFFFF );
			// ChangeWeaponing money
#ifdef CHANGE_WEAPON_SHIELD_EVENT	
			_pUIMgr->GetDrawPort()->PutTextEx( _S(2315," 교체 이벤트"),	m_nPosX + 95, m_nPosY + 102, 0xE1B300FF );	
#else
			_pUIMgr->GetDrawPort()->PutTextEx( m_strChangeWeaponMoney,	m_nPosX + 95, m_nPosY + 102, 0xE1B300FF );				
#endif 

		}
		
		nY = m_nPosY + 143;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1059, "무기 리스트" ), m_nPosX + 12,		
											nY, 0xE18600FF );

		nY = m_nPosY + 269;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1065, "무기 이름" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponName, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1066, "무기 레벨" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponLevel, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1067, "사용 클래스" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponClass, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1068, "공격력" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponAttack, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1069, "마법 공격력" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponMagicAttack, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1070, "무게" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeight, m_nPosX + WEAPON_DESC_START_X,
											nY, 0xFFd223FF );
	}
	else if( m_eChangeWeaponState == CHANGEWEAPON_SHIELD){
		// Text in refine
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 2313,"방어구 교체"  ), m_nPosX + CHANGEWEAPON_TITLE_TEXT_OFFSETX,		
											m_nPosY + CHANGEWEAPON_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextEx( _S( 2314,"교체 방어구" ), m_nPosX + 30,		
											m_nPosY + 60, 0xFFFFFFFF );

		// cash item check wooss 051011
		if(!m_bCashItemChk) {
			nY = m_nPosY + 104;
			_pUIMgr->GetDrawPort()->PutTextEx( _S( 1058, "필요나스" ), m_nPosX + 40,		
										nY, 0xFFFFFFFF );
#ifdef CHANGE_WEAPON_SHIELD_EVENT	
			_pUIMgr->GetDrawPort()->PutTextEx( _S(2315," 교체 이벤트"), m_nPosX + 95, m_nPosY + 102, 0xE1B300FF );	
#else
			_pUIMgr->GetDrawPort()->PutTextEx( m_strChangeWeaponMoney,	m_nPosX + 95, m_nPosY + 102, 0xE1B300FF );				
#endif 
		}
		
		nY = m_nPosY + 143;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(2316, "클래스 리스트" ), m_nPosX + 12,		
											nY, 0xE18600FF );

		nY = m_nPosY + 269;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(2317, "방어구 이름" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponName, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(2318, "방어구 레벨" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponLevel, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1067, "사용 클래스" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponClass, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S(87, "방어력" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeaponDefence, m_nPosX + WEAPON_DESC_START_X,		
											nY, 0xFFd223FF );

		nY += 15;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1070, "무게" ), m_nPosX + WEAPON_INFO_START_X,		
											nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextExRX( m_strWeight, m_nPosX + WEAPON_DESC_START_X,
											nY, 0xFFd223FF );

	}


	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChangeWeapon::MouseMessage( MSG *pMsg )
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
			// List box
			else if( m_lbWeaponList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbWeaponDesc.MouseMessage( pMsg ) != WMSG_FAIL )
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
				// List box
				else if( m_lbWeaponList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					int	iSelWeapon = m_lbWeaponList.GetCurSel();
					if( iSelWeapon != -1 && !m_btnSlotItem.IsEmpty() && m_slLevel > 0 )
					{
						sWeaponInfo &TempMember = m_vectorWeaponInfo[iSelWeapon];

						if( m_eChangeWeaponState == CHANGEWEAPON_STAT )
						{
							m_llChangeWeaponMoney = CalculateNeedNas( m_slLevel );
							m_strChangeWeaponMoney.PrintF( "%I64d", m_llChangeWeaponMoney );
						
							// FIXME : 이 함수를 호출하기 전에, 아이템을 선택했는지부터 확인.
							// 아이템 설명을 얻습니다.
							GetWeaponInfo( _pNetwork->MyCharacterInfo.job, m_slLevel, TempMember.iIndex );
						}
						else if( m_eChangeWeaponState == CHANGEWEAPON_SHIELD )
						{
							m_llChangeWeaponMoney = CalculateNeedNasS( m_slLevel );
							m_strChangeWeaponMoney.PrintF( "%I64d", m_llChangeWeaponMoney );
						
							// FIXME : 이 함수를 호출하기 전에, 아이템을 선택했는지부터 확인.
							// 아이템 설명을 얻습니다.
							GetShieldInfo( iSelWeapon, m_slLevel,m_selItem.ItemData.GetSubType());
						}
						
					}
					return WMSG_SUCCESS;
				}
				else if( m_lbWeaponDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, TRUE );
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
						CloseChangeWeapon();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						SendChangeWeaponReq();				
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseChangeWeapon();

					return WMSG_SUCCESS;
				}
				// List box
				else if( ( wmsgResult = m_lbWeaponList.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
					}
					return WMSG_SUCCESS;
				}
				else if( m_lbWeaponDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
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
						{
							// Set refine item
							if(m_eChangeWeaponState == CHANGEWEAPON_STAT)
								SetChangeWeaponItem();
							else if(m_eChangeWeaponState == CHANGEWEAPON_SHIELD)
								SetChangeShieldItem();
						}
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
				return WMSG_SUCCESS;
		}
		break;
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbWeaponList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbWeaponDesc.MouseMessage( pMsg ) != WMSG_FAIL )
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

// ----------------------------------------------------------------------------
// Name : SetChangeShieldItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::SetChangeShieldItem()
{
	// If this is wearing item
	if( _pUIMgr->GetHoldBtn().GetItemWearType() >= 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1071, "착용된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// If this is not weapon or armor
	int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();
	// CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	// wooss : add shield item exchange
	m_selItem = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = m_selItem.ItemData;
	if( rItemData.GetType() != CItemData::ITEM_SHIELD )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(2320, "방어구만 교체 할 수 있습니다." ), SYSMSG_ERROR );		
		return;
	}

#ifndef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	if( rItemData.GetLevel() < 29 || rItemData.GetLevel() >72)
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1073, "교체할 수 있는 아이템 레벨이 아닙니다." ), SYSMSG_ERROR );		
		return;
	}
#else

	// 교환 가능한 아이템 목록인지 확인합니다.
	if( !IsChangeableWeapon( rItems.Item_Index ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(2158,"교체할 수 없는 아이템입니다."), SYSMSG_ERROR );
		return;
	}
#endif

	// If refining money is short
	m_slLevel = rItemData.GetLevel();

	// Insert upgrade slot
	m_btnSlotItem.Copy( _pUIMgr->GetHoldBtn() );
	
	m_llChangeWeaponMoney = CalculateNeedNasS( m_slLevel );
	m_strChangeWeaponMoney.PrintF( "%I64d", m_llChangeWeaponMoney );

	// Lock inventory
	//_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_CHANGEWEAPON );
	int	iSelWeapon = m_lbWeaponList.GetCurSel();
	if( iSelWeapon == -1 )
	{
		return;
	}
	else
	{
		sWeaponInfo &TempMember = m_vectorWeaponInfo[iSelWeapon];

		// FIXME : 이 함수를 호출하기 전에, 아이템을 선택했는지부터 확인.
		// 아이템 설명을 얻습니다.
		GetShieldInfo( _pNetwork->MyCharacterInfo.job, m_slLevel,m_selItem.ItemData.GetSubType());

		// Lock inventory
		_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_CHANGEWEAPON );
	}
}

// ----------------------------------------------------------------------------
// Name : SetChangeWeaponItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::SetChangeWeaponItem()
{
	// If this is wearing item
	if( _pUIMgr->GetHoldBtn().GetItemWearType() >= 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1071, "착용된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// If this is not weapon or armor
	int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();
	// CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	// wooss : add shield item exchange
	CItems		&m_selItem = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = m_selItem.ItemData;
	if( rItemData.GetType() != CItemData::ITEM_WEAPON )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1072, "무기만 교체 할 수 있습니다." ), SYSMSG_ERROR );		
		return;
	}

	// 060810 wooss
	// 추가된 교환 불가 아이템
	// 1. 플래티늄 제련석 적용 아이템 2. 합성한 아이템
	if(		m_selItem.IsFlag(PLATINUM_MAX_PLUS) 
		||	m_selItem.IsFlag(FLAG_ITEM_COMPOSITION))
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(2158,"교체할 수 없는 아이템입니다."), SYSMSG_ERROR );
		return;
	}

#ifndef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	if( rItemData.GetLevel() < 29 || rItemData.GetLevel() >69)
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1073, "교체할 수 있는 아이템 레벨이 아닙니다." ), SYSMSG_ERROR );		
		return;
	}
#else

	// 교환 가능한 아이템 목록인지 확인합니다.
	if( !IsChangeableWeapon( rItems.Item_Index ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(2158,"교체할 수 없는 아이템입니다."), SYSMSG_ERROR );
		return;
	}
#endif

	// If refining money is short
	m_slLevel = rItemData.GetLevel();


	// Insert upgrade slot
	m_btnSlotItem.Copy( _pUIMgr->GetHoldBtn() );

	m_llChangeWeaponMoney = CalculateNeedNas( m_slLevel );
	m_strChangeWeaponMoney.PrintF( "%I64d", m_llChangeWeaponMoney );



	// Lock inventory
	//_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_CHANGEWEAPON );
	int	iSelWeapon = m_lbWeaponList.GetCurSel();
	if( iSelWeapon == -1 )
	{
		return;
	}
	else
	{
		sWeaponInfo &TempMember = m_vectorWeaponInfo[iSelWeapon];

		// FIXME : 이 함수를 호출하기 전에, 아이템을 선택했는지부터 확인.
		// 아이템 설명을 얻습니다.
		GetWeaponInfo( _pNetwork->MyCharacterInfo.job, m_slLevel,  TempMember.iIndex );

		// Lock inventory
		_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_CHANGEWEAPON );
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_CHANGEWEAPON_REP:		// 무기 교체 확인.
		{
			if( bOK )
			{
				int	iSelWeapon = m_lbWeaponList.GetCurSel();
				if( iSelWeapon != -1 )
				{
					SBYTE sbRow		= m_btnSlotItem.GetItemRow();
					SBYTE sbCol		= m_btnSlotItem.GetItemCol();
					int iUniIndex	= m_btnSlotItem.GetItemUniIndex();
					// 서버쪽으로 메세지 보내기.
					sWeaponInfo &TempMember = m_vectorWeaponInfo[iSelWeapon];

#ifndef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
					if(m_bCashItemChk)
						_pNetwork->UseChangeWeaponItem( sbRow, sbCol, iUniIndex, TempMember.iIndex );
					else
					{
						if(m_eChangeWeaponState == CHANGEWEAPON_STAT)
							_pNetwork->ChangeWeaponReq( sbRow, sbCol, iUniIndex, TempMember.iIndex );
						else if(m_eChangeWeaponState == CHANGEWEAPON_SHIELD)
							_pNetwork->ChangeWeaponReq( sbRow, sbCol, iUniIndex, iSelWeapon);
					}
							
#else				
			//		if(m_bCashItemChk) _pNetwork->ChangeWeaponReq( sbRow, sbCol, iUniIndex, TempMember.iIndex );  else //wooss 051011
					_pNetwork->ChangeWeaponEventReq( iUniIndex, TempMember.iIndex );
#endif
				}
			}
		}
		break;
	case MSGCMD_CHANGEWEAPON_NOTIFY:
		{
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_CHANGEWEAPON_REQ:
		if( nResult == CHANGEWEAPON_OK )
		{
			m_vectorWeaponInfo.clear();		
			AddWeaponInfo( CItemData::ITEM_WEAPON_KNIFE,	_S( 1038, "기사도" ),	TITAN );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_CROSSBOW, _S( 1039, "석궁" ),		ROGUE );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_STAFF,	_S( 1040, "스태프" ),	MAGE );			
			AddWeaponInfo( CItemData::ITEM_WEAPON_BIGSWORD, _S( 1041, "대검" ),		TITAN );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_AXE,		_S( 1042, "도끼" ),		TITAN );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_SSTAFF,	_S( 1043, "숏스태프" ), MAGE );			
			AddWeaponInfo( CItemData::ITEM_WEAPON_BOW,		_S( 1044, "활" ),		HEALER );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_DAGGER,	_S( 1045, "단검" ),		ROGUE );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_TWOSWORD, _S( 1046, "이도류" ),	KNIGHT );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_WAND,		_S( 1047, "힐러완드" ),		HEALER );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_SCYTHE,	_S(2306, "사이드" ),		SORCERER );		
			AddWeaponInfo( CItemData::ITEM_WEAPON_POLEARM,		_S(2307, "폴암" ),		SORCERER );		

			RefreshWeaponList();

			m_eChangeWeaponState = CHANGEWEAPON_STAT;
			_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, TRUE );

			if( !_pUIMgr->GetInventory()->IsVisible() )
				_pUIMgr->GetInventory()->ToggleVisible();
#ifdef HELP_SYSTEM_1			
// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				_pUIMgr->GetHelp3()->ClearHelpString();
				_pUIMgr->GetHelp3()->AddHelpString(_S(3288, "교체하고자 하는 무기나 방어구를 왼쪽 상단의 빈 칸에 올려 놓으면 교체비용이 계산되며, 교체하려는 종류를 선택하고 확인을 누르면 비용이 지불되고 제련된 수치 그대로 선택한 종류의 장비로 교환이 이루어 집니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3289, "※ 교체되는 무기 및 방어구의 블러드 옵션은 옮겨지지 않습니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3290, "※ 29레벨 이상의 장비부터 교체가 가능합니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3291, "※ 착용 중인 장비나 레어옵션 장비(73레벨 이상)는 교체할 수 없습니다."));
				_pUIMgr->GetHelp3()->OpenHelp(this);
			}
#endif
		}		
		else if( nResult == CHANGEWEAPON_TALK )				// 이야기하기.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
			//_pUIMgr->GetQuest()->OpenQuest( _pUIMgr->GetCharacterInfo()->GetMobIndex(), m_fNpcX, m_fNpcZ );
		}
		else if( nResult == CHANGEWEAPON_EVENT )			// Event
		{
		}
		else if(nResult == CHANGESHIELD_OK)
		{
			m_vectorWeaponInfo.clear();
					
			AddWeaponInfo( TITAN,	_S( 43, "타이탄" ),	TITAN );		
			AddWeaponInfo( KNIGHT,  _S( 44, "기사" ),		KNIGHT );		
			AddWeaponInfo( HEALER,	_S( 45, "힐러" ),		HEALER );			
			AddWeaponInfo( MAGE,	_S( 46, "메이지" ),	MAGE );			
			AddWeaponInfo( ROGUE,	_S( 47, "로그" ),		ROGUE );		
			AddWeaponInfo( SORCERER, _S( 48,"소서러" ),	SORCERER );		
		
			RefreshWeaponList();
		

			m_eChangeWeaponState = CHANGEWEAPON_SHIELD;
			_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, TRUE );

			if( !_pUIMgr->GetInventory()->IsVisible() )
				_pUIMgr->GetInventory()->ToggleVisible();
#ifdef HELP_SYSTEM_1
			// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				_pUIMgr->GetHelp3()->ClearHelpString();
				_pUIMgr->GetHelp3()->AddHelpString(_S(3288, "교체하고자 하는 무기나 방어구를 왼쪽 상단의 빈 칸에 올려 놓으면 교체비용이 계산되며, 교체하려는 종류를 선택하고 확인을 누르면 비용이 지불되고 제련된 수치 그대로 선택한 종류의 장비로 교환이 이루어 집니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3289, "※ 교체되는 무기 및 방어구의 블러드 옵션은 옮겨지지 않습니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3290, "※ 29레벨 이상의 장비부터 교체가 가능합니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3291, "※ 착용 중인 장비나 레어옵션 장비(73레벨 이상)는 교체할 수 없습니다."));
				_pUIMgr->GetHelp3()->OpenHelp(this);
			}
#endif
		}
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}
		else
		{
			_pUIMgr->RearrangeOrder( UI_CHANGEWEAPON, FALSE );
		}
		break;
		
	
	}
}

// ----------------------------------------------------------------------------
// Name : CalculateNeedNas()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIChangeWeapon::CalculateNeedNas( int iWeaponLevel )
{
#ifdef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	return 0;
#else
	ASSERT( iWeaponLevel <= CRITERION_LEVEL && "Invalid Weapon Level" );

	SQUAD llPrice	= 0;
	if( iWeaponLevel <= CRITERION_LEVEL )
		return llPrice;

//	llPrice			= ( iWeaponLevel - CRITERION_LEVEL ) * 5000;
// wooss 051217
//	llPrice = 5000 * (m_selItem.Item_Plus * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	llPrice = 5000 * (m_btnSlotItem.GetItemPlus() * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	return llPrice;
#endif
}

// ----------------------------------------------------------------------------
// Name : CalculateNeedNasS()
// Desc : Shield item Price
// ----------------------------------------------------------------------------
SQUAD CUIChangeWeapon::CalculateNeedNasS( int iWeaponLevel )
{
#ifdef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	return 0;
#else
	ASSERT( iWeaponLevel <= CRITERION_LEVEL && "Invalid Weapon Level" );

	SQUAD llPrice	= 0;
	if( iWeaponLevel <= CRITERION_LEVEL )
		return llPrice;

//	llPrice			= ( iWeaponLevel - CRITERION_LEVEL ) * 5000;
// wooss 051217
//	llPrice = 3000 * (m_selItem.Item_Plus * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	llPrice = 3000 * (m_btnSlotItem.GetItemPlus() * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	return llPrice;
#endif
}
// ----------------------------------------------------------------------------
// Name : SendChangeWeaponReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::SendChangeWeaponReq()
{
	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1074, "스킬 사용중에는 아이템을 교체할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1075, "순간 이동중에는 아이템를 교체할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( m_btnSlotItem.IsEmpty() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1076, "교체할 아이템이 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	const ULONG ulFlag = m_btnSlotItem.GetItemFlag();
/*
#ifndef CHANGE_WEAPON_SHIELD_EVENT		// 2판4판 무기 교체 이벤트.
	if( m_btnSlotItem.GetItemPlus() > 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1077, "플러스 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}
#endif

	

#ifndef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
	if( ulFlag & FLAG_ITEM_OPTION_ENABLE )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1078, "블러드 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}
#endif
*/
	if( ulFlag & FLAG_ITEM_SEALED )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

#ifndef CHANGE_WEAPON_SHIELD_EVENT
	if( m_llChangeWeaponMoney > _pNetwork->MyCharacterInfo.money && !m_bCashItemChk )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 1080, "교체 비용이 부족합니다." ), SYSMSG_ERROR );		
		return;
	}
#endif

	if( m_lbWeaponList.GetCurSel() == -1 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2321,"새로운 무기및 방어구를 선택해주십시요." ), SYSMSG_ERROR );		
		return;
	}

	// wooss 051217 add shield item exchange
	if(m_slLevel < EXCHANGE_LIMIT_LEVEL)
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S(2322,"29Lv이상의 무기및 방어구만 교체를 하실 수 있습니다."));
		return;
	}

	
	_pUIMgr->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	
	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2308, "무기및 방어구 교체" ), UMBS_OKCANCEL,UI_CHANGEWEAPON, MSGCMD_CHANGEWEAPON_REP );
	MsgBoxInfo.AddString( _S(2323,"교체 무기및 방어구에 제련된 기록은 옮겨지지만 블러드 옵션은 옮겨지지 않습니다. 무기및 방어구를 교체하시겠습니까?" ) );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}


// ----------------------------------------------------------------------------
// Name : AddWeaponDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::AddWeaponDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbWeaponDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddWeaponDescString( strTemp, colDesc );
		}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strDesc,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

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

				AddWeaponDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddWeaponDescString( strTemp, colDesc );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbWeaponDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddWeaponDescString( strTemp, colDesc );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

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

				AddWeaponDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbWeaponDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddWeaponDescString( strTemp, colDesc );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetWeaponInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::GetWeaponInfo( int iJob, int iWeaponLevel, int iWeaponType )
{
	ASSERT( iWeaponLevel != -1 && "Invalid Weapon Level" );	
	for( int i = 1; i < _pNetwork->wo_iNumOfItem; ++i )
	{
		CItemData& ID = _pNetwork->GetItemData( i );
		if( ID.GetType() != CItemData::ITEM_WEAPON )
			continue;
		
		if( ID.GetSubType() != iWeaponType )
			continue;
		
		if( ID.GetLevel() != iWeaponLevel )
			continue;

#ifdef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
		if( !IsChangeableWeapon( ID.GetItemIndex() ) )
		{
			continue;
		}
#endif
		
		m_strWeaponName			= ID.GetName();
		m_strWeaponLevel.PrintF( "%d", ID.GetLevel() );
		
		// FIXME : 하나의 무기를 다른 두 직업이 쓸수 있나???
		CTString strTemp;
		for(int i = 0; i < TOTAL_JOB; ++i)
		{
			if( ID.CanUse(i) )
			{
				strTemp += JobInfo().GetName(i);
				break;
			}
		}
		
		m_strWeaponClass		= strTemp;
		m_strWeaponAttack.PrintF( "%d", ID.GetPhysicalAttack() );
		m_strWeaponMagicAttack.PrintF( "%d", ID.GetMagicAttack() );
		m_strWeight.PrintF( "%d", ID.GetWeight() );
		return;
	}	

	m_strWeaponName.Clear();
	m_strWeaponLevel.Clear();
	m_strWeaponClass.Clear();
	m_strWeaponAttack.Clear();
	m_strWeaponMagicAttack.Clear();
	m_strWeight.Clear();
	m_strChangeWeaponMoney.Clear();
}

// ----------------------------------------------------------------------------
// Name : GetShieldInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::GetShieldInfo( int iJob, int iWeaponLevel ,int iWeaponType)
{
	ASSERT( iWeaponLevel != -1 && "Invalid Weapon Level" );	
	for( int i = 1; i < _pNetwork->wo_iNumOfItem; ++i )
	{
		CItemData& ID = _pNetwork->GetItemData( i );
		if( ID.GetType() != CItemData::ITEM_SHIELD )
			continue;

		if( ID.GetSubType() != iWeaponType )
			continue;
				
		if( ID.GetLevel() != iWeaponLevel )
			continue;
		
		if( !(ID.GetJob()&(1<<iJob)))
			continue;

		if( !(ID.IsFlag(ITEM_FLAG_CHANGE)))
			continue;
		

#ifdef CHANGE_WEAPON_EVENT_2PAN4PAN		// 2판4판 무기 교체 이벤트.
		if( !IsChangeableWeapon( ID.GetItemIndex() ) )
		{
			continue;
		}
#endif
		
		m_strWeaponName			= ID.GetName();
		m_strWeaponLevel.PrintF( "%d", ID.GetLevel() );
		m_strWeaponClass		= JobInfo().GetName(iJob);
		m_strWeaponDefence.PrintF( "%d", ID.GetPhysicalDefence() );
		m_strWeight.PrintF( "%d", ID.GetWeight() );
		return;
	}	

	m_strWeaponName.Clear();
	m_strWeaponLevel.Clear();
	m_strWeaponClass.Clear();
	m_strWeaponDefence.Clear();
	m_strWeight.Clear();
	m_strChangeWeaponMoney.Clear();
}

// ----------------------------------------------------------------------------
// Name : AddWeaponInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::AddWeaponInfo( int iType, const CTString& strName, int iJob )
{
	sWeaponInfo		TempWeapon;
	TempWeapon.strName		= strName;
	TempWeapon.iIndex		= iType;
	TempWeapon.iJob			= iJob;
	m_vectorWeaponInfo.push_back(TempWeapon);
}

// ----------------------------------------------------------------------------
// Name : RefreshWeaponList()
// Desc :
// ----------------------------------------------------------------------------
void CUIChangeWeapon::RefreshWeaponList()
{
	m_lbWeaponList.ResetAllStrings();
	std::sort( m_vectorWeaponInfo.begin(), m_vectorWeaponInfo.end() );

	std::vector<sWeaponInfo>::const_iterator End = m_vectorWeaponInfo.end();	
	for( std::vector<sWeaponInfo>::iterator it = m_vectorWeaponInfo.begin();
		it != End; ++it )
	{
		m_lbWeaponList.AddString( 0, (*it).strName );
	}
	
}

