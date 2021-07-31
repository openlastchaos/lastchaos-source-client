#include "StdH.h"
#include <Engine/Build.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/ProgressHook.h>
#include <Engine/Base/Timer.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Compression.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Base/MemoryTracking.h>
//#include <Engine/Network/GameSpy.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Precaching.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CEntityClass.h>

#include <Engine/interface/UIManager.h>
#include <Engine/Interface/UIProcess.h>			// 이기환 추가 ( 12. 6 )
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Rendering/Render.h>
#include <Engine/Rendering/Render_internal.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Math/Float.h>

#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/LevelChange.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Entities/LastPositions.h>

#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>

#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CModelInstance.h>

#include <Engine/Network/EntityHashing.h>
#include <Engine/World/WorldEntityHashing.h>
//0105
#include <Engine/Network/tcpipconnection.h>
//0603 kwon 
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>
#include <Engine/Interface/UIManager.h>		// yjpark
#include <Engine/Effect/CTerrainEffect.h>	// yjpark
#include <Engine/JobInfo.h>
#include <Engine/PetInfo.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIQuestBook.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIChatting.h>		// yjpark
#include <Engine/LocalDefine.h>
// WSS_NPROTECT 070402 --------------------------------->>
#ifndef NO_GAMEGUARD
	#include <NPGameLib.h>
	#include <Engine\GameState.h>
	extern ENGINE_API CNPGameLib npgl;
#endif
// -----------------------------------------------------<<

#define MAX_MOVE_LIST		(20)

// pointer to global instance of the only game object in the application
CNetworkLibrary *_pNetwork= NULL;

extern BOOL _bNeedPretouch;
extern BOOL _bMultiPlayer = FALSE;
extern INDEX _ctEntities = 0;
extern BOOL _bSendEnumRequest = FALSE;
extern BOOL _bEnumerating = FALSE;
extern LevelChangePhase _lphCurrent = LCP_NOCHANGE;
extern BOOL con_bCapture; 
extern CTString con_strCapture;

static CWorld *_pwoCurrentWorld = NULL;
static FLOAT _bStartDemoRecordingNextTime = FALSE;
static FLOAT _bStopDemoRecordingNextTime = FALSE;
static INDEX dem_iRecordedNumber = 0;
extern INDEX dem_iAnimFrame = -1;
static INDEX dem_iMotionBlurInterFrames = 0;

// network control
extern INDEX net_bNetworkGame = FALSE;
extern INDEX net_bMmoGame = FALSE;					// by seo-40225
extern INDEX ser_iRememberBehind = 0;
extern INDEX ser_bClientsMayPause = TRUE;
extern INDEX ser_bEnumeration  = TRUE;
//extern INDEX ser_bHeartbeatGameSpy = TRUE;
extern FLOAT ser_tmKeepAlive = 0.1f;
extern FLOAT ser_tmPingUpdate = 3.0f;
extern INDEX ser_bWaitFirstPlayer = 0;
extern INDEX ser_iMaxAllowedBPS = 10000;
extern CTString ser_strIPMask = "";
extern CTString ser_strNameMask = "";
extern INDEX ser_bInverseBanning = FALSE;
extern CTString ser_strMOTD = "";
extern FLOAT ser_fAckDropRate = 0.0f;
extern FLOAT ser_fPositionTreshold = 0.05f;
extern FLOAT ser_fOrientationTreshold = 0.15f;
extern INDEX ser_iSkipTicks = 0;
extern INDEX ser_iMaxBufferSize = SERVER_MAX_BUFFER_SIZE;
extern INDEX ser_iMaxPendingData = 0;
extern INDEX ser_iMaxTotalData = 0;
extern INDEX cli_iSendBehind = 3;
extern INDEX cli_iBufferActions = 10;
extern INDEX cli_iMaxBPS = 75000;
extern INDEX cli_iMinBPS = 0;
extern INDEX cli_iFPSLimit = 30;
extern ENGINE_API INDEX  cli_iCurrentFPS = 10000;

extern FLOAT cli_fTickDropRate = 0.0f;
extern INDEX net_iCompression = 1;
extern INDEX net_bLookupHostNames = FALSE;
extern INDEX net_bReportPackets = FALSE;
extern INDEX net_bReportServerTraffic = FALSE;
extern INDEX net_bReportClientTraffic = TRUE;//FALSE;
extern INDEX net_bReportDemoTraffic = FALSE;
extern INDEX net_iDemoQuality = 128;
extern INDEX net_bLocalPrediction = TRUE;
extern INDEX net_iMaxSendRetries = 10;
extern FLOAT net_fSendRetryWait = 0.5f;
extern INDEX net_bReportTraffic = FALSE;
extern INDEX net_bReportICMPErrors = FALSE;
extern INDEX net_bReportMiscErrors = FALSE;
extern INDEX net_bLerping       = TRUE;
extern INDEX net_iGraphBuffer = 100;
extern INDEX net_bDumpStreamBlocks = 0;
extern INDEX net_bDumpConnectionInfo = 0;

extern INDEX net_iPort = 25600;
extern CTString net_strLocalHost = "";
extern CTString net_strLocationCode = "";
extern CTString net_strConnectPassword = "";
extern CTString net_strAdminPassword = "";
extern CTString net_strVIPPassword = "";
extern CTString net_strObserverPassword = "";
extern INDEX net_iVIPReserve = 0;
extern INDEX net_iMaxObservers = 16;
extern INDEX net_iMaxClients = 0;
extern FLOAT net_tmConnectionTimeout = 60.0f;
extern FLOAT net_tmProblemsTimeout = 5.0f;
extern FLOAT net_tmDisconnectTimeout = 60.0f;
extern INDEX net_bReportCRC = FALSE;
extern FLOAT net_fDropPackets = 0.0f;
extern INDEX net_iHashCompartments = 50;
extern INDEX net_iHashSlotsPerComp = 20;
extern INDEX net_iHashStep = 5;

extern INDEX ent_bReportSpawnInWall = FALSE;
extern FLOAT cmd_tmTick = 0.0f;
extern CTString cmd_cmdOnTick = "";
extern CTString cmd_strChatSender = "";
extern CTString cmd_strChatMessage = "";
extern CTString cmd_cmdOnChat = "";
extern INDEX net_ctChatMessages = 0;  // counter for incoming chat messages

extern CPacketBufferStats _pbsSend;
extern CPacketBufferStats _pbsRecv;

extern INDEX g_iCountry;

class CGatherCRC {
public:
  BOOL bOld;
  CGatherCRC();
  ~CGatherCRC();
};

CGatherCRC::CGatherCRC() {
  bOld = CRCT_bGatherCRCs;
}

CGatherCRC::~CGatherCRC() {
  CRCT_bGatherCRCs = bOld;
}

// precache control
extern INDEX _precache_NONE      = PRECACHE_NONE;
extern INDEX _precache_SMART     = PRECACHE_SMART;
extern INDEX _precache_ALL       = PRECACHE_ALL;
extern INDEX _precache_PARANOIA  = PRECACHE_PARANOIA;
extern INDEX gam_iPrecachePolicy = _precache_ALL;
extern INDEX _precache_bNowPrecaching = FALSE;

extern INDEX dbg_bBreak = FALSE;
extern INDEX gam_bPretouch = FALSE;

extern FLOAT phy_fCollisionCacheAhead  = 5.0f;
extern FLOAT phy_fCollisionCacheAround = 1.5f;
extern INDEX phy_bNonZoningClipMove = TRUE;
extern INDEX phy_bNonZoningClipRay = TRUE;

extern INDEX shd_bCacheAll;
extern void RendererInfo(void);
extern void ClearRenderer(void);
extern void DumpSkaStringToIDTable(void);
extern void DumpShellHashTable(void);

// Check Attack Flag
#define ENF_EX2_PVP				(1L<<0)
#define ENF_EX2_LEGIT			(1L<<1)
#define ENF_EX2_MYPARTY			(1L<<2)
#define ENF_EX2_MYGUILD			(1L<<3)
#define ENF_EX2_ENEMYGUILD		(1L<<4)
#define ENF_EX2_WAR_OURFORCE	(1L<<5)
#define ENF_EX2_WAR_ENEMY		(1L<<6)

// cache all shadowmaps now
extern void CacheShadows(void)
{
	// mute all sounds
	_pSound->Mute();
	CWorld *pwo = (CWorld*)_pShell->GetINDEX("pwoCurrentWorld");
	if( pwo!=NULL) {
		pwo->wo_baBrushes.CacheAllShadowmaps();
		CPrintF( TRANS("All shadows recached"));
		if( shd_bCacheAll) CPrintF(".\n");
		else CPrintF( TRANS(", but not for long.\n(precache all shadows function is disabled)\n"));
	}
	// mark that we need pretouching
	_bNeedPretouch = TRUE;
}

// check if a name or IP matches a mask
extern BOOL MatchesBanMask(const CTString &strString, const CTString &strMask)
{
	CTString strRest = strMask;
	CTString strLine;
	while(strRest!="") {
		strLine = strRest;
		strLine.OnlyFirstLine();
		strRest.RemovePrefix(strLine);
		strRest.DeleteChar(0);
		
		if (strString.Matches(strLine)) {
			return TRUE;
		}
	}
	return FALSE;
}

extern CTString RemoveSubstring(const CTString &strFull, const CTString &strSub);

static void AddIPMask(const CTString &strIP)
{
	ser_strIPMask+= strIP+"\n";
}

static void RemIPMask(const CTString &strIP)
{
	ser_strIPMask = RemoveSubstring(ser_strIPMask, strIP+"\n");
}

static void AddNameMask(const CTString &strName)
{
	ser_strNameMask += strName+"\n";
}

static void RemNameMask(const CTString &strName)
{
	ser_strNameMask = RemoveSubstring(ser_strNameMask, strName+"\n");
}

static void StartDemoRecording(void)
{
	_bStartDemoRecordingNextTime = TRUE;
}

static void StopDemoRecording(void)
{
	_bStopDemoRecordingNextTime = TRUE;
}

void ReinitializeLevel(const CTString &strWorld) {
	if (strWorld.Length() > 0) {
		CWorld wldTemp;
		try {
			wldTemp.Load_t(CTString("Levels\\") + strWorld + CTString(".wld"));
			CPrintF("Reinitialization successfull\n");
		} catch (char *strError) {
			CPrintF("Reinitialization failed. Error: %s\n", strError);
		}
	}
	extern void FreeUnusedStock(void);
	FreeUnusedStock();
}

void DumpPlayer(INDEX iEntityID)
{
	CEntity* penEntity;
	if (!_pNetwork->ga_World.EntityExists(iEntityID,penEntity)) {
		CPrintF(TRANS("Entity with ID %X does not exist!\n"),iEntityID);
		return;
	} if (!penEntity->IsPlayer()) {
		CPrintF(TRANS("Entity with ID %X is not a player!\n"),iEntityID);
		return;
	} else {
		((CPlayerEntity*)penEntity)->DumpToConsole();
	}       
}

void DumpAllPlayers(INDEX iVerbose)
{
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ++iplt) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			if (!iVerbose) {
				CPrintF(TRANS("Entity ID: %X, Name: %s\n"),plt.plt_penPlayerEntity->en_ulID,plt.plt_penPlayerEntity->en_pcCharacter.pc_strName);
			} else {
				plt.plt_penPlayerEntity->DumpToConsole();
				CPrintF("\n");
			}
		}
	}
}

void DumpClassReferences(void)
{
	ULONG aulClasses[2048] = {0};
	
	CPrintF(TRANS("Number of entities of each class:\n\n"));
	FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
		CEntity* penEntity = iten;
		ULONG ulClassID = penEntity->en_pecClass->ec_pdecDLLClass->dec_iID;
		if ((ulClassID < 2048) && (ulClassID >= 0)) {
			aulClasses[ulClassID]++;
		}
	}
  
	for (INDEX iClass=0;iClass < 2048;++iClass) {
		if (aulClasses[iClass]==0) {
			continue;
		}
		EntityClassEntry* peceEntry = FindClassInContainer(iClass);
		CPrintF(TRANS("%d  \t%d  \t"),iClass,aulClasses[iClass]);
		if (peceEntry != NULL) {
			CPrintF(TRANS("%s\n"),peceEntry->ece_fnmEntityClass);
		} else {
			CPrintF(TRANS("N/A\n"));
		}
	}
}

void DumpEntityHashEfficiency(void)
{
	_pNetwork->ga_World.wo_ehEntityHashContainer.ReportEfficiency();
}

void DumpWorldEntityHashEfficiency(void)
{
	_pNetwork->ga_World.wo_whWorldEntityContainer.ReportEfficiency();
}

// bVerbose == 0 - dump entity id, number of references and entity class name
// bVerbose == 1 - for each reference dump it's holder's name
// bVerbose == 2 - also dump info on pending events
void DumpEntityInfo(INDEX iVerbose,BOOL bAll) 
{
	if (bAll) {
		CPrintF(TRANS("All entities list:\n"));
		FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenAllEntities, CEntity, iten) {
			CEntity* penEntity = iten;
			CPrintF(TRANS("  EntityID: %X     References: %d    Class: %s\n"),iten->en_ulID,iten->en_ctReferences,iten->en_pecClass->GetName());
			if (iVerbose>=1) {
				_pNetwork->ga_World.DumpReferenceInfo(penEntity);
				
				if (iVerbose>=2) {
					if (IsDerivedFromClass(penEntity,"MovableEntity")) {
						INDEX ctStates = ((CRationalEntity*)penEntity)->en_stslStateStack.Count();
						for(INDEX iState=ctStates-1; iState>=0; iState--) {
							SLONG slState = ((CRationalEntity*)penEntity)->en_stslStateStack[iState];
							CPrintF("  Event pending: 0x%08x %s\n", slState, 
								((CRationalEntity*)penEntity)->en_pecClass->ec_pdecDLLClass->HandlerNameForState(slState));
						};
					}
				}
			}
		}
	} else {
		CPrintF(TRANS("Existing entities list:\n"));
		FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
			CEntity* penEntity = iten;
			CPrintF(TRANS("  EntityID: %X     References: %d    Class: %s\n"),iten->en_ulID,iten->en_ctReferences,iten->en_pecClass->GetName());
			if (iVerbose>=1) {
				_pNetwork->ga_World.DumpReferenceInfo(penEntity);
				
				if (iVerbose>=2) {
					if (IsDerivedFromClass(penEntity,"MovableEntity")) {
						INDEX ctStates = ((CRationalEntity*)penEntity)->en_stslStateStack.Count();
						for(INDEX iState=ctStates-1; iState>=0; iState--) {
							SLONG slState = ((CRationalEntity*)penEntity)->en_stslStateStack[iState];
							CPrintF("  Event pending: 0x%08x %s\n", slState, 
								((CRationalEntity*)penEntity)->en_pecClass->ec_pdecDLLClass->HandlerNameForState(slState));
						};
					}
				}
			}
		}
	}
}

void DumpServerStats() {
	if (_pNetwork != NULL) {
		_pNetwork->ga_srvServer.DumpStats();
	}
}

static void DumpEntity(INDEX ulEntityID)
{
	CEntity* penEntity = NULL;
	
	if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
		CPrintF(TRANS("Entity with ID %X does not exist!\n"),ulEntityID);
		return;
	} else {
		CPrintF(TRANS("Entity ID: %X\n"),ulEntityID);
		CPrintF(TRANS("   Entity class: %s\n"),penEntity->en_pecClass->GetName());
		CPrintF(TRANS("   Flags:  0x%X    Physics flags: 0x%X\n"),penEntity->en_ulFlags,penEntity->en_ulPhysicsFlags);
		if (penEntity->IsSentOverNet()) {
			CPrintF(TRANS("   IsSentOverNet = TRUE\n"));
		} else {
			CPrintF(TRANS("   IsSentOverNet = FALSE\n"));
		}
		if (penEntity->IsFlagOn(ENF_ALIVE)) {
			CPrintF(TRANS("   ENF_ALIVE = TRUE\n"));
		} else {
			CPrintF(TRANS("   ENF_ALIVE = FALSE\n"));
		}
		CPrintF(TRANS("   Placement: x: %6.2f, y: %6.2f, z:%6.2f\n"),penEntity->en_plPlacement.pl_PositionVector(1)
			,penEntity->en_plPlacement.pl_PositionVector(2),penEntity->en_plPlacement.pl_PositionVector(3));
		
		switch (penEntity->en_RenderType) {
		case CEntity::RT_ILLEGAL        : { CPrintF(TRANS("   RenderType: RT_ILLEGAL\n")); break;}
		case CEntity::RT_NONE           : { CPrintF(TRANS("   RenderType: RT_NONE\n")); break;}
		case CEntity::RT_MODEL          : { CPrintF(TRANS("   RenderType: RT_MODEL\n")); break;}
		case CEntity::RT_BRUSH          : { CPrintF(TRANS("   RenderType: RT_BRUSH\n")); break;}
		case CEntity::RT_EDITORMODEL    : { CPrintF(TRANS("   RenderType: RT_EDITORMODEL\n")); break;}
		case CEntity::RT_VOID           : { CPrintF(TRANS("   RenderType: RT_VOID\n")); break;}
		case CEntity::RT_FIELDBRUSH     : { CPrintF(TRANS("   RenderType: RT_FIELDBRUSH\n")); break;}
		case CEntity::RT_SKAMODEL       : { CPrintF(TRANS("   RenderType: RT_SKAMODEL\n")); break;}
		case CEntity::RT_SKAEDITORMODEL : { CPrintF(TRANS("   RenderType: RT_SKAEDITORMODEL\n")); break;}
		case CEntity::RT_TERRAIN        : { CPrintF(TRANS("   RenderType: RT_TERRAIN\n")); break;}
		};
		_pNetwork->ga_World.DumpReferenceInfo(penEntity);
	}       
}         

static void NetworkInfo(void)
{
	CPrintF("*Network library information:\n");
	CPrintF("Entities existing: %d\n", _ctEntities);
	
	CPrintF("Server:\n");
	if (_pNetwork->ga_srvServer.srv_bActive) {
		CPrintF("  last processed tick: %g\n", _pNetwork->ga_srvServer.srv_tmLastProcessedTick);
		CPrintF("  last processed sequence: %d\n", _pNetwork->ga_srvServer.srv_iLastProcessedSequence);
		CPrintF("  players:\n");
		for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ++iplt) {
			CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
			if (plt.plt_bActive) {
				CPrintF("    %2d(%2d):'%s'@client%2d: (%dact)\n", 
					iplt, plt.plt_Index, RemoveSpecialCodes(plt.plt_pcCharacter.GetNameForPrinting()),
					plt.plt_iClient, plt.plt_abReceived.GetCount());
			}
		}
		CPrintF("  clients:\n");
		for(INDEX iSession=0; iSession<_pNetwork->ga_srvServer.srv_assoSessions.Count(); ++iSession) {
			CSessionSocket &sso = _pNetwork->ga_srvServer.srv_assoSessions[iSession];
			if (sso.sso_bActive) {
				CPrintF("  %2d:'%s'\n", iSession, _cmiComm.Server_GetClientName(iSession)),
					CPrintF("    buffer: %dblk=%dk\n", 
					sso.sso_nsBuffer.GetUsedBlocks(),
					sso.sso_nsBuffer.GetUsedMemory()/1024);
				CPrintF("    state:");
				if (sso.sso_iDisconnectedState>0) {
					CPrintF("    disconnecting");
				} else if (sso.sso_bSendStream) {
					CPrintF("    connected");
				} else {
					CPrintF("    connecting");
				}
				CPrintF("\n");
			}
		}
	} else {
		CPrintF("  not a server\n");
	}
	CPrintF("Session state:\n");
	CPrintF("  buffer: (%dblk)%dk\n", 
		_pNetwork->ga_sesSessionState.ses_nsGameStream.GetUsedBlocks(),
		_pNetwork->ga_sesSessionState.ses_nsGameStream.GetUsedMemory()/1024);
	CPrintF("  last processed tick: %g\n", _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick);
	CPrintF("  last processed sequence: %d\n", _pNetwork->ga_sesSessionState.ses_iLastProcessedSequence);
	CPrintF("  level change: %d\n", _pNetwork->ga_sesSessionState.ses_ubNumLevelChanges);
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			ULONG ulID = -1;
			if (plt.plt_penPlayerEntity!=NULL) {
				ulID = plt.plt_penPlayerEntity->en_ulID;
			}
			CPrintF("  player %2d (ID:%d)\n", iplt, ulID);
		}
	}	
	if (TIMER_PROFILING) {		
		CTString strNetProfile;		
		_pfNetworkProfile.Report(strNetProfile);		
		CPrintF(strNetProfile);		
	}	
}

static void ListPlayers(void)
{
	CPrintF("player list:\n");
	if (!_pNetwork->ga_srvServer.srv_bActive) {
		CPrintF("  <not a server>\n");
		return;
	}
	
	CPrintF("  client# name\n");
	CPrintF("  ----------------------\n");
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			CPrintF("     %-2d   %s\n", plt.plt_iClient, RemoveSpecialCodes(plt.plt_pcCharacter.GetNameForPrinting()));
		}
	}
	CPrintF("  ----------------------\n");
}

static void KickClient(INDEX iClient, const CTString &strReason)
{
	if (!_pNetwork->IsServer()) {
		CPrintF( TRANS("Only server can kick people!\n"));
		return;
	}
	iClient = Clamp(iClient, INDEX(0), INDEX(NET_MAXGAMECOMPUTERS));
	if (!_pNetwork->ga_srvServer.srv_assoSessions[iClient].IsActive()) {
		CPrintF(TRANS("Client not connected!\n"));
		return;
	}
	if (iClient == 0) {
		CPrintF(TRANS("Can't kick local client!\n"));
		return;
	}
	CPrintF( TRANS("Kicking %d with explanation '%s'...\n"), iClient, strReason);
	_pNetwork->ga_srvServer.SendDisconnectMessage(iClient, "Admin: "+strReason);
}

static void KickByName(const CTString &strName, const CTString &strReason)
{
	if (!_pNetwork->IsServer()) {
		CPrintF( TRANS("Only server can kick people!\n"));
		return;
	} 
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive && plt.plt_pcCharacter.GetNameForPrinting().Undecorated().Matches(strName)) {
			KickClient(plt.plt_iClient, strReason);
		}
	}
}

static void Admin(const CTString &strCommand)
{
/* //0522 kwon 삭제.
CNetworkMessage nm(MSG_ADMIN_COMMAND);
nm<<net_strAdminPassword<<strCommand;
_pNetwork->SendToServerReliable(nm);
	*/
}

// Print to file or to console
extern void FCPrintF(CTFileStream *pstrm,const char *strFormat, ...);

// Write stock info
extern void StockInfoToStream(CTFileStream *pstrm)
{
	// find memory used by shadowmap (both cached and uploaded)
	INDEX ctCachedShadows=0, ctDynamicShadows=0, ctFlatShadows=0;
	SLONG slStaticMemory=0,  slDynamicMemory=0,  slUploadMemory=0;
	SLONG slShdBytes=0,  slSlackMemory=0,    slFlatMemory=0;
	INDEX ct256=0, ct128=0, ct64=0, ct32=0, ct16=0; 
	SLONG sl256Memory=0, sl128Memory=0, sl64Memory=0, sl32Memory=0, sl16Memory=0; 
	
	if( _pGfx!=NULL)
	{
		FLOAT fSlackRatio;
		FOREACHINLIST( CShadowMap, sm_lnInGfx, _pGfx->gl_lhCachedShadows, itsm)
		{ // get polygon size in pixels (used portion of shadowmap)
			SLONG slStaticSize, slDynamicSize, slUploadSize;
			BOOL bIsFlat = itsm->GetUsedMemory( slStaticSize, slDynamicSize, slUploadSize, fSlackRatio);
			SLONG slTotalSize = slDynamicSize+slUploadSize;
			if( bIsFlat) {
				slStaticMemory += 4;
				slTotalSize    += 4;
				slFlatMemory   += slStaticSize;
				ctFlatShadows++;
			} else {
				slStaticMemory += slStaticSize;
				slTotalSize    += slStaticSize;
				if( slTotalSize>0) ctCachedShadows++;
			}
			if( slDynamicSize>0) {
				slDynamicMemory += slDynamicSize;
				ctDynamicShadows++;
			}
			slUploadMemory  += slUploadSize;
			slShdBytes  += slTotalSize + sizeof(CShadowMap);
			slSlackMemory   += slTotalSize*fSlackRatio;
			
			if( !bIsFlat) { // by size ...
				if(      slStaticSize>128*1024) { ct256++; sl256Memory+=slTotalSize; }
				else if( slStaticSize> 64*1024) { ct128++; sl128Memory+=slTotalSize; }
				else if( slStaticSize> 32*1024) { ct64++;  sl64Memory +=slTotalSize; }
				else if( slStaticSize> 16*1024) { ct32++;  sl32Memory +=slTotalSize; }
				else if( slStaticSize> 0)       { ct16++;  sl16Memory +=slTotalSize; }
			}
		}
		// report shadowmap memory usage (if any)
		if( slShdBytes>0) {
			FCPrintF(pstrm, "\nCached shadowmaps:\n");
			FCPrintF(pstrm, "    Total: %d in %d KB with %d%% (%d KB) of slack space\n", ctCachedShadows, slShdBytes/1024, slSlackMemory*100/slShdBytes, slSlackMemory/1024);
			FCPrintF(pstrm, "   Static: %d KB\n", slStaticMemory/1024);
			FCPrintF(pstrm, "   Upload: %d KB\n", slUploadMemory/1024);
			FCPrintF(pstrm, "  Dynamic: %d in %d KB\n", ctDynamicShadows, slDynamicMemory/1024);
			if( ctCachedShadows<1) ctCachedShadows=1; // for percentage calc
			FCPrintF(pstrm, "    Flats: %d (%d%%) with %d KB saved\n", ctFlatShadows, ctFlatShadows*100/ctCachedShadows, slFlatMemory/1024);
			FCPrintF(pstrm, "of size:\n");
			FCPrintF(pstrm, "    >128K: %4d in %d KB\n", ct256, sl256Memory/1024);
			FCPrintF(pstrm, "  128-64K: %4d in %d KB\n", ct128, sl128Memory/1024);
			FCPrintF(pstrm, "   64-32K: %4d in %d KB\n", ct64,  sl64Memory /1024);
			FCPrintF(pstrm, "   32-16K: %4d in %d KB\n", ct32,  sl32Memory /1024);
			FCPrintF(pstrm, "    <=16K: %4d in %d KB\n", ct16,  sl16Memory /1024);
		}
	}
	
	// report world stats
	INDEX ctEntities=0, ctShadowLayers=0, ctPolys=0,    ctPlanes=0,   ctEdges=0,    ctVertices=0, ctSectors=0;
	SLONG slEntBytes=0, slLyrBytes=0,     slPlyBytes=0, slPlnBytes=0, slEdgBytes=0, slVtxBytes=0, slSecBytes=0;
	SLONG slCgrBytes=0;
	
	CWorld *pwo = NULL;
	// if pwoCurrentWorld exists
	if(_pShell->GetSymbol("pwoCurrentWorld",TRUE)!=NULL) {
		pwo = (CWorld*)_pShell->GetINDEX("pwoCurrentWorld");
	}
	if( pwo!=NULL)
	{
		// report count of and memory used by entities
		FOREACHINDYNAMICCONTAINER( pwo->wo_cenEntities, CEntity, iten) {
			ctEntities++;
			slEntBytes += iten->GetUsedMemory();
		}
		
		// report shadow layers and world geometry memory usage
		FOREACHINDYNAMICARRAY( pwo->wo_baBrushes.ba_abrBrushes, CBrush3D, itbr) // for all brush entities in the world
		{ 
			// skip brush without entity
			if( itbr->br_penEntity==NULL) continue; 
			
			// for each mip
			FOREACHINLIST( CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) 
			{ 
				// for each sector in the brush mip
				FOREACHINDYNAMICARRAY( itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// add sector class memory usage to polygons memory
					ctSectors++;
					slSecBytes += itbsc->GetUsedMemory();
					
					// add each vertex and working vertex in sector
					ctVertices += itbsc->bsc_abvxVertices.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abvxVertices, CBrushVertex,   itbvx) slVtxBytes += itbvx->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awvxVertices, CWorkingVertex, itwvx) slVtxBytes += 32; // aligned to 32 bytes!
					
					// add each plane and working plane in sector
					ctPlanes += itbsc->bsc_abplPlanes.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abplPlanes, CBrushPlane,   itbpl) slPlnBytes += itbpl->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awplPlanes, CWorkingPlane, itwpl) slPlnBytes += sizeof(CWorkingPlane);
					
					// add each edge and working edge in sector
					ctEdges += itbsc->bsc_abedEdges.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abedEdges, CBrushEdge,   itbed) slEdgBytes += itbed->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awedEdges, CWorkingEdge, itwed) slEdgBytes += sizeof(CWorkingEdge);
					
					// for each polygon in sector
					ctPolys += itbsc->bsc_abpoPolygons.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) { 
						CBrushPolygon &bpo = *itbpo;
						slPlyBytes += bpo.GetUsedMemory();
						// count in the shadow layers (if any)
						if( bpo.bpo_smShadowMap.bsm_lhLayers.IsEmpty()) continue; // skip polygon without shadowmap
						ctShadowLayers += bpo.bpo_smShadowMap.GetShadowLayersCount();
						slLyrBytes += bpo.bpo_smShadowMap.GetUsedMemory();
					}
				}
			}
		} // add in memory used by collision grid
		extern SLONG  GetCollisionGridMemory( CCollisionGrid *pcg);
		slCgrBytes += GetCollisionGridMemory( pwo->wo_pcgCollisionGrid);
	}
	
	// stock info
	const DOUBLE dToMB = 1.0/1024.0/1024.0;
	const FLOAT fTexBytes = dToMB * _pTextureStock->CalculateUsedMemory();
	const FLOAT fClsBytes = dToMB * _pEntityClassStock->CalculateUsedMemory();
	const FLOAT fMdlBytes = dToMB * _pModelStock->CalculateUsedMemory();
	const FLOAT fSndBytes = dToMB * _pSoundStock->CalculateUsedMemory();
	const FLOAT fMshBytes = dToMB * _pMeshStock->CalculateUsedMemory();
	const FLOAT fAstBytes = dToMB * _pAnimSetStock->CalculateUsedMemory();
	const FLOAT fShaBytes = dToMB * _pShaderStock->CalculateUsedMemory();
	const FLOAT fSkaBytes = dToMB * _pSkeletonStock->CalculateUsedMemory();
	const FLOAT fSmcBytes = dToMB * _pModelInstanceStock->CalculateUsedMemory();
	
	extern INDEX _ctExistingModelInstances;
	FCPrintF(pstrm, "\nStock information:\n");
	FCPrintF(pstrm, "     Textures: %5d (%5.2f MB)\n", _pTextureStock->GetTotalCount(), fTexBytes);
	FCPrintF(pstrm, "   ShadowMaps: %5d (%5.2f MB)\n", ctCachedShadows, slShdBytes*dToMB);
	FCPrintF(pstrm, "     Entities: %5d (%5.2f MB)\n", ctEntities,      slEntBytes*dToMB);
	FCPrintF(pstrm, "       Sounds: %5d (%5.2f MB)\n", _pSoundStock->GetTotalCount(), fSndBytes);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "      Sectors: %5d (%5.2f MB)\n", ctSectors,  slSecBytes*dToMB);
	FCPrintF(pstrm, "       Planes: %5d (%5.2f MB)\n", ctPlanes,   slPlnBytes*dToMB);
	FCPrintF(pstrm, "        Edges: %5d (%5.2f MB)\n", ctEdges,    slEdgBytes*dToMB);
	FCPrintF(pstrm, "     Polygons: %5d (%5.2f MB)\n", ctPolys,    slPlyBytes*dToMB);
	FCPrintF(pstrm, "     Vertices: %5d (%5.2f MB)\n", ctVertices, slVtxBytes*dToMB);
	FCPrintF(pstrm, " ShadowLayers: %5d (%5.2f MB)\n", ctShadowLayers, slLyrBytes*dToMB);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "      Classes: %5d (%5.2f MB)\n", _pEntityClassStock->GetTotalCount(),fClsBytes);
	FCPrintF(pstrm, "       Models: %5d (%5.2f MB)\n", _pModelStock->GetTotalCount(),    fMdlBytes);
	FCPrintF(pstrm, "       Meshes: %5d (%5.2f MB)\n", _pMeshStock->GetTotalCount(),     fMshBytes);
	FCPrintF(pstrm, "    Skeletons: %5d (%5.2f MB)\n", _pSkeletonStock->GetTotalCount(), fSkaBytes);
	FCPrintF(pstrm, "     AnimSets: %5d (%5.2f MB)\n", _pAnimSetStock->GetTotalCount(),  fAstBytes);
	FCPrintF(pstrm, "      Shaders: %5d (%5.2f MB)\n", _pShaderStock->GetTotalCount(),   fShaBytes);
	FCPrintF(pstrm, "         Smcs: %5d (%5.2f MB) (%d)\n", _pModelInstanceStock->GetTotalCount(), fSmcBytes, _ctExistingModelInstances);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "CollisionGrid: %.2f MB\n", slCgrBytes*dToMB);
	FCPrintF(pstrm, "--------------\n");
	FCPrintF(pstrm, "        Total: %.2f MB\n", fTexBytes+fSndBytes+fMdlBytes+fMshBytes+fSkaBytes+fAstBytes+fShaBytes+fSmcBytes
		+ (slShdBytes+slEntBytes+slSecBytes+slPlnBytes+slEdgBytes+slPlyBytes+slVtxBytes+slLyrBytes+slCgrBytes)*dToMB);
	FCPrintF(pstrm, "\n");
}

static void StockInfo(void)
{
	StockInfoToStream(NULL);
}

extern void StockDumpToStream(CTFileStream *pstrm)
{
	pstrm->PutLine_t("Animations:");
	_pAnimStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Textures:");
	_pTextureStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Models:");
	_pModelStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Meshes:");
	_pMeshStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Skeletons:");
	_pSkeletonStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Animsets:");
	_pAnimSetStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Shaders:");
	_pShaderStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("ModelInstances:");
	_pModelInstanceStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Sounds:");
	_pSoundStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Classes:");
	_pEntityClassStock->DumpMemoryUsage_t(*pstrm);
}

static void StockDump(void)
{
	try {
		CTFileStream strm;
		CTFileName fnm = CTString("Temp\\StockDump.txt");
		strm.Create_t(fnm);
		// Dump stock to file
		StockDumpToStream(&strm);
		CPrintF("Dumped to '%s'\n", CTString(fnm));
	} catch (char *strError) {
		CPrintF("Error: %s\n", strError);
	}
}

// free all unused stocks
extern void FreeUnusedStock(void)
{
	// free all unused stocks
	_pEntityClassStock->FreeUnused();
	_pModelStock->FreeUnused();
	_pSoundStock->FreeUnused();
	_pFontStock->FreeUnused();
	_pModelInstanceStock->FreeUnused();
	_pTextureStock->FreeUnused();
	_pAnimStock->FreeUnused();
	_pMeshStock->FreeUnused();
	_pSkeletonStock->FreeUnused();
	_pAnimSetStock->FreeUnused();
	_pShaderStock->FreeUnused();
}

/*
* Default constructor.
*/
CNetworkLibrary::CNetworkLibrary(void) :
ga_IsServer(FALSE),               // is not server
ga_bFirstMainLoop(TRUE),          // MainLoop will be run for the first time
ga_bDemoRec(FALSE),               // not recording demo
ga_bDemoPlay(FALSE),              // not playing demo
ga_bDemoPlayFinished(FALSE),      // demo not finished
ga_srvServer(*new CServer),
ga_sesSessionState(*new CSessionState),
#ifdef NDEBUG
m_ubGMLevel( 0 )
#else
m_ubGMLevel( 1 )
#endif
{
	// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG

	// EDIT : BS : BEGIN
	m_tickSendMoveList = 0;
	// EDIT : BS : END

	TRACKMEM(Mem,"Network");
	ga_aplsPlayers.New(NET_MAXLOCALPLAYERS);
	
	// default demo syncronization is real-time, with 1:1 playback speed
	ga_fDemoSyncRate = DEMOSYNC_REALTIME;
	ga_fDemoRealTimeFactor = 1.0f;
	ga_fGameRealTimeFactor = 1.0f;
	ga_pubDefaultState = NULL;
	ga_slDefaultStateSize = 0;
	memset(ga_aubDefaultProperties, 0, sizeof(ga_aubDefaultProperties));
	ga_pubCRCList = NULL;
	ga_slCRCList = 0;
	
	ga_csNetwork.cs_iIndex = 2000;
	ga_fEnumerationProgress = 0;
	ga_bEnumerationChange = FALSE;
	ga_aulBandwidthGraph.Clear();
	ga_aulBandwidthGraph.New(net_iGraphBuffer);
	ga_ubNumLevelChanges = 0;
	
	memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);
	//0105
	//IsTcpServer = FALSE;		// by seo-40225
	m_bReadyStart = FALSE;
	wo_aItemData.Clear();
//	wo_aItemData.New(MAXITEMDATA);			// 원본.
	wo_aItemName.Clear();
//	wo_aItemName.New(MAXITEMDATA);
	wo_iNumOfItem = -1;
	wo_iNumOfItemName = -1;
	wo_iNumOfAction = -1;			// yjpark
	wo_iNumOfSkill = -1;			// yjpark
	wo_iNumOfSSkill = -1;
	wo_iNumOfMobName = -1;
	m_bSingleMode = FALSE;
	
	wo_aOptionData.Clear();
	wo_vecItemRareOption.clear();
	wo_iNumOfOption = -1;
	
	wo_dwEnemyCount			= 0;
	wo_dwKilledEnemyCount	= 0;
	// set check point variable
	wo_stCheckPoint.m_iCheckFlag = 0;
	wo_stCheckPoint.m_iMaxCheckFlag = 0;
	wo_stCheckPoint.m_iQuestIndex	= 0;

	MyCharacterInfo.StatPoint = 0;	
	MyCharacterInfo.sbItemEffectOption = 0;
	
	// 케릭터와 연관된 Pet 관련 데이터 초기화
	MyCharacterInfo.bPetRide = FALSE; 
	MyCharacterInfo.iPetType = -1;
	MyCharacterInfo.iPetAge = -1;
	
	MyCharacterInfo.secretkey = 0;

	// WSS_VISIBLE 070511
	MyCharacterInfo.m_ModelColor = NULL;
	MyCharacterInfo.ubGuildNameColor =0;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	// WSS_GUILDMASTER 070511
	MyCharacterInfo.sbGuildRank = 0;
	MyCharacterInfo.bConsensus = FALSE;	// WSS_DRATAN_SEIGEWARFARE 2007/08/01	
	
	MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL;

	m_uiSendedTime = 0;
	m_bSendMessage = FALSE;

	m_iServerGroup = -1;
	m_iServerCh	= -1;
	
	m_bIsPvp = FALSE;

	for(int i =0; i<WEAR_TOTAL;++i)
	{	
		pMyCurrentWearing[i] = NULL;
	}

	ga_bGuildWar = FALSE;
}

/*
* Destructor.
*/
CNetworkLibrary::~CNetworkLibrary(void)
{
	wo_aItemData.Clear();
	wo_aItemName.Clear();
	wo_iNumOfItem = -1;
	wo_iNumOfItemName = -1;
	wo_iNumOfAction = -1;			// yjpark
	wo_iNumOfSkill = -1;			// yjpark
	wo_iNumOfSSkill = -1;
	wo_iNumOfMobName = -1;
	wo_aOptionData.Clear();
	wo_vecItemRareOption.clear();
	wo_iNumOfOption = -1;
	// clear the global world
	ga_World.Clear();
	
	wo_dwEnemyCount			= 0;
	wo_dwKilledEnemyCount	= 0;
	// set check point variable
	wo_stCheckPoint.m_iCheckFlag = 0;
	wo_stCheckPoint.m_iMaxCheckFlag = 0;
	wo_stCheckPoint.m_iQuestIndex	= 0;
	delete &ga_sesSessionState;
	delete &ga_srvServer;
}

