#include "StdH.h"
#include "UIImageBox.h"
#include "UIManager.h"
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIButtonEx.h>


CUIImageBox::CUIImageBox()
: m_eImageType( IT_GENERAL )
, m_bHighlight( FALSE )
, m_colHighlight( 0x808080FF )
{
}

CUIImageBox::~CUIImageBox()
{
}

void CUIImageBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	int nX0 = nX + m_rtRenderRegion.Left;
	int nX1 = nX + m_rtRenderRegion.Right;
	int nY0 = nY + m_rtRenderRegion.Top;
	int nY1 = nY + m_rtRenderRegion.Bottom;
	float fU0 = m_rtUV.U0 + (m_rtUV.U1 - m_rtUV.U0) * m_rtRenderRegion.Left / float(m_nWidth);
	float fU1 = m_rtUV.U0 + (m_rtUV.U1 - m_rtUV.U0) * m_rtRenderRegion.Right / float(m_nWidth);
	float fV0 = m_rtUV.V0 + (m_rtUV.V1 - m_rtUV.V0) * m_rtRenderRegion.Top / float(m_nHeight);
	float fV1 = m_rtUV.V0 + (m_rtUV.V1 - m_rtUV.V0) * m_rtRenderRegion.Bottom / float(m_nHeight);

	if(m_eImageType == IT_GENERAL)
	{
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
		_pUIMgr->GetDrawPort()->AddTexture(nX0, nY0, nX1, nY1,
											fU0, fV0,
											fU1, fV1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}
	else
	{
		UIBtnExType bet;
		switch(m_eImageType)
		{
			case IT_EXP:
			case IT_MONEY:
			case IT_ITEM:
			case IT_SP:		// [090617: selo] SP 이미지 타입
				bet = UBET_ITEM;
				break;
			case IT_SKILL:
			case IT_SSKILL:
				bet = UBET_SKILL;
				break;
			case IT_ACTION:
				bet = UBET_ACTION;
				break;
		}
		_pUIMgr->GetDrawPort()->AddBtnTexture(m_nTextureID, nX0, nY0, nX1, nY1,
											fU0, fV0,
											fU1, fV1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( bet, PBT_BLEND );

		if(m_bHighlight)
		{
			_pUIMgr->GetDrawPort()->AddBtnTexture(m_nTextureID, nX0, nY0, nX1, nY1,
												fU0, fV0,
												fU1, fV1,
												m_colHighlight );

			// Render all button elements
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( bet, PBT_ADD );
		}
	}
}


void CUIImageBox::SetImageByType(eImageType type, int index)
{
	FLOAT fTexWidth;
	FLOAT fTexHeight;
	int nTexRow, nTexCol;
	int nTextureID;
	int nUBET;
	
	switch(type)
	{
	case IT_EXP://hardcoding
		{
			nTextureID = 1;
			nTexRow = 15;
			nTexCol = 5;
			nUBET = UBET_ITEM;
		} break;
	case IT_SP:	// [090617: selo] SP 이미지 추가 하드코딩
		{
			nTextureID = 9;
			nTexRow = 12;
			nTexCol = 11;
			nUBET = UBET_ITEM;
		} break;
	case IT_MONEY:
	case IT_ITEM:
		{
			if(IT_MONEY == type) index = 19;//hardcoding
			CItemData	&rItemData = _pNetwork->GetItemData( index );
			nTextureID = rItemData.GetIconTexID();
			nTexRow = rItemData.GetIconTexRow();
			nTexCol = rItemData.GetIconTexCol();
			nUBET = UBET_ITEM;
		} break;
	case IT_SKILL:
		{
			CSkill	&rSkillData = _pNetwork->GetSkillData( index );
			nTextureID = rSkillData.GetIconTexID();
			nTexRow = rSkillData.GetIconTexRow();
			nTexCol = rSkillData.GetIconTexCol();
			nUBET = UBET_SKILL;
		} break;
	case IT_SSKILL:// Special Skill
		{
			CSpecialSkill	&rSkillData = _pNetwork->GetSSkillData( index );
			nTextureID = rSkillData.GetIconTexID();
			nTexRow = rSkillData.GetIconTexRow();
			nTexCol = rSkillData.GetIconTexCol();
			nUBET = UBET_SKILL;
		} break;
	case IT_ACTION:
		{
			CAction	&rActionData = _pNetwork->GetActionData(index);
			nTextureID = rActionData.GetTexID();
			nTexRow = rActionData.GetTexRow();
			nTexCol = rActionData.GetTexCol();
			nUBET = UBET_ACTION;
		} break;
	default:
		{
			ASSERTALWAYS("Somthing wrong");
		} break;
	}

	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UIBtnExType(nUBET), nTextureID );
	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UIBtnExType(nUBET), nTextureID );
	m_ptdBaseTexture = _pUIBtnTexMgr->GetTex( UIBtnExType(nUBET), nTextureID );
	m_ptdBaseTexture->AddReference();

	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

	m_eImageType = type;
	m_nTextureID = nTextureID;
}
