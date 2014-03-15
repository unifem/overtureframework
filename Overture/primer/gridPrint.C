// =================================================================================
//    Read a grid from a data base and output the data to a plain text file. 
//   
//  This routine can be changed to create an output file with any desired format.
//
// Example:
//     gridPrint cic.hdf cic.out
//     mpirun -np 2 gridPrint cic.hdf cic.out
// ==================================================================================

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
    int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
    I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int np= max(1,Communication_Manager::numberOfProcessors());  // number of processors
  const int myid=max(0,Communication_Manager::My_Process_Number);    // my rank 

  printF(" Usage: gridPrint gridName.hdf outfile \n");

  aString nameOfOGFile, fileName;
  if( argc==3 )
  {
    nameOfOGFile=argv[1];
    fileName=argv[2];
  }
  else
  {
    printF("Usage: gridPrint gridName.hdf outfile \n");
    Overture::abort("error");
  }
  

  FILE *file=NULL;
  if( np==1 )
    file= fopen((const char*)fileName,"w" );
  else 
  {
    // In parallel write info to separate files on each processor
    aString buff;
    file = fopen(sPrintF(buff,"%s.p%i",(const char*)fileName,myid),"w" ); 
  }
  
  

  // Read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  const int numberOfDimensions=cg.numberOfDimensions();
  
  fprintf(file,"%i %i  (number of grids, number of dimensions)\n",cg.numberOfComponentGrids(),numberOfDimensions);

  const IntegerArray & ni = cg.numberOfInterpolationPoints;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    c.update(MappedGrid::THEvertex | MappedGrid::THEmask );  // create the vertex and mask arrays

    const IntegerArray & d  = c.dimension();
    const IntegerArray & gir= c.gridIndexRange();
    // const IntegerArray & ir = c.indexRange();
    // const IntegerArray & eir = c.extendedIndexRange();
    // const IntegerArray & egir = extendedGridIndexRange(c); // *note*
    // const IntegerArray & er = c.extendedRange();
    const IntegerArray & bc  = c.boundaryCondition();

    fprintf(file,"%i %s (grid and name)\n"
	    "%i %i %i %i %i %i (dimension(0:1,0:2), array dimensions)\n"
	    "%i %i %i %i %i %i (gridIndexRange(0:1,0:2), grid bounds)\n"
	    "%i %i %i %i %i %i (boundaryCondition(0:1,0:2))\n"
	    "%i %i %i          (isPeriodic(0:2), 0=not, 1=deriv, 2=function)\n",
	    grid,(const char*)c.getName(),
	    d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
	    gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2),
	    bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
	    c.isPeriodic(0),c.isPeriodic(1),c.isPeriodic(2));
    
    fprintf(file,"%i (total number of interpolation points)\n",ni(grid));
    if( ni(grid)>0 )
    {
      intSerialArray ig,viw,ip,il;
      realSerialArray ci;
      if( cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
      {
        printF("Found interpolationPoint\n");
	
        // If the grid was written in serial, the interpolation arrays are saved here:
	ip.reference( cg.interpolationPoint[grid].getLocalArray());
	il.reference( cg.interpoleeLocation[grid].getLocalArray());
	ig.reference( cg.interpoleeGrid[grid].getLocalArray());
	viw.reference( cg.variableInterpolationWidth[grid].getLocalArray());
	ci.reference(cg.interpolationCoordinates[grid].getLocalArray());
      }
      else
      {
        // If the grid was written in parallel, the interpolation arrays are saved as separate serial arrays:
        printF("Found interpolationPointLocal\n");

	ip.reference( cg->interpolationPointLocal[grid]);
	il.reference( cg->interpoleeLocationLocal[grid]);
	ig.reference( cg->interpoleeGridLocal[grid]);
	viw.reference( cg->variableInterpolationWidthLocal[grid]);
	ci.reference(cg->interpolationCoordinatesLocal[grid]);
      }
      
      if( false )
      { // show how to copy the interpolation point data to one processor
        Index Iv[2];
	Iv[0]=ip.dimension(0);
	Iv[1]=ip.dimension(1);
	
        int p0=0;   // create an aggregate array on proc. 0 holding all ip values
        IntegerArray ip0;
	CopyArray::getAggregateArray( ip, Iv, ip0,p0);
	fprintf(file,"%i (number of interpolation points in the aggregate array)\n",ip0.getLength(0));
	for( int i=ip0.getBase(0); i<=ip0.getBound(0); i++ )
	{
	  fprintf(file,"%i %i %i  (ip)\n",
		  ip0(i,0),ip0(i,1),(numberOfDimensions==2 ? 0 : ip0(i,2)));
	}
      }

      // ig : donor grid 
      // viw : interpolation width 
      // ip : interpolation point (on grid)
      // il : lower left corner of donor stencil (on the donor grid)
      // ci : unit square coordinates of the interpolation point in the donor grid

      int niLocal = ip.getLength(0);  // number of interpolation points on this processor
      fprintf(file,"%i (number of interpolation points on this processor)\n",niLocal);
      for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
      {
	fprintf(file,"%i %i  %i %i %i  %i %i %i  %e %e %e (donor, width, ip, il, ci)\n",
		ig(i),viw(i),
		ip(i,0),ip(i,1),(numberOfDimensions==2 ? 0 : ip(i,2)),
		il(i,0),il(i,1),(numberOfDimensions==2 ? 0 : il(i,2)),
		ci(i,0),ci(i,1),(numberOfDimensions==2 ? 0 : ci(i,2)));
      }
    }

    Index I1,I2,I3;
    getIndex(c.dimension(),I1,I2,I3);
    int i1,i2,i3;

    const intArray & mask = c.mask();
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);          // local array on this processor
    int includeGhost=0;
    bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);  // get bounds of the local array (no ghost)

    if( false )
    {
      // print bounds on the mask array and local mask array: 
      printf(" grid=%i, myid=%i : mask=[%i,%i][%i,%i][%i,%i]  maskLocal=[%i,%i][%i,%i][%i,%i]\n",
	     grid,myid,
	     mask.getBase(0),mask.getBound(0),
	     mask.getBase(1),mask.getBound(1),
	     mask.getBase(2),mask.getBound(2),
	     maskLocal.getBase(0),maskLocal.getBound(0),
	     maskLocal.getBase(1),maskLocal.getBound(1),
	     maskLocal.getBase(2),maskLocal.getBound(2));
    }
    

    if( ok )  // if there are points on this processor
    {
      fprintf(file,"mask (based on the dimension array, -1=interp, 0=not used, 1=used)\n");
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	int m=maskLocal(i1,i2,i3);
	if( m<0 ) 
	  m=-1;   // interpolation point 
	else if( m>0 ) 
	  m=1;    // discretization point 
	fprintf(file,"%i ",m);
      }
      fprintf(file,"\n");
    
      const realArray & vertex = c.vertex();
      realSerialArray vertexLocal; getLocalArrayWithGhostBoundaries(vertex,vertexLocal);          // local array on this processor

      fprintf(file,"vertex (based on the dimension array)\n");
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	if( numberOfDimensions==2 )
	  fprintf(file,"%e ",vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1));
	else
	  fprintf(file,"%e ",vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1),vertexLocal(i1,i2,i3,2));
      }
      fprintf(file,"\n");
    }
    
    c.destroy(MappedGrid::THEvertex | MappedGrid::THEmask );  // destroy arrays to save space

  }
  fclose(file);
  if( np==1 )
    printF("Output written to file %s\n",(const char*)fileName);
  else
    printF("Output written to files:  %s.p<processor-number>\n",(const char*)fileName);
  
  Overture::finish();          
  return 0;
}
