// ----------------------------------------------------------------------------
//  File : UISkillLearn.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISKILLLEARN_H_
#define	UISKILLLEARN_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE


// Define skill slot
#define	SLEARN_SLOT_SX					18
#define	SLEARN_SLOT_SY					56
#define	SLEARN_SLOT_OFFSETY				37
#define	SLEARN_SLOT_ROW					4
#define	SLEARN_SLOT_ROW_TOTAL			30

// Define size of skill learn
#define	SKILLLEARN_HEIGHT				400

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#define	SKILLLEARN_NEW_WIDTH			256
#define	SKILLLEARN_NEW_HEIGHT			341

#define  SKILLINFO_MAX_WIDTH  300
#define	 SKILLINFO_MIN_WIDTH  150

enum eSkillSatisfiedList	//스킬을 배울때 각 상황의 불만족을 체크하는 플래그
{
	ALL_SATISFIED =				0X00000000,
	NOT_SATISFIED_LEVEL =		0x00000001, 
	NOT_SATISFIED_SP =			0X00000002,
	NOT_SATISFIED_STR =			0X00000004,
	NOT_SATISFIED_DEX =			0X00000008,
	NOT_SATISFIED_INT =			0X00000010,
	NOT_SATISFIED_CON =			0X00000020,
	NOT_SATISFIED_SKILL_0 =		0X00000040,	// 필요한 스킬은 최대 3개로 제한 되어 있다.
	NOT_SATISFIED_SKILL_1 =		0X00000080,
	NOT_SATISFIED_SKILL_2 =		0X00000100,
	NOT_SATISFIED_ITEM_0 =		0X00000200, // 필요한 아이템은 최대 3개로 제한되어 있다.
	NOT_SATISFIED_ITEM_1 =		0X00000400,
	NOT_SATISFIED_ITEM_2 =		0X00000800,
};

enum eBtnType
{
	BTN_SKILL_ITEM = 0,
	BTN_SKILL_ACTIVE,
	BTN_SKILL_PASSIVE,
	BTN_SKILL_SPECIAL,
	BTN_SKILL_VOUCHER,
	BTN_ACTION_NORMAL,
	BTN_ACTION_SOCIAL,
	BTN_ACTION_PARTY,
	BTN_ACTION_GUILD,
};
//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Name : CUISkillLearn
// Desc :
// ----------------------------------------------------------------------------
class CUISkillLearn : public CUIWindow
{
protected:
	// Position of target npc
	FLOAT		m_fNpcX, m_fNpcZ;
	int			m_iMobIdx;
	int			m_iMobVirIdx;
	BOOL		m_bQuest;
	BOOL		m_bSpecial;			// 특수 스킬 NPC?
	int			m_iSelChangeJob;	// 원하는 전직.

public:
	CUISkillLearn();
	~CUISkillLearn();

	// Create
	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	// Open skill learn
	ENGINE_API void	OpenSkillLearn( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	// calling function modified. [10/13/2011 rumist]
	void PriorityOpen(int iIndex, BOOL bHasQuest, BOOL bUseCard=FALSE);
	// Command functions
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void LearnSkill( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel, BOOL bSpecial = FALSE );
	void LearnSkillError( UBYTE ubError );
	void LearnSSkillError( UBYTE ubError );
	int IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial );
};


#endif	// UISKILLLEARN_H_

