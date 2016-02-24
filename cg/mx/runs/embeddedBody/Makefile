ogen = /home/henshw/Overture.g/bin/ogen

# generate some grids
grids : grids2d grids2dp

# 2D grids: 
grids2d:
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=2
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=16
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=16
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=16

# 2D grids with periodic BC's
grids2dp:
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=2
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=16
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=16
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=4
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=8
	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=16


# Make the documentation:
doc:
	pdflatex embeddedBody
	bibtex embeddedBody