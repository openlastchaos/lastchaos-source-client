#include "stdh.h"
#include "TeleportUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Help/Util_Help.h>

#define	DEF_TELEPORT_MAX_MEMO			5
#define	DEF_TELEPORT_MAX_ROW			5

static CTString	strTempMemo;
extern INDEX g_iCountry;

CTeleportUI::CTeleportUI()
	: m_pMoveArea(NULL)
	, m_plistPortal(NULL)
	, m_pExtendTime(NULL)
	, m_useTime(0)
{
	int i;
 	for (i = 0; i < eBTN_MAX; ++i)
 		m_pBtn[i] = NULL;

	ClearTeleportList();
	setInherit(false);
}

void CTeleportUI::ClearTeleportList()
{
	_setListIndex(-1);

	for( int i = 0; i < DEF_TELEPORT_EXTEND_MAX_MEMO; i++ )
	{
		m_strZone[i] = CTString( "" );
		m_strCoord[i] = CTString( "" );
		m_strMemo[i] = CTString( "" );
		m_nZoneNum[i] = -1;
	}
}

void CTeleportUI::ListItemDown()
{
	UIMGR()->CloseMessageBox( MSGCMD_TELEPORT_MEMO );

	int nMaxSlot = _getMaxSlot();
	int nCurSelIdx = _getListSelectIndex();

	BOOL bMoveEnable, bSaveEnable;

	if (nCurSelIdx < 0 || nCurSelIdx >= nMaxSlot)
	{
		bMoveEnable = FALSE;
		bSaveEnable = FALSE;
	}
	else
	{
		bSaveEnable = TRUE;

		if (m_nZoneNum[nCurSelIdx] > -1 && _pNetwork->MyCharacterInfo.zoneNo == m_nZoneNum[nCurSelIdx])
			bMoveEnable = TRUE;
		else
			bMoveEnable = FALSE;
	}

	_setBtnEnable(eBTN_MOVE, bMoveEnable);
	_setBtnEnable(eBTN_SAVE, bSaveEnable);
}

void CTeleportUI::UpdateListItem( int nSlotIdx )
{
	if (m_plistPortal == NULL)
		return;

	int nMaxSlot = _getMaxSlot();

	if (nSlotIdx < 0 || nSlotIdx >= nMaxSlot)
		return;

	CUIListItem* pItem = NULL;

	pItem = (CUIListItem*)m_plistPortal->GetListItem(nSlotIdx);

	if (pItem == NULL)
		return;

#ifndef		WORLD_EDITOR
	pItem->SetCommandFUp(boost::bind(&CTeleportUI::ListItemDown, this));
#endif		// WORLD_EDITOR

	CUIText* pText = NULL;
	CTString strTemp;

	// 번호
	pText = (CUIText*)pItem->findUI("str_number");

	if(pText != NULL)
	{
		strTemp.PrintF("%d", nSlotIdx + 1);
		pText->SetText(strTemp);
	}

	// 존
	pText = (CUIText*)pItem->findUI("str_zone");

	if(pText != NULL)
		pText->SetText(m_strZone[nSlotIdx]);

	// 좌표
	pText = (CUIText*)pItem->findUI("str_pos");

	if(pText != NULL)
		pText->SetText(m_strCoord[nSlotIdx]);

	// 메모
	pText = (CUIText*)pItem->findUI("str_memo");

	if(pText != NULL)
		pText->SetText(m_strMemo[nSlotIdx]);


	// 기간
	pText = (CUIText*)pItem->findUI("str_time");

	if(pText != NULL)
	{
		int nStringId;

		if (m_nZoneNum[nSlotIdx] < 0)
			strTemp = CTString("");
		else
		{
			if (nSlotIdx < DEF_TELEPORT_MAX_MEMO)
				nStringId = 2120; // 영구
			else
				nStringId = 6031; // 확장

			strTemp.PrintF( _S(nStringId,"영구 or 확장") );
		}		
				
		pText->SetText(strTemp);
	}
}

void CTeleportUI::PressMoveBtn()
{
	if(_getListSelectIndex() >= _getMaxSlot()) 
		NotUseMemEx();
	else 
		SendTeleportMove();
}

void CTeleportUI::PressSaveBtn()
{
	if(_getListSelectIndex() >= _getMaxSlot()) 
		NotUseMemEx();
	else 
		OpenMemorizeMsgBox();
}


