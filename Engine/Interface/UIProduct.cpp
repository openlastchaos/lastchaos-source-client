#include "stdh.h"

// 헤더 정리 [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/ProductNPCUI.h>

//SealType의 서브타입개수를 누적함
int SealTypeCount[MAX_SEALTYPE+1] = 
{
	0,	//3,				//정교한 제작 : 투구, 상의, 장갑
	3,	//4,				//화려한 제작 : 하의, 신발, 방패, 날개
	7,  //7,				//날카로운 제작 : 석궁, 스태프, 도끼, 이도류, 완드, 폴암, 혼
	14,	//6,				//단단한 제작 : 기사도, 대검, 숏 스태프, 활, 단검, 사이드
	20,	//3,				//소모품 제작 : 일회용, 포션, 기타
	23,	//End
};

int SealSubType[MAX_SEALSUBTYPE] = 
						{ 0, 1, 3,						//정교한 제작 : 투구, 상의, 장갑
						 2, 4, 5, 6,					//화려한 제작 : 하의, 신발, 방패, 날개
						 1, 2, 4, 11, 12, 14, 15,		//날카로운 제작 : 석궁, 스태프, 도끼, 이도류, 완드, 폴암, 혼
						 0, 3, 5, 6, 7, 13, 			//단단한 제작 : 기사도, 대검, 숏 스태프, 활, 단검, 사이드
						 2, 6, 4,						//소모품 제작 : 일회용, 포션, 기타
						};

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProduct()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProduct::CUIProduct()
	: m_pTxtSubject(NULL)
	, m_pListItems(NULL)
	, m_pListDesc(NULL)
	, m_pBtnOK(NULL)
{
	setInherit(false);
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct::~CUIProduct()
{
	Clear();
	Destroy();	
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Clear()
{
	m_nSelectProductItem	= -1;
	m_bWaitProductResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_nProductText			= -1;
	m_nTab					= -1;
	m_nInvenIdx				= -1;
	
	m_nNeedItemCount		= 0;

	memset(m_NeedItems, 0, sizeof(CNeedItems) * MAX_MAKE_ITEM_MATERIAL);

	m_bWaitProductResult = FALSE;

	if (m_pListItems != NULL)
		m_pListItems->DeleteAllListItem();

	if (m_pListDesc != NULL)
		m_pListDesc->DeleteAllListItem();

	clearItems();
}

// ----------------------------------------------------------------------------
// Name : InitProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::InitProduct()
{
	clearItems();

	CItemData* pTextItemData = _pNetwork->GetItemData ( m_nProductText );
	const char* szItemName = _pNetwork->GetItemName( m_nProductText );
	
	// 임시 처리 
	int nProductType = pTextItemData->GetProcessType();
	int nProductSubType = pTextItemData->GetProcessSubType();

	if (m_pTxtSubject != NULL)
		m_pTxtSubject->SetText(CTString((char*)szItemName));

	m_pListItems->DeleteAllListItem();
	CUIListItem* pTemp = m_pListItems->GetListItemTemplate();
	CUIListItem* pClone = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;

	int SubTypeBit;
	int nCnt = 0;
	CTString tmpStr, strCount;

	// 제조 문서로 제조 할수 있는 아이템 검색 
	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemData* pItemData = (*iter).second;

		if (pItemData == NULL)
			continue;

		if ( pItemData->GetFlag() & ITEM_FLAG_MADE )
		{
			SubTypeBit = 0x00000001 << pItemData->GetSubType();		

			if ( ( pItemData->GetType () == nProductType ) && ( SubTypeBit & nProductSubType ) )
			{
				// Date : 2005-01-07,   By Lee Ki-hwan 
				// 포션인 경우에는 등급을 체크 하고, 다른 아이템인 경우에는 레벨을 체크 한다. 
				if ( pItemData->GetType () == CItemData::ITEM_POTION )
				{
					if ( pItemData->GetGrade() != pTextItemData->GetGrade() ) continue;
				}
				else
				{
					if ( pItemData->GetLevel() != pTextItemData->GetLevel() ) continue;
				}
				
				{
					nCnt = pTextItemData->GetMakePotionCount();

					CItems* pItems = new CItems(pItemData->GetItemIndex());
					m_vecItems.push_back(pItems);

					pClone = (CUIListItem*)pTemp->Clone();
					m_pListItems->AddListItem(pClone);

					pIcon = (CUIIcon*)pClone->findUI("icon");

					if (pIcon != NULL)
					{
						pIcon->setData(pItems);
					}

					pText = (CUIText*)pClone->findUI("text");

					if (pText != NULL)
					{
						pText->SetText(CTString(pItemData->GetName()));

						if (nCnt > 0)
						{
							pText->SetPosY(0);							
						}
					}

					if (nCnt > 0)
					{
						pText = (CUIText*)pClone->findUI("txt_count");

						if (pText != NULL)
						{
							strCount = UIMGR()->IntegerToCommaString(nCnt);
							tmpStr.PrintF(_S(766, "%s 개"), strCount);
							pText->SetText(tmpStr);
						}
					}
				}
			}
		}
	}

	m_pListItems->UpdateScroll(m_vecItems.size());
	m_pListItems->UpdateList();
		 
	AddString ( _S( 572, "아이템을 선택해 주십시오." ), COLOR_TEXT );

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx )
{
	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear();

	if( pUIManager->GetInventory()->IsVisible() == FALSE )
		pUIManager->GetInventory()->ToggleVisible();

	m_nProductText	= nItemIndex;
	m_nTab			= nTab;
	m_nInvenIdx		= inven_idx;

	pUIManager->RearrangeOrder( UI_PRODUCT, TRUE );
	
	InitProduct ();

	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_PRODUCT );

	Hide(FALSE);
}

