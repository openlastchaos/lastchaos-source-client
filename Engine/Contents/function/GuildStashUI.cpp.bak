#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Contents/function/GuildStashUI.h>
#include <Engine/GameState.h>
#include <Common/Packet/ptype_old_do_guild.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Contents/function/GuildStashUseRecord.h>

#define SEL_VIEW	0
#define SEL_TAKE	1

#define WAREHOUSE_TRADEPRICE_POSX 177

//2013/04/08 jeil 나스 아이탬 제거
#define  GUILDSTASH_ITEM_NAS 19;
#define  GUILDSTASH_ITEM_NAS_ROW -1;
#define  GUILDSTASH_ITEM_NAS_COL -1;

class CmdGuildStashAddItem : public Command
{
public:
	CmdGuildStashAddItem() : m_pWnd(NULL)	{}
	void setData(CGuildStashUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CGuildStashUI* m_pWnd;
};

enum __tagNewGuildError
{
	GSERR_TAKE_FULL,
	GSERR_TAKE_ALREADY,
	GSERR_KEEP_FAIL,
	GSERR_KEEP_FAIL_EMPTY,
	GSERR_TAKE_FAIL,
	GSERR_TAKE_FAIL_EMPTY,
	GSERR_ITEM_COUNT,
};


CGuildStashUI::CGuildStashUI() :
	m_pItemsDummy(NULL)
	, m_pArrKeep(NULL)
	, m_pArrTake(NULL)
	, m_pBtnNas(NULL)
	, m_pBtnProcess(NULL)
	, m_pTxtInOutNas(NULL)
	, m_pTxtInOutNasCnt(NULL)
	, m_pImgKeepNas_bg(NULL)
	, m_pTxtKeepNasCnt(NULL)
	, m_pTxtTitle(NULL)
{
	setInherit(false);

	m_strTitle = "Stash";
	GameState = GSS_NPC;
}

CGuildStashUI::~CGuildStashUI()
{
	SAFE_DELETE(m_pItemsDummy);
}

void CGuildStashUI::initialize()
{
#ifndef		WORLD_EDITOR
	int i;
	CUIBase* pBase = NULL;

	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pArrKeep = (CUIArray*)findUI("arr_keep");

	if (m_pArrTake = (CUIArray*)findUI("arr_trade"))
	{
		for (i = 0; i < GSITEM_COL; ++i)
		{
			pBase = m_pArrTake->GetArrayItem(i);
			if (pBase == NULL)
				continue;
			
			m_pIconsTakeItem[i] = (CUIIcon*)pBase->findUI("icon_item");
		}
	}

	// 입출금 버튼.
	m_pBtnNas = (CUIButton*)findUI("btn_in_out_nas");
	// 보관, 찾기 버튼.
	m_pBtnProcess = (CUIButton*)findUI("btn_in_out_item");
	// '입출금' 텍스트 
	m_pTxtInOutNas = (CUIText*)findUI("txt_in_out_nas");
	// 입출금 나스량
	m_pTxtInOutNasCnt = (CUIText*)findUI("txt_in_out_nas_cnt");
	// '보관 금액' 텍스트
	//m_pTxtKeepNas = (CUIText*)findUI("txt_keeping_nas");
	// 보관 나스량
	m_pTxtKeepNasCnt = (CUIText*)findUI("txt_keeping_nas_cnt");
	m_pImgKeepNas_bg = (CUIImageSplit*)findUI("img_keeping_bg");
	m_pTxtTitle = (CUIText*)findUI("txt_title");

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CGuildStashUI::CloseStash, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CGuildStashUI::CloseStash, this));

#endif // WORLD_EDITOR
}

void CGuildStashUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//------------------------------------------------------------------------------
// CGuildStashUI::AdjustPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CGuildStashUI::OpenGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::OpenGuildStash()
{	
	if( GetHide() == TRUE || IsVisible() || GameState != GSS_NPC )
		CloseStash();

	CUIManager* pUIManager = CUIManager::getSingleton();
#ifdef ENABLE_GUILD_STASH
	CTString	strGuildName;

	pUIManager->CloseMessageBoxL( MSGLCMD_NEWGUILDSTASH_REQ );

	pUIManager->CreateMessageBoxL( _S( 5534, "재정 관리인" ) , UI_GUILDSTASH, MSGLCMD_NEWGUILDSTASH_REQ );	 

	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, _S( 5535, "길드 재무관리인" ), -1, 0xE18600FF );	 
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, CTString( " " ), -1, 0xE18600FF );		

	strGuildName.PrintF( _S( 5536, "어서 오십시오.\n저는 [%s]길드의 소중한 재산을 관리하고 지키는 재무 관리인입니다.\n어떤 용무가 있으신가요?" ), _pNetwork->MyCharacterInfo.strGuildName );
	m_strTitle = _pNetwork->MyCharacterInfo.strGuildName;
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, strGuildName, -1, 0xA3A1A3FF );		

	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5537, "길드 세금 창고" ), GSMODE_DUTY );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5538, "길드 창고에 물품 맡기기" ), GSMODE_KEEP );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5539, "길드 창고에서 물품 꺼내기" ), GSMODE_TAKE );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5540, "길드 창고 이용 기록 확인" ), GSMODE_LOG );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 1220, "취소한다." ) );
