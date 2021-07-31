#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/LocalDefine.h>
#include <algorithm>

static int	_iMaxDescStringChar = 0;
static int	_iMaxSelStringChar = 0;
static int	_nMsgBoxLineHeight = 0;
static int	_iMaxDescStringCharNew = 0;				// [090601: selo] 신규 퀘스트 북을 위한 한 열에 들어가는 문자열 최대 길이
static int  _iMaxSelStringCharNew = 0;				// [090601: selo] 신규 퀘스트 북을 위한 한 열에 들어가는 선택된 문자열 최대 길이
#define MAX_QUEST		16

extern INDEX g_iCountry;

BOOL CUIQuestBook::m_bLockRequestQuest;
float CUIQuestBook::m_fLastRequestTime;

const int iQuestListTitleTextOffsetX	= 5;		// 타이틀 Text X 오프셋
const int iQuestListTitleTextOffsetY	= 25;		// 타이틀 Text Y 오프셋
const int iQuestListTopHeight			= 26;
const int iQuestListDescHeight			= 164;
const int iQuestListBottomHeight		= 7;
const int iQuestListWidth				= 475;		// UI 전체 너비
const int iQuestListHeight				= 331;		// UI 전체 높이

const int iQuestListTitleHeight			= 36;		// 타이틀 높이
const CTString strQuestListTexFileName  = "Data\\Interface\\QuestBook.tex";
const CTString strCommonBtnTexFileName  = "Data\\Interface\\CommonBtn.tex";


// ----------------------------------------------------------------------------
// Name : CUIQuestBook()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuestBook::CUIQuestBook( int nUIIndex )
: m_nUIIndex( nUIIndex )
{
	m_bTitleBarClick		= FALSE;
	m_nSelectedQuestIndex	= -1;
	m_bLockRequestQuest		= FALSE;
	m_nTargetIndex			= -1;
	m_nTargetUIType			= -1;	
	m_pQuestItem			= NULL;
	m_bProceedListExtended	= TRUE;
	m_bCompleteListExtended	= TRUE;	
	m_bRaidMessageListExtended = TRUE;
	m_nPrevNPCIndex			= -1;
	m_nPrevNPCUIType		= -1;
	m_nSortType				= 0;
	m_bLockQuestList		= FALSE;
	
	for( int i = 0; i < QCATEGORY_END; ++i )
		m_bCategoryListExtended[i] = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuestBook()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuestBook::~CUIQuestBook()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
#ifdef NEW_QUESTBOOK
	//if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{
		CreateQuestListNew(pParentWnd, nX, nY, nWidth, nHeight);
		return;
	}
#endif

	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxDescStringChar = 266 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxSelStringChar = 275 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 311, 22 );	

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddleScrollL.SetUV( 0, 27, 40, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollM.SetUV( 40, 27, 176, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollR.SetUV( 176, 27, 216, 29, fTexWidth, fTexHeight );

	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 279, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 240, 260, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnCancel.Create( this, _S( 870, "닫기" ), 240, 260, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnDeny.Create( this, _S( 1696, "거절" ), 240, 260, 63, 21 );			
	m_btnDeny.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnDeny.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_DISABLE );
		
	m_btnGiveUp.Create( this, _S( 1697, "퀘스트 포기" ), 8, 260, 95, 21 );		
	m_btnGiveUp.SetUV( UBS_IDLE, 134, 117, 229, 138, fTexWidth, fTexHeight );
	m_btnGiveUp.SetUV( UBS_CLICK, 134, 139, 229, 160, fTexWidth, fTexHeight );
	m_btnGiveUp.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGiveUp.CopyUV( UBS_IDLE, UBS_DISABLE );
		
	m_btnAccept.Create( this, _S( 1698, "수락" ), 8, 260, 95, 21 );		
	m_btnAccept.SetUV( UBS_IDLE, 134, 117, 229, 138, fTexWidth, fTexHeight );
	m_btnAccept.SetUV( UBS_CLICK, 134, 139, 229, 160, fTexWidth, fTexHeight );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnReserve.Create( this, _S( 1699, "보류" ), 160, 260, 63, 21 );		
	m_btnReserve.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnReserve.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_DISABLE );

	// == 2009. 05. 26 김정래
	// 확장팩 기획에 따른 이전 버튼(NPC 메뉴로 돌아가기) 추가	
	m_btnPrev.Create( this, _S( 2681, "이전" ), 160, 260, 63, 21 );		
	m_btnPrev.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );	
	// == End 2009. 05. 26 김정래
	
	// List box
	m_lbDescription.Create( this, 7, 26, 288, 226, _nMsgBoxLineHeight, 12, 8, 1, TRUE, FALSE );
	m_lbDescription.CreateScroll( TRUE, 0, 0, 9, 224, 9, 7, 0, 0, 10 );
	// Up button
	m_lbDescription.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbDescription.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbDescription.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
	m_lbDescription.SetSelectColor( 0xF8E1B5FF );

	// List box
	m_lbQuestList.Create( this, 7, 26, 288, 226, _nMsgBoxLineHeight, 12, 8, 2, TRUE );
	m_lbQuestList.CreateScroll( TRUE, 0, 0, 9, 224, 9, 7, 0, 0, 10 );
	m_lbQuestList.SetSelBar( 280, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbQuestList.SetOverColor( 0xF8E1B5FF );
	m_lbQuestList.SetSelectColor( 0xF8E1B5FF );
	m_lbQuestList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbQuestList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbQuestList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbQuestList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
	// pop button
	m_lbQuestList.SetPopBtnSpace(1, 0);
	m_lbQuestList.SetPopBtnSize(14, 14);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE, 230, 33, 230+14, 33+14, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK, 230, 117, 230+14, 117+14, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE_DISABLE, 230, 48, 230+14, 48+14, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK_DISABLE, 230, 132, 230+14, 132+14, fTexWidth, fTexHeight);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	switch(m_nUIIndex)
	{
	case UI_QUESTBOOK_LIST:
		{
#ifdef NEW_QUESTBOOK
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 44, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 44 );
#else
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 44, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 44 );
#endif
		}break;
	case UI_QUESTBOOK_CONTENT:
		{
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 22, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 22 );
		}break;
	case UI_QUESTBOOK_COMPLETE:
		{
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 0, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 0 );
		}break;
	case UI_QUESTBOOK_NEW:
		{
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 + 22, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 + 22 );
		}break;
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ToggleVisible()
{
	if( IsVisible() )
		CloseQuestBook();
	else
		OpenQuestBook();
}

// ----------------------------------------------------------------------------
// Name : OpenQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::OpenQuestBook( int iQuestIndex, CItems* pQuestItem )
{	
	switch(m_nUIIndex)
	{
	case UI_QUESTBOOK_LIST:
		{
			m_strTitle	= _S( 1700, "퀘스트 북" );	
		}break;
	case UI_QUESTBOOK_CONTENT:
		{
			m_strTitle	= _S( 1701, "퀘스트 내용" );	
		}break;
	case UI_QUESTBOOK_COMPLETE:
		{
			m_strTitle	= _S( 1702, "퀘스트 보상" );	
		}break;
	case UI_QUESTBOOK_NEW:
		{
			m_strTitle	= _S( 99, "퀘스트" );
		}break;
	}

	if( IsVisible() && m_nUIIndex == UI_QUESTBOOK_LIST )
		return;//이 함수 대신 RefreshQuestList를 사용
	// 퀘스트 인덱스가 변화가 없는데, 보여지는 상태라면...
	if( IsVisible() && m_nSelectedQuestIndex == iQuestIndex )
		return;

	if( ( m_nUIIndex != UI_QUESTBOOK_LIST ) )
		InitQuestBook();

	m_nSelectedQuestIndex	= iQuestIndex;

	// 퀘스트 목록
	if( m_nUIIndex == UI_QUESTBOOK_LIST )
	{
		m_btnCancel.SetEnable( TRUE );
#ifdef NEW_QUESTBOOK // [090601: selo] 신규 퀘스트 목록은 퀘스트 내용을 포함한다.
		m_lbDescription.SetEnable( TRUE );
#else
		m_lbDescription.SetEnable( FALSE );
#endif
		m_lbQuestList.SetEnable( TRUE );

		RefreshQuestList();
	}
	// 퀘스트 내용
	else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
	{
		m_btnCancel.SetEnable( TRUE );
		m_lbDescription.SetEnable( TRUE );
		m_lbQuestList.SetEnable( FALSE );

		RefreshQuestContent(m_nSelectedQuestIndex);
	}
	// 퀘스트 완료
	else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
	{
		if(iQuestIndex == -1 && !m_vectorCompleteQuestList.empty())
		{
			m_nSelectedQuestIndex = iQuestIndex = m_vectorCompleteQuestList[0].iQuestIndex;
		}
		if(m_nSelectedQuestIndex == -1) return;

		m_nSelectPrizeCount = -1;
		m_nSelectPrizeFirstIndex = -1;

		// NOTE : 차원의 문은 보상 인터페이스가 뜨면 안됩니다.
		if( iQuestIndex == 105 )
		{
			// FIXME : NPC 인덱스까지 하드 코딩...ㅡ.ㅡ
			m_nTargetIndex	= 71;
			PrizeQuest();
			CQuestSystem::Instance().DeleteCurrentRequest();
			return;
		}
		else
		{			
			m_btnCancel.SetEnable( FALSE );
			m_lbDescription.SetEnable( TRUE );
			m_lbQuestList.SetEnable( FALSE );

			m_btnPrev.SetEnable( TRUE );
			if( 71 == _pUIMgr->GetQuestBookList()->m_nTargetIndex )
				m_btnPrev.SetEnable( FALSE );
			
			RefreshQuestPrize(m_nSelectedQuestIndex);
		}
	}
	// 신규 퀘스트
	else if( m_nUIIndex == UI_QUESTBOOK_NEW )
	{
		if( !m_pQuestItem )
		{
			m_pQuestItem	= pQuestItem;			
		}
		if(iQuestIndex == -1 && !m_vectorProceedQuestList.empty())
		{
			if(m_iNextQuestIndex == -1)
			{
				m_nSelectedQuestIndex = iQuestIndex = m_vectorProceedQuestList[0].iQuestIndex;
			}
			else
			{
				//다음 것을 찾아보고 없으면 처음 것으로.
				for(int i=0; i<m_vectorProceedQuestList.size(); ++i)
				{
					if(m_vectorProceedQuestList[i].iQuestIndex == m_iNextQuestIndex)
					{
						m_nSelectedQuestIndex = iQuestIndex = m_vectorProceedQuestList[i].iQuestIndex;
						break;
					}
				}
				if(i == m_vectorProceedQuestList.size())
					m_nSelectedQuestIndex = iQuestIndex = m_vectorProceedQuestList[0].iQuestIndex;
			}
			m_iNextQuestIndex = -1;
		}
		if(m_nSelectedQuestIndex == -1) return;

#ifndef NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 수정
		m_btnCancel.SetEnable( FALSE );
#else
		m_btnCancel.SetEnable( TRUE );
#endif
		m_lbDescription.SetEnable( TRUE );
		m_lbQuestList.SetEnable( FALSE );
		
		m_btnPrev.SetEnable( TRUE );
		if( 71 == _pUIMgr->GetQuestBookList()->m_nTargetIndex )
			m_btnPrev.SetEnable( FALSE );

		RefreshQuestNew(m_nSelectedQuestIndex);
	}
	CQuestSystem::Instance().DeleteCurrentRequest();
	_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestContent(INDEX questIndex)
{
#ifndef NEW_QUESTBOOK	// [090601: selo] 신규 퀘스트 북(퀘스트리스트)은 컨텐츠 UI 를 따로 두지 않는다.
	ASSERT(m_nUIIndex == UI_QUESTBOOK_CONTENT);		
#endif
	m_btnGiveUp.SetEnable(TRUE);
	if( isRaidMessage(questIndex) )
	{
		RefreshQuestContentByRaid(questIndex);
		return;
	}

	if(questIndex != m_nSelectedQuestIndex)
		return;
	int oldSbPos = m_lbDescription.GetScrollBarPos();
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();

	CQuestDynamicData *pQuestDD = NULL;
	if( !isRaidMessage(questIndex) )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);	
	}
	else
	{
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(questIndex));
		pQuestDD = &qdd;	
	}

	
	// 2009. 06. 01 김정래
	// 확장팩 퀘스트 북 변경 처리에 의한 설명글의 순서 변경

