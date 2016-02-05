

#
pseudoCode : pseudoCode.tex
	pdflatex pseudoCode


pseudoCodeOld : pseudoCode.tex
	latex pseudoCode
	dvips -P pdf -G0 -o pseudoCode.ps pseudoCode.dvi
	ps2pdf pseudoCode.ps


getBib:
	cp /Users/henshaw/papers/common/{journalISI.bib,henshaw.bib,henshawPapers.bib,fsi.bib} .
	bibClean henshawPapers.bib
