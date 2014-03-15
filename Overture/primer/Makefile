include ../config/Makefile.$(VENDOR)

# over-ride defaults
# CC= insure
# CCFLAGS= $(CCFLAGSO)
# CCFLAGS= $(CCFLAGSF)
# PURIFY  = purify $(PFLAGS)

current = .
mapping = $(current)/../mapping
ogshow = $(current)/../ogshow

VPATH = $(mapping):$(ogshow)

all = mappedGridExample1 mappedGridExample2 mappedGridExample3 mappedGridExample4 mappedGridExample5 mappedGridExample6 \
      example1 example2 example3 example4 example5 example6 example7 example8 example9  example10 wave \
      move1 amrExample1 mgExample1 mgExample2 gridGenExample
all: $(all);

primer:
	latex primer
	bibtex primer
	makeindex primer
	latex primer
	dvips  -P pdf -G0 -o primer.ps primer.dvi 
	ps2pdf primer.ps

primerhtml:
	lh.p primer.tex
	latex primerHTML
	bibtex primerHTML
	makeindex primerHTML
	latex primerHTML
	latex2html -local_icons primerHTML

doc: primer

dochtml: primerhtml
	latex primer
	makeindex primer
	bibtex primer
	latex primer
	latex primer
	dvips  -P pdf -G0 -o primer.ps primer.dvi 
	ps2pdf primer.ps
	mv primer.pdf $(WEBPAGE)/documentation
	rm -rf $(WEBPAGE)/documentation/primerHTML
	# mkdir $(WEBPAGE)/documentation/primerHTML
	cp -r primerHTML $(WEBPAGE)/documentation
	rm -rf primerHTML

linkFiles:
	ln -sf $(OvertureCheckout)/gf/ParallelOverlappingGridInterpolator.h .
	ln -sf $(OvertureCheckout)/gf/ParallelOverlappingGridInterpolator.C .

# NOTE: to check for P++ arrays created statically, use gdb and break in 'doubleArray::doubleArray()'
ex = ex.o
ex: $(ex) 
	$(PURIFY) $(CC) $(CCFLAGS) -o ex $(ex) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

heatSolver = heatSolver.o
heatSolver: $(heatSolver) 
	$(CC) $(CCFLAGS) -o heatSolver $(heatSolver) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

mappedGridExample1 = mappedGridExample1.o
mappedGridExample1: $(mappedGridExample1) 
	$(CC) $(CCFLAGS) -o mappedGridExample1 $(mappedGridExample1) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

mappedGridExample1cc = mappedGridExample1cc.o ../grid/MappedGridGeometry1.o ../gf/doubleMappedGridFunction.o
mappedGridExample1cc: $(mappedGridExample1cc) 
	$(CC) $(CCFLAGS) -o mappedGridExample1cc $(mappedGridExample1cc) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

mge2 = mge2.o
mge2: $(mge2) 
	$(CC) $(CCFLAGS) -o mge2 $(mge2) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

mappedGridExample2 = mappedGridExample2.o
mappedGridExample2: $(mappedGridExample2) 
	$(CC) $(CCFLAGS) -o mappedGridExample2 $(mappedGridExample2) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

u_mappedGridExample2 = u_mappedGridExample2.o
u_mappedGridExample2: $(u_mappedGridExample2) 
	$(CC) $(CCFLAGS) -o u_mappedGridExample2 $(u_mappedGridExample2) \
        $(CLIBS) $(FLIBS)  $(GLIBS)

mappedGridExample3 = mappedGridExample3.o 
mappedGridExample3: $(mappedGridExample3) 
	$(CC) $(CCFLAGS) -o mappedGridExample3 $(mappedGridExample3) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

mappedGridExample3CC = mappedGridExample3CC.o
mappedGridExample3CC: $(mappedGridExample3CC) 
	$(CC) $(CCFLAGS) -o mappedGridExample3CC $(mappedGridExample3CC) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

