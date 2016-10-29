*
# g=godunov: 
*  plotStuff plotShockMultiDisk.cmd -show=shockMultiDisk2g.show -wireFrame=1
*  plotStuff plotShockMultiDisk.cmd -show=shockMultiDisk4g.show
*  plotStuff plotShockMultiDisk.cmd -show=shockMultiDisk8g.show -sMin=0. -sMax=7. 
*  plotStuff plotShockMultiDisk.cmd -show=shockMultiDisk16g.show -sMin=0. -sMax=7. -rMin=1. -rMax=5.
* -movie: 
*  plotStuff plotShockMultiDisk.cmd -show=shockMultiDisk16m.show -sMin=0. -sMax=7. -rMin=1. -rMax=5.
*
$wireFrame=0; $show="shockMultiDisk4g.show";
$sMin=0.; $sMax=-1.; # max/min for speed in solids
$rMin=0.; $rMax=-1.; # max/min for rho in fluid
*
GetOptions( "show=s"=>\$show, "wireFrame=s"=>\$wireFrame,"sMin=f"=>\$sMin,"sMax=f"=>\$sMax,"rMin=f"=>\$rMin,"rMax=f"=>\$rMax );
if( $wireFrame eq 1 ){ $wireFrame = "wire frame (toggle)"; }else{ $wireFrame="#"; }
if( $sMax > $sMin ){ $solidScale = "min max $sMin $sMax"; }else{ $solidScale ="#"; }
if( $rMax > $rMin ){ $rhoScale = "min max $rMin $rMax"; }else{ $solidScale ="#"; }
#
$show
*
frame series:outerDomain
* 
contour
  plot:rho
  vertical scale factor 0.
  plot contour lines (toggle)
  $wireFrame
  $rhoScale
  coarsening factor 1 (<0 : adaptive)
  compute coarsening factor 0
  exit
frame series:innerDomain1
* 
 derived types
  speed
 specify velocity components
  4 5 6
 specify displacement components
   6 7 8
exit
contour
  adjust grid for displacement 1
  * plot:vorz
  plot:speed
  vertical scale factor 0.
  plot contour lines (toggle)
  $wireFrame
  $solidScale
  coarsening factor 1 (<0 : adaptive)
  compute coarsening factor 0
exit
frame series:innerDomain2
* 
 derived types
  speed
 specify velocity components
  4 5 6
 specify displacement components
   6 7 8
exit
contour
  adjust grid for displacement 1
  * plot:vorz
  plot:speed
  vertical scale factor 0.
  plot contour lines (toggle)
  $wireFrame
  $solidScale
  coarsening factor 1 (<0 : adaptive)
  compute coarsening factor 0
exit
#
# -- commands for the movie: 
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
movie file name: shockMultiDisk
save movie files 1

show movie



# -- figures for poster:
line width scale factor:0 2
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
$cmd="#";
for( $i=1; $i<22; $i=$i+2 ){ $cmd .= "\n solution: $i\n hardcopy file name:0 shockMultiCylDensitySpeed$i.ps\n hardcopy save:0"; }
$cmd



solution: 11
hardcopy file name:0 shockMultiCylDensitySpeedt1p0.ps
hardcopy save:0


#  -- coarse grid wire frame plot solution: 10
frame series:outerDomain
contour
  min max 1.8 4.5
  exit
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
line width scale factor:0 4
bigger:0
solution: 10
hardcopy file name:0 shockMultiCyl2WireFrameDensitySpeedt0p9.ps
hardcopy save:0
