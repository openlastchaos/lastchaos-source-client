#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "PartyAutoUIReg.h"
#include "CommunityDefines.h"


CUIPartyAutoReg::CUIPartyAutoReg()
{

}

void CUIPartyAutoReg::initialize()
{
#ifndef		WORLD_EDITOR
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoReg::close, this));

	pBtn = (CUIButton*)findUI("btn_normal");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoReg::type_normal, this));

	pBtn = (CUIButton*)findUI("btn_battle");

	if (pBtn != NULL)
	{
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoReg::type_battle, this));

//#ifdef DISABLE_PARTY_TYPE_BATTLE
		extern INDEX g_iCountry;
		if (IsGamigo(g_iCountry) == TRUE)
			pBtn->SetEnable(FALSE);
//#endif	// DISABLE_PARTY_TYPE_BATTLE
	}
#endif	// WORLD_EDITOR
}

void CUIPartyAutoReg::open()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_PARTYAUTO_REG, TRUE);
}

void CUIPartyAutoReg::close()
{
	SetVisible(FALSE);
	Hide(TRUE);
}

void CUIPartyAutoReg::type_normal()
{
	_pNetwork->SendPartyRegistNormal(PT_PEACEEVER);
	close();
}

void CUIPartyAutoReg::type_battle()
{
	_pNetwork->SendPartyRegistNormal(PT_ATTACK);
	close();
}
