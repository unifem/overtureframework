#include "DomainSolver.h"
#include "GridFunction.h"
#include "PlotStuff.h"
#include "RigidBodyMotion.h"

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{userDefinedOutput}}  
int DomainSolver::
userDefinedOutput( GridFunction & gf, int stepNumber  )
// =========================================================================================
// /Description:
//   This routine is called every time step to give the user a chance to output
//  any information that he or she desires. 
//
//  NOTE: You must turn on the toggle 'allow user defined output' 
//        in the output-options dialog to have this routine called. 
//
// /gf (input) : current grid function (see below for details)
//
//\end{CompositeGridSolverInclude.tex} 
// =========================================================================================
{

  realCompositeGridFunction & u = gf.u; // current solution
  CompositeGrid & cg = gf.cg;           // current grid
  real time = gf.t;                     // current time
  

  // **********************************************************
  // *** The following is an example of saving information ****
  // **********************************************************

  // Make a sub-directory in the data-base to store variables used here
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedOutputData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("userDefinedOutputData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedOutputData");
  if( !db.has_key("fileNumber") )
  {
    db.put<int>("fileNumber",0);
    db.put<real>("tSave",0.);
  }
  int & fileNumber = db.get<int>("fileNumber");  // keep a count of different file numbers
  real & tSave     = db.get<real>("tSave");

  //  if( (stepNumber  % 2 ) == 0 )
  if( time >= tSave-parameters.dbase.get<real >("dt")*.5 ) // within half a time step of next time to save.
  {
    tSave+=parameters.dbase.get<real >("tPrint");   // next time to save.
    
    // output a solution on a grid line
    aString fileName;
    sPrintF(fileName,"myFile%i.dat",fileNumber);
    fileNumber++;
    

    FILE *output = fopen((const char*)fileName,"w" );      // Here is the output file.

    int direction=0; // line varies along this axis
    int iv[3]={0,0,0};    //  choose index values in the other two directions


    int numberOfComponents=parameters.dbase.get<int >("numberOfComponents");

    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      mg.update(MappedGrid::THEvertex);  // make sure the vertex array is built

      realMappedGridFunction & ug = u[grid];
      intArray & mask = mg.mask();
      realArray & vertex = mg.vertex();
    
      // displayMask(mask);

      int refinementLevelNumber = cg.refinementLevelNumber(grid);
      int refinementRatio = cg.refinementFactor(0,grid);
    
      bool ok=true;
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	if( axis!=direction && 
	    (mg.gridIndexRange(Start,axis) > iv[axis]*refinementRatio || 
               iv[axis]*refinementRatio > mg.gridIndexRange(End,axis) ) )
	{
	  ok=false;
	  break;
	}
      }
      if( ok )
      {
	int i1Start = direction==0 ? mg.gridIndexRange(Start,axis1) : iv[0]*refinementRatio;
	int i1End   = direction==0 ? mg.gridIndexRange(End  ,axis1) : iv[0]*refinementRatio;
      
	int i2Start = direction==1 ? mg.gridIndexRange(Start,axis2) : iv[1]*refinementRatio;
	int i2End   = direction==1 ? mg.gridIndexRange(End  ,axis2) : iv[1]*refinementRatio;
      
	int i3Start = direction==2 ? mg.gridIndexRange(Start,axis3) : iv[2]*refinementRatio;
	int i3End   = direction==2 ? mg.gridIndexRange(End  ,axis3) : iv[2]*refinementRatio;
      
	for( int i3=i3Start; i3<=i3End; i3++ )
	{
	  for( int i2=i2Start; i2<=i2End; i2++ )
	  {
	    for( int i1=i1Start; i1<=i1End; i1++ )
	    {
	
               // flag=0 outside, 1=inside
              int amrFlag=(mask(i1,i2,i3) & MappedGrid::IShiddenByRefinement)!=0;
              int flag = mask(i1,i2,i3)!=0 && !amrFlag;
	      
	      fprintf(output," grid=%i i1=%i i2=%i %e %e %i %i ",grid,i1,i2,
                               vertex(i1,i2,i3,0),vertex(i1,i2,i3,1),flag,amrFlag);
	      int n=0;
              for( n=0; n<numberOfComponents; n++ )
	      {
		fprintf(output," %e",ug(i1,i2,i3,n));
	      }
	      fprintf(output,"\n");

	    }
	  }
	}
      }
    }
    

    // Here we show how to access the info about moving rigid bodies
    RealArray xCM(3),vCM(3);

    const int numberOfRigidBodies = parameters.dbase.get<MovingGrids >("movingGrids").getNumberOfRigidBodies();
    for( int bodyNumber=0; bodyNumber<numberOfRigidBodies; bodyNumber++ )
    {
      // here is the object that knows about a rigid body:
      RigidBodyMotion & body =  parameters.dbase.get<MovingGrids >("movingGrids").getRigidBody(bodyNumber);
      
      body.getPosition( time,xCM ); // position of the centre of mass
      body.getVelocity( time,vCM ); // velocity of the centre of mass
      
      real mass = body.getMass();
      real density=body.getDensity();   // this only works if you have set the density
      // you could compute the radius of a cylinder from the mass and density

      printf("userDefinedOutput: rigid body %i is located at (%5.3f,%5.3f,%5.3f), "
	     "velocity=(%5.3f,%5.3f,%5.3f)\n",bodyNumber,xCM(0),xCM(1),xCM(2),vCM(0),vCM(1),vCM(2));
      

    }
    


      fclose(output);
    
  }


  return 0;
}

