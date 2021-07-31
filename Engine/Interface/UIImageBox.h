#ifndef __UIIMAGEBOX_H__
#define __UIIMAGEBOX_H__

#include <Engine/Interface/UIWindow.h>

class CUIImageBox : public CUIWindow
{
public:
	enum eImageType
	{
		IT_GENERAL	= -1,
		IT_ITEM		= 0,
		IT_MONEY	= 1,
		IT_EXP		= 2,
		IT_SP		= 3,
		IT_SKILL	= 4,
		IT_SSKILL	= 5,
		IT_ACTION	= 6,
	};

	CUIImageBox();
	virtual ~CUIImageBox();

	virtual void Render();

	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	UIRectUV	&GetUV() { return m_rtUV; }
	void	SetRenderRegion(int left, int top, int right, int bottom)
	{
		m_rtRenderRegion.SetRect(left, top, right, bottom);
	}
	UIRect	GetRenderRegion()	{ return m_rtRenderRegion;	}

	void SetImageByType(eImageType type, int index);

	//IT_GENERAL is not effected. 
	void SetHighlight(BOOL enable, COLOR colHighlight);
private:
	eImageType	m_eImageType;
	int			m_nTextureID;
	UIRectUV	m_rtUV;
	UIRect		m_rtRenderRegion;
	BOOL		m_bHighlight;
	COLOR		m_colHighlight;
};

#endif //__UIIMAGEBOX_H__