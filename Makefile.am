# Makefile.am for libtermio
#
# Copyright (C) 2010-2015 Olaf Bergmann <bergmann@tzi.org>
#           (C) 2015-2016 Carsten Schoenert <c.schoenert@t-online.de>
#           (C) 2016 Axel Moinet <axel.moinet@u-bourgogne.fr>
#
# This file is part of the libtermio C library. Please see README and
# COPYING for terms of use.

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AUTOMAKE_OPTIONS = subdir-objects

## Additional files for the distribution archive
EXTRA_DIST = \
  CONTRIBUTE \
  TODO \
  $(pkgconfig_DATA).in \
  libtermio-$(LIBTERMIO_API_VERSION).map \
  libtermio-$(LIBTERMIO_API_VERSION).sym 

SUBDIRS = . $(DOC_DIR) src

#CTAGS_IGNORE=-I "termio_packet_extract_pbuf coap_pdu_from_pbuf"

.PHONY:	update-map-file subdirs $(SUBDIRS) libtermio-$(LIBTERMIO_API_VERSION).map libtermio-$(LIBTERMIO_API_VERSION).sym

## Helper target to generate the symbol table needed by libtool.
## The .map format is used when ld supports linker scripts, otherwise
## it must fall back to a plain symbol file.
update-map-file: libtermio-$(LIBTERMIO_API_VERSION).map libtermio-$(LIBTERMIO_API_VERSION).sym

libtermio-$(LIBTERMIO_API_VERSION).map:
	$(MAKE) libtermio-$(LIBTERMIO_API_VERSION).map -C src

libtermio-$(LIBTERMIO_API_VERSION).sym:
	$(MAKE) libtermio-$(LIBTERMIO_API_VERSION).sym -C src

clean-local:
	rm libtermio-1.sym
	rm libtermio-1.map

distclean-local: clean-local

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtermio-$(LIBTERMIO_API_VERSION).pc

## Define an independent executable script for inclusion in the distribution
## archive.  However, it will not be installed on an end user's system due to
## the noinst_ prefix.
dist_noinst_SCRIPTS = autogen.sh

all-am: update-map-file
