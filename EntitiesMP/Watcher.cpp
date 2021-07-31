/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"

#include "StdH.h"

#include "EntitiesMP/EnemyBase.h"

#include <EntitiesMP/Watcher.h>
#include <EntitiesMP/Watcher_tables.h>
CEntityEvent *EWatcherInit::MakeCopy(void) { CEntityEvent *peeCopy = new EWatcherInit(*this); return peeCopy;}
SLONG EWatcherInit::GetSizeOf(void) { return sizeof(*this);}
EWatcherInit::EWatcherInit() : CEntityEvent(EVENTCODE_EWatcherInit) {;
 ClearToDefault(eidOwner);
};
BOOL EWatcherInit::CheckIDs(void) { return TRUE && eidOwner.IsValid(); }
CEntityEvent *EWatch::MakeCopy(void) { CEntityEvent *peeCopy = new EWatch(*this); return peeCopy;}
SLONG EWatch::GetSizeOf(void) { return sizeof(*this);}
EWatch::EWatch() : CEntityEvent(EVENTCODE_EWatch) {;
 ClearToDefault(eidSeen);
};
BOOL EWatch::CheckIDs(void) { return TRUE && eidSeen.IsValid(); }
void CWatcher::SetDefaultProperties(void) {
  m_penOwner = NULL;
  m_tmDelay = 6.0f;
  m_fClosestPlayer = UpperLimit(0.0f);
  m_iPlayerToCheck = 0;
  m_iOwnerID = -1;
  m_penPrediction = NULL;
  CRationalEntity::SetDefaultProperties();
}
  