#else
	_openDutyView();
#endif
}

//------------------------------------------------------------------------------
// CGuildStashUI::OpenTake
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::OpenTake( LONGLONG nBalance )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	pUIManager->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );
	
	m_llBalance = nBalance;
	
	if( m_llBalance > 0 )
		GameState = GSS_TAKE;
	
	m_strTitle = _S( 1911 , "세금 잔액 확인 및 출금" ); 
	m_btnOk.SetEnable( FALSE );
	
	// Button pos reset
	CTString strSysMessage, strNas;
	CUIMsgBox_Info	MsgBoxInfo;
	
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_USER_12 | UMBS_INPUT_MASK, UI_GUILDSTASH, MSGCMD_GUILDSTASH_TAKE_VIEW, MSGBOX_WIDTH + 20 ); 
	MsgBoxInfo.SetUserBtnName( _S( 1913, "출금" ),_S( 139 ,  "취소" ) ); 
	
	strNas.PrintF( "%I64d", m_llBalance );
	pUIManager->InsertCommaToString( strNas );
	
	strSysMessage.PrintF( _S( 1914, "잔액 : %s Nas" ) , strNas); 
	MsgBoxInfo.AddString( strSysMessage, pUIManager->GetNasColor( strNas ), TEXT_CENTER );
	
	strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	strSysMessage.PrintF( _S( 1915, "출금할 금액을 입력하여 주십시오." ) ); 
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	strSysMessage.PrintF( " " );  
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	MsgBoxInfo.SetInputBox( 4, 2, 34, 187 );	
	
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CIGuildStash::MsgBoxCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_WAREHOUSE_ADD_ITEM:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount <= 0 )
				{
					SAFE_DELETE(m_pItemsDummy);
					_showErrorMsgBox( 6 );
				}
				else
				{
					if (llCount > m_pItemsDummy->Item_Sum)
						llCount = m_pItemsDummy->Item_Sum;

					m_pItemsDummy->Item_Sum = llCount;
					_copyItemToBasket();
				}
			}
		}
		break;
	case MSGCMD_WAREHOUSE_ADD_PLUSITEM:
		{
			_copyItemToBasket();
		}
		break;
		// 게임 종료.
	case MSGCMD_NEWGUILDSTASH_ILLEGAL_USER:
		{
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;		
		}
		break;
		//2013/04/05 jeil 나스 아이템 제거
	case MSGCMD_WAREHOUSE_ADD_MONEY:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( (llCount + m_nInNas) > 0 && (llCount + m_nInNas) <= _pNetwork->MyCharacterInfo.money)
				{
					//SendWareHouseKeepReqNas( llCount);
					m_nInNas		+= llCount;
					m_strInNas.PrintF( "%I64d", m_nInNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strInNas );
					m_pTxtInOutNasCnt->SetText(m_strInNas);
					m_pTxtInOutNasCnt->setFontColor(UIMGR()->GetNasColor(m_strInNas));

				}
				else
				{
					CTString strInNasError;
					strInNasError.PrintF( _S( 5911, "현재 가지고 있는 나스 보다 많은 나스를 입금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strInNasError );
				}
			}
		}
		break;
	case MSGCMD_WAREHOUSE_TAKE_MONEY:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( (llCount + m_nOutNas) > 0 && (llCount + m_nOutNas) <= m_nTotalNas )
				{
					//SendWareHouseTakeReqNas( llCount);
					m_nOutNas		+= llCount;
					m_strOutNas.PrintF( "%I64d", m_nOutNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strOutNas );
					m_pTxtInOutNasCnt->SetText(m_strOutNas);
					m_pTxtInOutNasCnt->setFontColor(UIMGR()->GetNasColor(m_strOutNas));
				}
				else
				{
					CTString strOutNasError;
					strOutNasError.PrintF( _S( 5912, "현재 입금 되어 있는 나스 보다 많은 나스를 출금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strOutNasError );
				}
			}
		}
		break;

	case MSGCMD_GUILDSTASH_TAKE_VIEW:
		{
			if( !bOK ) 
			{
				OpenGuildStash();
				return;
			}
	
			if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS 
				&& _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_VICE_BOSS )
			{
				// 출금 요청 
				Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK );	
				return;
			}
	
			LONGLONG llReqNas = _atoi64( strInput.str_String );
			_pNetwork->SendGuildStashTakeReq( llReqNas );
	
		}
		break;
	
	case MSGCMD_GUILDSTASH_TAKE_STASH:
			if( !bOK ) 
				return;
	case MSGCMD_GUILDSTASH_INFO:
			if( !bOK ) 
				return;
			OpenGuildStash();
			return;
	case MSGCMD_GUILDSTASH_ERROR:
		{	
			switch( GameState )
			{
			case GSS_NPC:
				OpenGuildStash();
				break;
			case GSS_TAKE:
				OpenTake( m_llBalance );
				break;
			}
		}
		break;
	}

	GuildStashDoesMessageBoxExist();
}

