/*------------------------------------------------------------------------
 * Signum
 * Platform-specific includes and optimisations
 *-----------------------------------------------------------------------*/
#pragma once

#ifdef __APPLE__

    /*------------------------------------------------------------------------
	 * Unsightly workaround to prevent namespace collisions
	 * (with Delay object).
	 *-----------------------------------------------------------------------*/
    #define VIMAGE_H
    #include <Accelerate/Accelerate.h>

    /*------------------------------------------------------------------------
     * Mac platform-specific defines
	 *-----------------------------------------------------------------------*/
    #include "TargetConditionals.h"

    /*------------------------------------------------------------------------
     * Extension to TargetConditionals.h
	 * Useful to determine if we're targeting a Mac desktop
	 *-----------------------------------------------------------------------*/
    #ifndef TARGET_OS_OSX
    #define TARGET_OS_OSX (TARGET_OS_MAC && !TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR)
    #endif

#endif
