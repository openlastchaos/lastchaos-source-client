#ifndef __LUA_FILE_LOADER_H__
#define __LUA_FILE_LOADER_H__

#include <Engine/Base/CTString.h>

class CLoadLua
{
public:
	static int		readDataToInteger(const char* fileFath, const char* fieldName);
	static CTString readDataToString(const char* fileFath, const char* fieldName);

protected:
	static bool		Lua_Load(const char* fileFath, lua_State* L);
};

#endif	//	__LUA_FILE_LOADER_H__