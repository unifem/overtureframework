// 
//  Test code for adding refiment grids to an existing overlapping grid
//  Solve a problem with Oges on the resulting grid
//
//  mpirun -np 2 -all-local refine
// mcr:
//  mpirun-wdh -np 2 -all-local refine
//  totalview srun -a -N2 -n2 -ppdebug refine
// srun -N16 -n32 -ppdebug refine amrDebug32


#include "PlotStuff.h"
#include "Ogen.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "BoxMapping.h" 
#include "HDF_DataBase.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "display.h"
#include "conversion.h"
#include "interpPoints.h"
#include "InterpolateRefinements.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "gridFunctionNorms.h"
#include "App.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)



void
printInfo( CompositeGrid & cg, int option=0 )
{
  char buff[100];

  printF(" cg.numberOfComponentGrids()=%i, cg.numberOfGrids()=%i, cg.numberOfBaseGrids()=%i, "
        "rl.numberOfRefinementLevels()=%i \n",
        cg.numberOfComponentGrids(),cg.numberOfGrids(),cg.numberOfBaseGrids(),cg.numberOfRefinementLevels());

  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    printF(" grid=%i, componentGridNumber=%i\n",grid,cg.componentGridNumber(grid));
    
  if( false )
  {
    display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
    for( grid=0; grid<cg.interpolationPoint.getLength(); grid++ )
    {
      display(cg.interpolationPoint[grid],sPrintF(buff,"cg.interpolationPoint[%]",grid));
    }
  }
  
  for( int l=0; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    printF("level=%i, rl.numberOfComponentGrids()=%i, rl.numberOfGrids()=%i, rl.numberOfBaseGrids()=%i, "
           "numberOfRefinementLevels=%i \n",l,
           rl.numberOfComponentGrids(),rl.numberOfGrids(),rl.numberOfBaseGrids(),rl.numberOfRefinementLevels());
    for( int g=0; g<rl.numberOfGrids(); g++ )
    {
      grid=rl.gridNumber(g);
      const IntegerArray & gid=cg[grid].gridIndexRange();
      printF("level=%i, g=%i, rl.gridNumber(g)=%i, rl.baseGridNumber(g)=%i, cg.baseGridNumber(%i)=%i "
             "gid=[%i,%i]x[%i,%i]x[%i,%i] rf=[%i,%i]\n",
             l,g,rl.gridNumber(g),rl.baseGridNumber(g),grid,cg.baseGridNumber(grid),
             gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
             cg.refinementFactor(0,grid),cg.refinementFactor(1,grid));
      if( max(abs( gid-rl[g].gridIndexRange()))!=0 )
      {
	printF("***ERROR*** cg[grid].gridIndexRange() != rl[g].gridIndexRange()\n");
	display(rl[g].gridIndexRange(),"rl[g].gridIndexRange()");
      }

      if( option>0 && l>0 )
      {
	displayMask(cg[grid].mask(),sPrintF(buff,"mask for grid %i\n",grid));
      }
    }
  }
  // display(cg.interpolationWidth,"cg.interpolationWidth","%2i");
  // display(cg.interpolationOverlap,"cg.interpolationOverlap");
}

// int
// saveGrid(CompositeGrid & cg, const aString & fileName, const aString & gridName )
// {
//   HDF_DataBase dataFile;
//   dataFile.mount(fileName,"I");

//   // CompositeGrid cg2 = c;  // should I make a copy before destroying ?

// //       // *******************
//   if( true )
//   {
//     cg->computedGeometry |=
//       CompositeGrid::THEmask                     |
//       CompositeGrid::THEinterpolationCoordinates |
//       CompositeGrid::THEinterpolationPoint       |
//       CompositeGrid::THEinterpoleeLocation       |
//       CompositeGrid::THEinterpoleeGrid;
//   }
  

//       // first destroy any big geometry arrays: (but not the mask if we have more than 1 grid)
//   if( cg.numberOfGrids() > 1 )
//     cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
//   else
//     cg.destroy(CompositeGrid::EVERYTHING);
      
//   cg.put(dataFile,gridName);

//   dataFile.unmount();

//   return 0;
// }


enum ProblemTypeEnum
{
  testGeneralInterpolateRefinements=10,
  testInterpolateRefinements,
  testInterpolateRefinementBoundaries,
  testInterpolant,
  testCoarseFromFine,
  testFineFromCoarse,
  testExtrapolateInterpolationNeighbours,
  testOges,
  testIdentityOges
};



int
solveProblem( aString & gridName,
              CompositeGrid & cg, realCompositeGridFunction & u, GenericGraphicsInterface & ps, 
	      GraphicsParameters & psp, int problemType=0, int plotOption=0,
              CompositeGrid *cga=NULL, realCompositeGridFunction *ua=NULL  )
