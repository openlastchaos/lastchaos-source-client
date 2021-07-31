#include "stdh.h"
#include <Engine/Interface/UIParty.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>

// [7/13/2009 rumist] reject invite party.
extern INDEX g_iRejectParty; 
// ----------------------------------------------------------------------------
// Name : CUIParty()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIParty::CUIParty()
{
	Init();
}

// ----------------------------------------------------------------------------
// Name : ~CUIParty()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIParty::~CUIParty()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	//m_rcTitle.SetRect( 0, 0, 140, 22 );
	m_rcTitle.SetRect( 0, 0, 161, 15 );
	
	//int	nPosX = 10, nPosY = 39;
	int	nPosX = 32, nPosY = 27;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		//m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 5 );
		//m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 15 );
		m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 6 );
		m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 16 );
		nPosY += PARTY_STRETCH_HEIGHT;
	}
	
	//m_rcBuff.SetRect( 119, 3, 129, 13 );
	m_rcBuff.SetRect( 126, 3, 136, 13 );

	// Create party texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Party.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtTop.SetUV( 0, 0, 140, 22, fTexWidth, fTexHeight );
	//m_rtMiddle.SetUV( 0, 22, 140, 58, fTexWidth, fTexHeight );
	//m_rtBottom.SetUV( 0, 62, 140, 64, fTexWidth, fTexHeight );
	m_rtTop.SetUV( 721, 0, 883, 15, fTexWidth, fTexHeight );
	m_rtMiddle.SetUV( 721, 15, 883, 42, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 721, 42, 883, 57, fTexWidth, fTexHeight );

	// HP
	//m_rtHP.SetUV( 187, 0, 188, 5, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 307, 240, 310, 245, fTexWidth, fTexHeight );

	// MP
	//m_rtMP.SetUV( 192, 0, 193, 8, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 312, 240, 315, 245, fTexWidth, fTexHeight );

	// Buff
	//m_rtBuff[0].SetUV( 163, 0, 173, 10, fTexWidth, fTexHeight );
	//m_rtBuff[1].SetUV( 174, 0, 184, 10, fTexWidth, fTexHeight );
	m_rtBuff[0].SetUV( 284, 236, 294, 246, fTexWidth, fTexHeight );
	m_rtBuff[1].SetUV( 295, 236, 305, 246, fTexWidth, fTexHeight );

	// Close Button 
	m_btnClose.Create( this, CTString( "" ), 112, 6, 10, 10 );
	m_btnClose.SetUV( UBS_IDLE, 141, 0, 151, 10, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 152, 0, 162, 10, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnClose.SetEnable(FALSE);


	// UI_REFORM :Su-won
	m_rcBuff.SetRect( 126, 3, 136, 13 );

	m_btnOption.Create( this, CTString( "" ), 113, 3, 10, 10 );
	m_btnOption.SetUV( UBS_IDLE, 508, 256, 526, 275, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 530, 256, 548, 275, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_IDLE, 141, 0, 151, 10, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 152, 0, 162, 10, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : Init()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Init()
{
	m_nType = PT_PEACEEVER;
	m_nMemberCount = 0;
	m_bAmILeader = FALSE;
	m_bIsPartyRequested = FALSE;
	m_bShowBuff = FALSE;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_aslIndices[i] = -1;
		m_astrNames[i] = CTString( "" );
		m_abLeaders[i] = FALSE;
		m_asbJobs[i] = -1;
		m_aswLevels[i] = 0;
		m_aswHPs[i] = 0;
		m_aswMaxHPs[i] = 0;
		m_aswMPs[i] = 0;
		m_aswMaxMPs[i] = 0;
		m_afPosX[i] = 0.0f;
		m_afPosZ[i] = 0.0f;
		m_asbLayer[i] = 0;
		m_aslZone[i] = 0;
		m_aubBuff[i] = 0;

		m_ultargetID[i] =0;
		m_sbtargetType[i] =0;
	}
	m_strMandateCharName = CTString("");
}

// ----------------------------------------------------------------------------
// Name : TargetMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::TargetMember( int nIndex )
{
	if( m_aslIndices[nIndex] == -1 )
		return;

	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iObj = 0; iObj < ctCha; iObj++ )
	{
		// Get target character
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iObj];
		if( ct.cha_Index == m_aslIndices[nIndex] )
		{
			CEntity			*penTargetEntity;
			penTargetEntity = ct.cha_pEntity;

			CEntity			*penPlEntity;
			CPlayerEntity	*penPlayerEntity;
			penPlEntity = CEntity::GetPlayerEntity( 0 );
			penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
			penPlayerEntity->SetTarget( penTargetEntity );
			return;
		}
	}
