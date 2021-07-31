#ifndef SE_INCL_NETWORK_H
#define SE_INCL_NETWORK_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/FileName.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Stream.h>
#include <Engine/World/World.h>
#include <Engine/Network/MessageDispatcher.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/GlobalDefinition.h>
//#include <Engine/Interface/UIProcess.h>			// 이기환 추가 ( 12. 6 )
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Entities/ItemEffect.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Network/ChatMsgBuffer.h>
#include <Engine/Network/MultiTarget.h>
#include <Engine/Entities/CharacterAnimation.h>
// EDIT : BS : 070413 : 신규 패킷 암호화
// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
#include <LCCrypt.h>
#else
#include <Engine/Network/CryptNetMsg.h>
#endif

#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Entities/NpcHelp.h>

// 공성 참여 상태(status, appear )
#define WCJF_NONE   0  // 비참여
#define WCJF_OWNER   1  // 성주 길드
#define WCJF_DEFENSE_GUILD 2  // 수성 길드
#define WCJF_DEFENSE_CHAR 3  // 수성측 용병
#define WCJF_ATTACK_GUILD 4  // 공성 길드
#define WCJF_ATTACK_CHAR 5  // 공성측 용병

//0603 kwon 
#include <Engine/Entities/Items.h>
#include <Engine/Entities/TargetInfo.h>

#include <Engine/Entities/SmcParser.h> // eons 071005 : SmcParser 클래스
#include <Engine/LocalDefine.h>//RESTART_GAME

#define NET_MAXGAMECOMPUTERS SERVER_CLIENTS		// max overall computers in game
#define NET_MAXGAMEPLAYERS			1			// max overall players in game
#define NET_MAXLOCALPLAYERS			1			// max players on a single machine

#define NET_WAITMESSAGE_DELAY		50			// wait time between receive message attempts

#define NET_MAXSESSIONPROPERTIES	2048		// size of buffer for custom use by CGame and entities

// demo synchronization constants
#define DEMOSYNC_REALTIME			(0.0f)
#define DEMOSYNC_STOP				(-1.0f)

//[ttos_2009_1_20]: 채팅 금지 flag
#define CHAT_FLAG_NO_SAY  (1 << 0)  // 일반 채팅 금지
#define CHAT_FLAG_NO_PARTY  (1 << 1)  // 파티 채팅 금지
#define CHAT_FLAG_NO_GUILD  (1 << 2)  // 길드 채팅 금지
#define CHAT_FLAG_NO_TRADE  (1 << 3)  // 매매 채팅 금지
#define CHAT_FLAG_NO_WHISPER (1 << 4)  // 귓속말 금지
#define CHAT_FLAG_NO_SHOUT  (1 << 5)  // 외치기 금지
 
extern ENGINE_API INDEX net_bLocalPrediction;

enum NetGraphEntryType {
	NGET_ACTION = 0,
	NGET_NONACTION,
	NGET_MISSING,
	NGET_SKIPPEDACTION,
	NGET_REPLICATEDACTION,
};

enum CurEnterType { // 현재 게임 입장 타입(진행 되고 있는 모드?)
	CURRENT_ENTER_NORMAL,
	CURRENT_ENTER_PARTYCUBE,
	CURRENT_ENTER_GUILDCUBE,
};

struct NetGraphEntry {
	enum NetGraphEntryType nge_ngetType;	// type of packet/event
	FLOAT nge_fLatency;						// latency in seconds
	void Clear(void);
};

#define NONCE_SIZE 8

struct CNeedItems;

/*
 * Network session description.
 */
class ENGINE_API CNetworkSession {
public:
	CListNode ns_lnNode;				// for linking in list of available sessions
	CTString ns_strAddress;				// session address

	CTString ns_strSession;				// session name
	CTString ns_strWorld;				// world name
	TIME     ns_tmPing;					// current players
	INDEX    ns_ctPlayers;				// current players
	INDEX    ns_ctMaxPlayers;			// max number of players
	CTString ns_strGameType;			// game type
	CTString ns_strMod;					// active mod
	CTString ns_strVer;					// version
	TIME     ns_tmLastSeen;				// time of the last rewponse to enumeration
	ULONG    ns_ulAddress;				// server's address
	UWORD    ns_uwPort;					// server's port
	
public:
	void Copy(const CNetworkSession &nsOriginal);

	/* Default constructor. */
	CNetworkSession(void);
	/* Construct a session for connecting to certain server. */
	CNetworkSession(const CTString &strAddress);
};

/*
 * Game, used for general game initialization/manipulation
 */
class ENGINE_API CNetworkLibrary : public CMessageDispatcher {
	
public:
	// EDIT : BS : BEGIN
	DWORD m_tickSendMoveList;
	// EDIT : BS : END
	BOOL ga_IsServer;								// set if this is a server computer
	CServer &ga_srvServer;							// server (active only if this is server computer)

	CTCriticalSection ga_csNetwork;					// critical section for access to network data

	CSessionState &ga_sesSessionState;				// local session state
	CStaticArray<CPlayerSource> ga_aplsPlayers;		// local players
	CTString ga_strSessionName;						// name of current session
	CTString ga_strServerAddress;					// address of game server (if joined)
	CTFileName ga_fnmWorld;							// filename of current world
	CTString ga_strGameType;						// game type (cooperative, scorematch, deathmatch)
	CTString ga_strMod;								// name of the mod
	CTString ga_strVer;								// version

	UBYTE *ga_pubDefaultState;						// default state for connecting (server only)
	SLONG ga_slDefaultStateSize;
	UBYTE ga_aubDefaultProperties[NET_MAXSESSIONPROPERTIES];
	UBYTE *ga_pubCRCList;							// list of files for CRC checking (server only)
	SLONG ga_slCRCList;
	ULONG ga_ulCRC;									// CRC of CRCs of all files in the list

	BOOL ga_bLocalPause;							// local pause for single player/demo
	BOOL ga_bDemoRec;								// set if currently recording a demo
	CTFileStream ga_strmDemoRec;					// currently recorded demo file
	BOOL ga_bDemoPlay;								// set if currently playing a demo
	BOOL ga_bDemoPlayFinished;						// set if currently playing demo has finished
	CTFileStream ga_strmDemoPlay;					// currently played demo file
	CTimerValue ga_tvDemoTimerLastTime;				// real time timer for demo synchronization
	CTimerValue ga_tvGameFinished;					// time when the game was finished
	TIME ga_tmNextDemoTick;							// time of the next tick to be read from stream for demo play

	CTFileName ga_fnmNextLevel;						// world for next level
	BOOL  ga_bNextRemember;							// remember old levels when changing to new one
	INDEX ga_iNextLevelUserData;					// user data for next level
	UBYTE ga_ubNumLevelChanges;						// how many times has levelchange occured

	CListHead ga_lhEnumeratedSessions;				// list of sessions found after last enumeration was triggered
	FLOAT ga_fEnumerationProgress;					// enumeration progress percentage (0-1)
	CTString ga_strEnumerationStatus;				// description of current operation
	BOOL ga_bEnumerationChange;						// this is raised if something was changed in the list
	BOOL ga_bGuildWar;								// 길드 전쟁 중인가?


	CTString ga_strRequiredMod;						// set if connection failed due to a different mod running on the server

	CTimerValue ga_tvLastMainLoop;					// when was the main loop last run - for tick sync
	BOOL  ga_bFirstMainLoop;						// has the main loop been run before

	// buffer for custom use by CGame and entities
	UBYTE ga_aubProperties[NET_MAXSESSIONPROPERTIES];

	BOOL IsServer(void) { return ga_IsServer;};

	// make actions packet for local players and send to server
	void SendActionsToServer(void);

	/* Loop executed in timer interrupt, every game tick. */
	void TimerLoop(void);

