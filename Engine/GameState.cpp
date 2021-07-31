#include "stdh.h"
#include "GameState.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Entities/SmcParser.h>

extern CGameState	*_pGameState = NULL;

// Camera Entity List
static int _aiCameraEntities[] =
{
	3607,		// 캐릭터 생성시 기본 카메라.
	3626,		// Titan
	3631,		// Knight
	3636,		// Healer
	3641,		// Mage
	3947,		// Rogue
	3953,		// Sorcerer
};

#define LOGIN_CAMERA	(1628)
#define SELCHAR_CAMERA	(3229)

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")


extern ENGINE_API BOOL g_bHead_change;
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::ClearCharacterSlot()
{
	for( int i = 0; i < MAX_SLOT; ++i )
	{
		m_SlotInfo[i].bActive = FALSE;
		
		m_SlotInfo[i].index	= -1;
		m_SlotInfo[i].job	= -1;
		m_SlotInfo[i].level	= -1;
		m_SlotInfo[i].curExp = -1;
		m_SlotInfo[i].needExp = -1;
		m_SlotInfo[i].hp		= -1;
		m_SlotInfo[i].maxHP	= -1;
		m_SlotInfo[i].mp		= -1;
		m_SlotInfo[i].maxMP	= -1;
		m_SlotInfo[i].sp		= -1;
		m_SlotInfo[i].bExtension = FALSE;
		m_SlotInfo[i].m_time = -1;

		for(int j = 0; j < WEAR_COUNT; ++j)
		{
			m_SlotInfo[i].wear[j] = -1;
		}
		m_SlotInfo[i].itemEffect.Clear();

		m_pEntModels[i] =NULL;	//wooss 050821
	}

	m_ulExistChaNum = 0;
	m_pEntModels[0]	= NULL;
	m_pEntModels[1]	= NULL;
	m_pEntModels[2]	= NULL;
	m_pEntModels[3]	= NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : failtype - 
//			nWhichUI - 
//			nCommandCode - 
//			nPosX - 
//			nPosY - 
//-----------------------------------------------------------------------------
void CGameState::DisplayErrorMessage(unsigned char failtype, int nWhichUI, int nCommandCode, int nPosX, int nPosY)
{	
	if(!_pUIMgr)	return;

	_pUIMgr->CloseMessageBox( nCommandCode );

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK,
								nWhichUI, nCommandCode );
	MsgBoxInfo.AddString( m_astrErrorMsg[failtype] );

	_pUIMgr->CreateMessageBox( MsgBoxInfo, nPosX, nPosY );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &nmMessage - 
//-----------------------------------------------------------------------------
void CGameState::ReceiveCharSlot(CNetworkMessage &nmMessage)
{
	SLONG		index;		//인덱스 //1013
	CTString	name;		//캐릭 이름
	SBYTE		job;		//직업
	SBYTE		job2;
	SBYTE		hairstyle;	//헤어 스타일 //1013
	SBYTE		facestyle;	//얼굴 스타일
	SLONG		level;		//레벨
	SQUAD		needexp;	//경험치
	SQUAD		curexp;		//경험치
	SLONG		hp;			//현재체력
	SLONG		maxHP;		//최대체력
	SLONG		mp;			//현재마나
	SLONG		maxMP;		//최대마나
	SLONG		sp;			//스킬 포인트
	SLONG		wear[WEAR_COUNT];	
	SLONG		itemPlus[WEAR_COUNT];
	SLONG		iTime = 0;
	int			i;

	
	nmMessage >> index;
	nmMessage >> name;
	nmMessage >> job;
	nmMessage >> job2;
	nmMessage >> hairstyle;//1013
	nmMessage >> facestyle;
	nmMessage >> level;	
	nmMessage >> curexp;
	nmMessage >> needexp;
	nmMessage >> sp;		//스킬 포인트
	nmMessage >> hp;
	nmMessage >> maxHP;
	nmMessage >> mp;
	nmMessage >> maxMP;
	for( i = 0; i < WEAR_COUNT ; ++i)
	{
		nmMessage >> wear[i];
		nmMessage >> itemPlus[i];
	}
	
#ifdef NEW_DELETE_CHAR
	nmMessage >> iTime;
#endif
	m_SlotInfo[m_ulExistChaNum].bActive	= TRUE;
	
	m_SlotInfo[m_ulExistChaNum].index	= index;
	strcpy(m_SlotInfo[m_ulExistChaNum].name, name);
	m_SlotInfo[m_ulExistChaNum].job		= job;
	m_SlotInfo[m_ulExistChaNum].job2	= job2;
	m_SlotInfo[m_ulExistChaNum].level	= level;
	m_SlotInfo[m_ulExistChaNum].curExp	= curexp;
	m_SlotInfo[m_ulExistChaNum].needExp	= needexp;
	m_SlotInfo[m_ulExistChaNum].hp		= hp;
	m_SlotInfo[m_ulExistChaNum].maxHP	= maxHP;
	m_SlotInfo[m_ulExistChaNum].mp		= mp;
	m_SlotInfo[m_ulExistChaNum].sp		= sp;
	m_SlotInfo[m_ulExistChaNum].hairstyle= hairstyle;
	m_SlotInfo[m_ulExistChaNum].facestyle= facestyle;
	m_SlotInfo[m_ulExistChaNum].maxMP	= maxMP;
	for(i = 0; i < WEAR_COUNT ; ++i)
	{
		m_SlotInfo[m_ulExistChaNum].wear[i]	= wear[i];			
		m_SlotInfo[m_ulExistChaNum].itemPlus[i]	= itemPlus[i];
	}
	
	m_SlotInfo[m_ulExistChaNum].m_time = iTime;

	// FIXME : 맘에 안드는 부분...
	// 무기일 경우...
#ifdef HEAD_CHANGE
	g_bHead_change = TRUE;
	if( wear[2] != -1 )
	{
		CItemData& ID = _pNetwork->GetItemData( wear[2] );
		m_SlotInfo[m_ulExistChaNum].bExtension = _pNetwork->IsExtensionState( job, ID );
	}
#else
	g_bHead_change = FALSE;
	if( wear[1] != -1 )
	{
		CItemData& ID = _pNetwork->GetItemData( wear[1] );
		m_SlotInfo[m_ulExistChaNum].bExtension = _pNetwork->IsExtensionState( job, ID );
	}
#endif

	m_ulExistChaNum++;
	CPrintF("Received Data : %d\n", m_ulExistChaNum);

}

//-----------------------------------------------------------------------------
// Purpose: ID&PW 입력창으로 카메라를 돌림.
// Input  : 
//-----------------------------------------------------------------------------
void CGameState::BackToLogin()
{
	const int iCameraEntityID	= LOGIN_CAMERA;
	CEntity *pCameraEntity		= NULL;
	CEntity *pEntity			= NULL;
	BOOL bExist					= _pNetwork->ga_World.EntityExists(iCameraEntityID, pCameraEntity);
	if(bExist)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 선택화면으로 카메라를 돌림
// Input  : 
//-----------------------------------------------------------------------------
void CGameState::BackToSelChar()
{
	const int iCameraEntityID	= SELCHAR_CAMERA;
	CEntity *pCameraEntity		= NULL;
	CEntity *pEntity			= NULL;
	BOOL bExist					= _pNetwork->ga_World.EntityExists(iCameraEntityID, pCameraEntity);
	if(bExist)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);							
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iJob - 기본 카메라는 -1이 들어와서 +1을 해서 0이됨.
//				   그 외에는 직업의 값 +1로 처리됨...
//-----------------------------------------------------------------------------
void CGameState::SetCameraByJob(int iJob)
{
	int iCameraEntityID			= _aiCameraEntities[iJob + 1];
	CEntity *pCameraEntity		= NULL;
	CEntity *pEntity			= NULL;
	BOOL bExist					= _pNetwork->ga_World.EntityExists(iCameraEntityID, pCameraEntity);
	if(bExist)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, TRUE);
	}
}