void CTeleportUI::ShowTeleportError()
{
	CTString	strSysMessage;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_SKILL ) )
		strSysMessage = _S( 376, "스킬을 사용하고 있습니다." );
	else if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
		strSysMessage = _S( 377, "다른 장소로 이동을 준비하고 있습니다." );
	else if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) )
		strSysMessage = _S( 378, "파티 신청이 진행중입니다." );
	else if( pUIManager->IsCSFlagOn( CSF_EXCHANGE ) )
		strSysMessage = _S( 379, "교환이 진행중입니다." );
	else if( pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) ) // [sora] 원정대 신청중
		strSysMessage = _S( 4656, "원정대 신청이 진행중입니다.." );
	else
		strSysMessage = _S( 384, "이동할 수 없습니다." );

	pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
}

void CTeleportUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_TELEPORT_MEMO:
		{
			strTempMemo = strInput;
			SendTeleportWrite();
		}
		break;
	}
}

void CTeleportUI::SetTeleportInfo( UBYTE ubSlot, SLONG slZone, FLOAT fX, FLOAT fZ, CTString &strComment )
{
	m_nZoneNum[ubSlot] = slZone;
	m_strZone[ubSlot] = CZoneInfo::getSingleton()->GetZoneName( slZone );
	m_strCoord[ubSlot].PrintF( "%4d,%-4d", (int)fX, (int)fZ );
	m_strMemo[ubSlot] = strComment;
}

void CTeleportUI::NotUseMemEx()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
		return;

	CTString	strTitle,strMsg;
	strTitle = _S(191,"확인");
	strMsg	 = "메모리 스크롤 확장 카드를 사용중이 아니거나 이용시간이 경과하였습니다.";
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMsg );
	pUIManager->CreateMessageBox(MsgBoxInfo);
}

void CTeleportUI::OpenTeleport()
{
	if (GetHide() == FALSE)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	Hide(FALSE);
	SetVisible(TRUE);

	_setUI();

	pUIManager->RearrangeOrder( UI_TELEPORT, TRUE );
}

void CTeleportUI::CloseTeleport()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();

	pUIMgr->CloseMessageBox( MSGCMD_TELEPORT_MEMO );
	pUIMgr->RearrangeOrder( UI_TELEPORT, FALSE );
}

void CTeleportUI::initialize()
{
#ifndef		WORLD_EDITOR
	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");
	
	if (m_pMoveArea != NULL)
	{
		int l, t, r, b;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		r = l + m_pMoveArea->GetWidth();
		b = t + m_pMoveArea->GetHeight();
		setTitleRect(l, t, r, b);
	}

 	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_move", "btn_save", "btn_cancel" };
 
 	for (int i = 0; i < eBTN_MAX; ++ i)
 		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

 	if (m_pBtn[eBTN_CLOSE] != NULL)
 		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CTeleportUI::CloseTeleport, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CTeleportUI::CloseTeleport, this));

	if (m_pBtn[eBTN_MOVE] != NULL)
		m_pBtn[eBTN_MOVE]->SetCommandFUp(boost::bind(&CTeleportUI::PressMoveBtn, this));

	if (m_pBtn[eBTN_SAVE] != NULL)
		m_pBtn[eBTN_SAVE]->SetCommandFUp(boost::bind(&CTeleportUI::PressSaveBtn, this));

	m_plistPortal = (CUIList*)findUI("list_movepos");

	m_pExtendTime = (CUIText*)findUI("str_usetime");
#endif		// WORLD_EDITOR
}

WMSG_RESULT CTeleportUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_plistPortal && m_plistPortal->IsInside(x, y) == FALSE)
		ListItemDown();	

	CUIManager::getSingleton()->RearrangeOrder( UI_TELEPORT, TRUE );
	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CTeleportUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->ResetHoldBtn();

	return CUIWindow::OnLButtonUp(x, y);
}

void CTeleportUI::SetUseTime( int t )
{
	m_useTime = t;

	_setExtendTimeString();
	_setList();
}

void CTeleportUI::_setUI()
{
	_setBtnEnable(eBTN_MOVE, FALSE);
	_setBtnEnable(eBTN_SAVE, FALSE);

	_setList();
	_setExtendTimeString();
}

void CTeleportUI::_setBtnEnable( int nBtnIdx, BOOL bEnable )
{
	if (nBtnIdx < 0 || nBtnIdx >= eBTN_MAX)
		return;

	if (m_pBtn[nBtnIdx] != NULL)
		m_pBtn[nBtnIdx]->SetEnable(bEnable);
}

