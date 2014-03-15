***************************************************************************
*
*  Create a grid for curved pipe consisting of a 
*   solid hollow pipe and inner fluid region
*
*  Usage:
*    ogen noplot curvedPipe -factor=<> -interp=[e,i] -order=[2,4,6,8] -option=[hollow|solid] -angle=<> ...
*                           -startAngle=<> -rgd=[fixed|var]
*
*  -angle : angle (in degrees) through which the pipe cross-section is revolved. 
*  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
*
* Small curved pipe for testing:
*   ogen noplot curvedPipe -angle=22 -interp=e -factor=1
*   ogen noplot curvedPipe -angle=22 -interp=e -factor=2
*   ogen noplot curvedPipe -angle=22 -interp=e -factor=4
*
*   ogen noplot curvedPipe -startAngle=45 -angle=67 -interp=e -factor=1
*   ogen noplot curvedPipe -startAngle=45 -angle=67 -interp=e -factor=2
*   ogen noplot curvedPipe -startAngle=45 -angle=67 -interp=e -factor=4
*
*   ogen noplot curvedPipe -angle=45 -interp=e -factor=1
*   ogen noplot curvedPipe -angle=45 -interp=e -factor=2
*   ogen noplot curvedPipe -angle=45 -interp=e -factor=3
*   ogen noplot curvedPipe -angle=45 -interp=e -factor=4
*
*   ogen noplot curvedPipe -angle=90 -interp=e -factor=1
*   ogen noplot curvedPipe -angle=90 -interp=e -factor=2
*   ogen noplot curvedPipe -angle=90 -interp=e -factor=3
*   ogen noplot curvedPipe -angle=90 -interp=e -factor=4
*
*  -- for grid convergence use a fixed radial grid distance:
*   ogen noplot curvedPipe -angle=90 -interp=e -rgd=fixed -factor=1
*   ogen noplot curvedPipe -angle=90 -interp=e -rgd=fixed -factor=2
*   ogen noplot curvedPipe -angle=90 -interp=e -rgd=fixed -factor=3
*   ogen noplot curvedPipe -angle=90 -interp=e -rgd=fixed -factor=4
*
* ogen noplot curvedPipe -option=hollow -angle=90 -interp=e -factor=1
* ogen noplot curvedPipe -option=hollow -angle=90 -interp=e -factor=2
* ogen noplot curvedPipe -option=hollow -angle=90 -interp=e -factor=4
* 
* ogen noplot curvedPipe -option=solid -angle=90 -interp=e -factor=1 
* ogen noplot curvedPipe -option=solid -angle=90 -interp=e -factor=2 
* ogen noplot curvedPipe -option=solid -angle=90 -interp=e -factor=4 
* 
***************************************************************************
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name="";  $option=""; 
$xa =-2.; $xb=2.; $ya=0.; $yb=3.; $za=-.5; $zb=.5; 
$radius=.20;          # radius of the tube
$nr=7;                # number of lines in the radial direction 
$rgd="var";         # 
$pi=4.*atan2(1.,1.); 
$pipeAngle=90; $pipeAngleStart=0; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=>\$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "angle=f"=> \$pipeAngle,"startAngle=f"=> \$pipeAngleStart,"interp=s"=> \$interp,\
            "option=s"=> \$option,"rgd=s"=> \$rgd,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$prefix = "curvedPipe"; 
if( $option ne "" ){ $prefix = $option . "CurvedPipe"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $name eq "" ){$name = "$prefix$pipeAngle" . "$interp$factor" . $suffix . ".hdf";}
*
$ds=.05/$factor;      # target grid spacing for solid
$rDist=($nr-2)*$ds; # normal distance for grids  (allow extra points for stretching)
if( $rgd eq "fixed" ){ $rDist=.15; } # fix for convergence tests
* 
$bcInflow=10;       # bc for inflow boundaries on the tube
$bcOutflow=11;      # bc for outflow boundaries on the tube
$bcInterface1=100;  # bc for interfaces are numbered starting from 100 
* 
$pipeInnerRad=.5;
$pipeOuterRad=.7; 
$pipeMajorRadius=2.5; # radius of the "torus"
*
*
create mappings
  annulus
    inner and outer radii
      $innerRad=$pipeInnerRad-$rDist; 
      $innerRad $pipeInnerRad 
    boundary conditions
      -1 -1 0 2
    share
      0 0 0 2
    lines
      31 7
    mappingName
      fluidAnnulus
    exit
