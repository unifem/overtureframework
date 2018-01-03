# Makefile for the Cgasf documentation


all = Cgasf
Cgasf: Cgasf.tex
	latex Cgasf
	dvips -P pdf -G0 -o Cgasf.ps Cgasf.dvi
	ps2pdf Cgasf.ps


reactions:
	latex reactions.tex
	dvips -P pdf -G0 -o reactions.ps reactions.dvi
	ps2pdf reactions.ps

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
