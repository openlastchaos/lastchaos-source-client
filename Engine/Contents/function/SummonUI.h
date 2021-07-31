#ifndef	SUMMON_UI_H_
#define	SUMMON_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIText;
class CUIImageArray;
class CUIImageSplit;
class CUIIcon;

class CSummonUI : public CUIWindow
{
public:
	CSummonUI(int nUIIndex);

	void SetSummonType( int iType );
	void ResetSummon();
	void WarpOfResetSummon();
	void UseSkill();
	void UseSkill( int nIndex );
	void ShowCommandGuide( BOOL bShow, int nCommandID = -1 );
	BOOL StartSkillDelay( int nIndex );	

	ENGINE_API void	SetCommand( INDEX iCommand );	
	ENGINE_API INDEX& GetCommand();

	void SetSummonEntity( CEntity* pEntity )	{ m_penEntity = pEntity; }
	ENGINE_API CEntity*	GetSummonEntity()		{ return m_penEntity;	}

	void SetSummonIndex( int iIndex )			{ m_nIndex = iIndex;	}
	int	 GetSummonIndex()	const				{ return m_nIndex;		}	

	void SetLeftTime( int nLeftTime );
	void SetMaxTime( int nMaxTime );

	void openUI();
	void closeUI();
	void resetUI();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	void OnPostRender(CDrawPort* pDraw);

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	void UpdateHPInfo();
	void UpdateTimeInfo();
	void UpdateBtnState();

	void AddSkill( int iSkillIndex );
	CTString GetCmdGuideString(int nCommandID);
	// drag
	enum
	{
		eIMG_HP = 0,
		eIMG_TIME,
		eIMG_MAX
	};

	CUIImage* m_pImg[eIMG_MAX];
	CUIImage* m_pMoveArea;

	enum
	{
		eBTN_ATTACK = 0,
		eBTN_PINCERS,
		eBTN_HOLD,
		eBTN_PROTECT,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	CUIImageArray*	m_pIaSummonType;
	CUIIcon*		m_pIconSkill;
	CUIImageSplit*	m_pSpTooltipBg;
	CUIText*		m_pTextTooltip;

	const int		m_nUIIndex;

	int				m_nSummonType;
	INDEX			m_nOldCommand;
	INDEX			m_nCurrentCommand;

	CEntity*		m_penEntity;
	int				m_nIndex;	
	int				m_nLeftTime;					// 남은 시간
	int				m_nMaxTime;						// 최대 시간
	TIME			m_tmLeftTime;					// 타이머 용 임시 변수
	BOOL			m_bSetMaxTime;					// 최대 시간 설정 유무.
};

#endif	// SUMMON_UI_H_

