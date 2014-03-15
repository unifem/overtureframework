//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"
#include "multigrid1.h"
#include "blockTridiag2d.h"
#include "display.h"
#include "LineMapping.h"
#include "Square.h"
#include "BoxMapping.h"
#include "DataPointMapping.h"
#include "TridiagonalSolver.h"
#include "StretchedSquare.h"

static int TEST_RESIDUAL=0;

multigrid::
multigrid(void)
{
  //Default constructor
  initializeParameters();
  

}

int multigrid::
initializeParameters()
{
  debugFile = fopen("elliptic.debug","w" );      // Here is the debug file

  debug=0;

  ps=0;
  
  useNewStuff=TRUE;

  map=NULL;
  mg=NULL;
  operators=NULL;
  u=NULL;
  rhs=NULL;
  Source=NULL;
  w=NULL;
  rBoundary=NULL;
  
  numberOfPointsOfAttraction=0;
  numberOfLinesOfAttraction=0;

  numberOfPeriods=0;
  residualTolerance=1.e-3;  // relative convergence criteria for the maximum residual
  maximumNumberOfIterations=10;
  smoothingMethod=jacobiSmooth;

  omega1=0.1;
  lambda=0.0;
  useBlockTridiag=0;

  boundarySpacing.redim(2,3);
  boundarySpacing=.1;
  
  maximumResidual=previousMaximumResidual=REAL_MIN;

//  numberOfPointattractions=0;
//  numberOfIlineattractions==0;
//  numberOfJlineattractions==0;


  return 0;
}



void multigrid::
setup(Mapping & mapToUse )
// ==========================================================================================
//  /Description:
//     Setup the EllipticGridGenerator
// ==========================================================================================
{
  userMap=&mapToUse;
  
  domainDimension=userMap->getDomainDimension();
  rangeDimension=userMap->getRangeDimension();

  maximumResidual=previousMaximumResidual=REAL_MIN;

  Rx=Range(0,rangeDimension-1);
  if( rangeDimension==1)
     omega=2.0/3.0;
  else if( rangeDimension==2 )
     omega=4.0/5.0;
  else 
    omega=4.0/5.0;   // **** what should this be ?? 0.5;

  maximumNumberOfLevels=1;
  int dim[3] ={1,1,1};
  for( int axis=0; axis<domainDimension; axis++ )
  {
    dim[axis]=userMap->getGridDimensions(axis);
    int num=dim[axis]-1;
    int numLevels=1;
    while( num>2 && num % 2 ==0 )  // keep at most 2 cells on the finest grid.
    {
      num/=2;
      numLevels++;
    }
    maximumNumberOfLevels=axis==0 ? numLevels : min(maximumNumberOfLevels,numLevels);
  }
  numberOfLevels= maximumNumberOfLevels;
  
  alpha=0.0;

  // map : pointer to the unit line, unit square or unit cube mapping.
  delete map;
  if( domainDimension==1 )
    map= new LineMapping();
  else if( domainDimension==2 ) 
    map = new SquareMapping();
  else  
    map = new BoxMapping();

  niter = 1;
  mg    = new MappedGrid[numberOfLevels];

  operators = new MappedGridOperators[numberOfLevels];
  
  u     = new realMappedGridFunction[numberOfLevels];
  rhs   = new realMappedGridFunction[numberOfLevels];  
  w     = new realMappedGridFunction[numberOfLevels];

  Source= new realArray[numberOfLevels];

  rBoundary = new realArray[numberOfLevels];

  dx.redim(3,numberOfLevels);
  dx=1.;

  boundaryCondition.redim(2,3);
  boundaryCondition=dirichlet;

  numberOfCoefficients = rangeDimension==1 ? 1 : rangeDimension==2 ? 3 : 6;

  int i,j,dimension[3];

  Range all;
  // Initialize working arrays
  gridIndex.redim(2,3,numberOfLevels);
  gridIndex=0;
  
  for( axis=0; axis<domainDimension; axis++ )
  {
    map->setGridDimensions(axis,dimension[axis]);
    if( userMap->getIsPeriodic(axis) )
    {
      map->setIsPeriodic(axis, userMap->getIsPeriodic(axis));
      boundaryCondition(0,axis)=-1;
      boundaryCondition(1,axis)=-1;
    }
  }

  // initialize variables at each multigrid level.
  for( i=0; i<numberOfLevels; i++ )
  {
     
    for( axis=0; axis<domainDimension; axis++ )
    {
      const int numberOfGridPoints= (dim[axis]-1)/pow(2,i)+1;
      map->setGridDimensions(axis,numberOfGridPoints);
    }
    mg[i]=MappedGrid(*map);
    mg[i].update();                                      // ******* should not be repeated *******

    rBoundary[i] = mg[i].vertex();  // holds unit square coords for boundary points.
    
    // display(mg[i].vertex(),sPrintF(buff,"mg[%i].vertex()",i));
    
    u[i].updateToMatchGrid(mg[i],all,all,all,rangeDimension);
    u[i]=0.0;

    operators[i].updateToMatchGrid(mg[i]);
    u[i].setOperators(operators[i]);
    
    // gridIndex : interior points plus boundaries where interior equations are applied.
    for( axis=0; axis<domainDimension; axis++ )
    {
      if( userMap->getIsPeriodic(axis) )
      {
        gridIndex(Start,axis,i)=mg[i].gridIndexRange(Start,axis);
        gridIndex(End  ,axis,i)=mg[i].gridIndexRange(End  ,axis);
      }
      else
      {
        gridIndex(Start,axis,i)=mg[i].gridIndexRange(Start,axis)+1;
        gridIndex(End  ,axis,i)=mg[i].gridIndexRange(End  ,axis)-1;
      }
    }
    

    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    getIndex(mg[i].gridIndexRange(),J1,J2,J3,1);  // add one ghost point
    Source[i].redim(J1,J2,J3,rangeDimension);
    Source[i]=0.0;

    rhs[i].updateToMatchGrid(mg[i],all,all,all,rangeDimension);
    rhs[i]=0.0;
    if (i>0)
    {
      w[i].updateToMatchGrid(mg[i],all,all,all,rangeDimension);
      w[i]=0.0;
    }
    for( int axis=0; axis<rangeDimension; axis++ )
      dx(axis,i)=mg[i].gridSpacing(axis);

  }

  // set the dB boundary layer thickness to default.

/* -----
   realArray x;
   x=userMap->getGrid();
   Range I(0,ib), J(0,jb);
   dS(0,0) = max(sqrt((x(1,J,0,0)-x(0,J,0,0))*(x(1,J,0,0)-x(0,J,0,0))+
		      (x(1,J,0,1)-x(0,J,0,1))*(x(1,J,0,1)-x(0,J,0,1))));
   dS(1,0) = max(sqrt((x(ib,J,0,0)-x(ib-1,J,0,0))*(x(ib,J,0,0)-x(ib-1,J,0,0))+
		      (x(ib,J,0,1)-x(ib-1,J,0,1))*(x(ib,J,0,1)-x(ib-1,J,0,1))));
   dS(0,1) = max(sqrt((x(I,1,0,0)-x(I,0,0,0))*(x(I,1,0,0)-x(I,0,0,0))+
		      (x(I,1,0,1)-x(I,0,0,1))*(x(I,1,0,1)-x(I,0,0,1))));
   dS(1,1) = max(sqrt((x(I,jb,0,0)-x(I,jb-1,0,0))*(x(I,jb,0,0)-x(I,jb-1,0,0))+
		      (x(I,jb,0,1)-x(I,jb-1,0,1))*(x(I,jb,0,1)-x(I,jb-1,0,1))));
  dB=dS;
---- */


}

multigrid::
~multigrid()
{
  fclose(debugFile);

  delete map;

  delete [] mg;
  delete [] operators;
  delete [] u;
  delete [] rhs;
  delete [] Source;
  delete [] w;
  delete [] rBoundary;
}

realArray multigrid::
SignOf(realArray & uarray)
{
  realArray u1;

  u1.redim(uarray);
  u1=0.0;
  where(uarray>0.0) u1=1.0;
  elsewhere(uarray<0.0) u1=-1.0;

  return u1;
}






int multigrid::
plot( const RealMappedGridFunction & v, const aString & label )
//===========================================================================
//   Plot a grid function.
//===========================================================================
{
  if( debug & 2 )
    display(v,label,debugFile);

  if( ps!=NULL )
  {
    ps->erase();
    psp.set(GI_TOP_LABEL,label);
    ps->contour(v,psp);
  }
  return 0;
}


