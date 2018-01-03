# 
vp: vp.tex
	latex vp
	bibtex vp
	latex vp
	dvips -P pdf -G0 -o vp.ps vp.dvi
	ps2pdf vp.ps

fig: fig.tex
	latex fig
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps
