#include "StdH.h"
#include <Engine/Base/FileName.h>
#include <Engine/Base/Stream.h>

#include <Engine/Entities/QuestSystem.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/JobInfo.h>
#include <Engine/World/World.h>
#include <Engine/LocalDefine.h>

// ------------- CQuestStaticData ------------- 
CQuestStaticData::CQuestStaticData()
: m_iIndex( -1 )
, m_iCategory( 0 )
, m_iPartyScale( 0 )
{
}

CQuestStaticData::~CQuestStaticData()
{
}

// ------------- CQuestDynamicData ------------- 
CQuestDynamicData::CQuestDynamicData(const CQuestStaticData &data)
: m_rStaticData( data )
, m_bQuestComplete( FALSE )
, m_iQuestIndex( data.m_iIndex )
, m_ctTitleDesc( 0 )
, m_ctStatusDesc( 0 )
, m_ctPrizeDesc( 0 )
, m_ctOptionPrizeDesc( 0 )
, m_ctNeedDesc( 0 )
{
	for(INDEX i=0; i<QUEST_MAX_CONDITION; ++i)
	{
		//m_iConditionValue[i] = conditionValue[i];
		m_iCurrentConditionValueStatus[i] = 0;
	}
	
	m_bPrize = FALSE;
	for(i=0; i<QUEST_MAX_PRIZE; ++i)
	{
		m_bPrize = m_bPrize || (m_rStaticData.m_iPrizeType[i] != -1);
		m_iPrizeType[i] = m_rStaticData.m_iPrizeType[i];
		m_iPrizeIndex[i] = FindItemForPrize(m_iPrizeType[i], m_rStaticData.m_iPrizeIndex[i]);
		m_iPrizeData[i] = m_rStaticData.m_iPrizeData[i];
				
		m_bOptionPrize = m_rStaticData.m_bOptionPrize;
		m_iOptionPrizeType[i] = m_rStaticData.m_iOptionPrizeType[i];
		m_iOptionPrizeIndex[i] = FindItemForPrize(m_iOptionPrizeType[i], m_rStaticData.m_iOptionPrizeIndex[i]);
		m_iOptionPrizeData[i] = m_rStaticData.m_iOptionPrizeData[i];
		m_iOptionPrizePlus[i] = m_rStaticData.m_iOptionPrizePlus[i];
	}
	
	m_strIntroDesc = "";
	for( i = 0; i < QUEST_TITLE_DESC; i++ )
		m_strTitleDesc[i] = "";
	for( i = 0; i < QUEST_STATUS_DESC; i++ )
		m_strStatusDesc[i] = "";
	for( i = 0; i < QUEST_PRIZE_DESC; i++ )
		m_strPrizeDesc[i] = "";
	for( i = 0; i < QUEST_PRIZE_DESC; i++ )
		m_strOptionPrizeDesc[i] = "";
	for( i = 0; i < QUEST_NEED_DESC; i++ )
		m_strNeedDesc[i] = "";
	
	MakeQuestFirstDesc();
}

CQuestDynamicData::~CQuestDynamicData()
{
}

INDEX CQuestDynamicData::FindItemForPrize(INDEX type, INDEX index)
{
	if(type != QPRIZE_ITEM) return index;

	CItemData	&ItemData = _pNetwork->GetItemData( index );
	if(ItemData.GetType() == CItemData::ITEM_SHIELD
	&& ItemData.GetSubType() != CItemData::ITEM_SHIELD_SHIELD)
	{
		for(int i=0; i<_pNetwork->wo_iNumOfItem; ++i)
		{
			if(_pNetwork->GetItemData(i).GetType() == ItemData.GetType()
			&& _pNetwork->GetItemData(i).GetSubType() == ItemData.GetSubType()
			&& _pNetwork->GetItemData(i).GetLevel() == ItemData.GetLevel()
			&& _pNetwork->GetItemData(i).CanUse(_pNetwork->MyCharacterInfo.job))
			{
				return i;
			}
		}
	}
	return index;
}

void CQuestDynamicData::SetCurrentConditionValueStatus(INDEX current[QUEST_MAX_CONDITION])
{
	for( int i = 0; i < QUEST_MAX_CONDITION; ++i )
		m_iCurrentConditionValueStatus[i] = current[i];
	
	MakeQuestStatusDesc();
}

void CQuestDynamicData::MakeQuestFirstDesc()
{
	MakeQuestTitleDesc();
	MakeQuestStatusDesc();
	MakeQuestPrizeDesc();
	MakeQuestNeedDesc();
	MakeQuestOptionPrizeDesc();
}

