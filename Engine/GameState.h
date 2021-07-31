// ----------------------------------------------------------------------------
//  File : GameManager.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	GameState_H_
#define	GameState_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemEffect.h>
#include <Engine/LocalDefine.h>


#define MAX_SLOT		(4)	//wooss 050819
#define	MAX_ERROR_MSG	(50)

#ifdef HEAD_CHANGE
	#define WEAR_COUNT 7
#else
	#define WEAR_COUNT 6
#endif



#ifdef HEAD_CHANGE
	static int wearTypeTable[WEAR_COUNT] = {WEAR_HELMET, WEAR_JACKET, WEAR_WEAPON, WEAR_PANTS, WEAR_SHIELD, WEAR_GLOVES, WEAR_BOOTS };
#else
	static int wearTypeTable[WEAR_COUNT] = {WEAR_JACKET, WEAR_WEAPON, WEAR_PANTS, WEAR_SHIELD, WEAR_GLOVES, WEAR_BOOTS };
#endif

class ENGINE_API CGameState
{
public:
	struct SLOT_INFO
	{
		BOOL		bActive;		
		SLONG		index;	//인덱스
		char		name[50]; //캐릭 이름
		SBYTE		job;		//직업
		SBYTE		job2;		//직업
		SBYTE		hairstyle;//헤어 스타일 //1013
		SBYTE		facestyle;//얼굴 스타일
		SLONG		level;	//레벨
		SQUAD		curExp, needExp; //경험치
		SLONG		hp; //현재체력
		SLONG		maxHP; //최대체력
		SLONG		mp; //현재마나
		SLONG		maxMP; //최대마나
		SLONG		wear[WEAR_COUNT];		
		SLONG		itemPlus[WEAR_COUNT];
		SLONG		m_time;
		SLONG		sp; //스킬 포인트
		CItemEffect itemEffect;
		BOOL		bExtension;
	};

	enum GameMode 
	{
		GM_NONE = 0,			
		GM_NETWORK,
		GM_INTRO,
		GM_LOGIN,
	};

public:
	CGameState()
	{
		m_RunningGameMode	= GM_NONE;
		m_bRunning			= TRUE;
		m_bQuitScreen		= TRUE;
		m_ulExistChaNum		= 0;
		m_iSelectedSlot		= 0;
		
		for(int i = 0 ; i<MAX_SLOT ;i++)
		{
			m_pEntModels[i]		= NULL;
		}
		
	};	

	inline INDEX&	GetGameMode()
	{ return	m_RunningGameMode; };

	inline BOOL&		Running()
	{ return	m_bRunning; };

	inline BOOL&		QuitScreen()
	{ return	m_bQuitScreen; };

	inline ULONG		GetExistCharNum()		const
	{ return	m_ulExistChaNum; };

	inline INDEX&		SelectedSlot()
	{ return	m_iSelectedSlot;	};

	void				ClearCharacterSlot();
	void				ReceiveCharSlot(CNetworkMessage &nmMessage);

	// 캐릭터 생선화면에서 캐릭터 옷 입히기.  
	// FIXME : player.es와 동일한 부분임.
	void				DeleteDefaultArmor( CModelInstance *pMI, int type, int iJob );
	void				TakeOffArmor( CModelInstance *pMI, CItemData& ID );
	void				WearingArmor( CModelInstance *pMI, CItemData& ID );

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// [070810: Su-won] EVENT_ADULT_OPEN
	// 헌터&카오 머리띠 착용관련
	void				GetHairBandFilePath( INDEX iIndex, int iJob, CTString* strBMPath, CTString* strTexPath );
	void				WearingHairBand( CModelInstance *pMI, INDEX iIndex );
	void				TakeOffHairBand( CModelInstance *pMI, INDEX iIndex );
	// [070810: Su-won] EVENT_ADULT_OPEN
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// X-MAS 루돌프 코
	void				GetRudolphNoseFilePath(INDEX iIndex, int iJob, CTString& strBMPath, CTString& strTexPath);
	void				WearingRudolphNose(CModelInstance *pMI, INDEX iIndex);
	void				TakeOffRudolphNose(CModelInstance *pMI, INDEX iIndex);

	// 에러 메시지 출력.
	void				DisplayErrorMessage(unsigned char failtype, int nWhichUI, int nCommandCode, int nPosX = -1, int nPosY = -1);	
	
	// 로그인 프로세스 과정상에서의 카메라...
	void				BackToLogin();
	void				BackToSelChar();
	void				SetCameraByJob(int iJob = -1);
	
	// 테스트용
	void				WearingArmorTest(CModelInstance *pMI, INDEX iIndex);
	void				TakeOffArmorTest(CModelInstance *pMI, INDEX iIndex);

public:
	CEntity*			m_pEntModels[MAX_SLOT];
	SLOT_INFO			m_SlotInfo[MAX_SLOT];

	CTString			m_astrErrorMsg[MAX_ERROR_MSG];

private:
	INDEX				m_RunningGameMode;
	BOOL				m_bRunning;
	BOOL				m_bQuitScreen;		
	ULONG				m_ulExistChaNum;
	INDEX				m_iSelectedSlot;
};

ENGINE_API extern CGameState *_pGameState;
#endif // GAMESTATE_H_
