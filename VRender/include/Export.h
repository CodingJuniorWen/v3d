
#ifndef VRENDER_EXPORT_H
#define VRENDER_EXPORT_H

#ifdef VRENDER_STATIC_DEFINE
#  define VRENDER_EXPORT
#  define VRENDER_NO_EXPORT
#else
#  ifndef VRENDER_EXPORT
#    ifdef VRender_EXPORTS
        /* We are building this library */
#      define VRENDER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VRENDER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VRENDER_NO_EXPORT
#    define VRENDER_NO_EXPORT 
#  endif
#endif

#ifndef VRENDER_DEPRECATED
#  define VRENDER_DEPRECATED __declspec(deprecated)
#endif

#ifndef VRENDER_DEPRECATED_EXPORT
#  define VRENDER_DEPRECATED_EXPORT VRENDER_EXPORT VRENDER_DEPRECATED
#endif

#ifndef VRENDER_DEPRECATED_NO_EXPORT
#  define VRENDER_DEPRECATED_NO_EXPORT VRENDER_NO_EXPORT VRENDER_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VRENDER_NO_DEPRECATED
#    define VRENDER_NO_DEPRECATED
#  endif
#endif

#endif /* VRENDER_EXPORT_H */