// ----------------------------------------------------------------------------
// Name : CloseProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::CloseProduct()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close refine
	pUIManager->RearrangeOrder( UI_PRODUCT, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PRODUCT );

	Clear();

	Hide(TRUE);	
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :!!
// ----------------------------------------------------------------------------
void CUIProduct::SelectItem( int _nIndex )
{
 
	/*  cpp2angel
		제조창을 띄운 상태에서 아이템을 습득 하였을 때 SelectItem()을 호출 하여서 
		현재 선택된 아이템으로 필요 조건을 다시 습득 하기 위한 루틴
	*/
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProductItem;	// 현재 선택된 아이템으로 셋팅

		if (_nIndex == -1)   // 그래도 -1이면 Return...
			return;	
		
	}
	
	BOOL bNeedItem[MAX_MAKE_ITEM_MATERIAL];
	BOOL bNeedItemCount[MAX_MAKE_ITEM_MATERIAL];
	
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;

	/*
	BOOL bNeedSSkill2		= FALSE;
	BOOL bNeedSSkillLevel2	= FALSE;
	*/

	// 데이터 초기화
	memset ( bNeedItem, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );
	memset ( bNeedItemCount, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );

	m_nNeedItemCount = 0;
	m_pListDesc->DeleteAllListItem();
	
	// 생산 하고자 하는 아이템 모록
	if ( m_vecItems[_nIndex]->Item_Index == -1 ) return;
	CItemData* pProductItemData	= _pNetwork->GetItemData ( m_vecItems[_nIndex]->Item_Index );
	CItemData* pTextItemData	= _pNetwork->GetItemData ( m_nProductText );
	
	if ( pProductItemData->GetNeedSSkillIndex () == -1 ) return;
	CSpecialSkill* pNeedSSkill	= CSpecialSkill::getData( pProductItemData->GetNeedSSkillIndex () );

	if (pNeedSSkill == NULL)
		return;

	if ( pProductItemData->GetNeedSSkillCount () == -1 ) return;
	int			nSSkillLevel	= pProductItemData->GetNeedSSkillCount ();

	// 필요 스킬 ( Preference 채크 바람 )
	bNeedSSkill =  CUIManager::getSingleton()->GetCharacterInfo()->CheckSSkillByIndex ( pNeedSSkill->GetIndex(), nSSkillLevel, &bNeedSSkillLevel );

	// 필요 아이템
	int nIndexTemp = 0;
	CItemData* pInvenItem = NULL;

	int		nIndex;
	// 필요 아이템 종류와 갯수 얻기 
	for ( nIndex = 0; nIndex < MAX_MAKE_ITEM_MATERIAL; nIndex++ )
	{	
		nIndexTemp = pProductItemData->GetNeedItemIndex( nIndex );

		if( nIndexTemp == -1 )	continue;
		
		m_NeedItems[m_nNeedItemCount].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
		m_NeedItems[m_nNeedItemCount].llCount	= pProductItemData->GetNeedItemCount( nIndex ); 
	
		m_nNeedItemCount++;	
	}

	if ( m_nNeedItemCount == 0 ) return;

	CUIInventory* pInven = CUIManager::getSingleton()->GetInventory();
	int		dbIndex = 0;

	// 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		SWORD		nTab, nIdx;
		dbIndex = m_NeedItems[nIndex].ItemData->GetItemIndex();
		if (ItemHelp::HaveItem( dbIndex, &nTab, &nIdx ) == true)
		{
			bNeedItem[nIndex] = TRUE;

			if (pInven->GetItemCount(dbIndex) >= m_NeedItems[nIndex].llCount)
			{
				bNeedItemCount[nIndex] = TRUE;

				m_NeedItems[nIndex].MatTab = nTab;
				m_NeedItems[nIndex].inven_idx = nIdx;
			}
		}
	}