void CQuestDynamicData::MakeQuestTitleDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctTitleDesc; i++ )
		m_strTitleDesc[i].Clear();
	m_ctTitleDesc = 0;
	
	// Introduction desc
	m_strIntroDesc.PrintF( "%s\n\n\n", GetDesc() );
	m_colIntroDesc = 0xA3A1A3FF;
	
	// Title desc
	m_strTitleDesc[m_ctTitleDesc].PrintF( "%s", GetName() );
	m_colTitleDesc[m_ctTitleDesc++] = 0xF2F2F2FF;
	
	CTString	strTemp;
	switch( GetQuestType1() )
	{
	case QTYPE_KIND_COLLECTION:		// 수집형 퀘스트
		{
			// Title desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL);
				
				//const char	*szItemName = _pNetwork->GetItemData( GetConditionIndex( iCond ) ).GetName();
				const char	*szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 49, szItemName, ", %s<를> %d마리" ),
						szItemName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S2( 50, szItemName, ", %s<를> %d개" ),
						szItemName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 )
			{
				m_strTitleDesc[m_ctTitleDesc] += _S( 51, " 모아와라\n\n" );
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
		
	case QTYPE_KIND_DELIVERY:		// 배달형 퀘스트
		{
			// Title desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL );
				
				const char	*szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S( 460, "%s %d개 " ),	
						szItemName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S( 461 , ", %s %d개 " ),
						szItemName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 && GetPrizeNPCIndex() != -1)
			{
				//const char	*szMobName = _pNetwork->GetMobData( GetPrizeNPCIndex() ).GetMonsterName();
				const char	*szMobName = _pNetwork->GetMobName( GetPrizeNPCIndex() );
				strTemp.PrintF( _S( 462, "를 %s에게 전달하라.\n" ), szMobName);			
				
				m_strTitleDesc[m_ctTitleDesc] += strTemp;
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
		
	case QTYPE_KIND_REPEAT:			// 반복 퀘스트
		{
			// Desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_NPC );				
				
				//const char	*szMobName = _pNetwork->GetMobData( GetConditionIndex( iCond ) ).GetMonsterName();
				const char	*szMobName = _pNetwork->GetMobName( GetConditionIndex( iCond ) );
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 52, szMobName, "%s<를> %d마리" ),
						szMobName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S2( 53, szMobName, ", %s<를> %d마리" ),
						szMobName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 )
			{
				m_strTitleDesc[m_ctTitleDesc] += _S( 54, " 격파하라\n\n" );
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
	case QTYPE_KIND_SAVE:		// 구출형 퀘스트 수행.
		{
		/*
		// Desc
		INDEX	ctCond = 0;
		for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
		{
		const int iConditionType = GetConditionType( iCond );
		if( iConditionType == -1 )
		continue;
		
		  //ASSERT( GetConditionType( iCond ) == QCONDITION_NPC );
		  
			if(iConditionType == QCONDITION_NPC)
			{
			const char*	szMobName = _pNetwork->GetMobData( GetConditionIndex( iCond ) ).GetMonsterName();
			if( ctCond == 0 )
			{
			m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 55, szMobName, "%s<를>" ), szMobName );
			ctCond++;
			}
			else
			{
			strTemp.PrintF( _S2( 56, szMobName, ", %s<를>" ), szMobName );
			m_strTitleDesc[m_ctTitleDesc] += strTemp;
			ctCond++;
			}
			}
			else if(iConditionType == QCONDITION_ITEM)
			{
			const char	*szItemName = _pNetwork->GetItemData( GetConditionIndex( iCond ) ).GetName();
			if( ctCond == 0 )
			{
			m_strTitleDesc[m_ctTitleDesc].PrintF( _S( 460, "%s %d개 ",	
			szItemName, GetConditionNum( iCond ) );
			ctCond++;
			}
			else
			{
			strTemp.PrintF( _S( 461 , ", %s %d개 " ),						
			szItemName, GetConditionNum( iCond ) );
			m_strTitleDesc[m_ctTitleDesc] += strTemp;
			ctCond++;
			}
			}
			}
			*/
			
			// FIXME : 하드 코딩한 부분
			// FIXME : 필드상에는 구출하려는 NPC가 찍혀있지 않기 때문에, 몹 LOD 화일에 NPC 정보가 없어서,
			// FIXME : 표시가 되지 않아서, 하드 코딩함.
			m_strTitleDesc[m_ctTitleDesc] += _S( 463 , "나오 공주를 구출하라.\n" );		
			m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
		}
		break;
	case QTYPE_KIND_DEFEAT:		// 싱글던젼 퀘스트 수행.
		{
			// FIXME : QTYPE_KIND_REPEAT와 중복되는 부분임.
			// Desc
			INDEX	ctCond = 0;
			/*
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
			if( GetConditionType( iCond ) == -1 )
			continue;
			
			  ASSERT( GetConditionType( iCond ) == QNEED_MONSTER );
			  
				const char	*szMonName = _pNetwork->GetMobData( GetConditionIndex( iCond ) ).GetMonsterName();
				if( ctCond == 0 )
				{
				m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 348, szMonName, "%s<를> %d마리" ),
				szMonName, GetConditionValue( iCond ) );
				ctCond++;
				}
				else
				{
				strTemp.PrintF( _S2( 349, szMonName, ", %s<를> %d마리" ),
				szMonName, GetConditionValue( iCond ) );
				m_strTitleDesc[m_ctTitleDesc] += strTemp;
				ctCond++;
				}
				}
			*/
			
			//if( ctCond > 0 )
			//{
			// FIXME : 하드 코딩.
			m_strTitleDesc[m_ctTitleDesc] += _S( 350, "발록을 격파하라\n\n" );
			m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			//}
		}
		break;
	}
	
	// Need item
	BOOL	bNeedItem = FALSE;
	for( INDEX iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
	{
		if( GetNeedItemIndex( iNeed ) != -1 )
			bNeedItem = TRUE;
	}
	
	if( bNeedItem )
	{
		m_strTitleDesc[m_ctTitleDesc] = _S( 351, "필요 아이템" );
		m_colTitleDesc[m_ctTitleDesc++] = 0xF2F2F2FF;
		
		INDEX	ctNeed = 0;
		for( iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
		{
			if( GetNeedItemIndex( iNeed ) == -1 )
				continue;
			
			const char* szItemName = _pNetwork->GetItemName( GetNeedItemIndex( iNeed ) );
			if( ctNeed == 0 )
			{
				m_strTitleDesc[m_ctTitleDesc].PrintF( _S( 352, "%s %d개\n" ),
					szItemName,
					GetNeedItemCount( iNeed ) );
				ctNeed++;
			}
			else
			{
				strTemp.PrintF( _S( 352, "%s %d개\n" ),
					szItemName,
					GetNeedItemCount( iNeed ) );
				m_strTitleDesc[m_ctTitleDesc] += strTemp;
				ctNeed++;
			}
		}
		
		m_strTitleDesc[m_ctTitleDesc] += "\n";
		m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
	}
}

void CQuestDynamicData::MakeQuestStatusDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctStatusDesc; i++ )
		m_strStatusDesc[i].Clear();
	m_ctStatusDesc = 0;
	
	// Status desc
	switch( GetQuestType1() )
	{
	case QTYPE_KIND_COLLECTION:		// 수집형 퀘스트
	case QTYPE_KIND_DELIVERY:		// 배달형 퀘스트
		{
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL );
				const char *szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				
				m_strStatusDesc[m_ctStatusDesc].PrintF( _S( 58, "%s 숫자" ),
					szItemName );
				m_colStatusDesc[m_ctStatusDesc++] = 0xC0C0C0FF;
				m_strStatusDesc[m_ctStatusDesc].PrintF( "%d / %d", GetCurrentConditionValueStatus( iCond ),
					GetConditionNum( iCond ) );
				m_colStatusDesc[m_ctStatusDesc++] = 0x80CC17FF;
			}
			
			if( m_ctStatusDesc > 0 )
				m_strStatusDesc[m_ctStatusDesc - 1] += "\n\n";
		}
		break;
		
