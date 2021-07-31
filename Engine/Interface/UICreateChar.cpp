#include "stdh.h"
#include <Engine/Interface/UICreateChar.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Rendering/Render.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/World/World.h>
#include <Engine/GameState.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Interface/UIFiltering.h>
#include <Engine/JobInfo.h>
#include <Engine/Math/Placement.h>
#include <Engine/Interface/UIManager.h>

extern INDEX g_iCountry;


#define MAXJOB	(6)
//#define MAXJOB ((g_iCountry == GERMANY) ? 5 : 6 ) 

// Character Position Marker List
static int _aiMarkerEntities[TOTAL_JOB] =
{
	3381,		// Titan
	3383,		// Knight
	3411,		// Healer
	3382,		// Mage	
	3952,		// Rogue	
	3954,		// Sorcerer
};

static int _aiBasicWearing[TOTAL_JOB][6] =
{
	{			// TITAN
		2,		// 상의
		12,		// 칼
		3,		// 하의
		-1,		// 방패
		4,		// 장갑
		8,		// 부츠		
	},
	{			// KNIGHT
		34,		// 상의
		48,		// 칼
		38,		// 하의
		49,		// 방패
		39,		// 장갑
		41,		// 부츠
	},
	{			// HEALER
		26,		// 상의
		50,		// 활
		28,		// 하의
		-1,		// 방패
		30,		// 장갑
		32,		// 부츠		
	},
	{			// MAGE
		266,	// 상의
		356,	// 칼
		18,		// 하의
		-1,		// 방패
		22,		// 장갑
		20,		// 부츠				
	},
	{			// ROGUE
		524,	// 상의
		528,	// 칼
		525,	// 하의
		-1,		// 방패
		527,	// 장갑
		526,	// 부츠
	},
	{			// SORCERER
		1000,	// 상의
		976,	// 칼
		1010,	// 하의
		-1,		// 방패
		1020,	// 장갑
		1030,	// 부츠
	},
};


// Maximum size of character information
static	int	_iMaxCharInfoChar = 0;

#define MIN_NAME_LEN	(4)
#define MAX_NAME_LEN	(16)

// ----------------------------------------------------------------------------
// Name : CUICreateChar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICreateChar::CUICreateChar() :
m_iSelectedJob(0),
//m_pCurChar(NULL),
m_fZDelta(0.0f),
m_fRotDelta(0.0f),
m_sbSelectedHair(0),
m_sbSelectedFace(0),
m_iSelectedColor(0),
m_nCurInfoLines(0),
m_bAutoRotate(FALSE),
m_pWorld(NULL)
{
}

