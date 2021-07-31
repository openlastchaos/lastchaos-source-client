#ifndef SE_INCL_CREDITS_H
#define SE_INCL_CREDITS_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// turn credits on
void Credits_On(INDEX iType);
// turn credits off
void Credits_Off(void);
// render credits to given drawport
FLOAT Credits_Render(CDrawPort *pdp);


#endif  /* include-once check. */