#ifndef NEW_QUESTBOOK	// [090601: selo] 퀘스트 수락 설명 순서 변경
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );
	AddDescString( CTString(pQuestDD->GetDesc()), pQuestDD->GetColorIntroDesc() );		
	AddDescString( CTString("\n"), 0xFFFFFFFF );		
	AddDescString( CTString(pQuestDD->GetDesc3()), 0x34B603FF );	
#else
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );	
	AddDescString( CTString("\n"), 0xFFFFFFFF );		
	AddDescString( CTString(pQuestDD->GetDesc3()), 0x34B603FF );
	AddDescString( CTString("\n"), 0xFFFFFFFF );
	if(pQuestDD->GetCountStatusDesc() > 0)
		AddDescString( _S( 1703, "\n진행상황" ), 0xFFFFFFFF );
	for(INDEX i=0; i<pQuestDD->GetCountStatusDesc(); ++i)
		AddDescString( pQuestDD->GetStatusDesc( i ), pQuestDD->GetColorStatusDesc( i ) );
#endif
	
	//고정보상
	if(pQuestDD->IsPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetPrizeDesc( 0 ), pQuestDD->GetColorPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorPrizeDesc( i ) );
			AddDescString( CTString("     "), pQuestDD->GetColorPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1))
				, pQuestDD->GetPrizeIndex(i-1));
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetOptionPrizeDesc( 0 ), pQuestDD->GetColorOptionPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountOptionPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorOptionPrizeDesc( i ) );
			AddDescString( CTString("     "), pQuestDD->GetColorOptionPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1))
				, pQuestDD->GetOptionPrizeIndex(i-1));
		}
	}

#ifdef NEW_QUESTBOOK	// [090526: selo] 퀘스트 수락 설명 순서 변경	
	AddDescString( CTString("\n"), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD->GetDesc()), pQuestDD->GetColorIntroDesc() );
#else
	if(pQuestDD->GetCountStatusDesc() > 0)
		AddDescString( _S( 1703, "\n진행상황" ), 0xFFFFFFFF );
	for(INDEX i=0; i<pQuestDD->GetCountStatusDesc(); ++i)
		AddDescString( pQuestDD->GetStatusDesc( i ), pQuestDD->GetColorStatusDesc( i ) );

	AddDescString( _S( 1704, "수행가능 레벨" ), 0xFFFFFFFF );
	CTString strTemp;
	if(pQuestDD->GetNeedMinLevel() == pQuestDD->GetNeedMaxLevel())
	{
		strTemp.PrintF(_S( 1705, "레벨 %d" ), pQuestDD->GetNeedMinLevel());
	}
	else
	{
		strTemp.PrintF(_S( 1706, "레벨 %d ~ 레벨 %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
	}
	AddDescString( strTemp, 0xD0D0FFFF );
#endif

	m_lbDescription.SetScrollBarPos(oldSbPos);
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestNew()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestNew(INDEX questIndex)
{
	ASSERT(m_nUIIndex == UI_QUESTBOOK_NEW);
	if(questIndex != m_nSelectedQuestIndex)
		return;
	CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create( questIndex );

	// 2009. 05. 26 김정래
	// 확장팩 퀘스트 북 변경 처리에 의한 설명글의 순서 변경

#ifndef NEW_QUESTBOOK	// [090526: selo] 퀘스트 수락 설명 순서 변경
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );
	AddDescString( CTString(pQuestDD->GetDesc()), pQuestDD->GetColorIntroDesc() );		
	AddDescString( CTString("\n"), 0xFFFFFFFF );		
	AddDescString( CTString(pQuestDD->GetDesc3()), 0x34B603FF );
#else
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );	
	AddDescString( CTString("\n"), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD->GetDesc3()), 0x34B603FF );
#endif
	
	//고정보상
	if(pQuestDD->IsPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetPrizeDesc( 0 ), pQuestDD->GetColorPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorPrizeDesc( i ) );
			AddDescString( CTString("     "), pQuestDD->GetColorPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1))
				, pQuestDD->GetPrizeIndex(i-1));
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetOptionPrizeDesc( 0 ), pQuestDD->GetColorOptionPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountOptionPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorOptionPrizeDesc( i ) );
			AddDescString( CTString("     "), pQuestDD->GetColorOptionPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1))
				, pQuestDD->GetOptionPrizeIndex(i-1));
		}
	}

#ifdef NEW_QUESTBOOK	// [090526: selo] 퀘스트 수락 설명 순서 변경
	AddDescString( CTString("\n"), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD->GetDesc()), pQuestDD->GetColorIntroDesc() );
#else
	AddDescString( _S( 1704, "수행가능 레벨" ), 0xFFFFFFFF );
	CTString strTemp;
	if(pQuestDD->GetNeedMinLevel() == pQuestDD->GetNeedMaxLevel())
	{
		strTemp.PrintF(_S( 1705, "레벨 %d" ), pQuestDD->GetNeedMinLevel());
	}
	else
	{
		strTemp.PrintF(_S( 1706, "레벨 %d ~ 레벨 %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
	}
	AddDescString( strTemp, 0xD0D0FFFF );
#endif
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestPrize(INDEX questIndex)
{
	ASSERT(m_nUIIndex == UI_QUESTBOOK_COMPLETE);
	if(questIndex != m_nSelectedQuestIndex)
		return;

	CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create( questIndex );
	
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );
	AddDescString( CTString(pQuestDD->GetDesc2()), pQuestDD->GetColorIntroDesc() );		
	
	//고정보상
	if(pQuestDD->IsPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetPrizeDesc( 0 ), pQuestDD->GetColorPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorPrizeDesc( i ) );
			AddDescString( CTString("     "), pQuestDD->GetColorPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1))
				, pQuestDD->GetPrizeIndex(i-1));
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetOptionPrizeDesc( 0 ), pQuestDD->GetColorOptionPrizeDesc( 0 ) );
		m_nSelectPrizeFirstIndex = m_lbDescription.GetCurItemCount(0);//선택 보상의 첫번째 index.
		for( INDEX i = 1; i < pQuestDD->GetCountOptionPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			strPrize = CTString("     ") + strPrize;
			AddDescString( strPrize, pQuestDD->GetColorOptionPrizeDesc( i ), TRUE );
			AddDescString( CTString("     "), pQuestDD->GetColorOptionPrizeDesc( i ), TRUE );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1))
				, pQuestDD->GetOptionPrizeIndex(i-1));
		}
		m_nSelectPrizeCount = m_lbDescription.GetCurItemCount(0) - m_nSelectPrizeFirstIndex;
/*
		AddDescString( CTString("\n"), 0 );
		AddDescString( pQuestDD->GetOptionPrizeDesc( 0 ), pQuestDD->GetColorOptionPrizeDesc( 0 ) );
		m_nSelectPrizeFirstIndex = m_lbDescription.GetCurItemCount(0);//선택 보상의 첫번째 index.
		for(INDEX i = 1; i < pQuestDD->GetCountOptionPrizeDesc(); ++i )
			AddDescString( pQuestDD->GetOptionPrizeDesc( i ), pQuestDD->GetColorOptionPrizeDesc( i ), TRUE );
		m_nSelectPrizeCount = m_lbDescription.GetCurItemCount(0) - m_nSelectPrizeFirstIndex;
*/
	}
}

// ----------------------------------------------------------------------------
// Name : InitQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::InitQuestBook()
{
	m_nSelectedQuestIndex	= -1;
	m_nTargetIndex			= -1;
	m_nTargetUIType			= -1;
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();
	m_lbQuestList.Reset();
	m_lbQuestList.ResetAllStrings();
}

