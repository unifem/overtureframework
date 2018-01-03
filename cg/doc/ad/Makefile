# Makefile for the Cgad documentation


all = CgadUsersGuide

all: $(all);


user: CgadUsersGuide
CgadUsersGuide: CgadUsersGuide.tex 
	pdflatex CgadUsersGuide


ref: CgadReferenceGuide
CgadReferenceGuide: CgadReferenceGuide.tex 
	pdflatex CgadReferenceGuide

fig:
	pdflatex fig.tex


clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc

