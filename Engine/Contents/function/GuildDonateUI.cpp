#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "GuildDonateUI.h"
#include <Engine/Interface/UIGuild.h>

#define DEF_DEFALT_MIN		0
#define DEF_DEFALT_MAX		100
#define DEF_DEFALT_DONATE	0

CGuildDonateUI::CGuildDonateUI()
	: m_nOpenType(-1)
{
	setInherit(false);

	int i;
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eEDIT_MAX; ++i)
	{
		m_pEditBox[i] = NULL;
		m_bEditLock[i] = false;
	}
}

CGuildDonateUI::~CGuildDonateUI()
{
	Destroy();
}

void CGuildDonateUI::initialize()
{
#ifndef		WORLD_EDITOR
	int i;
	std::string strBtnId[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnId[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CGuildDonateUI::PopupUIOff, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CGuildDonateUI::PressOk, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CGuildDonateUI::PopupUIOff, this));

	std::string strEditId[eEDIT_MAX] = { "eb_payExpMin", "eb_payExpMax", "eb_PayExp", 
		"eb_payFameMin", "eb_payFameMax", "eb_PayFame" };

	for (i = 0; i < eEDIT_MAX; ++i)
		m_pEditBox[i] = (CUIEditBox*)findUI(strEditId[i].c_str());
#endif	//	WORLD_EDITOR
}

void CGuildDonateUI::PopupUIOff()
{
	CUIGuild* pUI = UIMGR()->GetGuild();

	if (pUI != NULL)
		pUI->SetManageSetAllPopup(FALSE);
}

void CGuildDonateUI::FocuseChange()
{
	int nNextFocuse = -1;
	int i;
	for (i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
			continue;

		if (m_pEditBox[i]->IsFocused())
		{
			if (m_nOpenType == eDONATE_UI_CHANGE_INFO)
			{
				if (i == eEDIT_EXP)
					nNextFocuse = eEDIT_FAME;
				else if (i == eEDIT_FAME)
					nNextFocuse = eEDIT_EXP;
			}
			else
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

bool CGuildDonateUI::IsFocused()
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

void CGuildDonateUI::toggleVisible( BOOL bEnable, int nUIType )
{
	if (nUIType < 0 || nUIType >= eDONATE_UI_TYPE_MAX)
	{
		closeUI();
		return;
	}

	m_nOpenType = nUIType;

	if (bEnable == TRUE)
		openUI();
	else
		closeUI();
}

void CGuildDonateUI::KillFocusEditBox()
{
	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		m_pEditBox[i]->SetFocus(FALSE);
	}
}

void CGuildDonateUI::SetFocusEditBox( int nFocusIndex )
{

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
			continue;

		if (i == nFocusIndex)
			m_pEditBox[i]->SetFocus(TRUE);
		else
			m_pEditBox[i]->SetFocus(FALSE);
	}
}

void CGuildDonateUI::ResetEditBox()
{
	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		m_pEditBox[i]->ResetString();
		m_pEditBox[i]->SetEnable(FALSE);
		m_bEditLock[i] = false;
	}
}

void CGuildDonateUI::PressOk()
{
	switch(m_nOpenType)
	{
	case eDONATE_UI_SET_ALL:
		UIMGR()->GetGuild()->PressChangeSetAllUIOK();
		break;

	case eDONATE_UI_CHANGE_INFO:
		UIMGR()->GetGuild()->PressDonateUIOK();
		break;
	}
	
}

void CGuildDonateUI::ResetUI()
{
	ResetEditBox();
	m_nOpenType = -1;
}

void CGuildDonateUI::SetStringLock()
{
	int i;

	switch(m_nOpenType)
	{
	case eDONATE_UI_SET_ALL:
		{
			for (i = 0; i < eEDIT_MAX; ++i)
			{
				m_bEditLock[i] = false;
			}
		}
		break;
	case eDONATE_UI_CHANGE_INFO:
		{
			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (i == eEDIT_EXP || i == eEDIT_FAME)
					m_bEditLock[i] = false;
				else
					m_bEditLock[i] = true;
			}
		}
		break;
	default:
		return;
	}
}

void CGuildDonateUI::openUI()
{
	Hide(FALSE);
	SetEnable(TRUE);

	updateUI();
	SetStringLock();

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
			continue;

		m_pEditBox[i]->SetEnable(TRUE);
	}
}

void CGuildDonateUI::closeUI()
{
	Hide(TRUE);
	SetEnable(FALSE);
}

void CGuildDonateUI::updateUI()
{
	if (m_nOpenType == eDONATE_UI_SET_ALL)
	{
		CTString strTemp;
		strTemp.PrintF("%d", DEF_DEFALT_MIN);
		SetString(eEDIT_EXP_MIN, strTemp);
		SetString(eEDIT_FAME_MIN, strTemp);

		strTemp.PrintF("%d", DEF_DEFALT_MAX);
		SetString(eEDIT_EXP_MAX, strTemp);
		SetString(eEDIT_FAME_MAX, strTemp);

		strTemp.PrintF("%d", DEF_DEFALT_DONATE);
		SetString(eEDIT_EXP, strTemp);
		SetString(eEDIT_FAME, strTemp);
	}
}

void CGuildDonateUI::SetString( int nEditId, CTString strString )
{
	if (nEditId < 0 || nEditId >= eEDIT_MAX)
		return;

	if (m_pEditBox[nEditId] == NULL)
		return;

	m_pEditBox[nEditId]->SetString(strString.str_String);
}

CTString CGuildDonateUI::GetString( int nEditId )
{
	CTString strTemp = "";
	if (nEditId < 0 || nEditId >= eEDIT_MAX)
		return strTemp;

	if (m_pEditBox[nEditId] == NULL)
		return strTemp;

	strTemp = m_pEditBox[nEditId]->GetString();
	return strTemp;
}

WMSG_RESULT CGuildDonateUI::KeyMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
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

WMSG_RESULT CGuildDonateUI::IMEMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
			continue;

		if( m_pEditBox[i]->IsFocused())
			return m_pEditBox[i]->IMEMessage(pMsg); 
	}

	return WMSG_FAIL;
}

WMSG_RESULT CGuildDonateUI::CharMessage( MSG *pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	for (int i = 0; i < eEDIT_MAX; ++i)
	{
		if (m_pEditBox[i] == NULL)
			continue;

		if (m_bEditLock[i] == true)
			continue;

		if( m_pEditBox[i]->CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CGuildDonateUI::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	int i = 0;

	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
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
			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (m_bEditLock[i] == true)
					continue;

				if (m_pEditBox[i] && (wmsgResult = m_pEditBox[i]->LButtonDown(nX, nY)) != WMSG_FAIL)
				{
					SetFocus(i);
					return wmsgResult;
				}
			}

			for (i = 0; i < eBTN_MAX; ++i)
			{
				if (m_pBtn[i] && (wmsgResult = m_pBtn[i]->LButtonDown(nX, nY)) != WMSG_FAIL)
					return wmsgResult;
			}

			KillFocusEditBox();
			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONUP:
		{
			for (i = 0; i < eEDIT_MAX; ++i)
			{
				if (m_bEditLock[i] == true)
					continue;

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

