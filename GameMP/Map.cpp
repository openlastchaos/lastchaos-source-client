#include "stdafx.h"
#include "LCDDrawing.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>


// Define
#define LOADING_IMAGE_COUNT		33 //23
#define	ROUND					0.005f
#define FIRST_INTO_WORLD		LOADING_IMAGE_COUNT - 1

//Text Pos
#define PIX_TEXT				18
#define PIY_TEXT				18

ENGINE_API extern INDEX g_iCountry;
// static Variable
static CTextureObject	atoIcons[LOADING_IMAGE_COUNT][2];
static CTextureObject	ptoProgressBar;
static CTextureObject	ptoLoadingBar;
static CTextureObject	toText;
static CTextureObject	toClassification;
static CTextureData*	ptdProgressBar = NULL;
static CTextureData*	ptdLoadingBar = NULL;
static UIRectUV			rtProgressBar[3];
static UIRectUV			rtLoadingBar[3];

static int nLoadingStep = 0;

extern ENGINE_API INDEX g_iCountry;

float afMaxStep[LOADING_IMAGE_COUNT] = { 16.0f, // First Start
						13.0f, // 주노 0
						13.0f, // 벨피스트 1
						13.0f, // 스트레이아 2 
						13.0f, // 프리키온 3
						13.0f, // 드라탄 4 
						16.0f, // 미노크 탄광 5
						13.0f, // 듀토리얼 6
						13.0f, // 메라크 7
						13.0f, // 길드 전용방 8
						13.0f, // 메라크 던전 9 
						13.0f, // 아자카 협곡 10
						13.0f, // 차원의 문 11
						13.0f, // 망각의 신전 12
						13.0f, // 태국 프리PK존 13
						13.0f, // 이벤트 존....
						13.0f, // 에게하 15
						13.0f, // 에게하 PK존
						13.0f, // 에게하 던전 1~7F
						13.0f, // 에게하 던전 8F
						13.0f, // 에게하 던전 9F
						13.0f, // 에게하 던전 10F
						13.0f, // 드라탄 공성 던전 21
						13.0f, // 몬스터 콤보 존 22
						13.0f, // 스트레이아나 23
						13.0f, // PK 토너먼트 24
						13.0f, // 큐브 25
						13.0f, // 영혼의 동굴 26
						13.0f, // 콰니안 동굴 27
						13.0f, // 골램 동굴 28
						13.0f, // 에보니 탄광 (Ebony mine) 29
						13.0f, // 미스티 협곡(Misty Canyon) 30
						13.0f, // 플로라임 동굴 31
						 };	// Into the World, Start LastChaos 는 제외 


// ----------------------------------------------------------------------------
// Name : GetLoadingCount()
// Desc : written by seo 40815
// ----------------------------------------------------------------------------
float GetLoadingCount ( )
{
	return afMaxStep[g_slZone+1];
}

