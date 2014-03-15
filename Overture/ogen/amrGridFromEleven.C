// ====================================================================================================================
//  Changes:
//  o 2012/03/09 - initial version from Kyle checked into Overture/ogen cvs repo.
//  o 2012/03/09 - changed mask to be -1 inside bodies and 1 outside -- eventually make this a signed distance.
// ====================================================================================================================
#include <vector>
#include <fstream>
#include <iostream>

// Overture include files
#include "Overture.h"
#include "CompositeGrid.h"
#include "PlotIt.h"
#include "CompositeGridFunction.h"
#include "BoxMapping.h"
#include "SquareMapping.h"
#include "Regrid.h"
#include "ErrorEstimator.h"
#include "InterpolateRefinements.h"
#include "CompositeGridOperators.h"
#include "OGgetIndex.h"
#include "Ogen.h"
#include "Ogshow.h"
#include "GridStatistics.h"

// undefine the following 2 overture things so they don't confuse eleven...
#undef RANGE_CHK 
#undef REAL_EPSILON

// Eleven include files
#include "kk_defines.hh"
#include "DBase.hh"
#include "model.hh"
#include "eleven_geometry.hh"
#include "eleven_util.hh"
#include "eleven_shapefile.hh"


using namespace ELEVEN;
using namespace std;

namespace {
  // handles the height tag for the Eleven curves
  void real_tag_handler( void *&data, bool mode )
  {
    if ( mode )
      data = (void *)new KK::real;
    else
      delete (KK::real*)data;
  }

  // checkH is the user defined predicate used by ELEVEN::Model::isInside
  bool
  checkH( const Vector3D &v, CompositeCurveP c )
  {
    KK::real h = -ELEVEN::tol;
    void *tdata;
    if ( c->get_tag_data("height", tdata) )
      h = *( (KK::real *)tdata);
    
    return v[2]<=h;
  }

  void generateMask(CompositeGrid &cg, realCompositeGridFunction &rmask, intCompositeGridFunction imask, ELEVEN::Model &model)
  {
    KK::StaticArray<int,2,3,1,1> index_range;
    ELEVEN::BoundingBox gbox;
    KK::Array<int> vert_classification;

    for (int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid &mg = cg[grid];
	assert(mg.isRectangular());
	realMappedGridFunction &rmgf = rmask[grid];
	intMappedGridFunction &imgf = imask[grid];

	rmgf=1.; // *wdh* 2012/03/09
	imgf=1;
	
	real dx[3],xab[2][3];
	mg.getRectangularGridParameters(dx,xab);
	Index II[3],&I1=II[0],&I2=II[1],&I3=II[2];
	getIndex(mg.indexRange(),I1,I2,I3);
	for ( int a=0; a<3; a++ )
	  {
	    index_range(0,a) = 0;
	    index_range(1,a) = II[a].getLength()-1;
	    for ( int s=0; s<2; s++ )
	      {
		gbox(s,a) = xab[s][a];
	      }
	  }
      
        //  *wdh* 2012/03/09 - changed mask to be -1 inside bodies and 1 outside -- eventually make this a signed distance.
	for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  {
	    gbox(0,2) = dx[2]*i3;
	    model.isInside(gbox,index_range,vert_classification,checkH);
	    for ( int i2=I2.getBase(),ic2=0; i2<=I2.getBound(); i2++,ic2++ )
	      for ( int i1=I1.getBase(),ic1=0; i1<=I1.getBound(); i1++,ic1++ )
		{
		  rmgf(i1,i2,i3) = vert_classification(ic1,ic2) & ELEVEN::Model::CartGridQuery::insideVertex ? -1. : 1.;
		  imgf(i1,i2,i3) = vert_classification(ic1,ic2) & ELEVEN::Model::CartGridQuery::insideVertex ? -1 : 1;
		}
	  }
      }
  }

}

// convert the number of grid points n to the nearest large value that supports numberOfMultigridLevels
int 
intmg(const int n, const int multigridLevels )
{
  int ml2=int(pow(2,multigridLevels)+.5);
  
  return int(int(n+ml2-2)/ml2)*ml2+1;
}

