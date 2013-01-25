
# define SYSROOT, SRC_ROOT need defined before this
SYSROOT := $(shell (cd $(SRC_ROOT)/..; pwd))/sysroot

# define output directory
LIBDIR := $(shell (cd $(SRC_ROOT); pwd))/lib
BINDIR := $(shell (cd $(SRC_ROOT)/..; pwd))/output/bin

# define compiler
CC := arm-none-linux-gnueabi-gcc --sysroot=$(SYSROOT)

CPPFLAGS := $(CPPFLAGS) -I$(shell (cd $(SRC_ROOT); pwd))/include

CFLAGS := -Wall -O3 -Os -pipe \
		 -mtune=cortex-a8 -march=armv7-a -mabi=aapcs-linux -msoft-float \
		 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
		 -D_GNU_SOURCE -D_REENTRANT \
		 $(CFLAGS)

# uses '=' let the variable parse when it's used
# COMPILE: compiles the source file specified
# COMPILEX: compiles the source file
# COMPILE_MSG: print compiling message
COMPILE = @$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		  $(CPPFLAGS) $(CFLAGS)

COMPILEX = @$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		  $(CPPFLAGS) $(CFLAGS) -c $< -o $@

COMPILE_MSG = @echo "  CC\t$<"

LDFLAGS := -L$(SYSROOT)/lib -L$(SYSROOT)/usr/lib -L$(LIBDIR) \
	$(LDFLAGS)

# uses '=' let the variable parse when it's used
# LINK: link the objects specified
# LINKX: compiles the objects
# LINK_MSG: print linking message
LINK = @$(CC) $(CFLAGS) $(LDFLAGS)
 
LINKX = @$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LDFLAGS)

LINK_MSG = @echo "  LN\t$^ -> $@"
 
AR = @arm-none-linux-gnueabi-ar

ARFLAGS := -rc

