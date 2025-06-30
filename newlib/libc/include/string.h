/*
 * string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _STRING_H_
#define	_STRING_H_

#include "_ansi.h"
#include <sys/reent.h>
#include <sys/cdefs.h>
#include <sys/features.h>

#define __need_size_t
#define __need_NULL
#include <stddef.h>

#if __POSIX_VISIBLE >= 200809
#include <sys/_locale.h>
#endif

#if __BSD_VISIBLE
#include <strings.h>
#endif

_BEGIN_STD_C

void *	 memchr (const void *, int, size_t);
int 	 memcmp (const void *, const void *, size_t);
void *	 memcpy (void *__restrict, const void *__restrict, size_t);
void *	 memmove (void *, const void *, size_t);
void *	 memset (void *, int, size_t);
char 	*strcat (char *__restrict, const char *__restrict);
char 	*strchr (const char *, int);
int	 strcmp (const char *, const char *);
int	 strcoll (const char *, const char *);
char 	*strcpy (char *__restrict, const char *__restrict);
size_t	 strcspn (const char *, const char *);
char 	*strerror (int);
size_t	 strlen (const char *);
char 	*strncat (char *__restrict, const char *__restrict, size_t);
int	 strncmp (const char *, const char *, size_t);
char 	*strncpy (char *__restrict, const char *__restrict, size_t);
char 	*strpbrk (const char *, const char *);
char 	*strrchr (const char *, int);
size_t	 strspn (const char *, const char *);
char 	*strstr (const char *, const char *);

#if defined(__RL78__)

#undef __FAR
#ifdef __FAR_DATA__
#define __FAR
#else
#define __FAR __far
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __FAR * _COM_memchr_f(const void __FAR *s, int c, size_t n);
int _COM_memcmp_ff(const void __FAR *s1, const void __FAR *s2, size_t n);
void __FAR * _COM_memcpy_ff(void __FAR * __restrict s1, const void __FAR * __restrict s2, size_t n);
void __FAR * _COM_memmove_ff(void __FAR *s1, const void __FAR *s2, size_t n);
void __FAR * _COM_memset_f(void __FAR *s, int c, size_t n);
char __FAR * _COM_strcat_ff(char __FAR * __restrict s1, const char __FAR * __restrict s2);
char __FAR * _COM_strchr_f(const char __FAR *s, int c);
int _COM_strcmp_ff(const char __FAR *s1, const char __FAR *s2);
char __FAR * _COM_strcpy_ff(char __FAR * __restrict s1, const char __FAR * __restrict s2);
size_t _COM_strlen_f(const char __FAR *s);
char __FAR * _COM_strncat_ff(char __FAR * __restrict s1, const char __FAR * __restrict s2, size_t n);
int _COM_strncmp_ff(const char __FAR *s1, const char __FAR *s2, size_t n);
char __FAR * _COM_strncpy_ff(char __FAR * __restrict s1, const char __FAR * __restrict s2, size_t n);
size_t _COM_strcspn_ff(const char __FAR *s1, const char __FAR *s2);
char __FAR * _COM_strpbrk_ff(const char __FAR *s1, const char __FAR *s2);
char __FAR * _COM_strrchr_f(const char __FAR *s, int c);
size_t _COM_strspn_ff(const char __FAR *s1, const char __FAR *s2);
char __FAR * _COM_strstr_ff(const char __FAR *s1, const char __FAR *s2);

#ifdef __cplusplus
} /* extern "C" */

/* in C++, make these available in the std namespace also */
namespace std {
using ::_COM_memchr_f;
using ::_COM_memcmp_ff;
using ::_COM_memcpy_ff;
using ::_COM_memmove_ff;
using ::_COM_memset_f;
using ::_COM_strcat_ff;
using ::_COM_strchr_f;
using ::_COM_strcmp_ff;
using ::_COM_strcpy_ff;
using ::_COM_strlen_f;
using ::_COM_strncat_ff;
using ::_COM_strncmp_ff;
using ::_COM_strncpy_ff;
using ::_COM_strcspn_ff;
using ::_COM_strpbrk_ff;
using ::_COM_strrchr_f;
using ::_COM_strspn_ff;
using ::_COM_strstr_ff;
}
#endif /* __cplusplus */

/* NOTE: the HAVE_CONFIG_H definition is tested b/c we don't want the macros defined
         when building the lib with -mfar-data
   TODO: a better solution for this? */
#if defined(__FAR_ROM__) || (defined(__FAR_DATA__) && !defined(HAVE_CONFIG_H))
#define memchr(_s, _c, _n)     _COM_memchr_f((_s), (_c), (_n))
#define memcmp(_s1, _s2, _n)   _COM_memcmp_ff((_s1), (_s2), (_n))
#define memcpy(_s1, _s2, _n)   _COM_memcpy_ff((_s1), (_s2), (_n))
#define memmove(_s1, _s2, _n)  _COM_memmove_ff((_s1), (_s2), (_n))
#define memset(_s, _c, _n)     _COM_memset_f((_s), (_c), (_n))
#define strcat(_s1, _s2)       _COM_strcat_ff((_s1), (_s2))
#define strchr(_s, _c)         _COM_strchr_f((_s), (_c))
#define strcmp(_s1, _s2)       _COM_strcmp_ff((_s1), (_s2))
#define strcpy(_s1, _s2)       _COM_strcpy_ff((_s1), (_s2))
#define strlen(_s)             _COM_strlen_f((_s))
#define strncat(_s1, _s2, _n)  _COM_strncat_ff((_s1), (_s2), (_n))
#define strncmp(_s1, _s2, _n)  _COM_strncmp_ff((_s1), (_s2), (_n))
#define strncpy(_s1, _s2, _n)  _COM_strncpy_ff((_s1), (_s2), (_n))
#define strpbrk(_s1, _s2)      _COM_strpbrk_ff((_s1), (_s2))
#define strrchr(_s, _c)        _COM_strrchr_f((_s), (_c))
#define strspn(_s1, _s2)       _COM_strspn_ff((_s1), (_s2))
#define strstr(_s1, _s2)       _COM_strstr_ff((_s1), (_s2))
#define strcspn(_s1, _s2)      _COM_strcspn_ff((_s1), (_s2))
#endif  /* __FAR_ROM__ */