#line 40 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
class CEnemyBase * CWatcher::GetOwner(void) 
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 42 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
ASSERT  (m_penOwner  != NULL );
#line 43 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return (CEnemyBase  *) & * m_penOwner ;
#line 44 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX CWatcher::GetRandomPlayer(void) 
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 50 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX ctMaxPlayers  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . size  ();
#line 51 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX ctActivePlayers  = 0;
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
for(INDEX i  = 0;i  < ctMaxPlayers ;i  ++)
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * pEntity  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  [ i  ];
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(pEntity  != NULL )
#line 56 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
ctActivePlayers  ++;
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(ctActivePlayers  == 0)
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return 0;
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX iChosenActivePlayer  = IRnd  () % ctActivePlayers ;
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX iActivePlayer  = 0;
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
for(i  = 0;i  < ctMaxPlayers ;i  ++)
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * pEntity  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  [ i  ];
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(pEntity  != NULL )
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(iActivePlayer  == iChosenActivePlayer )
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return i ;
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
iActivePlayer  ++;
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
ASSERT  (FALSE );
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return 0;
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity * CWatcher::FindClosestPlayer(void) 
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * penClosestPlayer  = NULL ;
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fClosestPlayer  = UpperLimit  (0.0f);
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
for(std  :: vector  < CEntity  * > :: iterator  iter  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . begin  ();
#line 134 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
iter  != GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . end  ();
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
++ iter )
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if((* iter ) != NULL  && (* iter ) -> GetFlags  () & ENF_ALIVE  && ! ((* iter ) -> GetFlags  () & ENF_INVISIBLE ))
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fDistance  = ((* iter ) -> GetPlacement  () . pl_PositionVector  - m_penOwner  -> GetPlacement  () . pl_PositionVector ) . Length  ();
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(fDistance  < fClosestPlayer )
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
fClosestPlayer  = fDistance ;
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
penClosestPlayer  = (* iter );
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(penClosestPlayer  == NULL )
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
fClosestPlayer  = 10.0f;
#line 154 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_fClosestPlayer  = fClosestPlayer ;
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return penClosestPlayer ;
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
void CWatcher::SendWatchEvent(CEntity * penPlayer) 
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 162 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
EWatch  eWatch ;
#line 163 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
eWatch  . eidSeen  = penPlayer ;
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_penOwner  -> SendEvent  (eWatch );
#line 165 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
void CWatcher::CheckIfPlayerVisible(void) 
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(GetOwner  () -> m_bBlind )
#line 171 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 173 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return ;
#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 175 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . empty  ())
#line 176 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 177 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return ;
#line 178 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 181 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX ctPlayers  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . size  ();
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * penEntity  = NULL ;
#line 185 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_iPlayerToCheck  = (m_iPlayerToCheck  + 1) % ctPlayers ;
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
INDEX iFirstChecked  = m_iPlayerToCheck ;
#line 187 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FOREVER 
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 189 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
penEntity  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  [ m_iPlayerToCheck  ];
#line 190 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(penEntity  != NULL )
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
break ;
#line 193 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_iPlayerToCheck  ++;
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_iPlayerToCheck  %= ctPlayers ;
#line 196 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(m_iPlayerToCheck  == iFirstChecked )
#line 197 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return ;
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 203 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(! (penEntity  -> GetFlags  () & ENF_ALIVE ) || (penEntity  -> GetFlags  () & ENF_INVISIBLE ))
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 206 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return ;
#line 207 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 210 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(GetOwner  () -> SeeEntity  (penEntity  , Cos  (GetOwner  () -> m_fViewAngle  / 2.0f)))
#line 211 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SendWatchEvent  (penEntity );
#line 214 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 218 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
void CWatcher::SetWatchDelays(void) 
#line 219 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
const FLOAT tmMinDelay  = 0.1f;
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
const FLOAT tmSeeDelay  = 5.0f;
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
const FLOAT tmTick  = _pTimer  -> TickQuantum ;
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fSeeDistance  = GetOwner  () -> m_fIgnoreRange ;
#line 224 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fNearDistance  = Min  (GetOwner  () -> m_fStopDistance  , GetOwner  () -> m_fCloseDistance );
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(m_fClosestPlayer  <= fNearDistance )
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_tmDelay  = tmMinDelay ;
#line 232 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
else 
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_tmDelay  = tmMinDelay  + 
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
(m_fClosestPlayer  - fNearDistance ) * (tmSeeDelay  - tmMinDelay ) / (fSeeDistance  - fNearDistance );
#line 239 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_tmDelay  = floor  (m_tmDelay  / tmTick ) * tmTick ;
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 244 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
void CWatcher::Watch(void) 
#line 245 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fOrgDistance  = m_fClosestPlayer ;
#line 249 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * penClosest  = NULL ;
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
penClosest  = FindClosestPlayer  ();
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fSeeDistance  = GetOwner  () -> m_fIgnoreRange ;
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fStopDistance  = Max  (fSeeDistance  * 1.5f , GetOwner  () -> m_fActivityRange );
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(fOrgDistance  < fStopDistance  && m_fClosestPlayer  >= fStopDistance )
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_penOwner  -> SendEvent  (EStop  ());
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
else if(fOrgDistance  >= fStopDistance  && m_fClosestPlayer  < fStopDistance )
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_penOwner  -> SendEvent  (EStart  ());
#line 267 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(m_fClosestPlayer  < fSeeDistance )
#line 272 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CheckIfPlayerVisible  ();
#line 275 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fSenseRange  = GetOwner  () -> m_fSenseRange ;
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(penClosest  != NULL  && fSenseRange  > 0 && m_fClosestPlayer  < fSenseRange )
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SendWatchEvent  (penClosest );
#line 284 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 287 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetWatchDelays  ();
#line 288 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 291 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity * CWatcher::CheckCloserPlayer(CEntity * penCurrentTarget,FLOAT fRange) 
#line 292 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 294 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(GetOwner  () -> m_bBlind ){
#line 296 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return NULL ;
#line 297 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 299 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity  * penClosestPlayer  = NULL ;
#line 300 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fClosestPlayer  = 
#line 301 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
(penCurrentTarget  -> GetPlacement  () . pl_PositionVector  - m_penOwner  -> GetPlacement  () . pl_PositionVector ) . Length  ();
#line 302 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
fClosestPlayer  = Min  (fClosestPlayer  , fRange );
#line 336 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
for(std  :: vector  < CEntity  * > :: iterator  iter  = GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . begin  ();
#line 337 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
iter  != GetOwner  () -> en_pwoWorld  -> m_vectorTargetNPC  . end  ();
#line 338 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
++ iter )
#line 339 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 340 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if((* iter  == NULL  || (* iter ) == penCurrentTarget ))
#line 341 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 342 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
continue ;
#line 343 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 345 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if((* iter ) -> GetFlags  () & ENF_ALIVE  && ! ((* iter ) -> GetFlags  () & ENF_INVISIBLE ))
#line 346 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 348 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
FLOAT fDistance  = ((* iter ) -> GetPlacement  () . pl_PositionVector  - m_penOwner  -> GetPlacement  () . pl_PositionVector ) . Length  ();
#line 350 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(fDistance  < fClosestPlayer  && 
#line 351 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
GetOwner  () -> SeeEntity  ((* iter ) , Cos  (GetOwner  () -> m_fViewAngle  / 2.0f)))
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 353 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
fClosestPlayer  = fDistance ;
#line 354 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
penClosestPlayer  = (* iter );
#line 355 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 356 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 357 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 380 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return penClosestPlayer ;
#line 381 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 385 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
CEntity * CWatcher::CheckAnotherPlayer(CEntity * penCurrentTarget) 
#line 386 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 431 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return NULL ;
#line 432 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
  
