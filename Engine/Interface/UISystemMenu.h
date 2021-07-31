// ----------------------------------------------------------------------------
//  File : UISystemMenu.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISYSTEMMENU_H_
#define	UISYSTEMMENU_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>


// Define text position
#define	SYSMENU_TITLE_TEXT_OFFSETX	25
#define	SYSMENU_TITLE_TEXT_OFFSETY	5


// Define size of system menu
//#define	SYSTEMMENU_WIDTH			216
//#define	SYSTEMMENU_HEIGHT			114
#define	SYSTEMMENU_WIDTH			178
#define	SYSTEMMENU_HEIGHT			191


// ----------------------------------------------------------------------------
// Name : CUISystemMenu
// Desc :
// ----------------------------------------------------------------------------
class CUISystemMenu : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnOption;							// Option button
	CUIButton			m_btnHelp;								// Help button
	CUIButton			m_btnRestart;							// Restart button
	CUIButton			m_btnExit;								// Exit button

	// Region of each part
	UIRect				m_rcTitle;								// Region of title

	// UV of each part
	UIRectUV			m_rtBackground;							// UV of background

	CTextureData		*m_ptdButtonTexture;		// Texture of Button

#ifdef RESTART_GAME
	BOOL				m_bRestartGame;							// Game restart reserve value
	__int64				m_llStartTime;							// restart start time attack
#endif	

protected:
	// Command functions
	void	OpenOption();
	void	OpenHelp();
	void	Restart();
#ifndef RESTART_GAME
	void	Restart_Internal();
#endif
	void	Exit();

public:
	CUISystemMenu();
	~CUISystemMenu();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Toggle visible
	void	ToggleVisible();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

#ifdef RESTART_GAME
	void	Restart_Internal();

	void	SetRestartGameValue(BOOL bSet)		{ m_bRestartGame = bSet; }
	BOOL	GetRestartGameValue()				{ return m_bRestartGame; }
	void	ResetRestartTime(void)				{ m_llStartTime = 0; }
	BOOL	IsRestartGame()						{ return (m_bRestartGame ? TRUE : FALSE); }
	__int64	GetRestartTime()					{ return m_llStartTime; }
#endif
};


#endif	// UISYSTEMMENU_H_

