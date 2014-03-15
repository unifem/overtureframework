create mappings 
  nurbs (curve) 
    use Eleven eval 
    lines 
    41 
    mappingName
    nurbsCurve
    exit
  check mapping
** 
** CHECKING NURBS CURVE
**
  nurbsCurve
  nurbs (surface)
    use Eleven eval
    lines
    81,81
    mappingName 
    nurbsSurface
    exit
** 
** CHECKING NURBS SURFACE
**
  check mapping
  nurbsSurface
*
*
  box
    lines
    41,21,11
    exit
  stretch coordinates
    Stretch r2:exp
    close r2 stretching parameters
    Stretch r2:exp blend
    close r2 stretching parameters
    Stretch r1:itanh
    Stretch r2:itanh
    Stretch r3:itanh
    STP:stretch r1 itanh: position and min dx .5 0.01
    STP:stretch r2 itanh: position and min dx .5 0.01
    STP:stretch r3 itanh: position and min dx .5 0.01
    stretch grid
    exit
rotate/scale/shift
    rotate
    23,1
    0 0 0
    rotate
    56,0
    0 0 0
    rotate
    12,2
    0 0 0
    exit
nurbs (surface)
    interpolate from mapping with options
    Transform
    parameterize by index (uniform)
    done
    mappingName
    nurbsVolume
    lines 
    41, 31, 57
    exit
** 
** CHECKING NURBS VOLUME
**
  check mapping
  nurbsVolume
pause
  exit this menu
exit
