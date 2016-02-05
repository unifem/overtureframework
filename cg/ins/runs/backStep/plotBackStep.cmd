#
#  plotStuff plotBackStep.cmd -show=backStep4.show -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStep8 -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStep8a -vorMin=-5. -vorMax=5.
#
#  plotStuff plotBackStep.cmd -show=backStep16 -vorMin=-10. -vorMax=10.
#  plotStuff plotBackStep.cmd -show=backStep16a -vorMin=-10. -vorMax=10.
#
#  plotStuff plotBackStep.cmd -show=backStepRefine8  -vorMin=-20. -vorMax=20. -zoom=1 -solution=201 -plotName=backStepRefineG8Vort20
#  plotStuff plotBackStep.cmd -show=backStepRefine16 -vorMin=-20. -vorMax=20. -zoom=1 -solution=201 -plotName=backStepRefineG16Vort20
#  plotStuff plotBackStep.cmd -show=backStepRefine32 -vorMin=-20. -vorMax=20. -zoom=1 -solution=201 -plotName=backStepRefineG32Vort20
#
#  plotStuff plotBackStep.cmd -show=backStepAndBody4.show -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStepAndBody4a.show -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStepAndBody8.show -vorMin=-5. -vorMax=5.
#
# -- MOVING BODY --
#  plotStuff plotBackStep.cmd -show=backStepAndBodyMove4.show -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStepAndBodyMove4a.show -vorMin=-5. -vorMax=5.
#  plotStuff plotBackStep.cmd -show=backStepAndBodyMove8.show -vorMin=-20. -vorMax=20.
#  plotStuff plotBackStep.cmd -show=backStepAndBodyMove16.show -vorMin=-20. -vorMax=20.
#  plotStuff plotBackStep.cmd -show=backStepAndBodyMove16a.show -vorMin=-20. -vorMax=20.
#
#  plotStuff plotBackStep.cmd -show=backStepAndBodyRefineMove16.show -vorMin=-20. -vorMax=20.
#
# --- BACK-STEP IN A CHANNEL : compare to experiment ---
#  plotStuff plotBackStep.cmd -show=backStepInChannel8.show -vorMin=-10. -vorMax=10.
#  plotStuff plotBackStep.cmd -show=backStepInChannel16.show -vorMin=-10. -vorMax=10. -plotName=backStepRefineG16Vort10t50
#  plotStuff plotBackStep.cmd -show=backStepInChannel16a.show -vorMin=-10. -vorMax=10. -plotName=junk
#
#  plotStuff plotBackStep.cmd -show=backStepInChannel16Average50to300.show 
#
$show="cylbeam2.show"; $plotName=""; $solution=-1; 
$vorMin=-50; $vorMax=25.; $zoom=0; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
             "zoom=i"=>\$zoom,"plotName=s"=>\$plotName );
#
$show
# 
derived types
vorticity
exit
#
previous
contour
  plot:vorticity
  coarsening factor 1 (<0 : adaptive)
  min max $vorMin $vorMax
  vertical scale factor 0.
  plot contour lines (toggle)
exit
if( $solution ne -1 ){ $cmd="solution: $solution"; }else{ $cmd="#"; }
$cmd
#
# Zoom including step and some wake for backStepRefine 
$zoomCmd="#";
if( $zoom eq 1 ){ $zoomCmd="DISPLAY AXES:0 0\n DISPLAY COLOUR BAR:0 0\n set view:0 0.452398 0.14198 0 5.00825 1 0 0 0 1 0 0 0 1"; }
$zoomCmd
#
if( $plotName ne "" ){ $cmd="DISPLAY AXES:0 0\n DISPLAY COLOUR BAR:0 0\n bigger\n hardcopy file name:0 $plotName.ps\n hardcopy save:0"; }else{ $cmd="#"; }
$cmd



# 
# 
forcing regions
  body force grid lines 1
  line width: 2
  # forcing region colour (bf,colour): 0 RED
exit




# -- movie
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 -0.00504559 0.0647128 0 1.45693 1 0 0 0 1 0 0 0 1
movie file name: twoBeamsInAChannel
save movie files 1
show movie


# -- Hardcopy:
set view:0 0.103601 0.038383 0 1.94067 1 0 0 0 1 0 0 0 1
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
previous
  hardcopy file name:0 twoBeamsInAChannelVor_t10.ps
  hardcopy save:0