int multigrid::
update(DataPointMapping & dpm,
       GenericGraphicsInterface *gi_ /* = NULL */, 
       GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
//===========================================================================
// /Description:
//    Prompt for changes to parameters and compute the grid.
// /dpm (input) : build this mapping with the grid.
// /gi (input) : supply a graphics interface if you want to see the grid as it
//    is being computed.
// /parameters (input) : optional parameters used by the graphics interface.
//\end{EllipticTransformInclude.tex}
//===========================================================================
{
//  PlotStuff & gi = (PlotStuff &)(*gi_);  // *** need to check if this is safe.
  GenericGraphicsInterface & gi = *gi_; 
  ps=&gi;

  aString menu[] = 
    {
      "generate grid",
      ">convergence tolerance",
        "residual tolerance",
      "<>attraction",
        "point attraction",
        "line attraction",
        "plane attraction",
      "<elliptic boundary conditions",
      ">project",
        "project onto original mapping",
        "do not project onto original mapping",
      "<reset elliptic transform",
      ">parameters",
        "source interpolation coefficient",
        "order of interpolation",
        "change resolution for elliptic grid",
      "<>multigrid",
        ">smoothing method",
	  "Jacobi",
	  "red black",
	  "line",
	  "zebra",
        "<maximum number of iterations",
	"number of multigrid levels",
        ">parameters",
  	  "smoother relaxation coefficient",
	  "use block tridiagonal solver",
	  "do not use block tridiagonal solver",
	  "source relaxation coefficient",
	  "source interpolation power",
        "< ",
      "<useNewStuff",
      "change plot parameters",
      "plot residual",
      "plot control function",
      "test orthogonal",
      "debug",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Transform a Mapping by Elliptic Grid Generation",
      "transform which mapping? : 		choose the mapping to transform",
      "elliptic smoothing : 			smooth out grid with elliptic transform",
      "change resolution for elliptic grid:	change iDim,jDim for elliptic solver",
      "set Poisson i-line sources: 		set line sources for constant i",
      "set Poisson j-line sources: 		set line sources for constant j",
      "set Poisson point sources: 		set point sources in field",
      "set GRID boundary conditions: 		set b.c's for elliptic solver",
      "set source interpolation coefficient     set lambda for interpolation of B. sources terms",
      "set number of periods: 			make sources periodic",
      "project onto original mapping (toggle)",
      "reset elliptic transform                 start iterations from scratch",
      "set order of interpolation               order of interpolation for data point mapping",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  bool plotObject=TRUE;
  aString answer,line,answer2; 
  gi.appendToTheDefaultPrompt("elliptic>"); // set the default prompt

  for( int it=0;; it++ )
  {

    if( it==0 && plotObject )
      answer="plotObject";  // plot first time through
    else
      gi.getMenuItem(menu,answer);
    
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="generate grid" )
    {
      applyMultigrid();
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      gi.plot(dpm,(GraphicsParameters&)psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="debug" )
    {
      gi.inputString(line, sPrintF(buff,"Enter debug (4=plot) (current = %i)",debug));
      if ( line != "")
        sScanF( line,"%i",&debug);
      cout << "debug=" << debug << endl;
    }
    else if( answer=="useNewStuff" )
    {
      useNewStuff=!useNewStuff;
      cout << "useNewStuff=" << useNewStuff << endl;
    }
    else if( answer=="residual tolerance" )
    {
      gi.inputString(line, sPrintF(buff,"Enter the residual tolerance (current = %e)",residualTolerance));
      if ( line != "")
        sScanF( line,"%e",&residualTolerance);
      if( residualTolerance<REAL_EPSILON )
      {
	residualTolerance=REAL_EPSILON*10.;
        printf("ERROR: residualTolerance to small. Setting to %e \n",residualTolerance);
      }
    }
    else if( answer=="point attraction" )
    {
      
      printf("A point of attraction is defined by a source terms of the form \n"
             "  P_m =  - a sign( r_m-c_m ) exp( - b | r - c| ) m=0,1[,2] \n"
             " In 2D [3D] there are 2 [3] source terms, where \n"
             "    a = weight   (example: a=5.)\n"
             "    b = exponent (example: b=5.) \n"
             "    c = location of the point source (c0,c1[,c2]), each c_m in the range [0,1] \n");

      numberOfPointsOfAttraction=0;
      gi.inputString(line,sPrintF(buff,"Enter the number of points of attraction"));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfPointsOfAttraction);
        if( numberOfPointsOfAttraction>0 )
	{
	  pointAttractionParameters.redim(5,numberOfPointsOfAttraction); pointAttractionParameters=0.;
	  for (int n=0; n<numberOfPointsOfAttraction; n++)	       
	  {
	    if( domainDimension==1 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));
	    else if( domainDimension==2 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0,c1 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));
	    else if( domainDimension==3 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0,c1,c2 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));

	    sScanF(line,"%e %e %e %e %e %e",
		   &pointAttractionParameters(0,n),
		   &pointAttractionParameters(1,n),
		   &pointAttractionParameters(2,n),
		   &pointAttractionParameters(3,n),
		   &pointAttractionParameters(4,n));
	  }
	}
      }
    }
    else if( answer=="line attraction" )
    {
      printf("A line of attraction is defined by a source term of the form \n"
             "  P_m =  - a sign( r_m-c ) exp( - b |r_m-c| ) \n"
             " where \n"
             "    m = direction (a coordinate direction 0,1, or 2)\n"
             "    a = weight    (example: a=5.)\n"
             "    b = exponent  (example: b=5.)\n"
             "    c = location along coordinate m, in the range [0,1] \n");

      numberOfLinesOfAttraction=0;
      gi.inputString(line,sPrintF(buff,"Enter the number of lines of attraction"));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfLinesOfAttraction);
        if( numberOfLinesOfAttraction>0 )
	{
	  lineAttractionDirection.redim(numberOfLinesOfAttraction); lineAttractionDirection=0;
	  lineAttractionParameters.redim(3,numberOfLinesOfAttraction); lineAttractionParameters=0.;
	  for (int n=0; n<numberOfLinesOfAttraction; n++)	       
	  {
	    gi.inputString(line,sPrintF(buff,"Enter m,a,b,c for P(r) = -a sign( r_m-c ) exp( - b |r_m-c| )"));
	    sScanF(line,"%i %e %e %e",&lineAttractionDirection(n),
		   &lineAttractionParameters(0,n),
		   &lineAttractionParameters(1,n),
		   &lineAttractionParameters(2,n));
	  }
	}
      }
    }
    else if( answer=="plane attraction" )
    {
      printf("Sorry, this option not implemented\n");
    }
    else if( answer=="set number of periods (for sourced problems)") // ***** remove this ****
    {
      gi.inputString(line,sPrintF(buff,"Enter number of periods for resolving"
		     " periodic problem with source (default==%d):",numberOfPeriods));
      if (line!="")
	 sScanF(line,"%d",&numberOfPeriods);
      if (numberOfPeriods%2==0) 
	 numberOfPeriods++;
    }
    else if( answer=="elliptic boundary conditions" )
    {

/* ----
      gi.outputString("Enter Boundary conditions:  (bc==1-->Dirichlet, bc==2-->Normal");
      gi.outputString("                             bc==3-->Combined, bc==-1-->Periodic");
      gi.inputString(line,sPrintF(buff,"Enter bc(0,0), bc(1,0), bc(0,1), bc(1,1)"
		     "(default = (%d,%d,%d,%d)): ",boundaryCondition(0,0),boundaryCondition(1,0),boundaryCondition(0,1),boundaryCondition(1,1)));
      if( line!="" ) 
	sScanF( line,"%d %d %d %d",&boundaryCondition(0,0),&boundaryCondition(1,0),&boundaryCondition(0,1),&boundaryCondition(1,1));
      for (int i=0;i<numDim;i++)
       for (int j=0;j<numDim;j++)
	if (boundaryCondition(i,j)==3)	{
	  gi.inputString(line,sPrintF(buff,"Enter thickness of boundary layer dS(%d,%d) "
     			 "[default = %e] :",i,j,dS(i,j)));
	  if ( line!="" ){
	    sScanF( line,"%e",&dS(i,j));

	    if (j==0)
	    	dB(i,j)=dS(i,j)/di;
	    else
		dB(i,j)=dS(i,j)/dj;
	  }
        }
------- */
      gi.appendToTheDefaultPrompt("bc>"); 
      aString bcMenu[] =
      {
        "left   (side=0,axis=0)",
        "right  (side=1,axis=0)",
        "bottom (side=0,axis=1)",
        "top    (side=1,axis=1)",
        "back   (side=0,axis=2)",
        "front  (side=1,axis=2)",
        "all sides",
        "exit",
        ""
      };
      aString bcChoices[] = 
      {
        "dirichlet",
        "slip orthogonal",
        "noSlip orthogonal and specified spacing",
        "noSlip orthogonal",
        "specified spacing",
        "no change",
        ""
      };

      for( ;; )
      {
	int sideChosen = gi.getMenuItem(bcMenu,answer,"choose a menu item");
	if( answer=="exit" )
	{
	  break;
	}
	else if( sideChosen<0 )
	{
	  gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer2) );
	  gi.stopReadingCommandFile();
	  break;
	}
	else if( sideChosen>=0 )
	{
          Range A,S;
          if( answer=="all sides" )
	  {
            A=Range(0,domainDimension-1);
	    S=Range(0,1);
	  }
	  else
	  {
            int side=sideChosen %2;
	    int axis=sideChosen/2;
	    S=Range(side,side);
	    A=Range(axis,axis);
	  }
	  int itemChosen = gi.getMenuItem(bcChoices,answer2,"choose a boundary condition");
          itemChosen++;
          if( itemChosen>0 )
	  {
            where( boundaryCondition(S,A)>=0 )
              boundaryCondition(S,A)=(BoundaryConditionTypes)itemChosen;
            for( int side=S.getBase(); side<=S.getBound(); side++ )
	    {
	      for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
	      {
		if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing || 
                    boundaryCondition(side,axis)==specifiedSpacing )
		{
		  gi.inputString(line, sPrintF(buff,"Enter the spacing for (side,axis)=(%i,%i) (current = %e)",
					       side,axis,boundarySpacing(side,axis)));
		  if ( line != "")
		    sScanF( line,"%e",&boundarySpacing(side,axis));
		  if( boundarySpacing(side,axis)<=0. )
		  {
		    boundarySpacing(side,axis)=REAL_EPSILON*10.;
		    printf("ERROR: boundarySpacing value is <=0 ! Setting to %e \n",boundarySpacing(side,axis));
		  }
		}
	      }
	    }
	  }
	  else
	  {
	    gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer2) );
	    gi.stopReadingCommandFile();
	    break;
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();  // reset

      stretchTheGrid(*userMap);
      defineBoundaryControlFunction();
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.plot(dpm,(GraphicsParameters&)psp);
      
    }
    else if( answer=="project onto original mapping" )
    {
    }
    else if( answer=="do not project onto original mapping" )
    {
    }
    else if( answer=="reset elliptic transform" )
    {
      startingGrid(userMap->getGrid());
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      gi.plot(dpm,(GraphicsParameters&)psp);
    }
    else if( answer=="source interpolation coefficient" )
    {
      gi.inputString(line, sPrintF(buff,"Enter the value of lambda  (default = %g)",lambda));
      if ( line != "")
        sScanF( line,"%f",&lambda);
    }
    else if( answer=="order of interpolation" )
    {
    }
    else if( answer=="change resolution for elliptic grid" )
    {
    }
    else if( answer=="Jacobi" ||answer=="jacobi" )
    {
      smoothingMethod=jacobiSmooth;
      omega=4./5.; // ***** 2d/3d ******
    }
    else if( answer=="red black" )
    {
      smoothingMethod=redBlackSmooth;
      omega=1.;
    }
    else if( answer=="line" )
    {
      smoothingMethod=lineSmooth;
      omega=1.;
    }
    else if( answer=="zebra" )
    {
      smoothingMethod=zebraSmooth;
      omega=1.;
    }
    else if( answer=="maximum number of iterations" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the maximum number of iterations (default=%i): ",
           maximumNumberOfIterations));
      if( line != "" )
      {
	sScanF(line,"%i",&maximumNumberOfIterations);
      }
    }
    else if( answer=="number of multigrid levels" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the number of levels (current==%i,max=%i): ",numberOfLevels,
              maximumNumberOfLevels));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfLevels);
	if( numberOfLevels>maximumNumberOfLevels)
        {
	  gi.outputString("Error:: Too big. Using the maximum");
	  numberOfLevels=maximumNumberOfLevels;
	}
      }
    }
    else if( answer=="smoother relaxation coefficient" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the relaxation coefficient (default=%f): ",omega));
      if( line != "" ) sScanF(line,"%e", &omega);
    }
    else if( answer=="use block tridiagonal solver" )
    {
      useBlockTridiag=1;
      if( smoothingMethod==3 || smoothingMethod==4 )
	printf(" Will use block tridiagonal solver\n");
    }
    else if( answer=="do not use block tridiagonal solver" )
    {
      useBlockTridiag=0;
      if( smoothingMethod==3 || smoothingMethod==4 )
	printf(" Will not use block tridiagonal solver\n");
    }
    else if( answer=="source relaxation coefficient" )
    {
    }
    else if( answer=="source interpolation power" )
    {
      gi.inputString(line,sPrintF(buff,"enter the interpolation exponent (default=%e): ",lambda));
      if (line != "")
	sScanF(line,"%e", &lambda);
    }
    else if( answer=="show parameters" )
    {
      printf("--------------- parameters for EllipticGridGeneration ------------------------\n");
      printf("number of multigrid levels=%i (maximum=%i)\n",numberOfLevels,maximumNumberOfLevels);
      printf("residualTolerance = %e \n",residualTolerance);
      
      for( int axis=0; axis<=domainDimension; axis++ )
      {
	for( int side=0; side<=0; side++ )
	{
	  printf(" boundaryCondition(side=%i,axis=%i) = %s \n",side,axis,
		 boundaryCondition(side,axis)==-1 ? "periodic" : 
                 boundaryCondition(side,axis)==dirichlet ? "dirichlet" :
                 boundaryCondition(side,axis)==slipOrthogonal ? "slip orthogonal" :
		 boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing ? 
 		                              "noSlip orthogonal and specified spacing" :
                 boundaryCondition(side,axis)==noSlipOrthogonal ? "noSlip orthogonal" :
                 boundaryCondition(side,axis)==specifiedSpacing ? "specified spacing" :
                 "unknown");
	}
      }
      if( numberOfLinesOfAttraction>0 )
      {
        printf("Attraction to a line: P_m =  - a sign( r_m-c ) exp( - b |r_m-c| ) \n"
               "  point    m     a     b     c \n");
	for(int n=0; n<numberOfPointsOfAttraction; n++)	       
	{
          printf("%6i %6i %6.2e %6.2e %6.2e  \n",
		 lineAttractionDirection(n),
                 lineAttractionParameters(0,n),
                 lineAttractionParameters(1,n),
                 lineAttractionParameters(2,n));
	}
      }
      if( numberOfPointsOfAttraction>0 )
      {
	printf("Attraction to a point: P_m =  - a sign( r_m-c_m ) exp( - b | r - c| ) m=0,1[,2] \n"
               "  point    a     b     c_0     c_1    c_2 \n");
	for(int n=0; n<numberOfPointsOfAttraction; n++)	       
	{
          printf("%6i %6.2e %6.2e %6.2e %6.2e %6.2e \n",
		 pointAttractionParameters(0,n),
		 pointAttractionParameters(1,n),
		 pointAttractionParameters(2,n),
		 pointAttractionParameters(3,n),
		 pointAttractionParameters(4,n));
	}
      }
    }
    else if( answer=="plot residual" )
    {
      Range all;
      RealMappedGridFunction res(mg[0],all,all,all,Rx);
      getResidual(res,0);
      gi.erase();
      gi.contour(res);
      gi.erase();
    }
    else if( answer=="plot control function" )
    {
      Range all;
      RealMappedGridFunction source(mg[0],all,all,all,Rx);
      source=Source[0];
      gi.erase();
      gi.contour(source);
      gi.erase();
    }
    else if( answer=="test orthogonal" )
    {
      printf("Apply orthogonal BC to level 0\n");
      applyBoundaryConditions(0,u[0]);
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      gi.plot(dpm,(GraphicsParameters&)psp);
    }
    else if( answer=="plotObject" )
    {
    }
    else if( answer=="change plot parameters" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      gi.erase();
      gi.plot(dpm,(GraphicsParameters&)psp);
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}


