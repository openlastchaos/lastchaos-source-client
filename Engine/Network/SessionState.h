#ifndef SE_INCL_SESSIONSTATE_H
#define SE_INCL_SESSIONSTATE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Synchronization.h>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Network/Common.h>
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Network/ItemTarget.h>
#include <Engine/Network/SessionSocket.h>
#include <Engine/Base/Timer.h>
//0105
#include <Engine/Network/Cmd.h>
#include <Engine/GlobalDefinition.h>

/*
 * Session state, manipulates local copy of the world
 */
class ENGINE_API CSessionState {
public:
//  CStaticArray<CPlayerTarget> ses_apltPlayers; // client targets for all players in game

  CTString ses_strMOTD;               // MOTD as sent from the server
  INDEX ses_iLastProcessedSequence;   // sequence of last processed stream block
  CNetworkStream ses_nsGameStream;    // stream of blocks from server

  // lerp params
  CTimerValue ses_tvInitialization;  // exact moment when the session state was started
  TIME ses_tmInitializationTick;     // tick when the session state was started
  // secondary lerp params for non-predicted movement
  CTimerValue ses_tvInitialization2;  // exact moment when the session state was started
  TIME ses_tmInitializationTick2;     // tick when the session state was started

  TIME ses_tmLastProcessedTick;      // last tick when all actions were processed

  INDEX ses_iMissingSequence;       // first missing sequence
  CTimerValue ses_tvResendTime;     // timer for missing sequence retransmission
  TIME ses_tmResendTimeout;         // timeout value for increasing the request interval
  CTimerValue ses_tvMessageReceived;  // exact moment when the session state was started

  TIME ses_tmLastDemoSequence;    // synchronization timer for demo playing
  ULONG ses_ulRandomSeed;         // seed for pseudo-random number generation
  ULONG ses_ulSpawnFlags;         // spawn flags for current game

  BOOL ses_bKeepingUpWithTime;     // set if the session state is keeping up with the time
  TIME ses_tmLastUpdated;
  CListHead ses_lhRememberedLevels;   // list of remembered levels
  BOOL ses_bAllowRandom;            // set while random number generation is valid
  
  BOOL ses_bPause;      // set while game is paused
  BOOL ses_bWantPause;  // set while wanting to have paused
  BOOL ses_bGameFinished;  // set when game has finished
  BOOL ses_bWaitingForServer;        // wait for server after level change
  CTString ses_strDisconnected; // explanation of disconnection or empty string if not disconnected
  UBYTE ses_ubNumLevelChanges; // number of server level changes

  INDEX ses_ctMaxPlayers; // maximum number of players allowed in game
  BOOL ses_bWaitAllPlayers; // if set, wait for all players to join before starting
  FLOAT ses_fRealTimeFactor;  // enables slower or faster time for special effects

  CEMsgBuffer     ses_embReceiveBuffer; // buffer used for receiving tick data from the server
  CEntityMessage  ses_emEntityMessage;  // message object used to decode messages from the buffer
  TIME            ses_tmLastTickTime;    // time of the last tick data block received
  ULONG           ses_ulMsgId; // GORAN: temp - to be removed

  // these variables are used for synchronisation with server time
  double ses_dtmOldTickTime;     
  double ses_dtmOldRealTime;
  double ses_dtmAccumulatedDelta; // accumulated difference between server real time and server tick time (detects server stalls)
  BOOL ses_bRestartLocalTime;     // resets the local time delta

  CSessionSocketParams ses_sspParams; // local copy of server-side parameters
public:
  // network message waiters
  void Start_AtServer_t(void);     // throw char *
  void Start_AtClient_t(INDEX ctLocalPlayers);     // throw char *
  // Set lerping factor for current frame.
  void SetLerpFactor(CTimerValue tvNow);
  // notify entities of level change
  void SendLevelChangeNotification(class CEntityEvent &ee);
  // wait for a stream to come from server
  void WaitStream_t(CTMemoryStream &strmMessage, const CTString &strName, INDEX iMsgCode);
  // check if disconnected
  BOOL IsDisconnected(void);

  // print an incoming chat message to console
  void PrintChatMessage(ULONG ulFrom, const CTString &strFrom, const CTString &strMessage);
public:
  /* Constructor. */
  CSessionState(void);
  /* Destructor. */
  ~CSessionState(void);

  // get a pseudo-random number (safe for network gaming)
  ULONG Rnd(void);

  /* Stop the session state. */
  void Stop(void);
  /* Start session state. */
  void Start_t(INDEX ctLocalPlayers); // throw char *

