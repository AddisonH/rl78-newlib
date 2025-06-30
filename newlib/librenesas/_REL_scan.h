#ifndef __REL_SCAN_H
#define __REL_SCAN_H

#if defined(__CA78K0R__)
#define FAR1 __far
#define FAR2
#else
#define FAR1
#define FAR2 __far
#endif

#define FALSE  0
#define TRUE   1
#define SPEC_N 2

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

int _REL_scan( int (*f_in)(),
                const char __far * RESTRICT __near * format,
                int __far * __near *ap,
                int __near *bk,
                int __near *total );

#endif
