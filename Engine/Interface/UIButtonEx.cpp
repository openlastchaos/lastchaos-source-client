#include "stdh.h"
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIPetInfo.h>

#define EVENT_START_INDEX		2000		// 이벤트 인덱스의 시작 위치.


// ----------------------------------------------------------------------------
// Name : CUIButtonEx()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIButtonEx::CUIButtonEx()
{
	m_nTab = -1;
	m_nRow = -1;
	m_nCol = -1;

	InitBtn();
	m_bSkillDelay = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIButtonEx()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIButtonEx::~CUIButtonEx()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							SBYTE nWhichUI, UIBtnExType betType, SBYTE nTab, SBYTE nRow, SBYTE nCol )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nWhichUI = nWhichUI;
	m_betType = betType;
	m_nTab = nTab;
	m_nRow = nRow;
	m_nCol = nCol;

	// Identification of button
	static UWORD	uwBtnID = 0;
	m_uwBtnID = uwBtnID++;
	m_bSkillDelay = FALSE;

	m_ptdAddTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdAddTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdAddTexture->GetPixHeight();

	m_rtPopupInfo.SetUV(0,137,141,227,fTexWidth,fTexHeight);
	m_rtPopupName.SetUV(0,229,85,251,fTexWidth,fTexHeight);
	
}

// ----------------------------------------------------------------------------
// Name : InitBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::InitBtn()
{
	m_nIndex = -1;
	m_nUniIndex = -1;
	
	m_nCashIndex = -1;
	m_nCashType = -1;
	m_nCashName = "";
	m_nCashDesc = "";
	m_nCashListCount = -1;
	m_nCashTypeIndex = -1;
		
	m_nWearType = -1;
	m_ulPlus = 0;
	m_ulFlag = 0;
	m_llCount = 0;
	m_llPrice = 0;
	m_llTime = 0;

	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		m_sbOptionType[i] = -1;
		m_sbOptionLevel[i] = 0;
	}

	m_nTextureID = -1;
	m_betType = UBET_ITEM;
	m_bEmpty = TRUE;
	m_bsState = UBES_IDLE;
	m_bSpecial	= FALSE;		// Dongmin
	m_ulUsed = 0;
	m_bSkillDelay = FALSE;
	m_bLButtonDown = FALSE;

	m_bShow = FALSE;
	m_bSetPopUp = FALSE;

	m_uwRareIndex =0;
	for (i = 0; i < MAX_ITEMINFO_LINE; i++)
	{
		m_strButtonInfo[i].Clear();	
	}
	
	m_eCharType = CHAR_NAME_NONE;
	m_strCharName = "";
}

// ----------------------------------------------------------------------------
// Name : Copy()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::Copy( CUIButtonEx &btnSrc )
{
	// Item button
	if( btnSrc.GetBtnType() == UBET_ITEM )
		CopyItemInfo( btnSrc );
	// Skill button
	else if( btnSrc.GetBtnType() == UBET_SKILL )
		CopySkillInfo( btnSrc );
	// Action button
	else if( btnSrc.GetBtnType() == UBET_ACTION )
		CopyActionInfo( btnSrc );
	else if( btnSrc.GetBtnType() == UBET_COMBO)
		CopyComboInfo( btnSrc );
	else
		InitBtn();
}

// ----------------------------------------------------------------------------
// Name : Swap()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::Swap( CUIButtonEx &btnSrc )
{
	// Item button
	if( btnSrc.GetBtnType() == UBET_ITEM )
		SwapItemInfo( btnSrc );
}

// ----------------------------------------------------------------------------
// Name : SetItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetItemInfo( SBYTE nTab, SBYTE nRow, SBYTE nCol, int nIndex, int nUniIndex, SBYTE nWearingType, 
							  int nCashIndex , int nCashType , CTString nName, CTString nDesc, int nListCount ,int nTypeIndex)
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	// Set item information
	m_nIndex	= nIndex;
	m_nUniIndex = nUniIndex;

	m_nCashIndex = nCashIndex;
	m_nCashType = nCashType;
	m_nCashName = nName;
	m_nCashDesc = nDesc;
	m_nCashListCount = nListCount;
	m_nCashTypeIndex = nTypeIndex;

	m_nWearType = nWearingType;
	m_nTab		= nTab;
	m_nRow		= nRow;
	m_nCol		= nCol;
	m_bEmpty	= FALSE;
	m_betType	= UBET_ITEM;

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_ITEM && m_nCashIndex == -1)
		return;
	
	int nUVSX = BTN_SIZE;
	int nUVSY = BTN_SIZE;

	if(m_nCashIndex > 0){
		if( nTab !=-1 && nCol !=-1 && nRow !=-1){
			m_nTextureID=nTab;
			nUVSX *= nCol;
			nUVSY *= nRow;
		} else {
			int tv_index = 
				_pNetwork->GetCashShopData(m_nCashType).m_vShopItemArray[m_nCashTypeIndex].m_vItemDataArray[0].m_itemIndex;
			// Set texture data
			CItemData	&rItemData = _pNetwork->GetItemData( tv_index );
			m_nTextureID = rItemData.GetIconTexID();
			int	nTexRow = rItemData.GetIconTexRow();
			int	nTexCol = rItemData.GetIconTexCol();
			nUVSX *= nTexCol;
			nUVSY *= nTexRow;
		}
			
	}
	else {
		// Set texture data
		CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
		m_nTextureID = rItemData.GetIconTexID();
		int	nTexRow = rItemData.GetIconTexRow();
		int	nTexCol = rItemData.GetIconTexCol();
		nUVSX *= nTexCol;
		nUVSY *= nTexRow;
	}

	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, m_nTextureID );
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

	m_bSkillDelay = _pUIMgr->GetInventory()->GetSkillDelay( nIndex );
}

// ----------------------------------------------------------------------------
// Name : CopyItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::CopyItemInfo( CUIButtonEx &btnSrc )
{
	m_nIndex = btnSrc.GetItemIndex();
	m_nUniIndex = btnSrc.GetItemUniIndex();
	
	m_nCashIndex = btnSrc.GetCashIndex();
	m_nCashType = btnSrc.GetCashType();
	m_nCashName = btnSrc.GetCashName();
	m_nCashDesc = btnSrc.GetCashDesc();
	m_nCashListCount = btnSrc.GetCashListCount();
	m_nCashTypeIndex = btnSrc.GetCashTypeIndex();

	m_nWearType = btnSrc.GetItemWearType();
	m_llCount = btnSrc.GetItemCount();
	m_llPrice = btnSrc.GetItemPrice();
	m_ulFlag = btnSrc.GetItemFlag();
	m_ulPlus = btnSrc.GetItemPlus();
	m_ulUsed = btnSrc.GetItemUsed();
	m_nTab = btnSrc.GetItemTab();
	m_nRow = btnSrc.GetItemRow();
	m_nCol = btnSrc.GetItemCol();

	m_uwBtnID = btnSrc.GetBtnID();
	m_nTextureID = btnSrc.GetTextureID();
	m_nWhichUI = btnSrc.GetWhichUI();
	m_betType = btnSrc.GetBtnType();
	m_bEmpty = btnSrc.IsEmpty();	
	m_rtUV = btnSrc.GetUV();
	m_bSkillDelay = btnSrc.GetSkillDelay();
	
	m_dwWndState = btnSrc.GetWndState();

	m_uwRareIndex =btnSrc.GetItemRareIndex();
	//for문은 피하자..(20090206.. Gee~~~)
	for (SBYTE i=0; i<MAX_ITEM_OPTION; ++i)
	{
		m_sbOptionType[i] = btnSrc.GetItemOptionType(i);
		m_sbOptionLevel[i] = btnSrc.GetItemOptionLevel(i);
	}

	m_eCharType = btnSrc.GetCharType();
	m_strCharName = btnSrc.GetCharName();
}

// ----------------------------------------------------------------------------
// Name : SwapItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SwapItemInfo( CUIButtonEx &btnSrc )
{
	int	nTab0 = m_nTab;
	int	nRow0 = m_nRow;
	int	nCol0 = m_nCol;
	int	nTab1 = btnSrc.GetItemTab();
	int	nRow1 = btnSrc.GetItemRow();
	int	nCol1 = btnSrc.GetItemCol();

	CUIButtonEx	btnTemp;
	btnTemp.CopyItemInfo( *this );
	CopyItemInfo( btnSrc );
	btnSrc.CopyItemInfo( btnTemp );

	SetItemLocation( nTab0, nRow0, nCol0 );
	btnSrc.SetItemLocation( nTab1, nRow1, nCol1 );
}

// ----------------------------------------------------------------------------
// Name : SetSkillInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetSkillInfo( int nIndex, SBYTE sbSkillLevel, BOOL bSpecial )
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	// Set skill information
	m_nIndex = nIndex;
	m_nSkillLevel = sbSkillLevel;
	m_bEmpty = FALSE;
	m_betType = UBET_SKILL;
	m_bSpecial	= bSpecial;			// Dongmin

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_SKILL )
		return;

	FLOAT	fTexWidth;
	FLOAT	fTexHeight;
	int		nTexRow = -1;
	int		nTexCol = -1;

	if(!bSpecial)
	{
		// Set texture data
		CSkill	&rSkillData = _pNetwork->GetSkillData( nIndex );
		m_nTextureID = rSkillData.GetIconTexID();
		fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_SKILL, m_nTextureID );
		fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_SKILL, m_nTextureID );
		
		nTexRow = rSkillData.GetIconTexRow();
		nTexCol = rSkillData.GetIconTexCol();
	}
	// Special Skill
	else
	{
		CSpecialSkill	&rSkillData = _pNetwork->GetSSkillData( nIndex );
		m_nTextureID = rSkillData.GetIconTexID();
		fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_SKILL, m_nTextureID );
		fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_SKILL, m_nTextureID );
		
		nTexRow = rSkillData.GetIconTexRow();
		nTexCol = rSkillData.GetIconTexCol();
	}
	
	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

	// 펫 액티브 스킬만!!!
	if( !bSpecial )
	{
		// Set texture data
		CSkill	&rSkillData = _pNetwork->GetSkillData( nIndex );
		if( rSkillData.GetType() == CSkill::ST_PET_SKILL_ACTIVE )
		{
			m_bSkillDelay = _pUIMgr->GetPetInfo()->GetSkillDelay( nIndex );
			return;
		}
	}

	m_bSkillDelay = _pUIMgr->GetCharacterInfo()->GetSkillDelay( nIndex );
}

// ----------------------------------------------------------------------------
// Name : CopySkillInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::CopySkillInfo( CUIButtonEx &btnSrc )
{
	m_nIndex = btnSrc.GetSkillIndex();
	m_nSkillLevel = btnSrc.GetSkillLevel();
	m_bSpecial	= btnSrc.GetSkillSpecial();

	m_uwBtnID = btnSrc.GetBtnID();
	m_nTextureID = btnSrc.GetTextureID();
	m_nWhichUI = btnSrc.GetWhichUI();
	m_betType = btnSrc.GetBtnType();
	m_bEmpty = btnSrc.IsEmpty();	
	m_rtUV = btnSrc.GetUV();
	m_bSkillDelay = btnSrc.GetSkillDelay();
	
	m_dwWndState = btnSrc.GetWndState();
}

// ----------------------------------------------------------------------------
// Name : SetActionInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetActionInfo( int nIndex )
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	// Set action information
	m_nIndex = nIndex;
	m_bEmpty = FALSE;
	m_betType = UBET_ACTION;

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_ACTION )
		return;

	// Set texture data
	CAction	&rActionData = _pNetwork->GetActionData(nIndex);
	m_nTextureID = rActionData.GetTexID();
	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ACTION, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ACTION, m_nTextureID );

	int	nUVSX = BTN_SIZE * rActionData.GetTexCol();
	int	nUVSY = BTN_SIZE * rActionData.GetTexRow();
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : CopyActionInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::CopyActionInfo( CUIButtonEx &btnSrc )
{
	m_nIndex = btnSrc.GetActionIndex();

	m_uwBtnID = btnSrc.GetBtnID();
	m_nTextureID = btnSrc.GetTextureID();
	m_nWhichUI = btnSrc.GetWhichUI();
	m_betType = btnSrc.GetBtnType();
	m_bEmpty = btnSrc.IsEmpty();	
	m_rtUV = btnSrc.GetUV();
	
	m_dwWndState = btnSrc.GetWndState();
}

// ----------------------------------------------------------------------------
// Name : SetQuestInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetQuestInfo( int nIndex, SBYTE sbType, SBYTE sbQuestFlag )
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	// Set quest information
	m_nIndex = nIndex;
	m_nQuestType = sbType;
	m_sbQuestFlag = sbQuestFlag;
	m_bEmpty = FALSE;
	m_betType = UBET_QUEST;

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_QUEST )
		return;

	// Set texture data
	m_nTextureID = 0;
	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_QUEST, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_QUEST, m_nTextureID );

	int	nTexRow = ( nIndex - 1 ) / 4;
	int	nTexCol = ( nIndex - 1 ) % 4;

	if( nIndex == 1000 ) // 지불 아이콘으로 사용하는 Quest버튼 : 열라 하드...
	{
		nTexRow = 0;
		nTexCol = 4;
	}
	// 펫 관련 퀘스트...
	else if( nIndex == 4000 || nIndex == 4003 || nIndex == 4005)
	{
		nTexRow = 1;
		nTexCol = 1;
	}
	// 펫 관련 퀘스트...
	else if( nIndex == 4001 || nIndex == 4004 || nIndex == 4006)
	{
		nTexRow = 1;
		nTexCol = 1;
	}
	// 이벤트 버튼의 이미지.
	else if( nIndex >= EVENT_START_INDEX )
	{
		nTexRow = 1;
		nTexCol = 4;
	}

	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : SetEventInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetEventInfo( int nIndex )
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	// Set event information
	m_nIndex = nIndex;
	m_bEmpty = FALSE;
	m_betType = UBET_EVENT;

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_EVENT )
		return;

	// Set texture data
	m_nTextureID = 0;
	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_EVENT, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_EVENT, m_nTextureID );

	int	nTexRow = ( nIndex - 1 ) / 4;
	int	nTexCol = ( nIndex - 1 ) % 4;
	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : SetRemissionInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::SetRemissionInfo( SBYTE iRemissionType, SBYTE nTab, SBYTE nRow, SBYTE nCol, int nIndex, int nUniIndex, SBYTE nWearingType )
{
	//int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;
	SBYTE		sbOldType = m_sbRemissionType;
	
	// Set remission information
	m_nIndex	= nIndex;
	m_nUniIndex = nUniIndex;
	m_nWearType = nWearingType;
	m_nTab		= nTab;
	m_nRow		= nRow;
	m_nCol		= nCol;
	m_bEmpty	= FALSE;
	m_betType	= UBET_REMISSION;
	m_sbRemissionType = iRemissionType;
	
	// Texture data is already set
	if( sbOldType == iRemissionType && betOldType == UBET_REMISSION )
		return;
	
	// Set texture data
	m_nTextureID = 0;
	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_REMISSION, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_REMISSION, m_nTextureID );
	
	int	nTexRow = ( iRemissionType - 1 ) / 4;
	int	nTexCol = ( iRemissionType - 1 ) % 4;
	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : SetComboInfo()
