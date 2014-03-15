  * mbuilder command file 
  * 
  *   Build grids for the front wheel of the asmo automobile 
  * 
  * 
  * Asmo notes:  car bottom is at z=0 
  *              wheel bottom is a z=-33.558 
  * 
  read iges file 
    asmo.igs 
    continue 
    * 
    * Choose a set of surfaces near the wheel since it then 
    *  faster to dela with the surface 
    * 
    choose a list 
      *     0 89 11 3 
      90 12 54 53 88 225 89 55 235 0 244 2 1 11 3 
      78 83 215 216 
      84 243 234 224 214 
      done 
    x-r:0 
    x-r:0 
    CSUP:determine topology 
    merge tolerance .65 
    deltaS 10. 
    compute topology 
    exit 
    exit 
  *** 
  builder 
    set view:0 0 0 0 1 0.89535 -0.256258 -0.364254 -0.411604 -0.163763 -0.896529 0.170092 0.952635 -0.252102 
    *  surface grids near the front wheel 
    pause 
    * 
    * create main wheel surface grid 
    * 
    create surface grid... 
      choose boundary curve 1 1.274938e+02 -1.201653e+02 -3.350000e+01 
      done 
      surface grid options... 
      *      We must explicitly indicate that the start curve is periodic 
      edit initial curve 
        periodicity 
        2 
        exit 
      * 
      *     set the bc's -1=periodic, 1=physical boundary 0=interpolation 
      boundary conditions 
      -1 -1 1 0 
      points on initial curve 61 
      forward 
      distance to march 25 
      lines to march 13 
      *      reduce the smoothing since we have sharp corners 
      uniform dissipation 0.02 
      volume smooths 2 
      generate 
      set view:0 -0.0530974 0.268437 0 2.07975 0.89535 -0.256258 -0.364254 -0.411604 -0.163763 -0.896529 0.170092 0.952635 -0.252102* 
      * surface grid for wheel 
      pause 
      exit 
    * 
    *  main wheel base, volume grid 
    *    This volume grid is a bit tricky to make since there is a sharp 
    *    angle between the rubber and the road. 
    * 
    create volume grid... 
      name frontWheelBase 
      backward 
      *       As a BC we want the bottom to match to a plane 
      BC: bottom fix z, float x and y 
      * 
      *     The join between the wheel and road is a tight angle -- we therefore 
      *      increase the blending of the normals with the boundary since by default the 
      *      grid lines want to grow in an orthogonal direction 
      * 
      normal blending 20 20 20 20  (lines, left,right,bottom,top) 
      BC: top outward splay 
      outward splay .5 .5 .5 .5 (left,right,bottom,top for outward splay BC) 
      volume smooths 40 
      * 
      *   we need to reduce dissipation for the sharp convex corners 
      * 
      uniform dissipation 0.1 
      dissipation transition 10 
      distance to march 35 25 9 
      * we get a better grid if we take small steps 
      lines to march 53 51 35  11 
      Boundary Condition: back  4 
      Share Value: back  4 
      Boundary Condition: bottom 3 
      Share Value: bottom 3 
      * reduce lines on final grid 
      generate 
      * pause 
      lines 
      65 9 13   61 9 11 
      set view:0 -0.0530974 0.268437 0 2.07975 0.892921 -0.0589012 -0.446343 -0.414475 0.279557 -0.866059 0.17579 0.958321 0.22521 
      * volume grid matching wheel to the road 
      pause 
      exit 
    * 
    *   surface grid used to join wheel to body 
    * 
    x+r:0 
    create surface grid... 
      choose edge curve -106 1.544661e+02 -1.229999e+02 1.853815e-02 
      choose edge curve -109 1.167252e+02 -1.229872e+02 1.348547e-01 
      choose edge curve -113 1.051863e+02 -1.205729e+02 -8.873583e-04 
      choose edge curve -110 1.044706e+02 -1.078485e+02 -2.917035e-01 
      choose edge curve -103 1.068398e+02 -9.776197e+01 -1.418499e-01 
      choose edge curve -111 1.253661e+02 -9.700195e+01 -1.177881e-01 
      choose edge curve -104 1.605078e+02 -9.699510e+01 -1.064973e-01 
      choose edge curve -24 1.850226e+02 -9.699608e+01 -9.430876e-02 
      choose edge curve -112 1.948887e+02 -9.957857e+01 -1.560460e-01 
      choose edge curve -108 1.955290e+02 -1.129534e+02 -3.683522e-01 
      choose edge curve -102 1.931568e+02 -1.222528e+02 -1.032158e-01 
      choose edge curve -27 1.850530e+02 -1.230037e+02 -4.463791e-02 
      choose edge curve -107 1.710803e+02 -1.230029e+02 -1.875661e-02 
      done 
      points on initial curve 91 
      distance to march 45 
      lines to march 15 
      * set BC of start curve to be a physical boundary 
      boundary conditions 
      -1 -1 1 0 
      generate 
      set view:0 -0.0513244 0.0629554 0 1.6638 0.892921 -0.274181 -0.357094 -0.414475 -0.190926 -0.889808 0.17579 0.942535 -0.284123 
      * surface grid joining the wheel to the body 
      pause 
      exit 
    * 
    create volume grid... 
      name frontWheelJoin 
      backward 
      * 
      *      As a boundary condition we want the grid to match to the body surface: 
      marching options... 
      BC: bottom match to a mapping 
      asmo.igs.compositeSurface 
      * 
      distance to march 28. 
      lines to march 11 
      generate 
      * volume grid joining the wheel to the body 
      pause 
      *     Set the boundary conditions 
      Boundary Condition: bottom  4 
      Share Value: bottom  4 
      Boundary Condition: back  2 
      Share Value: back  2 
      * 
      *     change the number of grid lines -- the existing grid will 
      *     just be interpolated to the new positions 
      * 
      lines 
      81 9 9   91 7 9 
      exit 
    * 
    build a box grid 
      x bounds: -200 1000 
      y bounds: -200 0 
      z bounds: -33.558 400 
      lines: 161 33 65     161 31 61  121 21 45  51  81 15 33 
      box details... 
        mappingName 
        mainBoxForWheels 
        boundary conditions 
        2 3 4 1 5 6 
        share 
        0 0 0 1 3 0 
      pause 
        exit 
      exit 
    * 
pause
    save grids to a file... 
    file name: asmoWheels.hdf 
    save file 
    exit 
    exit 
  exit 