// 가까이에 없다면 
	_pNetwork->_TargetInfo.Init();
	_pNetwork->_TargetInfo.fMaxHealth = m_aswMaxHPs[nIndex];
	_pNetwork->_TargetInfo.fHealth = m_aswHPs[nIndex];
	_pNetwork->_TargetInfo.bIsActive = TRUE;
	_pNetwork->_TargetInfo.iLevel = m_aswLevels[nIndex];
	strcpy(_pNetwork->_TargetInfo.TargetName, m_astrNames[nIndex] );
	_pNetwork->_TargetInfo.TargetType = CHARACTER;
}

// ----------------------------------------------------------------------------
// Name : AddPartyMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	if( m_nMemberCount >= PARTY_MAX_MEMBER )
		return;

	m_aslIndices[m_nMemberCount] = slIndex;
	m_astrNames[m_nMemberCount] = strName;
	m_abLeaders[m_nMemberCount] = static_cast<BOOL>(sbLeader);// FALSE
	m_asbJobs[m_nMemberCount] = sbJob;// sbLeader
	m_aswLevels[m_nMemberCount] = iLevel;
	m_aswHPs[m_nMemberCount] = iHP;
	m_aswMaxHPs[m_nMemberCount] = iMaxHP;
	m_aswMPs[m_nMemberCount] = iMP;
	m_aswMaxMPs[m_nMemberCount] = iMaxMP;
	m_afPosX[m_nMemberCount] = fX;
	m_afPosZ[m_nMemberCount] = fZ;
	m_asbLayer[m_nMemberCount] = sbLayer;
	m_aubBuff[m_nMemberCount] = 0;
	m_aslZone[m_nMemberCount] = slZone;

	UpdateMemberInfo( m_nMemberCount );
	m_nMemberCount++;

	_pUIMgr->RearrangeOrder( UI_PARTY, TRUE );

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : RemoveMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::RemoveMember( SLONG slIndex )
{
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	if( iMem == m_nMemberCount )
		return;

	CTString	strSysMessage;
	strSysMessage.PrintF( _S2( 421, m_astrNames[iMem], "%s<가> 파티에서 탈퇴하셨습니다." ), m_astrNames[iMem] );
	_pUIMgr->GetChatting()->AddSysMessage(strSysMessage );

	m_nMemberCount--;
	for( int iPos = iMem; iPos < m_nMemberCount; iPos++ )
	{
		m_aslIndices[iPos] = m_aslIndices[iPos + 1];
		m_astrNames[iPos] = m_astrNames[iPos + 1];
		m_abLeaders[iPos] = m_abLeaders[iPos + 1];
		m_asbJobs[iPos] = m_asbJobs[iPos + 1];
		m_aswLevels[iPos] = m_aswLevels[iPos + 1];
		m_aswHPs[iPos] = m_aswHPs[iPos + 1];
		m_aswMaxHPs[iPos] = m_aswMaxHPs[iPos + 1];
		m_aswMPs[iPos] = m_aswMPs[iPos + 1];
		m_aswMaxMPs[iPos] = m_aswMaxMPs[iPos + 1];
		m_afPosX[iPos] = m_afPosX[iPos + 1];
		m_afPosZ[iPos] = m_afPosZ[iPos + 1];
		m_asbLayer[iPos] = m_asbLayer[iPos + 1];
		m_aubBuff[iPos] = m_aubBuff[iPos + 1];
	}
	m_aslIndices[iPos] = -1;
	m_astrNames[iPos] = CTString( "" );
	m_abLeaders[iPos] = FALSE;
	m_asbJobs[iPos] = -1;
	m_aswLevels[iPos] = 0;
	m_aswHPs[iPos] = 0;
	m_aswMaxHPs[iPos] = 0;
	m_aswMPs[iPos] = 0;
	m_aswMaxMPs[iPos] = 0;
	m_afPosX[iPos] = 0.0f;
	m_afPosZ[iPos] = 0.0f;
	m_asbLayer[iPos] = 0;
	m_aubBuff[iPos] = 0;

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : BreakUpParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::BreakUpParty()
{
	Init();

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_PARTY );

	_pUIMgr->RearrangeOrder( UI_PARTY, FALSE );
}

