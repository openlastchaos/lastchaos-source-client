#include "stdh.h"
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/LocalDefine.h>

static BOOL m_bLock = false;

extern int _aSummonSkill[5];

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIQuickSlot()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuickSlot::CUIQuickSlot()
{
	m_bHorizontal = TRUE;
	m_nCurrentPage = 0;
	m_strCurrentPage = CTString( "1" );
	m_nSelSlotBtnID = -1;
	m_bShowSlotInfo = FALSE;
	m_bShowExSlotInfo = FALSE;

	m_bGiftRecv =FALSE;
	m_eGiftPos =GIFTPOS_TOP;
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuickSlot()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuickSlot::~CUIQuickSlot()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
#define SHIFT_VAL 38
	//m_rcTitle1.SetRect( 0, 0, 14, 44 );
	m_rcTitle1.SetRect( 0, 17, 21, 70 );
//	m_rcTitle2.SetRect( 402+SHIFT_VAL, 0, 416+SHIFT_VAL, 44 );
	m_rcTitle2.SetRect( 416+SHIFT_VAL, 0, 430+SHIFT_VAL, 44 );
	
	//m_rcSlot.SetRect( 33, 3, 384 +70, 41 );
	m_rcSlot.SetRect( 39, 34, 455, 65 );

	//m_rcHorzSlotText.SetRect( 35, 6, 362, 13 );
	m_rcHorzSlotText.SetRect( 41, 36, 438, 42);
	m_rcVertSlotText.SetRect( 6, 35, 18, 357 );

	// Create quick slot texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\QuickSlot.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdGiftTexture = CreateTexture( CTString( "Data\\Interface\\Gift.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtBackground.SetUV( 0, 0+64, 440+SHIFT_VAL, 44+64, fTexWidth, fTexHeight );
	m_rtBackground.SetUV( 27, 127, 506, 198, fTexWidth, fTexHeight );

	// Slot information region
	//m_rtSlotInfoL.SetUV( 418, 2, 425, 25, fTexWidth, fTexHeight );
	//m_rtSlotInfoM.SetUV( 428, 2, 430, 25, fTexWidth, fTexHeight );
	//m_rtSlotInfoR.SetUV( 433, 2, 440, 25, fTexWidth, fTexHeight );
	m_rtSlotInfoL.SetUV( 239, 253, 246, 269, fTexWidth, fTexHeight );
	m_rtSlotInfoM.SetUV( 246, 253, 328, 269, fTexWidth, fTexHeight );
	m_rtSlotInfoR.SetUV( 328, 253, 335, 269, fTexWidth, fTexHeight );

	// Preview & next buttons
	//m_rtPrevHorzIdle.SetUV( 452, 3, 461, 16, fTexWidth, fTexHeight );
	//m_rtPrevHorzClick.SetUV( 467, 3, 476, 16, fTexWidth, fTexHeight );
	m_rtPrevHorzIdle.SetUV( 507, 278, 520, 291, fTexWidth, fTexHeight );
	m_rtPrevHorzClick.SetUV( 525, 278, 538, 291, fTexWidth, fTexHeight );
	m_rtPrevVertIdle.SetUV( 482, 5, 495, 14, fTexWidth, fTexHeight );
	m_rtPrevVertClick.SetUV( 482, 20, 495, 29, fTexWidth, fTexHeight );

	//m_rtNextHorzIdle.SetUV( 452, 20, 461, 33, fTexWidth, fTexHeight );
	//m_rtNextHorzClick.SetUV( 467, 20, 476, 33, fTexWidth, fTexHeight );
	m_rtNextHorzIdle.SetUV( 507, 298, 520, 311, fTexWidth, fTexHeight );
	m_rtNextHorzClick.SetUV( 525, 298, 538, 311, fTexWidth, fTexHeight );
	m_rtNextVertIdle.SetUV( 499, 5, 512, 14, fTexWidth, fTexHeight );
	m_rtNextVertClick.SetUV( 499, 20, 512, 29, fTexWidth, fTexHeight );

	m_rtRotHorzIdle.SetUV( 444, 37, 457, 50, fTexWidth, fTexHeight );
	m_rtRotHorzClick.SetUV( 458, 37, 471, 50, fTexWidth, fTexHeight );
	m_rtRotVertIdle.SetUV( 444, 51, 457, 64, fTexWidth, fTexHeight );
	m_rtRotVertClick.SetUV( 458, 51, 471, 64, fTexWidth, fTexHeight );

	// Outline of buttons
	m_rtBtnOutline.SetUV( 478,  30, 512, 64, fTexWidth, fTexHeight );

	// Slot text
	//m_rtHorzSlotText.SetUV( 5, 45, 332, 52, fTexWidth, fTexHeight );
	m_rtHorzSlotText.SetUV( 342, 319, 740, 326, fTexWidth, fTexHeight );
	m_rtVertSlotText.SetUV( 5, 52, 327, 64, fTexWidth, fTexHeight );

	// Quest Book Button
	m_rtQuestBook.SetUV( 387, 70, 420, 103, fTexWidth, fTexHeight );
	m_rtQuestBookDown.SetUV( 0, 0, 34, 34, fTexWidth, fTexHeight );
	// wooss 
	// Cash Shop open button
	//m_rtCashShop.SetUV( 422,70,455,103, fTexWidth, fTexHeight );
	//m_rtCashShopDown.SetUV( 35, 0, 69, 34, fTexWidth, fTexHeight );
	m_rtCashShop.SetUV( 364,127,473,155, fTexWidth, fTexHeight );
	m_rtCashShopDown.SetUV( 517, 170, 626, 198, fTexWidth, fTexHeight );


	//선물 도착 알림 버튼 UV	:Su-won
#ifdef CASH_GIFT
	fTexWidth = m_ptdGiftTexture->GetPixWidth();
	fTexHeight = m_ptdGiftTexture->GetPixHeight();
	m_rtGift[GIFTPOS_TOP][0].SetUV( 138,11,176,49, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_TOP][1].SetUV( 138,50,176,88, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_LEFT][0].SetUV( 50,10,96,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_LEFT][1].SetUV( 50,50,96,80, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_BOTTOM][0].SetUV( 98,2,136,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_BOTTOM][1].SetUV( 98,42,136,80, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_RIGHT][0].SetUV( 2,10,48,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_RIGHT][1].SetUV( 2,50,48,80, fTexWidth, fTexHeight );
#endif

	// Prev slot button
	//m_btnPrevSlot.Create( this, CTString( "" ), QSLOT_PREV_BTN_SX, QSLOT_PREV_BTN_SY, 9, 13 );
	m_btnPrevSlot.Create( this, CTString( "" ), 22, 33, 13, 13 );
	m_btnPrevSlot.CopyUV( UBS_IDLE, m_rtPrevHorzIdle );
	m_btnPrevSlot.CopyUV( UBS_CLICK, m_rtPrevHorzClick );
	m_btnPrevSlot.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrevSlot.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next slot button
	//m_btnNextSlot.Create( this, CTString( "" ), QSLOT_NEXT_BTN_SX, QSLOT_NEXT_BTN_SY, 9, 13 );
	m_btnNextSlot.Create( this, CTString( "" ), 22, 53, 13, 13 );
	m_btnNextSlot.CopyUV( UBS_IDLE, m_rtNextHorzIdle );
	m_btnNextSlot.CopyUV( UBS_CLICK, m_rtNextHorzClick );
	m_btnNextSlot.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNextSlot.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Rotate button
	m_btnRotate.Create( this, CTString( "" ), QSLOT_ROT_BTN_SX, QSLOT_ROT_BTN_SY, 13, 13 );
	m_btnRotate.CopyUV( UBS_IDLE, m_rtRotHorzIdle );
	m_btnRotate.CopyUV( UBS_CLICK, m_rtRotHorzClick );
	m_btnRotate.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRotate.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Quest Book
	m_btnQuestBook.Create( this, CTString(""), 387, 6, 32, 32 );
	m_btnQuestBook.CopyUV( UBS_IDLE, m_rtQuestBook );
	m_btnQuestBook.CopyUV( UBS_CLICK, m_rtQuestBookDown );
	m_btnQuestBook.CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuestBook.CopyUV( UBS_IDLE, UBS_DISABLE );

	// wooss 050815
	// Cash Shop button
	//m_btnCashShop.Create( this, CTString(""), 422, 6, 32, 32 );
	m_btnCashShop.Create( this, CTString(""), 337, 0, 108, 27 );
	m_btnCashShop.CopyUV( UBS_IDLE, m_rtCashShop );
	m_btnCashShop.CopyUV( UBS_CLICK, m_rtCashShopDown );
	m_btnCashShop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCashShop.CopyUV( UBS_IDLE, UBS_DISABLE );

	//선물 도착 알림 버튼	:Su-won
	//m_btnGiftNotice.Create( this, CTString( "" ), 422-15, 6-(QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW), 
	//												QSLOT_GIFT_BTN_X, QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW);
	m_btnGiftNotice.Create( this, CTString( "" ), m_btnCashShop.GetPosX(), m_btnCashShop.GetPosY()-32, 
													QSLOT_GIFT_BTN_X, QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW);
	m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[GIFTPOS_TOP][0] );
	m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[GIFTPOS_TOP][1] );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buttons of quick slot
	int	iPage, iBtn;
	int	nPosX;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		nPosX = QSLOT_BTN_SX;
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
		{
			m_abtnItems[iPage][iBtn].Create( this, nPosX, QSLOT_BTN_SY, BTN_SIZE, BTN_SIZE,
												UI_QUICKSLOT, UBET_SKILL );
		}
	}

	m_btnRotate.SetEnable(FALSE);					// Button for rotating this UI
	m_btnQuestBook.SetEnable(FALSE);				// Button for Quest Book
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth, pixMaxJ - m_nHeight );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ShowSlotInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ShowSlotInfo( BOOL bShowInfo, int nSlotIndex )
{
	static int	nOldSlotID = -1;

	// Hide slot information
	if( !bShowInfo || nSlotIndex < 0 )
	{
		m_bShowSlotInfo = FALSE;
		nOldSlotID = -1;
		return;
	}

	int		nSlotID = nSlotIndex + m_nCurrentPage * QSLOT_BTN_COUNT;
	BOOL	bUpdateInfo = FALSE;
	int		nInfoPosX, nInfoPosY;

	// Update slot information
	if( nOldSlotID != nSlotID )
	{
		CUIButtonEx	& rbtnSelect = m_abtnItems[m_nCurrentPage][nSlotIndex];

		if( rbtnSelect.IsEmpty() )
		{
			m_bShowSlotInfo = FALSE;
		}
		else
		{			
			m_bShowSlotInfo = TRUE;
			bUpdateInfo = TRUE;
			nOldSlotID = nSlotID;

			rbtnSelect.GetAbsPos( nInfoPosX, nInfoPosY );

			// Get slot information
			switch( rbtnSelect.GetBtnType() )
			{
			// Skill
			case UBET_SKILL:
				{
					int		nSkillIndex = rbtnSelect.GetSkillIndex();
					CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIndex );
					int		nSkillLevel = rbtnSelect.GetSkillLevel() - 1;
					int		nNeedMP = rSkill.GetNeedMP( nSkillLevel );
					int		nNeedHP = rSkill.GetNeedHP( nSkillLevel );
					if( nNeedHP == 0 )
					{
						if( nNeedMP == 0 )
							m_strSlotInfo.PrintF( "%s", rSkill.GetName() );
						else
						{
							//MP소모량 감소 옵션이 붙은 장비를 입고 있으면 감소되는 MP량 
							if( _pUIMgr->GetNeedMPReductionRate() >0)
							{
								int nNeedMPReduction = (nNeedMP*_pUIMgr->GetNeedMPReductionRate())/100;
								m_strSlotInfo.PrintF( "%s (MP:%d -%d)", rSkill.GetName(), nNeedMP, nNeedMPReduction);
							}
							else
								m_strSlotInfo.PrintF( "%s (MP:%d)", rSkill.GetName(), nNeedMP );
						}
					}
					else
					{
						if( nNeedMP == 0 )
							m_strSlotInfo.PrintF( "%s (HP:%d)", rSkill.GetName(), nNeedHP );
						else
						{
							if( _pUIMgr->GetNeedMPReductionRate() >0)
							{
								int nNeedMPReduction = (nNeedMP*_pUIMgr->GetNeedMPReductionRate())/100;
								m_strSlotInfo.PrintF( "%s (MP:%d -%d,HP:%d)", rSkill.GetName(), nNeedMP, nNeedMPReduction, nNeedHP );
							}
							else
								m_strSlotInfo.PrintF( "%s (MP:%d,HP:%d)", rSkill.GetName(), nNeedMP, nNeedHP );
						}
					}
				}
				break;

			// Action
			case UBET_ACTION:
				{
					int	nActionIndex = rbtnSelect.GetActionIndex();
					m_strSlotInfo = _pNetwork->GetActionData(nActionIndex).GetName();
				}
				break;

			// Item
			case UBET_ITEM:
				{
					int	nTab = rbtnSelect.GetItemTab();
					int	nRow = rbtnSelect.GetItemRow();
					int	nCol = rbtnSelect.GetItemCol();
					CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
					CItemData	&rItemData = rItems.ItemData;
					const char* szItemName = _pNetwork->GetItemName( rItemData.GetItemIndex() );

					if( rItemData.GetFlag() & ITEM_FLAG_COUNT )
					{
						CTString	strCount;
						strCount.PrintF( "%I64d", rItems.Item_Sum );
						_pUIMgr->InsertCommaToString( strCount );
						m_strSlotInfo.PrintF( "%s (%s)", szItemName, strCount );
					}
					else
						m_strSlotInfo = szItemName;
				}
				break;
			}

			// NOTE : 툴팁 표시할때는 QUICKSLOT을 제일 위로 올라오게 함.
			_pUIMgr->RearrangeOrder( UI_QUICKSLOT, TRUE );
		}
	}

	// Update slot information box
	if( bUpdateInfo )
	{
		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strSlotInfo);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strSlotInfo.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoPosX += ( BTN_SIZE - nInfoWidth ) / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX += _pUIMgr->GetMaxI() - ( nInfoPosX + nInfoWidth );

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE + 2;
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowSlotInfo )
	{
		nOldSlotID = -1;
		m_bShowSlotInfo = FALSE;
	}
}


