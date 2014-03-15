#include "util.h"
#include "display.h"
#include "ParallelUtility.h"

void
printInfo( CompositeGrid & cg, int option /* =0 */ )
{
  char buff[100];

  printf(" cg.numberOfComponentGrids()=%i, cg.numberOfGrids()=%i, cg.numberOfBaseGrids()=%i, "
        "rl.numberOfRefinementLevels()=%i \n",
        cg.numberOfComponentGrids(),cg.numberOfGrids(),cg.numberOfBaseGrids(),cg.numberOfRefinementLevels());

  int grid;
  if( false )
  {
    display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
    for( grid=0; grid<cg.interpolationPoint.getLength(); grid++ )
    {
      display(cg.interpolationPoint[grid],sPrintF(buff,"cg.interpolationPoint[%]",grid));
    }
  }
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & bc = cg[grid].boundaryCondition();
    printf("grid=%i : bc=[%i,%i][%i,%i]\n",grid,bc(0,0),bc(1,0),bc(0,1),bc(1,1));
  }
  
  for( int l=0; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    printf("level=%i, rl.numberOfComponentGrids()=%i, rl.numberOfGrids()=%i, rl.numberOfBaseGrids()=%i, "
           "numberOfRefinementLevels=%i \n",l,
           rl.numberOfComponentGrids(),rl.numberOfGrids(),rl.numberOfBaseGrids(),rl.numberOfRefinementLevels());
    for( int g=0; g<rl.numberOfGrids(); g++ )
    {
      grid=rl.gridNumber(g);
      const IntegerArray & gid=cg[grid].gridIndexRange();
      const IntegerArray & d=cg[grid].dimension();
      printf("level=%i, g=%i, rl.gridNumber(g)=%i, rl.baseGridNumber(g)=%i, cg.baseGridNumber(%i)=%i "
             "gid=[%i,%i]x[%i,%i]x[%i,%i] d=[%i,%i]x[%i,%i]x[%i,%i]\n",
             l,g,rl.gridNumber(g),rl.baseGridNumber(g),grid,cg.baseGridNumber(grid),
             gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
             d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2));
      if( option>0 && l>0 )
      {
	displayMask(cg[grid].mask(),sPrintF(buff,"mask for grid %i\n",grid));
      }
    }
  }
}

void
printInfo( GridCollection & cg, int option /* =0  */ )
{
  char buff[100];

  printf(" cg.numberOfComponentGrids()=%i, cg.numberOfGrids()=%i, cg.numberOfBaseGrids()=%i, "
        "rl.numberOfRefinementLevels()=%i \n",
        cg.numberOfComponentGrids(),cg.numberOfGrids(),cg.numberOfBaseGrids(),cg.numberOfRefinementLevels());

  int grid;
  
  for( int l=0; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    printf("level=%i, rl.numberOfComponentGrids()=%i, rl.numberOfGrids()=%i, rl.numberOfBaseGrids()=%i, "
           "numberOfRefinementLevels=%i \n",l,
           rl.numberOfComponentGrids(),rl.numberOfGrids(),rl.numberOfBaseGrids(),rl.numberOfRefinementLevels());
    for( int g=0; g<rl.numberOfGrids(); g++ )
    {
      grid=rl.gridNumber(g);
      const IntegerArray & gid=cg[grid].gridIndexRange();
      const IntegerArray & d=cg[grid].dimension();
      printf("level=%i, g=%i, rl.gridNumber(g)=%i, rl.baseGridNumber(g)=%i, cg.baseGridNumber(%i)=%i "
             "gid=[%i,%i]x[%i,%i]x[%i,%i] d=[%i,%i]x[%i,%i]x[%i,%i]\n",
             l,g,rl.gridNumber(g),rl.baseGridNumber(g),grid,cg.baseGridNumber(grid),
             gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
             d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2));
      if( option>0 && l>0 )
      {
	displayMask(cg[grid].mask(),sPrintF(buff,"mask for grid %i\n",grid));
      }
    }
  }
}



int
getTrueSolution( realGridCollectionFunction & u, 
                 real t, 
                 RealArray & topHatCentre,
                 RealArray & topHatVelocity,
                 real topHatRadius,
                 InitialConditionEnum type /* = topHat */ )
