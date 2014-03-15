include ../config/Makefile.$(VENDOR)

# over-ride defaults here
# PURIFY  = purify $(PFLAGS) 

current = .
op = $(current)/../op
VPATH = .:$(op)


all = tg
all: $(all);

GRIDS= $(Overture)/MappedGrid.o $(Overture)/GenericGridCollection.o \
       $(Overture)/GridCollection.o $(Overture)/CompositeGrid.o \
       $(Overture)/MultigridCompositeGrid.o

GRIDS= $(Overture)/GRID.o
GRIDS=

grid: 
	latex grid
	makeindex grid.idx
	latex grid
	dvips -P pdf -G0 -o grid.ps grid.dvi  
	ps2pdf grid.ps
	cp grid.pdf $(WEBPAGE)/documentation

gridGuide: 
	latex gridGuide
	makeindex gridGuide.idx
	latex gridGuide
	dvips -P pdf -G0 -o gridGuide.ps gridGuide.dvi  
	ps2pdf gridGuide.ps
	cp gridGuide.pdf $(WEBPAGE)/documentation

gridhtml: 
	lh.p grid.tex
	latex gridHTML
#	bibtex gridHTML
	makeindex gridHTML.idx
	latex gridHTML
	latex2html -local_icons gridHTML

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps


# pstricks.pro etc. must appear in the gridGuideHTML directory **
gridGuidehtml: 
	lh.p gridGuide.tex
	latex gridGuideHTML
#	bibtex gridGuideHTML
	makeindex gridGuideHTML.idx
	latex gridGuideHTML
	latex2html -local_icons gridGuideHTML
	cp pstricks.pro gridGuideHTML
	cp pst-node.pro gridGuideHTML

doc: grid gridGuide

dochtml: gridhtml gridGuidehtml
	latex grid
	makeindex grid.idx
	latex grid
	latex grid
	dvips -P pdf -G0 -o grid.ps grid.dvi  
	ps2pdf grid.ps
	rm -rf $(WEBPAGE)/documentation/gridHTML
	mkdir $(WEBPAGE)/documentation/gridHTML
	cp gridHTML/*.* $(WEBPAGE)/documentation/gridHTML
#	rm -rf gridGuideHTML
	latex gridGuide
	makeindex gridGuide.idx
	latex gridGuide
	latex gridGuide
	dvips -P pdf -G0 -o gridGuide.ps gridGuide.dvi  
	ps2pdf gridGuide.ps
	rm -rf $(WEBPAGE)/documentation/gridGuideHTML
	mkdir $(WEBPAGE)/documentation/gridGuideHTML
	cp gridGuideHTML/*.* $(WEBPAGE)/documentation/gridGuideHTML
#	rm -rf gridGuideHTML
	mv grid.pdf $(WEBPAGE)/documentation
	mv gridGuide.pdf $(WEBPAGE)/documentation


hybTestO = readhybrid.o CompositeGrid.o
hybTest: $(hybTestO)
	$(CC) $(CCFLAGS) -o $@ $(hybTestO) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

MappedGridGeometry2.o: MappedGridGeometry2.C
	 $(CC) $(CCFLAGSG) -c MappedGridGeometry2.C
#	 $(CC) $(CCFLAGSF) -c MappedGridGeometry2.C

# test for MappedGrid -- test vertexBoundaryNormal and boundingBox etc. in parallel
tg = tg.o ../mapping/Mapping.o ../mapping/DistributedInverse.o ../mapping/stencilWalk.o
tg = tg.o MappedGrid.o MappedGridGeometry2.o ../mapping/Mapping.o 
tg = tg.o MappedGrid.o MappedGridGeometry.o MappedGridGeometry1.o MappedGridGeometry2.o UnstructuredGridGeometry.o
tg = tg.o MappedGrid.o MappedGridGeometry2.o
tg: $(tg)
	$(CC) $(CCFLAGS) -o tg \
        $(tg) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS)

# test for GridCollection
tgc = tgc.o GridCollection.o
tgc = tgc.o CompositeGrid.o
tgc: $(tgc)
	$(CC) $(CCFLAGS) -o tgc $(tgc) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS)

tadd = tadd.o GridCollection.o
tadd: $(tadd)
	$(CC) $(CCFLAGS) -o tadd \
        $(tadd) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

tadd3d = tadd3d.o GridCollection.o MappedGrid.o
tadd3d: $(tadd3d)
	$(CC) $(CCFLAGS) -o tadd3d \
        $(tadd3d) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

amr = amr.o GridCollection.o GenericGridCollection.o MappedGrid.o GenericGrid.o ../mapping/RestrictionMapping.o \
          ../mapping/ReparameterizationTransform.o
amr: $(amr)
	$(CC) $(CCFLAGS) -o amr \
        $(amr) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

GridCollection.o: GridCollection.C
	 $(CC) $(CCFLAGS) -c GridCollection.C

GenericGridCollection.o: GenericGridCollection.C
	 $(CC) $(CCFLAGS) -c GenericGridCollection.C
  
tamr = tamr.o CompositeGrid.o GridCollection.o GenericGridCollection.o 
tamr: $(tamr)
	$(CC) $(CCFLAGS) -o tamr \
        $(tamr) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77


# test for building domains (sub-collection grids, for EquationDomains)
tdomain = tdomain.o CompositeGrid.o GridCollection.o GenericGridCollection.o
tdomain: $(tdomain)
	$(CC) $(CCFLAGS) -o tdomain \
        $(tdomain) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS)



MyFunction.o: MyFunction.h
BoundaryAdjustment.o: BoundaryAdjustment.h

SRC = *.C

# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CFLAGS) -- $(SRC)

# include the dependencies here
include depend


clean:
	rm -rf *.o tg tadd tamr tdomain 

.PRECIOUS:
