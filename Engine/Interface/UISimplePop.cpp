#include "stdh.h"
#include <Engine/Interface/UISimplePop.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIChatting.h>
#include <Engine/Interface/UITextureManager.h>


// 리스트 박스 크기에 맞게 박스를 그릴때 위,아래/ 좌,우 여백 
#define SPACE_UPDONW	14
#define SPACE_LEFTRIGT	10

CUISimplePop::CUISimplePop()
{
}

CUISimplePop::~CUISimplePop()
{
}

void CUISimplePop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	CUIListBox::Create( NULL, 0,  0, 100, 174, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE, TRUE );
	SetScrollBar( FALSE );
	SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	SetOverColor( 0xF8E1B5FF );
	SetSelectColor( 0xF8E1B5FF );

	m_tpList.m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_tpList.Create( this, 0,  0, 100, 100, _pUIFontTexMgr->GetLineHeight(), 6, 3, 1, TRUE, TRUE );
	m_tpList.SetScrollBar( FALSE );
	m_tpList.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpList.SetOverColor( 0xF8E1B5FF );
	m_tpList.SetSelectColor( 0xF8E1B5FF );

	m_tpList.AddMenuList(_S(3354, "일반"));
	m_tpList.AddMenuList(_S(2664, "입수우선"));
	m_tpList.AddMenuList(_S(2665, "전투 형"));

	//서브 메뉴 툴팁에 사용될 화살표 텍스쳐 추가
	m_ptdMenuTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	fTexWidth = m_ptdMenuTexture->GetPixWidth();
	fTexHeight = m_ptdMenuTexture->GetPixHeight();

	m_rtArrow.SetUV(34, 117, 42, 125, fTexWidth, fTexHeight);		//노란 화살표
}

void CUISimplePop::OpenPop(CTString strUserName, BOOL bParty, BOOL bGuild, INDEX ixPos, INDEX iyPos)
{
	SetExeList(strUserName, bParty, bGuild);
	_pUIMgr->RearrangeOrder( UI_SIMPLE_POP, TRUE );
// [KH_070423] 크기 관련(화면을 넘을 것 같으면 화면에 젤 오른쪽에 붙인다)
	if(ixPos + GetWidth() > _pUIMgr->GetDrawPort()->GetWidth())
		ixPos = _pUIMgr->GetDrawPort()->GetWidth() - GetWidth();
	if(iyPos + GetHeight() > _pUIMgr->GetDrawPort()->GetHeight())
		iyPos = _pUIMgr->GetDrawPort()->GetHeight() - GetHeight();

	SetPos(ixPos, iyPos);
	Show();
}

void CUISimplePop::ClosePop()
{
	_pUIMgr->RearrangeOrder( UI_SIMPLE_POP, FALSE );
	Hide();
	
}