// ========================================================================================================
// 
// problemType: Chosen from the ProblemTypeEnum above
// 
// ========================================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  int debug=0;
  bool saveCheckFile=false; // true;
  bool useTtrigTZ=true;  // **********
  
  int grid;

  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
  
  Range all;
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);

  coeff.setOperators(op);

  // create grid functions: 
  u.updateToMatchGrid(cg);
  realCompositeGridFunction f(cg), err(cg), ue(cg);
  f=0.; // for iterative solvers
  err=0.;

  u.setOperators(op);

  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3;
  char buff[180];
  const real bogusValue=99.;
  
  for( int degree=1; degree<=1; degree++ )
  {
    
    // create a twilight-zone function for checking the errors
    int degreeOfSpacePolynomial = degree; // problemType<2 ? 2 : 1;
    int degreeOfTimePolynomial = 0;
    int numberOfComponents = cg.numberOfDimensions();
    OGPolyFunction poly(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
			degreeOfTimePolynomial);

    real fx=1., fy=1., fz=cg.numberOfDimensions()==2 ? 0. : 1.;

    OGTrigFunction trig(fx,fy,fz);
    RealArray gx(10);
    gx=.333;
    trig.setShifts(gx,gx,gx,gx);

    OGFunction *exactPointer=&poly;

    if( useTtrigTZ ) exactPointer=&trig;  // *************
    
    
    OGFunction & exact= *exactPointer;


    // ========== Test the Interpolant. ================
    if( problemType==testInterpolateRefinements || 
        problemType==testInterpolateRefinementBoundaries ||
        problemType==testGeneralInterpolateRefinements ||
        problemType==testInterpolant ||
        problemType==testCoarseFromFine ||
        problemType==testExtrapolateInterpolationNeighbours ) 
    {
      aString label=(problemType==testInterpolateRefinements ? "interp refinements" :
		     problemType==testInterpolateRefinementBoundaries ? "interp refinement bndrys" :
		     problemType==testGeneralInterpolateRefinements ? "general interp refinements" :
		     problemType==testInterpolant ? "Interpolant" : 
                     problemType==testCoarseFromFine ? "coarse from fine" :
                     problemType==testExtrapolateInterpolationNeighbours ? "extrap interp neighbours" :
                     "unknown");

      u=0.;

      Interpolant interpolant(cg);

      exact.assignGridFunction(u);
      exact.assignGridFunction(ue);
      
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();
	getIndex(mg.dimension(),I1,I2,I3);  
//	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

        intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
        realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray ueLocal;   getLocalArrayWithGhostBoundaries(ue[grid],ueLocal);

	bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);
	if( !ok ) continue;
	
	if( problemType==testCoarseFromFine )
	{
	  where( (maskLocal & MappedGrid::IShiddenByRefinement) && ( maskLocal > 0 )  )
	  {
	    uLocal=bogusValue;
	  }
	}
	else if( problemType==testInterpolant )
	{
	  where( maskLocal<=0 ) 
	  {
	    uLocal=bogusValue;
	  }
	}
	else if( problemType==testExtrapolateInterpolationNeighbours )
	{
	  where( maskLocal==0 ) 
	  {
	    uLocal=bogusValue;
	  }
	}
	
        if( grid==cg.baseGridNumber(grid) )
	{
          // uLocal=0.;  // ****************** check for a bug in interpRefinementBoundaries
	  

          // base grid: set bogus values at hidden points
	  if( problemType==testInterpolateRefinements )
	  {
	    where( maskLocal<=0 || (maskLocal & MappedGrid::IShiddenByRefinement) )
	    {
	      uLocal=bogusValue;
	    }
	  }
	  
	}
	else
	{ 
          // for refinement grids, set bogus values everywhere
	  if( problemType==testInterpolateRefinements )
	  {
	    uLocal=bogusValue;
	  }
	  
          // for refinement grids, set bogus values on the interpolation points.
	  if(  problemType!=testCoarseFromFine )
	  {
	    ForBoundary(side,axis)
	    {
	      if( mg.boundaryCondition(side,axis) == 0 )
	      {
                // The extendedIndexRange includes all discretization points plus ghost line interpolation points
		getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);

		bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,1);
		if( !ok ) continue;
		
                if( problemType==testInterpolateRefinementBoundaries )
		{
                  where( maskLocal(Ib1,Ib2,Ib3)>0 )
		    uLocal(Ib1,Ib2,Ib3)=bogusValue;
		}
		else
		{
                  where( maskLocal(Ib1,Ib2,Ib3)!=0 )
  		    uLocal(Ib1,Ib2,Ib3)=bogusValue;
		}
		
	      }
	    }
	  }
	  
	}

	if( false && mg.numberOfDimensions()==3 )
	{
          const IntegerArray & gid=mg.gridIndexRange();
          intArray & mask = mg.mask();
          int extra=2;
          if( grid==0 )
	  {
	    int i1=26/2, i2=28/2, i3=38/2;
            printf(" After fill bogus: grid=%i: iv=(%i,%i,%i) mask=%i uLocal=%e  grid=[%i,%i][%i,%i][%i,%i]\n",
                   grid,i1,i2,i3,maskLocal(i1,i2,i3),
		   uLocal(i1,i2,i3),gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));

            Index I1=Range(i1-extra,i1+extra), I2=Range(i2-extra,i2+extra), I3=Range(i3-extra,i3+extra);
            displayMask(mask(I1,I2,I3));

	  }
	  
	  if( grid==2 )
	  {
	    int i1=26, i2=28, i3=38;
            printf(" After fill bogus: grid=%i: iv=(%i,%i,%i) mask=%i uLocal=%e  grid=[%i,%i][%i,%i][%i,%i]\n",
                   grid,i1,i2,i3,maskLocal(i1,i2,i3),
		   uLocal(i1,i2,i3),gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
            Index I1=Range(i1-extra,i1+extra), I2=Range(i2-extra,i2+extra), I3=Range(i3-extra,i3+extra);
            displayMask(mask(I1,I2,I3));
	  }

	  
	  if( grid==3 )
	  {
	    int i1=26*2, i2=28*2, i3=38*2;
	     
            printf(" After fill bogus: grid=%i: iv=(%i,%i,%i) mask=%i uLocal=%e  grid=[%i,%i][%i,%i][%i,%i]\n",
                   grid,i1,i2,i3,maskLocal(i1,i2,i3),
		   uLocal(i1,i2,i3),gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
            Index I1=Range(i1-extra,i1+extra), I2=Range(i2-extra,i2+extra), I3=Range(i3-extra,i3+extra);
            displayMask(mask(I1,I2,I3));
	  }
	}
	

	if( false && grid==0 )
	{
	  
	  int i1=53, i2=9, i3=0;
	  if( i1>=I1.getBase() && i1+2<=I1.getBound() &&
              i2>=I2.getBase() && i2+2<=I2.getBound() )
	  {
	    printF(" YYYY myid=%i, i=(%i,%i,%i) grid=%i  local bounds=[%i,%i][%i,%i]\n"
		   " donor values: %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   " exact values: %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n",
		   myid,i1,i2,i3,grid,
		   uLocal.getBase(0),uLocal.getBound(0), uLocal.getBase(1),uLocal.getBound(1),
		   uLocal(i1  ,i2  ,i3),uLocal(i1+1,i2  ,i3),uLocal(i1+2,i2  ,i3),
		   uLocal(i1  ,i2+1,i3),uLocal(i1+1,i2+1,i3),uLocal(i1+2,i2+1,i3),
		   uLocal(i1  ,i2+2,i3),uLocal(i1+1,i2+2,i3),uLocal(i1+2,i2+2,i3),
		   ueLocal(i1  ,i2  ,i3),ueLocal(i1+1,i2  ,i3),ueLocal(i1+2,i2  ,i3),
		   ueLocal(i1  ,i2+1,i3),ueLocal(i1+1,i2+1,i3),ueLocal(i1+2,i2+1,i3),
		   ueLocal(i1  ,i2+2,i3),ueLocal(i1+1,i2+2,i3),ueLocal(i1+2,i2+2,i3));
	    
	  }
	  else if( i1>=I1.getBase() && i1+1<=maskLocal.getBound(0) &&
                   i2>=I2.getBase() && i2+1<=maskLocal.getBound(1) )
	  {
	    printF(" ZZZZ myid=%i, i=(%i,%i,%i) grid=%i local bounds=[%i,%i][%i,%i]\n"
		   " donor values: %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   " exact values: %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n"
		   "               %6.4f %6.4f %6.4f \n",
		   myid,i1,i2,i3,grid,
		   uLocal.getBase(0),uLocal.getBound(0), uLocal.getBase(1),uLocal.getBound(1),
		   uLocal(i1  ,i2  ,i3),uLocal(i1+1,i2  ,i3),uLocal(i1+2,i2  ,i3),
		   uLocal(i1  ,i2+1,i3),uLocal(i1+1,i2+1,i3),uLocal(i1+2,i2+1,i3),
		   uLocal(i1  ,i2+2,i3),uLocal(i1+1,i2+2,i3),uLocal(i1+2,i2+2,i3),
		   ueLocal(i1  ,i2  ,i3),ueLocal(i1+1,i2  ,i3),ueLocal(i1+2,i2  ,i3),
		   ueLocal(i1  ,i2+1,i3),ueLocal(i1+1,i2+1,i3),ueLocal(i1+2,i2+1,i3),
		   ueLocal(i1  ,i2+2,i3),ueLocal(i1+1,i2+2,i3),ueLocal(i1+2,i2+2,i3));
	    
	  }

	}
      }

      // u.display("u before interpolation");
      // u.interpolate();


      InterpolateRefinements interp(cg.numberOfDimensions());

      if( problemType==testInterpolateRefinements )
      {
        if( cga==NULL )
	{
	  printF(" test interpolateRefinements by interpolating u from exact on the same grid\n");

	  interp.interpolateRefinements(ue, u);
          Overture::checkMemoryUsage("After interp.interpolateRefinements");  
	}
	else
	{
	  printF(" test interpolateRefinements by interpolating u from exact on another grid\n");

	  CompositeGrid & cgOld = cga[1];
          cgOld.update(MappedGrid::THEvertex | MappedGrid::THEcenter );

	  realCompositeGridFunction & uOld = ua[1];
          uOld.updateToMatchGrid(cgOld);
	  exact.assignGridFunction(uOld);

          Overture::checkMemoryUsage("Before interp.interpolateRefinements");  
	  interp.interpolateRefinements(uOld, u);
          Overture::checkMemoryUsage("After interp.interpolateRefinements");  

	}
      }
      else if( problemType==testInterpolateRefinementBoundaries )
      {
	interp.interpolateRefinementBoundaries(u);
      }
      else if( problemType==testGeneralInterpolateRefinements )
      {
	// see $gf/interpolateRefinements.C
	//    : this will assign interpolation points on refinement grids
	interpolateRefinements(u);  // **** do this to set MappedGrid::IShiddenByRefinement
      }
      else if( problemType==testInterpolant )
      {
        Overture::checkMemoryUsage("Before interpolate");  

        u.interpolate();  // this interp's refinement grids too now

        // interp.interpolateCoarseFromFine(u); // testing **************

	if( false )
	{
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    u[grid].updateGhostBoundaries();
	
	  u.interpolate();  // this interp's refinement grids too now
	}
        Overture::checkMemoryUsage("After interpolate");  
      }
      else if( problemType==testExtrapolateInterpolationNeighbours )
      {
        BoundaryConditionParameters extrapParams;
        extrapParams.orderOfExtrapolation=2;
        u.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,0.,
                                 extrapParams);
	if( true )
	{ // in general this is needed:
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    u[grid].updateGhostBoundaries();
	}

      }
      else if( problemType==testCoarseFromFine )
      {
        interp.interpolateCoarseFromFine(u);
	u.periodicUpdate();  // *wdh* 060415
      }
      else
      {
	Overture::abort("error -- problemType?");
      }
      

      if( saveCheckFile )
      {
	aString checkFileName = gridName(0,gridName.length()-1) + sPrintF(buff,".refineNP%i.check",np);
	FILE *check = fopen((const char*)checkFileName,"w");
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  ::display(u[grid],sPrintF(buff,"Solution on grid %i",grid),check,"%20.14e ");
	}
	fclose(check);
	printF("Solution saved to the check file: %s\n",(const char*)checkFileName);
      }

      // u.display("u after interpolation","%6.2e ");
      if( false )
      {
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  u[grid].updateGhostBoundaries();
	
      }

      int extra=1;
      real error=0.;
      // err=fabs(u-ue); // problem here (with non-padre version ?)
      // error=maxNorm(err,0,0,extra);
      error=0.;

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {

	// getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
	if( true || problemType==testInterpolateRefinementBoundaries )
	{
          getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);  // include ghost pts on interp sides
	}
	
        const intArray & mask = cg[grid].mask();
	intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
	realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray ueLocal;   getLocalArrayWithGhostBoundaries(ue[grid],ueLocal);
	realSerialArray errLocal;  getLocalArrayWithGhostBoundaries(err[grid],errLocal);

        real maxErr=0.;

	bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3);
	if( ok )
	{
	  // err[grid](I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  errLocal=fabs(uLocal-ueLocal);
	  if( problemType==testExtrapolateInterpolationNeighbours )
	  {
	    where( maskLocal(I1,I2,I3)==0 && 
		   maskLocal(I1+1,I2,I3)==0 &&
		   maskLocal(I1-1,I2,I3)==0 &&
		   maskLocal(I1,I2+1,I3)==0 &&
		   maskLocal(I1,I2-1,I3)==0 )
	    {
	      errLocal(I1,I2,I3)=0.;
	    }
	  }
	  else
	  {
	    where( maskLocal(I1,I2,I3)==0 )
	    {
	      errLocal(I1,I2,I3)=0.;
	    }
	  }
	
	  // where( cg[grid].mask()(I1,I2,I3)!=0 )
	  maxErr=max(fabs(errLocal(I1,I2,I3)));

	}


	maxErr=ParallelUtility::getMaxValue(maxErr);
	error=max(error,maxErr);

	Overture::checkMemoryUsage("After check errors");  

	if( myid==0 )
	{
	  printF(" ** max error on grid %i = %e \n",grid,maxErr);
	  MappedGrid & mg=cg[grid];
	  
          const IntegerArray & egir = extendedGridIndexRange(mg);
	  
	  if( false )
	  {
	    printF(" grid=%i: bc=[%i,%i][%i,%i] ir=[%i,%i][%i,%i] gir=[%i,%i][%i,%i] eir=[%i,%i][%i,%i] "
		   "egir=[%i,%i][%i,%i]\n",grid,
		   mg.boundaryCondition(0,0),mg.boundaryCondition(1,0),
		   mg.boundaryCondition(0,1),mg.boundaryCondition(1,1),
		   mg.indexRange(0,0),mg.indexRange(1,0),
		   mg.indexRange(0,1),mg.indexRange(1,1),
		   mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),
		   mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
		   mg.extendedIndexRange(0,0),mg.extendedIndexRange(1,0),
		   mg.extendedIndexRange(0,1),mg.extendedIndexRange(1,1),
		   egir(0,0),egir(1,0),
		   egir(0,1),egir(1,1));
	  }
	  
                 
	}
	
	// where( cg[grid].mask()(I1,I2,I3)!=0 )
	//   error=max(error,max(abs(err[grid](I1,I2,I3))));    
	const real tol=1.; // .05;
	if( Oges::debug & 8 || ( problemType==testInterpolant  && maxErr>tol ) )
	{

	  // displayMask(cg[grid].mask(),"Here is the mask");
	  // display(err,"abs(error on indexRange +1)","%5.1e ");

	  // display(err[grid](I1,I2,I3),"abs(error on indexRange +1)","%3.1f ");

	  MappedGrid & mg = cg[grid];
	  const IntegerArray & gid=mg.gridIndexRange();

 	  if( !ok ) continue;

	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( errLocal(i1,i2,i3)>tol )
	    {
	      printf(" ++++ myid=%i grid=%i err(%i,%i,%i)=%8.2e, u=%11.5e ue=%11.5e mask=%i mask&hidden=%i"
                     " grid=[%i,%i][%i,%i][%i,%i]\n",
		     myid,grid,i1,i2,i3,errLocal(i1,i2,i3),uLocal(i1,i2,i3),ueLocal(i1,i2,i3),maskLocal(i1,i2,i3),
                     int(maskLocal(i1,i2,i3)&MappedGrid::IShiddenByRefinement),
                     gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2)  );
	    }
	  }
	  
	  fflush(stdout);
	  
	}
      } // end for grid
      

      if( myid==0 )
	printF("\n >>>Maximum error for %s = %e (degree=%i,np=%i)\n\n",(const char*)label,error,degree,np);  

      
      if( false )
      {
	ps.erase();
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution after %s, degree=%i",(const char*)label,degree));
	PlotIt::contour(ps,u,psp);

	ps.erase();
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Error after %s, degree=%i",(const char*)label,degree));
	PlotIt::contour(ps,err,psp);
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      }


      continue;
    }


