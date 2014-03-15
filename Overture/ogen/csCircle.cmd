create mappings
*
* Define a cross section mapping
*
Circle or ellipse (3D)
  specify radius of the circle
    1.
  specify centre
    0. 0. 0.
  mappingName
   circle0
exit
Circle or ellipse (3D)
  specify radius of the circle
    1.
  specify centre
    0. 0. .4
  mappingName
   circle1
exit
Circle or ellipse (3D)
  specify radius of the circle
    .8
  specify centre
    0. 0. .6
  mappingName
   circle2
exit
Circle or ellipse (3D)
  specify radius of the circle
    .8
  specify centre
    0. 0. 1.
  mappingName
   circle3
exit
*
* make a cross section mapping
CrossSection
  general
  4
  circle0
  circle1
  circle2
  circle3



