// ----------------------------------------------------------------------------
//  File : UIInventory.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIINVENTORY_H_
#define	UIINVENTORY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/GlobalDefinition.h>


// Define item type tab
enum ItemTab
{
	ITEM_TAB_NORMAL	= 0,
	ITEM_TAB_QUEST,
	ITEM_TAB_EVENT,
	ITEM_TAB_TOTAL,
};

enum InventoryTab
{
	INVEN_TAB_NORMAL0 = 0,
	INVEN_TAB_NORMAL1,
	INVEN_TAB_NORMAL2,
	INVEN_TAB_NORMAL3,
	INVEN_TAB_NORMAL4,
	INVEN_TAB_QUEST,
	INVEN_TAB_EVENT,
	INVEN_TAB_TOTAL,
};

#define	ITEM_TAB_HEIGHT				58
#define	ITEM_TAB_WIDTH				21
#define	ITEM_TAB_GAP				3


// Define process requesting to lock inventory
enum InvenLockProcess
{
	LOCK_NONE = 0,
	LOCK_UPGRADE_ITEM,
	LOCK_BLOOD_OPTION,
	LOCK_REFINE,
	LOCK_COMPOUND,
	LOCK_GW_MIX,
	LOCK_MIX,
	LOCK_MIXNEW,
	LOCK_PROCESS,
	LOCK_PRODUCT,
	LOCK_REMISSION,
	LOCK_EXCHANGE,
	LOCK_SHOP,
	LOCK_CHANGEWEAPON,
	LOCK_PERSONAL_SHOP,
	LOCK_WAREHOUSE,
	LOCK_QUEST,
	LOCK_WARP,
	LOCK_PRESSCORPS,
};


// Define item slot
#define	ITEM_SLOT_SX				29
#define	ITEM_SLOT_SY				204


// Define max char and line of item informaion 
#define	MAX_ITEMINFO_CHAR			26
#define	MAX_ITEMINFO_LINE			20


// Define text position
#define	INVEN_TITLE_TEXT_OFFSETX	25
#define	INVEN_TITLE_TEXT_OFFSETY	5
#define	ITEMTAB_TEXT_OFFSETY		147
#define	INVEN_MONEY_TEXT_POSX		211
#define	INVEN_MONEY_TEXT_POSY		182
#define	INVEN_WEIGHT_TEXT_POSX		82
#define	LEASE_INFO_TEXT_OFFSETX		180

// Define size of inventory
//#define	INVENTORY_WIDTH				230
//#define	INVENTORY_HEIGHT			385
#define	INVENTORY_WIDTH				256
#define	INVENTORY_HEIGHT			408
#define	INVENTORY_WEIGHT_WIDTH		62

// ----------------------------------------------------------------------------
// Name : CUIInventory
// Desc :
// ----------------------------------------------------------------------------
class CUIInventory : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	BOOL				m_bLockInven;							// If inventory is locked or not
	BOOL				m_bLockArrange;							// If arrange button is locked or not
	InvenLockProcess	m_eLockInvenProcess;					// Process requesting to lock inventory
	BOOL				m_bShowToolTip;							// If tool tip is shown or not
	CTString			m_strToolTip;							// String of tool tip

	// Items
	InventoryTab		m_InvenCurrentTab;						//
	CUIButtonEx			m_abtnWearing[WEAR_TOTAL];				// Wearing items
	CUIButtonEx			m_abtnItems[INVEN_SLOT_TAB][INVEN_SLOT_ROW_TOTAL][INVEN_SLOT_COL];	// Slot items
	// wooss 050807
	// Items info add	
	int					m_col;
	int					m_row;

	// Item information
	int					m_nSelWearItemID;						// Selected weating item
	int					m_nSelSlotItemID[INVEN_TAB_TOTAL];		// Selected slot item
	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	CTString			m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	UIRect				m_rcItemInfo;							// Item information region
	CTString			m_strMyMoney;							// String of current money
	CTString			m_strWeight;							// String of current weight
	CTString			m_strNumItems;							// String of current items count
	COLOR				m_colWeight;							// Color of weight string
	int					m_iItemServerIndex;						// for booster upgrade

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcWearing;							// Region of wearing
	UIRect				m_rcItemTypeTab[2];						// Region of item type tab
	UIRect				m_rcItemSlot;							// Region of item slot
	UIRect				m_rcToolTip;							// Region of tool tip

	CUIRectSurface		m_NewInvenSurface;

	CUIButton			m_btnTab[INVEN_TAB_TOTAL];
	CUIButton			m_btnTrashcan;							// 휴지통 버튼

	CUIButtonEx			m_abtnDestroyItem;						// 파괴할 아이템 정보