mappedGridExample4 = mappedGridExample4.o ChannelMapping.o
mappedGridExample4: $(mappedGridExample4) 
	$(CC) $(CCFLAGS) -o mappedGridExample4 $(mappedGridExample4) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

mappedGridExample5 = mappedGridExample5.o
mappedGridExample5: $(mappedGridExample5) 
	$(CC) $(CCFLAGS) -o mappedGridExample5 $(mappedGridExample5) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

mappedGridExample6 = mappedGridExample6.o getDt.o
mappedGridExample6: $(mappedGridExample6) 
	$(CC) $(CCFLAGS) -o mappedGridExample6 $(mappedGridExample6) \
        $(CLIBS) $(FLIBS) $(GLIBS) 


example1 = example1.o
example1: $(example1) 
	$(CC) $(CCFLAGS) -o example1 $(example1) $(CLIBS) $(FLIBS)  $(GLIBS)

example2 = example2.o
example2: $(example2) 
	$(CC) $(CCFLAGS) -o example2 $(example2) $(CLIBS) $(FLIBS)  $(GLIBS)

example3 = example3.o
example3: $(example3) 
	$(CC) $(CCFLAGS) -o example3 $(example3) $(CLIBS) $(FLIBS) $(GLIBS) 

example4 = example4.o
example4: $(example4) 
	$(CC) $(CCFLAGS) -o example4 $(example4) $(CLIBS) $(FLIBS) $(GLIBS) 

example5 = example5.o
example5: $(example5) 
	$(CC) $(CCFLAGS) -o example5 $(example5) $(CLIBS) $(FLIBS) $(GLIBS) 

example6 = example6.o
example6: $(example6) 
	$(CC) $(CCFLAGS) -o example6 $(example6) $(CLIBS) $(FLIBS)  $(GLIBS)

example6a = example6a.o
example6a: $(example6a) 
	$(CC) $(CCFLAGS) -o example6a $(example6a) $(CLIBS) $(FLIBS)  $(GLIBS)

example7 = example7.o 
example7: $(example7) 
	$(CC) $(CCFLAGS) -o example7 $(example7) $(CLIBS) $(FLIBS) $(GLIBS) 

example8 = example8.o 
example8: $(example8) 
	$(CC) $(CCFLAGS) -o example8 $(example8) $(CLIBS) $(FLIBS) $(GLIBS)

example9 = example9.o
example9: $(example9) 
	$(CC) $(CCFLAGS) -o example9 $(example9) $(CLIBS) $(FLIBS) $(GLIBS)

example10 = example10.o 
example10: $(example10) 
	$(CC) $(CCFLAGS) -o example10 $(example10) $(CLIBS) $(FLIBS) $(GLIBS)


gridGenExample = gridGenExample.o 
gridGenExample: $(gridGenExample) 
	$(CC) $(CCFLAGS) -o gridGenExample $(gridGenExample) $(CLIBS) $(FLIBS) $(GLIBS)

# gridGenExample2 = gridGenExample2.o 
# gridGenExample2: $(gridGenExample2) 
# 	$(CC) $(CCFLAGS) -o gridGenExample2 $(gridGenExample2) $(CLIBS) $(FLIBS) $(GLIBS)

gridGenExample_backup = gridGenExample_backup.o 
gridGenExample_backup: $(gridGenExample_backup) 
	$(CC) $(CCFLAGS) -o gridGenExample_backup $(gridGenExample_backup) $(CLIBS) $(FLIBS) $(GLIBS)

# 2d wave equation
wave.o : wave.C; $(CC) $(CCFLAGSF) -c $*.C
wave = wave.o getDt.o
# wave = wave.o getDt.060830.o
wave: $(wave) 
	$(CC) $(CCFLAGS) -o wave $(wave) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

# Parallel wave equation solver
# Compile optimized:
pwave.o : pwave.C; $(CC) $(CCFLAGSG) -c $*.C

