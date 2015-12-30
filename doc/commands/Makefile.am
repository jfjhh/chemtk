docdir = $(datadir)/doc/@PACKAGE@

if HAVE_PDFLATEX

CMDSPECNAME   = command_specification
CMDSPECTEXSRC = $(CMDSPECNAME).tex
CMDSPECAUX    = $(CMDSPECNAME).aux
CMDSPECLOG    = $(CMDSPECNAME).log
CMDSPECPDF    = $(CMDSPECNAME).pdf
docfiles      = $(CMDSPECPDF)
CLEANFILES    = $(CMDSPECPDF) $(CMDSPECAUX) $(CMDSPECLOG)
doc_DATA      = $(docfiles)
dist_doc_DATA = $(docfiles)

$(CMDSPECPDF): $(srcdir)/$(CMDSPECTEXSRC)
	$(PDFLATEX) $<

endif