// Desc : 미션 케이스 버튼 이미지
// ----------------------------------------------------------------------------
void CUIButtonEx::SetComboInfo( SBYTE nTab , SBYTE nRow , SBYTE nCol )
{
	int nUVSX = COMBO_BTN_SIZE-2;
	int nUVSY = COMBO_BTN_SIZE-2;

	m_nTextureID = nTab;
	int	nTexRow = nRow;
	int	nTexCol = nCol;
	m_bEmpty	= FALSE;
	m_betType	= UBET_COMBO;

	nUVSX *= nTexCol;
	nUVSY *= nTexRow;

	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_COMBO, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_COMBO, m_nTextureID );
	m_rtUV.SetUV( nUVSX, nUVSY, nUVSX + COMBO_BTN_SIZE-2, nUVSY + COMBO_BTN_SIZE-2, fTexWidth, fTexHeight );
}

void CUIButtonEx::SetAuctionNotice( int nIndex )
{
	int			nOldIndex = m_nIndex;
	UIBtnExType	betOldType = m_betType;

	m_nIndex = nIndex;
	m_bEmpty = FALSE;
	m_betType = UBET_AUCTION;

	// Texture data is already set
	if( nOldIndex == nIndex && betOldType == UBET_AUCTION )
		return;

	// Set texture data
	m_nTextureID = 0;
	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_AUCTION, m_nTextureID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_AUCTION, m_nTextureID );

	m_rtUV.SetUV( 569, 521, 603, 555, fTexWidth, fTexHeight );
}


void CUIButtonEx::CopyComboInfo(CUIButtonEx &btnSrc)
{
	m_nIndex = btnSrc.GetItemIndex();
		
	m_nTab = btnSrc.GetItemTab();
	m_nRow = btnSrc.GetItemRow();
	m_nCol = btnSrc.GetItemCol();

	m_uwBtnID = btnSrc.GetBtnID();
	m_nTextureID = btnSrc.GetTextureID();
	m_nWhichUI = btnSrc.GetWhichUI();
	m_betType = btnSrc.GetBtnType();
	m_bEmpty = btnSrc.IsEmpty();	
	m_rtUV = btnSrc.GetUV();
		
	m_dwWndState = btnSrc.GetWndState();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );
			
	DOUBLE	dDelayTime;
	DOUBLE	dElapsedTime;

	if( m_betType == UBET_SKILL )
	{
		CSkill	&rSkill = _pNetwork->GetSkillData( m_nIndex );
		dDelayTime = DOUBLE( rSkill.GetReUseTime() + _pNetwork->MyCharacterInfo.magicspeed ) / 10.0;
		int nCoolTimeReductionRate= _pUIMgr->GetCoolTimeReductionRate();
		if( nCoolTimeReductionRate >0)
			dDelayTime *= DOUBLE(100-nCoolTimeReductionRate)/100.0f;

		dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds() - rSkill.Skill_Data.Skill_StartTime;
	}
	// 광속 아이템일 경우 쿨타임 표시
	else if( m_nIndex ==2407 || m_nIndex ==2408 || m_nIndex == 2609 )
	{
		CItemData	&rItemData = _pNetwork->GetItemData( m_nIndex );

		CSkill	&rSkill = _pNetwork->GetSkillData( rItemData.GetNum0() );
		dDelayTime = DOUBLE( rSkill.GetReUseTime() ) / 10.0;
		dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds() - rItemData.StartTime;
	}
	else if( m_betType == UBET_ITEM  && m_nCashIndex == -1)
	{
		CItemData	&rItemData = _pNetwork->GetItemData( m_nIndex );
		if( (rItemData.GetType() != CItemData::ITEM_POTION ) && (rItemData.GetSubType() != CItemData::ITEM_SUB_TARGET))  
		{
			RenderDefaultButton();
			return;
		}

		CSkill	&rSkill = _pNetwork->GetSkillData( rItemData.GetNum0() );
		dDelayTime = DOUBLE( rSkill.GetReUseTime() ) / 10.0;
		dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds() - rItemData.StartTime;
	}
	else if(m_betType == UBET_AUCTION)
	{
		_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID,
												nX, nY, nX + m_nWidth, nY + m_nHeight,//old
												m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,//old
												0xFFFFFFFF );
		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_BLEND );
		
		if( m_bsState == UBES_CLICK )
		{

			// Add render regions
			_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID,
													nX+1, nY+1, nX + m_nWidth-1, nY + m_nHeight-1,//old
													m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,//old
													0x969696FF );

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_ADD );
		}
	}
	else
	{
		RenderDefaultButton();
		return;
	}

	// If button is skill and is delay state
	if( m_bSkillDelay && ( !m_bSpecial || m_betType == UBET_ITEM ) )
	{
		if( dElapsedTime > dDelayTime - 0.5 )
		{
			float fDelta;
			
			fDelta = ( dElapsedTime - dDelayTime ) * -10;

			if( fDelta < 0 || fDelta > 5 ) 
			{
				fDelta = 0;
			}
			// Add render regions
			_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID, nX - fDelta, nY - fDelta, nX + m_nWidth + fDelta *2, nY + m_nHeight + fDelta * 2,
													m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,
													0xFFFFFFFF );

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_BLEND );

//			if( fDelta < 4 )//4
//			{
//				// Render all button elements
//				_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_MULTIPLY );	
//			}
//			if( fDelta < 5 )//5
//			{
//				// Render all button elements
//				_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_MULTIPLY );	
//			}

			// Add render regions
			_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID, nX - fDelta, nY - fDelta, nX + m_nWidth + fDelta *2, nY + m_nHeight + fDelta * 2,
													m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,
													0x808080FF );

		

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_ADD );

			// Reset
			if( dElapsedTime > dDelayTime )
				m_bSkillDelay = FALSE;
		}
		else
		{
			// Add render regions
			_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID, nX, nY, nX + m_nWidth, nY + m_nHeight,
													m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,
													0x646464FF );

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_BLEND );

			// Delay animation
			FLOAT	fDelayRatio = 1.0f - FLOAT( dElapsedTime / ( dDelayTime - 0.3) );
			FLOAT	fSizeOffset = FLOAT(BTN_SIZE / 2) * fDelayRatio;
			FLOAT	fUOffset = ( m_rtUV.U1 - m_rtUV.U0 ) * 0.5f * fDelayRatio;
			FLOAT	fVOffset = ( m_rtUV.V1 - m_rtUV.V0 ) * 0.5f * fDelayRatio;
			FLOAT	fX0 = FLOAT(nX) + fSizeOffset;
			FLOAT	fY0 = FLOAT(nY) + fSizeOffset;
			FLOAT	fX1 = FLOAT(nX + m_nWidth) - fSizeOffset;
			FLOAT	fY1 = FLOAT(nY + m_nHeight) - fSizeOffset;

			// Add render regions
			_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID, fX0, fY0, fX1, fY1,
													m_rtUV.U0 + fUOffset, m_rtUV.V0 + fVOffset,
													m_rtUV.U1 - fUOffset, m_rtUV.V1 - fVOffset,
													0xDFCFCFFF );

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_ADD );
		}
	}
	else
	{
		RenderDefaultButton();
		return;
	}
}

