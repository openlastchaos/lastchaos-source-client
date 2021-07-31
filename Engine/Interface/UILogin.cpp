#include "stdh.h"
#include <Engine/Interface/UiLogin.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Rendering/Render.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Sound/SoundLibrary.h>		// 서정원 로긴 관련 사운드 처리

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
	#include <NPGameLib.h>
#endif
// ---------------------------------------------------<<


// External variables
extern HWND	_hwndMain;
extern INDEX login_bSkipSelect;
extern INDEX g_iCountry;

#define MAXLENGTH			(15)		// Max Length of ID & PW

// ----------------------------------------------------------------------------
// Name : CUILogin()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUILogin::CUILogin()
//: m_bConnectToLoginServer(FALSE)
{
	m_ptdClassification = NULL;

	m_bSecurity =FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUILogin()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUILogin::~CUILogin()
{
	
	if( m_ptdClassification )
	{
		_pTextureStock->Release( m_ptdClassification );
		m_ptdClassification = NULL;
	}
	

	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUILogin::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Create inventory texture
	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	{
		m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Login_ger.tex" ) );
	}
	else
	{
		m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Login.tex" ) );
	}

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	int ntmpX = 183;

	// UV Coordinate of each part
	// Background
	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 189; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	
	m_rtLogin.SetUV( 0, 0, ntmpX, 116, fTexWidth, fTexHeight );

	m_rtSecurityTop.SetUV( 0, 0, ntmpX, 36, fTexWidth, fTexHeight );
	m_rtSecurityMid.SetUV( 0, 23, ntmpX, 32, fTexWidth, fTexHeight );
	m_rtSecurityBottom.SetUV( 0, 54, ntmpX, 116, fTexWidth, fTexHeight );

	ntmpX = 39;
	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 50; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124

	// OK button (connect)
	m_btnOK.Create( this, _S( 180, "접속" ), ntmpX, 88, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 193, 4, 256, 25, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 193, 26, 256, 47, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	ntmpX = 105;

	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 116; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124

	// Cancel button (exit)
	m_btnCancel.Create( this, _S( 181, "나가기" ), ntmpX, 88, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 193, 4, 256, 25, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 193, 26, 256, 47, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// ID edit box
	ntmpX = 75;

	if ( g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 84; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124

	m_ebID.Create( this, ntmpX, 37, 91, 15, MAXLENGTH );
	m_ebID.SetFocus( TRUE );
	m_ebID.SetReadingWindowUV( 239, 49, 256, 65, fTexWidth, fTexHeight );
	m_ebID.SetCandidateUV( 239, 49, 256, 65, fTexWidth, fTexHeight );

	// Password edit box
	m_ebPW.Create( this, ntmpX, 56, 91, 15, MAXLENGTH, TRUE );
	m_ebPW.SetFocus( FALSE );

	m_ebSecurity.Create( this, ntmpX, 56, 91, 15, 4, TRUE );
	m_ebSecurity.SetFocus( FALSE );

	// [090715: selo] - check button ( 좌표와 체크버튼 텍스쳐 세팅)
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	fTexWidth	= m_ptdButtonTexture->GetPixWidth();
	fTexHeight	= m_ptdButtonTexture->GetPixHeight();

	extern INDEX g_iSaveID;

	int nStrWidth = ( _S( 290, "저장" ).Length() + 3 ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );	
	m_btnCheck.Create(this, 10, 74, 13, 13, _S( 290, "저장"), FALSE, 15);
	m_btnCheck.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_btnCheck.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_btnCheck.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_btnCheck.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_btnCheck.SetTextColor( TRUE, 0xF2F2F2FF );
	m_btnCheck.SetTextColor( FALSE, 0xF2F2F2FF );
	m_btnCheck.SetCheck( g_iSaveID );

	m_ptdClassification = CreateTexture ( CTString ( "Data\\Interface\\Loading\\Classification_15.tex" ) );
	//1228
	
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fnTemp		= strFullPath + "vtm.brn";

	extern int g_iLocalVersion;
	//int LocalVersion;
	int encr_version;
	FILE *fp;
	fp = fopen(fnTemp, "rb");
	if (fp) {
		fread(&encr_version, sizeof(int), 1, fp);
		g_iLocalVersion = (encr_version - 27) / 3;		

		fclose(fp);
	} else {
		g_iLocalVersion = 700;
	}

	_pUIMgr->SetVersion(g_iLocalVersion);

	// [090715: selo] 저장된 아이디 읽어오기
#ifdef LOGIN_SAVEID
	extern INDEX g_iSaveID;

	if(g_iSaveID)		
		ReadPreviousId();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Reset
//-----------------------------------------------------------------------------
void CUILogin::Reset()
{
	m_strUserID		= "";
	m_strUserPW		= "";

	// NOTE : 패스워드 입력 부분에 포커스가 활성화되는 것 때문에 주석처리함...
	//Lock(FALSE);
	m_ebID.SetEnable(TRUE);
	m_ebPW.SetEnable(TRUE);
	m_btnOK.SetEnable(TRUE);

	// [090715: selo] 아이디 저장 체크박스 활성/비활성화
#ifdef LOGIN_SAVEID
	m_btnCheck.SetEnable(TRUE);
#else
	m_btnCheck.SetEnable(FALSE);
#endif
	
	m_bSecurity =FALSE;
	m_ebSecurity.SetEnable(TRUE);
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUILogin::Render()
{
	// Set login texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	if( m_bSecurity )
		SecurityRender();
	else
	{
		// Add render regions
		// Background
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtLogin.U0, m_rtLogin.V0, m_rtLogin.U1, m_rtLogin.V1,
											0xFFFFFFFF );

		// OK button (connect)
		m_btnOK.Render();
		
		// Cancel button (exit)
		m_btnCancel.Render();

		// Edit box
		m_ebID.Render();
		m_ebPW.Render();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// [090715: selo] - check button
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );
		
		if( m_btnCheck.IsEnabled() )
			m_btnCheck.Render();

		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		
		// Prepare Rendering Text
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 182, "로그인" ),
							m_nPosX + LOGIN_TITLE_TEXT_OFFSETX, m_nPosY + LOGIN_TITLE_TEXT_OFFSETY );

		int ntmpX = 9;
		if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 6; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124
		
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 183, "아이디" ), m_nPosX + ntmpX, m_nPosY + 38 );
			
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 184, "패스워드" ), m_nPosX + ntmpX, m_nPosY + 57 );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();

		// Reading window
		if( m_ebID.DoesShowReadingWindow() )
		{
			// Set login texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebID.RenderReadingWindow();

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
	}

	if(g_iCountry != BRAZIL && g_iCountry != GERMANY && g_iCountry != SPAIN && g_iCountry != FRANCE && g_iCountry != POLAND)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	{
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdClassification );

		int nWidth = m_ptdClassification->GetPixWidth();
		int nHeigth = m_ptdClassification->GetPixHeight();
		
		int nPosX = _pUIMgr->GetDrawPort()->GetWidth() - ( nWidth + nWidth / 4 ) ;
		int nPosY = ( nHeigth / 4 );

		_pUIMgr->GetDrawPort()->AddTexture( nPosX, nPosY,
											nPosX + nWidth, nPosY + nHeigth, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}

}

