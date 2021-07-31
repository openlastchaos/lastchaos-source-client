
#include "stdh.h"
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <iostream>

//#define DEL_PET_AI_EDIT
extern INDEX g_iCountry;

CUIWildPetInfo::CUIWildPetInfo()
{
	m_bAIActive = FALSE;
	m_bSlideUI = FALSE;
	m_nTebnum = WILDPET_INFO_SLOT;

	for (int nslot = 0; nslot < WILDPET_AI_MAXSLOT; nslot++)
	{
		m_sAISlot[nslot].m_bActive = FALSE;
		m_sAISlot[nslot].m_nSillType = -1;
		m_sAISlot[nslot].m_nActionSlot = -1;
		m_sAISlot[nslot].m_nTargetSlot = -1;
	
	}
}
CUIWildPetInfo::~CUIWildPetInfo()
{

}

static int m_nSelSkillIndex = -1;
static int m_nShowItem = -1;
static int m_nShowSkill = -1;
static int m_nAIShowItem = -1;
static int m_nAIShowSkill = -1;

void CUIWildPetInfo::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_PetInfo.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtsBace.AddRectSurface(UIRect(0,0,nWidth,37),UIRectUV(239,389,511,425,fTexWidth,fTexHeight));
	m_rtsBace.AddRectSurface(UIRect(0,37,nWidth,nHeight-10),UIRectUV(239,425,511,502,fTexWidth,fTexHeight));
	m_rtsBace.AddRectSurface(UIRect(0,nHeight-10,nWidth,nHeight),UIRectUV(239,502,511,511,fTexWidth,fTexHeight));

	m_rtsAIBace.AddRectSurface(UIRect(273,118,397,400),UIRectUV(525,0,649,282,fTexWidth,fTexHeight));

	m_rtTitle.SetRect(0,0,nWidth,37);

	m_rtInfoBace.SetUV(0,0,234,356,fTexWidth,fTexHeight);

	m_rtSkillBace.SetUV(277,0,511,356,fTexWidth,fTexHeight);

	m_rtAIONOFF.SetRect(48, 177, 73, 202);

	m_btnAIEdit.Create(this, CTString(""), 219, 177, 25, 25);
	m_btnAIEdit.SetUV(UBS_IDLE, 114, 375, 138, 399, fTexWidth, fTexHeight);
	m_btnAIEdit.SetUV(UBS_CLICK, 142, 375, 166, 399, fTexWidth, fTexHeight);
	m_btnAIEdit.CopyUV(UBS_IDLE, UBS_ON);
	m_btnAIEdit.CopyUV(UBS_IDLE, UBS_DISABLE);
	

	for(int nAISlot = 0; nAISlot < WILDPET_AI_MAXSLOT; nAISlot++)
	{
		m_abtAITarget[nAISlot].Create(this,297,141+(nAISlot*39),32,32,UI_WILDPET_INFO,UBET_ITEM);
		m_rtAITarget[nAISlot].SetRect(297,141+(nAISlot*39),297+32,141+(nAISlot*39)+32);
		m_abtAIAction[nAISlot].Create(this,348,141+(nAISlot*39),32,32,UI_WILDPET_INFO,UBET_SKILL);
		m_rtAIAction[nAISlot].SetRect(348,141+(nAISlot*39),348+32,141+(nAISlot*39)+32);

	}

	// Button Texture

	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();
	

	for(int i = 0; i < WILDPET_SLOT_TOTAL; i++)
	{
		m_btTabSlot[i].Create(this,CTString( "" ),10,41+(i*(3+100)),21,100);
		m_btTabSlot[i].SetUV(UBS_IDLE,86,0,107,101,fTexWidth,fTexHeight);
		m_btTabSlot[i].SetUV(UBS_CLICK,59,0,80,101,fTexWidth,fTexHeight);
		m_btTabSlot[i].CopyUV(UBS_IDLE,UBS_ON);
		m_btTabSlot[i].CopyUV(UBS_IDLE,UBS_DISABLE);
	}

	m_btTabSlot[WILDPET_INFO_SLOT].SetText(_S(4204,"기본정보"),TRUE);
	m_btTabSlot[WILDPET_SKILL_SLOT].SetText(_S(4205,"스킬정보"),TRUE);

	m_btnClose.Create(this,CTString(""),247,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211,33,226,48,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229,33,244,48,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);


	m_btnPoint[WILDPET_STR].Create(this,CTString(""),126,215,15,15);
	m_btnPoint[WILDPET_STR].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_STR].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_STR].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_STR].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_CON].Create(this,CTString(""),234,215,15,15);
	m_btnPoint[WILDPET_CON].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_CON].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_CON].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_CON].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_DEX].Create(this,CTString(""),126,237,15,15);
	m_btnPoint[WILDPET_DEX].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_DEX].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_DEX].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_DEX].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_INT].Create(this,CTString(""),234,237,15,15);
	m_btnPoint[WILDPET_INT].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_INT].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_INT].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_INT].CopyUV(UBS_IDLE,UBS_DISABLE);
		
	m_sbSkillList.Create( this, 246, 74, 10, 307 );
	m_sbSkillList.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbSkillList.SetScrollPos( 0 );
	m_sbSkillList.SetScrollRange( 0 );
	m_sbSkillList.SetItemsPerPage( 8 );
	// Up button
	m_sbSkillList.SetUpUV( UBS_IDLE, 156,33,165,42, fTexWidth, fTexHeight );
	m_sbSkillList.SetUpUV( UBS_CLICK, 168,33,177,42,  fTexWidth, fTexHeight );
	m_sbSkillList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSkillList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbSkillList.SetDownUV( UBS_IDLE, 156, 45, 165, 54, fTexWidth, fTexHeight );
	m_sbSkillList.SetDownUV( UBS_CLICK, 168, 45, 177, 54, fTexWidth, fTexHeight );
	m_sbSkillList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSkillList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbSkillList.SetBarTopUV( 185, 32, 194, 40, fTexWidth, fTexHeight );
	m_sbSkillList.SetBarMiddleUV( 185, 40, 194, 62, fTexWidth, fTexHeight );
	m_sbSkillList.SetBarBottomUV( 185, 62, 194, 70, fTexWidth, fTexHeight );
	// Wheel region
	m_sbSkillList.SetWheelRect( -205, 0, 215, 307 );

	m_abtnWearing[WILDPET_WEAR_HEAD].Create(this,129,120,32,32,UI_WILDPET_INFO,UBET_ITEM);
	m_abtnWearing[WILDPET_WEAR_BODY].Create(this,129,159,32,32,UI_WILDPET_INFO,UBET_ITEM);
	m_abtnWearing[WILDPET_WEAR_WEAPON].Create(this,88,139,32,32,UI_WILDPET_INFO,UBET_ITEM);
	m_abtnWearing[WILDPET_WEAR_ACCE].Create(this,171,140,32,32,UI_WILDPET_INFO,UBET_ITEM);

	for (int sSlot = 0; sSlot < 8; sSlot++)
	{
		m_abtnSkill[sSlot].Create(this,42,75+(sSlot*40),32,32,UI_WILDPET_INFO,UBET_SKILL);
	}

	for( nAISlot = 0; nAISlot < WILDPET_AI_MAXSLOT; nAISlot++)
	{
		m_cbtnAISlotActive[nAISlot].Create(this, 279, 150+(nAISlot*39), 15, 13, CTString(""), TRUE);
		m_cbtnAISlotActive[nAISlot].SetUV(UCBS_NONE, 139, 75, 153, 87, fTexWidth, fTexHeight);
		m_cbtnAISlotActive[nAISlot].SetUV(UCBS_CHECK, 119, 75, 133, 87, fTexWidth, fTexHeight);
		m_cbtnAISlotActive[nAISlot].CopyUV(UCBS_NONE, UCBS_CHECK_DISABLE);
		m_cbtnAISlotActive[nAISlot].CopyUV(UCBS_NONE, UCBS_NONE_DISABLE);
		m_cbtnAISlotActive[nAISlot].SetCheck(FALSE);		
	}

	m_btnAIMake.Create(this, _S(4021,"편집 완료"), 295, 374, 81, 22);
	m_btnAIMake.SetUV(UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight);
	m_btnAIMake.SetUV(UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight);
	m_btnAIMake.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAIMake.CopyUV( UBS_IDLE, UBS_DISABLE );

	// 임시
	m_nAISlotCount = 3;

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}
void CUIWildPetInfo::ToggleVisible()
{
	BOOL	bVisible = !IsVisible();

	if(bVisible)
	{
		OpenWildPetInfo();
	}else
	{
		CloseWildPetInfo();
	}

}

void CUIWildPetInfo::OpenWildPetInfo( )
{
	// If this is already exist
	if( IsVisible() )
		return;	

	/*sendSkillList();*/
	_pUIMgr->RearrangeOrder( UI_WILDPET_INFO, TRUE );

}

void CUIWildPetInfo::CloseWildPetInfo()
{
	PetWearItemReSet();
	PetAIReSet();
	_pUIMgr->RearrangeOrder( UI_WILDPET_INFO, FALSE );
}

void CUIWildPetInfo::AIClear()
{
	for (int nslot = 0; nslot < WILDPET_AI_MAXSLOT; nslot++)
	{
		m_sAISlot[nslot].m_bActive = FALSE;
		m_sAISlot[nslot].m_nSillType = -1;
		m_sAISlot[nslot].m_nActionSlot = -1;
		m_sAISlot[nslot].m_nTargetSlot = -1;

		m_cbtnAISlotActive[nslot].SetCheck(FALSE);
		m_abtAITarget[nslot].InitBtn();
		m_abtAIAction[nslot].InitBtn();
	
	}

}

void CUIWildPetInfo::Render()
{
	if(_pNetwork->_WildPetInfo.bIsActive == FALSE) return;
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	m_rtsBace.SetPos(m_nPosX,m_nPosY);
	m_rtsBace.RenderRectSurface(_pUIMgr->GetDrawPort(),0xFFFFFFFF);

	//Text
	CTString strTemp;
	strTemp.PrintF(_S(2173, "펫 정보"));
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+WILDPETINFO_WIDTH/2, m_nPosY+15);

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );

	for(int i = 0; i < WILDPET_SLOT_TOTAL; i++)
	{
		m_btTabSlot[i].Render();
	}
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	
	if(m_nTebnum == WILDPET_INFO_SLOT)
	{
		InfoRender();		
	}
	else if(m_nTebnum == WILDPET_SKILL_SLOT)
	{
		SkillRender();
	}

	if(m_bSlideUI)
	{
		AIPopupRender();		
	}
	
	_pUIMgr->GetDrawPort()->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

}
#define COLOR_RED RGBAToColor(255,0,0,255)
#define COLOR_YELLOW RGBAToColor(255,255,0,255)
void CUIWildPetInfo::InfoRender()
{
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	_pUIMgr->GetDrawPort()->AddTexture(m_nPosX+30,m_nPosY+37,m_nPosX+264,m_nPosY+393,
		m_rtInfoBace.U0,m_rtInfoBace.V0,m_rtInfoBace.U1,m_rtInfoBace.V1, 0xFFFFFFFF);

	//Text
	CTString strTemp;

	int tempboxheight = 15;
	strTemp.PrintF(_S(71, "이름"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+49);
	_pUIMgr->GetDrawPort()->PutTextExCX(_pNetwork->_WildPetInfo.m_strName, m_nPosX+170,m_nPosY+49);

	strTemp.PrintF(_S(73, "레벨"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+69);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nLevel);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+107,m_nPosY+69);

	strTemp.PrintF(_S(4206, "종족"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+141, m_nPosY+69);
	
	if (_pNetwork->_WildPetInfo.m_nType == 1)
	{
		strTemp.PrintF(_S(4207, "인간형"));
	}else if(_pNetwork->_WildPetInfo.m_nType == 2)
	{
		strTemp.PrintF(_S(4208, "동물형"));
	}
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+214,m_nPosY+69);

	strTemp.PrintF(_S(89, "경험치"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+89);
	strTemp.PrintF("%I64d / %I64d",_pNetwork->_WildPetInfo.m_exp,_pNetwork->_WildPetInfo.m_next_exp);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+170,m_nPosY+89);

	if (g_iCountry == USA) {
		strTemp.PrintF(_S(4406, "힘"));
	} else {
		strTemp.PrintF(_S(80, "힘"));
	}

	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+216);
	strTemp.PrintF("%d(%d)",_pNetwork->_WildPetInfo.m_nStr,_pNetwork->_WildPetInfo.m_nStrPlus);
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41+38, m_nPosY+216);

	if (g_iCountry == USA) {
		strTemp.PrintF(_S(4409, "체질"));
	} else {
		strTemp.PrintF(_S(83, "체질"));
	}
	
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+146, m_nPosY+216);
	strTemp.PrintF("%d(%d)",_pNetwork->_WildPetInfo.m_nCon,_pNetwork->_WildPetInfo.m_nConPlus);
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+146+38, m_nPosY+216);

	if (g_iCountry == USA) {
		strTemp.PrintF(_S(4407, "민첩"));
	} else {
		strTemp.PrintF(_S(81, "민첩"));
	}

	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+238);
	strTemp.PrintF("%d(%d)",_pNetwork->_WildPetInfo.m_nDex,_pNetwork->_WildPetInfo.m_nDexPlus);
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41+38, m_nPosY+238);

	if (g_iCountry == USA) {
		strTemp.PrintF(_S(4408, "지혜"));
	} else {
		strTemp.PrintF(_S(82, "지혜"));
	}
	
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+146, m_nPosY+238);
	strTemp.PrintF("%d(%d)",_pNetwork->_WildPetInfo.m_nInt,_pNetwork->_WildPetInfo.m_nIntPlus);
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+146+38, m_nPosY+238);

	strTemp.PrintF(_S(84, "성장포인트"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+258);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nLevelupPoint);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+258);

	// 충성도
	FLOAT temResult = (FLOAT)_pNetwork->_WildPetInfo.m_nFaith/(FLOAT)_pNetwork->_WildPetInfo.m_nMaxFaith *100.0f;
	if(temResult > 60)
	{
		m_rtFaithImage.SetUV(69,405,87,423,fTexWidth,fTexHeight);
	}else if(temResult > 20)
	{
		m_rtFaithImage.SetUV(92,405,110,423,fTexWidth,fTexHeight);
	}else
	{
		m_rtFaithImage.SetUV(115,405,133,423,fTexWidth,fTexHeight);
	}
	strTemp.PrintF(_S(4209, "충성도"));
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp,m_nPosX+234,m_nPosY+116);
	_pUIMgr->GetDrawPort()->AddTexture(m_nPosX+224,m_nPosY+130,m_nPosX+244,m_nPosY+150,
		m_rtFaithImage.U0,m_rtFaithImage.V0,m_rtFaithImage.U1,m_rtFaithImage.V1,0xFFFFFFFF);
	strTemp.PrintF("%.2f%%",temResult);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp,m_nPosX+234,m_nPosY+152);

	COLOR	temColor = 0xFFFFFFFF;

	// 배고품 정도에 따라 패널티가 주어짐
	if(temResult < 20)
	{
		temColor = COLOR_RED;
	}else
	{
		temColor = 0xFFFFFFFF;
	}

	strTemp.PrintF(_S(85, "공격력"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+281);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_ATTACK]);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+281,temColor);
	
	strTemp.PrintF(_S(86, "마법공격력"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+299);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_MAGIC_ATTACK]);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+299,temColor);

	
	strTemp.PrintF(_S(87, "방어력"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+317);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_DEFENCE]);
		_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+317,temColor);

	strTemp.PrintF(_S(88, "마법방어력"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+335);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_MAGIC_DEFENCE]);
		_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+335,temColor);

			// 스테미너
	temResult = (FLOAT)_pNetwork->_WildPetInfo.m_nStm/(FLOAT)_pNetwork->_WildPetInfo.m_nMaxStm *100.0f;
	if(temResult > 60)
	{
		m_rtStmImage.SetUV(0,405,18,423,fTexWidth,fTexHeight);
	}else if(temResult > 20)
	{
		m_rtStmImage.SetUV(23,405,41,423,fTexWidth,fTexHeight);
	}else
	{
		m_rtStmImage.SetUV(46,405,64,423,fTexWidth,fTexHeight);
	}

	strTemp.PrintF(_S(4210, "배고품"));
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp,m_nPosX+56,m_nPosY+116);
	_pUIMgr->GetDrawPort()->AddTexture(m_nPosX+46,m_nPosY+130,m_nPosX+66,m_nPosY+150,
		m_rtStmImage.U0,m_rtStmImage.V0,m_rtStmImage.U1,m_rtStmImage.V1,0xFFFFFFFF);
	strTemp.PrintF("%.2f%%",temResult);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp,m_nPosX+56,m_nPosY+152);


	// 배고품 정도에 따라 패널티가 주어짐
	if(temResult < 20)
	{
		temColor = COLOR_RED;
	}else
	{
		temColor = 0xFFFFFFFF;
	}

	strTemp.PrintF(_S(4211, "명중도"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+353);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_HITPOINT]);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+353,temColor);

	strTemp.PrintF(_S(4212, "회피"));
	_pUIMgr->GetDrawPort()->PutTextEx(strTemp, m_nPosX+41, m_nPosY+371);
	strTemp.PrintF("%d",_pNetwork->_WildPetInfo.m_nWildPetState[STATE_AVOID]);
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+371,temColor);

