docdir        = $(datadir)/doc/@PACKAGE@
DOCFILES      = README DONTS
doc_DATA      = $(DOCFILES)
SUBDIRS       = commands/ doxygen/

uninstall-hook:
	-if test -d $(docdir); then rm -rf $(docdir); fi