* 
  body of revolution
    revolve which mapping?
      fluidAnnulus
    choose a point on the line to revolve about
      $pipeMajorRadius 0 0
    start/end angle
      $pipeAngleStart $pipeAngle
    lines
      $nTheta = int( 2.*$pi*($pipeInnerRad)/$ds +1.5 );
      $nAxial = int( $pi*$pipeAngle/180.*($pipeMajorRadius)/$ds +1.5 );
      $nRad = int( ($pipeInnerRad - $innerRad)/$ds +1.5 );
      $nTheta $nRad $nAxial
    boundary conditions
      -1 -1 0 $bcInterface1 $bcInflow $bcOutflow 
    share
       0 0 0 0 $bcInflow $bcOutflow
    mappingName
      fluidPipe
    exit
*
*  -- make the fluid core grid --
* 
  rectangle
   set corners
     * Note: we need to make the core grid larger for explicit interpolation
     if( $interp eq "e" ){ $dsPlus = 2.*$ds; }else{ $dsPlus=$ds; }
     $xac=-($innerRad+$dsPlus); $xbc=-$xac; $yac=$xac; $ybc=$xbc; 
     $xac $xbc $yac $ybc
   lines
     $nxc = int( ($xbc-$xac)/$ds +2.5 ); 
     $nyc = int( ($ybc-$yac)/$ds +2.5 ); 
     $nxc $nyc
   boundary conditions
     0 0 0 0 
   mappingName
     core-cross-section
 exit
* 
  body of revolution
    revolve which mapping?
      core-cross-section
    choose a point on the line to revolve about
      $pipeMajorRadius 0 0
    start/end angle
      $pipeAngleStart $pipeAngle
    lines
      $nxc $nyc $nAxial
    boundary conditions
       0 0 0 0 $bcInflow $bcOutflow 
    share
       0 0 0 0 $bcInflow $bcOutflow
    mappingName
      fluidCore
    exit
*
*  -- Make the solid "pipe" --
* 
  annulus
    inner and outer radii
      $pipeInnerRad $pipeOuterRad 
    boundary conditions
      -1 -1 2 3 
    share
      0 0 2 3 
    lines
      31 7
    mappingName
      solidAnnulus
    exit
* 
  body of revolution
    revolve which mapping?
      solidAnnulus
    choose a point on the line to revolve about
      $pipeMajorRadius 0 0
    start/end angle
      $pipeAngleStart $pipeAngle
    lines
      * $nTheta = int( 2.*$pi*($pipeInnerRad)/$ds +1.5 );
      * $nAxial = int( $pi*$pipeAngle/180.*($pipeMajorRadius)/$ds +1.5 );
      $nRad = int( ($pipeOuterRad -$pipeInnerRad)/$ds +1.5 );
    * 
      $nTheta $nRad $nAxial
    boundary conditions
      -1 -1 $bcInterface1 2 $bcInflow $bcOutflow
    share
       0 0 0 0 $bcInflow $bcOutflow
    mappingName
      solidPipe
    exit
* 
exit
*
* ----- generate the overlapping grid ---------
*
generate an overlapping grid
  if( $option eq "" ){ $grids = "fluidCore\n fluidPipe\n solidPipe\n"; }elsif( $option eq "hollow" ){ $grids = "solidPipe"; }else{ $grids = "fluidCore\n fluidPipe"; }
  $grids
  done choosing mappings
* 
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    if( $option eq "" || $option eq "solid" ){ $cmds = "specify a domain\n fluidDomain\n fluidCore\n fluidPipe\n done"; }else{ $cmds="*";}
    $cmds 
    if( $option eq "" || $option eq "hollow" ){ $cmds = "specify a domain\n solidDomain\n solidPipe\n done"; }else{ $cmds="*";}
    $cmds
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    exit 
* 
  compute overlap
  * pause
exit
* save an overlapping grid
save a grid (compressed)
$name
curvedPipe
exit