pwave = pwave.o getDt.o ParallelOverlappingGridInterpolator.o AnnulusMapping.o contourOpt.o contour.o \
        MappedGridOperators.o MappedGridGeometry2.o Mapping.o
pwave = pwave.o getDt.o ParallelOverlappingGridInterpolator.o AnnulusMapping.o  \
        MappedGridOperators.o MappedGridGeometry2.o
pwave = pwave.o getDt.o ../gf/ParallelOverlappingGridInterpolator.o 
pwave = pwave.o getDt.o contour.o MappedGrid.o ParallelUtility.o CompositeGrid.o
pwave = pwave.o getDt.o 
pwave: $(pwave) 
	$(CC) $(CCFLAGS) -o pwave $(pwave) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

# P++ wave equation solver:
pppWave = pppWave.o 
pppWave: $(pppWave) 
	$(CC) $(CCFLAGS) -o pppWave $(pppWave) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

# *** Bug when an array dimension gets too big ***
pTest4 = pTest4.o 
pTest4: $(pTest4) 
	$(CC) $(CCFLAGS) -o pTest4 $(pTest4) \
        $(FLIBS) $(GLIBS) 


pTest1 = pTest1.o MappedGridGeometry2.o AnnulusMapping.o Mapping.o
pTest1: $(pTest1) 
	$(CC) $(CCFLAGS) -o pTest1 $(pTest1) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

pTest2 = pTest2.o
pTest2: $(pTest2) 
	$(CC) $(CCFLAGS) -o pTest2 $(pTest2) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

pTest3 = pTest3.o AnnulusMapping.o  Mapping.o
pTest3: $(pTest3) 
	$(CC) $(CCFLAGS) -o pTest3 $(pTest3) \
        $(CLIBS) $(FLIBS) $(GLIBS) 

# A moving grid example
move1 = move1.o 
move1: $(move1) 
	$(CC) $(CCFLAGS) -o move1 $(move1) $(CLIBS) $(FLIBS) $(GLIBS)

# A deforming grid example
deform = deform.o 
deform: $(deform) 
	$(CC) $(CCFLAGS) -o deform $(deform) $(CLIBS) $(FLIBS) $(GLIBS)

# Build an airfoil with the hyperolic grid generator (exmaple for Dominic)
# also changed HyperbolicMapping.{h,C}
airfoil = airfoil.o ../hype/update.o ../hype/HyperbolicMapping.o ../mapping/AirfoilMapping.o
airfoil = airfoil.o 
airfoil: $(airfoil) 
	$(CC) $(CCFLAGS) -o airfoil $(airfoil) $(CLIBS) $(FLIBS) $(GLIBS)

# Example from Kara-Lee Maki:
movetoy = movetoy.o 
movetoy: $(movetoy) 
	$(CC) $(CCFLAGS) -o movetoy $(movetoy) $(CLIBS) $(FLIBS) $(GLIBS)


# amr example
amrExample1 = amrExample1.o 
amrExample1: $(amrExample1) 
	$(CC) $(CCFLAGS) -o amrExample1 $(amrExample1) $(CLIBS) $(FLIBS) $(GLIBS)

# hyperbolic amr example
amrHype= amrHype.o getDt.o 
amrHype: $(amrHype) 
	$(CC) $(CCFLAGS) -o amrHype $(amrHype) $(CLIBS) $(FLIBS) $(GLIBS)

amrh= amrHype.o getDt.o 
amrh: $(amrh) 
	$(CC) $(CCFLAGS) -o amrh $(amrh) $(CLIBS) $(FLIBS) $(GLIBS)


# mg example
mgExample1 = mgExample1.o 
mgExample1: $(mgExample1) 
	$(CC) $(CCFLAGS) -o mgExample1 $(mgExample1) $(CLIBS) $(FLIBS) $(GLIBS)

a = a.o 
a: $(a) 
	$(CC) $(CCFLAGS) -o a $(a) $(CLIBS) $(FLIBS) $(GLIBS)

