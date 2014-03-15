*
*  Make a grid for a two-stroke engine
* 
***************************************************************************
* scale number of grid points in each direction by the following factor
* $factor=1; $name = "twoStrokeEngine.hdf";   $mgLevels=0;
$factor=1.5; $name = "twoStrokeEngine1p5.hdf";   $mgLevels=0;
* $factor=2; $name = "twoStrokeEngine2.hdf";   $mgLevels=0;
* $factor=4; $name = "twoStrokeEngine4.hdf";   $mgLevels=0;
* check this -> $factor=6; $name = "twoStrokeEngine6.hdf";   $mgLevels=0;
*
*-----
  $mgFactor=2**$mgLevels;
*-----
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5); \
  $nx=int( int(($nx-1)/$mgFactor)*$mgFactor+1.5); if( $nx==1 ){ $nx=int($mgFactor+1.5); } \
  $ny=int( int(($ny-1)/$mgFactor)*$mgFactor+1.5); if( $ny==1 ){ $ny=int($mgFactor+1.5); }\
  $nz=int( int(($nz-1)/$mgFactor)*$mgFactor+1.5); if( $nz==1 ){ $nz=int($mgFactor+1.5); }\
}
*
***************************************************************************
*
$pi = 3.141592653;
$ds = .08333; # target grid spacing for factor=1
*
create mappings
  *
  *  Here is a cylinder
  *
  $outerRadius=1.; $deltaRadius=.25/$factor; 
  $innerRadius=$outerRadius-$deltaRadius; 
  Cylinder
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on the axial variable
      $cya=-.1; $cyb=1.; 
      $cya $cyb
    lines
      * 49 17 9
     $nx=int(2.*$pi*$innerRadius/$ds+1.5); $ny=int(($cyb-$cya)/$ds+1.5); $nz=int(($outerRadius-$innerRadius)/$ds+2.5); 
      getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
*   -- orient along the y-axis
    orientation
     2 0 1
    boundary conditions
      -1 -1  0 2  0 1
    periodicity
      2 0 0
    * share: top=2 outside=1
    share
      0 0  0 2  0 1 
    mappingName
      cylinder
    * pause
  exit
*
* ----- here is the core of the main cylinder -----
*
  Box
    set corners
      $delta=2.*ds/$factor; # increase box a bit
      $xa=-$innerRadius-$delta; $xb=-$xa; $ya=$cya; $yb=$cyb; $za=$xa; $zb=$xb;
      $xa $xb $ya $yb $za $zb 
    lines
      * 19 17 19 
      $nx=int( ($xb-$xa)/$ds+1.5 ); $ny=int( ($yb-$ya)/$ds+1.5 ); $nz=int( ($zb-$za)/$ds+1.5 );
      getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
    boundary
      0 0  0 3  0 0 
    * share: 2=top
    share
      0 0  0 2  0 0
    mappingName
      cylinder-core
  exit
  *
  *  Here is the piston (theta,axial,r)
  *
  Cylinder
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on the axial variable
     * $pya=-1.; $pyb=-.25; 
      $pya=-1.; $pyb=0.; 
      $pya $pyb
    lines
     $nx=int(2.*$pi*$innerRadius/$ds+1.5); $ny=int(($pyb-$pya)/$ds+1.5); $nz=int(($outerRadius-$innerRadius)/$ds+2.5); 
      getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
