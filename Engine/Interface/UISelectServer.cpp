#include "stdh.h"
#include <Engine/Interface/UISelectServer.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/World/World.h>
#include <algorithm>
#include <Engine/GameState.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Interface/UILogin.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/LocalDefine.h>

//#define USER_NUM_FULL		300
//#define USER_NUM_BUSY		280

// Date : 2005-05-17(오후 4:54:18), By Lee Ki-hwan
extern INDEX g_iCountry;
extern CDrawPort *_pdpMain;

#define TEMP_SERVER_LIST // define 서버군 번호 순서로 정렬..
// -----wooss 060425------------------------------------------->>
#define NOTICE_SIZE_Y	112

#ifndef RECOMMEND_SERVER_EVENT
#undef NOTICE_SIZE_Y
#define NOTICE_SIZE_Y	0
#endif

#define RECOMMEND_POS_X   133
// -------------------------------------------------<<

// ----------------------------------------------------------------------------
// Name : CUISelectServer()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISelectServer::CUISelectServer()
{
	m_nRecentServerGroup = -1;
	m_nRecentServer = -1;
	m_nRecomendSvr = -1;	
	m_nRecomendSvrOrder = -1;

	m_astrServerName[0] = _S( 796, "카타르" ); 
	m_astrServerName[1] = _S( 797, "크리스" ); 
	m_astrServerName[2] = _S( 798, "하르페" ); 
	m_astrServerName[3] = _S( 799, "칸자르" ); 
	m_astrServerName[4] = _S( 800, "세이버" ); 
	m_astrServerName[5] = _S( 801, "스페도나" ); 
	m_astrServerName[6] = _S( 802, "파냐드" ); 
	m_astrServerName[7] = _S( 803, "마노플" ); 
	m_astrServerName[8] = _S( 804, "커틀러스" ); 
	m_astrServerName[9] = _S( 805, "샤브르" ); 
	
	// 대만 2차 서버군 
	m_astrServerName[10] = _S( 1846, "2st-Group1-tw" );
	m_astrServerName[11] = _S( 1847, "2st-Group1-tw" );
	m_astrServerName[12] = _S( 1848, "2st-Group1-tw" );
	m_astrServerName[13] = _S( 1849, "2st-Group1-tw" );
	m_astrServerName[14] = _S( 1850, "2st-Group1-tw" );
	m_astrServerName[15] = _S( 1851, "2st-Group1-tw" );
	m_astrServerName[16] = _S( 1852, "2st-Group1-tw" );
	m_astrServerName[17] = _S( 1853, "2st-Group1-tw" );
	m_astrServerName[18] = _S( 1854, "2st-Group1-tw" );
	m_astrServerName[19] = _S( 1855, "2st-Group1-tw" );

	// 2st Local 중국에 추가되는 2차 지역에 대한 서버군 Name
	m_astrServerName[20] = _S( 1395, "2st-Group1" );
	m_astrServerName[21] = _S( 1396, "2st-Group2" );
	m_astrServerName[22] = _S( 1397, "2st-Group3" );
	m_astrServerName[23] = _S( 1398, "2st-Group4" );
	m_astrServerName[24] = _S( 1399, "2st-Group5" );
	m_astrServerName[25] = _S( 1400, "2st-Group6" );
	m_astrServerName[26] = _S( 1401, "2st-Group7" );
	m_astrServerName[27] = _S( 1402, "2st-Group8" );
	m_astrServerName[28] = _S( 1403, "2st-Group9" );
	m_astrServerName[29] = _S( 1404, "2st-Group10" );
	m_astrServerName[30] = _S( 246, "테스트 서버" );
	m_astrServerName[31] = _S( 246, "테스트 서버" );
	m_astrServerName[32] = _S( 246, "테스트 서버" );
	m_astrServerName[33] = _S( 246, "테스트 서버" );
	m_astrServerName[34] = _S( 246, "테스트 서버" );
	m_astrServerName[35] = _S( 246, "테스트 서버" );
	m_astrServerName[36] = _S( 246, "테스트 서버" );
	m_astrServerName[37] = _S( 246, "테스트 서버" );
	m_astrServerName[38] = _S( 246, "테스트 서버" );
	m_astrServerName[39] = _S( 246, "테스트 서버" );
	
}

