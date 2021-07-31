// ----------------------------------------------------------------------------
//  File : UIBuff.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIBUFF_H_
#define	UIBUFF_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIWindow.h>
#include <Engine/Effect/CEffectGroup.h >


// ----------------------------------------------------------------------------
// Name : BuffInfo
// Desc :
// ----------------------------------------------------------------------------
typedef struct _BuffInfo
{
public:
	SLONG		m_slItemIndex;
	SLONG		m_slSkillIndex;
	SLONG		m_slRemain;
	SBYTE		m_sbLevel;
	__int64		m_llStartTime;
	CEffectGroup * m_pEG;


	void	Init()
	{
		m_slItemIndex = -1;
		m_slSkillIndex = -1;
		m_sbLevel = -1;
		m_slRemain = 0;
		m_pEG = NULL;
	}
} BuffInfo;


// Buff type
enum BuffType
{
	MY_BUFF		= 0,
	PARTY_BUFF,
	TARGET_BUFF,
};


// Buff size
#define	BUFF_SIZE_BIG				20
#define	BUFFOUT_SIZE_BIG			22
#define	BUFF_SIZE_SMALL				16
#define	BUFFOUT_SIZE_SMALL			18


// Buff count
#define	BUFF_MAX_COUNT				38
#define	BUFF_GOOD_COUNT				30				//Buff영역 확장 22 - > 30
#define	BUFF_BAD_COUNT				8


// Define max char and line of buff informaion 
#define	MAX_BUFFINFO_CHAR			30  // 26 -> 30 wooss 060622
#define	MAX_BUFFINFO_LINE			20
#define	MAX_BUFF_COL				8
#define	MAX_BUFF_ROW				5				//Buff영역 확장 4 - > 5

// world time multiple
#define WORLDTIME_MUL				(24) 

// ----------------------------------------------------------------------------
// Name : CBuffIcon
// Desc :
// ----------------------------------------------------------------------------
class CBuffIcon
{
protected:
	SLONG		m_slItemIndex;
	SLONG		m_slSkillIndex;
	int			m_nTextureID;
	SBYTE		m_sbLevel;
	__int64		m_llRemain;

	UIRectUV	m_rtBuff;
	__int64		m_llStartTime;

public:
	CBuffIcon() { Init(); }
	~CBuffIcon() {}

	void	Init()
	{
		m_slItemIndex = -1;
		m_slSkillIndex = -1;
		m_nTextureID = -1;
		m_sbLevel = -1;
		m_llRemain = 0;
	}
	void	Set( SLONG slItemIndex, SLONG slSkillIndex, SBYTE sbLevel, SLONG slRemain, __int64 llStartTime );

	SLONG		GetItemIndex() const { return m_slItemIndex; }
	SLONG		GetSkillIndex() const { return m_slSkillIndex; }
	int			GetTextureID() const { return m_nTextureID; }
	SBYTE		GetLevel() const { return m_sbLevel; }
	UIRectUV	&GetUV() { return m_rtBuff; }
	__int64		GetRemainTime( __int64 llCurTime ) { return m_llRemain - ( llCurTime - m_llStartTime ); }
	bool		IsUpSkill() const;
};


// ----------------------------------------------------------------------------
// Name : CUIBuff
// Desc :
// ----------------------------------------------------------------------------
class CUIBuff
{
protected:
	// My buff
	int				m_nMyGoodBuffSX;
	int				m_nMyGoodBuffSY;
	int				m_nMyBadBuffSX;
	int				m_nMyBadBuffSY;
	UBYTE			m_ubMyGoodCount;
	UBYTE			m_ubMyBadCount;
	BuffInfo		m_aMyBuffInfo[BUFF_MAX_COUNT];
	CBuffIcon		m_aMyBuff[BUFF_MAX_COUNT];
	UIRect			m_rcMyGoodBuff;
	UIRect			m_rcMyGoodBuffRow[MAX_BUFF_ROW];
	UIRect			m_rcMyBadBuff;

	// Party buff
	int				m_nPartyBuffSX;
	int				m_nPartyBuffSY;
	SLONG			m_slPartyIndex;
	UBYTE			m_ubPartyGoodCount;
	UBYTE			m_ubPartyBadCount;
	CBuffIcon		m_aPartyBuff[BUFF_MAX_COUNT];
	UIRect			m_rcPartyBuff;
	UIRect			m_rcTempPartyBuff;

	// Target buff
	UBYTE			m_ubTargetGoodCount;
	UBYTE			m_ubTargetBadCount;
	CBuffIcon		m_aTargetBuff[BUFF_MAX_COUNT];
	UIRect			m_rcTargetBuff;
	UIRect			m_rcTempTargetBuff;

	// Buff information
	int				m_nSelBuffID;							// Selected buff ID
	int				m_nSelBuffItemIndex;					// Selected buff item index
	int				m_nSelBuffSkillIndex;					// Selected buff skill index
	SBYTE			m_sbSelBuffSkillLevel;					// Selected buff skill level
	BOOL			m_bShowBuffInfo;						// If buff information is shown or not
	int				m_nCurInfoLines;						// Count of current item information lines
	int				m_nRemainTimeLine;						// Line index of remained time
	CTString		m_strBuffInfo[MAX_BUFFINFO_LINE];		// Buff information string
	COLOR			m_colBuffInfo[MAX_BUFFINFO_LINE];		// Color of buff information string
	UIRect			m_rcBuffInfo;							// Buff information region

