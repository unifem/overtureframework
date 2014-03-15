#
# two-dimensional centrifgual pump
#
#
# usage: ogen [noplot] pump2dGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -xScaleBlade=<> -yScaleBlade=<> 
#
# Examples:
#    ogen -noplot pump2dGrid -interp=e -factor=4
#    ogen -noplot pump2dGrid -interp=e -factor=8
#    ogen -noplot pump2dGrid -interp=e -factor=16
# -- multigrid
#    ogen -noplot pump2dGrid -interp=e -ml=1 -factor=4
#    ogen -noplot pump2dGrid -interp=e -ml=1 -factor=8
#    ogen -noplot pump2dGrid -interp=e -ml=2 -factor=8
#    ogen -noplot pump2dGrid -interp=e -ml=2 -factor=16
# -- fourth-order
#    ogen -noplot pump2dGrid -interp=e -order=4 -ml=1 -factor=8 (OK
#    ogen -noplot pump2dGrid -interp=e -order=4 -ml=2 -factor=16 (OK  360K pts
#    ogen -noplot pump2dGrid -interp=e -order=4 -ml=3 -factor=32 (OK  1.2M pts
#    ogen -noplot pump2dGrid -interp=e -order=4 -ml=3 -factor=64 (OK  4.8M pts
#    ogen -noplot pump2dGrid -interp=e -order=4 -ml=4 -factor=64 (OK  4.8M pts
#
#
$xScaleBlade=1.1; $yScaleBlade=1.1; # scale factors for blade size
# $xScaleBlade=1.0; $yScaleBlade=1.0; # scale factors for blade size
# 
$prefix="pump2dGrid";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name="";
$blf=8;  # stretching factor - boundary layer spacing is this many times smaller than target spacing $ds
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "xScaleBlade=f"=>\$xScaleBlade,"yScaleBlade=f"=>\$yScaleBlade );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
# if( $branch ne 0 ){ $prefix = $prefix . "Branch"; }
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
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
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
# 
#
create mappings
#
#
#
# First make a background grid  
#
 rectangle
   mappingName
     background
   set corners
     $xa=-1.45; $xb=1.7; $ya=-1.55; $yb=1.8; 
     $xa $xb $ya $yb
   lines
     $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
     $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
     $nx $ny
    #101 101
   boundary conditions
     0 0 0 0  
 exit
#
#create outlet rectangle
#
   rectangle
    mappingName
     outlet_rec
    set corners
      $xar=-.25; $xbr=-.15; $yar=1.42+3.*$ds; $ybr=1.8-3.*$ds; 
      $xar $xbr $yar $ybr
      # -.25 -.15 1.47 1.75 
    lines
     $nx = intmg( ($xbr-$xar)/$ds +1.5 ); 
     $ny = intmg( ($ybr-$yar)/$ds +1.5 ); 
     $nx $ny
    boundary conditions
      2 0 0 0
    share
      2 0 0 0  
   exit   
#  
#create a 2D line
#
   line
    number of dimensions
     2
    set end points
     -.25 -.25 1.42 1.8
    mappingName
     outlet_line
   exit
#
# map blade geometry
#
SmoothedPolygon
# start on a side so that the polygon is symmetric
  vertices 
  31
 -0.0250  -.0001 
 -0.0217  0.0124 
  0.0127  0.0356
  0.0859  0.0702
  0.2004  0.1074
  0.3453  0.1250
  0.5018  0.1196
  0.6581  0.1028
  0.7990  0.0778
  0.9109  0.0514
  0.9826  0.0314
  0.9962  0.0186
  1.0099  0.0059  
  1.0235 -0.0068
  1.0051 -0.0101
  0.9868 -0.0133
  0.9684 -0.0166
  0.8982  0.0030 
  0.7888  0.0289 
  0.6509  0.0533
  0.4982  0.0697 
  0.3457  0.0750
  0.2118  0.0588
  0.1051  0.0240
  0.0362 -0.0086
  0.0124 -0.0217
  0.0001 -0.0250
  0.0126 -0.0216  
  0.0001 -0.0250     
 -0.0124 -0.0217
 -0.0250 -0.0001
 n-dist
  fixed normal distance
  $nr = 9;
  $nr = intmg( $nr );
  $nDist = ($nr-3)*$ds; 
  #  .075
  $nDist
 n-stretch
   1. 8.0 0.
 sharpness
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  30.
  30.
  30.
  30.
  30.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
  40.
 lines
  $arcLength=2.5; # guess at blade arcLength
  $ns = intmg( $arcLength/$ds + 1.5 );
  # 75 9
  $ns $nr 
 periodicity
    2
 boundary conditions
    -1 -1 1 0
 mappingName
  impeller-sp
exit
#
# Interpolate the smoothed polygon for the blade to a Nurbs to smooth out the parameterization
#
 nurbs (curve)
   interpolate from mapping with options
    impeller-sp
    parameterize by chord length
    done
    # scale the blade to be longer
    scale
      $xScaleBlade $yScaleBlade
 exit
#
#
#
 stretch coordinates
#   transform which mapping?
#   impeller-sp
   STRT:multigrid levels $ml
    # stretch in the tangential directions at the tips:
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 .25 10. 0. (id>=0,weight,exponent,position)
    STP:stretch r1 itanh: layer 1 .25 10. .5 (id>=0,weight,exponent,position)
#   # stretch in the normal direction
   Stretch r2:exp to linear
    $dx1 = $ds*1.1;
    $dx2 =$ds/$blf;
    STP:stretch r2 expl: min dx, max dx $dx2 $dx1
    STRT:name impeller
  exit
# rotate the 1st profile
  rotate/scale/shift  
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     mappingName
     blade_1_orig
  exit
