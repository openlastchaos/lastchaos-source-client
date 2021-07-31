#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIFiltering.h> // 이기환 추가 (11.25)
#include <Engine/Interface/UISignBoard.h>
#include <Engine/Interface/UIAutoHelp.h>

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/GameState.h>

// Date : 2005-11-18(오후 1:45:04), By Lee Ki-hwan
//#define SHOW_DAMAGE_TEXT	// text로 표시되는 데미지 

extern INDEX	g_iCountry;

#define SCROLLBAR_ADJUST_Y 2

// ----------------------------------------------------------------------------
// Name : CUIChatting()
// Desc : Constructor
// ----------------------------------------------------------------------------
#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
CUIChatting::CUIChatting() : m_iCeilWritingMaxCnt(3)
#else
CUIChatting::CUIChatting()
#endif
{
	m_colChatTypeColor[CHATMSG_NORMAL] = 0xCCCCCCFF;
	m_colChatTypeColor[CHATMSG_PARTY] = 0x91A7EAFF;
	m_colChatTypeColor[CHATMSG_GUILD] = 0xD6A6D6FF;
	m_colChatTypeColor[CHATMSG_TRADE] = 0x50C800FF;
	m_colChatTypeColor[CHATMSG_WHISPER] = 0xE1B300FF;
	if(g_iCountry == THAILAND ) m_colChatTypeColor[CHATMSG_SHOUT] = 0xFF96BEFF;
	else m_colChatTypeColor[CHATMSG_SHOUT] = 0xDF6900FF;
	m_colChatTypeColor[CHATMSG_NOTICE] = 0xE18600FF;
	
	if( g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
		m_colChatTypeColor[CHATMSG_LORD] = 0x31CEC7FF;
	else
		m_colChatTypeColor[CHATMSG_LORD] = 0xf6f82dFF;

	m_colChatTypeColor[CHATMSG_RANKER] = 0x00EDBDFF;
	// WSS_GMTOOL 070517 ----------------------------->><<
	m_colChatTypeColor[CHATMSG_GMTOOL] = 0xE18600FF;

	// Date : 2005-04-14(오후 2:32:19), By Lee Ki-hwan
	// 중국 버전의 경우에만 gm 메세지 빨강색으로 표시 
	if( g_iCountry == CHINA )
	{
		m_colChatTypeColor[CHATMSG_GM] = 0xFF0000FF;
	}
	else 
	{
		m_colChatTypeColor[CHATMSG_GM] = 0xE18600FF;
	}
	
	m_colSysTypeColor[SYSMSG_NORMAL] = 0xC4D6A6FF;
	m_colSysTypeColor[SYSMSG_ATTACK] = 0x92C253FF;
	m_colSysTypeColor[SYSMSG_ATTACKED] = 0xE2BE69FF;
	m_colSysTypeColor[SYSMSG_ERROR] = 0xE28769FF;
	m_colSysTypeColor[SYSMSG_NOTIFY] = 0x6060FFFF;
	m_colSysTypeColor[SYSMSG_USER] = m_colSysTypeColor[SYSMSG_NORMAL]; // wooss 070307 kw: WSS_WHITEDAY_2007

	m_nCurSelTab = CHATTAB_ALL;
	m_nOldSelTab = CHATTAB_ALL;
	m_nCurChatShowLine = 0;
	m_bShowWhisperHistory = FALSE;
	m_nWhisperCount = 0;
	m_nCurWhisper = 0;
	m_nMaxCharCount = 0;
	m_bShowOption = FALSE;

	int	iChatTab, iChatLine, iChatType;
	for( iChatTab = 0; iChatTab < CHATTAB_TOTAL; iChatTab++ )
	{
		for( iChatLine = 0; iChatLine < MAX_CHAT_LINE; iChatLine++ )
			m_strChatString[iChatTab][iChatLine] = CTString( "" );

		for( iChatType = 0; iChatType < CHATOPT_TOTAL; iChatType++ )
			m_bChatOption[iChatTab][iChatType] = TRUE;

		m_nCurChatCount[iChatTab] = 0;
		m_nCurChatInsert[iChatTab] = 0;
		m_nFirstChatRow[iChatTab] = 0;
	}

	for( iChatLine = 0; iChatLine < MAX_SYSTEM_LINE; iChatLine++ )
		m_strSysString[iChatLine] = CTString( "" );

	m_nCurSysCount = 0;
	m_nCurSysInsert = 0;
	m_nFirstSysRow = 0;


	for( int iWhisper = 0; iWhisper < MAX_WHISPER_SAVE; iWhisper++ )
		m_strWhisper[iWhisper] = CTString( "" );
	m_strWhisperTarget = CTString( "" );

	// Date : 2005-02-16,   By Lee Ki-hwan
	m_iCurrentMsgBuffer = 0;
	m_strMsgBuffer.clear();
#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
	m_sCeilWriting.clear();
	m_bIsCeilWritingCheck = FALSE;
#endif
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
	m_sChatInfo.strMsg = CTString("");
	m_sChatInfo.strName = CTString("");

	for (iChatTab = 0; iChatTab < CHATTAB_TOTAL; ++iChatTab)
	{
		for (iChatLine = 0; iChatLine < MAX_CHAT_LINE; ++iChatLine)
			m_sChatInfo.strSendName[iChatTab][iChatLine] = CTString("");
	}
	
	m_sChatInfo.colSendNameType[CHATMSG_NORMAL] = 0xFFFFFFFF;//0xFFFFFFFF		0x808080FF
	m_sChatInfo.colSendNameType[CHATMSG_PARTY] = 0x5776E5FF;
	m_sChatInfo.colSendNameType[CHATMSG_GUILD] = 0xBA54FFFF;
	m_sChatInfo.colSendNameType[CHATMSG_TRADE] = 0x97FF93FF;
	m_sChatInfo.colSendNameType[CHATMSG_WHISPER] = 0xFFE97FFF;
	m_sChatInfo.colSendNameType[CHATMSG_SHOUT] = 0xFFB27FFF;
#endif	
}

// ----------------------------------------------------------------------------
// Name : ~CUIChatting()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIChatting::~CUIChatting()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nChatLineHeight = _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING;
	m_nMaxCharCount = ( nWidth - CHAT_STRING_OFFSETX - 9 ) /
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Rectangle region
	// Input box
	m_rcInput.SetRect( 0, -22, nWidth, 0 );

	// Tab
	int	nPosX = 0;
	m_rcTab[CHATTAB_ALL].SetRect( nPosX, -42, nPosX + CHAT_TAB_SEL_WIDTH, -23 );
	for( int i = CHATTAB_PARTY; i < CHATTAB_TOTAL; i++ )
	{
		nPosX = m_rcTab[i - 1].Right + CHAT_TAB_OFFSET;
		m_rcTab[i].SetRect( nPosX, -42, nPosX + CHAT_TAB_WIDTH, -23 );
	}
	m_rcAllTab.SetRect( 0, -42, 301, -23 );

	// Chatting region
	int	nChatHeight = m_nChatLineHeight * MIN_SHOW_CHATLINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcChatting.SetRect( 0, -44 - nChatHeight, nWidth, -44 );

	// System region
	int	nBottom = m_rcChatting.Top - 1;
	nChatHeight = m_nChatLineHeight * SYSTEM_SHOW_LINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcSystem.SetRect( 0, nBottom - nChatHeight, nWidth, nBottom );

	// Resizing frame region
	m_rcResizeFrame.SetRect( 0, m_rcSystem.Top, nWidth, m_rcSystem.Top + 10 );

	// Whisper region
	nPosX = 21 + 19 - _pUIFontTexMgr->GetFontSpacing() +
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) * 16;
	m_rcWhisper.SetRect( 21, -18, nPosX, -18 );
	m_rcSelWhisper.SetRect( 23, -m_nChatLineHeight, nPosX - 5, 0 );

	// Create texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Chatting.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Chatting region
	/******
	m_rtChatUL.SetUV( 0, 0, 18, 14, fTexWidth, fTexHeight );
	m_rtChatUM.SetUV( 19, 0, 23, 14, fTexWidth, fTexHeight );
	m_rtChatUR.SetUV( 20, 0, 38, 14, fTexWidth, fTexHeight );
	m_rtChatML.SetUV( 0, 15, 18, 20, fTexWidth, fTexHeight );
	m_rtChatMM.SetUV( 19, 15, 23, 20, fTexWidth, fTexHeight );
	m_rtChatMR.SetUV( 20, 15, 38, 20, fTexWidth, fTexHeight );
	m_rtChatLL.SetUV( 0, 22, 18, 36, fTexWidth, fTexHeight );
	m_rtChatLM.SetUV( 19, 22, 23, 36, fTexWidth, fTexHeight );
	m_rtChatLR.SetUV( 20, 22, 38, 36, fTexWidth, fTexHeight );
	m_rtChatUL2.SetUV( 0, 37, 18, 51, fTexWidth, fTexHeight );
	m_rtChatUM2.SetUV( 19, 37, 23, 51, fTexWidth, fTexHeight );
	m_rtChatUR2.SetUV( 20, 37, 38, 51, fTexWidth, fTexHeight );
	**********/
	m_rtChatUL.SetUV( 0, 342, 18, 366, fTexWidth, fTexHeight );
	m_rtChatUM.SetUV( 18, 342, 303, 366, fTexWidth, fTexHeight );
	m_rtChatUR.SetUV( 303, 342, 321, 366, fTexWidth, fTexHeight );
	m_rtChatML.SetUV( 0, 366, 18, 404, fTexWidth, fTexHeight );
	m_rtChatMM.SetUV( 18, 366, 303, 404, fTexWidth, fTexHeight );
	m_rtChatMR.SetUV( 303, 366, 321, 404, fTexWidth, fTexHeight );
	m_rtChatLL.SetUV( 0, 404, 18, 419, fTexWidth, fTexHeight );
	m_rtChatLM.SetUV( 18, 404, 303, 419, fTexWidth, fTexHeight );
	m_rtChatLR.SetUV( 303, 404, 321, 419, fTexWidth, fTexHeight );
	m_rtChatUL2.SetUV( 0, 421, 18, 435, fTexWidth, fTexHeight );
	m_rtChatUM2.SetUV( 18, 421, 303, 435, fTexWidth, fTexHeight );
	m_rtChatUR2.SetUV( 303, 421, 321, 435, fTexWidth, fTexHeight );

	// Tab regin
	/***
	m_rtTabL.SetUV( 39, 0, 64, 19, fTexWidth, fTexHeight );
	m_rtTabM.SetUV( 64, 0, 78, 19, fTexWidth, fTexHeight );
	m_rtTabR.SetUV( 78, 0, 103, 19, fTexWidth, fTexHeight );
	***/
	m_rtTabL.SetUV( 343, 220, 377, 239, fTexWidth, fTexHeight );
	m_rtTabM.SetUV( 377, 220, 412, 239, fTexWidth, fTexHeight );
	m_rtTabR.SetUV( 412, 220, 446, 239, fTexWidth, fTexHeight );

	// Input region
	/***
	m_rtInputBoxL.SetUV( 39, 42, 64, 64, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 64, 42, 68, 64, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 68, 42, 94, 64, fTexWidth, fTexHeight );
	***/
	m_rtInputBoxL.SetUV( 0, 278, 25, 300, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 25, 278, 296, 300, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 296, 278, 321, 300, fTexWidth, fTexHeight );

	// Whisper region
	/***
	m_rtWhisperUL.SetUV( 106, 42, 113, 49, fTexWidth, fTexHeight );
	m_rtWhisperUM.SetUV( 115, 42, 119, 49, fTexWidth, fTexHeight );
	m_rtWhisperUR.SetUV( 121, 42, 128, 49, fTexWidth, fTexHeight );
	m_rtWhisperML.SetUV( 106, 52, 113, 54, fTexWidth, fTexHeight );
	m_rtWhisperMM.SetUV( 115, 52, 119, 54, fTexWidth, fTexHeight );
	m_rtWhisperMR.SetUV( 121, 52, 128, 54, fTexWidth, fTexHeight );
	m_rtWhisperLL.SetUV( 106, 57, 113, 64, fTexWidth, fTexHeight );
	m_rtWhisperLM.SetUV( 115, 57, 119, 64, fTexWidth, fTexHeight );
	m_rtWhisperLR.SetUV( 121, 57, 128, 64, fTexWidth, fTexHeight );
	m_rtSelWhisper.SetUV( 118, 30, 126, 39, fTexWidth, fTexHeight );
	***/
	m_rtWhisperUL.SetUV( 239, 253, 246, 260, fTexWidth, fTexHeight );
	m_rtWhisperUM.SetUV( 246, 253, 331, 260, fTexWidth, fTexHeight );
	m_rtWhisperUR.SetUV( 331, 253, 338, 260, fTexWidth, fTexHeight );
	m_rtWhisperML.SetUV( 239, 260, 246, 265, fTexWidth, fTexHeight );
	m_rtWhisperMM.SetUV( 246, 260, 331, 265, fTexWidth, fTexHeight );
	m_rtWhisperMR.SetUV( 331, 260, 338, 265, fTexWidth, fTexHeight );
	m_rtWhisperLL.SetUV( 239, 265, 246, 272, fTexWidth, fTexHeight );
	m_rtWhisperLM.SetUV( 246, 265, 331, 272, fTexWidth, fTexHeight );
	m_rtWhisperLR.SetUV( 331, 265, 338, 272, fTexWidth, fTexHeight );
	
	m_rtSelWhisper.SetUV( 118, 30, 126, 39, fTexWidth, fTexHeight );

	// Option button
	m_btnOption.Create( this, CTString( "" ), 303, -42, 19, 19 );
	//m_btnOption.SetUV( UBS_IDLE, 59, 20, 78, 39, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 39, 20, 58, 39, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_IDLE, 508, 256, 526, 275, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 530, 256, 548, 275, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Chatting input box
	m_ebChatInput.Create( this, 23, -18, 277, 12, 250 );
	//m_ebChatInput.SetReadingWindowUV( 79, 20, 96, 36, fTexWidth, fTexHeight );
	//m_ebChatInput.SetCandidateUV( 79, 20, 96, 36, fTexWidth, fTexHeight );
	m_ebChatInput.SetReadingWindowUV( 18, 281, 317, 296, fTexWidth, fTexHeight );
	m_ebChatInput.SetCandidateUV( 18, 281, 317, 296, fTexWidth, fTexHeight );

	// Chatting scroll bar
	//m_sbChatScrollBar.Create( this, 6, m_rcChatting.Top + 3, 11, m_rcChatting.GetHeight() - 6 );
	//m_sbChatScrollBar.CreateButtons( TRUE, 11, 8, 0, 0, 9 );
	m_sbChatScrollBar.Create( this, 5, m_rcChatting.Top + SCROLLBAR_ADJUST_Y, 13, m_rcChatting.GetHeight() - 4 );
	m_sbChatScrollBar.CreateButtons( TRUE, 13, 13, 0, 0, 9 );	
	m_sbChatScrollBar.SetScrollPos( 0 );
	m_sbChatScrollBar.SetScrollRange( MAX_CHAT_LINE );
	m_sbChatScrollBar.SetCurItemCount( 0 );
	m_sbChatScrollBar.SetItemsPerPage( MIN_SHOW_CHATLINE );
	// Up button
	//m_sbChatScrollBar.SetUpUV( UBS_IDLE, 117, 0, 128, 8, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_CLICK );
	m_sbChatScrollBar.SetUpUV( UBS_IDLE, 252, 218, 265, 231, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetUpUV( UBS_CLICK, 268, 218, 281, 231, fTexWidth, fTexHeight );	
	m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	//m_sbChatScrollBar.SetBarTopUV( 105, 0, 116, 9, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.SetBarMiddleUV( 105, 8, 116, 11, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.SetBarBottomUV( 105, 10, 116, 19, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarTopUV( 238, 218, 251, 222, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarMiddleUV( 238, 222, 251, 227, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarBottomUV( 238, 227, 251, 233, fTexWidth, fTexHeight );
	// Down button
	//m_sbChatScrollBar.SetDownUV( UBS_IDLE, 117, 10, 128, 18, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_CLICK );
	m_sbChatScrollBar.SetDownUV( UBS_IDLE, 252, 233, 265, 246, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetDownUV( UBS_CLICK, 268, 233, 281, 246, fTexWidth, fTexHeight );
	m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	// System scroll bar
	//m_sbSysScrollBar.Create( this, 6, m_rcSystem.Top + 3, 11, m_rcSystem.GetHeight() - 6 );
	//m_sbSysScrollBar.CreateButtons( TRUE, 11, 8, 0, 0, 9 );
	m_sbSysScrollBar.Create( this, 5, m_rcSystem.Top +SCROLLBAR_ADJUST_Y, 13, m_rcSystem.GetHeight() - 4 );
	m_sbSysScrollBar.CreateButtons( TRUE, 13, 13, 0, 0, 9 );	
	m_sbSysScrollBar.SetScrollPos( 0 );
	m_sbSysScrollBar.SetScrollRange( MAX_SYSTEM_LINE );
	m_sbSysScrollBar.SetCurItemCount( 0 );
	m_sbSysScrollBar.SetItemsPerPage( SYSTEM_SHOW_LINE );
	// Up button
	//m_sbSysScrollBar.SetUpUV( UBS_IDLE, 117, 0, 128, 8, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_CLICK );
	m_sbSysScrollBar.SetUpUV( UBS_IDLE, 252, 218, 265, 231, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetUpUV( UBS_CLICK, 268, 218, 281, 231, fTexWidth, fTexHeight );	
	m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	//m_sbSysScrollBar.SetBarTopUV( 105, 0, 116, 9, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.SetBarMiddleUV( 105, 8, 116, 11, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.SetBarBottomUV( 105, 10, 116, 19, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarTopUV( 238, 218, 251, 222, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarMiddleUV( 238, 222, 251, 227, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarBottomUV( 238, 227, 251, 233, fTexWidth, fTexHeight );
	// Down button
	//m_sbSysScrollBar.SetDownUV( UBS_IDLE, 117, 10, 128, 18, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_CLICK );
	m_sbSysScrollBar.SetDownUV( UBS_IDLE, 252, 233, 265, 246, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetDownUV( UBS_CLICK, 268, 233, 281, 246, fTexWidth, fTexHeight );
	m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	// Option region
	int	nMaxSize = _S( 452, "일반" ).Length();

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
 int nSize = _S( 453, "파티(?)" ).Length();
#else
 int nSize = _S( 453, "파티(@)" ).Length();
#endif

	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 454, "길드(#)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 455, "매매($)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 456, "귓속말(%)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = strlen( _S( 459, "외치기(%)" ) );
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 457, "시스템" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	
	nMaxSize *= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int	nOptionHeight = ( _pUIFontTexMgr->GetFontHeight() + 3 ) * CHATOPT_TOTAL + 15 - _pUIFontTexMgr->GetLineSpacing();
	m_rcOption.SetRect( 323, -35 - nOptionHeight, 323 + nMaxSize + 39, -35 );

	// Chatting option check button
	nMaxSize += 10;
	nPosX = m_rcOption.Right - 22;
	int	nPosY = m_rcOption.Top + 6;

	fTexWidth = m_ptdButtonTexture ->GetPixWidth();
	fTexHeight = m_ptdButtonTexture ->GetPixHeight();

	for( i = 0; i < CHATOPT_TOTAL; i++ )
	{
		m_cbtnChatOption[i].Create( this, nPosX, nPosY, 11, 11, CTString( "" ), TRUE, nMaxSize, nMaxSize + 4 );
		//m_cbtnChatOption[i].SetUV( UCBS_CHECK, 0, 53, 11, 64, fTexWidth, fTexHeight );
		//m_cbtnChatOption[i].SetUV( UCBS_NONE, 12, 53, 23, 64, fTexWidth, fTexHeight );
		//m_cbtnChatOption[i].SetUV( UCBS_CHECK_DISABLE, 25, 53, 35, 64, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );		
		m_cbtnChatOption[i].SetUV( UCBS_CHECK_DISABLE, 139, 75, 152, 88, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
		m_cbtnChatOption[i].SetCheck( TRUE );
		nPosY += _pUIFontTexMgr->GetFontHeight() + 3;
	}
	m_cbtnChatOption[CHATOPT_NORMAL].SetText( _S( 452, "일반" ) );

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
 m_cbtnChatOption[CHATOPT_PARTY].SetText( _S( 453, "파티(?)" ) );
#else
 m_cbtnChatOption[CHATOPT_PARTY].SetText( _S( 453, "파티(@)" ) );
#endif

	m_cbtnChatOption[CHATOPT_GUILD].SetText( _S( 454, "길드(#)" ) );
	m_cbtnChatOption[CHATOPT_TRADE].SetText( _S( 455, "매매($)" ) );
	m_cbtnChatOption[CHATOPT_WHISPER].SetText( _S( 456, "귓속말(%)" ) );
	m_cbtnChatOption[CHATOPT_SHOUT].SetText( _S( 459, "외치기(%)" ) );			
	m_cbtnChatOption[CHATOPT_SYSTEM].SetText( _S( 457, "시스템" ) );
	
	//m_cbtnChatOption[CHATOPT_NORMAL].SetEnable( FALSE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	//SetPos( pixMinI, pixMaxJ );
	SetPos( pixMinI, pixMaxJ -28 );

	// Resize chatting region
	int	nLineOffset = m_nCurChatShowLine - MIN_SHOW_CHATLINE;
	for( int i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nFirstChatRow[i] += nLineOffset;

		if( m_nFirstChatRow[i] < 0 )
			m_nFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;
			if( m_nFirstChatRow[i] > nLimitRow )
				m_nFirstChatRow[i] = nLimitRow;
		}
	}

	// Maximum chattine line
	m_nMaxChatShowLine = ( pixMaxJ - pixMinJ - 160 - _pUIMgr->GetPlayerInfo()->GetHeight() ) /
							( _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING );

	// Chatting region
	m_nCurChatShowLine = MIN_SHOW_CHATLINE;
	int	nChatHeight = m_nChatLineHeight * MIN_SHOW_CHATLINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcChatting.SetRect( 0, -44 - nChatHeight, m_nWidth, -44 );

	// Scroll bar of chatting
	//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
	m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
	m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
	m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );

	// System region
	int	nHeight = m_rcSystem.GetHeight();
	int	nBottom = m_rcChatting.Top - 1;
	m_rcSystem.SetRect( 0, nBottom - nHeight, m_nWidth, nBottom );

	// Scroll bar of system
	//m_sbSysScrollBar.SetPosY( m_rcSystem.Top + 3 );
	m_sbSysScrollBar.SetPosY( m_rcSystem.Top + SCROLLBAR_ADJUST_Y );

	// Resizing frame region
	if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] )
		m_rcResizeFrame.SetRect( 0, m_rcSystem.Top, m_nWidth, m_rcSystem.Top + 10 );
	else
		m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	//SetPos( pixMinI, pixMaxJ );
	SetPos( pixMinI, pixMaxJ -28 );

	// Maximum chattine line
	m_nMaxChatShowLine = ( pixMaxJ - pixMinJ - 160 - _pUIMgr->GetPlayerInfo()->GetHeight() ) /
							( _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING );

	// Resize chatting region
	if( m_nCurChatShowLine > m_nMaxChatShowLine )
	{
		int	nLineOffset = m_nCurChatShowLine - m_nMaxChatShowLine;
		for( int i = 0; i < CHATTAB_TOTAL; i++ )
		{
			m_nFirstChatRow[i] += nLineOffset;

			if( m_nFirstChatRow[i] < 0 )
				m_nFirstChatRow[i] = 0;
			else
			{
				int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
				if( nLimitRow < 0 )
					nLimitRow = 0;
				if( m_nFirstChatRow[i] > nLimitRow )
					m_nFirstChatRow[i] = nLimitRow;
			}
		}

		// Chatting region
		m_nCurChatShowLine = m_nMaxChatShowLine;
		int	nChatHeight = m_nChatLineHeight * m_nCurChatShowLine + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
		m_rcChatting.SetRect( 0, -44 - nChatHeight, m_nWidth, -44 );

		// Scroll bar of chatting
		//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
		m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
		m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
		m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );

		// System region
		int	nHeight = m_rcSystem.GetHeight();
		int	nBottom = m_rcChatting.Top - 1;
		m_rcSystem.SetRect( 0, nBottom - nHeight, m_nWidth, nBottom );

		// Scroll bar of system
		//m_sbSysScrollBar.SetPosY( m_rcSystem.Top + 3 );
		m_sbSysScrollBar.SetPosY( m_rcSystem.Top + SCROLLBAR_ADJUST_Y );

		// Resizing frame region
		if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] )
			m_rcResizeFrame.SetRect( 0, m_rcSystem.Top, m_nWidth, m_rcSystem.Top + 10 );
		else
			m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );
	}
}

