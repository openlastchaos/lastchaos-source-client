// ----------------------------------------------------------------------------
//  File : UIChatting.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICHATTING_H_
#define	UICHATTING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UICheckButton.h>
#include <vector>


// Define notice delay time
#define	CHATMSG_NOTICE_DELAY		8000			// milliseconds
#define	CHATMSG_NOTICE_FADEOUT		7000
#define	CHATMSG_NOTICE_FADETIME		( CHATMSG_NOTICE_DELAY - CHATMSG_NOTICE_FADEOUT )

// Date : 2005-02-16,   By Lee Ki-hwan
#define CHAT_MEMORY_COUNT			5



// Chatting type tab
enum ChattingTypeTab
{
	CHATTAB_ALL		= 0,
	CHATTAB_PARTY,
	CHATTAB_GUILD,
	CHATTAB_TRADE,
	CHATTAB_TOTAL,
};


// Chatting message type
enum ChattingMsgType
{
	CHATMSG_NORMAL					= 0,
	CHATMSG_PARTY					= 1,
	CHATMSG_GUILD					= 2,
	CHATMSG_TRADE					= 3,
	CHATMSG_WHISPER					= 4,
	CHATMSG_SHOUT					= 5,
	CHATMSG_NOTICE					= 6,
	CHATMSG_GM						= 7,
	CHATMSG_DUMMY					= 8,		// 메신저 시스템과 같이 맞춰 주기 위함(이기환)
	CHATMSG_LORD					= 9,		// 성주
	CHATMSG_RANKER					= 10,		// 랭커
	CHATMSG_RANKER_CONN				= 11,		// 랭커
	CHATMSG_NOTICE_SERVERDOWN		= 12,		// 서버 종료 메시지 060420
	CHATMSG_GMTOOL					= 15,		// GM TOOL을 이용한 메시지 처리 WSS_GMTOOL 070517 (13,14 서버용)
	CHATMSG_PRESSCORPS,
	CHATMSG_TOTAL					,		
	CHATMSG_COMMAND					,			// [/command]
};

// Chatting option type
enum ChattingOptionType
{
	CHATOPT_NORMAL					= 0,
	CHATOPT_PARTY					= 1,
	CHATOPT_GUILD					= 2,
	CHATOPT_TRADE					= 3,
	CHATOPT_WHISPER					= 4,
	CHATOPT_SHOUT					= 5,
	CHATOPT_SYSTEM					= 6,
	CHATOPT_TOTAL					= 7,
};

// System message type
enum SystemMsgType
{
	SYSMSG_NORMAL					= 0,
	SYSMSG_ATTACK					= 1,
	SYSMSG_ATTACKED					= 2,
	SYSMSG_ERROR					= 3,
	SYSMSG_NOTIFY					= 4,
	SYSMSG_USER						= 5, // wooss 070307 kw:WSS_WHITEDAY_2007
	SYSMSG_TOTAL					= 6,
};

#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
struct sCeilWriting
{
	sCeilWriting()
	{
		strCeilWriting = CTString("");
		iCnt		   = 0;
		llStartTime    = 0;
	}

	CTString		strCeilWriting;
	int				iCnt;
	__int64			llStartTime;
};
#endif

// Define chatting line
#define	MIN_SHOW_CHATLINE			6
#define	MAX_CHAT_LINE				150
#define	MAX_SYSTEM_LINE				100
#define	SYSTEM_SHOW_LINE			5
#define	CHAT_LINESPACING			1


// Define chatting tab size
#define	CHAT_TAB_WIDTH				64
#define	CHAT_TAB_SEL_WIDTH			103
#define	CHAT_TAB_OFFSET				2


// Define whisper history
#define	MAX_WHISPER_SAVE			5


// Define position of text
#define	CHAT_STRING_OFFSETX			23
#define	CHAT_STRING_OFFSTY			7


// Define size of chatting box
#define	CHATTING_WIDTH				322
#define	CHATTING_HEIGHT				137


#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
struct sChat
{
	CTString	strName;
	CTString	strMsg;
	CTString	strSendName[CHATTAB_TOTAL][MAX_CHAT_LINE];	
//	CTString	strNameMsg[CHATTAB_TOTAL][MAX_CHAT_LINE];
	
