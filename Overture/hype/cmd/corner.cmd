  plane or rhombus
    specify plane or rhombus by three points
    0 1 0  1 1 0  0 1 1
    mappingName
      top
    exit
  plane or rhombus
    specify plane or rhombus by three points
    0 0 1  0 0 0  0 1 1
    mappingName
      left
    exit
*
  plane or rhombus
    specify plane or rhombus by three points
    1 0 1  1 0 0  1 1 1
    mappingName
      right
    exit
*
  plane or rhombus
    specify plane or rhombus by three points
     0 0 1  1 0 1  0 1 1 
    mappingName
      front
    exit
*
  composite surface
    CSUP:add a mapping top
    CSUP:add a mapping left
    CSUP:add a mapping right
    CSUP:add a mapping front
    CSUP:determine topology
      deltaS 0.1
      build edge curves
      merge edge curves
      triangulate
      exit
    exit
  builder
    create surface grid...

      x+r:0 
      y+r:0
      surface grid options...
      initial curve:curve on surface
      choose point on surface 3 2.008583e-01 2.951191e-01 1.000000e+00 2.008583e-01 2.951191e-01
      choose point on surface 3 3.378134e-01 4.359557e-01 1.000000e+00 3.378134e-01 4.359557e-01
      choose point on surface 3 7.486239e-01 5.784620e-01 1.000000e+00 7.486239e-01 5.784620e-01
      done



      picking:create boundary curve
      choose edge curve 7
      done
      choose edge curve 11
      done
      picking:choose initial curve
      choose edge curve 2
      done
      backward
      generate



      x+r:0



      picking:create boundary curve
      y+r:0
      choose edge curve 7
      choose edge curve 2
      done


      choose edge curve 3
      done
      choose edge curve 1
      done
      picking:choose initial curve
      choose edge curve 2
      generate


      choose edge curve 2


      points on initial curve 5
*      debug 
*       7
      set view:0 0 0 0 1 0.999308 0.0070855 -0.0365087 0.0153458 0.815651 0.578341 0.0338762 -0.578501 0.814978
      picking:create boundary curve
      mogl-select:0 2 
            98 529179840 542390656  107 529179840 662832256  
      mogl-coordinates 1.445428e-01 1.445428e-01 4.330484e-01 4.330484e-01 5.291798e+08 -2.289581e-03 7.439499e-01 1.009659e+00
      done
      mogl-select:0 2 
            102 504232640 514424288  107 504232640 559597632  
      mogl-coordinates 7.522124e-01 7.522124e-01 3.447294e-01 3.447294e-01 5.042326e+08 1.014014e+00 6.041039e-01 9.965742e-01
      set view:0 0 0 0 1 0.947611 0.00633527 0.319365 -0.195326 0.80259 0.563646 -0.252748 -0.596497 0.76178
      done
      picking:choose initial curve
      mogl-select:0 2 
            93 409503264 415782592  107 409503264 429791904  
      mogl-coordinates 4.100295e-01 4.100295e-01 5.754986e-01 5.754986e-01 4.095033e+08 6.092042e-01 1.001183e+00 1.003737e+00
      set view:0 -0.0560472 -0.230088 0 3.94382 0.994895 -0.0159057 -0.0996497 0.0637491 0.864561 0.498468 0.0782247 -0.502276 0.861162
*
      backward
      reset:0
      set view:0 0 0 0 1 0.989781 0.0256243 -0.140273 0.0333418 0.914863 0.402386 0.138641 -0.402951 0.90466
      distance to march 0.2
      lines to march 3
      generate

pause
      exit
    exit
  exit




      points on initial curve 5
      mogl-select:0 2 
            93 416628000 420691936  107 416628000 437656224  
      mogl-coordinates 5.280236e-01 5.280236e-01 6.296296e-01 6.296296e-01 4.166280e+08 3.940115e-01 1.002193e+00 1.001285e+00
      backward
      distance to march 0.5
      lines to march 5
      debug
        7
      generate
pause
      exit
    exit
  exit