//------------------------------------------------------------------------------
// CUIButtonEx::RenderDefaultButton
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIButtonEx::RenderDefaultButton()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CItemData	&rItemData = _pNetwork->GetItemData( m_nIndex );
	// 플러스 수치 표시
	if(m_ulPlus > 0 && (rItemData.GetType() == CItemData::ITEM_ETC || ( rItemData.GetType() == CItemData::ITEM_ONCEUSE &&
		rItemData.GetSubType() != CItemData::ITEM_ETC_EVENT && rItemData.GetSubType() != CItemData::ITEM_SUB_BOX)) )
	{ // 월드컵 카드로 인해 조건 추가, 일회용 상자 제외
		CTString tv_plus;
		tv_plus.PrintF("+%d", m_ulPlus);

		_pUIMgr->GetDrawPort()->PutTextEx( 
			tv_plus, 
			nX+17,
			nY+17, 
			0xFF0000FF );
	}

	// Add render regions
	_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID,
											nX, nY, nX + m_nWidth, nY + m_nHeight,//old
											m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,//old
											0xFFFFFFFF );
	if( m_bsState == UBES_CLICK )
	{
		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_BLEND );

		// Add render regions
		_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID,
												nX+1, nY+1, nX + m_nWidth-1, nY + m_nHeight-1,//old
												m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,//old
												0x969696FF );

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, PBT_ADD );
	}
}

