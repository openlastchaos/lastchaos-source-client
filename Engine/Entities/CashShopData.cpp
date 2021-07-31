#include "stdh.h"
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/CashShopData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>

#define NEW_SHOP
#define ITEM_ENABLE // wooss 060503 상품 등록을 용이하게(판매중지된 제품 장바구니에 표시)

//////////////////////////////////////////////////////////////////////////
// 상품 플래그 
#ifdef NEW_SHOP
#define		CATALOG_FLAG_NEW	(1 << 0)	// 신상품
#define		CATALOG_FLAG_POP	(1 << 1)	// 인기상품
#endif
//////////////////////////////////////////////////////////////////////////

CCashShopData::~CCashShopData()
{
	m_vShopItemArray.clear();
	
}

int CCashShopData::LoadShopDataFromFile(CStaticArray<CCashShopData> &apShopData, const char* FileName)
{
	FILE *fp		= NULL;
	
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
	//	MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		ASSERT("Catalog File is not Exist.");
		return -1;
	}
	int shopDataCnt = 0;
	int	iLength		= 0;
	int	iReadBytes	= 0;
	
	iReadBytes = fread(&shopDataCnt, sizeof(int), 1, fp);		// CASH_SHOP 아이템 종류 수
	apShopData.New(MAX_CASHSHOP_CLASS);
	ASSERT(apShopData.Count() > 0 && "Invalid CASH SHOP Data");		
	ASSERT(shopDataCnt > 0 && "Invalid CASH SHOP Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION  
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	


	for(int i = 0; i < shopDataCnt; ++i)
	{
		int iIndex	;
		int itype	;	
		char tv_name[1024];
		char tv_desc[2048];

		LOADINT(iIndex);
		if(iReadBytes <= 0)		break;
		LOADINT(itype);
		ASSERT(itype>=0 && "The type is start from 0 !!");
		CCashShopData& CS			= apShopData[itype];
#ifdef NEW_SHOP
		CCashShopData& NewCS		= apShopData[8];	// 신상품
		CCashShopData& HotCS		= apShopData[9];	// 인기상품
#endif
		CASH_SHOP_DATA ShopData;		
		ShopData.m_shopItemIndex	= iIndex;
		ShopData.m_type = itype;
		LOADINT(ShopData.m_cash);
		LOADINT(ShopData.m_mileage);
		
		LOADINT(ShopData.m_texID);
		LOADINT(ShopData.m_texRow);
		LOADINT(ShopData.m_texCol);
#ifdef NEW_SHOP		// 상품 플래그
		LOADINT(ShopData.m_flag);
#endif

#ifdef ITEM_ENABLE
		LOADCHAR(ShopData.m_enable);
#endif


		memset(tv_name,'\0',sizeof(tv_name));
		LOADSTR(tv_name);
		ShopData.m_itemName = tv_name;
		memset(tv_desc,'\0',sizeof(tv_desc));
		LOADSTR(tv_desc);
		ShopData.m_itemDesc=tv_desc;
		LOADINT(ShopData.m_itemListCnt);

		ASSERT(ShopData.m_itemListCnt >= 0 && "Invalid Item Count!!!");
		ShopData.m_vItemDataArray.size();
		ShopData.m_vItemDataArray.resize(ShopData.m_itemListCnt);

		for(int j=0;j<ShopData.m_itemListCnt;j++ )
		{
			LOADINT(ShopData.m_vItemDataArray[j].m_itemIndex);
			LOADINT(ShopData.m_vItemDataArray[j].m_flag);
			LOADINT(ShopData.m_vItemDataArray[j].m_plus);
			LOADINT(ShopData.m_vItemDataArray[j].m_option);
			LOADINT(ShopData.m_vItemDataArray[j].m_itemCnt);

			if(iReadBytes <= 0)
			{
				MessageBox(NULL, "CASH SHOP 데이터 화일이 올바르지 않습니다.", "Error!", MB_OK);
				fclose(fp);
				return -1;
			}
		}
		ShopData.m_typeIndex=CS.m_vShopItemArray.size();
#ifdef NEW_SHOP
		if( ( ( ShopData.m_flag & CATALOG_FLAG_NEW ) >> 0 ) )		// eons	// 신상품
		{
			ShopData.m_typeIndex=NewCS.m_vShopItemArray.size();
			NewCS.m_vShopItemArray.push_back( ShopData );
		}
		else if( ( ( ShopData.m_flag & CATALOG_FLAG_POP ) >> 1 ) )	// eons // 인기상품
		{
			ShopData.m_typeIndex=HotCS.m_vShopItemArray.size();
			HotCS.m_vShopItemArray.push_back( ShopData );
		}
#endif

		{
			ShopData.m_typeIndex=CS.m_vShopItemArray.size();
			CS.m_vShopItemArray.push_back(ShopData);
		}
	}
	fclose(fp);
	
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return shopDataCnt;
}