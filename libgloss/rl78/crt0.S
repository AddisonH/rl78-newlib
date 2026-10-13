/* Copyright (c) 2011 Red Hat Incorporated.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met: 

     Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

     The name of Red Hat Incorporated may not be used to endorse 
     or promote products derived from this software without specific 
     prior written permission.

   This software is provided by the copyright holders and contributors
   "AS IS" and any express or implied warranties, including, but not
   limited to, the implied warranties of merchantability and fitness for
   a particular purpose are disclaimed.  In no event shall Red Hat
   incorporated be liable for any direct, indirect, incidental, special,
   exemplary, or consequential damages (including, but not limited to,
   procurement of substitute goods or services; loss of use, data, or
   profits; or business interruption) however caused and on any theory of
   liability, whether in contract, strict liability, or tort (including
   negligence or otherwise) arising in any way out of the use of this
   software, even if advised of the possibility of such damage.  */

#include "rl78-sys.h"

    .section ".vec","a"
    .short	_start

    .section ".ivec","a"
    .macro	_iv	x
    .weak	\x
    .short	\x
    .endm
#define IV(x) _iv ___vector_##x##_
#define IVx() .short 0

    /* To use a vector, simply define a global function named foo_handler()
       for any IV(foo) listed below (i.e. tm05_handler) */

    .global	_interrupt_vector_table
_interrupt_vector_table:
    IV(4)
    IV(6)
    IV(8)
    IV(a)
    IV(c)
    IV(e)
    IV(10)
    IV(12)

    IV(14)
    IV(16)
    IV(18)
    IV(1a)
    IV(1c)
    IV(1e)
    IV(20)
    IV(22)

    IV(24)
    IV(26)
    IV(28)
    IV(2a)
    IV(2c)
    IV(2e)
    IV(30)
    IV(32)

    IV(34)
    IV(36)
    IV(38)
    IV(3a)
    IV(3c)
    IV(3e)
    IV(40)
    IV(42)

    IV(44)
    IV(46)
    IV(48)
    IV(4a)
    IV(4c)
    IV(4e)
    IV(50)
    IV(52)

    IV(54)
    IV(56)
    IV(58)
    IV(5a)
    IV(5c)
    IV(5e)
    IV(60)
    IV(62)

    IV(64)
    IV(66)
    IV(68)
    IV(6a)
    IV(6c)
    IV(6e)
    IVx()
    IVx()

    IVx()
    IVx()
    IVx()
    IVx()
    IVx()
    IV(7e)
    /* Note: 126 vectors */


    .weak	__rl78_option_byte_0
    .weak	__rl78_option_byte_1
    .weak	__rl78_option_byte_2
    .weak	__rl78_option_byte_3
__rl78_option_byte_0 = 0x6e
__rl78_option_byte_1 = 0xff
__rl78_option_byte_2 = 0xe8
__rl78_option_byte_3 = 0x85

    .section ".option_byte", "a"
    .byte	__rl78_option_byte_0
    .byte	__rl78_option_byte_1
    .byte	__rl78_option_byte_2
    .byte	__rl78_option_byte_3

    .text

    .global _start
    .type	_start, @function
_start:
    movw	sp, #__stack


;; block move to initialize .data
#if defined(__OPTIMIZE_SIZE__) || defined(__FAR_DATA__)
    movw ax, #LOWW(__romdatacopysize)
    push ax
    movw bc, #LOWW(__romdatastart)
    movw de, #LOWW(__datastart)
    mov x, #LOW(HIGHW(__romdatastart))
    mov a, #LOW(HIGHW(__datastart))
    call !!__COM_memcpy_ff
    pop ax
#else
    mov     es, #LOW(HIGHW(__romdatastart))
    movw	bc, #LOWW(__romdatacopysize)
    clrw	ax
    cmpw	ax, bc
    bz	$2f
1:
    decw	bc
    decw	bc
    movw	ax, es:LOWW(__romdatastart)[bc]
    movw	LOWW(__datastart)[bc], ax
    clrw	ax
    cmpw    ax, bc
    bnz      $1b
2:
#endif