# exmaple from Kara:
PoissonND = PoissonND.o 
PoissonND: $(PoissonND) 
	$(CC) $(CCFLAGS) -o PoissonND $(PoissonND) $(CLIBS) $(FLIBS) $(GLIBS)

# mg example
mgExample2 = mgExample2.o 
mgExample2: $(mgExample2) 
	$(CC) $(CCFLAGS) -o mgExample2 $(mgExample2) $(CLIBS) $(FLIBS) $(GLIBS)

mgex2 = mgex2.o 
mgex2: $(mgex2) 
	$(CC) $(CCFLAGS) -o mgex2 $(mgex2) $(CLIBS) $(FLIBS) $(GLIBS)

# Test routines 
testInterpolant = testInterpolant.o 
testInterpolant: $(testInterpolant) 
	$(CC) $(CCFLAGS) -o testInterpolant $(testInterpolant) $(CLIBS) $(FLIBS) $(GLIBS)

# Test routines for coefficient matrices, see the operator documentation
tcm = tcm.o 
tcm: $(tcm) 
	$(CC) $(CCFLAGS) -o tcm $(tcm) $(CLIBS) $(FLIBS) $(GLIBS)

tcm2 = tcm2.o 
tcm2: $(tcm2) 
	$(CC) $(CCFLAGS) -o tcm2 $(tcm2) $(CLIBS) $(FLIBS) $(GLIBS)

tcm3 = tcm3.o dlapcg.o dgmres.o cgesl1234.o blas2.o
tcm3 = tcm3.o $(Overture)/Oges/slapFiles.o $(Overture)/Oges/blas2.o
tcm3 = tcm3.o $(Overture)/Oges/slapFiles.o $(Overture)/Oges/blas2.o
tcm3 = tcm3.o 
tcm3: $(tcm3) 
	$(CC) $(CCFLAGS) -o tcm3 $(tcm3) $(CLIBS) $(FLIBS) $(GLIBS)

tcm4 = tcm4.o 
tcm4: $(tcm4) 
	$(CC) $(CCFLAGS) -o tcm4 $(tcm4) $(CLIBS) $(FLIBS) $(GLIBS)

lins = lins.o 
lins: $(lins) 
	$(CC) $(CCFLAGS) -o lins $(lins) $(CLIBS) $(FLIBS) $(GLIBS)

tbox = tbox.o 
tbox: $(tbox) 
	$(CC) $(CCFLAGS) -o tbox $(tbox) $(CLIBS) $(FLIBS) $(GLIBS)

gridQuery = gridQuery.o 
gridQuery: $(gridQuery) 
	$(CC) $(CCFLAGS) -o gridQuery $(gridQuery) $(CLIBS) $(FLIBS) $(GLIBS)

gridPrint = gridPrint.o 
gridPrint: $(gridPrint) 
	$(CC) $(CCFLAGS) -o gridPrint $(gridPrint) $(CLIBS) $(FLIBS) $(GLIBS)

callingFortran = callingFortran.o mySolver.o
callingFortran: $(callingFortran) 
	$(CC) $(CCFLAGS) -o callingFortran $(callingFortran) $(CLIBS) $(FLIBS) $(GLIBS)

vlasov = vlasov.o vlasovdu.o
vlasov: $(vlasov) 
	$(CC) $(CCFLAGS) -o vlasov $(vlasov) $(CLIBS) $(FLIBS) $(GLIBS)


clean:  
	rm -f *.o mappedGridExample1 mappedGridExample2 mappedGridExample3 mappedGridExample4 mappedGridExample5 \
	   mappedGridExample6 \
           example1 example2 example3 example4 example5 example6 example7 example8 example9 example10 tcm tcm2 tcm3 tcm4 \
           move1 primer.show move1.show amrExample1 mappedGridExample1cc mgExample1 mgExample2 lins example6a wave \
           amrHype callingFortran gridGenExample pwave pTest1 pTest2 pTest3 deform

.PRECIOUS: 
