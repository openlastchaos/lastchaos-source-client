// ----------------------------------------------------------------------------
//  File : UIButtonEx.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIBUTTONEX_H_
#define	UIBUTTONEX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIWindow.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>


// UI expansion button state
enum UIBtnExState
{
	UBES_IDLE		= 0,
	UBES_CLICK		= 1,
	UBES_TOTAL		= 2,
};


// UI expansion button type
enum UIBtnExType
{
	UBET_SKILL		= 0,
	UBET_ACTION		= 1,
	UBET_ITEM		= 2,
	UBET_QUEST		= 3,
	UBET_EVENT		= 4,
	UBET_REMISSION	= 5,
	UBET_COMBO		= 6,	//ttos: 몬스터 콤보
	UBET_AUCTION	= 7,
	UBET_TYPE_TOTAL	= 8
};

//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
enum eUIBtnCharNameType
{
	CHAR_NAME_NONE = 0,
	CHAR_NAME_BUYER = 1,
	CHAR_NAME_SELLER = 2,
	CHAR_NAME_OWNER = 3,
};

// Define button size
#define	BTN_SIZE	32

#define	MAX_ITEMINFO_LINE			20
// ----------------------------------------------------------------------------
// Name : CUIButton
// Desc : Expansion version for item, skill and action
// ----------------------------------------------------------------------------
class CUIButtonEx : public CUIWindow
{
protected:
	CTextureData	*m_ptdAddTexture;
	UWORD				m_uwBtnID;							// ID of button
	int					m_nIndex;							// Index of item, skill, action or quest
	int					m_nUniIndex;						// Unique index of item
	
	// ------- Cash Item ----------------->
	int					m_nCashIndex;
	int					m_nCashType;
	CTString			m_nCashName;
	CTString			m_nCashDesc;
	int					m_nCashListCount;
	int					m_nCashTypeIndex;
	// <-----------------------------------
	
	union
	{
		SBYTE			m_nWearType;						// Type of wearing item
		SBYTE			m_nSkillLevel;						// Level of skill
		SBYTE			m_nQuestType;						// Type of quest
		SBYTE			m_nEventType;						// Type of event
	};

	ULONG				m_ulPlus;							// Plus of item
	BOOL				m_bSkillDelay;						// If skill is delay state
	SBYTE				m_sbQuestFlag;					// If quest is completed

	union
	{
		ULONG			m_ulFlag;							// Flag of item
		BOOL			m_bSpecial;							// 특수 스킬인가?
		SBYTE			m_sbRemissionType;					// 면죄부 종류.
	};

	LONG				m_ulUsed;							// 내구도
	LONG				m_ulUsed2;							

	SQUAD				m_llCount;							// Count of item
	SQUAD				m_llPrice;							// Price of Item
	SQUAD				m_llTime;							// Use Item Start Time
	SBYTE				m_sbOptionType[MAX_ITEM_OPTION];	// Option type of item
	SBYTE				m_sbOptionLevel[MAX_ITEM_OPTION];	// Option level of item
	SBYTE				m_nTab;								// Tab of item
	SBYTE				m_nRow;								// Row of item
	SBYTE				m_nCol;								// Column of item
	UWORD				m_uwRareIndex;						// 레어 옵션 인덱스

	SBYTE				m_nTextureID;						// Texture ID for rendering this button
	SBYTE				m_nWhichUI;							// From which UI
	UIBtnExType			m_betType;							// Type of button ( item, skill or action )	
	BOOL				m_bEmpty;
	UIBtnExState		m_bsState;							// State of UI expansion button
	UIRectUV			m_rtUV;								// UV of button	 
	
	BOOL				m_bLButtonDown;						// Left Mouse Button Click
	CTString			m_strButtonInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colButtonInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	INDEX				m_nCurInfoLines;
	UIRectUV			m_rtPopupInfo;
	UIRectUV			m_rtPopupName;
	BOOL				m_bShow;
	BOOL				m_bSetPopUp;						// Popup Enable;
	sPetItem_Info		m_petData;

	//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
	eUIBtnCharNameType	m_eCharType;
	CTString			m_strCharName;
protected:
	// UV
	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	UIRectUV	&GetUV() { return m_rtUV; }

