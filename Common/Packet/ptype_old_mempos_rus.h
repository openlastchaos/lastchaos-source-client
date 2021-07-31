#ifndef __PTYPE_OLD_MEMPOS_RUS_H__
#define __PTYPE_OLD_MEMPOS_RUS_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct memposWriteRus : public pTypeBase 
	{
		char		slot;
		char		comment[MEMPOS_COMMENT_LENGTH_RUS + 1];
	};

// 	struct memposMove : public pTypeBase 
// 	{
// 		char		slot;
// 	};
}

namespace ResponseClient
{
	struct memposElementRus 
	{
		unsigned char	index;
		int				zone;
		float			x;
		float			z;
		char			comment[MEMPOS_COMMENT_LENGTH_RUS + 1];
	};

 	struct memposWriteRus : public pTypeBase 
 	{
 		memposElementRus	data;
 	};
 
 	struct memposListRus : public pTypeBase 
 	{
 		int					memposTime;
 		unsigned short		listCount;
 		memposElementRus	list[0];
 	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
