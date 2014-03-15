#! /bin/csh -f
#

# here is where the Overture Grid Function files are:
set gff = "/users/henshaw/Overture.g/templates"
set gfi = "/users/henshaw/Overture.g/include"

echo "Copy template files Overture.g directory: $gff ..."


cp ListOf{Float,Int,Double}{Array,SerialArray,DistributedArray,GenericGridFunction,MappedGridFunction}.C  $gff
cp ListOf{Float,Int,Double}{GridCollectionFunction,CompositeGridFunction}.C                               $gff
cp ListOfListOf{Float,Int,Double}{Array}.C                                                                $gff
cp ListOf{GenericGrid,MappedGrid,GenericGridCollection,GridCollection,CompositeGrid}.C                    $gff
cp ListOf{MappingRC}.C                                                                                    $gff
cp ListOf{BoundingBox,MappedGridOperators,CompositeGridOperators,MappingRC}.C                             $gff
cp ListOf{GenericMappedGridOperators,GenericGridCollectionOperators,GenericCompositeGridOperators}.C      $gff

echo "now copy the include files to $gfi"


cp ListOfReferenceCountedObjects.{h,C}                                                                    $gfi
cp tlist.{h,C}                                                                                            $gfi
cp ListOf{Float,Int,Double}{Array,SerialArray,DistributedArray,GenericGridFunction,MappedGridFunction}.h  $gfi
cp ListOf{Float,Int,Double}{GridCollectionFunction,CompositeGridFunction}.{h}                             $gfi
cp ListOfListOf{Float,Int,Double}{Array}.h                                                                $gfi
cp ListOfListOf{Real}{Array}.h                                                                            $gfi
cp ListOf{GenericGrid,MappedGrid,GenericGridCollection,GridCollection,CompositeGrid}.h                    $gfi
cp ListOf{MappingRC}.h                                                                                    $gfi
cp ListOf{BoundingBox,MappedGridOperators,CompositeGridOperators,MappingRC}.h                             $gfi
cp ListOf{GenericMappedGridOperators,GenericGridCollectionOperators,GenericCompositeGridOperators}.h      $gfi