	// really do the level change
	void ChangeLevel_internal(void);
	void ChangeLevel_client_internal_t(void);

	// save current version of engine
	void WriteVersion_t(CTStream &strm);
	// load version of engine saved in file and check against current
	void CheckVersion_t(CTStream &strm, BOOL bAllowReinit, BOOL &bNeedsReinit);

	// add a value to the netgraph
	void AddNetGraphValue(enum NetGraphEntryType nget, FLOAT fLatency);

	// add a value to the bandwidth graph
	void AddBandwidthGraphValue(ULONG ulBandwidth);

	// call this while game is not running - to update server enumeration lists
	void GameInactive(void);

	// automatically adjust network settings
	void AutoAdjustSettings(void);

	// initialize gathering of file CRCs to CRC table
	void InitCRCGather(void);
	// finish gathering of file CRCs to CRC table (call for server only!)
	void FinishCRCGather(void);

public:
	CWorld ga_World;										// local copy of world
	FLOAT ga_fDemoTimer;									// timer for demo playback (in seconds)
	FLOAT ga_fDemoRealTimeFactor;							// slow/fast playback factor (for DEMOSYNC_REALTIME only)
	FLOAT ga_fGameRealTimeFactor;							// game time accelerator
	FLOAT ga_fDemoSyncRate;									// demo sync speed in FPS (or realtime/stop)
	CStaticArray<struct NetGraphEntry> ga_angeNetGraph;		// array of netgraph entries
	CStaticArray<ULONG> ga_aulBandwidthGraph;				// array of bandwidth entries

	//0603 kwon 
	CItems MySlotItem[INVEN_SLOT_TAB][INVEN_SLOT_ROW_TOTAL][INVEN_SLOT_COL];
	CItems	*pMyCurrentWearing[WEAR_TOTAL];
	
	//0724
	// NOTE : 도대체 Infom과 InfomReal 두개를 왜 따로 둔거지???
	// NOTE : 왜 두개가 필요한지 이해할수 없음.
	CTargetInfom		_TargetInfo;
	CTargetInfomReal	_TargetInfoReal;
	CPetTargetInfom		_PetTargetInfo;
	CSlaveTargetInfom	_SlaveTargetInfo[2];
	CWildPetInfo		_WildPetInfo;				//내가 착용하고 있는 공격형 펫
	//int		MyCurrentSkill[50];
	//int		MySkillNum;
	int						wo_iNumOfAction;		// yjpark
	int						wo_iNumOfSkill;			// yjpark
	int						wo_iNumOfMobName;
	int						wo_iNumOfSSkill;
	int						wo_iNumOfOption;
	CStaticArray<CItemData> wo_aItemData; 
	CStaticArray<CItemName>		wo_aItemName; 
	std::vector<CItemRareOption>	wo_vecItemRareOption;	//레어 옵션을 저장할 백터
	CStaticArray<COptionData> wo_aOptionData;
	CStaticArray<CNpcHelp>	wo_aNpcList;
	CSmcParserList			wo_aItemSmcInfo;	// 아이템 Smc 정보
	CStaticArray<CMissionCase>	wo_aMissionCase;
	CStaticArray<CWildPetData>	wo_aWildPetData;	//ttos_080725: 신규 펫 데이터

	int						wo_iNumOfItem;
	int						wo_iNumOfItemName;
	int						wo_iNumOfRareOption;		//레어 옵션 개수

	int						wo_dwEnemyCount;		// 에너미의 갯수.
	int						wo_dwKilledEnemyCount;
	int						wo_iNumofNpcList;
	int						wo_iNomofMissionCase;
	int						wo_iNomOfWildPetData;	//ttos_080725: 신규 펫 데이터

	struct CHECKPOINT{								// 해당 체크 포인트 진행 여부
		int m_iQuestIndex;
		int m_iMaxCheckFlag;				
		int m_iCheckFlag;
	} ;
	CHECKPOINT wo_stCheckPoint;

	//0917
	CTString	m_strUserID;
	CTString	m_strUserPW;
	int			m_iServerGroup;
	int			m_iServerCh;

	BOOL	m_bIsPvp;
	void	SetPvpMode() { m_bIsPvp = !m_bIsPvp; };
	LONG	m_lLoadingZone;

	// interface
	/* Default constructor. */
	CNetworkLibrary(void);
	~CNetworkLibrary(void);
	DECLARE_NOCOPYING(CNetworkLibrary);

	/* Initialize game management. */
	void Init(const CTString &strGameID);
	/* Start a peer-to-peer game session. */
	void StartPeerToPeer_t(const CTString &strSessionName,
		const CTFileName &fnmWorld, ULONG ulSpawnFlags, 
		INDEX ctMaxPlayers, BOOL bWaitAllPlayers,
		void *pvSessionProperties); // throw char *

	/* Start numeration of existing sessions. */
	void EnumSessionsStart(BOOL bInternet);
	/* Continue numeration of existing sessions. */
	void EnumSessionsContinue();
	/* Stop numeration of existing sessions. */
	void EnumSessionsStop();

	/* Join a running multi-player game. */
	void JoinSession_t(const CNetworkSession &nsSesssion, INDEX ctLocalPlayers); // throw char *
	/* Start playing a demo. */
	//void StartDemoPlay_t(const CTFileName &fnDemo); // throw char *
	/* Test if currently playing a demo. */
	BOOL IsPlayingDemo(void);
	/* Test if currently recording a demo. */
	//BOOL IsRecordingDemo(void);
	/* Test if currently playing demo has finished. */
	//BOOL IsDemoPlayFinished(void);
	/* Stop currently running game. */
	void StopGame(void);


	// pause/unpause game
	void TogglePause(void);
	// test if game is paused
	BOOL IsPaused(void);
	// test if game is waiting for more players to connect
	BOOL IsWaitingForPlayers(void);
	// test if game is waiting for server
	BOOL IsWaitingForServer(void);
	// mark that the game has finished -- called from AI
	void SetGameFinished(void);
	BOOL IsGameFinished(void);
	// manipulation with realtime factor for slower/faster time -- called from AI
	void SetRealTimeFactor(FLOAT fSpeed);
	FLOAT GetRealTimeFactor(void);   
	// test if having connnection problems (not getting messages from server regulary)
	BOOL IsConnectionStable(void);
	// test if completely disconnected and why
	BOOL IsDisconnected(void);
	const CTString &WhyDisconnected(void);

	// set/get server side pause (for single player or demo only)
	void SetLocalPause(BOOL bPause);
	BOOL GetLocalPause(void);

	// get server/client name and address
	void GetHostName(CTString &strName, CTString &strAddress);

	// test if playing in network or locally
	BOOL IsNetworkEnabled(void);

	// initiate level change
	void ChangeLevel(const CTFileName &fnmNextLevel, BOOL bRemember, INDEX iUserData);

	/* Obtain file name of world that is currently loaded. */
	CTFileName &GetCurrentWorld(void) { return ga_fnmWorld;};

	/* Start recording a demo. */
	//void StartDemoRec_t(const CTFileName &fnDemo); // throw char *
	/* Stop recording a demo. */
	//void StopDemoRec(void);

	/* Read current game situation from a stream. */
	void Read_t(CTStream *pstr);   // throw char *
	/* Write current game situation into a stream. */
	void Write_t(CTStream *pstr);  // throw char *

	/* Save the game. */
	void Save_t(const CTFileName &fnmGame); // throw char *
	/* Load the game. */
	void Load_t(const CTFileName &fnmGame); // throw char *

	/* Save a debugging game. */
	void DebugSave(void);   // this doesn't throw anything

	/* Add a new player to game. */
	CPlayerSource *AddPlayer_t(CPlayerCharacter &pcCharacter);  // throw char *