int main(int argc, char *argv[])
{
  ELEVEN::initialize();
  Overture::start(argc,argv);

  bool gen3D = true;
  int numberOfRefinementLevels = 1;
  int N = 100;
  real expansionFactor = 0.25;
  aString commandFileName="";
  aString showFileName="amrGrid.show", shapeFileName="myShapeFile.shp";
  int plotOption=true;

  printF("Usage: amrGridFromEleven [file.cmd] -shape=<shapeFile> -nl <num-levels> -N <num-cells> \n");

  aString line;
  int len=0;
  for ( int arg=1; arg<argc; arg++ )
  {
    line=argv[arg];
      
    if ( !strcmp(argv[arg],"-nl") )
    {
      arg++;
      numberOfRefinementLevels = atoi(argv[arg]);
    }
    else if ( !strcmp(argv[arg],"-N") )
    {
      arg++;
      N = atoi(argv[arg]);
    }
    else if ( !strcmp(argv[arg],"-f") )
    {
      arg++;
      expansionFactor = atof(argv[arg]);
    }
    else if( len=line.matches("-shape=") )
    {
      shapeFileName = line(len,line.length()-1);
    }
    else if( line(0,0)!="-" )
    {
      commandFileName=line;
    }
    
  }

  if (shapeFileName=="")
    {
      cout<<"A shapefile must be specified!"<<endl;
      exit(1);
    }

  ELEVEN::Model model;


  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("Grids from Eleven",plotOption,argc,argv);
  PlotStuffParameters psp;
  
  // By default start saving the command file called "motion.cmd"
  aString logFile="amrGridFromEleven.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file [%s]\n",(const char *)logFile);

  gi.appendToTheDefaultPrompt("amrGridFromEleven>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("Read command file =%s.\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }

  int refinementRatio=2;
  int numberOfBufferZones=2;
  
  real targetGridSpacing=.01;  // by default there are approx. 100 points across the un-expanded domain
  int multigridLevels=0;
  bool saveGridsAsBaseGrids=false;
  
  bool scaleGeometry=false;
  // geomFactor(side,axis) : geometry expansion factors 
  RealArray geomFactor(2,3);
  real &xsa = geomFactor(0,0), &xsb=geomFactor(1,0);
  real &ysa = geomFactor(0,1), &ysb=geomFactor(1,1);
  real &zsa = geomFactor(0,2), &zsb=geomFactor(1,2);
  geomFactor=expansionFactor;
  
  // xsa=0.,xsb=1.,ysa=0.,ysb=1.,zsa=0.,zsb=1.; 
  
  real xScale[3]={1.,1.,1.};  // scale factors for the geometry
  real xOffset[3]={0.,0.,0.};  // scale factors for the geometry

  GUIState dialog;
  dialog.setWindowTitle("Grids from Eleven");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"read shape file",
		    "generate grid",
                    "save show file",
		    "contour",
		    "grid",
		    ""};

  int numberOfPushButtons=5;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"scale geometry",
                          "three dimensional",
                          "save grids as base grids",
			  ""};
  int tbState[10];
  tbState[0] = scaleGeometry;
  tbState[1] = gen3D;
  tbState[2] = saveGridsAsBaseGrids;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=20;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textLabels[nt] = "shape file name:";  sPrintF(textStrings[nt],"%s",(const char*)shapeFileName);  nt++; 
  textLabels[nt] = "show file name:";  sPrintF(textStrings[nt],"%s",(const char*)showFileName);  nt++; 

  textLabels[nt] = "xScale,yScale,zScale:";  
  sPrintF(textStrings[nt],"%g,%g,%g",xScale[0],xScale[1],xScale[2]);  nt++; 

  textLabels[nt] = "xOffset,yOffset,zOffset:";  
  sPrintF(textStrings[nt],"%g,%g,%g",xOffset[0],xOffset[1],xOffset[2]);  nt++; 

  textLabels[nt] = "target grid spacing:";  
  sPrintF(textStrings[nt],"%g (scaled geometry)",targetGridSpacing);  nt++; 

  textLabels[nt] = "refinement levels:";  
  sPrintF(textStrings[nt],"%i",numberOfRefinementLevels);  nt++; 

  textLabels[nt] = "buffer zones:";  
  sPrintF(textStrings[nt],"%i",numberOfBufferZones);  nt++; 

  textLabels[nt] = "refinement ratio:";  
  sPrintF(textStrings[nt],"%i",refinementRatio);  nt++; 

  textLabels[nt] = "multigrid levels:";  
  sPrintF(textStrings[nt],"%i",multigridLevels);  nt++; 

  textLabels[nt] = "geometry expansion factors:";  
  sPrintF(textStrings[nt],"%g,%g, %g,%g, %g,%g",geomFactor(0,0),geomFactor(1,0),
	  geomFactor(0,1),geomFactor(1,1),geomFactor(0,2),geomFactor(1,2));  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // dialog.buildPopup(menu);
  gi.pushGUI(dialog);

  CompositeGrid cgs[2], &cg=cgs[0],&cgrg=cgs[1];
  realCompositeGridFunction rmask; // holds the "real" mask 
  rmask.setName("ioMask",0);

  bool shapeFileRead=false;
  bool gridGenerated=false;
  
  ELEVEN::BoundingBox bbox;
  DBase::DataBase tmp_db;
  real max_height = 0;
  aString answer;
  for(;;)
  {
    gi.getAnswer(answer,"");
    if( answer=="exit" )
      break;
  
    if( dialog.getToggleValue(answer,"scale geometry",scaleGeometry) )
    {
      if( scaleGeometry )
	printF(" scaleGeometry=true: the geometry will be scaled to the domain [xsa,xsb]x[ysa,ysb]x[zsa,zsb]\n");
      else
        printF(" scaleGeometry=false: the geometry will not be scaled\n");
    }
    else if( dialog.getToggleValue(answer,"three dimensional",gen3D) )
    {
      printF(" Setting gen3d=%i.\n",(int)gen3D);
    }
    else if( dialog.getToggleValue(answer,"save grids as base grids",saveGridsAsBaseGrids) )
    {
      printF("saveGridsAsBaseGrids=%i. (this only works for 1 refinement level).\n",(int)saveGridsAsBaseGrids);
      
    }
    else if( dialog.getTextValue(answer,"target grid spacing:","%g",targetGridSpacing) ){} // 
    else if( dialog.getTextValue(answer,"refinement levels:","%i",numberOfRefinementLevels) ){} // 
    else if( dialog.getTextValue(answer,"refinement ratio:","%i",refinementRatio) ){} // 
    else if( dialog.getTextValue(answer,"buffer zones:","%i",numberOfBufferZones) ){} // 
    else if( dialog.getTextValue(answer,"multigrid levels:","%i",multigridLevels) ){} // 
    else if( dialog.getTextValue(answer,"show file name:","%s",showFileName) ){} // 
    else if( dialog.getTextValue(answer,"shape file name:","%s",shapeFileName) ){} // 
    else if( len=answer.matches("xScale,yScale,zScale:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&xScale[0],&xScale[1],&xScale[2]);
      printF("The geometry will be scaled by xScale=%g, yScale=%g, zScale=%g\n",xScale[0],xScale[1],xScale[2]);
      dialog.setTextLabel("xScale,yScale,zScale:",sPrintF("%g, %g, %g",xScale[0],xScale[1],xScale[2]));
    }
    else if( len=answer.matches("xOffset,yOffset,zOffset:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&xOffset[0],&xOffset[1],&xOffset[2]);
      printF("The geometry will be offset by (%g,&g,%g)\n",xOffset[0],xOffset[1],xOffset[2]);
      dialog.setTextLabel("xOffset,yOffset,zOffset:",sPrintF("%g, %g, %g",xOffset[0],xOffset[1],xOffset[2]));
      
    }
    else if( len=answer.matches("geometry expansion factors:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e",&geomFactor(0,0),&geomFactor(1,0),
             &geomFactor(0,1),&geomFactor(1,1),&geomFactor(0,2),&geomFactor(1,2));
      printF("The geometry will be increased by the following factors in each direction:\n"
             "            [%g,%g]x[%g,%g]x[%g,%g]\n",geomFactor(0,0),geomFactor(1,0),
             geomFactor(0,1),geomFactor(1,1),geomFactor(0,2),geomFactor(1,2));
      dialog.setTextLabel("geometry expansion factors:",
			  sPrintF("%g,%g, %g,%g, %g,%g",geomFactor(0,0),geomFactor(1,0),
				  geomFactor(0,1),geomFactor(1,1),geomFactor(0,2),geomFactor(1,2)));
    }
	     
    else if( answer=="read shape file" )
    {
      shapeFileRead=true;
      
      KK::real t0 = KK::getcpu();
      printF("Reading shapefile [%s]\n",(const char*)shapeFileName);
      
      ELEVEN::readShapeFile( shapeFileName, tmp_db );
      INFO_LOG("time to read shapefile = "<<KK::getcpu()-t0);

      try {
	CompositeCurveList &tmp_crvs = tmp_db.get<CompositeCurveList>("curves");
              
	for ( CompositeCurveList::iterator c=tmp_crvs.begin(); 
	      c!=tmp_crvs.end(); c++ )
	{
	  //	(*c)->set(isNew);
	  model.addLoop(*c);
	  if ( tmp_db.has_key(getName(**c)+"_height") )
	  {
	    KK::real h = tmp_db.get<KK::real>(getName(**c)+"_height");
	    KK::real *data = new KK::real(h);
	    (*c)->add_tag("height", (void *)data, ::real_tag_handler);

	    max_height = max(max_height,h);
	  }
	}
      } catch ( KK::Err &e ) {
	ERROR_LOG("Error : could not read database file contents, message : "<<e.msg,true);
      }

      bbox = model.computeBounds();
      printF("Original geometry bounds: [%g,%g]x[%g,%g]x[%g,%g]\n"
             " x-scale=%g, y-scale=%g, z-scale=%g\n",
	     bbox(0,0),bbox(1,0),bbox(0,1),bbox(1,1),bbox(0,2),bbox(1,2),
             bbox(1,0)-bbox(0,0),bbox(1,1)-bbox(0,1),bbox(1,2)-bbox(0,2) );

    }
    else if( answer=="generate grid" )
    {

      if( !shapeFileRead )
      {
	printF("You must read a shape file before you can generate a grid\n");
	continue;
      }
      gridGenerated=true;
      
      //cout<<"BOUNDING BOX = "<<bbox<<endl;
      //  cout<<"MAX HEIGHT = "<<max_height<<endl;

      KK::StaticArray<KK::real,3,1,1,1> xd;
  
      xd[0] = bbox(1,0) - bbox(0,0);
      xd[1] = bbox(1,1) - bbox(0,1);
      xd[2] = max_height;
      real maxD = (1.+geomFactor(1,2))*max_height;
      for ( int a=0; a<2; a++ )
      {
	bbox(0,a) -= geomFactor(0,a)*xd[a];
	bbox(1,a) += geomFactor(1,a)*xd[a];

	// maxD = max(maxD,(1+2*f)*xd[a]);
	maxD = max(maxD,bbox(1,a)-bbox(0,a));
      }
      bbox(1,2) += (1.+geomFactor(1,2))*max_height;

      real dx = maxD/N;
      if( scaleGeometry && targetGridSpacing>0. )
      {
        dx = targetGridSpacing*maxD;
      }
      

      KK::StaticArray<int,2,3,1,1> index_range;
      for ( int a=0; a<3; a++ )
      {
	index_range(0,a) = 0;
        int n;
	if( scaleGeometry && targetGridSpacing>0. )
	{ // base number of points on targetGridSpacing: 
          n = int( (bbox(1,a) - bbox(0,a))/(xScale[a]*targetGridSpacing) + .5 );
	}
	else
	{ // old way:
          n = floor((bbox(1,a) - bbox(0,a))/dx);
	}
	  
	n = intmg(n+1,multigridLevels)-1;
	printF(" axis=%i n=%i (cells) multigridLevels=%i\n",a,n,multigridLevels);
	
	index_range(1,a) = n;
      }

      KK::Array<int> vert_classification;

      Mapping *map = 0;

      if ( !gen3D )
      {
	index_range(1,2) = 0;
	map = new SquareMapping(bbox(0,0),bbox(1,0),bbox(0,1),bbox(1,1)); map->incrementReferenceCount();
	for ( int a=0; a<2; a++ )
	{
	  map->setGridDimensions(a,index_range(1,a)+1);
	}
      }
      else
      {
	map = new BoxMapping(bbox(0,0),bbox(1,0),bbox(0,1),bbox(1,1),bbox(0,2),bbox(1,2)); map->incrementReferenceCount();
	for ( int a=0; a<3; a++ )
	{
	  map->setGridDimensions(a,index_range(1,a)+1);
	}
      }
      int nd = map->getRangeDimension();
      for ( int axis=0; axis<nd; axis++ )for( int side=0; side<=1; side++ )
      {
	int bc = 1 + side + 2*(axis);
        map->setBoundaryCondition(side,axis,bc);
        map->setShare(side,axis,bc);
      }
      

      cg.add(*map);
      for ( int a=0; a<nd; a++ )
	for ( int s=0; s<2; s++ )
	  cg[0].setNumberOfGhostPoints(s,a,2);

      cg.update( MappedGrid::THEmask );

      Interpolant &interpolator = *new Interpolant(cg);
      CompositeGridOperators op(cg); 
      intCompositeGridFunction imask(cg);
      rmask.updateToMatchGrid(cg); // just for plotting
      rmask.setOperators(op);

      // use the error estimator to only refine on the boundaries of the buildings, not inside them
      InterpolateRefinements interp(cg.numberOfDimensions());
      ErrorEstimator errorEst(interp);
      RealArray scaleFactor(2); scaleFactor = 1.0;
      Ogen ogen;
      errorEst.setScaleFactor(scaleFactor);
      realCompositeGridFunction error(cg);

      // ok, the "error" should look like O(s1/dx + s2/(dx^2)) since the rmask is either 1 or zero everywhere
      // refine everywhere things are bigger than 1e-2, say, to catch smoothed points
      real threshhold = 1e-2;
      Regrid regrid;
      regrid.setNumberOfBufferZones(numberOfBufferZones);
      regrid.setRefinementRatio(refinementRatio);
      if( saveGridsAsBaseGrids && numberOfRefinementLevels==1 )
        regrid.setGridAdditionOption( Regrid::addGridsAsBaseGrids );  // this works with one level

      real t1 = KK::getcpu();
      for ( int level=1; level<=numberOfRefinementLevels; level++ )
      {
	INFO_LOG("generating refinement level "<<level);
	CompositeGrid &cg = cgs[(level+1)%2];
	CompositeGrid &cgrg = cgs[level%2];

	interpolator.updateToMatchGrid(cg,level-1);
	op.updateToMatchGrid(cg);
	rmask.updateToMatchGrid(cg);
	imask.updateToMatchGrid(cg);
	error.updateToMatchGrid(cg);
	error = 0.0;
      
	rmask.setOperators(op);
	error.setOperators(op);

	// generate the mask information for the level-1 grid
	generateMask(cg,rmask,imask,model); // we should really only need this for the current level?
	interp.interpolateRefinementBoundaries(rmask);
	errorEst.computeAndSmoothErrorFunction(rmask,error,1); // only do 1 smooth

	// if( level==numberOfRefinementLevels )
	//  regrid.setGridAdditionOption( Regrid::addGridsAsBaseGrids );

	regrid.regrid(cg,cgrg,error,threshhold,level);
	ogen.updateRefinement(cgrg);
	cgrg.update(MappedGrid::THEmask);

      }
      INFO_LOG("time to generate grid = "<<KK::getcpu()-t1<<"s");

      CompositeGrid & cgr = cgs[numberOfRefinementLevels%2];

      // -- adjust the index ranges back to base 0 (for Ogmg)
      bool fixUpIndexRange=false; // true;
      if( fixUpIndexRange )
      {
	for( int grid=0; grid<cgr.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = cgr[grid];
	  Mapping & map = mg.mapping().getMapping();

	  const IntegerArray & gir = mg.gridIndexRange();
	  const IntegerArray & ir = mg.indexRange();
	  const IntegerArray & dim = mg.dimension();
	
	  printF("grid=%i: gid=[%i,%i][%i,%i][%i,%i]\n",grid,gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2));

	  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    int na=gir(0,axis), nb=gir(1,axis);
	    
	    mg.setGridIndexRange(0,axis,0);
	    mg.setGridIndexRange(1,axis,nb-na);
	  }
	  
	}
	cgr.update(MappedGrid::THEmask); 
      }


      // now re-evaluate the inside/outside information on the final grid for plotting
      real t2 = KK::getcpu();

      rmask.updateToMatchGrid(cgr);
      imask.updateToMatchGrid(cgr);
      interpolator.updateToMatchGrid(cgr,numberOfRefinementLevels);
      op.updateToMatchGrid(cgr);
      generateMask(cgr,rmask,imask,model); // we should really only need this for the last level?
      interp.interpolateRefinementBoundaries(rmask);

      INFO_LOG("time to re-evaluate inside/outside = "<<KK::getcpu()-t2);

      if( map->decrementReferenceCount()==0 )
        delete map;

      // delete &interpolator;
      
      

      if( scaleGeometry )
      {
	CompositeGrid & cg = cgr;

        printF("Original geometry: [%g,%g]x[%g,%g]x[%g,%g]\n",
               bbox(0,0),bbox(1,0),bbox(0,1),bbox(1,1),bbox(0,2),bbox(1,2));
	
// 	real scale[3]={1.,1.,1.};
// 	for( int axis=0; axis<nd; axis++ )
// 	{
// 	  scale[axis]=(xsab(1,axis)-xsab(0,axis))/(bbox(1,axis)-bbox(0,axis));
// 	}
	RealArray xab(2,3), xabNew(2,3);
        // -- We only need to scale the mapping for grid=0 since all other grids use this ---
	int grid=0;
	MappedGrid & mg = cg[grid];
	Mapping & map = mg.mapping().getMapping();
	if( nd==2 )
	{
          assert( map.getClassName()=="SquareMapping");
	  
	  SquareMapping & square = (SquareMapping&)map;
	  square.getVertices( xab(0,0),xab(1,0),xab(0,1),xab(1,1) );
	  printF("square: xab=[%g,%g]x[%g,%g] xScale=%e,%e\n",xab(0,0),xab(1,0),xab(0,1),xab(1,1));
	  for( int axis=0; axis<nd; axis++ )
	  {
	    xabNew(0,axis) = xOffset[axis] + (xab(0,axis)-bbox(0,axis))/xScale[axis];
	    xabNew(1,axis) = xOffset[axis] + (xab(1,axis)-bbox(0,axis))/xScale[axis];
	    // printF(" grid=%i axis=%i xab(0,axis)=%e bbox(0,axis)=%e xScale[axis]=%e xabNew(0,axis)=%e xabNew(1,axis)=%e\n",
	    //	   grid,axis,xab(0,axis),bbox(0,axis),xScale[axis],xabNew(0,axis),xabNew(1,axis));
	  }
	  //printF("square: [%g,%g]x[%g,%g] xScale=%e,%e\n",xabNew(0,0),xabNew(1,0),xabNew(0,1),xabNew(1,1),
          //        xScale[0],xScale[1]);
	    
	  square.setVertices( xabNew(0,0),xabNew(1,0),xabNew(0,1),xabNew(1,1) );
	}
	else
	{
          assert( map.getClassName()=="BoxMapping");

	  BoxMapping & box = (BoxMapping&)map;
	  box.getVertices( xab(0,0),xab(1,0),xab(0,1),xab(1,1),xab(0,2),xab(1,2) );
	  for( int axis=0; axis<nd; axis++ )
	  {
	    xabNew(0,axis) = xOffset[axis] + (xab(0,axis)-bbox(0,axis))/xScale[axis];
	    xabNew(1,axis) = xOffset[axis] + (xab(1,axis)-bbox(0,axis))/xScale[axis];
	    //printF(" grid=%i axis=%i xab(0,axis)=%e bbox(0,axis)=%e xScale[axis]=%e xabNew(0,axis)=%e\n",
	    //	   grid,axis,xab(0,axis),bbox(0,axis),xScale[axis],xabNew(0,axis));
	      
	  }
	  //printF("grid=%i: xabNew=[%g,%g]x[%g,%g]x[%g,%g]\n",grid,
	  //	 xabNew(0,0),xabNew(1,0),xabNew(0,1),xabNew(1,1),xabNew(0,2),xabNew(1,2));
	  box.setVertices( xabNew(0,0),xabNew(1,0),xabNew(0,1),xabNew(1,1),xabNew(0,2),xabNew(1,2) );
	}

// 	  printF("grid=%i : scaling the geometry to [%g,%g]x[%g,%g]x[%g,%g]\n",grid,
// 		 xabNew(0,0),xabNew(1,0),xabNew(0,1),xabNew(1,1),xabNew(0,2),xabNew(1,2));


	cg.geometryHasChanged(~MappedGrid::THEmask); // invalidate any geometry
      }
      
      int totalNumberOfGridPoints=-1;
      GridStatistics::getNumberOfPoints(cgr,totalNumberOfGridPoints);
      printF("Summary: Number of grids=%i, total number of grid points=%i.\n",cgr.numberOfComponentGrids(),
              totalNumberOfGridPoints);
      
    }
    else if( answer=="save show file" )
    {
      if( !gridGenerated )
      {
	printF("WARNING: You must generate a grid before you can save a show file.\n");
	continue;
      }

      int useStreamMode=true;  // save in compressed mode
      Ogshow ogshow(showFileName,".",useStreamMode);
      ogshow.startFrame();
      ogshow.saveSolution(rmask);
      ogshow.close();
    }
    else if( answer=="contour" )
    {
      if( !gridGenerated )
      {
	printF("WARNING: You must generate a grid before you can plot contours.\n");
	continue;
      }

      gi.erase();
      PlotIt::contour(gi,rmask,psp);
    }
    else if( answer=="grid" )
    {
      if( !gridGenerated )
      {
	printF("WARNING: You must generate a grid before you can plot it.\n");
	continue;
      }

      CompositeGrid & cg = *rmask.getCompositeGrid();
      
      gi.erase();
      PlotIt::plot(gi,cg,psp);
    }
    else
    {
      printf("Unknown answer=[%s]\n",(const char*)answer);
      continue;
    }
  }
  


//
//  KK::real t0 = KK::getcpu();
//  DBase::DataBase tmp_db;
//  ELEVEN::readShapeFile( shapefile_name, tmp_db );
//  INFO_LOG("time to read shapefile = "<<KK::getcpu()-t0);
//
//  real max_height = 0;
//  try {
//    CompositeCurveList &tmp_crvs = tmp_db.get<CompositeCurveList>("curves");
//              
//    for ( CompositeCurveList::iterator c=tmp_crvs.begin(); 
//	  c!=tmp_crvs.end(); c++ )
//      {
//	//	(*c)->set(isNew);
//	model.addLoop(*c);
//	if ( tmp_db.has_key(getName(**c)+"_height") )
//	  {
//	    KK::real h = tmp_db.get<KK::real>(getName(**c)+"_height");
//	    KK::real *data = new KK::real(h);
//	    (*c)->add_tag("height", (void *)data, ::real_tag_handler);
//
//	    max_height = max(max_height,h);
//	  }
//      }
//  } catch ( KK::Err &e ) {
//    ERROR_LOG("Error : could not read database file contents, message : "<<e.msg,true);
//  }

//   ELEVEN::BoundingBox bbox = model.computeBounds();

  
//   //cout<<"BOUNDING BOX = "<<bbox<<endl;
//   //  cout<<"MAX HEIGHT = "<<max_height<<endl;

