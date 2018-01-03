# This is just a dummy file for testing acmacros.
# $Id: Makefile.am,v 1.13 2002/01/28 20:08:50 gunney Exp $

show: show-acsubst show-config


# Rule to show configure substitutions
show-acsubst:
	@echo "========== Configure substitutions ==========";	\
	pattern=`echo $(SUBST_SHOW) | @SED@ 's/[ 	]\{1,\}/\|/g'`; \
	: echo "pattern is '$$pattern'"; \
	@EGREP@ "^($$pattern)[ 	]+=" Makefile | sort


# Rule to show configure macros
show-config:
	@echo "========== config.h macros ==========";	\
	pattern=`echo $(CONFIG_SHOW) | @SED@ 's/[ 	]\{1,\}/\|/g'`; \
	: echo "pattern is '$$pattern'"; \
	@EGREP@ "#(define|undef)[ 	]+($$pattern)" config.h

# Rule to force rerunning aclocal.
acloc:
	cd $(top_srcdir) && $(ACLOCAL)

# Remove .psrc links files.
distclean-local:
	$(btng_remove_auto_psrc_links)
