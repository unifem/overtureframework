#
# Top level Makefile for cg solvers
#
# NOTE: 
# setenv CGBUILDPREFIX /some/other/dir will put the obj/lib/bin in /some/other/dir/{common,cgad...}/{lib,obj,dep,bin}
#

include config/make.dirs

all : libCommon cgad cgcns cgins cgasf cgsm cgmp 
#unitTests cgmx message

everything: all 

message : cgad cgcns cgins cgasf cgsm cgmp unitTests cgmx
	@echo "CG:INFO: type \"make check-quiet\" or \"make -j8 check-quiet\" or \"make check\" to run regression tests"	

libCommon:
	@mkdir -p $(CGBUILDPREFIX)/common/dep
	@cd common; $(MAKE) 

# advection-diffusion
cgad: libCommon
	@mkdir -p $(CGBUILDPREFIX)/ad/dep
	@cd ad;     $(MAKE) 

# all-speed compressible Navier-Stokes
cgasf: libCommon
	@mkdir -p $(CGBUILDPREFIX)/asf/dep
	@cd asf;  $(MAKE) 

# compressible Navier-Stokes
cgcns: libCommon
	@mkdir -p $(CGBUILDPREFIX)/cns/dep
	@cd cns;    $(MAKE) 

# incompressible Navier-Stokes
cgins: libCommon
	@mkdir -p $(CGBUILDPREFIX)/ins/dep
	@cd ins;    $(MAKE) 


# multi-physics solver
cgmp: cgins cgcns cgad libCommon
	@mkdir -p $(CGBUILDPREFIX)/mp/dep
	@cd mp;     $(MAKE) 

# Maxwell solver
cgmx: 
	@mkdir -p $(CGBUILDPREFIX)/mx/dep
	@cd mx;     $(MAKE) 

# Solid mechanics:
cgsm: libCommon
	@cd sm;     $(MAKE) 

# Unit tests
unitTests: libCommon
	@cd unitTests;     $(MAKE) 

# run regression tests:
check:
	-@cd unitTests/check;  $(MAKE)  check
	-@cd ad/check;  $(MAKE)  check
	-@cd asf/check; $(MAKE)  check
	-@cd cns/check; $(MAKE)  check
	-@cd ins/check; $(MAKE)  check
	-@cd sm/check;  $(MAKE)  check
	-@cd mx/check;  $(MAKE)  check
	-@cd mp/check;  $(MAKE)  check

check-unitTests-quiet:
	-@cd unitTests/check;  $(MAKE) -s  check-quiet
check-ad-quiet:
	-@cd ad/check;  $(MAKE) -s check-quiet
check-asf-quiet:
	-@cd asf/check; $(MAKE) -s  check-quiet
check-cns-quiet:
	-@cd cns/check; $(MAKE) -s  check-quiet
check-ins-quiet:
	-@cd ins/check; $(MAKE) -s  check-quiet
check-sm-quiet:
	-@cd sm/check;  $(MAKE) -s  check-quiet
check-mx-quiet:
	-@cd mx/check;  $(MAKE) -s  check-quiet
check-mp-quiet:
	-@cd mp/check;  $(MAKE) -s  check-quiet

# run regression tests with minimal output:  [ make -j8 check-quiet ]
check-quiet: check-unitTests-quiet \
             check-ad-quiet \
             check-asf-quiet \
             check-cns-quiet\
             check-ins-quiet \
             check-sm-quiet \
             check-mx-quiet \
             check-mp-quiet

# clean out obj and bin directories and temporary files.
clean:
	rm -f *~ */*~ */*/*~ */*/*/*~
	-@cd ad;            $(MAKE) clean
	-@cd asf;           $(MAKE) clean
	-@cd cns;           $(MAKE) clean
	-@cd common;        $(MAKE) clean
	-@cd common/moving; $(MAKE) clean
	-@cd ins;           $(MAKE) clean
	-@cd mp;            $(MAKE) clean
	-@cd mx;            $(MAKE) clean
	-@cd sm;            $(MAKE) clean
	-@cd user;          $(MAKE) clean
	-@cd unitTests;     $(MAKE) clean

