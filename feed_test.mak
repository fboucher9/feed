# See LICENSE for license details

#
# Module: feed_test.mak
#
# Description:
#	Build a test application for feed library.
#

FEED_TEST_SRCS = \
    $(FEED_DST_PATH)/_obj_feed_os.o \
    $(FEED_DST_PATH)/_obj_feed_main.o \
    $(FEED_DST_PATH)/libfeed.a

# Default target
.PHONY: all
all : testfeed

# Test application
.PHONY: testfeed
testfeed: $(FEED_DST_PATH)/testfeed.exe

# Link the target
$(FEED_DST_PATH)/testfeed.exe : $(FEED_TEST_SRCS)
	@echo linking $@
	@echo -o $@ $(FEED_CFLAGS) $(FEED_TEST_SRCS) $(FEED_LDFLAGS) $(FEED_LIBS) > $(FEED_DST_PATH)/_obj_testfeed.cmd
	@$(FEED_CC) @$(FEED_DST_PATH)/_obj_testfeed.cmd

# Indicate that all object files have dependency on precompiled header
$(FEED_TEST_SRCS) : $(FEED_DST_PATH)/feed_os.h.gch

# Indicate dependency on makefiles
$(FEED_DST_PATH)/testfeed.exe : \
    $(FEED_SRC_PATH)/feed_project.mak \
    $(FEED_SRC_PATH)/feed_test.mak

# Indicate dependency on makefiles
$(FEED_TEST_SRCS): \
    $(FEED_SRC_PATH)/feed_project.mak \
    $(FEED_SRC_PATH)/feed_test.mak