// ----------------------------------------------------------------------------
// Name : UpdateMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberInfo( int nMemIndex )
{
	FLOAT	fHPRatio = (FLOAT)m_aswHPs[nMemIndex] / (FLOAT)m_aswMaxHPs[nMemIndex];
	if( fHPRatio > 1.0f ) fHPRatio = 1.0f;
	m_arcHPs[nMemIndex].Right = m_arcHPs[nMemIndex].Left + PARTY_BAR_WIDTH * fHPRatio;

	FLOAT	fMPRatio = (FLOAT)m_aswMPs[nMemIndex] / (FLOAT)m_aswMaxMPs[nMemIndex];
	if( fMPRatio > 1.0f ) fMPRatio = 1.0f;
	m_arcMPs[nMemIndex].Right = m_arcMPs[nMemIndex].Left + PARTY_BAR_WIDTH * fMPRatio;
}

// 051203
/*
// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( CEntity *pEntity )
{
	ASSERT( pEntity != NULL && "Invalid Entity Pointer!" );
	const int iPartyMemberCount	= GetMemberCount();
	if( iPartyMemberCount > 0 )
	{
		for(INDEX ipl2 = 0; ipl2 < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl2) 
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl2];
			if( ct.cha_iClientIndex == pEntity->en_ulID )
			{						
				for( int i = 0; i < iPartyMemberCount; ++i )
				{
					LONG lMemberIndex = GetMemberIndex( i );
					if( lMemberIndex == ct.cha_Index )
					{
						return true;						
					}
				}
				break;
			}
		}
	}
	return false;
}
*/

// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( INDEX iChaIndex )
{
	const int iPartyMemberCount	= GetMemberCount();
	if( iPartyMemberCount > 0 )
	{
		for( int i = 0; i < iPartyMemberCount; ++i )
		{
			LONG lMemberIndex = GetMemberIndex( i );
			if( lMemberIndex == iChaIndex )
			{
				return true;
			}
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
// Name : UpdateMemberStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP,
									int iMP, int iMaxMP )
{
	// Find party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	// Not found
	if( iMem == m_nMemberCount )
		return;

	// Update infos
	m_aswHPs[iMem] = iHP;
	m_aswMaxHPs[iMem] = iMaxHP;
	m_aswMPs[iMem] = iMP;
	m_aswMaxMPs[iMem] = iMaxMP;

	UpdateMemberInfo( iMem );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	//SetPos( pixMinI, pixMinJ + _pUIMgr->GetPlayerInfo()->GetHeight() );
	SetPos( pixMinI, pixMinJ + _pUIMgr->GetPlayerInfo()->GetHeight() +10);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Render()
{
	// If party is not exist
	if( m_nMemberCount == 0 )
		return;

	// Set party texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	int	nY = m_nPosY + m_rcTitle.Bottom;

	/********************************
	// Top
	

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		// Middle
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + PARTY_STRETCH_HEIGHT,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Buff toggling icon
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		nY += PARTY_STRETCH_HEIGHT;

		// Gague of HP & MP
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Right, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Right, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	// Bottom
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + 2,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );
	********************************/
	
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		nY = m_nPosY +PARTY_STRETCH_HEIGHT *iMem;

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY +15,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

		// Middle
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY +15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT -15,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Bottom
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY +PARTY_HEIGHT -15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

		// Buff toggling icon
		
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		// Gague of HP & MP
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Right, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Right, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	

	// Close button
	//m_btnClose.Render();

	// UI_REFORM :Su-won
	m_btnOption.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Text in party
	//_pUIMgr->GetDrawPort()->PutTextEx( _S( 215, "파티" ), m_nPosX + PARTY_TITLE_TEXT_OFFSETX,
	//									m_nPosY + PARTY_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nY = m_nPosY + PARTY_NAME_SY;

	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		COLOR	clrPartyLeader = 0xFFFFFFFF;
		
		CTString strLevel;
		strLevel.PrintF(CTString("%d"), m_aswLevels[iMem]);
		//_pUIMgr->GetDrawPort()->PutTextExCX(strLevel, m_nPosX + 13, nY, clrPartyLeader);
		_pUIMgr->GetDrawPort()->PutTextExCX(strLevel, m_nPosX + 12, nY -PARTY_NAME_SY +18, clrPartyLeader);

		if( m_abLeaders[iMem] )
		{// 파티장 구분 색
			clrPartyLeader = 0x72D02EFF;
		}

		_pUIMgr->GetDrawPort()->PutTextEx( m_astrNames[iMem], m_nPosX + PARTY_NAME_SX+13, nY, clrPartyLeader );
		nY += PARTY_STRETCH_HEIGHT;
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render party buff
	if( m_bShowBuff )
		_pUIBuff->RenderPartyBuff();
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ToggleVisible()
{
	if( m_nMemberCount == 0 )
		return;

	BOOL	bVisible = !IsVisible();
	_pUIMgr->RearrangeOrder( UI_PARTY, bVisible );
}

// ----------------------------------------------------------------------------
// Name : UpdatePartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdatePartyBuff( int nMemIndex, int nX, int nY )
{
	if( m_aslIndices[nMemIndex] == _pUIBuff->GetPartyIndex() )
		return;

	m_bShowBuff = FALSE;
	_pUIBuff->ResetPartyBuff();
	_pUIBuff->SetPartyBuffPos( m_aslIndices[nMemIndex], nX, nY );

	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iCha = 0; iCha < ctCha; iCha++ )
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iCha];
		if( m_aslIndices[nMemIndex] == ct.cha_Index )
		{
			if( ct.cha_BuffCount <= 0 )
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for( SBYTE sbBuff = 0; sbBuff < ct.cha_BuffCount; sbBuff++ )
				_pUIBuff->AddPartyBuff( m_aslIndices[nMemIndex], ct.cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}

	_pUIBuff->ResetPartyBuff();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIParty::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessagePartyBuff( pMsg ) ) != WMSG_FAIL )
	{
		if( wmsgResult == WMSG_COMMAND )
			_pUIMgr->RearrangeOrder( UI_PARTY, TRUE );

		return wmsgResult;
	}

	// Title bar
	static BOOL bTitleBarClick = FALSE;

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

			// Move party
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// UI_REFORM :Su-won
			// Option Button
			else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Buff toggle
			else
			{
				for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
					m_aubBuff[iMem] = 0;

				int	nBuffX = m_nPosX;
				if( nX >= nBuffX + m_rcBuff.Left && nX <= nBuffX + m_rcBuff.Right )
				{
					//int	nBuffY = m_nPosY + m_rcTitle.Bottom;
					int	nBuffY = m_nPosY;

					for( iMem = 0; iMem < m_nMemberCount; iMem++ )
					{
						if( nY >= nBuffY + m_rcBuff.Top && nY <= nBuffY + m_rcBuff.Bottom )
						{
							m_aubBuff[iMem] = 1;

							UpdatePartyBuff( iMem, nBuffX + m_rcBuff.Left, nBuffY + m_rcBuff.Bottom );
							return WMSG_SUCCESS;
						}

						nBuffY += PARTY_STRETCH_HEIGHT;
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// UI_REFORM :Su-won
				// Option Button
				if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Targetting party member
				else
				{
					int	iMem = ( nY - m_nPosY) / (PARTY_HEIGHT);
					if( iMem >= 0 && iMem < m_nMemberCount )
						TargetMember( iMem );
				}

				_pUIMgr->RearrangeOrder( UI_PARTY, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If party isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						_pUIMgr->RearrangeOrder( UI_PARTY, FALSE );

					return WMSG_SUCCESS;
				}

				// UI_REFORM :Su-won
				// Option Button
				if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( _pUIMgr->DoesMessageBoxExist(MSGCMD_PARTY_OPTION) )
						return WMSG_SUCCESS;

					CUIMsgBox_Info MsgBoxInfo;

					CTString strParty =_S(2679, "파티 종류") +_s(":  ");

					switch(m_nType)
					{
					case PT_PEACEEVER:
						strParty += _S(2660, "균등분배 파티");
						break;
					case PT_SURVIVAL:
						strParty += _S(2661, "입수우선 파티");
						break;
					case PT_ATTACK:
						strParty += _S(2662, "전투형 파티");
						break;
					}
				
					MsgBoxInfo.AddString( _s("   ") );
					MsgBoxInfo.AddString( strParty, 0xF2F2F2FF, TEXT_CENTER );
					
					if( AmILeader() )
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(4166, "파티 옵션"), UMBS_OKCANCEL | UMBS_COMBOBOX ,UI_PARTY, MSGCMD_PARTY_OPTION);

						MsgBoxInfo.AddString( _s("   ") );
						MsgBoxInfo.AddString( _s("   ") );
						MsgBoxInfo.AddString(_S(4170, "파티 종류 변경"));

						_pUIMgr->CreateMessageBox(MsgBoxInfo);

						_pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().ResetStrings();
						_pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().AddString(_S(2660, "균등분배 파티"));
						_pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().AddString(_S(2661, "입수우선 파티"));
						_pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().AddString(_S(2662, "전투형 파티"));

						_pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().SetPosY(110);
					}
					else
					{
						MsgBoxInfo.SetMsgBoxInfo(_S(4166, "파티 옵션"), UMBS_OK, UI_PARTY, MSGCMD_NULL);

						_pUIMgr->CreateMessageBox(MsgBoxInfo);
					}

					

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				int	iMem = (nY - m_nPosY) / (PARTY_HEIGHT);
				if( iMem >= 0 && iMem < m_nMemberCount )
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(m_ultargetID[iMem], m_sbtargetType[iMem]);

				_pUIMgr->RearrangeOrder( UI_PARTY, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

  		case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PARTY_INVITE:
		if( !bOK )
		{
			SendPartyReject();
		}

		break;

	case MSGCMD_PARTY_ALLOW:
		if( bOK )
			SendPartyAllow();
		else
			SendPartyReject();

		break;
	case MSGCMD_PARTY_MANDATE_REQ: 
		{
			if( bOK )
			{
				int nCharPos;

				if( ( nCharPos = IsOurPartyPos( m_strMandateCharName ) ) != -1 )
				{
					m_abLeaders[nCharPos] = TRUE;
					m_bAmILeader = FALSE;

					_pNetwork->MandateBossReq( m_strMandateCharName );
				}
			}
		}
		break;
	// UI_REFORM :Su-won
	case MSGCMD_PARTY_OPTION:
		{
			if( bOK )
			{
				CNetworkMessage	nmMessage( MSG_PARTY );
				nmMessage << (SBYTE)MSG_PARTY_CHANGETYPE;
				nmMessage << (SBYTE) _pUIMgr->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().GetCurSel();;
					
				_pNetwork->SendToServerNew( nmMessage );				
			}
		}
		break;
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendPartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyInvite( SBYTE sbType, CTString &strDestName )
{
	// If I'm not a leader
	if( m_nMemberCount > 0 && !m_bAmILeader )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 364, "파티 신청 권한이 없습니다" ), SYSMSG_ERROR );
		return;
	}

	// If party is already requested
	CTString	strMessage;
	if( m_bIsPartyRequested )
	{
		strMessage.PrintF( _S( 361, "이미 %s에게 파티를 신청중입니다." ), m_strDestName );
		_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	// If party type is different
	if( m_nMemberCount > 0 && sbType != m_nType )
	{
		switch( m_nType )
		{
		case PT_PEACEEVER:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 362, "이미 균등 분배 파티중입니다." ), SYSMSG_ERROR );
			break;
		case PT_SURVIVAL:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 363, "이미 입수 우선 파티중입니다." ), SYSMSG_ERROR );
			break;
		case PT_ATTACK:
			_pUIMgr->GetChatting()->AddSysMessage( _S( 1651,  "이미 전투형 파티중입니다."  ), SYSMSG_ERROR ); 
			break;
		}
		return;
	}

	// Find index of character
	SLONG	slIndex = -1;
	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iPlyaer = 0; iPlyaer < ctCha; iPlyaer++ ) 
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iPlyaer];
		if( ct.cha_strName == strDestName )
		{
			if( ct.cha_sbShopType != PST_NOSHOP )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 940, "개인 상점과는 파티를 할 수 없습니다." ) , SYSMSG_ERROR );	
				return;
			}
			slIndex = ct.cha_Index;
			break;
		}
	}

	// If target is already my party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( slIndex == m_aslIndices[iMem] )
		{
			strMessage.PrintF( _S2( 420, m_strDestName, "이미 %s<과> 파티중입니다." ), m_strDestName );
			_pUIMgr->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );

			return;
		}
	}

	// If character doesn't exist
	if( slIndex == -1 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 310, "파티 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyInvite( sbType, slIndex );
	m_strDestName = strDestName;
}

