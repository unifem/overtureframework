#! /bin/csh -f
#

echo "Copy xCog conversion files into the Overture.g..."

set OvertureX  = "/home/henshaw/Overture.g/otherStuff"

cp {xCogToOverture.C,hdf_stuff.h,c_array.h,real.h,stupid_compiler.h} $OvertureX
# turn these into C++ :
cp c_array.c                                        $OvertureX/c_array.C
cp hdf_stuff.c                                      $OvertureX/hdf_stuff.C

exit

