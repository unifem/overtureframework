//===============================================================================
// 
//  getEnergy.C  : Interpolate solution values from a showfile. 
// 
//     This sample program demonstrates how to read solutions from a show file,
//  and then obtain solution values on some user defined locations.
// 
//
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "ShowFileReader.h"
#include "Integrate.h"
#include "CompositeGridOperators.h"
#include "PlotStuff.h"
#include "InterpolatePoints.h"

#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture and A++/P++

  aString nameOfShowFile;
  cout << ">> Enter the name of the (old) show file:" << endl;
  cin >> nameOfShowFile;
  ShowFileReader showFileReader(nameOfShowFile);

  int numberOfFrames=showFileReader.getNumberOfFrames();
  int numberOfSolutions = max(1,numberOfFrames);
  int solutionNumber=numberOfFrames;

  CompositeGrid cg;
  realCompositeGridFunction u;

  PlotStuff ps;                      // create a PlotStuff object
  PlotStuffParameters psp;           // create an object that is used to pass parameters
    
  aString answer,answer2;
  aString menu[] = { "!getEnergy",
		     "get a solution",
		     "get energy",
		     "exit",
		     "" };
  const aString *headerComment;
  int numberOfHeaderComments;
  char buff[80];
  
  real time,reynoldsNumber,machNumber,nu;

  // Look for some parameters (these may or may not be in the show file)
  showFileReader.getGeneralParameter("reynoldsNumber",reynoldsNumber);
  showFileReader.getGeneralParameter("machNumber",machNumber);
  showFileReader.getGeneralParameter("nu",nu);

  printf("Values from the show file: nu=%e, Reynolds number=%e, Mach number=%e \n",nu,reynoldsNumber,machNumber);
  

  int numberOfFaces, numberOfGrids;
  IntegerArray integrateForceOnGridFace;
  IntegerArray boundary;
  Integrate integrate;
  int surfaceID=0;    // this number identifies the surface
  int side, axis, grid;
  aString line;
  real rho=1., u0=1., length=1., scale=1.;

  int it;
  for( it=0; ; it++)
  {

    if( it==0 )
      answer="get a solution";
    else
      ps.getMenuItem(menu,answer);

    if( answer=="get a solution" )
    {
      // In this case the user is asked to choose a solution to read in
      // Choosing a number that is too large will cause the last solution to be read 

      if( it>0 )
      {
        ps.inputString(line,sPrintF(buff,"Enter the solution number, [1,%i] \n",numberOfSolutions));
        sscanf(line,"%i",&solutionNumber);
      }
      
      showFileReader.getASolution(solutionNumber,cg,u);        // read in a grid and solution

      // read any header comments that go with this solution
      headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);

      for( int i=0; i<numberOfHeaderComments; i++ )
        printf("Header comment: %s \n",(const char *)headerComment[i]);

      // Look for the variable "t" which indicates the value of time for this solution
      HDF_DataBase & db = *showFileReader.getFrame();
      time=0.;
      db.get(time,"t");
      
      printf("Solution number = %i : time=%e\n",solutionNumber,time);

      numberOfGrids=cg.numberOfGrids();
      integrateForceOnGridFace.redim(2,3,numberOfGrids);
      integrateForceOnGridFace=FALSE;

      integrate.updateToMatchGrid(cg);
  
    }
    //
    // getEnergy
    //
    else if( answer=="get energy" )
    {
      //VE

      //if( it>0 )
      //{
      //  ps.inputString(line,sPrintF(buff,"Enter the solution number, [1,%i] \n",numberOfSolutions));
      //sscanf(line,"%i",&solutionNumber);
      //}

      // int maxSolutionNumber=101;
      
      int nx=121, ny=121;
      real xa=-2., xb=2.;
      real ya=-2., yb=2.;
      
      real IntValue;
      IntValue=0;
      real MaxTempValue, focusTempValue;
      MaxTempValue=0;
      real densityValue;

      FILE *extFile = fopen("maxTemp.txt", "w");

      for( int solutionNumber=1; solutionNumber<numberOfSolutions; solutionNumber++ )
      {
	  
	ShowFileReader::ReturnType rt;
        rt = showFileReader.getASolution(solutionNumber,cg,u);        // read in a grid and solution
        if( rt==ShowFileReader::notFound )
	{
	  printf("getEnergy:ERROR: solution %i was not found!\n",solutionNumber);
	  continue;
	}
	  
	// read any header comments that go with this solution
	headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);
	  
	for( int j=0; j<numberOfHeaderComments; j++ )
	  printf("Header comment: %s \n",(const char *)headerComment[j]);
	  
	HDF_DataBase & db = *showFileReader.getFrame();
	time=0.;
	db.get(time,"t");
	printf("time=%e ",time);
	  
	numberOfGrids=cg.numberOfGrids();


	// integrateForceOnGridFace.redim(2,3,numberOfGrids);
	// integrateForceOnGridFace=FALSE;
	// integrate.updateToMatchGrid(cg);
	  
	printf("getEnergy: interpolate an array of points from solution number %i:  nx=%i, ny=%i, xa=%g, xb=%g, ya=%g, yb=%g\n",
                solutionNumber,nx,ny,xa,xb,ya,yb);
	InterpolatePoints interpPoints;
	  
	int numberOfPoints=nx*ny;
	realArray pts(numberOfPoints,3);
	  
	int i=0;
	for( int i1=0; i1<nx; i1++ )
	{
	  for (int i2=0; i2<ny; i2++ )
	  {
	    real x0 = xa + i1*(xb-xa)/(nx-1);
	    real y0 = ya + i2*(yb-ya)/(ny-1);
	    pts(i,0)=x0;
	    pts(i,1)=y0;
	    i++;
	  }
	}
	  
	Range C=u.getComponentDimension(0); //  interpolate all components rho,u,v,T,p
	Range N=numberOfPoints;
	realArray values(N,C);
	  
       
        // -- These next lines can go away with the latest version of InterpolatePoints.C ---
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
 	{
 	  if( cg[grid].isRectangular() )
            cg[grid].update( MappedGrid::THEboundingBox );
	  else
 	    cg[grid].update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEboundingBox );
 	}
        // ----------------------------------
	
	interpPoints.interpolatePoints( pts, u, values, C);
	  

	//FILE *extFile = fopen("dataCirc.txt", "w");
	//FILE *extFile = fopen(filename, "w");
	 
	i=0;
	for( int i1=0; i1<nx; i1++ )
	{
	  for (int i2=0; i2<ny; i2++ )
	  {
	    //fprintf(extFile," %12.5e %12.5e",pts(i,0),pts(i,1));
	    for( int n=values.getBase(1); n<=values.getBound(1); n++ )
	    {
	      if( abs(values(i,n))<1e-99 ) // VE for att matlab 
	      {
		values(i,n)=0.0;
	      }
	      //fprintf(extFile," %12.5e",values(i,n));
	    }
	    //fprintf(extFile,"\n");
	    i++;
	  }    
	}
	// fclose(extFile);
	  
	  
	real dx = (xb-xa)/(nx-1);
	real dy = (yb-ya)/(ny-1);

	i=0;
	real cv=717.65; // cv=R/(gamma-1)

	printf(" dx= %16.10e  dy= %16.10e  \n",dx,dy);

	for( int i1=0; i1<nx-1; i1++ )
	  for (int i2=0; i2<ny-1; i2++ )
	  {	 
		
	    IntValue=IntValue+dx*dy*values(i,0)*(0.5*(values(i,1)*values(i,1)
						      +values(i,2)*values(i,2))
						 +cv*values(i,3));
		
	    MaxTempValue = (values(i,3)>MaxTempValue) ? values(i,3):MaxTempValue;
	    densityValue = values(i,0);
		
	    if(i1==60)
	      if(i2==60)
	      {
		focusTempValue=values(i,3);
	      }
		
	    i++;
	  }


	fprintf(extFile,"%i %e %16.10e %16.10e %16.10e \n",solutionNumber,time, IntValue,MaxTempValue, densityValue); 
	printf("MaxTemp = %16.10e\n",MaxTempValue);
	MaxTempValue=0;
      }
      fclose(extFile);
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      cout << "Invalid answer :" << answer << endl;
    }
  }
  
  Overture::finish();      
  return 0;
}


