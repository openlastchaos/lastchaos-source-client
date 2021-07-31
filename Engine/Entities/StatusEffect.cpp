#include "stdh.h"

#include "StatusEffect.h"
#include <Engine/Base/Timer.h>

/*
//skill.h에서 정의.
#define EST_ASSIST_POISON		0			// 독
#define EST_ASSIST_HOLD			1			// 홀드
#define EST_ASSIST_CONFUSION	2			// 혼란
#define EST_ASSIST_STONE		3			// 석화
#define EST_ASSIST_SILENT		4			// 봉인
#define EST_ASSIST_BLOOD		5			// 출혈
#define EST_ASSIST_BLIND		6			// 블라인드
#define EST_ASSIST_STURN		7			// 스턴
#define EST_ASSIST_SLEEP		8			// 슬립
#define EST_ASSIST_HP			9			// HP회복
#define EST_ASSIST_MP			10			// MP회복
#define EST_ASSIST_MOVESPD		11			// 이속 향상
#define EST_ASSIST_HP_CANCEL	12			// HP변화(0, MAX에서 취소)
#define EST_ASSIST_MP_CANCEL	13			// MP변화(0, MAX에서 취소)
#define EST_ASSIST_DIZZY		14			// 현기증
#define EST_ASSIST_INVISIBLE	15			// 인비저블
#define EST_ASSIST_SLOTH		16			// 슬로스 : 공속하락
#define EST_ASSIST_FEAR			17			// 공포 : 해당 NPC가 도망
#define EST_ASSIST_FAKEDEATH	18			// 죽은척
#define EST_ASSIST_PERFECTBODY	19			// 타이탄 퍼펙 바디
#define EST_ASSIST_FRENZY		20			// 타이탄 프렌지
#define EST_ASSIST_DAMAGELINK	21			// 데미지링크
#define EST_ASSIST_BERSERK		22			// 그랜드 몬스터의 분노- 공속
#define EST_ASSIST_DESPAIR		23			// 힐러 디세이어 마크
#define EST_ASSIST_MANA_SCREEN	24			// 소서러 마나 스크린
#define EST_ASSIST_BLESS		25			// 소서러 윈즈 블레싱
#define EST_ASSIST_SAFEGUARD	26			// 보호망(전투불가능)
#define EST_ASSIST_MANTLE		27			// 망토
#define EST_ASSIST_GUARD		28			// 경비병 소환
#define EST_ASSIST_CRACKER		29			// 침입자 이펙트 관련(상태 이상과는 무관:서버와 상관없이 강제 적용)
#define EST_ASSIST_COUNT		30	
*/
const char	*CStatusEffect::m_szStatusEffectName[EST_ASSIST_COUNT][T_COUNT] = 
{
	{NULL, NULL, NULL}//"STATE_POISON"
	, {NULL, NULL, NULL}//"STATE_HOLD"
	, {NULL, NULL, NULL}//"STATE_CONFUSION"
	, {"STATE_STONE", "STATE_STONE", "STATE_STONE"}
	, {"STATE_SILENT", "STATE_SILENT", "STATE_SILENT"}
	, {NULL, NULL, NULL}//"STATE_BLOOD"
	, {"STATE_BLIND", NULL, NULL}
	, {"STATE_STUN", "STATE_STUN", "STATE_STUN"}
	, {"STATE_SLEEP","STATE_SLEEP","STATE_SLEEP"}//"STATE_SLEEP"
	, {NULL, NULL, NULL}//"STATE_HP"
	, {NULL, NULL, NULL}//"STATE_MP"
	, {NULL, NULL, NULL}//"STATE_MOVESPD"
	, {NULL, NULL, NULL}//"HP_CANCEL"
	, {NULL, NULL, NULL}//"MP_CANCEL"
	, {"STATE_DIZZY", "STATE_DIZZY", "STATE_DIZZY"}
	, {NULL, NULL, NULL}//EST_ASSIST_INVISIBLE
	, {NULL, NULL, NULL}//EST_ASSIST_SLOTH
	, {NULL, NULL, NULL}//EST_ASSIST_FEAR
	, {NULL, NULL, NULL}//EST_ASSIST_FAKEDEATH
	, {"STATE_PERFECTBODY", "STATE_PERFECTBODY", "STATE_PERFECTBODY"}
	, {"STATE_FRENZY", "STATE_FRENZY", "STATE_FRENZY"}
	, {"STATE_DAMAGELINK", "STATE_DAMAGELINK", "STATE_DAMAGELINK"}
	, {NULL, NULL, NULL}//EST_ASSIST_BERSERK
	, {NULL, NULL, NULL}//EST_ASSIST_DESPAIR
	, {NULL, NULL, NULL}//EST_ASSIST_MANA_SCREEN
	, {NULL, NULL, NULL}//EST_ASSIST_BLESS
	, {NULL, NULL, NULL}//EST_ASSIST_SAFEGUARD
	, {NULL, NULL, NULL}//EST_ASSIST_MANTLE
	, {NULL, NULL, NULL}//EST_ASSIST_CRACKER (일반 서버에서 사용 안함)
};

