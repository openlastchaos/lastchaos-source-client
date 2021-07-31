#ifndef		_ROYAL_RUMBLE_UI_H_
#define		_ROYAL_RUMBLE_UI_H_

class CUIWindow;

class CUIRoyalRumbleIcon : public CUIWindow
{
public:
	CUIRoyalRumbleIcon();

	void				initialize();

	void				SetLeftCount( const INDEX iLeftCount );
	void				ShowRoyalRumbleIcon( BOOL bShow );

private:
	CUISpriteAni*		m_pSpriteAni;
	CUIImageFont*		m_pImgFont;
};

#endif		// _ROYAL_RUMBLE_UI_H_