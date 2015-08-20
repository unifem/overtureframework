#
#  Plot results from Cgmx scattering from the crew reetnery vehichle
#
#    plotStuff plotReentryVehicle.cmd -show=crv2.show
#    plotStuff plotReentryVehicle.cmd -show=crv4.show
#    plotStuff plotReentryVehicle.cmd -show=crv8.show
#
$show="crv2.show"; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show );
# -------------------------------------------------------------------------------------------------
$show
#
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 -0.01251 0.000152542 0 1.09579 0.966822 -0.105335 0.232722 0.0244387 0.944987 0.326194 -0.254279 -0.309684 0.916208
previous
#
grid
  toggle grid 0 0
  plot block boundaries 0
  coarsening factor 2
  grid colour 1 BRASS
  grid colour 2 BRASS
  grid colour 3 BRASS
  plot grid lines 0
exit this menu
# 
contour
  pick to delete contour planes
  delete contour plane 0
  contour lines 0
exit
