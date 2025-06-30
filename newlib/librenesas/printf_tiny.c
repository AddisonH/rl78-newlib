#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif	/* C99 */

#include <stdarg.h>
#include <stdio.h>
#include "_REL_print.h"
static int _REL_pc( int );

int printf_tiny( const char __far * RESTRICT format, ... )
{
    va_list ap;
    int total;

    total = 0;

    va_start(ap, format);

    while ( *format ) {
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                if( putchar('%') == EOF )
                    return EOF;
                format++;
                total++;
            } else if(!(format = _REL_print_tiny(_REL_pc, format,
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
    va_end(ap);
    return total;
}

static int _REL_pc( int c )
{
    return putchar(c);
}
