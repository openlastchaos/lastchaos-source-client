#include "stdh.h"
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/LocalDefine.h>

// const value				
#define MAX_SIGNAL					3	// 화면에 표시되는 최대 Signal 수 

// Position offset of controls
#define	MAP_CLOSE_X					32
#define	MAP_CLOSE_Y					5
#define	MAP_OPACITY_X				227 + 20
#define	MAP_OPACITY_Y				26
#define	MAP_ZOOM_Y					38
#define	MAP_COMBO_X					145 + 20
#define	MAP_COMBO_Y					27
#define	MAP_SLIDEINFO_X				232	+ 20
#define	MAP_SLIDEINFO_Y				29
#define MAP_SIGNAL_X				MAP_OPACITY_X - 20
#define	MAP_SIGNAL_Y				26

bool CompareZ( CastleData first, CastleData second )
{
	return ( first.fZ < second.fZ );
}


extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIMap()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMap::CUIMap()
{
	m_bShowWorldMap = TRUE;
	m_bShowFrame = TRUE;
	m_bFrameBlending = FALSE;
	m_bMapBlendingForDetail = FALSE;
	m_bMapBlendingForLayer = FALSE;
	m_bCanScrollMapWorld = FALSE;
	m_bCanScrollMapDetail = FALSE;
	m_llFrameBlendTime = 0;
	m_llFrameBlendElapsedTime = 0;
	m_llMapBlendTimeForDetail = 0;
	m_llMapBlendTimeForLayer = 0;
	m_llMapBlendElapsedTimeForLayer = 0;

	m_sbCurLayer = 0;
	m_nCurMapData = -1;
	m_nCurDetail = -1;
	m_fZoomWorld = 1.0f;
	m_fZoomDetail = 1.0f;
	m_fMapOpacity = 1.0f;
	m_nResizeWidthOffset = 0;
	m_nResizeHeightOffset = 0;
	m_bShowOpacitySlideInfo = FALSE;
	m_bShowZoomSlideInfo = FALSE;
	m_strOpacitySlideInfo = CTString( "" );
	m_strZoomSlideInfo = CTString( "" );
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );

	m_ptdWorldTex[0] = NULL;
	m_ptdWorldTex[1] = NULL;
	m_ptdDetailTex = NULL;

	m_bSignalBtnOn = FALSE;
	m_bInsideMouse = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMap()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMap::~CUIMap()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 512, 22 );
	m_rcResizeLeft.SetRect( 0, 22, 10, m_nHeight - 10 );
	m_rcResizeRight.SetRect( m_nWidth - 10, 22, m_nWidth, m_nHeight - 10 );
	m_rcResizeBottomL.SetRect( 0, m_nHeight - 10, 10, m_nHeight );
	m_rcResizeBottomM.SetRect( 10, m_nHeight - 10, m_nWidth - 10, m_nHeight );
	m_rcResizeBottomR.SetRect( m_nWidth - 10, m_nHeight - 10, m_nWidth, m_nHeight );
	m_rcPC.SetRect( -13, -13, 13, 13 );
	m_rcNPC.SetRect( -6, -9, 6, 8 );
#ifdef HELP_SYSTEM_1
	m_rcHelpNpc.SetRect( -6, -9, 6, 8);
	m_rcQuestNpc.SetRect(-6, -6, 6, 6);
	m_rcSuccessNpc.SetRect(-6, -6, 6, 6);
#endif
	m_rcParty.SetRect( -6, -6, 5, 5 );
	m_rcCompass.SetRect( 5, -97, 80, -5 );
	m_rcSubZone[VILLAGE].SetRect( -16, -18, 16, 18 );
	m_rcSubZone[DUNGEON].SetRect( -16, -18, 16, 18 );
	m_rcSubZone[CHARGE_PRIVATE].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[CHARGE_PUBLIC].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[MINE_PRIVATE].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[MINE_PUBLIC].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[GATHER_PRIVATE].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[GATHER_PUBLIC].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[CASTLE_GATE].SetRect( -9, -9, 9, 9 );//!!

	// 공성 관련 추가 +
	m_rcCastle[CASTLE_LORD].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_REGEN].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_TOWER].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_SIGNAL].SetRect( -17, -17, 16, 16 );//!!
	
	m_rcViewportWorld.SetRect( 0, 0, MAP_FRAME_BASE_WIDTH, MAP_FRAME_BASE_HEIGHT );
	m_rcViewportDetail.SetRect( 0, 0, MAP_FRAME_BASE_WIDTH, MAP_FRAME_BASE_HEIGHT );

	// Create map frame texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Map.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 34, 22, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 37, 0, 48, 22, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 51, 0, 84, 22, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 25, 34, 30, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 37, 25, 48, 30, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 51, 25, 84, 30, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 33, 34, 48, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 37, 33, 48, 48, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 51, 33, 84, 48, fTexWidth, fTexHeight );
	m_rtBackListBar.SetUV( 89, 2, 102, 16, fTexWidth, fTexHeight );
	m_rtPC.SetUV( 81, 65, 107, 91, fTexWidth, fTexHeight );
	m_rtNPC.SetUV( 68, 65, 80, 82, fTexWidth, fTexHeight );
#ifdef HELP_SYSTEM_1
	if ( g_iCountry == USA ) //게이트가 추가되면서 Map.tex텍스처가 바뀜
	{ // 이것이 최신 텍스처 좌표
		m_rtHelpNpc.SetUV( 32, 130, 44, 148, fTexWidth, fTexHeight );
		m_rtQuestNpc.SetUV( 6, 122, 20, 136, fTexWidth, fTexHeight );
		m_rtSuccessNpc.SetUV( 6, 140, 20, 154, fTexWidth, fTexHeight);
	}
	else
	{
		m_rtHelpNpc.SetUV( 127, 64, 140, 82, fTexWidth, fTexHeight );
		m_rtQuestNpc.SetUV( 190, 24, 206, 41, fTexWidth, fTexHeight );
		m_rtSuccessNpc.SetUV( 172, 24, 189, 41, fTexWidth, fTexHeight);
	}
#endif
	m_rtParty.SetUV( 108, 67, 119, 78, fTexWidth, fTexHeight );
	m_rtSubZone[VILLAGE].SetUV( 67, 91, 99, 127, fTexWidth, fTexHeight );
	m_rtSubZone[DUNGEON].SetUV( 67, 91, 99, 127, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PRIVATE].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PUBLIC].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PRIVATE].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PUBLIC].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PRIVATE].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PUBLIC].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[CASTLE_GATE].SetUV( 170, 10, 188, 28, fTexWidth, fTexHeight );//!!
#ifdef HELP_SYSTEM_1
	m_rtCompass.SetUV( 179, 42, 254, 124, fTexWidth, fTexHeight );
	m_rtCastle[CASTLE_LORD].SetUV( 208, 4, 226, 24, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_REGEN].SetUV( 230, 4, 248, 24, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_TOWER].SetUV( 189, 4, 207, 24, fTexWidth, fTexHeight );//!!
#else
	m_rtCompass.SetUV( 179, 32, 254, 124, fTexWidth, fTexHeight );
	m_rtCastle[CASTLE_LORD].SetUV( 208, 10, 226, 28, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_REGEN].SetUV( 230, 10, 248, 28, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_TOWER].SetUV( 189, 10, 207, 28, fTexWidth, fTexHeight );//!!
