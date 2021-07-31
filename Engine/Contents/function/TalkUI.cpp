#include "StdH.h"
#include "TalkUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Help/Util_Help.h>

extern INDEX g_iCountry;
extern CFontData *_pfdDefaultFont;
static int _iMaxMsgStringChar = 0;

#define TALK_SYSTEM_COLOR		0xE18600FF
#define TALK_DESC_CHAR_WIDTH	260
#define DEF_DEFUALT_ITEM_HEIGHT 15
#define DEF_WIDTH_GAP			10

CTalkUI::CTalkUI(void)
	: m_pMoveArea(NULL)
	, m_pImgChatColor(NULL)
	, m_pTextMemberInfo(NULL)
	, m_pBtnClose(NULL)
	, m_pArrChatColor(NULL)
	, m_pEditInput(NULL)
	, m_nWhichUI(-1)
	, m_pChatDesign(NULL)
	, m_nColIndex(0)
{
	int i;
	for (i = 0; i < eCHK_MAX; ++i)
		m_pCheckBtn[i] = NULL;

	for (i = 0; i < eIMGSPLIT_MAX; ++i)
		m_pImgSplit[i] = NULL;

	for (i = 0; i < eLIST_MAX; ++i)
		m_pList[i] = NULL;
}

CTalkUI::~CTalkUI()
{
	Destroy();
	SAFE_DELETE(m_pChatDesign);
}

void CTalkUI::AddTalkListString( CTString strName, CTString strMessage, bool bFocus /*= true*/, COLOR colDesc /*= 0xF2F2F2FF */ )
{
	if( bFocus ) SetFocus( TRUE );

	CTString strTalk = strMessage;

	if( strName != "" )
		strTalk = strName + "> " + strMessage;

	m_vecTalkList.push_back( strTalk );

	AddTalkListString( strTalk, colDesc );
}

void CTalkUI::AddTalkListString( CTString &strDesc, COLOR colDesc /*= 0xF2F2F2FF */ )
{
	if (m_pList[eLIST_CHAT] == NULL || m_pChatDesign == NULL)
		return;

	int		nOldItemCount = m_pList[eLIST_CHAT]->getListItemCount();

	m_pChatDesign->SetSplitMode(SPLIT_SPACE);
	m_pChatDesign->AddText(std::string(strDesc), colDesc);

	int nCount = m_pChatDesign->GetTextExCount();
	int nListItemCount = m_pList[eLIST_CHAT]->getListItemCount() + nCount;

	CUIListItem* pItemTmp = m_pList[eLIST_CHAT]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	for (int i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)pItemTmp->Clone();
		m_pList[eLIST_CHAT]->AddListItem(pItem);

		if (CUITextBoxEx* pText = (CUITextBoxEx*)pItem->findUI("tbex_contents"))
		{
			CUIBase* pAddItem = m_pChatDesign->GetLineText(i);
			pText->SetSplitMode(SPLIT_ONELINE);
			pText->AddUI(pAddItem);

			pAddItem->SetPosY(0);
		}
	}

	if (m_pList[eLIST_CHAT]->GetScroll() != NULL)
	{
		int nCurScrollPos = m_pList[eLIST_CHAT]->GetScroll()->GetScrollPos();
		int nShowCount = m_pList[eLIST_CHAT]->GetItemShowNum();

		if (nOldItemCount - nShowCount == nCurScrollPos ||
			(nCurScrollPos == 0 && (nOldItemCount - nShowCount < 0)))
			nCurScrollPos = m_pList[eLIST_CHAT]->getListItemCount() - m_pList[eLIST_CHAT]->GetItemShowNum();

		m_pList[eLIST_CHAT]->UpdateScroll(m_pList[eLIST_CHAT]->getListItemCount());
		m_pList[eLIST_CHAT]->GetScroll()->SetScrollCurPos(nCurScrollPos);
	}

	m_pList[eLIST_CHAT]->UpdateList();
	m_pChatDesign->erase();
}

void CTalkUI::AddErrorTalk( CTString &strDesc, COLOR colDesc /*= 0xE28769FF */ )
{
	AddTalkListString( "", strDesc, false, colDesc );
}

