***************************************************************************
*
*    Grid for an interface calculation between two squares
*
* usage: ogen [noplot] twoSquaresInterface -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name= -bc=[d|p]
* 
*  factor : grid resolution factor 
*
* 
* Examples:
*        ogen noplot twoSquaresInterface -factor=.25 -yFactor=.25 -interp=e -name="twoSquaresInterface0.hdf"
*        ogen noplot twoSquaresInterface -factor=1 -yFactor=.5 -interp=e
*        ogen noplot twoSquaresInterface -factor=2 -yFactor=.5 -interp=e
*        ogen noplot twoSquaresInterface -factor=2 -yFactor=2 -interp=e
* 
*        ogen noplot twoSquaresInterface -factor=4 -yFactor=4 -interp=e
*        ogen noplot twoSquaresInterface -factor=16 -yFactor=16 -interp=e
*        ogen noplot twoSquaresInterface -factor=32 -yFactor=32 -interp=e
*        ogen noplot twoSquaresInterface -factor=64 -yFactor=64 -interp=e
* 
*        ogen noplot twoSquaresInterface -factor=1 -name="twoSquaresInterface1.hdf"
*        ogen noplot twoSquaresInterface -factor=1 -order=4 -interp=e
*        ogen noplot twoSquaresInterface -factor=2 -order=4 -name="twoSquaresInterface2.order4.hdf"
*        ogen noplot twoSquaresInterface -factor=4 -order=4 -name="twoSquaresInterface4.order4.hdf"
*        ogen noplot twoSquaresInterface -factor=8 -order=4 -name="twoSquaresInterface8.order4.hdf"
*        ogen noplot twoSquaresInterface -factor=16 -order=4 -name="twoSquaresInterface16.order4.hdf"
* 
*        ogen noplot twoSquaresInterface -factor=.5 -yFactor=.5 -interp=e -name="twoSquaresInterfacee0.order2"
* 
*  periodic in y:
*    ogen noplot twoSquaresInterface -factor=2 -yFactor=.5 -bc=p -interp=e -name="twoSquaresInterfacenp2.hdf"
*    ogen noplot twoSquaresInterface -factor=32 -yFactor=4 -bc=p -interp=e -name="twoSquaresInterfacenp32.hdf"
*
* rotated:
*    ogen noplot twoSquaresInterface -factor=1 -order=2 -angle=45 -name="twoSquaresInterfaceRotated1.order2.hdf"
*    ogen noplot twoSquaresInterface -factor=1 -order=4 -angle=45 -name="twoSquaresInterfaceRotated1.order4.hdf"
* 
* non-matching:
*    ogen noplot twoSquaresInterface -factor=1 -yFactor=.5 -yFactorRight=2. -name="twoSquaresInterface1to2.order2.hdf"
*
* with a refinement at the interface
*    ogen noplot twoSquaresInterface -factor=1 -yFactor=1 -refineLeft=1 -name="twoSquaresInterface1RefineLeft.order2.hdf"
*    ogen noplot twoSquaresInterface -factor=1 -yFactor=1 -refineLeft=1 -refineRight=1 -name="twoSquaresInterface1Refine.order2.hdf"
* 
***************************************************************************
$order=2;  $orderOfAccuracy = "second order";
$interp="i"; $interpType = "implicit for all grids";
$order=2; $interp="i"; $name=""; $bc="d"; 
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$angle=0.;
$factor=1; $yFactor=1; 
$xFactorRight=1;  # additional grid resolution factor for the right domain (for non-matching grid lines)
$yFactorRight=1;  # additional grid resolution factor for the right domain (for non-matching grid lines)
$refineLeft=0; $refineRight=0; 
* 
* get command line arguments
GetOptions("order=i"=>\$order,"factor=f"=> \$factor,"yFactor=f"=> \$yFactor,"interp=s"=> \$interp,\
           "outerRad=f"=> \$outerRad,"xa=f"=> \$xa,"xb=f"=> \$xb,"bc=s"=>\$bc,"angle=f"=> \$angle,\
           "name=s"=>\$name,"xFactorRight=f"=> \$xFactorRight,"yFactorRight=f"=> \$yFactorRight,\
           "refineLeft=i"=>\$refineLeft,"refineRight=i"=>\$refineRight );
