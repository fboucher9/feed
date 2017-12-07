# See LICENSE for license details

#
# Module: feed_clean.mak
#
# Description:
#	Clean temporary files
#

ifndef FEED_DST_PATH
FEED_DST_PATH = .
endif

.PHONY: clean
clean :
	-rm -f $(FEED_DST_PATH)/_obj_*
	-rm -f $(FEED_DST_PATH)/feed_os.h.gch
	-rm -f $(FEED_DST_PATH)/feed_os.h.gch.oxx
	-rm -f $(FEED_DST_PATH)/testfeed.exe
	-rm -f $(FEED_DST_PATH)/libfeed.a
	-rm -f $(FEED_DST_PATH)/libfeed.so

