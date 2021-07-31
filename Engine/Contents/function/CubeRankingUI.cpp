#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "CubeRankingUI.h"

CCubeRankingUI::CCubeRankingUI()
	: m_pTextPoint(NULL)
	, m_pTextTitle(NULL)
{
	setInherit(false);

	m_lMypoint = 0;
	m_bGuild = FALSE;

	for (int i = 0; i < eRANK_MAX; ++i)
	{
		m_pListRank[i] = NULL;
		m_pGroup[i] = NULL;
	}
}
CCubeRankingUI::~CCubeRankingUI()
{

}

void CCubeRankingUI::initialize()
{
#ifndef		WORLD_EDITOR

	CUIBase* pBase = NULL;

	std::string str[eRANK_MAX] = {"cur", "prev"};
	std::string strTemp;
	for (int i = 0; i < eRANK_MAX; ++i)
	{
		strTemp = "group_";
		
		if (m_pGroup[i] = findUI((strTemp + str[i]).c_str()))
		{
			strTemp = "list_";
			strTemp += str[i];
			strTemp += "_rank";
			m_pListRank[i] = (CUIList*)m_pGroup[i]->findUI(strTemp.c_str());
		}
	}

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CCubeRankingUI::CubeRankClose, this));

	if (pBase = findUI("btn_ok"))
		pBase->SetCommandFUp(boost::bind(&CCubeRankingUI::CubeRankClose, this));

	if (pBase = findUI("drag_area"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pTextPoint = (CUIText*)findUI("text_point");
	m_pTextTitle = (CUIText*)findUI("text_title");

#endif		// WORLD_EDITOR
}

void CCubeRankingUI::CubeRankInit()
{
	m_lMypoint = 0;
	m_bGuild = FALSE;

	for (int i = 0; i < eRANK_MAX; i++ )
	{
		for (int j = 0; j < 5; j++)
		{
			m_stCubeRank[i][j].Init();
		}

		m_pListRank[i]->DeleteAllListItem();
	}
}

void CCubeRankingUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CCubeRankingUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CCubeRankingUI::CubeRankClose()
{
	CubeRankInit();
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_CUBERANK, FALSE );
}

void CCubeRankingUI::CreateCubeState(CNetworkMessage *istr, BOOL bGuild)
{
	CubeRankInit();

	LONG lMyPoint = 0, lInfoCubePoint = 0; // 자신이 소속된 길드 및 개인 포인트 , 각 리스트의 큐브 포인트
	BYTE InfoCount = 0, InfoRank = 0; // 리스트 갯수, 랭크 정보
	CTString strGuildName, strChaName; // 길드 이름, 길드 마스터 및 캐릭터 이름
	int		i;

	(*istr) >> lMyPoint;
	(*istr) >> InfoCount;

	m_lMypoint = lMyPoint;
	// 이번주 랭킹
	for( i = 0; i<InfoCount; ++i )
	{
		(*istr) >> InfoRank >> strGuildName >> strChaName >> lInfoCubePoint;

		m_stCubeRank[0][i].nRank = InfoRank;
		m_stCubeRank[0][i].strGuildName = strGuildName;
		m_stCubeRank[0][i].strChaName = strChaName;
		m_stCubeRank[0][i].lPoint = lInfoCubePoint;
		AddRankList(m_pListRank[eRANK_CUR], InfoRank, lInfoCubePoint, strChaName.str_String, strGuildName.str_String);
	}

	m_pListRank[eRANK_CUR]->UpdateList();

	(*istr) >> InfoCount;
	// 지난주 랭킹
	for( i = 0; i < InfoCount; ++i )
	{		
		(*istr) >> InfoRank >> strGuildName >> strChaName >> lInfoCubePoint;

		m_stCubeRank[1][i].nRank = InfoRank;
		m_stCubeRank[1][i].strGuildName = strGuildName;
		m_stCubeRank[1][i].strChaName = strChaName;
		m_stCubeRank[1][i].lPoint = lInfoCubePoint;
		AddRankList(m_pListRank[eRANK_PREV], InfoRank, lInfoCubePoint, strChaName.str_String, strGuildName.str_String);
	}

	m_pListRank[eRANK_PREV]->UpdateList();

	SetTitle(bGuild ? true : false);

	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder( UI_CUBERANK, TRUE );
}

void CCubeRankingUI::AddRankList( CUIList* list, BYTE rank, LONG point, char* c_name, char* g_name )
{
	if (list == NULL)
		return;

	CUIBase* pTmp = list->GetListItemTemplate();
	if (pTmp == NULL)
		return;

	list->AddListItem(pTmp->Clone());

	CUIBase* pItem = list->GetListItem(list->getListItemCount() - 1);
	if (pItem == NULL)
		return;
	
	CUIText* pText = NULL;
	char buf[4];

	if (pText = (CUIText*)pItem->findUI("text_rank"))
	{
		pText->SetText(CTString(itoa(rank, buf, 10)));
	}

	if (pText = (CUIText*)pItem->findUI("text_point"))
	{
		pText->SetText(CTString(itoa(point, buf, 10)));
	}

	if (pText = (CUIText*)pItem->findUI("text_guild"))
	{
		pText->SetText(CTString(g_name));
	}

	if (pText = (CUIText*)pItem->findUI("text_name"))
	{
		pText->SetText(CTString(c_name));
	}
}

void CCubeRankingUI::SetTitle( bool bGuild )
{
	if (m_pTextPoint == NULL || m_pTextTitle == NULL)
		return;

	CUIText* pText = NULL;
	CTString strTemp;
	CTString strPoint = UIMGR()->IntegerToCommaString(m_lMypoint);

	if (bGuild == true)
	{
		m_pTextTitle->SetText(CTString(_S(4388, "길드 큐브 포인트 현황")));
		strTemp.PrintF(_S(4389, "자신이 소속된 길드 큐브 포인트 : %s"), strPoint);
		m_pTextPoint->SetText(strTemp);

		for (int i = 0; i < eRANK_MAX; ++i)
		{
			if (pText = (CUIText*)m_pGroup[i]->findUI("text_guild"))
				pText->SetText(CTString(_S(3451, "길드명")));

			if (pText = (CUIText*)m_pGroup[i]->findUI("text_name"))
				pText->SetText(CTString(_S(4344, "길드 마스터")));
		}
	}
	else
	{
		m_pTextTitle->SetText(CTString(_S(4390, "개인 큐브 포인트 현황")));
		strTemp.PrintF(_S(4391, "자신의 개인 큐브 포인트 : %s"), strPoint);
		m_pTextPoint->SetText(strTemp);

		for (int i = 0; i < eRANK_MAX; ++i)
		{
			if (pText = (CUIText*)m_pGroup[i]->findUI("text_guild"))
				pText->SetText(CTString(_S(4392, "소속 길드")));

			if (pText = (CUIText*)m_pGroup[i]->findUI("text_name"))
				pText->SetText(CTString(_S(4393, "캐릭터 이름")));
		}
	}
}
