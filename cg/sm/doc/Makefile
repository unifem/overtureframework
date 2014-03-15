#
#   SM: Solid mechanics code
#

CgsmUserGuide : CgsmUserGuide.tex
	latex CgsmUserGuide
	dvips -P pdf -G0 -o CgsmUserGuide.ps CgsmUserGuide.dvi
	ps2pdf CgsmUserGuide.ps

notes : notes.tex
	latex notes
	dvips -P pdf -G0 -o notes.ps notes.dvi
	ps2pdf notes.ps

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps



.PRECIOUS: 

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
