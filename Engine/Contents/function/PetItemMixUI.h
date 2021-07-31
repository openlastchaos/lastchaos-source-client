#ifndef	PET_ITEM_MIX_UI_H_
#define	PET_ITEM_MIX_UI_H_

#include <Engine/Interface/UIWindow.h>
#include <Engine/Entities/Itemdata.h>
#include <Engine/Interface/UIProcess.h>

class CUIButton;
class CUIImage;
class CUIIcon;
class CUIList;
class CUIText;
class CUITextBox;

 enum KNIGHT_AMOR_TYPE
 {
 	T_HELMET = 0,		// 투구
 	T_COAT,			// 상의
 	T_PANTS,			// 하의
 	T_BOOTS,			// 부츠
 	T_GLOVE,			// 장갑
 	T_SHIELD,
 	T_KNIGHT_MAX,		
 };
 
 struct  CNeedStuff {
 	int		nNeedA[3];		// 필요재료 
 	int		nNeedB[3];
 };

class CPetItemMixUI : public CUIWindow
{
public:
	CPetItemMixUI();

	void SelectItem( int nIndex, int nOpenType );

	void openUI(int nType);
	void closeUI();
	void ResetUI();

	void initialize();
	
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	// Network message functions ( receive )
	void PetItemMixRep( SBYTE sbResult );

	// Send ...
	void SendPetItemMixReq(int nBtnType);

private:
	void _setWeaponUI();
	void _setArmorUI();
	void _setMakeItemListUI(int nType);
	void _setGuild();
	void _InitNeedItem();

	enum
	{
		eBTN_CLOSE = 0, //					x 버튼
		eBTN_CANCEL,	//						|
		eBTN_FIRST,		//	btn		btn		btn	|
		eBTN_MIDDLE,	//	first	middle	can |
		eBTN_MAX		//______________________|
	};
	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eLIST_MAKE_ITEM = 0,
		eLIST_METERIAL_ITEM,
		eLIST_MAX
	};

	CUIList* m_pList[eLIST_MAX];
	CUIText* m_pTitle;
	CUITextBox* m_ptbGuide;
	// drag
	CUIImage* m_pMoveArea;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;

	CNeedItems m_NeedItems[MAX_MAKE_ITEM_MATERIAL]; // 필요 아이템 정보
	CNeedStuff m_NStuffKnight[T_KNIGHT_MAX];		// 나이트 메어 필요 재료 정보
};

#endif	// PET_ITEM_MIX_UI_H_

