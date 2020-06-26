AC_PREREQ([2.68])
AC_INIT([netevtmon], [0.0.1])
AC_CONFIG_SRCDIR(src/netevtmond.cpp)
AM_INIT_AUTOMAKE([subdir-objects 1.11 -Wall -Werror])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_RANLIB

AC_OUTPUT

m4_divert_once([HELP_ENABLE], [netevtmond monitors:])

AC_ARG_ENABLE(netroutemon,
    [   --enable-netroutemon            Enables linux route event monitoring.],
    [   if test "${enableval}" = "yes"; then
            AC_DEFINE(ENABLE_NETROUTEMON)
        fi
    ])

AC_ARG_ENABLE(netconntrackemon,
    [   --enable-netconntrackmon        Enables linux conntrack event monitoring.],
    [   if test "${enableval}" = "yes"; then
            AC_DEFINE(ENABLE_NETCONNTRACKMON)
        fi
    ])
