## Process this file with automake to create Makefile.in

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../config/Makefile.setup

## include $(top_srcdir)/config/Makefile.config.in
include ../config/Makefile.config.in

if COMPILE_PERFORMANCE_TESTS
   PERFORMANCE_TESTS_SUBDIRS = PerformanceTests
else
   PERFORMANCE_TESTS_SUBDIRS = 
endif

if DQ_DEVELOPER_TESTS
   DEVELOPMENT_DIRECTORIES = dqDeveloperTests
else
   DEVELOPMENT_DIRECTORIES = 
endif

## EXTRA_DIST = BUGS README-alpha
# SUBDIRS = include src TESTS EXAMPLES config @OPTIONAL_PERFORMANCE_TESTS_SUBDIRS@
# SUBDIRS = include src TESTS EXAMPLES config $(PERFORMANCE_TESTS_SUBDIRS) $(DEVELOPMENT_DIRECTORIES)
SUBDIRS = include src TESTS EXAMPLES $(PERFORMANCE_TESTS_SUBDIRS) $(DEVELOPMENT_DIRECTORIES)

## We don't have to specify these since A++ is always in the distribution (???)
## DIST_SUBDIRS = config

## touch include/config.h so that all files will not be rebuilt
depend:
	cd src; $(MAKE) depend;
	touch include/config.h

#dist-hook:
#	cp -rp $(srcdir)/config $(distdir)/config

# Install config.status so people know how their library was configured.
# version autoconf 2.52 and greater now store this info in config.log
install-data-local:
	$(INSTALL_DATA) $(top_builddir)/config.status $(DESTDIR)$(prefix)
	$(INSTALL_DATA) $(top_builddir)/config.log $(DESTDIR)$(prefix)

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/config.status
	rm -f $(DESTDIR)$(prefix)/config.log

all-local:
	$(MAKE) generalized-serial-run


ACLOCAL = @ACLOCAL@ -I ../acmacros -I ../config -I ./config -I ../CASC_MACROS
# Note: Make sure aclocal options in build and Makefile.am match.

# EXTRA_DIST = tarch

distclean-local:
	rm -f src/array_src/config.h
	rm -f src/OPERATORS/config.h
	rm -f src/MDI/config.h

# These are BTNG's rules and unless you are curious, you can ignore it.
checks-clean:
	@echo Making clean in all checking directories.
	@for i in TESTS EXAMPLES;	\
	do	\
	  echo "Cleaning in $$i"; \
	  (cd $$i; $(MAKE) clean) ;\
	done
# End BTNG's rules.