// ----------------------------------------------------------------------------
// Name : CloseQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::CloseQuestBook()
{
	CUIQuestBook::UnlockQuest();
	InitQuestBook();
	if(m_nUIIndex != UI_QUESTBOOK_LIST) ClearQuestList();

	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_QUEST );

	_pUIMgr->RearrangeOrder( m_nUIIndex, FALSE );
}

// 퀘스트 목록에 추가
// ----------------------------------------------------------------------------
// Name : AddToQuestList()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuestBook::AddToQuestList( int iQuestIndex, BOOL bComplete )
{
	if( m_vectorCompleteQuestList.size() + m_vectorProceedQuestList.size() >= MAX_QUEST )
	{	
		// [090527: selo] 메시지 박스 추가
		CUIMsgBox_Info	MsgBoxInfo;		
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
		MsgBoxInfo.AddString( _S(4417, "퀘스트 최대 수량을 초과하여 수락할 수 없습니다.") );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		return FALSE;
	}

	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();
	TempQuest.iQuestType	= qdd.GetQuestCategory();
	TempQuest.iQuestScale	= qdd.GetQuestPartyScale();
	
	// [090603: selo] 카테고리별로 구분하는 리스트에 추가
	if( TempQuest.iQuestType >= QCATEGORY_NORMAL && TempQuest.iQuestType <= QCATEGORY_NIGHTSHADOW )
	{
		// 해당 카테고리에 없으면 추가
		if(m_vectorCategoryQuestList[TempQuest.iQuestType].end()
			== std::find_if(m_vectorCategoryQuestList[TempQuest.iQuestType].begin(), m_vectorCategoryQuestList[TempQuest.iQuestType].end(), FindQuest(TempQuest) ) )
		{
			m_vectorCategoryQuestList[TempQuest.iQuestType].push_back(TempQuest);
		}
	}


	if( bComplete )
	{
		if(m_vectorCompleteQuestList.end()
		!= std::find_if(m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest(TempQuest)))
			return FALSE;

		m_vectorCompleteQuestList.push_back( TempQuest );
		return TRUE;
	}
	else
	{
		if(m_vectorProceedQuestList.end()
		!= std::find_if(m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest(TempQuest)))
			return FALSE;

		m_vectorProceedQuestList.push_back( TempQuest );
		return TRUE;
	}
	return FALSE;
}

// 퀘스트 목록에서 제거
// ----------------------------------------------------------------------------
// Name : DelFromQuestList()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuestBook::DelFromQuestList( int iQuestIndex, BOOL bComplete )
{
	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	
	// [090602: selo] : 전체화면에 표시하기 위해 체크되어 있는 퀘스트를 제거한다.
	RemoveSelectedQuest(iQuestIndex);


	// [090603: selo] : 카테고리별로 구분하는 리스트에서 지우기
	for( int i = 0; i < QCATEGORY_END; ++i )
	{
		std::vector<sQuestInfo>::iterator it = std::find_if( m_vectorCategoryQuestList[i].begin(), m_vectorCategoryQuestList[i].end(), FindQuest( TempQuest ) );
		if( it != m_vectorCategoryQuestList[i].end() )
		{
			m_vectorCategoryQuestList[i].erase( it );
			break;
		}
	}
	
	// [090609: selo] : 불필요하게 assert가 걸리게 되는 부분을 막기 위해
	//					진행중과 완료인 것중에 있기만 하면 삭제 하도록 하였음.
	std::vector<sQuestInfo>::iterator it = std::find_if( m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest( TempQuest ) );
	if( it != m_vectorCompleteQuestList.end() )
	{
		m_vectorCompleteQuestList.erase( it );
		return TRUE;
	}
	
	it = std::find_if( m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest( TempQuest ) );
	if( it != m_vectorProceedQuestList.end() )
	{
		m_vectorProceedQuestList.erase( it );
		return TRUE;
	}
	
	
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : ClearQuestList()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ClearQuestList()
{
	m_bProceedListExtended = TRUE;
	if(!m_vectorCompleteQuestList.empty())
	{
		m_vectorCompleteQuestList.clear();
	}
	m_bCompleteListExtended = TRUE;
	if(!m_vectorProceedQuestList.empty())
	{
		m_vectorProceedQuestList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestList()
// Desc : 서버에서 메세지를 다 받은 후 갱신할때...
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestList()
{
#ifdef NEW_QUESTBOOK
	RefreshQuestListNew();
	return;
#endif

	CTString strTemp;
	ASSERT(m_nUIIndex == UI_QUESTBOOK_LIST);
	m_lbQuestList.ResetAllStrings();
	m_lbQuestList.Reset();
	m_lbQuestList.ChangeCurrentState(m_bProceedListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1707, "진행중인 퀘스트 (%d)" ), m_vectorProceedQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	std::vector<sQuestInfo>::iterator it = m_vectorProceedQuestList.begin();
	std::vector<sQuestInfo>::iterator itend = m_vectorProceedQuestList.end();
	for( ; it != itend; ++it )
	{
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHILD);
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, (*it).strQuestTitle, 0xF2F2F2FF );
	}

	m_lbQuestList.ChangeCurrentState(m_bCompleteListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1708, "완료된 퀘스트 (%d)" ), m_vectorCompleteQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	it = m_vectorCompleteQuestList.begin();
	itend = m_vectorCompleteQuestList.end();
	for( ; it != itend; ++it )
	{
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHILD);
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, (*it).strQuestTitle, 0xF2F2F2FF );
	}
}

// ----------------------------------------------------------------------------
// Name : AddDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::AddDescString( CTString &strMessage, const COLOR colDesc, BOOL bSelectable )
{
	// Get length of string
	INDEX	nLength = strMessage.Length();
	if( nLength == 0 )
		return;
	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strMessage);
		INDEX	nChatMax= (_iMaxDescStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#ifdef NEW_QUESTBOOK
		if( UI_QUESTBOOK_LIST == m_nUIIndex )
			nChatMax =  (_iMaxDescStringCharNew-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#endif

		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{ 
			// Check line character
			for( int iPos = 0; iPos < nLength; ++iPos )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strMessage, colDesc, bSelectable );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strMessage.Split( iPos, strTemp2, strTemp );
				
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable ); 

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddDescString( strTemp, colDesc, bSelectable );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxDescStringChar;
#ifdef NEW_QUESTBOOK
			if( UI_QUESTBOOK_LIST == m_nUIIndex )
				nSplitPos = _iMaxDescStringCharNew;
#endif
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strMessage,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; ++iPos )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strMessage.Split( iPos, strTemp2, strTemp );
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; ++iPos )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddDescString( strTemp, colDesc, bSelectable );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strMessage.Split( iPos, strTemp2, strTemp );
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddDescString( strTemp, colDesc, bSelectable );
			}

		}
		
	} else {
	
		// If length of string is less than max char
		int iTempMaxDescStringChar = _iMaxDescStringChar;
#ifdef NEW_QUESTBOOK
		if( UI_QUESTBOOK_LIST == m_nUIIndex )
			iTempMaxDescStringChar = _iMaxDescStringCharNew;
#endif
		if( nLength <= iTempMaxDescStringChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; ++iPos )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strMessage, colDesc, bSelectable );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strMessage.Split( iPos, strTemp2, strTemp );
				
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable ); 

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddDescString( strTemp, colDesc, bSelectable );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxDescStringChar;
#ifdef NEW_QUESTBOOK
			if( UI_QUESTBOOK_LIST == m_nUIIndex )
				nSplitPos = _iMaxDescStringCharNew;
