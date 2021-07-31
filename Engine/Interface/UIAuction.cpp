#include "stdh.h"
#include <Engine/Interface/UIAuction.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <map>

#define	AUCTION_TAB_WIDTH 101
#define AUCTION_ITEMSTRING_CENTERX 181
#define AUCTION_ITEMCOUNT_CENTERX 367
#define AUCTION_ITEMTOTALNAS_CENTERX 449
#define AUCTION_ETCSTRING_CENTERX 568

#define AUCTION_NAS_MAX 999999999999	//입력가능 나스 최대 금액

#define AUCTION_CANT_SEARCH_AND_REG		// [090616: selo] 판매대행 검색 및 등록 금지

CTString strTabName[AUCTION_TAB_TOTAL];

CTString strItemType[7];

CTString strItemSubTypeWeapon[16];
CTString strItemSubTypeArmor[7];
CTString strItemSubTypeAccessory[9];

CTString strItemClass[8];

CTString strHelp[AUCTION_TAB_TOTAL];


static int nShopSellRate = 0;		//npc아이템 구매시 적용되는 비율
static int nShowItemSlot = 0;		//현재 가리키는 슬롯(아이템 정보 표시용)

const int nLimitRegistItemNumber = 10; // 플레이어당 등록가능한 아이템 개수

// ----------------------------------------------------------------------------
// Name : CUIAuction()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIAuction::CUIAuction()
{
	m_nCurrnetTab = AUCTION_TAB_REFER;
	m_nCurrentPage = 1;
	m_nMaxPage = 1;

	m_nCurretnSorting = -1;
	m_nCurrentSelectedItem = -1;

	m_bReverse = FALSE;
	m_bRegister = FALSE;
	m_bSettle = FALSE;
	m_bSettleRequest = FALSE;

	m_strTitleName = CTString("");

	m_nDepositMoney = 1000;

	m_nSearchedItemType = 0;
	m_nSearchedItemSubType = 0;
	m_nSearchedClass = 0;
	m_strSearched = CTString("");

	m_nNpcIndex = -1;
	m_AuctionSurface.Clear();

	strTabName[0] = _S( 399, "조회" );
	strTabName[1] = _S( 2489, "등록" );
	strTabName[2] = _S( 4265, "정산" );

	//일회용 뒤에 탄환 플래그가 있음
	strItemType[0] =_S( 506, "전체" ); 
	strItemType[1] =_S( 2375, "무기" ); 
	strItemType[2] =_S( 2376, "방어구" ); 
	strItemType[3] =_S( 4266, "일회용" );
	strItemType[4] =_S( 4267, "기타" );
	strItemType[5] =_S( 726, "악세사리" );
	strItemType[6] =_S( 4268, "포션" );

	strItemSubTypeWeapon[0] = _S( 506, "전체" );
	strItemSubTypeWeapon[1] = _S( 1038, "기사도" );
	strItemSubTypeWeapon[2] = _S( 1039, "석궁" );
	strItemSubTypeWeapon[3] = _S( 1040, "스태프" );
	strItemSubTypeWeapon[4] = _S( 1041, "대검" );
	strItemSubTypeWeapon[5] = _S( 1042, "도끼" );
	strItemSubTypeWeapon[6] = _S( 4269, "메이지완드" );
	strItemSubTypeWeapon[7] = _S( 1044, "활" );
	strItemSubTypeWeapon[8] = _S( 1045, "단검" );
	strItemSubTypeWeapon[9] = _S( 4270, "채굴도구" );
	strItemSubTypeWeapon[10] = _S( 4271, "채집도구" );
	strItemSubTypeWeapon[11] = _S( 4272, "차지도구" );
	strItemSubTypeWeapon[12] = _S( 1046, "이도류" );
	strItemSubTypeWeapon[13] = _S( 1047, "힐러완드" );
	strItemSubTypeWeapon[14] = _S( 2306, "사이드" );
	strItemSubTypeWeapon[15] = _S( 2307, "폴암" );

	strItemSubTypeArmor[0] = _S( 506, "전체" );
	strItemSubTypeArmor[1] = _S( 142, "머리" );
	strItemSubTypeArmor[2] = _S( 4273, "상의" );
	strItemSubTypeArmor[3] = _S( 4274, "하의" );
	strItemSubTypeArmor[4] = _S( 4275, "장갑" );
	strItemSubTypeArmor[5] = _S( 4276, "신발" );
	strItemSubTypeArmor[6] = _S( 4277, "방패" );

	strItemSubTypeAccessory[0] = _S( 506, "전체" );
	strItemSubTypeAccessory[1] = _S( 4278, "부적" );
	strItemSubTypeAccessory[2] = _S( 4279, "마련석" );
	strItemSubTypeAccessory[3] = _S( 4280, "반짝이는 돌" );
	strItemSubTypeAccessory[4] = _S( 4281, "귀걸이" );
	strItemSubTypeAccessory[5] = _S( 4282, "반지" );
	strItemSubTypeAccessory[6] = _S( 4283, "목걸이" );
	strItemSubTypeAccessory[7] = _S( 2188, "애완동물" );
	strItemSubTypeAccessory[8] = _S( 4284, "공격형 애완동물" );

	//소서러와 공격형 PET사이에 not use 플래그 하나 있음
	strItemClass[0] = _S( 506, "전체" );
	strItemClass[1] = _S( 43, "타이탄" );
	strItemClass[2] = _S( 44, "기사" );
	strItemClass[3] = _S( 45, "힐러" );
	strItemClass[4] = _S( 46, "메이지" );
	strItemClass[5] = _S( 47, "로그" );
	strItemClass[6] = _S( 48, "소서러" );
	strItemClass[7] = _S( 4284, "공격형 애완동물" );

	strHelp[0] = CTString( "-" );
	strHelp[1] = _S( 4285,"인벤토리에서 아이템을 등록하여 사용할 수 있습니다.");
	strHelp[2] = _S( 4286,"정산 서비스를 통해 거래를 완료하실 수 있습니다.");

}

// ----------------------------------------------------------------------------
// Name : ~CUIAuction()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIAuction::~CUIAuction()
{
	Destroy();
}