// ----------------------------------------------------------------------------
// Name : ShowSlotInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ShowExSlotInfo( BOOL bShowInfo, int nSlotIndex )
{
	// Hide slot information
	if( !bShowInfo || nSlotIndex < 0 )
	{
		m_bShowExSlotInfo = FALSE;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoPosX, nInfoPosY;

	// Update slot information
	{
			m_bShowExSlotInfo = TRUE;
			bUpdateInfo = TRUE;
		
			// Get slot information
			switch( nSlotIndex )
			{
				case 0:
					{
						m_btnCashShop.GetAbsPos( nInfoPosX, nInfoPosY );
						m_strSlotInfo = _S(99,"퀘스트");					
					}
					break;

				case 1:
					{
						m_btnCashShop.GetAbsPos( nInfoPosX, nInfoPosY );
						m_strSlotInfo = _S(2572,"아이리스 상점");
					}
					break;
	
				//선물 도착 알림 :Su-won
				case 2:
					{
						m_btnGiftNotice.GetAbsPos( nInfoPosX, nInfoPosY );
						m_strSlotInfo = _S(3112, "선물 도착");
					}
					break;
				////////////////////////////////////////////////////////////////////////////
				// [070604: Su-won] 1차 펫 기능 개선	|----->
				//이전 페이지 버튼
				case 3:
					{
						m_btnPrevSlot.GetAbsPos( nInfoPosX, nInfoPosY );
						int PrevPage = m_nCurrentPage>0 ? m_nCurrentPage : 3 ;
						CTString str;
						str.PrintF("%d %s(Alt+%d)", PrevPage, _S(3076, "페이지"), PrevPage);
						m_strSlotInfo =str;

					}
					break;
				//다음 페이지 버튼
				case 4:
					{
						m_btnNextSlot.GetAbsPos( nInfoPosX, nInfoPosY );
						int NextPage = (m_nCurrentPage+2)<=3 ? m_nCurrentPage+2 : 1;
						CTString str;
						str.PrintF("%d %s(Alt+%d)", NextPage, _S(3076, "페이지"), NextPage);
						m_strSlotInfo =str;

					}
					break;
				// [070604: Su-won] 1차 펫 기능 개선	<-----|
				////////////////////////////////////////////////////////////////////////////
			}

			// NOTE : 툴팁 표시할때는 QUICKSLOT을 제일 위로 올라오게 함.
			_pUIMgr->RearrangeOrder( UI_QUICKSLOT, TRUE );
	}

	// Update slot information box
	if( bUpdateInfo )
	{
		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strSlotInfo);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strSlotInfo.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoPosX += ( BTN_SIZE - nInfoWidth ) / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX += _pUIMgr->GetMaxI() - ( nInfoPosX + nInfoWidth );

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE + 2;
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowExSlotInfo )
	{
		m_bShowExSlotInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RenderBtns()
{
	// Outline of buttons
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	if( m_bHorizontal )
	{
		int	nX = m_nPosX + QSLOT_BTN_OUTLINE_SX;
		int	nY = m_nPosY + QSLOT_BTN_OUTLINE_SY;
		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nX += 35 )
		{
			if( m_abtnItems[m_nCurrentPage][iBtn].IsEmpty() )
				continue;

			/***
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
												nX + QSLOT_BTN_OUTLINE_SIZE, nY + QSLOT_BTN_OUTLINE_SIZE,
												m_rtBtnOutline.U0, m_rtBtnOutline.V0,
												m_rtBtnOutline.U1, m_rtBtnOutline.V1,
												0xFFFFFFFF );
			***/
		}
	}
	else
	{
		int	nX = m_nPosX + QSLOT_BTN_OUTLINE_SY;
		int	nY = m_nPosY + QSLOT_BTN_OUTLINE_SX;
		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nY += 35 )
		{
			if( m_abtnItems[m_nCurrentPage][iBtn].IsEmpty() )
				continue;

			_pUIMgr->GetDrawPort()->AddTexture( nX, nY,
												nX + QSLOT_BTN_OUTLINE_SIZE, nY + QSLOT_BTN_OUTLINE_SIZE,
												m_rtBtnOutline.U0, m_rtBtnOutline.V0,
												m_rtBtnOutline.U1, m_rtBtnOutline.V1,
												0xFFFFFFFF );
		}
	}

	m_btnQuestBook.Render();
	m_btnCashShop.Render();
	
	RenderGiftButton();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Skill buttons
	for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
	{
		if( m_abtnItems[m_nCurrentPage][iBtn].IsEmpty() ||
			m_abtnItems[m_nCurrentPage][iBtn].GetBtnType() != UBET_SKILL )
			continue;

		m_abtnItems[m_nCurrentPage][iBtn].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

	// Action buttons
	for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
	{
		if( m_abtnItems[m_nCurrentPage][iBtn].IsEmpty() ||
			m_abtnItems[m_nCurrentPage][iBtn].GetBtnType() != UBET_ACTION )
			continue;

		m_abtnItems[m_nCurrentPage][iBtn].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ACTION );

	// Item buttons
	for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
	{
		if( m_abtnItems[m_nCurrentPage][iBtn].IsEmpty() ||
			m_abtnItems[m_nCurrentPage][iBtn].GetBtnType() != UBET_ITEM )
			continue;

		m_abtnItems[m_nCurrentPage][iBtn].Render();
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	// Slot text ( F1, F2, ..., F10 )
	if( m_bHorizontal )
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHorzSlotText.Left, m_nPosY + m_rcHorzSlotText.Top,
											m_nPosX + m_rcHorzSlotText.Right, m_nPosY + m_rcHorzSlotText.Bottom,
											m_rtHorzSlotText.U0, m_rtHorzSlotText.V0, m_rtHorzSlotText.U1, m_rtHorzSlotText.V1,
											0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcVertSlotText.Left, m_nPosY + m_rcVertSlotText.Top,
											m_nPosX + m_rcVertSlotText.Right, m_nPosY + m_rcVertSlotText.Bottom,
											m_rtVertSlotText.U0, m_rtVertSlotText.V1, m_rtVertSlotText.U1, m_rtVertSlotText.V1,
											m_rtVertSlotText.U1, m_rtVertSlotText.V0, m_rtVertSlotText.U0, m_rtVertSlotText.V0,
											0xFFFFFFFF );
	}

	// Slot information
	if( m_bShowSlotInfo || m_bShowExSlotInfo)
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left, m_rcSlotInfo.Top,
											m_rcSlotInfo.Left + 7, m_rcSlotInfo.Bottom,
											m_rtSlotInfoL.U0, m_rtSlotInfoL.V0, m_rtSlotInfoL.U1, m_rtSlotInfoL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left + 7, m_rcSlotInfo.Top,
											m_rcSlotInfo.Right - 7, m_rcSlotInfo.Bottom,
											m_rtSlotInfoM.U0, m_rtSlotInfoM.V0, m_rtSlotInfoM.U1, m_rtSlotInfoM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Right - 7, m_rcSlotInfo.Top,
											m_rcSlotInfo.Right, m_rcSlotInfo.Bottom,
											m_rtSlotInfoR.U0, m_rtSlotInfoR.V0, m_rtSlotInfoR.U1, m_rtSlotInfoR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render item information
		_pUIMgr->GetDrawPort()->PutTextEx( m_strSlotInfo, m_rcSlotInfo.Left + 8, m_rcSlotInfo.Top + 3 );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	else
	{
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::Render()
{
	// Set quick slot texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	if( m_bHorizontal )
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
											0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V0,
											m_rtBackground.U1, m_rtBackground.V1, m_rtBackground.U0, m_rtBackground.V1,
											0xFFFFFFFF );
	}

	// Prev button
	m_btnPrevSlot.Render();

	// Next button
	m_btnNextSlot.Render();

	// Rotate button
	//m_btnRotate.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Text of current quick slot tab
	/***
	if( m_bHorizontal )
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( m_strCurrentPage, m_nPosX + QSLOT_TAB_TEXT_CX,
												m_nPosY + QSLOT_TAB_TEXT_SY );
	}
	else
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( m_strCurrentPage, m_nPosX + QSLOT_TAB_TEXT_CX,
												m_nPosY + QSLOT_TAB_TEXT_SY );
	}
	***/

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Buttons
	RenderBtns();
}

