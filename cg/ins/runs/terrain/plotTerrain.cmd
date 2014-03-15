# Usage:
#   plotStuff plotSite300.cmd -show=<> -name=<> -cf=[1|2|3..] -vorMax=<> -name=<> -plotTerrain=[0|1|2]
# Options:
#   -plotTerrain : 0=no terrain, 1=plot terrain, 2=only plot terrain
#
#
# plotStuff plotSite300.cmd -show=site3003d2e2.show
# plotStuff plotSite300.cmd -show=site300g4o2.show
# 
# Movie: 
#  NOTE: first made a movie with a "brass" terrain -name=site300O4G4
#        second movie with "green" terrain : -name=site300O4G4g
#  1. plot terrain only for 25 frames:
#   srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4.show -cf=2 -vorMax=1. -name=site300O4G4g -plotTerrain=2 -numMovieFrames=25 -numTimes=0
#  2. plot terrain and vorticity
# srun -N2 -n16 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4.show -cf=2 -vorMax=1. -name=site300O4G4g -terrainOffset=25 -numTimes=3 [136 frames, last frame 160=136+25-1
#
# srun -N2 -n16 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4a.show -cf=2 -vorMax=1. -name=site300O4G4g -terrainOffset=25 -offset=135 -numTimes=4 [166 frames - last frame 325
#
# srun -N2 -n16 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4b.show -cf=2 -vorMax=1. -name=site300O4G4g -terrainOffset=25 -offset=300 -numTimes=5 [ last frame 526 300 ... 500, 202 frames  .. TIME LIMIT last frame 480
# 
# srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4a.show -cf=2 -vorMax=2. 
# srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4b.show -cf=2 -vorMax=1. 
# -- add buffer zones:
# srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4d.show -cf=2 -vorMax=1. 
# srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4e.show -cf=2 -vorMax=1. 
# srun -N1 -n8 -ppdebug $plotStuffp plotSite300.cmd -show=site300g4o4f.show -cf=2 -vorMax=1. 
#
# FINER: 
#   srun -N2 -n16 -ppdebug $plotStuffp plotSite300.cmd -show=site300g8o4.show -cf=4 -vorMax=2. 
#  1. plot terrain only for 25 frames:
#   srun -N2 -n16 -ppdebug $plotStuffp plotSite300.cmd -show=site300g8o4.show -cf=4 -vorMax=2. -name=site300O8G4g -plotTerrain=2 -numMovieFrames=25 -numTimes=0
#  2. plot terrain and vorticity
# srun -N4 -n32 -ppdebug $plotStuffp plotSite300.cmd -show=site300g8o4.show -cf=4 -vorMax=2. -name=site300O8G4g -terrainOffset=25 -numTimes=1 [ 32 frames, last frame 56=32+25-1
# srun -N4 -n32 -ppdebug $plotStuffp plotSite300.cmd -show=site300g8o4a.show -cf=4 -vorMax=2. -name=site300O8G4g -terrainOffset=25 -offset=31 -numTimes=1 [ 34 frames last frame 089
# srun -N4 -n32 -ppdebug $plotStuffp plotSite300.cmd -show=site300g8o4b.show -cf=4 -vorMax=2. -name=site300O8G4g -terrainOffset=25 -offset=64 -numTimes=1 [ 
#
# NOTE: offset: first solution is $offset*$dt 
#
$show="site3003d2e2.show"; $vorMax=2.; $cf=2; $dt=1.; $numTimes=5; $plotTerrain=1; $terrainOffset=0; 
$name = "site300O4G4"; $offset=0; $numMovieFrames=-1; 
GetOptions( "show=s"=>\$show, "cf=i"=>\$cf, "vorMax=f"=>\$vorMax,"name=s"=>\$name,"offset=i"=>\$offset,\
            "dt=f"=>\$dt,"numTimes=i"=>\$numTimes, "plotTerrain=i"=>\$plotTerrain, \
            "numMovieFrames=i"=>\$numMovieFrames,"terrainOffset=i"=>\$terrainOffset );
#
$show
#
 x-r 90
