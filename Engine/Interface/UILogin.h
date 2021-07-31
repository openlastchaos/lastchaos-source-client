// ----------------------------------------------------------------------------
//  File : UILogin.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UILOGIN_H_
#define	UILOGIN_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIEditBox.h>
// [090715:selo] - 체크버튼 추가 
#include <Engine/Interface//UICheckButton.h>
#include <vector>


// Define text position
#define	LOGIN_TITLE_TEXT_OFFSETX	25
#define	LOGIN_TITLE_TEXT_OFFSETY	5


// Define size
#define	LOGIN_WIDTH					183
#define	LOGIN_HEIGHT				116


// ----------------------------------------------------------------------------
// Name : CUILogin
// Desc :
// ----------------------------------------------------------------------------
class CUILogin : public CUIWindow
{
public:
	CUILogin();
	~CUILogin();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Reset
	void	Reset();

	// Render
	void	Render();
	void	SecurityRender();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	CTString	GetUserId(){ return m_strUserID; };
	CTString	GetPassword(){ return m_strUserPW; };
	
	// [090715: selo] 아이디 저장
	void ReadPreviousId();
	void WritePreviousId();	
	

	void	Lock(BOOL bLock);	
	void	SetPWFocus() { m_ebPW.SetFocus(TRUE); }

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
		{
			m_ebID.SetFocus( FALSE );
			m_ebPW.SetFocus( FALSE );
		}
	}

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebID.IsFocused() || m_ebPW.IsFocused(); }
	void	KillFocusEditBox()
	{
		m_ebID.SetFocus( FALSE );
		m_ebPW.SetFocus( FALSE );
	}
	void SecurityCardSet(UBYTE* SecurityNum);

protected:
	// Press OK button
	void	PressOKBtn();

	BOOL	TryToLogin();			// 로그인 시도

	// Connect to Server
	BOOL	ConnectToLoginServer();
	//070901_ttos: 보안카드 UI	
	BOOL	ConnectToSecurityCard();
	
private:
	// User Login
	UIRect				m_rcSecurityBack;
	UIRectUV			m_rtLogin;
	UIRectUV			m_rtSecurityTop;
	UIRectUV			m_rtSecurityMid;
	UIRectUV			m_rtSecurityBottom;
	CUIEditBox			m_ebID;								// Input Box for User ID
	CUIEditBox			m_ebPW;								// Input Box for User Password
	CUIEditBox			m_ebSecurity;						// 070823_ttos: Input Box for Security Code
	int					m_nLoginMsgPosX, m_nLoginMsgPosY;	// Position of error message box for login
	
	
	// Buttons
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	
	// [090715: selo] - checkbox
	CUICheckButton		m_btnCheck;							// Check button
	BOOL				m_bChecked;

private:
	CTString 			m_strUserID;						// User ID
	CTString			m_strUserPW;						// User Password
	BOOL				m_bUserLogin;
//	BOOL				m_bConnectToLoginServer;
	CTextureData*		m_ptdClassification;					// 등급 표시 
//	int					m_LocalVersion;
	CHAR				m_chSecurityNum[4];					//보안 카드 넘버
	BOOL				m_bSecurity;						//보안카드 체크
	CTString			m_strSecurityCode;					//보안 카드 코드
	CTString			m_strSecurityPW;					//보안 카드 패스워드

	// [090715: selo] 체크버튼 텍스쳐 
	CTextureData*		m_ptdButtonTexture;
};

#endif // UISELCHAR_H_