	/* Loop executed in main application thread. */
	void MainLoop(void);

	/* Get count of local players. */
	INDEX GetLocalPlayerCount(void);
	/* Get player entity for a given local player. */
	CEntity *GetLocalPlayerEntity(CPlayerSource *ppls);
	/* Get player entity for a given player by name. */
	CEntity *GetPlayerEntityByName(const CTString &strName);

	/* Get number of entities with given name. */
	INDEX GetNumberOfEntitiesWithName(const CTString &strName);
	/* Get n-th entity with given name. */
	CEntity *GetEntityWithName(const CTString &strName, INDEX iEntityWithThatName);
	/* Get number of player entities with given name. */
	INDEX GetNumberOfPlayerEntitiesWithName(const CTString &strName);
	/* Get n-th player entity with given name. */
	CPlayerEntity *GetNthPlayerEntityWithName(const CTString &strName, INDEX iEntityWithThatName);
	/* Get n-th player entity. */
	CPlayerEntity *GetNthPlayerEntity(INDEX iEntity);
																			 
	/* Get n-th entity of given class. */
	CEntity *GetEntityOfClass(const CTString &strClass, INDEX iEntityOfThatClass);
	/* Test if a given player is local to this computer. */
	BOOL IsPlayerLocal(CEntity *pen);
	// get player source for a given player if it is local to this computer
	CPlayerSource *GetPlayerSource(CEntity *pen);

	// get game time in currently running game
	TIME GetGameTime(void);

	/* Get session properties for current game. */
	void *GetSessionProperties(void);

	int  RefreshShopItem(int iShopIndex);
	int	 RefreshLeaseItem(int iJobIndex);
	int  RefreshEventItem(int iJobIndex, int itype); // 2007 가정의 달 이벤트 (어린이날 이벤트)

	/* Send chat message from some players to some other players. */
	void SendChat(ULONG ulFrom, ULONG ulTo, const CTString &strMessage);
	void SendChatMessage(int index, CTString &strRecvName, CTString &strMessage);//0616 kwon
//0105
	void SendMessagetoServer(char *msg);
	//BOOL IsTcpServer;					// by seo-40225 부모클래스 CMessageDispatcher로 변수 위치 옮김.
	BOOL m_bReadyStart; //0311 kwon serverloop()가 구동될 준비가 되었는가.
	BOOL IsReadyStart(){return m_bReadyStart;}
	void SetReadyStart(BOOL bstart){ m_bReadyStart = bstart;}
	BOOL m_bSingleMode;

	// wooss 070305 ------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	CEntity* GetEntityByTypeIndex(SBYTE sbType, SLONG slIndex);

	// --------------------------------------------------------------<<

	// wooss 070305 ------------------------------------------------->>
	// kw : WSS_EVENT_LOD
	int ReturnCCC(int iSCC);
	// --------------------------------------------------------------<<


	//-----------------------------------------
	// Pet 관련 구조체.
	struct sPetInfo
	{
		sPetInfo()
		{
			lIndex			= -1;
			sbPetTypeGrade	= -1;
			lLevel			= -1;
			llExp			= -1;
			llNeedExp		= -1;
			lHP				= -1;
			lMaxHP			= 0;
			lAbility		= 0;
			lHungry			= -1;
			lMaxHungry		= -1;
			lSympathy		= -1;
			lMaxSympathy	= -1;
			fX				= 0.0f;
			fZ				= 0.0f;
			fH				= 0.0f;
			fR				= 0.0f;			
			sbMapAttribute	= -1;
			lRemainRebirth	= 0;
			strNameCard		= CTString("");
		};

		LONG		lIndex;
		SBYTE		sbPetTypeGrade;
		LONG		lLevel;
		__int64		llExp;
		__int64		llNeedExp;
		LONG		lHP;
		LONG		lMaxHP;
		LONG		lHungry;
		LONG		lMaxHungry;
		LONG		lAbility;
		LONG		lSympathy;
		LONG		lMaxSympathy;
		FLOAT		fX;
		FLOAT		fZ;
		FLOAT		fH;
		FLOAT		fR;		
		SBYTE		sbMapAttribute;
		LONG		lRemainRebirth;
		CTString	strNameCard;
	};

