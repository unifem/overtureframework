#include "Overture.h"
#include "MappedGridOperators.h"
#include "SquareMapping.h"
#include "display.h"
#include "ParallelUtility.h"

//================================================================================
//  Examples showing how to apply boundary conditions to MappedGridFunction's
//================================================================================

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
 
  printf(" ---------------------------------------------------- \n");
  printf(" Demonstrate the application of Boundary Conditions   \n");
  printf("            to a MappedGridFunction                   \n");
  printf(" ---------------------------------------------------- \n");

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,5);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,5);                  // axis2==1, set no. of grid points
  
  MappedGrid mg(square);                               // MappedGrid for a square

  // add extra ghost lines since we show how to extrapolate ghost line 2 below
  int side,axis;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
    for( side=Start; side<=End; side++ )
      mg.numberOfGhostPoints()(side,axis)=2;
  mg.update();

  realArray & vertex = mg.vertex();
  
  // define some higher level boundary conditions, put these into the MappedGrid
  const int inflow=1, outflow=2, wall=3;
  mg.boundaryCondition()(0,0)=inflow;
  mg.boundaryCondition()(1,0)=outflow;
  mg.boundaryCondition()(0,1)=wall;
  mg.boundaryCondition()(1,1)=wall;
  mg.boundaryCondition().display("Here is mg.boundaryCondition()");

  Index I1,I2,I3, Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
  Range all;
  realMappedGridFunction u(mg),v(mg,all,all,all,3),w(mg);   // define some component grid functions

  MappedGridOperators operators(mg);                     // define some differential operators
  u.setOperators( operators );                           // Tell u which operators to use

  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
  realSerialArray wLocal; getLocalArrayWithGhostBoundaries(w,wLocal);
  realSerialArray xLocal; getLocalArrayWithGhostBoundaries(vertex,xLocal);


  u=-77.;
  getIndex(mg.indexRange(),I1,I2,I3);                                  // assign I1,I2,I3

  int includeGhost=1;
  bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  if( ok )
    uLocal(I1,I2,I3)=xLocal(I1,I2,I3,axis1)+xLocal(I1,I2,I3,axis2);   // u=x+y on interior + boundary points

  display(u,"Here is u before assigning BCs");

  // make some shorter names for readability
  BCTypes::BCNames dirichlet       = BCTypes::dirichlet,
                   neumann         = BCTypes::neumann,
                   extrapolate     = BCTypes::extrapolate,
                   normalComponent = BCTypes::normalComponent,
                   aDotU           = BCTypes::aDotU,
                   aDotGradU       = BCTypes::aDotGradU,
                   allBoundaries   = BCTypes::allBoundaries; 

  // ****************************************************************
  //     First we apply Dirchlet boundary conditions to all sides 
  // ****************************************************************
  int component=0;
  u.applyBoundaryCondition(component,dirichlet,allBoundaries,0.);
  u.finishBoundaryConditions();
  display(u,"Here is u after assigning BCs");

  uLocal=-77.;
  if( ok )
    uLocal(I1,I2,I3)=xLocal(I1,I2,I3,axis1)+xLocal(I1,I2,I3,axis2);   // u=x+y on interior + boundary points
  
  // ****************************************************************
  //    apply different BC's to different sides
  // ****************************************************************
  u.applyBoundaryCondition(component,dirichlet,  inflow,1.);     // u=1
  u.applyBoundaryCondition(component,neumann,    outflow,0.);    // u.n=0.
  u.applyBoundaryCondition(component,extrapolate,wall,0.);
  u.finishBoundaryConditions();
  display(u,"Here is u after assigning BCs (1=D,2=N,3=E)");
   
  uLocal=-77.;
  if( ok )
    uLocal(I1,I2,I3)=xLocal(I1,I2,I3,axis1)+xLocal(I1,I2,I3,axis2);   // u=x+y on interior + boundary points

  // **********************************************************************
  //   extrapolate ghost line 1
  // **********************************************************************
  u.applyBoundaryCondition(component,extrapolate,allBoundaries,0.,0.);
  display(u,"Here is u after extrapolate ghost line 1 on all boundaries");

  // **********************************************************************
  //   extrapolate ghost line 2 to 5'th order
  // **********************************************************************
  BoundaryConditionParameters extrapParams;
  extrapParams.ghostLineToAssign=2;
  extrapParams.orderOfExtrapolation=5;
  u.applyBoundaryCondition(component,extrapolate,allBoundaries,0.,0.,extrapParams);
  display(u,"Here is u after extrapolate ghost line 2 to order 5 on all boundaries");


  uLocal=-77.;
  if( ok )
    uLocal(I1,I2,I3)=xLocal(I1,I2,I3,axis1)+xLocal(I1,I2,I3,axis2);   // u=x+y on interior + boundary points
  // ***********************************************************************
  //   Assign BC's with variable RHS
  // ***********************************************************************
  // The RHS for the BC is saved in the grid function w
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); // Assign Index values for this boundary
      bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
      if( !ok ) continue;
      if( axis==0 )
        wLocal(Ib1,Ib2,Ib3)=sin(Pi*xLocal(Ib1,Ib2,Ib3,axis2));   // Set u=sin(pi y) on axis=0, side=0,1
      else
        wLocal(Ib1,Ib2,Ib3)=cos(Pi*xLocal(Ib1,Ib2,Ib3,axis1));   // set u=cos(pi x) on axi==1, side=0,1
    }
  }
  u.applyBoundaryCondition(component,dirichlet,allBoundaries,w,0.);
  display(u,"Here is u with u=sin(pi y) (left/right) and u=cos(pi x) (bottom/top)");


  // ***********************************************************************
  //   apply some boundary conditions on a vector grid function
  // ***********************************************************************
  v.setOperators( operators );                 
  if( ok )
  {
    vLocal=-88.;
    vLocal(I1,I2,I3,0)=xLocal(I1,I2,I3,axis1)+xLocal(I1,I2,I3,axis2);  
    vLocal(I1,I2,I3,1)=xLocal(I1,I2,I3,axis1)-xLocal(I1,I2,I3,axis2);  
    vLocal(I1,I2,I3,2)=1.;
  }
  
  v.applyBoundaryCondition(Range(0,2),dirichlet,allBoundaries,2.);  
  display(v,"Here is v after v=2 on boundary");

  Range C(0,1);
  v.applyBoundaryCondition(C,normalComponent,inflow,0.);   // n.(u_0,u_1)=0.



  // **********************************************************************
  // apply BC: aDotU : use BoundaryConditionParameters to specify "a"
  // **********************************************************************
  BoundaryConditionParameters aDotUParams;
  aDotUParams.a.redim(3);
  aDotUParams.a(0)=1.;
  aDotUParams.a(1)=2.;
