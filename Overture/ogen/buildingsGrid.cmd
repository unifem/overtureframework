#
# Generate a grid for city building -- Ogen command file.
#
# usage: ogen [-noplot] buildingsGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
#  ogen -noplot buildingsGrid -factor=2
#  ogen -noplot buildingsGrid -interp=e -factor=2
# 
# -- multigrid:
#  ogen -noplot buildingsGrid -interp=e -ml=2 -factor=2  [ OK 
#  ogen -noplot buildingsGrid -interp=e -ml=2 -factor=4  [ OK 
#  ogen -noplot buildingsGrid -interp=e -ml=3 -factor=4  [ OK 
#
# -- order 4 and MG
#  ogen -noplot buildingsGrid -interp=i -order=4 -ml=2 -factor=2 [ OK
#  ogen -noplot buildingsGrid -interp=e -order=4 -ml=2 -factor=4 [ 
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
$xar=-1.5; $xbr=1.; $yar=0.; $ybr=2.0; $zar=-1.0; $zbr=1.0;   # refinement box near buildings 
$xa=-2.5; $xb=3.; $ya=0.; $yb=2.5; $za=-2.0; $zb=2.0;   # back ground grid
$ml=0; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "buildingsGrid" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.05/$factor; # target grid spacing
$dsBL=$ds*.2;   # boundary layer stretching
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
# ------------------------------------------------------------------------------
#* Boundary conditions:
#   1= noSlipWall
#   2= slipWall
#   3=inflow
#   4=outflow
#------------------------------------------------------------------------------
# scale number of grid points in each direction by the following factor
$factor=1;
# Here we get twice as many points:
# $factor=2.**(1./3.); printf(" factor=$factor\n");
#
# Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
#
$pi = 4.*atan2(1.,1.);
$nrExtra=0; 
$dsExtra=$ds;  # extra overlap for some grids 
if( $interp eq "e" ){ $nrExtra=($order-2); $dsExtra=$nrExtra*$ds; } # add extra width for explicit and higher order
$nr = intmg( 9 + $nrExtra ); # number of radial lines -- fix me 
#
#*************************************************************************
#
create mappings 
# 
#
#************************************************************************
#   Make the roundedCylinder buildings
#************************************************************************
#
include buildRoundedCylinder.h
#
#************************************************************************
#   Make the poly-building - using a smoothedPolygon as the cross-section
#************************************************************************
#
include buildPolyBuilding.cmd
#
#**************************************************************************
#   Now take the basic building and scale/shift it to create new buildings
#**************************************************************************
#
# ============== rounded building 1 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .25 0 0.
    scale
     .5 1. 1.
   mappingName
    roundedCylinderGrid1
  exit
#
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .25 0 0.
    scale
     .5 1. 1.
   mappingName
    roundedCylinderTop1
  exit
#
# ============== rounded building 2 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .85 0. .6 
    scale
     1. 1.5 .75 
   mappingName
    roundedCylinderGrid2
  exit
#
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .85 0. .6 
    scale
     1. 1.5 .75 
   mappingName
    roundedCylinderTop2
  exit
# ============== rounded building 3 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .85 0. -.6 
    scale
     1. 1.25 1.
   mappingName
    roundedCylinderGrid3
  exit
#
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .85 0. -.6 
    scale
     1. 1.25 1.
   mappingName
    roundedCylinderTop3
  exit
# ===================================================================
# ============== poly building 1 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      -.45 0 .5 
    scale
     1. .75 .75 
   mappingName
    polyBuilding1
  exit
#
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      -.45 0 .5 
    scale
     1. .75 .75 
   mappingName
    polyTopBox1
  exit
# ============== poly building 2 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      -.75 0 -1.0 
    scale
     .55 1.25 .55  
   mappingName
    polyBuilding2
  exit
#
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      -.75 0 -1.0 
    scale
     .55 1.25 .55  
   mappingName
    polyTopBox2
  exit
# ============== poly building 3 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      .25 0 1.25
    scale
     .75 1. .75    
   mappingName
    polyBuilding3
  exit
#
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      .25 0 1.25  
    scale
     .75 1. .75 
   mappingName
    polyTopBox3
  exit
# ============== poly building 4 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      3.00 0 -.5 
    scale
     .55 1.25 1. 
   mappingName
    polyBuilding4
  exit
#
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      3.00 0 -.5 
    scale
     .55 1.25 1. 
   mappingName
    polyTopBox4
  exit
