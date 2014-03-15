# -- tower base that joins to the ground ---
  cylinder
    bounds on the radial variable
      $rtb=$towerRadius+$rDist;
      $towerRadius $rtb
    centre for cylinder
      $ztb = groundLevel($xTowerShift,$yTowerShift);
      $xTowerShift $yTowerShift $ztb
    bounds on the axial variable
      $tbza=-.5; $tbzb=1.; 
      $tbza $tbzb
    boundary conditions
      -1 -1 0 0 1 2 
    lines
      $nTheta = intmg( $pi*($towerRadius+$rtb)/$ds + 1.5 );
      $nAxial = intmg( ($tbzb-tbza)/$ds + 1.5 );
      $nTheta $nAxial $nr
    mappingName
      $towerBaseCylName="towerBaseCyl$numberOfTowers";
      $towerBaseCylName
    exit
  join
    choose curves
      $towerBaseCylName
    ground (side=0,axis=2)
    compute join
    boundary conditions
      -1 -1 $groundBC 0 $towerBC 0 
    share  
       0 0 $groundShare 0 $towerShare 0 
    mappingName
     $towerBaseJoinName="towerBaseJoin$numberOfTowers";
     $towerBaseJoinName
    exit
#
  stretch coordinates
    transform which mapping?
      $towerBaseJoinName
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    $towerBaseStretchName="towerBaseStretch$numberOfTowers";
    STRT:name $towerBaseStretchName
  exit
#  -- convert the base to a Nurbs for faster evaluation
  nurbs 
    interpolate from a mapping 
      $towerBaseStretchName
    mappingName
     $towerBaseName="towerBase$numberOfTowers";
     $gridNames.="\n$towerBaseName"; 
     $towerBaseName
    exit
#
# Define an explicit hole cutter 
#  This cylinder is used to cut holes at the base of the tower where the ground grid is
#  supposed to be removed, but the implicit hole cutter has trouble.
  cylinder
    lines
     $outerRad=$towerRadius-$ds*.1; 
     $innerRad=0.;
     $nTheta=31; $nr = 7; $nz = 21;
     $nTheta $nz $nr 
    bounds on the radial variable
     $innerRad $outerRad
    bounds on the axial variable
      # ground level is at -.5 compared to the base of the tower: 
      $zac=-1.5; $zbc=.5; 
      $zac $zbc
    centre for cylinder
      $xTowerShift $yTowerShift $zTowerShift
    boundary conditions
      -1 -1 1 1 1 1
    mappingName
     $towerBaseHoleCutterName = "towerBaseHoleCutter$numberOfTowers";
     $explicitHoleCutterNames.="\n$towerBaseHoleCutterName";
     $towerBaseHoleCutterName
    exit
