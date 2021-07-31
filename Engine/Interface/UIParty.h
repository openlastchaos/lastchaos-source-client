// ----------------------------------------------------------------------------
//  File : UIParty.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPARTY_H_
#define	UIPARTY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>


// Party type
enum PartyType
{
	PT_PEACEEVER = 0,	// 균등 분배
	PT_SURVIVAL = 1,	// 입수우선 
	PT_ATTACK = 2,		// 공격자경험치 우선
};


// Maximum count of party members
#define	PARTY_MAX_MEMBER			7


// Define text position
#define	PARTY_TITLE_TEXT_OFFSETX	25
#define	PARTY_TITLE_TEXT_OFFSETY	5
//#define	PARTY_NAME_SX				11
//#define	PARTY_NAME_SY				23
#define	PARTY_NAME_SX				15
#define	PARTY_NAME_SY				2


// Define size of option
//#define	PARTY_WIDTH					140
//#define	PARTY_HEIGHT				60
//#define	PARTY_STRETCH_HEIGHT		36
//#define	PARTY_BAR_WIDTH				120
#define	PARTY_WIDTH					161
#define	PARTY_HEIGHT				57
#define	PARTY_STRETCH_HEIGHT		58
#define	PARTY_BAR_WIDTH				99


// ----------------------------------------------------------------------------
// Name : CUIParty
// Desc :
// ----------------------------------------------------------------------------
class CUIParty : public CUIWindow
{
protected:
	// Control
	CUIButton			m_btnClose;									// Close button

	CUIButton			m_btnOption;								// UI_REFORM :Su-won

	// Info
	int					m_nType;									// Party type
	int					m_nMemberCount;								// Count of members
	BOOL				m_bAmILeader;								// If I am a leader or not
	BOOL				m_bIsPartyRequested;						// If party is requested now or not
	CTString			m_strDestName;								// String of dest name when request party
	SLONG				m_aslIndices[PARTY_MAX_MEMBER];				// Indices of members
	SLONG				m_aslZone[PARTY_MAX_MEMBER];				// Zone of members
	CTString			m_astrNames[PARTY_MAX_MEMBER];				// Strings of member's name
	BOOL				m_abLeaders[PARTY_MAX_MEMBER];				// If this member is a leader or not
	SBYTE				m_asbJobs[PARTY_MAX_MEMBER];				// Jobs of members
	int					m_aswLevels[PARTY_MAX_MEMBER];				// Levels of members
	int					m_aswHPs[PARTY_MAX_MEMBER];					// HPs of members
	int					m_aswMaxHPs[PARTY_MAX_MEMBER];				// Maximum HPs of members
	int					m_aswMPs[PARTY_MAX_MEMBER];					// MPs of members
	int					m_aswMaxMPs[PARTY_MAX_MEMBER];				// Maximum MPs of members
	FLOAT				m_afPosX[PARTY_MAX_MEMBER];					// Position x of members
	FLOAT				m_afPosZ[PARTY_MAX_MEMBER];					// Position z of members
	SBYTE				m_asbLayer[PARTY_MAX_MEMBER];				// Layer of members
	UBYTE				m_aubBuff[PARTY_MAX_MEMBER];				// If buff is shown or not
	BOOL				m_bShowBuff;								// iF buff is shown or not

	ULONG				m_ultargetID[PARTY_MAX_MEMBER];				// 파티원이 공격하고 있는 타겟 ID
	SBYTE				m_sbtargetType[PARTY_MAX_MEMBER];			// 파티원이 공격하고 있는 타겟 Type

	// Region
	UIRect				m_rcTitle;
	UIRect				m_arcHPs[PARTY_MAX_MEMBER];					// Region of member's HP
	UIRect				m_arcMPs[PARTY_MAX_MEMBER];					// Region of member's MP
	UIRect				m_rcBuff;									// Region of buff toggling icon

	// UV
	UIRectUV			m_rtTop;									// UV of top region
	UIRectUV			m_rtMiddle;									// UV of middle region
	UIRectUV			m_rtBottom;									// UV of bottom region
	UIRectUV			m_rtHP;										// UV of HP
	UIRectUV			m_rtMP;										// UV of MP
	UIRectUV			m_rtBuff[2];								// UV of buff toggling icon
	CTString			m_strMandateCharName ;						// 위임하려는 케릭터의 Index

protected:
	// Internal functions
	void	AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	RemoveMember( SLONG slIndex );
	void	BreakUpParty();
	void	UpdateMemberInfo( int nMemIndex );
	void	TargetMember( int nIndex );
	void	UpdatePartyBuff( int nMemIndex, int nX, int nY );

	// Network message functions ( send )
	void	SendPartyAllow();
	void	SendPartyReject();

public:
	CUIParty();
	~CUIParty();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Init
	void	Init();

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Party full
	BOOL	IsPartyFull() { return m_nMemberCount == PARTY_MAX_MEMBER; }

	// Get information
	int		GetMemberCount() const { return m_nMemberCount; }
	SLONG	GetMemberIndex( int nIndex ) const { return m_aslIndices[nIndex]; }
	SLONG	GetMemberZone( int nIndex ) const { return m_aslZone[nIndex]; }
	FLOAT	GetPosX( int nIndex ) const { return m_afPosX[nIndex]; }
	FLOAT	GetPosZ( int nIndex ) const { return m_afPosZ[nIndex]; }
	SBYTE	GetLayer( int nIndex ) const { return m_asbLayer[nIndex]; }

	// 051203
	// ENGINE_API bool IsPartyMember( CEntity *pEntity );
	ENGINE_API bool IsPartyMember( INDEX iChaIndex );

	// Update status of member
	ENGINE_API void	UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP, int iMP, int iMaxMP );

	// Toggle visible
	void	ToggleVisible();

	// Hide party buff
	void	HidePartyBuff() { m_bShowBuff = FALSE; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Network message functions ( send )
	void	SendPartyInvite( SBYTE sbType, CTString &strDestName );
	void	SendPartyQuit();
	void	SendPartyKick( CTString &strDestName );

	// Network message functions ( receive )
	void	PartyInvite( SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName );
	void	PartyReject( BOOL bLeader );
	void	PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	PartyQuit( SLONG slIndex );
	void	PartyKick( SLONG slIndex );
	void	PartyEnd();
	void	PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	MandateBossReq( CTString strManadateChaName );
	void	MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate );
	void	PartyError( SBYTE sbError );
	bool	IsOurParty( CTString strName );		// 이름으로 파티원 검색
	int		IsOurPartyPos( CTString strName );	// 이름으로 검색 
	int		IsOurParty( SLONG slCharIndex );	// 식별자로 파티원 검색
	void	SetPartyLeader();					// 파티 리더 설정
	BOOL	AmILeader() { return m_bAmILeader; }
	void	SetDestName( CTString strDest ) { m_strDestName = strDest; }
	BOOL	IsPartyRequested() { return m_bIsPartyRequested; }
	void	SetPartyRequested( BOOL IsRequested ) { m_bIsPartyRequested = IsRequested; }

	ENGINE_API void	SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType);

	void	SetPartyType( SBYTE sbPartyType) { m_nType = sbPartyType; }
};


#endif	// UIPARTY_H_

