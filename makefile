#
# makefile for script.
#
# Copyright (C) 2024 Stefan Kleinschmidt (littlesmith23)
#


# If you build release binary, set y.
RELEASE = y
TARGET           = cscript
C_SRCS         = cscript.c cache.c script_file.c sha256.c tools.c

ifeq ($(RELEASE),y)
CFLAGS          ?= -Wall -O2
else
CFLAGS          ?= -Wall -O0 -g -DDEBUG=1
endif

EXTRA_CXXFLAGS   =
EXTRA_LDFLAGS    =

# set cross compiler
LD               = $(CROSS)ld
GCC              = $(CROSS)gcc
CC               = $(GCC)
AR               = $(CROSS)ar
STRIP            = $(CROSS)strip
INSTALL         ?= install
RM              ?= rm -f

C_OBJS           = $(patsubst %.c,%.o,$(C_SRCS))

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

.PHONY: all clean install uninstall

all : $(TARGET)

$(TARGET): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(CXX_OBJS) $(C_OBJS) $(STATIC_LIB) $(EXTRA_LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I -c $< -o $@

clean:
	$(RM) *.o $(TARGET) *~

install:
	install $TARGET $(DESTDIR)($PREFIX)/bin/
	install -d /usr/share/doc/cscript
	install -m 644 README.md /usr/share/doc/cscript

uninstall:
	rm $(DESTDIR)($PREFIX)/bin/$TARGET
	rm -R /usr/share/doc/cscript
# end of file.
