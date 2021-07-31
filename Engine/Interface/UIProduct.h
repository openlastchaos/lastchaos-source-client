
// ----------------------------------------------------------------------------
//  File : UIProduct.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIPRODUCT_H_
#define	UIPRODUCT_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Interface/UIListBoxEx.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIProcess.h>

// Define max char and line of strings
#define	MAX_PRODUCT_STRING			4
#define	PRODUCT_STRING_CHAR_WIDTH	170

// Define text position
#define	PRODUCT_TITLE_TEXT_OFFSETX	25
#define	PRODUCT_TITLE_TEXT_OFFSETY	5
#define	PRODUCT_DESC_TEXT_SX		18
#define	PRODUCT_DESC_TEXT_SY		33

// Define size of Product
#define	PRODUCT_TOP_HEIGHT			339
#define	PRODUCT_BOTTOM_HEIGHT		41

#define	PRODUCT_WIDTH				216
#define	PRODUCT_HEIGHT				380

#define PRODUCT_SLOT_ROW			4
#define PRODUCT_SLOT_ROW_TOTAL		30

// ----------------------------------------------------------------------------
// Name : CUIProduct
// Desc : 제조 
// ----------------------------------------------------------------------------

enum eSealType
{
	SEAL_1 = 0,		//정교한 제작
	SEAL_2,			//화려한 제작
	SEAL_3,			//날카로운 제작
	SEAL_4,			//단단한 제작
	SEAL_5,			//소모품 제작
	SEAL_MEMBER,	//회원 등록
};

#define SEALTYPE_START_INDEX 656	 

#define MAX_SEALTYPE	5
#define MAX_SEALSUBTYPE	23

extern int SealTypeCount[MAX_SEALTYPE+1];
extern int SealSubType[MAX_SEALSUBTYPE];


class CUIProduct : public CUIWindow
{
protected:		
	// Skill buttons
	std::vector<CItems*>	m_vecItems;							// 가공 아이템

//	Product Item Info

	int						m_nProductText;						// 가공 문서 아이템 인덱스
	SWORD					m_nTab;
	SWORD					m_nInvenIdx;

	int						m_nSelectProductItem;				// 현재 선택된 가공물
	CTString				m_StrProductType;					// 가공 타입
	int						m_nProductItemCount;				// 가공문서에 맞는 가공품 갯수
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보

	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
		
// Network ...
	BOOL					m_bWaitProductResult;				// Wait Message

public:
	CUIProduct();
	~CUIProduct();
	
// Open & close Product
	void	Clear ();
	void	InitProduct();

	ENGINE_API	void	OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx );
	void				CloseProduct();

// Messages
	WMSG_RESULT	MouseMessage1( MSG *pMsg );
	
// Network message functions ( receive )
	void	ProductRep( SBYTE sbResult );
	
// Send ...
	void	SendProductReq();

	void	SelectItem(int nIndex = -1);

	void	initialize();
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:

	void	clearItems();
	
	void	AddString(CTString& strText, COLOR colText = 0xffffffff);

	void	pushback_string(CTString& strText, COLOR color);
	void	callback_select();

	CUIText*	m_pTxtSubject;
	CUIList*	m_pListItems;
	CUIList*	m_pListDesc;
	CUIButton*	m_pBtnOK;

};

#endif	// UIProduct_H_

