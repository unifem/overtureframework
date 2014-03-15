* Choose an initial mapping.
* In this case it is a diamond
* airfoil
* create mappings
  Airfoil
   airfoil type
     diamond
  exit
* Now we choose a transformation
* to apply on the mapping, in this
* case it is the elliptic transform
elliptic
  project onto original mapping (toggle)
  change resolution for elliptic grid
  33 17
  set GRID boundary conditions
  1 1 1 1
* 1 is used for dirichlet boundary condition
* 2 for orthogonal boundary condition
* 3 for combined boundary condition
*-1 for periodic boundary condition
* The order of the boundary is:
* iside=0, iaxis=0; iside=1, iaxis=0;
* iside=0, iaxis=1; and iside=1, iaxis=1.
* Need to input the successive boundary 
* thickness for  each combined boundary 
* condition
* 0.02
* 0.02
* Now start the smoothing process
  elliptic smoothing
       Line Solver
    maximum number of V-cycles
      10
   *source interpolation power
   *  6
   * start smoothing
