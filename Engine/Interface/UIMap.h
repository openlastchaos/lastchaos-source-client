// ----------------------------------------------------------------------------
//  File : UIMap.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMAP_H_
#define	UIMAP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UISlideBar.h>
#include <vector>
#include <deque>


// Sub zone tyoe
enum SubZoneType
{
	VILLAGE			= 0,
	DUNGEON,
	CHARGE_PRIVATE,
	CHARGE_PUBLIC,
	MINE_PRIVATE,
	MINE_PUBLIC,
	GATHER_PRIVATE,
	GATHER_PUBLIC,
	CASTLE_GATE,		// 성문
	SUBZONE_TOTAL,
};

enum CastleType
{
	// 공성 관련 
	CASTLE_LORD,		// 권좌
	CASTLE_REGEN,		// 리젠 포인트 
	CASTLE_TOWER,		// 수호탑
	CASTLE_SIGNAL,		// 시그널 
	CASTLE_TOTAL,
};


// Define text position
#define	MAP_TITLE_TEXT_OFFSETX		25
#define	MAP_TITLE_TEXT_OFFSETY		5


// Blend time
#define	MAP_FRAME_BLENDTIME			200
#define	MAP_DETAIL_BLENDTIME		500
#define	MAP_LAYER_BLENDTIME			500


// Define size of map
#define	MAP_FRAME_WIDTH				518
#define	MAP_FRAME_HEIGHT			537
#define	MAP_FRAME_WIDTH_MIN			235
#define	MAP_FRAME_HEIGHT_MIN		260
#define	MAP_FRAME_BASE_WIDTH		512
#define	MAP_FRAME_BASE_HEIGHT		512
#define	MAP_FRAME_EDGE_SIZE			3
#define	MAP_FRAME_EDGE_HORZ			6
#define	MAP_FRAME_EDGE_VERT			25


// Define slide bar info
#define	MAP_ZOOM_SLIDE_ORIGIN		12



// Define position of map region
#define	MAP_SX						3
#define	MAP_SY						22

typedef struct _CastleData
{
	LONG	nType;
	LONG	nIndex;
	LONG	nHP, nMaxHP;
	FLOAT	fX, fZ;
	BOOL	bVisible;
	DOUBLE	dStartTime;
} CastleData;

// ----------------------------------------------------------------------------
// Name : CUIMap
// Desc :
// ----------------------------------------------------------------------------
class CUIMap : public CUIWindow
{
protected:
	typedef struct _WorldMapData
	{
		int							nZoneIndex;
		SBYTE						sbLayer;
		UIRect						rcMapSize;
		FLOAT						fRatio;
		LONG						lOffsetX, lOffsetZ;
		CTString					strName;
	} WorldMapData;

	typedef struct _DetailMapData
	{
		UIRect						rcSelRegion;
		UIRect						rcMapSize;
		FLOAT						fRatio;
		FLOAT						fOffsetX, fOffsetZ;
	} DetailMapData;

	typedef struct _SubZoneData
	{
		int							nSubZoneIndex;
		FLOAT						fX, fZ;
		SBYTE						sbType;
	} SubZoneData;

	typedef struct _NpcData
	{
		int							nIndex;
		LONG						nYLayer;
		FLOAT						fX, fZ;
	} NpcData;

	typedef struct _MapData
	{
		WorldMapData				World;
		std::vector<DetailMapData>	vecDetail;
		std::vector<SubZoneData>	vecSubZone;
		std::vector<NpcData>		vecNpc;
		std::vector<CastleData>		vecCastleData;
	} MapData;

	std::deque<CastleData>		m_qSignalData;

	// Control
	CUIButton				m_btnSignal;							// Signal Button 
	CUIButton				m_btnClose;								// Close button
	CUIComboBox				m_cmbMap;								// Map combo box
	CUISlideBar				m_slbOpacity;							// Opacity slide bar
	CUISlideBar				m_slbZoomWorld;							// Zooming world map slide bar
	CUISlideBar				m_slbZoomDetail;						// Zooming Detail map slide bar
	BOOL					m_bShowWorldMap;						// If world map is shown or village map is shown
	BOOL					m_bShowFrame;							// If frame is shown or not
	BOOL					m_bFrameBlending;						// If frame is blending or not
	BOOL					m_bMapBlendingForDetail;				// If map is blending for detail or not
	BOOL					m_bMapBlendingForLayer;					// If map is blending for layer or not
	BOOL					m_bCanScrollMapWorld;					// If scrolling world map is possible or not
	BOOL					m_bCanScrollMapDetail;					// If scrolling detail map is possible or not
	BOOL					m_bNowScrollMapWorld;					// if world map is scrolling now or not
	BOOL					m_bNowScrollMapDetail;					// If detail map is scrolling now or not
	__int64					m_llFrameBlendTime;						// Frame blending start time
	__int64					m_llFrameBlendElapsedTime;				// Frame blending elasped time
	__int64					m_llMapBlendTimeForDetail;				// Map blending start time for detail
	__int64					m_llMapBlendTimeForLayer;				// Map blending start time for layer
	__int64					m_llMapBlendElapsedTimeForLayer;		// Map blending elasped time of layer

