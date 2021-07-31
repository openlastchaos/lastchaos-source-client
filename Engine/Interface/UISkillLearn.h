// ----------------------------------------------------------------------------
//  File : UISkillLearn.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISKILLLEARN_H_
#define	UISKILLLEARN_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIButtonEx.h>

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#include <Engine/Interface/UIRectString.h>

// Define skill slot
#define	SLEARN_SLOT_SX					18
#define	SLEARN_SLOT_SY					56
#define	SLEARN_SLOT_OFFSETY				37
#define	SLEARN_SLOT_ROW					4
#define	SLEARN_SLOT_ROW_TOTAL			30


// Define text
#define	SLEARN_NAME_CX					122
#define	SLEARN_NAME_SY					58
#define	SLEARN_NEED_RX					184
#define	SLEARN_CURSP_SX					75 //이기환 수정 (05.01.03) : SP->숙련도 라는 단어 변경 하면서 수치 수정(97->75)
#define	SLEARN_CURSP_RX					199
#define	SLEARN_CURSP_SY					343
#define	SLEARN_DESC_CHAR_WIDTH			170


// Define text position
#define	SLEARN_TITLE_TEXT_OFFSETX		25
#define	SLEARN_TITLE_TEXT_OFFSETY		5


// Define size of skill learn
#define	SKILLLEARN_WIDTH				216
#define	SKILLLEARN_HEIGHT				400
//#define ADJUST_MEMORIZE_SKILL 

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#define	SKILLLEARN_NEW_WIDTH			256
#define	SKILLLEARN_NEW_HEIGHT			341

#define	SKILLINFO_NEW_WIDTH				479
#define	SKILLINFO_NEW_HEIGHT			294

#define	SKILLLEARN_NEW_SLOT_ROW			6
#define	 SKILLINFO_NEW_SLOT_ROW			12
#define	SKILLLEARN_NEW_SLOT_ROW_TOTAL	50
#define	SKILLLEARN_NEW_SLOT_TOTAL		50


#define  SKILLINFO_MAX_WIDTH  300
#define	 SKILLINFO_MIN_WIDTH  150

enum eSkillSatisfiedList	//스킬을 배울때 각 상황의 만족을 체크하는 플래그
{
	ALL_SATISFIED =				0X00000000,
	NOT_SATISFIED_LEVEL =		0x00000001, 
	NOT_SATISFIED_SP =			0X00000010,
	NOT_SATISFIED_STR =			0X00000100,
	NOT_SATISFIED_DEX =			0X00001000,
	NOT_SATISFIED_INT =			0X00010000,
	NOT_SATISFIED_CON =			0X00100000,
	NOT_SATISFIED_SKILL =		0X01000000,
	NOT_SATISFIED_ITEM =		0X10000000,
	NOT_SATISFIED_SKILL_LEVEL = NOT_SATISFIED_ITEM, //특수스킬 전용(스킬은 배웠는데 스킬 레벨이 부족함?)
};


// 스킬 정보 표시창

enum eSkillInfoList
{
	SKILLINFO_CURRENT = 0,
	SKILLINFO_NEXT,
};

enum eBtnType
{
	BTN_SKILL_ACTIVE	= 0,
	BTN_SKILL_PASSIVE,
	BTN_SKILL_SPECIAL,
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
	// Define skill tab
	enum SLEARN_TAB
	{
		SLEARN_TAB_ACTIVE	= 0,
		SLEARN_TAB_PASSIVE,
#ifdef ADJUST_MEMORIZE_SKILL
		SLERAN_TAB_MEMORIZE,
#endif
		SLEARN_TAB_SPECIAL,
		SLEARN_TAB_TOTAL,
	};

	// Controls
	int						m_nCurrentTab;							// Current tab
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnLearn;								// Learn button
	CUIButton				m_btnCancel;							// Cancel button
	CUIScrollBar			m_sbActiveSkillIcon;					// Scrollbar of active skill icon
	CUIScrollBar			m_sbPassiveSkillIcon;					// Scrollbar of passive skill icon
	CUIScrollBar			m_sbMemorizeSkillIcon;					// Scrollbar of memorize skill icon
	CUIScrollBar			m_sbSpecialSkillIcon;					// Scrollbar of special skill icon
	CUIListBox				m_lbSkillDesc;							// List box of skill description

	// Skill buttons
	CUIButtonEx				m_btnActiveSkills[SKILLLEARN_NEW_SLOT_TOTAL];	// Buttons of active skill
	CUIButtonEx				m_btnPassiveSkills[SKILLLEARN_NEW_SLOT_TOTAL];	// Buttons of passive skill
	CUIButtonEx				m_btnMemorizeSkills[SKILLLEARN_NEW_SLOT_TOTAL];	// Buttons of passive skill
	CUIButtonEx				m_btnSpecialSkills[SKILLLEARN_NEW_SLOT_TOTAL];	// Buttons of special skill

	// Skill information
	int						m_nSelActiveSkillID;					// Selected active skill ID
	int						m_nSelPassiveSkillID;					// Selected passive skill ID
	int						m_nSelMemorizeSkillID;					// Selected memorize skill ID
	int						m_nSelSpecialSkillID;					// Selected special skill ID
	CTString				m_strShortDesc;							// Short skill information string

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons
	UIRect					m_rcTab;								// Region of tab

