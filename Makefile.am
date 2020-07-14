bin_PROGRAMS = netevtmond
netevtmond_SOURCES = src/netevtmond.cpp
netevtmond_SOURCES += src/evt_monitor.cpp
netevtmond_SOURCES += src/dconfig.cpp
netevtmond_SOURCES += src/logger.cpp

netevtmond_LDADD = -lmnl

AM_CXXFLAGS=-g -std=c++11 -pthread

if BUILD_netintfmon
netevtmond_SOURCES += src/intf_evt_monitor.cpp
netevtmond_SOURCES += src/net_interface.cpp
endif
