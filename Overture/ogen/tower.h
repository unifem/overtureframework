# 
  nurbs 
    interpolate from mapping with options
      tower
      parameterize by index (uniform)
    done
    rotate
      -90 1
      0 0 0
    shift 
      $xTowerShift $yTowerShift $zTowerShift
    share 
      $towerShare++;
      $groundShare 0 0 0 $towerShare 0 
    mappingName
     $numberOfTowers++;
     $towerName="tower$numberOfTowers";
     $gridNames.="\n$towerName"; 
     $towerName
    exit
# 
  nurbs 
    interpolate from mapping with options
      towerCap
      parameterize by index (uniform)
    done
    rotate
      -90 1
      0 0 0
    shift 
      $xTowerShift $yTowerShift $zTowerShift
    share 
      0 0 0 0 $towerShare 0 
    mappingName
     $towerCapName="towerCap$numberOfTowers";
     $gridNames.="\n$towerCapName"; 
     $towerCapName
    exit
