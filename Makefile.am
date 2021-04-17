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

common_monitors_SOURCES = src/common/evt_monitor.cpp \
						  src/common/evt_monitor.h

evtmond_CPPFLAGS = -I$(top_srcdir)/src/common
evtmond_CPPFLAGS += -I$(top_srcdir)/src

evtmond_LDADD = -lmnl -lpthread -ljsoncpp -ldl
EXTRA_DIST = autogen.sh

pkglib_LTLIBRARIES = 
if BUILD_netintfmon
pkglib_LTLIBRARIES += libnetintfmon.la
libnetintfmon_la_SOURCES = \
	src/monitors/netintfmon/netintfmon.cpp \
	src/monitors/netintfmon/netintfmon.h \
	src/monitors/netintfmon/netlink_socket.cpp \
	src/monitors/netintfmon/netlink_socket.h \
	src/monitors/netintfmon/net_interface.cpp \
	src/monitors/netintfmon/net_interface.h
libnetintfmon_la_CPPFLAGS = -I$(top_srcdir)/src/monitors/netintfmon/ -I$(top_srcdir)/src/common
libnetintfmon_la_LDFLAGS = -lmnl
endif
