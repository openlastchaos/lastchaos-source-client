// ----------------------------------------------------------------------------
//  File : UIShop.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISHOP_H_
#define	UISHOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define size of shop
#define SHOP_MAIN_WIDTH					216
#define	SHOP_MAIN_HEIGHT				305

// ----------------------------------------------------------------------------
// Name : CUIShop
// Desc :
// ----------------------------------------------------------------------------
class CUIShop : public CUIWindow
{
protected:
	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;
	int					m_nNpcIndex;
	int					m_nNpcVirIdx;

public:
	CUIShop();
	~CUIShop();

	void	OpenShop( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild);
};

#endif // UISHOP_H_

