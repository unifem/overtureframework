#
# Create the initial grid for a flexible beam in a channel
# Use this grid with cgmp for a fluid-structure example.
#
# Usage:
#         ogen [noplot] solidBeamInAChannelGrid [options]
# where options are
#     -factor=<num>          : grid spacing factor
#     -interp=[e/i]          : implicit or explicit interpolation
#     -prefix=<string>       : over-ride the default prefix in the grid name
#     -beamThickness=<float> : beam thickness
#     -sharpness=<num>       : sharpness parameter
#     -xa, xb, ya, yb        : channel dimensions
#     -beamX=<float>         : location of the beam, from the start of the channel
#
# Examples:
#
#    ogen -noplot solidBeamInAChannelGrid -interp=e -sharpness=20 -factor=8
#    ogen -noplot solidBeamInAChannelGrid -interp=e -sharpness=20 -factor=16
# 
#   Fine grid with sharper corners:
#      ogen -noplot solidBeamInAChannelGrid -interp=e -sharpness=30 -factor=8
#      ogen -noplot solidBeamInAChannelGrid -interp=e -sharpness=30 -factor=16 
#
#  Thinner beam: 
#   ogen -noplot solidBeamInAChannelGrid -interp=e -sharpness=50 -tStretch=1.5 -beamThickness=.1 -prefix=thinSolidBeamInAChannelGrid -factor=16
#
$prefix = "solidBeamInAChannelGrid";
$factor=1; $name="";
$interp="i"; $interpType = "implicit for all grids";
$order=2; $orderOfAccuracy = "second order"; $ng=2;
$nExtra=0;
$sharpness=20;   # $sharpness=40;
$tStretch=1.5;    # 10.
$xa=-1.; $xb=1.5;
$ya=0.; $yb=1.5; 
$beamX=0.0;
$beamThickness=.2;
$beamLength=1.;
#
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,\
           "nExtra=i"=>\$nExtra,"width=f"=> \$width, "beamThickness=f"=> \$beamThickness, \
           "sharpness=f"=> \$sharpness, "xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb, \
           "tStretch=f"=> \$tStretch,"beamX"=>\$beamX,"prefix=s"=> \$prefix );
#
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }                  
#
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
#
#
$ds0=.1;
$ds=$ds0/$factor;
#
#     --- create the individual mappings -----
#
create mappings 
#
  rectangle 
    $nx=int( ($xb-$xa)/$ds+1.5 );
    $ny=int( ($yb-$ya)/$ds+1.5 );
    set corners 
      $xa $xb $ya $yb
    lines 
      $nx,$ny 
    share 
      0,0,3,0 
    boundary conditions 
      1 2 3 4 
    mappingName
      backGroundFluid
    exit 
# 
  smoothedPolygon  
    $halfT=$beamThickness*0.5;
    $beamXa=$beamX-$halfT;
    $beamXb=$beamX+$halfT;  
    # 
    $spacingFactor=1.2;  # add extra points around beam
    $nl=int($spacingFactor*(($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
    $nx=$nl;
#    $ny=int(7);
#    $ndist=($ny-1)*$ds;
    # wdh:
    $ny=int(7);
    $ndist=($ny-3)*$ds;
    vertices 
      4 
      $beamXa,0 
      $beamXa,$beamLength
      $beamXb,$beamLength 
      $beamXb,0 
    curve or area (toggle)
    lines 
    $nx
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    t-stretch
      0.0, 10.
      0.1, $tStretch
      0.1, $tStretch
      0.0, 10.
*    n-stretch
*      1 5 0
    exit 
#
#  -- fluid beam grid ----
# 
  hyperbolic
    backward
    distance to march $ndist
    lines to march $ny
    points on initial curve $nx
    # wdh: 
    # geometric stretch factor 1.1
    volume smooths 50
    # This next line is important to keep the ghost lines
    # near the corner of good quality:
    apply boundary conditions to start curve 1
    generate 
    boundary conditions
      3 3 100 0 
    share
      3 3 100 0 
    boundary condition options...
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    normal blending 5 5 (lines: left, right)
    #
    generate 
    name beamInterfaceFluid
    exit
#
#
# -------------------- SOLID GRIDS ---------------------
  # Make solid grids a bit finer when grid is coarse
  $solidFactor = $spacingFactor*(1. + 2./$factor);
# 
  hyperbolic
    $nx=$nl;
    $nr0 = int(6); 
    $nr=int($nr0*$solidFactor);
    # wdh $ndist=$ds*$ny*0.1;
    # $ndist=$ds*$ny*0.175;
    $ndist=min( $beamThickness*.15, ($nr0-3)*$ds);
    forward
    distance to march $ndist
    lines to march $nr
    points on initial curve $nx
    volume smooths 200
    # This next line is important to keep the ghost lines
    # near the corner of good quality:
    apply boundary conditions to start curve 1
    boundary conditions
      3 3 100 0 
    share 
      3 3 100 0 
    boundary condition options...
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    normal blending 5 5 (lines: left, right)
    #
    generate 
    name beamInterfaceSolid
    exit
# 
  rectangle     
    $nx=int( $solidFactor*$beamThickness/$ds+1.5 );
    $ny=int( $solidFactor*$beamLength/$ds+1.5 );
    $recxa=$beamXa+$ds*0.25;
    $recxb=$beamXb-$ds*0.25;
    $recy=$beamLength;
    set corners 
    $recxa, $recxb, $ya, $recy
    lines 
    $nx,$ny
    share 
      0,0,3,0 
    boundary conditions 
      0,0,3,0
    mappingName
      backGroundSolid
    exit 
  exit this menu 
# 
generate an overlapping grid
  backGroundFluid
  beamInterfaceFluid
  backGroundSolid
  beamInterfaceSolid
  done
  #
  change parameters
    specify a domain
      fluidDomain
      backGroundFluid
      beamInterfaceFluid
      done
    specify a domain
      solidDomain
      backGroundSolid
      beamInterfaceSolid
      done
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all 
      $ng $ng $ng $ng $ng $ng
  exit
  compute overlap
  exit
save an overlapping grid
$name
solidBeamInAChannel
exit
