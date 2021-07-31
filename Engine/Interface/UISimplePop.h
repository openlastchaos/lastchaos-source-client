#ifndef	UISIMPLEPOP_H_
#define	UISIMPLEPOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UITrackPopup.h>

typedef enum _SimplePopState
{
	SPSO_NAME				= 0,			// 유저이름
	SPSO_WHISPER			= 1,			// 귓속말
	SPSO_FRIEND				= 2,			// 친구 등록
	SPSO_TRADE				= 3,			// 교환 신청
	SPSO_PARTY				= 4,			// 파티 신청 (일반 / 입수우선 / 전투형)
	SPSO_PARTY_OUT			= 5,			// 파티 강퇴
	SPSO_PARTY_JANG			= 6,			// 파티장 위임
	SPSO_GUILD_IN			= 7,			// 길드 가입
	SPSO_GUILD_WAR_START	= 8,			// 길드전투 신청
	SPSO_GUILD_WAR_END		= 9,			// 길드전투 중단
	SPSO_CANCEL				= 10,			// 취소
	SPSO_END				= 11,			// SimplePopStateOne End

	SPST_START				= 12,			// SimplePopStateTwo End
	SPST_PARTY_JOIN_A		= SPST_START,	// 일반 파티 신청
	SPST_PARTY_JOIN_B		= 13,			// 입수우선 파티 신청
	SPST_PARTY_JOIN_C		= 14,			// 전투형 파티 신청
} SIMPLE_POP_STATE;

class CUISimplePop : public CUITrackPopup
{
public:
	INDEX m_iExeNum[SPSO_END];

	CUITrackPopup m_tpList;

	CTextureData*			m_ptdMenuTexture;			//툴팁에 사용될 화살표를 위한 텍스쳐
	UIRectUV				m_rtArrow;					//화살표 텍스쳐 좌표

public:
	CUISimplePop();
	~CUISimplePop();

	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	ENGINE_API void OpenPop(CTString strName, BOOL bParty, BOOL bGuild, INDEX ixPos, INDEX iyPos);
	void ClosePop();

	void Render();

	void SetExeList(CTString strUserName, BOOL bParty, BOOL bGuild);

	void Execution(INDEX exeNum);

	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	// UISIMPLEPOP_H_