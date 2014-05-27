#
#  plotStuff plotBeamInAChannel.cmd -show=beam4.show
#
#
$show="beam4.show";
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution );
#
$show
# 
derived types
vorticity
exit
#
contour
  plot:vorticity
  min max -50 25
  vertical scale factor 0.
  plot contour lines (toggle)
  exit
