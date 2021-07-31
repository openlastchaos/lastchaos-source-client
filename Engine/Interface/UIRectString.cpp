#include "stdh.h"
#include <Engine/Interface/UIRectString.h>
#include <Engine/Interface/UITextureManager.h>

//////////////////////////////////////////////////////////////////////////

void CRectStringData::CalculateRect()
{
	int nTotalWidth = 0;
	int n2ByteCharWidth = _pUIFontTexMgr->GetFontWidth2Byte();
	int nCharWidth = _pUIFontTexMgr->GetFontWidth();
	int nFontSpace = _pUIFontTexMgr->GetFontSpacing();

	for(int i=0; i<m_strData.Length(); i++)
	{
		if( m_strData[i] & 0x80 )
		{
			nTotalWidth += n2ByteCharWidth;
			i++;
		}
		else
		{
			nTotalWidth += nCharWidth;
		}
		nTotalWidth += nFontSpace;
	}

	m_rcStrRect.Right = m_rcStrRect.Left + nTotalWidth;
	m_rcStrRect.Bottom = m_rcStrRect.Top + _pUIFontTexMgr->GetLineHeight() + _pUIFontTexMgr->GetLineSpacing();
}

void CRectStringData::SetString(CTString& strInput, COLOR color /* = 0xFFFFFFFF */)
{
	m_strData = strInput;
	m_Color = color;
	CalculateRect();
}


int CUIRectString::CheckSplitPos(CTString& strInput, int nWidth)
{
	int nTotalWidth = 0;
	int n2ByteCharWidth = _pUIFontTexMgr->GetFontWidth2Byte();
	int nCharWidth = _pUIFontTexMgr->GetFontWidth();
	int nFontSpace = _pUIFontTexMgr->GetFontSpacing();

	for(int i=0; i<strInput.Length(); i++)
	{
		if( strInput[i] & 0x80 )
		{
			if(nTotalWidth + n2ByteCharWidth > nWidth)
			{
				return i;
			}
			else
			{
				nTotalWidth += n2ByteCharWidth;
				i++;
			}
		}
		else
		{
			if(nTotalWidth + nCharWidth > nWidth)
			{
				return i;
			}
			else
			{
				nTotalWidth += nCharWidth;
			}
		}
		nTotalWidth += nFontSpace;	
	}

	return i;
}

void CUIRectString::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	m_pParentWnd = pParentWnd;

	m_nPosX = nX;
	m_nPosY = nY;
	
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void CUIRectString::Create(CUIWindow *pParentWnd, UIRect rcRect)
{
	Create(pParentWnd, rcRect.Left, rcRect.Top, rcRect.GetWidth(), rcRect.GetHeight());
}

void CUIRectString::SetSize(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	CUIRectString tmp;
	tmp = *this;

	tmp.ClearString();
	
	for(int i=0; i<m_vecStringList.size(); i++)
	{
		tmp.AddString(m_vecStringList[i], m_vecColor[i]);
	}

	*this = tmp;
}

void CUIRectString::SetWidth(int nWidth )
{
	SetSize(nWidth, m_nHeight);
}

void CUIRectString::SetHeight(int nHeight )
{
	SetSize(m_nWidth, nHeight);
}

void CUIRectString::SetPos(int nPosX, int nPosY)
{
		m_nPosX = nPosX;
		m_nPosY = nPosY;
		
		SetAlign(m_nAlign);
}

void CUIRectString::SetPosX(int nX )
{
	SetPos(nX, m_nPosY);
}

void CUIRectString::SetPosY(int nY )
{
	SetPos(m_nPosX, nY);
}

void CUIRectString::Move(int ndX, int ndY )
{
	SetPos(m_nPosX+ndX, m_nPosY+ndY);
}

int CUIRectString::GetMaxStrWidth()
{
	int nMaxSize = 0;
	for(int i=0; i<m_vecRcStringData.size(); i++)
	{
		if(nMaxSize < m_vecRcStringData[i].GetWidth())
		{
			nMaxSize = m_vecRcStringData[i].GetWidth();
		}
	}

	return nMaxSize;
}

int CUIRectString::GetMaxStrHeight()
{
	int nMaxSize = 0;
	for(int i=0; i<m_vecRcStringData.size(); i++)
	{
		nMaxSize += _pUIFontTexMgr->GetLineHeight() + _pUIFontTexMgr->GetLineSpacing();
	}

	return nMaxSize;
}


