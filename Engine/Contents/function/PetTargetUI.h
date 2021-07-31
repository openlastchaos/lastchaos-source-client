#ifndef	PET_TARGET_UI_H_
#define	PET_TARGET_UI_H_

class CPetTargetUI : public CUIWindow
{
public:
	CPetTargetUI();

	void TogglePetInfo();

	void initialize();

	void openUI();
	void closeUI();
	void updateUI();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void UpdateHPInfo();
	void UpdateHungryInfo();
	void _updatePetInfoBtntooltip();

	enum 
	{
		eBACK_TITLE = 0,
		eBACK_TAIL,
		eBACK_MAX
	};

	CUIImageSplit* m_pImgSplit[eBACK_MAX];

	enum
	{
		eTEXT_LEVEL = 0,
		eTEXT_NAME,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	enum
	{
		eIMG_HP = 0,
		eIMG_HUNGRY,
		eIMG_MAX
	};
	
	CUIImage* m_pImg[eIMG_MAX];
	CUIButton* m_pBtnPetInfo;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;
};

#endif	// PET_TARGET_UI_H_