protected:
	// Internal functions
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	void	GetItemInfo( int nTab, int nRow, int nCol, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, BOOL bRenew = FALSE, int nWear = -1, int nRow = -1, int nCol = -1 );
	void	RenderItems();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );
	void	UpdateWeight();

	// Command functions
	void	UpgradeItem( SBYTE sbWearPos, SLONG slIndex, SLONG slUniIndex
		, SBYTE sbRefineRow, SBYTE sbRefineCol, SLONG slRefineLevel );

	void	OptionAddItem( SBYTE sbWearPos, SLONG slIndex, SBYTE sbItemRow,
							SBYTE sbItemCol );
	void	OptionDelItem( SBYTE sbWearPos, SLONG slIndex, SBYTE sbItemRow,
							SBYTE sbItemCol );

	// Network message functions ( send )
	void	SendArrangeItem();
	void	SendSwapSlotItems( int nTab0, int nRow0, int nCol0, int nRow1, int nCol1 );
	void	SendUpgradeItem( INDEX iProtect =-1);
	void	SendItemLvDown();

	void	SendOptionAddItem();
	void	SendOptionDelItem();

public:
	CUIInventory();
	~CUIInventory();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Get row, col of warehouse item
	void	GetLocationOfNormalItem( int nUniIndex, int &nRow, int &nCol );

	//wooss 050807
	//Get item info(tab,row,col) of DoubleClicked item
	void	GetUseItemSlotInfo(int &nTab,int &nRow, int &nCol) ;
	void	SetUseItemSlotInfo(int nTab,int nRow, int nCol) ;
	
	// find
	SQUAD	GetItemCount(int iItemIndex);
	CTString GetItemName(int iItemIndex);
	SQUAD	GetItemAll();

	// Lock
	void	Lock( BOOL bLock, BOOL bOnlyArrange, InvenLockProcess eWhichProcess )
	{
		if( !bLock && eWhichProcess != LOCK_NONE && m_eLockInvenProcess != eWhichProcess )
			return;

		m_bLockArrange = bLock;
		if( !bOnlyArrange )
			m_bLockInven = bLock;

		bLock ? m_eLockInvenProcess = eWhichProcess : m_eLockInvenProcess = LOCK_NONE;
	}
	BOOL	IsLocked() const { return m_bLockInven; }
	void	ShowLockErrorMessage();

	// Toggle visible
	void	ToggleVisible();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( send )
	void	SendUseSlotItem( int nTab, int nRow, int nCol, SBYTE sbWearType = -1 );

	// Network message functions ( receive )
	void	InitInventory( int nTab, int nRow, int nCol, int nUniIndex, int nIndex, int nWearingType );
	void	SwapSlotItems( int nTab0, int nRow0, int nCol0, int nTab1, int nRow1, int nCol1 );
	void	UpgradeItem( SBYTE sbResult );
	void	ClearAllItems( SBYTE sbTab );
	void	OptionAddRep( SBYTE sbResult );
	void	OptionDelRep( SBYTE sbResult );
	void	SetPetItem(int nTab, int nRow, int nCol); 
	
	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	BOOL	StartSkillDelay( int nIndex );
	BOOL	GetSkillDelay( int nIndex );
	
	LONG	GetItemIndex( int nTab, int nRow, int nCol );

	// [070824: Su-won] REFINE_PROTECT
	// 제련을 시도할 아이템의 +수치를 얻음
	SLONG	GetUpgradeItemPlus();

	INDEX	GetMySlotItemsCount(void);

	void	ShowItemDestroyMessage(CUIButtonEx& abtn);
	BOOL	FindEmptyabtnItem(int nTab, int &nRow, int &nCol);
	
	// [090608: selo] 퀘스트 아이템 사용제한 체크
	BOOL	IsRestritionItem(int nTab, int nRow, int nCol);
};


#endif	// UIINVENTORY_H_

