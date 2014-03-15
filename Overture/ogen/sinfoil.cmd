*
*  Make a channel with an sinusoidal airfoil along the bottom
*
* $factor=.25; $name="sinfoila.hdf"; 
* $factor=.5; $name="sinfoil0.hdf"; 
$factor=1; $name="sinfoil.hdf"; 
* $factor=2; $name="sinfoil$factor.hdf"; 
* 
$ds = 2./64./$factor; 
*
create mappings
  airfoil
    airfoil type
      sinusoid
   mappingName
    Airfoil
  exit
*
  stretch coordinates
    transform which mapping?
      Airfoil
    stretch
      specify stretching along axis=1
      layers
        1
      1. 5. 0.
    exit
  exit
  lines
    $nx = int( 2./$ds + 1.5 );
    $ny = int( .5/$ds + 1.5 );
    * 65 17
    $nx $ny
  boundary conditions
    2 3 1 1
  mappingName
    airfoil
  exit
exit
*
*
* make an overlapping grid
*
generate an overlapping grid
    airfoil
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
$name
sinfoil
exit

