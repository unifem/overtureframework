// #define BOUNDS_CHECK

// --------------------------------------------------------------------------------------------------
// ---- Here are DataFormats functions that use higher level Overture types such as MappedGrid. -----
// --------------------------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <iomanip>

#include "DataFormats.h"

#include "conversion.h"

#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "UnstructuredMapping.h"
#include "OGgetIndex.h"
#include <string.h>
#include "plyFileInterface.h"
#include "MappedGrid.h"
#include "CompositeGrid.h"

#define OPPLT3D    EXTERN_C_NAME(opplt3d)
#define RDPLT3D    EXTERN_C_NAME(rdplt3d)
#define RDPLT3DS   EXTERN_C_NAME(rdplt3ds)
#define RDPLT3DD   EXTERN_C_NAME(rdplt3dd)
#define RDPLT3DQS  EXTERN_C_NAME(rdplt3dqs)
#define RDPLT3DQD  EXTERN_C_NAME(rdplt3dqd)
#define OPINGRID   EXTERN_C_NAME(opingrid)
#define RDINGRID   EXTERN_C_NAME(rdingrid)
#define CLINGRID   EXTERN_C_NAME(clingrid)
#define WRPLT3DS   EXTERN_C_NAME(wrplt3ds)
#define WRPLT3DD   EXTERN_C_NAME(wrplt3dd)
#define WRPLT3DQS  EXTERN_C_NAME(wrplt3dqs)
#define WRPLT3DQD  EXTERN_C_NAME(wrplt3dqd)
#define WRINGRID   EXTERN_C_NAME(wringrid)
#define CLOSEPLT3D EXTERN_C_NAME(closeplt3d)
#define WPEG5D     EXTERN_C_NAME(wpeg5d)
#define WPEG5F     EXTERN_C_NAME(wpeg5f)

using namespace std;

extern "C" {
void WRPLT3DS(char filename[], const int & fileFormat, const int & iunit,const int & ng,const int & grid,
	      const int & nx, const int & ny, const int & nz,
	      const int & nd, const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb,
	      const int & ndta, const int & ndtb, const float & xy, 
	      const int & writeIblank, const int & iblank, int & ierr, const int len_filename );

void WRPLT3DD(char filename[], const int & fileFormat, const int & iunit,const int & ng,const int & grid,
	      const int & nx, const int & ny, const int & nz,
	      const int & nd, const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb,
	      const int & ndta, const int & ndtb, const double & xy, 
	      const int & writeIblank, const int & iblank, int & ierr, const int len_filename );

// write a plot3d grid and q file (single precision):
void WRPLT3DQS( char gridFileName[], char qFileName[], const int & fileFormat,
		const int & iunit,const int & junit, const int & ng,const int & grid,
		const int & nx,const int & ny,const int & nz,
		const int & nd,const int & ndra,const int & ndrb,const int & ndsa,const int & ndsb,const int & ndta,
		const int & ndtb,const float &xy, const int & nq, const int & nqc, const float &q, 
		const int & writeIblank, const int &iblank, const double &par, const int & ierr, 
                const int len_gridFileName, const int len_qFileName );

// write a plot3d grid and q file (double precision):
void WRPLT3DQD( char gridFileName[], char qFileName[], const int & fileFormat,
		const int & iunit,const int & junit, const int & ng,const int & grid,
		const int & nx,const int & ny,const int & nz,
		const int & nd,const int & ndra,const int & ndrb,const int & ndsa,const int & ndsb,const int & ndta,
		const int & ndtb,const double &xy, const int & nq, const int & nqc, const double &q, 
		const int & writeIblank, const int &iblank, const double &par, const int & ierr, 
                const int len_gridFileName, const int len_qFileName );


// wpeg5 can be found in otherStuff/prtpeg.f
void WPEG5D( const int &iunit, const int &grid, const int &ngrids,
	     const int &mjmax, const int &mkmax, const int &mlmax,
	     const int &ji, const int &ki, const int &li, 
	     const double &dxint, const double &dyint, const double &dzint,
	     const int &ib, const int &jb, const int &kb, 
	     const int &ibc,
	     const int &ibpnts, const int &iipnts,
	     const int &iieptr, const int &iisptr, const int &iblank,
	     const int &usePeg4 );

void WPEG5F( const int &iunit, const int &grid, const int &ngrids,
	     const int &mjmax, const int &mkmax, const int &mlmax,
	     const int &ji, const int &ki, const int &li, 
	     const float &dxint, const float &dyint, const float &dzint,
	     const int &ib, const int &jb, const int &kb, 
	     const int &ibc,
	     const int &ibpnts, const int &iipnts,
	     const int &iieptr, const int &iisptr, const int &iblank,
	     const int &usePeg4 );
}

