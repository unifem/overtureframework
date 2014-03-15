# 
# This Makefile was created by the configure script using
# the files config/MakeDefs.sun5 and FiniteVolumeOperators/Makefile.in 
# 
#
# MakeDefs.sun5 : definitions for sun5 (solaris)
#
cc = cc
CC = CC
FC = f77

# these next flags are compiler dependent
cc_FLAGS = -KPIC -Xa -DSHM 
CC_FLAGS = -cg92 -PIC 
FF_FLAGS = -cg92 -PIC 

A++ = $(APlusPlus) 
# AppLibraries = -L$(APlusPlus) -lA++ -lA++_static 
AppLibraries = -L$(APlusPlus)/lib -lApp -lApp_static 

Include=  ../include 
Lib=      ../lib 
LibOverture= -lOverture -lOverture_static 

BOXLIB_DEFS = -DBL_USE_DOUBLE -DBL_Solaris   

C_OPTIMIZATION_FLAG = -g
CPP_OPTIMIZATION_FLAG = -g
FORTRAN_OPTIMIZATION_FLAG = -g

CFLAGS = $(cc_FLAGS)  $(C_OPTIMIZATION_FLAG) $(FLAGS) -I$(XLIBS)/include

CCFLAGS= $(CC_FLAGS)  $(CPP_OPTIMIZATION_FLAG) $(FLAGS) -DUSE_MESA -I$(Include) -I$(APlusPlus)/include -I$(OpenGL)/include -I$(XLIBS)/include $(BOXLIB_DEFS) 

# things that are always compile with optimization use this flag
CCFLAGSO= $(CC_FLAGS)  -O $(FLAGS) -DUSE_MESA -I$(Include) -I$(APlusPlus)/include -I$(OpenGL)/include -I$(XLIBS)/include $(BOXLIB_DEFS) 

# things that need to be compiled with no optimization use the following
CCFLAGSG= $(CC_FLAGS)  -g $(FLAGS) -DUSE_MESA -I$(Include) -I$(APlusPlus)/include -I$(OpenGL)/include -I$(XLIBS)/include $(BOXLIB_DEFS) 


FFLAGS=  $(FF_FLAGS)  -r8 -i4 $(FORTRAN_OPTIMIZATION_FLAG) $(FLAGS) -I$(Include)
# single precision always: (but keep DOUBLE definition)
FFLAGSS=   $(FF_FLAGS) $(FORTRAN OPTIMIZATION_FLAG) $(FLAGS) -I$(Include) 


FORTRAN_LIBS = -lF77 -lM77 -lV77 -lnsl -lsunmath   
#gcc FORTRAN_LIBS =-lnsl -lg2c
LIB_TARGETS= lib_date libso_date 
LIB_DEPENDENCIES= ../lib/lib_date 
TEMPLATES= ../boxlib/Templates.DB/*.o 
# CC V5.0 puts templates in SunWS_cache/*/*.o
#CC5 TEMPLATES=
ARFLAGS = ruv  

LD = $(CC)
#gcc LD = gcc

RANLIB = true 
SOFLAGS = -G
OpenGLibs =-lMesaGL -lMesaGLU -lGLw 
GLIBS=   -L$(OpenGL)/lib $(OpenGLibs) -L$(HDF)/lib -lmfhdf -ldf -ljpeg -lz -L$(MOTIF)/lib -lXm -L$(XLIBS)/lib -lXt -lXmu -lXi -lXext -lX11 -lm 
#
#


.SUFFIXES:
.SUFFIXES:.C .o .f .o
.C.o:; $(CC) $(CCFLAGS) -c $*.C
.f.o:; $(FC) $(FFLAGS) -c $*.f

default:      FVO_date

Fvoobjs=  \
 CompositeGridFiniteVolumeOperators.o \
 GridCollectionFiniteVolumeOperators.o \
 MappedGridFiniteVolumeOperators.o \
 Display.o \
 Projection.o \
 setEllipticSolverParameter.o \
 makeDebugPlots.o \
 testUtils.o \
 TwilightZoneWizard.o \
 getDefaultIndex.o \
 createGeometry.o \
 average.o \
 differences.o \
 contravariantVelocity.o \
 cellsToFaces.o \
 divergence.o \
 gradient.o \
 convectiveDerivative.o \
 vorticity.o \
 laplacianCoeff.o \
 laplacian.o \
 inputErrorHandling.o \
 boundaryConditions.o \
 boundaryConditionCoefficients.o \
 derivatives.o \
 derivativeCoefficients.o \
 inputErrorHandling.o \
 defaultMappedGridFunctions.o 

Mgobjs=         GenericGridMotion.o MatrixTransformGridMotion.o \
		GenericGridMotionParameters.o MatrixTransformGridMotionParameters.o \
		DynamicMovingGrids.o MatrixTransformMotionFunction.o

FVO_date:$(Fvoobjs) $(Mgobjs)
	  touch $@

.PRECIOUS:$(FVO_date) FVO_date



