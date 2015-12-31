if HAVE_DOXYGEN

docdir          = $(datadir)/doc/@PACKAGE@
doxygendir      = $(top_srcdir)/doc/doxygen
DOXYGENDB       = doxygen_sqlite3.db
DOXYFILE        = Doxyfile
HTMLDOC         = html/*
CLEANFILES      = $(HTMLDOC) $(DOXYGNEDB)
DISTCLEANFILES  = $(HTMLDOC) $(DOXYGENDB)
dist_doc_DATA   = $(HTMLDOC) $(DOXYFILE)

$(HTMLDOC):
	$(DOXYGEN) $(doxygendir)/$(DOXYFILE)

endif