// 조건 출력 
	CTString strTitle;
	m_bSatisfied = TRUE;
	
	// 필요스킬 -------------------------------------------------------------------------------	
	AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE );		
	AddString ( CTString ( pNeedSSkill->GetName() ), bNeedSSkill? COLOR_TEXT : COLOR_NONE );

	strTitle.PrintF ( _S( 573, "%s : %d " ), _S( 574, "레벨" ), nSSkillLevel ); 
	AddString ( strTitle, bNeedSSkillLevel? COLOR_TEXT : COLOR_NONE );


	{
		m_bSatisfied &= bNeedSSkill;		
	}


	// 필요 물품 -------------------------------------------------------------------------------
	AddString ( CTString ( "" ) );		
	AddString ( _S( 575, "필요 물품" ), COLOR_SUB_TITLE );		
		
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		m_bSatisfied &= bNeedItem[nIndex] && bNeedItemCount[nIndex];
	}
	
	
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		const char* szItemName;
		if (m_NeedItems[nIndex].ItemData != NULL)
			szItemName = _pNetwork->GetItemName( m_NeedItems[nIndex].ItemData->GetItemIndex() );

		CTString strCount = UIMGR()->IntegerToCommaString(m_NeedItems[nIndex].llCount);
		strTitle.PrintF ( _S( 576, "%s : %s개" ), szItemName, strCount );
		AddString ( strTitle, bNeedItemCount[nIndex]? COLOR_TEXT : COLOR_NONE );
	}

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProduct::AddString ( CTString& strText, COLOR colText )
{
	int nLength = UTIL_HELP()->GetFontWidth(strText.str_String);

	// 줄이 넘어가는 경우
	if (nLength >= _iMaxMsgStringChar)
	{
		CUITextBox	_tb;
		_tb.SetSplitMode(SPLIT_SPACE);
		_tb.SetWidth(_iMaxMsgStringChar);
		_tb.SetText(strText);

		int max = _tb.GetLineCount();

		for (int i = 0; i < max; ++i)
		{
			pushback_string(_tb.GetLineString(i), colText);
		}
	}
	else 
	{
		pushback_string(strText, colText);
	}

	if (m_pListDesc != NULL)
	{
		m_pListDesc->UpdateScroll(m_pListDesc->getListItemCount());
		m_pListDesc->UpdateList();
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProductReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::SendProductReq()
{
	if( m_bWaitProductResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 943, "스킬 사용중에는 제작을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 944, "순간 이동중에는 제작을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	_pNetwork->ProductReq ( m_nTab, m_nInvenIdx, m_vecItems[m_nSelectProductItem]->Item_Index, m_nNeedItemCount, 
						m_NeedItems );

	m_bWaitProductResult = TRUE;
	
	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(!m_bWaitProductResult);
	//SetEnable ( !m_bWaitProductResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProductRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ProductRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_PRODUCT_REP );

	// Show result
	CTString	strMessage;

	switch( sbResult )
	{
	case MSG_MAKE_FAIL_SYSTEM:				// 있어서는 안되는 이유로 실패
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_FAIL_MATERIAL:			// 재료 정보 오류
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_FAIL_NO_SSKILL:			// 제작 스킬 없음
		strMessage = _S( 578, "제작에 필요한 제작 스킬이 없습니다." );							
		break;
	case MSG_MAKE_FAIL_ITEM_LEVEL :			// 현재 스킬로 제작 불가능 아이템 (레벨)
		strMessage = _S( 579, "제작에 필요한 스킬 레벨이 부족합니다." );						
		break;
	case MSG_MAKE_FAIL_PROB:				// 확률 실패
		strMessage = _S( 577, "제작에 실패 하였습니다." );										
		break;
	case MSG_MAKE_SUCCESS:
		strMessage = _S( 580, "제작에 성공 하였습니다." );										
		break;
	}

	CloseProduct ();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 571, "제작" ), UMBS_OK, UI_PRODUCT, MSGCMD_PRODUCT_REP );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	
	m_bWaitProductResult = FALSE;	
}

void CUIProduct::initialize()
{
	m_bSatisfied = FALSE;

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

	m_pTxtSubject = (CUIText*)findUI("txt_subject");

	m_pListItems = (CUIList*)findUI("list_process");

	m_pListDesc = (CUIList*)findUI("list_desc");

	if (m_pListDesc != NULL)
	{
		CUIListItem* pItem = m_pListDesc->GetListItemTemplate();

		if (pItem != NULL)
			_iMaxMsgStringChar = pItem->GetWidth();
	}

	m_pBtnOK = (CUIButton*)findUI("btn_ok");	

	CUIButton* pBtn;

#ifndef		WORLD_EDITOR

	if (m_pListItems != NULL)
		m_pListItems->SetCommandFSelect(boost::bind(&CUIProduct::callback_select, this));

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetCommandFUp(boost::bind(&CUIProduct::SendProductReq, this));

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProduct::CloseProduct, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProduct::CloseProduct, this));

