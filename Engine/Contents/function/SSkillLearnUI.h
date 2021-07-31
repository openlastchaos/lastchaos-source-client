#ifndef	SSKILL_LEARN_UI_H_
#define	SSKILL_LEARN_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIButton;
class CUIText;
class CUIImage;
class CUIList;

class CSSkillLearnUI : public CUIWindow
{
public:
	CSSkillLearnUI();
	~CSSkillLearnUI();

	void BtnLock();
	void BtnUnLock();
	
	void openUI(int iMobIndex, int iMobVirIdx, FLOAT fX, FLOAT fZ);
	void closeUI();
	void updateList();
	void resetUI();

	void initialize();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	void SendLearnSSkill();

private:
	void _setSubTitle();
	void _updateSP();

	bool _CheckSatisfied();

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eTEXT_SUB_TITLE = 0,
		eTEXT_SKILL_POINT,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	CUIImage* m_pMoveArea;
	CUIList* m_pSkillList;

	FLOAT m_fNpcX, m_fNpcZ;
	int m_iMobIdx;
	int m_iMobVirIdx;

	std::vector<int> m_vecSatisfied;
};

#endif	// SSKILL_LEARN_UI_H_