set home
previous
# 
# set view:0 0.037014 0.0208637 0 1.17167 0.497521 0.308639 -0.810688 -0.866928 0.144424 -0.477051 -0.0301537 0.940151 0.339422
# set view:0 0.0472508 -0.00241692 0 1.27701 0.5 0.296198 -0.813798 -0.866025 0.17101 -0.469846 5.30288e-17 0.939693 0.34202
set view:0 0.00193353 0.0300665 0 1.60617 0.34202 0.321394 -0.883022 -0.939693 0.116978 -0.321394 5.75396e-17 0.939693 0.34202
# view from south south west up 70 degrees approx.
##set view:0 0.0378006 0.128 0 1.60617 0.984808 0.143961 -0.0971028 -0.173648 0.816443 -0.550698 1.06329e-17 0.559193 0.829038
grid
  plot block boundaries 0
  coarsening factor $cf 
  toggle grid 0 0
  toggle grid 1 0
   #              s a g 
  toggle boundary 0 0 2 0
  toggle boundary 0 1 2 0
  toggle boundary 1 1 2 0
  toggle boundary 1 0 2 0
  toggle grid lines on boundary 0 0 2 0
  toggle grid lines on boundary 0 1 2 0
#
#  plot grid lines 0
#  grid colour 2 BRASS
#  grid colour 2 GOLD
#
# pause
exit this menu
# 
derived types
  enstrophy
  speed
exit
# plot:enstrophy
# 
  contour
    plot:enstrophy
    min max 0 $vorMax 
    contour lines 0
    #
    # Optionally turn off plotting the contours on the backGround grid so
    # that we can see more contour planes.
    toggle grids on and off
     0 : backGround is (on)
    exit
    #
    delete contour plane 2
    delete contour plane 1
    delete contour plane 0
    # y-planes (y goes from about 0 to 1400 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02  100.  4.67857e+02 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02  400.  4.67857e+02 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02  700.  4.67857e+02 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02 1000.  4.67857e+02 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02 1300.  4.67857e+02 
  exit
# 
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
DISPLAY SQUARES:0 0
# 
#
pause
#
# ---------------------------
# ----- hard copies ---------
# ---------------------------
# 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
DISPLAY SQUARES:0 0
$deltaTimes=50.;
@times=( 0.,50.,100.,150.,200.,250.,300.,350.,400.,450.,500.,550.,600.,650.,700.,750.,800.,850.,900.,950.,1000. );
$cmd="#"; 
$t0 = $offset*$dt;    # time for first solution in the show file
$i0 = int( $t0/$deltaTimes + .9999999  );  # index of first output time to look for 
#
plot:speed
# pause
for( $i=$i0; $i<$numTimes+$i0; $i++ ){ $t=$times[$i]; $j=int( ($t-$t0)/$dt + 1.5); $tn =$times[$i]; $tn =~ s/\./p/; $plotName=$name . "velT" . "$tn.ps"; $cmd .= "\n solution: $j \n hardcopy file name:0 $plotName\n hardcopy save:0"; }
$cmd
plot:enstrophy
$cmd="#"; 
for( $i=$i0; $i<$numTimes+$i0; $i++ ){ $t=$times[$i]; $j=int( ($t-$t0)/$dt + 1.5); $tn =$times[$i]; $tn =~ s/\./p/; $plotName=$name . "vorT" . "$tn.ps"; $cmd .= "\n solution: $j \n hardcopy file name:0 $plotName\n hardcopy save:0"; }
$cmd
#
# ----------- movie: ----------------------------
if( $plotTerrain eq 2 ){ $cmd="erase\n grid\n exit this menu\n"; }else{ $cmd="#"; }
$cmd
DISPLAY LABELS:0 0
# 
solution: 1
movie file name offset
$totalOffset= $terrainOffset + $offset;
$totalOffset
save movie files 1
movie file name: $name
if( $numMovieFrames > 0 ){ $cmd="movie frames: $numMovieFrames"; }else{ $cmd="#"; }
$cmd
show movie






derived types
  enstrophy
exit


    # y-plane near back:
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  5.53351e+02  1.24288e+03  4.67857e+02 
    # y-plane near middle:
    ## add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  8.01881e+02  8.34157e+02  4.34978e+02 
    # y-plane near front: 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  4.86314e+02  3.16725e+02  3.36421e+02 
