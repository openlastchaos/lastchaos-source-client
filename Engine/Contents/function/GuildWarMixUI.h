#ifndef __GUILD_WAR_MIX_H__
#define __GUILD_WAR_MIX_H__

#define GW_MIX_ITEM_SLOT_COUNT			3

class CGuildWarMixUI : public CUIWindow
{
public:
	CGuildWarMixUI();
	~CGuildWarMixUI();
	
	void initialize();
	ENGINE_API	void	OpenGWMix();
	void CloseGWMix();

	void MixRep( SBYTE sbResult );
	void SetMixItem ();
	void SendMixReq();
	void ResetMixItem (int nIdx);
	void ResetMixItem (CUIIcon* pIcon);

	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
private:
	CUIButton*	m_pBtnOk;							// OK button
	CUIIcon*	m_pIconsSlot[GW_MIX_ITEM_SLOT_COUNT];					// Slot item button

	int	m_nCurItemCount;
	void setBtnEnable(bool b)	{ if (m_pBtnOk)	m_pBtnOk->SetEnable(b ? TRUE : FALSE);	}

	CUIIcon* GetItemSlot ( int nIndex )
	{
		if ( nIndex < 0 && nIndex >= GW_MIX_ITEM_SLOT_COUNT )
		{
			return NULL;
		}

		return m_pIconsSlot[nIndex];
	}
};
#endif // __GUILD_WAR_MIX_H__