// ----------------------------------------------------------------------------
// Name : Create()
// Desc : 생성
// ----------------------------------------------------------------------------
void CUIAuction::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nCurrnetTab = AUCTION_TAB_REFER;
	m_strTitleName = _S( 4287, "거래 대행 서비스" );
	
	CTextureData* tmpTexture = CreateTexture(CTString("Data\\Interface\\Auction.tex"));
	FLOAT	fTexWidth = tmpTexture->GetPixWidth();
	FLOAT	fTexHeight = tmpTexture->GetPixHeight();

	m_rcTitle.SetRect(0,0,AUCTION_WIDTH,36);

	m_rcRegisterArea.SetRect(0,0,AUCTION_WIDTH,AUCTION_HEIGHT);
	m_rcSelectArea.SetRect(0,0,0,0);
	m_rcItemArea.SetRect(12,86,631,454);
	
	int left = 8; 
	for(int i=0; i<AUCTION_TAB_TOTAL; i++)
	{
		m_rcTabArea[i].SetRect(left,37,left+AUCTION_TAB_WIDTH,58);
		left += (AUCTION_TAB_WIDTH + 3);
	}

	m_AuctionSurface.AddRectSurface(UIRect(0,0,643,509), UIRectUV(0,0,643,509,fTexWidth,fTexHeight));				//메인 윈도우 0
	m_AuctionSurface.AddRectSurface(UIRect(416, 65, 547, 81), UIRectUV(0, 518, 105, 534, fTexWidth, fTexHeight));	//에디트 박스 1
	m_AuctionSurface.AddRectSurface(UIRect(131, 64, 510, 80), UIRectUV(176, 538, 556, 555, fTexWidth, fTexHeight));	//도움말 배경 2

	m_btnFirst.Create( this, CTString( "" ), 252, 480, 17, 19 );
	m_btnFirst.SetUV( UBS_IDLE, 138, 515, 155, 534, fTexWidth, fTexHeight );
	m_btnFirst.SetUV( UBS_CLICK, 217, 515, 234, 534, fTexWidth, fTexHeight );
	m_btnFirst.SetUV( UBS_DISABLE, 138, 515, 155, 534, fTexWidth, fTexHeight );
	m_btnFirst.CopyUV( UBS_IDLE, UBS_ON );
	m_btnFirst.SetEnable(FALSE);

	m_btnPrev.Create( this, CTString( "" ), 274, 480, 11, 19 );
	m_btnPrev.SetUV( UBS_IDLE, 160, 515, 171, 534, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 239, 515, 250, 534, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_DISABLE, 160, 515, 171, 534, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.SetEnable(FALSE);

	m_btnNext.Create( this, CTString( "" ), 372, 480, 11, 19 );
	m_btnNext.SetUV( UBS_IDLE, 179, 515, 190, 534, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_CLICK, 258, 515, 269, 534, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_DISABLE, 179, 515, 190, 534, fTexWidth, fTexHeight );
	m_btnNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNext.SetEnable(FALSE);

	m_btnLast.Create( this, CTString( "" ), 388, 480, 17, 19 );
	m_btnLast.SetUV( UBS_IDLE, 196, 515, 213, 534, fTexWidth, fTexHeight );
	m_btnLast.SetUV( UBS_CLICK, 275, 515, 292, 534, fTexWidth, fTexHeight );
	m_btnLast.SetUV( UBS_DISABLE, 196, 515, 213, 534, fTexWidth, fTexHeight );
	m_btnLast.CopyUV( UBS_IDLE, UBS_ON );
	m_btnLast.SetEnable(FALSE);

	m_cmbItemType.Create( this, 11, 65, 105, 16, 87, 6, 13, 7, 16, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbItemType.SetBackUV( 0, 518, 105, 534, fTexWidth, fTexHeight );
	m_cmbItemType.SetDownBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbItemType.SetUpBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbItemType.SetDropListUV( 0, 537, 105, 555, fTexWidth, fTexHeight );
//	m_cmbItemType.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	for(int k=0; k<7; k++)
	{
		m_cmbItemType.AddString(strItemType[k]);
	}

	m_cmbItemSubType.Create( this, 122, 65, 141, 16, 123, 6, 13, 7, 16, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbItemSubType.SetBackUV( 0, 518, 105, 534, fTexWidth, fTexHeight );
	m_cmbItemSubType.SetDownBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbItemSubType.SetUpBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbItemSubType.SetDropListUV( 0, 537, 105, 555, fTexWidth, fTexHeight );
//	m_cmbItemSubType.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	m_cmbItemSubType.AddString(CTString("-"));

	m_cmbClass.Create( this, 269, 65, 141, 16, 123, 6, 13, 7, 16, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbClass.SetBackUV( 0, 518, 105, 534, fTexWidth, fTexHeight );
	m_cmbClass.SetDownBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbClass.SetUpBtnUV( 117, 522, 130, 529, fTexWidth, fTexHeight );
	m_cmbClass.SetDropListUV( 0, 537, 105, 555, fTexWidth, fTexHeight );
//	m_cmbClass.AddString(CTString("-"));
//	m_cmbClass.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	for(int l=0; l<7; l++)
	{
		m_cmbClass.AddString(strItemClass[l]);
	}

	m_ebSearch.Create( this, 421, 65, 126, 15, 40 );
	m_ebSearch.SetFocus( FALSE );
	m_ebSearch.SetReadingWindowUV( 0, 518, 105, 534, fTexWidth, fTexHeight );
	m_ebSearch.SetCandidateUV( 0, 518, 105, 534, fTexWidth, fTexHeight );
	// [090611: selo] 판매 대행시 서버 문제로 검색 Editbox를 사용 불가 하게 함
	m_ebSearch.SetEnable(FALSE);


	m_btnSortItem.Create( this, _S( 4288, "아이템" ), 16, 90, 325, 17 );
	m_btnSortItem.SetUV( UBS_IDLE, 0, 560, 325, 577, fTexWidth, fTexHeight );
	m_btnSortItem.SetUV( UBS_CLICK, 0, 579, 325, 596, fTexWidth, fTexHeight );
	m_btnSortItem.SetUV( UBS_DISABLE, 0, 560, 325, 577, fTexWidth, fTexHeight );
	m_btnSortItem.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortItem.SetEnable(FALSE);

	m_btnSortCount.Create( this, _S( 2396, "수량" ), 343, 90, 50, 17 );
	m_btnSortCount.SetUV( UBS_IDLE, 327, 560, 377, 577, fTexWidth, fTexHeight );
	m_btnSortCount.SetUV( UBS_CLICK, 327, 579, 377, 596, fTexWidth, fTexHeight );
	m_btnSortCount.SetUV( UBS_DISABLE, 327, 560, 377, 577, fTexWidth, fTexHeight );
	m_btnSortCount.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortCount.SetEnable(FALSE);

	m_btnSortTotalNas.Create( this, _S( 4289, "총액" ), 395, 90, 113, 17 );
	m_btnSortTotalNas.SetUV( UBS_IDLE, 379, 560, 492, 577, fTexWidth, fTexHeight );
	m_btnSortTotalNas.SetUV( UBS_CLICK, 379, 579, 492, 596, fTexWidth, fTexHeight );
	m_btnSortTotalNas.SetUV( UBS_DISABLE, 379, 560, 492, 577, fTexWidth, fTexHeight );
	m_btnSortTotalNas.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortTotalNas.SetEnable(FALSE);

	m_btnSortLevel.Create( this, _S( 73, "레벨" ), 510, 90, 118, 17 );
	m_btnSortLevel.SetUV( UBS_IDLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortLevel.SetUV( UBS_CLICK, 494, 579, 612, 596, fTexWidth, fTexHeight );
	m_btnSortLevel.SetUV( UBS_DISABLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortLevel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortLevel.SetEnable(FALSE);

	m_btnSortFinishDay.Create( this, _S( 4290, "마감일자" ), 510, 90, 118, 17 );
	m_btnSortFinishDay.SetUV( UBS_IDLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortFinishDay.SetUV( UBS_CLICK, 494, 579, 612, 596, fTexWidth, fTexHeight );
	m_btnSortFinishDay.SetUV( UBS_DISABLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortFinishDay.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortFinishDay.SetEnable(FALSE);

	m_btnSortPassDay.Create( this, _S( 4291, "상태" ), 510, 90, 118, 17 );
	m_btnSortPassDay.SetUV( UBS_IDLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortPassDay.SetUV( UBS_CLICK, 494, 579, 612, 596, fTexWidth, fTexHeight );
	m_btnSortPassDay.SetUV( UBS_DISABLE, 494, 560, 612, 577, fTexWidth, fTexHeight );
	m_btnSortPassDay.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSortPassDay.SetEnable(FALSE);

	
	tmpTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	fTexWidth = tmpTexture->GetPixWidth();
	fTexHeight = tmpTexture->GetPixHeight();

	m_AuctionSurface.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(145, 138, 239, 171, fTexWidth, fTexHeight));	//선택이미지 3
	
	// 아이템 상세정보창 4~12
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,137,19,156,fTexWidth,fTexHeight));	// 20 * 20
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,137,120,156,fTexWidth,fTexHeight));	// 101 * 20
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,137,140,156,fTexWidth,fTexHeight));	// 20 * 20

	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,157,19,206,fTexWidth,fTexHeight));	// 20 * 50
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,157,120,206,fTexWidth,fTexHeight)); // 101 * 50
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,157,140,206,fTexWidth,fTexHeight)); // 20 * 50

	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,207,19,226,fTexWidth,fTexHeight));	// 20 * 20
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(20,207,120,226,fTexWidth,fTexHeight)); // 101 * 20
	m_AuctionSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(121,207,140,226,fTexWidth,fTexHeight));	// 20 * 20

	m_rtSelectedTab.SetUV(0, 106, 101, 128, fTexWidth, fTexHeight );
	m_rtUnSelectedTab.SetUV(104, 106, 205, 128, fTexWidth, fTexHeight );

	m_btnClose.Create( this, CTString( "" ), 619, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK,229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_DISABLE,155, 74, 169, 88, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	
	m_btnSearch.Create( this, _S( 386, "검색" ), 552, 62, 79, 22 );
	m_btnSearch.SetUV( UBS_IDLE, 113, 0, 183, 21, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_CLICK, 186, 0, 256, 21, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_DISABLE, 145, 178, 215, 199, fTexWidth, fTexHeight );
	m_btnSearch.CopyUV( UBS_IDLE, UBS_ON );
#ifdef AUCTION_CANT_SEARCH_AND_REG	// [090616: selo] 판매대행 검색 버튼 비활성화
	m_btnSearch.SetEnable(FALSE);
#endif

	m_btnBuy.Create( this, _S( 600, "구입" ), 533, 478, 98, 22 );
	m_btnBuy.SetUV( UBS_IDLE, 113, 0, 183, 21, fTexWidth, fTexHeight );
	m_btnBuy.SetUV( UBS_CLICK, 186, 0, 256, 21, fTexWidth, fTexHeight );
	m_btnBuy.SetUV( UBS_DISABLE, 145, 178, 215, 199, fTexWidth, fTexHeight );
	m_btnBuy.CopyUV( UBS_IDLE, UBS_ON );

	m_btnCancelRegister.Create( this, _S( 4292, "등록취소" ), 533, 478, 98, 22 );
	m_btnCancelRegister.SetUV( UBS_IDLE, 113, 0, 183, 21, fTexWidth, fTexHeight );
	m_btnCancelRegister.SetUV( UBS_CLICK, 186, 0, 256, 21, fTexWidth, fTexHeight );
	m_btnCancelRegister.SetUV( UBS_DISABLE, 145, 178, 215, 199, fTexWidth, fTexHeight );
	m_btnCancelRegister.CopyUV( UBS_IDLE, UBS_ON );

	m_btnExecuteSettlement.Create( this, _S( 4265, "정산" ), 533, 478, 98, 22 );
	m_btnExecuteSettlement.SetUV( UBS_IDLE, 113, 0, 183, 21, fTexWidth, fTexHeight );
	m_btnExecuteSettlement.SetUV( UBS_CLICK, 186, 0, 256, 21, fTexWidth, fTexHeight );
	m_btnExecuteSettlement.SetUV( UBS_DISABLE, 145, 178, 215, 199, fTexWidth, fTexHeight );
	m_btnExecuteSettlement.CopyUV( UBS_IDLE, UBS_ON );

	int nBtnPosY = 112;
	for(int j=0; j<AUCTION_SLOT_MAX; ++j)
	{
		m_btnAuctionItem[j].Create( this, 17, nBtnPosY, BTN_SIZE, BTN_SIZE, UI_AUCTION, UBET_ITEM );
		nBtnPosY += 36;
	}

	m_btnRegisterItem.Create(this, -1, -1, BTN_SIZE, BTN_SIZE, UI_AUCTION, UBET_ITEM);

}

