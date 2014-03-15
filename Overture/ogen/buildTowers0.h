#  ============== only include the tower ======================
$xTowerShift=0.;  $yTowerShift=0;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=0.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
# include turbineTower.h
include tower.h
# ************ Blade 1 *******************
# $bladeTheta=90.;
# include turbineBlade.h
# ************ Blade 2 *******************
# $bladeTheta=$bladeTheta+180.; $bladeShare=$bladeShare+1;
# include turbineBlade.h
# -- tower base that joins to the ground ---
include towerBase.h
