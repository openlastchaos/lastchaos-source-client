#include "stdh.h"
#include <Engine/Interface/UiSelChar.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Rendering/Render.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UICreateChar.h>
#include <Engine/Sound/SoundLibrary.h>

// 임시 Define
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/World/WorldRayCasting.h>

#include <Engine/Ska/Mesh.h>

#include <Engine/JobInfo.h>
//wooss 051017
#include <Engine/Base/Input.h>
#include <Engine/LocalDefine.h>

extern INDEX g_iCountry;
extern BOOL g_bAutoLogin;
#define CHECK_NUM 7

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")
//#define EXSLOT // 슬롯 확장
// ----------------------------------------------------------------------------
// Name : CUISelChar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISelChar::CUISelChar() : 
m_iDestPosID(-1),
m_pWorld(NULL)
{	
	memset(&m_aEGslotPosID[0], NULL, sizeof(m_aEGslotPosID));

}

// ----------------------------------------------------------------------------
// Name : ~CUISelChar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelChar::~CUISelChar()
{	
	Destroy();
	
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcHP.SetRect( 0, 25, 0, 33 );
	m_rcMP.SetRect( 0, 39, 0, 47 );
	m_rcEXP.SetRect( 0, 53, 0, 61 );

	// Create select character
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\SelectCharacter.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtCharInfo.SetUV( 74, 0, 256, 68, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 2, 45, 4, 52, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 7, 45, 9, 52, fTexWidth, fTexHeight );
	m_rtEXP.SetUV( 12, 45, 14, 52, fTexWidth, fTexHeight );
	m_rtExSlot.SetUV( 93, 3, 106, 16,fTexWidth,fTexHeight);			//wooss 050819
	m_rtExSlotLine.SetUV( 74, 66, 255, 67,fTexWidth,fTexHeight);	//wooss 050819
	m_rtExSlotLineMidV.SetUV( 74, 63, 255, 65,fTexWidth,fTexHeight);//wooss 050819
//	m_rtExSlotLineMidH.SetUV( 93, 3, 106, 17,fTexWidth,fTexHeight);	//wooss 050819
//	m_rtExSlotLineBtm.SetUV( 93, 3, 106, 17,fTexWidth,fTexHeight);	//wooss 050819


	// OK Button(Connect)
	m_btnOK.Create( this, _S( 239, "게임 시작" ), 0, 0, 100, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// Create Character Button
	m_btnCreateChar.Create( this, _S( 140, "캐릭터 생성" ), 120, 0, 100, 21 );
	m_btnCreateChar.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnCreateChar.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnCreateChar.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCreateChar.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Delete Character Button
	m_btnDeleteChar.Create( this, _S( 2902, "삭제/취소" ), 240, 0, 100, 21 );
	m_btnDeleteChar.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnDeleteChar.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnDeleteChar.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDeleteChar.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Options Button
	m_btnOptions.Create( this, _S( 196, "옵션" ), 360, 0, 63, 21 );
	m_btnOptions.SetUV( UBS_IDLE, 0, 0, 63, 21, fTexWidth, fTexHeight );
	m_btnOptions.SetUV( UBS_CLICK, 0, 23, 63, 44, fTexWidth, fTexHeight );
	m_btnOptions.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOptions.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Exit Button
	m_btnExit.Create( this, _S( 181, "나가기" ),  440, 0, 63, 21 );
	m_btnExit.SetUV( UBS_IDLE, 0, 0, 63, 21, fTexWidth, fTexHeight );
	m_btnExit.SetUV( UBS_CLICK, 0, 23, 63, 44, fTexWidth, fTexHeight );
	m_btnExit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExit.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Delete Time
	//RECT
	m_rcDelTime.SetRect( 0, 0, 181, 37 );
	// UV
	m_rtDelTime.SetUV( 74, 68, 255, 105, fTexWidth, fTexHeight );

	// set random seed 
	srand((unsigned int)time((time_t *)NULL));

	// init slot ani
	for(int i=0;i<MAX_SLOT;i++)
	{
		m_aSlotAni[i].m_idAni = ANIM_IDLE;
		m_aSlotAni[i].m_startTime = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - 520, pixMaxJ - 38 );

	// Region of character information
	m_rcCharInfo.SetRect( pixMaxI - 182, 0, pixMaxI, 68 );
	m_rcExSlot.SetRect(pixMaxI - 178, 70, pixMaxI-3, 120);		//wooss 050819
	m_rcExSlotLineTop.SetRect(pixMaxI - 182,68,pixMaxI,69);
	m_rcExSlotLineMidV.SetRect(pixMaxI - 182,69,pixMaxI,124);		
	m_rcExSlotLineMidH.SetRect(pixMaxI - 182,85,pixMaxI,86);		
	m_rcExSlotLineBtm.SetRect(pixMaxI - 182,124,pixMaxI,125);		

//	m_rcDelTime.SetRect(pixMaxI - 182,68,pixMaxI,115);		

	m_rcHP.Left = m_rcMP.Left = m_rcEXP.Left = pixMaxI - 171;
	UpdateStatus();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//-----------------------------------------------------------------------------
// Purpose: Reset
//-----------------------------------------------------------------------------
void CUISelChar::Reset()
{
}

// ----------------------------------------------------------------------------
// Name : UpdateStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::UpdateStatus()
{
	INDEX			iSlot = _pGameState->SelectedSlot() - 1;
	if(iSlot < 0 || iSlot >= MAX_SLOT) return;
	CGameState::SLOT_INFO	&CurrentSlot = _pGameState->m_SlotInfo[iSlot];

	FLOAT	fHPRatio = CurrentSlot.hp / (FLOAT)(CurrentSlot.maxHP );
	if( fHPRatio > 1.0f ) fHPRatio = 1.0f;
	FLOAT	fMPRatio = CurrentSlot.mp / (FLOAT)( CurrentSlot.maxMP );
	if( fMPRatio > 1.0f ) fMPRatio = 1.0f;
	FLOAT	fEXPRatio = CurrentSlot.curExp / (FLOAT)( CurrentSlot.needExp );
	FLOAT	fEXPBarRatio = fEXPRatio;
	if( fEXPBarRatio > 1.0f ) fEXPBarRatio = 1.0f;
	else if( fEXPBarRatio < 0.0f ) fEXPBarRatio = 0.0f;
	FLOAT	fSPRatio = ( CurrentSlot.sp % 10000 ) / 10000.0f;

	m_rcHP.Right = m_rcHP.Left + SELCHAR_BAR_WIDTH * fHPRatio;
	m_rcMP.Right = m_rcMP.Left + SELCHAR_BAR_WIDTH * fMPRatio;
	m_rcEXP.Right = m_rcEXP.Left + SELCHAR_BAR_WIDTH * fEXPBarRatio;

	m_strName.PrintF( "%s", CurrentSlot.name );
	m_strLevel.PrintF( "%d", CurrentSlot.level );
	m_strHP.PrintF( "%d/%d", CurrentSlot.hp, CurrentSlot.maxHP );
	m_strMP.PrintF( "%d/%d", CurrentSlot.mp, CurrentSlot.maxMP );
	m_strEXP.PrintF( "%.2f%%", fEXPRatio * 100.0f );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::Render()
{
	if(m_pWorld)
	{		
		CDrawPort	*pdp = _pUIMgr->GetDrawPort();

		// Set select character texture
		pdp->InitTextureData( m_ptdBaseTexture );

		// Character information
		// Background
		_pUIMgr->GetDrawPort()->AddTexture( m_rcCharInfo.Left, m_rcCharInfo.Top, m_rcCharInfo.Right, m_rcCharInfo.Bottom,
											m_rtCharInfo.U0, m_rtCharInfo.V0, m_rtCharInfo.U1, m_rtCharInfo.V1,
											0xFFFFFFFF );
#ifdef EXSLOT
		//wooss 050819
		// Extend Slot
		// -------------확장 슬롯 (유료아이템)------------------------------->>
		// ------------------------------------------------------------------>>
		_pUIMgr->GetDrawPort()->AddTexture( m_rcExSlot.Left, m_rcExSlot.Top, m_rcExSlot.Right, m_rcExSlot.Bottom,
											m_rtExSlot.U0, m_rtExSlot.V0, m_rtExSlot.U1, m_rtExSlot.V1,
											0xFFFFFFFF );
		// Slot mid Vertical line
		_pUIMgr->GetDrawPort()->AddTexture( m_rcExSlotLineMidV.Left, m_rcExSlotLineMidV.Top, m_rcExSlotLineMidV.Right, m_rcExSlotLineMidV.Bottom,
											m_rtExSlotLineMidV.U0, m_rtExSlotLineMidV.V0, m_rtExSlotLineMidV.U1, m_rtExSlotLineMidV.V1,
											0xFFFFFFFF );
		// Slot top line
		_pUIMgr->GetDrawPort()->AddTexture( m_rcExSlotLineTop.Left, m_rcExSlotLineTop.Top, m_rcExSlotLineTop.Right, m_rcExSlotLineTop.Bottom,
											m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
											0xFFFFFFFF );
		// slot middle line
		_pUIMgr->GetDrawPort()->AddTexture( m_rcExSlotLineMidH.Left, m_rcExSlotLineMidH.Top, m_rcExSlotLineMidH.Right, m_rcExSlotLineMidH.Bottom,
											m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
											0xFFFFFFFF );
		// slot bottom line
		_pUIMgr->GetDrawPort()->AddTexture( m_rcExSlotLineBtm.Left, m_rcExSlotLineBtm.Top, m_rcExSlotLineBtm.Right, m_rcExSlotLineBtm.Bottom,
											m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
											0xFFFFFFFF );
		// ------------------------------------------------------------------<<
#endif
		
		// 이동 wooss 050819
		// Text in Character information
		int	nSX = m_rcCharInfo.Left;
		int	nSY = m_rcCharInfo.Top;

#ifdef EXSLOT		
		// -------------확장 슬롯 (유료아이템)------------------------------->>
		// ------------------------------------------------------------------>>
		CTString slotStr1,slotStr2;
		slotStr1.PrintF(_S( 2114, "확장슬롯 1 : %5d" ),m_exSlot1);   
		slotStr2.PrintF(_S( 2115, "확장슬롯 2 : %5d" ),m_exSlot2);

		_pUIMgr->GetDrawPort()->PutTextExCX( CTString("확장슬롯 남은 기간"), nSX + SELCHAR_EX_SLOT_X1,nSY + SELCHAR_EX_SLOT_Y1 );
		_pUIMgr->GetDrawPort()->PutTextExCX( slotStr1, nSX + SELCHAR_EX_SLOT_X2, nSY + SELCHAR_EX_SLOT_Y2 );
		_pUIMgr->GetDrawPort()->PutTextExCX( slotStr2, nSX + SELCHAR_EX_SLOT_X2, nSY + SELCHAR_EX_SLOT_Y3 );
		// ------------------------------------------------------------------<<
#endif
		if( _pGameState->SelectedSlot() > 0 )
		{
			// HP
			_pUIMgr->GetDrawPort()->AddTexture( m_rcHP.Left, m_rcHP.Top, m_rcHP.Right, m_rcHP.Bottom,
												m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
												0xFFFFFFFF );
			// MP
			_pUIMgr->GetDrawPort()->AddTexture( m_rcMP.Left, m_rcMP.Top, m_rcMP.Right, m_rcMP.Bottom,
												m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
												0xFFFFFFFF );
			// EXP
			_pUIMgr->GetDrawPort()->AddTexture( m_rcEXP.Left, m_rcEXP.Top, m_rcEXP.Right, m_rcEXP.Bottom,
												m_rtEXP.U0, m_rtEXP.V0, m_rtEXP.U1, m_rtEXP.V1,
												0xFFFFFFFF );

			// Text in Character information
			
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strLevel, nSX + SELCHAR_LEVEL_CX,
													nSY + SELCHAR_NAME_SY, 0xFFD3A7FF );
			_pUIMgr->GetDrawPort()->PutTextEx( m_strName, nSX + SELCHAR_NAME_SX, nSY + SELCHAR_NAME_SY );
			_pUIMgr->GetDrawPort()->PutTextEx( CTString( "HP" ), nSX + SELCHAR_HP_SX,
													nSY + SELCHAR_HP_SY );
			_pUIMgr->GetDrawPort()->PutTextEx( CTString( "MP" ), nSX + SELCHAR_HP_SX,
													nSY + SELCHAR_MP_SY );
			_pUIMgr->GetDrawPort()->PutTextEx( CTString( "EXP" ), nSX + SELCHAR_HP_SX,
													nSY + SELCHAR_EXP_SY );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strHP, nSX + SELCHAR_HP_CX,
													nSY + SELCHAR_HP_SY );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strMP, nSX + SELCHAR_HP_CX,
													nSY + SELCHAR_MP_SY );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strEXP, nSX + SELCHAR_HP_CX,
													nSY + SELCHAR_EXP_SY );
#ifdef	NEW_DELETE_CHAR										
			SLONG tv_time = _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time;
			if( tv_time  > 0 ) {
				CTString str_time,str_time2;
			//	time_t tv_time2;
				int tv_hour,tv_min;
			//	time(&tv_time2); 
			//	tv_time -= tv_time2;
				tv_hour = tv_time / (60*60);
				tv_min = (tv_time %(60*60)) /60;
					
				str_time.PrintF(_S( 2903, "삭제대기 남은시간"));
				_pUIMgr->GetDrawPort()->PutTextEx(str_time, 37,	3 );
			
				str_time.Clear();
				if(tv_hour >0 )	str_time.PrintF(_S( 2512, "%d시간"),tv_hour);
				if(tv_min > 0 ) {
					str_time2.PrintF(_S( 2513, "%d분"),tv_min);
					str_time += str_time2;
				}
				int tv_posX = (m_rcDelTime.Right - m_rcDelTime.Left)/2;
				_pUIMgr->GetDrawPort()->PutTextExCX(str_time, tv_posX , 21 ,0xFFAA44FF);

				_pUIMgr->GetDrawPort()->AddTexture( m_rcDelTime.Left, m_rcDelTime.Top, m_rcDelTime.Right, m_rcDelTime.Bottom,
												m_rtDelTime.U0, m_rtDelTime.V0, m_rtDelTime.U1, m_rtDelTime.V1,
												0xFFFFFFFF );
				
			}
#endif
		}



		
		// OK Button(Connect)
		m_btnOK.Render();
		
		// Create Character Button
		m_btnCreateChar.Render();
		
		// Delete Character Button
		m_btnDeleteChar.Render();
		
		// Option Button
		m_btnOptions.Render();
		
		// Exit Button
		m_btnExit.Render();

		// Render all elements
		pdp->FlushRenderingQueue();

		// Flush all render text queue
		pdp->EndTextEx();
	}

		
#ifdef NEW_DELETE_CHAR
		// Check ANI
		// 선택된 캐릭터에 idle Animation을 제거함.
		for(int j = 0; j < MAX_SLOT; j++)
		{
			CEntity *pEntity = _pGameState->m_pEntModels[j];
			if(pEntity)
			{
				int iJob		= _pGameState->m_SlotInfo[j].job;
				INDEX idAni		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, m_aSlotAni[j].m_idAni ) );
				CModelInstance* pMI =  pEntity->GetModelInstance();

				if(pMI)
				{
					if(	pMI->IsAnimationPlaying(idAni)	&& m_aSlotAni[j].m_idAni != ANIM_IDLE)
					{			
						if(_pTimer->CurrentTick() - m_aSlotAni[j].m_startTime	
							>= (pMI->GetAnimLength(idAni) + 0.3f ) )
						{
							pMI->NewClearState(CLEAR_STATE_LENGTH);
							INDEX idWalk		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
							m_aSlotAni[j].m_idAni		= ANIM_IDLE;	
							pMI->AddAnimation(idWalk,AN_LOOPING|AN_NORESTART,1,0);

							// put on - weapon ----------------------------------------------------<<
							int nItemSubType;
							int nItemIndex = -1;
#ifdef HEAD_CHANGE
							// 무기 착용 확인 
							SLONG iWear		=	_pGameState->m_SlotInfo[j].wear[2]; // 무기 
							SLONG iWear3	=	_pGameState->m_SlotInfo[j].wear[4];	// 방패 
#else
							// 무기 착용 확인 
							SLONG iWear		=	_pGameState->m_SlotInfo[j].wear[1]; // 무기 
							SLONG iWear3	=	_pGameState->m_SlotInfo[j].wear[3];	// 방패 
#endif
							if(iWear > 0)
							{				
								CItemData &ID = _pNetwork->GetItemData(iWear);
								nItemIndex = ID.GetItemIndex();
								nItemSubType = ID.GetSubType();
								_pGameState->WearingArmor(pMI, ID);
								_pGameState->m_SlotInfo[j].itemEffect.Change(
								_pGameState->m_SlotInfo[j].job
								, &ID
								, wearTypeTable[1]
								, _pGameState->m_SlotInfo[j].itemPlus[1]
								, &pMI->m_tmSkaTagManager
								, 1, nItemSubType );
								
							}
							if(iWear3 > 0)	
							{
								CItemData &ID = _pNetwork->GetItemData(iWear3);
								_pGameState->WearingArmor(pMI, ID);
							}
							// ---------------------------------------------------------------------------->>
						}
					}

				}
			}
		}