//------------------------------------------------------------------------------
// CGuildStashUI::AddTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney )
{
	STakeInfo TakeInfo;
	TakeInfo.SetData( nMonth, nDay, llMoney );
	
	m_vTakeInfo.push_back( TakeInfo );
}

//------------------------------------------------------------------------------
// CGuildStashUI::ResetTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::ResetTakeInfo()
{
	m_vTakeInfo.clear();
}


//------------------------------------------------------------------------------
// CUIMessenger::Message
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::Message( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_GUILDSTASH, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CGuildStashUI::ErrorProc
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::ErrorProc( LONG lErrorCode, UBYTE ubType )
{
	switch( lErrorCode )
	{
	case MSG_GUILD_STASH_ERROR_OK:				// 성공
		break;
	case MSG_GUILD_STASH_ERROR_NOHISTORY:		// 히스토리 없음
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1918, "세금에 대한 입금 내역이 없습니다." ), UMBS_OK ); 
		break;
	case MSG_GUILD_STASH_ERROR_NOTBOSS:			// 길드장/부관 아님
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK ); 
		break;
	case MSG_GUILD_STASH_ERROR_NOTENOUGH:		// 잔액 부족
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1920, "잔액이 부족합니다.  확인하시고 다시 입력하여 주십시오." ), UMBS_OK ); 
		break;
	case MSG_GUILD_STASH_ERROR_FAIL_DB:			// 시스템 오류
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 37, "시스템 오류" ), UMBS_OK ); 
			CloseStash();
		break;
		// 이전 메시지는 헬퍼의 길드 창고 오류와 동일해야함
	case MSG_GUILD_STASH_ERROR_NOSPACE:			// 인벤토리 공간 부족으로 지급 불가
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1921, "인벤토리가 부족합니다." ), UMBS_OK ); 
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ResetGuildStash()
// Desc : 
// ----------------------------------------------------------------------------
void CGuildStashUI::CloseStash()
{
	GameState = GSS_NPC;
	_closeStashView();
}

//------------------------------------------------------------------------------
// CGuildStashUI::MsgBoxLCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CGuildStashUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_NEWGUILDSTASH_REQ:
		{
			if( nResult == GSMODE_DUTY )			// 세금
			{
				_openDutyView();
			}
			else if( nResult == GSMODE_KEEP )		// 맡기기
			{	
				if (pUIManager->GetInventory()->IsLocked() == TRUE ||
					pUIManager->GetInventory()->IsLockedArrange() == TRUE)
				{
					// 이미 Lock 인 창이 있을 경우 열지 못한다.
					pUIManager->GetInventory()->ShowLockErrorMessage();
					return;
				}

				m_bIsTake = FALSE;	//2013/04/02 jeil 나스 아이템 제거 
				_openStashView( GSMODE_KEEP );					
				SendGuildStashListReq();

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if ( nResult == GSMODE_TAKE )		// 찾기.
			{
				if (pUIManager->GetInventory()->IsLocked() == TRUE ||
					pUIManager->GetInventory()->IsLockedArrange() == TRUE)
				{
					// 이미 Lock 인 창이 있을 경우 열지 못한다.
					pUIManager->GetInventory()->ShowLockErrorMessage();
					return;
				}

				m_bIsTake = TRUE;	//2013/04/02 jeil 나스 아이템 제거 
				_openStashView( GSMODE_TAKE );
				SendGuildStashListReq();

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == GSMODE_LOG )		// log
			{
				UIMGR()->GetGSUseRecord()->OpenUI(m_strTitle);
				_pNetwork->SendGuildStashUseRecord();
			}
		}
		break;
	case MSGLCMD_GUILDSTASH_REQ:
		{
			if( nResult == SEL_VIEW )
			{
				// 세금 내용 요청 
				_pNetwork->SendGuildStashHistroyReq();
				// 서버에서 요청을 제대로 받았으면 ui출력 
			}
			else if ( nResult == SEL_TAKE )
			{
				// 잔액 요청 
				_pNetwork->SendGuildStashViewReq();
				// 잔액 요청시 재대로 받았으면 출력 
			}
			else 
			{
				CloseStash();
			}
		}
		break;
	}
}

void CGuildStashUI::SendGuildStashListReq()
{
	CNetworkMessage nmStashList( (UBYTE)MSG_GUILD );
	nmStashList << (UBYTE)MSG_NEW_GUILD_STASH_LIST;
	_pNetwork->SendToServerNew(nmStashList);
}