;; block move to initialize .dataf
#if defined(__OPTIMIZE_SIZE__) || defined(__FAR_DATA__)
    movw ax, #LOWW(__romdatafcopysize)
    push ax
    movw bc, #LOWW(__romdatafstart)
    movw de, #LOWW(__datafstart)
    mov x, #LOW(HIGHW(__romdatafstart))
    mov a, #LOW(HIGHW(__datafstart))
    call !!__COM_memcpy_ff
    pop ax
#else
    mov     es, #LOW(HIGHW(__romdatafstart))
    movw	bc, #LOWW(__romdatafcopysize)
    clrw	ax
    cmpw	ax, bc
    bz	$2f
1:
    decw	bc
    decw	bc
    movw	ax, es:LOWW(__romdatafstart)[bc]
    movw	LOWW(__datafstart)[bc], ax
    clrw	ax
    cmpw    ax, bc
    bnz      $1b
2:
#endif

;; block move to initialize .sdata
#if defined(__OPTIMIZE_SIZE__) || defined(__FAR_DATA__)
    movw ax, #LOWW(__romsdatacopysize)
    push ax
    movw bc, #LOWW(__romsdatastart)
    movw de, #LOWW(__sdatastart)
    mov x, #LOW(HIGHW(__romsdatastart))
    mov a, #LOW(HIGHW(__sdatastart))
    call !!__COM_memcpy_ff
    pop ax
#else
    mov     es, #LOW(HIGHW(__romsdatastart))
    movw	bc, #LOWW(__romsdatacopysize)
    clrw	ax
    cmpw	ax, bc
    bz	$2f
1:
    decw	bc
    decw	bc
    movw	ax, es:LOWW(__romsdatastart)[bc]
    movw	LOWW(__sdatastart)[bc], ax
    clrw	ax
    cmpw    ax, bc
    bnz      $1b
2:
#endif

;; block fill to .sbss
#ifdef __OPTIMIZE_SIZE__
#ifdef __FAR_DATA__
    movw ax, #LOWW(__sbsssize)
    push ax
    movw de, #LOWW(__sbssstart)
    mov a, #LOW(HIGHW(__sbssstart))
    clrw bc
    call !!__COM_memset_f
    pop ax
#else
    movw de, #LOWW(__sbsssize)
    movw ax, #LOWW(__sbssstart)
    clrw bc
    call !!_memset
#endif
#else
    movw    bc, #LOWW(__sbsssize)
    clrw    ax
    cmpw    ax, bc
    bz      $_sbss_zero_done
1:
    decw    bc
    decw    bc
    movw    LOWW(__sbssstart)[bc], ax
    cmpw    ax, bc
    bnz      $1b
_sbss_zero_done:
#endif

;; block fill to .bss
#ifdef __OPTIMIZE_SIZE__
#ifdef __FAR_DATA__
    movw ax, #LOWW(__bsssize)
    push ax
    movw de, #LOWW(__bssstart)
    mov a, #LOW(HIGHW(__bssstart))
    clrw bc
    call !!__COM_memset_f
    pop ax
#else
    movw de, #LOWW(__bsssize)
    movw ax, #LOWW(__bssstart)
    clrw bc
    call !!_memset
#endif
#else
    movw    bc, #LOWW(__bsssize)
    clrw    ax
    cmpw    ax, bc
    bz      $_bss_zero_done
1:
    decw    bc
    decw    bc
    movw    LOWW(__bssstart)[bc], ax
    cmpw    ax, bc
    bnz      $1b
_bss_zero_done:
#endif

;; block fill to .bssf
#ifdef __OPTIMIZE_SIZE__
	movw ax, #LOWW(__bssfsize)
	push ax
	movw de, #LOWW(__bssfstart)
	mov a, #LOW(HIGHW(__bssfstart))
	clrw bc
	call !!__COM_memset_f
	pop ax
#else
    movw    bc, #LOWW(__bssfsize)
    mov     es, #LOW(HIGHW(__bssfstart))
    clrw    ax
    cmpw    ax, bc
    bz      $_bssf_zero_done
1:
    decw    bc
    decw    bc
    movw    es:LOWW(__bssfstart)[bc], ax
    cmpw    ax, bc
    bnz      $1b
