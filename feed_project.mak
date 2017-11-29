# See LICENSE for license details

#
# Module: feed_project.mak
#
# Description:
#   Makefile definitions for feed project.
#
# Remarks:
#   Definitions must be independant of current folder, all folders
#   must be relative to FEED_SRC_PATH and FEED_OBJ_PATH.
#

ifndef DBG
DBG = 1
endif

ifndef FEED_SRC_PATH
FEED_SRC_PATH = .
endif

ifndef FEED_DST_PATH
FEED_DST_PATH = .
endif

ifndef FEED_CC
FEED_CC = $(CC)
endif

ifndef FEED_CXX
FEED_CXX = $(CXX)
endif

ifndef FEED_AR
FEED_AR = $(AR)
endif

ifeq ($(DBG),1)
FEED_CFG_DBG = chk
else
FEED_CFG_DBG = fre
endif

FEED_CFLAGS_base = -fPIC

FEED_CFLAGS_chk = -g -O0 $(FEED_CFLAGS_base)

FEED_CFLAGS_fre = -O2 -Os $(FEED_CFLAGS_base)

FEED_CFLAGS_WARNINGS = \
    -pedantic -Wall -Wextra -Wabi -Waggregate-return -Warray-bounds \
    -Wattributes -Wbad-function-cast -Wbuiltin-macro-redefined -Wc++-compat \
    -Wcast-align -Wcast-qual -Wconversion -Wdeclaration-after-statement \
    -Wdeprecated -Wdiv-by-zero -Wendif-labels -Wfloat-equal \
    -Wformat-contains-nul -Wformat-extra-args -Wformat-nonliteral \
    -Wformat-security -Wformat-y2k -Wformat-zero-length -Wint-to-pointer-cast \
    -Wlarger-than=4096 -Wlong-long -Wmissing-declarations \
    -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-prototypes \
    -Wmultichar -Wnested-externs -Wold-style-definition -Woverflow \
    -Woverlength-strings -Wpacked -Wpacked-bitfield-compat -Wpadded \
    -Wpointer-arith -Wpointer-to-int-cast -Wpragmas -Wredundant-decls \
    -Wsequence-point -Wshadow -Wstrict-overflow=5 -Wstrict-prototypes \
    -Wsync-nand -Wundef -Wunused -Wunused-macros -Wunused-result \
    -Wvariadic-macros -Wvla -Wwrite-strings

FEED_CXXFLAGS_chk = $(FEED_CFLAGS_chk)

FEED_CXXFLAGS_fre = $(FEED_CFLAGS_fre)

FEED_CXXFLAGS_WARNINGS = \
    -pedantic -Wall -Wextra -Wabi -Waggregate-return -Warray-bounds \
    -Wattributes -Wbuiltin-macro-redefined -Wc++0x-compat \
    -Wcast-align -Wcast-qual -Wconversion \
    -Wdeprecated -Wdiv-by-zero -Wendif-labels -Wfloat-equal \
    -Wformat-contains-nul -Wformat-extra-args -Wformat-nonliteral \
    -Wformat-security -Wformat-y2k \
    -Wlarger-than=4096 -Wlong-long -Wmissing-declarations \
    -Wmissing-format-attribute -Wmissing-include-dirs \
    -Wmultichar -Woverflow \
    -Woverlength-strings -Wpacked -Wpacked-bitfield-compat -Wpadded \
    -Wpointer-arith -Wpragmas -Wredundant-decls \
    -Wsequence-point -Wshadow -Wstrict-overflow=5 \
    -Wsync-nand -Wundef -Wunused -Wunused-macros -Wunused-result \
    -Wvariadic-macros -Wvla -Wwrite-strings \
    -Wctor-dtor-privacy -Weffc++ -Wenum-compare -Wnon-virtual-dtor \
    -Woverloaded-virtual -Wstrict-null-sentinel -Wsign-promo

FEED_INCLUDES = -I$(FEED_DST_PATH)

FEED_CFLAGS = $(CFLAGS) $(FEED_CFLAGS_$(FEED_CFG_DBG)) $(FEED_CFLAGS_WARNINGS) $(FEED_INCLUDES)

FEED_CXXFLAGS = $(CXXFLAGS) $(FEED_CXXFLAGS_$(FEED_CFG_DBG)) $(FEED_CXXFLAGS_WARNINGS) $(FEED_INCLUDES)

FEED_LDFLAGS_chk =

FEED_LDFLAGS_fre = -s

FEED_LDFLAGS = $(LDFLAGS) $(FEED_LDFLAGS_$(FEED_CFG_DBG))

FEED_LIBS =