#endif
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; ++iPos )
			{
				if( strMessage[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; ++iPos )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;

#ifdef LINE_CHANGE_BY_WORD
				if( strMessage[nSplitPos] != ' ' )
				{
					for(int i=iPos; i>=0; --i)
					{
						if( strMessage[i] == ' ' )
						{
							iPos =i;
							break;
						}
					}
				}
#endif

				strMessage.Split( iPos, strTemp2, strTemp );
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; ++iPos )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddDescString( strTemp, colDesc, bSelectable );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strMessage.Split( iPos, strTemp2, strTemp );
				m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
				m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddDescString( strTemp, colDesc, bSelectable );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::Render()
{
#ifdef NEW_QUESTBOOK
	//if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{
		RenderQuestListNew();
		return;
	}
#endif

	// Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	// Background
	// Top
	nY = m_nPosY + QUESTBOOK_TOP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	// Desc middle
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, m_nPosY + QUESTBOOK_HEIGHT - 40,
										m_rtMiddleScrollL.U0, m_rtMiddleScrollL.V0,
										m_rtMiddleScrollL.U1, m_rtMiddleScrollL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, m_nPosY + QUESTBOOK_HEIGHT - 40,
										m_rtMiddleScrollM.U0, m_rtMiddleScrollM.V0,
										m_rtMiddleScrollM.U1, m_rtMiddleScrollM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, m_nPosY + QUESTBOOK_HEIGHT - 40,
										m_rtMiddleScrollR.U0, m_rtMiddleScrollR.V0,
										m_rtMiddleScrollR.U1, m_rtMiddleScrollR.V1,
										0xFFFFFFFF );
	
	// Gap middle	
	nY = m_nPosY + QUESTBOOK_HEIGHT - 40;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + 33,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + 33,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + 33,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );	

	// Bottom	
	nY = m_nPosY + QUESTBOOK_HEIGHT - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, m_nPosY + QUESTBOOK_HEIGHT,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, m_nPosY + QUESTBOOK_HEIGHT,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, m_nPosY + QUESTBOOK_HEIGHT,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );	

	// Close box
	m_btnClose.Render();
	if( m_nUIIndex == UI_QUESTBOOK_LIST )
	{	
		//RenderGroup();

		//_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
		m_btnCancel.Render();					// 닫기
		m_lbQuestList.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
	{
		m_btnGiveUp.Render();					// 퀘스트 포기
		m_btnCancel.Render();					// 닫기
		// List box
		m_lbDescription.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
	{
		m_btnOK.Render();						// 확인	
#ifdef  NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 수정
		m_btnPrev.Render();						// 이전
#endif
		// List box
		m_lbDescription.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_NEW )
	{		
#ifndef	NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 수정
		m_btnAccept.Render();					// 수락
		m_btnReserve.Render();					// 보류	
		m_btnDeny.Render();						// 거절
#else
		m_btnAccept.Render();					// 수락
		m_btnPrev.Render();						// 이전
		m_btnCancel.Render();					// 닫기
#endif
		// List box
		m_lbDescription.Render();
	}	

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Title
	_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + QUESTBOOK_TITLE_TEXT_OFFSETX,
										m_nPosY + QUESTBOOK_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );


	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : AcceptQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::AcceptQuest()				// 퀘스트 수락
{
	if(m_nSelectedQuestIndex == -1) return;
	
	if( !m_pQuestItem )
	{
		this->SendQuestStart(m_nSelectedQuestIndex);
	}
	else
	{
		_pNetwork->SendItemUse(m_pQuestItem->Item_Tab, m_pQuestItem->Item_Row, m_pQuestItem->Item_Col, m_pQuestItem->Item_UniIndex, m_nSelectedQuestIndex);		
	}
}

// ----------------------------------------------------------------------------
// Name : ReserveQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ReserveQuest()				// 퀘스트 보류
{
	CQuestSystem::Instance().DeleteCurrentRequest();	
	for(int i=0; i<m_vectorProceedQuestList.size(); ++i)
	{
		if(m_nSelectedQuestIndex == m_vectorProceedQuestList[i].iQuestIndex)
		{
			if(m_vectorProceedQuestList.size() != i+1)
				m_iNextQuestIndex = m_vectorProceedQuestList[i+1].iQuestIndex;
			else if(i == 0)
				m_iNextQuestIndex = m_vectorProceedQuestList[i-1].iQuestIndex;
			else 
				m_iNextQuestIndex = -1;
		}
	}

	CloseQuestBook();
}

// ----------------------------------------------------------------------------
// Name : DenyQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::DenyQuest()				// 퀘스트 거절
{
	if(m_nSelectedQuestIndex == -1) return;

	_pUIMgr->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	
	MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, m_nUIIndex, MSGCMD_QUEST_GIVEUP);
	CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(m_nSelectedQuestIndex));
	if(QuestDD.GetQuestType2() == QTYPE_REPEAT_ONCE)
	{
		strMessage.PrintF( _S( 1709, "이 퀘스트를 거절하면 다시는 이 퀘스트를 받을 수 없습니다. 정말로 거절하시겠습니까?" ) );
	}
	else if(QuestDD.GetQuestType2() == QTYPE_REPEAT_UNLIMITED)
	{
		strMessage.PrintF( _S( 1710, "정말 이 퀘스트를 거절하시겠습니까?" ) );
	}
	// [090616: selo] 일일 퀘스트 거절
	else if(QuestDD.GetQuestType2() == QTYPE_REPEAT_DAY)
	{
		strMessage.PrintF( _S( 1710, "정말 이 퀘스트를 거절하시겠습니까?" ) );
	}
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : GiveUpQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::GiveUpQuest()				// 퀘스트 포기
{
	if(m_nSelectedQuestIndex == -1) return;

	if( !m_pQuestItem )
	{
		this->SendQuestCancel(m_nSelectedQuestIndex);
	}
	else
	{
		// 아이템을 통해서 퀘스트를 얻으려 할 경우
		_pNetwork->SendItemUse( m_pQuestItem->Item_Tab, m_pQuestItem->Item_Row, m_pQuestItem->Item_Col, m_pQuestItem->Item_UniIndex, -1 );
		CloseQuestBook();
	}
}

// ----------------------------------------------------------------------------
// Name : PrizeQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::PrizeQuest()			// 퀘스트 완료
{
	CQuestDynamicData *pDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(m_nSelectedQuestIndex);
	if(pDD == NULL)
	{
		ASSERT(0 && "Something wrong");
		return;
	}

	INDEX iItemIndex = -1;
	INDEX iItemPlus = 0;
	if(pDD->IsOptionPrizeExist())
	{
		INDEX iOptionPrizeIndex = m_lbDescription.GetCurSel() - m_nSelectPrizeFirstIndex;
		if(m_lbDescription.GetCurSel() == -1
		|| iOptionPrizeIndex >= m_nSelectPrizeCount
		|| iOptionPrizeIndex < 0)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_COMPLETE, MSGCMD_QUEST_NOTIFY );
			CTString strMessage;
			strMessage.PrintF( _S( 1711, "받고 싶은 보상을 선택하여 주십시오." ) );
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			m_lbDescription.SetCurSel(-1);
			return;
		}

		iOptionPrizeIndex = iOptionPrizeIndex >> 1;
		iItemIndex = pDD->GetOptionPrizeIndex(iOptionPrizeIndex);
		if(iItemIndex == -1)
		{
			ASSERT(0 && "Something wrong");
			return;
		}
		iItemPlus = pDD->GetOptionPrizePlus(iOptionPrizeIndex);
	}
	this->SendQuestPrize(m_nSelectedQuestIndex, m_nTargetIndex, iItemIndex, iItemPlus);
}

// ----------------------------------------------------------------------------
// [5/26/2009 selo]
// Name : ReturnToNPCMenu()
// Desc : NPC 메뉴로 돌아간다
// ----------------------------------------------------------------------------
void CUIQuestBook::ReturnToNPCMenu()
{
	int nNpcIndex = -1;
	int nTargetUIType = -1;
	
	if(_pUIMgr->GetQuestBookComplete()->GetCompleteQuestCount())
	{
		nNpcIndex = _pUIMgr->GetQuestBookComplete()->m_nPrevNPCIndex;
		nTargetUIType = _pUIMgr->GetQuestBookComplete()->m_nPrevNPCUIType;
	}
	else if(_pUIMgr->GetQuestBookNew()->GetProceedQuestCount())
	{
		nNpcIndex = _pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex;
		nTargetUIType = _pUIMgr->GetQuestBookNew()->m_nPrevNPCUIType;
	}	
	
	// 일반적인 퀘스트 수행 이외의 예외 상황의 경우 처리
	if( -1 == nNpcIndex )
	{
		nNpcIndex = _pUIMgr->GetQuest()->GetNpcIndex();
		nTargetUIType = UI_QUEST;
	}

	// 던전 마스터의 경우 무시하게 한다
	if( 71 == nNpcIndex )
		return;

	CloseQuestBook();

	// 이전으로 돌아갈 NPC 상대가 없다면 퀘스트 재요청을 하지 않는다.
	if( -1 == nNpcIndex )
		return;

	m_nTargetIndex = nNpcIndex;
	_pUIMgr->GetQuestBookList()->m_nTargetIndex = m_nTargetIndex;

	RequestQuest( nNpcIndex, nTargetUIType, m_fNpcX, m_fNpcZ );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_QUEST_GIVEUP:		// 퀘스트 포기
		if(bOK)
		{
			GiveUpQuest();
		}
		m_bLockQuestList = FALSE;
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuestBook::MouseMessage( MSG *pMsg )
{
#ifdef NEW_QUESTBOOK
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{
		return MsgProcQuestListNew(pMsg);		
	}
#endif

	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box
			else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
			{						
				if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
			{
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else if( m_nUIIndex == UI_QUESTBOOK_NEW )
			{				
				if( m_btnAccept.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnReserve.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnDeny.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;			
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if(m_lbDescription.GetCurSel() != -1)
					{
						int iOptionPrizeSelected = m_lbDescription.GetCurSel() - m_nSelectPrizeFirstIndex;
						m_lbDescription.SetCurSel(iOptionPrizeSelected - (iOptionPrizeSelected & 0x00000001) + m_nSelectPrizeFirstIndex);
					}
				}
				// Quest List Box
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// 선택된 퀘스트.
					int	iSelQuest = m_lbQuestList.GetCurSel();
					if( iSelQuest != -1 )
					{
						// 진행중인 퀘스트 중의 하나 선택.
						if( iSelQuest >= 1 && iSelQuest <= m_vectorProceedQuestList.size() + 1 )
						{
							_pUIMgr->GetQuestBookContent()->OpenQuestBook( m_vectorProceedQuestList[iSelQuest - 1].iQuestIndex );
							return WMSG_SUCCESS;
						}
						else if( iSelQuest >= ( m_vectorProceedQuestList.size() + 2 ) && iSelQuest < ( m_vectorProceedQuestList.size() + 2 ) + m_vectorCompleteQuestList.size() )
						{
							_pUIMgr->GetQuestBookContent()->OpenQuestBook( m_vectorCompleteQuestList[iSelQuest - m_vectorProceedQuestList.size() - 2].iQuestIndex );
							return WMSG_SUCCESS;
						}
					}
					int groupNameLine = 0;
					for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
					{
						if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
						|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
						{
							if(groupNameLine == 0)//proceed
							{
								m_bProceedListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
								++groupNameLine;
							}
							else if(groupNameLine == 1)//complete
							{
								m_bCompleteListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
								++groupNameLine;
								break;
							}
						}
					}
				}
				else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
				{
					if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
				}
				else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
				{
					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
#ifdef NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 변경
					if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
#endif
				}
				else if( m_nUIIndex == UI_QUESTBOOK_NEW )
				{				
					if( m_btnAccept.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
#ifndef	NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 변경
					else if( m_btnReserve.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					else if( m_btnDeny.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
#else
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
#endif
				}

				_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				m_bTitleBarClick = FALSE;
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
				{
					// 퀘스트 포기
					if( m_nSelectedQuestIndex != -1 && ( wmsgResult = m_btnGiveUp.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{						
						_pUIMgr->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;	
						MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, m_nUIIndex, MSGCMD_QUEST_GIVEUP);	
						strMessage.PrintF( _S( 504, "정말 이 퀘스트를 취소하시겠습니까?" ));	
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
						return WMSG_SUCCESS;
					}
					if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseQuestBook();
						return WMSG_SUCCESS;				
					}
				}
				else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
				{
					// 퀘스트 완료
					if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						PrizeQuest();
						return WMSG_SUCCESS;				
					}
#ifdef NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 변경
					// NPC 메뉴 리스트로 돌아감
					else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ReturnToNPCMenu();
						return WMSG_SUCCESS;				
					}
#endif
				}
				else if( m_nUIIndex == UI_QUESTBOOK_NEW )
				{
					// 퀘스트 수락
					if( ( wmsgResult = m_btnAccept.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							AcceptQuest();
						return WMSG_SUCCESS;				
					}
#ifndef	NEW_QUESTBOOK	// [090526: selo] 확장팩 퀘스트 변경
					// 퀘스트 보류
					else if( ( wmsgResult = m_btnReserve.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ReserveQuest();
						return WMSG_SUCCESS;				
					}
					// 퀘스트 거절
					else if( ( wmsgResult = m_btnDeny.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							DenyQuest();
						return WMSG_SUCCESS;				
					}
#else
					// NPC 메뉴 리스트로 돌아감
					else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ReturnToNPCMenu();
						return WMSG_SUCCESS;				
					}
					// 퀘스트 닫기
					else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseQuestBook();
						return WMSG_SUCCESS;				
					}					
#endif
				}
				else if( m_nUIIndex == UI_QUESTBOOK_LIST )
				{
					if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseQuestBook();
						return WMSG_SUCCESS;				
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : RequestQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RequestQuest( int nNpcIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ )
{
/*
	// too many request is deny
	if( _pTimer->GetLerpedCurrentTick() - m_fLastRequestTime < 0.2f )
	{
		return;
	}
	m_fLastRequestTime = _pTimer->GetLerpedCurrentTick();
*/

	// If quest is aleardy requested
	if( m_bLockRequestQuest )
		return;

	// If requesting quest is exist
	if( CQuestSystem::Instance().GetCurrentRequest() )
		return;

	switch(sbUIType)
	{
	case UI_SHOP:
		{
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
				return;
		}
		break;
	case UI_REFINE:
		{
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_REFINE_REQ ))
				return;
		}
		break;
	case UI_SKILLLEARN:
		{
			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_SKILLLEARN_REQ ) )
				return;
			// Special Skill
			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_SSKILLLEARN_REQ ) )
				return;
		}
		break;
	case UI_PROCESSNPC:
		{
			if (_pUIMgr->DoesMessageBoxLExist( MSGLCMD_PROCESSNPC_REQ ))
				return;
		}
		break;
	case UI_REMISSION:
		{			
			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_REMISSION_REQ ) || IsVisible() )
				return;
		}
		break;
	case UI_QUEST:
		{
			if (_pUIMgr->IsUIVisible(sbUIType))
				return;

			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_QUEST_REQ ) || IsVisible() )
				return;
		}
		break;
	case UI_WAREHOUSE:
		{			
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_WAREHOUSE_REQ ))
				return;
		}
		break;
	case UI_GUILD:
		{
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_GUILD_REQ ))
				return;
		}
		break;
	case UI_CHANGEWEAPON:
		{
			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_CHANGEWEAPON_REQ ))
				return;
		}
		break;
	case UI_INITJOB:
		{
			if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_INITJOB_REQ ))
				return;
		}
		break;
	case UI_SIEGE_WARFARE:
		{
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_SIEGE_WARFARE ))
				return;
		}
		break;
	case UI_PETTRAINING:	// 애완동물 조련사.
		{
			if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_PETTRAINING_REQ ))
				return;
		}
		break;
		// ==! TODO[selo] 제작 관련 처리 주석 푼다
	/*
	case UI_PRODUCTNPC:
		{
			if (_pUIMgr->DoesMessageBoxLExist(MSGLCMD_PRODUCETNPC_VOUCHER))
			{
				return;
			}

			if (_pUIMgr->DoesMessageBoxLExist(MSGLCMD_PRODUCETNPC_LEARN))
			{
				return;
			}
		}
	*/
	}

	//INDEX	iPrizeQuestIndex = CQuestSystem::Instance().GetVectorIndexByPrizeNPCIndex( nNpcIndex );
	CMobData& MD = _pNetwork->GetMobData(nNpcIndex);
	m_nTargetIndex	= nNpcIndex;
	m_nTargetUIType = sbUIType;	

	_pUIMgr->GetQuestBookComplete()->m_nTargetIndex = nNpcIndex;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	SendQuestReq( nNpcIndex );
	
	_pUIMgr->GetQuestBookNew()->ClearQuestList();
	_pUIMgr->GetQuestBookComplete()->ClearQuestList();
}


