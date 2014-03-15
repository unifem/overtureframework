***************************************************************************
*
*  Builds grids for a multi-domain problem with half an annulus 
*    for an axi-symmetric problem
*
* usage: ogen [noplot] hio -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot hio -factor=1
*     ogen noplot hio -factor=2
*
***************************************************************************
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$deltaRadius0=.175;
$name=""; 
* $xa=-1.; $xb=1.; $ya=0.; $yb=1.;   # these don't work yet
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "hio" . "$interp$factor" . $suffix . ".hdf";}
* 
*
* domain parameters:  
$ds = .05/$factor; # target grid spacing
*
*
$bcInterface=100;  # bc for interfaces
$ishare=100;
$pi=4.*atan2(1.,1.);
* 
create mappings 
*
  annulus 
    mappingName 
      innerAnnulus 
* 
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    inner and outer radii 
      $innerRadius $outerRadius
    start and end angles
      0. .5
    lines 
      $nx=int( ($pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nTheta=$nx;
      $nx $ny 
    boundary conditions 
      3 3 0 $bcInterface 
    share
     * material interfaces are marked by share>=100
      3 3 0 $ishare
    exit 
*
  rectangle 
    mappingName
      innerSquare
    $xa=-$innerRadius-$ds;  $xb=$innerRadius+$ds; 
    $ya=0.;                 $yb=$innerRadius+$ds; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      0 0 3 0
    share 
      0 0 3 0 
    exit 
*
  annulus 
    mappingName 
      outerAnnulus 
    $innerRadius=$outerRadius; 
    $outerRadius=$innerRadius+$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
    start and end angles
      0. .5
    lines 
      $nx=$nTheta; 
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nx $ny
    boundary conditions 
      3  3 $bcInterface 0 
    share
     * material interfaces are marked by share>=100
      3 3 $ishare 0   
    exit 
*
  rectangle 
    mappingName
      outerSquare
    $xa=-1.;  $xb=1.0; 
    $ya=0.;   $yb=1.0; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny 
    boundary conditions 
      1 2 3 4 
    share
     * material interfaces are marked by share>=100
      0 0 3 0 
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
hio
exit
