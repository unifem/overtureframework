#
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyl.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyla.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylb.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylc.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyld.show -name=submergedCyld -vorMin=-10. -vorMax=10.
#
# -- plots for doc: 
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylG4.show -name=submergedCylG4 -vorMin=-10. -vorMax=10.
#
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyle.show -vorMin=-10. -vorMax=10. [ longer domain G2
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylf.show -vorMin=-10. -vorMax=10. [ longer domain G4
# 
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylgm5.show -vorMin=-10. -vorMax=10. [ g=-5 
#
#
$show="iceDeform4.show"; $name="subMergedCyl"; 
$vorMin=-20; $vorMax=20.; $numStreamLines=100; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "numStreamLines=i"=>\$numStreamLines );
#
$show
# 
derived types
vorticity
exit
#
contour
  plot:vorticity
  coarsening factor 1 (<0 : adaptive)
  min max $vorMin $vorMax
  vertical scale factor 0.
  plot contour lines (toggle)
 exit


x-
DISPLAY AXES:0 0
#
# plot contours and streamlines at different times:
#
#  plotContours(num,timeLabel)
sub plotContours\
{ local($num,$label)=@_; \
  $plotName = $name . "t$label" . "vorticity.ps"; \
  $cmds = "erase\n contour\n exit\n solution: $num \n" . \
   "plot:vorticity\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
  $plotName = $name . "t$label" . "sl.ps"; \
  $cmds .= "erase\n stream lines\n streamline density 80\n arrow size .025\n exit\n" .  \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
}
#
plotContours(11,"1p0"); 
$cmds
pause
#
plotContours(21,"2p0"); 
$cmds
#
plotContours(31,"3p0"); 
$cmds
#
plotContours(51,"5p0"); 
$cmds
#
plotContours(101,"10p0"); 
$cmds
#
plotContours(201,"20p0"); 
$cmds

#


# 
# -- movie
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 0.00294913 -0.0107739 0 1.30058 1 0 0 0 1 0 0 0 1
save movie files 1
movie file name: submergedCylinder



stream lines
  streamline density $numStreamLines
 exit





derived types
vorticity
exit
#
contour
  plot:vorticity
  coarsening factor 1 (<0 : adaptive)
  min max $vorMin $vorMax
  vertical scale factor 0.
  plot contour lines (toggle)
  exit
# 


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