#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIPartyAuto.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/Base/PartyAutoUIReg.h>
#include <Engine/Contents/Base/PartyAutoUIRegBoss.h>
#include <Engine/Contents/Base/PartyAutoUIInviteList.h>
#include <Engine/Contents/Base/PartyAutoUIPartyList.h>

extern INDEX		g_iCountry;
extern CDrawPort	*_pdpMain;

#define PT_JOIN_WIDTH	215
#define PT_JOIN_HEIGHT	165
#define PT_LEADER_JOIN_WIDTH	400
#define PT_LEADER_JOIN_HEIGHT	200
#define PT_PARTY_LIST_WIDTH		600
#define PT_PARTY_LIST_HEIGHT	450

// Window basic size
#define WBS_TITLE_HEIGHT	22
#define WBS_GAP_HEIGHT		3
#define WBS_BOTTOM_HEIGHT	3

#define WBS_JOIN_HEIGHT		134
#define WBS_LEADER_JOIN_HEIGHT_1	141
#define WBS_LEADER_JOIN_HEIGHT_2	30

#define WBS_PARTY_LIST_BGD_1	398
#define WBS_PARTY_LIST_BGD_2	352
#define WBS_PARTY_LIST_BGD_WIDTH	576

// check box TAB
#define CB_TAB		40

// UV Pos
#define UVP_TAB_X	96
#define UVP_TAB_Y   25	

// Rect Select Tab
#define RST_HEIGHT_TAB	26

// Menu Tab( center align )
#define NUMBER_POS		42			// 42
#define SUBJECT_STR		154			// 165
#define LIMIT_DIFF		273			// 273
#define ZONE_POS		348			// 348
#define NEED_CLASS		469			// 469
#define PARTY_TYPE		550			// 550

// Class Image size ( n*n )
#define CLASS_IMAGE_SIZE	18
#define LEVELDIFF_IMAGE_SIZE 22

// LIST EACH START HEGIHT
#define LESH_NUMBER			66
#define LESH_STRING			65
#define LESH_LEVELDIFF		59
#define LESH_IMAGE			62

// TEXT SCROLL DELAY TIME
#define TEXT_DELAY_TIME		200



// ----------------------------------------------------------------------------
// Name : CUIPartyAuto()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPartyAuto::CUIPartyAuto()
{
	m_nSelectState = -1;
	m_nCurrentPageNum = 0;
	m_FrontChPos = 0;
	m_bTextDir = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIParty()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPartyAuto::~CUIPartyAuto()
{

}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	// join set
	m_rcJoinTitle.SetRect( 0, 0, 215, 22 );
}

