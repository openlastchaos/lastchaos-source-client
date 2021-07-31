#ifndef		_PRODUCT_NPC_CMD_H_
#define		_PRODUCT_NPC_CMD_H_

template <typename T >
class CmdProductNPCSelect : public Command
{
public:
	CmdProductNPCSelect() : _ui(NULL), _fac_idx(-1), 
		_tree_sub_idx(-1), _tree_item_idx(-1) {}
	void setData(T* pui, int nsub, int idx, int fac_idx, COLOR col) { 
		_ui = pui; 
		_fac_idx = fac_idx;
		_tree_sub_idx = nsub;	_tree_item_idx = idx;
		_color = col;
	}
	void execute()
	{
		if (_ui == NULL || _fac_idx < 0 ||
			_tree_sub_idx < 0 || _tree_item_idx < 0)
			return;

		_ui->callback_select(_tree_sub_idx, _tree_item_idx, _fac_idx, _color);
	}
private:
	T*		_ui;
	int		_fac_idx;
	int		_tree_sub_idx;		// 서브 인덱스
	int		_tree_item_idx;		// 
	COLOR	_color;				// 원래 색 (선택해제 시 필요)
};

template <typename T >
class CmdProductNPCOver : public Command
{
public:
	CmdProductNPCOver() : _ui(NULL), _tree_sub_idx(-1), _tree_item_idx(-1) {}
	void setData(T* pui, int sub, int idx, bool over, COLOR col) { 
		_ui = pui;
		_tree_sub_idx = sub;
		_tree_item_idx = idx;
		_bover = over;
		_color = col;
	}
	void execute() {

		if (_ui == NULL || _tree_sub_idx < 0 || _tree_item_idx < 0)
			return;

		_ui->callback_over(_tree_sub_idx, _tree_item_idx, _bover, _color);
	}

private:
	T*		_ui;
	int		_tree_sub_idx;		// 서브 인덱스
	int		_tree_item_idx;		// 
	bool	_bover;
	COLOR	_color;
};


#endif		// _PRODUCT_NPC_CMD_H_