#include "stdh.h"
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIFiltering.h>
#include <algorithm>

#define MAX_HELPER_LEVEL				(5)
#define BUTTON_SIZE						(140)

enum eSelection
{
	SEL_HELPER_TEACHER,
	SEL_HELPER_STUDENT,
};

#define	HELPER_TAB_WIDTH				(120)
#define HELPER_BUTTON_START_Y			(HELPER_HEIGHT - 80)
#define HELPER_LIST_BOX_WIDTH			(223)
#define HELPER_LIST_BOX_HEIGHT			(198)

#define INFOMARK_SIZE 16
#define INFOMARK_TAB_SIZE 32
#define INFOMARK_Y_POS 46

#define SCROLLBAR_GAP 12

#define TAB_WIDTH 234
#define TAB_POS_LEFT 7

#define TAB3_SIZE0 114
#define TAB3_SIZE1 76
#define TAB3_SIZE2 (32 + SCROLLBAR_GAP)
#define TAB3_POS0 0
#define TAB3_POS1 TAB3_SIZE0
#define TAB3_POS2 (TAB3_SIZE0 + TAB3_SIZE1)
#define TAB3_POS3 (TAB3_SIZE0 + TAB3_SIZE1 + TAB3_SIZE2)

#define TAB4_SIZE0 98
#define TAB4_SIZE1 32
#define TAB4_SIZE2 60
#define TAB4_SIZE3 (32 + SCROLLBAR_GAP)
#define TAB4_POS0 0
#define TAB4_POS1 TAB4_SIZE0
#define TAB4_POS2 (TAB4_SIZE0 + TAB4_SIZE1)
#define TAB4_POS3 (TAB4_SIZE0 + TAB4_SIZE1 + TAB4_SIZE2)
#define TAB4_POS4 (TAB4_SIZE0 + TAB4_SIZE1 + TAB4_SIZE2 + TAB4_SIZE3)

