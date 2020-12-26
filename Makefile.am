bin_PROGRAMS = evtmond
evtmond_SOURCES = src/evtmond.cpp
evtmond_SOURCES += src/evt_monitor.cpp
evtmond_SOURCES += src/dconfig.cpp
evtmond_SOURCES += src/logger.cpp

evtmond_LDADD = -lmnl -lpthread -ljsoncpp

if BUILD_netintfmon
evtmond_SOURCES += src/intf_evt_monitor.cpp
evtmond_SOURCES += src/net_interface.cpp
endif
