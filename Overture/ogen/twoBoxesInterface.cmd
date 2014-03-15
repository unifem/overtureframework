***************************************************************************
*
*  A grid for an interface calculation between two boxes
*
* Usage: 
*         ogen [noplot] twoBoxesInterface [options]
* where options are
*     -factor=<num>      : use this factor for all directions (if given)
*     -xFactor=<num>     : default grid spacing in x-direction is multiplied by this factor
*     -yFactor=<num>     : default grid spacing in y-direction is multiplied by this factor
*     -zFactor=<num>     : default grid spacing in z-direction is multiplied by this factor
*     -order=[2/4/6/8]  : order of accuracy 
*     -interp=[e/i]     : implicit or explicit interpolation
*     -bc=[d|p]
* Examples: 
*    ogen noplot twoBoxesInterface -xFactor=1 -order=2 -interp=e    (creates twoBoxesInterfacee111.order2.hdf)
*    ogen noplot twoBoxesInterface -factor=1 -order=2 -interp=e
*    ogen noplot twoBoxesInterface -factor=2 -order=2 -interp=e
*    ogen noplot twoBoxesInterface -factor=4 -order=2 -interp=e
*    ogen noplot twoBoxesInterface -factor=8 -order=2 -interp=e
* 
*    ogen noplot twoBoxesInterface -order=4 -interp=e -factor=1
*    ogen noplot twoBoxesInterface -order=4 -interp=e -factor=2
*    ogen noplot twoBoxesInterface -order=4 -interp=e -factor=4
*    ogen noplot twoBoxesInterface -order=4 -interp=e -factor=8
*    ogen noplot twoBoxesInterface -order=4 -interp=e -factor=16
* -- periodic
*    ogen noplot twoBoxesInterface -order=4 -interp=e -bc=p -factor=2
* -- rotated:
*    ogen noplot twoBoxesInterface -factor=1 -order=2 -angle=45 -name="twoBoxesInterfaceRotated1.order2.hdf" 
*    ogen noplot twoBoxesInterface -factor=1 -order=4 -angle=45 -name="twoBoxesInterfaceRotated1.order4.hdf" 
* 
***************************************************************************
$order=2; $bc="d"; 
$factor=-1; $xFactor=1; $yFactor=1; $zFactor=1; 
$interp="i"; $interpType = "implicit for all grids";
$angle=0.;
*
$bcLeft = "1 100  1  1  1 1"; 
$bcRight= "100 1  1  1  1 1"; 
* 
$bclp = "1 100 -1 -1  -1 -1 ";  # periodic in the y/z-direction
$bcrp= "100 1 -1 -1  -1 -1 ";  # periodic in the y/z-direction
* 
$ya= 0.;   $yb=.5; 
$za= 0.;   $zb=.5; 
*
* 
* get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"xFactor=i"=> \$xFactor,"yFactor=i"=> \$yFactor,"zFactor=i"=> \$zFactor,"interp=s"=> \$interp,"angle=f"=> \$angle,"name=s"=>\$name,"bc=s"=>\$bc);
* 
if( $factor>0 ){ $xFactor=$factor; $yFactor=$factor; $zFactor=$factor; }
if( $order eq 2 ){ $orderOfAccuracy="second order"; $ng=2; }\
elsif( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $bc eq "p" ){ $suffix .= "p"; } # periodic
if( $name eq "" ){ $name = "twoBoxesInterface" . "$interp$xFactor$yFactor$zFactor" . $suffix . ".hdf"; }
if( $bc eq "p" ){ $bcLeft=$bclp; $bcRight=$bcrp; }
* 
* 
* scale number of grid points in each direction by $factor
* $factor=.5; $name = "twoBoxesInterface0.hdf";
* $factor=1; $name = "twoBoxesInterface1.hdf"; 
* $factor=2; $name = "twoBoxesInterface2.hdf"; 
* $factor=4; $name = "twoBoxesInterface4.hdf"; 
* $factor=8; $name = "twoBoxesInterface8.hdf"; 
* $factor=16; $name = "twoBoxesInterface16.hdf"; 
*
* -- fourth-order accurate ---
* $factor=.5; $name = "twoBoxesInterface0.order4.hdf";  $orderOfAccuracy = $order4; 
* $factor=1; $name = "twoBoxesInterface1.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=2; $name = "twoBoxesInterface2.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=4; $name = "twoBoxesInterface4.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=8; $name = "twoBoxesInterface8.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=16; $name = "twoBoxesInterface16.order4.hdf";  $orderOfAccuracy = $order4;
* 
* -- sixth-order accurate ---
* $factor=.5; $name = "twoBoxesInterface0.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=1.; $name = "twoBoxesInterface1.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=2.; $name = "twoBoxesInterface2.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
* $factor=4.; $name = "twoBoxesInterface4.order6.hdf";  $orderOfAccuracy = $order6; $bc = "1 2 1 1";
*
* square aspect ratio
* $factor=4; $yFactor=$factor; $name = "twoBoxesInterface4s.hdf"; $bc = "1 2 1 1";
* $factor=8; $yFactor=$factor; $name = "twoBoxesInterface8s.hdf"; $bc = "1 2 1 1";
* $factor=16; $yFactor=$factor; $name = "twoBoxesInterface16s.hdf"; $bc = "1 2 1 1";
*
* $factor=2; $yFactor=$factor; $name = "twoBoxesInterface2s.order4.hdf";  $orderOfAccuracy = $order4; 
* $factor=4; $yFactor=$factor; $name = "twoBoxesInterface4s.order4.hdf";  $orderOfAccuracy = $order4; 
* $factor=8; $yFactor=$factor; $name = "twoBoxesInterface8s.order4.hdf";  $orderOfAccuracy = $order4; 
* $factor=8; $yFactor=$factor; $name = "twoBoxesInterface8sp.order4.hdf";  $orderOfAccuracy = $order4;
* $factor=16; $yFactor=$factor; $name = "twoBoxesInterface16s.order4.hdf";  $orderOfAccuracy = $order4;
*  -- sixth:
* $factor=2.; $yFactor=$factor; $name = "twoBoxesInterface2s.order6.hdf";  $orderOfAccuracy = $order6; 
* $factor=4.; $yFactor=$factor; $name = "twoBoxesInterface4s.order6.hdf";  $orderOfAccuracy = $order6; 
* $factor=8.; $yFactor=$factor; $name = "twoBoxesInterface8s.order6.hdf";  $orderOfAccuracy = $order6; 
*
* -- rotated
* $factor=4; $yFactor=$factor; $name = "twoBoxesInterface4s45.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4; 
*   periodic in tranverse direction
* $factor=4; $yFactor=$factor; $name = "twoBoxesInterface4s45p.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4;
* $factor=8; $yFactor=$factor; $name = "twoBoxesInterface8s45p.order4.hdf";  $angle=45.; $orderOfAccuracy = $order4;
* $factor=4; $yFactor=$factor; $name = "twoBoxesInterface4s90.order4.hdf";  $angle=90.; $orderOfAccuracy = $order4; 
*
*
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
*
*
***************************************************************************
*
*
* domain parameters:  
$dsx= .1/$zFactor; # target grid spacing in the x direction
$dsy= .1/$yFactor;          # target grid spacing in the y direction
$dsz= .1/$zFactor;          # target grid spacing in the y direction
*
create mappings
*
*  here is the left grid
*
  Box
    $xa=-1.0;  $xb=0.0; 
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $ny=int( ($yb-$ya)/$dsy+1.5 );
      $nz=int( ($zb-$za)/$dsz+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcLeft
    share
      * for now interfaces are marked with share>=100 
      0 100 0 0 0 0
    mappingName
      leftBox0
  exit
*
*
  Box
    $xa= 0.0;  $xb=1.0; 
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcRight
    share
      * for now interfaces are marked with share>=100 
      100 0 0 0 0 0 
    mappingName
      rightBox0
  exit
*
*
  rotate/scale/shift
    transform which mapping?
      leftBox0
    rotate
     * rotate about the z-axis
     $angle 2 
     $yr = ($ya+$yb)*.5; 
     0. $yr 0.
    mappingName
      leftBox
    exit
*
  rotate/scale/shift
    transform which mapping?
      rightBox0
    rotate
     $angle 2
     0. $yr 0.
    mappingName
      rightBox
    exit
*
  exit this menu
*
generate an overlapping grid
  leftBox
  rightBox
  done 
  change parameters
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      leftDomain 
      * grids in the domain:
      leftBox
      done
    specify a domain
      * domain name:
      rightDomain 
      * grids in the domain:
      rightBox
      done
    order of accuracy
     $orderOfAccuracy
  * choose implicit or explicit interpolation
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
* pause
  compute overlap
* pause
  exit
*
save an overlapping grid
  $name
  twoBoxesInterface
exit
