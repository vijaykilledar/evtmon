bin_PROGRAMS = netevtmond
netevtmond_SOURCES = src/netevtmond.cpp
netevtmond_SOURCES += src/evt_monitor.cpp
netevtmond_SOURCES += src/dconfig.cpp

netevtmond_LDADD = -lmnl

if BUILD_netintfmon
netevtmond_SOURCES += src/intf_evt_monitor.cpp
netevtmond_SOURCES += src/net_interface.cpp
endif