//   KK::StaticArray<KK::real,3,1,1,1> xd;
  
//   xd[0] = bbox(1,0) - bbox(0,0);
//   xd[1] = bbox(1,1) - bbox(0,1);
//   xd[2] = max_height;
//   real maxD = (1+f)*max_height;
//   for ( int a=0; a<2; a++ )
//     {
//       bbox(0,a) -= f*xd[a];
//       bbox(1,a) += f*xd[a];

//       maxD = max(maxD,(1+2*f)*xd[a]);
//     }
//   bbox(1,2) += (1+f)*max_height;

//   real dx = maxD/N;

//   KK::StaticArray<int,2,3,1,1> index_range;
//   for ( int a=0; a<3; a++ )
//     {
//       index_range(0,a) = 0;
//       index_range(1,a) = floor((bbox(1,a) - bbox(0,a))/dx);
//     }

//   KK::Array<int> vert_classification;

//   CompositeGrid cgs[2], &cg=cgs[0],&cgrg=cgs[1];
//   Mapping *map = 0;

//   if ( !gen3D )
//     {
//       index_range(1,2) = 0;
//       map = new SquareMapping(bbox(0,0),bbox(0,1),bbox(0,1),bbox(1,1));
//       for ( int a=0; a<2; a++ )
// 	{
// 	  map->setGridDimensions(a,index_range(1,a)+1);
// 	}
//     }
//   else
//     {
//       map = new BoxMapping(bbox(0,0),bbox(1,0),bbox(0,1),bbox(1,1),bbox(0,2),bbox(1,2));
//       for ( int a=0; a<3; a++ )
// 	{
// 	  map->setGridDimensions(a,index_range(1,a)+1);
// 	}
//     }

