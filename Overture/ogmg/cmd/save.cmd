****************************************************************
*  cmd file to save the multigrid composite grid to a file
***************************************************************
$debug = 3;
$cycles=1; 
$trig = "turn on trigonometric"; $poly="turn on polynomial"; $tz=$trig; 
* 
* $grid="square5"; 
* $grid="square8"; $name="square8mg.hdf";
* $grid="nonSquare8"; $name="nonSquare8mg.hdf";
* Use grid with mg levels already built:
* $grid="square8mg.hdf";
* square16
* square10
* square32
* $grid="square64"; $name="square64mg.hdf";
* $grid="square16.order4"; 
* square64.order4
* square128.order4
* square256.order4
*
* $grid="sis2mg.hdf"; $name="sis2mgmg.hdf"; 
* $grid="sbs1mg.hdf"; $name="sbs1mgmg.hdf"; 
*
* $grid="cic.bbmg0.hdf"; $name="cic0mg.hdf"; 
* $grid="cic.bbmg3.hdf"; $name="cic3mg.hdf"; 
* $grid="cic.bbmg5.hdf"; $name="cic5mg.hdf"; 
* $grid="cic.bbmg6.hdf"; $name="cic6mg.hdf"; 
$grid="cic.bbmg7.hdf"; $name="cic7mg.hdf"; 
*
* cic3.order4
* about 1M pts
*cic4.order4
*
* square128
* square512
*
$grid
* 
laplace (predefined)
* heat equation (predefined)
$tz
 set trigonometric frequencies
   2. 2. 2.
*
*==================CHANGE PARAMETERS =======================================
change parameters
*
save the multigrid composite grid
  $name
* read the multigrid composite grid
*  $name
*
exit
 debug
   $debug
exit
