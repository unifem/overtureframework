#
#  Create an annulus and stretch the grid lines
#
create mappings
# create an Annulus
  Annulus
  lines
    41 11
  exit
# stretch the grid lines
  stretch coordinates
    transform which mapping?
      Annulus
    stretch
      specify stretching along axis=0
# choose a layer stretching a*tanh(b*(r-c))     
        layers
          1
#         give a,b,c in above formula
          1. 10. .5
        exit
      specify stretching along axis=1
        layers
          1
          1. 5. 0.
      exit
    exit
  exit
exit this menu
#
# make an overlapping grid
#
generate an overlapping grid
  stretched-Annulus
  done
  compute overlap
  exit
#
# save as an hdf file
#
save an overlapping grid
stretchedAnnulus.hdf
grid
exit