void CGuildStashUI::SendGuildStashKeepReq()
{
	// 강제 종료.
	if( _isVaildateData() == false )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		if( pUIManager->DoesMessageBoxExist( MSGCMD_NEWGUILDSTASH_ILLEGAL_USER ) )
			return;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5722, "에러 메시지"), UMBS_OK|UMBS_USE_TIMER, UI_GUILDSTASH, MSGCMD_NEWGUILDSTASH_ILLEGAL_USER);

		MsgBoxInfo.AddString( _S(5731, "길드 창고 데이터가 손상되었습니다. 종료합니다.") );

		MsgBoxInfo.SetMsgBoxTimer( 10, TRUE );			// 10 seconds.
		pUIManager->CreateMessageBox(MsgBoxInfo);
		return;
	}

	int		count = 0, i;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;
		++count;
	}

	if( !count )
	{
		if(m_nInNas <= 0)	//2013/04/05 jeil 나스 아이템 제거 
		{
			_showErrorMsgBox(GSERR_KEEP_FAIL_EMPTY);

			return;
		}
	}

	CNetworkMessage nmMessage;
	RequestClient::doNewGuildStashKeep* packet = reinterpret_cast<RequestClient::doNewGuildStashKeep*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_NEW_GUILD_STASH_KEEP;

	int		iStart = 0;

	packet->keepMoney = m_nInNas;
	packet->listCount = count;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;

		CItems* pItems = m_pIconsTakeItem[i]->getItems();

		packet->list[iStart].tab = pItems->Item_Tab;
		packet->list[iStart].invenIndex = pItems->InvenIndex;
		packet->list[iStart].itemDBIndex = pItems->Item_Index;
		packet->list[iStart].count = pItems->Item_Sum;

		++iStart;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * count));

	_pNetwork->SendToServerNew( nmMessage );

}

void CGuildStashUI::SendGuildStashTakeReq()
{
	CNetworkMessage nmMessage;
	RequestClient::doNewGuildStashTake* packet = reinterpret_cast<RequestClient::doNewGuildStashTake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_NEW_GUILD_STASH_TAKE;

	int count = 0, i, idx;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if (m_pIconsTakeItem[i]->IsEmpty() == true)
			continue;
		++count;
	}

	if (count == 0 && m_nOutNas <= 0)
	{
		_showErrorMsgBox(GSERR_TAKE_FAIL_EMPTY);
		return;
	}

	packet->guildIndex = _pNetwork->MyCharacterInfo.lGuildIndex;
	packet->takeMoney = m_nOutNas;
	packet->listCount = count;

	idx = 0;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if (m_pIconsTakeItem[i]->IsEmpty() == true)
			continue;

		CItems* pItems = m_pIconsTakeItem[i]->getItems();

		// vector 역추적해서 위치 잡음.
		packet->list[idx].stashIndex = pItems->Item_UniIndex;
		packet->list[idx].itemCount = pItems->Item_Sum;
		++idx;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * count));

	_pNetwork->SendToServerNew(nmMessage);
}

void CGuildStashUI::_openStashView(__tagGuildStashMode _mode /* = GSMODE_TAKE  */)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_GUILDSTASH );

	m_enMode = _mode;
	// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
	_enableProcessButton(TRUE);

#ifndef		WORLD_EDITOR
	if( _mode == GSMODE_TAKE )
	{
		// ITS #9069 : 길드창고 재오픈시 보여지는 UI에 대한 초기화 코드 삽입.
		_initStashUI();
		m_pBtnProcess->SetText( _S(813, "찾기") );
		m_pBtnProcess->SetCommandFUp(boost::bind(&CGuildStashUI::SendGuildStashTakeReq, this));

		m_pBtnNas->SetText( _S(5907, "출금") );
		m_pBtnNas->SetCommandFUp(boost::bind(&CGuildStashUI::OutNas, this));

		m_pTxtInOutNas->SetText(_S(5910, "출금 금액"));
		m_pImgKeepNas_bg->Hide(FALSE);
	}
	else if( _mode == GSMODE_KEEP )
	{
		_initStashUI();
		m_pBtnProcess->SetText( _S(812, "보관") );
		m_pBtnProcess->SetCommandFUp(boost::bind(&CGuildStashUI::SendGuildStashKeepReq, this));
		
		m_pBtnNas->SetText( _S(5906, "입금") );
		m_pBtnNas->SetCommandFUp(boost::bind(&CGuildStashUI::InNas, this));

		m_pTxtInOutNas->SetText(_S(5908, "입금 금액"));
		m_pImgKeepNas_bg->Hide(TRUE);
	}
#endif		// WORLD_EDITOR
	
	CTString strTemp;
	strTemp.PrintF( _s("[%s]"), m_strTitle );
	strTemp += _S( 5542, "길드 창고" );
	m_pTxtTitle->SetText(strTemp);
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILDSTASH, TRUE );
	Hide(FALSE);
}

void CGuildStashUI::_closeStashView()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	_initStashUI();
	GuildStashDoesMessageBoxExist();
	pUIManager->RearrangeOrder( UI_GUILDSTASH, FALSE );

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_GUILDSTASH );
	Hide(TRUE);
}

void CGuildStashUI::_initStashUI()
{
	_initTakeItemBtn();
	
	m_pArrKeep->ResetArray();

	m_strLeftUsedTime = "";
	m_iStashCapacity = 0;
	m_pTxtInOutNasCnt->SetText((CTString)"0");
	m_pTxtKeepNasCnt->SetText((CTString)"0");

	//m_sbStash.SetCurItemCount( 0 );

	m_strTotalNas		= CTString( "0" );
	m_strInNas			= CTString( "0" );
	m_strOutNas			= CTString( "0" );
	m_nInNas			= 0;
	m_nOutNas			= 0;
	m_nTotalNas			= 0;
}

