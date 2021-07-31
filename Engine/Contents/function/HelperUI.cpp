#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "HelperUI.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include "HelperManager.h"

#include <vector>
#include <algorithm>

#define MAX_HELPER_LEVEL				(5)
#define INFO_MAX						(4)
#define	HELPER_MAX_STUDENTS				(20)		// 최대 가입 신청자.

class CmdHelperMouseOver : public Command
{
public:
	CmdHelperMouseOver() : m_pWnd(NULL), m_nOver(-1)	{} ;
	void setData(CHelperUI* pWnd, int nOver)	{ m_pWnd = pWnd; m_nOver = nOver;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->ChangeOverColor(m_nOver);
	}
private:
	CHelperUI* m_pWnd;
	int m_nOver;
};

class CmdHelperMouseLeave : public Command
{
public:
	CmdHelperMouseLeave() : m_pWnd(NULL), m_nLeave(-1)	{} ;
	void setData(CHelperUI* pWnd, int nLeave)	{ m_pWnd = pWnd; m_nLeave = nLeave;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->ChangeLeaveColor(m_nLeave);
	}
private:
	CHelperUI* m_pWnd;
	int m_nLeave;
};

CHelperUI::CHelperUI()
	: m_pBtnRegistTeacher(NULL)
	, m_pListDisciple(NULL)
	, m_pListInfo(NULL)
	, m_nOldMouseOverIdx(-1)
{
	setInherit(false);
}

CHelperUI::~CHelperUI()
{
	Destroy();
}

void CHelperUI::initialize()
{
#ifndef		WORLD_EDITOR
	CUIBase* pBase = NULL;
	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pListDisciple = (CUIList*)findUI("list_disciple");
	m_pListInfo		= (CUIList*)findUI("list_info");

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::CloseUI, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::CloseUI, this));

	if (pBase = findUI("btn_amend"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::AmendReq, this));

	if (pBase = findUI("btn_abandon"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::FireMyStudent, this));

	m_pBtnRegistTeacher = (CUIButton*)findUI("btn_enrolment");
#endif // WORLD_EDITOR
}

void CHelperUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CHelperUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CHelperUI::OpenUI( )
{
	if (GetHide() == FALSE || IsVisible() == TRUE)
		return;

	RefreshStudentList(TRUE);

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_HELPER, TRUE);
}

static LONG g_lChaIndex				= -1;		// 가입 신청과 관련된 부분에만 쓰임.
static CTString g_strTeacherName	= CTString("");

void CHelperUI::CloseUI()
{
	g_lChaIndex				= -1;
	g_strTeacherName.Clear();

	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_HELPER, FALSE );
}

void CHelperUI::SetRegistredTeacher( BOOL bRegistered )
{
	if (m_pBtnRegistTeacher == NULL)
		return;

	CTString strTemp;

#ifndef		WORLD_EDITOR
	// 후견인 리스트에 등록이 안되어 있을 경우
	if (bRegistered == FALSE)
	{
		m_pBtnRegistTeacher->SetCommandFUp(boost::bind(&CHelperUI::RegisterTeacher, this));
		strTemp = _S(1136, "후견인 등록");
	}
	else
	{
		m_pBtnRegistTeacher->SetCommandFUp(boost::bind(&CHelperUI::CancelRegisterTeacher, this));
		strTemp = _S(1137, "후견인 등록 취소");
	}
#endif		// WORLD_EDITOR

	m_pBtnRegistTeacher->SetText(strTemp);
}