#ifndef DEL_PET_AI_EDIT
	m_btnAIEdit.Render();
	if (m_bAIActive)
	{
// 		m_btnAION.SetEnable(TRUE);
// 		m_btnAIOFF.SetEnable(FALSE);
// 		m_btnAION.Render();
		m_rvAIONOFF.SetUV(2, 375,26,399,fTexWidth,fTexHeight);
		
	}else
	{
// 		m_btnAION.SetEnable(FALSE);
// 		m_btnAIOFF.SetEnable(TRUE);
// 		m_btnAIOFF.Render();
		m_rvAIONOFF.SetUV(58, 375, 82, 399, fTexWidth, fTexHeight);
	}
	_pUIMgr->GetDrawPort()->AddTexture(m_rtAIONOFF.Left+m_nPosX, m_rtAIONOFF.Top+m_nPosY, m_rtAIONOFF.Right+m_nPosX, m_rtAIONOFF.Bottom+m_nPosY,
										m_rvAIONOFF.U0, m_rvAIONOFF.V0, m_rvAIONOFF.U1, m_rvAIONOFF.V1,0xFFFFFFFF);
#endif

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

	_pUIMgr->GetDrawPort()->InitTextureData(m_ptdButtonTexture);

	for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
	{
		m_btnPoint[nPoint].Render();
	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		if(_pNetwork->_WildPetInfo.m_nPetWearIndex[i] < 0) continue;
		m_abtnWearing[i].Render();
	}
		// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	if(m_nShowItem != -1)
	{
		m_abtnWearing[m_nShowItem].RenderInfoPopup();
	}

	

}

