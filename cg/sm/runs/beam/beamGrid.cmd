#
# make a grid for a rectangular beam
# usage: ogen [noplot] square -nx=<num> -order=[2/4/6/8] -periodic=[p|n] -numGhost=[]
# 
# -periodic=p : periodic in axial direction
# 
# examples:
#
#     ogen -noplot beamGrid -height=.02 -nx=81 -ny=21
#
#     ogen -noplot beamGrid -periodic=p -height=.04 -nx=81 -ny=21
#     ogen -noplot beamGrid -periodic=p -height=.04 -nx=161 -ny=41
#
#     ogen -noplot beamGrid -periodic=p -height=.02 -nx=81 -ny=21
#     ogen -noplot beamGrid -periodic=p -height=.02 -nx=161 -ny=41
#     ogen -noplot beamGrid -periodic=p -height=.02 -nx=321 -ny=81
#
$xa=0.; $xb=1.; $height=.1; 
$order=2; $nx=41; $ny=11; # default values
$orderOfAccuracy = "second order"; $ng=2; $periodic=""; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"nx=i"=> \$nx,"ny=i"=> \$ny,"xa=f"=>\$xa,"xb=f"=>\$xb,"height=f"=>\$height,\
            "periodic=s"=>\$periodic,"numGhost=i"=> \$numGhost );
#
$ya=-.5*$height; $yb=.5*$height; 
$ar=int( ($xb-$xa)/$height + .5 ); # aspect ratio
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$lines = $nx;
$suffix="";
if( $periodic eq "p" ){ $suffix = "p"; }
$suffix .= ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
# 
# 
$name = "beamGrid" . "nx$nx" . "ny$ny" . "ar$ar" . $suffix;
#
create mappings
  rectangle
    mappingName
      beam
    set corners
      $xa $xb $ya $yb
    lines
      $nx $ny
    boundary conditions
     if( $periodic eq "p" ){ $bc ="-1 -1 3 4"; }else{ $bc="1 2 3 4"; }
     $bc
  exit
exit
#
generate an overlapping grid
  beam
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ngp $ng $ng
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
  display computed geometry
exit
#
save an overlapping grid
  $name.hdf
  $name
exit