// ----------------------------------------------------------------------------
// Name : ObtainLoadingData()
// Desc : written by seo 40815
// ----------------------------------------------------------------------------
BOOL ObtainLoadingData( void )
{
	nLoadingStep = 0;

	try 
	{		
		// Load Back Image
		atoIcons[0][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_1_1.tex" ) );	// 쥬노 필드
		atoIcons[0][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_1_2.tex" ) );
    
		atoIcons[1][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_1_1.tex" ) );	// 벨피스트 신전 던젼
		atoIcons[1][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_1_2.tex" ) );

		atoIcons[2][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_1_1.tex" ) );	// 스트레이아 신전 싱글 던젼
		atoIcons[2][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_1_2.tex" ) );    

		atoIcons[3][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_2_1.tex" ) );	// 프로키온 신전 던젼
		atoIcons[3][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_2_2.tex" ) );

		atoIcons[4][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_2_1.tex" ) );	// 드라탄 필드
		atoIcons[4][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_2_2.tex" ) );

		atoIcons[5][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_2_1.tex" ) );	// 미노크 탄광 싱글 던젼
		atoIcons[5][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_2_2.tex" ) );

		atoIcons[6][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_1_1.tex" ) );	// 스트레이아 신전 싱글 던젼
		atoIcons[6][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_1_2.tex" ) );

		atoIcons[7][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_3_1.tex" ) );	// 메라크 필드
		atoIcons[7][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_3_2.tex" ) );

		atoIcons[8][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\g_1_1.tex" ) );	// 길드 전용 방
		atoIcons[8][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\g_1_2.tex" ) );

		atoIcons[9][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_3_1.tex" ) );	// 마르가둠 던젼
		atoIcons[9][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_3_2.tex" ) );

		atoIcons[10][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_3_1.tex" ) );	// 아자카 협곡 싱글 던젼
		atoIcons[10][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_3_2.tex" ) );

		atoIcons[11][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_4_1.tex" ) );	// 차원의 문 싱글 던젼
		atoIcons[11][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_4_2.tex" ) );

		atoIcons[12][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_4_1.tex" ) );	// 망각의 신전 던젼
		atoIcons[12][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_4_2.tex" ) );

		atoIcons[13][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_3_1.tex" ) );	// 마르가둠 pk존 던젼
		if(g_iCountry == HONGKONG)
		{	atoIcons[13][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_3_2_pk.tex" ) );}	//ttos_080506 : 홍콩만 처리
		else atoIcons[13][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_3_2.tex" ) );
		

		atoIcons[14][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Loading00_1.tex" ) );	// OX 이벤트 존
		atoIcons[14][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Loading00_2.tex" ) );

		atoIcons[15][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_4_1.tex" ) );	// 에게하 설정....
		atoIcons[15][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_4_2.tex" ) );
		
		atoIcons[16][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_4_1.tex" ) );	// 에게하 PK존 임시설정....
		atoIcons[16][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_4_2.tex" ) );

		atoIcons[17][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_1.tex" ) );	// 에게하던전 1층....
		atoIcons[17][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_2.tex" ) );
		
		atoIcons[18][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_1.tex" ) );	// 에게하던전 8층....
		atoIcons[18][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_2.tex" ) );

		atoIcons[19][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_1.tex" ) );  // 에게하던전 9층....
		atoIcons[19][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_2.tex" ) );  // 에게하던전 9층....
    
		atoIcons[20][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_1.tex" ) );  // 에게하던전 10층....
		atoIcons[20][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_17_2.tex" ) );  // 에게하던전 10층....

		atoIcons[21][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_21_1.tex" ) );
		atoIcons[21][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_21_2.tex" ) );

		atoIcons[22][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_22_1.tex" ) );	// 몬스터 콤보 존
		atoIcons[22][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_22_2.tex" ) );	// 몬스터 콤보 존

		if (g_iCountry == USA || g_iCountry == BRAZIL || g_iCountry == HONGKONG || g_iCountry == MALAYSIA || g_iCountry == GERMANY)
		{
			atoIcons[23][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_5_1.tex") ); // 스트레이아나
			atoIcons[23][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\f_5_2.tex") ); // 스트레이아나
	
		
			atoIcons[24][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_24_1.tex" ) );	// PK토너먼트존
			atoIcons[24][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_24_2.tex" ) );	// PK토너먼트존

			atoIcons[25][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_25_1.tex" ) );	// 큐브
			atoIcons[25][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_25_2.tex" ) );	// 큐브

			atoIcons[26][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_26_1.tex" ) );	// 큐브
			atoIcons[26][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_26_2.tex" ) );	// 큐브

			atoIcons[27][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_27_1.tex" ) );	// 콰니안 동굴
			atoIcons[27][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_27_2.tex" ) );	// 콰니안 동굴

			atoIcons[28][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_28_1.tex" ) );	// 골렘 동굴
			atoIcons[28][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\s_28_2.tex" ) );	// 골렘 동굴	
		}

		if (g_iCountry == USA || g_iCountry == GERMANY)
		{
			
			atoIcons[29][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_29_1.tex" ) );	// 에보니 탄광
			atoIcons[29][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_29_2.tex" ) );	// 에보니 탄광

			atoIcons[30][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_30_1.tex" ) );	// 미스티 협곡
			atoIcons[30][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_30_2.tex" ) );	// 미스티 협곡

			atoIcons[31][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_31_1.tex" ) );	// 미스티 협곡
			atoIcons[31][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\d_31_2.tex" ) );	// 미스티 협곡			
		}

		// Load First into world
		atoIcons[FIRST_INTO_WORLD][0].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Loading00_1.tex" ) );
		atoIcons[FIRST_INTO_WORLD][1].SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Loading00_2.tex" ) );    

		// Load Classification 
		toClassification.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Classification_15.tex" ) );
		toText.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Text.tex" ) );
		
		// Load Progress Bar
		ptoProgressBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\ProgressBar.tex" ) );
		ptoLoadingBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\LoadingBar.tex" ) );

		ptdProgressBar = (CTextureData*)ptoProgressBar.GetData();
		ptdLoadingBar = (CTextureData*)ptoLoadingBar.GetData();

		FLOAT	fTexWidth	= ptdProgressBar->GetPixWidth();
		FLOAT	fTexHeight	= ptdProgressBar->GetPixHeight();

		// UV Coordinate of Progress Bar Icon
 		rtProgressBar[0].SetUV( 0, 0, 13, 17, fTexWidth, fTexHeight );
		rtProgressBar[1].SetUV( 13, 0, 36, 17, fTexWidth, fTexHeight );
		rtProgressBar[2].SetUV( 49, 0, 62, 17, fTexWidth, fTexHeight );

		fTexWidth	= ptdLoadingBar->GetPixWidth();
		fTexHeight	= ptdLoadingBar->GetPixHeight();

		// UV Coordinate of Loading Bar Icon
 		rtLoadingBar[0].SetUV( 0, 0, 4, 11, fTexWidth, fTexHeight );
		rtLoadingBar[1].SetUV( 4, 0, 7, 11, fTexWidth, fTexHeight );
		rtLoadingBar[2].SetUV( 7, 0, 11, 11, fTexWidth, fTexHeight );
				
	} 
	catch( char *strError ) 
	{
		CPrintF("%s\n", strError);
		return FALSE;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ReleaseLoadingData()
// Desc : 
// Last Edit : 이기환 ( 12. 10 )
// ----------------------------------------------------------------------------
void ReleaseLoadingData( void )
{
	int nCount = LOADING_IMAGE_COUNT;

	while( nCount-- )
	{
		atoIcons[nCount][1].SetData(NULL);
		atoIcons[nCount][0].SetData(NULL);
	}

	ptoProgressBar.SetData ( NULL );
	ptdProgressBar = NULL;
	ptoLoadingBar.SetData ( NULL );
	ptdLoadingBar = NULL;

	toText.SetData ( NULL );

	toClassification.SetData ( NULL );
	//toClassification = NULL;

}

// ----------------------------------------------------------------------------
// Name : RenderLoading()
// Desc : 
// Last Edit : 이기환 ( 12. 10 )
// ----------------------------------------------------------------------------
void RenderLoading( CDrawPort *pdp, ULONG ulLevelMask, CProgressHookInfo *pphi)
{
	static float fMaxStep = 0.0f;
	static int nLoadingImage = FIRST_INTO_WORLD;

	// 최초 실행
	if( pphi->phi_phsStatus == PHS_INIT )	
	{
		if( !ObtainLoadingData() )
		{
			ReleaseLoadingData();
			return;
		}
		
		fMaxStep = GetLoadingCount();

		if ( g_slZone == -1 )	// Start
		{
			nLoadingImage = FIRST_INTO_WORLD;
		}
		else
		{
			if ( !g_bFirstIntoWorld )
			{
				// Render Back Image Number
				//nLoadingImage = ( ZoneInfo().GetZoneType( g_slZone ) + 1 ); // 필드 타입 +1 (start추가) 입니다.
				nLoadingImage = g_slZone;
			}
			else // 처음 월드로 진입할 때는 다른 이미지 출력
			{
				nLoadingImage = g_slZone;
				g_bFirstIntoWorld = FALSE;
			}
		}
	}

	
	// Render Back Image ==============================================================================
	
  	PIX pixdpw = pdp->GetWidth();
	PIX pixdph = pdp->GetHeight();

	FLOAT fPointStretch	= 0.0f;;	// 축소 시 아래 쪽 그림이 출력될 위치
	FLOAT fStretch		= 1.0f;	// 가로대 세로 축소 비율 ( 800/600, 1024/768 ... )
			
	PIX pixOriginalX	= 1024;		// 원본 이미지의 크기
	PIX pixOriginalY	= 768;

	PIX ImageSizeY512	= 512;		// 커팅 된 이미지 싸이즈
	PIX ImageSizeY256	= 256;

	PIX pixSX			= 0;		// 이미지 출력 위치
	PIX pixSY			= 0;

	float fPersent		= 1.0f;
	
	if( pixdpw == 1024 && pixdph == 768 )	// 1024 x 768 : 그냥 뿌러 주자
	{
		pdp->PutTexture( &atoIcons[nLoadingImage][0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixOriginalX,ImageSizeY512)), C_WHITE|255 );
		pdp->PutTexture( &atoIcons[nLoadingImage][1], PIXaabbox2D( PIX2D(pixSX,ImageSizeY512), PIX2D(pixOriginalX,pixOriginalY)), C_WHITE|255 );
	}
	else if ( pixdph < 768 ) // 1024 x 768 보다 작을 때 ( 800 x 600 ) : 세로 크기를 기준으로 축소 한두 가운데 정렬
	{
		fStretch		= 1.3333f;
		
		fPersent = (float)pixdph / (float)pixOriginalY;
		
		// 축소
		fPointStretch	= (FLOAT)ImageSizeY512 / (FLOAT)pixOriginalY;
		PIX	pixPointY	= ( pixdph * fPointStretch );

		// 비율로 조정된 가로 위치 ( 소수점 3째 자리에서 반올림 )
		pixOriginalX	= pixdph *  fStretch + ROUND;
		
		// 가로 가운데 정렬
		pixSX			= ( pixdpw - pixOriginalX ) / 2;
		
		pdp->PutTexture( &atoIcons[nLoadingImage][0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixdpw-pixSX,pixPointY)), C_WHITE|255);
		pdp->PutTexture( &atoIcons[nLoadingImage][1], PIXaabbox2D( PIX2D(pixSX,pixPointY), PIX2D(pixdpw-pixSX,pixdph)), C_WHITE|255);

	}
	else // 1024 x 768 보다 클 때	// 가운대로 띄우자
	{
		// 출력 위치를 가운데로 보정
		pixSX = ( pixdpw - pixOriginalX ) / 2;
		pixSY = ( pixdph - pixOriginalY ) / 2;
		
		pdp->PutTexture( &atoIcons[nLoadingImage][0], PIXaabbox2D( PIX2D(pixSX, pixSY), PIX2D(pixSX+pixOriginalX,pixSY+ImageSizeY512)), C_WHITE|255);
		pdp->PutTexture( &atoIcons[nLoadingImage][1], PIXaabbox2D( PIX2D(pixSX, pixSY+ImageSizeY512), PIX2D(pixSX+pixOriginalX, pixSY+pixOriginalY)), C_WHITE|255);
	}
	
	
	
	

	// Render Classification ============================================================================ FRANCE_SPAIN_CLOSEBETA_NA_20081124
	if ( nLoadingImage == 0 && g_iCountry != BRAZIL && g_iCountry != GERMANY && g_iCountry != SPAIN && g_iCountry != FRANCE && g_iCountry != POLAND) // start 화면
	{
		float fWidth = toClassification.GetWidth();		// * fPersent;	// 등급표시는 축소 안함
		float fHeight = toClassification.GetHeight();	// * fPersent;		
			
		// 출력 위치 : 배경 이미지에 상관 없이 화면 우측 상단
		// PIX	pixX = pixdpw - ( fWidth + fWidth / 2 );
		// PIX	pixY = ( fHeight / 2 );

		// 배경 이미지의 우측 상단
		PIX	pixX = pixSX + pixOriginalX - ( fWidth + fWidth / 4 );
		PIX	pixY = pixSY + ( fHeight / 4 );
				
		pdp->PutTexture( &toClassification, PIXaabbox2D( PIX2D(pixX, pixY), PIX2D(pixX+fWidth,pixY+fHeight)), C_WHITE|255);	
	}

	// Render Text
	if ( nLoadingImage == FIRST_INTO_WORLD ) // 처음 월드로 들어갈 때 화면
	{
		float fWidth = toText.GetWidth();	// * fPersent;	// 등급표시는 축소 안함
		float fHeight = toText.GetHeight();	// * fPersent;		
			
		// 출력 위치 : 배경 이미지에 상관 없이 화면 우측 상단
		// PIX	pixX = pixdpw - ( fWidth + fWidth / 2 );
		// PIX	pixY = ( fHeight / 2 );

		// 배경 이미지의 우측 상단
		PIX	pixX = pixSX + PIX_TEXT;
		PIX	pixY = pixSY + PIY_TEXT;
				
		pdp->PutTexture( &toText, PIXaabbox2D( PIX2D(pixX, pixY), PIX2D(pixX+fWidth,pixY+fHeight)), C_WHITE|255);	


	}

	if ( nLoadingImage == FIRST_INTO_WORLD || nLoadingImage == 14) 
	{
		
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 49 * fPersent;		// 이미지 출력 위치
		float m_pY				= pixSY + 736 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// 단계 하나가 차지 하는 이미지 길이
		
		float fSideImageWidth	= 12.0f * fPersent;				// 시작 이미지의 크기
		float fSideImageHeight	= 16.0f * fPersent; 
		
		float fProgressLength	= 902.0f / fMaxStep * fPersent;	// 한단계의 로딩이 이동해야 하는 길이
		
		float m_eX = 0.0f;
		
		// Render Start ...
		pdp->InitTextureData( ptdProgressBar );
		
		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[0].U0, rtProgressBar[0].V0, rtProgressBar[0].U1, rtProgressBar[0].V1,
			0xFFFFFFFF );
		
		// Progress Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
		else 
		{
			fUniStepWidth = fProgressLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
		}
		
		// Progress End Position
		m_eX  = m_pX + ( nLoadingStep * fProgressLength ) + fUniStepWidth;
		
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtProgressBar[1].U0, rtProgressBar[1].V0, rtProgressBar[1].U1, rtProgressBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[2].U0, rtProgressBar[2].V0, rtProgressBar[2].U1, rtProgressBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
		
	}
	else
	{			
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 149 * fPersent;		// 이미지 출력 위치
		float m_pY				= pixSY + 612 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// 단계 하나가 차지 하는 이미지 길이
		
		float fSideImageWidth	= 11.0f * fPersent;				// 시작 이미지의 크기
		float fSideImageHeight	= 11.0f * fPersent; 
		
		float fLoadingLength	= 716.0f / fMaxStep * fPersent;	// 한단계의 로딩이 이동해야 하는 길이
		
		float m_eX = 0.0f;
		
		// Render Start ...
		pdp->InitTextureData( ptdLoadingBar );
		
		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[0].U0, rtLoadingBar[0].V0, rtLoadingBar[0].U1, rtLoadingBar[0].V1,
			0xFFFFFFFF );
		
		// Loading Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
		else 
		{
			fUniStepWidth = fLoadingLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
		}
		
		// Loading End Position
		m_eX  = m_pX + ( nLoadingStep * fLoadingLength ) + fUniStepWidth;
		
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtLoadingBar[1].U0, rtLoadingBar[1].V0, rtLoadingBar[1].U1, rtLoadingBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[2].U0, rtLoadingBar[2].V0, rtLoadingBar[2].U1, rtLoadingBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
	}
	
	// free textures used in map rendering
	if( pphi->phi_phsStatus == PHS_END )
	{
		ReleaseLoadingData();
		
		// Date : 2005-11-03(오후 1:46:43), By Lee Ki-hwan
		// 로딩이 끝났으면 서버에 게임 시작 메세지 전송 
		if( _pNetwork->GetLoadingZone() == g_slZone && g_slZone != -1 ) 
		{
			_pUIMgr->SetUIGameState( UGS_GAMELOADING );
			_pNetwork->SendGameStart();
		}
	}
}

