if HAVE_DOXYGEN

doxygendir      = $(top_builddir)/doc/doxygen
DOXYGENDB       = doxygen_sqlite3.db
DOXYFILE        = Doxyfile
HTMLDOC         = html/*
CLEANFILES      = $(HTMLDOC) $(DOXYGNEDB)
DISTCLEANFILES  = $(HTMLDOC) $(DOXYGENDB)
dist_doc_DATA   = $(HTMLDOC) $(DOXYFILE)

$(HTMLDOC):
	$(DOXYGEN) $(DOXYFILE)

endif