void CTeleportUI::_setList()
{
	if (m_plistPortal == NULL)
		return;

	m_plistPortal->DeleteAllListItem();

	int nMaxSlot = _getMaxSlot();

	CUIListItem* pTemp = m_plistPortal->GetListItemTemplate();

	if (pTemp == NULL)
		return;

	for (int i = 0; i < nMaxSlot; ++i)
	{
		m_plistPortal->AddListItem(pTemp->Clone());
		UpdateListItem(i);
	}		

	_setListIndex(-1);
	CUIScrollBar* pScroll = m_plistPortal->GetScroll();

	if (pScroll != NULL)
		pScroll->SetScrollCurPos(0);

	m_plistPortal->UpdateScroll(nMaxSlot);
	m_plistPortal->UpdateList();	
}

void CTeleportUI::_setListIndex( int nIndex )
{
	int nMaxSlot = _getMaxSlot();
	int nUnSelectIdx = -1;

	if (nIndex < 0 || nIndex >= nMaxSlot)
		nIndex = nUnSelectIdx;

	if (m_plistPortal != NULL)
		m_plistPortal->setCurSel(nIndex);
}

void CTeleportUI::_setExtendTimeString()
{
	if (m_pExtendTime == NULL)
		return;

	CTString strExpireTime = CTString("");

	if (m_useTime > 0)
	{
		tm* pTime = NULL;
		pTime = localtime((time_t*)&m_useTime);

		strExpireTime.PrintF(  _S( 6070,"만료 : %d년%d월%d일%d시%d분"),pTime->tm_year + 1900
			,pTime->tm_mon + 1,pTime->tm_mday,pTime->tm_hour, pTime->tm_min);
	}

	m_pExtendTime->SetText(strExpireTime);
}

int CTeleportUI::_getListSelectIndex()
{
	if (m_plistPortal == NULL)
		return -1;

	return m_plistPortal->getCurSel();
}

int CTeleportUI::_getMaxSlot()
{
	int nMaxSlot = DEF_TELEPORT_MAX_MEMO;

	if (m_useTime > 0)
		nMaxSlot = DEF_TELEPORT_EXTEND_MAX_MEMO;

	return nMaxSlot;
}


// ----------------------------------------------------------------------------
// Name : OpenMemorizeMsgBox()
// Desc :
// ----------------------------------------------------------------------------
void CTeleportUI::OpenMemorizeMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	if( _getListSelectIndex() == -1 )
		return;

	CTString	strZone, strCoord;
	strZone.PrintF( "%-8s: %s", _S( 291, "존" ),
		CZoneInfo::getSingleton()->GetZoneName( _pNetwork->MyCharacterInfo.zoneNo ) );
	strCoord.PrintF( "%-8s: %d,%d", _S( 292, "좌표" ),
		(int)_pNetwork->MyCharacterInfo.x, (int)_pNetwork->MyCharacterInfo.z );

	CUIMsgBox_Info	MsgBoxInfo;
	// [KH_070315] 프리미엄일 때 분기
	MsgBoxInfo.SetMsgBoxInfo( _S( 375, "메모리 스크롤" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
		UI_TELEPORT, MSGCMD_TELEPORT_MEMO );
	MsgBoxInfo.AddStringEx( strZone, 0, 0 );
	MsgBoxInfo.AddStringEx( strCoord, 1, 0 );
	MsgBoxInfo.AddStringEx( _S( 293, "메모" ), 2, 0 );

	if (g_iCountry == RUSSIA)
		MsgBoxInfo.SetInputBox( 3, 10, 25 ); // 2->3 으로 문장이 겹침 modified by sam 11/02/22
	else
		MsgBoxInfo.SetInputBox( 2, 10, 32 );

	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CTeleportUI::SendTeleportWrite()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIMgr->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if (UTIL_HELP()->IsSpecialChar(strTempMemo.str_String) == true)
	{
		pUIMgr->GetChattingUI()->AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
		return;
	}

	if (g_iCountry != RUSSIA)
		_pNetwork->SendTeleportWrite( MSG_MEMPOS, _getListSelectIndex(), strTempMemo );
	else
		_pNetwork->SendTeleportWriteRus( MSG_MEMPOS, _getListSelectIndex(), strTempMemo );	//wooss 050816

	strTempMemo.Clear();
}

void CTeleportUI::SendTeleportMove()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	int nSelectIndex = _getListSelectIndex();

	if( nSelectIndex == -1 || nSelectIndex >= _getMaxSlot())
		return;

	if( _pNetwork->MyCharacterInfo.zoneNo != m_nZoneNum[nSelectIndex] )
		return;

	if( pUIManager->IsCSFlagOn( CSF_CANNOT_TELEPORT_MASK ) )
		ShowTeleportError();
	else if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}
	else
	{
		_pNetwork->SendTeleportMove( MSG_MEMPOS, nSelectIndex);
	}
}
