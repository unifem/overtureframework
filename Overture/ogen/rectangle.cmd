* make a rectangle with various aspect ratios (ar)
*
$xa=0.; $xb=1.; $ya=0.; $yb=1.; 
$order = "second order"; $ng=2;
$suffix = ""; 
* $name = "rectangle40.ar5"; $nx=41; $ny=11; $yb=.2; $ng=2; 
* $name = "rectangle80.ar5"; $nx=81; $ny=21; $yb=.2; $ng=2; 
* $name = "rectangle160.ar5"; $nx=161; $ny=41; $yb=.2; $ng=2; 
* 
* $name = "rectangle40.ar10"; $nx=41; $ny=11; $yb=.1; $ng=2; 
# $name = "rectangle80.ar10"; $nx=81; $ny=21; $yb=.1; $ng=2; 
* $name = "rectangle160.ar10"; $nx=161; $ny=41; $yb=.1; $ng=2; 
*
* $name = "rectangle40.ar20"; $nx=41; $ny=11; $yb=.05; $ng=2; 
$name = "rectangle80.ar20"; $nx=81; $ny=21; $yb=.05; $ng=2; 
* $name = "rectangle160.ar20"; $nx=161; $ny=41; $yb=.05; $ng=2; 
*
* $name = "offsetNonSquare5"; $nx=6; $ny=$nx; $ya=.1; $yb=1.1;  # off axis square for testing axis-symmetric 
* $name = "offsetSquare5"; $nx=6; $ny=$nx; $ya=.1; $yb=1.1;  # off axis square for testing axis-symmetric 
* $name = "offsetSquare20"; $nx=21; $ny=$nx; $ya=.1; $yb=1.1;  # off axis square for testing axis-symmetric 
*
* $name = "squareOnAxis5"; $nx=6; $ny=$nx; $ya=0.; $yb=1.;  # on axis square for testing axis-symmetric 
* $name = "squareOnAxis20"; $nx=21; $ny=$nx; $ya=0.; $yb=1.;  # on axis square for testing axis-symmetric 
*
* $lines = "17"; $order = "fourth order"; $suffix=".order4"; $ng=2; 
* $lines = "21"; $order = "fourth order"; $suffix=".order4"; $ng=2; 
* $lines = "33"; $order = "fourth order"; $suffix=".order4"; $ng=2; 
* 
*
create mappings
  rectangle
  set corners
    $xa $xb $ya $yb
    mappingName
      rectangle-Cartesian
    lines
      $nx $ny
    boundary conditions
      * set the singular axis to a BC=13
      # $bcCommand = $ya == 0. ? "1 1 13 1" : "1 2 3 4";
      $bcCommand = "1 2 3 4";
      $bcCommand
  exit
*  -- make non-Cartesian
*   rotate/scale/shift
*     mappingName
*      rectangle
*     exit
exit
*
generate an overlapping grid
  rectangle
  done
  change parameters
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
    order of accuracy
      $order
  exit
  compute overlap
*   display computed geometry
exit
*
save an overlapping grid
  $name.hdf
  $name
exit

