
all: pub

.SUFFIXES:
.SUFFIXES:.tex .dvi
.tex.dvi :;	latex $*

pub: overturePublications.dvi
	latex2html -local_icons overturePublications.tex

soft: overtureSoftware.dvi
	latex2html -local_icons overtureSoftware.tex

download: overtureDownload.dvi
	latex2html -local_icons overtureDownload.tex


install : install.tex
	pdflatex install.tex