void CTalkUI::AddTalkTarget( CTString strName )
{
	std::vector<CTString>::iterator iter;

	iter = std::find(m_vecTarget.begin(), m_vecTarget.end(), strName);

	if( iter == m_vecTarget.end() )
	{
		m_vecTarget.push_back(strName);
		updateTargetList();

		if( strName != CUIManager::getSingleton()->GetMessenger()->GetMyInfo().m_strName)
		{
			CTString strMessage;
			strMessage.PrintF(_S( 3036,"%s님이 대화방에 들어오셨습니다."), strName );
			AddErrorTalk( strMessage ); 
		}
	}
}

void CTalkUI::DeleteTalkTarget( CTString strName )
{
	std::vector<CTString>::iterator iter;

	iter = std::find(m_vecTarget.begin(), m_vecTarget.end(), strName);

	//해당 캐릭이 존재하지 않음
	if( iter == m_vecTarget.end() )
		return;

	CTString strMessage;
	strMessage.PrintF(_S(3039,"%s 님이 대화방에서 나가셨습니다."), (*iter) );
	AddErrorTalk( strMessage ); 

	//채팅 멤버 리스트에서 지움.
	m_vecTarget.erase(iter);
	updateTargetList();
}

void CTalkUI::Open( CTString strName )
{
	Hide(FALSE);
	SetVisible(TRUE);

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetMessenger()->CloseAllMessageBox();
	Clear();	
	resetTalkList();

	if (m_pEditInput != NULL)
	{
		m_pEditInput->ResetString();
		m_pEditInput->SetFocus(TRUE);
	}
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	ResetPosition( pDrawPort->dp_MinI, pDrawPort->dp_MinJ, pDrawPort->dp_MaxI, pDrawPort->dp_MaxJ );
		
	AddTalkTarget(strName);
	AddTalkListString( "", _S( 1642, "대화내용을 입력하여주십시오."), true, TALK_SYSTEM_COLOR );

	pUIManager->RearrangeOrder( m_nWhichUI, 1);
}

void CTalkUI::Open( int clientIndex, const CMemberInfo targetInfo )
{
	Hide(FALSE);
	SetVisible(TRUE);

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetMessenger()->CloseAllMessageBox();
	Clear();
	resetTalkList();

	const CDrawPort	*pdp = pUIManager->GetDrawPort();
	ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );

	if (m_pEditInput != NULL)
	{
		m_pEditInput->ResetString();
		m_pEditInput->SetFocus(TRUE);
	}

	m_vecTarget.push_back(pUIManager->GetMessenger()->GetMyInfo().m_strName);
	updateTargetList();
	m_nTargetCharIndex = targetInfo.m_nCharIndex;
	AddTalkTarget(targetInfo.m_strName);
	AddTalkListString( "", _S( 1642, "대화내용을 입력하여주십시오."), true, TALK_SYSTEM_COLOR ); 

	pUIManager->RearrangeOrder( m_nWhichUI, 1);
}

void CTalkUI::ToggleVisibleMemberList()
{
	if (m_pImgSplit[eIMGSPLIT_MEMBER] != NULL)
	{
		BOOL bHide = m_pImgSplit[eIMGSPLIT_MEMBER]->GetHide();
		m_pImgSplit[eIMGSPLIT_MEMBER]->Hide(!bHide);
	}
}

void CTalkUI::ToggleVisibleChangeChatColor()
{
	if (m_pImgChatColor != NULL)
	{
		BOOL bHide = m_pImgChatColor->GetHide();
		m_pImgChatColor->Hide(!bHide);
	}
}

void CTalkUI::Close()
{
	_pNetwork->MgrFriendOut( m_nMakeCharIndex, m_nChatIndex);

	Hide(TRUE);
	SetVisible(FALSE);
	Clear();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( m_nWhichUI, FALSE );
	pUIManager->GetMessenger()->CloseTalk( m_nWhichUI );
}

void CTalkUI::Clear()
{
	m_vecTarget.clear();
	m_vecTalkList.clear();
	resetTargetList();

	m_nMakeCharIndex	=-1;
	m_nTargetCharIndex	= -1;
	m_nColIndex = 0;

	if (m_pEditInput != NULL)
		m_pEditInput->SetFocus(FALSE);
}