// FIXME : Player.es와 같은 내용의 함수임.
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::DeleteDefaultArmor(CModelInstance *pMI, int type, int iJob)
{
	int iWearPos = -1;

	// FIXME : type 값과 입는 위치값이 다르기 때문에 문제 발생...
	switch( type )// 헬멧은 헤어랑 같이 쓰고 있기 때문에 따로 처리
	{
#ifdef HEAD_CHANGE
	case WEAR_HELMET:
		iWearPos = HEAD;
		break;
#endif
	case WEAR_JACKET:
		iWearPos = BODYUP;
		break;
	case WEAR_PANTS:
		iWearPos = BODYDOWN;
		break;
	case WEAR_GLOVES:
		iWearPos = HAND;
		break;
	case WEAR_BOOTS:
		iWearPos = FOOT;
		break;
	}

	if( iWearPos == -1 )
		return;

	if( type == WEAR_PANTS )
	{
		CTFileName fnMeshName = JobInfo().GetMeshName( iJob, SKIRT );
		if(strlen( fnMeshName ) > 0)
		{
			pMI->DeleteMesh( fnMeshName );
		}
	}
#ifdef HEAD_CHANGE
	if (type == WEAR_HELMET )
	{ // 헬멧의 경우 헤어를 삭제한다.
		INDEX iHeadMesh = pMI->mi_iHairMesh;
		MeshInstance* TempMI = pMI->FindMeshInstance(iHeadMesh);
		if(TempMI)
		{
			pMI->DeleteMesh(TempMI->mi_pMesh->GetName());
		}
	}
	else
	{ // 장비의 경우 삭제
		pMI->DeleteMesh( JobInfo().GetMeshName( iJob, iWearPos ) );
	}
#endif
	pMI->DeleteMesh( JobInfo().GetMeshName( iJob, iWearPos ) );

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::TakeOffArmor( CModelInstance *pMI, CItemData& ID )
{	
	int iItemIndex = ID.GetItemIndex();

	if (_pNetwork->wo_aItemSmcInfo.size() <= iItemIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iItemIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfoListSize();
	int i;

	//////////////////////////////////////////////
	// [070810: Su-won] EVENT_ADULT_OPEN
	if( iItemIndex ==2378 || iItemIndex ==2379 )		// 헌터&카오 머리띠일 때
	{
		TakeOffHairBand( pMI, iItemIndex);
		return;
	}
	// [070810: Su-won] EVENT_ADULT_OPEN
	//////////////////////////////////////////////

	if (iItemIndex >= 2470 && iItemIndex <= 2475)
	{ // 호박탈을 벗으면 hidden 속성의 npc를 볼 수 없다.
		if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance() == pMI) // 자신의 캐릭터가 장비를 벗었을 때
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOff(ENF_SHOWHIDDEN);
			if (_pNetwork->_TargetInfo.dbIdx == 454)
			{
				if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
					_pUIMgr->StopTargetEffect(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID()); // 타겟 이펙트 종료
			}
		}
	}

	if (iItemIndex >= 2598 && iItemIndex <= 2603 ||
		iItemIndex >= 2611 && iItemIndex <= 2618)
	{ // 빛나는 산타 모자 착용시 루돌프 코 추가
		TakeOffRudolphNose(pMI, iItemIndex);
	}

	for (i=0; i<iMeshCount; i++)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfo(i);

		pMI->DeleteMesh(Mesh.GetMeshTFNM());
	}
}

