// ----------------------------------------------------------------------------
//  File : UIQuickSlot.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIQUICKSLOT_H_
#define	UIQUICKSLOT_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>


// Define quick slot buttons
//#define	QSLOT_BTN_COUNT					10
#define	QSLOT_PAGE_COUNT				3
//#define	QSLOT_BTN_SX				35
//#define	QSLOT_BTN_SY				6
#define	QSLOT_BTN_COUNT					12
#define	QSLOT_BTN_SX					39
#define	QSLOT_BTN_SY					34


// Define outline of buttons
#define	QSLOT_BTN_OUTLINE_SX			34
#define	QSLOT_BTN_OUTLINE_SY			5
#define	QSLOT_BTN_OUTLINE_SIZE			34


// Define position of buttons
//#define	QSLOT_PREV_BTN_SX			18
//#define	QSLOT_PREV_BTN_SY			3
//#define	QSLOT_NEXT_BTN_SX			18 
//#define	QSLOT_NEXT_BTN_SY			27
#define	QSLOT_PREV_BTN_SX				22
#define	QSLOT_PREV_BTN_SY				33
#define	QSLOT_NEXT_BTN_SX				22 
#define	QSLOT_NEXT_BTN_SY				53

#define	QSLOT_ROT_BTN_SX				460
#define	QSLOT_ROT_BTN_SY				15


// Define position of text
#define	QSLOT_TAB_TEXT_CX				22
#define	QSLOT_TAB_TEXT_SY				16


// Define size of quick slot
#define	QUICKSLOT_WIDTH					478 //wooss
//#define	QUICKSLOT_HEIGHT			44	
#define	QUICKSLOT_HEIGHT				70	

#define	QSLOT_GIFT_BTN_X				39
#define	QSLOT_GIFT_BTN_Y				31
#define	QSLOT_GIFT_BTN_ARROW			9

//Su-won
enum eGiftPos
{
	GIFTPOS_TOP,
	GIFTPOS_BOTTOM,
	GIFTPOS_RIGHT,
	GIFTPOS_LEFT,
};

// ----------------------------------------------------------------------------
// Name : CUIQuickSlot
// Desc :
// ----------------------------------------------------------------------------
class CUIQuickSlot : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnPrevSlot;									// Button for prev slot
	CUIButton			m_btnNextSlot;									// Button for next slot
	CUIButton			m_btnRotate;									// Button for rotating this UI
	CUIButton			m_btnQuestBook;									// Button for Quest Book
	//wooss
	CUIButton			m_btnCashShop;									// Button for Cash Shop

	//Su-won
	CUIButton			m_btnGiftNotice;								//선물 도착 알림 버튼
	
	BOOL				m_bHorizontal;									// If quick slot is horizontal or vertical
	CUIButtonEx			m_abtnItems[QSLOT_PAGE_COUNT][QSLOT_BTN_COUNT];	// Buttons of quick slot
	int					m_nCurrentPage;									// Current quick slot page
	CTString			m_strCurrentPage;								// Text of current quick slot page

	// Slot information
	int					m_nSelSlotBtnID;								// Selected button ID of quick slot
	BOOL				m_bShowSlotInfo;								// If slot tool tip is shown or not
	BOOL				m_bShowExSlotInfo;
	CTString			m_strSlotInfo;									// Slot information string
	UIRect				m_rcSlotInfo;									// Slot information region

	//Su-won
	BOOL				m_bGiftRecv;									//선물 도착 여부
	CTextureData		*m_ptdGiftTexture;								//선물 도착 버튼 텍스쳐


	// Region of each part
	UIRect				m_rcTitle1;										// Region of title1
	UIRect				m_rcTitle2;										// Region of title2
	UIRect				m_rcSlot;										// Region of slot
	UIRect				m_rcHorzSlotText;								// Region of horizontal slot text
	UIRect				m_rcVertSlotText;								// Region of vertical slot text

	// UV of each part
	UIRectUV			m_rtBackground;									// UV of background
	UIRectUV			m_rtPrevHorzIdle;								// UV of horizontal prev button
	UIRectUV			m_rtPrevHorzClick;								// UV of horizontal prev button
	UIRectUV			m_rtPrevVertIdle;								// UV of vertical prev button
	UIRectUV			m_rtPrevVertClick;								// UV of vertical prev button
	UIRectUV			m_rtNextHorzIdle;								// UV of horizontal next button
	UIRectUV			m_rtNextHorzClick;								// UV of horizontal next button
	UIRectUV			m_rtNextVertIdle;								// UV of vertical next button
	UIRectUV			m_rtNextVertClick;								// UV of vertical next button
	UIRectUV			m_rtRotHorzIdle;								// UV of horizontal rotate button
	UIRectUV			m_rtRotHorzClick;								// UV of horizontal rotate button
	UIRectUV			m_rtRotVertIdle;								// UV of vertical rotate button
	UIRectUV			m_rtRotVertClick;								// UV of vertical rotate button
	UIRectUV			m_rtSlotInfoL;									// UV of left region of slot information
	UIRectUV			m_rtSlotInfoM;									// UV of middle region of slot information
	UIRectUV			m_rtSlotInfoR;									// UV of right region of slot information
	UIRectUV			m_rtBtnOutline;									// UV of outline of buttons
	UIRectUV			m_rtHorzSlotText;								// UV of horizontal slot text
	UIRectUV			m_rtVertSlotText;								// UV of vertical slot text
	UIRectUV			m_rtQuestBook;
	UIRectUV			m_rtQuestBookDown;

	UIRectUV			m_rtCashShop;									// UV of cash shop
	UIRectUV			m_rtCashShopDown;								// UV of cash shop button down

	//Su-won
	UIRectUV			m_rtGift[4][2];									//선물 도착 알림 버튼 UV
	eGiftPos			m_eGiftPos;										//선물 버튼 위치

protected:
	// Internal functions
	void	RotateQuickSlot();
	void	RenderBtns();
	//Su-won
	void	RenderGiftButton();
	void	ShowSlotInfo( BOOL bShowInfo, int nSlotIndex = -1 );
	void	ShowExSlotInfo( BOOL bShowInfo, int nSlotIndex = -1 );
	//Su-won
	void	SetGiftNoticePos();

	// Command functions
	void	AddBtnToQuickSlot( int nSlotIndex );
	void	SwapBtnsInQuickSlot( int nSlot1, int nSlot2 );

	// Network message functions ( send )
	void	SendAddBtn( int nPage, int nSlotNum, int nSlotType = -1, int nData0 = -1, int nData1 = -1 );
	void	SendSwapBtns( int nPage, int nSlotNum1, int nSlotNum2 );

public:
	CUIQuickSlot();
	~CUIQuickSlot();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	UseQuickSlot( int nSlotIndex );
	void	ChangePage( int nPage );
	void	RemoveBtn( int nBtnID );
	void	UpdateSkillLevel( int nIndex, SBYTE sbLevel );
	BOOL	StartSkillDelay( int nIndex );
	//Su-won
	void	SetGiftRecv(BOOL bGiftRecv)	{ m_bGiftRecv =bGiftRecv; }

	// Network message functions ( receive )
	void	AddBtn( int nPage, int nSlotNum, int nSlotType = -1, int nData0 = -1, int nData1 = -1 );
	void	SwapBtns( int nPage, int nSlotNum1, int nSlotNum2 );
	void	RefreshBtn( int nType, int nIndex );
	void	ClearAllBtns( int nPage );

	void	RemoveSummonSkill();
	void	RemoveElementalSkill();
	void	RemovePetSkill();
	void	RemoveWildPetSkill();
};


#endif	// UIQUICKSLOT_H_

