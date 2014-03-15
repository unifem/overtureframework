#! /bin/csh -f
#

echo "Copy Mapping files into the Overture.g directory..."

# here is where the library is
set Overture        = "/home/henshaw/Overture.g"
set OvertureMapping = "/home/henshaw/Overture.g/Mapping"
set OvertureInclude = "/home/henshaw/Overture.g/include"
set hypgen          = "/home/henshaw/hypgen/chimera/include"
set OvertureHypgen  = "/home/henshaw/Overture.g/Hypgen"

# these files must go in the static library, so put them in the static directory:
cp {initStaticMappingVariables}.C                                                         $Overture/static

cp {strtch,cs,cggpoly,dpm,tspack,ingrid}.f                                                $OvertureMapping
cp {r1mach,i1mach,d1mach}.F                                                               $OvertureMapping

cp {Inverse,stencilWalk}.C                                                                $OvertureMapping
cp {ComposeMapping,MatrixMapping}.C                                                       $OvertureMapping
cp {Fraction,Bound,Mapping,checkMapping,StretchMapping,StretchedSquareMapping,MappingRC}.C   $OvertureMapping
cp {BoundingBox,SquareMapping,SphereMapping,SmoothedPolygonMapping}.C                     $OvertureMapping
cp {DataPointMapping,dpmScalar,dpmInverse,AnnulusMapping,FilletMapping}.C                 $OvertureMapping
cp {CircleMapping,NormalMapping,ReductionMapping}.C                                       $OvertureMapping
cp {MappingP}.C                                                                           $OvertureMapping
cp {CylinderMapping,PlaneMapping,IntersectionMapping,Triangle}.C                          $OvertureMapping
cp {RevolutionMapping,BoxMapping}.C                                                       $OvertureMapping
cp {OrthographicTransform,ReparameterizationTransform,CrossSectionMapping}.C              $OvertureMapping
cp {RestrictionMapping,SplineMapping,TFIMapping}.C                                        $OvertureMapping
cp {LineMapping,MatrixTransform,StretchTransform}.C                                       $OvertureMapping
cp initializeMappingList.C                                                                $OvertureMapping
cp {createMappings,viewMappings}.C                                                        $OvertureMapping
cp {AirfoilMapping,EllipticTransform,DepthMapping,JoinMapping,SweepMapping}.C             $OvertureMapping
cp {HyperbolicMapping,UnstructuredMapping}.C                                              $OvertureMapping
cp {equi.C,hyperNull.C}                                                                   $OvertureMapping
cp {NurbsMapping,IgesReader,TrimmedMapping,CompositeSurface,MappingProjectionParameters}.C $OvertureMapping
cp {readMappings.C,DataFormats.C,readMappingsFromAnOverlappingGridFile.C}                  $OvertureMapping

cp {EllipticGridGenerator,Elliptic,QuadTree}.C  $OvertureMapping 

cp sPrintF.C                                                                              $OvertureMapping

# copy include files


cp {Mapping,MappingEnums,MappingP,MappingRC,MappingWS,BoundingBox}.h                      $OvertureInclude
cp {ComposeMapping,MatrixMapping,PlaneMapping,IntersectionMapping,Triangle}.h             $OvertureInclude
cp {Annulus,Bound,CircleMapping,CylinderMapping,Fraction,Inverse,Sphere,Square,SquareMapping}.h    $OvertureInclude
cp {SmoothedPolygon,DataPointMapping,NormalMapping,RestrictionMapping}.h                  $OvertureInclude
cp {RevolutionMapping,BoxMapping,StretchMapping,ReductionMapping}.h                       $OvertureInclude
cp {OrthographicTransform,ReparameterizationTransform,CrossSectionMapping}.h              $OvertureInclude
cp MappingInformation.h                                                                   $OvertureInclude
cp {LineMapping,MatrixTransform,StretchedSquare,StretchTransform}.h                       $OvertureInclude
cp {SplineMapping,TFIMapping,FilletMapping}.h                                             $OvertureInclude
cp {AirfoilMapping,EllipticTransform,DepthMapping,JoinMapping,SweepMapping}.{h}           $OvertureInclude
cp {EquiDistribute,HyperbolicMapping,DataFormats,UnstructuredMapping}.{h}                 $OvertureInclude
cp {NurbsMapping,IgesReader,TrimmedMapping,CompositeSurface,MappingProjectionParameters}.h $OvertureInclude
cp {EllipticGridGenerator,QuadTree}.h                 $OvertureInclude

echo "copy sample .cmd files into $Overture/sampleMappings"
cp {mastSail2d,hypeCan,hypeLine,hypeBump}.cmd   $Overture/sampleMappings

# echo "Copy hypgen files into $OvertureHypgen"
# cp {hyper.C,hypgen.f,hypgen2.f,surgrd.F,cmpltm.F,cmpltm.h}                                $OvertureHypgen
# cp $hypgen/{toplgy.F,submms.F,offsur.F,error.F,errcom.h,nrstpt.F,cellpr.F,sfuns.F}        $OvertureHypgen
# cp $hypgen/{rsttop.F,surnor.F,cmemc.c,precis.h,forttype.h,chimera_dimens.h,fortcall.h}    $OvertureHypgen
# cp $hypgen/{ncrspr.F,cellpx.F,normal.F,trim.F,inv3x3.F,ssvdc.F,zero.F,copy.F,intsec.F}    $OvertureHypgen


exit