/*
* Initialize game management.
*/
void CNetworkLibrary::Init(const CTString &strGameID)
{
	TRACKMEM(Mem,"Network");
	// remember the game ID
	CMessageDispatcher::Init(strGameID);
	
	// add shell symbols
	_pShell->DeclareSymbol("user INDEX dbg_bBreak;", &dbg_bBreak);
	_pShell->DeclareSymbol("persistent user INDEX gam_bPretouch;", &gam_bPretouch);
	
	_pShell->DeclareSymbol("user INDEX dem_iRecordedNumber;",     &dem_iRecordedNumber);
	_pShell->DeclareSymbol("user void StartDemoRecording(void);", &StartDemoRecording);
	_pShell->DeclareSymbol("user void StopDemoRecording(void);",  &StopDemoRecording);
	_pShell->DeclareSymbol("user void NetworkInfo(void);",  &NetworkInfo);
	_pShell->DeclareSymbol("user void ReinitializeLevel(CTString);",&ReinitializeLevel);
	_pShell->DeclareSymbol("user void DumpClassReferences(void);",  &DumpClassReferences);  
	_pShell->DeclareSymbol("user void DumpEntityHashEfficiency(void);",  &DumpEntityHashEfficiency);
	_pShell->DeclareSymbol("user void DumpWorldEntityHashEfficiency(void);",  &DumpWorldEntityHashEfficiency);  
	_pShell->DeclareSymbol("user void DumpEntity(INDEX);",  &DumpEntity);
	_pShell->DeclareSymbol("user void DumpEntityInfo(INDEX,INDEX);",  &DumpEntityInfo);
	_pShell->DeclareSymbol("user void DumpPlayer(INDEX);",  &DumpPlayer);  
	_pShell->DeclareSymbol("user void DumpAllPlayers(INDEX);",  &DumpAllPlayers);  
	_pShell->DeclareSymbol("user void DumpServerStats();",  &DumpServerStats);
	_pShell->DeclareSymbol("user void StockInfo(void);",    &StockInfo);
	_pShell->DeclareSymbol("user void StockDump(void);",    &StockDump);
	_pShell->DeclareSymbol("user void DumpSkaStringToIDTable(void);", &DumpSkaStringToIDTable);
	_pShell->DeclareSymbol("user void DumpShellHashTable(void);", &DumpShellHashTable);
	_pShell->DeclareSymbol("user void RendererInfo(void);", &RendererInfo);
	_pShell->DeclareSymbol("user void ClearRenderer(void);",   &ClearRenderer);
	_pShell->DeclareSymbol("user void CacheShadows(void);",    &CacheShadows);
	_pShell->DeclareSymbol("user void KickClient(INDEX, CTString);", &KickClient);
	_pShell->DeclareSymbol("user void KickByName(CTString, CTString);", &KickByName);
	_pShell->DeclareSymbol("user void ListPlayers(void);", &ListPlayers);
	_pShell->DeclareSymbol("user void Admin(CTString);", &Admin);	
	_pShell->DeclareSymbol("user void AddIPMask(CTString);", &AddIPMask);
	_pShell->DeclareSymbol("user void RemIPMask(CTString);", &RemIPMask);
	_pShell->DeclareSymbol("user void AddNameMask(CTString);", &AddNameMask);
	_pShell->DeclareSymbol("user void RemNameMask(CTString);", &RemNameMask);	
	_pShell->DeclareSymbol("user FLOAT dem_tmTimer;",         &ga_fDemoTimer);
	_pShell->DeclareSymbol("user FLOAT dem_fSyncRate;",       &ga_fDemoSyncRate);
	_pShell->DeclareSymbol("user FLOAT dem_fRealTimeFactor;", &ga_fDemoRealTimeFactor);
	_pShell->DeclareSymbol("user FLOAT gam_fRealTimeFactor;", &ga_fGameRealTimeFactor);
	_pShell->DeclareSymbol("user INDEX dem_iAnimFrame;", &dem_iAnimFrame);
	_pShell->DeclareSymbol("user INDEX dem_iMotionBlurInterFrames;", &dem_iMotionBlurInterFrames);
	
	_pShell->DeclareSymbol("user const FLOAT cmd_tmTick;", &cmd_tmTick);
	_pShell->DeclareSymbol("persistent user CTString cmd_cmdOnTick;", &cmd_cmdOnTick);
	_pShell->DeclareSymbol("user CTString cmd_strChatSender ;", &cmd_strChatSender );
	_pShell->DeclareSymbol("user CTString cmd_strChatMessage;", &cmd_strChatMessage);
	_pShell->DeclareSymbol("persistent user CTString cmd_cmdOnChat;", &cmd_cmdOnChat);
	
	_pShell->DeclareSymbol("user INDEX net_ctChatMessages;", &net_ctChatMessages);
	
	_pShell->DeclareSymbol("persistent user INDEX ent_bReportSpawnInWall;", &ent_bReportSpawnInWall);
	
	_pShell->DeclareSymbol("user INDEX net_bSendEnumRequest;",   &_bSendEnumRequest);
	_pShell->DeclareSymbol("user INDEX net_bDumpStreamBlocks;",   &net_bDumpStreamBlocks);
	_pShell->DeclareSymbol("user INDEX net_bDumpConnectionInfo;", &net_bDumpConnectionInfo);
	_pShell->DeclareSymbol("user INDEX net_iPort;", &net_iPort);
	_pShell->DeclareSymbol("persistent user CTString net_strLocalHost;", &net_strLocalHost);
	_pShell->DeclareSymbol("persistent user CTString net_strLocationCode;", &net_strLocationCode);
	_pShell->DeclareSymbol("user CTString net_strVIPPassword;", &net_strVIPPassword);
	
	_pShell->DeclareSymbol("user CTString net_strObserverPassword;", &net_strObserverPassword);
	_pShell->DeclareSymbol("user INDEX net_iVIPReserve;", &net_iVIPReserve);
	_pShell->DeclareSymbol("user INDEX net_iMaxObservers;", &net_iMaxObservers);
	_pShell->DeclareSymbol("user INDEX net_iMaxClients;", &net_iMaxClients);
	_pShell->DeclareSymbol("user CTString net_strConnectPassword;", &net_strConnectPassword);
	_pShell->DeclareSymbol("user CTString net_strAdminPassword;", &net_strAdminPassword);
	_pShell->DeclareSymbol("user FLOAT net_tmConnectionTimeout;", &net_tmConnectionTimeout);
	_pShell->DeclareSymbol("user FLOAT net_tmProblemsTimeout;", &net_tmProblemsTimeout);
	_pShell->DeclareSymbol("user FLOAT net_tmDisconnectTimeout;", &net_tmDisconnectTimeout);
	_pShell->DeclareSymbol("user INDEX net_bReportCRC;", &net_bReportCRC);
	_pShell->DeclareSymbol("user INDEX ser_iRememberBehind;", &ser_iRememberBehind);
	_pShell->DeclareSymbol("persistent user INDEX net_bLookupHostNames;",    &net_bLookupHostNames);
	_pShell->DeclareSymbol("persistent user INDEX net_iCompression ;",       &net_iCompression);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportPackets;", &net_bReportPackets);
	_pShell->DeclareSymbol("INDEX net_bReportServerTraffic;", &net_bReportServerTraffic);
	_pShell->DeclareSymbol("INDEX net_bReportClientTraffic;", &net_bReportClientTraffic);
	_pShell->DeclareSymbol("INDEX net_bReportDemoTraffic;", &net_bReportDemoTraffic);
	_pShell->DeclareSymbol("INDEX net_iDemoQuality;", &net_iDemoQuality);
	_pShell->DeclareSymbol("INDEX net_bLocalPrediction;", &net_bLocalPrediction); 
	_pShell->DeclareSymbol("persistent user INDEX net_iMaxSendRetries;", &net_iMaxSendRetries);
	_pShell->DeclareSymbol("persistent user FLOAT net_fSendRetryWait;", &net_fSendRetryWait);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportTraffic;", &net_bReportTraffic);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportICMPErrors;", &net_bReportICMPErrors);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportMiscErrors;", &net_bReportMiscErrors);
	_pShell->DeclareSymbol("user INDEX net_bLerping;",       &net_bLerping);
	_pShell->DeclareSymbol("persistent user INDEX ser_bClientsMayPause;", &ser_bClientsMayPause);
	_pShell->DeclareSymbol("persistent user INDEX ser_bEnumeration;",      &ser_bEnumeration);
//#if _GAMESPY
//	_pShell->DeclareSymbol("persistent user INDEX ser_bHeartbeatGameSpy;", &ser_bHeartbeatGameSpy);
//#endif
	
	_pShell->DeclareSymbol("user INDEX net_bNetworkGame;", &net_bNetworkGame);  
	
	
	
	_pShell->DeclareSymbol("persistent user FLOAT ser_tmKeepAlive;", &ser_tmKeepAlive);
	_pShell->DeclareSymbol("persistent user FLOAT ser_tmPingUpdate;", &ser_tmPingUpdate);
	_pShell->DeclareSymbol("persistent user INDEX ser_bWaitFirstPlayer;", &ser_bWaitFirstPlayer);
	_pShell->DeclareSymbol("persistent user INDEX ser_iMaxAllowedBPS;", &ser_iMaxAllowedBPS);
	_pShell->DeclareSymbol("persistent user CTString ser_strIPMask;", &ser_strIPMask);
	_pShell->DeclareSymbol("persistent user CTString ser_strNameMask;", &ser_strNameMask);
	_pShell->DeclareSymbol("persistent user INDEX ser_bInverseBanning;", &ser_bInverseBanning);
	_pShell->DeclareSymbol("persistent user CTString ser_strMOTD;", &ser_strMOTD);
	_pShell->DeclareSymbol("user FLOAT ser_fAckDropRate;", &ser_fAckDropRate);
	_pShell->DeclareSymbol("persistent user FLOAT ser_fPositionTreshold;", &ser_fPositionTreshold);
	_pShell->DeclareSymbol("persistent user FLOAT ser_fOrientationTreshold;", &ser_fOrientationTreshold);
	_pShell->DeclareSymbol("user INDEX ser_iSkipTicks;", &ser_iSkipTicks);
	_pShell->DeclareSymbol("persistent user INDEX ser_iMaxBufferSize;", &ser_iMaxBufferSize);
	_pShell->DeclareSymbol("     const user INDEX ser_iMaxPendingData;", &ser_iMaxPendingData);
	_pShell->DeclareSymbol("     const user INDEX ser_iMaxTotalData;", &ser_iMaxTotalData);
	
	_pShell->DeclareSymbol("INDEX net_iHashCompartments;", &net_iHashCompartments);
	_pShell->DeclareSymbol("INDEX net_iHashSlotsPerComp;", &net_iHashSlotsPerComp);
	_pShell->DeclareSymbol("INDEX net_iHashStep;", &net_iHashStep);
	
	_pShell->DeclareSymbol("persistent user INDEX cli_iSendBehind;", &cli_iSendBehind);
	_pShell->DeclareSymbol("persistent user INDEX cli_iMaxBPS;",     &cli_iMaxBPS);
	_pShell->DeclareSymbol("persistent user INDEX cli_iMinBPS;",     &cli_iMinBPS);
	_pShell->DeclareSymbol("persistent user FLOAT cli_fTickDropRate;", &cli_fTickDropRate);
	_pShell->DeclareSymbol("persistent user INDEX cli_iFPSLimit;", &cli_iFPSLimit);
	
	_pShell->DeclareSymbol("user FLOAT net_fLimitLatencySend;",   &_pbsSend.pbs_fLatencyLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLimitLatencyRecv;",   &_pbsRecv.pbs_fLatencyLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLatencyVariationSend;", &_pbsSend.pbs_fLatencyVariation);
	_pShell->DeclareSymbol("user FLOAT net_fLatencyVariationRecv;", &_pbsRecv.pbs_fLatencyVariation);
	_pShell->DeclareSymbol("user FLOAT net_fLimitBandwidthSend;", &_pbsSend.pbs_fBandwidthLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLimitBandwidthRecv;", &_pbsRecv.pbs_fBandwidthLimit);
	_pShell->DeclareSymbol("user FLOAT net_fDropPackets;", &net_fDropPackets);
	_pShell->DeclareSymbol("persistent user INDEX net_iGraphBuffer;", &net_iGraphBuffer);
	_pShell->DeclareSymbol("user const INDEX precache_NONE;",     &_precache_NONE);
	_pShell->DeclareSymbol("user const INDEX precache_SMART;",    &_precache_SMART);
	_pShell->DeclareSymbol("user const INDEX precache_ALL;",      &_precache_ALL);
	_pShell->DeclareSymbol("user const INDEX precache_PARANOIA;", &_precache_PARANOIA);
	_pShell->DeclareSymbol("persistent user INDEX gam_iPrecachePolicy;", &gam_iPrecachePolicy);
	
	_pShell->DeclareSymbol("user FLOAT phy_fCollisionCacheAhead;",  &phy_fCollisionCacheAhead);
	_pShell->DeclareSymbol("user FLOAT phy_fCollisionCacheAround;", &phy_fCollisionCacheAround);
	
	_pShell->DeclareSymbol("user INDEX phy_bNonZoningClipMove;", &phy_bNonZoningClipMove);
	_pShell->DeclareSymbol("user INDEX phy_bNonZoningClipRay ;", &phy_bNonZoningClipRay );
	
	_pShell->DeclareSymbol("INDEX pwoCurrentWorld;", &_pwoCurrentWorld);

	// WSS_DRATAN_SEIGEWARFARE 2007/08/14 -------------------------------->>
	// 390 ~ 399 공성 부활진지 초기화
	for(int i=0;i<10;i++)
	{	
		_pNetwork->MyCharacterInfo.mQuarter[390+i] = -1;
		_pNetwork->MyCharacterInfo.mQuarterName[390+i] = CTString("");
	}
	// -------------------------------------------------------------------<<
}





/*
* Start a peer-to-peer game session.
*
* remember to keep this routine up to date with CNetworkLibrary::Read()
*/
void CNetworkLibrary::StartPeerToPeer_t(const CTString &strSessionName,
										const CTFileName &fnmWorld, ULONG ulSpawnFlags, 
										INDEX ctMaxPlayers, BOOL bWaitAllPlayers,
										void *pvSessionProperties) // throw char *
{
	TRACKMEM(Mem,"Network");
	// remove all pending sounds
	_pSound->Flush();
	StartProgress();
	
	// go on
#ifdef _DEBUG
	CPrintF( TRANS("Starting session: '%s'\n"), strSessionName);
	CPrintF( TRANS("  level: '%s'\n"), (const char*) fnmWorld);
	CPrintF( TRANS("  spawnflags: %08x\n"), ulSpawnFlags);
	CPrintF( TRANS("  max players: %d\n"), ctMaxPlayers);
	CPrintF( TRANS("  waiting: %d\n"), bWaitAllPlayers);
#endif
	
	CGatherCRC gc;
	
	// if starting in network
	if (_cmiComm.IsNetworkEnabled()) {
		CPrintF( TRANS("  network is on\n"));
		// start gathering CRCs
		FreeUnusedStock();
		InitCRCGather();
		
	} else {
		CPrintF( TRANS("  network is off\n"));
	}
	
	// access to the list of handlers must be locked
	CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, TRUE);
	
	ga_strSessionName = strSessionName;
	ga_bLocalPause = FALSE;
	
	ga_ubNumLevelChanges++;
	ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;
	
	ga_sesSessionState.ses_ulSpawnFlags = ulSpawnFlags;
	
	memcpy(ga_aubProperties, pvSessionProperties, sizeof(CSessionProperties));
	ga_IsServer = TRUE;
	
	// remember the world filename
	ga_fnmWorld = fnmWorld;
	ga_fnmNextLevel = CTString("");
	try {
		// load the world
		_pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading
//		ga_World.Load_t(fnmWorld);
		//wooss 050822
		if(!_pUIMgr->m_testUI_MODE)	ga_World.Load_t(fnmWorld);
		
		// delete all entities that don't fit given spawn flags
		ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
	} catch(char *) {
		ga_IsServer = FALSE;
		ga_fnmWorld = CTString("");
		_cmiComm.Server_Close();
		_cmiComm.Client_Close();
		StopProgress();
		throw;
	}
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);
	
	//  SetProgressDescription(TRANS("starting server"));
	SetProgressDescription(TRANS("starting Last Chaos..."));
	CallProgressHook_t(0.0f);
	// initialize server
	try {
		//! 서버 초기화.
		ga_srvServer.Start_t();
	} catch (char *) {
		ga_World.Clear();
		StopProgress();
		throw;
	}
	ga_IsServer = TRUE;
	CallProgressHook_t(1.0f);
	
	SetProgressDescription(TRANS("Entering World"));
	CallProgressHook_t(0.0f);
	// initialize session state
	try {
		//! 세션 스테이트 초기화 및 서버 스타트.
		ga_sesSessionState.Start_t(-1);
	} catch (char *strError) {
		(void)strError;
		ga_srvServer.Stop();
		ga_World.Clear();
		StopProgress();
		throw;
	}
	CallProgressHook_t(1.0f);
	
	// remember maximum number of players
	ga_sesSessionState.ses_ctMaxPlayers = ctMaxPlayers;
	ga_sesSessionState.ses_bWaitAllPlayers = bWaitAllPlayers;
	
	// time speed is normal by default
	ga_sesSessionState.ses_fRealTimeFactor = 1.0f;
	
	// eventually cache all shadowmaps in world (memory eater!)
	if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
	// flush stale caches
	FreeUnusedStock();
	// mark that pretouching is required
	_bNeedPretouch = TRUE;
	
	FinishCRCGather();
	CPrintF( TRANS("  started.\n"));
	StopProgress();
	// mute sounds for one second
	_pSound->Mute(1);
}

/*
 * Save the game.
 */
void CNetworkLibrary::Save_t(const CTFileName &fnmGame) // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // mute all sounds
  _pSound->Mute(1);

  // must be server
  if (!ga_IsServer) 
  {
    if (IsPlayingDemo()) 
	{
      throw "";
    } 
	else 
	{
      throw TRANS("Cannot save game - not a server!\n");
    }
  }
  
#if _DEBUG
  CTimerValue tvLoadStart = _pTimer->GetHighPrecisionTimer();
#endif
  extern INDEX gam_iMaxSaveGameSize;

  // create the file
  CTFileStream strmFile;
  strmFile.Create_t(fnmGame);

  if(gam_iMaxSaveGameSize>0) {
    strmFile.SetFileSize_t(gam_iMaxSaveGameSize);
  }

  // write game to stream
  strmFile.WriteID_t("GAME");
  ga_sesSessionState.Write_t(&strmFile);
  strmFile.WriteID_t("GEND");   // game end
  

  
  

#if _DEBUG
  CTimerValue tvLoadEnd = _pTimer->GetHighPrecisionTimer();
  FLOAT fLoadTime = (tvLoadEnd-tvLoadStart).GetSeconds();
  SLONG slGameSize = strmFile.GetPos_t();
  CPrintF("Time to save game %.2f, save game size is %.2f KB\n",fLoadTime,slGameSize/1024.0f);
#endif

  // Close file
  strmFile.Close();
}

/*
 * Load the game.
 *
 * remember to keep this routine up to date with CNetworkLibrary::StartPeerToPeer()
 */
void CNetworkLibrary::Load_t(const CTFileName &fnmGame) // throw char *
{
  // remove all pending sounds
  _pSound->Flush();
  

  StartProgress();

  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  CGatherCRC gc;


  ga_bLocalPause = FALSE;

  // open the file
  CTFileStream strmFile;
  strmFile.Open_t(fnmGame);

  // if starting in network
  if (_cmiComm.IsNetworkEnabled()) {
    // start gathering CRCs
    InitCRCGather();
  }

  // initialize server
  ga_srvServer.Start_t();
  ga_IsServer = TRUE;
  ga_fnmNextLevel = CTString("");
  memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);

  strmFile.ExpectID_t("GAME");
  // read session state
  try {
    ga_sesSessionState.Start_t(-1);
    ga_sesSessionState.Read_t(&strmFile);
    strmFile.ExpectID_t("GEND");   // game end
  } catch(char *) {
    StopProgress();
    ga_srvServer.Stop();
    ga_IsServer = FALSE;
    throw;
  }
  
  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // set time and pause for server from the saved game
  ga_ubNumLevelChanges++;
  ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;

  ga_srvServer.srv_tmLastProcessedTick = ga_sesSessionState.ses_tmLastProcessedTick;
  ga_srvServer.srv_iLastProcessedSequence = ga_sesSessionState.ses_iLastProcessedSequence;
  ga_srvServer.srv_bPause = ga_sesSessionState.ses_bPause;
  ga_srvServer.srv_bGameFinished = ga_sesSessionState.ses_bGameFinished;
  // start sending stream to local state
  ga_srvServer.srv_assoSessions[0].sso_bSendStream = TRUE;
  ga_srvServer.srv_assoSessions[0].sso_iLastSentSequence = ga_srvServer.srv_iLastProcessedSequence;

  // if the names match, check if it's already active
  for (INDEX iTarget=0;iTarget<NET_MAXGAMEPLAYERS;iTarget++) {
    if (ga_srvServer.srv_apltPlayers[iTarget].IsActive()) {
      // and if it is, skip it (someone is already playing with it)
      ga_srvServer.srv_apltPlayers[iTarget].Deactivate();
    }
  }

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  FinishCRCGather();
  StopProgress();
}

/*
 * Save a debugging game.
 */
void CNetworkLibrary::DebugSave(void)
{
  // try to save game
  try {
    Save_t(CTString("Save\\Debug.sav"));
  // if not successful
  } catch (char *strError){
    FatalError("Cannot save debug game:\n%s", strError);
  }
}


/*
 * Join a running multi-player game.
 */
void CNetworkLibrary::JoinSession_t(const CNetworkSession &nsSesssion, INDEX ctLocalPlayers) // throw char *
{
  TRACKMEM(Mem,"Network");
  // remove all pending sounds
  _pSound->Flush();
  StartProgress();

  // report session addres
  CPrintF( TRANS("Joining session at: '%s'\n"), nsSesssion.ns_strAddress);

  ga_bLocalPause = FALSE;

  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  // start gathering CRCs
  CGatherCRC gc;
  InitCRCGather();



  // set session name and server address
  ga_strSessionName = nsSesssion.ns_strSession;
  ga_strServerAddress = nsSesssion.ns_strAddress;
  ga_fnmNextLevel = CTString("");
  ga_fnmWorld = CTString("");
  memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);

  ga_IsServer = FALSE;

  SetProgressDescription(TRANS("connecting"));
  CallProgressHook_t(0.0f);
  // initialize session state
  try {
    ga_sesSessionState.Start_t(ctLocalPlayers);
  } catch(char *) {
    ga_World.Clear();
    FreeUnusedStock();
    StopProgress();
    throw;
  }

  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  // run main loop to let session state process messages from server
  MainLoop();

  CPrintF("  joined\n");
  StopProgress();
}

/* Start playing a demo. */
/*
void CNetworkLibrary::StartDemoPlay_t(const CTFileName &fnDemo)  // throw char *
{
  // remove all pending sounds
  _pSound->Flush();
  StartProgress();


  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  ga_bLocalPause = FALSE;

  // open the file
  ga_strmDemoPlay.Open_t(fnDemo);

  // remember that playing demo
  ga_bDemoPlay = TRUE;
  ga_bDemoPlayFinished = FALSE;

  // create session name from demo name
  CTString strSessionName = CTString("Demo: ")+fnDemo;
  ga_strSessionName = strSessionName;

  ga_IsServer = FALSE;
  // initialize server
  try {
    // read initial info from stream
    ga_strmDemoPlay.ExpectID_t("DEMO");

    // write world filename and spawn flags
    ga_strmDemoPlay>>ga_fnmWorld;
    ga_strmDemoPlay>>ga_sesSessionState.ses_ulSpawnFlags;
    // write sessin properties
    ga_strmDemoPlay.Read_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);

    ga_World.Load_t(ga_fnmWorld);
    // delete all entities that don't fit given spawn flags
    ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);

    // set overdue timers in just loaded world to be due in current time
    ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);
    
    _pNetwork->ga_World.wo_ulNextEntityID = 0x10000001;

    ga_sesSessionState.Read_t(&ga_strmDemoPlay,TRUE);

    _pNetwork->ga_strmDemoPlay.ExpectID_t("DTTM");
    ga_strmDemoPlay>>ga_tmNextDemoTick;

    extern TIME _tmLocalTick;
    _tmLocalTick = ga_tmNextDemoTick;
    ga_fDemoTimer = ga_tmNextDemoTick;
    ga_sesSessionState.ses_bRestartLocalTime = TRUE;
  } catch(char *) {
    ga_strmDemoPlay.Close();
    ga_bDemoPlay = FALSE;
    StopProgress();
    throw;
  }

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // demo synchronization starts at the beginning initially
  ga_tvDemoTimerLastTime = _pTimer->GetHighPrecisionTimer();

  // run main loop to let server process messages from host
  MainLoop();
  StopProgress();
}
	*/

/*
BOOL CNetworkLibrary::IsDemoPlayFinished(void)
{
  return ga_bDemoPlay && ga_bDemoPlayFinished;
}
*/
BOOL CNetworkLibrary::IsPlayingDemo(void)
{
  return ga_bDemoPlay;
}

/*
void CNetworkLibrary::StartDemoRec_t(const CTFileName &fnDemo) // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // if already recording
  if (ga_bDemoRec) {
    // error
    throw TRANS("Already recording a demo!");
  }

  // create the file
  ga_strmDemoRec.Create_t(fnDemo);

  // write initial info to stream
  ga_strmDemoRec.WriteID_t("DEMO");

  // write world filename and spawn flags
  ga_strmDemoRec<<ga_fnmWorld;
  ga_strmDemoRec<<ga_sesSessionState.ses_ulSpawnFlags;
  // write sessin properties
  ga_strmDemoRec.Write_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);
  // write gamestate
  ga_sesSessionState.Write_t(&ga_strmDemoRec,TRUE);

  // remember that recording demo
  ga_bDemoRec = TRUE;
}

void CNetworkLibrary::StopDemoRec(void)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // if not recording
  if (!ga_bDemoRec) {
    // do nothing
    return;
  }
  // write terminal info to the stream
  ga_strmDemoRec.WriteID_t("DEND");   // game end
  // close the file
  ga_strmDemoRec.Close();
  // remember that not recording demo
  ga_bDemoRec = FALSE;
}


BOOL CNetworkLibrary::IsRecordingDemo(void)
{
  return ga_bDemoRec;
}
*/
BOOL CNetworkLibrary::IsNetworkEnabled(void)
{ 
  return _cmiComm.IsNetworkEnabled();
}




// pause/unpause game
void CNetworkLibrary::TogglePause(void)
{
  ga_sesSessionState.ses_bWantPause = !ga_sesSessionState.ses_bWantPause;
}

// test if game is paused
BOOL CNetworkLibrary::IsPaused(void)
{
  if (this==NULL) {
    return TRUE; 
  }
  return ga_sesSessionState.ses_bWantPause;
}

// test if having connnection problems (not getting messages from server regulary)
BOOL CNetworkLibrary::IsConnectionStable(void)
{
  // if this is the server or network is not enabled
  if (IsServer() || !_cmiComm.IsNetworkEnabled() || ga_bDemoPlay) {
    // it is always stable
    return TRUE;
  }

  // check when last message was received.
  return (_pTimer->GetHighPrecisionTimer()-ga_sesSessionState.ses_tvMessageReceived).GetSeconds()<net_tmProblemsTimeout;
}
// test if completely disconnected and why
BOOL CNetworkLibrary::IsDisconnected(void)
{
  return ga_sesSessionState.ses_strDisconnected!="";
}

const CTString &CNetworkLibrary::WhyDisconnected(void)
{
  return ga_sesSessionState.ses_strDisconnected;
}

// set/get server side pause (for single player only)
void CNetworkLibrary::SetLocalPause(BOOL bPause)
{
  ga_bLocalPause = bPause;
}

BOOL CNetworkLibrary::GetLocalPause(void)
{
  if (this==NULL) {
    return TRUE;
  }
  return ga_bLocalPause;
}

// get server/client name and address
void CNetworkLibrary::GetHostName(CTString &strName, CTString &strAddress)
{
  _cmiComm.GetHostName(strName, strAddress);
}

// mark that the game has finished -- called from AI
void CNetworkLibrary::SetGameFinished(void)
{
  // remove all pending sounds
  _pSound->Flush();
  ga_tvGameFinished = _pTimer->GetHighPrecisionTimer();
  ga_sesSessionState.ses_bGameFinished = TRUE;
  if (IsServer()) {
    ga_srvServer.srv_bGameFinished = TRUE;

    INDEX iActiveSessions = ga_srvServer.ActiveRemoteSessions();
    if (iActiveSessions > 0) {
      ga_sesSessionState.ses_tmLastProcessedTick += _pTimer->TickQuantum;
      ga_srvServer.srv_tmLastProcessedTick += _pTimer->TickQuantum;
      _pNetwork->ga_srvServer.StartNewTick(ga_sesSessionState.ses_tmLastProcessedTick);
      _pNetwork->ga_srvServer.srv_embSendBuffer.RequestTickAcknowledge(ga_sesSessionState.ses_tmLastProcessedTick,iActiveSessions);
    }
  }
}

BOOL CNetworkLibrary::IsGameFinished(void)
{
  return ga_sesSessionState.ses_bGameFinished;
}

// manipulation with realtime factor for slower/faster time -- called from AI
void CNetworkLibrary::SetRealTimeFactor(FLOAT fSpeed)
{
  ga_sesSessionState.ses_fRealTimeFactor = fSpeed;
}

FLOAT CNetworkLibrary::GetRealTimeFactor(void)
{
  return ga_sesSessionState.ses_fRealTimeFactor;
}

// test if game is waiting for more players to connect
BOOL CNetworkLibrary::IsWaitingForPlayers(void)
{
  // if game mode does not include waiting for players
  if (!ga_sesSessionState.ses_bWaitAllPlayers) {
    // not waiting
    return FALSE;
  }
  // if server
  if (IsServer()) {
    // check number of players on server
    return ga_srvServer.GetPlayersCount()<ga_sesSessionState.ses_ctMaxPlayers;
  // if not server
  } else {
    // check number of players in session
    return ga_sesSessionState.GetPlayersCount()<ga_sesSessionState.ses_ctMaxPlayers;
  }
}
// test if game is waiting for server
BOOL CNetworkLibrary::IsWaitingForServer(void)
{
  return ga_sesSessionState.ses_bWaitingForServer;
}


/*
 * Stop currently running game.
 */
void CNetworkLibrary::StopGame(void)
{
	TRACKMEM(Mem,"Network");
	// remove all pending sounds and finish rendering
	_pSound->Flush();
	_pGfx->Flush();
	
	CPrintF( TRANS("stopping game.\n"));
	// access to the list of handlers must be locked
	CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, TRUE);
	
	/*
	// stop demo recording if active
	StopDemoRec();
	
	// if playing demo
	if (ga_bDemoPlay) {
		// close the demo file
		ga_strmDemoPlay.Close();
		// remember that not playing demo
		ga_bDemoPlay = FALSE;
		ga_bDemoPlayFinished = FALSE;
	}
	*/

	LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
	ClearPetList();
	_pNetwork->_PetTargetInfo.Init();
	
	// stop session
	ga_sesSessionState.Stop();
	
	// stop server
	if (ga_IsServer) {
		ga_srvServer.Stop();
		ga_IsServer = FALSE;
	}
	
	ga_strSessionName = "";
	
	ga_World.Clear();
	
	
	// free default state if existing
	if (ga_pubDefaultState!=NULL) {
		FreeMemory(ga_pubDefaultState);
		ga_pubDefaultState = NULL;
		ga_slDefaultStateSize = 0;
		memset(ga_aubDefaultProperties, 0, sizeof(ga_aubDefaultProperties));
	}
	if (ga_pubCRCList!=NULL) {
		FreeMemory(ga_pubCRCList);
		ga_pubCRCList = NULL;
		ga_slCRCList = 0;
	}
	
	{
		MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
		ga_aplsPlayers.Clear();
		ga_aplsPlayers.New(NET_MAXLOCALPLAYERS);
	}
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)NULL);
	
	ClearEntityClassContainer();
	
	FreeUnusedStock();
	
	// rewind the timer
	_pTimer->SetCurrentTick(0.0f);
}

// initiate level change
void CNetworkLibrary::ChangeLevel(
								  const CTFileName &fnmNextLevel, BOOL bRemember, INDEX iUserData)
{
	ShowCursor(FALSE);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, TRUE);
	
	// if not currently changing
	if (_lphCurrent==LCP_NOCHANGE) 
	{
		// initiate change
		ga_fnmNextLevel = fnmNextLevel;
		ga_bNextRemember = bRemember;
		ga_iNextLevelUserData = iUserData;
		_lphCurrent = LCP_INITIATED;
	}
}

// really do the level change
void CNetworkLibrary::ChangeLevel_internal(void)
{
	_pNetwork->MyCharacterInfo.itemEffect.Clear();
	_cmiComm.m_inbuf.Clear();
	//TODO : 다른 캐릭터들의 item effect도 처리해야함.
	ga_ubNumLevelChanges++;
	ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;
	
	{for (INDEX iClient=0;iClient<NET_MAXGAMECOMPUTERS;++iClient) 
	{
		CSessionSocket &sso = ga_srvServer.srv_assoSessions[iClient];
		if (sso.IsActive() && sso.sso_bSendStream) {
			sso.sso_ubNumLevelChanges++;
		}
	}}
	
	// remove all pending sounds
	_pSound->Flush();
	
	TRACKMEM(Mem,"Network");
	BOOL bMultiplayer = FALSE;
	extern BOOL _bReinitEntitiesWhileCopying;
	_bReinitEntitiesWhileCopying = FALSE;
	
	if (ga_srvServer.ActiveRemoteSessions() > 0) {
		bMultiplayer = TRUE;
	}
	
	if (ga_srvServer.srv_bRunning) {
		ga_srvServer.StopNetProcess(); 
	}
	
	StartProgress();
	
	// find all entities that are to cross to next level
	CEntitySelection* psenToCross = new CEntitySelection;
	{FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
		if (iten->en_ulFlags&ENF_CROSSESLEVELS) {
			psenToCross->Select(*iten);
		}
	}}
	
	// copy them to a temporary world
	CWorld* pwldTemp = new CWorld;
	CEntitySelection* psenInTemp = new CEntitySelection;
	pwldTemp->CopyEntities(ga_World, *psenToCross,
		*psenInTemp, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)));
	
	
	// remember characters for all player targets and disable them
	CPlayerCharacter apc[NET_MAXGAMEPLAYERS];
	BOOL abWasActive[NET_MAXGAMEPLAYERS];
	INDEX aiClientIndex[NET_MAXGAMEPLAYERS];
	CPlayerAction apaActions[NET_MAXGAMEPLAYERS][2];
	{for(INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) {
		CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
		abWasActive[i] = plt.IsActive();
		aiClientIndex[i] = plt.plt_iClient;
		if (plt.IsActive()) {
			apc[i] = plt.plt_penPlayerEntity->en_pcCharacter;
			apaActions[i][0] = plt.plt_paLastAction;
			apaActions[i][1] = plt.plt_paPreLastAction;
			plt.plt_penPlayerEntity = NULL;
			plt.Deactivate();
		}
	}}
	
	// destroy all entities that will cross level
	ga_World.DestroyEntities(*psenToCross);
	delete psenToCross;
	
	// if should remember old levels
	if (ga_bNextRemember) {
		// remember current level
		ga_sesSessionState.RememberCurrentLevel(ga_fnmWorld);
	}
	
	CGatherCRC gc;
	
	ga_World.Clear();
	
	
	// if starting in network
	//if (_cmiComm.IsNetworkEnabled()) {
    // start gathering CRCs
    FreeUnusedStock();
    InitCRCGather();
	//}
	
	/* //0522 kwon 삭제.
	if (bMultiplayer) {
    // create base info to be sent
    extern CTString ser_strMOTD;
    CNetworkMessage nmLevelChange(MSG_CHANGE_LEVEL);
    nmLevelChange<<ser_strMOTD;
    nmLevelChange<<ga_fnmNextLevel;
    nmLevelChange<<_pNetwork->ga_sesSessionState.ses_ulSpawnFlags;
    nmLevelChange.Write(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);
	
	  
		
		  // notify active clients of level change
		  for( INDEX iClient=1; iClient<ga_srvServer.srv_assoSessions.Count(); iClient++) {
		  if (ga_srvServer.srv_assoSessions[iClient].IsActive()) {
		  CPrintF("Sent levelchange to client: %d\n",iClient);
		  _pNetwork->SendToClientReliable(iClient, nmLevelChange);
		  }
		  }
		  _cmiComm.Server_Update();
		  }
	*/
	// remember original world filename
	CTFileName fnmOldWorld = ga_fnmWorld;
	// try to
	try {
		// load the new world
		_pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading	
		ga_World.Load_t(ga_fnmNextLevel);
		// delete all entities that don't fit given spawn flags
		ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
		// if failed
	} catch(char *strError) {
		// report error
		CPrintF(TRANS("Cannot change level:\n%s"), strError);
		// try to
		try {
			// load the old world
			ga_fnmNextLevel = fnmOldWorld;
			ga_World.Load_t(ga_fnmNextLevel);
			// delete all entities that don't fit given spawn flags
			ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
			// if that fails
		} catch (char *strError2) {
			StopProgress();
			// fatal error
			FatalError(
				TRANS("Cannot change level because:\n%s\n"
				"and cannot go back to original one because:\n%s"), strError, strError2);
			return;
		}
	}
	
	
	// remember the world filename
	ga_fnmWorld = ga_fnmNextLevel;
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);
	
	// set overdue timers in just loaded world to be due in current time
	ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);
	
	// copy entities from temporary world into new one
	CEntitySelection* psenCrossed = new CEntitySelection;
	ga_World.CopyEntities(*pwldTemp, *psenInTemp, 
		*psenCrossed, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)));
	delete psenInTemp;
	delete psenCrossed;
	delete pwldTemp;
	
	SetProgressDescription(TRANS("precaching"));
	CallProgressHook_t(0.0f);
	// precache data needed by entities
	if( gam_iPrecachePolicy>=PRECACHE_SMART) {
		ga_World.PrecacheEntities_t();
	}
	CallProgressHook_t(1.0f);

	// restore pointers to entities for all active player targets
	{for(INDEX i=0; i<NET_MAXGAMEPLAYERS; ++i) {
		CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
		if (abWasActive[i]) {
			plt.Activate(aiClientIndex[i]);
			plt.plt_paLastAction    = apaActions[i][0];
			plt.plt_paPreLastAction = apaActions[i][1];
			plt.AttachEntity(ga_World.FindEntityWithCharacter(apc[i]));
			plt.plt_penPlayerEntity->SetWeaponAndAnimator();
		}
	}}
	
	_bReinitEntitiesWhileCopying = TRUE;
	
	if (bMultiplayer) {
		ga_srvServer.srv_embSendBuffer.Clear();
		ga_srvServer.InitServerNetProcess();
	}
	
	// for each player
	{for( INDEX iPlayer=0; iPlayer<NET_MAXGAMEPLAYERS; ++iPlayer) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];
		if (plt.plt_bActive) {
			// add one dummy action
			CPlayerAction pa;
			pa.Clear();
			pa.pa_aRotation = plt.plt_paLastAction.pa_aRotation;
			pa.pa_aViewRotation = plt.plt_paLastAction.pa_aViewRotation;
			plt.plt_abReceived.AddAction(pa);
		}
	}}
	
	// flush stale caches
	FreeUnusedStock();
	// mark that pretouching is required
	_bNeedPretouch = TRUE;
	
	CTerrainEffect::SetTerrain( NULL );		// yjpark
	
	FinishCRCGather();
	Sleep(200);
	StopProgress();
	
	// mute sounds for one second
	_pSound->Mute(1);
}