void multigrid::
getResidual(realArray & resid1, 
            const int & i )
// old interface
{
  RealArray coeff;
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),i),J1,J2,J3);

  getResidual(resid1,i,Jv,coeff);
}


/* ----
int multigrid::
make2Power( int n)
{
  int i=0;
  if ((n<=1)&&(i==0)) return (1);
  else
  {
    while (!(((pow(2,i)+1)<n)&&((pow(2,i+1)+1)>=n)))
      i ++;
    return(int(pow(2,i+1))+1);
  }
}
---- */

int multigrid::
applyBoundaryConditions(const int & level,
                      RealMappedGridFunction & uu )
// =================================================================================
// /Description:
//    Apply boundary conditions. 
//
//  {\bf slip orthogonal boundary:}
//
//  Adjust the points on the boundary to make the grid orthogonal al the boundary.
//  To do this we compute the amount to shift the point in the unit square coordinates.
//  We then recompute the $\xv$ coordinates by evaluating the Mapping on the boundary.
//
//  Suppose that  $r,s$ are the coordinates tangential to the boundary and $t$ is the coordinate
//  normal to the boundary. Let $\xv_0$ be the grid point on the boundary that we
// want to adjust and let $\xv_1$ be the grid point one line away from the boundary.
//  Then we want to choose a new boundary 
// point $\xv(r,s)$ so that
// \begin{align*}
//    (\xv-\xv_1) \cdot \xv_r &= 0
//    (\xv-\xv_1) \cdot \xv_s &= 0
// \end{align*}
// We use the approximation
// \[
//    \xv(r,s) \approx \xv_0 + \Delta r \xv_r^0 + \Delta s \xv_s^0 
// \]
// and thus the equation for $(\Delta r,\Delta s)$ is
// \begin{align*}
//   \begin{bmatrix}  
//       \xv_r^0\cdot\xv_r^0 & \xv_r^0\cdot\xv_s^0 \\
//       \xv_r^0\cdot\xv_s^0 & \xv_s^0\cdot\xv_s^0 
//   \end{bmatrix}  
//   \begin{bmatrix} \Delta r & \Delta s \end{bmatrix} =
//   \begin{bmatrix} (\xv_1-\xv_0)\cdot xv_r^0 & (\xv_1-\xv_0)\cdot xv_s^0 \end{bmatrix}
// \end{align*}
// with solution
// \begin{align*}
//   \begin{bmatrix} \Delta r & \Delta s \end{bmatrix} =
//   {1\over \xv_r^0\cdot\xv_r^0 \xv_s^0\cdot\xv_s^0 - (\xv_r^0\cdot\xv_s^0)^2 }
//    \begin{bmatrix}  
//       \xv_s^0\cdot\xv_s^0 &-\xv_r^0\cdot\xv_s^0 \\
//      -\xv_r^0\cdot\xv_s^0 & \xv_r^0\cdot\xv_r^0 
//   \end{bmatrix}  
//   \begin{bmatrix} (\xv_1-\xv_0)\cdot xv_r^0 & (\xv_1-\xv_0)\cdot xv_s^0 \end{bmatrix}
// \end{align*}
// In 2D this reduces to
// \[
//  \Delta r = {1\over \xv_r^0\cdot\xv_r^0} (\xv_1-\xv_0)\cdot xv_r^0
// \]
// =================================================================================
{
  if( level>0 || rangeDimension<2 )
    return 0;
  
  RealArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
  is[0]=is[1]=is[2]=0;



  bool applySlipOrthogonal = FALSE;
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
    applySlipOrthogonal = applySlipOrthogonal || 
                       boundaryCondition(0,axis)==slipOrthogonal || 
                       boundaryCondition(1,axis)==slipOrthogonal;

  if( applySlipOrthogonal )
  {
    // printf("ERROR: applyOrthogonalSlipBC not implemented yet\n");
    RealArray & rB = rBoundary[level];

    Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];
    
    for( axis=0; axis<rangeDimension; axis++ )
    {
      const int axisp1 = (axis+1) % rangeDimension;
      const int axisp2 = (axis+2) % rangeDimension;
      is[axisp1]=1;
      for( int side=Start; side<=End; side++ )
      {
        if( boundaryCondition(side,axis)==slipOrthogonal )
	{
          const int extra=-1; // do not adjust end points. These must remain at 0. and 1.
	  getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3,extra);
          getGhostIndex(mg[level].gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1,extra); // first line inside
	  
          RealArray du(I1,I2,I3,Rx);
          du=u1(Ip1,Ip2,Ip3,Rx)-u1(I1,I2,I3,Rx);
	  RealArray ur(I1,I2,I3,Rx); 
	  if( axisp1==0 )
	    ur=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  else if( axisp1==1 )
	    ur=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	  else
	    ur=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
          if( rangeDimension==2 )
	  {
	    RealArray & urDotDu=evaluate(du(I1,I2,I3,0)*ur(I1,I2,I3,0)+du(I1,I2,I3,1)*ur(I1,I2,I3,1));
	    RealArray & urDotUr=evaluate(SQR(ur(I1,I2,I3,0))+SQR(ur(I1,I2,I3,1)));
	    RealArray & dr = evaluate(urDotDu/urDotUr);
	    // we need the current r values of the boundary points
/* -----
            where( dr>.5*(rB(I1+is1,I2+is2,I3,axisp1)-rB(I1,I2,I3,axisp1)) )
	    {
              dr=.5*(rB(I1+is1,I2+is2,I3,axisp1)-rB(I1,I2,I3,axisp1));
	    }
            where( dr<.5*(rB(I1-is1,I2-is2,I3,axisp1)-rB(I1,I2,I3,axisp1)) )
	    {
              dr=.5*(rB(I1-is1,I2-is2,I3,axisp1)-rB(I1,I2,I3,axisp1));
	    }
----- */
            if( debug & 4 )
              display(rBoundary[level](I1,I2,I3,axisp1),
                sPrintF(buff,"applyOrthogonalSlipBC: rBoundary[level] on (axis,side)=(%i,%i) level=%i",
                 axis,side,level));
	    rB(I1,I2,I3,axisp1)+=dr;
            if( debug & 4 )
              display(dr,sPrintF(buff,"applyOrthogonalSlipBC: dr on (axis,side)=(%i,%i)",axis,side));
	    
	  }
	  else
	  {
	    RealArray us(I1,I2,I3,Rx); 
	    if( axisp2==0 )
	      us=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    else if( axisp2==1 )
	      us=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    else 
	      us=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    RealArray & urDotDu=evaluate(du(I1,I2,I3,0)*ur(I1,I2,I3,0)+
					 du(I1,I2,I3,1)*ur(I1,I2,I3,1)+
					 du(I1,I2,I3,2)*ur(I1,I2,I3,2));
	    RealArray & usDotDu=evaluate(du(I1,I2,I3,0)*us(I1,I2,I3,0)+
					 du(I1,I2,I3,1)*us(I1,I2,I3,1)+
					 du(I1,I2,I3,2)*us(I1,I2,I3,2));
	    RealArray & urDotUr=evaluate(SQR(ur(I1,I2,I3,0))+SQR(ur(I1,I2,I3,1)+SQR(ur(I1,I2,I3,2))));
	    RealArray & usDotUs=evaluate(SQR(us(I1,I2,I3,0))+SQR(us(I1,I2,I3,1)+SQR(us(I1,I2,I3,2))));
	    RealArray & urDotUs=evaluate(ur(I1,I2,I3,0)*us(I1,I2,I3,0)+
                                         ur(I1,I2,I3,1)*us(I1,I2,I3,1)+
                                         ur(I1,I2,I3,2)*us(I1,I2,I3,2));
            RealArray & detInverse = evaluate(1./(urDotUr*usDotUs-SQR(urDotUs)));
	    RealArray & dr = evaluate((usDotUs*urDotDu-urDotUs*usDotDu)*detInverse);
	    RealArray & ds = evaluate((usDotUs*usDotDu-urDotUs*urDotDu)*detInverse);
	    
	    // we need the current r values of the boundary points
	    rB(I1,I2,I3,axisp1)+=dr;
	    rB(I1,I2,I3,axisp2)+=ds;
	  }
	  
          // we may have to put guards on rBoundary.
          if( debug & 4 )
  	    display(rBoundary[level](I1,I2,I3,Rx),
		  sPrintF(buff,"applyOrthogonalSlipBC: AFTER : rBoundary[level] on (axis,side)=(%i,%i) level=%i",
			  axis,side,level));

	  // userMap->mapGrid(rBoundary[level](I1,I2,I3,Rx),u2); // this doesn't work -- reshape view is wrong.

          RealArray r2(I1,I2,I3,Rx), u2(I1,I2,I3,Rx);
          r2=rB(I1,I2,I3,Rx);
	  userMap->mapGrid(r2,u2);
          u1(I1,I2,I3,Rx)=u2;
	  
	}
      } // end for side
      is[axisp1]=0;  // reset
    }
  }
  return 0;
}


