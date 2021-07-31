#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Contents/Base/UIDurability.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/ItemComposeUI.h>
#include <Engine/Contents/function/SSkillLearnUI.h>

#define SKILL_LEARNED_COLOR				0xF2F2F2FF
#define NOT_SATISFACTION_COL			0xFF0000FF
#define SATISFACTION_COL				0xFFFF00FF
#define EXTENSION_LEVEL					(31)
#define SKILL_NOT_EXIST					-1
// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;

enum eSelection
{
	SKILL_LEARN,
	SKILL_TALK,
	SKILL_EVENT,
	SKILL_JOB_1,			// 전직하기 1번
	SKILL_JOB_2,			// 전직하기 2번
	SKILL_NPC_HELP,
	SKILL_TITAN,
	SKILL_KNIGHT,
	SKILL_HEALER,
	SKILL_MAGE,
	SKILL_ROGUE,
	SKILL_SORCERER,
	SKILL_NIGHTSHADOW,
#ifdef CHAR_EX_ROGUE
	SKILL_EX_ROGUE,	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	SKILL_EX_MAGE,	// 2013/01/08 jeil EX메이지 추가 
#endif
	SKILL_QUEST,
#ifdef DURABILITY
	ITEM_DURABILITY,
	ITEM_RECOVERY,
#endif	//	DURABILITY
	ITEM_COMPOSE
};

enum eSatisfactionType
{
	SKILL_CONDITION_LEVEL = 0,
	SKILL_CONDITION_SP,
	SKILL_CONDITION_STR,
	SKILL_CONDITION_DEX,
	SKILL_CONDITION_INT,
	SKILL_CONDITION_CON,
	SKILL_CONDITION_SKILL_0,
	SKILL_CONDITION_SKILL_1,
	SKILL_CONDITION_SKILL_2,
	SKILL_CONDITION_ITEM_0,
	SKILL_CONDITION_ITEM_1,
	SKILL_CONDITION_ITEM_2,
};

