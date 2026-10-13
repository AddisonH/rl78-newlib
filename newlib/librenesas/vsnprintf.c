#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

#include <stdarg.h>
#include <stdio.h>
#include "_REL_print.h"

static int _REL_snp( int );
static char __far *_REL_pointer;

static	size_t	count;
static	size_t	max_size;

int vsnprintf( char __far * restrict ptr, size_t size, const char __far * restrict format, va_list ap )
{
    int total;

    count = 0;
    max_size = (size == 0) ? 0 : size - 1;

    _REL_pointer = ptr;
    total = 0;

    while ( *format ) {
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                if (count < max_size) {
                    *_REL_pointer++ = '%';
                    count++;
                }
                format++;
                total++;
            } else
                format = _REL_print(_REL_snp, format,
                                    (int __near * __near *)&ap,
                                    &total);
        } else {
            if (count < max_size) {
                *_REL_pointer++ = *format++;
                count++;
            }
            else {
                format++;
            }
            total++;
        }
    }
    *_REL_pointer = '\0';
    return total;
}

static int _REL_snp( int c )
{
    if(count < max_size) {
        *_REL_pointer++ = (char)c;
        count++;
    }
    return 1;
}

#endif