/*
	case QTYPE_KIND_DELIVERY:		// 배달형 퀘스트
		{
		}
		break;
*/
		
	case QTYPE_KIND_REPEAT:			// 반복 퀘스트
		{
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				//ASSERT( GetConditionType( iCond ) == QCONDITION_MONSTER );
				
				m_strStatusDesc[m_ctStatusDesc].PrintF( _S( 59, "%s 격파 수" ),
					//_pNetwork->GetMobData( GetConditionIndex( iCond ) ).GetMonsterName() );
					_pNetwork->GetMobName( GetConditionIndex( iCond ) ) );
				m_colStatusDesc[m_ctStatusDesc++] = 0xC0C0C0FF;
				//m_strStatusDesc[m_ctStatusDesc].PrintF( "0" );
				m_strStatusDesc[m_ctStatusDesc].PrintF( "%d / %d", GetCurrentConditionValueStatus( iCond ),
					GetConditionNum( iCond ) );
				m_colStatusDesc[m_ctStatusDesc++] = 0x80CC17FF;
			}
			
			if( m_ctStatusDesc > 0 )
				m_strStatusDesc[m_ctStatusDesc - 1] += "\n\n";
		}
		break;
	case QTYPE_KIND_SAVE:			// 구출 퀘스트
		{
		}
		break;
	case QTYPE_KIND_DEFEAT:			// 격파 퀘스트
		{
		}
		break;
	}
}