void CUIWildPetInfo::SkillRender()
{

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	_pUIMgr->GetDrawPort()->AddTexture(m_nPosX+30,m_nPosY+37,m_nPosX+264,m_nPosY+393,
		m_rtSkillBace.U0,m_rtSkillBace.V0,m_rtSkillBace.U1,m_rtSkillBace.V1, 0xFFFFFFFF);

	int	nX = 46, nY = 75;
	int	iRowE;
	int nCount = 0;
	LONG	nPetIndex = _pNetwork->_PetTargetInfo.lIndex;
	int nSelectID = -1;
	UIButton_map::iterator	iter;
	UIButton_map::iterator	iterSelect;
	UIButton_map::iterator	iterBegin;
	UIButton_map::iterator	iterEnd;
	
	LONG nSkillIconPos = m_sbSkillList.GetScrollPos();

	int nIconPos;

	iterBegin	= m_btnmapSkills.begin();
	iterEnd		= m_btnmapSkills.end();
	nIconPos	= nSkillIconPos;
	iterSelect	= m_btnmapSkills.find( m_nSelSkillIndex );
		
	if( iterSelect != iterEnd )
	{
		nSelectID = std::distance( iterBegin, iterSelect );
	}

	iter = iterBegin;

	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
			

		//Render Select OutLine
	iRowE = nIconPos + 4;

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		

	_pUIMgr->GetDrawPort()->InitTextureData(m_ptdButtonTexture);
	m_sbSkillList.SetCurItemCount( m_btnmapSkills.size() );

	m_sbSkillList.Render();

		// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
// Render Skill Info
	nY = 76;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	int nCharSP		= _pNetwork->MyCharacterInfo.sp;

	iter = iterBegin;
	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
	
	for( nCount = 0; ( iter != iterEnd ) && ( nCount < 8 ); iter++, nCount++, nY += 39 )
	{   
		if( iter->second->IsEmpty() ) continue;

		LONG nSkillIndex = iter->second->GetSkillIndex();
		CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIndex );
		SBYTE	sbLevel = iter->second->GetSkillLevel();
		BOOL	bDelay = iter->second->GetSkillDelay();
		CTString tempLv;
		tempLv.PrintF("Lv %d",sbLevel);

		m_abtnSkill[nCount].SetSkillInfo(nSkillIndex,sbLevel);
		m_abtnSkill[nCount].SetSkillDelay(bDelay);
		m_abtnSkill[nCount].Render();

		_pUIMgr->GetDrawPort()->PutTextExCX( rSkill.GetName(), m_nPosX + 150, m_nPosY + nY, 0xC5C5C5FF );
		_pUIMgr->GetDrawPort()->PutTextExCX( tempLv, m_nPosX + 150, m_nPosY + nY + 15, 0xC5C5C5FF );
	
	}	

	_pUIMgr->GetDrawPort()->EndTextEx();

			// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

	if(m_nShowSkill != -1)
	{
		m_abtnSkill[m_nShowSkill].RenderInfoPopup();
	}

}

