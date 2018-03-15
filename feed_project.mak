# See LICENSE for license details

#
# Module: feed_project.mak
#
# Description:
#   Makefile definitions for feed project.
#
# Remarks:
#   Definitions must be independant of current folder, all folders
#   must be relative to FEED_SRC_PATH and FEED_DST_PATH.
#

ifndef DBG
DBG = 1
endif

ifndef PIC
PIC = 1
endif

ifndef FEED_SRC_PATH
FEED_SRC_PATH =
endif

ifndef FEED_DST_PATH
FEED_DST_PATH =
endif

ifndef FEED_CC
FEED_CC = $(CC)
endif

ifndef FEED_CXX
FEED_CXX = $(CXX)
endif

ifndef FEED_OS
FEED_OS = std
endif

ifndef FEED_AR
FEED_AR = $(AR)
endif

ifeq ($(DBG),1)
FEED_CFG_DBG = chk
else
FEED_CFG_DBG = fre
endif

ifeq ($(PIC),1)
FEED_CFG_PIC =
else
FEED_CFG_PIC = _nopic
endif

FEED_OBJ_PATH = $(FEED_DST_PATH).obj/feed/$(FEED_CFG_DBG)$(FEED_CFG_PIC)/

ifeq ($(PIC),1)
FEED_CFLAGS_PIC = -fPIC
else
FEED_CFLAGS_PIC =
endif

ifeq ($(DBG),1)
FEED_CFLAGS_DBG = -g -O0
else
FEED_CFLAGS_DBG = -O2 -Os
endif

ifeq ($(FEED_OS),std)
FEED_CFLAGS_OS = -DFEED_OS_LINUX
else
FEED_CFLAGS_OS = -DFEED_OS_WINDOWS
endif

FEED_CXXFLAGS_PIC = $(FEED_CFLAGS_PIC)

FEED_CXXFLAGS_DBG = $(FEED_CFLAGS_DBG)

FEED_CXXFLAGS_OS = $(FEED_CFLAGS_OS)

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
    -Woverloaded-virtual -Wstrict-null-sentinel -Wsign-promo \
    -fno-rtti -fno-exceptions

FEED_INCLUDES = -I$(FEED_OBJ_PATH)

FEED_CFLAGS = $(CFLAGS) $(FEED_CFLAGS_PIC) $(FEED_CFLAGS_DBG) $(FEED_CFLAGS_OS) $(FEED_CFLAGS_WARNINGS) $(FEED_INCLUDES)

FEED_CXXFLAGS = $(CXXFLAGS) $(FEED_CXXFLAGS_PIC) $(FEED_CXXFLAGS_DBG) $(FEED_CXXFLAGS_OS) $(FEED_CXXFLAGS_WARNINGS) $(FEED_INCLUDES)

ifeq ($(DBG),1)
FEED_LDFLAGS_DBG =
else
FEED_LDFLAGS_DBG = -s
endif

FEED_LDFLAGS = $(LDFLAGS) $(FEED_LDFLAGS_DBG)

FEED_LIBS =

define FEED_compile
FEED_option_$(1)_input ?= $$(FEED_SRC_PATH)$(1).c
FEED_option_$(1)_cflags ?= $$(FEED_CFLAGS)
FEED_option_$(1)_cxxflags ?= $$(FEED_CXXFLAGS)
FEED_option_$(1)_output ?= $$(FEED_OBJ_PATH)$(1).o
FEED_option_$(1)_cmdfile ?= $$(FEED_option_$(1)_output).cmd
FEED_option_$(1)_depfile ?= $$(FEED_option_$(1)_output).d
$$(FEED_option_$(1)_cmdfile) : $$(FEED_SRC_PATH)feed_project.mak | $$(FEED_OBJ_PATH)
	@echo generate $$(FEED_option_$(1)_cmdfile)
	@echo  -c -o $$(FEED_option_$(1)_output) $$(FEED_option_$(1)_cflags) -MT $$(FEED_option_$(1)_output) -MMD -MP -MF $$(FEED_option_$(1)_depfile) $$(FEED_option_$(1)_input) > $$(FEED_option_$(1)_cmdfile)
