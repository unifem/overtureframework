#
# Grid for a beam behind a cylinder (for FSI beam simulations)
#
# Usage:
#         ogen [-noplot] cylBeamGrid [options]
# 
# where options are
#     -factor=<num>          : grid spacing factor
#     -interp=[e/i]          : implicit or explicit interpolation
#     -name=<string>         : over-ride the default name
#     -sharpness=<num>       : sharpness parameter
#
# Channel: [xa,xb] X [ya,yb]
# Beam : beamLength, beamThickness
# Cylinder: centre (xCyl,yCyl), radius rCyl
#
#  yb  +----------------------------------------------+
#      |                                              | 
#      |              beamLength                      | 
#      |          |<------------>|                    | 
#      |      +-+                                     | 
#      |     | o |===============  beamThickness      | 
#      |      +-+                                     |
#      |       |                                      | 
#      |   (xCyl,yCyl)                                | 
#      |                                              | 
#  ya  +----------------------------------------------+
#      xa                                             xb
#     
# Examples:
#
#      ogen -noplot cylBeamGrid -interp=e -factor=4
#      ogen -noplot cylBeamGrid -interp=e -factor=8 
#
#
$factor=1; $name="";  $ml=0;
$interp="i"; $interpType = "implicit for all grids";
$order=2; $orderOfAccuracy = "second order"; $ng=2;
#
# -- Turek-Hron parameters
$xa=0.; $xb=1.5; $ya=0; $yb=0.41; # domain bounds
$xCyl=0.2; $yCyl=.2; $rCyl=.05; 
$sharpness=40*4; $ds0=0.02;
$beamThickness=0.02; $beamLength=0.35;
# -- Beam closer to lower wall: 
$xa=-2.; $xb=7.; $ya=-1; $yb=2; # domain bounds
$xCyl=0.; $yCyl=0.0; $rCyl=.5; 
$sharpness=40.*2; $ds0=0.1;   
$beamThickness=0.2; $beamLength=3.;
#
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"ml=i"=>\$ml,\
           "sharpness=f"=> \$sharpness);
#
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "cylBeamGrid" . "$interp$factor" . $suffix . ".hdf";}
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
$pi = 4.*atan2(1.,1.);
# 
$ds=$ds0/$factor;
$nl=int((($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
$L=2.5;
$H=0.41;
$blf=2.0;
$nl=int((($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
#
create mappings 
#
#  Fine background grid near cylinder and beam
# 
  rectangle 
    $xar=$xa;  $xbr=$xb; $yar=$ya; $ybr=$yb; 
    $nx = intmg( ($xbr-$xar)/$ds + 1.5 );
    $ny = intmg( ($ybr-$yar)/$ds + 1.5 );
    set corners 
      $xar $xbr $yar $ybr 
    lines 
      $nx,$ny 
    share 
      3,0,2,5
    boundary conditions 
      # 3,0,2,2 
      1 2 3 4 
    mappingName
      backGround
    exit 
#
# Coarse background grid for wake 
#
  rectangle 
    $nx=int( $L/$ds/2.0000000/2.0+1.5 );
    $ny=int( ($H)/$ds/2.0+1.5 );
    $ya=0.0;
    $yb=$H;
    $xb=$L/1.99;
    $xa=$L/2.01;
    $xb=$L;
    set corners 
      $xa,$xb,$ya,$yb
    lines 
      $nx,$ny 
    share 
      0,4,2,5 
    boundary conditions 
      # 0,4,2,2 
      0 2 3 4 
    mappingName
      coarseBackGround
    exit 
#
#   Cylinder grid
#
  annulus
   $ro=$rCyl+$ds*7;
   $npts=intmg( 2.0*$pi*$ro/$ds+1.5 );
   center: $xCyl $yCyl
   inner radius
    $rCyl
   outer radius
    $ro
   lines
    $npts,7
   share 
      6,6,1,0
   boundary conditions 
      -1 -1 7 0 
   mappingName
      unstretched-cylinder
   exit
#
  stretch coordinates
    Stretch r2:exp to linear
    #STRT:multigrid levels $ml
    $dsMin=$ds/$blf; # normal grid spacing on the cylinder
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    STRT:name cylinder 
    stretch grid
    share 
      6,6,1,0
    boundary conditions 
      -1,-1, 7, 0 
  exit
#
#  Beam curve
#
#  The beam starts on the cylinder
#
#               (xb0,yb0)    : xb0^2 + yb0^2 = rCyl^2                         (xbl,yb0)
#               + --------------------------------------------------------------
#           rCyl \
#        +--------+ 
#    (xCyl,yCyl)  /
#                 
#               + --------------------------------------------------------------
#              (xb0,yb1) 
#
  $hbt = $beamThickness*.5; # half beam thickness
  smoothedPolygon
    $xb0 =sqrt($rCyl*$rCyl - $hbt*$hbt) + $xCyl;
    $xb0a=$xb0 + $rCyl;
    $yb0=$hbt + $yCyl;
    $xbl=$xb0 + $beamLength;
    $yb1=-$hbt + $yCyl;
    $nx=$nl;
    # wdh:
    $ny=intmg(9);
    $ndist=($ny-3)*$ds;
    vertices
    6
    $xb0,  $yb1
    $xb0a, $yb1
    $xbl,  $yb1
    $xbl,  $yb0
    $xb0a, $yb0
    $xb0,  $yb0
    curve or area (toggle)
    lines 
    $nl
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    t-stretch
      0.1,30
      0.1,10
      0.1,10
      0.1,10
      0.1,10
      0.1,30
*    n-stretch
*      1 5 0
    boundary conditions
      7 7 8 0 
    share 
      1,1, 100,0 
    exit 
#
#  Beam grid
#
  hyperbolic
    $stretchFactor=1.25;
    forward
    distance to march $ndist
    lines to march $ny
    points on initial curve $nx
    # wdh: 
    geometric stretch factor 1.0
    BC: left match to a mapping
      cylinder (side=0,axis=1)
    BC: right match to a mapping
      cylinder (side=0,axis=1)
    normal blending 10,10 (lines: left, right)
    boundary conditions
      7 7 8 0 
    share 
      1,1, 100,0
    # make sure the ghost line in the marching direction
    # matches the boundary:
    boundary offset 0, 0, 0, 1 (l r b t)
    # open graphics
    generate 
    name beam
    exit
  exit this menu
# done create mappings
#
#
generate an overlapping grid
  backGround
#  coarseBackGround
  cylinder
  beam
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
  # open graphics
  compute overlap
  exit
save an overlapping grid
$name
cylBeamGrid
exit