// ----------------------------------------------------------------------------
// Name : OpenAuction(int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ) (npc 인덱스, 위치값) 
// Desc : 거래대행창 열기
// ----------------------------------------------------------------------------
void CUIAuction::OpenAuction(int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ)
{
	
	InitAll();

	if(nShopSellRate == 0)
	{
		const CShopData &SD = _pNetwork->GetShopData(20); //임시 쥬노 잡화상 기준 
		nShopSellRate = SD.GetSellRate();
	}

	m_nNpcIndex = nNpcIndex;
	m_fNpcX = fNpcPosX;
	m_fNpcZ = fNpcPosZ;

	_pUIMgr->RearrangeOrder(UI_AUCTION, TRUE);
}

// ----------------------------------------------------------------------------
// Name : CloseAuction()
// Desc : 거래대행창 닫기
// ----------------------------------------------------------------------------
void CUIAuction::CloseAuction()
{
	CloseAuctionMessageBox();

	_pUIMgr->RearrangeOrder(UI_AUCTION, FALSE);
}


// ----------------------------------------------------------------------------
// Name : CloseAuctionMessageBox()
// Desc : 거래대행창 에서 사용한 메시지박스가 열려있을 경우 닫아준다
// ----------------------------------------------------------------------------
void CUIAuction::CloseAuctionMessageBox()
{
	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_ITEMREGISTER))		//아이템 등록창
		_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_ITEMREGISTER);
	
	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_BUYITEM))			//아이템 구매창
		_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_BUYITEM);
	
	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_CANCELREGISTER))		//아이템 등록취소창
		_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_CANCELREGISTER);

	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_INFO))				//등록완료, 취소등	
		_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_INFO);
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc : 렌더
// ----------------------------------------------------------------------------
void CUIAuction::Render()
{

	//일정 거리이상 떨어지면 창을 닫는다
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		CloseAuction();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\Auction.tex")));
	
	m_AuctionSurface.SetPos(m_nPosX, m_nPosY);
	m_AuctionSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, 0);
	m_btnFirst.Render();
	m_btnPrev.Render();
	m_btnNext.Render();
	m_btnLast.Render();

	m_btnSortItem.Render();
	m_btnSortCount.Render();
	m_btnSortTotalNas.Render();

	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{
		m_btnSortLevel.Render();
	}
	else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)
	{
		m_btnSortFinishDay.Render();
	}
	else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT)
	{
		m_btnSortPassDay.Render();
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString( "Data\\Interface\\CommonBtn.tex" )));

	for(int i=0; i<AUCTION_TAB_TOTAL; i++)
	{
		if(i == m_nCurrnetTab)
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTabArea[i].Left , m_nPosY + m_rcTabArea[i].Top, m_nPosX + m_rcTabArea[i].Right, m_nPosY + m_rcTabArea[i].Bottom,
							m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1,
							0xFFFFFFFF );
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTabArea[i].Left , m_nPosY + m_rcTabArea[i].Top, m_nPosX + m_rcTabArea[i].Right, m_nPosY + m_rcTabArea[i].Bottom,
							m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
							0xFFFFFFFF );		
		}

	}


	m_btnClose.Render();
	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{
		// [090616: selo] 경매대행 검색버튼 비활성화
		if( m_btnSearch.IsEnabled() )
			m_btnSearch.Render();
		m_btnBuy.Render();
	}
	else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)
	{
		m_btnCancelRegister.Render();
	}
	else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT)
	{
		m_btnExecuteSettlement.Render();
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	int nStrPosY = 0;
	CTString strTemp = CTString("");

	if (m_AuctionItemData.Count() > 0) // 아이템 리스트가 있다면
	{
		int nCnt;

		for (nCnt = 0; nCnt < m_AuctionItemData.Count(); ++nCnt)
		{
			m_btnAuctionItem[nCnt].Render();

			nStrPosY = m_nPosY + (nCnt * 36) + 123;
			strTemp.PrintF("%s", GetItemFullName(m_btnAuctionItem[nCnt]) );
			_pUIMgr->GetDrawPort()->PutTextExCX( strTemp,m_nPosX + AUCTION_ITEMSTRING_CENTERX, nStrPosY, 0xFFFFFFFF);
			strTemp.PrintF("%d", m_btnAuctionItem[nCnt].GetItemCount());
			_pUIMgr->GetDrawPort()->PutTextExCX( strTemp,m_nPosX + AUCTION_ITEMCOUNT_CENTERX, nStrPosY, 0xFFFFFFFF);

			if(m_AuctionItemData[nCnt].llNasTotal > 0)
			{
				//strTemp.PrintF("%I64d", m_nNasTotal[l]);
				//_pUIMgr->InsertCommaToString( strTemp );
				strTemp = _pUIMgr->NasNumToWord(m_AuctionItemData[nCnt].llNasTotal);
			}
			else
			{
				strTemp = CTString("-");
			}
			
			_pUIMgr->GetDrawPort()->PutTextExRX( strTemp,m_nPosX + AUCTION_ITEMTOTALNAS_CENTERX + 50, nStrPosY ,_pUIMgr->GetNasColor(m_AuctionItemData[nCnt].llNasTotal));
			
			if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
			{
				strTemp.PrintF("%d", m_AuctionItemData[nCnt].nEtc);
				_pUIMgr->GetDrawPort()->PutTextExCX( strTemp, m_nPosX + AUCTION_ETCSTRING_CENTERX, nStrPosY ,0xFFFFFFFF);
			}
			else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)
			{
				if(m_AuctionItemData[nCnt].AuctionItemStatus == MSG_TRADEAGENT_FINISH_DAY)
				{
					strTemp.PrintF(_S(2511, "%d일"), m_AuctionItemData[nCnt].nEtc);
				}
				else if(m_AuctionItemData[nCnt].AuctionItemStatus == MSG_TRADEAGENT_FINISH_HOUR)
				{
					strTemp.PrintF(_S(2512, "%d시간"), m_AuctionItemData[nCnt].nEtc);
				}
				else if(m_AuctionItemData[nCnt].AuctionItemStatus == MSG_TRADEAGENT_FINISH_MIN)
				{
					strTemp.PrintF(_S(2513, "%d분"), m_AuctionItemData[nCnt].nEtc);
				}
				
				_pUIMgr->GetDrawPort()->PutTextExCX( strTemp, m_nPosX + AUCTION_ETCSTRING_CENTERX, nStrPosY ,0xFFFFFFFF);
			}
			else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT)
			{
				switch(m_AuctionItemData[nCnt].AuctionItemStatus)
				{
					case MSG_TRADEAGENT_STATE_SELL_BEFORE:
						strTemp.PrintF(_S(4310, "판매중"));
						break;
					case MSG_TRADEAGENT_STATE_SELL_COMPLETE:
						strTemp.PrintF(_S(4311, "판매(%d일)"), m_AuctionItemData[nCnt].nEtc);
						break;
					case MSG_TRADEAGENT_STATE_SELL_RETURNED:
						strTemp.PrintF(_S(4312, "반송(%d일)"), m_AuctionItemData[nCnt].nEtc);
						break;
					case MSG_TRADEAGENT_STATE_CALCULATE_COMPLETE:
						strTemp.PrintF(_S(4313, "정산완료"));
						break;
					case MSG_TRADEAGENT_STATE_CALCULATE_OVERTIME:
						strTemp.PrintF(_S(4379, "정산기한 초과"));
						break;
					case MSG_TRADEAGENT_STATE_BUY:
						strTemp.PrintF(_S(4314, "구입(%d일)"), m_AuctionItemData[nCnt].nEtc);
						break;
				}
				_pUIMgr->GetDrawPort()->PutTextExCX( strTemp, m_nPosX + AUCTION_ETCSTRING_CENTERX, nStrPosY ,0xFFFFFFFF);
			}
		}
	}

	_pUIMgr->GetDrawPort()->PutTextExCX( m_strTitleName, m_nPosX + (AUCTION_WIDTH / 2), m_nPosY + 15, 0xDED9A0FF );

	COLOR colorStr = 0xFFFFFFFF;
	for(int j=0; j<AUCTION_TAB_TOTAL; ++j)
	{
		if(j == m_nCurrnetTab)
		{
			colorStr = 0xFFFFFFFF;
		}
		else
		{
			colorStr = 0xa3a1a3ff;
		}
		_pUIMgr->GetDrawPort()->PutTextExCX( strTabName[j], m_nPosX + m_rcTabArea[j].GetCenterX() + 1, m_nPosY + 44, colorStr );
	}

	CTString strPage = CTString("");
	strPage.PrintF("%d/%d", m_nCurrentPage, m_nMaxPage);
	_pUIMgr->GetDrawPort()->PutTextExCX( strPage, m_nPosX + 328, m_nPosY + 482, 0xFFFFFFFF );
	
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	_pUIMgr->GetDrawPort()->EndTextEx();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString( "Data\\Interface\\CommonBtn.tex" )));
	
	if(m_nCurrentSelectedItem != -1)
	{
		m_AuctionSurface.m_RectSurfaceArray[3].m_RT = m_rcSelectArea;
		m_AuctionSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, 3);
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\Auction.tex")));

	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{
		m_cmbItemType.Render();
		m_cmbItemSubType.Render();
		m_cmbClass.Render();

		// [090611: selo] 검색 Editbox가 비할성화 이면 랜더 안하게 한다.
		if( TRUE == m_ebSearch.IsEnabled() )
		{
			m_AuctionSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, 1);
		
			m_ebSearch.Render();

			if(m_ebSearch.DoesShowReadingWindow())
			{
				m_ebSearch.RenderReadingWindow();
			}
		}
	}
	else
	{
		m_AuctionSurface.RenderRectSurface(_pUIMgr->GetDrawPort(), 0xFFFFFFFF, 2);

		_pUIMgr->GetDrawPort()->PutTextExCX( strHelp[m_nCurrnetTab], m_nPosX + 320, m_nPosY + 65, 0xFFFFFFFF );
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();

	if(m_AuctionItemData.Count() > 0)
	{
		m_btnAuctionItem[nShowItemSlot].RenderInfoPopup();
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIAuction::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIAuction::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


// ----------------------------------------------------------------------------
// Name : MouseMessage( MSG *pMsg )
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIAuction::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL bTitleBarClick = FALSE;

	static BOOL	bLButtonDownInBtn = FALSE;

	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();


			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnFirst.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnLast.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
			{
				if( m_btnSearch.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnBuy.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortLevel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_cmbItemType.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_cmbItemSubType.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_cmbClass.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_ebSearch.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortItem.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortCount.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortTotalNas.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)
			{
				if( m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortFinishDay.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT)
			{
				if( m_btnExecuteSettlement.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnSortPassDay.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}

			int nCnt;
			for (nCnt = 0; nCnt < m_AuctionItemData.Count(); ++nCnt)
			{
				if (m_btnAuctionItem[nCnt].MouseMessage(pMsg) != WMSG_FAIL)
				{
					nShowItemSlot = nCnt;
					return WMSG_SUCCESS;
				}
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ))
			{
				nOldX = nX;		nOldY = nY;
				BOOL bInside = FALSE;
				
				if( IsInsideRect( nX, nY, m_rcTitle ) )	{
					bTitleBarClick = TRUE;
				}
				
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )	{
					// Nothing
				}
				else if( m_btnFirst.MouseMessage( pMsg ) != WMSG_FAIL )	{
					// Nothing
				}
				else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )	{
					// Nothing
				}
				else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )	{
					// Nothing
				}
				else if( m_btnLast.MouseMessage( pMsg ) != WMSG_FAIL )	{
					// Nothing
				}
				else if( IsInsideRect(nX, nY, m_rcItemArea) )
				{
					m_nCurrentSelectedItem  = -1;

					for(int nCurrentItem=0; nCurrentItem<m_AuctionItemData.Count(); ++nCurrentItem)
					{
						m_rcSelectArea.SetRect(16, 111 + (nCurrentItem*36), 628, 145 + (nCurrentItem*36));
						
						if(IsInsideRect(nX, nY, m_rcSelectArea))
						{
							m_nCurrentSelectedItem = nCurrentItem;
							break;
						}
					}
				}
				else
				{
					for(int nTabNum=0; nTabNum<AUCTION_TAB_TOTAL; ++nTabNum)  // 탭 선택
					{
						if( IsInsideRect(nX, nY, m_rcTabArea[nTabNum]) && nTabNum != m_nCurrnetTab && m_btnSearch.IsEnabled())
						{
							if (nTabNum == AUCTION_TAB_REGISTER)
								_pNetwork->SendTradeAgentRegListReq(1, m_nCurretnSorting);
							else if (nTabNum == AUCTION_TAB_SETTLEMENT && m_bSettle)
								_pNetwork->SendTradeAgentCalcListReq(1, m_nCurretnSorting);
							else
								SetTab(nTabNum);
						}
					}
				}

				if (m_nCurrnetTab == AUCTION_TAB_REFER) //  조회 페이지
				{
					if( (wmsgResult = m_cmbItemType.MouseMessage( pMsg )) != WMSG_FAIL ) // 메인 타입 선택
					{
						if( wmsgResult == WMSG_COMMAND )
							SetItemSubType(m_cmbItemType.GetCurSel());
					}
					else if( m_cmbItemSubType.MouseMessage( pMsg ) != WMSG_FAIL ) { // 서브 타입 선택

					}
					else if( m_cmbClass.MouseMessage( pMsg ) != WMSG_FAIL ) { // 클래스 선택

					}
					else if( (wmsgResult = m_btnSearch.MouseMessage( pMsg )) != WMSG_FAIL ) { // 검색

					}
					else if( m_btnBuy.MouseMessage( pMsg ) != WMSG_FAIL ) { // 구입

					}
					else if( m_ebSearch.MouseMessage( pMsg ) != WMSG_FAIL ) { // 검색입력 에디트 박스

					}
					else if( m_btnSortItem.MouseMessage( pMsg ) != WMSG_FAIL ) {
						// Nothing
					}
					else if( m_btnSortCount.MouseMessage( pMsg ) != WMSG_FAIL ) {
						// Nothing
					}
					else if( m_btnSortTotalNas.MouseMessage( pMsg ) != WMSG_FAIL )	{
						// Nothing
					}
					else if( m_btnSortLevel.MouseMessage( pMsg ) != WMSG_FAIL ) {  // 레벨 정렬

					}
				}
				else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)	// 등록 페이지
				{
					if( m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )	{

					}
					else if( m_btnSortFinishDay.MouseMessage( pMsg ) != WMSG_FAIL )	{

					}
				}
				else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT) // 정산 페이지
				{
					if( m_btnExecuteSettlement.MouseMessage( pMsg ) != WMSG_FAIL )	{

					}
					else if( m_btnSortPassDay.MouseMessage( pMsg ) != WMSG_FAIL )	{

					}
				}

				_pUIMgr->RearrangeOrder( UI_AUCTION, TRUE );
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
				bTitleBarClick = FALSE;
				if( !IsFocused() )
					return WMSG_FAIL;

				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseAuction();

					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnFirst.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						GoPage(1);
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_nCurrentPage  <= 1)
						{
							GoPage(1);
						}
						else
						{
							GoPage(m_nCurrentPage-1);
						}
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_nCurrentPage  >= m_nMaxPage)
						{
							GoPage(m_nMaxPage);
						}
						else
						{
							GoPage(m_nCurrentPage+1);
						}
					}	
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnLast.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						GoPage(m_nMaxPage);
					return WMSG_SUCCESS;
				}

				if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
				{
					if( ( wmsgResult = m_btnSearch.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							SearchItem(1, m_nCurretnSorting);
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_cmbItemType.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_cmbItemSubType.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_cmbClass.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnBuy.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_BUYITEM))
							{
								return WMSG_SUCCESS;
							}

							if(m_nCurrentSelectedItem >= 0 && m_nCurrentSelectedItem <= m_AuctionItemData.Count())
							{
								m_nRegisterItemPrice = m_AuctionItemData[m_nCurrentSelectedItem].llNasTotal;
								m_btnRegisterItem.Copy(m_btnAuctionItem[m_nCurrentSelectedItem]);
								m_nRegisterItemCount = m_btnAuctionItem[m_nCurrentSelectedItem].GetItemCount();

								CUIMsgBox_Info MsgBoxInfo;
								MsgBoxInfo.SetMsgBoxInfo( _S( 4293,"아이템 구입 정보"), UMBS_OKCANCEL | UMBS_BUTTONEX, UI_AUCTION, MSGCMD_AUCTION_BUYITEM );
								MsgBoxInfo.SetBtnType(m_btnAuctionItem[m_nCurrentSelectedItem].GetBtnType(), -1, 1);
								CTString strTemp, strTemp2;
								strTemp.PrintF("%s",GetItemFullName(m_btnAuctionItem[m_nCurrentSelectedItem]));
								MsgBoxInfo.AddStringEx(strTemp, 0, 10);
								strTemp.PrintF(_S(4294, "아이템 개수 : %I64d"), m_btnAuctionItem[m_nCurrentSelectedItem].GetItemCount());
								MsgBoxInfo.AddStringEx(strTemp, 3, 0);
								strTemp2.PrintF("%I64d", m_AuctionItemData[m_nCurrentSelectedItem].llNasTotal);
								strTemp.PrintF(_S(4295,"총 금액 : %s"), strTemp2);
								MsgBoxInfo.AddStringEx(strTemp, 5, 0);
								
								_pUIMgr->CreateMessageBox(MsgBoxInfo);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_BUYITEM)->GetBtnEx(0).Copy(m_btnAuctionItem[m_nCurrentSelectedItem]);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_BUYITEM)->GetBtnEx(0).SetPos(15, 30);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_BUYITEM)->GetBtnSlotRect(0).SetRect(14, 29, 16 + BTN_SIZE, 31 + BTN_SIZE);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_BUYITEM)->GetBtnInsertSlot(0).SetRect(0,0,0,0);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_BUYITEM)->GetBtnOK().SetEnable(TRUE);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortLevel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(m_nCurretnSorting / 2 == ITEM_SORT_LEVEL)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_LEVEL, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_LEVEL, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortTotalNas.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(m_nCurretnSorting / 2 == ITEM_SORT_TOTALNAS)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_TOTALNAS, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_TOTALNAS, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortItem.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							//m_nCurretnSorting값은 정의된 정렬방식 * 오름,내림차순의2방식으로 정의된 정렬값의 2배로 되어 있음
							if(m_nCurretnSorting / 2 == ITEM_SORT_INDEX)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_INDEX, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_INDEX, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortCount.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(m_nCurretnSorting / 2 == ITEM_SORT_COUNT)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_COUNT, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_COUNT, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
				}
				else if(m_nCurrnetTab ==  AUCTION_TAB_REGISTER)
				{
					if( ( wmsgResult = m_btnCancelRegister.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{	
							if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_CANCELREGISTER))
								_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_CANCELREGISTER);

							if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_ITEMREGISTER))
								_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_ITEMREGISTER);

							if(m_nCurrentSelectedItem != -1)
							{
								if (m_bRegister) // 등록 요청 중일때
								{
									_pNetwork->ClientSystemMessage(_S(4387, "등록요청중에는 할 수 없습니다."), SYSMSG_ERROR);
								}
								else
								{
									m_nSelectedItemCancel = m_nCurrentSelectedItem;
									CUIMsgBox_Info MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S(4292, "등록취소"), UMBS_OKCANCEL, UI_AUCTION, MSGCMD_AUCTION_CANCELREGISTER );
									MsgBoxInfo.AddString(_S(4296, "선택한 아이템의 거래를 취소하시겠습니까?"));
									MsgBoxInfo.AddString(_S(4297, "거래를 취소하면 보증금은 돌려받을 수 없습니다."));

									_pUIMgr->CreateMessageBox(MsgBoxInfo);
								}
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortFinishDay.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(m_nCurretnSorting / 2 == ITEM_SORT_FINISHDAY)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_FINISHDAY, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_FINISHDAY, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
				}
				else if(m_nCurrnetTab ==  AUCTION_TAB_SETTLEMENT)
				{
					if( ( wmsgResult = m_btnExecuteSettlement.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{	
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_AuctionItemData.Count() > 0 && m_bSettle && !m_bSettleRequest) // 정산할 아이템이 있으면 정산 요청
							{
								_pNetwork->SendTradeAgentCalculateReq();
								m_bSettleRequest = TRUE;
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSortPassDay.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if(m_nCurretnSorting / 2 == ITEM_SORT_PASSDAY)
							{
								m_bReverse = !m_bReverse;
								SortItem(ITEM_SORT_PASSDAY, m_bReverse);
							}
							else
							{
								m_bReverse = FALSE;
								SortItem(ITEM_SORT_PASSDAY, m_bReverse);
							}
						}
						return WMSG_SUCCESS;
					}
				}
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					if(m_nCurrnetTab == AUCTION_TAB_REGISTER)
					{
						if(IsInsideRect(nX, nY, m_rcRegisterArea))
						{
#ifndef AUCTION_CANT_SEARCH_AND_REG	// [090616: selo] 판매대행 등록 못하게함
							if(CanRegister())
							{
								if(_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_ITEMREGISTER))
									_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_ITEMREGISTER);

								// 등록 취소 메세지 박스를 닫아 준다.
								if (_pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_CANCELREGISTER))
									_pUIMgr->CloseMessageBox(MSGCMD_AUCTION_CANCELREGISTER);

								m_btnRegisterItem.Copy(_pUIMgr->GetHoldBtn());

								CUIMsgBox_Info MsgBoxInfo;
								CTString strTemp;
								MsgBoxInfo.SetMsgBoxInfo( _S(4298, "아이템 등록 정보"), UMBS_OKCANCEL | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX,
																			UI_AUCTION, MSGCMD_AUCTION_ITEMREGISTER, 250);	
								MsgBoxInfo.AddString( _S(4299, "아이템 개수") );
								MsgBoxInfo.AddString( _S(4300, "판매할 총 금액") );
								MsgBoxInfo.AddString(CTString(" "));
								strTemp.PrintF(_S(4301, "보증금: %I64d나스"), m_nDepositMoney);
								MsgBoxInfo.AddString(strTemp, 0xF2F2F2FF, TEXT_RIGHT);

								MsgBoxInfo.SetInputBox( 0, 15, 0, 130 );
								MsgBoxInfo.SetSEInputBox( 1 , 15, 0, 130 );
								
								MsgBoxInfo.m_nInputPosY += 4;
								MsgBoxInfo.m_nSEInputPosY += 7;

								_pUIMgr->CreateMessageBox( MsgBoxInfo );

								strTemp.PrintF("%I64d", m_btnRegisterItem.GetItemCount());
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_ITEMREGISTER)->GetInputBox().SetString(strTemp.str_String);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_ITEMREGISTER)->GetInputBox().SetFocus(FALSE);
								if(m_btnRegisterItem.GetItemPrice() > 0)
								{
									strTemp.PrintF("%I64d", m_btnRegisterItem.GetItemCount() * ( ( m_btnRegisterItem.GetItemPrice() * nShopSellRate ) / 100 ));
								}
								else
								{
									strTemp.PrintF("0");
								}
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_ITEMREGISTER)->GetSEInputBox().SetString(strTemp.str_String);
								_pUIMgr->GetMessageBox(MSGCMD_AUCTION_ITEMREGISTER)->GetSEInputBox().SetFocus(FALSE);
							}
							else
							{
								CUIMsgBox_Info MsgBoxInfo;
								MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );

								if(_pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM)
								{
									MsgBoxInfo.AddString(_S(4346, "등록할 수 없는 아이템입니다."));
								}
								else
								{
									MsgBoxInfo.AddString(_S(4347, "아이템만 등록할 수 있습니다."));
								}	
								
								_pUIMgr->CreateMessageBox(MsgBoxInfo);	
							}
