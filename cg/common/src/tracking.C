#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "Ogen.h"

// int 
// newAdaptiveGridBuilt(CompositeGrid & cg, realCompositeGridFunction & u, Parameters & parameters,
//                      bool updateSolution);

// int 
// getAmrErrorFunction(realCompositeGridFunction & u, 
//                     real t,
// 		    Parameters & parameters,
//                     realCompositeGridFunction & error,
//                     bool computeOnFinestLevel =false   );


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{tracking}}  
int DomainSolver::
tracking( GridFunction & gf0, int stepNumber  )
// =========================================================================================
// /Description:
//
// /gf (input) : current grid function (see below for details)
//
//\end{CompositeGridSolverInclude.tex} 
// =========================================================================================
{

//  realCompositeGridFunction & u = gf.u; // current solution
//  CompositeGrid & cg = gf.cg;           // current grid
//  real time = gf.t;                     // current time
  
  GridFunction & solution = gf[current];

  int debugt=0;
  printf("****tracking: stepNumber=%i, trackingFrequency=%i\n",stepNumber, parameters.dbase.get<int >("trackingFrequency"));
  if( stepNumber>1 && ((stepNumber % parameters.dbase.get<int >("trackingFrequency")) == 0) )
  {
    printf("****tracking: define a new grid at stepNumber=%i\n",stepNumber);
    printf("****tracking:start: solution.form=%i, gf0.form = %i\n",solution.form,gf0.form);

    CompositeGrid & cg = gf0.cg;
    
    GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
    GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

    if( false )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);
      ps.erase();
      psp.set(GI_TOP_LABEL,"solution.u BEFORE TRACKING");
      PlotIt::contour(ps,solution.u,psp);
      ps.erase();
      psp.set(GI_TOP_LABEL,"gf0.u BEFORE TRACKING");
      PlotIt::contour(ps,gf0.u,psp);

    }

    // keep track of boundaries on new grids that should have the same BC as an old grid
    // sharedBoundaryCondition(side,axis,grid) = side2+2*(axis2+3*grid2) : match to (side2,axis2,grid2)
    IntegerArray sharedBoundaryCondition(2,3,cg.numberOfGrids()+20);
    sharedBoundaryCondition=-1;

    Mapping *newMapping=NULL;
    int newNumberOfGrids=cg.numberOfBaseGrids();
    userDefinedGrid( solution, newMapping, newNumberOfGrids, sharedBoundaryCondition  );

    if( newMapping==NULL )
    {
      printf("tracking: ---- NO new grid was added ---- \n");
      return 1;
    }
    

    bool useNewWay=true;

    int mappingWasAdded=false;
    int gridChanged=false;
    int newGrid=-1;

    int numberOfChanges=0;
    IntegerArray changes(2,cg.numberOfGrids()+20);
    changes=-1;

    
//    CompositeGrid cgNew=cg;     // this is a waste if no changes are made.

    // **** define the new grid from the first two grids *****

    // CompositeGrid cgNew(cg.numberOfDimensions(),1);  // make a CompositeGrid with 1 component grid
     CompositeGrid cgNew;
     for( int grid=0; grid<cg.numberOfBaseGrids()-1; grid++ )
     {
       MappedGrid mg;
       mg=cg[grid];   // we need to make a copy since the mask will be changed
       cgNew.add(mg);
     }
     
//      cgNew[0].reference(cg[0]);
//      cgNew.updateReferences();
//      cgNew.add(cg[1]);
//      cgNew.add(cg[2]);

     cgNew.rcData->interpolant=cg.getInterpolant();   // **** fix this ***

     // set the interpolationWidth etc. based on cg
     cgNew.setOverlapParameters(cg);
     
//      cgNew.interpolationWidth=3; // cg.interpolationWidth;
//      cgNew.interpolationOverlap=.5; // cg.interpolationOverlap;
//      cgNew.interpolationIsImplicit=true; // cg.interpolationIsImplicit;
     

//      // we even add the last base grid so we can delete it and interpolate underneath -- fix this --
//      for( int grid=1; grid<cg.numberOfBaseGrids(); grid++ )
//        cgNew.add(cg[grid]);       // add this grid 


//     if( parameters.dbase.get<Ogen* >("gridGenerator")==NULL )
//        parameters.dbase.get<Ogen* >("gridGenerator") = new Ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));
//     parameters.dbase.get<Ogen* >("gridGenerator")->debug=1;
//     parameters.dbase.get<Ogen* >("gridGenerator")->updateOverlap(cgNew);
//     ps.plot(cgNew);
    
    newNumberOfGrids=cgNew.numberOfGrids();
    if( false && useNewWay )
      newNumberOfGrids++;
    