// ----------------------------------------------------------------------------
// Name : SendPartyAllow()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyAllow()
{
	_pNetwork->PartyAllow();
}

// ----------------------------------------------------------------------------
// Name : SendPartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyReject()
{
	_pNetwork->PartyReject();
}

// ----------------------------------------------------------------------------
// Name : SendPartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyQuit()
{
	if( m_nMemberCount > 0 )
		_pNetwork->PartyQuit();
}

// ----------------------------------------------------------------------------
// Name : SendPartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyKick( CTString &strDestName )
{
	if( !m_bAmILeader )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 365, "파티원 강퇴 권한이 없습니다." ), SYSMSG_ERROR );
		return;
	}

	// Find index of character
/*	SLONG	slIndex = -1;
	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iPlyaer = 0; iPlyaer < ctCha; iPlyaer++ ) 
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iPlyaer];
		if( ct.cha_strName == strDestName )
		{
			slIndex = ct.cha_Index;
			break;
		}
	}
*/
	int nIndex = IsOurPartyPos( strDestName );

	// If character doesn't exist
	if( nIndex == -1 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 366, "강퇴 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyKick( m_aslIndices[nIndex] );
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyInvite( SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName )
{
	m_bAmILeader = slSrcIndex == _pNetwork->MyCharacterInfo.index;
	m_nType = sbType;

	CTString	strMessage;
	if( m_bAmILeader )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_CANCEL, UI_PARTY, MSGCMD_PARTY_INVITE );
		strMessage.PrintF( _S( 216, "%s에게 파티를 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );

		m_bIsPartyRequested = TRUE;
	}
	else
	{
		if( !g_iRejectParty )
		{
			switch( m_nType )
			{
			case PT_PEACEEVER:
				strMessage.PrintF( _S2( 217, strSrcName, "%s<가> 파티(균등 분배) 신청을 했습니다. 수락하시겠습니까?" ), strSrcName );
				break;
			case PT_SURVIVAL:
				strMessage.PrintF( _S2( 218, strSrcName, "%s<가> 파티(입수 우선) 신청을 했습니다. 수락하시겠습니까?" ), strSrcName );
				break;
			case PT_ATTACK:
				strMessage.PrintF( _S2( 1652, strSrcName, "%s<가> 파티(전투형) 신청을 했습니다. 수락하시겠습니까?" ), strSrcName );
				break;
			}
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_ALLOW );
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		// [7/13/2009 rumist] reject invite message.
		else
		{
			SendPartyReject();
			return;
		}
	}

	// Character state flags
	_pUIMgr->SetCSFlagOn( CSF_PARTY_REQ );
}

// ----------------------------------------------------------------------------
// Name : PartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyReject( BOOL bLeader )
{
	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader && !bLeader )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_PARTY_INVITE );

	}
	else if( !m_bAmILeader && bLeader )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_PARTY_ALLOW );
	}
	// 파티 매칭 승낙시에 메세지 창 제거
	_pUIMgr->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );

	_pUIMgr->GetChatting()->AddSysMessage( _S( 219, "파티 신청이 취소되었습니다." ) );

	if( m_bAmILeader && m_nMemberCount <= 0 )
		m_bAmILeader = FALSE;

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_PARTY_REQ );
}

// ----------------------------------------------------------------------------
// Name : PartyAddMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	int nIsCharPos;
	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader )
	{ _pUIMgr->CloseMessageBox( MSGCMD_PARTY_INVITE ); }
	else// 파티 매칭으로 들어온 메세지 창을 닫는다.
	{ 
		_pUIMgr->CloseMessageBox( MSGCMD_PARTY_INVITE );
		_pUIMgr->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );
	}

	if( ( nIsCharPos = IsOurParty( slIndex ) ) != -1 )// 이미 파티에 들어 있는 유저(갱신)
	{
		m_aslIndices[nIsCharPos] = slIndex;
		m_astrNames[nIsCharPos] = strName;
		m_abLeaders[nIsCharPos] = static_cast<BOOL>(sbLeader);
		m_asbJobs[nIsCharPos] = sbJob;
		m_aswLevels[nIsCharPos] = iLevel;
		m_aswHPs[nIsCharPos] = iHP;
		m_aswMaxHPs[nIsCharPos] = iMaxHP;
		m_aswMPs[nIsCharPos] = iMP;
		m_aswMaxMPs[nIsCharPos] = iMaxMP;
		m_afPosX[nIsCharPos] = fX;
		m_afPosZ[nIsCharPos] = fZ;
		m_asbLayer[nIsCharPos] = sbLayer;
		m_aubBuff[nIsCharPos] = 0;
		m_aslZone[nIsCharPos] = slZone;

		UpdateMemberInfo( nIsCharPos );
	}
	else	// 파티에 없는 유저
	{
		AddPartyMember( sbLeader, slIndex, strName, sbJob, sbJob2, iLevel, iHP, iMaxHP, iMP, iMaxMP, fX, fZ, sbLayer, slZone );
	}
	// 현재 가진 파티원 정보를 토대로 리더를 설정 한다.
	SetPartyLeader();

	// Character state flags
	_pUIMgr->SetCSFlagOff( CSF_PARTY_REQ );
	_pUIMgr->SetCSFlagOn( CSF_PARTY );
}

