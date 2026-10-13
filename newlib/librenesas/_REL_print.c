#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include "_REL_float.h"
#include "_REL_print.h"

#define M_LEFT		0x0001
#define M_SIGN		0x0002
#define M_SHRP		0x0004
#define M_BLNK		0x0008
#define M_MINUS		0x0010
#define M_EFUGO		0x0020
#define M_G		    0x0040
#define M_EFG		0x0080
#define M_FUGOU		0x0100
#define M_GONYU		0x0200
#define M_KETA		0x0400

#define CHK_LEFT	(flag & M_LEFT)
#define CHK_SIGN	(flag & M_SIGN)
#define CHK_SHRP	(flag & M_SHRP)
#define CHK_BLNK	(flag & M_BLNK)
#define CHK_MINUS	(flag & M_MINUS)
#define CHK_EFUGO	(flag & M_EFUGO)
#define CHK_G		(flag & M_G)
#define CHK_EFG		(flag & M_EFG)
#define CHK_FUGOU	(flag & M_FUGOU)
#define CHK_GONYU	(flag & M_GONYU)
#define CHK_KETA	(flag & M_KETA)

#define SET_LEFT	(flag |= M_LEFT)
#define SET_SIGN	(flag |= M_SIGN)
#define SET_SHRP	(flag |= M_SHRP)
#define SET_BLNK	(flag |= M_BLNK)
#define SET_MINUS	(flag |= M_MINUS)
#define SET_EFUGO	(flag |= M_EFUGO)
#define SET_G		(flag |= M_G)
#define SET_EFG		(flag |= M_EFG)
#define SET_FUGOU	(flag |= M_FUGOU)
#define SET_GONYU	(flag |= M_GONYU)
#define SET_KETA	(flag |= M_KETA)

#define CLR_LEFT	(flag &= ~M_LEFT)
#define CLR_SIGN	(flag &= ~M_SIGN)
#define CLR_SHRP	(flag &= ~M_SHRP)
#define CLR_BLNK	(flag &= ~M_BLNK)
#define CLR_MINUS	(flag &= ~M_MINUS)
#define CLR_EFUGO	(flag &= ~M_EFUGO)
#define CLR_G		(flag &= ~M_G)
#define CLR_EFG		(flag &= ~M_EFG)
#define CLR_FUGOU	(flag &= ~M_FUGOU)
#define CLR_GONYU	(flag &= ~M_GONYU)
#define CLR_KETA	(flag &= ~M_KETA)

#define STRMAX	30
#define TRUE	1
#define ZERO	0L
#define OCT	8L
#define DEC	10L
#define HEX	16L
#define ZERO_LL	0LL
#define OCT_LL	8LL
#define DEC_LL	10LL
#define HEX_LL	16LL
#define LONG_LONG 2
#define FALSE	0

#define BFS 64
#define DBM 4
#define DBS BFS+DBM
#define BLEN 16
#define DIV (DTYP)10000
#define BMSK (DTYP)0x8000
#define DMSK (DTYP)0xffff
#define UNIT 4
#define DIG 6
#define EMAX 5
#if defined(__DBL8)
#define EFFECTIVE_DIGIT 17
#else
#define EFFECTIVE_DIGIT 8
#endif

typedef u_short DTYP;
typedef union ptr {
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    char  __near *c_p;
#endif
    int  __near *i_p;
    long __near *l_p;
#if !defined(__CA78K0R__)
    long long __near *ll_p;
#endif
    double __near *d_p;
    int  __far * __near *i_pp;
    long __far * __near *l_pp;
#if !defined(__CA78K0R__)
    long long __far * __near *ll_pp;
#endif
    char __far * __near *c_pp;
} UNIONPTR;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define	RESTRICT restrict
#else
#define	RESTRICT
#endif

static int _REL_fltprn( int (*func)(int), double db,
                        double db_org, int pres, char p_flg,
                        char p_flg_org, int cnt, char pad,
                        int width, int __near *total, u_int flag );
static void _REL_henkan1( double __near * db, FLOATP f );
static double _REL_henkan2( FLOATP f );
static int _REL_pri( int (*func)(int), char p_flg,
                    char __near *inte, char __near *deci,
                    char __near *estr, int seisu, _Bool is_exist_deci,
                    int last, int i, int deci_headzero_num,
                    int deci_footzero_num, int deci_num, int pres,
                    u_int flag);
static void _REL_fltgeti( DTYP __near *b, FLOATP f );
static int _REL_inmod( DTYP __near *w0, DTYP __near *sp, DTYP w1,
                        int __near *size );

const char __far * _REL_print( int (*func)(int),
                                const char __far * RESTRICT format,
                                int __near * __near *ap,
                                int __near *total )
{
    int i, j;
    UNIONPTR *arg;
    u_int flag;
    unsigned char p_flg;
    int lh_flg;
    char pad;
    int width;
    int pres;
    long get;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    signed char get_c;
#endif
#if !defined(__CA78K0R__)
    long long get_ll;
#endif
    double db;
    FLOAT dbdata;
    u_long data;
#if !defined(__CA78K0R__)
    unsigned long long data_ll;
#endif
    char str[STRMAX+1];
    char hex_char;
    char __far *top;
    int num;
    int t_num;
    int cnt = 0;
    double db_org = 0.0;
    unsigned char p_flg_org = 0;
    int pres_pad = 0;

    arg = (UNIONPTR *)ap;
    flag = lh_flg = 0;

    pad = ' ';
    while ( *format == '-' || *format == '+'
         || *format == ' ' || *format == '#'
         || *format == '0' ) {

        switch ( *format++ ) {
        case '-': SET_LEFT;
              pad = ' ';
              break;
        case '+': SET_SIGN;
              break;
        case '#': SET_SHRP;
              break;
        case ' ': SET_BLNK;
              break;
        case '0':

            if (!CHK_LEFT) {
                pad = '0';
            }
              break;
        }
    }

    width = 0;
    if ( isdigit(*format) ) {

        while ( isdigit( *format ) ) {
            width = ( width * 10 ) + ( *format++ & 0xf );
        }
    } else if ( *format == '*' ) {

        format++;
        width = *(arg->i_p)++;
        if ( width < 0 ) {

            SET_LEFT;
            width = -width;
        }
    }

    pres = -1;
    if ( *format == '.' ) {
        format++;
        if ( *format == '*' ) {

            format++;
            pres = *(arg->i_p)++;
            if ( pres < 0 )
                pres = -1;
        } else {
            pres = 0;
            while ( isdigit( *format ) )
                pres = ( pres * 10 ) + ( *format++ & 0xf );
        }
    }

  #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    if ( *format == 'l' || *format == 'h'|| *format == 'j' ){
  #else
    if ( *format == 'l' || *format == 'h' ){
  #endif

        lh_flg = *format++;
        if ( (*(format - 1) == 'l') && (*format == 'l') ){

            lh_flg = LONG_LONG;
            format++;
        }
  #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        else if( *(format - 1) == 'j' ) {
            lh_flg = LONG_LONG;
        } else if ( (*(format - 1) == 'h') && (*format == 'h') ){
            lh_flg = 'b';
            format++;
        }
  #endif
    }
  #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    else if ( *format == 'L' || *format == 'z' || *format == 't' )
  #else
    else if ( *format == 'L' )
  #endif

        *format++;

    p_flg = *format;

    switch ( p_flg ) {
    case 'd':
    case 'i':
        if ( lh_flg == 'l' ) {

            get = *(arg->l_p)++;
            if ( get < 0L ) {
                  SET_MINUS;
                   get = -get;
              }
            data = ( u_long )( get );
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
          } else if ( lh_flg == 'b' ) {
            get_c = *(arg->c_p);
            if (get_c < 0) {
                SET_MINUS;
                get_c = -get_c;
            }
            data = ( u_long )( get_c ) & 0xffL;
            (arg->i_p)++;
#endif
#if !defined(__CA78K0R__)
        } else if ( lh_flg == LONG_LONG ) {

            get_ll = *(arg->ll_p)++;
            if ( get_ll < 0LL ) {
                SET_MINUS;
                get_ll = -get_ll;
            }
            data_ll = ( unsigned long long )( get_ll);
#endif
        } else {
            get = ( long )( *(arg->i_p)++ );
            if ( get < 0L ) {
                  SET_MINUS;
                   get = -get;
              }
               data = ( u_long )( get ) & 0xffffL;
        }
        break;
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if ( lh_flg == 'l' )

            data = ( u_long )( *(arg->l_p)++ );
#if !defined(__CA78K0R__)
        else if ( lh_flg == LONG_LONG )

            data_ll = ( unsigned long long )( *(arg->ll_p)++ );
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        else if ( lh_flg == 'b' ) {
            data = ( u_long )( *(arg->c_p) ) & 0xffL;
            (arg->i_p)++;
        }
#endif
#endif
        else
            data = ( u_long )( *(arg->i_p)++ ) & 0xffffL;
        break;
    case 'e':
    case 'E':
    case 'f':
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    case 'F':
#endif
        db = *(arg->d_p)++ ;
        _REL_henkan1(&db, &dbdata);
        if (dbdata.sign) {

              SET_MINUS;
            dbdata.sign = 0;
            db = -db;
        }
        break;
    case 'g':
    case 'G':
        db = *(arg->d_p) ;
        _REL_henkan1(&db, &dbdata);
        if (dbdata.sign) {

              SET_MINUS;
            dbdata.sign = 0;
            db = -db;
        }
        break;
    }

    switch ( p_flg ) {
    case 'd':
    case 'i':
    case 'o':
    case 'u':
    case 'x':
    case 'X':
        if ( pres != -1 || (CHK_LEFT) )
            pad = ' ';
    }

    i = STRMAX;
    t_num = 0;

    switch ( p_flg ) {
    case 'd':
    case 'i':
    case 'u':
#if !defined(__CA78K0R__)
        if ( lh_flg == LONG_LONG ){

            if ( pres == 0 && data_ll == ZERO_LL ) {

                num = 0;
                break;
            }

            do {
                str[i--] = ( char )( data_ll % DEC_LL ) + '0';

                data_ll /= DEC_LL;
            } while ( data_ll != ZERO_LL );
        }else{
#endif
            if ( pres == 0 && data == ZERO ) {

                num = 0;
                break;
            }

            do {
                str[i--] = ( char )( data % DEC ) + '0';

                data /= DEC;
            } while ( data != ZERO );
#if !defined(__CA78K0R__)
        }
#endif
        num = STRMAX - i;
        if ( num < pres ) {
            pres_pad = pres - num;
            num += pres_pad;
        }

        if ( p_flg == 'u' ){
            CLR_SIGN;
            CLR_BLNK;
        }
        if ( CHK_MINUS || CHK_SIGN || CHK_BLNK ){

            num++;
            t_num = 1;
        }
        if ( CHK_MINUS )
            str[i] = '-';
        else if ( CHK_SIGN )
            str[i] = '+';
        else if ( CHK_BLNK )
            str[i] = ' ';
        else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'x':
    case 'X':
#if !defined(__CA78K0R__)
        if ( lh_flg == LONG_LONG ){

            if ( pres == 0 && data_ll == ZERO_LL ) {

                num = 0;
                break;
            }

            if ( p_flg == 'X' )
                hex_char = 'A';
            else
                hex_char = 'a';
            if ( !data_ll )

                CLR_SHRP;

            do {
                str[i] = ( char )( data_ll % HEX_LL );

                if( str[i] >= 0xa )
                    str[i] = str[i] - 0xa + hex_char;
                else
                       str[i] = str[i] + '0';
                i--;
                data_ll /= HEX_LL;
            } while ( data_ll != ZERO_LL );
        }else{
#endif
            if ( pres == 0 && data == ZERO ) {

                num = 0;
                break;
            }

            if ( p_flg == 'X' )
                hex_char = 'A';
            else
                hex_char = 'a';
            if ( !data )

                CLR_SHRP;

            do {
                str[i] = ( char )( data % HEX );

                if( str[i] >= 0xa )
                    str[i] = str[i] - 0xa + hex_char;
                else
                       str[i] = str[i] + '0';
                i--;
                data /= HEX;
            } while ( data != ZERO );
#if !defined(__CA78K0R__)
        }
#endif
        num = STRMAX - i;
        if ( num < pres ) {
            pres_pad = pres - num;
            num += pres_pad;
        }
        if ( CHK_SHRP ) {

            num += 2;
            t_num = 2;
        }
        if ( CHK_SHRP ) {

            str[i--] = p_flg;
            str[i] = '0';
        } else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'o':
#if !defined(__CA78K0R__)
        if ( lh_flg == LONG_LONG ){

            if ( !CHK_SHRP && pres == 0 && data_ll == ZERO_LL ) {

                num = 0;
                break;
            }
            if ( !data_ll )

                CLR_SHRP;

            do {
                str[i--] = ( char )( data_ll % OCT_LL ) + '0';

                data_ll /= OCT_LL;
            } while ( data_ll != ZERO_LL );
        }else{
#endif
            if ( !CHK_SHRP && pres == 0 && data == ZERO ) {

                num = 0;
                break;
            }
            if ( !data )

                CLR_SHRP;

            do {
                str[i--] = ( char )( data % OCT ) + '0';

                data /= OCT;
            } while ( data != ZERO );
#if !defined(__CA78K0R__)
        }
#endif
        num = STRMAX - i;
        if ( CHK_SHRP )

            num++;
        if ( num < pres ) {
            pres_pad = pres - num;
            num += pres_pad;
        }
        if ( CHK_SHRP )

            str[i] = '0';
        else
            i++;
        top = (char __far *)&str[i];
        break;
    case 'e':
    case 'E':
    case 'f':
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    case 'F':
#endif
    case 'g':
    case 'G':

        SET_EFG;
        if (dbdata.exp == DBL_INFEXP) {

            const char __far *p;
#if defined(__DBL8)
            if ((dbdata.mant.ml != 0) || (dbdata.mant2.ml != 0)) {
#else
            if (dbdata.mant.ml) {
#endif
                p = (p_flg == 'e' || p_flg == 'f' || p_flg == 'g')
                                 ? "-nan" : "-NAN";
                CLR_MINUS;
            } else {
                p = (p_flg == 'e' ||p_flg =='f' || p_flg == 'g')
                                 ? "-inf" : "-INF";
            }
            if (!(CHK_MINUS))
                p++;
            while (*p != '\0') {
                if ((*func)((int)(*p++)) == EOF)
                    return  0;
                *total += 1;
            }
            if ( p_flg == 'g' || p_flg == 'G' )
                *(arg->d_p)++;
            break;
        }
        if ( pres == 0 && db == 0.0 ){

            num = 0;
        }
        if( pres == -1 )
            pres = 6;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        if ( (p_flg != 'f' && p_flg != 'F') && db != 0.0 ) {
#else
        if ( p_flg != 'f' && db != 0.0 ) {
#endif
            if ( db < 1.0 ) {

                while ( db < 1.0 ) {
                    db = db * 10;
                    cnt++;
                 }
                 SET_EFUGO;
            } else {

                 while ( db >= 10.0 ) {
                    db = db / 10.0;
                    cnt++;
                 }
                 CLR_EFUGO;
            }
        }
        if ( p_flg == 'g' || p_flg == 'G' ) {

            SET_G;

            db_org = *(arg->d_p);
            p_flg_org = p_flg;

            if( pres == 0 )
                pres = 1;

            if ( (CHK_EFUGO && cnt > 4)
                 || (!CHK_EFUGO && cnt >= pres ) ){
                 if ( p_flg == 'g' )

                    p_flg = ( char )'e';
                 else if ( p_flg == 'G' )

                    p_flg = ( char )'E';
                 pres -= 1;
            } else {

                 p_flg = ( char )'f';
                 db = *(arg->d_p) ;
                 _REL_henkan1(&db, &dbdata);
                if (dbdata.sign) {
                    dbdata.sign = 0;
                    db = -db;
                }
                if (db==0)
                    ;
                else if ( !CHK_EFUGO )
                    pres -= (cnt + 1);

                else if (CHK_EFUGO)
                    pres -= ( (-1)*cnt + 1 );
            }
            *(arg->d_p)++ ;
        }

        if (_REL_fltprn( func, db, db_org, pres, p_flg, p_flg_org,
                         cnt, pad, width, total, flag ))
            return  0;
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
            str[i] = ( char )( data & 0xf );
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
    case 'n':
        if( lh_flg == 'l' ) {
            **(arg->l_pp) = *total;
            *(arg->l_pp)++ ;
#if !defined(__CA78K0R__)
        } else if ( lh_flg == LONG_LONG ) {
            **(arg->ll_pp) = *total;
            *(arg->ll_pp)++;
#endif
        } else{
            **(arg->i_pp) = *total;
            *(arg->i_pp)++;
        }
        num = 0;
        return ( format + 1 );
    case 's':
        pad = ' ';
        top = *(arg->c_pp)++;
        for ( num = 0 ; num != pres && top[num] ; num++ ) ;
        break;
    default:
        return format;
    }

    if ( !CHK_EFG ) {

        if ( num < width ) {

        if ( CHK_LEFT ) {

            for ( i = 0 ; i < t_num ; i++ ) {
                if ( (*func)( (int)(*top++) ) == EOF )
                    return  0;
            }

            for ( i = 0 ; i < pres_pad ; i++ ) {
                if ( ( *func )( '0' ) == EOF )
                    return  ( char * )FALSE;
            }

            for ( i = 0 ; i < ( num - t_num - pres_pad ) ; i++ ) {
                if ( ( *func )( (int)(*top++) ) == EOF )
                    return  ( char * )FALSE;
            }

            for ( i = 0 ; i < ( width - num ) ; i++ ) {
                if ( (*func)( ' ' ) == EOF )
                    return  0;
            }
        } else {
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

            for ( i = 0 ; i < pres_pad ; i++ ) {
                if ( ( *func )( '0' ) == EOF )
                    return  ( char * )FALSE;
            }

            for ( i = 0 ; i < ( num - t_num - pres_pad ) ; i++ ) {
                if ( ( *func )( (int)(*top++) ) == EOF )
                    return  ( char * )FALSE;
            }
        }
        num = width;
        } else {

        for ( i = 0 ; i < t_num ; i++ ) {
            if ( ( *func )( (int)(*top++) ) == EOF )
                return  ( char * )FALSE;
        }

        for ( i = 0 ; i < pres_pad ; i++ ) {
            if ( ( *func )( '0' ) == EOF )
                return  ( char * )FALSE;
        }

        for ( i = 0 ; i < ( num - t_num - pres_pad ) ; i++ ) {
            if ( (*func)( (int)(*top++) ) == EOF )
                return  0;
        }
        }
        *total += num;
    }
    return ( format + 1 );
}

static int _REL_fltprn( int (*func)(int), double db,
                        double db_org, int pres, char p_flg,
                        char p_flg_org, int cnt, char pad,
                        int width, int __near *total, u_int flag )
{
    DTYP b[DBS];
    char inte[DBL_MAX_10_EXP+1];
    char deci[DBL_MAX_10_EXP+1];
    DTYP sp;
    FLOAT f;
    char estr[EMAX+1];
    int x, y, i, seisu,r;
    int size;
    int last;
    int strcnt;
    int unit;
    int aft_cnt=1;
    int pre_seisu=0;
    int pres_org = pres;
    int cnt_org = cnt;
    int flag_org = flag;
    int deci_headzero_num = 0;
    int deci_footzero_num = 0;
    int first_not0_place = 0;

    int deci_num = 0;
    _Bool is_exist_deci;

ENTER_R8PRI:

    last = strcnt = 0;
    _REL_henkan1(&db,&f);
    if (f.exp == DBL_INFEXP) {

        return 1;
    }

    for (r=0; r < DBL_MAX_10_EXP+1; )
        inte[r++] = '0';

    for (r=0; r < DBL_MAX_10_EXP+1; )
        deci[r++] = '0';

    is_exist_deci = 1;

    deci_headzero_num = 0;

    deci_footzero_num = 0;

    first_not0_place = 0;

    deci_num = 0;

    _REL_fltgeti(b, &f);

    for (size=BFS, seisu=0; ; ) {

        if (!_REL_inmod(b, &sp, DIV, &size))
            break;
        for (y = 0; y < UNIT; y++) {

            inte[seisu++] = (char)( (sp % (DTYP)10) + '0');
            sp /= (DTYP)10;
            if (inte[seisu-1] != '0') {

                first_not0_place = seisu;
            }
        }
    }

    db = _REL_henkan2(&f);
    if (db != 0.0) {
        int require_digit;

        if (first_not0_place == 0) {

            require_digit = pres + 1;
        } else {

            require_digit = EFFECTIVE_DIGIT+1 - first_not0_place;
            if (require_digit < 0) {

                require_digit = 0;
            }
            else if (require_digit > pres + 1) {

                require_digit = pres + 1;
            }
        }

        for ( r = 0; r < require_digit; r++ ) {

            db *= (double)10;
            if (first_not0_place == 0 && (int)db != 0) {

                first_not0_place = -(r+1);
            }
            if (first_not0_place == 0) {

                ++deci_headzero_num;
            } else {

                ++deci_num;
            }

            if (first_not0_place > 0 &&
                first_not0_place + r+1 >= EFFECTIVE_DIGIT+1) {

                deci_footzero_num = pres - r;
                break;
            }
            else if (first_not0_place < 0 &&
                     r + first_not0_place+2 >= EFFECTIVE_DIGIT+1) {

                deci_footzero_num = pres - r;
                break;
            }
        }
        if (first_not0_place == 0) {

            --deci_headzero_num;
        } else {

            if (deci_num>0) {
                --deci_num;
            }
        }

    } else {

        deci_footzero_num = pres;
    }

    _REL_henkan1(&db,&f);

    _REL_fltgeti(b, &f);

    for (size=BFS, x=0; ; ) {

        if (!_REL_inmod(b, &sp, DIV, &size))
            break;

        if ( !x ) {
            if ( ( sp % (DTYP)10) >= 5 )
                SET_GONYU;
            sp /= (DTYP)10;
            unit = UNIT - 1;
        } else
            unit = UNIT;

        for (y = 0; y < unit; y++) {

            deci[x++] = (char)( (sp % (DTYP)10) + '0');
            sp /= (DTYP)10;
        }
    }

    if ( CHK_GONYU ) {
        for ( r=0; r<deci_num; r++ ){
            if ( deci[r] == '9' )
                deci[r] = '0';
            else {
                ++deci[r];
                break;
            }
        }
        if ( r==deci_num ) {
            if (deci_headzero_num > 0) {

                ++deci[deci_num];
                ++deci_num;
                --deci_headzero_num;
            } else {

                SET_KETA;
            }
        }
    }

    if ( CHK_G && !CHK_SHRP ) {

        for ( last=0; last<deci_num; last++ ){
            if ( deci[last] != '0' )
                break;
        }
        if ( last == deci_num ) {
            is_exist_deci = 0;
        } else {

            strcnt += deci_headzero_num + (deci_num - last);
        }
    } else {

        strcnt += deci_headzero_num + deci_num + deci_footzero_num;
    }

    if ( CHK_G && CHK_SHRP && ( p_flg == 'f' || p_flg == 'F' ) ) {
        if ( CHK_EFUGO ){

            aft_cnt += deci_headzero_num;
            for ( r=deci_num; r>0; r-- ){
                if( deci[r-1] == '0' ){
                    aft_cnt++;
                } else {
                    break;
                }
            }
        }

        if ( CHK_KETA ){

            for ( pre_seisu=seisu-1; pre_seisu >= 0; --pre_seisu ){
                if ( inte[pre_seisu] != '0' ){
                    break;
                }
            }
        }
    }

    if ( CHK_KETA ) {
        if ( (p_flg == 'e' || p_flg == 'E') && inte[0]=='9' ) {

            inte[0] = '1';
            if ( CHK_EFUGO ) {

                cnt--;

                if ( CHK_G && cnt <= 4 ) {

                    p_flg = ( char )'f';
                    db = db_org;
                    pres = pres_org;
                    cnt  = cnt_org;
                    flag = flag_org;

                    pres += 1;
                    if ( !CHK_EFUGO )

                        pres -= (cnt + 1);
                    else if (CHK_EFUGO)
                        pres -= ( (-1)*cnt + 1 );

                    goto	ENTER_R8PRI;
                }

                if ( !cnt )

                    CLR_EFUGO;
            } else
                cnt++;
        } else {
            for ( r=0; r<seisu; r++ ) {
                if ( inte[r] == '9' )
                    inte[r] = '0';
                else {
                    ++inte[r];
                    break;
                }
            }
            if ( r==seisu && r!=0) {
                inte[seisu] = '1';
                seisu++;
            }
            else if(seisu == 0){
                inte[seisu] = '1';
                seisu++;
            }
        }
    }

    if (seisu) {

        for (--seisu; seisu >= 0; seisu--) {
            if (inte[seisu] != '0')
                break;
        }
    }

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    if( CHK_G && (p_flg == 'f' || p_flg == 'F') && !CHK_EFUGO && seisu >= pres+cnt+1 ) {
#else
    if( CHK_G && p_flg == 'f' && !CHK_EFUGO && seisu >= pres+cnt+1 ) {
#endif

        if ( p_flg_org == 'g' ) {
            p_flg = ( char )'e';
        } else {
            p_flg = ( char )'E';
        }
        db = db_org;
        pres = pres_org;
        cnt  = cnt_org;
        flag = flag_org;

        pres -= 1;
        if ( !CHK_EFUGO ) {
            pres += (cnt+1);
        } else if (CHK_EFUGO) {
            pres += ( (-1)*cnt + 1 );
        }

        if ( db != 0.0 ) {
            if ( db < 1.0 ) {

                while ( db < 1.0 ) {
                    db = db * 10;
                }
            } else {

                while ( db >= 10.0 ) {
                    db = db / 10;
                }
            }
        }

        goto	ENTER_R8PRI;
    }

    if ( CHK_G
         && CHK_SHRP
         && ( p_flg == 'f' || p_flg == 'F' )
         && ( ( CHK_KETA && (pre_seisu != seisu) )
              || (CHK_EFUGO && (cnt != aft_cnt) ) ) ) {

        ++last;
        --strcnt;
    }

    strcnt += (seisu + 1);

    if ( CHK_MINUS || CHK_SIGN || CHK_BLNK ){
        strcnt ++;
        SET_FUGOU;
    }
    if ( CHK_MINUS )
        inte[++seisu] = '-';
    else if ( CHK_SIGN )
        inte[++seisu] = '+';
    else if ( CHK_BLNK )
        inte[++seisu] = ' ';

    if (pres>0 && CHK_G && !is_exist_deci && (!CHK_SHRP));

    else if ( pres>0 || (pres<=0 && CHK_SHRP))
        strcnt += 1;

    if ( p_flg == 'e' || p_flg == 'E' ) {

        i = EMAX ;
        do {
            estr[i--] = ( char )( cnt % 10 ) + '0';
            cnt /= 10;
        } while ( cnt != 0 );
        if ( i == EMAX - 1 )
            estr[i--] = '0';
        if ( CHK_EFUGO )
            estr[i--] = '-';
        else
            estr[i--] = '+';
        if ( p_flg == 'e' )
            estr[i] = 'e';
        else if ( p_flg == 'E' )
            estr[i] = 'E';
        strcnt += (EMAX - i + 1);
        }

    if ( strcnt < width ) {
        if ( CHK_LEFT ){

            if ( _REL_pri(func, p_flg, inte, deci, estr,
                seisu, is_exist_deci, last, i, deci_headzero_num,
                deci_footzero_num, deci_num, pres, flag) )
                return  1;
            for ( x = 0; x < (width-strcnt); x++ ) {

                if ( (*func)(' ') == EOF )
                    return 1;
            }
        } else {
            if ( (CHK_FUGOU) && pad == '0' ) {
                if ( ( *func )( (int)inte[seisu--] ) == EOF )

                    return  1;
                for ( x=0; x < (width-strcnt); x++ ) {

                    if ( (*func)('0') == EOF )
                    return 1;
                }

                if ( _REL_pri(func, p_flg, inte, deci, estr,
                   seisu, is_exist_deci, last, i,
                   deci_headzero_num, deci_footzero_num,
                   deci_num, pres, flag) )
                    return  1;
            } else {
                for ( x=0; x < (width-strcnt) ;x++ ) {

                    if( (*func)( (int)pad ) == EOF )
                        return 1;
                }

                 if ( _REL_pri(func, p_flg, inte, deci, estr,
                    seisu, is_exist_deci, last, i,
                    deci_headzero_num, deci_footzero_num,
                    deci_num, pres, flag) )
                    return  1;
            }
        }
        strcnt = width;
    } else {

        if ( _REL_pri(func, p_flg, inte, deci, estr, seisu,
                      is_exist_deci, last, i, deci_headzero_num,
                      deci_footzero_num, deci_num, pres, flag) )
            return  1;
    }
    *total += strcnt;
    return 0;
}

void _REL_henkan1( double __near * db, FLOATP f )
{
#if defined(__DBL8)
    MANT upl;
    upl.ml = *(((unsigned long*)db)+1);

    f->sign = (upl.mc[3] >> 7);
    f->exp  = ((upl.ms[1] << 1) >> 5);
    upl.mc[3] = 0;
    upl.mc[2] &= 0x0F;
    f->mant.ml = (upl.ml) & 0x000FFFFF;
    f->mant2.ml = *((unsigned long*)db);

#else
    MANT upl;
    upl.ml = *((unsigned long*)db);

    f->sign = (upl.mc[3] >> 7);
    f->exp  = ((upl.ms[1] << 1) >> 8);
    f->mant.ml = (upl.ml) & 0x007FFFFF;

#endif
}

double _REL_henkan2( FLOATP f )
{
    double r;
    MANT ldata;

#if defined(__DBL8)

    ldata.ms[0] = f->mant.ms[0];
    ldata.mc[2] = f->mant.mc[2] & 0xF;
    ldata.mc[2] |= (unsigned char)(f->exp << 4);
    ldata.mc[3] = (unsigned char)(f->exp >> 4) & 0x7F;
    ldata.mc[3] |= (f->sign & 0x1) << 7;

    *((unsigned long*)&r) = f->mant2.ml;
    *(((unsigned long*)&r)+1) = ldata.ml;

#else

    ldata.ms[0] = f->mant.ms[0];
    ldata.mc[2] = f->mant.mc[2] & 0x7F;
    ldata.mc[2] |= ((unsigned char)(f->exp)<<7);
    ldata.mc[3] = (unsigned char)((f->exp)>>1) & 0x7F;
    ldata.mc[3] |= ((f->sign) & 0x1)<<7;

    *((unsigned long*)&r) = ldata.ml;

#endif

    return r;
}

static int _REL_pri( int (*func)(int), char p_flg,
                    char __near *inte, char __near *deci,
                    char __near *estr, int seisu, _Bool is_exist_deci,
                    int last, int i, int deci_headzero_num,
                    int deci_footzero_num, int deci_num, int pres,
                    u_int flag)
{
    int x;
    int effc_cnt = EFFECTIVE_DIGIT;

    if ( CHK_MINUS || CHK_SIGN || CHK_BLNK ){

        ++effc_cnt;
        if (seisu == 1 && inte[0] == '0') {

            ++effc_cnt;
        }
    }
    if (seisu || (!seisu && inte[0] != '0') ) {
        for (; seisu >= 0; seisu--) {
            if (effc_cnt <= 0) {
                if ( ( *func )( (int)'0' ) == EOF )
                    return  1;
            } else {
                if ( ( *func )( (int)inte[seisu] ) == EOF )
                    return  1;
                --effc_cnt;
            }
        }
    } else if ( CHK_KETA ) {
        if ( ( *func )( '1' ) == EOF )
            return  1;
        --effc_cnt;
    } else if ( ( *func )( '0' ) == EOF )
        return  1;

    if ( pres>0 && CHK_G && !is_exist_deci && (!CHK_SHRP) ){

        if ( p_flg == 'e' || p_flg == 'E' ) {
            for ( ; i<=EMAX ; i++ ) {
                if ( (*func)( (int)(estr[i]) ) == EOF )
                    return 1;
            }
        }
        return 0;
    }else if ( pres>0 || (pres<=0 && CHK_SHRP) ) {
        if ( ( *func )( '.' ) == EOF )
            return  1;
    }

    if ( pres > 0 && deci_headzero_num > 0 ) {
        for (x = 0; x < deci_headzero_num && pres > 0; ++x, --pres) {
            if ( ( *func )( (int)'0' ) == EOF ) {
                return  1;
            }
            if (effc_cnt != EFFECTIVE_DIGIT) {

                --effc_cnt;
            }
        }
        if (effc_cnt < 0) {
            effc_cnt = 0;
        }
    }

    if ( pres > 0 && deci_num > 0 ) {
        for (x = deci_num-1; x >= last && pres > 0; --x, --pres) {
            if (effc_cnt <= 0) {
                if ( ( *func )( (int)'0' ) == EOF )
                    return  1;
            } else {
                if ( ( *func )( (int)deci[x] ) == EOF )
                    return  1;
                --effc_cnt;
            }
        }
    }

    if ( (!CHK_G || CHK_SHRP) && pres > 0 && deci_footzero_num > 0 ) {
        for (x = 0; x < deci_footzero_num && pres > 0; ++x, --pres) {
            if ( ( *func )( (int)'0' ) == EOF ) {
                return  1;
            }
        }
    }

      if ( p_flg == 'e' || p_flg == 'E' ) {
        for ( ; i<=EMAX ; i++ ) {
            if ( (*func)( (int)(estr[i]) ) == EOF )
                return 1;
        }
       }
    return  0;
}

void _REL_fltgeti( DTYP __near *b, FLOATP f )
{
    int x;
    DTYP c1, c2, m;
    unsigned char cl1, cl2;
    int j, k;
    MANT l;

    for (x = 0; x < DBS; )
        b[x++] = 0;

    if ((!f->exp) && (!f->mant.ml) && (!f->mant2.ml))
        return;
#if defined(__DBL8)
    if (f->exp < 0x3ff)
        return;
#else
    if (f->exp < 0x7f)
        return;
#endif

    b[BFS-1] = 1L;

#if defined(__DBL8)
    b[BFS+0] = (DTYP)(f->mant.ml >> 4);
    b[BFS+1] = (DTYP)f->mant.mc[0] << 12;
    b[BFS+1] |= (f->mant2.ms[1] >> 4);
    b[BFS+2] = (DTYP)(f->mant2.ml >> 4);
    b[BFS+3] = f->mant2.mc[0] << 12;
#else
    b[BFS+0] = (DTYP)(f->mant.ml >> 7);
    b[BFS+1] = (DTYP)f->mant.mc[0] << 9;
    b[BFS+2] = 0;
    b[BFS+3] = 0;
#endif

#if defined(__DBL8)
    j = (f->exp - 0x3ff) / BLEN;
#else
    j = (f->exp - 0x7f) / BLEN;
#endif
    if (j) {
        for (x=0; x+j < DBS; x++)
            b[x] = b[x+j];
        for (; x < DBS; x++)
            b[x] = 0;
        f->exp -= (j * BLEN);
    }

#if defined(__DBL8)
    j = f->exp - 0x3ff;
    f->exp = 0x3ff;
#else
    j = f->exp - 0x7f;
    f->exp = 0x7f;
#endif
    if (j) {
        k = BLEN - j;
        m = DMSK << k;
        for (c1 = 0, x = DBS-1; x >= 0; x--) {
            c2 = ((DTYP)(b[x] & m) >> k);
            b[x] <<= j;
            b[x] |= c1;
            c1 = c2;
        }
    }

#if defined(__DBL8)
    l.ml = (unsigned long)b[BFS+2] << 4;
    l.mc[0] |= (unsigned char)(b[BFS+3] >> 12);
    l.ms[1] |= b[BFS+1] << 4;
    f->mant2.ml = l.ml;
    l.ml = (unsigned long)b[BFS+0] << 4;
    l.mc[0] |= (unsigned char)(b[BFS+1] >> 12);
    f->mant.ml = l.ml;
#else
    f->mant2.ml = 0;
    l.ml = (unsigned long)b[BFS+0] << 7;
    l.mc[0] |= (unsigned char)(b[BFS+1] >> 9);
    f->mant.ml = l.ml;
#endif

    if ((! f->mant.ml) && (! f->mant2.ml)) {
        f->exp = 0;
        return;
    }

    for ( ; ; ) {
        f->exp--;
#if defined(__DBL8)
        cl1 = f->mant.mc[2] & 0x8;
        cl2 = ((f->mant2.mc[3] & 0x80) ? 1 : 0);
#else
        cl1 = f->mant.mc[2] & 0x40;
        cl2 = 0;
#endif
        f->mant2.ml = (u_long)f->mant2.ml << 1;
        f->mant.ml = (u_long)f->mant.ml << 1;
        f->mant.mc[0] |= cl2;
        if (cl1)
            break;
    }
}

static int _REL_inmod( DTYP __near *w0, DTYP __near *sp, DTYP w1,
                             int __near *size )
{
    register int x;
    static DTYP qt[BFS];
    DTYP c;
    int st;
    int bfs;
    int blen;
    DTYP msk;
    DTYP seb;
    DTYP *wp, *qp;
    DTYP ssp;
    DTYP wd;
    int sz;

    sz = *size;

    qp = qt;
    wp = w0;
    for (msk = 0, x = 0; x < sz; x++) {
        *qp++ = 0;
        msk |= *wp++;
    }
    if (!msk)
        return 0;

    for (wp = w0, st = 0; st < sz; st++) {
        if (*wp++)
            break;
    }

    bfs = sz - st;
    blen = bfs * BLEN;

    ssp = (DTYP)0;
    msk = BMSK;
    wp = &w0[st];
    wd = *wp;
    seb = (DTYP)1;
    qp = &qt[st];
    qp--;

    while (1) {
    D_LP1:
        c = 0;
    D_LP2:

        if (c)
            *qp |= seb;
        if ((--blen) < 0)
            break;
        seb >>= 1;
        if (!seb) {

            seb = BMSK;
            qp++;
        }
        if (!msk) {

            msk = BMSK;
            wp++;
            wd = *wp;
        }

        c = ((wd & msk) ? 1 : 0);
        msk >>= 1;

        ssp <<= 1;
        ssp |= c;

        if (w1 > ssp)
            goto D_LP1;

        ssp = ssp - w1;

        c = 1;
        goto D_LP2;
    }

    qp = &qt[st];
    wp = w0;
    for (x = 0; x < bfs; x++)
        *wp++ = *qp++;
    *size = bfs;
    *sp = ssp;
    return 1;
}
