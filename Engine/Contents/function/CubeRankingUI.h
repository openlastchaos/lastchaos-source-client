#ifndef __DEF_CUBERANKING_UI_H__
#define __DEF_CUBERANKING_UI_H__

class CCubeRankingUI : public CUIWindow
{
private:
	struct sCubeRank
	{
		sCubeRank()
		{
			Init();
		}
		void Init()
		{
			nRank = 0;
			strGuildName = "";
			strChaName = "";
			lPoint = 0;
		}
		BYTE nRank;
		CTString strGuildName;
		CTString strChaName;
		LONG lPoint;
	};

	enum eRANK_VIEW
	{
		eRANK_CUR = 0,
		eRANK_PREV,
		eRANK_MAX
	};

	CUIBase* m_pGroup[eRANK_MAX];
	CUIList* m_pListRank[eRANK_MAX];
	CUIText* m_pTextPoint;
	CUIText* m_pTextTitle;

	LONG		m_lMypoint;	
	sCubeRank	m_stCubeRank[eRANK_MAX][5];
	BOOL		m_bGuild;

	void AddRankList(CUIList* list, BYTE rank, LONG point, char* c_name, char* g_name);
	void SetTitle(bool bGuild);
public:
	CCubeRankingUI();
	~CCubeRankingUI();
	void initialize();
	// Adjust position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void CubeRankInit();
	void CubeRankClose();
	void CreateCubeState(CNetworkMessage *istr, BOOL bGuild);
};

#endif // __DEF_CUBERANKING_UI_H__