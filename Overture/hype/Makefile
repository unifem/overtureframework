include ../config/Makefile.$(VENDOR)

# over-ride defaults
# CCFLAGS= $(CCFLAGSO)
# CCFLAGS)= $(CCFLAGSG)
FFLAGSO = $(FFLAGSG)

current = .
mapping = $(current)/../mapping
ogshow = $(current)/../ogshow
rapDir = $(current)/../rap
VPATH = .:$(mapping):$(ogshow):$(rapDir)

hdf=         $(HDF)

all = hype
all: $(all)

doc: hyperbolic

# HyperbolicMappingInclude.tex: HyperbolicMapping.C
# 	extract.p HyperbolicMapping.C


hyperbolic: 
	pdflatex hyperbolic
	makeindex hyperbolic.idx
	bibtex hyperbolic

hyperbolicOld:  HyperbolicMappingInclude.tex 
	latex hyperbolic
	makeindex hyperbolic.idx
	bibtex hyperbolic
	latex hyperbolic
	dvips -P pdf -G0 -o hyperbolic.ps hyperbolic.dvi 
	ps2pdf hyperbolic.ps
#	cp hyperbolic.pdf $(WEBPAGE)/documentation

hyperbolichtml:  HyperbolicMappingInclude.tex
	lh.p hyperbolic.tex
	latex hyperbolicHTML
	bibtex hyperbolicHTML
	makeindex hyperbolicHTML.idx
	latex hyperbolicHTML
	latex2html -local_icons hyperbolicHTML
	rm -rf $(WEBPAGE)/documentation/hyperbolicHTML
	mkdir $(WEBPAGE)/documentation/hyperbolicHTML
	cp hyperbolicHTML/*.* $(WEBPAGE)/documentation/hyperbolicHTML
	rm -rf hyperbolicHTML



# -----------------------

GeometricADT3dInt.o: $(Overture)/templates/GeometricADT3dInt.C
	$(CC) $(CCFLAGS) -c $(Overture)/templates/GeometricADT3dInt.C

NurbsMapping.o: NurbsMapping.C
	$(CC) $(CCFLAGS) -c NurbsMapping.C

nurbsEvaluation.o: nurbsEvaluation.C
	$(CC) $(CCFLAGSO) -c nurbsEvaluation.C

TrimmedMapping.o: TrimmedMapping.C
	$(CC) $(CCFLAGS) -c TrimmedMapping.C

UnstructuredMapping.o: UnstructuredMapping.C
	$(CC) $(CCFLAGS) -c UnstructuredMapping.C

TriangleWrapper.o: ../ugen/TriangleWrapper.C
	$(CC) $(CCFLAGS) -c ../ugen/TriangleWrapper.C

HyperbolicMapping.p.o: HyperbolicMapping.p.C
	$(CC) $(CCFLAGSO) -c HyperbolicMapping.p.C

hypeUtil.p.o: hypeUtil.p.C
	$(CC) $(CCFLAGSO) -c hypeUtil.p.C

mappingUtil.o: mappingUtil.C
	$(CC) $(CCFLAGSF) -c mappingUtil.C

ellipticSmooth.f : ellipticSmooth.bf; bpp ellipticSmooth.bf

hype= hype.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o stretch.o ../mapping/MappingBuilder.o GridSmoother.o  \
           ellipticSmooth.o MatchingCurve.o GridStretcher.o
hype= hype.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o stretch.o GridSmoother.o GridStretcher.o Bound.o TrimmedMapping.o \
          ../mapping/UnstructuredMapping.o ../mapping/compositeTopology.o ../ogshow/plotGridQuality.o
hype= hype.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o stretch.o GridSmoother.o GridStretcher.o Bound.o ../ogshow/plotGridQuality.o \
          ellipticSmooth.o
hype= hype.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o stretch.o GridSmoother.o GridStretcher.o MatchingCurve.o ../mapping/DataPointMapping.o
hype= hype.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o stretch.o GridSmoother.o GridStretcher.o MatchingCurve.o generateParallel.o \
          DataPointMapping.o plotStructured.o dpmMap.o dpmScalar.o Mapping.o
hype: $(hype) 
	$(CC) $(CCFLAGS) -o $@ $(hype) $(CLIBS) $(FLIBS) $(GLIBS)

rap = rap.o generate.o update.o HyperbolicMapping.o util.o project.o applyBoundaryConditions.o startCurve.o \
          plot.o compositeTopology.o unstructuredProject.o
rap: $(rap)
	$(CC) $(CCFLAGS) -o rap $(rap) $(Overture)/Cad/rap.a $(CLIBS) $(FLIBS) $(GLIBS)

clean:
	rm -f *.o hype

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