	// Function Object
	struct FindPet
	{
		SLONG lIndex;
		FindPet(sPetInfo& info) : lIndex(info.lIndex) {};
		bool operator() (const sPetInfo& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sMoveInfo
	{
		sMoveInfo()
		{
			iIndex	= -1;
			fX		= 0.0f;
			fZ		= 0.0f;
			fH		= 0.0f;
			fAngle	= 0.0f;
		};

		INDEX	iIndex;
		FLOAT	fX;
		FLOAT	fZ;
		FLOAT	fH;
		FLOAT	fAngle;
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sAttackInfo
	{
		sAttackInfo()
		{
			iAttackIndex	= -1;
			iTargetIndex	= -1;
			ubAttackType	= 0;
			ubTargetType	= 0;
		};

		UBYTE	ubAttackType;
		UBYTE	ubTargetType;
		INDEX	iAttackIndex;
		INDEX	iTargetIndex;
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sRegenInfo
	{
		sRegenInfo()
		{
			iIndex		= -1;
			iMobType	= -1;
			fX			= 0.0f;
			fY			= 0.0f;
			fZ			= 0.0f;
			fAngle		= 0.0f;
			iYLayer		= 0;
			iEntityID	= -1;
		};
		INDEX	iIndex;
		INDEX	iMobType;
		FLOAT	fX;
		FLOAT	fY;
		FLOAT	fZ;
		FLOAT	fAngle;
		INDEX	iYLayer;
		INDEX	iEntityID;
	};

	std::vector<sPetInfo>		m_vectorPetList;
	std::vector<sMoveInfo>		m_vectorMoveList;
	std::vector<sAttackInfo>	m_vectorAttackNPCList;		// NPC를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackPCList;		// PC를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackSummonList;	// 소환수를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackPetList;	// 소환수를 공격하는 리스트.
	std::vector<sRegenInfo>		m_vectorRegenList;
	std::vector<int>			m_vectorLegitList; // 정당방위 유저 목록
	__int64						m_llMoveStartTime;
	__int64						m_llAttackStartTime;

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	//0527 kwon struct 추가.
	struct MyChaInfo 
	{
		SLONG	 index;
		CTString name;
		UBYTE	 job;
		SBYTE	 job2;					// 전직시...
		ULONG	 zoneNo;
		ULONG	 areaNo;
		ULONG	 LocalNo;				// 지역 정보 ( show signboard )
		FLOAT	 x, z;
		FLOAT	 h;
		FLOAT	 r; //방향
		SBYTE	 yLayer;
		SLONG    userIndex;
		SBYTE	 hairStyle, faceStyle;

		SLONG level, hp, maxHP, mp, maxMP, str, dex, intel, con, opt_str, opt_dex, opt_intel, opt_con;
		SLONG sp, weight, maxweight;
		FLOAT walkspeed, runspeed,attackrange;
		SBYTE attackspeed;
		SQUAD money, curExp, needExp;
		SLONG StatPoint;//1013
		SBYTE sbJoinFlagMerac;			// 공성 참여시 값.
		SBYTE sbJoinFlagDratan;			// WSS_DRATAN_SIEGEWARFARE 070720
		UBYTE sbAttributePos;			// 속성맵 값.
		SBYTE sbMountPet;

		FLOAT camera_angle;

		SBYTE pktitle;
		SLONG pkpenalty, pkcount;
		SBYTE pk_mode;
		SLONG fame;						// 명성.
		// HP, MP 패널티 카운트
		SBYTE hpcount, mpcount;
		BOOL  bOtherZone;
		BOOL  bConsensus;							// WSS_DRATAN_SEIGEWARFARE 2007/08/01 교감중
		std::map<int,int> mQuarter;					// WSS_DRATAN_SEIGEWARFARE 2007/08/09 부활진지
		std::map<int,CTString> mQuarterName;		// WSS_DRATAN_SEIGEWARFARE 2007/08/09 부활진지 이름(소유 길드이름 추가)

		SBYTE			sbShopType;		
		LONG			lGuildIndex;		// 임시	길드의 인덱스
		LONG			lGuildLevel;		// 임시 길드의 레벨
		LONG			lGuildPosition;		// 임시 길드에서의 캐릭터의 레벨
		CTString		strGuildName;		// 길드명.
		UBYTE			ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		SBYTE			sbGuildRank;
		BOOL			bExtension;			// 두번째 타입의 무기를 든 경우...

		INDEX			iPetType;			// 애완동물 종류...(?)
		INDEX			iPetAge;			// 애완동물 나이...(?)
		BOOL			bPetRide;			// 애완동물을 타고 있나?
		
		SBYTE			sbEvocationType;		// 강신 타입.

		LONG			lTeacherIndex;		// 내 스승의 인덱스
		CTString		strTeacherName;		// 내 스승의 이름.

		ChatMsgBuffer	ChatMsg;		// yjpark
		ChatMsgBuffer	ShopMsg;		// yjpark
		FLOAT			fWaterHeight;	// yjpark - 임시로 추가한 파일. 적당한 위치에 적당한 형식으로 수정 예정..

		SLONG	magicspeed;
		SLONG	skillSpeed;
		CStatusEffect statusEffect;
		CItemEffect itemEffect;
		SBYTE	sbItemEffectOption;
//0707
		SLONG secretkey;
		SLONG lLocation;// Date : 2006-04-27(오후 2:33:10), By eons( NetCafe )

		SLONG lWarpZone;
		SLONG lWarpPos;

		// 강신체의 정보가 유지되도록...
		MyChaInfo()
		{
			sbEvocationType = -1;
			LocalNo = 0;
			sbJoinFlagDratan = WCJF_NONE;			// WSS_DRATAN_SIEGEWARFARE 070720
		};
		
		// WSS_VISIBLE 070511
		// 운영자 모드 캐릭트 비저블 처리
		// visible 명령어 사용시 세팅됨
		COLOR m_ModelColor;

		// 080623 라카 기자단 혜택
		SBYTE			sbPresscorps;	

		// 080901 UI개편
		SLONG			baseHP, baseMP, addedAttack, addedMagic, addedDefense, addedResist;
		SLONG			dodgeRate, baseDodgeRate, magicDodgeRate, baseMagicDodgeRate, hitRate, baseHitRate, magicHitRate, baseMagicHitRate;
		SLONG			critical, baseCritical, deadly, baseDeadly;
		FLOAT			baseRunSpeed;
		SBYTE           baseAttackSpeed;
		CTString		guildPosName;
		ULONG			ChatFlag;
		CurEnterType	EntranceType; // 게임중 현재 존 입장 상태 or 게임 모드 상태(특별한 상태로 인해 예외적인 적용을 위한 부분)(예:익스트림 큐브)
	};
	
	MyChaInfo			MyCharacterInfo;

	// EDIT : BS : 070413 : 신규 패킷 암호화		
	// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
	CNM_KEY_TYPE cnmKey;
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE cnmKeyServer;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	ULONG cnmKey;
#endif // #ifdef CRYPT_NET_MSG
/*
	//0729 kwon 스킬 애니메이션 아이디.
	struct SkillAnimationID
	{
		BOOL bIsBuff;
		INDEX idPlayer_Anim_Skill[3];
	};
	
	SkillAnimationID	_SkillAnimID[300];//우선 스킬 300개만 하자.
*/
	void SendLoginMessage(CTString& strID, CTString& strPW, ULONG ulVersion);
	void SendSecurityMessage(CTString& strSecurity);							//070901_ttos: 보안카드 UI
	void SendDeleteCharacter(ULONG ulIndex, BYTE del = FALSE);
	void SendDeleteCharacter(ULONG ulIndex, CTString& strSecuNum, BYTE del);
	void SendGameStart(ULONG ulIndex);
	void SendStopChange();
	void SendStopEvocation();
	void SendCreateCharacter( CTString& strName, UBYTE ubJob, UBYTE ubHair, UBYTE ubFace );

	void SendItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lIndex, LONG lEtc );
	// 타겟이 필요한 아이템 사용
	void SendtargetItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lIndex, SBYTE sbType, LONG lMobIndex );	
	// 결합 주문서 사용
	void SendMixItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lMixItemidx, LONG lCashItemIdx, LONG lgameItemIdx);
	// 분리 주문서 사용
	void SendUnMixItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lMixItemidx, LONG lCashItemIdx);
	
	// Inventory
	void SetMyCurrentWearing(int tabId, int rowId, int colId);
	void DeleteMyCurrentWearing(int weartype);
	void UseSlotItem( int tabId, int rowId, int colId, SBYTE sbWearType = -1 );
	void DropItem(int tabId, int rowId, int colId, SQUAD cnt =1);
	void SwapItem(int tabId, int rowId, int colId, int rowId2, int colId2);
	void ArrangeItem(int tab);
	BOOL IsExtensionState( int iJob, CItemData& ID );
	BOOL HasItem( int iIndex );

	bool IsLord();

	// yjpark |<--
	// Teleport
	void SendWarpCancel();
// [KH_070316] 변경 프리미엄 메모리 관련
	void SendTeleportWrite( UBYTE sendMSG, UBYTE ubSlot, CTString &strComment );
	void SendTeleportMove( UBYTE sendMSG, UBYTE ubSlot );
	void SendWarpTeleport( int iTeleportIndex );
	// Learn skill
	void SendSkillLearn( SLONG slIndex );
	void SendSSkillLearn( SLONG slIndex );
	// Stat point
	void SendUseStatPoint( UBYTE ubStatType );

	// HP, MP 회복
	void SendRecoverHPMP( SBYTE sbHPCount, SBYTE sbMPCount );
	void SendRecoverItemSeal( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, SLONG slIndex);

	// 이벤트 보상.
	void SendEventPrize();

	// 서버에 보물의 아이템 지급 목록을 얻음.
	void SendEventTreasureList();

	// 서버로 보물상자를 열겠다고 메세지를 보냄.
	void SendEventOpenTreasure();
	
	// 크리스마스,새해 이벤트 
	void SendEventNewyear(int tv_event);

// [KH_070413] 스승의날 이벤트 관련 추가
	void SendEventMaster();

	// Upgrade
	void UpgradeItem( SBYTE sbRow1, SBYTE sbCol1, SLONG slPlus, SBYTE sbRow2, SBYTE sbCol2, SLONG slLevel, INDEX iProtect =-1, SBYTE sbRow3=-1, SBYTE sbCol3=-1);
	
	// Item Level Down
	void ItemLevelDown(SBYTE sbRow1, SBYTE sbCol1, SBYTE sbRow2, SBYTE sbCol2);

	// 블러드 아이템 & 정화석.
	void OptionAddItem( SBYTE sbRow1, SBYTE sbCol1, SBYTE slWearPos, SBYTE sbRow2, SBYTE sbCol2 );
	void OptionDelItem( SBYTE sbRow1, SBYTE sbCol1, SBYTE slWearPos, SBYTE sbRow2, SBYTE sbCol2 );
	
	// Refine
	void RefineReq( SBYTE sbRow, SBYTE sbCol );

	// Process
	void ProcessReq( SBYTE sbRow, SBYTE sbCol, SLONG slResultIndex, SLONG slResultCount, CNeedItems* NeedItems );