// ----------------------------------------------------------------------------
// Name : TeacherAccept()
// Desc : 후견인이 되어주겠는지 물어봄.
// ----------------------------------------------------------------------------
void CHelperUI::TeacherAccept( LONG lIndex, const CTString& strStudentName )
{
	g_lChaIndex			= lIndex;
	g_strTeacherName	= strStudentName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );

	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1154, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_STUDENT_ACCEPT );	
	strMessage.PrintF( _S( 1155,  "[%s] 님이 후견인이 되어달라는 부탁을 합니다. 수락하시겠습니까?"  ),  strStudentName );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelperUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_NOTIFY:		// 에러 발생
		{			
		}
		break;
	case MSGCMD_HELPER_REGISTER:			// 선생 목록에 추가합니다.
		{
			if( !bOK )
				return;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strTemp;
			MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "후견인 시스템"), UMBS_OKCANCEL | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX, UI_HELPER, MSGCMD_HELPER_PLAYTIME, 250);	
			MsgBoxInfo.AddString( _S( 5073, "주로 접속하는 시간을 입력해주세요.") );
			MsgBoxInfo.AddString( _S( 5074, "(견습생이 후견인 선정에 있어 참고가 됩니다.)") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddStringEx( _s("h ~ "), 5, 15 );
			MsgBoxInfo.AddStringEx( _s("h"), 5, 30 );
			MsgBoxInfo.SetInputBox( 5, 5, 2, 50 );
			MsgBoxInfo.SetSEInputBox( 5 , 20, 2, 50 );
			MsgBoxInfo.m_nInputPosY += 4;
			MsgBoxInfo.m_nSEInputPosY += 4;
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGCMD_HELPER_CANCEL_REGISTER:		// 선생 목록에서 내림
		{
			if( !bOK )
				return;
			// 서버에 메세지를 보냄.
			_pNetwork->TeachTeacherCancelRegister();
		}
		break;	

	case MSGCMD_HELPER_STUDENT_ACCEPT:
		// 학생이 후견인 신청을 요청한 상태에서...(현재 선생의 입장이지...)
		// 허가냐? 취소냐?
		{
			if( !bOK )		// 학생을 안받아 들임
			{
				_pNetwork->TeachTeacherReject( FALSE, g_lChaIndex, g_strTeacherName );
			}
			else			// 학생 받어~!!!
			{
				_pNetwork->TeachTeacherAccept( _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name, 
					g_lChaIndex, g_strTeacherName );
			}			
		}
		break;
	case MSGCMD_HELPER_FIRE_MYSTUDENT:
		{	// 선생이 학생 포기
			if ( !bOK )
				return;
			int nPos = m_pListDisciple->getCurSel();

			if (nPos < 0)
				return;

			CHelperManager::sHelperMember* member = GameDataManager::getSingleton()->GetHelperManager()->GetHelperMember(nPos);
			int nIndex = member->lIndex;
			CTString strStudentName = member->strName;
			_pNetwork->TeachTeacherGiveUp(nIndex, strStudentName, _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name);
		}
		break;
	}
}

void CHelperUI::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

void CHelperUI::RegisterTeacher( )					// 후견인 등록
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1164, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_REGISTER );	
	strMessage.PrintF( _S( 1165, "후견인 목록에 등록하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelperUI::CancelRegisterTeacher( )			// 후견인 등록 취소
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1166, "후견인 등록 취소" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_CANCEL_REGISTER );	
	strMessage.PrintF( _S( 1167, "후견인 목록에서 삭제하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : Edit Box 2개일 때,
// ----------------------------------------------------------------------------
void CHelperUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_PLAYTIME:
		{
			if ( bOK )
			{
				// 후견인 신청 메시지 보내기 : Client -> Server
				CTString strMessage = CTString("");
				CUIMsgBox_Info MsgBoxInfo;
				int sbStartPlayTime;
				int sbEndPlayTime;

				if ( strInput.IsInteger() && strSEInput.IsInteger() )
				{
					sbStartPlayTime = atoi(strInput);
					sbEndPlayTime = atoi(strSEInput);

					if ( sbStartPlayTime > 24 || sbEndPlayTime > 24 )
					{
						strMessage = _S( 5080, "24 이하의 숫자를 입력해주세요.");
					}
					else if ( sbStartPlayTime < 0 || sbEndPlayTime < 0 )
					{
						strMessage = _S( 4348, "0 이상의 숫자를 입력해 주시기 바랍니다.");
					}
				}
				else
				{
					strMessage = _S(4351, "숫자를 입력해 주시기 바랍니다.");
				}

				if (strMessage.Length() > 0)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "후견인 시스템"), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage );
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
					return;
				}

				_pNetwork->TeachTeacherRegister( sbStartPlayTime, sbEndPlayTime );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : FireMyStudent()
// Desc : 후견인이 견습생 포기
// ----------------------------------------------------------------------------
void CHelperUI::FireMyStudent()
{
	int nPos = m_pListDisciple->getCurSel();
	if ( nPos >= 0 )
	{
		CHelperManager* hm = GameDataManager::getSingleton()->GetHelperManager();
		CHelperManager::sHelperMember* pData = hm->GetHelperMember(nPos);

		CTString	strStudentName = pData->strName;
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 5068, "후견인 포기" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_FIRE_MYSTUDENT );		
		strMessage.PrintF( _S( 5081, "[%s]님의 후견인을 그만 두시겠습니까? 그만 두게되면 명성치 5를 잃게 됩니다." ), strStudentName );	
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );	
	}
}

