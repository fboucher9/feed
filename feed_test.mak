# See LICENSE for license details

#
# Module: feed_test.mak
#
# Description:
#	Build a test application for feed library.
#

FEED_TEST_MODULES = feed_os feed_main

FEED_TEST_SRCS = $(foreach module,$(FEED_TEST_MODULES), $(FEED_OBJ_PATH)$(module).o)

FEED_TEST_SRCS += $(FEED_OBJ_PATH)libfeed.a

# Default target
.PHONY: all
all : testfeed

# Test application
.PHONY: testfeed
testfeed: $(FEED_OBJ_PATH)testfeed.exe

# Link the target
$(FEED_OBJ_PATH)testfeed.exe.cmd : $(FEED_OBJ_PATH)
	@echo generating $(FEED_OBJ_PATH)testfeed.exe.cmd
	@echo -o $(FEED_OBJ_PATH)testfeed.exe $(FEED_CFLAGS) $(FEED_TEST_SRCS) $(FEED_LDFLAGS) $(FEED_LIBS) > $(FEED_OBJ_PATH)testfeed.exe.cmd

$(FEED_OBJ_PATH)testfeed.exe : $(FEED_TEST_SRCS) | $(FEED_OBJ_PATH) $(FEED_OBJ_PATH)testfeed.exe.cmd
	@echo linking $(FEED_OBJ_PATH)testfeed.exe
	@$(FEED_CC) @$(FEED_OBJ_PATH)testfeed.exe.cmd

$(foreach module,$(FEED_TEST_MODULES),$(eval $(call FEED_compile,$(module))))

# Indicate that all object files have dependency on precompiled header
$(FEED_TEST_SRCS) : $(FEED_OBJ_PATH)feed_os.h.gch

# Indicate dependency on makefiles
$(FEED_OBJ_PATH)testfeed.exe : \
    $(FEED_SRC_PATH)feed_project.mak \
    $(FEED_SRC_PATH)feed_test.mak

# Indicate dependency on makefiles
$(FEED_TEST_SRCS): \
    $(FEED_SRC_PATH)feed_project.mak \
    $(FEED_SRC_PATH)feed_test.mak

.PHONY: clean
clean: feed_test_clean

.PHONY: feed_test_clean
feed_test_clean:
	-rm -r -f $(FEED_OBJ_PATH)