  // do physics for a game tick
  void HandleMovers(void);
  // let the client move the entities around
  void HandleMovers_client(void);
  // do thinking for a game tick
  void HandleTimers(TIME tmCurrentTick);
  // do thinking for a game tick, on the client
  void HandleClientTimers(TIME tmCurrentTick);
  // do a warm-up run of the world for a few ticks
  void WarmUpWorld(void);
  // reset random number generator (always randomizes to same sequence!)
  void ResetRND(void);
  /* Process a game tick. */
  void ProcessGameTick(CNetworkMessage &nmMessage, TIME tmCurrentTick);
  /* Process a gamestream block. */
  void ProcessGameStreamBlock(CNetworkMessage &nmMessage);
  /* Process all eventual avaliable gamestream blocks. */
  void ProcessGameStream(void);
  /* Get number of active players. */
  INDEX GetPlayersCount(void);
  // attach a player entity to this session 
  void AddPlayer(INDEX iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulEntityID,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX iClient);


  /* Read session state information from a stream. */
  void Read_t(CTStream *pstr,BOOL bNetwork = FALSE);   // throw char *
  void ReadState_t(CTStream *pstr);            // throw char *
  void ReadWorldAndState_t(CTStream *pstr);    // throw char *
  void ReadRememberedLevels_t(CTStream *pstr);   // throw char *
  /* Write session state information into a stream. */
  void Write_t(CTStream *pstr,BOOL bNetwork = FALSE);  // throw char *
  void WriteWorldAndState_t(CTStream *pstr);   // throw char *
  void WriteState_t(CTStream *pstr);           // throw char *
  void WriteRememberedLevels_t(CTStream *pstr);   // throw char *

  // remember current level
  void RememberCurrentLevel(const CTString &strFileName);
  // find a level if it is remembered
  class CRememberedLevel *FindRememberedLevel(const CTString &strFileName);
  // restore some old level
  void RestoreOldLevel(const CTString &strFileName);
  // forget all remembered levels
  void ForgetOldLevels(void);

  /* adjust local time for the remote client */
  void AdjustLocalTime();
  /* Session state loop. */
  void SessionStateLoop(void);
 
  // demo playback tick data processing
//  void RunDemoTick();
  // extract an entity message from the buffer and process it
  BOOL ReceiveMessage();
  // get the tick data from a network message
  void ReceiveTick(TIME tmTickTime,UBYTE* pubData,SLONG slSize);
  // send an acknowledge for a recieved tick
  void SendAcknowledge(TIME tmTickTime);

//0105
  void ProcessOneSentence(char *msg);
  char* AnyOneArg(char *argument, char *first_arg);
  void SkipSpaces(char **string);
  void StartGame();
  char		m_commIn[2048], m_commInTmp[1024], m_commOut[512], m_oneSentence[512];
  CCmd		*m_pCmd;

  void SwapItem( int tab, int row, int col, int tab2, int row2, int col2 );
  
  //0714 kwon
  void ReceiveUIMessage(CNetworkMessage *istr);
  void ReceiveStatMessage(CNetworkMessage *istr);
  void ReceiveSkillMessage(CNetworkMessage *istr);
  void ReceiveSSkillMessage(CNetworkMessage *istr);
  void ReceiveQuickSlotMessage(CNetworkMessage *istr);
  void ReceiveExchangeMessage(CNetworkMessage *istr);
  void ReceiveDamageMessage(CNetworkMessage *istr);
  void ReceiveAttackMessage(CNetworkMessage *istr);
  void ReceiveItemMessage(CNetworkMessage *istr);
  void ReceiveGoZoneMessage(CNetworkMessage *istr);
  void ReceivePartyMessage(CNetworkMessage *istr);
  void ReceiveInventoryMessage(CNetworkMessage *istr);
  void ReceiveGmMessage(CNetworkMessage *istr);
// [KH_070316] 변경 프리미엄 메모리 관련
  void ReceiveMemPosMessage(UBYTE recvMSG, CNetworkMessage *istr);
  void ReceiveGoToMessage(CNetworkMessage *istr);
  void ReceiveMoveMessage(CNetworkMessage *istr);
  void ReceiveStatusMessage(CNetworkMessage *istr);
  void ReceiveAtMessage(CNetworkMessage *istr);
  void ReceiveDisappearMessage(CNetworkMessage *istr);
  void ReceiveAppearMessage(CNetworkMessage *istr);
  
  void ReceiveMobAppearMessage(CNetworkMessage *istr);			// Mob Appear
  void ReceiveCharacterAppearMessage(CNetworkMessage *istr);	// Character Appear
  void ReceivePetAppearMessage(CNetworkMessage *istr,BOOL isNew);
  void ReceiveSummonAppearMessage(CNetworkMessage *istr);		// Summon Appear
  void ReceiveWildPetAppearMessage(CNetworkMessage *istr);		// Wild Pet Appear
  // 로그인 관련 메시지...
  void ReceiveLoginMessage(CNetworkMessage *istr);
  void ReceiveServerListMessage(CNetworkMessage *istr);
  void ReceiveChatMessage(CNetworkMessage *istr);
  void ReceiveNpcRegenMessage(CNetworkMessage *istr);
  void ReceiveSystemMessage( CNetworkMessage *istr );
  void ReceiveRebirthMessage( CNetworkMessage *istr );
  void ReceiveEffectMessage( CNetworkMessage *istr );
  void ReceiveEXPMessage( CNetworkMessage *istr );
  
