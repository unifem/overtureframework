* 
* two drops in a channel 
* 
*************************************************************************** 
* scale number of grid points in each direction by the following factor 
*   ***NOTE: restore file to factor=1 for regression tests ***** 
* $factor=.5; $name = "twoDropNew0.hdf"; $mgLevels=2; 
* $factor=1; $name = "twoDropNew.hdf";   $mgLevels=2; 
* $factor=2; $name = "twoDropNew2.hdf";  $mgLevels=3; 
$factor=4; $name = "twoDropNew4.hdf";   $mgLevels=3; 
printf(" factor=$factor, mgLevels=$mgLevels\n"); 
* 
*----- 
$mgFactor=2**$mgLevels; 
*----- 
* 
* Define a subroutine to convert the number of grid points 
sub getGridPoints{ local($n1,$n2)=@_;   $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5);   $nx=int( int(($nx-1)/$mgFactor)*$mgFactor+1.5);   $ny=int( int(($ny-1)/$mgFactor)*$mgFactor+1.5); } 
* 
*************************************************************************** 
create mappings 
