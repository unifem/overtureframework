#STL_HOME         = /home/bobbyp/STLport
#STL_Library =      $(STL_HOME)/stlport -I$(STL_HOME)/stlport/SC5
#STL_Library =      $(STL_HOME)/old_hp -I$(STL_HOME)/SC5
#STL_Library =      $(STL_HOME) -I$(STL_HOME)/SC5
#STL_Library =      /home/bobbyp/STL
STL_Library =      /home/chand/STL
APPLib=            $(APlusPlus)/lib
AMR_BOXLIB_DIR   = $(AMRPP_HOME)/boxlib
BOXLIB_INCLUDE   = $(AMR_BOXLIB_DIR)
BOXLIB_TEMPLATES = $(AMR_BOXLIB_DIR)
USE_LOCAL_BOXLIB = $(AMRPP_HOME)/LIBRARIES/boxlib.a
#USE_LOCAL_BOXLIB = 
PCSINFO_INCLUDE  = /home/bobbyp/overture_pcs


# CC VERSION
CC = CC
cc = cc

# PURIFY FLAGS
PFLAGS  = -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes
PURIFY = purify -best-effort $(PFLAGS)
PURIFY = 
PURIFY_OPTIONS = -I$(PDIR) -g $(LINK_FLAGS)

#INCLUDE = $(BOXLIB_INCLUDE) -I$(Overture)/include -I$(STL_Library) -instances=global -I$(APlusPlus)/include -I$(PCSINFO_INCLUDE) -DBL_AUTO_INSTANTIATE

INCLUDE = $(BOXLIB_INCLUDE) -I$(Overture)/include -I$(BOXLIB_INCLUDE) -I$(STL_Library) -I$(APlusPlus)/include -I$(PCSINFO_INCLUDE) -DBL_AUTO_INSTANTIATE
#TEMPLATES = -pta -ptrSERIAL_REPOSITORY -ptr../SERIAL_REPOSITORY -ptr$(BOXLIB_TEMPLATES) -I$(BOXLIB_INCLUDE)
#TEMPLATES = -pto -pta -ptrSERIAL_REPOSITORY -ptr../SERIAL_REPOSITORY -ptr$(BOXLIB_TEMPLATES) -I$(BOXLIB_INCLUDE)
TEMPLATES = -pto
CCFLAGS = -g -PIC -cg92 -I. -I$(INCLUDE) $(TEMPLATES)
LIBS = $(USE_LOCAL_BOXLIB) -L$(Overture)/lib -lOverture -lOverture_static -L$(APPLib) -lApp -lApp_static -L$(PCSINFO_INCLUDE) -lPCSLib -I$(STL_Library)
###### CC VERSION ####
GLIBS=  -L$(HDF)/lib -lmfhdf -ldf -ljpeg -lz   \
	-L$(OpenGL)/lib -lGLw -lMesaGLU -lMesaGL  \
	-L$(MOTIF)/lib -lXm -lXt -lXmu -lXi -lXext -lX11 -lnsl -lm -lF77 -lM77 -lV77 -lsunmath

.SUFFIXES: .C .o

ALL_SRCS = \
     test0.C \
     test1.C \
     test2.C \
     test3.C \
     test4.C \
     test5.C \
     test6.C \
     test7.C \
     test8.C \
     testCellCentredBoxes.C

#ALL_OBJS = $(ALL_SRCS:.C=.o)

.C.o:
	$(CC) $(CCFLAGS) -o $@ -c $*.C

all = test0 test1 test2 test3 test4 test5 test6 test7 

test0 = test0.o 
test0: $(test0) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test0 $(test0) $(LIBS) $(GLIBS)

test1 = test1.o
test1: $(test1) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test1 $(test1) \
        $(LIBS) $(GLIBS)

test2 = test2.o
test2: $(test2) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test2 $(test2) \
        $(LIBS) $(GLIBS)

test3 = test3.o
test3: $(test3) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test3 $(test3) \
        $(LIBS) $(GLIBS)

test4 = test4.o
test4: $(test4) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test4 $(test4) \
        $(LIBS) $(GLIBS)

test5 = test5.o
test5: $(test5) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test5 $(test5) \
        $(LIBS) $(GLIBS)

test6 = test6.o
test6: $(test6) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test6 $(test6) \
        $(LIBS) $(GLIBS)

test7 = test7.o
test7: $(test7) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o test7 $(test7) \
        $(LIBS) $(GLIBS)

testCellCentredBoxes = testCellCentredBoxes.o
testCellCentredBoxes: $(testCellCentredBoxes) $(PCSINFO_INCLUDE)/libPCSLib.a
	$(PURIFY) $(CC) $(CCFLAGS) -o testCellCentredBoxes $(testCellCentredBoxes) \
        $(LIBS) $(GLIBS)

clean:
	rm -rf *.o *~ test0 test1 test2 test3 test4 test5 test6 test7 test8