#endif	// AUCTION_CANT_SEARCH_AND_REG
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

// ----------------------------------------------------------------------------
// Name : IMEMessage( MSG *pMsg )
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIAuction::IMEMessage(MSG *pMsg )
{
	WMSG_RESULT wmsgResult;

	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{
		if( ( wmsgResult = m_ebSearch.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage( MSG *pMsg )
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIAuction::CharMessage(MSG *pMsg )
{
	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{
		if( m_ebSearch.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage( MSG *pMsg )
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIAuction::KeyMessage(MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;

	if(m_nCurrnetTab ==  AUCTION_TAB_REFER)
	{	
		if( pMsg->wParam == VK_RETURN )
		{	
			//엔터키 누르면 겸색
			SearchItem(1, m_nCurretnSorting);
			return WMSG_SUCCESS;
		}

		if( m_ebSearch.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;	
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
// Desc :
// ----------------------------------------------------------------------------
void CUIAuction::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_AUCTION_CANCELREGISTER:	//등록 취소
		{
			if(bOK)
			{
				CancelRegister();
			}
			else
			{

			}
		}
			break;

		case MSGCMD_AUCTION_BUYITEM:		//아이템 구매
		{
			if(bOK)
			{
				BuyItem();
			}
			else
			{

			}
		}
			break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm) 
// (strInput = 등록할 아이템 개수, strConfirm = 등록할 아이템 총액)
// Desc : 아이템 등록 정보 입력 메시지 박스에서 온 값을 받는 부분
// ----------------------------------------------------------------------------
void CUIAuction::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm)
{

	CUIMsgBox_Info MsgBoxInfo;
	CTString strTemp;

	switch( nCommandCode )
	{
		case MSGCMD_AUCTION_ITEMREGISTER:
		{
			if(bOK)
			{
				if( strInput.IsInteger() && strConfirm.IsInteger() )
				{
					SQUAD nItemCount = _atoi64(strInput);
					SQUAD nItemPrice = _atoi64(strConfirm);	
					if(nItemCount <= 0 || nItemPrice <= 0)
					{
						strTemp = _S(4348, "0 이상의 숫자를 입력해 주시기 바랍니다.");
					}
					else if(nItemCount > m_btnRegisterItem.GetItemCount())
					{
						strTemp = _S(4349, "보유 개수를 초과하였습니다.");
					}
					else if(nItemPrice > AUCTION_NAS_MAX)
					{
						strTemp.PrintF(_S(4350, "금액을 1~%I64d사이의 값으로 입력해 주시기 바랍니다."), AUCTION_NAS_MAX);
					}
					else
					{
						m_nRegisterItemCount = nItemCount;
						m_nRegisterItemPrice = nItemPrice;
						RegisterItem();
						return;
					}

				}
				else
				{
					strTemp = _S(4351, "숫자를 입력해 주시기 바랍니다.");
				}
			}
			else
			{
				return;
			}
		}
		break;
	}

	MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString(strTemp);
	
	_pUIMgr->CreateMessageBox(MsgBoxInfo);	
}

// ----------------------------------------------------------------------------
// Name : InitSlotItem()
// Desc : 아이템 슬롯 초기화
// ----------------------------------------------------------------------------
void CUIAuction::InitSlotItem()
{
	int nCnt;

	for (nCnt=0; nCnt < m_AuctionItemData.Count(); ++nCnt)
	{
		m_btnAuctionItem[nCnt].InitBtn();
	}

	m_AuctionItemData.Clear();
	m_nCurrentSelectedItem = -1;
}

// ----------------------------------------------------------------------------
// Name : InitAll()
// Desc : 거래대행 초기화
// ----------------------------------------------------------------------------
void CUIAuction::InitAll()
{
	m_nCurrnetTab = AUCTION_TAB_REFER;
	m_bRegister = FALSE;
	m_bSettleRequest = FALSE;
	InitSlotItem();
	m_cmbItemType.SetCurSel(0);
	SetItemSubType(0);
	m_cmbClass.SetCurSel(0);
	m_ebSearch.ResetString();
	m_nCurrentSelectedItem = -1;
	m_nCurrentPage = 1;
	m_nMaxPage = 1;
	m_nDepositMoney = 1000;
	m_nSearchedItemType = 0;
	m_nSearchedItemSubType = 0;
	m_nSearchedClass = 0;
	m_strSearched = CTString("");

	m_btnSortItem.SetEnable(FALSE);
	m_btnSortCount.SetEnable(FALSE);
	m_btnSortTotalNas.SetEnable(FALSE);
	m_btnSortLevel.SetEnable(FALSE);

	m_btnFirst.SetEnable(FALSE);
	m_btnPrev.SetEnable(FALSE);
	m_btnNext.SetEnable(FALSE);
	m_btnLast.SetEnable(FALSE);
	m_btnSearch.SetEnable(TRUE);
}


// ----------------------------------------------------------------------------
// Name : SetSlotItem(int nIndex, int nAuctionIndex, CUIButtonEx &btnAuctionItem, SQUAD nNasTotal, int nEtc, int nItemStatus)
//			         (   슬롯번호, 거래대행등록번호,          등록할 아이템 버튼,         총 금액, 남은시간,    아이템 상태 )
// Desc : 거래대행 아이템 슬롯에 해당 정보 입력
// ----------------------------------------------------------------------------
void CUIAuction::SetSlotItem(int nIndex, int nAuctionIndex, CUIButtonEx &btnAuctionItem, SQUAD nNasTotal, int nEtc, int nItemStatus)
{
	INDEX iListCnt = m_AuctionItemData.Count();

	if (iListCnt < AUCTION_SLOT_MAX)
	{
		AuctionItems tmpItr;

		tmpItr.AuctionItemIndex = nAuctionIndex;
		tmpItr.AuctionItemStatus = nItemStatus;
		tmpItr.llNasTotal = nNasTotal;
		tmpItr.nEtc = nEtc;
		m_AuctionItemData.Add(tmpItr);  // A += 1;
		m_btnAuctionItem[iListCnt].Copy(btnAuctionItem); // B = A;
	}
}

// ----------------------------------------------------------------------------
// Name : SetItemSubType(int nItemType)
// Desc : 검색콤보박스  아이템 타입에 따른 아이템 서브타입 콤보박스 설정
// ----------------------------------------------------------------------------
void CUIAuction::SetItemSubType(int nItemType)
{
	m_cmbItemSubType.ResetStrings();
	
	switch(nItemType)
	{
		case 0:	//전체
		{
			m_cmbItemSubType.AddString(CTString("-"));
		}
		break;

		case 1: //무기
		{
			for(int i=0; i<16; i++)
			{
				m_cmbItemSubType.AddString(strItemSubTypeWeapon[i]);
			}
		}
		break;

		case 2: //방어구
		{
			for(int i=0; i<7; i++)
			{
				m_cmbItemSubType.AddString(strItemSubTypeArmor[i]);
			}
		}
		break;
		
		case 3: //엑세서리
		{
			for(int i=0; i<9; i++)
			{
				m_cmbItemSubType.AddString(strItemSubTypeAccessory[i]);
			}
		}
		break;
		case 4:	//일회용
		case 5:	//포션
		case 6:	//기타
		{
			m_cmbItemSubType.AddString(_S(506, "전체"));
		}
		break;
		
		default:
		{
			m_cmbItemSubType.AddString(CTString("-"));
		}
		break;
	}
}


// ----------------------------------------------------------------------------
// Name : GetRegisterItemData(CTString &strItemName, SQUAD& nItemCount, SQUAD& nItemPrice)
// Desc : 구매한 아이템의 정보를 반환
// ----------------------------------------------------------------------------
void CUIAuction::GetRegisterItemData(CTString &strItemName, SQUAD& nItemCount, SQUAD& nItemPrice)
{
	strItemName = GetItemFullName(m_btnRegisterItem);
	nItemCount = m_nRegisterItemCount;
	nItemPrice = m_nRegisterItemPrice;
}


// ----------------------------------------------------------------------------
// Name : GetItemFullName(CUIButtonEx &btnItem)
// Desc : 해당 아이템의 전체이름(prefix, +포함)을 얻는다
// ----------------------------------------------------------------------------
CTString CUIAuction::GetItemFullName(CUIButtonEx &btnItem)
{
	CTString strTemp;
	strTemp =_pNetwork->GetItemName( btnItem.GetItemIndex() );

	CItemData &rItemData = _pNetwork->GetItemData(btnItem.GetItemIndex());
	if( rItemData.IsFlag(ITEM_FLAG_RARE)  )
	{
		INDEX iRare = btnItem.GetItemRareIndex();
		CItemRareOption rItemRareOption =_pNetwork->GetRareOptionData(0);
		rItemRareOption =_pNetwork->GetRareOptionData(iRare);
		
		CTString strPrefix = rItemRareOption.GetPrefix();
		if( strPrefix.Length() >0)
			strTemp = strPrefix + CTString(" ") + strTemp;
	}

	if(btnItem.GetItemPlus() > 0)
	{
		if(  btnItem.GetItemFlag() & PLATINUM_MAX_PLUS )
		{
			int plat_plus,diff;
			FLAG_ITEM_PLATINUM_GET(btnItem.GetItemFlag(),plat_plus);
			diff = btnItem.GetItemPlus() - plat_plus;

			CTString strMsg;
			if(diff > 0)
			{
				strMsg.PrintF("+%d",diff) ;
				strTemp +=strMsg;
			}
			if(plat_plus > 0)
			{
				strMsg.PrintF("[+%d]",plat_plus) ;
				strTemp +=strMsg;
			}
		}
		else
		{
			strTemp.PrintF("%s+%d", strTemp, btnItem.GetItemPlus());
		}
	}

	return strTemp;
}

// ----------------------------------------------------------------------------
// Name : SearchItem(int nPageNum, int nAlignType)
// Desc : 아이템 검색 요청(페이지 번호, 정렬 방식)
// ----------------------------------------------------------------------------
void CUIAuction::SearchItem(int nPageNum, int nAlignType)
{
	CTString strSearch;
	strSearch.PrintF("%s", m_ebSearch.GetString());

	m_nSearchedItemType = m_cmbItemType.GetCurSel();
	m_nSearchedItemSubType = m_cmbItemSubType.GetCurSel();
	m_nSearchedClass = m_cmbClass.GetCurSel();
	m_strSearched = strSearch;

	int nItemtype = (m_nSearchedItemType > 3 ? m_nSearchedItemType : m_nSearchedItemType - 1 ); //아이템 타입 중간에 사용되지 않는 부븐을 비켜가기 위함
	int nItemSubType = m_nSearchedItemSubType - 1;
	int nClass =  (m_nSearchedClass > 6 ? m_nSearchedClass : m_nSearchedClass - 1 );			//아이템 타입 중간에 사용되지 않는 부븐을 비켜가기 위함
	
	m_btnSearch.SetEnable(FALSE);	// [090611: selo] 검색 요청 후 서버 응답 전까지 search 버튼을 diable 시킨다.

	_pNetwork->SendTradeAgentSearchReq(nPageNum, nItemtype, nItemSubType, nClass, strSearch, nAlignType);
}


// ----------------------------------------------------------------------------
// Name : IsSearchConditionChanged()
// Desc : 아이템 검색후 조건이 바뀌었는가(콤보박스변경, 검색어 변경등)
// ----------------------------------------------------------------------------
BOOL CUIAuction::IsSearchConditionChanged()
{
	if( m_nSearchedItemType == m_cmbItemType.GetCurSel() &&
	    m_nSearchedItemSubType == m_cmbItemSubType.GetCurSel() &&
		m_nSearchedClass == m_cmbClass.GetCurSel() &&
		m_strSearched == m_ebSearch.GetString() )
	{
		return TRUE;
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : GoPage(int nPageNum)
// Desc : 페이지 이동 요청
// ----------------------------------------------------------------------------
void CUIAuction::GoPage(int nPageNum)
{
/*
	if(nPageNum > m_nMaxPage)
	{
		return;
	}
	else if(nPageNum < 1)
	{
		return;
	}
*/
	InitSlotItem();

	switch(m_nCurrnetTab)
	{
		case AUCTION_TAB_REFER:
			{
				if(IsSearchConditionChanged()) //검색 조건이 바귀었다면
				{
					//이전 조건으로 되돌리고 리스트 요청
					m_cmbItemType.SetCurSel(m_nSearchedItemType);
					m_cmbItemSubType.SetCurSel(m_nSearchedItemSubType);
					m_cmbClass.SetCurSel(m_nSearchedClass);
					m_ebSearch.ResetString();
					m_ebSearch.SetString(m_strSearched.str_String);
				}

				SearchItem(nPageNum, m_nCurretnSorting);
			}
			break;
		case AUCTION_TAB_REGISTER:
			{
				_pNetwork->SendTradeAgentRegListReq(nPageNum, m_nCurretnSorting);
			}
			break;
		case AUCTION_TAB_SETTLEMENT:
			{
				_pNetwork->SendTradeAgentCalcListReq(nPageNum, m_nCurretnSorting);
			}
			break;
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItem()
// Desc : 아이템 구매 요청
// ----------------------------------------------------------------------------
void CUIAuction::BuyItem()
{
	if(m_nCurrentSelectedItem < 0)
	{
		return;
	}
	//_pNetwork->SendTradeAgentBuyReq(m_nAuctionItemIndex[m_nCurrentSelectedItem], m_nNasTotal[m_nCurrentSelectedItem]);
	_pNetwork->SendTradeAgentBuyReq(m_AuctionItemData[m_nCurrentSelectedItem].AuctionItemIndex,
									m_AuctionItemData[m_nCurrentSelectedItem].llNasTotal);
}

// ----------------------------------------------------------------------------
// Name : CancelRegister()
// Desc : 등록 취소 요청
// ----------------------------------------------------------------------------
void CUIAuction::CancelRegister()
{
	if(m_nSelectedItemCancel < 0)
	{
		return;
	}

	_pNetwork->SendTradeAgentRegCancelReq(m_AuctionItemData[m_nSelectedItemCancel].AuctionItemIndex);
}


// ----------------------------------------------------------------------------
// Name : RegisterItem()
// Desc : 아이템 등록 요청
// ----------------------------------------------------------------------------
void CUIAuction::RegisterItem()
{
	m_bRegister = TRUE; // 등록 요청 상태
	_pNetwork->SendTradeAgentRegReq(m_btnRegisterItem.GetItemTab(), m_btnRegisterItem.GetItemRow(), m_btnRegisterItem.GetItemCol(), 
									m_btnRegisterItem.GetItemIndex(), m_nRegisterItemCount, m_nRegisterItemPrice, m_nDepositMoney);
}

// ----------------------------------------------------------------------------
// Name : CanRegister()
// Desc : 현재 드래그&드롭 된 아이템이 등록할 수 있는 아이템인지 확인 (아이템 속성에 exchange가 체크된 아이템만 거래대행 가능)
// ----------------------------------------------------------------------------
BOOL CUIAuction::CanRegister()
{
	if (m_bRegister)
	{
		_pNetwork->ClientSystemMessage(_S(4387, "등록요청중에는 할 수 없습니다."), SYSMSG_ERROR);
		return FALSE;
	}

	if(_pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
	_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY)
	{
		SBYTE sbWearType = _pUIMgr->GetHoldBtn().GetItemWearType();
		
		CItemData&	rItemData = _pNetwork->GetItemData( _pUIMgr->GetHoldBtn().GetItemIndex() );

		
		//나스는 등록 불가
		if((rItemData.GetType() == CItemData::ITEM_ETC && 
			rItemData.GetSubType() == CItemData::ITEM_ETC_MONEY))
		{
			return FALSE;
		}

		CItems	&rItems = _pNetwork->MySlotItem[_pUIMgr->GetHoldBtn().GetItemTab()][_pUIMgr->GetHoldBtn().GetItemRow()][_pUIMgr->GetHoldBtn().GetItemCol()];
		
		//착용한 아이템이 아니면서, 대여, 플래티늄 부스터도 아니어야 한다
		if( (sbWearType < 0) &&
			(rItems.ItemData.GetFlag() & ITEM_FLAG_EXCHANGE) && 
			!(rItems.IsFlag(FLAG_ITEM_LENT)) && 
			!(rItems.IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED)) )
		{
			return TRUE;
		}
	}
								
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : GetCurrentAuctionItemCount()
// Desc : 현재 페이지에 표시되어있는 아이템 개수를 반환
// ----------------------------------------------------------------------------
int CUIAuction::GetCurrentAuctionItemCount()
{
	return m_AuctionItemData.Count();
}


// ----------------------------------------------------------------------------
// Name : SortItem(int nSortType, BOOL bReverse /* = FALSE */)
// Desc : 정렬 방식을 지정하여 요청(정렬 할종류, 방식)
// ----------------------------------------------------------------------------
void CUIAuction::SortItem(int nSortType, BOOL bReverse /* = FALSE */)
{

	int nAlignType = (nSortType * 2) + (bReverse ? 1 : 0);

	m_nCurretnSorting = nAlignType;

	switch(m_nCurrnetTab)
	{
		case AUCTION_TAB_REFER:
			{
				if(IsSearchConditionChanged())
				{
					m_cmbItemType.SetCurSel(m_nSearchedItemType);
					m_cmbItemSubType.SetCurSel(m_nSearchedItemSubType);
					m_cmbClass.SetCurSel(m_nSearchedClass);
					m_ebSearch.ResetString();
					m_ebSearch.SetString(m_strSearched.str_String);

					SearchItem(m_nCurrentPage, nAlignType);
				}
				else
				{
					SearchItem(m_nCurrentPage, nAlignType);
				}
			}
			break;
		case AUCTION_TAB_REGISTER:
			{
				_pNetwork->SendTradeAgentRegListReq(m_nCurrentPage, nAlignType);
			}
			break;
		case AUCTION_TAB_SETTLEMENT:
			{
				_pNetwork->SendTradeAgentCalcListReq(m_nCurrentPage, nAlignType);
			}
			break;
	}
}

// ----------------------------------------------------------------------------
// Name : EnableSortBtn()
// Desc : 정렬버튼 비활성화(리스트에 아이템이 없을경우에 비활성화 시킨다)
// ----------------------------------------------------------------------------
void CUIAuction::EnableSortBtn()
{
	m_btnSortItem.SetEnable(TRUE);
	m_btnSortCount.SetEnable(TRUE);
	m_btnSortTotalNas.SetEnable(TRUE);
	m_btnSortLevel.SetEnable(TRUE);

	m_btnFirst.SetEnable(TRUE);
	m_btnPrev.SetEnable(TRUE);
	m_btnNext.SetEnable(TRUE);
	m_btnLast.SetEnable(TRUE);

	m_btnSearch.SetEnable(TRUE);
}

// ----------------------------------------------------------------------------
//  [6/11/2009 selo]
// Name : EnableSearchBtn()
// Desc : 검색 버튼 활성화
// ----------------------------------------------------------------------------
void CUIAuction::EnableSearchBtn()
{
	m_btnSearch.SetEnable(TRUE);
}

// ----------------------------------------------------------------------------
// Name : SetTab()
// Desc : 탭선택에 따른 거래대행 페이지 셋팅
// ----------------------------------------------------------------------------
void CUIAuction::SetTab(int nIndex)
{
	InitSlotItem();

	if (m_nCurrnetTab != nIndex)
	{
		m_nCurrnetTab = nIndex;
		m_nCurrentPage = 1;
		m_nMaxPage = 1;
		m_nCurretnSorting = -1;

		CloseAuctionMessageBox();

		if(m_nCurrnetTab == AUCTION_TAB_REFER)
		{
			m_btnSortItem.SetEnable(FALSE);
			m_btnSortCount.SetEnable(FALSE);
			m_btnSortTotalNas.SetEnable(FALSE);
			m_btnSortLevel.SetEnable(FALSE);

			m_btnFirst.SetEnable(FALSE);
			m_btnPrev.SetEnable(FALSE);
			m_btnNext.SetEnable(FALSE);
			m_btnLast.SetEnable(FALSE);

			m_btnSearch.SetEnable(TRUE);

			if( _pUIMgr->GetInventory()->IsVisible() )
			{
				_pUIMgr->GetInventory()->ToggleVisible();
			}
		}
		else if(m_nCurrnetTab == AUCTION_TAB_REGISTER)
		{
			m_btnSortItem.SetEnable(FALSE);
			m_btnSortCount.SetEnable(FALSE);
			m_btnSortTotalNas.SetEnable(FALSE);
			m_btnSortLevel.SetEnable(FALSE);

			m_btnFirst.SetEnable(TRUE);
			m_btnPrev.SetEnable(TRUE);
			m_btnNext.SetEnable(TRUE);
			m_btnLast.SetEnable(TRUE);
			if( !_pUIMgr->GetInventory()->IsVisible() )
			{
				_pUIMgr->GetInventory()->ToggleVisible();
			}

			_pUIMgr->RearrangeOrder( UI_INVENTORY, TRUE );
		}
		else if(m_nCurrnetTab == AUCTION_TAB_SETTLEMENT)
		{
			m_btnSortItem.SetEnable(FALSE);
			m_btnSortCount.SetEnable(FALSE);
			m_btnSortTotalNas.SetEnable(FALSE);
			m_btnSortLevel.SetEnable(FALSE);

			m_btnFirst.SetEnable(TRUE);
			m_btnPrev.SetEnable(TRUE);
			m_btnNext.SetEnable(TRUE);
			m_btnLast.SetEnable(TRUE);
			if( _pUIMgr->GetInventory()->IsVisible() )
			{
				_pUIMgr->GetInventory()->ToggleVisible();
			}
		}	
	}
}