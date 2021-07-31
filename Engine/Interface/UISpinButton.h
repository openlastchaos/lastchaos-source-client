
#ifndef	UISPINBUTTON_H_
#define	UISPINBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <vector>
#include <Engine/Interface/UIButton.h>
//------------------------------------------------------------------------------
// CUISpinButton
// Explain:  
// Date : 2005-06-23,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUISpinButton : public CUIWindow
{
protected:
	std::vector<CTString>	m_vecData;			// 저장되는 데이터 
	BOOL					m_bDataBack;		// 데이터의 배경이 출력되는가?
	int						m_nDataBackWidth;	// 데이터의 배경의 너비(높이는 UI의 높이를 따라감)
	UIRectUV3				m_rt3Data;			// 데이터 배경의 UV

	CTString				m_strTitle;			// 제목
	CUIButton				m_btnUp;			// 위쪽 버튼 
	CUIButton				m_btnDown;			// 아래쪽 버튼 
	
	int						m_nSelectIndex;		// 현재 선택된 Data Index ( AddData하면 0으로 셋팅됨 )
	int						m_nStrWidth;		// Title 의 길이
	BOOL					m_bVertical;		// SpinButton이 세로인지 가로인지
	UIRect					m_rcWheel;
	

public:

	CUISpinButton();
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, CTString strTitle, int nDataBackWidth );
	void	CreateButtons( int nBtnWidth, int nBtnHeight, int nButtonGap = 0,  BOOL bVertical = TRUE );

	void	Render();

	// Message
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void		AddData( CTString strData );		// 스트링 형태로 저정하지만 숫자가 될수도 있다.
	int			GetDataToNumber( int nIndex = -1 );		// 데이터를 숫자형태 얻는다.
	CTString	GetDataToString( int nIndex = -1 );		// 데이터를 스트링 형태로 얻는다		
	inline int	GetSelectPos()	{return m_nSelectIndex;} // 선택한 위치 // WSS_DRATAN_SEIGEWARFARE 2007/08/06
	inline void SetSelectPos(int tPos)	{ m_nSelectIndex = tPos;} // 선택한 위치 // WSS_DRATAN_SEIGEWARFARE 2007/08/06
	void		MoveDataIndex( int nIndexDelta );
	void		ResetAllDatas()
	{
		m_vecData.clear();
	}
	// 현재 UI의 PosX, PosY을 기존으로 nLeft, nTop을 잡아야함.
	void	SetWheelRect( int nLeft, int nTop, int nWidth, int nHeight )
	{
		m_rcWheel.SetRect( nLeft, nTop, nLeft + nWidth, nTop + nHeight );
	}

	// InLine : Set/Cppy UV
	void	SetDataBackUV3( FLOAT fLU0, FLOAT fLV0, FLOAT fLU1, FLOAT fLV1,
							FLOAT fMU0, FLOAT fMV0, FLOAT fMU1, FLOAT fMV1,
							FLOAT fRU0, FLOAT fRV0, FLOAT fRU1, FLOAT fRV1,
							FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_bDataBack = TRUE;
		m_rt3Data.SetUV3( fLU0, fLV0, fLU1, fLV1, fMU0, fMV0, fMU1, fMV1, fRU0, fRV0, fRU1, fRV1,
						fTexWidth, fTexHeight );
	}

	void	SetUpUV( UIBtnState bsState, FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnUp.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnUp.CopyUV( bsSrcState, bsDstState );
	}
	void	SetDownUV( UIBtnState bsState, FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnDown.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnDown.CopyUV( bsSrcState, bsDstState );
	}
	
	void RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );	

	void SetTitle( CTString strTitle );
};


#endif	// UISPINBUTTON_H_

