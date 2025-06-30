MEMORY
{
	VEC : ORIGIN = 0x0, LENGTH = 4
	IVEC : ORIGIN = 0x4, LENGTH = 0x7c
	CALLT0 : ORIGIN = 0x80, LENGTH = 0x40
	OPT : ORIGIN = 0xC0, LENGTH = 4
	SEC_ID : ORIGIN = 0xC4, LENGTH = 10
	ROM : ORIGIN = 0xD8, LENGTH = 0x7FD28
	MIRROR : ORIGIN = 0xF3000, LENGTH = 0x4F00
	SADDR (w) : ORIGIN = 0xffe20, LENGTH = 0x000a0
	RAM : ORIGIN = 0xF7F00, LENGTH = 32768
}

SIM__family = 0x0; /* -string value converted to hex - e.g. 10Y is 0x00313059 */
SIM__romsize = 0x7FD28;
SIM__ramstart = 0xF7F00;
SIM__mirrorromstart = 0x03000;
SIM__mirrorramstart = 0xF3000;
SIM__mirrorsize = 0x4F00;

SECTIONS
{
	/*Constant 64kb */
	_64k_size = 0x10000;

	.vec 0x0 : AT(0x0)
	{
		KEEP(*(.vec))
	} >VEC

	.ivec 0x4 : AT(0x4)
	{
		KEEP(*(.ivec))
	} >IVEC

	.callt0 0x80 : AT(0x80) ALIGN(2)
	{
		. = ALIGN(2);
		KEEP(*(.callt0))
	} >CALLT0

	.option_byte 0xC0 : AT(0xC0)
	{
		KEEP(*(.option_byte))
	} >OPT

	.security_id 0xC4 : AT(0xC4)
	{
		KEEP(*(.security_id))
	} >SEC_ID

	.lowtext 0xD8 : AT(0xD8)
	{
		*(.plt)
		*(.lowtext)
		. = ALIGN(2);
	} >ROM

	.tors :
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		KEEP(*(.ctors))
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		KEEP(*(.dtors))
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
	} >ROM


      __mdata = .;

	.init (. +__romdatacopysize):
	{
		KEEP(*(.init))
	} >ROM
	.fini :
	{
		KEEP(*(.fini))
	} >ROM

	PROVIDE(__rodata_limit = CONSTANT(MIRRORAREASTART)+0x3000 + LENGTH(MIRROR));

	/* The rodata section is placed in MIRROR area in order to access as near addressing. */
	.rodata ALIGN(MAX(., (CONSTANT(MIRRORAREASTART)+0x3000)), 2):
	{
		. = ALIGN(2);
		__rodata = .;
		*(.rodata)
		*(.rodata.*)

		. = ALIGN(2);
		*(.const)
		*(.const.*)

		. = ALIGN(2);
		PROVIDE(__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE(__init_array_end = .);
		PROVIDE(__fini_array_start = .);
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		PROVIDE(__fini_array_end = .);
		SHORT(0);
		__erodata = .;
		/*The rodata section is copied into the MIRROR area, that is why we check if the start and end addresses for rodata are less then  _rodata_limit*/
		ASSERT((SIZEOF (.rodata) == 0) || (__rodata < __rodata_limit), "Error: rodata section start address is too large. Move the text section after the rodata section to ensure that correct data is added to the MIRROR area.");
		ASSERT((SIZEOF (.rodata) == 0) || (__erodata <= __rodata_limit), "Error: rodata section size exceeds length of the MIRROR area.");

	}>ROM

	.text :
	{
		. = ALIGN(2);
		*(.text)
		*(.text.*)
		__etext = .;
		ASSERT((__etext < _64k_size), "Error: text - maximum end address for text is 0x0FFFF");
		/*INPUT_SECTION_FLAGS(SHF_EXECINSTR) *(*_n)*/

		*(.gnu.warning*)
		. = ALIGN(2);

	} >ROM AT>ROM

	.textf :
	{   	. = ALIGN(2);
		*(.textf)
		*(.textf.*)
		/*INPUT_SECTION_FLAGS(SHF_EXECINSTR) *(*_f)*/
	}>ROM

	.eh_frame_hdr :
	{
		PROVIDE(___eh_frame_hdr_start = .);
		*(.eh_frame_hdr)
		PROVIDE(___eh_frame_hdr_end = .);
	} >ROM

	.eh_frame :
	{
		PROVIDE(___eh_frame_start = .);
		*(.eh_frame)
		PROVIDE(___eh_frame_end = .);
	} >ROM

	.data 0xF7F00 : AT(__mdata)
	{
		. = ALIGN(2);
		PROVIDE (__datastart = .);
		__data = .;
		*(.data)
		*(.data.*)
		/*INPUT_SECTION_FLAGS(!SHF_EXECINSTR & SHF_WRITE & SHF_ALLOC) *(*_n)*/
		. = ALIGN(2);
		__edata = .;
		ASSERT((__edata/_64k_size == __data/_64k_size), "Error: data - crosses boundary of 64 Kbytes")

	} >RAM
	/* Note that crt0 assumes this is a multiple of two; all the
	start/stop symbols are also assumed word-aligned.  */

	PROVIDE(__romdatastart = LOADADDR(.data));
	PROVIDE (__romdatacopysize = SIZEOF(.data));


	.dataf : ALIGN(2)
	{
		. = ALIGN(2);
		PROVIDE (__datafstart = .);
		. = ALIGN(2);
		*(.dataf)
		*(.dataf.*)
		/*INPUT_SECTION_FLAGS(!SHF_EXECINSTR & SHF_WRITE & SHF_ALLOC) *(*_f)*/
		. = ALIGN(2);
		PROVIDE (__datafend = .);
		ASSERT((__datafend/_64k_size == __datafstart/_64k_size), "Error: dataf - crosses boundary of 64 Kbytes")
	} > RAM AT> ROM
	PROVIDE(__romdatafstart = LOADADDR(.dataf));
	PROVIDE (__romdatafcopysize = SIZEOF(.dataf));


	.sdata : ALIGN(2)
	{
		. = ALIGN(2);
		PROVIDE (__sdatastart = .);

		/* We want the small data sections together, so single-instruction offsets
		   can access them all, and initialized data all before uninitialized, so
		   we can shorten the on-disk segment size.  */

		*(.sdata)
		*(.sdata.*)
		PROVIDE (__sdataend = .);
	} > RAM AT> ROM

	 /* Note that crt0 assumes this is a multiple of two; all the
	 start/stop symbols are also assumed word-aligned.  */
	 PROVIDE(__romsdatastart = LOADADDR(.sdata));
	 PROVIDE (__romsdatacopysize = SIZEOF(.sdata));


	.frodata : ALIGN(65536) /* this 64KB alignment is needed to prevent string literals cross a segment boundary */
	{
		. = ALIGN(2);
		__frodata = .;
		*(.frodata)
		*(.frodata.*)
		__efrodata = .;

		. = ALIGN(2);
		__constf = .;
		*(.constf)
		*(.constf.*)
		__econstf = .;
		ASSERT((__econstf/_64k_size == __constf/_64k_size), "Error: constf - crosses boundary of 64 Kbytes")

		/* NOTE: we can't restrict the size of .frodata to a single segment as e.g. the charconv floating-point functions */
		/*       already use more than that for their lookup tables => all the related tests will fail b/c of this ASSERT */
		/*ASSERT((SIZEOF (.frodata) <= 65536), "Error: frodata section is too large (>64KB).");*/

	} >ROM

	.sbss (NOLOAD) : ALIGN(2)
	{
		PROVIDE(__sbssstart = .);
		. = ALIGN(2);
		__sbss = .;
		*(.sbss)
		*(.sbss.*)
		/*INPUT_SECTION_FLAGS(!SHF_EXECINSTR & SHF_WRITE & SHF_ALLOC) *(*_s)*/
		. = ALIGN(2);
		*(.sbss_bit)
		. = ALIGN(2);
		__esbss = .;

	} >RAM AT>RAM
	PROVIDE(__sbsssize = SIZEOF(.sbss));

	.bssf (NOLOAD) : ALIGN(2)
	{
		PROVIDE(__bssfstart = .);
		. = ALIGN(2);
		*(.bssf)
		*(.bssf.*)
		/*INPUT_SECTION_FLAGS(!SHF_EXECINSTR & SHF_WRITE & SHF_ALLOC) *(*_f)*/
		. = ALIGN(2);
		ASSERT((__end/_64k_size == __bssfstart/_64k_size), "Error: bssf - crosses boundary of 64 Kbytes")
	} >RAM AT>RAM
	PROVIDE(__bssfsize = SIZEOF(.bssf));

	.bss :
	{
		PROVIDE(__bssstart = .);
		. = ALIGN(2);
		__bss = .;
		*(.bss)
		*(.bss.*)
		. = ALIGN(2);
		*(.bss_bit)
		. = ALIGN(2);
		*(COMMON)
		. = ALIGN(2);
		__ebss = .;
		. = ALIGN(128);
		__end = .;
		ASSERT((__ebss/_64k_size == __bss/_64k_size), "Error: bss - crosses boundary of 64 Kbytes")
	} >RAM AT>RAM
	PROVIDE(__bsssize = SIZEOF(.bss));

	PROVIDE(__stack_size = 0x100);
	.stack 0xFFE20 (NOLOAD) : AT(0xFFE20)
	{

		PROVIDE(__stack = .);
		ASSERT((__stack > (__end + __stack_size)), "Error: Too much data - no room left for the stack");
	} >RAM

	/DISCARD/ :
	{
		*(.note)
		*(.note.*)
		*(.gnu.warning*)
	}
	.modelinfo  (INFO) : {
   		*(.modelinfo)
    	LONG (SIM__family);
    	LONG (SIM__romsize);
    	LONG (SIM__ramstart);
    	LONG (SIM__mirrorromstart);
    	LONG (SIM__mirrorramstart);
    	LONG (SIM__mirrorsize);
   }
}
