# cgDoc/cg Makefile 



cgDevelopersGuide:
	latex cgDevelopersGuide.tex
	dvips -P pdf -G0 -o cgDevelopersGuide.ps cgDevelopersGuide.dvi
	ps2pdf cgDevelopersGuide.ps

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
