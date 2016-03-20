/*------------------------------------------------------------------------
 * Signum
 * Platform-specific includes and optimisations
 *-----------------------------------------------------------------------*/

#ifdef __APPLE__

    /*------------------------------------------------------------------------
	 * Unsightly workaround to prevent namespace collisions
	 * (with Delay object).
	 *-----------------------------------------------------------------------*/
    #define VIMAGE_H
    #include <Accelerate/Accelerate.h>

#endif
