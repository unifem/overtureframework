#
#   SM: Solid mechanics code
#

user: CgsmUserGuide.tex $(CgsmFiles)
	pdflatex CgsmUserGuide
	bibtex CgsmUserGuide
#	makeindex CgsmUserGuide.idx

# Reference manual
ref: CgsmRef.tex
	pdflatex CgsmRef
	pdflatex CgsmRef
	makeindex CgsmRef.idx
	bibtex CgsmRef
#	makeindex CgsmRef.idx


getBib:
	ln -sf /data2/henshw/papers/henshaw.bib .
	ln -sf /data2/henshw/papers/henshawPapers.bib .


# Notes for Heinz:
eeNotes : eeNotes.tex
	latex eeNotes
	dvips -P pdf -G0 -o eeNotes.ps eeNotes.dvi
	ps2pdf eeNotes.ps



notes : notes.tex
	latex notes
	dvips -P pdf -G0 -o notes.ps notes.dvi
	ps2pdf notes.ps

fos : fos.tex
	latex fos
	dvips -P pdf -G0 -o fos.ps fos.dvi
	ps2pdf fos.ps

nonlinear : nonlinear.tex
	pdflatex nonlinear
	bibtex nonlinear

#	latex nonlinear
#	dvips -P pdf -G0 -o nonlinear.ps nonlinear.dvi
#	ps2pdf nonlinear.ps

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps



.PRECIOUS: 

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