namespace {

  void writeOverflowBC( ostream &o, const IntegerArray &bc, const aString &name )
  {
    int ddim = bc.getLength(1);

    o<<"$GRDNAM \nNAME = "<<"'"<<name<<"',\n $END"<<endl;
    int nbc = 0;
    for ( int a=0; a<ddim; a++ )
      for ( int s=0; s<2; s++ )
	if ( bc(s,a)!=0 )
	  nbc++;

    o<<setw(5);
    o<<"$BCINP\n";
    o<<"NBC = "<<nbc<<",\n"; 
    o<<"IBTYP = ";
    for ( int a=0; a<ddim; a++ )
      for ( int s=0; s<2; s++ )
	{ 
	  if ( bc(s,a)!=0 )
	    o<<(bc(s,a)<0 ? 10 : bc(s,a))<<", "; // 10 is periodic in the Overflow input
	}
    o<<endl;

    o<<"IBDIR = ";
    for ( int a=0; a<ddim; a++ )
      for ( int s=0; s<2; s++ )
	{
	  if ( bc(s,a)!=0 )
	    {
	      int dir = s==0 ? (a+1) : -(a+1);
	      o<<dir<<", ";
	    }
	}
    o<<endl;

    aString bdirs[] = {"JB","KB","LB"};
    aString bcse[] = {"CS","CE"};
    for ( int a=0; a<ddim; a++ )
      for ( int s=0; s<2; s++ )
	{
	  o<<bdirs[a]+bcse[s]<<" = ";
	  for ( int a2=0; a2<ddim; a2++ )
	    {
	      for ( int s2=0; s2<2; s2++ )
		{
		  if ( bc(s2,a2)!=0 )
		    {
		      if ( a2==a )
			o<< ( s2==0 ? 1 : -1 )<<", ";
		      else
			o<< (s==0 ? 1 : -1)<<", ";
		    }
		}
	    }
	  o<<endl;
	}

    o<<"$END"<<endl;
  }

}

int DataFormats::
writePlot3d(MappedGrid & mg,
	    const aString & gridFileName /* =nullString */ )
// ==============================================================================
/// \details 
// =====================================================================================
{
  int dims[3];
  dims[2] = 1;
  Index I[3],&I1=I[0],&I2=I[1],&I3=I[2];
  getIndex(mg.gridIndexRange(),I1,I2,I3);

  realArray &xyz = mg.vertex();
  for ( int i=0; i<mg.numberOfDimensions(); i++ )
    dims[i] = I[i].getBound()-I[i].getBase()+1;

  int writeIblank = 0;

  // aString filenm =  gridFileName==nullString ? "fort.1"      filenm=gridFileName;
  aString filenm;
  if( gridFileName==nullString )
    filenm="fort.1";
  else
    filenm=gridFileName;
    
  int ierr;

  int format = 1;//unformatted
  int ngrids=1;
  int iunit = 1;
  int rDim = mg.numberOfDimensions();

#ifdef OV_USE_DOUBLE
  WRPLT3DD( const_cast<char *>(filenm.c_str()), format, iunit,
	    ngrids, ngrids, dims[0],dims[1],dims[2],rDim,
	    1-mg.numberOfGhostPoints(0,0),xyz.getRawDataSize(0)-mg.numberOfGhostPoints(1,0),
	    1-mg.numberOfGhostPoints(0,1),xyz.getRawDataSize(1)-mg.numberOfGhostPoints(1,1),
	    1-mg.numberOfGhostPoints(0,2),xyz.getRawDataSize(2)-mg.numberOfGhostPoints(1,2),
	    *xyz.getDataPointer(),writeIblank,0,ierr,filenm.length() );
#else
  WRPLT3DS( const_cast<char *>(filenm.c_str()), format, iunit,
	    ngrids, ngrids, dims[0],dims[1],dims[2],rDim,
	    1-mg.numberOfGhostPoints(0,0),xyz.getRawDataSize(0)-mg.numberOfGhostPoints(1,0),
	    1-mg.numberOfGhostPoints(0,1),xyz.getRawDataSize(1)-mg.numberOfGhostPoints(1,1),
	    1-mg.numberOfGhostPoints(0,2),xyz.getRawDataSize(2)-mg.numberOfGhostPoints(1,2),
	    *xyz.getDataPointer(),writeIblank,0,ierr,filenm.length() );
#endif

  ofstream ovbc;
  ovbc.open((filenm+".ovbc").c_str());
  
  writeOverflowBC( ovbc, mg.boundaryCondition(),mg.mapping().getMapping().getName(Mapping::mappingName) );

  return 0;
}

int DataFormats::
writePlot3d(CompositeGrid & cg,
	    const aString & gridFileName /* =nullString */,
	    const aString & interpolationDataFileName /* =nullString */ )
// ==============================================================================
/// \details 
// =====================================================================================
{
  int ngrids=cg.numberOfGrids();
  intSerialArray dims(ngrids,3);
  intSerialArray ibnds(ngrids,2,3);
  dims = 1;
  cg.update(MappedGrid::THEvertex);
  for ( int grid=0; grid<ngrids; grid++ )
    {
      MappedGrid &mg = cg[grid];

      mg.update(MappedGrid::THEmask | MappedGrid::THEvertex );  // *wdh* 080404

      //      Index I[3],&I1=I[0],&I2=I[1],&I3=I[2];
      //      getIndex(mg.gridIndexRange(),I1,I2,I3);
      IntegerArray extGridInd = extendedGridIndexRange(mg);
      const realArray &xyz = mg.vertex();
      mg.dimension().display();
      for ( int i=0; i<3; i++ ) // *wdh* 080404 -- fill in all 3 dims
	{
	  dims(grid,i) = extGridInd(1,i) - extGridInd(0,i) + 1;
	  cout<<grid<<"  "<<i<<"  "<<mg.numberOfGhostPoints(0,i)<<"  "<<xyz.getRawDataSize(i)<<endl;
	  if ( mg.boundaryCondition(0,i)==0 )
	    ibnds(grid,0,i) = 2 - mg.numberOfGhostPoints(0,i) + 1;  // need 2 ghost lines and the first should be at 1
	  else
	    ibnds(grid,0,i) = 1-mg.numberOfGhostPoints(0,i);
	  ibnds(grid,1,i) = ibnds(grid,0,i) + xyz.getRawDataSize(i)-1;
	}
      //      dims(grid,i) = I[i].getBound()-I[i].getBase()+1;
    }

  
  ibnds.display("ibnds");
  dims.display("dims");
  int format = 1;//0-formatted, 1-unformatted
  int writeIblank = 1;
  int usePeg4 = 1;


  // aString filenm =  gridFileName==nullString ? "fort.1" : gridFileName;
  aString filenm;
  if( gridFileName==nullString )
    filenm="fort.1";
  else
    filenm=gridFileName;
  ofstream ovbc;
  ovbc.open((filenm+".ovbc").c_str());
  
    
  int ierr;
  int iunit = 1;
  int rDim = cg.numberOfDimensions();
  for ( int grid=1; grid<=ngrids; grid++ )
    {
      MappedGrid &mg = cg[grid-1];
      mg.update(MappedGrid::THEvertex | MappedGrid::THEmask);
      const realArray &xyz = mg.vertex();
      IntegerArray extGridInd = extendedGridIndexRange(mg);

      Index I[3],&I1=I[0],&I2=I[1],&I3=I[2];
      //	  getIndex(g.gridIndexRange(),I1,I2,I3);
      getIndex(mg.dimension(),I1,I2,I3);

      intArray iblank(I1,I2,I3);
      iblank(I1,I2,I3) = mg.mask()(I1,I2,I3);
      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    if ( iblank(i1,i2,i3)>0 ) 
	      iblank(i1,i2,i3) = 1;
	    else
	      iblank(i1,i2,i3) = 0;
      
	  


#ifdef OV_USE_DOUBLE
      WRPLT3DD( const_cast<char *>(filenm.c_str()), format, iunit,
		ngrids, grid, dims(0,0),dims(0,1),dims(0,2),rDim,
		ibnds(grid-1,0,0),ibnds(grid-1,1,0),
		ibnds(grid-1,0,1),ibnds(grid-1,1,1),
		ibnds(grid-1,0,2),ibnds(grid-1,1,2),
		//		1-mg.numberOfGhostPoints(0,0),xyz.getRawDataSize(0)-mg.numberOfGhostPoints(1,0),
		//		1-mg.numberOfGhostPoints(0,1),xyz.getRawDataSize(1)-mg.numberOfGhostPoints(1,1),
		//		1-mg.numberOfGhostPoints(0,2),xyz.getRawDataSize(2)-mg.numberOfGhostPoints(1,2),
		*xyz.getDataPointer(),writeIblank,*iblank.getDataPointer(),ierr,filenm.length());
#else
      WRPLT3DS( const_cast<char *>(filenm.c_str()), format, iunit,
		ngrids, grid, dims(0,0),dims(0,1),dims(0,2),rDim,
		ibnds(grid,0,0),ibnds(grid,1,0),
		ibnds(grid,0,1),ibnds(grid,1,1),
		ibnds(grid,0,2),ibnds(grid,1,2),
		//		1-mg.numberOfGhostPoints(0,0),xyz.getRawDataSize(0)-mg.numberOfGhostPoints(1,0),
		//		1-mg.numberOfGhostPoints(0,1),xyz.getRawDataSize(1)-mg.numberOfGhostPoints(1,1),
		//		1-mg.numberOfGhostPoints(0,2),xyz.getRawDataSize(2)-mg.numberOfGhostPoints(1,2),
		*xyz.getDataPointer(),writeIblank,*iblank.getDataPointer(),ierr,filenm.length());
#endif

      writeOverflowBC( ovbc, mg.boundaryCondition(),mg.mapping().getMapping().getName(Mapping::mappingName) );
    }

  if ( interpolationDataFileName!=nullString && sum(cg.numberOfInterpolationPoints) )
    {
      intSerialArray nDonor(cg.numberOfComponentGrids()); 
      Range R(cg.numberOfComponentGrids());
      nDonor(R) = 0;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = cg[grid];
	  
	  intArray & ig = cg.interpoleeGrid[grid];     

	  for ( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	    nDonor( ig(i) )++;
	}

      intSerialArray  *ibc = new intSerialArray[ cg.numberOfComponentGrids() ];
      realSerialArray *dr  = new realSerialArray[ cg.numberOfComponentGrids() ];
      intSerialArray  *ijk = new intSerialArray[ cg.numberOfComponentGrids() ];
      intSerialArray  *bpijk = new intSerialArray[ cg.numberOfComponentGrids() ];

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  ibc[grid].redim( cg.numberOfInterpolationPoints( grid ) );
	  dr[grid].redim( nDonor(grid), 3 );
	  ijk[grid].redim( nDonor(grid), 3);
	  bpijk[grid].redim( cg.numberOfInterpolationPoints( grid ),3);
	}
      intSerialArray ndOffset( cg.numberOfGrids()+1);
      ndOffset(0) = 0;
      for ( int grid=1; grid<=cg.numberOfComponentGrids(); grid++ )
	ndOffset( grid ) = ndOffset(grid-1) + nDonor(grid-1);

      nDonor(R) = 0;
      for( int grid=1; grid<=cg.numberOfComponentGrids(); grid++ )
	{
	  int grd = grid-1;
	  MappedGrid & g = cg[grd];
	  
	  intArray & ip = cg.interpolationPoint[grd]; 
	  intArray & il = cg.interpoleeLocation[grd]; 
	  intArray & ig = cg.interpoleeGrid[grd];     

	  for( int i=0; i<cg.numberOfInterpolationPoints(grd); i++ )
	    {
	      int igrid = ig(i);
	      MappedGrid & cgridi = cg[igrid];
	      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
		{
		  int indexPosition=il(i,axis);
		  real relativeOffset=cg.interpolationCoordinates[grd](i,axis)/cgridi.gridSpacing(axis)
		    +cgridi.indexRange(Start,axis);
		  dr[igrid](nDonor(igrid),axis)= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5 
		    : relativeOffset-indexPosition;

		  bpijk[grd](i,axis) = ibnds(grd,axis) + ip(i,axis)-g.indexRange(Start,axis);
		  ijk[igrid](nDonor(igrid),axis) = ibnds(igrid,axis) + il(i,axis)-cgridi.indexRange(Start,axis);


		}
	      ibc[grd](i) = ndOffset(igrid)+nDonor(igrid)+1;
	      nDonor(igrid)++;
	      
	    }	  
	}

      int nStart=1, nEnd=0;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{

	  MappedGrid & g = cg[grid];
#if 0
	  int grd = grid-1;

	  
	  intArray & ip = cg.interpolationPoint[grd]; // JB,KB,LB before adjustment to new grid ranges
	  intArray & il = cg.interpoleeLocation[grd]; // JI,KI,LI before adjustment to new grid ranges
	  intArray & ig = cg.interpoleeGrid[grd];     // IBC

	  intArray ipa,// JB,KB,LB
	    ila; // JI,KI,LI

	  ipa = ip;
	  ila = il;
	  Range R(0,cg.numberOfInterpolationPoints(grd)-1);
	  RealArray dr(R,3);// DXINT,DYINT,DZINT
	  for( int i=0; i<cg.numberOfInterpolationPoints(grd); i++ )
	    {
	      int gridi = cg.interpoleeGrid[grd](i);
	      assert( gridi>=0 && gridi<cg.numberOfComponentGrids() );
	      
	      MappedGrid & cgridi = cg[gridi];
	      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
		{
		  int indexPosition=cg.interpoleeLocation[grd](i,axis);
		  real relativeOffset=cg.interpolationCoordinates[grd](i,axis)/cgridi.gridSpacing(axis)
		    +cgridi.indexRange(Start,axis);
		  dr(i,axis)= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5 
		    : relativeOffset-indexPosition;

		  ipa(i,axis) = ibnds(grd,axis) + ip(i,axis)-g.indexRange(Start,axis);
		  ila(i,axis) = ibnds(gridi,axis) + il(i,axis)-cgridi.indexRange(Start,axis);
		}
	    }
	  nEnd=nStart+cg.numberOfInterpolationPoints(grd)-1;
#endif

	  nEnd=nStart+nDonor(grid)-1;
	  Index I[3],&I1=I[0],&I2=I[1],&I3=I[2];
	  //	  getIndex(g.gridIndexRange(),I1,I2,I3);
	  getIndex(extendedGridIndexRange(g),I1,I2,I3);

	  intArray iblank(I1,I2,I3);
	  iblank(I1,I2,I3) = g.mask()(I1,I2,I3);
	  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		if ( iblank(i1,i2,i3)>0 ) 
		  iblank(i1,i2,i3) = 1;
		else
		  iblank(i1,i2,i3) = 0;
		  
	  

	  
	  int ibpnts = cg.numberOfInterpolationPoints(grid);
	  int iipnts = nDonor(grid);
	  int unit = 2;

	  int igrid = grid+1;


#ifdef OV_USE_DOUBLE
	  WPEG5D(unit,igrid,ngrids,dims(grid,0),dims(grid,1),dims(grid,2),
		 ijk[grid](0,0), ijk[grid](0,1), ijk[grid](0,2), 
		 dr[grid](0,0), dr[grid](0,1), dr[grid](0,2), 
		 bpijk[grid](0,0), bpijk[grid](0,1), bpijk[grid](0,2), 
		 *ibc[grid].getDataPointer(),
		 //		 ibpnts, iipnts, nStart, nEnd, *iblank.getDataPointer(),
		 ibpnts, iipnts, nEnd, nStart, *iblank.getDataPointer(),
		 usePeg4);

#else
	  WPEG5F(unit,igrid,ngrids,dims(grid,0),dims(grid,1),dims(grid,2),
		 ijk[grid](0,0), ijk[grid](0,1), ijk[grid](0,2), dr[grid](0,0), dr[grid](0,1), dr[grid](0,2), 
		 bpijk[grid](0,0), bpijk[grid](0,1), bpijk[grid](0,2), 
                 *ibc[grid].getDataPointer(),
		 //		 ibpnts, iipnts, nStart, nEnd, *iblank.getDataPointer(),
		 ibpnts, iipnts, nEnd,nStart, *iblank.getDataPointer(),
		 usePeg4);
#endif

#if 0
#ifdef OV_USE_DOUBLE
	  WPEG5D(unit,igrid,ngrids,dims(grid,0),dims(grid,1),dims(grid,2),
		 ila(0,0), ila(0,1), ila(0,2), dr(0,0), dr(0,1), dr(0,2), 
		 ipa(0,0), ipa(0,1), ipa(0,2), *ig.getDataPointer(),
		 //		 ibpnts, iipnts, nStart, nEnd, *iblank.getDataPointer(),
		 ibpnts, iipnts, nEnd, nStart, *iblank.getDataPointer(),
		 usePeg4);

#else
	  WPEG5F(unit,igrid,ngrids,dims(grid,0),dims(grid,1),dims(grid,2),
		 ila(0,0), ila(0,1), ila(0,2), dr(0,0), dr(0,1), dr(0,2), 
		 ipa(0,0), ipa(0,1), ipa(0,2), *ig.getDataPointer(),
		 //		 ibpnts, iipnts, nStart, nEnd, *iblank.getDataPointer(),
		 ibpnts, iipnts, nEnd,nStart, *iblank.getDataPointer(),
		 usePeg4);
#endif
#endif
	  //	  PRTPEG( unit,*il.getDataPointer(),*ip.getDataPointer(),*ig.getDataPointer(),
	  //		  *dr.getDataPointer(),cg.numberOfInterpolationPoints(grid),nStart,nEnd );
	
	  nStart=nEnd+1;
	}

      delete []   ibc;
      delete []   dr;
      delete []   ijk;
      delete []   bpijk;

    }

  return 0;
}

