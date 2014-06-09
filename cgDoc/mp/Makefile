# Makefile for the mp documentation


# User Guide and Reference manual
mp: mp.tex
	latex mp
	makeindex mp.idx
	bibtex mp
	latex mp
	dvips -P pdf -G0 -o mp.ps mp.dvi
	ps2pdf mp.ps
#	mv mp.pdf $(WEBPAGE)/documentation


# mp reference guide
ref: ref.tex
	latex ref
	makeindex ref.idx
	bibtex ref
	latex ref
	dvips -P pdf -G0 -o ref.ps ref.dvi
	ps2pdf ref.ps

# Notes on solving multi-domain problems
multiDomain: multiDomain.tex
	latex multiDomain
	makeindex multiDomain.idx
	bibtex multiDomain
	latex multiDomain
	dvips -P pdf -G0 -o multiDomain.ps multiDomain.dvi
	ps2pdf multiDomain.ps


# Notes on solving fluid solid-mechanics problems
fsm: fluidStructure/fsm.tex
	cd fluidStructure; make fsm


fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc

