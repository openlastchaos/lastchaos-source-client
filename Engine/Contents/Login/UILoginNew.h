#ifndef __CUILOGIN_NEW_H__
#define __CUILOGIN_NEW_H__

class CUIComboBox;
class CUISpriteAni;
class CUICheckButton;
class CUIImage;
class CUIButton;

class ENGINE_API CUILoginNew : public CUIWindow
{
public:
	CUILoginNew(void);
	virtual ~CUILoginNew(void);

	void	PressLoginBtn();
	void	PressCloseBtn();
	void	PressCheckSave();
	BOOL	TryToLogin();		// 로그인 시도
	
	void	OpenUI();
	void	CloseUI();

	void	initialize();
	void	SetLogin();
	void	Reset();
	void	ResetBtn();

	void	Lock(BOOL bLock);

	void	ReadPreviousId();
	void	WritePreviousId();

	void	SetPWFocus() 
	{ 
		if (m_pEbPw != NULL)
			m_pEbPw->SetFocus(TRUE);
	}

	void	SetFocus( BOOL bVisible );
	BOOL	IsEditBoxFocused();
	void	KillFocusEditBox();
	void	OnTabKeyProc();

	void	OnRender(CDrawPort* pDraw);
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	
	void setVersion(const char* strVer);
	const char* getVersion()		{ return m_strVersion.c_str(); }
	
#ifdef DEV_LOGIN
	void	ReadTextDevPreviousIdPw(); // Login.txt 에 저장된 아이디 패스워드 얻어오기
#endif // DEV_LOGIN

	WMSG_RESULT OnKeyMessage(MSG* pMsg);
private:
	void	sendIP();
	void	updateLogo();
	void	updateLogoHal(float fDelta);
	void	updateLogoXmas(float fDelta);

	void	updateLogoAlpha(CUIImage* pUpdateImage, float fAlphaLv);
	float	calcLogoAlphaLv(int iType, float fCurAlphaLv, bool bUp, float fDeltaTime);

	CUISpriteAni*	m_pLogoFrame;
	CUIButton*		m_pBtnLogin;
	CUIButton*		m_pBtnClose;
	CUICheckButton*	m_pBtnSave;
	CUIComboBox*	m_pCbSelServer;
	CUIEditBox*		m_pEbId;
	CUIEditBox*		m_pEbPw;
	CUIImage*		m_pImgLogo;	// 기본 라카 로고
	CUIImage*		m_pImgLogoHal; // 할로윈 이벤트용 로고
	CUIImage*		m_pImgHalEye; // 할로윈 이벤트 로고의 눈 이미지
	CUIImage*		m_pImgLogoXmas;
	CUIImage*		m_pImgXmasEffect1;
	CUIImage*		m_pImgXmasEffect2;
	std::string		m_strVersion;
	int				m_nLogoType;
};
#endif	//	__CUILOGIN_NEW_H__