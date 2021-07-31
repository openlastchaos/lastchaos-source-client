// ----------------------------------------------------------------------------
//  File : UIRadar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIRADAR_H_
#define	UIRADAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICheckButton.h>


// Radar option
enum RadarOption
{
	RADAR_PARTY			= 0,
	RADAR_GUILD,
	RADAR_NPC,
	RADAR_HELPNPC,
	RADER_QUESTNPC,
	RADER_QUESTSUCCESS,
	RADAR_MOB,
	RADAR_OPTION_TOTAL,
};


// Define position
#define	RADAR_CENTER_OFFSETX			71
#define	RADAR_CENTER_OFFSETY			115
#define	RADAR_COORD_TEXT_CX				90
#define	RADAR_COORD_TEXT_SY				4
#define	RADAR_OPTION_BASE_WIDTH			35
#define	RADAR_OPTION_SY				2
#define	RADAR_OPTION_BTN_SX				-1
#define	RADAR_OPTION_BTN_SY				8
#define	RADAR_OPTION_ICON_SY			14
#define	RADAR_OPTION_OFFSETY			16


// Define size of radar
#define	RADAR_COMPASS_RADIUS			50.0f
#define	RADAR_VIEW_RADIUS				55.0f
#define	RADAR_VIEW_DISTANCE				70.0f
#define	RADAR_WIDTH						140
#define	RADAR_HEIGHT					53


// ----------------------------------------------------------------------------
// Name : CUIRadar
// Desc :
// ----------------------------------------------------------------------------
class CUIRadar : public CUIWindow
{
	typedef struct _SSignal
	{
		FLOAT	fX, fZ;
		DOUBLE	dStartTime;
		BOOL	bVisible;
	} SSignal;
	
protected:
	// Time 
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	LONG m_startTime;
	int m_cipher;


	// Signal 
	SSignal	m_Signal;

	// Controls
	CUIButton			m_btnOption;						// Option button
	CUIButton			m_btnMap;							// Map button
	CUIButton			m_btnSignal;							// Map button

	CUICheckButton		m_cbtnOption[RADAR_OPTION_TOTAL];	// Option check buttons
	BOOL				m_bShowOptionPopup;					// If popup of radar option is shown or not
	BOOL				m_bShowToolTip;						// If tool tip is shown or not
	BOOL				m_bSignalBtnOn;						// 시그널 버튼 사용 
	BOOL				m_bInsideMouse;						// 마우스가 해당 윈도우 안에 있는지 
	// String
	char				m_szCoord[9];						// String of coordinate
	CTString			m_strToolTip;						// String of tool tip

	// Region of each part
	UIRect				m_rcTop;							// Region of top background
	UIRect				m_rcCompassIn;						// Region of inner compass
	UIRect				m_rcCompassOut;						// Region of outer compass
	UIRect				m_rcOption;							// Region of option
	UIRect				m_rcPoint[RADAR_OPTION_TOTAL];		// Region of point of objects
	UIRect				m_rcMyPoint;						// Region of my point
	UIRect				m_rcTarget;							// Region of target object
	UIRect				m_rcSignal;							// Region of Signal object
	UIRect				m_rcOutSignal;						// Region of Out Signal ( Direction ) object

	UIRect				m_rcToolTip;						// Region of tool tip
	int					m_nOptionIconSX;					// Position x of option icon

	// UV of each part
	UIRectUV			m_rtBackTop;						// UV of top background
	UIRectUV			m_rtCompassIn;						// UV of inner compass
	UIRectUV			m_rtCompassOut;						// UV of outer compass
	UIRectUV			m_rtOptionL;						// UV of left region of option popup
	UIRectUV			m_rtOptionM;						// UV of middle region of option popup
	UIRectUV			m_rtOptionR;						// UV of right region of option popup
	UIRectUV			m_rtPoint[RADAR_OPTION_TOTAL];		// UV of point of objects
	UIRectUV			m_rtMyPoint;						// UV of my point
	UIRectUV			m_rtTarget;							// UV of point of target object
	UIRectUV			m_rtSignal;							// UV of Signal object
	UIRectUV			m_rtOutSignal;						// UV of Out Signal ( Direction ) object
	UIRectUV			m_rtTargetAni;						// UV of animation point of target object
	UIRectUV			m_rtToolTipL;						// UV of left region of tool tip
	UIRectUV			m_rtToolTipM;						// UV of middle region of tool tip
	UIRectUV			m_rtToolTipR;						// UV of right region of tool tip

	// time rect
	// wooss ------------------------------------------------------------->> 060415
	UIRect				m_rcYear;
	UIRect				m_rcMonth;
	UIRect				m_rcDay;
	UIRect				m_rcTime;
	UIRectUV			m_rtFigure[10];

	// -------------------------------------------------------------------<<
	
	CTextureData	*m_ptdButtonTexture;			// Texture of Button

protected:
	// Internal functinos
	void	RenderObjectLocation();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUIRadar();
	~CUIRadar();

	// Create & destroy
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT KeyMessage( MSG *pMsg );
		
	// 공성 Signal 
	void SetSignal( float fX, float fZ );
	void SendSignal( float fX, float fZ );
	bool RadarToWorld( float& fX, float& fZ );
	bool IsPossibleSignal();
	bool IsPossibleCastleRadar();
	void RenderSignal();
	void SetSignalOn( bool bOn );

	// Set Time 
	inline void SetTime(int year=0,int month =0 ,int day = 0,int hour = 0,LONG startTime=0) { m_year = year, m_month =month,m_day =day, m_hour = hour, m_startTime = startTime; 	}
	inline time_t GetStartTime() { return m_startTime;}
	// Get cipher
	int CountCipher( int tv_time);
	void DisplayNum( int tv_time,UIRect tv_rect);

};


#endif	// UIRADAR_H_