void CUISimplePop::Render()
{
	if( !IsVisible() )
		return;
	if( m_vecString.size() <= 0 )
		return;

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	CUITrackPopup::Render();
	m_tpList.Render();	

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

// [KH_070423] 화살표
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdMenuTexture );

	INDEX nX = GetPosX() + 80;
	INDEX nY = GetPosY();
	INDEX nLine = 4;

	INDEX nYSpace =((_pUIFontTexMgr->GetLineHeight()+3) - 7)/2 + 2;



	_pUIMgr->GetDrawPort()->AddTexture( nX, nY + nYSpace +(_pUIFontTexMgr->GetLineHeight()+2)*nLine,
										nX + 7, nY + nYSpace +(_pUIFontTexMgr->GetLineHeight()+2)*nLine+7,
										m_rtArrow.U0, m_rtArrow.V0, m_rtArrow.U1, m_rtArrow.V1,
										0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

void CUISimplePop::SetExeList(CTString strUserName, BOOL bParty, BOOL bGuild)
{
	INDEX iListNum = 0;
	INDEX iExeNum = SPSO_NAME;

	ResetAllStrings();

	AddMenuList(strUserName, C_YELLOW|0xff);			m_iExeNum[iListNum++] = iExeNum++;
	AddMenuList(_S(3355, "귓속말") + _s("       "));	m_iExeNum[iListNum++] = iExeNum++;
	AddMenuList(_S(2484, "친구등록"));					m_iExeNum[iListNum++] = iExeNum++;
	AddMenuList(_S(3356, "교환신청"));					m_iExeNum[iListNum++] = iExeNum++;
	AddMenuList(_S(3357, "파티신청"));					m_iExeNum[iListNum++] = iExeNum++;

	if(bParty)
	{
		AddMenuList(_S(3358, "파티 강퇴"));				m_iExeNum[iListNum++] = iExeNum++;
		AddMenuList(_S(3359, "파티장 위임"));			m_iExeNum[iListNum++] = iExeNum++;
	}
	else
		iExeNum += 2;

	AddMenuList(_S(3360, "길드가입"));					m_iExeNum[iListNum++] = iExeNum++;

	if(bGuild)
	{
		CTString tempString;
		tempString.PrintF("%s %s", _S(1097, "길드전투"), _S(124, "신청"));
		AddMenuList(tempString);						m_iExeNum[iListNum++] = iExeNum++;
		tempString.PrintF("%s %s", _S(1097, "길드전투"), _S(3361, "중단"));
		AddMenuList(tempString);						m_iExeNum[iListNum++] = iExeNum++;
		
	}	
	else
		iExeNum += 2;

	AddMenuList(_S(139,  "취소"));			m_iExeNum[iListNum++] = iExeNum++;

	while(iListNum < SPSO_END)
		m_iExeNum[iListNum++] = SPSO_END;
}

void CUISimplePop::Execution(INDEX exeNum)
{
	switch(exeNum)
	{
	case SPSO_NAME:
		NOTHING;
		break;
	case SPSO_WHISPER:
		{
			CTString tempInput;

			ClosePop();		// [KH_070424] 귓속말시 채팅창 포커스가 꺼져서 미리 꺼버림
			tempInput.PrintF("!%s ", _pNetwork->_TargetInfo.TargetName);
			_pUIMgr->GetChatting()->OpenAndSetString(tempInput);
		}
		break;
	case SPSO_FRIEND:
		_pUIMgr->GetMessenger()->MsgBoxCommand( MSGCMD_MESSENGER_ADD_REQ, TRUE, CTString(_pNetwork->_TargetInfo.TargetName) );
		break;
	case SPSO_TRADE:
		_pUIMgr->GetCharacterInfo()->UseAction( 5 );
		break;
	case SPSO_PARTY:
		NOTHING;
		break;
	case SPSO_PARTY_OUT:
		_pUIMgr->GetCharacterInfo()->UseAction( 12 );
		break;
	case SPSO_PARTY_JANG:
		_pUIMgr->GetCharacterInfo()->UseAction( 39 );
		break;
	case SPSO_GUILD_IN:
		_pUIMgr->GetCharacterInfo()->UseAction( 24 );
		break;
	case SPSO_GUILD_WAR_START:
		_pUIMgr->GetCharacterInfo()->UseAction( 31 );
		break;
	case SPSO_GUILD_WAR_END:
		_pUIMgr->GetCharacterInfo()->UseAction( 32 );
		break;
	case SPSO_CANCEL:
		NOTHING;
		break;
	case SPSO_END:
		ASSERTALWAYS("이것이 들어오면 안되는 겁니다.");
		break;

	case SPST_PARTY_JOIN_A:
		_pUIMgr->GetCharacterInfo()->UseAction( 6 );
		break;
	case SPST_PARTY_JOIN_B:
		_pUIMgr->GetCharacterInfo()->UseAction( 7 );
		break;
	case SPST_PARTY_JOIN_C:
		_pUIMgr->GetCharacterInfo()->UseAction( 8 );
		break;
	default:
		ASSERTALWAYS("이것이 들어오면 안되는 겁니다.");
		break;
	}
}

WMSG_RESULT	CUISimplePop::MouseMessage(MSG *pMsg)
{
	static int	nOldX, nOldY;
	
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	if(pMsg->message == WM_LBUTTONUP)
	{
		INDEX iExeNum = SPSO_END;
		if( m_tpList.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			iExeNum = m_tpList.GetCurSel() + SPST_START;

			Execution(iExeNum);
			
			m_tpList.Hide();
			ClosePop();

			return WMSG_SUCCESS;
		}
		else if( CUITrackPopup::MouseMessage( pMsg ) != WMSG_FAIL )
		{
			iExeNum = m_iExeNum[GetCurSel()];
			m_tpList.Hide();

			Execution(iExeNum);

			if(iExeNum != SPSO_NAME && iExeNum != SPSO_PARTY && iExeNum != SPSO_WHISPER)
				ClosePop();

			return WMSG_SUCCESS;
		}
		else
		{
			ClosePop();
			if(m_tpList.IsEnabled())
				m_tpList.Hide();
		}

	}
	else if(pMsg->message == WM_MOUSEMOVE && (m_tpList.MouseMessage( pMsg ) != WMSG_FAIL || m_iExeNum[GetCurSel()] == SPSO_PARTY))
	{	// [KH_070423] m_tpList.MouseMessage 는 실행을 안하면 선택표시가 안되므로 해주는 것이다. (if 구문과는 별 상관 없음)
		if(m_iExeNum[GetCurSel()] == SPSO_PARTY)
		{
			int PosX = GetWidth() - 3;
			int PosY = (_pUIFontTexMgr->GetLineHeight()+2)*4;

// [KH_070423] 크기관련 추가
			if(GetPosX() + GetWidth() + m_tpList.GetWidth() > _pUIMgr->GetDrawPort()->GetWidth())
				PosX = 3 - m_tpList.GetWidth();

			m_tpList.SetPos( PosX, PosY );
			m_tpList.Show();
		}

		return CUITrackPopup::MouseMessage(pMsg);
	}
	else
	{
		if(pMsg->message != WM_LBUTTONDOWN)
			m_tpList.Hide();
		return CUITrackPopup::MouseMessage(pMsg);
	}

	return WMSG_FAIL;
}
