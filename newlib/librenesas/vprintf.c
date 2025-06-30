#include <stdarg.h>
#include <stdio.h>
#include "_REL_print.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

static int _REL_pc( int );

int vprintf(const char __far * RESTRICT format, va_list ap)
{
    int total;

    total = 0;

    while ( *format ) {
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                if( putchar('%') == EOF )
                    return EOF;
                format++;
                total++;
            } else if(!(format = _REL_print(_REL_pc, format,
                                       (int __near * __near *)&ap,
                                       &total)))
                return EOF;
        } else {
            if( putchar( *format ) == EOF )
                return EOF;
            format++;
            total++;
        }
    }
    return total;
}

static int _REL_pc( int c )
{
    return putchar(c);
}