void CGuildStashUI::_initTakeItemBtn()
{
	for( int i = 0; i < GSITEM_COL; ++i )
		m_pIconsTakeItem[i]->clearIconData();
}

void CGuildStashUI::_showNetErrorMsgBox( int _error )
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	switch( _error )
	{
	case GSERR_NO_GUILD:
		MsgInfo.AddString( _S(5549, "길드가 없습니다.") );	
		_closeStashView();
		break;
	case GSERR_INVEN_FULL:
		MsgInfo.AddString( _S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다.") );	
		break;
	case GSERR_USED:
		MsgInfo.AddString( _S( 5550, "이미 사용중입니다.") );	
		break;
	case GSERR_DONT_KEEP_ITEM:
		MsgInfo.AddString( _S( 5551, "맡길수 없는 아이템입니다.") );	
		break;
	case GSERR_NO_PERMISSION:
		MsgInfo.AddString( _S( 973, "권한이 없습니다.") );	
		_closeStashView();
		break;
	case GSERR_NO_GUILD_SKILL:
		MsgInfo.AddString( _S( 3894, "길드 스킬이 없습니다.") );	
		_closeStashView();
		break;
	case GSERR_CREATE_FAIL:
		MsgInfo.AddString( _S( 5552, "창고가 생성되어있지 않습니다.") );	
		break;
	case GSERR_STASH_FULL :	// [2012/06/01 : Sora] ITS 9436 길드창고 공간 부족 메시지 추가
		MsgInfo.AddString( _S( 5699, "길드창고에 공간이 없습니다.") );	
		break;
	default:
		MsgInfo.AddString( _s( "unknown msg in new guild stash.") );	

	}
	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void CGuildStashUI::_showErrorMsgBox( int _error )
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	switch( _error )
	{
	case GSERR_TAKE_FULL:
		MsgInfo.AddString( _S( 265, "인벤토리 공간이 부족합니다.") );	
		break;
	case GSERR_TAKE_ALREADY:
		MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
		break;
	case GSERR_KEEP_FAIL:
		MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
		break;
	case GSERR_KEEP_FAIL_EMPTY:
		_enableProcessButton(TRUE);		//2013/04/23 jeil 나스 아이템 제거 
		MsgInfo.AddString( _S( 5554, "맡길 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );	
		break;
	case GSERR_TAKE_FAIL:
		MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
		break;
	case GSERR_TAKE_FAIL_EMPTY:
		_enableProcessButton(TRUE);		//2013/04/23 jeil 나스 아이템 제거 
		MsgInfo.AddString( _S( 5555, "찾을 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );	
		break;
	case GSERR_ITEM_COUNT:
		MsgInfo.AddString( _S( 783, "요청한 아이템 개수가 올바르지 않습니다.") );	
		break;
	default:
		MsgInfo.AddString( _s( "unknown msg in new guild stash.") );	

	}
	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void CGuildStashUI::_addItemToBasket( CItems* pItem )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	GuildStashDoesMessageBoxExist();	

	if( m_enMode == GSMODE_TAKE )
	{
		int		i;
		for (i = 0; i < GSITEM_COL; ++i)
		{
			if (m_pIconsTakeItem[i]->IsEmpty() == TRUE)
				break;

			if (pItem->Item_UniIndex == m_pIconsTakeItem[i]->getItems()->Item_UniIndex)
			{
				// Error
				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( 
					_S(6055, "선택 하신 아이템은 이미 등록되어 있는 상태로..."), SYSMSG_ERROR );
				return;
			}
		}

		m_pItemsDummy = new CItems;
		memcpy(m_pItemsDummy, pItem, sizeof(CItems));
	}
	else if( m_enMode == GSMODE_KEEP )
	{
		// 스택시스템 1차 - 동일한 아이템 중복 입출금지 한번에 한번만 가능
		int		i;
		
		for (i = 0; i < GSITEM_COL; ++i)
		{
			if (m_pIconsTakeItem[i]->IsEmpty() == TRUE)
				break;
		
			if (m_pIconsTakeItem[i]->getItems()->Item_UniIndex == pItem->Item_UniIndex)
			{
				// Error
				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( 
					_S(6055, "선택 하신 아이템은 이미 등록되어 있는 상태로..."), SYSMSG_ERROR );
				return;
			}
		}
		
		m_pItemsDummy = new CItems;
		memcpy(m_pItemsDummy, pItem, sizeof(CItems));
	}

	CItemData*	pItemData = _pNetwork->GetItemData(m_pItemsDummy->Item_Index);

	const char*	szItemName = _pNetwork->GetItemName(m_pItemsDummy->Item_Index);

	if( (pItemData->GetFlag() & ITEM_FLAG_NO_STASH ) || (m_pItemsDummy->Item_Flag & FLAG_ITEM_BELONG) ) // 맡길수 없는 아이템에 귀속아이템 추가
	{
		CUIMsgBox_Info MsgBoxError;
		MsgBoxError.SetMsgBoxInfo( _S(5542, "길드 창고"), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxError.AddString( _S(5551, "맡길수 없는 아이템입니다." ) );
		pUIManager->CreateMessageBox( MsgBoxError );
		return;
	}

	// Ask quantity
	if( ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) )
	{
		CTString	strMessage;

		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(5542, "길드 창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
				UI_GUILDSTASH, MSGCMD_WAREHOUSE_ADD_ITEM );

			strMessage.PrintF( _S( 1323, "몇 나스를 옮기시겠습니까?" ) );	

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			CUIMessageBox* pMsgBox = pUIManager->GetMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
			CTString strItemCount;
			strItemCount.PrintF( "%I64d", m_pItemsDummy->Item_Sum );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			UIMGR()->InsertCommaToString(strItemCount);
			pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdGuildStashAddItem* pCmd = new CmdGuildStashAddItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S(5542, "길드 창고" ), strMessage, 1, m_pItemsDummy->Item_Sum);
		}

	}
	else if (m_pItemsDummy != NULL && m_pItemsDummy->Item_Plus > 0 && 
		!(pUIManager->IsPet(pItemData) || pUIManager->IsWildPet(pItemData) ||
		(pItemData->GetType() == CItemData::ITEM_ETC && 
		(pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || m_pItemsDummy->Item_Index == 6941))))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5542, "길드 창고" ), UMBS_YESNO,		
			UI_GUILDSTASH, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		const char	*szItemName = _pNetwork->GetItemName( m_pItemsDummy->Item_Index );
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, m_pItemsDummy->Item_Plus );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		_copyItemToBasket();
	}
}