void CTalkUI::initialize()
{
#ifndef		WORLD_EDITOR
	int i;
	// check button
	std::string strChkBtnID[eCHK_MAX] = { "chk_member", "chk_chatcolor" };

	for (i = 0; i < eCHK_MAX; ++i)
		m_pCheckBtn[i] = (CUICheckButton*)findUI(strChkBtnID[i].c_str());

	if (m_pCheckBtn[eCHK_COLOR] != NULL)
		m_pCheckBtn[eCHK_COLOR]->SetCommandF(boost::bind(&CTalkUI::ToggleVisibleChangeChatColor, this));

	if (m_pCheckBtn[eCHK_MEMBER] != NULL)
		m_pCheckBtn[eCHK_MEMBER]->SetCommandF(boost::bind(&CTalkUI::ToggleVisibleMemberList, this));

	// image splite
	std::string strImgSplitID[eIMGSPLIT_MAX] = { "is_back", "is_memberlist" };

	for (i = 0; i < eIMGSPLIT_MAX; ++i)
	{
		m_pImgSplit[i] = (CUIImageSplit*)findUI(strImgSplitID[i].c_str());

		if (m_pImgSplit[i] != NULL)
			m_pImgSplit[i]->setInherit(false);
	}

	// list
	std::string strListID[eLIST_MAX] = { "list_chat", "list_member" };

	for (i = 0; i < eLIST_MAX; ++i)
		m_pList[i] = (CUIList*)findUI(strListID[i].c_str());

	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	m_pImgChatColor= (CUIImage*)findUI("img_color");

	if (m_pImgChatColor != NULL)
		m_pImgChatColor->setInherit(false);

	m_pTextMemberInfo = (CUIText*)findUI("text_memberlist");

	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
		m_pBtnClose->SetCommandFUp(boost::bind(&CTalkUI::Close, this));

	m_pArrChatColor = (CUIArray*)findUI("array_color");

	if (m_pArrChatColor != NULL)
		m_pArrChatColor->SetCommandF(boost::bind(&CTalkUI::SelectChatColor, this));

	m_pEditInput = (CUIEditBox*)findUI("edit_input");

	_iMaxMsgStringChar = calcMaxStringCount();

	// Chat Contents
	m_pChatDesign = new CUITextBoxEx;
	m_pChatDesign->SetSplitMode(SPLIT_SPACE);
	m_pChatDesign->SetSize(260, 15);

#endif		// WORLD_EDITOR
}

void CTalkUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	int nOffset = -50;
	for( int iUI = UI_MESSENGER_TALK_START; iUI < UI_MESSENGER_TALK_END; iUI++ )
	{
		if( CUIManager::getSingleton()->GetTalk(iUI)->IsEnabled() )
		{
			nOffset += 10;
		}
	}

	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 + nOffset, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 + nOffset );
}

void CTalkUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CTalkUI::SendChatMessageFiltering( CTString strMessage )
{
	// 운영자 인 경우에 필터링 안함
	if( _pNetwork->m_ubGMLevel > 1 ) {
		SendChatMessage(strMessage);
		return;
	}

	// 운영자 이외의 경우 불량 단어 필터링 
	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strMessage.str_String );

	// Filtering
	if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE )
	{
		AddTalkListString( "", _S( 437, "문장에 금지된 단어가 포함되어 있습니다."), true, TALK_SYSTEM_COLOR ); 
		strMessage.Clear();

		if (IsGamigo(g_iCountry) == TRUE)
		{
			for (int i = 1; i <= nIndexBuffer[0]; i++)
			{	
				strMessage += "[";
				strMessage += _UIFiltering.GetString( nIndexBuffer[i] );
				strMessage += "] ";
			}
			AddTalkListString( "", strMessage, true, TALK_SYSTEM_COLOR ); 
		}		
		return;
	}

	SendChatMessage(strMessage);
}

void CTalkUI::SendChatMessage( CTString strMessage )
{
	if( m_nChatIndex < 0 )	/*서버와 연결이 되어있지 않은 상태*/
	{
		if (UIMGR()->GetMessenger()->GetMemberCondition(m_nTargetCharIndex) == OFFLINE)
		{
			UIMGR()->GetMessenger()->ErrorMessage(_S(1638, "[오프라인] 상태인 대상과는 대화를 할 수 없습니다."));
			return;
		}

		_pNetwork->MgrOneVsOneConnect( CUIManager::getSingleton()->GetMessenger()->GetMyInfo().m_nCharIndex, m_nTargetCharIndex , strMessage );
	}
	_pNetwork->MgrFriendChat(m_nMakeCharIndex, m_nChatIndex, strMessage);
}

