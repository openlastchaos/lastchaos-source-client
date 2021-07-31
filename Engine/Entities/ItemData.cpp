
#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/CNetwork.h>

#define LOAD_ITEMARMOR_EFFECT
//#define DEL_PET_AI

/*
 *  Constructor.
 */
CItemData::CItemData(void)
:Item_MeshCnt(0), Item_TexCnt(0), Item_Tex2Cnt(0)
{
	memset(&Item_Data, 0, sizeof(_ItemStat));
	Item_Data.index = -1;
	ZeroMemory(&fileBm,255);
	ZeroMemory(&fileTex,255);
	ZeroMemory(&fileTexNormal,255);
	ZeroMemory(&fileBm2,255);
	ZeroMemory(&fileTex2,255);
	ZeroMemory(&fileTex2Normal,255);
	ZeroMemory(&fileBm3,255);
	ZeroMemory(&fileTex3,255);
	ZeroMemory(&fileTex3Normal,255);
	bCreateSet = FALSE;
}

/*
 *  Destructor.
 */
CItemData::~CItemData(void) 
{
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 데이터를 파일로부터 읽어들입니다.
// Input  : &apItemData - 아이템 목록이 저장될 배열.
//			FileName - 파일명.
// Output : 	static int
//-----------------------------------------------------------------------------
int CItemData::LoadItemDataFromFile(CStaticArray<CItemData> &apItemData, const char* FileName)
{	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iLastItemIndex	= 0;
	int iLength			= -1;
	int iReadBytes		= 0;

	iReadBytes = fread(&iLastItemIndex, sizeof(int), 1, fp);				// ITEM 데이터의 마지막 인덱스.
	apItemData.New(iLastItemIndex); 
	ASSERT(apItemData.Count() >= iLastItemIndex && "Invalid Array Count");
	ASSERT(iLastItemIndex > 0 && "Invalid Item Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	
	
	for(int i = 0; i < iLastItemIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);		
		if(iReadBytes <= 0)		break;										// EOF
		ASSERT(iIndex != -1	&& "Invalid Item Index");

		CItemData& ID		= apItemData[iIndex];
		_ItemStat& ItemData = ID.Item_Data;
		ItemData.index		= iIndex;

		LOADSTR(ItemData.name);
//		LOADINT(ItemData.job);
		{ int iLen;
		LOADINT(iLen); 
		iReadBytes = fread(&ItemData.job, iLen, 1, fp); 
		}
		LOADINT(ItemData.weight);
		LOADINT(ItemData.fame);
		LOADINT(ItemData.level);
		LOADINT(ItemData.flag);

		LOADINT(ItemData.wearing);
		LOADINT(ItemData.type);
		LOADINT(ItemData.subType);

		for (int j = 0; j < MAX_MAKE_ITEM_MATERIAL; ++j)
		{
			LOADINT(ItemData.needItemIndex[j]);
			LOADINT(ItemData.needItemCount[j]);
		}

		LOADINT(ItemData.needSSkillIndex);
		LOADINT(ItemData.needSSkillCount);
		LOADINT(ItemData.needSSkillIndex2);
		LOADINT(ItemData.needSSkillCount2);

		LOADINT(ItemData.num0);
		LOADINT(ItemData.num1);
		LOADINT(ItemData.num2);
		LOADINT(ItemData.num3);

		LOADINT(ItemData.price);

		LOADSTR(ItemData.fileSMC);

		LOADINT(ItemData.iIconTexID);
		LOADINT(ItemData.iIconTexRow);
		LOADINT(ItemData.iIconTexCol);

		LOADSTR(ItemData.descr);

#ifdef LOAD_ITEMARMOR_EFFECT
		LOADSTR(ItemData.ArmorEffectName);
#endif

#ifdef DEL_PET_AI
		LOADINT(ItemData.PetAI_Set0);
		LOADINT(ItemData.PetAI_Set1);
		LOADINT(ItemData.PetAI_Set2);
		LOADINT(ItemData.PetAI_Set3);
#endif
	}
	fclose(fp);
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iLastItemIndex;
}

//-----------------------------------------------------------------------------
CItemName::CItemName()
{
	memset(&Item_Data, 0, sizeof(_ItemStat));
	Item_Data.index = -1;
}

CItemName::~CItemName()
{
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 이름를 파일로부터 읽어들입니다.
//-----------------------------------------------------------------------------
int CItemName::LoadItemNameFromFile(CStaticArray<CItemName> &apItemName, const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}
	
	int iLastItemIndex	= 0;
	int iLength			= -1;
	int iReadBytes		= 0;
	
	iReadBytes = fread(&iLastItemIndex, sizeof(int), 1, fp);				// ITEM 데이터의 마지막 인덱스.
	apItemName.New(iLastItemIndex);
	ASSERT(apItemName.Count() >= iLastItemIndex && "Invalid Array Count");
	ASSERT(iLastItemIndex > 0 && "Invalid Item Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	
	
	for(int i = 0; i < iLastItemIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);		
		if(iReadBytes <= 0)		break;										// EOF
		ASSERT(iIndex != -1	&& "Invalid Item Index");
		
		CItemName& ID		= apItemName[iIndex];
		_ItemStat& ItemData = ID.Item_Data;
		ItemData.index		= iIndex;
		
		LOADSTR(ItemData.name);
		LOADSTR(ItemData.descr);
	}
	fclose(fp);
	//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iLastItemIndex;
}





static float fN = pow( 1.06, 12 );
int nLordItmeIndex[CItemData::LORD_ITEM_TOTAL] = { 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 1070,1071};

int	CItemData::GetPhysicalAttack()
{	
	if( !IsFlag( ITEM_FLAG_LORD ) ) return Item_Data.pAttack;
	int nLordLevel = _pNetwork->MyCharacterInfo.level;
	if(!IsFlag( PLATINUM_MAX_PLUS )) nLordLevel = nLordLevel > 15 ? 15 : nLordLevel;
	
	
	float fAttack = 0.0f;

	float	fIncrease = 0.0f;
	int		nBaseAttack = 0;
	
	switch( Item_Data.index )
	{
	case 861:	// 대검
		fIncrease = 3.0f;
		nBaseAttack = 20;
		break;
	case 862:	// 도끼
		fIncrease = 3.3f;
		nBaseAttack = 23;
		break;
	case 863:	// 한손검  
		fIncrease = 2.4f;
		nBaseAttack = 16;
		break;
	case 864:	// 이도류
		fIncrease = 2.8f;
		nBaseAttack = 19;
		break;
	case 865:	// 활
		fIncrease = 2.5f;
		nBaseAttack = 14;
		break;
	case 869:	// 단검
		fIncrease = 2.9f;
		nBaseAttack = 10;
		break;
	case 970:	// 석궁
		fIncrease = 2.5f;
		nBaseAttack = 10;
		break;

	}

	// 성주의 무기인 경우에는 성주에 레벨을 기초로 공격력을 계산
	fAttack = ( nBaseAttack + ( nLordLevel - 1 ) * fIncrease ) * fN;

	return fAttack;	
}

int	CItemData::GetMagicAttack()
{	
	if( !IsFlag( ITEM_FLAG_LORD ) ) return Item_Data.mAttack;
		
	int nLordLevel = _pNetwork->MyCharacterInfo.level;

	if(!IsFlag( PLATINUM_MAX_PLUS )) nLordLevel = nLordLevel > 15 ? 15 : nLordLevel;
	
	float fAttack = 0;

	float	fIncrease = 0.0f;
	int		nBaseAttack = 0;

	switch( Item_Data.index )
	{
	case 866:	// 완드(힐러)
		fIncrease = 1.9f;
		nBaseAttack = 10;
		break;
	case 867:	// 완드(메이지)
		fIncrease = 3.5f;
		nBaseAttack = 10;
		break;
	case 868:	// 스테프
		fIncrease = 3.0f;
		nBaseAttack = 10;
		break;
	case 1070:  // 사이드
		fIncrease = 2.9f;
		nBaseAttack = 20;
		break;
	case 1071:  // 폴암
		fIncrease = 2.5f;
		nBaseAttack = 18;
		break;
	}
	
	// 성주의 무기인 경우에는 성주에 레벨을 기초로 공격력을 계산
	fAttack = ( nBaseAttack + ( nLordLevel - 1 ) * fIncrease ) *fN;

	return fAttack;	
	
}

// 성주 무기인지 아닌지 판다
bool CItemData::IsLordItem( int nIndex )
{
	for( int i = 0; i < CItemData::LORD_ITEM_TOTAL; i++ )
	{
		if( nLordItmeIndex[i] == nIndex )
		{
			return true;
		}
	}
	return false;
}

// 성주 무기인지 아닌지 판다
bool CItemData::IsUniqueItem( int nIndex )
{
	if(nIndex == 887) return true;
	if(nIndex == 1280) return true;
	if(nIndex >= 907 && nIndex <= 947) return true;
	return false;
}







CItemRareOption::CItemRareOption()
{
	//ZeroMemory(this, 0);
	memset(this, 0 , sizeof(CItemRareOption));
	//m_iIndex = 0;
	m_iGrade = -1;
	//memset(m_strPrefix, 0 , 50);
}

CItemRareOption::~CItemRareOption()
{
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 레어 옵션을 파일로부터 읽어들입니다.
//-----------------------------------------------------------------------------
int CItemRareOption::LoadItemRareOptionFromFile(std::vector<CItemRareOption> &apItemRareOption, const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}
	
	int iCount	= 0;
	int iReadBytes		= 0;
	
	iReadBytes = fread(&iCount, sizeof(int), 1, fp);				// 데이터의 개수
	apItemRareOption.reserve(iCount+1);
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	
	
	//0번은 기본 객체 삽입. 실제로 사용되지는 않음. 
	//vector의 객체 인덱스와 옵션 인덱스를 일치시키기 위해.....
	apItemRareOption.push_back( CItemRareOption() );
	for(int i = 0; i < iCount; ++i)
	{
		CItemRareOption temp;
		
		LOADINT(temp.m_iIndex);
		//if(iReadBytes <= 0)		break;										// EOF
		//ASSERT(iIndex != -1	&& "Invalid Item Index");
		
		LOADINT(temp.m_iGrade);
		LOADINT(temp.m_iType);
		LOADSTR(temp.m_strPrefix);
		LOADINT(temp.m_iPhysical_attack);
		LOADINT(temp.m_iPhysical_defence);
		LOADINT(temp.m_iMagic_attack);
		LOADINT(temp.m_iMagic_defence);
		for(int j=0; j<10; ++j)
		{
			LOADINT(temp.m_Option[j].index);
			LOADINT(temp.m_Option[j].level);
		}

		apItemRareOption.push_back(temp);
	}
	fclose(fp);
	//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iCount;
}