void CGameState::TakeOffArmorTest(CModelInstance *pMI, INDEX iIndex)
{
	if (iIndex <= 0) return;

	if (_pNetwork->wo_aItemSmcInfo.size() <= iIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iIndex);
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfoListSize();
	int i;

	for (i=0; i<iMeshCount; i++)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfo(i);

		pMI->DeleteMesh(Mesh.GetMeshTFNM());
	}	
}

// FIXME : Player.es와 같은 내용의 함수임.
// FIXME : Character.es 와 Player.es에도 WearingArmor가 있는데, TakeOff하는 시점이 다른거 같아서,
// FIXME : 하나로 합치는데 문제가 있음.
// FIXME : 두 군데서 사용하는 루틴이 같아야함.
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::WearingArmor(CModelInstance *pMI, CItemData& ID ) //갑옷을 입을때 호출.
{
	int iItemIndex = ID.GetItemIndex();
	// 정보가 없다.
	if (_pNetwork->wo_aItemSmcInfo.size() <= iItemIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iItemIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iItemIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iItemIndex);
		return;
	}

	//0609 kwon 추가.
#ifdef HEAD_CHANGE
	if(ID.GetType() == CItemData::ITEM_SHIELD || ID.GetType() == CItemData::ITEM_WEAPON)
#else
	if((ID.GetType() == CItemData::ITEM_SHIELD
		&& 	(ID.GetSubType() == CItemData::ITEM_SHIELD_COAT  
		|| 	ID.GetSubType() == CItemData::ITEM_SHIELD_PANTS 
		|| 	ID.GetSubType() == CItemData::ITEM_SHIELD_GLOVE 
		|| 	ID.GetSubType() == CItemData::ITEM_SHIELD_SHOES
		|| 	ID.GetSubType() == CItemData::ITEM_SHIELD_SHIELD
		)) ||  ID.GetType() == CItemData::ITEM_WEAPON //0808 무기도 이제 매쉬를 쓴다.
		)			
