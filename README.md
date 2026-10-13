# newlib for the RL78

[newlib](https://sourceware.org/newlib/) 4.6.0 with the Renesas RL78 port, for
use with the RL78 LLVM toolchain at <https://github.com/AddisonH/rl78-llvm>.
It provides the C library (`libc.a`), the maths library (`libm.a`) and,
through libgloss, the startup code, linker scripts and simulator system calls.

The generic `README` next to this file is newlib's own.

## Branches

| Ref | Content |
|---|---|
| `rl78` | newlib 4.6.0 + the RL78 port + the changes listed below (default branch) |
| `vendor/renesas-4.3` | newlib 4.3.0 + one commit per public Renesas release |
| `renesas/17.0.1.YYYYMM` | tags on `vendor/renesas-4.3`, one per release |

Everything else is upstream newlib history
(`git://sourceware.org/git/newlib-cygwin.git`).

## Where the RL78 port comes from

Upstream newlib has a minimal RL78 port (setjmp and a libgloss directory for
the GNU simulator). Renesas extends it for LLVM: assembly string and memory
routines, far-pointer variants of the string and conversion functions
(`_COM_*_f`), startup code and linker scripts, and fixes for 16-bit `int` in
generic code. Those changes are published as `newlib-4.3.0.tar.gz` inside the
source archives of "LLVM for Renesas RL78" at
<https://llvm-gcc-renesas.com/rl78/rl78-latest-source-code/>.

`vendor/renesas-4.3` imports each of those trees unchanged on top of the
upstream `newlib-4.3.0` tag (the commit messages carry the archive checksums),
and `rl78` merges that branch onto `newlib-4.6.0`.

The files Renesas added under `newlib/libc/machine/rl78` carry no copyright or
licence notice of their own; they are redistributed here as published,
alongside newlib's licences (`COPYING.NEWLIB`, `COPYING.LIBGLOSS`).

## Changes on `rl78`

- `librenesas` (the printf/scanf family with the CC-RL calling convention) and
  the `__CCRL__` declarations in `stdio.h` are removed.
- `newlib/libm/machine/rl78`: `sqrt` and `sqrtf` by integer arithmetic,
  correctly rounded, about six times faster than the generic bit-by-bit
  routines.
- `newlib/libc/machine/rl78`: `memcpy` and `memset` move words when not
  optimizing for size; a size build keeps the 16- and 14-byte byte loops.

## Building

One configuration per library variant, with the compiler's options for that
variant in the `*_FOR_TARGET` flags:

```sh
B=/path/to/rl78-llvm/bin
FLAGS="-mcpu=s3 -O2 -fomit-frame-pointer -ffunction-sections -fdata-sections \
       -mmirror-source-common -mcommon-rom -fno-exceptions"
export CC_FOR_TARGET=$B/clang AS_FOR_TARGET=$B/clang LD_FOR_TARGET=$B/clang \
       AR_FOR_TARGET=$B/llvm-ar NM_FOR_TARGET=$B/llvm-nm \
       RANLIB_FOR_TARGET=$B/llvm-ranlib READELF_FOR_TARGET=$B/llvm-readelf \
       CFLAGS_FOR_TARGET="$FLAGS" ASFLAGS_FOR_TARGET="$FLAGS" CPPFLAGS_FOR_TARGET="$FLAGS"
mkdir build && cd build
../configure --target=rl78-unknown-elf --disable-nls --disable-multilib \
    --enable-newlib-io-c99-formats --enable-newlib-io-long-long --prefix=$PWD/../install
make MAKEINFO=true -j8 && make MAKEINFO=true install
```

`clang -print-multi-lib` lists the variants the compiler looks for and the
options of each; the result of `make install` (`rl78-unknown-elf/lib` and
`include`) goes to `lib/clang-runtimes/rl78-unknown-elf/<variant>/` of the
toolchain. The small "nano" variant (`libc_nano.a`, `-fnewlib-nano`) is the
same build with newlib's nano options (`--enable-newlib-reent-small
--enable-newlib-nano-malloc --enable-newlib-nano-formatted-io
--disable-newlib-fvwrite-in-streamio --disable-newlib-fseek-optimization
--disable-newlib-wide-orient --disable-newlib-unbuf-stream-opt`).

The generated `configure` and `Makefile.in` files are checked in, as upstream
does; regenerate them with autoconf 2.69 and automake 1.15.1 (`autoreconf` in
`newlib/` and `libgloss/`) after changing a `Makefile.am`, `Makefile.inc` or
`acinclude.m4`.
