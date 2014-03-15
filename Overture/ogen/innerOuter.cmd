***************************************************************************
*
*  Builds grids for regions exterior and interior to a boundary
*
* usage: ogen [noplot] innerOuter -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name=<> -deltaRad=<val>
* 
* examples:
*     ogen noplot innerOuter -factor=2 -order=2 -interp=e 
*     ogen noplot innerOuter -factor=2 -order=4 -deltaRad=.5 -interp=e -name="innerOuter2.order4.hdf"
*     ogen noplot innerOuter -factor=4 -order=4 -deltaRad=.5 -interp=e -name="innerOuter4.order4.hdf"
*     ogen noplot innerOuter -factor=8 -order=4 -deltaRad=.5 -interp=e -name="innerOuter8.order4.hdf"
* 
***************************************************************************
$orderOfAccuracy = "second order";
$interpType="implicit for all grids";
$explicit="explicit for all grids";
$deltaRadius0=.175;
$order4="fourth order"; 
*
* scale number of grid points in each direction by $factor
* $factor=1; $name = "innerOuter.hdf"; $interpType=$explicit;
* $factor=2; $name = "innerOuter2.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=2; $name = "outer2.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=4; $name = "innerOuter4.hdf";  $deltaRadius0=.25; $interpType=$explicit;
* $factor=8; $name = "innerOuter8.hdf"; $deltaRadius0=.25; $interpType=$explicit;
* $factor=16; $name = "innerOuter16.hdf"; $deltaRadius0=.25; $interpType=$explicit;
*
* -- fourth-order accurate ---
* $factor=.5; $name = "innerOuter0.order4.hdf";  $orderOfAccuracy=$order4; $bc = "1 2 1 1";
* $factor=1; $name = "innerOuter1.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=2; $name = "innerOuter2.order4.hdf";  $orderOfAccuracy=$order4;
* $factor=4; $name = "innerOuter4.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=8; $name = "innerOuter8.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
* $factor=16; $name = "innerOuter16.order4.hdf";  $orderOfAccuracy=$order4; $deltaRadius0=.5; $interpType=$explicit;
*
* get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"deltaRad=f"=> \$deltaRadius0,"name=s"=>\$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){ $name = "innerOuter" . "$interp$factor" . $suffix . ".hdf"; }
*
***************************************************************************
*
*
* domain parameters:  
$ds = .05/$factor; # target grid spacing
*
*
create mappings 
*
  annulus 
    mappingName 
      innerAnnulus 
    boundary conditions 
      -1 -1 0 1 
    share
     * material interfaces are marked by share>=100
      0 0 0 100
    $pi=3.141592653;
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
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
      -1 -1 2 0 
    share
     * material interfaces are marked by share>=100
      0 0 100 0   
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
      innerDomain 
      * grids in the domain:
      innerSquare
      innerAnnulus
      done
    ghost points 
      all 
      $ng $ng $ng $ng $ng $ng 
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
innerOuter
exit
