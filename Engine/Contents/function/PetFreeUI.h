#ifndef	PET_FREE_UI_H_
#define	PET_FREE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIButton;
class CUIImage;
class CUIIcon;
class CUIText;
class CUITextBox;

class CPetFreeUI : public CUIWindow
{
public:
	CPetFreeUI();

	// 프리미엄 캐릭터
	void SetPremiumBenefit(bool bUse) { m_bPremiumChar = bUse; }

	void PressOK();
	// Send Network
	void SendPetFreeReq();
	// recevie
	void PetFreeError( SLONG PetIndex, SBYTE sbResult );
	
	void openUI(float fX, float fZ, int nIdex);
	void closeUI();
	void ResetUI();

	void initialize();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:	
	void _resetPetSlot();
	void _setNas(SQUAD llNas = -1);
	void _setOkBtnEnable(BOOL bEnable);
	SQUAD _calcPetFreeConsumeNas(int nPetLevel);
	bool _CheckPetItem();

	enum
	{
		eBTN_OK = 0,
		eBTN_CANCEL,
		eBTN_CLOSE,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIText* m_pstrNas;
	CUIIcon* m_pIconPet;
	CUITextBox* m_pTbContent;

	// drag
	CUIImage* m_pMoveArea;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;

	SLONG m_slPetIndex;
	int	m_nPettype;
	float m_fNpcPosX, m_fNpcPosZ;
	int	m_nNpcIndex;
	bool m_bPremiumChar;
};

#endif	// PET_FREE_UI_H_

