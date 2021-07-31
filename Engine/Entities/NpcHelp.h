#ifndef INCL_NPCHELP_H
#define INCL_NPCHELP_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>
#include <vector>


class ENGINE_API CNpcHelp
{
private:
	typedef struct _tagNpcList
	{
		int zone_index;
		int npc_index;
		char npc_name[50];
		char npc_explan[1024];
	
	}TNpcList, *PTNpcList;
	
protected:
	
public:
	CNpcHelp();
	~CNpcHelp();


	TNpcList			m_NpcList;
	int					m_nHelpNpcIndex;

	void		SetData(int iKeeperIdx, const char* pszShopName, int iSellRate, int iBuyRate);
	static int	LoadNpcListFromFile(CStaticArray<CNpcHelp> &apNpcList, const char* FileName);
};
#endif