#endif
	m_rtCastle[CASTLE_SIGNAL].SetUV( 143, 46, 176, 79, fTexWidth, fTexHeight );//!!

	// Tool tip
	m_rtToolTipL.SetUV( 108, 6, 115, 28, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 118, 6, 120, 28, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 123, 6, 130, 28, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 486, 5, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 31, 97, 45, 111, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 46, 97, 60, 111, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnSignal.Create( this, CTString( "" ), m_nWidth - MAP_SIGNAL_X, MAP_SIGNAL_Y, 19, 19 );
	m_btnSignal.SetUV( UBS_IDLE, 108, 29, 128, 48, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_CLICK, 0, 96, 19, 115, fTexWidth, fTexHeight );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Map combo box
	m_cmbMap.Create( this, 373, 27, 120, 15, 126-20, 1, 13, 13,
						8, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbMap.SetBackUV( 0, 49, 140, 64, fTexWidth, fTexHeight );
	m_cmbMap.SetDownBtnUV( 31, 112, 44, 125, fTexWidth, fTexHeight );
	m_cmbMap.SetUpBtnUV( 45, 112, 58, 125, fTexWidth, fTexHeight );	
	m_cmbMap.SetDropListUV( 0, 49, 140, 64, fTexWidth, fTexHeight );
	m_cmbMap.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	// Up button
	m_cmbMap.SetScrollUpUV( UBS_IDLE, 41, 67, 50, 74, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollUpUV( UBS_CLICK, 51, 67, 60, 74, fTexWidth, fTexHeight );
	m_cmbMap.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cmbMap.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cmbMap.SetScrollBarTopUV( 31, 67, 40, 77, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollBarMiddleUV( 31, 78, 40, 80, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollBarBottomUV( 31, 81, 40, 91, fTexWidth, fTexHeight );
	// Down button
	m_cmbMap.SetScrollDownUV( UBS_IDLE, 41, 76, 50, 83, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollDownUV( UBS_CLICK, 51, 76, 60, 83, fTexWidth, fTexHeight );
	m_cmbMap.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cmbMap.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	// Opacity slide bar
	m_slbOpacity.Create( this, 291, 26, 76, 5, 10, 10, 75, 76, 0, 75 );
	m_slbOpacity.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbOpacity.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	// Zooming world map slide bar
	m_slbZoomWorld.Create( this, 291, 38, 76, 5, 10, 10, 12, 25, 0, 24 );
	m_slbZoomWorld.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbZoomWorld.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	// Zooming detail map slide bar
	m_slbZoomDetail.Create( this, 291, 38, 76, 5, 10, 10, 12, 25, 0, 24 );
	m_slbZoomDetail.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbZoomDetail.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	// Zone name
	CZoneInfo::Instance().SetZoneName( 0, _S( 342, "쥬노" ) );
	CZoneInfo::Instance().SetZoneName( 1, _S( 343, "벨피스트 신전" ) );
	CZoneInfo::Instance().SetZoneName( 2, _S( 344, "스트레이아" ) );
	CZoneInfo::Instance().SetZoneName( 3, _S( 345, "프로키온 신전" ) );
	CZoneInfo::Instance().SetZoneName( 4, _S( 346, "드라탄" ) );
	CZoneInfo::Instance().SetZoneName( 5, _S( 423, "미노크 탄광" ) );
	CZoneInfo::Instance().SetZoneName( 6, _S( 928, "튜토리얼" ) );		
	CZoneInfo::Instance().SetZoneName( 7, _S( 929, "메라크" ) );		
	CZoneInfo::Instance().SetZoneName( 8, _S( 1200, "길드 전용방" ) );	
	CZoneInfo::Instance().SetZoneName( 9, _S( 1201, "마르가둠" ) );		
	CZoneInfo::Instance().SetZoneName( 10, _S( 1201, "마르가둠" ) );		// 새로운 싱글 던젼.
	CZoneInfo::Instance().SetZoneName( 12, _S( 2368,"망각의 신전" ) );		
	CZoneInfo::Instance().SetZoneName( 13, _S( 1201, "마르가둠)") );		// 태국 PK 던전				
	CZoneInfo::Instance().SetZoneName( 14, _S( 3049,"OX이벤트룸") );		 			
	CZoneInfo::Instance().SetZoneName( 15, _S(3050, "에게하") );
	CZoneInfo::Instance().SetZoneName( 17, _S(3050, "에게하") ); // 에게하 던전
	CZoneInfo::Instance().SetZoneName( 23, _S(4136, "스트레이아나") ); // 스트레이아나
	CZoneInfo::Instance().SetZoneName( 24, _S(4199, "PVP 아레나 존") ); // PK토너먼트	
	CZoneInfo::Instance().SetZoneName( 26, _S(4259, "영혼의 동굴")); // 스트레이아나 던전
	CZoneInfo::Instance().SetZoneName( 27, _S(4260, "콰니안 동굴"));
	CZoneInfo::Instance().SetZoneName( 28, _S(4261, "골렘의 동굴"));	
	CZoneInfo::Instance().SetZoneName( 29, _S(4319, "에보니 탄광"));
	CZoneInfo::Instance().SetZoneName( 30, _S(4317, "미스티 협곡"));
	CZoneInfo::Instance().SetZoneName( 31, _S(4395, "플로라임 동굴"));

	CZoneInfo::Instance().SetExtraName( 0, 0, _S( 530, "란돌" ) );						
	CZoneInfo::Instance().SetExtraName( 0, 1, _S( 531, "벨피스트 신전 입구" ) );		
	CZoneInfo::Instance().SetExtraName( 0, 2, _S( 532, "버뱅크 방앗간" ) );			
	CZoneInfo::Instance().SetExtraName( 0, 3, _S( 533, "거목 엔드라실" ) );			
	CZoneInfo::Instance().SetExtraName( 0, 4, _S( 534, "크라우드 풀" ) );				
	//CZoneInfo::Instance().SetExtraName( 0, 5, _S( 535, "미네랄 포트" ) );				
	//CZoneInfo::Instance().SetExtraName( 0, 6, _S( 536, "크락 가든" ) );				
	CZoneInfo::Instance().SetExtraName( 0, 5, _S( 4318, "에보니 탄광 입구" ) );
	CZoneInfo::Instance().SetExtraName( 1, 0, _S( 531, "벨피스트 신전 입구" ) );		
	CZoneInfo::Instance().SetExtraName( 2, 0, _S( 537, "스트레이아 입구" ) );			
	CZoneInfo::Instance().SetExtraName( 3, 0, _S( 538, "프로키온 신전 입구" ) );		
	CZoneInfo::Instance().SetExtraName( 3, 1, _S( 538, "프로키온 신전 입구" ) );		
	CZoneInfo::Instance().SetExtraName( 4, 0, _S( 539, "마자르" ) );					
	CZoneInfo::Instance().SetExtraName( 4, 1, _S( 538, "프로키온 신전 입구" ) );		
	CZoneInfo::Instance().SetExtraName( 4, 2, _S( 540, "하이잘 에너지 클라스터" ) );	
	CZoneInfo::Instance().SetExtraName( 4, 3, _S( 541, "솔로우 에너지 클라스터" ) );	
	CZoneInfo::Instance().SetExtraName( 4, 4, _S( 542, "므디카 광산" ) );				
	CZoneInfo::Instance().SetExtraName( 4, 5, _S( 543, "켈케다 광산" ) );				
	CZoneInfo::Instance().SetExtraName( 4, 6, _S( 544, "세렐 농장" ) );				
	CZoneInfo::Instance().SetExtraName( 4, 7, _S( 545, "고난다 농장" ) );				
	CZoneInfo::Instance().SetExtraName( 4, 8, _S( 546, "알비타 신전 입구" ) );			
	CZoneInfo::Instance().SetExtraName( 4, 9, _S( 547, "레굴루스 토템바" ) );
	CZoneInfo::Instance().SetExtraName( 4, 10, _S(2369, "망각의 신전 입구" ) );
	CZoneInfo::Instance().SetExtraName( 4, 29, _S(3946, "테오스의 무덤 입구" ) );
	CZoneInfo::Instance().SetExtraName( 5, 0, _S( 548, "미노크 탄광 입구" ) );	

	// Date : 2005-02-04,   By Lee Ki-hwan
	CZoneInfo::Instance().SetExtraName( 7, 0, _S( 930, "메라크 캐론" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 1, _S( 931, "드레드가든" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 2, _S( 931, "드레드가든" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 3, CTString( "" ) );					
	CZoneInfo::Instance().SetExtraName( 7, 4, _S( 932, "마그마리버" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 5, _S( 932, "마그마리버" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 6, _S( 933, "미스틱 스웜프" ) );	
	CZoneInfo::Instance().SetExtraName( 7, 7, _S( 934, "오울 빌리지" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 8, _S( 935, "어비스 던전" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 9, _S( 936, "덴오브피어" ) );		
	CZoneInfo::Instance().SetExtraName( 7, 10, CTString( "" ) );
	CZoneInfo::Instance().SetExtraName( 7, 19, _S(4316, "미스티 협곡 입구") );

	CZoneInfo::Instance().SetExtraName( 8, 0, _S( 1200, "길드 전용방" ) );			

	CZoneInfo::Instance().SetExtraName( 9, 0, _S( 1202, "마르가둠 입구" ) );				
	CZoneInfo::Instance().SetExtraName( 12, 0, _S(2370, "망각의 신전 내부" ) );				
	// wooss 051213 thai pk-zone
	CZoneInfo::Instance().SetExtraName( 13, 0,CTString("Free PK Zone"));			// 번역 
	CZoneInfo::Instance().SetExtraName( 13, 1,CTString("Free PK Zone"));			// 번역 
	CZoneInfo::Instance().SetExtraName( 13, 2,CTString("Ranking PK Zone"));			// 번역 

	CZoneInfo::Instance().SetExtraName( 15, 0, _S(3051, "에게하 아론" ) );
	CZoneInfo::Instance().SetExtraName( 15, 1, _S(3182, "투스트 트룸 입구" ) );
	CZoneInfo::Instance().SetExtraName( 15, 2, _S(4076, "에게하 검은 늪 관문" ) );
	CZoneInfo::Instance().SetExtraName( 15, 3, _S(4396, "플로라임 동굴 입구" ) );

	CZoneInfo::Instance().SetExtraName( 17, 0, _S(3174, "루스트 투룸 내부" ) );
	CZoneInfo::Instance().SetExtraName( 23, 0, _S(4137, "검은 안개 마을") );
	CZoneInfo::Instance().SetExtraName( 23, 3, _S(4259, "영혼의 동굴 입구"));
	CZoneInfo::Instance().SetExtraName( 23, 4, _S(4260, "콰니안 동굴 입구"));
	CZoneInfo::Instance().SetExtraName( 23, 5, _S(4261, "골렘의 동굴 입구"));
	CZoneInfo::Instance().SetExtraName( 24, 0, _S(4200, "붉은 모래바람 투기장") ); // PK토너먼트
	CZoneInfo::Instance().SetExtraName( 26, 0, _S(4259, "영혼의 동굴"));
	CZoneInfo::Instance().SetExtraName( 27, 0, _S(4260, "콰니안 동굴"));
	CZoneInfo::Instance().SetExtraName( 28, 0, _S(4261, "골렘의 동굴"));
	CZoneInfo::Instance().SetExtraName( 29, 0, _S(4319, "에보니 탄광"));
	CZoneInfo::Instance().SetExtraName( 30, 0, _S(4317, "미스티 협곡"));
	CZoneInfo::Instance().SetExtraName( 31, 0, _S(4395, "플로라임 동굴"));

	// Read map data
	ReadMapData();
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Destroy()
{
	CUIWindow::Destroy();

	if( m_ptdWorldTex[0] )
	{
		_pTextureStock->Release( m_ptdWorldTex[0] );
		m_ptdWorldTex[0] = NULL;
	}

	if( m_ptdWorldTex[1] )
	{
		_pTextureStock->Release( m_ptdWorldTex[1] );
		m_ptdWorldTex[1] = NULL;
	}

	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	SetSize( MAP_FRAME_WIDTH, MAP_FRAME_HEIGHT );

	m_btnClose.SetPosX( m_nWidth - MAP_CLOSE_X );
	m_slbOpacity.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_slbZoomWorld.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_slbZoomDetail.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_cmbMap.SetPosX( m_nWidth - MAP_COMBO_X );
	m_btnSignal.SetPosX( m_nWidth - MAP_SIGNAL_X );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ReadMapData()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ReadMapData()
{
	if( m_vecMapData.size() > 0  )
		return;

	CTFileName	fnmMapData;
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	fnmMapData = strFullPath + "data\\Map.dta";

	CTFileStream	fsMapData;
	fsMapData.Open_t( fnmMapData );

	CTString	strZoneName;
	int			nZoneCount;
	LONG		lOffsetX;
	LONG		lOffsetZ;

	fsMapData >> nZoneCount;

	for( int i = 0; i < nZoneCount; i++ )
	{
		MapData	tempMapData;
		int		nIndex, nLeft, nTop, nRight, nBottom;
		FLOAT	fRatio, fX, fZ;
		SBYTE	sbLayer;

		fsMapData >> nIndex;
		fsMapData >> sbLayer;
		fsMapData >> nLeft;
		fsMapData >> nTop;
		fsMapData >> nRight;
		fsMapData >> nBottom;
		fsMapData >> fRatio;
		fsMapData >> lOffsetX;
		fsMapData >> lOffsetZ;

		tempMapData.World.nZoneIndex = nIndex;
		tempMapData.World.fRatio = fRatio;
		tempMapData.World.rcMapSize.SetRect( nLeft, nTop, nRight, nBottom );
		tempMapData.World.sbLayer = sbLayer;
		tempMapData.World.lOffsetX = lOffsetX;
		tempMapData.World.lOffsetZ = lOffsetZ;

		SBYTE	sbDetailCount;
		fsMapData >> sbDetailCount;

		for( int j = 0; j < sbDetailCount; j++ )
		{
			DetailMapData	tempDetail;
			int				nSelLeft, nSelTop, nSelRight, nSelBottom;

			fsMapData >> nSelLeft;
			fsMapData >> nSelTop;
			fsMapData >> nSelRight;
			fsMapData >> nSelBottom;
			fsMapData >> nLeft;
			fsMapData >> nTop;
			fsMapData >> nRight;
			fsMapData >> nBottom;
			fsMapData >> fX;
			fsMapData >> fZ;
			fsMapData >> fRatio;

			tempDetail.rcSelRegion.SetRect( nSelLeft, nSelTop, nSelRight, nSelBottom );
			tempDetail.rcMapSize.SetRect( nLeft, nTop, nRight, nBottom );
			tempDetail.fOffsetX = fX;
			tempDetail.fOffsetZ = fZ;
			tempDetail.fRatio = fRatio;

			tempMapData.vecDetail.push_back( tempDetail );
		}

		SBYTE	sbSubZoneCount;
		fsMapData >> sbSubZoneCount;

		for( j = 0; j < sbSubZoneCount; j++ )
		{
			SubZoneData	tempSubZone;
			SBYTE		sbType;

			fsMapData >> nIndex;
			fsMapData >> fX;
			fsMapData >> fZ;
			fsMapData >> sbType;

			tempSubZone.nSubZoneIndex = nIndex;
			tempSubZone.fX = fX;
			tempSubZone.fZ = fZ;
			tempSubZone.sbType = sbType;

			tempMapData.vecSubZone.push_back( tempSubZone );
		}

		int	nNpcCount;
		fsMapData >> nNpcCount;
		
		LONG nYLayer;

		for( j = 0; j < nNpcCount; j++ )
		{
			NpcData	tempNpc;
			
			fsMapData >> nIndex;
			fsMapData >> nYLayer;
			fsMapData >> fX;
			fsMapData >> fZ;

			tempNpc.nIndex = nIndex;
			tempNpc.nYLayer = nYLayer;	
			tempNpc.fX = fX;
			tempNpc.fZ = fZ;
			
			tempMapData.vecNpc.push_back( tempNpc );
		}

		if( ZoneInfo().GetZoneType( tempMapData.World.nZoneIndex ) == ZONE_DUNGEON )
		{
			strZoneName.PrintF( "%s %dF", ZoneInfo().GetZoneName( tempMapData.World.nZoneIndex ),
											( tempMapData.World.sbLayer + 2 ) / 2);
		}
		else
		{
			if( tempMapData.World.sbLayer > 0 )
			{
				strZoneName.PrintF( _S( 3083, "%s 공성지역" ), _pUISWDoc->GetCastleName( tempMapData.World.nZoneIndex ) );
			}
			else
			{
				strZoneName.PrintF( "%s", ZoneInfo().GetZoneName( tempMapData.World.nZoneIndex ) );
			}
		}
			
		tempMapData.World.strName = strZoneName;

		m_vecMapData.push_back( tempMapData );

	
		if( !IsCastleMap( i ) )	
		m_cmbMap.AddString( strZoneName );
	}
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ShowToolTip( BOOL bShow, CTString &strName, int nX, int nY )
{
	if( !bShow )
	{
		m_bShowToolTip = FALSE;
		m_strToolTip.Clear();
		return;
	}

	m_bShowToolTip = TRUE;

	if( !m_strToolTip.IsEqualCaseSensitive( strName ) )
	{
		if( m_bShowWorldMap )
		{
			UIRect	rcTempWorld = m_rcMapWorld;
			rcTempWorld.Scale( m_fZoomWorld );

			nX *= m_fZoomWorld;
			nY *= m_fZoomWorld;
			nX += m_rcViewportWorld.Left - rcTempWorld.Left + m_nPosX + MAP_SX;
			nY += m_rcViewportWorld.Top - rcTempWorld.Top + m_nPosY + MAP_SY;
		}
		else
		{
			UIRect	rcTempDetail = m_rcMapDetail;
			rcTempDetail.Scale( m_fZoomDetail );

			nX *= m_fZoomDetail;
			nY *= m_fZoomDetail;
			nX += m_rcViewportDetail.Left - rcTempDetail.Left + m_nPosX + MAP_SX;
			nY += m_rcViewportDetail.Top - rcTempDetail.Top + m_nPosY + MAP_SY;
		}

		m_strToolTip.PrintF( "%s", strName );

		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nX -= nInfoWidth / 2;

		if( nX < _pUIMgr->GetMinI() )
			nX = _pUIMgr->GetMinI();
		else if( nX + nInfoWidth > _pUIMgr->GetMaxI() )
			nX = _pUIMgr->GetMaxI() - nInfoWidth;

		if( nY - nInfoHeight < _pUIMgr->GetMinJ() )
			nY = _pUIMgr->GetMinJ() + nInfoHeight;
		else if( nY > _pUIMgr->GetMaxJ() )
			nY = _pUIMgr->GetMaxJ();

		m_rcToolTip.SetRect( nX, nY - nInfoHeight, nX + nInfoWidth, nY );
	}
}

// ----------------------------------------------------------------------------
// Name : FindMapData()
// Desc :
// ----------------------------------------------------------------------------
int	CUIMap::FindMapData( int nZoneIndex, SBYTE sbLayer )
{
	for( int i = 0; i < m_vecMapData.size(); i++ )
	{
		if( m_vecMapData[i].World.nZoneIndex == nZoneIndex &&
			m_vecMapData[i].World.sbLayer == sbLayer )
			return i;
	}

	return -1;
}

// ----------------------------------------------------------------------------
// Name : MakeBaseMapSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::MakeBaseMapSize( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		m_nMapWorldBaseWidth = MAP_FRAME_BASE_WIDTH;
		m_nMapWorldBaseHeight = MAP_FRAME_BASE_HEIGHT;

		if( m_nCurMapData != -1 )
		{
			m_bCanScrollMapWorld = FALSE;
			UIRect	rcTempWorld = m_vecMapData[m_nCurMapData].World.rcMapSize;
			if( rcTempWorld.GetWidth() <= m_nMapWorldBaseWidth )
				m_nMapWorldBaseWidth = rcTempWorld.GetWidth();
			else
				m_bCanScrollMapWorld = TRUE;

			if( rcTempWorld.GetHeight() <= m_nMapWorldBaseHeight )
				m_nMapWorldBaseHeight = rcTempWorld.GetHeight();
			else
				m_bCanScrollMapWorld = TRUE;
		}
	}
	else
	{
		m_nMapDetailBaseWidth = MAP_FRAME_BASE_WIDTH;
		m_nMapDetailBaseHeight = MAP_FRAME_BASE_HEIGHT;

		if( m_nCurMapData != -1 && m_nCurDetail != -1 )
		{
			m_bCanScrollMapDetail = FALSE;
			UIRect	rcTempDetail = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;
			if( rcTempDetail.GetWidth() <= m_nMapDetailBaseWidth )
				m_nMapDetailBaseWidth = rcTempDetail.GetWidth();
			else
				m_bCanScrollMapDetail = TRUE;

			if( rcTempDetail.GetHeight() <= m_nMapDetailBaseHeight )
				m_nMapDetailBaseHeight = rcTempDetail.GetHeight();
			else
				m_bCanScrollMapDetail = TRUE;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetCurrentWorldMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::SetCurrentWorldMap( int nWorld, SBYTE sbLayer )
{
	if( m_nCurMapData != -1 && m_vecMapData[m_nCurMapData].World.nZoneIndex == nWorld && m_vecMapData[m_nCurMapData].World.sbLayer == sbLayer )
		return;

	// ********************************************************* //
	//                      임시 추가                            //
	_pNetwork->MyCharacterInfo.fWaterHeight = -100.0f;
	switch( nWorld )
	{
	case 0:
		_pNetwork->MyCharacterInfo.fWaterHeight = 155.688f;	
		break;
	case 4:
		_pNetwork->MyCharacterInfo.fWaterHeight = -1.0f;	// WSS_DRATAN_SIEGEWARFARE 2007/10/16 공성지역 저지대로 인해 수정 99.5f -> -1.0f
		break;
	}
	// ********************************************************* //

	int	nMapDataIndex = FindMapData( nWorld, sbLayer );
	if( nMapDataIndex == -1 && m_nCurMapData == -1 )	// If map window is opened first time and map is not exist, default map(start zone) is set...
		nMapDataIndex = 0;
	if( nMapDataIndex == -1 )							// No map
		return;

	m_bShowWorldMap = TRUE;
	m_sbCurLayer = sbLayer;
	m_nCurMapData = nMapDataIndex;
	//공성 비공성 지역 표시
	m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
	nWorld = m_vecMapData[m_nCurMapData].World.nZoneIndex;

	MakeBaseMapSize( TRUE );
	int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
	int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
	int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
	int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
	m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
	AdjustViewport( 6 );

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdWorldTex[0] )
	{
		_pTextureStock->Release( m_ptdWorldTex[0] );
		m_ptdWorldTex[0] = NULL;
	}
	if( m_ptdWorldTex[1] )
	{
		_pTextureStock->Release( m_ptdWorldTex[1] );
		m_ptdWorldTex[1] = NULL;
	}
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;
	strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex", nWorld, sbLayer );

	m_nCurWorldTex = 0;
	m_ptdWorldTex[0] = CreateTexture( strFileName );
	m_fWorldTexWidth = m_ptdWorldTex[0]->GetPixWidth();
	m_fWorldTexHeight = m_ptdWorldTex[0]->GetPixHeight();

	m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
						m_fWorldTexWidth, m_fWorldTexHeight );
}

// ----------------------------------------------------------------------------
// Name : ChangeMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ChangeMap()
{
	// 공성 용 맵의 경우에는 공성 맵 정보 요청
	if(m_nCurMapData!=-1){
		if( IsPossibleCastleMap() && IsCastleMap( m_nCurMapData ) )
		{
			_pNetwork->SendCastleMapRecent();
		}
	}

	if( m_cmbMap.GetCurSel() == -1 || m_nCurMapData == -1 ||m_nCurMapData == m_cmbMap.GetCurSel() )
		return;
	
	// Change 
	m_qSignalData.clear();
	// 신규필드 추가로 3(메라크 공성지역)이후 +1을 더한다.
	// m_vecMapData 모든 지역 데이터 포함 공성이 아닌경우 공성 지역은 표시에서 제외된다.
	m_nCurMapData	= m_cmbMap.GetCurSel();
	m_nCurMapData	= (!IsPossibleCastleMap()&&m_nCurMapData>=3) ? m_nCurMapData+1:m_nCurMapData;
	int	nZoneIndex	= m_vecMapData[m_nCurMapData].World.nZoneIndex;
	m_sbCurLayer	= m_vecMapData[m_nCurMapData].World.sbLayer;
	m_bShowWorldMap = TRUE;

	MakeBaseMapSize( TRUE );
	int	nLayerDiff = m_sbCurLayer - _pNetwork->MyCharacterInfo.yLayer;
	int	nMapIndex = FindMapData( nZoneIndex, _pNetwork->MyCharacterInfo.yLayer );
	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo &&
		( nMapIndex == -1 || ( nLayerDiff >= -1 && nLayerDiff <= 1 ) ) )
	{
		int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
		m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}
	else
	{
		m_rcMapWorld.SetRect( 0, 0, m_nMapWorldBaseWidth / m_fZoomWorld, m_nMapWorldBaseHeight / m_fZoomWorld );
		AdjustViewport( 1 );
	}

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_fMoveElapsedErrXDetail = 0.0f;
	m_fMoveElapsedErrYDetail = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdWorldTex[0] )
	{
		_pTextureStock->Release( m_ptdWorldTex[0] );
		m_ptdWorldTex[0] = NULL;
	}
	if( m_ptdWorldTex[1] )
	{
		_pTextureStock->Release( m_ptdWorldTex[1] );
		m_ptdWorldTex[1] = NULL;
	}
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;
	strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex", nZoneIndex, m_sbCurLayer );

	m_nCurWorldTex = 0;
	m_ptdWorldTex[0] = CreateTexture( strFileName );
	m_fWorldTexWidth = m_ptdWorldTex[0]->GetPixWidth();
	m_fWorldTexHeight = m_ptdWorldTex[0]->GetPixHeight();

	m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
						m_fWorldTexWidth, m_fWorldTexHeight );
}

// ----------------------------------------------------------------------------
// Name : OpenDetailMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::OpenDetailMap( int nIndex )
{
	if( m_nCurMapData == -1 )
		return;

	if( nIndex >= m_vecMapData[m_nCurMapData].vecDetail.size() )
		return;

	m_nCurDetail = nIndex;
	int		nZoneIndex = m_vecMapData[m_nCurMapData].World.nZoneIndex;
	SBYTE	sbLayer = m_vecMapData[m_nCurMapData].World.sbLayer;

	m_slbZoomDetail.SetCurPos( MAP_ZOOM_SLIDE_ORIGIN );
	m_fZoomDetail = 1.0f;

	MakeBaseMapSize( FALSE );
	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo )
	{
		int	nCX = ( _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX ) *
					m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio / m_fZoomDetail;
		int	nCY = ( _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ ) *
					m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio / m_fZoomDetail;
		int	nHalfW = m_nMapDetailBaseWidth / m_fZoomDetail * 0.5f;
		int	nHalfH = m_nMapDetailBaseHeight / m_fZoomDetail * 0.5f;
		m_rcMapDetail.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}
	else
	{
		m_rcMapDetail.SetRect( 0, 0, m_nMapDetailBaseWidth / m_fZoomDetail, m_nMapDetailBaseHeight / m_fZoomDetail );
		AdjustViewport( 6 );
	}

	m_fMoveElapsedErrXDetail = 0.0f;
	m_fMoveElapsedErrYDetail = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;
	strFileName.PrintF( "Data\\Interface\\Map_World%d%d_%d.tex", nZoneIndex, sbLayer, nIndex );

	m_ptdDetailTex = CreateTexture( strFileName );
	m_fDetailTexWidth = m_ptdDetailTex->GetPixWidth();
	m_fDetailTexHeight = m_ptdDetailTex->GetPixHeight();

	m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
						m_fDetailTexWidth, m_fDetailTexHeight );

	m_bMapBlendingForDetail = TRUE;
	m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

// ----------------------------------------------------------------------------
// Name : CloseDetailMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::CloseDetailMap()
{
	int	nZoneIndex = m_vecMapData[m_nCurMapData].World.nZoneIndex;
	if( _pNetwork->MyCharacterInfo.zoneNo != nZoneIndex )
	{
		m_bMapBlendingForDetail = TRUE;
		m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		return;
	}

	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo )
	{
		int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
		m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	m_bMapBlendingForDetail = TRUE;
	m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

// ----------------------------------------------------------------------------
// Name : ScrollMapWorld()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ScrollMapWorld( int ndX, int ndY )
{
	if( m_nCurMapData == -1 )
		return;

	FLOAT	fdX = (FLOAT)ndX / m_fZoomWorld;
	FLOAT	fdY = (FLOAT)ndY / m_fZoomWorld;

	BOOL	bModified = FALSE;
	UIRect	rcWhole = m_vecMapData[m_nCurMapData].World.rcMapSize;

	// Scroll horizontal
	if( ( ndX < 0 && m_rcMapWorld.Left > rcWhole.Left ) ||
		( ndX > 0 && m_rcMapWorld.Right < rcWhole.Right ) )
	{
		int	nX = (int)( fdX + m_fMoveElapsedErrXWorld );
		if( m_rcMapWorld.Left + nX < rcWhole.Left )
			nX = rcWhole.Left - m_rcMapWorld.Left;
		else if( m_rcMapWorld.Right + nX > rcWhole.Right )
			nX = rcWhole.Right - m_rcMapWorld.Right;
		m_rcMapWorld.Left += nX;
		m_rcMapWorld.Right += nX;
		m_fMoveElapsedErrXWorld += fdX - (FLOAT)nX;
		bModified = TRUE;
	}

	// Scroll vertical
	if( ( ndY < 0 && m_rcMapWorld.Top > rcWhole.Top ) ||
		( ndY > 0 && m_rcMapWorld.Bottom < rcWhole.Bottom ) )
	{
		int	nY = (int)( fdY + m_fMoveElapsedErrYWorld );
		if( m_rcMapWorld.Top + nY < rcWhole.Top )
			nY = rcWhole.Top - m_rcMapWorld.Top;
		else if( m_rcMapWorld.Bottom + nY > rcWhole.Bottom )
			nY = rcWhole.Bottom - m_rcMapWorld.Bottom;
		m_rcMapWorld.Top += nY;
		m_rcMapWorld.Bottom += nY;
		m_fMoveElapsedErrYWorld += fdY - (FLOAT)nY;
		bModified = TRUE;
	}

	if( bModified )
	{
		m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
							m_fWorldTexWidth, m_fWorldTexHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : ScrollMapDetail()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ScrollMapDetail( int ndX, int ndY )
{
	if( m_nCurMapData == -1 || m_nCurDetail == -1 )
		return;

	FLOAT	fdX = (FLOAT)ndX / m_fZoomDetail;
	FLOAT	fdY = (FLOAT)ndY / m_fZoomDetail;

	BOOL	bModified = FALSE;
	UIRect	rcWhole = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;

	// Scroll horizontal
	if( ( ndX < 0 && m_rcMapDetail.Left > rcWhole.Left ) ||
		( ndX > 0 && m_rcMapDetail.Right < rcWhole.Right ) )
	{
		int	nX = (int)( fdX + m_fMoveElapsedErrXDetail );
		if( m_rcMapDetail.Left + nX < rcWhole.Left )
			nX = rcWhole.Left - m_rcMapDetail.Left;
		else if( m_rcMapDetail.Right + nX > rcWhole.Right )
			nX = rcWhole.Right - m_rcMapDetail.Right;
		m_rcMapDetail.Left += nX;
		m_rcMapDetail.Right += nX;
		m_fMoveElapsedErrXDetail += fdX - (FLOAT)nX;
		bModified = TRUE;
	}

	// Scroll vertical
	if( ( ndY < 0 && m_rcMapDetail.Top > rcWhole.Top ) ||
		( ndY > 0 && m_rcMapDetail.Bottom < rcWhole.Bottom ) )
	{
		int	nY = (int)( fdY + m_fMoveElapsedErrYDetail );
		if( m_rcMapDetail.Top + nY < rcWhole.Top )
			nY = rcWhole.Top - m_rcMapDetail.Top;
		else if( m_rcMapDetail.Bottom + nY > rcWhole.Bottom )
			nY = rcWhole.Bottom - m_rcMapDetail.Bottom;
		m_rcMapDetail.Top += nY;
		m_rcMapDetail.Bottom += nY;
		m_fMoveElapsedErrYDetail += fdY - (FLOAT)nY;
		bModified = TRUE;
	}

	if( bModified )
	{
		m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
							m_fDetailTexWidth, m_fDetailTexHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustViewport()
// Desc : nSelRegion - 1: left, 2: right, 3: middle bottom, 4: left bottom
//        5: right bottom, 6: center
// ----------------------------------------------------------------------------
void CUIMap::AdjustViewport( int nSelRegion )
{
	FLOAT	fRatioW = (FLOAT)( m_nWidth - MAP_FRAME_EDGE_HORZ ) / (FLOAT)MAP_FRAME_BASE_WIDTH;
	FLOAT	fRatioH = (FLOAT)( m_nHeight - MAP_FRAME_EDGE_VERT ) / (FLOAT)MAP_FRAME_BASE_HEIGHT;

	UIRect	rcTempWorld, rcTempDetail;
	if( nSelRegion == 1 || nSelRegion == 4 )
	{
		rcTempWorld.SetRect( m_rcMapWorld.Right - m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld,
								m_rcMapWorld.Top,
								m_rcMapWorld.Right,
								m_rcMapWorld.Top + m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld );
		rcTempDetail.SetRect( m_rcMapDetail.Right - m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail,
								m_rcMapDetail.Top,
								m_rcMapDetail.Right,
								m_rcMapDetail.Top + m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail );
	}
	else if( nSelRegion == 6 )
	{
		int	nHalfWidth = m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld * 0.5f;
		int	nHalfHeight = m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld * 0.5f;
		int	nCX = m_rcMapWorld.Left + m_rcMapWorld.GetWidth() / 2;
		int	nCY = m_rcMapWorld.Top + m_rcMapWorld.GetHeight() / 2;
		rcTempWorld.SetRect( nCX - nHalfWidth, nCY - nHalfHeight,
								nCX + nHalfWidth, nCY + nHalfHeight );

		nHalfWidth = m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail * 0.5f;
		nHalfHeight = m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail * 0.5f;
		nCX = m_rcMapDetail.Left + m_rcMapDetail.GetWidth() / 2;
		nCY = m_rcMapDetail.Top + m_rcMapDetail.GetHeight() / 2;
		rcTempDetail.SetRect( nCX - nHalfWidth, nCY - nHalfHeight,
								nCX + nHalfWidth, nCY + nHalfHeight );
	}
	else
	{
		rcTempWorld.SetRect( m_rcMapWorld.Left,
								m_rcMapWorld.Top,
								m_rcMapWorld.Left + m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld,
								m_rcMapWorld.Top + m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld );
		rcTempDetail.SetRect( m_rcMapDetail.Left,
								m_rcMapDetail.Top,
								m_rcMapDetail.Left + m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail,
								m_rcMapDetail.Top + m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail );
	}

	if( m_nCurMapData != -1 )
	{
		int	ndX = 0, ndY = 0;
		UIRect	rcWholeWorld = m_vecMapData[m_nCurMapData].World.rcMapSize;
		if( rcTempWorld.Left < rcWholeWorld.Left )
			ndX = rcWholeWorld.Left - rcTempWorld.Left;
		else if( rcTempWorld.Right > rcWholeWorld.Right )
			ndX = rcWholeWorld.Right - rcTempWorld.Right;

		if( rcTempWorld.Top < rcWholeWorld.Top )
			ndY = rcWholeWorld.Top - rcTempWorld.Top;
		else if( rcTempWorld.Bottom > rcWholeWorld.Bottom )
			ndY = rcWholeWorld.Bottom - rcTempWorld.Bottom;

		rcTempWorld.Left += ndX;
		rcTempWorld.Right += ndX;
		rcTempWorld.Top += ndY;
		rcTempWorld.Bottom += ndY;

		m_bCanScrollMapWorld = FALSE;
		if( rcWholeWorld.GetWidth() <= rcTempWorld.GetWidth() )
		{
			m_rcMapWorld.Left = rcWholeWorld.Left;
			m_rcMapWorld.Right = rcWholeWorld.Right;

			int	nNewWidth = m_rcMapWorld.GetWidth() * m_fZoomWorld;
			m_rcViewportWorld.Left = ( m_nWidth - MAP_FRAME_EDGE_HORZ - nNewWidth ) / 2;
			m_rcViewportWorld.Right = m_rcViewportWorld.Left + nNewWidth;
		}
		else
		{
			m_rcMapWorld.Left = rcTempWorld.Left;
			m_rcMapWorld.Right = rcTempWorld.Right;
			m_rcViewportWorld.Left = 0;
			m_rcViewportWorld.Right = m_nWidth - MAP_FRAME_EDGE_HORZ;
			m_bCanScrollMapWorld = TRUE;
		}

		if( rcWholeWorld.GetHeight() <= rcTempWorld.GetHeight() )
		{
			m_rcMapWorld.Top = rcWholeWorld.Top;
			m_rcMapWorld.Bottom = rcWholeWorld.Bottom;

			int	nNewHeight = m_rcMapWorld.GetHeight() * m_fZoomWorld;
			m_rcViewportWorld.Top = ( m_nHeight - MAP_FRAME_EDGE_VERT - nNewHeight ) / 2;
			m_rcViewportWorld.Bottom = m_rcViewportWorld.Top + nNewHeight;
		}
		else
		{
			m_rcMapWorld.Top = rcTempWorld.Top;
			m_rcMapWorld.Bottom = rcTempWorld.Bottom;
			m_rcViewportWorld.Top = 0;
			m_rcViewportWorld.Bottom = m_nHeight - MAP_FRAME_EDGE_VERT;
			m_bCanScrollMapWorld = TRUE;
		}

		m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
							m_fWorldTexWidth, m_fWorldTexHeight );
	
		if( m_nCurDetail != -1 )
		{
			int	ndX = 0, ndY = 0;
			UIRect	rcWholeDetail = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;

			if( rcTempDetail.Left < rcWholeDetail.Left )
				ndX = rcWholeDetail.Left - rcTempDetail.Left;
			else if( rcTempDetail.Right > rcWholeDetail.Right )
				ndX = rcWholeDetail.Right - rcTempDetail.Right;

			if( rcTempDetail.Top < rcWholeDetail.Top )
				ndY = rcWholeDetail.Top - rcTempDetail.Top;
			else if( rcTempDetail.Bottom > rcWholeDetail.Bottom )
				ndY = rcWholeDetail.Bottom - rcTempDetail.Bottom;

			rcTempDetail.Left += ndX;
			rcTempDetail.Right += ndX;
			rcTempDetail.Top += ndY;
			rcTempDetail.Bottom += ndY;

			m_bCanScrollMapDetail = FALSE;
			if( rcWholeDetail.GetWidth() <= rcTempDetail.GetWidth() )
			{
				m_rcMapDetail.Left = rcWholeDetail.Left;
				m_rcMapDetail.Right = rcWholeDetail.Right;

				int	nNewWidth = m_rcMapDetail.GetWidth() * m_fZoomDetail;
				m_rcViewportDetail.Left = ( m_nWidth - MAP_FRAME_EDGE_HORZ - nNewWidth ) / 2;
				m_rcViewportDetail.Right = m_rcViewportDetail.Left + nNewWidth;
			}
			else
			{
				m_rcMapDetail.Left = rcTempDetail.Left;
				m_rcMapDetail.Right = rcTempDetail.Right;
				m_rcViewportDetail.Left = 0;
				m_rcViewportDetail.Right = m_nWidth - MAP_FRAME_EDGE_HORZ;
				m_bCanScrollMapDetail = TRUE;
			}

			if( rcWholeDetail.GetHeight() < rcTempDetail.GetHeight() )
			{
				m_rcMapDetail.Top = rcWholeDetail.Top;
				m_rcMapDetail.Bottom = rcWholeDetail.Bottom;

				int	nNewHeight = m_rcMapDetail.GetHeight() * m_fZoomDetail;
				m_rcViewportDetail.Top = ( m_nHeight - MAP_FRAME_EDGE_VERT - nNewHeight ) / 2;
				m_rcViewportDetail.Bottom = m_rcViewportDetail.Top + nNewHeight;
			}
			else
			{
				m_rcMapDetail.Top = rcTempDetail.Top;
				m_rcMapDetail.Bottom = rcTempDetail.Bottom;
				m_rcViewportDetail.Top = 0;
				m_rcViewportDetail.Bottom = m_nHeight - MAP_FRAME_EDGE_VERT;
				m_bCanScrollMapDetail = TRUE;
			}

			m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
								m_fDetailTexWidth, m_fDetailTexHeight );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResizeMapFrame()
// Desc : nSelRegion - 1: left, 2: right, 3: middle bottom, 4: left bottom
//        5: right bottom
// ----------------------------------------------------------------------------
void CUIMap::ResizeMapFrame( int nSelRegion, int ndX, int ndY )
{
	m_nResizeWidthOffset += ndX;
	m_nResizeHeightOffset += ndY;

	int	nX = m_nResizeWidthOffset;
	int	nY = m_nResizeHeightOffset;
	switch( nSelRegion )
	{
	case 1:
		if( m_nWidth - nX < MAP_FRAME_WIDTH_MIN )
			nX = m_nWidth - MAP_FRAME_WIDTH_MIN;
		m_nPosX += nX;
		m_nWidth -= nX;
		m_nResizeWidthOffset -= nX;
		break;

	case 2:
		if( m_nWidth + nX < MAP_FRAME_WIDTH_MIN )
			nX = MAP_FRAME_WIDTH_MIN - m_nWidth;
		m_nWidth += nX;
		m_nResizeWidthOffset -= nX;
		break;

	case 3:
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		m_nResizeHeightOffset -= nY;
		break;

	case 4:
		if( m_nWidth - nX < MAP_FRAME_WIDTH_MIN )
			nX = m_nWidth - MAP_FRAME_WIDTH_MIN;
		m_nPosX += nX;
		m_nWidth -= nX;
		m_nResizeWidthOffset -= nX;
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		m_nResizeHeightOffset -= nY;
		break;

	case 5:
		if( m_nWidth + nX < MAP_FRAME_WIDTH_MIN )
			nX = MAP_FRAME_WIDTH_MIN - m_nWidth;
		m_nWidth += nX;
		m_nResizeWidthOffset -= nX;
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		m_nResizeHeightOffset -= nY;
		break;
	}

	m_btnClose.SetPosX( m_nWidth - MAP_CLOSE_X );
	m_slbOpacity.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_slbZoomWorld.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_slbZoomDetail.SetPosX( m_nWidth - MAP_OPACITY_X );
	m_cmbMap.SetPosX( m_nWidth - MAP_COMBO_X );
	m_btnSignal.SetPosX( m_nWidth - MAP_SIGNAL_X );

	m_rcTitle.Right = m_nWidth;
	m_rcResizeLeft.SetRect( 0, 22, 10, m_nHeight - 10 );
	m_rcResizeRight.SetRect( m_nWidth - 10, 22, m_nWidth, m_nHeight - 10 );
	m_rcResizeBottomL.SetRect( 0, m_nHeight - 10, 10, m_nHeight );
	m_rcResizeBottomM.SetRect( 10, m_nHeight - 10, m_nWidth - 10, m_nHeight );
	m_rcResizeBottomR.SetRect( m_nWidth - 10, m_nHeight - 10, m_nWidth, m_nHeight );

	AdjustViewport( nSelRegion );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Render()
{
	int		nX, nY;
	float	fX, fZ;

	// World map changing by layer
	if( _pNetwork->MyCharacterInfo.zoneNo == m_vecMapData[m_nCurMapData].World.nZoneIndex )
	{
		int	nLayerDiff = _pNetwork->MyCharacterInfo.yLayer - m_sbCurLayer;
		if( nLayerDiff < -1 || nLayerDiff > 1 )
		{
			m_sbCurLayer = _pNetwork->MyCharacterInfo.yLayer;
			int	nMapDataIndex = FindMapData( _pNetwork->MyCharacterInfo.zoneNo,
												_pNetwork->MyCharacterInfo.yLayer );
			if( nMapDataIndex != -1 )
			{
				m_nCurMapData = nMapDataIndex;
				m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
				m_bMapBlendingForLayer = TRUE;
				m_nCurWorldTex = m_nCurWorldTex == 0 ? 1 : 0;

				// Release map texture
				if( m_ptdWorldTex[m_nCurWorldTex] )
				{
					_pTextureStock->Release( m_ptdWorldTex[m_nCurWorldTex] );
					m_ptdWorldTex[m_nCurWorldTex] = NULL;
				}

				// Create map texture
				CTString	strFileName;
				strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex",
									_pNetwork->MyCharacterInfo.zoneNo, m_sbCurLayer );
				m_ptdWorldTex[m_nCurWorldTex] = CreateTexture( strFileName );

				m_llMapBlendTimeForLayer = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				m_llMapBlendElapsedTimeForLayer = 0;
			}
		}
	}

	// Scroll map
	/*static FLOAT	fOldX = _pNetwork->MyCharacterInfo.x;
	static FLOAT	fOldZ = _pNetwork->MyCharacterInfo.z;
	if( !m_bNowScrollMapWorld && !m_bNowScrollMapDetail )
	{
		if( m_bShowWorldMap )
		{
			ScrollMapWorld( 0, 0 );
		}
		else
		{
			ScrollMapDetail( 0, 0 );
		}
	}*/

	// Frame blending
	FLOAT	fBackRatio = 1.0f;
	if( m_bFrameBlending )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_llFrameBlendElapsedTime += llCurTime - m_llFrameBlendTime;
		m_llFrameBlendTime = llCurTime;

		if( m_llFrameBlendElapsedTime < MAP_FRAME_BLENDTIME )
		{
			fBackRatio = m_llFrameBlendElapsedTime / (FLOAT)MAP_FRAME_BLENDTIME;
			if( m_bShowFrame )
				fBackRatio = 1.0f - fBackRatio;
		}
		else
		{
			m_llFrameBlendElapsedTime = 0;
			m_bFrameBlending = FALSE;
			m_bShowFrame = !m_bShowFrame;
		}
	}

	// Map blending for detail map
	FLOAT	fDetailRatio = 1.0f;
	if( m_bMapBlendingForDetail )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( llCurTime - m_llMapBlendTimeForDetail < MAP_DETAIL_BLENDTIME )
		{
			fDetailRatio = ( llCurTime - m_llMapBlendTimeForDetail ) / (FLOAT)MAP_DETAIL_BLENDTIME;
			if( m_bShowWorldMap )
				fDetailRatio = 1.0f - fDetailRatio;
		}
		else
		{
			m_bMapBlendingForDetail = FALSE;
			m_bShowWorldMap = !m_bShowWorldMap;
		}
	}

	// Map blending for layer
	FLOAT	fLayerRatio = 1.0f;
	if( m_bMapBlendingForLayer )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_llMapBlendElapsedTimeForLayer += llCurTime - m_llMapBlendTimeForLayer;
		m_llMapBlendTimeForLayer = llCurTime;

		if( m_llMapBlendElapsedTimeForLayer < MAP_LAYER_BLENDTIME )
		{
			fLayerRatio = m_llMapBlendElapsedTimeForLayer / (FLOAT)MAP_LAYER_BLENDTIME;
			if( m_nCurWorldTex == 1 )
				fLayerRatio = 1.0f - fLayerRatio;
		}
		else
		{
			m_llMapBlendElapsedTimeForLayer = 0;
			m_bMapBlendingForLayer = FALSE;
		}
	}

	// Background
	if( m_bShowFrame || m_bFrameBlending )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fBackRatio * m_fMapOpacity );

		// Set map frame texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 3, m_nPosY + 22,
											m_nPosX + m_nWidth - 3, m_nPosY + m_nHeight - 3,
											m_rtBackListBar.U0, m_rtBackListBar.V0, m_rtBackListBar.U1, m_rtBackListBar.V1,
											colBlend );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}

	nX = m_nPosX + MAP_SX;
	nY = m_nPosY + MAP_SY;

	// Detail map
	if( m_bMapBlendingForDetail || !m_bShowWorldMap )
	{
		COLOR	colBlend;
		if( m_bShowWorldMap )
			colBlend = 0xFFFFFF00 | UBYTE( 0xFF * ( 1.0f - fDetailRatio ) * m_fMapOpacity );
		else
			colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * m_fMapOpacity );

		// Set detail texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdDetailTex );

		// Add render regions
		_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcViewportDetail.Left, nY + m_rcViewportDetail.Top,
											nX + m_rcViewportDetail.Right, nY + m_rcViewportDetail.Bottom,
											m_rtDetail.U0, m_rtDetail.V0, m_rtDetail.U1, m_rtDetail.V1,
											colBlend );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}

	// World map
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		BOOL	bShowTex0 = TRUE, bShowTex1 = TRUE;
		if( !m_bMapBlendingForLayer )
		{
			if( m_nCurWorldTex == 0 )
				bShowTex1 = FALSE;
			else
				bShowTex0 = FALSE;
		}

		if( bShowTex0 && m_ptdWorldTex[0] )
		{
			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * fLayerRatio * m_fMapOpacity );

			// Set world texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdWorldTex[0] );

			// Add render regions
			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcViewportWorld.Left, nY + m_rcViewportWorld.Top,
												nX + m_rcViewportWorld.Right, nY + m_rcViewportWorld.Bottom,
												m_rtWorld.U0, m_rtWorld.V0, m_rtWorld.U1, m_rtWorld.V1,
												colBlend );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}

		if( bShowTex1 && m_ptdWorldTex[1] )
		{
			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * ( 1.0f - fLayerRatio ) * m_fMapOpacity );

			// Set world texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdWorldTex[1] );

			// Add render regions
			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcViewportWorld.Left, nY + m_rcViewportWorld.Top,
												nX + m_rcViewportWorld.Right, nY + m_rcViewportWorld.Bottom,
												m_rtWorld.U0, m_rtWorld.V0, m_rtWorld.U1, m_rtWorld.V1,
												colBlend );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}
	}

	// Set map frame texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );


	// Location of subzone
	int	i;
	UIRect	rcTempWorld = m_rcMapWorld;
	UIRect	rcTempDetail = m_rcMapDetail;
	rcTempWorld.Scale( m_fZoomWorld );
	rcTempDetail.Scale( m_fZoomDetail );
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );

		for( i = 0; i < m_vecMapData[m_nCurMapData].vecSubZone.size(); i++ )
		{
			SBYTE	sbSubType = m_vecMapData[m_nCurMapData].vecSubZone[i].sbType;
			fX = m_vecMapData[m_nCurMapData].vecSubZone[i].fX - m_vecMapData[m_nCurMapData].World.lOffsetX;
			fZ = m_vecMapData[m_nCurMapData].vecSubZone[i].fZ - m_vecMapData[m_nCurMapData].World.lOffsetZ;
			nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nX += m_rcViewportWorld.Left - rcTempWorld.Left;
			nY += m_rcViewportWorld.Top - rcTempWorld.Top;
			if( nX + m_rcSubZone[sbSubType].Left >= m_rcViewportWorld.Left &&
				nX + m_rcSubZone[sbSubType].Right <= m_rcViewportWorld.Right &&
				nY + m_rcSubZone[sbSubType].Top >= m_rcViewportWorld.Top &&
				nY + m_rcSubZone[sbSubType].Bottom <= m_rcViewportWorld.Bottom )
			{
				nX += m_nPosX + MAP_SX;
				nY += m_nPosY + MAP_SY;
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcSubZone[sbSubType].Left, nY + m_rcSubZone[sbSubType].Top,
													nX + m_rcSubZone[sbSubType].Right, nY + m_rcSubZone[sbSubType].Bottom,
													m_rtSubZone[sbSubType].U0, m_rtSubZone[sbSubType].V0,
													m_rtSubZone[sbSubType].U1, m_rtSubZone[sbSubType].V1,
													colBlend );
			}
		}
	}
		

	// Render Castle object 
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		if( IsCastleMap( m_nCurMapData ) )
		{
			std::sort( m_vecMapData[m_nCurMapData].vecCastleData.begin(), m_vecMapData[m_nCurMapData].vecCastleData.end(), CompareZ );

			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );

			for( i = 0; i < m_vecMapData[m_nCurMapData].vecCastleData.size(); i++ )
			{
				LONG	nType = m_vecMapData[m_nCurMapData].vecCastleData[i].nType;
				
				if( nType == CASTLE_TOWER ) // 수호탑의 경우에는 hp가 0이면 그리지 않는다.
				{
					if( m_vecMapData[m_nCurMapData].vecCastleData[i].nHP <= 0 )
					{
						continue;
					}
				}
				
				fX = m_vecMapData[m_nCurMapData].vecCastleData[i].fX - m_vecMapData[m_nCurMapData].World.lOffsetX;
				fZ = m_vecMapData[m_nCurMapData].vecCastleData[i].fZ - m_vecMapData[m_nCurMapData].World.lOffsetZ;

				nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;

				if( nX + m_rcCastle[nType].Left >= m_rcViewportWorld.Left &&
					nX + m_rcCastle[nType].Right <= m_rcViewportWorld.Right &&
					nY + m_rcCastle[nType].Top >= m_rcViewportWorld.Top &&
					nY + m_rcCastle[nType].Bottom <= m_rcViewportWorld.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcCastle[nType].Left, nY + m_rcCastle[nType].Top,
														nX + m_rcCastle[nType].Right, nY + m_rcCastle[nType].Bottom,
														m_rtCastle[nType].U0, m_rtCastle[nType].V0,
														m_rtCastle[nType].U1, m_rtCastle[nType].V1,
														colBlend );
				}

			}
		}
	}

	// Location of NPC
	for( i = 0; i < m_vecMapData[m_nCurMapData].vecNpc.size(); i++ )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );
