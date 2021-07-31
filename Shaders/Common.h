#ifndef SE_INCL_SHADER_COMMON_H
#define SE_INCL_SHADER_COMMON_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

void DoSpecularLayer(INDEX iSpecularTexture,INDEX iSpecularColor);
void DoReflectionLayer(INDEX iReflectionTexture,INDEX iReflectionColor,BOOL bFullBright);

#endif  /* include-once check. */
