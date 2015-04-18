#
#  plotStuff plotFlexibleChannel -show=fc2.show -name=fc2
#  plotStuff plotFlexibleChannel -show=fc4.show -name=fc4
#  plotStuff plotFlexibleChannel -show=fc8.show -name=fc8
#  plotStuff plotFlexibleChannel -show=fc16.show -name=fc16
#
#  plotStuff plotFlexibleChannel -show=fc2a.show -name=fc2a
#
# -- Case A : results for paper:
#  plotStuff plotFlexibleChannel -show=fc2an.show -name=fc2an
#  plotStuff plotFlexibleChannel -show=fc4an.show -name=fc4an
#  plotStuff plotFlexibleChannel -show=fc8an.show -name=fc8an
#  plotStuff plotFlexibleChannel -show=fc16an.show -name=fc16an
#
# -- Case B:  results for paper:
#  plotStuff plotFlexibleChannel -show=fc2b.show -name=fc2b
#  plotStuff plotFlexibleChannel -show=fc4b.show -name=fc4b
#  plotStuff plotFlexibleChannel -show=fc8b.show -name=fc8b
#  plotStuff plotFlexibleChannel -show=fc16b.show -name=fc16b
#
# -- cleaner case: 
#  plotStuff plotFlexibleChannel -show=fc2c.show -name=fc2b
#  plotStuff plotFlexibleChannel -show=fc4c.show -name=fc4b
#  plotStuff plotFlexibleChannel -show=fc8c.show -name=fc8b
#
# -- TP scheme: 
#  plotStuff plotFlexibleChannel -show=fc2TP.show -name=fc2TP
#  plotStuff plotFlexibleChannel -show=fc4TP.show -name=fc4TP
#  plotStuff plotFlexibleChannel -show=fc8TP.show -name=fc8TP
#
#
#  Movie:
#    plotStuff plotFlexibleChannel -show=flexibleChannel4.show -name=flexibleChannel4p -component=p
#    plotStuff plotFlexibleChannel -show=flexibleChannel4.show -name=flexibleChannel4u -component=u
#    plotStuff plotFlexibleChannel -show=flexibleChannel4.show -name=flexibleChannel4v -component=v
#
$show="fc2.show"; $name="fc2"; $component="p"; 
GetOptions( "show=s"=>\$show,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,"name=s"=>\$name,"component=s"=>\$component );
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
  vertical scale factor 0.
#  wire frame (toggle)
#  plot contour lines (toggle)
#  compute coarsening factor 0
#  coarsening factor 1 (<0 : adaptive)
#  plot boundaries (toggle)
exit
#
forcing regions
  # body force grid lines 1
exit
# ---------------------
# Save line plots of the solution on the surface
# ---------------------
solution: 4
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
      $matlab = $name . "_t1p5.m"; 
      # $matlab = $name . "_t0p015.m"; 
      $matlab
      exit this menu
pause
   exit
 exit
exit


# ---------------------
# -- hard-copies: -----
# ---------------------
DISPLAY AXES:0 0
set view:0 -0.1 -0.0120846 0 1.05079 1 0 0 0 1 0 0 0 1
#
solution: 2
$time="t0p5"; hardcopy($name,$time);
$cmd
#
solution: 3
$time="t1p0"; hardcopy($name,$time);
$cmd
#
solution: 4
$time="t1p5"; hardcopy($name,$time);
$cmd



#
solution: 5
$time="t0p02"; hardcopy($name,$time);
$cmd




#
# MOVIE
#
erase
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
line width scale factor:0 2
contour
  # min max -7000 7000
   plot:$component
  xScale, yScale 1 3
  vertical scale factor 0.
exit
solution:1
movie file name: $name
save movie files 1




#  --------------------
#  -- plot the grid ---
#  --------------------
solution: 3
erase
  DISPLAY SQUARES:0 0
  DISPLAY AXES:0 0
  set view:0 0.00241692 -0.00483384 0 1.28494 1 0 0 0 1 0 0 0 1
  line width scale factor:0 3
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
# 
grid
  # plot interpolation points 1
  exit this menu
hardcopy file name:0 fc2Gridt0p010.ps
hardcopy save:0
next
hardcopy file name:0 fc2Gridt0p015.ps
hardcopy save:0
next
hardcopy file name:0 fc2Gridt0p020.ps
hardcopy save:0







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

