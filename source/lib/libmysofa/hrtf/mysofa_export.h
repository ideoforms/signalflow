
#ifndef MYSOFA_EXPORT_H
#define MYSOFA_EXPORT_H

#ifdef MYSOFA_STATIC
#  define MYSOFA_EXPORT
#  define MYSOFA_NO_EXPORT
#else
#  ifndef MYSOFA_EXPORT
#    ifdef mysofa_shared_EXPORTS
        /* We are building this library */
#      define MYSOFA_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define MYSOFA_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef MYSOFA_NO_EXPORT
#    define MYSOFA_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef MYSOFA_DEPRECATED
#  define MYSOFA_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MYSOFA_DEPRECATED_EXPORT
#  define MYSOFA_DEPRECATED_EXPORT MYSOFA_EXPORT MYSOFA_DEPRECATED
#endif

#ifndef MYSOFA_DEPRECATED_NO_EXPORT
#  define MYSOFA_DEPRECATED_NO_EXPORT MYSOFA_NO_EXPORT MYSOFA_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MYSOFA_NO_DEPRECATED
#    define MYSOFA_NO_DEPRECATED
#  endif
#endif

#endif /* MYSOFA_EXPORT_H */