#endif
	{
		//////////////////////////////////////////////
		// [070810: Su-won] EVENT_ADULT_OPEN
		if( iItemIndex ==2378 || iItemIndex ==2379 )	// 헌터&카오 머리띠 일 때
		{
			WearingHairBand( pMI, iItemIndex);
			return;
		}
		// [070810: Su-won] EVENT_ADULT_OPEN
		//////////////////////////////////////////////
		
		if (iItemIndex >= 2470 && iItemIndex <= 2475)
		{ // 호박탈은 hidden 속성의 npc를 볼 수 있다.
			if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance() == pMI) // 자신의 캐릭터가 장비를 착용할때
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
		}

		if (iItemIndex >= 2598 && iItemIndex <= 2603 ||
			iItemIndex >= 2611 && iItemIndex <= 2618)
		{ // 빛나는 산타 모자 착용시 루돌프 코 추가
			WearingRudolphNose(pMI, iItemIndex);
		}

		int iMeshCount = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfoListSize();
		int i,j;
		MeshInstance *mi;

		for (i=0; i<iMeshCount; i++)
		{
			CMeshInfo Mesh;
			Mesh = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfo(i);

			mi = pMI->AddArmor(Mesh.GetMeshTFNM());
			int iSize = Mesh.GetTexInfoSize();
			
			for (j=0; j<iSize; j++)
			{
				CTFileName fnFileName = (CTString)Mesh.GetTexInfoTFNM(j);
				pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
			}
		}
	}
}

void CGameState::WearingArmorTest(CModelInstance *pMI, INDEX iIndex)
{
	if (iIndex <= 0) return;

	if (_pNetwork->wo_aItemSmcInfo.size() <= iIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iIndex);
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfoListSize();
	int i,j;
	MeshInstance *mi;

	for (i=0; i<iMeshCount; i++)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfo(i);

		mi = pMI->AddArmor(Mesh.GetMeshTFNM());
		int iSize = Mesh.GetTexInfoSize();
		
		for (j=0; j<iSize; j++)
		{
			CTFileName fnFileName = (CTString)Mesh.GetTexInfoTFNM(j);
			pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// [070810: Su-won] EVENT_ADULT_OPEN
void CGameState::GetHairBandFilePath( INDEX iIndex, int iJob, CTString* strBMPath, CTString* strTexPath )
{
	if( iIndex == 2378 )
	{
		switch( iJob )
		{
		case TITAN:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Titan\\ti_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Titan\\Texture\\ti_angel.tex";
			}
			break;
		case KNIGHT:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Knight\\ni_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Knight\\Texture\\ni_angel.tex";
			}
			break;
		case HEALER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Healer\\hw_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Healer\\Texture\\hw_angel.tex";
			}
			break;
		case MAGE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel.tex";

				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_angel_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel_1.tex";
			}
			break;
		case ROGUE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_angel.bm";
				strTexPath[0] = "Data\\Item\\Shield\\Rogue\\Texture\\ro_angel.tex";
			}
			break;
		case SORCERER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Sorcerer\\so_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Sorcerer\\Texture\\so_angel.tex";
			}
			break;
		}
	}
	else if( iIndex == 2379 )
	{
		switch( iJob )
		{
		case TITAN:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Titan\\ti_devil.bm";
				strTexPath[0] = "Data\\Item\\Shield\\Titan\\Texture\\ti_devil.tex";
			}
			break;
		case KNIGHT:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Knight\\ni_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Knight\\Texture\\ni_devil.tex";
			}
			break;
		case HEALER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Healer\\hw_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Healer\\Texture\\hw_devil.tex";
			}
			break;
		case MAGE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil.tex";

				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_devil_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil_1.tex";
			}
			break;
		case ROGUE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Rogue\\Texture\\ro_devil.tex";
			}
			break;
		case SORCERER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Sorcerer\\so_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Sorcerer\\Texture\\so_devil.tex";
			}
			break;
		}
	}
}

