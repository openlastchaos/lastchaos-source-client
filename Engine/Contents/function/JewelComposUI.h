#ifndef	JEWEL_COMPOS_UI_H_
#define	JEWEL_COMPOS_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUITextBox;
class CUIIcon;
class CItems;

enum
{
	eJEWEL_COMPOS_OPEN_NORMAL = 0,
	eJEWEL_COMPOS_OPEN_CHAOS = 1,
};

class CJewelComposUI : public CUIWindow
{
public:
	CJewelComposUI();

	void PressOK();
	void SetResultJewelUI(int nVirJewelIndex);

	void openUI(int nOpenType);
	void closeUI();
	void resetUI();

	void initialize();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnRButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
private:
	void BtnLockOn();
	void BtnLockOff();

	void GuideStringChange(int nState);
	void SetNasString(SQUAD sqNas);

	bool CheckJewelItem(CItems* pItem);
	bool CheckScrollItem(CItems* pItem);
	bool CheckRunningConditions();

	void _PlaySound(int nSoundType);
	void _updateUI();
	void _updateGuide();
	void _updateSlotIcon();
	void _setResultImgHide(int nOpenType, BOOL bHide);
	void _setTitle();
	void _resetIcon(int nSlotIdx);
	void _HideChaosBackImg(BOOL bHide);
	SQUAD calcConsumeNas();
	int _getRegistJewelCount();

	void _ChangeDirectionEfect();		// 합성연출 보석 이펙트 변경
	BOOL _CheckComposDirectionTime();	// 시간 체크
	BOOL _JewelComposDirectionGradeInfo(); // 등급 별 보석 인포 얻기
	void _JewelComposDirectionStart();	// 합성연출 시작
	void _JewelComposDirectionEnd();
	void _JewelComposDirection();
	void _SetIconDisable(bool bDisable);

	// drag
	CUIImage* m_pMoveArea;

	enum
	{
		eIMG_RESULT_NORMAL = 0,
		eIMG_RESULT_CHAOS,
		eIMG_CHAOS_UI_BACK,
		eIMG_MAX
	};

	CUIImage* m_pImg[eIMG_MAX];

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	
	enum
	{
		eICON_JEWEL_START = 0,
		eICON_JEWEL1 = eICON_JEWEL_START,
		eICON_JEWEL2,
		eICON_JEWEL3,
		eICON_JEWEL_MAX,
		eICON_SCROLL = eICON_JEWEL_MAX,
		eICON_RESULT,
		eICON_MAX
	};
	
	CUIIcon* m_pIcon[eICON_MAX];

	enum
	{
		eTEXT_TITLE = 0,
		eTEXT_NAS,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];
	CUITextBox* m_pTbGuide;
	int m_nOpenUIType;

	_vecJewelInfo				m_DirectionInfo;	// 합성연출에 사용되는 보석 인포
	_vecJewelInfo::iterator		m_iterChanegeInfo;	// 합성연출에서 변경될 다음 보석을 가리키는 iterator

	int	m_nOneMoreComposScrollpay;	// 한번의 합성에 사용되는 스크롤 갯수
	DWORD m_dwStartTime;			// 합성연출 시작 시간
	DWORD m_dwChanegeTime;			// 합성연출에서 보석 변경 시간
	bool m_bComposing;
};

#endif	// JEWEL_COMPOS_UI_H_