void CQuestDynamicData::MakeQuestPrizeDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctPrizeDesc; i++ )
		m_strPrizeDesc[i].Clear();
	m_ctPrizeDesc = 0;
	
	m_strPrizeDesc[m_ctPrizeDesc] = _S( 60, "퀘스트 보상" );
	m_colPrizeDesc[m_ctPrizeDesc++] = 0xF2F2F2FF;
	
	for( INDEX iPrize = 0; iPrize < QUEST_MAX_PRIZE; iPrize++ )
	{
		switch( GetPrizeType( iPrize ) )
		{
		case QPRIZE_ITEM:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S( 61, "%s %d개" ),
					_pNetwork->GetItemName( iPrizeIndex ), GetPrizeData( iPrize ) );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_MONEY:
			{
				//INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				//CItemData	&ItemData = _pNetwork->GetItemData( iPrizeIndex );
				
				//if( ItemData.GetType() == CItemData::ITEM_ETC &&			//돈일때 ~개라고 하면 이상하니깐.
				//	ItemData.GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					//m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s", _pNetwork->GetItemData( iPrizeIndex ).GetName());
					m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S( 836, "%d 나스" ), GetPrizeData( iPrize ) );
				}
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
			
		case QPRIZE_EXP:
			{
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S( 62, "경험치 %d" ), GetPrizeData( iPrize ) );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			} break;
			
		case QPRIZE_SP:
			{
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "SP %d", GetPrizeData( iPrize ) );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			} break;
		case QPRIZE_SKILL:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s", _pNetwork->GetSkillData( iPrizeIndex ).GetName() );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_SSKILL:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s", _pNetwork->GetSSkillData( iPrizeIndex ).GetName() );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		default:
			break;
		}
	}
}

void CQuestDynamicData::MakeQuestOptionPrizeDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctOptionPrizeDesc; i++ )
		m_strOptionPrizeDesc[i].Clear();
	m_ctOptionPrizeDesc = 0;
	
	m_strOptionPrizeDesc[m_ctOptionPrizeDesc] = _S( 1654, "퀘스트 선택 보상 (다음중 1)" );
	m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xF2F2F2FF;
	
	for( INDEX iPrize = 0; iPrize < QUEST_MAX_PRIZE; iPrize++ )
	{
		switch( GetOptionPrizeType( iPrize ) )
		{
		case QPRIZE_ITEM:
			{
				INDEX		iPrizeItemIndex = GetOptionPrizeIndex( iPrize );
				if(GetOptionPrizePlus(iPrize) > 0)
				{
					m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 1655, "%s +%d  %d개" ),
						_pNetwork->GetItemName( iPrizeItemIndex ), GetOptionPrizePlus(iPrize), GetOptionPrizeData( iPrize ) );
				}
				else
				{
					if(iPrizeItemIndex == 19)//나스인 경우(플러스된 나스는 존재하지 않는다)
					{
						m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 836, "%d 나스" ),
							GetOptionPrizeData( iPrize ) );
					}
					else
					{
						m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 61, "%s %d개" ),
							_pNetwork->GetItemName( iPrizeItemIndex ), GetOptionPrizeData( iPrize ) );
					}
				}
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			}
			break;
/*		//일단은 아이템만 있다고 함.
		case QPRIZE_MONEY:
			{
				m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 836, "%d 나스" ), GetOptionPrizeData( iPrize ) );
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			}
			break;
			
		case QPRIZE_EXP:
			{
				m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 62, "경험치 %d" ), GetOptionPrizeData( iPrize ) );
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			} break;
			
		case QPRIZE_SP:
			{
				m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( "SP %d", GetOptionPrizeData( iPrize ) );
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			} break;
		case QPRIZE_SKILL:
			{
				INDEX		iPrizeIndex = GetOptionPrizeIndex( iPrize );
				m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( "%s", _pNetwork->GetSkillData( iPrizeIndex ).GetName() );
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_SSKILL:
			{
				INDEX		iPrizeIndex = GetOptionPrizeIndex( iPrize );
				m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( "%s", _pNetwork->GetSSkillData( iPrizeIndex ).GetName() );
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			}
			break;
*/
		default:
			break;
		}
	}
}