const char	*CStatusEffect::m_szNPCQuestMark[CQuestSystem::NQT_COUNT] =
{
	NULL,
	"QMARK_HAVE_QUEST",
	"QMARK_CAN_PRIZE",
};


CStatusEffect::CStatusEffect()
: m_slCurrentStatus( 0 )
, m_eType( T_NPC )
, m_pNPCQuestMark( NULL )
, m_eCurrentNPCQuestType( CQuestSystem::NQT_NONE )
{
	ASSERT(EST_ASSIST_COUNT == 30);//뭔가 바뀌게 있으면 m_szStatusEffectName의 값을 고쳐라-_-
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);
}

CStatusEffect::~CStatusEffect()
{
	Reset();
}

CStatusEffect::CStatusEffect(const CStatusEffect &other)
{
	m_slCurrentStatus = other.m_slCurrentStatus;
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	m_pNPCQuestMark = NULL;
}

CStatusEffect &CStatusEffect::operator=(const CStatusEffect &other)
{
	m_slCurrentStatus = other.m_slCurrentStatus;
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	m_pNPCQuestMark = NULL;
	return *this;
}

void CStatusEffect::Reset()
{
	m_slCurrentStatus = 0;
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	m_eCurrentNPCQuestType = CQuestSystem::NQT_NONE;
	if(m_pNPCQuestMark
	&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark))
	{
		m_pNPCQuestMark->Stop(0.5f);
	}
	m_pNPCQuestMark = NULL;
}

void CStatusEffect::Refresh(CTagManager *pTM, eRelation relation)
{
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	if(m_pNPCQuestMark
	&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark))
	{
		m_pNPCQuestMark->Stop(0.5f);
	}
	if(pTM != NULL)
	{
		ChangeStatus(pTM, m_slCurrentStatus, relation);
		ChangeNPCQuestMark(pTM, m_eCurrentNPCQuestType);
	}
}

#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & ((1L)<<(flagVal)) )

void CStatusEffect::ChangeStatus(CTagManager *pTM, SLONG newStatus, eRelation relation)
{
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		if(m_szStatusEffectName[i][m_eType] == NULL) continue;
		if(i == EST_ASSIST_DAMAGELINK && relation != R_PARTY)  continue;

		//Off -> On, Create Effect
		if( !IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i) )
		{
			ASSERT(m_pStatusEffectArray[i] == NULL);
			m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
		}
		//On -> Off, Destroy Effect
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && !IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] != NULL
			&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
			{
				m_pStatusEffectArray[i]->Stop(0.1f);
			}
			m_pStatusEffectArray[i] = NULL;
		}
		//On -> On, Effect Life Time Refresh
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] == NULL
				|| (
					m_pStatusEffectArray[i] != NULL
					&& !CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i])
				) )
			{
				m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
			}
		}
	}
	m_slCurrentStatus = newStatus;
}

BOOL CStatusEffect::IsState(INDEX state)
{
	if(IS_FLAG_ON(m_slCurrentStatus, state)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsSturn()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_STURN)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsConfuse()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_CONFUSION)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsHold()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_HOLD)) return TRUE;
	else return FALSE;
}

void CStatusEffect::ChangeNPCQuestMark(CTagManager *pTM, CQuestSystem::eNpcQuestType newStatus)
{
	m_eCurrentNPCQuestType = newStatus;
	if(m_pNPCQuestMark
	&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark))
	{
		m_pNPCQuestMark->Stop(0.1f);
	}
	if(m_szNPCQuestMark[newStatus] != NULL)
	{
		m_pNPCQuestMark = StartEffectGroup(m_szNPCQuestMark[newStatus], pTM, _pTimer->GetLerpedCurrentTick());
	}
}
