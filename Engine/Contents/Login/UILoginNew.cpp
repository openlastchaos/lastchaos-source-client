#include "StdH.h"
#if		!defined(WORLD_EDITOR)
#include <Engine/Network/NetClient.h>
#endif	// WORLD_EDITOR
#include <Engine/Interface/UIInternalClasses.h>
#include "UILoginNew.h"
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/Sound/SoundLibrary.h>		// 서정원 로긴 관련 사운드 처리
#include <Engine/Base/md5.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/Login/LoginNewCmd.h>
#include <Engine/LoginJobInfo.h>
#include "BackImageManager.h"
#include <Common/Packet/ptype_authentication.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/GameStageManager/StageMgr.h>

#define DEF_HOLLOWEEN_RATIO (0.7f)
#define DEF_XMAS_RATIO		(0.7f)

#define DEF_ALPHA_LV_HOLLOWEEN_MAX (1.0f)
#define DEF_ALPHA_LV_HOLLOWEEN_MIN (0.3f)

#define DEF_ALPHA_LV_XMAS_MAX (1.0f)
#define DEF_ALPHA_LV_XMAS_MIN (0.0f)

enum eXMAS_EFFECT_STEP
{
	eEFFECT_STEP_READY = 0, // 준비
	eEFFECT_STEP_FINISH,	// 모든 스탭을 마침
};

#ifdef EUROUPEAN_SERVER_LOGIN
extern INDEX g_iConnectEuroupean;
#endif	//	EUROUPEAN_SERVER_LOGIN
#define	DEF_BASIC_SERVER 0
#define	DEF_EUROUPEAN_SERVER 1

#ifndef NO_GAMEGUARD
#include "Engine/GameGuardInterface.h"    
#endif

CUILoginNew::CUILoginNew(void)
	: m_pLogoFrame(NULL)
	, m_pBtnLogin(NULL)
	, m_pBtnClose(NULL)
	, m_pBtnSave(NULL)
	, m_pCbSelServer(NULL)
	, m_pEbId(NULL)
	, m_pEbPw(NULL)
	, m_pImgLogo(NULL)
	, m_pImgLogoHal(NULL)
	, m_pImgHalEye(NULL)
	, m_pImgLogoXmas(NULL)
	, m_pImgXmasEffect1(NULL)
	, m_pImgXmasEffect2(NULL)
	, m_nLogoType(eLOGO_NORMAL)
{
	setInherit(false);
}

CUILoginNew::~CUILoginNew(void)
{
	Destroy();
}

void CUILoginNew::PressLoginBtn()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	pLoginData->SetUserId(m_pEbId->GetString());
	pLoginData->SetPassword(m_pEbPw->GetString());

	_pSound->Mute();

	BOOL bLogin = FALSE;

#ifdef EUROUPEAN_SERVER_LOGIN
	if (m_pCbSelServer != NULL)
	{
		g_iConnectEuroupean = m_pCbSelServer->GetCurSel();
		CUIFocus::getSingleton()->setUI(NULL);
	}
#endif	//	EUROUPEAN_SERVER_LOGIN

	bLogin = TryToLogin();

#ifdef RESTRICT_SOUND
	extern BOOL _bNTKernel;
	//if( _bNTKernel )
	_pSound->UpdateSounds();
#endif

	m_pEbPw->ResetString();

	if(pLoginData->GetSaveID() && bLogin)		
		WritePreviousId();
#if		!defined(WORLD_EDITOR)
	if (bLogin == TRUE)
	{
		boost::thread t(boost::bind(&CUILoginNew::sendIP, this));
	}
#endif	// WORLD_EDITOR
}

void CUILoginNew::PressCloseBtn()
{
	CLoginJobInfo::getSingleton()->LoginModelDelete();

	_pGameState->Running()		= FALSE;
	_pGameState->QuitScreen()	= FALSE;
}

void CUILoginNew::PressCheckSave()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	if (m_pBtnSave != NULL)
	{
		BOOL bChecked =	m_pBtnSave->IsChecked() ? TRUE : FALSE;
		pLoginData->SetSaveID(bChecked);

		extern CTString g_strSaveID;
		if( bChecked == FALSE )
			g_strSaveID = "";
	}
}

