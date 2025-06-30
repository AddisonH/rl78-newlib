#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include "_REL_scan.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

static int _REL_ss( void );
static const char __far *string;

int sscanf( const char __far * RESTRICT ptr, const char __far * RESTRICT format, ... )
{
    va_list ap;
    int __far * __near * arg;
    int total;
    int cnt;
    int status;
    int no_flg;
    int bk;
    int bk_end;

    string = ptr;
    cnt = total = 0;
    bk_end = FALSE;

    va_start(ap, format);

    arg = (int __far * __near *)ap;

    while ( *format ) {
        if ( isspace( *format ) ) {
            do {
                format++;
            } while ( isspace( *format ) );
            while ( isspace( *string ) ) {

                string++;
                total++;
            }
            if ( !( *string ) ) {
                if ( cnt )

                    return cnt;
                else
                    return EOF;
            }
        }
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                if ( *string != '%' ) {
                    if ( !*string )

                        return EOF;
                    return cnt;
                } else {
                    string++;
                    format++;
                    total++;
                }
            } else {
                if ( *format == '*' )
                    no_flg = TRUE;
                else
                    no_flg = FALSE;
                status = _REL_scan(_REL_ss, &format,
                                   arg,&bk,&total);
                if ( !no_flg )
                    arg++;
                switch ( status ) {
                case TRUE:
                    if ( !no_flg && !bk_end)

                        cnt++;
                    if ( bk == EOF ) {

                        bk_end = TRUE;
                        if ( *(format)=='\0' )
                            return cnt;
                        else
                            string++;
                    }
                    break;
                case SPEC_N:

                    if ( bk == EOF )

                        return cnt;
                    break;
                case FALSE:
                case EOF:
                    if ( bk == EOF && !bk_end)

                        return EOF;
                    else
                        return cnt;
                }
                string--;
                total--;
            }
        } else {
            if ( *format++ != *string ) {
                if ( !*string )

                    return EOF;
                else
                    return cnt;
            }
            string++;
            total++;
        }
    }
    return cnt;
}

static int _REL_ss( void )
{
    if ( *string )
        return *string++;
    else
        return EOF;
}
