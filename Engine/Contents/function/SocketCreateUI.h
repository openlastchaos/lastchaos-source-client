#ifndef	SOCKET_CREATE_UI_H_
#define	SOCKET_CREATE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUIIcon;
class CUIProgressBar;
class CItems;

class CSocketCreateUI : public CUIWindow
{
public:
	CSocketCreateUI();

	void PressOK();
	void ProgressComplete();
	void BtnLockOn();
	void BtnLockOff();
	void ShowResultSuccess(int nItemVirIdx);
	void ShowResultFail();

	void openUI();
	void closeUI();
	void resetUI();

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void _setCompleteUI();
	void changeGuide(int nState);
	void SetNasString(SQUAD sqNas);
	bool CheckItem(CItems* pItem);
	bool CheckScrollItem(CItems* pItem);
	SQUAD calcConsumeNas(CItems* pItem);

	// drag
	CUIImage* m_pMoveArea;

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	
	enum
	{
		eTEXT_GUIDE = 0,
		eTEXT_NAS,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	enum
	{
		eICON_ITEM = 0,
		eICON_SCROLL,
		eICON_RESULT,
		eICON_MAX
	};
	
	CUIIcon* m_pIcon[eICON_MAX];
	CUIProgressBar* m_pProgressBar;

	bool m_bProgress;
};

#endif	// SOCKET_CREATE_UI_H_