BOOL CUILoginNew::TryToLogin()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return FALSE;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMessage;

	if( pLoginData->GetUserId().Length() <= 0 )
	{
		pUIManager->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
			UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 185, "아이디를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox(MsgBoxInfo);

		return FALSE;
	}
	else if( pLoginData->GetPassword().Length() <= 0 )
	{
		pUIManager->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
			UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 186, "패스워드를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox(MsgBoxInfo);

		return FALSE;
	}

	CGame* _pGame = pUIManager->GetGame();
	
	// NOTE : 여기서 소켓을 생성하고 있음.
	if(_pGame->PreNewGame())
	{
		return pLoginData->ConnectToLoginServer();
	}

	return TRUE;
}

void CUILoginNew::OpenUI()
{
	if (IsVisible() == TRUE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);
	// Logo Update
	updateLogo();
	SetLogin();
	CUIManager::getSingleton()->RearrangeOrder(UI_LOGIN, TRUE);

	CUIManager* pUIManager = CUIManager::getSingleton();

	CBackImageManager* pBGMgr = CBackImageManager::getSingleton();
	if (pBGMgr->IsInit() == false)
		pBGMgr->InitData();

	pBGMgr->SetRenderTex(CBackImageManager::eTEX_LOGIN_BACK);
}

void CUILoginNew::CloseUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	Reset();

	CUIManager::getSingleton()->RearrangeOrder(UI_LOGIN, FALSE);
}

void CUILoginNew::SetLogin()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	if (m_pLogoFrame != NULL && m_nLogoType == eLOGO_NORMAL)
		m_pLogoFrame->PlayAni();

	if (m_pBtnSave != NULL)
		m_pBtnSave->SetCheck( pLoginData->GetSaveID() );

	if (pLoginData->GetSaveID() != FALSE)
	{
		ReadPreviousId();
	}
	else
	{
		m_pEbId->SetFocus(TRUE);
	}

	if (m_pEbPw != NULL)
	{
		m_pEbPw->SetPassWord(TRUE);
	}
	
	if (m_pCbSelServer != NULL)
	{
		m_pCbSelServer->SetCurSel(DEF_BASIC_SERVER);
		m_pCbSelServer->SetEnable(FALSE);
#ifdef EUROUPEAN_SERVER_LOGIN
		m_pCbSelServer->SetEnable(TRUE);
		if (g_iConnectEuroupean)
		{
			m_pCbSelServer->SetCurSel(DEF_EUROUPEAN_SERVER);
		}
#endif	//	EUROUPEAN_SERVER_LOGIN		
	}


#ifdef	DEV_LOGIN
	ReadTextDevPreviousIdPw();
#endif	// DEV_LOGIN
}

void CUILoginNew::Reset()
{
	if (m_pEbPw != NULL)
		m_pEbPw->ResetString();

	ResetBtn();

	if (m_pLogoFrame != NULL)
		m_pLogoFrame->StopAni();
}

void CUILoginNew::ResetBtn()
{
	if (m_pEbId != NULL)
		m_pEbId->SetEnable(TRUE);

	if (m_pEbPw != NULL)
		m_pEbPw->SetEnable(TRUE);
	
	if (m_pBtnLogin != NULL)
		m_pBtnLogin->SetEnable(TRUE);
}

void CUILoginNew::Lock(BOOL bLock)
{
	if (m_pBtnLogin == NULL || m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbPw->SetFocus(!bLock);
	m_pEbId->SetEnable(!bLock);
	m_pEbPw->SetEnable(!bLock);
	m_pBtnLogin->SetEnable(!bLock);
}

void CUILoginNew::updateLogo()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	m_nLogoType = pLoginData->GetLogoType();

	switch(m_nLogoType)
	{
	case eLOGO_NORMAL:
		{
			if (m_pLogoFrame != NULL)
				m_pLogoFrame->Hide(FALSE);

			if (m_pImgLogo != NULL)
				m_pImgLogo->Hide(FALSE);
		}
		break;
	case eLOGO_HOLLOWEEN:
		{
			if (m_pImgLogoHal != NULL)
				m_pImgLogoHal->Hide(FALSE);

			if (m_pImgHalEye != NULL)
				m_pImgHalEye->Hide(FALSE);
		}
		break;
	case eLOGO_XMAS:
		{
			if (m_pImgLogoXmas != NULL)
				m_pImgLogoXmas->Hide(FALSE);

			UtilHelp* pUtil = UtilHelp::getSingleton();
			COLOR colAlpha = 0xFFFFFF00;

			if (pUtil != NULL)
			{
				colAlpha = DEF_UI_COLOR_WHITE;
				colAlpha = pUtil->GetColorAlpha(DEF_ALPHA_LV_XMAS_MIN, colAlpha);
			}

			if (m_pImgXmasEffect1 != NULL)
			{
				m_pImgXmasEffect1->Hide(FALSE);
				m_pImgXmasEffect1->setPBT(PBT_ADDALPHA);
				m_pImgXmasEffect1->SetAlpha(colAlpha);
			}

			if (m_pImgXmasEffect2 != NULL)
			{
				m_pImgXmasEffect2->Hide(FALSE);
				m_pImgXmasEffect2->setPBT(PBT_ADDALPHA);
				m_pImgXmasEffect2->SetAlpha(colAlpha);
			}
		}
		break;
	}
}

