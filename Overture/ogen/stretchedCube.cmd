*
* Create a 3D cube with stretched grid lines
*
create mappings
  Box
    exit
  stretch coordinates
    stretch
*    choose a layer stretching a*tanh(b*(r-c))
*    along axis 0   
      specify stretching along axis=0 (x1)
        layers
        1
*         give a,b,c in above formula
        1. 10. .5
      exit
*    choose a stretching function with 2 
*    layers along axis1
      specify stretching along axis=1 (x2)
        layers
        2
*         give a,b,c for layer 1
        1. 10. 0.
*         give a,b,c for layer 2
        1. 10. 1.
      exit
    exit
  exit
exit this menu
generate an overlapping grid
  stretched-box
  done
  compute overlap
exit
save an overlapping grid
  stretchedCube.hdf
  stretchedCube
exit