* 
*
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){ $name = "twoSquaresInterface" . "$interp$factor" . $suffix . ".hdf"; }
*
* 
*
$bcLeft = "1 100  1  1"; 
$bcRight= "100 1  1  1"; 
* 
if( $bc eq "p" ){ $bcLeft ="1 100 -1 -1"; $bcRight="100 1 -1 -1"; }
*
$order4 = "fourth order";
$order6 = "sixth order";
*
*
*
* scale number of grid points in each direction by $factor
* $factor=.5; $name = "twoSquaresInterface0.hdf";
* $factor=.5; $interp="e"; $name = "twoSquaresInterface0e.hdf"; 
* $factor=1; $name = "twoSquaresInterface1p.hdf"; $bcLeft=$bclp; $bcRight=$bcrp;
* $factor=1; $name = "twoSquaresInterface1.hdf"; 
* $factor=1; $interp="e"; $name = "twoSquaresInterface1e.hdf"; $bc = "1 2 1 1";
* $factor=2; $name = "twoSquaresInterface2.hdf"; 
* $factor=4; $name = "twoSquaresInterface4.hdf"; $bc = "1 2 1 1";
* $factor=8; $name = "twoSquaresInterface8.hdf"; 
* $factor=16; $name = "twoSquaresInterface16.hdf"; 
*
* -- fourth-order accurate ---
* $factor=.5; $name = "twoSquaresInterface0.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 1 1";
* $factor=1; $name = "twoSquaresInterface1.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=2; $name = "twoSquaresInterface2.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=4; $name = "twoSquaresInterface4.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=8; $name = "twoSquaresInterface8.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=16; $name = "twoSquaresInterface16.order4.hdf";  $orderOfAccuracy = $order4;
* 
* -- sixth-order accurate ---
* $factor=.5; $name = "twoSquaresInterface0.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=1.; $name = "twoSquaresInterface1.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=2.; $name = "twoSquaresInterface2.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=4.; $name = "twoSquaresInterface4.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
*
* square aspect ratio
* $factor=4; $yFactor=$factor; $name = "twoSquaresInterface4s.hdf"; $bc = "1 2 1 1";
* $factor=8; $yFactor=$factor; $name = "twoSquaresInterface8s.hdf"; $bc = "1 2 1 1";
* $factor=16; $yFactor=$factor; $name = "twoSquaresInterface16s.hdf"; $bc = "1 2 1 1";
*
* $factor=2; $yFactor=$factor; $name = "twoSquaresInterface2s.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 1 1";
* $factor=4; $yFactor=$factor; $name = "twoSquaresInterface4s.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 1 1";
* $factor=8; $yFactor=$factor; $name = "twoSquaresInterface8s.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 1 1";
* $factor=8; $yFactor=$factor; $name = "twoSquaresInterface8sp.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 -1 -1";
* $factor=16; $yFactor=$factor; $name = "twoSquaresInterface16s.order4.hdf";  $orderOfAccuracy = $order4; $bc = "1 2 1 1";
*  -- sixth:
* $factor=2.; $yFactor=$factor; $name = "twoSquaresInterface2s.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=4.; $yFactor=$factor; $name = "twoSquaresInterface4s.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=8.; $yFactor=$factor; $name = "twoSquaresInterface8s.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
*
* -- rotated
* $factor=4; $yFactor=$factor; $name = "twoSquaresInterface4s45.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4; $bc = "1 2 1 1";
*   periodic in tranverse direction
* $factor=4; $yFactor=$factor; $name = "twoSquaresInterface4s45p.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4; $bc = "1 2 -1 -1";
* $factor=8; $yFactor=$factor; $name = "twoSquaresInterface8s45p.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4; $bc = "1 2 -1 -1";
* $factor=4; $yFactor=$factor; $name = "twoSquaresInterface4s90.order4.hdf";  $angle=90.; $orderOfAccuracy = $order4; $bc = "1 2 1 1";
*
*
* 
***************************************************************************
*
*
* domain parameters:  
$dsx = .1/$factor; # target grid spacing in the x direction
$dsy= .1/$yFactor;          # target grid spacing in the y direction
*
create mappings
*
*  here is the left grid
*
  rectangle
    $xa=-1.0;  $xb=0.0; 
    $ya= 0.;   $yb=1.; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $ny=int( ($yb-$ya)/$dsy+1.5 );
      $nx $ny
    boundary conditions
      $bcLeft
    share
      * for now interfaces are marked with share>=100 
      0 100 0 0
    mappingName
      leftSquare0
**      leftSquare
  exit
*
*
  rectangle
    $xa= 0.0;  $xb=1.0; 
    $ya= 0.;   $yb=1.; 
    set corners
     $xa $xb $ya $yb 
    lines
      $dsx = $dsx/$xFactorRight;
      $dsy = $dsy/$yFactorRight;
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $ny=int( ($yb-$ya)/$dsy+1.5 );
      $nx $ny
    boundary conditions
      $bcRight
    share
      * for now interfaces are marked with share>=100 
      100 0 0 0
    mappingName
      rightSquare0
**      rightSquare
  exit
*
  rotate/scale/shift
    transform which mapping?
      leftSquare0
    rotate
     $angle
    0. .5 0.
    mappingName
      leftSquare
    exit
*
  rotate/scale/shift
    transform which mapping?
      rightSquare0
    rotate
     $angle
    0. .5 0.
    mappingName
      rightSquare
    exit
*
  reparameterize
    transform which mapping?
      leftSquare0
    set corners
      .5 1. .25 .75
    lines
      $nxr = $nx;  $nyr=$ny; 
      $nxr $nyr
    boundary conditions
     0 100 0 0 
    mappingName
      refinedLeftSquare
    exit
* 
    $leftGrids="leftSquare"; 
    if( $refineLeft eq 1 ){ $leftGrids .="\n refinedLeftSquare"; }
* 
*
  reparameterize
    transform which mapping?
      rightSquare0
    set corners
      .0 .5 .1 .6
    lines
      $nxr = $nx;  $nyr=$ny; 
      $nxr $nyr
    boundary conditions
     100 0 0 0 
    mappingName
      refinedRightSquare
    exit
* 
    $rightGrids="rightSquare"; 
    if( $refineRight eq 1 ){ $rightGrids .="\n refinedRightSquare"; }
* 
  exit this menu
*
generate an overlapping grid
  $leftGrids
  $rightGrids
  done 
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      leftDomain 
      * grids in the domain:
      $leftGrids
      done
    specify a domain
      * domain name:
      rightDomain 
      * grids in the domain:
      $rightGrids
      done
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    ghost points 
      all 
      2 2 2 2 
    order of accuracy
     $orderOfAccuracy
    exit 
* 
*    display intermediate results
* pause
  compute overlap
* pause
  exit
*
save an overlapping grid
  $name
  twoSquaresInterface
exit





* ------------ old way --------------

generate an overlapping grid
  leftSquare
  rightSquare
  done 
  change parameters
    prevent interpolation
      all
      all
    done
    order of accuracy
     $orderOfAccuracy
    ghost points
      all
      3 3 3 3 3 3 
  exit
* pause
  compute overlap
* pause
  exit
*
save an overlapping grid
  $name
  twoSquaresInterface
exit
