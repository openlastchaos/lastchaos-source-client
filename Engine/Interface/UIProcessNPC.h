
// ----------------------------------------------------------------------------
//  File : UIProcessNPC.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIProcessNPC_H_
#define	UIProcessNPC_H
#ifdef	PRAGMA_ONCE 
#pragma once
#endif

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UIInventory.h>

// Define max char and line of strings
#define	MAX_PROCESSNPC_STRING			4
#define	PROCESSNPC_STRING_CHAR_WIDTH	170

// Define text position
#define	PROCESSNPC_TITLE_TEXT_OFFSETX	25
#define	PROCESSNPC_TITLE_TEXT_OFFSETY	5
#define	PROCESSNPC_DESC_TEXT_SX		18
#define	PROCESSNPC_DESC_TEXT_SY		33

// Define size of ProcessNPC
#define	PROCESSNPC_TOP_HEIGHT			339
#define	PROCESSNPC_BOTTOM_HEIGHT		41

#define	PROCESSNPC_WIDTH				216
#define	PROCESSNPC_HEIGHT				380

#define PROCESSNPC_SLOT_ROW			4
#define PROCESSNPC_SLOT_ROW_TOTAL		30


// ----------------------------------------------------------------------------
// Name : CUIProcessNPC
// Desc : 제조 
// ----------------------------------------------------------------------------
class CUIProcessNPC : public CUIWindow
{
protected:
	
	// Controls...
	
	// Button
	CUIButton				m_btnClose;							// 닫기 버튼 
	CUIButton				m_btnOK;							// 가공 버튼 
	CUIButton				m_btnCancel;						// 취소 버튼 
	CUIButton				m_btnUP;							// 갯수 증가
	CUIButton				m_btnDown;							// 갯수 감소.
	
	// Skill buttons
	std::vector<CUIButtonEx> m_btnProcessNPCItems;				// 가공 아이템
	int						m_nProcessItemCount;				// 만들고자 하는 수.
	
	// Etc ...
	CUIScrollBar			m_sbProcessNPCItem;					// 가공 아이템 창 스크롤 바
	CUIListBox				m_lbPreconditionDesc;				// 필요 조건 설명 리스트 박스
	
	
	//	ProcessNPC Item Info	
	//int						m_nProcessNPCText;					// 가공 문서 아이템 인덱스	
	
	int						m_nSelectProcessNPCItem;			// 현재 선택된 가공물
	CTString				m_StrProcessNPCType;				// 가공 타입
	int						m_nProcessNPCItemCount;				// 가공문서에 맞는 가공품 갯수
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보
	
	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
	
	// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	//UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
	// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button
	UIRectUV				m_rtInput;
	
	// Network ...
	BOOL					m_bWaitProcessNPCResult;				// Wait Message
	
	// ToolTip
	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	
	BOOL					m_bShowItemInfo;
	
	UIRect					m_rcItemInfo;
	int						m_nCurInfoLines;
	
	CTString				m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR					m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	BOOL					m_bDetailItemInfo;

	int						m_iResourceType;
	int						m_iResourceSubType;
	int						m_iLastResourceType;
	int						m_iLastResourceSubType;
	
public:
	CUIProcessNPC();
	~CUIProcessNPC();
	
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void	Render();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Open & close ProcessNPC
	void	Clear ();
	void	InitProcessNPC();
	
	ENGINE_API	void	OpenProcessNPC( );
	ENGINE_API	void	OpenProcessList( int iType, int iSubType );
	void				CloseProcessNPC();
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	// Network message functions ( receive )
	void	ProcessNPCRep( LONG processItemDBIndex, LONG count, LONG result );
	
	// Send ...
	void	SendProcessNPCReq();
	
	// RenderItemInfo ...
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew = FALSE );
	void	RenderItemInfo ();
	// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
};


#endif	// UIProcessNPC

