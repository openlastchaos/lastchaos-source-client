#ifndef INCL_ITEMS_H
#define INCL_ITEMS_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>


#define	ITEM_PLUS_COFACTOR		(1.06f)			// 업그레이드 수치 공식에 필요한 인자

class  CItems
{
public:

	CItemData	ItemData;

	SBYTE	Item_Tab;
	SBYTE	Item_Row; 
	SBYTE	Item_Col;

	LONG	Item_Index;
	LONG	Item_UniIndex;
	
	ULONG	Item_Plus;
	ULONG	Item_Flag;
	LONG	Item_Used;
	LONG	Item_Used2;	

	SBYTE	Item_Wearing;

	SQUAD	Item_Sum;
	SQUAD	Item_Price;

	SBYTE	Item_OptionType[MAX_ITEM_OPTION];
	SBYTE	Item_OptionLevel[MAX_ITEM_OPTION];
	LONG	ComItem_index;

	LONG	Item_RareIndex;
	sPetItem_Info	Item_PetInfo;


public:
	/* Default constructor. */
	CItems(void);
	/* Destructor. */
	~CItems(void);

	void	Init(void);
	void	SetData( LONG lIndex, LONG lUniIndex, SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, ULONG ulPlus,
						ULONG ulFlag, LONG lUsed, LONG lUsed2, SBYTE sbWearing, SQUAD llSum);
	void	SetPrice(SQUAD nPrice);
	void	InitOptionData(void);
	void	SetOptionData( SBYTE sbIndex, SBYTE sbOptionType, SBYTE sbOptionLevel );
	void	SetRareIndex( LONG iRareIndex ) { Item_RareIndex =iRareIndex; }
	void	SetPetInfo(CTString petName, int petLeve, int petStr, int petCon, int petDex, int petInt); 

	SBYTE	GetOptionType( SBYTE sbOption ) const { return Item_OptionType[sbOption]; }
	SBYTE	GetOptionLevel( SBYTE sbOption ) const { return Item_OptionLevel[sbOption]; }

	static	int CalculatePlusDamage( int iAttack, ULONG ulItemPlus );

	inline BOOL IsFlag( int nFlag ) const 
	{
		if( Item_Flag & nFlag ) return TRUE;
		return FALSE;
	}
	
	int GetWearLevelReduction();			//착용레벨감소옵션(Index:49)이 붙은 장비를 입고 있을 경우 감소되는 레벨량 구함.
	BOOL	IsEmptyItem(void)
	{
		return (Item_UniIndex<0);
	}
};

// Function Object
struct FindItem
{
	LONG iNum;
	FindItem( CItems& item ) : iNum( item.Item_UniIndex ) {};
	bool operator() (const CItems& item)
	{
		if(item.Item_UniIndex == iNum)
		{
			return true;
		}
		else
			return false;
	}
};

#endif