int multigrid::
defineBoundaryControlFunction()
// =================================================================================
// /Description:
//    Determine the control functions on the boundary that impose
//  orthogonality and a specified grid spacing.
//
//  We compute the the boundary normal vector $\nv(r,s)$ to the boundary $t=t_0$. 
// Given a specified grid spacing $\Delta x$ we have 
// \[
//     \xv(r,s,t_0 \pm \Deta t) = \xv(r,s,t_0)  \pm \Delta x \nv(r,s)
// \]
// and thus we have the approximations  
// \begin{align*}
//     \xv_t(r,s,t_0) &= {\Delta x \over \Delta t} \nv(r,s) \\
//                    &= = \beta \nv(r,s)
// \end{align*}
// If we have a boundary layer stretching then we expect that the
// grid spacing will increase at an exponential rate in the normal direction,
// \[
//      \xv(r,s,t) \approx \xv(r,s,0) + C e^{\alpha t} \nv(r,s)
// \]
// for $t<< 1$.
// Thus the second derivative $\xv_tt$ will satisfy
// \[
//    \xv_tt(r,s,0) \approx \alpha \xv_t(r,s,0) = \alpha {\Delta x \over \Delta t} \nv(r,s)
// \]
// We can make a guess for $\xv_tt(r,s,0)$ given a guess for $\alpha$ 
//
//
// Initial grid: If the user has requested a very small spacing near the boundary we can
// can explicitly stretch the initial grid to approximately statisfy the grid spacing.
// To do this we measure the actual grid spacing near each boundary that needs to be stretched.
// We then determine stretching functions.
// 
// 
// 
// 
// =================================================================================
{
  if( rangeDimension<2 )
    return 0;
  
  const int level=0;
  RealMappedGridFunction & uu = u[level];
  RealArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
  is[0]=is[1]=is[2]=0;

  bool applyNoSlipOrthogonalAndSpecifiedSpacing = FALSE;
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
    applyNoSlipOrthogonalAndSpecifiedSpacing = applyNoSlipOrthogonalAndSpecifiedSpacing || 
                       boundaryCondition(0,axis)==noSlipOrthogonalAndSpecifiedSpacing || 
                       boundaryCondition(1,axis)==noSlipOrthogonalAndSpecifiedSpacing;

  if( applyNoSlipOrthogonalAndSpecifiedSpacing )
  {
    // position the ghost point to be a specified distance in the normal direction
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

    for( axis=0; axis<rangeDimension; axis++ )
    {
      // Here are the tangential direction(s)
      const int axisp1 = (axis+1) % rangeDimension;
      const int axisp2 = (axis+2) % rangeDimension;
      for( int side=Start; side<=End; side++ )
      {
        if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
	{
	  getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3);    // boundary line
          getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1); // first ghost line.
	  
          // first compute the outward unit normal : grad_x t
	  RealArray normal(I1,I2,I3,Rx);

          const int sgn = 2*side-1;  // multiply normal by this to be an outward normal

	  RealArray ur(I1,I2,I3,Rx); // tangential derivative
	  if( axisp1==0 )
	    ur=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  else if( axisp1==1 )
	    ur=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  else
	    ur=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  RealArray norm(I1,I2,I3);
	  if( rangeDimension==2 )
	  {
	    normal(I1,I2,I3,axis1)=-ur(I1,I2,I3,axis2); // this will be normal in the direction of increasing r_axis
	    normal(I1,I2,I3,axis2)= ur(I1,I2,I3,axis1);
	    norm=SQRT(SQR(normal(I1,I2,I3,axis1))+SQR(normal(I1,I2,I3,axis2)));
	  }
	  else
	  {
	    RealArray us(I1,I2,I3,Rx); 
	    if( axisp2==0 )
	      us=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    else if( axisp2==1 )
	      us=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    else 
	      us=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
            // this will be normal in the direction of increasing r_axis
            normal(I1,I2,I3,axis1)=ur(I1,I2,I3,axis2)*us(I1,I2,I3,axis3)-ur(I1,I2,I3,axis3)*us(I1,I2,I3,axis2);
            normal(I1,I2,I3,axis2)=ur(I1,I2,I3,axis3)*us(I1,I2,I3,axis1)-ur(I1,I2,I3,axis1)*us(I1,I2,I3,axis3);
            normal(I1,I2,I3,axis3)=ur(I1,I2,I3,axis1)*us(I1,I2,I3,axis2)-ur(I1,I2,I3,axis2)*us(I1,I2,I3,axis1);
	    norm=SQRT(SQR(normal(I1,I2,I3,axis1))+SQR(normal(I1,I2,I3,axis2))+SQR(normal(I1,I2,I3,axis3)));
	  }
          where( norm>0. ) // **** what to do about norm==0 ?
	  {
	    norm=(sgn*boundarySpacing(side,axis))/norm;
	  }
          printf("defineBoundaryControlFunction: setting ghost point on (side,axis)=(%i,%i) \n");
	  
	  for( int dir=0; dir<rangeDimension; dir++ )
            u1(Ig1,Ig2,Ig3,dir)=u1(I1,I2,I3,dir)+normal(I1,I2,I3,dir)*norm;
	}
      }
    }
  }

  return 0;
}

int multigrid::
stretchTheGrid(Mapping & mapToStretch)
// =================================================================================
// /Description:
//
//   Determine a starting guess for grids with stretched boundary layer spacing.
//
//  If the user has requested a very small spacing near the boundary we can
// can explicitly stretch the initial grid to approximately statisfy the grid spacing.
// To do this we measure the actual grid spacing near each boundary that needs to be stretched.
// We then determine use stretching functions to determine a new grid by composing a 
// stretched
// 
// 
// 
// 
// =================================================================================
{
  if( rangeDimension<2 )
    return 0;
  
  const int level=0;
  RealMappedGridFunction & uu = u[level];
  RealArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

  RealArray spacingRatio(2,3);
  spacingRatio=1.;
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
      {
	getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3);    // boundary line
	getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,-1); // first interior line
	  
	// determine the average current grid spacing
	RealArray uDiff(I1,I2,I3,Rx);
	uDiff = u1(I1,I2,I3,Rx)-u1(Ig1,Ig2,Ig3,Rx);
	real averageSpacing;
	if( rangeDimension==2 )
	  averageSpacing = sum( SQRT( SQR(uDiff(I1,I2,I3,0))+SQR(uDiff(I1,I2,I3,1)) ) );
	else
	  averageSpacing = sum( SQRT( SQR(uDiff(I1,I2,I3,0))+SQR(uDiff(I1,I2,I3,1))+SQR(uDiff(I1,I2,I3,2)) ) );
	int num=I1.getLength()*I2.getLength()*I3.getLength();
	averageSpacing/=max(1,num);
	  
	spacingRatio(side,axis) = averageSpacing/boundarySpacing(side,axis);
        printf("stretchTheGrid: (side,axis)=(%i,%i) average spacing = %e, spacingRatio=%e \n",
             side,axis,averageSpacing,spacingRatio(side,axis));

      }
    }
  }

  StretchedSquare stretchedSquare(domainDimension);
  for( axis=0; axis<rangeDimension; axis++ )
  {
    stretchedSquare.setGridDimensions(axis,mg[0].gridIndexRange(End,axis)-mg[0].gridIndexRange(Start,axis)+1);
    stretchedSquare.setIsPeriodic(axis,userMap->getIsPeriodic(axis));

    const bool StretchStart =spacingRatio(0,axis)>1.5;
    const bool StretchEnd   =spacingRatio(1,axis)>1.5;
     
    if( StretchStart || StretchEnd )
    {
      const int numberOfSidesToStretch=StretchStart+StretchEnd;
      stretchedSquare.stretchFunction(axis).setStretchingType(StretchMapping::inverseHyperbolicTangent);
      stretchedSquare.stretchFunction(axis).setNumberOfLayers(numberOfSidesToStretch);
      // assign stretching parameters for a*tanh(b(r-c))  : a=1., b=spacingRatio, c=0 or 1.
      for( int i=0; i<numberOfSidesToStretch; i++ )
      {
	int side = StretchStart ? i : i+1;
	stretchedSquare.stretchFunction(axis).setLayerParameters( side, 1.,spacingRatio(side,axis),side);
      }
    }
  }

  getIndex(mg[0].gridIndexRange(),I1,I2,I3);

  RealArray rStretch(I1,I2,I3,Rx);
  rStretch=stretchedSquare.getGrid();
  
  RealArray uStretch(I1,I2,I3,Rx);

  mapToStretch.mapGrid(rStretch,uStretch);  // ghost lines ?...
  
  // reassign the starting grid and all coarser levels.
  startingGrid(uStretch,mg[0].gridIndexRange());

  return 0;
}



int multigrid::
redBlack(const int & level, 
       RealMappedGridFunction & uu,
       const int & ichange )
