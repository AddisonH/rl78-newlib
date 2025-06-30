#include <stdio.h>
#include <ctype.h>
#include "_REL_print.h"

#define M_SHRP		0x04
#define M_MINUS		0x10

#define CHK_SHRP	(flag & M_SHRP)
#define CHK_MINUS	(flag & M_MINUS)

#define SET_SHRP	(flag |= M_SHRP)
#define SET_MINUS	(flag |= M_MINUS)

#define CLR_SHRP	(flag &= ~M_SHRP)
#define CLR_MINUS	(flag &= ~M_MINUS)
#define STRMAX	15
#define TRUE	1
#define FALSE	0
#define ZERO	0L
#define OCT	8L
#define DEC	10L
#define HEX	16L

typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef union ptr {
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    signed char		__near *sc_p;
    unsigned char	__near *uc_p;
#endif
    int  __near *i_p;
    unsigned int  __near *ui_p;
    long __near *l_p;
    unsigned long __near *ul_p;
    char __far * __near *c_pp;
} UNIONPTR;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

const char __far * _REL_print_tiny( int ( *func)(int),
                                const char __far * RESTRICT format,
                                int __near * __near *ap,
                                int __near *total )
{
    int i, j;
    UNIONPTR *arg;
    unsigned char flag;
    unsigned char p_flg;
    char lh_flg;
    char pad;
    int width;
    u_long data;
    char str[STRMAX+1];
    char __far *top;
    int num;
    int t_num;

    arg = (UNIONPTR *)ap;
    flag = lh_flg = 0;

    pad = ' ';
    while ( *format == '#' || *format == '0' ) {
        if ( *format == '#')
            SET_SHRP;
        else
            pad = '0';
        *format++;
    }

    width = 0;

    while ( isdigit( *format ) ) {
        width = ( width * 10 ) + ( *format++ & 0xf );
    }

    if ( *format == 'l' || *format == 'h' ){

        lh_flg = *format++;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        if ( (*(format - 1) == 'h') && (*format == 'h') ){
            lh_flg = 'b';
            *format++;
        }
#endif
    }

    p_flg = *format;

    switch ( p_flg ) {
    case 'd':
    case 'i':
        if ( lh_flg == 'l' ) {

            data = *(arg->l_p)++;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        } else if ( lh_flg == 'b' ) {
            data = (long)(*(arg->sc_p));
            (arg->i_p)++;
#endif
        } else {
            data = (long)(*(arg->i_p)++);
        }
        if ( (long)data < 0L ) {
            SET_MINUS;
            data = -((long)data);
        }
        break;
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if ( lh_flg == 'l' )

            data = ( *(arg->ul_p)++ );
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        else if ( lh_flg == 'b' ) {
            data = (u_long)(*(arg->uc_p));
            (arg->i_p)++;
        }
#endif
        else
            data = ( u_long )( *(arg->ui_p)++ );
        break;
    }

    i = STRMAX;
    t_num = 0;

    switch ( p_flg ) {
    case 'd':
    case 'i':
    case 'u':

        do {
            str[i--] = ( char )( data % DEC ) + '0';

            data /= DEC;
        } while ( data != ZERO );
        num = STRMAX - i;
        if ( CHK_MINUS ){
            num++;
            t_num = 1;
            str[i] = '-';
        }
        else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'x':
    case 'X':
        if ( !data )

            CLR_SHRP;

        do {
            str[i] = ( char )( data % HEX );

            if( str[i] >= 0xa )

                str[i] += (p_flg == 'X' ? 'A' : 'a') - 0xa;
            else
                   str[i] += '0';
            i--;
            data /= HEX;
        } while ( data != ZERO );
        num = STRMAX - i;
        if ( CHK_SHRP ) {

            num += 2;
            t_num = 2;
            str[i--] = p_flg;
            str[i] = '0';
        } else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'o':
        if ( !data )

            CLR_SHRP;

        do {
            str[i--] = ( char )( data % OCT ) + '0';

            data /= OCT;
        } while ( data != ZERO );
        num = STRMAX - i;
        if ( CHK_SHRP )	{

            num++;
            str[i] = '0';
        }
        else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'c':
        pad = ' ';
        str[i] = (char)*(arg->i_p)++;
        num = 1;
        top = (char __far *)&str[i];
        break;
    case 'p':
        pad = ' ';
        data = ( u_long )( *(arg->c_pp)++ );
        data &= 0xffffffL;
        for ( j = 0 ; j < 6 ; j++ ) {
            str[i] = ( ( char )data & 0xf );
            if( str[i] >= 0xa )
                str[i] = str[i] - 0xa + 'A';
            else
                str[i] = str[i] + '0';
            i--;
            data /= HEX;
        }
        for ( j = 0 ; j < 2 ; j++ ) {
            str[i] = '0';
            i--;
        }
        num = STRMAX - i;
        top = (char __far *)&str[i+1];
        break;
    case 's':
        pad = ' ';
        top = *(arg->c_pp)++;
        for ( num = 0 ; top[num] ; num++ ) ;
        break;
    default:
        return format;
    }

    if ( num < width ) {

        if ( t_num && pad == '0' ) {

            for ( i = 0 ; i < t_num ; i++ ) {
                if ( (*func)( (int)(*top++) ) == EOF )
                    return  0;
            }

            for ( i = 0 ; i < ( width - num ) ; i++ ) {
                if ( (*func)( '0' ) == EOF )
                    return  0;
            }
        } else {

            for ( i = 0 ; i < ( width - num ) ; i++ ) {
                if ( (*func)( (int)pad ) == EOF )
                    return  0;
            }

            for ( i = 0 ; i < t_num ; i++ ) {
                if ( (*func)( (int)(*top++) ) == EOF )
                    return  0;
            }
        }

        for ( i = 0 ; i < ( num - t_num ) ; i++ ) {
            if ( ( *func )( (int)(*top++) ) == EOF )
                return  0;
        }
        num = width;
    } else {

        for ( i = 0 ; i < t_num ; i++ ) {
            if ( ( *func )( (int)(*top++) ) == EOF )
                return  0;
        }

        for ( i = 0 ; i < ( num - t_num ) ; i++ ) {
            if ( (*func)( (int)(*top++) ) == EOF )
                return  0;
        }
    }
    *total += num;
    return ( format + 1 );
}
