// ==========================================================================================
//  Test the SurfaceStitcher class for stitching overlapping surface grids
//
// Examples:
//    stitcher -cmd=sib.stitch
//    stitcher -cmd=ellipsoid.stitch
//    stitcher -cmd=quarterSphere.stitch
//    stitcher -cmd=boxsbs.stitch
// ==========================================================================================



// #define BOUNDS_CHECK
#define OV_DEBUG

/** TODO
 * DONE 1. change updateHybrid to allow an automatic update w/o going through
           the gui.
 * DONE (ANS: NOT NOW) 2. should we return the mapping of the boundary vertices between the grids and
       the unstructured mesh?
 * DONE   3. add a field (UnstructuredMapping) to CompositeGrid to store the stiching
*/

#include "Overture.h"
#include "ReductionMapping.h"
#include "CompositeGrid.h"

#include "Ugen.h"
#include "display.h"
#include "BodyDefinition.h"

#include "SurfaceStitcher.h"


void 
buildSurfaceCompositeGrid(CompositeGrid &cg, CompositeGrid &cgSurf, BodyDefinition *bodyDefinition=NULL);

#ifdef STITCHER_MAIN

int stitchCompositeGrid(CompositeGrid &cg);

int stitchSurfaceCompositeGrid( CompositeGrid & cgSurf, 
				Ugen & ugen,
				int option= 1 );