	// UV of each part
	UIRectUV				m_rtBackground;							// UV of background
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button
	UIRectUV				m_rtTabLine;							// UV of tab seperator

	int						m_nSSkillType;

	BOOL					m_bSpecial;								// 특수 스킬 NPC?
	BOOL					m_bUseCard;							// 카드 사용 유무
	int						m_iSelChangeJob;						// 원하는 전직.

	BOOL m_bSkillInfoVisible;

	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information

	CUIRectString			m_rsSkillName;
	CUIRectString			m_rsSkillDesc;

	CUIRectString			m_rsCurrentSkillInfo;
	CUIRectString			m_rsNextSkillInfo;

	UIRect					m_rcSkillInfo;

protected:
	// Internal functions
	void	InitSkillLearn( BOOL  bSpecial );
	void	RenderSkillBtns();
	void	GetSkillDesc( int nIndex, int nLevel = 0, BOOL bSpecial = FALSE );
	void	AddSkillDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
	void	CloseSkillLearn();

	// Network message functions ( send )
	void	SendLearnSkill();

	struct	sCollectSkill
	{
		LONG	lSkillIndex;
		SBYTE	sbSkillLevel;
		WORD	sbNeedSkillLevel;

		sCollectSkill()
		{
			lSkillIndex		= -1;
			sbSkillLevel	= -1;
			sbNeedSkillLevel= -1;
		}

		bool operator<(const sCollectSkill &other) const
		{			
			if(sbNeedSkillLevel < other.sbNeedSkillLevel)		return true;
			else if(sbNeedSkillLevel > other.sbNeedSkillLevel)	return false;
			else												return false;
		}

		bool operator>(const sCollectSkill &other) const
		{			
			return other.operator < (*this);
		}

		void SetData(int nIndex, SBYTE sbLevel, SBYTE sbNeedLevel)
		{
			lSkillIndex			= nIndex;
			sbSkillLevel		= sbLevel;
			sbNeedSkillLevel	= sbNeedLevel;
		}
	};

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE

	UIRectUV				m_rtSelOutlineTopL;	//선택된 스킬 표시 라인
	UIRectUV				m_rtSelOutlineTopM;
	UIRectUV				m_rtSelOutlineTopR;
	UIRectUV				m_rtSelOutlineMiddleL;
	UIRectUV				m_rtSelOutlineMiddleM;
	UIRectUV				m_rtSelOutlineMiddleR;
	UIRectUV				m_rtSelOutlineBottomL;
	UIRectUV				m_rtSelOutlineBottomM;
	UIRectUV				m_rtSelOutlineBottomR;

	UIRectUV				m_rtSelectedTab;
	UIRectUV				m_rtUnSelectedTab;

	CUIButtonEx*			m_btnSelectedSkill;	//현재 스킬

	UIRect					m_rcButtonArea;	//버튼영역을 더 넓게 체크

	CUIScrollBar			m_sbScrollBar;


	int					m_nActiveSkillSatisfied[SKILLLEARN_NEW_SLOT_TOTAL];
	int					m_nPassiveSkillSatisfied[SKILLLEARN_NEW_SLOT_TOTAL];
	int					m_nSpecialSkillSatisfied[SKILLLEARN_NEW_SLOT_TOTAL];
	

	int*					m_pSelectedSkillSatisfied;

	CTString				m_strDesc;

	int						m_nSelectedSkillID;
	int						m_nNumOfActiveSkill;
	int						m_nNumOfPassiveSkill;
	int						m_nNumOfSpecialSkill;
	int						m_nCurrentSkillType;

	int						m_nCurrentShown;

	void	ShowSkillLearnInfo(int nRow = -1);

	void	RenderNewSkillLearnBtns();
	void 	RenderNewSkillLearn();
	void	RenderSkillInfoDesc();
	void	AddSkillInfoString(int nSkillInfoList, CTString strSkillInfo, COLOR strColor = 0xF2F2F2FF);
	void	ResetSkillInfoString();
	void	SetSkillInfoPos(int nPosX, int nPosY);
	int	    GetSkillInfoWidth();
	int 	GetSkillInfoHeight();

	enum SkillType
{
	SKILL_ACTIVE	= 0,
	SKILL_PASSIVE,
	SKILL_SPECIAL,
	SKILL_TOTAL,
};

	void SetCurrentSkill(int skill);

	WMSG_RESULT	SKillLearnNewMouseMessage( MSG *pMsg );
//////////////////////////////////////////////////////////////////////////
	
public:
	CUISkillLearn();
	~CUISkillLearn();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open skill learn
	ENGINE_API void	OpenSkillLearn( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	PriorityOpen(int iIndex, BOOL bUseCard=FALSE);

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void	LearnSkill( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel, BOOL bSpecial = FALSE );
	void	LearnSkillError( UBYTE ubError );
	void	LearnSSkillError( UBYTE ubError );

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
	void CreateNewSkillLearn(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);
	void	UpdateSkillLearn();
	int IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial );
//////////////////////////////////////////////////////////////////////////
};


#endif	// UISKILLLEARN_H_

