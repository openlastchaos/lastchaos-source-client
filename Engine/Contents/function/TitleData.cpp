#include "stdh.h"

#include <sstream>
#include "TitleData.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/CustomTitleUI.h>

extern INDEX	g_iCountry;

std::vector<UINT> CustomTitleData::m_vecFrontColor;
std::vector<UINT> CustomTitleData::m_vecBackColor;
std::vector<std::string> CustomTitleData::m_vecCustomTitleEffect;
std::map<int, TitleSystemMakeInfo*> CustomTitleData::m_mapCustomTitleItemInfo;

TitleNetwork::TitleNetwork()
{
	ResetTitleSelectInfo();
}

void TitleNetwork::RecvTitleMessage( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pTypeThirdBase* pPacket = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());
	CTString	strSysMessage, strTitle;
	CUIMsgBox_Info	MsgBoxInfo;

	strTitle = _S(4826, "호칭");

	switch(pPacket->thirdType)
	{
	case MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST:
		{
			pUIManager->GetNickName()->Open();
		}
		break;

	case MSG_EX_TITLE_SYSTEM_LIST_REP_EXIST_LIST:
		{
			ResponseClient::TitleSystemList* pRecv = reinterpret_cast<ResponseClient::TitleSystemList*>(istr->GetBuffer());

			pUIManager->GetNickName()->Open();	

			for (int i = 0 ; i < pRecv->title_count ; ++i)
			{
				pUIManager->GetNickName()->AddNickNameList( pRecv->data[i].index, pRecv->data[i].end_time, pRecv->data[i].custom_title_index);
			}

			pUIManager->GetNickName()->SortNickNameData();
		}
		break;

	case MSG_EX_TITLE_SYSTEM_ALREADY_HAVE_TITLE:
		{
			strSysMessage	=_S(4827, "이미 등록되어 있는 호칭입니다.");
		}
		break;

	case MSG_EX_TITLE_SYSTEM_INSERT_SUCCESS:
		{
			UpdateClient::TitleUse* pRecv = reinterpret_cast<UpdateClient::TitleUse*>(istr->GetBuffer());

			pUIManager->GetNickName()->AddNickNameList( pRecv->title_index, pRecv->end_time, pRecv->custom_title_index);
			pUIManager->GetNickName()->SortNickNameData();
			
			int iItemIndex = TitleStaticData::getData(pRecv->title_index)->GetItemIndex();

			if (IsGamigo(g_iCountry) == FALSE)
			{
				if( pUIManager->GetHelp()->IsBeginner() )
				{
					TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();
					if (pTitle != NULL)
						GAMEDATAMGR()->GetTitleNetwork()->SendTitleSelectReq(pRecv->title_index);

					return;
				}
			}
			
			if (CustomTitleData::IsKeepTitleInfo(pRecv->custom_title_index) == true)
				strSysMessage.PrintF(_S(4828, "%s 호칭을 추가하였습니다."), CustomTitleData::m_mapCustomTitleItemInfo[pRecv->custom_title_index]->name);
			else
				strSysMessage.PrintF(_S(4828, "%s 호칭을 추가하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));
		}
		break;

	case MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE:
		{
			strSysMessage	=_S(4829, "존재하지 않는 호칭 입니다.");
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_SELECT_SUCCESS:
		{
			ResponseClient::TitleSystem* pRecv = reinterpret_cast<ResponseClient::TitleSystem*>(istr->GetBuffer());
			
			if ( pRecv->char_index == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = pRecv->title_index;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();

				bool bCustomEffect = CustomTitleData::IsKeepTitleInfo(pRecv->custom_title_index);

				// 강신중일땐 이펙트 안붙임.
				if (bCustomEffect == false && _pNetwork->MyCharacterInfo.nEvocationIndex <= 0)
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetNickNameDamageEffect(_pNetwork->MyCharacterInfo.iNickType, CEntity::NICKNAME_ATTACH_EFFECT);

				int iItemIndex = TitleStaticData::getData(pRecv->title_index)->GetItemIndex();

				if( !pUIManager->GetHelp()->IsBeginner() )
				{
					if (bCustomEffect == true)
						strSysMessage.PrintF(_S(4830,"%s 호칭으로 설정 완료하였습니다."), CustomTitleData::m_mapCustomTitleItemInfo[pRecv->custom_title_index]->name);
					else
						strSysMessage.PrintF(_S(4830,"%s 호칭으로 설정 완료하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));					
				}
				else
					pUIManager->GetHelp()->SetBeginner( FALSE );

				pUIManager->GetNickName()->SetCancelButtonState( TRUE );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pRecv->char_index);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = pRecv->title_index;

					// 강신중일땐 패스.
					if (pTarget->cha_nTransformIndex <= 0)
					{
						pTarget->cha_itemEffect.DeleteNickEffect();
						pTarget->m_pEntity->SetNickNameDamageEffect(pTarget->cha_NickType, CEntity::NICKNAME_ATTACH_EFFECT);
					}
				}
			}		
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS:
		{
			ResponseClient::TitleSystem* pRecv = reinterpret_cast<ResponseClient::TitleSystem*>(istr->GetBuffer());

			if ( pRecv->char_index == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = 0;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();
				_pNetwork->MyCharacterInfo.stCustomTitle.Init();

				strSysMessage = _S(4831, "호칭이 정상적으로 취소되었습니다.");
				pUIManager->GetNickName()->SetCancelButtonState( FALSE );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pRecv->char_index);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = 0;
					pTarget->cha_itemEffect.DeleteNickEffect();
					pTarget->cha_CustomTitle.Init();
				}
			}
			break;
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_DELETE_SUCCESS:
		{
			ResponseClient::TitleSystem* pRecv = reinterpret_cast<ResponseClient::TitleSystem*>(istr->GetBuffer());

			pUIManager->GetNickName()->DelNickNameList( pRecv->title_index );
			//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
			int iItemIndex = TitleStaticData::getData(pRecv->title_index)->GetItemIndex();

			if (CustomTitleData::IsKeepTitleInfo(pRecv->custom_title_index) == true)
				strSysMessage.PrintF(_S(4832, "정상적으로 %s 호칭을 삭제하였습니다."), CustomTitleData::m_mapCustomTitleItemInfo[pRecv->custom_title_index]->name);
			else
				strSysMessage.PrintF(_S(4832, "정상적으로 %s 호칭을 삭제하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_EXPIRED:
		{
			ResponseClient::TitleSystem* pRecv = reinterpret_cast<ResponseClient::TitleSystem*>(istr->GetBuffer());

			if ( pRecv->char_index == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = 0;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();
				pUIManager->GetNickName()->DelNickNameList( pRecv->title_index );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pRecv->char_index);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = 0;
					pTarget->cha_itemEffect.DeleteNickEffect();
				}
			}
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_CHANGE_TIME:
		{
			ResponseClient::TitleSystemForceChangeTime* pRecv = reinterpret_cast<ResponseClient::TitleSystemForceChangeTime*>(istr->GetBuffer());

			pUIManager->GetNickName()->ChangeTitleTime( pRecv->title_index, pRecv->end_time );
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_MAKE:
		{
			ResponseClient::TitleSystemMake* pRecv = reinterpret_cast<ResponseClient::TitleSystemMake*>(istr->GetBuffer());

			CustomTitleData::AddCustomItemInfo(&pRecv->info);
			
			if (pUIManager->GetCustomTitle()->IsVisible() != FALSE)
				pUIManager->GetCustomTitle()->closeUI();
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_MAKE_INFO_LIST:
		{
			UpdateClient::TitleSystemMakeList* pUpdate = reinterpret_cast<UpdateClient::TitleSystemMakeList*>(istr->GetBuffer());
			
			for (int i = 0; i < pUpdate->count; ++i)
			{
				CustomTitleData::AddCustomItemInfo(&pUpdate->info[i]);
			}
		}
		break;

	case MSG_EX_TITLE_SYSTEM_TITLE_USER_INFO:
		{
			UpdateClient::TitleSystemUserInfo* pUpdate = reinterpret_cast<UpdateClient::TitleSystemUserInfo*>(istr->GetBuffer());

			CTString strEffectName;
			bool bInit = true;

			if (pUpdate->effect >= 0 && pUpdate->effect < DEF_EFFECT_MAX)
			{
				bInit = false;
				strEffectName = CustomTitleData::m_vecCustomTitleEffect[pUpdate->effect].c_str();			
			}

			if ( pUpdate->char_index == _pNetwork->MyCharacterInfo.index )
			{
				if (bInit == true)
				{
					_pNetwork->MyCharacterInfo.iNickType = 0;
					_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();
					_pNetwork->MyCharacterInfo.stCustomTitle.Init();
					return;
				}

				_pNetwork->MyCharacterInfo.iNickType = DEF_DUMMY_TITLE_INDEX;
				_pNetwork->MyCharacterInfo.stCustomTitle.nFrontColor = pUpdate->color;
				_pNetwork->MyCharacterInfo.stCustomTitle.nBackColor = pUpdate->background;
				_pNetwork->MyCharacterInfo.stCustomTitle.nEffectIdx = pUpdate->effect;
				memcpy(_pNetwork->MyCharacterInfo.stCustomTitle.name , pUpdate->name, CUSTOM_TITLE_NAME_LENGTH);
				_pNetwork->MyCharacterInfo.stCustomTitle.name[CUSTOM_TITLE_NAME_LENGTH - 1] = '\0';

				// 강신 중일땐 이펙트 안붙임
				if (_pNetwork->MyCharacterInfo.nEvocationIndex <= 0)
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCustomTitleEffect(strEffectName);
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pUpdate->char_index);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					if (bInit == true)
					{
						pTarget->cha_NickType = 0;
						pTarget->cha_itemEffect.DeleteNickEffect();
						pTarget->cha_CustomTitle.Init();
						return;
					}

					pTarget->cha_NickType = DEF_DUMMY_TITLE_INDEX;
					pTarget->cha_CustomTitle.nFrontColor = pUpdate->color;
					pTarget->cha_CustomTitle.nBackColor = pUpdate->background;
					pTarget->cha_CustomTitle.nEffectIdx = pUpdate->effect;
					memcpy(pTarget->cha_CustomTitle.name , pUpdate->name, CUSTOM_TITLE_NAME_LENGTH);
					pTarget->cha_CustomTitle.name[CUSTOM_TITLE_NAME_LENGTH - 1] = '\0';

					// 강신 중일땐 이펙트 안붙임
					if (pTarget->cha_nTransformIndex <= 0)
						pTarget->m_pEntity->SetCustomTitleEffect(strEffectName);
				}
			}		
		}
		break;

	case MSG_EX_TITLE_SYSTEM_DELETE_INFO:
		{
			UpdateClient::TitleSystemDeleteInfo* pUpdate = reinterpret_cast<UpdateClient::TitleSystemDeleteInfo*>(istr->GetBuffer());
			CustomTitleData::DeleteCustomItemInfo(pUpdate->title_index);
		}
		break;
	}

	if (strSysMessage != CTString(""))
	{
		MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_OK,UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString(strSysMessage);
		pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}

void TitleNetwork::SendTitleListReq()
{
	CNetworkMessage nm;
	RequestClient::TitleSystemList* packet = reinterpret_cast<RequestClient::TitleSystemList*>(nm.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_LIST_REQ;
	nm.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew(nm);
}

void TitleNetwork::SendTitleSelectReq( int nTitleIdx )
{
	CNetworkMessage nm;
	RequestClient::TitleSystemSelect* packet = reinterpret_cast<RequestClient::TitleSystemSelect*>(nm.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ;
	packet->title_index = nTitleIdx;
	nm.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew(nm);
}

void TitleNetwork::SendTitleCancelReq( int nTitleIdx )
{
	CNetworkMessage nm;
	RequestClient::TitleSystemCancel* packet = reinterpret_cast<RequestClient::TitleSystemCancel*>(nm.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ;
	packet->title_index = nTitleIdx;
	nm.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew(nm);
}

void TitleNetwork::SendTitleDeleteReq( int nTitleIdx )
{
	CNetworkMessage nm;
	RequestClient::TitleSystemDelete* packet = reinterpret_cast<RequestClient::TitleSystemDelete*>(nm.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ;
	packet->title_index = nTitleIdx;
	nm.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew(nm);
}

void TitleNetwork::SendTitleMake( int tab, int invenIdx, int virIdx)
{
	CNetworkMessage nm;
	RequestClient::TitleSystemMake* packet = reinterpret_cast<RequestClient::TitleSystemMake*>(nm.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_MAKE;
	packet->tab = tab;
	packet->inven_index = invenIdx;
	packet->vIndex = virIdx;
	packet->color = m_nSelectFrontColorIdx;
	packet->background_color = m_nSelectBackColorIdx;
	packet->effect = m_nSelectEffectIdx;
	strcpy(packet->name, m_strCustomTitleName);
	packet->name[CUSTOM_TITLE_NAME_LENGTH - 1] = '\0';
	nm.setSize( sizeof(*packet) );
	_pNetwork->SendToServerNew(nm);
}

void TitleNetwork::ResetTitleSelectInfo()
{
	m_nSelectFrontColorIdx = 0;
	m_nSelectBackColorIdx = 0;
	m_nSelectEffectIdx = 0;
	CTString strTemp = UIMGR()->GetString(7092);
	setCustomTitleName(strTemp.str_String);
}

bool TitleStaticData::loadEx(const char* FileName)
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stTitle* pdata = new stTitle[_nSize];
	fread(pdata, sizeof(stTitle) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		TitleStaticData* ptmp = new TitleStaticData;
		memcpy(ptmp, &pdata[i], sizeof(stTitle));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}

		if (ptmp != NULL)
			_vecdata.push_back(ptmp);
	}

	m_dummy = new TitleStaticData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stTitle));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

bool CustomTitleData::LoadCustomTitleDataFromFile( const char* fileName )
{
	FILE *fp		= NULL;
	if ((fp = fopen(fileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "customTitle File is not Exist.", "error!", MB_OK);
		return false;
	}
	fflush(fp);

	int iReadBytes	= 0;

#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADTITLEDATA(t, c) {t = new stCustomTitleString[c]; iReadBytes = fread(t, sizeof(stCustomTitleString) * c, 1, fp);}

	int optionCount = 0;
	int nFrontCount = 0;
	int nBackCount = 0;
	int nEffectCount = 0;

	// 타이틀 옵션
	LOADINT(optionCount);
	
	if(optionCount > 0)
	{
		stCustomTitleOption* tempLoadData = new stCustomTitleOption[optionCount];
		iReadBytes = fread(tempLoadData, sizeof(stCustomTitleOption) * optionCount, 1, fp);
		SAFE_ARRAY_DELETE(tempLoadData);
	}

	if( iReadBytes <= 0 )
	{
		fclose(fp);
		return false;
	}

	stCustomTitleString* tempFrontColor = NULL;
	stCustomTitleString* tempBackColor = NULL;
	stCustomTitleString* tempEffectName = NULL;

	// Front Color
	LOADINT(nFrontCount);
		
	if(nFrontCount > 0)
		LOADTITLEDATA(tempFrontColor, nFrontCount);

	if( iReadBytes <= 0 )
	{
		SAFE_ARRAY_DELETE(tempFrontColor);
		fclose(fp);
		return false;
	}

	// Back Color
	LOADINT(nBackCount);	

	if(nBackCount > 0)
		LOADTITLEDATA(tempBackColor, nBackCount);

	if( iReadBytes <= 0 )
	{
		SAFE_ARRAY_DELETE(tempFrontColor);
		SAFE_ARRAY_DELETE(tempBackColor);
		fclose(fp);
		return false;
	}

	// Effect name
	LOADINT(nEffectCount);

	if(nEffectCount > 0)
		LOADTITLEDATA(tempEffectName, nEffectCount);

#undef LOADINT
#undef LOADTITLEDATA

	fclose(fp);

	int i;

	// Front Color
	for ( i = 0; i < nFrontCount; ++i)
	{
		UINT nTempInt = ColorStringToInt(tempFrontColor[i].strValue);
		m_vecFrontColor.push_back(nTempInt);
	}

	// Back Color
	for ( i = 0; i < nBackCount; ++i)
	{
		UINT nTempInt = ColorStringToInt(tempBackColor[i].strValue);
		m_vecBackColor.push_back(nTempInt);
	}

	// Effect name
	for ( i = 0; i < nEffectCount; ++i)
	{
		m_vecCustomTitleEffect.push_back(tempEffectName[i].strValue);
	}

	SAFE_ARRAY_DELETE(tempFrontColor);
	SAFE_ARRAY_DELETE(tempBackColor);
	SAFE_ARRAY_DELETE(tempEffectName);
	return true;
}

UINT CustomTitleData::ColorStringToInt( std::string ColorString )
{
	UINT ColorValue;
	std::stringstream sstream(ColorString);
	sstream >> std::hex >> ColorValue;
	return ColorValue;
}

void CustomTitleData::AddCustomItemInfo(TitleSystemMakeInfo* pInfo )
{
	if (IsKeepTitleInfo(pInfo->custom_title_index) == false)
	{
		TitleSystemMakeInfo* pTitleInfo = new TitleSystemMakeInfo;
		memcpy(pTitleInfo, pInfo, sizeof(TitleSystemMakeInfo));
		m_mapCustomTitleItemInfo.insert(std::make_pair(pInfo->custom_title_index, pTitleInfo));
	}	
}

void CustomTitleData::clearCustomItemInfo()
{
	std::map<int, TitleSystemMakeInfo*>::iterator iter = m_mapCustomTitleItemInfo.begin();
	std::map<int, TitleSystemMakeInfo*>::iterator iter_end = m_mapCustomTitleItemInfo.end();

	for (; iter != iter_end; ++iter)
	{
		SAFE_DELETE((*iter).second);
	}

	m_mapCustomTitleItemInfo.clear();
}

bool CustomTitleData::IsKeepTitleInfo( int index )
{
	std::map<int, TitleSystemMakeInfo*>::iterator iter = m_mapCustomTitleItemInfo.find(index);
	std::map<int, TitleSystemMakeInfo*>::iterator iter_end = m_mapCustomTitleItemInfo.end();

	if (iter != iter_end)
		return true;
	return false;
}

void CustomTitleData::DeleteCustomItemInfo( int index )
{
	std::map<int, TitleSystemMakeInfo*>::iterator iter = m_mapCustomTitleItemInfo.find(index);
	std::map<int, TitleSystemMakeInfo*>::iterator iter_end = m_mapCustomTitleItemInfo.end();

	if (iter != iter_end)
	{
		SAFE_DELETE((*iter).second);
		m_mapCustomTitleItemInfo.erase(iter);
	}
}