#
#
# ==================================================================
#   ** build the tower **
include buildTower.h
#
#
# Now shift and scale the tower 
#
  rotate/scale/shift
    transform which mapping?
    towerPod
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerPod1
  exit
  rotate/scale/shift
    transform which mapping?
    towerPodTop
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerPodTop1
  exit
  rotate/scale/shift
    transform which mapping?
    tower
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    tower1
  exit
  rotate/scale/shift
    transform which mapping?
      towerSpike
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerSpike1
  exit
  rotate/scale/shift
    transform which mapping?
      towerSpikeCap
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerSpikeCap1
  exit
#
# Define an explicit hole cutter for the tower base -- this is
# needed since we have a background grid and a refinement patch
#
  cylinder
    lines
     $zaCyl=-.1; $zbCyl=.1; 
     $outerRad=$baseWidth*.75-$ds; # outer radius of tower base 
     $innerRad=0.;
     $nTheta=int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5);
     $nr = int( ($outerRad-$innerRad)/$ds + 1.5 );
     $nz = int(($zbCyl-$zaCyl)/$ds + 1.5);
     $nTheta $nz $nr 
    centre for cylinder
      -.95 0 -.125   
    orientation
      2 0 1
    bounds on the radial variable
     $innerRad $outerRad
    bounds on the axial variable
      $zaCyl $zbCyl
    boundary conditions
      -1 -1 1 1 1 1
    mappingName
     towerBaseHoleCutter 
    exit
## ===================================================================
#
#
# Here is the refinement box around the buildings
#
Box
  set corners
   # -2. 2. 0. 2.5 -1.5 1.5 
   $xar $xbr $yar $ybr $zar $zbr
  lines
    ## getGridPoints(81,65,65);
    $nx = intmg( ($xbr-$xar)/$ds +1.5 ); 
    $ny = intmg( ($ybr-$yar)/$ds +1.5 );
    $nz = intmg( ($zbr-$zar)/$ds +1.5 );
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0
  share
    0 0 2 0 0 0
  mappingName
    refinementPatch
  exit
#
# Here are coarser backgrdound grid
#
Box
  set corners
   # $xac = $xb; $xbc=5.; $yac=$ya; $ybc=$yb; $zac=$za; $zbc=$zb; 
   # 2.00  5.00   0. 2.5  -1.5 1.5 
   $xa $xb $ya $yb $za $zb
  lines
    # 33 33 33
    # getGridPoints(33,33,33);
    $dsc=$ds*2.;  # coarser by this factor 
    $nx = intmg( ($xb-$xa)/$dsc +1.5 ); 
    $ny = intmg( ($yb-$ya)/$dsc +1.5 );
    $nz = intmg( ($zb-$za)/$dsc +1.5 );
    $nx $ny $nz
  boundary conditions
    3 4 1 2 2 2
  share
    0 0 2 0 0 0
  mappingName
    backGround
  exit
#
# Define a subroutine to convert a Mapping to a Nurbs Mapping
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands .= "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
# -- it is usually faster to evaluate the Nurbs  --
#  NOTE: we could scale and rotate here!
$commands="";
convertToNurbs("roundedCylinderTop1","roundedCylinderTop1Nurbs",0.);
convertToNurbs("roundedCylinderGrid1","roundedCylinderGrid1Nurbs",0.);
#
convertToNurbs("tower1","tower1Nurbs",0.);
convertToNurbs("towerPod1","towerPod1Nurbs",0.);
convertToNurbs("towerSpikeCap1","towerSpikeCap1Nurbs",0.);
convertToNurbs("towerSpike1","towerSpike1Nurbs",0.);
$commands
#*
exit
generate an overlapping grid
  backGround
  refinementPatch
  roundedCylinderTop1Nurbs
  roundedCylinderGrid1Nurbs
#  roundedCylinderTop2
#  roundedCylinderGrid2
#  roundedCylinderTop3
#  roundedCylinderGrid3
#
  polyBuilding1
  polyTopBox1
#-  polyBuilding2
#-  polyTopBox2
#-  polyBuilding3
#-  polyTopBox3
#-  polyBuilding4
#-  polyTopBox4
#
  tower1Nurbs
  towerPod1Nurbs
  towerSpikeCap1Nurbs
  towerSpike1Nurbs
  done
#
  change the plot
    toggle grid 0 0
    toggle grid 1 0
    plot block boundaries (toggle) 1
   exit this menu
# display intermediate results
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    # Define an explicit hole cutter
    create explicit hole cutter
      Hole cutter:towerBaseHoleCutter
      name: towerBaseCutter
    exit
  exit
# pause
# open graphics 
#
  compute overlap 
# pause
  exit
save a grid (compressed)
$name
buildingsGrid
exit