// ----------------------------------------------------------------------------
// Name : CUISkillLearn()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISkillLearn::CUISkillLearn()
{
	m_iSelChangeJob	= -1;
	m_iMobIdx = -1;
	m_iMobVirIdx = -1;
	m_bQuest = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUISkillLearn()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISkillLearn::~CUISkillLearn()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
//
// 현재 스킬이 조건을 만족하는지를 체크합니다.
//
//////////////////////////////////////////////////////////////////////////
int CUISkillLearn::IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial )
{
	int nSatisfied = 0;

	if( nSkillIndex <= 0 )
		return SKILL_NOT_EXIST;

	if (nLevel > 0)
		--nLevel; // 데이터 레벨은 1부터, 스킬데이터 접근 배열은 0 부터

	if (nLevel < 0)
	{
		LOG_DEBUG("스킬 레벨이 %d 입니다.", nLevel);
		return SKILL_NOT_EXIST;
	}

	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );

		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;

		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );

			if( nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}

		// 필요 레벨 체크
		if(_pNetwork->MyCharacterInfo.level < rSkill.GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// 필요 숙련도 체크
		if((_pNetwork->MyCharacterInfo.sp / 10000) < rSkill.GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;
		// 필요 능력치 체크

		const int iLearnStr = rSkill.GetLearnStr( nLevel );
		const int iLearnDex = rSkill.GetLearnDex( nLevel );
		const int iLearnInt = rSkill.GetLearnInt( nLevel );
		const int iLearnCon = rSkill.GetLearnCon( nLevel );

		if( iLearnStr > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.str - _pNetwork->MyCharacterInfo.opt_str  < iLearnStr )
				nSatisfied |= NOT_SATISFIED_STR;
		}

		if( iLearnDex > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.dex - _pNetwork->MyCharacterInfo.opt_dex < iLearnDex )
				nSatisfied |= NOT_SATISFIED_DEX;
		}

		if( iLearnInt > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel < iLearnInt )
				nSatisfied |= NOT_SATISFIED_INT;
		}

		if( iLearnCon > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.con - _pNetwork->MyCharacterInfo.opt_con < iLearnCon )
				nSatisfied |= NOT_SATISFIED_CON;
		}

		CUIManager* pUIManager = CUIManager::getSingleton();

		// 필요 스킬 체크
		if( bLearnSkill )
		{
			for( int i = 0; i < 3; i++ )
			{
				if (nLearnSkillIndex[i] != -1)
				{
					//내 스킬 가져오는 방법을 찾자.
					// 캐릭터 인포쪽인가?
					int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex[i]);
					if(nCurrentSkillLevel < sbLearnSkillLevel[i])
					{
						nSatisfied |= (NOT_SATISFIED_SKILL_0 << i);
					}
				}
			}
		}

		// 필요 아이템 체크
		if( bLearnItem )
		{
			for( int i = 0; i < 3; i++ )
			{
				//2013/04/10 jeil
				if( nLearnItemIndex[i] != -1 )
				{
					if(nLearnItemIndex[i] == 19)
					{
						if(_pNetwork->MyCharacterInfo.money < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
					else
					{
						SQUAD nCurrentItemCount = pUIManager->GetInventory()->GetItemCount(nLearnItemIndex[i]);
						if(nCurrentItemCount < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
				}
			}
		}
	}
	else
	{
		CSpecialSkill* pSSkill = CSpecialSkill::getData( nSkillIndex );

		if (pSSkill == NULL)
			return SKILL_NOT_EXIST;

		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;

		nLearnSkillIndex	= pSSkill->GetLearnSkillIndex();
		sbLearnSkillLevel	= pSSkill->GetLearnSkillLevel();

		// 필요 레벨 체크
		if(_pNetwork->MyCharacterInfo.level < pSSkill->GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// 필요 숙련도 체크
		if((_pNetwork->MyCharacterInfo.sp / 10000) < pSSkill->GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;

		if( nLearnSkillIndex != -1 )
		{
			int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex, true);
			if(nCurrentSkillLevel < sbLearnSkillLevel)
			{
				nSatisfied |= NOT_SATISFIED_SKILL_0;
			}
		}
	}

	return nSatisfied;
}

void CUISkillLearn::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
}

// ----------------------------------------------------------------------------
// Name : OpenSkillLearn()
// Desc : nMasterType은 일반 스킬일때는 직업, 특수 스킬일때는 특수 스킬타입이 됩니다.
// ----------------------------------------------------------------------------
void CUISkillLearn::OpenSkillLearn( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SKILLLEARN_REQ ) || IsVisible() )
		return;
	
	// Special Skill
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SSKILLLEARN_REQ ) || IsVisible() )
		return;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;
	m_iMobIdx = iMobIndex;
	m_bQuest = bHasQuest;

	m_iMobVirIdx = iMobVirIdx;

	CMobData* MD	= CMobData::getData(iMobIndex);
	int nSSkillType	= MD->GetSpecialSkillMaster();
	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
	m_bSpecial		= MD->IsSSkillMaster() ? TRUE : FALSE;

	if( MD->IsSSkillMaster() )		// 특수 스킬 마스터일때...
	{
		// FIXME : 아래와 같이 메세지가 그때 그때 다르면 대략 낭패.
		switch( nSSkillType )
		{
		case SSKILL_MINING:			// 채굴
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 630, "채굴 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 631, "광석을 채굴하는거는 그리 어렵지 않아." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 632, "하지만 얼마만큼 빠른 시간에 많은 양을 캐내느냐가 실력을 좌우 하지." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_GATHERING:		// 채집
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 633, "채집 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 634, "크락을 채집할 때도 많은 기술이 필요하지." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 635, "기술이 좋으면 좋을수록 더 많은 양을 채집하고 잘만 하면 보기 힘들다는 크락의 꽃도 얻을 수 있다네." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_CHARGE:			// 차지
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 636, "차지 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 637, "차지 기술에 대해 좀더 알고 싶습니까?" ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 638, "좀 더 고급의 차지 기술을 배운다면 많은양의 원소를 얻을 수 있을 것입니다.  배워 보시겠습니까?" ), -1, 0xA3A1A3FF );	
			}
			break;
		case SSKILL_STONE:			// 광석정련술
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 639, "스톤정련 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
#ifdef DURABILITY
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 6194, "좋은 장비를 쓰는것도 중요 하지만, 장비를 수리하지 않으면 아무 쓸모작에도 없지." ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 6195, "자네 장비를 수리할 생각 없는가? 새 장비를 입은 기분이 날껄세" ), -1, 0xA3A1A3FF );

				CTString strTemp;
				strTemp.PrintF( _S( 6429, "합성한다." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_COMPOSE );
				strTemp.PrintF( _S( 6192, "내구도를 수리한다." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_DURABILITY );
				strTemp.PrintF( _S( 6193, "최대 내구도를 복구한다." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_RECOVERY );
#else	//	DURABILITY
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 640, "스톤을 정련 하는 것은 아무나 하는 것이 아냐! 자네가 배워 보겠다고?" ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 641, "요즘 젊은이들은 스톤 정련 하는 것을 너무 쉽게 보는 경향이 있어." ), -1, 0xA3A1A3FF );

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, _S( 6429, "합성한다." ), ITEM_COMPOSE );
#endif	//	DURABILITY
			}
			break;
		case SSKILL_PLANT:			// 식물가공술
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 642, "식물가공 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 643, "크낙을 이용해 옷을 만들고 싶다고? 정신이 나갔군." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 644, "크낙으로 옷까지 만드려면 엄청나게 오랜 시간이 걸려! 과연 당신이 할 수 있을까?" ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_ELEMENT:		// 원소정제술
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 645, "원소정제 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 646, "원소를 정제 하려면 경건한 마음과 많은 인내가 필요 합니다." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 647, "하지만 정제술을 배우기만 한다면 그에 상응하는 보상을 받을 수 있을 것입니다. 자연은 거짓말을 하지 않으니까요." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_WEAPON:	// 무기제작
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 648, "무기제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 649, "이런! 또 무기 제작을 배우고 싶어서 온 친구군." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 650, "난 더 이상 무기를 만들지 않아 ! 더 이상 예전의 내가 아니라고! 말로 해도 소용 없군." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_POTINO: // 물약 제작
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 767, "물약 제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 768, "물약 제조는 조심스럽고도 어려운 작업입니다." ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 769, "물약을 찾는 사람은 많은데 만들 사람이 없는 실정이기도 합니다. 힘들지만 물약 제작기술을 배워 보시겠나요?" ), -1, 0xA3A1A3FF );
			}
			break;
		case SSKILL_MAKE_WEAR:
		case SSKILL_MAKE_G_B:
		case SSKILL_MAKE_ARMOR:
		case SSKILL_MAKE_H_S:
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 651, "방어구제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 652, "조금이나마 편이 살고 싶다면 방어구 제작 기술을 배워 보라구!" ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 653, "요즘은 수요가 많아서 아주 할만 하다구!" ), -1, 0xA3A1A3FF );		
			}
			break;
		}
		
		CTString strMessage;		
		strMessage.PrintF( _S( 1218, "스킬을 습득한다." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
			// 2009. 05. 27 김정래
			// 이야기한다 변경 처리
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SSKILLLEARN_REQ);				
		}
			
		strMessage.PrintF(_S( 1748, "NPC 안내" ));
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );

		strMessage.PrintF( _S( 1220, "취소한다." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage );
	}
	// ITS# 4252 : BUG-FIXED  [9/26/2011 rumist]
	else if( iMobIndex == 1200 || iMobIndex == 1198 )	// 타리안 NPC - 스킬 마스터 푸치아
	{	// 어둠의 스킬 마스터 UI를 보여준다
		PriorityOpen(iMobIndex, bHasQuest, FALSE);
	}
	else						// 일반 스킬 마스터일때...
	{
		const int iJob = _pNetwork->MyCharacterInfo.job;
		// If job is different
		if( MD->GetSkillMaster() != iJob )
		{
			// WSS_BUG_FIX 070802 ------>><<
			if( pUIManager->DoesMessageBoxExist( MSGCMD_SKILLLEARN_NOTIFY ) ) { return;}

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(3589,"해당 직업의 스킬은 습득할 수 없습니다.") );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}
		
		// Create skill learn message box
		pUIManager->CreateMessageBoxL( _S( 270, "스킬" ), UI_SKILLLEARN, MSGLCMD_SKILLLEARN_REQ );

		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 271, "끊임없이 노력하는 자만이 진정한 강함을 손에 넣을 수 있는 법!\n\n그런 강인한 의지가 있는 자를 인도하는 것이 내가 하는 일이지.\n" ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 272, "현재의 자네 수준에 맞는 스킬을 선택하여 익히도록 하게나." ), -1, 0xA3A1A3FF );

		CTString strMessage;
		strMessage.PrintF( _S( 1218, "스킬을 습득한다." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
			// 2009. 05. 27 김정래
			// 이야기한다 변경 처리
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SKILLLEARN_REQ);				
		}

		// 전직 가능 레벨이고 전직한 상태가 아닐때...
		// 나이트 쉐도우는 전직이 없으므로. [8/6/2010 rumist]
		if( _pNetwork->MyCharacterInfo.level >= EXTENSION_LEVEL && _pNetwork->MyCharacterInfo.job2 == 0 &&
			_pNetwork->MyCharacterInfo.job != NIGHTSHADOW )
		{
			strMessage.PrintF( _S( 1223, "[%s]로 전직" ), CJobInfo::getSingleton()->GetExtensionName( iJob, 0 ) );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_1 );

			strMessage.PrintF( _S( 1223, "[%s]로 전직" ), CJobInfo::getSingleton()->GetExtensionName( iJob, 1 ) );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_2 );
		}

		strMessage.PrintF(_S( 1748, "NPC 안내" ));
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );

		strMessage.PrintF( _S( 1220, "취소한다." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage );

		
	}
}

