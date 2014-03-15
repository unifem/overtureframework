include ../config/Makefile.intel
# include ../config/Makefile.$(VENDOR)

# current = ${shell pwd}
current = .
mapping = $(current)/../mapping
ogshow = $(current)/../ogshow
hype=  $(current)/../hype
ugen=  $(current)/../ugen

VPATH = $(mapping):$(ogshow):$(hype):$(ugen)

SILO = /home/chand/Linux

unstructuredDoc: $(docIncludeFiles)
	latex unstructured
#	makeindex unstructured.idx
	latex unstructured
	dvips -P pdf -G0 -o unstructured.ps unstructured.dvi
	ps2pdf unstructured.ps
	cp unstructured.pdf $(WEBPAGE)/documentation

smeshDoc: 
	latex smesh
	bibtex smesh
	latex smesh
	dvips -P pdf -G0 -o smesh.ps smesh.dvi
	ps2pdf smesh.ps
	cp smesh.pdf $(WEBPAGE)/documentation

unstructuredhtml: $(docIncludeFiles)
	lh.p unstructured.tex
	latex unstructuredHTML
#	bibtex unstructuredHTML
#	makeindex unstructuredHTML.idx
	latex unstructuredHTML
	latex2html -local_icons unstructuredHTML
	rm -rf $(WEBPAGE)/documentation/unstructuredHTML
	mkdir $(WEBPAGE)/documentation/unstructuredHTML
	cp unstructuredHTML/*.* $(WEBPAGE)/documentation/unstructuredHTML
	rm -rf unstructuredHTML

doc: unstructuredDoc smeshDoc


all = test
all: $(all)

# AdvancingFront.o: AdvancingFront.C
# 	 $(CC) $(CCFLAGSO) -c AdvancingFront.C

test= Ugen.o
test: $(test) 
	$(CC) $(CCFLAGS) -o $@ $(test) $(CLIBS) $(FLIBS) $(GLIBS)

adt.o: adt.C
	$(CC) $(CCFLAGSF) -c adt.C
#	$(CC) $(CCFLAGSTO) -c adt.C
#	$(CC) $(CCFLAGS) -c adt.C

adt = adt.o 
adt: $(adt) 
	$(CC) $(CCFLAGS) -o $@ $(adt) $(CLIBS) $(FLIBS) $(GLIBS) 

adt2.o: adt2.C
	$(CC) $(CCFLAGSF) -c adt2.C

GeometricADT3dInt.o: GeometricADT3dInt.C
	$(CC) $(CCFLAGSF) -c GeometricADT3dInt.C


adt2 = adt2.o GeometricADT3dInt.o 
adt2: $(adt2) 
	$(CC) $(CCFLAGS) -o $@ $(adt2) $(CLIBS) $(FLIBS) $(GLIBS)

ArraySimpleTest = ArraySimpleTest.o 
ArraySimpleTest: $(ArraySimpleTest) 
	$(CC) $(CCFLAGSF) -o $@ $(ArraySimpleTest)  $(FLIBS) $(GLIBS)

quality = MeshQuality.o
quality: $(quality)

# Examplw to show the advancement of a front in 2D
fgen= fgen.o AdvancingFront.o
fgen: $(fgen) 
	$(CC) $(CCFLAGS) -o $@ $(fgen) $(CLIBS) $(FLIBS) $(GLIBS)

ov_hot.o: ov_hot.C
	$(CC) $(CCFLAGS) -I$(SILO)/include -o $@ -c $<

ov_hot= ov_hot.o 
ov_hot: $(ov_hot) 
	$(CC) $(CCFLAGS) -o $@ $(ov_hot) $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a -L$(SILO)/lib -lsilo $(GLIBS)

smesh.o: smesh.cc
	$(CC) $(CCFLAGS) -o $@ -c $<

smesh= simpleMesh.o smesh.o 
smesh: $(smesh) 
	$(CC) $(CCFLAGS) -o $@ $(smesh) $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a $(GLIBS)


verifymesh= verifymesh.o 
verifymesh: $(verifymesh) 
	$(CC) $(CCFLAGS) -o $@ $(verifymesh) $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a -L$(SILO)/lib -lsilo $(GLIBS)

convertUM= convertUM.o 
convertUM: $(convertUM) 
	$(CC) $(CCFLAGS) -o $@ $(convertUM) $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a $(GLIBS)

TriangleSource.o: TriangleSource.c
	$(cc) $(CFLAGS) -o $@ -c $<

stitcherm.o: stitcher.C
	$(CC) $(CCFLAGS) -DSTITCHER_MAIN -o $@ -c $<

stitcher.o: stitcher.C
	$(CC) -DSTITCHER_MAIN $(CCFLAGS) -c stitcher.C

stitcher= stitcher.o Ugen.o SurfaceStitcher.o ../gf/InterpolatePoints.o
stitcher= stitcher.o SurfaceStitcher.o
stitcher: $(stitcher) 
	$(CC) $(CCFLAGS) -o $@ $(stitcher) $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a $(GLIBS)

CompositeGridHybridConnectivity.o: CompositeGridHybridConnectivity.C
	$(CC) $(CCFLAGS) -o $@ -c $<
clean:
	rm -f *.o test adt adt2 fgen ov_hot stitcher
	rm -rf Templates.DB


SRC = *.C


#		//
#		// DEPENDENCIES
#		//

# to remake the dependencies: make dep
dep:;
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend


.PRECIOUS:$(MAPLIB)