	// Copy & swap
	void	CopyItemInfo( CUIButtonEx &btnSrc );
	void	CopySkillInfo( CUIButtonEx &btnSrc );
	void	CopyActionInfo( CUIButtonEx &btnSrc );
	void	CopyComboInfo( CUIButtonEx &btnSrc );
	void	SwapItemInfo( CUIButtonEx &btnSrc );

public:
	CUIButtonEx();
	~CUIButtonEx();

	// Create buton
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					SBYTE nWhichUI = -1, UIBtnExType betType = UBET_ITEM,
					SBYTE nTab = -1, SBYTE nRow = -1, SBYTE nCol = -1 );

	// Initialize button
	void	InitBtn();

	// Render
	void	Render();
	void	RenderInfoPopup();
	void	RenderDefaultButton();
	void	RenderHighlight( const COLOR colHighlight, const ULONG ulPBT = PBT_ADD );

	// Common information
	void	SetTextureID( SBYTE nTexID ) { m_nTextureID = nTexID; }
	void	SetBtnType( UIBtnExType betType ) { m_betType = betType; }
	void	SetEmpty( BOOL bEmpty ) { m_bEmpty = bEmpty; }
	void	SetBtnState( UIBtnExState bsState ) { m_bsState = bsState; }
	UWORD	GetBtnID() const { return m_uwBtnID; }
	int		GetIndex() const { return m_nIndex; }
	SBYTE	GetTextureID() const { return m_nTextureID; }
	SBYTE	GetWhichUI() const { return m_nWhichUI; }
	UIBtnExType		GetBtnType() const { return m_betType; }
	BOOL	IsEmpty() const { return m_bEmpty; }
	UIBtnExState	GetBtnState() const { return m_bsState; }

	// Item information
	void	SetItemInfo( SBYTE nTab, SBYTE nRow, SBYTE nCol, int nIndex, int nUniIndex, SBYTE nWearingType ,
		int nCashIndex = -1, int nCashSection = -1, CTString nName ="", CTString nDesc ="", int nListCount = -1, int nTypeIndex = -1);
	void	SetItemIndex( int nIndex ) { m_nIndex = nIndex; }
	void	SetItemUniIndex( int nUniIndex ) { m_nUniIndex = nUniIndex; }
	void	SetItemWearType( SBYTE nWearType ) { m_nWearType = nWearType; }
	void	SetItemPlus( ULONG ulPlus ) { m_ulPlus = ulPlus; }
	void	SetItemFlag( ULONG ulFlag ) { m_ulFlag = ulFlag; }
	void	SetItemUsed( LONG ulUsed )	{ m_ulUsed = ulUsed; }
	void	SetItemUsed2( LONG ulUsed2 ) { m_ulUsed2 = ulUsed2; }
	void	SetItemOptionData( SBYTE sbOption, SBYTE sbOptionType, SBYTE sbOptionLevel )
	{
		m_sbOptionType[sbOption] = sbOptionType;
		m_sbOptionLevel[sbOption] = sbOptionLevel;
	}
	void	SetItemCount( SQUAD llCount ) { m_llCount = llCount; }
	void	SetItemPrice( SQUAD llPrice ) { m_llPrice = llPrice; }
	void	SetUseItemStartTime( SQUAD llTime ) { m_llTime = llTime; }
	void	SetItemTab( SBYTE nTab ) { m_nTab = nTab; }
	void	SetItemRow( SBYTE nRow ) { m_nRow = nRow; }
	void	SetItemCol( SBYTE nCol ) { m_nCol = nCol; }
	void	SetItemLocation( SBYTE nTab, SBYTE nRow, SBYTE nCol ) { m_nTab = nTab; m_nRow = nRow; m_nCol = nCol; }
	void	SetItemRareIndex( UWORD uwIndex ) { m_uwRareIndex =uwIndex; }
	void	SetCashName(CTString strName) { m_nCashName = strName;}

	int		GetItemIndex() const { return m_nIndex; }
	int		GetItemUniIndex() const { return m_nUniIndex; }
	SBYTE	GetItemWearType() const { return m_nWearType; }
	// ---------- Cash Item ---------->
	int		GetCashIndex() const { return m_nCashIndex; }
	int		GetCashType() const { return m_nCashType; }
	CTString GetCashName() const { return m_nCashName; }
	CTString GetCashDesc() const { return m_nCashDesc; }
	int		GetCashListCount() const { return m_nCashListCount; }
	int		GetCashTypeIndex() const { return m_nCashTypeIndex; }
	
	CUIWindow* GetWindow() { return m_pParentWnd ;}
	void	  SetWindow(CUIWindow* pParentWnd) { m_pParentWnd = pParentWnd ;}	

	// <-------------------------------
	ULONG	GetItemPlus() const { return m_ulPlus; }
	LONG	GetItemUsed() const { return m_ulUsed; }
	LONG	GetItemUsed2() const { return m_ulUsed2; }
	ULONG	GetItemFlag() const { return m_ulFlag; }
	SBYTE	GetItemOptionType( int nOption ) const { return m_sbOptionType[nOption]; }
	SBYTE	GetItemOptionLevel( int nOption ) const { return m_sbOptionLevel[nOption]; }
	SQUAD	GetItemCount() const { return m_llCount; }
	SQUAD	GetItemPrice() const { return m_llPrice; }
	SQUAD	GetUseItemStartTime() const { return m_llTime; }
	SBYTE	GetItemTab() const { return m_nTab; }
	SBYTE	GetItemRow() const { return m_nRow; }
	SBYTE	GetItemCol() const { return m_nCol; }
	UWORD	GetItemRareIndex() const {return m_uwRareIndex; }

	// Skill information
	void	SetSkillInfo( int nIndex, SBYTE sbSkillLevel, BOOL bSpecial = FALSE );
	void	SetSkillLevel( SBYTE sbSkillLevel ) { m_nSkillLevel = sbSkillLevel; }
	void	SetSkillDelay( BOOL bSkillDelay ) { m_bSkillDelay = bSkillDelay; }
	int		GetSkillIndex() const { return m_nIndex; }
	SBYTE	GetSkillLevel() const { return m_nSkillLevel; }
	BOOL	GetSkillDelay() const { return m_bSkillDelay; }
	BOOL	GetSkillSpecial() const { return m_bSpecial; }

	// Action information
	void	SetActionInfo( int nIndex );
	int		GetActionIndex() const { return m_nIndex; }
	
	// Quest information
	void	SetQuestInfo( int nIndex, SBYTE sbType, SBYTE sbQuestFlag );
	void	SetQuestFlag( BOOL bQuestFlag ) { m_sbQuestFlag = bQuestFlag; }
	int		GetQuestIndex() const { return m_nIndex; }
	SBYTE	GetQuestType() const { return m_nQuestType; }
	SBYTE	GetQuestFlag() const { return m_sbQuestFlag; }

	// Event information
	void	SetEventInfo( int nIndex );
	int		GetEventIndex() const { return m_nIndex; }

	// Remission information
	void	SetRemissionInfo( SBYTE iRemissionType, SBYTE nTab = -1, SBYTE nRow = -1, SBYTE nCol = -1, int nIndex = -1, int nUniIndex = -1, SBYTE nWearingType = -1 );
	SBYTE	GetRemissionType() const { return m_sbRemissionType; }

	// Aution information
	void	SetAuctionNotice( int nIndex );
	int		GetAuctionIndex() const { return m_nIndex; }
	BOOL	GetShowInfo() const { return m_bShow; }

	//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
	eUIBtnCharNameType GetCharType() { return m_eCharType; }
	CTString GetCharName() { return m_strCharName; }
	void	SetCharName(eUIBtnCharNameType eCharType, CTString strCharName);

	// MissionCase information
	void	SetComboInfo( SBYTE nTab = -1, SBYTE nRow = -1, SBYTE nCol = -1);

	void	SetPetDataInfo(sPetItem_Info petinfo) { m_petData = petinfo;}

	// Copy & swap
	void	Copy( CUIButtonEx &btnSrc );
	void	Swap( CUIButtonEx &btnSrc );

	void	AddInfoString( CTString &strInfo, COLOR colInfo = 0xF2F2F2FF );
	void	GetButtonInfo(/*CTString *strInfo, COLOR *colInfo, */int &nInfoWidth, int &nInfoHeight);
	int		GetWearLevelReduction();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIBUTTONEX_H_

