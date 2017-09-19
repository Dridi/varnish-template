# Copyright (C) 2017  Dridi Boukelmoune
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This is what an autoconf macro for counters may look like in future
# versions of varnish.m4, and needs to be maintained out of tree for
# now.
m4_ifndef([VARNISH_COUNTERS], [

# _VARNISH_VSC_CONFIG
# --------------------
AC_DEFUN([_VARNISH_VSC_CONFIG], [

	AC_REQUIRE([_VARNISH_PKG_CONFIG])
	AC_REQUIRE([_VARNISH_CHECK_DEVEL])
	AC_REQUIRE([_VARNISH_CHECK_PYTHON])

	dnl Define an automake silent execution for vmodtool
	[am__v_VSCTOOL_0='@echo "  VSCTOOL " $''@;']
	[am__v_VSCTOOL_1='']
	[am__v_VSCTOOL_='$(am__v_VSCTOOL_$(AM_DEFAULT_VERBOSITY))']
	[AM_V_VSCTOOL='$(am__v_VSCTOOL_$(V))']
	AC_SUBST([am__v_VSCTOOL_0])
	AC_SUBST([am__v_VSCTOOL_1])
	AC_SUBST([am__v_VSCTOOL_])
	AC_SUBST([AM_V_VSCTOOL])
])

# _VARNISH_COUNTER(NAME)
# ----------------------
AC_DEFUN([_VARNISH_COUNTER], [

	AC_REQUIRE([_VARNISH_VSC_CONFIG])

	VSC_RULES="

VSC_$1.h: $1.vsc
	\$(A""M_V_VSCTOOL) \$(PYTHON) \$(VSCTOOL) -h \$(srcdir)/$1.vsc

VSC_$1.c: $1.vsc
	\$(A""M_V_VSCTOOL) \$(PYTHON) \$(VSCTOOL) -c \$(srcdir)/$1.vsc

VSC_$1.rst: $1.vsc
	\$(A""M_V_VSCTOOL) \$(PYTHON) \$(VSCTOOL) -r \$(srcdir)/$1.vsc >VSC_$1.rst

clean: clean-vsc-$1

distclean: clean-vsc-$1

clean-vsc-$1:
	rm -f VSC_$1.h VSC_$1.c VSC_$1.rst

"

	AC_SUBST(m4_toupper(BUILD_VSC_$1), [$VSC_RULES])
	m4_ifdef([_AM_SUBST_NOTMAKE],
		[_AM_SUBST_NOTMAKE(m4_toupper(BUILD_VSC_$1))])
])

# VARNISH_COUNTERS(NAMES)
# -----------------------
AC_DEFUN([VARNISH_COUNTERS], [
	m4_foreach([_vsc_name],
		m4_split(m4_normalize([$1])),
		[_VARNISH_COUNTER(_vsc_name)])
])



])
