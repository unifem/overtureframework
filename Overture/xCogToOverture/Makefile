Include=  $(Overture)/include
Lib=      $(Overture)/lib
CC=       CC
CFLAGS=   $(FLAGS) -g $(OverturePrecision) -I$(Include) -I$(A++)/include -I$(HDF)/include -I$(OpenGL)/include -I$(MOTIF)/include 
CFLAGS2=  $(FLAGS) -g -fast -O2 -ptr$(Lib) -I$(Include) -I$(A++)/include -I$(HDF)/include -I$(OpenGL)/include -I$(MOTIF)/include -I$(X11)/include
XLIBS=    -L$(MOTIF)/lib -R$(MOTIF)/lib -lXm -lXt -lXmu -lXi -lXext -lX11 -lm
ALIBS=    -L$(A++) -R$(A++) -lApp -lApp_static
FLIBS=    -R/opt/SUNWspro/lib -lF77 -lM77 -lV77 -lsunmath
GLIBS=    -L$(OpenGL)/lib -R$(OpenGL)/lib -lMesaGLU -lMesaGL -lGLw $(XLIBS)
HLIBS=    -L$(HDF)/lib -R$(HDF)/lib -lmfhdf -ldf -ljpeg -lz
OLIBS=    -L$(Lib) -R$(Lib) -lOverture -lOverture_static $(ALIBS) $(HLIBS) $(FLIBS) $(GLIBS)

PFLAGS=   -best-effort -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -recursion-depth-limit=200000
PDIR=     `purify -print-home-dir`
PSTUBS=   $(PDIR)/purify_stubs.a
PURIFY_OPTIONS=-I$(PDIR)
PURIFY=   purify $(PFLAGS)

.SUFFIXES:
.SUFFIXES:.C .c .o
.C.o:;    $(CC) $(CFLAGS) -DNO_REAL -c $*.C
.c.o:;    $(CC) $(CFLAGS) -DNO_REAL -c $*.c

all:      xCogToCG

# Geoff's original version
# xcogToCG= xcogToCG.o c_array.o hdf_stuff.o
# xcogToCG: $(xcogToCG); $(CC) $(CFLAGS) -o $@ $(xcogToCG) $(OLIBS)

# New version using a function call:
xCogToCG= xCogToCGDriver.o xCogToOverture.o c_array.o hdf_stuff.o CompositeGrid.o \checkOverlappingGrid.o \
          ../gf/conversion.o
xCogToCG= xCogToCGDriver.o xCogToOverture.o c_array.o hdf_stuff.o ../mapping/DataPointMapping.o
xCogToCG= xCogToCGDriver.o xCogToOverture.o c_array.o hdf_stuff.o 
xCogToCG: $(xCogToCG); $(CC) $(CFLAGS) -o $@ $(xCogToCG) $(OLIBS)

clean:; rm -f *.o xCogToCG



