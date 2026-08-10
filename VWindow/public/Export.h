
#ifndef VWINDOW_EXPORT_H
#define VWINDOW_EXPORT_H

#ifdef VWINDOW_STATIC_DEFINE
#  define VWINDOW_EXPORT
#  define VWINDOW_NO_EXPORT
#else
#  ifndef VWINDOW_EXPORT
#    ifdef VWindow_EXPORTS
        /* We are building this library */
#      define VWINDOW_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VWINDOW_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VWINDOW_NO_EXPORT
#    define VWINDOW_NO_EXPORT 
#  endif
#endif

#ifndef VWINDOW_DEPRECATED
#  define VWINDOW_DEPRECATED __declspec(deprecated)
#endif

#ifndef VWINDOW_DEPRECATED_EXPORT
#  define VWINDOW_DEPRECATED_EXPORT VWINDOW_EXPORT VWINDOW_DEPRECATED
#endif

#ifndef VWINDOW_DEPRECATED_NO_EXPORT
#  define VWINDOW_DEPRECATED_NO_EXPORT VWINDOW_NO_EXPORT VWINDOW_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VWINDOW_NO_DEPRECATED
#    define VWINDOW_NO_DEPRECATED
#  endif
#endif

#endif /* VWINDOW_EXPORT_H */