// ===============================================================================================
//  /Description:
//     Jacobi smooth.
// /uu (input/output) : On input and output : current solution valid at all points, including periodic points.
// ===============================================================================================
{ 
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  RealArray & u1 = uu;

  // getSource(level,ichange);
  getControlFunctions(level);
  

  RealArray coeff(J1,J2,J3,numberOfCoefficients);
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  getIndex(mg[level].gridIndexRange(),K1,K2,K3);
  RealArray resid(K1,K2,K3,rangeDimension);  
  
  const int rb3End = domainDimension<3 ? 1 : 2;
  const int rb2End = domainDimension<2 ? 1 : 2;
  for( int rb3=0; rb3<rb3End; rb3++ )
  {
    for( int rb2=0; rb2<rb2End; rb2++ )
    {
      for( int rb1=0; rb1<2; rb1++ )
      {
	int shift1= (rb1+rb2+rb3) % 2;
	int shift2= rb1;
	int shift3= rangeDimension==3 ? rb2 : 0;
	K1=Range(J1.getBase()+shift1,J1.getBound(),2);  // stride 2
	K2=domainDimension>1 ? Range(J2.getBase()+shift2,J2.getBound(),2) : Range(J2);  // stride 2
	K3=domainDimension>2 ? Range(J3.getBase()+shift3,J3.getBound(),2) : Range(J3);

        RealArray ur,us,ut;
	ur = uu.r1(K1,K2,K3,Rx);  
        if( rangeDimension>1 )
  	  us = uu.r2(K1,K2,K3,Rx);
        if( rangeDimension>2 )
  	  ut = uu.r3(K1,K2,K3,Rx);

	getCoefficients(coeff,K1,K2,K3,ur,us,ut);


	bool computeCoeff=FALSE;
	getResidual( resid,level,Kv,coeff,computeCoeff); // **** fix *** only need resid j 

	const real dxSq=dx(0,level)*dx(0,level);
	const real dySq=dx(1,level)*dx(1,level);
	const real dzSq=dx(2,level)*dx(2,level);
	RealArray omegaOverDiag(K1,K2,K3);

	if( rangeDimension==2 )
	  omegaOverDiag =  (-.5*omega)/(coeff(K1,K2,K3,0)*(1./dxSq)+coeff(K1,K2,K3,1)*(1./dySq));
	else if( rangeDimension==3 )
	  omegaOverDiag=(-.5*omega)/(coeff(K1,K2,K3,0)*(1./dxSq)+coeff(K1,K2,K3,1)*(1./dySq)+
                                     coeff(K1,K2,K3,2)*(1./dzSq));
	else 
	  omegaOverDiag = (-.5*omega)/(coeff(K1,K2,K3,0)*(1./dxSq)) ;
  
	for( int j=0; j<rangeDimension; j++ )
	  u1(K1,K2,K3,j)+=resid(K1,K2,K3,j)*omegaOverDiag;

	periodicUpdate(uu);
      }
    }
  }
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);

  return 0;
}


int multigrid::
jacobi(const int & level, 
       RealMappedGridFunction & uu,
       const int & ichange )
// ===============================================================================================
//  /Description:
//     Jacobi smooth.
// /uu (input/output) : On input and output : current solution valid at all points, including periodic points.
// ===============================================================================================
{ 
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  RealArray & u1 = uu;

  getControlFunctions(level);

  RealArray coeff(J1,J2,J3,numberOfCoefficients);
  RealArray ur,us,ut;
  ur = uu.r1();
  if( rangeDimension>1 )
    us = uu.r2();
  if( rangeDimension>2 )
    ut = uu.r3();
  getCoefficients(coeff,J1,J2,J3,ur,us,ut);

  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  getIndex(mg[level].gridIndexRange(),K1,K2,K3);
  RealArray resid(K1,K2,K3,rangeDimension);  
  
  bool computeCoeff=FALSE;
  getResidual( resid,level,Jv,coeff,computeCoeff); 
  if( debug & 2 )
    printf("jacobi:START:omega=%f, residual=%e \n",omega,max(fabs(resid(J1,J2,J3,Rx))));

  const real dxSq=dx(0,level)*dx(0,level);
  const real dySq=dx(1,level)*dx(1,level);
  const real dzSq=dx(2,level)*dx(2,level);
  RealArray omegaOverDiag(J1,J2,J3);

  if( rangeDimension==2 )
    omegaOverDiag=(-.5*omega)/(coeff(J1,J2,J3,0)*(1./dxSq)+coeff(J1,J2,J3,1)*(1./dySq));
  else if( rangeDimension==3 )
    omegaOverDiag=(-.5*omega)/(coeff(J1,J2,J3,0)*(1./dxSq)+coeff(J1,J2,J3,1)*(1./dySq)+coeff(J1,J2,J3,2)*(1./dzSq));
  else 
    omegaOverDiag=(-.5*omega)/(coeff(J1,J2,J3,0)*(1./dxSq)) ;
  
  for( int j=0; j<rangeDimension; j++ )
    u1(J1,J2,J3,j)+=resid(J1,J2,J3,j)*omegaOverDiag;

  if( debug & 2 )
  {
    getResidual( resid,level,Jv,coeff,computeCoeff); 
    printf("jacobi:2    :omega=%f,   residual=%e \n",omega,max(fabs(resid(J1,J2,J3,Rx))));
  }

  periodicUpdate(uu);
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);
  
  if( debug & 2 )
  {
    getResidual( resid,level,Jv,coeff,computeCoeff); 
    printf("jacobi:END  :omega=%f,   residual=%e \n",omega,max(fabs(resid(J1,J2,J3,Rx))));
  }
  
  return 0;
}


int multigrid::
lineSmoother(const int & direction,
             const int & level,
             RealMappedGridFunction & uu )
// ===================================================================================================
// /Description:
//   Perform a line smooth.
// /direction (input) : perform a line smooth along this axis, 0,1, or 2.
// /uu (input/output) : On input and output : current solution valid at all points, including periodic points.
// ===================================================================================================
{
  TridiagonalSolver tri;   // fix this *****************************************

  assert( direction>=0 && direction<rangeDimension );
  
  RealArray & u1 = uu;

  // J1,J2,J3 : interior 
  // Jv[direction] : include boundaries except for right periodic boundary
  // Jv[axis] : interior plus left periodic boundary, axis!=direction

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];

  getIndex(mg[level].gridIndexRange(),K1,K2,K3);
  RealArray resid(K1,K2,K3,rangeDimension);  


  getIndex(mg[level].gridIndexRange(),J1,J2,J3,-1);  // interior
  K1=J1; K2=J2;  K3=J3;

  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    // include periodic boundary at left edge
    if( userMap->getIsPeriodic(axis) )
      Jv[axis]=Range(Jv[axis].getBase()-1,Jv[axis].getBound()); 
    else if( axis==direction )
      Jv[axis]=Range(Jv[axis].getBase()-1,Jv[axis].getBound()+1);  
  }
  
  RealArray a(J1,J2,J3),b(J1,J2,J3),c(J1,J2,J3),r(J1,J2,J3);
  RealArray coeff(J1,J2,J3,numberOfCoefficients);
  real dxSq[3];
  dxSq[0]=dx(0,level)*dx(0,level);
  dxSq[1]=dx(1,level)*dx(1,level);
  dxSq[2]=SQR(dx(2,level));
  
  int is[3]=  {0,0,0};    //
  is[direction]=1;

  for( int component=0; component<rangeDimension; component++ )  // *** could move this loop down
  {

    getControlFunctions(level);

    RealArray ur,us,ut;
    ur = uu.r1();
    if( rangeDimension>1 )
      us = uu.r2();
    if( rangeDimension>2 )
      ut = uu.r3();
    getCoefficients(coeff,J1,J2,J3,ur,us,ut);

  
    bool computeCoeff=FALSE;
    getResidual( resid,level,Jv,coeff,computeCoeff); // **** computes residual of ALL components **** fix
      

    // The tridiagonal system is
    //   a = lower diagonal
    //   b = diagonal
    //   c = upper diagonal
  
    a(J1,J2,J3)=(1./dxSq[direction])*coeff(J1,J2,J3,direction);

    if( rangeDimension==2 )
      b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0)+(-2.0/dxSq[1])*coeff(J1,J2,J3,1);
    else if( rangeDimension==3 )
      b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0)+(-2.0/dxSq[1])*coeff(J1,J2,J3,1)+(-2.0/dxSq[2])*coeff(J1,J2,J3,2);
    else
      b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0);
  
    c(J1,J2,J3)=(1./dxSq[direction])*coeff(J1,J2,J3,direction);
  

    // resid = RHS - L(u)
    // subtract the left hand side operator from the entire residual
    r(J1,J2,J3)=resid(J1,J2,J3,component)+( a(J1,J2,J3)*u1(J1-is[0],J2-is[1],J3-is[2],component)+
					    b(J1,J2,J3)*u1(J1      ,J2      ,J3      ,component)+
					    c(J1,J2,J3)*u1(J1+is[0],J2+is[1],J3+is[2],component) );

    // Boundary Conditions
    if( userMap->getIsPeriodic(direction)!=Mapping::functionPeriodic )
    {
    
      Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
      Ib1=J1; Ib2=J2; Ib3=J3;
      for( int side=Start; side<=End; side++ )
      {
	int n= side==Start ? Jv[direction].getBase() : Jv[direction].getBound();
	Ibv[direction]=Range(n,n);
	if( boundaryCondition(side,direction)==dirichlet || 
            boundaryCondition(side,direction)==noSlipOrthogonalAndSpecifiedSpacing )
	{
	  // dirichlet boundary conditions:
	  a(Ib1,Ib2,Ib3)=0.;
	  b(Ib1,Ib2,Ib3)=1.;
	  c(Ib1,Ib2,Ib3)=0.;
	  r(Ib1,Ib2,Ib3)=u1(Ib1,Ib2,Ib3,component);
	}
	else
	{
	  // slip orthogonal boundary conditions??
	  a(Ib1,Ib2,Ib3)=0.;
	  b(Ib1,Ib2,Ib3)=1.;
	  c(Ib1,Ib2,Ib3)=0.;
	  r(Ib1,Ib2,Ib3)=u1(Ib1,Ib2,Ib3,component);
	}
      }
    }
  
    TridiagonalSolver::SystemType systemType = 
      userMap->getIsPeriodic(direction)==Mapping::functionPeriodic ? TridiagonalSolver::periodic : 
      TridiagonalSolver::normal;
  
    if( debug & 8 && systemType==TridiagonalSolver::periodic )
      printf("***** periodic TridiagonalSolver called ***** \n");

    tri.factor(a,b,c,systemType,direction);
    tri.solve(r,J1,J2,J3);

    if( omega==1. )
      u1(J1,J2,J3,component)=r(J1,J2,J3);
    else
      u1(J1,J2,J3,component)=(1.0-omega)*u1(J1,J2,J3,component)+omega*r(J1,J2,J3);

    periodicUpdate(uu,Range(component,component));
  }
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);
  

  return 0;
}




int multigrid::
smooth(const int & level, 
       const SmoothingTypes & smoothingType,
       const int & numberOfSubIterations /* =1 */,
       const int & ichange /* =1 */ )
