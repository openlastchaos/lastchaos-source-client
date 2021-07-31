#ifndef		_PARTY_AUTO_REG_H_
#define		_PARTY_AUTO_REG_H_

class CUIPartyAutoReg : public CUIWindow
{
public:
	CUIPartyAutoReg();

	void initialize();

	void open();
	void close();

private:

	void type_normal();
	void type_battle();
};


#endif		// _PARTY_AUTO_REG_H_