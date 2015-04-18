include ../config/Makefile.$(VENDOR)

current = .
mapping = $(current)/../mapping
ogshow = $(current)/../ogshow
hype = $(current)/../hype
grid = $(current)/../grid

VPATH = $(mapping):$(ogshow):$(hype):$(grid)


all = dia
all: $(all);

extractFiles = grid.C PlotStuff.C plotCompositeSurface.C plotUnstructured.C plotStructured.C \
               plotMapping.C plotAdvancingFront.C GL_GraphicsInterface.C label.C plotAxes.C \
               colourTable.C osRender.C \
               GenericGraphicsInterface.C GenericGraphicsInterface.h GraphicsParameters.C NameList.C \
               Ogshow.C ShowFileReader.C plotPoints.C ColourBar.C GL_GraphicsInterface.h contour.C \
               streamLines.C getAnswer.C textGetMenu.C GUIState.C DialogData.C mogl.C 


GraphicsDoc: GraphicsDoc.dvi 
	dvips -P pdf -G0 -o GraphicsDoc.ps GraphicsDoc.dvi
	ps2pdf GraphicsDoc.ps
	mv GraphicsDoc.pdf $(WEBPAGE)/documentation

GraphicsDoc.pdf: GraphicsDoc.dvi 
	dvips -P pdf -G0 -o GraphicsDoc.ps GraphicsDoc.dvi
	ps2pdf GraphicsDoc.ps

GraphicsDoc.dvi: GraphicsDoc.tex $(extractFiles)
	rm -f GraphicsDoc.ind
	extract.p $(extractFiles)
	latex GraphicsDoc.tex
	makeindex GraphicsDoc.idx
	latex GraphicsDoc.tex

# The following might not work unless you have all the style files that Anders has in ~andersp/tex/style
#
# If you want to use pdflatex on some other latex document, you might want to look in GraphicsDoc.tex
# to see what needs to be done. In particular, note that you need to convert all postscript figures to 
# the png format before you run pdflatex. 
# For this purpose you can use the script /home/andersp/bin/ps2png

GraphicsDocHtml: $(extractFiles)
	rm -f GraphicsDocHTML.ind
	lh.p GraphicsDoc.tex
	latex GraphicsDocHTML
