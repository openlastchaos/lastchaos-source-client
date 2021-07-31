#include <Engine\Engine.h>
#include <GameMP/SessionProperties.h>
#include <GameMP/PlayerSettings.h>

/* rcg10042001 protect against Visual C-isms. */
#ifdef _MSC_VER
#define DECL_DLL _declspec(dllexport)
#endif

#ifdef PLATFORM_UNIX
#define DECL_DLL 
#endif

#include "..\Global.h"
#include "..\Common\Flags.h"
#include "..\Common\Common.h"
#include "..\Common\Particles.h"
#include "..\Common\EmanatingParticles.h"
#include "..\Common\GameInterface.h"

//강동민 수정 시작 접속 시퀀스 작업	06.03
#pragma comment(lib, "winmm.lib")
//강동민 수정 끝 접속 시퀀스 작업	06.03