	// 이기환 추가 ( 12. 6 ) : 제조 메세지 
	void ProductReq( SBYTE sbRow, SBYTE sbCol, SLONG slResultIndex, SLONG slResultCount, CNeedItems* NeedItems );

	// 이기환 추가 ( 12. 8 ) : 조합 메세지 
	void MixReq( SBYTE sbTextRow, SBYTE sbTextCol, SBYTE* sbRow, SBYTE* sbCol );
	
	// Date : 2005-01-12,   By Lee Ki-hwan
	void CompoundReq( SBYTE sbTextRow, SBYTE sbTextCol, SBYTE* sbRow, SBYTE* sbCol );
	/****   WareHouse **********************************************************/
	void	SendWareHouseChangePassword( const CTString& strOld, const CTString& strNew );
	void	SendWareHouseIsSetPassword();
	void	SendWareHouseSeal();
	void	SendWareHouseListReq( const CTString& strPW );
	void	SendWareHouseDeletePassWord( const CTString& strID );
	void	SendWareHouseCheckPassWord( const CTString& strPW );
	void	SendWareHouseSetupPassWord( const CTString& strPW );
	void	SendWareHouseSetupPassWord( const CTString& strPW ,const CTString& strUNLOCK);

	/****   Guild   ***********************************************************/
	void GuildJoin( SLONG slGuildIndex, SLONG slSrcIndex, SLONG slDestIndex );				// 가입 신청
	void GuildQuit( );									// 탈퇴 신청

	void GuildCreate( const CTString &strGuildName );	// 길드 생성
	void GuildUpgrade();								// 길드 승급
	void GuildDestroy();								// 길드 해체
	void GuildChangeBoss( SLONG slDestIndex );			// 단장 이임
	void GuildAddViceBoss( SLONG slDestIndex );			// 부단장 임명
	void GuildDelViceBoss( SLONG slDestIndex );			// 부단장 해임
	void GuildMemberFire( SLONG slDestIndex );			// 멤버 퇴출

	void GuildApplicantAccept( SLONG slDestIndex );		// 가입 승인
	void GuildApplicantReject( SBYTE sbWhoCancel );		// 가입 거부

	// Date : 2005-03-18,   By Lee Ki-hwan
	void GBReq( SLONG nCharIndex, SLONG nPrize, SLONG nTime );	
	void GBReqReject();	
	void GBReqAccept();	
	void GBStopReq( SLONG nCharIndex );		
	void GBStopReqReject();	
	void GBStopReqAccept();
	/****   Teach   ***********************************************************/
	void TeachTeacherRegister();			// 선생 목록에 추가
	void TeachTeacherCancelRegister();		// 선생 목록에서 빼줘
	void TeachRefreshTeacherList();			// 후견인 목록 갱신
	void TeachTeacherRequest( SLONG slCharIndex, CTString& strName );				// 선생이 되어달라고 요청함.
	void TeachTeacherReject( BOOL bStudent, SLONG slCharIndex, CTString& strName );	// 선생 안할래.
	void TeachTeacherAccept( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName );	// 선생 해줄께.
	void TeachTeacherGiveUp( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName );	// 선생 필요없어~!!!

	/****   ChangeJob ***********************************************************/
	void ChangeJobReq( SBYTE sbJob );		// 전직 요청
	void ChangeJobGiveUp();					// 직업 포기

	/****   ChangeWeapon ********************************************************/
	void ChangeWeaponReq( SBYTE sbRow, SBYTE sbCol, LONG lItemIndex, LONG lChangeType );
	void ChangeWeaponEventReq( LONG lItemIndex, LONG lChangeType );

	// 스탯 초기화
	void StatReset( int iStr, int iDex, int iInt, int iCon );

	// 무기 교환 이벤트
	void SendChangeWeaponEvent();

	/****   MoonStone   ********************************************************/
	void SendMoonStoneStartReq();
	void SendMoonStoneTryReq();
	void SendMoonStoneResultReq();

	/****   MoonStone   ********************************************************/
	void SendCashMoonStoneReq(CUIButtonEx slotBtn);
	void SendCashMoonStoneReward();
	/****   Pet System   ********************************************************/
	void SendPetChangeRide();
	void SendPetSkillInit();
	void SendPetWarpTown();
	void SendPetDestruction();	// eons	펫 소멸
	void SendPetItemMix( SLONG slPetItemIndex, SLONG slmethod );		// eons 펫 조합
	void SendPetRebirth( SLONG slPetIndex );		// eons 펫 봉인 해제 요청 메세지
	void SendWildPetRebirth( int nRow, int nCol);
	void SendPetitemInfo(SLONG slOwnerIndex,int nitemIndex);		//인벤의 펫 아이템 정보받

	/****   Exchange   ********************************************************/
	void ExchangeReq_Req( SLONG slDestIndex, CTString &strDestName );
	void ExchangeReq_Rep();
	void ExchangeReq_Rej();
	void ExchangeReq_Ready();
	void ExchangeReq_Ok();
	void ExchangeItem_Add( int nUniIndex, SQUAD llCount );
	void ExchangeItem_Del( int nUniIndex, SQUAD llCount );
	// Quick slot
	void AddQuickSlot( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 );
	void SwapQuickSlot( int nPage, int nSlotNum1, int nSlotNum2 );
	// Party
	void PartyInvite( SBYTE sbType, SLONG slIndex );
	void PartyAllow();
	void PartyReject();
	void PartyQuit();
	void PartyKick( SLONG slIndex );
	// Item plus effect
	void ItemPlusEffectReq( SBYTE sbOption );
	// yjpark     -->|

	// 라카 기자단 혜댁 이벤트
	void SendPresscorpsMessage(ULONG ulItemIndex, CTString Sendstr);
	
	void SendUsingSpeedHack(void); // 스피드핵 사용자 통보 
	
	// PLAYTIME 10분간격으로 전달
	void SendUpdatePlayTime(ULONG nTime);
	void SendInitSSkillReq(void);
//안태훈 수정 시작	//(Zone Change System)(0.1)
	void GoZone(int zone, int extra, int NpcIdx = -1 );
//안태훈 수정 끝	//(Zone Change System)(0.1)

//안태훈 수정 시작	//(Teleport System)(0.1)
	void WriteCurrentPos(int slot, const char *szComment);
//안태훈 수정 끝	//(Teleport System)(0.1)
//안태훈 수정 시작	//(GM Command)(0.1)
	//-------------------Item Data---------------------------
	inline CItemData&	GetItemData(int iIndex)
	{
		if( iIndex > wo_iNumOfItem || iIndex < 0 )
		{
//			ASSERTALWAYS("Invalid Item Index!!!");
			return wo_aItemData[0];
		}		
		return wo_aItemData[iIndex];
	}

	inline const char* GetItemName(int iIndex)
	{
		if( iIndex > wo_iNumOfItem || iIndex < 0 )
		{
			ASSERTALWAYS("Invalid Item Index!!!");
			return wo_aItemName[0].GetName();
		}		
		return wo_aItemName[iIndex].GetName();
		/*
		if( iIndex > wo_iNumOfItem || iIndex < 0 )
		{
			ASSERTALWAYS("Invalid Item Index!!!");
			return wo_aItemData[0].GetName();
		}		
		return wo_aItemData[iIndex].GetName();
		*/
	}

	inline const char* GetItemDesc(int iIndex)
	{
		if( iIndex > wo_iNumOfItem || iIndex < 0 )
		{
			ASSERTALWAYS("Invalid Item Index!!!");
			return wo_aItemName[0].GetDesc();
		}		
		return wo_aItemName[iIndex].GetDesc();
		/*
		if( iIndex > wo_iNumOfItem || iIndex < 0 )
		{
			ASSERTALWAYS("Invalid Item Index!!!");
			return wo_aItemData[0].GetDesc();
		}		
		return wo_aItemData[iIndex].GetDesc();
		*/
	}