// ----------------------------------------------------------------------------
// Name : RenderHighlight()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonEx::RenderHighlight( const COLOR colHighlight, const ULONG ulPBT/* = PBT_ADD*/ )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	_pUIMgr->GetDrawPort()->AddBtnTexture( m_nTextureID, nX, nY, nX + m_nWidth, nY + m_nHeight,
											m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,
											colHighlight );

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_betType, ulPBT );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIButtonEx::MouseMessage( MSG *pMsg )
{
	// If button is disabled
	if( !IsEnabled() || IsEmpty() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				if( !( pMsg->wParam & MK_LBUTTON ) )
					m_bLButtonDown = FALSE;

				if( m_bLButtonDown )
					m_bsState = UBES_CLICK;
				else
					m_bsState = UBES_IDLE;

				m_bShow = TRUE;

				return WMSG_SUCCESS;
			}
			m_bShow = FALSE;
			m_bsState = UBES_IDLE;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				m_bsState = UBES_CLICK;
				m_bLButtonDown = TRUE;
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			m_bLButtonDown = FALSE;

			if( IsInside( nX, nY ) )
			{
				if( m_bsState == UBES_CLICK )
				{
					m_bsState = UBES_IDLE;
					return WMSG_COMMAND;
				}
				else
				{
					m_bsState = UBES_IDLE;
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				m_bsState = UBES_IDLE;
				return WMSG_COMMAND;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

extern INDEX g_iCountry;
static int	_iMaxStringChar = 26;
void CUIButtonEx::AddInfoString( CTString &strInfo, COLOR colInfo )
{
	if( m_nCurInfoLines >= 20 )
		return;

	// Get length of string
	INDEX	nLength = strInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strInfo);
		INDEX	nChatMax= (_iMaxStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			m_strButtonInfo[m_nCurInfoLines] = strInfo;
			m_colButtonInfo[m_nCurInfoLines++] = colInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strInfo.Split( nSplitPos, m_strButtonInfo[m_nCurInfoLines], strTemp );
			m_colButtonInfo[m_nCurInfoLines++] = colInfo;

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

			AddInfoString( strTemp, colInfo );

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxStringChar )
		{
			m_strButtonInfo[m_nCurInfoLines] = strInfo;
			m_colButtonInfo[m_nCurInfoLines++] = colInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strInfo.Split( nSplitPos, m_strButtonInfo[m_nCurInfoLines], strTemp );
			m_colButtonInfo[m_nCurInfoLines++] = colInfo;

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

			AddInfoString( strTemp, colInfo );
		}
	}
}

tm* gt_tv_t = NULL; // 기간 표시용 전역 포인터 변수
#define IN_VALIDTM(a) if (a) {delete a; a=NULL;}
void CUIButtonEx::GetButtonInfo(/*CTString *strInfo, COLOR *colInfo,*/ int &nInfoWidth, int &nInfoHeight)
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	switch(m_betType)
	{
	case UBET_ITEM:
		{
			CItemData &rItemData = _pNetwork->GetItemData(m_nIndex);
			CItemRareOption rItemRareOption = _pNetwork->GetRareOptionData(0);
			int iRareGrade = -1;
			BOOL bRareItem = FALSE;
			int nItemleveldown = 0;
			if(  rItemData.IsFlag(ITEM_FLAG_RARE))
			{			
				bRareItem = TRUE;
				INDEX iRare = m_uwRareIndex;
				rItemRareOption =_pNetwork->GetRareOptionData(iRare);
				iRareGrade = rItemRareOption.GetGrade();
			}
			else
			{
				bRareItem = FALSE;
				iRareGrade =-1;
			}

			const CTString strItemName =_pNetwork->GetItemName( rItemData.GetItemIndex() );

			CTString szItemName =strItemName;
			if( bRareItem )
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
				
				strCount.PrintF( "%I64d", m_llCount );
				_pUIMgr->InsertCommaToString( strCount );
				strTemp.PrintF( "%s(%s)", szItemName, strCount );

				colNas = _pUIMgr->GetNasColor( m_llCount );		

				AddInfoString( strTemp, colNas );
			}
			else
			{
				if( ( ( rItemData.GetType() == CItemData::ITEM_WEAPON && !rItemData.IsProductionItem() )||
					rItemData.GetType() == CItemData::ITEM_SHIELD ) )
				{
					if(m_ulFlag&(PLATINUM_MAX_PLUS) || m_ulFlag&(FLAG_ITEM_COMPOSITION) || 
						rItemData.IsFlag(ITEM_FLAG_COMPOSITE) || rItemData.IsFlag(ITEM_FLAG_CASH))
					{
					
						if(m_ulPlus > 0){
							
							int plat_plus,diff;
							FLAG_ITEM_PLATINUM_GET(m_ulFlag,plat_plus);
							diff = m_ulPlus - plat_plus;

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
								
						//	AddInfoString( strTemp, colNas );
										
						}
						else {
							strTemp.PrintF( "%s",szItemName);
						//	AddInfoString( strTemp );
						}

						AddInfoString( strTemp, iRareGrade >= 0 ? RareItem_Name_Color[iRareGrade] : colNas);
						
						CTString strTime1, strTime2;

						if (rItemData.IsFlag(ITEM_FLAG_COMPOSITE))
						{
							strTime1 = _S(3219, "의상만료 : %d년%d월%d일%d시");
						}

						if(m_ulFlag&(FLAG_ITEM_COMPOSITION))
						{
							strTemp.PrintF( _S( 2706, "능력치 결합 효과"));
							AddInfoString( strTemp, 0xFFAA44FF );
							strTime2 = _S(3220, "결합만료 : %d년%d월%d일%d시");

						}
						else if(m_ulFlag&(PLATINUM_MAX_PLUS)){
								strTemp.PrintF( _S( 2675, "플레티늄 제련 효과"));
								AddInfoString( strTemp, 0xFFAA44FF );

								strTime1 = _S(2525,"만료 : %d년%d월%d일%d시");
								strTime2 = CTString("");
						}else if(rItemData.IsFlag(ITEM_FLAG_CASH))
						{
							strTime1 = _S(2525,"만료 : %d년%d월%d일%d시");
							strTime2 = CTString("");
						}

						if (m_ulUsed > 0)
						{
							time_t tv_used = m_ulUsed - _pUIMgr->GetRadar()->GetStartTime();
							gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);				
							strTemp.PrintF(strTime1,gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
							AddInfoString( strTemp, 0xFFAA44FF );
							IN_VALIDTM(gt_tv_t)
						}

						// ITEM_FLAG_ABS 플래그가 있는 아이템은 Item_Used2에 남은시간이 있다.(기간이 아닌 시간(초))
						// 현재는 Item_Used만으로 표시한다.
						if (m_ulUsed2 > 0 && !rItemData.IsFlag(ITEM_FLAG_ABS))
						{
							if ((m_ulUsed2 - m_ulUsed) < 0)
							{
								time_t tv_used = m_ulUsed2 - _pUIMgr->GetRadar()->GetStartTime();
								gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);				
								strTemp.PrintF(strTime2,gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
								AddInfoString( strTemp, 0xFFAA44FF );
								IN_VALIDTM(gt_tv_t)
							}
						}
					}
					else if(m_ulFlag&(FLAG_ITEM_LENT))
					{
						CTString tv_str=_S(3046,"대여");
						tv_str+= CTString(" ")+szItemName;
						if(m_ulPlus > 0){
							strTemp.PrintF( "%s +%d", tv_str, m_ulPlus );
							AddInfoString( strTemp, colNas );
						}
						else {
							strTemp.PrintF( "%s",tv_str);
							AddInfoString( strTemp );
						}
						if(m_ulUsed > 0)
						{
							time_t tv_used = m_ulUsed - _pUIMgr->GetRadar()->GetStartTime();
							gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
					
							AddInfoString( strTemp, 0xFFAA44FF );
							
							IN_VALIDTM(gt_tv_t)
						}
					}
					else if (m_ulFlag&(FLAG_ITEM_LEVELDOWN))
					{
						strTemp = _S(3343, "아이템 레벨제한 감소효과");
						AddInfoString(strTemp, 0xFFAA44FF);
						nItemleveldown = 6;

						if (m_ulUsed2 > 0)
						{
							time_t tv_used = m_ulUsed2 - _pUIMgr->GetRadar()->GetStartTime();
							gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
					
							AddInfoString( strTemp, 0xFFAA44FF );
							
							IN_VALIDTM(gt_tv_t)
						}
					}
					else {
						if(m_ulPlus > 0){
							strTemp.PrintF( "%s +%d", szItemName, m_ulPlus );
						//	AddInfoString( strTemp, colNas );
						}
						else {
							strTemp.PrintF( "%s",szItemName);
						//	AddInfoString( strTemp );
						}
						AddInfoString( strTemp, iRareGrade >= 0 ? RareItem_Name_Color[iRareGrade] : colNas);

					}
				}
				else if( rItemData.GetType() == CItemData::ITEM_ACCESSORY)
				{
					if( rItemData.GetSubType() ==CItemData::ACCESSORY_PET)
					{
						const INDEX iPetIndex = m_ulPlus;
						SBYTE sbPetTypeGrade =_pUIMgr->GetPetInfo()->GetPetTypeGrade( iPetIndex );

						if( sbPetTypeGrade >0 )
						{
							INDEX iPetType;
							INDEX iPetAge;
							_pNetwork->CheckPetType( sbPetTypeGrade, iPetType, iPetAge );
							szItemName +=CTString(" - ")+PetInfo().GetName(iPetType, iPetAge);
						}
					}

				//	AddInfoString( szItemName, colNas );
					AddInfoString( szItemName, iRareGrade >= 0 ? RareItem_Name_Color[iRareGrade] : colNas);
				}
				else
				{
					if( m_ulFlag&( FLAG_ITEM_BOOSTER_ADDED ) )
					{
						strTemp.PrintF(_S( 1929, "부스트 %s" ), szItemName);	
					}
					else if( m_ulFlag&( FLAG_ITEM_SILVERBOOSTER_ADDED ) )
					{
						strTemp.PrintF(_S( 2429, "실버 부스트 %s" ), szItemName);	
					}
					else if( m_ulFlag&( FLAG_ITEM_GOLDBOOSTER_ADDED ) )
					{
						strTemp.PrintF(_S( 2430,"골드 부스트 %s" ), szItemName);	
					}
					else if( m_ulFlag&( FLAG_ITEM_PLATINUMBOOSTER_ADDED ) )
					{
						strTemp.PrintF(_S( 2639, "플래티늄 부스트 %s" ), szItemName);	
					}
					else
					{
						strTemp.PrintF("%s", szItemName);
					}
					
					AddInfoString( strTemp, colNas );
				}
			}
			
			// 기간제 악세사리와 광속 아이템의 경우 만료 일자 표시
			if(rItemData.GetType() == CItemData::ITEM_ACCESSORY
				|| rItemData.GetItemIndex()==2407 || rItemData.GetItemIndex()==2408 || rItemData.GetItemIndex()==2609)
			{
				if(m_ulUsed > 0)
				{
					//if ( g_iCountry == TAIWAN || g_iCountry == TAIWAN2 )
					{// 대만 라카력 적용
						time_t tv_used = m_ulUsed - _pUIMgr->GetRadar()->GetStartTime();
						gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);
						strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
						IN_VALIDTM(gt_tv_t)
					}
								
					AddInfoString( strTemp, 0xFFAA44FF );
				}
			}

			switch( rItemData.GetType() )
			{
			case CItemData::ITEM_WEAPON:		// Weapon item
				{
					//초고급 제련석 여부
					//TEMP : 초고급 제련석 관련
					//대여 아이템 추가...
					if(m_ulFlag&(FLAG_ITEM_LENT))
					{
						CTString strLeaseItem(_S(3047,"제련석 사용불가" ));
						AddInfoString( strLeaseItem, 0xFF4040FF);

					}
					else if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) ) // 업 그레이드 가능 아이템 
					{
						// 초고제 사용할 수 있을 때만 표시
						BOOL bCanUseSuperGOJE = !(m_ulFlag&FLAG_ITEM_SUPER_STONE_USED );
						CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
						if( bCanUseSuperGOJE )
							AddInfoString(strCanUseSuperGOJE, 0xFFFF40FF);
					}

					// Class
					CUIManager::GetClassOfItem( rItemData, strTemp );
					AddInfoString( strTemp, 0xFFFFFFFF );

					//레벨에 상관없이 흰색으로 표시
					int	nItemLevel = rItemData.GetLevel();

					int nWearLevelReduction =GetWearLevelReduction();

					if( nWearLevelReduction >0 || nItemleveldown > 0 )
						strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction+nItemleveldown );
					else
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					
					if (nItemleveldown > 0) { AddInfoString(strTemp, 0x1C54CC); }
					else { AddInfoString( strTemp,	0xFFFFFFFF ); }



					int	nPlusValue;
					int nBasePhysicalAttack =rItemData.GetPhysicalAttack() +rItemRareOption.GetPhysicalAttack();
					int nBaseMagicAttack =rItemData.GetMagicAttack() +rItemRareOption.GetMagicAttack();
					if( m_ulPlus > 0 )
					{
						// Physical attack
						if( rItemData.GetPhysicalAttack() > 0 )
						{
							nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalAttack(), m_ulPlus );
							if( nPlusValue > 0 )
								strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), nBasePhysicalAttack, nPlusValue );
							else
								strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack);
																
							AddInfoString( strTemp, 0xFFFFFFFF );	//흰색으로 표시

							if( m_ulPlus >= 15 )
							{
								strTemp.PrintF(_S( 1891, "물리 공격력 + 75"));
								AddInfoString( strTemp, 0xFFFFFFFF );	//흰색으로 표시
							}
						}

						// Magic attack
						if( rItemData.GetMagicAttack() > 0 )
						{
							nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicAttack(), m_ulPlus );
							if( nPlusValue > 0 )
								strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), nBaseMagicAttack, nPlusValue );
							else
								strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack);

							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

							if( m_ulPlus >= 15 )
							{
								strTemp.PrintF(_S(1892,"마법 공격력 + 50"));
								AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
							}
						}
					}
					else
					{
						// Physical attack
						if( rItemData.GetPhysicalAttack() > 0 )
						{
							strTemp.PrintF( _S( 161, "공격력 : %d" ), nBasePhysicalAttack);						
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
						}

						// Magic attack
						if( rItemData.GetMagicAttack() > 0 )
						{
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), nBaseMagicAttack );
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
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
					if( nPenalty > 0 && !bRareItem)
					{
						strTemp.PrintF( _S( 1029, "패널티 : 공격력 %d%% 감소" ), nPenalty );		
						AddInfoString( strTemp, 0xE53535FF );
					}				
				
					if(rItemData.GetSubType() == CItemData::ITEM_WEAPON_MINING 
						|| rItemData.GetSubType() == CItemData::ITEM_WEAPON_CHARGE
						|| rItemData.GetSubType() == CItemData::ITEM_WEAPON_GATHERING)
					{
						strTemp.PrintF(  _S( 510, "내구도 : %ld" ), m_ulUsed);		
						AddInfoString( strTemp, 0xDEC05BFF );
					}
				}
				break;

			case CItemData::ITEM_SHIELD:		// Shield item
				{
					//초고급 제련석 제련석 여부
					//TEMP : 초고급 제련석 관련
					if( rItemData.IsFlag( ITEM_FLAG_UPGRADE ) ) // 업 그레이드 가능 아이템 
					{
						// 초고제 사용할 수 있을 때만 표시
						BOOL bCanUseSuperGOJE = !(m_ulFlag&FLAG_ITEM_SUPER_STONE_USED );
						CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
					
						if( bCanUseSuperGOJE )
							AddInfoString(strCanUseSuperGOJE, 0xFFFF40FF );
					}
					
					// Class
					CUIManager::GetClassOfItem( rItemData, strTemp );				
					AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

					int	nItemLevel = rItemData.GetLevel();

					int nWearLevelReduction = GetWearLevelReduction();

					if( nWearLevelReduction >0 || nItemleveldown > 0 )
						strTemp.PrintF( _S( 160, "레벨: %d" ) +" (-%d)", nItemLevel, nWearLevelReduction+nItemleveldown );
					else
						strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );

					if (nItemleveldown > 0) { AddInfoString(strTemp, 0x1C54CC); }
					else { AddInfoString( strTemp,	0xFFFFFFFF ); }

					int	nPlusValue;
					int nBasePhysicalDefence =rItemData.GetPhysicalDefence() +rItemRareOption.GetPhysicalDefence();
					int nBaseMagicDefence =rItemData.GetMagicDefence() +rItemRareOption.GetMagicDefence();
					if( m_ulPlus > 0 )
					{
						// Physical defense
						if( rItemData.GetPhysicalDefence() > 0 )
						{							
							nPlusValue = CItems::CalculatePlusDamage( rItemData.GetPhysicalDefence(), m_ulPlus );
							if( nPlusValue > 0 )
								strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), nBasePhysicalDefence, nPlusValue );
							else
								strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );
																
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

							if( m_ulPlus >= 15 )
							{
								strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));										
								AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

								strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
								AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
							}
						}

						// Magic defense
						if( rItemData.GetMagicDefence() > 0 )
						{
							nPlusValue = CItems::CalculatePlusDamage( rItemData.GetMagicDefence(), m_ulPlus );
							if( nPlusValue > 0 )
								strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), nBaseMagicDefence, nPlusValue );
							else
								strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
														
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

							if( m_ulPlus >= 15 )
							{
								strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));		
								AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시

								strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
								AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
							}
						}
					}
					else
					{
						// Physical defense
						if( rItemData.GetPhysicalDefence() > 0 )
						{
							strTemp.PrintF( _S( 163, "방어력 : %d" ), nBasePhysicalDefence );
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
						}

						// Magic defense
						if( rItemData.GetMagicDefence() > 0 )
						{
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), nBaseMagicDefence );
							AddInfoString( strTemp, 0xFFFFFFFF );		//흰색으로 표시
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
					if( nPenalty > 0 && !bRareItem)
					{
						strTemp.PrintF( _S( 1030, "패널티 : 방어력 %d%% 감소" ), nPenalty );		
						AddInfoString( strTemp, 0xE53535FF );
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
						if( _pUIMgr->GetPetInfo()->GetPetExchangeInfo( m_ulPlus, pPetExchangeInfo ) )
						{
							if( pPetExchangeInfo.strNameCard.Length() >0)
								AddInfoString( pPetExchangeInfo.strNameCard, 0xF2F2F2FF );
							AddInfoString( pPetExchangeInfo.strLevel, 0xDEC05BFF );		
							AddInfoString( pPetExchangeInfo.strHP, 0xDEC05BFF );		
							AddInfoString( pPetExchangeInfo.strlExp, 0xDEC05BFF );		
							AddInfoString( pPetExchangeInfo.strHungry, 0xDEC05BFF );		
							AddInfoString( pPetExchangeInfo.strSympathy, 0xDEC05BFF );		
							AddInfoString( pPetExchangeInfo.strAbility, 0xDEC05BFF );		
	#ifdef PET_SEAL_TIME
							AddInfoString( pPetExchangeInfo.strRemainTime, 0xDEC05BFF );
	#endif
						}
						
					}
					else if(rItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
					{
						sPetItem_Info pWildPetInfo;

						if(_pUIMgr->GetWildPetInfo()->GetWildPetInfo(m_ulPlus,pWildPetInfo))
						{
							strTemp.PrintF(_S(4215, "이름: %s"),pWildPetInfo.pet_name);
							AddInfoString(strTemp,  0xF2F2F2FF );
							strTemp.PrintF(_S(4216, "펫 레벨: %d"),pWildPetInfo.pet_level);
							AddInfoString(strTemp, 0xDEC05BFF);
							strTemp.PrintF(_S(4217, "힘: %d"),pWildPetInfo.pet_str);
							AddInfoString(strTemp, 0xDEC05BFF);
							strTemp.PrintF(_S(4218, "민첩: %d"),pWildPetInfo.pet_dex);
							AddInfoString(strTemp, 0xDEC05BFF);
							strTemp.PrintF(_S(4219, "지혜: %d"),pWildPetInfo.pet_int);
							AddInfoString(strTemp, 0xDEC05BFF);
							strTemp.PrintF(_S(4220, "체력: %d"),pWildPetInfo.pet_con);
							AddInfoString(strTemp, 0xDEC05BFF);
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
							AddInfoString( strTemp, 0xDEC05BFF );
							
							const int iMinLevel = CQuestSystem::Instance().GetQuestMinLevel( iQuestIndex );
							const int iMaxLevel = CQuestSystem::Instance().GetQuestMaxLevel( iQuestIndex );

							// 레벨 제한 출력.
							strTemp.PrintF( _S( 1660, "레벨 제한 : %d ~ %d" ), iMinLevel, iMaxLevel );		
							AddInfoString( strTemp, 0xDEC05BFF );
						}
					}
					
					if ( rItemData.GetSubType() == CItemData::ITEM_SUB_WARP || rItemData.GetSubType() == CItemData::ITEM_SUB_CASH_ITEM)
					{
						if(m_ulUsed > 0)
						{
							time_t tv_used = m_ulUsed - _pUIMgr->GetRadar()->GetStartTime();
							gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);

							
							strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
							AddInfoString( strTemp, 0xDEC05BFF );
							IN_VALIDTM(gt_tv_t)
						}
					}

				}
				break;

			case CItemData::ITEM_POTION:	// Date : 2005-01-07,   By Lee Ki-hwan
				{

					// Date : 2005-01-14,   By Lee Ki-hwan
					
					if ( rItemData.GetSubType() == CItemData::POTION_UP )
					{
						if( m_ulFlag > 0 )
						{
							// Level
							strTemp.PrintF( _S( 160, "레벨: %d" ), m_ulFlag );
							AddInfoString( strTemp, 0xD28060FF );

							// 향상 타입
							int nSkillType = rItemData.GetSkillType();
							CSkill	&rSkill = _pNetwork->GetSkillData( nSkillType );
							int Power = rSkill.GetPower( m_ulFlag - 1);


							if(  rItemData.GetNum1() == CItemData::POTION_UP_PHYSICAL ) // 물리
							{
								if(  rItemData.GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
								{
									strTemp.PrintF ( _S( 790, "물리 공격력 +%d 상승" ), Power );
									AddInfoString( strTemp, 0xDEC05BFF );
								}
								else if( rItemData.GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
								{
									strTemp.PrintF ( _S( 791, "물리 방어력 +%d 상승" ),  Power );
									AddInfoString( strTemp, 0xDEC05BFF );
								}

							}
							else if( rItemData.GetNum1() == CItemData::POTION_UP_MAGIC ) // 마법
							{
								if(  rItemData.GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
								{
									strTemp.PrintF ( _S( 792, "마법 공격력 +%d 상승" ),  Power );
									AddInfoString( strTemp, 0xDEC05BFF );
								}
								else if( rItemData.GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
								{
									strTemp.PrintF ( _S( 793, "마법 방어력 +%d 상승" ),  Power );
									AddInfoString( strTemp, 0xDEC05BFF );
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
							if( m_ulFlag&( FLAG_ITEM_OPTION_ENABLE ) )
							{
								m_ulFlag ^= FLAG_ITEM_OPTION_ENABLE;
							}

							// Level
							if( m_ulFlag > 0 )
							{
								strTemp.PrintF( _S( 160, "레벨: %d" ), m_ulFlag );
								AddInfoString( strTemp, 0xD28060FF );
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
							if( m_nIndex == 1483 )
							{// 월드컵 카드
								ULONG ulTemp = m_ulPlus;

								int nScoreA = static_cast<int>( ( ulTemp & 0xFFFF0000 ) >> 16 );
								int nScoreB = static_cast<int>( ulTemp & 0x0000FFFF );
								
								strTemp.PrintF( "%s (%d) VS (%d) %s", _pUIMgr->GetQuest()->GetStrTeamA(), nScoreA,
									nScoreB, _pUIMgr->GetQuest()->GetStrTeamB() );

								AddInfoString( strTemp, 0xFFAA44FF );
							}
						}
						break;
					case CItemData::ITEM_PET_AI:
						{
							if(m_ulUsed > 0)
							{
								time_t tv_used = m_ulUsed - _pUIMgr->GetRadar()->GetStartTime();
								gt_tv_t = _pUIMgr->LClocaltime((time_t*)&tv_used);

								
								strTemp.PrintF(  _S( 2525,"만료 : %d년%d월%d일%d시"),gt_tv_t->tm_year+1,gt_tv_t->tm_mon+1,gt_tv_t->tm_yday+1,gt_tv_t->tm_hour);
								AddInfoString( strTemp, 0xDEC05BFF );
								IN_VALIDTM(gt_tv_t)
							}

						}break;
					}
				}
				break;
			}

			// Weight
			strTemp.PrintF( _S( 165, "무게 : %d" ), rItemData.GetWeight() );
			AddInfoString( strTemp, 0xDEC05BFF );


			const int iFame = rItemData.GetFame();
			if( iFame > 0 )
			{
				strTemp.PrintF( _S( 1096, "명성 %d 필요" ), iFame );		
				AddInfoString( strTemp, 0xDEC05BFF );
			}

			if( bRareItem )
			{
				if( m_uwRareIndex ==0 )
					AddInfoString( _S(3165, "<미공개 옵션>"), 0xFF4040FF );
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
						sbOptionType = m_sbOptionType[sbOption];
						sbOptionLevel = m_sbOptionLevel[sbOption];

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
						//case 55:		//크리티컬확률 증가
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

						AddInfoString( strTemp, 0x94B7C6FF );
					}
					if( m_ulFlag&( FLAG_ITEM_OPTION_ENABLE ) )
					{
						AddInfoString( _S( 511, "블러드 옵션 가능" ), 0xE53535FF );		
					}
					if( m_ulFlag&( FLAG_ITEM_SEALED ) )
					{
						AddInfoString(  _S( 512, "봉인된 아이템" ), 0xE53535FF );		
					}
				}
				break;
			}

			// Description
			const char	*pDesc = _pNetwork->GetItemDesc( rItemData.GetItemIndex() );
			if( pDesc[0] != NULL )
			{
				strTemp.PrintF( "%s", pDesc );
				AddInfoString( strTemp, 0x9E9684FF );
			}

		}break;
	case UBET_SKILL:
		{
			if(!m_bSpecial)
			{		
				CSkill	&rSelSkill = _pNetwork->GetSkillData( m_nIndex );
				// Get skill name
				strTemp.PrintF( "%s Lv %d", rSelSkill.GetName(), m_nSkillLevel );
				AddInfoString( strTemp );

				// Get skill type etc...
				//m_nSkillLevel--;
				switch( rSelSkill.GetType() )
				{
				case CSkill::ST_MELEE:
				case CSkill::ST_RANGE:
				case CSkill::ST_MAGIC:
					{
						if( rSelSkill.GetFlag() & SF_SINGLEMODE )
							AddInfoString( _S( 499, "퍼스널던전 전용 스킬" ), 0xCACACAFF );		
						else
							AddInfoString( _S( 63, "액티브 스킬" ), 0xCACACAFF );

						int	nNeedMP = rSelSkill.GetNeedMP( m_nSkillLevel-1 );
						int	nNeedHP = rSelSkill.GetNeedHP( m_nSkillLevel-1 );
						if( nNeedHP == 0 )
						{
							if( nNeedMP != 0 )
							{
								if( _pUIMgr->GetNeedMPReductionRate() >0)
								{
									int nNeedMPReduction = (nNeedMP*_pUIMgr->GetNeedMPReductionRate())/100;
									strTemp.PrintF( _S( 64, "소모 MP : %d" )+" (-%d)", nNeedMP, nNeedMPReduction );
								}
								else
									strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
								AddInfoString( strTemp, 0x94B7C6FF );
							}
						}
						else
						{
							if( nNeedMP == 0 )
							{
								strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
								AddInfoString( strTemp, 0x94B7C6FF );
							}
							else
							{
								if( _pUIMgr->GetNeedMPReductionRate() >0)
								{
									int nNeedMPReduction = (nNeedMP*_pUIMgr->GetNeedMPReductionRate())/100;
									strTemp.PrintF( _S( 64, "소모 MP : %d" )+" (-%d)", nNeedMP, nNeedMPReduction );
								}
								else
									strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
								AddInfoString( strTemp, 0x94B7C6FF );
								strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
								AddInfoString( strTemp, 0x94B7C6FF );
							}
						}

						if( rSelSkill.GetPower( m_nSkillLevel-1 ) > 0 )
						{
							strTemp.PrintF( _S( 65, "위력 : %d" ), rSelSkill.GetPower( m_nSkillLevel-1 ) );
							AddInfoString( strTemp, 0x94B7C6FF );
						}
						strTemp.PrintF( _S( 66, "유효 거리 : %.1f" ), rSelSkill.GetFireRange() );
						AddInfoString( strTemp, 0x94B7C6FF );
					}
					break;

				case CSkill::ST_PASSIVE:
					AddInfoString( _S( 67, "패시브 스킬" ), 0xCACACAFF );
					break;
				}

				const char	*pDesc = rSelSkill.GetDescription();
				if( pDesc != NULL )
				{
					strTemp.PrintF( "%s", pDesc );
					AddInfoString( strTemp, 0x9E9684FF );
				}
			}
			// Special Skill
			else
			{
				CSpecialSkill	&rSelSkill = _pNetwork->GetSSkillData( m_nIndex );		
				// Get skill name
				strTemp.PrintF( "%s Lv %d", rSelSkill.GetName(), m_nSkillLevel );
				AddInfoString( strTemp );

				// Get skill type etc...
				m_nSkillLevel--;		
				AddInfoString( _S( 67, "패시브 스킬" ), 0xCACACAFF );
				const char	*pDesc = rSelSkill.GetDescription();
				if( pDesc != NULL )
				{
					strTemp.PrintF( "%s", pDesc );
					AddInfoString( strTemp, 0x9E9684FF );
				}
			}

		}break;
		case UBET_ACTION:
		{
			CAction	&rSelAction = _pNetwork->GetActionData(m_nIndex);

			strTemp.PrintF("%s",rSelAction.GetName());
			AddInfoString(strTemp);
			const char	*pDesc = rSelAction.GetDesc();
			if( pDesc != NULL )
			{
				strTemp.PrintF( "%s", pDesc );
				AddInfoString( strTemp, 0x9E9684FF );
			}

		}break;
	}

	if(m_eCharType > CHAR_NAME_NONE)
	{
		switch(m_eCharType)
		{
			case CHAR_NAME_BUYER:
				strTemp.PrintF(_S(4380, "구매자 : %s"), m_strCharName);
				break;
			case CHAR_NAME_SELLER:
				strTemp.PrintF(_S(4381, "판매자 : %s"), m_strCharName);
				break;
			case CHAR_NAME_OWNER:
				strTemp.PrintF(_S(4382, "소유자 : %s"), m_strCharName);
				break;
		}	
		AddInfoString( strTemp, 0xE53535FF );
	}
//	strInfo = m_strButtonInfo;
//	colInfo	= m_colButtonInfo;
	nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + _iMaxStringChar *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + (m_nCurInfoLines+1) * _pUIFontTexMgr->GetLineHeight();

	
}

int CUIButtonEx::GetWearLevelReduction()
{
	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{
		int sbOptionType = (int)m_sbOptionType[sbOption];
		int sbOptionLevel = (int)m_sbOptionLevel[sbOption];
		if( sbOptionType == -1 || sbOptionLevel == 0 )
			break;

		//착용제한레벨 다운 옵션
		if( sbOptionType==49)
		{
			COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType );
			return odItem.GetValue( sbOptionLevel - 1 );
		}
	}
	
	return 0;
}

