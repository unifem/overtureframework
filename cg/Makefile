#
# Top level Makefile for cg solvers
#
# NOTE: 
# setenv CGBUILDPREFIX /some/other/dir will put the obj/lib/bin in /some/other/dir/{common,cgad...}/{lib,obj,dep,bin}
#

all : libCommon cgad cgcns cgins cgasf cgsm cgmp message

everything: all cgmx 

message : cgad cgcns cgins cgasf cgsm cgmp
	@echo "CG:INFO: type \"make check-quiet\" or \"make check\" to run regression tests"	

libCommon:
	@cd common; $(MAKE) 

# advection-diffusion
cgad: libCommon
	@cd ad;     $(MAKE) 

# all-speed compressible Navier-Stokes
cgasf: libCommon
	@cd asf;  $(MAKE) 

# compressible Navier-Stokes
cgcns: libCommon
	@cd cns;    $(MAKE) 

# incompressible Navier-Stokes
cgins: libCommon
	@cd ins;    $(MAKE) 


# multi-physics solver
cgmp: cgins cgcns cgad libCommon
	@cd mp;     $(MAKE) 

# Maxwell solver
cgmx: 
	@cd mx;     $(MAKE) 

# Solid mechanics:
cgsm: libCommon
	@cd sm;     $(MAKE) 

# run regression tests:
check:
	-@cd ad/check;  $(MAKE)  check
	-@cd asf/check; $(MAKE)  check
	-@cd cns/check; $(MAKE)  check
	-@cd ins/check; $(MAKE)  check
	-@cd sm/check;  $(MAKE)  check
	-@cd mp/check;  $(MAKE)  check

# run regression tests with minimal output:
check-quiet:
	-@cd ad/check;  $(MAKE) -s check-quiet
	-@cd asf/check; $(MAKE) -s  check-quiet
	-@cd cns/check; $(MAKE) -s  check-quiet
	-@cd ins/check; $(MAKE) -s  check-quiet
	-@cd sm/check;  $(MAKE) -s  check-quiet
	-@cd mp/check;  $(MAKE) -s  check-quiet

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