void CUILoginNew::initialize()
{
	m_pLogoFrame = (CUISpriteAni*)findUI("ani_frame");
	m_pImgLogo = (CUIImage*)findUI("img_frame");

	if (m_pLogoFrame != NULL)
		m_pLogoFrame->Hide(TRUE);

	if (m_pImgLogo != NULL)
		m_pImgLogo->Hide(TRUE);

	m_pImgLogoHal = (CUIImage*)findUI("img_frameHal");
	m_pImgHalEye = (CUIImage*)findUI("img_halEye");

	if (m_pImgLogoHal != NULL)
		m_pImgLogoHal->Hide(TRUE);

	if (m_pImgHalEye != NULL)
		m_pImgHalEye->Hide(TRUE);

	m_pImgLogoXmas = (CUIImage*)findUI("img_frameChris");
	m_pImgXmasEffect1 = (CUIImage*)findUI("img_chrisEffect1");
	m_pImgXmasEffect2 = (CUIImage*)findUI("img_chrisEffect2");

	if (m_pImgLogoXmas != NULL)
		m_pImgLogoXmas->Hide(TRUE);

	if (m_pImgXmasEffect1 != NULL)
		m_pImgXmasEffect1->Hide(TRUE);

	if (m_pImgXmasEffect2 != NULL)
		m_pImgXmasEffect2->Hide(TRUE);
		
	m_pBtnLogin = (CUIButton*)findUI("btn_login");

	if (m_pBtnLogin != NULL)
	{
		CmdLoginOk* pCmd = new CmdLoginOk;
		pCmd->setData(this);
		m_pBtnLogin->SetCommandUp(pCmd);
	}

	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
	{
		CmdLoginClose* pCmd = new CmdLoginClose;
		pCmd->setData(this);
		m_pBtnClose->SetCommandUp(pCmd);
	}

	m_pCbSelServer = (CUIComboBox*)findUI("cb_server");

	if (m_pCbSelServer != NULL)
	{
		m_pCbSelServer->AddString( _S(6084, "가미고 서버로 접속"));
		m_pCbSelServer->SetCurSel(DEF_BASIC_SERVER);
		m_pCbSelServer->SetEnable(FALSE);
#ifdef EUROUPEAN_SERVER_LOGIN
		m_pCbSelServer->AddString( _S(5777, "유러피안 서버로 접속"));
		m_pCbSelServer->SetEnable(TRUE);
		if (g_iConnectEuroupean)
		{
			m_pCbSelServer->SetCurSel(DEF_EUROUPEAN_SERVER);
		}
#endif	//	EUROUPEAN_SERVER_LOGIN		
	}

	m_pEbId = (CUIEditBox*)findUI("eb_id");
	m_pEbPw = (CUIEditBox*)findUI("eb_pw");
	m_pBtnSave = (CUICheckButton*)findUI("btn_save");

	if (m_pBtnSave != NULL)
	{
		CmdLoginSaveId* pCmd = new CmdLoginSaveId;
		pCmd->setData(this);
		m_pBtnSave->SetCommand(pCmd);
	}
}

void CUILoginNew::ReadPreviousId()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	extern CTString g_strSaveID;

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if (strlen(g_strSaveID.str_String) > 0)
	{
		m_pEbId->SetString(g_strSaveID.str_String);
		m_pEbId->SetFocus(FALSE);
		m_pEbPw->SetFocus(TRUE);
	}
	else
	{
		m_pEbId->SetFocus(TRUE);
	}
}	

