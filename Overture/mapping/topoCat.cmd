*  create mappings
*
  read iges file
    /home/henshaw/iges/cat2.igs
    continue
*  choose some
*   0 10
  choose some
    0 -1  112  -1
pause
*     need to fix 33 and 112
*   -- trouble with 29..32 : matches at 3 points, but curves do not match! tol=default
*     29 32
*    0 -1 
*   choose a list
*     109 181 182 183
*
*     67 106 108 109 158 159 165 170 181 182 
*     183 184 185 186 187 188 189 190
*   done
*       0
*       72
*     72 73 : generate an out of bounding box error
*    72 73 75
*   72 73 75 78 79 123 124 125
    * here is a valve
*    206 207 208 209 210 211 212 213 214 215 216
*  right port
*       68  72  73  75  76  77  78  79  80  81 
*       82 122 123 124 125 126 127 128 129
     * here are the joining pieces
*      93 94 95 96 97 98 99
*    done
    mappingName
      cat
    * 
    *  --- fix a few surfaces ---- 
    * 
    CSUP:examine a sub-surface 33
    view domain
pause
    edit curves
    edit trim curve 0
    Mouse Mode Snap To Intersection
    snap to intersection 1 2 1 0 4.973569e-01 2.032036e-02 4.392081e-01 1.079841e-01 
    auto assemble
pause
    exit
*
    CSUP:examine a sub-surface 112
    view domain
    edit curves
    edit trim curve 0
    Mouse Mode Snap To Intersection
    snap to intersection 1 2 1 0 4.286356e-01 9.064402e-01 4.762119e-01 9.878658e-01 
    auto assemble
    exit
*
    CSUP:examine a sub-surface 26
    force trimming valid
    exit
*
    CSUP:examine a sub-surface 105
    force trimming valid
*
    exit
*
    determine topology
      $ds=5.; $da=$ds*$ds; 
      deltaS $ds
      maximum area $da
*   -- trouble with 29..32 : matches at 3 points, but curves do not match!
     * single precision seems to require .02
     merge tolerance .02   .01
     build edge curves
     merge edge curves
     triangulate


*  save in a data base file.
  exit
  open a data-base
  cat2Fixed.hdf
  open a new file
  put to the data-base
  cat
  close the data-base







    CSUP:examine a sub-surface 33
    view domain 
    edit curves 
    edit trim curve 0
    Mouse Mode Snap To Intersection 
    snap to intersection 1 2 1 0 4.709256e-01 1.992703e-02 4.286356e-01 9.220394e-02 
    auto assemble 
    exit
*
    CSUP:examine a sub-surface 112
    view domain 
    edit curves 
    edit trim curve 0
    Mouse Mode Snap To Intersection 
    snap to intersection 1 2 1 0 4.550668e-01 8.703803e-01 5.132156e-01 9.858126e-01
    auto assemble 
    exit 
*
    CSUP:examine a sub-surface 26
    force trimming valid
    exit
*
    CSUP:examine a sub-surface 105
    force trimming valid
    exit




*
*  --- fix two surfaces ----
*
    * 
    *  --- fix two surfaces ---- 
    * 
    CSUP:examine a sub-surface 33
    view domain
    edit curves
    mogl-select:0 1 
          60 1073741824 1073741824  
    mogl-coordinates 5.468278e-01 5.468278e-01 5.415473e-01 5.415473e-01 1.073742e+09 5.851083e-01 4.741857e-01 -6.784331e-10
    Mouse Mode Snap To Intersection
    mogl-select:0 1 
          63 1073741824 1073741824  
    mogl-coordinates 4.592145e-01 4.592145e-01 3.323782e-01 3.323782e-01 1.073742e+09 4.286356e-01 9.220394e-02 -6.822663e-10
    mogl-select:0 1 
          65 1073741824 1073741824  
    mogl-coordinates 4.833837e-01 4.833837e-01 2.922636e-01 2.922636e-01 1.073742e+09 4.709256e-01 1.992703e-02 -6.822663e-10
    auto assemble
    exit
    CSUP:examine a sub-surface 112
    view domain
    edit curves
    mogl-select:0 1 
          72 1073741824 1073741824  
    mogl-coordinates 6.586103e-01 6.586103e-01 7.134671e-01 7.134671e-01 1.073742e+09 7.768239e-01 9.888275e-01 -6.728924e-10
    Mouse Mode Snap To Intersection
    mogl-select:0 1 
          75 1073741824 1073741824  
    mogl-coordinates 5.075529e-01 5.075529e-01 7.163324e-01 7.163324e-01 1.073742e+09 5.132156e-01 9.858126e-01 -6.822663e-10
    mogl-select:0 1 
          77 1073741824 1073741824  
    mogl-coordinates 4.743202e-01 4.743202e-01 6.532952e-01 6.532952e-01 1.073742e+09 4.550668e-01 8.703803e-01 -6.822663e-10
    auto assemble
    exit


    CSUP:examine a sub-surface 33
    view domain
    Mouse Picking 2
    mogl-select:0 1 
          670 1181116032 1181116032  
    mogl-coordinates 4.716418e-01 4.716418e-01 1.181116e+09 4.542994e-01 7.013967e-02 -1.139369e-06
    Mouse Mode 5
    mogl-select:0 1 
          673 1181116032 1181116032  
    mogl-coordinates 4.597015e-01 4.597015e-01 1.181116e+09 4.295002e-01 7.013967e-02 -1.145806e-06
    mogl-select:0 1 
          675 1181116032 1181116032  
    mogl-coordinates 4.776119e-01 4.776119e-01 1.181116e+09 4.608334e-01 1.914627e-02 -1.145806e-06
    auto assemble
    exit
    CSUP:examine a sub-surface 112
    view domain
    Mouse Picking 2
    mogl-select:0 1 
          1925 1181116032 1181116032  
    mogl-coordinates 5.850746e-01 5.880597e-01 1.181116e+09 6.524339e-01 9.925694e-01 -1.130064e-06
    Mouse Mode 5
    mogl-select:0 1 
          1928 1181116032 1181116032  
    mogl-coordinates 4.985075e-01 4.985075e-01 1.181116e+09 4.973889e-01 9.905301e-01 -1.145806e-06
    mogl-select:0 1 
          1930 1181116032 1181116032  
    mogl-coordinates 4.716418e-01 4.716418e-01 1.181116e+09 4.503890e-01 8.714945e-01 -1.145806e-06
    auto assemble
    exit
    determine topology
      deltaS 10. 4. 2.
