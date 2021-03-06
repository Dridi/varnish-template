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

AM_CFLAGS = $(VARNISHAPI_CFLAGS)

# VMOD library

vmod_LTLIBRARIES = libvmod_template.la

libvmod_template_la_LDFLAGS = $(VMOD_LDFLAGS)
libvmod_template_la_SOURCES = vmod_template.c
nodist_libvmod_template_la_SOURCES = \
	vcc_template_if.c \
	vcc_template_if.h \
	VSC_tpl.c \
	VSC_tpl.h

@BUILD_VSC_TPL@
@BUILD_VMOD_TEMPLATE@

# VUT program

bin_PROGRAMS = varnishtemplate

varnishtemplate_LDFLAGS = $(VARNISHAPI_LIBS)
varnishtemplate_SOURCES = \
	varnishtemplate.c \
	varnishtemplate_options.h

# Test suite

AM_TESTS_ENVIRONMENT = \
	PATH="$(abs_builddir):$(VARNISH_TEST_PATH):$(PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = \
	-p vcl_path="$(abs_top_srcdir)/vcl" \
	-p vmod_path="$(abs_builddir)/.libs:$(vmoddir)"

TESTS = tests/template.vtc

# Documentation

dist_doc_DATA = \
	vmod_template.vcc \
	tpl.vsc \
	$(TESTS)

dist_man_MANS = \
	varnishtemplate.1 \
	vmod_template.3

vmod_template.3: VSC_tpl.rst

@GENERATE_VARNISHTEMPLATE_DOCS@

pdf-local: \
	varnishtemplate.pdf \
	vmod_template.pdf

.rst.1:
	$(AM_V_GEN) $(RST2MAN) $< $@

.rst.pdf:
	$(AM_V_GEN) rst2pdf $< $@

CLEANFILES = *.pdf
