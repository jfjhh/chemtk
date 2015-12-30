if HAVE_PDFLATEX

docdir         = $(datadir)/doc/@PACKAGE@
CMDSPECNAME    = command_specification
CMDSPECTEXSRC  = $(CMDSPECNAME).tex
CMDSPECAUX     = $(CMDSPECNAME).aux
CMDSPECLOG     = $(CMDSPECNAME).log
CMDSPECPDF     = $(CMDSPECNAME).pdf
docfiles       = $(CMDSPECPDF)
CLEANFILES     = $(CMDSPECPDF) $(CMDSPECAUX) $(CMDSPECLOG)
DISTCLEANFILES = $(CLEANFILES) $(docdir)
doc_DATA       = $(docfiles)

$(CMDSPECPDF): $(srcdir)/$(CMDSPECTEXSRC)
	$(PDFLATEX) $<

endif