// ----------------------------------------------------------------------------
// Name : PriorityOpen()
// Desc : 어둠의 스킬 마스터의 UI 창
//			스킬 습득 UI 창보다 먼저 실행
// ----------------------------------------------------------------------------	
// BUF FIX : ITS# 4472 [10/13/2011 rumist]
void CUISkillLearn::PriorityOpen(int iIndex, BOOL bHasQuest, BOOL bUseCard/* =FALSE */ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxLExist(MSGLCMD_RESERVE_REQ)) return;
	
	CTString	strNpcName = CMobData::getData(iIndex)->GetName();
	// Create skill learn message box
	pUIManager->CreateMessageBoxL( _S( 270, "스킬" ), UI_SKILLLEARN, MSGLCMD_RESERVE_REQ );

	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 271, "끊임없이 노력하는 자만이 진정한 강함을 손에 넣을 수 있는 법!\n\n그런 강인한 의지가 있는 자를 인도하는 것이 내가 하는 일이지.\n" ), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 3541,"그대 또한 강함을 쫓는 자인가? 그런 강인한 의지가 있는 자를 인도 하는 것이 내 일이지! 각오는 되어 있겠지?"), -1, 0xA3A1A3FF);

	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3555, "타이탄 스킬을 습득한다."),SKILL_TITAN);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3556, "나이트 스킬을 습득한다."),SKILL_KNIGHT);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3557, "힐러 스킬을 습득한다."),SKILL_HEALER);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3558, "메이지 스킬을 습득한다."),SKILL_MAGE);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3559, "로그 스킬을 습득한다."),SKILL_ROGUE);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3560, "소서러 스킬을 습득한다."),SKILL_SORCERER);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 4797, "나이트 쉐도우 스킬을 습득한다."),SKILL_NIGHTSHADOW);