#line 436 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SLONG CWatcher::GetUsedMemory(void) 
#line 437 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 438 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return (sizeof  (CWatcher ) - sizeof  (CRationalEntity ) + CRationalEntity  :: GetUsedMemory  ());
#line 439 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
BOOL CWatcher::
#line 443 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Active(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatcher_Active
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatcher::Active expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 446 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Jump(STATE_CURRENT,0x02bc0005, FALSE, EInternal());return TRUE;}BOOL CWatcher::H0x02bc0005_Active_03(const CEntityEvent &__eeInput)
#line 447 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0005
if(!(TRUE )){ Jump(STATE_CURRENT,0x02bc0006, FALSE, EInternal());return TRUE;}
#line 449 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Watch  ();
#line 451 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetTimerAfter(m_tmDelay );
Jump(STATE_CURRENT, 0x02bc0003, FALSE, EBegin());return TRUE;}BOOL CWatcher::H0x02bc0003_Active_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0003
switch(__eeInput.ee_slEvent)
#line 452 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02bc0004, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EStop):{const EStop&e= (EStop&)__eeInput;
Jump(STATE_CURRENT, STATE_CWatcher_Inactive, TRUE, EVoid());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&e= (EStart&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02bc0004, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETeleport):{const ETeleport&e= (ETeleport&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02bc0004, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 460 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}return TRUE;}BOOL CWatcher::H0x02bc0004_Active_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0004
Jump(STATE_CURRENT,0x02bc0005, FALSE, EInternal());return TRUE;
#line 461 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}BOOL CWatcher::H0x02bc0006_Active_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0006
 ASSERT(FALSE); return TRUE;};BOOL CWatcher::
#line 465 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Inactive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatcher_Inactive
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatcher::Inactive expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 467 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x02bc0008, FALSE, EBegin());return TRUE;}BOOL CWatcher::H0x02bc0008_Inactive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0008
switch(__eeInput.ee_slEvent)
#line 468 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&e= (EStart&)__eeInput;
Jump(STATE_CURRENT, STATE_CWatcher_Active, TRUE, EVoid());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 471 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}return TRUE;}BOOL CWatcher::H0x02bc0009_Inactive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc0009
 ASSERT(FALSE); return TRUE;};BOOL CWatcher::
#line 475 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Dummy(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatcher_Dummy
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatcher::Dummy expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 478 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x02bc000b, FALSE, EBegin());return TRUE;}BOOL CWatcher::H0x02bc000b_Dummy_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc000b
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 481 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}return TRUE;}BOOL CWatcher::H0x02bc000c_Dummy_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc000c
; ASSERT(FALSE); return TRUE;};BOOL CWatcher::
#line 484 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatcher_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EWatcherInit, "CWatcher::Main expects 'EWatcherInit' as input!");  const EWatcherInit &eInit = (const EWatcherInit &)__eeInput;
#line 487 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
ASSERT  ((CEntity  *) eInit  . eidOwner  != NULL );
#line 488 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_penOwner  = eInit  . eidOwner ;
#line 490 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_iOwnerID  = m_penOwner  -> en_ulID ;
#line 493 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
InitAsVoid  ();
#line 494 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 495 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 498 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(! _pNetwork  -> m_bSingleMode )
#line 499 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 501 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
if(GetSP  () -> sp_gmGameMode  == CSessionProperties  :: GM_FLYOVER )
#line 502 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
{
#line 504 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Jump(STATE_CURRENT, STATE_CWatcher_Dummy, TRUE, EVoid());return TRUE;
#line 506 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 507 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
}
#line 512 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
m_iPlayerToCheck  = GetRandomPlayer  () - 1;
#line 516 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
STATE_CWatcher_Inactive, TRUE;
Jump(STATE_CURRENT, 0x02bc000d, FALSE, EBegin());return TRUE;}BOOL CWatcher::H0x02bc000d_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc000d
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: Call(STATE_CURRENT, STATE_CWatcher_Inactive, TRUE, EVoid());return TRUE;case EVENTCODE_EEnd: Jump(STATE_CURRENT,0x02bc000e, FALSE, __eeInput); return TRUE;default: return FALSE; }}BOOL CWatcher::H0x02bc000e_Main_02(const CEntityEvent &__eeInput){
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bc000e
const EEnd&__e= (EEnd&)__eeInput;
;
#line 519 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Destroy  ();
#line 521 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
Return(STATE_CURRENT,EVoid());
#line 521 "E:/질산/Korea/Client_USA/EntitiesMP/Watcher.es"
return TRUE; ASSERT(FALSE); return TRUE;};