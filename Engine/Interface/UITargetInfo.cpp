#include "stdh.h"
#include <Engine/Interface/UITargetInfo.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/LocalDefine.h>
//이름옆에
#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3
//HP표시되는 곳에
//#define INFOMARK_POS_X 120
//#define INFOMARK_POS_Y 23

#define INFOMARK_SIZE 16
extern INDEX g_iCountry;
int m_strGap=0;
// ----------------------------------------------------------------------------
// Name : CUITargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUITargetInfo::CUITargetInfo()
{
	m_colNameColor[0] = 0xCE4900FF;		// Mob
	m_colNameColor[1] = 0xD7AA00FF;
	m_colNameColor[2] = 0xC5C5C5FF;
	m_colNameColor[3] = 0x5B9900FF;
	m_colNameColor[4] = 0x008BAAFF;

	m_colNameColor[5] = 0xE18600FF;		// Npc

	m_colNameColor[6] = 0x06C1FFFF;		// Title, PK
	m_colNameColor[7] = 0x0066FFFF;
	m_colNameColor[8] = 0xFFFFFFFF;
	m_colNameColor[9] = 0xABABABFF;
	m_colNameColor[10] = 0xFF1E00FF;
	m_colNameColor[11] = 0xBB3B00FF;

	m_bShowBuff = FALSE;
	m_bShowPkHp = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUITargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUITargetInfo::~CUITargetInfo()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 140, 22 );
	//m_rcHP.SetRect( 10, 27, 130, 35 );
	m_rcHP.SetRect( 36, 31, 155, 38 );
	m_rcHPBack.SetRect( 5, 25, 135, 37 );
	//m_rcLv.SetRect(16, 2, 38, 18);
	m_rcLv.SetRect(2, 15, 14, 31);

	// Create inventory texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TargetInfo.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground.SetUV( 0, 0, 140, 43, fTexWidth, fTexHeight );
	//m_rtBackground_new_L.SetUV( 178,0,214,64,fTexWidth,fTexHeight);
	
	m_rtNPCBackground.SetUV( 519, 79, 706, 136, fTexWidth, fTexHeight );
	m_rtMobBackground.SetUV( 519, 0, 706, 57, fTexWidth, fTexHeight );;
	m_rtBackground_new_L.SetUV( 302,0,501,80,fTexWidth,fTexHeight);
	m_rtBackground_new_M.SetUV( 215,0,218,64,fTexWidth,fTexHeight);
	m_rtBackground_new_R.SetUV( 218,0,254,64,fTexWidth,fTexHeight);

	// HP
	//m_rtHP.SetUV( 1, 44, 2, 52, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 286, 220, 289, 228, fTexWidth, fTexHeight );

	// HP background
	m_rtHPBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );

	m_rtLvUV.SetUV( 142, 46, 158, 62, fTexWidth, fTexHeight);

	//info mark
	/***
	m_rcInfoMark.SetRect(
		INFOMARK_POS_X
		, INFOMARK_POS_Y
		, INFOMARK_POS_X + INFOMARK_SIZE
		, INFOMARK_POS_Y + INFOMARK_SIZE);
		***/
	m_rcInfoMark.SetRect(
		158
		, 2
		, 158 + 12
		, 2+ 12);

	//m_rtInfoMarkUV.SetUV(142, 28, 158, 44, fTexWidth, fTexHeight );
	m_rtInfoMarkUV.SetUV( 295, 236, 305, 246, fTexWidth, fTexHeight );

	/***
	m_rtInfoUL.SetUV( 164-24, 45-41, 171-24, 63-41, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 174-24, 45-41, 176-24, 63-41, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 179-24, 45-41, 186-24, 63-41, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 164-24, 55-41, 171-24, 58-41, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 174-24, 55-41, 176-24, 58-41, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 179-24, 55-41, 186-24, 58-41, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 164-24, 60-41, 171-24, 68-41, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 174-24, 60-41, 176-24, 68-41, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 179-24, 60-41, 186-24, 68-41, fTexWidth, fTexHeight );
	***/
	m_rtInfoUL.SetUV( 239, 253, 246, 260, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 246, 253, 329, 260, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 329, 253, 336, 260, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 239, 260, 246, 262, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 246, 260, 329, 262, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 329, 260, 336, 262, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 239, 262, 246, 269, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 246, 262, 329, 269, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 329, 262, 336, 269, fTexWidth, fTexHeight );

	m_bShowInfo = FALSE;
	m_nCurInfoLines = 0;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::UpdateHPInfo()
{
	// HP
	FLOAT	fHPRatio = _pNetwork->_TargetInfo.fHealth / _pNetwork->_TargetInfo.fMaxHealth;
	m_rcHP.Right = m_rcHP.Left + (m_strGap + TARGETINFO_BAR_WIDTH) * fHPRatio;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::SetTargetBuff( BOOL bMe )
{
	m_bShowBuff = FALSE;
	_pUIBuff->ResetTargetBuff();

	if( bMe )
	{
		_pUIBuff->CopyMyBuffToTargetBuff();
		m_bShowBuff = TRUE;
	}
	else
	{
		if( _pNetwork->_TargetInfo.TargetType == CHARACTER )
		{
			INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
			for( INDEX iCha = 0; iCha < ctCha; iCha++ ) 
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iCha];
				if( _pNetwork->_TargetInfo.pen_pEntity == ct.cha_pEntity )
				{
					for( SBYTE sbBuff = 0; sbBuff < ct.cha_BuffCount; sbBuff++ )
						_pUIBuff->AddTargetBuff( ct.cha_Buff[sbBuff] );

					m_bShowBuff = TRUE;
					return;
				}
			}
		}
		else if( _pNetwork->_TargetInfo.TargetType == MOB )
		{
			INDEX	ctMob = _pNetwork->ga_srvServer.srv_amtMob.Count();
			for( INDEX iMob = 0; iMob < ctMob; iMob++ ) 
			{
				CMobTarget	&mt = _pNetwork->ga_srvServer.srv_amtMob[iMob];
				if( _pNetwork->_TargetInfo.pen_pEntity == mt.mob_pEntity )
				{
					for( SBYTE sbBuff = 0; sbBuff < mt.mob_BuffCount; sbBuff++ )
						_pUIBuff->AddTargetBuff( mt.mob_Buff[sbBuff] );

					m_bShowBuff = TRUE;
					return;
				}
			}
		}
		else if( _pNetwork->_TargetInfo.TargetType == SUMMON )
		{
			INDEX	ctSlave = _pNetwork->ga_srvServer.srv_actSlave.Count();
			for( INDEX iMob = 0; iMob < ctSlave; iMob++ ) 
			{
				CSlaveTarget	&st = _pNetwork->ga_srvServer.srv_actSlave[iMob];
				if( _pNetwork->_TargetInfo.pen_pEntity == st.slave_pEntity )
				{
					for( SBYTE sbBuff = 0; sbBuff < st.slave_BuffCount; sbBuff++ )
						_pUIBuff->AddTargetBuff( st.slave_Buff[sbBuff] );

					m_bShowBuff = TRUE;
					return;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::Render()
{
	if( !_pNetwork->_TargetInfo.bIsActive )
		return;

	// Set target information texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// wooss 060511----------------------------------------------<<
		// wooss 050925
		int tv_num = 0;
		m_strGap = 0;
		if(g_iCountry == MALAYSIA || g_iCountry == USA || g_iCountry == HONGKONG){
			extern BOOL g_bIsMalEng;
			if(g_bIsMalEng || g_iCountry == USA){
				CTString tv_str;
				tv_str.PrintF("%s", _pNetwork->_TargetInfo.TargetName);
				tv_num=(tv_str.Length()+10)*( _pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
				if(tv_num > TARGETINFO_WIDTH){
					m_strGap = tv_num - TARGETINFO_WIDTH;
				}
				else {
					tv_num = TARGETINFO_WIDTH;
				}
				SetSize(tv_num,m_nHeight);
			}
		}
		
		//------------------------------------------------------------>>
	
	
	// Add render regions
	CMobData& MD = _pNetwork->GetMobData(_pNetwork->_TargetInfo.dbIdx);
	if( MD.IsPartyMob() )
	{
		// Background New 
		/********************************
		int nX1 = m_nPosX - 18;
		int nX2 = m_nPosX + 18;
		_pUIMgr->GetDrawPort()->AddTexture( nX1, m_nPosY, nX2 , m_nPosY + m_nHeight + 21,
											m_rtBackground_new_L.U0, m_rtBackground_new_L.V0, m_rtBackground_new_L.U1, m_rtBackground_new_L.V1,
											0xFFFFFFFF );
		nX1 = nX2;
		nX2 = nX1 + m_nWidth - 36;
		_pUIMgr->GetDrawPort()->AddTexture( nX1, m_nPosY, nX2 , m_nPosY + m_nHeight + 21,
											m_rtBackground_new_M.U0, m_rtBackground_new_M.V0, m_rtBackground_new_M.U1, m_rtBackground_new_M.V1,
											0xFFFFFFFF );
		nX1 = nX2;
		nX2 = nX1 + 36;
		_pUIMgr->GetDrawPort()->AddTexture( nX1, m_nPosY,nX2 , m_nPosY + m_nHeight + 21,
											m_rtBackground_new_R.U0, m_rtBackground_new_R.V0, m_rtBackground_new_R.U1, m_rtBackground_new_R.V1,
											0xFFFFFFFF );
		********************************/
		int nX = m_nPosX - 12;
		int nY = m_nPosY + m_nHeight + 22;

		_pUIMgr->GetDrawPort()->AddTexture( nX, m_nPosY,  m_nPosX + m_nWidth, nY,
											m_rtBackground_new_L.U0, m_rtBackground_new_L.V0, m_rtBackground_new_L.U1, m_rtBackground_new_L.V1,
											0xFFFFFFFF );
	}
	else 
	{
		// Background
		/***
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
											0xFFFFFFFF );
		***/
		if( _pUIMgr->IsEnemy(_pNetwork->_TargetInfo.pen_pEntity, TARGET_TYPE(_pNetwork->_TargetInfo.TargetType), TRUE) )
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtMobBackground.U0, m_rtMobBackground.V0, m_rtMobBackground.U1, m_rtMobBackground.V1,
											0xFFFFFFFF );
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtNPCBackground.U0, m_rtNPCBackground.V0, m_rtNPCBackground.U1, m_rtNPCBackground.V1,
											0xFFFFFFFF );
		}		
	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	switch( _pNetwork->_TargetInfo.TargetType )
	{
	case MOB:
		{
			// Update info of target
			UpdateHPInfo();
			// Background of HP
			/*******
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHPBack.Left, m_nPosY + m_rcHPBack.Top,
												m_nPosX + m_rcHPBack.Right+m_strGap, m_nPosY + m_rcHPBack.Bottom,
												m_rtHPBack.U0, m_rtHPBack.V0, m_rtHPBack.U1, m_rtHPBack.V1,
												0xFFFFFFFF );
			*****/
			// HP
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
												m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
												m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
												0xFFFFFFFF );
			// Level Background
			/*****
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcLv.Left, m_nPosY + m_rcLv.Top,
												m_nPosX + m_rcLv.Right, m_nPosY + m_rcLv.Bottom,
												m_rtLvUV.U0, m_rtLvUV.V0, m_rtLvUV.U1, m_rtLvUV.V1,0xFFFFFFFF);
			*****/


			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Text color
			int	nIndex = 4;
			int	nLevelDiff = _pNetwork->_TargetInfo.iLevel - _pNetwork->MyCharacterInfo.level;
			if( nLevelDiff > 5 ) nIndex = 0;
			else if( nLevelDiff > 2 ) nIndex = 1;
			else if( nLevelDiff > -3 ) nIndex = 2;
			else if( nLevelDiff > -6 ) nIndex = 3;

			//CTString tempStr;
			//tempStr.PrintF("%.2f / %.2f", _pNetwork->_TargetInfo.fHealth, _pNetwork->_TargetInfo.fMaxHealth);
			//_pUIMgr->GetDrawPort()->PutTextCharEx( tempStr, 0, m_nPosX + m_rcHPBack.Left + 10, m_nPosY + m_rcHPBack.Top);
			// Mob Level
			CTString tv_str;
			tv_str.PrintF("%d", _pNetwork->_TargetInfo.iLevel);
			_pUIMgr->GetDrawPort()->PutTextCharExCX(tv_str, 0 ,m_nPosX+m_rcLv.Left+11, m_nPosY+m_rcLv.Top+2);
			// Mob name 레벨 표시로 간격 벌림
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														//(m_nPosX + m_nWidth / 2) + 11, m_nPosY + TARGETINFO_NAME_SY,
														(m_nPosX + m_nWidth / 2) , m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[nIndex] );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		break;

	case PET:
	case WILDPET:
		{
			// Character name
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[9] );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		break;

	case NPC:
		{
			// Npc name
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[5] );
#ifdef HELP_SYSTEM_1
			//NPC 안내시스템
			if(_pUIMgr->m_nHelpNpc_Index == _pNetwork->_TargetInfo.dbIdx) _pUIMgr->m_nHelpNpc_Index = -1;
#endif

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		break;

	case CHARACTER:
		{
			// WSS_PK_TARGET_HP 070726 ------------------------------------------------------------>>
			if( m_bShowPkHp )
			{			
			// Update info of target
			UpdateHPInfo();
			// HP
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
												m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
												m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
												0xFFFFFFFF );
			// HP Value
			CTString tStr;
			tStr.PrintF("%d",(int)_pNetwork->_TargetInfo.fHealth);
			_pUIMgr->GetDrawPort()->PutTextCharExCX(	tStr.str_String , 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY + 18,
														m_colNameColor[5] );			 
			}
			// -------------------------------------------------------------------------------------<<

			// Info mark
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcInfoMark.Left, m_nPosY + m_rcInfoMark.Top,
												m_nPosX + m_rcInfoMark.Right, m_nPosY + m_rcInfoMark.Bottom,
												m_rtInfoMarkUV.U0, m_rtInfoMarkUV.V0
												, m_rtInfoMarkUV.U1, m_rtInfoMarkUV.V1,
												0xFFFFFFFF );
			

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Text color
			int	nIndex = 9;
			if( _pNetwork->_TargetInfo.PkState < -9 ) nIndex = 11;
			else if( _pNetwork->_TargetInfo.PkState > 9 ) nIndex = 7;
			if( _pNetwork->_TargetInfo.PkMode != 0 )
				nIndex--;

			// Character name
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[nIndex] );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		break;

	case ITEM:
		{
			// Item name
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[2] );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		break;

	default:	// Product
		{
			// Name
			_pUIMgr->GetDrawPort()->PutTextCharExCX( _pNetwork->_TargetInfo.TargetName, 0,
														m_nPosX + m_nWidth / 2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[2] );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
	};

	// Render target buff
	if( m_bShowBuff )
		_pUIBuff->RenderTargetBuff();

	RenderInfo();
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::RenderInfo()
{
	if( !m_bShowInfo ) return;

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// information region
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Top,
										m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top,
										m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top,
										m_rcInfo.Right, m_rcInfo.Top + 7,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Top + 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rcInfo.Right, m_rcInfo.Bottom - 7,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left, m_rcInfo.Bottom - 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right, m_rcInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFFF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Render information
	int	nInfoX = m_rcInfo.Left + 12;
	int	nInfoY = m_rcInfo.Top + 8;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strInfo[iInfo], nInfoX, nInfoY, m_colInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUITargetInfo::MouseMessage( MSG *pMsg )
{
	// If target is not exist
	if( !_pNetwork->_TargetInfo.bIsActive )
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	// Buff mouse message
	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessageTargetBuff( pMsg ) ) != WMSG_FAIL )
	{
		_pUIMgr->RearrangeOrder( UI_TARGETINFO, TRUE );
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
			m_bShowInfo = FALSE;
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			if(_pNetwork->_TargetInfo.TargetType == CHARACTER && IsInsideRect(nX, nY, m_rcInfoMark))
			{
				//여기서 타겟인포를 m_strInfo에 저장한다.
				//명성 공개시
				bool bFound = false;
				SLONG fame = 0;
				if(_pNetwork->_TargetInfo.pen_pEntity == CEntity::GetPlayerEntity(0))//내 정보
				{
					bFound = true;
					fame = _pNetwork->MyCharacterInfo.fame;
				}
				else
				{
					for(int i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						if( _pNetwork->_TargetInfo.pen_pEntity != NULL &&
							_pNetwork->_TargetInfo.pen_pEntity->en_ulID == _pNetwork->ga_srvServer.srv_actCha[i].cha_iClientIndex)
						{
							bFound = true;
							fame = _pNetwork->ga_srvServer.srv_actCha[i].cha_nFame;
						}
					}
				}
				
				if(bFound)
				{
					m_nCurInfoLines = 1;
					if(fame == -1)
						m_strInfo[0].PrintF(_S( 1741, "명성수치 비공개" ));
					else
						m_strInfo[0].PrintF(_S( 1742, "명성수치 : %d" ), fame);
					m_colInfo[0] = 0xFFFFFFFF;
					m_bShowInfo = TRUE;
				}
			}

			if(m_bShowInfo)
			{
				//크기 계산
				int		nInfoWidth, nInfoHeight;
				nInfoWidth = 0;
				for(int i=0; i<m_nCurInfoLines; ++i)
				{
					int infoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strInfo[i].Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
					if(infoWidth > nInfoWidth) nInfoWidth = infoWidth;
				}
				nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
				
				m_rcInfo.SetRect(m_nPosX + TARGETINFO_WIDTH + 4
					, m_nPosY
					, m_nPosX + TARGETINFO_WIDTH + 4 + nInfoWidth
					, m_nPosY + nInfoHeight);
			}

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !_pNetwork->_TargetInfo.bIsActive )
					bTitleBarClick = FALSE;

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}

				_pUIMgr->RearrangeOrder( UI_TARGETINFO, TRUE );
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

				if( IsInside( nX, nY ) )
				{
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

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
// 070907_ttos : 심플 메뉴
#ifdef  SIMPLE_POPUP
	case WM_RBUTTONDOWN:
		{
			if( IsInside(nX,nY))
			{
				if(_pNetwork->_TargetInfo.TargetType == CHARACTER && IsInsideRect(nX, nY, m_rcTitle))
				{
					_pUIMgr->GetSimplePop()->OpenPop(_pNetwork->_TargetInfo.TargetName,
						( _pUIMgr->GetParty()->GetMemberCount() > 0 && _pUIMgr->GetParty()->AmILeader() ) ? TRUE : FALSE,	// 파티장이면 TRUE
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ? TRUE : FALSE,						// 내가 길드장일때만 TRUE
						nX, nY);
					return WMSG_SUCCESS;
				}

			}		

		}
		break;
#endif
	}

	return WMSG_FAIL;
}