#endif 
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :burn
// ----------------------------------------------------------------------------
WMSG_RESULT CUISelChar::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	CTString	strMessage;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCreateChar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnDeleteChar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnOptions.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				//-- Down 동작을 표현하기위해
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnCreateChar.MouseMessage( pMsg ) != WMSG_FAIL ) 
				{
					// Nothing
				}
				else if( m_btnDeleteChar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnOptions.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_SEL_CHAR, TRUE );		
				return WMSG_SUCCESS;
			}
			else
				SelectCharacter(nX, nY);
		}
		break;
		
	case WM_LBUTTONDBLCLK:
		{
			SelectCharacter(nX, nY);
		
			if( _pGameState->GetGameMode() != CGameState::GM_NETWORK )
			{
				if(_pGameState->SelectedSlot() > 0)
				{

#ifdef NEW_DELETE_CHAR
					if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) break;
#endif
					// wooss 050822 
					// 확장된 슬롯을 선택 했다면 남은 기간을 체크한다
					int tv_t1,tv_t2;
					GetExSlotTime(0,&tv_t1,&tv_t2);
					if(_pGameState->SelectedSlot()==5)
					{
						if(tv_t1<=0)
						{
							_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( CTString( "확장슬롯 1 의 이용 기간이 지났습니다"));
							_pUIMgr->CreateMessageBox( MsgBoxInfo );
							return WMSG_FAIL;
						}

					}
					else if(_pGameState->SelectedSlot()==6)
					{
						if(tv_t2<=0)
						{
							_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( CTString( "확장슬롯 2 의 이용 기간이 지났습니다"));
							_pUIMgr->CreateMessageBox( MsgBoxInfo );
							return WMSG_FAIL;

						}
					}
					StartGame();					
				}
			}
		}
		break;
		
	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{			
				if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND && _pGameState->GetGameMode() != CGameState::GM_NETWORK)
					{
						if(_pGameState->SelectedSlot() > 0)
						{
							// wooss 050822 
							// 확장된 슬롯을 선택 했다면 남은 기간을 체크한다
							int tv_t1,tv_t2;
							GetExSlotTime(0,&tv_t1,&tv_t2);
							if(_pGameState->SelectedSlot()==5)
							{
								if(tv_t1<=0)
								{
									_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( CTString( "확장슬롯 1 의 이용 기간이 지났습니다"));
									_pUIMgr->CreateMessageBox( MsgBoxInfo );
									return WMSG_FAIL;
								}

							}
							else if(_pGameState->SelectedSlot()==6)
							{
								if(tv_t2<=0)
								{
									_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( CTString( "확장슬롯 2 의 이용 기간이 지났습니다"));
									_pUIMgr->CreateMessageBox( MsgBoxInfo );
									return WMSG_FAIL;

								}

							}
							// 게임을 시작함.
							//_pSound->Mute();
							StartGame();
							//_pSound->UpdateSounds();
						}
						else
						{
							_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 241, "캐릭터 선택 오류" ), UMBS_OK,
														UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( _S( 242, "캐릭터를 선택하지 않으셨습니다." ) );
							_pUIMgr->CreateMessageBox( MsgBoxInfo );
						}
					}
				}
				else if( ( wmsgResult = m_btnCreateChar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND)
					{
						if(_pGameState->GetExistCharNum() >= MAX_SLOT)
						{
							_pGameState->DisplayErrorMessage( MSG_FAIL_DB_FULL_CHAR, UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR);
						}
						else
						{
							_pGameState->SetCameraByJob();

							// 슬롯에 몇개의 공간이 있는지 확인해서,
							// 꽉 차 있으면 더이상 생성할수 없음을 나타내줌.
							_pUIMgr->SetUIGameState(UGS_CREATECHAR);
					
#ifdef NEW_DELETE_CHAR
							// init slot effect 
							for(int i=0;i<MAX_SLOT;i++)
								DestroyEffectGroupIfValid(m_aEGslotPosID[i]);

#endif
						}
					}
				}
				else if( ( wmsgResult = m_btnDeleteChar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND)
					{
						if(_pGameState->SelectedSlot() > 0)
						{
							BOOL tv_chk = FALSE;
							int tv_line;

							// FIXME : 캐릭터 삭제 메세지 정의할것.
							_pUIMgr->CloseMessageBox( MSGCMD_DELETE_CHARACTER );
							
							CUIMsgBox_Info	MsgBoxInfo;
							if(g_iCountry == TAIWAN2 || g_iCountry == THAILAND ){
								if( atoi(m_strLevel) >= 15 )
								{					
									_pUIMgr->CloseMessageBox( MSGCMD_NULL );
									
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_OK,
																UI_NONE, MSGCMD_NULL);
									MsgBoxInfo.AddString( _S( 1856, "레벨 15이상의 캐릭터는 삭제할 수 없습니다"  ) );
									_pUIMgr->CreateMessageBox( MsgBoxInfo );
									break;	
								}
							}
							if(g_iCountry == KOREA && !g_bAutoLogin) {
								MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_OK | UMBS_INPUTPASSWORD,
														UI_SEL_CHAR, MSGCMD_DELETE_CHARACTER_SECURITY );
								
								
#ifdef NEW_DELETE_CHAR
								if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) tv_chk = TRUE;
								if( !tv_chk ){
									MsgBoxInfo.AddString( _S( 2904, "캐릭터 삭제는 삭제요청 후 24시간이 지나야 삭제가 됩니다. 단, 시간안에 취소 가능" ) );
									MsgBoxInfo.AddString( CTString( " " ) );
									tv_line = 4;
								}
								else {
									MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
									tv_line = 2;
									
								}
#else
								{
									MsgBoxInfo.AddString( _S( 1724, "계정에 등록된 주민등록 번호 뒷자리 7자리를 입력하여 주십시오." ) );
									MsgBoxInfo.AddString( CTString( " " ) );
									MsgBoxInfo.AddString( CTString( " " ) );
									tv_line = 4;

								}
#endif
								MsgBoxInfo.AddString( CTString( " " ) );

								MsgBoxInfo.AddStringEx( _S( 2906, "주민번호"),tv_line,1,0xFF9156FF);
								MsgBoxInfo.SetInputBox(tv_line,10,7);
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
								_pUIMgr->Lock(TRUE);
							}
							else{
								MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_YESNO,
														UI_SEL_CHAR, MSGCMD_DELETE_CHARACTER );
