#
# In this example we show how to build a grid over the pole
# of a sphere using hype. This approach can be used for other
# surfaces with a polar singularity. Note that we cannot grow
# a hyperbolic grid on the original sphere directly since we
# do not know that the sphere is periodic across the poles.
#
#  Steps:
#    1. build a composite surface and global triangulation for two halves of a sphere
#       (We need to first split the sphere into two halves 
#        since the topology routine doesn't like periodic surfaces)
#    2. Enter "builder" and generate the surface grid over the pole.
#
  sphere
    surface or volume (toggle)
    bounds on theta (longitude)
    0 .5
    lines
      31 21
    mappingName
    sphereTop
    exit
  sphere
    surface or volume (toggle)
    bounds on theta (longitude)
      .5 1. 
    lines
      31 21
    mappingName
    sphereBottom
    exit
  composite surface
    CSUP:add a mapping sphereTop
    CSUP:add a mapping sphereBottom
    CSUP:plotObject
    CSUP:determine topology
    deltaS 0.05
    maximum area 4.e-4
    compute topology
  exit
  exit
  builder 
    build curve on surface
      plane point 1 -0.2 -0.2 0
      plane point 2 -.2 -.2 1
      plane point 3 -.2 .2 0
      cut with plane
      exit
    create surface grid...
      choose boundary curve 0
      done
      forward
      distance to march .4 
      lines to march 41 
      generate
      name sphereCap
      exit

