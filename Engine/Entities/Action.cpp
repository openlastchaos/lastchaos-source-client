#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/Action.h>
#include <Engine/Base/CTString.h>


CAction::CAction(void) 
{	
	Action_Index	= -1;
	Action_Type		= -1;
	Action_Job		= 0;

	// FIXME : 뻑나는 경우가 있는데 어딘지 위치를 몰라서, 0, 0, 0으로 설정함.
	Action_TexID	= 0;
	Action_TexRow	= 0;
	Action_TexCol	= 0;
}

CAction::~CAction(void) 
{
}

int CAction::LoadActionDataFromFile( CStaticArray<CAction> &apActionData, const CTFileName &fnm )
{	
	CTFileStream	fsAction;
	fsAction.Open_t( fnm );

	int	nCount, nIndex, nLength;
	int	nTexID, nTexRow, nTexCol;

	// Count
	fsAction >> nCount;
	apActionData.New(nCount);
	ASSERT( apActionData.Count() >= nCount && "Invalid Array Count" );
	ASSERT( nCount > 0 && "Invalid Action Data" );

	for( int i = 0; i < nCount; i++ )
	{
		// Index
		fsAction >> nIndex;
		ASSERT( nIndex != -1 && "Invalid Action Index" );

		CAction&	rActionData = apActionData[nIndex];	
		rActionData.Action_Index = nIndex;

		// Type
		fsAction >> rActionData.Action_Type;

		// Name
		fsAction >> nLength;
		if( nLength > 0 )
		{
			fsAction.Read_t( &rActionData.Action_strName, nLength );
			rActionData.Action_strName[nLength] = NULL;
		}

		// Description
		fsAction >> nLength;
		if( nLength > 0 )
		{
			fsAction.Read_t( &rActionData.Action_strDesc, nLength );
			rActionData.Action_strDesc[nLength] = NULL;
		}

		// Animation file name
		for( int j = 0; j < TOTAL_JOB; j++ )
		{
			fsAction >> nLength;
			if( nLength > 0 )
			{
				fsAction.Read_t( &rActionData.Action_strAnimName[j], nLength );
				rActionData.Action_strAnimName[j][nLength] = NULL;
			}
		}

		// Job flag
		fsAction >> rActionData.Action_Job;

		// Tex ID, row, col
		fsAction >> nTexID;
		fsAction >> nTexRow;
		fsAction >> nTexCol;
		rActionData.Action_TexID = nTexID;
		rActionData.Action_TexRow = nTexRow;
		rActionData.Action_TexCol = nTexCol;
	}

	return nCount;
}