void CGuildStashUI::DelItemInBasket( CUIIcon* pIcon )
{
	// 해당 아이콘이 Take 인지 검사하여 삭제
	for (int i = 0; i < GSITEM_COL; ++i)
	{
		if (m_pIconsTakeItem[i] == NULL)
			continue;

		if (m_pIconsTakeItem[i] == pIcon)
		{
			m_pIconsTakeItem[i]->clearIconData();
			break;
		}
	}
}

void CGuildStashUI::_copyItemToBasket()
{
	//	CItemData* pItemData = _pNetwork->GetItemData( pItems->Item_Index );

	int		iCol;
	for( iCol = 0; iCol < GSITEM_COL; ++iCol )
	{
		if (m_pIconsTakeItem[iCol]->IsEmpty())
		{
			m_pIconsTakeItem[iCol]->setData(m_pItemsDummy, false);
			CmdDragIcon* pDrag = new CmdDragIcon();
			pDrag->setData(m_pIconsTakeItem[iCol]);
			m_pIconsTakeItem[iCol]->SetCommandDrag(pDrag);
			m_pIconsTakeItem[iCol]->SetWhichUI(UI_GUILDSTASH);
			break;
		}
	}

	if( iCol == GSITEM_COL )
	{
		CTString	strMessage;

		// Add system message
		if( m_enMode == GSMODE_KEEP )
			strMessage.PrintF( _S( 831, "한번에 최대 %d개의 아이템을 보관하실 수 있습니다." ), 5 );	
		else
			strMessage.PrintF( _S( 832, "한번에 최대 %d개의 아이템을 찾으실 수 있습니다." ), 5 );		

		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	CItems* pItem = GetStashItem(m_pIconsTakeItem[iCol]->getItems()->InvenIndex);

	if (m_enMode == GSMODE_TAKE && 
		(m_pIconsTakeItem[iCol]->getItems()->Item_Sum >= pItem->Item_Sum))
	{
		int idx = m_pIconsTakeItem[iCol]->getItems()->InvenIndex;
		m_pIconsTakeItem[iCol]->getItems()->Item_Sum = pItem->Item_Sum;
	}

	m_pItemsDummy = NULL;
}

const bool	CGuildStashUI::_isVaildateData()
{
	// 이건 끝까지 조사할 필요가 없다.
	// 왜냐하면 마지막 것을 J에서 조사를 하므로 무시해도 됨.
	for( int i = 0; i < GSITEM_COL-1; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;
		// ITS#10060 : BUG FIX : 창고 입고시 조건상 문제점 수정. [8/3/2012 rumist]
		for (int j = i+1; j < GSITEM_COL; ++j )
		{
			if( m_pIconsTakeItem[j]->IsEmpty() )
				continue;
			// item index가 동일하다면.
			//if( m_btnTakeItem[i].GetItemIndex() == m_btnTakeItem[j].GetItemIndex() )
			// ITS#10189 : 길드 창고 이용시 클라이언트가 접속 종료되는 현상
			if (m_pIconsTakeItem[i]->getItems()->Item_UniIndex == m_pIconsTakeItem[j]->getItems()->Item_UniIndex)
				return false;
			// item index가 다르지만 위치가 동일하다면.
			if( (m_pIconsTakeItem[i]->getItems()->Item_Tab == m_pIconsTakeItem[j]->getItems()->Item_Tab) &&
				(m_pIconsTakeItem[i]->getItems()->InvenIndex == m_pIconsTakeItem[j]->getItems()->InvenIndex) )
				return false;
		}
	}

	return true;
}
//2013/04/02 jeil 나스 아이템 제거 
//나스 입금 팝업 생성 
void CGuildStashUI::InNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_MONEY))
		return;

	MsgBoxInfo.SetMsgBoxInfo( _S( 5906, "입금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_GUILDSTASH, MSGCMD_WAREHOUSE_ADD_MONEY );
	strMessage.PrintF( _S( 5904, "몇 개의 나스를 입금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;
}

//나스 출금하기 팝업 생성 
void CGuildStashUI::OutNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_TAKE_MONEY))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 5907, "출금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_GUILDSTASH, MSGCMD_WAREHOUSE_TAKE_MONEY );
	strMessage.PrintF( _S( 5905, "몇개의 나스를 출금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;
}

//길드 창고 보관중인 나스 요청 패킷 생성 및 서버에 메시지 보내기
void CGuildStashUI::SendGuildStashListReqNas()
{
	CNetworkMessage nmStashList( (UBYTE)MSG_GUILD );
	nmStashList << (UBYTE)MSG_NEW_GUILD_STASH_LIST_MONEY;
	_pNetwork->SendToServerNew(nmStashList);
}

void CGuildStashUI::GuildStashDoesMessageBoxExist()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if ( pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_ITEM) || 
		pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_PLUSITEM) )
	{
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_ITEM);
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_PLUSITEM);
	}

	if( pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_MONEY) ||
		pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_TAKE_MONEY) )
	{
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_MONEY);
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_TAKE_MONEY);
	}

	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}

