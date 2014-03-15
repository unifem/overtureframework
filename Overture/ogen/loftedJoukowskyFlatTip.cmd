#
# Twisted Joukosky airfoil with a flattend tip
#
# usage: ogen [noplot] loftedJoukowskyFlatTip -factor=<num> -order=[2/4/6/8] -interp=[e/i]
#
# examples:
#     ogen -noplot loftedJoukowskyFlatTip -factor=1 -order=2
#     ogen -noplot loftedJoukowskyFlatTip -interp=e -factor=1 -order=2
#     ogen -noplot loftedJoukowskyFlatTip -interp=e -factor=2 -order=2 ??
#
$xa=-.75; $xb=.75; $ya=-.4; $yb=.6; $za=0.; $zb=3.5; 
$nrExtra=2; $loadBalance=0; $ml=0; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "loftedJoukowskyFlatTip" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.01/$factor;
# 
create mappings
#
#  Build a tip grid on a lofted Joukowsky surface
#  mbuilder loftedJoukowskyFlatTip
#
  lofted surface
    twisted Joukowsky sections
    flat tip profile
    lines
      161 101
    mappingName
      loftedSurface
    share
      0 0 0 0 7 0     
    exit
  builder
    Start curve:loftedSurface
#
#  Cap of wing
#
    create surface grid...
      Pick rotation point:0
           -0.000452495 0.252276 2.99926 
      surface grid options...
      target grid spacing .01 .01(tang,normal, <0 : use default)
      initial curve:points on surface
      choose point on surface 0 -3.156209e-01 3.444051e-01 2.904446e+00 7.696758e-01 4.669674e-01
      choose point on surface 0 -2.797280e-01 3.531426e-01 2.952217e+00 8.115623e-01 4.563484e-01
      choose point on surface 0 -2.093377e-01 3.393780e-01 2.987000e+00 8.728527e-01 4.477450e-01
      choose point on surface 0 -1.286100e-01 3.125689e-01 2.996674e+00 9.242720e-01 4.278801e-01
      choose point on surface 0 -6.491677e-02 2.856211e-01 2.998836e+00 9.602424e-01 4.072957e-01
      choose point on surface 0 3.021346e-02 2.392298e-01 2.999165e+00 9.728445e-01 1.798981e-01
      choose point on surface 0 9.854584e-02 1.937458e-01 2.997089e+00 9.288840e-01 1.313675e-01
      choose point on surface 0 1.441633e-01 1.560538e-01 2.993336e+00 8.992087e-01 1.046492e-01
      choose point on surface 0 2.104677e-01 9.701592e-02 2.975831e+00 8.456825e-01 7.478294e-02
      choose point on surface 0 2.470107e-01 5.726082e-02 2.948315e+00 8.072525e-01 4.864640e-02
      choose point on surface 0 2.705409e-01 3.008081e-02 2.908405e+00 7.725228e-01 3.214890e-02
      done
      forward and backward
      name wingCapSurface
      lines to march 13 13 (forward,backward)  
      generate
      exit
#
#   Wing surface
#
    create surface grid...
      target grid spacing .01 .01 (tang,normal, <0 : use default)
      surface grid options...
      initial curve:coordinate line 0
      choose point on surface 0 -2.248340e-02 1.048172e-01 0. 0. 2.639995e-01
      done
      forward
      lines to march 297
      generate
      name wingSurface
      exit
#
# volume grid for the wing
#
    create volume grid...
      target grid spacing .01 .01 (tang,normal, <0 : use default)
      BC: bottom fix z, float x and y
      generate
      spacing: geometric
      geometric stretch factor 1.05 
      generate
      boundary conditions
        -1 -1 5 0 7 0
      share
         0  0 5 0 7 0
      name wingVolume
      exit
#
# Volume grid for the wing-cap
#
    create volume grid...
      Start curve:wingCapSurface
      target grid spacing .01 .01 (tang,normal, <0 : use default)
      marching spacing...
      geometric stretch factor 1.05 
      forward
      generate
      boundary conditions
        0 0 0 0 7 0
      share
        0 0 0 0 7 0
      name wingCapVolume
      exit
    exit
#
# Here is the back-ground grid
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 5 0
  mappingName
    backGround
  exit
exit this menu
#
generate an overlapping grid
  backGround
  wingVolume
  wingCapVolume
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
open graphics

 compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
sib
exit








      choose point on surface 0 -2.410388e-01 3.422794e-01 2.978681e+00 8.514320e-01 4.652045e-01
      choose point on surface 0 -1.628619e-01 3.202447e-01 2.994577e+00 9.068500e-01 4.505797e-01
      choose point on surface 0 -9.410754e-02 2.919256e-01 2.998369e+00 9.485694e-01 4.464914e-01
      choose point on surface 0 -3.776128e-02 2.656368e-01 2.999291e+00 9.802975e-01 4.621568e-01
      choose point on surface 0 3.509328e-02 2.288006e-01 2.999234e+00 9.765703e-01 1.205724e-01
      choose point on surface 0 8.028339e-02 1.950421e-01 2.998378e+00 9.487586e-01 8.935211e-02
      choose point on surface 0 1.304504e-01 1.560868e-01 2.995645e+00 9.147762e-01 7.329898e-02
      choose point on surface 0 1.776541e-01 1.148768e-01 2.988650e+00 8.784227e-01 5.232247e-02
      done
      forward and backward
      set view:0 -0.0128411 0.0160514 0 4.48201 1 0 0 0 1 0 0 0 1
      generate
