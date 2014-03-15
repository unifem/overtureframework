#! /bin/csh -f
#

set Overture = "/home/henshaw/Overture.g"

set os = "$Overture/otherStuff"
set include = "$Overture/include"
set examples = "$Overture/examples"
set tests = "$Overture/tests"

echo "Copy otherStuff files into the Overture.g directory..."



cp {OGgetIndex,OGFunction,OGTrigFunction,OGPolyFunction,Integrate,TridiagonalSolver}.C    $os
cp {getFromADataBase,displayMask}.C                                                       $os
cp {floatDisplay.C,intDisplay.C,doubleDisplay.C,displayMask.C,DisplayParameters.C}        $os
cp {FortranIO.C,fortranIO.f}                                                              $os
cp Stencil.C                                                                              $os

cp {OGgetIndex,OGFunction,OGTrigFunction,OGPolyFunction,Integrate,TridiagonalSolver}.h $include
cp {display,DisplayParameters,DataBaseAccessFunctions,FortranIO}.h                     $include
cp Stencil.h                                                                           $include

cp {tz}.{C,check}     $tests
cp {trid}.{C}         $tests

cp {ReferenceCountedClass}.{h,C} $examples
