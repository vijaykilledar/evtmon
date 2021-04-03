bin_PROGRAMS = evtmond
evtmond_SOURCES = src/evtmond.cpp
evtmond_SOURCES += src/common/evt_monitor.cpp
evtmond_SOURCES += src/evtmon_config.cpp
evtmond_SOURCES += src/common/logger.cpp

evtmond_LDADD = -lmnl -lpthread -ljsoncpp
evtmond_CPPFLAGS = -Isrc/common

if BUILD_netintfmon
evtmond_SOURCES += src/monitors/netintfmon/intf_evt_monitor.cpp
evtmond_SOURCES += src/common/net_interface.cpp
evtmond_CPPFLAGS += -Isrc/monitors/netintfmon/
endif
