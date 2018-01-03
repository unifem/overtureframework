# Makefile for the Cgcns documentation


all = user ref

# User Guide
# obFiles = $(OverBlown)/OverBlown.C $(OverBlown)/setParameters.C $(OverBlown)/getInitialConditions.C \
#           $(OverBlown)/OB_CompositeGridSolver.C $(OverBlown)/OB_Parameters.C
user: CgcnsUserGuide.tex $(CgcnsFiles)
	extract.p $(CgcnsFiles)
	pdflatex CgcnsUserGuide
	bibtex CgcnsUserGuide
	pdflatex CgcnsUserGuide
	makeindex CgcnsUserGuide.idx

#	mv CgcnsUserGuide.pdf $(WEBPAGE)/documentation
userOld: CgcnsUserGuide.tex $(CgcnsFiles)
	extract.p $(CgcnsFiles)
	latex CgcnsUserGuide
	makeindex CgcnsUserGuide.idx
	dvips -P pdf -G0 -o CgcnsUserGuide.ps CgcnsUserGuide.dvi
	ps2pdf CgcnsUserGuide.ps
#	mv CgcnsUserGuide.pdf $(WEBPAGE)/documentation

# Reference Guide
ref: CgcnsRef.tex
	pdflatex CgcnsRef
	bibtex CgcnsRef
	bibtex bu1
	bibtex bu2
	pdflatex CgcnsRef
	makeindex CgcnsRef.idx


refOld: CgcnsRef.tex
	latex CgcnsRef
	makeindex CgcnsRef.idx
	bibtex CgcnsRef
	latex CgcnsRef
	dvips -P pdf -G0 -o CgcnsRef.ps CgcnsRef.dvi
	ps2pdf CgcnsRef.ps
#	mv CgcnsRef.pdf $(WEBPAGE)/documentation

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
