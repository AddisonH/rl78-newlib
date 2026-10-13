%C%_src = \
    %D%/printf.c \
    %D%/printf_tiny.c \
    %D%/scanf.c \
    %D%/sscanf.c \
    %D%/vprintf.c \
    %D%/vsprintf.c \
    %D%/sprintf.c \
    %D%/sprintf_tiny.c \
    %D%/snprintf.c \
    %D%/vscanf.c \
    %D%/vsnprintf.c \
    %D%/vsscanf.c \
    %D%/_REL_print_tiny.c \
    %D%/_REL_print.c \
    %D%/_REL_scan.c

librenesas_a_SOURCES += $(%C%_src)
librenesas_a_CFLAGS += -frenesas-extensions
librenesas_a_CCASFLAGS += -frenesas-extensions
librenesas_a_CPPFLAGS += -frenesas-extensions
