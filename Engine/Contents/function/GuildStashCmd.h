#ifndef __COMMAND_GUILDSTASH_H__
#define __COMMAND_GUILDSTASH_H__

class Command;
class CmdDragIcon;
// G = Guild, S = Stash
class CmdGSDragIcon : public CmdDragIcon
{
public:
	CmdGSDragIcon();
	void setData(CUIIcon* pIcon)
	{
		CmdDragIcon::setData(pIcon);
	}
private:
};

#endif // __COMMAND_GUILDSTASH_H__