void CQuestDynamicData::MakeQuestNeedDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctNeedDesc; i++ )
		m_strNeedDesc[i].Clear();
	m_ctNeedDesc = 0;
	
	m_strNeedDesc[m_ctNeedDesc] = _S( 464, "필요 조건" );
	m_colNeedDesc[m_ctNeedDesc++] = 0xF2F2F2FF;
	
	m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 465, "레벨 : %d ~ %d" ), GetNeedMinLevel(), GetNeedMaxLevel() );
	m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
	
	const int iNeedJob = GetNeedJob();
	if(iNeedJob != -1)
	{
		m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 466, "직업 : %s" ), JobInfo().GetName(iNeedJob, 0));
		m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
	}
	
	INDEX	ctNeed = 0;
	for( int iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
	{
		if( GetNeedItemIndex( iNeed ) == -1 )
			continue;
		
		const char* szItemName = _pNetwork->GetItemName( GetNeedItemIndex( iNeed ) );
		if( ctNeed == 0 )
		{
			m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 352, "%s %d개\n" ),
				szItemName,
				GetNeedItemCount( iNeed ) );
			ctNeed++;
		}
		else
		{
			CTString strTemp;
			strTemp.PrintF( _S( 352, "%s %d개\n" ),
				szItemName,
				GetNeedItemCount( iNeed ) );
			m_strNeedDesc[m_ctNeedDesc] += strTemp;
			ctNeed++;
		}
	}
	
	m_strNeedDesc[m_ctNeedDesc] += "\n";
	m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
}


// ------------- CQuestSystem ------------- 
CQuestSystem CQuestSystem::m_instance;

static int test_cnt = 0;
CQuestSystem::CQuestSystem()
: m_iQuestStaticDataLastIndex(0)
, m_aQuestStaticData( NULL )
, m_pCurrentRequest( NULL )
{
	++test_cnt;
}

CQuestSystem::~CQuestSystem()
{
	--test_cnt;
	delete m_pCurrentRequest;
	for(int i=0; i<m_vectorCurrentUserQuest.size(); ++i)
	{
		delete m_vectorCurrentUserQuest[i];
	}
	delete[] m_aQuestStaticData;
}

void CQuestSystem::ClearAllDynamicData()
{
	for( int i = 0; i < m_vectorCurrentUserQuest.size(); ++i )
	{
		delete m_vectorCurrentUserQuest[i];
	}
	m_vectorCurrentUserQuest.clear();
}

void CQuestSystem::ClearAllAllowList()
{
	for(int i=0; i<m_vectorQuestAllowList.size(); ++i)
	{
		m_vectorQuestAllowList[i] = QAT_ALLOW;
	}
}

CQuestDynamicData *CQuestSystem::GetDynamicDataByQuestIndex(INDEX idxQuest)
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetQuestIndex() == idxQuest)
		{
			return m_vectorCurrentUserQuest[i];
		}
	}
	return NULL;
}

INDEX CQuestSystem::GetVectorIndexByGiveNPCIndex( INDEX idxNPC )
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && 
			m_vectorCurrentUserQuest[i]->GetStartType() == QSTART_NPC &&
			m_vectorCurrentUserQuest[i]->GetStartData() == idxNPC)
		{
			return i;
		}
	}
	return -1;
}

INDEX CQuestSystem::GetVectorIndexByPrizeNPCIndex( INDEX idxNPC )
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetPrizeNPCIndex() == idxNPC )
		{
			return i;
		}
	}
	return -1;
}

CQuestDynamicData *CQuestSystem::Create(INDEX idxQuest)
{
	if(m_pCurrentRequest) DeleteCurrentRequest();
	m_pCurrentRequest = new CQuestDynamicData(GetStaticData(idxQuest));
	return m_pCurrentRequest;
}

INDEX CQuestSystem::AddCurrentRequest()
{
	m_vectorCurrentUserQuest.push_back(m_pCurrentRequest);
	m_pCurrentRequest = NULL;
	return m_vectorCurrentUserQuest[m_vectorCurrentUserQuest.size() - 1]->GetQuestIndex();
}

void CQuestSystem::DeleteCurrentRequest()
{
	delete m_pCurrentRequest;
	m_pCurrentRequest = NULL;
}

BOOL CQuestSystem::Remove(INDEX idxQuest)
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetQuestIndex() == idxQuest)
		{
			delete *(m_vectorCurrentUserQuest.begin() + i);
			m_vectorCurrentUserQuest.erase(m_vectorCurrentUserQuest.begin() + i);
			
			return TRUE;
		}
	}
	return FALSE;
}

