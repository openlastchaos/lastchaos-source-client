#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "SingleBattleUI.h"

#define TOP_SPACE					241
#define RIGHT_SPACE					12
#define BOX_WIDTH					118

CSingleBattleUI::CSingleBattleUI()
	: m_nOldCount(0)
{
	for (int i = 0; i < eIMG_FONT_MAX; ++i)
		m_pImgFont[i] = NULL;
}

void CSingleBattleUI::openUI()
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);
	_resetUI();

	UIMGR()->RearrangeOrder( UI_SINGLE_BATTLE, TRUE );
}

void CSingleBattleUI::closeUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	Hide(TRUE);
	SetVisible(FALSE);

	pUIManager->RearrangeOrder( UI_SINGLE_BATTLE, FALSE );
}

void CSingleBattleUI::initialize()
{
#ifndef		WORLD_EDITOR
	std::string strId[eIMG_FONT_MAX] = { "if_killcnt", "if_time" };

	for (int i = 0; i < eIMG_FONT_MAX; ++i)
		m_pImgFont[i] = (CUIImageFont*)findUI(strId->c_str());
#endif	//	WORLD_EDITOR
}

void CSingleBattleUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - RIGHT_SPACE - BOX_WIDTH, TOP_SPACE);
}

void CSingleBattleUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CSingleBattleUI::_resetUI()
{
	_setKillCountString();
}

void CSingleBattleUI::_setKillCountString( int nKillCount )
{
	if (m_pImgFont[eIMG_FONT_KILL_COUNT] != NULL)
	{
		CTString strTemp;
		strTemp.PrintF("%d", nKillCount);
		m_pImgFont[eIMG_FONT_KILL_COUNT]->setString(strTemp.str_String);
		m_nOldCount = nKillCount;
	}
}

void CSingleBattleUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (GetHide() == TRUE)
		return;

	int nKillCount = _pNetwork->wo_dwKilledEnemyCount;

	if (m_nOldCount != nKillCount)
		_setKillCountString(nKillCount);
}

