#ifndef	ACTION_H_
#define ACTION_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif


class  CAction
{
protected:
	LONG		Action_Index;
	LONG		Action_Job;
	SBYTE		Action_Type;
	SBYTE		Action_TexID;
	SBYTE		Action_TexRow;
	SBYTE		Action_TexCol;
	char		Action_strName[32];
	char		Action_strDesc[256];
	char		Action_strAnimName[6][32];

	enum	ACTIONTYPE
	{
		GENERAL		= 0,
		SOCIAL		= 1,
		PARTY		= 2,
		GUILD		= 3,
	};

public:
	CAction(void);
	~CAction(void);

	inline char *		GetName() { return Action_strName; }
	inline char *		GetDesc() { return Action_strDesc; }
	inline char *		GetAnimName( int nJob ) { return Action_strAnimName[nJob]; }
	inline LONG			GetIndex() const { return Action_Index; }
	inline LONG			GetJob() const { return Action_Job; }
	inline SBYTE		GetType() const { return Action_Type; }
	inline SBYTE		GetTexID() const { return Action_TexID; }
	inline SBYTE		GetTexRow() const { return Action_TexRow; }
	inline SBYTE		GetTexCol() const { return Action_TexCol; }

	static int	LoadActionDataFromFile( CStaticArray<CAction> &apActionData, const CTFileName &fnm );
};


#endif	//ACTION_H_