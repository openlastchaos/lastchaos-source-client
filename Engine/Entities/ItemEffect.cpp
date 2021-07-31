#include "StdH.h"

#include "ItemEffect.h"

#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Base/Timer.h>

extern UINT g_uiEngineVersion;

CItemEffect::CItemEffect() : m_pAccessroyEffect(NULL)
{
	Clear();
}

CItemEffect::~CItemEffect()
{
	Clear();
}

CItemEffect::CItemEffect(const CItemEffect &other)
{
	m_PlusEffectGourp = other.m_PlusEffectGourp;

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		m_ItemEffectGroup[i] = other.m_ItemEffectGroup[i];
	}

	memcpy(m_aItemPlus, other.m_aItemPlus, sizeof(int)*WEAR_TOTAL);
}

CItemEffect &CItemEffect::operator=(const CItemEffect &other)
{
	m_PlusEffectGourp = other.m_PlusEffectGourp;

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		m_ItemEffectGroup[i] = other.m_ItemEffectGroup[i];
	}

	memcpy(m_aItemPlus, other.m_aItemPlus, sizeof(int)*WEAR_TOTAL);
	return *this;
}

void CItemEffect::Clear()
{
	memset(m_PlusEffectGourp.szEffectName, 0, sizeof(m_PlusEffectGourp.szEffectName));
	DestroyEffectGroupIfValid(m_PlusEffectGourp.m_pEffectGroup);

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		m_aItemPlus[i] = -1;
		memset(m_ItemEffectGroup[i].szEffectName, 0, sizeof(m_ItemEffectGroup[i].szEffectName));
		DestroyEffectGroupIfValid(m_ItemEffectGroup[i].m_pEffectGroup);
	}
}

void CItemEffect::Refresh(CTagManager *pTM, SBYTE sbOption)
{
	if( !(sbOption & 0x01) )
		pTM = NULL;

	DestroyEffectGroupIfValid(m_PlusEffectGourp.m_pEffectGroup);

	if (m_PlusEffectGourp.szEffectName[0] != 0)
		m_PlusEffectGourp.m_pEffectGroup = StartEffectGroup(m_PlusEffectGourp.szEffectName, pTM, _pTimer->GetLerpedCurrentTick());
	
	RefreshAccessoryEffect(pTM);

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		DestroyEffectGroupIfValid(m_ItemEffectGroup[i].m_pEffectGroup);

		if (m_ItemEffectGroup[i].szEffectName[0] != 0)
		{
			m_ItemEffectGroup[i].m_pEffectGroup = StartEffectGroup(m_ItemEffectGroup[i].szEffectName,
																	pTM, _pTimer->GetLerpedCurrentTick());
		}
	}
}

void CItemEffect::DeleteEffect(const int wearPos)
{
	if (wearPos < WEAR_HELMET || wearPos > WEAR_ACCESSORY4) return;

	DestroyEffectGroupIfValid(m_ItemEffectGroup[wearPos].m_pEffectGroup);
}

//=========================================================================================
// CItemEffect::AddAccessoryEffect()
// Explain: 특정 악세사리 착용시 Effect적용( case MSG_EX_PLAYER_STATE_CHANGE: )
//=========================================================================================
BOOL CItemEffect::AddAccessoryEffect(const char* strEffectName, CTagManager *pTM)
{
	if ( !strEffectName || !pTM ) return FALSE;

	// Refresh
	DelAccessoryEffect(); // reset
	strcpy(m_szAccessoryEffectName, strEffectName); // add str
	m_pAccessroyEffect = StartEffectGroup(m_szAccessoryEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect

	return TRUE;
}
//=========================================================================================
// CItemEffect::RefreshAccessoryEffect()
// Explain: 특정 악세사리 착용시 적용된 Effect 재설정(모델이 변경사항이 있을 시 Effect가 떨어지지않게 재설정)
//=========================================================================================
void CItemEffect::RefreshAccessoryEffect(CTagManager *pTM)
{
	DestroyEffectGroupIfValid(m_pAccessroyEffect);

	if (m_szAccessoryEffectName != 0)
		m_pAccessroyEffect = StartEffectGroup(m_szAccessoryEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect
}

BOOL CItemEffect::AddWearEffect(const char* strEffectName, const int wearPos, CTagManager *pTM)
{
	if ( !strEffectName || !pTM || wearPos < WEAR_HELMET || wearPos > WEAR_ACCESSORY4)
		return FALSE;

	DelWearEffect(wearPos); // reset
	strcpy(m_ItemEffectGroup[wearPos].szEffectName, strEffectName); // add EffectName
	m_ItemEffectGroup[wearPos].m_pEffectGroup = StartEffectGroup(m_ItemEffectGroup[wearPos].szEffectName,
																	pTM, _pTimer->GetLerpedCurrentTick()); // add effectgroup

	return TRUE;
}

BOOL CItemEffect::DelWearEffect(const int wearPos)
{
	if (wearPos < WEAR_HELMET || wearPos > WEAR_ACCESSORY4)
		return FALSE;

	memset(m_ItemEffectGroup[wearPos].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPos].szEffectName));
	DestroyEffectGroupIfValid(m_ItemEffectGroup[wearPos].m_pEffectGroup);
	return TRUE;
}

