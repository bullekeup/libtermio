
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I ../m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -I../include

## Define a libtool archive target "libtermio-@LIBTERMIO_API_VERSION@.la", with
## @LIBTERMIO_API_VERSION@ substituted into the generated Makefile at configure
## time.
## The libtool archive file (.la) will be installed into the directory named
## by the predefined variable $(bindir), along with the actual shared library
## file (.so).
lib_LTLIBRARIES = libtermio-@LIBTERMIO_API_VERSION@.la

libtermio_@LIBTERMIO_API_VERSION@_la_CFLAGS = \
  -fPIC \
  -fPIE \
  $(AM_CFLAGS)

## Define the source file list for the "libtermio.la" target.
## Note that it is not necessary to list header files which are already listed
## elsewhere in a _HEADERS variable assignment.
## TODO: Add dtls files and auth files
libtermio_@LIBTERMIO_API_VERSION@_la_SOURCES = \
  ./termurxvt.c \
  ./termxterm.c \
  ./termutils.c \
  ./terminal.c

libtermio_includedir = $(includedir)/termio
libtermio_include_HEADERS = \
  ../include/termio/terminal.h \
  ../include/termio/termxterm.h \
  ../include/termio/termurxvt.h \
  ../include/termio/ansicodes.h \
  ../include/termio/termutils.h

# This helper is called by libcoap-$(LIBCOAP_API_VERSION).{map,sym} to see if
# configure has detected a usable version of the ctags program and aborts if not.
check_ctags:
	@if [ "$(CTAGS_PROG)" = "" ]; then \
		echo ;\
		echo "There was no ctags program found by the configure script!" ;\
		echo "ctags is needed for running this target! Please note the warning about the missed ctags program of the configure script." ;\
		echo ;\
		exit 1;\
	fi

.PHONY: check_ctags

#sed in ctags before sort | awk '/^coap_/ { print "  " $$1 ";" }'
libtermio-$(LIBTERMIO_API_VERSION).map: check_ctags $(libtermio_include_HEADERS)
	( echo "VER_$(LIBCOAP_API_VERSION) {" ; \
	echo "global:" ; \
	$(CTAGS_PROG) $(CTAGS_IGNORE) -f - --c-kinds=p $(libtermio_include_HEADERS) | awk '/.*/ { print "  " $$1 ";" }' | sort ; \
	echo "local:" ; \
	echo "  *;" ; \
	echo "};" ) > $@.new
	mv $@.new ../../$@

# | awk '/^coap_/ { print "  " $$1 ";" }'
libtermio-$(LIBTERMIO_API_VERSION).sym: check_ctags $(libtermio_include_HEADERS)
	( $(CTAGS_PROG) $(CTAGS_IGNORE) -f - --c-kinds=p $(libtermio_include_HEADERS) | awk '/.*/ { print $$1 }' | sort ) \
	> $@.new
	mv $@.new ../../$@

# If there is a API change to something $(LIBTERMIO_API_VERSION) > 1 the install
# prefix for the header files has to change to not conflict the older version
# if the user want's to install both versions. There will be something used like
# libtermio_include_HEADERS = \
#  $(top_srcdir)/include/termio-$(LIBTERMIO_API_VERSION)/*

## Instruct libtool to include API version information in the generated shared
## library file (.so). The library ABI version will later defined in configure.ac,
## so that all version information is kept in one place.
libtermio_@LIBTERMIO_API_VERSION@_la_LDFLAGS =					\
  -version-info $(LT_LIBTERMIO_CURRENT):$(LT_LIBTERMIO_REVISION):$(LT_LIBTERMIO_AGE)	\
  @libtermio_SYMBOLS@ \
  -pie -lutil
