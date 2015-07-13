#
# Two adjacent boxes
#
# usage: ogen [noplot] twoBoxes -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#  ogen -noplot twoBoxes -order=2 -interp=i -factor=1 
#  ogen -noplot twoBoxes -order=2 -interp=i -factor=2 
#  ogen -noplot twoBoxes -order=2 -interp=i -factor=4 
#  					       				       
#  ogen -noplot twoBoxes -order=2 -interp=e -factor=1
#  ogen -noplot twoBoxes -order=2 -interp=e -factor=2 
#  ogen -noplot twoBoxes -order=4 -interp=e -factor=2 
#
#  -- split the unit box [0,1]^3
# 
#  ogen -noplot twoBoxes -order=2 -interp=e -xa=0. -xb=1 -ya=0 -yb=1 -za=0 -zb=1 -prefix=unitBoxSplit -factor=1
#  ogen -noplot twoBoxes -order=2 -interp=e -xa=0. -xb=1 -ya=0 -yb=1 -za=0 -zb=1 -prefix=unitBoxSplit -factor=2
#  ogen -noplot twoBoxes -order=2 -interp=e -xa=0. -xb=1 -ya=0 -yb=1 -za=0 -zb=1 -prefix=unitBoxSplit -factor=4
#
#  -- split nonBox 
#  ogen -noplot twoBoxes -order=2 -interp=e -xa=0. -xb=1 -ya=0 -yb=1 -za=0 -zb=1 -prefix=unitNonBoxSplit -factor=1
#
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.; $za=-.5; $zb=.5; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$prefix="twoBoxes";
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"prefix=s"=>\$prefix,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"ml=i"=>\$ml,\
            "numGhost=i"=>\$numGhost );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor;
$width = ($order-2)/2;
if( $interp eq "e" ){ $width=$width+1.; }
$overlap = $ds*$width + $ds*.125;
#
if( $prefix =~ /^[\w]*NonBox[\w]*/ ){ $nonBox=1; }else{ $nonBox=0; }
# 
create mappings
  Box
    set corners
     $xm = .5*($xa+$xb); # x-mid-point 
     $xas=$xa; $xbs=$xm + $overlap; $yas=$ya; $ybs=$yb
     $xas $xbs $ya $yb $za $zb
    lines
      $nx=int( ($xbs-$xas)/$ds+1.5 );
      $ny=int( ($ybs-$yas)/$ds+1.5 );
      $nz=int( ($zb -$za )/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      1 0 3 4 5 6 
    share
      0 0 3 4 5 6 
    mappingName
      if( $nonBox eq 1 ){ $mapName = "leftNonBox"; }else{ $mapName = "leftBox"; }
      $mapName
    exit
#  -- optionally create a "nonBox"
  if( $nonBox eq 1 ){ $cmd ="rotate/scale/shift\n mappingName\n leftBox\n exit\n"; }else{ $cmd="#"; }
  $cmd
# 
  Box
    set corners
     $xas=$xm-$overlap; $xbs=$xb; $yas=$ya; $ybs=$yb
     $xas $xbs $ya $yb $za $zb
    lines
      $ds2 = $ds*1.12345;
      $nx=int( ($xbs-$xas)/$ds2+1.5 );
      $ny=int( ($ybs-$yas)/$ds2+1.5 );
      $nz=int( ($zb -$za )/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      0 2 3 4 5 6 
    share
      0 0 3 4 5 6 
    mappingName
      if( $nonBox eq 1 ){ $mapName = "rightNonBox"; }else{ $mapName = "rightBox"; }
      $mapName
    exit
#  -- optionally create a "nonBox"
  if( $nonBox eq 1 ){ $cmd ="rotate/scale/shift\n mappingName\n rightBox\n exit\n"; }else{ $cmd="#"; }
  $cmd
#
  exit
#
generate an overlapping grid
  leftBox
  rightBox
  done
  change parameters
    ghost points
      all
       $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
  compute overlap
exit
save a grid (compressed)
$name
sis
exit
