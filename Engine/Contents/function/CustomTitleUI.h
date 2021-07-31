#ifndef	__CUSTOM_TITLE_H__
#define	__CUSTOM_TITLE_H__

#include <Engine/Interface/UIWindow.h>

class CUIButton;
class CUIArray;
class CUIImage;
class CUIEditBox;
class CUITextBox;
class CUIIcon;
class CUIImageSplit;
class CUIText;

#define DEF_EFFECT_MAX 10

typedef enum eMAKE_ERROR_TYPE
{
	eMAKE_ERROR_SUCCESS = 0,
	eMAKE_ERROR_NO_STRING,
	eMAKE_ERROR_FILTERING_NOPASS,
	eMAKE_ERROR_SAME_UNIQUE_TITLE,
	eMAKE_ERROR_NOT_SELECT_FRONT_COLOR,
	eMAKE_ERROR_NOT_SELECT_BACK_COLOR,
	eMAKE_ERROR_NOT_SELECT_EFFECT,
	eMAKE_ERROR_NO_HAVE_CASHITEM,
	eMAKE_ERROR_NOT_TITLE_MAKE_ITEM,
	eMAKE_ERROR_FULL_INVEN,
} MakeError;

enum eChangeType
{
	eChange_Right = 0,
	eChange_Left,
};

class CCustomTitleUI : public CUIWindow
{
public:
	CCustomTitleUI();
	~CCustomTitleUI();

	void PressMakeBtn();
	void MakeTitle();
	void CancelMakeTitle();
	void SelectEffect();
	void SelectFrontColor();
	void SelectBackColor();
	void ChangeMenu(int nMenuType, eChangeType eType);

	void openUI(float fX, float fZ);
	void closeUI();
	void updateUI();

	void initialize();

	BOOL IsEditBoxFocused();
	void KillFocusEditBox();

	void OnRender(CDrawPort* pDraw);
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void		_HudModelSetting();
	void		_resetUI();
	void		_updateSelectMenu(int nMenuType);
	void		_updatePreview();
	
	bool		_InputTitleName();
	void		_resetTitleName();
	int			_getSelectIndex(int selType);
	char*		_getTitleName();

	bool		IsFilteringPass();
	bool		IsSameUniqueTitleSring();
	bool		IsReadyMakeTitle();
	MakeError	_CheckMakeError();
	void		_CheckMakeBtnEnable();
	void		_CheckMsgBox();
	void		ShowErrorMessage(int nErrorCode);

	// Position of target npc
	float		m_fNpcX, m_fNpcZ;
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;
	int			m_nMsgID;
	CUIImage* m_pMoveArea;
	CUIImage* m_pImgPreviewBack;
	CUIEditBox* m_pEditTitleName;
	CUITextBox* m_pTbGuide;
	CUIIcon* m_pIconCashItem;

	UIRectUV m_uvSelectEffect[DEF_EFFECT_MAX];

	enum
	{
		eMENU_FRONT_COLOR = 0,
		eMENU_BACK_COLOR,
		eMENU_EFFECT,
		eMENU_MAX,
	};

	CUIArray* m_pArray[eMENU_MAX];
	int m_nShowStartIdx[eMENU_MAX];

	enum
	{
		eBTN_MAKE = 0,
		eBTN_CANCEL,
		eBTN_CLOSE,
		eBTN_FRONT_LEFT,
		eBTN_FRONT_RIGHT,
		eBTN_BACK_LEFT,
		eBTN_BACK_RIGHT,
		eBTN_EFFECT_LEFT,
		eBTN_EFFECT_RIGHT,
		eBTN_MAX,
	};

	CUIButton* m_pbtn[eBTN_MAX];

	CRenderTexture* m_ModelRenderTarget; // 3D 모델이 실제 랜더링 될 공간
	UIRect m_rtPreview;
};

#endif	//	__CUSTOM_TITLE_H__