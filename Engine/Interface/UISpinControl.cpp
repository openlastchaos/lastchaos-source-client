#include "stdh.h"

#include <Engine/Interface/UIEditBox.h>

#include "UISpinControl.h"

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICommand.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIImageSplit.h>

class SpinCmdOnChange : public Command
{
public:
	SpinCmdOnChange() : parent_(NULL) {}

	void setData(CUISpinControl* parent, bool bUp)	{ parent_ = parent; bUp_ = bUp; }
	void execute() 
	{
		if (parent_)
		{
			parent_->MoveData(bUp_);
		}
	}

private:
	CUISpinControl* parent_;
	bool bUp_;
};

CUISpinControl::CUISpinControl()
	: m_nMin(0)
	, m_llMax(0)
	, m_pBackImage(NULL)
	, m_pCmdReturn(NULL)
{
	setInherit(false);

	setType(eUI_CONTROL_SPIN_CONTROL);

	m_nMaxCharCount = 16;
	SetOnlyIntegerMode(TRUE);

	for (int i = 0; i < 2; ++i)
	{
		m_pSpinBtn[i] = NULL;
	}
}

CUISpinControl::~CUISpinControl()
{
	SAFE_DELETE(m_pBackImage);
	SAFE_DELETE(m_pCmdReturn);

	CUIEditBox::Destroy();
}

void CUISpinControl::initialize()
{
	if (m_pSpinBtn[0] = (CUIButton*)findUI("btn_up"))
	{
		SpinCmdOnChange* pCmd = new SpinCmdOnChange;
		pCmd->setData(this, true);
		m_pSpinBtn[0]->SetCommandUp(pCmd);
	}

	if (m_pSpinBtn[1] = (CUIButton*)findUI("btn_down"))
	{
		SpinCmdOnChange* pCmd = new SpinCmdOnChange;
		pCmd->setData(this, false);
		m_pSpinBtn[1]->SetCommandUp(pCmd);
	}

	CUIEditBox::initialize();
}

CUIBase* CUISpinControl::Clone()
{
	CUISpinControl* pSpinCtrl = new CUISpinControl(*this);	

	erase_pointer();

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();

	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pSpinCtrl->SetRectSurface(pCopy);
		}		
	}

	// 텍스쳐를 사용하면 복사 시 Ref Count 를 올려준다.
	if (m_pTexData != NULL)
		m_pTexData->MarkUsed();

	if (m_pBackImage != NULL)
		pSpinCtrl->SetBackImage((CUIImageSplit*)m_pBackImage->Clone());
	else
		pSpinCtrl->SetBackImage(NULL);

	CUIBase::CloneChild(pSpinCtrl);

	// child 복제 후 초기화 (버튼 초기화해야 됨.)
	pSpinCtrl->initialize();

	return pSpinCtrl;
}

void CUISpinControl::MoveData( bool bUp )
{
	SQUAD llCurData = _atoi64(GetString());

	if (bUp)
	{
		++llCurData;

		if (llCurData > m_llMax)
			llCurData = m_nMin;
	}
	else
	{
		--llCurData;

		if (llCurData < m_nMin)
			llCurData = m_llMax;
	}

	CTString strTmp;
	strTmp.PrintF("%I64d", llCurData);
	SetString(strTmp.str_String);

	if (m_funcMove)
		m_funcMove(this);
}

WMSG_RESULT CUISpinControl::OnMouseWheel( UINT16 x, UINT16 y, int wheel )
{
	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	MoveData(wheel > 0 ? true : false);
	
	return WMSG_SUCCESS;
}

void CUISpinControl::OnRender( CDrawPort* pDraw )
{
	
	if (m_pBackImage != NULL)
	{
		int nX, nY;
		GetAbsPos(nX, nY);

		m_pBackImage->SetPos(nX + m_rcBackPos.Left, nY + m_rcBackPos.Top);

		m_pBackImage->OnRender(pDraw);
	}

	CUIEditBox::OnRender(pDraw);
}

WMSG_RESULT CUISpinControl::OnCharMessage( MSG* pMsg )
{
	extern UBYTE	_abKeysPressed[256];

	// If editbox is not focused
	if( !IsFocused() || m_InValidEditBox )
		return WMSG_FAIL;

	if(m_bOnlyInteger && (pMsg->wParam < 48 || pMsg->wParam > 57))
	{
		return WMSG_SUCCESS;
	}

	InsertChar(m_nCursorIndex, pMsg->wParam);

	if (_atoi64(GetString()) > m_llMax)
	{
		CTString strTmp;
		strTmp.PrintF("%I64d", m_llMax);
		UIMGR()->InsertCommaToString(strTmp);
		SetString(strTmp.str_String);
	}

	return WMSG_SUCCESS;
}

WMSG_RESULT CUISpinControl::OnKeyMessage( MSG* pMsg )
{
	if( !IsFocused() || m_InValidEditBox )
		return WMSG_FAIL;

	if (pMsg->wParam == VK_RETURN)
	{
		if (m_pCmdReturn != NULL)
			m_pCmdReturn->execute();

		if (m_funcReturn)
			m_funcReturn(this);
	}

	return CUIEditBox::OnKeyMessage(pMsg);
}

void CUISpinControl::SetBackGround( UIRect rc, UIRectUV uv, int nUnit )
{
	if (m_pBackImage == NULL)
		m_pBackImage = new CUIImageSplit;

	if (m_pTexData != NULL)
	{
		m_pBackImage->setTexString(m_strTex.c_str());
	}

	m_rcBackPos = rc;

	m_pBackImage->SetUnit(nUnit);
	m_pBackImage->SetSplitMode(CUIImageSplit::SPLIT_3H);

	m_pBackImage->SetPos(rc.Left,  rc.Top);
	m_pBackImage->SetSize(rc.Right, rc.Bottom);
	m_pBackImage->SetUV(uv);

	m_pBackImage->UpdateSplit();
}

void CUISpinControl::GetBackGround( UIRect& rc, UIRectUV& uv, int& nUnit )
{
	if (m_pBackImage == NULL)
		return;

	rc = m_rcBackPos;
	uv = m_pBackImage->GetAbsUV();
	nUnit = m_pBackImage->GetUnit();
}

void CUISpinControl::CmdErase()
{
	m_pCmdReturn = NULL;
	m_funcMove = NULL;
	m_funcReturn = NULL;
	
	CUIBase::CmdErase();
}

void CUISpinControl::OnEnter( UINT16 x, UINT16 y )
{
	CUIFocus::getSingleton()->setUI(this);

	CUIBase::OnEnter(x, y);
}

void CUISpinControl::OnLeave( UINT16 x, UINT16 y )
{
	CUIFocus::getSingleton()->killFocus(this);

	CUIBase::OnLeave(x, y);
}