#ifdef DEV_LOGIN
void CUILoginNew::ReadTextDevPreviousIdPw() // Login.txt 에 저장된 아이디 패스워드 얻어오기
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fileName = strFullPath + "Bin\\pass.txt";

	FILE *fp = NULL;
	CTString fileTesx = CTString(" ");

	if ((fp = fopen(fileName, "rt")) == NULL) 
	{
		return;
	}

	char buf[MAX_PATH] = {0,}; 
	fread(buf, MAX_PATH, 1, fp);

	fclose(fp);

	std::string		str[2];
	int		cur = 0;
	{
		char* sep = " \n";
		char* token = NULL;

		token = strtok( buf, sep );

		while( token )
		{
			if( cur < 2 )
				str[cur++] = token;

			token = strtok( NULL, sep );
		}
	}

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbId->SetString( (char*)str[0].c_str() );
	m_pEbPw->SetString( (char*)str[1].c_str() );
}
#endif // DEV_LOGIN

void CUILoginNew::WritePreviousId()
{
	extern CTString g_strSaveID;

	if (m_pEbId != NULL)
		g_strSaveID = m_pEbId->GetString();
}

BOOL CUILoginNew::IsEditBoxFocused()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return FALSE;

	return m_pEbId->IsFocused() || m_pEbPw->IsFocused();
}

void CUILoginNew::KillFocusEditBox()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbId->SetFocus( FALSE );
	m_pEbPw->SetFocus( FALSE );
}

void CUILoginNew::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if( !bVisible )
	{
		m_pEbId->SetFocus( FALSE );
		m_pEbPw->SetFocus( FALSE );
	}
}

void CUILoginNew::OnTabKeyProc()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if( m_pEbId->IsFocused() )
	{
		m_pEbId->SetFocus( FALSE );
		m_pEbPw->SetFocus( TRUE );
	}
	else if( m_pEbPw->IsFocused() )
	{
		m_pEbPw->SetFocus( FALSE );
		m_pEbId->SetFocus( TRUE );
	}
}

void CUILoginNew::setVersion( const char* strVer )
{
	m_strVersion = strVer;

	while (true)
	{
		int nFind = m_strVersion.find(", ");

		if (nFind == std::string::npos)
			break;

		m_strVersion.replace(nFind, 2, ".");
	}	
}

void CUILoginNew::OnRender( CDrawPort* pDraw )
{
	// Version
	if (m_strVersion.empty() == false)
	{
		pDraw->PutTextEx( CTString(m_strVersion.c_str()), 0, 0 );
	}
}

WMSG_RESULT CUILoginNew::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		if ( m_pBtnLogin != NULL )
		{
			if (m_pBtnLogin->IsEnabled() == FALSE)
				return WMSG_FAIL;

			PressLoginBtn();
			return WMSG_SUCCESS;
		}
	}
	else if( pMsg->wParam == VK_TAB )
	{
		OnTabKeyProc();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

const unsigned char ip4 = 144;
const unsigned char ip3 = 127;
const unsigned char ip2 = 63;
const unsigned char ip1 = 14;

void CUILoginNew::sendIP()
{
#if		!defined(WORLD_EDITOR)
	CNetClient _net;
	boost::asio::io_service& _service = _net.get_service();

	std::string strIP, strPort;

	strIP = boost::str( boost::format("%d.%d.") % (int)ip1 % (int)ip2 );
	strIP += boost::str( boost::format("%d.%d") % (int)ip3 % (int)ip4 );

	strPort = boost::str( boost::format("%d") % ((1000 * 3) + 1) );

	if (_net.init(strIP, strPort) == true)
	{
		// Send
		CNetworkMessage msg;
		Authentication::auth* packet = reinterpret_cast<Authentication::auth*>(msg.nm_pubMessage);
		
		strncpy(packet->ip, _cmiComm.cci_szAddr, 16);
		packet->port = (unsigned short)_cmiComm.cci_iPort;
		packet->nation = UtilHelp::getSingleton()->GetNationCode();

		msg.setSize(sizeof(*packet));
		
		_net.send(msg);
		_service.run();
		_net.recv();
	}

	LOG_DEBUG("Send OK!");
#endif	// WORLD_EDITOR
}

void CUILoginNew::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (STAGEMGR()->GetCurStage() == eSTAGE_LOGIN)
	{
		switch(m_nLogoType)
		{
		case eLOGO_HOLLOWEEN:
			updateLogoHal(fDeltaTime);
			break;
		case eLOGO_XMAS:
			updateLogoXmas(fDeltaTime);
			break;
		}
	}
}

