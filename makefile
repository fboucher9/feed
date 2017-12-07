# See LICENSE for license details

#
# Module: makefile
#
# Description:
#   Template of makefile to demonstrate out-of-source build.
#

# Default is to build in-source
# Override this setting to build out-of-source
ifndef FEED_SRC_PATH
FEED_SRC_PATH = .
endif

include $(FEED_SRC_PATH)/feed_project.mak

include $(FEED_SRC_PATH)/feed_test.mak
