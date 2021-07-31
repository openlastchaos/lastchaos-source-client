// ----------------------------------------------------------------------------
//  File : UIPetTargetInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIPETTARGETINFO_H_
#define	UIPETTARGETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define position
#define	PETTARGETINFO_NAME_SY			5
#define	PETTARGETINFO_WIDTH				140
#define	PETTARGETINFO_HEIGHT			56
// Define size of player information

// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIPetTargetInfo : public CUIWindow
{
protected:
	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcHP;							// HP
	UIRect				m_rcHPBack;						// Background of HP
	UIRect				m_rcHungry;						// Hungry
	UIRect				m_rcHungryBack;					// Background of Hungry
	UIRect				m_rcInfoMark;	
	

	// UV of each part
	UIRectUV			m_rtBackground1;					// Background	
	UIRectUV			m_rtBackground2;					// Background	
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtHPBack;						// Background of HP	
	UIRectUV			m_rtHungry;						// Hungry
	UIRectUV			m_rtHungryBack;					// Background of Hungry
	CUIButton			m_btnPetInfo;
	
	// TOOLTIP UI
	BOOL				m_bShowTooltip;
	UIRectUV			m_rtToolTipL;
	UIRectUV			m_rtToolTipM;
	UIRectUV			m_rtToolTipR;
	CTString			m_strToolTip;
	
	
protected:
	// Update info of target
	void	UpdateHPInfo();
	void	UpdateHungryInfo();

public:
	CUIPetTargetInfo();
	~CUIPetTargetInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();	
	void    RenderTooltip();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	// UIPETTARGETINFO_H_

