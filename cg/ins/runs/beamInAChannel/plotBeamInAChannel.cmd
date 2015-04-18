#
#  plotStuff plotBeamInAChannel.cmd -show=beam.show
#  plotStuff plotBeamInAChannel.cmd -show=beam4.show
# 
#  plotStuff plotBeamInAChannel.cmd -show=multiBeam4.show
#  plotStuff plotBeamInAChannel.cmd -show=multiBeam8.show
#
# -- plot streamlines:
#   plotStuff plotBeamInAChannel.cmd -show=bicBig2.show -vorMin=-10 -vorMax=5
#   plotStuff plotBeamInAChannel.cmd -show=bicBigE20RS1.show -vorMin=-10 -vorMax=5 -option=sl -name=bicBigE20RS1G2
#   plotStuff plotBeamInAChannel.cmd -show=bicBigE20RS100.show -vorMin=-10 -vorMax=5 -option=sl -name=bicBigE20RS100G2
# 
#  plotStuff plotBeamInAChannel.cmd -show=bicA.show -vorMin=-20 -vorMax=5
# 
#  plotStuff plotBeamInAChannel.cmd -show=bic2.show -vorMin=-20 -vorMax=5
#  plotStuff plotBeamInAChannel.cmd -show=bic4.show -vorMin=-20 -vorMax=5
#  plotStuff plotBeamInAChannel.cmd -show=bic8.show -vorMin=-20 -vorMax=5
#
#  plotStuff plotBeamInAChannel.cmd -show=bicG2.show -vorMin=-20 -vorMax=5
#  plotStuff plotBeamInAChannel.cmd -show=bicG4.show -vorMin=-20 -vorMax=5
#  plotStuff plotBeamInAChannel.cmd -show=bicG8.show -vorMin=-20 -vorMax=5
#
# -- plot streamlines:
#  plotStuff plotBeamInAChannel.cmd -show=bicG4long.show -vorMin=-10 -vorMax=5 -option=sl -name=bicBig_E5_R1_G4
#
# Movie:
#  plotStuff plotBeamInAChannel.cmd -show=multiBeam16m.show -vorMin=-100 -vorMax=60
#
#  plotStuff plotBeamInAChannel.cmd -show=multiBeam32.show -vorMin=-500 -vorMax=300
#
#
$show="beam4.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="bic"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name );
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
# 
if( $option eq "sl" ){ $cmd="erase"; }else{ $cmd="#"; }
$cmd
# 
forcing regions
  body force grid lines 1
  line width: 2
  # forcing region colour (bf,colour): 0 RED
exit
#
if( $option eq "sl" ){ $cmd="#"; }else{ $cmd=" "; }
# -- STREAM-LINES
DISPLAY AXES:0 0
DISPLAY LABELS:0 1
DISPLAY COLOUR BAR:0 0
#
# set view:0 0.278265 0.0060423 0 2.32062 1 0 0 0 1 0 0 0 1
reset
bigger 1.3
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 0.02
exit
$plotName = $name . "_SL.ps"; 
hardcopy file name:0 $plotName
hardcopy save:
pause
# 
# Zoom:
# set view:0 0.464345 0.00742516 0 5.38919 1 0 0 0 1 0 0 0 1
# set view:0 0.492117 0.00652821 0 6.94635 1 0 0 0 1 0 0 0 1
set view:0 0.267774 0.0112432 0 5.18392 1 0 0 0 1 0 0 0 1
$plotName = $name . "_SL_ZOOM.ps"; 
hardcopy file name:0 $plotName
hardcopy save:



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