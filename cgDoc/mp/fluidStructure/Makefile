# Makefile for fsm documentation


# Notes on solving fluid solid-mechanics problems
fsm: fsm.tex
	latex fsm
	# bibtex fsm
	latex fsm
	dvips -P pdf -G0 -o fsm.ps fsm.dvi
	ps2pdf fsm.ps


fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc

