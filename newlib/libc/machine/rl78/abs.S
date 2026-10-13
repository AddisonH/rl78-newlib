	.file	"abs.S"

	.section .text
    
	.global  _abs
	.type	 _abs,@function

_abs:
    bf      a.7, $1f
    movw    bc, ax
    clrw    ax
    subw    ax, bc
1:  
    ret

    .size _abs, . - _abs