WMSG_RESULT CUIWildPetInfo::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT	wmsgResult;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	static INDEX clickWearpos = -1;
	static INDEX clickSkillpos = -1;
	static INDEX clickAITargetpos = -1;
	static INDEX clickAIActionpos = -1;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInBtn && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				// Close message box of inventory
				_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

				if(clickWearpos != -1)
				{
					_pUIMgr->SetHoldBtn( m_abtnWearing[clickWearpos]);
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
					
					clickWearpos = -1;
					m_abtnWearing[clickWearpos].SetBtnState( UBES_IDLE );

				}else if (clickSkillpos != -1)
				{
					_pUIMgr->SetHoldBtn( m_abtnSkill[clickSkillpos]);
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					clickSkillpos = -1;
					 m_abtnSkill[clickSkillpos].SetBtnState( UBES_IDLE );

				}else if (clickAITargetpos != -1)
				{
					_pUIMgr->SetHoldBtn( m_abtAITarget[clickAITargetpos]);
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

				//	clickAITargetpos = -1;
					 m_abtAITarget[clickAITargetpos].SetBtnState( UBES_IDLE );

				}else if (clickAIActionpos != -1)
				{
					_pUIMgr->SetHoldBtn( m_abtAIAction[clickAIActionpos]);
					int	nOffset = BTN_SIZE / 2;
					_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

				//	clickAIActionpos = -1;
					 m_abtAIAction[clickAIActionpos].SetBtnState( UBES_IDLE );
				}
				bLButtonDownInBtn = FALSE;
			}
			// Move skill learn
			if( m_bTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			if(IsInside(nX,nY))
			{
				if(m_nTebnum == WILDPET_INFO_SLOT)
				{
					for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nShowItem = iWear;
							return WMSG_SUCCESS;
						}
					}
				}else if(m_nTebnum == WILDPET_SKILL_SLOT)				
				{
					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if(m_abtnSkill[nSkillnum].MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_nShowSkill = nSkillnum;
							return WMSG_SUCCESS;
						}
						
					}
				}

			}else if(IsInsideRect(nX,nY,UIRect(273,118,397,400)))
			{
				for (int nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
				{
					if (m_abtAIAction[nSlot].MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_nAIShowSkill = nSlot;
						return WMSG_SUCCESS;
					} 
					else if(m_abtAITarget[nSlot].MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_nAIShowItem = nSlot;
						return WMSG_SUCCESS;
					}

				}

			}

		}break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}else if( IsInsideRect( nX, nY, m_rtTitle ) )
				{
					m_bTitle = TRUE;
				}			

				for( int ntab = 0; ntab < WILDPET_SLOT_TOTAL; ntab++ )
				{
					if(m_btTabSlot[ntab].MouseMessage(pMsg) != WMSG_FAIL)
					{

					}

				}
				
				if(m_nTebnum == WILDPET_INFO_SLOT)
				{
					for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
					{
						if(m_btnPoint[nPoint].MouseMessage(pMsg) != WMSG_FAIL)
						{
							
						}
						
					}
					
					for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							clickWearpos = iWear;
							bLButtonDownInBtn = TRUE;					
							return WMSG_SUCCESS;
						}
					}
				}else if(m_nTebnum == WILDPET_SKILL_SLOT)
				{
					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if(m_abtnSkill[nSkillnum].MouseMessage(pMsg) != WMSG_FAIL)
						{
							clickSkillpos = nSkillnum;
							bLButtonDownInBtn = TRUE;
							return WMSG_SUCCESS;
						}
					}
				}

				if (m_btnAIEdit.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}
				
				_pUIMgr->RearrangeOrder( UI_WILDPET_INFO, TRUE );
				return WMSG_SUCCESS;
			}else if(IsInsideRect(nX,nY,UIRect(273,118,397,400)))
			{
				if (m_btnAIMake.MouseMessage(pMsg) != WMSG_FAIL)
				{				
				}

				for (int nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
				{
					if (m_abtAITarget[nSlot].MouseMessage(pMsg) != WMSG_FAIL)
					{
						clickAITargetpos = nSlot;
						bLButtonDownInBtn = TRUE;
						return WMSG_SUCCESS;

					}
					
					if (m_abtAIAction[nSlot].MouseMessage(pMsg) != WMSG_FAIL)
					{
						clickAIActionpos = nSlot;
						bLButtonDownInBtn = TRUE;
						return WMSG_SUCCESS;
					}

					if (m_cbtnAISlotActive[nSlot].MouseMessage(pMsg) != WMSG_FAIL )
					{
					}
				}

				_pUIMgr->RearrangeOrder(UI_WILDPET_INFO, TRUE);
				return WMSG_SUCCESS;
			}
			
		}break;
	case WM_LBUTTONUP:
		{
			m_bTitle = FALSE;
			clickSkillpos = -1;
			clickWearpos = -1;
			bLButtonDownInBtn = FALSE;
			if(!_pUIMgr->GetHoldBtn().IsEmpty())
			{
				if(m_nTebnum == WILDPET_INFO_SLOT && IsInside(nX,nY))
				{
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
					{
						CItemData temItem = _pNetwork->GetItemData(_pUIMgr->GetHoldBtn().GetItemIndex());
						_pNetwork->SendUseWildPetItem(temItem.GetWearingPosition(),
									_pUIMgr->GetHoldBtn().GetItemTab(),	_pUIMgr->GetHoldBtn().GetItemRow(),
									_pUIMgr->GetHoldBtn().GetItemCol(),_pUIMgr->GetHoldBtn().GetItemIndex());
						
						
					}
					_pUIMgr->ResetHoldBtn();
				}
				else if (IsInsideRect(nX,nY,UIRect(273,118,397,400)))
				{
					int nSlotnum = 0;

					for ( nSlotnum = 0; nSlotnum < m_nAISlotCount; nSlotnum++)
					{
						if (IsInsideRect(nX,nY,m_rtAITarget[nSlotnum]))
						{
							if ( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
							{				
								CItemData curItem = _pNetwork->GetItemData(_pUIMgr->GetHoldBtn().GetItemIndex());

								if (curItem.GetSubType() == CItemData::ITEM_PET_AI)
								{
									m_abtAITarget[nSlotnum].Copy(_pUIMgr->GetHoldBtn());
								}								
							}

						}else if (IsInsideRect(nX,nY,m_rtAIAction[nSlotnum]))
						{

							if(_pUIMgr->GetHoldBtn().GetBtnType() == UBET_SKILL &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_WILDPET_INFO )
							{
								m_abtAIAction[nSlotnum].Copy(_pUIMgr->GetHoldBtn());							

							}else if (_pUIMgr->GetHoldBtn().GetBtnType() == UBET_ACTION &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_CHARACTERINFO )
							{
								CAction curAction =	_pNetwork->GetActionData(_pUIMgr->GetHoldBtn().GetItemIndex());			
								if (curAction.GetType() == 5)
								{
									m_abtAIAction[nSlotnum].Copy(_pUIMgr->GetHoldBtn());
								}								
							}
						}
					}

					_pUIMgr->ResetHoldBtn();

				}

				if (clickAIActionpos != -1)
				{
					m_abtAIAction[clickAIActionpos].InitBtn();
					clickAIActionpos = -1;
				} 
				else if(clickAITargetpos != -1)
				{
					m_abtAITarget[clickAITargetpos].InitBtn();
					clickAITargetpos = -1;
				}
				_pUIMgr->ResetHoldBtn();
				return WMSG_SUCCESS;

			}
			else if( IsInside( nX, nY ) )
			{
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseWildPetInfo();
					}
					return WMSG_SUCCESS;
				}

				for( int ntab = 0; ntab < WILDPET_SLOT_TOTAL; ntab++ )
				{
					if(m_btTabSlot[ntab].MouseMessage( pMsg ) == WMSG_COMMAND )
					{
						m_nTebnum = ntab;
						return WMSG_SUCCESS;
					}	
				}

				if(m_nTebnum == WILDPET_INFO_SLOT)
				{
#ifndef DEL_PET_AI_EDIT
					if (m_btnAIEdit.MouseMessage(pMsg) == WMSG_COMMAND)
					{
						if(m_bSlideUI)
						{
							PetAIReSet();
							m_bSlideUI = FALSE;
						}else m_bSlideUI = TRUE;

					}
				
					if (IsInsideRect(nX,nY,m_rtAIONOFF))
					{
						if (m_bAIActive)
						{
							SendAIActive(FALSE);

						}else
						{
							SendAIActive(TRUE);

						}
					}
#endif
					for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
					{
						if(m_btnPoint[nPoint].MouseMessage( pMsg ) == WMSG_COMMAND )
						{
							sendWildPetPointUP((UBYTE)nPoint);
							return WMSG_SUCCESS;
						}						
					}
				}else if(m_nTebnum == WILDPET_SKILL_SLOT)				
				{
					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if(m_abtnSkill[nSkillnum].MouseMessage(pMsg) == WMSG_COMMAND)
						{
							const int iSkillIndex = m_abtnSkill[nSkillnum].GetSkillIndex();
							if( iSkillIndex != -1 && !m_abtnSkill[nSkillnum].GetSkillDelay() )
							{
								UseSkill( iSkillIndex );
							}
							
						}
					}
					
				}		
				
				return WMSG_SUCCESS;
			}
			else if (IsInsideRect(nX,nY,UIRect(273,118,397,400)))
			{				
				if (m_btnAIMake.MouseMessage(pMsg) == WMSG_COMMAND)
				{
					SendAIList();
					return WMSG_SUCCESS;
				}
			}
		}break;
	case WM_LBUTTONDBLCLK:
		{
			if(IsInside(nX,nY)&& m_nTebnum == WILDPET_INFO_SLOT)
			{
				for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_abtnWearing[iWear].MouseMessage( pMsg ) == WMSG_COMMAND )
						{
							// Close message box of inventory
							_pUIMgr->CloseMessageBox( MSGCMD_DROPITEM );

							_pNetwork->SendUseWildPetItem(iWear,-1,-1,-1,_pNetwork->_WildPetInfo.m_nPetWearIndex[iWear]);
							
							return WMSG_SUCCESS;
						}
					}
				return WMSG_SUCCESS;
			}
		}break;
	case WM_MOUSEWHEEL:
		{
		}break;
	}
	return WMSG_FAIL;

}

