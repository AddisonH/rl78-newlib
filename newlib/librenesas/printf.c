#include <stdarg.h>
#include <stdio.h>
#include "_REL_print.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif	/* C99 */

static int _REL_pc( int );

int printf( const char __far * RESTRICT format, ... )
{
    va_list ap = 0;
    int total;

    total = 0;

#if defined(__CA78K0R__)
    va_starttop(ap, format);
#else /* !__CA78K0R__ */
    va_start(ap, format);
#endif /* __CA78K0R__ */

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
    va_end(ap);
    return total;
}

static int _REL_pc( int c )
{
    return putchar(c);
}