// ====================================================================
// /Description:
//   Handles the different smoothing methods.
// ====================================================================

{
  int axis;
  for( int subiter=0; subiter<numberOfSubIterations; subiter++ )
  {
    switch( smoothingType )
    {
    case jacobiSmooth:       //underelaxed Jacobi
      // printf("jacobi smooth at level=%i...\n",i);
      jacobi(level,u[level],ichange);
      break;

    case redBlackSmooth:
      // printf("red black smooth at level=%i...\n",i);
      redBlack(level,u[level],ichange);
      break;

    case lineSmooth:
    case zebraSmooth:
      // printf("line smooth at level=%i...\n",i);
      if( useBlockTridiag )
	printf("****** BlockTridiag not implemented yet. Using non-block line smooth *****\n");
    
      if( smoothingMethod==zebraSmooth )
	printf("****** zebraSmooth not implemented yet, using lineSmooth *****\n");
    
      for( axis=0; axis<domainDimension; axis++ )
        lineSmoother( axis, level,u[level] );  // solve for x,y,z 
      break; 

    default:
      printf("smooth:ERROR:Unknown method. Exiting\n");
      exit(1);
    }
  }
  return 0;
}

void multigrid::
Interpolate(int i1, int i2, realArray &u1, realArray &u2, int jmax)
// ===================================================================================
// /Description:
//   Interpolate the solution at level1 from level2.
//   if level1<level2 then this is a prologations, otherwise it is a restriction.
// ==================================================================================
{
  Index Idouble, I, Jdouble, J;

  Index J1,J2,J3;
  getIndex(mg[i1].gridIndexRange(),J1,J2,J3);
  
  Index K1,K2,K3;
  getIndex(mg[i2].gridIndexRange(),K1,K2,K3);

  switch (rangeDimension)
  {
  case 1:
    if ((i1>i2)&&(J1.getBound()<K1.getBound()))
    {
      // interpolation from coarser to finer
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      u2(Idouble,K2,K3,Rx)=u1(J1,J2,J3,Rx);
      Idouble=Range(K1.getBase(), K1.getBound()-2,2);
      I=Range(J1.getBase(), J1.getBound()-1);

      u2(Idouble+1,K2,K3,Rx)=0.5*(u1(I,J2,J3,Rx)+u1(I+1,J2,J3,Rx));
    }
    else if ((i1<i2)&&(J1.getBound()>K1.getBound()))
    {
      // interpolation from finer to coarser
      Idouble=Range(2,J1.getBound()-2,2);
      I=Range(1,K1.getBound()-1);

      u2(K1.getBase(),K2,K3,Rx)=	u1(J1.getBase(),J2,J3,Rx);
      u2(K1.getBound(),K2,K3,Rx)=u1(J1.getBound(),J2,J3,Rx);
      u2(I,K2,K3,Rx)=0.25*(u1(Idouble-1,J2,J3,Rx)+2.0*u1(Idouble,J2,J3,Rx)+u1(Idouble+1,J2,J3,Rx));
    }
    else fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);
    break;

  case 2:
    if ((i1>i2)&&(J1.getBound()<K1.getBound())&&
	(J2.getBound()<K2.getBound()))
    {
      // Interpolate from coarser to finer
      // start with the 2i,2j points
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      Jdouble=Range(K2.getBase(), K2.getBound(),2);
      u2(Idouble,Jdouble,K3,Rx)=u1(J1,J2,J3,Rx);

      // i is even (2i) and j is odd (2j+1)
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      Jdouble=Range(K2.getBase()+1, K2.getBound()-1,2);
      I=Range(J1.getBase(), J1.getBound());
      J=Range(J2.getBase(), J2.getBound()-1);
      u2(Idouble,Jdouble,K3,Rx)=0.5*(u1(I,J,J3,Rx)+u1(I,J+1,J3,Rx));

	// i is odd (2i+1) and j is even (2j)
      Idouble=Range(K1.getBase()+1, K1.getBound()-1,2);
      Jdouble=Range(K2.getBase(),K2.getBound(),2);
      I=Range(J1.getBase(), J1.getBound()-1);
      J=Range(J2.getBase(), J2.getBound());
      u2(Idouble,Jdouble,K3,Rx)=0.5*(u1(I,J,J3,Rx)+u1(I+1,J,J3,Rx));

	// i is odd (2i+1) and j is odd (2j+1)
      Idouble=Range(K1.getBase()+1, K1.getBound()-1,2);
      Jdouble=Range(K2.getBase()+1, K2.getBound()-1,2);
      I=Range(J1.getBase(), J1.getBound()-1);
      J=Range(J2.getBase(), J2.getBound()-1);
      u2(Idouble,Jdouble,K3,Rx)=0.25*(u1(I,J,J3,Rx)+u1(I+1,J,J3,Rx)+ u1(I,J+1,J3,Rx)+u1(I+1,J+1,J3,Rx));
    }
    else if ((i1<i2)&&(J1.getBound()>K1.getBound())&&
	     (J2.getBound()>K2.getBound()))
    {
      // interpolation from finer to coarser
      // Interior point use 9 points full weighted
      Idouble=Range(J1.getBase()+2, J1.getBound()-2,2);
      Jdouble=Range(J2.getBase()+2, J2.getBound()-2,2);
      I=Range(K1.getBase()+1, K1.getBound()-1);
      J=Range(K2.getBase()+1, K2.getBound()-1);
      u2(I,J,K3,Rx)=(1.0/16.0)*(u1(Idouble-1, Jdouble-1, J3,Rx)+
			       2.0*u1(Idouble,   Jdouble-1, J3,Rx)+
			       u1(Idouble+1, Jdouble-1, J3,Rx)+
			       2.0*u1(Idouble-1, Jdouble,   J3,Rx)+
			       4.0*u1(Idouble,   Jdouble,   J3,Rx)+
			       2.0*u1(Idouble+1, Jdouble,   J3,Rx)+
			       u1(Idouble-1, Jdouble+1, J3,Rx)+
			       2.0*u1(Idouble,   Jdouble+1, J3,Rx)+
			       u1(Idouble+1, Jdouble+1, J3,Rx));

      //Use also full weighting on the boundary points
      //and same values in the 4 corners
      Idouble=Range(J1.getBase()+2,J1.getBound()-2,2);
      Jdouble=Range(J2.getBase()+2,J2.getBound()-2,2);
      I=Range(K1.getBase()+1, K1.getBound()-1);
      J=Range(K2.getBase()+1, K2.getBound()-1);
      u2(I,K2.getBase(),K3,Rx)=
	0.25*(   u1(Idouble-1,J2.getBase(),J3,Rx)+
		 2.0*u1(Idouble,  J2.getBase(),J3,Rx)+
		 u1(Idouble+1,J2.getBase(),J3,Rx));
      u2(I,K2.getBound(),K3,Rx)=
	0.25*(   u1(Idouble-1,J2.getBound(),J3,Rx)+
		 2.0*u1(Idouble,  J2.getBound(),J3,Rx)+
		 u1(Idouble+1,J2.getBound(),J3,Rx));
      u2(K1.getBase(),J,K3,Rx)=
	0.25*(   u1(J1.getBase(),Jdouble-1,J3,Rx)+
		 2.0*u1(J1.getBase(),Jdouble,  J3,Rx)+
		 u1(J1.getBase(),Jdouble+1,J3,Rx));
      u2(K1.getBound(),J,K3,Rx)=
	0.25*(   u1(J1.getBound(),Jdouble-1,J3,Rx)+
		 2.0*u1(J1.getBound(),Jdouble,  J3,Rx)+
		 u1(J1.getBound(),Jdouble+1,J3,Rx));
      u2(K1.getBase(), K2.getBase(),K3,Rx)=
	u1(J1.getBase(),J2.getBase(),J3,Rx);
      u2(K1.getBound(), K2.getBase(),K3,Rx)=
	u1(J1.getBound(),J2.getBase(),J3,Rx);
      u2(K1.getBase(), K2.getBound(),K3,Rx)=
	u1(J1.getBase(),J2.getBound(),J3,Rx);
      u2(K1.getBound(), K2.getBound(),K3,Rx)=
	u1(J1.getBound(),J2.getBound(),J3,Rx);
    }
    else 
      fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);

    break;
       
  default:
    printf("Interpolate:Untreated condition\n");
    {throw "error";}
  }
}




#define FULL_WEIGHTING_1D(i1,i2,i3,Rx) (  \
      cr(-1,cf1)*defectFine(i1-1,i2,i3,Rx)           \
     +cr( 0,cf1)*defectFine(i1  ,i2,i3,Rx)           \
     +cr(+1,cf1)*defectFine(i1+1,i2,i3,Rx)           \
                                    )
#define FULL_WEIGHTING_1D_001(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*defectFine(i1,i2,i3-1,Rx)           \
     +cr( 0,cf3)*defectFine(i1,i2,i3  ,Rx)           \
     +cr(+1,cf3)*defectFine(i1,i2,i3+1,Rx)           \
                                    )

#define FULL_WEIGHTING_2D(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D(i1,i2+1,i3,Rx)  \
                                    )

#define FULL_WEIGHTING_3D(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*FULL_WEIGHTING_2D(i1,i2,i3-1,Rx)  \
     +cr( 0,cf3)*FULL_WEIGHTING_2D(i1,i2,i3  ,Rx)  \
     +cr(+1,cf3)*FULL_WEIGHTING_2D(i1,i2,i3+1,Rx)  \
                                    )

#define BOUNDARY_DEFECT_PLANE_110(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D(i1,i2+1,i3,Rx)  \
                                    )
#define BOUNDARY_DEFECT_PLANE_101(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*FULL_WEIGHTING_1D(i1,i2,i3-1,Rx)  \
     +cr( 0,cf3)*FULL_WEIGHTING_1D(i1,i2,i3  ,Rx)  \
     +cr(+1,cf3)*FULL_WEIGHTING_1D(i1,i2,i3+1,Rx)  \
                                    )
#define BOUNDARY_DEFECT_PLANE_011(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D_001(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D_001(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D_001(i1,i2+1,i3,Rx)  \
                                    )

// The boundary defect in 2D should be called in one of two ways
#define BOUNDARY_DEFECT_LINE(is1,is2,is3,i1,i2,i3,Rx)                           \
    ( .5*defectFine(i1,i2,i3,Rx)+.25*(defectFine(i1+is1,i2+is2,i3+is3,Rx)+defectFine(i1-is1,i2-is2,i3+is3,Rx)) )


int multigrid::
fineToCoarse(const int & level, 
	     const RealMappedGridFunction & uFine, 
	     RealMappedGridFunction & uCoarse,
             const bool & isAGridFunction /* = FALSE */ )