int main(int argc, char *argv[])
{

  Overture::start(argc,argv);

  printF("Usage: stitcher [-cmd=<file.cmd>] [-noplot] [-grid=gridName]\n");

  GenericGraphicsInterface &gi = *Overture::getGraphicsInterface();
  GraphicsParameters gp;
  gp.set(GI_PLOT_UNS_EDGES,true);
  gp.set(GI_PLOT_UNS_FACES,true);
  gp.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByGrid);
  
  MappingInformation mapInfo;
  mapInfo.graphXInterface = &gi;
  gi.appendToTheDefaultPrompt("stitcher>");

  CompositeGrid cg;

  aString fileName;

  aString commandFileName="";
  int plotOption=true;



  if ( argc==1 )
  {
    gi.inputFileName(fileName, "", ".hdf");
  }
  else
  {
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
	printF("Reading commands from file=[%s]\n",(const char*)commandFileName);
      }
      else if( len=line.matches("-grid=") )
      {
        fileName=line(len,line.length()-1);
	printF("grid file name =[%s]\n",(const char*)fileName);
      }
      else
      {
	printF("Unknown option=[%s]\n",(const char*)line);
      }
    }
  }
  
  // Start saving a command file...
  aString logFile="stitcher.cmd";
  gi.saveCommandFile(logFile);
  gi.outputToCommandFile(fileName+"\n");  // first command in the command file is the grid file name
  
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  gi.appendToTheDefaultPrompt("stitcher>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
    gi.inputString(fileName,"Enter the name of the grid file");
  }

  getFromADataBase(cg,fileName);

  if ( cg.numberOfDimensions()!=3 )
    {
      cout<<"stitcher only runs in 3d"<<endl;
      return 1;
    }

  // Here is the object that knows how to stitch surfaces.
  SurfaceStitcher stitcher;

  // Here is the object used to define which surfaces to stitch -- by default stitch all surfaces.
  BodyDefinition bd;

  int minDiscretizationWidth=INT_MAX;
  int minInterpolationWidth=INT_MAX;
  real overlapWidth=0.;
  Range R=cg.numberOfDimensions();
  const IntegerArray & iw = cg.interpolationWidth;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const IntegerArray & dw = mg.discretizationWidth();
    minDiscretizationWidth=min(minDiscretizationWidth,min(dw(R)));
    for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
    {
      MappedGrid & mg2 = cg[grid];
      if( grid!=grid2 )
	minInterpolationWidth=min( minInterpolationWidth,min(iw(R,grid,grid2)));

      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	int width, l=0;
	if( cg.interpolationIsImplicit(grid,grid2,l) || cg.refinementLevelNumber(grid)>0 )
	  width = max(cg.interpolationWidth(axis,grid,grid2,l),mg2.discretizationWidth(axis)) - 2;
	else
	  width=cg.interpolationWidth(axis,grid,grid2,l) + mg2.discretizationWidth(axis) - 3;

        overlapWidth =  max(overlapWidth, .5*width );
      }
    }
  }
  printf(" *** minDiscretizationWidth=%i, minInterpolationWidth=%i overlapWidth=%g ****\n",minDiscretizationWidth,
	 minInterpolationWidth,overlapWidth);


  // By default increase the gap between overlapping surface grids by this many extra grid lines:
  int gapWidth=int(overlapWidth/2. + 2.5);  
  gapWidth=-gapWidth;   // <0 mean use automatic algorithm

  real minGapSizeInGridLines=.5;
  int extraGapWidth=1;


  GUIState dialog;

  dialog.setWindowTitle("stitcher");
  dialog.setExitCommand("exit", "exit");

  // option menus
  // dialog.setOptionMenuColumns(1);

  aString cmds[] = {"stitch surfaces",
		    "stitch all surfaces",
                    "define a surface",
                    "change volume grid plot",
                    "change surface grid plot",
		    ""};
  int numberOfPushButtons=9;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  bool plotVolumeGrid=false;
  bool plotSurfaceGrid=true;
  bool plotStitching=true;
  bool interactiveStitcher=true;
  aString tbCommands[] = {"plot volume grid",
                          "plot surface grid",
			  "plot stitching",
                          "interactive stitcher",
			  ""};
  int tbState[10];
  tbState[0] = plotVolumeGrid; 
  tbState[1] = plotSurfaceGrid; 
  tbState[2] = plotStitching;
  tbState[3] = interactiveStitcher;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=7;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "gap width:";  sPrintF(textStrings[nt],"%i (<0 -> use min gap size)",gapWidth);  nt++; 
  textLabels[nt] = "min gap size:";  sPrintF(textStrings[nt],"%g",minGapSizeInGridLines);  nt++; 
  textLabels[nt] = "extra gap width:";  sPrintF(textStrings[nt],"%i",extraGapWidth);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // dialog.buildPopup(menu);
  gi.pushGUI(dialog);

  bool stitchAllSurfaces=false;
  aString answer;
  int len=0;
  for( int it=0; ; it++)
  {
    bool compute=false;
    if( it==0 )
      answer="plot";
    else
    {
      gi.getAnswer(answer,"");
    }
     
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="stitch surfaces" )
    {
      compute=true;
    }
    else if( answer=="stitch all surfaces" )
    {
      compute=true;
      stitchAllSurfaces=true;
    }
    else if( answer=="change volume grid plot" )
    {
      gi.erase();
      PlotIt::plot(gi,cg,gp);
    }
    else if( answer=="change surface grid plot" )
    {
      if( stitcher.getSurfaceCompositeGrid()!=NULL )
      {
	gi.erase();
	PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);
      }
    }
    else if( dialog.getToggleValue(answer,"plot volume grid",plotVolumeGrid) ){}//    
    else if( dialog.getToggleValue(answer,"plot surface grid",plotSurfaceGrid) ){}//    
    else if( dialog.getToggleValue(answer,"plot stitching",plotStitching) ){}//    
    else if( dialog.getToggleValue(answer,"interactive stitcher",interactiveStitcher) ){}//    
    else if( dialog.getTextValue(answer,"gap width:","%i",gapWidth) ){}//
    else if( dialog.getTextValue(answer,"min gap size:","%e",minGapSizeInGridLines) ){}//
    else if( dialog.getTextValue(answer,"extra gap width:","%i",extraGapWidth) ){}//
    else if( answer.matches("define a surface") )
    {
	  
      int surfaceNumber=-1,grid=0,side=0,axis=0;
      gi.inputString(answer,"Enter the surface number (0,1,2,...)");
      sScanF(answer,"%i",&surfaceNumber);
      int maxNumberOfFaces=100;
      IntegerArray boundary(3,maxNumberOfFaces);
      int numberOfFaces=0;
      for( ;; )
      {
	gi.inputString(answer,"Enter grid, side, axis (for a face on the surface) Enter `done' to finish)");
	if( answer=="done" )
	{
	  break;
	}
	else
	{

	  sScanF(answer(len,answer.length()-1),"%i %i %i",&grid,&side,&axis);
          if( grid<0 || grid>=cg.numberOfGrids() || side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
	  {
            printF("ERROR: invalid input: grid=%i side=%i axis=%i\n",grid,side,axis);
	  }
	  else
	  {
	    if( numberOfFaces>=maxNumberOfFaces )
	    {
              maxNumberOfFaces*=2;
	      boundary.resize(3,maxNumberOfFaces);
	    }
	    printF("Adding face %i : (grid,side,axis)=(%i,%i,%i) to surface %i\n",numberOfFaces,
                    grid,side,axis,surfaceNumber);
	    boundary(0,numberOfFaces)=side;
	    boundary(1,numberOfFaces)=axis;
	    boundary(2,numberOfFaces)=grid;
	    numberOfFaces++;
	  }
	}
      }
      if( numberOfFaces>0 )
      {
	bd.defineSurface( surfaceNumber,numberOfFaces,boundary ); 
      }
      
    }
    else if( answer=="plot" )
    {
    }
    else
    {
      printF("ERROR: unknown answer=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
	     
    if( compute )
    {
      if( true )
      {
	// newer way

	if( bd.totalNumberOfSurfaces()>0 && !stitchAllSurfaces )
	  stitcher.defineSurfaces( cg,&bd );
	else
	  stitcher.defineSurfaces(cg);  // choose all boundary surfaces

	if( false && plotSurfaceGrid && stitcher.getSurfaceCompositeGrid()!=NULL )
	{

          gp.set(GI_TOP_LABEL,"Before enlarge gap");
          gi.erase();
	  PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);
	}
	
	if( gapWidth>=0  )
          stitcher.enlargeGap(gapWidth);
        else 
          stitcher.enlargeGapWidth( minGapSizeInGridLines,extraGapWidth );  // automatic gap widening

	if( plotSurfaceGrid && stitcher.getSurfaceCompositeGrid()!=NULL )
	{

          gp.set(GI_TOP_LABEL,"After enlarge gap");
          gi.erase();
	  PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);

	  if( false )
	  {
	    
	    stitcher.setMask(SurfaceStitcher::originalMask);
	  
	    gp.set(GI_TOP_LABEL,"After setMask(originalMask)");
	    gi.erase();
	    PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);

	    stitcher.setMask(SurfaceStitcher::enlargedHoleMask);

	    gi.erase();
	    gp.set(GI_TOP_LABEL,"After setMask(enlargedHoleMask)");
	    PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);
	  }
	  
	}

	stitcher.stitchSurfaceCompositeGrid(interactiveStitcher);

	cg.setSurfaceStitching( stitcher.getUnstructuredGrid() );
	
      }
      else if( false )
      {
	stitchCompositeGrid(cg);
      }
      else if( false )
      {
	// new way:  
	//  Here we have access to the surface composite grid and Ugen object (for the unstructured stitched grid)
	CompositeGrid cgSurf;
    
	if( bd.totalNumberOfSurfaces()>0 && !stitchAllSurfaces )
	  buildSurfaceCompositeGrid(cg, cgSurf, &bd );
	else
	  buildSurfaceCompositeGrid(cg, cgSurf );

	gi.erase();
        gp.set(GI_TOP_LABEL,"surface grids");
        PlotIt::plot(gi,cgSurf,gp);

	if( true && cgSurf.numberOfGrids()>6 )
	{
	  displayMask(cgSurf[6].mask(),"cgSurf[6].mask() BEFORE stitch");
	}
    
	Ugen ugen(*Overture::getGraphicsInterface());

	stitchSurfaceCompositeGrid(cgSurf,ugen);

	if( true && cgSurf.numberOfGrids()>6 )
	{
	  displayMask(cgSurf[6].mask(),"cgSurf[6].mask() AFTER stitch");
	}

	cg.setSurfaceStitching( ugen.getUnstructuredMapping() );

      }

    }

    gp.set(GI_PLOT_UNS_EDGES,true);
    gp.set(GI_PLOT_UNS_FACES,true);

    gi.erase();
    aString label; sPrintF(label,"stitcher: grid=%s",(const char*)fileName);
    gp.set(GI_TOP_LABEL,label);
    gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    if( plotVolumeGrid )
	PlotIt::plot(gi,cg,gp);

    if( plotSurfaceGrid && stitcher.getSurfaceCompositeGrid()!=NULL )
    {
      PlotIt::plot(gi,*stitcher.getSurfaceCompositeGrid(),gp);
    }
    else
    {
      if( plotStitching && cg.getSurfaceStitching()!=NULL )
	PlotIt::plot(gi,*cg.getSurfaceStitching(),gp);
    }
    
    gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

  }
  


  //  bd.defineSurface(const int & surfaceNumber, const int & numberOfFaces_, IntegerArray & boundary )
  //  boundary(3,numberOfFaces) : (side,axis,grid)=boundary(0:2,i) i=0,1,...numberOfFaces.
