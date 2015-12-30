if HAVE_DOXYGEN

docdir         = $(datadir)/doc/@PACKAGE@
HTMLDOC        = html/* doxygen_sqlite3.db
CLEANFILES     = $(HTMLDOC)
DISTCLEANFILES = $(HTMLDOC) $(docdir)
doc_DATA       = $(HTMLDOC)

$(HTMLDOC):
	$(DOXYGEN)

endif

