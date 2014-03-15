#! /bin/csh -f
#

# here is where the Overture Grid Function files are:
set Overture = "/home/henshaw/Overture.g"
set grid = $Overture/Grid
set include = $Overture/include

echo "Copy grid files into the Overture.g directory: $grid ..."


cp {GenericGrid,MappedGrid,MappedGridGeometry1,MappedGridGeometry2}.C $grid
cp {GenericGridCollection,GridCollection,UnstructuredGridGeometry}.C                     $grid
cp {CompositeGrid,CompositeGridGeometry}.C                                               $grid
cp {ReferenceCounting}.C                                                                 $grid


cp {GenericGrid,MappedGrid,GenericGridCollection,GridCollection,CompositeGrid}.h         $include
cp {ReferenceCounting}.h                                                                 $include

echo "done"
exit
