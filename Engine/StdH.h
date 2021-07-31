
#define ENGINE_INTERNAL 1
#define ENGINE_EXPORTS 1

#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <search.h>   // for qsort
#include <float.h>    // for FPU control

#include <crtdbg.h>

#include <Engine/Xbox/XKeyboard.h>

  #include <winsock2.h>
  #include <windows.h>
  #include <mmsystem.h> // for timers

#include <Engine/Base/Types.h>
#include <Engine/Base/Assert.h>

#pragma warning(disable : 4786)
