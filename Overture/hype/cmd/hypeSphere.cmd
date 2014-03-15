#
#  Generate a surface patch on a sphere to demonstrate the 
#  use of the boundary offset
#
  sphere
    surface or volume (toggle)
    lines
      201 201 
    mappingName
    sphereSurface
    exit
# 
  builder
    create surface grid...
      surface grid options...
      initial curve:coordinate line 0
        choose point on surface 0 4.998568e-01 -6.200494e-03 -1.023264e-02 5.065148e-01 9.980259e-01
      done
      reparameterize initial curve
        set corners
        .4 .6
        exit
      target grid spacing .05 .05 (tang,normal, <0 : use default)
      forward and backward
      BC: left (backward) free floating
      BC: right (backward) free floating
      lines to march 7 7 (forward,backward) 
      ghost lines to plot: 3
      plot reference surface 0
      plot reference surface 1
      generate
# 
      y+r 90


      DISPLAY AXES:0 0
      line width scale factor:0 5
      hardcopy vertical resolution:0 2048
      hardcopy horizontal resolution:0 2048
#
      boundary offset 0 0 0 0 (l r b t)
      hardcopy file name:0 hypeSphereBoundaryOffset0.pdf
      hardcopy save:0
#
      boundary offset 1 1 1 1 (l r b t)
      hardcopy file name:0 hypeSphereBoundaryOffset1.pdf
      hardcopy save:0
#
      boundary offset 2 2 2 2 (l r b t)
      hardcopy file name:0 hypeSphereBoundaryOffset2.pdf
      hardcopy save:0


  Sphere
    surface or volume (toggle)
*    lines
*      7 7
    exit
  hyperbolic
    lines to march
      2
    distance to march
      .2
*    generate

