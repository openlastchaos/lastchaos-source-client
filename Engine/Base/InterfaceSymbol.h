#ifndef __INTERFACE_SYMBOL_H__
#define __INTERFACE_SYMBOL_H__

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <Engine/Base/Stream.h>

enum eUISYMBOL_INT_TYPE
{
	eUISYMBOL_INT_LOGO = 0,
	eUISYMBOL_INT_MAX,
};

class ENGINE_API CInterfaceSymbol : public CSingletonBase<CInterfaceSymbol>
{
public:
	CInterfaceSymbol();

	void SaveInterfaceSymbol();

	int getLogoType() { return _nValue[eUISYMBOL_INT_LOGO]; }
	void setLogoType(int nLogoType = 0);

private:
	void _loadSymbolIntegerData();

	int			_nValue[eUISYMBOL_INT_MAX];
	CTFileName	_fnmInterfaceSymbols;
};

#define ISYMBOL_MGR() CInterfaceSymbol::getSingleton()

#endif	//	__INTERFACE_SYMBOL_H__