// ----------------------------------------------------------------------------
// Name : ResetChatting()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ResetChatting()
{
	for( int i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nCurChatCount[i] = 0;
		m_nCurChatInsert[i] = 0;
		m_nFirstChatRow[i] = 0;
	}
	m_nCurSysCount = 0;
	m_nCurSysInsert = 0;
	m_nFirstSysRow = 0;

	m_bShowWhisperHistory = FALSE;
	m_nWhisperCount = 0;
	m_nCurWhisper = 0;

	m_sbChatScrollBar.SetScrollPos( 0 );
	m_sbChatScrollBar.SetCurItemCount( 0 );

	m_sbSysScrollBar.SetScrollPos( 0 );
	m_sbSysScrollBar.SetCurItemCount( 0 );
}

// ----------------------------------------------------------------------------
// Name : ChangeChattingBoxHeight()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ChangeChattingBoxHeight( int ndLines )
{
	int	ndHeight = ndLines * m_nChatLineHeight;

	// Adjust chatting message box
	m_rcChatting.Top -= ndHeight;
	m_nCurChatShowLine += ndLines;
	//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
	m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
	m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
	m_sbChatScrollBar.ChangeSize( ndHeight );	

	for( int i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nFirstChatRow[i] -= ndLines;
		if( m_nFirstChatRow[i] < 0 )
			m_nFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;

			if( m_nFirstChatRow[i] > nLimitRow )
				m_nFirstChatRow[i] = nLimitRow;
		}
	}
	m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );

	// Adjust system message box
	m_rcSystem.Top -= ndHeight;
	m_rcSystem.Bottom -= ndHeight;
	//m_sbSysScrollBar.SetPosY( m_rcSystem.Top + 3 );
	m_sbSysScrollBar.SetPosY( m_rcSystem.Top + SCROLLBAR_ADJUST_Y );

	// Adjust resizing frame region
	m_rcResizeFrame.Top -= ndHeight;
	m_rcResizeFrame.Bottom -= ndHeight;
}

