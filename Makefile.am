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

ACLOCAL_AMFLAGS = -I m4 -I $(VARNISHAPI_DATAROOTDIR)/aclocal

DISTCHECK_CONFIGURE_FLAGS = RST2MAN=:

SUBDIRS = src

dist_doc_DATA = README.rst LICENSE
dist_pkgvcl_DATA = vcl/$(PACKAGE)/common.vcl

EXTRA_DIST = $(PACKAGE).spec

# RPM packaging

RPMBUILD = CC= PKG_CONFIG_PATH= ACLOCAL_PATH= \
	rpmbuild -D '_rpmdir rpmbuild' -D '_srcrpmdir rpmbuild/src'
MOCK = mock -r varnish-51-epel-7-x86_64.cfg $(MOCK_OPTS) --resultdir mockbuild

rpm: dist rpmbuild-clean
	$(RPMBUILD) -ts $(PACKAGE)-$(VERSION).tar.gz
	$(RPMBUILD) --rebuild rpmbuild/src/$(PACKAGE)-$(VERSION)-*.src.rpm

rpmlint: rpm
	rpmlint $(PACKAGE).spec rpmbuild/*/$(PACKAGE)-*.rpm

mockbuild: dist mockbuild-clean
	$(MOCK) --buildsrpm --sources . --spec $(PACKAGE).spec
	$(MOCK) --rebuild mockbuild/$(PACKAGE)-$(VERSION)-*.src.rpm

mocklint: mockbuild
	rpmlint $(PACKAGE).spec mockbuild/$(PACKAGE)-*.rpm

rpmbuild-clean:
	rm -rf rpmbuild/

mockbuild-clean:
	rm -rf mockbuild/

clean-local: rpmbuild-clean mockbuild-clean

.PHONY: rpmbuild-clean mockbuild-clean

# XXX: not shipped yet by Varnish
EXTRA_DIST += src/foreign
