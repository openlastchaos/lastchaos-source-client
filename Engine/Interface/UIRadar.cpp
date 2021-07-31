#include "stdh.h"
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/LocalDefine.h>

#define RADIAN	(ANGLE_180 / PI)

// Show ToolTip
#define STT_OPTION		0
#define STT_MAP			1
#define STT_SIGNAL		2
#define WORLD_TIME_FONT_SIZE  7
extern INDEX g_iCountry;


// ----------------------------------------------------------------------------
// Name : CUIRadar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIRadar::CUIRadar()
{
	m_szCoord[0] = NULL;
	m_bShowOptionPopup = FALSE;
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
	m_bInsideMouse = FALSE;
	m_bSignalBtnOn = FALSE;
	m_year = m_month = m_hour = m_day= 0;
	m_cipher = 1;
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIRadar()
// Desc : Destructorㄱㄱㄱ
// ----------------------------------------------------------------------------
CUIRadar::~CUIRadar()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	nWidth = RADAR_WIDTH;
	nHeight = 200;
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTop.SetRect( 0, 0, 140, 53 );
	m_rcCompassIn.SetRect( -52, -52, 52, 52 ); 
	m_rcCompassOut.SetRect( -76, -76, 76, 76 );
	m_rcOption.SetRect( 0, 3, 18, 130 );
	
	m_rcPoint[RADAR_PARTY].SetRect( -5, -5, 6, 6 );
	m_rcPoint[RADAR_GUILD].SetRect( -5, -5, 6, 6 );
	m_rcPoint[RADAR_NPC].SetRect( -4, -5, 5, 6 );
	m_rcPoint[RADER_QUESTSUCCESS].SetRect( -8, -8, 8, 8 );
	m_rcPoint[RADER_QUESTNPC].SetRect( -8, -8, 8, 8 );
	m_rcPoint[RADAR_HELPNPC].SetRect(-4, -5, 5, 6);
	m_rcPoint[RADAR_MOB].SetRect( -2, -2, 2, 2 );
	m_rcMyPoint.SetRect( -6, -11, 7, 8 );
	m_rcTarget.SetRect( -2, -2, 3, 3 );
	m_rcSignal.SetRect( -12, -12, 12, 12 );
	m_rcOutSignal.SetRect( -10, -5, 11, 5 );

	if(g_iCountry == BRAZIL)				// 070531 ttos : 브라질 날짜 표기 수정 (일/월/년 시)
	{
		m_rcYear.SetRect(88,32,94,42);
		m_rcMonth.SetRect(66,32,72,42);
		m_rcDay.SetRect(45,32,51,42);
		m_rcTime.SetRect(116,32,122,42);

	}else
	{
		// world time wooss ---------------------------->> 060415
		m_rcYear.SetRect(54,32,61,42);
		m_rcMonth.SetRect(71,32,78,42);
		m_rcDay.SetRect(88,32,94,42);
		m_rcTime.SetRect(120,32,126,42);
		// ---------------------------------------------<<
	}

	
	// Create radar texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Radar.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	/***
	m_rtBackTop.SetUV( 0, 0, 140, 53, fTexWidth, fTexHeight );
	m_rtCompassIn.SetUV( 152, 143, 256, 247, fTexWidth, fTexHeight );
	m_rtCompassOut.SetUV( 0, 54, 152, 206, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_PARTY].SetUV( 178, 23, 189, 34, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_GUILD].SetUV( 178, 39, 189, 50, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_NPC].SetUV( 179, 55, 188, 66, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_HELPNPC].SetUV(163, 52, 174,66,fTexWidth, fTexHeight);
	m_rtPoint[RADER_QUESTNPC].SetUV(159, 67, 175,84,fTexWidth, fTexHeight);
	m_rtPoint[RADER_QUESTSUCCESS].SetUV(158, 85, 174,102,fTexWidth, fTexHeight);
	m_rtPoint[RADAR_MOB].SetUV( 182, 75, 186, 79, fTexWidth, fTexHeight );
	m_rtMyPoint.SetUV( 177, 2, 190, 21, fTexWidth, fTexHeight );
	m_rtTarget.SetUV( 189, 74, 194, 79, fTexWidth, fTexHeight );
	m_rtSignal.SetUV( 176, 86, 200, 110, fTexWidth, fTexHeight );
	m_rtOutSignal.SetUV( 194, 45, 215, 55, fTexWidth, fTexHeight );
	m_rtTargetAni.SetUV( 194, 2, 215, 23, fTexWidth, fTexHeight );
	***/
	m_rtBackTop.SetUV( 360, 248, 499, 304, fTexWidth, fTexHeight );
	m_rtCompassIn.SetUV( 517, 376, 623, 481, fTexWidth, fTexHeight );
	m_rtCompassOut.SetUV( 337, 340, 495, 497, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_PARTY].SetUV( 663, 408, 674, 419, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_GUILD].SetUV( 648, 408, 659, 419, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_NPC].SetUV( 665, 427, 674, 438, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_HELPNPC].SetUV( 650, 427, 659, 438,fTexWidth, fTexHeight);
	m_rtPoint[RADER_QUESTNPC].SetUV( 649, 448, 663, 463,fTexWidth, fTexHeight);
	m_rtPoint[RADER_QUESTSUCCESS].SetUV( 649, 467, 663, 481,fTexWidth, fTexHeight);
	m_rtPoint[RADAR_MOB].SetUV( 654, 395, 658, 399, fTexWidth, fTexHeight );
	m_rtMyPoint.SetUV( 687, 383, 698, 400, fTexWidth, fTexHeight );
	m_rtTarget.SetUV( 661, 395, 666, 400, fTexWidth, fTexHeight );
	m_rtSignal.SetUV( 682, 447, 706, 471, fTexWidth, fTexHeight );
	m_rtOutSignal.SetUV( 683, 427, 704, 437, fTexWidth, fTexHeight );
	//m_rtTargetAni.SetUV( 194, 2, 215, 23, fTexWidth, fTexHeight );
	m_rtTargetAni.SetUV( 684, 481, 703, 500, fTexWidth, fTexHeight );


	// Tool tip
	//m_rtToolTipL.SetUV( 219, 0, 226, 22, fTexWidth, fTexHeight );
	//m_rtToolTipM.SetUV( 229, 0, 231, 22, fTexWidth, fTexHeight );
	//m_rtToolTipR.SetUV( 234, 0, 241, 22, fTexWidth, fTexHeight );
	m_rtToolTipL.SetUV( 239, 253, 246, 270, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 246, 253, 328, 270, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 328, 253, 335, 270, fTexWidth, fTexHeight );

	// Option popup
	//m_rtOptionL.SetUV( 219, 24, 226, 96, fTexWidth, fTexHeight );
	//m_rtOptionM.SetUV( 229, 24, 231, 96, fTexWidth, fTexHeight );
	//m_rtOptionR.SetUV( 234, 24, 241, 96, fTexWidth, fTexHeight );
	m_rtOptionL.SetUV( 239, 253, 246, 270, fTexWidth, fTexHeight );
	m_rtOptionM.SetUV( 246, 253, 328, 270, fTexWidth, fTexHeight );
	m_rtOptionR.SetUV( 328, 253, 335, 270, fTexWidth, fTexHeight );

	// Option button
	//m_btnOption.Create( this, CTString( "" ), 19, 4, 14, 13 );
	//m_btnOption.SetUV( UBS_IDLE, 143, 13, 157, 26, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 158, 13, 172, 26, fTexWidth, fTexHeight );
	m_btnOption.Create( this, CTString( "" ), 13, 6, 14, 13 );
	m_btnOption.SetUV( UBS_IDLE, 451, 215, 465, 228, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 466, 215, 480, 228, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Map button
	//m_btnMap.Create( this, CTString( "" ), 38, 4, 14, 13 );
	//m_btnMap.SetUV( UBS_IDLE, 143, 28, 157, 41, fTexWidth, fTexHeight );
	//m_btnMap.SetUV( UBS_CLICK, 158, 28, 172, 41, fTexWidth, fTexHeight );
	m_btnMap.Create( this, CTString( "" ), 32, 6, 14, 13 );
	m_btnMap.SetUV( UBS_IDLE, 451, 230, 465, 243, fTexWidth, fTexHeight );
	m_btnMap.SetUV( UBS_CLICK, 466, 230, 480, 243, fTexWidth, fTexHeight );
	m_btnMap.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMap.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnSignal.Create( this, CTString( "" ), 9, 27, 19, 19 );
	//m_btnSignal.SetUV( UBS_IDLE, 212, 95, 231, 114, fTexWidth, fTexHeight );
	//m_btnSignal.SetUV( UBS_CLICK, 232, 95, 251, 114, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_IDLE, 488, 226, 505, 243, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_CLICK, 508, 226, 525, 243, fTexWidth, fTexHeight );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_DISABLE );

	// world time wooss --------------------------------------->> 060415
	//int tv_size = 0;
	int tv_size = 655;
	for(int i=0; i<10; i++){
		//m_rtFigure[i].SetUV(tv_size,214,tv_size+WORLD_TIME_FONT_SIZE,224,fTexWidth,fTexHeight);
		m_rtFigure[i].SetUV(tv_size,367,tv_size+WORLD_TIME_FONT_SIZE,377,fTexWidth,fTexHeight);
		tv_size += WORLD_TIME_FONT_SIZE+1;
	}

	// --------------------------------------------------------<<
	
	// Calcluate size of option popup
	int	nMaxSize = _S( 444, "파티" ).Length();
	int	nSize = _S( 445, "길드" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 446, "NPC" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 448, "몬스터" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nMaxSize *= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();

	// Option check buttons
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	int	nPosY = RADAR_OPTION_BTN_SY;
	nSize = nMaxSize + 10;
	for( int iOpt = 0; iOpt < RADAR_OPTION_TOTAL; iOpt++ )
	{
		m_cbtnOption[iOpt].Create( this, RADAR_OPTION_BTN_SX, nPosY, 11, 11, CTString( "" ), TRUE, nSize, nSize + 17 );
		//m_cbtnOption[iOpt].SetUV( UCBS_CHECK, 143, 0, 154, 11, fTexWidth, fTexHeight );
		//m_cbtnOption[iOpt].SetUV( UCBS_NONE, 156, 0, 167, 11, fTexWidth, fTexHeight );
		m_cbtnOption[iOpt].SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
		m_cbtnOption[iOpt].SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
		m_cbtnOption[iOpt].CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
		m_cbtnOption[iOpt].CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
		m_cbtnOption[iOpt].SetCheck( TRUE );
		m_cbtnOption[iOpt].SetTextColor( 0, 0xC5C5C5FF );
		m_cbtnOption[iOpt].SetTextColor( 1, 0xC5C5C5FF );
		nPosY += RADAR_OPTION_OFFSETY;
	}

	m_cbtnOption[RADAR_PARTY].SetText( _S( 444, "파티" ) );
	m_cbtnOption[RADAR_GUILD].SetText( _S( 445, "길드" ) );
	m_cbtnOption[RADAR_NPC].SetText( _S( 446, "NPC" ) );
	m_cbtnOption[RADAR_HELPNPC].SetText( _S(1748,"안내"));
	m_cbtnOption[RADER_QUESTNPC].SetText(_S(106,"퀘스트"));
	m_cbtnOption[RADER_QUESTSUCCESS].SetText(_S(106,"퀘스트"));
	m_cbtnOption[RADAR_MOB].SetText( _S( 448, "몬스터" ) );

	// Calcluate size of option popup
	nSize += 22;
	m_nOptionIconSX = 10 - nSize;
	m_rcOption.Left = -nSize;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - GetWidth(), pixMinJ );
	_pUIBuff->SetMyBadBuffPos( m_nPosX - 2, 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
static FLOAT	fSin, fCos;
void CUIRadar::Render()
{
	FLOAT	fLeftCos, fLeftSin, fRightCos, fRightSin;
	FLOAT	fTopSin, fTopCos, fBottomSin, fBottomCos;
	FLOAT	fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4;
	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;
	fSin = Sin( _pNetwork->MyCharacterInfo.camera_angle );
	fCos = Cos( _pNetwork->MyCharacterInfo.camera_angle );

	// Set radar texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	// Top
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY,
										m_nPosX + m_rcTop.Right, m_nPosY + m_rcTop.Bottom,
										m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	// Inner Compass
	fLeftCos = m_rcCompassIn.Left * fCos;
	fLeftSin = m_rcCompassIn.Left * fSin;
	fRightCos = m_rcCompassIn.Right * fCos;
	fRightSin = m_rcCompassIn.Right * fSin;
	fTopSin = m_rcCompassIn.Top * fSin;
	fTopCos = m_rcCompassIn.Top * fCos;
	fBottomSin = m_rcCompassIn.Bottom * fSin;
	fBottomCos = m_rcCompassIn.Bottom * fCos;

	fX1 = fLeftCos - fTopSin;
	fY1 = fLeftSin + fTopCos;
	fX2 = fLeftCos - fBottomSin;
	fY2 = fLeftSin + fBottomCos;
	fX3 = fRightCos - fBottomSin;
	fY3 = fRightSin + fBottomCos;
	fX4 = fRightCos - fTopSin;
	fY4 = fRightSin + fTopCos;
	_pUIMgr->GetDrawPort()->AddTexture( nX + fX1, nY + fY1, m_rtCompassIn.U0, m_rtCompassIn.V0, 0xFFFFFFFF,
										nX + fX2, nY + fY2, m_rtCompassIn.U0, m_rtCompassIn.V1, 0xFFFFFFFF,
										nX + fX3, nY + fY3, m_rtCompassIn.U1, m_rtCompassIn.V1, 0xFFFFFFFF,
										nX + fX4, nY + fY4, m_rtCompassIn.U1, m_rtCompassIn.V0, 0xFFFFFFFF );

	// My point
	_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcMyPoint.Left, nY + m_rcMyPoint.Top,
										nX + m_rcMyPoint.Right, nY + m_rcMyPoint.Bottom,
										m_rtMyPoint.U0, m_rtMyPoint.V0, m_rtMyPoint.U1, m_rtMyPoint.V1,
										0xFFFFFFFF );
	
	// Render location of objects
	RenderObjectLocation();

	// Outer Compass
	fLeftCos = m_rcCompassOut.Left * fCos;
	fLeftSin = m_rcCompassOut.Left * fSin;
	fRightCos = m_rcCompassOut.Right * fCos;
	fRightSin = m_rcCompassOut.Right * fSin;
	fTopSin = m_rcCompassOut.Top * fSin;
	fTopCos = m_rcCompassOut.Top * fCos;
	fBottomSin = m_rcCompassOut.Bottom * fSin;
	fBottomCos = m_rcCompassOut.Bottom * fCos;

	fX1 = fLeftCos - fTopSin;
	fY1 = fLeftSin + fTopCos;
	fX2 = fLeftCos - fBottomSin;
	fY2 = fLeftSin + fBottomCos;
	fX3 = fRightCos - fBottomSin;
	fY3 = fRightSin + fBottomCos;
	fX4 = fRightCos - fTopSin;
	fY4 = fRightSin + fTopCos;
	// wooss 060413 move down radarY 
	_pUIMgr->GetDrawPort()->AddTexture( nX + fX1, nY + fY1, m_rtCompassOut.U0, m_rtCompassOut.V0, 0xFFFFFFFF,
										nX + fX2, nY + fY2, m_rtCompassOut.U0, m_rtCompassOut.V1, 0xFFFFFFFF,
										nX + fX3, nY + fY3, m_rtCompassOut.U1, m_rtCompassOut.V1, 0xFFFFFFFF,
										nX + fX4, nY + fY4, m_rtCompassOut.U1, m_rtCompassOut.V0, 0xFFFFFFFF );

	// Render Signal 
	RenderSignal();
	
	// Option button
	m_btnOption.Render();

	// Map button
	m_btnMap.Render();

	// Signal Button 
	m_btnSignal.Render();

	

	// Option popup
	if( m_bShowOptionPopup )
	{
		// Background
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom,
											m_rtOptionL.U0, m_rtOptionL.V0, m_rtOptionL.U1, m_rtOptionL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom,
											m_rtOptionM.U0, m_rtOptionM.V0, m_rtOptionM.U1, m_rtOptionM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Bottom,
											m_rtOptionR.U0, m_rtOptionR.V0, m_rtOptionR.U1, m_rtOptionR.V1,
											0xFFFFFFFF );

		// Icons
		nX = m_nPosX + m_nOptionIconSX;
		nY = m_nPosY + RADAR_OPTION_ICON_SY;
		for( int i = 0; i < RADAR_OPTION_TOTAL; i++ )
		{
			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[i].Left, nY + m_rcPoint[i].Top,
												nX + m_rcPoint[i].Right, nY + m_rcPoint[i].Bottom,
												m_rtPoint[i].U0, m_rtPoint[i].V0, m_rtPoint[i].U1, m_rtPoint[i].V1,
												0xFFFFFFFF );
			nY += RADAR_OPTION_OFFSETY;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		// Set radar texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );

		// Check buttons
		for( int iOption = 0; iOption < RADAR_OPTION_TOTAL; iOption++ )
			m_cbtnOption[iOption].Render();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		// Set radar texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	}

	// Text in radar
	sprintf( m_szCoord, "%4d,%-4d", int(_pNetwork->MyCharacterInfo.x), int(_pNetwork->MyCharacterInfo.z) );
	_pUIMgr->GetDrawPort()->PutTextCharExCX( m_szCoord, 9, m_nPosX + RADAR_COORD_TEXT_CX,
												m_nPosY + RADAR_COORD_TEXT_SY, 0x72D02EFF );

#ifdef DISPLAY_SERVER_INFO
	CTString strServerInfo;
	strServerInfo.PrintF("%s Ch. %d", _pUIMgr->GetSelServer()->GetServerGroupName(_pNetwork->m_iServerGroup), _pNetwork->m_iServerCh );
	_pUIMgr->GetDrawPort()->PutTextExCX( strServerInfo, m_nPosX +this->GetWidth()/2, m_nPosY + this->GetHeight() -5, 0xF2F2F2B2 );
#endif
	

	// Display LC Time 
	if(m_year>=0){
		DisplayNum(m_year+1,m_rcYear);
	}
	if(m_month>=0){
		DisplayNum(m_month+1,m_rcMonth);
	}
	if(m_day>=0){
		DisplayNum(m_day+1,m_rcDay);
	}
	if(m_hour>=0){
		DisplayNum(m_hour,m_rcTime);
	}


/*
	if(m_year>=0){
		m_cipher = 1;
		tv_c = CountCipher(m_year);
		char *tv_str;
		tv_str = new(char[40]);
		itoa(m_year,tv_str,10);
		for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
			tv_num=tv_str[tv_c-tv_i-1]-'0';
				_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+m_rcYear.Left -6*tv_i, m_nPosY+m_rcYear.Top,
											m_nPosX+m_rcYear.Right -6*tv_i, m_nPosY+m_rcYear.Bottom,
											m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
											0xFFFFFFFF );			
		}
				
	}
	*/
/*	if(m_month>=0){
		tv_c = CountCipher(m_month);
		char *tv_str;
		tv_str = new(char[40]);
		itoa(m_month,tv_str,10);
		for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
			tv_num=tv_str[tv_c-tv_i]-'0';
				_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+m_rcMonth.Left -6*tv_i, m_nPosY+m_rcMonth.Top,
											m_nPosX+m_rcMonth.Right -6*tv_i, m_nPosY+m_rcMonth.Bottom,
											m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
											0xFFFFFFFF );			
		}
				
	}
	if(m_day>=0){
		tv_c = CountCipher(m_day);
		char *tv_str;
		tv_str = new(char[40]);
		itoa(m_day,tv_str,10);
		for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
			tv_num=tv_str[tv_c-tv_i]-'0';
				_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+m_rcDay.Left -6*tv_i , m_nPosY+m_rcDay.Top,
											m_nPosX+m_rcDay.Right-6*tv_i, m_nPosY+m_rcDay.Bottom,
											m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
											0xFFFFFFFF );			
		}
				
	}
	if(m_hour>=0){
		tv_c = CountCipher(m_hour);
		char *tv_str;
		tv_str = new(char[40]);
		itoa(m_hour,tv_str,10);
		for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
			tv_num=tv_str[tv_c-tv_i]-'0';
				_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+m_rcTime.Left -6*tv_i, m_nPosY+m_rcTime.Top,
											m_nPosX+m_rcTime.Right -6*tv_i, m_nPosY+m_rcTime.Bottom,
											m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
											0xFFFFFFFF );			
		}
				
	}*/

//	_pUIMgr->GetDrawPort()->AddTexture(   m_nPosX+m_rcYear.Left , m_nPosY+m_rcYear.Top,
//											  m_nPosX+m_rcYear.Right , m_nPosY+m_rcYear.Bottom,
//											m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
//											0xFFFFFFFF );			

	
/*	
	CTString tv_time;
	tv_time.PrintF("%d.%d.%d    %d",m_year,m_month,m_day,m_hour);
	_pUIMgr->GetDrawPort()->PutTextCharExCX( tv_time, tv_time.Length(), m_nPosX + RADAR_COORD_TEXT_CX,
												m_nPosY + RADAR_COORD_TEXT_SY + 25, 0xFFFFFFFF );
*/
  


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

// ----------------------------------------------------------------------------
// Name : RenderObjectLocation()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::RenderObjectLocation()
{
	CEntity			*penObject;
	FLOAT3D			vObjectPos;
	FLOAT			fX, fZ;
	int				nX, nY;
	SBYTE			sbLayerDiff;

	static FLOAT	fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE;
	static FLOAT	fRatio = RADAR_VIEW_RADIUS / RADAR_VIEW_DISTANCE;
	FLOAT			fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT			fZpc = _pNetwork->MyCharacterInfo.z;
	int				nCX = m_nPosX + RADAR_CENTER_OFFSETX;
	int				nCY = m_nPosY + RADAR_CENTER_OFFSETY;
	SBYTE			sbLayerpc = _pNetwork->MyCharacterInfo.yLayer;

	// Mob
	if( m_cbtnOption[RADAR_MOB].IsChecked() )
	{
		INDEX	ctMob = _pNetwork->ga_srvServer.srv_amtMob.Count();
		for( INDEX iObj = 0; iObj < ctMob; iObj++ )
		{
			// Get target mob
			CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iObj];
			penObject = mt.mob_pEntity;

			if( mt.IsNPC() )
				continue;

			sbLayerDiff = mt.mob_yLayer - sbLayerpc;
			if( sbLayerDiff < -1 || sbLayerDiff > 1 )
				continue;

			vObjectPos = penObject->en_plPlacement.pl_PositionVector;

			// Test distance
			fX = vObjectPos(1) - fXpc;
			fZ = vObjectPos(3) - fZpc;
			if( fX * fX + fZ * fZ > fSqrDist )
				continue;

			fX *= fRatio;
			fZ *= fRatio;

			nX = nCX + ( fX * fCos - fZ * fSin );
			nY = nCY + ( fX * fSin + fZ * fCos );

			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_MOB].Left,  nY + m_rcPoint[RADAR_MOB].Top,
												nX + m_rcPoint[RADAR_MOB].Right, nY + m_rcPoint[RADAR_MOB].Bottom,
												m_rtPoint[RADAR_MOB].U0, m_rtPoint[RADAR_MOB].V0,
												m_rtPoint[RADAR_MOB].U1, m_rtPoint[RADAR_MOB].V1,
												0xFFFFFFFF );
		}
	}

	// Npc
	if( m_cbtnOption[RADAR_NPC].IsChecked() )
	{
		INDEX	ctMob = _pNetwork->ga_srvServer.srv_amtMob.Count();
		for( INDEX iObj = 0; iObj < ctMob; iObj++ ) 
		{
			// Get target mob
			CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iObj];
			penObject = mt.mob_pEntity;

			if( !mt.IsNPC() )
				continue;

			sbLayerDiff = mt.mob_yLayer - sbLayerpc;
			if( sbLayerDiff < -1 || sbLayerDiff > 1 )
				continue;

			vObjectPos = penObject->en_plPlacement.pl_PositionVector;

			// Test distance
			fX = vObjectPos(1) - fXpc;
			fZ = vObjectPos(3) - fZpc;
			if( fX * fX + fZ * fZ > fSqrDist )
				continue;

			fX *= fRatio;
			fZ *= fRatio;

			nX = nCX + ( fX * fCos - fZ * fSin );
			nY = nCY + ( fX * fSin + fZ * fCos );

			if (mt.mob_iType == _pUIMgr->m_nHelpNpc_Index) // NPC 안내 시스템 레이더 표시
			{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_HELPNPC].Left,  nY + m_rcPoint[RADAR_HELPNPC].Top,
												nX + m_rcPoint[RADAR_HELPNPC].Right, nY + m_rcPoint[RADAR_HELPNPC].Bottom,
												m_rtPoint[RADAR_HELPNPC].U0, m_rtPoint[RADAR_HELPNPC].V0,
												m_rtPoint[RADAR_HELPNPC].U1, m_rtPoint[RADAR_HELPNPC].V1,
												0xFFFFFFFF );
			}else if(CQuestSystem::Instance().TestNPCForQuest(mt.mob_iType) == CQuestSystem::NQT_HAVE_QUEST)
			{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADER_QUESTNPC].Left,  nY + m_rcPoint[RADER_QUESTNPC].Top,
												nX + m_rcPoint[RADER_QUESTNPC].Right, nY + m_rcPoint[RADER_QUESTNPC].Bottom,
												m_rtPoint[RADER_QUESTNPC].U0, m_rtPoint[RADER_QUESTNPC].V0,
												m_rtPoint[RADER_QUESTNPC].U1, m_rtPoint[RADER_QUESTNPC].V1,
												0xFFFFFFFF );
					

			}else if(CQuestSystem::Instance().TestNPCForQuest(mt.mob_iType) == CQuestSystem::NQT_CAN_PRIZE)
			{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADER_QUESTSUCCESS].Left,  nY + m_rcPoint[RADER_QUESTSUCCESS].Top,
												nX + m_rcPoint[RADER_QUESTSUCCESS].Right, nY + m_rcPoint[RADER_QUESTSUCCESS].Bottom,
												m_rtPoint[RADER_QUESTSUCCESS].U0, m_rtPoint[RADER_QUESTSUCCESS].V0,
												m_rtPoint[RADER_QUESTSUCCESS].U1, m_rtPoint[RADER_QUESTSUCCESS].V1,
												0xFFFFFFFF );
				
			}else
			{
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_NPC].Left,  nY + m_rcPoint[RADAR_NPC].Top,
												nX + m_rcPoint[RADAR_NPC].Right, nY + m_rcPoint[RADAR_NPC].Bottom,
												m_rtPoint[RADAR_NPC].U0, m_rtPoint[RADAR_NPC].V0,
												m_rtPoint[RADAR_NPC].U1, m_rtPoint[RADAR_NPC].V1,
												0xFFFFFFFF );
			}

			
		}
	}

	// Guild
	if( m_cbtnOption[RADAR_GUILD].IsChecked() )
	{
		INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
		for( INDEX iObj = 0; iObj < ctCha; iObj++ ) 
		{
			// Get target player
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iObj];

			if( ct.cha_lGuildIndex != GUILD_MEMBER_NOMEMBER && 
				ct.cha_lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
			{
				sbLayerDiff = ct.cha_yLayer - sbLayerpc;
				if( sbLayerDiff < -1 || sbLayerDiff > 1 )
					continue;

				penObject = ct.cha_pEntity;
				vObjectPos = penObject->en_plPlacement.pl_PositionVector;

				// Test distance
				fX = vObjectPos(1) - fXpc;
				fZ = vObjectPos(3) - fZpc;
				if( fX * fX + fZ * fZ > fSqrDist )
					continue;

				fX *= fRatio;
				fZ *= fRatio;

				nX = nCX + ( fX * fCos - fZ * fSin );
				nY = nCY + ( fX * fSin + fZ * fCos );

				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_GUILD].Left,  nY + m_rcPoint[RADAR_GUILD].Top,
													nX + m_rcPoint[RADAR_GUILD].Right, nY + m_rcPoint[RADAR_GUILD].Bottom,
													m_rtPoint[RADAR_GUILD].U0, m_rtPoint[RADAR_GUILD].V0,
													m_rtPoint[RADAR_GUILD].U1, m_rtPoint[RADAR_GUILD].V1,
													0xFFFFFFFF );
			}
		}
	}

	// Party
	if( m_cbtnOption[RADAR_PARTY].IsChecked() )
	{
		for( int iParty = 0; iParty < _pUIMgr->GetParty()->GetMemberCount(); iParty++ )
		{
			sbLayerDiff = _pUIMgr->GetParty()->GetLayer( iParty ) - sbLayerpc;
			if( sbLayerDiff < -1 || sbLayerDiff > 1 )
				continue;

			// Test distance
			fX = _pUIMgr->GetParty()->GetPosX( iParty ) - fXpc;
			fZ = _pUIMgr->GetParty()->GetPosZ( iParty ) - fZpc;
			if( fX * fX + fZ * fZ > fSqrDist )
			{
				FLOAT	fResizeRatio = RADAR_VIEW_DISTANCE / sqrtf( fX * fX + fZ * fZ );
				fX *= fResizeRatio;
				fZ *= fResizeRatio;
			}

			fX *= fRatio;
			fZ *= fRatio;

			nX = nCX + ( fX * fCos - fZ * fSin );
			nY = nCY + ( fX * fSin + fZ * fCos );

			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_PARTY].Left,  nY + m_rcPoint[RADAR_PARTY].Top,
												nX + m_rcPoint[RADAR_PARTY].Right, nY + m_rcPoint[RADAR_PARTY].Bottom,
												m_rtPoint[RADAR_PARTY].U0, m_rtPoint[RADAR_PARTY].V0,
												m_rtPoint[RADAR_PARTY].U1, m_rtPoint[RADAR_PARTY].V1,
												0xFFFFFFFF );
		}
	}