_bssf_zero_done:
#endif

	movw    ax, #__stack
    movw    bc, #__end
    subw    ax, bc 
    movw    bc, ax

    movw    ax, #0
    cmpw    ax, bc
    bz  $_heap_zero_done
1:
    decw    bc
    decw    bc
    movw    __end[bc], ax
    cmpw    ax, bc
    bnz $1b

_heap_zero_done:

	call	!!__rl78_init

;;TODO
#ifdef PROFILE_SUPPORT	/* Defined in gcrt0.S.  */
    movw	ax, # _start
    movw	bc, # _etext
    call	!!__monstartup
#endif

    clrw	ax	/* argv */
    clrw	bc	/* argc */
    call	!!_main

.LFE2:

#ifdef PROFILE_SUPPORT
    call	!!__mcleanup
#endif

    call	!!_exit

    .size	_start, . - _start

    .global	_rl78_run_init_array
    .type	_rl78_run_init_array,@function
_rl78_run_init_array:
    movw	hl, #__init_array_start
    movw	de, #__init_array_end
#if defined(__RL78_MEDIUM__) || defined(__FAR_DATA__)
    movw	bc, #4
#else
    movw	bc, #2
#endif
    br	$_rl78_run_inilist

    .global	_rl78_run_fini_array
    .type	_rl78_run_fini_array,@function
_rl78_run_fini_array:
    movw	hl, #__fini_array_start
    movw	de, #__fini_array_end
#if defined(__RL78_MEDIUM__) || defined(__FAR_DATA__)
    movw	bc, #4
#else
    movw	bc, #2
#endif
    /* fall through */

    ;; HL = start of list
    ;; DE = end of list
    ;; BC = step direction (+2 or -2)
_rl78_run_inilist:
next_inilist:
    movw	ax, hl
    cmpw	ax, de
    bz	$done_inilist
    movw	ax, [hl]
    cmpw	ax, #-1
    bz	$skip_inilist
    cmpw	ax, #0
    bz	$skip_inilist
    push	ax
    push	bc
    push	de
    push	hl
#if defined(__RL78_MEDIUM__) || defined(__FAR_DATA__)
    push    ax
    mov		a, [hl+2]
    mov     cs, a
    pop     ax
#endif
    call	ax
    pop	hl
    pop	de
    pop	bc
    pop	ax
skip_inilist:
    movw	ax, hl
    addw	ax, bc
    movw	hl, ax
    br	$next_inilist
done_inilist:
    ret

    .section	.init,"ax"

    .global __rl78_init
__rl78_init:

    .section	.fini,"ax"

    .global __rl78_fini
__rl78_fini:
    call	!!_rl78_run_fini_array

    .section .data
    .global ___dso_handle
    .weak   ___dso_handle
___dso_handle:
    .long	0

;;; Provide Dwarf unwinding information that will help GDB stop
;;; backtraces at the right place.  This is stolen from assembly
;;; code generated by GCC with -dA.
    .section	.debug_frame,"",@progbits
.Lframe0:
    .4byte	.LECIE0-.LSCIE0	; Length of Common Information Entry
.LSCIE0:
    .4byte	0xffffffff	; CIE Identifier Tag
    .byte	0x1	; CIE Version
    .ascii "\0"	; CIE Augmentation
    .uleb128 0x1	; CIE Code Alignment Factor
    .sleb128 -1	; CIE Data Alignment Factor
    .byte	0xd	; CIE RA Column
    .byte	0xc	; DW_CFA_def_cfa
    .uleb128 0xc
    .uleb128 0x3
    .byte	0x8d	; DW_CFA_offset, column 0xd
    .uleb128 0x3
    .p2align 2
.LECIE0:
.LSFDE0:
    .4byte	.LEFDE0-.LASFDE0	; FDE Length
.LASFDE0:
    .4byte	.Lframe0	; FDE CIE offset
    .4byte	_start		; FDE initial location
    .4byte	.LFE2 - _start	; FDE address range
    .byte   0xf     	; DW_CFA_def_cfa_expression
    .uleb128 1		; length of expression
    .byte 	0x30		; DW_OP_lit0
    .p2align 2
.LEFDE0:

    .text
