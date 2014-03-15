#
# Grid for a 3d room : flow and heat transfer
#
# usage: ogen [noplot] room3d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
# NOTE: watch out at inlet/outlet : the background grid may retain a short section of wall where it shouldn't
#       if there is not enough overlap
#
# examples:
#     ogen -noplot room3d -factor=4             [OK 1M pts (room only)
#     ogen -noplot room3d -interp=e -factor=4   [OK, .9M pts with desk 
#     ogen -noplot room3d -interp=e -factor=8 
#     ogen -noplot room3d -interp=e -factor=16 
#     ogen -noplot room3d -interp=e -factor=32 
#
#  - MG levels:
#     ogen -noplot room3d -interp=e -factor=4 -ml=1   [OK .98M pts (includes desk and cabinet)
#     ogen -noplot room3d -interp=e -factor=8 -ml=2   [OK 7M pts (room only) 7.5M with desk/cabinet
#     ogen -noplot room3d -interp=e -factor=16 -ml=2  [53 M (room only)
#     ogen -noplot room3d -interp=e -factor=32 -ml=3
# 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "room3d" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.1;
$ds=$ds0/$factor;
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$Pi = 4.*atan2(1.,1.);
#
create mappings
#
  # nr = number of lines in normal directions to boundaries
  # $nr = max( 5 + $ng + 2*($order-2), 2**($ml+2) );
  $nr0 = 5 + $ng + 2*($order-2);
  # On finer grids the number of points in the radial direction should be more
  ## if( $interp eq "e" && $factor > 4 ){ $nr0 =  9 + $ng + 2*($order-2); }
  $nr = intmg( $nr0 );
# Room:
$bcFloor=3; $shareFloor=3; 
$bcCeiling=4;  $shareCeiling=4; 
$bcLeg=1; 
$bcDesk=1; 
$shareDesk=10; # share value for desk 
$bcInlet=11; $shareInlet=11;
$bcOutlet=12; $shareOutlet=12;
$bcCabinet=13; $shareCabinet=13;
#
$ceilingHeight=2.5; 
$xaRoom=0.; $xbRoom=2.5; $yaRoom=0.; $ybRoom=$ceilingHeight; $zaRoom=0.; $zbRoom=2.; 
#
# -------------------------------------
# --------- Cabinet (box on floor) ----
# -------------------------------------
#
$xCabinet=1.5; $yCabinet=0.;  $zCabinet=.75;   # position of the cabinet
#
# -- cabinet cross section --
#
  $cabinetWidthX=.5; $cabinetWidthZ=.75;  # shape of cabinet on the ceiling 
  $cabinetHeight=.7;   # height of cabinet 
  $sharpnessCabinet=15.;   # corner sharpness
#
  $overlap=(1+$ng)*$ds; # $overlap is the amount the cabinet edge overlaps the top
  $overlap=max(.08,$overlap);  # Do not make the overlap too small since we don't want the top to overhang too much
#
  $xi=-$cabinetWidthX*.5; $yb=0.; $ybi=$yb+$cabinetHeight;
 smoothedPolygon
  vertices
    $x0=$xi;          $y0=$yb;
    $x1=$xi;          $y1=$ybi;
    $x2=$xi+$overlap; $y2=$ybi;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    40
    40
    40
  t-stretch
    0 40
    .15 10
    0 40
  n-stretch
    .5 4. 0
  n-dist
    $nDist = max(.1,($nr-3)*$ds); 
    $nrCabinet = intmg( $nDist/$ds + 3.5 );
    fixed normal distance
     $nDist
   lines
      $length = $cabinetHeight+$overlap;
      $nsCabinet = intmg( 1.25*$length/$ds + 1.5 );
      $nsCabinet $nrCabinet
    boundary conditions
      0 5 1 0
    share
      0 5 4 0 
    mappingName
      cabinetCrossSection
 exit
