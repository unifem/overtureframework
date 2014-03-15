//
//  Petsc solvers in Overture
//
//  $Id: OgesPETScNull.C,v 1.3 1999/12/11 00:23:31 henshaw Exp $
// 

#include "Oges.h"

void Oges::
initializePetscSLES()
{
}

void Oges::
setPetscParameters() 
{
}

int Oges::
solvePETSc(realCompositeGridFunction & u,
	     realCompositeGridFunction & f)
{
  printf("Oges::solvePETSc:ERROR: PETSc not available or not linked to this program \n");
  return 1;
}


void Oges::
getCsortWorkspace(int nWorkSpace00)
{
}

 
//..Build Petsc MATRIX: rescale & prealloc the matrix
void Oges::
buildPetscMatrix()
{
}

void Oges::
preallocRowStorage()
{
 
}

//..Allocate space for diag scaling, set to 1 if no scaling,
//   or 1/rownorms otherwise
void Oges::
computeDiagScaling()
{
}

// ..Build PETSC rhs and solution vector
//
void Oges::
buildRhsAndSolVector(realCompositeGridFunction & u,
		     realCompositeGridFunction & f)
{
}

