#ifndef	WILDPET_TARGET_UI_H_
#define	WILDPET_TARGET_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImageArray;
class CUIImageSplit;
class CUICheckButton;
class CUIText;
class CUIProgressBar;
class CUIButton;

class CWildPetTargetUI : public CUIWindow
{
public:
	CWildPetTargetUI();
	~CWildPetTargetUI();

	// command
	void ExtendBtnPress();
	void ToggleWildPetInfo();

	void initialize();

	void openUI();
	void closeUI();

	void updateUI();
	void updateBase();
	void updateExpPet();
	void updateExtend();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void _setUI();
	void _setUIExtend();
	void _setUIExpPet();

	FLOAT calcAccuExpInfo();

	FLOAT calcHPBarInfo();
	FLOAT calcMPBarInfo();
	FLOAT calcEXPBarInfo();
	FLOAT calcAccuExpBarWidth();

	void _updateAccExpPetTooltip();
	void _updateOpenWildPetInfoTooltip();
	// exp Pet 
	CUIImageSplit* m_pIsExpPetBack;
	CUIText* m_pTextExpPetUseExp;
	CUIImage* m_pImgAccExp;
	// extend UI
	enum eIMG_ARRAY_TYPE
	{
		eIMG_ARR_HUNGRY = 0,
		eIMG_ARR_FAITH,
		eIMG_ARR_MAX
	};
	CUIImageSplit* m_pIsExtendBack;
	CUIImageArray* m_pImgArray[eIMG_ARR_MAX];
	CUIImageArray* m_pIaAIState;
	CUIText* m_pTextArr[eIMG_ARR_MAX];

	// base info
	enum eBTN_TYPE
	{
		eBTN_EXTEND_UP = 0,
		eBTN_EXTEND_DOWN,
		eBTN_OPENINFO,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIImageSplit* m_pIsBaseBack;

	enum eTEXT_TYPE
	{
		eTEXT_LEVEL = 0,
		eTEXT_NAME,
		eTEXT_HP,
		eTEXT_MP,
		eTEXT_EXP,
		eTEXT_MAX
	};

	CUIText* m_pTextBase[eTEXT_MAX];

	enum ePROGRESS_TYPE
	{
		ePROGRESS_HP = 0,
		ePROGRESS_MP,
		ePROGRESS_EXP,
		ePROGRESS_MAX
	};

	CUIProgressBar* m_pbBarBase[ePROGRESS_MAX];

	CUIImage* m_pMoveArea;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;
	bool m_bExtendUI;

};

#endif	// WILDPET_TARGET_UI_H_

