#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProduct.h>

enum eSelProcessNPC
{
	SEL_MINERAL = 0,	// 광석 정련
	SEL_CROP,			// 식물 가공
	SEL_ENERGY,			// 원소 정제
};

enum eSelMineral
{
	SEL_MINERAL_1 = 0,	// 스톤 온
	SEL_MINERAL_2,		// 스톤 리스
	SEL_MINERAL_3,		// 스톤 웨버
	SEL_MINERAL_4,		// 스톤 비스트
	SEL_MINERAL_5,		// 스톤 웰스던
};

enum eSelEnergy
{
	SEL_ENERGY_1 = 0,	// E
	SEL_ENERGY_2,		// D
	SEL_ENERGY_3,		// C
	SEL_ENERGY_4,		// B
	SEL_ENERGY_5,		// A
};

enum eSelCrop
{
	SEL_CROP_1 = 0,		// 노란잎
	SEL_CROP_2,			// 줄기
	SEL_CROP_3,			// 파란잎
	SEL_CROP_4,			// 가시
	SEL_CROP_5,			// 꽃
};

// ----------------------------------------------------------------------------
// Name : CUIProcessNPC()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcessNPC::CUIProcessNPC()
	: m_pListItems(NULL)
	, m_pListDesc(NULL)
	, m_pBtnOK(NULL)
	, m_nNeedTextWidth(100)
{
	setInherit(false);
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcessNPC()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcessNPC::~CUIProcessNPC()
{
	clearVecBtnEX();
	Destroy();	
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Clear()
{
    m_iResourceType			= -1;
    m_iResourceSubType		= -1;
    m_nProcessItemCount		= 1;

    m_nSelectProcessNPCItem	= -1;
    m_bWaitProcessNPCResult	= TRUE;
    m_bSatisfied			= FALSE;

    m_nNeedItemCount		= 0;

    memset(m_NeedItems, 0, sizeof(CNeedItems) * MAX_MAKE_ITEM_MATERIAL);

    m_bWaitProcessNPCResult = FALSE;
    
	if (m_pListItems != NULL)
		m_pListItems->DeleteAllListItem();
	
	if (m_pListDesc != NULL)
		m_pListDesc->DeleteAllListItem();

    clearVecBtnEX();
}

// ----------------------------------------------------------------------------
// Name : InitProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::InitProcessNPC()
{
    clearVecBtnEX();

    // 임시 처리
    int nProcessType = m_iResourceType;
    int nProcessSubType = m_iResourceSubType;

	CUIListItem* pTemp = m_pListItems->GetListItemTemplate();
	CUIListItem* pClone = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	CUISpinControl* pSpin = NULL;

    // 제조  Type 설정
    int bit = 0x00000001;

    //!! 제조 문서로 제조 할수 있는 아이템 검색
    CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
    CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

    for (; iter != eiter; ++iter)
    {
        CItemData* pItemData = (*iter).second;

        if (pItemData == NULL)
            continue;

        if (pItemData->GetFlag() & ITEM_FLAG_MADE)
        {
            if (pItemData->GetType() == CItemData::ITEM_ETC &&
                pItemData->GetSubType() == CItemData::ITEM_ETC_PROCESS &&  // 가공품 이라는 얘기
                pItemData->GetProcessType() == nProcessType
                && pItemData->GetProcessSubType() == nProcessSubType
               )
            {
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
					pText->SetText(CTString(pItemData->GetName()));

				pSpin = (CUISpinControl*)pClone->findUI("spin_cnt");

				if (pSpin != NULL)
				{
					pSpin->Hide(TRUE);

#ifndef		WORLD_EDITOR
					pSpin->SetCommandFMove(boost::bind(&CUIProcessNPC::callback_spin, this));
					pSpin->SetCommandFReturn(boost::bind(&CUIProcessNPC::callback_spin, this));
#endif		// WORLD_EDITOR
				}
            }
        }
    }

    AddString(_S(561, "가공품을 선택해 주십시오."), COLOR_TEXT);

	m_pListItems->UpdateScroll(m_vecItems.size());
	m_pListItems->UpdateList();

    m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessNPC()
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_REQ);

    pUIManager->CreateMessageBoxL(_S(1786, "가공 NPC") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_REQ);

    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1787, "무엇을 가공하러 왔는가?"), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1788, "어떤 생산품이든 맡겨만 주면 내가 완벽하게 가공해 주겠네. 물론 나도 먹고 살아야 하니 무료로는 힘들어."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1789, "수수료를 주는 것이 싫다면 어쩔 수 없지만 이 근처에서 나만큼 뛰어난 가공기술을 가지고 있는 사람은 보질 못했어.  잘 생각해 보라구."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1790, "광석 정련"), SEL_MINERAL);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1791, "식물 가공"), SEL_CROP);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1792, "원소 정제"), SEL_ENERGY);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1220, "취소한다."));
}

