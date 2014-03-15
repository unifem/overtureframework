***************************************************************************
*
* two drops in a channel -- for a moving grid computation
*
*  Examples: 
*     ogen noplot twoDrop3d -factor=1 -mgLevels=1
*     ogen noplot twoDrop3d -factor=2 -mgLevels=1
*     ogen noplot twoDrop3d -factor=4 -mgLevels=2
*
***************************************************************************
* scale number of grid points in each direction by the following factor
* $factor=1; $name = "twoDrop3d.hdf";   $mgLevels=1;
* $factor=2; $name = "twoDrop3d2.hdf";   $mgLevels=1;
* $factor=4; $name = "twoDrop3d4.hdf";   $mgLevels=2;
*
$factor=1; $mgLevels=1;
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"mgLevels=i"=>\$mgLevels,"name=s"=> \$name);
* 
if( $name eq "" ){$name = "twoDrop3d" . "$factor" . ".hdf";}
* 
printf(" factor=$factor, mgLevels=$mgLevels\n");
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
create mappings
*
box
*   $xa=-1.; $xb=1.; $ya=-3.; $yb=1.; $za=$xa; $zb=$xb; 
  $xa=-1.; $xb=1.; $ya=-2.; $yb=2.; $za=$xa; $zb=$xb; 
  set corners
*    -1. 1. -1 1 -6. 1. 
*     -1. 1. -3.  1 -1. 1. 
    $xa $xb $ya $yb $za $zb 
  lines
*     21 41 21 
*   getGridPoints(21,41,21);
  $nx= int( ($xb-$xa)*10+1.5 ); $ny= int( ($yb-$ya)*10+1.5 ); $nz= int( ($zb-$za)*10+1.5 ); 
  getGridPoints($nx,$ny,$nz);
    $nx $ny $nz
  boundary conditions
    1 1 1 1 1 1
  mappingName
   channel
exit
*
* reduce the oudter radius of the sphere as the grid is defined.
* we need to keep enough multigrid levels though.
* case 1:  
*   $nx0=21; $ny0=21; $nz0 = 7; $innerRadius=.35; $deltaRadius=.3;  
* case 2:
*   $nx0=17; $ny0=17; $nz0 = 7; $innerRadius=.125; $deltaRadius=.2;
* case 3:  
  $nx0=13; $ny0=13; $nz0 = 5; $innerRadius=.25; $deltaRadius=.225;  
*
  $nzMG =int( int(($nz0+$mgFactor-1)/$mgFactor)*$mgFactor+1.5);
  $outerRadius = $innerRadius + $deltaRadius*$nzMG/$nz0/$factor;
*
  getGridPoints($nx0,$ny0,$nz0);
*
* ------------- sphere 1 ---------------
*
Sphere
  inner and outer radii
    $innerRadius $outerRadius
  centre for sphere
    0. 0. 0.
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
* ------------- sphere 2 ---------------
*
Sphere
  inner and outer radii
    $innerRadius $outerRadius
  centre for sphere
    0. 1.  0.
  mappingName
   sphere2
exit
*
* 
* north pole of sphere 2
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
   0 0 0 0 2 0
  mappingName
    sphere2-north-pole
exit
* south pole of sphere 2
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
    0 0 0 0 2 0
  mappingName
    sphere2-south-pole
exit
*
exit
generate an overlapping grid
    channel
    sphere1-north-pole
    sphere1-south-pole
    sphere2-north-pole
    sphere2-south-pole
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
twoDrop3d
exit

