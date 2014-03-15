*
*  Grid for a square object in a box
*  
*
create mappings
*
$ds=.025; 
*
rectangle
  set corners
    -1. 2. -1. 2.
  lines
    $nx = int( 4./$ds +1.5 ); $ny=$nx; 
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
    backGround
exit
* 
* Build a curve representing the square boundary -- merge 4 nurbs together
* 
  nurbs (curve)
    enter points
    2 3
    0 0 
    1 0
    mappingName
    bottom
    exit
  nurbs (curve)
    enter points
    2 3
    1 0
    1 1
    merge
    bottom
    lines
    41 
    mappingName
    nurbs2
    exit
  nurbs (curve)
    enter points
    2 3
    1 1
    0 1
    merge
    nurbs2
    mappingName
    nurbs3
    exit
  nurbs (curve)
    enter points
    2 3
    0 1
    0 0
    merge
    nurbs3
    mappingName
    squareBoundary
    lines
     121 
    exit
  stretch coordinates
    Stretch r1:itanh
    $a=.25; $b=40.; 
    STP:stretch r1 itanh: layer 0 $a $b 0 (id>=0,weight,exponent,position)
    STP:stretch r1 itanh: layer 1 $a $b 0.25 (id>=0,weight,exponent,position)
    STP:stretch r1 itanh: layer 2 $a $b 0.5 (id>=0,weight,exponent,position)
    STP:stretch r1 itanh: layer 3 $a $b 0.75 (id>=0,weight,exponent,position)
    stretch grid
    stretch grid 
    exit
  hyperbolic
    distance to march .2  
    uniform dissipation 0.01
    volume smooths 4
    spacing: geometric
    geometric stretch factor 1.1 
    lines to march 21
    generate
    name square
  exit
exit
* 
generate an overlapping grid
    backGround
    square
  done
  change parameters
    * choose implicit or explicit interpolation
    ghost points
      all
      2 2 2 2 2 2 
  exit
* display intermediate results
* 
  compute overlap
* pause
  exit
*
save an overlapping grid
* 
squareObject.hdf
squareObject
exit
