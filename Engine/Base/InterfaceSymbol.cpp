#include "StdH.h"
#include "InterfaceSymbol.h"
#include <Engine/lua/LuaFileLoader.h>
#include <Engine/Contents/Login/LoginNew.h>

const CTString strIntegerField[eUISYMBOL_INT_MAX] = { "loginType" };

CInterfaceSymbol::CInterfaceSymbol()
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
	_fnmInterfaceSymbols = strFullPath + CTString("Data\\etc\\IS.lua");
	_loadSymbolIntegerData();
}

void CInterfaceSymbol::setLogoType( int nLogoType /*= 0*/ )
{
	if (nLogoType < 0 || nLogoType >= eLOGO_MAX)
		_nValue[eUISYMBOL_INT_LOGO] = 0;
	else
		_nValue[eUISYMBOL_INT_LOGO] = nLogoType;
}

void CInterfaceSymbol::_loadSymbolIntegerData()
{
#ifndef		WORLD_EDITOR
	int i;

	// integerData Load
	for (i = 0; i < eUISYMBOL_INT_MAX; ++i)
	{
		_nValue[i] = CLoadLua::readDataToInteger(_fnmInterfaceSymbols, strIntegerField[i]);
	}

	// StringData Load
#endif		// WORLD_EDITOR
}

void CInterfaceSymbol::SaveInterfaceSymbol()
{
	FILE *fs = fopen(_fnmInterfaceSymbols, "w");
	
	if (fs == NULL)
		return;
	CTString strSave;

	for (int i = 0; i < eUISYMBOL_INT_MAX; ++i)
	{
		strSave.PrintF("%s=%d\n", strIntegerField[i], _nValue[i]);
		fputs(strSave.str_String, fs);
	}

	fclose(fs);
}
