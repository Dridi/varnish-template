================
varnish-template
================

The goal of this repository is to show a project skeleton dedicated to means
of extending Varnish. It serves as a both a template and a tutorial, relying
on facilities provided upstream by Varnish.

As a consequence this projects relies on:

- C99 (Varnish's programming language)
- autotools for building and testing
- ``pkg-config`` for the Varnish dependency
- ``libtool`` for VMODs compilation and linking
- ``python`` in order to run the ``vmodtool``
- ``rst2man`` to build manual pages
- ``rst2pdf`` (optional) to build PDF manuals

You can of course extend Varnish with other tools, you don't have to write
C code to interact with Varnish but the tools here are the ones used upstream.
Varnish maintains and supports integration with them (``varnishapi.pc``,
``varnish.m4``, etc).

Portability
===========

Successfully tested on:

- Fedora 26 (x86_64)
- FreeBSD 11.0 (amd64)

Extending Varnish
=================

There are mainly four ways to extend Varnish:

1. Utility programs
2. VCL code
3. C modules
4. Custom counters

While this order may seem counter-intuitive, it is actually very accurate.
Varnish ships itself with external programs, utilities that can add feature
outside of ``varnishd``'s scope. For example Varnish doesn't write access
logs, but instead provides ``varnishncsa`` that does just that.

Varnish UTilities (VUT)
-----------------------

Such utilities can interact with Varnish via the ``libvarnishapi`` library
that accesses Varnish SHared Memory LOG (shmlog). You may interact with the
``varnish-cli``\(7), the ``varnish-counters``\(7) or the ``vsl``\(7). In the
case of VSL tools, you can even avoid most of the work as demonstrated in this
repository using the VUT functions.

Two of the tools provided by Varnish are particularly nice to build new tools
on top of them: ``varnishncsa`` and ``varnishadm``. The former because its
output is parsing-friendly, and the latter is a good fit for shell scripting.

VCL
---

Why isn't VCL the first thing to extend Varnish? Mostly because that's a
mandatory step to even use Varnish. You can define your caching policy, but
you can also develop applications in VCL. Real-life examples include paywalls,
WAFs, analytics...

Varnish 5.0 introduced a directory in ``/usr`` in the ``vcl_path`` in addition
to the classic one located in ``/etc`` (assuming a default installation). It
means that you can ship VCL that can be included from a relative path, and you
can as a result expect after installing varnish-template to do this::

    include "varnish-template/common.vcl";

It will work without extra configuration. Since Varnish 5.1 installing a VCL
library requires even less effort.

You can also copy it to ``/etc/varnish/varnish-template/common.vcl`` if you
need to make changes to the file while preserving the original one: fix a bug
locally until an update takes care of it for instance. This is akin to what
systemd does with units.

C modules
---------

Because VCL is a domain-specific and safe language, it has a limited syntax
and limited capabilities once you try to move outside of the domain (HTTP
messages and caching). You can write VMODs (Varnish MODules) to interact
deeper with Varnish, and possibly outside Varnish.

Custom Counters
---------------

Since Varnish 5.2 it is possible to ship Varnish Shared Counters (VSC) via
VMODs. These counters show up in tools like ``varnishstat`` and effectively
allow to keep track of custom metrics alongside those already provided by
Varnish. If you are already monitoring Varnish counters, then custom counters
need no further integration.

Counters defined by VMODs is still an experimental feature and is not expected
to be stable.

Build
=====

This template uses the GNU build system aka the autotools. You can build it
and run the test suite with the following commands::

    ./bootstrap
    make
    make check

You can then install it::

    sudo make install

The ``bootstrap`` script takes care of the ``configure`` invocation. If you
need to reconfigure your project, you can run ``configure`` directly, unless
you changed ``configure.ac``. The ``bootstrap`` script also supports VPATH
builds. Arguments to the ``bootstrap`` script are passed to ``configure``.

If Varnish was not installed in the default system's location, you need to
set the proper PATHs prior to running ``bootstrap``::

    export PKG_CONFIG_PATH=/path/to/varnish/lib/pkgconfig
    export ACLOCAL_PATH=/path/to/varnish/share/aclocal

The ``PKG_CONFIG_PATH`` may differ on multi-lib systems.

The installation prefix is inherited from Varnish, so there's usually no need
to specify one. And the test suite is set up in a way that allows test cases
to be runnable both using ``make check`` in the build tree or ``varnishtest``
once the package is installed.

After a standard installation, you can try::

    $ varnishtest /usr/share/doc/varnish-template/template.vtc
    #    top  TEST /usr/share/doc/varnish-template/template.vtc passed (2.632)

Documentation
=============

Documentation is not an afterthought, it is featured in this template too.
Moreover, Varnish generates the documentation as ReStructured Text (RST) for
VMODs, based on their descriptor (the VCC file). If you are writing a utility
program, you may even be able to generate RST for the synopsis and options.

It is not possible to define an include path in RST ; in order to do so one
would need to write their own include directive from scratch. Instead, you can
manage includes by:

- renaming your offending ``.rst`` file to ``.rst.in``
- adding it to ``AC_CONFIG_FILES`` in ``configure.ac``
- making includes relative to ``@builddir@``

This way it should be possible to mix generated and hand-written parts of your
manuals without too much hassle. This approach is also compatible with VPATH
builds.

RPM packaging
=============

This repository features turnkey RPM packaging. It comes in two flavors: you
can either use RPM directly or build for a specific platform using ``mock(1)``
and an appropriate configuration.

To build using ``rpmbuild(1)``::

    make rpm

The resulting packages can be found in the ``rpmbuild`` directory in your
build tree.

Building with ``mock(1)`` can be as simple as::

    make mockbuild

It is set up by default to use a configuration based on CentOS 7 x86_64 with
EPEL, and the YUM repositories for the Varnish 5.1 branch. You can specify
additional options using the ``MOCK_OPTS`` variable. For example on Fedora
you need to explicitly use DNF.

Building with custom options::

    make mockbuild MOCK_OPTS="--dnf --root my-mock-config.cfg"

The resulting packages can be found in the ``mockbuild`` directory in your
build tree.

License
=======

Don't feel obliged to comply to the GPLv3+ license, choose your own. It was
selected as an example license for the template, just for the sake of having
a real license. Also that would be my license of choice.

Known limitations
=================

This template is not perfect, it started as a one-hour hack:

- It requires at least Varnish 5.2.0, the latest release at the time these
  lines were written. Some of the build system's goodies aren't available on
  older releases, but with proper version checking it is possible to only
  use what's supported.

- Only RPM packaging is supported out of the box.

- The hello world of this template is bloody stupid.

- There is no branding script to automate a rename if you want to spin your
  own project off this structure.

- It's still lacking (around 0% completion) on the tutorial side.

- It needs to be tested on more platforms.