//   int nd = map->getRangeDimension();

//   cg.add(*map);
//   for ( int a=0; a<nd; a++ )
//     for ( int s=0; s<2; s++ )
//       cg[0].setNumberOfGhostPoints(s,a,2);

//   cg.update();
//   Interpolant &interpolator = *new Interpolant(cg);
//   CompositeGridOperators op(cg); 
//   intCompositeGridFunction imask(cg);
//   realCompositeGridFunction rmask(cg);
//   rmask.setName("ioMask",0);
//   rmask.setOperators(op);

//   // use the error estimator to only refine on the boundaries of the buildings, not inside them
//   InterpolateRefinements interp(cg.numberOfDimensions());
//   ErrorEstimator errorEst(interp);
//   RealArray scaleFactor(2); scaleFactor = 1.0;
//   Ogen ogen;
//   errorEst.setScaleFactor(scaleFactor);
//   realCompositeGridFunction error(cg);

//   // ok, the "error" should look like O(s1/dx + s2/(dx^2)) since the rmask is either 1 or zero everywhere
//   // refine everywhere things are bigger than 1e-2, say, to catch smoothed points
//   real threshhold = 1e-2;
//   Regrid regrid;
//   regrid.setNumberOfBufferZones(2);
  
//   real t1 = KK::getcpu();
//   for ( int level=1; level<=nlevels; level++ )
//     {
//       INFO_LOG("generating refinement level "<<level);
//       CompositeGrid &cg = cgs[(level+1)%2];
//       CompositeGrid &cgrg = cgs[level%2];