#ifdef NEW_DELETE_CHAR
								BOOL tv_chk = FALSE;
								if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) tv_chk = TRUE;
								if( !tv_chk ){
									MsgBoxInfo.AddString( _S( 2904, "캐릭터 삭제는 삭제요청 후 24시간이 지나야 삭제가 됩니다. 단, 시간안에 취소 가능" ) );
									MsgBoxInfo.AddString( CTString( " " ) );
									tv_line = 4;
								}
								else {
									MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
									tv_line = 2;
									
								}
#else								
								MsgBoxInfo.AddString( _S( 243, "캐릭터를 삭제 하시겠습니까?" ) );
#endif
								_pUIMgr->CreateMessageBox( MsgBoxInfo );
							}
						}
						else
						{
							_pUIMgr->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 241, "캐릭터 선택 오류" ), UMBS_OK,
														UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( _S( 242, "캐릭터를 선택하지 않으셨습니다." ) );
							_pUIMgr->CreateMessageBox( MsgBoxInfo );
						}
					}
				}
				else if( ( wmsgResult = m_btnOptions.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						_pUIMgr->GetOption()->OpenOption();
				}
				else if( ( wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						

// wooss 060209 TEST JAPAN 
//#define AUTO_LOGIN
//#ifdef AUTO_LOGIN
						extern BOOL g_bAutoLogin;
						if(g_bAutoLogin)
						{
						
							extern CTString g_nmID;
							extern CTString g_nmPW;

							_pNetwork->m_strUserID = g_nmID;
							_pNetwork->m_strUserPW = g_nmPW;
							_pUIMgr->GetSelServer()->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
						
							const int	iCameraEntityID	= 1628;
							CEntity		*pCameraEntity = NULL;
							CEntity		*pEntity = NULL;
							BOOL bExist = _pNetwork->ga_World.EntityExists( iCameraEntityID, pCameraEntity );
							if(bExist)
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
							}

							_pUIMgr->SetUIGameState( UGS_SELSERVER );	
												
						}
//#else
						else
						{
							
							_pSound->Mute();
							// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
							//_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
							_cmiComm.Disconnect();

							_pUIMgr->SetUIGameState(UGS_LOGIN);
							// 기존에 월드에 있던 엔티티를 제거함.
							for(int i = 0; i < MAX_SLOT; ++i)
							{
								CEntity *pEntity = _pGameState->m_pEntModels[i];
								if(pEntity)
								{
									pEntity->en_RenderType = CEntity::RT_SKAEDITORMODEL;
								}
							}
							_pGameState->SelectedSlot() = 0; //슬롯 선택 커서 초기화.
							
							_pGameState->BackToLogin();
						}
//#endif
						
//#define RESTRICT_SOUND 	
#ifdef RESTRICT_SOUND
						extern BOOL _bNTKernel;
						//if( _bNTKernel )

							_pSound->UpdateSounds();
						#endif

#ifdef NEW_DELETE_CHAR
						// init slot effect 
						for(int i=0;i<MAX_SLOT;i++)
							DestroyEffectGroupIfValid(m_aEGslotPosID[i]);

#endif
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUISelChar::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( bOK )
	{
		switch( nCommandCode )
		{
			
			case MSGCMD_DELETE_CHARACTER:
				_pSound->Mute();
				DeleteCharacter();
			#ifdef RESTRICT_SOUND
				extern BOOL _bNTKernel;
				//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
				
				break;
			
			case MSGCMD_DELETE_CHARACTER_SECURITY:
				_pSound->Mute();
				DeleteCharacter(strInput);
			#ifdef RESTRICT_SOUND
				extern BOOL _bNTKernel;
				//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
				
				break;
			
		}

	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CUISelChar::ShowCharacter()
{
	if(!m_pWorld) return FALSE;

	// 받아들인 정보를 바탕으로...
	// 엔티티를 생성하고, ModelInstance를 생성하고...
#ifdef EXSLOT
#undef	MAX_SLOT
#define 	MAX_SLOT	6 	
#endif
	int aiMarker[MAX_SLOT] =
	{
#ifdef EXSLOT
		3377, 3378, 3379, 3380, 3953, 3954		//wooss 050819 add new slot maker index 3953,3954
#else 
		3377, 3378, 3379, 3380, // 3953, 3954
#endif
	};	

//강동민 수정 시작 클로즈 2차 작업	08.18
	for(int i = 0; i < MAX_SLOT; ++i)
	{
		CEntity *pEntity		= m_pWorld->EntityFromID(aiMarker[i]);
		if(pEntity)
		{
		//	pEntity->SetFlagOn(ENF_HIDDEN);
			pEntity->en_RenderType = CEntity::RT_SKAEDITORMODEL;
		}
	}
//강동민 수정 끝 클로즈 2차 작업		08.18

	int iSlot = 0;
	while( iSlot <MAX_SLOT && _pGameState->m_SlotInfo[iSlot].bActive == TRUE )
	{	
		CEntity *penMarker					= m_pWorld->EntityFromID(aiMarker[iSlot]);
		if( penMarker == NULL )
			return TRUE;
		//CPlacement3D plMarker	= penMarker->GetLerpedPlacement();		
		_pGameState->m_pEntModels[iSlot]	= penMarker;
		penMarker->en_RenderType			= CEntity::RT_SKAMODEL;
		penMarker->en_ulCollisionFlags |= ECF_TESTMASK;
		penMarker->ModelChangeNotify();

		const int iSlotJob = _pGameState->m_SlotInfo[iSlot].job;

		//penEntity->InitAsSkaModel();				
		penMarker->SetSkaModel(JobInfo().GetFileName(iSlotJob));
		CModelInstance* pMI = penMarker->GetModelInstance();
		ASSERT(pMI != NULL && "Invalid Model Instance!");
		if(pMI)
		{
			pMI->mi_iFaceMesh	= -1;
			//pMI->mi_iFaceTexture= -1;
			pMI->mi_iHairMesh	= -1;
			//pMI->mi_iHairTexture= -1;
		}

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, iSlotJob, _pGameState->m_SlotInfo[iSlot].hairstyle, _pGameState->m_SlotInfo[iSlot].facestyle);
		CPrintF("Slot : %d, Hair : %d, Face : %d\n", iSlot, _pGameState->m_SlotInfo[iSlot].hairstyle, _pGameState->m_SlotInfo[iSlot].facestyle);

		
		if( _pNetwork->wo_iNumOfItem > 0 )
		{
			// 슬롯의 캐릭터에 장비를 장착한 모습을 보여줘야 하는 부분...
			// 상의 하의 무기 방패 장갑 신발 특수
			for(int i = 0; i < WEAR_COUNT ; ++i)
			{
//				if( i == 1) continue;	// 무기
//				if( i == 3) continue;	// 방어구
				
				int nItemSubType;
				int nItemIndex = -1;

				const SLONG iWear = _pGameState->m_SlotInfo[iSlot].wear[i];
				CItemData &ID = _pNetwork->GetItemData(iWear);
				
				if(iWear > 0)
				{
#ifdef HEAD_CHANGE
					if(ID.GetWearingPosition() != WEAR_HELMET || (CTString)ID.GetItemSmcFileName() != MODEL_TREASURE)
					{
						nItemIndex = ID.GetItemIndex();
						nItemSubType = ID.GetSubType();
						_pGameState->DeleteDefaultArmor(pMI, ID.GetWearingPosition(), iSlotJob);
						_pGameState->WearingArmor(pMI, ID);
					}
#else
					nItemIndex = ID.GetItemIndex();
					nItemSubType = ID.GetSubType();
					_pGameState->DeleteDefaultArmor(pMI, ID.GetWearingPosition(), iSlotJob);
					_pGameState->WearingArmor(pMI, ID);
#endif
				}
				_pGameState->m_SlotInfo[iSlot].itemEffect.Change(
					_pGameState->m_SlotInfo[iSlot].job
					, &ID
					, wearTypeTable[i]
					, _pGameState->m_SlotInfo[iSlot].itemPlus[i]
					, &pMI->m_tmSkaTagManager
					, 1, nItemSubType );
			}
		}
		
		if(pMI)
		{
			const int iJob		= _pGameState->m_SlotInfo[iSlot].job;
			INDEX idIdle		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
			pMI->AddAnimation(idIdle,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);

#ifdef NEW_DELETE_CHAR		
			// 060608
			if(_pGameState->m_SlotInfo[iSlot].m_time >0){
				pMI->m_tmSkaTagManager.SetOwner(_pGameState->m_pEntModels[iSlot]);
				if(!CEffectGroupManager::Instance().IsValidCreated(m_aEGslotPosID[iSlot]))
					m_aEGslotPosID[iSlot] = StartEffectGroup("STATE_DELETE",&pMI->m_tmSkaTagManager,_pTimer->GetLerpedCurrentTick());
			}
			else StopEffectGroupIfValid(m_aEGslotPosID[iSlot],0.1f);
#endif
		}

		iSlot++;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 선택된 캐릭터를 삭제합니다.
//-----------------------------------------------------------------------------
BOOL CUISelChar::DeleteCharacter()
{	
	CTString strMessage;
	if(_pGameState->SelectedSlot() > 0)
	{
		ULONG	ulDelIndex;
		ulDelIndex	= _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].index;
		
		if(_pNetwork->m_bSendMessage)
			return FALSE;
		
		BOOL tv_chk = FALSE;
#ifdef NEW_DELETE_CHAR
		if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0 ) tv_chk = TRUE;
#endif
		_pNetwork->SendDeleteCharacter(ulDelIndex,tv_chk);
		_pUIMgr->Lock(TRUE);
	}
	return FALSE;
}


//-----------------------------------------------------------------------------
// Purpose: 선택된 캐릭터를 삭제합니다. 2
//-----------------------------------------------------------------------------
BOOL CUISelChar::DeleteCharacter(CTString  secuNum)
{	
	CTString strMessage;
	if(secuNum.Length()!=CHECK_NUM) {
		_pUIMgr->CloseMessageBox(MSGCMD_NULL);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_OK,
									UI_SEL_CHAR, MSGCMD_NULL );
		MsgBoxInfo.AddString(_S(2340, "7자리 숫자를 입력하여 주십시오" ) );
									_pUIMgr->CreateMessageBox( MsgBoxInfo );
		_pUIMgr->Lock(FALSE);
		return FALSE;
	}
	if(_pGameState->SelectedSlot() > 0)
	{
		ULONG	ulDelIndex;
		ulDelIndex	= _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].index;
		
		if(_pNetwork->m_bSendMessage)
			return FALSE;
		BOOL tv_chk = FALSE;
		if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0 ) tv_chk = TRUE;

		_pNetwork->SendDeleteCharacter(ulDelIndex, secuNum, tv_chk );
		_pUIMgr->Lock(TRUE);

	
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUISelChar::StartGame()
{
	const int iSelectedSlot		= _pGameState->SelectedSlot()-1;
	ASSERT( iSelectedSlot >= 0 && iSelectedSlot <= (MAX_SLOT - 1) && "Invalid Selected Slot!!!" );
	if( iSelectedSlot < 0 || iSelectedSlot > (MAX_SLOT - 1) )
	{
		return;
	}

	ULONG	StartIndex;
	StartIndex	= _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].index;

	if(_pNetwork->m_bSendMessage)
		return;

	_pNetwork->SendGameStart(StartIndex);
	
	CGame* _pGame			= _pUIMgr->GetGame();
	CPlayerCharacter &pc	= _pGame->gm_apcPlayers[0];
	pc.pc_iPlayerType		= _pGameState->m_SlotInfo[iSelectedSlot].job;
	
	CPlayerEntity *penPlayerEntity	= NULL;
	CEntity* penPlEntity			= NULL;
	penPlEntity						= CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity					= (CPlayerEntity*) penPlEntity;	
	penPlayerEntity->CharacterChanged(pc);		
	_pUIMgr->Lock(TRUE);
	// wooss 051017 
	// 캐릭터 선택시 Input.cpp ReadKeyboardDevice::_abKeysDressed[KIP_MOUSE1] 에서 토글식으로 처리하여
	// 캐릭터를 선택하고 게임시작을 누르면 마우스 버튼이 누른 상태로 표시되어 게임을 시작하면 캐릭터가 마우스 포인터쪽으로 뛰어가게 된다.
	_pInput->SetLMousePressed(FALSE);

}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nX - 
//			nY - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
void CUISelChar::SelectCharacter(int nX, int nY)
{
	CEntity*		penPlEntity		= NULL;
	CPlayerEntity*	penPlayerEntity = NULL;
	penPlEntity						= CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity					= (CPlayerEntity*) penPlEntity;
	CPlacement3D plCamera			= penPlayerEntity->en_plLastViewpoint;
	CAnyProjection3D apr;
	penPlayerEntity->GetProjection(apr);	
	ASSERT( apr != NULL && "Invalid Projection3D!" );
	if( apr == NULL )
		return;
	apr->Prepare();
	
	// make the ray from viewer point through the mouse point, in current projection
	CPlacement3D plRay;
	apr->RayThroughPoint(
		FLOAT3D((float)nX, ( _pUIMgr->GetMaxJ() - _pUIMgr->GetMinJ() ) - (float)nY, 0.0f), 
		plRay);
	
	// construct cast ray for base world
	CCastRay crRay( NULL, plRay);
	crRay.cr_penHit						= NULL;
	crRay.cr_ttHitModels				= CCastRay::TT_NONE;     // only brushes block the damage
	crRay.cr_bHitTranslucentPortals		= FALSE;
	crRay.cr_bPhysical					= FALSE;
	
	for(int i = 0; i < MAX_SLOT; ++i)
	{
		if(!_pGameState->m_SlotInfo[i].bActive) continue;
		CEntity *pEntity = _pGameState->m_pEntModels[i];
		if(pEntity)
		{
		
			// 슬롯과 충돌했음.
			crRay.TestModelCollisionBox(pEntity);
			if(crRay.cr_penHit != NULL && (crRay.cr_penHit->en_RenderType == CEntity::RT_SKAMODEL))
			{
				int iSelectedSlot = _pGameState->SelectedSlot();

				// 현재 선택된 캐릭터가 변함이 없을때...
				if( iSelectedSlot != i + 1 )
				{

			/*		// 선택된 캐릭터에 idle Animation을 제거함.
					if(iSelectedSlot > 0)
					{
						CModelInstance* pMI		= _pGameState->m_pEntModels[iSelectedSlot - 1]->GetModelInstance();
						if(pMI)
						{
							const int iJob		= _pGameState->m_SlotInfo[iSelectedSlot - 1].job;
							INDEX idWalk		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
							pMI->AddAnimation(idWalk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
						}
					}
			*/	

					iSelectedSlot = i + 1;

					// 선택된 캐릭터에 idle Animation을 줍니다.
					_pGameState->SelectedSlot() = iSelectedSlot;
					{
						CModelInstance* pMI		= _pGameState->m_pEntModels[iSelectedSlot - 1]->GetModelInstance();
						if(pMI)
						{
							const int iJob		= _pGameState->m_SlotInfo[iSelectedSlot - 1].job;
							const BOOL bExtension = _pGameState->m_SlotInfo[iSelectedSlot - 1].bExtension;

							INDEX idAttack;
							int tv_flag =0;
#ifdef	NEW_DELETE_CHAR	
					//		if( pMI->IsNotAnimationPlaying()) return;
							if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time <=0 )
							{
								do{ 
									m_aSlotAni[iSelectedSlot - 1].m_idAni = rand()%( ANIM_SOCIAL_10 - ANIM_SOCIAL_0  +1) +ANIM_SOCIAL_0;
								}
								while (m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_2
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_5 
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_7 
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_9) ;

								tv_flag = NULL;							
							}
							else
							{
								m_aSlotAni[iSelectedSlot - 1].m_idAni = ANIM_SOCIAL_5;
								tv_flag = NULL;
							}
#else 
							{
								m_aSlotAni[iSelectedSlot - 1].m_idAni = ANIM_ATTACK_IDLE;
								tv_flag = AN_NORESTART|AN_CLEAR;

							}
#endif
							idAttack		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, m_aSlotAni[iSelectedSlot - 1].m_idAni ) );

							pMI->AddAnimation(idAttack,tv_flag,1,0);
#ifdef	NEW_DELETE_CHAR	
							// Check animation time
							m_aSlotAni[iSelectedSlot - 1].m_startTime = _pTimer->CurrentTick();
							
							// put off - weapon ----------------------------------------------------<<
	#ifdef HEAD_CHANGE
							// 무기 착용 확인 
							SLONG iWear		=	_pGameState->m_SlotInfo[i].wear[2]; // 무기 
							SLONG iWear3	=	_pGameState->m_SlotInfo[i].wear[4];	// 방패 
	#else
							// 무기 착용 확인 
							SLONG iWear		=	_pGameState->m_SlotInfo[i].wear[1]; // 무기 
							SLONG iWear3	=	_pGameState->m_SlotInfo[i].wear[3];	// 방패 
	#endif
							BOOL bTakeOffArmor = FALSE;
							
							if(iWear > 0 ){
								CItemData &ID = _pNetwork->GetItemData(iWear);
								_pGameState->TakeOffArmor(pMI, ID);
								_pGameState->m_SlotInfo[i].itemEffect.DeleteEffect(ID.GetWearingPosition());
								bTakeOffArmor = TRUE;
								
							}
							if(iWear3 > 3)	
							{
								CItemData &ID = _pNetwork->GetItemData(iWear3);
								_pGameState->TakeOffArmor(pMI, ID);
								bTakeOffArmor = TRUE;
							}

							if (bTakeOffArmor)
								_pGameState->m_SlotInfo[i].itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
							// ---------------------------------------------------------------------------->>
#endif																							
						}
					}

					// Update character status
					UpdateStatus();

#ifdef	NEW_DELETE_CHAR		
				
					if(_pGameState->m_SlotInfo[iSelectedSlot - 1].m_time>0)
					{	
						m_btnOK.SetEnable(FALSE);	
					}
					else{
						m_btnOK.SetEnable(TRUE);
					}
			
#endif
				}

				return;
			} // 충돌
		}
	}

	return;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUISelChar::Lock(BOOL bLock)
{
	m_btnCreateChar.SetEnable(!bLock);
	m_btnDeleteChar.SetEnable(!bLock);
	//m_btnOptions.SetEnable(!bLock);	
	m_btnOK.SetEnable(!bLock);
	//m_btnCancel.SetEnable(!bLock);
}


void CUISelChar::SetExSlotTime(int slotNum,int rTime)
{
	if(slotNum==1) m_exSlot1=rTime;
	else if(slotNum==2) m_exSlot2=rTime;
	
}

int	CUISelChar::GetExSlotTime(int slotNum,int* tSlot1,int* tSlot2)
{
	if(slotNum==1) return m_exSlot1;
	else if(slotNum==2) return m_exSlot2;
	else if(slotNum==0) 
	{
		*tSlot1=m_exSlot1;
		*tSlot2=m_exSlot2;
	}
	return 0;
}