	//------------------Skill Data---------------------------
	inline CSkill	&GetSkillData( const int iIndex )
	{
		if( iIndex > wo_iNumOfSkill || iIndex < 0 )
		{
			ASSERTALWAYS("Invalid Skill Index!!!");
			return ga_World.wo_aSkillData[0];
		}
		return ga_World.wo_aSkillData[iIndex];
	}

	inline CSpecialSkill &GetSSkillData(const int iIndex)
	{
		if(iIndex > wo_iNumOfSSkill || iIndex < 0)
		{
			ASSERTALWAYS("Invalid Special Skill Index!!!");
			return ga_World.wo_aSSkillData[0];
		}
		return ga_World.wo_aSSkillData[iIndex];
	}

	inline CShopData&	GetShopData(const int iIndex)
	{
		if(iIndex > ga_World.wo_iNumOfShop || iIndex < 0)
		{
			ASSERTALWAYS("Invalid Shop Index!!!");
			return ga_World.wo_aShopData[0];
		}
		return ga_World.wo_aShopData[iIndex];
	}

	//-------------------Cash Shop Data-----------------------
	// wooss 050903
	inline CCashShopData& GetCashShopData(const int iIndex)
	{
		if( iIndex < 0 ) 
		{
			ASSERTALWAYS("Invalid Shop Index!!!");
			return ga_World.wo_aCashShopData[0];
		}
		return ga_World.wo_aCashShopData[iIndex];
	}

	//-------------------Mob Data-----------------------------
	inline CMobData&	GetMobData(int iIndex)
	{
		if(iIndex-1 > ga_World.wo_iNumOfNPC || iIndex < 0)
		{
	//		ASSERTALWAYS("Invalid Mob Index!!!");
			return ga_World.wo_aMobData[0];
		}
		return ga_World.wo_aMobData[iIndex];
	}

	inline const char*	GetMobName(int iIndex)
	{
		if(iIndex-1 > wo_iNumOfMobName || iIndex < 0)
		{
			ASSERTALWAYS("Invalid Mob Name Index!!!");
			return ga_World.wo_aMobName[0].GetName();
		}
		return ga_World.wo_aMobName[iIndex].GetName();
	}

	inline COptionData&	GetOptionData(const int iOptionType)
	{
		if(iOptionType > wo_iNumOfOption || iOptionType < 0)
		{
			ASSERTALWAYS("Invalid Option Index!!!");
			return wo_aOptionData[0];
		}
		return wo_aOptionData[iOptionType];
	}
	//----------------Action Data-----------------------------
	inline CAction& GetActionData(int iIndex)
	{
		if(iIndex > wo_iNumOfAction || iIndex < 0)
		{
			ASSERTALWAYS("Invalid Action Index!!!");
			return ga_World.wo_aActionData[0];
		}
		return ga_World.wo_aActionData[iIndex];
	}

	//-------------------Rare Option---------------------------
	inline CItemRareOption&	GetRareOptionData(int iIndex)
	{
		if( iIndex > wo_iNumOfRareOption || iIndex < 0 )
		{
			//ASSERTALWAYS("Invalid RareOption Index!!!");
			return wo_vecItemRareOption[0];
		}		
		return wo_vecItemRareOption[iIndex];
	}

	void SendLeaseItem(int iItemIdx);
	void SendEventItem(int iItemIdx, int iItemcont);
	void BuyItem(int iShopID, int iNumOfItem, __int64 iTotalPrice);			// 상점이 팔고 있는 아이템을 구입함.
	void SellItem(int iShopID, int iNumOfItem, __int64 iTotalPrice);		// 자신이 가지고 있는 아이템을 판매함.
	void FieldShopBuyItem( int iNumOfItem, __int64 iTotalPrice);			// ttos : 잡화상 이용 주문서 사용시
	void FieldShopSellItem(int iNumOfItem, __int64 iTotalPrice);			// ttos : 잡화상 이용 주문서 사용시
	void SendWhoAmI();
	void SendGMCommand(const char *szCommand);

// 강동민 수정 시작		// 2nd Single Dungeon
	void AddRescueNPC(CEntity* pEntity);	
	void UpdateRescueNPC(CEntity* pEntity, int iHP, int iMP);	
	void EndRescueNPC();
// 강동민 수정 끝		// 2nd Single Dungeon

// 강동민 수정 시작
	// NOTE : 메세지를 보낸 시간.
	// NOTE : 타임 아웃을 체크하기 위한 부분.
	unsigned int	m_uiSendedTime;		// 메세지를 보냈을때의 시간.
	BOOL			m_bSendMessage;		// 메세지를 보냈으며(TRUE), 그에 대한 응답을 받으면(FALSE)
// 강동민 수정 끝
	UBYTE	m_ubGMLevel;
//안태훈 수정 끝	//(GM Command)(0.1)
	
	//0701 kwon	
	SLONG SearchItemIndex(ULONG Index);
	SLONG SearchClientChaIndex(ULONG Index);
	SLONG SearchClientMobIndex(ULONG Index);
	SLONG SearchClientPetIndex(ULONG Index);
	SLONG SearchClientSlaveIndex(ULONG Index);