void CQuestSystem::Load(const CTFileName &fnm, void (*progress)(FLOAT) )
{
	try
	{
		CTFileStream read;
		read.Open_t(fnm);
		
		INDEX lastIndex = 0;
		read >> lastIndex;
		if(m_aQuestStaticData) delete[] m_aQuestStaticData;
		m_aQuestStaticData = new CQuestStaticData[lastIndex+1];
		if(m_aQuestStaticData == NULL) throw "Not enough memory for load quest data";
		m_vectorQuestAllowList.resize(lastIndex + 1);
		memset(&m_vectorQuestAllowList[0], QAT_ALLOW, sizeof(BOOL)*(lastIndex+1));
		m_iQuestStaticDataLastIndex = lastIndex;
		
		for(INDEX i=0; i<=lastIndex; ++i)
		{
			INDEX index = -1;
			read >> index;
			
			//없는 index는 건너뛴다.
			for(INDEX j=i; j<index; ++j) m_aQuestStaticData[j].m_iIndex = -1;
			i = index;
			
			m_aQuestStaticData[i].m_iIndex = index;
			
			INDEX lenName = 0;
			read >> lenName;
			memset(&m_aQuestStaticData[i].m_szName, 0, sizeof(m_aQuestStaticData[i].m_szName));
			read.Read_t(&m_aQuestStaticData[i].m_szName, lenName);
			
			read >> m_aQuestStaticData[i].m_iType1;
			read >> m_aQuestStaticData[i].m_iType2;			
			read >> m_aQuestStaticData[i].m_iStartType;
			read >> m_aQuestStaticData[i].m_iStartData;
			//read >> m_aQuestStaticData[i].m_iNPCIndex;
			//			read >> m_aQuestStaticData[i].m_iGiveNPCIndex;
			read >> m_aQuestStaticData[i].m_iPrizeNPCIndex;

			read >> m_aQuestStaticData[i].m_iPreQuestIndex;
			read >> m_aQuestStaticData[i].m_iStartNPCZoneIndex;
			read >> m_aQuestStaticData[i].m_iPrizeNPCZoneIndex;

			read >> m_aQuestStaticData[i].m_iNeedEXP;
			read >> m_aQuestStaticData[i].m_iNeedMinLevel;
			read >> m_aQuestStaticData[i].m_iNeedMaxLevel;
			read >> m_aQuestStaticData[i].m_iNeedJob;
#ifdef PKPENALTY_QUEST
			read >> m_aQuestStaticData[i].m_iMinPenalty;
			read >> m_aQuestStaticData[i].m_iMaxPenalty;
#endif

			if(QSTART_NPC == m_aQuestStaticData[i].m_iStartType)
			{
				ASSERT(m_aQuestStaticData[i].m_iStartData != -1);
				mmap::value_type insertPairValue(m_aQuestStaticData[i].m_iStartData, m_aQuestStaticData[i].m_iIndex);
				m_mmapNPCToQuest.insert(insertPairValue);

				if (m_aQuestStaticData[i].m_iPreQuestIndex>0)
				{
					mmap::value_type insertPairReverseValue(m_aQuestStaticData[i].m_iPreQuestIndex, m_aQuestStaticData[i].m_iIndex);
					m_mmapPreQuestToCurQuest.insert(insertPairReverseValue);
				}
			}			

#			if MAX_MAX_NEED_ITEM < QUEST_MAX_CONDITION
#			error "Quest System : Max Need Item Count must be larger than Max Condition Count"
#			endif
			for(j=0; j<MAX_MAX_NEED_ITEM; ++j)
			{
				read >> m_aQuestStaticData[i].m_iNeedItemIndex[j];
				read >> m_aQuestStaticData[i].m_iNeedItemCount[j];
			}
			for(j = 0; j < QUEST_MAX_CONDITION;++j)
			{
				read >> m_aQuestStaticData[i].m_iConditionType[j];
				read >> m_aQuestStaticData[i].m_iConditionIndex[j];
				read >> m_aQuestStaticData[i].m_iConditionNum[j];
				
				for(int t = 0; t < QUEST_MAX_CONDITION_DATA; ++t)
				{
					read >> m_aQuestStaticData[i].m_iConditionData[j][t];
				}
			}
			
			for(j = 0; j  <QUEST_MAX_PRIZE; ++j)
			{
				read >> m_aQuestStaticData[i].m_iPrizeType[j];
				read >> m_aQuestStaticData[i].m_iPrizeIndex[j];
				read >> m_aQuestStaticData[i].m_iPrizeData[j];
			}

			read >> m_aQuestStaticData[i].m_bOptionPrize;
			for(j = 0; j  <QUEST_MAX_PRIZE; ++j)
			{
				read >> m_aQuestStaticData[i].m_iOptionPrizeType[j];
				read >> m_aQuestStaticData[i].m_iOptionPrizeIndex[j];
				read >> m_aQuestStaticData[i].m_iOptionPrizeData[j];
				read >> m_aQuestStaticData[i].m_iOptionPrizePlus[j];
			}

			INDEX lenDesc = 0;
			read >> lenDesc;
			memset(&m_aQuestStaticData[i].m_szDesc, 0, sizeof(m_aQuestStaticData[i].m_szDesc));
			read.Read_t(&m_aQuestStaticData[i].m_szDesc, lenDesc);
			
			lenDesc = 0;
			read >> lenDesc;
			memset(&m_aQuestStaticData[i].m_szDesc2, 0, sizeof(m_aQuestStaticData[i].m_szDesc2));
			read.Read_t(&m_aQuestStaticData[i].m_szDesc2, lenDesc);
			
			lenDesc = 0;
			read >> lenDesc;
			memset(&m_aQuestStaticData[i].m_szDesc3, 0, sizeof(m_aQuestStaticData[i].m_szDesc3));
			read.Read_t(&m_aQuestStaticData[i].m_szDesc3, lenDesc);

			// [090617: selo] 파티 타입 추가 ( 0 : 일반, 1 : 파티, 2 : 원정대 )
#ifdef NEW_QUESTBOOK
			read >> m_aQuestStaticData[i].m_iPartyScale;
#endif
			
			if(progress != NULL) (*progress)(FLOAT(i) / FLOAT(lastIndex));
		}
	}
	catch(char *szErr)
	{
		ThrowF_t("Error while loading Quest data, Reported message is \"%s.\" ", szErr);
	}
}