#ifdef HELP_SYSTEM_1
		/**** NPC 안내시스템 NPC 마크 렌더링 *****/	
		static BOOL		bHighlight = FALSE;
		static DOUBLE	dElapsedTime = 0.0;
		static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		dElapsedTime += dCurTime - dOldTime;
		dOldTime = dCurTime;
		if( dElapsedTime > 0.5 )
		{
			bHighlight = !bHighlight;
			do
			{
				dElapsedTime -= 0.5;
			}
			while( dElapsedTime > 0.5 );
		}
		/*---------------------------------*/
#endif
		if( ZoneInfo().GetZoneType( m_vecMapData[m_nCurMapData].World.nZoneIndex ) == ZONE_DUNGEON )
		{
			if( m_vecMapData[m_nCurMapData].World.sbLayer != m_vecMapData[m_nCurMapData].vecNpc[i].nYLayer )
			{
				continue;
			}
		}

		fX = m_vecMapData[m_nCurMapData].vecNpc[i].fX;
		fZ = m_vecMapData[m_nCurMapData].vecNpc[i].fZ;
		if( m_bShowWorldMap )
		{
			fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
			fZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
			nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nX += m_rcViewportWorld.Left - rcTempWorld.Left;
			nY += m_rcViewportWorld.Top - rcTempWorld.Top;
			if( nX + m_rcNPC.Left >= m_rcViewportWorld.Left && nX + m_rcNPC.Right <= m_rcViewportWorld.Right &&
				nY + m_rcNPC.Top >= m_rcViewportWorld.Top && nY + m_rcNPC.Bottom <= m_rcViewportWorld.Bottom )
			{
				nX += m_nPosX + MAP_SX;
				nY += m_nPosY + MAP_SY;
#ifdef HELP_SYSTEM_1
				//NPC 안내시스템 -> 안내된 NPC의 미니맵 표시를 틀리게 함
				if(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == _pUIMgr->m_nHelpNpc_Index)
				{
					if(bHighlight)
					{_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcHelpNpc.Left, nY + m_rcHelpNpc.Top,
													nX + m_rcHelpNpc.Right, nY + m_rcHelpNpc.Bottom,
													m_rtHelpNpc.U0, m_rtHelpNpc.V0, m_rtHelpNpc.U1, m_rtHelpNpc.V1,
													colBlend ); }
				}else if(CQuestSystem::Instance().TestNPCForQuest(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex) == CQuestSystem::NQT_HAVE_QUEST)
				{
					
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcQuestNpc.Left, nY + m_rcQuestNpc.Top,
														nX + m_rcQuestNpc.Right, nY + m_rcQuestNpc.Bottom,
														m_rtQuestNpc.U0, m_rtQuestNpc.V0, m_rtQuestNpc.U1, m_rtQuestNpc.V1,
														colBlend );
					

				}else if(CQuestSystem::Instance().TestNPCForQuest(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex) == CQuestSystem::NQT_CAN_PRIZE)
				{
					if(bHighlight)// 보상 받는 NPC 깜박거림
					{
						_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcSuccessNpc.Left, nY + m_rcSuccessNpc.Top,
														nX + m_rcSuccessNpc.Right, nY + m_rcSuccessNpc.Bottom,
														m_rtSuccessNpc.U0, m_rtSuccessNpc.V0, m_rtSuccessNpc.U1, m_rtSuccessNpc.V1,
														colBlend );
					}
				}else
				{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcNPC.Left, nY + m_rcNPC.Top,
													nX + m_rcNPC.Right, nY + m_rcNPC.Bottom,
													m_rtNPC.U0, m_rtNPC.V0, m_rtNPC.U1, m_rtNPC.V1,
													colBlend );
			}
