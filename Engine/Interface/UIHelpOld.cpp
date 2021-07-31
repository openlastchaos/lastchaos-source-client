#include "stdh.h"
#include <Engine/Interface/UIHelpOld.h>
#include <Engine/Interface/UIInternalClasses.h>

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIHelp()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIHelpOld::CUIHelpOld()
{
	m_nCurSection		= HELPS_HELP;
	m_nCurBoardType		= HELPT_LIST;
	m_nCurrentPage		= 0;
	m_nCurPage			= 0;
	m_nValidPageBtnCount= 0;
	m_bWaitResponseMode = FALSE;
	m_nWantPage			= 1;
	m_nWantWrite		= 0;
	m_bVisible			= FALSE;
	m_ptdImageTexture	= NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIHelpOld()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelpOld::~CUIHelpOld()
{
	Destroy();
}

// ------------------------------- ---------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 600, 22 );	
	m_rcListTitle.SetRect( 15, 35, 585, 57 );
	m_rcListContent.SetRect( 27, 76, 578, 366 );
	m_rcReadSubject.SetRect( 23, 61, 577, 84 );
	m_rcReadContent.SetRect( 23, 102, 577, 373 );	

	// Create web board texture
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTitleUL.SetUV( 0, 0, 49, 28, fTexWidth, fTexHeight );
	m_rtTitleUM.SetUV( 50, 0, 56, 28, fTexWidth, fTexHeight );
	m_rtTitleUR.SetUV( 57, 0, 89, 28, fTexWidth, fTexHeight );

	m_rtBackUL.SetUV( 0, 42, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 42, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 42, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 0, 75, 96, 93, fTexWidth, fTexHeight );
	m_rtSplitterS.SetUV( 91, 18, 98, 19, fTexWidth, fTexHeight );
	m_rtSplitterL.SetUV( 91, 21, 98, 23, fTexWidth, fTexHeight );

	m_rtBackSub.SetUV( 91, 26, 106, 41, fTexWidth, fTexHeight );

	m_rtEditBoxL.SetUV( 90, 0, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxM.SetUV( 95, 0, 101, 16, fTexWidth, fTexHeight );
	m_rtEditBoxR.SetUV( 103, 0, 106, 16, fTexWidth, fTexHeight );

	m_rtEditBoxUL.SetUV( 90, 0, 93, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUM.SetUV( 94, 0, 102, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUR.SetUV( 103, 0, 106, 3, fTexWidth, fTexHeight );
	m_rtEditBoxML.SetUV( 90, 4, 93, 12, fTexWidth, fTexHeight );
	m_rtEditBoxMM.SetUV( 96, 5, 101, 11, fTexWidth, fTexHeight );
	m_rtEditBoxMR.SetUV( 103, 4, 106, 12, fTexWidth, fTexHeight );
	m_rtEditBoxLL.SetUV( 90, 13, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLM.SetUV( 94, 13, 102, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLR.SetUV( 103, 13, 106, 16, fTexWidth, fTexHeight );
	
	// Close Button
	m_btnClose.Create( this, CTString( "" ), 568, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev button
	m_btnPrev.Create( this, CTString( "" ), 0, 0, 9, 9 );
	m_btnPrev.SetUV( UBS_IDLE, 0, 117, 9, 126, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_ON, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next button
	m_btnNext.Create( this, CTString( "" ), 0, 0, 9, 9 );
	m_btnNext.SetUV( UBS_IDLE, 11, 117, 20, 126, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_CLICK, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_ON, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// PrevContent button
	m_btnPrevContent.Create( this, CTString( "" ), 290, 407, 9, 9 );
	m_btnPrevContent.SetUV( UBS_IDLE, 0, 117, 9, 126, fTexWidth, fTexHeight );
	m_btnPrevContent.SetUV( UBS_CLICK, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrevContent.SetUV( UBS_ON, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrevContent.CopyUV( UBS_IDLE, UBS_DISABLE );

	// NextContent button
	m_btnNextContent.Create( this, CTString( "" ), 310, 407, 9, 9 );
	m_btnNextContent.SetUV( UBS_IDLE, 11, 117, 20, 126, fTexWidth, fTexHeight );
	m_btnNextContent.SetUV( UBS_CLICK, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNextContent.SetUV( UBS_ON, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNextContent.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List button
	m_btnList.Create( this, _S( 313, "목록" ), 23, 407, 63, 21 );	
	m_btnList.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnList.CopyUV( UBS_IDLE, UBS_ON ); 

	// Page buttons
	for( int i = 0; i < 10; i++ )
	{
		m_btnPage[i].Create( this, CTString( "10" ), 0, 0, 0, 0 );
		m_btnPage[i].SetTextColor( UBS_IDLE, 0x9EB19AFF );
		m_btnPage[i].SetTextColor( UBS_ON, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_CLICK, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_DISABLE, 0xFFFFCAFF );
	}

	// List box of list type
	m_lbListContent.Create( this, 15, 75, 570, 305, 29, 0, 9, 5, TRUE );
	m_lbListContent.SetOverColor( 0xFFD13CFF );
	m_lbListContent.SetSelectColor( 0xFFD13CFF );

	// Adjust column position of list box
	m_lbListContent.SetColumnPosX( 0, HELPOLD_LIST_MAIN_NO_CX, TEXT_CENTER );				// No
	m_lbListContent.SetColumnPosX( 1, HELPOLD_LIST_MAIN_SUBJECT_SX );						// Subject

	// List box of read type
	m_lbReadContent.Create( this, 24, 139, 542, 232, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 1, FALSE );

	// Create web board texture	
	m_ptdImageTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Help.tex" ) );
	FLOAT	fImageTexWidth	= m_ptdImageTexture->GetPixWidth();
	FLOAT	fImageTexHeight	= m_ptdImageTexture->GetPixHeight();

	sPageItem TempItem;
	// ----------------------------------------------------------------------------
	int iPageID		= AddPage( _S( 1453, "상단 인터페이스(개인정보 창, 메뉴 창, 타겟창)" ) );		

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 35;
	TempItem.nY		= 108;
	TempItem.strText= _S( 1454, "1) 개인 정보 및 메뉴창" );		
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 48;
	TempItem.nY		= 132;
	TempItem.nWidth	= 140;
	TempItem.nHeight= 100;
	TempItem.rtImage.SetUV( 370, 341, 510, 441, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_LIST;
	TempItem.lbListBox.Create( this, 208, 129, 350, 124, 12, 0, 8, 1, FALSE );
	AddReadingString( TempItem.lbListBox, _S( 1455, "1. 개인정보창" ), 0xe2e2e2FF );		
	AddReadingString( TempItem.lbListBox, _S( 1456, "플레이어 캐릭터의 정보와 현재 상태를 알려주는 창으로" ), 0xabababFF );	
	AddReadingString( TempItem.lbListBox, _S( 1457, "캐릭터의 레벨, HP, MP, 경험치를 보여줍니다." ), 0xabababFF );		
	AddReadingString( TempItem.lbListBox, _S( 1458, "2. 메뉴창" ), 0xe2e2e2FF );	
	AddReadingString( TempItem.lbListBox, _S( 1459, "개인정보창 내부상단에 위치한 5개의 메뉴 아이콘(게시판," ), 0xabababFF );	
	AddReadingString( TempItem.lbListBox, _S( 1460, "메신저, 스테이터스, 인벤토리, 시스템)으로 이루어져 있으며," ), 0xabababFF );	
	AddReadingString( TempItem.lbListBox, _S( 1461, "각각의 아이콘을 클릭하면 해당 메뉴 창이 열립니다." ), 0xabababFF );	
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 35;
	TempItem.nY		= 290;
	TempItem.strText= _S( 1462, "2) 타겟 정보창" );	
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 48;
	TempItem.nY		= 311;
	TempItem.nWidth	= 140;
	TempItem.nHeight= 43;
	TempItem.rtImage.SetUV( 370, 445, 510, 488, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 208, 312, 350, 56, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1463, "플레이어를 포함한 특정한 상대를 클릭했을 때 표시되는 창." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1464, "선택된 대상의 이름과 HP상태를 보여주고, NPC의 경우" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1465, "플레이어와의 상대적 강함정도의 정보를 보여줍니다." ), 0xabababFF );		
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1647, "스테이터스" ) );	
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 106, 510, 64, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1466, "메뉴 창의 스테이터스 아이콘을 클릭하거나, 단축키를 눌러 창을 열수 있습니다." ), 0xcfcaffFF );	
		AddReadingString( TempItem.lbListBox, _S( 1467, "엔터 채팅 시의 단축키 : T, 퀵 채팅 시의 단축키 : Alt + T" ), 0xcfcaffFF );						
		AddReadingString( TempItem.lbListBox, _S( 1468, "스테이터스 창에서는 캐릭터의 자세한 상태 정보를 볼 수 있으며, 탭을 통해" ), 0xcfcaffFF );		
		AddReadingString( TempItem.lbListBox, _S( 1469, "캐릭터 정보, 스킬, 액션, 소셜, 파티, 퀘스트 등이 표시됩니다." ), 0xcfcaffFF );					
		AddPageItem( iPageID, TempItem );
	}
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 38, 172, 530, 202, 12, 0, 8, 2, FALSE );
		TempItem.lbListBox.SetColumnPosX( 0, 1, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 1, 93, TEXT_LEFT );
		
		TempItem.lbListBox.AddString( 0, _S( 1470, "1. 캐릭터 정보" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1471, "캐릭터의 각종 능력치와 성향을 볼 수 있으며, 레벨업 시에 얻어지는 성장" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1472, "포인트를 분배하는 것도 가능합니다." ), 0xabababff );		

		TempItem.lbListBox.AddString( 0, _S( 1473, "2. 스킬" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1474, "현재 소유하고 있는 각종 스킬에 대한 정보를 볼 수 있으며 직접 클릭해" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0,CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1475, "사용하거나 드래그하여 단축 창에 등록할 수 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, _S( 1476, "3. 액션" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1477, "공격, 앉기 등 게임 진행에 필요한 액션들로 구성되어 있습니다. 직접 클릭해" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1478, "사용하거나 드래그하여 단축 창에 등록할 수 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, _S( 1479, "4. 소설" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1480, "인사, 환호 등 커뮤니티에 필요한 사회적 동작들로 구성되어 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1481, "직접 클릭해 사용하거나 드래그하여 단축 창에 등록할 수 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, _S( 1482, "5. 파티" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1483, "파티 초대, 파티 탈퇴 등 파티 플레이에 필요한 명령들로 구성되어 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1484, "직접 클릭해 사용하거나 드래그하여 단축 창에 등록할 수 있습니다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, _S( 1485, "6. 퀘스트" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1486, "현재 자신이 수행 중인 퀘스트들의 정보, 진행 현황을 볼 수 있으며," ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString(""), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1487, "수행 중인 퀘스트의 취소도 가능합니다." ), 0xabababff );		

		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1488, "채팅" ) );			
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 154;
	TempItem.nWidth	= 230;
	TempItem.nHeight= 152;
	TempItem.rtImage.SetUV( 118, 253, 343, 403, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 106, 530, 42, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1489, "전투 메시지를 포함한 각종 채팅 메시지를 볼 수 있는 창으로 탭의 형태로 되어있는 전체," ), 0xcfcaffFF );	
		AddReadingString( TempItem.lbListBox, _S( 1490, "파티, 길드, 매매의 4개 항목과 상단의 시스템메시지 창으로 구분되어 있습니다." ), 0xcfcaffFF );	
		AddPageItem( iPageID, TempItem );
	}

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 272, 152, 283, 110, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1491, "각 탭에 따라 자신이 입력하는 문구의 형태가" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1492, "바뀌게 되어 자신의 상황에 맞춰 탭을" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1493, "설정하게 되며, 시스템메시지에는 주고받은" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1494, "데미지 여부와 경험치 입수 상황, 공지 사항 등" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1495, "각종 시스템 관련 메시지가 표시됩니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 318, 530, 42, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1496, "퀵 채팅과 단축키의 사용이 유용한 대신 입력 시 엔터 키를 활용하는 엔터 채팅이 있습니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1497, "기본 설정은 퀵 채팅 방식이지만 옵션에서 언제라도 엔터 채팅으로 바꿀 수 있습니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1498, "단축창" ) );				

	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 108;
	TempItem.nWidth	= 254;
	TempItem.nHeight= 30;
	TempItem.rtImage.SetUV( 118, 481, 367, 507, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );
	
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 147, 520, 120, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1499, "스킬, 액션, 아이템 등 자신이 자주 쓰는 것들을 등록하여 단축키로 빠르게 사용할 수" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1500, "있는 메뉴로 각종 창에서 아이콘을 드래그하여 단축 창에 원하는 장소에 올려놓는 것으로" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1501, "간편하게 등록이 가능합니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1502,  "최대 3 페이지까지 등록이 가능하며 페이지 숫자 옆에 위치한 화살표를 클릭하여" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1503, "전환할 수 있습니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1648, "이동/시점변환" ) );		
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 30, 108, 540, 236, 12, 0, 8, 1, FALSE );
		AddReadingString( TempItem.lbListBox, _S( 1504, "1. 이동" ), 0xcfcaffFF );	
		AddReadingString( TempItem.lbListBox, _S( 1505, "이동하기를 원하는 장소를 마우스 왼쪽 버튼으로 클릭하면 해당 장소로 캐릭터가 이동합니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1506, "단, 캐릭터가 올라갈 수 없는 높은 곳이나 장애물로 막혀있는 곳일 경우에는 이동하지" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1507, "못합니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1508, "2. 시점 변환" ), 0xcfcaffFF );	
		AddReadingString( TempItem.lbListBox, _S( 1509, "마우스를 우 클릭한 상태를 유지하면서 마우스를 움직이는 것으로 자유롭게 시점을 변경" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1510, "마우스 휠을 앞으로 돌리는 것으로 확대, 뒤로 돌리는 것으로 축소할 수 있고 마우스 휠을" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1511, "클릭하는 것으로 시점이 180도 회전하게 됩니다. " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1512, "방향키로도 시점 변환이 가능하며," ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1513, "-> 키로 시계방향으로 360도 <- 키로는 시계 반대 방향으로 360도 회전하며," ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1514, "상하 방향키로는 시점의 상하로 변환이 됩니다." ), 0xabababFF );	
		
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1515, "아이템 사용" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 174;
	TempItem.nWidth	= 230;
	TempItem.nHeight= 90;
	TempItem.rtImage.SetUV( 118, 406, 343, 478, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 108, 530, 60, 12, 0, 8, 1, FALSE );

		AddReadingString( TempItem.lbListBox, _S( 1516, "사용하려는 아이템을 인벤토리(퀵 채팅 : Alt+E, 엔터 채팅 : E)에서 더블 클릭하거나" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1517, "단축 창에 올려놓은 뒤, 마우스로 등록되어있는 아이콘을 직접 클릭하거나 해당 단축키를" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1518, "누르는 것으로 사용할 수 있습니다." ), 0xabababFF );	
		
		AddPageItem( iPageID, TempItem );
	}
	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1519, "교환" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 108;
	TempItem.nWidth	= 116;
	TempItem.nHeight= 262;
	TempItem.rtImage.SetUV( 0, 253, 114, 511, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 164, 108, 404, 245, 12, 0, 8, 1, FALSE );

		AddReadingString( TempItem.lbListBox, _S( 1520, "우선 원하는 상대를 선택한 뒤, 액션 창(단축키:Alt+A) " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1521, "또는 단축 창에 등록해놓은 교환 아이콘을 클릭하면 상대에게 교환" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1522, "신청후 자신이 교환을 원하는 아이템을 인벤토리로부터 드래그하여" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1523, "교환 창에 올려 놓은 뒤, 준비 완료 버튼을 클릭하면 교환창에 올려진" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1524, "아이템의 외각선이 밝게 빛나며 준비를 완료하게 됩니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );
		AddReadingString( TempItem.lbListBox, _S( 1525, "상대방도 준비를 완료하면 교환 버튼이 활성화 되며 버튼을 클릭하면" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1526, "교환 하게 됩니다. " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );
		AddReadingString( TempItem.lbListBox, _S( 1649, "교환창에 한번 올린 아이템은 다시 소지품창으로 옮길수 없으니" ), 0xe2e2e2FF );	
		AddReadingString( TempItem.lbListBox, _S( 1527, "교환창에 아이템을 잘못 올렸을 경우에는 교환을 취소하신 후" ), 0xe2e2e2FF );	
		AddReadingString( TempItem.lbListBox, _S( 1528, "교환신청을 다시하여 교환하고자 하는 아이템을 새롭게 올리셔야" ), 0xe2e2e2FF );	
		AddReadingString( TempItem.lbListBox, _S( 1529, "합니다." ), 0xe2e2e2FF );	
		
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1530, "레이더" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 110;
	TempItem.nWidth	= 166;
	TempItem.nHeight= 142;
	TempItem.rtImage.SetUV( 347, 200, 511, 338, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 208, 108, 360, 218, 12, 0, 8, 1, FALSE );	
		AddReadingString( TempItem.lbListBox, _S( 1531, "레이더는 외곽에 동서남북이 표시되어 플레이어의 방향을" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1532, "알 수 있으며 안쪽에 표시되는 지형을 통해 현재 위치를" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1533, "알 수 있습니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );
		AddReadingString( TempItem.lbListBox, _S( 1534, "레이더 안쪽에 표시되는 포인트는 각 캐릭터들을 표시하며" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1535, "몬스터는 회색, NPC는 노란색, 선택된 상대는 적색으로" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1536, "표시하여 해당 캐릭터와의 거리를 쉽게 알 수 있습니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );
		AddReadingString( TempItem.lbListBox, _S( 1537, "레이더 상단에 옵션과 미니맵(ALT+W, W) 아이콘이 존재하며" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1538, "옵션에서는 레이더 내부에 파티 멤버, 길드원, NPC, 몬스터를" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1539, "포인트로 표시할지 여부를 설정할 수 있습니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}
	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1540, "미니맵" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 108;
	TempItem.nWidth	= 166;
	TempItem.nHeight= 198;
	TempItem.rtImage.SetUV( 347, 0, 512, 196, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 210, 108, 356, 196, 12, 0, 8, 1, FALSE );
		
		AddReadingString( TempItem.lbListBox, _S( 1541, "미니 맵은 현재 있는 존의 미니 맵과 함께 자신의 위치와" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1542, "방향을 알려주며 미니 맵 상단의 메뉴를 조작하여 다른 존의" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1543, "지도를 보거나 지도의 확대, 축소를 할 수 있습니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1544, "또한 해당 존의 마을 부분을 클릭하면 확대된 마을 지도를" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1545, "볼 수 있는데, 마을 내에 존재하는 NPC의 위치가 자세히" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1546, "표시되어 있어 쉽게 찾아갈 수 있습니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1547, "마을 지도 상태에서 한 번 더 클릭하면 원래의 존 지도로" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1548, "돌아가게 됩니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1549, "기본전투" ) );			
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 34, 108, 540, 268, 12, 0, 8, 1, FALSE );

		AddReadingString( TempItem.lbListBox, _S( 1550, "공격하길 원하는 캐릭터 위에 커서를 놓고 마우스 좌 버튼을 클릭하면 해당 몬스터의 이름과" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1551, "체력이 표시되는 타겟 창이 뜨며 자동적으로 사정거리까지 뛰어가 공격을 개시합니다." ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1552, "한 번 공격한 뒤부터는 계속 클릭해주지 않더라도 자동적으로 계속 공격하며, 공격을" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1553, "취소하고 싶을 때는 몬스터가 아닌 다른 곳을 클릭하면 공격을 멈추고 클릭된 지점으로" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1554, "이동하게 됩니다." ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1555, "타겟을 취소하고 싶으실 때는 ESC키를 눌러주시면 되며, 타겟을 바꾸고 싶을 때는 원하는" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1556, "대상을 클릭하면 그 대상으로 타겟이 바뀌게 됩니다." ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1557, "상대에게 커서를 놓고 마우스 우 클릭을 하면 상대를 공격하지는 않고 타겟팅만 하게 됩니다." ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1558, "이후 공격 아이콘이나 스킬 아이콘을 클릭하는 것으로 상대를 향해 사정거리까지 이동한 뒤," ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1559, "공격 또는 스킬을 사용합니다. " ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1560, "참고로 몬스터의 레벨이 플레이어의 레벨보다 높을 경우에는 붉은색 계통으로, 낮을 경우에는" ), 0xe2e2e2FF );		
		AddReadingString( TempItem.lbListBox, _S( 1561, "푸른색 계통으로 표시됩니다. 몬스터를 죽인 후 몬스터로부터 경험치와 아이템을 획득 할" ), 0xe2e2e2FF );		
		AddReadingString( TempItem.lbListBox, _S( 1562, "수 있습니다. 떨어진 아이템을 클릭하면 아이템을 집을 수 있습니다." ), 0xe2e2e2FF );		
		
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1563, "스킬" ) );				

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 35;
	TempItem.nY		= 108;
	TempItem.strText= _S( 1564, "1. 스킬의 종류" );		
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 35;
	TempItem.nY		= 262;
	TempItem.strText= _S( 1565, "2. 스킬의 습득" );		
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 32, 142, 546, 128, 12, 0, 8, 2, FALSE );
		TempItem.lbListBox.SetColumnPosX( 0, 1, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 1, 76, TEXT_LEFT );
		
		TempItem.lbListBox.AddString( 0, _S( 1566, "일반 스킬" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1567, "적이나 아군, 플레이어 자체에게 사용할 수 있는 스킬로 MP를 소모하여 적에게" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1568, "데미지를 주거나 플레이어 또는 아군의 능력치를 일시적으로 올려주거나" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1569, "회복시켜주는 스킬" ), 0xabababff );		

		TempItem.lbListBox.AddString( 0, _S( 1570, "강화 스킬" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1571, "한번 습득하게 되면 MP의 소모 없이 영구적으로 플레이어 캐릭터의 능력치가" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1572, "일정량 상승하게 되는 스킬" ), 0xabababff );		

		TempItem.lbListBox.AddString( 0, _S( 1573, "기타 스킬" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1574, "생산 등 전투와 직접적으로 연관되지는 않지만 게임 진행에 도움이 되는 스킬" ), 0xabababff );		
		
		AddPageItem( iPageID, TempItem );
	}

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 32, 296, 546, 74, 12, 0, 8, 1, FALSE );		
		
		AddReadingString( TempItem.lbListBox, _S( 1575, "해당 스킬마스터에게 숙련도를 소모하며 배워야 합니다.  스킬을 배우기 위해서는 해당 스킬에" ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1576, "지정된 최저 레벨 이상이어야 하며, 지정된 수치 이상의 숙련도를 지니고 있어야 합니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1577, "습득한 스킬을 스킬 창에 표시되며 드래그를 통해 단축 창에 등록하는 것이 가능합니다." ), 0xabababFF );	
		AddReadingString( TempItem.lbListBox, _S( 1578, "단축 창에 등록된 스킬은 해당하는 펑션 키 (F1~F10)을 클릭하여 사용하실 수 있습니다." ), 0xabababFF );	
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1579, "액션" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 108;
	TempItem.nWidth	= 151;
	TempItem.nHeight= 256;
	TempItem.rtImage.SetUV( 154, 0, 301, 249, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 196, 108, 372, 120, 12, 0, 8, 2, FALSE );
		TempItem.lbListBox.SetColumnPosX( 0, 1, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 1, 88, TEXT_LEFT );

		TempItem.lbListBox.AddString( 0, _S( 1580, "1. 일반 액션" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1581, "걷기/뛰기, 줍기, 교환등의 일반적인 플레이를" ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1582, "하는데 필요한 기능이다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "  " ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, CTString( "  " ), 0xabababff );
		TempItem.lbListBox.AddString( 0, _S( 1583, "2. 소셜 액션" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1584, "사교 동작 기능으로 다양한 표현을 할수 있다." ), 0xabababff );		
		TempItem.lbListBox.AddString( 0, CTString( "  " ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, CTString( "  " ), 0xabababff );
		TempItem.lbListBox.AddString( 0, _S( 1585, "3. 그룹 액션" ), 0xe2e2e2ff );		
		TempItem.lbListBox.AddString( 1, _S( 1586, "길드와 파티에 관련된 기능이다." ), 0xabababff );		
		
		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID			= AddPage( _S( 1587, "단축키" ) );				
	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 36;
	TempItem.nY		= 108;	
	TempItem.strText= _S( 1588, "시스템 메뉴 >> 옵션에 가면 엔터채팅을 체크하면 아래와 같이 사용할 수 있습니다." );		
	TempItem.crText	= 0xe2e2e2ff;
	AddPageItem( iPageID, TempItem );

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 36;
	TempItem.nY		= 140;	
	TempItem.strText= _S( 1589, "1. 엔터채팅" );		
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 40, 158, 540, 268, 12, 0, 8, 6, FALSE );
		TempItem.lbListBox.SetColumnPosX( 0, 43, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 1, 65, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 2, 160, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 3, 182, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 4, 290, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 5, 312, TEXT_LEFT );

		TempItem.lbListBox.AddString( 0, CTString(  "E, Tab" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1590, "인벤토리" ), 0xabababff );		
		TempItem.lbListBox.AddString( 2, CTString( "W" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1591, "지도창" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "T" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1592, "스테이터스 창" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "S" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1593, "스킬" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "A" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1594, "일반 액션" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "C" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1595, "소셜 액션" ), 0xabababff );		

		TempItem.lbListBox.AddString( 0, CTString( "G" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1596, "그룹 액션" ), 0xabababff );		
		TempItem.lbListBox.AddString( 2, CTString( "Q" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1597, "퀘스트&이벤트" ), 0xabababff );	
		TempItem.lbListBox.AddString( 4, CTString( "Z" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1598, "시스템 메뉴" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "B" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1599, "웹 게시판" ), 0xabababff );		
		TempItem.lbListBox.AddString( 2, CTString( "F" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1600, "메신저" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "X" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1601, "인터페이스 숨기기" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "F1~F10" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1602, "퀵 슬롯" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, CTString( "" ), 0xabababff );	
		TempItem.lbListBox.AddString( 4, CTString( "Alt + F1 ~ F3" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1603, "퀵 슬롯 페이지 변환" ), 0xabababff );	

		AddPageItem( iPageID, TempItem );
	}

	TempItem.eType	= HELP_TEXT;
	TempItem.nX		= 36;
	TempItem.nY		= 268;	
	TempItem.strText= _S( 1604, "2. 퀵채팅" );		
	TempItem.crText	= 0xcfcaffff;
	AddPageItem( iPageID, TempItem );

	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 40, 284, 540, 268, 12, 0, 8, 6, FALSE );
		TempItem.lbListBox.SetColumnPosX( 0, 53, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 1, 75, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 2, 215, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 3, 237, TEXT_LEFT );
		TempItem.lbListBox.SetColumnPosX( 4, 384, TEXT_RIGHT );
		TempItem.lbListBox.SetColumnPosX( 5, 406, TEXT_LEFT );

		TempItem.lbListBox.AddString( 0, CTString( "Alt + E,Tab" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1590, "인벤토리" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "Alt + W" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1591, "지도창" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "Alt + T" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1592, "스테이터스 창" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "Alt + S" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1593, "스킬" ), 0xabababff );				
		TempItem.lbListBox.AddString( 2, CTString( "Alt + A" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1594, "일반 액션" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "Alt + C" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1595, "소셜 액션" ), 0xabababff );		

		TempItem.lbListBox.AddString( 0, CTString( "Alt + G" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1596, "그룹 액션" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "Alt + Q" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1597, "퀘스트&이벤트" ), 0xabababff );	
		TempItem.lbListBox.AddString( 4, CTString( "Alt + Z" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1598, "시스템 메뉴" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "Alt + B" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1599, "웹 게시판" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "Alt + F" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, _S( 1600, "메신저" ), 0xabababff );		
		TempItem.lbListBox.AddString( 4, CTString( "Alt + X" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1601, "인터페이스 숨기기" ), 0xabababff );	

		TempItem.lbListBox.AddString( 0, CTString( "F1~F10" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 1, _S( 1602, "퀵 슬롯" ), 0xabababff );			
		TempItem.lbListBox.AddString( 2, CTString( "" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 3, CTString( "" ), 0xabababff );	
		TempItem.lbListBox.AddString( 4, CTString( "Alt + F1 ~ F3" ), 0xe2e2e2ff );
		TempItem.lbListBox.AddString( 5, _S( 1603, "퀵 슬롯 페이지 변환" ), 0xabababff );	

		AddPageItem( iPageID, TempItem );
	}

	// ----------------------------------------------------------------------------
	iPageID		= AddPage( _S( 1605, "옵션" ) );				
	TempItem.eType	= HELP_IMAGE;
	TempItem.nX		= 34;
	TempItem.nY		= 108;
	TempItem.nWidth	= 150;
	TempItem.nHeight= 254;
	TempItem.rtImage.SetUV( 0, 0, 150, 249, fImageTexWidth, fImageTexHeight );	
	AddPageItem( iPageID, TempItem );
	{
		sPageItem TempItem;
		TempItem.eType	= HELP_LIST;
		TempItem.lbListBox.Create( this, 190, 112, 378, 92, 12, 0, 8, 1, FALSE );
		
		AddReadingString( TempItem.lbListBox, _S( 1606, "옵션창은 Alt+Z 키로 열거나, 메뉴에서 시스템 메뉴를 클릭하여" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1607, "열 수 있습니다. " ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, CTString( "  " ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1608, "옵션 창에서는 게임에 직접적으로 관련된 설정과 비디오, 오디오" ), 0xabababFF );		
		AddReadingString( TempItem.lbListBox, _S( 1609, "등에 관한 설정을 플레이어의 편의에 따라 조정할 수 있습니다." ), 0xabababFF );		
		AddPageItem( iPageID, TempItem );
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::ToggleVisible()
{
	if( IsVisible() )
	{
		CloseHelp();
		m_bVisible = FALSE;
	}	
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::OpenHelp()
{
	m_nCurSection	= HELPS_HELP;
	m_nCurBoardType = HELPT_LIST;
	
	SetListContent( 1 );

	_pUIMgr->RearrangeOrder( UI_HELP, TRUE );
}

// ----------------------------------------------------------------------------
// Name : CloseHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::CloseHelp()
{
	_pUIMgr->RearrangeOrder( UI_HELP, FALSE );
}

// ----------------------------------------------------------------------------
// Name : SetPageButtons()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::SetPageButtons( int nTotalWrite, int nCurPage )
{
	m_nTotalPage = ( nTotalWrite + 9 ) / 10;
	m_nCurrentPage = nCurPage;

	m_nCurrentFirstPage = m_nCurrentPage - 5;
	if( m_nCurrentFirstPage <= 1 )
	{
		m_nCurrentFirstPage = 1;
		m_btnPrev.SetEnable( FALSE );
	}
	else
		m_btnPrev.SetEnable( TRUE );

	m_nCurrentLastPage = m_nCurrentFirstPage + 9;
	if( m_nCurrentLastPage >= m_nTotalPage )
	{
		m_nCurrentLastPage = m_nTotalPage;
		m_btnNext.SetEnable( FALSE );
	}
	else
		m_btnNext.SetEnable( TRUE );

	int	nLength, nTotalLength = 0;
	for( int iPage = m_nCurrentFirstPage; iPage <= m_nCurrentLastPage; iPage++ )
	{
		if( iPage > 99999 )		nLength = 6;
		else if( iPage > 9999 )	nLength = 5;
		else if( iPage > 999 )	nLength = 4;
		else if( iPage > 99 )	nLength = 3;
		else if( iPage > 9 )	nLength = 2;
		else nLength = 1;

		nTotalLength += nLength + 3;
	}

	int	nFontWidth	= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int	nWidth		= nTotalLength * nFontWidth;
	int	nX			= ( m_nWidth - nWidth ) / 2;
	int	nBtnIndex	= 0;
	CTString	strText;

	if( m_btnPrev.IsEnabled() )
		m_btnPrev.SetPos( nX - m_btnPrev.GetWidth() - 13, HELPOLD_PAGE_BUTTON_SY +
							( _pUIFontTexMgr->GetFontHeight() - m_btnPrev.GetHeight() ) / 2 );

	for( iPage = m_nCurrentFirstPage; iPage <= m_nCurrentLastPage; iPage++ )
	{
		strText.PrintF( "[%d]", iPage );
		m_btnPage[nBtnIndex].SetEnable( TRUE );
		nLength = m_btnPage[nBtnIndex].SetText( strText ) + 1;
		m_btnPage[nBtnIndex].SetPos( nX, HELPOLD_PAGE_BUTTON_SY );

		nX += nLength * nFontWidth;
		nBtnIndex++;
	}

	if( m_btnNext.IsEnabled() )
		m_btnNext.SetPos( nX + 6 - _pUIFontTexMgr->GetFontSpacing(), HELPOLD_PAGE_BUTTON_SY +
							( _pUIFontTexMgr->GetFontHeight() - m_btnPrev.GetHeight() ) / 2 );

	m_nValidPageBtnCount = nBtnIndex;
	m_btnPage[m_nCurrentPage - m_nCurrentFirstPage].SetEnable( FALSE );
}

// ----------------------------------------------------------------------------
// Name : AddListSubjectString()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::AddListSubjectString( CTString &strSubject )
{
	CTString	strTemp = strSubject;

	INDEX	nLength = strTemp.Length();
	if( nLength > HELPOLD_LIST_SUBJECT_MAX_CHAR )
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = HELPOLD_LIST_SUBJECT_MAX_CHAR - 4;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strTemp[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Trim right
		strTemp.TrimRight( nSplitPos );
		strTemp += CTString( "..." );
	}

	m_lbListContent.AddString( 1, strTemp, 0xE6E6E6FF );
}

// ----------------------------------------------------------------------------
// Name : AddReadingString()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar )
{
	// Get length of string
	INDEX	nLength = strContent.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strContent);
		INDEX	nChatMax= (iMaxChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				lbList.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strContent,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddReadingString( lbList, strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= iMaxChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				lbList.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddReadingString( lbList, strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RenderListCommon()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::RenderListCommon()
{
	int	nX, nY;

	// Title region of list
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcListTitle.Left, m_nPosY + m_rcListTitle.Top,
										m_nPosX + m_rcListTitle.Right, m_nPosY + m_rcListTitle.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Splitter - small
	nX = m_nPosX + HELPOLD_LIST_SPLIT_SX;
	nY = m_nPosY + HELPOLD_LIST_SPLITS_SY;	
	for( int iList = 0; iList < HELPOLD_MAX_LIST - 1; iList++ )
	{
		
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + HELPOLD_SPLIT_WIDTH, nY + 1,
											m_rtSplitterS.U0, m_rtSplitterS.V0, m_rtSplitterS.U1, m_rtSplitterS.V1,
											0xFFFFFFFF );
		nY += HELPOLD_LIST_MAIN_OFFSETY;
	}

	// Splitter - large
	nY = m_nPosY + HELPOLD_LIST_SPLITL_SY;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + HELPOLD_SPLIT_WIDTH, nY + 2,
										m_rtSplitterL.U0, m_rtSplitterL.V0, m_rtSplitterL.U1, m_rtSplitterL.V1,
										0xFFFFFFFF );

	// Page buttons
	for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
		m_btnPage[iPage].Render();

	// List box
	m_lbListContent.Render();

	// Text in list type
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "No" ), m_nPosX + HELPOLD_LIST_TITLE_NO_SX,
										m_nPosY + HELPOLD_LIST_TITLE_SY, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 341, "제목" ), m_nPosX + HELPOLD_LIST_TITLE_SUBJECT_SX,
										m_nPosY + HELPOLD_LIST_TITLE_SY, 0xFFFFCAFF );	
}

// ----------------------------------------------------------------------------
// Name : RenderReadCommon()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::RenderReadCommon()
{
	int	nX, nY, nX2, nY2;

	// Content region
	nX = m_nPosX + m_rcReadContent.Left;
	nX2 = m_nPosX + m_rcReadContent.Right;
	// Upper left
	nY = m_nPosY + m_rcReadContent.Top;
	nY2 = nY + 3;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxUL.U0, m_rtEditBoxUL.V0, m_rtEditBoxUL.U1, m_rtEditBoxUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxUM.U0, m_rtEditBoxUM.V0, m_rtEditBoxUM.U1, m_rtEditBoxUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxUR.U0, m_rtEditBoxUR.V0, m_rtEditBoxUR.U1, m_rtEditBoxUR.V1,
										0xFFFFFFFF );
	// Middle left
	nY = m_nPosY + m_rcReadContent.Bottom - 3;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY2, nX + 3, nY,
										m_rtEditBoxML.U0, m_rtEditBoxML.V0, m_rtEditBoxML.U1, m_rtEditBoxML.V1,
										0xFFFFFFFF );
	// Middle middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 3, nY2, nX2 - 3, nY,
										m_rtEditBoxMM.U0, m_rtEditBoxMM.V0, m_rtEditBoxMM.U1, m_rtEditBoxMM.V1,
										0xFFFFFFFF );
	// Middle right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 3, nY2, nX2, nY,
										m_rtEditBoxMR.U0, m_rtEditBoxMR.V0, m_rtEditBoxMR.U1, m_rtEditBoxMR.V1,
										0xFFFFFFFF );
	// Lower left
	nY2 = m_nPosY + m_rcReadContent.Bottom;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxLL.U0, m_rtEditBoxLL.V0, m_rtEditBoxLL.U1, m_rtEditBoxLL.V1,
										0xFFFFFFFF );
	// Lower middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxLM.U0, m_rtEditBoxLM.V0, m_rtEditBoxLM.U1, m_rtEditBoxLM.V1,
										0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxLR.U0, m_rtEditBoxLR.V0, m_rtEditBoxLR.U1, m_rtEditBoxLR.V1,
										0xFFFFFFFF );

	// Subject region of read
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcReadSubject.Left, m_nPosY + m_rcReadSubject.Top,
										m_nPosX + m_rcReadSubject.Right, m_nPosY + m_rcReadSubject.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Splitter - small
	nX = m_nPosX + HELPOLD_READ_SPLIT_SX;
	nY = m_nPosY + HELPOLD_READ_SPLITS_SY;	
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + HELPOLD_SPLIT_WIDTH, nY + 1,
										m_rtSplitterS.U0, m_rtSplitterS.V0, m_rtSplitterS.U1, m_rtSplitterS.V1,
										0xFFFFFFFF );
	// Splitter - large
	nY = m_nPosY + HELPOLD_READ_SPLITL_SY;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + HELPOLD_SPLIT_WIDTH, nY + 2,
										m_rtSplitterL.U0, m_rtSplitterL.V0, m_rtSplitterL.U1, m_rtSplitterL.V1,
										0xFFFFFFFF );

	// Text in read type
	_pUIMgr->GetDrawPort()->PutTextEx( m_strReadNo, m_nPosX + HELPOLD_READ_NO_SX,
											m_nPosY + HELPOLD_READ_SUBJECT_SY, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( m_strReadSubject, m_nPosX + HELPOLD_READ_SUBJECT_SX,
											m_nPosY + HELPOLD_READ_SUBJECT_SY, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextExRX( m_strReadName, m_nPosX + HELPOLD_READ_NAME_RX,
											m_nPosY + HELPOLD_READ_SUBJECT_SY, 0xFFFFCAFF );

	m_btnPrevContent.Render();
	m_btnNextContent.Render();
}

// ----------------------------------------------------------------------------
// Name : RenderPage()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::RenderPage()
{	
	sPage& TempPage		= m_vectorPages[m_nCurPage];

	if( TempPage.vectorPageItem.size() <= 0 )
		return;

	std::vector<sPageItem>::const_iterator itend = TempPage.vectorPageItem.end();

	// FIXME : 이미지 데이터가 있을 경우에만...
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdImageTexture );
	int nX = m_nPosX;
	int nY = m_nPosY;
	for( std::vector<sPageItem>::iterator it = TempPage.vectorPageItem.begin(); it != itend; ++it )
	{
		if( (*it).eType == HELP_IMAGE )
		{
			_pUIMgr->GetDrawPort()->AddTexture( nX + (*it).nX, nY + (*it).nY, nX + (*it).nX + (*it).nWidth, nY + (*it).nY + (*it).nHeight,
										(*it).rtImage.U0, (*it).rtImage.V0, (*it).rtImage.U1, (*it).rtImage.V1,
										0xFFFFFFFF );
		}
	}
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	for( it = TempPage.vectorPageItem.begin(); it != itend; ++it )
	{
		if( (*it).eType == HELP_TEXT )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( (*it).strText, m_nPosX + (*it).nX,
											m_nPosY + (*it).nY, (*it).crText );

		}
		else if( (*it).eType == HELP_LIST )
		{
			(*it).lbListBox.Render();
		}
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::Render()
{	
	m_bVisible = TRUE;
	// Set web board texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 28;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtTitleUL.U0, m_rtTitleUL.V0, m_rtTitleUL.U1, m_rtTitleUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtTitleUM.U0, m_rtTitleUM.V0, m_rtTitleUM.U1, m_rtTitleUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtTitleUR.U0, m_rtTitleUR.V0, m_rtTitleUR.U1, m_rtTitleUR.V1,
										0xFFFFFFFF );

	nY = nY2;
	nY2 = nY2 + 10;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 15;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );	

	// Close button
	m_btnClose.Render();

	// List
	if( m_nCurBoardType == HELPT_LIST )
	{
		RenderListCommon();		
	}
	// Read
	else if( m_nCurBoardType == HELPT_READ )
	{
		RenderReadCommon();

		// List button
		m_btnList.Render();
	}	

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	if( m_nCurBoardType == HELPT_READ )
		RenderPage();

	// Text in web board
	// Title
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1610, "도움말" ), m_nPosX + HELPOLD_TITLE_OFFSETX,		
										m_nPosY + HELPOLD_TITLE_OFFSETY, 0xFFFFFFFF );	

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();	
}

// ----------------------------------------------------------------------------
// Name : AddPage()
// Desc :
// ----------------------------------------------------------------------------
int CUIHelpOld::AddPage( const CTString &strPageTitle )
{
	sPage				TempPage;
	TempPage.iIndex		= m_vectorPages.size();
	TempPage.strTitle	= strPageTitle;
	m_vectorPages.push_back( TempPage );
	return TempPage.iIndex;
}

// ----------------------------------------------------------------------------
// Name : AddPageItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::AddPageItem( int iPage, sPageItem& NewPageItem )
{
	sPage	TempPage;
	TempPage.iIndex	= iPage;
	std::vector<sPage>::iterator it = std::find_if( m_vectorPages.begin(), m_vectorPages.end(), FindPage(TempPage) );
	
	// 찾았음.
	if( it != m_vectorPages.end() )
	{
		(*it).vectorPageItem.push_back( NewPageItem );
	}
	// 없는 페이지임...
	else
	{
		ASSERTALWAYS( "Invalid Page Index!!!" );
	}
}

// ----------------------------------------------------------------------------
// Name : SetListContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::SetListContent( int iPage )
{
	m_nCurBoardType = HELPT_LIST;

	// Reset list box
	m_lbListContent.ResetAllStrings();
		
	int nTotalWrite		= m_vectorPages.size();
	int nCurPageNo		= iPage;
	int lastPageNo		= INDEX((nTotalWrite-1) * 0.1f) + 1;
	int	nCurrentListCount;
	if(nCurPageNo == lastPageNo)
	{
		nCurrentListCount = nTotalWrite % 10;
		nCurrentListCount = nCurrentListCount == 0 ? 10 : nCurrentListCount;
	}
	else
		nCurrentListCount = 10;
	
	for( int i = 0; i < nCurrentListCount; i++ )
	{	
		const int iCurPage	= (nCurPageNo - 1) * 10 + i;
		sPage &TempPage		= m_vectorPages[ iCurPage ];
		m_nListIndex[i]		= TempPage.iIndex;		

		//number
		CTString strTemp;
		strTemp.PrintF( "%d", TempPage.iIndex );
		m_lbListContent.AddString( 0, strTemp, 0xE6E6E6FF );		

		//title		
		AddListSubjectString( TempPage.strTitle );
	}
	SetPageButtons( nTotalWrite, nCurPageNo );
}

// ----------------------------------------------------------------------------
// Name : SetViewContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpOld::SetViewContent( int iIndex )
{
	m_nCurBoardType = HELPT_READ;

	if(m_nCurSection == HELPS_HELP )
	{
		const int iCurPage	= (m_nWantPage - 1) * 10 + iIndex;
		m_nCurPage			= iCurPage;

		sPage& TempPage		= m_vectorPages[iCurPage];

		//notice no		
		m_strReadNo.PrintF( CTString( "No.%d" ), TempPage.iIndex );

		//title		
		m_strReadSubject = TempPage.strTitle;

		//content
		//m_lbReadContent.ResetAllStrings();
		//AddReadingString( CTString( "Content Testing" ) );

		if( m_nWantPage <= 1 && iIndex == 0 )
			m_btnPrevContent.SetEnable( FALSE );
		else
		{
			m_btnPrevContent.SetEnable( TRUE );
		}

		if( (iCurPage + 1) == m_vectorPages.size() )
		{
			m_btnNextContent.SetEnable( FALSE );
		}
		else
		{
			m_btnNextContent.SetEnable( TRUE );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelpOld::KeyMessage( MSG *pMsg )
{
	// If web board is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if(m_bWaitResponseMode) return WMSG_FAIL;

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelpOld::CharMessage( MSG *pMsg )
{
	if(m_bWaitResponseMode) return WMSG_FAIL;
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIHelpOld::IMEMessage( MSG *pMsg )
{
	if(m_bWaitResponseMode) return WMSG_FAIL;
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelpOld::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if(m_bWaitResponseMode && !m_bVisible) return WMSG_FAIL;
	if(m_bWaitResponseMode && m_bVisible)
	{
		switch( pMsg->message )
		{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					_pUIMgr->SetMouseCursorInsideUIs();

				// Move web board
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;

					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}
					_pUIMgr->RearrangeOrder( UI_HELP, TRUE );
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
					bTitleBarClick = FALSE;

					// If teleport isn't focused
					if( !IsFocused() )
						return WMSG_FAIL;

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
					return WMSG_SUCCESS;
			}
			break;
		}

		return WMSG_FAIL;
	}

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
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
			// Notice
			else if( m_nCurSection == HELPS_HELP )
			{
				// List
				if( m_nCurBoardType == HELPT_LIST )
				{
					// List box
					if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Prev button
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Next button
					else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
						{
							if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Read
				else if( m_nCurBoardType == HELPT_READ )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if( m_btnPrevContent.MouseMessage( pMsg ) != WMSG_FAIL )					
						return WMSG_SUCCESS;					
					else if( m_btnNextContent.MouseMessage( pMsg ) != WMSG_FAIL )					
						return WMSG_SUCCESS;					
				}
			}			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				//_pUIMgr->RearrangeOrder( UI_HELP, TRUE );
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
				// Notice
				else if( m_nCurSection == HELPS_HELP )
				{
					// List
					if( m_nCurBoardType == HELPT_LIST )
					{
						// List box
						if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								m_nWantWrite = m_lbListContent.GetCurSel();
								
								SetViewContent( m_nWantWrite );
							}
						}
						// Prev button
						else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Next button
						else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
									break;
							}
						}
					}
					// Read
					else if( m_nCurBoardType == HELPT_READ )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						else if( m_btnPrevContent.MouseMessage( pMsg ) != WMSG_FAIL )
						{
						}
						else if( m_btnNextContent.MouseMessage( pMsg ) != WMSG_FAIL )
						{
						}
					}
				}
				_pUIMgr->RearrangeOrder( UI_HELP, TRUE );
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
				bTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseHelp();

					return WMSG_SUCCESS;
				}
				// Notice
				else if( m_nCurSection == HELPS_HELP )
				{
					// List
					if( m_nCurBoardType == HELPT_LIST )
					{
						// List box
						if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Prev button
						else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
								m_nWantPage = m_nCurrentFirstPage - 10;
								if(m_nWantPage < 1) m_nWantPage = 1;								
								SetListContent( m_nWantPage );
							}
							return WMSG_SUCCESS;
						}
						// Next button
						else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
								m_nWantPage = m_nCurrentPage + 10;
								if(m_nWantPage > m_nTotalPage) m_nWantPage = m_nTotalPage;								
								SetListContent( m_nWantPage );
							}
							return WMSG_SUCCESS;
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( ( wmsgResult = m_btnPage[iPage].MouseMessage( pMsg ) ) != WMSG_FAIL )
								{
									if( wmsgResult == WMSG_COMMAND )
									{
										// 페이지 변경 처리
										m_nWantPage = m_nCurrentFirstPage + iPage;
										SetListContent( m_nWantPage );
									}
									return WMSG_SUCCESS;
								}
							}
						}
					}
					// Read
					else if( m_nCurBoardType == HELPT_READ )
					{
						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
								m_nWantPage = m_nCurrentPage;
								SetListContent( m_nWantPage );								
							}
							return WMSG_SUCCESS;
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						else if( ( wmsgResult = m_btnPrevContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								m_nWantWrite--;
								if( m_nWantWrite < 0 )
								{
									m_nWantWrite = 9;
									m_nWantPage--;
									m_nCurrentPage = m_nWantPage;
								}
								//m_lbListContent.SetCurSel( m_nWantWrite );
								SetViewContent( m_nWantWrite );
							}
							return WMSG_SUCCESS;
						}
						else if( ( wmsgResult = m_btnNextContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								m_nWantWrite++;
								if( m_nWantWrite >= 10 )
								{
									m_nWantWrite = 0;
									m_nWantPage++;
									m_nCurrentPage = m_nWantPage;
								}

								//m_lbListContent.SetCurSel( m_nWantWrite );								
								SetViewContent( m_nWantWrite );
							}
							return WMSG_SUCCESS;
						}
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
				// Notice
				if( m_nCurSection == HELPS_HELP )
				{
					// Read
					if( m_nCurBoardType == HELPT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Notice
				if( m_nCurSection == HELPS_HELP )
				{
					// Read
					if( m_nCurBoardType == HELPT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}				
			}
		}
		break;
	}

	return WMSG_FAIL;
}