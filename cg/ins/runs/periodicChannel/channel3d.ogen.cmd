################################################################################
## ogen command file for a simple 2d periodic channel, to be used with the cgins file periodicChannel.cmd
## 
## 130213: kkc initial version
##
##
$orderOfAccuracyStr = "second order"; $ng=2;
if( $order eq 4 ){ $orderOfAccuracyStr="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracyStr="sixth order"; $ng=4; };
##
setMultigridLevels($ml);
#
$gridFilename = "channel3d.$Nx.$Ny.$Nz.order${order}.ml${ml}.hdf";
create mappings
#
box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $Nxml = intmg($Nx);
    $Nyml = intmg($Ny);
    $Nzml = intmg($Nz);
    $Nxml $Nyml $Nzml
  periodicity
    1 0 1
  boundary conditions
$bcnumber = ($bc eq "wallModel" ? 1 : ($bc eq "noSlipWall" ? 2 : 3)); 
    -1 -1 $bcnumber $bcnumber -1 -1
  mappingName
    unstretched-channel
exit
#
stretch coordinates
  Stretch r2:itanh
    STP:stretch r2 itanh: layer 0 1 $ystr 0 (id>=0,weight,exponent,position)
    STP:stretch r2 itanh: layer 1 1 $ystr 1 (id>=0,weight,exponent,position)
    stretch grid
  mappingName
   channel
exit
#
exit
#
generate an overlapping grid
  channel
  done
change parameters
  ghost points
  all
    $ngp=$ng+1;
  $ng $ng $ng $ng $ng $ngp
   order of accuracy
      $orderOfAccuracyStr
  exit
  compute overlap
#pause
  exit
#
save a grid (compressed)
$gridFilename
channel
exit