#ifdef CHAR_EX_ROGUE
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 5755, "EX로그 스킬을 습득한다."),SKILL_EX_ROGUE);	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 5843, "아크메이지 스킬을 습득한다."),SKILL_EX_MAGE);	// 2013/01/08 jeil EX메이지 추가 스트링 나오면 추가 수정 필요 
#endif

	// 2009. 05. 27 김정래
	// 이야기한다 변경 처리
	// quest bug FIX : ITS#4472  [10/13/2011 rumist]
	if( bHasQuest )
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_RESERVE_REQ);				

	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 1220, "취소한다." ) );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SKILLLEARN_NOTIFY:
		{
			CUIManager::getSingleton()->CloseMessageBox(MSGCMD_SKILLLEARN_NOTIFY);
		}
		break;

	case MSGCMD_CHANGEJOB:			// 전직 할꺼에요?
		if( bOK )
		{
			// 서버에 메세지 보내기.
			_pNetwork->ChangeJobReq( m_iSelChangeJob, m_iMobVirIdx );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CJobInfo* pInfo = CJobInfo::getSingleton();

	CTString strMessage;

	switch( nCommandCode )
	{
	case MSGLCMD_SKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// 스킬을 습득한다.
		{
			//pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );

// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3308, "캐릭터가 배울 수 있는 스킬의 목록이 보여집니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3309, "목록의 스킬 아이콘을 클릭하면 해당 스킬의 설명과 필요조건이 표시되며 배우고 싶은 스킬을 결정했다면 스킬 아이콘을 클릭 한 뒤 습득 버튼을 누릅니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3310, "※ 스킬을 배우는데 필요한 조건에 만족된 경우에만 스킬을 습득할 수 있습니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3311, "※ 스킬을 배울 때는 스킬 종류 및 레벨에 따라 필요한 숙련도가 자동 삭감됩니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3312, "※ 습득한 스킬은 캐릭터의 스킬 창(ALT+S)에서 확인할 수 있습니다."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}

			pUIManager->GetSkillNew()->OpenUI(m_iMobIdx, m_iMobVirIdx, m_bQuest, m_fNpcX, m_fNpcZ);

		}
		else if( nResult == SKILL_TALK )				// 이야기 한다.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == SKILL_JOB_1 )				// 전직 클래스 1
		{
			pUIManager->CreateMessageBoxL( _S( 1224, "전직 정보" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int iJob			= _pNetwork->MyCharacterInfo.job;

			CTString strNeedNas;
			strNeedNas.PrintF("%I64d", llNeedNas);
			pUIManager->InsertCommaToString(strNeedNas);

			// FiXME : 아래 부분은 정리가 필요한 부분.

			switch( iJob )
			{
			case TITAN:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_HIGHLANDER ), -1, 0xFFC672FF );				
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1225, "전 클래스 가운데서 공격력이 으뜸인 클래스로서 적에게 맹공을 퍼붓는 공격수로서의 몫을 톡톡히 해 내는 클래스입니다.\n" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1226, "사용 무기 : 도끼" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1227, "선결 조건 : %dlv달성, 보디 크래셔 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );					
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1229, "힘 : 공격상승, 물리방어 상승" ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1230, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );								
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1231, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1232, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );				
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1233, "전직 보너스 스탯 : 힘" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "타고난 강인한 육체에서 터져 나오는 투혼은 타이탄 중에서도 으뜸으로 인정받는 자들이 바로 하이랜더 입니다." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "이들은 전장에서 한 걸음의 물러섬없이 전장을 자유롭게 종횡무진하며 거대한 도끼를 자신의 육체처럼 자유롭게 사용합니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case KNIGHT:		//  로열 나이트
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ROYALKNIGHT ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1234, "기본적으로 단련된 단단한 방어력과 높아진 공격력을 비롯한 빠른 공격으로 적을 제압하며 개인간 전투에서 안정적인 전투를 보여주는 클래스입니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1235, "사용 무기 : 이도류" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1236, "선결 조건 : %dlv달성, 마나 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1238, "힘 : 공격상승, 물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1239, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1240, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1241, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1242, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "정의를 실천하는 방법으로서 악으로부터 소중한 존재를 보호하는 것에서, 악을 응징하는 것으로 선회한 자들이 바로 로열 나이트입니다. " ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "로열 나이트는 그들을 지켜주던 굳건한 방패는 뒤로 하고 대신 예리하게 날이 선 두 자루의 검을 택하여 그들이 가진 능력 이상으로 공격력을 끌어 올렸습니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ARCHER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1243, "하이엘프 아처들은 화살 한발만으로도 다수의 적을 공격하거나, 빠른 이동속도, 활을 이용한 장거리 공격으로 히트 앤 런 방식의 공격에 특화된 능력을 가지고 있습니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1244, "사용 무기 : 활" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1245, "선결 조건 : %dlv달성, 더블 스팅샷 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1246, "힘 :  물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1247, "민첩 : 공격 상승, 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1248, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1249, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1250, "전직 보너스 스탯 : 힘" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "많은 하이엘프들이 그들이 선천적으로 타고난 힘인 마법을 이용해 동료들의 생명을 구하며 치유와 재생에 힘 쓰지만 때론 마법적인 힘보다, 물리적인 힘에 관심을 갖고 이에 집중하는 자들이 하이엘프 아처입니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WIZARD ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1251, "높은 공격력으로 파괴적이고 넓은 공간을 일순간에 불바다로 만들어 버리는 광역마법에 특화 된 자들로 마법에 활용에 있어 으뜸인 클래스입니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1252, "사용 무기 : 스태프" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1253, "선결 조건 : %dlv달성, 노바 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1254, "힘 :  물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1255, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1256, "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1257, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1258, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드는 마법 그 자체를 광범위하게 연구하는 메이지들 중에 마법의 원초적인 정수만을 뽑아 거대한 힘을 구성하는 자들입니다." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드가 허공을 향해 성호를 그을 때마다 강력한 군세 자체가 소멸 될 정도의 위력을 지니기에 그 한 명 한 명이 하나의 군단급으로 평가된다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case ROGUE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ASSASSIN ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1378, "안정적인 공격, 방어 능력과 빠른 스킬 속도를 겸비한 클래스로서 1:1의 전투에서 맹 활약을 펼치는 기습형 클래스입니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1379, "사용 무기 : 단검" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1380, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1381, "힘 :  물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1382, "민첩 : 공격 상승 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1383, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1384, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1385, "전직 보너스 스탯 : 민첩" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "어세신은 짧은 단검과 날렵한 몸놀림을 이용한 초 접근전을 펼치는 것이 주된 전투법이며, 상대가 미쳐 단련하지 못은 부분이나 갑옷으로 감싸지 못한 미세한 틈을 노려 공격하며 조금이라도 빈틈을 보이면 곧장 급소를 공격하여 순식간에 상대의 목숨을 끊는 것이 어세신의 특기입니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case SORCERER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, 1 ), -1, 0xFFC672FF );									
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2342,"엘리멘탈 리스트는 정령 소환을 마스터하여 불, 물, 바람, 대지로 구분되는 4대 정령들을 강화 시키고 그들을 소환하여 함께 전투에 임하는 클래스입니다."  ), -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2343,"사용 무기 : 폴암"  ), -1, 0xA3A1A3FF );

					strMessage.PrintF( _S(2344, "선결 조건 : %dlv달성, 아이스 스파이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "지혜 : 마법 공격 상승, 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "체질 : 생명, 마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "전직 보너스 스탯 : 지혜"  ), -1, 0xA3A1A3FF );											
					
				}
				break;
#ifdef CHAR_EX_ROGUE
			case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_ASSASSIN ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5739, "안정적인 공격, 방어 능력과 빠른 스킬 속도를 겸비한 클래스로서 1:1의 전투에서 맹 활약을 펼치는 기습형 클래스입니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5740, "사용 무기 : 단검" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 5741, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5742, "힘 :  물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5743, "민첩 : 공격 상승 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5744, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5745, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5746, "전직 보너스 스탯 : 민첩" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "어세신은 짧은 단검과 날렵한 몸놀림을 이용한 초 접근전을 펼치는 것이 주된 전투법이며, 상대가 미쳐 단련하지 못은 부분이나 갑옷으로 감싸지 못한 미세한 틈을 노려 공격하며 조금이라도 빈틈을 보이면 곧장 급소를 공격하여 순식간에 상대의 목숨을 끊는 것이 어세신의 특기입니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
#endif
#ifdef CHAR_EX_MAGE
			case EX_MAGE: //2013/01/08 jeil EX메이지 추가 스트링 나오면 추가 수정 필요 
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_WIZARD ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5827, "높은 공격력으로 파괴적이고 넓은 공간을 일순간에 불바다로 만들어 버리는 광역마법에 특화 된 자들로 마법에 활용에 있어 으뜸인 클래스입니다." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5828, "사용 무기 : 스태프" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 5829, "선결 조건 : %dlv달성, 골든 노바 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5830, "힘 :  물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5831, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5832, "지혜 : 마법 공격상승, 마법 방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5833, "체질 : 생명,마나 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5834, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드는 마법 그 자체를 광범위하게 연구하는 메이지들 중에 마법의 원초적인 정수만을 뽑아 거대한 힘을 구성하는 자들입니다." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드가 허공을 향해 성호를 그을 때마다 강력한 군세 자체가 소멸 될 정도의 위력을 지니기에 그 한 명 한 명이 하나의 군단급으로 평가된다.\n" ), -1, 0xA3A1A3FF );			
				}
				break;
