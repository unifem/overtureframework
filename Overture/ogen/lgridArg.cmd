*
* create a grid for an L-shaped region
*
* usage: 
*       ogen [noplot] lgridArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name=<name> -sharp=<num> -tstretch=<num> -nstretch=<num> ...
*                              -xa=<num> -xba=<num> -ya=<num> -yb=<num> 
* where
*    tstretch  : stretching in the tangential direction for the corner grid
*    nstretch  : stretching in the normal direction 
*
* examples:
*     ogen noplot lgridArg -factor=1 -order=2 -interp=e 
*     ogen noplot lgridArg -factor=1 -order=2 -interp=e -xa=-.5 -xb=.5 -ya=-.5 -yb=.5 -name="lgridSmall1e.order2.hdf"
*     ogen noplot lgridArg -factor=2 -order=2 -interp=e -xa=-.5 -xb=.5 -ya=-.5 -yb=.5 -name="lgridSmall2e.order2.hdf"
*
* --- assign default values to parameters: 
$name="";
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$sharpness=8.;  $tStretch=2.; $nStretch=2.; 
$bxa=-1.; $bxb=1.; $bya=-1.; $byb=1.; # bounds on outer domain 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=>\$factor,"sharp=f"=>\$sharpness,"tstretch=f"=>\$tstretch,"nstretch=f"=>\$nstretch,\
             "interp=s"=>\$interp,"xa=f"=>\$bxa,"xb=f"=>\$bxb,"ya=f"=>\$bya,"yb=f"=>\$byb,"name=s"=>\$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "lgrid" . "$interp$factor" . $suffix . ".hdf";}
* 
* ---- old way: 
* $sharpness=50.;  $tStretch=15.; $nStretch=10.; 
*  $sharpness=20.;  $tStretch=8.; $nStretch=12.; 
*
*  $factor=1;
*  $gridName = "lgrid1A.order4.hdf"; $factor=1;
* $gridName = "lgrid2A.order4.hdf"; $factor=2;
* $gridName = "lgrid3A.order4.hdf"; $factor=3;
* $gridName = "lgrid4A.order4.hdf"; $factor=4;
* $gridName = "lgrid5A.order4.hdf"; $factor=5;
* $gridName = "lgrid6A.order4.hdf"; $factor=6;
* $gridName = "lgrid7A.order4.hdf"; $factor=7;
* $gridName = "lgrid8A.order4.hdf"; $factor=8;
* 
* $gridName = "lgrid3B.order4.hdf"; $factor=3;   $sharpness=30.;  $tStretch=10.; $nStretch=20.;
* $gridName = "lgrid6B.order4.hdf"; $factor=6;   $sharpness=30.;  $tStretch=10.; $nStretch=30.;
*
$ds = 1./40./$factor;
$offset = .1; # offset of left and bottom grids from the corner
*
create mappings
*
  rectangle
    $xa=$bxa; $xb=-$offset; $ya=0.; $yb=$byb; 
    set corners
       $xa $xb $ya $yb
    mapping parameters
    close mapping dialog
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $ny
    boundary conditions
     1 0 1 1
   share
      0 0 3 4
    mappingName
     left-square
   exit
*
*
  rectangle
    $xa=-$offset-$ds*($ng-1); $xb=$bxb; $ya=-$offset-$ds*($ng-1); $yb=$byb; 
    set corners
       $xa $xb $ya $yb
    mapping parameters
    close mapping dialog
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $nx
    boundary conditions
     0 1 0 1
    share
     0 2 0 4 
    mappingName
     right-square
   exit
*
*
  rectangle
    $xa=0.; $xb=$bxb; $ya=$bya; $yb=-$offset; 
    set corners
      $xa $xb $ya $yb
    mapping parameters
    close mapping dialog
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $ny
    boundary conditions
     1 1 1 0
    share
      3 2 0 0 
    mappingName
     bottom-square
   exit
*
  * 
  smoothedPolygon
      * $cornerOffset=.3/$factor;
      $cornerOffset=.2;
      $cornerLength=$cornerOffset*2.1;  
      * $normalDist=.12/$factor; 
      $normalDist=.125/$factor; 
*
      $nStretchFactor=1.35; # add extra lines to account for stretching 
      $nxc = int( $nStretchFactor*$cornerLength/$ds + 1.5 );
      * $nyc = 13; 
      $tStretchFactor=1.35; # add extra lines to account for stretching 
      $nyc = int( $tStretchFactor*$normalDist/$ds + 1.5 );
* 
    lines
      $nxc $nyc 
    vertices
      $xc1=-$cornerOffset; $xc2=$xc1/2. 
      5
      $xc1 0.
      $xc2 0.
      0 0
      0 $xc2
      0 $xc1
* 
    n-dist
    fixed normal distance
      $normalDist
    t-stretch
      0 50
      0. 10 
      1. $tStretch 
      0. 10 
      0 50
*
    n-stretch
     1. $nStretch 0. 
*
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    boundary conditions
      0 0 1 0
    share
      0 0 3 0
    mappingName
      corner
    exit
*
  reparameterize
    set corners
      .2 .8 0. .6
    lines
      $nxr=int( $nxc*1.25 );  $nyr=int( $nyc*1.25 );
      $nxr $nyr
    mappingName
      cornerRefinement
    exit
* 
* For testing -- interpolate to a Nurbs
  nurbs (surface)
    interpolate from a mapping
      corner
    mappingName
      corner-nurbs
   exit 
* 
  exit this menu
*
generate an overlapping grid
  left-square
  right-square
  bottom-square
*  corner
  corner-nurbs
**cornerRefinement
  done
  change parameters
    prevent hole cutting
      all
      all
      done
    allow hole cutting
      corner
      all
      done
*
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
* 
    exit
* 
  compute overlap
* 
  * pause
exit
*
save a grid (compressed)
  $name
  lgrid
exit