// ----------------------------------------------------------------------------
// Name : CUIHelper()
// Desc : 
// ----------------------------------------------------------------------------
CUIHelper::CUIHelper()
{
	m_eHelperMenuState		= HELPER_MENU_REQ;
	m_bRegistredTeacher		= FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIHelper()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelper::~CUIHelper()
{
	if(!m_vectorHelperList.empty())
	{
		m_vectorHelperList.clear();
	}

	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, HELPER_WIDTH, 22 );
	m_rcTab.SetRect( 6, 26, HELPER_WIDTH - 6, 42 );

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackManagerTop.SetUV( 36, 69, 252, 87, fTexWidth, fTexHeight );
	m_rtBackManagerMiddle.SetUV( 36, 106, 252, 112, fTexWidth, fTexHeight );
	m_rtBackManagerBottom.SetUV( 36, 112, 252, 116, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 43, 69, 44, 86, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 220, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	int nButtonY = HELPER_BUTTON_START_Y;

	// MemberQuit button
	m_btnRefreshTeacher.Create( this, _S( 1135, "후견인 목록 갱신" ), 80, nButtonY, BUTTON_SIZE, 21 );			
	m_btnRefreshTeacher.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnRefreshTeacher.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnRefreshTeacher.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRefreshTeacher.CopyUV( UBS_IDLE, UBS_DISABLE );

	nButtonY += 23;

	// MemberQuit button
	m_btnRegisterTeacher.Create( this, _S( 1136, "후견인 등록" ), 80, nButtonY, BUTTON_SIZE, 21 );		
	m_btnRegisterTeacher.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnRegisterTeacher.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnRegisterTeacher.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRegisterTeacher.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// MemberQuit button
	m_btnCancelRegister.Create( this, _S( 1137, "후견인 등록 취소" ), 80, nButtonY, BUTTON_SIZE, 21 );	
	m_btnCancelRegister.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnCancelRegister.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnCancelRegister.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancelRegister.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// MemberQuit button
	m_btnBeMyTeacher.Create( this, _S( 1138, "견습생 신청" ), 80, nButtonY, BUTTON_SIZE, 21 );	
	m_btnBeMyTeacher.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnBeMyTeacher.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnBeMyTeacher.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBeMyTeacher.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// MemberQuit button
	m_btnFireMyTeacher.Create( this, _S( 1139, "견습생 포기"), 80, nButtonY, BUTTON_SIZE, 21 );	
	m_btnFireMyTeacher.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnFireMyTeacher.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnFireMyTeacher.CopyUV( UBS_IDLE, UBS_ON );
	m_btnFireMyTeacher.CopyUV( UBS_IDLE, UBS_DISABLE );	

	nButtonY += 23;
	// Exit button
	m_btnExit.Create( this, _S( 870, "닫기" ), 80, nButtonY, BUTTON_SIZE, 21 );
	m_btnExit.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnExit.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnExit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExit.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box of guild description
	m_lbTeacherList.Create( this, 8, 44, HELPER_LIST_BOX_WIDTH, HELPER_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 3, TRUE );
	m_lbTeacherList.CreateScroll( TRUE, 0, 0, 9, HELPER_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbTeacherList.SetSelBar( HELPER_LIST_BOX_WIDTH, _pUIFontTexMgr->GetLineHeight()+5, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbTeacherList.SetOverColor( 0xF8E1B5FF );
	m_lbTeacherList.SetSelectColor( 0xF8E1B5FF );
	m_lbTeacherList.SetColumnPosX( 1, TAB3_POS1);
	m_lbTeacherList.SetColumnPosX( 2, TAB3_POS2);
	// Up button
	m_lbTeacherList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbTeacherList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbTeacherList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbTeacherList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbTeacherList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbTeacherList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbTeacherList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// List box of guild description
	m_lbStudentList.Create( this, 8, 44, HELPER_LIST_BOX_WIDTH, HELPER_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 4, TRUE );
	m_lbStudentList.CreateScroll( TRUE, 0, 0, 9, HELPER_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbStudentList.SetSelBar( HELPER_LIST_BOX_WIDTH, _pUIFontTexMgr->GetLineHeight()+5, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbStudentList.SetOverColor( 0xF8E1B5FF );
	m_lbStudentList.SetSelectColor( 0xF8E1B5FF );
	m_lbStudentList.SetColumnPosX( 1, TAB4_POS1 );
	m_lbStudentList.SetColumnPosX( 2, TAB4_POS2 );
	m_lbStudentList.SetColumnPosX( 3, TAB4_POS3 );
	// Up button
	m_lbStudentList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbStudentList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbStudentList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbStudentList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbStudentList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbStudentList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbStudentList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	//info mark
	m_rtInfoMarkUV.SetUV(159, 162, 175, 178, fTexWidth, fTexHeight);
	for(int i=0; i<m_lbStudentList.GetScrollBarItemsPerPage(); ++i)
	{
		m_rcInfoMark[i].SetRect(
			TAB_POS_LEFT + TAB3_POS2 + (INFOMARK_TAB_SIZE - INFOMARK_SIZE) / 2
			, INFOMARK_Y_POS + i * m_lbTeacherList.GetLineHeight()
			, TAB_POS_LEFT + TAB3_POS2 + (INFOMARK_TAB_SIZE - INFOMARK_SIZE) / 2 + INFOMARK_SIZE
			, INFOMARK_Y_POS + i * m_lbTeacherList.GetLineHeight() + INFOMARK_SIZE);
	}

	//tool-tip
	m_rtInfoUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );	
	m_rtInfoLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_bShowInfo = FALSE;
	m_nCurInfoLines = 0;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenHelper()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::OpenHelper( )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_HELPER_REQ ) || IsVisible())
		return;	

	ResetHelper();

	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	
	CTString strTitle;
	CTString strMessage;

	const int iLevel	= _pNetwork->MyCharacterInfo.level;
	
	if( iLevel >= 20 )
	{
		strTitle = _S( 1140, "견습생 관리" );			
		// Create message box
		_pUIMgr->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1141, "게임을 시작하는 유저들을 도와주기 위해서 후견인을 등록할 수 있습니다. 후견인 등록을 하면, 신규 유저가 후견인 리스트를 볼 때 [%s]님의 이름이 보이게 됩니다." ), _pNetwork->MyCharacterInfo.name );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			
		
		strMessage.PrintF( _S( 1142, "돌봐주는 유저가 %d레벨이 되면, 명성치를 얻을 수 있으며, 명성치가 올라감에 따라 보상이 주어집니다." ), GetLimitLevel() );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );		
		
		strMessage.PrintF( "" );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		strMessage.PrintF( _S( 1143, "보상내용" ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		strMessage.PrintF( _S( 1144, "명성치가 100이 되면 각 마을의 방어구 상점에서, 명성 아이템을 구매할 수 있는 권한이 주어집니다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		strMessage.PrintF( _S( 1145, "명성 아이템은 명성치가 100이하인 유저는 구매 및 착용을 할 수 없습니다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_TEACHER );		
	}
	else
	{
		strTitle = _S( 1147, "후견인 관리" );			
		// Create message box
		_pUIMgr->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1148, "후견인은 [%s]님을 %d레벨까지 성장하는데 도움을 주는 사람을 이야기합니다. 후견인 리스트에서 [%s]님께 도움을 줄 수 있는 사람을 찾아보세요." ), _pNetwork->MyCharacterInfo.name, GetLimitLevel(), _pNetwork->MyCharacterInfo.name );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		strMessage.PrintF( _S( 1149, "[%s]님께서는 후견인을 단 한분만 선정할 수 있으며, 선정후에는 후견인을 변경할 수 없습니다." ), _pNetwork->MyCharacterInfo.name );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_STUDENT );		
	}
	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE, _S( 880, "취소하기." ) );				
}