// ----------------------------------------------------------------------------
// Name : RenderChatStringList()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::RenderChatStringList()
{
	int	nFirstChat, nFirstChatLast, nSecondChat, nSecondChatLast;

	// Just render in turn
	if( m_nCurChatCount[m_nCurSelTab] < MAX_CHAT_LINE || m_nCurChatInsert[m_nCurSelTab] == MAX_CHAT_LINE )
	{
		nFirstChat = m_nFirstChatRow[m_nCurSelTab];
		nFirstChatLast = nFirstChat + m_nCurChatShowLine;
		nSecondChat = nSecondChatLast = 0;

		if( nFirstChatLast > m_nCurChatCount[m_nCurSelTab] )
			nFirstChatLast = m_nCurChatCount[m_nCurSelTab];
	}
	// Split chatting list into two
	else
	{
		nFirstChat = m_nFirstChatRow[m_nCurSelTab] + m_nCurChatInsert[m_nCurSelTab];
		if( nFirstChat >= MAX_CHAT_LINE )
			nFirstChat -= MAX_CHAT_LINE;
		nFirstChatLast = nFirstChat + m_nCurChatShowLine;

		if( nFirstChatLast > MAX_CHAT_LINE )
		{
			nSecondChat = 0;
			nSecondChatLast = nFirstChatLast - MAX_CHAT_LINE;
			nFirstChatLast = MAX_CHAT_LINE;
		}
		else
			nSecondChat = nSecondChatLast = 0;
	}

	// Render chatting strings
	int	nX = m_nPosX + m_rcChatting.Left + CHAT_STRING_OFFSETX;
	int	nY = m_nPosY + m_rcChatting.Top + CHAT_STRING_OFFSTY;
	for( int iChat = nFirstChat; iChat < nFirstChatLast; iChat++ )
	{
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
		RenderChatNameMsgString(nX, nY, iChat);
#else
		_pUIMgr->GetDrawPort()->PutTextEx( m_strChatString[m_nCurSelTab][iChat], nX, nY,
											m_colChatString[m_nCurSelTab][iChat], TRUE, 0x181818FF );
#endif
		nY += m_nChatLineHeight;
	}
	for( iChat = nSecondChat; iChat < nSecondChatLast; iChat++ )
	{
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
		RenderChatNameMsgString(nX, nY, iChat);
#else
		_pUIMgr->GetDrawPort()->PutTextEx( m_strChatString[m_nCurSelTab][iChat], nX, nY,
											m_colChatString[m_nCurSelTab][iChat], TRUE, 0x181818FF );
#endif
		nY += m_nChatLineHeight;
	}
}

