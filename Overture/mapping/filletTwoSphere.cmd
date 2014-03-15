* build a sphere
  Sphere
    surface or volume (toggle)
    mappingName
      sphere1
  exit
* build a second sphere
  Sphere
    surface or volume (toggle)
    mappingName
      sphere2
    centre for sphere
      .5 .5
    exit
* build the fillet
  fillet
    Start curve 1:sphere1
    Start curve 2:sphere2
    orient curve 1+ to curve 2-
    compute fillet


    choose curves
    sphere1
    sphere2
   orient curve 1+ to curve 2-
    compute fillet