void CUILoginNew::updateLogoHal( float fDelta )
{
	if (m_pImgHalEye == NULL)
		return;

	static float falphaLv = 0.0f;
	static bool	bUp = true;

	if (falphaLv <= DEF_ALPHA_LV_HOLLOWEEN_MIN)
		bUp = true;
	else if (falphaLv >= DEF_ALPHA_LV_HOLLOWEEN_MAX)
		bUp = false;

	falphaLv = calcLogoAlphaLv(eLOGO_HOLLOWEEN, falphaLv, bUp, fDelta);
	updateLogoAlpha(m_pImgHalEye, falphaLv);
}

void CUILoginNew::updateLogoXmas( float fDelta )
{
	if (m_pImgXmasEffect1 == NULL || m_pImgXmasEffect2 == NULL)
		return;

	static float falphaLv1 = DEF_ALPHA_LV_XMAS_MIN;
	static float falphaLv2 = DEF_ALPHA_LV_XMAS_MIN;
	static bool	bUp = true; // 감소인지, 증가인지
	static bool	bEffect1 = true; // 첫번째 인지 두번째 인지
	static int nEffectStep = eEFFECT_STEP_READY; // 다음 이펙트로 넘어가기위한 스탭 
		
	float* pAlpha = NULL;
	CUIImage* pImg = NULL;

	if (bEffect1 == true)
	{
		pAlpha = &falphaLv1;
		pImg = m_pImgXmasEffect1;
	}
	else
	{
		pAlpha = &falphaLv2;
		pImg = m_pImgXmasEffect2;
	}

	if ( (*pAlpha) <= DEF_ALPHA_LV_XMAS_MIN)
	{
		bUp = true;
		if (nEffectStep == eEFFECT_STEP_FINISH)
		{
			bEffect1 = !bEffect1;
			nEffectStep = eEFFECT_STEP_READY;
		}
	}
	else if ((*pAlpha) >= DEF_ALPHA_LV_XMAS_MAX)
	{
		bUp = false;
		if (nEffectStep == eEFFECT_STEP_READY)
			nEffectStep = eEFFECT_STEP_FINISH;
	}

	(*pAlpha) = calcLogoAlphaLv(eLOGO_XMAS, (*pAlpha), bUp, fDelta);

	updateLogoAlpha(pImg, (*pAlpha));
}

float CUILoginNew::calcLogoAlphaLv(int iType, float fCurAlphaLv, bool bUp, float fDeltaTime)
{
	float fAlphaLv = fCurAlphaLv;
	float fMin, fMax, fRatio;

	switch(iType)
	{
	case eLOGO_HOLLOWEEN:
		{
			fRatio = DEF_HOLLOWEEN_RATIO;
			fMin = DEF_ALPHA_LV_HOLLOWEEN_MIN;
			fMax = DEF_ALPHA_LV_HOLLOWEEN_MAX;
		}		
		break;
	case eLOGO_XMAS:
		{
			fRatio = DEF_XMAS_RATIO;
			fMin = DEF_ALPHA_LV_XMAS_MIN;
			fMax = DEF_ALPHA_LV_XMAS_MAX;
		}
		break;
	default:
		return fAlphaLv;
	}

	if (bUp == true)
	{
		fAlphaLv += (fDeltaTime * fRatio);

		if (fAlphaLv > fMax)
			fAlphaLv = fMax;
	}
	else
	{
		fAlphaLv -= (fDeltaTime * fRatio);

		if (fAlphaLv < fMin)
			fAlphaLv = fMin;
	}

	return fAlphaLv;
}

void CUILoginNew::updateLogoAlpha( CUIImage* pUpdateImage, float fAlphaLv )
{
	UtilHelp* pUtil = UtilHelp::getSingleton();

	if (pUtil == NULL || pUpdateImage == NULL)
		return;

	COLOR colAlpha = DEF_UI_COLOR_WHITE;

	colAlpha = pUtil->GetColorAlpha(fAlphaLv, colAlpha);
	pUpdateImage->SetAlpha(colAlpha);
}