*   -- orient along the y-axis
    orientation
     2 0 1
    boundary conditions
      -1 -1  1 0 0 1 
    periodicity
      2 0 0
    * share 2=bottom, 1=outside
    share
      0 0  2 0  0 1
    mappingName
      piston
    * pause
  exit
  *
  * ----- here is the core of piston -----
  *
  Box
    set corners
      $xa=-$innerRadius-$delta; $xb=-$xa; $ya=$pya; $yb=$pyb; $za=$xa; $zb=$xb;
      $xa $xb $ya $yb $za $zb 
    lines
      * 19 17 19 
      $nx=int( ($xb-$xa)/$ds+1.5 ); $ny=int( ($yb-$ya)/$ds+1.5 ); $nz=int( ($zb-$za)/$ds+1.5 );
      getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
    boundary
      0 0  1 0  0 0
    * bottom=2
    share
      0 0  2 0  0 0
    mappingName
      piston-core
    * pause
  exit
  *
  * Here is a port for the two-stroke-engine
  *
  SmoothedPolygon
    vertices
    4
    -1. -2.
    -1.2 -2.
    -1.2 -1.
    -1. -1.
    n-dist
    variable normal distance
    .35 .3 5.
    .3 .25 5.
    .25 .25 5.
    sharpness
    20.
    20.
    20.
    20.
    n-stretch
    1. 1. 0.
    t-stretch
    1. 0
    1. 8.
    1. 4.
    1. 0.
    lines
    31 9
    mappingName
    2d-port
    * pause
  exit
  *
  * make a 3d port
  *
  body of revolution
    revolve which mapping?
      2d-port
    start/end angle
      -10. 10.
    tangent of line to revolve about
      0 1 0
    choose a point on the line to revolve about
      0 0 0
    lines
     $nx=int(3./$ds+1.5);  $ny=int(.3/$ds+2.5); $nz=int(.3/$ds+2.5);
    *  31 7 7
     getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
    boundary conditions
      1 1 2 2 3 3
    mappingName
      3d-port
  exit
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    3d-port
    stretch
      specify stretching along axis=0
      layers
        1
        1. 2. 1.
      exit
    exit
    mappingName
    stretched-3d-port
    * pause
    exit
  *
  * shift to the right spot
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    mappingName
    port-1
  exit
  *
  * Here is port 2
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    * rotate -75 degress about the y-axis
    rotate
    -75. 1
    0. 0. 0.
    mappingName
    port-2
  exit
  *
  * Here is port 3
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    * rotate -75 degress about the y-axis
    rotate
    -105. 1
    0. 0. 0.
    mappingName
    port-3
  exit
  *
  * Here is the cross-section of the exhaust port
  *
  rectangle
    specify corners
    1. -1. 2. -.5
    mappingName
    2d-exhaust
  exit
  *
  * make a 3d exhaust port
  *
  body of revolution
    revolve which mapping?
    2d-exhaust
    start/end angle
    -30. 30.
    tangent of line to revolve about
    0 1 0
    choose a point on the line to revolve about
    0 0 0
    choose a point on the line to revolve about
    0 0 0
    boundary conditions
    1 1 2 2 3 3
    lines
     * 19 13 11
     $nx=int(1.5/$ds+1.5);  $ny=int(.4/$ds+2.5); $nz=int(2./$ds+1.5);
     getGridPoints($nx,$ny,$nz);
      $nx $ny $nz
    mappingName
    3d-exhaust
  exit
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    3d-exhaust
    mappingName
    stretched-3d-exhaust
    stretch
      specify stretching along axis=0
      layers
      1
      1. 5. 0.
      exit
    exit
  exit
  *
  * shift to the right spot
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-exhaust
    shift
    -.025 -.05 0.
    mappingName
     exhaust
  exit
* ============== convert some mappings to dpm's for spped
*
   DataPointMapping
     build from a mapping
       port-1
     mappingName
       port-1-dpm 
  exit
*
   DataPointMapping
     build from a mapping
       port-2
     mappingName
       port-2-dpm 
  exit
*
   DataPointMapping
     build from a mapping
       port-3
     mappingName
       port-3-dpm 
  exit
*
   DataPointMapping
     build from a mapping
       exhaust
     mappingName
       exhaust-dpm 
  exit
*
 exit
*
generate an overlapping grid
    cylinder-core
    piston-core
    cylinder
    piston
    port-1-dpm 
    port-2-dpm 
    port-3-dpm 
    exhaust-dpm
  done
  change parameters
    prevent hole cutting
      all
        all
    done
    allow hole cutting
      cylinder
        cylinder-core
      cylinder
        piston-core
      piston
        piston-core
      piston
        cylinder-core
    done
   ghost points
      all
      2 2 2 2 2 2
  exit
*
  compute overlap
* pause
  exit
*
save an overlapping grid
  $name
  twoStrokeEngine
exit


