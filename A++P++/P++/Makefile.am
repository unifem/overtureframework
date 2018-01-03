## Process this file with automake to create Makefile.in

## makefile be used in make different Makefile
## include ../config/Makefile.setup

## include the A++P++ Makefile.config.in
include ../config/Makefile.config.in

if COMPILE_PADRE_DIRECTORY
   PADRE_SUBDIR = PADRE
else
 # PADRE_SUBDIR = PARTI
   PADRE_SUBDIR =
endif

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

# SUBDIRS = include $(PADRE_SUBDIR) src TESTS EXAMPLES $(PERFORMANCE_TESTS_SUBDIRS) $(DEVELOPMENT_DIRECTORIES)
SUBDIRS = include PARTI $(PADRE_SUBDIR) src TESTS EXAMPLES $(PERFORMANCE_TESTS_SUBDIRS) $(DEVELOPMENT_DIRECTORIES)

# Directories that MUST make it into the distribution (might not need to be set explicitly)
# DIST_SUBDIRS = include PADRE src TESTS EXAMPLES config Benchmark
# DIST_SUBDIRS = include PADRE src TESTS EXAMPLES PerformanceTests dqDeveloperTests


depend:
	cd src; $(MAKE) depend;
	cd PADRE; $(MAKE) depend;
	for i in @OPTIONAL_PADRE_SUBDIRS@; do \
	  ( cd $i; $(MAKE) depend )
	fi


ACLOCAL = @ACLOCAL@ -I ../acmacros -I ../config -I ./config -I PADRE/config -I ../CASC_MACROS
# Note: Make sure aclocal options in build and Makefile.am match.


distclean-local:
	rm -f machine.file
	rm -f src/array_src/config.h
	rm -f src/OPERATORS/config.h
	rm -f src/MDI/config.h
	rm -rf $(CXX_TEMPLATE_REPOSITORY) Templates.DB cxx_repository SunWS_cache PI* ti_files ii_files

# These are BTNG's rules and unless you are curious, you can ignore it.
padre-clean:
	@echo Making clean in all directories that use PADRE source codes.
	@for i in PADRE src/array_src src/OPERATORS PADRE/src TESTS EXAMPLES ;\
	do \
	  echo "Cleaning in $$i"; \
	  (cd $$i; $(MAKE) clean) ;\
	done
checks-clean:
	@echo Making clean in all checking directories.
	@for i in TESTS EXAMPLES;	\
	do	\
	  echo "Cleaning in $$i"; \
	  (cd $$i; $(MAKE) clean) ;\
	done
# End BTNG's rules.

all-local:
	$(MAKE) generalized-parallel-run

dist-hook:
	ln -s ../STL-link $(distdir)
#	echo "Making a link to the PADRE/PARTI directory for support of P++ without PADRE"
#	ln -s $(distdir)/PADRE/PARTI $(distdir)/PARTI

# Install config.status so people know how their library was configured.
install-data-local:
	$(INSTALL_DATA) $(top_builddir)/config.status $(DESTDIR)$(prefix)

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/config.status