//     // ========== Test the Interpolant. ================
//     if( problemType==testInterpolant ) 
//     {
//       CompositeGridOperators cgop(cg);
//       u.setOperators(cgop);
    
//       u=0.;

//       Interpolant interpolant(cg);
//       interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	MappedGrid & mg = cg[grid];
// 	const intArray & mask = mg.mask();
// 	getIndex(mg.dimension(),I1,I2,I3);  
// 	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

// 	where( cg[grid].mask()(I1,I2,I3)<=0 )
// 	  u[grid](I1,I2,I3)=-99.;
//       }

//       if( debug & 4 )
//       {
// 	display(u[3],"u[3] before interpolation","%6.2e ");
// 	displayMask(cg[3].mask(),"mask");
//       }
    
//       u.interpolate();
    
//       if( debug & 4 )
//       {
// 	display(u[3],"u[3] after interpolation","%6.2e ");
//       }
    
//       Index J1,J2,J3;
//       real error=0., err=0.;
//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
// 	where( cg[grid].mask()(I1,I2,I3)!=0 )
// 	{
// 	  err=max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)));    
// 	  error=max(error,err);
// 	}
// 	getIndex(cg[grid].gridIndexRange(),J1,J2,J3);
// 	real uMax=0., uMin=0.;
// 	where( cg[grid].mask()(J1,J2,J3)!=0 )
// 	{
// 	  uMin=min(u[grid](J1,J2,J3));
// 	  uMax=max(u[grid](J1,J2,J3));
// 	}
// 	printF("testInterp: degree=%i grid=%i uMin=%8.2e uMax=%8.2e err=%8.2e \n",degree,grid,uMin,uMax,err);  
      
