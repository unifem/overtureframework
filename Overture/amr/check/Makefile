include ../../config/Makefile.$(VENDOR)

# over-ride defaults
# === use these for opt ===== (setenv compile opt)
ifeq ($(compile),opt)
  CCFLAGS= $(CCFLAGSO)
  CCFLAGSG= $(CCFLAGSO)
endif

current = .
# ogen = $(current)/../../ogen
# VPATH = $(ogen)

linkFiles:;
	ln -sf ../../ogen/refine.C .
	ln -sf ../../ogen/checkInterpolation.C .

all:      refine

DIR=`pwd`
dir:; @echo "pwd = $(DIR)"

# These files we build dependency info for:
SRC = refine.C checkInterpolation.C


refine=   refine.o checkInterpolation.o 
refine:   $(refine); $(CC) $(CFLAGS) -o $@ $(refine) $(CLIBS)  $(FLIBS)  $(GLIBS)


# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

clean: 
	rm -f *.o refine 

precious:
