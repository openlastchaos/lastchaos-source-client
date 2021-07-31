#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "ProductNPCUI.h"
#include <Engine/Interface/UIProduct.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Info/MyInfo.h>
#include "ProductNPCCmd.h"
#include <Engine/Contents/function/Product2UI.h>

static int	_nMsgBoxLineHeight = 0;
//*****************************
// CUIProductNPC
//*******************************
CUIProductNPC::CUIProductNPC()
	: m_pTreeTemp(NULL)
	, m_pTree(NULL)
	, m_pTreePos(NULL)
	, m_pTreeItemTemp(NULL)
	, m_pScroll(NULL)
	, m_pTxtNas(NULL)
	, m_pTxtSP(NULL)
	, m_nfactory_idx(-1)
	, m_nsel_sub(-1)
	, m_nsel_idx(-1)
	, m_colPrev(DEF_UI_COLOR_WHITE)
{
	setInherit(false);

	Reset();
	m_nNpcVirIdx = -1;
}

CUIProductNPC::~CUIProductNPC()
{
	Reset();

	Destroy();
}

void CUIProductNPC::OpenProductNPC(int iMobIndex, int iMobVirIdx, FLOAT npcx, FLOAT npcz)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_fNpcX = npcx;
	m_fNpcZ = npcz;
	m_nNpcVirIdx = iMobVirIdx;

	if (iMobIndex == 1069)//제작연합 위원장 머리
	{
		pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_VOUCHER );
		
		pUIManager->CreateMessageBoxL( _S( 4621, "제작연합 위원장"  ) , UI_PRODUCTNPC, MSGLCMD_PRODUCETNPC_VOUCHER );		
		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4622, "이방인이여 어서 오십시오 저희 제작연합은 무기와 방어구 제작 기술로 지금까지 삶을 번영 시키고 부를 축척할 수 있었습니다." ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4623, "그 결과 다른 많은 세력으로부터 부와 기술을 노리는 침략이 끊임없이 이루어졌습니다. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4624, "그래서 저희 제작연합에서는 기술을 원하는 자에게 연합회원으로 받아들여 제작 기술을 외부로 보급하기로 결정하였습니다. " ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4625, "이유는 더 이상의 침략으로부터 벗어나 무기와 방어구 제작이 아닌 생활에 필요한 제작 기술을 발전하기 위함입니다. " ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4626, "이방인이여 제작연하회원에 관심이 있으신가요?" ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4627, "제작 연합 회원 등록" ), SEAL_MEMBER );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4628, "정교한 방어구 제작자 증표" ), SEAL_1 );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4629, "화려한 방어구 제작자 증표" ), SEAL_2 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4630, "날카로운 무기 제작자 증표" ), SEAL_3 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4631, "단단한 무기 제작자 증표" ), SEAL_4 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4632, "소모품 제작자 증표" ), SEAL_5 );

	}else
	{
		pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_LEARN );
	
		pUIManager->CreateMessageBoxL( _S( 4633, "제작 기술 NPC"  ) , UI_PRODUCTNPC, MSGLCMD_PRODUCETNPC_LEARN );		
		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4634, "무슨 일로 저를 찾아오셨습니까?" ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4635, "제작기술을 전수 받기 ㅇ위해 오셨다고요? 저희 제작 기술은 아무나 전수 받을 수 있는 것이 아닙니다. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4636, "제작 연합 위원장 머리님을 통하여 회원으로 등록된 자들만 전수 받을 수 있습니다. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4637, "위원장님께 가서 증표를 받아오시면 제작 기술을 전수해 드리겠습니다. 증표를 받아오셨다면 증표를 보여주십시오." ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4638, "정교한 방어구 제작기술" ), SEAL_1 );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4639, "화려한 방어구 제작기술" ), SEAL_2 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4640, "날카로운 무기 제작기술" ), SEAL_3 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4641, "단단한 무기 제작기술" ), SEAL_4 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4642, "소모품 제작기술" ), SEAL_5 );

	}

	pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1220, "취소한다." ) );
}

void CUIProductNPC::OpenProductList(INDEX sealtype)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetProduct2()->IsVisible())
	{
		pUIManager->GetProduct2()->CloseProduct2();
	}

	Reset();
	m_nSealType = sealtype;

	set_data();

	// If this is already exist
	Hide(FALSE);
	if( IsVisible() == TRUE )
		return;

	pUIManager->RearrangeOrder( UI_PRODUCTNPC, TRUE );
}

void CUIProductNPC::CloseProductNPC()
{
	Reset();
	m_nNpcVirIdx = -1;
	CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCTNPC, FALSE );
	Hide(TRUE);
}

void CUIProductNPC::Reset()
{
	m_nSealNum = -1;
	m_nfactory_idx = -1;
	m_nsel_sub = -1;
	m_nsel_idx = -1;

	if (m_pTxtNas != NULL)
		m_pTxtNas->SetText(CTString(""));

	if (m_pTxtSP != NULL)
		m_pTxtSP->SetText(CTString(""));
}

void CUIProductNPC::MsgBoxLCommand( int nCommandCode, int nResult )
{

	switch( nCommandCode )
	{
	case MSGLCMD_PRODUCETNPC_VOUCHER:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			switch( nResult )
			{
			case SEAL_MEMBER:	//연합 등록
				{
					CUIMsgBox_Info	MsgBoxInfo;
					CSkill &rSkill = _pNetwork->GetSkillData(SEALTYPE_START_INDEX-1);
					SQUAD nNeedCount = rSkill.GetLearnItemCount(0,0);
					CTString strTemp, strCount;
					strCount.PrintF("%I64d", nNeedCount);
					pUIManager->InsertCommaToString(strCount);
					strTemp.PrintF(_S(904, "필요 나스 : %s" ), strCount);

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_PRODUCTNPC, MSGCMD_SEAL_MEMBER );		
					MsgBoxInfo.AddString( _S( 4712, "제작 연합 회원 등록을 하시겠습니까?") );
					MsgBoxInfo.AddString( strTemp );
					pUIManager->CreateMessageBox( MsgBoxInfo );

				}break;
			case SEAL_1:	// 제작 증표 요청			
			case SEAL_2:				
			case SEAL_3:				
			case SEAL_4:
			case SEAL_5:
				{
					m_nSealNum = SEALTYPE_START_INDEX+nResult;

					CUIMsgBox_Info	MsgBoxInfo;
					CSkill &rSkill = _pNetwork->GetSkillData(m_nSealNum);
					SQUAD nNeedCount = rSkill.GetLearnItemCount(0,0);
					CTString strTemp, strCount;
					strCount.PrintF("%I64d", nNeedCount);
					pUIManager->InsertCommaToString(strCount);
					strTemp.PrintF(_S(904, "필요 나스 : %s" ), strCount);

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_PRODUCTNPC, MSGCMD_LEARN_SEAL );		
					MsgBoxInfo.AddString( _S( 4713, "제작자 증표를 획득 하시겠습니까?") );
					MsgBoxInfo.AddString( strTemp );
					pUIManager->CreateMessageBox( MsgBoxInfo );

				}break;
			default:
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_VOUCHER );
				}
			}
		}break;
	case MSGLCMD_PRODUCETNPC_LEARN:
		{
			switch(nResult)
			{
			case SEAL_1:	// 제작 증표 요청			
			case SEAL_2:				
			case SEAL_3:				
			case SEAL_4:
			case SEAL_5:
				{
					_pNetwork->SendMakeItemList(SEALTYPE_START_INDEX+nResult,FALSE );
					
				}break;
			default:
				{
					CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_LEARN );
				};
				
			}
		}break;
	}

}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : [090824 sora]
// ----------------------------------------------------------------------------
void CUIProductNPC::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_SEAL_MEMBER:
			{
				if(bOK)
				{
					CNetworkMessage nmMessage;
					RequestClient::sealSkillLearn* packet = reinterpret_cast<RequestClient::sealSkillLearn*>(nmMessage.nm_pubMessage);
					packet->type = MSG_SKILL;
					packet->subType = MSG_SKILL_LEARN_SEAL;
					packet->npcIndex = m_nNpcVirIdx;
					packet->skillIndex = SEALTYPE_START_INDEX-1; //연합등록 스킬 인덱스 655
					nmMessage.setSize( sizeof(*packet) );

					_pNetwork->SendToServerNew(nmMessage);
				}
			}
			break;

		case MSGCMD_LEARN_SEAL:
			{
				if(bOK)
				{
					CNetworkMessage nmMessage;
					RequestClient::sealSkillLearn* packet = reinterpret_cast<RequestClient::sealSkillLearn*>(nmMessage.nm_pubMessage);
					packet->type = MSG_SKILL;
					packet->subType = MSG_SKILL_LEARN_SEAL;
					packet->npcIndex = m_nNpcVirIdx;
					packet->skillIndex = m_nSealNum;
					nmMessage.setSize( sizeof(*packet) );

					_pNetwork->SendToServerNew(nmMessage);
				}
				else
				{
					m_nSealNum = -1;
				}
			}
			break;

		default:
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage(_S(16, "알수 없는 오류가 발생했습니다."));
			break;
	}
}


