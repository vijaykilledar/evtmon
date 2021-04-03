bin_PROGRAMS = evtmond
evtmond_SOURCES = \
	src/evtmond.cpp \
	src/evtmond.h \
	src/evtmon_config.cpp \
	src/evtmon_config.h \
	src/common/evt_monitor.cpp \
	src/common/evt_monitor.h \
	src/common/logger.cpp \
	src/common/logger.h \
	src/common/global.h \
	src/common/utils.h

evtmond_CPPFLAGS = -I$(top_srcdir)/src/common
evtmond_CPPFLAGS += -I$(top_srcdir)/src

evtmond_LDADD = -lmnl -lpthread -ljsoncpp
EXTRA_DIST = autogen.sh

if BUILD_netintfmon
evtmond_SOURCES += \
	src/monitors/netintfmon/intf_evt_monitor.cpp \
	src/monitors/netintfmon/intf_evt_monitor.h \
	src/monitors/netintfmon/net_interface.cpp \
	src/monitors/netintfmon/net_interface.h
evtmond_CPPFLAGS += -I$(top_srcdir)/src/monitors/netintfmon/
endif