//   if( true && fileName.matches("sib") )
//   {
//     int surface=0;
//     int numberOfFaces=2;
//     IntegerArray boundary(3,numberOfFaces);
//     int side=0, axis=axis3, grid=1;
//     boundary(0,0)=side;
//     boundary(1,0)=axis;
//     boundary(2,0)=grid;
//     grid=2;
//     boundary(0,1)=side;
//     boundary(1,1)=axis;
//     boundary(2,1)=grid;
//     bd.defineSurface( 0,numberOfFaces,boundary ); 
//   }
  
  


  Overture::finish();

  return 0;
}

#endif

int 
stitchSurfaceCompositeGrid( CompositeGrid & cgSurf, 
                            Ugen & ugen,
                            int option /* = 1 */ )
// =================================================================================
// 
//              Surface grid stitcher
//
//  /Description:
//  
//   Build the unstructured grid that joins the patches on a composite grid 
//   for (one or more) surfaces
// 
// /cgSurf (input) : composite grid with surface grids
// /ugen (input/output) : use this Ugen object. On output this object will hold the
//   unstructured stitched grid. 
// /option (input) : 1=run interactively, 0=run non-interactively
// 
// =================================================================================
{
  if ( cgSurf.numberOfGrids()>0 )
  {

    cout<<"There are "<<cgSurf.numberOfGrids()<<" surface grids"<<endl;

    if( option==1 )
    {
      // bring up the hybrid mesh interface

      MappingInformation mapInfo;
      mapInfo.graphXInterface = Overture::getGraphicsInterface();
      ugen.updateHybrid(cgSurf,mapInfo);
    }
    else
      ugen.updateHybrid(cgSurf); // try to build the mesh

  }
  else
    cout<<"WARNING::stitchSurfaceCompositeGrid:: no valid surface grids were found"<<endl;

  // now we should have a surface grid, what to do with it?

  return 0;
}



