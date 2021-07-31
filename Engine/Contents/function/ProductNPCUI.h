#ifndef		_UI_PRODUCT_NPC_H_
#define		_UI_PRODUCT_NPC_H_

class CUIProductNPC: public CUIWindow
{
protected:
	INDEX			m_nSealType;
	FLOAT			m_fNpcX, m_fNpcZ;
	int				m_nNpcVirIdx;
	int				m_nSealNum;		// 증표획득 확인 창에 사용할 값 저장

public:
	CUIProductNPC();
	~CUIProductNPC();

	void	initialize();

	void	Reset();

	void	OpenProductList(INDEX sealtype);
	void	OpenProductNPC(int iMobIndex, int iMobVirIdx, FLOAT npcx, FLOAT npcz);

	void	CloseProductNPC();

	// Messages
	void MsgBoxLCommand( int nCommandCode, int nResult );
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput );
	void SendFactoryLearn(int nIndex);

	void callback_select(int nsub, int idx, int fac_idx, COLOR col);
	void callback_over(int nsub, int idx, bool bover, COLOR col);

private:
	bool set_data();
	void _callback_ok();
	CUITree* getTree(int nsub, int idx);
	void set_color(CUITree* pTree, COLOR col);

	typedef	std::vector<CMakeItemData*>	vec_makeitem;
	typedef vec_makeitem::iterator		vec_iter;

	CUITree*		m_pTreeTemp;
	CUITree*		m_pTree;

	CUIBase*		m_pTreePos;
	CUITreeItem*	m_pTreeItemTemp;
	CUIScrollBar*	m_pScroll;

	CUIText*		m_pTxtNas;
	CUIText*		m_pTxtSP;

	vec_makeitem	m_vecList;

	int				m_nfactory_idx;
	int				m_nsel_sub, m_nsel_idx;		// 선택된 인덱스 기록.
	COLOR			m_colPrev;
};

extern const int sub_type_string_idx[];
extern bool comp(CMakeItemData* a, CMakeItemData* b);


#endif		// _UI_PRODUCT_NPC_H_