void CTalkUI::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	if (m_pEditInput != NULL)
		m_pEditInput->SetFocus( bVisible );
}

bool CTalkUI::IsExistTarget( CTString strName )
{
	std::vector<CTString>::iterator iter;
	iter = std::find(m_vecTarget.begin(), m_vecTarget.end(), strName );

	if( iter != m_vecTarget.end() )
		return true;
	
	return false;
}

BOOL CTalkUI::IsEditBoxFocused()
{
	if (m_pEditInput != NULL)
		return m_pEditInput->IsFocused();
	return FALSE;
}

void CTalkUI::KillFocusEditBox()
{
	if (m_pEditInput != NULL)
		m_pEditInput->SetFocus(FALSE);
}

int CTalkUI::calcMaxStringCount()
{
	int nMaxCount = 0;
	int nWidth = TALK_DESC_CHAR_WIDTH;

	if(m_pList[eLIST_CHAT] != NULL)
	{
		CUIListItem* pItem = m_pList[eLIST_CHAT]->GetListItemTemplate();

		if (pItem != NULL)
			nWidth = pItem->GetWidth();
	}

	if (g_iCountry == RUSSIA)
		nMaxCount = 32;
	else
		nMaxCount = nWidth / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	return nMaxCount;
}

void CTalkUI::updateTargetList()
{
	if (m_pList[eLIST_MEMBER] == NULL)
		return;

	resetTargetList();

	CUIListItem* pTemp = m_pList[eLIST_MEMBER]->GetListItemTemplate();

	if (pTemp == NULL)
		return;

	CUIListItem* pItem = NULL;
	CUIText* pText = NULL;

	int nCount = m_vecTarget.size();

	if (m_pTextMemberInfo != NULL)
	{
		CTString strTitle;
		strTitle.PrintF( _S(3035, "대화 참가 인원 (%d)"), nCount);
		m_pTextMemberInfo->SetText(strTitle);
	}

	for (int i = 0; i < nCount; ++i)
	{
		m_pList[eLIST_MEMBER]->AddListItem(pTemp->Clone());
		pItem = (CUIListItem*)m_pList[eLIST_MEMBER]->GetListItem(i);

		if (pItem == NULL)
			continue;

		pText = (CUIText*)pItem->findUI("text_name");

		if (pText != NULL)
			pText->SetText(m_vecTarget[i]);
	}

	updateTargetListSize();
	m_pList[eLIST_MEMBER]->UpdateList();
}

void CTalkUI::resetTargetList()
{
	if (m_pList[eLIST_MEMBER] != NULL)
		m_pList[eLIST_MEMBER]->DeleteAllListItem();
}

void CTalkUI::resetTalkList()
{
	if (m_pList[eLIST_CHAT] != NULL)
		m_pList[eLIST_CHAT]->DeleteAllListItem();
}

void CTalkUI::updateTargetListSize()
{
	if (m_pImgSplit[eIMGSPLIT_MEMBER] == NULL)
		return;

	if (m_pList[eLIST_MEMBER] == NULL)
		return;

	CUIListItem* pTemp = m_pList[eLIST_MEMBER]->GetListItemTemplate();

	int nItemHeight = DEF_DEFUALT_ITEM_HEIGHT;
	if (pTemp != NULL)
		nItemHeight = pTemp->GetHeight();

	int nCount = m_vecTarget.size();
	
	int nItemGap = m_pList[eLIST_MEMBER]->GetGap();
	int nStartX, nStartY;
	m_pList[eLIST_MEMBER]->GetItemStart(nStartX, nStartY);

	int nWidth = calcMaxTargetNameLength() + (nStartX * 2) + DEF_WIDTH_GAP;
	int nHeight = (nStartY * 2) + ((nItemHeight + nItemGap) * nCount);

	m_pList[eLIST_MEMBER]->SetWidth(nWidth);
	m_pList[eLIST_MEMBER]->SetHeight(nHeight);
	m_pImgSplit[eIMGSPLIT_MEMBER]->SetWidth(nWidth);
	m_pImgSplit[eIMGSPLIT_MEMBER]->SetHeight(nHeight);
	m_pImgSplit[eIMGSPLIT_MEMBER]->UpdateSplit();	
}