// 	if( Oges::debug & 8 )
// 	{
// 	  cg[grid].mask().display("Here is the mask");
// 	  realArray err(I1,I2,I3);
// 	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
// 	  where( cg[grid].mask()(I1,I2,I3)==0 )
// 	    err(I1,I2,I3)=0.;
// 	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
// 	  // display(err,"abs(error on indexRange +1)","%5.1e ");
// 	  display(err,"abs(error on indexRange +1)","%3.1f ");
// 	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
// 	}
//       }
//       printF("Maximum error in interpolate= %e (degree=%i)\n",error,degreeOfSpacePolynomial);  

//       continue;
//     }
  
  
//     // ========== Test coarse from fine interpolation. ================
//     if( problemType==testCoarseFromFine ) 
//     {
//       cg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
    
//       u=0.;

//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	MappedGrid & mg = cg[grid];
// 	const intArray & mask = mg.mask();
// 	getIndex(mg.dimension(),I1,I2,I3);  
// 	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

// 	where( (mask & MappedGrid::IShiddenByRefinement) && ( mask > 0 )  )
// 	{
// 	  u[grid]=99.;
// 	}
// 	// if( grid==2 ) u[grid]=77;
      
//       }

//       // u.display("u before interpolation");
//       // u.interpolate();

//       InterpolateRefinements interp(cg.numberOfDimensions());
    
