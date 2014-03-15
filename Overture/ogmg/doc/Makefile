
vpath % ../
VPATH = ../


extractFiles = ../Ogmg.C ../smooth.C ../lineSmooth.C ../fineToCoarse.C ../coarseToFine.C ../defect.C ../boundaryConditions.C \
               ../buildExtraLevels.C  

# User guide
user: 
	pdflatex ogmgUser
	bibtex ogmgUser

# Reference manual
ref: 
	pdflatex ogmgRef
	bibtex ogmgRef


# Convergence results
results: ogmgResults.tex
	pdflatex ogmgResults


# OLD: 
ogmg.pdf: ogmg.ps
	dvips -P pdf -G0 -o ogmg.ps ogmg.dvi
	ps2pdf ogmg.ps


# Old version of Reference -- includes eigenfunctions
refOld: 
	latex ogmgRefOld
	dvips -P pdf -G0 -o ogmgRefOld.ps ogmgRefOld.dvi
	ps2pdf ogmgRefOld.ps


ogmgFourthOrder.pdf:  ogmgFourthOrder.tex
	latex ogmgFourthOrder
	dvips -P pdf -G0 -o ogmgFourthOrder.ps ogmgFourthOrder.dvi
	ps2pdf ogmgFourthOrder.ps

ogmg:
	extract.p $(extractFiles)
	latex ogmg.tex
	makeindex ogmg.idx
	bibtex ogmg
	latex ogmg.tex
	dvips -P pdf -G0 -o ogmg.ps ogmg.dvi
	ps2pdf ogmg.ps
	cp ogmg.pdf $(WEBPAGE)/documentation

ogmghtml: 
	lh.p ogmg.tex
	latex ogmgHTML
	bibtex ogmgHTML
	makeindex ogmgHTML.idx
	latex ogmgHTML
	latex2html -local_icons ogmgHTML
	rm -rf $(WEBPAGE)/documentation/ogmgHTML
	mkdir $(WEBPAGE)/documentation/ogmgHTML
	cp ogmgHTML/*.* $(WEBPAGE)/documentation/ogmgHTML
	rm -rf ogmgHTML

doc: ogmg 