// ============================================================================================
// /Description:
//   Compute the restriction of the defect
// /isAGridFunction (input) : If true then this variable defines some (x,y,z) coordinates on a grid.
//    This is used to get the correct periodicity.
// ============================================================================================
{
  realArray cr(Range(-1,1),Range(1,2));   // coefficients for restriction
  cr(-1,1)=0.;  cr(0,1)=1.; cr(+1,1)=0.;  // coarsening factor of 1
  cr(-1,2)=.25; cr(0,2)=.5; cr(+1,2)=.25; // coarsening factor of 2
  
  MappedGrid & mgFine      = mg[level];
  MappedGrid & mgCoarse    = mg[level+1];  
  const RealArray & defectFine   = uFine;
  RealArray & fCoarse      = uCoarse;

  const int & numberOfDimensions = rangeDimension;

  int cf1,cf2,cf3,cf[3];  // ***** coarsening factors are all 2 for now *****
  cf1=cf[0]=2;
  cf2=cf[1]=2;
  cf3=cf[2]=2;

  assert(cf[0]==2 && (cf[1]==2 || numberOfDimensions<2) && (cf[2]==2 || numberOfDimensions<3));

  fCoarse=0.;   // **** could do better ***

  int numberOfFictitiousPoints = 1;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  getIndex(mgFine.indexRange(),I1,I2,I3);                    // Index's for fine grid, 
  // set stride
  int dir;
  for( dir=0; dir<3; dir++ )
    Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound(),cf[dir]); 
  
  getIndex(mgCoarse.indexRange(),J1,J2,J3);                  // Index's for coarse grid

  // Average interior points using the full weighting operator
  if( numberOfDimensions==1 )
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_1D(I1,I2,I3,Rx);      
  else if( numberOfDimensions==2 )
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_2D(I1,I2,I3,Rx);          
  else
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_3D(I1,I2,I3,Rx);    
  
  //   === Boundaries ===
  Index Iev[3], &Ie1=Iev[0], &Ie2=Iev[1], &Ie3=Iev[2];
  Index Jev[3], &Je1=Jev[0], &Je2=Jev[1], &Je3=Jev[2];
  
  int side,axis;
  int is[3]={0,0,0};
  for( axis=0; axis<numberOfDimensions; axis++ ) 
  {
    for( side=0; side<=1; side++ )
    {
      if( boundaryCondition(side,axis)>0 )
      { 
	getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);  // bndry pts
	for( dir=0; dir<3; dir++ )
	  Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound(),2);          // set stride to 2

	getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3); 

	if( numberOfDimensions==1 )
	  fCoarse(J1,J2,J3,Rx)=defectFine(I1,I2,I3,Rx);    // inject boundary defects in 1D
	else if( numberOfDimensions==2 )
	{
	  const int axisp1= (axis+1) % numberOfDimensions; // tangential direction

       	  is[axisp1]=1;   // we average in this direction
	  fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_LINE(is[0],is[1],is[2],I1,I2,I3,Rx);  // average
       	  is[axisp1]=0;   // reset

  	  // Inject values at corners in 2D or edges in 3D (some points are done twice but who cares...)
	  //                    X -------- X
	  //                    |          |
	  //                    |          |
	  //                    |          |
	  //                    |          |
	  //                    X -------- X
	  if( boundaryCondition(Start,axisp1)>0 )
	    fCoarse(J1.getBase(),J2.getBase(),J3.getBase(),Rx)=defectFine(I1.getBase(),I2.getBase(),I3.getBase(),Rx);
	  if( boundaryCondition(End,axisp1)>0 )
	    fCoarse(J1.getBound(),J2.getBound(),J3.getBase(),Rx)=defectFine(I1.getBound(),I2.getBound(),I3.getBound(),Rx);

	}
	else
	{
          // boundaries in 3D:  
          //          o average faces using 2D full weighting
          //          o average edges using 1D full weighting
          //          o vertices are injected.
          if( axis==0 )
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_011(I1,I2,I3,Rx); // average along boundary face
          else if( axis==1 )
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_101(I1,I2,I3,Rx); // average along boundary face
          else
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_110(I1,I2,I3,Rx); // average along boundary face
          
          // do the edges of this plane
          Ie1=I1; Ie2=I2; Ie3=I3;  // defines the edge on the fine grid.
          Je1=J1; Je2=J2; Je3=J3;  // defines the edge on the coarse grid.
	  int side2, axist;
	  for( side2=Start; side2<=End; side2++ )                // loop over left and right side
	  {
	    for( int axist=0; axist<numberOfDimensions-1; axist++ )    // 2 tangential directions
	    {
	      const int axisp1= (axis+axist+1) % numberOfDimensions; // edge is defined by axis==side, axisp1=side2
              if( boundaryCondition(side2,axisp1)>0 )
	      {
		Iev[axisp1]= side2==0 ? Iv[axisp1].getBase() : Iv[axisp1].getBound();
		Jev[axisp1]= side2==0 ? Jv[axisp1].getBase() : Jv[axisp1].getBound();

		const int axisp2= (axisp1+1) % numberOfDimensions; // this direction still varies.
                is[axisp2]=1;  
		fCoarse(Je1,Je2,Je3,Rx)=BOUNDARY_DEFECT_LINE(is[0],is[1],is[2],Ie1,Ie2,Ie3,Rx); 
                is[axisp2]=0;  

                // vertices are injected
		if( boundaryCondition(Start,axisp2)>0 )
		  fCoarse(J1.getBase(),J2.getBase(),J3.getBase(),Rx)=defectFine(I1.getBase(),I2.getBase(),I3.getBase(),Rx);
		if( boundaryCondition(End,axisp2)>0 )
		  fCoarse(J1.getBound(),J2.getBound(),J3.getBase(),Rx)=defectFine(I1.getBound(),I2.getBound(),I3.getBound(),Rx);

	      }
	    }
	  }
	}
      
      }
    }
  }
  
  periodicUpdate(uCoarse,nullRange,isAGridFunction);
  return 0;
}

#undef FULL_WEIGHTING_1D
#undef FULL_WEIGHTING_2D
#undef FULL_WEIGHTING_3D
#undef BOUNDARY_DEFECT_2D
#undef BOUNDARY_DEFECT_3D



//---------------------------------------------------------------------------------------------
//   Prolongation on a component grid
//
//     u.multigridLevel[level] += Prolongation[ u.multigridLevel[level+1] ]
//   
//---------------------------------------------------------------------------------------------
//     ...2nd order interpolation
#define Q2000(j1,j2,j3) ( uCoarse(j1,j2,j3) )
#define Q2100(j1,j2,j3) ( cp2(0,cf1)*uCoarse(j1,j2,j3)+cp2(1,cf1)*uCoarse(j1+1,j2  ,j3  ) )
#define Q2010(j1,j2,j3) ( cp2(0,cf2)*uCoarse(j1,j2,j3)+cp2(1,cf2)*uCoarse(j1  ,j2+1,j3  ) )
#define Q2001(j1,j2,j3) ( cp2(0,cf3)*uCoarse(j1,j2,j3)+cp2(1,cf3)*uCoarse(j1  ,j2  ,j3+1) )
#define Q2110(j1,j2,j3) ( cp2(0,cf2)*  Q2100(j1,j2,j3)+cp2(1,cf2)*  Q2100(j1  ,j2+1,j3  ) )
#define Q2101(j1,j2,j3) ( cp2(0,cf3)*  Q2100(j1,j2,j3)+cp2(1,cf3)*  Q2100(j1  ,j2  ,j3+1) )
#define Q2011(j1,j2,j3) ( cp2(0,cf3)*  Q2010(j1,j2,j3)+cp2(1,cf3)*  Q2010(j1  ,j2  ,j3+1) )
#define Q2111(j1,j2,j3) ( cp2(0,cf3)*  Q2110(j1,j2,j3)+cp2(1,cf3)*  Q2110(j1  ,j2  ,j3+1) )

//     ...fourth order interpolation
#define Q4000(j1,j2,j3) ( uCoarse(j1,j2,j3) )

#define Q4100(j1,j2,j3) ( cp4( 0,cf1)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf1)*uCoarse(j1+1,j2  ,j3  ) \
                         +cp4(-1,cf1)*uCoarse(j1-1,j2  ,j3  )+cp4(2,cf1)*uCoarse(j1+2,j2  ,j3  ) )

#define Q4010(j1,j2,j3) ( cp4( 0,cf2)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf2)*uCoarse(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*uCoarse(j1  ,j2-1,j3  )+cp4(2,cf2)*uCoarse(j1  ,j2+2,j3  ) )

#define Q4001(j1,j2,j3) ( cp4( 0,cf3)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf3)*uCoarse(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*uCoarse(j1  ,j2  ,j3-1)+cp4(2,cf3)*uCoarse(j1  ,j2  ,j3+2) )

#define Q4110(j1,j2,j3) ( cp4( 0,cf2)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf2)*  Q4100(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*  Q4100(j1  ,j2-1,j3  )+cp4(2,cf2)*  Q4100(j1  ,j2+2,j3  ) )

#define Q4101(j1,j2,j3) ( cp4( 0,cf3)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4100(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4100(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4100(j1  ,j2  ,j3+2) )

#define Q4011(j1,j2,j3) ( cp4( 0,cf3)*  Q4010(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4010(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4010(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4010(j1  ,j2  ,j3+2) )

#define Q4111(j1,j2,j3) ( cp4( 0,cf3)*  Q4110(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4110(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4110(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4110(j1  ,j2  ,j3+2) )

//===================================================================
//             Correct a Component Grid
//      u(i,j) = u(i,j) + P[ u2(i,j) ]   ( P : Prolongation )
//  cp21,cp22,cp23 : coeffcients for prolongation, 2nd order
//  cp41,cp41,cp43 : coeffcients for prolongation, 4th order
//
//===================================================================
  //     & cp21( 0:1),cp22( 0:1),cp23( 0:1),
  //     & cp41(-1:2),cp42(-1:2),cp43(-1:2)