// ----------------------------------------------------------------------------
// Name : PartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyQuit( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		BreakUpParty();
		_pUIMgr->GetChatting()->AddSysMessage( _S( 367, "파티에서 탈퇴하셨습니다." ) );
	}
	else
		RemoveMember( slIndex );
}

// ----------------------------------------------------------------------------
// Name : PartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyKick( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		BreakUpParty();
		_pUIMgr->GetChatting()->AddSysMessage( _S( 368, "파티에서 강퇴 되셨습니다." ) );
	}
	else
		RemoveMember( slIndex );
}

// ----------------------------------------------------------------------------
// Name : PartyEnd()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyEnd()
{
	m_bIsPartyRequested = FALSE;

	BreakUpParty();
	_pUIMgr->GetChatting()->AddSysMessage( _S( 220, "파티가 해체되었습니다." ) );
}

// ----------------------------------------------------------------------------
// Name : PartyMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	// Find party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	// Not found
	if( iMem == m_nMemberCount )
		return;

	// Update infos
	m_aswLevels[iMem] = iLevel;
	m_aswHPs[iMem] = iHP;
	m_aswMaxHPs[iMem] = iMaxHP;
	m_aswMPs[iMem] = iMP;
	m_aswMaxMPs[iMem] = iMaxMP;
	m_afPosX[iMem] = fX;
	m_afPosZ[iMem] = fZ;
	m_asbLayer[iMem] = sbLayer;
	m_aslZone[iMem] = slZone;

	UpdateMemberInfo( iMem );
}

