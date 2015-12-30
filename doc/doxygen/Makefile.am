SUBDIRS = html/

include doxygen_files.make

docdir     = $(datadir)/doc/@PACKAGE@
CLEANFILES = $(doxygen_files)

if HAVE_DOXYGEN

$(doxygen_files):
	$(DOXYGEN)

clean-generic:
	rm -rf $(CLEANFILES)

endif


