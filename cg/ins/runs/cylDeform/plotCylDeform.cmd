#
#   plotStuff plotCylDeform -show=cylDeformG1.show
#   plotStuff plotCylDeform -show=cylDeformG2.show
#
#
$show="cylDeformG1.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="cylDeform"; $matlab="cylDeform"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name, "matlab=s"=>\$matlab );
#
$show
# 
previous
grid
  toggle grid 0 0
  toggle boundary 1 1 1 0
  toggle boundary 0 1 1 0
  plot grid lines 0
  plot block boundaries 0 
  grid colour 1 BRASS
exit this menu
contour
  exit
plot:u
set view:0 0.00302115 -0.00302115 0 1.0216 0.5 0.433013 -0.75 0 0.866025 0.5 0.866025 -0.25 0.433013
DISPLAY AXES:0 0
