# Point this at ogen binary
ogen = /home/angelj2/Overture.g/bin/ogen

# generate some grids
grids : grids2d grids2dp

annulusGrids: 
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=2 -factor=2
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=2 -factor=4
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=2 -factor=8
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=2 -factor=16
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -factor=4
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -factor=8
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -factor=16
#       Upwind scheme needs additional ghost line for higher order
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -numGhost=3 -factor=4
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -numGhost=3 -factor=8
	$(ogen) -noplot embeddedBodyGridAnnulus -interp=i -order=4 -numGhost=3 -factor=16

# Not for this case
## 2D grids: 
#grids2d:
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=2
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -factor=16
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -factor=16
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -numGhost=3 -factor=16
#
## 2D grids with periodic BC's
#grids2dp:
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=2
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=16
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -factor=16
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=8
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -numGhost=3 -factor=16
#
#
## grids no body:
#grid2dNoBody:
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -prefix=embeddedNoBodyGrid -body=0 -factor=4
#	$(ogen) -noplot embeddedBodyGrid -interp=e -order=4 -prefix=embeddedNoBodyGrid -body=0 -factor=8
