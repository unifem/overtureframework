## Process this file with automake to create Makefile.in

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
MAKE=gmake

ACLOCAL = @ACLOCAL@ -I config -I A++/config -I P++/config -I ./CASC_MACROS
# Note: make sure aclocal options between build and Makefile.am match.

## Get the subdirectries from the configure script (this allows P++ to be optional)
## Use this to permit autoconf to specify the subsets to be 
## built (as in building A++ and P++) (page 15-16 in automake manual)
## SUBDIRS = @subdirs@

# Use Automake's conditional make mechanism to control use of P++
if COMPILE_PXX_DIRECTORY
   PXX_SUBDIR = P++
else
   PXX_SUBDIR =
endif

# Use Automake's conditional make mechanism to control use of Performance Testing
if COMPILE_PERFORMANCE_TESTS
   PERFORMANCE_TESTS_SUBDIRS = BenchmarkBase
else
   PERFORMANCE_TESTS_SUBDIRS = 
endif

# Setup subdirectories
SUBDIRS = A++ $(PXX_SUBDIR) $(PERFORMANCE_TESTS_SUBDIRS)

# Only build a distributuion using the directories that were setup
# If not set Automake will correctly set this to a maximal set (directories: A++ P++ BenchmarkBase)
DIST_SUBDIRS = $(SUBDIRS)

## Generate fast compression while we test the configuration 
## using the expect script on remote machines
GZIP_ENV = --fast

## We don't have to include options since they will automatically be found in the
## the config.h file (read automatically).  We use the makedepend program here
## because it seems to be installed on all the machines that we have checked.
depend:
	cd A++; $(MAKE) depend;
if COMPILE_PXX_DIRECTORY
	cd P++; $(MAKE) depend;
endif

## Developer rule for building source code from internal common code templates
source:
	cd A++; $(MAKE) source;
if COMPILE_PXX_DIRECTORY
	cd P++; $(MAKE) source;
endif

##########################################################################
## More developer rules (suggestions and notes by Brian Gunney)
##     These rules are discussed more fully in the
##     A++P++/A++/config/Makefile.config.in
##########################################################################

## When making distribution, add some non-automake-controlled sources.
dist-hook:
	for i in config acmacros doc SCRIPTS STL-link sla-1.9.3; do	\
	  rm -rf $(distdir)/$$i	;\
	  cp -rp $(srcdir)/$$i $(distdir)	;\
	  rm -rf $(distdir)/$$i/CVS	;\
	done

#######################################################
## additional clean rule for removing machine.file in
## this directory (used by MPI).
#######################################################
distclean-local:
	rm -f machine.file

install-data-local:
	@echo "There is handy documentation located in the doc directory that make install does not move.  YOU SHOULD PUT IT SOMEWHERE."


## 
## Example of how to build man pages for PetSc, we might want to do the same.
## 
## # Builds man pages (LaTeX version)
## latexpages:
##         @-if [ "${MANSEC}" != "" ] ; then \
##         /home/gropp/bin/sun4/doc2lt     \
##           ${SOURCEC} ${SOURCEH} >> \
##                     ${PETSC_DIR}/docs/tex/rsum/rsum${MANSEC}.tex ; fi 
## 
## # Builds man pages (HTML version) in two stages
## # See rule for allwwwpages in ${PETSC_DIR}/makefile 
## #   1.) wwwpages_buildcite: builds the file manualpages.cit for hyperlinks
## #   2.) wwwpages: builds the html pages, complete with hyperlinks
## manualpages_buildcite:
##         @-if [ "${MANSEC}" != "" ] ; then \
##         /home/gropp/bin/sun4/doctext -html -indexdir ../man${MANSEC} \
##                 -index ${PETSC_DIR}/docs/manualpages/manualpages.cit \
##                 -mpath ${PETSC_DIR}/docs/manualpages/man${MANSEC} ${SOURCEC} ${SOURCEH}; \
## #
## #   You should have DOCTEXT_PATH point to /home/bsmith/petsc/docs/tex/doctext
## #
## manualpages:
##         @-if [ "${MANSEC}" != "" ] ; then \
##         /home/gropp/bin/sun4/doctext -html \
##                 -mpath ${PETSC_DIR}/docs/manualpages/man${MANSEC} -heading PETSc \
##                 -locdir ${LOCDIR} -mapref ${PETSC_DIR}/docs/manualpages/manualpages.cit \
##                 -defn ${PETSC_DIR}/docs/manualpages/sec/eop.${MANSEC} \
##                 ${SOURCEC} ${SOURCEH}; \
## 


# These are BTNG's rules and unless you are curious, you can ignore it.
padre-clean:
if COMPILE_PXX_DIRECTORY
	cd P++ && $(MAKE) padre-clean
endif

checks-clean:
	cd A++ && $(MAKE) checks-clean
if COMPILE_PXX_DIRECTORY
	cd P++ && $(MAKE) checks-clean
endif
# End BTNG's rules.

AUTOMAKE_OPTIONS = foreign