// ----------------------------------------------------------------------------
// Name : OpenProcessList()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessList(int iType, int iSubType)
{
    // If this is already exist
    if (IsVisible() == TRUE)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    // If inventory is already locked
    if (pUIManager->GetInventory()->IsLocked())
    {
        pUIManager->GetInventory()->ShowLockErrorMessage();
        return;
    }

    Clear();

    if (!pUIManager->GetInventory()->IsVisible())
        pUIManager->GetInventory()->ToggleVisible();

    //m_nProcessNPCText	= nItemIndex;
    m_iResourceType = iType;
    m_iResourceSubType = iSubType;

	Hide(FALSE);
    pUIManager->RearrangeOrder(UI_PROCESSNPC, TRUE);

    InitProcessNPC();
}

// ----------------------------------------------------------------------------
// Name : CloseProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::CloseProcessNPC()
{
    // Close refine
	Hide(TRUE);
    CUIManager::getSingleton()->RearrangeOrder(UI_PROCESSNPC, FALSE);

    Clear();
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SelectItem(int _nIndex)
{
    if (_nIndex == -1)
    {
        _nIndex = m_nSelectProcessNPCItem;

        if (_nIndex == -1) return;
    }

    if (_nIndex >= m_vecItems.size())
        return;

    BOOL bNeedItem			= FALSE;
    BOOL bNeedItemCount		= FALSE;

	if (m_pListDesc != NULL)
		m_pListDesc->DeleteAllListItem();

	// SpinControl 보이기
	show_spinctrl(m_nSelectProcessNPCItem, FALSE);

    // 생산 하고자 하는 아이템 모록
    if (m_vecItems[_nIndex]->Item_Index == -1) return;

    CItemData*		pProcessNPCItemData	= _pNetwork-> GetItemData(m_vecItems[_nIndex]->Item_Index);

    // 필요 아이템
    int nIndexTemp = 0;

    // 필요 아이템 종류와 갯수 얻기
    nIndexTemp = pProcessNPCItemData->GetNeedItemIndex(0);

    if (nIndexTemp == -1) return;

    m_NeedItems[0].ItemData	= _pNetwork->GetItemData(nIndexTemp);
    m_NeedItems[0].llCount	= pProcessNPCItemData->GetNeedItemCount(0);
    __int64 myMoney = 0;
    myMoney = _pNetwork->MyCharacterInfo.money;
    // 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )

    SWORD nTab, nIndex;

    if (ItemHelp::HaveItem(m_NeedItems[0].ItemData->GetItemIndex(), &nTab, &nIndex, m_NeedItems[0].llCount))
    {
        bNeedItem = TRUE;
        bNeedItemCount = TRUE;

        m_NeedItems[0].MatTab = nTab;
        m_NeedItems[0].inven_idx = nIndex;
    }

    // 조건 출력
    CTString strTitle;
    CTString strSpace = "";

    // 필요 스킬 출력
    //AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE );
    //AddString ( strSpace + NeedSSkill.GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
    //m_bSatisfied = bNeedSSkill;

    //AddString ( CTString ( " " ) );

    // 필요 물품
    switch (m_iResourceType)
    {
    case CItemData::PROCESS_DOC_STONE :
        strTitle = _S(563, "필요 스톤");
        break;

    case CItemData::PROCESS_DOC_PLANT :
        strTitle = _S(564, "필요 크락");
        break;

    case CItemData::PROCESS_DOC_ELEMENT :
        strTitle = _S(565, "필요 원소");
        break;
    }

    AddString(strTitle, COLOR_SUB_TITLE);


    bNeedItem = bNeedItem && bNeedItemCount;
    const char* szItemName = NULL;

    if (m_NeedItems[0].ItemData != NULL)
    {
        szItemName = _pNetwork->GetItemName(m_NeedItems[0].ItemData->GetItemIndex());
		CTString strCount = UIMGR()->IntegerToCommaString(m_nProcessItemCount * m_NeedItems[0].llCount);
        strTitle.PrintF(_S(576, "%s : %s개 "), strSpace + szItemName, strCount);
    }

    AddString(strTitle, bNeedItem ? COLOR_TEXT : COLOR_NONE);
    m_bSatisfied = bNeedItem;

    AddString(CTString(""), COLOR_TEXT);
    AddString(_S(1793, "수수료"), COLOR_SUB_TITLE);
    CTString strMoney;
    static int s_iMoney[3][5] =
    {
        {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
    };
    __int64 needMoney = s_iMoney[m_iResourceType][m_iResourceSubType] * m_nProcessItemCount * m_NeedItems[0].llCount;

	CTString strNeedNas;
	strNeedNas.PrintF("%I64d", needMoney);
	UIMGR()->InsertCommaToString(strNeedNas);

    strMoney.PrintF("%s %s", strNeedNas, _S(1762, "나스"));
    AddString(strMoney, needMoney <= myMoney ? COLOR_TEXT : COLOR_NONE);

    m_bSatisfied = m_bSatisfied && needMoney <= myMoney;
    m_pBtnOK->SetEnable(m_bSatisfied);
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AddString(CTString& strText, COLOR colText)
{
	int nLength = UTIL_HELP()->GetFontWidth(strText.str_String);

	// 줄이 넘어가는 경우
	if (nLength >= m_nNeedTextWidth)
	{
		CUITextBox	_tb;
		_tb.SetPos(0, 0);
		if (m_pListDesc != NULL)
			_tb.SetSize(m_pListDesc->GetWidth(), m_pListDesc->GetHeight());

		_tb.SetSplitMode(SPLIT_SPACE);
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
		m_pListDesc->UpdateList();
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessNPCReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SendProcessNPCReq()
{
    if (m_bWaitProcessNPCResult == TRUE)
        return;

    if (m_nSelectProcessNPCItem >= m_vecItems.size())
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    if (pUIManager->IsCSFlagOn(CSF_TELEPORT))
    {
        pUIManager->GetChattingUI()->AddSysMessage(_S(942, "순간 이동중에는 가공을 할 수 없습니다.") , SYSMSG_ERROR);
        return;
    }

    _pNetwork->SendNPCProcessMessage(m_vecItems[m_nSelectProcessNPCItem]->Item_Index, m_nProcessItemCount);

    m_bWaitProcessNPCResult = TRUE;

    m_pBtnOK->SetEnable(!m_bWaitProcessNPCResult);
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessNPCRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ProcessNPCRep(LONG processItemDBIndex, LONG count, LONG result)
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    m_bWaitProcessNPCResult = FALSE;

    // Close message box
    pUIManager->CloseMessageBox(MSGCMD_PROCESS_REP);

    // Show result
    CTString	strMessage;
    bool bRetProcessNPCList = false;
    CTString strTitle;
    CTString strContent;

    int commandCode = -1;

    strTitle = _S(1794, "가공 실패");

    switch (result)
    {
    case MSG_ITEM_PROCESS_NPC_ERROR_OK:		// 성공
        {
            strTitle = _S(1795, "가공 성공");
            strContent = _S(1796, "물품 가공에 성공하였습니다. 수수료를 지불합니다.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_FAIL:	// 실패
        {
            strContent = _S(1797, "물품 가공에 실패하였습니다. 수수료는 지불하지 않습니다.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_COUNT:	// 카운트 오류
        {
            strContent = _S(1798, "가공할 재료의 개수를 입력하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE:	// 재료 부족
        {
            strContent = _S(1799, "재료가 부족합니다. 확인하고 다시 시도하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_MONEY:	// 나스 부족
        {
            strContent = _S(1800, "수수료가 부족합니다. 확인하고 다시 시도하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;
    }

    m_iLastResourceType = m_iResourceType;
    m_iLastResourceSubType = m_iResourceSubType;
    CloseProcessNPC();

    pUIManager->CloseMessageBox(MSGCMD_PROCESS_NOTIFY);
    CUIMsgBox_Info	MsgBoxInfo;
    MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_OK, UI_PROCESSNPC, commandCode);
    MsgBoxInfo.AddString(strContent);
    pUIManager->CreateMessageBox(MsgBoxInfo);

    m_bWaitProcessNPCResult = FALSE;
}

void CUIProcessNPC::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput)
{
    switch (nCommandCode)
    {
    case MSGCMD_PROCESS_NOTIFY:
        {
        } break;

    case MSGCMD_PROCESS_REP:
        {
            OpenProcessList(m_iLastResourceType, m_iLastResourceSubType);
        }
        break;
    }
}

void CUIProcessNPC::MsgBoxLCommand(int nCommandCode, int nResult)
{
    switch (nCommandCode)
    {
    case MSGLCMD_PROCESSNPC_REQ:
        {
            CUIManager* pUIManager = CUIManager::getSingleton();

            switch (nResult)
            {
            case SEL_MINERAL:		// 광석 정련
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->CreateMessageBoxL(_S(1790, "광석 정련") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1802, "자 이제 어떤 스톤을 정련할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1803, "아 나라고 무조건 정련에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1804, "스톤 온 정련"), SEL_MINERAL_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1805, "스톤 리스 정련"), SEL_MINERAL_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1806, "스톤 웨버 정련"), SEL_MINERAL_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1807, "스톤 비스트 정련" ), SEL_MINERAL_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1808, "스톤 웰스던 정련" ), SEL_MINERAL_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;

            case SEL_ENERGY:		// 원소 정제
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->CreateMessageBoxL(_S(1792, "원소 정제") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1809, "자 이제 어떤 원소를 정제할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1810, "아 나라고 무조건 정제에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1811, "E등급 원소 정제"), SEL_ENERGY_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1812, "D등급 원소 정제"), SEL_ENERGY_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1813, "C등급 원소 정제"), SEL_ENERGY_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1814, "B등급 원소 정제" ), SEL_ENERGY_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1815, "A등급 원소 정제" ), SEL_ENERGY_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;

            case SEL_CROP:			// 식물 가공.
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->CreateMessageBoxL(_S(1791, "식물 가공") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1816, "자 이제 크락의 어떤 부분을 가공할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1817, "아 나라고 무조건 가공에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1818, "크락의 노란잎 가공"), SEL_CROP_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1819, "크락의 줄기 가공"), SEL_CROP_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1820, "크락의 파란잎 가공"), SEL_CROP_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1821, "크락의 가시 가공" ), SEL_CROP_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1822, "크락의 꽃 가공" ), SEL_CROP_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_MINERAL_REQ:
        {
            switch (nResult)
            {
            case SEL_MINERAL_1:		// 광석 정련
            case SEL_MINERAL_2:		// 광석 정련
            case SEL_MINERAL_3:		// 광석 정련
            case SEL_MINERAL_4:		// 광석 정련
            case SEL_MINERAL_5:		// 광석 정련
                OpenProcessList(SEL_MINERAL, nResult);
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_ENERGY_REQ:
        {
            switch (nResult)
            {
            case SEL_ENERGY_1:		// 원소 정제
            case SEL_ENERGY_2:		// 원소 정제
            case SEL_ENERGY_3:		// 원소 정제
            case SEL_ENERGY_4:		// 원소 정제
            case SEL_ENERGY_5:		// 원소 정제
                {
                    OpenProcessList(SEL_ENERGY, nResult);
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_CROP_REQ:
        {
            switch (nResult)
            {
            case SEL_CROP_1:		// 크락 가공
            case SEL_CROP_2:		// 크락 가공
            case SEL_CROP_3:		// 크락 가공
            case SEL_CROP_4:		// 크락 가공
            case SEL_CROP_5:		// 크락 가공
                {
                    OpenProcessList(SEL_CROP, nResult);
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------

void CUIProcessNPC::clearVecBtnEX()
{
    vecItems_Iter iter = m_vecItems.begin();
    vecItems_Iter eiter = m_vecItems.end();

    for (; iter != eiter; ++iter)
    {
        SAFE_DELETE(*iter);
    }

    m_vecItems.clear();
}

void CUIProcessNPC::initialize()
{
#ifndef		WORLD_EDITOR

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

	m_pListItems = (CUIList*)findUI("list_process");

	if (m_pListItems != NULL)
	{
		m_pListItems->SetKeepSelect(true);
		m_pListItems->SetCommandFSelect(boost::bind(&CUIProcessNPC::callback_select, this));
	}

	m_pListDesc = (CUIList*)findUI("list_desc");

	if (m_pListDesc != NULL)
	{
		CUIListItem* pItem = m_pListDesc->GetListItemTemplate();

		if (pItem != NULL)
		{
			CUIBase* pbase = pItem->findUI("txt_line");

			if (pbase != NULL)
				m_nNeedTextWidth = pbase->GetWidth();
		}
	}

	m_pBtnOK = (CUIButton*)findUI("btn_ok");

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetCommandFUp(boost::bind(&CUIProcessNPC::SendProcessNPCReq, this));

	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProcessNPC::CloseProcessNPC, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProcessNPC::CloseProcessNPC, this));

#endif		// WORLD_EDITOR
}

void CUIProcessNPC::pushback_string( CTString& strText, COLOR color )
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

void CUIProcessNPC::callback_select()
{
	if (m_pListItems == NULL)
		return;

	int sel = m_pListItems->getCurSel();

	show_spinctrl(m_nSelectProcessNPCItem, TRUE);

	// 다른게 선택되었다면
	if (m_nSelectProcessNPCItem != sel)
		m_nProcessItemCount = 1;

	m_nSelectProcessNPCItem = sel;
	SelectItem(sel);
}

void CUIProcessNPC::show_spinctrl( int idx, BOOL bhide )
{
	if (m_pListItems == NULL)
		return;

	if (idx < 0 || idx >= m_pListItems->getListItemCount())
		return;

	// 이전에 선택된 것이 있다면 Spin 숨김
	CUIBase* pItem = m_pListItems->GetListItem(m_nSelectProcessNPCItem);

	if (pItem != NULL)
	{
		CUISpinControl* pSpin = (CUISpinControl*)pItem->findUI("spin_cnt");

		if (pSpin != NULL)
		{
			pSpin->Hide(bhide);

			if (bhide == FALSE)
			{
				// default 숫자
				CTString str;
				str.PrintF("%d", m_nProcessItemCount);
				pSpin->SetString(str.str_String);
			}
		}
	}
}

void CUIProcessNPC::callback_spin()
{
	if (m_pListItems == NULL)
		return;

	if (m_nSelectProcessNPCItem < 0 || m_nSelectProcessNPCItem >= m_pListItems->getListItemCount())
		return;

	CUIBase* pItem = m_pListItems->GetListItem(m_nSelectProcessNPCItem);

	if (pItem != NULL)
	{
		CUISpinControl* pSpin = (CUISpinControl*)pItem->findUI("spin_cnt");

		if (pSpin != NULL)
		{			
			char* str_num = pSpin->GetString();

			int num = atoi(str_num);

			if (num > 0 && num <= 10)
			{
				m_nProcessItemCount = num;

				SelectItem(m_nSelectProcessNPCItem);
			}			
		}
	}
}