	BOOL SearchEntityByNetworkID( long lIndex, SBYTE sbType, CEntity* &pEntity );
	BOOL SearchEntityByNetworkID( long lIndex, SBYTE sbType, CEntity* &pEntity, INDEX& cnt);
	void SendPickMessage( CEntity *pEntity, ULONG ItemIndex, BOOL bLayerCheck = TRUE );
	void SendStopMessage(CEntity *pEntity, CPlacement3D MyPlacement);
	void SendMoveMessage(CEntity *pEntity, CPlacement3D plPlacement, SLONG speed);
	void SendAttackMessage(CEntity *pAttackerEntity, CEntity *pTargetEntity, BOOL bPvp=FALSE);	
	void SendAttackSymbol();
	void SendProduceMessage(ULONG Index, SBYTE sbKind);
	void SendCollectProduceMessage(LONG npc_client_index);
	void SendSelectProduceMessage(LONG npc_client_index, LONG item_db_index);
	void SendNPCProcessMessage(LONG processItemDBIndex, LONG count);
	void SendBoosterUpgrade(LONG itemServerIndex);
	void SendCancelSkillMessage();
	void SendAttackMessageInContainer(CSelectedEntities& dcEntities);
	void SendSkillMessage(int nSkillIndex, CEntity *pEntity, int nTargetIndex, BOOL bFire, UBYTE ubMove=0);
	void SendPetSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire);
	void SendPetCommandMessage( int nSkillIndex, CEntity *pSourceEntity );

	// 소환수의 스킬 메세지.
	void SendSlaveSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire);
	void SendSlaveSkillMessageInContainer(int nSkillIndex, CEntity *pSourceEntity, CSelectedEntities &dcEntities, BOOL bFire);
	
	void SendSkillMessageInContainer(int nSkillIndex, CSelectedEntities& dcEntities, BOOL bFire);	
	void InitSkillID();
	void SetMyPosition(CPlacement3D plPlacement, FLOAT camera_angle);

	// 0 = SYSMSG_NORMAL
	void ClientSystemMessage( CTString &strSysMessage, int nSysType = 0 );
	void SendRebirthMessage();
	void DeleteAllMob();
	
	//wooss 050805
	//확장된 리버스 메시지 
	void SendRebirthMessageEx(int, BOOL ,BOOL);
	//wooss 050808
	//send warp message
	void SendWarpItemMessage(UBYTE warpType,CTString charName=CTString(""),BOOL allow=FALSE);
	void SendProlongMessage(int tabId, int rowId, int colId);
	//wooss 050817
	//send chang name message
	void SendChangMyName(CTString strMsg);
	void SendChangGuildName(CTString strMsg);
	//wooss 050820
	void SendExSlotMessage(int slotNum);
	//wooss 051031 
	//send 2pan4pan event message
	void SendEvent24(int sel,SBYTE eventType);
	
	// send find friend 060201
	void SendFindFriend(int tv_event, void * tv_input = NULL);
	void SendCashItemMessage(int nType);

	void SendActionMessage(SBYTE action_type, SBYTE action_id, SBYTE state, CEntityPointer epTarget = NULL );
	void PickItemAround();
	void DelMobTarget(ULONG ClientIndex);
	void ResetMobStatus(ULONG ClientIndex);
	void DelChaTarget(ULONG ClientIndex);
	void ResetChaStatus(ULONG ClientIndex);

	void SendQuestMessage(UBYTE msgQuestType, INDEX data);
	void SendQuestPrizeMessage(UBYTE msgType, INDEX iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus);
	void SendSkillCancelMessage();

	// Ranking
	void Ranking_RequestList( SBYTE sbJob );
	void Ranking_RequestPrizeList();
	void Ranking_Prize();

	// 싱글던젼에서 메세지를 묶어서 보내기 위해서 필요한 부분.
	void AddAttackList( UBYTE ubAttackType, INDEX iAttackIndex, UBYTE ubTargetType, INDEX iTargetIndex );
	void SendAttackList();
	void ClearAttackList();

	void AddMoveList( INDEX iIndex, FLOAT fX, FLOAT fZ, FLOAT FH, FLOAT fAngle );
	// EDIT : BS : BEGIN
	void AddMoveList(CEntity &en);
	void SendMoveList(bool bForce = false);
	// EDIT : BS : END
	void ClearMoveList();

	void AddRegenList(INDEX iIndex, INDEX iMobType, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fAngle, INDEX iYLayer, INDEX iEntityID);
	void SendRegenList();
	void ClearRegenList();

	// 정당방위 리스트 관리
	void AddLegitList(int Index);
	BOOL SearchLegitList(int Index, BOOL bDel = FALSE);
	void DelLegitList(int Index);

	// 멀티 타겟을 위해서 범위내의 적들을 모두 찾아냅니다.
	void FindTargetsInRange(
								 CEntity* pPlayer, 
								 CEntity* pCenter,
								 CSelectedEntities &cen, 
								 FLOAT fFallOffRange, 
								 INDEX iMaxEnemies,
								 FLOAT fAngle = 360.0f,
								 INDEX iSkillIndex = -1,
								 INDEX	iTargetType = CSkill::STT_TARGET_D120 );
	
	void FindTargetsInRangeEx(CEntity* pCenter,	CSelectedEntities& cen, FLOAT fFallOffRange, INDEX iMaxEnemies);

	void CheckAttackTarget( INDEX iSkillIndex, CEntity* pTargetEntity, FLOAT fDist = 0.0f, BOOL bElementalAttack = FALSE ); 
	// 스킬 공격을 할수 있는지 판단합니다.
	BOOL CheckSkillAttack( INDEX iSkillIndex, CEntity* pEntity );
	
	// Date : 2005-05-06(오후 11:32:39), By Lee Ki-hwan
	void BillInfoSectionListReq();
	void BillInfoUserInfoReq();			
	void BillInfoPayReserveReq();
	void BillInfoPayOtherReq( CTString strGuid );
	void BillItemListReq();		// 유료화 지급 아이템 리스트 요청
	void BillItemReceiveReq();	// 유료화 지급 아이템 요청( To Inventory )

	// Date : 2005-05-19(오후 8:25:50), By Lee Ki-hwan
	void MgrRegistReq( int nCharIndex, CTString strCharName );
	void MgrRegistAllow( int nCharIndex, CTString strReqCharName );
	void MgrSetCondition( int nCharIndex, int nCondition );
//	void MgrChatting();
	void MgrRegistCancel();
	void MgrRegistCancel(int nCharIndex, CTString strReqName);
	void MgrDeleteMember( int nCharIndex, int nTargetIndex, CTString strName );
	void SendFriendCatting( int nCharIndex, CTString strName, CTString strTargetName, CTString strMessage );
	//메신저 추가 기능
	void MgrFriendInvite( int nCharIndex, int nChatIndex, int nTargetIndex);
	void MgrFriendOut( int nCharIndex, int nChatIndex );
	void MgrFriendChat( int nCharIndex, int nChatIndex, CTString strChat);
	void MgrBlock( CTString strTarget );
	void MgrUnBlock( int nCharIndex);
	void MgrGroupAdd( CTString strName);
	void MgrGroupDel( int nGroupIndex);
	void MgrGroupRename( int nGroupIndex, CTString strNewName);
	void MgrGroupMove( int nGroupIndex, int nCharIndex);
	void MgrSetChatColor( int nColIndex);
	void MgrOneVsOneConnect( int nCharIndex, int nOtherCharIndex, CTString strChat );
	
	// Date : 2005-07-06(오후 2:36:01), By Lee Ki-hwan
	//  공성 관련 메세지 
	void SetTimeReq( int nDay, int nHour ); // 공성시간 설정 요청
	void GetTimeReq();						// 공성 시간 확인 요청
	void AttackReq();						// 공성 신청 
	void DefenseGuildReq();					// 수성 신청 
	void AttackCharReq();					// 공성 용병신청
	void AttackGuildReq();					// 수성 용병 신청 
	void WarItemMixReq( SBYTE* sbRow, SBYTE* sbCol ); // 공성 아이템 조합 신청 

	// 애완동물 관련...
	void CallPet( LONG lIndex );													// 펫을 호출함.
	void CheckPetType( SBYTE sbPetTypeGrade, INDEX &iPetType, INDEX &iPetAge );		// 펫 타입과 나이를 얻음.
	void RidePet( CEntity *pCharacter, CEntity *pPet, INDEX iPetType );		// 애완동물을 타도록 처리.
	void LeavePet( CEntity *pCharacter );									// 애완동물에서 내리도록 처리.
	void UpdatePetTargetInfo( INDEX iPetIndex );						// 애완동물의 정보를 갱신함.
	void ClearPetList();
	void ChangePetMount();												// 애완동물을 성체로 변경요청...
	void SendMovingGuildWarArea();

	// Date : 2005-09-06(오전 11:22:30), By Lee Ki-hwan
	void SendGuildStashHistroyReq();
	void SendGuildStashViewReq();
	void SendGuildStashTakeReq( LONGLONG llMoney );
	
	// Date : 2005-09-08(오후 5:59:52), By Lee Ki-hwan
	void SendChuseokUpgrade();
	void SendChuseokExchange();

	///////////////////////////////////////////////////////////////////////////////////
	// 2006 추석 이벤트 :Su-won	|---------->
	
	//송편 만들기
	void Send2006ChuseokRicecakeMake();
	void Send2006ChuseokRainbowMake();
	//오색송편 보상품으로 교환하기
	void Send2006ChuseokExchange();
	
	// 2006 추석 이벤트 :Su-won	<----------|
	///////////////////////////////////////////////////////////////////////////////////
	
	// 2006 X-Mase Event [12/12/2006 Theodoric]
	void Send2006XMasEvent( SLONG nCakeCount);
		
	void SendSuperStoneReq();
// 060118 wooss japan package item....
	void SendCouponItemReq(CTString strNum);
// 060223 wooss japan connect item....
	void SendConnectItemReq();
// 060321 eons japan clothes exchange event
	void SendClothesExchange( int nValue );