FEED_LIBRARY_SRCS = \
    $(FEED_DST_PATH)/_obj_feed_client.o \
    $(FEED_DST_PATH)/_obj_feed_heap.o \
    $(FEED_DST_PATH)/_obj_feed_tty.o \
    $(FEED_DST_PATH)/_obj_feed_buf.o \
    $(FEED_DST_PATH)/_obj_feed_keys.o \
    $(FEED_DST_PATH)/_obj_feed_input.o \
    $(FEED_DST_PATH)/_obj_feed_list.o \
    $(FEED_DST_PATH)/_obj_feed_esc.o \
    $(FEED_DST_PATH)/_obj_feed_glyph.o \
    $(FEED_DST_PATH)/_obj_feed_line.o \
    $(FEED_DST_PATH)/_obj_feed_text.o \
    $(FEED_DST_PATH)/_obj_feed_screen.o \
    $(FEED_DST_PATH)/_obj_feed_screen_info.o \
    $(FEED_DST_PATH)/_obj_feed_screen_iterator.o \
    $(FEED_DST_PATH)/_obj_feed_prompt.o \
    $(FEED_DST_PATH)/_obj_feed_prompt_iterator.o \
    $(FEED_DST_PATH)/_obj_feed_suggest.o \
    $(FEED_DST_PATH)/_obj_feed_object.o \
    $(FEED_DST_PATH)/_obj_feed_utf8.o \
    $(FEED_DST_PATH)/_obj_feed_text_iterator.o \
    $(FEED_DST_PATH)/_obj_feed_view.o \
    $(FEED_DST_PATH)/_obj_feed.o

FEED_TEST_SRCS = \
    $(FEED_DST_PATH)/_obj_feed_os.o \
    $(FEED_DST_PATH)/_obj_feed_main.o \
    $(FEED_DST_PATH)/libfeed.so

# Default target
.PHONY: all
all : testfeed libfeed

# Test application
.PHONY: testfeed
testfeed: $(FEED_DST_PATH)/testfeed.exe

# Library
.PHONY: libfeed
libfeed: $(FEED_DST_PATH)/libfeed.a $(FEED_DST_PATH)/libfeed.so

# Link the target
$(FEED_DST_PATH)/testfeed.exe : $(FEED_TEST_SRCS)
	@echo linking $@
	@echo -o $@ $(FEED_CFLAGS) $(FEED_TEST_SRCS) $(FEED_LDFLAGS) $(FEED_LIBS) > $(FEED_DST_PATH)/_obj_testfeed.cmd
	@$(FEED_CC) @$(FEED_DST_PATH)/_obj_testfeed.cmd

$(FEED_DST_PATH)/libfeed.a : $(FEED_LIBRARY_SRCS)
	@echo creating $@
	@$(FEED_AR) rc $(FEED_DST_PATH)/libfeed.a $(FEED_LIBRARY_SRCS)

$(FEED_DST_PATH)/libfeed.so : $(FEED_LIBRARY_SRCS) $(FEED_SRC_PATH)/feed.exports
	@echo linking $@
	@echo -shared -Wl,--version-script,$(FEED_SRC_PATH)/feed.exports -o $@ $(FEED_CFLAGS) $(FEED_LIBRARY_SRCS) $(FEED_LDFLAGS) $(FEED_LIBS) > $(FEED_DST_PATH)/_obj_libfeed_so.cmd
	@$(FEED_CC) @$(FEED_DST_PATH)/_obj_libfeed_so.cmd

# Build each object file
$(FEED_DST_PATH)/_obj_%.o : $(FEED_SRC_PATH)/%.c
	@echo compiling $@
	@echo -c -o $@ $(FEED_CFLAGS) -MT $@ -MMD -MP -MF $@.d $< > $@.cmd
	@$(FEED_CXX) -c -x c++ -o $@.oxx $(FEED_CXXFLAGS) $<
	@$(FEED_CC) @$@.cmd

# Build the precompiled header
$(FEED_DST_PATH)/feed_os.h.gch : $(FEED_SRC_PATH)/feed_os.h
	@echo generating $@
	@$(FEED_CXX) -c -o $@.oxx $(FEED_CXXFLAGS) $(FEED_SRC_PATH)/feed_os.h
	@$(FEED_CC) -c -o $@ $(FEED_CFLAGS) $(FEED_SRC_PATH)/feed_os.h

# Indicate that all object files have dependency on precompiled header
$(FEED_TEST_SRCS) : $(FEED_DST_PATH)/feed_os.h.gch

# Indicate dependency on makefile
$(FEED_DST_PATH)/testfeed.exe : $(FEED_SRC_PATH)/feed_project.mak

$(FEED_DST_PATH)/libfeed.a : $(FEED_SRC_PATH)/feed_project.mak

# Indicate dependency on makefile
$(FEED_TEST_SRCS): $(FEED_SRC_PATH)/feed_project.mak

$(FEED_LIBRARY_SRCS) : $(FEED_SRC_PATH)/feed_project.mak

# Clean temporary files
.PHONY: clean
clean :
	-rm -f $(FEED_DST_PATH)/_obj_*
	-rm -f $(FEED_DST_PATH)/feed_os.h.gch
	-rm -f $(FEED_DST_PATH)/feed_os.h.gch.oxx
	-rm -f $(FEED_DST_PATH)/testfeed.exe
	-rm -f $(FEED_DST_PATH)/libfeed.a
	-rm -f $(FEED_DST_PATH)/libfeed.so

# Include header dependency files
-include $(FEED_DST_PATH)/_obj_*.o.d