// really do the level change
void CNetworkLibrary::ChangeLevel_client_internal_t(void)
{
  // remove all pending sounds
  _pSound->Flush();

  TRACKMEM(Mem,"Network");
  StartProgress();

  EPreLevelChange ePreChange;
  ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
  ga_sesSessionState.SendLevelChangeNotification(ePreChange);
  CEntity::HandleSentEvents();

  CGatherCRC gc;
  BOOL bPaused = _pNetwork->GetLocalPause();
  _pNetwork->SetLocalPause(TRUE);
  // start gathering CRCs
  ga_World.Clear();
  FreeUnusedStock();
  InitCRCGather();

  // remember original world filename
  CTFileName fnmOldWorld = ga_fnmWorld;
  
  // disable all player targets
  {for(INDEX i=0; i<NET_MAXGAMEPLAYERS; ++i) {
    CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
    if (plt.IsActive()) {
      plt.plt_penPlayerEntity = NULL;
      plt.Deactivate();
    }
  }} 

  // load the new world
  _pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading
  ga_World.Load_t(ga_fnmNextLevel);
  // delete all entities that don't fit given spawn flags
  ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);

  // remember the world filename
  ga_fnmWorld = ga_fnmNextLevel;
  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // set overdue timers in just loaded world to be due in current time
  ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);

  ga_sesSessionState.ses_bWaitingForServer = TRUE;

  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  FinishCRCGather();
/* //0522 kwon 삭제.
    // send data request
  CPrintF(TRANS("Sending full game state request\n"));
  CNetworkMessage nmRequestGameState(MSG_REQ_GAMESTATE);
  _pNetwork->SendToServerReliable(nmRequestGameState);
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  // wait for server's response
  CTMemoryStream strmMessage;

  _pNetwork->ga_sesSessionState.WaitStream_t(strmMessage, "data", MSG_REP_GAMESTATE);

  CTMemoryStream strmGameState;
  CzlibCompressor comp;
  comp.UnpackStream_t(strmMessage, strmGameState);

  // get the number of server level changes, and store it
  strmGameState>>ga_sesSessionState.ses_ubNumLevelChanges;

  // set proper ID sequence for entities created by this machine - used to separate 
  // server and client generated IDs
  if (!_pNetwork->IsServer()) {
    _pNetwork->ga_World.wo_ulNextEntityID = 0x10000001;
  }
  ga_sesSessionState.Read_t(&strmGameState,TRUE);

  _lphCurrent = LCP_CHANGED;
  EPostLevelChange ePostChange;
  ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
  ga_sesSessionState.SendLevelChangeNotification(ePostChange);
  CEntity::HandleSentEvents();
  _lphCurrent = LCP_NOCHANGE;

  _pNetwork->SetLocalPause(bPaused);
  StopProgress();

  ga_sesSessionState.ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer(); 

  SendActionsToServer();
  _cmiComm.Client_Update();
*/
}



// split the rcon response string into lines and send one by one to the client
static void SendAdminResponse(ULONG ulAdr, UWORD uwPort, ULONG ulCode, const CTString &strResponse)
{
  CTString str = strResponse;
  INDEX iLineCt = 0;
/* //0522 kwon 삭제.
  while (str!="") {
    CTString strLine = str;
    strLine.OnlyFirstLine();
    str.RemovePrefix(strLine);
    str.DeleteChar(0);
    if (strLine.Length()>0) { 
      CNetworkMessage nm(MSG_EXTRA);
      nm<<CTString(0, "log %u %d %s\n", ulCode, iLineCt++, strLine);
      _pNetwork->SendBroadcast(nm, ulAdr, uwPort);
    }
  }
*/
}


/*
 * Main loop.
 */
void CNetworkLibrary::MainLoop(void)
{
  TRACKMEM(Mem,"Network");


  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  if (ga_bFirstMainLoop) {
    ga_tvLastMainLoop = _pTimer->GetHighPrecisionTimer();
    ga_bFirstMainLoop = FALSE;
  }
/* //0311 삭제
  //! 1명이상 플레이 하고 있다면,
  // update network state variable (to control usage of some cvars that cannot be altered in mulit-player mode)
  _bMultiPlayer = (_pNetwork->ga_sesSessionState.GetPlayersCount() > 1);
*/
  //! 월드 체인지.
  // if should change world
  if (_lphCurrent==LCP_SIGNALLED) {
    // really do the level change here
    ChangeLevel_internal();
    _lphCurrent=LCP_CHANGED;
  }
/* //0311 삭제
  if (_bStartDemoRecordingNextTime) {
    if (!_pNetwork->IsServer()) {
      CPrintF("Demos can be recorded only on server computer or in single player mode.\n");
    } else {
      ga_srvServer.srv_tmNextAvailableDemoTimeSlot = _pTimer->GetHighPrecisionTimer().GetSeconds();
      _bStartDemoRecordingNextTime = 0.0f;

      if (!ga_bDemoRec) {
        try {
          CTString strName;
          strName.PrintF("Temp\\Recorded%02d.dem", (INDEX)dem_iRecordedNumber);
          StartDemoRec_t(strName);
          dem_iRecordedNumber+=1;
        } catch(char *strError) {
          CPrintF(TRANS("Demo recording error: %s\n"), strError);
        }
      }
    }
  }
  if (_bStopDemoRecordingNextTime) {
    _bStopDemoRecordingNextTime = 0.0f;
    if (ga_bDemoRec) {
      StopDemoRec();
    }
  }
*/
  _sfStats.StartTimer(CStatForm::STI_MAINLOOP);


  TIME tmBefore;
  CTimerValue tvNewMainLoop = _pTimer->GetHighPrecisionTimer();
  static CTimerValue tvLastAction = -1.0f;
  if (tvLastAction.GetSeconds() < 0) {
    tvLastAction = _pTimer->GetHighPrecisionTimer();
  }

  /*
  {CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  if (ga_bDemoPlay) {
    ga_sesSessionState.SetLerpFactor(CTimerValue(ga_fDemoTimer));
  } 
  */
  /*else {
    ga_sesSessionState.SetLerpFactor(tvNow);
  }}
  */
/*
  if (_pNetwork->IsPlayingDemo() && !_pNetwork->ga_bDemoPlayFinished) {
    int ctUpdates=0;
    while (_pNetwork->ga_fDemoTimer > (_pNetwork->ga_tmNextDemoTick) && !_pNetwork->ga_bDemoPlayFinished) {
      ga_sesSessionState.RunDemoTick();
      ctUpdates++;
    }
    if (ctUpdates) {
      ga_sesSessionState.HandleTimers(_pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum);
      ga_sesSessionState.HandleMovers_client();
    }
  }
*/
  //0109
  //ga_fGameRealTimeFactor:게임 시간 가속인자 =1
  //ses_fRealTimeFactor : 특수효과에 대하여 시간을 느리게 하거나 빠르게 하는 인자 =1
  //tvLastAction : 마지막 액션을 한 시간
  //tvNewMainLoop : 네트워크 메인loop에 들어온 시간.
  extern BOOL _bWorldEditorApp;
  TIME fPeriod = _pTimer->TickQuantum/(ga_fGameRealTimeFactor*ga_sesSessionState.ses_fRealTimeFactor);
  double dDeltaTime = tvNewMainLoop.GetSeconds() - ga_tvLastMainLoop.GetSeconds();

  // NOTE : 캐릭터가 제자리에서 못움직이던 문제 때문에...
  // NOTE : 원인은 dDeltaTime이 음수값이어서 그랬음...
  // NOTE : 월드 로딩이 오래걸리거나, 문제가 생길경우에...
  if( !ga_bFirstMainLoop && dDeltaTime < 0 )
  {
	  ga_tvLastMainLoop = tvNewMainLoop.GetSeconds();
	  dDeltaTime = 0.0f;
  }

  ULONG ulNumTicks = (dDeltaTime) / _pTimer->TickQuantum;

  dDeltaTime = tvNewMainLoop.GetSeconds() - tvLastAction.GetSeconds();

  // NOTE : 캐릭터가 제자리에서 못움직이던 문제 때문에...
  // NOTE : 원인은 dDeltaTime이 음수값이어서 그랬음...
  // NOTE : 월드 로딩이 오래걸리거나, 문제가 생길경우에...
  if( dDeltaTime < 0 )
  {
	  tvLastAction = tvNewMainLoop.GetSeconds();
	  dDeltaTime = 0.0f;
  }

  ULONG ulActions = (dDeltaTime) / fPeriod;
  ULONG ulClampedTicks = Clamp(ulNumTicks,(ULONG)0, (ULONG)2);
  ULONG ulClampedActions = Clamp(ulActions, (ULONG)0, (ULONG)10);
  extern INDEX net_bReportMiscErrors;
  if (net_bReportMiscErrors) 
  {
	  if ((ulNumTicks - ulClampedTicks) > 50) 
	  {
		  CPrintF(TRANS("Warning: main loop time clamp by %ld ticks!\n"),ulNumTicks-ulClampedTicks);
	  }
	  
	  if (ga_IsServer || _bWorldEditorApp) 
	  {
		  if ((ulClampedActions - ulActions) > 50) 
		  {
			  CPrintF(TRANS("Warning: main loop actions clamp by %ld ticks!\n"),ulClampedActions - ulActions);
		  }
	  }
  }

  INDEX ctTicks = 0;

  // handle messages for session state
  if (!ga_bDemoPlay) 
  {
    if (ga_IsServer) 
	{
      if (_cmiComm.Client_Update() == FALSE) 
	  {
        _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
        ga_sesSessionState.Stop();
        return;
      } 
      if ((ga_IsServer || _bWorldEditorApp) && _cmiComm.cci_bClientInitialized) 
	  {
        while (ctTicks < ulClampedActions) 
		{
		  //0109 
		  //ulActions는 지난번 액션후 밀린 액션 갯수
		  //ulClampedActions는 처리 허용 액션갯수.최대 10개.
          // make actions packet for all local players and send to server
          SendActionsToServer();
          ctTicks++;
        }
        tvLastAction += ulActions*fPeriod;
      }
  
      if (_cmiComm.Client_Update() == FALSE) 
	  {
        ga_sesSessionState.Stop();
        _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
        return;
      }
    }
  }
   
  // if this is server computer
  if (ga_IsServer) 
  {
    ctTicks=0;
    // handle server messages
    _cmiComm.Server_Update();

    while (ctTicks < ulClampedTicks) 
	{
      ga_srvServer.ServerLoop();     
      ctTicks++;
    }

    _cmiComm.Server_Update();
    ga_srvServer.HandleAll();
  }
/* //0311 삭제
//! 클라이언트 업데이트.
    if (_cmiComm.Client_Update() == FALSE) {
      ga_sesSessionState.Stop();
      _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
      return;
    }
*/
//! 세션 loop
    ga_sesSessionState.SessionStateLoop();
/* //0311 삭제     
//! 또 클라이언트 업데이트.
    if (_cmiComm.Client_Update() == FALSE) {
      ga_sesSessionState.Stop();
      _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
      return;
    }
//  }  
 */
  if (IsServer() ) {
    _cmiComm.Server_Update();
  }  

  ga_tvLastMainLoop += ulNumTicks * _pTimer->TickQuantum;

  
  // let server process game stream
  tmBefore = _pTimer->GetRealTimeTick();
  _pTimer->SetLerp(0.0f);

  if (IsServer()) {
    _cmiComm.Server_Update();
    ga_srvServer.HandleAll();
	//! 서버로 부터 오는 게임 스트림을 처리한다. 로컬클라이언트.
    // process the game stream coming from the server
    ga_sesSessionState.ProcessGameStream();
  }
/* //0311 삭제
//! tick 데이타를 클라이언트로 보낸다. 
  // sent tick data to the clients only once, and after the last tick has been processed
  if (ulClampedTicks>0 && ga_IsServer && !ga_bDemoPlay && ga_srvServer.srv_bActive) {
    ga_srvServer.ServerNetProcess();     
    _cmiComm.Server_Update();
  }
*/
  ga_sesSessionState.ses_tmLastUpdated = _pTimer->GetRealTimeTick();

  TIME tmAfter = _pTimer->GetRealTimeTick();
  ga_sesSessionState.ses_bKeepingUpWithTime = (tmAfter-tmBefore)<=_pTimer->TickQuantum*2.01f;

  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  // set the lerping factor for current frame
  if (!ga_bDemoPlay) {
    ga_sesSessionState.SetLerpFactor(tvNow);
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
	g_fGWTime += (float)(( tvNow - ga_tvDemoTimerLastTime ).GetSeconds()) * RWTOneSec2GWTSec * g_fGWTimeMul;
	if( g_fGWTime > (FLOAT)GAMETIME_ONEDAY_SECOND )
		g_fGWTime -= (FLOAT)GAMETIME_ONEDAY_SECOND * int(g_fGWTime / (FLOAT)GAMETIME_ONEDAY_SECOND);															// yjpark     -->|
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
  } else {
    ga_sesSessionState.SetLerpFactor(CTimerValue(ga_fDemoTimer));
  }
  ga_tvDemoTimerLastTime = tvNow;
/* //0311 삭제
  // if playing a demo
  if (ga_bDemoPlay) {
    // if synchronizing by real time
    if (ga_fDemoSyncRate==DEMOSYNC_REALTIME) {
      // if server is keeping up
      if (ga_sesSessionState.ses_bKeepingUpWithTime) {
        // add passed time with slow/fast factor
        ga_fDemoTimer += FLOAT((tvNow-ga_tvDemoTimerLastTime).GetSeconds())
          *ga_fDemoRealTimeFactor*ga_sesSessionState.ses_fRealTimeFactor;
      }
    // if synchronizing is stopped
    } else if (ga_fDemoSyncRate==DEMOSYNC_STOP) {
      // don't step
      NOTHING;
    // if synchronizing by given steps
    } else {
      // just add the step
      dem_iMotionBlurInterFrames = Clamp( dem_iMotionBlurInterFrames, 0L, 99L);
      const FLOAT fInterFrames = (dem_iAnimFrame<0) ? 1.0f : (dem_iMotionBlurInterFrames+1);
      ga_fDemoTimer += ga_fDemoRealTimeFactor / (ga_fDemoSyncRate*fInterFrames);
    }
  }
  // remember the demo timer
*/
/*#if _GAMESPY
  // do services for gamespy querying
  //GameSpy_ServerHandle();
#endif*/

  // if network
  if (_cmiComm.IsNetworkEnabled()) {
    // repeat
    FOREVER {
      CNetworkMessage nmReceived;

      ULONG ulFrom;
      UWORD uwPort;
      BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom, uwPort);
      // if there are no more messages
      if (!bHasMsg) {
        // finish
        break;
      }
/*
//! 서버는 이밑으로 안들어가~
      // if this message is not valid rcon message or a server enumeration request
      if (nmReceived.GetType() != MSG_EXTRA && nmReceived.GetType() != MSG_REQ_ENUMSERVERS) {
        // skip it
        continue;
      }

      // if requesting enumeration and this is server, and the server is visible and the game is not finished
      if (nmReceived.GetType()==MSG_REQ_ENUMSERVERS && IsServer() && ser_bEnumeration && !IsGameFinished()) {
      


        CTString strGameType;
        // get function that will provide us the info about gametype
        CShellSymbol *pss = _pShell->GetSymbol("GetCurrentGameTypeName",  TRUE);
        // if none
        if (pss==NULL) {
          // just give dummy info
          strGameType = "N/A";
        // if found
        } else {
          // get the info
          CTString (*pFunc)(void) = (CTString (*)(void))pss->ss_pvValue;
          strGameType = pFunc();
        }

        // create response
        CNetworkMessage nmEnum(MSG_SERVERINFO);
        nmEnum<<ga_strSessionName;
        nmEnum<<ga_World.wo_strName;
        nmEnum<<ga_srvServer.GetPlayersCount();
        nmEnum<<ga_sesSessionState.ses_ctMaxPlayers;
        nmEnum<<strGameType;
        nmEnum<<_strModName;
        nmEnum<<_SE_VER_STRING;
        nmEnum<<0.0f; // ping
      

        // send it back
        SendBroadcast(nmEnum,INADDR_BROADCAST,uwPort);
      } else {
        // get the string from the message
        CTString strMsg;
        nmReceived>>strMsg;

        // if this is server
        if (IsServer()) {
          // accept requests
          if (!strMsg.RemovePrefix("rcmd ")) {
            continue;
          }
          ULONG ulCode;
          char strPass[80];
          char strCmd[256];
          strMsg.ScanF("%u \"%80[^\"]\"%256[^\n]", &ulCode, strPass, strCmd);
          CTString strAdr = AddressToString(ulFrom);

          if (net_strAdminPassword=="" || net_strAdminPassword!=strPass) {
            CPrintF(TRANS("Server: Client '%s', Wrong password for remote administration.\n"), (const char*)strAdr);
            continue;
          }

          CPrintF(TRANS("Server: Client '%s', Admin cmd: %s\n"), (const char*)strAdr, strCmd);

          con_bCapture = TRUE;
          con_strCapture = "";
          _pShell->Execute(CTString(strCmd)+";");

          CTString strResponse = CTString(">")+strCmd+"\n"+con_strCapture;
          SendAdminResponse(ulFrom, uwPort, ulCode, strResponse);
          con_bCapture = FALSE;
          con_strCapture = "";
        }
      }
*/
    }
  }
  _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
}
//! 이함수는 로컬 플레이어를 위한것.
// make actions packet for local players and send to server
void CNetworkLibrary::SendActionsToServer(void)
{
	// make the packet
	CNetworkMessage nmAction(MSG_ACTIONS);
	
	// for all local players on this machine
	for(INDEX ipls=0; ipls<ga_aplsPlayers.Count(); ++ipls) 
	{
		CPlayerSource &pls = ga_aplsPlayers[ipls];
		// create action packet if the player exists
		pls.WriteActionPacket(nmAction);
	}
	// send the packet
	//0130 1line 지우기
	//0203
	if (_pNetwork->IsServer())
	{
		SendToServer(nmAction);
	}
	
	if (_cmiComm.Client_Update() == FALSE) 
	{
		ga_sesSessionState.Stop();
		return;
	}
}

/* Get count of local players. */
INDEX CNetworkLibrary::GetLocalPlayerCount(void)
{
  INDEX iCount = 0;
  // for all local players on this machine
  for(INDEX ipls=0; ipls<ga_aplsPlayers.Count(); ++ipls) {
    CPlayerSource &pls = ga_aplsPlayers[ipls];
    // create action packet if the player exists
    if (pls.IsActive()) {
      iCount++;
    }
  }
  return iCount;
}

/* Get player entity for a given local player. */
CEntity *CNetworkLibrary::GetLocalPlayerEntity(CPlayerSource *ppls)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // get the index of the player target in game state
  INDEX iPlayerTarget = ppls->pls_Index;
  // if player is not added
  if (iPlayerTarget<0) {
    // no entity
    return NULL;
  // if player is added
  } else {
    // get the entity from player target
    CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[iPlayerTarget];
    CPlayerEntity *pen = plt.plt_penPlayerEntity;
    return pen;
  }
}

/* Get player entity for a given player by name. */
CEntity *CNetworkLibrary::GetPlayerEntityByName(const CTString &strName)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // for each player in game
  CStaticArray<CPlayerTarget> &aplt = ga_srvServer.srv_apltPlayers;
  for(INDEX iplt = 0; iplt<aplt.Count(); ++iplt) {
    // if it is active and has that name
    if (aplt[iplt].IsActive()
      &&aplt[iplt].plt_penPlayerEntity->en_pcCharacter.GetName()==strName) {
      // return it
      return aplt[iplt].plt_penPlayerEntity;
    }
  }
  // else not found
  return NULL;
}

/* Get number of entities with given name. */
INDEX CNetworkLibrary::GetNumberOfEntitiesWithName(const CTString &strName)
{
  INDEX ctEntities = 0;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->GetName()==strName) {
      ctEntities++;
    }
  }}
  return ctEntities;
}

/* Get n-th entity with given name. */
CEntity *CNetworkLibrary::GetEntityWithName(const CTString &strName, INDEX iEntityWithThatName)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->GetName()==strName) {
      if (ctEntities==iEntityWithThatName) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return pen;
}

/* Get number of entities with given name. */
INDEX CNetworkLibrary::GetNumberOfPlayerEntitiesWithName(const CTString &strName)
{
  INDEX ctEntities = 0;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer() && iten->GetName()==strName) {
      ctEntities++;
    }
  }}
  return ctEntities;
}

/* Get n-th player entity with given name. */
CPlayerEntity *CNetworkLibrary::GetNthPlayerEntityWithName(const CTString &strName, INDEX iEntityWithThatName)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer() && iten->GetName()==strName) {      
      if (ctEntities==iEntityWithThatName) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return (CPlayerEntity*)pen;
}

/* Get n-th player entity. */
CPlayerEntity *CNetworkLibrary::GetNthPlayerEntity(INDEX iEntity)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer()) {      
      if (ctEntities==iEntity) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return (CPlayerEntity*)pen;
}

/* Get n-th entity of given class. */
CEntity *CNetworkLibrary::GetEntityOfClass(const CTString &strClass, INDEX iEntityOfThatClass)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (IsOfClass(iten, strClass)) {
      pen = iten;
      if (ctEntities==iEntityOfThatClass) {
        break;
      }
      ctEntities++;
    }
  }}
  return pen;
}

// wooss 070305 ------------------------------------------------->>
// kw : WSS_WHITEDAY_2007
CEntity* CNetworkLibrary::GetEntityByTypeIndex(SBYTE sbCharType, SLONG slCharIndex)
{
	CEntity* penEntity=NULL;
	if( sbCharType == MSG_CHAR_PC )
	{
		if( _pNetwork->MyCharacterInfo.index == slCharIndex )
		{
			penEntity = (CPlayerEntity*)CEntity::GetPlayerEntity(0);
		}
		else
		{
			for( INDEX iChar = 0; iChar < _pNetwork->ga_srvServer.srv_actCha.Count(); iChar++ )
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iChar];
				if( ct.cha_Index == slCharIndex )
				{
					penEntity = ct.cha_pEntity;
				}
			}
		}
	}
	else if( sbCharType == MSG_CHAR_NPC )
	{
		for( INDEX iMob = 0; iMob < _pNetwork->ga_srvServer.srv_amtMob.Count(); iMob++ )
		{
			CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iMob];
			if( mt.mob_Index == slCharIndex )
			{
				penEntity = mt.mob_pEntity;					
			}
		}
	}
	else if( sbCharType == MSG_CHAR_ELEMENTAL )
	{
		for( INDEX iSlave = 0; iSlave < _pNetwork->ga_srvServer.srv_actSlave.Count(); iSlave++ )
		{
			CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[iSlave];
			if( st.slave_Index == slCharIndex )
			{
				penEntity = st.slave_pEntity;					
			}
		}
	}

	return penEntity;
}

// --------------------------------------------------------------<<

// wooss 070305 ------------------------------------------------->>
// kw		:	WSS_EVENT_LOD
// Argument	:	Server Country Code
// return	: 	Client Country Code
	
	//Clinet Define country
	//KOREA					0		// 한국
	//TAIWAN				1		// 대만
	//CHINA					2		// 중국
	//THAILAND				3		// 태국
	//TAIWAN2				4		// 대만 천하대란 wooss 050929
	//JAPAN					5		// 일본 wooss 051123
	//MALAYSIA    			6
	//USA					7		// 미국
	//BRAZIL				8		// 브라질
	//HONGKONG				9		// 홍콩

	//Sever Define country
	//LC_KOR		0	// 국내
	//LC_TWN		1	// 대만 - 혁명
	//LC_TWN2		2	// 대만 - 천하대란, LC_TWN과 동시에 설정해야 함
	//LC_CHN		3	// 중국
	//LC_TLD		4	// 태국
	//LC_TLD_ENG	5	// 태국 영어
	//LC_JPN		6 	// 일본
	//LC_MAL		7 	// 말레이시아
	//LC_MAL_ENG	8 	// 말레이시아 영어 
	//LC_USA		9	// 미국
	//LC_BRZ		10	// 브라질
	//LC_HBK		11	// 홍콩

int CNetworkLibrary::ReturnCCC(int iSCC)
{
	int iCCC;
	switch(iSCC)
	{
		case 0:
			iCCC = KOREA;
			break;
		case 1:
			iCCC = TAIWAN;
			break;
		case 2:
			iCCC = TAIWAN2;
			break;
		case 3:
			iCCC = CHINA;
			break;
		case 4:
		case 5:
			iCCC = THAILAND;
			break;
		case 6:
			iCCC = JAPAN;
			break;
		case 7:		
		case 8:
			iCCC = MALAYSIA;
			break;
		case 9:
			iCCC = USA;
			break;
		case 10:
			iCCC = BRAZIL;
			break;
		case 11:
		case 12:
			iCCC = HONGKONG;
			break;
		case 13:
			iCCC = GERMANY;
			break;
		case 14://FRANCE_SPAIN_CLOSEBETA_NA_20081124
			iCCC = SPAIN;
			break;
		case 15:
			iCCC = FRANCE;
			break;
		case 16:
			iCCC = POLAND;
			break;
		case 17:
			iCCC = RUSSIA;
			break;
		case 18:
			iCCC = TURKEY;
			break;
		default : 
			iCCC = -1;
	}

	return iCCC;
}
// --------------------------------------------------------------<<



/* Test if a given player is local to this computer. */
BOOL CNetworkLibrary::IsPlayerLocal(CEntity *pen)
{
  return GetPlayerSource(pen)!=NULL;
}
// get player source for a given player if it is local to this computer
CPlayerSource *CNetworkLibrary::GetPlayerSource(CEntity *pen)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // for all local player on this machine
  {FOREACHINSTATICARRAY(ga_aplsPlayers, CPlayerSource, itpls) {
    // get the index of the player target in game state
    INDEX iPlayerTarget = itpls->pls_Index;
    // if player is added
    if (iPlayerTarget>=0) {
      // get the player target
      CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[iPlayerTarget];
      // if it is that one
      if (plt.plt_penPlayerEntity == pen) {
        // return it
        return itpls;
      }
    }
  }}

  // if not found, it is not local
  return NULL;
}

// get game time in currently running game
TIME CNetworkLibrary::GetGameTime(void)
{
  return ga_sesSessionState.ses_tmLastProcessedTick;
}

/*
 * Add a new client to game.
 */
CPlayerSource *CNetworkLibrary::AddPlayer_t(CPlayerCharacter &pcCharacter)  // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
//  CPrintF( TRANS("Adding player: '%s'\n"), RemoveSpecialCodes(pcCharacter.GetNameForPrinting()));

  // for all local clients on this machine
	FOREACHINSTATICARRAY(ga_aplsPlayers, CPlayerSource, itcls) 
	{
		if(itcls->IsActive())
		{
			itcls->Stop();
		}
    // if client is not active
		if (!itcls->IsActive()) 
		{
      // activate it
      itcls->Start_t(pcCharacter);
//      CPrintF( TRANS("  done.\n"));
      return &itcls.Current();
    }
  }
  // number of local clients is limited with NET_MAXLOCALCLIENTS
  ASSERTALWAYS("Adding too much local clients!");
  throw TRANS("Cannot add more local clients");
  return NULL;
}

/* Get session properties for current game. */
void *CNetworkLibrary::GetSessionProperties(void)
{
  // synchronize access to network (no need for that one!)
  //CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  return ga_aubProperties;
}

/* Send chat message from some players to some other players. */
/*
void CNetworkLibrary::SendChat(ULONG ulFrom, ULONG ulTo, const CTString &strMessage)
{
  // if the string is too long
  if (strlen(strMessage)>256) {
    // refuse it
    return;
  }

  // just make the message and send it to server
  CNetworkMessage nm(MSG_CHAT_IN);
  nm<<ulFrom;
  nm<<ulTo;
  nm<<strMessage;
  SendToServer(nm);
}
*/

void CNetworkLibrary::SendChat(ULONG ulFrom, ULONG ulTo, const CTString &strMessage)
{
  // if the string is too long
  if (strlen(strMessage)>256) {
    // refuse it
    return;
  }

  // just make the message and send it to server
//  CNetworkMessage nm(MSG_CHAT_IN);
//  nm<<ulFrom;
//  nm<<ulTo;

// 채팅				: chat_type(uc) sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)												
  ULONG recvIndex = -1;
  CTString	strRecvName="";
  /*
  	for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_actCha.Count(); ipl++) 
	{
		CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						
		if (ct.cha_strName == strRecvName) 
		{
			recvIndex = ct.cha_Index;
			break;
		}
	}
	if(recvIndex==-1)//만약 찾지 못했다면,
	{
		return;
	}
  */
  CNetworkMessage nm(MSG_CHAT);
  nm<< (unsigned char)MSG_CHAT_SAY; //0528 kwon
  nm<< MyCharacterInfo.index;
  nm<< MyCharacterInfo.name;
  nm<< strRecvName;
  nm<<strMessage;
//  SendToServer(nm);
  SendToServerNew(nm);
}

// save current version of engine
void CNetworkLibrary::WriteVersion_t(CTStream &strm)
{
  strm.WriteID_t("BUIV"); // build version
  strm<<INDEX(_SE_BUILD_MAJOR);
}

// load version of engine saved in file and check against current
void CNetworkLibrary::CheckVersion_t(CTStream &strm, BOOL bAllowReinit, BOOL &bNeedsReinit)
{
  // if not saved
  if (strm.PeekID_t()!=CChunkID("BUIV")) { // build version
    // behave as if everything is ok (for old versions)
    bNeedsReinit = FALSE;
    return;
  }
  strm.ExpectID_t("BUIV"); // build version
  // read the saved one
  INDEX iSaved;
  strm>>iSaved;
  // get current one
  INDEX iCurrent = _SE_BUILD_MAJOR;
  // if current version is an internal build
  if (iCurrent==0) {
    // it is never forced to reinit
    bNeedsReinit = FALSE;
    return;
  }

  // if current version is older than the saved one
  if (iCurrent<iSaved) {
    // it cannot be reinitialized
    ThrowF_t(TRANS("File '%s' was saved by a newer version of engine, it cannot be loaded"),
      strm.GetDescription());
    return;
  }

  // if current version is same as the saved one
  if (iCurrent==iSaved) {
    // all ok
    bNeedsReinit = FALSE;
    return;
  }

  // if current version is newer than the saved one
  if (iCurrent>iSaved) {
    // it should be reinitialized
    bNeedsReinit = TRUE;

    // if it may not be reinitialized
    if (!bAllowReinit) {
      ThrowF_t(TRANS("File '%s' was saved by an older version of engine, it cannot be loaded"),
        strm.GetDescription());
    }
    return;
  }
  // this should not happen
  ASSERT(FALSE);
  bNeedsReinit = FALSE;
  return;
}

// add a value to the netgraph
void CNetworkLibrary::AddNetGraphValue(enum NetGraphEntryType nget, FLOAT fLatency)
{
  net_iGraphBuffer = Clamp(net_iGraphBuffer, INDEX(20), INDEX(1000));

  // make sure the netgraph has wanted number of values
  if (ga_angeNetGraph.Count()!=net_iGraphBuffer) {
    ga_angeNetGraph.Clear();
    ga_angeNetGraph.New(net_iGraphBuffer);
    memset(&ga_angeNetGraph[0], 0, ga_angeNetGraph.Count()*sizeof(ga_angeNetGraph[0]));
  }

  // scroll the values in the netgraph by one value
  memmove(&ga_angeNetGraph[1], &ga_angeNetGraph[0], (ga_angeNetGraph.Count()-1)*sizeof(ga_angeNetGraph[0]));

  // add the new value
  ga_angeNetGraph[0].nge_ngetType = nget;
  ga_angeNetGraph[0].nge_fLatency = fLatency;
}

 
// add a value to the bandwidth graph
void CNetworkLibrary::AddBandwidthGraphValue(ULONG ulBandwidth)
{
  net_iGraphBuffer = Clamp(net_iGraphBuffer, INDEX(20), INDEX(1000));

  // make sure the netgraph has wanted number of values
  if (ga_aulBandwidthGraph.Count()!=net_iGraphBuffer) {
    ga_aulBandwidthGraph.Clear();
    ga_aulBandwidthGraph.New(net_iGraphBuffer);
    memset(&ga_aulBandwidthGraph[0], 0, ga_aulBandwidthGraph.Count()*sizeof(ga_aulBandwidthGraph[0]));
  }

  // scroll the values in the netgraph by one value
  memmove(&ga_aulBandwidthGraph[1], &ga_aulBandwidthGraph[0], (ga_aulBandwidthGraph.Count()-1)*sizeof(ga_aulBandwidthGraph[0]));

  // add the new value
  ga_aulBandwidthGraph[0] = ulBandwidth;
}
  
//! 게임이 아직 실행되지 않았을때는 브로드캐스트 메시지만 받는다.
// handle broadcast messages (server enumeration)
void CNetworkLibrary::GameInactive(void)
{
//#if _GAMESPY
//	GameSpy_EnumHandle();
//#endif

  // if no network
  if (!_cmiComm.IsNetworkEnabled()) {
    // do not handle
    return;
  }

  // repeat
  FOREVER {
    CNetworkMessage nmReceived;

    ULONG ulFrom;
    UWORD uwPort;
    BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom, uwPort);
    // if there are no more messages
    if (!bHasMsg) {
      // finish
      break;
    }
  }
}

void CNetworkLibrary::InitCRCGather(void)
{
  CRCT_ResetActiveList();
  CRCT_bGatherCRCs = TRUE;  
  
  // clear the container that holds pairs of entity class ID & entity class*
  ClearEntityClassContainer();

  CRCT_AddFile_t(CTString("Classes\\Player.ecl"));

  // add the Player to entity class contianer
  EntityClassEntry* peceEntry = new EntityClassEntry;
  peceEntry->ece_fnmEntityClass = CTString("Classes\\Player.ecl");
  peceEntry->ece_iClassID = 401;
  _cEntityClassContainer.Add(peceEntry);

  try {
    _pEntityClassStock->Release(_pEntityClassStock->Obtain_t(CTString("Classes\\Player.ecl")));
  } catch (char *strError) {
    CPrintF("WARNING: %s", strError);
  }

}

// finish gathering of file CRCs to CRC table (call for server only!)
void CNetworkLibrary::FinishCRCGather(void)
{
  try {
    // make the list
    CTMemoryStream strmCRC;
    CRCT_MakeFileList_t(strmCRC);
    // remember it
    strmCRC.SetPos_t(0);
    ga_slCRCList = strmCRC.GetStreamSize();
    ga_pubCRCList = (UBYTE*)AllocMemory(ga_slCRCList);
    strmCRC.Read_t(ga_pubCRCList, ga_slCRCList);
    // remember its CRC
    strmCRC.SetPos_t(0);
    ga_ulCRC = CRCT_MakeCRCForFiles_t(strmCRC);
  } catch (char *strError) {
    CPrintF(TRANS("Warning, cannot get CRCs: %s\n"), strError);
  }
}

/* Start numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsStart(BOOL bInternet)
{
  _bEnumerating = TRUE;
  // clear old list
  FORDELETELIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
    delete &*itns;
  }
	/*
// gamespy enumeration
#if _GAMESPY 
  // make sure network is on
  if (!_cmiComm.IsNetworkEnabled()) {
  _cmiComm.PrepareForUse(TRUE, FALSE); // have to enumerate as server
  }
  // request enumeration
  GameSpy_EnumTrigger(bInternet);
// Croteam enumeration 
#else
	*/
  // request enumeration
  _bSendEnumRequest = TRUE;
  // make sure network is on
  if (!_cmiComm.IsNetworkEnabled()) {
    _cmiComm.PrepareForUse(/*network*/TRUE, /*client*/TRUE); 
  }
  if (!ga_srvServer.srv_bActive) {
    ga_srvServer.Start_t();
  }
//#endif
}

/* Continue numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsContinue()
{
/* //0522 kwon 삭제.
  // we will send enumeration requests two times per second
  const TIME tmRequestInterval = 0.75f;
  static TIME tmLastRequest = -1.0f;
  TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
  // if we have never enumerated before, or a enough time has passed since last request
  if (tmLastRequest<0 || ((tmNow-tmLastRequest) >= tmRequestInterval)) {
    // send enumeration request
    tmLastRequest = tmNow;
    CNetworkMessage nmEnum(MSG_REQ_ENUMSERVERS);
    SendBroadcast(nmEnum,INADDR_BROADCAST,net_iPort);
  }
  ga_bEnumerationChange = FALSE;
  // repeat
  FOREVER {
    CNetworkMessage nmReceived;
    // manage input/output buffers
    _cmiComm.Server_Update();
    ULONG ulFrom; UWORD uwPort;
    BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom,uwPort);
    // if there are no more messages
    if (!bHasMsg) {
      // finish
      break;
    } 
    // received a response to an enumeration request
    if (nmReceived.GetType()==MSG_SERVERINFO) {
      BOOL bFound = FALSE;
      
      //char strPort[8];
      //itoa(uwPort,strPort,10);
      //CTString strAddress = AddressToString(ulFrom) + CTString(":") + CTString(strPort);
      // create a new session
      CNetworkSession &nsNew = *new CNetworkSession;      

      // read it
      nmReceived>>nsNew.ns_strSession;
      nmReceived>>nsNew.ns_strWorld;
      nmReceived>>nsNew.ns_ctPlayers;
      nmReceived>>nsNew.ns_ctMaxPlayers;
      nmReceived>>nsNew.ns_strGameType;
      nmReceived>>nsNew.ns_strMod;
      nmReceived>>nsNew.ns_strVer;
      nmReceived>>nsNew.ns_tmPing;


      // try the find the server that responded in the list of existing sessions 
      FOREACHINLIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
        CNetworkSession &ns = *itns;

        BOOL bSameAddr = FALSE;
          if ((ns.ns_ulAddress == ulFrom) && (ns.ns_uwPort == uwPort)) {
            bSameAddr = TRUE;
          }
        
        // if the server is found
        if (bSameAddr) {
          // if there was a change in the number of players, flag it
          if (ns.ns_ctPlayers != nsNew.ns_ctPlayers) {
            ga_bEnumerationChange = TRUE;
          }
          //update it's data
          ns.ns_strSession = nsNew.ns_strSession;
          ns.ns_strWorld = nsNew.ns_strWorld;
          ns.ns_ctPlayers = nsNew.ns_ctPlayers;
          ns.ns_ctMaxPlayers = nsNew.ns_ctMaxPlayers;
          ns.ns_strGameType = nsNew.ns_strGameType;
          ns.ns_strMod = nsNew.ns_strMod;
          ns.ns_strVer = nsNew.ns_strVer;
          ns.ns_tmPing = nsNew.ns_tmPing;

            ns.ns_tmLastSeen = tmNow;
          bFound = TRUE;
          delete &nsNew;
          break;
        }
      }

      // if the server was not listed already, add it
      if (!bFound) {
        char strPort[8];
        itoa(uwPort,strPort,10);
        CTString strAddress = AddressToString(ulFrom) + CTString(":") + CTString(strPort);    
        nsNew.ns_ulAddress = ulFrom;
        nsNew.ns_uwPort = uwPort;
        nsNew.ns_tmLastSeen = tmNow;
        nsNew.ns_strAddress = strAddress;
        ga_lhEnumeratedSessions.AddTail(nsNew.ns_lnNode);            
        ga_bEnumerationChange = TRUE;
      }
    }
  }

  // run through the list and delete stale sessions
  FORDELETELIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
    CNetworkSession &ns = *itns;
    if (ns.ns_tmLastSeen < 0.0f || ((tmNow - ns.ns_tmLastSeen) > 2*tmRequestInterval)) {
      delete &*itns;
      ga_bEnumerationChange = TRUE;
    }
  }
  */
}


