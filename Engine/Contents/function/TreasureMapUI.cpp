#include "stdh.h"
#include "TreasureMapUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIMap.h>

CTreasureMapUI::CTreasureMapUI()
	: m_pMoveArea(NULL)
	, m_pBtnClose(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
	for (int i = 0; i < eIMG_MAX; ++i)
		m_pImg[i] = NULL;

	setInherit(false);
}

void CTreasureMapUI::OpenMap( INDEX ZoneIndex, FLOAT fPosX, FLOAT fPosY )
{
	if (GetHide() == FALSE)
		return;

	if (UpdateUI(ZoneIndex, fPosX, fPosY) == false)
		return;

	Hide(FALSE);
	SetVisible(TRUE);
	UIMGR()->RearrangeOrder( UI_TREASURE_MAP, TRUE );
}

void CTreasureMapUI::Close()
{
	Hide(TRUE);
	SetVisible(FALSE);

	m_nZoneIndex = -1;
	m_fposX = 0.0;
	m_fposY = 0.0;

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_TREASURE_MAP, FALSE );
}

bool CTreasureMapUI::UpdateUI(INDEX ZoneIndex, FLOAT fPosX, FLOAT fPosY)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_nZoneIndex = ZoneIndex;
	m_fposX = fPosX;
	m_fposY = fPosY;

	BOOL bChackMap = FALSE;
	bChackMap = pUIManager->GetMap()->GetWorldToMapPos(ZoneIndex, m_fposX, m_fposY, 0.73f);

	if (!bChackMap)
	{
		return false;
	}

	// Create map texture
	if (m_pImg[eIMG_MAP] == NULL)
		return false;
	
	CTString	strFileName;
	strFileName.PrintF( "Map_World%d%d.tex", ZoneIndex, 0 );
	m_pImg[eIMG_MAP]->setTexString(strFileName);

	if (m_pImg[eIMG_MARK] != NULL)
		m_pImg[eIMG_MARK]->SetPos(m_fposX-15, m_fposY);
	
	return true;
}

void CTreasureMapUI::initialize()
{
#ifndef		WORLD_EDITOR
	std::string strImgID[eIMG_MAX] = { "img_map", "img_mark" };

	for (int i = 0; i < eIMG_MAX; ++ i)
		m_pImg[i] = (CUIImage*)findUI(strImgID[i].c_str());

	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
		m_pBtnClose->SetCommandFUp(boost::bind(&CTreasureMapUI::Close, this));

	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");

#endif		// WORLD_EDITOR
}

WMSG_RESULT CTreasureMapUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();
	return WMSG_FAIL;
}

WMSG_RESULT CTreasureMapUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_TREASURE_MAP, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CTreasureMapUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->ResetHoldBtn();
	return WMSG_FAIL;
}
