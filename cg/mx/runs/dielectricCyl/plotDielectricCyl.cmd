#
#  plotStuff plotDielectricCyl.cmd -show=dc4.show -name=dieCyl4Sosup
# 
#
$show="dc4.show"; $name="dc4";
GetOptions( "show=s"=>\$show,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,"name=s"=>\$name );
# 
# -- perl subroutine for hardcopies ---
sub hardcopy{ local($var,$name,$t)=@_; \
    $varName=$var; $varName =~ s/ //g; \
    $cmd .="plot:$var \n hardcopy file name:0 $name\_$t\_$varName.ps\n hardcopy save:0\n";\
    }
#
$show
#
previous
DISPLAY AXES:0 0
x-:0
# 
contour
  $time="1p0";
  $cmd=""; 
  hardcopy(Ex,$name,$time); 
  hardcopy(Ey,$name,$time); 
  hardcopy(Hz,$name,$time); 
  hardcopy(Ext,$name,$time); 
  hardcopy(Eyt,$name,$time); 
  hardcopy(Hzt,$name,$time); 
  hardcopy("Ex error",$name,$time); 
  hardcopy("Ey error",$name,$time); 
  hardcopy("Hz error",$name,$time); 
  hardcopy("Ext error",$name,$time); 
  hardcopy("Eyt error",$name,$time); 
  hardcopy("Hzt error",$name,$time); 
  $cmd







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


