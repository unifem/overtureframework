*
* one drop in a channel
*
***************************************************************************
* scale number of grid points in each direction by the following factor
*   ***NOTE: restore file to factor=1 for regression tests *****
* $factor=.5; $name = "oneDrop0.hdf"; $mgLevels=2;
$factor=1; $name = "oneDrop1.hdf";   $mgLevels=2;
* $factor=2; $name = "oneDrop2.hdf";  $mgLevels=3;
* $factor=4; $name = "oneDrop4.hdf";   $mgLevels=3;   
* $factor=8; $name = "oneDrop8.hdf";   $mgLevels=4;   
printf(" factor=$factor, mgLevels=$mgLevels\n");
*
*-----
  $mgFactor=2**$mgLevels;
*-----
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
  $nx=int( int(($nx-1)/$mgFactor)*$mgFactor+1.5); if( $nx==1 ){ $nx=int($mgFactor+1.5); } \
  $ny=int( int(($ny-1)/$mgFactor)*$mgFactor+1.5); if( $ny==1 ){ $ny=int($mgFactor+1.5); }\
}
*
***************************************************************************
create mappings
*
rectangle
  # $xa =-1.; $xb=1.; $ya=-3.; $yb=1.; 
  $xa =-1.; $xb=1.; $ya=-1.; $yb=1.; 
  set corners
    $xa $xb $ya $yb 
  lines
    $nx = int( 25*($xb-$xa)+1.5);  $ny=int( 25*($yb-$ya)+1.5);
    getGridPoints($nx,$ny);
    $nx $ny 
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  lines
    $nya=9;  # fix radial lines since outer radius is reduced
    $nymg=int( int(($nya+$mgFactor-1)/$mgFactor)*$mgFactor+1.5); 
   getGridPoints(57,$nymg);
    $nx $nymg
  inner and outer radii
    $innerRadius=.25;
    $outerRadius=$innerRadius+(.25/$factor)*($nymg/$nya);
    $innerRadius $outerRadius
  centre for annulus
    0. 0. 
  boundary conditions
    -1 -1 1 0
  mappingName
   drop-unstretched
exit
*
  stretch coordinates
    Stretch r2:itanh
    STP:stretch r2 itanh: layer 0 1 4 0 (id>=0,weight,exponent,position)
    stretch grid
    mappingName
     drop
   exit
*
*
exit
generate an overlapping grid
    channel
    drop
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
*  pause
  exit
*
save an overlapping grid
$name
oneDrop
exit