#	bibtex GraphicsDocHTML
	makeindex GraphicsDocHTML.idx
	latex GraphicsDocHTML
	makeindex GraphicsDocHTML.idx
	rm -rf GraphicsDocHTML
	latex2html GraphicsDocHTML
	rm -rf $(WEBPAGE)/documentation/GraphicsDocHTML
	mkdir $(WEBPAGE)/documentation/GraphicsDocHTML
	cp GraphicsDocHTML/*.* $(WEBPAGE)/documentation/GraphicsDocHTML
#	rm -rf GraphicsDocHTML

Ogshow: 
	extract.p Ogshow.C ShowFileReader.C
	latex ogshow.tex
	latex ogshow.tex
	makeindex ogshow.idx
	dvips -P pdf -G0 -o ogshow.ps ogshow.dvi 
	ps2pdf ogshow.ps
	mv ogshow.pdf $(WEBPAGE)/documentation

Ogshowhtml: 
	lh.p ogshow.tex
	latex ogshowHTML
#	bibtex ogshowHTML
	makeindex ogshowHTML.idx
	latex ogshowHTML
	latex2html -local_icons ogshowHTML
	rm -rf $(WEBPAGE)/documentation/ogshowHTML
	mkdir $(WEBPAGE)/documentation/ogshowHTML
	cp ogshowHTML/*.* $(WEBPAGE)/documentation/ogshowHTML
	rm -rf ogshowHTML

plotStuffDoc: 
	extract.p DerivedFunctions.C plotStuff.C userDefinedDerivedFunction.C
	latex plotStuff.tex
	makeindex plotStuff.idx
	bibtex plotStuff
	latex plotStuff.tex
	dvips -P pdf -G0 -o plotStuff.ps plotStuff.dvi 
	ps2pdf plotStuff.ps
	cp plotStuff.pdf $(WEBPAGE)/documentation

plotStuffDochtml: 
	lh.p plotStuff.tex
	latex plotStuffHTML
#	bibtex plotStuffHTML
	makeindex plotStuffHTML.idx
	latex plotStuffHTML
	rm -rf plotStuffHTML
	latex2html -local_icons plotStuffHTML
	rm -rf $(WEBPAGE)/documentation/plotStuffHTML
	mkdir $(WEBPAGE)/documentation/plotStuffHTML
	cp plotStuffHTML/*.* $(WEBPAGE)/documentation/plotStuffHTML
	rm -rf plotStuffHTML



# doc:  GraphicsDoc GraphicsDocHtml Ogshow Ogshowhtml plotStuffDoc plotStuffDochtml
doc:  GraphicsDoc Ogshow plotStuffDoc

perlex1.o: perlex1.C
	$(CC) -I/usr/local/include \
               -I/usr/lib/perl5/5.6.1/i386-linux/CORE \
               -L/usr/lib/perl5/5.6.1/i386-linux/CORE \
               -c perlex1.C

# PERLIB = /usr/lib/perl5/5.6.1/i386-linux/CORE
PERLIB = $(OV_PERL_LIB)
perlex1: perlex1.o
	$(CC) -I/usr/local/include -I$(PERLIB) -L$(PERLIB) \
               -o perlex1 perlex1.o -lperl -lcrypt -lm  

ovp.o : ovp.C
	$(CC) $(CCFLAGS) -I$(PERLIB) -c ovp.C
OvertureParser.o : OvertureParser.C
	$(CC) $(CCFLAGS) -I$(PERLIB) -c OvertureParser.C

# These files need to include the perl directory
getAnswer.o : getAnswer.C
	$(CC) $(CCFLAGS) -I$(PERLIB) -c getAnswer.C
textGetMenu.o : textGetMenu.C
	$(CC) $(CCFLAGS) -I$(PERLIB) -c textGetMenu.C
GenericGraphicsInterface.o : GenericGraphicsInterface.C
	$(CC) $(CCFLAGS) -I$(PERLIB) -c GenericGraphicsInterface.C

ovp = ovp.o OvertureParser.o textGetMenu.o getAnswer.o 
ovp: $(ovp) 
	$(CC) $(CCFLAGS) -I$(PERLIB)  -o $@ $(ovp) \
        $(CLIBS) $(FLIBS) $(GLIBS) -L$(PERLIB) -lperl -lcrypt -lm 


# Tests 
togshow =  togshow.o Ogshow.o ../hdf/HDF_DataBase.o
togshow: $(togshow) 
	$(CC) $(CCFLAGS) -o togshow \
        $(togshow) $(CLIBS) $(FLIBS) $(GLIBS)

bug =  bug.o $(Overture)/Overture/MappedGrid.o ../hdf/HDF_DataBase.o
bug: $(bug) 
	$(CC) $(CCFLAGS) -o bug \
        $(bug) $(CLIBS) $(FLIBS) $(GLIBS)

# test NameList
nl = nl.o NameList.o
nl: $(nl)
	$(CC) $(CCFLAGS) -o nl $(nl) $(CLIBS) $(FLIBS) $(GLIBS) 

tn= tn.o
tn: $(tn)
	$(CC) $(CCFLAGS) -o tn $(tn) $(CLIBS) $(FLIBS) $(GLIBS) 

# test moving grids in show file
tmg = tmg.o
tmg: $(tmg)
	$(CC) $(CCFLAGS) -o tmg $(tmg) $(CLIBS) $(FLIBS) $(GLIBS) 

tbug: tbug.o  
	$(CC) $(CCFLAGS) $(Include) -o tbug \
        tbug.o $(CLIBS) $(FLIBS) 

bits: bits.o  
	$(CC) $(CCFLAGS) $(Include) -o bits \
        bits.o $(CLIBS) $(FLIBS) $(GLIBS)

paperplane = paperplane.o
paperplane : $(paperplane)
	 $(cc) $(ccFLAGS) -o paperplane $(paperplane)  $(GLIBS)

pixmap2eps = pixmap2eps.o
pixmap2eps : $(pixmap2eps)
	 $(cc) $(ccFLAGS) -o pixmap2eps $(pixmap2eps)  -L$(OpenGL)/lib  $(GL_LIBS) \
           -lXm -lXt -lXmu -lXi -lXext -lX11 -lm

# test routine for mogl
gi = giMain.o mogl.o overlay.o 
gi : $(gi)
	 $(CC) $(CCFLAGS) -o gi $(gi) $(CLIBS) $(FLIBS) $(GLIBS)

gi2 = gi2.o mogl.o overlay.o 
gi2 : $(gi2)
	 $(CC) $(CCFLAGS) -o gi2 $(gi2) $(CLIBS) $(FLIBS) $(GLIBS)

gi0 = giMain0.o mogl0.o overlay.o 
gi0 : $(gi0)
	 $(CC) $(CCFLAGS) -o gi0 $(gi0) $(GLIBS)

## build gi2 without linking in full Overture -- for testing mogl.C... **pf
giPlain =  gi2.o GUIState.o DialogData.o mogl.o overlay.o $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
giPlain: $(giPlain)
	 $(CC) -g -o giPlain $(giPlain) $(FLIBS) $(GLIBS)

grid3d.o: grid3d.C
	$(CC) $(CCFLAGS) -c grid3d.C

plotMapping.o: plotMapping.C
	$(CC) $(CCFLAGSF) -c $*.C

contour.o: contour.C
	$(CC) $(CCFLAGS) -c $*.C

contourOpt.o: contourOpt.C
	$(CC) $(CCFLAGS) -c $*.C

contour3dOpt.o: contour3dOpt.C
	$(CC) $(CCFLAGSF) -c $*.C

gridOpt.o: gridOpt.C
	$(CC) $(CCFLAGS) -c $*.C

GridStatistics.o: GridStatistics.C
	$(CC) $(CCFLAGSG) -c $*.C

contourTest.o: contourTest.C
	$(CC) $(CCFLAGS) -c contourTest.C

isosurf.o: isosurf.f
	$(FC) $(FFLAGSF) -c $*.f

cggi.o: cggi.f
	$(FC) $(FFLAGSF) -c $*.f

streamLines.o: streamLines.C
	$(CC) $(CCFLAGSO) -c $*.C

streamLinesOpt.o: streamLinesOpt.C
	$(CC) $(CCFLAGSF) -c $*.C

xInterpolate.o: xInterpolate.C
	$(CC) $(CCFLAGSF) -c $*.C

contourOpt.C: contourOpt.bC; bpp contourOpt.bC
gridOpt.C: gridOpt.bC; bpp gridOpt.bC
grid3d.C: grid3d.bC; bpp grid3d.bC
GridStatistics.C: GridStatistics.bC; bpp GridStatistics.bC
ParallelUtility.C: ParallelUtility.bC; bpp ParallelUtility.bC

mogl.o: mogl.C
	$(CC) $(CCFLAGS)  -I$(Overture)/static -c $*.C


# Main test routine
pl = pl.o grid.o gridStatistics.o gridOpt.o
pl = pl.o contour.o contour3d.o GridStatistics.o grid.o
pl = pl.o GL_GraphicsInterface.o textGetMenu.o getAnswer.o
pl = pl.o grid.o GridStatistics.o
pl = pl.o grid.o gridOpt.o grid3d.o plotGridQuality.o
pl = pl.o plotPoints.o
pl: $(pl) 
	$(CC) $(CCFLAGS) -o $@ $(pl) \
        $(CLIBS) $(FLIBS) $(GLIBS)   

# fancy fonts
tf = tf.o 
tf: $(tf) 
	$(CC) $(CCFLAGS) -o $@ $(tf) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

ftgl.o: ftgl.C; $(CC) $(CCFLAGS) -I/home/henshaw/junk/FTGL/include -I/usr/include/freetype2 -c ftgl.C
ftgl = ftgl.o
ftgl: $(ftgl) 
	$(CC) $(CCFLAGS) -o $@ $(ftgl) \
        $(CLIBS) $(FLIBS) /home/henshaw/junk/FTGL/unix/src/.libs/libftgl.a   -L/usr/X11R6/lib /usr/lib/libfreetype.so $(GLIBS)   

demo.o: demo.C; $(CC) $(CCFLAGS) -I/usr/casc/overture/OpenGL/glut-3.6/include -I/home/henshaw/junk/FTGL/include -I/usr/include/freetype2 -c demo.C
demo = demo.o
demo: $(demo) 
	$(CC) $(CCFLAGS) -o $@ $(demo) \
        $(CLIBS) $(FLIBS) -L/usr/casc/overture/OpenGL/glut-3.6/lib/glut -lglut /home/henshaw/junk/FTGL/unix/src/.libs/libftgl.a   -L/usr/X11R6/lib /usr/lib/libfreetype.so $(GLIBS)   


FTGLDemo.o: FTGLDemo.C; $(CC) $(CCFLAGS) -I/usr/casc/overture/OpenGL/glut-3.6/include -I/home/henshaw/junk/FTGL/include -I/usr/include/freetype2 -c FTGLDemo.C
FTPolyGlyph.o: FTPolyGlyph.C; $(CC) $(CCFLAGS) -I/usr/casc/overture/OpenGL/glut-3.6/include -I/home/henshaw/junk/FTGL/include -I/usr/include/freetype2 -c FTPolyGlyph.C
FTGLDemo = FTGLDemo.o FTPolyGlyph.o \
           /home/henshaw/junk/FTGL/unix/demo/tb.o /home/henshaw/junk/FTGL/unix/demo/trackball.o
FTGLDemo: $(FTGLDemo) 
	$(CC) $(CCFLAGS) -o $@ $(FTGLDemo) \
        $(CLIBS) $(FLIBS) -L/usr/casc/overture/OpenGL/glut-3.6/lib/glut -lglut /home/henshaw/junk/FTGL/unix/src/.libs/libftgl.a   -L/usr/X11R6/lib /usr/lib/libfreetype.so $(GLIBS)   


testnc = testnc.o 
testnc: $(testnc) 
	$(CC) $(CCFLAGS) -o $@ $(testnc) \
        $(CLIBS) $(FLIBS) $(Overture)/Cad/rap.a $(GLIBS)   

# test routine for Diana
plt = plt.o
plt: $(plt) 
	$(CC) $(CCFLAGS) -o $@ $(plt) \
        $(CLIBS) $(FLIBS) $(GLIBS)   

map= map.o 
map: $(map) 
	$(CC) $(CCFLAGS) -o $@ $(map) $(CLIBS) $(FLIBS) $(GLIBS)

small-GI-test = small-GI-test.o PlotStuff.o GL_GraphicsInterface.o GUIState.o mogl.o grid.o contour.o \
        contour3d.o label.o plotPoints.o xInterpolate.o plotCompositeSurface.o plotStructured.o plotMapping.o
small-GI-test = small-GI-test.o
small-GI-test: $(small-GI-test) 
	$(CC) $(CCFLAGS) -o $@ $(small-GI-test) \
        $(CLIBS) $(FLIBS) $(GLIBS)   

triplot = triplot.o
triplot: $(triplot) 
	$(CC) $(CCFLAGS) -o $@ $(triplot) \
        -L$(OvertureLib) -lRapsodi -lRapsodi_static $(libApp)  \
            -L$(HDF)/lib -lmfhdf -ldf -ljpeg -lz  $(FLIBS) $(GLIBS)   

# Main test routine
dia = dia.o mogl.o
dia: $(dia) 
	$(CC) $(CCFLAGS) -o $@ $(dia) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

// example option menu:
option = option.o
option: $(option) 
	$(CC) $(CCFLAGS) -o $@ $(option) \
        $(CLIBS) $(FLIBS) $(GLIBS) 


tadd3d = tadd3d.o grid.o grid3d.o PlotStuff.o
tadd3d: $(tadd3d)
	$(CC) $(CCFLAGS) -o tadd3d \
        $(tadd3d) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

hybrid = hybrid.o GL_GraphicsInterface.o grid.o plotMapping.o 
hybrid = hybrid.o grid.o plotMapping.o grid3d.o
hybrid = hybrid.o 
hybrid: $(hybrid) 
	$(CC) $(CCFLAGS) -o $@ $(hybrid) \
        $(CLIBS) $(FLIBS) $(GLIBS)

showPlot3d = showPlot3d.o ../mapping/dpm.o ../mapping/DataFormats.o ../mapping/readPlot3d.o
showPlot3d: $(showPlot3d) 
	$(CC) $(CCFLAGS) -o $@ $(showPlot3d) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# convert normal .hdf file to expanded (non-streamming mode) hdf file
decompress = decompress.o
decompress: $(decompress) 
	$(CC) $(CCFLAGS) -o $@ $(decompress) \
        $(CLIBS) $(FLIBS) $(GLIBS)


ps2ppm = ps2ppm.o 
ps2ppm: $(ps2ppm) 
	$(CC) $(CCFLAGS) -o $@ $(ps2ppm) \
        $(CLIBS) $(FLIBS) $(GLIBS)

pk = pk.o GL_GraphicsInterface.o GenericGraphicsInterface.o 
pk = pk.o  GL_GraphicsInterface.o GenericGraphicsInterface.o  ../motif/mogl.o
pk = pk.o  GL_GraphicsInterface.o GraphicsParameters.o plotMapping.o GenericGraphicsInterface.o
pk = pk.o  GL_GraphicsInterface.o GenericGraphicsInterface.o plotMapping.o plotAxes.o mogl.o 
pk: $(pk) 
	$(CC) $(CCFLAGS) -o $@ $(pk) \
        $(CLIBS) $(FLIBS) $(GLIBS)


osRender = osRender.o
osRender: $(osRender) 
	$(CC) $(CCFLAGS) -o $@ $(osRender) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test the show file reader
readShowFile = readShowFile.o ShowFileReader.o
readShowFile: $(readShowFile) 
	$(CC) $(CCFLAGS) -o $@ $(readShowFile) \
        $(CLIBS) $(FLIBS) $(GLIBS)

axes = axes.o label.o contour.o plotAxes.o GL_GraphicsInterface.o GenericGraphicsInterface.o
axes: $(axes) 
	$(CC) $(CCFLAGS) -o $@ $(axes) \
        $(CLIBS) $(FLIBS) $(GLIBS)


plotStuff= plotStuffDriver.o plotStuff.o GenericGraphicsInterface.o GL_GraphicsInterface.o mogl.o plotMapping.o plotAxes.o contour.o contour3d.o streamLines.o streamLines3d.o grid.o grid3d.o GraphicsParameters.o label.o xColours.o plotPoints.o render.o osRender.o plotCompositeSurface.o plotUnstructured.o plotStructured.o PlotStuff.o stroke.o
plotStuff= plotStuffDriver.o plotStuff.o DerivedFunctions.o contour3d.o
plotStuff= plotStuffDriver.o plotStuff.o ShowFileReader.o Ogshow.o
plotStuff= plotStuffDriver.o plotStuff.o ../mapping/AirfoilMapping.o
plotStuff= plotStuffDriver.o plotStuff.o  DerivedFunctions.o 
plotStuff= plotStuffDriver.o plotStuff.o  ShowFileReader.o Ogshow.o 
plotStuff= plotStuffDriver.o plotStuff.o DerivedFunctions.o ShowFileReader.o userDefinedDerivedFunction.o \
             ../gf/Interpolant.o
plotStuff: $(plotStuff) 
	$(CC) $(CCFLAGS) -DADD_USER_MAPPINGS -o $@ $(plotStuff) \
        $(CLIBS) $(FLIBS) $(GLIBS)

surf = testSurf.o PlotStuff.o GL_GraphicsInterface.o plotAxes.o
surf = testSurf.o  PlotStuff.o plotAxes.o GL_GraphicsInterface.o contour.o
testSurf: $(surf) 
	$(CC) $(CCFLAGS) -o testSurf $(surf) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

# Use glut:
plotg = plot.o Glut_GraphicsInterface.o
plotg: $(plotg)
	$(CC) $(CCFLAGS) -o $@ $(plotg)  $(glut)/lib/glut/wdh_event.o \
        $(CLIBS) $(FLIBS) $(GGLIBS)

LLIBS		= -ltk -ltcl -lX11 -lmC
tkh = tkh.o
tkh: $(tkh)
	$(CC) $(CCFLAGS) -o $@ $(tkh)  $(glut)/lib/glut/wdh_event.o \
        $(CLIBS) $(FLIBS) $(GLIBS) -L/usr/local/lib $(LLIBS)

cgshow= Cgshow.o cgshow.o grid.o contour.o colourTable.o label.o
cgshow: $(cgshow)
	$(CC) $(CCFLAGS) -o $@ $(cgshow) $(CLIBS) $(FLIBS) $(GLIBS)

sphere= sphere.o
sphere: $(sphere)
	$(CC) $(CCFLAGS) -o $@ $(sphere) $(glut)/lib/glut/wdh_event.o $(CLIBS) $(FLIBS) $(GLIBS)

teapots= teapots.o
teapots: $(teapots)
	$(CC) $(CCFLAGS) -o $@ $(teapots) $(glut)/lib/glut/wdh_event.o $(CLIBS) $(FLIBS) $(GLIBS)

pick= picksquare.o
pick: $(pick)
	$(CC) $(CCFLAGS) -o $@ $(pick) $(GLIBS)

stroke= stroke.o
stroke: $(stroke)
	$(CC) $(CCFLAGS) -o $@ $(stroke) $(CLIBS) $(FLIBS) $(GLIBS)

gears= gears.o
gears: $(gears)
	$(CC) $(CCFLAGS) -o $@ $(gears) $(CLIBS) $(FLIBS) $(GLIBS) -lglut

xColours= xColours.o
xColours: $(xColours)
	$(CC) $(CCFLAGS) -o $@ $(xColours) $(CLIBS) $(FLIBS) $(GLIBS)

hc= hc.o
hc: $(hc)
	$(CC) $(CCFLAGS) -o $@ $(hc) $(CLIBS) $(FLIBS) $(GLIBS)

# PowerWall versions
VDL_FLAGS  = -I$(VDLHOME)/include -DUSE_POWERWALL
VDL_LIB    = -L$(VDLHOME)/lib -lvdl -lptutil
PW_MOGL_O  = vdl_mogl.o
##PW_MOGL_O  = mogl_special_vdl.o
#PW_MOGL_O = mogl_special_vdl.o  ##A hacked&slashed version
#MOGL_C     = smogl.C
MOGL_C     = mogl.C

#VDL_OVERTURE = vdl_mogl.o vdl_GL_GraphicsInterface.o
VDL_OVERTURE =  $(PW_MOGL_O) vdl_GL_GraphicsInterface.o

vdl_mogl.o: $(MOGL_C)
	$(CC) $(CCFLAGS) $(VDL_FLAGS) -c -o vdl_mogl.o $(MOGL_C)

vdl_GL_GraphicsInterface.o: GL_GraphicsInterface.C
	$(CC) $(CCFLAGS) $(VDL_FLAGS) -c -o vdl_GL_GraphicsInterface.o \
	GL_GraphicsInterface.C

vdl_plotStuff = $(plotStuff) $(VDL_OVERTURE)
vdl_plotStuff: $(vdl_plotStuff)
	$(CC) -g -mt -DADD_USER_MAPPINGS -o $@ \
	$(vdl_plotStuff) \
        $(VDL_LIB) $(CLIBS) $(FLIBS) $(GLIBS) -lpthread

vdl_ogen = $(Overture)/bin/ogen.o $(Overture)/bin/ogenDriver.o $(VDL_OVERTURE) plotCompositeSurface.o
vdl_ogen: $(vdl_ogen)
	$(CC) -g -mt -o $@ \
	$(vdl_ogen) \
        $(VDL_LIB) $(CLIBS) $(FLIBS) $(GLIBS) -lpthread

powerWallTest = small-GI-test.o $(VDL_OVERTURE)
powerWallTest: $(powerWallTest) 
	$(CC) -g -mt  -o $@ $(powerWallTest) \
	$(VDL_LIB) $(CLIBS) $(FLIBS) $(GLIBS)   

#giPowerWall =  giPowerWall.o GUIState.o DialogData.o overlay.o vdl_mogl.o $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
giPowerWall =  giPowerWall.o GUIState.o DialogData.o overlay.o $(PW_MOGL_O) $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
giPowerWall: $(giPowerWall)
	 $(CC) -g -mt -o giPowerWall $(giPowerWall) \
	$(VDL_LIB) $(FLIBS) $(GLIBS)

#simplePW =  simplePW.o GUIState.o DialogData.o overlay.o mogl_special_vdl.o $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
simplePW =  simplePW.o GUIState.o DialogData.o overlay.o $(PW_MOGL_O) $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
simplePW: $(simplePW)
	 $(CC) -g -mt -o simplePW $(simplePW) \
	$(VDL_LIB) $(FLIBS) $(GLIBS) -lpthread -lm

## build gi2 without linking in full Overture -- for testing mogl.C... **pf
giSmogl =  gi2.o GUIState.o DialogData.o mogl.o overlay.o $(Overture)/boxlib/BoxLib.o $(Overture)/boxlib/aString.o $(Overture)/Mapping/sPrintF.o
giSmogl: $(giSmogl)
	 $(CC) -g -o giSmogl $(giSmogl) $(FLIBS) $(GLIBS)


clean:
	rm -f *.o dia


# here are things we make dependencies for:
SRC = ColourBar.C    DialogData.C                GL_GraphicsInterface.C  helpOverture.C  osRender.C        plotPoints.C  textGetMenu.C \
      colourTable.C  GenericGraphicsInterface.C  GraphicsParameters.C    label.C         OvertureParser.C  render.C  \
      dia.C          getAnswer.C

# to remake the dependencies:  make dep
dep: 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend


.PRECIOUS:
