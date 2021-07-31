#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "RoyalrumbleUI.h"

CUIRoyalRumbleIcon::CUIRoyalRumbleIcon()
    : m_pSpriteAni(NULL)
    , m_pImgFont(NULL)
{
	setInherit(false);
}

void CUIRoyalRumbleIcon::ShowRoyalRumbleIcon(BOOL bShow)
{
    CUIManager::getSingleton()->RearrangeOrder(UI_ROYALRUMBLE_ICON, bShow);
	Hide(!bShow);
}

void CUIRoyalRumbleIcon::initialize()
{
	// 윈도우 전체가 Move 기준이다
	{
		// TitleRect 는 원래 Child Control 기준이므로, x, y 위치는 0, 0 으로 넣어준다.
		int l, t, r, b;
		l = 0;
		t = 0;
		r = l + GetWidth();
		b = t + GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pImgFont = (CUIImageFont*)findUI("imgfont");
}

void CUIRoyalRumbleIcon::SetLeftCount(const INDEX iLeftCount)
{
    ShowRoyalRumbleIcon(TRUE);

	if (m_pImgFont != NULL)
	{
		CTString tmp;
		tmp.PrintF("%d", iLeftCount);
		m_pImgFont->setString(tmp);
	}
}
