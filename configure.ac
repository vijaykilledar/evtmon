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

dnl
AC_DEFUN(
    [AC_HOOK],
    [
     enable_hook="no"
     AC_ARG_ENABLE([$1],
                   [AS_HELP_STRING([--enable-$1],[$3])],
                   [    if test "${enableval}" = "yes"; then
                            enable_hook="yes"
                        fi
                   ])
     enable_$1="$enable_hook"
    ]
)

m4_divert_once([HELP_ENABLE], [netevtmond monitors:])

AC_HOOK([netroutemon],                [yes],      [linux route event monitoring.])
AC_HOOK([netconntrackemon],           [yes],      [linux conntrack event monitoring.])

AC_MSG_RESULT()
AC_MSG_RESULT([  Net Evt Monitors:])
AC_MSG_RESULT([    netroutemon . . . . . . .  $enable_netroutemon])
AC_MSG_RESULT([    netconntrackemon . . . . . $enable_netconntrackemon])
