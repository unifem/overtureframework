#
#   Plot results from flow past a quarter box 
#
#   plotStuff plotQuarterBox.cmd -show=quarterBox8.show -vorMax=20.
#   plotStuff plotQuarterBox.cmd -show=quarterBox16.show -vorMax=60.
#   mpirun -np 4 $plotStuffx plotQuarterBox.cmd -show=quarterBox16a.show -vorMax=30.
#   mpirun -np 4 $plotStuffx plotQuarterBox.cmd -show=quarterBox16b.show -vorMax=30.
#
#   mpirun -np 4 $plotStuffx plotQuarterBox.cmd -show=qBox16.show -vorMax=30.
#
#    -- plot u,v,w,p,vor
#   mpirun -np 4 $plotStuffx plotQuarterBox.cmd -show=qBox16.show -vorMax=30. -uMin=-.6 -uMax=1.1 -vMin=-.7 -vMax=.4 -wMin=-.5 -wMax=.5  -pMin=-.2 -pMax=.15
#
#
$show="quarterBox8.show";  $cf=2; $dt=1.; $numTimes=5; $plotTerrain=1; $terrainOffset=0; 
$uMin=0.; $uMax=-1.; $vMin=0.; $vMax=-1.; $wMin=0.; $wMax=-1.; $pMin=0.; $pMax=-1.; $vorMin=0.; $vorMax=-1.;
$name = "site300O4G4"; $offset=0; $numMovieFrames=-1; 
$variable="enstrophy"; 
GetOptions( "show=s"=>\$show, "cf=i"=>\$cf, "name=s"=>\$name,"offset=i"=>\$offset,\
            "uMin=f"=>\$uMin,"uMax=f"=>\$uMax,\
            "vMin=f"=>\$vMin,"vMax=f"=>\$vMax,\
            "wMin=f"=>\$wMin,"wMax=f"=>\$wMax,\
            "pMin=f"=>\$pMin,"pMax=f"=>\$pMax,\
            "vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "dt=f"=>\$dt,"numTimes=i"=>\$numTimes, "plotTerrain=i"=>\$plotTerrain, \
            "numMovieFrames=i"=>\$numMovieFrames,"terrainOffset=i"=>\$terrainOffset,"variable=s"=>\$variable );
#
$show
#
previous
# 
derived types
  enstrophy
  speed
  # divergence
exit
DISPLAY COLOUR BAR:0 0
DISPLAY SQUARES:0 0
DISPLAY AXES:0 0
#
grid
  plot block boundaries 0
  coarsening factor 1
  toggle grid 0 0
  plot grid lines 0
  toggle boundary 0 0 1 0
  toggle boundary 0 0 2 0
  toggle boundary 0 0 3 0
  grid colour 0 BRASS
  grid colour 1 BRASS
  grid colour 2 BRASS
  grid colour 3 BRASS
  grid colour 4 BRASS
  close colour choices
exit this menu
contour
  # Set min max for different variables if they have been provided
  $cmd="#"; 
  if( $uMax > $uMin ){ $cmd = $cmd . "\n plot:u\n min max $uMin $uMax"; }
  if( $vMax > $vMin ){ $cmd = $cmd . "\n plot:v\n min max $vMin $vMax"; }
  if( $wMax > $wMin ){ $cmd = $cmd . "\n plot:w\n min max $wMin $wMax"; }
  if( $pMax > $pMin ){ $cmd = $cmd . "\n plot:p\n min max $pMin $pMax"; }
  if( $vorMax > $vorMin ){ $cmd = $cmd . "\n plot:enstrophy\n min max $vorMin $vorMax"; }
  $cmd
#  plot:enstrophy
#  min max $vorMin $vorMax
#
 # plot:divergence
  contour lines 0
  delete contour plane 2
  delete contour plane 1
  delete contour plane 0
  # Optionally turn off plotting the contours on the backGround grid so
  # that we can see more contour planes.
  toggle grids on and off
   # 0 : backGround is (on)
  exit
  #  [normal] [ point] 
  add contour plane  0.00000e+00  0.00000e+00  1.00000e+00  0 0  0.
  add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  0 .2  0.
exit
# angle from down stream
set view:0 0.484141 0.102719 0 2.34175 0.939693 -0.116978 0.321394 0 0.939693 0.34202 -0.34202 -0.321394 0.883022
# angle from up stream:
# set view:0 0.276699 0.251753 0 1.65526 0.766044 0.321394 -0.55667 0 0.866025 0.5 0.642788 -0.383022 0.663414
#
set view:0 0.15452 0.0854513 0 1.46954 0.939693 0.17101 -0.296198 0 0.866025 0.5 0.34202 -0.469846 0.813798

# --- HARD-COPIES ----
hardcopy file name:0 quarterBox16vorT10.ps
hardcopy save:0
plot:p
hardcopy file name:0 quarterBox16pT10.ps
hardcopy save:0
plot:u
hardcopy file name:0 quarterBox16uT10.ps
hardcopy save:0
plot:v
hardcopy file name:0 quarterBox16vT10.ps
hardcopy save:0
plot:w
hardcopy file name:0 quarterBox16wT10.ps
hardcopy save:0
hardcopy save:0
plot:speed
hardcopy file name:0 quarterBox16speedT10.ps
hardcopy save:0



