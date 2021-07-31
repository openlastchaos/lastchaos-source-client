#ifndef	SOCKET_COMBINE_UI_H_
#define	SOCKET_COMBINE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUITextBox;
class CUIIcon;
class CUIProgressBar;
class CItems;

enum eJuwelslotState
{
	eSLOT_STATE_NONE = -1,
	eSLOT_STATE_BLOCK = 0,
	eSLOT_STATE_COMBINE
};

class CSocketCombineUI : public CUIWindow
{
public:
	CSocketCombineUI();

	// command
	void PressOK();
	void ProgressComplete();

	void openUI();
	void closeUI();
	void resetUI();

	void initialize();

	// funtion
	void ShowResultSuccess(int nItemVirIdx);
	void ShowResultFail();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void BtnLockOn();
	void BtnLockOff();

	void GuideStringChange(int nState);
	void JuwelSlotStateChange(int nSlotIdx, eJuwelslotState eState = eSLOT_STATE_NONE);

	void SetNasString(SQUAD sqNas);
	void SetSlotState(int nSlotIdx, int nState);

	bool CheckItem(CItems* pItem);
	bool CheckScrollItem(CItems* pItem);
	bool CheckJewelItem(CItems* pItem, int nSlotIdx);

	void _resetProgress();
	void _resetIcon(int nSlotIdx);
	void _resetSocketSlot();
	void _updateSlotState();	
	void _updateBtn(int nSlotIdx);
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
	CUIImage* m_pImgCombine[eIMG_SLOT_MAX];
	
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
		eICON_SLOT_SOCKET_MAX,
		eICON_SCROLL = eICON_SLOT_SOCKET_MAX,
		eICON_MAX
	};
	
	CUIIcon* m_pIcon[eICON_MAX];
	int m_nIconImgState[eICON_MAX];

	CUIText* m_pTextConsumeNas;
	CUITextBox* m_pTbGuide;
	CUIProgressBar* m_pProgressBar;

	bool m_bProgress;
	bool m_bUseDefendScroll;
	int m_nRegistSlotIdx;
};

#endif	// SOCKET_COMBINE_UI_H_

