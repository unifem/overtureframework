#
# Circular plate with holes
#
#
# usage: ogen [noplot] plateWithHoles -numberOfHoles=<val> -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#  ogen noplot plateWithHoles -numberOfHoles=2  -factor=2 
#  ogen noplot plateWithHoles -numberOfHoles=12 -factor=2 
#  ogen noplot plateWithHoles -numberOfHoles=12 -interp=e -factor=2 
#  ogen noplot plateWithHoles -numberOfHoles=12 -factor=4 
# 
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -R3=2.25 -R4=.4 -theta2=15. -numHoles2=12 -factor=2
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -R3=2.25 -R4=.4 -theta2=15. -numHoles2=12 -factor=4
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -R3=2.25 -R4=.4 -theta2=15. -numHoles2=12 -factor=8
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -R3=2.25 -R4=.4 -theta2=15. -numHoles2=12 -factor=32
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -R3=2.25 -R4=.4 -theta2=15. -numHoles2=12 -factor=16
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -factor=8
#  ogen noplot plateWithHoles -interp=e -R1=3.4 -R2=.3 -numHoles1=24 -factor=16
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$R0 = 4.; # outer radius of the plate
$R1 = 3.; # holes sit on a circle of this radius (ring 1)
$R2 = .5; # radius of the holes                  (ring 1)
$theta1 =0.; # offset of first hole in degrees.   (ring 1)
$numHoles1=2;
$R3 = 2.; # holes sit on a circle of this radius (ring 2)
$R4 = .4; # radius of the holes                  (ring 2)
$theta2 =10.; # offset of first hole in degrees.  (ring 2)
$numHoles2=2;
# 
$name=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"R0=f"=> \$R0,"R1=f"=> \$R1,"R2=f"=> \$R2,\
            "theta1=f"=>\$theta1, "interp=s"=> \$interp,"name=s"=> \$name, "numHoles1=i"=>\$numHoles1,\
            "R3=f"=>\$R3,"R4=f"=>\$R4,"theta2=f"=>\$theta2, "numHoles2=i"=>\$numHoles2 );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "plateWith$numHoles1" . "Holes" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
create mappings
#
# ------  Main outer annulus ----------------------
Annulus
  $nr = 5+$ng;
  $outerRad=$R0; $innerRad=$outerRad - ($nr-1)*$ds;
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 0 1
  mappingName
    outerAnnulus
exit
# 
#  ---------------  Holes for outer ring ------------------------------
Annulus
  $nr = 5+$ng;
  $innerRad=$R2; $outerRad=$innerRad + ($nr-1)*$ds;
  inner and outer radii
    $innerRad $outerRad
  centre for annulus
    $R1 0. 0. 
  lines
    $nTheta = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0 
  mappingName
    hole1Ring1
exit
#       ---  make holes on outer radius ---
$i=0;
$cmds=""; $gridNames=""; 
for( $j=1; $j<=$numHoles1; $j++ ){\
 $i=$i+1;\
 $holeName= "hole$i"; $angle = $theta1 + 360.*($j-1)/$numHoles1;\
 $gridNames .= "$holeName\n"; \
 $cmds .= "rotate/scale/shift\n"; \
 $cmds .= " transform which mapping?\n"; \
 $cmds .= "   hole1Ring1\n"; \
 $cmds .= " rotate\n"; \
 $cmds .= "   $angle \n"; \
 $cmds .= "   0 0 0\n"; \
 $cmds .= " mappingName\n"; \
 $cmds .= "   $holeName\n"; \
 $cmds .= " exit\n";}
$cmds .= "#"; 
#
$cmds
# 
#  --------------- Holes for inner ring  ------------------------------
Annulus
  $nr = 5+$ng;
  $innerRad=$R4; $outerRad=$innerRad + ($nr-1)*$ds;
  inner and outer radii
    $innerRad $outerRad
  centre for annulus
    $R3 0. 0. 
  lines
    $nTheta = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0 
  mappingName
    hole1Ring2
exit
#       ---  make holes on outer radius ---
$cmds=""; 
for( $j=1; $j<=$numHoles2; $j++ ){\
 $i=$i+1;\
 $holeName= "hole$i"; $angle = $theta2 + 360.*($j-1)/$numHoles2;\
 $gridNames .= "$holeName\n"; \
 $cmds .= "rotate/scale/shift\n"; \
 $cmds .= " transform which mapping?\n"; \
 $cmds .= "   hole1Ring2\n"; \
 $cmds .= " rotate\n"; \
 $cmds .= "   $angle \n"; \
 $cmds .= "   0 0 0\n"; \
 $cmds .= " mappingName\n"; \
 $cmds .= "   $holeName\n"; \
 $cmds .= " exit\n";}
$cmds .= "#"; 
#
$cmds
#
$gridName .= "#";
# 
rectangle
  $delta = ($nr-2)*$ds; 
  $xa=-($R0-$delta); $xb=-$xa; $ya=$xa; $yb=$xb; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  mappingName
    backGround
exit
#
exit
generate an overlapping grid
    backGround
    outerAnnulus
    $gridNames
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
# plot
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
plateWithHoles
exit