#define RADAR_DEFENSE_COLOR 0x0000FFFF		// 수성측 Point 색
#define	RADAR_ATTACK_COLOR	0xFF0000FF		// 공성측 Point 색

	// 공성
	// 현재 공성중이고 내가 공성에 참여중이라면
	if( _pUISWDoc->IsWar() && _pNetwork->MyCharacterInfo.sbJoinFlagMerac != WCJF_NONE ) 
	{
		COLOR colRenderType = 0xFFFFFFFF;
		INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
		for( INDEX iObj = 0; iObj < ctCha; iObj++ ) 
		{
			// Get target player
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iObj];
			
			switch( ct.cha_sbJoinFlagMerac )
			{
			// 참여하고 있지 않음
			case WCJF_NONE: 
				continue;
				break;
		
			// 수성
			case WCJF_OWNER:
			case WCJF_DEFENSE_GUILD:
			case WCJF_DEFENSE_CHAR:
				colRenderType = RADAR_DEFENSE_COLOR;
				break;

			// 공성측
			case WCJF_ATTACK_GUILD:
			case WCJF_ATTACK_CHAR:
				colRenderType = RADAR_ATTACK_COLOR;
				break;
			default :
				continue;
			}

// ! 길드는 따로 처리 			
//			if( m_cbtnOption[RADAR_GUILD].IsChecked() ) // 길드원이면 길드원 우선 표시
//			{
//				if( ct.cha_lGuildIndex != GUILD_MEMBER_NOMEMBER && 
//					ct.cha_lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
//				{
//					bRenderGuild = TRUE;
//				}
//			}

			sbLayerDiff = ct.cha_yLayer - sbLayerpc;
			if( sbLayerDiff < -1 || sbLayerDiff > 1 )
				continue;

			penObject = ct.cha_pEntity;
			vObjectPos = penObject->en_plPlacement.pl_PositionVector;

			// Test distance
			fX = vObjectPos(1) - fXpc;
			fZ = vObjectPos(3) - fZpc;
			if( fX * fX + fZ * fZ > fSqrDist )
				continue;

			fX *= fRatio;
			fZ *= fRatio;

			nX = nCX + ( fX * fCos - fZ * fSin );
			nY = nCY + ( fX * fSin + fZ * fCos );

		
			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcPoint[RADAR_MOB].Left,  nY + m_rcPoint[RADAR_MOB].Top,
											nX + m_rcPoint[RADAR_MOB].Right, nY + m_rcPoint[RADAR_MOB].Bottom,
											m_rtPoint[RADAR_MOB].U0, m_rtPoint[RADAR_MOB].V0,
											m_rtPoint[RADAR_MOB].U1, m_rtPoint[RADAR_MOB].V1,
											colRenderType );
		} // GuildWar for
	} // GuildWar if			

	// Target
	if( _pNetwork->_TargetInfo.bIsActive )
	{
		// Get target
		penObject = _pNetwork->_TargetInfo.pen_pEntity;

		if( penObject == NULL ) return;

		vObjectPos = penObject->en_plPlacement.pl_PositionVector;

		// Test distance
		fX = vObjectPos(1) - fXpc;
		fZ = vObjectPos(3) - fZpc;
		if( fX * fX + fZ * fZ <= fSqrDist )
		{
			fX *= fRatio;
			fZ *= fRatio;

			nX = nCX + ( fX * fCos - fZ * fSin );
			nY = nCY + ( fX * fSin + fZ * fCos );

			_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcTarget.Left,  nY + m_rcTarget.Top,
												nX + m_rcTarget.Right, nY + m_rcTarget.Bottom,
												m_rtTarget.U0, m_rtTarget.V0, m_rtTarget.U1, m_rtTarget.V1,
												0xFFFFFFFF );

			static __int64	llOldTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static __int64	llElapsedTime = 0;
			__int64			llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			llElapsedTime += llCurTime - llOldTime;
			llOldTime = llCurTime;

			if( llElapsedTime > 1800 )
				llElapsedTime = 0;

			FLOAT	fAniRatio = llElapsedTime / 1800.0f;
			if( fAniRatio < 0.65f )
			{
				if( fAniRatio < 0.5f )
				{
					FLOAT	fSin = sinf( 3.141592f * fAniRatio );
					FLOAT	fSize = 2.0f + 11.0f * fSin;
					UBYTE	ubAlpha = 0xFF * ( 1.0f - fSin );
					_pUIMgr->GetDrawPort()->AddTexture( nX - fSize,  nY - fSize, nX + fSize, nY + fSize,
														m_rtTargetAni.U0, m_rtTargetAni.V0, m_rtTargetAni.U1, m_rtTargetAni.V1,
														0xFFFFFF00 | ubAlpha );
				}

				if( fAniRatio > 0.15f )
				{
					FLOAT	fSin = sinf( 3.141592f * ( fAniRatio - 0.15f ) );
					FLOAT	fSize = 2.0f + 11.0f * fSin;
					UBYTE	ubAlpha = 0xFF * ( 1.0f - fSin );
					_pUIMgr->GetDrawPort()->AddTexture( nX - fSize,  nY - fSize, nX + fSize, nY + fSize,
														m_rtTargetAni.U0, m_rtTargetAni.V0, m_rtTargetAni.U1, m_rtTargetAni.V1,
														0xFFFFFF00 | ubAlpha );
				}
			}
		}
	} // Target

}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		extern INDEX	g_iEnterChat;
		int	nInfoX, nInfoY, nWidth, nHeight;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case STT_OPTION:		// Option
			m_strToolTip = _S( 442, "정보표시 옵션" );
			m_btnOption.GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnOption.GetWidth();
			nHeight = m_btnOption.GetHeight();
			break;

		case STT_MAP:		// Map
			if( g_iEnterChat )
				m_strToolTip.PrintF( "%s %s", _S( 190, "지도" ), CTString( "(Alt+W)" ) );
			else
				m_strToolTip.PrintF( "%s %s", _S( 190, "지도" ), CTString( "(W,Alt+W)" ) );
			m_btnMap.GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnMap.GetWidth();
			nHeight = m_btnMap.GetHeight();
			break;
		case STT_SIGNAL:		// Signal
			m_strToolTip = _S(2222, "시그널" );
			m_btnOption.GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnOption.GetWidth();
			nHeight = m_btnOption.GetHeight();
			nHeight = m_btnMap.GetHeight();
			break;
		}

		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY += nHeight + 2;

		if( nInfoX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoX += _pUIMgr->GetMaxI() - ( nInfoX + nInfoWidth );

		m_rcToolTip.SetRect( nInfoX, nInfoY, nInfoX + nInfoWidth, nInfoY + nInfoHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRadar::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL	bInOptionPopup = FALSE;	// If mouse is in option popup or not

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Popup of option
			if( m_bShowOptionPopup && IsInsideRect( nX, nY, m_rcOption ) )
			{
				bInOptionPopup = TRUE;
			}
			else
			{
				// Close option popup
				if( bInOptionPopup )
				{
					bInOptionPopup = FALSE;
					m_bShowOptionPopup = FALSE;
				}
				// Option button
				else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ShowToolTip( TRUE, STT_OPTION );
					return WMSG_SUCCESS;
				}
				// Map button
				else if( m_btnMap.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ShowToolTip( TRUE, STT_MAP );
					return WMSG_SUCCESS;
				}
				else if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL ) 
				{
					ShowToolTip( TRUE, STT_SIGNAL );
					return WMSG_SUCCESS;
				}
			}
		
			if( IsInside( nX, nY ) )
			{
				m_bInsideMouse = TRUE;

				if( m_bSignalBtnOn ) 
				{
					SetSignalOn( true );
					return WMSG_SUCCESS;
				}
				else
				{
					_pUIMgr->SetMouseCursorInsideUIs();
				}
			}
			else // 레이더 영역 밖으로 나가면 취소됨.
			{
				SetSignalOn( FALSE );
				m_bInsideMouse = FALSE;
			}
			
			// Hide tool tip
			ShowToolTip( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			// Popup of option
			if( m_bShowOptionPopup && IsInsideRect( nX, nY, m_rcOption ) )
			{
				for( int iOption = 0; iOption < RADAR_OPTION_TOTAL; iOption++ )
				{
					if( m_cbtnOption[iOption].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						_pUIMgr->RearrangeOrder( UI_RADAR, TRUE );
						return WMSG_SUCCESS;
					}
				}

				return WMSG_SUCCESS;
			}
			else if( IsInside( nX, nY ) )
			{
				// Option button
				if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Map button
				else if( m_btnMap.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL ) 
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_RADAR, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// If radar isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Option button
				if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_bShowOptionPopup = !m_bShowOptionPopup;
					}

					return WMSG_SUCCESS;
				}
				// Map button
				else if( ( wmsgResult = m_btnMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_pUIMgr->GetMap()->ToggleVisible();
					}

					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnSignal.MouseMessage( pMsg ) ) != WMSG_FAIL ) 
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( !IsPossibleSignal() ) return WMSG_SUCCESS;
						// 시그널 버튼을 누르면 레이더상에 최종 위치 표시해줌
						m_Signal.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
						m_Signal.bVisible = TRUE;

						// Signal 셋팅
						SetSignalOn( TRUE );
					}

					return WMSG_SUCCESS;
				}
				
				if( IsInside( nX, nY ) )
				{
					if( !m_bSignalBtnOn )
					{
						return WMSG_SUCCESS;
					}

					SetSignalOn( FALSE );
					
					FLOAT fX = nX;
					FLOAT fY = nY;

					if( RadarToWorld( fX, fY ) )
					{
						SendSignal( fX, fY );
						_pUIMgr->RearrangeOrder( UI_RADAR, TRUE );
					}
					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					SetSignalOn( FALSE );
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bShowOptionPopup && IsInsideRect( nX, nY, m_rcOption ) )
				return WMSG_SUCCESS;
			else if( IsInsideRect( nX, nY, m_rcTop ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

#define SIGNAL_TIME				5.0f								// 시그널 표시 시간 
#define SIGNAL_COUNT			5.0f								// 시그널 표시 횟수 
#define SIGNAL_UNITTIME			( SIGNAL_TIME / SIGNAL_COUNT )		// 한번 표시가 할때 걸리는 시간
#define SIGNAL_UNITTIME_HALF	( SIGNAL_UNITTIME / 2 ) 

//------------------------------------------------------------------------------
// CUIRadar::RenderSignal
// Explain:  
// Date : 2005-10-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::RenderSignal()
{
	static FLOAT fAngleOffSet = 270.0f; // 방향 표시기가 ^를 향하기 있도록 하기위해서 270도를 꺽어준다.
	static FLOAT fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE;
	static FLOAT fRatio = RADAR_VIEW_RADIUS / RADAR_VIEW_DISTANCE;

	if( !m_Signal.bVisible ) return;

	// 표시 시간 및 2d 효과 조정
	DOUBLE dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE dDealy		= dElapsedTime - m_Signal.dStartTime;

	COLOR	colBlend;
	
	if( dDealy > SIGNAL_TIME ) 
	{
		m_Signal.bVisible = FALSE;
		return;
	}

	BYTE	bBlend;
	BOOL	bFirst = TRUE;
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
		fTemp = 0xFF * ( fTemp );
		bBlend = 0xFF - fTemp;

		if( nCount <= 0 ) 
		{
			colOriBlend = ( 0xFFFFFFFF - 0xFF ) | bBlend;
		}
	}
	else 
	{
		DOUBLE fTemp = ( dDealy ) / SIGNAL_UNITTIME_HALF;
		fTemp = 0xFF * ( fTemp );
		bBlend = fTemp;
	}
		
	colBlend = ( 0xFFFFFFFF - 0xFF ) | bBlend;

	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;

	// Get My position
	FLOAT	fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT	fZpc = _pNetwork->MyCharacterInfo.z;

	// 케릭터의 위치를 원점으로 한 시그널의 위치
	FLOAT fX = m_Signal.fX - fXpc;
	FLOAT fZ = m_Signal.fZ - fZpc;
	
	FLOAT fDist = fX * fX + fZ * fZ;
	
	// Position & direction of character
	FLOAT	fSin = Sin( _pNetwork->MyCharacterInfo.camera_angle );
	FLOAT	fCos = Cos( _pNetwork->MyCharacterInfo.camera_angle );

	// Render Signal ( 레이더의 범위안에 있는 경우 ) 
	if( fDist <= fSqrDist ) 
	{
		fX *= fRatio;
		fZ *= fRatio;

		nX += ( fX * fCos - fZ * fSin );
		nY += ( fX * fSin + fZ * fCos );

		FLOAT fWidth = (  m_rcSignal.Right - m_rcSignal.Left ) / 2;
		FLOAT fHeight = ( m_rcSignal.Bottom - m_rcSignal.Top ) / 2;

		FLOAT	fOriWidth = fWidth;
		FLOAT	fOriHeight = fHeight;

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

		_pUIMgr->GetDrawPort()->AddTexture( nX - fWidth,  nY - fHeight,
											nX + fWidth, nY + fHeight,
											m_rtSignal.U0, m_rtSignal.V0,
											m_rtSignal.U1, m_rtSignal.V1,
											colBlend );
	
		_pUIMgr->GetDrawPort()->AddTexture( nX - fOriWidth,  nY - fOriHeight,
											nX + fOriWidth, nY + fOriHeight,
											m_rtSignal.U0, m_rtSignal.V0,
											m_rtSignal.U1, m_rtSignal.V1,
											colOriBlend );
		return;
	}

	// 레이더의 표시 범위안에 있지 않는다면 레이더 외각에 방향으로 표시
	
	// 케릭터의 X축을 중심으로 시그널의 각도 구하기
	FLOAT	angle = GetAngle( fX, fZ, 1.0f, 0.0f );	 // X 축 중심
	angle *= RADIAN;

	if( fZ < 0 ) // 360도 
	{
		angle = 360 - angle;
	}

	angle -= fAngleOffSet;  // 방향 표시기가 ^를 향하기 있도록 하기위해서 90도를 꺽어준다.

	FLOAT	fSin2 = Sin( _pNetwork->MyCharacterInfo.camera_angle + angle );
	FLOAT	fCos2 = Cos( _pNetwork->MyCharacterInfo.camera_angle + angle );
	
	FLOAT	fLeftCos = m_rcOutSignal.Left * fCos2;
	FLOAT	fLeftSin = m_rcOutSignal.Left * fSin2;
	FLOAT	fRightCos = m_rcOutSignal.Right * fCos2;
	FLOAT	fRightSin = m_rcOutSignal.Right * fSin2;
	FLOAT	fTopSin = m_rcOutSignal.Top * fSin2;
	FLOAT	fTopCos = m_rcOutSignal.Top * fCos2;
	FLOAT	fBottomSin = m_rcOutSignal.Bottom * fSin2;
	FLOAT	fBottomCos = m_rcOutSignal.Bottom * fCos2;

	FLOAT	fX1 = fLeftCos - fTopSin;
	FLOAT	fY1 = fLeftSin + fTopCos;
	FLOAT	fX2 = fLeftCos - fBottomSin;
	FLOAT	fY2 = fLeftSin + fBottomCos;
	FLOAT	fX3 = fRightCos - fBottomSin;
	FLOAT	fY3 = fRightSin + fBottomCos;
	FLOAT	fX4 = fRightCos - fTopSin;
	FLOAT	fY4 = fRightSin + fTopCos;
	
	FLOAT	fResizeRatio = RADAR_VIEW_DISTANCE / sqrtf( fDist );
	
	fX *= fResizeRatio;
	fZ *= fResizeRatio;

	fX *= fRatio;
	fZ *= fRatio;

	nX += ( fX * fCos - fZ * fSin );
	nY += ( fX * fSin + fZ * fCos );

	_pUIMgr->GetDrawPort()->AddTexture( nX + fX1, nY + fY1, m_rtOutSignal.U0, m_rtOutSignal.V0, colBlend,
										nX + fX2, nY + fY2, m_rtOutSignal.U0, m_rtOutSignal.V1, colBlend,
										nX + fX3, nY + fY3, m_rtOutSignal.U1, m_rtOutSignal.V1, colBlend,
										nX + fX4, nY + fY4, m_rtOutSignal.U1, m_rtOutSignal.V0, colBlend );
}

//------------------------------------------------------------------------------
// CUIRadar::SetSignal
// Explain:  레이더의 시그널의 항상 최신 것을 기준으로 1개만 표시된다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SetSignal( float fX, float fZ )
{
	m_Signal.bVisible = TRUE;
	
	m_Signal.fX = fX;
	m_Signal.fZ = fZ;

	m_Signal.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
}

//------------------------------------------------------------------------------
// CUIRadar::SendSignal
// Explain: 서버에 시그널을 보냄
// 맵의 시그널 추가 루틴을 통해서 정송됨.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SendSignal( float fX, float fZ )
{
	if( IsPossibleSignal() )
	{
		_pUIMgr->GetMap()->SendSignal( fX, fZ );
	}

	// 임시 코딩 바로 자신의 시그널로 보내도록
	//SetSignal( fX, fZ );
}
		

//------------------------------------------------------------------------------
// CUIRadar::RadarToWorld
// Explain:  레이더 상의 좌표를 World 좌표로 변경 
// * Return Value : 입력된 자표가 레이더 상에 있는지
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::RadarToWorld( float& fX, float& fZ )
{
	static FLOAT fRatio = RADAR_VIEW_RADIUS / RADAR_VIEW_DISTANCE;
	static FLOAT fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE;

	double nTempX;
	double nTempZ;
	
	double	fSin = Sin( -_pNetwork->MyCharacterInfo.camera_angle ); // 현재 케릭의 각도에 역 회전 방향
	double	fCos = Cos( -_pNetwork->MyCharacterInfo.camera_angle );
	
	fX -= m_nPosX + RADAR_CENTER_OFFSETX; // 레이더 중심 좌표를 기준( 레이터 상의 케릭터의 위치 )
	fZ -= m_nPosY + RADAR_CENTER_OFFSETY;

	fX /= fRatio;	// 축척 
	fZ /= fRatio;

	nTempX = ( fX * fCos - fZ * fSin ); // 회전 
	nTempZ = ( fX * fSin + fZ * fCos );

	fX = nTempX;
	fZ = nTempZ;

	// Get My position
	FLOAT	fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT	fZpc = _pNetwork->MyCharacterInfo.z;
	
	if( ( fX * fX + fZ * fZ ) <= fSqrDist )
	{
		fX += fXpc; // 케릭터의 위치가 중심이기 때문에 Offset 조정
		fZ += fZpc;
	
		return true;
	}
		
	return false;
}

//------------------------------------------------------------------------------
// CUIRadar::IsPossibleSignal
// Explain:  시그널을 보낼 수 있는지 확인
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::IsPossibleSignal()
{
	// 길드장, 부장인경우 
	// 공성 맵인 경우 
	LONG nRanking = _pNetwork->MyCharacterInfo.lGuildPosition;

	if( nRanking != GUILD_MEMBER_BOSS && nRanking != GUILD_MEMBER_VICE_BOSS )
		return false; 

	if( !IsPossibleCastleRadar() ) 
		return false; 

	return true;
}

//------------------------------------------------------------------------------
// IsPossibleCastleRadar
// Explain: 공성 레이더를 사용할 수 있는 상황인지 확인 ( 버튼 활성화 여부 )
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::IsPossibleCastleRadar()
{
	if( !_pUISWDoc->IsWar() || _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return false;
	
	if( _pNetwork->MyCharacterInfo.zoneNo != 7 )
		return false;

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::SetSignalOn
// Explain:  시그널 변수를 셋팅하고 마우스 커서를 변경 합니다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SetSignalOn( bool bOn ) 
{
	if( !bOn )
	{
		if( m_bSignalBtnOn ) 
		{
			m_bSignalBtnOn = FALSE; 
			_pUIMgr->SetMouseCursorInsideUIs();
		}	
	}
	else
	{
		if( !IsPossibleSignal() ) return;

		m_bSignalBtnOn = TRUE;
		_pUIMgr->SetMouseCursorInsideUIs( UMCT_SIGNAL );
	}

}


//------------------------------------------------------------------------------
//  CUIRadar::KeyMessage
// Explain: Ctrl키를 눌렀을 때 시그널을 보낼 수 있도록 KeyMessage를 받는다 
// * 일반적인 UIWindow는 KEYUP 메세지를 받지 않는다. ( 조합키를 사용하는 곳이 없음)
// *  UIMgr에서 MAP과 Rar만 예외 처리해서 KEYUP메세지를 받도록 하였음
// Date : 2005-10-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUIRadar::KeyMessage( MSG *pMsg )
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


// ----------------------------------------------------------------------------
// Name : CountCipher()
// Desc : return their cipher
// ----------------------------------------------------------------------------
int CUIRadar::CountCipher( int tv_time)
{
	CTString str;
	int tv_sum = tv_time/pow(10,m_cipher);
	if( tv_sum ) {
		m_cipher ++;
		str.PrintF("%d %d ",tv_sum,m_cipher);
		CountCipher(tv_sum);
	}
	str.PrintF("%d %d %d ",tv_time,tv_sum,m_cipher);
	return m_cipher;
	
}

void CUIRadar::DisplayNum( int tv_time,UIRect tv_rect)
{
	int tv_c,tv_i,tv_num;
	m_cipher = 1;
	tv_c = CountCipher(tv_time);
	char *tv_str;
	tv_str = new(char[40]);
	itoa(tv_time,tv_str,10);

	CTString str;
	str.PrintF("%d %d ",tv_c,tv_time);
	for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
		tv_num=tv_str[tv_c-tv_i-1]-'0';
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+tv_rect.Left -WORLD_TIME_FONT_SIZE*tv_i, m_nPosY+tv_rect.Top,
										m_nPosX+tv_rect.Right -WORLD_TIME_FONT_SIZE*tv_i, m_nPosY+tv_rect.Bottom,
										m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
										0xFFFFFFFF );			
	}
	
}