  void ReceiveActionMessage( CNetworkMessage *istr );
  void ReceiveQuestMessage(CNetworkMessage *istr);
  
  void ReceiveEnvMessage( CNetworkMessage *istr );//1013
  void ReceiveMobStatusMessage( CNetworkMessage *istr );//1013
  void ReceiveAssistMessage( CNetworkMessage *istr );
  void ReceiveWarpMessage(CNetworkMessage *istr);
  void ReceivePkMessage(CNetworkMessage *istr);
  void ReceiveEventMessage(CNetworkMessage *istr);
  void ReceivePersonalShopMessage(CNetworkMessage *istr);
  void ReceiveLegitimateMessage(CNetworkMessage *istr);
  void ReceiveWareHouseMessage(CNetworkMessage *istr);

  void ReceiveGuildMessage(CNetworkMessage *istr);
  void ReceiveTeachMessage(CNetworkMessage *istr);
  void ReceiveChangeJobMessage(CNetworkMessage *istr);  
  void ReceiveBillingMessage( CNetworkMessage *istr ); // Date : 2005-05-06(오후 9:20:21), By Lee Ki-hwan
  void ReceiveMessengerMessage( CNetworkMessage *istr ); 
  void ReceiveMessengerExMessage( CNetworkMessage *istr ); 
  void ReceiveTradeAgentMessage( CNetworkMessage *istr ); //경매 시스템
  //wooss 050818
  void ReceiveExtendMessage(CNetworkMessage *istr);	//확장 메시지
  void ReceiveFailMessage(CNetworkMessage *istr); // 실패 메시지 출력
 
  // eons 061206
  void ReceiveExHairChange(CNetworkMessage *istr); // 산타모자 관련( 헤어 변경 )
  void ReceiveExPlayerStateChange(CNetworkMessage *istr); // 게임서포터 관련( 특정아이템 착용시 이펙트 적용 )
// [KH_070413] 스승의날 이벤트 관련 추가
  void ReceiveEventMaster(CNetworkMessage *istr);
  ///////////////////////////////////////////////////////////////////////////////////

  // WSS_NPROTECT 070402 ------------------------------->>
 #ifndef NO_GAMEGUARD
  void ReceiveExnProtect(CNetworkMessage *istr);
 #endif
  // ---------------------------------------------------<<
  
  void ReceivePetStatusMessage( CNetworkMessage *istr ); 
  void ReceivePetMountMessage( CNetworkMessage *istr );
  void ReceivePetLearnMessage( CNetworkMessage *istr );
  void ReceivePetSkillListMessage( CNetworkMessage *istr );
  void ReceivePetResetSkillMessage( CNetworkMessage *istr );
  void ReceivePetSellInfoMessage( CNetworkMessage *istr );
  void ReceivePetChangeMountMessage( CNetworkMessage *istr );
  void ReceivePetLevelUpMessage( CNetworkMessage *istr );
  void ReceviePetItemMixMessage( CNetworkMessage *istr ); // eons
  void ReceviePetItemChangeMessage( CNetworkMessage *istr ); // eons
  void ReceviePetRebirthMessage( CNetworkMessage *istr ); // eons
  
  void ReceivePartyRecall( CNetworkMessage *istr );  // wooss 060306
  
  void ReceiveElementalStatusMessage( CNetworkMessage *istr );
  void ReceiveElementalDeleteMessage( CNetworkMessage *istr );
  void ReceiveEvocationStart( CNetworkMessage *istr );
  void ReceiveEvocationStop( CNetworkMessage *istr );
  // [070613: Su-won] 펫 명찰 아이템 메시지 처리
  void ReceiveExPetNameChange(CNetworkMessage *istr);

  // [070824: Su-won] PET_COLOR_CHANGE
  void ReceiveExPetColorChange(CNetworkMessage *istr);

  void SetRecentServer(int iRecentGroup, int iRecentServer);
  int GetRecentGroup()	{	return m_iRecentGroup;	}
  int GetRecentServer()	{	return m_iRecentServer;	}

  // EDIT : BS
  void MoveOtherServer(ULONG zone, CTString ip, ULONG port);
  // --- EDIT : BS
  // 판매대행 상인
  void ReceiveCashPersonShopMessage(CNetworkMessage *istr);

  void ReceiveExWildPetMessage(UBYTE index, CNetworkMessage *istr);

protected:
	int		m_iRecentGroup;
	int		m_iRecentServer;
};

BOOL PCStartEffectGroup(const char *szEffectGroupName, SLONG slPCIndex, CEntity *penPC = NULL, float fDelayTime=0.0f);
BOOL ENGINE_API WildPetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet = NULL );

#endif  /* include-once check. */