// =======================================================================================
// /Description:
//    Define a moving pulse that is a good test for AMR.
//
// =======================================================================================
{
  GridCollection & gc = *u.getGridCollection();
  
  u = 0.;
  Index I1,I2,I3;
  
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    const realArray & x = gc[grid].vertex();
    getIndex(gc[grid].dimension(),I1,I2,I3);

    if( type==topHat )
    {
      real a = topHatVelocity(0);
      real b = gc.numberOfDimensions()>1 ? topHatVelocity(1) : 0.;
      real c = gc.numberOfDimensions()>2 ? topHatVelocity(2) : 0.;

      realArray radius0;
      real x0=topHatCentre(0)+a*t, y0=topHatCentre(1)+b*t, z0= topHatCentre(2)+c*t;
      // real x0=0.0+a*t, y0=0.0+b*t, z0=0.;
      real rad=topHatRadius;
    
      if( gc.numberOfDimensions()==2 )
        radius0=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0);
      else if( gc.numberOfDimensions()==1 )
        radius0=SQR(x(I1,I2,I3,0)-x0);
      else 
        radius0=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0)+SQR(x(I1,I2,I3,2)-z0);
      where( radius0 < SQR(rad) )
	u[grid](I1,I2,I3)=1.;
    }
  }
  return 0;
}


realArray& 
getTrueSolution(OGFunction & exact, realMappedGridFunction & u, 
		const Index & I1, const Index & I2, const Index & I3, real t)
// ===================================================================================
// /Description:
//     
// ===================================================================================
{

  MappedGrid & mg = *u.getMappedGrid();
  
  mg.update(MappedGrid::THEcenter);
	
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
  #else
    realSerialArray & uLocal = u;
    const realSerialArray & xLocal = mg.center();
  #endif

  Index J1=I1, J2=I2, J3=I3;
  bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,J1,J2,J3,1);
  if( ok )
  {
    const bool isRectangular=false;  // do this for now
    int ntd=0, nxd=0, nyd=0,nzd=0; // defines a derivative
    exact.gd( uLocal,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,J1,J2,J3,0,t);
  }
  
  return u;
}


real
checkError( realCompositeGridFunction & u, real t, 
            OGFunction & exact, 
            const aString & label,
            FILE *file /* =NULL */, 
            int debug /* =0 */,
            realGridCollectionFunction *pErr /* =NULL */ )
// ===================================================================================
// /Description:
//    Compute the error in twilight-zone flow.
// 
// debug (iput): if debug>0 print the errors to file.
// pErr (input) : if pErr!=NULL then fill in the error
//
// ===================================================================================
{
  CompositeGrid & cg = *u.getCompositeGrid();
  
  if( (false || debug & 4) && file!=NULL )
  {
    aString buff;
    u.display(sPrintF(buff,"checkError: %s : solution at time t=%9.3e",(const char*)label,t),file,"%9.2e ");
  }
  

  Index I1,I2,I3;
  real maxErrv[2];
  int gridMaxv[2];
  int numGhost=1; // =0; 
  for( int it=0; it<2; it++ )
  {
    real maxErr=0.,err;
    int gridMax=0;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realArray & ug = u[grid];
      mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter);

      if( it==0 )
        getIndex(extendedGridIndexRange(mg),I1,I2,I3); // this includes ghost points on faces with bc==0
      else
        getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost);



      realSerialArray error;
      #ifdef USE_PPP
        intSerialArray maskLocal;   getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
        realSerialArray uLocal;     getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray xLocal;     getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
	if( pErr!=NULL )
          getLocalArrayWithGhostBoundaries((*pErr)[grid],error);
      #else
        const intSerialArray & maskLocal=mg.mask();
        const realSerialArray & uLocal = u[grid];
        const realSerialArray & xLocal = mg.vertex();
	if( pErr!=NULL )
	  error.reference((*pErr)[grid]);
      #endif
      
      bool ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,I1,I2,I3);

      if( false )
	printf(" checkError: grid=%i %s : I1=[%i,%i], I2=[%i,%i] \n",grid,(it==0 ? "egid" : "gid+1"), 
	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());

      err=0.;
      if( ok )
      {
	realSerialArray uTrue(I1,I2,I3);
        const bool isRectangular=false;  // do this for now
	exact.gd( uTrue,xLocal,cg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,0,t);

        if( pErr!=NULL ) 
          error=0.;
        else
          error.redim(I1,I2,I3);
       
	error(I1,I2,I3)=fabs(uLocal(I1,I2,I3)-uTrue(I1,I2,I3));

	if( it==0 )
	{
	  where( maskLocal(I1,I2,I3)==0 || (maskLocal(I1,I2,I3) & MappedGrid::IShiddenByRefinement) )
	    error(I1,I2,I3)=0.;
	}
	else
	{
	  where( maskLocal(I1,I2,I3)==0 || (maskLocal(I1,I2,I3) & MappedGrid::IShiddenByRefinement) )
	    error(I1,I2,I3)=0.;
	}
      
	err=max(error(I1,I2,I3));
      }
      err=ParallelUtility::getMaxValue(err);

      if( err > maxErr )
      {
	gridMax=grid;
	maxErr=err;
      }
      if( debug>0 && file!=NULL )
      {
        if( it==0 )
	{
	  fprintf(file,"\n**** %s t=%e, grid=%i, max error=%8.2e (egid=includes interp)***** \n",
                  (const char*)label,t,grid,err);
	  // display(u[grid](I1,I2,I3),"solution on extendedGridIndexRange",file,"%4.2f ");
      
          const IntegerArray & gir = mg.gridIndexRange();
          fPrintF(file,"grid=%i: gridIndexRange=[%i,%i][%i,%i][%i,%i] ng=%i ni=%i\n",
                  grid,gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2),cg.numberOfComponentGrids(),
                  cg.numberOfInterpolationPoints(grid));
             
          if( err>1.e-4 )
	  {
	    display(error(I1,I2,I3)/err,sPrintF("grid=%i rel-err on extendedGIR t=%8.2e, err=%8.2e, maxErr=%8.2e",
					    grid,t,err,maxErr),file,"%4.1f");

	    displayMask(maskLocal(I1,I2,I3),"mask",file);
	  }
	  
	}
	
  	// display(error(I1,I2,I3),"error on extendedGridIndexRange",file,"%6.0e");

        // check many digits for comparing parallel results
	// display(error(I1,I2,I3),"error on extendedGridIndexRange",file,"%19.12e");

      }
      if( false && debug>0 )
      {
	printF("**** t=%e, grid=%i, maximum error=%8.2e ***** \n",t,grid,err);
	// display(u[grid](I1,I2,I3),"solution on extendedGridIndexRange",file,"%4.2f ");
	
      }
      
    }
    maxErrv[it]=maxErr;
    gridMaxv[it]=gridMax;
  }
  
  if( file!=NULL )
    fprintf(file,"---------------------------------------------------------------------------------------\n"
                 "%s max err at t=%8.2e is (egid)=%8.2e (grid %i) or gid=%8.2e (grid %i) \n"
                 "-------------------------------------------------------------------------------------\n",
              (const char*)label,t,maxErrv[0],gridMaxv[0],maxErrv[1],gridMaxv[1]);
  printF("%s maximum error at t=%8.2e is (egid)=%9.3e (grid %i) or (gid+%i ghost)=%9.3e (grid %i)\n",
         (const char*)label,t,maxErrv[0],gridMaxv[0], numGhost,maxErrv[1],gridMaxv[1]);
  
  return maxErrv[0]; // return error from egid (no ghost)
}


int
outputRefinementInfo( GridCollection & gc, 
                      int refinementRatio, 
                      const aString & gridFileName, 
                       const aString & fileName )
// =======================================================================================
// /Description:
//   This function will output a command file for the "refine" test code.
// /gc(input) : name of the grid.
// /refinementRatio (input) : refinement ratio.
// /gridFileName (input) : grid file name, such as "cic.hdf". This is not essential,
//    but then you will have to edit the comamnd file to add the correct name.
// /fileName (input) : name of the output command file, such as "bug.cmd"
// The output will be a file of the form
// \begin{verbatim}
// choose a grid
//   cic.hdf
// add a refinement
//   0 1 4 10 12 15
// add a refinement
//   0 1 3 10 15 19
// add a refinement
//   1 1 12 16 0 7
// add a refinement
//   1 1 16 20 3 7
// \end{verbatim}
// ========================================================================================
{
  printf("*** outputing a command file %s for refine ****\n",(const char*)fileName);
  
  FILE *file=fopen(fileName,"w");
  fprintf(file,"choose a grid\n"
	  " %s \n",(const char*)gridFileName);
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>0 )
    {
      MappedGrid & mg = gc[grid];
      fprintf(file,"add a refinement\n"
              " %i %i  %i %i %i %i %i %i %i\n",gc.baseGridNumber(grid),gc.refinementLevelNumber(grid),
              mg.gridIndexRange(0,0)/refinementRatio,mg.gridIndexRange(1,0)/refinementRatio,
              mg.gridIndexRange(0,1)/refinementRatio,mg.gridIndexRange(1,1)/refinementRatio,
              mg.gridIndexRange(0,2)/refinementRatio,mg.gridIndexRange(1,2)/refinementRatio,
             refinementRatio );
    }
  }
  fclose(file);
  return 0;
}