CQuestSystem::eNpcQuestType CQuestSystem::TestNPCForQuest(INDEX iNPCIndex)
{
	///현재 완료된(보상은 안받은) 모든 quest의 보상 npc index와 비교한다.
	///보상해 줄 수 있다면 NQT_CAN_PRIZE를 리턴.
	for(INDEX i=0; i<m_vectorCurrentUserQuest.size(); ++i)
	{
		if(m_vectorCurrentUserQuest[i]->IsQuestComplete()
		&& (m_vectorCurrentUserQuest[i]->GetPrizeNPCZoneIndex() == -1
			|| m_vectorCurrentUserQuest[i]->GetPrizeNPCZoneIndex() == g_slZone
			)
		&& m_vectorCurrentUserQuest[i]->GetPrizeNPCIndex() == iNPCIndex)
		{
			return NQT_CAN_PRIZE;
		}
	}

	///이 npc가 줄 수 있는 모든 quest를 찾는다.
	mmap::iterator iterLower = m_mmapNPCToQuest.lower_bound(iNPCIndex); 
	mmap::iterator iterUpper = m_mmapNPCToQuest.upper_bound(iNPCIndex);
	if(iterLower == m_mmapNPCToQuest.end()) return NQT_NONE;
	
	///수행 가능 조건을 검사해서 수행가능하면 NQT_HAVE_QUEST를 리턴.
	//if(iterUpper != m_mmapNPCToQuest.end()) ++iterUpper;
	for(mmap::iterator iter=iterLower; iter!=iterUpper; ++iter)
	{
		INDEX iQuestIndex = (*iter).second;
		///현재 수행 중인 퀘스트인지 확인한다.
		///m_vectorQuestAllowList에서 이미 수행하거나 포기한 퀘스트인지 검사한다.
		if (m_vectorQuestAllowList[ iQuestIndex ] == QAT_ALLOW)
		{
			ASSERT(m_aQuestStaticData[iQuestIndex].m_iStartType == QSTART_NPC);
			//start quest npc zone 검사
			if(m_aQuestStaticData[iQuestIndex].m_iStartNPCZoneIndex == -1
			|| m_aQuestStaticData[iQuestIndex].m_iStartNPCZoneIndex == g_slZone)
			{
				if(CanIDoQuest(iQuestIndex))
				{
					return NQT_HAVE_QUEST;
				}
			}
		}
	}

	///NQT_NONE을 리턴
	return NQT_NONE;
}