//       // u.display("u after interpolation","%6.2e ");
//       // cout << "cg[2].box = " << cg[2].box() << endl;
    
// //       display(u[2],"u[2] BEFORE interpolateCoarseFromFine","%6.2e ");
// //       displayMask(cg[2].mask(),"mask");
    
//       interp.interpolateCoarseFromFine(u);
    
//       u.periodicUpdate();  // *wdh* 060415
      
//       if( false )
//       {
// 	display(u[2],"u[2] AFTER interpolateCoarseFromFine","%6.2e ");
// 	display(u[1],"u[1] AFTER interpolateCoarseFromFine","%6.2e ");
//       }
      

//       real error=0.;
//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
// 	where( cg[grid].mask()(I1,I2,I3)!=0 )
// 	  error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
// 	if( Oges::debug & 8 )
// 	{
// 	  cg[grid].mask().display("Here is the mask");
// 	  realArray err(I1,I2,I3);
// 	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
// 	  where( cg[grid].mask()(I1,I2,I3)==0 )
// 	    err(I1,I2,I3)=0.;
// 	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
// 	  // display(err,"abs(error on indexRange +1)","%5.1e ");
// 	  display(err,"abs(error on indexRange +1)","%3.1f ");
// 	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
// 	}
//       }
//       printF("Maximum error in interpolateCoarseFromFine(u)= %e\n",error);  

//       continue;
//     }

    // ========== Now test the elliptic equation solver. ================
    if( testOges || testIdentityOges )
    {
      if( FALSE )
	Oges::debug=63;
  
//  if( Oges::debug > 3 )
//    SparseRepForMGF::debug=3;

      // make some shorter names for readability
      BCTypes::BCNames extrapolate           = BCTypes::extrapolate,
	dirichlet             = BCTypes::dirichlet,
	allBoundaries         = BCTypes::allBoundaries; 

      // make a grid function to hold the coefficients
//   Range all;
//   int stencilSize=pow(3,cg.numberOfDimensions())+1;  // add 1 for interpolation equations
//   realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
//   coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
//   CompositeGridOperators op(cg);                            // create some differential operators
//   op.setStencilSize(stencilSize);
//   coeff.setOperators(op);  

      coeff.updateToMatchGrid( cg , stencilSize, all, all, all);
      op.updateToMatchGrid( cg );
      op.gridCollection.updateReferences(); // **** work around to fix a bug in CG reference function
      coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
      coeff.setOperators(op);
  
      if( problemType==testOges )
	coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      else
	coeff=op.identityCoefficients();
  
      // coeff[0]=op[0].identityCoefficients();

      coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);


      BoundaryConditionParameters bcParams;
      if( false )
      {
	printF("*****  bcParams.setRefinementLevelToSolveFor(0); ******* \n");
	bcParams.setRefinementLevelToSolveFor(0);
      }
      coeff.finishBoundaryConditions(bcParams);
      //  coeff.display("Here is coeff after finishBoundaryConditions");

      Oges solver( cg );                     // create a solver
      solver.setCoefficientArray( coeff );   // supply coefficients
      if( cg.numberOfDimensions()==3 )
      {
	solver.setSolverType(Oges::bcg);
	solver.setConjugateGradientPreconditioner(Oges::incompleteLU);
	solver.setConjugateGradientTolerance(REAL_EPSILON*10.);
      }    

      // assign the rhs: Laplacian(u)=f, u=exact on the boundary
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.indexRange(),I1,I2,I3);  
	// display(mg.boundaryCondition(),"Here is bc");
	if( problemType==0 )
	{
	  if( cg.numberOfDimensions()==1 )
	    f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
	  else if( cg.numberOfDimensions()==2 )
	    f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
	  else
	    f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
	}
	else
	{
	  f[grid](I1,I2,I3)=exact(mg,I1,I2,I3,0);
	}
    
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	  }
	}
      }
      if( Oges::debug & 16 )
	f.display("Here is the rhs f");
  
      u=0.;  // initial guess for iterative solvers
      real time0=getCPU();
      printF("solve Poisson's equation with Oges...\n");
      solver.solve( u,f );   // solve the equations
      real time=getCPU()-time0;
      cout << "time for 1st solve of the Dirichlet problem = " << time << endl;

      if( Oges::debug & 16 )
	u.display("Here is u");
      real error=0., gridError;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
    
	getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	{
	  gridError=max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))); 
	}
	error=max(error, gridError);
	if( TRUE )
	  printF("Maximum error = %e on grid %s\n",gridError,
		 (const char *)cg[grid].mapping().getName(Mapping::mappingName));

	if( FALSE || Oges::debug & 8 )
	{
	  char buff[80];
      
	  displayMask(cg[grid].mask(),sPrintF(buff,"Here is the mask on grid %i",grid));
	  realArray err(I1,I2,I3);
	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  where( cg[grid].mask()(I1,I2,I3)==0 )
	    err(I1,I2,I3)=0.;
	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
	  // display(err,"abs(error on indexRange +1)","%5.1e ");
	  // display(err,"abs(error on indexRange +1)","%3.1f ");
	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	}
      }
      printF("Maximum error with dirichlet bc's= %e\n",error);  

      continue;
    }
    

  }  // end for degree


  return 0;


}


int 
checkInterpolation( realCompositeGridFunction & u,  
		    real & maxError, 
		    OGFunction *exact=NULL, 
		    ParallelOverlappingGridInterpolator *pogi=NULL,
		    const aString & label = blankString );


int checkInterpolation( realCompositeGridFunction & u, const aString & label = blankString )
{

  CompositeGrid & cg = *u.getCompositeGrid();

  int degreeOfSpacePolynomial = 2; // degree; // problemType<2 ? 2 : 1;
  int degreeOfTimePolynomial = 0;
  int numberOfComponents = u.getComponentDimension(0); 
  OGPolyFunction poly(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
		      degreeOfTimePolynomial);

  Interpolant interpolant(cg);
  CompositeGridOperators cgop(cg);
  u.setOperators(cgop);
  
  real maxError;
  checkInterpolation(u,maxError,&poly,NULL,label);

  return 0;
}


int
outputInterpolationData( CompositeGrid & cg0, const aString & label, FILE *file=stdout )
// ================================================================================
//  /Description:
//     Output all interpolation data for this grid. This can be used for regression
//     testing. 
// ================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int processorToPrint=0;
  
  // Just copy the grid and all interpolation data to a single processor:
  CompositeGrid cg;
  ParallelGridUtility::redistribute( cg0, cg, Range(processorToPrint,processorToPrint) );
  if( myid==processorToPrint) 
  {
    const int numberOfDimensions=cg.numberOfDimensions();
    fPrintF(file,"Interpolation Data: %s",(const char*)label);
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {  
      int ni = cg.numberOfInterpolationPoints(grid);
      
      const intSerialArray & interpolationPoint = cg.interpolationPoint[grid].getLocalArray(); 
      const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid].getLocalArray(); 
      const intSerialArray & variableInterpolationWidth = cg.variableInterpolationWidth[grid].getLocalArray(); 
      const realSerialArray & interpolationCoordinates = cg.interpolationCoordinates[grid].getLocalArray();
      const intSerialArray & interpoleeLocation = cg.interpoleeLocation[grid].getLocalArray(); 

      fPrintF(file,"\n ---- grid=%i ni=%i ---\n",grid,ni);
      for( int i=0; i<ni; i++ )
      {
// 	printF("grid=%i i=%i ip=(%i,%i,%i)\n",grid,i,interpolationPoint(i,0),interpolationPoint(i,1),
// 	       (numberOfDimensions==2 ? 0 : interpolationPoint(i,2)));
// 	printF(" donor=%i \n",interpoleeGrid(i));
// 	printF(" il=(%i,%i,%i)\n",
// 	       interpoleeLocation(i,0),interpoleeLocation(i,1),
// 	       (numberOfDimensions==2 ? 0 : interpoleeLocation(i,2)));
// 	printF("ci=(%9.2e,%9.2e,%9.2e)\n",
// 	       interpolationCoordinates(i,0),interpolationCoordinates(i,1),
// 	       (numberOfDimensions==2 ? 0. : interpolationCoordinates(i,2)));
// 	printF("width=%i\n",variableInterpolationWidth(i));
      
	fPrintF(file," grid=%i: i=%i ip=(%i,%i,%i) donor=%i il=(%i,%i,%i) width=%i ci=(%9.2e,%9.2e,%9.2e)\n",
		grid,i,
		interpolationPoint(i,0),interpolationPoint(i,1),
		(numberOfDimensions==2 ? 0 : interpolationPoint(i,2)),
		interpoleeGrid(i),
		interpoleeLocation(i,0),interpoleeLocation(i,1),
		(numberOfDimensions==2 ? 0 : interpoleeLocation(i,2)),
		variableInterpolationWidth(i),
		interpolationCoordinates(i,0),interpolationCoordinates(i,1),
		(numberOfDimensions==2 ? 0. : interpolationCoordinates(i,2)));
      }
	
    }
    fflush(file);
  }
  
  return 0;
}




int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

//  Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON ); // release memory when done

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  Overture::turnOnMemoryChecking(true);

#ifdef USE_PPP
//    aString fileName = "refineInputFile";
//    ParallelUtility::getArgsFromFile(fileName,argc,argv );

#endif


  int debug=0;

  ParallelOverlappingGridInterpolator::debug=0;

  // Reduce the width of extrapolation for interp neighbours
  // and increase parallel ghost boundary width to 2 -- this will mean that we should
  // get the same result independent of np
  GenericMappedGridOperators::setDefaultMaximumWidthForExtrapolateInterpolationNeighbours(3);
  MappedGrid::setMinimumNumberOfDistributedGhostLines(2); 

  char buff[80];

  bool plotOption=true;
  bool loadBalance=true;
  aString commandFileName="", line;
  if( argc>1 )
  {
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];

      printf("myid=%i argv[%i]=%s\n",myid,i,argv[i]);
      
      if( len=line.matches("-noplot") )
      {
	plotOption=false;
	printF(" Setting plotOption=false\n");
      }
      else if( line=="loadBalance" || line=="-loadBalance" )
      {
	loadBalance=true;
      }
      else if( commandFileName=="" )
      {
	commandFileName=line;    
	if( myid==0 ) printF("Using command file = [%s]\n",(const char*)commandFileName);
      }
    }
  }
  