int CTalkUI::calcMaxTargetNameLength()
{
	std::vector<CTString>::iterator iter = m_vecTarget.begin();
	std::vector<CTString>::iterator iterEnd = m_vecTarget.end();

	int nFontSize = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int nMaxLength = nFontSize;
	int nCurLength = 0;

	UtilHelp* pHelp = UTIL_HELP();

	for (; iter != iterEnd; ++iter)
	{
		if (g_iCountry == RUSSIA)
			nCurLength = pHelp->GetNoFixedWidth(_pfdDefaultFont, (*iter).str_String);
		else
			nCurLength = (*iter).Length() * nFontSize;

		if ( nCurLength > nMaxLength )
			nMaxLength = nCurLength;
	}

	return nMaxLength;
}

WMSG_RESULT CTalkUI::OnKeyMessage( MSG* pMsg )
{
	if (m_pEditInput == NULL || m_pEditInput->IsFocused() == FALSE) 
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	if( ( wmsgResult = m_pEditInput->KeyMessage( pMsg ) ) != WMSG_FAIL )
	{
		if ( wmsgResult == WMSG_COMMAND )
		{
			SendChatMessageFiltering( m_pEditInput->GetString() );
			m_pEditInput->ResetString();
		}
		return WMSG_SUCCESS; 
	}

	return WMSG_FAIL;
}

WMSG_RESULT CTalkUI::OnCharMessage( MSG* pMsg )
{
	if (m_pEditInput == NULL || m_pEditInput->IsFocused() == FALSE) 
		return WMSG_FAIL;

	if( m_pEditInput->CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	} 

	return WMSG_FAIL;
}

WMSG_RESULT CTalkUI::OnIMEMessage( MSG* pMsg )
{
	if (m_pEditInput == NULL || m_pEditInput->IsFocused() == FALSE) 
		return WMSG_FAIL;

	return m_pEditInput->IMEMessage( pMsg );
}

WMSG_RESULT CTalkUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pEditInput != NULL && m_pEditInput->IsInside(x, y) == TRUE)
		m_pEditInput->SetFocus(TRUE);

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( m_nWhichUI, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CTalkUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (IsInside(x, y) == FALSE)
	{
		pUIManager->GetMessenger()->SetDrop(false);
		return WMSG_FAIL;
	}

	DropMessengerItem();	
	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CTalkUI::SelectChatColor()
{
	if (m_pArrChatColor == NULL)
		return;

	int nSelIdx = m_pArrChatColor->GetSelectIdx();

	if (nSelIdx < 0)
		return;

	m_nColIndex = nSelIdx;
	_pNetwork->MgrSetChatColor(m_nColIndex);

	if (m_pCheckBtn[eCHK_COLOR] != NULL)
		m_pCheckBtn[eCHK_COLOR]->SetCheck(FALSE);

	if (m_pImgChatColor != NULL)
		m_pImgChatColor->Hide(TRUE);
}

void CTalkUI::DropMessengerItem()
{
	SetFocus( TRUE );

	CUIManager* pUIMgr = UIMGR();
	CUIMessenger* pMessenger = pUIMgr->GetMessenger();

	if (pMessenger == NULL || pMessenger->IsDrop() == false)
		return;

	pUIMgr->RearrangeOrder( m_nWhichUI, TRUE );

	CMemberInfo miTemp = pMessenger->GetSelectMember();
	//대화중인 상대가 아니면...
	if( !IsExistTarget(miTemp.m_strName) )
	{
		if( miTemp.m_eCondition == OFFLINE)
 			AddErrorTalk( _S(3037, "대화상대가 [오프라인] 상태이므로 초대할 수 없습니다." ) ); 
 		else
 		{
 			//대화 상대추가
 			if( m_nChatIndex < 0 ) // 1:1대화상태이면  자신의 대화상대도 초대[12/12/2006 KwonYongDae]
 			{
 				_pNetwork->MgrFriendInvite(m_nMakeCharIndex, m_nChatIndex<0? -1:m_nChatIndex, m_nTargetCharIndex );
 				m_nTargetCharIndex = miTemp.m_nCharIndex;
 			}
 			else
 			{
 				_pNetwork->MgrFriendInvite(m_nMakeCharIndex, m_nChatIndex, miTemp.m_nCharIndex);
 			}
 	
 		}
 	}
 	else
 		AddErrorTalk( _S(3038, "이미 대화중인 상대입니다." ) ); 

	pMessenger->SetDrop(false); 
}
