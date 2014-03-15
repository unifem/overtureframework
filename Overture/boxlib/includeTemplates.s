#! /bin/csh -f
#

# Copy the template .C files to the include directory so that they will be found when compiling

echo "Copy the template .C files to the include directory so that they will be found"

# leave off List.C ???

cp {AliasedDPtr,ArithFab,Array,BaseFab,DPtr,FabArray,NormedFab,OrderedFab,PArray,Pointers,SimpleDPtr,Tuple}.C ../include

echo "done"