#else
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcNPC.Left, nY + m_rcNPC.Top,
														nX + m_rcNPC.Right, nY + m_rcNPC.Bottom,
														m_rtNPC.U0, m_rtNPC.V0, m_rtNPC.U1, m_rtNPC.V1,
														colBlend );
#endif
				
			}
		}
		else
		{
			fX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
			fZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
			nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
			nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
			nX += m_rcViewportDetail.Left - rcTempDetail.Left;
			nY += m_rcViewportDetail.Top - rcTempDetail.Top;
			if( nX + m_rcNPC.Left >= m_rcViewportDetail.Left && nX + m_rcNPC.Right <= m_rcViewportDetail.Right &&
				nY + m_rcNPC.Top >= m_rcViewportDetail.Top && nY + m_rcNPC.Bottom <= m_rcViewportDetail.Bottom )
			{
				nX += m_nPosX + MAP_SX;
				nY += m_nPosY + MAP_SY;
#ifdef HELP_SYSTEM_1
				//NPC 안내시스템 -> 안내된 NPC의 미니맵 표시를 틀리게 함
				if(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == _pUIMgr->m_nHelpNpc_Index)
				{
					if(bHighlight)
					{
						_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcHelpNpc.Left, nY + m_rcHelpNpc.Top,
													nX + m_rcHelpNpc.Right, nY + m_rcHelpNpc.Bottom,
													m_rtHelpNpc.U0, m_rtHelpNpc.V0, m_rtHelpNpc.U1, m_rtHelpNpc.V1,
													colBlend ); 
					}
				}else if(CQuestSystem::Instance().TestNPCForQuest(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex) == CQuestSystem::NQT_HAVE_QUEST)
				{
			
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcQuestNpc.Left, nY + m_rcQuestNpc.Top,
														nX + m_rcQuestNpc.Right, nY + m_rcQuestNpc.Bottom,
														m_rtQuestNpc.U0, m_rtQuestNpc.V0, m_rtQuestNpc.U1, m_rtQuestNpc.V1,
														colBlend );
					

				}else if(CQuestSystem::Instance().TestNPCForQuest(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex) == CQuestSystem::NQT_CAN_PRIZE)
				{
					if(bHighlight)// 보상 받는 NPC 깜박거림
					{
						_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcSuccessNpc.Left, nY + m_rcSuccessNpc.Top,
														nX + m_rcSuccessNpc.Right, nY + m_rcSuccessNpc.Bottom,
														m_rtSuccessNpc.U0, m_rtSuccessNpc.V0, m_rtSuccessNpc.U1, m_rtSuccessNpc.V1,
														colBlend );
					}

				}else
				{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcNPC.Left, nY + m_rcNPC.Top,
													nX + m_rcNPC.Right, nY + m_rcNPC.Bottom,
													m_rtNPC.U0, m_rtNPC.V0, m_rtNPC.U1, m_rtNPC.V1,
													colBlend );
				}
