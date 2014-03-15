  read iges file
    /home/henshaw/Overture/mapping/ship_kcs.igs
    continue
    choose all
*    choose some
*      0 20
*    choose a list
*      10 11
*    done
*      6 8 9 17
*       1 10 11 12 13 14 15
*        5 7 8
*      something funny with these next two:
*      3 4 
*
*       4
*      2 3 4 5 6 7 8 9 
*     surface 10 doesn't quit plot correctly near (0,1) in trim space
*       10 12
*      11 15
    mappingName
      kcs
    determine topology
      deltaS 30.
      merge tolerance 1.5
*      curvatureTolerance 0.025
      build edge curves
      merge edge curves
      triangulate
      y-r:0 140
      x+r:0 20

    exit
  exit
  builder
    create surface grid...


      mogl-select:0 2 
            507 1019436288 1019632384  484 1019401792 1019622656  
      mogl-coordinates 3.223881e-01 3.223881e-01 1.019402e+09 7.485415e+03 -1.354905e+00 1.238266e+02
      surface grid options...
      Start curve parameter bounds .9 1.
*      Start curve parameter bounds .9 .95
*      Start curve parameter bounds .9 .915
      points on initial curve 41
      forward
      lines to march 5
      distance to march 50
      extrapolate an extra ghost line
      generate
*      lines to step 3
*      step
      exit
    create volume grid...
      reset:0
      x+r:0
      backward
      distance to march 30
      lines to march 7 
      implicit coefficient 1.
      BC: bottom fix y, float x and z
      y-r:0 220
      x+r:0


      debug
       7
      plot ghost lines
       1









  unstructured
    build topology
      kcs
    deltaS 30.
    merge tolerance 1.5
    y-r:0 140
    x+r:0 20


   builder





     determine topology
    exit
  builder
