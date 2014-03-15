#! /bin/csh -f
#

echo "Copy Oges files into the Overture.g/Oges directory..."

set OvertureOges    = "/home/henshaw/Overture.g/Oges"
set OvertureInclude = "/home/henshaw/Overture.g/include"
set OvertureTests = "/home/henshaw/Overture.g/tests"

cp {Oges.h,OgesParameters.h} $OvertureInclude

cp {EquationSolver,PETScEquationSolver,YaleEquationSolver,SlapEquationSolver,HarwellEquationSolver}.h $OvertureInclude
cp {MultigridEquationSolver}.h $OvertureInclude

cp {Oges,oges,OgesParameters,formMatrix,generateMatrix,buildEquationSolvers,determineErrors,obsolete}.C  $OvertureOges
cp {EquationSolver,PETScEquationSolver,YaleEquationSolver,SlapEquationSolver,HarwellEquationSolver}.C    $OvertureOges
cp {MultigridEquationSolver}.C   $OvertureOges

cp {cgesl1234,slapFiles,drmach}.F                                                     $OvertureOges

# cp {blas2,cgesbpc,cgesnull,cgesrd,cgessra,linpack}.f                                $OvertureOges
cp {blas2,cgesnull,linpack,csort}.f                                                   $OvertureOges

cp {sbcg,scgs,sgmres,slapcg,slaputil,smset,smvops,spsor,xersla}.f                     $OvertureOges
cp {dbcg,dcgs,dgmres,dlapcg,dlaputil,dmset,dmvops}.f                                  $OvertureOges

cp {ma28a,ma28dd}.f                                                                   $OvertureOges
cp yalesp.F                                                                           $OvertureOges

cp toges.C   $OvertureTests

echo "done"