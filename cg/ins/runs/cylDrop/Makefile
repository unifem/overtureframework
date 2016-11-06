ogen = ${Overture}/bin/ogen

# generate some grids
grids : diskDrop risingDisk

# 2D grids: 
diskDrop:
	$(ogen) -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=2 
	$(ogen) -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=4
	$(ogen) -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=8
	$(ogen) -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=16
	$(ogen) -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=32


#   wider channel
risingDisk:
	$(ogen) -noplot diskDropGrid.cmd -interp=e -xa=-1.5 -xb=1.5 -cx=.9 -cy=.5 -prefix=risingDiskGrid -factor=2
	$(ogen) -noplot diskDropGrid.cmd -interp=e -xa=-1.5 -xb=1.5 -cx=.9 -cy=.5 -prefix=risingDiskGrid -factor=4


# add inflow grid 
risingDrop:
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=1 
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=2 
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=4
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=8
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=16
	$(ogen) -noplot diskDropGrid.cmd -improveQuality=1 -interp=e -addInlet=1 -cx=.5 -cy=.5 -prefix=risingDropGrid -factor=32

doc:
	pdflatex cylDrop