int stitchCompositeGrid(CompositeGrid &cg)
{
  // surf_cg will be the overlapping surface grid to be stitched
  //    it will be constructed using bc and share info from cg
  CompositeGrid surf_cg; 

  buildSurfaceCompositeGrid(cg, surf_cg);

  //  cg.update();

  // bring up the hybrid mesh interface
  if ( surf_cg.numberOfGrids()>0 )
    {

      cout<<"There are "<<surf_cg.numberOfGrids()<<" surface grids"<<endl;
      Ugen ugen(*Overture::getGraphicsInterface());

      if ( true )
        {
           MappingInformation mapInfo;
           mapInfo.graphXInterface = Overture::getGraphicsInterface();
           ugen.updateHybrid(surf_cg,mapInfo);
        }
      else
         ugen.updateHybrid(surf_cg); // try to build the mesh

      cg.setSurfaceStitching( ugen.getUnstructuredMapping() );

    }
  else
    cout<<"WARNING::stitchCompositeGrid:: no valid surface grids were found"<<endl;

  // now we should have a surface grid, what to do with it?

  return 0;
}

void 
buildSurfaceCompositeGrid(CompositeGrid &cg, CompositeGrid &cgSurf, BodyDefinition *bodyDefinition /* =NULL */ )
// ==============================================================================================
// /Description:
//    Build a CompositeGrid holding surfaces. 
//
// /cg (input): build surfaces from this volume grid.
// /cgSurf (output): CompositeGrid that holds the surfaces.
// /bodyDefinition (input): if not NULL, this is a pointer to a BodyDefinition object that defines
//  the the surfaces as a collection of faces.
// 
// /Note: 
//    kkc: most of this code has been cut and pasted from sealHoles.C, sealHoles3D 
// /Authors:
//   kkc - initial version
//   wdh - added BodyDefintion option
// ================================================================================================
{

  int grid;
  int side,axis;

  const bool useAllBoundaries = bodyDefinition==NULL;
  
  BodyDefinition & bd = bodyDefinition!=NULL ? *bodyDefinition : *new BodyDefinition();

  // If no BodyDefintion is supplied, create a BodyDefinition that holds all boundary faces:
  if( useAllBoundaries )
  {
    const int maxNumberOfFaces=cg.numberOfGrids()*6;
    IntegerArray boundary(3,maxNumberOfFaces);  
    
    int numberOfFaces=0;  // counts boundary faces
    for( grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      // loop through each side of each axis of mg looking for surfaces to add
      for( int axis=0; axis<mg.domainDimension(); axis++ )
      {
	for( int side=0; side<2; side++ )
	{
	  if( (mg.boundaryFlag(side,axis)==MappedGrid::physicalBoundary ||
	       mg.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary) &&
               // -20 is magic meaning don't add this physical surface to the surface grid: 
	      mg.sharedBoundaryFlag(side,axis)!=-20) 
	  {	    
	    boundary(0,numberOfFaces)=side;
	    boundary(1,numberOfFaces)=axis;
	    boundary(2,numberOfFaces)=grid;
	    numberOfFaces++;
	  }
	}
      }
    }
    if( numberOfFaces>0 )
      bd.defineSurface( 0,numberOfFaces,boundary ); 
  }
  
  const int numberOfSurfaces=bd.totalNumberOfSurfaces();
  for( int surf=0; surf<numberOfSurfaces; surf++ )
  {
    const int numberOfFaces=bd.numberOfFacesOnASurface(surf);
    for( int face=0; face<numberOfFaces; face++ )
    {
      int grid,side,axis;
      bd.getFace(surf,face,side,axis,grid);
      
      MappedGrid & mg = cg[grid];

      // Build a Mapping for this face by using a ReductionMapping
      ReductionMapping *redMap = new ReductionMapping(*(mg.mapping().mapPointer), axis, real(side));
      redMap->incrementReferenceCount();

      real sj = mg.mapping().mapPointer->getSignForJacobian();

      // if ( axis==1 ) sj *= -1;  // *wdh* 070223 -- this looks wrong
      if ( side==1 ) sj *= -1;
		
      cgSurf.add(*redMap);

      MappedGrid & mgSurf = cgSurf[cgSurf.numberOfGrids()-1];
      mgSurf.update(MappedGridData::THEmask | MappedGrid::THEboundingBox);
      
      intArray &mask = mgSurf.mask();
      intArray &mgmask = mg.mask();

      Index Ib1, Ib2, Ib3;
      int ie[3]; ie[0] = ie[1] = ie[2] = 1;
      ie[axis] = 0;
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
      mask = -1;

      if ( axis==axis1 )
      {
	for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
	  for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
	  {
	    mask(i,j,0) = mgmask(Ib1.getBase(),i,j);
	  }
      }
      else if ( axis==axis2 )
      {
	for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
	  for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
	  {
	    mask(i,j,0) = mgmask(i,Ib2.getBase(),j);
	  }
      }
      else
      {
	for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
	  for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
	  {
	    mask(i,j,0) = mgmask(i,j,Ib3.getBase());
	  }
      }
		
      ApproximateGlobalInverse & agi = *mgSurf.mapping().mapPointer->approximateGlobalInverse;
      

      agi.setParameter(MappingParameters::THEboundingBoxExtensionFactor, 0.);
      agi.setParameter(MappingParameters::THEstencilWalkBoundingBoxExtensionFactor, 0.);
      agi.initialize();
      mgSurf.mapping().mapPointer->setSignForJacobian(sj);

      //		cout<<"REDUCTION MAPPING "<<gid<<" BBOX "<<endl;
      //mgSurf.mapping().mapPointer->approximateGlobalInverse->getBoundingBox().display();

      if ( (redMap->decrementReferenceCount()) == 0 ) delete redMap; 

    } // end for face
  } // end for surface
  
  
  if( bodyDefinition==NULL ) delete & bd;
  

}