# rotate the 2nd profile 
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
      51.4286
     0.0 0.0
     mappingName
     blade_2_orig
  exit
# rotate the 3rd profile
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
      102.8571
     0.0 0.0
     mappingName
     blade_3_orig
  exit
# rotate the 4th profile
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
      154.2857
     0.0 0.0
     mappingName
     blade_4_orig
  exit
# rotate the 5th profile
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
       205.7143 
     0.0 0.0
     mappingName
     blade_5_orig
  exit
# rotate the 6th profile
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
      257.1429 
     0.0 0.0
     mappingName
     blade_6_orig
  exit
# rotate the 7th profile
  rotate/scale/shift  
     transform which mapping?
      impeller
     rotate
      -60.
      0.0 0.0
     shift
      0.3 0.0
     rotate
      308.5714 
     0.0 0.0
     mappingName
     blade_7_orig
  exit
# Here is the outer boundary: volute (a logarithm spiral closed by an ellipse)
  spline
    enter spline points
    48
  -0.2500   1.4200
   0.0000   1.4200
   0.0294   1.4105
   0.0476   1.3855
   0.0476   1.3545 
   0.0294   1.3295
   0.0000   1.3200
  -0.2081   1.3139  
  -0.4143   1.2750
  -0.6134   1.2038
  -0.8003   1.1015
  -0.9703   0.9703
  -1.1188   0.8128
  -1.2417   0.6327
  -1.3357   0.4340
  -1.3980   0.2214 
  -1.4264   0.0000 
  -1.4198  -0.2249
  -1.3778  -0.4477   
  -1.3009  -0.6628 
  -1.1904  -0.8648 
  -1.0485  -1.0485 
  -0.8784  -1.2090
  -0.6837  -1.3418
  -0.4690  -1.4434
  -0.2393  -1.5107
  -0.0000  -1.5414 
   0.2430  -1.5343 
   0.4838  -1.4889
   0.7163  -1.4057
   0.9346  -1.2863
   1.1330  -1.1330
   1.3064  -0.9492 
   1.4500  -0.7388 
   1.5598  -0.5068
   1.6325  -0.2586 
   1.6657  -0.0000    
   1.6580   0.2626 
   1.6089   0.5228
   1.5191   0.7740  
   1.3900   1.0099
   1.2244   1.2244 
   1.0257   1.4118    
   0.7984   1.5669
   0.5477   1.6856
   0.2794   1.7641 
   0.0000   1.8000
  -0.2500   1.8000
    shape preserving (toggle)
  lines
    $boundaryLength = 8.; # approx length of the volute boundary
    $ns = intmg( $boundaryLength/$ds + 1.5 );
    $ns 
    #  121
 # pause
    exit
#
  reparameterize
    equidistribution
    arclength weight
      1.
    curvature weight
      10.
    re-evaluate equidistribution
#    re-evaluate equidistribution
#    re-evaluate equidistribution
    mappingName
      volute_1 
 # pause
    exit
   hyperbolic
    backward
    spacing: geometric
    geometric stretch factor 1.1
    $nrv = intmg( $nr + 2*($order-2) ); # add more lines for higher order so we have more overlap
    $nDist = ($nrv-4)*$ds;
    distance to march $nDist 
    $nLines = $nrv-1 + 8*($order-2); 
    lines to march $nLines
    BC: left match to a mapping
      outlet_line
    BC: right match to a mapping
      outlet_line
    mappingName
      volute_ext_orig
    boundary conditions
      2 2 1 0
    share
      2 2 0 0
    generate
  exit
#
#  create internal annulus
# 
   Annulus
   $nra=intmg( 6 );
   $deltaRad=($nra-1)*$ds; 
   $innerRad=.1; 
   $outerRad = $innerRad+$deltaRad;
   outer radius
     $outerRad
   inner radius
     $innerRad
   lines
    $nTheta = intmg( 2.*$pi*( $innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nra
   boundary conditions
     -1 -1 3 0
   mappingName
   inlet_circle_unstretched
   exit
#
#
  stretch coordinates
   Stretch r2:exp to linear
    STRT:multigrid levels $ml
    $dx1 = $ds*1.5;
    $dx2 = $ds/4.;
    STP:stretch r2 expl: min dx, max dx $dx2 $dx1
    close r2 stretching parameters
    STRT:name inlet_circle_orig
  exit
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands .= "\n nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit"; \
}
#
# Convert most mappings to Nurbs for faster evaluation (I hope) and use in parallel
#
$commands="#"; 
convertToNurbs("volute_ext_orig","volute_ext",0.);
convertToNurbs("blade_1_orig","blade_1",0.);
convertToNurbs("blade_2_orig","blade_2",0.);
convertToNurbs("blade_3_orig","blade_3",0.);
convertToNurbs("blade_4_orig","blade_4",0.);
convertToNurbs("blade_5_orig","blade_5",0.);
convertToNurbs("blade_6_orig","blade_6",0.);
convertToNurbs("blade_7_orig","blade_7",0.);
convertToNurbs("inlet_circle_orig","inlet_circle",0.);
$commands
exit 
#
# now make an overlapping grid
#
 generate an overlapping grid
   background
   outlet_rec
   volute_ext
   blade_1
   blade_2
   blade_3
   blade_4
   blade_5
   blade_6
   blade_7
   inlet_circle
   done
  change parameters
   order of accuracy
     $orderOfAccuracy
   interpolation type
     $interpType
   ghost points
     all
     $ng $ng $ng $ng $ng $ng
  exit
  #  display intermediate
  # open graphics
  compute overlap
 #  continue
 #   pause
  exit
save an overlapping grid
$name
pump2d
exit

