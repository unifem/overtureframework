include ../config/Makefile.$(VENDOR)

# over-ride defaults here
# PURIFY  = purify $(PFLAGS) 
# CLIBS = $(CLIBS2)
# Use the .a Overture library, libOvertureBig.a
# CLIBS = $(CLIBS3)

# CC= insure

current = .
op = $(current)/../op
VPATH = .:$(op)

BOXLIB_DEFINES = -DBL_SPACEDIM=3 -DBL_USE_FLOAT -DBL_ARCH_IEEE -DBL_FORT_USE_UNDERSCORE -DBL_BUG_DEFARG -DBL_LANG_CC

CC_FLAGS = -fPIC  -fpermissive -fmessage-length=200 $(BOXLIB_DEFINES)

CCFLAGS3 = -I. -fPIC  -fpermissive -I/home/henshaw/Overture.g/include  -DINTEL86 -DUSE_MESA -I/home/henshaw2/A++P++-3.0.3/A++P++-0.7.9d/A++/install/include -I/usr/casc/overture/OpenGL/Mesa-4.0.3.intel.gcc/include -I/usr/X11R6/include  -DBL_USE_DOUBLE -DBL_Solaris  -g -DBL_SPACEDIM=3 -DBL_USE_FLOAT -DBL_ARCH_IEEE -DBL_FORT_USE_UNDERSCORE -DBL_BUG_DEFARG -DBL_LANG_CC

# .SUFFIXES:
# .SUFFIXES:.C .o .f .o
# .C.o:; $(CC) -I. $(CCFLAGS) $(BOXLIB_DEFINES) -c $*.C
# .f.o:; $(FC) $(FFLAGS) -c $*.f
# 
default:      boxlib_date tb

boxlib= AliasedDPtr.o  \
	Box.o  \
	BoxList.o  \
	FabConv.o  \
	Pointers.o  \
        ArithFab.o  \
	BoxArray.o  \
	IndexType.o  \
	OrderedFab.o  \
	BoxAssoc.o  \
	FArrayBox.o  \
	IntVect.o  \
	Orientation.o  \
        BArena.o  \
	BoxDomain.o  \
	FPC.o  \
	Utility.o  \
	BoxLib.o  \
	MultiFab.o  \
	ParmParse.o  \
	aString.o


boxlib_date:$(boxlib)
	  touch $@

# boxlib = -L$(Overture)/lib -lOverture_static
tb = tb.o
tb: $(tb)
	$(PURIFY) $(CC) $(CCFLAGS) $(PURIFY_OPTIONS) -o tb \
        $(tb) $(boxlib)  $(FLIBS) $(GLIBS) 

clean:
	rm -f *.o tb

.PRECIOUS:boxlib_date