#
#
# Cabinet shape: 
#
$xao=$cabinetWidthX*.5; $yao=$cabinetWidthZ*.5; 
smoothedPolygon 
  $x0=-$xao;   $y0=0.;
  $x1=-$xao;   $y1=+$yao;
  $x2= $xao;   $y2=+$yao;
  $x3= $xao;   $y3=-$yao;
  $x4=-$xao;   $y4=-$yao;
  $x5=-$xao;   $y5=0.;
  vertices 
       6
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
     $x4 $y4
     $x5 $y5
  sharpness
    $sharpnessCabinet 
    $sharpnessCabinet
    $sharpnessCabinet
    $sharpnessCabinet
    $sharpnessCabinet
    $sharpnessCabinet
  t-stretch
    0 40
    .010 2.
    .010 2.
    .010 2.
    .010 2.
    0 40
#
  curve or area (toggle)
  make 3d (toggle)
    .0  * z-level
  mappingName
    cabinetCurveUnrotated
  exit
#
  rotate/scale/shift
    rotate
      90 0
      0. 0 .0 
    mappingName
     cabinetCurve
    exit
#
# create the 3d cabinet edge by sweeping the cross-section
#
  sweep
    choose reference mapping
      cabinetCrossSection
    choose sweep curve
      cabinetCurve
    specify center
      -$xao 0 0
    # change the orientation to turn the mapping inside-out
    orientation 
      -1  
    lines
      $length = $cabinetWidthX*2 + $cabinetWidthZ*2; 
      $nSweep = intmg( 1.2*$length/$ds + 1.5 );      
      $nsCabinet $nrCabinet $nSweep
    boundary conditions
      $bcFloor      0  $bcCabinet    0  -1 -1 
    share
      $shareFloor   0  $shareCabinet 0  0 0  
    mappingName
     cabinetEdgeMaster
  exit
# Shift cabinet to correct position
  rotate/scale/shift
    transform which mapping?
      cabinetEdgeMaster
    shift
      $xCabinet $yCabinet $zCabinet
    mappingName
     cabinetEdge
 exit
#
#  Cabinet Top
#
Box
  # NOTE: cabinet top should not overhang by too much or else it will cut holes in the background
  # $shrink=3.*$ds;
  $shrink=.05;
  $nDist = ($nr-3)*$ds; 
  $xad=$xCabinet-$cabinetWidthX*.5+$shrink; $xbd=$xCabinet+$cabinetWidthX*.5-$shrink; $yad=$yCabinet+$cabinetHeight; $ybd=$yad+$nDist;
  $zad=$zCabinet-$cabinetWidthZ*.5+$shrink; $zbd=$zCabinet+$cabinetWidthZ*.5-$shrink;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 $bcCabinet    0 0 0 
  share
    0 0 $shareCabinet 0 0 0 
  mappingName
    cabinetTop
  exit
#
# ----------------------------
# ------ Square outlet -------
# ----------------------------
#
$xOutlet=2.; $yOutlet=$ceilingHeight;  $zOutlet=1.5;   # position of the outlet
#
# -- outlet cross section --
#
  $outletWidthX=.5; $outletWidthZ=.5;  # shape of outlet on the ceiling 
  $outletDepth=.25;   # depth of outlet pipe above the ceiling 
  $overlap=(4+$ng)*$ds; # $overlap is the amount the outlet overlaps the ceiling
  $xi=-$outletWidthX*.5; $yb=0.; $ybi=$yb+$outletDepth;
 smoothedPolygon
  vertices
    $x0=$xi-$overlap; $y0=$yb;
    $x1=$xi;          $y1=$yb;
    $x2=$xi;          $y2=$ybi;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $outletDepth+$overlap;
      $nsOutlet = intmg( 1.1*$length/$ds + 1.5 );
      $nsOutlet $nr
    boundary conditions
      0 5 1 0
    share
      0 5 4 0 
    mappingName
      outletCrossSection
  # pause
 exit
