#ifndef	SOCKET_REMOVE_SINGLE_UI_H_
#define	SOCKET_REMOVE_SINGLE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUITextBox;
class CUIIcon;
class CUIProgressBar;
class CItems;

class CSocketRemoveSingleUI : public CUIWindow
{
public:
	CSocketRemoveSingleUI();

	enum eJuwelslotState
	{
		eSLOT_STATE_NONE = -1,
		eSLOT_STATE_BLOCK = 0,
		eSLOT_STATE_SELECT
	};

	// command
	void PressOK();

	void openUI();
	void closeUI();
	void resetUI();

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void BtnLockOn();
	void BtnLockOff();

	void GuideStringChange(int nState);
	void JuwelSlotStateChange(int nSlotIdx, eJuwelslotState eState = eSLOT_STATE_NONE);

	void SetNasString(SQUAD sqNas);
	bool CheckItem(CItems* pItem);

	void _resetIcon(int nSlotIdx);
	void _resetSocketSlot();
	void _updateSlotState();

	// drag
	CUIImage* m_pMoveArea;

	enum
	{
		eIMG_SLOT1 = 0,
		eIMG_SLOT2,
		eIMG_SLOT3,
		eIMG_SLOT4,
		eIMG_SLOT5,
		eIMG_SLOT6,
		eIMG_SLOT7,
		eIMG_SLOT_MAX
	};

	CUIImage* m_pImgBlock[eIMG_SLOT_MAX];
	CUIImage* m_pImgSelect[eIMG_SLOT_MAX];
	
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	
	enum
	{
		eICON_ITEM = 0,
		eICON_SLOT_SOCKET1,
		eICON_SLOT_SOCKET2,
		eICON_SLOT_SOCKET3,
		eICON_SLOT_SOCKET4,
		eICON_SLOT_SOCKET5,
		eICON_SLOT_SOCKET6,
		eICON_SLOT_SOCKET7,
		eICON_MAX
	};
	
	CUIIcon* m_pIcon[eICON_MAX];
	CUIText* m_pTextConsumeNas;
	CUITextBox* m_pTbGuide;

	int m_nSelectIndex;
};

#endif	// SOCKET_REMOVE_SINGLE_UI_H_

