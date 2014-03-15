*
* Circle in a channel (taking arguments)
*
*
* usage: ogen [noplot] vpCylGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -periodic=[true|false] -x0=<> -y0=<> -name=<>
*
*  -periodic : if true, create a periodic channel
*  -x0, -y0 : initial centre of the annulus
* 
* examples:
*     ogen noplot vpCylGrid -factor=1 -order=2              (creates vpCylGridi1.order2.hdf)
*     ogen noplot vpCylGrid -factor=2 -order=2              (creates vpCylGridi2.order2.hdf)
* 
*   cylinder offset to x0=2: 
*     ogen noplot vpCylGrid -factor=2 -order=2 -x0=2. -name=vpCylGridi2Offset2.order2.hdf
*     ogen noplot vpCylGrid -factor=4 -order=2 -x0=2. -name=vpCylGridi4Offset2.order2.hdf
* 
*     ogen noplot vpCylGrid -factor=1 -order=2 -periodic=true -x0=2. -name=vpCylGridi1p.order2.hdf
*     ogen noplot vpCylGrid -factor=2 -order=2 -periodic=true -x0=2. -name=vpCylGridi2p.order2.hdf
* 
*     ogen noplot vpCylGrid -factor=1 -order=4
*     ogen noplot vpCylGrid -factor=2 -order=4
*     ogen noplot vpCylGrid -factor=2 -order=2 -interp=e    ( creates vpCylGride2.order2.hdf)
*     ogen noplot vpCylGrid -factor=3 -order=2 -interp=e    ( creates vpCylGride3.order2.hdf)
*     ogen noplot vpCylGrid -factor=4 -order=2 -interp=e    ( creates vpCylGride4.order2.hdf)
* 
*     ogen noplot vpCylGrid -factor=2 -order=2 -interp=e -xa=-5. -xb=5. -name="vpCylGride2L10.hdf"
*     ogen noplot vpCylGrid -factor=4 -order=2 -interp=e -xa=-5. -xb=5. -name="vpCylGride4L10.hdf"
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-4.; $xb=4.; $ya=-2.; $yb=2.; $periodic="false"; $x0=0.; $y0=0.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"periodic=s"=> \$periodic,"name=s"=> \$name,"x0=f"=>\$x0,"y0=f"=>\$y0);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "vpCylGrid" . "$interp$factor" . $suffix . ".hdf";}
* 
$bcWall=1; $bcInflow=2; $bcOutflow=3; 
$ds=.1/$factor;
* 
create mappings
*
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    $bc="$bcInflow $bcOutflow $bcWall $bcWall"; 
    if( $periodic eq "true" ){ $bc="-1 -1 $bcWall $bcWall"; }
    $bc
    * 2 3 1 1
  mappingName
  square
exit
*
Annulus
  $innerRad=.5; $outerRad = .8;
  inner and outer radii
    $innerRad $outerRad
  centre for annulus
    $x0 $y0
  lines
    $nTheta = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $stretchFactor=1.25; # add extra points to account for stretching
    $nr = int( $stretchFactor*($outerRad-$innerRad)/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0
exit
* 
* stretch the annulus
*
* Stretch coordinates
stretch coordinates
  transform which mapping?
  Annulus
  stretch
    specify stretching along axis=1
      layers
      1
      1. 7. 0.
      exit
    exit
  mappingName
    annulus
  exit
*
*
exit
generate an overlapping grid
    square
    annulus
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
*  display intermediate results
  compute overlap
*   plot
**  display computed geometry
  exit
*
save an overlapping grid
$name
vpCylGrid
exit