#else
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcNPC.Left, nY + m_rcNPC.Top,
													nX + m_rcNPC.Right, nY + m_rcNPC.Bottom,
													m_rtNPC.U0, m_rtNPC.V0, m_rtNPC.U1, m_rtNPC.V1,
													colBlend );
#endif
				
			}
		}
	}

	if( _pNetwork->MyCharacterInfo.zoneNo == m_vecMapData[m_nCurMapData].World.nZoneIndex )
	{
		int	nLayerDiff;
		// Location of Party
		for( i = 0; i < _pUIMgr->GetParty()->GetMemberCount(); i++ )
		{
			nLayerDiff = _pUIMgr->GetParty()->GetLayer( i ) - m_sbCurLayer;
			if( nLayerDiff < -1 || nLayerDiff > 1 )
				continue;
			
			// 파티원 멤버가 같은 맵에 없다면 표시 않함
			if( m_vecMapData[m_nCurMapData].World.nZoneIndex != _pUIMgr->GetParty()->GetMemberZone( i ) )
				continue;

			fX = _pUIMgr->GetParty()->GetPosX( i );
			fZ = _pUIMgr->GetParty()->GetPosZ( i );
			if( m_bShowWorldMap )
			{
				fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
				fZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
				nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;
				if( nX + m_rcParty.Left >= m_rcViewportWorld.Left && nX + m_rcParty.Right <= m_rcViewportWorld.Right &&
					nY + m_rcParty.Top >= m_rcViewportWorld.Top && nY + m_rcParty.Bottom <= m_rcViewportWorld.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcParty.Left, nY + m_rcParty.Top,
														nX + m_rcParty.Right, nY + m_rcParty.Bottom,
														m_rtParty.U0, m_rtParty.V0, m_rtParty.U1, m_rtParty.V1,
														0xFFFFFFFF );
				}
			}
			else
			{
				fX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
				fZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
				nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nX += m_rcViewportDetail.Left - rcTempDetail.Left;
				nY += m_rcViewportDetail.Top - rcTempDetail.Top;
				if( nX + m_rcParty.Left >= m_rcViewportDetail.Left && nX + m_rcParty.Right <= m_rcViewportDetail.Right &&
					nY + m_rcParty.Top >= m_rcViewportDetail.Top && nY + m_rcParty.Bottom <= m_rcViewportDetail.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcParty.Left, nY + m_rcParty.Top,
														nX + m_rcParty.Right, nY + m_rcParty.Bottom,
														m_rtParty.U0, m_rtParty.V0, m_rtParty.U1, m_rtParty.V1,
														0xFFFFFFFF );
				}
			}
		}

		// Position & direction of character
		nLayerDiff = _pNetwork->MyCharacterInfo.yLayer - m_sbCurLayer;
		if( nLayerDiff >= -1 && nLayerDiff <= 1 )
		{
			FLOAT	fSin = Sin( -_pNetwork->MyCharacterInfo.r );
			FLOAT	fCos = Cos( -_pNetwork->MyCharacterInfo.r );
			FLOAT	fLeftCos = m_rcPC.Left * fCos;
			FLOAT	fLeftSin = m_rcPC.Left * fSin;
			FLOAT	fRightCos = m_rcPC.Right * fCos;
			FLOAT	fRightSin = m_rcPC.Right * fSin;
			FLOAT	fTopSin = m_rcPC.Top * fSin;
			FLOAT	fTopCos = m_rcPC.Top * fCos;
			FLOAT	fBottomSin = m_rcPC.Bottom * fSin;
			FLOAT	fBottomCos = m_rcPC.Bottom * fCos;

			FLOAT	fX1 = fLeftCos - fTopSin;
			FLOAT	fY1 = fLeftSin + fTopCos;
			FLOAT	fX2 = fLeftCos - fBottomSin;
			FLOAT	fY2 = fLeftSin + fBottomCos;
			FLOAT	fX3 = fRightCos - fBottomSin;
			FLOAT	fY3 = fRightSin + fBottomCos;
			FLOAT	fX4 = fRightCos - fTopSin;
			FLOAT	fY4 = fRightSin + fTopCos;

			if( m_bShowWorldMap )
			{
				nX = ( _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].World.lOffsetX ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nY = ( _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].World.lOffsetZ ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;
				if( nX + m_rcPC.Left >= m_rcViewportWorld.Left && nX + m_rcPC.Right <= m_rcViewportWorld.Right &&
					nY + m_rcPC.Top >= m_rcViewportWorld.Top && nY + m_rcPC.Bottom <= m_rcViewportWorld.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					_pUIMgr->GetDrawPort()->AddTexture( nX + fX1, nY + fY1, m_rtPC.U0, m_rtPC.V0, 0xFFFFFFFF,
														nX + fX2, nY + fY2, m_rtPC.U0, m_rtPC.V1, 0xFFFFFFFF,
														nX + fX3, nY + fY3, m_rtPC.U1, m_rtPC.V1, 0xFFFFFFFF,
														nX + fX4, nY + fY4, m_rtPC.U1, m_rtPC.V0, 0xFFFFFFFF );
				}
			}
			else
			{
				fX = _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
				fZ = _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
				nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nX += m_rcViewportDetail.Left - rcTempDetail.Left;
				nY += m_rcViewportDetail.Top - rcTempDetail.Top;
				if( nX + m_rcPC.Left >= m_rcViewportDetail.Left && nX + m_rcPC.Right <= m_rcViewportDetail.Right &&
					nY + m_rcPC.Top >= m_rcViewportDetail.Top && nY + m_rcPC.Bottom <= m_rcViewportDetail.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					_pUIMgr->GetDrawPort()->AddTexture( nX + fX1, nY + fY1, m_rtPC.U0, m_rtPC.V0, 0xFFFFFFFF,
														nX + fX2, nY + fY2, m_rtPC.U0, m_rtPC.V1, 0xFFFFFFFF,
														nX + fX3, nY + fY3, m_rtPC.U1, m_rtPC.V1, 0xFFFFFFFF,
														nX + fX4, nY + fY4, m_rtPC.U1, m_rtPC.V0, 0xFFFFFFFF );
				}
			}
		}
	}

	RenderSignal();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Background
	if( m_bShowFrame || m_bFrameBlending )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fBackRatio );

		// Add render regions
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY,
											m_nPosX + 34, m_nPosY + 22,
											m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 34, m_nPosY,
											m_nPosX + m_nWidth - 34, m_nPosY + 22,
											m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_nWidth - 34,
											m_nPosY, m_nPosX + m_nWidth, m_nPosY + 22,
											m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + 22,
											m_nPosX + 34, m_nPosY + m_nHeight - 15,
											m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 34, m_nPosY + 22,
											m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight - 15,
											m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_nWidth - 34, m_nPosY + 22,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight - 15,
											m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY + m_nHeight - 15,
											m_nPosX + 34, m_nPosY + m_nHeight,
											m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 34, m_nPosY + m_nHeight - 15,
											m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight,
											m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
											colBlend );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight - 15,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
											colBlend );

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 3, m_nPosY + 22,
											m_nPosX + m_nWidth - 3, m_nPosY + 47,
											m_rtBackListBar.U0, m_rtBackListBar.V0, m_rtBackListBar.U1, m_rtBackListBar.V1,
											colBlend );

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcCompass.Left, m_nPosY + m_nHeight + m_rcCompass.Top,
											m_nPosX + m_rcCompass.Right, m_nPosY + m_nHeight + m_rcCompass.Bottom,
											m_rtCompass.U0, m_rtCompass.V0, m_rtCompass.U1, m_rtCompass.V1,
											colBlend );

		// Text in map
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 190, "지도" ), m_nPosX + MAP_TITLE_TEXT_OFFSETX,
											m_nPosY + MAP_TITLE_TEXT_OFFSETY, colBlend );
		if( m_bShowOpacitySlideInfo )
		{
			if( m_nWidth - MAP_SLIDEINFO_X - m_nOpacityStringWidth > 0 )
			{
				colBlend = ( colBlend & 0x000000FF ) | 0xDDDDDDFF;
				_pUIMgr->GetDrawPort()->PutTextExRX( m_strOpacitySlideInfo, m_nPosX + m_nWidth - MAP_SLIDEINFO_X,
														m_nPosY + MAP_SLIDEINFO_Y, colBlend );
			}
		}
		else if( m_bShowZoomSlideInfo )
		{
			if( m_nWidth - MAP_SLIDEINFO_X - m_nZoomStringWidth > 0 )
			{
				colBlend = ( colBlend & 0x000000FF ) | 0xDDDDDDFF;
				_pUIMgr->GetDrawPort()->PutTextExRX( m_strZoomSlideInfo, m_nPosX + m_nWidth - MAP_SLIDEINFO_X,
														m_nPosY + MAP_SLIDEINFO_Y, colBlend );
			}
		}

		if( fBackRatio >= 1.0f )
		{
			// Close button
			m_btnClose.Render();

			if( IsPossibleSignal() )
			{
				m_btnSignal.Render();
			}

			// Map combo box
			m_cmbMap.Render();

			// Zoom slide bar
			if( m_bShowWorldMap )
				m_slbZoomWorld.Render();
			else
				m_slbZoomDetail.Render();

			// Opacity slide bar
			m_slbOpacity.Render();
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();

		// Tool tip
		if( m_bShowToolTip )
		{
			// Set texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top,
												m_rcToolTip.Left + 7, m_rcToolTip.Bottom,
												m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Left + 7, m_rcToolTip.Top,
												m_rcToolTip.Right - 7, m_rcToolTip.Bottom,
												m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( m_rcToolTip.Right - 7, m_rcToolTip.Top,
												m_rcToolTip.Right, m_rcToolTip.Bottom,
												m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
												0xFFFFFFFF );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Text in tool tip
			_pUIMgr->GetDrawPort()->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ToggleVisible()
{
	extern SLONG	g_slZone;
	
	// 현재 던전의 경우에는 지도를 표시하지 않는다.
	if( ZoneInfo().GetZoneType( g_slZone ) == ZONE_SDUNGEON )
	{
		if( IsVisible() )
			_pUIMgr->RearrangeOrder( UI_MAP, FALSE );

		return;
	}

	

	// Toggle map
	BOOL	bVisible = !IsVisible();
	_pUIMgr->RearrangeOrder( UI_MAP, bVisible );

	// !! 공성전 Map표시 조건에 맞다면 서버로 정보 요청 
	//
	if( bVisible )
	{
		ReSetData();
		_pNetwork->SendCastleMapRecent();
	}
	else
	{
		m_qSignalData.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : MakeCenterForZoom()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::MakeCenterForZoom( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		m_nZoomCenterX = m_rcMapWorld.Left + m_rcMapWorld.GetWidth() / 2;
		m_nZoomCenterY = m_rcMapWorld.Top + m_rcMapWorld.GetHeight() / 2;
	}
	else
	{
		m_nZoomCenterX = m_rcMapDetail.Left + m_rcMapDetail.GetWidth() / 2;
		m_nZoomCenterY = m_rcMapDetail.Top + m_rcMapDetail.GetHeight() / 2;
	}
}

// ----------------------------------------------------------------------------
// Name : ZoomMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ZoomMap( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
		m_rcMapWorld.SetRect( m_nZoomCenterX - nHalfW, m_nZoomCenterY - nHalfH,
								m_nZoomCenterX + nHalfW, m_nZoomCenterY + nHalfH );
		AdjustViewport( 6 );
	}
	else
	{
		int	nHalfW = m_nMapDetailBaseWidth / m_fZoomDetail * 0.5f;
		int	nHalfH = m_nMapDetailBaseHeight / m_fZoomDetail * 0.5f;
		m_rcMapDetail.SetRect( m_nZoomCenterX - nHalfW, m_nZoomCenterY - nHalfH,
								m_nZoomCenterX + nHalfW, m_nZoomCenterY + nHalfH );
		AdjustViewport( 6 );
	}
}

// ----------------------------------------------------------------------------
// Name : IsInsideMap()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIMap::IsInsideMap( BOOL bWinPos, BOOL bWorldMap, int nX, int nY, BOOL bZoomRect, UIRect &rcRect )
{
	if( bWinPos )
	{
		nX -= m_nPosX + MAP_SX;
		nY -= m_nPosY + MAP_SY;
	}

	if( bWorldMap )
	{
		nX -= m_rcViewportWorld.Left;
		nY -= m_rcViewportWorld.Top;
		nX /= m_fZoomWorld;
		nY /= m_fZoomWorld;
		nX += m_rcMapWorld.Left;
		nY += m_rcMapWorld.Top;

		if( nX < m_rcMapWorld.Left || nX > m_rcMapWorld.Right ||
			nY < m_rcMapWorld.Top || nY > m_rcMapWorld.Bottom )
			return FALSE;

		if( bZoomRect )
		{
			int	nWidth = rcRect.GetWidth() / m_fZoomWorld;
			int	nHeight = rcRect.GetHeight() / m_fZoomWorld;
			int	nCX = rcRect.GetCenterX();
			int	nCY = rcRect.GetCenterY();

			rcRect.Left = nCX - nWidth / 2;
			rcRect.Right = rcRect.Left + nWidth;
			rcRect.Top = nCY - nHeight / 2;
			rcRect.Bottom = rcRect.Top + nHeight;
		}

		if( nX < rcRect.Left || nX > rcRect.Right ||
			nY < rcRect.Top || nY > rcRect.Bottom )
			return FALSE;
	}
	else
	{
		nX -= m_rcViewportDetail.Left;
		nY -= m_rcViewportDetail.Top;
		nX /= m_fZoomDetail;
		nY /= m_fZoomDetail;
		nX += m_rcMapDetail.Left;
		nY += m_rcMapDetail.Top;

		if( nX < m_rcMapDetail.Left || nX > m_rcMapDetail.Right ||
			nY < m_rcMapDetail.Top || nY > m_rcMapDetail.Bottom )
			return FALSE;

		if( bZoomRect )
		{
			int	nWidth = rcRect.GetWidth() / m_fZoomDetail;
			int	nHeight = rcRect.GetHeight() / m_fZoomDetail;
			int	nCX = rcRect.GetCenterX();
			int	nCY = rcRect.GetCenterY();

			rcRect.Left = nCX - nWidth / 2;
			rcRect.Right = rcRect.Left + nWidth;
			rcRect.Top = nCY - nHeight / 2;
			rcRect.Bottom = rcRect.Top + nHeight;
		}

		if( nX < rcRect.Left || nX > rcRect.Right ||
			nY < rcRect.Top || nY > rcRect.Bottom )
			return FALSE;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : UpdateOpacityRatio()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateOpacityRatio()
{
	m_fMapOpacity = ( 25 + m_slbOpacity.GetCurPos() ) / 100.0f;
}

// ----------------------------------------------------------------------------
// Name : UpdateOpacitySlideInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateOpacitySlideInfo()
{
	m_bShowOpacitySlideInfo = TRUE;
	m_bShowZoomSlideInfo = FALSE;
	m_strOpacitySlideInfo.PrintF( _S( 1035, "투명 %d%%" ), ( 25 + m_slbOpacity.GetCurPos() ) );

	m_nOpacityStringWidth = m_strOpacitySlideInfo.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}

// ----------------------------------------------------------------------------
// Name : UpdateZoomSlideInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateZoomSlideInfo( BOOL bWorldMap )
{
	m_bShowOpacitySlideInfo = FALSE;
	m_bShowZoomSlideInfo = TRUE;

	if( bWorldMap )
	{
		if( m_slbZoomWorld.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomWorld.GetCurPos() ) * 5 );
		else
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 + ( m_slbZoomWorld.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 );
	}
	else
	{
		if( m_slbZoomDetail.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomDetail.GetCurPos() ) * 5 );
		else
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 + ( m_slbZoomDetail.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 );
	}

	m_nZoomStringWidth = m_strZoomSlideInfo.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}

// ----------------------------------------------------------------------------
// Name : UpdateZoomRatio()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateZoomRatio( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		if( m_slbZoomWorld.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_fZoomWorld = (FLOAT)( 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomWorld.GetCurPos() ) * 5 ) / 100.0f;
		else
			m_fZoomWorld = (FLOAT)( 100 + ( m_slbZoomWorld.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 ) / 100.0f;
	}
	else
	{
		if( m_slbZoomDetail.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_fZoomDetail = (FLOAT)( 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomDetail.GetCurPos() ) * 5 ) / 100.0f;
		else
			m_fZoomDetail = (FLOAT)( 100 + ( m_slbZoomDetail.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 ) / 100.0f;
	}

	ZoomMap( bWorldMap );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMap::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Resize bar
	static int	nResizeBarClick = 0;	// 0: None, 1:Left, 2:Right, 3:Middle bottom, 4:Left bottom, 5:Right bottom
	// Scroll world map
	static BOOL	bScrollWorldMap = FALSE;
	// Scroll detail map
	static BOOL	bScrollDetailMap = FALSE;
	static BOOL bScrolledDetailMap = FALSE;
	// Detail map click
	static BOOL	bDetailMapClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Slide info
			m_bShowOpacitySlideInfo = FALSE;
			m_bShowZoomSlideInfo = FALSE;

			if( IsInside( nX, nY ) )
			{
				m_bInsideMouse = true;
				if( nResizeBarClick == 0 && !bTitleBarClick && !bScrollWorldMap && !bScrollDetailMap )
				{
					if( m_bFrameBlending )
					{
						if( m_bShowFrame )
						{
							m_llFrameBlendElapsedTime = MAP_FRAME_BLENDTIME - m_llFrameBlendElapsedTime;
							m_bShowFrame = FALSE;
						}
					}
					else if( !m_bShowFrame )
					{
						m_llFrameBlendElapsedTime = 0;
						m_llFrameBlendTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
						m_bFrameBlending = TRUE;
					}
				}

			}
			else
			{
				// 컨트롤 밖으로 나가면 무조건 취소
				SetSignalOn( false );
				m_bInsideMouse = false;
				if( nResizeBarClick == 0 && !bTitleBarClick && !bScrollWorldMap && !bScrollDetailMap )
				{
					if( m_bFrameBlending )
					{
						if( !m_bShowFrame )
						{
							m_llFrameBlendElapsedTime = MAP_FRAME_BLENDTIME - m_llFrameBlendElapsedTime;
							m_bShowFrame = TRUE;
						}
					}
					else if( m_bShowFrame )
					{
						m_llFrameBlendElapsedTime = 0;
						m_llFrameBlendTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
						m_bFrameBlending = TRUE;
					}
				}
		
			}

			if( m_bShowFrame )
			{
				// 시그널이 온인 경우네는 다른 모든 마우스 커서 컨트롤 무시
				if( m_bSignalBtnOn ) 
				{
					SetSignalOn( true );
					return WMSG_SUCCESS;
				}

				// Scroll world map
				if( bScrollWorldMap && ( pMsg->wParam & MK_LBUTTON ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_PICK );

					int	ndX = nOldX - nX;
					int	ndY = nOldY - nY;
					nOldX = nX;	nOldY = nY;

					if( IsInsideRect( nX - MAP_SX, nY - MAP_SY, m_rcViewportWorld ) )
						ScrollMapWorld( ndX, ndY );

					return WMSG_SUCCESS;
				}

				// Scroll detail map
				if( bScrollDetailMap && ( pMsg->wParam & MK_LBUTTON ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_PICK );

					int	ndX = nOldX - nX;
					int	ndY = nOldY - nY;
					nOldX = nX;	nOldY = nY;

					if( IsInsideRect( nX - MAP_SX, nY - MAP_SY, m_rcViewportDetail ) )
					{
						ScrollMapDetail( ndX, ndY );
						bScrolledDetailMap = TRUE;
					}

					return WMSG_SUCCESS;
				}
				
				// Resize map
				if( nResizeBarClick != 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );

					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					ResizeMapFrame( nResizeBarClick, ndX, ndY );

					return WMSG_SUCCESS;
				}

				
				_pUIMgr->SetMouseCursorInsideUIs();

				// Move map
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Opacity slide bar
				if( ( wmsgResult = m_slbOpacity.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateOpacitySlideInfo();
					if( wmsgResult == WMSG_COMMAND )
						UpdateOpacityRatio();

					return WMSG_SUCCESS;
				}
				// World zoom slide bar
				if( m_bShowWorldMap && ( wmsgResult = m_slbZoomWorld.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateZoomSlideInfo( TRUE );
					if( wmsgResult == WMSG_COMMAND )
						UpdateZoomRatio( TRUE );

					return WMSG_SUCCESS;
				}
				// Detail zoom slide bar
				if( !m_bShowWorldMap && ( wmsgResult = m_slbZoomDetail.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateZoomSlideInfo( FALSE );
					if( wmsgResult == WMSG_COMMAND )
						UpdateZoomRatio( FALSE );

					return WMSG_SUCCESS;
				}
				// Map list combo box
				if( m_cmbMap.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				// Resize region
				if( ( IsInsideRect( nX, nY, m_rcResizeLeft ) || IsInsideRect( nX, nY, m_rcResizeRight ) ||
					IsInsideRect( nX, nY, m_rcResizeBottomM ) || IsInsideRect( nX, nY, m_rcResizeBottomL ) ||
					IsInsideRect( nX, nY, m_rcResizeBottomR ) ) && !_pUIMgr->IsInsideUpperUIs( UI_MAP, nX, nY ) )
				{
					_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
					return WMSG_SUCCESS;
				}

				if( !_pUIMgr->IsInsideUpperUIs( UI_MAP, nX, nY ) )
				{
					// Tool tip
					UIRect	rcTip;
					int		nTipX, nTipZ;
					FLOAT	fTipX, fTipZ;
					BOOL	bShowToolTip = FALSE;
					for( int i = 0; i < m_vecMapData[m_nCurMapData].vecNpc.size(); i++ )
					{
						
						if( ZoneInfo().GetZoneType( m_vecMapData[m_nCurMapData].World.nZoneIndex ) == ZONE_DUNGEON )
						{
							if( m_vecMapData[m_nCurMapData].World.sbLayer != m_vecMapData[m_nCurMapData].vecNpc[i].nYLayer )
							{
								continue;
							}
						}
							
						fTipX = m_vecMapData[m_nCurMapData].vecNpc[i].fX;
						fTipZ = m_vecMapData[m_nCurMapData].vecNpc[i].fZ;
						if( m_bShowWorldMap )
						{
							fTipX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
							fTipZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;

							nTipX = fTipX * m_vecMapData[m_nCurMapData].World.fRatio;
							nTipZ = fTipZ * m_vecMapData[m_nCurMapData].World.fRatio;
						}
						else
						{
							fTipX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
							fTipZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
							nTipX = fTipX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
							nTipZ = fTipZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
						}

						rcTip = m_rcNPC;
						rcTip.Offset( nTipX, nTipZ );
					
						if( IsInsideMap( TRUE, m_bShowWorldMap, nX, nY, TRUE, rcTip ) )
						{
							bShowToolTip = TRUE;
							CTString	strNpcName = _pNetwork->GetMobName( m_vecMapData[m_nCurMapData].vecNpc[i].nIndex );
							ShowToolTip( TRUE, strNpcName, rcTip.GetCenterX(), rcTip.GetCenterY() );
							break;
						}
					}
					if( !bShowToolTip )
					{
						for( int i = 0; i < m_vecMapData[m_nCurMapData].vecSubZone.size(); i++ )
						{
							SBYTE	sbType = m_vecMapData[m_nCurMapData].vecSubZone[i].sbType;
							fTipX = m_vecMapData[m_nCurMapData].vecSubZone[i].fX;
							fTipZ = m_vecMapData[m_nCurMapData].vecSubZone[i].fZ;
							if( m_bShowWorldMap )
							{
								nTipX = fTipX * m_vecMapData[m_nCurMapData].World.fRatio;
								nTipZ = fTipZ * m_vecMapData[m_nCurMapData].World.fRatio;
							}
							else
							{
								fTipX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
								fTipZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
								nTipX = fTipX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
								nTipZ = fTipZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
							}

							rcTip = m_rcSubZone[sbType];
							rcTip.Offset( nTipX, nTipZ );
							if( IsInsideMap( TRUE, m_bShowWorldMap, nX, nY, TRUE, rcTip ) )
							{
								CTString	strTemp;
								if( sbType == CHARGE_PRIVATE || sbType == MINE_PRIVATE || sbType == GATHER_PRIVATE )
								{
									strTemp.PrintF( _S( 549, "%s(사유지)" ),			
													ZoneInfo().GetExtraName( m_vecMapData[m_nCurMapData].World.nZoneIndex,
																				m_vecMapData[m_nCurMapData].vecSubZone[i].nSubZoneIndex ) );
								}
								else
								{
									strTemp = ZoneInfo().GetExtraName( m_vecMapData[m_nCurMapData].World.nZoneIndex,
																		m_vecMapData[m_nCurMapData].vecSubZone[i].nSubZoneIndex );
								}

								bShowToolTip = TRUE;
								ShowToolTip( TRUE, strTemp, rcTip.GetCenterX(), rcTip.GetCenterY() );
								break;
							}
						}
					}
					if( !bShowToolTip )
						ShowToolTip( FALSE, CTString( "" ) );

					// Detail map region
					if( m_bShowWorldMap )
					{
						for( i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
						{
							if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE,
												m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion ) )
							{
								_pUIMgr->SetMouseCursorInsideUIs( UMCT_ZOOMIN );
								return WMSG_SUCCESS;
							}
						}
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bShowFrame )
			{
				if( !m_bSignalBtnOn ) // 시그널이 아닌경우에만 가능 
				{
					// Resize region - left
					if( IsInsideRect( nX, nY, m_rcResizeLeft ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 1;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;
					
						_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
						_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - right
					if( IsInsideRect( nX, nY, m_rcResizeRight ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 2;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
						_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - middle bottom
					if( IsInsideRect( nX, nY, m_rcResizeBottomM ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 3;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
						_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - left bottom
					if( IsInsideRect( nX, nY, m_rcResizeBottomL ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 4;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
						_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}
				}

				// Resize region - right bottom
				if( IsInsideRect( nX, nY, m_rcResizeBottomR ) )
				{
					nOldX = nX;		nOldY = nY;
					nResizeBarClick = 5;
					m_nResizeWidthOffset = 0;
					m_nResizeHeightOffset = 0;

					_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIZE );
					_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
					return WMSG_SUCCESS;
				}
			}

			if( IsInside( nX, nY ) )
			{
				bDetailMapClick = FALSE;
				nOldX = nX;		nOldY = nY;

				if( m_bShowFrame )
				{
					// Close button
					if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					else if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					// Opacity slide bar
					else if( ( wmsgResult = m_slbOpacity.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateOpacitySlideInfo();
						if( wmsgResult == WMSG_COMMAND )
							UpdateOpacityRatio();

						return WMSG_SUCCESS;
					}
					// World zoom slide bar
					else if( m_bShowWorldMap && ( wmsgResult = m_slbZoomWorld.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateZoomSlideInfo( TRUE );
						MakeCenterForZoom( TRUE );
						if( wmsgResult == WMSG_COMMAND )
							UpdateZoomRatio( TRUE );

						return WMSG_SUCCESS;
					}
					// Detail zoom slide bar
					else if( !m_bShowWorldMap && ( wmsgResult = m_slbZoomDetail.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateZoomSlideInfo( FALSE );
						MakeCenterForZoom( FALSE );
						if( wmsgResult == WMSG_COMMAND )
							UpdateZoomRatio( FALSE );

						return WMSG_SUCCESS;
					}
					// Map combo box
					else if( ( wmsgResult = m_cmbMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ChangeMap();
						
					}
					// Title bar
					else if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}
					else
					{
						int	nNewX = nX - MAP_SX;
						int	nNewY = nY - MAP_SY;
						if( m_bShowWorldMap )
						{
							if( IsInsideRect( nNewX, nNewY, m_rcViewportWorld ) )
							{
								if( m_bSignalBtnOn )
								{
									if( m_qSignalData.size() < MAX_SIGNAL )
									{
										FLOAT fX = nNewX - m_nPosX; 
										FLOAT fY = nNewY - m_nPosY;
									
										MapToWorld( fX, fY );
										//AddSignal( fX, fY, -1, -1 );
										SendSignal( fX, fY );
									}
																	
									if( pMsg->wParam != MK_CONTROL )
									{
										SetSignalOn( false );
									}
									return WMSG_SUCCESS;
								}

								// Open detail map
								for( int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
								{
									if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE,
														m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion ) )
									{
										bDetailMapClick = TRUE;
										
										_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
										return WMSG_SUCCESS;
									}
								}
							
								// Scroll world map
								if( m_bCanScrollMapWorld )
								{
									bScrollWorldMap = TRUE;
									_pUIMgr->SetMouseCursorInsideUIs( UMCT_PICK );
								}
							}
						}
						else
						{
							if( IsInsideRect( nNewX, nNewY, m_rcViewportDetail ) )
							{
								// Scroll detail map
								bScrolledDetailMap = FALSE;
								if( m_bCanScrollMapDetail )
								{
									_pUIMgr->SetMouseCursorInsideUIs( UMCT_PICK );
									bScrollDetailMap = TRUE;
								}
							}
						}
					}
					

					//key가 눌린 후 Control이 아닌경우 모두 취소 
					if( pMsg->wParam != MK_CONTROL )
					{
						SetSignalOn( false );
					}
					_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
					return WMSG_SUCCESS;
				}
			}
			else 
			{
				SetSignalOn( false );
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

				// Resize bar
				nResizeBarClick = 0;

				// Reset cursor
				if( bScrollWorldMap || bScrollDetailMap )
					_pUIMgr->SetMouseCursorInsideUIs();

				// Scroll world map
				bScrollWorldMap = FALSE;
				m_fMoveElapsedErrXWorld = 0;
				m_fMoveElapsedErrYWorld = 0;

				// Scroll detail map
				bScrollDetailMap = FALSE;
				m_fMoveElapsedErrXDetail = 0;
				m_fMoveElapsedErrYDetail = 0;

				// If map isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( m_bShowFrame || m_bFrameBlending )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							_pUIMgr->RearrangeOrder( UI_MAP, FALSE );

						return WMSG_SUCCESS;
					}
					else if( (wmsgResult = m_btnSignal.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND && IsPossibleSignal() )
						{
							SetSignalOn( true );
							_pUIMgr->RearrangeOrder( UI_MAP, TRUE );
						}

						return WMSG_SUCCESS;
					}
					// Opacity slide bar
					else if( m_slbOpacity.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// World zoom slide bar
					else if( m_slbZoomWorld.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Detail zoom slide bar
					else if( m_slbZoomDetail.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Map combo box
					else if( ( wmsgResult = m_cmbMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else
					{
						int	nNewX = nX - MAP_SX;
						int	nNewY = nY - MAP_SY;
						// Open detail map
						if( m_bShowWorldMap )
						{
							if( bDetailMapClick && IsInsideRect( nNewX, nNewY, m_rcViewportWorld ) )
							{
								for( int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
								{
									if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE,
														m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion ) )
									{
										bDetailMapClick = FALSE;
										OpenDetailMap( i );

										return WMSG_SUCCESS;
									}
								}
							}
						}
						// Close detail map
						else
						{
							if( !bScrolledDetailMap &&
								!IsInsideRect( nX, nY, m_rcTitle ) &&
								IsInsideRect( nNewX, nNewY, m_rcViewportDetail ) )
							{
								CloseDetailMap();
								
								return WMSG_SUCCESS;
							}
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
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
//  CUIMap::KeyMessage
// Explain: Ctrl키를 눌렀을 때 시그널을 보낼 수 있도록 KeyMessage를 받는다 
// * 일반적인 UIWindow는 KEYUP 메세지를 받지 않는다. ( 조합키를 사용하는 곳이 없음)
// *  UIMgr에서 MAP과 Rar만 예외 처리해서 KEYUP메세지를 받도록 하였음
// Date : 2005-10-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUIMap::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;
	
	// 마우스가 지도안에 있을 때만 처리한다.
	if( !m_bInsideMouse ) 
	{
		return WMSG_FAIL;
	}

	/* UP키를 예외 처리하지 않고 F1키 눌민면 그냥 가는방식
	if( pMsg->wParam == VK_F1 )
	{
		SetSignalOn( true );
		return WMSG_SUCCESS;
	}
	*/
	
	// 시그널을 사용할 수 있는지 체크 
	if( !IsPossibleSignal() ) 
	{
		return WMSG_FAIL;
	}

	switch( pMsg->message )
	{
	case WM_KEYDOWN:
		if( pMsg->wParam == VK_CONTROL )
		{
			SetSignalOn( true );
			return WMSG_SUCCESS;
		}
		break;

	case WM_KEYUP:
		if( pMsg->wParam == VK_CONTROL )
		{
			SetSignalOn( false );
			return WMSG_SUCCESS;
		}
		break;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUIMap::MapToWorld
// Explain:  지도상의 좌표를 월드 좌표로 변환( Detail Map은 지원하지 않음 )
// Date : 2005-10-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::MapToWorld( float& fX, float& fY )
{
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );
	
	// Viewport에 의한 위치 이동 
	fX = fX - m_rcViewportWorld.Left + rcTempWorld.Left;	
	fY = fY - m_rcViewportWorld.Top + rcTempWorld.Top;

	// 지도에 대한 축척과 지도 Zoom비율
	fX = fX / m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;	
	fY = fY / m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;

	// 지도의 Offset~
	fX += m_vecMapData[m_nCurMapData].World.lOffsetX;	
	fY += m_vecMapData[m_nCurMapData].World.lOffsetZ;
}


//------------------------------------------------------------------------------
// CUIMap::WorldToMap
// Explain:  지도상의 좌표를 월드 좌표로 변환( Detail Map은 지원하지 않음 )
// Date : 2005-10-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::WorldToMap( float& fX, float& fY )
{
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );

	// 지도의 Offset~
	fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;	
	fY -= m_vecMapData[m_nCurMapData].World.lOffsetZ;

	// 지도에 대한 축척과 지도 Zoom비율
	fX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;	
	fY = fY * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
	
	// Viewport에 의한 위치 이동 
	fX = fX + m_rcViewportWorld.Left - rcTempWorld.Left;	
	fY = fY + m_rcViewportWorld.Top - rcTempWorld.Top;

}

//------------------------------------------------------------------------------
// CUIMap::InitCastleData
// Explain: 지도상에 표시되는 공성 관련 데이터 초기화 
// * 서버에서 공성 데이터 올때 마다 초기화 한후에 다시 셋팅한다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::InitCastleData()
{
	m_vecMapData[m_nCurMapData].vecCastleData.clear();
	ClearAllSignal();
}


//------------------------------------------------------------------------------
// CUIMap::AddCastleData
// Explain: 서버에서 보내주는 공성 관련 데이터 셋팅
// * 현재는 HP는 처리하지 않는다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::AddCastleData( LONG nType, FLOAT fX, FLOAT fY, LONG nIndex, LONG nHP, LONG nMaxHP )
{
	CastleData TempCastleData;

	TempCastleData.nType	= nType;
	TempCastleData.fX		= fX;
	TempCastleData.fZ		= fY;
	TempCastleData.nIndex	= nIndex;
	TempCastleData.nHP		= nHP;
	TempCastleData.nMaxHP	= nMaxHP;

	m_vecMapData[m_nCurMapData].vecCastleData.push_back( TempCastleData );
}


//------------------------------------------------------------------------------
// CUIMap::SendSignal
// Explain: 서버에 시그널 좌표를 전송한다. ( 월드 좌표 )
// * Radar Signal 도 이곳을 통해서 전송됨.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::SendSignal( FLOAT fX, FLOAT fY )
{
	if( m_qSignalData.size() >= MAX_SIGNAL ) return;
	_pNetwork->SendCastleMapSignal( fX, fY, _pNetwork->MyCharacterInfo.lGuildPosition, _pNetwork->MyCharacterInfo.index );
}

//------------------------------------------------------------------------------
// CUIMap::AddSignal
// Explain: 서버에서 시그널 메세지를 받음
// * Radar에도 시그널 메세지를 보내준다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::AddSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex )
{
	CastleData	TempCastleData;
	CPrintF( "AddSignal(%f, %f)", fX, fY);
	TempCastleData.fX = fX;
	TempCastleData.fZ = fY;
	TempCastleData.nIndex = lSenderIndex;

	TempCastleData.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	
	m_qSignalData.push_back( TempCastleData );

	// Date : 2005-11-10(오후 1:30:55), By Lee Ki-hwan 
	// 시그널 사운드 추가 
	PCStartEffectGroup( "snd_signal", _pNetwork->MyCharacterInfo.index );

	_pUIMgr->GetRadar()->SetSignal( fX, fY );
}

//------------------------------------------------------------------------------
// CUIMap::ClearAllSignal
// Explain: 지도를 닫을 때는 모든 시그널 메세지를 삭제한다.
// * 다시 지도를 띄웠을 때 최종 시그널 메세지를 표시 하기 때문에 
//   중복 표시 되는 걸 막기 위해서  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::ClearAllSignal()
{
	m_qSignalData.clear();
}

//------------------------------------------------------------------------------
// CUIMap::IsCastleMap
// Explain: 해당 MAP이 공성맵인지 체크한다.
// * 공성 맵의 조건 
//   1. 던전 맵이 아니다.
//   2. 레이어가 1이상이다. 
//   * 던전의 경우에는 층수에 따라서 레이어가 존재 함 ( 현재는 구현되어 있지 않음) 
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIMap::IsCastleMap( LONG nMapIndex )
{
	if( ZoneInfo().GetZoneType( m_vecMapData[nMapIndex].World.nZoneIndex ) == ZONE_DUNGEON ) 
	{
		return FALSE;	
	}
	
	if( m_vecMapData[nMapIndex].World.sbLayer < 1 )
	{
		return FALSE;
	}

	return TRUE;

}

// 시그널 표시 정보
#define SIGNAL_SHOWTIME		5.0f							// 5초 동안
#define SIGNAL_COUNT		5.0f							// 5번 
#define SIGNAL_UNITTIME		(SIGNAL_SHOWTIME/SIGNAL_COUNT)  // 1번 표시할때 걸리는 시간.
#define SIGNAL_UNITTIME_HALF (SIGNAL_UNITTIME/2)
#define SIGNAL_ALPHA		0xFF

//------------------------------------------------------------------------------
// CUIMap::RenderSignal
// Explain: 2D 이펙트를 위한 하드 코딩~~~~즐~ 
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::RenderSignal()
{
	static FLOAT fOpacity	= 0.0f;

	for( int i = 0; i < m_qSignalData.size(); i++ )
	{
		DOUBLE dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		DOUBLE dDealy = dElapsedTime - m_qSignalData[i].dStartTime;
		
		if( dDealy > SIGNAL_SHOWTIME ) // 다끝났으면 삭제 
		{
			std::deque<CastleData>::iterator iter = m_qSignalData.begin() + i;
			m_qSignalData.erase( iter );
			return;
		}
		
		FLOAT	fX = 0.0f;
		FLOAT	fY = 0.0f;
		BOOL	bFirst = TRUE;
		UBYTE	bBlend;
		COLOR	colOriBlend = 0xFFFFFFFF;
		int		nCount = SIGNAL_COUNT-1;
				
		// signal의 Alpha & Size 조정 
		while( dDealy  > SIGNAL_UNITTIME )
		{
			nCount--;
			dDealy -= SIGNAL_UNITTIME;
			bFirst	= FALSE;
		}
	
		if( dDealy > SIGNAL_UNITTIME_HALF )
		{
			DOUBLE fTemp = ( dDealy - SIGNAL_UNITTIME_HALF ) / SIGNAL_UNITTIME_HALF;
			fTemp = SIGNAL_ALPHA * ( fTemp );
			bBlend = SIGNAL_ALPHA - fTemp;

			if( nCount <= 0 ) 
			{
				colOriBlend = ( 0xFFFFFFFF - SIGNAL_ALPHA ) | bBlend;
			}
		}
		else 
		{
			DOUBLE fTemp = ( dDealy ) / SIGNAL_UNITTIME_HALF;
			fTemp = SIGNAL_ALPHA * ( fTemp );
			bBlend = fTemp;
		}
		
		COLOR	colBlend = ( 0xFFFFFFFF - SIGNAL_ALPHA ) | bBlend;
		
		
		// Convert position
		fX = m_qSignalData[i].fX;
		fY = m_qSignalData[i].fZ;

		WorldToMap( fX, fY );

		if( fX + m_rcCastle[CASTLE_SIGNAL].Left >= m_rcViewportWorld.Left &&
			fX + m_rcCastle[CASTLE_SIGNAL].Right <= m_rcViewportWorld.Right &&
			fY + m_rcCastle[CASTLE_SIGNAL].Top >= m_rcViewportWorld.Top &&
			fY + m_rcCastle[CASTLE_SIGNAL].Bottom <= m_rcViewportWorld.Bottom )
		{
			fX += m_nPosX + MAP_SX;
			fY += m_nPosY + MAP_SY;
		
			FLOAT fWidth = ( m_rcCastle[CASTLE_SIGNAL].Right - m_rcCastle[CASTLE_SIGNAL].Left ) / 2;
			FLOAT fHeight = ( m_rcCastle[CASTLE_SIGNAL].Bottom - m_rcCastle[CASTLE_SIGNAL].Top ) / 2;
		
			FLOAT fOriWidth = fWidth;
			FLOAT fOriHeight = fHeight;

			if( bFirst ) // 맨 처음에는 크게 ~
			{
				fWidth = fWidth + (fWidth + fWidth) * dDealy;
				fHeight = fHeight + (fHeight + fHeight) * dDealy;
			}
			else // 기본 크기로 
			{
				fWidth = fWidth + fWidth * dDealy;
				fHeight = fHeight + fHeight * dDealy;
			}
		
			// 잔상 Signal
			_pUIMgr->GetDrawPort()->AddTexture( fX - fWidth, fY - fHeight,
												fX + fWidth, fY + fHeight,
												m_rtCastle[CASTLE_SIGNAL].U0, m_rtCastle[CASTLE_SIGNAL].V0,
												m_rtCastle[CASTLE_SIGNAL].U1, m_rtCastle[CASTLE_SIGNAL].V1,
												colBlend );
			
			// 기본 Signal 
			_pUIMgr->GetDrawPort()->AddTexture( fX - fOriWidth, fY - fOriHeight,
												fX + fOriWidth, fY + fOriHeight,
												m_rtCastle[CASTLE_SIGNAL].U0, m_rtCastle[CASTLE_SIGNAL].V0,
												m_rtCastle[CASTLE_SIGNAL].U1, m_rtCastle[CASTLE_SIGNAL].V1,
												colOriBlend );
		} // if

	} // for( iter = iterBegin; i != iterEnd ; iter++ )
} 

//------------------------------------------------------------------------------
// CUIMap::SetSignalOn
// Explain:  시그널 변수를 셋팅하고 마우스 커서를 변경 합니다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::SetSignalOn( bool bOn ) 
{
	if( !bOn )
	{
		if( m_bSignalBtnOn ) 
		{
			m_bSignalBtnOn = false; 
			_pUIMgr->SetMouseCursorInsideUIs();
		}	
	}
	else
	{
		m_bSignalBtnOn = true;
		_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIGNAL );
	}

}

//------------------------------------------------------------------------------
// CUIMap::IsPossibleSignal
// Explain:  시그널을 사용할 수 있는 지 체크 
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIMap::IsPossibleSignal()
{
	// 길드장, 부장인경우 
	// 공성 맵인 경우 
	LONG nRanking = _pNetwork->MyCharacterInfo.lGuildPosition;

	if( nRanking != GUILD_MEMBER_BOSS && nRanking != GUILD_MEMBER_VICE_BOSS )
		return false; 

	if( m_nCurMapData < 0 ) 
		return false;

	if( !IsCastleMap( m_nCurMapData ) )
		return false;

	if( !IsPossibleCastleMap() ) 
		return false; 

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::IsPossibleCastleMap
// Explain: 공성전 지도를 사용할 수 있는지 체크 
// * 1. 공성전 중이고, 2. 내가 공성전에 참여 중이고, 공성전이 있는 존이여야함
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIMap::IsPossibleCastleMap()
{	
	// 공성전이 진행 되고 있어야함 
	if( !_pUISWDoc->IsWar() || _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return false;
	if( _pNetwork->MyCharacterInfo.zoneNo != 7 )
		return false;

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::ReSetData
// Explain: 맵 선택 콤보상자에 공성전 Map를 보이거나 그렇지 않거나 하도록 함.
// Date : 2005-10-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::ReSetData()
{
	// 1. Map List
	m_cmbMap.ResetStrings();
	
	for( int i = 0; i < m_vecMapData.size(); i++ )
	{
		if( IsCastleMap( i ) ) // 공성전 멥이라면 
		{
			if( IsPossibleCastleMap() ) // 공성전이 가능한 상황인지 파악해서 추가
			{
				m_cmbMap.AddString( m_vecMapData[i].World.strName );	
			}
		}
		else // 공성 맵이 아니라면 그냥 추가 
		{
			m_cmbMap.AddString( m_vecMapData[i].World.strName );	
		}
	}

	m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
}