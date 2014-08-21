#
# Hyperbolic grid around a naca airfoil
#
create mappings
  # make the NACA airfoil (curve)
  Airfoil
    airfoil type
      naca
    camber 
      .1
    lines
     91
    mappingName
      airfoil-curve
# pause
    exit
#
  hyperbolic
#
    BC: left trailing edge
    BC: right trailing edge
#
    lines to march 21
    distance to march .45
#    uniform dissipation coefficient .01
    geometric stretch factor 1.3
# 
    generate
# pause
    mappingName
     airfoil
    boundary conditions
      -1 -1 1 0
    exit
 exit