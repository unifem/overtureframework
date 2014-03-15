create mappings
    spline
    enter spline points
      62
      0.811700  0.060300
      0.809400  0.061200
      0.802300  0.063900
      0.790500  0.068500
      0.774200  0.075000
      0.753600  0.082900
      0.729000  0.092400
      0.700500  0.101600
      0.668600  0.110500
      0.633600  0.117100
      0.596000  0.122900
      0.556100  0.126500
      0.514500  0.128900
      0.471500  0.129900
      0.427800  0.129300
      0.383900  0.12740
      0.340200  0.123900
      0.297300  0.119100
      0.255600  0.11290
      0.215800  0.10550
      0.178100  0.096700
      0.143100  0.08680
      0.111200  0.076000
      0.082800  0.064400
      0.058100  0.052400
      0.037500  0.040000
      0.021200  0.027900
      0.009500  0.015800
      0.002400  0.005600
      0.000000  0.000000
      0.002400  -.003400
      0.009500  -.008900
      0.021200  -.014700
      0.037500  -.020500
      0.058100  -.026300
      0.082800  -.031800
      0.111200  -.037100
      0.143100  -.041900
      0.178100  -.046200
      0.215800  -.049900
      0.255600  -.053000
      0.297300  -.055300
      0.340200  -.057100
      0.383900  -.058100
      0.427800  -.058400
      0.471500  -.057700
      0.514500  -.056100
      0.556100  -.053400
      0.596000  -.049500
      0.633600  -.042400
      0.650000  -.018400
      0.654800  0.007200
      0.665100  0.026400
      0.682300  0.042400
      0.697200  0.050800
      0.716900  0.057800
      0.730700  0.061100
      0.747600  0.063400
      0.766000  0.064400
      0.780800  0.063600
      0.795800  0.061800
      0.811700  0.060300
    periodicity
      2
    mappingName
      main-element-spline
    exit
  spline
  enter spline points
      39
      1.000000  0.000000
      0.996900  0.001000
      0.987500  0.003700
      0.971900  0.008500
      0.950500  0.015000
      0.923400  0.023000
      0.890900  0.033300
      0.853500  0.045000
      0.811700  0.058400
      0.795800  0.061800
      0.780800  0.063600
      0.766000  0.064400
      0.747600  0.063400
      0.730700  0.061100
      0.716900  0.057800
      0.697200  0.050800
      0.682300  0.042400
      0.665100  0.026400
      0.654800  0.007200
      0.650000  -.018400
      0.654800  -.029800
      0.665100  -.034100
      0.682300  -.032900
      0.697200  -.031100
      0.716900  -.028100
      0.730700  -.025700
      0.747600  -.023400
      0.766000  -.020800
      0.780800  -.018300
      0.795800  -.016500
      0.811700  -.013700
      0.853500  -.007900
      0.890900  -.003300
      0.923400  -.000300
      0.950500  0.000600
      0.971900  0.000900
      0.987500  0.000700
      0.996900  0.000300
      1.000000  0.000000  
    periodicity
      2
    mappingName
      flap-cruise-config-spline
    exit
  line (2D)
    set end points
      0.811700  1.126 0.0603 -0.1426
    mappingName
      wakeline
    exit
  rotate/scale/shift
    transform which mapping?
    flap-cruise-config-spline
    shift
      .12, -0.01
    rotate
      -24
       0.650000  -.018400
    mappingName
       flap-24-deg-spline
    exit
  change a mapping
  main-element-spline
    lines
      81
    exit
  stretch coordinates
    transform which mapping?
       main-element-spline
    stretch
     specify stretching along axis=0 (x1)
        layers
          3
          1,5,0
          1,8,.48
          1,5,1
        exit
      exit
    exit
   hyperbolic
      lines
        81,21
      lines to march
        21
      initial grid spacing
        0.001
*      inverse hyperbolic stretching
*        20
      boundary conditions for marching
        left   (side=0,axis=0)
        trailing edge
        right  (side=1,axis=0)
        trailing edge
        exit
      periodicity
        2,0
*      geometric stretching, specified ratio
*        1.05
      geometric stretching, specified ratio
        1.2
      uniform dissipation coefficient
        0.0
      volume smoothing iterations
        40      
      upwind dissipation coefficient
        2
      generate
    boundary conditions
      -1, -1, 1, 0
    mappingName
      main-element
    exit
  hyperbolic
    start from which curve/surface?
    flap-24-deg-spline
    lines
      31,21
    lines to march
      21
    boundary conditions for marching
      left   (side=0,axis=0)
      trailing edge
      right  (side=1,axis=0)
      trailing edge
      exit
    initial grid spacing
      0.001
    geometric stretching, specified ratio
      1.2
    uniform dissipation coefficient
      0
    upwind dissipation coefficient
      1
    generate
    boundary conditions
      -1, -1, 1, 0
    mappingName
      flap-24-degrees
    exit
  rectangle
    set corners
      -.5,1.6,-.75,.75
    lines
      57,57
    mappingName
      background
    exit
  exit this menu
generate a hybrid mesh
    background
    main-element
    flap-24-degrees
    done choosing mappings
*  change parameters
*    compute for hybrid mesh
    * useBoundaryAdjustment
*    ghost points
*      all
*      2 2 2 2 2 2
*  exit
*  debug
*    7
  compute overlap
  exit
  exit
  set plotting frequency (<1 for never)
    -1
  continue generation
    exit
  exit

