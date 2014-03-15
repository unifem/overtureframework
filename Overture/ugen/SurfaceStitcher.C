#include "SurfaceStitcher.h"
#include "ReductionMapping.h"
#include "CompositeGrid.h"
#include "Ugen.h"
#include "display.h"
#include "BodyDefinition.h"
#include "ParallelUtility.h"
#include "InterpolatePoints.h"

// ***********************************************************************
//   This class can be used to create an unstructured grid in the
//   the region between overlapping surface grids.
// ***********************************************************************

SurfaceStitcher::
SurfaceStitcher()
{
  pCgSurf=NULL;
  pUgen=NULL;

  maskOption=originalMask;
  surfMask=NULL;

}

SurfaceStitcher::
~SurfaceStitcher()
{
  delete pCgSurf;
  delete pUgen;
  delete [] surfMask;
}


int SurfaceStitcher::
defineSurfaces( CompositeGrid & cg, BodyDefinition *bodyDefinition /* =NULL */ )
// ==============================================================================================
// /Description:
//    Define the grid faces for one or more surfaces.
//    This function will build the CompositeGrid holding surfaces. 
//
// /cg (input): build surfaces from this volume grid.
// /bodyDefinition (input): if not NULL, this is a pointer to a BodyDefinition object that defines
//  the the surfaces as a collection of faces. If NULL, all physical boundary faces will be used. 
// 
// ================================================================================================
{

  buildSurfaceCompositeGrid( cg,bodyDefinition );

  return 0;
}


CompositeGrid* SurfaceStitcher::
getSurfaceCompositeGrid()
{
  return pCgSurf;
}


UnstructuredMapping* SurfaceStitcher::
getUnstructuredGrid()
{
  if( pUgen==NULL ) return NULL;
  else 
  {
    return pUgen->getUnstructuredMapping();
  }
}

Ugen* SurfaceStitcher::getUnstructuredGridGenertator()
{
  return pUgen;
}


int SurfaceStitcher::
stitchSurfaceCompositeGrid(int option /* = 1 */)
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
  assert( pCgSurf!=NULL );
  CompositeGrid & cgSurf = *pCgSurf;
  
  if( pUgen==NULL )
  {
    pUgen = new Ugen(*Overture::getGraphicsInterface());
  }
  Ugen & ugen = *pUgen;

  if ( cgSurf.numberOfGrids()>0 )
  {

    cout<<"SurfaceStitcher::stitchSurfaceCompositeGrid: there are "<<cgSurf.numberOfGrids()<<" surface grids"<<endl;

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


void SurfaceStitcher::
buildSurfaceCompositeGrid(CompositeGrid &cg, BodyDefinition *bodyDefinition /* =NULL */ )
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
  delete pCgSurf;
  pCgSurf = new CompositeGrid();

  CompositeGrid & cgSurf = *pCgSurf;


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
      if ( side==1 ) sj*=-1;
      // the ReductionMapping implicitly reverses the orientation of a surface when axis==1, so flip it back here
      if ( axis==1 ) sj*=-1;
      cgSurf.add(*redMap);

      const int surfGrid=cgSurf.numberOfGrids()-1;
      MappedGrid & mgSurf = cgSurf[surfGrid];
      mgSurf.update(MappedGridData::THEmask | MappedGrid::THEboundingBox);
      
      // printF(" SurfaceStitcher:: mg::share(0,2)=%i, Reduction: share=%i mgSurf::share=%i\n",
      //    mg.sharedBoundaryFlag(0,2),redMap->getShare(0,2),mgSurf.sharedBoundaryFlag(0,2));

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
  

  // keep a copy of the original mask on the surface grid
  maskOption=originalMask;
  delete surfMask; surfMask=NULL;
  if( cgSurf.numberOfComponentGrids()>0 )
  {
    surfMask = new intArray [cgSurf.numberOfComponentGrids()];

    for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++ )
    {
      surfMask[grid]=cgSurf[grid].mask();  
    }
  }

  if( bodyDefinition==NULL ) delete & bd;
  

}

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