BOOL CQuestSystem::CanIDoQuest(INDEX iQuestIndex)
{
	if(iQuestIndex == -1) return FALSE;
	if(m_aQuestStaticData[iQuestIndex].m_iIndex == -1) return FALSE;

	CQuestStaticData &Quest = m_aQuestStaticData[iQuestIndex];

	///선행 퀘스트 조건 검사
	if(Quest.m_iPreQuestIndex != 0
	&& m_vectorQuestAllowList[ Quest.m_iPreQuestIndex ] != QAT_ALREADY_DONE)
	{
		return FALSE;
	}
	///경험치 검사
	if(Quest.m_iNeedMinLevel == _pNetwork->MyCharacterInfo.level
	&& Quest.m_iNeedEXP > _pNetwork->MyCharacterInfo.curExp)
	{
		return FALSE;
	}
	///레벨 검사
	if(Quest.m_iNeedMinLevel > _pNetwork->MyCharacterInfo.level
	|| Quest.m_iNeedMaxLevel < _pNetwork->MyCharacterInfo.level)
	{
		return FALSE;
	}
	///직업 검사
	if(Quest.m_iNeedJob != -1/*ALL*/ && _pNetwork->MyCharacterInfo.job != Quest.m_iNeedJob)
	{
		return FALSE;
	}
#ifdef PKPENALTY_QUEST	
	if ((Quest.m_iMinPenalty != 0 && Quest.m_iMaxPenalty != 0)) // min, max penalty가 모두 0이면 검사 하지 않는다.
	{	// pk 페널티 조건 검사
		if (_pNetwork->MyCharacterInfo.pkpenalty < Quest.m_iMinPenalty ||
			_pNetwork->MyCharacterInfo.pkpenalty > Quest.m_iMaxPenalty)
		{
			return FALSE;
		}
	}
#endif

	///필요 아이템 검사
	for(int i=0; i<MAX_MAX_NEED_ITEM; ++i)
	{
		if(Quest.m_iNeedItemIndex[i] != -1
		&& _pUIMgr->GetInventory()->GetItemCount(Quest.m_iNeedItemIndex[i]) < Quest.m_iNeedItemCount[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CQuestSystem::RefreshNPCQuestMark(INDEX iNPCIndex)
{
	if(iNPCIndex == -1) return;
	INDEX iMobCount = _pNetwork->ga_srvServer.srv_amtMob.Count();
	for(INDEX i=0; i<iMobCount; ++i)
	{
		CMobTarget &mt = _pNetwork->ga_srvServer.srv_amtMob[i];
		if(mt.mob_iType == iNPCIndex)
		{
			CEntity *penNPC = mt.mob_pEntity;
			if(penNPC && penNPC->GetModelInstance())
			{
				mt.mob_statusEffect.ChangeNPCQuestMark(
					&penNPC->GetModelInstance()->m_tmSkaTagManager
					, TestNPCForQuest(iNPCIndex));
			}
			return;
		}
	}
}

CTString CQuestSystem::MakeInfoForCondition(int iQuestIndex, int iConditionType, int iConditionIndex, int iCurrentCnt, int iConditionCnt)
{
	CTString strRet;
	if(iConditionType == QCONDITION_NPC)
	{
		CMobData &mob = _pNetwork->GetMobData(iConditionIndex);
		strRet.PrintF( _S( 1656, "%s 마리 %d/%d" ), mob.GetName(), iCurrentCnt, iConditionCnt);
	}
#ifdef PKPENALTY_QUEST	
	else if (iConditionType == QCONDITION_PC)
	{// PK 퀘스트
		strRet.PrintF( _S(3976, "PK 승리 %d/%d"), iCurrentCnt, iConditionCnt);
	}
#endif	
	else if(iConditionType == QCONDITION_ITEM || iConditionType == QCONDITION_ITEM_NORMAL)
	{
		CItemData &item = _pNetwork->GetItemData(iConditionIndex);
		strRet.PrintF( _S( 1657, "%s 개수 %d/%d" ), item.GetName(), iCurrentCnt, iConditionCnt);
	}
	// [090616: selo] 확장팩 퀘스트 신규 조건
	else if(iConditionType == QCONDITION_AREA)
	{
		strRet.PrintF( _s("%d/%d"), iCurrentCnt, iConditionCnt);
	}
	else if(iConditionType == QCONDITION_ITEMUSE)
	{
		CItemData &item = _pNetwork->GetItemData(iConditionIndex);
		strRet.PrintF( _S( 1657, "%s 개수 %d/%d" ), item.GetName(), iCurrentCnt, iConditionCnt);
	}

	return strRet;
}

INDEX CQuestSystem::SearchContactQuestIndex(INDEX iQuestIndex)
{
	INDEX iContactNpcIndex = 0;
	INDEX iContactQuest = 0;
	CQuestStaticData pContactData;

	mmap::iterator ItrQuestToNPC = m_mmapPreQuestToCurQuest.find(iQuestIndex);

	if (ItrQuestToNPC != m_mmapPreQuestToCurQuest.end())
	{
		iContactQuest = (*ItrQuestToNPC).second;
		iContactNpcIndex = m_aQuestStaticData[iContactQuest].m_iPrizeNPCIndex;
	}

	return iContactNpcIndex;
}