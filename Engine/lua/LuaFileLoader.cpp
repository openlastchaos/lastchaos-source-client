#include "StdH.h"
#include "LuaFileLoader.h"
#include <Engine/Base/Stream.h>

bool CLoadLua::Lua_Load(const char* fileFath, lua_State* L)
{
	if (L == NULL)
		return false;

	// 파일 로드 & 초기화
	if (luaL_loadfile(L, fileFath) != 0 || lua_pcall(L, 0, 0, 0) != 0)
		return false;

	return true;
}

int CLoadLua::readDataToInteger( const char* fileFath, const char* fieldName )
{
	if (fieldName == NULL)
		return 0;

	// 루아 생성
	lua_State* L = luaL_newstate();

	// 파일 로드
	if(Lua_Load(fileFath, L) == false)
	{
		lua_close(L);
		return 0;
	}

	// 루아 스택에 전역 이름의 값을 넣는다.
	lua_getglobal(L, fieldName);

	// 루아 스택의 인덱스 (plus, minus 인덱스가 존재하며 두개다 허용된다.)
	// top	   2	     -1
	// bottom  1     -2
	int nReturnVal = lua_tointeger(L, -1);
	// 루아파일을 닫아준다.
	lua_close(L);

	return nReturnVal;
}

CTString CLoadLua::readDataToString( const char* fileFath, const char* fieldName )
{
	CTString strReturn = CTString("");

	if (fieldName == NULL)
		return strReturn;

	lua_State* L = luaL_newstate();

	if(Lua_Load(fileFath, L) == false)
	{
		lua_close(L);
		return strReturn;
	}

	lua_getglobal(L, fieldName);
	const char* strTemp = lua_tolstring(L, -1, NULL);
	strReturn = strTemp;
	lua_close(L);

	return strReturn;
}
