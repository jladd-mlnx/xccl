#
# Copyright (C) Mellanox Technologies Ltd. 2001-2019.  ALL RIGHTS RESERVED.
# See file LICENSE for terms.
#

AC_DEFUN([TCCL_CHECK_UCX],[

AS_IF([test "x$ucx_checked" != "xyes"],[

ucx_happy="no"

AC_ARG_WITH([ucx],
            [AS_HELP_STRING([--with-ucx=(DIR)], [Enable the use of UCX (default is guess).])],
            [], [with_ucx=guess])

AS_IF([test "x$with_ucx" != "xno"],
    [save_CPPFLAGS="$CPPFLAGS"
     save_CFLAGS="$CFLAGS"
     save_LDFLAGS="$LDFLAGS"

     AS_IF([test ! -z "$with_ucx" -a "x$with_ucx" != "xyes" -a "x$with_ucx" != "xguess"],
            [
            ucx_check_ucx_dir="$with_ucx"
            AS_IF([test -d "$with_ucx/lib64"],[libsuff="64"],[libsuff=""])
            ucx_check_ucx_libdir="$with_ucx/lib$libsuff"
            CPPFLAGS="-I$with_ucx/include $save_CPPFLAGS"
            LDFLAGS="-L$ucx_check_ucx_libdir $save_LDFLAGS"
            ])
        AS_IF([test ! -z "$with_ucx_libdir" -a "x$with_ucx_libdir" != "xyes"],
            [ucx_check_ucx_libdir="$with_ucx_libdir"
            LDFLAGS="-L$ucx_check_ucx_libdir $save_LDFLAGS"])

        AC_CHECK_HEADERS([ucp/api/ucp.h],
            [AC_CHECK_LIB([ucp] , [ucp_tag_send_nb],
                           [ucx_happy="yes"],
                           [AC_MSG_WARN([UCX is not detected. Disable.])
                            ucx_happy="no"])
            ], [ucx_happy="no"])

        CFLAGS="$save_CFLAGS"
        CPPFLAGS="$save_CPPFLAGS"
        LDFLAGS="$save_LDFLAGS"

        AS_IF([test "x$ucx_happy" = "xyes"],
            [
                AC_SUBST(UCX_CPPFLAGS, "-I$ucx_check_ucx_dir/include/ ")
                AC_SUBST(UCX_LDFLAGS, "-lucp -L$ucx_check_ucx_libdir")
            ],
            [
                AS_IF([test "x$with_ucx" != "xguess"],
                    [AC_MSG_ERROR([ucx support is requested but ucx packages cannot be found])],
                    [AC_MSG_WARN([UCX not found])])
            ])

    ],
    [AC_MSG_WARN([UCX was explicitly disabled])])

ucx_checked=yes
AM_CONDITIONAL([HAVE_UCX], [test "x$ucx_happy" != xno])
])

])