/* Stop numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsStop()
{
#if !_GAMESPY
  if (ga_srvServer.srv_bActive) {
    ga_srvServer.Stop();
  }

  _cmiComm.Unprepare();
  _bEnumerating = FALSE;
#endif
}

//0105
void CNetworkLibrary::SendMessagetoServer(char *msg)
{
	char Buf[1024];
	
	sprintf(Buf, "say %s\n", msg);
	_tcpip.SendNetMessage(Buf, TRUE);
}
//..

/****   Inventory   **********************************************************/
void  CNetworkLibrary::SetMyCurrentWearing(int tabId, int rowId, int colId)
{
	//= MySlotItem[tabId][rowId][colId]
	CItemData& ItemData = _pNetwork->MySlotItem[tabId][rowId][colId].ItemData;
	CItems& Items = _pNetwork->MySlotItem[tabId][rowId][colId];
	
	if(ItemData.GetType()==CItemData::ITEM_SHIELD)
	{
		switch(ItemData.GetSubType())
		{
		case CItemData::ITEM_SHIELD_HEAD://머리
			Items.Item_Wearing = WEAR_HELMET;
			pMyCurrentWearing[WEAR_HELMET] = &Items;
			break;
		case CItemData::ITEM_SHIELD_COAT:// 상의		
			Items.Item_Wearing = WEAR_JACKET;
			//MyCurrentWearing[ARMOR] =Items;
			pMyCurrentWearing[WEAR_JACKET] = &Items;
			break;
		case CItemData::ITEM_SHIELD_PANTS:// 하의			
			Items.Item_Wearing = WEAR_PANTS;
			//MyCurrentWearing[PANTS] = Items;
			pMyCurrentWearing[WEAR_PANTS] = &Items;
			break;
		case CItemData::ITEM_SHIELD_GLOVE:// 장갑			
			Items.Item_Wearing = WEAR_GLOVES;
			//MyCurrentWearing[GLOVE] = Items;
			pMyCurrentWearing[WEAR_GLOVES] = &Items;
			break;
		case CItemData::ITEM_SHIELD_SHOES:// 신발			
			Items.Item_Wearing = WEAR_BOOTS;
			//MyCurrentWearing[BOOTS] = Items;
			pMyCurrentWearing[WEAR_BOOTS] = &Items;
			break;		
		case CItemData::ITEM_SHIELD_SHIELD:// 방패.			
			Items.Item_Wearing = WEAR_SHIELD;
			//MyCurrentWearing[SHIELD] = Items;
			pMyCurrentWearing[WEAR_SHIELD] = &Items;
			break;
		}
	} 
	else if(ItemData.GetType()==CItemData::ITEM_WEAPON) //0609 kwon
	{	
		const BOOL bExtension = _pNetwork->IsExtensionState( _pNetwork->MyCharacterInfo.job, ItemData );
		_pNetwork->MyCharacterInfo.bExtension = bExtension;

		if( pMyCurrentWearing[WEAR_WEAPON] && 
			pMyCurrentWearing[WEAR_WEAPON]->Item_Index == Items.Item_Index )
		{
			return;
		}

		Items.Item_Wearing = WEAR_WEAPON;
		//MyCurrentWearing[WEAPON] = Items;
		pMyCurrentWearing[WEAR_WEAPON] = &Items;
	}
	else if( ItemData.GetType() == CItemData::ITEM_ACCESSORY )
	{
		//MyCurrentWearing[Items.Item_Wearing] = Items;
		pMyCurrentWearing[Items.Item_Wearing] = &Items;
	}

//[ttos_2009_1_15]:쿨타임 감소 및 MP량 감소 옵션을 클라에서 계산하지 않고 서버에서 받음
 	_pUIMgr->SetCoolTimeReductionRate(0);
	_pUIMgr->SetNeedMPReductionRate(0);

	CItems rItems =_pNetwork->MySlotItem[tabId][rowId][colId];
	for(SBYTE sbOption=0; sbOption<MAX_ITEM_OPTION; ++sbOption)
	{
		//옵션중에 스킬쿨타임 감소(Index: 58)나 MP소모량 감소(Index: 59)가 있으면 옵션값 설정...
		SBYTE sbOptionType =rItems.GetOptionType(sbOption);

		if( sbOptionType ==-1)
			break;
		/**/
		if( sbOptionType ==58 || sbOptionType ==59)
		{
			COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType );
			SBYTE sbOptionLevel =rItems.GetOptionLevel(sbOption);							
			int nOptionValue =odItem.GetValue( sbOptionLevel - 1 );
			
			if( sbOptionType ==58 )
				_pUIMgr->AddCoolTimeReductionRate(nOptionValue);
			if( sbOptionType ==59 )
				_pUIMgr->AddNeedMPReductionRate(nOptionValue);
		}/**/
	}
}

void  CNetworkLibrary::DeleteMyCurrentWearing(int weartype)
{
	if(weartype==-1)
	{
		return;
	}
	SBYTE Tab,Row,Col;
	if(_pNetwork->pMyCurrentWearing[weartype])
	{
		Tab = _pNetwork->pMyCurrentWearing[weartype]->Item_Tab;
		Row = _pNetwork->pMyCurrentWearing[weartype]->Item_Row;
		Col = _pNetwork->pMyCurrentWearing[weartype]->Item_Col;	
		
		//Tab = _pNetwork->MyCurrentWearing[weartype].Item_Tab;
		//Row = _pNetwork->MyCurrentWearing[weartype].Item_Row;
		//Col = _pNetwork->MyCurrentWearing[weartype].Item_Col;
		//	CPrintF(TRANS("DeleteMyCurrentWearing : weartype=%d Tab=%d,Row=%d,Col=%d \n"), weartype,Tab,Row,Col);
		if(Tab==-1 || Row==-1 || Col==-1)
		{
			return;
		}
		MySlotItem[Tab][Row][Col].Item_Wearing = -1; //착용이 해제되었다는 뜻.


		//옵션중에 스킬쿨타임 감소(Index: 58)나 MP소모량 감소(Index: 59)가 있으면 옵션값 0으로 설정...
		//[ttos_2009_1_15]:쿨타임 감소 및 MP량 감소 옵션을 클라에서 계산하지 않고 서버에서 받음
		CItems* rItems =_pNetwork->pMyCurrentWearing[weartype];

		for(SBYTE sbOption=0; sbOption<(SBYTE)MAX_ITEM_OPTION; ++sbOption)
		{
			SBYTE sbOptionType =rItems->GetOptionType(sbOption);
			if( sbOptionType ==-1)
				break;
			/**/
			if( sbOptionType ==58 || sbOptionType ==59)
			{
				COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType );
				SBYTE sbOptionLevel =rItems->GetOptionLevel(sbOption);							
				int nOptionValue =odItem.GetValue( sbOptionLevel - 1 );
				
				if( sbOptionType ==58 )
					_pUIMgr->AddCoolTimeReductionRate(-nOptionValue);
				if( sbOptionType ==59 )
					_pUIMgr->AddNeedMPReductionRate(-nOptionValue);
			}/**/
		}
	}

	if( weartype == WEAR_WEAPON )
	{
		_pNetwork->MyCharacterInfo.bExtension = FALSE;
	}
	
	//_pNetwork->MyCurrentWearing[weartype].Init(); // wear slot의 갑옷을 지우기.
	_pNetwork->pMyCurrentWearing[weartype] = NULL;
}

void CNetworkLibrary::DropItem(int tabId, int rowId, int colId, SQUAD cnt)
{
	CItems& Items = MySlotItem[tabId][rowId][colId];
	//0616 kwon 입고있는 아이템은 드롭 못하게 하기.
	if(Items.Item_Wearing != -1)
	{
//		CPrintF(TRANS("Can't Drop because this Item is Wearing... \n"));
		return;
	}
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_THROW;							
	nmItem << Items.Item_Tab;
	nmItem << Items.Item_Row;
	nmItem << Items.Item_Col;
	nmItem << Items.Item_UniIndex;
	nmItem << cnt;//Items.Item_Sum; //0621 kwon 수정.
	SendToServerNew(nmItem);
}

void CNetworkLibrary::SwapItem(int tabId, int rowId, int colId, int rowId2, int colId2)
{
	CItems& Items = MySlotItem[tabId][rowId][colId]; //source
	CItems& Items2 = MySlotItem[tabId][rowId2][colId2];//target
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_SWAP;							
	nmItem << Items.Item_Tab;
	nmItem << Items.Item_Row;
	nmItem << Items.Item_Col;
	nmItem << Items.Item_UniIndex;
	nmItem << (SBYTE)rowId2;//Items.Item_Row;
	nmItem << (SBYTE)colId2;//Items.Item_Col;
	nmItem << Items2.Item_UniIndex;
	SendToServerNew(nmItem);
//	CPrintF(TRANS("Send MSG_ITEM_SWAP : ITEM [%d][%d][%d] -> [%d][%d][%d]  Swaped!!! \n"),tabId,rowId,colId,tabId,rowId2,colId2);
}

void CNetworkLibrary::ArrangeItem(int tab)
{				 
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_ARRANGE;								
	nmItem << (SBYTE)tab;
	SendToServerNew(nmItem);

	_pUIMgr->GetInventory()->ClearAllItems( tab );
//	CPrintF(TRANS("Send MSG_ITEM_ARRANGE!!! \n"));		
	//0610 kwon 초기화.		
	
/*
	for(int i =0; i<TOTAL_WEAR;++i)
	{			
		ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->DeleteCurrentArmor(i);//1005 아이템 깨지는 버그수정.
		//MyCurrentWearing[i].Init();
		pMyCurrentWearing[i] = NULL;
	}

	for(int row=0;row<TOTAL_ROW;++row)
	{
		for(int col=0;col<TOTAL_COL;++col)
		{
			MySlotItem[tab][row][col].Init();
		}
	}
*/
 //메시지 온후에 처리.
}

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UpgradeItem( SBYTE sbRow1, SBYTE sbCol1, SLONG slPlus,
									SBYTE sbRow2, SBYTE sbCol2, SLONG slLevel, 
									INDEX iProtect, SBYTE sbRow3, SBYTE sbCol3 )
{				 
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_UPGRADE_REQ;								
	nmItem << sbRow1;
	nmItem << sbCol1;
	nmItem << slPlus;
	nmItem << sbRow2;
	nmItem << sbCol2;
	nmItem << slLevel;
	
	// [070824: Su-won] REFINE_PROTECT
#ifdef REFINE_PROTECT
	nmItem << iProtect;		//제련에 사용할 제련보호석 아이템 인덱스
	nmItem << sbRow3;		//사용한 제련보호석의 인벤 위치
	nmItem << sbCol3;
#endif

	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ItemLevelDown(SBYTE sbRow1, SBYTE sbCol1, SBYTE sbRow2, SBYTE sbCol2)
{
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_LEVELDOWN;
	nmItem << sbRow1;
	nmItem << sbCol1;
	nmItem << sbRow2;
	nmItem << sbCol2;
	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : OptionAddItem()
// Desc : 블러드 아이템 & 정화 옵션 추가
// ----------------------------------------------------------------------------
void CNetworkLibrary::OptionAddItem( SBYTE sbRow1, SBYTE sbCol1, SBYTE slWearPos,
									SBYTE sbRow2, SBYTE sbCol2 )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_OPTION_ADD_REQ;
	nmItem << sbRow1;
	nmItem << sbCol1;
	nmItem << slWearPos;
	nmItem << sbRow2;
	nmItem << sbCol2;
	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : OptionDelItem()
// Desc : 블러드 아이템 & 정화 옵션 제거
// ----------------------------------------------------------------------------
void CNetworkLibrary::OptionDelItem( SBYTE sbRow1, SBYTE sbCol1, SBYTE slWearPos,
									SBYTE sbRow2, SBYTE sbCol2 )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_OPTION_DEL_REQ;
	nmItem << sbRow1;
	nmItem << sbCol1;
	nmItem << slWearPos;
	nmItem << sbRow2;
	nmItem << sbCol2;
	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : HasItem()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CNetworkLibrary::HasItem( int iIndex )
{
	// 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )
	for ( int nTab = 0; nTab < INVEN_SLOT_TAB; nTab++ )
	{
		for ( int nRow = 0 ; nRow < INVEN_SLOT_ROW_TOTAL; nRow++ )
		{
			for ( int nCol = 0 ; nCol < INVEN_SLOT_COL; nCol++ )
			{
				CItems& rItem = _pNetwork->MySlotItem[nTab][nRow][nCol];
				
				if ( iIndex == rItem.Item_Index )
				{
					return TRUE;
				}		
			}		
		}
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : RefineReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::RefineReq( SBYTE sbRow, SBYTE sbCol )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_REFINE_REQ;								
	nmItem << sbRow;
	nmItem << sbCol;
	SendToServerNew(nmItem);
}

// 이기환 수정 시작 ( 12. 6 ) : CNeedItems 배열 형태로 변환
// ----------------------------------------------------------------------------
// Name : ProcessReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ProcessReq( SBYTE sbRow, SBYTE sbCol, 
								 SLONG slResultIndex, SLONG slResultCount, 
								 CNeedItems* NeedItems )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	
	nmItem << (SBYTE)MSG_ITEM_PROCESS_REQ;								
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << slResultIndex;
	nmItem << slResultCount;

	for(int i = 0 ; i < slResultCount; ++i)
	{
		nmItem << NeedItems[i].sbMatTab;
		nmItem << NeedItems[i].sbMatRow;
		nmItem << NeedItems[i].sbMatCol;
		nmItem << NeedItems[i].llCount;
	}
	
	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : ProductReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ProductReq( SBYTE sbRow, SBYTE sbCol, 
								 SLONG slResultIndex, SLONG slResultCount, 
								 CNeedItems* NeedItems )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	
	nmItem << (SBYTE)MSG_ITEM_MAKE_REQ;								
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << slResultIndex;
	nmItem << slResultCount;

	for(int i = 0 ; i < slResultCount; ++i)
	{
		nmItem << NeedItems[i].sbMatTab;
		nmItem << NeedItems[i].sbMatRow;
		nmItem << NeedItems[i].sbMatCol;
		nmItem << NeedItems[i].llCount;
	}
	
	SendToServerNew(nmItem);
}
// 이기환 수정 끝 ( 12. 6 )

// 이기환 수정 시작 ( 12. 8 ) : 조합
// ----------------------------------------------------------------------------
// Name : MixReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::MixReq( SBYTE sbTextRow, SBYTE sbTextCol, SBYTE* sbRow, SBYTE* sbCol )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	
	nmItem << (SBYTE)MSG_ITEM_MIX_REQ;	
	
	nmItem << sbTextRow;
	nmItem << sbTextCol;
		
	for(int i = 0 ; i < MIX_ITEM_SLOT_COUNT; ++i)
	{
		nmItem << sbRow[i];
		nmItem << sbCol[i];
	}
	
	SendToServerNew(nmItem);
}
// 이기환 수정 끝 (04.12.18)


//------------------------------------------------------------------------------
// CNetworkLibrary::CompoundReq
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::CompoundReq( SBYTE sbTextRow, SBYTE sbTextCol, SBYTE* sbRow, SBYTE* sbCol )
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	
	nmItem << (SBYTE)MSG_ITEM_ARCANE_REQ;	
	
	nmItem << sbTextRow;
	nmItem << sbTextCol;
		
	for(int i = 0 ; i < COMPOUND_ITEM_SLOT_COUNT; ++i)
	{
		nmItem << sbRow[i];
		nmItem << sbCol[i];
	}
	
	SendToServerNew(nmItem);
}
// ----------------------------------------------------------------------------
// Name : SendProlongMessage() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendProlongMessage( int tabId, int rowId, int colId)
{
	CItems& Items = MySlotItem[tabId][rowId][colId];

	if(Items.Item_Index ==-1) //비어있는 슬롯이다.
		return;
	
	CNetworkMessage nmItem(MSG_ITEM); 	
			nmItem << (SBYTE)MSG_ITEM_USE_PROLONG;				
			nmItem << (UBYTE)tabId;
			nmItem << (UBYTE)rowId;
			nmItem << (UBYTE)colId;
			nmItem << (ULONG)Items.Item_UniIndex;
			
	SendToServerNew(nmItem);
	
}

// ----------------------------------------------------------------------------
// Name : SendCashItemMessage() wooss 060105
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendCashItemMessage(int nType)
{
	int tv_i,nCnt;
	CNetworkMessage nmItem(MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)nType;
	switch(nType)
	{
		case MSG_EX_CASHITEM_BALANCE_REQ :
			SendToServerNew(nmItem);
			break;

		case MSG_EX_CASHITEM_PURCHASE_REQ :
			{
				CUIButtonEx tv_btn;
				for(tv_i=0 ,nCnt =0; tv_i<MAX_KIT_SIZE ; tv_i++){ 
					tv_btn = _pUIMgr->GetCashShop()->m_abtnTradeItems[tv_i];
					if(!tv_btn.IsEmpty()) nCnt++; 
				}
				nmItem<<(ULONG)_pUIMgr->GetCashShop()->GetMyCash();
				nmItem<<(ULONG)nCnt;
				
				int tv_sum=0;
				for(tv_i=0 ; tv_i<MAX_KIT_SIZE ; tv_i++){
					tv_btn= _pUIMgr->GetCashShop()->m_abtnTradeItems[tv_i];
					if(tv_btn.IsEmpty()	)
						continue;
					nmItem<<(ULONG)tv_btn.GetCashIndex();
					CCashShopData& CD	= _pNetwork->GetCashShopData(tv_btn.GetCashType());
					tv_sum += CD.m_vShopItemArray[tv_btn.GetCashTypeIndex()].m_cash ;
				}
				nmItem<<(ULONG)tv_sum;
				
				SendToServerNew(nmItem);
			}
			break;

		case MSG_EX_CASHITEM_BRING_REQ :
			{
				CUIButtonEx tv_btn;
				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
					tv_btn = _pUIMgr -> GetCashShop()->m_abtnInvenItems[tv_i];
					if(!tv_btn.IsEmpty())  nCnt++;
				}
				nmItem<<(ULONG)nCnt;

				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
					tv_btn= _pUIMgr->GetCashShop()->m_abtnInvenItems[tv_i];
					if(tv_btn.IsEmpty()	)
						continue;
					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
					nmItem<<(ULONG)tv_btn.GetCashIndex();
				}
				SendToServerNew(nmItem);
		
			}			
			break;

		case MSG_EX_CASHITEM_PURCHASELIST_REQ :
			SendToServerNew(nmItem);

			break;

		case MSG_EX_CASHITEM_PURCHASEHISTORY_REQ :
			{
				std::vector<CTString> tv_vecStr;
				int tv_curSel;
				// YEAR
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbYear.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbYear.GetCurSel();
				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
				// MONTH
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbMonth.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbMonth.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
				// DAY
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbDay.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbDay.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);

				SendToServerNew(nmItem);
			}
			break;

		//선물 관련 :Su-won		|-------------------------------------------->
		case MSG_EX_CASHITEM_GIFT_REQ :			// 선물 보내기 요청
			{
				//charName(str) Msg(str) count(n) idx(n) ctid(n)

				nmItem<<_pUIMgr->GetCashShop()->m_ebChar.GetString();
				nmItem<<_pUIMgr->GetCashShop()->m_ebGiftMessage.GetString();

				CUIButtonEx tv_btn;
				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
					tv_btn = _pUIMgr -> GetCashShop()->m_abtnInvenItems[tv_i];
					if(!tv_btn.IsEmpty())  nCnt++;
				}
				nmItem<<(ULONG)nCnt;

				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
					tv_btn= _pUIMgr->GetCashShop()->m_abtnInvenItems[tv_i];
					if(tv_btn.IsEmpty()	)
						continue;
					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
					nmItem<<(ULONG)tv_btn.GetCashIndex();
				}
				SendToServerNew(nmItem);
			}
			break;

		case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ:  // 보낸 선물 내역 리스트 요청 : y(n) m(c) d(c)
			{
				std::vector<CTString> tv_vecStr;
				int tv_curSel;
				// YEAR
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftYear.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftYear.GetCurSel();
				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
				// MONTH
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftMonth.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftMonth.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
				// DAY
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftDay.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftDay.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);

				SendToServerNew(nmItem);
			}
			break;

		case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ:  // 받은 선물 내역 리스트 요청 : y(n) m(c) d(c)
			{
				std::vector<CTString> tv_vecStr;
				int tv_curSel;
				// YEAR
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftYear.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftYear.GetCurSel();
				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
				// MONTH
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftMonth.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftMonth.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
				// DAY
				tv_vecStr = _pUIMgr->GetCashShop()->m_cbGiftDay.GetVecString();
				tv_curSel = _pUIMgr->GetCashShop()->m_cbGiftDay.GetCurSel();
				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);

				SendToServerNew(nmItem);
			}
			break;
		case MSG_EX_CASHITEM_GIFT_RECVLIST_REQ:  // 받은 선물 리스트 요청
			{
				SendToServerNew(nmItem);
			}
			break;
		case MSG_EX_CASHITEM_GIFT_RECV_REQ:		// 받은 선물 인벤으로 이동 요청 : count(n) idx(c) ctid(c)
			{
				CUIButtonEx tv_btn;
				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
					tv_btn = _pUIMgr -> GetCashShop()->m_abtnInvenItems[tv_i];
					if(!tv_btn.IsEmpty())  nCnt++;
				}
				nmItem<<(ULONG)nCnt;

				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
					tv_btn= _pUIMgr->GetCashShop()->m_abtnInvenItems[tv_i];
					if(tv_btn.IsEmpty()	)
						continue;
					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
					nmItem<<(ULONG)tv_btn.GetCashIndex();
				}
				SendToServerNew(nmItem);
			}
			break;
		//선물 관련 :Su-won		<--------------------------------------------|
	}
}

// ----------------------------------------------------------------------------
// Name : SendChangeMyName() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangMyName(CTString strMsg)
{

	CNetworkMessage nmItem(MSG_EXTEND);
		nmItem << (ULONG)MSG_EX_NAMECHANGE;
		nmItem << (ULONG)CHANGE_MY_NAME_ITEM;
		nmItem << strMsg;
	SendToServerNew(nmItem);

}


// ----------------------------------------------------------------------------
// Name : SendChangeGuildName() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangGuildName(CTString strMsg)
{
	CNetworkMessage nmItem(MSG_EXTEND);
		nmItem << (ULONG)MSG_EX_NAMECHANGE;
		nmItem << (ULONG)CHANGE_GUILD_NAME_ITEM;
		nmItem << strMsg;
	SendToServerNew(nmItem);
}


// ----------------------------------------------------------------------------
// Name : SendExSlotMessage() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendExSlotMessage(int slotNum)
{
	int tabId,rowId,colId;
	_pUIMgr->GetInventory()->GetUseItemSlotInfo(tabId,rowId,colId);
	CItems& Items = MySlotItem[tabId][rowId][colId];

	_pUIMgr->CloseMessageBox(MSGCMD_USE_SLOT_ITEM);
	
	if(Items.Item_Index ==-1) //비어있는 슬롯이다.
		return;
	if(slotNum>0) slotNum--;
	CNetworkMessage nmItem(MSG_ITEM); 	
			nmItem << (SBYTE)MSG_ITEM_USE;				
			nmItem << (UBYTE)tabId;
			nmItem << (UBYTE)rowId;
			nmItem << (UBYTE)colId;
			nmItem << (ULONG)Items.Item_UniIndex;
			nmItem << (ULONG)slotNum;

	SendToServerNew(nmItem);

}