void CGuildStashUI::AddItemCallback()
{
	SQUAD llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if (llCount <= 0)
		return;

	m_pItemsDummy->Item_Sum = llCount;
	_copyItemToBasket();
}

void CGuildStashUI::ReceiveGuildStashMessage(UBYTE ubType, CNetworkMessage* istr )
{
	// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
	_enableProcessButton(TRUE);
	switch( ubType )
	{
	case MSG_NEW_GUILD_STASH_LIST:
		{
			m_pArrKeep->ResetArray();

			INDEX nCount;
			SQUAD llNas;
			SBYTE optCnt, optType, skillCnt, skillLv;
			INDEX optLv, socket;
			LONG optVar, skillIdx;

			(*istr) >> m_strLeftUsedTime;
			(*istr) >> m_iStashCapacity;

			(*istr) >> llNas;
			(*istr) >> nCount;

			CUIArrayItem* pTmp = m_pArrKeep->GetArrayItemTemplate();
			CUIArrayItem* pItem = NULL;
			CUIIcon*	pIcon = NULL;

			for( int i = 0; i < nCount; ++i )
			{
				CItems*		pItems = new CItems;
				pItems->InvenIndex = i;
				(*istr) >> pItems->Item_UniIndex;
				(*istr) >> pItems->Item_Index;
				CItemData* pItemData = CItemData::getData(pItems->Item_Index);
				pItems->ItemData = pItemData;
				(*istr) >> pItems->Item_Plus;
				(*istr) >> pItems->Item_Flag;
				(*istr) >> pItems->Item_Used;
				(*istr) >> pItems->Item_Used2;
#ifdef DURABILITY
				(*istr) >> pItems->Item_durability_now;
				(*istr) >> pItems->Item_durability_max;
#endif	//	DURABILITY
				(*istr) >> pItems->Item_Sum;
				(*istr) >> optCnt;

				pItems->InitOptionData();

				// 레어 아이템 옵션 셋팅
				if( pItemData->GetFlag() & ITEM_FLAG_RARE )
				{
					SBYTE sbOptype[MAX_OPTION_INC_ORIGIN];
					INDEX nOptionLevel[MAX_OPTION_INC_ORIGIN];

					for( int options = 0; options < optCnt; ++options )
					{
						(*istr) >> sbOptype[options];
						(*istr) >> nOptionLevel[options];
					}

					if (optCnt == 0)
					{
						pItems->SetRareIndex(0);
					}
					else
					{
						pItems->SetRareIndex(nOptionLevel[0]);

						WORD wCBit =1;
						SBYTE sbOption =-1;

						for(int iBit = 0; iBit < 10; ++iBit)
						{
							if(nOptionLevel[1] & wCBit)
							{
								CItemRareOption* prItem = CItemRareOption::getData(nOptionLevel[0]);

								if (prItem == NULL)
									continue;

								if (prItem->GetIndex() < 0)
									continue;

								int OptionType = prItem->rareOption[iBit].OptionIdx;
								int OptionLevel = prItem->rareOption[iBit].OptionLevel;
								pItems->SetOptionData( ++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
							}

							wCBit <<=1;
						}
					}
				}
				else
				{
					for( int options = 0; options < optCnt; ++options )
					{
						(*istr) >> optType;
						(*istr) >> optLv;

						if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
							(*istr) >> optVar;
						else
							optVar = ORIGIN_VAR_DEFAULT;

						pItems->SetOptionData(options, optType, optLv, optVar);
					}

					if( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
					{
						(*istr) >> pItems->Item_Belong;
						(*istr) >> skillCnt;
						for (SBYTE sbSkillpos = 0; sbSkillpos < skillCnt; sbSkillpos++)
						{
							(*istr) >> skillIdx; 
							(*istr) >> skillLv;

							pItems->SetItemSkill(sbSkillpos, skillIdx, skillLv);
						}
					}
				}

				(*istr) >> pItems->Item_State_Plus;

				if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
				{
					pItems->InitSocketInfo();

					SBYTE	sbSocketCreateCount = 0;
					LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
					int	j;

					for (j = 0; j < JEWEL_MAX_COUNT; j++)
					{
						(*istr) >> socket;

						pItems->SetSocketOption(j, socket);

						if (socket >= 0)
							sbSocketCreateCount++;
					}						

					pItems->SetSocketCount(sbSocketCreateCount);
				}

				//m_vecGuildItemInfo.push_back(pItems);
				m_pArrKeep->AddArrayItem((CUIArrayItem*)pTmp->Clone());
				pItem = m_pArrKeep->GetArrayItem(i);
				pIcon = (CUIIcon*)pItem->findUI("icon_item");
				pIcon->setData(pItems);

				if (m_enMode == GSMODE_TAKE)
				{
					CmdDragIcon* pDrag = new CmdDragIcon();
					pDrag->setData(pIcon);
					pIcon->SetCommandDrag(pDrag);
				}
			}

			//m_pArrKeep->update
			m_nTotalNas = llNas;
			m_strTotalNas.PrintF( "%I64d", m_nTotalNas );
			CUIManager::getSingleton()->InsertCommaToString( m_strTotalNas );
			m_pTxtKeepNasCnt->SetText(m_strTotalNas);
			m_pTxtKeepNasCnt->setFontColor(UIMGR()->GetNasColor(m_strTotalNas));
		}

		break;
	case MSG_NEW_GUILD_STASH_KEEP:	
		{
			SBYTE errorCode;
			(*istr) >> errorCode;
			if( errorCode )
				_showNetErrorMsgBox( errorCode );
			else
			{
				_initStashUI();
				SendGuildStashListReq();
			}
		}
		break;
	case MSG_NEW_GUILD_STASH_TAKE:	
		{
			SBYTE errorCode;
			(*istr) >> errorCode;
			if( errorCode )
				_showNetErrorMsgBox( errorCode );
			else
			{
				_initStashUI();
				SendGuildStashListReq();
			}
		}
		break;
	case MSG_NEW_GUILD_STASH_LOG:
		{
			UIMGR()->GetGSUseRecord()->GSUseRecordListRecv(istr);
		}
		break;
	case MSG_NEW_GUILD_STASH_ERROR:
		{
			BYTE		bErrorCode;
			(*istr) >> bErrorCode;
			_showNetErrorMsgBox( bErrorCode );
		}
		break;
	}
}

CItems* CGuildStashUI::GetStashItem( int idx )
{
	if (m_pArrKeep == NULL)
		return NULL;

	int max = m_pArrKeep->GetArrayChildCount();
	if (idx < 0 || idx >= max)
		return NULL;

	CUIArrayItem* pItem = m_pArrKeep->GetArrayItem(idx);

	if (pItem == NULL)
		return NULL;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_item");
	
	if (pIcon == NULL)
		return NULL;

	return pIcon->getItems();
}

void CGuildStashUI::AddItemToBasket()
{
	CUIIcon* pDrag = UIMGR()->GetDragIcon();

	if (pDrag == NULL)
		return;

	// 
	if (pDrag->GetWhichUI() == UI_INVENTORY &&
		m_enMode == GSMODE_TAKE)
		return;

	CItems* pItems = pDrag->getItems();
	if (pItems == NULL)
		return;

	_addItemToBasket(pItems);
}

WMSG_RESULT CGuildStashUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (CUIWindow::OnLButtonUp(x, y) == WMSG_OB)
		return WMSG_OB;

	if (CUIIcon* pDrag = UIMGR()->GetDragIcon())
	{
		if (m_pArrTake != NULL)
		{
			if (m_pArrTake->IsInside(x, y) == TRUE)
				AddItemToBasket();
			else
				DelItemInBasket(pDrag);
		}
	}

	UIMGR()->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CGuildStashUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (CUIWindow::OnLButtonDown(x, y) == WMSG_OB)
		return WMSG_OB;

	UIMGR()->RearrangeOrder(UI_GUILDSTASH, TRUE);
	return WMSG_FAIL;
}

void CGuildStashUI::_openDutyView()
{
	CUIManager* pUIManager = UIMGR();
	pUIManager->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );

	pUIManager->CreateMessageBoxL( _S( 1905, "재정 관리인" ) , UI_GUILDSTASH, MSGLCMD_GUILDSTASH_REQ );	 

	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1906, "길드 재정관리인" ), -1, 0xE18600FF );	 
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, CTString( " " ), -1, 0xE18600FF );		

	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1907, "길드에서 거두어 들이는 세금이 있으신가요?" ), -1, 0xA3A1A3FF );		 
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1908, "저는 그 세금을 관리하는 재정관리인입니다." ), -1, 0xA3A1A3FF );		
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1909, "이곳에서 세금을 출금하고 세금의 입금 목록을 확인할 수 있습니다." ), -1, 0xA3A1A3FF );		

	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1910, "세금 수입 확인" ), SEL_VIEW );	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1911, "세금 잔액 확인 및 출금" ), SEL_TAKE );	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S(1220 , "취소한다." ) );
}
