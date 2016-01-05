GIANTARCHIVE      = data.tar.xz
EXTRA_DIST        = commands.yml constants.txt data.h elements get_files \
		    pager.txt scicalc.txt
dist_data_DATA    = $(GIANTARCHIVE) commands.yml data.h pager.txt scicalc.txt
dist_pkgdata_DATA = $(GIANTARCHIVE) commands.yml data.h pager.txt scicalc.txt

install-data-hook:
	cd $(DESTDIR)$(pkgdatadir); \
		cat $(GIANTARCHIVE) | $(UNXZ) | $(TAR) -t > uninstall_manifest.txt; \
		cat $(GIANTARCHIVE) | $(UNXZ) | $(TAR) --no-same-owner -x; \
		rm -f $(GIANTARCHIVE); \
		cat uninstall_manifest.txt | sed 's/^\|$$/"/g' | xargs chmod a=rX,u+w

uninstall-local:
	cd $(DESTDIR)$(pkgdatadir); \
		cat uninstall_manifest.txt | sed 's/ /\\ /g' | xargs rm -rf; \
		rm -f uninstall_manifest.txt; rmdir $(DESTDIR)$(pkgdatadir)

