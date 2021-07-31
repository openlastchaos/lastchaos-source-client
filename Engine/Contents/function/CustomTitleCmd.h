#ifndef __COMMAND_CUSTOM_TITLE_H__
#define __COMMAND_CUSTOM_TITLE_H__

class Command;

//--------------------------------------------------------------
// 닫기
class CmdCustomTitleClose : public Command
{
public:
	CmdCustomTitleClose() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->closeUI();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 제작버튼 클릭
class CmdCustomPressMakeBtn : public Command
{
public:
	CmdCustomPressMakeBtn() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressMakeBtn();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 제작하기
class CmdCustomTitleMake : public Command
{
public:
	CmdCustomTitleMake() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->MakeTitle();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 제작 취소하기
class CmdCustomTitleMakeCancel : public Command
{
public:
	CmdCustomTitleMakeCancel() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CancelMakeTitle();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 메뉴 아이템 바꾸기
class CmdCustomTitleChangeMenu : public Command
{
public:
	CmdCustomTitleChangeMenu() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI, int menuType, eChangeType eType)
	{ 
		pUI_ = pUI; 
		_MenuType = menuType;
		_ChangeType = eType;
	}
	void execute() {
		if (pUI_)
			pUI_->ChangeMenu(_MenuType, _ChangeType);
	}
private:
	CCustomTitleUI* pUI_;
	int _MenuType;
	eChangeType _ChangeType;
};

//--------------------------------------------------------------
// 이펙트 선택
class CmdCustomTitleSelectEffect : public Command
{
public:
	CmdCustomTitleSelectEffect() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectEffect();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 호칭글자색 선택
class CmdCustomTitleSelectFrontColor : public Command
{
public:
	CmdCustomTitleSelectFrontColor() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectFrontColor();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// 호칭배경색 선택
class CmdCustomTitleSelectBackColor : public Command
{
public:
	CmdCustomTitleSelectBackColor() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectBackColor();
	}
private:
	CCustomTitleUI* pUI_;
};

#endif	//	__COMMAND_CUSTOM_TITLE_H__