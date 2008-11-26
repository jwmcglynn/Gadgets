// Chooses which pixman floating point extension should be used.

#ifdef USE_SSE2
#include "../pixman/pixman-sse2.c"
#endif

#ifdef USE_MMX
#include "../pixman/pixman-mmx.c"
#endif

#ifdef USE_VMX
#include "../pixman/pixman-vmx.c"
#endif