void CUIPartyAuto::Clear()
{
	m_nSelectState = -1;
	m_nLevelLimit = 0;
	m_nCurrentPageNum = 0;
	m_FrontChPos = 0;
	m_nSelectLine = -1;
	m_bShowTextScroll = FALSE;
	m_bTextDir = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenPartyMatching()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::OpenPartyMatching()
{
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( GAMEDATAMGR()->GetPartyInfo()->IsPartyRequested() )
		return;

	pUIManager->CloseMessageBoxL( MSGLCMD_PARTYAUTOMATCH_REQ );
	pUIManager->CreateMessageBoxL( _S( 2682, "파티 매칭 시스템" ), UI_PARTYAUTO, MSGLCMD_PARTYAUTOMATCH_REQ );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, TRUE,
				_S( 2683, "용사님들이 보다 수월하게 모험을 하실 수 있도록 파티원들을 모집해 주는 시스템입니다." ), -1, 0xA3A1A3FF );

	CTString strMessage;
	strMessage.PrintF( _S( 2684, "파티(원) 등록" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_REGIST );

	strMessage.PrintF( _S( 2685, "초대가능 파티원 리스트" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, INVITE_LIST );

	strMessage.PrintF( _S( 2686, "참가가능 파티 리스트" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_LIST );

	strMessage.PrintF( _S( 2687, "파티(원) 등록 삭제" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_DEL );
					
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage );
}

// ----------------------------------------------------------------------------
// Name : OpenAutoParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::OpenAutoParty( int nType )
{
	if( IsVisible() == TRUE )
		return;

	Clear();

	CUIManager* pUIMgr = UIMGR();

	switch (nType)
	{
	case PARTY_REGIST:
		pUIMgr->GetPartyAutoReg()->open();
		break;
	case PARTY_REGIST_LEADER:
		pUIMgr->GetPartyAutoRegBoss()->open();
		break;
	case INVITE_LIST:
		pUIMgr->GetPartyAutoInvite()->open();
		break;
	case PARTY_LIST:
		pUIMgr->GetPartyAutoParty()->open();
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustUIPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::AdjustUIPos()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	PIX pixMinI, pixMaxI, pixMinJ, pixMaxJ;

	pixMinI = pUIManager->GetMinI();
	pixMinJ = pUIManager->GetMinJ();
	pixMaxI = pUIManager->GetMaxI();
	pixMaxJ = pUIManager->GetMaxJ();

	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CloseAutoParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::CloseAutoParty()
{
	
	CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, FALSE );

	Clear();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int nX = LOWORD( pMsg->lParam );
	int nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int ndX = nX - nOldX;
				int ndY = nY - nOldY;
				nOldX = nX; nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( m_nSelectState >= PARTY_REGIST )
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;
					// Title bar
					if( IsInsideRect( nX, nY, m_rcJoinTitle ) )
					{
						bTitleBarClick = TRUE;

						return WMSG_SUCCESS;
					}					
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}			
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{

}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CTString strMessage;
	switch( nCommandCode )
	{
	case MSGLCMD_PARTYAUTOMATCH_REQ:
		{
			if( nResult == PARTY_REGIST ) // 파티 등록
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() <= 0 ) // 파티중이 아니라면
				{
					OpenAutoParty( PARTY_REGIST );
				}
				else
				{
					if( GAMEDATAMGR()->GetPartyInfo()->AmILeader() )
					{
						OpenAutoParty( PARTY_REGIST_LEADER );
					}
					else
					{
						pUIManager->CloseMessageBoxL( MSGLCMD_PARTYAUTOMATCH_REQ );
						CUIMsgBox_Info MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						CTString	strMessage = _S( 2690, "파티장만 등록할 수 있습니다" );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
				}
			}
			else if( nResult == INVITE_LIST ) // 초대가능 리스트
			{
				OpenAutoParty( INVITE_LIST );
			}
			else if( nResult == PARTY_LIST ) // 참가 가능 리스트
			{
				OpenAutoParty( PARTY_LIST );
			}
			else if( nResult == PARTY_DEL ) // 파티 등록 삭제
			{
				_pNetwork->SendPartyRegistDelete();
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MatchRegMemberRep()
// Desc : 파티 매칭 일반 등록 응답
// ----------------------------------------------------------------------------
void CUIPartyAuto::MatchRegMemberRep( int nErrorcode )
{
	CTString	strMessage;

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_OK:
		strMessage = _S( 2695, "초대가능 파티원 리스트에 캐릭이 등록 되었습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY:
		strMessage = _S( 2696, "이미 파티에 참여 중입니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG:
		strMessage = _S( 2697, "이미 파티 등록이 되어 있습니다" );
		break;
	}

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->GetPartyAutoReg()->close();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage );
	pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MatchRegPartyRep()
// Desc : 파티 매칭 일반 등록 응답
// ----------------------------------------------------------------------------
void CUIPartyAuto::MatchRegPartyRep( int nErrorcode )
{
	CTString	strMessage;

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_OK:
		strMessage = _S( 2698, "참가가능 파티 리스트에 파티가 등록 되었습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS:
		strMessage = _S( 2699, "파티장만 파티등록 신청을 할 수 있습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_FULL:
		strMessage = _S( 2700, "파티인원이 가득차 있습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG:
		strMessage = _S( 2701, "이미 리스트에 등록된 상태입니다" );
		break;
	}

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->GetPartyAutoRegBoss()->close();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage );
	pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : ListSelectLine()
// Desc :
// ----------------------------------------------------------------------------
int CUIPartyAuto::ListSelectLine( int nY )
{
	const int ndivide = 26;
	int nSelY = nY - ( m_nPosY + 58 );

	nSelY /= ndivide;

	return nSelY;
}

// ----------------------------------------------------------------------------
// Name : TextScroll()
// Desc : 문자열을 스크롤 한다.(한글자씩)
// ----------------------------------------------------------------------------
void CUIPartyAuto::TextScroll( CTString strText )
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	CTString strTemp1, strTemp2;
	CTString strSaveSplit = strText;
	char cCurrent;

	int nX, nY;	
	int nSaveHeight = m_nSelectLine * 25;
	int nStartPos = m_nPosX + 64; // 텍스트 맨 앞 위치
	int nEndPos = nStartPos +  ( 30 * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) );

	nY = m_nPosY+LESH_STRING+nSaveHeight;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - mtmTextScrollTime;

	if( llCurDelay < TEXT_DELAY_TIME )
	{
		if( m_bTextDir )
		{
			strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
			strSaveSplit = strTemp2;

			if( strSaveSplit.Length() > 30 )
			{
				BOOL b2Byte = Is2ByteChar( 30, 0, strSaveSplit );
				
				if( b2Byte )
				{ strSaveSplit.Split( 31, strTemp1, strTemp2 ); }
				else
				{ strSaveSplit.Split( 30, strTemp1, strTemp2 ); }

				strSaveSplit = strTemp1;				
			}
			
			nX = nStartPos;
			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}
		else
		{
			strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
			strSaveSplit = strTemp1;

			int nStrWidth = strSaveSplit.Length() * 
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			nX = nEndPos-nStrWidth;

			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}

		return;
	}
	// 시간 갱신
	mtmTextScrollTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	if( m_bTextDir )
	{
		if( m_FrontChPos <= strSaveSplit.Length() )
		{
			cCurrent = strSaveSplit[m_FrontChPos];

			if( cCurrent == '\n' || cCurrent == '\r' || cCurrent == ' ' )
			{
				m_FrontChPos++;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}
			else if( cCurrent & 0x80 )
			{
				m_FrontChPos += 2;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}
			else
			{
				m_FrontChPos += 1;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}

			nX = nStartPos;
			if( strSaveSplit.Length() > 30 )
			{
				BOOL b2Byte = Is2ByteChar( 30, 0, strSaveSplit );
				
				if( b2Byte )
				{ strSaveSplit.Split( 31, strTemp1, strTemp2 ); }
				else
				{ strSaveSplit.Split( 30, strTemp1, strTemp2 ); }
					
				strSaveSplit = strTemp1;				
			}

			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}
		else
		{
			m_bTextDir = FALSE;
			m_FrontChPos = 0;
		}
	}
	else
	{
		if( m_FrontChPos < 30 )
		{
			cCurrent = strSaveSplit[m_FrontChPos];

			if( cCurrent == '\n' || cCurrent == '\r' || cCurrent == ' ' )
			{
				m_FrontChPos++;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			else if( cCurrent & 0x80 )
			{
				m_FrontChPos += 2;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			else
			{
				m_FrontChPos += 1;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			int nStrWidth = strSaveSplit.Length() * 
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			nX = nEndPos-nStrWidth;
			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );

			if( m_FrontChPos >= 30 )
			{
				m_bTextDir = TRUE;
				m_FrontChPos = 0;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Is2ByteChar()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIPartyAuto::Is2ByteChar( int nCharIndex, int nFirstCheck, CTString strText )
{
	BOOL	b2ByteChar = FALSE;
	const char	*pcTemp = &strText[nFirstCheck];

	for( int iPos = nFirstCheck; iPos < nCharIndex; iPos++, pcTemp++ )
	{
#if defined (G_THAI)
		b2ByteChar = FALSE;
#else
		if( (*pcTemp & 0x80) )
		{
			if( _pUIFontTexMgr->GetLanguage() == FONT_JAPANESE && ((UCHAR)*pcTemp >= 0xa1 && (UCHAR)*pcTemp <= 0xdf) ) 
				b2ByteChar = FALSE;
			else b2ByteChar = !b2ByteChar;
		}
		else
			b2ByteChar = FALSE;
#endif
	}

	return b2ByteChar;
}