#
#
# Outlet shape: 
#
$xao=$outletWidthX*.5; $yao=$outletWidthZ*.5; 
smoothedPolygon 
  $x0=-$xao;   $y0=0.;
  $x1=-$xao;   $y1=-$yao;
  $x2= $xao;   $y2=-$yao;
  $x3= $xao;   $y3= $yao;
  $x4=-$xao;   $y4= $yao;
  $x5=-$xao;   $y5=0.;
  vertices 
       6
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
     $x4 $y4
     $x5 $y5
  sharpness
    10 
    10
    10
    10
    10
    10
  t-stretch
    0 40
    .010 2.
    .010 2.
    .010 2.
    .010 2.
    0 40
#
  curve or area (toggle)
  make 3d (toggle)
    .0  * z-level
  mappingName
    outletCurveUnrotated
  # pause
  exit
#
  rotate/scale/shift
    rotate
      90 0
      0. 0 .0 
    mappingName
     outletCurve
# pause
    exit
#
# create the 3d outlet edge by sweeping the cross-section
#
  sweep
    choose reference mapping
      outletCrossSection
    choose sweep curve
      outletCurve
    specify center
      -$xao 0 0
    # change the orientation to turn the mapping inside-out
    orientation 
      -1    
    lines
      $length = $outletWidthX*2 + $outletWidthZ*2; 
      $nSweep = intmg( 1.*$length/$ds + 1.5 );      
      $nsOutlet $nr $nSweep
    boundary conditions
      0 $bcOutlet    $bcCeiling    0  -1 -1 
    share
      0 $shareOutlet $shareCeiling 0  0 0  
    mappingName
     outletEdgeMaster
  exit
# Shift outlet to correct position
  rotate/scale/shift
    transform which mapping?
      outletEdgeMaster
    shift
      $xOutlet $yOutlet $zOutlet
    mappingName
     outletEdge
 exit
#
#  Outlet Core
#
Box
  $shrink=$ds; 
  $xad=$xOutlet-$outletWidthX*.5+$shrink; $xbd=$xOutlet+$outletWidthX*.5-$shrink; $yad=$yOutlet-3.*$ds; $ybd=$yOutlet+$outletDepth;  
  $zad=$zOutlet-$outletWidthZ*.5+$shrink; $zbd=$zOutlet+$outletWidthZ*.5-$shrink;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 0 $bcOutlet 0 0 
  share
    0 0 0 $shareOutlet 0 0 
  mappingName
    outletCore
  exit
#
# ----------------------------
# -------- Round Inlet -------
# ----------------------------
#
$xInlet=.5; $yInlet=$ceilingHeight;  $zInlet=.5;  # position of the inlet
# -- inlet cross section
#
  $inletDepth=.25;  $inletRadius=.25; 
#
# IMPORTANT NOTE: The inlet must overlap the adjacent surface far enough so that it can properly cut holes
#    in the inlet core. (Since the backGround grid does NOT cut holes in the inlet core!)
#
  $overlap=(4+$ng)*$ds; # $overlap is the amount the inlet overlaps the ceiling
  $overlap=max($overlap,.125); # $overlap is the amount the inlet overlaps the ceiling
