***************************************************************************
*
*  Builds grids for regions exterior and interior to a cylinder
*
*    ogen noplot innerOuter3d 
*    ogen noplot innerOuter3d -factor=2
* 
***************************************************************************
GetOptions("factor=i"=>\$factor);
$orderOfAccuracy = "second order";
$interpType="implicit for all grids";
$explicit="explicit for all grids";
$deltaRadius0=.175;
$order4="fourth order"; 
*
* scale number of grid points in each direction by $factor
*#$factor=1; $name = "innerOuter3d.hdf"; 
$interpType=$explicit;
if ( !$factor ) {$factor=1; $name="innerOuter3d.hdf"} else {$name = "innerOuter3d$factor.hdf";};
* $factor=2; $name = "innerOuter3d2.hdf";
* $factor=2; $name = "innerOuter3d2.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=2; $name = "outer2.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=4; $name = "innerOuter3d4.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=8; $name = "innerOuter3d8.hdf"; $deltaRadius0=.25; $interpType=$explicit;
* $factor=16; $name = "innerOuter3d16.hdf"; $deltaRadius0=.25; $interpType=$explicit;
*
* -- fourth-order accurate ---
* $factor=.5; $name = "innerOuter3d0.order4.hdf";  $orderOfAccuracy=$order4; $bc = "1 2 1 1";
* $factor=1; $name = "innerOuter3d1.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=2; $name = "innerOuter3d2.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=4; $name = "innerOuter3d4.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=8; $name = "innerOuter3d8.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=16; $name = "innerOuter3d16.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
*
***************************************************************************
*
*
* domain parameters:  
$ds = .05/$factor; # target grid spacing
*
*
$ishare=100;
$bcInterface=100.;  # bc for interfaces
$pi=3.141592653;
$za=-.5; $zb=.5; 
$nz=int( ($zb-$za)/$ds+1.5 );
* 
create mappings 
*
  Cylinder
    mappingName
      innerCyl
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on the axial variable
      $za $zb
    boundary conditions 
      -1 -1 1 1 0 $bcInterface 
    share
     * material interfaces are marked by share>=100
      0 0 0 0 0 $ishare 
    lines 
      $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nTheta=$nx;
      $nx $nz $ny 
    exit 
*
  Box
    mappingName
      innerBox
    $xa=-$innerRadius-$ds;  $xb=$innerRadius+$ds; 
    $ya=-$innerRadius-$ds;  $yb=$innerRadius+$ds; 
    set corners
      $xa $xb $ya $yb $za $zb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      0 0 0 0 1 1 
    exit 
*
  Cylinder
    mappingName
      outerCyl
    $innerRadius=$outerRadius; 
    $outerRadius=$innerRadius+$deltaRadius;
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on the axial variable
      $za $zb
    lines 
      $nx=$nTheta; 
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nx $nz $ny 
    boundary conditions 
      -1 -1 1 1 $bcInterface 0 
    share
     * material interfaces are marked by share>=100
      0 0 0 0 $ishare 0 
    exit 
*
*
  Box
    mappingName
      outerBox
    $xa=-1.;  $xb=1.0; 
    $ya=-1.;  $yb=1.0; 
    set corners
      $xa $xb $ya $yb $za $zb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny $nz
    boundary conditions 
      1 1 1 1 1 1 
    exit 
  exit this menu 
*
generate an overlapping grid 
  outerBox
  outerCyl
  innerBox
  innerCyl
  done 
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      outerBox
      outerCyl
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      innerBox
      innerCyl
      done
    ghost points 
      all 
      2 2 2 2 2 2
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    exit 
*    display intermediate results
* pause
    compute overlap
* pause
  exit
save a grid
$name
innerOuter3d
exit