void CGameState::WearingHairBand( CModelInstance *pMI, INDEX iIndex )
{
	CTString strBMPath[2];
	CTString strTexPath[2];

	int iJob;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;

	GetHairBandFilePath( iIndex, iJob, strBMPath, strTexPath);

	MeshInstance *mi =pMI->AddArmor(strBMPath[0]);
	CTFileName fnFileName = strTexPath[0];
	pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	

	//메이지는 Mesh 2개임...
	if( iJob ==MAGE )
	{
		mi =pMI->AddArmor(strBMPath[1]);
		fnFileName = strTexPath[1];
		pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	
	}
}

void CGameState::TakeOffHairBand( CModelInstance *pMI, INDEX iIndex )
{
	CTString strBMPath[2];
	CTString strTexPath[2];

	int iJob;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;

	GetHairBandFilePath( iIndex, iJob, strBMPath, strTexPath);

	pMI->DeleteMesh( strBMPath[0] );

	//메이지는 mesh 2개임...
	if( iJob )
		pMI->DeleteMesh( strBMPath[1] );

}
// [070810: Su-won] EVENT_ADULT_OPEN
//////////////////////////////////////////////////////////////////////////////////////////////////

void CGameState::GetRudolphNoseFilePath(INDEX iIndex, int iJob, CTString& strBMPath, CTString& strTexPath)
{
	switch (iJob)
	{
	case TITAN:
		{
			strBMPath = "Data\\Item\\Shield\\Titan\\ti_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Titan\\Texture\\ti_rudolphnose.tex";
		}
		break;
	case KNIGHT:
		{
			strBMPath = "Data\\Item\\Shield\\Knight\\ni_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Knight\\Texture\\ni_rudolphnose.tex";
		}
		break;
	case HEALER:
		{
			strBMPath = "Data\\Item\\Shield\\Healer\\hw_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Healer\\Texture\\hw_rudolphnose.tex";
		}
		break;
	case MAGE:
		{
			strBMPath = "Data\\Item\\Shield\\Mage\\ma_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Mage\\Texture\\ma_rudolphnose.tex";
		}
		break;
	case ROGUE:
		{
			strBMPath = "Data\\Item\\Shield\\Rogue\\ro_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Rogue\\Texture\\ro_rudolphnose.tex";
		}
		break;
	case SORCERER:
		{
			strBMPath = "Data\\Item\\Shield\\Sorcerer\\so_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Sorcerer\\Texture\\so_rudolphnose.tex";
		}
		break;
	}
}

void CGameState::WearingRudolphNose(CModelInstance *pMI, INDEX iIndex)
{
	CTString strBMPath;
	CTString strTexPath;

	int iJob;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;

	GetRudolphNoseFilePath( iIndex, iJob, strBMPath, strTexPath);

	MeshInstance *mi =pMI->AddArmor(strBMPath);
	CTFileName fnFileName = strTexPath;
	pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	
}

void CGameState::TakeOffRudolphNose(CModelInstance *pMI, INDEX iIndex)
{
	CTString strBMPath;
	CTString strTexPath;

	int iJob;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;

	GetRudolphNoseFilePath( iIndex, iJob, strBMPath, strTexPath);

	pMI->DeleteMesh( strBMPath );
}