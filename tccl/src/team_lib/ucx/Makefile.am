#
# Copyright (c) 2009-2020 Mellanox Technologies.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

if HAVE_UCX
allreduce = allreduce/allreduce.h \
	allreduce/allreduce_knomial.c

reduce = reduce.h \
	reduce/reduce_linear.c

fanin = fanin.h \
	fanin/fanin_linear.c

fanout = fanout.h \
	fanout/fanout_linear.c

bcast = bcast.h \
	bcast/bcast_linear.c\
	bcast/bcast_knomial.c

barrier = barrier.h \
	barrier/barrier_knomial.c

sources = \
	tccl_ucx_tag.h \
	tccl_ucx_team.h \
	tccl_ucx_team.c \
	tccl_ucx_context.h \
	tccl_ucx_context.c \
	tccl_ucx_lib.h \
	tccl_ucx_ep.h \
	tccl_ucx_lib.c \
	tccl_ucx_sendrecv.h \
	${allreduce} \
	${reduce} \
	${fanin}\
	${fanout}\
	${bcast} \
	${barrier}

component_noinst =
component_install = tccl_team_lib_ucx.la

tccl_team_lib_ucx_la_SOURCES =$(sources)
tccl_team_lib_ucx_la_CPPFLAGS = $(AM_CPPFLAGS) $(UCX_CPPFLAGS)
tccl_team_lib_ucx_la_LDFLAGS = -module -avoid-version $(UCX_LDFLAGS)

pkglib_LTLIBRARIES = $(component_install)
endif
