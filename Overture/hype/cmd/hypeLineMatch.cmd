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
    # apply boundary conditions to start curve 1 
    apply boundary conditions to start curve 0
    debug 
      3
    backward
    generate
#
    step
    ghost lines to plot: 2
    # set interpolation order to 4 so 2nd ghost line matches the data points
    # other-wise the 2nd ghost seems to be obtained from the mapping.map() 
    fourth order

    smoothing...
    GSM:number of iterations 1

    GSM:smooth grid

    GSM:smooth grid
