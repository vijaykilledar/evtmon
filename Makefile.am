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
	src/common/utils.h \
	src/common/event.h \
	src/common/event.cpp \
	src/common/event_queue.h \
	src/common/event_queue.cpp


evtmond_CPPFLAGS = -I$(top_srcdir)/src/common
evtmond_CPPFLAGS += -I$(top_srcdir)/src

evtmond_LDADD = -lpthread -ljsoncpp -ldl
EXTRA_DIST = autogen.sh

pkglib_LTLIBRARIES = 

if BUILD_netintfmon
pkglib_LTLIBRARIES += libnetintfmon.la
libnetintfmon_la_SOURCES = \
	src/monitors/netintfmon/netintfmon.cpp \
	src/monitors/netintfmon/netintfmon.h \
	src/monitors/netintfmon/net_interface.cpp \
	src/monitors/netintfmon/net_interface.h \
	src/common/event.h \
	src/common/event.cpp \
	src/common/event_queue.h \
	src/common/event_queue.cpp
libnetintfmon_la_CPPFLAGS = -I$(top_srcdir)/src/monitors/netintfmon/ -I$(top_srcdir)/src/common
libnetintfmon_la_LDFLAGS = -lmnl
endif