$$(FEED_option_$(1)_output) : $$(FEED_option_$(1)_input) $$(FEED_SRC_PATH)feed_project.mak | $$(FEED_OBJ_PATH) $$(FEED_option_$(1)_cmdfile)
	@echo verifying $$(FEED_option_$(1)_output).oxx
	@$$(FEED_CXX) -c -x c++ -o $$(FEED_option_$(1)_output).oxx $$(FEED_option_$(1)_cxxflags) $$(FEED_option_$(1)_input)
	@echo compiling $$(FEED_option_$(1)_output)
	@$$(FEED_CC) @$$(FEED_option_$(1)_cmdfile)
endef

FEED_LIBRARY_MODULES = \
    feed_client \
    feed_heap \
    feed_tty \
    feed_buf \
    feed_keys \
    feed_input \
    feed_list \
    feed_esc \
    feed_glyph \
    feed_line \
    feed_text \
    feed_screen \
    feed_screen_info \
    feed_screen_iterator \
    feed_prompt \
    feed_prompt_iterator \
    feed_suggest \
    feed_object \
    feed_utf8 \
    feed_text_iterator \
    feed_view \
    feed_page \
    feed_theme \
    feed_dict \
    feed_device_std \
    feed_device_w32 \
    feed_device \
    feed_syntax \
    feed

FEED_option_precomp_input = $(FEED_SRC_PATH)feed_os.h
FEED_option_precomp_output = $(FEED_OBJ_PATH)feed_os.h.gch

# Default target
.PHONY: all
all : libfeed

# Library
.PHONY: libfeed
libfeed: $(FEED_OBJ_PATH)libfeed.a $(FEED_OBJ_PATH)libfeed.so

$(foreach module,$(FEED_LIBRARY_MODULES), $(eval $(call FEED_compile,$(module))))
$(foreach module,precomp, $(eval $(call FEED_compile,$(module))))

# Create list of inputs for library
FEED_LIBRARY_SRCS = $(foreach module,$(FEED_LIBRARY_MODULES), $(FEED_option_$(module)_output))

# Dependency of each .o file with precompiled header file
$(FEED_LIBRARY_SRCS) : $(FEED_option_precomp_output)

$(FEED_OBJ_PATH) :
	@echo creating $(FEED_OBJ_PATH) folder
	@mkdir -p $(FEED_OBJ_PATH)

$(FEED_OBJ_PATH)libfeed.a : $(FEED_LIBRARY_SRCS) | $(FEED_OBJ_PATH)
	@echo creating $(FEED_OBJ_PATH)libfeed.a
	@$(FEED_AR) rc $(FEED_OBJ_PATH)libfeed.a $(FEED_LIBRARY_SRCS)

$(FEED_OBJ_PATH)libfeed_so.cmd : $(FEED_SRC_PATH)feed_project.mak | $(FEED_OBJ_PATH)
	@echo generating $(FEED_OBJ_PATH)libfeed_so.cmd
	@echo -shared -Wl,--version-script,$(FEED_SRC_PATH)feed.exports -o $(FEED_OBJ_PATH)libfeed.so $(FEED_CFLAGS) $(FEED_LIBRARY_SRCS) $(FEED_LDFLAGS) $(FEED_LIBS) > $(FEED_OBJ_PATH)libfeed_so.cmd

$(FEED_OBJ_PATH)libfeed.so : $(FEED_LIBRARY_SRCS) $(FEED_SRC_PATH)feed.exports | $(FEED_OBJ_PATH) $(FEED_OBJ_PATH)libfeed_so.cmd
	@echo linking $(FEED_OBJ_PATH)libfeed.so
	@$(FEED_CC) @$(FEED_OBJ_PATH)libfeed_so.cmd

# Indicate dependency on makefile
$(FEED_OBJ_PATH)libfeed.a : $(FEED_SRC_PATH)feed_project.mak

$(FEED_OBJ_PATH)libfeed.so : $(FEED_SRC_PATH)feed_project.mak

# Cleanup of temporary files
.PHONY: clean
clean : feed_library_clean

.PHONY: feed_library_clean
feed_library_clean:
	-rm -r -f $(FEED_OBJ_PATH)

# Include header dependency files
-include $(FEED_OBJ_PATH)*.o.d

