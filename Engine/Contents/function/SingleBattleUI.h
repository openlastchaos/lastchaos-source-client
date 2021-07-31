#ifndef	SINGLE_BATTLE_UI_H_
#define	SINGLE_BATTLE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImageFont;

class CSingleBattleUI : public CUIWindow
{
public:
	CSingleBattleUI();
	
	void openUI();
	void closeUI();

	void initialize();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

private:
	void _resetUI();
	void _setKillCountString(int nKillCount = 0);

	enum 
	{
		eIMG_FONT_KILL_COUNT = 0,
		eIMG_FONT_TIME,
		eIMG_FONT_MAX
	};

	CUIImageFont* m_pImgFont[eIMG_FONT_MAX];
	int m_nOldCount;
};

#endif	// SINGLE_BATTLE_UI_H_