int SurfaceStitcher::
enlargeGap( int gapWidth, int gridToChange /* = -1 */ )
// =================================================================================================
//  /Description:
//    Enlarge the gap between component surface grids. This routine will change the mask array
// on the surface grids. NOTE: The unstructured grid generator will reduce the gap-width by removing
// interpolation points. 
// 
//
// /gapWidth (input) : increase the gap by this many grid lines.
// /gridToChange (input) : increase the gap on this grid. By default increase the gap on all grids.
// ==================================================================================================
{
  if( gapWidth==0 ) return 0;
 
  maskOption=enlargedHoleMask;  // indicates that the mask has been enlarged
  
  assert( pCgSurf!=NULL );
  CompositeGrid & cgSurf = *pCgSurf;

  for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++ )
  {
    if( gridToChange==-1 || grid==gridToChange )
    {
      MappedGrid & mg = cgSurf[grid];
      Index I1,I2,I3;
      getIndex(mg.gridIndexRange(),I1,I2,I3);

      intArray & maskd = mg.mask();
      #ifdef USE_PPP
        intSerialArray mask;  getLocalArrayWithGhostBoundaries(maskd,mask);
        bool ok = ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3); 
        if( !ok ) continue;
      #else
        const intSerialArray & mask=maskd; 
      #endif

      intSerialArray mask2(I1,I2,I3);  // here is where we keep a copy of the mask

      
    int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
    const int maskDim1=mask.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    int *mask2p = mask2.Array_Descriptor.Array_View_Pointer2;
    const int mask2Dim0=mask2.getRawDataSize(0);
    const int mask2Dim1=mask2.getRawDataSize(1);
    const int mask2d1=mask2Dim0, mask2d2=mask2d1*mask2Dim1; 
#define MASK2(i0,i1,i2) mask2p[(i0)+(i1)*mask2d1+(i2)*mask2d2]

      int i1,i2,i3;
      for( int n=0; n<gapWidth; n++ )
      {
        // Mark all non-zero mask points that are next to mask<=0 pts as zero
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  MASK2(i1,i2,i3)=MASK(i1,i2,i3);
	  if( MASK(i1,i2,i3)>0 && 
	      (MASK(i1-1,i2-1,i3)<=0 || MASK(i1,i2-1,i3)<=0 || MASK(i1+1,i2-1,i3)<=0 ||
	       MASK(i1-1,i2  ,i3)<=0 ||                        MASK(i1+1,i2  ,i3)<=0 ||
	       MASK(i1-1,i2+1,i3)<=0 || MASK(i1,i2+1,i3)<=0 || MASK(i1+1,i2+1,i3)<=0 ) )
	  {
	    MASK2(i1,i2,i3)=0;
	  }
	}
	FOR_3(i1,i2,i3,I1,I2,I3)
	{
	  MASK(i1,i2,i3)=MASK2(i1,i2,i3);
	}
      }
      
    }
  }

  return 0;
}


int SurfaceStitcher::
enlargeGapWidth(real minGapSizeInGridLines /* = .5 */, int extraGapWidth /* = 0 */  )
// =================================================================================================
//  /Description:
//    Automatically enlarge the gap between component surface grids. This routine will change the mask array
// on the surface grids. NOTE: The unstructured grid generator will reduce the gap-width by removing
// interpolation points. 
// 
//
// /minGapSizeInGridLines (input) : create a gap that is at least this many grid lines wide
// /extraGapWidth (input) : expand the gap by this number of extra lines. 
// ==================================================================================================
{
  maskOption=enlargedHoleMask;  // indicates that the mask has been enlarged
  
  assert( pCgSurf!=NULL );
  CompositeGrid & cgSurf = *pCgSurf;
  const int rangeDimension = cgSurf.numberOfDimensions();
  cgSurf.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  
  
  InterpolatePoints interpolatePoints;
  // reduce the allowable region for valid interpolation so we remove fewer points
  interpolatePoints.setInterpolationOffset(minGapSizeInGridLines);

  int infoLevel=1; // 3 
  interpolatePoints.setInfoLevel( infoLevel );

  // numberMask[grid] = number of points masked out in the last iteration 
  //                    These were points that could interpolate from other grids.
  int *numberMasked = new int[cgSurf.numberOfComponentGrids()];
  for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++ )
    numberMasked[grid]=1;  // initial value=1 means try to mask more points on this grid


