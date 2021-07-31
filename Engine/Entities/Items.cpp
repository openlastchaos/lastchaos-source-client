
#include "stdh.h"

#include <Engine/Entities/Items.h>
#include <Engine/Network/CNetwork.h>

CItems::CItems(void) 
{
	//ItemData = NULL;
	
	Init(); 
}

CItems::~CItems(void) 
{
//	memset(&ItemData, 0, sizeof(_ItemStat));
	Item_Tab = -1;
	Item_Row = -1; 
	Item_Col = -1;

	Item_Index = -1;
	Item_UniIndex = -1;

	Item_Plus = 0;
	Item_Flag = 0;
	Item_Used = -1;

	Item_Wearing = -1;

	Item_Sum = 0;
	Item_Price = 0;

	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
	}
	
	ComItem_index = -1;

	Item_RareIndex =0;
}

void CItems::Init(void)
{
	memset(&ItemData, 0, sizeof(ItemData));

	Item_Tab = -1;
	Item_Row = -1; 
	Item_Col = -1;

	Item_Index = -1;
	Item_UniIndex = -1;

	Item_Plus = 0;
	Item_Flag = 0;
	Item_Used = -1;

	Item_Wearing = -1;

	Item_Sum = 0;
	Item_Price = 0;

	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
	}

	Item_RareIndex =0;
}

void CItems::SetPrice(SQUAD nPrice)
{
	Item_Price = nPrice;
}

void CItems::SetData( LONG lIndex, LONG lUniIndex, SBYTE sbTab, SBYTE sbRow, SBYTE sbCol,
						ULONG ulPlus, ULONG ulFlag, LONG lUsed, LONG lUsed2, SBYTE sbWearing, SQUAD llSum)
{
	Item_Index = lIndex;
	Item_UniIndex = lUniIndex;
	Item_Tab = sbTab;
	Item_Row = sbRow;
	Item_Col = sbCol;
	Item_Plus = ulPlus;
	Item_Flag = ulFlag;						
	Item_Used = lUsed;
	Item_Used2 = lUsed2;
	Item_Wearing = sbWearing;
	Item_Sum	= llSum;
}

void CItems::InitOptionData(void) 
{
	for( int i = 0; i < MAX_ITEM_OPTION; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
	}
}

void CItems::SetOptionData( SBYTE sbIndex, SBYTE sbOptionType, SBYTE sbOptionLevel )
{
	Item_OptionType[sbIndex] = sbOptionType;
	Item_OptionLevel[sbIndex] = sbOptionLevel;
}

// 아이템 플러스 수치 변경.
#define NEW_UPGRADE_ITEM
int CItems::CalculatePlusDamage( int iAttack, ULONG ulItemPlus )
{
	int nPlusValue = 0;
#ifdef NEW_UPGRADE_ITEM
	if( ulItemPlus >= 15 )
		nPlusValue = (int)( iAttack * pow( (1.07f), ulItemPlus ) ) - iAttack;
	else if( ulItemPlus >= 11 )
	{
		nPlusValue = (int)( iAttack * pow( (1.07f), ulItemPlus ) ) - iAttack;
	}
	else if( ulItemPlus >= 1 )
	{
		nPlusValue = (int)( iAttack * pow( (1.06f), ulItemPlus ) ) - iAttack;
	}	
#else
	nPlusValue = (int)( iAttack * pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - iAttack;
#endif
	return nPlusValue;
}

//착용레벨 감소 옵션(Index: 49)이 붙은 장비를 입고 있을 경우 감소되는 레벨량을 구함. 
//해당 옵션이 없으면 0 리턴!
int CItems::GetWearLevelReduction()
{
	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; ++sbOption )
	{
		SBYTE sbOptionType = GetOptionType( sbOption );
		SBYTE sbOptionLevel = GetOptionLevel( sbOption );

		if( sbOptionType == -1 || sbOptionLevel == 0 )
			break;

		//착용제한레벨 다운 옵션
		if( sbOptionType==49)
		{
			COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType );
			return odItem.GetValue( sbOptionLevel - 1 );
		}
	}
	
	return 0;
}

void CItems::SetPetInfo(CTString petName, int petLeve, int petStr, int petCon, int petDex, int petInt)
{
	Item_PetInfo.pet_name = petName;
	Item_PetInfo.pet_level = petLeve;
	Item_PetInfo.pet_str = petStr;
	Item_PetInfo.pet_con = petCon;
	Item_PetInfo.pet_dex = petDex;
	Item_PetInfo.pet_int = petInt;
}