#endif
			}

			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]로 전직한다." ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, 0 ) );		
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "취소하기") );			

			m_iSelChangeJob = 1;
		}
		else if( nResult == SKILL_JOB_2 )				// 전직 클래스 2
		{
			pUIManager->CreateMessageBoxL( _S( 1224, "전직 정보" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int	iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int	iJob		= _pNetwork->MyCharacterInfo.job;

			CTString strNeedNas;
			strNeedNas.PrintF("%I64d", llNeedNas);
			pUIManager->InsertCommaToString(strNeedNas);
			// FiXME : 아래 부분은 정리가 필요한 부분.
			switch( iJob )
			{
			case TITAN:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WARMASTER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1269,  "대규모 전투에서 단연 눈에 뛰는 활약을 보여주며 하이랜더를 상회하는 공격력과 전 클래스를 통틀어 가장 높은 체력을 가지고 있습니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1270,  "사용 무기 : 대검"  ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1271, "선결 조건 : %dlv달성, 보디 크래셔 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1272,  "힘 : 공격상승, 물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1273,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1274,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1275,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1276,  "전직 보너스 스탯 : 체질"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "호전적인 성격으로 오랜 시간 전투가 몸에 베인 타이탄 중에 탁월한 감각으로 전장의 세를 읽고 순발력있게 대처하거나 아군의 사기를 높은 함성으로 붇돋아 주는 클래스가 이들 워 마스터입니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case KNIGHT:		// 템플 나이트
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_TEMPLEKNIGHT ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1277,  "파티원을 위험해서 구하거나, 몬스터를 조롱하여 자신에게 덤벼들게 하는 등의 파티에서 든든한 벽이 되어 주며, 대규모 전투에서도 선봉장의 역할을 수행하게 됩니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1278,  "사용 무기 : 검과 방패"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1279, "선결 조건 : %dlv달성, 마나 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1280,  "힘 : 공격상승, 물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1281,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1282,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1283,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1284,  "전직 보너스 스탯 : 체질"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "희생을 자신들이 가진 최고의 미덕으로 여기고 있는 템플나이트는 파티원을 위험에서 보호하며 그들을 지원하여, 팀 플레이를 원활하게 해 주는 윤활유 같은 역할을 해 주는 존재입니다. " ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "때문에 이들의 공격력은 로열 나이트에 비해 비록 뒤쳐지지만, 높은 방어력과 튼튼한 체력은 이들이 가진 큰 장점입니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_CLERIC ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1285, "높은 회복력으로 적을 치유하며, 동료의 능력을 최대치로 끌어올려주는 보조마법 등을 사용하는 등 파티 플레이의 든든한 지원자 역할을 수행하는 클래스 입니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1286, "사용 무기 : 완드"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1287, "선결 조건 : %dlv달성, 더블 스팅샷 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1288, "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1289, "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1290, "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1291, "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1292, "전직 보너스 스탯 : 지혜"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "하이엘프들 가운데 파괴를 증오하고, 창조와 재생에 관해 몰두하는 하이엘프 클레릭은 높은 마법력으로 동료들에게 생명력을 불어 넣어 주며, 독, 결빙, 상태이상의 마법에서 육체를 자유로울 수 있도록 도와줍니다.\n" ), -1, 0xA3A1A3FF );				
					
					
				}
				break;
			case MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WITCH ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1293, "상태이상 마법 및 저주 마법을 사용하는 클래스로서 적이 가공할 힘을 가졌다 하더라도 그 힘을 원천적으로 봉쇄하는등의 지능적인 마법사 입니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1294, "사용 무기 : 완드"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1295, "선결 조건 : %dlv달성, 노바 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1296,  "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1297,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1298,  "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1299,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1300,  "전직 보너스 스탯 : 민첩"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위치는 위험스럽고, 치명적이며 은밀한 마법을 연구하는 자들입니다. 이들이 행하는 마법의 연구대상은 다름아닌 인간의 육체이며, 작은 마법의 흐름으로 적을 혼란에 빠뜨리거나, 잠재우고, 신체조직을 변이 시키는 등의 마법을 사용해 적을 무기력하게 만듭니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case ROGUE:	
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_RANGER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1301,  "레인저는 주변의 자연지물을 이용한 함정과 석궁을 이용한 장거리 공격을 하며, 속사의 명수입니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1302,  "사용 무기 : 석궁"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1303, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1304,  "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1305,  "민첩 : 공격 상승 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1306,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1307,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1308,  "전직 보너스 : 힘"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "레인저의 두려움은 근거리가 아닌 중, 원거리에서의 전투와 다양한 함정과 도구의 활용에서 나타난다." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "짧은 시간 내에도 그 곳의 환경을 활용한 다양한 트랩을 설치하는 것은 물론 어떤 상황에서도 어김없이 급소를 향해 정확한 공격이 가능한 레인저는 자신의 몸놀림을 활용하여 사방팔방을 전장을 활보한다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case SORCERER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, 2 ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2350,"스페셜리스트는 정령의 힘을 빌어 불, 바람의 정령 강신과 물, 땅의 정령 강신을 통해 자신의 신체를 강화시켜 직접 전투에 임하는 전투 스타일을 갖는 클래스 입니다. "), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2351, "사용 무기 : 사이드" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S(2344, "선결 조건 : %dlv달성, 아이스 스파이크 lv5 습득" ), iNeedLevel );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "힘 : 물리방어 상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );												
				}
				break;
