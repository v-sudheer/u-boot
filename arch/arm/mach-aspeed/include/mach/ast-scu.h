#if defined(CONFIG_MACH_ASPEED_G6) || defined(CONFIG_MACH_ASPEED_G5) || defined(CONFIG_MACH_ASPEED_G4)
#include "ast-bmc-scu.h"
#elif defined(CONFIG_RT360_CAM)
#include "ast-cam-scu.h"
#else
#err "no define for ast-scu.h"
#endif