//    userDefinedGrid( solution, newMapping, newNumberOfGrids, sharedBoundaryCondition  );

    if( newMapping!=NULL )
    {
      mappingWasAdded=true;
      newGrid=newNumberOfGrids; newNumberOfGrids++;
      
      changes(0,numberOfChanges)=newGrid;
      changes(1,numberOfChanges)=gridWasAdded;
      numberOfChanges++;

      if( mappingWasAdded )
      {
	mappingWasAdded=FALSE;
	int i=numberOfChanges-1;
	assert( changes(1,i)==gridWasAdded || changes(1,i)==refinementWasAdded );
	assert( newMapping!=NULL );
	cgNew.add( *newMapping);    // Add a new component grid, built from this Mapping.
	newMapping->decrementReferenceCount();
	newMapping=NULL;
      }


      // **** delete the highest numbered base grid. *****
      if( true ) // we need to do this even if we don't delete so we interpolated exposed --- fix ******
      {
	int grid=cg.numberOfBaseGrids()-1;
	if( grid>0 )
	{
	  gridChanged=true;
	  newGrid=grid;

	  changes(0,numberOfChanges)=newGrid;
	  changes(1,numberOfChanges)=gridWasRemoved;
	  numberOfChanges++;
	}
      }
      

    }      

    if( numberOfChanges>0 )
    {
    
      const int oldNumberOfGrids=cg.numberOfGrids();

      int i;
      for( i=0; i<numberOfChanges; i++ )
      {
	if( changes(1,i)==gridWasAdded || changes(1,i)==refinementWasAdded )
	{
	  // this was dealt with above
	}
	else if( changes(1,i)==gridWasRemoved || changes(1,i)==refinementWasRemoved  )
	{
	  if( !useNewWay ) 
            cgNew.deleteGrid( changes(0,i) );

	  printf("After deleteGrid: numberOfComponentGrids=%i, numberOfGrids=%i\n",
		 cgNew.numberOfComponentGrids(),cgNew.numberOfGrids());
      
	}
	else if( changes(1,i)==gridWasChanged )
	{
	}
	else
	{
	  printf("addGrids:ERROR: unknown change! \n");
	  Overture::abort("error");
	}
      }
      
      if( parameters.dbase.get<Ogen* >("gridGenerator")==NULL )
	parameters.dbase.get<Ogen* >("gridGenerator") = new Ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));
      assert( parameters.dbase.get<Ogen* >("gridGenerator")!=NULL );
      
      // *************** build the new overlapping grid *********************
      // parameters.dbase.get<Ogen* >("gridGenerator")->debug=3;
      
      parameters.dbase.get<Ogen* >("gridGenerator")->updateOverlap(cgNew);
      
      if( false )
      {
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	ps.erase();
	psp.set(GI_TOP_LABEL,"Grid after updateOverlap");  
	PlotIt::plot(ps,cgNew,psp);
      
      }
    
      updateToMatchNewGrid(cgNew,changes,sharedBoundaryCondition,solution);

      if( false )
      {
	ps.erase();
	psp.set(GI_TOP_LABEL,"solution.u after updateToMatchNewGrid");  
	PlotIt::contour(ps,solution.u,psp);

	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	ps.erase();
	psp.set(GI_TOP_LABEL,"solution.cg : Grid after updateOverlap");  
	PlotIt::plot(ps,solution.cg,psp);
      }
    

      if( parameters.dbase.get<bool >("adaptiveGridProblem") )
      {
	printf("*********tracking: Regrid for AMR *************\n");

        int level=0;
        
	if( true )
	{
	  realCompositeGridFunction error(solution.cg);
	  getAmrErrorFunction(solution.u,0.,error);

          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
          psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);
	  ps.erase();
	  psp.set(GI_TOP_LABEL,"solution.u BEFORE new adaptive grid");
	  PlotIt::contour(ps,solution.u,psp);
	  ps.erase();
	  psp.set(GI_TOP_LABEL,"gf0.u BEFORE new adaptive grid");
	  PlotIt::contour(ps,gf0.u,psp);

	  ps.erase();
	  psp.set(GI_TOP_LABEL,"AMR error function");
	  PlotIt::contour(ps,error,psp);
	}
	  
        while( newAdaptiveGridBuilt(cgNew,solution.u,false) )
	{
           
	  level++;
	  if( debugt & 2 )
	  {
	    ps.erase();
	    psp.set(GI_TOP_LABEL,"New adaptive grid");
	    PlotIt::plot(ps,cgNew,psp);


	  }
      

	  numberOfChanges=0;
	  changes=-1;

          GridCollection & cgr = cgNew.refinementLevel[level];
          for( int grid=0; grid<cgr.numberOfGrids(); grid++ )
	  {
            printf(" level=%i: gridNumber=%i was added\n",level,cgr.gridNumber(grid));
	    
	    changes(0,numberOfChanges)=cgr.gridNumber(grid);
	    changes(1,numberOfChanges)=refinementWasAdded;
	    numberOfChanges++;
	  }

          updateToMatchNewGrid(cgNew,changes,sharedBoundaryCondition,solution);

          printf("****After new AMR grid: solution.form=%i, gf0.form = %i\n",solution.form,gf0.form);
	  
	  if( debugt & 2 )
	  {
            ps.erase();
	    psp.set(GI_TOP_LABEL,"solution.u on new adaptive grid");
	    PlotIt::contour(ps,solution.u,psp);
            ps.erase();
	    psp.set(GI_TOP_LABEL,"gf0.u on new adaptive grid");
	    PlotIt::contour(ps,gf0.u,psp);

	  }

	}
      }

      parameters.dbase.get<int >("saveGridInShowFile")=true;  // we need to save the grid in the show file if it has changed.
    }

  }

  return 0;
}