// ----------------------------------------------------------------------------
// Name : UseSlotItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UseSlotItem( int tabId, int rowId, int colId, SBYTE sbWearType )
{
	CItems& Items = MySlotItem[tabId][rowId][colId];
	
	if(Items.Item_Index ==-1) //비어있는 슬롯이다.
		return;
	
	//0615 kwon 
	INDEX PlayerType = ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->en_pcCharacter.pc_iPlayerType;//타이탄:0,mage:1,healer:2
	
	
	if(!Items.ItemData.CanUse(PlayerType) && !Items.ItemData.CanUse(WILDPET_JOB)) //0615 kwon 
	{
		ClientSystemMessage( _S( 294, "착용 할수 없는 아이템입니다." ), SYSMSG_ERROR );
		return;
	}
	
	if( Items.ItemData.GetType() == CItemData::ITEM_WEAPON || 
		Items.ItemData.GetType() == CItemData::ITEM_SHIELD ||
		Items.ItemData.GetType() == CItemData::ITEM_ACCESSORY )
	{
		// NOTE : 테섭에서 애완동물 탈것을 장착할수 없도록 하는 부분...
		// 애완동물 일때...
		if( (Items.ItemData.GetSubType() == CItemData::ACCESSORY_PET || Items.ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET)
			&& Items.Item_Wearing == -1 )
		{
			const INDEX iPetIndex = MySlotItem[tabId][rowId][colId].Item_Plus;
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= iPetIndex;
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
				std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
			if( iter != _pNetwork->m_vectorPetList.end() )
			{
				INDEX iPetType;
				INDEX iPetAge;
				_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );				
				const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
				
				// 타야하는 말일경우...
				if( bPetRide )
				{
					//ClientSystemMessage( _S( 294, "착용 할수 없는 아이템입니다." ), SYSMSG_ERROR );
					
					// 공격중이거나 스킬 사용중일때는 바꿀수 없음.
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsAttacking() )
					{
						ClientSystemMessage( _S(2579, "공격중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}
					
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsPolymophing() )
					{
						ClientSystemMessage( _S( 2574, "변신중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;						
					}
					
					if( _pNetwork->MyCharacterInfo.sbEvocationType != -1 )
					{
						ClientSystemMessage( _S(2580, "강신중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}
					
					// 공성중이고 공성지역 내에서는 애완동물을 소환할 수 없음
					if( _pNetwork->MyCharacterInfo.sbAttributePos == ATTC_WAR && _pUISWDoc->IsWar() )
					{
						ClientSystemMessage( _S( 2583, "공성중에는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}					
					
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
					{
						ClientSystemMessage( _S(2581, "스킬 사용중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}

					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSitting() )
					{
						return;
					}
				}
			}
		}
		
		// 무기 일때...
		if( Items.ItemData.GetType() == CItemData::ITEM_WEAPON )
		{
			// 공격중이거나 스킬 사용중일때는 바꿀수 없음.
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsAttacking() )
			{
				ClientSystemMessage( _S( 1324, "공격중일때는 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
				return;
			}
			
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				ClientSystemMessage( _S( 1325, "스킬 사용중일때는 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
				return;
			}
		}
		
		if(Items.Item_Wearing != -1) //현재 이 갑옷을 입고 있네? -1이 아니면 착용중이라는 뜻.
		{					
			CNetworkMessage nmItem(MSG_ITEM); 	
			nmItem << (SBYTE)MSG_ITEM_WEAR;				
			nmItem << Items.Item_Wearing;
			nmItem << (SBYTE)-1;
			nmItem << (SBYTE)-1;
			nmItem << (SBYTE)-1;
			nmItem << (SLONG)-1;
			
			SendToServerNew(nmItem);	
		}
		else //이 갑옷을 입고 있지 않소. 버럭~
		{
			SBYTE	sbWear = -1;
			if( Items.ItemData.GetType() == CItemData::ITEM_ACCESSORY && 
				!(Items.ItemData.GetSubType() == CItemData::ACCESSORY_PET || Items.ItemData.GetSubType() == CItemData::ACCESSORY_WILDPET))
			{
				// 인벤토리의 빈자리를 확인합니다.
				for(int iWearPos = WEAR_ACCESSORY1; iWearPos <= WEAR_ACCESSORY3; ++iWearPos)
				{
					if(!_pNetwork->pMyCurrentWearing[iWearPos])
					{
						sbWear = iWearPos;
						break;
					}
				}
			}
			else
			{
				sbWear = Items.ItemData.GetWearingPosition();
			}

			CNetworkMessage nmItem(MSG_ITEM); 	
			nmItem << (SBYTE)MSG_ITEM_WEAR;
			nmItem << sbWear;
			nmItem << Items.Item_Tab;
			nmItem << Items.Item_Row;
			nmItem << Items.Item_Col;
			nmItem << Items.Item_UniIndex;

			SendToServerNew(nmItem);	
		}
	}
	else if(Items.ItemData.GetType() == CItemData::ITEM_ONCEUSE)
	{
		switch( Items.ItemData.GetSubType() )
		{
	
		case CItemData::ITEM_SUB_WARP:			// 이동
			{
				if( Items.ItemData.GetWarpType() == 0 )			// 리턴 스크롤
				{
					if( _pUIMgr->IsCSFlagOn( CSF_CANNOT_TELEPORT_MASK ) )
					{
						_pUIMgr->GetTeleport()->ShowTeleportError();
						return;
					}
				}
				else if( Items.ItemData.GetWarpType() == 1 )	// 메모리 스크롤
				{
					// [KH_070315] 프리미엄 메모리스크롤 관련 추가
					if(Items.Item_Index == PRIMIUM_TELEPORT)
						_pUIMgr->GetTeleportPrimium()->OpenTeleport();
					else
						_pUIMgr->GetTeleport()->OpenTeleport();
					return;
				}			
			}
			break;
		
		// 이기환 수정 시작 ( 12. 6 ) : 생산 시스템
		case CItemData::ITEM_SUB_PROCESS_DOC:	// 가공 문서
			{
				if ( _pUIMgr->GetProduct()->IsVisible() || _pUIMgr->GetMix()->IsVisible() ||
					_pUIMgr->GetCompound()->IsVisible() )
				{
					return;
				}

				_pUIMgr->GetProcess()->OpenProcess( Items.Item_Index, rowId, colId );
				return;
			}
			break;
		case CItemData::ITEM_SUB_MAKE_POTION_DOC:	// 가공 문서
		case CItemData::ITEM_SUB_MAKE_TYPE_DOC:	// 제작 문서
			{
				if ( _pUIMgr->GetProcess()->IsVisible() || _pUIMgr->GetMix()->IsVisible() ||
					_pUIMgr->GetCompound()->IsVisible() )
				{
					return;
				}

				_pUIMgr->GetProduct()->OpenProduct( Items.Item_Index, rowId, colId );
				return;
			}
			break;
		// 이기환 수정 끝 ( 12. 10 )
		// Date : 2005-01-12,   By Lee Ki-hwan
		case CItemData::ITEM_SUB_BOX : // 상자 ( 조합 )
			{
				switch ( Items.ItemData.GetBoxType() )
				{
				case CItemData::BOX_REMAKE: // 아이템 조합 ( 재활의 상자 )
					if ( _pUIMgr->GetProcess()->IsVisible() ||
						_pUIMgr->GetProduct()->IsVisible() ||
						_pUIMgr->GetCompound()->IsVisible() )
					{
						return;
					}
					
					_pUIMgr->GetMix()->OpenMix ( Items.Item_Index, rowId, colId );
					return;
					
				case CItemData::BOX_ARCANE:
					if ( _pUIMgr->GetProcess()->IsVisible() || 
						_pUIMgr->GetProduct()->IsVisible() ||
						_pUIMgr->GetMix()->IsVisible() )
					{
						return;
					}

					_pUIMgr->GetCompound()->OpenCompound ( Items.Item_Index, rowId, colId );
					return;
				
				
				}
			}
			break;
		// Date : 2005-01-12,   By Lee Ki-hwan
		case  CItemData::ITEM_SUB_CHANGE_DOC:	
			{
			
				if(!ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->CheckChangeCondition(Items.ItemData.GetNum2(), Items.ItemData.GetNum3()))
				{
					return;
				}
				
			}
			break;
			// 퀘스트 스크롤.
		case CItemData::ITEM_SUB_QUEST_SCROLL:
			{
				const int iQuestIndex = Items.ItemData.GetNum0();
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{						
						_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_QUEST );
						_pUIMgr->GetQuestBookNew()->OpenQuestBook( iQuestIndex, &Items );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1744, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1745, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
				return;
			}
			break;

		case CItemData::ITEM_SUB_TARGET:
			{
				if( !_pNetwork->_TargetInfo.bIsActive )
						return;
				
				LONG iMobClientIndex = _pNetwork->_TargetInfo.pen_pEntity->en_lNetworkID;
				SBYTE cTargetType = _pNetwork->_TargetInfo.TargetType;

				CItemData	&rItemData = _pNetwork->GetItemData( Items.Item_Index );

				if( !_pUIMgr->GetQuickSlot()->StartSkillDelay( Items.ItemData.GetNum0() ) )
				{
					if( !_pUIMgr->GetInventory()->StartSkillDelay( Items.ItemData.GetNum0() ) )
						return;
				}
				
				SendtargetItemUse(Items.Item_Tab, Items.Item_Row, Items.Item_Col, Items.Item_UniIndex, cTargetType, iMobClientIndex);
				return;
			}
			break;

		default:
			break;
		}

		SendItemUse(Items.Item_Tab, Items.Item_Row, Items.Item_Col, Items.Item_UniIndex, 0);		
	}
	else if ( Items.ItemData.GetType() == CItemData::ITEM_POTION )
	{
		CItemData	&rItemData = _pNetwork->GetItemData( Items.Item_Index );

		if( !_pUIMgr->GetQuickSlot()->StartSkillDelay( Items.ItemData.GetNum0() ) )
		{
			if( !_pUIMgr->GetInventory()->StartSkillDelay( Items.ItemData.GetNum0() ) )
			return;
		}

		SendItemUse(Items.Item_Tab, Items.Item_Row, Items.Item_Col, Items.Item_UniIndex, 0);
	}
	// 생산물의 경우.
	else if( Items.ItemData.GetType() == CItemData::ITEM_ETC && 
		Items.ItemData.GetSubType() == CItemData::ITEM_ETC_PRODUCT )
	{
		if( _pNetwork->_PetTargetInfo.bIsActive )
		{
			SendItemUse(Items.Item_Tab, Items.Item_Row, Items.Item_Col, Items.Item_UniIndex, 0);
		}
	}
}

/****   WareHouse *********************************************************/
//-----------------------------------------------------------------------------
// Purpose: SendWareHouseChangePassword
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseChangePassword(const CTString& strOld, const CTString& strNew)
{	
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_CHANGE_PASSWORD_REQ;
	nmWareHouse << strOld;
	nmWareHouse << strNew;
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseIsSetPassword
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseIsSetPassword()
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_ISSETPASSWORD;	
	SendToServerNew(nmWareHouse);
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseSeal
// Input  : 창고 잠금 요청.
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseSeal()
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_SEAL;	
	SendToServerNew(nmWareHouse);
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseListReq
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseListReq( const CTString& strPW )
{	
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_LIST_REQ;
	nmWareHouse << strPW;	
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseCheckPassWord
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseCheckPassWord( const CTString& strPW )
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_CHECK_PASSWORD_REQ;
	nmWareHouse << strPW;
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseSetupPassWord
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseSetupPassWord( const CTString& strPW )
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_SETTING_NEWPASSWORD_REQ;
	nmWareHouse << strPW;
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseSetupPassWord
// Input  : 051125 wooss
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseSetupPassWord( const CTString& strPW ,const CTString& strUNLOCK)
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_SETTING_NEWPASSWORD_REQ;
	nmWareHouse << strPW;
	nmWareHouse << strUNLOCK;
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseDeletePassWord
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseDeletePassWord( const CTString& strID )
{
	CNetworkMessage nmWareHouse(MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_DELETE_PASSWORD_REQ;
	nmWareHouse << strID;
	SendToServerNew(nmWareHouse);	
}

/****   Guild   ***********************************************************/
// ----------------------------------------------------------------------------
// Name : GuildJoin()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildJoin( SLONG slGuildIndex, SLONG slSrcIndex, SLONG slDestIndex )			// 가입 신청
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_REQ;
	nmGuild << slGuildIndex;
	nmGuild << slSrcIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildQuit()
// Desc : 길드 탈퇴
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildQuit( )
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_OUT_REQ;
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildCreate()
// Desc : 길드 생성
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildCreate( const CTString &strGuildName )		// 길드 생성
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_CREATE;
	//nmGuild << _pNetwork->MyCharacterInfo.index;
	nmGuild << strGuildName;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildUpgrade()
// Desc : 길드 승급
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildUpgrade()							// 길드 승급
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_LEVELUP;	
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildDestroy()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildDestroy()							// 길드 해체
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BREAKUP;	
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildChangeBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildChangeBoss( SLONG slDestIndex )		// 단장 이임
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_CHANGE_BOSS;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << _pNetwork->MyCharacterInfo.index;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildAddViceBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildAddViceBoss( SLONG slDestIndex )		// 부단장 임명
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_APPOINT_OFFICER;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildDelViceBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildDelViceBoss( SLONG slDestIndex )		// 부단장 해임
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_FIRE_OFFICER;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildMemberFire()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildMemberFire( SLONG slDestIndex )		// 멤버 퇴출
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_KICK;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildApplicantAccept()
// Desc : 요청자에 대해서 가입 승인하기.
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildApplicantAccept( SLONG slDestIndex )	// 가입 승인
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_ALLOW;
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildApplicantReject()
// Desc : 요청자에 대해서 가입 거부하기.
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildApplicantReject( SBYTE sbWhoCancel )	// 가입 거부
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_CANCEL;	
	nmGuild << sbWhoCancel;
	SendToServerNew(nmGuild);
}


// Guild Battle

//------------------------------------------------------------------------------
// CNetworkLibrary::GBReq
// Explain: 길드 전투 요청
// Date : 2005-03-18(오후 3:16:42) Lee Ki-hwan
// Edit History 
// Date : 2005-04-15(오후 7:49:53), By Lee Ki-hwan : SLONG nTime 추가
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReq( SLONG nCharIndex, SLONG nPrize, SLONG nTime )
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_REQ;	
	nmGuild << nCharIndex;
	nmGuild << nPrize;
	nmGuild << nTime;

	SendToServerNew(nmGuild);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBReqReject
// Explain: 길드 전투 요청 거절
// Date : 2005-03-18(오후 3:16:45) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReqReject()
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_REJECT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBReqAccept
// Explain: 길드 전투 수락
// Date : 2005-03-18(오후 3:16:47) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReqAccept()
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_ACCEPT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqSend
// Explain: 길드 전투 중지 요청 
// Date : 2005-03-18(오후 3:16:50) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReq( SLONG nCharIndex )
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_REQ;
	nmGuild << nCharIndex;
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqReject
// Explain:  길드 전투 중지 요청 거절 
// Date : 2005-03-18(오후 3:16:52) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReqReject()	
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_REJECT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqAccept
// Explain:  길드 전투 중지 요청 수락 
// Date : 2005-03-18(오후 3:30:03) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReqAccept()	
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_ACCEPT;	
	SendToServerNew(nmGuild);
}

/****   Teach      ***********************************************************/
// ----------------------------------------------------------------------------
// Name : TeachTeacherRegister()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherRegister()			// 선생 목록에 추가
{
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_UP;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherCancelRegister()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherCancelRegister()		// 선생 목록에서 빼줘
{
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_DN;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachRefreshTeacherList()
// Desc : 후견인 목록 갱신
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachRefreshTeacherList()
{
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_SHOW;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherRequest()
// Desc : 후견인 신청
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherRequest( SLONG slCharIndex, CTString& strName )
{
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_REQ;	
	nmTeach << (LONG)slCharIndex;
	nmTeach << strName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherReject()
// Desc : 후견인 신청 취소.
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherReject( BOOL bStudent, SLONG slCharIndex, CTString& strName )
{
	// 선생 요청 거절	: char_type(uc) reject_charindex(n) reject_charname(str)
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_REJECT;

	/*
	if( !bStudent )
		nmTeach << (UBYTE)MSG_TEACH_TEACHER_TYPE;
	else
		nmTeach << (UBYTE)MSG_TEACH_STUDENT_TYPE;
		*/
	nmTeach << slCharIndex;
	nmTeach << strName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherAccept()
// Desc : 후견인 신청 확인.
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherAccept( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName )
{
	// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_ACCEPT;
	nmTeach << slTeacherIndex;
	nmTeach << strTeacherName;
	nmTeach << slStudentIndex;
	nmTeach << strStudentName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherGiveUp()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherGiveUp( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName )
{
	// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
	CNetworkMessage nmTeach(MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_GIVEUP;
	nmTeach << slTeacherIndex;
	nmTeach << strTeacherName;
	nmTeach << slStudentIndex;
	nmTeach << strStudentName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : ChangeJobReq()
// Desc : 전직 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeJobReq( SBYTE sbJob )
{
	SBYTE sbChangeJob = sbJob;
	CNetworkMessage nmChangeJob(MSG_CHANGEJOB);
	nmChangeJob << (UBYTE)MSG_CHANGEJOB_REQ;	
	nmChangeJob << sbChangeJob;
	SendToServerNew(nmChangeJob);
}

// ----------------------------------------------------------------------------
// Name : ChangeJobGiveUp()
// Desc : 직업 포기
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeJobGiveUp()
{
	CNetworkMessage nmChangeJob(MSG_CHANGEJOB);
	nmChangeJob << (UBYTE)MSG_CHANGEJOB_RESET_REQ;		
	SendToServerNew(nmChangeJob);
}

// ----------------------------------------------------------------------------
// Name : ChangeWeaponReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeWeaponReq( SBYTE sbRow, SBYTE sbCol, LONG lItemIndex, LONG lChangeType )
{
	// 무기 교환 요청		: row(c) col(c) itemindex(n) changetype(n)
	CNetworkMessage nmChangeWeapon(MSG_ITEM);
	nmChangeWeapon << (UBYTE)MSG_ITEM_CHANGEWEAPON_REQ;		
	nmChangeWeapon << sbRow;
	nmChangeWeapon << sbCol;
	nmChangeWeapon << lItemIndex;
	nmChangeWeapon << lChangeType;
	SendToServerNew(nmChangeWeapon);
}

// ----------------------------------------------------------------------------
// Name : ChangeWeaponEventReq()
// Desc : 2판 4판 무기 교체 이벤트
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeWeaponEventReq( LONG lItemIndex, LONG lChangeType )
{
	if(_pUIMgr->GetChangeWeapon()->GetCashItem()) {
		
		int tv_tab,tv_row,tv_col;
		_pUIMgr->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_row,tv_col);
		
		CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_row][tv_col];

		if(Items.Item_Index ==-1) //비어있는 슬롯이다.
			return;
	
		CNetworkMessage nmItem(MSG_ITEM); 	
		nmItem << (SBYTE)MSG_ITEM_USE;								
		nmItem << Items.Item_Tab;
		nmItem << Items.Item_Row;
		nmItem << Items.Item_Col;
		nmItem << Items.Item_UniIndex;
		nmItem << lItemIndex;
		nmItem << lChangeType;
		SendToServerNew(nmItem);
	
				
	} else {
		// MSG_EVENT_CHANGE_WITHOUT_OPTION,			// 2판4판 리뉴얼 무기 교체 이벤트 : old_itemindex(n) new_subtype(n)
		CNetworkMessage nmChangeWeapon(MSG_EVENT);
		nmChangeWeapon << (UBYTE)MSG_EVENT_CHANGE_WITHOUT_OPTION;			
		nmChangeWeapon << lItemIndex;
		nmChangeWeapon << lChangeType;
		SendToServerNew(nmChangeWeapon);
	}
}

// ----------------------------------------------------------------------------
// Name : StatReset()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::StatReset( int iStr, int iDex, int iInt, int iCon )
{
	LONG lStr = iStr;
	LONG lDex = iDex;
	LONG lInt = iInt;
	LONG lCon = iCon;

	// 포인트 초기화
	CNetworkMessage nmStat(MSG_STATPOINT);
	nmStat << (UBYTE)MSG_STATPOINT_RESET;
	nmStat << lStr;
	nmStat << lDex;
	nmStat << lInt;
	nmStat << lCon;
	SendToServerNew( nmStat );
}

// ----------------------------------------------------------------------------
// Name : SendChangeWeaponEvent()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangeWeaponEvent()
{
	if( !_pNetwork->pMyCurrentWearing[WEAR_WEAPON] )
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1326, "교체할 무기를 장착하여 주십시오." ) );		
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
		return;
	}
	
	const int iWeaponType = _pNetwork->pMyCurrentWearing[WEAR_WEAPON]->ItemData.GetSubType();
	
	// 생산도구를 장착하구 있을때...
	if(iWeaponType == CItemData::ITEM_WEAPON_MINING || 
		iWeaponType == CItemData::ITEM_WEAPON_GATHERING || 
		iWeaponType == CItemData::ITEM_WEAPON_CHARGE)
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1327, "생산 도구는 교체할 수 없습니다." ) );		
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
		return;
	}
		
	CNetworkMessage nmChangeWeapon(MSG_EVENT);
	nmChangeWeapon << (UBYTE)MSG_EVENT_CHANGEWEAPON;
	SendToServerNew( nmChangeWeapon );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneStartReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneStartReq()
{
	CNetworkMessage	nmMoonStone( MSG_EVENT );

#ifdef NEW_MOONSTONE
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_START_REQ;
#else
	LONG lIndex	= MyCharacterInfo.index;
	
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE_START_REQ;	
	nmMoonStone << lIndex;
#endif
	SendToServerNew( nmMoonStone );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneTryReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneTryReq()
{
	CNetworkMessage	nmMoonStone( MSG_EVENT );

#ifdef NEW_MOONSTONE
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_TRY_REQ;
	nmMoonStone << (ULONG)_pUIMgr->GetGamble()->GetUsedMoonStoneIndex();
#else
	LONG lIndex	= MyCharacterInfo.index;
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE_TRY_REQ;	
	nmMoonStone << lIndex;
#endif
	SendToServerNew( nmMoonStone );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneResultReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneResultReq()
{
	CNetworkMessage	nmMoonStone( MSG_EVENT );

#ifdef NEW_MOONSTONE
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_RESULT_REQ;
#else
	LONG lIndex	= MyCharacterInfo.index;
	
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_MOONSTONE_RESULT_REQ;
	nmMoonStone << lIndex;
#endif
	SendToServerNew( nmMoonStone );
}

// [071122: Su-won] NEW_MOONSTONE
void CNetworkLibrary::SendMoonStoneMix(INDEX iMoonStone, INDEX iMoonStoneBox)
{
	CNetworkMessage	nmMoonStone( MSG_EVENT );

	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_MIX_REQ;
	nmMoonStone << iMoonStone;
	nmMoonStone << iMoonStoneBox;

	SendToServerNew( nmMoonStone );
}

//////////////////////////////////////////////////////////////////////////
// Name : SendMoonStoneTryReq()	
// Desc :
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////
void CNetworkLibrary::SendCashMoonStoneReq(CUIButtonEx slotBtn)
{
	CNetworkMessage nmItem(MSG_EXTEND);
	nmItem	<< (ULONG)MSG_EX_CASHITEM;
	nmItem	<< (UBYTE)MSG_EX_CASHITEM_MOONSTONE_START;
	nmItem	<< (UBYTE)slotBtn.GetItemTab();
	nmItem	<< (UBYTE)slotBtn.GetItemRow(); 
	nmItem	<< (UBYTE)slotBtn.GetItemCol();
	nmItem	<< (ULONG)slotBtn.GetItemUniIndex();
	nmItem  << (LONG)_pUIMgr->GetGamble()->GetSelCashItemIdx();
	
	SendToServerNew(nmItem);
}
//////////////////////////////////////////////////////////////////////////
// Name : SendMoonStoneTryReq()	
// Desc :
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////
void CNetworkLibrary::SendCashMoonStoneReward()
{
	CNetworkMessage nmItem(MSG_EXTEND);
	nmItem	<< (ULONG)MSG_EX_CASHITEM;
	nmItem	<< (UBYTE)MSG_EX_CASHITEM_MOONSTONE_STOP;
	SendToServerNew(nmItem);
}

// ypark |<--
/****   Exchange   ***********************************************************/
void CNetworkLibrary::ExchangeReq_Req( SLONG slDestIndex, CTString &strDestName )
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REQ;
	nm << MyCharacterInfo.index;
	nm << MyCharacterInfo.name;
	nm << slDestIndex;
	nm << strDestName;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Rep()
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REP;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Rej()
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REJECT;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Ready()
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_READY;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Ok()
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_OK;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeItem_Add( int nUniIndex, SQUAD llCount )
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_ITEM;
	nm << (SBYTE)MSG_EXCHANGE_ITEM_ADD;
	nm << (SLONG)nUniIndex;
	nm << llCount;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeItem_Del( int nUniIndex, SQUAD llCount )
{
	// Send network message
	CNetworkMessage	nm( MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_ITEM;
	nm << (SBYTE)MSG_EXCHANGE_ITEM_DEL;
	nm << (SLONG)nUniIndex;
	nm << llCount;

	SendToServerNew( nm );
}

void CNetworkLibrary::AddQuickSlot( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	// Send network message
	CNetworkMessage	nm( MSG_QUICKSLOT );
	nm << (SBYTE)MSG_QUICKSLOT_ADD;
	nm << (SBYTE)nPage;
	nm << (SBYTE)nSlotNum;
	nm << (SBYTE)nSlotType;

	if( nSlotType == 2 )
	{
		nm << (SBYTE)nData0;
		nm << (SBYTE)nData1;
	}
	else if( nSlotType >= 0 )
	{
		nm << (SLONG)nData0;
	}

	SendToServerNew( nm );
}

void CNetworkLibrary::SwapQuickSlot( int nPage, int nSlotNum1, int nSlotNum2 )
{
	// Send network message
	CNetworkMessage	nm( MSG_QUICKSLOT );
	nm << (SBYTE)MSG_QUICKSLOT_SWAP;
	nm << (SBYTE)nPage;
	nm << (SBYTE)nSlotNum1;
	nm << (SBYTE)nSlotNum2;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyInvite( SBYTE sbType, SLONG slIndex )
{
	// Send network message
	CNetworkMessage	nm( MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_INVITE;
	nm << sbType;
	nm << slIndex;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyAllow()
{
	CNetworkMessage	nm( MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_ALLOW;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyReject()
{
	CNetworkMessage	nm( MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_REJECT;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyQuit()
{
	CNetworkMessage	nm( MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_QUIT;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyKick( SLONG slIndex )
{
	CNetworkMessage	nm( MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_KICK;
	nm << slIndex;

	SendToServerNew( nm );
}

void CNetworkLibrary::ItemPlusEffectReq( SBYTE sbOption )
{
	CNetworkMessage	nm( MSG_UI );
	nm << (SBYTE)MSG_UI_PLUS_EFFECT_REQ;
	nm << sbOption;

	SendToServerNew( nm );
}
// ypark -->|

// ----------------------------------------------------------------------------
// Name : FindTargetsInRange()
// Desc : 
// ----------------------------------------------------------------------------
// FIXME : 퍼포먼스 측정이 필요한 부분.
// FIXME : 소환수 및 싱글던젼의 경우 속도에서 문제가 발생할수 있는 함수임.
void CNetworkLibrary::FindTargetsInRange(
								 CEntity* pPlayer, 								// 타겟.
								 CEntity* pCenter,
								 CSelectedEntities &cen, 								 
								 FLOAT fFallOffRange, 
								 INDEX iMaxEnemies,
								 FLOAT fAngle, 								 
								 INDEX iSkillIndex,
								 INDEX iTargetType )
{
	INDEX iCount		= 0;
	FLOAT3D vCenter		= pCenter->GetPlacement().pl_PositionVector;
	const FLOATaabbox3D &boxRange = FLOATaabbox3D(vCenter, fFallOffRange);

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

	// for each entity in the world of this entity
	FOREACHINDYNAMICCONTAINER(pPlayer->en_pwoWorld->wo_cenEntities, CEntity, iten) 
	{
		// if it is zoning brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH && (iten->en_ulFlags&ENF_ZONING)) 
		{
			// get first mip in its brush
			CBrushMip *pbm = iten->en_pbrBrush->GetFirstMip();
			// if the mip doesn't touch the box
			if (!pbm->bm_boxBoundingBox.HasContactWith(boxRange)) 
			{
				// skip it
				continue;
			}

			// for all sectors in this mip
			FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc) 
			{
				// if the sector doesn't touch the box
				if (!itbsc->bsc_boxBoundingBox.HasContactWith(boxRange)) 
				{
					// skip it
					continue;
				}

				// for all entities in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
					
					// SKA 모델만 컨테이너에 추가합니다.
					if ((pen->en_RenderType==CEntity::RT_SKAMODEL)
						&& (pen->GetFlags() & ENF_ALIVE)
						&& boxRange.HasContactWith(
						FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius)))
					{
						// if it has collision box
						if (pen->en_pciCollisionInfo!=NULL) 
						{
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms) 
							{
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter * pen->en_mRotation + pen->en_plPlacement.pl_PositionVector;

								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR))) 
								{
									// add it to container
									if (!cen.IsMember(pen)) 
									{
										FLOAT3D vHitPos;

										if ( (pen != pCenter) )
										{
											// 최대 갯수를 넘지 않도록 하고...
											if(iCount >= iMaxEnemies)
											{
												return;
											}

											// 내가 나한테 쓴 경우...
											if( pPlayer == pCenter )
											{
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;
												vDelta(2)	= 0.0f;
												FLOAT fLength = vDelta.Length();
												if( fLength <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget(iSkillIndex, pen, fLength) )
													{
														cen.Add(pen);
														iCount++;
													}
												}
											}
											// 범위 제한이 없을때...( 이 경우에는 플레이어 중심이어야만 함!!! )
											else if( iTargetType != CSkill::STT_TARGET_RECT && 
													fAngle != 360.0f )
											{
												// 플레이어와 각 엔티티 사이의 각도를 계산하여 처리함.
												// 플레이어의 위치와 엔티티의 위치의 벡터를 구하고,
												// 그 값을 Heading으로 변환함.
												FLOAT3D vNormal;
												FLOAT	fLength;
												//AnglesToDirectionVector(pPlayer->GetPlacement().pl_OrientationAngle, vNormal);
												vNormal = vCenter - pPlayer->GetPlacement().pl_PositionVector;
												vNormal(2) = 0.0f;
												vNormal.Normalize();
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;
												vDelta(2) = 0.0f;
												fLength	= vDelta.Length();
												vDelta.Normalize();

												if( fLength <= fFallOffRange )
												{
													// FIXME : 느린 루틴임.
													// FIXME : 테이블을 이용할것.
													//ANGLE aDelta = GetRelativeHeading(vDelta);
													float fDelta = acos((vNormal%vDelta)/(vNormal.Length() * vDelta.Length()));
													ANGLE aDelta = AngleRad(fDelta);

													if(fmod(fabs(aDelta), 360.0f) < fAngle)
													{
														if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, fLength ) )
														{
															cen.Add(pen);
															iCount++;
														}													
													}
												}
											}
											// 타겟 중심.
											else if( iTargetType == CSkill::STT_TARGET_RANGE )
											{
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pCenter->GetPlacement().pl_PositionVector;
												vDelta(2)	= 0.0f;
												if( vDelta.Length() <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, vDelta.Length() ) )
													{
														cen.Add(pen);
														iCount++;
													}												
												}
											}
											// 일직선 사각형 영역일 경우...
											else if( iTargetType == CSkill::STT_TARGET_RECT )											
											{
												FLOAT3D vNormal;												
												vNormal = vCenter - pPlayer->GetPlacement().pl_PositionVector;
												vNormal(2) = 0.0f;
												vNormal.Normalize();

												FLOAT fLength = 0.0f;
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;												
												vDelta(2) = 0.0f;
												fLength = vDelta.Length();
												vDelta.Normalize();												

												float fDelta	= acos((vNormal%vDelta)/(vNormal.Length() * vDelta.Length()));
												float fDist		= fLength * sin(fDelta);
												float fDist2	= fLength * cos(fDelta);
												float fDot		= vDelta % vNormal;

												if( fDot <= 0 )
												{
													goto next_entity;
												}
												
												if( fabs(fDist) <= 0.5f && fabs(fDist2) <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, fLength ) )
													{
														cen.Add(pen);
														iCount++;
													}												
												}
											}
										}										
									}
									goto next_entity;
								} 
							}						
						}						
					}
					next_entity:;
				ENDFOR}
			}
		}
	}
}

void CNetworkLibrary::FindTargetsInRangeEx(CEntity* pCenter, CSelectedEntities& cen, FLOAT fFallOffRange, INDEX iMaxEnemies)
{
	INDEX iCount = 0;
	FLOAT3D vCenter = pCenter->GetPlacement().pl_PositionVector;
	const FLOATaabbox3D &boxRange = FLOATaabbox3D(vCenter, fFallOffRange);

	// for each entity in the world of this entity
	FOREACHINDYNAMICCONTAINER(pCenter->en_pwoWorld->wo_cenAllEntities, CEntity, iten)
	{
		// if it is zoning brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH && (iten->en_ulFlags & ENF_ZONING))
		{
			// get first mip in its brush
			CBrushMip *pbm = iten->en_pbrBrush->GetFirstMip();
			// if the mip doesn't touch the box
			if (!pbm->bm_boxBoundingBox.HasContactWith(boxRange))
			{	// skip it
				continue;
			}

			// for all sectors in this mip
			FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc)
			{
				// if the sector doesn't touch the box
				if (!itbsc->bsc_boxBoundingBox.HasContactWith(boxRange))
				{// skip it
					continue;
				}

				// for all entities in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
					// SKA 모델만 컨테이너에 추가합니다.
					if ((pen->en_RenderType == CEntity::RT_SKAMODEL) && (pen->GetFlags() & ENF_ALIVE) &&
						boxRange.HasContactWith(FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius)))
					{
						// if it has collision box
						if (pen->en_pciCollisionInfo != NULL)
						{
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms)
							{
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter * pen->en_mRotation + pen->en_plPlacement.pl_PositionVector;
								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR)))
								{
									// add it to container
									if (!cen.IsMember(pen))
									{
										FLOAT3D vHitPos;

										if (pen != pCenter)
										{
											FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pCenter->GetPlacement().pl_PositionVector;
											vDelta(2) = 0.0f;

											if (vDelta.Length() <= fFallOffRange)
											{
												//if (((CUnit*)_pNetwork->_WildPetInfo.pet_pEntity)->CheckTarget(pen))	// 현재 등록된 펫의 AI 조건
												if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckEntityOfTarget(_pNetwork->_WildPetInfo.pet_pEntity, pen))
												{
													cen.Add(pen);
													iCount++;

													if (iCount >= iMaxEnemies)
														return;
												}
											}
										}
									}
									goto next_ent;
								}
							}
						}
					}
					next_ent:;
				ENDFOR}
			}
		}
	}
}

// 멀티 공격가능한지를 판단합니다.
// ----------------------------------------------------------------------------
// Name : CheckSkillAttack()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CNetworkLibrary::CheckSkillAttack( INDEX iSkillIndex, CEntity* pEntity )
{
	// 이쪽으로 iSkillIndex 가 -1이 들어오면 안됨.
	if( iSkillIndex == -1 )
		return FALSE;

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	BOOL bForHelp		= FALSE;	
	CSkill &rSkillData	= GetSkillData( iSkillIndex );
	bForHelp		= rSkillData.GetFlag() & SF_FORHELP;
	const int iJob2		= rSkillData.GetJob2();

	BOOL bIsCharacter	= pEntity->IsCharacter();
	BOOL bIsMob			= pEntity->IsEnemy();
	BOOL bIsPet			= pEntity->IsPet();
	BOOL bIsSummon		= pEntity->IsSlave();
	BOOL bIsPlayer		= pEntity->IsPlayer();

	// 애완동물을 타고 있을때 쓸수 있는 스킬일 경우.
	if( _pNetwork->MyCharacterInfo.bPetRide && !( ( iJob2 == 2 || iJob2 == 3 ) ) )
	{
		return FALSE;
	}
	
	// HELP 스킬은 내 애완동물
	if( bForHelp )
	{
		if( bIsPlayer )
			return TRUE;
		else if( pEntity->GetFirstExFlags() & ( ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_PET ) )
			return TRUE;
		else if( bIsPet || bIsSummon || bIsCharacter )
		{	
			return !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckNormalAttack( pEntity, 0.0f );
		}		
	}
	else
	{
		// 자기가 자기한테 공격할수는 없음.
		if( bIsPlayer )
		{
			return FALSE;
		}
		// 그 이외의 경우.
		else
		{
			return ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckNormalAttack( pEntity, 0.0f );
		}
	}
	return FALSE;
}

// NOTE : 공격자 목록이 20개를 넘거나, 시간이 초과한경우에는 메시지를 보낼것...
// ----------------------------------------------------------------------------
// Name : AddAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddAttackList( UBYTE ubAttackType, INDEX iAttackIndex, UBYTE ubTargetType, INDEX iTargetIndex )
{
	sAttackInfo TempAttack;
	TempAttack.ubAttackType	= ubAttackType;
	TempAttack.iAttackIndex	= iAttackIndex;	
	TempAttack.iTargetIndex	= iTargetIndex;
	TempAttack.ubTargetType	= ubTargetType;

	if( ubTargetType == MSG_CHAR_NPC )
	{
		m_vectorAttackNPCList.push_back(TempAttack);
	}
	else if( ubTargetType == MSG_CHAR_ELEMENTAL )
	{
		m_vectorAttackSummonList.push_back(TempAttack);
	}
	else if( ubTargetType == MSG_CHAR_PET )
	{
		m_vectorAttackPetList.push_back(TempAttack);
	}
	else
	{
		m_vectorAttackPCList.push_back(TempAttack);
	}
}

// ----------------------------------------------------------------------------
// Name : SendAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackList()
{
	if( !m_vectorAttackNPCList.empty() )
	{
		INDEX iCount	= m_vectorAttackNPCList.size();
		CNetworkMessage nmAttack(MSG_PD_ATTACK);

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackNPCList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackNPCList.end();
		
		// NPC가 NPC를 공격하려고 할때...
		nmAttack << (UBYTE)MSG_CHAR_NPC;
		nmAttack << (*it).iTargetIndex;
		nmAttack << (UBYTE)(*it).ubAttackType;
		nmAttack << (UBYTE)iCount;		
		
		for( ; it != end; ++it )
		{
			INDEX iIndex = (*it).iAttackIndex;
			nmAttack << iIndex;
		}
		
		SendToServerNew(nmAttack);

		if( !m_vectorAttackNPCList.empty() )
			m_vectorAttackNPCList.clear();
	}

	if( !m_vectorAttackSummonList.empty() )
	{
		INDEX iCount	= m_vectorAttackSummonList.size();
		CNetworkMessage nmAttack(MSG_PD_ATTACK);

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackSummonList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackSummonList.end();
		
		// NPC가 NPC를 공격하려고 할때...
		nmAttack << (UBYTE)MSG_CHAR_ELEMENTAL;
		nmAttack << (*it).iTargetIndex;
		nmAttack << (UBYTE)(*it).ubAttackType;
		nmAttack << (UBYTE)iCount;		
		
		for( ; it != end; ++it )
		{
			INDEX iIndex = (*it).iAttackIndex;
			nmAttack << iIndex;
		}
		
		SendToServerNew(nmAttack);

		if( !m_vectorAttackSummonList.empty() )
			m_vectorAttackSummonList.clear();
	}

	if( !m_vectorAttackPetList.empty() )
	{
		INDEX iCount	= m_vectorAttackPetList.size();
		CNetworkMessage nmAttack(MSG_PD_ATTACK);

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackPetList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackPetList.end();
		
		// NPC가 NPC를 공격하려고 할때...
		nmAttack << (UBYTE)MSG_CHAR_PET;
		nmAttack << (*it).iTargetIndex;
		nmAttack << (UBYTE)(*it).ubAttackType;
		nmAttack << (UBYTE)iCount;		
		
		for( ; it != end; ++it )
		{
			INDEX iIndex = (*it).iAttackIndex;
			nmAttack << iIndex;
		}
		
		SendToServerNew(nmAttack);

		if( !m_vectorAttackPetList.empty() )
			m_vectorAttackPetList.clear();
	}
	
	if( !m_vectorAttackPCList.empty() )
	{
		INDEX iCount	= m_vectorAttackPCList.size();
		CNetworkMessage nmAttack(MSG_PD_ATTACK);

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackPCList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackPCList.end();
		
		// NPC가 PC를 공격하려고 할때...
		nmAttack << (UBYTE)MSG_CHAR_PC;
		nmAttack << (*it).iTargetIndex;		
		nmAttack << (UBYTE)(*it).ubAttackType;
		nmAttack << (UBYTE)iCount;		
		
		for( ; it != end; ++it )
		{
			INDEX iIndex = (*it).iAttackIndex;
			nmAttack << iIndex;
		}
		
		SendToServerNew(nmAttack);
		CPrintF("Send Attack List : %d\n", iCount );

		if( !m_vectorAttackPCList.empty() )
			m_vectorAttackPCList.clear();
	}
	//ClearAttackList();
}

// ----------------------------------------------------------------------------
// Name : ClearAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearAttackList()
{
	if( !m_vectorAttackPCList.empty() )
		m_vectorAttackPCList.clear();

	if( !m_vectorAttackSummonList.empty() )
		m_vectorAttackSummonList.clear();

	if( !m_vectorAttackPetList.empty() )
		m_vectorAttackPetList.clear();

	if( !m_vectorAttackNPCList.empty() )
		m_vectorAttackNPCList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddMoveList( INDEX iIndex, FLOAT fX, FLOAT fZ, FLOAT fH, FLOAT fAngle )
{	
	sMoveInfo TempMove;
	TempMove.iIndex		= iIndex;
	TempMove.fX			= fX;
	TempMove.fZ			= fZ;
	TempMove.fH			= fH;
	TempMove.fAngle		= fAngle;
	m_vectorMoveList.push_back(TempMove);

	// 20개를 넘어가면... 서버로 메세지를 보냄.
	if( m_vectorMoveList.size() >= MAX_MOVE_LIST )
	{
		SendMoveList();	
	}
}

// EDIT : BS : BEGIN
void CNetworkLibrary::AddMoveList(CEntity &en)
{
	if (en.IsEnemy())
	{
		DWORD curTickCount = GetTickCount();
		// EDIT : BS : 시간 딜레이
		if (en.m_tickSendPDMove == 0 || curTickCount - en.m_tickSendPDMove >= 500)
		{
			en.m_tickSendPDMove = curTickCount;

			CPlacement3D pl		= en.GetLerpedPlacement();

			// 같은 넘이 있으면 좌표만 수정
			bool bAdd = true;
			if (!m_vectorMoveList.empty())
			{
				INDEX iCount		= m_vectorMoveList.size();	
				std::vector<sMoveInfo>::iterator it		= m_vectorMoveList.begin();
				std::vector<sMoveInfo>::iterator end	= m_vectorMoveList.end();
				for( ; it != end; ++it )
				{
					if ((*it).iIndex == en.GetNetworkID())
					{
						(*it).fX			= pl.pl_PositionVector(1);
						(*it).fZ			= pl.pl_PositionVector(3);
						(*it).fH			= pl.pl_PositionVector(2);
						(*it).fAngle		= pl.pl_OrientationAngle(1);
						bAdd = false;
						break;
					}
				}
			}

			if (bAdd)
			{
				sMoveInfo TempMove;
				TempMove.iIndex		= en.GetNetworkID();
				TempMove.fX			= pl.pl_PositionVector(1);
				TempMove.fZ			= pl.pl_PositionVector(3);
				TempMove.fH			= pl.pl_PositionVector(2);
				TempMove.fAngle		= pl.pl_OrientationAngle(1);
				m_vectorMoveList.push_back(TempMove);
			}

			// 20개를 넘어가면... 서버로 메세지를 보냄.
			if( m_vectorMoveList.size() >= MAX_MOVE_LIST )
			{
				SendMoveList(true);	
			}
		}
	}
}
// EDIT : BS : END

// ----------------------------------------------------------------------------
// Name : SendMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoveList(bool bForce)
{
	if( m_vectorMoveList.empty() )
		return;

// EDIT : BS : BEGIN
	DWORD tickCur = GetTickCount();
	// EDIT : BS : 시간 딜레이
	// 리스트가 가득 안 차고 최근 보낸지 0.5초가 안 지났으면 모은다
	if (!bForce
		&& m_vectorMoveList.size() < MAX_MOVE_LIST
		&& m_tickSendMoveList != 0
		&& tickCur - m_tickSendMoveList < 500)
		return ;
	m_tickSendMoveList = tickCur;
// EDIT : BS : END
	
	INDEX iCount		= m_vectorMoveList.size();	

	//MSG_PD_MOVE,				//무브				: movetype(uc) speed(f) x(f) z(f) h(f) r(f) y(n) multicount(c) multiindex(n:multicount)
	CNetworkMessage nmMove(MSG_PD_MOVE);
	UBYTE ubMoveType	= (UBYTE)MSG_MOVE_STOP;
	FLOAT fSpeed		= 0.0f;	
	nmMove << ubMoveType;
	nmMove << fSpeed;
	
	nmMove << (UBYTE)iCount;

	std::vector<sMoveInfo>::iterator it		= m_vectorMoveList.begin();
	std::vector<sMoveInfo>::iterator end	= m_vectorMoveList.end();
	for( ; it != end; ++it )
	{
		INDEX	iIndex	= (*it).iIndex;
		FLOAT	fX		= (*it).fX;
		FLOAT	fZ		= (*it).fZ;
		FLOAT	fH		= (*it).fH;
		FLOAT	fAngle	= (*it).fAngle;

		nmMove << iIndex;
		nmMove << fX;
		nmMove << fZ;
		nmMove << fH + 0.5f;
		nmMove << fAngle;
		nmMove << MyCharacterInfo.yLayer;			// Y Layer
	}
	SendToServerNew(nmMove);
	CPrintF("Send Move List : %d\n", iCount );
	ClearMoveList();
}

// ----------------------------------------------------------------------------
// Name : ClearMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearMoveList()
{
	if( !m_vectorMoveList.empty() )
		m_vectorMoveList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddRegenList(INDEX iIndex, INDEX iMobType, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fAngle, INDEX iYLayer, INDEX iEntityID)
{
	sRegenInfo TempRegen;
	TempRegen.iIndex		= iIndex;
	TempRegen.iMobType		= iMobType;
	TempRegen.fX			= fX;
	TempRegen.fY			= fY;
	TempRegen.fZ			= fZ;
	TempRegen.fAngle		= fAngle;
	TempRegen.iYLayer		= iYLayer;
	TempRegen.iEntityID		= iEntityID;
	m_vectorRegenList.push_back(TempRegen);
}

// ----------------------------------------------------------------------------
// Name : SendRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRegenList()
{
	if( m_vectorRegenList.empty() )
		return;
	
	INDEX iCount		= m_vectorRegenList.size();	

	std::vector<sRegenInfo>::iterator it = m_vectorRegenList.begin();
	std::vector<sRegenInfo>::iterator end = m_vectorRegenList.end();

	for(;it != end; ++it)
	{	
		CNetworkMessage nmMobSpawn(MSG_NPC_REGEN);	
		nmMobSpawn << (*it).iIndex;						// Index
		nmMobSpawn << (*it).iMobType;					// DB Index
		nmMobSpawn << (*it).fX;							// Pos X
		nmMobSpawn << (*it).fZ;							// Pos Z
		nmMobSpawn << (*it).fY;							// Pos H						
		nmMobSpawn << (*it).fAngle;						// Angle
		nmMobSpawn << (*it).iYLayer;					// Y Layer
		nmMobSpawn << (*it).iEntityID;					// EntityID
		_pNetwork->SendToServerNew(nmMobSpawn);
	}
	
	ClearRegenList();
}

// ----------------------------------------------------------------------------
// Name : ClearRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearRegenList()
{
	if( !m_vectorRegenList.empty() )
		m_vectorRegenList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddLegitList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddLegitList(int Index)
{
	if (!SearchLegitList(Index))
	{
		m_vectorLegitList.push_back(Index);
	}
}

// ----------------------------------------------------------------------------
// Name : SearchLegitList()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CNetworkLibrary::SearchLegitList(int Index, BOOL bDel/* = FALSE*/)
{
	if (!m_vectorLegitList.empty())
	{
		std::vector<int>::iterator tmpItr;

		tmpItr = std::find(m_vectorLegitList.begin(), m_vectorLegitList.end(), Index);

		if (tmpItr != m_vectorLegitList.end())
		{
			if (bDel) m_vectorLegitList.erase(tmpItr);
			return TRUE;
		}
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : DelLegitList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::DelLegitList(int Index)
{
	SearchLegitList(Index, TRUE);
}

//안태훈 수정 시작	//(Zone Change System)(0.1)
// ----------------------------------------------------------------------------
// Name : GoZone()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GoZone(int zone, int extra,int npcIdx )
{
//	const UINT iMinLevel = ZoneInfo().GetMinLevel(zone);
//	const UINT iMaxLevel = ZoneInfo().GetMaxLevel(zone);
	/*
	if(
		iMinLevel <= _pNetwork->MyCharacterInfo.level &&	// 레벨 체크
		_pNetwork->MyCharacterInfo.level <= iMaxLevel)
	{
	*/
		const int iJob = _pNetwork->MyCharacterInfo.job;
		if(ZoneInfo().GetAccessJob(zone) & (1 << iJob))			// 직업 체크
		{
			CNetworkMessage nmZone(MSG_GO_ZONE);
			nmZone << (SLONG)zone;
			nmZone << (SLONG)extra;
			
			// -- wooss 060515 ---------------------------------->>
			nmZone << (LONG)npcIdx;
			// --------------------------------------------------<<

			SendToServerNew(nmZone);
			// (eons) 신전 과 필드 이동시 지역표시 문제로 지역 번호 저장
			_pNetwork->MyCharacterInfo.LocalNo = extra;			

			CEntity* penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
			CPlayerEntity* penPlayerEntity;			
			penPlayerEntity = (CPlayerEntity*) penPlEntity;
			penPlayerEntity->DeathInit();
		}
		else
		{
			CTString strSysMessage;
			strSysMessage.PrintF( _S2( 295, JobInfo().GetName(iJob), "%s<는> 이동할수 없는 곳입니다." ), JobInfo().GetName(iJob) );
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
		}
	/*
	}
	else
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S2( 295, JobInfo().GetName(iJob), "%s<는> 이동할수 없는 곳입니다." ), JobInfo().GetName(iJob) );
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
	}
	*/
}
//안태훈 수정 끝	//(Zone Change System)(0.1)

//안태훈 수정 시작	//(Teleport System)(0.1)
void CNetworkLibrary::WriteCurrentPos(int slot, const char *szComment)
{
	CNetworkMessage nmMemPos(MSG_MEMPOS);
	nmMemPos << (SBYTE)MSG_MEMPOS_WRITE;
	nmMemPos << (SBYTE)slot;
	nmMemPos << CTString(szComment);
	SendToServerNew(nmMemPos);
}
//안태훈 수정 끝	//(Teleport System)(0.1)

//안태훈 수정 시작	//(GM Command)(0.1)
void CNetworkLibrary::SendWhoAmI()
{
	CNetworkMessage nmGM(MSG_GM);
	nmGM << (SBYTE)MSG_GM_WHOAMI;
	SendToServerNew(nmGM);
}

void CNetworkLibrary::SendGMCommand(const char *szCommand)
{
	CNetworkMessage nmGM(MSG_GM);
	nmGM << (SBYTE)MSG_GM_COMMAND;
	nmGM << CTString(szCommand);
	SendToServerNew(nmGM);
}
//안태훈 수정 끝	//(GM Command)(0.1)
//안태훈 수정 끝	//(Game Manager Command)(0.1)
//-----------------------------------------------------------------------------
// NOTE : 구출 NPC의 정보를 파티로 표시하기 위한 부분이며,
// NOTE : 현재는 파티멤버로 반드시 1명만 되도록 되어있음.
// NOTE : 추후, 변경할것.
// ----------------------------------------------------------------------------
// Name : AddRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddRescueNPC(CEntity* pEntity)
{
	if(pEntity->IsEnemy())
	{				
		CEntityProperty	&epPropertyNpcIndex	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
		const INDEX		iMobIndex = ENTITYPROPERTY( &*pEntity, epPropertyNpcIndex.ep_slOffset, INDEX);
		CMobData		&MD	= _pNetwork->GetMobData(iMobIndex);
		FLOAT3D			vPos = pEntity->en_plPlacement.pl_PositionVector;
		_pUIMgr->GetParty()->PartyAddMember( 0, 0, CTString(_pNetwork->GetMobName(iMobIndex)), 0, 0, MD.GetLevel(),
												MD.GetHealth(), MD.GetHealth(), MD.GetMP(), MD.GetMP(),
												vPos( 1 ), vPos( 3 ), 0, _pNetwork->MyCharacterInfo.zoneNo );
	}
}

// ----------------------------------------------------------------------------
// Name : UpdateRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UpdateRescueNPC(CEntity* pEntity, int iHP, int iMP)
{
	if(pEntity->IsEnemy())
	{				
		CEntityProperty	&epPropertyNpcIndex	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
		const INDEX		iMobIndex = ENTITYPROPERTY( &*pEntity, epPropertyNpcIndex.ep_slOffset, INDEX);
		CMobData		&MD	= _pNetwork->GetMobData(iMobIndex);
		FLOAT3D			vPos = pEntity->en_plPlacement.pl_PositionVector;
		//_pUIMgr->GetParty()->PartyAddMember(0, 0, CTString(MD.GetMonsterName()), 1, MD.GetLevel(), MD.GetHealth(), MD.GetHealth(),  MD.GetMP(), MD.GetMP());
		_pUIMgr->GetParty()->PartyMemberInfo( 0, MD.GetLevel(), iHP, MD.GetHealth(), iMP, MD.GetMP(),
												vPos( 1 ), vPos( 3 ), 0, _pNetwork->MyCharacterInfo.zoneNo );		
	}
}

// ----------------------------------------------------------------------------
// Name : EndRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::EndRescueNPC()
{
	if(_pUIMgr->GetParty()->GetMemberCount())
		_pUIMgr->GetParty()->PartyEnd();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name : 
// Desc : 대여 아이템 정렬을 위한 비교함수
//-----------------------------------------------------------------------------
bool ItemLevelOfComp( const int& x, const int& y )
{
	CItemData	rItemData[2];

	rItemData[0] = _pNetwork->GetItemData(x);
	rItemData[1] = _pNetwork->GetItemData(y);

	return rItemData[0].GetLevel() < rItemData[1].GetLevel();
}

//-----------------------------------------------------------------------------
// Purpose: 대여하는 아이템 목록을 설정해줌.
// Input  : job index ( 1 - 6 )
//-----------------------------------------------------------------------------
int CNetworkLibrary::RefreshLeaseItem(int iJobIndex)
{
	if(iJobIndex == -1)	return - 1;
				
	CItemData	rItemData[2];
	
	int			tv_itemIdx;
	int			tv_itemLevel;
	int			iRow,iCol;

	std::vector<int> vecbtnItems;

	for(int t = 0, selNum=0 ; t < wo_iNumOfItem; ++t)
	{	
		if(	wo_aItemData[t].GetJob() == 0x01 << iJobIndex &&
			wo_aItemData[t].GetType() == CItemData::ITEM_WEAPON )
		{
			tv_itemLevel = wo_aItemData[t].GetLevel();
			if(	tv_itemLevel >= LEASE_MIN_LEVEL &&
				tv_itemLevel <= LEASE_MAX_LEVEL )
			{
				if(wo_aItemData[t].IsFlag(ITEM_FLAG_LENT) )
				{
					tv_itemIdx = wo_aItemData[t].GetItemIndex();

					vecbtnItems.push_back(tv_itemIdx);

					selNum++;
				}
			}
		}
	}
	// 아이템 레벨별 정렬
	std::sort(vecbtnItems.begin(), vecbtnItems.end(), ItemLevelOfComp );

	std::vector<int>::iterator btnItr;

	for ( btnItr=vecbtnItems.begin(), selNum=0; btnItr!=vecbtnItems.end(); btnItr++ )
	{
		tv_itemIdx = (*btnItr);
		
		if(selNum !=0)
		{
			rItemData[0] = _pNetwork->GetItemData((*btnItr));
			rItemData[1] = _pNetwork->GetItemData(
			(_pUIMgr->GetShop()->m_abtnShopItems[0]+selNum-1)->GetItemIndex());
				
			if(rItemData[1].GetLevel() == rItemData[0].GetLevel())
			{
				// 나중에 서브타입이 작다면 바꿔준다.
				if(rItemData[1].GetSubType() > rItemData[0].GetSubType())
				{
					iRow = (selNum-1) / SHOP_SHOP_SLOT_COL;
					iCol = (selNum-1) % SHOP_SHOP_SLOT_COL;
					_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, tv_itemIdx, -1, -1 );
					tv_itemIdx = rItemData[1].GetItemIndex();																										 
				}
			}
		}
		iRow = selNum / SHOP_SHOP_SLOT_COL;
		iCol = selNum % SHOP_SHOP_SLOT_COL;
		// 이부분이 Shop 인벤토리 안으로 들어갈것...
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, tv_itemIdx, -1, -1 );
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemCount( 1 );
		selNum++;
	}

	return selNum;
}

//-----------------------------------------------------------------------------
// Purpose: 판매하는 아이템 목록을 인벤토리에 설정해줌.
// Input  : SD - 
//-----------------------------------------------------------------------------
int CNetworkLibrary::RefreshShopItem(int iShopIndex)
{
	if(iShopIndex == -1)	return - 1;
	//if(_pUIMgr->GetShop()->IsVisible())	return;
	CShopData &SD = _pNetwork->GetShopData(iShopIndex);
	if(SD.GetIndex() == -1) return - 1;
	for(int t = 0; t < SD.m_iNumOfItem; ++t)
	{						
		int iItemIndex = SD.m_vectorSellItems[t];
		CItemData &ID = _pNetwork->GetItemData(iItemIndex);
		if(ID.GetItemIndex() == -1) continue;

		const int iRow = t / SHOP_SHOP_SLOT_COL;
		const int iCol = t % SHOP_SHOP_SLOT_COL;
		// 이부분이 Shop 인벤토리 안으로 들어갈것...
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, iItemIndex, -1, -1 );
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemCount( 1 );
	}

	return t;
}
//-----------------------------------------------------------------------------
// Purpose: 이벤트로 판매할  아이템 목록을 설정해줌. 무기대여상의 로직 적용
// Input  : job index ( 1 - 6 )
//-----------------------------------------------------------------------------
#define EVENT_MAY_LEVELMIN (5)
#define EVENT_MAY_LEVELMAX (32)

int CNetworkLibrary::RefreshEventItem(int iJobIndex, int itype)
{
	if(iJobIndex == -1)	return - 1;
				
	CItemData	rItemData[2];
	
	int			tv_itemIdx;
	int			tv_itemLevel;
	int			iRow,iCol;

	std::vector<int> vecbtnItems;
	
	CShopData SD;
	
	if(itype == CItemData::ITEM_SHIELD)		// 상점 NPC의 아이템을 가져옴
	{
		SD = (CShopData)_pNetwork->GetShopData(90);			// 방어구 상인
	}else
	{
		SD = (CShopData)_pNetwork->GetShopData(33);			// 무기상인
	}	

	for(int t = 0, selNum=0 ; t < SD.m_iNumOfItem; ++t)
	{	
		int iItemIndex = SD.m_vectorSellItems[t];
		CItemData &ID = _pNetwork->GetItemData(iItemIndex);
		
		if( ID.GetJob() == 0x01 << iJobIndex &&	ID.GetType() == itype )
		{
			tv_itemLevel = ID.GetLevel();
			if(	tv_itemLevel >= EVENT_MAY_LEVELMIN &&
				tv_itemLevel <= EVENT_MAY_LEVELMAX )
			{			
				tv_itemIdx = ID.GetItemIndex();

				vecbtnItems.push_back(tv_itemIdx);

				selNum++;
				
			}
		}
	}
	// 아이템 레벨별 정렬
	std::sort(vecbtnItems.begin(), vecbtnItems.end(), ItemLevelOfComp );

	std::vector<int>::iterator btnItr;

	for ( btnItr=vecbtnItems.begin(), selNum=0; btnItr!=vecbtnItems.end(); btnItr++ )
	{
		tv_itemIdx = (*btnItr);
		
		if(selNum !=0)
		{
			rItemData[0] = _pNetwork->GetItemData((*btnItr));
			rItemData[1] = _pNetwork->GetItemData(
			(_pUIMgr->GetShop()->m_abtnShopItems[0]+selNum-1)->GetItemIndex());
				
			if(rItemData[1].GetLevel() == rItemData[0].GetLevel())
			{
				// 나중에 서브타입이 작다면 바꿔준다.
				if(rItemData[1].GetSubType() > rItemData[0].GetSubType())
				{
					iRow = (selNum-1) / SHOP_SHOP_SLOT_COL;
					iCol = (selNum-1) % SHOP_SHOP_SLOT_COL;
					_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, tv_itemIdx, -1, -1 );
					tv_itemIdx = rItemData[1].GetItemIndex();																										 
				}
			}
		}
		iRow = selNum / SHOP_SHOP_SLOT_COL;
		iCol = selNum % SHOP_SHOP_SLOT_COL;
		// 이부분이 Shop 인벤토리 안으로 들어갈것...
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemInfo( 0, iRow, iCol, tv_itemIdx, -1, -1 );
		_pUIMgr->GetShop()->m_abtnShopItems[iRow][iCol].SetItemCount( 1 );
		selNum++;
	}

	return selNum;
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 대여 합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendLeaseItem(int iItemIdx)
{
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_LEND_WEAPON;
	nmItem << (LONG)iItemIdx;						// ITEM INDEX
	SendToServerNew(nmItem);	
}
//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 이벤트 아이템을 구입합니다. (2007 가정의 달 이벤트)
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventItem(int iItemIdx, int iItemcont)
{
	CNetworkMessage nmItem(MSG_EVENT);
	nmItem << (SBYTE)MSG_EVENT_CHILDRENSDAY_2007;
	nmItem << (LONG)MSG_EVENT_CHILDRENSDAY_2007_REQ;
	nmItem << (LONG)iItemIdx;						// ITEM INDEX
	nmItem << (LONG)iItemcont;
	SendToServerNew(nmItem);	
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 구입합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::BuyItem(int iShopID, int iNumOfItem, __int64 iTotalPrice)
{
	__int64 iSumPrice	= iTotalPrice;	
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_BUY;
	nmItem << (SLONG)iShopID;						// NPC INDEX
	nmItem << iTotalPrice;							// PRICE
	nmItem << (SLONG)iNumOfItem;
	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
	{
		if( _pUIMgr->GetShop()->m_abtnTradeItems[i].IsEmpty() )
			break;
		SLONG	slIndex = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemIndex();
		SQUAD	llItemCount = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCount();
////		CPrintF("Buy IDX:%d,Cnt:%d\n", slIndex, llItemCount);
		nmItem << slIndex;
		nmItem << llItemCount;
	}
	SendToServerNew(nmItem);	
//	CPrintF("Send MSG_ITEM_BUY, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);	
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 판매합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SellItem(int iShopID, int iNumOfItem, __int64 iTotalPrice)
{
	__int64 iSumPrice = iTotalPrice;
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_SELL;
	nmItem << (SLONG)iShopID;						// NPC INDEX
	nmItem << iTotalPrice;							// PRICE	
	nmItem << (SLONG)iNumOfItem;
	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
	{
		if( _pUIMgr->GetShop()->m_abtnTradeItems[i].IsEmpty() )
			break;
		SBYTE	sbTab = 0;
		SBYTE	sbRow = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemRow();
		SBYTE	sbCol = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCol();
		SLONG	slUniIndex = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemUniIndex();
		SQUAD	llCnt = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCount();
		if(_pNetwork->m_ubGMLevel > 1)
			CPrintF("Sell Tab:%d,Row:%d,Col:%d,UDX:%d,Cnt:%d\n", sbTab, sbRow, sbCol, slUniIndex, llCnt);
		nmItem << sbTab;					// TAB
		nmItem << sbRow;					// ROW
		nmItem << sbCol;					// COL
		nmItem << llCnt;					// COUNT
	}		
	SendToServerNew(nmItem);
//	CPrintF("Send MSG_ITEM_SELL, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);
}
//-----------------------------------------------------------------------------
// Purpose: 잡화상 이용 주문서 목록에 있는 아이템을 구입합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::FieldShopBuyItem( int iNumOfItem, __int64 iTotalPrice)
{
	__int64 iSumPrice	= iTotalPrice;	
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_USE_GROCERY_BUY;
	nmItem << iTotalPrice;							// PRICE
	nmItem << (SLONG)iNumOfItem;
	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
	{
		if( _pUIMgr->GetShop()->m_abtnTradeItems[i].IsEmpty() )
			break;
		SLONG	slIndex = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemIndex();
		SQUAD	llItemCount = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCount();
////		CPrintF("Buy IDX:%d,Cnt:%d\n", slIndex, llItemCount);
		nmItem << slIndex;
		nmItem << llItemCount;
	}
	SendToServerNew(nmItem);	
//	CPrintF("Send MSG_ITEM_BUY, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);	
}
//-----------------------------------------------------------------------------
// Purpose: 잡화상 이용 주문서 목록에 있는 아이템을 판매합니다.
// Input  :  
//-----------------------------------------------------------------------------
void CNetworkLibrary::FieldShopSellItem(int iNumOfItem, __int64 iTotalPrice)
{
	__int64 iSumPrice = iTotalPrice;
	CNetworkMessage nmItem(MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_USE_GROCERY_SELL;
	nmItem << iTotalPrice;							// PRICE	
	nmItem << (SLONG)iNumOfItem;
	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
	{
		if( _pUIMgr->GetShop()->m_abtnTradeItems[i].IsEmpty() )
			break;
		SBYTE	sbTab = 0;
		SBYTE	sbRow = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemRow();
		SBYTE	sbCol = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCol();
		SLONG	slUniIndex = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemUniIndex();
		SQUAD	llCnt = _pUIMgr->GetShop()->m_abtnTradeItems[i].GetItemCount();
		if(_pNetwork->m_ubGMLevel > 1)
			CPrintF("Sell Tab:%d,Row:%d,Col:%d,UDX:%d,Cnt:%d\n", sbTab, sbRow, sbCol, slUniIndex, llCnt);
		nmItem << sbTab;					// TAB
		nmItem << sbRow;					// ROW
		nmItem << sbCol;					// COL
		nmItem << llCnt;					// COUNT
	}		
	SendToServerNew(nmItem);
//	CPrintF("Send MSG_ITEM_SELL, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);
}


//0616 kwon
void CNetworkLibrary::SendChatMessage(int index, CTString &strRecvName, CTString &strMessage)
{
	CNetworkMessage nm(MSG_CHAT);
	nm << (unsigned char)index;
	nm << MyCharacterInfo.index;
	nm << MyCharacterInfo.name;
	nm << strRecvName;
	nm << strMessage;
	
	SendToServerNew(nm);
}

//0701 kwon
void CNetworkLibrary::SendStopMessage(CEntity *pEntity, CPlacement3D MyPlacement)
{
	if( !pEntity )
		return;

	if(_cmiComm. IsNetworkOn())
	{		
		CNetworkMessage nmPlayerMove(MSG_MOVE); 	
		nmPlayerMove << (UBYTE)pEntity->GetNetworkType();
		nmPlayerMove << (UBYTE)MSG_MOVE_STOP;				
		nmPlayerMove << pEntity->GetNetworkID();
		nmPlayerMove << 0.0f;//plr_fSpeed;
		nmPlayerMove << MyPlacement.pl_PositionVector(1);
		nmPlayerMove << MyPlacement.pl_PositionVector(3);
		nmPlayerMove << MyPlacement.pl_PositionVector(2);
		nmPlayerMove << MyPlacement.pl_OrientationAngle(1);
		nmPlayerMove << MyCharacterInfo.yLayer;
		SendToServerNew(nmPlayerMove);	
	}
}

//클라이언트 아이템 인덱스로 서버인덱스를 찾는다.
SLONG CNetworkLibrary::SearchItemIndex(ULONG Index)
{
	SLONG item_index = -1;
	for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_aitItem.Count(); ++ipl) 
	{//클라이언트 인덱스로 아이템 유니크 인덱스 찾기.
		CItemTarget &it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
		if (it.item_iClientIndex == Index)
		{								
			item_index = it.item_Index;
			break;
		}
	}
	return item_index; 
}

SLONG CNetworkLibrary::SearchClientChaIndex(ULONG Index)//이건 클라이언트 인덱스를 찾는 함수.
{
	SLONG	cha_index = -1;
	for(INDEX ipl=0; ipl<ga_srvServer.srv_actCha.Count(); ++ipl) 
	{
		CCharacterTarget &ct = ga_srvServer.srv_actCha[ipl];
		if (ct.cha_Index == Index) 
		{																			
			cha_index = ct.cha_iClientIndex;
			break;
		}
	}
	return cha_index;
}

SLONG CNetworkLibrary::SearchClientMobIndex(ULONG Index)
{
	SLONG	mob_index = -1;
	for(INDEX ipl=0; ipl<ga_srvServer.srv_amtMob.Count(); ++ipl) 
	{
		CMobTarget &mt = ga_srvServer.srv_amtMob[ipl];
		if (mt.mob_Index == Index) 
		{																			
			mob_index = mt.mob_iClientIndex;
			break;
		}
	}
	return mob_index;
}

SLONG CNetworkLibrary::SearchClientPetIndex(ULONG Index)
{
	SLONG	pet_index = -1;
	for(INDEX ipl=0; ipl<ga_srvServer.srv_actPet.Count(); ++ipl) 
	{
		CPetTarget &mt = ga_srvServer.srv_actPet[ipl];
		if (mt.pet_Index == Index) 
		{																			
			pet_index = mt.pet_iClientIndex;
			break;
		}
	}
	return pet_index;
}

SLONG CNetworkLibrary::SearchClientSlaveIndex(ULONG Index)
{
	SLONG	slave_index = -1;
	for(INDEX ipl=0; ipl<ga_srvServer.srv_actSlave.Count(); ++ipl) 
	{
		CSlaveTarget &st = ga_srvServer.srv_actSlave[ipl];
		if (st.slave_Index == Index) 
		{																			
			slave_index = st.slave_iClientIndex;
			break;
		}
	}
	return slave_index;
}

//-----------------------------------------------------------------------------
// Purpose: 네트워크 ID값으로 엔티티를 얻습니다.
// Input  : 
//-----------------------------------------------------------------------------
BOOL CNetworkLibrary::SearchEntityByNetworkID( long lIndex, SBYTE sbType, CEntity* &pEntity )
{
	// 캐릭터에 대해서...
	if( sbType == MSG_CHAR_PC )
	{
		if( lIndex == MyCharacterInfo.index )
		{
			pEntity = CEntity::GetPlayerEntity(0);
			return TRUE;
		}

		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actCha.Count(); ipl2++) 
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl2];
			if (ct.cha_Index == lIndex )
			{				
				return ga_World.EntityExists(ct.cha_iClientIndex, pEntity);
			}
		}
	}
	// 몹에 대해서...
	else if( sbType == MSG_CHAR_NPC )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_amtMob.Count(); ipl2++) 
		{
			CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[ipl2];
			if (mt.mob_Index == lIndex) 
			{				
				return _pNetwork->ga_World.EntityExists(mt.mob_iClientIndex, pEntity);
			}
		}
	}
	// 애완동물에 대해서...
	else if( sbType == MSG_CHAR_PET )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actPet.Count(); ipl2++) 
		{
			CPetTarget &pt = _pNetwork->ga_srvServer.srv_actPet[ipl2];
			if (pt.pet_Index == lIndex) 
			{			
				return ga_World.EntityExists(pt.pet_iClientIndex, pEntity);
			}
		}
	}
	// 소환수에 대해서...
	else if( sbType == MSG_CHAR_ELEMENTAL )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actSlave.Count(); ipl2++) 
		{
			CSlaveTarget &st = _pNetwork->ga_srvServer.srv_actSlave[ipl2];
			if (st.slave_Index == lIndex) 
			{			
				return ga_World.EntityExists(st.slave_iClientIndex, pEntity);
			}
		}
	}
	// 애완동물에 대해서...
	else if( sbType == MSG_CHAR_WILDPET )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl2++) 
		{
			CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl2];
			if (pt.m_nNetIndex == lIndex) 
			{			
				return ga_World.EntityExists(pt.pet_iClientIndex, pEntity);
			}
		}
	}

	return FALSE;
}

