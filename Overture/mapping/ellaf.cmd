*
* Smooth a diamond airfoil with the 
*  elliptic transform mapping.
*
* first make a diamond airfoil
*
Airfoil
  airfoil type
    diamond
  thickness-chord ratio
    .2
  lines
    51 21
exit
*
* now smooth the diamond airfoil
*
elliptic
  *
  * do not project back onto the original mapping
  * since it has a discontinuity
  *
  project onto original mapping (toggle)
  * 
  * now generate the elliptic transform:
  * 
  elliptic smoothing
exit