#endif		// WORLD_EDITOR
}

WMSG_RESULT CUIProduct::OnLButtonDown( UINT16 x, UINT16 y )
{
	CUIManager::getSingleton()->RearrangeOrder(UI_PRODUCT, TRUE);

	return CUIWindow::OnLButtonDown(x, y);
}

//-------------------------------------------------------------------
// PRIVATE

void CUIProduct::clearItems()
{
	int		i, nMax = m_vecItems.size();

	for (i = 0; i < nMax; ++i)
	{
		SAFE_DELETE(m_vecItems[i]);
	}

	m_vecItems.clear ();
}

void CUIProduct::pushback_string( CTString& strText, COLOR color )
{
	if (m_pListDesc == NULL)
		return;

	CUIListItem* pclone = NULL;
	CUIListItem* pTemp = m_pListDesc->GetListItemTemplate();
	CUIText* pText = NULL;

	if (pTemp == NULL)
		return;

	pclone = (CUIListItem*)pTemp->Clone();
	m_pListDesc->AddListItem(pclone);

	pText = (CUIText*)pclone->findUI("txt_line");

	if (pText != NULL)
	{
		pText->SetText(strText);
		pText->setFontColor(color);		
	}
}

void CUIProduct::callback_select()
{
	if (m_pListItems == NULL)
		return;

	int sel = m_pListItems->getCurSel();

	m_nSelectProductItem = sel;
	SelectItem(sel);
}
