include config/Makefile.override.install.includeHEADERS

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
MAKE=gmake

ACLOCAL = @ACLOCAL@ -I ../../acmacros -I ./config
# Note: make sure aclocal options match those in build.

# Optional use of Parti.
# if ENABLE_Parti
#    PARTI_SUBDIRS = PARTI
# endif

# Optional use of IndirectAddressing.
if ENABLE_IndirectAddressing
   IndirectAddressing_SUBDIRS = IndirectAddressing
endif

# Make sure that src comes after sublibraries, because the building of
# libPADRE_withsubs.a requires that sublibraries are already built.
# DQ (12/26/2001) I don't see why we should have config listed here
# SUBDIRS = $(PARTI_SUBDIRS) PGSLIB $(IndirectAddressing_SUBDIRS) config src
# SUBDIRS = $(PARTI_SUBDIRS) PGSLIB $(IndirectAddressing_SUBDIRS) src
SUBDIRS = PGSLIB $(IndirectAddressing_SUBDIRS) src

# Make sure this gets into the distribution (until we eliminate it via new Autoconf 2.52 features)
# EXTRA_DIST = acconfig.h

depend:
	cd src; $(MAKE) depend;

# PADRE_config.h is the PADRE-specific configuration macros.
# It is installed so that PADRE source code is used in the
# same way it was configured.  BTNG.
# install-data-local:
#	$(INSTALL_DATA) $(top_builddir)/PADRE_config.h $(DESTDIR)$(includedir)

dist-hook:
	ln -s ../../STL-link $(distdir)
	mkdir $(distdir)/config
	cp $(srcdir)/config/Makefile.override.install.includeHEADERS $(distdir)/config


distclean-local:
	rm -rf $(CXX_TEMPLATE_REPOSITORY) Templates.DB cxx_repository SunWS_cache PI* ti_files ii_files




















