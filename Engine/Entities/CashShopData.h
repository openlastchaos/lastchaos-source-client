#ifndef _CASHSHOP_DATA_H
#define _CASHSHOP_DATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>

#define	MAX_CASHSHOP_CLASS					10		// wooss 050902 a kind of cash shop items

enum CASH_SHOP_KIND
{	
	CONSUME_ITEM = 0,
	TIME_LIMIT_ITEM,
	POTION_ITEM,
	AVATA_ITEM,
	WEAPON_ITEM,
	DEFEND_ITEM,
	SET_ITEM,
	PLATINUM_ITEM,
	CASH_SHOP_TOTAL,

};


class ENGINE_API CCashShopData
{
	
public :

	typedef struct _ItemData
	{
		INDEX		m_itemIndex;
		INDEX		m_flag;
		INDEX		m_plus;
		INDEX		m_option;
		INDEX		m_itemCnt;
	} ITEM_DATA;


	// inner data class
	class CASH_SHOP_DATA
	{
	public :
		INDEX		m_shopItemIndex;
		INDEX		m_type;
		INDEX		m_cash;
		INDEX		m_mileage;

		INDEX		m_texID;
		INDEX		m_texRow;
		INDEX		m_texCol;
		INDEX		m_flag;
		CHAR		m_enable;

		CTString	m_itemName;
		CTString	m_itemDesc;
		INDEX		m_itemListCnt;
		INDEX		m_typeIndex;		// 각 상품 배열에서의 인덱스 

		INDEX		m_limitCnt;			// 플래티늄 아이템 남은 수량

		std::vector<ITEM_DATA> m_vItemDataArray;
		
		CASH_SHOP_DATA() :	m_shopItemIndex(-1) ,
							m_type(0),
							m_cash(0),
							m_mileage(0),
							m_itemListCnt(0) ,
							m_limitCnt(0) {};
		~CASH_SHOP_DATA() { m_vItemDataArray.clear(); }
		
	};

	~CCashShopData();
	
	std::vector<CASH_SHOP_DATA> m_vShopItemArray;

	static int	LoadShopDataFromFile(CStaticArray<CCashShopData> &apShopData, const char* FileName);
};

#endif