#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildChangeSetUI.h"
#include <Engine/Interface/UIGuild.h>

CGuildChangeSetUI::CGuildChangeSetUI()
	: m_pCBCorps(NULL)
	, m_pChkStashUsable(NULL)
{
	setInherit(false);

	int i;
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eEDIT_MAX; ++i)
		m_pEditBox[i] = NULL;
}

CGuildChangeSetUI::~CGuildChangeSetUI()
{
	Destroy();
}

void CGuildChangeSetUI::PressOk()
{
	UIMGR()->GetGuild()->PressChangeSetUIOK();
}

void CGuildChangeSetUI::PopupUIOff()
{
	CUIGuild* pUI = UIMGR()->GetGuild();

	if (pUI != NULL)
		pUI->SetManagePopup(FALSE);

	if (m_pCBCorps != NULL)
		CUIFocus::getSingleton()->killFocus(m_pCBCorps);
}

void CGuildChangeSetUI::FocuseChange()
{
	int nNextFocuse = -1;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_pEditBox[i]->IsFocused())
		{
			nNextFocuse = i + 1;
			break;
		}
	}

	if (nNextFocuse == -1)
		return;

	if (nNextFocuse >= eEDIT_MAX)
		nNextFocuse = 0;

	SetFocusEditBox(nNextFocuse);
}

void CGuildChangeSetUI::KillFocusEditBox()
{
	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		m_pEditBox[i]->SetFocus(FALSE);
	}
}

void CGuildChangeSetUI::SetFocusEditBox( int nFocusIndex )
{

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (i == nFocusIndex)
			m_pEditBox[i]->SetFocus(TRUE);
		else
			m_pEditBox[i]->SetFocus(FALSE);
	}
}

void CGuildChangeSetUI::SetString( int nEditId, CTString strString )
{
	if (nEditId < 0 || nEditId >= eEDIT_MAX)
		return;

	if (m_pEditBox[nEditId] == NULL)
		return;

	m_pEditBox[nEditId]->SetString(strString.str_String);
}

CTString CGuildChangeSetUI::GetString( int nEditId )
{
	CTString strTemp = "";
	if (nEditId < 0 || nEditId >= eEDIT_MAX)
		return strTemp;

	if (m_pEditBox[nEditId] == NULL)
		return strTemp;

	strTemp = m_pEditBox[nEditId]->GetString();
	return strTemp;
}

bool CGuildChangeSetUI::IsFocused()
{
	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_pEditBox[i]->IsFocused())
			return true;
	}

	return false;
}

void CGuildChangeSetUI::toggleVisible( BOOL bEnable )
{
	if (bEnable == TRUE)
		openUI();
	else
		closeUI();
}

void CGuildChangeSetUI::openUI()
{
	Hide(FALSE);
	SetEnable(TRUE);

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		m_pEditBox[i]->SetEnable(TRUE);
	}
}

void CGuildChangeSetUI::closeUI()
{
	Hide(TRUE);
	SetEnable(FALSE);
}

void CGuildChangeSetUI::ResetUI()
{
	ResetEditBox();
}

void CGuildChangeSetUI::ResetEditBox()
{
	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		m_pEditBox[i]->ResetString();
	}
}

void CGuildChangeSetUI::initialize()
{
#ifndef		WORLD_EDITOR
	int i;
 	std::string strBtnId[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };
 
 	for (i = 0; i < eBTN_MAX; ++i)
 		m_pBtn[i] = (CUIButton*)findUI(strBtnId[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CGuildChangeSetUI::PopupUIOff, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CGuildChangeSetUI::PressOk, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CGuildChangeSetUI::PopupUIOff, this));

	std::string strEditId[eEDIT_MAX] = { "eb_changePosition", "eb_payExpMin", "eb_payExpMax",
		"eb_payFameMin", "eb_payFameMax" };

	for (i = 0; i < eEDIT_MAX; ++i)
		m_pEditBox[i] = (CUIEditBox*)findUI(strEditId[i].c_str());

	m_pCBCorps = (CUIComboBox*)findUI("combo_corps");
	m_pChkStashUsable = (CUICheckButton*)findUI("chk_stashEnable");
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CGuildChangeSetUI::KeyMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if( m_pEditBox[i]->KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}

	if( pMsg->wParam == VK_TAB )
	{
		FocuseChange();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CGuildChangeSetUI::IMEMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if( m_pEditBox[i]->IsFocused())
			return m_pEditBox[i]->IMEMessage(pMsg); 
	}

	return WMSG_FAIL;
}

