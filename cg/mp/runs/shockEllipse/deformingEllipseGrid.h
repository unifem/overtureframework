  $rad0 = 0.5*($rada+$radb);
# ---------------  
  rectangle
   # make the inner square bigger for deforming grid problems
    $expand=.5; 
    $xa = $xc - $rada - $expand; 
    $xb = $xc + $rada + $expand;
    $ya = $yc - $rada - $expand; 
    $yb = $yc + $rada + $expand;
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    mappingName
      $mapName = "innerSquare$diskNumber";  $diskNames .= "\n $mapName"; 
      $mapName
    exit
#
# Create a start curve for the interface
#
    $innerRadius=$rad0; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
# 
  spline
    $n=201;
    enter spline points
      $n 
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=2.*$Pi*$i/($n-1.); $x0=$rada*cos($theta); $y0=$radb*sin($theta); \
                              $commands = $commands . "$x0 $y0\n"; }
    $commands
    lines
      $nr = int( 2.*$Pi*$rad0/$ds+1.5 );
      $nr
    periodicity
      2
 # pause
    exit
  rotate/scale/shift
    rotate
      $phi
      0 0
    shift
      $xc $yc
  exit
#  
   $width = $width + $ds0*$nExtra;
# 
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );  if( $ns<3 ){ $ns=3; }
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
#    uniform dissipation 0.05
#    volume smooths 500
#    equidistribution .2 (in [0,1])
    generate
#    pause
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "outerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
#  pause
    exit
#  
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 ); if( $ns<3 ){ $ns=3; }
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
#    uniform dissipation 0.05
#    volume smooths 500
#    equidistribution .2 (in [0,1])
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "innerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
    exit