void CUIProductNPC::SendFactoryLearn(int nIndex)
{
	CNetworkMessage nmMessage(MSG_FACTORY);

	nmMessage << (UBYTE)MSG_FACTORY_LEARN;
	nmMessage << (ULONG) nIndex;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIProductNPC::initialize()
{
#ifndef		WORLD_EDITOR

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

	m_pTreeTemp = (CUITree*)findUI("tree_list");

	m_pTreePos = findUI("ui_pos");
	m_pTreeItemTemp = (CUITreeItem*)findUI("ti_sub");
	m_pScroll = (CUIScrollBar*)findUI("scroll");
	m_pTxtNas = (CUIText*)findUI("txt_nas");
	m_pTxtSP = (CUIText*)findUI("txt_sp");

	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::CloseProductNPC, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::CloseProductNPC, this));

	pBtn = (CUIButton*)findUI("btn_ok");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::_callback_ok, this));


#endif		// WORLD_EDITOR
}

bool comp(CMakeItemData* a, CMakeItemData* b)
{
	if (a == NULL || b == NULL)
		return false;

	if (a->GetFactoryIndex() < b->GetFactoryIndex())
		return true;

	return false;
}

const int sub_type_string_idx[] = {
	2544, // "투구"
	4273, // "상의"
	4275, // "장갑"
	4274, // "하의"
	4276, // "신발"
	4277, // "방패"
	4613, // "날개"
	1039, // "석궁"
	1040, // "스태프"
	1042, // "도끼"
	1046, // "이도류"
	1047, // "힐러완드"
	2307, // "폴암"
	4614, // "혼"
	1038, // "기사도"
	1041, // "대검"
	1043, // "숏스태프"
	1044, // "활"
	1045, // "단검"
	2306, // "사이드"
	4266, // "일회용"
	4268, // "포션"
	4267, // "기타"
};

bool CUIProductNPC::set_data()
{
	if (m_pTreeTemp == NULL || m_pTreePos == NULL ||
		m_pTreeItemTemp == NULL)
		return false;

	if (m_nSealType < 0)
		return false;

	int nSealtypenum = m_nSealType - SEALTYPE_START_INDEX;
	//nSealSubCount는 List 저장을 위해 subtype 갯수를 저장함
	int nSealSubCount = SealTypeCount[nSealtypenum+1] - SealTypeCount[nSealtypenum];	

	MyInfo* pMyinfo = MY_INFO();

	int		i;
	//Seal type에 따른 Sub타입에 맞게 저장 시킴
	for( i = 0; i < _pNetwork->wo_iNomOfMakeItemData; ++i )
	{
		CMakeItemData* temMakeItem =  &_pNetwork->wo_aMakeItemData[i];

		if (temMakeItem != NULL && temMakeItem->GetMakeItemType() == m_nSealType)
		{
			{
				// 배우지 않은 스킬 리스트 저장
				if (pMyinfo->find_makeitem(temMakeItem->GetFactoryIndex()) == false)
				{
					m_vecList.push_back(temMakeItem);
				}

			}

		}
	}

	std::sort(m_vecList.begin(), m_vecList.end(), comp);

	// 기존 트리에 데이터가 있다면 삭제한다.
	if (m_pTree != NULL)
	{
		m_pTreePos->deleteChild(m_pTree);
		m_pTree = NULL;
	}

	// tree root 처리
	{
		m_pTree = (CUITree*)m_pTreeTemp->Clone();
		m_pTree->setRoot();
		m_pTreePos->addChild(m_pTree);

		m_pScroll->Hide(FALSE);
		m_pTree->setScroll(m_pScroll);
	}

	CUITree*		ptree_clone = NULL;
	CUITree*		ptree_child = NULL;
	CUIBase*		pclone = NULL;
	CUIText*		pText = NULL;

	int temStartpos = SealTypeCount[nSealtypenum];
	CTString strTemp;
	UQUAD nSealExp = MY_INFO()->GetSkillExp(m_nSealType);

	vec_iter endIter = m_vecList.end();
	vec_iter startIter;

	int		idx;
	// List에 SubType대로 저장함
	for( i = 0; i < nSealSubCount; i++)
	{
		strTemp = _S(sub_type_string_idx[temStartpos + i], "");

		// 서브 메뉴 추가
		{
			ptree_clone = (CUITree*)m_pTreeTemp->Clone();

			pclone = m_pTreeItemTemp->Clone();
			pclone->Hide(FALSE);
			pText = (CUIText*)pclone->findUI("txt_");
			pText->SetText(strTemp);

			ptree_clone->addChild(pclone);		// Child 를 붙인 후 			
		}

		startIter = m_vecList.begin();

		for(; startIter != endIter; ++startIter )
		{
			CMakeItemData* tempdata = (*startIter);
			COLOR colTemp = 0xC0C0C0FF;

			if (SealSubType[temStartpos+i] == tempdata->GetMakeItemSubType())
			{
				strTemp = tempdata->GetName();
				SQUAD temResult = nSealExp - tempdata->GetNeedExp();

				if (temResult < 10)
				{
					colTemp = 0xFF8000FF;
				}else if (temResult < 20)
				{
					colTemp = 0xFFFF00FF;
				}else if (temResult < 30)
				{
					colTemp = 0x008000FF;
				}

				ptree_child = (CUITree*)m_pTreeTemp->Clone();
				ptree_child->SetHeight(20);			

				pclone = m_pTreeItemTemp->Clone();
				pclone->Hide(FALSE);
				pText = (CUIText*)pclone->findUI("txt_");
				pText->SetText(strTemp);
				pText->setFontColor(colTemp);

				ptree_child->addChild(pclone);

				ptree_clone->addTree(ptree_child, false);
				idx = ptree_clone->getChildTreeCount() - 1;

				// cmd 등록
				CmdProductNPCSelect<CUIProductNPC>* pcmd = new CmdProductNPCSelect<CUIProductNPC>;
				pcmd->setData(this, i, idx, tempdata->GetFactoryIndex(), colTemp);
				ptree_child->SetCommandUp(pcmd);

				CmdProductNPCOver<CUIProductNPC>* pcmd_over = new CmdProductNPCOver<CUIProductNPC>;
				pcmd_over->setData(this, i, idx, true, DEF_UI_COLOR_OVER);
				ptree_child->SetCommandEnter(pcmd_over);

				CmdProductNPCOver<CUIProductNPC>* pcmd_leave = new CmdProductNPCOver<CUIProductNPC>;
				pcmd_leave->setData(this, i, idx, false, colTemp);
				ptree_child->SetCommandLeave(pcmd_leave);
			}
		}

		m_pTree->addTree(ptree_clone);		// Tree 에 등록해야 높이가 잡힌다.
		ptree_clone->setUncollapse(TRUE);
		ptree_clone->updateTree();
	}

	m_vecList.clear();

	m_pTree->SetScrollPos(0);
	m_pTree->updateTree();

	return true;
}

