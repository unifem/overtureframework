#
#
#  plotStuff plotEmbeddedGrid -show=embeddedBodySharpGride8.order2.hdf
#
$show="ebG4Order4Angle60.hdf";
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
$show
#
  DISPLAY SQUARES:0 0
  set view:0 0.00131791 0.0521067 0 10.4305 1 0 0 0 1 0 0 0 1
  line width scale factor:0 3
  colour boundaries by chosen name
  colour grid lines from chosen name
  grid colour 3 BLUE
  grid colour 4 RED
  grid colour 2 GREEN
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  hardcopy file name:0 embeddedBodyZoomNearBody.ps
  hardcopy save:0