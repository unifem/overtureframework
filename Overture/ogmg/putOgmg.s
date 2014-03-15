#! /bin/csh -f
#

echo "Copy Ogmg.C and related files into the Overture.g..."

# here is where the original files are:
set Overture  = "/home/henshaw/Overture.g"

set OvertureOgmg    = $Overture/Ogmg
set OvertureInclude = $Overture/include

cp {Ogmg,smooth,defect,fineToCoarse,coarseToFine,ogmgTests,ogmgUtil}.C $OvertureOgmg
cp {lineSmooth,boundaryConditions,OgmgParameters,operatorAveraging}.C  $OvertureOgmg
cp {buildExtraLevels,checkGrid}.C  $OvertureOgmg
cp {Ogmg,OgmgParameters}.h                                             $OvertureInclude

# should cp to the bin:
cp ogmgt.C                                                             $OvertureOgmg 
