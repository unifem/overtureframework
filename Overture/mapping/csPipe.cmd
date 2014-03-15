* create mappings
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
*   pause
exit
*
  * 
  copy a mapping
  pipe
   mappingName
    pipe2
  exit
open a data-base
  pm.hdf
  open a new file
put to the data-base
  pipe2
close the data-base
*