//   int halfWidth=1;  // should be dw/2
//   enlargeGap(halfWidth); // remove interpolation points 
  
  // we keep an extra mask to indicate which points were not interpolated on the
  //    previous pass
  // *wdh* 081211 intArray mask2a[cgSurf.numberOfComponentGrids()];
  intArray *mask2a = new intArray [cgSurf.numberOfComponentGrids()];

  const int maxNumberOfLayers=10;  // *** fix this ***

  for( int layer=0; layer<maxNumberOfLayers; layer++ )
  {
  
    for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++ )
    {
      if( numberMasked[grid]==0 ) continue; 

      MappedGrid & mg = cgSurf[grid];
      const realArray & center = mg.center();

      // ::display(center,"center","%5.2f ");

      
      Index I1,I2,I3;
      getIndex(extendedGridIndexRange(mg),I1,I2,I3);

      intArray & maskd = mg.mask();
#ifdef USE_PPP
      intSerialArray mask;  getLocalArrayWithGhostBoundaries(maskd,mask);
      bool ok = ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3); 
      if( !ok ) continue;
#else
      const intSerialArray & mask=maskd; 
#endif

    intArray & mask2 = mask2a[grid];
    if( layer==0 )
    {
      mask2.redim(mask.dimension(0),mask.dimension(1),mask.dimension(2));
      mask2=0;
    }
    
      int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
    const int maskDim1=mask.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    int *mask2p = mask2.Array_Descriptor.Array_View_Pointer2;
    const int mask2Dim0=mask2.getRawDataSize(0);
    const int mask2Dim1=mask2.getRawDataSize(1);
    const int mask2d1=mask2Dim0, mask2d2=mask2d1*mask2Dim1; 
#define MASK2(i0,i1,i2) mask2p[(i0)+(i1)*mask2d1+(i2)*mask2d2]

    int i1,i2,i3;

    // On the first pass, remove all interpolation points
    if( layer==0 )
    {

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( MASK(i1,i2,i3)<0 )
	{
	  MASK(i1,i2,i3)=0;
	}
      }
      continue;
    }

      numberMasked[grid]=0;  // count how many points can be interpolated and are thus masked out
      
      const int maxPoints = I1.getLength()*I2.getLength()*I3.getLength();
      IntegerArray ia(maxPoints,3);

      int *iap = ia.Array_Descriptor.Array_View_Pointer1;
      const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]


      // make a list of points next to hole points

      // **NOTE: we could keep an extra mask to indicate which points were not interpolated on the
      //         previous pass
      int i=0;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( MASK(i1,i2,i3)!=0 && MASK2(i1,i2,i3)==0 && 
	    (MASK(i1-1,i2-1,i3)==0 || MASK(i1,i2-1,i3)==0 || MASK(i1+1,i2-1,i3)==0 ||
	     MASK(i1-1,i2  ,i3)==0 ||                        MASK(i1+1,i2  ,i3)==0 ||
	     MASK(i1-1,i2+1,i3)==0 || MASK(i1,i2+1,i3)==0 || MASK(i1+1,i2+1,i3)==0 ) )
	{
	  IA(i,0)=i1; IA(i,1)=i2; IA(i,2)=i3;
          i++;
	  assert( i<maxPoints );
	}
      }
      int numberToCheck=i;
      if( numberToCheck==0 ) continue;
      
      realArray xa(numberToCheck,rangeDimension);
      real *xap = xa.Array_Descriptor.Array_View_Pointer1;
      const int xaDim0=xa.getRawDataSize(0);
#define XA(i0,i1) xap[i0+xaDim0*(i1)]
      for( i=0; i<numberToCheck; i++ )
      {
	i1=IA(i,0); i2=IA(i,1); i3=IA(i,2);
	for( int axis=0; axis<rangeDimension; axis++ )
	  XA(i,axis)=center(i1,i2,i3,axis);

	if( infoLevel & 2 )
	  printF("enlargeGap: check grid=%i (i1,i2,i3)=(%i,%i,%i) x=(%g,%g,%g)\n",
		 grid, IA(i,0),IA(i,1),IA(i,2), XA(i,0),XA(i,1),XA(i,2));
      }
     
      // Attempt to interpolate from all grids in cgSurf except this "grid":
      IntegerArray checkTheseGrids(cgSurf.numberOfComponentGrids());
      checkTheseGrids=1; checkTheseGrids(grid)=0;

      // xa.resize(Range(numberToCheck),rangeDimension);
      // realArray xp(numberToCheck,rangeDimension);

      #ifndef USE_PPP
      interpolatePoints.buildInterpolationInfo(xa, cgSurf, NULL, &checkTheseGrids);
      #else
      Overture::abort("finish me for parallel");
      #endif
      const IntegerArray & status = interpolatePoints.getStatus();

      int *statusp = status.Array_Descriptor.Array_View_Pointer0;
