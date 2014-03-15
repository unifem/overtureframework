* make a short channel
* 
$xa=0.; $xb=1.; $ya=0.; $yb=1.; 
* Keep this next one for checking:
$name="channelShort.hdf"; $nx=201; $ny=3;  
* $name="channelShort1.hdf"; $nx=5; $ny=3;
* $name="channelShort2.hdf"; $nx=401; $ny=3;
* $name="channelShort4.hdf"; $nx=801; $ny=3;
* $name="channelShort8.hdf"; $nx=1601; $ny=3;
* $name="channelShort32.hdf"; $nx=6401; $ny=3;
* $name="channelShort64.hdf"; $nx=12801; $ny=3;
* $name="channelShortCoarse.hdf"; $nx=11; $ny=5;
*
* $name="channelShortish.hdf"; $nx=401; $ny=3; $xa=-1.; $xb=1.; 
*
create mappings
  rectangle
    mappingName
      rectangle
    set corners
      $xa $xb $ya $yb
    lines
      $nx $ny
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  rectangle
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
  channel
exit
