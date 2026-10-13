#include <stdarg.h>
#include <stdio.h>
#include "_REL_print.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

static int _REL_sp( int );
static char __far *_REL_pointer;

int sprintf( char __far * RESTRICT ptr, const char __far * RESTRICT format, ... )
{
    va_list ap;
    int total;

    _REL_pointer = ptr;
    total = 0;

    va_start(ap, format);

    while ( *format ) {
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                *_REL_pointer++ = '%';
                format++;
                total++;
            } else
                format = _REL_print(_REL_sp, format,
                                    (int __near * __near *)&ap,
                                    &total);
        } else {
            *_REL_pointer++ = *format++;
            total++;
        }
    }
    *_REL_pointer = '\0';
    va_end(ap);
    return total;
}

static int _REL_sp( int c )
{
    *_REL_pointer++ = (char)c;
    return 1;
}
