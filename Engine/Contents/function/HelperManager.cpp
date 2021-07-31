#include "stdh.h"
#include <Engine/Interface/UIWindow.h>
#include "HelperManager.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>
#include "HelperUI.h"
#include "Helper_Student.h"
#include "Helper_GuardianInfoUI.h"

CHelperManager::CHelperManager()
{

}

CHelperManager::~CHelperManager()
{
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
	}
	if ( !m_vectorTeacherInfoList.empty() )
	{
		m_vectorTeacherInfoList.clear();
	}
	if ( !m_vectorStudentInfoList.empty() )
	{
		m_vectorStudentInfoList.clear();
	}
}

void CHelperManager::OpenDialog()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxLExist( MSGLCMD_HELPER_REQ ) == TRUE)
		return;	

	const int iLevel	= _pNetwork->MyCharacterInfo.level;

	if (iLevel >= 50)
	{
		if (pUIManager->GetHelper_GuardianInfo()->GetHide() == FALSE)
			return;
	}
	else
	{
		if (pUIManager->GetHelper_Student()->GetHide() == FALSE)
			return;
	}

	//ResetHelper();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYSTUDENT );

	CTString strTitle;
	CTString strMessage;

	

	if( iLevel >=  50 )
	{
		strTitle = _S( 1140, "견습생 관리" );			
		// Create message box
		pUIManager->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1141, "게임을 시작하는 유저들을 도와주기 위해서 후견인을 등록할 수 있습니다." \
			"후견인 등록을 하면, 신규 유저가 후견인 리스트를 볼 때 [%s]님의 이름이 보이게 됩니다." ), _pNetwork->MyCharacterInfo.name );		
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		strMessage.PrintF( _S( 5045, "견습생이 50레벨이 되는 동안, 다양한 보상을 얻을 수 있습니다." ), LIMIT_GUARDIAN_APP_LEVEL );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );		

		strMessage.PrintF( "" );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_TEACHER );		
	}
	else
	{
		strTitle = _S( 1147, "후견인 관리" );			
		// Create message box

		// 후견인이 있을 경우 후견인 정보창을 띄운다.
		if (_pNetwork->MyCharacterInfo.lTeacherIndex < 0)
			pUIManager->CreateMessageBoxL( strTitle, UI_HELPER_STUDENT, MSGLCMD_HELPER_REQ );
		else	// 후견인이 없다면 후견인 리스트 ui를 띄운다.
			pUIManager->CreateMessageBoxL( strTitle, UI_HELPER_GUARDIAN_INFO, MSGLCMD_HELPER_REQ );

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1148, "후견인은 [%s]님을 %d레벨까지 성장하는데 도움을 주는 사람을 이야기합니다." \
			"후견인 리스트에서 [%s]님께 도움을 줄 수 있는 사람을 찾아보세요." )
			, _pNetwork->MyCharacterInfo.name, LIMIT_GUARDIAN_APP_LEVEL, _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		strMessage.PrintF( _S( 5044, "최초 선정까지 총 3번을 선정할 수가 있으며, 30레벨이 넘으면 후견인을"\
			"선정 할수가 없으니 참고하시기 바랍니다." ), _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_STUDENT );		
	}

	pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE, _S( 880, "취소하기." ) );	
}

void CHelperManager::PrepareOpen()
{
	CUIManager* pUIManager = UIMGR();

	if (pUIManager->GetHelper()->GetHide() == TRUE &&
		pUIManager->GetHelper_Student()->GetHide() == TRUE &&
		pUIManager->GetHelper_GuardianInfo()->GetHide() == TRUE)
		return;

	const int iLevel	= _pNetwork->MyCharacterInfo.level;

	pUIManager->GetHelper()->CloseUI();
	pUIManager->GetHelper_Student()->CloseUI();
	pUIManager->GetHelper_GuardianInfo()->CloseUI();

	// 후견인
	if( iLevel >=  50 )	
	{
		pUIManager->GetHelper()->OpenUI();
	}
	// 견습생
	else	
	{
		// 견습생인데 후견인이 있는 경우
		if( _pNetwork->MyCharacterInfo.lTeacherIndex > 0 )
		{
			pUIManager->GetHelper_GuardianInfo()->OpenUI();
		}
		// 없는 경우.
		else
		{
			pUIManager->GetHelper_Student()->OpenUI();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddToHelperList()
// Desc : 멤버 목록에 추가
// ----------------------------------------------------------------------------
void CHelperManager::AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter == m_vectorHelperList.end() )
	{
		if( m_vectorHelperList.size() <= HELPER_MAX_STUDENTS )
		{
			sHelperMember TempHelperMember;
			TempHelperMember.lIndex			= lIndex;
			TempHelperMember.strName		= strName;
			TempHelperMember.iLevel			= iLevel;
			TempHelperMember.iJob			= iJob;
			TempHelperMember.iJob2			= iJob2;
			m_vectorHelperList.push_back(TempHelperMember);
		}
	}
}

void CHelperManager::AddToStudentInfoList(LONG index, const char *szFirstDate, const char *szFinalDate)
{
	if(m_vectorHelperList.size() == m_vectorStudentInfoList.size() + 1)
	{
		sStudentInfo info;
		info.index = index;
		strcpy(info.szFirstDate, szFirstDate);
		strcpy(info.szFinalDate, szFinalDate);
		m_vectorStudentInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorStudentInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : DelFromHelperList()
// Desc : 멤버 목록에서 제거
// ----------------------------------------------------------------------------
void CHelperManager::DelFromHelperList( LONG lIndex )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		int dist = std::distance(m_vectorHelperList.begin(), iter);
		m_vectorHelperList.erase( iter );
		if(m_vectorTeacherInfoList.size() > 0)
		{
			m_vectorTeacherInfoList.erase(m_vectorTeacherInfoList.begin() + dist);
		}
		else if(m_vectorStudentInfoList.size() > 0)
		{
			m_vectorStudentInfoList.erase(m_vectorStudentInfoList.begin() + dist);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeHelperLevel()
// Desc : 멤버의 레벨을 변경함.
// ----------------------------------------------------------------------------
void CHelperManager::ChangeHelperLevel( LONG lIndex, int iLevel )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		(*iter).iLevel = iLevel;
	}
}

void CHelperManager::ClearHelperList()
{
	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
		m_vectorTeacherInfoList.clear();
		m_vectorStudentInfoList.clear();
	}
}


void CHelperManager::AddToTeacherInfoList(LONG index, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail, SBYTE StartPlayTime, SBYTE EndPlayTime)
{
	if(m_vectorHelperList.size() == m_vectorTeacherInfoList.size() + 1)
	{
		sTeacherInfo info;
		info.index = index;
		info.nFame = fame;
		info.nCntTeachingStudent = cntTeaching;
		info.nCntCompleteStudent = cntComplete;
		info.nCntFailStudent = cntFail;
		info.sbStartPlayTime= StartPlayTime;
		info.sbEndPlayTime	= EndPlayTime;
		m_vectorTeacherInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorTeacherInfoList.size());
}