//======================================================================================================
// Name : SearchEntityByNetworkID
// Exlplain : 인덱스 정보로 CEntity 및 CCharacterTarget Count 정보를 얻는다.
// 
//======================================================================================================
BOOL CNetworkLibrary::SearchEntityByNetworkID(long lIndex, SBYTE sbType, CEntity* &pEntity, INDEX& cnt)
{
	INDEX ipl2;

	if( sbType == MSG_CHAR_PC )
	{
		for ( ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actCha.Count(); ipl2++ ) 
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl2];
			if (ct.cha_Index == lIndex )
			{				
				if ( ga_World.EntityExists(ct.cha_iClientIndex, pEntity) )
				{
					cnt = ipl2;
					return TRUE;
				}
			}
		}		
	}
	else if( sbType == MSG_CHAR_NPC )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_amtMob.Count(); ipl2++) 
		{
			CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[ipl2];
			if (mt.mob_Index == lIndex) 
			{				
				if ( ga_World.EntityExists(mt.mob_iClientIndex, pEntity) )
				{
					cnt = ipl2;
					return TRUE;
				}
			}
		}
	}
	else if( sbType == MSG_CHAR_PET )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actPet.Count(); ipl2++) 
		{
			CPetTarget &pt = _pNetwork->ga_srvServer.srv_actPet[ipl2];
			if (pt.pet_Index == lIndex) 
			{			
				if ( ga_World.EntityExists(pt.pet_iClientIndex, pEntity) )
				{
					cnt = ipl2;
					return TRUE;
				}
			}
		}
	}
	else if( sbType == MSG_CHAR_ELEMENTAL )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actSlave.Count(); ipl2++) 
		{
			CSlaveTarget &st = _pNetwork->ga_srvServer.srv_actSlave[ipl2];
			if (st.slave_Index == lIndex) 
			{			
				if ( ga_World.EntityExists(st.slave_iClientIndex, pEntity) )
				{
					cnt = ipl2;
					return TRUE;
				}
			}
		}
	}
	else if( sbType == MSG_CHAR_WILDPET )
	{
		for(INDEX ipl2=0; ipl2<_pNetwork->ga_srvServer.srv_actWildPet.Count(); ipl2++) 
		{
			CWildPetInfo &pt = _pNetwork->ga_srvServer.srv_actWildPet[ipl2];
			if (pt.m_nNetIndex == lIndex) 
			{			
				if ( ga_World.EntityExists(pt.pet_iClientIndex, pEntity) )
				{
					cnt = ipl2;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CNetworkLibrary::SendPickMessage( CEntity* pEntity, ULONG ItemIndex, BOOL bLayerCheck )
{
	if(_cmiComm. IsNetworkOn())
	{
		if( bLayerCheck )
		{
			for( INDEX i = 0; i < _pNetwork->ga_srvServer.srv_aitItem.Count(); ++i )
			{
				CItemTarget	&it = _pNetwork->ga_srvServer.srv_aitItem[i];
				if( it.item_Index == ItemIndex )
				{
					if( abs( it.item_yLayer - MyCharacterInfo.yLayer ) > 1 )
						return;

					break;
				}
			}
		}

		CNetworkMessage nmItem(MSG_ITEM); 	
		nmItem << (UBYTE)MSG_ITEM_TAKE;									
		nmItem << pEntity->GetNetworkType();
		nmItem << pEntity->GetNetworkID();
		nmItem << ItemIndex;							
		SendToServerNew(nmItem);	
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : plPlacement - 
//			speed - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendMoveMessage(CEntity *pEntity, CPlacement3D plPlacement, SLONG speed)
{
	if( !pEntity )
		return;

	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerMove(MSG_MOVE);
		SBYTE sbNetworkType = pEntity->GetNetworkType();
		nmPlayerMove << (UBYTE)sbNetworkType;
		if(speed < 2.0f)
		{							
			nmPlayerMove << (UBYTE)MSG_MOVE_WALK;
		}
		else
		{
			nmPlayerMove << (UBYTE)MSG_MOVE_RUN;
		}
		nmPlayerMove << pEntity->GetNetworkID();
		nmPlayerMove << speed;						
		nmPlayerMove << plPlacement.pl_PositionVector(1);
		nmPlayerMove << plPlacement.pl_PositionVector(3);
		nmPlayerMove << plPlacement.pl_PositionVector(2);
		nmPlayerMove << plPlacement.pl_OrientationAngle(1);
		nmPlayerMove << MyCharacterInfo.yLayer;
		
		SendToServerNew(nmPlayerMove);

		// 길드 전쟁중 포탈을 타려고 하는데 이동을  하면 취소 (중지)
		_pUIMgr->GetGuildWarPortal()->PortalCancel();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackSymbol()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_ATTACK_SYMBOL;
	SendToServerNew(nmGuildWar);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackMessage(CEntity *pAttackerEntity, CEntity *pTargetEntity, BOOL bPvp) 
{
	if( !pAttackerEntity || !pTargetEntity )
	{
		return;
	}
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerAttack(MSG_ATTACK);
		nmPlayerAttack << (UBYTE)pAttackerEntity->GetNetworkType();
		nmPlayerAttack << pAttackerEntity->GetNetworkID();
		
		nmPlayerAttack << (UBYTE)pTargetEntity->GetNetworkType();
		INDEX iNetworkID = pTargetEntity->GetNetworkID();
		nmPlayerAttack << iNetworkID;
		nmPlayerAttack<< (UBYTE)0;
		nmPlayerAttack<< (UBYTE)0;
		
		SendToServerNew(nmPlayerAttack);		
	}
}

//-----------------------------------------------------------------------------
// Purpose: Booster 업글.
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendBoosterUpgrade(LONG itemServerIndex)
{
	CNetworkMessage nmPlayerProcess(MSG_ITEM);
	nmPlayerProcess << (unsigned char)MSG_ITEM_ADD_BOOSTER;
	nmPlayerProcess << itemServerIndex;
	SendToServerNew(nmPlayerProcess);	
}

//-----------------------------------------------------------------------------
// Purpose: NPC 통해서 가공하기
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendNPCProcessMessage(LONG processItemDBIndex, LONG count)
{
	//MSG_ITEM_PROCESS_NPC,		// NPC를 통해 가공		: processItemDBIndex(n) count(n) errcode(n:s)
	if(!_cmiComm.IsNetworkOn()) return;

	CNetworkMessage nmPlayerProcess(MSG_ITEM);
	nmPlayerProcess << (unsigned char)MSG_ITEM_PROCESS_NPC;
	nmPlayerProcess << processItemDBIndex;
	nmPlayerProcess << count;
	SendToServerNew(nmPlayerProcess);	
}

//-----------------------------------------------------------------------------
// Purpose: 생산하기.
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSelectProduceMessage(LONG npc_client_index, LONG item_db_index)
{
	// 생산 관련		: producekind(c) attackchartype(c) attackIndex(n) targetchartype(c) targetindex(n)
	if(!_cmiComm.IsNetworkOn()) return;

	if( item_db_index >= 0 )
	{
		CNetworkMessage nmPlayerProduce(MSG_SELECT_PRODUCE);
		nmPlayerProduce << npc_client_index;
		nmPlayerProduce << item_db_index;
		SendToServerNew(nmPlayerProduce);	
	}
	else
	{
		CNetworkMessage nmPlayerProduce( MSG_RANDOM_PRODUCE );
		nmPlayerProduce << npc_client_index;
			SendToServerNew(nmPlayerProduce);
	}
}


//---------------------------------------------------------------------->>
// Name : SendSelectProduceMessage()
// Desc : Produce some collect goods
// Date : [7/20/2006] , Wooss
//----------------------------------------------------------------------<<

void CNetworkLibrary::SendCollectProduceMessage(LONG npc_client_index)
{
	if(!_cmiComm.IsNetworkOn()) return;

	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_COLLECT;
	nmQuest << npc_client_index;

	SendToServerNew(nmQuest);

	
}


void CNetworkLibrary::SendProduceMessage(ULONG Index, SBYTE sbKind)
{
	// 생산 관련		: producekind(c) attackchartype(c) attackIndex(n) targetchartype(c) targetindex(n)
/*	if(_cmiComm. IsNetworkOn())
	{		
		CNetworkMessage nmPlayerAttack(MSG_PRODUCE);
		nmPlayerAttack << (UBYTE)sbKind;
		nmPlayerAttack << (UBYTE)MSG_CHAR_PC;						
		nmPlayerAttack << MyCharacterInfo.index;
		nmPlayerAttack << (UBYTE)MSG_CHAR_NPC;
		
		CEntity* penEntity;
		INDEX ipl;
		for(ipl=0; ipl<ga_srvServer.srv_amtMob.Count(); ++ipl) 
		{
			CMobTarget &mt = ga_srvServer.srv_amtMob[ipl];
			if (mt.mob_iClientIndex == Index) 
			{																			
				if (ga_World.EntityExists(mt.mob_iClientIndex,penEntity)) 
				{
					nmPlayerAttack<< mt.mob_Index;
					if(_pNetwork->m_ubGMLevel > 1)
						CPrintF("Send Produce Message : index: %d ClientID: %d \n", mt.mob_Index,Index );
				}
				break;
			}
		}
		SendToServerNew(nmPlayerAttack);	
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 배열내에 있는 모든 엔티티에 대해서 Attack Message를 보냅니다(싱글 모드에서만 사용됨)
// Input  : &dcEntities - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackMessageInContainer(CSelectedEntities &dcEntities)
{
	if(_pNetwork->m_bSingleMode)
	{	
		for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
		it != dcEntities.vectorSelectedEntities.end(); ++it )
		{
			CEntity &en = *(*it);
// EDIT : BS : BEGIN
//			if( en.IsEnemy() || en.IsSlave() || en.IsPet() )
//			{
//				CPlacement3D pl		= en.GetLerpedPlacement();
//				_pNetwork->AddMoveList( 
//					en.GetNetworkID(),
//					pl.pl_PositionVector(1), 
//					pl.pl_PositionVector(3), 
//					pl.pl_PositionVector(2),
//					pl.pl_OrientationAngle(1) );
//			}			
			_pNetwork->AddMoveList(en);
// EDIT : BS : END
		}
		
		_pNetwork->SendMoveList();
	}
	
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerAttack(MSG_ATTACK);						
		
		nmPlayerAttack << (UBYTE)MSG_CHAR_PC;						
		nmPlayerAttack << MyCharacterInfo.index;			
		
		nmPlayerAttack << (UBYTE)MSG_CHAR_NPC;
		
		BOOL bFirst = FALSE;
		// for each entity in container		
		for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
			it != dcEntities.vectorSelectedEntities.end(); ++it )
		{
			CEntity &en = *(*it);
			INDEX iMobIndex = en.GetNetworkID();
			if(!bFirst)
			{
				bFirst = TRUE;					
				nmPlayerAttack << (ULONG)iMobIndex;
				nmPlayerAttack << (UBYTE)0;
				nmPlayerAttack << (UBYTE)(dcEntities.Count() - 1);
				continue;
			}
			nmPlayerAttack << (ULONG)iMobIndex;
		}
		SendToServerNew(nmPlayerAttack);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 스킬 사용 메세지를 보내줌.
// Input  : nSkillIndex - 
//			nTargetIndex - 
//			bFire - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
void CNetworkLibrary::SendSkillMessage(int nSkillIndex, CEntity *pEntity, int nTargetIndex, BOOL bFire, UBYTE ubMove/*=0*/)
{
	ASSERT( pEntity != NULL && "Invalid Entity Pointer" );
	if( pEntity == NULL )
		return;

	CSkill &SkillData = GetSkillData( nSkillIndex );//0807

	// FIXME : 애완동물을 타고 있는 상태에서 스킬을 쓰려고 할때...
	if( pEntity->IsPlayer() 
		&& _pNetwork->MyCharacterInfo.bPetRide 
		&& SkillData.GetJob() == PET_JOB )
	{	
		SendPetSkillMessage( nSkillIndex, NULL, pEntity, bFire );
		return;
	}
	
	if(_cmiComm. IsNetworkOn())
	{	
		if( _pNetwork->m_bSingleMode && pEntity->IsEnemy() )
		{
			CPlacement3D pl		= pEntity->GetLerpedPlacement();
			_pNetwork->AddMoveList( 
				pEntity->GetNetworkID(),
				pl.pl_PositionVector(1), 
				pl.pl_PositionVector(3), 
				pl.pl_PositionVector(2),
				pl.pl_OrientationAngle(1) );		
			_pNetwork->SendMoveList();
		}

		// char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count
		CTString strSysMessage;
		CNetworkMessage nmPlayerSkill(MSG_SKILL);
		if(bFire)
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_FIRE;	
			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
		}
		else
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_READY;			
			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
		}
		ClientSystemMessage( strSysMessage);
		UBYTE ubCharType = (UBYTE)MSG_CHAR_PC;
		nmPlayerSkill << ubCharType;
		nmPlayerSkill << MyCharacterInfo.index;
		nmPlayerSkill << (ULONG)nSkillIndex;
		LONG nTargetType;
		nTargetType = pEntity->GetNetworkType();
		nmPlayerSkill << (unsigned char)nTargetType; //우선은 타겟=NPC만.
		nmPlayerSkill << (ULONG)nTargetIndex; 
		nmPlayerSkill << (SBYTE)0;
		nmPlayerSkill << (ULONG)0; // skillspeed (사용안함 현재 0고정 )

		CEntity *penPlEntity = NULL;
		CPlayerEntity *penPlayerEntity = NULL;
		FLOAT3D plVector = FLOAT3D( 0.0f, 0.0f, 0.0f );
		ANGLE3D plAngle = ANGLE3D( 0.0f, 0.0f, 0.0f );
		SBYTE sbLayer = 0;

		if ( nSkillIndex == 401 && ubMove == 1 ) // 대쉬
		{
			penPlEntity = CEntity::GetPlayerEntity(0);
			penPlayerEntity = (CPlayerEntity*)penPlEntity;
			CPlacement3D pl = penPlayerEntity->GetPlacement();

			plVector = pl.pl_PositionVector;
			plAngle = pl.pl_OrientationAngle;

			INDEX Cnt;

			if ( _pNetwork->SearchEntityByNetworkID( nTargetIndex, nTargetType, pEntity, Cnt) )
			{
				switch( nTargetType )
				{
				case MSG_CHAR_PC:
					{
						sbLayer = ga_srvServer.srv_actCha[Cnt].cha_yLayer;
					}
					break;
				case MSG_CHAR_NPC:
					{
						sbLayer = ga_srvServer.srv_amtMob[Cnt].mob_yLayer;
					}
					break;
				case MSG_CHAR_PET:
					{
						sbLayer = ga_srvServer.srv_actPet[Cnt].pet_yLayer;
					}
					break;
				case MSG_CHAR_ELEMENTAL:
					{
						sbLayer = ga_srvServer.srv_actSlave[Cnt].slave_yLayer;
					}
					break;
				}
			}
		}

		nmPlayerSkill << (UBYTE)ubMove; // 이동 여부
		nmPlayerSkill << plVector(1); // x
		nmPlayerSkill << plVector(3); // z
		nmPlayerSkill << plVector(2); // h
		nmPlayerSkill << plAngle(1); // r
		nmPlayerSkill << sbLayer; // y(c) // 층

		SendToServerNew(nmPlayerSkill);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//			
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSkillMessage 계열들 정리 필요.
void CNetworkLibrary::SendPetSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire)
{	
	CSkill &SkillData = GetSkillData( nSkillIndex );

	// char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count
	CTString strSysMessage;
	CNetworkMessage nmPetSkill(MSG_SKILL);
	if(bFire)
	{
		nmPetSkill << (UBYTE)MSG_SKILL_FIRE;	
		strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
	}
	else
	{
		nmPetSkill << (UBYTE)MSG_SKILL_READY;			
		strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
	}
	ClientSystemMessage( strSysMessage);	
	nmPetSkill << (UBYTE)MSG_CHAR_PET;
	nmPetSkill << (LONG)_pNetwork->_PetTargetInfo.lIndex;
	nmPetSkill << (ULONG)nSkillIndex;		
	nmPetSkill << (unsigned char)pTargetEntity->GetNetworkType();
	nmPetSkill << (ULONG)pTargetEntity->GetNetworkID(); 
	nmPetSkill << (SBYTE)0;
	SendToServerNew(nmPetSkill);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//			
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSkillMessage 계열들 정리 필요.
void CNetworkLibrary::SendSlaveSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire)
{
	//ASSERT( pSourceEntity != NULL && "Invalid Entity Pointer" );
	//if( pSourceEntity == NULL )
	//	return;

	CSkill &SkillData = GetSkillData( nSkillIndex );

	// char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count
	CTString strSysMessage;
	CNetworkMessage nmSkill(MSG_SKILL);
	if(bFire)
	{
		nmSkill << (UBYTE)MSG_SKILL_FIRE;	
		strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
	}
	else
	{
		nmSkill << (UBYTE)MSG_SKILL_READY;			
		strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
	}
	ClientSystemMessage( strSysMessage);	
	nmSkill << (UBYTE)pSourceEntity->GetNetworkType();
	nmSkill << (LONG)pSourceEntity->GetNetworkID();
	nmSkill << (ULONG)nSkillIndex;		
	nmSkill << (unsigned char)pTargetEntity->GetNetworkType();
	nmSkill << (ULONG)pTargetEntity->GetNetworkID(); 
	nmSkill << (SBYTE)0;
	SendToServerNew(nmSkill);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendPetCommandMessage( int nSkillIndex, CEntity *pSourceEntity )
{
	// MSG_EX_PET_COMMAND,			// 펫 사교동작		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
	ASSERT( pSourceEntity != NULL && "Invalid Entity Pointer" );
	if( pSourceEntity == NULL )
		return;	

	if( !pSourceEntity->IsPet() )
		return;

	CSkill &SkillData = _pNetwork->GetSkillData(nSkillIndex);

	const int iSkillType = SkillData.GetType();

	// 펫 커맨드 스킬만!!!
	if( iSkillType != CSkill::ST_PET_COMMAND )
	{
		return;
	}

	CNetworkMessage nmPet( MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_COMMAND;
	nmPet << pSourceEntity->GetNetworkID();	
	nmPet << (LONG)nSkillIndex;
	nmPet << pSourceEntity->GetNetworkType();
	nmPet << pSourceEntity->GetNetworkID();
	SendToServerNew(nmPet);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nSkillIndex - 
//			&dcEntities - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSlaveSkillMessageInContainer() 소화수의 스킬 메세지.
void CNetworkLibrary::SendSkillMessageInContainer(int nSkillIndex, CSelectedEntities &dcEntities, BOOL bFire)
{
	INDEX iMobCount	= dcEntities.Count();

	CPrintF( "Send Skill Attack : %d\n", iMobCount );
	if(_cmiComm. IsNetworkOn() && iMobCount > 0)
	{
		if(_pNetwork->m_bSingleMode)
		{
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en = *(*it);
// EDIT : BS : BEGIN
//				if( en.IsEnemy() || en.IsSlave() || en.IsPet() )
//				{
//					CPlacement3D pl		= en.GetLerpedPlacement();
//					_pNetwork->AddMoveList( 
//						en.GetNetworkID(),
//						pl.pl_PositionVector(1), 
//						pl.pl_PositionVector(3), 
//						pl.pl_PositionVector(2),
//						pl.pl_OrientationAngle(1) );
//				}
				_pNetwork->AddMoveList(en);
// EDIT : BS : END
			}
			_pNetwork->SendMoveList();		
		}

		CSkill &SkillData = GetSkillData( nSkillIndex );//0807
		CTString strSysMessage;
		CNetworkMessage nmPlayerSkill(MSG_SKILL);
		// char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count
		if(bFire)
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_FIRE;	

			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
			ClientSystemMessage( strSysMessage );

			UBYTE ubCharType = (UBYTE)MSG_CHAR_PC;		

			// FIXME : 펫을 타고 있는 경우.
			if( _pNetwork->MyCharacterInfo.bPetRide )
			{
				ubCharType = (UBYTE)MSG_CHAR_PET;
			}

			BOOL bFirst = FALSE;
			nmPlayerSkill << ubCharType;

			// FIXME : 펫을 타고 있는 경우.
			if( _pNetwork->MyCharacterInfo.bPetRide )
			{
				nmPlayerSkill << (LONG)_pNetwork->_PetTargetInfo.lIndex;
			}
			else
			{
				nmPlayerSkill << MyCharacterInfo.index;
			}
			nmPlayerSkill << (ULONG)nSkillIndex;						
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en			= *(*it);
				INDEX iIndex		= -1;
				INDEX nTargetType	= 1;

				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();
				
				if(!bFirst)
				{
					bFirst = TRUE;					
					nmPlayerSkill << (unsigned char)nTargetType;
					nmPlayerSkill << (ULONG)iIndex;
					nmPlayerSkill << (SBYTE)(iMobCount - 1);			// 타겟의 갯수.
					continue;
				}
				nmPlayerSkill << (unsigned char)nTargetType;
				nmPlayerSkill << (ULONG)iIndex;
			}
		}
		else
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_READY;
			
			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
			ClientSystemMessage( strSysMessage);
			
			UBYTE ubCharType = (UBYTE)MSG_CHAR_PC;		
			// FIXME : 펫을 타고 있는 경우.
			if( _pNetwork->MyCharacterInfo.bPetRide )
			{
				ubCharType = (UBYTE)MSG_CHAR_PET;
			}			

			nmPlayerSkill << ubCharType;

			// FIXME : 펫을 타고 있는 경우.
			if( _pNetwork->MyCharacterInfo.bPetRide )
			{
				nmPlayerSkill << (LONG)_pNetwork->_PetTargetInfo.lIndex;
			}
			else
			{
				nmPlayerSkill << MyCharacterInfo.index;
			}
			nmPlayerSkill << (ULONG)nSkillIndex;
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en			= *(*it);
				INDEX iIndex		= -1;
				INDEX nTargetType	= 1;
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();
				
				nmPlayerSkill << (unsigned char)nTargetType; //우선은 타겟=NPC만.
				nmPlayerSkill << (ULONG)iIndex;
			}			
		}
		SendToServerNew(nmPlayerSkill);	
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nSkillIndex - 
//			&dcEntities - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSlaveSkillMessageInContainer() 소화수의 스킬 메세지.
void CNetworkLibrary::SendSlaveSkillMessageInContainer(int nSkillIndex, CEntity *pSourceEntity, CSelectedEntities &dcEntities, BOOL bFire)
{
	if( pSourceEntity == NULL )
		return;

	INDEX iMobCount	= dcEntities.Count();

	CPrintF( "Send Skill Attack : %d\n", iMobCount );
	if(_cmiComm. IsNetworkOn() && iMobCount > 0)
	{
		if(_pNetwork->m_bSingleMode)
		{
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en = *(*it);
// EDIT : BS : BEGIN
//				if( en.IsEnemy() || en.IsSlave() || en.IsPet() )
//				{
//					CPlacement3D pl		= en.GetLerpedPlacement();
//					_pNetwork->AddMoveList( 
//						en.GetNetworkID(),
//						pl.pl_PositionVector(1), 
//						pl.pl_PositionVector(3), 
//						pl.pl_PositionVector(2),
//						pl.pl_OrientationAngle(1) );
//				}
				_pNetwork->AddMoveList(en);
// EDIT : BS : END
			}
			_pNetwork->SendMoveList();		
		}

		CSkill &SkillData = GetSkillData( nSkillIndex );//0807
		CTString strSysMessage;
		CNetworkMessage nmPlayerSkill(MSG_SKILL);
		// char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count
		if(bFire)
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_FIRE;	

			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
			ClientSystemMessage( strSysMessage );

			UBYTE ubCharType = (UBYTE)pSourceEntity->GetNetworkType();
			BOOL bFirst = FALSE;
			nmPlayerSkill << ubCharType;
			nmPlayerSkill << pSourceEntity->GetNetworkID();
			
			nmPlayerSkill << (ULONG)nSkillIndex;						
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en			= *(*it);
				INDEX iIndex		= -1;
				INDEX nTargetType	= 1;

				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();
				
				if(!bFirst)
				{
					bFirst = TRUE;					
					nmPlayerSkill << (unsigned char)nTargetType;
					nmPlayerSkill << (ULONG)iIndex;
					nmPlayerSkill << (SBYTE)(iMobCount - 1);			// 타겟의 갯수.
					continue;
				}
				nmPlayerSkill << (unsigned char)nTargetType;
				nmPlayerSkill << (ULONG)iIndex;
			}
		}
		else
		{
			nmPlayerSkill << (UBYTE)MSG_SKILL_READY;
			
			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
			ClientSystemMessage( strSysMessage);
			
			UBYTE ubCharType = (UBYTE)pSourceEntity->GetNetworkType();
			nmPlayerSkill << ubCharType;
			nmPlayerSkill << pSourceEntity->GetNetworkID();			
			nmPlayerSkill << (ULONG)nSkillIndex;
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en			= *(*it);
				INDEX iIndex		= -1;
				INDEX nTargetType	= 1;
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();
				
				nmPlayerSkill << (unsigned char)nTargetType; //우선은 타겟=NPC만.
				nmPlayerSkill << (ULONG)iIndex;
			}			
		}
		SendToServerNew(nmPlayerSkill);	
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendCancelSkillMessage()
{
	CNetworkMessage nmPlayerSkill(MSG_SKILL);

	UBYTE ubCharType = (UBYTE)MSG_CHAR_PC;
	// FIXME : 펫을 타고 있는 경우.
	if( _pNetwork->MyCharacterInfo.bPetRide )
	{
		ubCharType = (UBYTE)MSG_CHAR_PET;
	}
	
	nmPlayerSkill << (UBYTE)MSG_SKILL_CANCEL;
	nmPlayerSkill << ubCharType;
	// FIXME : 펫을 타고 있는 경우.
	if( _pNetwork->MyCharacterInfo.bPetRide )
	{
		nmPlayerSkill << (LONG)_pNetwork->_PetTargetInfo.lIndex;
	}
	else
	{
		nmPlayerSkill << MyCharacterInfo.index;
	}
	SendToServerNew(nmPlayerSkill);	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : plPlacement - 
//			camera_angle - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SetMyPosition(CPlacement3D plPlacement, FLOAT camera_angle)
{
	MyCharacterInfo.x = plPlacement.pl_PositionVector(1);
	MyCharacterInfo.h = plPlacement.pl_PositionVector(2);
	MyCharacterInfo.z = plPlacement.pl_PositionVector(3);
	MyCharacterInfo.r = plPlacement.pl_OrientationAngle(1);
	MyCharacterInfo.camera_angle = camera_angle;
}

void CNetworkLibrary::ClientSystemMessage( CTString &strSysMessage, int nSysType )
{
	_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, nSysType );
}

void CNetworkLibrary::SendRebirthMessage()
{	
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmRebirth(MSG_PC_REBIRTH); 			
		SendToServerNew(nmRebirth);	
	}
}

//wooss 050805
//확장된 리버스 메시지 아이템 사용여부와 부활 장소 
void CNetworkLibrary::SendRebirthMessageEx(int nIndex ,BOOL bUse ,BOOL bHere)
{	
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmRebirth(MSG_PC_REBIRTH);
		nmRebirth << (ULONG)nIndex;
		nmRebirth << (UBYTE)bUse;
		nmRebirth << (UBYTE)bHere;
		SendToServerNew(nmRebirth);	
	}
}

//wooss 050808
void CNetworkLibrary::SendWarpItemMessage(UBYTE nmWarpType, CTString sCharName,BOOL bAllow)
{
	CNetworkMessage nmWarp(MSG_WARP);

	switch(nmWarpType)
	{
		case MSG_WARP_TO_REQ :
			nmWarp << (UBYTE)MSG_WARP_TO_REQ;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TAKE_REQ :
			nmWarp << (UBYTE)MSG_WARP_TAKE_REQ;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TO :
		
			nmWarp << (UBYTE)MSG_WARP_TO;
			nmWarp << sCharName;
			nmWarp << (UBYTE)bAllow;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TAKE :
		
			nmWarp << (UBYTE)MSG_WARP_TAKE;
			nmWarp << sCharName;
			nmWarp << (UBYTE)bAllow;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_CANCEL :
			_pNetwork->SendWarpCancel();
			break;

		case MSG_WARP_TO_REQING :
			nmWarp << (UBYTE)MSG_WARP_TO_REQING;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;
		case MSG_WARP_TAKE_REQING :
			nmWarp << (UBYTE)MSG_WARP_TAKE_REQING;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;
	
	}
}

void CNetworkLibrary::DeleteAllMob()
{
	for( INDEX ipl = 0; ipl < ga_srvServer.srv_amtMob.Count(); ++ipl )
	{
		CMobTarget	&mt = ga_srvServer.srv_amtMob[ipl];
		mt.Init();
	}
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	const int iMaxMobNum = ga_srvServer.srv_iMaxMobNum;
	ga_srvServer.srv_amtMob.Clear();
    ga_srvServer.srv_amtMob.New(iMaxMobNum);
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	for( ipl = 0; ipl < ga_srvServer.srv_actCha.Count(); ++ipl )
	{
		CCharacterTarget	&ct = ga_srvServer.srv_actCha[ipl];
		ct.Init();
	}	

	for( ipl = 0; ipl < ga_srvServer.srv_aitItem.Count(); ++ipl )
	{
		CItemTarget	&it = ga_srvServer.srv_aitItem[ipl];
		it.Init();
	}
	ga_srvServer.ReAllocEntities();		
}

// ----------------------------------------------------------------------------
// Name : SendActionMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendActionMessage(SBYTE action_type, SBYTE action_id, SBYTE state, CEntityPointer epTarget )
{
	if(_cmiComm. IsNetworkOn())
	{		
		CNetworkMessage nmAction(MSG_ACTION); 			
		nmAction << MyCharacterInfo.index;
		nmAction << action_type;
		nmAction << action_id;
		//nmAction << state;

		//물뿌리기 액션일 때는 타겟 인덱스 전달
		if( action_id ==42 && epTarget )
			nmAction << (ULONG)epTarget->GetNetworkID();

		SendToServerNew(nmAction);	
	}
}

// ----------------------------------------------------------------------------
// Name : SendQuestMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendQuestMessage(UBYTE msgQuestType, INDEX data)
{
	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)msgQuestType;
	nmQuest << SLONG(data);

	SendToServerNew(nmQuest);
}

