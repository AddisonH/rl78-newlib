#ifndef __REL_FLOAT_H
#define __REL_FLOAT_H

typedef signed char s_char;
typedef signed short s_short;
typedef signed int s_int;
typedef signed long s_long;
#if !defined(__CA78K0R__)
typedef signed long long s_llong;
#endif

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
#if !defined(__CA78K0R__)
typedef unsigned long long u_llong;
#endif

#define SGL_MBSZ	5
#define SGL_CLS		1
#define	SGL_INFEXP	0xff
#define	SGL_INFMANT	0L
#define	SGL_NANEXP	0xff
#define	SGL_NANMANT	0x7fffffL
#define	SGL_UFLLMT	24
#define SGL_UFLEXP	1
#define SGL_UFLMANT	0L
#define SGL_OFLEXP	0xfe
#define SGL_OFLMANT	SGL_NANMANT
#define SGL_MANT_MSK	0x7fffffL
#define SGL_EXP_MSK	0xff
#define SGL_MANT_MSB	0x400000L

#if defined(__DBL8)

#define DBL_MBSZ	9
#define DBL_CLS		2
#define	DBL_INFEXP	0x7ff
#define	DBL_INFMANT	0L
#define	DBL_NANEXP	0x7ff
#define	DBL_NANMANT	0xfffffL
#define	DBL_NANMANT2	0xffffffffL
#define	DBL_UFLLMT	53
#define DBL_UFLEXP	1
#define DBL_UFLMANT	0L
#define DBL_OFLEXP	0x7fe
#define DBL_OFLMANT	DBL_NANMANT
#define DBL_OFLMANT2	DBL_NANMANT2
#define DBL_MANT_MSK	0xfffffL
#define DBL_MANT2_MSK	0xffffffffL
#define DBL_EXP_MSK	0x7ff
#define DBL_MANT_MSB	0x80000L
#else
#define DBL_MBSZ	SGL_MBSZ
#define DBL_CLS		SGL_CLS
#define	DBL_INFEXP	SGL_INFEXP
#define	DBL_INFMANT	SGL_INFMANT
#define	DBL_NANEXP	SGL_NANEXP
#define	DBL_NANMANT	SGL_NANMANT
#define	DBL_NANMANT2	0xffffffffL
#define	DBL_UFLLMT	SGL_UFLLMT
#define DBL_UFLEXP	SGL_UFLEXP
#define DBL_UFLMANT	SGL_UFLMANT
#define DBL_OFLEXP	SGL_OFLEXP
#define DBL_OFLMANT	SGL_OFLMANT
#define DBL_OFLMANT2	0xffffffffL
#define DBL_MANT_MSK	SGL_MANT_MSK
#define DBL_MANT2_MSK	0xffffffffL
#define DBL_EXP_MSK	SGL_EXP_MSK
#define DBL_MANT_MSB	SGL_MANT_MSB
#endif

#define	OPE_ADD		1
#define	OPE_SUB		2
#define	OPE_MUL		3
#define	OPE_DIV		4

typedef union {
    unsigned long ml;
    u_short ms[2];
    u_char mc[4];
} MANT;

typedef struct {
    char _class;
    char sign;
    signed short exp;
    MANT mant;
    MANT mant2;
} FLOAT;

typedef FLOAT __near * FLOATP;

#endif