void CUIRectString::AddString(CTString& strInput, COLOR color /* = 0xFFFFFFFF */)
{
	m_vecStringList.push_back(strInput);
	m_vecColor.push_back(color);
	AddCRectStringData(strInput, color);
}

void CUIRectString::AddCRectStringData(CTString& strInput, COLOR color)
{
	CRectStringData strCheck;
	CTString strSplited1 = strInput;
	CTString strSplited2;

	strCheck.SetString(strSplited1);

	if(strCheck.GetWidth() > m_nWidth)
	{
		int nSplitPos = CheckSplitPos(strInput, m_nWidth);
		strInput.Split(nSplitPos, strSplited1, strSplited2);
		AddCRectStringData(strSplited1, color);
		AddCRectStringData(strSplited2, color);
	}
	else
	{
		int nPosX = 0;
		int nPosY = 0;

		strCheck.SetString(strInput, color);

		if(m_nAlign == ALIGN_LEFT)
		{
			nPosX = 0;
		}
		else if(m_nAlign == ALIGN_CENTER)
		{
			nPosX = ((m_nWidth - strCheck.GetWidth()) / 2);
		}
		else if(m_nAlign == ALIGN_RIGHT)
		{
			nPosX = m_nWidth - strCheck.GetWidth();
		}
	
		if(m_vecRcStringData.size() != 0)
		{
			nPosY = m_vecRcStringData[m_vecRcStringData.size()-1].GetRect().Bottom;
		}
		else
		{
			nPosY = 0;
		}

		strCheck.SetPos(nPosX, nPosY);
		m_vecRcStringData.push_back(strCheck);

	}
}

void CUIRectString::SetAlign(int nAlign)
{
	if(nAlign < ALIGN_LEFT || nAlign > ALIGN_RIGHT)
		return;

	m_nAlign = nAlign;

	int nNewPosX = 0;

	for(int i=0; i<m_vecRcStringData.size(); i++)
	{
		if(m_nAlign == ALIGN_LEFT)
		{
			m_vecRcStringData[i].SetPos(0, m_vecRcStringData[i].GetPosY());
		}
		else if(m_nAlign == ALIGN_CENTER)
		{
			nNewPosX = ((m_nWidth - m_vecRcStringData[i].GetWidth()) / 2);
			m_vecRcStringData[i].SetPos(nNewPosX, m_vecRcStringData[i].GetPosY());
		}
		else if(m_nAlign == ALIGN_RIGHT)
		{
			nNewPosX = m_nWidth - m_vecRcStringData[i].GetWidth();
			m_vecRcStringData[i].SetPos(nNewPosX, m_vecRcStringData[i].GetPosY());
		}
	}
}


void CUIRectString::ClearString()
{
	for(int i=m_vecRcStringData.size()-1; i>=0; i--)
	{
		m_vecRcStringData[i].ClearString();
		m_vecRcStringData.pop_back();
	}

	m_vecStringList.clear();
	m_vecColor.clear();
}

void CUIRectString::Render()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
//	_pUIMgr->GetDrawPort()->Fill(nX, nY, m_nWidth, m_nHeight, 0x00000099);

	for(int i=0; i<m_vecRcStringData.size(); i++)
	{
		if(m_vecRcStringData[i].GetRect().Bottom <	nY + m_nHeight)
		_pUIMgr->GetDrawPort()->PutTextEx( m_vecRcStringData[i].GetString(), nX + m_vecRcStringData[i].GetPosX(), 
										   nY + m_vecRcStringData[i].GetPosY(), m_vecRcStringData[i].GetColor() );
	}

	_pUIMgr->GetDrawPort()->EndTextEx();
}


void CUIRectStringList::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	
	m_nPosX = nX;
	m_nPosY = nY;
	
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_RectSeparate.SetRect(0, 0, 0, 0);
}

void CUIRectStringList::Create(CUIWindow *pParentWnd, UIRect rcRect)
{
	Create(pParentWnd, rcRect.Left, rcRect.Top, rcRect.GetWidth(), rcRect.GetHeight());
}

void CUIRectStringList::SetAlignAll(int nAlign)
{
	for(int i=0; i<m_vecRectString.size(); i++)
	{
		m_vecRectString[i].SetAlign(nAlign);
	}
}

void CUIRectStringList::SetAlignRow(int nRow, int nAlign)
{
	if(nRow < 1 || nRow > m_nRowMax)
		return;
	int nNum = CalcRowColNum(nRow, 0);

	for(int i=0; i<m_nColMax; i++)
	{
		m_vecRectString[nNum].SetAlign(nAlign);
		nNum += m_nRowMax;
	}
}