	// Map data
	std::vector<MapData>	m_vecMapData;							// Map data
	int						m_nCurMapData;							// Current index of world map data
	int						m_nCurDetail;							// Current index of detail map data

	// Map information
	SBYTE					m_sbCurLayer;							// Current layer
	UIRect					m_rcViewportWorld;						// Region of viewport for world map
	UIRect					m_rcViewportDetail;						// Region of viewport for detail map
	UIRect					m_rcMapWorld;							// Texel region of worla map
	UIRect					m_rcMapDetail;							// Texel region of detail map
	FLOAT					m_fZoomWorld;							// Zoom ratio of world map
	FLOAT					m_fZoomDetail;							// Zoom ratio of detail map
	FLOAT					m_fMapOpacity;							// Opacity ratio of map
	FLOAT					m_fMoveElapsedErrXWorld;				// Elapsed x by scrolling world map
	FLOAT					m_fMoveElapsedErrYWorld;				// Elapsed y by scrolling world map
	FLOAT					m_fMoveElapsedErrXDetail;				// Elapsed x by scrolling detail map
	FLOAT					m_fMoveElapsedErrYDetail;				// Elapsed y by scrolling detail map
	int						m_nResizeWidthOffset;					// Elapsed x by resizing frame
	int						m_nResizeHeightOffset;					// Elapsed y by resizing frame
	int						m_nMapWorldBaseWidth;					// Base width of world map
	int						m_nMapWorldBaseHeight;					// Base height of world map
	int						m_nMapDetailBaseWidth;					// Base width of detail map
	int						m_nMapDetailBaseHeight;					// Base height of detail map
	BOOL					m_bShowOpacitySlideInfo;				// If opacity slide information is shown or not
	BOOL					m_bShowZoomSlideInfo;					// If zoom slide information is shown or not
	CTString				m_strOpacitySlideInfo;					// String of opacity slide information
	CTString				m_strZoomSlideInfo;						// String of zoom slide information
	int						m_nOpacityStringWidth;					// Size of opacity string
	int						m_nZoomStringWidth;						// Size of zoom string
	int						m_nZoomCenterX;							// Center x for zomming map
	int						m_nZoomCenterY;							// Center y for zomming map
	BOOL					m_bShowToolTip;							// If tool tip is shown or not
	CTString				m_strToolTip;							// String of tool tip

	// Texture
	int						m_nCurWorldTex;							// Current texture index of world map
	CTextureData			*m_ptdWorldTex[2];						// Texture of world map
	CTextureData			*m_ptdDetailTex;						// Texture of detail map
	FLOAT					m_fWorldTexWidth;						// Width of world texture
	FLOAT					m_fWorldTexHeight;						// Height of world texture
	FLOAT					m_fDetailTexWidth;						// Width of detail texture
	FLOAT					m_fDetailTexHeight;						// Height of detail texture

	// Region of each part
	UIRect					m_rcTitle;								// Region of title
	UIRect					m_rcResizeLeft;							// Region of left resizing
	UIRect					m_rcResizeRight;						// Region of right resizing
	UIRect					m_rcResizeBottomL;						// Region of left bottom resizing
	UIRect					m_rcResizeBottomM;						// Region of middle bottom resizing
	UIRect					m_rcResizeBottomR;						// Region of right bottom resizing
	UIRect					m_rcPC;									// Region of PC
	UIRect					m_rcSubZone[SUBZONE_TOTAL];				// Region of sub zone
	UIRect					m_rcCastle[CASTLE_TOTAL];				// Region of sub zone
	UIRect					m_rcNPC;								// Region of NPC
	UIRect					m_rcParty;								// Region of party
	UIRect					m_rcCompass;							// Regino of compass
	UIRect					m_rcToolTip;							// Region of tool tip
	UIRect					m_rcHelpNpc;							// NPC 안내시스템
	UIRect					m_rcQuestNpc;							// 퀘스트 NPC
	UIRect					m_rcSuccessNpc;							// 퀘스트 NPC

	// UV of each part
	UIRectUV				m_rtBackUL;								// UV of upper left background
	UIRectUV				m_rtBackUM;								// UV of upper middle background
	UIRectUV				m_rtBackUR;								// UV of upper right background
	UIRectUV				m_rtBackML;								// UV of middle left background
	UIRectUV				m_rtBackMM;								// UV of middle middle background
	UIRectUV				m_rtBackMR;								// UV of middle right background
	UIRectUV				m_rtBackLL;								// UV of lower left background
	UIRectUV				m_rtBackLM;								// UV of lower middle background
	UIRectUV				m_rtBackLR;								// UV of lower right background
	UIRectUV				m_rtBackListBar;						// UV of list bar
	UIRectUV				m_rtPC;									// UV of PC
	UIRectUV				m_rtSubZone[SUBZONE_TOTAL];				// UV of sub zone
	UIRectUV				m_rtCastle[CASTLE_TOTAL];				// UV of sub zone
	UIRectUV				m_rtNPC;								// UV of NPC
	UIRectUV				m_rtHelpNpc;							// NPC 안내 시스템
	UIRectUV				m_rtQuestNpc;							// 퀘스트 NPC
	UIRectUV				m_rtSuccessNpc;							// 퀘스트 NPC
	UIRectUV				m_rtParty;								// UV of party
	UIRectUV				m_rtCompass;							// UV of compass
	UIRectUV				m_rtWorld;								// UV of world map
	UIRectUV				m_rtDetail;								// UV of detail map
	UIRectUV				m_rtToolTipL;							// UV of left region of tool tip
	UIRectUV				m_rtToolTipM;							// UV of middle region of tool tip
	UIRectUV				m_rtToolTipR;							// UV of right region of tool tip

	BOOL					m_bSignalBtnOn;
	BOOL					m_bInsideMouse;
protected:
	// Internal functions
	void	ReadMapData();
	int		FindMapData( int nZoneIndex, SBYTE sbLayer );
	void	MakeBaseMapSize( BOOL bWorldMap );
	void	ScrollMapWorld( int ndX, int ndY );
	void	ScrollMapDetail( int ndX, int ndY );
	void	AdjustViewport( int nSelRegion );

	void	ResizeMapFrame( int nSelRegion, int ndX, int ndY );
	void	ChangeMap();
	void	OpenDetailMap( int nIndex );
	void	CloseDetailMap();
	BOOL	IsInsideMap( BOOL bWinPos, BOOL bWorldMap, int nX, int nY, BOOL bZoomRect, UIRect &rcRect );

	void	UpdateOpacityRatio();
	void	UpdateZoomRatio( BOOL bWorldMap );
	void	UpdateOpacitySlideInfo();
	void	UpdateZoomSlideInfo( BOOL bWorldMap );
	void	MakeCenterForZoom( BOOL bWorldMap );
	void	ZoomMap( BOOL bWorldMap );

	void	ShowToolTip( BOOL bShow, CTString &strName, int nX = 0, int nY = 0 );

public:

	CUIMap();
	~CUIMap();

	// Create & destroy
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Destroy();

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Set map of current world
	void	SetCurrentWorldMap( int nWorld, SBYTE sbLayer );

	// Toggle visible
	void	ToggleVisible();

	void	MapToWorld( float& fX, float& fY );
	void	WorldToMap( float& fX, float& fY );
	void	InitCastleData();
	void	AddCastleData( LONG nType, FLOAT fX, FLOAT fY, LONG nIndex = -1, LONG nHP = -1, LONG nMaxHP = -1 );
	
	void	SendSignal( FLOAT fX, FLOAT fY );
	void	AddSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex );
	void	RenderSignal();
	void	ClearAllSignal();

	BOOL	IsCastleMap( LONG nMapIndex );

	void	ReSetData();
	
	bool	IsPossibleCastleMap();
	bool	IsPossibleSignal();
	void	SetSignalOn( bool bOn );
	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT KeyMessage( MSG *pMsg );

};



#endif	// UIMAP_H_






