	// Texture
	CTextureData	*m_ptdBuffTexture;						// Texture of buff

	// UV of each part
	UIRectUV		m_rtOutlineBigGood;						// UV of good outline ( 22x22 )
	UIRectUV		m_rtOutlineBigBad;						// UV of bad outline ( 22x22 )
	UIRectUV		m_rtOutlineSmallGood;					// UV of good outline ( 18x18 )
	UIRectUV		m_rtOutlineSmallBad;					// UV of bad outline ( 18x18 )
	UIRectUV		m_rtInfoUL;								// UV of upper left region of information
	UIRectUV		m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV		m_rtInfoUR;								// UV of upper right region of information
	UIRectUV		m_rtInfoML;								// UV of middle left region of information
	UIRectUV		m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV		m_rtInfoMR;								// UV of middle right region of information
	UIRectUV		m_rtInfoLL;								// UV of lower left region of information
	UIRectUV		m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV		m_rtInfoLR;								// UV of lower right region of information

protected:
	void	AdjustMyBuffRegion( BOOL bGoodBuff );
	void	AdjustPartyBuffRegion();
	void	AdjustTargetBuffRegion();
	void	AddBuffInfoString( CTString &strBuffInfo, COLOR colBuffInfo = 0xF2F2F2FF );
	void	GetBuffInfo( int nItemIndex, int nSkillIndex, int nSkillLevel, int &nInfoWidth, int &nInfoHeight );
	void	ShowBuffInfo( BOOL bShowInfo, int nWhichBuff = -1, int nSelBuff = -1 );
	void	RenderToolTip( int nWhichBuff, __int64 llCurTime );

public:
	CUIBuff();
	~CUIBuff();

	// Create
	void	Create();

	// Render
	void	RenderMyBuff();
	void	RenderPartyBuff();
	void	RenderTargetBuff();

	// Reset
	void	ResetMyBuff()
	{
		m_ubMyGoodCount = 0;
		m_ubMyBadCount = 0;
		m_rcMyGoodBuff.SetRect( -1, -1, -1, -1 );
		for(int i =0 ;i< MAX_BUFF_ROW;i++){
			m_rcMyGoodBuffRow[i].SetRect(-1,-1,-1,-1);
		}
		for(i=0; i<MAX_BUFF_ROW ;i++){
			m_aMyBuffInfo[i].Init();
		}
		
		
		m_rcMyBadBuff.SetRect( -1, -1, -1, -1 );
	}
	void	ResetPartyBuff()
	{
		m_slPartyIndex = -1;
		m_ubPartyGoodCount = 0;
		m_ubPartyBadCount = 0;
	}
	void	ResetTargetBuff()
	{
		m_ubTargetGoodCount = 0;
		m_ubTargetBadCount = 0;
	}

	// Copy
	void	CopyMyBuffToTargetBuff();

	// Add
	void	AddMyBuff( BuffInfo &rBuffInfo );
	void	AddPartyBuff( SLONG slPartyIndex, BuffInfo &rBuffInfo );
	void	AddTargetBuff( BuffInfo &rBuffInfo );

	// Remove
	void	RemoveMyBuff( SLONG slItemIndex, SLONG slSkillIndex );
	void	RemovePartyBuff( SLONG slPartyIndex, SLONG slItemIndex, SLONG slSkillIndex );
	void	RemoveTargetBuff( SLONG slItemIndex, SLONG slSkillIndex );

	// Position
	void	SetMyGoodBuffPos( int nX, int nY )
	{
		m_nMyGoodBuffSX = nX;
		m_nMyGoodBuffSY = nY;
	}
	void	SetMyBadBuffPos( int nX, int nY )
	{
		m_nMyBadBuffSX = nX;
		m_nMyBadBuffSY = nY;
	}
	void	SetPartyBuffPos( SLONG slPartyIndex, int nX, int nY )
	{
		m_slPartyIndex = slPartyIndex;
		m_nPartyBuffSX = nX;
		m_nPartyBuffSY = nY;
	}

	// Info
	void	SetPartyIndex( SLONG slPartyIndex ) { m_slPartyIndex = slPartyIndex; }
	SLONG	GetPartyIndex() const { return m_slPartyIndex; }
	
	// wooss 050802
	// -->
	BuffInfo* GetBuffArray() { return m_aMyBuffInfo; }		// : insert get info method 
															//   for buff check	
	BOOL	ENGINE_API IsBuff(SLONG itemIndex) ;						// : find some buff in Buff Array
	// wooss 070310 ------------------------------>><<
	// kw : WSS_WHITEDAY_2007
	CEffectGroup* IsBuffBySkill(SLONG skillIndex) ;						// : find some buff in Buff Array by Skill index
	CEffectGroup* GetEGFromSkillIndex(SLONG skillIndex);

	UBYTE	ENGINE_API IsMyGoodBuffCount() { return m_ubMyGoodCount; }	//
	UBYTE	ENGINE_API IsMyBadBuffCount() { return m_ubMyBadCount; }		//
	
	// <--


	// Messages
	WMSG_RESULT	MouseMessageMyBuff( MSG *pMsg );
	WMSG_RESULT	MouseMessagePartyBuff( MSG *pMsg );
	WMSG_RESULT	MouseMessageTargetBuff( MSG *pMsg );
};

extern ENGINE_API CUIBuff	*_pUIBuff;


#endif	// UIBUFF_H_