//-----------------------------------------------------------------------------
// Purpose: 
//
//-----------------------------------------------------------------------------
void CUILogin::PressOKBtn()
{
	// Get input string
	if( m_ebID.GetString() )
		m_strUserID = m_ebID.GetString();
		
	if( m_ebPW.GetString() )
		m_strUserPW = m_ebPW.GetString();

	_pSound->Mute();
	BOOL bLogin = FALSE;

	if( m_bSecurity )
		ConnectToSecurityCard();
	else
		bLogin = TryToLogin();
	
#ifdef RESTRICT_SOUND
	extern BOOL _bNTKernel;
	//if( _bNTKernel )
	_pSound->UpdateSounds();
#endif

	m_ebPW.ResetString();

	m_ebSecurity.ResetString();


	// [090715: selo] 아이디 저장
#ifdef LOGIN_SAVEID
	extern INDEX g_iSaveID;

	if(g_iSaveID && bLogin)		
		WritePreviousId();	
#endif

	// 접속에 성공했다면 서버 선택화면으로...
	//if( bUserLogin )
	//	_pUIMgr->SetUIGameState( UGS_SELSERVER );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUILogin::KeyMessage( MSG *pMsg )
{
	// If login is not focused
	if( !IsFocused() )
		return WMSG_FAIL;
	
	if ( !m_btnOK.IsEnabled() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{	
		PressOKBtn();
		return WMSG_SUCCESS;
	}

	if( pMsg->wParam == VK_TAB )
	{
		if( m_ebID.IsFocused() )
		{
			m_ebID.SetFocus( FALSE );
			m_ebPW.SetFocus( TRUE );
		}
		else if( m_ebPW.IsFocused() )
		{
			m_ebPW.SetFocus( FALSE );
			m_ebID.SetFocus( TRUE );
		}

		return WMSG_SUCCESS;
	}

	if( m_bSecurity )
	{
		if( m_ebSecurity.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}
	else
	{
		if( m_ebID.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
		else if( m_ebPW.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUILogin::IMEMessage( MSG *pMsg )
{

	// 이기환 수정 시작 (11. 15) : return 값 조절을 위해 로직 변경
	WMSG_RESULT wmsgResult;

	if( m_bSecurity )
	{
		if( ( wmsgResult = m_ebSecurity.IMEMessage( pMsg ) ) != WMSG_FAIL)
		{
			return wmsgResult;
		}
	}
	else
	{
		if( ( wmsgResult = m_ebID.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
		else if( ( wmsgResult = m_ebPW.IMEMessage( pMsg ) ) != WMSG_FAIL)
		{
			return wmsgResult;
		}
	}

	// 이기환 수정 끝
	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUILogin::CharMessage( MSG *pMsg )
{
	if( m_bSecurity )
	{
		if( m_ebSecurity.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}
	else
	{
		if( m_ebID.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
		else if( m_ebPW.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}
	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUILogin::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	extern INDEX g_iSaveID;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	if( pMsg->message == WM_MOUSEMOVE )
	{
		// OK button
		if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		// Cancel button
		else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;

		// [090715: selo] 
		else if( m_btnCheck.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
	}
	else if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( IsInside( nX, nY ) )
		{
			if( m_bSecurity )
			{
				//보안카드 패스워드 입력 박스
				if( m_ebSecurity.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebSecurity.SetFocus( TRUE );
				}
			}
			else
			{
				// ID input box
				if( m_ebID.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebPW.SetFocus( FALSE );
					m_ebID.SetFocus( TRUE );
				}
				// PW input box
				else if( m_ebPW.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebID.SetFocus( FALSE );
					m_ebPW.SetFocus( TRUE );
				}
			}
			
			// OK Button
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing
			}
			// Cancel Button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing
			}

			// [090715: selo] 아이디 저장 체크박스 처리
			else if ( ( wmsgResult = m_btnCheck.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if ( wmsgResult == WMSG_SUCCESS )
				{
					m_bChecked = m_btnCheck.IsChecked() ? 1 : 0;
					g_iSaveID = m_bChecked;

					extern CTString g_strSaveID;
					if( false == m_bChecked )
						g_strSaveID = "";
				}
			}

			_pUIMgr->RearrangeOrder( UI_LOGIN, TRUE );
			return WMSG_SUCCESS;
		}
	}
	else if( pMsg->message == WM_LBUTTONUP )
	{
		if( IsInside( nX, nY ) )
		{
			// If login isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Connect & login button
			if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					PressOKBtn();
			}
			// Exit & back button
			else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					if( m_bSecurity )
					{
						m_bSecurity =FALSE;
						m_ebSecurity.ResetString();
					}
					else
					{
						_pGameState->Running()		= FALSE;
						_pGameState->QuitScreen()	= FALSE;
					}
				}
			}

			return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CUILogin::TryToLogin()
{
	CTString strMessage;

	if( m_strUserID.Length() <= 0 )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
									UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 185, "아이디를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo, m_nLoginMsgPosX, m_nLoginMsgPosY );

		return FALSE;
	}
	else if( m_strUserPW.Length() <= 0 )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
									UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 186, "패스워드를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo, m_nLoginMsgPosX, m_nLoginMsgPosY );

		return FALSE;
	}

	CGame* _pGame		= _pUIMgr->GetGame();
	//CTFileName fnWorld	= _pGame->gam_strCustomLevel;
	
	//extern BOOL _bUseSocket;
	//_bUseSocket = TRUE;
	
	_pGameState->ClearCharacterSlot();
	
	// NOTE : 여기서 소켓을 생성하고 있음.
	if(_pGame->PreNewGame())
	{
		//gm_bUseSocket = TRUE;
		// 로그인 서버에 로그인을 시도합니다.
// wooss 060209 TEST JAPAN
//#define AUTO_LOGIN
//#ifndef AUTO_LOGIN
		extern BOOL g_bAutoLogin;
		if(!g_bAutoLogin)
			ConnectToLoginServer();
//#endif
	}
	//_bUseSocket = FALSE;

	//return m_bConnectToLoginServer;
	
	// [090715: selo] 성공시 TRUE 리턴 
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUILogin::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );

	m_nLoginMsgPosX = ( ( pixMaxI + pixMinI - MSGBOX_WIDTH ) / 2 );
	m_nLoginMsgPosY = m_nPosY + m_nHeight + 5;
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUILogin::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ReadPreviousId()
// Desc : 저장된 아이디를 읽어 온다
// ----------------------------------------------------------------------------
void CUILogin::ReadPreviousId()
{	
	extern CTString g_strSaveID;

	m_ebID.SetString(g_strSaveID.str_String);
	m_ebID.SetFocus(FALSE);
	m_ebPW.SetFocus(TRUE);
}

// ----------------------------------------------------------------------------
// Name : WritePreviousId()
// Desc : 아이디를 저장한다
// ----------------------------------------------------------------------------
void CUILogin::WritePreviousId()
{
	extern CTString g_strSaveID;

	g_strSaveID = m_ebID.GetString();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CUILogin::ConnectToLoginServer()
{
	if(_pNetwork->m_bSendMessage)
		return FALSE;

	// 문제가 발생했습니다.
/*	if( _tcpip.Socket == INVALID_SOCKET || !_tcpip.IsConnected(_tcpip.Socket) )
	{
		CPrintF("로그인 서버와 연결할수 없습니다.\n");

		_pUIMgr->CloseMessageBox(MSGCMD_CONNECT_ERROR);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 424, "접속 오류" ), UMBS_OK, UI_LOGIN, MSGCMD_CONNECT_ERROR );
		MsgBoxInfo.AddString( _S( 425, "로그인 서버와 연결할 수 없습니다." ) );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		_pNetwork->m_bSendMessage = FALSE;
		return FALSE;
	}*/
	// 소켓의 연결을 끊었다가, 다시 연결함.	
	//-------------------------------------------------
	_pUIMgr->Lock(TRUE);
	_pNetwork->SendLoginMessage(m_strUserID, m_strUserPW, _pUIMgr->GetVersion());		
	_pNetwork->m_strUserID = m_strUserID;
	_pNetwork->m_strUserPW = m_strUserPW;


	// WSS_NPROTECT 070402 ------------------------------->>
	// 사용자 로그인시 ID를 GameMon에 통보
#ifndef NO_GAMEGUARD
	extern ENGINE_API CNPGameLib npgl;
//	CPrintF("[ ---->> GameGuard Send(gameID)...Start <<---- ] - %lu\n",timeGetTime());
	npgl.Send(m_strUserID.str_String);
//	CPrintF("[ ---->> GameGuard Send(gameID)...End <<---- ] - %lu\n",timeGetTime());
#endif
	// ---------------------------------------------------<<

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : Lock()
// Desc :
// ----------------------------------------------------------------------------
void CUILogin::Lock(BOOL bLock)
{
	m_ebPW.SetFocus(!bLock);
	m_ebID.SetEnable(!bLock);
	m_ebPW.SetEnable(!bLock);
	m_btnOK.SetEnable(!bLock);	

	m_ebSecurity.SetEnable(!bLock);
}

void CUILogin::SecurityCardSet(UBYTE* SecurityNum)
{
	if( SecurityNum == NULL )
	{
		m_bSecurity =FALSE;
	}
	else
	{
		m_bSecurity =TRUE;

		m_strSecurityCode.PrintF( "%s: [%c,%d][%c,%d][%c,%d][%c,%d]", 
											_S(3668, "보안 코드"), 
											'A'+SecurityNum[0]/8, SecurityNum[0]%8+1, 
											'A'+SecurityNum[1]/8, SecurityNum[1]%8+1, 
											'A'+SecurityNum[2]/8, SecurityNum[2]%8+1, 
											'A'+SecurityNum[3]/8, SecurityNum[3]%8+1);
	}
}

BOOL CUILogin::ConnectToSecurityCard()
{
	if( strlen(m_ebSecurity.GetString()) <= 0 )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 3667, "보안 카드" ), UMBS_OK,
									UI_LOGIN, MSGCMD_LOGIN_ERROR );
		MsgBoxInfo.AddString( _S( 186, "패스워드를 입력해주십시오." ) );
		_pUIMgr->CreateMessageBox( MsgBoxInfo, m_nLoginMsgPosX, m_nLoginMsgPosY );

		return FALSE;
	}

	if(_pNetwork->m_bSendMessage)
		return FALSE;

	_pUIMgr->Lock(TRUE);
	_pNetwork->SendSecurityMessage( CTString(m_ebSecurity.GetString()) );

	return TRUE;
}

void CUILogin::SecurityRender()
{
	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY,
										m_nPosX + m_nWidth, m_nPosY + 36,
										m_rtSecurityTop.U0, m_rtSecurityTop.V0, m_rtSecurityTop.U1, m_rtSecurityTop.V1,
										0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY +36,
										m_nPosX + m_nWidth, m_nPosY + 36 +9,
										m_rtSecurityMid.U0, m_rtSecurityMid.V0, m_rtSecurityMid.U1, m_rtSecurityMid.V1,
										0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY +36 +9,
										m_nPosX + m_nWidth, m_nPosY + 36 +9 +9,
										m_rtSecurityMid.U0, m_rtSecurityMid.V0, m_rtSecurityMid.U1, m_rtSecurityMid.V1,
										0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY +36 +9 +9,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtSecurityBottom.U0, m_rtSecurityBottom.V0, m_rtSecurityBottom.U1, m_rtSecurityBottom.V1,
										0xFFFFFFFF );

	// OK button (connect)
	m_btnOK.Render();
	
	// Cancel button (exit)
	m_btnCancel.Render();

	m_ebSecurity.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	// Prepare Rendering Text
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 3667, "보안 카드" ),
						m_nPosX + LOGIN_TITLE_TEXT_OFFSETX, m_nPosY + LOGIN_TITLE_TEXT_OFFSETY );

	int ntmpX = 9;
	if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND) { ntmpX = 6; }//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	
	_pUIMgr->GetDrawPort()->PutTextEx( m_strSecurityCode, m_nPosX + ntmpX, m_nPosY + 39 );
		
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 184, "패스워드" ), m_nPosX + ntmpX, m_nPosY + 58 );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}