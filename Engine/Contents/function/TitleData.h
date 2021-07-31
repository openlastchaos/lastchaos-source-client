#ifndef		__TITLE_DATA_H__
#define		__TITLE_DATA_H__

#include <Engine/LocalDefine.h>
#include <Engine/Help/LoadLod.h>
#include <common/header/def_title.h>
#include <common/Packet/ptype_old_do_title.h>
#include <vector>
#include <Engine/Util.h>

#define DEF_CUSTOMTITLE_MAKE_ITEM 11036
#define DEF_DUMMY_TITLE_INDEX 199

struct stCustomTitleStatForPc
{
	int nFrontColor;
	int nBackColor;
	int nEffectIdx;
	char name[CUSTOM_TITLE_NAME_LENGTH];

	void Init()
	{
		nFrontColor = -1;
		nBackColor = -1;
		nEffectIdx = -1;
		memset(name, '\0', CUSTOM_TITLE_NAME_LENGTH);
	}
};

class TitleNetwork
{
public:
	TitleNetwork();

	void ResetTitleSelectInfo();

	void setSelectFCIndex(int nSelIdx) { m_nSelectFrontColorIdx = nSelIdx; }
	void setSelectBCIndex(int nSelIdx) { m_nSelectBackColorIdx = nSelIdx; }
	void setSelectEffectIndex(int nSelIdx) { m_nSelectEffectIdx = nSelIdx; }
	void setCustomTitleName(char* strName) 
	{ 
		strcpy(m_strCustomTitleName, strName);
		m_strCustomTitleName[CUSTOM_TITLE_NAME_LENGTH - 1] = '\0';
	}

	int getSelectFCIndex()		{ return m_nSelectFrontColorIdx;	}
	int getSelectBCIndex()		{ return m_nSelectBackColorIdx;		}
	int getSelectEffectIndex()	{ return m_nSelectEffectIdx;		}
	char* getCustomTitleName()	{ return m_strCustomTitleName;		}

	void SendTitleListReq();
	void SendTitleSelectReq(int nTitleIdx);
	void SendTitleCancelReq(int nTitleIdx);
	void SendTitleDeleteReq(int nTitleIdx);
	void SendTitleMake(int tab, int invenIdx, int virIdx);
	void RecvTitleMessage(CNetworkMessage* istr); // 출석 체크

private:
	int m_nSelectFrontColorIdx;
	int m_nSelectBackColorIdx;
	int m_nSelectEffectIdx;
	char m_strCustomTitleName[CUSTOM_TITLE_NAME_LENGTH];
};

class ENGINE_API TitleStaticData : public stTitle, public LodLoader<TitleStaticData>
{
public:
	static bool loadEx(const char* FileName);

	int			GetItemIndex()			const {	return a_item_index;	}
	COLOR		GetColor()				const {	return a_icolor;		}
	COLOR		GetBGColor()			const {	return a_ibgcolor;		}
	const char*	GetEffectFile()			const {	return a_effect;		}
	const char*	GetAttackEffectFile()	const {	return a_attack;		}
	const char*	GetDamageEffectFile()	const {	return a_damage;		}
	const int*	GetOptionIndex()		const {	return a_option_index;	}
	const BYTE*	GetOptionLevel()		const {	return a_option_level;	}
};

const int nOptionLevelMax = 5;

struct stCustomTitleOption
{
	int nIndex;
	int nLevel[nOptionLevelMax];

	stCustomTitleOption()
	{
		nIndex = -1;

		for (int i = 0; i < nOptionLevelMax; ++i)
		{
			nLevel[i] = -1;
		}
	}
};

struct stCustomTitleString
{
	char strValue[32];

	stCustomTitleString()
	{
		memset(strValue, '\0', sizeof(strValue));
	}
};

class ENGINE_API CustomTitleData
{
public:
	~CustomTitleData();

	static bool LoadCustomTitleDataFromFile(const char* fileName);

	static UINT ColorStringToInt(std::string ColorString);

	static void AddCustomItemInfo(TitleSystemMakeInfo* pInfo);
	static void DeleteCustomItemInfo(int index);
	static bool IsKeepTitleInfo(int index);
	static void clearCustomItemInfo();
	
	static std::vector<UINT> m_vecFrontColor;
	static std::vector<UINT> m_vecBackColor;
	static std::vector<std::string> m_vecCustomTitleEffect;
	static std::map<int, TitleSystemMakeInfo*> m_mapCustomTitleItemInfo;
};

template <typename T>
static void ReleaseForVector(std::vector<T> releseVec)
{
	std::vector<T>::iterator iter;
	std::vector<T>::iterator iter_end;

	iter = releseVec.begin();
	iter_end = releseVec.end();

	for (; iter != iter_end; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	releseVec.clear();
}

#endif		// __TITLE_DATA_H__