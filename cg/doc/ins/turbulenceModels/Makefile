# Temporary doc for turbulence models (A shorter version)
tm: tm.tex
	latex tm
	makeindex tm.idx
	bibtex tm
	latex tm
	dvips -P pdf -G0 -o tm.ps tm.dvi
	ps2pdf tm.ps