// ----------------------------------------------------------------------------
// Name : ~CUICreateChar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICreateChar::~CUICreateChar()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICreateChar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxCharInfoChar = MAX_CHARINFO_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Create creating character texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\CreateCharacter.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtCharInfoTop.SetUV( 0, 0, 255, 386, fTexWidth, fTexHeight );
	m_rtCharInfoBottom.SetUV( 0, 390, 255, 393, fTexWidth, fTexHeight );

	// OK Button(Create character)
	m_btnOK.Create( this, _S( 138, "결정" ), 0, 0, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 397, 63, 418, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 397, 129, 418, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON ); 
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Back Button
	m_btnCancel.Create( this, _S( 139, "취소" ), 0, 0, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 397, 63, 418, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 397, 129, 418, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// ZoomIn Button
	m_btnZoomIn.Create( this, CTString( "" ), 0, 0, 38, 37 );
	m_btnZoomIn.SetUV( UBS_IDLE, 151, 418, 189, 455, fTexWidth, fTexHeight );
	m_btnZoomIn.SetUV( UBS_CLICK, 151, 418, 189, 455, fTexWidth, fTexHeight );
	m_btnZoomIn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnZoomIn.CopyUV( UBS_IDLE, UBS_DISABLE );

	// ZoomOut Button
	m_btnZoomOut.Create( this, CTString( "" ), 0, 0, 38, 37 );
	m_btnZoomOut.SetUV( UBS_IDLE, 151, 464, 189, 501, fTexWidth, fTexHeight );
	m_btnZoomOut.SetUV( UBS_CLICK, 151, 464, 189, 501, fTexWidth, fTexHeight );
	m_btnZoomOut.CopyUV( UBS_IDLE, UBS_ON );
	m_btnZoomOut.CopyUV( UBS_IDLE, UBS_DISABLE );

	// RotateToLeft Character Button
	m_btnRotateToLeft.Create( this, CTString( "" ), 0, 0, 38, 37 );
	m_btnRotateToLeft.SetUV( UBS_IDLE, 209, 441, 247, 478, fTexWidth, fTexHeight );
	m_btnRotateToLeft.SetUV( UBS_CLICK, 209, 441, 247, 478, fTexWidth, fTexHeight );
	m_btnRotateToLeft.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRotateToLeft.CopyUV( UBS_IDLE, UBS_DISABLE );

	// RotateToRight Button
	m_btnRotateToRight.Create( this, CTString( "" ), 0, 0, 38, 37 );
	m_btnRotateToRight.SetUV( UBS_IDLE, 94, 441, 132, 478, fTexWidth, fTexHeight );
	m_btnRotateToRight.SetUV( UBS_CLICK, 94, 441, 132, 478, fTexWidth, fTexHeight );
	m_btnRotateToRight.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRotateToRight.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev Job
	m_btnJobPrev.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnJobPrev.SetUV( UBS_IDLE, 0, 421, 13, 434, fTexWidth, fTexHeight );
	m_btnJobPrev.SetUV( UBS_CLICK, 0, 436, 13, 449, fTexWidth, fTexHeight );
	m_btnJobPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnJobPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next Job
	m_btnJobNext.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnJobNext.SetUV( UBS_IDLE, 16, 421, 29, 434, fTexWidth, fTexHeight );
	m_btnJobNext.SetUV( UBS_CLICK, 16, 436, 29, 449, fTexWidth, fTexHeight );
	m_btnJobNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnJobNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev Head
	m_btnFacePrev.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnFacePrev.SetUV( UBS_IDLE, 0, 421, 13, 434, fTexWidth, fTexHeight );
	m_btnFacePrev.SetUV( UBS_CLICK, 0, 436, 13, 449, fTexWidth, fTexHeight );
	m_btnFacePrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnFacePrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next Head
	m_btnFaceNext.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnFaceNext.SetUV( UBS_IDLE, 16, 421, 29, 434, fTexWidth, fTexHeight );
	m_btnFaceNext.SetUV( UBS_CLICK, 16, 436, 29, 449, fTexWidth, fTexHeight );
	m_btnFaceNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnFaceNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev Hair
	m_btnHairPrev.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnHairPrev.SetUV( UBS_IDLE, 0, 421, 13, 434, fTexWidth, fTexHeight );
	m_btnHairPrev.SetUV( UBS_CLICK, 0, 436, 13, 449, fTexWidth, fTexHeight );
	m_btnHairPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHairPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next Hair
	m_btnHairNext.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnHairNext.SetUV( UBS_IDLE, 16, 421, 29, 434, fTexWidth, fTexHeight );
	m_btnHairNext.SetUV( UBS_CLICK, 16, 436, 29, 449, fTexWidth, fTexHeight );
	m_btnHairNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHairNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev Face
	m_btnHeadPrev.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnHeadPrev.SetUV( UBS_IDLE, 0, 421, 13, 434, fTexWidth, fTexHeight );
	m_btnHeadPrev.SetUV( UBS_CLICK, 0, 436, 13, 449, fTexWidth, fTexHeight );
	m_btnHeadPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHeadPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next Face
	m_btnHeadNext.Create( this, CTString( "" ), 0, 0, 13, 13 );
	m_btnHeadNext.SetUV( UBS_IDLE, 16, 421, 29, 434, fTexWidth, fTexHeight );
	m_btnHeadNext.SetUV( UBS_CLICK, 16, 436, 29, 449, fTexWidth, fTexHeight );
	m_btnHeadNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHeadNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Character name edit box
	m_ebCharName.Create( this, 0, 0, 88, 13, MAX_NAME_LEN );
	m_ebCharName.SetReadingWindowUV( 34, 423, 51, 439, fTexWidth, fTexHeight );
	m_ebCharName.SetCandidateUV( 34, 423, 51, 439, fTexWidth, fTexHeight );
	m_ebCharName.SetFocus( TRUE );

	// Description of character
	m_astrDescription[0] = _S( 129, " 겉보기에는 다소 덩치 큰 사람처럼 보일 수 있으나, 겉모습으로는 상상할 수 없을 정도의 엄청난 괴력, 체력과 함께 전투 상황에 대한 기민한 판단력까지 지니고 있는 타고난 전사." );
	m_astrDescription[1] = _S( 130, "자신의 힘을 기르고 육체를 단련하는 것을 삶의 목표로 여기고 있으며 목숨이 왔다갔다 거리는 전장을 최고의 단련장소라 여기고 끊임없는 전투를 즐기며 살아가는 삶을 보낸다." );
	m_astrDescription[2] = _S( 131, " 빛의 신을 모시는 성스러운 기사로 단순히 무엇보다 악으로 물들어가는 혼돈의 시대를 살아가기 위해서만이 아닌 자신의 명예와 소중히 여기는 것을 지키기 위해 계속 단련하는 존재." );
	m_astrDescription[3] = _S( 132, "자신을 포함한 주변의 동료들의 상황을 언제나 보호하면서 싸우는 전법을 사용하기에 공격 기술 이상가는 다양한 방어기술을 지니고 있으며 이를 활용하여 전투를 항상 승리로 이끈다." );
	m_astrDescription[4] = _S( 133, " 정령의 힘을 이용하여 다양한 회복, 치유에 관련된 온화한 마법을 사용하는 것이 특기인 하이엘프 힐러는 전투 중 상처를 입은 동료와 자연을 치유하는 나날을 보내고 있다." );
	m_astrDescription[5] = _S( 134, "접근전을 좋아하지 않는 그들은 주로 활을 사용하여 원거리로부터 전투를 지원하는데 바람의 정령에게 도움을 받기 때문인지 여느 숙련된 궁수에 못지 않는 강력함을 지닌다." );
	m_astrDescription[6] = _S( 449, "정령의 가호를 받고 있는 엘프들과 달리 자신 스스로의 노력에 의해 습득한 지식에 의해 마나의 흐름을 통제하고 이를 응용하여 자유롭게 사용하는 자들이 이들 메이지이다. " );
	m_astrDescription[7] = _S( 450, "폭발적인 위력을 자랑하는 공격 마법과 상대를 혼란시키는 저주 마법을 주 특기로 사용하는 메이지는 그에게 적대하는 상대를 전멸, 제거하는데 그 힘을 아끼지 않고 사용한다. " );
	m_astrDescription[8] = _S( 1025, "무엇에도 얽매이지 않고 바람보다도 자유롭게 살아가는 존재. 정령 실프의 축복을 받아 태어난 이들은 매우 빠른 몸놀림과 뛰어난 시야를 지니며 그와 함께 자유분방한 사고방식과 여행을 좋아하는 성격을 타고났다고 전해진다." );		
	m_astrDescription[9] = _S( 1026, "빠른 몸놀림과 시야를 활용할 수 있는 단검과 석궁을 주로 사용하는 로그는 낮은 체력을 커버하기 위해 공격 하나하나를 적의 약점을 노려 치명상을 입히거나 원거리에서 적에 대해 아군을 서포트하는 것에 주력한다." );	
	m_astrDescription[10] = _S( 1027, "치유를 담당하는 하이엘프 힐러와 달리 하이엘프 소서러는 자연을 상처 입히거나 그들의 계율을 어기는 자에 대해 강력한 마법으로 제재를 가하는 역할을 맡고 있다." );	
	m_astrDescription[11] = _S( 1028, "자연 상의 마나를 에너지의 형태로 변환시켜 사용하는 메이지와 달리 소서러는 4대 정령의 힘을 빌은 강력한 4대 속성의 마법을 사용한다." );	
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICreateChar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	m_rcCharInfo.SetRect( pixMaxI - CREATECHAR_INFO_WIDTH, pixMinJ, pixMaxI, pixMaxJ );

	int	nX = ( pixMaxI + pixMinI - CREATECHAR_CONTROL_WIDTH ) / 2;
	int	nY = pixMaxJ - CREATECHAR_CONTROL_HEIGHT - 5;
	m_rcCharControl.SetRect( nX, nY, nX + CREATECHAR_CONTROL_WIDTH, nY + CREATECHAR_CONTROL_HEIGHT );

	// OK & cancel button
	m_btnOK.SetPos( pixMaxI - 138, 360 );
	m_btnCancel.SetPos( pixMaxI - 72, 360 );

	// Job button
	m_btnJobPrev.SetPos( pixMaxI - 172, 264 );
	m_btnJobNext.SetPos( pixMaxI - 89, 264 );

	// Head button
	m_btnFacePrev.SetPos( pixMaxI - 172, 283 );
	m_btnFaceNext.SetPos( pixMaxI - 89, 283 );

	// Hair button
	m_btnHairPrev.SetPos( pixMaxI - 172, 303 );
	m_btnHairNext.SetPos( pixMaxI - 89, 303 );

	// Face button
	m_btnHeadPrev.SetPos( pixMaxI - 172, 323 );
	m_btnHeadNext.SetPos( pixMaxI - 89, 323 );

	// Character name edit box
	m_ebCharName.SetPos( pixMaxI - 168, 229 );

	// ZoomIn Button
	m_btnZoomIn.SetPos( m_rcCharControl.Left + 57, m_rcCharControl.Top );

	// ZoomOut Button
	m_btnZoomOut.SetPos( m_rcCharControl.Left + 57, m_rcCharControl.Top + 46 );

	// RotateToLeft Character Button
	m_btnRotateToLeft.SetPos( m_rcCharControl.Left, m_rcCharControl.Top + 23 );

	// RotateToRight Button
	m_btnRotateToRight.SetPos( m_rcCharControl.Left + 115, m_rcCharControl.Top + 23 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICreateChar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUICreateChar::Reset()
{	
	Lock(FALSE);
	m_iSelectedJob	= 0;	
	m_fZDelta		= 0.0f;
	m_fRotDelta		= 0.0f;

	m_ebCharName.ResetString();

	// 카메라 설정, 캐릭터 위치 설정.	
	for(int i = 0; i < MAXJOB; ++i)
	{
		const int iMarker			= _aiMarkerEntities[i];
		CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);	
		penMarker->SetSkaModel(JobInfo().GetFileName(i));
		
		CModelInstance* pMI	= penMarker->GetModelInstance();
		if(pMI)
		{		
			INDEX idAttackIdle		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( i, ANIM_IDLE ) );
			pMI->AddAnimation(idAttackIdle, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1, 0);
		}
	}
	
	if(g_iCountry == BRAZIL) 
		CharWearing();

	ChangeSelJob();
}

