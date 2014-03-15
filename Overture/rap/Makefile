include ../config/Makefile.$(VENDOR)

# current = ${shell pwd}
current = .
mapping = $(current)/../mapping
ogshow = $(current)/../ogshow
hype=  $(current)/../hype
ugen=  $(current)/../ugen

VPATH = $(mapping):$(ogshow):$(hype):$(ugen)

OvertureCad=      $(Overture)/Cad


all: rap

# ====================== start documentation =================================

documentFiles = ModelBuilderInclude.tex 

ModelBuilderInclude.tex: ModelBuilder.C rapNewModel.C rapEditModel.C rapSimpleGeometry.C linerGeometry.C
	extract.p ModelBuilder.C
	extract.p rapNewModel.C
	extract.p rapEditModel.C
	extract.p rapSimpleGeometry.C
	extract.p linerGeometry.C


# -- new way
modelBuilder: $(documentFiles) modelBuilder.tex
	pdflatex modelBuilder
	makeindex modelBuilder.idx
	pdflatex modelBuilder

# modelBuilder: $(documentFiles) modelBuilder.tex
# 	latex modelBuilder
# 	makeindex modelBuilder.idx
# 	latex modelBuilder
# 	dvips -P pdf -G0 -o modelBuilder.ps  modelBuilder.dvi
# 	ps2pdf modelBuilder.ps

lb : linerBuilder
	acroread linerBuilder.pdf &

linerBuilder: $(documentFiles) linerBuilder.tex
	latex linerBuilder
	makeindex linerBuilder.idx
	latex linerBuilder
	dvips -P pdf -G0 -o linerBuilder.ps  linerBuilder.dvi
	ps2pdf linerBuilder.ps

# ====================== end documentation =================================


nurbsEvaluation.o: nurbsEvaluation.C
	$(CC) $(CCFLAGSO) -c nurbsEvaluation.C
#	$(CC) $(CCFLAGS) -c nurbsEvaluation.C

rap = rap.o rapSimpleGeometry.o rapNewModel.o rapEditModel.o rapUtil.o nurbsCurveEditor.o \
       linerGeometry.o ModelBuilder.o rapSurfaceGrids.o rapVolumeGrids.o SphereLoading.o \
      fillVolumeWithSpheres.o RandomSampling.o ../mapping/unstructuredProject.o

rap = rap.o rapSimpleGeometry.o rapNewModel.o rapEditModel.o rapUtil.o nurbsCurveEditor.o \
       linerGeometry.o ModelBuilder.o rapSurfaceGrids.o rapVolumeGrids.o SphereLoading.o \
      fillVolumeWithSpheres.o RandomSampling.o 
rap: $(rap) 
	$(PURIFY) $(CC) $(CCFLAGS) -o $@ $(rap) \
        $(CLIBS) $(OvertureCad)/rap.a $(FLIBS) $(GLIBS)   

# test code to call liner with a list of commands
buildLiner = buildLiner.o rapSimpleGeometry.o rapNewModel.o rapEditModel.o rapUtil.o nurbsCurveEditor.o \
       linerGeometry.o ModelBuilder.o rapSurfaceGrids.o rapVolumeGrids.o
buildLiner: $(buildLiner) 
	$(PURIFY) $(CC) $(CCFLAGS) -o $@ $(buildLiner) \
        $(CLIBS) $(OvertureCad)/rap.a $(FLIBS) $(GLIBS)


cgridO = cgrid.o ingrid.o UnstructuredMapping.o DataFormats.o plotUnstructured.o GL_GraphicsInterface.o

cgrid: $(cgridO) 
	$(PURIFY) $(CC) $(CCFLAGS) -o $@ $(cgridO) \
        $(CLIBS) $(OvertureCad)/rap.a $(FLIBS) $(GLIBS)   

tgridO = tgrid.o ingrid.o UnstructuredMapping.o DataFormats.o plotUnstructured.o

tgrid: $(tgridO) 
	$(PURIFY) $(CC) $(CCFLAGS) -o $@ $(tgridO) \
        $(CLIBS) $(OvertureCad)/rap.a $(FLIBS) $(GLIBS)   

rapt:
	@echo $(ogshow) $(mapping)

clean:
	rm -f *.o rap


# here are things we make dependencies for:
SRC = rap.C rapNewModel.C rapEditModel.C rapSurfaceGrids.C rapVolumeGrids.C rapUtil.C Edge.C Point.C \
      rapSimpleGeometry.C nurbsCurveEditor.C linerGeometry.C buildLiner.C SphereLoading.C \
      fillVolumeWithSpheres.C RandomSampling.C

# to remake the dependencies:  make dep
dep: 
	$(Overture)/config/makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

check:
	perl check.p

heal:
	perl heal.p

# include the dependencies here

.PRECIOUS:
