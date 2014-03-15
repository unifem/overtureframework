include ../config/Makefile.$(VENDOR)


all = doc
all: $(all);

doc: developersGuideweb masterweb

developersGuide:
	pdflatex developersGuide

developersGuidehtml: 
	lh.p developersGuide.tex
	latex developersGuideHTML
#	bibtex developersGuideHTML
#	makeindex developersGuideHTML.idx
	latex developersGuideHTML
	latex2html -local_icons developersGuideHTML

developersGuideweb: developersGuide developersGuidehtml
	latex developersGuide
	dvips -P pdf -G0 -o developersGuide.ps developersGuide.dvi 
	ps2pdf developersGuide.ps
	rm -rf $(WEBPAGE)/documentation/developersGuideHTML
	mkdir $(WEBPAGE)/documentation/developersGuideHTML
	cp developersGuideHTML/*.* $(WEBPAGE)/documentation/developersGuideHTML
	rm -rf developersGuideHTML developersGuideHTML.*
	mv developersGuide.pdf $(WEBPAGE)/documentation

# Master index :
masterweb: master masterhtml

master:
	buildMasterIndex.p
	# makeindex masterIndex.idx
	latex masterIndex
	bibtex masterIndex
	latex masterIndex
	dvips  -P pdf -G0 -o masterIndex.ps masterIndex.dvi
	ps2pdf masterIndex.ps
	mv masterIndex.pdf $(WEBPAGE)/documentation
	cp help.index $(Overture)/doc

masterhtml:
	buildMasterIndex.p
	# makeindex masterIndex.idx
	latex2html -local_icons masterIndex
	rm -rf $(WEBPAGE)/documentation/masterIndex
	mkdir $(WEBPAGE)/documentation/masterIndex
	cp masterIndex/*.* $(WEBPAGE)/documentation/masterIndex
	rm -rf masterIndex

clean:
	rm *.ind *.dvi *.toc *.blg *.bbl *.log *.idx *.aux *.ilg *.ind2