int DataFormats::
writeIngrid(CompositeGrid & cg,
	    const aString & gridFileName /* =nullString */ )
// ==============================================================================
/// \details  write a composte grid to an ascii file in an unstructured format
// =====================================================================================
{  
  UnstructuredMapping *umap = new UnstructuredMapping;
  umap->buildFromACompositeGrid(cg);

  writeIngrid(*umap, gridFileName);

  return 0;
}




int DataFormats::
writePlot3d(realCompositeGridFunction & u,
	    const RealArray & par )
// ==============================================================================
/// \details 
/// \param u (input) : grid function to save in the q file.
/// \param par (input) : plot3d parameters: 
///        par(0)=machNumber;
///        par(1)=alpha;
///        pra(2)=reynoldsNumber;
///        par(3)=t;
///        par(4)=gamma;
///        par(5)=Rg;
/// \param qFileNameIn (input) : optinally specify the name of the q file.
// =====================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert( giPointer!=NULL );

  GenericGraphicsInterface & gi = *giPointer;
  
  CompositeGrid & cg = *u.getCompositeGrid();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();


  aString gridFileName="myPlot3dFile.x";
  aString qFileName   ="myPlot3dFile.q";
  
  enum PrecisionEnum
  {
    singlePrecision,
    doublePrecision
  } precision;
  precision = sizeof(real)==sizeof(double) ? doublePrecision : singlePrecision;
  
 
  GUIState dialog;

  dialog.setWindowTitle("writePlot3d");
  dialog.setExitCommand("exit","Exit");


  dialog.setOptionMenuColumns(1);

  enum FormatTypeEnum
  {
    formatted,
    unformatted
  } formatType=formatted;
  
  aString formatTypeCommands[] = {"formatted", "unformatted", "" };
  dialog.addOptionMenu("file format:", formatTypeCommands, formatTypeCommands, (int)formatType );

  aString precisionTypeCommands[] = {"single precision", "double precision", "" };
  dialog.addOptionMenu("precision:", precisionTypeCommands, precisionTypeCommands, (int)precision );

  aString pushButtonCommands[] = {"save plot3d files",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"save iblank",
                          "convert to cell centered",
                          "" };  // 
  int writeIblank= numberOfComponentGrids>1;
  bool convertToCellCentered=false;
 
  int tbState[10];
  tbState[0] = writeIblank;
  tbState[1] = convertToCellCentered;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // **  dialog.addInfoLabel("Volume = 0");

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  
  int nt=0;
  textCommands[nt] = "grid file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)gridFileName);  nt++; 

  textCommands[nt] = "q file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)qFileName);  nt++; 

  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  // real machNumber=1., alpha=0., reynoldsNumber=0., t=0., gamma=1.4, Rg=1.;

  aString answer, line;

  
  gi.pushGUI(dialog);

  for( int it=0;; it++ )
  {
     
    gi.getAnswer(answer, "");
   
    int len;
    if( dialog.getTextValue(answer,"grid file","%s",gridFileName) ){} // 
    else if( dialog.getTextValue(answer,"q file","%s",qFileName) ){} // 
    else if( dialog.getToggleValue(answer,"save iblank",writeIblank) ){}//
    else if( dialog.getToggleValue(answer,"convert to cell centered",convertToCellCentered) ){}//
    else if( answer=="formatted" || answer=="unformatted" )
    {
      if( answer=="formatted" )
      { 
	formatType=formatted; 
      }
      else
      {
	formatType=unformatted;
      }
    }
    else if( answer=="single precision" || answer=="double precision" )
    {
      if( answer=="single precision" )
      { 
	precision=singlePrecision;
      }
      else
      {
	precision=doublePrecision;
      }
    }
    else if( answer=="save plot3d files" )
    {
      printf("Saving the plot3d grid file=[%s], and q (solution) file=[%s], save iblank=%i\n",
              (const char*)gridFileName,
	     (const char*)qFileName,writeIblank);

#ifdef USE_PPP
        printF("ERROR: Saving plot3d files does not work in parallel yet\n");
        OV_ABORT("ERROR: finish me for parallel");
#else
  
      // Compute the number of grid points in each direction for all grids:
      // We use the extendedGridIndexRange since we save ghost point values on interpolation boundaries
      // (Other ghost points are not saved in the plot3d file).
      IntegerArray nx(numberOfComponentGrids,3);
      for( int grid=0; grid<numberOfComponentGrids; grid++ )
      {
	MappedGrid & mg = cg[grid];
        const IntegerArray & eir = extendedGridIndexRange(mg); // eir holds ghost points on bc=0 sides. 
	for( int axis=0; axis<3; axis++ )
	  nx(grid,axis)= eir(1,axis)-eir(0,axis)+1;
      }

      const int iunit=24, junit=25;  // fortran unit numbers for grid and q files.
      for( int grid=0; grid<numberOfComponentGrids; grid++ )
      {

	MappedGrid & mg = cg[grid];
	mg .update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );

        realArray & xy = mg.vertex();
	intArray & mask = mg.mask();
        realArray & q = u[grid];
	
        // create the iblank array from the mask array
        //   iblank = 1 (valid point)
        //          = 0 unused point (hole point)
	Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
	getIndex(mg.dimension(),I1,I2,I3);
	intArray iblank(I1,I2,I3);
	for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      if ( mask(i1,i2,i3)==0 ) 
		iblank(i1,i2,i3) = 0;
	      else
		iblank(i1,i2,i3) = 1;

        const int g=grid+1;
	const int numberOfComponents = q.getLength(3);
	const int numberOfSpecies=1;  // do this for now
	int ierr=0;

        if( convertToCellCentered )
	{
	  OV_ABORT("writePlot3d:ERROR: 'convert to cell centered' NOT implemented yet.");
	}
	

	if( precision==singlePrecision )
	{
	  floatSerialArray xs;
	  floatSerialArray *xp;
	  if( sizeof(real)!=sizeof(float) )
	  { // make a single precision copy
	    xp=&xs;
	    xs.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
	    equals(xs,xy);

            OV_ABORT("writePlot3d:ERROR: conversion to single precion NOT implemented yet.");

	  }
	  else
	    xp=(floatSerialArray*)(&xy);

	  floatSerialArray & x = *xp;

	  WRPLT3DQS((char *)((const char*)gridFileName), (char *)((const char*)qFileName), (int)formatType,
		    iunit, junit,
		    numberOfComponentGrids,g,nx(0,0),nx(0,1),nx(0,2),
		    cg.numberOfDimensions(),
		    x.getBase(0)+1,x.getBound(0)+1,  // shift base and bound by 1 
		    x.getBase(1)+1,x.getBound(1)+1,
		    x.getBase(2)+1,x.getBound(2)+1,
		    *getDataPointer(x),
		    numberOfComponents, numberOfSpecies, *q.getDataPointer(),
		    writeIblank,*iblank.getDataPointer(), par(0), ierr,
                    strlen(gridFileName),strlen(qFileName) );


	}
	else
	{
	  doubleSerialArray xd;
	  doubleSerialArray *xp;
	  if( sizeof(real)!=sizeof(double) )
	  { // make a double precision copy
	    xp=&xd;
	    xd.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
	    equals(xd,xy);

            OV_ABORT("writePlot3d:ERROR: conversion to double precion NOT implemented yet.");

	  }
	  else
	    xp=(doubleSerialArray*)(&xy);
	  doubleSerialArray & x = *xp;

	  WRPLT3DQD((char *)((const char*)gridFileName), (char *)((const char*)qFileName), (int)formatType,
		    iunit, junit,
		    numberOfComponentGrids,g,nx(0,0),nx(0,1),nx(0,2),
		    cg.numberOfDimensions(),
		    x.getBase(0)+1,x.getBound(0)+1,  // shift base and bound by 1 
		    x.getBase(1)+1,x.getBound(1)+1,
		    x.getBase(2)+1,x.getBound(2)+1,
		    *getDataPointer(x),
		    numberOfComponents, numberOfSpecies, *q.getDataPointer(),
		    writeIblank,*iblank.getDataPointer(), par(0), ierr,
                    strlen(gridFileName),strlen(qFileName) );


	}



      } // end for grid 
      
#endif 

    }
    else if ( answer=="exit" )
    {
      break;
    }
    else 
    {
      printF("overtureToPlot3d:ERROR: unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }


  }
  
  gi.erase();
  gi.popGUI();  // pop dialog

 return 0;

}