// ----------------------------------------------------------------------------
// Name : SetMyTeachInfo()
// Desc : 내 정보 설정
// ----------------------------------------------------------------------------
void CHelperUI::SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt )
{
	if (m_pListInfo == NULL)
		return;

	if (m_pListInfo->GetItemCount() < INFO_MAX)
		return;

	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CTString strTmp;
	int i;
	// 양성중인 인원, 양성 완료 인원, 양성 실패 인원, 양성 성공률
	int nStr_idx[INFO_MAX] = {5077, 5078, 5082, 5079};
	// 양성 성공률
	int nCompletePercent = 0;

	if (CompleteTeachCnt + FailTeachCnt > 0)
		nCompletePercent = int(CompleteTeachCnt / float(CompleteTeachCnt + FailTeachCnt) * 100 + 0.5f);

	int nCnt[INFO_MAX] = {CurTeachCnt, CompleteTeachCnt, FailTeachCnt, nCompletePercent};

	for (i = 0; i < INFO_MAX; ++i)
	{
		pItem = m_pListInfo->GetListItem(i);

		if (pItem == NULL)
			continue;

		if (pText = (CUIText*)pItem->findUI("txt_title"))
		{
			pText->SetText(_S(nStr_idx[i], " "));
		}

		if (pText = (CUIText*)pItem->findUI("txt_cnt"))
		{
			// 성공률은 퍼센티지로 입력.
			if (i == 3)
				strTmp.PrintF("%d%%", nCnt[i]);
			else
				strTmp.PrintF("%d", nCnt[i]);

			pText->SetText(strTmp);
		}

	}
}

// ----------------------------------------------------------------------------
// Name : RefreshStudentList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CHelperUI::RefreshStudentList( BOOL bInit )
{
	if( !bInit && GetHide() == TRUE )
		return;

	m_pListDisciple->DeleteAllListItem();
	CHelperManager* hm = GameDataManager::getSingleton()->GetHelperManager();
	CHelperManager::_vec_member list = hm->GetHelperList();
	std::sort(list.begin(), list.end());

	COLOR		crLevel = 0xAAAAAAFF;
	CTString strLevel;
	CUIBase* pTmp = m_pListDisciple->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CUIImageArray* pImgArr = NULL;

	CHelperManager::_vec_member::const_iterator member_end = list.end();
	CHelperManager::_vec_member::iterator it = list.begin();
	int i = 0;

	for(it = list.begin(); it != member_end; ++it)
	{		
		if((*it).iJob>=0 && (*it).iJob<TOTAL_JOB)
		{
			const int iJob		= (*it).iJob;
			const int iJob2		= (*it).iJob2;
			const int iLevel	= (*it).iLevel;
			strLevel.PrintF( "%d", iLevel );

			m_pListDisciple->AddListItem(pTmp->Clone());
			pItem = m_pListDisciple->GetListItem(i);
			
			if (pItem == NULL)
				continue;

			if (pImgArr = (CUIImageArray*)pItem->findUI("img_class"))
			{
				pImgArr->SetRenderIdx(iJob);
				pImgArr->setTooltip(CJobInfo::getSingleton()->GetName(iJob,  iJob2 ));
			}
			
			if (pText = (CUIText*)pItem->findUI("txt_name"))
			{
				pText->SetText((*it).strName);
				pText->setFontColor(0xffcc66FF);
			}

			if (pText = (CUIText*)pItem->findUI("txt_lv"))
			{
				pText->SetText(strLevel);
				pText->setFontColor(0xffcc66FF);
			}

			CmdHelperMouseOver* pCmdOver = new CmdHelperMouseOver();
			pCmdOver->setData(this, i);
			pItem->SetCommandEnter(pCmdOver);

			CmdHelperMouseLeave* pCmdLeave = new CmdHelperMouseLeave();
			pCmdLeave->setData(this, i);
			pItem->SetCommandLeave(pCmdLeave);

			++i;
		} 
		else 
			ASSERTALWAYS("JOB NUMBER must have this range [iJob>=0 && iJob<TOTAL_JOB]!!");
	}

	m_pListDisciple->UpdateList();
	m_pListDisciple->UpdateScroll(m_pListDisciple->getListItemCount());
}

void CHelperUI::AmendReq()
{
	_pNetwork->TeachGiftReq();
}

void CHelperUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_HELPER_REQ:
		{
			switch ( nResult ) 
			{
			case SEL_HELPER_TEACHER:		// 견습생 관리
				{
					OpenUI();
				}
				break;
			}
		}
		break;
	}
}

void CHelperUI::ChangeOverColor(int idx)
{
	if (idx < 0)
		return;

	CUIBase* pTemp = NULL;
	CUIText* pText = NULL;

	pTemp = m_pListDisciple->GetListItem(idx);
	if (pTemp != NULL)
	{
		if (pText = (CUIText*)pTemp->findUI("txt_name"))
			pText->setFontColor(0xF8E1B5FF);

		if (pText = (CUIText*)pTemp->findUI("txt_lv"))
			pText->setFontColor(0xF8E1B5FF);
	}
}

void CHelperUI::ChangeLeaveColor( int idx )
{
	if (idx < 0)
		return;

	CUIBase* pTemp = NULL;
	CUIText* pText = NULL;

	pTemp = m_pListDisciple->GetListItem(idx);
	if (pTemp != NULL)
	{
		if (pText = (CUIText*)pTemp->findUI("txt_name"))
			pText->setFontColor(0xffcc66FF);

		if (pText = (CUIText*)pTemp->findUI("txt_lv"))
			pText->setFontColor(0xffcc66FF);
	}
}
