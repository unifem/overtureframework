# Makefile for the Cgins documentation


# User Guide
# obFiles = $(OverBlown)/OverBlown.C $(OverBlown)/setParameters.C $(OverBlown)/getInitialConditions.C \
#           $(OverBlown)/OB_CompositeGridSolver.C $(OverBlown)/OB_Parameters.C

user: CginsUserGuide.tex $(CginsFiles)
	extract.p $(CginsFiles)
	pdflatex CginsUserGuide
	bibtex CginsUserGuide
	makeindex CginsUserGuide.idx

# Reference manual
ref: CginsRef.tex
	pdflatex CginsRef
	makeindex CginsRef.idx
	bibtex CginsRef
#	mv CginsRef.pdf $(WEBPAGE)/documentation

refOldWay: CginsRef.tex
	latex CginsRef
	makeindex CginsRef.idx
	bibtex CginsRef
	latex CginsRef
	dvips -P pdf -G0 -o CginsRef.ps CginsRef.dvi
	ps2pdf CginsRef.ps
#	mv CginsRef.pdf $(WEBPAGE)/documentation

move : movingBodies.tex
	pdflatex movingBodies.tex

# Cgins convergence results: 
conv : conv.tex
	latex conv
	dvips -P pdf -G0 -o conv.ps conv.dvi
	ps2pdf conv.ps

twoPhase : twoPhase.tex
	latex twoPhase
	dvips -P pdf -G0 -o twoPhase.ps twoPhase.dvi
	ps2pdf twoPhase.ps


# temp:
flatPlate:
	pdflatex flatPlate.tex


fig: fig.tex
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc

