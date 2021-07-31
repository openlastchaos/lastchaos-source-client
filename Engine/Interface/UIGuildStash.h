
#ifndef	UIGUILDSTASH_H_
#define	UIGUILDSTASH_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <vector>

using namespace std;

struct STakeInfo
{
	int			m_nMonth;
	int			m_nDay;
	LONGLONG	m_llMoney;

	void SetData( int nMonth, int nDay, LONGLONG llMoney )
	{
		m_nMonth	= nMonth;
		m_nDay		= nDay;
		m_llMoney	= llMoney;

	}

};

typedef vector<STakeInfo>	VTakeInfo;


enum eGuilStashState
{
	GSS_NPC,
	GSS_VIEW,
	GSS_TAKE,
};

class CUIGuildStash : public CUIWindow
{
public:

protected:
	
	CTString m_strTitle;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar

	// UV of each part
	UIRectUV				m_rtTopL;								// UV of top background
	UIRectUV				m_rtTopM;								// UV of top background
	UIRectUV				m_rtTopR;								// UV of top background
	UIRectUV				m_rtMiddleL;							// UV of middle background
	UIRectUV				m_rtMiddleM;							// UV of middle background
	UIRectUV				m_rtMiddleR;							// UV of middle background
	UIRectUV				m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV				m_rtBottomL;							// UV of bottom background
	UIRectUV				m_rtBottomM;							// UV of bottom background
	UIRectUV				m_rtBottomR;							// UV of bottom background

	// Table
	UIRectUV3				m_rtTableTitle;							// UV of top background
	UIRectUV3				m_rtTableMiddle;						// UV of top background
	UIRectUV3				m_rtTableBottom;						// UV of top background
	
	UIRectUV				m_rtTableGrid;

		
	// Properties
	CTString			strTitle;
	eGuilStashState		GameState;
	CUIButton			m_btnOk;
	
	LONGLONG			m_llBalance;		// Balance;

	VTakeInfo			m_vTakeInfo;


public:
	CUIGuildStash();
	~CUIGuildStash();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderTake();	// Ăâ±Ý Ă˘ ·»´ő 
	void	RenderTable();


	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	
	ENGINE_API void Message( int nCommandCode, CTString strMessage, DWORD dwStyle );

	// Open
	ENGINE_API void	OpenGuildStash();

	void OpenTake( LONGLONG nBalance );
	void OpenView();

	void	CloseStash();

	void AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney );
	void ResetTakeInfo();
	
	void ErrorProc( LONG lErrorCode, UBYTE ubType );
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	
	
	
};


#endif	// UIGUILDSTASH_H_