static LONG g_lChaIndex				= -1;		// 가입 신청과 관련된 부분에만 쓰임.
static CTString g_strTeacherName	= CTString("");

// ----------------------------------------------------------------------------
// Name : ResetHelper()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::ResetHelper()
{
	m_lbTeacherList.ResetAllStrings();
	m_lbStudentList.ResetAllStrings();

	m_eHelperMenuState		= HELPER_MENU_REQ;
	g_lChaIndex				= -1;
	g_strTeacherName.Clear();

	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	//ClearHelperList();

	_pUIMgr->RearrangeOrder( UI_HELPER, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ClearHelperList()
// Desc : 멤버 목록을 클리어합니다.
// ----------------------------------------------------------------------------
void CUIHelper::ClearHelperList()
{
	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
		m_vectorTeacherInfoList.clear();
		m_vectorStudentInfoList.clear();
	}
}

//static LONG g_lHelperIndex	= -1;

// ----------------------------------------------------------------------------
// Name : BeMyTeacher()
// Desc : 견습생이 후견인이 되어달라고 요청함.
// ----------------------------------------------------------------------------
void CUIHelper::BeMyTeacher( )
{
	int	iSelMember = m_lbTeacherList.GetCurSel();
	if( iSelMember != -1 )
	{
		sHelperMember &TempMember = m_vectorHelperList[iSelMember];

		g_lChaIndex		= TempMember.lIndex;
		g_strTeacherName= TempMember.strName;
		
		_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
		_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
		_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 1150, "후견인 신청" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_BEMYTEACHER );	
		strMessage.PrintF( _S( 1151, "[%s]님을 후견인으로 신청하시겠습니까?" ), TempMember.strName );			
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
// Name : SetRegistredTeacher()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::SetRegistredTeacher( BOOL bRegistered )
{
	m_bRegistredTeacher = bRegistered;
}

// ----------------------------------------------------------------------------
// Name : FireMyTeacher()
// Desc : 나 니 학생 안할란다...
// ----------------------------------------------------------------------------
void CUIHelper::FireMyTeacher( )
{
	ASSERT( _pNetwork->MyCharacterInfo.lTeacherIndex != -1 );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1152, "견습생 포기" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_FIRE_MYTEACHER );		
	strMessage.PrintF( _S( 1153, "[%s]님의 견습생을 그만 두시겠습니까? 한번 그만 두게되면 다시는 후견인을 선정할 수 없습니다." ), _pNetwork->MyCharacterInfo.strTeacherName );	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : TeacherAccept()
// Desc : 후견인이 되어주겠는지 물어봄.
// ----------------------------------------------------------------------------
void CUIHelper::TeacherAccept( LONG lIndex, const CTString& strStudentName )
{
	g_lChaIndex			= lIndex;
	g_strTeacherName	= strStudentName;

	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1154, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_STUDENT_ACCEPT );	
	strMessage.PrintF( _S( 1155,  "[%s] 님이 후견인이 되어달라는 부탁을 합니다. 수락하시겠습니까?"  ),  strStudentName );	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : GetLimitLevel()
// Desc : 견습생과 후견인의 제한 레벨을 얻습니다.
// ----------------------------------------------------------------------------
int CUIHelper::GetLimitLevel()
{
	extern UINT g_uiEngineVersion;
	if( g_uiEngineVersion >= 10000 )	
		m_iLimitLevel	= 5;
	else
		m_iLimitLevel	= 20;
	return m_iLimitLevel;
}

// ----------------------------------------------------------------------------
// Name : AddToHelperList()
// Desc : 멤버 목록에 추가
// ----------------------------------------------------------------------------
void CUIHelper::AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );
	
	if( iter == m_vectorHelperList.end() )
	{
		if( m_vectorHelperList.size() <= HELPER_MAX_STUDENTS )
		{
			sHelperMember TempHelperMember;
			TempHelperMember.lIndex			= lIndex;
			TempHelperMember.strName		= strName;
			TempHelperMember.iLevel			= iLevel;
			TempHelperMember.iJob			= iJob;
			TempHelperMember.iJob2			= iJob2;
			m_vectorHelperList.push_back(TempHelperMember);
		}
	}
}


