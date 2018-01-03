# Makefile moving body documentation

beamModels: beamModels.tex
	pdflatex beamModels.tex


movingBodies: movingBodies.tex
	pdflatex movingBodies.tex

# movingBodies: movingBodies.tex
# 	latex movingBodies
# 	bibtex movingBodies
# 	dvips -P pdf -G0 -o movingBodies.ps movingBodies.dvi
# 	ps2pdf movingBodies.ps

notes:
	latex notes.tex
	dvips -P pdf -G0 -o notes.ps notes.dvi
	ps2pdf notes.ps


#  RigidBodyMotion:
#  	latex RigidBodyMotion.tex
#  	dvips -P pdf -G0 -o RigidBodyMotion.ps RigidBodyMotion.dvi
#  	ps2pdf RigidBodyMotion.ps

fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps

clean: 
	-rm -f *.aux *.bbl *.blg *.ind *.ilg *.log *.idx *.toc
