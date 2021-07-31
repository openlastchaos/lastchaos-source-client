#ifndef	__WILDPET_INFO_UI_H__
#define	__WILDPET_INFO_UI_H__

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIText;
class CUIButton;
class CUICheckButton;
class CUITabPage;
class CUIImageArray;
class CUIIcon;
class CUIList;
class CUITextBox;
class CUITab;

#define WILDPET_AI_MAXSLOT			6

struct sWildPet_AI_Slot 
{
	BOOL	m_bActive;		// AI 슬롯 활성 상태
	INDEX	m_nSillType;	// 스킬 타입 Skill = 0, Action = 1
	INDEX	m_nTargetSlot;	// 타겟 아이템 인덱스 저장
	INDEX	m_nActionSlot;	// 행동 스킬 인덱스 저장
};

enum eAI_TYPE
{
	AI_NONE = -1,
	AI_TARGET_ENEMY = 0,
	AI_TARGET_OWNER,
	AI_TARGET_PET,
	AI_TARGET_ITEM,
};

enum eWILDPET_TYPE
{
	eWILDPET_HUMAN = 1, // 인간형
	eWILDPET_ANIMAL = 2,	// 동물형
	eWILDPET_DEMON = 3	// 데몬형
};

class CWildPetInfoUI : public CUIWindow
{
public:
	CWildPetInfoUI();
	~CWildPetInfoUI();

	// skill 관련
	ENGINE_API BOOL IsLearnSkill( int nSkillIndex );
	ENGINE_API BOOL IsUsableSkill( LONG nSkillIndex );
	void UseSkill( int nIndex );
	void AddSkill( int nSkillIndex, SBYTE sbSkillLevel );
	void SkillClear();	

	// inven 관련
	void PetWearItemReSet();
	void WearItemDBL(int nWearPos);
	
	// network
	void ReceiveWearItemData(CNetworkMessage *istr);
	void ReceiveDeleteEquip(CNetworkMessage *istr);

	void SendAIList();
	void SendAIActive(); // AI 활성화 끄고 켜기
	void SendWildPetPointUP(UBYTE ubPoint);
	void SendUpgradeReq();

	// PetInfo data
	void AddWildPetInfo( sPetItem_Info sPetInfo );
	BOOL GetWildPetInfo( int nPetIndex, sPetItem_Info& WildPetInfo );

	// AI 관련
	inline void SetPetAISlotCount(int nSlotcount)	{ m_nAISlotCount = nSlotcount; updateAISetUI();	}
	inline int GetPetAISlotCount()					{ return m_nAISlotCount;		}

	inline void	SetPetAIActive(BOOL bActive)		{ m_bAIActive = bActive;		}
	inline BOOL GetAIActive()						{ return m_bAIActive;			}	

	sWildPet_AI_Slot* GetPetAI()					{ return m_sAISlot;				}
	void RemoveSlot(CUIIcon* pIcon);
	void AddAIData(int nSlotnum, sWildPet_AI_Slot sAISlotdata);
	void AIClear();
	void OpenAISetUI();
	void CloseAISetUI();
	void PetAIReSet();

	// exp Pet
	void ExpUse();

	// error
	void ErrorMessage(UBYTE errorcode);

	// UI 업데이트 
	void updateUI();

	// Base Info Tab 
	void updateBaseTab();

	// skill Tab
	void updateSkillTab();

	// AI set UI update
	void updateAISetUI();

	void changeMainTab();
	void updateStat();
	void updateExpInfo();

	void ToggleVisible();
	void openUI();
	void closeUI();

	void initialize();

	void OnUpdate(float fDeltaTime, ULONG ElapsedTime);

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	BOOL CloseWindowByEsc() { closeUI();	return TRUE; }

	void clear_upgradeinfo();
	void DeleteEquip(ULONG idx, INDEX item_idx = -1);

protected:
	typedef std::vector<SLONG> vec_skill;
	typedef std::map<SLONG, sPetItem_Info> WildPetInfo_map;

private:
	void _initBaseInfoTab();
	void _initSkillTab();
	void _initAISetUI();
	void _setUI();
	void _checkStatUpBtnEnable();
	void _updateSkillIconCoolTime();
	void _updateAISkillIconCoolTime();
	void _updateExpPetCooltime();

	BOOL CheckAIItem(CItemData* pAIItem, int nSlot);
	BOOL CheckAIAction(CUIIcon* pIcon, int nSlot);

	BOOL CheckAIType(int nAIType, int nActionType);
	int	 GetActionAIType(CUIIcon* pIcon);
	BOOL IsWildPetCoolTime();

	bool check_upgrade(int idx);

	// 스트링 정보
	enum eSTRING_TYPE
	{
		eSTRING_NAME = 0,
		eSTRING_LEVEL,
		eSTRING_SPECIES,	// 종족
		eSTRING_EXP,
		eSTRING_STAT_POINT,	// 성장 포인트
		eSTRING_ATT,
		eSTRING_MAGIC_ATT,
		eSTRING_DEF,
		eSTRING_MAGIC_DEF,
		eSTRING_ACCURACY,	// 명중도
		eSTRING_DODGE,		// 회피도
		eSTRING_MAX
	};

	CUIText*	m_pText[eSTRING_MAX];

	// 스텟
	enum eSTAT_TYPE
	{
		eSTAT_STR = 0,
		eSTAT_CON,
		eSTAT_DEX,
		eSTAT_INT,
		eSTAT_MAX
	};

	CUIText*	m_pStatText[eSTAT_MAX];
	CUIButton*	m_pBtnStat[eSTAT_MAX];
		
	enum eBUTTON_TYPE
	{
		eBTN_CLOSE = 0, // 닫기
		eBTN_AI_SET,	// AI 셋팅창 켜기,
		eBTN_MAX
	};

	CUIButton*	m_pBtn[eBTN_MAX];

	// 탭버튼
	enum eTAB_TYPE
	{
		eTAB_BASE = 0,
		eTAB_SKILL,
		eTAB_MAX
	};

	CUICheckButton* m_pChkTabBtn[eTAB_MAX];
	CUIText*		m_pTextChk[eTAB_MAX];

	// 스테이트
	enum eSTATE_TYPE
	{
		eSTATE_HUNGRY = 0,	// 배고픔
		eSTATE_FAITH,		// 충성도
		eSTATE_MAX
	};

	CUIImageArray*	m_pImgArr[eSTATE_MAX];
	CUIText*		m_pTextState[eSTATE_MAX];

	// 경험치 펫 관련
	enum eEXP_STRING_TYPE
	{
		eEXP_STRING_PERCENT = 0,
		eEXP_STRING_MAX_EXP,
		eEXP_STRING_CURRENT_EXP,
		eEXP_STRING_PET_COOLTIME_TITLE,
		eEXP_STRING_PET_COOLTIME,
		eEXP_STRING_MAX
	};

	CUIText*		m_pTextExp[eEXP_STRING_MAX];
	CUIImage*		m_pImgExpBack;
	CUIButton*		m_pBtnExpUse;

	// drag
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;
	CUIImage*	m_pMoveArea;

	// main
	CUIImage*	m_pImgBack;
	CWildPetData*	m_Petdata;
	WildPetInfo_map	m_mapWildPetInfo;
	CUITab*		m_pMainTab;

	// inven
	CUIIcon*	m_pIcon[WILDPET_WEAR_TOTAL]; // 착용 장비
	CUIImageArray* m_pInvenBack;

	// skill
	CUIList*	m_pListSkill;
	vec_skill	m_vecSkills;	// Buttons of Skill

	// SetAIUI
	CUIImage*	m_pImgAISetUIBack;
	CUIList*	m_pListAI;
	CUIButton*	m_pBtnAISetOK;
	CUICheckButton* m_pChkAISet;
	CUICheckButton*	m_pchkAISlotActive[WILDPET_AI_MAXSLOT];
	CUIIcon*		m_pIconAITarget[WILDPET_AI_MAXSLOT];
	CUIIcon*		m_pIconAIAction[WILDPET_AI_MAXSLOT];

	int			m_nAISlotCount;	
	BOOL		m_bAIActive;
	BOOL		m_bSlideUI;

	sWildPet_AI_Slot m_sAISlot[WILDPET_AI_MAXSLOT];

	int			m_nUpgradeSelidx;
	int			m_nUpgradeTab;
	int			m_nUpgradeInvenIdx;

};

#endif	//	__WILDPET_INFO_UI_H__