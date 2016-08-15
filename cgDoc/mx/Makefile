#g Makefile for the Maxwell Solver documentation


all = CgmxUsersGuide

all: $(all);


user: CgmxUsersGuide
CgmxUsersGuide: CgmxUsersGuide.tex 
	pdflatex CgmxUsersGuide
	bibtex CgmxUsersGuide


ref: CgmxReferenceGuide
CgmxReferenceGuide: CgmxReferenceGuide.tex 
	pdflatex CgmxReferenceGuide
	bibtex CgmxUsersGuide


CgmxUsersGuideOld: CgmxUsersGuide.tex 
	latex CgmxUsersGuide
	dvips -P pdf -G0 -o CgmxUsersGuide.ps CgmxUsersGuide
	ps2pdf CgmxUsersGuide.ps


slac03: slac03.pdf
	acroread -geometry 900x800 slac03.pdf&


slac03.pdf: slac03.tex 
	latex slac03
	dvips -P pdf -G0 -o slac03.ps slac03
	ps2pdf slac03.ps

maxwell: maxwell.tex 
	pdflatex maxwell

maxwellOld: maxwell.tex 
	latex maxwell
	dvips -P pdf -G0 -o maxwell.ps maxwell
	ps2pdf maxwell.ps

modifiedwave.pdf: modifiedwave.tex 
	latex modifiedwave
	dvips -P pdf -G0 -o modifiedwave.ps modifiedwave
	ps2pdf modifiedwave.ps

# parallel performance results
parallel.pdf: parallel.tex 
	latex parallel
	dvips -P pdf -G0 -o parallel.ps parallel
	ps2pdf parallel.ps

stabilizedDSI.pdf: stabilizedDSI.tex 
	latex stabilizedDSI
	dvips -P pdf -G0 -o stabilizedDSI.ps stabilizedDSI
	ps2pdf stabilizedDSI.ps

mx3d: mx3d.tex 
	latex mx3d
	bibtex mx3d
	dvips -P pdf -G0 -o mx3d.ps mx3d
	ps2pdf mx3d.ps

mxCompare: mxCompare.tex 
	latex mxCompare
	bibtex mxCompare
	dvips -P pdf -G0 -o mxCompare.ps mxCompare
	ps2pdf mxCompare.ps

pic: pic.tex 
	latex pic
	dvips -P pdf -G0 -o pic.ps pic
	ps2pdf pic.ps


fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc *.dvi 
