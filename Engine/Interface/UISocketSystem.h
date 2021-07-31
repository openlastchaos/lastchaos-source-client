// ***************************************************************
//  UISocketSystem   version:  1.0   ? date: 04/12/2010
//  -------------------------------------------------------------
//  Author	: rumist
//  Desc	: ui windows class about socket system.
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#ifndef UISOCKETSYSTEM_H__
#define UISOCKETSYSTEM_H__
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#define JEWEL_MAX_COUNT 7

class CUISocketSystem : public CUIWindow
{
public:
	struct stPACKET_ITEM
	{
		SWORD Item_Tab;
		SWORD InvenIdx;
		LONG ItemVirIdx;
		int SlotIdx;

		stPACKET_ITEM()
		{
			Item_Tab = -1;
			InvenIdx = -1;
			ItemVirIdx = -1;
			SlotIdx = -1;
		}
	};

	enum _SOCKET_STAT
	{
		STAT_CREATE = 0,			// punched item.
		STAT_COMBINE,				// item combines with jewels.
		STAT_JEWEL_COMPOS,
		STAT_CHAOSJEWEL_COMPOS,
		STAT_EMPTY,					// empty socket slot. destroy jewels.
		STAT_ONCEEMPTY,
		STAT_END,
	};

	enum _SOCKET_ERROR_TYPE
	{
		SOCKET_ERROR_INEXACT = 0,	// button type is miss matched in socket slot.
		SOCKET_ERROR_EMPTY,			// equip button is empty.
		SOCKET_ERROR_EXCEED,		// slot have exceeded about socket count.
		SOCKET_ERROR_ITEM_LACK,		// lacking item count.
		SOCKET_ERROR_JEWEL_OVERLAP,	// check jewel type about equipment.
		SOCKET_ERROR_COMBINE_TERMS, // 결합 조건에 맞지 않은 보석 입니다. 
		SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT,	// 보석 제거시, 제거된 보석을 받을 인벤토리 공간이 충분하지 않을 경우.
	};
	enum _JEWEL_ERROR_TYPE
	{
		JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL,	// 일반 보석을 등록 하세요.
		JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL,		// 카오스 보석을 등록 하세요.
		JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL,	// 등록할 수 없는 아이템 입니다.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY,		// 합성하는 보석이 없습니다.
		JEWEL_COMPOS_ERROR_JEWEL_LACK,			// 보석이 부족합니다.
		JEWEL_COMPOS_ERROR_NAS_LACK,			// 나스가 부족합니다.
		JEWEL_COMPOS_ERROR_COMPOS_SCROLL,		// 보석 합성스크롤이 잘못 되었습니다.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY, // 인벤토리 공간이 부족합니다.
	};
protected:
	// 공통
	_SOCKET_STAT				m_euSocketStat;		
	int							m_nNPCIndex;
	int							m_nNPCVIdx;
	FLOAT						m_fNPCPosX;
	FLOAT						m_fNPCPosZ;

	void						_toggleVisible( const _SOCKET_STAT _status = STAT_CREATE );
	void						_close();

	// 소켓 단일 제거
	CUIIcon*					m_pIconEmpty;	
	SBYTE						m_nEmptyJewelPos;			

public:
	CUISocketSystem();
	~CUISocketSystem();

	//////////////////////////////////////////////////////////////////////////
	// Overriding functions.
	// create
	void						Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )	;
	void						MsgBoxLCommand(int nCommandCode, int nResult );
	
	void						ToggleVisible( const _SOCKET_STAT _status = STAT_CREATE );
	const BOOL					IsOpenedUI()	const;
	void						CloseAllUI();
	void						absCloseAllUI();

	const FLOAT					GetNPCPosX()	const									{ return m_fNPCPosX;			}
	const FLOAT					GetNPCPosZ()	const									{ return m_fNPCPosZ;			}
	void						GetNPCPos( FLOAT &fNPCPosX, FLOAT &fNPCPosZ )	const	{ fNPCPosX = m_fNPCPosX; fNPCPosZ = m_fNPCPosZ; }
	
	//////////////////////////////////////////////////////////////////////////
	// User creation functions.
	ENGINE_API void				OpenSocketSystem( const int iMobIndex, const BOOL bHasQuest, const FLOAT fX, const FLOAT fZ );
	ENGINE_API void				CloseSocketSystem()										{ _close();						}
		
	//////////////////////////////////////////////////////////////////////////
	// user network message functions.
	void						SendCreateReq(SWORD item_Tab1, SWORD invenIdx1, LONG itemVirIdx1, SWORD item_Tab2, SWORD invenIdx2, LONG itemVirIdx2);
	void						SendCombineReq(stPACKET_ITEM Item, stPACKET_ITEM Juwel, stPACKET_ITEM Scroll);
	void						SendUpgradeReq(bool bChaosJewel, stPACKET_ITEM Jewel1, stPACKET_ITEM Jewel2, stPACKET_ITEM Jewel3, stPACKET_ITEM Scroll, int nScrollCnt);
	void						SendCleanAllJewelReq(SWORD item_Tab, SWORD invenIdx, LONG itemVirIdx);
	void						SendCleanOneJewelReq(SWORD tab, SWORD inven_idx, SLONG index);

	void						_showErrorSystemMessage( const _JEWEL_ERROR_TYPE type); // 에러메시지 출력
	void						showErrorMessage( const _SOCKET_ERROR_TYPE type = SOCKET_ERROR_INEXACT );
	int							IsEmptyInvenSlotCount();	// 인벤토리 내에 빈 슬롯의 카운트.
	
	// 단일 소캣 삭제 Send Data Set
	void						SetRemoveSingleItem(CUIIcon* pItem);
	void						SetRemoveSingleJewelPos(int nJewelPos);
};


#endif
