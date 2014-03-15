*
* Create the initial grid for a deforming cavity (for Veronica's shock tube)
* Use this grid with cgmp for a fluid-structure example.
*
* Usage:
*         ogen [noplot] cavityDeform [options]
* where options are
*     -factor=<num>     : grid spacing is .1 divided by this factor
*     -interp=[e/i]     : implicit or explicit interpolation
*     -name=<string>    : over-ride the default name  
*     -case=[inner|outer] : only build a grid for the inner or outer domain
*
* Examples:
*
*      ogen noplot cavityDeform -factor=1
*      ogen noplot cavityDeform -factor=2
*      ogen noplot cavityDeform -factor=4
*      ogen noplot cavityDeform -factor=8
* 
*      ogen noplot cavityDeform -interp=e -factor=4
*      ogen noplot cavityDeform -interp=e -factor=8
* 
* outer-domain only:
*     ogen noplot cavityDeform -case=outer -factor=1 -interp=e
*
*
$factor=1; $name=""; $case=""; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; $nrExtra=0; 
* 
* get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"case=s"=> \$case);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $nrExtra=2; }
if( $name eq "" ){$name = "cavityDeform$case" . "$interp$factor" . ".hdf";}
*
$bcFluidWall=1;
$bcFluidInflow=2;
$bcFluidSymmetry=3;
$bcInterface0=100;  # bc for interfaces
$bcInterface1=101;  
$shareInterface=100;        # share value for interfaces
*
$Pi=4.*atan2(1.,1.);
*
$ds0 = .02; 
* target grid spacing:
$ds = $ds0/$factor;
*
    $width=4.*$ds0; # width is divided by factor below 
    $rad=.5; 
* 
create mappings
*
  rectangle
    $xa=0; $xb=2.; $ya=0; $yb=1.; 
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    mappingName
      outerSquare
    share
      1 0 2 0   
    exit
*
  rectangle
    * make the inner square bigger for deforming grid problems
    * $xb = $rad-$width/$factor + 2.*$ds ;
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      $bcFluidInflow 0 $bcFluidSymmetry 0 
    share
      1 0 2 0 
    mappingName
      innerSquare
    exit
*
* Create a start curve for the interface
*
    $innerRadius=$rad; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
* 
  spline
    $n=51;
    pick spline points
     $length=2.5; 
    0 2 0 1
         1.5   0.0 
         1.495   0.065
         1.47368 0.12   
         1.25054 0.249084 0 
         0.745543 0.401034 0 
         0.244459 0.498589 0 
         0. .5 
         done
* 
*!    enter spline points
*!      $n 
*!*     $x0=0.; $y0=1.*$rad;
*!    $x0=0.; $y0=0.; $length=.5*$Pi*$rad; 
*!    $commands="";
*!    for( $i=0; $i<$n; $i++ ){ $theta=.5*$Pi*$i/($n-1.); $x0=$rad*cos($theta); $y0=$rad*sin($theta); \
*!                              $commands = $commands . "$x0 $y0\n"; }
*!    $commands
    lines
      $nr = int( $length*$rad/$ds+1.5 );
      $nr
 *  pause
    exit
*  --------------- interface grid on the outer solid --------
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 + $nrExtra +1.5 );
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix y, float x and z
    BC: right fix x, float y and z
* 
    $uniformDissipation=.5;  # .1
    $equidistribution=.0;    # 0. 
    $volumeSmooths=20;       # 20 
* 
    uniform dissipation $uniformDissipation
    boundary dissipation 0.01
    volume smooths $volumeSmooths
    equidistribution $equidistribution (in [0,1])
* 
    generate
    boundary conditions
       1  1 $bcInterface0 0
    share
       2  1 $shareInterface 0 
    name outerInterface
*  pause
    exit
*  
*  --------------- interface grid on the inner fluid --------
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +$nrExtra +1.5 );
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    BC: left fix y, float x and z
    BC: right fix x, float y and z
* 
    uniform dissipation $uniformDissipation
    boundary dissipation 0.01
    volume smooths $volumeSmooths
    equidistribution $equidistribution (in [0,1])
* 
    generate
    boundary conditions
       $bcFluidSymmetry  $bcFluidInflow $bcInterface0 0
    share
       2  1 $shareInterface 0 
    name innerInterface
    exit
*
*   open a data-base
*   junk.hdf
*   open a new file
*   put to the data-base
*   outerInterface
*   put to the data-base
*   innerInterface
*
  exit this menu
*
generate an overlapping grid
if( $case eq "inner" ){ $gridList="innerSquare\n innerInterface"; }\
elsif( $case eq "outer" ){ $gridList="outerSquare\n outerInterface"; }\
else{ $gridList="outerSquare\n outerInterface\n innerSquare\n innerInterface";  }
  $gridList
  done choosing mappings
* 
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
   if( $case eq "" ){ $cmd="specify a domain\n outerDomain\n outerSquare\n outerInterface\n done";}else{ $cmd="*"; }
      $cmd
   if( $case eq "" ){ $cmd="specify a domain\n innerDomain\n innerSquare\n innerInterface\n done";}else{ $cmd="*"; }
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
  cavityDeform
exit