void CUIProductNPC::_callback_ok()
{
	if (m_nfactory_idx < 0)
		return;

	SendFactoryLearn(m_nfactory_idx);
}

void CUIProductNPC::callback_select( int nsub, int idx, int fac_idx, COLOR col )
{
	CUITree* ptree;

	// 같은 것을 선택하면 리턴
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	// 이전 선택을 되돌림.
	if (m_nsel_sub >= 0 && m_nsel_idx >= 0)
	{
		ptree = getTree(m_nsel_sub, m_nsel_idx);

		if (ptree != NULL)
		{
			set_color(ptree, m_colPrev);
			CUIBase* pimg = ptree->findUI("img_select");

			if (pimg != NULL)
				pimg->Hide(TRUE);
		}
	}

	ptree = getTree(nsub, idx);

	if (ptree == NULL)
		return;

	// select image
	{
		CUIBase* pimg = ptree->findUI("img_select");

		if (pimg != NULL)
			pimg->Hide(FALSE);
	}

	m_nsel_sub = nsub;
	m_nsel_idx = idx;
	m_nfactory_idx = fac_idx;
	m_colPrev = col;

	CMakeItemData& Itemdata = _pNetwork->GetMakeItemData(m_nfactory_idx);

	if (Itemdata.GetFactoryIndex() > 0)
	{
		CTString str;
		CUIManager* pUIMgr = UIMGR();

		if (m_pTxtNas != NULL)
		{
			str.PrintF("%I64d", Itemdata.GetNeedNas());
			pUIMgr->InsertCommaToString(str);
			m_pTxtNas->SetText(str);
		}

		if (m_pTxtSP != NULL)
		{
			str.PrintF("%I64d", Itemdata.GetNeedExp());
			pUIMgr->InsertCommaToString(str);
			m_pTxtSP->SetText(str);
		}		
	}
}

void CUIProductNPC::callback_over( int nsub, int idx, bool bover, COLOR col )
{
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	CUITree* ptree = getTree(nsub, idx);

	if (bover == true)
	{
		set_color(ptree, DEF_UI_COLOR_OVER);
	}
	else
	{
		set_color(ptree, col);
	}
}

CUITree* CUIProductNPC::getTree( int nsub, int idx )
{
	CUITree* ret = NULL;

	if (m_pTree == NULL)
		return ret;

	CUITree* psub = m_pTree->getChildTree(nsub);

	if (psub != NULL)
	{
		ret = psub->getChildTree(idx);
	}

	return ret;
}

void CUIProductNPC::set_color( CUITree* pTree, COLOR col )
{
	if (pTree == NULL)
		return;

	CUIText* ptxt = (CUIText*)pTree->findUI("txt_");

	if (ptxt == NULL)
		return;

	ptxt->setFontColor(col);
}
