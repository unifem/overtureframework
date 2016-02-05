#
#  hype test for marching off a surface and matching to another surface
#
# Examples:
#   ../hype hypePlaneMatch -nr=11 -ns=11 -nt=7
#   ../hype hypePlaneMatch -nr=5 -ns=5 -nt=4
#
$nr=11; $ns=11; $nt=7; 
# get command line arguments
GetOptions( "nr=i"=>\$nr,"ns=i"=>\$ns,"nt=i"=>\$nt );
#
# Match to this plane (on the right side, angled to the right
  plane or rhombus
    specify plane or rhombus by three points
      # 1 0 0  1 0 1   2. 1  0
      1 0 0  1 0 1   1.5 1  0
    mappingName
      matchingBoundary
  exit
# Starting plane:
  plane or rhombus
    specify plane or rhombus by three points
      0 0 0  1 0 0   0 0 1
    # boundary conditions
    #  -1 -1 -1 -1 1 2
  exit
#
  hyperbolic 
    BC: right match to a mapping
      matchingBoundary
   lines to march $nt
   points on initial curve $nr, $ns
   distance to march .4
   #
   plot cell quality
   plot bad cells 1
   # use new ghost point option
   GSM:ghost point option 1
   # this next option will set ghost values on the start line using the BC's
   apply boundary conditions to start curve 1
   debug 
     3
   backward
   generate



  line (2D)
   # line at angle to match while marching 
    specify end points
    # 
       1 0 2 1
    # acute corner:
#      1 0 .5 1
    mappingName
      matchingBoundary
    exit
# Horizontal line for start curve
  line (2D)
    lines
      21
    exit
*
  hyperbolic 
    BC: right match to a mapping
    matchingBoundary
    lines to march 3 
    distance to march .1
    plot cell quality
    plot bad cells 1
    # use new ghost point option
    GSM:ghost point option 1
    # this next option will set ghost values on the start line using the BC's
    apply boundary conditions to start curve 1
    debug 
      3
    backward
    generate
