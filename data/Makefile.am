# pkgdatadir = $(datadir)/data/@PACKAGE@
# pkg_DATA = constants.txt elements/ get_files/ legal/ pager.txt scicalc.txt
# GIANTARCHIVE = data.tar.xz

# EXTRA_DIST = $(srcdir)/$(GIANTARCHIVE)

# install-data-hook:
# 	cd $(DESTDIR)$(pkgdatadir); \
	# 		tar xJvf $(srcdir)/$(GIANTARCHIVE) --no-same-owner \
	# 		| tee uninstall_manifest.txt; \
	# 		rm -f $(GIANTARCHIVE); \
	# 		cat uninstall_manifest.txt | sed 's/^\|$$/"/g' \
	# 		| xargs chmod a=rX,u+w

# uninstall-local:
# 	cd $(DESTDIR)$(pkgdatadir); \
	# 		cat uninstall_manifest.txt | sed 's/ /\\ /g' | xargs rm -f; \
	# 		rm -f uninstall_manifest.txt


GIANTARCHIVE = data.tar.xz

dist_pkgdata_DATA = $(GIANTARCHIVE)

install-data-hook:
	cd $(DESTDIR)$(pkgdatadir); \
		cat $(GIANTARCHIVE) | $(UNXZ) | $(TAR) --list > uninstall_manifest.txt; \
		cat $(GIANTARCHIVE) | $(UNXZ) | $(TAR) --no-same-owner --extract; \
		rm --force $(GIANTARCHIVE); \
		cat uninstall_manifest.txt | sed --expression='s/^\|$$/"/g' | xargs chmod a=rX,u+w

uninstall-local:
	cd $(DESTDIR)$(pkgdatadir); \
		cat uninstall_manifest.txt | sed --expression='s/ /\\ /g' | xargs rm --force; \
		rm --force uninstall_manifest.txt