#undef __FAR

#endif  /* __RL78__ */


#ifndef _REENT_ONLY
char 	*strtok (char *__restrict, const char *__restrict);
#endif
size_t	 strxfrm (char *__restrict, const char *__restrict, size_t);

#if __POSIX_VISIBLE >= 200809
int	 strcoll_l (const char *, const char *, locale_t);
char	*strerror_l (int, locale_t);
size_t	 strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);
#endif
#if __MISC_VISIBLE || __POSIX_VISIBLE
char 	*strtok_r (char *__restrict, const char *__restrict, char **__restrict);
#endif
#if __BSD_VISIBLE
int	 timingsafe_bcmp (const void *, const void *, size_t);
int	 timingsafe_memcmp (const void *, const void *, size_t);
#endif
#if __MISC_VISIBLE || __POSIX_VISIBLE
void *	 memccpy (void *__restrict, const void *__restrict, int, size_t);
#endif
#if __GNU_VISIBLE
void *	 mempcpy (void *, const void *, size_t);
void *	 memmem (const void *, size_t, const void *, size_t);
void *	 memrchr (const void *, int, size_t);
void *	 rawmemchr (const void *, int);
#endif
#if __POSIX_VISIBLE >= 200809
char 	*stpcpy (char *__restrict, const char *__restrict);
char 	*stpncpy (char *__restrict, const char *__restrict, size_t);
#endif
#if __GNU_VISIBLE
char	*strcasestr (const char *, const char *);
char 	*strchrnul (const char *, int);
#endif
#if __MISC_VISIBLE || __POSIX_VISIBLE >= 200809 || __XSI_VISIBLE >= 4
char 	*strdup (const char *) __malloc_like __result_use_check;
#endif
char 	*_strdup_r (struct _reent *, const char *);
#if __POSIX_VISIBLE >= 200809
char 	*strndup (const char *, size_t) __malloc_like __result_use_check;
#endif
char 	*_strndup_r (struct _reent *, const char *, size_t);

/* There are two common strerror_r variants.  If you request
   _GNU_SOURCE, you get the GNU version; otherwise you get the POSIX
   version.  POSIX requires that #undef strerror_r will still let you
   invoke the underlying function, but that requires gcc support.  */
#if __GNU_VISIBLE
char	*strerror_r (int, char *, size_t);
#elif __POSIX_VISIBLE >= 200112
# ifdef __GNUC__
int	strerror_r (int, char *, size_t)
#ifdef __ASMNAME
             __asm__ (__ASMNAME ("__xpg_strerror_r"))
#endif
  ;
# else
int	__xpg_strerror_r (int, char *, size_t);
#  define strerror_r __xpg_strerror_r
# endif
#endif

/* Reentrant version of strerror.  */
char *	_strerror_r (struct _reent *, int, int, int *);

#if __BSD_VISIBLE
size_t	strlcat (char *, const char *, size_t);
size_t	strlcpy (char *, const char *, size_t);
#endif
#if __POSIX_VISIBLE >= 200809
size_t	 strnlen (const char *, size_t);
#endif
#if __BSD_VISIBLE
char 	*strsep (char **, const char *);
#endif
#if __BSD_VISIBLE
char    *strnstr(const char *, const char *, size_t) __pure;
#endif

#if __MISC_VISIBLE
char	*strlwr (char *);
char	*strupr (char *);
#endif

#if __POSIX_VISIBLE >= 200809
char	*strsignal (int __signo);
#endif

#ifdef __CYGWIN__
int	strtosigno (const char *__name);
#endif

#if __GNU_VISIBLE
int	 strverscmp (const char *, const char *);
#endif

#if __GNU_VISIBLE && defined(__GNUC__)
#define strdupa(__s) \
	(__extension__ ({const char *__sin = (__s); \
			 size_t __len = strlen (__sin) + 1; \
			 char * __sout = (char *) __builtin_alloca (__len); \
			 (char *) memcpy (__sout, __sin, __len);}))
#define strndupa(__s, __n) \
	(__extension__ ({const char *__sin = (__s); \
			 size_t __len = strnlen (__sin, (__n)) + 1; \
			 char *__sout = (char *) __builtin_alloca (__len); \
			 __sout[__len-1] = '\0'; \
			 (char *) memcpy (__sout, __sin, __len-1);}))
#endif /* __GNU_VISIBLE && __GNUC__ */

/* There are two common basename variants.  If you do NOT #include <libgen.h>
   and you do

     #define _GNU_SOURCE
     #include <string.h>

   you get the GNU version.  Otherwise you get the POSIX versionfor which you
   should #include <libgen.h>i for the function prototype.  POSIX requires that
   #undef basename will still let you invoke the underlying function.  However,
   this also implies that the POSIX version is used in this case.  That's made
   sure here. */
#if __GNU_VISIBLE && !defined(basename)
# define basename basename
char	*__nonnull ((1)) basename (const char *) __asm__(__ASMNAME("__gnu_basename"));
#endif

#include <sys/string.h>

_END_STD_C

#if __SSP_FORTIFY_LEVEL > 0
#include <ssp/string.h>
#endif

#endif /* _STRING_H_ */
