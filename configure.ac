AC_PREREQ([2.68])
AC_INIT([evtmond], [0.0.1])
AC_CONFIG_SRCDIR(src/evtmond.cpp)
AM_INIT_AUTOMAKE([subdir-objects 1.11 ])
AC_CONFIG_FILES([Makefile])

LT_INIT([dlopen disable-static])

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
     AM_CONDITIONAL([BUILD_$1], [test "${enable_hook}" = "yes"])
     enable_$1="$enable_hook"
    ]
)

m4_divert_once([HELP_ENABLE], [Configuration:])
m4_divert_once([HELP_ENABLE], [evtmond monitors:])
AC_HOOK([netintfmon],                 [no],      [network interface event monitoring.])

m4_divert_once([HELP_ENABLE], [])
m4_divert_once([HELP_ENABLE], [evtmond collectors:])
AC_HOOK([collecttofile],                 [no],      [Dump collected event to the file.])

AC_MSG_RESULT([    netintfmon . . . . . . .   $enable_netintfmon])
AC_MSG_RESULT([    collecttotofile . . . . . . . . . .   $enable_collecttofile])