// 060601 wooss japan rainyday event
	void SendRainyDayExchange();

	void Send2p4pLetter( LONG lGameIndex );

	// 060306 wooss Party Recall CashItem
	void SendPartyRecallConfirm(CTString tmp_str,LONG tmp_idx,BOOL tmp_answer);
	// Date : 2006-04-27(오후 2:36:02), By eons
	void SendNetCafeOpenBox( void );

	// Date : 2006-11-16(오전 11:08:04), By eons
	void SendUsaOpenBetaGift(void);

	// Date : 2006-05-09(오후 5:08:22), By eons
	//파티 매칭 관련
	void SendPartyRegistNormal( int nType );
	void SendPartyRegistParty( CTString strComment, DWORD JobFlag, int LimitLevel );
	void SendPartyListReq( int nPage, int nJob, int nLimitLv, int nPtType, BOOL bState );
	void SendPartyRegistDelete( void );
	void SendPartyInviteReq( int nCharIndex );
	void SendPartyJoinReq( int nBossIndex );
	void SendPartyJoinAllow( int nReqIndex );
	void SendPartyJoinReject( int nReqIndex );
	
	// 060601	wooss worldcup event 1
	void SendWorldCupEvent( int msgCmd, int iSelCountry );
	void SendWorldCupGoldenBallEvent( int nSendType, int nTeamAScore, int nTeamBScore );

	// Date : 2006-06-27(오전 10:42:39), By eons
	void SendBuddhismEvent( BYTE nType );

	// Date : 2006-06-28(오후 4:18:06), By eons
	// 곤충 채집 이벤트
	void SendBuyCollectBox( void );
	void SendGiftCollectBox( void );
	void SendDropInsect( int nInsect );

	// 러브 러브 이벤트( 2007 발렌타인 데이 )
	void SendSaveBingoItem(BYTE nPos, int nGiftItemIndex, int nBoxItemIndex);
	void SendReqGiftChocoBox(int nUniItemIndex);

	int		m_iLastError;

	void LearnPetSkill( LONG nSkillIndex ); 
	void LearnWildPetSkill(LONG nSkillIndex);
	void MandateBossReq( CTString strMandateCharName );
	void SendCastleMapRecent();
	void SendCastleMapSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex );

	void CreateSlave( int iOwnerIndex, CEntity* pEntity, int iType );
	void SendGameStart();
	void SetLoadingZone( LONG lZone = -1 );
	LONG GetLoadingZone();

	// check point for dungeon quest
	void SetCheckPointData(int questIdx,int chkNumFlag, int maxChkflag=-1);

	// wooss 070307 --------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	void SendWhiteday2007(UBYTE ubType);
	void SendWhiteday2007Letter(int itemIdx,COLOR colMsg,CTString tStr);
	// ----------------------------------------------------------------<<

	// WSS_GUILDMASTER 070416 ----------------------------------------->>
	void SendRequestParentsday(int reqIdx);
	void SendParentsdayAddPoint(int charIdx);
	// ----------------------------------------------------------------<<		
	
#ifndef NO_GAMEGUARD
	// WSS_NPROTECT 070402 -------------------------------------------->>
    void SendnProtectAuth2(DWORD dwArg);
	BOOL Init_nProtect();
	void SetHwnd_nProtect(HWND _hwndMain);
	void Check_nProtect();
	BOOL NPGameMonCallbackErrChk( DWORD dwMsg, DWORD dwArg);
	// ----------------------------------------------------------------<<		
#endif
	// WSS_MINIGAME 070420 -------------------------------------------->>
	void SendMinigameDefaultMessage(UBYTE subType);
	void SendMinigameSelect(UBYTE cSelect);
	void SendMinigameSelectGift(ULONG cSelect);
	// ----------------------------------------------------------------<<

	/*** FLOWERTREE2007 ****************************************************/
	void SendFlowerTreeReq(UBYTE subType);
	/***********************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// 피닉스 이벤트

	void SendPhoenixCharacterCondition();		//피닉스 캐릭터 생성 가능여부
	void SendCreatePhoenixCharacter();	//피닉스 캐릭터 생성
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 경매 시스템

	void SendTradeAgentSearchReq(int nPageNo, int nitemType,int nitemSubType,int nJobClass,CTString stSearchWord, int nAlignType = -1);
	void SendTradeAgentBuyReq(int nListidx, LONGLONG nTotalMoney);
	void SendTradeAgentRegListReq(int nPageNo, int nAlignType = -1);
	void SendTradeAgentCalcListReq(int nPageNo, int nAlignType = -1);
	void SendTradeAgentRegCancelReq(int nListidx);
	void SendTradeAgentCalculateReq();
	void SendTradeAgentRegReq(char iTab, char iRow, char iCol,int iIndex, int nCount, LONGLONG nTotalMoney, LONGLONG nDepositMoney);
	void SendTradeAgentCheckCalcReq();
	//////////////////////////////////////////////////////////////////////////


	// [070613: Su-won]
	// 무기,방어구 교환카드 사용 요청
	void UseChangeWeaponItem( SBYTE sbRow, SBYTE sbCol, LONG lItemIndex, LONG lChangeType );
	// 펫 명찰 아이템 사용 요청
	void SendPetNameChageReq( int nPetIndex, CTString strPetName );

	/////////////////////////////////////////////////////////////////////////////
	// [070705: Su-won] EVENT_SUMMBER_2007
	void SendPaperFoldingReq(int nItemIndex0, int nItemIndex1, int nItemIndex2);
	void SendPaperBlendingReq(int nItemIndex);
	// [070705: Su-won] EVENT_SUMMBER_2007
	/////////////////////////////////////////////////////////////////////////////

	// Eons 070905
	void SendBJMono2007Req(UBYTE subType, ULONG ulNum=0); // SK BJ Mono Event
	void SendLC1000DayHatUpgradeReq(SBYTE sbRow, SBYTE sbCol, LONG lItemIndex); // 1000일 이벤트 기념모자 업그레이드
	void SendLC1000DayTakeHatReq();
	void SendTakeFireCracker();

	// [070807: Su-won] EVENT_ADULT_OPEN
	// 성인서버 오픈 이벤트 관련 보상 요청
	void SendAdultOpenEventReq( int iEvent, int tv_tab=-1, int tv_row=-1, int tv_col=-1);
	
	// WSS_TG2007 2007/09/14
	// 스크린샷 이벤트
	void SendTG2007ScreenshotReq();
	void SendTG2007RichYearReq(int subType);
	
	// Eons 071012 할로윈 이벤트 2007
	void SendHalloween2007Req(UBYTE subType);
	
	// [071122: Su-won] NEW_MOONSTONE
	void SendMoonStoneMix(INDEX iMoonStone, INDEX iMoonStoneBox);

	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	void SendDratanSiegeDungeonMSG(UBYTE ubSubType, UBYTE ubSubSubType =-1, int nValue =-1);
	
	////////////////////////////////////////////////////////////
	void SendCashPersonShop(UBYTE subType);

	// Eons 071204
	void SendXMAS2007DecoReq(UBYTE subType);
#ifdef RESTART_GAME
	void SendRestartGame();
#endif
	void SendUseGoDungeon(INDEX iItemIndex, INDEX iZone, INDEX iExtra);

	void SendItemDelete(SBYTE iTab, SBYTE iRow, SBYTE iCol, INDEX UniIndex, SQUAD count);
	//공격 펫 장비 아이템 사용시
	void SendUseWildPetItem(int posId, int tabId, int rowId, int colId, int item_index);
	
	void SendHanaposEvent();

	// Temp Message str, int
	// add wooss 060306 for message argument
	CTString	m_tmp_str;
	LONG		m_tmp_idx;

};					

// pointer to global instance of the only network object in the application
ENGINE_API extern CNetworkLibrary *_pNetwork;

// convert string address to a number
ENGINE_API extern ULONG StringToAddress(const CTString &strAddress);
// convert address to a printable string
ENGINE_API extern CTString AddressToString(ULONG ulHost);

#endif  /* include-once check. */