// ----------------------------------------------------------------------------
// Name : SendQuestPrizeMessage()
// Desc : 퀘스트 보상 메시지.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendQuestPrizeMessage(UBYTE msgType, INDEX iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlust)
{
	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PRIZE;
	nmQuest << SLONG(iQuestIndex);
	nmQuest << SLONG(iNpcIndex);
	nmQuest << SLONG(iOptionItemIndex);
	nmQuest << SLONG(iOptionItemPlust);

	SendToServerNew(nmQuest);
}

void CNetworkLibrary::SendUseStatPoint( UBYTE ubStatType )
{
	CNetworkMessage nmStat(MSG_STATPOINT);
	nmStat << (UBYTE)MSG_STATPOINT_USE;
	nmStat << ubStatType;

	SendToServerNew(nmStat);
}

//-----------------------------------------------------------------------------
// Purpose: 일반 스킬 배우기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSkillLearn( SLONG slIndex )
{
	CNetworkMessage nmSkillLearn(MSG_SKILL);
	nmSkillLearn << (UBYTE)MSG_SKILL_LEARN;
	nmSkillLearn << slIndex;

	SendToServerNew(nmSkillLearn);
}

//-----------------------------------------------------------------------------
// Purpose: 특수 스킬 배우기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSSkillLearn( SLONG slIndex )
{
	CNetworkMessage nmSkillLearn(MSG_SSKILL);
	nmSkillLearn << (UBYTE)MSG_SSKILL_LEARN;
	nmSkillLearn << slIndex;

	SendToServerNew(nmSkillLearn);
}

void CNetworkLibrary::SendTeleportWrite( UBYTE sendMSG, UBYTE ubSlot, CTString &strComment )
{
	CNetworkMessage nmMemPos(sendMSG);
	nmMemPos << (UBYTE)MSG_MEMPOS_WRITE;
	nmMemPos << ubSlot;
	nmMemPos << strComment;

	SendToServerNew(nmMemPos);
}

// [KH_070316] 프리미엄 메모리 관련 변경 ( UBYTE ubSlot ) -> ( UBYTE sendMSG, UBYTE ubSlot )
void CNetworkLibrary::SendTeleportMove( UBYTE sendMSG, UBYTE ubSlot )
{
	CNetworkMessage nmMemPos(sendMSG);
	nmMemPos << (UBYTE)MSG_MEMPOS_MOVE;
	nmMemPos << ubSlot;

	SendToServerNew(nmMemPos);
}

