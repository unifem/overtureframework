# Makefile for the Cgcns documentation


# User Guide
# obFiles = $(OverBlown)/OverBlown.C $(OverBlown)/setParameters.C $(OverBlown)/getInitialConditions.C \
#           $(OverBlown)/OB_CompositeGridSolver.C $(OverBlown)/OB_Parameters.C
CgcnsUserGuide: CgcnsUserGuide.tex $(CgcnsFiles)
	extract.p $(CgcnsFiles)
	latex CgcnsUserGuide
	makeindex CgcnsUserGuide.idx
	dvips -P pdf -G0 -o CgcnsUserGuide.ps CgcnsUserGuide.dvi
	ps2pdf CgcnsUserGuide.ps
#	mv CgcnsUserGuide.pdf $(WEBPAGE)/documentation

# Reference Guide
CgcnsRef: CgcnsRef.tex
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
