***************************************************************************
*
*  Build grids for an interface with a divot
*
* usage: ogen [noplot] divot -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot divot -factor=1 -order=2
*     ogen noplot divot -factor=1 -order=4
*     ogen noplot divot -factor=2 -order=4
*     ogen noplot divot -interp=e -factor=4 -order=4
*     ogen noplot divot -interp=e -factor=8 -order=4
*     ogen noplot divot -interp=e -factor=16 -order=4
*     ogen noplot divot -interp=e -factor=20 -order=4
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $extra=0; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$orderOfAccuracy+2; }
if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "divot" . "$interp$factor" . $suffix . ".hdf";}
* 
* domain parameters:  
$ds = .01/$factor; # target grid spacing
*
*
$bcInterface=100;  # bc for interfaces
$ishare=100;
* 
create mappings 
*
** -- nurbs --
* 
$pi =4.*atan2(1.,1.); $cmd="";
$n1=21; $a=-1.; $b=-.5; $h=($b-$a)/($n1-1);
for( $i=0; $i<$n1-1; $i++){ $x=$a + $h*$i; $y=0.; $cmd=$cmd . "$x $y\n"; }
$amp=.25; 
$n2=21; $a=-.5; $b=.5; $h=($b-$a)/($n2-1);
for( $i=0; $i<$n2; $i++){ $x=$a + $h*$i; $y=$amp*(-1.-cos(2.*$pi*$x)); $cmd=$cmd . "$x $y\n"; }
$n3=21; $a=.5; $b=1.; $h=($b-$a)/($n3-1);
for( $i=1; $i<$n3; $i++){ $x=$a + $h*$i; $y=0.; $cmd=$cmd . "$x $y\n"; }
$n=($n1-1)+$n2+($n3-1); 
create mappings 
  * 
  nurbs (curve)
    enter points
     $degree=5; 
     $n $degree
     $cmd
    mappingName
    interfaceCurve
    exit
* 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    $nr=7+$extra; 
    normal distance
      $dist=($nr-2)*$ds; 
      $dist 
    lines
      $length=2. + $amp*2.; 
      $nx = int( $length/$ds + 1.5 );
      $nx $nr 
    boundary conditions
      1 2 $bcInterface 0
    share
      1 2 $ishare 0
    mappingName
     lowerInterface
  exit
* 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    normal distance
      -$dist 
    lines
      $nx $nr
    boundary conditions
      1 2 $bcInterface 0
    share
      1 2 $ishare 0
    mappingName
     upperInterface
  exit
*
  $xa=-1.; $xb=1.; $ya=-1.5; $yb=0.;
  rectangle 
    mappingName
      lower
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      1 2 3 0
    share
      1 2 0 0 
    exit 
*
  $xa=-1.; $xb=1.; $ya=-2.*$amp; $yb=1.;
  rectangle 
    mappingName
      upper
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      1 2 0 4
    share
      1 2 0 0 
    exit 
exit
*
generate an overlapping grid 
  lower
  upper
  lowerInterface
  upperInterface
  done 
*
  change parameters
    specify a domain
      * domain name:
      lowerDomain 
      * grids in the domain:
      lower
      lowerInterface
      done
    specify a domain
      * domain name:
      upperDomain 
      * grids in the domain:
      upper
      upperInterface
      done
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit
  compute overlap
  exit
*
save an overlapping grid
$name
divot
exit
