#ifndef	PET_TRAINING_UI_H_
#define	PET_TRAINING_UI_H_

class CUIImage;
class CUIText;
class CUIButton;
class CUIList;
class CUITextBox;
class CUITab;

class CPetTrainingDesign : public CUIWindow
{
public:
	CPetTrainingDesign();

	void	initialize();
	
	CUIBase* CloneDescText()
	{
		if (m_ptbDesc == NULL)
			return NULL;
		return m_ptbDesc->Clone();
	}
	CUIBase* CloneNameText()
	{
		if (m_pstrName == NULL)
			return NULL;
		return m_pstrName->Clone();
	}
	CUIBase* CloneInfoText()
	{
		if (m_pstrInfo == NULL)
			return NULL;
		return m_pstrInfo->Clone();
	}
	CUIBase* CloneAddText()
	{
		if (m_pstrAdd == NULL)
			return NULL;
		return m_pstrAdd->Clone();
	}
	CUIBase* CloneSpace()
	{
		if (m_pstrSpace == NULL)
			return NULL;
		return m_pstrSpace->Clone();
	}
private:
	CUITextBox*	m_ptbDesc;
	CUIText*	m_pstrName;
	CUIText*	m_pstrInfo;
	CUIText*	m_pstrAdd;
	CUIText*	m_pstrSpace;
};

class CPetTrainingUI : public CUIWindow
{
public:
	CPetTrainingUI();
	~CPetTrainingUI();

	void initialize();

	ENGINE_API void	OpenPetTraining( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void openUI();
	void closeUI();
	void updateUI();

	void updateSkill();
	void updateCommand();
	void updateAbilityPoint();
	// Network message functions ( send )
	void SendLearnSkill();

	// Network message functions ( receive )
	void LearnSkill( int nPetIdx, SLONG slIndex, SBYTE sbLevel, BOOL bAutoLearn = FALSE, bool bShowMsg = true );
	void LearnSkillError( UBYTE ubError );	
	void PetChangeItemError( SBYTE sbResult );  // eons
	void EvolutionError(UBYTE errorcode);
	void SelectItem(int nSelectIndex, int nTabType);
	bool IsNotPetWear();

	// Command functions
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void MsgBoxLCommand( int nCommandCode, int nResult );

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	void _SetUI();
	void _AddString(int nAddType, CTString strAdd = CTString(""));
	struct	sCollectSkill
	{
		LONG	lSkillIndex;
		SBYTE	sbSkillLevel;
		WORD	sbNeedSkillLevel;

		sCollectSkill()
		{
			lSkillIndex		= -1;
			sbSkillLevel	= -1;
			sbNeedSkillLevel= -1;
		}

		bool operator<(const sCollectSkill &other) const
		{			
			if(sbNeedSkillLevel < other.sbNeedSkillLevel)		return true;
			else if(sbNeedSkillLevel > other.sbNeedSkillLevel)	return false;
			else												return false;
		}

		bool operator>(const sCollectSkill &other) const
		{			
			return other.operator < (*this);
		}

		void SetData(int nIndex, SBYTE sbLevel, SBYTE sbNeedLevel)
		{
			lSkillIndex			= nIndex;
			sbSkillLevel		= sbLevel;
			sbNeedSkillLevel	= sbNeedLevel;
		}
	};

	std::vector<sCollectSkill>		m_vecCommand;
	std::vector<sCollectSkill>		m_vecSkill;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;

	FLOAT m_fNpcX, m_fNpcZ;
	int m_nNpcIndex;

	enum
	{
		eLIST_COMMAND = 0,
		eLIST_SKILL,
		eLIST_CONTENTS,
		eLIST_MAX
	};

	CUIList* m_pList[eLIST_MAX];

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIText* m_pstrAbilityPoint;
	CUIImage* m_pMoveArea;
	CUITab* m_pMainTab;
	CPetTrainingDesign* m_pDesign;
};

#endif	// PET_TRAINING_UI_H_