#define STATUS(i0) statusp[i0]

      for( i=0; i<numberToCheck; i++ )
      {
	
	if( STATUS(i)==InterpolatePoints::interpolated )
	{
          if( infoLevel & 2 )
            printF("enlargeGap: mask out point grid=%i (i1,i2,i3)=(%i,%i,%i)\n",grid,IA(i,0),IA(i,1),IA(i,2));
	  MASK(IA(i,0),IA(i,1),IA(i,2))=0;

          numberMasked[grid]++;
	}
        else
	{
          MASK2(IA(i,0),IA(i,1),IA(i,2))=1;  // this point could not be interpolated
	}
	
      }
      if( infoLevel & 1 )
        printF("enlargeGap: layer=%i grid=%i numberMasked=%i\n",layer,grid,numberMasked[grid]);
      
    }
  }
  delete [] numberMasked;
  

  if( extraGapWidth>0 )
  { // widen the gap on all but the highest priority grid
    for( int grid=0; grid<cgSurf.numberOfComponentGrids()-1; grid++ )
    {
      enlargeGap(extraGapWidth,grid); 
    }
  }
  
  delete [] mask2a;
  
  return 0;
}



int SurfaceStitcher::
setMask( SurfaceMaskEnum option )
// ===========================================================================================
// /Description:
//    Set the mask in the surface grid to either the original mask or the mask for the enlarged gap.
//
// /option (input) : if option==originalMask then set the mask on the surface grid to the original
//   mask (before the hole was enlarged). If option==enlargedHoleMask then set the mask to the
//   the enlarged hole mask.
// 
// ===========================================================================================
{
  
  if( pCgSurf==NULL )
  {
    printF("SurfaceStitcher::setMask:ERROR: the surface grid is not built yet!\n");
    return 1;
  }
  if( surfMask==NULL )
  {
    printF("SurfaceStitcher::setMask:ERROR: the surface grid mask has not been created!?\n");
    return 1;
  }
  
  if( option!=maskOption )
  {
    // swap the masks

    assert( pCgSurf!=NULL );
    CompositeGrid & cgSurf = *pCgSurf;

    for( int grid=0; grid<cgSurf.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cgSurf[grid];
      if( mg.getGridType()!=MappedGrid::structuredGrid ) continue;  // skip unstructured grids
      
      intArray & mask = mg.mask();

#ifdef USE_PPP
      intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
      intSerialArray surfMaskLocal;  getLocalArrayWithGhostBoundaries(surfMask[grid],surfMaskLocal);
#else
      const intSerialArray & maskLocal=mask; 
      const intSerialArray & surfMaskLocal=surfMask[grid]; 
#endif
	
//       printF("SurfaceStitcher::setMask: grid=%i mask=[%i,%i][%i,%i] surfMask=[%i,%i][%i,%i]\n",
// 	     grid,
//              maskLocal.getBase(0),maskLocal.getBound(0),maskLocal.getBase(1),maskLocal.getBound(1),
//              surfMaskLocal.getBase(0),surfMaskLocal.getBound(0),surfMaskLocal.getBase(1),surfMaskLocal.getBound(1));
      
      assert( maskLocal.dimension(0)==surfMaskLocal.dimension(0) );
      assert( maskLocal.dimension(1)==surfMaskLocal.dimension(1) );

      Index I1=maskLocal.dimension(0), I2=maskLocal.dimension(1), I3=maskLocal.dimension(2);
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        int temp = maskLocal(i1,i2,i3);
	maskLocal(i1,i2,i3)=surfMaskLocal(i1,i2,i3);
	surfMaskLocal(i1,i2,i3)=temp;
      }
      

    }

    maskOption=option;  // here is the new option

  }
  

  return 0;
}
