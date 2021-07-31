#ifndef	SOCKET_REMOVE_ALL_UI_H_
#define	SOCKET_REMOVE_ALL_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUIIcon;
class CItems;

class CSocketRemoveAllUI : public CUIWindow
{
public:
	CSocketRemoveAllUI();

	bool CheckSocketItem(CUIIcon* pIcon);
	void PressOK();
	void OkBtnLockOn();
	void OkBtnLockOff();

	void openUI();
	void closeUI();
	void resetUI();

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	SQUAD calcConsumeNas(CItems* pItem);
	void setNeedMoney(SQUAD nas);

	// drag
	CUIImage* m_pMoveArea;

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIText* m_pConsumeNas;
	CUIIcon* m_pSocketItem;
};

#endif	// SOCKET_REMOVE_ALL_UI_H_