// ----------------------------------------------------------------------------
// Name : AddToTeacherInfoList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::AddToTeacherInfoList(LONG index, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail)
{
	if(m_vectorHelperList.size() == m_vectorTeacherInfoList.size() + 1)
	{
		sTeacherInfo info;
		info.index = index;
		info.nFame = fame;
		info.nCntTeachingStudent = cntTeaching;
		info.nCntCompleteStudent = cntComplete;
		info.nCntFailStudent = cntFail;
		m_vectorTeacherInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorTeacherInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : AddToStudentInfoList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::AddToStudentInfoList(LONG index, const char *szFirstDate, const char *szFinalDate)
{
	if(m_vectorHelperList.size() == m_vectorStudentInfoList.size() + 1)
	{
		sStudentInfo info;
		info.index = index;
		strcpy(info.szFirstDate, szFirstDate);
		strcpy(info.szFinalDate, szFinalDate);
		m_vectorStudentInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorStudentInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : DelFromHelperList()
// Desc : 멤버 목록에서 제거
// ----------------------------------------------------------------------------
void CUIHelper::DelFromHelperList( LONG lIndex )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		int dist = std::distance(m_vectorHelperList.begin(), iter);
		m_vectorHelperList.erase( iter );
		if(m_vectorTeacherInfoList.size() > 0)
		{
			m_vectorTeacherInfoList.erase(m_vectorTeacherInfoList.begin() + dist);
		}
		else if(m_vectorStudentInfoList.size() > 0)
		{
			m_vectorStudentInfoList.erase(m_vectorStudentInfoList.begin() + dist);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeHelperLevel()
// Desc : 멤버의 레벨을 변경함.
// ----------------------------------------------------------------------------
void CUIHelper::ChangeHelperLevel( LONG lIndex, int iLevel )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		(*iter).iLevel = iLevel;
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshTeacherList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIHelper::RefreshTeacherList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbTeacherList.ResetAllStrings();
	std::sort(m_vectorHelperList.begin(), m_vectorHelperList.end());

	COLOR		crLevel = 0xAAAAAAFF;	
	
	std::vector<sHelperMember>::const_iterator member_end = m_vectorHelperList.end();
	std::vector<sHelperMember>::iterator it = m_vectorHelperList.begin();
	for(it = m_vectorHelperList.begin(); it != member_end; ++it)
	{		
		const int iJob		= (*it).iJob;
		const int iJob2		= (*it).iJob2;
		m_lbTeacherList.AddString( 0, (*it).strName, 0xFFFFFFFF );	// 이름
		m_lbTeacherList.AddString( 1, JobInfo().GetName( iJob,  iJob2 ), 0xFFFFFFFF );					// 클래스
		m_lbTeacherList.AddString( 2, CTString(" "), 0xFFFFFFFF );					// info mark용 공간
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshStudentList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIHelper::RefreshStudentList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbStudentList.ResetAllStrings();
	std::sort(m_vectorHelperList.begin(), m_vectorHelperList.end());

	COLOR		crLevel = 0xAAAAAAFF;
	CTString strLevel;
	
	std::vector<sHelperMember>::const_iterator member_end = m_vectorHelperList.end();
	std::vector<sHelperMember>::iterator it = m_vectorHelperList.begin();
	for(it = m_vectorHelperList.begin(); it != member_end; ++it)
	{		
		if((*it).iJob>=0 && (*it).iJob<TOTAL_JOB){
			const int iJob		= (*it).iJob;
			const int iJob2		= (*it).iJob2;
			const int iLevel	= (*it).iLevel;
			strLevel.PrintF( "%d", iLevel );
			m_lbStudentList.AddString( 0, (*it).strName, 0xFFFFFFFF );	// 이름
			m_lbStudentList.AddString( 1, strLevel, 0xFFFFFFFF );									// 레벨
			m_lbStudentList.AddString( 2, JobInfo().GetName( iJob, iJob2 ), 0xFFFFFFFF );					// 클래스
			m_lbStudentList.AddString( 3, CTString(" "), 0xFFFFFFFF );					// info mark용 공간
		} else ASSERTALWAYS("JOB NUMBER must have this range [iJob>=0 && iJob<TOTAL_JOB]!!");
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::Render()
{
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;

	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
		m_rtBackTop.U0, m_rtBackTop.V0,
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );	

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 18,
		m_rtBackManagerTop.U0, m_rtBackManagerTop.V0,
		m_rtBackManagerTop.U1, m_rtBackManagerTop.V1,
		0xFFFFFFFF );
	
	nY += 18;
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + HELPER_LIST_BOX_HEIGHT + 2,
		m_rtBackManagerMiddle.U0, m_rtBackManagerMiddle.V0,
		m_rtBackManagerMiddle.U1, m_rtBackManagerMiddle.V1,
		0xFFFFFFFF );
	
	nY += HELPER_LIST_BOX_HEIGHT + 2;		
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 4,
		m_rtBackManagerBottom.U0, m_rtBackManagerBottom.V0,
		m_rtBackManagerBottom.U1, m_rtBackManagerBottom.V1,
		0xFFFFFFFF );
	
	nY += 4;	

	// Middle 2
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );

	// ----------------------------------------------------------------------------		

	if(m_eHelperMenuState == HELPER_MENU_STUDENT)
	{
		nX = m_nPosX + TAB_POS_LEFT + TAB3_POS1;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		// 탭 경계 그리기
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
			m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
			0xFFFFFFFF );

		nX = m_nPosX + TAB_POS_LEFT + TAB3_POS2;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
			m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
			0xFFFFFFFF );

		m_lbTeacherList.Render();

		if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 )
		{
			m_btnRefreshTeacher.Render();			// 후견인 목록 갱신		
			m_btnBeMyTeacher.Render();				// 견습생
		}
		else
		{
			m_btnFireMyTeacher.Render();
		}		
	}
	else
	{
		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS1;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		// 탭 경계 그리기
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
			m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
			0xFFFFFFFF );

		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS2;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
			m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
			0xFFFFFFFF );
		
		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS3;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
			m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
			0xFFFFFFFF );

		if( !m_bRegistredTeacher )
			m_btnRegisterTeacher.Render();			// 후견인 등록
		else
			m_btnCancelRegister.Render();			// 후견인 등록 취소	

		m_lbStudentList.Render();
	}

	m_btnExit.Render();							// 닫기

	// Close button
	m_btnClose.Render();

	//Info Mark
	int listCount = 0;
	if(m_eHelperMenuState == HELPER_MENU_STUDENT)
		listCount = m_vectorHelperList.size() - m_lbStudentList.GetScrollBarPos();
	else if(m_eHelperMenuState == HELPER_MENU_TEACHER)
		listCount = m_vectorHelperList.size() - m_lbTeacherList.GetScrollBarPos();
	for(int i=0; i<listCount; ++i)
	{
		int nX0 = m_rcInfoMark[i].Left + m_nPosX;
		int nX1 = m_rcInfoMark[i].Right + m_nPosX;
		int nY0 = m_rcInfoMark[i].Top + m_nPosY;
		int nY1 = m_rcInfoMark[i].Bottom + m_nPosY;
		_pUIMgr->GetDrawPort()->AddTexture(
			nX0, nY0, nX1, nY1
			, m_rtInfoMarkUV.U0, m_rtInfoMarkUV.V0, m_rtInfoMarkUV.U1, m_rtInfoMarkUV.V1
			, 0xFFFFFFFF );
	}


	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	CTString strTitle = "";
	switch( m_eHelperMenuState )
	{
	case HELPER_MENU_TEACHER :
		strTitle = _S( 1156, "견습생 리스트" );		
		break;	
	case HELPER_MENU_STUDENT:
		if( _pNetwork->MyCharacterInfo.lTeacherIndex != -1 )
		{
			strTitle.PrintF(_S( 1157, "후견인 - [%s]" ), _pNetwork->MyCharacterInfo.strTeacherName );
		}
		else
		{
			strTitle = CTString(_S( 1158, "후견인 리스트" ));		
		}
		break;
	}
	
	// Text in guild
	_pUIMgr->GetDrawPort()->PutTextEx( strTitle, m_nPosX + HELPER_TITLE_TEXT_OFFSETX,
										m_nPosY + HELPER_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	if( m_eHelperMenuState == HELPER_MENU_STUDENT )
	{
		nY = m_nPosY + 29;
		nX = m_nPosX + TAB_POS_LEFT + TAB3_POS0 + TAB3_SIZE0 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1159, "이 름" ), nX, nY, 0x6B6B6BFF );		
	
		nX = m_nPosX + TAB_POS_LEFT + TAB3_POS1 + TAB3_SIZE1 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1160, "클래스" ), nX, nY, 0x6B6B6BFF );		

		nX = m_nPosX + TAB_POS_LEFT + TAB3_POS2 + TAB3_SIZE2 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1661, "정보" ), nX, nY, 0x6B6B6BFF );
	}
	else
	{
		nY = m_nPosY + 29;
		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS0 + TAB4_SIZE0 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1159, "이 름" ), nX, nY, 0x6B6B6BFF );		

		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS1 + TAB4_SIZE1 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1161, "레벨" ), nX, nY, 0x6B6B6BFF );		
	
		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS2 + TAB4_SIZE2 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1160, "클래스" ), nX, nY, 0x6B6B6BFF );		

		nX = m_nPosX + TAB_POS_LEFT + TAB4_POS3 + TAB4_SIZE3 / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1661, "정보" ), nX, nY, 0x6B6B6BFF );
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	RenderInfo();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIHelper::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

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

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			//on info mark?
			m_bShowInfo = FALSE;
			int listCount = 0;
			if(m_eHelperMenuState == HELPER_MENU_STUDENT)
				listCount = m_vectorHelperList.size() - m_lbStudentList.GetScrollBarPos();
			else if(m_eHelperMenuState == HELPER_MENU_TEACHER)
				listCount = m_vectorHelperList.size() - m_lbTeacherList.GetScrollBarPos();
			for(int i=0; i<listCount; ++i)
			{
				if(IsInsideRect(nX, nY, m_rcInfoMark[i]))
				{
					m_bShowInfo = TRUE;
					ShowInfo(i, FALSE);
					break;
				}
			}

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL || 
				m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
			{
				if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnRefreshTeacher.MouseMessage( pMsg ) != WMSG_FAIL )			// 후견인 목록 갱신
					return WMSG_SUCCESS;						
				else if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnBeMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
					return WMSG_SUCCESS;
				else if(  _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && m_btnFireMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
					return WMSG_SUCCESS;
				else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else
			{
				// List box
				if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( !m_bRegistredTeacher && m_btnRegisterTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록
					return WMSG_SUCCESS;
				else if( m_bRegistredTeacher && m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록 취소
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
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ||
					m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
				else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
				{
					if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnRefreshTeacher.MouseMessage( pMsg ) != WMSG_FAIL )			// 후견인 목록 갱신
						return WMSG_SUCCESS;						
					else if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnBeMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
						return WMSG_SUCCESS;
					else if(  _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && m_btnFireMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
						return WMSG_SUCCESS;
					else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else
				{
					// List box
					if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if( !m_bRegistredTeacher && m_btnRegisterTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록
						return WMSG_SUCCESS;
					else if( m_bRegistredTeacher && m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록 취소
						return WMSG_SUCCESS;
				}

				_pUIMgr->RearrangeOrder( UI_HELPER, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Close button
			if( ((wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL ) ||
				((wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetHelper();
				}
				return WMSG_SUCCESS;
			}
			else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
			{					
				if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && ((wmsgResult = m_btnRefreshTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						// 서버에 목록 갱신 메세지 보내기.
						_pNetwork->TeachRefreshTeacherList();
					}
					return WMSG_SUCCESS;
				}
				else if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && ( (wmsgResult = m_btnBeMyTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						// 후견인에게 견습생으로 신청합니다.
						BeMyTeacher();
					}
					return WMSG_SUCCESS;
				}
				else if( _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && ( (wmsgResult = m_btnFireMyTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						FireMyTeacher();
					}
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}					
			}
			else
			{
				// List box
				if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( !m_bRegistredTeacher && ((wmsgResult = m_btnRegisterTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					// 선생 목록에 추가함.
					if(wmsgResult == WMSG_COMMAND)
					{
						RegisterTeacher();
					}
					return WMSG_SUCCESS;
				}
				else if( m_bRegistredTeacher && ((wmsgResult = m_btnCancelRegister.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					// 나 선생 안해~!!!
					if(wmsgResult == WMSG_COMMAND)
					{
						CancelRegisterTeacher();
					}
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{				
				if( m_eHelperMenuState == HELPER_MENU_STUDENT )
				{
					// List box of member list
					if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )		// 후견인 목록
						return WMSG_SUCCESS;
				}
				else if( m_eHelperMenuState == HELPER_MENU_TEACHER )
				{
					// List box of member list
					if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )		// 견습생 목록
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
void CUIHelper::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_NOTIFY:		// 에러 발생
		{			
		}
		break;
	case MSGCMD_HELPER_BEMYTEACHER:	// 후견인 신청 중...
		{
			if( !bOK )
				return;
			
			_pUIMgr->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 1162, "후견인 신청" ), UMBS_CANCEL, UI_HELPER, MSGCMD_HELPER_BEMYTEACHER_REQ );	
			strMessage.PrintF( _S( 1163, "%s 님께 후견인이 되어달라는 메시지를 보내고, 수락을 기다리는 중입니다."  ), g_strTeacherName );	
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			_pNetwork->TeachTeacherRequest( g_lChaIndex, g_strTeacherName );
		}
		break;
		
	case MSGCMD_HELPER_BEMYTEACHER_REQ:
		// 학생이 후견인 신청을 취소한 경우.
		{	
			_pNetwork->TeachTeacherReject( TRUE, g_lChaIndex, g_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_FIRE_MYTEACHER:		// 학생이 선생을 때려침(내가 학생일 때지...)
		{
			if( !bOK )
				return;

			// 서버에 메세지를 보냄.
			_pNetwork->TeachTeacherGiveUp( _pNetwork->MyCharacterInfo.lTeacherIndex, _pNetwork->MyCharacterInfo.strTeacherName, 
				g_lChaIndex, g_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_REGISTER:			// 선생 목록에 추가합니다.
		{
			if( !bOK )
				return;
			// 서버에 메세지를 보냄.
			_pNetwork->TeachTeacherRegister();
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
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_HELPER_REQ:
		{
			switch ( nResult ) 
			{
			case SEL_HELPER_TEACHER:		// 견습생 관리
				{
					m_eHelperMenuState = HELPER_MENU_TEACHER; 					
					RefreshStudentList( TRUE );
					_pUIMgr->RearrangeOrder( UI_HELPER, TRUE );
				}
				break;
				
			case SEL_HELPER_STUDENT:		// 후견인 관리
				{
					m_eHelperMenuState = HELPER_MENU_STUDENT; 
					RefreshTeacherList( TRUE );
					_pUIMgr->RearrangeOrder( UI_HELPER, TRUE );										
				}
				break;

			default:		// 취소
				{
					m_eHelperMenuState = HELPER_MENU_REQ;
					_pUIMgr->RearrangeOrder( UI_HELPER, FALSE );
				}
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetFocus()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

// ----------------------------------------------------------------------------
// Name : RegisterTeacher()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::RegisterTeacher( )					// 후견인 등록
{
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1164, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_REGISTER );	
	strMessage.PrintF( _S( 1165, "후견인 목록에 등록하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : CancelRegisterTeacher()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::CancelRegisterTeacher( )			// 후견인 등록 취소
{
	_pUIMgr->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1166, "후견인 등록 취소" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_CANCEL_REGISTER );	
	strMessage.PrintF( _S( 1167, "후견인 목록에서 삭제하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::RenderInfo()
{
	if( !m_bShowInfo ) return;

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// information region
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Top,
										m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top,
										m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top,
										m_rcInfo.Right, m_rcInfo.Top + 7,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Top + 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rcInfo.Right, m_rcInfo.Bottom - 7,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Bottom - 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right, m_rcInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFFF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Render information
	int	nInfoX = m_rcInfo.Left + 12;
	int	nInfoY = m_rcInfo.Top + 8;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strInfo[iInfo], nInfoX, nInfoY, m_colInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ShowInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::ShowInfo(int line, BOOL bRenew)
{
	if(!m_bShowInfo) return;
	int absLine = line;

	if(m_eHelperMenuState == HELPER_MENU_STUDENT)//견습생이 창을 띄움.
	{
		absLine += m_lbTeacherList.GetScrollBarPos();
		if(absLine < m_lbTeacherList.GetScrollBarPos()
		|| absLine >= m_vectorHelperList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sHelperMember &helpMem = m_vectorHelperList[absLine];
		//TEMP : 초고급 제련석 관련
		for(LONG i=0; i<m_vectorTeacherInfoList.size(); ++i)
		{
			if(helpMem.lIndex == m_vectorTeacherInfoList[i].index)
			{
				break;
			}
		}
		if(i == m_vectorTeacherInfoList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sTeacherInfo &info = m_vectorTeacherInfoList[i];
		m_nCurInfoLines = 5;
		m_strInfo[0].PrintF(_S( 1662, "명성 수치 : %d" ), info.nFame);
		m_colInfo[0] = 0xE0FF40FF;
		m_strInfo[1].PrintF(_S( 1663, "양성중인 초보자 : %d" ), info.nCntTeachingStudent);
		m_colInfo[1] = 0xFFFFFFFF;
		m_strInfo[2].PrintF(_S( 1664, "양성 완료 인원 : %d" ), info.nCntCompleteStudent);
		m_colInfo[2] = 0xFFFFFFFF;
		m_strInfo[3].PrintF(_S( 1665, "양성 실패 인원 : %d" ), info.nCntFailStudent);
		m_colInfo[3] = 0xFFFFFFFF;
		int percent = int(info.nCntCompleteStudent / float(info.nCntCompleteStudent + info.nCntFailStudent) * 100 + 0.5f);//반올림
		m_strInfo[4].PrintF(_S( 1666, "양성 성공률 : %d %%" ), percent); 
		m_colInfo[4] = 0xFFFFFFFF;
	}
	else if(m_eHelperMenuState == HELPER_MENU_TEACHER)//선생이 창을 띄움.
	{
		absLine += m_lbStudentList.GetScrollBarPos();
		if(absLine < m_lbStudentList.GetScrollBarPos()
		|| absLine >= m_vectorHelperList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sHelperMember &helpMem = m_vectorHelperList[absLine];
		//TEMP : 초고급 제련석 관련
		for(LONG i=0; i<m_vectorStudentInfoList.size(); ++i)
		{
			if(helpMem.lIndex == m_vectorStudentInfoList[i].index)
			{
				break;
			}
		}
		if(i == m_vectorStudentInfoList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sStudentInfo &info = m_vectorStudentInfoList[i];
		m_nCurInfoLines = 4;
		m_strInfo[0].PrintF(_S( 1667, "후견인 결성일" )); 
		m_colInfo[0] = 0xFFFFFFFF;
		m_strInfo[1].PrintF("%s", info.szFirstDate);
		m_colInfo[1] = 0xFFFFFFFF;

		m_strInfo[2].PrintF(_S( 1668, "종료 시간" )); 
		m_colInfo[2] = 0xFFFFFFFF;
		m_strInfo[3].PrintF("%s", info.szFinalDate);
		m_colInfo[3] = 0xFFFFFFFF;
	}

	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	GetAbsPos(nInfoPosX, nInfoPosY);
	nInfoPosX += m_rcInfoMark[line].Left;
	nInfoPosY += m_rcInfoMark[line].Top;
	
	nInfoWidth = 0;
	for(int i=0; i<m_nCurInfoLines; ++i)
	{
		int infoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strInfo[i].Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		if(infoWidth > nInfoWidth) nInfoWidth = infoWidth;
	}
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	
	// Update item information box
	nInfoPosX += INFOMARK_SIZE / 2 - nInfoWidth / 2;

	if( nInfoPosX < _pUIMgr->GetMinI() )
		nInfoPosX = _pUIMgr->GetMinI();
	else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
		nInfoPosX = _pUIMgr->GetMaxI() - nInfoWidth;

	if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
	{
		nInfoPosY += INFOMARK_SIZE;
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
	}
	else
	{
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
	}
}
