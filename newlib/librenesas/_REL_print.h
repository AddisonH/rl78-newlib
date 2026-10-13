#ifndef __REL_PRINT_H
#define __REL_PRINT_H

#define FAR1
#define FAR2 __far

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

const char __far * _REL_print( int (*func)(int),
                                const char __far * RESTRICT format,
                                int __near * __near *ap,
                                int __near *total );

const char __far * _REL_print_tiny( int (*func)(int),
                                const char __far * RESTRICT format,
                                int __near * __near *ap,
                                int __near *total );

#endif
