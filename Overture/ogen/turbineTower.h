if( $numBladesPerTower eq "" ){ $numBladesPerTower=3; }
# 
include tower.h
# ************ Blade 1 *******************
$bladeShare=$bladeShare+1;
if( $numBladesPerTower>0 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
# ************ Blade 2 *******************
  $bladeTheta=$bladeTheta+120.; $bladeShare=$bladeShare+1;
if( $numBladesPerTower>1 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
# ************ Blade 3 *******************
  $bladeTheta=$bladeTheta+120.; $bladeShare=$bladeShare+1;
if( $numBladesPerTower>2 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
