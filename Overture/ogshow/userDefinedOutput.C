#include "Overture.h"
#include "PlotIt.h"
#include "GraphicsParameters.h"
#include "ShowFileReader.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

int PlotIt::
userDefinedOutput(const realGridCollectionFunction & uv, 
                  GraphicsParameters & par,
                  const aString & callingFunctionName )
// =================================================================================================
// /Desctiption:
// 
// This routine is called from the contour plotter, for example, when the user chooses
// the "user defined output" option. A user can write a new version of this file in order
// to output values to a file in any given format. 
//
//  You can either change this file, and rebuild the Overture library, or you can make a copy of this
// file, change it, compile it and then link it ahead of the Overture library when building plotStuff
// or some other application.
//
//  /uv (input) : solution values in a grid function
//  /par (input) : graphics parameters (may include a pointer to a show file reader.
//  /callingFunctionName (input) : name of the calling function
// =================================================================================================
{

  GridCollection & gc = *uv.getGridCollection();

  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface(); // graphics interface

  // Here is a pointer to a ShowFileReader (which may be non-NULL if this function was called through plotStuff)
  ShowFileReader *pShowFileReader=(ShowFileReader*)par.showFileReader;
  int solutionNumber = par.showFileSolutionNumber;
  

  printf("userDefinedOutput: callingFunctionName=%s\n",(const char*)callingFunctionName); 

  // here is a menu of possible options
  aString menu[]=  
    {
      "file name",
      "save results",
      "save solution on a coordinate plane",
      "exit",
      ""
    };
  aString answer,answer2;
  gi.appendToTheDefaultPrompt(">user defined");

  aString fileName="user.dat"; // default file name

  aString pdeName="unknown";
  aString reactionName="unknown";
  int rc=-1, uc=-1, vc=-1, wc=-1, tc=-1, pc=-1, numberOfSpecies=0;
  real time=0., dt=0.;

  if( pShowFileReader!=NULL )
  {
    // Look for parameters in the show file
    // (These values are saved, for example, in OverBlown when the show file is written)

    ShowFileReader & showFileReader = *pShowFileReader;
    bool found;

    found=showFileReader.getGeneralParameter("pde",pdeName);
    printf(" pdeName =%s (from ShowFile)\n",(const char*)pdeName);
      
    found=showFileReader.getGeneralParameter("densityComponent",rc);
    found=showFileReader.getGeneralParameter("uComponent",uc);
    found=showFileReader.getGeneralParameter("vComponent",vc);
    found=showFileReader.getGeneralParameter("wComponent",wc);
    found=showFileReader.getGeneralParameter("temperatureComponent",tc);
    found=showFileReader.getGeneralParameter("pressureComponent",pc);

    found=showFileReader.getGeneralParameter("numberOfSpecies",numberOfSpecies);

    found=showFileReader.getGeneralParameter("reactionType",reactionName);

    // get parameters from the current frame
    HDF_DataBase *dbp= showFileReader.getFrame();
    assert( dbp!=NULL );
    HDF_DataBase & db = *dbp;
    db.get(time,"time");
    db.get(dt,"dt");
    printf(" time=%e, dt=%e (from ShowFile)\n",time,dt);
      
  }
    
  for( ;; ) // loop to check for an answer
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="file name" )
    {
      gi.inputString(fileName,sPrintF(answer,"Enter the name of the file (default=%s)\n",(const char*)fileName));
      if( fileName=="" || fileName==" " ) fileName="user.dat";
    }
    else if( answer=="save results" )
    {

      const int numberOfComponents = uv.getComponentDimension(0); // number of variables: rho,u,v,...
	

      if( gc.numberOfDimensions()==1 )
      {
	// *** Save results from contour cut where the solution is saved on a line ***

        // if callingFunctionName=="contour1d" then the next two components show be available:
	const int xc = numberOfComponents-2;    // component where the x-coordinate is stored
	const int yc = xc+1;                    // component where the y-coordinate is stored

	const int ca=0, cb=numberOfComponents-3;  // [ca,cb] : components where solution variables are stored

        printf("numberOfComponents=%i, ca=%i, cb=%i\n",numberOfComponents,ca,cb);
	for( int n=0; n<numberOfComponents; n++ )
	{
	  printf("component %i, name=[%s] \n",n,(const char*)uv.getName(n));
	}
	
	printf("Saving results to the file=[%s]\n",(const char*)fileName );
	
	FILE *file = fopen((const char*)fileName,"w" );  // open the file
  

	fprintf(file,"%e %i %i %i\n",time,numberOfComponents,1,1);
	int grid;
	for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  realMappedGridFunction & u = uv[grid];
	  const RealDistributedArray & x = gc[grid].center();
	  const IntegerDistributedArray & mask = gc[grid].mask();
	  Index I1,I2,I3;
	  getIndex(gc[grid].gridIndexRange(),I1,I2,I3);

	  fprintf(file,"%i %i\n",I1.getLength(),1);

// 	    if( numberOfComponentsToPlot==5 )
// 	      fprintf(file,"spec-volume velocity    temperature pressure    product\n");
// 	    else if( numberOfComponentsToPlot==6 )
// 	      fprintf(file,"spec-volume velocity    temperature pressure    product     radical\n");
// 	    else
// 	      fprintf(file,"solid frac  density(s)  velocity(s) pressure(s) density(g)  velocity(g) pressure(g)\n");

	  // output the names of the components:
          int n;
          fprintf(file,"x           y           ");
//        fprintf(file,"solid frac  density(s)  velocity(s) pressure(s) density(g)  velocity(g) pressure(g)\n");
	  for( n=ca; n<=cb; n++ )
	  {
	    fprintf(file,"component(%i)",n);
//          fprintf(file,"solid frac  density(s)  velocity(s) pressure(s) density(g)  velocity(g) pressure(g)\n");
	  }
	  fprintf(file,"\n");
	
	  intArray cMask(I1);
	  cMask=mask(I1,I2,I3)!=0 && mask(I1+1,I2,I3)!=0; //  && x(I1)>=xBound(Start,0) && x(I1)<=xBound(End,0);
	  // on refinement grids do not plot cells with all corners hidden by refinement
	  if( gc.numberOfRefinementLevels()>1 )
	    cMask=cMask && !( (mask(I1  ,I2  ,I3) & MappedGrid::IShiddenByRefinement) ||
			      (mask(I1+1,I2  ,I3) & MappedGrid::IShiddenByRefinement) );

	  int i1, i2=0,i3=0;
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    fprintf(file," %e ",x(i1,i2,i3,0));  // x holds the normalized distance
	    fprintf(file,"%e %e ",u(i1,i2,i3,xc),u(i1,i2,i3,yc));  // (x,y) coordinates
          
	    for( n=ca; n<=cb; n++ )
	    {
	      fprintf(file,"%e ",u(i1,i2,i3,n));  // output solution values
	    }

	    fprintf(file,"%i ",(cMask(i1)? 0 : 1));  // mask value

	    fprintf(file,"\n");
	  }
	}
	
	fclose(file);
      }
      else
      {
	// **** This is a 2D or 3D solution ****

	// nothing implemented in this case

        printf("No results saved -- this option not implemented yet for a 2D or 3D grid function\n");
      }
	
    }
    else if( answer=="save solution on a coordinate plane" )
    {
      CompositeGrid & cg = (CompositeGrid&)gc;

      int grid=0;
      gi.inputString(answer,sPrintF(answer,"Enter the grid number (from 0,...,%i)\n",cg.numberOfComponentGrids()-1));
      sScanF(answer,"%i",&grid);
      if( grid<0 || grid>cg.numberOfComponentGrids()-1 )
      {
	printF("ERROR: invalid value for grid=%i. Choosing grid=0.\n",grid);
	grid=0;
      }
      int coord=0;
      gi.inputString(answer,sPrintF(answer,"Enter the coordinate direction (0,1, or 2)\n"));
      sScanF(answer,"%i",&coord);
      coord=max(0,min(cg.numberOfDimensions()-1,coord));
      printF("chooing a plane in coordinate direction = %i.\n",coord);
      
      RealArray x(1,3), r(1,3);
      x=0.; r=-1.;
      gi.inputString(answer,sPrintF(answer,"Enter a point (x,y,z) on (or near) the plane\n"));
      sScanF(answer,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
      
      MappedGrid & mg = cg[grid];
      Mapping & map = mg.mapping().getMapping();
      r=-1.;
      map.inverseMapS(x,r);

      if( max(fabs(r))>5. )
      {
	printF("ERROR inverting the mapping. This point must not be in the grid!? You must start again.\n");
        continue;
      }
      
      // find the closest point grid point:
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      i1=i2=i3=0;
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
        const real shift = r(0,axis)>=0. ? .5 : -.5;
	iv[axis]=int(r(0,axis)/mg.gridSpacing(axis)+shift)+mg.gridIndexRange(0,axis);
      }
      printF(" x=(%9.3e,%9.3e,%9.3e) -> r=(%9.3e,%9.3e,%9.3e) : closest grid pt: iv=(%i,%i,%i)\n",
             x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2),i1,i2,i3);


      printF("I will save the solution on the coordinate plane axis%i=%i,  bounds=[%i,%i]\n",coord,iv[coord],
	     mg.gridIndexRange(0,coord),mg.gridIndexRange(1,coord));
      
      printF("Here are the solution components:\n");
      for( int c=uv.getComponentBase(0); c<=uv.getComponentBound(0); c++ )
      {
        printF("%i : %s\n",c,(const char*)uv.getName(c));
      }

      IntegerArray cc;
      int numberOfComponentsToSave=gi.getValues("Enter a list of components to save (`done' to finish)..",cc);

      printF("Saving components:\n");
      for( int c=0; c<numberOfComponentsToSave; c++ )
      {
        printF("%i : %s\n",cc(c),(const char*)uv.getName(cc(c)));
      }
      

      std::vector<aString> header;
      for( ;; )
      {
	gi.inputString(answer,sPrintF(answer,"Enter a header comment (`done' to finish)"));
	if( answer == "done" ) break;
        printF("Header=[%s]\n",(const char*)answer);
	header.push_back(answer);
      }
      
      realArray & u = uv[grid];
      
      mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
      realArray & vertex= mg.vertex();



      Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      Iv[coord]=iv[coord];


      const int myid=max(0,Communication_Manager::My_Process_Number);
      const int graphicsProcessor = gi.getProcessorForGraphics();
      realArray u0, vertex0;  // here are the distributed arrays that live on one processor.

      #ifdef USE_PPP
        // -- In parallel we copy the data we need to a single processor that can write to a file ---


        // build a partition for arrays that just lives on the graphicsProcessor.
        Partitioning_Type partition; 
        partition.SpecifyProcessorRange(Range(graphicsProcessor,graphicsProcessor)); 
        for( int axis=0; axis<4; axis++ )
        {
          int ghost=0; // uPartition.getGhostBoundaryWidth(axis);
          if( ghost>0 )
            partition.partitionAlongAxis(axis, true , ghost);
          else
            partition.partitionAlongAxis(axis, false, 0);
        }

	u0.partition(partition);
	vertex0.partition(partition);

        Iv[3]=Range(uv.getComponentBase(0),uv.getComponentBound(0));
	u0.redim(Iv[0],Iv[1],Iv[2],Iv[3]);
        int nd=4;
	ParallelUtility::copy(u0,Iv,u,Iv,nd);           // copy data from processor p to graphics processor

        Iv[3]=Range(cg.numberOfDimensions());
	vertex0.redim(Iv[0],Iv[1],Iv[2],Iv[3]);
	ParallelUtility::copy(vertex0,Iv,vertex,Iv,nd); // copy data from processor p to graphics processor

      #endif

      OV_GET_SERIAL_ARRAY(real,u0,uLocal);
      OV_GET_SERIAL_ARRAY(real,vertex0,vertexLocal);


      // File format:
      //  #  Header comments 
      //  #
      //  #
      //  nc        (number of components)
      //  nx ny nz  (grid points)
      //  x0 y0 z0 u0 u1 ...
      //  x1 y1 z1 u0 u1 ...

      if( myid == graphicsProcessor )
      {
	printF("Saving results to the file=[%s]\n",(const char*)fileName );
	
	FILE *file = fopen((const char*)fileName,"w" );  // open the file

	for( int i=0; i<header.size(); i++ )
	{
	  fprintf(file,"# %s\n",(const char*)header[i]);
	}
	fprintf(file,"%i        (number of components)\n",numberOfComponentsToSave);
	for( int axis=0; axis<3; axis++ )
	{
	  fprintf(file,"%i ",Iv[axis].getLength());
	}
	fprintf(file,"  (grid points)\n");
	if( cg.numberOfDimensions()==2 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    fprintf(file,"%e %e ",vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1));
	    for( int c=0; c<numberOfComponentsToSave; c++ )
	    {
	      fprintf(file,"%e ",uLocal(i1,i2,i3,cc(c)));
	    }
	    fprintf(file,"\n");
	  }
	}
	else if( cg.numberOfDimensions()==3 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    fprintf(file,"%e %e %e ",vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1),vertexLocal(i1,i2,i3,2));
	    for( int c=0; c<numberOfComponentsToSave; c++ )
	    {
	      fprintf(file,"%e ",uLocal(i1,i2,i3,cc(c)));
	    }
	    fprintf(file,"\n");
	  }
	}

	fclose(file);
      }
      
    }
    else 
    {
      printF("Unknown answer =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
  }
  gi.unAppendTheDefaultPrompt();

  return 0;
}

