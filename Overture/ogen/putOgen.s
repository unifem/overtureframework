#! /bin/csh -f
#

echo "Copy ogen.C and related files into the Overture.g..."

# here is where the original files are:
set Overture  = "/home/henshaw/Overture.g"
set OvertureOgshow  = "/home/henshaw/Overture.g/Ogshow"
set OvertureBin     = "/home/henshaw/Overture.g/bin"
set OvertureInclude = "/home/henshaw/Overture.g/include"
set OverturePrimer  = "/home/henshaw/Overture.g/primer"
set sampleGrids     = "/home/henshaw/Overture.g/sampleGrids"
set OvertureTests   = "/home/henshaw/Overture.g/tests"

cp {ogenDriver,ogenFunction}.C         $OvertureBin
# cp getFromADataBase.C          $OvertureOgshow

# cp DataBaseAccessFunctions.h   $OvertureInclude

# Here are files for Bill's grid generator
cp {Ogen,check,checkOverlap,changeParameters,classify,computeOverlap,cutHoles,buildCutout}.C $Overture/GridGenerator
cp {boundaryAdjustment,improveQuality}.C                                                     $Overture/GridGenerator
cp {Ogen.h}                                              $OvertureInclude

cp {checkOverlappingGrid.C}                                                 $OvertureOgshow
cp prtpeg.f                                                                 $Overture/otherStuff

cp generate.p                                                                          $sampleGrids

cp {square5,square5CC,square10,square20,square40,cic,cicCC,cic.4,cicmg,cilc}.cmd   $sampleGrids
cp {valve,valveCC,obstacle,inletOutlet,inletOutletCC,edgeRefinement,naca0012}.cmd  $sampleGrids
cp {mismatch,mismatchAnnulus,end,filletThree,stir,stirSplit,twoBump}.cmd           $sampleGrids
cp {sic,stretchedAnnulus,stretchedCube,box5,box10,box20,box40}.cmd                 $sampleGrids
cp {valve3d,valve3dCC,sphereInATube,tse,valvePort}.cmd                             $sampleGrids
cp {revolve,revolveCC,pipes,pipesCC,sib,sibCC,ellipsoid,ellipsoidCC}.cmd           $sampleGrids
cp {singularSphere,filletTwoCyl,joinTwoCyl,mastSail2d,naca.hype}.cmd               $sampleGrids
cp {halfCylinder}.cmd                                                              $sampleGrids

cp {move1.C,amrExample1.C}                                              $OverturePrimer
cp {move1.C,amrExample1.C}                                              ../primer

cp {move2.C,moveAndSolve.C}                                             $OvertureTests

echo "**** remember to copy the check files if they have changed ******"
