#ifndef SE_INCL_MENU_GADGET_LEVELBUTTON_H
#define SE_INCL_MENU_GADGET_LEVELBUTTON_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "MGButton.h"


class CMGLevelButton : public CMGButton {
public:
  CTFileName mg_fnmLevel;

  void OnActivate(void);
  void OnSetFocus(void);
};

#endif  /* include-once check. */