#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include "StageIntro.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Loading.h>

void CStageIntro::Init()
{
	g_slZone = -1; 
	g_bFirstIntoWorld = TRUE;
	// ... cpp2angel End 
	CUIManager* pUIManager = CUIManager::getSingleton();
	CLoadingImage::getSingleton()->InitLoadingData();
	pUIManager->GetGame()->LoginGame(LOGIN_WORLD);

	_pNetwork->m_ubGMLevel			= 0;
	_pNetwork->m_bSingleMode		= FALSE;
}

void CStageIntro::Release()
{
	_pNetwork->bMoveCharacterSelectUI = FALSE;
	CLoadingImage::getSingleton()->Hide(TRUE);
}

void CStageIntro::Run()
{

}