#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
void
CUIChatting::RenderChatNameMsgString(const int iPosX, const int iPosY, const int iChatLine)
{
		CTString	strTemp = m_strChatString[m_nCurSelTab][iChatLine];	
		CTString	strCompare = CTString("  ");
		int			istrCompareLen = strCompare.Length();
		char		cFirstChar[2] = {strTemp.str_String[0], strTemp.str_String[1]};
		int			nNameWid = (_pUIFontTexMgr->GetFontSpacing() + m_sChatInfo.strSendName[m_nCurSelTab][iChatLine].Length()) *
								(_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
		
		if (strTemp.RemovePrefix(m_sChatInfo.strSendName[m_nCurSelTab][iChatLine]))
			m_sChatInfo.strMsg = strTemp;					
		else
			m_sChatInfo.strMsg = strTemp;

		if (strncmp(cFirstChar, strCompare, istrCompareLen) == 0)
			_pUIMgr->GetDrawPort()->PutTextEx(m_sChatInfo.strMsg, iPosX, iPosY, m_colChatString[m_nCurSelTab][iChatLine], TRUE, 0x181818FF);//msg				
		else
		{
			_pUIMgr->GetDrawPort()->PutTextEx(m_sChatInfo.strSendName[m_nCurSelTab][iChatLine], iPosX, iPosY, m_sChatInfo.colSendName[m_nCurSelTab][iChatLine]);//, TRUE, 0x181818FF);//name
			_pUIMgr->GetDrawPort()->PutTextEx(m_sChatInfo.strMsg, iPosX+nNameWid, iPosY, m_colChatString[m_nCurSelTab][iChatLine], TRUE, 0x181818FF);//msg
		}		
}
#endif

// ----------------------------------------------------------------------------
// Name : RenderSysStringList()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::RenderSysStringList()
{
	int	nFirstChat, nFirstChatLast, nSecondChat, nSecondChatLast;

	// Just render in turn
	if( m_nCurSysCount < MAX_SYSTEM_LINE || m_nCurSysInsert == MAX_SYSTEM_LINE )
	{
		nFirstChat = m_nFirstSysRow;
		nFirstChatLast = nFirstChat + SYSTEM_SHOW_LINE;
		nSecondChat = nSecondChatLast = 0;

		if( nFirstChatLast > m_nCurSysCount )
			nFirstChatLast = m_nCurSysCount;
	}
	// Split chatting list into two
	else
	{
		nFirstChat = m_nFirstSysRow + m_nCurSysInsert;
		if( nFirstChat >= MAX_SYSTEM_LINE )
			nFirstChat -= MAX_SYSTEM_LINE;
		nFirstChatLast = nFirstChat + SYSTEM_SHOW_LINE;

		if( nFirstChatLast > MAX_SYSTEM_LINE )
		{
			nSecondChat = 0;
			nSecondChatLast = nFirstChatLast - MAX_SYSTEM_LINE;
			nFirstChatLast = MAX_SYSTEM_LINE;
		}
		else
			nSecondChat = nSecondChatLast = 0;
	}

	// Render chatting strings
	int	nX = m_nPosX + m_rcSystem.Left + CHAT_STRING_OFFSETX;
	int	nY = m_nPosY + m_rcSystem.Top + CHAT_STRING_OFFSTY;
	for( int iChat = nFirstChat; iChat < nFirstChatLast; iChat++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strSysString[iChat], nX, nY,
											m_colSysString[iChat], TRUE, 0x181818FF );
		nY += m_nChatLineHeight;
	}
	for( iChat = nSecondChat; iChat < nSecondChatLast; iChat++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strSysString[iChat], nX, nY,
											m_colSysString[iChat], TRUE, 0x181818FF );
		nY += m_nChatLineHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::Render()
{
	// Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	// Input box
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcInput.Left, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Left + 25, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcInput.Left + 25, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Right - 25, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcInput.Right - 25, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Right, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
										0xFFFFFFFF );

	// Chatting message region
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatML.U0, m_rtChatML.V0, m_rtChatML.U1, m_rtChatML.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatMM.U0, m_rtChatMM.V0, m_rtChatMM.U1, m_rtChatMM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatMR.U0, m_rtChatMR.V0, m_rtChatMR.U1, m_rtChatMR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLL.U0, m_rtChatLL.V0, m_rtChatLL.U1, m_rtChatLL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLM.U0, m_rtChatLM.V0, m_rtChatLM.U1, m_rtChatLM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLR.U0, m_rtChatLR.V0, m_rtChatLR.U1, m_rtChatLR.V1,
										0xFFFFFFFF );

	// System message region
	if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] )
	{
		// Chatting message region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUL2.U0, m_rtChatUL2.V0, m_rtChatUL2.U1, m_rtChatUL2.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUM2.U0, m_rtChatUM2.V0, m_rtChatUM2.U1, m_rtChatUM2.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUR2.U0, m_rtChatUR2.V0, m_rtChatUR2.U1, m_rtChatUR2.V1,
											0xFFFFFFFF );

		// System message region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUL.U0, m_rtChatUL.V0, m_rtChatUL.U1, m_rtChatUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUM.U0, m_rtChatUM.V0, m_rtChatUM.U1, m_rtChatUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUR.U0, m_rtChatUR.V0, m_rtChatUR.U1, m_rtChatUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatML.U0, m_rtChatML.V0, m_rtChatML.U1, m_rtChatML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatMM.U0, m_rtChatMM.V0, m_rtChatMM.U1, m_rtChatMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatMR.U0, m_rtChatMR.V0, m_rtChatMR.U1, m_rtChatMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLL.U0, m_rtChatLL.V0, m_rtChatLL.U1, m_rtChatLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLM.U0, m_rtChatLM.V0, m_rtChatLM.U1, m_rtChatLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLR.U0, m_rtChatLR.V0, m_rtChatLR.U1, m_rtChatLR.V1,
											0xFFFFFFFF );

		// Scroll bar of System message
		m_sbSysScrollBar.Render();

		// System string list
		RenderSysStringList();
	}
	else
	{
		// Chatting message region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUL.U0, m_rtChatUL.V0, m_rtChatUL.U1, m_rtChatUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUM.U0, m_rtChatUM.V0, m_rtChatUM.U1, m_rtChatUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUR.U0, m_rtChatUR.V0, m_rtChatUR.U1, m_rtChatUR.V1,
											0xFFFFFFFF );
	}

	// Scroll bar of chatting message
	m_sbChatScrollBar.Render();

	// Chatting string list
	RenderChatStringList();

	// Option button
	m_btnOption.Render();

	// Option region
	if( m_bShowOption )
	{
		// Option region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUL.U0, m_rtWhisperUL.V0, m_rtWhisperUL.U1, m_rtWhisperUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUM.U0, m_rtWhisperUM.V0, m_rtWhisperUM.U1, m_rtWhisperUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUR.U0, m_rtWhisperUR.V0, m_rtWhisperUR.U1, m_rtWhisperUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperML.U0, m_rtWhisperML.V0, m_rtWhisperML.U1, m_rtWhisperML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperMM.U0, m_rtWhisperMM.V0, m_rtWhisperMM.U1, m_rtWhisperMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperMR.U0, m_rtWhisperMR.V0, m_rtWhisperMR.U1, m_rtWhisperMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLL.U0, m_rtWhisperLL.V0, m_rtWhisperLL.U1, m_rtWhisperLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLM.U0, m_rtWhisperLM.V0, m_rtWhisperLM.U1, m_rtWhisperLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLR.U0, m_rtWhisperLR.V0, m_rtWhisperLR.U1, m_rtWhisperLR.V1,
											0xFFFFFFFF );

		for( int i = 0; i < CHATOPT_TOTAL; i++ )
		{
			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );
			m_cbtnChatOption[i].Render();
			
			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
		}
	}

	// Tab region
	if( m_nCurSelTab != m_nOldSelTab )		// Animation
	{
		static int		nCurTab = m_nCurSelTab;
		static BOOL		bFirstEntering = TRUE;
		static __int64	llOldTime;
		static __int64	llElapsedTime;
		__int64			llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		if( bFirstEntering || nCurTab != m_nCurSelTab )
		{
			llOldTime = llCurTime;
			llElapsedTime = 0;
			bFirstEntering = FALSE;
			nCurTab = m_nCurSelTab;
		}

		llElapsedTime += llCurTime - llOldTime;
		llOldTime = llCurTime;

		FLOAT	fRatio = llElapsedTime / 300.0f;
		if( llElapsedTime > 300 )
			fRatio = 1.0f;

		int	nGoalWidth, nResize;
		for( int i = 0; i < CHATTAB_TOTAL; i++ )
		{
			if( i == m_nCurSelTab )
				nGoalWidth = CHAT_TAB_SEL_WIDTH;
			else
				nGoalWidth = CHAT_TAB_WIDTH;

			nResize = ( nGoalWidth - m_rcTab[i].GetWidth() ) * fRatio;
			m_rcTab[i].Right += nResize;
		}
			
		for( i = 0; i < CHATTAB_TOTAL; i++ )
		{
			int	nWidth = m_rcTab[i].GetWidth();

			if( i == 0 )
				m_rcTab[i].Left = 0;
			else
				m_rcTab[i].Left = m_rcTab[i - 1].Right + CHAT_TAB_OFFSET;

			m_rcTab[i].Right = m_rcTab[i].Left + nWidth;
		}

		if( llElapsedTime > 300 )
		{
			llElapsedTime = 0;
			m_nOldSelTab = m_nCurSelTab;
			bFirstEntering = TRUE;
		}
	}

	COLOR	colTab;
	for( int iTab = 0; iTab < CHATTAB_TOTAL; iTab++ )
	{
		if( iTab == m_nCurSelTab )
			colTab = 0xFFFFFFE5;
		else
			colTab = 0xFFFFFF9A;

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab[iTab].Left, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Left + 25, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabL.U0, m_rtTabL.V0, m_rtTabL.U1, m_rtTabL.V1,
											colTab );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab[iTab].Left + 25, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Right - 25, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabM.U0, m_rtTabM.V0, m_rtTabM.U1, m_rtTabM.V1,
											colTab );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab[iTab].Right - 25, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Right, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabR.U0, m_rtTabR.V0, m_rtTabR.U1, m_rtTabR.V1,
											colTab );
	}

	// Edit box
	m_ebChatInput.Render();

	// Tab text
	int	nY = m_nPosY + m_rcTab[CHATTAB_ALL].Top + 3;
	int	nX = m_nPosX + ( m_rcTab[CHATTAB_ALL].Left + m_rcTab[CHATTAB_ALL].Right ) / 2;
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 506, "전체" ), nX, nY,								
											m_nCurSelTab == CHATTAB_ALL ? 0xF8F8F7FF : 0xAFACA6FF );
	nX = m_nPosX + ( m_rcTab[CHATTAB_PARTY].Left + m_rcTab[CHATTAB_PARTY].Right ) / 2;
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 507, "파티" ), nX, nY,								
											m_nCurSelTab == CHATTAB_PARTY ? 0xF8F8F7FF : 0xAFACA6FF );
	nX = m_nPosX + ( m_rcTab[CHATTAB_GUILD].Left + m_rcTab[CHATTAB_GUILD].Right ) / 2;
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 508, "길드" ), nX, nY,								
											m_nCurSelTab == CHATTAB_GUILD ? 0xF8F8F7FF : 0xAFACA6FF );
	nX = m_nPosX + ( m_rcTab[CHATTAB_TRADE].Left + m_rcTab[CHATTAB_TRADE].Right ) / 2;
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 509, "매매" ), nX, nY,								
											m_nCurSelTab == CHATTAB_TRADE ? 0xF8F8F7FF : 0xAFACA6FF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Whisper history region
	if( m_bShowWhisperHistory )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Option region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUL.U0, m_rtWhisperUL.V0, m_rtWhisperUL.U1, m_rtWhisperUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUM.U0, m_rtWhisperUM.V0, m_rtWhisperUM.U1, m_rtWhisperUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUR.U0, m_rtWhisperUR.V0, m_rtWhisperUR.U1, m_rtWhisperUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperML.U0, m_rtWhisperML.V0, m_rtWhisperML.U1, m_rtWhisperML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperMM.U0, m_rtWhisperMM.V0, m_rtWhisperMM.U1, m_rtWhisperMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperMR.U0, m_rtWhisperMR.V0, m_rtWhisperMR.U1, m_rtWhisperMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLL.U0, m_rtWhisperLL.V0, m_rtWhisperLL.U1, m_rtWhisperLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLM.U0, m_rtWhisperLM.V0, m_rtWhisperLM.U1, m_rtWhisperLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLR.U0, m_rtWhisperLR.V0, m_rtWhisperLR.U1, m_rtWhisperLR.V1,
											0xFFFFFFFF );

		// Selected region
		nY = m_nPosY + m_rcWhisper.Bottom - 5;
		nY -= m_nChatLineHeight * m_nCurWhisper;
		//_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcSelWhisper.Left, nY + m_rcSelWhisper.Top,
		//									m_nPosX + m_rcSelWhisper.Right, nY + m_rcSelWhisper.Bottom,
		//									m_rtSelWhisper.U0, m_rtSelWhisper.V0, m_rtSelWhisper.U1, m_rtSelWhisper.V1,
		//									0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Text in whisper history
		nX = m_nPosX + m_rcWhisper.Left + 8;
		nY = m_nPosY + m_rcWhisper.Bottom - m_nChatLineHeight - 4;
		for( int i = 0; i < m_nWhisperCount; i++ )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strWhisper[i], nX, nY, 0xC5C5C5FF );
			nY -= m_nChatLineHeight;
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}

	// Reading window
	if( m_ebChatInput.DoesShowReadingWindow() )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebChatInput.RenderReadingWindow();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : AddChatString()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddChatString( CTString &strChatString, COLOR colChatString, int nCurTab, BOOL bNotice )
{
	// Get length of string
	INDEX	nLength = strChatString.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strChatString);
		INDEX	nChatMax= (CHATMSG_CHAR_MAX-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			if( m_nCurChatCount[nCurTab] < MAX_CHAT_LINE )
				m_nCurChatCount[nCurTab]++;

			int	nInsertIndex = m_nCurChatInsert[nCurTab];
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = m_nCurChatInsert[nCurTab] = 0;

			m_strChatString[nCurTab][nInsertIndex] = strChatString;
			m_colChatString[nCurTab][nInsertIndex] = colChatString;
			m_nCurChatInsert[nCurTab]++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = CHATMSG_CHAR_MAX;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strChatString,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			if( m_nCurChatCount[nCurTab] < MAX_CHAT_LINE )
				m_nCurChatCount[nCurTab]++;

			int	nInsertIndex = m_nCurChatInsert[nCurTab];
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = m_nCurChatInsert[nCurTab] = 0;
			strChatString.Split( nSplitPos, m_strChatString[nCurTab][nInsertIndex], strTemp );
			m_colChatString[nCurTab][nInsertIndex] = colChatString;
			m_nCurChatInsert[nCurTab]++;

			if( !bNotice )
				strTemp = CTString( "  " ) + strTemp;

			AddChatString( strTemp, colChatString, nCurTab, bNotice );
		}
		
	} else{
		// If length of string is less than max char
		if( nLength <= m_nMaxCharCount )
		{
			if( m_nCurChatCount[nCurTab] < MAX_CHAT_LINE )
				m_nCurChatCount[nCurTab]++;

			int	nInsertIndex = m_nCurChatInsert[nCurTab];
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = m_nCurChatInsert[nCurTab] = 0;

#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
			m_sChatInfo.strSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
			m_sChatInfo.colSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
#endif			
			m_strChatString[nCurTab][nInsertIndex] = strChatString;
			m_colChatString[nCurTab][nInsertIndex] = colChatString;
			m_nCurChatInsert[nCurTab]++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strChatString[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			if( m_nCurChatCount[nCurTab] < MAX_CHAT_LINE )
				m_nCurChatCount[nCurTab]++;

			int	nInsertIndex = m_nCurChatInsert[nCurTab];
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = m_nCurChatInsert[nCurTab] = 0;

#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
			m_sChatInfo.strSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
			m_sChatInfo.colSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
#endif
			strChatString.Split( nSplitPos, m_strChatString[nCurTab][nInsertIndex], strTemp );
			m_colChatString[nCurTab][nInsertIndex] = colChatString;
			m_nCurChatInsert[nCurTab]++;

			if( !bNotice )
				strTemp = CTString( "  " ) + strTemp;

			AddChatString( strTemp, colChatString, nCurTab, bNotice );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddSysString()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddSysString( CTString &strSysString, COLOR colSysString )
{
	// Get length of string
	INDEX	nLength = strSysString.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strSysString);
		INDEX	nChatMax= (m_nMaxCharCount-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			m_strSysString[nInsertIndex] = strSysString;
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strSysString,0,iPos) )
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			strSysString.Split( nSplitPos, m_strSysString[nInsertIndex], strTemp );
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;

			AddSysString( strTemp, colSysString );
		}
		
	} else{
		// If length of string is less than max char
		if( nLength <= m_nMaxCharCount )
		{
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			m_strSysString[nInsertIndex] = strSysString;
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strSysString[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			strSysString.Split( nSplitPos, m_strSysString[nInsertIndex], strTemp );
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;

			AddSysString( strTemp, colSysString );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddWhisperTarget()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddWhisperTarget( CTString &strWhisper )
{
	m_nCurWhisper = 0;

	for( int i = 0; i < m_nWhisperCount; i++ )
	{
		if( strWhisper.IsEqualCaseSensitive( m_strWhisper[i] ) )
			break;
	}

	if( i == m_nWhisperCount && m_nWhisperCount < MAX_WHISPER_SAVE )
		m_nWhisperCount++;

	if( i >= MAX_WHISPER_SAVE )
		i--;

	for( int j = i; j > 0; j-- )
		m_strWhisper[j] = m_strWhisper[j - 1];

	m_strWhisper[0] = strWhisper;

	m_rcWhisper.Top = m_rcWhisper.Bottom - m_nWhisperCount * m_nChatLineHeight - 7; 
}

// [KH_070423] 추가
void CUIChatting::OpenAndSetString(CTString strIn)
{
	m_ebChatInput.SetString(strIn.str_String);
	m_ebChatInput.SetCursorIndex(strIn.Length());	
	m_ebChatInput.SetFocus( TRUE );
	_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::KeyMessage( MSG *pMsg )
{
	extern INDEX	g_iEnterChat;
	if( g_iEnterChat )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = FALSE;

			WMSG_RESULT	wmsgResult;
			if( !m_ebChatInput.IsFocused() )
			{
				m_ebChatInput.SetFocus( TRUE );
				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );

				return WMSG_SUCCESS;
			}
			else if( ( wmsgResult = m_ebChatInput.KeyMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					SendChatMessage( m_ebChatInput.GetString() );

				m_ebChatInput.SetFocus( FALSE );
				return WMSG_SUCCESS;
			}
		}
		else if( m_ebChatInput.IsFocused() && m_ebChatInput.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = FALSE;

			return WMSG_SUCCESS;
		}
		else if( m_bShowWhisperHistory )
		{
			if( pMsg->wParam == VK_UP )
			{
				if( m_nCurWhisper + 1 < m_nWhisperCount )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper++;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = FALSE;
		}
		else // Date : 2005-02-16,   By Lee Ki-hwan
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				if( !m_ebChatInput.IsFocused() ) return WMSG_FAIL;
				if( m_strMsgBuffer.size() == 0 ) return WMSG_FAIL;
				
				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}
							
					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
						
					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_strMsgBuffer.size();
						m_ebChatInput.ResetString();
						m_ebChatInput.SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
											
					return WMSG_SUCCESS;
				}
			}
		}
	}
	else
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = FALSE;

			if( !m_ebChatInput.IsFocused() )
			{
				m_ebChatInput.SetFocus( TRUE );
				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
			}

			if( m_ebChatInput.KeyMessage( pMsg ) == WMSG_COMMAND )
				SendChatMessage( m_ebChatInput.GetString() );

			return WMSG_SUCCESS;
		}
		else if( m_ebChatInput.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = FALSE;

			return WMSG_SUCCESS;
		}
		else if( m_bShowWhisperHistory )
		{
			if( pMsg->wParam == VK_UP )
			{
				if( m_nCurWhisper + 1 < m_nWhisperCount )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper++;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = FALSE;
		}
		else // Date : 2005-02-16,   By Lee Ki-hwan
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				//if( !m_ebChatInput.IsFocused() ) return WMSG_FAIL;
				if( m_strMsgBuffer.size() == 0 ) return WMSG_FAIL;
				
				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}
							
					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
						
					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_strMsgBuffer.size();
						m_ebChatInput.ResetString();
						m_ebChatInput.SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
											
					return WMSG_SUCCESS;

				}
			}
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::CharMessage( MSG *pMsg )
{
	if( m_ebChatInput.CharMessage( pMsg ) != WMSG_FAIL )
	{
		if( m_bShowWhisperHistory )
			m_bShowWhisperHistory = FALSE;

		if( pMsg->wParam == '!' )
		{
			int	nLength = strlen( m_ebChatInput.GetString() );
			if( nLength == 1 && m_strWhisper[m_nCurWhisper].Length() > 0 )
			{
				m_ebChatInput.InsertChars( nLength, m_strWhisper[m_nCurWhisper].str_String );
				nLength = m_strWhisper[m_nCurWhisper].Length() + 1;
				m_ebChatInput.InsertChar( nLength, ' ' );

				m_bShowWhisperHistory = TRUE;
			}
		}

		return WMSG_SUCCESS;
	} 

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIChatting::IMEMessage( MSG *pMsg )
{
	if( m_bShowWhisperHistory )
		m_bShowWhisperHistory = FALSE;

	// 이기환 수정 시작 (11. 15) : return 값 조절을 위해 로직 변경
	return m_ebChatInput.IMEMessage( pMsg );
	// 이기환 수정 끝 
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL	bTopFrameClick = FALSE;
	static int	nHeightStretch = 0;			// Stretched height of top frame
	static BOOL	bInOptionPopup = FALSE;		// If mouse is in option popup or not

	// Mouse point
	static int	nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Popup of option
			if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				_pUIMgr->SetMouseCursorInsideUIs();
				bInOptionPopup = TRUE;

				return WMSG_SUCCESS;
			}
			else
			{
				// Close option popup
				if( bInOptionPopup )
				{
					_pUIMgr->SetMouseCursorInsideUIs();
					bInOptionPopup = FALSE;
					m_bShowOption = FALSE;

					return WMSG_SUCCESS;
				}
				// Edit box
				else if( m_ebChatInput.IsInside( nX, nY ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs();

					return WMSG_SUCCESS;
				}
				// Top frame moving
				else if( bTopFrameClick && pMsg->wParam & MK_LBUTTON )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );

					int	ndLineCount;
					int	ndY = nY - nOldY;
					nOldY = nY;

					nHeightStretch -= ndY;
					if( nHeightStretch > 0 )
					{
						if( nHeightStretch >= m_nChatLineHeight && m_nCurChatShowLine < m_nMaxChatShowLine )
						{
							ndLineCount = nHeightStretch / m_nChatLineHeight;
							if( m_nCurChatShowLine + ndLineCount > m_nMaxChatShowLine )
								ndLineCount = m_nMaxChatShowLine - m_nCurChatShowLine;

							nHeightStretch -= ndLineCount * m_nChatLineHeight;
							ChangeChattingBoxHeight( ndLineCount );
						}
					}
					else
					{
						if( nHeightStretch <= -m_nChatLineHeight && m_nCurChatShowLine > MIN_SHOW_CHATLINE )
						{
							ndLineCount = nHeightStretch / m_nChatLineHeight;
							if( m_nCurChatShowLine + ndLineCount < MIN_SHOW_CHATLINE )
								ndLineCount = MIN_SHOW_CHATLINE - m_nCurChatShowLine;

							nHeightStretch -= ndLineCount * m_nChatLineHeight;
							ChangeChattingBoxHeight( ndLineCount );
						}
					}

					return WMSG_SUCCESS;
				}
				// Top frame
				else if( IsInsideRect( nX, nY, m_rcResizeFrame ) && !_pUIMgr->IsInsideUpperUIs( UI_MAP, nX, nY ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );

					return WMSG_SUCCESS;
				}
				// Scroll bar of chatting message
				else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					_pUIMgr->SetMouseCursorInsideUIs();

					if( wmsgResult == WMSG_COMMAND )
						m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();

					return WMSG_SUCCESS;
				}
				// Scroll bar of system message
				else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
					( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					_pUIMgr->SetMouseCursorInsideUIs();

					if( wmsgResult == WMSG_COMMAND )
						m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

					return WMSG_SUCCESS;
				}
				// Option button
				else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					_pUIMgr->SetMouseCursorInsideUIs();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			// Close whisper history popup
			m_bShowWhisperHistory = FALSE;

			// Chatting input box
			if( m_ebChatInput.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Scroll bar of chatting message
			else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();

				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Top frame
			else if( IsInsideRect( nX, nY, m_rcResizeFrame ) )
			{
				bTopFrameClick = TRUE;
				nHeightStretch = 0;
				nOldY = nY;

				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Tab region
			else if( IsInsideRect( nX, nY, m_rcAllTab ) )
			{
				for( int i = 0; i < CHATTAB_TOTAL; i++ )
				{
					if( IsInsideRect( nX, nY, m_rcTab[i] ) )
					{
						if( m_nCurSelTab != i )
						{
							m_nOldSelTab = m_nCurSelTab;
							m_nCurSelTab = i;
							m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[i] );

							for( int j = 0; j < CHATOPT_TOTAL; j++ )
							{
								m_cbtnChatOption[j].SetCheck( m_bChatOption[i][j] );
								m_cbtnChatOption[j].SetEnable( j != m_nCurSelTab );
							}

							InsertChatPrefix( ChattingMsgType( i ) );
						}
						break;
					}
				}

				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Option button
			else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Option check buttons
			else if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				for( int i = 0; i < CHATOPT_TOTAL; i++ )
				{
					if( m_cbtnChatOption[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_bChatOption[m_nCurSelTab][i] = m_cbtnChatOption[i].IsChecked();
						
						if( i == CHATOPT_SYSTEM )
						{
							if( m_bChatOption[m_nCurSelTab][i] )
								m_rcResizeFrame.SetRect( 0, m_rcSystem.Top, m_nWidth, m_rcSystem.Top + 10 );
							else
								m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );
						}

						break;
					}
				}

				_pUIMgr->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Top frame
				bTopFrameClick = FALSE;

				// If chatting box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Scroll bar of chatting message
				if( m_sbChatScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Scroll bar of system message
				else if( m_sbSysScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Option button
				else if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						m_bShowOption = !m_bShowOption;

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			// Input box
			if( m_ebChatInput.IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
			// Tab region
			else if( IsInsideRect( nX, nY, m_rcAllTab ) )
			{
				return WMSG_SUCCESS;
			}
			// Option region
			else if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				return WMSG_SUCCESS;
			}
			// Scroll bar of chatting message
			else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			// Scroll bar of chatting message
			if( m_sbChatScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				m_sbSysScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : InsertChatPrefix()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::InsertChatPrefix( ChattingMsgType cmtType )
{

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224 
 static const char cPrefix[5] = { '!', '?', '#', '$', '%' };
#else
 static const char cPrefix[5] = { '!', '@', '#', '$', '%' };
#endif

	char				cFirstChar = m_ebChatInput.GetString()[0];
	int					nLength = strlen( m_ebChatInput.GetString() );

	switch( cmtType )
	{
	case CHATMSG_NORMAL:
	case CHATMSG_RANKER:
		if( nLength == 1 && ( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] ||
			cFirstChar == cPrefix[2] || cFirstChar == cPrefix[3] || cFirstChar == cPrefix[4] ) )
		{
			m_ebChatInput.DeleteChar( 0 );
		}
		break;

	case CHATMSG_PARTY:
		if( cFirstChar != cPrefix[1] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[2] || cFirstChar == cPrefix[3] ||
					cFirstChar == cPrefix[4] )
					m_ebChatInput.DeleteChar( 0 );

				m_ebChatInput.InsertChar( 0, cPrefix[1] );
			}
		}
		break;

	case CHATMSG_GUILD:
		if( cFirstChar != cPrefix[2] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] || cFirstChar == cPrefix[3] ||
					cFirstChar == cPrefix[4] )
					m_ebChatInput.DeleteChar( 0 );

				m_ebChatInput.InsertChar( 0, cPrefix[2] );
			}
		}
		break;

	case CHATMSG_TRADE:
		if( cFirstChar != cPrefix[3] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] || cFirstChar == cPrefix[2] ||
					cFirstChar == cPrefix[4] )
					m_ebChatInput.DeleteChar( 0 );

				m_ebChatInput.InsertChar( 0, cPrefix[3] );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ProcessParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ProcessParty( char *pcPartyString )
{
	// Extract sub command name from string
	int	nLength = strlen( pcPartyString );
	CTString	strCommand, strOther;
	for( int iChar = 0; iChar < nLength; iChar++ )
	{
		if( pcPartyString[iChar] == ' ' )
			break;
	}

	strOther = &pcPartyString[iChar + 1];
	pcPartyString[iChar] = NULL;
	strCommand = &pcPartyString[0];

	if( !strcmp( strCommand, _S( 124, "신청" ) ) )
		_pUIMgr->GetParty()->SendPartyInvite( 0, strOther );
	else if( !strcmp( strCommand, _S( 125, "탈퇴" ) ) )
		_pUIMgr->GetParty()->SendPartyQuit();
	else if( !strcmp( strCommand, _S( 126, "강퇴" ) ) )
		_pUIMgr->GetParty()->SendPartyKick( strOther );
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendChatMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::SendChatMessage( char *pcChatString, BOOL bLord )
{	
	// Start Date : 2005-02-16,   By Lee Ki-hwan
	if ( m_strMsgBuffer.size() > 1 )
	{
		if ( strcmp( pcChatString,  m_strMsgBuffer[m_strMsgBuffer.size()-1].str_String ) != 0 )
		{
			if ( m_strMsgBuffer.size() >= CHAT_MEMORY_COUNT )
			{
				m_strMsgBuffer.erase( m_strMsgBuffer.begin() );
			}

			m_strMsgBuffer.push_back ( CTString ( pcChatString ) );	
			m_iCurrentMsgBuffer = m_strMsgBuffer.size();
		}
		else
		{
			m_iCurrentMsgBuffer = m_strMsgBuffer.size();
		}
	}
	else 
	{
		if ( m_strMsgBuffer.size() >= CHAT_MEMORY_COUNT )
		{
			m_strMsgBuffer.erase( m_strMsgBuffer.begin() );
		}

		m_strMsgBuffer.push_back ( CTString ( pcChatString ) );	
		m_iCurrentMsgBuffer = m_strMsgBuffer.size();
	}
	// END Date : 2005-02-16,   By Lee Ki-hwan	

	// Find chatting message type
	UBYTE	ubType = CHATMSG_NORMAL;	

	switch( pcChatString[0] )
	{
#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
	case '?':
#else
	case '@':
#endif
		ubType = CHATMSG_PARTY;
		break;
	
	case '#':
		ubType = CHATMSG_GUILD;
		break;
	
	case '$':
		ubType = CHATMSG_TRADE;
		break;
	
	case '!':
		ubType = CHATMSG_WHISPER;
		break;

	case '%':
		ubType = CHATMSG_SHOUT;
		break;

	case '/':
		ubType = CHATMSG_COMMAND;
		break;

	// WSS_GMTOOL 070517
	case '^':
		ubType = CHATMSG_GMTOOL;
		break;
	}

	if( bLord ) ubType = CHATMSG_LORD;	

	// Check valid string
	int	nStart;
	if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER )
		nStart = 0;
	else
		nStart = 1;

	int	nLength = strlen( pcChatString );
	for( int iChar = nStart; iChar < nLength; iChar++ )
	{
		if( pcChatString[iChar] != ' ' )
			break;
	}

	if( pcChatString[iChar] == NULL )
	{
		// Reset string of input box
		m_ebChatInput.ResetString();
		InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );

		return;
	}

	// Chatting command message
	if( ubType == CHATMSG_COMMAND )
	{
		SendChatCommand( &(pcChatString[1] ), nLength - 1 );
		return;
	}

	// Split string to target name and chatting message
	CTString	strChatString;
	// WSS_GMTOOL 070517 --------------------------------------->><<
	if( ubType == CHATMSG_WHISPER || ubType == CHATMSG_GMTOOL)
	{
		for( iChar = 1; iChar < nLength; iChar++ )
		{
			if( pcChatString[iChar] == ' ' )
				break;
		}

		strChatString = &pcChatString[iChar + 1];
		pcChatString[iChar] = NULL;
		m_strWhisperTarget = &pcChatString[1];
	}
	else if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_LORD || ubType == CHATMSG_RANKER )
		strChatString = &pcChatString[0];
	else
		strChatString = &pcChatString[1];

	// 이기환 수정 시작 (11.29) : 불량 단어 필터링 
	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strChatString.str_String );

	// Date : 2005-01-10,   By Lee Ki-hwan
	// 운영자 인 경우에 필터링 안함
	if( _pNetwork->m_ubGMLevel > 1 )
	{
		// Date : 2005-04-08(오후 1:20:42), By Lee Ki-hwan
		/* commnet : 중국 Local GM의 경우 체팅 색 변경 요청에 의해 수정 */
		if( (g_iCountry == CHINA) && (ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER) )
		{
			ubType = CHATMSG_GM;
		}

		_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString );
	}
	else
	{
//[ttos_2009_1_23]:채팅 금지
#ifdef CHATTING_BAN
		if (_pNetwork->MyCharacterInfo.ChatFlag != 0 )
		{
			if (
				(ubType == CHATMSG_NORMAL && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_SAY) ||
				(ubType == CHATMSG_PARTY && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_PARTY) ||
				(ubType == CHATMSG_GUILD && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_GUILD) ||
				(ubType == CHATMSG_TRADE && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_TRADE) ||
				(ubType == CHATMSG_WHISPER && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_WHISPER) ||
				(ubType == CHATMSG_SHOUT && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_SHOUT)
				)
			{
				AddSysMessage(_S(4320,"채팅기능이 금지되어 대화를 할 수 없는 상태입니다. "));
				m_ebChatInput.ResetString();
				InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );
				return;
			}
		}
