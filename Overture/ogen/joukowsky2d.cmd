#
# Create an overlapping grid for 2D Joukowsky wing
#
# usage: ogen [noplot] joukowsky2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
#  ogen -noplot joukowsky2d -factor=2
#  ogen -noplot joukowsky2d -factor=4
# - explicit interp: 
#  ogen -noplot joukowsky2d -interp=e -factor=2
#  ogen -noplot joukowsky2d -interp=e -factor=4
#  ogen -noplot joukowsky2d -interp=e -factor=8
#
# -- multigrid:
#  ogen -noplot joukowsky2d -ml=3 -factor=1
#  ogen -noplot joukowsky2d -ml=3 -interp=e -factor=1
#  ogen -noplot joukowsky2d -ml=3 -interp=e -factor=2
#  ogen -noplot joukowsky2d -ml=3 -interp=e -factor=4
#  ogen -noplot joukowsky2d -ml=3 -interp=e -factor=8
#  ogen -noplot joukowsky2d -ml=3 -interp=e -factor=16
#  ogen -noplot joukowsky2d -interp=e -order=2 -ml=4 -factor=32
#
# -- order 4 and MG
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=2 -factor=2
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=3 -factor=4
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=3 -factor=8
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=3 -factor=16
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=4 -factor=32
#  ogen -noplot joukowsky2d -interp=e -order=4 -ml=4 -factor=64
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.5; $xb=4.; $ya=-1.5; $yb=1.5; 
$ml=0; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "joukowsky2d" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.05/$factor;
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
$chord=1.;  # chord 
#
create mappings
  airfoil
    airfoil type
    joukowsky
    joukowsky delta
      -15
    mappingName
      airfoil
    exit
# 
  rotate/scale/shift
    scale
     $scale=$chord/4.; 
      $scale $scale
    rotate
     0
     0 0
    shift
      0 0
    mappingName
      airfoilScaled
  exit
#
  mapping from normals
    extend normals from which mapping?
      airfoilScaled
    normal distance
      # $nr = intmg( 17 );
      $nr = intmg( 13 + 2*$order );
      $nDist = ($nr-10)*$ds; 
      $nDist
    lines
      $cfact=1.5; # use more points in the tangential to account for stretching
      $nTheta = intmg( ($cfact*2.*$chord)/$ds +1.5 ); 
      $nTheta $nr
    mappingName
      wingInitial
  exit
# stretch grid
  stretch coordinates
    Stretch r2:itanh
    $mindx = .01/$factor; 
    STP:stretch r2 itanh: position and min dx 0 $mindx
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 1 6 .3 (id>=0,weight,exponent,position)
    STP:stretch r1 itanh: layer 1 1 4 0.7 (id>=0,weight,exponent,position)
    stretch grid
    mappingName
     wingStretched
  exit
# To be safe, move the branch cut to be in the middle of the wing instead of the trailing edge.
  reparameterize
    restrict parameter space
      set corners
      -.25 .75
      exit
    boundary conditions
      -1 -1 1 0
    periodicity
      2 0
   mappingName
    wingShifted
  exit
# turn into a nurbs for faster evaluation
  nurbs (curve) 
    interpolate from mapping with options
    wingShifted
    parameterize by index (uniform)
    #number of ghost points to include
    #  1
    done
    mappingName
     wing
    exit
# 
  rectangle
    mappingName
      backGround
    set corners
      $xa $xb $ya $yb
    lines
      $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
    boundary conditions
      1 2 3 3 
  exit
exit
generate an overlapping grid
    backGround
    wing
  done
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  debug
#    7
#  display intermediate results
#  plot
  compute overlap
#  pause
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
jwing
exit
