# Makefile for the Cgins documentation


# User Guide
# obFiles = $(OverBlown)/OverBlown.C $(OverBlown)/setParameters.C $(OverBlown)/getInitialConditions.C \
#           $(OverBlown)/OB_CompositeGridSolver.C $(OverBlown)/OB_Parameters.C
CginsUserGuide: CginsUserGuide.tex $(CginsFiles)
	extract.p $(CginsFiles)
	latex CginsUserGuide
	makeindex CginsUserGuide.idx
	dvips -P pdf -G0 -o CginsUserGuide.ps CginsUserGuide.dvi
	ps2pdf CginsUserGuide.ps
#	mv CginsUserGuide.pdf $(WEBPAGE)/documentation

# Reference manual
CginsRef: CginsRef.tex
	latex CginsRef
	makeindex CginsRef.idx
	bibtex CginsRef
	latex CginsRef
	dvips -P pdf -G0 -o CginsRef.ps CginsRef.dvi
	ps2pdf CginsRef.ps
#	mv CginsRef.pdf $(WEBPAGE)/documentation

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc

