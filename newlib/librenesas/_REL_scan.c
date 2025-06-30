#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include "_REL_scan.h"

#define LONG_LONG 2

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

int _REL_scan( int (*f_in)(),
                const char __far * RESTRICT __near * format,
                int __far * __near *ap,
                int __near *bk,
                int __near *total )
{
    union ptr {
        char __far *c_p;
        int  __far *i_p;
        long __far *l_p;
#if !defined(__CA78K0R__)
        long long __far *ll_p;
#endif
        double __far *d_p;
        float __far *f_p;
        char __far * __far *c_pp;
    } __near *arg;
    int  no_flg;
    int  width;
    int  lh_flg;
    long data;
#if !defined(__CA78K0R__)
    long long data_ll = 0LL;
#endif
    double dbint;
    double dbdec;
    double keta;
    int  sign;
    int  esign;
    int  i;
    int  sisu;
    char type;
    int  c;
    const char __far *top;
    const char __far *end = NULL;
    const char __far *ptrtmp;
    int  circ_flg;
    int  mch_flg;
    int  status;

    int  bank;
    char __far *set;
    const char __far *ptr;

    arg = (union ptr __near *)ap;

    if ( **format == '*' ) {
        no_flg = TRUE;
        (*format)++;
    } else
        no_flg = FALSE;

    width = -1;
    if ( isdigit(**format) ) {
        width = 0;
        do {
            width = ( width * 10 ) + ( *(*format)++ & 0xf );
        } while ( isdigit( **format ) );
    }

    lh_flg = FALSE;
    if ( **format == 'l' ) {
        lh_flg = *(*format)++;
        if ( (*(*format - 1) == 'l') && (**format == 'l') ) {
            lh_flg = LONG_LONG;
            (*format)++;
        }
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    } else if ( **format == 'j' ) {
        lh_flg = LONG_LONG;
        (*format)++;
    } else if ( **format == 'h' ) {
        lh_flg = *(*format)++;
        if ( **format == 'h' ) {
            lh_flg = 'b';
            (*format)++;
        }
    } else if ( **format == 'L' || **format == 'z' || **format == 't' ) {
        lh_flg = *(*format)++;
    }
#else
    } else if ( **format == 'h' ) {
        lh_flg = *(*format)++;
    } else if ( **format == 'L' ) {
        lh_flg = *(*format)++;
    }