// ----------------------------------------------------------------------------
// Name : RemoveSummonSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemoveSummonSkill()
{
	const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);

	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if( m_abtnItems[iPage][iBtn].GetBtnType() == UBET_SKILL )
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn].GetSkillIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );

				// 소환스킬 인덱스 찾구,
				// 소환수 사용 스킬.
				if( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) )
				{			
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}
				else
				{
					for( int i = 0; i < iSummonSkillCount; ++i )
					{
						if( _aSummonSkill[i] == iSkillIndex )
						{					
							// Remove button
							SendAddBtn( iPage, iBtn, -1 );			
						}
					}
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RemoveElementalSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemoveElementalSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if( m_abtnItems[iPage][iBtn].GetBtnType() == UBET_SKILL )
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn].GetSkillIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );

				// 소환스킬 인덱스 찾구,
				// 소환수 사용 스킬.
				if( rSelSkill.GetSorcererFlag() & ( SSF_USE_HELLOUND | SSF_USE_ELENEN ) )
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RemovePetSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemovePetSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if( m_abtnItems[iPage][iBtn].GetBtnType() == UBET_SKILL )
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn].GetSkillIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 애완동물 스킬을 찾아서 제거.
				if( rSelSkill.GetType() == CSkill::ST_PET_SKILL_ACTIVE || 
					rSelSkill.GetType() == CSkill::ST_PET_COMMAND )
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}