#endif
		// Filtering
		if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE )
		{
			strChatString.Clear();
			AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );

			strChatString.Clear();
#ifndef FILTERING_WORD_VISIBLE_NA_20081013
			for ( int i = 1; i <= nIndexBuffer[0]; i++ )
			{	
				strChatString += "[";	
				strChatString += _UIFiltering.GetString( nIndexBuffer[i] );
				strChatString += "] ";
			}
#endif			
			ubType = CHATMSG_NORMAL;			
			AddSysMessage( strChatString );
		}
		// Send message
		else 
		{
#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
			if (ubType == CHATMSG_NORMAL)
			{
				if(CeilWritingCut_CompareStr(strChatString))
				{
					strChatString = _S(4221, "1분내 같은 문장, 단어를 2회 이상 입력 할 수 없습니다.");					
					AddSysMessage(strChatString);
				}
				else
					_pNetwork->SendChatMessage(ubType, m_strWhisperTarget, strChatString);
			}
			else
				_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString );
#else
			_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString );
#endif	
		}
	}
	// 이기환 수정 끝 

	// Reset string of input box
	m_ebChatInput.ResetString();
	InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );
}

// ----------------------------------------------------------------------------
// Name : SendChatCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::SendChatCommand( char *pcChatString, int nLength )
{
	// Extract command name from string
	CTString	strCommand, strOther;

	for( int iChar = 0; iChar < nLength; iChar++ )
	{
		if( pcChatString[iChar] == ' ' )
			break;
	}

	strOther = &pcChatString[iChar + 1];
	pcChatString[iChar] = NULL;
	strCommand = &pcChatString[0];

	// Send message
	// Exchange
	if( !strcmp( strCommand, _S( 127, "교환" ) ) )
	{
		_pUIMgr->GetExchange()->SendExchangeReq_Req( strOther );
	}
	// Party
	else if( !strcmp( strCommand, _S( 128, "파티" ) ) )
	{
		ProcessParty( &pcChatString[iChar + 1] );
	}
	// Social action
	/*else if( !strcmp( strCommand, "앉기" ) )
	{
		//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( nIndex );
	}
	else if( !strcmp( strCommand, "서기" ) )
	{
		//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( nIndex );
	}
	else if( !strcmp( strCommand, "인사" ) )
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( 10 );
	}
	else if( !strcmp( strCommand, "웃음" ) )
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( 13 );
	}
	else if( !strcmp( strCommand, "울음" ) )
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( 14 );
	}*/
	// Guild
	// Get gm level
	else if( !strcmp( strCommand, "whoami" ) )
	{
		_pNetwork->SendWhoAmI();
	}
 //0118 피케이 일시 막음.
	// PvP
	else if( !strcmp( strCommand, "pvp" ) )
	{
		//_pNetwork->SetPvpMode();
// KDM : BEGIN
		int nPvPLv = 15;

		if( g_iCountry == JAPAN ) { nPvPLv = 30; }

		if( _pNetwork->MyCharacterInfo.sbAttributePos == ATTC_WAR ||	// 공성 지역에 있다면?
				_pNetwork->MyCharacterInfo.level > nPvPLv )
// KDM : END
		{
		
			if(!((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsLegit())
			//if(!penPlayerEntity->IsLegit())
			{
				_pNetwork->SendActionMessage(0, 23, 0);//ACTION_NUM_PK
			}
		}
		else 
		{
			AddSysMessage( _S( 806, "15레벨 이하는 PVP를 할 수가 없습니다." ), SYSMSG_ERROR );		
		}

	}

	// Other GM commands
	else if( _pNetwork->m_ubGMLevel > 1 )
	{
		// Show name type
		if( !strcmp( strCommand, "nametype" ) )
		{
			extern INDEX	_iNameType;
			if( !strcmp( strOther, "0" ) )			// Index
				_iNameType = 0;
			else if( !strcmp( strOther, "1" ) )		// Name
				_iNameType = 1;
		}	
		else
		{
#ifdef GM_INVISIBLE_MODE
			// WSS_VISIBLE 070510
			// 운영자 모드에서 visible 실행시 캐릭터를 안 보인게 한다.
			if( !strcmp( strCommand, "visible" ) && strOther.Length()>0 )
			{
				if( _pNetwork->MyCharacterInfo.m_ModelColor == NULL)
				{				
					// 자신의 캐릭을 안 보이게 한다.
					COLOR tColor;
					int tLevel;
					_pNetwork->MyCharacterInfo.m_ModelColor = ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->GetModelColor();
					tColor = _pNetwork->MyCharacterInfo.m_ModelColor&0xFFFFFF00;
					tLevel = atoi(strOther.str_String);
					switch(tLevel)
					{

					case 0:
						tLevel=0xFF;
						break;
					case 1:
						tLevel=0x4F;
						break;
					case 2:
						tLevel=0x2F;
						break;
					case 3:
						tLevel=0x0F;
						break;
					case 4:
						tLevel=0x00;
						break;
											
					default:
						tLevel=0xFF;
						break;
					}
					tColor |= tLevel;
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->mi_colModelColor = tColor;
				}
				else 
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->mi_colModelColor = _pNetwork->MyCharacterInfo.m_ModelColor;
					_pNetwork->MyCharacterInfo.m_ModelColor = NULL;					
				}

				_pNetwork->SendGMCommand( strCommand.str_String );
				return;
			}
			else if( iChar < nLength )
#else 
			// Echo, go_zone and goto etc...
			if( iChar < nLength )
#endif
				pcChatString[iChar] = ' ';
			_pNetwork->SendGMCommand( pcChatString );
		}
	}

	// Reset string of input box
	m_ebChatInput.ResetString();
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : AddChatMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddChatMessage( UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName,
									CTString &strChatMessage )
{
	CTString	strChatString;
	COLOR		colChatString = m_colChatTypeColor[ubChatType];
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
	COLOR		colSendName = m_sChatInfo.colSendNameType[ubChatType];
	m_sChatInfo.colName = colSendName;
#endif
	
//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
#ifdef CHARATER_CHAT_FILTER
		if (_UICharacterChatFilter.Filtering(strSendName.str_String) == TRUE)
		{
			return;
		}
#endif // CHARATER_CHAT_FILTER	

	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strChatMessage.str_String );
	// Filtering
	if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE && ubChatType != CHATMSG_NOTICE && g_iCountry != KOREA 
		&& ubChatType != CHATMSG_PRESSCORPS/*기자단 확성기 메세지는 필터 적용 안함(욕하라고해~!)*/) {
		strChatString.Clear();
		AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
		return;
	}
	// WSS_GMTOOL 070517 -------------------->><<
	if( ubChatType == CHATMSG_WHISPER || ubChatType == CHATMSG_GMTOOL)
	{
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
		m_sChatInfo.strName = strSendName + CTString(" -> ");		
		strChatString = strSendName + CTString( " -> " ) + strChatMessage;
#else
		strChatString = strSendName + CTString( "-> " ) + strChatMessage;
#endif
		if( slSendIndex == _pNetwork->MyCharacterInfo.index )
			AddWhisperTarget( m_strWhisperTarget );
		else
			AddWhisperTarget( strSendName );
	}
	else 
	{
#ifdef IME_CHAR_COLOR_CHANGE_NA_20080910
		m_sChatInfo.strName = strSendName + CTString(" > ");
		strChatString = strSendName + CTString( " > " ) + strChatMessage;
#else
		strChatString = strSendName + CTString( "> " ) + strChatMessage;
#endif
	}

	// Add chatting string to chatting list
	int	ndLineCount;
	if( ubChatType == CHATMSG_NOTICE || ubChatType == CHATMSG_LORD || ubChatType == CHATMSG_NOTICE_SERVERDOWN || ubChatType == CHATMSG_PRESSCORPS)
	{
		if ( ubChatType == CHATMSG_LORD )
		{
			_UIAutoHelp->SetGMNotice ( strChatMessage, colChatString );
		}
		else if( ubChatType == CHATMSG_NOTICE_SERVERDOWN )
		{
			int tv_i = atoi(strChatMessage.str_String);
			CTString tv_str;
			tv_str.PrintF(_S(2573,"서버 종료까지 %d초 남았습니다."),tv_i);
			_UIAutoHelp->SetGMNotice ( tv_str );
			return;
		}
		else if( ubChatType == CHATMSG_PRESSCORPS )
		{
			colChatString = 0x00C80FFF;
			_UIAutoHelp->SetGMNotice ( strChatMessage, colChatString );
		}
		else 
		{
			_UIAutoHelp->SetGMNotice ( strChatMessage );
		}
		//AddSysMessage ( strChatString ); 
		
		for( int i = 0; i < CHATTAB_TOTAL; i++ )
			{
				if( m_bChatOption[i][ubChatType] )
				{
					int	nCurLineCount = m_nCurChatCount[i];

					//AddChatString( strChatString, colChatString, i, TRUE );
					AddChatString( strChatString, colChatString, i, FALSE );//임시 MODIFY_CHATWORD_NOTICE_NA_2009_0219

					if( m_nCurChatCount[i] > m_nCurChatShowLine )
					{
						ndLineCount = m_nCurChatCount[i] - nCurLineCount;
						m_nFirstChatRow[i] += ndLineCount;

						if( m_nFirstChatRow[i] + m_nCurChatShowLine > MAX_CHAT_LINE )
							m_nFirstChatRow[i] = MAX_CHAT_LINE - m_nCurChatShowLine;
					}
				}
			}
	}
	else
	{
		// 이기환 수정 시작 (11.29) : 불량단어 필터링 
		// 주석 처리 : 나중에 문제 생기면 추가
		//if( Filtering ( strChatString.str_String ) )
		{
			for( int i = 0; i < CHATTAB_TOTAL; i++ )
			{
				if( m_bChatOption[i][ubChatType] )
				{
					int	nCurLineCount = m_nCurChatCount[i];

					AddChatString( strChatString, colChatString, i, FALSE );

					if( m_nCurChatCount[i] > m_nCurChatShowLine )
					{
						ndLineCount = m_nCurChatCount[i] - nCurLineCount;
						m_nFirstChatRow[i] += ndLineCount;

						if( m_nFirstChatRow[i] + m_nCurChatShowLine > MAX_CHAT_LINE )
							m_nFirstChatRow[i] = MAX_CHAT_LINE - m_nCurChatShowLine;
					}
				}
			}
		}
	}

	// Reset scroll bar
	m_sbChatScrollBar.SetCurItemCount( m_nCurChatCount[m_nCurSelTab] );
	m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );

	if( slSendIndex == _pNetwork->MyCharacterInfo.index )
	{
		_pNetwork->MyCharacterInfo.ChatMsg.SetChatMsg( strChatMessage );
	}
	else
	{
		INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
		for( INDEX ipl = 0; ipl < ctCha; ipl++ ) 
		{
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
			if( ct.cha_Index == slSendIndex )
			{
				FLOAT3D	vCharPos = ct.cha_pEntity->en_plPlacement.pl_PositionVector;
				FLOAT	fX = vCharPos(1) - _pNetwork->MyCharacterInfo.x;
				FLOAT	fZ = vCharPos(3) - _pNetwork->MyCharacterInfo.z;
				
				if( fX * fX + fZ * fZ <= CHATMSG_SQRDIST )
					ct.ChatMsg.SetChatMsg( strChatMessage );

				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddSysMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddSysMessage( CTString &strSysMessage, int nSysType ,COLOR colMsg )
{
#ifndef SHOW_DAMAGE_TEXT 
		// Date : 2005-11-17(오후 3:37:59), By Lee Ki-hwan
		// 공격관련 메세지는 표시하지 않는다.
		// 운영자 인 경우에는 추가
		if (( _pNetwork->m_ubGMLevel <= 0 ) && (g_iCountry != BRAZIL))
		{
			if( nSysType == SYSMSG_ATTACK || nSysType == SYSMSG_ATTACKED) return;
		}
#endif

	COLOR	colSysString = m_colSysTypeColor[nSysType];

	// wooss 070307 ----------------------->>
	// kw : WSS_WHITEDAY_2007
	if ( nSysType == SYSMSG_USER )
	{	
		colSysString = colMsg;
	}
	// ------------------------------------<<

	// Add system message string to chatting list
	int	ndLineCount;
	int	nCurLineCount = m_nCurSysCount;
	AddSysString( strSysMessage, colSysString );
	if( m_nCurSysCount > SYSTEM_SHOW_LINE )
	{
		ndLineCount = m_nCurSysCount - nCurLineCount;
		m_nFirstSysRow += ndLineCount;

		if( m_nFirstSysRow + SYSTEM_SHOW_LINE > MAX_SYSTEM_LINE )
			m_nFirstSysRow = MAX_SYSTEM_LINE - SYSTEM_SHOW_LINE;
	}

	// Reset scroll bar
	m_sbSysScrollBar.SetCurItemCount( m_nCurSysCount );
	m_sbSysScrollBar.SetScrollPos( m_nFirstSysRow );
}

void CUIChatting::PetTransform(char *pet_changeString)
{
	int nLength = strlen(pet_changeString);
	CTString strNpcIndex = CTString(""), strPetSize = CTString("");
// 변신 몬스터의 인덱스와 사이즈를 가져 온다.
	int iChar;
	for (iChar=0; iChar<nLength; iChar++)
	{
		if (pet_changeString[iChar] == ' ')
			break;
	}

	strPetSize = &pet_changeString[iChar+1];
	pet_changeString[iChar] = '\0';
	strNpcIndex = &pet_changeString[0];

	BOOL bIsInteger0 = FALSE, bIsInteger1 = FALSE;

	const char *NpcIndexChar = strNpcIndex;
	const char *PetSizeChar = strPetSize;
	int nNpcIndex =0, nPetSize = 0;

	if (strNpcIndex.IsInteger())
	{
		nNpcIndex = atoi(NpcIndexChar);
	}
	else
	{
		AddSysMessage ( CTString("Npc Index를 다시 입력하세요!") );
		return;
	}

	if (strPetSize.IsInteger())
		nPetSize = atoi(PetSizeChar);

	if (_pNetwork->_PetTargetInfo.pen_pEntity != NULL)
	{
		_pNetwork->_PetTargetInfo.TransformIndex = nNpcIndex;
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(_pNetwork->_PetTargetInfo.pen_pEntity, nNpcIndex, nPetSize);
	}
}

void CUIChatting::WearingArmortest(char *item_string)
{
	CTString strItemIndex = CTString("");
	strItemIndex = item_string;

	int nItemIndex = 0;
	
	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->WearingArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

void CUIChatting::TakeOffArmortest(char *item_string)
{
	CTString strItemIndex = CTString("");
	strItemIndex = item_string;

	int nItemIndex = 0;
	
	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->TakeOffArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
BOOL CUIChatting::CeilWritingCut_CompareStr(const CTString& strChat)
{
	if (m_sCeilWriting.empty())
	{
		AddListCeilWriting(strChat);
		SetIsCeilWritingCheck(TRUE);
		return FALSE;
	}
	else
		EraseListCeilWriting();

	std::list<sCeilWriting>::iterator	iter = m_sCeilWriting.begin(), iterEnd = m_sCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter == NULL)			continue;
		if (strcmp(strChat, iter->strCeilWriting) == 0)
		{
			iter->iCnt++;
			if (iter->iCnt >= m_iCeilWritingMaxCnt)
				return TRUE;
			else
				return FALSE;
		}
	}
	AddListCeilWriting(strChat);
	return FALSE;
}

void CUIChatting::AddListCeilWriting(const CTString& str)
{
	sCeilWriting	sTemp;
	sTemp.iCnt = 1; 
	sTemp.llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	sTemp.strCeilWriting = str;
	m_sCeilWriting.push_back(sTemp);	
}

void CUIChatting::EraseListCeilWriting()
{
	__int64		llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	std::list<sCeilWriting>::iterator	iter = m_sCeilWriting.begin(), iterEnd = m_sCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter == NULL)			continue;
		if ( llCurTime - iter->llStartTime > 60000 )//1분
			iter = m_sCeilWriting.erase(iter);
	}
}
#endif//ADD_CHAT_CEILWRITING_CUT_NA_20081029
