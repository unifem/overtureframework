*
* Grid for the superseismic FSI example (cgmp)
*
*
* usage: ogen [noplot] superseismicGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot superseismicGrid -factor=1 
*     ogen noplot superseismicGrid -factor=2 
*     ogen noplot superseismicGrid -factor=4 
*     ogen noplot superseismicGrid -factor=8 
*     ogen noplot superseismicGrid -factor=16
*     ogen noplot superseismicGrid -factor=32
* 
* Past time grids:
*     ogen noplot superseismicGrid -factor=1 -t=-.01 -name="superseismicGrid1tmp01.hdf"
*     ogen noplot superseismicGrid -factor=2 -t=-.01 -name="superseismicGrid2tmp01.hdf"
*     ogen noplot superseismicGrid -factor=4 -t=-.01 -name="superseismicGrid4tmp01.hdf"
*     ogen noplot superseismicGrid -factor=8 -t=-.01 -name="superseismicGrid8tmp01.hdf"
*     ogen noplot superseismicGrid -factor=16 -t=-.01 -name="superseismicGrid16tmp01.hdf"
*     ogen noplot superseismicGrid -factor=32 -t=-.01 -name="superseismicGrid32tmp01.hdf"
* 
*     ogen noplot superseismicGrid -factor=1 -t=-.02 -name="superseismicGrid1tmp02.hdf"
*     ogen noplot superseismicGrid -factor=2 -t=-.02 -name="superseismicGrid2tmp02.hdf"
*     ogen noplot superseismicGrid -factor=4 -t=-.02 -name="superseismicGrid4tmp02.hdf"
*     ogen noplot superseismicGrid -factor=8 -t=-.02 -name="superseismicGrid8tmp02.hdf"
*     ogen noplot superseismicGrid -factor=16 -t=-.02 -name="superseismicGrid16tmp02.hdf"
*     ogen noplot superseismicGrid -factor=32 -t=-.02 -name="superseismicGrid32tmp02.hdf"
* 
*
$order=2; $factor=1; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.; $t=0; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "t=f"=> \$t,"interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "superseismicGrid" . "$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
* 
$dw = $order+1; $iw=$order+1; 
*
$bcInterface=100;  # bc for interfaces
$shareInterface=100;        # share value for interfaces
* 
create mappings
*
* rectangle for the lower solid region
rectangle
  set corners
    $xa $xb $ya 0.
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( (0.-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 1 1 $bcInterface
  share 
    0 0 0 $shareInterface
  mappingName
    solidGrid
exit
*
* Make a single hyperbolic grid for the fluid region
* 
# First make a curve for the fluid interface based on the 
# initial solid displacement
# undeformed normal : (normalOption=1)
# xi=6.047047e-02 (3.464703e+00 degrees)
# theta=1.551374e-01 (8.888718e+00 degrees) --normals should be the same, these have more correct digits: 
#  p-wave: alphap=3.079172e-01 normal: [n1,n2] = [5.328205e-01,-8.462283e-01]
#  s-wave: alphas=-1.229946e-01 normal: [n1,n2] = [2.683906e-01,-9.633102e-01]
$app=3.079172e-01; $k1p=5.328205e-01; $k2p=-8.462283e-01; $k3p=0.; 
$aps=-1.229946e-01; $k1s=2.683906e-01; $k2s=-9.633102e-01; $k3s=0.;
$rhoSolid=1.; $lambda=1.398307e-01; $mu=7.203351e-02; 
$cp=sqrt(($lambda+2.*$mu)/$rhoSolid); $cs=sqrt($mu/$rhoSolid);
$xaShock=0.; $yaShock=0.; # location of the shock on the interface
#$xi = $k1p*($x0-$xaShock) + $k2p*($y0-$yaShock) - $cp*$t;
#	  u1 += -ap*k1*xi;
#	  u2 += -ap*k2*xi;
$n=$nx; $a=$xa; $b=$xb; $h=($b-$a)/($n-1);
$cmd="";
# -- Evaluate the traveling wave solution on the interface --
#  add contributions from the p and s waves (see cg/sm/src/forcing.h)
for( $i=0; $i<$n; $i++){ $r1=$a + $h*$i; $r2=0.; \
$x=$r1; $y=$r2; \
$xi = $k1p*($r1-$xaShock) + $k2p*($r2-$yaShock) - $cp*$t;\
if( $xi<0. ){ $x=$x-$app*$k1p*$xi; $y=$y-$app*$k2p*$xi; }\
$xi = $k1s*($r1-$xaShock) + $k2s*($r2-$yaShock) - $cs*$t;\
if( $xi<0. ){ $x=$x+$aps*$k2s*$xi; $y=$y-$aps*$k1s*$xi; }\
$cmd=$cmd . "$x $y\n"; }
# -- this next is a guess: **fix me using actual formula **
#for( $i=0; $i<$n; $i++){ $x=$a + $h*$i; \
#if( $x<0. ){ $y=.148*$x; $x=.945*$x; }else{ $y=0.}\
#$cmd=$cmd . "$x $y\n"; }
#
$degree=3; 
nurbs (curve)
parameterize by index (uniform)
enter points
$n $degree
$cmd 
lines
  $nx
mappingName
  interfaceCurve
# pause
exit
* 
#  line (2D)
#    set end points
#      $xa $xb 0. 0.
#    lines
#      $nx
#    exit
  hyperbolic
    # use fourth order interpolant to define the mapping:
    $dist= $yb-0.;
    backward
    distance to march $dist
    $ny = int( ($yb-0.)/$ds +.5 ); 
    lines to march $ny 
    points on initial curve $nx
    generate
    fourth order
    boundary conditions
     1 2 $bcInterface 4
    share
     0 0 $shareInterface 0
    name fluidGrid
    * pause
  exit
*
*
exit
generate an overlapping grid
    solidGrid
    fluidGrid
  done
  change parameters
    specify a domain
     solidDomain
     solidGrid
    done
    specify a domain
     fluidDomain
     fluidGrid
    done
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
**  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
superseismicGrid
exit