BOOL CUIWildPetInfo::IsLearnSkill(int nSkillIndex )
{
	if( m_btnmapSkills.find( nSkillIndex ) != m_btnmapSkills.end() )
	{
			return TRUE;
	}
	else return FALSE;
}

INDEX CUIWildPetInfo::GetSkillLevel(int nSkillIndex )
{
	switch( _pNetwork->GetSkillData( nSkillIndex ).GetType() )
	{
	case CSkill::ST_MELEE:
		{
			return m_btnmapSkills[nSkillIndex]->GetSkillLevel();
		}
	}
	return -1;
}

void CUIWildPetInfo::AddSkill(int nSkillIndex, SBYTE sbSkillLevel )
{
	// 새로운 스킬을 추가한다
	CUIButtonEx* pbtnSkill = new CUIButtonEx();

	pbtnSkill->Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WILDPET_INFO,
											UBET_SKILL );

	pbtnSkill->SetSkillInfo( nSkillIndex, sbSkillLevel );
	UIButton_map::value_type tmpVal( nSkillIndex, pbtnSkill ); 
	
	m_btnmapSkills.insert( tmpVal );
	
}

void CUIWildPetInfo::UpdateSkill(int nSkillIndex, SBYTE sbLevel )
{
	switch( _pNetwork->GetSkillData( nSkillIndex ).GetType() )
	{
	case CSkill::ST_MELEE:
		{
			m_btnmapSkills[nSkillIndex]->SetSkillLevel( sbLevel );
		}
		break;

	}
}

void CUIWildPetInfo::SkillClear()
{
	m_btnmapSkills.clear();
}

void CUIWildPetInfo::UseSkill( int nIndex )
{	
	if (m_bAIActive)
	{
		SendAIActive(FALSE);
	}
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseWildSkill( _pNetwork->_WildPetInfo.pet_pEntity, nIndex );	
}

void CUIWildPetInfo::sendWildPetPointUP(UBYTE ubPoint)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_STATUP;
	nmMessage << (UBYTE)ubPoint;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::sendSkillList()
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_SKILLLIST;

	_pNetwork->SendToServerNew(nmMessage);

}

