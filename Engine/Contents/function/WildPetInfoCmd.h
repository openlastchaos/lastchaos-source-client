#ifndef __COMMAND_WILD_PET_INFO_H__
#define __COMMAND_WILD_PET_INFO_H__

class Command;


//--------------------------------------------------------------
// Stat Up
class CmdWildPetInfoStatUp : public Command
{
public:
	CmdWildPetInfoStatUp() : pUI_(NULL), statType_(0) {}
	void setData(CWildPetInfoUI* pUI, UBYTE statType)	{ pUI_ = pUI; statType_ = statType; }
	void execute() {
		if (pUI_)
			pUI_->SendWildPetPointUP(statType_);
	}
private:
	CWildPetInfoUI* pUI_;
	UBYTE statType_;
};

//--------------------------------------------------------------
// 스킬 사용
class CmdWildPetInfoSkillUse : public Command
{
public:
	CmdWildPetInfoSkillUse() : pUI_(NULL), nSkillIndex_(-1) {}
	void setData(CWildPetInfoUI* pUI, int nSkillIdx)	
	{ 
		pUI_ = pUI;
		nSkillIndex_ = nSkillIdx;
	}
	void execute() {
		if (pUI_)
		{
			if (nSkillIndex_ != -1 && MY_INFO()->GetPetSkillDelay(0, nSkillIndex_) == false)
				pUI_->UseSkill(nSkillIndex_);
		}
	}
private:
	CWildPetInfoUI* pUI_;
	int nSkillIndex_;
};

//--------------------------------------------------------------
// Inven Item 더블클릭
class CmdWildPetInfoInvenItemDBL : public Command
{
public:
	CmdWildPetInfoInvenItemDBL() : pUI_(NULL), nWearPos_(0) {}
	void setData(CWildPetInfoUI* pUI, int nWearPos)	{ pUI_ = pUI; nWearPos_ = nWearPos; }
	void execute() {
		if (pUI_)
			pUI_->WearItemDBL(nWearPos_);
	}
private:
	CWildPetInfoUI* pUI_;
	int nWearPos_;
};

//--------------------------------------------------------------
// mainTab 변경시
class CmdWildPetInfoMainTabChange : public Command
{
public:
	CmdWildPetInfoMainTabChange() : pUI_(NULL) {}
	void setData(CWildPetInfoUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->changeMainTab();
	}
private:
	CWildPetInfoUI* pUI_;
};

#endif	//	__COMMAND_WILD_PET_INFO_H__