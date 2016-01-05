AUTOMAKE_OPTIONS = -Wportability gnu dist-xz subdir-objects
ACLOCAL_AMFLAGS  = -I m4
SUBDIRS          = . man doc data src

distcleancheck_listfiles = find . -type f -exec sh -c 'test -f $(srcdir)/$$1 \
			   || echo $$1' sh '{}' ';'

