// ---------------------------------------------------------------------------
// Solve Maxwell's equations on overlapping and hybrid grids.
//
//
// ---------------------------------------------------------------------------

#include "MappedGridOperators.h"
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "MatrixTransform.h"
#include "DataPointMapping.h"

#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"

#include "Maxwell.h"
#include "ParallelUtility.h"

int 
getLineFromFile( FILE *file, char s[], int lim);

void display(realArray & u )
{
  printF("u.getlength(0)=%i\n",u.getLength(0));
  
  ::display(u,"u");
}



int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  // Use this to avoid un-necessary communication: 
  Optimization_Manager::setForceVSG_Update(Off);
  const int myid=Communication_Manager::My_Process_Number;

  Overture::turnOnMemoryChecking(true);

  printF("Usage: `cgmx [options] [file.cmd]' \n"
         "    Options: \n"
         "          -noplot:   run without graphics \n" 
         "          file.cmd: read this command file \n");

// -- these command line arguments are deprecated
//   printF("Usage: `mx [options] [file.cmd]' \n"
//     "    Options: \n"
//     "          -noplot:   run without graphics \n" 
//     "          -nx=<grid points> : set number of grid points in all directions\n"
//     "          -size=value sets the square to be [-value:value][-value:value]\n"
//     "          -tFinal : set final time\n"
//     "          -tPlot  : times to plot\n"
//     "          -cfl=<value> : cfl \n"
//     "          -ad=<value> : artificial dissipation\n"
//     "          -tri -quad  : use triangles or quads\n"
//     "          -uns        : use default unstructured elements (quads or hexes usually)\n"
//     "          -rot -sine -chevron -sqtri -sqquad -sinetri: -annulus grid type: rotated-square, sine, chevron or squareByTriangles\n"
//     "          -perturbedSquare -perturbedBox: square or box with random perturbations\n"
//     "          -box -chevbox: 3D box or chevroned box grid types\n"
//     "          -dsi : use the full DSI scheme (even on a rectangular grid where Yee is default)\n"
//     "          -dsimv : use the full DSI scheme with matrix-vector implementation (even on a rectangular grid where Yee is default)\n"
//     "          -nfdtd : use the non-orthogonal FDTD algorithm\n"
//     "          -new : use the new DSI algorithm\n"
//     "          -bc=[periodic][dirichlet][pec] : set boundary conditions\n"
//     "          -ox=[2/4] : order of accuracy in space (nfdtd only)\n"
//     "          -ot=[2/4] : order of accuracy in time \n"
//     "          -chevf=<value> : frequency factor for chevron grid oscillation (default=1)\n"
//     "          -cheva=<value> : amplitude factor for chevron grid oscillation (default=.1)\n"
//     "          file.cmd: read this command file \n");

  Maxwell & solver = *new Maxwell;

  int plotOption=true;
  aString commandFileName="";
  aString line;
  int len=0;
  
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      // printF(" parse input: argv[%i]=%s\n",i,argv[i]);
      
      if( line=="noplot" )
        plotOption=FALSE;
      else if( len=line.matches("-nx=") )
      {
	sScanF(line(len,line.length()-1),"%i",&solver.nx[0]);
        printF(" Setting nx AND ny AND nz =%i\n",solver.nx[0]);
        solver.nx[1]=solver.nx[2]=solver.nx[0];
      }
      else if( len=line.matches("-ny=") )
      {
	sScanF(line(len,line.length()-1),"%i",&solver.nx[1]);
        printF(" Setting ny=%i\n",solver.nx[1]);
      }
      else if( len=line.matches("-nz=") )
      {
	sScanF(line(len,line.length()-1),"%i",&solver.nx[2]);
        printF(" Setting nz=%i\n",solver.nx[2]);
      }
      else if( len=line.matches("-size=") )
      {
	real size;
	sScanF(&line[len],"%e",&size);
	solver.xab[0][0]=-size;
	solver.xab[1][0]= size;
	solver.xab[0][1]=-size;
	solver.xab[1][1]= size;
	solver.xab[0][2]=-size;
	solver.xab[1][2]= size;
      }
      else if( len=line.matches("-bc=") )
      {
	aString bcName=line(len,line.length()-1);
        printF(" Setting bcOption=%s\n",(const char*)bcName);
        if( bcName=="periodic" )
	{
          solver.bcOption=Maxwell::useAllPeriodicBoundaryConditions;
	}
	else if ( bcName=="dirichlet" ) 
	{
          solver.bcOption=Maxwell::useAllDirichletBoundaryConditions;
	}
	else if( bcName=="pec" )
	{
          solver.bcOption=Maxwell::useAllPerfectElectricalConductorBoundaryConditions;
	}
	else if( bcName=="general" )
	{
          solver.bcOption=Maxwell::useGeneralBoundaryConditions;
	}
        else
	{
	  printF("**ERROR** unknown bc option\n");
	}
      }
      else if( len=line.matches("-noplot") )
      {
	plotOption=false;
        printF(" Setting plotOption=false\n");
      }
      else if( len=line.matches("-tri") )
      {
	solver.elementType=Maxwell::triangles;
        printF(" Setting elementType=triangles\n");
      }
      else if(len=line.matches("-uns") )
      {
	solver.elementType=Maxwell::defaultUnstructured;
        printF(" Setting elementType=defaultUnstructured\n");
      }
      else if( len=line.matches("-quad") )
      {
	solver.elementType=Maxwell::quadrilaterals;
        printF(" Setting elementType=quadrilaterals\n");
      }
      else if( len=line.matches("-rot") )
      {
	solver.gridType=Maxwell::rotatedSquare;
        printF(" Setting gridType=rotatedSquare\n");
      }
      else if( len=line.matches("-sinetri") )
      {
	solver.gridType=Maxwell::sineByTriangles;
        solver.elementType=Maxwell::triangles;
        printF(" Setting gridType=sineByTriangles, elementType=triangles;\n");
      }
      else if( len=line.matches("-sine") )
      {
	solver.gridType=Maxwell::sineSquare;
        printF(" Setting gridType=sineSquare\n");
      }
      else if( len=line.matches("-chevron") )
      {
	solver.gridType=Maxwell::chevron;
        printF(" Setting gridType=chevron\n");
      }
      else if ( ( len=line.matches("-chevf=") ) )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.chevronFrequency);
	printf(" Setting chevronFrequency=%e\n",solver.chevronFrequency);
      }
      else if ( ( len=line.matches("-cheva=") ) )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.chevronAmplitude);
	printf(" Setting chevronAmplitude=%e\n",solver.chevronAmplitude);
      }
      else if( len=line.matches("-box") )
	{
	  solver.gridType=Maxwell::box;
	  printF(" Setting gridType=box\n");
	}
      else if( len=line.matches("-chevbox") )
	{
	  solver.gridType=Maxwell::chevbox;
	  printF(" Setting gridType=chevbox\n");
	}
      else if( len=line.matches("-sqtri") )
      {
	solver.gridType=Maxwell::squareByTriangles;
        solver.elementType=Maxwell::triangles;
        printF(" Setting gridType=squareByTriangles\n");
      }
      else if( len=line.matches("-sqquad") )
      {
	solver.gridType=Maxwell::squareByQuads;
        solver.elementType=Maxwell::quadrilaterals;
        printF(" Setting gridType=squareByQuads\n");
      }
      else if( len=line.matches("-skewedSquare") )
      {
	solver.gridType=Maxwell::skewedSquare;
        printF(" Setting gridType=skewedSquare\n");
      }      
      else if( len=line.matches("-annulus") )
      {
	solver.gridType=Maxwell::annulus;
        printF(" Setting gridType=annulus\n");
      }      
      else if( len=line.matches("-perturbedSquare") )
      {
	solver.gridType=Maxwell::perturbedSquare;
        printF(" Setting gridType=perturbedSquare\n");
      }
      else if( len=line.matches("-perturbedBox") )
      {
	solver.gridType=Maxwell::perturbedBox;
        printF(" Setting gridType=perturbedBox\n");
      }
      else if( len=line.matches("-tFinal=") )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.tFinal);
        printF(" Setting tFinal=%e\n",solver.tFinal);
      }
      else if( len=line.matches("-tPlot=") )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.tPlot);
        printF(" Setting tPlot=%e\n",solver.tPlot);
      }
      else if( len=line.matches("-cfl=") )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.cfl);
        printF(" Setting cfl=%e\n",solver.cfl);
      }
      else if( len=line.matches("-ad=") )
      {
	sScanF(line(len,line.length()-1),"%e",&solver.artificialDissipation);
        printF(" Setting artificialDissipation=%e\n",solver.artificialDissipation);
      }
      else if( len=line.matches("-new") )
      {
	solver.method=Maxwell::dsiNew;
        printF(" Setting method=dsiNew\n");
      }
      else if( len=line.matches("-dsimv") )
      {
	solver.method=Maxwell::dsiMatVec;
        printF(" Setting method=DSI-MatVec\n");
      }
      else if( len=line.matches("-dsi") )
      {
	solver.method=Maxwell::dsi;
        printF(" Setting method=dsi\n");
      }
      else if( len=line.matches("-nfdtd") )
      {
	solver.method=Maxwell::nfdtd;
        printF(" Setting method=nfdtd\n");
      }
      else if( len=line.matches("-ox=") )
      {
	sScanF(&line[len],"%i",&solver.orderOfAccuracyInSpace);
        printF(" Setting order of accuracy in space =%i\n",solver.orderOfAccuracyInSpace);
      }
      else if( len=line.matches("-ot=") )
      {
	sScanF(&line[len],"%i",&solver.orderOfAccuracyInTime);
        printF(" Setting order of accuracy in time =%i\n",solver.orderOfAccuracyInTime);
      }
      else if( len=line.matches("-sq") )
	{
	  solver.gridType=Maxwell::square;
	  printF(" Setting gridType=square\n");
	}
      else if( len=line.matches("-grid=") )
      {
	solver.nameOfGridFile=line(len,line.length()-1);
        solver.gridType=Maxwell::compositeGrid;
        printF(" Setting gridType=compositeGrid\n");
      }
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("Using command file = [%s]\n",(const char*)commandFileName);
      }
      
    }
  }

  GL_GraphicsInterface & gi = (GL_GraphicsInterface &)
                     (*Overture::getGraphicsInterface("Maxwell's Equation",plotOption,argc,argv));

  PlotStuffParameters psp;
  // By default start saving the command file called "ogen.cmd"
  aString logFile="cgmx.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  if( commandFileName!="" )
    gi.readCommandFile(commandFileName);

  
  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numGhost=2;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numGhost);
  #endif
 
  if( solver.gridType==Maxwell::unknown )
  {
    aString name;
    gi.inputString(name,"Enter the name of the grid file or one of -sq, -sine, -chevron etc.");
    if( name=="-sqtri" )
    {
      solver.gridType=Maxwell::squareByTriangles;
      solver.elementType=Maxwell::triangles;
      printF(" Setting gridType=squareByTriangles\n");
    }
    else if( name=="-sq" )
    {
      solver.gridType=Maxwell::square;
      printF(" Setting gridType=square\n");
    }
    else if( name=="-rot" )
    {
      solver.gridType=Maxwell::rotatedSquare;
      printF(" Setting gridType=rotatedSquare\n");
    }
    else if( name=="-sinetri" )
    {
      solver.gridType=Maxwell::sineByTriangles;
      solver.elementType=Maxwell::triangles;
      printF(" Setting gridType=sineByTriangles\n");
    }
    else if( name=="-sinequad" )
    {
      solver.gridType=Maxwell::sineSquare;
      solver.elementType=Maxwell::quadrilaterals;
      printF(" Setting gridType=sineSquare, elementType=quadrilaterals.\n");
    }
    else if( name=="-sine" )
    {
      solver.gridType=Maxwell::sineSquare;
      printF(" Setting gridType=sineSquare\n");
    }
    else if( name=="-chevron" )
    {
      solver.gridType=Maxwell::chevron;
      printF(" Setting gridType=chevron\n");
    }
    else if( name=="-box" )
      {
	solver.gridType=Maxwell::box;
	printf(" Setting gridType=box\n");
      }
    else if( name=="-chevbox" )
      {
	solver.gridType=Maxwell::chevbox;
	printf(" Setting gridType=chevbox\n");
      }
    else
    {
      solver.nameOfGridFile=name;
      solver.gridType=Maxwell::compositeGrid;
    }
    
  }

  // *wdh* 090427 -- read in the ComppositeGrid here to be consistent with other cg solvers
  if( solver.gridType==Maxwell::compositeGrid )
  {
    // In this case we grab the Mapping from the first component grid.
     
    // create and read in a CompositeGrid
    assert( solver.cgp == NULL );
    
    solver.cgp=new CompositeGrid;
    CompositeGrid & cg = *solver.cgp;
    getFromADataBase(cg,solver.nameOfGridFile); // read in the grid with the default loadBalancer

  }


  // Build the grid 
  real time0 = getCPU();
  solver.setupGrids();

  solver.interactiveUpdate(gi);
  solver.timing(Maxwell::totalTime) += getCPU()-time0;

  solver.solve(gi);

  delete &solver;  // do this here so the show file is closed before MPI is shut down
  
  Overture::finish();          
  return 0;
}