#endif

    status = FALSE;
    type = *(*format)++;
    sign = '+';
    c = (*f_in)();
    (*total)++;
    if ( type != 'c' && type != 'n' && type != '[' ){
        while ( isspace( c ) ) {
            c = (*f_in)();
            (*total)++;
        }
    }
    switch ( type ) {
        case 'i':
        case 'd':
        case 'u':
        case 'x':
    case 'X':
        case 'o':
            if ( !width )
                break;
            if ( c == '-' || c == '+' ) {
                sign = c;
                c = (*f_in)();
                (*total)++;
                if ( !--width )
                    break;
            }
            switch ( type ) {
                case 'd':
                case 'u':
                    goto dig;
                case 'x':
                case 'X':
                    goto hex;
                case 'o':
                    goto oct;
                default:
                    break;
            }
            if ( c == '0' ) {
                c = (*f_in)();
                (*total)++;
                if( !--width ) {
                    data = 0;
#if !defined(__CA78K0R__)
                    data_ll = 0LL;
#endif
                    status = TRUE;
                    break;
                }
                if ( c == 'x' || c == 'X' ){
                    c = (*f_in)();
                    (*total)++;
                    if ( !--width )
                        break;
                    goto hex;
                } else
                    goto oct;
            }
        dig:
            if ( !width || !isdigit( c ) )
                break;
#if !defined(__CA78K0R__)
            if ( lh_flg == LONG_LONG ){

                data_ll = 0LL;
                do {
                    data_ll = ( data_ll * 10LL ) + ( long long )( c & 0xf );

                    c = (*f_in)();
                    (*total)++;
                    width--;
                }while ( isdigit( c ) && width );
            }else{
#endif

                data = 0L;
                do {
                    data = ( data * 10L ) + ( long )( c & 0xf );

                    c = (*f_in)();
                    (*total)++;
                    width--;
                } while ( isdigit( c ) && width );
#if !defined(__CA78K0R__)
            }
#endif
            status = TRUE;
            break;
        hex:
            if ( !width || !isxdigit( c ) )
                break;
#if !defined(__CA78K0R__)
            if ( lh_flg == LONG_LONG ){

                data_ll =0LL;
                if(c=='0'){
                    c=(*f_in)();
                    width--;
                    if(c=='x'||c=='X'){
                        if(!isxdigit(c=(*f_in)()))
                            goto next;
                        }else if(!isxdigit(c)){
                            status = TRUE;
                            break;
                        }
                }
                do {
                    data_ll = data_ll * 16LL;
                    if ( isdigit( c ) )
                        data_ll += ( long long )( c - '0' );
                    else if ( islower( c ) )
                        data_ll += ( long long )( c - 'a' + 10 );
                    else
                        data_ll += ( long long )( c - 'A' + 10 );
                    c = (*f_in)();
                    (*total)++;
                    width--;
                } while ( isxdigit( c ) && width );
            } else {
#endif

        data = 0L;
        if(c=='0'){
            c=(*f_in)();
            width--;
            if(c=='x'||c=='X'){
                if(!isxdigit(c=(*f_in)()))
                    goto next;
                        } else if(!isxdigit(c)) {
                            status = TRUE;
                            break;
                        }
        }
                do {
                    data = data * 16L;
                    if ( isdigit( c ) )
                        data += ( long )( c - '0' );
                    else if ( islower( c ) )
                        data += ( long )( c - 'a' + 10 );
                    else
                        data += ( long )( c - 'A' + 10 );
                    c = (*f_in)();
                    (*total)++;
                    width--;
                } while ( isxdigit( c ) && width );
#if !defined(__CA78K0R__)
            }
#endif
next:
            status = TRUE;
            break;
        oct:
            if ( type == 'i' ) {

                status = TRUE;
            }
            if ( !width || ( c < '0' || c > '7' ) ) {
                break;
            }
#if !defined(__CA78K0R__)
            if ( lh_flg == LONG_LONG ){

                data_ll = 0LL;
                do {
                    data_ll = ( data_ll * 8LL ) + ( long long )( c & 0xf ) ;

                    c = (*f_in)();
                    (*total)++;
                    width--;
                } while ( ( c >= '0' && c <= '7' ) && width );
            } else {
#endif

                data = 0L;
                do {
                    data = ( data * 8L ) + ( long )( c & 0xf ) ;

                    c = (*f_in)();
                    (*total)++;
                    width--;
                } while ( ( c >= '0' && c <= '7' ) && width );
#if !defined(__CA78K0R__)
            }
#endif
            status = TRUE;
            break;
    case 'e':
    case 'E':
    case 'f':
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    case 'F':
#endif
    case 'g':
    case 'G':
            dbint = 0.0;
        dbdec = 0.0;
            if ( !width )
                break;
            if ( c == '-' || c == '+' ) {
                sign = c;
                c = (*f_in)();
                (*total)++;
                if ( !--width )
                    break;
            }
        if ( c == 'e' || c == 'E' ) {
         c = (*f_in)();
                (*total)++;
        break;
        }
        if ( !width || (!isdigit(c) && c != '.') )
                break;

        if ( isdigit(c) ) {
                do {
                   dbint = ( dbint * 10 ) + ( double )( c & 0xf );

                   c = (*f_in)();
                   (*total)++;
                   width--;
                } while ( isdigit( c ) && width );
                status = TRUE;
        }

        if ( c == '.' ) {
        c = (*f_in)();
                (*total)++;
                width--;
        }

        if ( isdigit(c) ) {
        keta = 1.0;
        do {
            keta /= 10;
            dbdec += keta * ( double )( c & 0xf );
            c = (*f_in)();
                    (*total)++;
                    width--;
        } while ( isdigit( c ) && width );
                status = TRUE;
        }
        dbint += dbdec;

        if ( status == TRUE && (c == 'e' || c == 'E') ) {
        c = (*f_in)();
                (*total)++;
                width--;
        if ( width && (c == '+' || c == '-') ) {
            esign = c;
            c = (*f_in)();
                    (*total)++;
                    width--;
                } else {
                    esign = '+';
                }
                if ( isdigit( c ) && width ) {
                    sisu = 0;
                    do {
                        sisu = (sisu*10) + (int)( c & 0xf );
                        c = (*f_in)();
                        (*total)++;
                        width--;
                    } while ( isdigit( c ) && width );
                    if ( esign == '-' ) {
                        for ( i=1; i<=sisu; i++ ) {
                            dbint /= 10;
                        }
                    }
                    else if (esign == '+' ) {
                        for ( i=1; i<=sisu; i++ ) {
                            dbint *= 10;
                        }
                    }
                }
        }
        break;

        case 'c':
            if ( !width || c == EOF )
                break;
            if ( width == -1 )
                width = 1;
            set = arg->c_p;
            do {
                if ( !no_flg )
                    *set++ = c;
                c = (*f_in)();
                (*total)++;
                width--;
            } while ( width && c != EOF );
            status = TRUE;
            break;
        case 's':
            if ( !width || c == EOF )
                break;
            set = arg->c_p;
            do {
                if ( !no_flg )
                    *set++ = c;
                c = (*f_in)();
                (*total)++;
                width--;
            } while ( width && !isspace( c ) && c != EOF );
            if ( !no_flg )
                *set = '\0';
            status = TRUE;
            break;
        case 'p':
            if ( !width || !isxdigit( c ) )
                break;

            data = 0L;
            do {
                data = data * 16L;
                if ( isdigit( c ) )
                    data += ( long )( c - '0' );
                else if ( islower( c ) )
                    data += ( long )( c - 'a' + 10 );
                else
                    data += ( long )( c - 'A' + 10 );
                c = (*f_in)();
                (*total)++;
            } while ( --width && isxdigit( c ) );
            data |= ( long )( bank ) << 16;
            status = TRUE;
            break;
        case '[':
            top = *format;
            if ( *top ) {

                ptrtmp = top + 1;
                if ( *top == '^' )
                    ptrtmp++;
                for (; *ptrtmp; ptrtmp++ ) {
                    if ( *ptrtmp == ']' ) {
                        end = ptrtmp;
                        break;
                    }
                }
            }
            if ( end == NULL ) {
        status = EOF;
                break;
            } else {
                *format = end + 1;
        }

            if ( !width || c == EOF )
                break;
            if ( *top == '^' ) {
                circ_flg = TRUE;
                top++;
            } else
                circ_flg = FALSE;
            set = arg->c_p;
            do {
                mch_flg = FALSE;

                for ( ptr = top ; ptr != end ; ) {

                    if ( ((ptr+2) < end)
                         && (*(ptr+1) == '-')
                         && ((unsigned char)*ptr < (unsigned char)*(ptr+2))){

                        if (((unsigned char)c >= (unsigned char)*ptr)
                            && ((unsigned char)c <= (unsigned char)*(ptr+2))) {

                            mch_flg = TRUE;
                            break;
                        }
                        ptr += 3;
                    } else if ( c == *ptr ) {
                        mch_flg = TRUE;
                        break;
                    } else {
                        ptr++;
                    }
                }
                if ( circ_flg ) {
                    if ( mch_flg )
                        break;
                    else {
                        status = TRUE;
                        if ( !no_flg )
                            *set++ = c;
                    }
                } else {
                    if ( mch_flg ) {
                        status = TRUE;
                        if ( !no_flg )
                            *set++ = c;
                    } else
                        break;
                }
                c = (*f_in)();
                (*total)++;
            } while ( --width && c != EOF );
            if ( !no_flg )
                *set = '\0';
            break;
        case 'n':

            status = SPEC_N;
            break;
        default:
            break;
    }
    *bk = c;

    if ( (status != TRUE && status != SPEC_N)
         || no_flg )
        return  status;

    switch ( type ) {
        case 'i':
        case 'd':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
            if ( sign == '-' ) {
                data = -data;
#if !defined(__CA78K0R__)
                data_ll = -data_ll;
#endif
            }
            if ( lh_flg == 'l' ) {
                *(arg->l_p) = data;
#if !defined(__CA78K0R__)
            } else if ( lh_flg == LONG_LONG ) {
                *(arg->ll_p) = data_ll;
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
            } else if ( lh_flg == 'b' ) {
                *(arg->c_p) = data;
#endif
            } else {
                *(arg->i_p) = ( int )data;
            }
            break;
    case 'e':
    case 'E':
    case 'f':
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    case 'F':
#endif
    case 'g':
    case 'G':
        if ( sign == '-' )
                dbint = -dbint;
        if ( lh_flg == 'l'  || lh_flg == 'L')
            *(arg->d_p) = dbint;
        else
        *(arg->f_p) = dbint;
        break;
        case 'p':
            *(arg->c_pp) = ( char __far * )data;
            break;
        case 'n':
            if ( lh_flg == 'l' ) {
                *(arg->l_p) = *total - 1;
#if !defined(__CA78K0R__)
            } else if ( lh_flg == LONG_LONG ) {
                *(arg->ll_p) = *total - 1;
#endif
            } else {
                *(arg->i_p) = *total - 1;
            }
            break;
        default:
            break;
    }
    return  status;
}
