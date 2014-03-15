create mappings
  rectangle
    mappingName
      backGround
    boundary conditions
      2 2 2 2
    specify corners
      -7 -7 100 7 
    lines
      50 25
  exit
  spline
    enter spline points
      17
     31.3 0
     30 .15
     25 .35
     20 .55
     15 .7
      2 .7
      1 .65
      .03 .1
      0 0
      .03 -.1
      1 -.65
      2 -.7
     15 -.7
     20 -.55
     25 -.35
     30 -.15
     31.3 0
    mappingName
      fish
    boundary conditions
      1 1 1 1
  exit
  spline
    enter spline points
      19
	36.3 0
	36.29 .3
	35 3.5
	30 5.15
	25 5.35
	20 5.55
	15 5.7
	2 5.7
	1 5.65
	-5 0
	1 -5.65
	2 -5.7
	15 -5.7
	20 -5.55
	25 -5.35
	30.5 -5.15
	35 -3.5
        36.29 -.3
	36.3 0
    mappingName
      outer
    exit
  * blend the airfoil to the ellipse to make a grid
  tfi
    choose bottom curve
      fish
    choose top curve
      outer
    boundary conditions
      -1 -1 1 0
    lines
      101 11  322 32
    mappingName
      fish-tfi
  exit
  *
  stretch coordinates
    transform which mapping?
    fish-tfi
    stretch
      reset
      specify stretching along axis=0 (x1)
        stretching type
        inverse hyperbolic tangent
        layers
          2
          1. 5. 0.
          1. 20. .5
        exit
      exit
      mappingName
        stretched-fish
    exit
*
  elliptic
    transform which mapping?
      stretched-fish
    set Poisson j-line sources
      * number of sources, power,  diffusivity (exponent) and location
      1
      5.
      2.
      0. 
    set maximum number of iterations
      200
    * do NOT project since the mapping has a singularity
    * at the trailing edge:
    project onto original mapping (toggle)
    * generate the elliptic grid
    elliptic smoothing
    mappingName
      fish-grid
    pause
  exit





create mappings
  rectangle
    mappingName
      backGround
    boundary conditions
      2 2 2 2
    specify corners
      -7 -7 100 7 
    lines
      50 25
  exit
  spline
    enter spline points
      9
      0 0
      .01 .1
      1 .65
      2 .7
     15 .7
     20 .55
     25 .35
     30 .15
     31.3 0
    mappingName
      roof
    boundary conditions
      -1 -1 1 0
    exit
  spline
    enter spline points
      9
      0 0
      .01 -.1
      1 -.65
      2 -.7
     15 -.7
     20 -.55
     25 -.35
     30 -.15
     31.3 0
    mappingName
      bottom
    boundary conditions
      -1 -1 0 1
    exit
 tfi
    choose top curve    (r_2=1)
      roof
    choose bottom curve (r_2=0)
      bottom
    boundary conditions
      1 1 1 1
    lines
      300 11
    exit
  exit this menu
  generate an overlapping grid
    backGround
    TFIMapping
    done choosing mappings