#
  $xi=-$inletRadius; $yb=0.; $ybi=$yb+$inletDepth; 
 smoothedPolygon
  vertices
    $x0=$xi-$overlap; $y0=$yb;
    $x1=$xi;          $y1=$yb;
    $x2=$xi;          $y2=$ybi;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $inletDepth+$overlap;
      $ns = intmg( 1.*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      0 5 1 0
    share
      0 5 4 0 
    mappingName
      inletCrossSection
 exit
#
  body of revolution
    revolve which mapping?
      inletCrossSection
    choose a point on the line to revolve about
      0. 0. 0.
    tangent of line to revolve about
      0 1 0
    lines
      $nTheta = intmg( 2.*$Pi*$inletRadius/$ds + 1.5 );
      $ns $nr $nTheta
    boundary conditions
      0 $bcInlet 1 0 -1 -1
    share
      0 $shareInlet $shareCeiling 0 0 0 
    mappingName
      roundInletMaster
 exit
# Shift Inlet to correct position
  rotate/scale/shift
    transform which mapping?
      roundInletMaster
    shift
      $xInlet $yInlet $zInlet
    mappingName
      roundInlet
 exit
#
#  Inlet Core
#
Box
  $dse=0.; 
  $xad=$xInlet-$inletRadius-$dse; $xbd=$xInlet+$inletRadius+$dse; $yad=$yInlet-3.*$ds; $ybd=$yInlet+$inletDepth;  
  $zad=$zInlet-$inletRadius-$dse; $zbd=$zInlet+$inletRadius+$dse;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 0 $bcInlet 0 0 
  share
    0 0 0 $shareInlet 0 0 
  mappingName
    roundInletCore
  exit
#
# -----------------------------
# -- Desk:
# -----------------------------
$xDesk=.25; $yDesk=1.; $zDesk=1.; # position of left front of desk top  ** zDesk=center ** fix xDesk 
#
#
# -- desk edge cross section:
#
#    /\    ---------
#     |   |
#   depth X                  X=(0,0)
#     |   |
#    \/    ---------
#         <- width->
#
# width = how far edge grid extends over top and bottom NOTE: do NOT make too thin since we cannot let the
#         top and bottom of the desk overhang too much (or else they cut holes in the background)
 $width=max(.08,$ds*4.);
 $deskEdgeDepth=.1;
 $deskBottom=$yDesk-$deskEdgeDepth*.5; $deskTop=$yDesk+$deskEdgeDepth*.5;
 smoothedPolygon
  vertices
    $x0=$width;   $y0=$deskEdgeDepth*.5;
    $x1=0.;       $y1=$y0;
    $x2=0.;       $y2=-$y1; 
    $x3=$width;   $y3=$y2;
    4
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
  sharpness
    20
    20
    20
    20
  t-stretch
    0 40
    .15 20
    .15 20
    0 40
  n-stretch
    .5 4. 0
 #
 # NOTE: do NOT make the normal distance too small since the desk top and bottom must
 #  not stick out too far at the corners (or else they cut holes in the background)
 $nDist = max(.1,($nr-3)*$ds); 
 $nrDesk = intmg( $nDist/$ds + 3.5 );
 #
  n-dist
   fixed normal distance
      -$nDist
   lines
      $length = $deskEdgeDepth + $width*2.; 
      $nsDECS = intmg( 1.25*$length/$ds + 1.5 );
      $nsDECS $nrDesk
    boundary conditions
      0 0 1 0
    share
      0 0 $shareDesk 0 
    mappingName
      deskEdgeCrossSection
 exit
#
# Desk shape: 
#
$deskWidth=1.5; $deskDepth=1.; $deskBack=-$deskDepth*.5; $deskFront=$deskDepth*.5;
smoothedPolygon 
  $x0= 0.;         $y0=0.;
  $x1= 0.;         $y1=$deskFront;
  $x2=$deskWidth;  $y2=$deskFront;
  $x3=$deskWidth;  $y3=$deskBack;
  $x4= 0.;         $y4=$deskBack;
  $x5= 0.;         $y5=0.;
  vertices 
       6
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
     $x4 $y4
     $x5 $y5
  sharpness
    20 
    20
    20
    20
    20
    20
  t-stretch
    0 40
    .10 5
    .10 5
    .10 5
    .10 5
    0 40
#
  curve or area (toggle)
  make 3d (toggle)
    .0  * z-level
  mappingName
    deskEdgeCurveUnrotated
  exit
#
  rotate/scale/shift
    rotate
      90 0
      0 0 .0 
    mappingName
     deskEdgeCurve
    exit
#
# create the 3d desk edge by sweeping the cross-section
#
  sweep
    choose reference mapping
      deskEdgeCrossSection
    choose sweep curve
      deskEdgeCurve
    specify center
      0 0 0
    lines
      $length = $deskWidth*2 + $deskDepth*2; 
      $nSweep = intmg( 1.2*$length/$ds + 1.5 );      
      $nsDECS $nrDesk $nSweep
    boundary conditions
      0 0 $bcDesk 0 -1 -1 
    share
      0 0 $shareDesk 0 0 0  
    mappingName
     deskEdgeMaster
  exit
# Shift desk to correct position
  rotate/scale/shift
    transform which mapping?
      deskEdgeMaster
    shift
      $xDesk $yDesk $zDesk
    mappingName
      deskEdge
 exit
#
#  Desk top 
#
Box
  # NOTE: desk top should not overhang desk by too much or else it will cut holes in the background
  # $d=$ds*($ng+1); # desk top does not need to over-hang desk
  $d=.25*$ds0*2.5; # desk top does not need to over-hang desk by too much
  $xad=$xDesk+$d; $xbd=$xDesk+$deskWidth-$d; $yad=$yDesk+$deskEdgeDepth*.5; $ybd=$yad+($nr-3)*$ds; 
  $zad=$zDesk-$deskDepth*.5+$d; $zbd=$zDesk+$deskDepth*.5-$d;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 $bcDesk 0 0 0 
  share
    0 0 $shareDesk 0 0 0 
  mappingName
    deskTop
  exit
#
#  Desk bottom
#
Box
  $xad=$xDesk+$d; $xbd=$xDesk+$deskWidth-$d; $ybd=$yDesk-$deskEdgeDepth*.5; $yad=$ybd-($nr-3)*$ds; 
  $zad=$zDesk-$deskDepth*.5+$d; $zbd=$zDesk+$deskDepth*.5-$d;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 0 $bcDesk 0 0 
  share
    0 0 0 $shareDesk 0 0 
  mappingName
    deskBottom
  exit
#
#  Desk -leg
#
  $legRadius=.05; 
  $legBot=$yaRoom; # floor level
  $legTop=$deskBottom;
  $xLeg=$deskWidth*.5+$xDesk; $zLeg=0.+$zDesk;
  cylinder
    orientation
      2 0 1
    centre for cylinder
      $xLeg 0. $zLeg 
    bounds on the axial variable
      $legBot $legTop
    bounds on the radial variable
      $outerRad = $legRadius + $nDist; 
      $legRadius $outerRad
    lines
      $nTheta = intmg( 2.*$Pi*($legRadius+$outerRad)*.5/$ds +1.5 ); 
      $ny = intmg( ($legTop-$legBot)/$ds +1.5 ); 
      $nTheta $ny $nr
    boundary conditions
      -1 -1 $bcFloor $bcDesk $bcLeg 0
    share
      0 0 $shareFloor $shareDesk $shareLeg 0
    mappingName
      deskLeg
  exit
#
#  Background
#
Box
  $xad=$xaRoom; $xbd=$xbRoom; $yad=$yaRoom; $ybd=$ybRoom; $zad=$zaRoom; $zbd=$zbRoom;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    1 2 3 4 5 6 
  mappingName
    roomBackGround
  exit
#
# Make the overlapping grid
#
exit
generate an overlapping grid
  roomBackGround
#
  deskTop
  deskBottom
  deskEdge
#
  deskLeg
#
  roundInletCore
  roundInlet
#
  outletCore
  outletEdge
#
  cabinetTop
  cabinetEdge
#
  done
  change parameters
 # prevent background from cutting holes in the ceiling inlet or outlet
   prevent hole cutting
     roomBackGround
       roundInletCore
     roomBackGround
       outletCore
   done
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
# display intermediate results
# open graphics
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
room3d
exit