	COLOR		colSendName[CHATMSG_TOTAL][MAX_CHAT_LINE];
	COLOR		colSendNameType[CHATMSG_TOTAL];
	COLOR		colName;
};
#endif

// ----------------------------------------------------------------------------
// Name : CUIChatting
// Desc :
// ----------------------------------------------------------------------------
class CUIChatting : public CUIWindow
{
protected:
	// Controls
	CUIEditBox		m_ebChatInput;										// Input box of chatting string
	CUIButton		m_btnOption;										// Button of chat type filtering option
	CUICheckButton	m_cbtnChatOption[CHATOPT_TOTAL];					// Check button of chat type filtering option
	CUIScrollBar	m_sbChatScrollBar;									// Scroll bar of chatting message
	CUIScrollBar	m_sbSysScrollBar;									// Scroll bar of system message
	int				m_nCurSelTab;										// Current selected chatting tab
	int				m_nOldSelTab;										// Old selected chatting tab

	// Chatting
	COLOR			m_colChatTypeColor[CHATMSG_TOTAL];					// Color of each chatting type
	COLOR			m_colChatString[CHATTAB_TOTAL][MAX_CHAT_LINE];		// Color of each chatting strings
	CTString		m_strChatString[CHATTAB_TOTAL][MAX_CHAT_LINE];		// Strings of each chatting type
	int				m_nCurChatCount[CHATTAB_TOTAL];						// Current count of chatting strings
	int				m_nCurChatInsert[CHATTAB_TOTAL];					// Index of current inserted chatting string
	int				m_nFirstChatRow[CHATTAB_TOTAL];						// First row of chatting strings in list box
	int				m_nCurChatShowLine;									// Count of current shown chatting line
	int				m_nMaxChatShowLine;									// Maximum count of shown chatting line
	int				m_nChatLineHeight;									// Height of one chatting line
	int				m_nMaxCharCount;									// Maximum count of one line characters

	// System
	COLOR			m_colSysTypeColor[SYSMSG_TOTAL];					// Color of system type
	CTString		m_strSysString[MAX_SYSTEM_LINE];					// Strings of system message
	COLOR			m_colSysString[MAX_SYSTEM_LINE];					// Color of system message
	int				m_nCurSysCount;										// Current count of system message
	int				m_nCurSysInsert;									// Index of current inserted system message
	int				m_nFirstSysRow;										// First row of system strings in list box

	// Whisper
	BOOL			m_bShowWhisperHistory;								// If whisper history is shown or not
	int				m_nWhisperCount;									// Count of whisper history
	int				m_nCurWhisper;										// Current index of whisper history
	CTString		m_strWhisper[MAX_WHISPER_SAVE];						// Strings of whisper target
	CTString		m_strWhisperTarget;									// String of old whisper targer

	// Option
	BOOL			m_bShowOption;										// If option is shown or not
	BOOL			m_bChatOption[CHATTAB_TOTAL][CHATOPT_TOTAL];		// If current chatting type is shown or not

	// Region of each part
	UIRect			m_rcResizeFrame;									// Region of top frame for resizing
	UIRect			m_rcSystem;											// Region of system message
	UIRect			m_rcChatting;										// Region of chatting message
	UIRect			m_rcAllTab;											// Region of all tabs
	UIRect			m_rcTab[CHATTAB_TOTAL];								// Region of each chatting type tab
	UIRect			m_rcInput;											// Region of input box
	UIRect			m_rcWhisper;										// Region of whisper history
	UIRect			m_rcSelWhisper;										// Region of selected whisper target
	UIRect			m_rcOption;											// Region of chatting option