int multigrid::
coarseToFine(const int & level,  
	     const RealMappedGridFunction & uCoarse, 
	     RealMappedGridFunction & uFineGF,
             const bool & isAGridFunction /* = FALSE */  )
{
  const int orderOfAccuracy=2;
  RealArray & uFine = uFineGF;

  const real c41=9./16.,c42=-1./16.;
  //  ....cp2,cp4 : coefficients for prolongation (2nd or 4th order)
  //         cp2(.,cf) :  kf=coarsening factor (1 or 2)
  realArray cp2(Range(0,1),Range(1,2));
  realArray cp4(Range(-1,2),Range(1,2));
  cp2(0,1)=1.; cp2(1,1)=0.;  // if coarsening factor =1 we just transfer the data
  cp2(0,2)=.5; cp2(1,2)=.5;  // coarsen factor = 2

  cp4(-1,1)=0.;     cp4(0,1)=1.;    cp4(1,1)=0.;    cp4(2,1)=0.;  
  cp4(-1,2)=-.0625; cp4(0,2)=.5625; cp4(1,2)=.5625; cp4(2,2)=-.0625;  // 4-point order interpolation

  MappedGrid & mgFine   = mg[level];  
  MappedGrid & mgCoarse = mg[level+1];  
  const int & numberOfDimensions = rangeDimension;

  int cf1,cf2,cf3, cf[3];
  cf1=cf[0]=2;  // coarsening factor
  cf2=cf[1]=2;
  cf3=cf[2]=2;

  assert(cf1==2 && (cf2==2 || numberOfDimensions<2) && (cf3==2 || numberOfDimensions<3));
  
  //----------------------------------------------------------------------------------------
  // There are two types of corrections:
  //   (1) when a fine grid and coarse grid point coincide, use Index's I1,I2,I3
  //   (2) when a fine grid point is midway between coarse grid points, use I1p,I2p,I3p
  //
  //        1--2--1--2--1--2------ ... -----2--1--2--1  fine grid
  //        X-----B-----X--------- ... -----X--B-----X  coarse grid
  //   
  //           B=boundary
  //
  //   Note that we use more fictitious points on the fine grid than on the coarse
  //-----------------------------------------------------------------------------------------
  int numberOfFictitiousPoints = 1;
  Index Iav[3], &I1a = Iav[0], &I2a=Iav[1], &I3a=Iav[2];
  Index Jav[3], &J1a = Jav[0], &J2a=Jav[1], &J3a=Jav[2];
  Index I1,I2,I3, J1,J2,J3;
  Index I1p,I2p,I3p,J1p,J2p,J3p;
  int nf0,nf1;

  getIndex(mgFine.indexRange(),I1a,I2a,I3a);
  // ************* this only works for coarsening factor=2 *********
  //----------------------------------
  //---  Get Index's for fine grid ---
  //----------------------------------
  nf0=((numberOfFictitiousPoints+1)/2)*2;   
  nf1=((numberOfFictitiousPoints-2)/2)*2;
  I1p=                          Range(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2p= numberOfDimensions > 1 ? Range(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : Range(I2a);
  I3p= numberOfDimensions > 2 ? Range(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : Range(I3a);

  nf0=((numberOfFictitiousPoints)/2)*2;  
  nf1=((numberOfFictitiousPoints)/2)*2;
  I1 =                          Range(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2 = numberOfDimensions > 1 ? Range(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : Range(I2a);
  I3 = numberOfDimensions > 2 ? Range(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : Range(I3a);

  //------------------------------------
  //---  Get Index's for coarse grid ---
  //------------------------------------
  getIndex(mgCoarse.indexRange(),J1a,J2a,J3a);   // this is ok

  nf0=((numberOfFictitiousPoints+1)/2);  
  nf1=((numberOfFictitiousPoints-2)/2);
  J1p=                          Range(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2p= numberOfDimensions > 1 ? Range(J2a.getBase()-nf0,J2a.getBound()+nf1) : Range(J2a);
  J3p= numberOfDimensions > 2 ? Range(J3a.getBase()-nf0,J3a.getBound()+nf1) : Range(J3a);
  nf0=((numberOfFictitiousPoints)/2);  
  nf1=((numberOfFictitiousPoints)/2);
  J1 =                          Range(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2 = numberOfDimensions > 1 ? Range(J2a.getBase()-nf0,J2a.getBound()+nf1) : Range(J2a);
  J3 = numberOfDimensions > 2 ? Range(J3a.getBase()-nf0,J3a.getBound()+nf1) : Range(J3a);

  uFine(I1,I2,I3)+=uCoarse(J1,J2,J3);
  
  if( orderOfAccuracy==2 )
  {
    uFine(I1p+1,I2,I3)+=Q2100(J1p,J2 ,J3);
    if( numberOfDimensions > 1 )
    {
      uFine(I1   ,I2p+1,I3)+=Q2010(J1 ,J2p,J3);
      uFine(I1p+1,I2p+1,I3)+=Q2110(J1p,J2p,J3);
    }  
    if( numberOfDimensions>2 )
    {
      uFine(I1   ,I2   ,I3p+1)+=Q2001(J1 ,J2 ,J3p);
      uFine(I1p+1,I2   ,I3p+1)+=Q2101(J1p,J2 ,J3p);
      uFine(I1   ,I2p+1,I3p+1)+=Q2011(J1 ,J2p,J3p);
      uFine(I1p+1,I2p+1,I3p+1)+=Q2111(J1p,J2p,J3p);
    }
  }
  else
  {   // -------- fourth-order ------------
    uFine(I1p+1,I2,I3)+=Q4100(J1p,J2 ,J3);
    if( numberOfDimensions > 1 )
    {
      uFine(I1   ,I2p+1,I3)+=Q4010(J1 ,J2p,J3);
      uFine(I1p+1,I2p+1,I3)+=Q4110(J1p,J2p,J3);
    }  
    if( numberOfDimensions>2 )
    {
      uFine(I1   ,I2   ,I3p+1)+=Q4001(J1 ,J2 ,J3p);
      uFine(I1p+1,I2   ,I3p+1)+=Q4101(J1p,J2 ,J3p);
      uFine(I1   ,I2p+1,I3p+1)+=Q4011(J1 ,J2p,J3p);
      uFine(I1p+1,I2p+1,I3p+1)+=Q4111(J1p,J2p,J3p);
    }
  }
  periodicUpdate(uFineGF,nullRange,isAGridFunction);
  return 0;
}

#undef Q2000
#undef Q2100
#undef Q2010
#undef Q2001
#undef Q2110
#undef Q2101
#undef Q2011
#undef Q2111
#undef Q4000
#undef Q4100
#undef Q4010
#undef Q4001
#undef Q4110
#undef Q4101
#undef Q4011
#undef Q4111






int multigrid::
multigridVcycle(const int & level )
// =========================================================================================
// /Description:
//   Multigrid V cycle.
// =========================================================================================
{
  if( level==0 )
   rhs[level]=0.0;

  if( debug & 4 && level>0 )
    plot( rhs[level],sPrintF(buff,"rhs at start of cycle, level %i\n",level));


  int numberOfSmooths=1;
  if( level==numberOfLevels-1 )
  {
    // coarse grid : do more iterations.
    numberOfSmooths=pow(2,numberOfLevels+1);
    smooth(level,smoothingMethod,numberOfSmooths,1);
    return 0;
  }
  
  if( debug & 4 )
    plot( u[level],sPrintF(buff,"solution BEFORE smooth on level %i\n",level));
  
  smooth(level,smoothingMethod,numberOfSmooths,1);

  Range all;
  RealMappedGridFunction restemp1(mg[level],all,all,all,Rx); // *** is this really needed ??

  if( debug & 2 )
  {
    getResidual(restemp1,level);
    printf("maximum residual = %e after initial smooth at level %i\n",max(fabs(restemp1)));
  }
  if( debug & 4 )
    plot( u[level],sPrintF(buff,"solution AFTER smooth on level %i\n",level));
  
  if (level != numberOfLevels-1 )
  {
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    getIndex(mg[level].gridIndexRange(),J1,J2,J3,1); // include 1 ghost point

    Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
    getIndex(mg[level+1].gridIndexRange(),K1,K2,K3,1);

    // restemp1=0.0;
    w[level+1]=0.0;
    getResidual(restemp1,level);
    restemp1.periodicUpdate();  // this seems to be needed.

    if( debug & 4  )
      printf("**** maximum residual=%e on level %i after initial smooth\n",max(fabs(restemp1)),level);
      
    if( debug & 4 )
      plot( restemp1,sPrintF(buff,"residual after smooth on level %i\n",level));

    fineToCoarse(level,restemp1,rhs[level+1]);
    const bool isAGridFunction=TRUE;
    fineToCoarse(level,u[level],w[level+1],isAGridFunction);

    u[level+1]=w[level+1];

    if( debug & 4 )
      plot( u[level+1],sPrintF(buff,"Initial restricted u level %i\n",level+1));

    updateRHS(level+1);

    multigridVcycle(level+1);

    w[level+1]=u[level+1]-w[level+1];
    
    coarseToFine(level,w[level+1],u[level],isAGridFunction);
    
  }

  smooth(level,smoothingMethod,numberOfSmooths,0);

  return 0;
}
  



int multigrid::
applyMultigrid()
// ===========================================================================================
// /Description:
//     Perform some multigrid iterations.
//
// /u0 (input) : initial guess.     // ****** fix this -- we should keep ghost values ------------------
//
// ===========================================================================================
{
  printf("applyMultigrid: number of levels=%i, smoother=%s \n",numberOfLevels,smoothingMethod==1 ? "jacobi" :
          smoothingMethod==2 ? "red-black" : smoothingMethod==3 ? "line" : "zebra" );
  

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(mg[0].gridIndexRange(),J1,J2,J3);

  realArray residtemp1(J1,J2,J3,rangeDimension);
  residtemp1=0.0;

  int i,axis;
  iter=0;

  real workUnitsPerCycle, work=0.0;
  real ratio=0.5;

  if( numberOfLevels==1 ) 
    workUnitsPerCycle=1.0;
  else if (numberOfLevels==2) 
    workUnitsPerCycle=1.0+1./4.+11./24.;
  else if (numberOfLevels==3)   
    workUnitsPerCycle=1.0+1./4.+11./24.+11./96.+1./16.;
  else 
    workUnitsPerCycle=1.0+1./4.+11./24.+11./96.+1./16.+11./384+1./64.;

  real time0,time;
  if( debug & 2 )
  {
    getResidual(residtemp1,0);
    maximumResidual=max(fabs(residtemp1));
    printf("** Initial residual=%e \n",  maximumResidual);
  }

  // Do few iterations of the V-cycle
  time0=getCPU();
  for(iter=0; iter<maximumNumberOfIterations; iter++)
  {
    // ***** call multigrid ******
    multigridVcycle(0);

    getResidual(residtemp1,0);
    previousMaximumResidual=maximumResidual;
    maximumResidual=max(fabs(residtemp1));

    ratio=maximumResidual/max(REAL_MIN,previousMaximumResidual);

    work += workUnitsPerCycle;
    time=getCPU()-time0;
    printf("iter=%i\t resid=%6.2e\t ratio=%6.3f\t ECR=%6.3f, cpu=%6.3e\t levels=%i\n",iter, maximumResidual, ratio, 
           pow(ratio,1./workUnitsPerCycle), time,numberOfLevels);

    if( maximumResidual<residualTolerance )  // scale by the number of grid points.
      break;

    if( FALSE )
      printf("%g\t %g\n",work,log10(maximumResidual));
    fflush(stdout);
    if( FALSE && ratio>1. && iter>5 && numberOfLevels>1 )
    {
      numberOfLevels--;
      printf("Decreasing the number of levels, ratio=%g\n",ratio);
    }
  }
  return 0;
}

  