#ifdef CHAR_EX_ROGUE
			case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_RANGER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5747,  "레인저는 주변의 자연지물을 이용한 함정과 석궁을 이용한 장거리 공격을 하며, 속사의 명수입니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5748,  "사용 무기 : 석궁"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 5749, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5750,  "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5751,  "민첩 : 공격 상승 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5752,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5753,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5754,  "전직 보너스 : 힘"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "레인저의 두려움은 근거리가 아닌 중, 원거리에서의 전투와 다양한 함정과 도구의 활용에서 나타난다." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "짧은 시간 내에도 그 곳의 환경을 활용한 다양한 트랩을 설치하는 것은 물론 어떤 상황에서도 어김없이 급소를 향해 정확한 공격이 가능한 레인저는 자신의 몸놀림을 활용하여 사방팔방을 전장을 활보한다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
#endif
#ifdef CHAR_EX_MAGE		//2013/01/08 jeil EX메이지 추가 스트링 나오면 추가 수정 필요 
			case EX_MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_WITCH ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5835, "상태이상 마법 및 저주 마법을 사용하는 클래스로서 적이 가공할 힘을 가졌다 하더라도 그 힘을 원천적으로 봉쇄하는등의 지능적인 마법사 입니다."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5836, "사용 무기 : 완드"  ), -1, 0xA3A1A3FF );							
					
					strMessage.PrintF( _S( 5837, "선결 조건 : %dlv달성, 골든 노바 브레이크 lv5 습득" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					
					strMessage.PrintF( _S( 1228, "필요 나스 : %s 나스" ), strNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5838,  "힘 :  물리방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5839,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5840,  "지혜 : 마법 공격상승, 마법 방어 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5841,  "체질 : 생명,마나 상승"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5842,  "전직 보너스 스탯 : 민첩"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위치는 위험스럽고, 치명적이며 은밀한 마법을 연구하는 자들입니다. 이들이 행하는 마법의 연구대상은 다름아닌 인간의 육체이며, 작은 마법의 흐름으로 적을 혼란에 빠뜨리거나, 잠재우고, 신체조직을 변이 시키는 등의 마법을 사용해 적을 무기력하게 만듭니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
#endif
			}
			
			
			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]로 전직한다." ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, 1 ) );		
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "취소하기" ) );			

			m_iSelChangeJob = 2;
		}
		else if(nResult == SKILL_NPC_HELP)											// 취소한다.
		{
			pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
		}
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}
		else
		{
			pUIManager->GetSSkillLearn()->closeUI();
		}
		break;

	case MSGLCMD_CHANGEJOB_REQ:							// 전직 정보창.
		{
			if( nResult == 0 )
			{				
				pUIManager->CloseMessageBox( MSGCMD_CHANGEJOB );				
				
				CTString	strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 1317, "전직" ), UMBS_OKCANCEL, UI_SKILLLEARN, MSGCMD_CHANGEJOB );		
				strMessage.PrintF( _S( 1318, "%s로 전직 하시겠습니까?" ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, m_iSelChangeJob - 1 ) );	
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			else
			{
			}
		}
		break;

		// Special Skill
	case MSGLCMD_SSKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// 스킬을 습득한다.
		{
			pUIManager->GetSSkillLearn()->openUI(m_iMobIdx, m_iMobVirIdx, m_fNpcX, m_fNpcZ);
		}
		else if( nResult == SKILL_TALK )				// 이야기 한다.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == SKILL_NPC_HELP)								
		{
			pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
		}
