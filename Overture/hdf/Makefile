# include ../config/Makefile.$(VENDOR)
include ../config/Makefile.intel

# over-ride defaults here
# PURIFY  = purify $(PFLAGS) 
# FFLAGSO= $(FFLAGS)
# FFLAGSO= $(FFLAGSO4)
current = .
grid = $(current)/../grid
gf = $(current)/../gf
mapping = $(current)/../mapping

VPATH = $(grid):$(gf):$(mapping)


db:
	extract.p GenericDataBase.C HDF_DataBase.C 
	latex db
	dvips  -P pdf -G0 -o db.ps db.dvi 
	ps2pdf db.ps
	cp db.pdf $(WEBPAGE)/documentation

dbhtml: 
	lh.p db.tex
	latex dbHTML
#	bibtex dbHTML
#	makeindex dbHTML.idx
	latex dbHTML
	latex2html -local_icons dbHTML
	rm -rf $(WEBPAGE)/documentation/dbHTML
	mkdir $(WEBPAGE)/documentation/dbHTML
	cp dbHTML/*.* $(WEBPAGE)/documentation/dbHTML
	rm -rf dbHTML

doc:  db dbhtml


all = thdf1
all: $(all);


# --- Kyle's DataBase ----

# cp $cg/common/dataBase/{dbAccess.C,dbFunctions.f,kk_ptr.cc,DBase.hh,kk_defines.hh,kk_ptr.hh} .
# tux291.llnl.gov{henshaw}38: cp $cg/common/dataBase/{dbAccess.bC} .
# tux291.llnl.gov{henshaw}39: cp $cg/common/dataBase/{tdb.C,testdb.cc} .

dbAccess.C: dbAccess.bC; bpp -quiet -clean dbAccess.bC

tdb = tdb.o dbAccess.o dbFunctions.o kk_ptr.o ut.o
tdb: $(tdb); $(CC) $(CCFLAGS) -o tdb $(tdb) $(CLIBS) $(FLIBS) $(GLIBS)

testdb = testdb.o kk_ptr.o 
testdb: $(testdb); $(CC) $(CCFLAGS) -o testdb $(tdb) $(CLIBS) $(FLIBS) $(GLIBS)




HDF_DataBase.C : HDF_DataBase.bC; bpp HDF_DataBase.bC
HDF5_DataBase.C : HDF5_DataBase.bC; bpp HDF5_DataBase.bC

boxlibFiles = aString.o 
boxlibFiles = 

# dbFiles = HDF_DataBase.o GenericDataBase.o DataBaseBuffer.o ListOfHDF_DataBaseRCData.o
dbFiles = HDF_DataBase.o GenericDataBase.o DataBaseBuffer.o 

# Main test routine: (also test gq)
thdf1 = $(dbFiles) thdf1.o $(boxlibFiles)
thdf1: $(thdf1); $(CC) $(CCFLAGS) -o thdf1 $(thdf1) $(CLIBS) $(FLIBS) $(GLIBS) 

# Test writing a large distributed array
thdf2 = $(dbFiles) thdf2.o $(boxlibFiles)
thdf2: $(thdf2); $(CC) $(CCFLAGS) -o thdf2 $(thdf2) $(CLIBS) $(FLIBS) $(GLIBS) 


# Jeff's bug:
bug1 = $(dbFiles) bug1.o $(boxlibFiles)
bug1: $(bug1); $(CC) $(CCFLAGS) -o bug1 $(bug1) $(CLIBS) $(FLIBS) $(GLIBS) 



dbex1 = $(dbFiles) dbex1.o $(boxlibFiles)
dbex1: $(dbex1); $(CC) $(CCFLAGS) -o dbex1 $(dbex1) $(CLIBS) $(FLIBS) $(GLIBS) 

dbex2 =  $(dbFiles) dbex2.o $(boxlibFiles)
dbex2: $(dbex2); $(CC) $(CCFLAGS) -o dbex2 $(dbex2) $(CLIBS) $(FLIBS) $(GLIBS) 

dbex2a =  $(dbFiles) dbex2a.o $(boxlibFiles)
dbex2a: $(dbex2a); $(CC) $(CCFLAGS) -o dbex2a $(dbex2a)   $(CLIBS) $(GLIBS)  

dbex3 =  $(dbFiles) dbex3.o $(boxlibFiles)
dbex3: $(dbex3); $(CC) $(CCFLAGS) -o dbex3 $(dbex3) $(CLIBS) $(FLIBS) $(GLIBS) 

thdf =  $(dbFiles) thdf.o $(boxlibFiles)
thdf: $(thdf) 
	$(CC) $(CCFLAGS) -o thdf $(thdf)   $(CLIBS) $(GLIBS) 

bug = bug.o  $(dbFiles) $(boxlibFiles)
bug: $(bug) 
	$(CC) $(CCFLAGS) -o bug $(bug)    $(CLIBS) $(GLIBS) 

ts = ts.o  $(dbFiles) $(boxlibFiles)
ts: $(ts) 
	$(CC) $(CCFLAGS) -o ts $(ts)    $(CLIBS) $(GLIBS) 

saveMap = saveMap.o
saveMap: $(saveMap)
	$(CC) $(CCFLAGS) -o saveMap $(saveMap)   $(CLIBS) $(FLIBS) $(GLIBS)

HDF5_DataBase.o: HDF5_DataBase.C
	 $(CC) $(CCFLAGS) -c HDF5_DataBase.C

#  --- test hdf5 ---
boxlibFiles5 = aString.o
CLIBS5=   $(AppLibraries) $(FORTRAN_LIBS)
dbFiles5 = HDF5_DataBase.o GenericDataBase.o DataBaseBuffer.o
thdf5 =  thdf5.o $(dbFiles5) $(boxlibFiles5) tempFiles.o
thdf5: $(thdf5) 
	$(CC) $(CCFLAGS) -o thdf5 $(thdf5)  $(CLIBS) $(FLIBS) $(GLIBS) 
# 	$(CC) $(CCFLAGS) -o thdf5 $(thdf5) -Wl,-rpath,$(HDF)/lib -L$(HDF)/lib -lhdf5 -lz $(CLIBS5)

# hdf5 collective read/write example
# ph5example.c


CCFLAGS5 = $(CCFLAGS) -I$(MPI_ROOT)/include

MPI_LIBS = -Wl,-rpath,/usr/global/tools/hdf5/mpich_yana/lib -L/usr/global/tools/hdf5/mpich_yana/lib -lmpich 


# zeus: MPI_LIBS = -Wl,-rpath,$(MPI_ROOT)/lib/shared -L$(MPI_ROOT)/lib/shared -lmpich 


CCFLAGS5 = -I$(MPI_ROOT)/include -I$(HDF)/include
ph5example = ph5example.C
ph5example: $(ph5example) 
	$(CC) $(CCFLAGS5) -o ph5example $(ph5example) -Wl,-rpath,$(HDF)/lib -L$(HDF)/lib -lhdf5 -lz $(MPI_LIBS) 
# 	$(CC) $(CCFLAGS5) -o ph5example $(ph5example) -Wl,-rpath,$(HDF)/lib -L$(HDF)/lib -lhdf5 -lz $(AppLibraries) $(FORTRAN_LIBS)


# read a CompositeGrid
HDF_DataBase.o : HDF5_DataBase.h
gq = gq.o HDF_DataBase.o GenericDataBase.o
gq: $(gq) 
	$(CC) $(CCFLAGS) -o gq $(gq) $(CLIBS) $(FLIBS) $(GLIBS)

gf = gf.o
gf: $(gf)
	$(CC) $(CCFLAGS) -o gf $(gf)  $(CLIBS)  $(FLIBS) $(GLIBS)

DB_FILES_H = HDF_DataBase.h GenericDataBase.h DataBaseBuffer.h

thdf.o : $(DB_FILES_H)
thdf.o :  $(DB_FILES_H)
thdf.o :  $(DB_FILES_H)
HDF_DataBase.o :  $(DB_FILES_H)
DataBaseBuffer.o : DataBaseBuffer.h
GenericDataBase.o : GenericDataBase.h DataBaseBuffer.h
ts.o : $(DB_FILES_H)
ListOfHDF_DataBaseRCData.o : HDF_DataBase.h GenericDataBase.h


vg_ex1 = vg_ex1.o
vg_ex1: $(vg_ex1) 
	$(cc) $(CFLAGS) -o vg_ex1 $(vg_ex1) $(CLIBS)

vg_ex2 = vg_ex2.o
vg_ex2: $(vg_ex2) 
	$(cc) $(CFLAGS) -o vg_ex2 $(vg_ex2) $(CLIBS)

vg_ex3 = vg_ex3.o
vg_ex3: $(vg_ex3) 
	$(cc) $(CFLAGS) -o vg_ex3 $(vg_ex3) $(CLIBS)

vg_ex4 = vg_ex4.o
vg_ex4: $(vg_ex4) 
	$(cc) $(CFLAGS) -o vg_ex4 $(vg_ex4) $(CLIBS)


sd_ex2 = sd_ex2.o
sd_ex2: $(sd_ex2) 
	$(cc) $(CFLAGS) -o sd_ex2 $(sd_ex2) $(CLIBS)

SRC = HDF_DataBase.C HDF5_DataBase.C GenericDataBase.C DataBaseBuffer.C ListOfHDF_DataBaseRCData.C \
      thdf1.C thdf5.C gf.C

# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend


clean:  
	rm -f *.o sd_ex2 sd_ex2 thdf5

.PRECIOUS: 
