#
#  plotStuff plotPartitioned -show=fp2.show -name=fp2
#  plotStuff plotPartitioned -show=fp2_rhoshs10.show -name=fp2_rhoshs10
#  plotStuff plotPartitioned -show=fp2_rhoshs10_AMP.show -name=fp2_rhoshs10_AMP
#  plotStuff plotPartitioned -show=fp1_rhoshs1_AMP.show -name=fp1_rhoshs1_AMP
#
#  plotStuff plotPartitioned -show=sb1.show -name=sb1
#  plotStuff plotPartitioned -show=sb4.show -name=sb4
# 
# -- beam under pressure - steady quadratic solution:
#  plotStuff plotPartitioned -show=fp_BUP_AMP_RHOS1_T5.show -name=String
# 
# -- beam under pressure - steady quartic solution:
#  plotStuff plotPartitioned -show=fp_BUP_AMP_RHOS1_E0p2.show -name=Beam
#
#  plotStuff plotPartitioned -show=bp.show
#  plotStuff plotPartitioned -show=pba.show
#  plotStuff plotPartitioned -show=pbf.show
#  plotStuff plotPartitioned -show=bp2pbf.show 
# -- movie:
#   plotStuff plotPartitioned -show=beamUnderPressure_EI0p2_Rhos10p0_AMP.show 
#
$show="fp2.show"; $name="fp2";
GetOptions( "show=s"=>\$show,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,"name=s"=>\$name );
# -- perl subroutine for hardcopies ---
sub hardcopy{ local($name,$time)=@_; \
    $cmd ="plot:p \n hardcopy file name:0 $name\_p_$time.ps\n hardcopy save:0\n";\
    $cmd.="plot:u \n hardcopy file name:0 $name\_u_$time.ps\n hardcopy save:0\n";\
    $cmd.="plot:v \n hardcopy file name:0 $name\_v_$time.ps\n hardcopy save:0";\
    }
#
$show
#
previous
contour
  plot:p 
#  wire frame (toggle)
#  plot contour lines (toggle)
#  vertical scale factor 0.
#  compute coarsening factor 0
#  coarsening factor 1 (<0 : adaptive)
#  plot boundaries (toggle)
exit
#
forcing regions
  # body force grid lines 1
exit

pause
#  --------------------
#  -- plot the grid ---
#  --------------------
erase
forcing regions
  # body force grid lines 1
exit
bigger
  DISPLAY SQUARES:0 0
  DISPLAY AXES:0 0
  line width scale factor:0 3
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
# 
grid
  # plot interpolation points 1
  exit this menu
solution: 1
#
$plotName = "flexiblePartition2" . $name . "Gridt00.ps";
hardcopy file name:0 $plotName
# hardcopy file name:0 flexiblePartition2Gridt00.ps
hardcopy save:0
pause
solution: 11
$plotName = "flexiblePartition2" . $name . "Gridt10.ps";
hardcopy file name:0 $plotName
# hardcopy file name:0 flexiblePartition2Gridt10.ps
hardcopy save:0


# --- Save Beam in a matlab file ---
forcing regions
pick to edit region
edit forcing region: 0
  save points in matlab format
  $matlabFile = "beamPoints_" . $name . ".m"; 
  $matlabFile



# ---------------------
# -- hard-copies: -----
# ---------------------
DISPLAY AXES:0 0
set view:0 -0.1 -0.0120846 0 1.05079 1 0 0 0 1 0 0 0 1
#
solution: 2
$time="t0p005"; hardcopy($name,$time);
$cmd
#
solution: 3
$time="t0p01"; hardcopy($name,$time);
$cmd
#
solution: 4
$time="t0p015"; hardcopy($name,$time);
$cmd
#
solution: 5
$time="t0p02"; hardcopy($name,$time);
$cmd

# ---------------------
# Save line plots of the solution on the surface
# ---------------------
contour
  line plots
    specify a boundary
    121 1 0 1
      p
      add u
      add v
      add x0
      add x1
      save results to a matlab file
      $matlab = $name . "_t0p015.m"; 
      $matlab
      exit this menu
pause
   exit
 exit
exit




solution: 2
DISPLAY AXES:0 0
set view:0 -0.1 -0.0120846 0 1.05079 1 0 0 0 1 0 0 0 1
plot:p
hardcopy file name:0 flexibleChannel4_p_t0p05.ps
hardcopy save:0
plot:u
hardcopy file name:0 flexibleChannel4_u_t0p05.ps
hardcopy save:0
plot:v
hardcopy file name:0 flexibleChannel4_v_t0p05.ps
hardcopy save:0





# -- hardcopies ---
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
# 
$plotName = $name . "Vorticity.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
erase 
# 
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 3.000000e-02
  $plotName = $name . "StreamLines.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0


# --- movie:
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
next
movie file name: periodicChannel
save movie files 1
show movie



set view:0 -0.0838874 0.0406987 0 1.17507 1 0 0 0 1 0 0 0 1

# -- hardcopies ---
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
# 
$plotName = $name . "Vorticity.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
erase 
# 
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 3.000000e-02
  $plotName = $name . "StreamLines.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