void CUIQuickSlot::RemoveWildPetSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if( m_abtnItems[iPage][iBtn].GetBtnType() == UBET_SKILL )
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn].GetSkillIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 애완동물 스킬을 찾아서 제거.
				if(rSelSkill.GetJob() == WILDPET_JOB)
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RotateQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RotateQuickSlot()
{
	m_bHorizontal = !m_bHorizontal;

	// Get pivot point
	int	nPivotX = m_nPosX + m_nWidth;
	int	nPivotY = m_nPosY + m_nHeight;

	// Rotate background
	int	nHeight = m_nHeight;
	m_nHeight = m_nWidth;
	m_nWidth = nHeight;
	SetPos( nPivotX - m_nWidth, nPivotY - m_nHeight );

	// Rotate title1
	int	nRight = m_rcTitle1.Right;
	m_rcTitle1.Right = m_rcTitle1.Bottom;
	m_rcTitle1.Bottom = nRight;

	// Rotate title2
	int	nTemp = m_rcTitle2.Left;
	m_rcTitle2.Left = m_rcTitle2.Top;
	m_rcTitle2.Top = nTemp;
	nTemp = m_rcTitle2.Right;
	m_rcTitle2.Right = m_rcTitle2.Bottom;
	m_rcTitle2.Bottom = nTemp;

	// Rotate region of slot
	nTemp = m_rcSlot.Left;
	m_rcSlot.Left = m_rcSlot.Top;
	m_rcSlot.Top = nTemp;
	nTemp = m_rcSlot.Right;
	m_rcSlot.Right = m_rcSlot.Bottom;
	m_rcSlot.Bottom = nTemp;

	// Rotate buttons
	int	nWidth = m_btnPrevSlot.GetHeight();
	nHeight = m_btnPrevSlot.GetWidth();
	m_btnPrevSlot.SetSize( nWidth, nHeight );

	nWidth = m_btnNextSlot.GetHeight();
	nHeight = m_btnNextSlot.GetWidth();
	m_btnNextSlot.SetSize( nWidth, nHeight );

	nWidth = m_btnRotate.GetHeight();
	nHeight = m_btnRotate.GetWidth();
	m_btnRotate.SetSize( nWidth, nHeight );

	int	iPage, iBtn, nX, nY;
	if( m_bHorizontal )
	{
		m_btnPrevSlot.SetPos( QSLOT_PREV_BTN_SX, QSLOT_PREV_BTN_SY );
		m_btnPrevSlot.CopyUV( UBS_IDLE, m_rtPrevHorzIdle );
		m_btnPrevSlot.CopyUV( UBS_CLICK, m_rtPrevHorzClick );

		m_btnNextSlot.SetPos( QSLOT_NEXT_BTN_SX, QSLOT_NEXT_BTN_SY );
		m_btnNextSlot.CopyUV( UBS_IDLE, m_rtNextHorzIdle );
		m_btnNextSlot.CopyUV( UBS_CLICK, m_rtNextHorzClick );

		m_btnRotate.SetPos( QSLOT_ROT_BTN_SX, QSLOT_ROT_BTN_SY );
		m_btnRotate.CopyUV( UBS_IDLE, m_rtRotHorzIdle );
		m_btnRotate.CopyUV( UBS_CLICK, m_rtRotHorzClick );

		m_btnQuestBook.SetPos( 387, 6 );	//wooss 050815
		m_btnCashShop.SetPos( 422, 6 );

		// 선물 도착 알림 버튼 설정	:Su-won
		m_btnGiftNotice.SetPos( 422-15, 6-(QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW) );
		m_btnGiftNotice.SetSize(QSLOT_GIFT_BTN_X, QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW);
		m_eGiftPos =GIFTPOS_TOP;

		for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
		{
			nX = QSLOT_BTN_SX;
			for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nX += 35 )
			{
				m_abtnItems[iPage][iBtn].SetPos( nX, QSLOT_BTN_SY );
			}
		}
	}
	else
	{
		m_btnPrevSlot.SetPos( QSLOT_PREV_BTN_SY, QSLOT_PREV_BTN_SX );
		m_btnPrevSlot.CopyUV( UBS_IDLE, m_rtPrevVertIdle );
		m_btnPrevSlot.CopyUV( UBS_CLICK, m_rtPrevVertClick );

		m_btnNextSlot.SetPos( QSLOT_NEXT_BTN_SY, QSLOT_NEXT_BTN_SX );
		m_btnNextSlot.CopyUV( UBS_IDLE, m_rtNextVertIdle );
		m_btnNextSlot.CopyUV( UBS_CLICK, m_rtNextVertClick );

		m_btnRotate.SetPos( QSLOT_ROT_BTN_SY, QSLOT_ROT_BTN_SX );
		m_btnRotate.CopyUV( UBS_IDLE, m_rtRotVertIdle );
		m_btnRotate.CopyUV( UBS_CLICK, m_rtRotVertClick );

		m_btnQuestBook.SetPos( 6, 387 );	//wooss
		m_btnCashShop.SetPos( 6, 422 );

		// 선물 도착 알림 버튼 설정	:Su-won
		m_btnGiftNotice.SetPos(6-(QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW), 422-10);
		m_btnGiftNotice.SetSize(QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW, QSLOT_GIFT_BTN_Y);
		m_eGiftPos =GIFTPOS_LEFT;

		for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
		{
			nY = QSLOT_BTN_SX;
			for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nY += 35 )
			{
				m_abtnItems[iPage][iBtn].SetPos( QSLOT_BTN_SY, nY );
			}
		}
	}
	m_btnPrevSlot.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrevSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnNextSlot.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNextSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnRotate.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRotate.CopyUV( UBS_IDLE, UBS_DISABLE );

	SetGiftNoticePos();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuickSlot::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	m_bShowExSlotInfo = FALSE;

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold button
			if( _pUIMgr->GetHoldBtn().IsEmpty() && bLButtonDownInBtn && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				if( m_nSelSlotBtnID >= 0 )
				{
			/*
					UseQuickSlot( m_nSelSlotBtnID );
					// Reset holding button
					bLButtonDownInBtn = FALSE;
					return WMSG_FAIL;
			*/		
					ndX = abs( ndX );
					ndY = abs( ndY );
					
					static int nD = 7;
				
					CPrintF( "%d, %d", ndX, ndY );
					if( ndX > nD || ndY > nD )
					{
						int	nSelTab = m_nSelSlotBtnID / QSLOT_BTN_COUNT;
						int	nSelBtn = m_nSelSlotBtnID % QSLOT_BTN_COUNT;
						
						_pUIMgr->SetHoldBtn( m_abtnItems[nSelTab][nSelBtn] );
						int	nOffset = BTN_SIZE / 2;
						_pUIMgr->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

						m_abtnItems[nSelTab][nSelBtn].SetBtnState( UBES_IDLE );
						bLButtonDownInBtn = FALSE;
					}
				}
				
			}

			// Move quick slot
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				SetGiftNoticePos();
				
				return WMSG_SUCCESS;
			}
			// Prev button
			else if( m_btnPrevSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
#ifdef Pet_IMPROVEMENT_1ST
				// [070604: Su-won] 1차 펫 기능 개선
				// 퀵슬롯 이전 페이지 버튼 단축키 표시
				ShowExSlotInfo(TRUE, 3);
#endif
				return WMSG_SUCCESS;
			}
			// Next button
			else if( m_btnNextSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
#ifdef Pet_IMPROVEMENT_1ST
				// [070604: Su-won] 1차 펫 기능 개선
				// 퀵슬롯 다음 페이지 버튼 단축키 표시
				ShowExSlotInfo(TRUE, 4);
#endif
				return WMSG_SUCCESS;
			}
			// Rotate button
			else if( m_btnRotate.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Slot
			else if( IsInsideRect( nX, nY, m_rcSlot ) )
			{
				int	nSlotIndex = -1;
				for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
				{
					if( m_abtnItems[m_nCurrentPage][iBtn].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						nSlotIndex = iBtn;
					}
				}

				// Show tool tip
				ShowSlotInfo( TRUE, nSlotIndex );

				return WMSG_SUCCESS;
			}
			// Show cash shop info
			else if( m_btnQuestBook.MouseMessage(pMsg) !=WMSG_FAIL ){
				// Show tool tip
				ShowExSlotInfo( TRUE, 0 );
				return WMSG_SUCCESS;
			}
			else if( m_btnCashShop.MouseMessage(pMsg) !=WMSG_FAIL ){
				// Show tool tip
				ShowExSlotInfo( TRUE, 1 );
				return WMSG_SUCCESS;
			}

			//Show Gift Recv info		:Su-won
			else if( m_btnGiftNotice.MouseMessage(pMsg) !=WMSG_FAIL )
			{
				// Show tool tip
				if( m_bGiftRecv )
					ShowExSlotInfo( TRUE, 2);
				return WMSG_SUCCESS;
			}

			
			// Hide tool tip
			ShowSlotInfo( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			//선물 도착 알림 버튼	:Su-won
			if( m_btnGiftNotice.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar 1
				if( IsInsideRect( nX, nY, m_rcTitle1 ) )
				{
					bTitleBarClick = TRUE;
				}
				/***
				// Title bar 2
				else if( IsInsideRect( nX, nY, m_rcTitle2 ) )
				{
					// Rotate button
					if( m_btnRotate.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					else
					{
						bTitleBarClick = TRUE;
					}
				}
				***/
				// Prev button
				else if( m_btnPrevSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Next button
				else if( m_btnNextSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnQuestBook.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//Nothing
				}
				// wooss 050815
				else if( m_btnCashShop.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//Nothing
				}
				// Slot
				else if( IsInsideRect( nX, nY, m_rcSlot ) )
				{
					m_nSelSlotBtnID = -1;

					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
					{
						if( m_abtnItems[m_nCurrentPage][iBtn].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected button
							m_nSelSlotBtnID = iBtn + m_nCurrentPage * QSLOT_BTN_COUNT;

							bLButtonDownInBtn = TRUE;

							_pUIMgr->RearrangeOrder( UI_QUICKSLOT, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}

				_pUIMgr->RearrangeOrder( UI_QUICKSLOT, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If quick slot isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Prev button
				if( ( wmsgResult = m_btnPrevSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nCurrentPage--;
						if( m_nCurrentPage < 0 )
							m_nCurrentPage = QSLOT_PAGE_COUNT - 1;
						m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
					}

					return WMSG_SUCCESS;
				}
				// Next button
				else if( ( wmsgResult = m_btnNextSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nCurrentPage++;
						if( m_nCurrentPage >= QSLOT_PAGE_COUNT )
							m_nCurrentPage = 0;
						m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
					}

					return WMSG_SUCCESS;
				}
				// Rotate button
				else if( ( wmsgResult = m_btnRotate.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						RotateQuickSlot();
					}

					return WMSG_SUCCESS;
				}
				else if( (wmsgResult = m_btnQuestBook.MouseMessage( pMsg )) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//TODO : NewQuestSystem
						//서버에서 목록을 받아야 하나?
						_pUIMgr->GetQuestBookList()->ToggleVisible();
					}

					return WMSG_SUCCESS;
				}
				// wooss 050815
				// Cash Shop Button 
				else if( (wmsgResult = m_btnCashShop.MouseMessage( pMsg )) != WMSG_FAIL)
				{
					// 테스트서버에는 상점을 열지 않는다
					// MALAYSIA CASH SHOP
					extern UINT g_uiEngineVersion;
//					if( wmsgResult == WMSG_COMMAND && g_uiEngineVersion < 10000 && g_iCountry != MALAYSIA )
					if( wmsgResult == WMSG_COMMAND && g_uiEngineVersion < 10000 )//&& g_iCountry != GERMANY ) 
//						/*g_iCountry != BRAZIL && *//*g_iCountry != HONGKONG*/ )
					{//_pNetwork->BillInfoUserInfoReq();
						if( g_iCountry == TAIWAN )
						{
							if( !_pUIMgr->GetBilling()->IsVisible() )
								_pNetwork->BillInfoUserInfoReq();
						}
						else if( g_iCountry == TAIWAN2 )
						{
							if( !_pUIMgr->GetBillItem()->IsVisible() )							
								_pNetwork->BillInfoUserInfoReq();
						}
						else
						{
							if(_pUIMgr->GetCashShop()->IsEnabled()&&_pUIMgr->GetCashShop()->IsVisible()) _pUIMgr->GetCashShop()->CloseCashShop();
							else{
								_pUIMgr->GetCashShop()->OpenCashShop();
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_BALANCE_REQ);														
							}
						}			
					}

					return WMSG_SUCCESS;
				}

				// 선물 도착 알림 버튼 클릭	:Su-won
				else if( (wmsgResult = m_btnGiftNotice.MouseMessage( pMsg )) != WMSG_FAIL )
				{
					//상점창 열기
					if( m_bGiftRecv )
					{
						_pUIMgr->GetCashShop()->OpenRecvGiftPage();
						_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_BALANCE_REQ);
						
						return WMSG_SUCCESS;
					}
				}

				// Slot
				else if( IsInsideRect( nX, nY, m_rcSlot ) )
				{
					m_nSelSlotBtnID = -1;

					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
					{
						if( ( wmsgResult = m_abtnItems[m_nCurrentPage][iBtn].MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
								UseQuickSlot( iBtn );

							return WMSG_SUCCESS;
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					_pUIMgr->GetMessenger()->SetDragging(false);

					// If holding button comes from quick slot
					if( _pUIMgr->GetHoldBtn().GetWhichUI() == UI_QUICKSLOT )
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if( m_abtnItems[m_nCurrentPage][iBtn].IsInside( nX, nY ) &&
									m_abtnItems[m_nCurrentPage][iBtn].GetBtnID() != _pUIMgr->GetHoldBtn().GetBtnID() )
								{
									int	nPageIndex = m_nSelSlotBtnID / QSLOT_BTN_COUNT;
									int	nSlotIndex = m_nSelSlotBtnID % QSLOT_BTN_COUNT;

									if( nPageIndex == m_nCurrentPage )
										SwapBtnsInQuickSlot( nSlotIndex, iBtn );
									
									break;
								}
							}
						}
					}
					// If holding button is skill and comes from skill window
					// If holding button is action and comes from action window
					// If holding button is item and comes from inventory
					else if( ( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_SKILL &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_CHARACTERINFO ) ||
							( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_SKILL &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_PETINFO ) ||
							 ( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ACTION &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_CHARACTERINFO ) ||							
							 ( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY ) ||
							( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_SKILL &&
								_pUIMgr->GetHoldBtn().GetWhichUI() == UI_WILDPET_INFO ))
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM )
							{
								int			nIndex = _pUIMgr->GetHoldBtn().GetItemIndex();
								CItemData	&ItemData = _pNetwork->GetItemData( nIndex );

								if( (ItemData.GetType() == CItemData::ITEM_ETC 					//Etc(x)
#ifdef Pet_IMPROVEMENT_1ST
										// [070604: Su-won] 1차 펫 기능 개선
										// 생산품을 퀵슬롯에 넣을 수 있게...
										&&ItemData.GetSubType() !=CItemData::ITEM_ETC_PRODUCT
#endif
									) ||
									(ItemData.GetType() == CItemData::ITEM_ACCESSORY				// Accessory(x)
#ifdef Pet_IMPROVEMENT_1ST
										// [070604: Su-won] 1차 펫 기능 개선
										// 펫을 퀵슬롯에 넣을 수 있게...
										&& ItemData.GetSubType() !=CItemData::ACCESSORY_PET
#endif
									) ||
									( ItemData.GetType() == CItemData::ITEM_BULLET &&				// Bullet - arrow(x)
									  ItemData.GetSubType() == CItemData::ITEM_BULLET_ARROW )
									)
									
								{
									// Reset holding button
									_pUIMgr->ResetHoldBtn();

									return WMSG_SUCCESS;
								}
							}

							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if( m_abtnItems[m_nCurrentPage][iBtn].IsInside( nX, nY ) )
								{
									AddBtnToQuickSlot( iBtn );

									break;
								}
							}
						}
					}

					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
	}
	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : UseQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::UseQuickSlot( int nSlotIndex )
{
	if( _pUIMgr->GetBilling()->IsLock() ) return;

	// If slot is empty
	if( m_abtnItems[m_nCurrentPage][nSlotIndex].IsEmpty() )
		return;

	// Use quick slot
	switch( m_abtnItems[m_nCurrentPage][nSlotIndex].GetBtnType() )
	{
	case UBET_SKILL:
		{
			// EDIT : bs : 060322
			BOOL	bDelay = m_abtnItems[m_nCurrentPage][nSlotIndex].GetSkillDelay();
				int		nSkillIndex = m_abtnItems[m_nCurrentPage][nSlotIndex].GetSkillIndex();

			if( !bDelay && _pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_SKILL_ACTIVE )
				{
					_pUIMgr->GetPetInfo()->UseSkill( nSkillIndex );					
				}
			else if( !bDelay &&_pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_COMMAND )
				{
					_pUIMgr->GetPetInfo()->UseCommand( nSkillIndex );
				}
			else if(!bDelay && _pNetwork->GetSkillData(nSkillIndex).GetJob() == WILDPET_JOB)
				{
					_pUIMgr->GetWildPetInfo()->UseSkill(nSkillIndex);
				}
				else
				{
					_pUIMgr->GetCharacterInfo()->UseSkill( nSkillIndex );
				}
//			if( !m_abtnItems[m_nCurrentPage][nSlotIndex].GetSkillDelay() )
//			{
//				int		nSkillIndex = m_abtnItems[m_nCurrentPage][nSlotIndex].GetSkillIndex();
//
//				if( _pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_SKILL_ACTIVE )
//				{
//					_pUIMgr->GetPetInfo()->UseSkill( nSkillIndex );					
//				}
//				else if( _pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_COMMAND )
//				{
//					_pUIMgr->GetPetInfo()->UseCommand( nSkillIndex );
//				}
//				else
//				{
//					_pUIMgr->GetCharacterInfo()->UseSkill( nSkillIndex );
//				}
//			}			
			// EDIT : bs : 060322
		}
		break;

	case UBET_ACTION:
		{
			int	nActionIndex = m_abtnItems[m_nCurrentPage][nSlotIndex].GetActionIndex();
			_pUIMgr->GetCharacterInfo()->UseAction( nActionIndex );
		}
		break;

	case UBET_ITEM:
		{
			if( _pUIMgr->IsCSFlagOn( CSF_WAREHOUSE ) )
				return;

			int	nTab = m_abtnItems[m_nCurrentPage][nSlotIndex].GetItemTab();
			int	nRow = m_abtnItems[m_nCurrentPage][nSlotIndex].GetItemRow();
			int	nCol = m_abtnItems[m_nCurrentPage][nSlotIndex].GetItemCol();

			_pUIMgr->GetInventory()->SetUseItemSlotInfo(nTab, nRow, nCol);
			_pUIMgr->GetInventory()->SendUseSlotItem( nTab, nRow, nCol );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : AddBtnToQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AddBtnToQuickSlot( int nSlotIndex )
{
	if( m_bLock ) return; // 현재 사용하지 않음 

	// Add button
	switch( _pUIMgr->GetHoldBtn().GetBtnType() )
	{
	case UBET_SKILL:
		{
			int	nSkillIndex = _pUIMgr->GetHoldBtn().GetSkillIndex();
			CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );
			if( !(rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) ) )
			{
				SendAddBtn( m_nCurrentPage, nSlotIndex, 0, nSkillIndex );
			}
			else
			{
				CTString strMessage = _S(2339,"소환수만 사용할 수 있는 스킬입니다.");
				_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
			}
		}
		break;

	case UBET_ACTION:
		{
			int	nActionIndex = _pUIMgr->GetHoldBtn().GetActionIndex();
			SendAddBtn( m_nCurrentPage, nSlotIndex, 1, nActionIndex );
		}
		break;

	case UBET_ITEM:
		{
			int	nRow, nCol;
			int	nUniIndex = _pUIMgr->GetHoldBtn().GetItemUniIndex();
			_pUIMgr->GetInventory()->GetLocationOfNormalItem( nUniIndex, nRow, nCol );

			if( nRow != -1 )
				SendAddBtn( m_nCurrentPage, nSlotIndex, 2, nRow, nCol );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SwapBtnsInQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SwapBtnsInQuickSlot( int nSlot1, int nSlot2 )
{
	SendSwapBtns( m_nCurrentPage, nSlot1, nSlot2 );
}

// ----------------------------------------------------------------------------
// Name : RemoveBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemoveBtn( int nBtnID )
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if( m_abtnItems[iPage][iBtn].GetBtnID() == nBtnID )
			{
				// Remove button
				SendAddBtn( iPage, iBtn, -1 );
				return;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangePage()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ChangePage( int nPage )
{
	m_nCurrentPage = nPage;
	m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
}

// ----------------------------------------------------------------------------
// Name : ClearSkills()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::UpdateSkillLevel( int nIndex, SBYTE sbLevel )
{
	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iCol = 0; iCol < QSLOT_BTN_COUNT; iCol++ )
		{
			if( m_abtnItems[iPage][iCol].GetBtnType() == UBET_SKILL &&
				m_abtnItems[iPage][iCol].GetSkillIndex() == nIndex )
			{
				m_abtnItems[iPage][iCol].SetSkillLevel( sbLevel );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : StartSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuickSlot::StartSkillDelay( int nIndex )
{
	BOOL bResult = FALSE;
	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iCol = 0; iCol < QSLOT_BTN_COUNT; iCol++ )
		{
			if( m_abtnItems[iPage][iCol].GetBtnType() == UBET_SKILL &&
				m_abtnItems[iPage][iCol].GetSkillIndex() == nIndex )
			{
				if( m_abtnItems[iPage][iCol].GetSkillDelay() )
				{
					continue;
				}
				else
				{
					m_abtnItems[iPage][iCol].SetSkillDelay( TRUE );
					bResult = TRUE;
				}
			}
			else if( m_abtnItems[iPage][iCol].GetBtnType() == UBET_ITEM )
			{
				int nItemIndex = m_abtnItems[iPage][iCol].GetItemIndex();
				if( nItemIndex == -1 ) continue;

				CItemData ItemData = _pNetwork->GetItemData( nItemIndex );
				if( (ItemData.GetType() == CItemData::ITEM_POTION && ItemData.GetNum0() == nIndex)
					|| (nItemIndex==2407 || nItemIndex==2408 || nItemIndex==2609) || 
					(ItemData.GetType() == CItemData::ITEM_ONCEUSE &&
					ItemData.GetSubType() == CItemData::ITEM_SUB_TARGET) ) //타겟 일회용 아이템, 광속 아이템일 때 쿨타임 표시...
				{
					if( m_abtnItems[iPage][iCol].GetSkillDelay() )
					{
						continue;
					}
					else
					{
						m_abtnItems[iPage][iCol].SetSkillDelay( TRUE );
						bResult = TRUE;
					}
				}
			}

		}
	}

	return bResult;
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendAddBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SendAddBtn( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	_pNetwork->AddQuickSlot( nPage, nSlotNum, nSlotType, nData0, nData1 );
}

// ----------------------------------------------------------------------------
// Name : SendSwapBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SendSwapBtns( int nPage, int nSlotNum1, int nSlotNum2 )
{
	_pNetwork->SwapQuickSlot( nPage, nSlotNum1, nSlotNum2 );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : AddBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AddBtn( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	// Empty slot
	if( nSlotType == -1 )
	{
		m_abtnItems[nPage][nSlotNum].InitBtn();
	}
	else
	{
		switch( nSlotType )
		{
		case UBET_SKILL:
			{
				// 펫 액티브 스킬의 경우.
				if( _pNetwork->GetSkillData( nData0 ).GetType() == CSkill::ST_PET_SKILL_ACTIVE)
				{
					SBYTE	sbSkillLevel = _pUIMgr->GetPetInfo()->GetSkillLevel( _pNetwork->_PetTargetInfo.lIndex, nData0 );
					m_abtnItems[nPage][nSlotNum].SetSkillInfo( nData0, sbSkillLevel, FALSE );
				}
				else if(_pNetwork->GetSkillData( nData0 ).GetJob() == WILDPET_JOB)
				{
					if(_pUIMgr->GetWildPetInfo()->IsLearnSkill(nData0))
					{
						SBYTE	sbSkillLevel = _pUIMgr->GetWildPetInfo()->GetSkillLevel(nData0);
						m_abtnItems[nPage][nSlotNum].SetSkillInfo( nData0, sbSkillLevel, FALSE );
					}else
					{
						SendAddBtn( nPage, nSlotNum, -1 );	
					}
				}
				else
				{
					SBYTE	sbSkillLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel( nData0, FALSE );
					m_abtnItems[nPage][nSlotNum].SetSkillInfo( nData0, sbSkillLevel, FALSE );
				}
			}
			break;
			
		case UBET_ACTION:
			{
				m_abtnItems[nPage][nSlotNum].SetActionInfo( nData0 );
			}
			break;
			
		case UBET_ITEM:
			{
				int	nIndex = _pNetwork->MySlotItem[0][nData0][nData1].Item_Index;
				int	nUniIndex = _pNetwork->MySlotItem[0][nData0][nData1].Item_UniIndex;
				int	nWearingType = _pNetwork->MySlotItem[0][nData0][nData1].Item_Wearing;
				m_abtnItems[nPage][nSlotNum].SetItemInfo( 0, nData0, nData1, nIndex, nUniIndex, nWearingType );
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SwapBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SwapBtns( int nPage, int nSlotNum1, int nSlotNum2 )
{
	static CUIButtonEx	btnTemp;
	btnTemp.Copy( m_abtnItems[nPage][nSlotNum1] );
	m_abtnItems[nPage][nSlotNum1].Copy( m_abtnItems[nPage][nSlotNum2] );
	m_abtnItems[nPage][nSlotNum2].Copy( btnTemp );
}

// ----------------------------------------------------------------------------
// Name : RefreshBtn()
// Desc : in case of item, nIndex is nUniIndex...
// ----------------------------------------------------------------------------
void CUIQuickSlot::RefreshBtn( int nType, int nIndex )
{
	switch( nType )
	{
	case UBET_SKILL:
		{
		}
		break;
		
	case UBET_ACTION:
		{
		}
		break;
		
	case UBET_ITEM:
		{
			int	nRow, nCol;
			_pUIMgr->GetInventory()->GetLocationOfNormalItem( nIndex, nRow, nCol );
			
			for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
			{
				for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
				{
					if( m_abtnItems[iPage][iBtn].GetItemUniIndex() == nIndex )
					{
						m_abtnItems[iPage][iBtn].SetItemRow( nRow );
						m_abtnItems[iPage][iBtn].SetItemCol( nCol );
					}
				}
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ClearAllBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ClearAllBtns( int nPage)
{
	for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		m_abtnItems[nPage][iBtn].InitBtn();
}

//선물 버튼 위치 설정	:Su-won
void CUIQuickSlot::SetGiftNoticePos()
{
	int Width =_pUIMgr->GetDrawPort()->GetWidth();
	int Height=_pUIMgr->GetDrawPort()->GetHeight();

	int nPosX, nPosY;
	int nAbsPosX, nAbsPosY;
				
	m_btnGiftNotice.GetPos(nPosX, nPosY);
	m_btnGiftNotice.GetAbsPos(nAbsPosX, nAbsPosY);
			
	if( m_bHorizontal )
	{
		/***
		if( nAbsPosY<0)
		{
			m_btnGiftNotice.SetPos(nPosX, m_btnCashShop.GetPosY()+32);
			m_eGiftPos =GIFTPOS_BOTTOM;
		}
		if( nAbsPosY+QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW>Height)
		{
			m_btnGiftNotice.SetPos(nPosX, m_btnCashShop.GetPosY()-(QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW) );
			m_eGiftPos =GIFTPOS_TOP;
		}
		***/
		m_btnGiftNotice.SetPos(nPosX, m_btnCashShop.GetPosY()-32);
	}
	else
	{
		if( nAbsPosX<0)
		{
			m_btnGiftNotice.SetPos(m_btnCashShop.GetPosX()+32, nPosY);
			m_eGiftPos =GIFTPOS_RIGHT;
		}
		if( nAbsPosX+QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW>Width)
		{
			m_btnGiftNotice.SetPos(m_btnCashShop.GetPosX()-(QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW), nPosY);
			m_eGiftPos =GIFTPOS_LEFT;
		}
	}
	
	m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[m_eGiftPos][0] );
	m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[GIFTPOS_TOP][0] );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
}

//선물 도착 알림 버튼 렌더링		:Su-won
void CUIQuickSlot::RenderGiftButton()
{
	if( m_bGiftRecv )
	{
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdGiftTexture );

		// Timer for highlight buttons
		static BOOL		bHighlight = FALSE;
		static DOUBLE	dElapsedTime = 0.0;
		static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		dElapsedTime += dCurTime - dOldTime;
		dOldTime = dCurTime;
		if( dElapsedTime > 0.5 )
		{
			bHighlight = !bHighlight;
			m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[m_eGiftPos][bHighlight] );
			m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[m_eGiftPos][bHighlight] );
			m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
			m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
			do
			{
				dElapsedTime -= 0.5;
			}
			while( dElapsedTime > 0.5 );
		}

		m_btnGiftNotice.Render();

		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}	
}