void CUIButtonEx::RenderInfoPopup()
{
		// Get position
	int	nInfoX;
	int nInfoWidth, nInfoHeight;
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdAddTexture );

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	if(!m_bShow) return;

	GetButtonInfo(nInfoWidth,nInfoHeight);

	nInfoX = (nX+16)-nInfoWidth/2;

	if(m_strButtonInfo[0] == "") return;

// 	if(bShow && m_bsState == UBES_IDLE)
// 	{
// 		_pUIMgr->GetDrawPort()->AddTexture(nInfoX,nY-20,nInfoX+nInfoWidth,nY,
// 			m_rtPopupName.U0,m_rtPopupName.V0,m_rtPopupName.U1,m_rtPopupName.V1,0xFFFFFFFF);
// 		
// 		_pUIMgr->GetDrawPort()->PutTextExCX(m_strButtonInfo[0],nX+16,nY-16,m_colButtonInfo[0]);
// 
// 	}else if(bShow && m_bsState == UBES_CLICK)
// 	{
		_pUIMgr->GetDrawPort()->AddTexture(nInfoX,nY-nInfoHeight,nInfoX+nInfoWidth,nY,
			m_rtPopupInfo.U0,m_rtPopupInfo.V0,m_rtPopupInfo.U1,m_rtPopupInfo.V1,0xFFFFFFFF);
		for (int i = 0; i < m_nCurInfoLines; i++)
		{
			_pUIMgr->GetDrawPort()->PutTextEx(m_strButtonInfo[i],nInfoX+20,nY-nInfoHeight+6+(i*14),m_colButtonInfo[i]);
		}

//	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

}

void CUIButtonEx::SetCharName(eUIBtnCharNameType eCharType, CTString strCharName)
{
	m_eCharType = eCharType;
	
	if(strCharName.Length() == 0)
	{
		m_strCharName = _S( 3865, "없음" );
	}
	else
	{
		m_strCharName = strCharName;
	}
}