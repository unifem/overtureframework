*
* Sample grid showing the use of the Cross-section mapping
* to build a hollow pipe with a bend in it
* 
 create mappings
  Annulus
    make 3d (toggle)
      -2.5
    mappingName
      circle0
    exit
*
  Annulus
    make 3d (toggle)
      -2.0
    mappingName
      circle1
    exit
*
  Annulus
    make 3d (toggle)
      -1.0
    mappingName
      circle2
    exit
*
  Annulus
    make 3d (toggle)
      0.
    mappingName
      circle3
    exit
*
  rotate/scale/shift
   rotate
     10. 0
     0 0 0
    shift
      0. -.5 0.
    mappingName
      circle3-rotated
   exit
*
  Annulus
    centre for annulus
      0. -1.
    make 3d (toggle)
       1.0
    mappingName
      circle4
    exit
*
  Annulus
    centre for annulus
      0. -1.
    make 3d (toggle)
       2.
    mappingName
      circle5
    exit
*
  Annulus
    centre for annulus
      0. -1.
    make 3d (toggle)
       2.5
    mappingName
      circle6
    exit
*
  CrossSection
    mappingName 
      pipe
    general
      7
    circle0
    circle1
    circle2
    circle3-rotated
    circle4
    circle5
    circle6
  cubic
  exit
exit
* 
generate an overlapping grid
    pipe
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
* pause
  exit
*
save an overlapping grid
csPipe.hdf
pipe
exit

