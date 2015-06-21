# include ../config/Makefile.$(VENDOR)
include ../config/Makefile.intel

# CC = insure

current = .
rapDir = $(current)/../rap
mapping = $(current)/../mapping
# ogshow = $(current)/../ogshow
hype = $(current)/../hype
VPATH = $(mapping):$(ogshow):$(hype):$(rapDir)

hdf=         $(HDF)

all = pm
all: $(all)

ChimeraLib = /users/henshaw/hypgen/chimeraDouble/include/libtools.a
ChimeraLib = /users/henshaw/hypgen/chimera/include/libtools.a

ChimeraLib = $(Overture)/Hypgen/hyper.o $(Overture)/Hypgen/surgrd.o -L$(Overture)/Hypgen -lHypgen
ChimeraLib = 

# These are the OLD style source code documents
documentFilesOLD = SplineMappingInclude.tex TFIMappingInclude.tex DataPointMappingInclude.tex \
                SmoothedPolygonMappingInclude.tex CircleMappingInclude.tex ReductionMappingInclude.tex \
                TrimmedMappingInclude.tex PlaneMappingInclude.tex TriangleInclude.tex \
                IntersectionMappingInclude.tex StretchMappingInclude.tex \
                AirfoilMappingInclude.tex  \
                FilletMappingInclude.tex JoinMappingInclude.tex BoxMappingInclude.tex \
                CrossSectionMappingInclude.tex MappingInclude.tex EllipticTransformInclude.tex \
                CylinderMappingInclude.tex SquareMappingInclude.tex \
                RevolutionMappingInclude.tex ReparameterizationTransformInclude.tex \
                RestrictionMappingInclude.tex AnnulusMappingInclude.tex MatrixMappingInclude.tex \
                MatrixTransformInclude.tex DepthMappingInclude.tex SphereMappingInclude.tex \
                OrthographicTransformInclude.tex NurbsMappingInclude.tex CompositeSurfaceInclude.tex \
                EllipticGridGeneratorInclude.tex LineMappingInclude.tex ApproximateGlobalInverseInclude.tex \
                ExactLocalInverseInclude.tex SweepMappingInclude.tex UnstructuredMappingImp.tex \
                QuadraticMappingInclude.tex RocketMappingInclude.tex OffsetShellInclude.tex \
                NormalMappingInclude.tex ReorientMappingInclude.tex LoftedSurfaceMappingInclude.tex


documentFiles = mapping.tex inverse.tex  \
       AnnulusMapping.tex \
       AirfoilMapping.tex  \
       BoxMapping.tex  \
       CircleMapping.tex  \
       ComposeMapping.tex  \
       CompositeSurface.tex  \
       CrossSectionMapping.tex  \
       CylinderMapping.tex  \
       DataPointMapping.tex  \
       DepthMapping.tex  \
       EllipticTransform.tex  \
       FilletMapping.tex  \
       IntersectionMapping.tex  \
       JoinMapping.tex  \
       LineMapping.tex  \
       LoftedSurfaceMapping.tex  \
       MatrixMapping.tex  \
       MatrixTransform.tex  \
       NormalMapping.tex  \
       NurbsMapping.tex  \
       OffsetShell.tex  \
       OrthographicTransform.tex  \
       PlaneMapping.tex  \
       QuadraticMapping.tex  \
       ReductionMapping.tex  \
       ReorientMapping.tex  \
       ReparameterizationTransform.tex  \
       RestrictionMapping.tex  \
       RevolutionMapping.tex  \
       RocketMapping.tex  \
       SmoothedPolygon.tex  \
       SphereMapping.tex  \
       SplineMapping.tex  \
       SquareMapping.tex  \
       StretchMapping.tex  \
       StretchedSquare.tex  \
       StretchTransform.tex  \
       SweepMapping.tex  \
       TFIMapping.tex  \
       TrimmedMapping.tex  \
       UnstructuredMapping.tex  \
       Fraction.tex


doc: $(documentFiles)
	pdflatex mapping

mappingOld.pdf: $(documentFiles)
	latex mapping
	makeindex mapping.idx
	latex mapping
	dvips -P pdf -G0 -o mapping.ps  mapping.dvi
	ps2pdf mapping.ps

