/* src/pwr_config.h.  Generated from pwr_config.h.in by configure.  */
/* src/pwr_config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <IntelPowerGadgetLib.h> header file. */
/* #undef HAVE_INTELPOWERGADGETLIB_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <piapi.h> header file. */
/* #undef HAVE_PIAPI_H */

/* Set to 1 if powergadget is found during configuration */
/* #undef HAVE_POWERGADGET */

/* Set to 1 if powerinsight is found during configuration */
/* #undef HAVE_POWERINSIGHT */

/* Set to 1 if Python was found */
#define HAVE_PYTHON 1

/* Define to 1 if you have the <Python.h> header file. */
#define HAVE_PYTHON_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Set to 1 if SWIG is found during configure */
/* #undef HAVE_SWIG */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <ulxmlrpcpp/ulxmlrpcpp.h> header file. */
/* #undef HAVE_ULXMLRPCPP_ULXMLRPCPP_H */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Set to 1 if xmlrpc is found during configuration */
/* #undef HAVE_XMLRPC */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "pwrapi"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "pwrapi"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "pwrapi 1.0.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "pwrapi"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Set to 1 to use debugging features of the Power API */
/* #undef USE_DEBUG */

/* Version number of package */
#define VERSION "1.0.1"

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict, even
   though the corresponding Sun C compiler does, which causes
   "#define restrict _Restrict" in the previous line.  Perhaps some future
   version of Sun C++ will work with _Restrict; if so, it'll probably
   define __RESTRICT, just as Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
#endif