// ----------------------------------------------------------------------------
// Name : OpenWindow()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::OpenWindow( BOOL bHasQuestList )
{
	int iMobIndex = m_nTargetIndex;
	// [090527: selo] 몹인덱스를 찾지 못하는 경우는 퀘스트 북에서 이전을 눌렀을때 발생한다.
	// 그리하여 PrevNPCIndex 로 다시 얻어온다.
	if( -1 == iMobIndex )
		iMobIndex = m_nPrevNPCIndex;

	switch( m_nTargetUIType )
	{
	case UI_SHOP:
		{
			_pUIMgr->GetShop()->OpenShop( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;		
	case UI_REFINE:
		{
			_pUIMgr->GetRefine()->OpenRefine( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_SKILLLEARN:
		{
			_pUIMgr->GetSkillLearn()->OpenSkillLearn( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_PROCESSNPC:
		{
			_pUIMgr->GetProcessNPC()->OpenProcessNPC( );
		}
		break;
	case UI_REMISSION:
		{			
			_pUIMgr->GetRemission()->OpenRemission( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_QUEST:
		{
			_pUIMgr->GetQuest()->OpenQuest( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_WAREHOUSE:
		{			
			_pUIMgr->GetWareHouse()->CheckHasPassWord( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_GUILD:
		{
			_pUIMgr->GetGuild()->OpenGuild( iMobIndex, bHasQuestList, _pNetwork->MyCharacterInfo.lGuildPosition, _pNetwork->MyCharacterInfo.lGuildLevel );
		}
		break;
	case UI_CHANGEWEAPON:
		{
			_pUIMgr->GetChangeWeapon()->OpenChangeWeapon( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_INITJOB:
		{
			_pUIMgr->GetInitJob()->OpenInitJob( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_SIEGE_WARFARE:
		{
			_pUIMgr->GetSiegeWarfare()->OpenSiegeWarfare();
		}
		break;
	case UI_PETTRAINING:	// 애완동물 조련사.
		{
			_pUIMgr->GetPetTraining()->OpenPetTraining( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
		// ==! TODO[selo] 제작 관련 처리 주석 푼다
	/*
	case UI_PRODUCTNPC:
		{
			_pUIMgr->GetProductNPC()->OpenProductNPC(iMobIndex, m_fNpcX, m_fNpcZ );
		}
	*/

		/*
	case UI_GAMBLE:		// 도박 NPC
		{			
			_pUIMgr->GetGamble()->OpenGamble( m_fNpcX, m_fNpcZ );
		}
		break;
		*/
	}
}

// ----------------------------------------------------------------------------
// Name : SendQuestReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestReq( int nNpcIndex )
{
	ASSERT(nNpcIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_REQUEST, nNpcIndex );
	//m_bLockRequestQuest = TRUE;
}

// ----------------------------------------------------------------------------
// Name : SendQuestCancel()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestCancel( int nQuestIndex )
{
	ASSERT(nQuestIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_GIVEUP, nQuestIndex );
}

// ----------------------------------------------------------------------------
// Name : SendQuestStart()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestStart( int nQuestIndex )
{
	ASSERT(nQuestIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_START, nQuestIndex );
}

// ----------------------------------------------------------------------------
// Name : SendQuestPrize()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestPrize(int iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus)
{
	ASSERT(iQuestIndex != -1 && iNpcIndex != -1);
	_pNetwork->SendQuestPrizeMessage(MSG_QUEST_PRIZE, iQuestIndex, iNpcIndex, iOptionItemIndex, iOptionItemPlus);
}

// ----------------------------------------------------------------------------
// Name : TalkWithNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::TalkWithNPC()
{
	if(_pUIMgr->GetQuestBookComplete()->GetCompleteQuestCount())
	{
		// 2009. 05. 27 김정래
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( _pUIMgr->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			_pUIMgr->GetQuestBookComplete()->m_nPrevNPCIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
			_pUIMgr->GetQuestBookComplete()->m_nPrevNPCUIType = _pUIMgr->GetQuestBookComplete()->m_nTargetUIType;
		}
		else
		{
			_pUIMgr->GetQuestBookList()->m_nTargetIndex = _pUIMgr->GetQuestBookComplete()->m_nPrevNPCIndex;			
		}

		_pUIMgr->GetQuestBookComplete()->OpenQuestBook();
		_pUIMgr->GetQuestBookComplete()->m_nTargetIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
		_pUIMgr->GetQuestBookList()->m_nTargetIndex = -1;
		_pUIMgr->GetQuestBookNew()->m_nTargetIndex = -1;
	}
	else if(_pUIMgr->GetQuestBookNew()->GetProceedQuestCount())
	{
		// 2009. 05. 27 김정래
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( _pUIMgr->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCUIType = _pUIMgr->GetQuestBookNew()->m_nTargetUIType;
		}
		else
		{
			_pUIMgr->GetQuestBookList()->m_nTargetIndex = _pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex;			
		}

		_pUIMgr->GetQuestBookNew()->OpenQuestBook();
		_pUIMgr->GetQuestBookNew()->m_nTargetIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
		_pUIMgr->GetQuestBookList()->m_nTargetIndex = -1;
	}
	else
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_NEW, MSGCMD_QUEST_NOTIFY );
		CTString strMessage;
		strMessage.PrintF( _S( 584, "수행할 수 있는 퀘스트가 없습니다." ) );					
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
	CUIQuestBook::UnlockQuest();
}


// ======================================================
// [5/27/2009 selo]
// Name : GetProceedQuestIndex()
// Desc : 진행 가능한 퀘스트 인덱스를 리턴한다
// ======================================================
INDEX CUIQuestBook::GetProceedQuestIndex(INDEX index)
{
	if( m_vectorProceedQuestList.size() - 1 < index )	
		return -1;

	sQuestInfo info = m_vectorProceedQuestList[index];
	return info.iQuestIndex;
}

// ======================================================
//  [5/27/2009 selo]
// Name : GetCompleteQuestIndex()
// Desc : 보상 처리 할 퀘스트 인덱스를 리턴한다
// ======================================================
INDEX CUIQuestBook::GetCompleteQuestIndex(INDEX index)
{
	if( m_vectorCompleteQuestList.size() - 1 < index )
		return -1;

	sQuestInfo info = m_vectorCompleteQuestList[index];
	return info.iQuestIndex;
}

// =====================================================
//  [5/27/2009 selo]
// Name : AddQuestListToMessageBoxL()
// Desc : 나열 가능한 퀘스트를 메시지 박스에 올린다.
// =====================================================
void CUIQuestBook::AddQuestListToMessageBoxL(const int& iMessageBoxType)
{
	int iQuestIndex = -1;
	CTString strQuestTitle;
	
	for( int iComplete = 0; iComplete < _pUIMgr->GetQuestBookComplete()->GetCompleteQuestCount(); ++iComplete )
	{
		iQuestIndex = _pUIMgr->GetQuestBookComplete()->GetCompleteQuestIndex(iComplete);
		
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();
		
		_pUIMgr->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("A") );
	}
	for( int iNew = 0; iNew < _pUIMgr->GetQuestBookNew()->GetProceedQuestCount(); ++iNew )
	{
		iQuestIndex = _pUIMgr->GetQuestBookNew()->GetProceedQuestIndex(iNew);
		
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();
		
		_pUIMgr->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("Q") );
	}
}

// =====================================================
//  [5/27/2009 selo]
// Name : SelectQuestFromMessageBox()
// Desc : 선택한 퀘스트에 따른 UI 를 띄운다.
// =====================================================
void CUIQuestBook::SelectQuestFromMessageBox(const int& iClassifierIndex)
{
	int iQuestIndex = iClassifierIndex - ciQuestClassifier;
	if( iQuestIndex <= 0 )
		return;

	SBYTE sbType = GetQuestType(iQuestIndex);	
	if( 1 == sbType )	// 수락
	{
		if( _pUIMgr->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCUIType = _pUIMgr->GetQuestBookNew()->m_nTargetUIType;
		}
		else
		{
			_pUIMgr->GetQuestBookList()->m_nTargetIndex = _pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex;			
		}

		_pUIMgr->GetQuestBookNew()->OpenQuestBook(iQuestIndex);
		_pUIMgr->GetQuestBookNew()->m_nTargetIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
		_pUIMgr->GetQuestBookList()->m_nTargetIndex = -1;		
	}
	else if( 2 == sbType ) // 보상
	{
		if( _pUIMgr->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			_pUIMgr->GetQuestBookComplete()->m_nPrevNPCIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
			_pUIMgr->GetQuestBookComplete()->m_nPrevNPCUIType = _pUIMgr->GetQuestBookComplete()->m_nTargetUIType;
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
			_pUIMgr->GetQuestBookNew()->m_nPrevNPCUIType = _pUIMgr->GetQuestBookNew()->m_nTargetUIType;
		}
		else
		{
			_pUIMgr->GetQuestBookList()->m_nTargetIndex = _pUIMgr->GetQuestBookComplete()->m_nPrevNPCIndex;
			/*if( -1 == _pUIMgr->GetQuestBookList()->m_nTargetIndex )
				_pUIMgr->GetQuestBookList()->m_nTargetIndex = _pUIMgr->GetQuestBookNew()->m_nPrevNPCIndex;*/
		}

		_pUIMgr->GetQuestBookComplete()->OpenQuestBook(iQuestIndex);
		_pUIMgr->GetQuestBookComplete()->m_nTargetIndex = _pUIMgr->GetQuestBookList()->m_nTargetIndex;
		_pUIMgr->GetQuestBookList()->m_nTargetIndex = -1;
		_pUIMgr->GetQuestBookNew()->m_nTargetIndex = -1;
	}
}

// =====================================================
//  [5/27/2009 selo]
// Name : GetQuestType()
// Desc : 퀘스트 인덱스를 이용해 퀘스트의 타입을 알아낸다.
// Return : 0 - 실패, 1 - 수락, 2 - 보상
// =====================================================
SBYTE CUIQuestBook::GetQuestType(const INDEX& iQuestIndex)
{
	int iTempIndex = -1;
	
	for( int iComplete = 0; iComplete < _pUIMgr->GetQuestBookComplete()->GetCompleteQuestCount(); ++iComplete )
	{
		iTempIndex = _pUIMgr->GetQuestBookComplete()->GetCompleteQuestIndex(iComplete);
		
		if( iQuestIndex == iTempIndex )
			return 2;
	}
	for( int iNew = 0; iNew < _pUIMgr->GetQuestBookNew()->GetProceedQuestCount(); ++iNew )
	{
		iTempIndex = _pUIMgr->GetQuestBookNew()->GetProceedQuestIndex(iNew);
		
		if( iQuestIndex == iTempIndex )
			return 1;
	}

	return 0;
}

// =====================================================
//  [6/2/2009 selo]
// Name : AddSelectedQuest()
// Desc : 전체 화면에 퀘스트 진행상황을 표시할 퀘스트를
//		  체크 하였을 때 list 에 추가
// =====================================================
void CUIQuestBook::AddSelectedQuest(const int& iQuestIndex)
{
	// 이미 리스트에 있으면 그냥 리턴
	if( std::count(m_listSelectedQuestIndex.begin(), m_listSelectedQuestIndex.end(), iQuestIndex) )
		return;

	m_listSelectedQuestIndex.push_back(iQuestIndex);
}

// =====================================================
//  [6/2/2009 selo]
// Name : RemoveSelectedQuest()
// Desc : 전체 화면에 뿌리고 있는 퀘스트를 체크 해제 할 때
//		  list 에서 제거
// =====================================================
void CUIQuestBook::RemoveSelectedQuest(const int& iQuestIndex)
{
	m_listSelectedQuestIndex.remove(iQuestIndex);
}

// =====================================================
//  [6/2/2009 selo]
// Name : IsSelectedQuest()
// Desc : 해당 인덱스가 선택되어 있는 지 확인
// =====================================================
bool CUIQuestBook::IsSelectedQuest(const int& iQuestIndex)
{
	if( std::count(m_listSelectedQuestIndex.begin(), m_listSelectedQuestIndex.end(), iQuestIndex) )
		return true;	

	return false;
}

// =====================================================
//  [6/3/2009 selo]
// Name : SortQuestListByCondition()
// Desc : 진행 상태에 따른 정렬.
//		  진행 중과 완료로 구분하여 정렬한다.
// =====================================================
void CUIQuestBook::SortQuestListByCondition()
{
	CTString strTemp;
	CQuestDynamicData *pQuestDD = NULL;
	
	m_lbQuestList.ResetAllStrings();
	m_lbQuestList.Reset();

	m_lbQuestList.ChangeCurrentState(m_bProceedListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1707, "진행중인 퀘스트 (%d)" ), m_vectorProceedQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
	
	std::vector<sQuestInfo>::iterator it = m_vectorProceedQuestList.begin();
	std::vector<sQuestInfo>::iterator itend = m_vectorProceedQuestList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");
		
		if( pQuestDD )
		{
			strTitle.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle);
		}
		else
		{
			strTitle = (*it).strQuestTitle;
		}
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
	
	m_lbQuestList.ChangeCurrentState(m_bCompleteListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1708, "완료된 퀘스트 (%d)" ), m_vectorCompleteQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
	
	it = m_vectorCompleteQuestList.begin();
	itend = m_vectorCompleteQuestList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");
		
		if( pQuestDD )
		{
			strTitle.PrintF("[%d] %s (%s)", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle, _S( 429, "완료" ));
		}
		else
		{
			strTitle.PrintF("%s (%s)", (*it).strQuestTitle, _S( 429, "완료" ));
		}
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}

	// [090708: selo] 레이드 퀘스트 관련 추가
	m_lbQuestList.ChangeCurrentState(m_bRaidMessageListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 4429, "레이드 진행 메시지 (%d)" ), m_vectorRaidMessageList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	it = m_vectorRaidMessageList.begin();
	itend = m_vectorRaidMessageList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");		
		
		strTitle.PrintF("%s", (*it).strQuestTitle);
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
}

// =====================================================
//  [6/3/2009 selo]
// Name : SortQuestListByType()
// Desc : 종류에 따른 정렬.
//		  퀘스트의 종류에 따라 구분하여 정렬한다.
// =====================================================
void CUIQuestBook::SortQuestListByType()
{
	CTString strTemp;
	CQuestDynamicData *pQuestDD = NULL;
	
	m_lbQuestList.ResetAllStrings();
	m_lbQuestList.Reset();

	for( int i = 0; i < QCATEGORY_END; ++i )
	{	
		m_lbQuestList.ChangeCurrentState(m_bCategoryListExtended[i] ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
		m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
		
		if( QCATEGORY_NORMAL == i )
		{
			strTemp.PrintF(_S(4418, "일반 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		else if( QCATEGORY_SCENARIO == i )
		{
			strTemp.PrintF(_S(4419, "시나리오 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		else if( QCATEGORY_SUPPLEMENT == i )
		{
			strTemp.PrintF(_S(4420, "외전 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		else if( QCATEGORY_NIGHTSHADOW == i )
		{
			strTemp.PrintF(_S(4421, "나이트 쉐도우 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}		
		m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
		
		std::vector<sQuestInfo>::iterator it = m_vectorCategoryQuestList[i].begin();
		std::vector<sQuestInfo>::iterator itend = m_vectorCategoryQuestList[i].end();
		for( ; it != itend; ++it )
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
			CTString strTitle("");
			
			if( pQuestDD )
			{
				strTitle.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle);

				if( IsCompleteQuest((*it).iQuestIndex) )
					strTitle += _S(4422, " (완료)");
				else if( QSCALE_PARTY == (*it).iQuestScale )
					strTitle += _S(4423, " (파티)");
				else if( QSCALE_EXPEDITION == (*it).iQuestScale )
					strTitle += _S(4424, " (원정대)");
			}
			else
			{
				strTitle = (*it).strQuestTitle;
			}
			
			m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
			
			if( IsSelectedQuest((*it).iQuestIndex) )
				m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
			
			m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
			m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
		}
	}

	// [090708: selo] 레이드 퀘스트 관련 추가
	m_lbQuestList.ChangeCurrentState(m_bRaidMessageListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 4429, "레이드 진행 메시지 (%d)" ), m_vectorRaidMessageList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	std::vector<sQuestInfo>::iterator it = m_vectorRaidMessageList.begin();
	std::vector<sQuestInfo>::iterator itend = m_vectorRaidMessageList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");		
		
		strTitle.PrintF("%s", (*it).strQuestTitle);
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
}

// =====================================================
//  [6/3/2009 selo]
// Name : IsCompleteQuest()
// Desc : 해당 인덱스의 퀘스트가 완료 중인지 판단한다.
// =====================================================
bool CUIQuestBook::IsCompleteQuest(int nQuestIndex)
{
	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= nQuestIndex;

	if(m_vectorCompleteQuestList.end()
		!= std::find_if(m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest(TempQuest)))
		return true;

	return false;
}

// =====================================================
//  [6/9/2009 selo]
// Name : GetTimeAttackRemainTime()
// Desc : 타임어택 퀘스트 중 제일 먼저 수락 받은 퀘스트의 초시간을 반환한다.
//		  
// =====================================================
DOUBLE	CUIQuestBook::GetTimeAttackRemainTime()
{
	DOUBLE dTempTime = -1;
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	
	std::vector<sQuestInfo>::iterator iter = m_vectorProceedQuestList.begin();
	while( iter != m_vectorProceedQuestList.end() )
	{
		if( (*iter).dRemainTime > 0 && (*iter).dOldTime > 0 )
		{	
			(*iter).dRemainTime -= (dCurTime - (*iter).dOldTime);
			(*iter).dOldTime = dCurTime;
			
			if( (*iter).dRemainTime > 0 )
				return (*iter).dRemainTime;
		}

		++iter;
	}

	return -1;
}

// =====================================================
//  [6/9/2009 selo]
// Name : SetQuestRemainTime()
// Desc : 타임어택 퀘스트의 남은 시간을 설정한다.
// =====================================================
void CUIQuestBook::SetQuestRemainTime(int iQuestIndex, int iRemainTime)
{
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();

	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	std::vector<sQuestInfo>::iterator iter = std::find_if(m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest(TempQuest));
	if( iter != m_vectorProceedQuestList.end() )
	{
		(*iter).dRemainTime = (DOUBLE)iRemainTime;
		(*iter).dOldTime = dCurTime;
	}	
}

// =====================================================
//  [7/8/2009 selo]
// Name : AddRaidMessage
// Desc : 레이드 진행 메시지에 추가한다 
// =====================================================
void CUIQuestBook::AddRaidMessage(const int& iQuestIndex)
{
	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();	
	
	if(m_vectorRaidMessageList.end()
		== std::find_if(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), FindQuest(TempQuest) ) )
	{
		m_vectorRaidMessageList.push_back(TempQuest);
	}

	// [090709: selo] 인덱스 작은게 앞으로 가게 정렬한다.
	std::sort(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end());
}

// =====================================================
//  [7/8/2009 selo]
// Name : RemoveRaidMessage
// Desc : 해당 인덱스의 레이드 메시지를 제거한다
// =====================================================
void CUIQuestBook::RemoveRaidMessage(const int& iQuestIndex)
{
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	// 아래의 구문이 이해가 안되는 사람은
	// " remove-erase 합성문 " 으로 구글 검색 해보시길
	m_vectorRaidMessageList.erase(std::remove(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), TempQuest));
}

// =====================================================
//  [7/8/2009 selo]
// Name : RemoveRaidMessageAll
// Desc : 레이드 메시지를 모두 제거한다
// =====================================================
void CUIQuestBook::RemoveRaidMessageAll(void)
{
	m_vectorRaidMessageList.clear();
}

// =====================================================
//  [7/8/2009 selo]
// Name : RefreshQuestContentByRaid
// Desc : 퀘스트 리스트에서 레이드 퀘스트를 선택하면
//		  RefreshQuestContent() 함수 대신에 불린다
// =====================================================
void CUIQuestBook::RefreshQuestContentByRaid(const int& iQuestIndex)
{
	m_btnGiveUp.SetEnable(FALSE);

	if( iQuestIndex != m_nSelectedQuestIndex )
		return;
	
	int oldSbPos = m_lbDescription.GetScrollBarPos();
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();

	CQuestDynamicData pQuestDD(CQuestSystem::Instance().GetStaticData(iQuestIndex));	

	AddDescString( pQuestDD.GetTitleDesc( 0 ), pQuestDD.GetColorTitleDesc( 0 ) );
	AddDescString( CTString("\n"), 0xFFFFFFFF );

	AddDescString( _S(3852, "내용" ), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD.GetDesc()), pQuestDD.GetColorIntroDesc() );		
	AddDescString( CTString("\n"), 0xFFFFFFFF );		

	AddDescString( CTString(pQuestDD.GetDesc3()), 0x0070C0FF );

	m_lbDescription.SetScrollBarPos(oldSbPos);
}

// =====================================================
//  [7/8/2009 selo]
// Name : isRaidMessage
// Desc : 해당 퀘스트가 레이드 메시지인지 확인
// =====================================================
bool CUIQuestBook::isRaidMessage(const int& iQuestIndex)
{
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	if( std::count(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), TempQuest) )
		return true;	

	return false;	
}

// =====================================================
//  [5/29/2009 selo]
// Name : CreateQuestListNew()
// Desc : 퀘스트 리스트를 확장하기 위한 새로운 생성 함수
// =====================================================
void CUIQuestBook::CreateQuestListNew( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	// ! Notice : nWidth, nHeight 는 사용하지 않고 
	// 미리 설정한 Width와 Height를 사용 하기로한다.

	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( iQuestListWidth, iQuestListHeight + 30 );
	
	_iMaxDescStringCharNew = 182 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxSelStringCharNew = 191 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxDescStringChar = _iMaxDescStringCharNew;
	_iMaxSelStringChar = _iMaxSelStringCharNew;
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;
	

	// Region of each part
	m_rcTitle.SetRect( 0, 0, iQuestListWidth, iQuestListTitleHeight );	

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( strQuestListTexFileName );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_rtTopL.SetUV( 0, 0, iQuestListWidth, iQuestListHeight, fTexWidth, fTexHeight );
	else
		m_rtTopL.SetUV( 0, 386, 251, 386+332, fTexWidth, fTexHeight );


	// Sort Combobox	
	m_cmbSort.Create( this, 9, 306, 155, 17, 155-20, 1, 15, 15,
						8, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbSort.SetBackUV( 9, 306, 164, 323, fTexWidth, fTexHeight );
	m_cmbSort.SetDownBtnUV( 144, 307, 159, 322, fTexWidth, fTexHeight );
	m_cmbSort.SetUpBtnUV( 144, 307, 159, 322, fTexWidth, fTexHeight );	
	m_cmbSort.SetDropListUV( 0, 363, 155, 378, fTexWidth, fTexHeight );	
	
	m_cmbSort.AddString(_S(4425, "진행 상태에 따른 정렬"));
	m_cmbSort.AddString(_S(4426, "종류에 따른 정렬"));
	m_cmbSort.SetCurSel(0);

	CTextureData* pTexture = CreateTexture( strCommonBtnTexFileName );
	fTexWidth = pTexture->GetPixWidth();
	fTexHeight = pTexture->GetPixHeight();

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 165, 304, 78, 22 );
	m_btnOK.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	// Deny button
	m_btnDeny.Create( this, _S( 1696, "거절" ), 165, 304, 78, 22 );			
	m_btnDeny.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnDeny.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_DISABLE );
		
	
	// Accept button	
	m_btnAccept.Create( this, _S( 1698, "수락" ), 7, 304, 78, 22 );		
	m_btnAccept.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnAccept.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	// Reserve button
	m_btnReserve.Create( this, _S( 1699, "보류" ), 86, 304, 78, 22 );		
	m_btnReserve.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnReserve.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Prev button
	m_btnPrev.Create( this, _S( 2681, "이전" ), 86, 304, 78, 22 );		
	m_btnPrev.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Close button
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_btnClose.Create( this, CTString( "" ), 452, 4, 16, 16 );
	else
		m_btnClose.Create( this, CTString( "" ), 227, 4, 16, 16 );		
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	

	
	// Cancel button
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_btnCancel.Create( this, _S( 870, "닫기" ), 388, 304, 78, 22 );
	else
		m_btnCancel.Create( this, _S( 870, "닫기" ), 165, 304, 78, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
			
	// Give Up Button
	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 88, 21);
	rcRight.SetRect(88, 0, 102, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);
	
	m_btnGiveUp.Create( this, _S( 1697, "퀘스트 포기" ), 280, 304, 102, 22 );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnGiveUp.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnGiveUp.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnGiveUp.SetNewType(TRUE);


	// Quest List box	
	m_lbQuestList.Create( this, 14, 46, 204, 246, _nMsgBoxLineHeight, 12, 8, 2, TRUE );
	m_lbQuestList.CreateScroll( TRUE, 0, 0, 10, 246, 10, 10, 0, 0, 10 );
	m_lbQuestList.SetSelBar( 204, _pUIFontTexMgr->GetLineHeight(), 145, 138, 240, 172, fTexWidth, fTexHeight );
	m_lbQuestList.SetOverColor( 0xF8E1B5FF );
	m_lbQuestList.SetSelectColor( 0xF8E1B5FF );
	m_lbQuestList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbQuestList.SetScrollUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbQuestList.SetScrollDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbQuestList.SetScrollBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	// pop button	
	m_lbQuestList.SetPopBtnSpace(1, 0);
	m_lbQuestList.SetPopBtnSize(15, 15);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE, 241, 57, 241+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK, 227, 57, 227+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE_DISABLE, 241, 57, 241+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK_DISABLE, 227, 57, 227+15, 57+15, fTexWidth, fTexHeight);	
	m_lbQuestList.SetCheckBtnSpace(1, 0);
	m_lbQuestList.SetCheckBtnSize(13, 13);
	m_lbQuestList.SetCheckBtnUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );	
	

	
	// Description List box
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_lbDescription.Create( this, 244, 46, 204, 246, _nMsgBoxLineHeight, 12, 8, 1, TRUE );
	else
		m_lbDescription.Create( this, 14, 46, 210, 246, _nMsgBoxLineHeight, 12, 8, 1, TRUE );
	m_lbDescription.CreateScroll( TRUE, 0, 0, 10, 246, 10, 10, 0, 0, 10 );
	// Up button
	m_lbDescription.SetScrollUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbDescription.SetScrollDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbDescription.SetScrollBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	m_lbDescription.SetSelectColor( 0xF8E1B5FF );
}

// =====================================================
//  [5/29/2009 selo]
// Name : RefreshQuestListNew()
// Desc : 기존의 RefreshQuestList() 함수를 후킹하여
//		  이 곳에서 작업이 이루어지게 한다.
// =====================================================
void CUIQuestBook::RefreshQuestListNew()
{
	ASSERT(m_nUIIndex == UI_QUESTBOOK_LIST);
	
	m_nSelectedQuestIndex = -1;

	if( 0 == m_nSortType )		// 진행상태에 따른 정렬 일 때
	{	
		SortQuestListByCondition();		
	}
	else
	{
		SortQuestListByType();
	}
}

// =====================================================
//  [5/29/2009 selo]
// Name : RenderQuestListNew()
// Desc : 퀘스트 리스트 확장을 위한 랜더 함수
// =====================================================
void CUIQuestBook::RenderQuestListNew()
{
	// Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	// Background
	// Top
	nY = m_nPosY + QUESTBOOK_TOP_HEIGHT;
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{	
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + iQuestListWidth, m_nPosY + iQuestListHeight,
											m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
											0xFFFFFFFF );
		m_cmbSort.Render();
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 251, m_nPosY + 332,
											m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
											0xFFFFFFFF );
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	CTextureData* pTexture = CreateTexture( strCommonBtnTexFileName );
	_pUIMgr->GetDrawPort()->InitTextureData( pTexture );
	
	// Close box
	m_btnClose.Render();
	if( m_nUIIndex == UI_QUESTBOOK_LIST )
	{	
		m_btnGiveUp.Render();
		m_btnCancel.Render();					// 닫기
		m_lbQuestList.Render();
		m_lbDescription.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_CONTENT )
	{
		m_btnGiveUp.Render();					// 퀘스트 포기
		m_btnCancel.Render();					// 닫기
		// List box
		m_lbDescription.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
	{
		m_btnOK.Render();						// 확인
		m_btnPrev.Render();						// 이전
		// List box
		m_lbDescription.Render();
	}
	else if( m_nUIIndex == UI_QUESTBOOK_NEW )
	{		
		m_btnAccept.Render();					// 수락
		m_btnPrev.Render();						// 이전
		m_btnCancel.Render();					// 닫기
		// List box
		m_lbDescription.Render();
	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Title
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + 209, m_nPosY + 15, 0xDED9A0FF );
	else if( UI_QUESTBOOK_NEW == m_nUIIndex )
		_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + 106, m_nPosY + 15, 0xDED9A0FF );
	else
		_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + 84, m_nPosY + 15, 0xDED9A0FF );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// =====================================================
//  [5/29/2009 selo]
// Name : MsgProcQuestListNew()
// Desc : 퀘스트 리스트 확장 구현을 위해 마우스 메시지를 후킹한다.
// =====================================================
WMSG_RESULT CUIQuestBook::MsgProcQuestListNew( MSG *pMsg )
{
	if( TRUE == m_bLockQuestList )
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// buttons
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			// List box
			else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;	
			
			else if( m_cmbSort.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// Sort Combobox				
				else if( ( wmsgResult = m_cmbSort.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nSortType = m_cmbSort.GetCurSel();
						RefreshQuestListNew();
					}
				}				
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// Quest List Box
				else if( ( wmsgResult = m_lbQuestList.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// 진행 상태에 따른 정렬이면
					if( 0 == m_nSortType )
					{
						// 선택된 퀘스트.						
						int	iSelQuest = m_lbQuestList.GetCurSel();
						if( iSelQuest != -1 )
						{
							// 진행중인 퀘스트 중의 하나 선택.
							if( iSelQuest >= 1 && iSelQuest <= m_vectorProceedQuestList.size() + 1 )
							{
								m_nSelectedQuestIndex = m_vectorProceedQuestList[iSelQuest - 1].iQuestIndex;
								RefreshQuestContent( m_nSelectedQuestIndex );								
							}
							else if( iSelQuest >= ( m_vectorProceedQuestList.size() + 2 ) && iSelQuest < ( m_vectorProceedQuestList.size() + 2 ) + m_vectorCompleteQuestList.size() )
							{
								m_nSelectedQuestIndex = m_vectorCompleteQuestList[iSelQuest - m_vectorProceedQuestList.size() - 2].iQuestIndex;
								RefreshQuestContent( m_nSelectedQuestIndex );								
							}
							else if( iSelQuest >= ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 ) &&
									 iSelQuest <  ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 ) + m_vectorRaidMessageList.size() )
							{
								m_nSelectedQuestIndex = m_vectorRaidMessageList[iSelQuest - ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 )].iQuestIndex;
								RefreshQuestContentByRaid( m_nSelectedQuestIndex );
							}

							// [090602: selo] 체크가 변경이면
							if( WMSG_COMMAND == wmsgResult )
							{
								if( m_lbQuestList.GetCheckState(iSelQuest) )
									AddSelectedQuest(m_nSelectedQuestIndex);
								else
									RemoveSelectedQuest(m_nSelectedQuestIndex);
							}

							return WMSG_SUCCESS;
						}
						int groupNameLine = 0;
						for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
						{
							if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
							{
								if(groupNameLine == 0)//proceed
								{
									m_bProceedListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
								}
								else if(groupNameLine == 1)//complete
								{
									m_bCompleteListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;									
								}
								else if(groupNameLine == 2) // [090708: selo] 레이드 퀘스트
								{
									m_bRaidMessageListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
									break;
								}
							}
						}
					}
					else
					{
						// 선택된 퀘스트.						
						int	iSelQuest = m_lbQuestList.GetCurSel();
						if( iSelQuest != -1 )
						{
							// [090715: selo] 종류에 따른 정렬 처리 버그 수정
							// 아래의 식은 접은 것을 펼친것 으로 계산하여 작업한 것임

							int iGroupCnt = -1;
							int iLastGroupIndex = 0;
							
							for( int i = 0; i < m_lbQuestList.GetCurItemCount(0); ++i )
							{
								if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
								{
									++iGroupCnt;
									iLastGroupIndex = i;

									bool bBreak = false;

									if( m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE )
									{
										for( int j = i + 1; j < m_lbQuestList.GetCurItemCount(0); ++j )
										{
											if( m_lbQuestList.GetState(j) == CUIListBoxEx::PS_CLOSE
												|| m_lbQuestList.GetState(j) == CUIListBoxEx::PS_EXTEND )
											{												
												iSelQuest += (j - (i + 1));
												i += (j - (i +1));												
												bBreak = true;
												
												break;
											}
										}
									}

									if( bBreak )
										continue;
								}

								if( i == iSelQuest )
								{
									int nTemp = iSelQuest - (iLastGroupIndex + 1);	// 해당 그룹의 몇번째 라인인지
									if( iGroupCnt >= 0  && iGroupCnt < QCATEGORY_END )
									{
										m_nSelectedQuestIndex = m_vectorCategoryQuestList[iGroupCnt][nTemp].iQuestIndex;
										RefreshQuestContent(m_nSelectedQuestIndex);
									}
									else if( iGroupCnt >= QCATEGORY_END )
									{
										m_nSelectedQuestIndex = m_vectorRaidMessageList[nTemp].iQuestIndex;
										RefreshQuestContentByRaid(m_nSelectedQuestIndex);
									}
								}
							}

							// [090602: selo] 체크가 변경이면
							if( WMSG_COMMAND == wmsgResult )
							{
								if( m_lbQuestList.GetCheckState(iSelQuest) )
									AddSelectedQuest(m_nSelectedQuestIndex);
								else
									RemoveSelectedQuest(m_nSelectedQuestIndex);
							}

							return WMSG_SUCCESS;
						}
						int groupNameLine = 0;
						for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
						{
							if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
							{
								if( groupNameLine < QCATEGORY_END)
								{
									m_bCategoryListExtended[groupNameLine] = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;								
								}
								else	// [090708: selo] 레이드 퀘스트
								{
									m_bRaidMessageListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
									break;
								}
							}
						}

					}
				}
				else if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}				

				_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				m_bTitleBarClick = FALSE;
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();					

					return WMSG_SUCCESS;
				}
				// Give up button				
				else if( m_nSelectedQuestIndex != -1 && ( wmsgResult = m_btnGiveUp.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{						
					_pUIMgr->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
					CTString	strMessage;
					CUIMsgBox_Info	MsgBoxInfo;	
					MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, m_nUIIndex, MSGCMD_QUEST_GIVEUP);	
					strMessage.PrintF( _S( 504, "정말 이 퀘스트를 취소하시겠습니까?" ));	
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
					m_bLockQuestList = TRUE;
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_cmbSort.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}