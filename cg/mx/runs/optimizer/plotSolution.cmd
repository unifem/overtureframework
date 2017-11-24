# Plot solutions created by the optimizer
#    plotStuff plotSolutions -show=<>
#
$show="optimizer.show"; $solution=-1; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"solution=i"=>\$solution );
# ------------------------------------------------------------------------------------------------
#
$show
# 
previous
contour
  plot:Ex
  # plot:Ey
  vertical scale factor 0.
  pause
exit this menu
exit
