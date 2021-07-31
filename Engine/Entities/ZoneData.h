#ifndef __ZONE_DATA_H__
#define __ZONE_DATA_H__

#define DEF_ZONE_ALBER		42		

class CZoneData
{
public:
	~CZoneData();

	static int LoadZoneDataLod(const char* szFileName);
};

#endif // __ZONE_DATA_H__