//   #ifdef USE_PPP
//     ParallelUtility::deleteArgsFromFile(argc,argv);
//   #endif


  PlotStuff ps(plotOption,"refine");               // for plotting
  GraphicsParameters psp;

  aString logFile="refine.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  IntegerArray range(2,3), factor(3);
  range=0; 
  factor=2; // refinement ratio is this by default

  bool checkValidity=true;
  

  aString menu[]=
    {
      "choose a grid",
      "add a refinement",
      "update refinements",
      "remove refinements",
      "delete a refinement",
      "add a base grid",
      "delete a base grid",
      "interpolation width 2" ,
      "solve with oges",
      "solve identity with oges",
      "test interpolateRefinementBoundaries",
      "test coarse from fine",
      "test interpolate",
      "test interpolate refinements",
      "check interpolate",
      "extrapolate interpolation neighbours",
      "grid plot",
      "contour plot",
      "plot parallel distribution",
      "save the grid",
      "debug",
      "set new current grid",
      "do not check validity",
      "output check file",
      "leak check",
      "erase",
      "exit",
      ""
    };
  aString answer,answer2;
  
  int currentGrid=0;
  CompositeGrid cga[2];
  realCompositeGridFunction ua[2];
  
  Ogen ogen(ps);
  ogen.debug=0;
    
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  // psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,0);
  psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,true);
  psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);
  psp.set(GI_PLOT_INTERPOLATION_POINTS,true);  
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);

  int grid=0, level=1;
  bool plotGrid=false;
  aString nameOfOGFile;
  
  for( ;; )
  {

    CompositeGrid & cg = cga[currentGrid];
    realCompositeGridFunction & u = ua[currentGrid];
    

    if( plotOption && plotGrid && cg.numberOfComponentGrids()>0 )
    {
      ps.erase();
      PlotIt::plot(ps,cg,psp);
    }
    
    ps.getMenuItem(menu,answer,"choose");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="set new current grid" )
    {
      currentGrid=1;
    }
    else if( answer=="do not check validity" )
    {
      checkValidity=false;
    }
    else if( answer=="choose a grid" )
    {

      ps.inputString(nameOfOGFile,"Enter the name of the grid");
      printF("read grid %s\n",(const char*)nameOfOGFile);

      getFromADataBase(cg,nameOfOGFile,loadBalance);
      
      cg.update();
      cg.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

      cg.displayDistribution("cg after reading from the file");
      plotGrid=true;
    }
    else if( answer=="add a refinement" )
    {
      if( false ) printInfo(cg);
      ps.inputString(answer2,"Enter grid,level,range(0,0),range(1,0),...,range(1,2) ratio p0 p1");
      grid=0;
      level=1;
      range=0;
      int refinementRatio=2;
      int p0=-1, p1=-1;
      sScanF(answer2,"%i %i %i %i %i %i %i %i %i %i %i",&grid,&level,&range(0,0),&range(1,0),
	     &range(0,1),&range(1,1),&range(0,2),&range(1,2),&refinementRatio,&p0,&p1);

      printF("*** refinementRatio=%i, [p0,p1]=[%i,%i] \n",refinementRatio,p0,p1);

      factor=refinementRatio;
      cg.addRefinement(range, factor, level, grid); 

      int g=cg.numberOfComponentGrids()-1;
      p0=min(p0,np-1);
      p1=min(p1,np-1);
      if( p0>=0 && p1>=p0 )
      {
	printF("Setting processors for grid %i to [%i,%i]\n",g,p0,p1);
	cg[g].specifyProcesses(Range(p0,p1));
      }
      
      cg.update(GridCollection::THErefinementLevel);  // this seems to be needed.
      // display(cg.interpolationWidth,"cg.interpolationWidth");

    }
    else if( answer=="leak check" )
    {
      for( int step=0; step<=10; step++ )
      {
	cg.update(
	  // CompositeGrid::THEinterpolationPoint       |
	  // CompositeGrid::THEinterpoleeGrid           |
	  // CompositeGrid::THEinterpoleeLocation       |
	  CompositeGrid::THEinterpolationCoordinates, 
	  CompositeGrid::COMPUTEnothing);
	
	checkArrayIDs(sPrintF(buff,"regrid:leak check: cg.update (step %i)",step)); 
      }
      
    }
    
    else if( answer=="update refinements" ||
	     answer=="update refinements new" )
    {
      const int numberOfSteps=1;  // check for leaks
      for( int step=0; step<numberOfSteps; step++ )
      {
	checkArrayIDs(sPrintF(buff,"regrid: before updateRefinement (step %i)",step)); // check for possible leaks

	real time0=getCPU();

	if( answer=="update refinements" )
	  ogen.updateRefinement(cg);
	else
	  ogen.updateRefinementNewer(cg);

	real time=ParallelUtility::getMaxValue(getCPU()-time0);
	printF("ogen.updateRefinement: step=%i cpu=%8.2e\n",step,time);
	checkArrayIDs(sPrintF(buff,"regrid: after updateRefinement (step %i)",step)); // check for possible leaks
	real mem=Overture::getCurrentMemoryUsage();
	printf("myid=%i memory usage=%g\n",myid,mem);
      }
      
      // cg.setMaskAtRefinements();

      if( checkValidity )
      {
	  
	printF("Checking validity of the overlapping grid...\n");
	bool onlyCheckBaseGrids=false; // check amr grids too
	int option=0; // 4;  // 4=print interp pt and donor info
	int numberOfErrors=checkOverlappingGrid(cg,option,onlyCheckBaseGrids);
	if( numberOfErrors==0 )
	{
	  printF("Overlapping grid is valid.\n");
	}
	else
	{
	  printF("Checking validity of the overlapping grid, Grid is not valid! Number of errors=%i\n",numberOfErrors);
	}
      }
      

      Ogen::checkUpdateRefinement( cg );

	

      if( debug & 2 )
      {
	for( int grid=0; grid<cg.numberOfGrids(); grid++ )
	  displayMask(cg[grid].mask(),"cg[grid].mask");
      }

//       intSerialArray *ipLocal = new intSerialArray [cg.numberOfBaseGrids()]; 
//       intSerialArray *ilLocal = new intSerialArray [cg.numberOfBaseGrids()]; 
//       realSerialArray *ciLocal = new realSerialArray [cg.numberOfBaseGrids()];
//       transferInterpDataForAMR(cg,ipLocal,ilLocal,ciLocal);

    }
    else if( answer=="remove refinements" )
    {
      cg.deleteRefinementLevels(0);
      cg.update(GridCollection::THErefinementLevel);
    }
    else if( answer=="delete a refinement" )
    {
      int grid=cg.numberOfComponentGrids();
      printInfo(cg);
      ps.inputString(answer2,"Enter the grid to delete.");
      sScanF(answer2,"%i",&grid);
      cg.deleteRefinement(grid);

      // we need to unmark hidden mask values. -- could do better here --
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	cg[grid].mask()&= ~MappedGrid::IShiddenByRefinement;

	// interpolation points on refinements are no longer valid
	if( cg.refinementLevelNumber(grid)>0 )
	{
	  cg.numberOfInterpolationPoints(grid)=0;
	  cg[grid].mask()=MappedGrid::ISdiscretizationPoint;
	  
	}
      }
      
      cg.update(GridCollection::THErefinementLevel);
      
      printF("**** After delete refinement.. ****\n");
      printInfo(cg);

    }
    else if( answer=="add a base grid" )
    {
      Mapping & map = * new SquareMapping(.25,.75,.25,.75 );
      map.incrementReferenceCount();

      map.setBoundaryCondition(Start,axis1,0);
      map.setBoundaryCondition(End  ,axis1,0);
      map.setBoundaryCondition(Start,axis2,0);
      map.setBoundaryCondition(End  ,axis2,0);
      
      cg.add( map );
      
      ogen.updateOverlap(cg);
      
    }
    else if( answer=="delete a base grid" )
    {
      int grid=cg.numberOfComponentGrids();
      printInfo(cg);
      ps.inputString(answer2,"Enter the grid to delete.");
      sScanF(answer2,"%i",&grid);

      cg.deleteGrid(grid);

      ogen.updateOverlap(cg);
      
      printF("**** After delete a base grid.. ****\n");
      printInfo(cg);

    }
    else if( answer=="solve with oges" )
    {
      solveProblem(nameOfOGFile,cg,u,ps,psp,testOges,plotOption);
    }
    else if( answer=="solve identity with oges" )
    {
      solveProblem(nameOfOGFile,cg,u,ps,psp,1,plotOption);
    }
    else if( answer=="test interpolateRefinementBoundaries" )
    {
      solveProblem(nameOfOGFile,cg,u,ps,psp,testInterpolateRefinementBoundaries,plotOption);

      // Interpolant::testInterpolation( cg,2 );  // ********* should be the same as above
    }
    else if( answer=="test interpolate" )
    {
      solveProblem(nameOfOGFile,cg,u,ps,psp,testInterpolant,plotOption);
    }
    else if( answer=="check interpolate" )
    {
      // solveProblem(nameOfOGFile,cg,u,ps,psp,3,plotOption)
      if( false )
      {
	Interpolant::testInterpolation( cg,1 );
      }
      else
      {
	aString label=nameOfOGFile;
        u.updateToMatchGrid(cg);
        checkInterpolation(u,label); 
      }
    }
    else if( answer=="test coarse from fine" )
    {
      solveProblem(nameOfOGFile,cg,u,ps,psp,testCoarseFromFine,plotOption);
    }
    else if( answer=="test interpolate refinements" )
    {
      checkArrayIDs("Before interpolate refinements");
      real mem=Overture::getCurrentMemoryUsage();
      printf("Before interpolate refinements: myid=%i memory usage=%g\n",myid,mem);

      // This options uses two CompositeGrids 
      solveProblem(nameOfOGFile,cga[0],ua[0],ps,psp,testInterpolateRefinements,plotOption,cga,ua);

      checkArrayIDs("After interpolate refinements");
      mem=Overture::getCurrentMemoryUsage();
      printf("After interpolate refinements: myid=%i memory usage=%g\n",myid,mem);
    }
    else if( answer=="extrapolate interpolation neighbours" )
    {

      solveProblem(nameOfOGFile,cg,u,ps,psp,testExtrapolateInterpolationNeighbours,plotOption);

//       u.updateToMatchGrid(cg);
//       CompositeGridOperators cgop(cg);

//       if( false )
//       {
// 	for( int grid=cg.numberOfBaseGrids(); grid<cg.numberOfComponentGrids(); grid++ )
// 	{
// 	  displayMask(cg[grid].mask(),sPrintF("mask on grid %i",grid));
// 	}
//       }
      
//       u.setOperators(cgop);
//       u=1.;
//       u.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
    }
    else if( answer=="grid plot" )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="contour plot" )
    {
      if( u.isNull() )
      {
	u.updateToMatchGrid(cg);
	u=1.;
      }
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::contour(ps,u,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="erase" )
    {
      ps.erase();
      plotGrid=false;
    }
    else if( answer=="plot parallel distribution" )
    {
      realCompositeGridFunction pd(cg);
      Index I1,I2,I3;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.dimension(),I1,I2,I3);

#ifdef USE_PPP
	realSerialArray pdLocal; getLocalArrayWithGhostBoundaries(pd[grid],pdLocal);
	bool ok=ParallelUtility::getLocalArrayBounds(pd[grid],pdLocal,I1,I2,I3);
	if( !ok ) continue;
#else
	realSerialArray & pdLocal = pd[grid];
#endif
	pdLocal=myid;
      }
	      
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,"Parallel distribution");
      PlotIt::contour(ps,pd,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
      sScanF(answer,"%i",&ogen.debug);
      printF(" ogen.debug = %i\n",ogen.debug);
    }
    else if( answer=="interpolation width 2" )
    {
      int width=2;
      cg.changeInterpolationWidth(width);
    }
    else if( answer=="output check file" )
    {
      aString checkFileName="refine.check";
      FILE *check = fopen((const char*)checkFileName,"w");
      aString label=nameOfOGFile;
      outputInterpolationData( cg,label,check);
      fclose(check);
      printF("Interpolation data written to the check file %s\n",(const char*)checkFileName);
    }
    else if( answer=="save the grid" )
    {
      aString fileName,gridName;
      ps.inputString(fileName,"Enter the name of the file");
      for( ;; )
      {
	ps.inputString(gridName,"Save the grid under which name?");
	if( gridName=="." )
	  ps.outputString("Error: do not choose `.' as a name");
	else
	  break;
      }      
      // printF("grid before\n");
      // printInfo(cg,1);

      real time=getCPU();
      cg.saveGridToAFile(fileName,gridName);
      time=getCPU()-time;
      printF(" refine: time to save the grid in a file = %8.2e(s)\n",time);
      
//       dataFile.mount(fileName,"I");

//       int streamMode=1;
//       dataFile.put(streamMode,"streamMode");
//       if( !streamMode )
// 	dataFile.setMode(GenericDataBase::noStreamMode);

//       // first destroy any big geometry arrays: (but not the mask)
//       cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );

// //       for( grid=0; grid<cg.numberOfGrids(); grid++ )
// //         cg[grid]->computedGeometry |=  MappedGrid::THEmask;

      
//       cg.put(dataFile,gridName);

//       dataFile.unmount();

      if( false )
      {
	CompositeGrid cg2;
	printF("read the grid back in\n");
        HDF_DataBase dataFile;
	dataFile.mount(fileName,"R");
	cg2.get(dataFile,gridName);
	cg2.update(MappedGrid::THEmask);
	// cg2.setMaskAtRefinements();

	// printInfo(cg2,1);

	ps.erase();
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	psp.set(GI_TOP_LABEL,"after reading back in");
	PlotIt::plot(ps,cg2,psp);
      }
      
    }
    else
    {
      printF("Unknown response: [%s] \n",(const char*)answer);
      ps.stopReadingCommandFile();
    }
  }
  

  Overture::finish();          
  return 0;
}