WMSG_RESULT CGuildChangeSetUI::CharMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if( m_pEditBox[i]->CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CGuildChangeSetUI::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	int i = 0;

	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if (m_pCBCorps != NULL)
				m_pCBCorps->MouseMove(nX, nY, pMsg);

			if (m_pChkStashUsable != NULL)
				m_pChkStashUsable->MouseMove(nX, nY, pMsg);

			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (m_pEditBox[i] != NULL)
					m_pEditBox[i]->MouseMove(nX, nY, pMsg);
			}

			for (i = 0; i < eBTN_MAX; ++i)
			{
				if (m_pBtn[i] != NULL)
					m_pBtn[i]->MouseMove(nX, nY, pMsg);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (m_pChkStashUsable != NULL)
				m_pChkStashUsable->LButtonDown(nX, nY);

			if (m_pCBCorps && (wmsgResult = m_pCBCorps->LButtonDown(nX, nY)) != WMSG_FAIL)
			{
				KillFocusEditBox();
				m_pCBCorps->SetFocus(TRUE);
				return wmsgResult;
			}

			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (m_pEditBox[i] && (wmsgResult = m_pEditBox[i]->LButtonDown(nX, nY)) != WMSG_FAIL)
				{
					SetFocus(i);
					if (m_pCBCorps != NULL)
						m_pCBCorps->SetFocus(FALSE);

					return wmsgResult;
				}
			}

			for (i = 0; i < eBTN_MAX; ++i)
			{
				if (m_pBtn[i] && (wmsgResult = m_pBtn[i]->LButtonDown(nX, nY)) != WMSG_FAIL)
					return wmsgResult;
			}

			KillFocusEditBox();
			if (m_pCBCorps != NULL)
				m_pCBCorps->SetFocus(FALSE);
			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONUP:
		{
			if (m_pChkStashUsable && m_pChkStashUsable->LButtonUp(nX, nY) != WMSG_FAIL)
				return WMSG_SUCCESS;

			if (m_pCBCorps && m_pCBCorps->LButtonUp(nX, nY) != WMSG_FAIL)
				return WMSG_SUCCESS;

			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (m_pEditBox[i] && m_pEditBox[i]->LButtonUp(nX, nY) != WMSG_FAIL)
					return WMSG_SUCCESS;			
			}

			for (i = 0; i < eBTN_MAX; ++i)
			{
				if (m_pBtn[i] && (wmsgResult = m_pBtn[i]->LButtonUp(nX, nY)) != WMSG_FAIL)
					return wmsgResult;
			}

			return WMSG_FAIL;
		}
		break;
	}

	return WMSG_FAIL;
}

BOOL CGuildChangeSetUI::GetStashUsable()
{
	if (m_pChkStashUsable == NULL)
		return FALSE;

	return m_pChkStashUsable->IsChecked();
}

void CGuildChangeSetUI::SetStashUsable( BOOL bUse )
{
	if (m_pChkStashUsable != NULL)
		m_pChkStashUsable->SetCheck(bUse);
}

void CGuildChangeSetUI::AddComboString( CTString strString )
{
	if (m_pCBCorps != NULL)
		m_pCBCorps->AddString(strString);
}

void CGuildChangeSetUI::SetFocusComboBox( BOOL bFocus )
{
	if (m_pCBCorps != NULL)
		m_pCBCorps->SetFocus(bFocus);
}

void CGuildChangeSetUI::SetComboCurSelIdx( int nSelIdx )
{
	if (m_pCBCorps != NULL)
		m_pCBCorps->SetCurSel(nSelIdx);
}

int CGuildChangeSetUI::GetComboCurSelIdx()
{
	if (m_pCBCorps == NULL)
		return -1;

	return m_pCBCorps->GetCurSel();
}

bool CGuildChangeSetUI::IsFocusedComboBox()
{
	if (m_pCBCorps == NULL)
		return false;

	return m_pCBCorps->IsFocused() ? true : false;
}

void CGuildChangeSetUI::ResetComboBox()
{
	if (m_pCBCorps != NULL)
		m_pCBCorps->ResetStrings();
}

void CGuildChangeSetUI::ResetCharPostionString()
{
	if (m_pEditBox[eEDIT_CHANGE_POSITION] != NULL)
		m_pEditBox[eEDIT_CHANGE_POSITION]->ResetString();
}