// ----------------------------------------------------------------------------
// Name : ~CUISelectServer()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelectServer::~CUISelectServer()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
#ifdef RECOMMEND_SERVER_EVENT
	SetSize( nWidth , nHeight + NOTICE_SIZE_Y );
#else
	SetSize( nWidth, nHeight );
#endif

	// Create portal texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SelectServer.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
#ifdef 	RECOMMEND_SERVER_EVENT
	m_rtBackTop.SetUV( 0, 0, 416, 28, fTexWidth, fTexHeight ); // 46- >24
	m_rtBackTop1.SetUV( 0, 23, 416, 27, fTexWidth, fTexHeight );
	// 공지 표시 창
	m_rtBackTop2.SetUV( 20, 50, 30, 60, fTexWidth, fTexHeight );
	// 서버군 , 서버 표시 
	m_rtBackMiddle0.SetUV( 0, 29, 416, 59, fTexWidth, fTexHeight );
#else 
	m_rtBackTop.SetUV( 0, 0, 416, 46, fTexWidth, fTexHeight );
#endif
	m_rtBackMiddle.SetUV( 0, 50, 416, 62, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 66, 416, 105, fTexWidth, fTexHeight );

	// 추천이벤트 이미지 wooss 060426
	if(g_iCountry == BRAZIL)
	{
		m_rtRecommend.SetUV(83,112,145,128,fTexWidth, fTexHeight );	// UV of Recommend Image
	}else
	{
		m_rtRecommend.SetUV(17,112,80,128,fTexWidth, fTexHeight );	// UV of Recommend Image
	}	

	// Ok button
	m_btnOK.Create( this, _S( 249, "접속" ), 274, 205 + NOTICE_SIZE_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 449, 0, 512, 21, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 449, 23, 512, 44, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetEnable(FALSE);

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 340, 205+ NOTICE_SIZE_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 449, 0, 512, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 449, 23, 512, 44, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Server group list box
	m_lbServerGroup.Create( this, 14, 47 + NOTICE_SIZE_Y, 183, 150, 15, 6, 2, 1, TRUE );
	m_lbServerGroup.CreateScroll( TRUE, 0, 0, 9, 150, 9, 7, 0, 0, 10 );
	m_lbServerGroup.SetSelBar( 183, 15, 3, 112, 11, 128, fTexWidth, fTexHeight );
	m_lbServerGroup.SetOverColor( 0xF8E1B5FF );
	m_lbServerGroup.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbServerGroup.SetScrollUpUV( UBS_IDLE, 482, 49, 491, 56, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollUpUV( UBS_CLICK, 492, 49, 501, 56, fTexWidth, fTexHeight );
	m_lbServerGroup.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbServerGroup.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbServerGroup.SetScrollBarTopUV( 503, 47, 512, 57, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollBarMiddleUV( 503, 58, 512, 61, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollBarBottomUV( 503, 62, 512, 72, fTexWidth, fTexHeight );
	// Down button
	m_lbServerGroup.SetScrollDownUV( UBS_IDLE, 482, 59, 491, 66, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollDownUV( UBS_CLICK, 492, 59, 501, 66, fTexWidth, fTexHeight );
	m_lbServerGroup.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbServerGroup.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	// Server list box
	m_lbServer.Create( this, 212, 47+ NOTICE_SIZE_Y, 181, 150, 15, 6, 2, 2, TRUE );
	m_lbServer.CreateScroll( TRUE, 0, 0, 9, 150, 9, 7, 0, 0, 10 );
	m_lbServer.SetSelBar( 183, 15, 3, 112, 11, 128, fTexWidth, fTexHeight );
	m_lbServer.SetColumnPosX( 1, 133 );		// wooss 051002 For thai 137 -> 133
	m_lbServer.SetOverColor( 0xF8E1B5FF );
	m_lbServer.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbServer.SetScrollUpUV( UBS_IDLE, 482, 49, 491, 56, fTexWidth, fTexHeight );
	m_lbServer.SetScrollUpUV( UBS_CLICK, 492, 49, 501, 56, fTexWidth, fTexHeight );
	m_lbServer.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbServer.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbServer.SetScrollBarTopUV( 503, 47, 512, 57, fTexWidth, fTexHeight );
	m_lbServer.SetScrollBarMiddleUV( 503, 58, 512, 61, fTexWidth, fTexHeight );
	m_lbServer.SetScrollBarBottomUV( 503, 62, 512, 72, fTexWidth, fTexHeight );
	// Down button
	m_lbServer.SetScrollDownUV( UBS_IDLE, 482, 59, 491, 66, fTexWidth, fTexHeight );
	m_lbServer.SetScrollDownUV( UBS_CLICK, 492, 59, 501, 66, fTexWidth, fTexHeight );
	m_lbServer.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbServer.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

//	m_nRecomendSvr = FindRecomendSvr();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Render()
{
	CDrawPort *pdp = _pUIMgr->GetDrawPort();

	// Set select server texture
	pdp->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
#ifdef RECOMMEND_SERVER_EVENT
	int	nY = m_nPosY + NOTICE_SIZE_Y + 29;
	int	nY2 = m_nPosY + m_nHeight - 39;
	pdp->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY +28,
						m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
						0xFFFFFFFF );
	pdp->AddTexture( m_nPosX, m_nPosY +28, m_nPosX + m_nWidth, nY,
						m_rtBackTop1.U0, m_rtBackTop1.V0, m_rtBackTop1.U1, m_rtBackTop1.V1,
						0xFFFFFFFF );
	// 공지 창
	pdp->AddTexture( m_nPosX + 13 , m_nPosY +29, m_nPosX + m_nWidth - 13, nY -3,
						m_rtBackTop2.U0, m_rtBackTop2.V0, m_rtBackTop2.U1, m_rtBackTop2.V1,
						0xFFFFFFFF );
//	pdp->AddTexture( m_nPosX + 18 , m_nPosY +29, m_nPosX + m_nWidth - 18, nY,
//						m_rtBackTop2.U0, m_rtBackTop2.V0, m_rtBackTop2.U1, m_rtBackTop2.V1,
//						0xFFFFFFFF );
	// 서버군, 서버 표시
	pdp->AddTexture( m_nPosX, nY , m_nPosX + m_nWidth, nY + 29,
						m_rtBackMiddle0.U0, m_rtBackMiddle0.V0, m_rtBackMiddle0.U1, m_rtBackMiddle0.V1,
						0xFFFFFFFF );
	pdp->AddTexture( m_nPosX, nY + 29 , m_nPosX + m_nWidth, nY2 ,
						m_rtBackMiddle.U0, m_rtBackMiddle.V0, m_rtBackMiddle.U1, m_rtBackMiddle.V1,
						0xFFFFFFFF );
	pdp->AddTexture( m_nPosX, nY2, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
						m_rtBackBottom.U0, m_rtBackBottom.V0, m_rtBackBottom.U1, m_rtBackBottom.V1,
						0xFFFFFFFF );

	// 추천 서버 이미지
	if( m_nRecomendSvr > 0)
	{
		pdp->AddTexture( m_nPosX+RECOMMEND_POS_X , nY+4+(15*m_nRecomendSvrOrder) , m_nPosX +RECOMMEND_POS_X+ 63, nY + 4 + (15*(m_nRecomendSvrOrder+1)),
						m_rtRecommend.U0, m_rtRecommend.V0, m_rtRecommend.U1, m_rtRecommend.V1,
						0xFFFFFFFF );
	}
#else
	int	nY = m_nPosY + 47;
	int	nY2 = m_nPosY + m_nHeight - 39;
	pdp->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, nY,
						m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
						0xFFFFFFFF );
	pdp->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY2,
						m_rtBackMiddle.U0, m_rtBackMiddle.V0, m_rtBackMiddle.U1, m_rtBackMiddle.V1,
						0xFFFFFFFF );
	pdp->AddTexture( m_nPosX, nY2, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
						m_rtBackBottom.U0, m_rtBackBottom.V0, m_rtBackBottom.U1, m_rtBackBottom.V1,
						0xFFFFFFFF );

#endif
	// Ok button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Server group list box
	m_lbServerGroup.Render();

	// Server list box
	m_lbServer.Render();

	// Render all elements
	pdp->FlushRenderingQueue();

	// text render
	pdp->PutTextEx( _S( 250, "서버 선택" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, m_nPosY + SELECTSERVER_TITLE_TEXT_OFFSETY , 0xFFFFFFFF );
	pdp->PutTextEx( _S( 251, "서버군" ),
					m_nPosX + SELECTSERVER_GROUP_TEXT_POSX, m_nPosY + SELECTSERVER_GROUP_TEXT_POSY + NOTICE_SIZE_Y, 0xFFB500FF );
	pdp->PutTextEx( _S( 370, "서버" ),
					m_nPosX + SELECTSERVER_SERVER_TEXT_POSX, m_nPosY + SELECTSERVER_SERVER_TEXT_POSY + NOTICE_SIZE_Y, 0xFFB500FF );
#ifdef RECOMMEND_SERVER_EVENT	
	// --Add Message -------------------------------------------------->>
	int tv_y = m_nPosY + SELECTSERVER_TITLE_TEXT_OFFSETY + 35;
	pdp->PutTextEx( _S(2618, "♣추천 서버란?" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFAAFFFF );
	tv_y+=20;
	pdp->PutTextEx( _S(2619, "매일 정오에 교체가 되며, 해당 서버에서 플레이를 하면 30레벨" ), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pdp->PutTextEx( _S(2620, "이하의 캐릭터는 사냥시 신비한 효과를 주는 버프 아이템을 얻을" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pdp->PutTextEx( _S(2621, "수 있으며, 모든 유저에 대해서 1시간 간격으로 추첨을 통하여 "), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pdp->PutTextEx( _S(2622, "문스톤 10개를 드리는 이벤트 입니다."), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	
	// ----------------------------------------------------------------<<
#endif
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISelectServer::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbServerGroup.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbServer.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
//			if( IsInside( nX, nY ) )
			{
				if( m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnOK.MouseMessage(pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( ( wmsgResult = m_lbServerGroup.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SetServerList();
				}
				else if( ( wmsgResult = m_lbServer.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						m_btnOK.SetEnable( TRUE );
				}

				_pUIMgr->RearrangeOrder( UI_SEL_SERVER, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If selection server isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					_pSound->Mute();
//#define RESTRICT_SOUND 	
					Close();
// wooss 060209 test JAPAN
//#define AUTO_LOGIN
//#ifdef AUTO_LOGIN
					extern BOOL g_bAutoLogin;
					if(g_bAutoLogin)
					{
						_pGameState->Running()		= FALSE;
						_pGameState->QuitScreen()	= TRUE;
						_pUIMgr->SetUIGameState( UGS_NONE );
					}
//#endif
#ifdef RESTRICT_SOUND
					extern BOOL _bNTKernel;
	//				if( _bNTKernel )

						_pSound->UpdateSounds();
					#endif
				}

				return WMSG_SUCCESS;
			}
			else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					PressOKBtn();					

				return WMSG_SUCCESS;
			}
			else if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
				{
				}
				else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
				{
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;
	}
			
	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUISelectServer::KeyMessage( MSG *pMsg )
{
	// Select server isn't focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{	
		PressOKBtn();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//-----------------------------------------------------------------------------
void CUISelectServer::PressOKBtn()
{
	// 서버군과 서버가 선택되었으면 접속 시도.
	int	nSelServerGroup = m_lbServerGroup.GetCurSel();
	int	nSelServer = m_lbServer.GetCurSel();
	if( nSelServerGroup != -1 && nSelServer != -1 )
	{
		sServerInfo &SI = m_vectorGroupInfo[nSelServerGroup].vectorServerInfo[nSelServer];
								
		// 서버에 연결을 시도합니다.
		if( SI.iPlayerNum < _cmiComm.cci_iFullUsers )
		{
				_pSound->Mute();		
			_pNetwork->m_iServerGroup = m_vectorGroupInfo[nSelServerGroup].iServerNo;
			_pNetwork->m_iServerCh = SI.iSubNum;

			ConnectToServer( SI.strAddress, SI.iPortNum );
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResetServerList()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::ResetServerList()
{
	// List box
	m_lbServerGroup.ResetAllStrings();
	m_lbServer.ResetAllStrings();

	// List info
	m_vectorGroupInfo.clear();
	m_nRecentServerGroup = -1;
	m_nRecentServer = -1;
	m_nRecomendSvr = -1;
	m_nRecomendSvrOrder = -1;

	// Button
	m_btnOK.SetEnable( FALSE );

	// Password
// EDIT : BS	_pNetwork->m_strUserPW = "";

//	m_nRecomendSvr = FindRecomendSvr();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : lServerNo - 
//			lSubNum - 
//			lPlayerNum - 
//			&strAddress - 
//			lPortNum - 
//-----------------------------------------------------------------------------
void CUISelectServer::AddToServerList(LONG lServerNo, LONG lSubNum, LONG lPlayerNum, CTString &strAddress, LONG lPortNum ,UBYTE bIsRecommend)
{
	sGroupInfo	GroupInfo;
	GroupInfo.iServerNo		= lServerNo;

	std::vector<sGroupInfo>::iterator iter = std::find_if(m_vectorGroupInfo.begin(), m_vectorGroupInfo.end(), 
		FindGroup(GroupInfo));

	if(iter == m_vectorGroupInfo.end())
	{
		m_vectorGroupInfo.push_back(GroupInfo);
		iter = m_vectorGroupInfo.end() - 1;
	}
// Date : 2006-04-21(오후 4:13:13), By eons
#if( g_iCountry == KOREA )
#undef TEMP_SERVER_LIST
#endif

#ifdef TEMP_SERVER_LIST

	extern INDEX	g_iCountry;

	// 국내
	if(g_iCountry == KOREA)
	{
		// FIXME : 임의적으로 4, 2, 1, 5, 3의 순서로 나오도록 처리함...
		// FIXME : 하드 코딩된 내용.
		switch(lServerNo)
		{
		case 4:
			(*iter).iGroupState	= 4;
			break;
		case 2:
			(*iter).iGroupState	= 3;
			break;
		case 1:
			(*iter).iGroupState	= 2;
			break;
		case 5:
			(*iter).iGroupState	= 1;
			break;
		case 3:
			(*iter).iGroupState	= 0;
			break;
		}	
	}
	// 대만
	else if(g_iCountry == TAIWAN || g_iCountry == TAIWAN2 ) //wooss 050929
	{
		// FIXME : 임의적으로 1, 2, 3, 4, 5, 6, 7, 8의 순서로 나오도록 처리함...		
		switch(lServerNo)
		{
		case 1:
		case 11:
			(*iter).iGroupState	= 7;
			break;
		case 2:
		case 12:
			(*iter).iGroupState	= 6;
			break;
		case 3:
		case 13:
			(*iter).iGroupState	= 5;
			break;
		case 4:
		case 14:
			(*iter).iGroupState	= 4;
			break;
		case 5:
		case 15:
			(*iter).iGroupState	= 3;
			break;
		case 6:
		case 16:
			(*iter).iGroupState	= 2;
			break;
		case 7:
		case 17:
			(*iter).iGroupState	= 1;
			break;
		case 8:
		case 18:
			(*iter).iGroupState	= 0;
			break;
		}
		
	}
#endif

	sServerInfo	ServerInfo;
	ServerInfo.iPlayerNum	= lPlayerNum;
	ServerInfo.iSubNum		= lSubNum;
	ServerInfo.strAddress	= strAddress;
	ServerInfo.iPortNum		= lPortNum;

#ifndef TEMP_SERVER_LIST
	(*iter).iGroupState		-= lPlayerNum;
	/*
	if( lPlayerNum == -1 )								// 점검
	{
		(*iter).iGroupState	+= 0;
	}
	else if( lPlayerNum >= _cmiComm.cci_iFullUsers )	// FULL
	{
		(*iter).iGroupState	+= 1;
	}
	else if( lPlayerNum >= _cmiComm.cci_iBusyUsers )	// BUSY
	{
		(*iter).iGroupState	+= 2;
	}
	else												// 원할
	{
		(*iter).iGroupState	+= 3;
	}
	*/
#endif
	if(bIsRecommend) {
		m_nRecomendSvr = lServerNo;
	}
	(*iter).vectorServerInfo.push_back(ServerInfo);
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Close()
{
	// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
	_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);

	ResetServerList();

	_pUIMgr->SetUIGameState( UGS_LOGIN );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : strIP - 
//			ulPortNum - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
void CUISelectServer::ConnectToServer(CTString strIP, ULONG ulPortNum)
{
	if(_pNetwork->m_bSendMessage)
		return;

	// 소켓의 연결을 끊었다가, 다시 연결함.
	_cmiComm.Reconnect(strIP, ulPortNum);
	if(_tcpip.Socket == INVALID_SOCKET)
	{
		//CPrintF("게임 서버와 연결할 수 없습니다.\n");

		_pUIMgr->CloseMessageBox(MSGCMD_CONNECT_ERROR);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 424, "접속 오류" ), UMBS_OK, UI_SEL_SERVER, MSGCMD_CONNECT_ERROR );
		MsgBoxInfo.AddString( _S( 426, "게임 서버와 연결할 수 없습니다." ) );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		_pNetwork->m_bSendMessage = FALSE;
		return;
	}

	_pNetwork->SendLoginMessage(_pNetwork->m_strUserID, _pNetwork->m_strUserPW, _pUIMgr->GetVersion());
	_pUIMgr->Lock(TRUE);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUISelectServer::Reset()
{
	Lock(FALSE);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iRecentGroup - 
//			iRecentServer - 
//-----------------------------------------------------------------------------
void CUISelectServer::SetRecentServer( int iRecentGroup, int iRecentServer )
{
	// Sort server list - 추후 정렬 방식이 추가될 지도 모름..
	std::sort(m_vectorGroupInfo.begin(), m_vectorGroupInfo.end());

	for( int iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
		std::sort( m_vectorGroupInfo[iGroup].vectorServerInfo.begin(), m_vectorGroupInfo[iGroup].vectorServerInfo.end());		

	m_nRecentServerGroup	= iRecentGroup;
	m_nRecentServer			= iRecentServer;

	int	nMaxLine = m_vectorGroupInfo.size();
	for( iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
	{
		if( nMaxLine < m_vectorGroupInfo[iGroup].vectorServerInfo.size() )
			nMaxLine = m_vectorGroupInfo[iGroup].vectorServerInfo.size();
	}
	if( nMaxLine > 20 )
		nMaxLine = 20;

	int	nListBoxLines = m_lbServerGroup.GetLinePerPage();
	if( nMaxLine > 10 )
	{
		if( nListBoxLines < nMaxLine )
		{
			int	nDiffLine = nMaxLine - nListBoxLines;
			int	nDiffHeight = nDiffLine * m_lbServerGroup.GetLineHeight();
			m_lbServerGroup.ChangeSize( nDiffLine );
			m_lbServer.ChangeSize( nDiffLine );
			SetHeight( GetHeight() + nDiffHeight );
			m_btnOK.Move( 0, nDiffHeight );
			m_btnCancel.Move( 0, nDiffHeight );

			const CDrawPort	*pdp = _pdpMain;
			ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );
		}
	}
	else
	{
		if( nListBoxLines > 10 )
		{
			int	nDiffLine = 10 - nListBoxLines;
			int	nDiffHeight = nDiffLine * m_lbServerGroup.GetLineHeight();
			m_lbServerGroup.ChangeSize( nDiffLine );
			m_lbServer.ChangeSize( nDiffLine );
			SetHeight( GetHeight() + nDiffHeight );
			m_btnOK.Move( 0, nDiffHeight );
			m_btnCancel.Move( 0, nDiffHeight );

			const CDrawPort	*pdp = _pdpMain;
			ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );
		}
	}

	CTString strRecomend = "";

	for( iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
	{
		int	iGroupNo = m_vectorGroupInfo[iGroup].iServerNo;

		// 테스트 서버면....
		extern UINT g_uiEngineVersion;
		if(g_uiEngineVersion >= 10000 ){
			if( iGroupNo == m_nRecomendSvr) 
				m_nRecomendSvrOrder = iGroupNo-1000;
		}

		if ( iGroupNo == 1001 )
		{
			iGroupNo -= 990;
		}
		
		// 추천 서버 때문에.
		else if (iGroupNo == m_nRecomendSvr) 
			m_nRecomendSvrOrder = iGroup +1;
		strRecomend = GetServerGroupName(iGroupNo);



		if( iGroupNo == iRecentGroup )
			m_lbServerGroup.AddString( 0, strRecomend, 0x7E8FEBFF );
		else
			m_lbServerGroup.AddString( 0, strRecomend, 0xC0C0C0FF );
	}

	m_lbServerGroup.SetEnable( TRUE );

	int iGroupCnt	= 0;
	int iServerCnt	= 0;
	iRecentGroup	= -1;
	iRecentServer	= -1;
	std::vector<sGroupInfo>::const_iterator end = m_vectorGroupInfo.end();
	for(std::vector<sGroupInfo>::iterator iter = m_vectorGroupInfo.begin(); 
	iter != end; ++iter, ++iGroupCnt)
	{
		if((*iter).iServerNo == m_nRecentServerGroup)
		{
			iRecentGroup		= iGroupCnt;			
			std::vector<sServerInfo>::const_iterator itend = (*iter).vectorServerInfo.end();
			for(std::vector<sServerInfo>::iterator it = (*iter).vectorServerInfo.begin();
			it != itend; ++it, ++iServerCnt)
			{
				if((*it).iSubNum == m_nRecentServer)
				{					
					iRecentServer	= iServerCnt;
				}
			}
			break;
		}
	}

	if( iRecentGroup != -1 )
	{
		m_lbServerGroup.SetCurSel( iRecentGroup );

		if( iRecentServer != -1 )
		{
			SetServerList();
			m_lbServer.SetCurSel( iRecentServer );
			m_btnOK.SetEnable( TRUE );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetServerList()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::SetServerList()
{
	m_btnOK.SetEnable( FALSE );

	if( m_lbServerGroup.GetCurSel() < 0 )
		return;

	m_lbServer.ResetAllStrings();

	int	iCurSelGroup = m_lbServerGroup.GetCurSel();
	int	iGroupNo = m_vectorGroupInfo[iCurSelGroup].iServerNo;
	for( int iServer = 0; iServer < m_vectorGroupInfo[iCurSelGroup].vectorServerInfo.size(); iServer++ )
	{
		sGroupInfo sGroup = m_vectorGroupInfo[iCurSelGroup];
		sServerInfo	sInfo = m_vectorGroupInfo[iCurSelGroup].vectorServerInfo[iServer];
		int	iServerNo = sInfo.iSubNum;

		CTString	strServerName;

		if ( iGroupNo == 1001 )
		{
			iGroupNo = 11;
		}
		
		if ( g_iCountry == USA )
		{
			CTString strServer;

			switch( iServerNo )
			{ // USA 5, 6 Non-Pvp Server
			case 3:
			case 4:
				{
					if (sGroup.iServerNo == 2 || sGroup.iServerNo == 3 || sGroup.iServerNo == 4 || sGroup.iServerNo == 5)
					{
						strServer = CTString("(Non-PvP)");
					}
				}
				break;
			case 5:
			case 6:
				{
					if (sGroup.iServerNo == 1)
					{
						strServer = CTString("(Non-PvP)");
					}
					else if (sGroup.iServerNo == 2 && iServerNo == 6)
					{
						strServer = CTString("(Roleplaying)");
					}
				}
				break;
			default:
				strServer = CTString("");
			}

			strServerName.PrintF("%s-%d %s ", GetServerGroupName(iGroupNo), iServerNo, strServer);
		}
		else if ( g_iCountry == BRAZIL )
		{
			CTString strServer;
			switch(iServerNo)
			{
			case 2:
				{
					strServer = CTString("(Serv-PVP)");
				}
				break;
			case 7: case 8:
				{
					if (sGroup.iServerNo == 1)
					{
						strServer = CTString("(Non-PvP)");
					}
				}
				break;
			default:
				strServer = CTString("");
			}			
			strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strServer);
		}
		else if ( g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
		{//공성채널 4번 
			CTString strServer;
			switch(iServerNo)
			{
			case 2:
			case 5:
				{
					strServer = CTString("(Non-PvP)");
				}break;
			case 6:
				{
					if (
						((g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) && iGroupNo == 2) ||
						(g_iCountry == GERMANY && iGroupNo == 6))
					{
						strServer = CTString("(RPG)");
					}
				}break;
			default:
				strServer = CTString("");
			}			
			
			strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strServer);
		}
		else
		{
			strServerName.PrintF( "%s-%d ", GetServerGroupName( iGroupNo ), iServerNo );
		}
		
		if( sGroup.iServerNo == m_nRecentServerGroup && iServerNo == m_nRecentServer )
			m_lbServer.AddString( 0, strServerName, 0x7E8FEBFF );
		else
			m_lbServer.AddString( 0, strServerName, 0xC0C0C0FF );

		if( sInfo.iPlayerNum == -1 )
		{
			m_lbServer.AddString( 1, _S( 372, "점검" ), 0x9400D6FF );
		}
		else if( sInfo.iPlayerNum >= _cmiComm.cci_iFullUsers )
		{
			m_lbServer.AddString( 1, _S( 373, "FULL" ), 0xD11184FF );
		}
		else if( sInfo.iPlayerNum >= _cmiComm.cci_iBusyUsers )
		{
			m_lbServer.AddString( 1, _S( 374, "혼잡" ), 0xFF9533FF );
		}
		else
		{
			m_lbServer.AddString( 1, _S( 371, "원활" ), 0xCCCCCCFF );
		}
	}

	m_lbServer.SetEnable( TRUE );
}

// ----------------------------------------------------------------------------
// Name : Lock()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Lock(BOOL bLock)
{
	m_btnOK.SetEnable(!bLock);
	m_lbServerGroup.SetEnable(!bLock);
	m_lbServer.SetEnable(!bLock);
}

//------------------------------------------------------------------------------
// CUISelectServer::GetServerGroupName
// Explain:  Group 번호를 바탕으로 서버군 이름을 리턴한다
// Date : 2005-05-02,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISelectServer::GetServerGroupName( int iGroupNo )
{
	iGroupNo--;

	extern INDEX	g_iCountry;

	if ( g_iCountry != CHINA )  // 중국 Local이 아니면 무조건 기존 루틴~
		return m_astrServerName[iGroupNo];

	if( !_cmiComm.Is2rdLocal() ) // 2번째 지역이 아니거나
	{
		return m_astrServerName[iGroupNo];
	}
	
	return m_astrServerName[iGroupNo + MAX_SERVER_NAME / 2 ];
	
	
}

//------------------------------------------------------------------------------
// CUISelectServer::FindRecomendSvr
// Explain:  게임루트에서 rsvr.txt 화일을 찾고, 해당 내용을 읽어와서 리턴한다.
// 추천 서버 이벤트를 위한 임시 처리..
// Date : 2005-07-06 ,Author: Seo
//------------------------------------------------------------------------------
int CUISelectServer::FindRecomendSvr()
{
	int iRSvr = -1;

	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fnTemp		= strFullPath + "rsvr.txt";
	
	FILE *fp;

	fp = fopen(fnTemp, "rt");
	if (fp) {
		fscanf(fp, "%d", &iRSvr);

		if (iRSvr < -1 || (iRSvr > 4 && iRSvr != 11))
			iRSvr = -1;
		return iRSvr;
	}

	return iRSvr;
}