// 브라질 캐릭생성시 옷입히기
void CUICreateChar::CharWearing()
{
	for(int i = 0 ; i < MAXJOB; ++i)
	{
		// 카메라 설정, 캐릭터 위치 설정.	
		const int iMarker			= _aiMarkerEntities[i];
		CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);	
		
		CModelInstance* pMI	= penMarker->GetModelInstance();
		
		// 슬롯의 캐릭터에 장비를 장착한 모습을 보여줘야 하는 부분...
		// 상의 무기 하의 방패 장갑 신발
		if(pMI)
		{
			
			for(int j = 0; j < 6; ++j)
			{
				const SLONG lWear = _aiBasicWearing[i][j];
				if(lWear > 0)
				{
					CItemData &ID = _pNetwork->GetItemData(lWear);
					_pGameState->DeleteDefaultArmor(pMI, ID.GetWearingPosition(), i);
					_pGameState->WearingArmor(pMI, ID);					
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICreateChar::Render()
{	
	extern INDEX	sam_bWideScreen;
	CDrawPort		*pdp = _pUIMgr->GetDrawPort();

	// 캐릭터의 위치 설정.
	//if(m_pCurChar)
	//{
		//CPlacement3D plCharacter = m_plChar;
		/*
		// 모델을 렌더링함.	
		CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
		extern FLOAT sam_fPlayerOffset;	
		//pl.pl_OrientationAngle	= ANGLE3D(6.0f * tvNow.GetSeconds(), 0.0f, 0.0f);
		//pl.pl_PositionVector	= FLOAT3D(-1.0f, -1.0f, 0.0f);
		*/
		//if(m_bAutoRotate)
		//{
			//plCharacter.pl_OrientationAngle(1) += m_fRotDelta;
		//}
		//m_pCurChar->SetPlacement(plCharacter);
		//}

	// Set create character textire
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Character information
	// Top
	_pUIMgr->GetDrawPort()->AddTexture( m_rcCharInfo.Left, m_rcCharInfo.Top,
										m_rcCharInfo.Right, m_rcCharInfo.Top + CREATECHAR_INFO_HEIGHT,
										m_rtCharInfoTop.U0, m_rtCharInfoTop.V0,
										m_rtCharInfoTop.U1, m_rtCharInfoTop.V1,
										0xFFFFFFFF );
	// Bottom
	_pUIMgr->GetDrawPort()->AddTexture( m_rcCharInfo.Left, m_rcCharInfo.Top + CREATECHAR_INFO_HEIGHT,
										m_rcCharInfo.Right, m_rcCharInfo.Bottom,
										m_rtCharInfoBottom.U0, m_rtCharInfoBottom.V0,
										m_rtCharInfoBottom.U1, m_rtCharInfoBottom.V1,
										0xFFFFFFFF );

	// Character name input box
	m_ebCharName.Render();

	// Job button
	m_btnJobPrev.Render();
	m_btnJobNext.Render();

	// Head button
	m_btnFacePrev.Render();
	m_btnFaceNext.Render();

	// Hair button
	m_btnHairPrev.Render();
	m_btnHairNext.Render();

	// Face button
	//m_btnHeadPrev.Render();
	//m_btnHeadNext.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Character control
	/*
	// Zoom in button
	m_btnZoomIn.Render();

	// Zoom out button
	m_btnZoomOut.Render();

	// Rotate to left button
	m_btnRotateToLeft.Render();

	// Rotate to right
	m_btnRotateToRight.Render();
	*/

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Render character information
	int	nX = m_rcCharInfo.Left + CREATECHAR_DESC_SX;
	int	nY = m_rcCharInfo.Top + CREATECHAR_DESC_SY;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strCharInfo[iInfo], nX, nY, m_colCharInfo[iInfo] );
		nY += _pUIFontTexMgr->GetLineHeight();
	}	
	
	// Prepare Rendering Text
	nX = m_rcCharInfo.Left;
	nY = m_rcCharInfo.Top;
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 140, "캐릭터 생성" ),
										nX + CREATECHAR_TITLE_OFFSETX, nY + CREATECHAR_TITLE_OFFSETY );
	_pUIMgr->GetDrawPort()->PutTextExRX( _S( 71, "아이템 레벨이 맞지 않습니다." ),
											nX + CREATECHAR_NAME_RX, nY + CREATECHAR_NAME_SY );
	_pUIMgr->GetDrawPort()->PutTextExRX( _S( 141, "직업" ),
											nX + CREATECHAR_NAME_RX, nY + CREATECHAR_JOB_SY );
	_pUIMgr->GetDrawPort()->PutTextExRX( _S( 144, "얼굴" ),		// 얼굴
											nX + CREATECHAR_NAME_RX, nY + CREATECHAR_FACE_SY );
	_pUIMgr->GetDrawPort()->PutTextExRX( _S( 143, "헤어" ),		// 헤어
											nX + CREATECHAR_NAME_RX, nY + CREATECHAR_HAIR_SY );
	//_pUIMgr->GetDrawPort()->PutTextExRX( _S( 142, "머리" ),		// 머리
	//										nX + CREATECHAR_NAME_RX, nY + CREATECHAR_HEAD_SY );

	// Print Job
	_pUIMgr->GetDrawPort()->PutTextExCX( JobInfo().GetName(m_iSelectedJob),
											nX + CREATECHAR_JOB_MAIN_CY, nY + CREATECHAR_JOB_SY );
	// Print Face
	CTString strTemp;
	strTemp.PrintF("%d", m_sbSelectedFace + 1);
	_pUIMgr->GetDrawPort()->PutTextExCX( strTemp,
											nX + CREATECHAR_JOB_MAIN_CY, nY + CREATECHAR_FACE_SY );

	strTemp.PrintF("%d", m_sbSelectedHair + 1);
	// Print Hair
	_pUIMgr->GetDrawPort()->PutTextExCX( strTemp,
											nX + CREATECHAR_JOB_MAIN_CY, nY + CREATECHAR_HAIR_SY );

	// Print Head
	//_pUIMgr->GetDrawPort()->PutTextExCX( CTString( "1" ),
	//										nX + CREATECHAR_JOB_MAIN_CY, nY + CREATECHAR_FACE_SY );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Reading window
	if( m_ebCharName.DoesShowReadingWindow() )
	{
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebCharName.RenderReadingWindow();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//-----------------------------------------------------------------------------
void CUICreateChar::PressOKBtn()
{	
	// Get input string
	if( m_ebCharName.GetString() )
	{
		m_strCharName = m_ebCharName.GetString();	
		
		// 공백 체크.		
		for(const char *chr = m_strCharName.str_String; *chr != 0; chr++)
		{
			if( (*chr) == ' ' || (*chr) == '\t' || (*chr) == '\n' || (*chr) == '\r' || 
				(*chr) == '%' || (*chr) == '#' || (*chr) == '&' || (*chr) == '?' || (*chr) == '+' || (*chr) == '=' ||
				(g_iCountry == HONGKONG && !CheckCharacterHK(chr))) // 홍콩 특수문자는 입력 불가처리
			{
				_pUIMgr->CloseMessageBox(MSGCMD_CREATE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
					UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
				CTString	strMessage = _S( 146, "캐릭터명에 공백이 들어갈 수 없습니다." );
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				
				m_ebCharName.ResetString();
				return;
			}
			if(((*chr) < 48 || (*chr) > 122) && (g_iCountry == MALAYSIA || g_iCountry == USA))
			{
				_pUIMgr->CloseMessageBox(MSGCMD_CREATE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
					UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
				CTString	strMessage = _S(2980, "아이디는 영문과 숫자만 허용됩니다." );
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				
				m_ebCharName.ResetString();
				return;
			}
		}
	}
//#define RESTRICT_SOUND 	

	_pSound->Mute();
	CreateCharacter();

#ifdef RESTRICT_SOUND
	extern BOOL _bNTKernel;
	//if( _bNTKernel )
		_pSound->UpdateSounds();
	#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUICreateChar::KeyMessage( MSG *pMsg )
{
	// If character creation is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{	
		PressOKBtn();
		return WMSG_SUCCESS;
	}
	
	if( m_ebCharName.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUICreateChar::IMEMessage( MSG *pMsg )
{

	// 이기환 수정 시작 (11.20) : IME 루틴 조정
	return m_ebCharName.IMEMessage( pMsg );
	// 이기환 수정 끝
	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUICreateChar::CharMessage( MSG *pMsg )
{
	if( m_ebCharName.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICreateChar::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

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
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnJobPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnJobNext.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnFacePrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnFaceNext.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnHairPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnHairNext.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnHeadPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnHeadNext.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			/*else if( m_btnZoomIn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnZoomOut.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnRotateToLeft.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnRotateToRight.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;*/
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInsideRect( nX, nY, m_rcCharInfo ) )
			{
				if( m_ebCharName.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnJobPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnJobNext.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnFacePrev.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnFaceNext.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnHairPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnHairNext.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnHeadPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnHeadNext.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_CREATE_CHAR, TRUE );
				return WMSG_SUCCESS;
			}
			/*else if( IsInsideRect( nX, nY, m_rcCharControl )
			{
				else if( m_btnZoomIn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnZoomOut.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnRotateToLeft.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnRotateToRight.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_CREATE_CHAR, TRUE );
				return WMSG_SUCCESS;
			}*/
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInsideRect( nX, nY, m_rcCharInfo ) )
			{
				if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						PressOKBtn();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_pUIMgr->SetUIGameState(UGS_SELCHAR);
						_pGameState->BackToSelChar();
					}
				}			
				else if( ( wmsgResult = m_btnJobPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{	
					if( wmsgResult == WMSG_COMMAND )
					{
						m_iSelectedJob--;
						//_pSound->Mute();						
						ChangeSelJob();
						//_pSound->UpdateSounds();
						_pGameState->SetCameraByJob(m_iSelectedJob);
					}
				}
				else if( ( wmsgResult = m_btnJobNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_iSelectedJob++;
						//_pSound->Mute();
						ChangeSelJob();
						//_pSound->UpdateSounds();
						_pGameState->SetCameraByJob(m_iSelectedJob);
					}
				}
				else if( ( wmsgResult = m_btnFacePrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_sbSelectedFace--;						

						if(m_sbSelectedFace < 0) 
							m_sbSelectedFace = TOTAL_HEAD - 1;
						m_sbSelectedFace	= m_sbSelectedFace % TOTAL_HEAD;

						const int iMarker			= _aiMarkerEntities[m_iSelectedJob];
						CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);	
						
						CModelInstance* pMI			= penMarker->GetModelInstance();

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(pMI, m_iSelectedJob, m_sbSelectedFace);
					}
				}
				else if( ( wmsgResult = m_btnFaceNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_sbSelectedFace++;
						
						m_sbSelectedFace	= m_sbSelectedFace % TOTAL_HEAD;

						const int iMarker			= _aiMarkerEntities[m_iSelectedJob];
						CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);							
						CModelInstance* pMI			= penMarker->GetModelInstance();

						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(pMI, m_iSelectedJob, m_sbSelectedFace);
					}
				}
				else if( ( wmsgResult = m_btnHairPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_sbSelectedHair--;

						if(m_sbSelectedHair < 0) 
							m_sbSelectedHair = TOTAL_HAIR - 1;
						m_sbSelectedHair	= m_sbSelectedHair % TOTAL_HAIR;

						const int iMarker			= _aiMarkerEntities[m_iSelectedJob];
						CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);							
						CModelInstance* pMI			= penMarker->GetModelInstance();
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI, m_iSelectedJob, m_sbSelectedHair);
						//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairTexture(pMI, m_iSelectedJob, m_sbSelectedHair);
					}
				}
				else if( ( wmsgResult = m_btnHairNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_sbSelectedHair++;						
						
						m_sbSelectedHair	= m_sbSelectedHair % TOTAL_HAIR;

						const int iMarker			= _aiMarkerEntities[m_iSelectedJob];
						CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);							
						CModelInstance* pMI			= penMarker->GetModelInstance();
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI, m_iSelectedJob, m_sbSelectedHair);
						//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairTexture(pMI, m_iSelectedJob, m_sbSelectedHair);
					}
				}
				else if( ( wmsgResult = m_btnHeadPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
					}
				}
				else if( ( wmsgResult = m_btnHeadNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
					}
				}

				return WMSG_SUCCESS;
			}
			else if( IsInsideRect( nX, nY, m_rcCharControl ) )
			{
				if( ( wmsgResult = m_btnZoomIn.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_fZDelta += 0.6f;
					}
				}
				else if( ( wmsgResult = m_btnZoomOut.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_fZDelta -= 0.6f;
					}
				}
				else if( ( wmsgResult = m_btnRotateToLeft.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//m_bAutoRotate = !m_bAutoRotate;
						m_fRotDelta -= 10.0f;
					}
				}
				else if( ( wmsgResult = m_btnRotateToRight.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//m_bAutoRotate = !m_bAutoRotate;
						m_fRotDelta += 10.0f;
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
void CUICreateChar::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
}

//-----------------------------------------------------------------------------
// Purpose: Prev 나 Next를 눌러서 선택된 캐릭터가 변경된 경우.
//-----------------------------------------------------------------------------
void CUICreateChar::ChangeSelJob()
{
//강동민 수정 시작 클로즈 2차 작업	08.18
	// FIXME : 모든 캐릭터가 다 나오면 주석을 풀것.
	//if(m_iSelectedJob < 0) 
	//	m_iSelectedJob = TOTAL_JOB - 1;
	//m_iSelectedJob	= m_iSelectedJob % TOTAL_JOB;

	// FIXME : 클로즈 2차때 나이트와 힐러를 보여주기 위한 부분.
	if(m_iSelectedJob < 0) 
		m_iSelectedJob = MAXJOB - 1;

	if(m_iSelectedJob > MAXJOB - 1)
		m_iSelectedJob = 0;
	
	m_sbSelectedHair= 0;
	m_sbSelectedFace= 0;
	m_iSelectedColor= 0;
	
//강동민 수정 끝 클로즈 2차 작업		08.18

	m_fRotDelta		= 0.0f;
	m_fZDelta		= 0.0f;
	if(!m_pWorld)			return;
	
	// 카메라 설정, 캐릭터 위치 설정.	
	const int iMarker			= _aiMarkerEntities[m_iSelectedJob];
	CEntity *penMarker			= m_pWorld->EntityFromID(iMarker);	
	//penMarker->SetSkaModel(_aJobFileName[m_iSelectedJob]);

	CModelInstance* pMI	= penMarker->GetModelInstance();
	if(pMI)
	{
		//pMI->mi_iFaceMesh	= -1;
		//pMI->mi_iFaceTexture= -1;
		//pMI->mi_iHairMesh	= -1;
		//pMI->mi_iHairTexture= -1;

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(pMI, m_iSelectedJob, 0);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI, m_iSelectedJob, 0);

		pMI->mi_iFaceMesh	= -1;
		pMI->mi_iHairMesh	= -1;

		if(_pNetwork->wo_iNumOfItem > 0)
		{
			// 슬롯의 캐릭터에 장비를 장착한 모습을 보여줘야 하는 부분...
			// 상의 무기 하의 방패 장갑 신발
			for(int i = 0; i < 6; ++i)
			{
				const SLONG lWear = _aiBasicWearing[m_iSelectedJob][i];
				if(lWear > 0)
				{
					CItemData &ID = _pNetwork->GetItemData(lWear);
					_pGameState->DeleteDefaultArmor(pMI, ID.GetWearingPosition(), m_iSelectedJob);
					_pGameState->WearingArmor(pMI, ID);					
				}
			}
		}

		//INDEX idWalk		= ska_GetIDFromStringTable(_afnIdleAnim[m_iSelectedJob]);
		//pMI->AddAnimation(idWalk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		// 선택한 캐릭터에 대한 설명을 보여줍니다.
		GetCharInfo();
	}	
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터를 생성합니다.
//-----------------------------------------------------------------------------
void CUICreateChar::CreateCharacter()
{
	CTString strMessage;
	//strcpy ( strMessage.str_String, m_strCharName.str_String );
	strMessage = m_strCharName;
	if(!_pUIMgr->checkName(strMessage,0)) return;
	
	if(_pNetwork->m_bSendMessage)
		return;

	_pNetwork->SendCreateCharacter( m_strCharName, m_iSelectedJob, m_sbSelectedHair + 1, m_sbSelectedFace + 1 );
	_pUIMgr->Lock(TRUE);
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터의 설명을 설정함.
// Input  : &strItemInfo - 
//			colItemInfo - 
//-----------------------------------------------------------------------------
void CUICreateChar::AddCharInfoString( CTString &strCharInfo, COLOR colCharInfo )
{
	if( m_nCurInfoLines >= MAX_CHARINFO_LINE )
		return;

	// Get length of string
	INDEX	nLength = strCharInfo.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strCharInfo);
		INDEX	nChatMax= (_iMaxCharInfoChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strCharInfo[iPos] == '\n' || strCharInfo[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_strCharInfo[m_nCurInfoLines] = strCharInfo;
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;
			}
			else
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( iPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddCharInfoString( strTemp, colCharInfo );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxCharInfoChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strCharInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strCharInfo[iPos] == '\n' || strCharInfo[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( nSplitPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddCharInfoString( strTemp, colCharInfo );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( iPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddCharInfoString( strTemp, colCharInfo );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength < _iMaxCharInfoChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strCharInfo[iPos] == '\n' || strCharInfo[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_strCharInfo[m_nCurInfoLines] = strCharInfo;
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;
			}
			else
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( iPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddCharInfoString( strTemp, colCharInfo );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxCharInfoChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strCharInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strCharInfo[iPos] == '\n' || strCharInfo[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( nSplitPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddCharInfoString( strTemp, colCharInfo );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strCharInfo.Split( iPos, m_strCharInfo[m_nCurInfoLines], strTemp );
				m_colCharInfo[m_nCurInfoLines++] = colCharInfo;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddCharInfoString( strTemp, colCharInfo );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUICreateChar::GetCharInfo()
{
	m_nCurInfoLines = 0;

	AddCharInfoString( m_astrDescription[m_iSelectedJob * 2], 0xCCCCCCFF );
	AddCharInfoString( m_astrDescription[m_iSelectedJob * 2 + 1], 0xCCCCCCFF );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUICreateChar::Lock(BOOL bLock)
{
	m_ebCharName.SetEnable(!bLock);
	m_btnOK.SetEnable(!bLock);
	m_btnJobPrev.SetEnable(!bLock);
	m_btnJobNext.SetEnable(!bLock);
	m_btnFacePrev.SetEnable(!bLock);
	m_btnFaceNext.SetEnable(!bLock);
	m_btnHairPrev.SetEnable(!bLock);
	m_btnHairNext.SetEnable(!bLock);
	m_btnHeadPrev.SetEnable(!bLock);
	m_btnHeadNext.SetEnable(!bLock);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUICreateChar::SetBackgroundWorld( CWorld *pWorld )
{
	m_pWorld = pWorld;
}

/************************************************************************/
/* Name : BOOL CheckChracterHK                                          */
/************************************************************************/
BOOL CUICreateChar::CheckCharacterHK(const char* chHK)
{
	unsigned char chTemp1 = (*chHK);
	unsigned char chTemp2;

	if ( ((chTemp1) >= 0x80 && (chTemp1) <= 0xA0) || (chTemp1) == 0xC7 || (chTemp1) == 0xC8 || ((chTemp1) >= 0xFA && (chTemp1) <= 0xFF) )
	{
		return FALSE;
	}
	else if ( (chTemp1) == 0xF9 )
	{
		chTemp2 = (*(chHK+1));

		if ( (chTemp2) >= 0xDD && (chTemp2) <= 0xFF )
		{
			return FALSE;
		}
	}
	else
	{
		chTemp2 = (*(chHK+1));

		if ( (chTemp2) >= 0x7F && (chTemp2) <= 0xA0 )
		{
			return FALSE;
		}
	}

	if (chTemp1 >= 0x80)
	{
		chHK++;
	}

	return TRUE;
}