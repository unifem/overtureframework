*
*  A annular shell separating two regions
*
***************************************************************************
$orderOfAccuracy = "second order";
$interpType="implicit for all grids";
$explicit="explicit for all grids";
*
$deltaRadius0=.175;  $shellThickness=.15; 
*
$order4="fourth order"; 
*
* scale number of grid points in each direction by $factor
* $factor=.5; $name = "cylShell0.hdf";
* $factor=1; $name = "cylShell.hdf";
$factor=2; $name = "cylShell2.hdf";  
* $factor=2; $name = "outer2.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=4; $name = "cylShell4.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=8; $name = "cylShell8.hdf"; $deltaRadius0=.25; $interpType=$explicit;
* $factor=16; $name = "cylShell16.hdf"; $deltaRadius0=.25; $interpType=$explicit;
*
* -- fourth-order accurate ---
* $factor=.5; $name = "cylShell0.order4.hdf";  $orderOfAccuracy=$order4; $bc = "1 2 1 1";
* $factor=1; $name = "cylShell1.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=2; $name = "cylShell2.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=4; $name = "cylShell4.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=8; $name = "cylShell8.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=16; $name = "cylShell16.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
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
$bcInterface=100;   # bc for interfaces
$ishareInner=100;
$ishareOuter=101;
* 
create mappings 
*
  annulus 
    mappingName 
      innerAnnulus 
    boundary conditions 
      -1 -1 0 $bcInterface 
    share
     * material interfaces are marked by share>=100
      0 0 0 $ishareInner
    $pi=3.141592653;
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    inner and outer radii 
      $innerRadius $outerRadius
*       $innerRadius $rad2
    lines 
      $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nTheta=$nx;
      $nx $ny 
    exit 
*
  rectangle 
    mappingName
      innerSquare
    $xa=-$innerRadius-$ds;  $xb=$innerRadius+$ds; 
    $ya=-$innerRadius-$ds;  $yb=$innerRadius+$ds; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      0 0 0 0
    exit 
*
* ----------------SHELL---------------------------------
  annulus 
    mappingName 
      shell
    $innerRadius=$outerRadius;
    $outerRadius=$innerRadius+$shellThickness;
    inner and outer radii 
      $innerRadius $outerRadius
    lines 
      $nx=$nTheta; 
      $ny=int( ($shellThickness)/$ds+2.5 );
      $nx $ny
    boundary conditions 
      -1 -1 $bcInterface $bcInterface 
    share
     * material interfaces are marked by share>=100
      0 0 $ishareInner $ishareOuter
    exit 
* --------------------------------------------------------
*
  annulus 
    mappingName 
      outerAnnulus 
    $innerRadius=$outerRadius;
    $outerRadius=$innerRadius+$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
    lines 
      $nx=$nTheta; 
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nx $ny
    boundary conditions 
      -1 -1 $bcInterface 0 
    share
     * material interfaces are marked by share>=100
      0 0 $ishareOuter 0   
    exit 
*
  rectangle 
    mappingName
      outerSquare
    $xa=-1.;  $xb=1.0; 
    $ya=-1.;  $yb=1.0; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    exit 
  exit this menu 
*
generate an overlapping grid 
  outerSquare
  outerAnnulus 
  shell
  innerSquare
  innerAnnulus 
  done 
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      outerSquare
      outerAnnulus
      done
    specify a domain
      * domain name:
      shellDomain 
      * grids in the domain:
      shell
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      innerSquare
      innerAnnulus
      done
    ghost points 
      all 
      2 2 2 2 
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
cylShell
exit