	// UV of each part
	UIRectUV		m_rtWhisperUL;										// UV of upper left background
	UIRectUV		m_rtWhisperUM;										// UV of upper middle background
	UIRectUV		m_rtWhisperUR;										// UV of upper right background
	UIRectUV		m_rtWhisperML;										// UV of middle left background
	UIRectUV		m_rtWhisperMM;										// UV of middle middle background
	UIRectUV		m_rtWhisperMR;										// UV of middle right background
	UIRectUV		m_rtWhisperLL;										// UV of lower left background
	UIRectUV		m_rtWhisperLM;										// UV of lower middle background
	UIRectUV		m_rtWhisperLR;										// UV of lower right background
	UIRectUV		m_rtSelWhisper;										// UV of selected whisper target
	UIRectUV		m_rtTabL;											// UV of left region of chatting type tab
	UIRectUV		m_rtTabM;											// UV of middle region of chatting type tab
	UIRectUV		m_rtTabR;											// UV of right region of chatting type tab
	UIRectUV		m_rtInputBoxL;										// UV of left region of input box
	UIRectUV		m_rtInputBoxM;										// UV of middle region of input box
	UIRectUV		m_rtInputBoxR;										// UV of right region of input box
	UIRectUV		m_rtChatUL;											// UV of upper left background with resizing region
	UIRectUV		m_rtChatUM;											// UV of upper middle background with resizing region
	UIRectUV		m_rtChatUR;											// UV of upper right background with resizing region
	UIRectUV		m_rtChatML;											// UV of middle left background
	UIRectUV		m_rtChatMM;											// UV of middle middle background
	UIRectUV		m_rtChatMR;											// UV of middle right background
	UIRectUV		m_rtChatLL;											// UV of lower left background
	UIRectUV		m_rtChatLM;											// UV of lower middle background
	UIRectUV		m_rtChatLR;											// UV of lower right background
	UIRectUV		m_rtChatUL2;										// UV of upper left background without resizing region
	UIRectUV		m_rtChatUM2;										// UV of upper middle background without resizing region
	UIRectUV		m_rtChatUR2;										// UV of upper right background without resizing region

	// Date : 2005-02-16,   By Lee Ki-hwan
	std::vector<CTString>		m_strMsgBuffer;
	int							m_iCurrentMsgBuffer;
	
	CTextureData	*m_ptdButtonTexture;			// Texture of Button
#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
	const int		m_iCeilWritingMaxCnt;
	BOOL			m_bIsCeilWritingCheck;
public:
	std::list<sCeilWriting>		m_sCeilWriting;
#endif
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
	sChat			m_sChatInfo;
#endif

protected:
	// Internal finctions
	void	ChangeChattingBoxHeight( int ndHeight );
	void	AddChatString( CTString &strChatString, COLOR colChatString, int nCurTab, BOOL bNotice );
	void	AddSysString( CTString &strSysString, COLOR colSysString );
	void	RenderChatStringList();
	void	RenderSysStringList();
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
	void	RenderChatNameMsgString(const int iPosX, const int iPosY, const int iChatLine);
#endif
	void	ProcessParty( char *pcPartyString );
	void	InsertChatPrefix( ChattingMsgType cmtType );
	void	AddWhisperTarget( CTString &strWhisper );
	void	PetTransform( char *pet_changeString );
	void	WearingArmortest(char *item_string);
	void	TakeOffArmortest(char *item_string);

	// Network message functions ( send )
	
	void	SendChatCommand( char *pcChatString, int nLength );

#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
	BOOL	CeilWritingCut_CompareStr(const CTString& strChat);
	void	AddListCeilWriting(const CTString& str);
	void	EraseListCeilWriting();

public:
	void	SetIsCeilWritingCheck(BOOL bCeil)			{ m_bIsCeilWritingCheck = bCeil; }
	BOOL	GetIsCeilWirtingCheck()						{ return m_bIsCeilWritingCheck; }
#endif

public:
	void	SendChatMessage( char *pcChatString, BOOL bLord = FALSE );
	
	CUIChatting();
	~CUIChatting();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Get input box
	CUIEditBox	&GetInputBox() { return m_ebChatInput; }

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
			m_ebChatInput.SetFocus( FALSE );
	}
	
// [KH_070423] 추가
	void OpenAndSetString(CTString strIn);

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebChatInput.IsFocused(); }
	void	KillFocusEditBox() { m_ebChatInput.SetFocus( FALSE ); }

	// Reset chatting
	void	ResetChatting();

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( receive )
	void ENGINE_API	AddChatMessage( UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName, CTString &strChatMessage );
	void ENGINE_API	AddSysMessage( CTString &strSysMessage, int nSysType = SYSMSG_NORMAL ,COLOR colMsg = 0xFFFFFFFF); // wooss 070307 kw : WSS_WHITEDAY_2007
};


#endif	// UICHATTING_H_