//=========================================================================================
// CItemEffect::DelAccessoryEffect()
// Explain: 특정 악세사리 탈착시 Effect제거( case MSG_EX_PLAYER_STATE_CHANGE: )
//=========================================================================================
BOOL CItemEffect::DelAccessoryEffect(void)
{
	if ( m_pAccessroyEffect )
	{ 
		memset(m_szAccessoryEffectName, 0, sizeof(m_szAccessoryEffectName));
		DestroyEffectGroupIfValid(m_pAccessroyEffect);
		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------------------
// CItemEffect::Change
// Explain: 
// Histroy 
//	- Date : 2005-04-07,Author: Lee Ki-hwan
//			2차 무기 아이템 처리 
//			int nItemSubType : 2차 무기 판단 GetSubTypeWeapon ( int nItemSubType ) 에 쓰인다.
//		
//------------------------------------------------------------------------------------------
void CItemEffect::Change(const int job, CItemData* ItemData, const int wearPosNew, const int itemPlusNewIn, CTagManager *pTM, SBYTE sbOption, int nItemSubType )
{
	//초기조건 검사
	if(job < 0 || job > TOTAL_JOB) return;
	if(wearPosNew < 0 || wearPosNew > WEAR_TOTAL) return;
	if(pTM == NULL) return;

	BOOL bRefresh = FALSE;

	//아이템 정보갱신
	int itemPlusNew = itemPlusNewIn < 0 ? -1 : itemPlusNewIn;
	m_aItemPlus[wearPosNew] = itemPlusNew;

	//무기 아니면 갑옷?
	char *pStrName = NULL;
	CTString strType;

	if(wearPosNew == WEAR_WEAPON)
	{
		// Date : 2005-04-07(오후 3:16:08), By Lee Ki-hwan
		int nNumWeapon = GetSubTypeWeapon ( nItemSubType ); 
		
		strType = "wp";
	
		// Date : 2005-04-07(오후 2:57:21), By Lee Ki-hwan
		if ( nNumWeapon > 1 )
		{
			strType.PrintF( "%s%d", "wp", nNumWeapon );	
		}

		pStrName = &m_ItemEffectGroup[wearPosNew].szEffectName[0];
		bRefresh = TRUE;
	}
	else if(wearPosNew == WEAR_JACKET || wearPosNew == WEAR_PANTS
		|| wearPosNew == WEAR_BOOTS || wearPosNew == WEAR_GLOVES)
	{
		int itemPlusMin = -1;
#		define MIN(A, B) ((A) < (B) ? (A) : (B))
		itemPlusMin = MIN( MIN(m_aItemPlus[WEAR_JACKET], m_aItemPlus[WEAR_PANTS])
						 , MIN(m_aItemPlus[WEAR_BOOTS], m_aItemPlus[WEAR_GLOVES]) );
#		undef MIN
		itemPlusNew = itemPlusMin; 
		strType = "am";
		pStrName = &m_PlusEffectGourp.szEffectName[0];
		bRefresh = TRUE;

		if (strlen(ItemData->GetArmorEffectName()) > 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
		}
	}
#ifdef HEAD_CHANGE
	else if(wearPosNew == WEAR_SHIELD || wearPosNew == WEAR_HELMET)
	{	
		if (strlen(ItemData->GetArmorEffectName()) > 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
		}
		bRefresh = TRUE;
	}
#else
	else if(wearPosNew == WEAR_SHIELD)
	{	
		bRefresh = TRUE;
	}
#endif
	else if(wearPosNew >= WEAR_ACCESSORY1 && wearPosNew < WEAR_ACCESSORY4)
	{
		if (strlen(ItemData->GetArmorEffectName()) > 0 && itemPlusNew >= 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
		}else if (itemPlusNew < 0)
		{
			m_ItemEffectGroup[wearPosNew].szEffectName[0] = 0;
		}

		bRefresh = TRUE;
	}

	if(strType.str_String == NULL || pStrName == NULL)
	{
		if(bRefresh) Refresh(pTM, sbOption);
		return;
	}

	//plus수치에 따른 이펙트 단계
	char *szLevel = 0;
	//직업 종류
	char *szJobName = NULL;

	// 성주 무기인 경우에는 6단계 레벨의 이펙트를 무조건 적용한다.
	if((CItemData::IsLordItem( ItemData->GetItemIndex() )||CItemData::IsUniqueItem(ItemData->GetItemIndex())) && itemPlusNew != -1)
	{
		szLevel = "6";
	}
	else 
	{
		switch(itemPlusNew)
		{
		case -1: case 0: case 1: case 2: case 3://음수값일 경우 장착 해제한 것임.
			pStrName[0] = 0;
			Refresh(pTM, sbOption);
			return;
		case 4: case 5: szLevel = "1";	break;
		case 6: case 7: szLevel = "2";	break;
		case 8: case 9: szLevel = "3";	break;
		case 10:case 11:szLevel = "4";	break;
		case 12:case 13:case 14: szLevel = "5";	break;
		default:		szLevel = "6";	break;
		}
		//if(szLevel == NULL) return;//필요없음.
	}

	static char *szEffectNameFormat = "i%s_%s_%s";

	switch(job)
	{
	case TITAN:		szJobName = "a_t";	strcpy( szEffectNameFormat, "i%s_%s_%s" );break;
	case MAGE:		szJobName = "_ma";	strcpy( szEffectNameFormat, "i%s_%s_%s" );break;
	case HEALER:	szJobName = "_hw";	strcpy( szEffectNameFormat, "i%s_%s_%s" );break;
	case KNIGHT:	szJobName = "a_k";	strcpy( szEffectNameFormat, "i%s_%s_%s" );break;
	case ROGUE:		szJobName = "h_ro";	strcpy( szEffectNameFormat, "i%s_%s_%s" );break;
	case SORCERER:	szJobName = "so";	strcpy( szEffectNameFormat, "%s_%s_%s" );  break;
	default:		ASSERTALWAYS("Unknown Character Type"); return;
	}
	//if(szJobName == NULL) return;//필요없음.

	//이펙트 이름 조합 후 이펙트 시작.
	sprintf(pStrName, szEffectNameFormat, szJobName, strType, szLevel);
	Refresh(pTM, sbOption);
}


//------------------------------------------------------------------------------
// GetSubTypeWeapon
// Explain:  아이템의 서브 타입을 입력 받아 차수 를 리턴하는 함수
// Date : 2005-04-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CItemEffect::GetSubTypeWeapon ( int nSubType )
{
	// 2차 무기 판단
	if ( nSubType == CItemData::ITEM_WEAPON_CROSSBOW	// 석궁(로그)
	||	nSubType == CItemData::ITEM_WEAPON_AXE			// 도끼(타이탄)
	||	nSubType == CItemData::ITEM_WEAPON_WAND			// 완드(힐러)
	||	nSubType == CItemData::ITEM_WEAPON_TWOSWORD		// 이도류(나이트)
	||	nSubType == CItemData::ITEM_WEAPON_STAFF		// 스태프(메이지)
	||	nSubType == CItemData::ITEM_WEAPON_SCYTHE		// 사이더(소서러)
	) return 2;
	else return 1; // 현재 2차 무기 이외의 것들은 1차 무기로 판단
	
	return -1;
}