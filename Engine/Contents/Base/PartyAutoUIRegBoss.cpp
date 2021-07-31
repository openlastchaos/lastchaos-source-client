#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "PartyAutoUIRegBoss.h"
#include <Engine/Interface/UIPartyAuto.h>

#define MAX_PARTY_NAME_LEN	20

class CmdCheckCallback : public Command
{
public:
	CmdCheckCallback() : _ui(NULL) {}
	void setData(CUIPartyAutoRegBoss* pUI, int jobidx) {
		_ui = pUI;	jidx = jobidx;
	}
	void execute() {
		if (_ui != NULL)
			_ui->check_callback(jidx);
	}
private:
	CUIPartyAutoRegBoss* _ui;
	int		jidx;
};


CUIPartyAutoRegBoss::CUIPartyAutoRegBoss()
	: m_dwJobFlag(0)
	, m_bLimit_level(FALSE)
	, m_pEditNotice(NULL)
	, m_pCheckLevel(NULL)
{
	setInherit(false);
}

void CUIPartyAutoRegBoss::initialize()
{
	int		i;

	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pEditNotice = (CUIEditBox*)findUI("edit_notice");

	if (m_pEditNotice != NULL)
		m_pEditNotice->SetMaxChar(MAX_PARTY_NAME_LEN);

	{
		const char* arr_chk_job[] = {
			"chk_titan",
			"chk_knight",
			"chk_healer",
			"chk_mage",
			"chk_rogue",
			"chk_sorcerer",
			"chk_nightshadow",
			"chk_exrogue",
			"chk_archmage",
		};

		for (i = 0; i < TOTAL_JOB; ++i)
		{
			m_pCheckJob[i] = (CUICheckButton*)findUI(arr_chk_job[i]);

			if (m_pCheckJob[i] != NULL)
			{
				CmdCheckCallback* pCmd = new CmdCheckCallback;
				pCmd->setData(this, i);
				m_pCheckJob[i]->SetCommand(pCmd);
			}
		}
	}

	m_pCheckLevel = (CUICheckButton*)findUI("chk_level");
#ifndef		WORLD_EDITOR
	if (m_pCheckLevel != NULL)
		m_pCheckLevel->SetCommandF(boost::bind(&CUIPartyAutoRegBoss::check_level, this));
	

	CUIButton* pBtn = (CUIButton*)findUI("btn_ok");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoRegBoss::send_reg, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoRegBoss::close, this));

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoRegBoss::close, this));
#endif	// WORLD_EDITOR
}

void CUIPartyAutoRegBoss::open()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_PARTYAUTO_REG_PB, TRUE);
}

void CUIPartyAutoRegBoss::close()
{
	SetVisible(FALSE);
	Hide(TRUE);
}

void CUIPartyAutoRegBoss::check_callback( int job_idx )
{
	if (job_idx < 0 || job_idx >= TOTAL_JOB)
		return;

	if (m_pCheckJob[job_idx] != NULL)
	{
		BOOL bCheck = m_pCheckJob[job_idx]->IsChecked();

		if (bCheck == TRUE)
			m_dwJobFlag |= (1 << job_idx);
		else
			m_dwJobFlag &= ~(1 << job_idx);
	}
}

BOOL CUIPartyAutoRegBoss::IsEditBoxFocused()
{
	if (m_pEditNotice != NULL && m_pEditNotice->IsFocused())
		return TRUE;

	return FALSE;
}

void CUIPartyAutoRegBoss::KillFocusEditBox()
{
	if (m_pEditNotice != NULL)
		m_pEditNotice->SetFocus(FALSE);
}

// ------------------------------------------------------------------

void CUIPartyAutoRegBoss::send_reg()
{
	CTString strNotice;
	
	if (m_pEditNotice != NULL)
	{
		strNotice = m_pEditNotice->GetString();
	}

	if (strNotice.Length() > MAX_PARTY_NAME_LEN)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_REGIST_ERROR );
		CUIMsgBox_Info MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_PARTYAUTO, MSGCMD_PARTYAUTO_REGIST_ERROR );
		CTString	strMessage = _S( 2691, "파티 공고가 너무 깁니다.\n( 한글 최대 25자, 영문 최대 50자" );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		return;
	}
	else if (strNotice.Length() <= 0)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_REGIST_ERROR );
		CUIMsgBox_Info MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_PARTYAUTO, MSGCMD_PARTYAUTO_REGIST_ERROR );
		CTString	strMessage = _S( 2692, "파티 공고를 입력하세요." );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		return;
	}

	if (m_dwJobFlag == 0) 
		m_dwJobFlag = JF_ALLCLASS;

	_pNetwork->SendPartyRegistParty(strNotice, m_dwJobFlag, m_bLimit_level);
	
}

void CUIPartyAutoRegBoss::check_level()
{
	if (m_pCheckLevel != NULL)
		m_bLimit_level = m_pCheckLevel->IsChecked();
}