mappingOld: $(documentFiles)
	latex mapping
	makeindex mapping.idx
	latex mapping
	dvips -P pdf -G0 -o mapping.ps  mapping.dvi
	ps2pdf mapping.ps
	# cp mapping.pdf $(WEBPAGE)/documentation

mappinghtml:  $(documentFiles)
	lh.p mapping.tex
	latex mappingHTML
	bibtex mappingHTML
	makeindex mappingHTML.idx
	latex mappingHTML
	latex2html -local_icons mappingHTML

mappingweb: mappinghtml
	rm -rf $(WEBPAGE)/documentation/mappingHTML
	mkdir $(WEBPAGE)/documentation/mappingHTML
	cp mappingHTML/*.* $(WEBPAGE)/documentation/mappingHTML
	rm -rf mappingHTML

cad:
	extract.p readMappings.C IgesReader.C
	latex mappingsFromCAD



mapDoc:  $(documentFiles)
	latex mapDoc

# -- for developing a new section or figure: 
fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps


AirfoilMappingInclude.tex: AirfoilMapping.C
	extract.p AirfoilMapping.C
AnnulusMappingInclude.tex: AnnulusMapping.C
	extract.p AnnulusMapping.C
ApproximateGlobalInverseInclude.tex: Inverse.C stencilWalk.C
	extract.p Inverse.C stencilWalk.C
BoxMappingInclude.tex: BoxMapping.C
	extract.p BoxMapping.C
CircleMappingInclude.tex: CircleMapping.C
	extract.p CircleMapping.C
CompositeSurfaceInclude.tex: CompositeSurface.C
	extract.p CompositeSurface.C
CrossSectionMappingInclude.tex: CrossSectionMapping.C
	extract.p CrossSectionMapping.C
CylinderMappingInclude.tex: CylinderMapping.C
	extract.p CylinderMapping.C
DataPointMappingInclude.tex: DataPointMapping.C
	extract.p DataPointMapping.C
DepthMappingInclude.tex: DepthMapping.C
	extract.p DepthMapping.C
EllipticTransformInclude.tex: EllipticTransform.C
	extract.p EllipticTransform.C
EllipticGridGeneratorInclude.tex: EllipticGridGenerator.C Elliptic.C
	extract.p EllipticGridGenerator.C Elliptic.C
ExactLocalInverseInclude.tex: Inverse.C
	extract.p Inverse.C
FilletMappingInclude.tex: FilletMapping.C
	extract.p FilletMapping.C
JoinMappingInclude.tex: JoinMapping.C
	extract.p JoinMapping.C
IntersectionMappingInclude.tex: IntersectionMapping.C
	extract.p IntersectionMapping.C
LoftedSurfaceMappingInclude.tex: LoftedSurfaceMapping.C
	extract.p LoftedSurfaceMapping.C
LineMappingInclude.tex: LineMapping.C
	extract.p LineMapping.C
MappingInclude.tex: Mapping.C
	extract.p Mapping.C
MatrixMappingInclude.tex: MatrixMapping.C
	extract.p MatrixMapping.C
MatrixTransformInclude.tex: MatrixTransform.C
	extract.p MatrixTransform.C
NormalMappingInclude.tex: NormalMapping.C
	extract.p NormalMapping.C
NurbsMappingInclude.tex: NurbsMapping.C
	extract.p NurbsMapping.C
OrthographicTransformInclude.tex: OrthographicTransform.C
	extract.p OrthographicTransform.C
PlaneMappingInclude.tex: PlaneMapping.C
	extract.p PlaneMapping.C
OffsetShellInclude.tex: OffsetShell.C
	extract.p OffsetShell.C
QuadraticMappingInclude.tex: QuadraticMapping.C
	extract.p QuadraticMapping.C
ReductionMappingInclude.tex: ReductionMapping.C
	extract.p ReductionMapping.C
ReorientMappingInclude.tex: ReorientMapping.C
	extract.p ReorientMapping.C
ReparameterizationTransformInclude.tex: ReparameterizationTransform.C
	extract.p ReparameterizationTransform.C
RestrictionMappingInclude.tex: RestrictionMapping.C
	extract.p RestrictionMapping.C
RevolutionMappingInclude.tex: RevolutionMapping.C
	extract.p RevolutionMapping.C
RocketMappingInclude.tex: RocketMapping.C
	extract.p RocketMapping.C
SmoothedPolygonMappingInclude.tex: SmoothedPolygonMapping.C
	extract.p SmoothedPolygonMapping.C
SplineMappingInclude.tex: SplineMapping.C
	extract.p SplineMapping.C
SphereMappingInclude.tex: SphereMapping.C
	extract.p SphereMapping.C
SquareMappingInclude.tex: SquareMapping.C
	extract.p SquareMapping.C
StretchMappingInclude.tex: StretchMapping.C
	extract.p StretchMapping.C
SweepMappingInclude.tex: SweepMapping.C
	extract.p SweepMapping.C
TFIMappingInclude.tex: TFIMapping.C
	extract.p TFIMapping.C
TrimmedMappingInclude.tex: TrimmedMapping.C
	extract.p TrimmedMapping.C
TriangleInclude.tex: TriangleClass.C
	extract.p TriangleClass.C
UnstructuredMappingImp.tex: UnstructuredMapping.C
	extract.p UnstructuredMapping.C

# GraphicsInterface.o 
files = Fraction.o Bound.o Mapping.o checkMapping.o Stretch.o maputil.o Inverse.o MappingRC.o \
        stencilWalk.o BoundingBox.o Square.o Sphere.o LineMapping.o GraphicsInterface.o 

files = MappingRC.o Stretch.o
files = 
map = map.o  $(files)
map: $(map) 
	$(CC) -y_nblank $(CCFLAGS) -o map $(map) \
        -y_dskini -ydskini_ $(CLIBS) $(FLIBS) $(GLIBS)

mapi = mapi.o $(files)
mapi: $(mapi) 
	$(CC) $(CCFLAGS)  \
	-o mapi $(mapi)  $(CLIBS) $(GLIBS) $(PSTUBS)

# test mappings and Graphics Interface
mapgi = mapgi.o GraphicsInterface.o SmoothedPolygon.o cggpoly.o \
        DataPointMapping.o dpm.o $(files)
mapgi: $(mapgi) 
	purelink $(CC) $(CCFLAGS)  \
	-o mapgi $(mapgi)  $(CLIBS) $(FLIBS) $(GLIBS)

# Test out the inverse routines that use bounding boxes
testInverse = testInverse.o  checkMapping.o CylinderMapping.o Mapping.o
testInverse = testInverse.o Inverse.o stencilWalk.o
testInverse = testInverse.o ComposeMapping.o
testInverse = testInverse.o  NurbsMapping.o nurbsEvaluation.o
testInverse = testInverse.o RevolutionMapping.o Mapping.o Inverse.o stencilWalk.o inverseMap.o
testInverse = testInverse.o stencilWalk.o Mapping.o inverseMap.o NurbsMapping.o Inverse.o
testInverse = testInverse.o findNearestGridPoint.o inverseMap.o DataPointMapping.o Inverse.o dpmMap.o
testInverse: $(testInverse) 
	$(CC) $(CCFLAGS) -o testInverse $(testInverse) $(CLIBS) $(FLIBS) $(GLIBS)

# test nurbs editing
testNurbs = testEditNurbs.o editNurbsCurve.o Point.o mogl.o
testNurbs: $(testNurbs)
	$(CC) $(CCFLAGS) -o testNurbs $(testNurbs) \
         $(CLIBS) $(FLIBS) $(GLIBS)

testNurbsInterpolate = testNurbsInterpolate.o NurbsMapping.o nurbsInterpolate.o
testNurbsInterpolate: $(testNurbsInterpolate)
	$(CC) $(CCFLAGS) -o testNurbsInterpolate $(testNurbsInterpolate) $(CLIBS)  $(FLIBS) $(GLIBS) 

sunil = hyperbolic.step1.o
sunil: $(sunil)
	$(CC) $(CCFLAGS) -o sunil $(sunil) \
         $(CLIBS) $(FLIBS) $(GLIBS)

# test inverse 
ti = ti.o
ti: $(ti) 
	$(CC) $(CCFLAGS) -o ti $(ti) \
         $(CLIBS) $(FLIBS) $(GLIBS)

example1 = example1.o
example1: $(example1) 
	$(CC) $(CCFLAGS) -o example1 $(example1) \
         $(CLIBS) $(FLIBS) $(GLIBS)



tf= tf.o r1mach.o d1mach.o
tf: $(tf)
	g77 -o tf $(tf) 

t1= t1.o ../gf/second3.o
t1: $(t1)
	$(CC) $(CCFLAGS) -o t1 $(t1) \
         $(CLIBS) $(FLIBS) $(GLIBS)

t2= t2.o 
t2: $(t2)
	$(CC) $(CCFLAGS) -o t2 $(t2) \
         $(CLIBS) $(FLIBS) $(GLIBS)

test1 = Square.o test1.o
test1: $(test1) 
	$(CC) $(CCFLAGS)  \
	-o test1 $(test1)  $(CLIBS) $(FLIBS) 

tscan = tscan.o
tscan: $(tscan) 
	$(CC) $(CCFLAGS)  \
	-o tscan $(tscan)  $(CLIBS) $(FLIBS)  $(GLIBS) $(PSTUBS)

startest: startest.o StarSplineMapping.o
	$(CC) $(CCFLAGS) -o startest startest.o StarSplineMapping.o $(CLIBS) $(FLIBS) $(GLIBS)
circtest: circtest.o CircSplineMapping.o
	$(CC) $(CCFLAGS) -o circtest circtest.o CircSplineMapping.o $(CLIBS) $(FLIBS) $(GLIBS)
slottest: slottest.o SlotSplineMapping.o
	$(CC) $(CCFLAGS) -o slottest slottest.o SlotSplineMapping.o $(CLIBS) $(FLIBS) $(GLIBS)

# test ray trace crossing algorithm
crossCheck = crossCheck.o stencilWalk.o Triangle.o
crossCheck = crossCheck.o 
crossCheck: $(crossCheck) 
	$(CC) $(CCFLAGS)  \
	-o crossCheck $(crossCheck)  $(CLIBS) $(FLIBS) $(GLIBS)

bb = Square.o bb.o stencilWalk.o BoundingBox.o tlist.o
bb: $(bb) 
	$(CC) $(CCFLAGS)  \
	-o bb $(bb)  $(CLIBS) $(FLIBS) 

space = space.o maputil.o
space: $(space) 
	$(CC) $(CCFLAGS)  \
	-o space $(space)  $(CLIBS) $(FLIBS) 

bound = Fraction.o Bound.o bound.o
bound: $(bound)
	$(CC) $(CCFLAGS) -o bound $(bound) $(CLIBS) $(FLIBS)  $(GLIBS)

gitest=   gitest.o GraphicsInterface.o $(DSKOBJ)
gitest:   $(gitest); $(CC) $(CCFLAGS) -o $@ $(gitest) $(CLIBS) $(GLIBS)

# Geoff's test of the mapping class
mapg = mapg.o $(files)
mapg: $(mapg) 
	$(CC) $(CCFLAGS) -o mapg $(mapg) \
        $(CLIBS) $(FLIBS) $(PSTUBS)

# create a mapping interactively with a graphics Interface
giFiles = ../ogshow/plotMapping.o ../ogshow/GL_GraphicsInterface.o ../ogshow/GenericGraphicsInterface.o \
          ../ogshow/GraphicsParameters.o

# hypgenFiles = HyperbolicSurfaceMapping.o hyper.o hypgen.o hypgen2.o surgrd.o cmpltm.o

# test distanceToCell function
distToCell = distToCell.o stencilWalk.o
distToCell: $(distToCell) 
	$(CC) $(CCFLAGS)  \
	-o distToCell $(distToCell)  $(CLIBS) $(FLIBS)  $(GLIBS) $(PSTUBS)


GeometricADT3dInt.o: $(Overture)/templates/GeometricADT3dInt.C
	$(CC) $(CCFLAGS) -c $(Overture)/templates/GeometricADT3dInt.C

NurbsMapping.o: NurbsMapping.C
	$(CC) $(CCFLAGSG) -c NurbsMapping.C

nurbsEvaluation.o: nurbsEvaluation.C
	$(CC) $(CCFLAGSF) -c nurbsEvaluation.C

TrimmedMapping.o: TrimmedMapping.C
	$(CC) $(CCFLAGSG) -c TrimmedMapping.C

UnstructuredMapping.o: UnstructuredMapping.C
	$(CC) $(CCFLAGSG) -c UnstructuredMapping.C


HyperbolicMapping.p.o: HyperbolicMapping.p.C
	$(CC) $(CCFLAGSO) -c HyperbolicMapping.p.C

mappingUtil.o: mappingUtil.C
	$(CC) $(CCFLAGS) -c mappingUtil.C

readMappings.o: readMappings.C
	$(CC) $(CCFLAGS) -c readMappings.C

# the function sequenceToItem can be slow
IgesReader.o: IgesReader.C
	$(CC) $(CCFLAGSO) -c IgesReader.C

compositeTopology.o: compositeTopology.C
	$(CC) $(CCFLAGSG) -c compositeTopology.C

AnnulusMapping.C : AnnulusMapping.bC; bpp AnnulusMapping.bC
AnnulusMapping.o: AnnulusMapping.C
	$(CC) $(CCFLAGSF) -c AnnulusMapping.C

# TriangleWrapper.o: ../ugen/TriangleWrapper.C
# 	$(CC) $(CCFLAGS) -c ../ugen/TriangleWrapper.C

TriangleWrapper.o: TriangleWrapper.C
	$(CC) $(CCFLAGSF) -c TriangleWrapper.C

# CFLAGST = -g -DLINUX $(cc_FLAGS)  $(Include)  -I$(XLIBS) -DSHM
CFLAGST = -g $(cc_FLAGS)  $(Include)  -I$(XLIBS) -DSHM
CFLAGST = -O3 -ffloat-store -I/home/henshaw/Overture.g/include
TriangleSource.o: TriangleSource.c
	$(cc) $(CFLAGST) -c TriangleSource.c

nurbs_eval.o: nurbs_eval.F; $(FC) $(FFLAGSO) -DKK_NRB_BLOCKED -c $<

# special target while we create the buildTrimmedMappingFunction: 
# createMappings.o: createMappings.C; $(CC) $(CCFLAGSF) -DBUILD_TRIMMED_MAPPING -c $<

mbuilder= mbuilder.o CompositeSurface.o NurbsMapping.o TrimmedMapping.o
mbuilder= mbuilder.o UserDefinedMapping1.o createMappings.o checkMapping.o
mbuilder= mbuilder.o DataPointMapping.o createMappings.o checkMapping.o
mbuilder= mbuilder.o Mapping.o UnstructuredMapping.o CompositeSurface.o unstructuredProject.o DataPointMapping.o \
         ../hype/project.o
mbuilder= mbuilder.o UnstructuredMapping.o
mbuilder= mbuilder.o checkMapping.o UserDefinedMapping1.o SweepMapping.o SplineMapping.o tspack.o CompositeSurface.o Mapping.o ReparameterizationTransform.o JoinMapping.o IntersectionMapping.o
mbuilder= mbuilder.o checkMapping.o IntersectionMapping.o
mbuilder= mbuilder.o checkMapping.o AnnulusMapping.o
mbuilder= mbuilder.o checkMapping.o MatrixTransform.o
mbuilder= mbuilder.o checkMapping.o UnstructuredMapping.o
mbuilder= mbuilder.o TrimmedMapping.o ../gui/getAnswer.o
mbuilder= mbuilder.o TrimmedMapping.o unstructuredProject.o 
mbuilder= mbuilder.o createMappings.o DataFormats.o UnstructuredMapping.o
mbuilder= mbuilder.o createMappings.o MappingBuilder.o UserDefinedMapping1.o SmoothedPolygonMapping.o LoftedSurfaceMapping.o
mbuilder= mbuilder.o createMappings.o TrimmedMappingBuilder.o TrimmedMapping.o ReparameterizationTransform.o NurbsMapping.o CompositeSurface.o compositeTopology.o FaceInfo.o
mbuilder= mbuilder.o createMappings.o SquareMapping.o Mapping.o AnnulusMapping.o stretchUpdate.o
mbuilder= mbuilder.o createMappings.o checkMapping.o StretchMapping.o stretchUpdate.o StretchTransform.o
mbuilder= mbuilder.o createMappings.o checkMapping.o NurbsMapping.o 
mbuilder= mbuilder.o createMappings.o checkMapping.o StretchMapping.o
mbuilder= mbuilder.o createMappings.o MappingBuilder.o LoftedSurfaceMapping.o
mbuilder= mbuilder.o createMappings.o MappingBuilder.o NurbsMapping.o nurbsInterpolate.o
mbuilder= mbuilder.o createMappings.o AnnulusMapping.o Mapping.o
mbuilder: $(mbuilder) 
	$(CC) $(CCFLAGS) -o $@ $(mbuilder) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

makeSurf = makeSurf.o
makeSurf: $(makeSurf) 
	$(CC) $(CCFLAGS) -o $@ $(makeSurf) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

rap= rap.o  
rap: $(rap) 
	$(CC) $(CCFLAGS) -o $@ $(rap) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

testMapping= testMapping.o  FilamentMapping.o HyperbolicMapping.o
testMapping: $(testMapping) 
	$(CC) $(CCFLAGS) -o $@ $(testMapping) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)


# test saving multiple Mapping's in same file (without duplication)
testdbid= testdbid.o ReferenceCountingList.o MappingRC.o Mapping.o
testdbid: $(testdbid) 
	$(CC) $(CCFLAGS) -o $@ $(testdbid) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

pm2= pm2.o HyperbolicSurfaceMapping.o 
pm2: $(pm2)
	$(CC) $(CCFLAGS) -o $@ $(pm2) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)


stencilWalk.o: stencilWalk.C
	 $(CC) $(CCFLAGSG) -c stencilWalk.C

Inverse.o: Inverse.C
	 $(CC) $(CCFLAGSG) -c Inverse.C

MappingGeometry.C : MappingGeometry.bC; bpp MappingGeometry.bC
MappingGeometry.o: MappingGeometry.C
	 $(CC) $(CCFLAGSF) -c MappingGeometry.C

dpmInverse.C : dpmInverse.bC; bpp dpmInverse.bC
dpmInverse.o: dpmInverse.C
	 $(CC) $(CCFLAGSG) -c dpmInverse.C
#	 $(CC) $(CCFLAGSF) -c dpmInverse.C

# this is the Array version of dpmInverse:
dpmInverseA.o: dpmInverseA.C
	 $(CC) $(CCFLAGSF) -c dpmInverseA.C

DataPointMapping.o: DataPointMapping.C
	$(CC) $(CCFLAGS) -c DataPointMapping.C

DataPointMapping.p.o: DataPointMapping.p.C
	$(CC) $(CCFLAGSO) -c DataPointMapping.p.C

# old version:
dpmScalarA.o: dpmScalarA.C
	$(CC) $(CCFLAGS) -c dpmScalarA.C

dpmScalar.o: dpmScalar.C
	$(CC) $(CCFLAGSG) -c dpmScalar.C
#	$(CC) $(CCFLAGSF) -c dpmScalar.C

dpmVector.p.o: dpmVector.p.C
	$(CC) $(CCFLAGSF) -c dpmVector.p.C

minmax.o: minmax.C
	$(CC) $(CCFLAGSF) -c minmax.C

unstructuredProject.o: unstructuredProject.C
	$(CC) $(CCFLAGSG) -c unstructuredProject.C

Triangle.o: Triangle.C
	$(CC) $(CCFLAGSF) -c Triangle.C

tdpm= tdpm.o dpmInverse.o DataPointMapping.o stencilWalk.o dpmScalar.o Inverse.o
tdpm= tdpm.o dpmInverse.o DataPointMapping.o
tdpm= tdpm.o dpmInverse.o DataPointMapping.o dpmScalar.o checkMapping.o stencilWalk.o 
tdpm= tdpm.o dpmInverse.o DataPointMapping.o dpmScalar.o dpmMap.o checkMapping.o stencilWalk.o Inverse.o \
      inverseMap.o
tdpm: $(tdpm)
	$(CC) $(CCFLAGS) -o $@ $(tdpm) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

# test and time the inverse 
tmi= tmi.o Inverse.o
tmi= tmi.o Mapping.o Inverse.o
tmi= tmi.o Inverse.o stencilWalk.o Mapping.o
tmi= tmi.o stencilWalk.o Inverse.o dpmInverse.o inverseMap.o NurbsMapping.o Mapping.o nurbsEvaluation.o 
tmi: $(tmi)
	$(CC) $(CCFLAGS) -o $@ $(tmi) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

# test the inversion of surfaces
tsi= tsi.o NurbsMapping.o Inverse.o
tsi: $(tsi)
	$(CC) $(CCFLAGS) -o $@ $(tsi) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

# test timing of nurbs
tnurbs= tnurbs.o nurbsEvaluation.o NurbsMapping.o Inverse.o stencilWalk.o nurbs_eval.o
tnurbs= tnurbs.o nurbsEvaluation.o NurbsMapping.o nurbs_eval.o
tnurbs: $(tnurbs)
	$(CC) $(CCFLAGS) -o $@ $(tnurbs) $(CLIBS) $(FLIBS) $(GLIBS)


# test out hypgen interface
hype= hype.o hypgen.o hypgen2.o hyper.o DataPointMapping.o
hype: $(hype)
	$(CC) $(CCFLAGS) -o $@ $(hype) $(CLIBS) $(FLIBS) $(GLIBS)

# surgrd: surgrd < collar.i   
#      ---> creates surgrd.xyz
surgrd= surgrd.o nrstpt.o toplgy.o rsttop.o error.o submms.o cmpltm.o sfuns.o offsur.o cellpr.o cellpx.o \
      trimChimera.o inv3x3.o surnor.o cmemc.o
surgrd= surgrd.o cmpltm.o
surgrd: $(surgrd)
	 $(FC) $(FFLAGS) -o $@ $(surgrd) /users/henshaw/hypgen/chimera/include/libtools.a

surGrd= surGrd.o surgrd.o cmpltm.o readPlot3d.o HyperbolicSurfaceMapping.o DataPointMapping.o
surGrd: $(surGrd)
	 $(CC) $(CCFLAGS) -o $@ $(surGrd)  $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)


# test out equi-distribution
equi= equiTest.o equi.o 
equi: $(equi)
	$(CC) $(CCFLAGS) -o $@ $(equi) $(CLIBS) $(FLIBS) $(GLIBS)

# test out refinements
refine= refine.o RestrictionMapping.o ReparameterizationTransform.o ../ogshow/plotMapping.o \
        ../gf/initOvertureGlobalVariables.o
refine= refine.o ReparameterizationTransform.o 
refine: $(refine)
	$(CC) $(CCFLAGS) -o $@ $(refine) $(CLIBS) $(FLIBS) $(GLIBS)

# test out iges reader
iges= iges.o IgesReader.o NurbsMapping.o
iges: $(iges)
	$(CC) $(CCFLAGS) -o $@ $(iges) $(CLIBS) $(FLIBS) $(GLIBS)

# test out NURBS class
nurbs= nurbs.o NurbsMapping.o
nurbs: $(nurbs)
	$(CC) $(CCFLAGS) -o $@ $(nurbs) $(CLIBS) $(FLIBS) $(GLIBS)

# test out the trimmed surface class
trim= trim.o TrimmedMapping.o  NurbsMapping.o ../ogshow/plotMapping.o Mapping.o
trim= trim.o TrimmedMapping.o  NurbsMapping.o Inverse.o Mapping.o stencilWalk.o
trim= trim.o TrimmedMapping.o  NurbsMapping.o Inverse.o Mapping.o stencilWalk.o initializeMappingList.o
trim= trim.o TrimmedMapping.o  NurbsMapping.o Inverse.o Mapping.o  ../ogshow/plotMapping.o 
trim= trim.o TrimmedMapping.o  NurbsMapping.o Inverse.o Mapping.o 
trim= trim.o TrimmedMapping.o QuadTree.o  NurbsMapping.o  initializeMappingList.o createMappings.o
trim= trim.o Mapping.o
trim: $(trim)
	$(CC) $(CCFLAGS) -o $@ $(trim) $(CLIBS) $(FLIBS) $(GLIBS)

# test out the CompositeSurface class
cSurf= cSurf.o CompositeSurface.o readMappings.o initializeMappingList.o TrimmedMapping.o Inverse.o \
       ../ogshow/plotMapping.o
cSurf= cSurf.o CompositeSurface.o TrimmedMapping.o initializeMappingList.o Mapping.o ../ogshow/plotMapping.o \
     NurbsMapping.o

cSurf= cSurf.o
cSurf: $(cSurf)
	$(CC) $(CCFLAGS) -o $@ $(cSurf) $(CLIBS) $(FLIBS) $(GLIBS) 

# Here is a test routine for Dale Slone
surfaceQuery= surfaceQuery.o unstructuredProject.o CompositeSurface.o
surfaceQuery: $(surfaceQuery)
	$(CC) $(CCFLAGS) -o $@ $(surfaceQuery) $(CLIBS) $(FLIBS) $(GLIBS) 

# Test inside outside for surfaces
testInsideOutside= testInsideOutside.o unstructuredProject.o TriangleClass.o UnstructuredMapping.o
testInsideOutside: $(testInsideOutside)
	$(CC) $(CCFLAGS) -o $@ $(testInsideOutside) $(CLIBS) $(FLIBS) $(GLIBS) 

# 
plot3dConvert= plot3dConvert.o dpm.o
plot3dConvert: $(plot3dConvert)
	$(CC) $(CCFLAGS) -o $@ $(plot3dConvert) $(CLIBS) $(FLIBS) $(GLIBS)

# intersection of surfaces
inter= inter.o IntersectionMapping.o Triangle.o NurbsMapping.o 
inter= inter.o IntersectionMapping.o Mapping.o plotMapping.o
inter: $(inter)
	$(CC) $(CCFLAGS) -o $@ $(inter) $(CLIBS) $(FLIBS) $(GLIBS)

# intersection of curves
interc= interCurve.o IntersectionMapping.o 
interc: $(interc)
	$(CC) $(CCFLAGS) -o $@ $(interc) $(CLIBS) $(FLIBS) $(GLIBS)

# test fillets
fillet= fillet.o FilletMapping.o IntersectionMapping.o  HyperbolicMapping.o Inverse.o \
        initializeMappingList.o createMappings.o HyperbolicSurfaceMapping.o dpm.o $(hypgenFiles) 
fillet= fillet.o FilletMapping.o HyperbolicSurfaceMapping.o  $(hypgenFiles) 
fillet: $(fillet)
	$(CC) $(CCFLAGS) -o $@ $(fillet) $(CLIBS) $(FLIBS) $(GLIBS) $(ChimeraLib)

# streaming data base:
tdb= tdb.o Mapping.o
tdb: $(tdb)
	$(CC) $(CCFLAGS) -o $@ $(tdb) $(CLIBS) $(FLIBS) $(GLIBS)

# Triangle class
tri= tri.o Triangle.o
tri: $(tri)
	$(CC) $(CCFLAGS) -o $@ $(tri) $(CLIBS) $(FLIBS) $(GLIBS)

# convert m6 data file
m6c= m6c.o 
m6c: $(m6c)
	$(FC) -# $(FFLAGS) -o $@ $(m6c) 

adt = adt.o
adt: $(adt) 
	$(CC) $(CCFLAGS) -o $@ $(adt) $(CLIBS) $(FLIBS) $(GLIBS) 

# PLY polygon file test codes **pf
PLY_OBJS = plytest.o plyfile.o
plytestraw: $(PLY_OBJS)
	cc -o plytestraw $(PLY_OBJS) -lm

plyTestFile: plyTestFile.o plyFileInterface.o plyfile.o
	$(CC) -o plyTestFile plyTestFile.o plyFileInterface.o plyfile.o \
	 $(CLIBS) $(FLIBS) $(GLIBS)

clean:
	rm -f *.o testInverse mbuilder nurbs trim hype equi iges cSurf surGrd tdb tdpm bound fillet interc crossCheck \
            tri ti surgrd cs m6c inter plot3dConvert tsi tmi adt

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

