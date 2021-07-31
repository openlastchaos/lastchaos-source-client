#ifndef	SYSTEM_MENU_UI_H_
#define	SYSTEM_MENU_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;

class CSystemMenuUI : public CUIWindow
{
public:
	CSystemMenuUI();

	void PressOption();
	void PressHelp();
	void PressGoCharSel();
	void PressGoLogin();
	
	ENGINE_API void	Restart_Internal();
	ENGINE_API void	CancelRestart();

	void ReStartNow();
	void LogoutRelicWarring(); // 유물 아이템을 가지고 있다면 경고
	bool IsHaveRelic(); // 유물 아이템을 가지고 있는지 검사

	void ResetRestartTime(void)		{ m_llStartTime = 0;		}	
	__int64	GetRestartTime()		{ return m_llStartTime;		}
	BOOL GetMoveCharSel()			{ return m_bCharacterMove;	}

	void openUI();
	void closeUI();
	void resetUI();

	void ToggleVisible();

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	void _reStart();
	void _setServerInfo();
	void _SetBtnEnable(int nBtnType, BOOL bEnable);
	// drag
	CUIImage* m_pMoveArea;

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OPTION,
		eBTN_HELP,
		eBTN_GO_CHAR_SEL_UI,
		eBTN_GO_LOGIN_UI,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIText* m_pTextConnetingServerName;

	BOOL m_bCharacterMove;
	__int64	m_llStartTime;	// restart start time attack

};

#endif	// SYSTEM_MENU_UI_H_