#ifndef USE_PPP
  // fix this for parallel
  v.applyBoundaryCondition(C,aDotU,inflow,0.,0.,aDotUParams);   // a.(u_0,u_1)=0.
  display(v,"Here is v after aDotU: (1,2).u  on inflow");
#endif


  // *************************************************************************
  // Apply a "masked" boundary condition -- only apply the BC to some points
  // *************************************************************************

  BoundaryConditionParameters bcParams;
  intArray & mask = bcParams.mask();
  mask.partition(mg.mask().getPartition());
  mask.redim(mg.mask());

  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

  if( ok )
    uLocal=-77;
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);  // boundary 
      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);     // ghost points 

      bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
      ok= ok && ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
      if( !ok ) continue;

      // build a mask to apply a BC on the left side for |y-.5| < .3
      // The mask values are stored on the ghost line
      maskLocal(Ig1,Ig2,Ig3) = fabs(xLocal(Ib1,Ib2,Ib3,axis2)-.5) < .3;
    }
  }
  
 #ifndef USE_PPP
  // this needs to be fixed for P++
  component=0;
  bcParams.setUseMask(TRUE);
  u.applyBoundaryCondition(component,dirichlet,inflow,0.,0.,bcParams); 
  bcParams.setUseMask(FALSE);
  mask.redim(0);
  display(u,"Here is u after 'masked' BC -- assign u=0 on inflow where |y-.5|<.3 ");

 #else

  // Here we show how to manually do a masked BC  -- this is not exactly the same
  // as the above statement since we also show how to assign some points on the boundary
  // and extrapolate other ghost point values

  int is[3]={0,0,0};//
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);  // boundary 
      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);     // ghost points 

      bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
      ok= ok && ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
      if( !ok ) continue;

      int is[3]={0,0,0};//
      is[axis]=1-2*side;
      where( fabs(xLocal(Ib1,Ib2,Ib3,axis2)-.5) < .3 )
      {
	uLocal(Ib1,Ib2,Ib3)=0.;
      }
      otherwise()
      { // extrapolate ghost points where |y-.5|>= .3 : (this is a demo for Philip) 
	uLocal(Ig1,Ig2,Ig3)=2.*uLocal(Ib1,Ib2,Ib3)-uLocal(Ib1+is[0],Ib2+is[1],Ib3+is[2]);
      }
      

    }
  }
  display(u,"Here is u after 'masked' BC -- assign u=0 where |y-.5|<.3 and extrap ghost otherwise ");
  


 #endif

  Overture::finish();          
  cout << "Program Terminated Normally! \n";
  return 0;

}
