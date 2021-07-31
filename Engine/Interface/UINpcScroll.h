#ifndef	UINpcScroll_H_
#define	UINpcScroll_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define READ_MAX_CHAR 32

// ----------------------------------------------------------------------------
// Name : CUINpcScroll.h
// Desc : Displays a list of NPCs 
// ----------------------------------------------------------------------------
class CUINpcScroll : public CUIWindow
{

protected:

	enum eSTATUS
	{
		eSTATUS_LIST	= 0,	// 목록 표시
		eSTATUS_DESC	,		// 설명 표시
		eSTATUS_YESNO	,		// 이동 선택 표시
		eSTATUS_MAX
	};

	int					m_nCurRow;
	int					m_nCurZone;
	int					m_nStringCount;
	CTString			m_strDesc[READ_MAX_CHAR];
	int					m_nCurNpcIndex;
	int					m_nCurMobIndex;
	

	float				fX, fZ; 
	// check open flag. [9/21/2009 rumist]
	BOOL				m_bIsOpen;

	CStaticArray<int> cArrItemIdex;

public:
	CUINpcScroll();
	~CUINpcScroll();

	void	initialize();
	
	void	OpenNPCScroll();
	void	CloseNpcScroll();

	void	SetViewList();

	void	RefreshNpcList();
	COLOR	GetNameColor(int nIndex);
	void	ReceiveNPCData(CNetworkMessage* istr);
	void	ReceiveMobLocation(CNetworkMessage* istr);
	void	OpenNPCScrollInfo(SLONG slIndex);
	BOOL	IsNPC(int nIndex);
	// check & toggling func. [9/21/2009 rumist]
	BOOL	IsOpened() const;
	void	ToggleNPCScroll();
	void	UseNPCScroll();
	BOOL	IsInPeaceArea();
	BOOL	IsOpenDetailedMap(int nZone, int nNpcIndex);

	void	choose(int idx, bool bnpc);
private:
	bool	add_list(CTString& str, COLOR col = DEF_UI_COLOR_WHITE, int idx = -1, bool bnpc = true);

	void	set_status(eSTATUS status);

	void	press_list();
	void	press_move();
	void	press_yes();
	void	press_no();

	typedef		std::vector<CMobData*>				vec_mob;
	typedef		std::vector<CMobData*>::iterator	vec_mob_iter;

	std::vector<int>	m_vectorNpclist;
	vec_mob				m_vectorMobList;
	eSTATUS		m_eStatus;

	CUIBase*	m_pGroup[eSTATUS_MAX];
	CUITextBox*	m_pTbDesc;
	CUIList*	m_pList;

	CUIText*	m_pTxtJob;
	CUITextBox* m_pTbDescMove;
};

#endif	