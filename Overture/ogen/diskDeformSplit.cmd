*
* Create the initial grid for a deforming disk using two grids on each side of the interface
* for testing this case 
* . 
* Use this grid with cgmp for a fluid-structure example.
*
* Usage:
*         ogen [noplot] diskDeformSplit [options]
* where options are
*     -factor=<num>     : grid spacing is .1 divided by this factor
*     -interp=[e/i]     : implicit or explicit interpolation
*     -name=<string>    : over-ride the default name  
*     -case=[inner|outer] : only build a grid for the inner or outer domain
*
* Examples:
*
*      ogen noplot diskDeformSplit -factor=1
*      ogen noplot diskDeformSplit -factor=2
*      ogen noplot diskDeformSplit -factor=1 -interp=e
*      ogen noplot diskDeformSplit -factor=2 -interp=e
* 
* outer-domain only:
*      ogen noplot diskDeformSplit -factor=2 -interp=e -case=outer
*
*
$factor=1; $name=""; $case=""; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
* 
* get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"case=s"=> \$case);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "diskDeformSplit$case" . "$interp$factor" . ".hdf";}
*
$bcInterface0=100;  # bc for interfaces
$bcInterface1=101;  
$shareInterface=100;        # share value for interfaces
*
$Pi=4.*atan2(1.,1.);
*
$ds0 = .1; 
* target grid spacing:
$ds = $ds0/$factor;
*
$width= $ds0*3; 
$rad=1.; 
* 
create mappings
*
  rectangle
    $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    mappingName
      outerSquare
    exit
*
  rectangle
    $xb = $rad-$width/$factor + 2.*$ds ;
    $xa=-$xb; $ya=$xa; $yb=$xb; 
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    mappingName
      innerSquare
    exit
*
* Create a start curve for the right-half of the interface
*
    $innerRadius=$rad; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
* 
  spline
    $n=51;
    enter spline points
      $n 
    $x0=0.; $y0=1.*$rad;
    $theta0=-$Pi*.5; $theta1=$Pi*.5; $dTheta=$theta1-$theta0;
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=$theta0+ $dTheta*$i/($n-1.); $x0=$rad*cos($theta); $y0=$rad*sin($theta); \
                              $commands = $commands . "$x0 $y0\n"; }
    $commands
    lines
      $nr = int( $dTheta*$rad/$ds+1.5 );
      $nr
    * pause
    exit
*  
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface0
    Share Value: bottom $shareInterface
    close mapping dialog
    name outerInterface1
    * pause
    exit
*  
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface0
    Share Value: bottom $shareInterface
    close mapping dialog
    name innerInterface1
    exit
*
*
* Create a start curve for the left-half of the interface
*
    $innerRadius=$rad; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
* 
  spline
    $n=51;
    enter spline points
      $n 
    $x0=0.; $y0=1.*$rad;
    $theta0=$Pi*.5; $theta1=$Pi*1.5; $dTheta=$theta1-$theta0;
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=$theta0+ $dTheta*$i/($n-1.); $x0=$rad*cos($theta); $y0=$rad*sin($theta); \
                              $commands = $commands . "$x0 $y0\n"; }
    $commands
    lines
      $nr = int( $dTheta*$rad/$ds+1.5 );
      $nr
    * pause
    exit
*  
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface0
    Share Value: bottom $shareInterface
    close mapping dialog
    name outerInterface2
    * pause
    exit
*  
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface0
    Share Value: bottom $shareInterface
    close mapping dialog
    name innerInterface2
    exit
*
*
  exit this menu
*
generate an overlapping grid
if( $case eq "inner" ){ $gridList="innerSquare\n innerInterface1\n innerInterface2"; }\
elsif( $case eq "outer" ){ $gridList="outerSquare\n outerInterface1\n outerInterface2"; }\
else{ $gridList="outerSquare\n outerInterface1\n outerInterface2\n innerSquare\n innerInterface1\n innerInterface2";  }
  $gridList
  done choosing mappings
* 
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
   if( $case eq "" ){ $cmd="specify a domain\n outerDomain\n outerSquare\n outerInterface1\n outerInterface2\n done";}else{ $cmd="*"; }
      $cmd
   if( $case eq "" ){ $cmd="specify a domain\n innerDomain\n innerSquare\n innerInterface1\n innerInterface2\n done";}else{ $cmd="*"; }
      $cmd
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
*
  compute overlap
*
exit
*
save an overlapping grid
  $name
  diskDeformSplit
exit