//   aString menu[]=
//   {
//     "exit",
//     "save file",
//     "save file for overflow",
//     ">format",
//       "formatted",
//       "unformatted",
//     "<>precision",
//       "single precision",
//       "double precision",
//     "<>iblank",
//       "include iblank",
//       "do not include iblank",
//     "<",
//     ""
//   };
  
//  aString answer;
//  
//  int writeIblank=0;
//
//  printF("The default file format is an unformatted file in %s precision with no iblank array\n",
//	 (precision==singlePrecision ? "single" : "double"));
//
//  gi.appendToTheDefaultPrompt("writePlot3d>"); // set the default prompt
//
//
//
//
//  for(;;)
//  {
//    gi.getMenuItem(menu,answer,"choose an option");
//    if( answer=="exit" || answer=="done" )
//    {
//      break;
//    }
//    else if( answer=="formatted" )
//    {
//      formatType=formatted;
//    }
//    else if(  answer=="unformatted" )
//    {
//      formatType=unformatted;
//    }
//    else if( answer=="single precision" )
//    {
//      precision=singlePrecision;
//    }
//    else if( answer=="double precision" )
//    {
//      precision=doublePrecision;
//    }
//    else if( answer=="include iblank" )
//    {
//      writeIblank=1;
//    }
//    else if( answer=="do not include iblank" )
//    {
//      writeIblank=0;
//    }
//    else if( answer=="save q file" )
//    {
//      
///* ----------------      
//      const int iunit=24;
//      int ng=1, grid=1, ierr=0;
//      IntegerArray nx(3,ng),iblank;
//      nx=1;
//      
//      RealArray *xyPointer = (RealArray*)(&map.getGrid());
//      RealArray xyz;
//      
//      int rangeDimension = map.getRangeDimension();
//      for( int axis=0; axis<map.getDomainDimension(); axis++ )
//      {
//        nx(axis,0)=map.getGridDimensions(axis);
//      }
//      
//      const int saveForOverflow = answer=="save file for overflow";
//      if( saveForOverflow )
//      {
//	formatType=unformatted;
//        // for overflow we save a 2D file as 3D with 3 lines, make y direction constant
//        if( rangeDimension==2 )
//	{
//          printf("INFO: save a 2d grid as a 3D grid for overflow. 3 points in i3, y=constant\n");
//	  
//          RealArray & xy = *xyPointer;
//	  rangeDimension=3;
//	  Range I1=xy.dimension(0), I2=xy.dimension(1);
//	  xyz.redim(I1,I2,3,3);
//
//          real dy=1.;
//	  nx(2,0)=3;
//	  for( int i3=0; i3<3; i3++ )
//	  {
//	    xyz(I1,I2,i3,0)=xy(I1,I2,0,0);
//	    xyz(I1,I2,i3,1)=dy*(1-i3);
//	    xyz(I1,I2,i3,2)=xy(I1,I2,0,1);
//	  }
//          xyPointer=&xyz;
//	}
//      }
//
//      if( !writeIblank )
//      {
//        printf("Saving the file in `plot3d' format (fortran %s file, %s precision):\n"
//               "nx, ny, ny \n"
//               "x(0) x(1) .... x(ny-1) \n"
//               "y(0) y(1) .... y(ny-1) \n",
//	       (formatType==formatted ? "formatted" : "unformatted"),
//               (precision==singlePrecision ? "single" : "double"));
//	if( rangeDimension==3 )
//	  printf("z(0) z(1) .... z(nz-1)\n");
//      }
//      else
//      {
//        printf("Saving the file in `plot3d' format with iblank (fortran %s file):\n"
//               "nx, ny, ny \n"
//               "x(0) x(1) .... x(ny-1) \n"
//               "y(0) y(1) .... y(ny-1) \n",
//	       (formatType==formatted ? "formatted" : "unformatted"));
//	if( rangeDimension==3 )
//	  printf("z(0) z(1) .... z(nz-1)\n");
//        printf("iblank(0) iblank(1) .... iblank(nz-1) \n");
//      }
//      
//      RealArray & xy = *xyPointer;
//
//      if( precision==singlePrecision )
//      {
//        floatSerialArray xs;
//        floatSerialArray *xp;
//        if( sizeof(real)!=sizeof(float) )
//	{ // make a single precision copy
//          xp=&xs;
//	  xs.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
//	  equals(xs,xy);
//	}
//        else
//          xp=(floatSerialArray*)(&xy);
//
//	floatSerialArray & x = *xp;
//	WRPLT3DS((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
//		 rangeDimension,
//                 1,x.getRawDataSize(0),  // shift so 1=boundary instead of 0
//                 1,x.getRawDataSize(1), 
//                 1,x.getRawDataSize(2),
//                 *getDataPointer(x),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
//
//      }
//      else
//      {
//        doubleSerialArray xd;
//        doubleSerialArray *xp;
//        if( sizeof(real)!=sizeof(double) )
//	{ // make a double precision copy
//          xp=&xd;
//	  xd.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
//	  equals(xd,xy);
//	}
//        else
//          xp=(doubleSerialArray*)(&xy);
//	doubleSerialArray & x = *xp;
//
//	WRPLT3DD((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
//		 rangeDimension,
//                 1,x.getRawDataSize(0),  // shift so 1=boundary instead of 0
//                 1,x.getRawDataSize(1), 
//                 1,x.getRawDataSize(2),
//                 *getDataPointer(x),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
//
//
//      }
//      CLOSEPLT3D(iunit);
//
//  --------------- */
//
//    }
//    else
//    {
//      printF("Unknown response=[%s]\n",(const char*)answer);
//      gi.stopReadingCommandFile();
//    }
//  }
//  gi.unAppendTheDefaultPrompt();  // reset
//
//  return 0;
//}
//
