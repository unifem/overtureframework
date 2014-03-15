  *
  nurbs (surface)
    read NURBS data, format cheryl
    WindwardSurf.nurbs
    lines
    141 101  81 51 51  15 15
*   restrict the domain
*   0. .975 0. 1.
    mappingName
    windWardSurface
    exit
  *
  nurbs (surface)
    read NURBS data, format cheryl
    LeewardSurf.nurbs
    lines
    141 101 81 81 51 51 31 31 15 15
    mappingName
    leeWardSurface
    exit
  *
  nurbs (surface)
    read NURBS data, format cheryl
    PerimeterSurf.nurbs
    lines
    15 301
    *     periodicity
    *       2 0
    * pause
    mappingName
    perimeterSurface
    exit
  *
  nurbs (surface)
    read NURBS data, format cheryl
    HeadSurf.nurbs
    lines
    31 31
    mappingName
    headSurface
    exit
  composite surface
    add all mappings
    plot normals (toggle)
    change the sign of a normal
      0
    change the sign of a normal
      2
    exit
*
  hyperbolic
    choose the initial curve
      create a curve from the surface
        reparameterized coordinate line
          2
          axis1=.5
    edit initial curve
      restrict the domain
       .1  .9 
      exit
    lines to march
      11
    distance to march
      20.
   geometric stretching, specified ratio
     1.15
    generate

