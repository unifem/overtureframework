*
* Grid for a solid lens in a fluid channel
* Use this grid with cgmp for a fluid-structure example.
*
* Usage:
*         ogen [noplot] lens [options]
* where options are
*     -factor=<num>     : grid spacing is .1 divided by this factor
*     -interp=[e/i]     : implicit or explicit interpolation
*     -name=<string>    : over-ride the default name  
*     -case=[inner|outer] : only build a grid for the inner or outer domain
*
* Examples:
*
*      ogen noplot lens -factor=1
*      ogen noplot lens -factor=2
*      ogen noplot lens -interp=e -factor=2
*      ogen noplot lens -interp=e -factor=4
*      ogen noplot lens -interp=e -factor=8
* 
$factor=1; $name=""; $case=""; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
* 
$pi=4.*atan2(1.,1.);
$lensRadius=1.25;
$lensHalfWidth=.2;
$lensHalfAngle=$pi/4.;
$stemHeight=.25;       # height of the vertical stems on the top and bottom of the lens arc
*
*
* get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"case=s"=> \$case);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "lens$case" . "$interp$factor" . ".hdf";}
*
$bcFluidWall=1;
$bcFluidInflow=2;
$bcFluidSymmetry=3;
$bcFluidOutflow=5; 
$bcFluidInterface1=100;  # bc for interfaces
$shareFluidInterface1=100;  # bc for interfaces
$bcFluidInterface2=101;  # bc for interfaces
$shareFluidInterface2=101;  # bc for interfaces
*
$bcSolidWall=1; 
$bcSolidInterface1=100;  
$shareSolidInterface1=100;  
$bcSolidInterface2=101;  
$shareSolidInterface2=101;  
*
*
* target grid spacing:
$ds0 = .05; 
$ds = $ds0/$factor;
*
create mappings
* --- make a spline curve for the lens surface (left side)
  spline
    $n1=7; $narc=51; $n2=7; 
    $n=$n1+$narc+$n2;
    enter spline points
      $n 
    $xc=-$lensRadius-$lensHalfWidth; $yc=0.;  # center of the lens circle
    $theta0=-$lensHalfAngle; $theta1=$lensHalfAngle;
    $x1 = $xc+$lensRadius*cos($theta0); $y1=$yc+$lensRadius*sin($theta0);
    $ya=$y1-$stemHeight; # define ya
    $x0=$x1; $y0=$ya; 
    $commands="";
    * make a line from (x0,y0) to (x1,y1)
    for( $i=0; $i<$n1; $i++ ){ $x= $x0+($x1-$x0)*$i/($n1); $y= $y0+($y1-$y0)*$i/($n1); \
                               $commands = $commands . "$x $y\n"; }
    * make an arc 
    for( $i=0; $i<$narc; $i++ ){ $theta=$theta0 + ($theta1-$theta0)*$i/($narc-1.); \
                              $x=$xc+$lensRadius*cos($theta); $y=$yc+$lensRadius*sin($theta); \
                              $commands = $commands . "$x $y\n"; }
    * make a line from (x2,y2) to (x3,y3)
    $x2 = $xc+$lensRadius*cos($theta1); $y2=$yc+$lensRadius*sin($theta1);
    $yb=$y2+$stemHeight; # define yb 
    $x3=$x2; $y3=$yb; 
    for( $i=0; $i<$n2; $i++ ){ $x= $x2+($x3-$x2)*($i+1)/($n2); $y= $y2+($y3-$y2)*($i+1)/($n2); \
                               $commands = $commands . "$x $y\n"; }
    $commands
    $length = ($yb-$ya)*1.25; # guess 
    lines
      $nr = int( $length/$ds+1.5 );
      $nr
 *  pause
    mappingName
     leftLensArc
    exit
* 
  rotate/scale/shift
    scale
    -1 1
    mappingName
     rightLensArc
    exit
*
  rectangle
    $xa=-2; $xb=2.;
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    mappingName
      backGroundFluid
    boundary conditions
      $bcFluidInflow $bcFluidOutflow $bcFluidWall $bcFluidWall
    share
      0 0 1 2 
    exit
*
* ------------ right fluid lens grid -----
  hyperbolic
    marching options...
    target grid spacing $ds $ds (tang,normal, <0 : use default)
    Start curve:rightLensArc
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    lines to march 5
    generate
    boundary conditions
      $bcFluidWall $bcFluidWall $bcFluidInterface2 0
    share
      1 2 $shareFluidInterface2 0
    name rightLensFluid
    * pause
  exit
* ------------ left fluid lens grid ----
  hyperbolic
    marching options...
    target grid spacing $ds $ds (tang,normal, <0 : use default)
    Start curve:leftLensArc
    backward
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    lines to march 5
    generate
    boundary conditions
     $bcFluidWall $bcFluidWall $bcFluidInterface1 0
    share
      1 2 $shareFluidInterface1 0
    name leftLensFluid
    * pause
  exit
*
*   -- solid grids ----
* 
*
  rectangle
    $xas=$x0; $xbs=-$xas; $yas=$ya; $ybs=$yb; 
    $nx=int( ($xbs-$xas)/$ds+1.5 ); 
    $ny=int( ($ybs-$yas)/$ds+1.5 ); 
    set corners
      $xas $xbs $yas $ybs 
    lines
      $nx $ny 
    mappingName
      backGroundSolid
    boundary conditions
      0 0 $bcSolidWall $bcSolidWall
    share
      0 0 1 2 
    exit
* ------------ right solid lens grid -----
  hyperbolic
    marching options...
    target grid spacing $ds $ds (tang,normal, <0 : use default)
    Start curve:rightLensArc
    backward
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    lines to march 5
    generate
    boundary conditions
      $bcSolidWall $bcSolidWall $bcSolidInterface2 0
    share
      1 2 $shareSolidInterface2 0
    name rightLensSolid
  exit
* ------------ left fluid lens grid ----
  hyperbolic
    marching options...
    target grid spacing $ds $ds (tang,normal, <0 : use default)
    Start curve:leftLensArc
    forward
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    lines to march 5
    generate
    boundary conditions
     $bcSolidWall $bcSolidWall $bcSolidInterface1 0
    share
      1 2 $shareSolidInterface1 0
    name leftLensSolid
  exit
*
exit this menu
*
generate an overlapping grid
  backGroundFluid
  leftLensFluid
  rightLensFluid
*
  backGroundSolid
  leftLensSolid
  rightLensSolid
  done choosing mappings
* 
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
   if( $case eq "" ){ $cmd="specify a domain\n fluidDomain\n backGroundFluid\n leftLensFluid\n rightLensFluid\n done";}else{ $cmd="*"; }
      $cmd
   if( $case eq "" ){ $cmd="specify a domain\n solidDomain\n backGroundSolid\n leftLensSolid\n rightLensSolid\n done";}else{ $cmd="*"; }
      $cmd
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
*    open graphics
  compute overlap
exit
*
save an overlapping grid
  $name
  lens
exit
