#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "_REL_scan.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

static int _REL_sc( void );

#define GETCHAR_BK() ((st_isungetc == FALSE) ? (getchar())                    \
                                             : (st_isungetc = FALSE, st_bk))
#define UNGETC_BK(c) (st_isungetc = TRUE, st_bk = c)

static int st_bk = EOF;
static unsigned char st_isungetc = FALSE;

int scanf( const char __far * RESTRICT format, ... )
{
    va_list ap;
    int __far * __near *arg;
    int total;
    int cnt;
    int status;
    int no_flg;
    int c;
    int bk;
    int bk_end;

    cnt = total = 0;

    st_isungetc = FALSE;
    st_bk = EOF;
    bk_end = FALSE;

#if defined(__CA78K0R__)
    va_starttop(ap, format);
#else
    va_start(ap, format);
#endif

    arg = (int __far * __near *)ap;

    while ( *format ) {
        if ( isspace( *format ) ) {
            do {
                format++;
            } while ( isspace( *format ) );

            while ( isspace( c = GETCHAR_BK() ) )
                total++;
            if ( UNGETC_BK(c) == EOF ) {
                if ( cnt )
                    return cnt;
                else
                    return EOF;
            }
        }
        if ( *format == '%' ) {
            format++;
            if ( *format == '%' ) {
                c = GETCHAR_BK();
                if ( c != '%' ) {
                    if ( UNGETC_BK(c) == EOF )

                        return EOF;
                    return cnt;
                } else {
                    format++;
                    total++;
                }
            } else {
                if ( *format == '*' )
                    no_flg = TRUE;
                else
                    no_flg = 0;
                status = _REL_scan(_REL_sc,&format,
                                   arg,&bk,&total);
                if ( !no_flg )
                    arg++;
                switch ( status ) {
                case TRUE:
                    if ( !no_flg )

                        cnt++;
                    break;
                case SPEC_N:

                    break;
                case FALSE:
                case EOF:
                    if ( UNGETC_BK(bk) == EOF && !bk_end )

                        return EOF;
                    else
                        return cnt;
                }
                if ( UNGETC_BK(bk) == EOF )
                    bk_end = TRUE;
                total--;
            }
        } else {
            c = GETCHAR_BK();
            if ( *format++ != c ) {
                if ( UNGETC_BK(c) == EOF )

                    return EOF;
                else
                    return cnt;
            }
            total++;
        }
    }
    return cnt;
}

static int _REL_sc( void )
{
    return GETCHAR_BK();
}