//       interpolator.updateToMatchGrid(cg,level-1);
//       op.updateToMatchGrid(cg);
//       rmask.updateToMatchGrid(cg);
//       imask.updateToMatchGrid(cg);
//       error.updateToMatchGrid(cg);
//       error = 0.0;
      
//       rmask.setOperators(op);
//       error.setOperators(op);

//       // generate the mask information for the level-1 grid
//       generateMask(cg,rmask,imask,model); // we should really only need this for the current level?
//       interp.interpolateRefinementBoundaries(rmask);
//       errorEst.computeAndSmoothErrorFunction(rmask,error,1); // only do 1 smooth

//       regrid.regrid(cg,cgrg,error,threshhold,level);
//       ogen.updateRefinement(cgrg);
//       cgrg.update();

//     }
//   INFO_LOG("time to generate grid = "<<KK::getcpu()-t1<<"s");

//   // now re-evaluate teh inside/outside information on the final grid for plotting
//   real t2 = KK::getcpu();

//   rmask.updateToMatchGrid(cgs[nlevels%2]);
//   imask.updateToMatchGrid(cgs[nlevels%2]);
//   interpolator.updateToMatchGrid(cgs[nlevels%2],nlevels);
//   op.updateToMatchGrid(cgs[nlevels%2]);
//   generateMask(cgs[nlevels%2],rmask,imask,model); // we should really only need this for the last level?
//   interp.interpolateRefinementBoundaries(rmask);

//   INFO_LOG("time to re-evaluate inside/outside = "<<KK::getcpu()-t2);

// #if 0
//   for ( int grid=0; grid<cgs[nlevels%2].numberOfComponentGrids(); grid++ )
//     {
//       realMappedGridFunction &rmg = rmask[grid];
//       rmg = rmg.r1r1()+rmg.r2r2() + rmg.r3r3() - 8*rmg;
//     }
// #endif

//   PlotIt::contour(gi,rmask);
//   //PlotIt::plot(*Overture::getGraphicsInterface(),cg);

//   Ogshow ogshow("amrGrid.show");
//   ogshow.startFrame();
//   ogshow.saveSolution(rmask);
//   ogshow.close();
//   delete map;

  gi.unAppendTheDefaultPrompt();  // reset prompt
  gi.popGUI(); // restore the previous GUI

  Overture::finish();
  ELEVEN::finalize();
}