void CNetworkLibrary::SendWarpTeleport( int iTeleportIndex )
{
	LONG lTeleportIndex = iTeleportIndex;
	CNetworkMessage nmWarp( MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_TELEPORT;
	nmWarp << lTeleportIndex;
	SendToServerNew( nmWarp );
}

void CNetworkLibrary::SendWarpCancel()
{
	CNetworkMessage nmWarp( MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_CANCEL;

	SendToServerNew( nmWarp );
}

//-----------------------------------------------------------------------------
// Purpose: HP와 MP의 패널티 회복
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendRecoverHPMP( SBYTE sbHPCount, SBYTE sbMPCount )
{
	CNetworkMessage nmRecoverHP( MSG_PK );
	nmRecoverHP << (UBYTE)MSG_PK_RECOVER_HPMP;
	nmRecoverHP << sbHPCount;
	nmRecoverHP << sbMPCount;
	SendToServerNew( nmRecoverHP );
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 봉인 해제
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendRecoverItemSeal( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, SLONG slIndex)
{
	CNetworkMessage nmRecoverItemSeal( MSG_PK );
	nmRecoverItemSeal << (UBYTE)MSG_PK_RECOVER_ITEMSEAL;
	nmRecoverItemSeal << sbTab;
	nmRecoverItemSeal << sbRow;
	nmRecoverItemSeal << sbCol;
	nmRecoverItemSeal << slIndex;
	SendToServerNew( nmRecoverItemSeal );
}

//-----------------------------------------------------------------------------
// Purpose: 이벤트 보상받기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventPrize()
{
	CNetworkMessage nmEventPrize( MSG_EVENT );

	nmEventPrize << (UBYTE)MSG_EVENT_LATTO;
	nmEventPrize << (UBYTE)MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REQ;
	
	SendToServerNew( nmEventPrize );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventTreasureList()
{
	CNetworkMessage nmEvent( MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX;
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_TRY_REQ;
	nmEvent << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew( nmEvent );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventOpenTreasure()
{
	CNetworkMessage nmEvent( MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX;
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_OPEN_REQ;
	nmEvent << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew( nmEvent );
}

//0826
void CNetworkLibrary::PickItemAround()
{
	for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_aitItem.Count(); ++ipl )
	{
		CItemTarget	&it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
		if( it.item_pEntity )
		{
			// If y layer is different
			if( abs( it.item_yLayer - MyCharacterInfo.yLayer ) > 1 )
				continue;

			FLOAT fDistance;
			fDistance = sqrt((it.item_pEntity->en_plPlacement.pl_PositionVector(1)-MyCharacterInfo.x) 
				*(it.item_pEntity->en_plPlacement.pl_PositionVector(1)-MyCharacterInfo.x) 
				+(it.item_pEntity->en_plPlacement.pl_PositionVector(3)-MyCharacterInfo.z) 
				* (it.item_pEntity->en_plPlacement.pl_PositionVector(3)-MyCharacterInfo.z) );
			
			if(fDistance <= 3.0f)
			{
				SendPickMessage( CEntity::GetPlayerEntity(0), it.item_Index, FALSE );
				break;
			}
		}
	}
}

void CNetworkLibrary::SendEventNewyear(int tv_event)
{
	CNetworkMessage nmEvent( MSG_EVENT );

	switch(tv_event)
	{
		case EVENT_NEWYEAR1_REWARD:
			nmEvent << (UBYTE)MSG_EVENT_XMAS_2005;
			_pNetwork->SendToServerNew( nmEvent );
			
			break;
	
		case EVENT_NEWYEAR2_TIMECHECK:
			nmEvent << (UBYTE)MSG_EVENT_NEWYEAR_2006_CHECK;
			_pNetwork->SendToServerNew( nmEvent );

			break;

		case EVENT_NEWYEAR2_REWARD:
			nmEvent << (UBYTE)MSG_EVENT_NEWYEAR_2006_GOODS;
			_pNetwork->SendToServerNew( nmEvent );

			break;
	}
}

// [KH_070413] 스승의날 이벤트 관련 추가
void CNetworkLibrary::SendEventMaster()
{
	CNetworkMessage nmEvent( MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TEACH_2007;
	_pNetwork->SendToServerNew( nmEvent );
}

void CNetworkLibrary::SendFindFriend(int tv_event,void * strInput)
{
	CNetworkMessage nmEvent( MSG_EVENT );
	nmEvent << (UBYTE)tv_event;
	switch(tv_event)
	{
		case MSG_EVENT_SEARCHFRIEND_ADD_CHECK :
			nmEvent << *((CTString *)strInput);
			break;
		case MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK :	
			{
				ULONG tv_idx = *((ULONG*)strInput);
				nmEvent << tv_idx;
			}
			break;
		case MSG_EVENT_SEARCHFRIEND_TIME_CHECK :	
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_CHECK :
			{
				ULONG tv_idx = *((ULONG*)strInput);
				nmEvent << tv_idx;
			}

			break;
	}
	_pNetwork->SendToServerNew( nmEvent );		

}

void CNetworkLibrary::DelMobTarget(ULONG ClientIndex)
{	
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	for(INDEX ipl=0; ipl<ga_srvServer.srv_amtMob.Count(); ++ipl) 
	{
		CMobTarget &mt = ga_srvServer.srv_amtMob[ipl];
		if (mt.mob_iClientIndex == ClientIndex) 
		{	
			//타겟 이펙트 없앰...
			_pUIMgr->StopTargetEffect( mt.mob_Index );

			mt.Init();
			ga_srvServer.srv_amtMob.SwapAndPop(ipl);

			break;
		}
	}	
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

void CNetworkLibrary::DelChaTarget(ULONG ClientIndex)
{
	for(INDEX ipl=0; ipl<ga_srvServer.srv_actCha.Count(); ++ipl) 
	{
		CCharacterTarget &ct = ga_srvServer.srv_actCha[ipl];
		if (ct.cha_iClientIndex == ClientIndex) 
		{	
			//타겟 이펙트 없앰...
			_pUIMgr->StopTargetEffect( ct.cha_Index );

			ct.Init();
			ga_srvServer.srv_actCha.SwapAndPop(ipl);

			break;
		}
	}	
}

void CNetworkLibrary::ResetMobStatus(ULONG ClientIndex)
{
	for(INDEX ipl=0; ipl<ga_srvServer.srv_amtMob.Count(); ++ipl) 
	{
		CMobTarget &mt = ga_srvServer.srv_amtMob[ipl];
		if (mt.mob_iClientIndex == ClientIndex) 
		{
			mt.ResetStatus();
			break;
		}
	}	
}

void CNetworkLibrary::ResetChaStatus(ULONG ClientIndex)
{
	for(INDEX ipl=0; ipl<ga_srvServer.srv_actCha.Count(); ++ipl) 
	{
		CCharacterTarget &ct = ga_srvServer.srv_actCha[ipl];
		if (ct.cha_iClientIndex == ClientIndex) 
		{
			ct.ResetStatus();
			break;
		}
	}	
}

void CNetworkLibrary::SendSkillCancelMessage()
{
	if(_cmiComm. IsNetworkOn())
	{		
		CNetworkMessage nmSkill(MSG_SKILL); 	
		
		nmSkill << (UBYTE)MSG_SKILL_CANCEL;			
		nmSkill << _pNetwork->MyCharacterInfo.index;					
		_pNetwork->SendToServerNew(nmSkill);	
	}
}

// Ranking 관련
//-----------------------------------------------------------------------------
// Purpose: 랭킹 리스트를 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_RequestList( SBYTE sbJob )
{
	SBYTE sbReqJob	= sbJob;
	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_RANK_VIEW_REQ;
	nmQuest << sbReqJob;
	_pNetwork->SendToServerNew(nmQuest);
}

//-----------------------------------------------------------------------------
// Purpose: 보상 리스트를 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_RequestPrizeList()
{
	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_RANK_REWARD_RANK_REQ;	
	_pNetwork->SendToServerNew(nmQuest);
}

//-----------------------------------------------------------------------------
// Purpose: 보상을 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_Prize()
{
	CNetworkMessage nmQuest(MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_REWARD;	
	_pNetwork->SendToServerNew(nmQuest);	
}

//-----------------------------------------------------------------------------
// Purpose: 직업과 해당 무기의 아이템 정보를 갖고서, 무기가 2번째 타입(전직)인지를 체크
//-----------------------------------------------------------------------------
BOOL CNetworkLibrary::IsExtensionState( int iJob, CItemData& ID )
{
	if( ID.GetType() == CItemData::ITEM_WEAPON )
	{
		if( ID.GetSubType() == JobInfo().GetSkillWeponType( iJob, 1 ) )
			return TRUE;		
	}
	return FALSE;
}



//------------------------------------------------------------------------------
// CNetworkLibrary::BillInfoSectionListReq
// Explain:  
// Date : 2005-05-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillInfoSectionListReq()
{
	CNetworkMessage nmBilling( MSG_BILLINFO );
	nmBilling << (UBYTE)MSG_BILLINFO_SECTION_LIST_REQ;
	
	SendToServerNew(nmBilling);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::BillInfoUserInfoReq
// Explain:  
// Date : 2005-05-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillInfoUserInfoReq()		
{
	CNetworkMessage nmBilling( MSG_BILLINFO );
	nmBilling << (UBYTE)MSG_BILLINFO_USERINFO_REQ;
	
	SendToServerNew(nmBilling);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::BillInfoPayReserveReq
// Explain:  
// Date : 2005-05-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillInfoPayReserveReq()
{
	CNetworkMessage nmBilling( MSG_BILLINFO );
	nmBilling << (UBYTE)MSG_BILLINFO_PAY_RESERVE_REQ;
	
	SendToServerNew(nmBilling);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::BillInfoPayOtherReq
// Explain:  
// Date : 2005-05-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillInfoPayOtherReq( CTString strGuid )
{
	CNetworkMessage nmBilling( MSG_BILLINFO );
	nmBilling << (UBYTE)MSG_BILLINFO_PAY_OTHER_REQ;
	nmBilling << strGuid;
	
	SendToServerNew(nmBilling);

}

//------------------------------------------------------------------------------
// CNetworkLibrary::BillItemListReq
// Explain: 유료화 지급 아이템 리스트 요청
// Date : 2005-08-10,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillItemListReq()
{
	CNetworkMessage nmNetMsg( MSG_BILLINFO );
	nmNetMsg << (UBYTE)MSG_BILLINFO_ITEM_LIST_REQ;
	
	SendToServerNew(nmNetMsg);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::BillItemReceiveReq
// Explain:  유료화 지급 아이템 요청( To Inventory )
// Date : 2005-08-10,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::BillItemReceiveReq()
{
	LONG	itemCount;
	
	std::vector<CUIButtonEx> tv_itemInfo;
	std::vector<int> tv_sel;
	CNetworkMessage nmNetMsg( MSG_BILLINFO );

	tv_itemInfo=_pUIMgr->GetBillItem()->GetBtnItemInfo();
	tv_sel=_pUIMgr->GetBillItem()->GetSelectItemInfo();
	
	nmNetMsg << (UBYTE)MSG_BILLINFO_ITEM_REQ;
	itemCount = tv_sel.size();
	nmNetMsg << itemCount;
	for(;itemCount>0;itemCount--)
	{
		nmNetMsg << (LONG)tv_itemInfo[tv_sel[itemCount-1]].GetItemUniIndex();
		nmNetMsg << (LONG)tv_itemInfo[tv_sel[itemCount-1]].GetItemIndex();
		nmNetMsg << (LONG)tv_itemInfo[tv_sel[itemCount-1]].GetItemPlus();
		nmNetMsg << (LONG)tv_itemInfo[tv_sel[itemCount-1]].GetItemFlag();
		nmNetMsg << (LONG)tv_itemInfo[tv_sel[itemCount-1]].GetItemCount();
	}


	SendToServerNew(nmNetMsg);
}

// Messenger...
//------------------------------------------------------------------------------
// CNetworkLibrary::MgrRegistReq
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrRegistReq( int nCharIndex, CTString strCharName )
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_REGIST_REQUEST;
	nmFriend << (ULONG)nCharIndex;
	nmFriend << strCharName;

	SendToServerNew(nmFriend);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrRegistAllow
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrRegistAllow( int nCharIndex, CTString strReqCharName )
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_REGIST_ALLOW;
	nmFriend << (ULONG)nCharIndex;
	nmFriend << strReqCharName;

	
	SendToServerNew(nmFriend);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrSetCondition
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrSetCondition( int nCharIndex, int nCondition )
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_SET_CONDITION;
	nmFriend << (ULONG)nCharIndex;
	nmFriend << (ULONG)nCondition;

	SendToServerNew(nmFriend);
}


void CNetworkLibrary::MgrDeleteMember( int nCharIndex, int nTargetIndex, CTString strName )
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_DELETE_MEMBER;
	nmFriend << (ULONG)nCharIndex;
	nmFriend << (ULONG)nTargetIndex;
	nmFriend << strName;

	SendToServerNew(nmFriend);
}


void CNetworkLibrary::MgrRegistCancel()
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_REGIST_CANCEL;
	
	SendToServerNew(nmFriend);

}

void CNetworkLibrary::MgrRegistCancel(int nCharIndex, CTString strReqName)
{
	CNetworkMessage nmFriend( MSG_FRIEND );
	nmFriend << (UBYTE)MSG_FRIEND_REGIST_CANCEL;
	nmFriend << (LONG)nCharIndex;
	nmFriend << strReqName;
	
	SendToServerNew(nmFriend);

}

void CNetworkLibrary::SendFriendCatting( int nCharIndex, CTString strName, CTString strTargetName, CTString strMessage )
{
	CNetworkMessage nmFriend( MSG_CHAT );
	nmFriend << (UBYTE)MSG_CHAT_MESSENGER;

	nmFriend << (ULONG)nCharIndex;
	nmFriend << strName;
	nmFriend << strTargetName;	
	nmFriend << strMessage;
		
	SendToServerNew(nmFriend);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SetTimeReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SetTimeReq( int nDay, int nHour )
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_SET_TIME_REQ;

	nmGuildWar << (ULONG)nDay;
	nmGuildWar << (ULONG)nHour;
		
	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::GetTimeReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GetTimeReq()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_GET_TIME;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackReq()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_ATTACK_GUILD;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::DefenseGuildReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::DefenseGuildReq()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_DEFENSE_GUILD;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackCharReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackCharReq()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_ATTACK_CHAR;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackGuildReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackGuildReq()
{
	CNetworkMessage nmGuildWar( MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_DEFENSE_CHAR;

	SendToServerNew(nmGuildWar);
}

void CNetworkLibrary::WarItemMixReq( SBYTE* sbRow, SBYTE* sbCol )
{
	CNetworkMessage nmWarItemMix( MSG_ITEM );
	nmWarItemMix << (UBYTE)MSG_ITEM_MIX_WARITEM;

	for( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		nmWarItemMix << (SBYTE)sbRow[i];
		nmWarItemMix << (SBYTE)sbCol[i];
	}
	
	SendToServerNew(nmWarItemMix);
}

//-----------------------------------------------------------------------------
// Purpose: 애완 동물을 호출함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::CallPet( LONG lIndex )
{
	CNetworkMessage nmPet( MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_CALL;
	nmPet << lIndex;
	SendToServerNew(nmPet);
}

//-----------------------------------------------------------------------------
// Purpose: 펫 타입 정보를 얻습니다.
// Input  : 서버에서 오는 것과 클라이언트에서 쓰이던게 달라서... 필요한 부분...
//-----------------------------------------------------------------------------
void CNetworkLibrary::CheckPetType( SBYTE sbPetTypeGrade, INDEX &iPetType, INDEX &iPetAge )
{	
	// MASK를 하드 코딩함...
	iPetType	= ((sbPetTypeGrade & 0xF0) >> 4) - 1;
	iPetAge		= (sbPetTypeGrade & 0x0F) - 1;
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫을 타도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::RidePet( CEntity *pCharacter, CEntity *pPet, INDEX iPetType )
{
	if( !pCharacter /*|| !pPet || !bRide */)
		return;

	LeavePet(pCharacter);

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->RidingPet( pCharacter, iPetType );
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫에서 내리도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::LeavePet( CEntity *pCharacter )
{
	if( !pCharacter )
		return;
	
	if( pCharacter->IsPlayer() )
	{
		// 플레이어의 경우.
		if( _pNetwork->MyCharacterInfo.bPetRide )
		{
			_pUIMgr->GetQuickSlot()->RemovePetSkill();
		}
	}

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->LeavingPet( pCharacter );	
}

//-----------------------------------------------------------------------------
// Purpose: 펫 정보를 갱신합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::UpdatePetTargetInfo( INDEX iPetIndex )
{
	if( !_pNetwork->_PetTargetInfo.bIsActive || _pNetwork->_PetTargetInfo.lIndex != iPetIndex )
		return;

	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= iPetIndex;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != _pNetwork->m_vectorPetList.end() )
	{		
		_pNetwork->_PetTargetInfo.iLevel		= (*iter).lLevel;
		_pNetwork->_PetTargetInfo.fHealth		= (*iter).lHP;
		_pNetwork->_PetTargetInfo.fMaxHealth	= (*iter).lMaxHP;
		_pNetwork->_PetTargetInfo.fMaxHungry	= (*iter).lMaxHungry;
		_pNetwork->_PetTargetInfo.fHungry		= (*iter).lHungry;
		_pNetwork->_PetTargetInfo.lAbility		= (*iter).lAbility;
		_pNetwork->_PetTargetInfo.strNameCard	= (*iter).strNameCard;

		INDEX iPetType	= -1;
		INDEX iPetAge	= -1;
		_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );
		_pNetwork->_PetTargetInfo.iAge			= iPetAge;
		_pNetwork->_PetTargetInfo.iType			= iPetType;

		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
		if( bPetRide )
		{
			// 마운트 상태에서 애완동물의 배고픔이 0이라면 움직이지 못함.
			if( (*iter).lHungry <= 0 )
				_pUIMgr->SetCSFlagOn( CSF_MOUNT_HUNGRY );
			else			
				_pUIMgr->SetCSFlagOff( CSF_MOUNT_HUNGRY );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 펫 정보를 클리어합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::ClearPetList()
{
	if(!_pNetwork->m_vectorPetList.empty())
	{		
		_pNetwork->m_vectorPetList.clear();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::ChangePetMount()
{
	CNetworkMessage nmPet( MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_CHANGE_MOUNT;	
	SendToServerNew(nmPet);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::LearnPetSkill
// Explain:  
// Date : 2005-09-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::LearnPetSkill( LONG nSkillIndex )
{
	CNetworkMessage nmPet( MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_LEARN;
	nmPet << (LONG)nSkillIndex;

	SendToServerNew(nmPet);
}

void CNetworkLibrary::LearnWildPetSkill( LONG nSkillIndex )
{
	CNetworkMessage nmWildPet( MSG_EXTEND );
	
	nmWildPet << (LONG)MSG_EX_ATTACK_PET;
	nmWildPet << (UBYTE)MSG_SUB_SKILLLEAN;
	nmWildPet << (LONG)nSkillIndex;

	SendToServerNew(nmWildPet);
}

// ----------------------------------------------------------------------------
// Name : SendPetChangeRide()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetChangeRide()
{
	CNetworkMessage	nmPet( MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_CHANGE_MOUNT;
		
	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetSkillInit()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetSkillInit()
{
	CNetworkMessage	nmPet( MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_RESET_SKILL;
		
	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetDestruction()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetDestruction()
{
	CNetworkMessage nmPet( MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_CHANGE_ITEM;

	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetItemMix()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetItemMix( SLONG slPetItemIndex, SLONG slmethod )
{
	CNetworkMessage nmPet( MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_MIX_ITEM;   // 조합 메세지
	nmPet << slPetItemIndex;   // 제작 유니크 아이템 인덱스
	nmPet << slmethod;		// 제작 유형 0, 1

	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetItemMix()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetRebirth( SLONG slPetIndex )
{
	CNetworkMessage nmPet( MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_REBIRTH;   // 조합 메세지
	nmPet << slPetIndex;   // 제작 유니크 아이템 인덱스

	SendToServerNew( nmPet );
}

void CNetworkLibrary::SendWildPetRebirth( int nRow, int nCol)
{
	CNetworkMessage nmPet( MSG_EXTEND );
	nmPet << (ULONG)MSG_EX_ATTACK_PET;
	nmPet << (UBYTE)MSG_SUB_REBIRTH;
	nmPet << (INDEX)nRow;
	nmPet << (INDEX)nCol;

	SendToServerNew( nmPet );

}

void CNetworkLibrary::SendPetitemInfo(SLONG slOwnerIndex,int nitemIndex)
{
	CNetworkMessage nmpet(MSG_EXTEND);

	nmpet << (ULONG)MSG_EX_ATTACK_PET;
	nmpet << (UBYTE)MSG_SUB_APET_INFO;
	nmpet << slOwnerIndex;
	nmpet << (INDEX)nitemIndex;

	SendToServerNew(nmpet);
}

// 060306 wooss Party Recall CashItem
//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokExchange
// Explain:  
// Date : 060306 wooss
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRecallConfirm(CTString tmp_str,LONG tmp_idx,BOOL tmp_answer)
{
	// 리콜 확인 메시지 YES/NO
	CNetworkMessage nmItem(MSG_EXTEND); 	
	nmItem << (LONG)MSG_EX_PARTY_RECALL;
	nmItem << (LONG)MSG_EX_PARTY_RECALL_CONFIRM;
	nmItem << tmp_idx;
	nmItem << tmp_str;
	nmItem << (UBYTE)tmp_answer;
	_pNetwork->SendToServerNew(nmItem);
}


void CNetworkLibrary::SendMovingGuildWarArea()
{
	CNetworkMessage nmWarp( MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_PROMPT;
	
	nmWarp << MyCharacterInfo.lWarpZone;
	nmWarp << MyCharacterInfo.lWarpPos;

	SendToServerNew(nmWarp);

	// WSS_DRATAN_SIEGEWARFARE 071011
	if(!_pUIMgr->GetSiegeWarfareNew()->GetWarState())
		_pUISWDoc->SetDealy();
}


bool CNetworkLibrary::IsLord() 
{
	// 내 길드가 성주 길드이고 내가 그 길드의 길드장이면 성주
	if( ( MyCharacterInfo.sbJoinFlagMerac == WCJF_OWNER || MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER) 
		&& ( MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ) )
		return true;
	return false;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashHistroyReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashHistroyReq()
{
	CNetworkMessage nmMessage( MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_HISTORY_REQ;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashViewReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashViewReq()
{
	CNetworkMessage nmMessage( MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_VIEW_REQ;
	
	SendToServerNew(nmMessage);

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashTakeReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashTakeReq( LONGLONG llMoney )
{
	CNetworkMessage nmMessage( MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_TAKE_REQ;
	
	nmMessage << llMoney;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokUpgrade
// Explain:  
// Date : 2005-09-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendChuseokUpgrade()
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_UPGRADE;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokExchange
// Explain:  
// Date : 2005-09-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendChuseokExchange()
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_EXCHANGE;
	
	SendToServerNew(nmMessage);
}

///////////////////////////////////////////////////////////////////////////////////
// 2006 추석 이벤트 :Su-won			|--------------------------------------------->
	
//송편 만들기
void CNetworkLibrary::Send2006ChuseokRicecakeMake()
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE;

	SendToServerNew( nmMessage );
}

//오색 송편 만들기
void CNetworkLibrary::Send2006ChuseokRainbowMake()
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE;

	SendToServerNew( nmMessage );
}

//오색송편 보상품으로 교환하기
void CNetworkLibrary::Send2006ChuseokExchange()
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_GIFT;

	SendToServerNew( nmMessage );
}	
// 2006 추석 이벤트 :Su-won			<---------------------------------------------|
///////////////////////////////////////////////////////////////////////////////////

// 2006 X-Mase event [12/12/2006 Theodoric]
void CNetworkLibrary::Send2006XMasEvent( SLONG nCakeCount) 
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_XMAS_2006;
	nmMessage << nCakeCount;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendCouponItemReq
// Explain:  
// Date : 060118 wooss japan package item
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCouponItemReq(CTString strNum)
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_PACKAGE_ITEM_GIVE;
	nmMessage << strNum;
	
	SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::SendConnectItemReq
// Explain:  
// Date : 060223 wooss japan connect event
//------------------------------------------------------------------------------
void CNetworkLibrary::SendConnectItemReq()
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_OPENBETA_CONN;
		
	SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CNetworkLibrary::SendClothesExchange
// Explain:  
// Date : 060321 eons japan clothes exchange event
//------------------------------------------------------------------------------
void CNetworkLibrary::SendClothesExchange( int nValue )
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_SAKURABUD;
	nmMessage << (UBYTE)nValue;

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::SendRainyDayExchange()
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_RAIN_2006;
	SendToServerNew( nmMessage );
}


void CNetworkLibrary::Send2p4pLetter( LONG lGameIndex )
{
	CNetworkMessage nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_2PAN4PAN_LETTER;
	nmMessage << (LONG)lGameIndex;
	
	SendToServerNew(nmMessage);
}

//-----------------------------------------------------------------------------
// Purpose: 타운으로 귀환합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendPetWarpTown()
{
	CNetworkMessage nmPet( MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_WARPTOWN;	
	SendToServerNew(nmPet);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MandateBossReq
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MandateBossReq( CTString strMandateCharName )
{
	CNetworkMessage	nmMessage( MSG_PARTY );
	nmMessage << (SBYTE)MSG_PARTY_CHANGEBOSS;
	nmMessage << (CTString) strMandateCharName;
		
	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 로그인 메세지를 서버로 보냅니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendLoginMessage(CTString& strID, CTString& strPW, ULONG ulVersion)
{
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 패킷 암호화 : 암호화 키 초기화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG

	// 로그인 서버에 접속 후, 새로운 서버에 접속을 시도합니다.
	CNetworkMessage nmLoginNew(MSG_LOGIN);			// 로그인 메시지 보내기.			
	nmLoginNew << ulVersion;	    
	nmLoginNew << (unsigned char)MSG_LOGIN_NEW;		// 서버 이동
	
	nmLoginNew << strID;
	nmLoginNew << strPW;

	// new Version serialization 060710
	// national code 
	extern INDEX g_iCountry;
	INDEX tv_idx;
	switch(g_iCountry)
	{
	case KOREA :
		tv_idx = 0;
		break;
	case TAIWAN : 
		tv_idx = 1;
		break;
	case CHINA : 
		tv_idx = 3;
		break;
	case THAILAND :
		tv_idx = 4;
		break;
	case TAIWAN2 :
		tv_idx = 2;
		break;
	case JAPAN : 
		tv_idx = 6;
		break;
	case MALAYSIA :
		tv_idx = 7;
		break;
	case USA:
		tv_idx = 9;
		break;
	case BRAZIL:
		tv_idx = 10;
		break;
	case HONGKONG: 
		tv_idx = 11;
		break;
	case GERMANY: 
		tv_idx = 13;
		break;
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
		tv_idx = 14;
		break;
	case FRANCE:
		tv_idx = 15;
		break;
	case POLAND:
		tv_idx = 16;
		break;
	case RUSSIA:
		tv_idx = 17;
		break;
	case TURKEY:
		tv_idx = 18;
		break;
	}

	nmLoginNew << (UBYTE)tv_idx;
	
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	ULONG ulSeed = (ULONG)CNM_MakeSeedForClient(strID, strPW, GetTickCount());
	nmLoginNew << ulSeed;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	SendToServerNew(nmLoginNew, TRUE);
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;

	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_MakeKeyFromSeed(&_pNetwork->cnmKey, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
}

void CNetworkLibrary::SendSecurityMessage(CTString& strSecurity)
{

	// 로그인 서버에 접속 후, 보안 카드 체크 시도합니다.
	CNetworkMessage nmSecurityCardNew(MSG_EXTEND);			// 보안카드 메시지 보내기.			
	nmSecurityCardNew << (LONG)MSG_EX_SECURE_CARD;
	for(int i=0; i<4; i++)
	{
		nmSecurityCardNew << (UBYTE)strSecurity[i];	    
	}
	SendToServerNew(nmSecurityCardNew);
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;
			
}
//-----------------------------------------------------------------------------
// Purpose: 슈퍼고제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSuperStoneReq()
{
	CNetworkMessage msg(MSG_EVENT);
	msg << (SBYTE)MSG_EVENT_SUPERSTONE;
	msg << (SBYTE)MSG_EVENT_SUPERSTONE_REQ;
	SendToServerNew(msg);
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 삭제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendDeleteCharacter(ULONG ulIndex,BYTE del)
{	
	CNetworkMessage nmChaDel(MSG_MENU); //캐릭
	nmChaDel << (unsigned char)MSG_MENU_DEL;
	nmChaDel << ulIndex;
#ifdef NEW_DELETE_CHAR
	nmChaDel << del;
#endif

	SendToServerNew(nmChaDel,TRUE);	
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 삭제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendDeleteCharacter(ULONG ulIndex, CTString& strSecuNum, BYTE del)
{	
	CNetworkMessage nmChaDel(MSG_MENU); //캐릭
	nmChaDel << (unsigned char)MSG_MENU_DEL;
	nmChaDel << ulIndex;
	nmChaDel << strSecuNum;
	nmChaDel << del;
	
		
	SendToServerNew(nmChaDel,TRUE);	
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 게임 시작.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendGameStart(ULONG ulIndex)
{
	/////스타트
	CNetworkMessage nmChaStart(MSG_MENU); 
	nmChaStart << (unsigned char)MSG_MENU_START;
	nmChaStart << ulIndex;
	SendToServerNew(nmChaStart,TRUE);	
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 몬스터 변신 종료.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendStopChange()
{
	CNetworkMessage nmStopChange(MSG_CHANGE); 	
	nmStopChange << (unsigned char)MSG_CHANGE_CANCEL;
	SendToServerNew(nmStopChange);
}

//-----------------------------------------------------------------------------
// Purpose: 강신 종료.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendStopEvocation()
{
	CNetworkMessage nmEvocation(MSG_EXTEND); 	
	nmEvocation << (LONG)MSG_EX_EVOCATION_STOP;
	nmEvocation << _pNetwork->MyCharacterInfo.index;
	SendToServerNew(nmEvocation);
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 생성.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendCreateCharacter( CTString& strName, UBYTE ubJob, UBYTE ubHair, UBYTE ubFace )
{
	CNetworkMessage nmChaNew(MSG_MENU);			//캐릭 생성.
	nmChaNew << (unsigned char)MSG_MENU_NEW;	  	
	nmChaNew << strName;
	nmChaNew << (UBYTE)ubJob;
	nmChaNew << (UBYTE)ubHair;
	nmChaNew << (UBYTE)ubFace;
    _pNetwork->SendToServerNew(nmChaNew,TRUE);
//	   _pNetwork->SendToServerNew(nmChaNew,FALSE);
	_pNetwork->m_uiSendedTime		= timeGetTime();
	_pNetwork->m_bSendMessage		= TRUE;
} 

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lIndex, LONG lEtc )
{
	// 아이템을 통해서 퀘스트를 얻으려 할 경우
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_USE;								
	nmItem << sbTab;
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << lIndex;
	nmItem << lEtc;
	_pNetwork->SendToServerNew(nmItem);
}

//-----------------------------------------------------------------------------
// Purpose: 타겟이 필요한 아이템 사용 메세지.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendtargetItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lIndex, SBYTE sbType, LONG lMobIndex )
{
	// 아이템을 통해서 퀘스트를 얻으려 할 경우
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_TARGET;								
	nmItem << sbTab;
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << lIndex;
	nmItem << sbType;
	nmItem << lMobIndex;
	_pNetwork->SendToServerNew(nmItem);
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지( 합성 )
// Date  :  060523 wooss
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendMixItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lMixItemidx, LONG lCashItemIdx, LONG lgameItemIdx)
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_USE;								
	nmItem << sbTab;
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << lMixItemidx;
	nmItem << lCashItemIdx;
	nmItem << lgameItemIdx;
	_pNetwork->SendToServerNew(nmItem);
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지( 분리 )
// Date  :  060627 wooss
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendUnMixItemUse( SBYTE sbTab, SBYTE sbRow, SBYTE sbCol, LONG lMixItemidx, LONG lCashItemIdx)
{
	CNetworkMessage nmItem(MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_USE;								
	nmItem << sbTab;
	nmItem << sbRow;
	nmItem << sbCol;
	nmItem << lMixItemidx;
	nmItem << lCashItemIdx;
	_pNetwork->SendToServerNew(nmItem);
}


//-----------------------------------------------------------------------------
// Purpose: 소환수 생성.
// Input  : 플레이어의 경우에만 해당하는 것 같음...
//-----------------------------------------------------------------------------
void CNetworkLibrary::CreateSlave( int iOwnerIndex, CEntity* pOwnerEntity, int iType )
{
	if(pOwnerEntity == NULL )
		return;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendEvent24
// Explain: 2pan4pan event message  
// Date : 2005-10-31,Author: wooss
//------------------------------------------------------------------------------

void CNetworkLibrary::SendEvent24(int sel,SBYTE eventType)
{
	CNetworkMessage	nmMessage( MSG_EVENT);
	
	switch(eventType)
	{
	case MSG_EVENT_2PAN4PAN_GOODS_CHECK:
		nmMessage << (UBYTE)eventType;	
		break;
	case MSG_EVENT_2PAN4PAN_GOODS_REQ:
		nmMessage << (UBYTE)eventType;
		break;
	case MSG_EVENT_2PAN4PAN_MONEY_REQ:
		nmMessage << (UBYTE)eventType;
		nmMessage << (ULONG)sel;
		break;
	case MSG_EVENT_2PAN4PAN_BOX_CHECK:
		nmMessage << (UBYTE)eventType;
		break;
	case MSG_EVENT_2PAN4PAN_BOX_REQ:
		nmMessage << (UBYTE)eventType;
		break;
	}
	
	SendToServerNew( nmMessage );


//	MSG_EVENT_2PAN4PAN_GOODS_CHECK,				// 2판4판 리뉴얼 아이템 리스트 요청 : count(n:server) wooss 051031
//	MSG_EVENT_2PAN4PAN_GOODS_REQ,				// 2판4판 리뉴얼 아이템 받기 요청 :
//	MSG_EVENT_2PAN4PAN_GOODS_REP,				// 2판4판 리뉴얼 아이템 받기 응답 : MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE(n) count(n) [itemdbindex(n) itemcount(ll)]*count

//	MSG_EVENT_2PAN4PAN_MONEY_REQ,				// 2판4판 머니 카드 교환 요청 : type(n)
												// type : 1 - 맞고, 2 - 포커
//	MSG_EVENT_2PAN4PAN_MONEY_REP,				// 2판4판 머니 카드 교환 응답 : success(n)
												// success : 0 - 실패, 1 - 성공

//	MSG_EVENT_2PAN4PAN_BOX_CHECK,				// 2판4판 상자 교환 검사	: flag(n:server)
												// flag : 0 - 없음, 1 - 있음
//	MSG_EVENT_2PAN4PAN_BOX_REQ,					// 2판4판 상자 교환 요청	: 
//	MSG_EVENT_2PAN4PAN_BOX_REP,					// 2판4판 상자 교환 응답	: MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE(n) count(n)  [itemdbindex(n) itemcount(ll)]*count

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendCastleMapRecent
// Explain:  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCastleMapRecent()
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	nmMessage << (LONG)MSG_EX_CASTLE_MAP_RECENT;
			
	SendToServerNew( nmMessage );

}


//------------------------------------------------------------------------------
// CNetworkLibrary::SendCastleMapSignal
// Explain:  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCastleMapSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex )
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	nmMessage << (LONG)MSG_EX_CASTLE_MAP_SIGNAL;

	nmMessage << (FLOAT)fX;
	nmMessage << (FLOAT)fY;
	nmMessage << (LONG)lSenderFlag;
	nmMessage << (LONG)lSenderIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGameStart
// Explain: 로딩이 완료되면 서버에 MSG_START_GAME를 보낸다.
// * 현재 로딩되고 있는 zone 번호는 초기화
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGameStart()
{
	SetLoadingZone( -1 );
	
	CNetworkMessage nmStartGame(MSG_START_GAME);
	_pNetwork->SendToServerNew(nmStartGame,TRUE);	
	
	_cmiComm.Server_Update();
}


//------------------------------------------------------------------------------
// CNetworkLibrary::SetLoadingZone
// Explain: 서버에서 MSG_DB_OK메세지를 받으면 존번호를 저장하여 
// 존 로딩이 완료 된 후에 MSG_START_GAME를 보낸다.
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SetLoadingZone( LONG lZone )
{
	m_lLoadingZone = lZone;
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GetLoadingZone
// Explain: 로딩되고 있는 존의 번호를 얻어 온다.
// * 로딩을 하고 있지 않을때는 -1
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
LONG CNetworkLibrary::GetLoadingZone()
{
	return m_lLoadingZone;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::GetLoadingZone
// Explain: 로딩되고 있는 존의 번호를 얻어 온다.
// * 로딩을 하고 있지 않을때는 -1
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendNetCafeOpenBox()
{
	CNetworkMessage	nmMessage( MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_PCBANG;
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistNormal
// Explain: 파티 매칭 일반 등록 요청
// Date : 2006-05-09(오후 5:14:07), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistNormal( int nType )
{
	CNetworkMessage		nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_REG_MEMBER_REQ;

	nmMessage << (UBYTE)nType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistParty
// Explain: 파티 매칭 파티 등록 요청
// Date : 2006-05-09(오후 5:14:07), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistParty( CTString strComment, DWORD JobFlag, int LimitLevel )
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_REG_PARTY_REQ;
	nmMessage << strComment;
	nmMessage << (ULONG)JobFlag;
	nmMessage << (SBYTE)LimitLevel;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistParty
// Explain: 파티 매칭 파티 등록 요청
// Date : 2006-05-13(오후 1:22:33), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyListReq( int nPage, int nJob, int nLimitLv, int nPtType, BOOL bState )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;

	if( bState ) // 파티 팀 리스트 요청
	{
		nmMessage << (ULONG)MSG_EX_PARTY_MATCH_PARTY_LIST_REQ;
	}
	else// 파티 멤버 리스트 요청
	{
		nmMessage << (ULONG)MSG_EX_PARTY_MATCH_MEMBER_LIST_REQ;
	}

	nmMessage << (SLONG)nPage;
	nmMessage << (SBYTE)nJob;
	nmMessage << (SBYTE)nLimitLv;
	nmMessage << (SBYTE)nPtType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistDelete
// Explain: 파티 매칭 파티 등록 삭제 요청
// Date : 2006-05-15(오전 9:54:51), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistDelete()
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_DEL_REQ;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyInviteReq
// Explain: 파티 매칭 일반 초대 요청
// Date : 2006-05-15(오전 9:54:51), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyInviteReq( int nCharIndex )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_INVITE_REQ;
	nmMessage << (SLONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinReq
// Explain: 파티 매칭 파티 조인 요청
// Date : 2006-05-15(오후 1:39:55), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinReq( int nBossIndex )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_REQ;
	nmMessage << (SLONG)nBossIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinAllow
// Explain: 파티 매칭 파티 조인 허락
// Date : 2006-05-15(오후 1:28:11), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinAllow( int nReqIndex )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_ALLOW;
	nmMessage << (SLONG)nReqIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinReject
// Explain: 파티 매칭 파티 조인 거절
// Date : 2006-05-15(오후 1:28:11), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinReject( int nReqIndex )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_REJECT;
	nmMessage << (SLONG)nReqIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendWorldCupEvent
// Explain: WorldCup Event 1
// Date : 2006-06-01, wooss
//------------------------------------------------------------------------------
void CNetworkLibrary::SendWorldCupEvent( int msgCmd, int iSelIdx )
{
	CNetworkMessage	nmMessage( MSG_EVENT );
	
	switch(msgCmd)
	{
		case MSGCMD_WORLDCUP_EVENT1_EXCHANGE :
		{
			nmMessage << (UBYTE)MSG_EVENT_WORLDCUP;
			nmMessage << (LONG)MSG_EVENT_WORLDCUP_VOTE;
			nmMessage << (LONG)iSelIdx;
		}
		break;
		case MSGCMD_WORLDCUP_EVENT1_RECEIVE :
		{
			nmMessage << (UBYTE)MSG_EVENT_WORLDCUP;
			nmMessage << (LONG)MSG_EVENT_WORLDCUP_GIFT;
			nmMessage << (LONG)iSelIdx;
		}
		break;

		
	}
	
	SendToServerNew( nmMessage );

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendWorldCupGoldenBallEvent
// Explain: Golden ball event
// Date : 2006-06-01(오후 8:05:27), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendWorldCupGoldenBallEvent( int nSendType, int nTeamAScore, int nTeamBScore )
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_GOLDENBALL;
	nmMessage << (LONG)nSendType;

	if( nSendType == MSG_EVENT_GOLDENBALL_VOTE )
	{
		nmMessage << (LONG)nTeamAScore;
		nmMessage << (LONG)nTeamBScore;
	}

	SendToServerNew( nmMessage );
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrOneVsOneConnect
// Explain: OneOnOneChatMessenger
// Date : 2006-12-11(오후 8:05:27), By KwonYongDae
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrOneVsOneConnect( int nCharIndex, int nOtherCharIndex, CTString strChat )
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_ONE_VS_ONE;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nOtherCharIndex;
	nmMessage << strChat;

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendBuddhismEvent
// Explain : Buddhism event
// Date : 2006-06-27(오전 10:44:09), By eons
//============================================================================================================
void CNetworkLibrary::SendBuddhismEvent( BYTE nType )
{
	CNetworkMessage nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_TLD_BUDDHIST;
	nmMessage << (BYTE)nType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendInvite
// Explain: 친구를 대화에 초대한다
// nCharIndex - 채팅창 생성 캐릭 인덱스, ChatIndex -채팅창 인덱스, strTargetName -초대할 캐릭 이름
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendInvite( int nCharIndex, int nChatIndex, int nTargetIndex)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_INVITE;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	nmMessage << (LONG)nTargetIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendOut
// Explain: 친구가 대화창에서 나감.
// nCharIndex - 채팅창 생성 캐릭 인덱스, nChatIndex -채팅창 인덱스
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendOut( int nCharIndex, int nChatIndex )
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_OUT;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendChat
// Explain: 친구가 대화창에서 나감.
// nCharIndex - 채팅창 생성 캐릭 인덱스, nChatIndex -채팅창 인덱스, strChat -채팅 내용
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendChat( int nCharIndex, int nChatIndex, CTString strChat)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_CHAT;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	nmMessage << strChat;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrBlock
// Explain: 친구를 차단시킴.
// strTarget -차단대상 캐릭 이름
// Date : 2006-6-2 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrBlock( CTString strTarget)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );

	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_BLOCK
			  << strTarget;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrUnBlock
// Explain: 차단된 캐릭을 해제함.
// strTarget -차단대상 캐릭 이름
// Date : 2006-6-9 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrUnBlock( int nCharIndex )
{
	CNetworkMessage	nmMessage( MSG_EXTEND );

	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_RELEASE_BLOCK
			  << (LONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupAdd
// Explain: 메신저에 그룹을 추가.
// strName -추가할 그룹 이름
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupAdd( CTString strName)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_GROUP_ADD
			  << strName;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupDel
// Explain: 메신저에 그룹을 삭제함.
// nGroupIndex -삭제할 그룹 인덱스
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupDel( int nGroupIndex)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_DEL;
	nmMessage << (LONG)nGroupIndex;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupRename
// Explain: 메신저에 그룹 이름을 변경함.
// nGroupIndex -변경할 그룹 인덱스, strNewName -변경할 이름.
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupRename( int nGroupIndex, CTString strNewName)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_CHANGE;
	nmMessage << (LONG)nGroupIndex;
	nmMessage << strNewName;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupMove
// Explain: 메신저에 친구의 그룹을 변경함.
// nGroupIndex -변경할 그룹 인덱스, strCharName -그룹을 이동할 캐릭 이름.
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupMove( int nGroupIndex, int nCharIndex)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_MOVE;
	nmMessage << (LONG)nGroupIndex;
	nmMessage << (LONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrSetChatColor
// Explain: 채팅글의 색깔 설정
// colChat -변경할 색
// Date : 2006-6-7 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrSetChatColor( int nColIndex)
{
	CNetworkMessage	nmMessage( MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_CHATCOLOR_CHAGNE;
	nmMessage << (LONG)nColIndex;

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendBuyCollectBox
// Explain : 곤충 채집 상자 구입
// Date : 2006-06-28(오후 4:21:15), By eons
//============================================================================================================
void CNetworkLibrary::SendBuyCollectBox( void )
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_BUY_BOX_REQ;

	SendToServerNew( nmMessage );	
}

//============================================================================================================
// CNetworkLibrary::SendGiftCollectBox
// Explain : 곤충 채집 상자 보상 요청
// Date : 2006-06-28(오후 4:23:19), By eons
//============================================================================================================
void CNetworkLibrary::SendGiftCollectBox( void )
{
	CNetworkMessage	nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_GIFT_REQ;

	SendToServerNew( nmMessage );	
}

//============================================================================================================
// CNetworkLibrary::SendDropInsect
// Explain : 곤충 아이템 드롭
// Date : 2006-06-28(오후 4:24:31), By eons
//============================================================================================================
void CNetworkLibrary::SendDropInsect( int nInsect )
{
	CNetworkMessage nmMessage( MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_DROP;
	nmMessage << (LONG)nInsect;

	SendToServerNew( nmMessage );
}


//---------------------------------------------------------------------->>
// Name : SetCheckPointData
// Desc : for s-dungeon check point (use touch field)
// Date : [7/7/2006] , Wooss
//----------------------------------------------------------------------<<
void CNetworkLibrary::SetCheckPointData(int questIdx,int chkNumFlag, int maxChkflag)
{
	if(questIdx != -1)
		wo_stCheckPoint.m_iQuestIndex	= questIdx;
	if(chkNumFlag != -1)
		wo_stCheckPoint.m_iCheckFlag	|= chkNumFlag;
	if(maxChkflag != -1)
		wo_stCheckPoint.m_iMaxCheckFlag = maxChkflag;
	
}

//==============================================================================================
// CNetworkLibrary::SendUsaOpenBetaGift
// Expalin : 미국 오픈 베타 선물 지급( 7일간 )
// Date : [11/16/2006] , eons
//==============================================================================================
void CNetworkLibrary::SendUsaOpenBetaGift(void)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_OPEN_BETA_GIFT;

	SendToServerNew(nmMessage);
}

//=============================================================================================
// CNetworkLibrary::SendSaveBingoItem
// Explain : 러브러브 이벤트(초코상자 아이템 저장 요청)
//
//=============================================================================================
void CNetworkLibrary::SendSaveBingoItem(BYTE nPos, int nGiftItemIndex, int nBoxItemIndex)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_VALENTINE_2007;
	nmMessage << (ULONG)MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE;
	nmMessage << (BYTE)nPos;
	nmMessage << (LONG)nGiftItemIndex;
	nmMessage << (LONG)nBoxItemIndex;

	SendToServerNew(nmMessage);
}

//=============================================================================================
// CNetworkLibrary::SendSaveBingoItem
// Explain : 러브러브 이벤트(초코상자 아이템 저장 요청)
//
//=============================================================================================
void CNetworkLibrary::SendReqGiftChocoBox(int nUniItemIndex)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_VALENTINE_2007;
	nmMessage << (ULONG)MSG_EVENT_VALENTINE_2007_BINGO_GIFT;
	nmMessage << (LONG)nUniItemIndex;

	SendToServerNew(nmMessage);
}

// wooss 070307 ---------------------------------------->>
// kw : WSS_WHITEDAY_2007 
void CNetworkLibrary::SendWhiteday2007(UBYTE ubType)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_WHITEDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ;
	nmMessage << (UBYTE)ubType;

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendWhiteday2007Letter(int itemIdx,COLOR colMsg,CTString tStr)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_WHITEDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_WHITEDAY_2007_LETTER_REQ;
	nmMessage << (ULONG)itemIdx;
	nmMessage << (ULONG)colMsg;
	nmMessage << tStr;

	SendToServerNew(nmMessage);
}
// -----------------------------------------------------<<

// WSS_GUILDMASTER 070416 ----------------------------------------->>

// 길드포인트 리스트 요청 
void CNetworkLibrary::SendRequestParentsday(int reqIdx)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PARENTSDAY_2007;
	nmMessage << (ULONG)reqIdx;
	
	SendToServerNew(nmMessage);
}

// 길드 포인트 등록
void CNetworkLibrary::SendParentsdayAddPoint(int charIdx)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PARENTSDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_PARENTSDAY_2007_ADD_POINT;
	nmMessage << (ULONG)charIdx;
		
	SendToServerNew(nmMessage);
}
// ----------------------------------------------------------------<<	
#ifndef NO_GAMEGUARD
// WSS_NPROTECT 070402 --------------------------------->>
void CNetworkLibrary::SendnProtectAuth2(DWORD dwArg)
{
	GG_AUTH_DATA m_AuthData;
	int msgLen = sizeof(GG_AUTH_DATA);
	memcpy(&m_AuthData,(PVOID)dwArg,msgLen);
	CNetworkMessage nmItem(MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_NPROTECT;
	nmItem << m_AuthData.dwIndex;
	nmItem << m_AuthData.dwValue1;
	nmItem << m_AuthData.dwValue2;
	nmItem << m_AuthData.dwValue3;			
	_pNetwork->SendToServerNew(nmItem);

//	CPrintF("Send Index  : %x\n", m_AuthData.dwIndex);
// 	CPrintF("Send Value1 : %x\n", m_AuthData.dwValue1);
// 	CPrintF("Send Value2 : %x\n", m_AuthData.dwValue2);
//	CPrintF("Send Value3 : %x\n", m_AuthData.dwValue3);
}

BOOL CNetworkLibrary::Init_nProtect()
{	
	// npgl 초기화 에러 메시지 출력
	DWORD dwResult = npgl.Init();
	if ( dwResult != NPGAMEMON_SUCCESS )
	{
		CTString msg,errStr;
		switch(dwResult)
		{
			// TODO : 메시지 출력(p18)
		case NPGAMEMON_ERROR_EXIST:
			errStr = _S(3321,"게임가드가 실행 중입니다. 잠시 후나 재부팅 후에 다시 실행해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			errStr = _S(3322,"게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해 보시기 바랍니다,");
			break;
		case NPGAMEMON_ERROR_INIT:
			errStr = _S(3323,"게임가드 초기화 에러입니다. 재부팅 후 다시 실행해 보거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:			
		case NPGAMEMON_ERROR_AUTH_INI:		
		case NPGAMEMON_ERROR_NFOUND_INI:
			errStr = _S(3324,"게임가드 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치해보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			errStr = _S(3325,"윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			errStr = _S(3326,"게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			errStr = _S(3327,"불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후 다시 실행해 보시기 바랍니다.");
//			break;
		case NPGMUP_ERROR_ABORT:
			errStr = _S(3328,"게임가드 업데이트를 취소하셨습니다. 접속이 계속되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGMUP_ERROR_CONNECT:			
		case HOOK_TIMEOUT:
			errStr = _S(3329,"바이러스나 스파이웨어로 인해 후킹이 실패하였습니다. 최신백신을 받으신 후 컴퓨터 전체검사를 해 주십시오");
			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			errStr = _S(3330,"게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업 파일을 다시 설치하고 게임을 실행해 주십시오");
			break;
		case NPGMUP_ERROR_PARAM:
			errStr = _S(3331,"ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하면 해결할 수 있습니다.");
			break;
		case NPGMUP_ERROR_INIT:
			errStr = _S(3332,"npgmup.des 초기화 에러입니다. 게임가드 폴더를 삭제후 다시 게임 실행해 주십시오.");
			break;
		case NPGMUP_ERROR_DOWNCFG:
			errStr = _S(3333,"게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 재시도 해보거나, 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGMUP_ERROR_AUTH:
			errStr = _S(3334,"게임가드 업데이트를 완료하지 못 했습니다. 바이러스 백신을 일시 중지시킨 후 재시도 해보시거나, PC관리 프로그램을 사용하시면 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			errStr = _S(3335,"바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.");			
			break;
		default:
			errStr = _S(3336,"게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard폴더에 있는 *erl 파일을 버그레포팅시 첨부하여 주시기 바랍니다.");
			break;			
		}
		msg.PrintF(_S(3342,"게임가드 에러 : %lu"),dwResult);
		MessageBox(NULL,errStr.str_String,msg,MB_OK);
		
		// 게임 종료
		return FALSE;
	}	

	return TRUE;
}


BOOL CNetworkLibrary::NPGameMonCallbackErrChk(DWORD dwMsg, DWORD dwArg)
{
	BOOL tAppExit = FALSE;
	extern ENGINE_API CTString g_szHackMsg;

	switch(dwMsg)
	{
		// TODO : 메시지 처리 (p21)		
		case NPGAMEMON_COMM_ERROR :
		case NPGAMEMON_COMM_CLOSE :
			tAppExit = TRUE;
			break;
		case NPGAMEMON_INIT_ERROR :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3337,"게임가드 초기화 에러 : %lu"), dwArg);
			break;
		case NPGAMEMON_SPEEDHACK :
			tAppExit = TRUE;
			g_szHackMsg = _S(3338,"스피드핵이 감지되었습니다.");
			break;						
		case NPGAMEMON_GAMEHACK_KILLED :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3339,"게임핵이 발견되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_GAMEHACK_DETECT :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3340,"게임핵이 발견되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_GAMEHACK_DOUBT :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3341,"게임이나 게임가드가 변조되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_CHECK_CSAUTH2 :
			SendnProtectAuth2(dwArg);
			break;
	}

	return tAppExit;
}

void CNetworkLibrary::SetHwnd_nProtect(HWND _hwndMain)
{
	npgl.SetHwnd(_hwndMain);
}

void CNetworkLibrary::Check_nProtect()
{
	if( npgl.Check() != NPGAMEMON_SUCCESS )
	{
		// 게임 종료
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= FALSE;
	}
}

#endif
// -----------------------------------------------------<<

// WSS_MINIGAME 070420 -------------------------------------------->>

void CNetworkLibrary::SendMinigameDefaultMessage(UBYTE subType)
{
	// MSG_EVENT_GOMDORI_2007_CHECKITEM
	// MSG_EVENT_GOMDORI_2007_START
	// MSG_EVENT_GOMDORI_2007_CONTINUE
	// MSG_EVENT_GOMDORI_2007_END
	// MSG_EVENT_GOMDORI_2007_VIEW_STATUS

	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007;	
	nmMessage << (UBYTE)subType;
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendMinigameSelect(UBYTE cSelect)
{
	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007;	
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007_SELECT;
	nmMessage << (UBYTE)cSelect;
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendMinigameSelectGift(ULONG cSelect)
{
	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007;	
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007_SELECT_GIFT;
	nmMessage << (ULONG)cSelect;
	SendToServerNew(nmMessage);
}

// ----------------------------------------------------------------<<

/************************************************************************/
/* Name : SendFlowerTreeSupport                                         */
/************************************************************************/
void CNetworkLibrary::SendFlowerTreeReq(UBYTE subType)
{
	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_FLOWERTREE_2007;
	nmMessage << subType;
	SendToServerNew(nmMessage);
}

// [070613: Su-won] 
// 무기,방어구 교환 카드 사용 요청
void CNetworkLibrary::UseChangeWeaponItem( SBYTE sbRow, SBYTE sbCol, LONG lItemIndex, LONG lChangeType )
{
	CNetworkMessage nmMessage(MSG_ITEM);
	nmMessage << (SBYTE)MSG_ITEM_EXCHANGE_EQUIP_REQ;

	int tv_tab,tv_row,tv_col;
	_pUIMgr->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_row,tv_col);
	
	CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_row][tv_col];

	if(Items.Item_Index ==-1) //비어있는 슬롯이다.
		return;
	
	nmMessage << sbRow;
	nmMessage << sbCol;
	nmMessage << lItemIndex;
	nmMessage << lChangeType;
	nmMessage << Items.Item_Row;
	nmMessage << Items.Item_Col;
	nmMessage << Items.Item_UniIndex;
	SendToServerNew(nmMessage);
}

// [070613: Su-won] 
// 펫 명찰 아이템 사용 요청
void CNetworkLibrary::SendPetNameChageReq( int nPetIndex, CTString strPetName )
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PET_CHANGE_NAME;
	
	nmMessage << (SLONG)nPetIndex;
	nmMessage << strPetName;

	SendToServerNew( nmMessage );
}

// [070705: Su-won] EVENT_SUMMBER_2007
// 2007 여름 이벤트: 종이 접기 요청
void CNetworkLibrary::SendPaperFoldingReq(int nItemIndex0, int nItemIndex1, int nItemIndex2)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007;
	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES;
	nmMessage << (LONG)nItemIndex0;
	nmMessage << (LONG)nItemIndex1;
	nmMessage << (LONG)nItemIndex2;

	SendToServerNew(nmMessage);
}

// [070705: Su-won] EVENT_SUMMBER_2007
// 2007 여름 이벤트: 색종이 조합 요청
void CNetworkLibrary::SendPaperBlendingReq(int nItemIndex)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007;
	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007_INCHEN;
	nmMessage << (LONG)nItemIndex;
	
	SendToServerNew(nmMessage);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [070807: Su-won] EVENT_ADULT_OPEN
// 성인서버 오픈 이벤트 관련 보상 요청
//
// Parameter:
// iEvent - 이벤트 번호( 0:매직카드 이벤트, 1:전직 이벤트, 2:연금술 이벤트)
// tv_tab, tv_row, tv_col - 연금술에 사용할 장비의 인벤 위치. 연금술 이벤트에만 사용
//
void CNetworkLibrary::SendAdultOpenEventReq( int iEvent, int tv_tab, int tv_row, int tv_col)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER;
	
	switch( iEvent )
	{
		//매직카드 이벤트
	case 0:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_REQ;
		}
		break;
		//전직 이벤트
	case 1:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE;
		}
		break;
		//연금술 이벤트
	case 2:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_REQ;

			nmMessage << (LONG)tv_tab;
			nmMessage << (LONG)tv_row;
			nmMessage << (LONG)tv_col;
		}
		break;
	}

	SendToServerNew(nmMessage);
}
// [070807: Su-won] EVENT_ADULT_OPEN
////////////////////////////////////////////////////////////////////////////////////////////

// WSS_TG2007 2007/09/14 ------------------------------------------------------------>>
void CNetworkLibrary::SendTG2007ScreenshotReq()
{
	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_SSHOT_2007;
	nmMessage << (UBYTE)MSG_EVENT_SSHOT_2007_RECIVE;	
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendTG2007RichYearReq(int subType)
{
	CNetworkMessage nmMessage(MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_RICHYEAR;
	nmMessage << (UBYTE)subType;	
	SendToServerNew(nmMessage);
}
// ----------------------------------------------------------------------------------<<
//============================================================================================================
// CNetworkLibrary::SendBJMono2007Req
// SK BJ mono Event
//============================================================================================================
void CNetworkLibrary::SendBJMono2007Req(UBYTE subType, ULONG ulNum)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_BJMONO_2007;
	nmMessage << subType;

	if (subType==MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ)
	{
		nmMessage << ulNum;
	}

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendLC1000DayCapUpgradeReq
// 
//============================================================================================================
void CNetworkLibrary::SendLC1000DayHatUpgradeReq(SBYTE sbRow, SBYTE sbCol, LONG lItemIndex)
{
	CNetworkMessage nmMessage(MSG_EVENT);
	
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_HAT_UPDATE;
	nmMessage << sbRow << sbCol;
	nmMessage << lItemIndex;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendLC1000DayTakeHatReq
//
//============================================================================================================
void CNetworkLibrary::SendLC1000DayTakeHatReq()
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_HAT;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTakeFireCracker
//
//============================================================================================================
void CNetworkLibrary::SendTakeFireCracker()
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_FIRECRACKER;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendHalloween2007Req
//
//============================================================================================================
void CNetworkLibrary::SendHalloween2007Req(UBYTE subType)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_HALLOWEEN_2007;
	nmMessage << (UBYTE)subType;

	SendToServerNew(nmMessage);
}

//[071123: Su-won] DRATAN_SIEGE_DUNGEON
void CNetworkLibrary::SendDratanSiegeDungeonMSG(UBYTE ubSubType, UBYTE ubSubSubType, int nValue)
{
	CNetworkMessage nmMessage( MSG_EXTEND );
	nmMessage<< (LONG)MSG_EX_DVD;
	nmMessage<< ubSubType;
	if( ubSubType == MSG_MANAGEMENT || ubSubType == MSG_DUNGEON_ENTER)
	{
		nmMessage<< ubSubSubType;

		if( ubSubSubType == MSG_MANAGEMENT_MANAGER_ENV_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_TAX_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE ) 
		{
			nmMessage<< (LONG)nValue;
		}
	}
	
	SendToServerNew(nmMessage);
}

// ttos : 판매대행 상인
void CNetworkLibrary::SendCashPersonShop(UBYTE subType)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	
	nmMessage << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmMessage << (UBYTE)subType;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendXMAS2007DecoReq
//
//============================================================================================================
void CNetworkLibrary::SendXMAS2007DecoReq(UBYTE subType)
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_XMAS_2007;
	nmMessage << subType;

	SendToServerNew(nmMessage);
}

#ifdef RESTART_GAME
//============================================================================================================
// CNetworkLibrary::SendRestartGame
// 웹 실행버전으로 인한 재시작 요청 메세지
//============================================================================================================
void CNetworkLibrary::SendRestartGame()
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_RESTART;

	SendToServerNew(nmMessage);
}
#endif

//============================================================================================================
// CNetworkLibrary::SendUseGoDungeon
// 던전이동 주문서 사용
//============================================================================================================
void CNetworkLibrary::SendUseGoDungeon(INDEX iItemIndex, INDEX iZone, INDEX iExtra)
{
	CNetworkMessage nmMessage(MSG_ITEM);

	nmMessage << (UBYTE)MSG_ITEM_USE_WARPDOC;
	nmMessage << (ULONG)iItemIndex;
	nmMessage << (ULONG)iZone;
	nmMessage << (ULONG)iExtra;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendPresscorpsMessage
// 라카기자단 혜택 관련 (확성기 기능 메세지 전송) 외치기!
//============================================================================================================
void CNetworkLibrary::SendPresscorpsMessage(ULONG ulItemIndex, CTString Sendstr)
{
	if(Sendstr.Length() == 0)
	{
		return;
	}
	CNetworkMessage nmMessage(MSG_ITEM); 	
	nmMessage << (SBYTE)MSG_ITEM_USE_PRESSCORPS;
	nmMessage << ulItemIndex;
	
	CTString tmpString = _pNetwork->MyCharacterInfo.name + ": " + Sendstr;

	nmMessage << tmpString;

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendUpdatePlayTime
// 플레이 시간 10분간격으로 업데이트 메세지 전달
//============================================================================================================
void CNetworkLibrary::SendUpdatePlayTime(ULONG nTime)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_UPDATE_PLAYTIME;
	nmMessage << (ULONG)nTime;

	_pNetwork->SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendItemDelete(SBYTE iTab, SBYTE iRow, SBYTE iCol, INDEX UniIndex, SQUAD count)
{
	CNetworkMessage nmMessage(MSG_ITEM);

	nmMessage << (SBYTE)MSG_ITEM_DELETE;
	nmMessage << iTab << iRow << iCol;
	nmMessage << UniIndex;
	nmMessage << count;

	_pNetwork->SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendUseWildPetItem(int posId, int tabId, int rowId, int colId, int item_index)
{
	
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_ITEM_WEAR;
	nmMessage << (SBYTE)posId;
	nmMessage << (SBYTE)tabId;
	nmMessage << (SBYTE)rowId;
	nmMessage << (SBYTE)colId;
	nmMessage << (ULONG)item_index;

	_pNetwork->SendToServerNew(nmMessage);
}

//////////////////////////////////////////////////////////////////////////
// 피닉스 캐릭터 가능 여부 확인

void CNetworkLibrary::SendPhoenixCharacterCondition()
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PHOENIX;

	_pNetwork->SendToServerNew(nmMessage);
}

//////////////////////////////////////////////////////////////////////////
// 피닉스 캐릭터 신청

void CNetworkLibrary::SendCreatePhoenixCharacter()
{
	CNetworkMessage nmMessage(MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PHOENIX_SUCCESS;

	_pNetwork->SendToServerNew(nmMessage);
}


//////////////////////////////////////////////////////////////////////////
//============================================================================================================
// CNetworkLibrary::SendInitSSkillReq
// 스페셜 스킬 초기화 요청
//============================================================================================================
void CNetworkLibrary::SendInitSSkillReq()
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_INIT_SSKILL;
	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendUsingSpeedHack
// 스피드 핵 사용자 서버에 통보
//============================================================================================================
void CNetworkLibrary::SendUsingSpeedHack()
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_DISCONNECT_HACK_CHARACTER;
	nmMessage << (FLOAT)_pTimer->tmTimeDelay;
	_pNetwork->SendToServerNew(nmMessage);
}



//////////////////////////////////////////////////////////////////////////
// 판매대행
//============================================================================================================
// CNetworkLibrary::SendTradeAgentSearchReq
// 거래대행 아이템 조회 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentSearchReq(int nPageNo, int nitemType,int nitemSubType,int nJobClass,CTString stSearchWord, int nAlignType /* = -1 */)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_SEARCH_REQ;
	nmMessage << (SLONG)nitemType; 
	nmMessage << (SLONG)nitemSubType; 
	nmMessage << (SLONG)nJobClass; 
	nmMessage << stSearchWord;		//검색어
	nmMessage << (SLONG)nPageNo; 
	nmMessage << (SLONG)nAlignType; 

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentBuyReq
// 거래대행 아이템 구입 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentBuyReq(int nListidx, LONGLONG nTotalMoney)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_BUY_REQ;
	nmMessage << (SLONG)nListidx; 
	nmMessage << (LONGLONG)nTotalMoney;

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentRegListReq
// 거래대행 아이템 등록 리스트 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentRegListReq(int nPageNo, int nAlignType /* = -1 */)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_REG_LIST_REQ;
	nmMessage << (SLONG)nPageNo; 
	nmMessage << (SLONG)nAlignType; 

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentCalcListReq
// 거래대행 정산 리스트 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentCalcListReq(int nPageNo, int nAlignType /* = -1 */)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_CALCLIST_REQ;
	nmMessage << (SLONG)nPageNo; 
	nmMessage << (SLONG)nAlignType; 

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentRegCancelReq
// 거래대행 아이템 등록 취소 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentRegCancelReq(int nListidx)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_REG_CANCEL_REQ;
	nmMessage << (SLONG)nListidx; 

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentRegReq
// 거래대행 아이템 등록 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentRegReq(char iTab, char iRow, char iCol,int iIIndex, int nCount, LONGLONG nTotalMoney, LONGLONG nDepositMoney)
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_REG_REQ;
	nmMessage << (SBYTE)iTab;
	nmMessage << (SBYTE)iRow;
	nmMessage << (SBYTE)iCol;
	nmMessage << (SLONG)iIIndex;
	nmMessage << (SLONG)nCount;
	nmMessage << (LONGLONG)nTotalMoney;
	nmMessage << (LONGLONG)nDepositMoney;

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentCalculateReq
// 거래대행 아이템 정산 요청
//============================================================================================================
void CNetworkLibrary::SendTradeAgentCalculateReq()
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_CALCULATE_REQ;

	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTradeAgentCheckCalcReq
// 거래대행 정산할 아이템 요청 (이게 왜 필요한거야~~~~~~)(로그인시 이메세지를 보내야만 정산 알림을 받을 수 있다.)
//============================================================================================================
void CNetworkLibrary::SendTradeAgentCheckCalcReq()
{
	CNetworkMessage nmMessage(MSG_TRADEAGENT);

	nmMessage << (UBYTE)MSG_TRADEAGENT_CHECKCALC_REQ;

	_pNetwork->SendToServerNew(nmMessage);
}

//[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
void CNetworkLibrary::SendHanaposEvent()
{
	CNetworkMessage nmMessage(MSG_SKILL);

	nmMessage << (UBYTE)MSG_SKILL_HANARO_EVENT;

	_pNetwork->SendToServerNew(nmMessage);
}