*   -- trouble with 29..32 : matches at 3 points, but curves do not match!
     merge tolerance  .01
     build edge curves
     merge edge curves
     triangulate







        CSUP:examine a sub-surface 33
        Mouse Picking 2
        mogl-select:0 1 
              676 1165851776 1166613888  
        mogl-coordinates 5.131195e-01 5.131195e-01 1.165852e+09 -6.926902e+01 6.262775e+01 1.546605e+02
        reset:0
        plot curve 0
        Mouse Mode 5
        mogl-select:0 1 
              675 1181116032 1181116032  
        mogl-coordinates 4.139942e-01 4.139942e-01 1.181116e+09 3.495896e-01 2.635930e-02 -1.145806e-06
        mogl-select:0 1 
              673 1181116032 1181116032  
        mogl-coordinates 4.344023e-01 4.344023e-01 1.181116e+09 3.852802e-01 -1.333101e-02 -1.145806e-06
        begin curve
        mogl-select:0 1 
              675 1181116032 1181116032  
        mogl-coordinates 6.297376e-01 6.297376e-01 1.181116e+09 7.268903e-01 2.675489e-02 -1.145806e-06
        mogl-select:0 1 
              672 1181116032 1181116032  
        mogl-coordinates 7.784256e-01 7.784256e-01 1.181116e+09 9.869218e-01 2.968373e-01 -1.145806e-06
        mogl-select:0 1 
              673 1181116032 1181116032  
        mogl-coordinates 7.026239e-01 7.026239e-01 1.181116e+09 8.543567e-01 6.969647e-01 -1.145806e-06
        exit
        exit
        CSUP:examine a sub-surface 112
        Mouse Picking 2
        mogl-select:0 1 
              1931 1167899008 1168589440  
        mogl-coordinates 7.172012e-01 7.172012e-01 1.167899e+09 7.674726e+01 5.530672e+01 1.524134e+02
        plot curve 0
        Mouse Mode 5
        mogl-select:0 1 
              1928 1181116032 1181116032  
        mogl-coordinates 3.965015e-01 3.965015e-01 1.181116e+09 3.189976e-01 9.743105e-01 -1.145806e-06
        mogl-select:0 1 
              1930 1181116032 1181116032  
        mogl-coordinates 4.314869e-01 4.314869e-01 1.181116e+09 3.801815e-01 1.030469e+00 -1.145806e-06
        begin curve
        mogl-select:0 1 
              1930 1181116032 1181116032  
        mogl-coordinates 5.364432e-01 5.364432e-01 1.181116e+09 5.637333e-01 5.607186e-01 -1.145806e-06
        mogl-select:0 1 
              1927 1181116032 1181116032  
        mogl-coordinates 7.930029e-01 7.930029e-01 1.181116e+09 1.012415e+00 7.480796e-01 -1.145806e-06
        mogl-select:0 1 
              1928 1181116032 1181116032  
        mogl-coordinates 7.201166e-01 7.201166e-01 1.181116e+09 8.849487e-01 9.924928e-01 -1.145806e-06
        exit
        exit
*
    mappingName
      cat
    determine topology
      deltaS 2.
*   -- trouble with 29..32 : matches at 3 points, but curves do not match!
     merge tolerance  .01
     build edge curves
     merge edge curves


    exit
*
  unstructured
    build topology
     cat
     deltaS 4.
*   -- trouble with 29..32 : matches at 3 points, but curves do not match!
     merge tolerance  .01
    x+r:0 45
