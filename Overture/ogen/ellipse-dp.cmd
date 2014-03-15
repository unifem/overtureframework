*
* Make an ellipsoid airfoil
*
create mappings
  *
  * First make a back-ground grid  
  *
  rectangle
    mappingName
      backGround
    specify corners
      -1.5 -1.5 2.5 1.5 
    lines
      41 31
  exit
  * make an ellipse as an inner boundary
  Circle or ellipse
    mappingName
      innerEllipse
    specify centre
     .5 .0
    specify axes of the ellipse
      1. .25
  exit
  * make an ellipse as an outer boundary
  Circle or ellipse
    mappingName
      outerEllipse
    specify centre
     .5 .0
    specify axes of the ellipse
      1.5 1.
  exit
  * blend the curves to make a grid
  tfi
    choose bottom curve
      innerEllipse
    choose top curve
      outerEllipse
    boundary conditions
      -1 -1 1 0
    lines
      71 15
    mappingName
      airfoil-tfi
    * pause
  exit
  *
   DataPointMapping
     build from a mapping
       airfoil-tfi
     mappingName
       airfoil-tfi-dp
     * interp order
     *   4
  exit
exit
*
* make an overlapping grid
*
generate an overlapping grid
    backGround
    airfoil-tfi-dp
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
ellipse-dp.hdf
ellipse-dp
exit
