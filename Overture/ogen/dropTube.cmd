***************************************************************************
*
*  sphere in a tube for moving grid computations
*
*  Examples: 
*     ogen noplot dropTube -factor=1 -mgLevels=1
*     ogen noplot dropTube -factor=2 -mgLevels=1
*     ogen noplot dropTube -factor=4 -mgLevels=2
* 
***************************************************************************
* scale number of grid points in each direction by the following factor
* $factor=.75; $name = "dropTube0.hdf";   $mgLevels=0;
* $factor=1; $name = "dropTube.hdf";   $mgLevels=1;
* $factor=2; $name = "dropTube2.hdf";   $mgLevels=1;
* $factor=3; $name = "dropTube3.hdf";   $mgLevels=2;
* $factor=4; $name = "dropTube4.hdf";   $mgLevels=2;
*
$factor=1; $mgLevels=1;
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"mgLevels=i"=>\$mgLevels,"name=s"=> \$name);
* 
if( $name eq "" ){$name = "dropTube" . "$factor" . ".hdf";}
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
* --- here is the target grid spacing ---
  $ds = .05; 
*
create mappings
*
* Here is the cylinder
*
$pi = 3.141592653;
$ya=-1.; $yb=1.; 
$cylOuterRadius=.5; 
$cylDeltaRadius=.2/$factor;
$cylInnerRadius=$cylOuterRadius-$cylDeltaRadius;
*
  * main cylinder
  Cylinder
    mappingName
      cylinder
    * orient the cylinder so y-axis is axial direction
    orientation
      2 0 1
    bounds on the radial variable
      $cylInnerRadius $cylOuterRadius
    bounds on the axial variable
      $ya $yb
    lines
     $nx = int( 2.*$pi*$cylOuterRadius/$ds +1.5);
     $ny = int( ($yb-$ya)/$ds + 1.5 );
     $nz = int($cylDeltaRadius/$ds+1.5);
     getGridPoints($nx,$ny,$nz);
       $nx $ny $nz 
    boundary conditions
      -1 -1 2 3 0 4
    share
      0 0 2 3 0 0 
  exit
*
* core of the main cylinder
*
$xac=-$cylInnerRadius; $xbc=-$xac; $yac=$ya; $ybc=$yb; $zac=$xac; $zbc=$xbc;
  Box
    mappingName
      cylinder-core
    set corners
      $xac $xbc $yac $ybc $zac $zbc
    lines
      $nx = int( ($xbc-$xac)/$ds + 1.5); 
      $ny = int( ($ybc-$yac)/$ds + 1.5); 
      $nz = int( ($zbc-$zac)/$ds + 1.5); 
     getGridPoints($nx,$ny,$nz);
       $nx $ny $nz       
    boundary conditions
      0 0 2 3 0 0 
    share
      0 0 2 3 0 0
  exit
*
*
* reduce the outer radius of the sphere as the grid is defined.
* we need to keep enough multigrid levels though.
* case 1:   $nx0=21; $ny0=21; $nz0 = 7; $innerRadius=.35; $deltaRadius=.3;  
* case 2:
  $innerRadius=.2; $deltaRadius=.25/$factor; if( $deltaRadius>.2 ){ $deltaRadius=.2; }
  $nx0 = int( $pi*($innerRadius+$deltaRadius)/$ds +1.5 );
  $stuff=$pi*$innerRadius/$ds; printf(" nx0=$nx0, pi=$pi  ds=$ds stuff=$stuff \n");
  $ny0=$nx0; $nz0 = 5;  
*
  $nzMG =int( int(($nz0+$mgFactor-1)/$mgFactor)*$mgFactor+1.5);
  $outerRadius = $innerRadius + $deltaRadius*$nzMG/$nz0/$factor; 
*
  getGridPoints($nx0,$ny0,$nz0);
*
Sphere
  inner and outer radii
    $innerRadius $outerRadius
  centre for sphere
    0. 0. 0. * 0. .5 0.  * 0. 0. 0.
  mappingName
   sphere1
exit
*
* 
* north pole of sphere 1
reparameterize
  orthographic
    specify sa,sb
      2. 2. 2.25 2.25  2.5 2.5
  exit
  lines
    * 23 23 7    15 15 11
    $nx $ny $nzMG
  boundary conditions
    0 0 0 0 1 0
  share
   0 0 0 0 1 0
  mappingName
    sphere1-north-pole
exit
* south pole of sphere 1
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      2. 2.  2.25 2.25 2.5 2.5
  exit
  lines
    $nx $ny $nzMG
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    sphere1-south-pole
exit
*
exit
generate an overlapping grid
    cylinder-core
    cylinder
    sphere1-north-pole
    sphere1-south-pole
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
*  x-r 90
  change the plot
  toggle shaded surfaces 0 0
  exit
*   pause
  exit
*
save an overlapping grid
$name
dropTube
exit