#ifdef DURABILITY
		else if (nResult == ITEM_DURABILITY)
		{
			pUIManager->GetDurability()->Open(CUIDurability::eDURABILITY, m_iMobIdx, m_fNpcX, m_fNpcZ);
		}
		else if (nResult == ITEM_RECOVERY)
		{
			pUIManager->GetDurability()->Open(CUIDurability::eRECOVERY, m_iMobIdx, m_fNpcX, m_fNpcZ);
		}
#endif	//	DURABILITY
		else if (nResult == ITEM_COMPOSE)
		{
			pUIManager->GetItemCompose()->openUI();
		}
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else// 취소한다.
		{
			pUIManager->GetSSkillLearn()->closeUI();
		}
		break;
	case MSGLCMD_RESERVE_REQ:
		{
			// [090527: selo] 확장팩 퀘스트 이야기 한다 처리 수정을 위한 루틴
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}
			
			switch(nResult)
			{
			pUIManager->SetCSFlagOff( CSF_SHOP );

			case SKILL_TITAN:
				{
					OpenSkillLearn(75, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_KNIGHT:
				{
					OpenSkillLearn(76, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_HEALER:
				{
					OpenSkillLearn(77, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_MAGE:
				{
					OpenSkillLearn(123, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_ROGUE:
				{
					OpenSkillLearn(176, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_SORCERER:
				{
					OpenSkillLearn(255, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_NIGHTSHADOW:
				{
					OpenSkillLearn(894, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#ifdef CHAR_EX_ROGUE
			case SKILL_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
				{
					OpenSkillLearn(1504, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#endif
#ifdef CHAR_EX_MAGE
			case SKILL_EX_MAGE:		//2013/01/08 jeil EX메이지 추가 의미를 몰라서 나중에 물어보고 추가 수정 필요 NPC 인텍스로 수정 
				{
					OpenSkillLearn(1521, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#endif
			case SKILL_QUEST:
				{
					CUIQuestBook::TalkWithNPC();
				}
				
				// [090527: selo] 확장팩 퀘스트 수정
			case ciQuestClassifier:
				{
					// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;
			}
		}break;
	}
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkill( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel, BOOL bSpecial )
{
 	CUIManager* pUIManager = CUIManager::getSingleton();
 
 	if( !bSpecial )
		return;

	CSpecialSkill* pSSkill = CSpecialSkill::getData( slIndex );

	if (pSSkill == NULL)
		return;
	
	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	
	// Create message box of skill learn
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK, UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), pSSkill->GetName() );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	MY_INFO()->SetSkill(slIndex, (int)sbLevel, bSpecial > 0);
	
	if( sbIsNew != 0 )
		pUIManager->GetCharacterInfo()->AddSkill( slIndex, sbLevel, bSpecial );

	pUIManager->GetSSkillLearn()->updateList();
}

// ----------------------------------------------------------------------------
// Name : LearnSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch( ubError )
	{
	case MSG_SKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "숙련도가 부족하여 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_ITEM:
		strMessage = _S( 280, "아이템이 존재하지 않아 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SKILL:
		strMessage = _S( 281, "스킬 조건이 맞지 않아 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_STR:			// 힘 부족
		strMessage = _S( 1319, "힘이 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_DEX:			// 덱스 부족
		strMessage = _S( 1320, "민첩이 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_INT:			// 지혜 부족
		strMessage = _S( 1321, "지혜가 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_CON:			// 체질 부족
		strMessage = _S( 1322, "체질이 부족하여 스킬을 습득할 수 없습니다." );		
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : LearnSSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch(ubError)
	{
	case MSG_SSKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "숙련도가 부족하여 스킬을 습득할 수 없습니다." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL:
		strMessage = _S( 654, "해당 기술을 배우는데 필요한 기술을 배우지 않았습니다." );			
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL_LEVEL:
		strMessage = _S( 655, "스킬 레벨이 부족합니다." );	
		break;
	case MSG_SSKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "스킬을 습득할 수 없습니다." );
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 656, "생산스킬" ), UMBS_OK,			
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