void CUIRectStringList::SetAlignCol(int nCol, int nAlign)
{
	if(nCol < 1 || nCol > m_nRowMax)
		return;

	int nNum = CalcRowColNum(0, nCol);

	for(int i=0; i<m_nRowMax; i++)
	{
		m_vecRectString[nNum+i].SetAlign(nAlign);
	}
}

void CUIRectStringList::SetAlign(int nRow, int nCol, int nAlign)
{
	if(nRow < 1 || nRow > m_nRowMax || nCol < 1 || nCol > m_nColMax)
		return;

	int nNum = CalcRowColNum(nRow, nCol);

	m_vecRectString[nNum].SetAlign(nAlign);
}

void CUIRectStringList::AddString(int nRow, int nCol, CTString& strInput, COLOR color /* = 0xFFFFFFFF */)
{
	if(nRow < 1 || nRow > m_nRowMax || nCol < 1 || nCol > m_nColMax)
		return;

	int nNum = CalcRowColNum(nRow, nCol);

	m_vecRectString[nNum].AddString(strInput, color);
}

void CUIRectStringList::SetPos(int nPosX, int nPosY)
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	if(m_nRowMax > 0 && m_nColMax > 0)
	{
		int nNum = 0;
		for(int i=0; i<m_nColMax; i++)
		{
			for(int j=0; j<m_nRowMax; j++)
			{
				nNum = (i * m_nColMax) + j;
				m_vecRectString[nNum].SetPos(nPosX + (m_RectSeparate.GetWidth() * j), nPosY + (m_RectSeparate.GetHeight() * i));
			}
		}
	}
}

void CUIRectStringList::SetPosX(int nX )
{
	SetPos(nX, m_nPosY);
}

void CUIRectStringList::SetPosY(int nY )
{
	SetPos(m_nPosX, nY);
}

void CUIRectStringList::SetSize(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	int nNum = 0;

	if(m_nRowMax > 0 && m_nColMax > 0)
	{
		for(int i=0; i<m_nColMax; i++)
		{
			for(int j=0; j<m_nRowMax; j++)
			{
				nNum = (i * m_nRowMax) + j;
				m_vecRectString[nNum].SetSize(m_RectSeparate.GetWidth(), m_RectSeparate.GetHeight());
				m_vecRectString[nNum].SetPos(m_nPosX + j*m_RectSeparate.GetWidth(), m_nPosY + i*m_RectSeparate.GetHeight());
			}
		}
	}
}

void CUIRectStringList::SetWidth(int nWidth )
{
	SetSize(nWidth, m_nHeight);
}

void CUIRectStringList::SetHeight(int nHeight )
{
	SetSize(m_nWidth, nHeight);
}

void CUIRectStringList::SetRowCol(int nRow, int nCol)
{
	if(nRow < 0  || nCol < 0)
		return;

	int nSeparateWidth =  m_nWidth / nRow;
	int nSeparateHeight = m_nHeight / nCol;

	m_RectSeparate.SetRect(0, 0, nSeparateWidth, nSeparateHeight);

	CUIRectString rcStringList; 
	rcStringList.Create(this, m_RectSeparate);

	if(m_nRowMax == 0 && m_nColMax == 0)
	{

		for(int i=0; i<nCol; i++)
		{
			for(int j=0; j<nRow; j++)
			{
				m_vecRectString.push_back(rcStringList);
			}
		}
	}
	else
	{
		if((nRow * nCol) > (m_nRowMax * m_nColMax))
		{
			int nAddSize = (nRow * nCol) - (m_nRowMax * m_nColMax);
			
			for(int i=0; i<nAddSize; i++)
			{
				m_vecRectString.push_back(rcStringList);
			}
		}
	}

	m_nColMax = nCol;
	m_nRowMax = nRow;

	SetSize(m_nWidth, m_nHeight);
}

void CUIRectStringList::ClearStringAll()
{
	for(int i=0; i<m_vecRectString.size(); i++)
	{
		m_vecRectString[i].ClearString();
	}
}

void CUIRectStringList::ClearString(int nRow, int nCol)
{
	if(nRow < 1 || nRow > m_nRowMax || nCol < 1 || nCol > m_nColMax)
		return;
	int nNum = CalcRowColNum(nRow, nCol);

	m_vecRectString[nNum].ClearString();
}

void CUIRectStringList::Render()
{
	for(int i=0; i<m_nRowMax*m_nColMax; i++)
	{
		m_vecRectString[i].Render();
	}
}


//////////////////////////////////////////////////////////////////////////