// ----------------------------------------------------------------------------
// Name : PartyError()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyError( SBYTE sbError )
{
	CTString	strSysMessage;
	switch( sbError )
	{
	case MSG_PARTY_ERROR_NOTFOUND:
		strSysMessage = _S( 397, "파티 대상을 찾을 수 없습니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_ME:
		strSysMessage = _S( 311, "이미 파티 중입니다." );
		break;

	case MSG_PARTY_ERROR_FULL:
		strSysMessage = _S( 312, "파티원이 가득 찼습니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_TARGET:
		strSysMessage = _S( 396, "대상이 이미 파티중입니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_REQUEST:
		strSysMessage = _S( 324, "대상이 이미 파티 신청중입니다." );
		break;

	case MSG_PARTY_ERROR_DIFF_TYPE:
		strSysMessage = _S( 325, "파티 형식이 다릅니다." );
		break;

	case MSG_PARTY_ERROR_NOT_BOSS:
		strSysMessage = _S( 326, "파티장이 아니라 권한이 없습니다." );
		break;

	case MSG_PARTY_ERROR_INVALID_LEVEL:
		strSysMessage = _S( 1653, "레벨이 맞지않아 파티 신청을 할 수 없습니다." ); 
		break;
	
	case MSG_PARTY_ERROR_NOT_PARTY:
		strSysMessage = _S(2167, "대상이 파티중이 아닙니다." ); 
		break;

	case MSG_PARTY_ERROR_NOT_PARTY_MEMBER:
		strSysMessage = _S(2168, "대상이 파티원이 아닙니다." ); 
		break;
	case MSG_PARTY_ERROR_PVP:
		strSysMessage = _S(3073, "PvP중인 캐릭터와는 파티를 할 수 없습니다" );
		break;
	// [090728: selo] 몬스터 콤보중 파티장을 변경 할 수 없음.
	case MSG_PARTY_ERROR_DO_NOT_CHANGE_BOSS:
		strSysMessage = _S(4695, "몬스터 콤보 중에는 파티장을 변경 할 수 없습니다.");
		break;
	}
	_pUIMgr->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

	if (_pUIMgr->DoesMessageBoxExist(MSGCMD_PARTY_INVITE))
	{
		_pUIMgr->CloseMessageBox( MSGCMD_PARTY_INVITE );
	}
}

//------------------------------------------------------------------------------
// CUIParty::MandateBossReq
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBossReq( CTString strManadateChaName )
{
	m_strMandateCharName = strManadateChaName;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "파티" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_MANDATE_REQ );

	CTString strNewBossName;
	CTString strMessage;

	strMessage.PrintF( _S(2170, "정말로 %s님께 파티장을 위임하시겠습니까?" ), strManadateChaName );
		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIParty::MandateBoss
// Explain:  파티장 위임 메시지 
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate )
{
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "파티" ), UMBS_OK, UI_PARTY, MSGCMD_PARTY_MANDATE );
	CTString strMessage;
	
	if( strManadateChaName == _pNetwork->MyCharacterInfo.name ) 
	{
		int nCharPos = IsOurPartyPos( strBossName );

		if( nCharPos != -1 )
		{
			m_abLeaders[nCharPos] = FALSE;
		}
	}
	else 
	{
		for( int nMem=0; nMem<m_nMemberCount; nMem++ )
		{
			if( m_aslIndices[nMem] == nManadateChaIndex )
			{
				int nCharPos = IsOurPartyPos( strBossName );

				if( nCharPos != -1 )
					m_abLeaders[nCharPos] = FALSE;

				m_abLeaders[nMem] = TRUE;
				break;
			}
		}
	}

	SetPartyLeader();

	if( sbMandate )
	{
		strMessage.PrintF( _S(2171, "%s님이 파티장을 위임하여 %s님이 새로운 파티장으로 임명되었습니다." ), strBossName, strManadateChaName );
	}
	else
	{
		strMessage.PrintF( _S(2172, "%s님이 새로운 파티장으로 임명되었습니다." ), strManadateChaName );
	}
	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  해당 파티원이 우리 파티원인지 아닌지 확인 
// Date : 2005-10-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIParty::IsOurParty( CTString strName )
{
	for( int i = 0; i < m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  식별자로 해당 파티원이 우리 파티원인지 아닌지 확인 ( 유저가 없으면 -1 리턴 )
// Date : 2006-05-19(오전 9:54:23), By eons
//------------------------------------------------------------------------------
int CUIParty::IsOurParty( SLONG slCharIndex )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_aslIndices[i] == slCharIndex )
			return i;
	}

	return -1;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  이름으로 해당 파티원이 우리 파티원인지 아닌지 확인 ( 유저가 없으면 -1 리턴 )
// Date : 2006-05-19(오전 9:54:23), By eons
//------------------------------------------------------------------------------
int	CUIParty::IsOurPartyPos( CTString strName )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
			return i;
	}

	return -1;
}

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  현재 가진 파티 정보 중 리더가 없으면 자신이 리더가 된다.
// Date : 2006-05-19(오전 10:11:07), By eons
//------------------------------------------------------------------------------
void CUIParty::SetPartyLeader()
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_abLeaders[i] == TRUE )
		{
			m_bAmILeader = FALSE;
			return;
		}
	}

	m_bAmILeader = TRUE;
}

void CUIParty::SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType)
{
	for(int i=0; i<PARTY_MAX_MEMBER; ++i)
	{
		if( m_aslIndices[i] ==partyID)
		{
			m_ultargetID[i] =targetID;
			m_sbtargetType[i] =sbtargetType;
			return;
		}
	}
}