void CUIWildPetInfo::PetWearItemReSet()
{
	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		if(_pNetwork->_WildPetInfo.m_nPetWearIndex[i] < 0)
		{
			m_abtnWearing[i].InitBtn();
		}
	}
}

void CUIWildPetInfo::ReceiveWearItemData(CNetworkMessage *istr)
{
	SBYTE nWearpos;
	ULONG nItemIndex, nItemPlus;
	ULONG nFlag,nUsed,nUsed2;
	SBYTE	sbOptionCount;
	SBYTE	sbOptionType, sbOptionLevel;

	(*istr) >> nWearpos;
	(*istr) >> nItemIndex;
	(*istr) >> nItemPlus;
	(*istr) >> nFlag;
	(*istr) >> nUsed;
	(*istr) >> nUsed2;
	(*istr) >> sbOptionCount;

	m_abtnWearing[nWearpos].SetItemInfo( -1, -1, -1, nItemIndex, -1, nWearpos );
	m_abtnWearing[nWearpos].SetItemIndex(nItemIndex);
	m_abtnWearing[nWearpos].SetItemPlus(nItemPlus);
	m_abtnWearing[nWearpos].SetItemFlag(nFlag);
	m_abtnWearing[nWearpos].SetItemUsed(nUsed);
	m_abtnWearing[nWearpos].SetItemUsed2(nUsed2);

	CItemData& ItemData	= _pNetwork->GetItemData(nItemIndex);
	CItems temItem;

	//레어 아이템일때...
	if( ItemData.GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if( sbOptionCount ==0)
			temItem.SetRareIndex(0);
		//감정된 레어아이템이면...
		else
		{
			_pUIMgr->SetRareOption(istr, temItem);

			
		}

		m_abtnWearing[nWearpos].SetItemRareIndex(temItem.Item_RareIndex);

		for(SBYTE i = 0; i < MAX_ITEM_OPTION; i++)
		{
			m_abtnWearing[nWearpos].SetItemOptionData(i,temItem.GetOptionType(i),temItem.GetOptionLevel(i));
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
		{
			(*istr) >> sbOptionType;
			(*istr) >> sbOptionLevel;

			m_abtnWearing[nWearpos].SetItemOptionData( sbOption, sbOptionType, sbOptionLevel );
		}
	}
}

void CUIWildPetInfo::AddWildPetInfo( sPetItem_Info sPetInfo )
{
	sPetItem_Info TempData;
	TempData = sPetInfo;
	int nPetIndex = TempData.pet_index;

	WildPetInfo_map::iterator iter = m_mapWildPetInfo.find( nPetIndex );
	
	if( iter != m_mapWildPetInfo.end() )	//이미 가지고 있는 펫 데이터이면 업데이트...
		iter->second =TempData;
	else										//가지고 있지 않는 펫 데이터이면 추가...
	{
		WildPetInfo_map::value_type tmpVal( nPetIndex, TempData );
		m_mapWildPetInfo.insert( tmpVal );
	}
}
BOOL CUIWildPetInfo::GetWildPetInfo(int nPetIndex, sPetItem_Info& WildPetInfo )
{
	WildPetInfo_map::iterator PetInfoiter = m_mapWildPetInfo.find( nPetIndex );

	if( PetInfoiter != m_mapWildPetInfo.end() )
	{
		WildPetInfo = PetInfoiter->second;
		return TRUE;
	}else return FALSE;
}

void CUIWildPetInfo::AIPopupRender()
{
	_pUIMgr->GetDrawPort()->InitTextureData(m_ptdBaseTexture);
	
	m_rtsAIBace.SetPos(m_nPosX,m_nPosY);
	m_rtsAIBace.RenderRectSurface(_pUIMgr->GetDrawPort(),0xFFFFFFFF);

	//Text
	CTString strTemp;
	strTemp.PrintF(_S(4373, "펫 AI 설정"));
	_pUIMgr->GetDrawPort()->PutTextExCX(strTemp, m_nPosX+335, m_nPosY+123);

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();


	for(int nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{	
		if (!m_abtAITarget[nAISot].IsEmpty())
		{
			m_abtAITarget[nAISot].Render();
		}
		
	}
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	
	for(nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{
		if(m_abtAIAction[nAISot].GetBtnType() == UBET_SKILL && !m_abtAIAction[nAISot].IsEmpty())
			m_abtAIAction[nAISot].Render();
	}
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

	for(nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{
		if(m_abtAIAction[nAISot].GetBtnType() == UBET_ACTION && !m_abtAIAction[nAISot].IsEmpty())
			m_abtAIAction[nAISot].Render();
	}
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ACTION );

	_pUIMgr->GetDrawPort()->InitTextureData(m_ptdButtonTexture);

	for (nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{
		m_cbtnAISlotActive[nAISot].Render();
	}

	m_btnAIMake.Render();	

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

	if (m_nAIShowItem != -1)
	{
		m_abtAITarget[m_nAIShowItem].RenderInfoPopup();
	} 
	
	if(m_nAIShowSkill != -1)
	{
		m_abtAIAction[m_nAIShowSkill].RenderInfoPopup();
	}
	
}

void CUIWildPetInfo::AddAIData(int nSlotnum, sWildPet_AI_Slot sAIData)
{
	m_sAISlot[nSlotnum] = sAIData;
	
	m_cbtnAISlotActive[nSlotnum].SetCheck(m_sAISlot[nSlotnum].m_bActive);

}

void CUIWildPetInfo::ErrorMassage(UBYTE errorcode)
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(errorcode)
	{
	case MSG_APET_ERROR_OK:
		{
		//	strMessage = _S(1970, "성공");
			PetAIReSet();
			AIPopupClose();
			return;
		}break;
	case MSG_APET_ERROR:
		{
			strMessage = _S(1971, "실패");
		}break;
	case MSG_APET_ERR_NOT_HAVE_ITEM:
		{
			strMessage = _S(2907, "이용 가능한 아이템이 아닙니다.");

		}break;
	case MSG_APET_ERR_WRONG_AI:
		{
			strMessage = _S(1971, "실패");

		}break;
	}

	PetAIReSet();

	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_WILDPET_INFO, UI_NONE );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

void CUIWildPetInfo::SendAIList()
{
	if (m_nAISlotCount <= 0)
	{
		return;
	}

	int temSlotcount = 0;
	for (INDEX nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (!m_abtAITarget[nSlot].IsEmpty() && !m_abtAIAction[nSlot].IsEmpty())
		{
			temSlotcount++;
		}
	}
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_UPDATE_REQ;
	nmMessage << m_nAISlotCount;
	nmMessage << (INDEX)temSlotcount;

	for ( nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (m_abtAITarget[nSlot].IsEmpty() && m_abtAIAction[nSlot].IsEmpty())
		{
			continue;
		}
		
		nmMessage << nSlot;
		nmMessage << (ULONG)m_abtAITarget[nSlot].GetItemIndex();
		

		if (m_abtAIAction[nSlot].GetBtnType() == UBET_SKILL)
		{
			nmMessage << (ULONG)m_abtAIAction[nSlot].GetBtnType();
			nmMessage << (ULONG)m_abtAIAction[nSlot].GetSkillIndex();
		} 
		else if(m_abtAIAction[nSlot].GetBtnType() == UBET_ACTION)
		{
			nmMessage << (ULONG)m_abtAIAction[nSlot].GetBtnType();
			nmMessage << (ULONG)m_abtAIAction[nSlot].GetActionIndex();
		}else
		{
			nmMessage << (ULONG) -1;
			nmMessage << (ULONG) -1;
		}
		UBYTE bCheckd;
		if (m_cbtnAISlotActive[nSlot].IsChecked())
		{
			bCheckd = TRUE;
		}else bCheckd = FALSE;

		nmMessage << bCheckd;
	}


	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::SendAIActive(SBYTE bActive)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_ONOFF_REQ;
	nmMessage << bActive;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::PetAIReSet()
{
	for (int nSlot = 0; nSlot < WILDPET_AI_MAXSLOT; nSlot++)
	{
		m_cbtnAISlotActive[nSlot].SetCheck(FALSE);
		m_abtAITarget[nSlot].InitBtn();
		m_abtAIAction[nSlot].InitBtn();

		m_cbtnAISlotActive[nSlot].SetCheck(m_sAISlot[nSlot].m_bActive);

		if (m_sAISlot[nSlot].m_nTargetSlot > 0)
		{
			m_abtAITarget[nSlot].SetItemInfo( -1, -1, -1, m_sAISlot[nSlot].m_nTargetSlot, -1, -1 );
		}

		if (m_sAISlot[nSlot].m_nActionSlot > 0)
		{
			if(m_sAISlot[nSlot].m_nSillType == UBET_SKILL)
			{
				if(IsLearnSkill(m_sAISlot[nSlot].m_nActionSlot))
				{
					m_abtAIAction[nSlot].SetSkillInfo(m_sAISlot[nSlot].m_nActionSlot, GetSkillLevel(m_sAISlot[nSlot].m_nActionSlot));
					m_abtAIAction[nSlot].SetBtnType(UBET_SKILL);
				}
			}else
			{
				m_abtAIAction[nSlot].SetActionInfo(m_sAISlot[nSlot].m_nActionSlot);
				m_abtAIAction[nSlot].SetBtnType(UBET_ACTION);
			}
		}
	}
}

void CUIWildPetInfo::StartSkillDelay(int nSkillindex)
{
	if( nSkillindex == -1 ) 
		return;

	for (int nAISlot = 0; nAISlot < m_nAISlotCount; nAISlot++)
	{
		if (m_abtAIAction[nAISlot].GetSkillIndex() == nSkillindex)
		{
			m_abtAIAction[nAISlot].SetSkillDelay(TRUE);			
		}else
		{
			m_abtAIAction[nAISlot].SetSkillDelay(FALSE);
		}
	}

	UIButton_map::iterator	iter;	
	UIButton_map::iterator	iterBegin;
	UIButton_map::iterator	iterEnd;

	iterBegin	= m_btnmapSkills.begin();
	iterEnd		= m_btnmapSkills.end();
	iter		= iterBegin;
	for( INDEX nCount = 0; ( iter != iterEnd ) && ( nCount < 8 ); iter++, nCount++ )
	{
		if( iter->second->GetSkillIndex() == nSkillindex )
		{
			iter->second->SetSkillDelay( TRUE );
			return;			
		}
	}	

}

BOOL CUIWildPetInfo::GetSkillDelay(int nslot)
{
	DOUBLE	dDelayTime;
	DOUBLE	dElapsedTime;

	if (m_sAISlot[nslot].m_nSillType == 0)
	{
		CSkill	&rSkill = _pNetwork->GetSkillData( m_sAISlot[nslot].m_nActionSlot );
		dDelayTime = DOUBLE( rSkill.GetReUseTime() + _pNetwork->MyCharacterInfo.magicspeed ) / 10.0;
		int nCoolTimeReductionRate= _pUIMgr->GetCoolTimeReductionRate();
		if( nCoolTimeReductionRate >0)
			dDelayTime *= DOUBLE(100-nCoolTimeReductionRate)/100.0f;

		dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds() - rSkill.Skill_Data.Skill_StartTime;
		
		if( dElapsedTime > dDelayTime )				
		{
			m_abtAIAction[nslot].SetSkillDelay(FALSE);
		}

		return m_abtAIAction[nslot].GetSkillDelay();
	}

	return FALSE;

	
}
