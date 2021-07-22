/** trace memory allocation */
#ifdef WIN32
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define malloc(s) mmalloc(s)

#endif
#endif
