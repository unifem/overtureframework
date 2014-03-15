#include "Overture.h"
#include "Ogen.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"

static int numberOfCheckHoleCuttingWarnings=0;
static int numberOfIncreasingNumberOfHoleWarnings=0;

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
 #define GET_LOCAL_CONST(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray & xs = xd
 #define GET_LOCAL_CONST(type,xd,xs)\
    const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

int Ogen::
projectToBoundary( CompositeGrid & cg,
		   const int & grid, 
		   const realArray & r,
		   const int iv[3], 
		   const int ivp[3], 
		   real rv[3] )
// ===============================================================================================
// /Description:
//   Determine the intersection of the line segment r(iv) --> r(ivp) with the rBound bounding box.
// or return the r(ivp) if there is no intersection
// ===============================================================================================
{

  // look for solutions to  rBound = r(iv) + s [ r(ivp) - r(iv) ]
  // Choose the root with a minimum value for s in [0,1]
  const real eps = REAL_EPSILON*10.;
  real sMin=2., s, rv0[3], rv1[3];
  int dir;
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
  {
    rv0[dir]=r(iv[0],iv[1],iv[2],dir);
    rv1[dir]=r(ivp[0],ivp[1],ivp[2],dir);

    real rDiff=rv1[dir]-rv0[dir];
    if( fabs(rDiff) > eps )
    {
      rDiff=1./rDiff;
      s=(rBound(Start,dir,grid)-rv0[dir])*rDiff;
      if( fabs(s-.5)<=.51 )
      {
	sMin=min(sMin,s);
        continue;     // this is a possible root. s in [0,1]. The other choice is not possible.
      }
      else
      {
	s=(rBound(End,dir,grid)-rv0[dir])*rDiff;
        if( fabs(s-.5)<.51 )
	  sMin=min(sMin,s);
      }
    }
  }
  if( fabs(sMin-.5)>.51 )
    sMin=1.;  // *wdh* 990421

  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
    rv[dir]=rv0[dir]+sMin*(rv1[dir]-rv0[dir]);
  return 0;
}

// bool Ogen::
// sharedBoundaryPoint( i,r, cg, g,side,axis, g2,map2 ) 
// {
//   bool canInterpolate=FALSE;
//   tol = max(g2.sharedBoundaryTolerance()(Range(0,1),Rx))*max(g2.gridSpacing()(Rx));
//   if( max(fabs(r(i,Rx)-.5))<=.5+tol ) 
//   {
//     int normalDirection=-1;
//     // Check to see if we are close to a physical boundary of another grid.
//     int dir;
//     for( dir=0; dir<cg.numberOfDimensions(); dir++ )
//     {
//       for( int side2=Start; side2<=End; side2++ )
//       {
// 	if( g2.boundaryCondition()(side2,dir)>0 &&
// 	    map2.getTypeOfCoordinateSingularity(side2,dir)!=Mapping::polarSingularity &&
// 	    ( fabs(r(i,dir)-side2) < boundaryEps || 
// 	      ( g.sharedBoundaryFlag()(side,axis)!=0 && 
// 		g.sharedBoundaryFlag()(side,axis)==g2.sharedBoundaryFlag()(side2,dir) &&
// 		fabs(r(i,dir)-side2) < g2.sharedBoundaryTolerance()(side2,dir)*g2.gridSpacing()(dir) )
// 	      ) )
// 	{
// 	  // double check that the normals to the surfaces are both in the same direction
//                     
// 	  i3=g2.indexRange(Start,axis3);
// 	  for( int ax=0; ax<cg.numberOfDimensions(); ax++ )
// 	  {
// 	    // iv : nearest point
// 	    if( ax!=dir )
// 	    {
// 	      iv[ax]=r(i,ax)/g2.gridSpacing(ax) + g2.indexRange(Start,ax) + cvShift;
// 	      iv[ax]=max(g2.dimension(Start,ax),min(g2.dimension(End,ax),iv[ax]));
// 	    }
// 	    else
// 	      iv[dir]=g2.gridIndexRange(side2,dir);    // use gir for cell centered grids
// 	  }
// 	  const realArray & normal2 = g2.vertexBoundaryNormal(side2,dir);
// 	  // ****** we may have to get a better approximation to the normal if a corner is nearby ??
// 	  real cosAngle = sum(normal(ia(i,0),ia(i,1),ia(i,2),Rx)*normal2(i1,i2,i3,Rx));
// 	  if( cosAngle>.7 )  // .8 // if cosine of the angle between normals > ?? 
// 	  {
// 	    canInterpolate=TRUE; 
// 	    normalDirection=dir;
// 	    break;
// 	  }
// 	  else
// 	  {
// 	    if( cosAngle>.3 )
// 	    {
// 	      printf("sharedBoundaryPoint:WARNING: a boundary point on grid %s can interpolate from the"
// 		     " boundary of grid %s,\n"
// 		     "   but the cosine of the angle between the surface normals is %e (too small).\n"
// 		     "   No interpolation assumed. r=(%e,%e,%e)\n",
// 		     (const char*)map1.getName(Mapping::mappingName),
// 		     (const char*)map2.getName(Mapping::mappingName),cosAngle,
// 		     r(i,0),r(i,1),(cg.numberOfDimensions()==2 ? 0. : r(i,2)));
// 	    }
// 	  }
// 	}
//       }
//     }
// 		  
//     if( canInterpolate )
//     { // tangential directions to the boundary have a stricter tolerance
//       for( dir=0; dir<cg.numberOfDimensions(); dir++ )
//       {
// 	if( dir!=normalDirection && fabs(r(i,dir)-.5) >= .5+boundaryEps )
// 	{
// 	  canInterpolate=FALSE;
// 	  break;
// 	}
//       }
//     }
//     
//   }
//   return canInterpolate;
// }
// 

int Ogen::
checkHoleCutting(CompositeGrid & cg)
// =================================================================================================
// /Description:
//   Double check the hole cutting.
// =================================================================================================
{
  real time0 = getCPU();
  
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  // if( true ) return 0;
  

  if( numberOfBaseGrids==1 ) return 0;

  if( info & 4 ) printf("check hole cutting...\n");
  if( debug & 2 ) fprintf(plogFile,"check hole cutting...\n");

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int grid; 

  int maximumNumberOfErrors=100;
  Range R=maximumNumberOfErrors;
  IntegerArray ia(R,3);
  

  const int bogusInverseGrid=cg.numberOfGrids()+100;  // also appears in classify
  
  int numberOfErrors=0;
  
  for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
  {
    MappedGrid & g = cg[grid];
    intArray & inverseGridd = cg.inverseGrid[grid];
    realArray & rId = cg.inverseCoordinates[grid];
    const realArray & centerd = g.center();
    intArray & maskgd = g.mask();
    const bool isRectangular = g.isRectangular();

    maskgd.updateGhostBoundaries();  // *wdh* 2012/07/08 -- this fixes suspicious point problem with turbine grid

    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);
    GET_LOCAL(int,maskgd,maskg);
    
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(centerd,center);
    #else
      const realSerialArray & center = g.center();
    #endif

    int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]


    if( debug & 8 )
      displayMask(maskg,sPrintF("checkHoleCutting: mask on grid=%i",grid),plogFile);

    // *wdh* 020614 getIndex(g.indexRange(),I1,I2,I3,-1); // ******* check interior only for now ****
    // *wdh* 040912 getIndex(g.extendedIndexRange(),I1,I2,I3,-1);  //  extend to interpolation boundaries
    const IntegerArray & eir = g.extendedIndexRange();
    getIndex(eir,I1,I2,I3);  //  *wdh* 040912 - check holes on interpolation ghost points too

    bool ok=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);
    for( int dir=0; dir<numberOfDimensions; dir++ )
    {
      // Include 1 parallel ghost since there could be a hole point on the parallel boundary
      // and a supicious point in the parallel ghost region -- does this work??
      //  *wdh* 2012/07/08 -- This assumes we have at least 2 parallel ghost
      //                   -- this fixes suspicious point problem with turbine grid
      if( Iv[dir].getBase() >eir(0,dir) ){ Iv[dir]=Range(Iv[dir].getBase()-1,Iv[dir].getBound()  ); }
      if( Iv[dir].getBound()<eir(1,dir) ){ Iv[dir]=Range(Iv[dir].getBase()  ,Iv[dir].getBound()+1); } 
    }


    const int I1Base=I1.getBase(), I1Bound=I1.getBound();
    const int I2Base=I2.getBase(), I2Bound=I2.getBound();
    const int I3Base=I3.getBase(), I3Bound=I3.getBound();
    
    const IntegerArray & dimension = g.dimension();
    // *wdh* 040912 -- check holes on interpolation ghost points too
    // *wdh* const int id1a=dimension(0,0)+1, id1b=dimension(1,0)-1;
    // *wdh* const int id2a=dimension(0,1)+1, id2b=dimension(1,1)-1;
    // *wdh* const int id3a=dimension(0,2)+1, id3b=dimension(1,2)-1;

    // *wdh* 040928 Only check mask up to extended index range
    // *wdh* 040928 const int id1a=dimension(0,0), id1b=dimension(1,0);
    // *wdh* 040928 const int id2a=dimension(0,1), id2b=dimension(1,1);
    // *wdh* 040928 const int id3a=dimension(0,2), id3b=dimension(1,2);

    // *wdh* 040928 Only check mask up to extended index range:
    const int id1a=eir(0,0), id1b=eir(1,0);
    const int id2a=eir(0,1), id2b=eir(1,1);
    const int id3a=eir(0,2), id3b=eir(1,2);
    
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      g.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=g.gridIndexRange(0,dir);
	if( g.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #define XC0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
    #define XC1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
    #define XC2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];

    if( true )
    {
      // --- new way for parallel ---

      // ib(i,0:2) i=0,1,... check these points 
      int maxNumToCheck=200;  // what should this be?
      IntegerArray ib(maxNumToCheck,3), ic(maxNumToCheck,3);
    
      bool cuttingIsValid=false;
      const int maximumNumberOfIterations=10;  // what should this be?
      int numberOfWarnings=0;
      const int maxNumberOfWarnings=50;
      // --- We may have to iterate a few times to fix points -- fixing some point may then require new
      //     points to be fixed.
      int it;
      for( it=0; it<maximumNumberOfIterations && !cuttingIsValid; it++ ) 
      {
	int numberFixed=0;
	const int numberOfErrorsAtStart=numberOfErrors;

	// -- First make a list of suspicious points ---
	int numToCheck=0;
	if( ok )
	{
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  {
	    for( i2=I2.getBase(); i2<=I2Bound; i2++ )
	    {
	      for( i1=I1Base; i1<=I1Bound; i1++ )
	      {
		if( MASK(i1,i2,i3)==0 )
		{
		  j3=i3;
		  const int j1a=max(i1-1,id1a), j1b=min(i1+1,id1b); // *wdh* 031130
		  const int j2a=max(i2-1,id2a), j2b=min(i2+1,id2b);
		  if( numberOfDimensions==2 )
		  {
		    for( j2=j2a; j2<=j2b; j2++ )
		    {
		      int j2m=max(j2-1,id2a), j2p=min(j2+1,id2b); // *wdh* 040912
		      for( j1=j1a; j1<=j1b; j1++ )
		      {
			int j1m=max(j1-1,id1a), j1p=min(j1+1,id1b);  // *wdh* 040912
	      
			// ***NOTE*** we could still miss an error where holes points were marked
			//            but no interpolation points!  (see cicThin.cmd)
			if( MASK(j1,j2,j3)>0 &&(
			      MASK(j1m,j2m,j3)<0 || MASK(j1 ,j2m,j3)<0 || MASK(j1p,j2m,j3)<0 || 
			      MASK(j1m,j2 ,j3)<0 || MASK(j1 ,j2 ,j3)<0 || MASK(j1p,j2 ,j3)<0 || 
			      MASK(j1m,j2p,j3)<0 || MASK(j1 ,j2p,j3)<0 || MASK(j1p,j2p,j3)<0 ) )
			{
			  // Check here if point (j1,j2,j3) can interpolate from another grid.
			  // (There could be a finer grid in this area that is meant to interpolate. cf. cicThin.cmd)

			  // *wdh* 081028 -- only check pts that are also next to hole points
			  bool neighbouringHolePoints =
			    MASK(j1m,j2m,j3)<0 || MASK(j1 ,j2m,j3)<0 || MASK(j1p,j2m,j3)<0 || 
			    MASK(j1m,j2 ,j3)<0 || MASK(j1 ,j2 ,j3)<0 || MASK(j1p,j2 ,j3)<0 || 
			    MASK(j1m,j2p,j3)<0 || MASK(j1 ,j2p,j3)<0 || MASK(j1p,j2p,j3)<0 ;
			
			  if( !neighbouringHolePoints ) 
                            continue;

			  // --- This is a suspicious point ---

			  if( MASK(j1,j2,j3) & MappedGrid::USESbackupRules )
			    continue;  // this point already in the list

			  // mark this point so we don't have duplicates - it should be safe to use USESbackupRules
			  // since this is normally only used for interp. pts. We could undo this below if need be
			  MASK(j1,j2,j3) |= MappedGrid::USESbackupRules;  

			  if( numToCheck>=maxNumToCheck )
			  {
			    maxNumToCheck*=2;
			    ib.resize(maxNumToCheck,3);
			    ic.resize(maxNumToCheck,3);
			  }
			  for( int axis=0; axis<3; axis++ )
			  {
			    ib(numToCheck,axis)=jv[axis];  // suspicious discretization point
			    ic(numToCheck,axis)=iv[axis];  // corresponding hole point
			  }
			  
			  numToCheck++;

			}
		      }
		    }
		  }
		  else
		  {
		    const int j3a=max(i3-1,id3a), j3b=min(i3+1,id3b);
		    for( j3=j3a; j3<=j3b; j3++ )
		    {
		      int j3m=max(j3-1,id3a), j3p=min(j3+1,id3b); // *wdh* 040912
		      for( j2=j2a; j2<=j2b; j2++ )
		      {
			int j2m=max(j2-1,id2a), j2p=min(j2+1,id2b); // *wdh* 040912
			for( j1=j1a; j1<=j1b; j1++ )
			{
			  int j1m=max(j1-1,id1a), j1p=min(j1+1,id1b); // *wdh* 040912

			  if( MASK(j1,j2,j3)>0 &&(
				MASK(j1m,j2m,j3m)<0 || MASK(j1 ,j2m,j3m)<0 || MASK(j1p,j2m,j3m)<0 || 
				MASK(j1m,j2 ,j3m)<0 || MASK(j1 ,j2 ,j3m)<0 || MASK(j1p,j2 ,j3m)<0 || 
				MASK(j1m,j2p,j3m)<0 || MASK(j1 ,j2p,j3m)<0 || MASK(j1p,j2p,j3m)<0 || 
				MASK(j1m,j2m,j3 )<0 || MASK(j1 ,j2m,j3 )<0 || MASK(j1p,j2m,j3 )<0 || 
				MASK(j1m,j2 ,j3 )<0 || MASK(j1 ,j2 ,j3 )<0 || MASK(j1p,j2 ,j3 )<0 || 
				MASK(j1m,j2p,j3 )<0 || MASK(j1 ,j2p,j3 )<0 || MASK(j1p,j2p,j3 )<0 || 
				MASK(j1m,j2m,j3p)<0 || MASK(j1 ,j2m,j3p)<0 || MASK(j1p,j2m,j3p)<0 || 
				MASK(j1m,j2 ,j3p)<0 || MASK(j1 ,j2 ,j3p)<0 || MASK(j1p,j2 ,j3p)<0 || 
				MASK(j1m,j2p,j3p)<0 || MASK(j1 ,j2p,j3p)<0 || MASK(j1p,j2p,j3p)<0 
				) )
			  {
			    // *wdh* 081028 -- only check pts that are also next to hole points
			    bool neighbouringHolePoints =
			      MASK(j1m,j2m,j3m)==0 || MASK(j1 ,j2m,j3m)==0 || MASK(j1p,j2m,j3m)==0 || 
			      MASK(j1m,j2 ,j3m)==0 || MASK(j1 ,j2 ,j3m)==0 || MASK(j1p,j2 ,j3m)==0 || 
			      MASK(j1m,j2p,j3m)==0 || MASK(j1 ,j2p,j3m)==0 || MASK(j1p,j2p,j3m)==0 || 
			      MASK(j1m,j2m,j3 )==0 || MASK(j1 ,j2m,j3 )==0 || MASK(j1p,j2m,j3 )==0 || 
			      MASK(j1m,j2 ,j3 )==0 || MASK(j1 ,j2 ,j3 )==0 || MASK(j1p,j2 ,j3 )==0 || 
			      MASK(j1m,j2p,j3 )==0 || MASK(j1 ,j2p,j3 )==0 || MASK(j1p,j2p,j3 )==0 || 
			      MASK(j1m,j2m,j3p)==0 || MASK(j1 ,j2m,j3p)==0 || MASK(j1p,j2m,j3p)==0 || 
			      MASK(j1m,j2 ,j3p)==0 || MASK(j1 ,j2 ,j3p)==0 || MASK(j1p,j2 ,j3p)==0 || 
			      MASK(j1m,j2p,j3p)==0 || MASK(j1 ,j2p,j3p)==0 || MASK(j1p,j2p,j3p)==0;
			
			    if( !neighbouringHolePoints ) continue;
			
			    // --- This is a suspicious point ---

                            if( MASK(j1,j2,j3) & MappedGrid::USESbackupRules )
			      continue;  // this point already in the list

                            // mark this point so we don't have duplicates - it should be safe to use USESbackupRules
                            // since this is normally only used for interp. pts. We could undo this below if need be
                            MASK(j1,j2,j3) |= MappedGrid::USESbackupRules;  
			    if( numToCheck>=maxNumToCheck )
			    {
			      maxNumToCheck*=2;
			      ib.resize(maxNumToCheck,3);
			      ic.resize(maxNumToCheck,3);
			    }
			    for( int axis=0; axis<3; axis++ )
			    {
			      ib(numToCheck,axis)=jv[axis];  // suspicious discretization point
			      ic(numToCheck,axis)=iv[axis];  // corresponding hole point
			    }
		      
			    numToCheck++;
		      
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  } // end for( i3 )
	} // end if( ok ) 
	

	if( debug & 4 )
	  fprintf(plogFile,"checkHoleCutting: grid=%i, suspicious points: numToCheck=%i. (I1,I2,i3)=[%i,%i]"
		  "[%i,%i][%i,%i]\n",grid,numToCheck,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
                  I3.getBase(),I3.getBound());

        int totalNumToCheck=ParallelUtility::getSum(numToCheck);
	if( totalNumToCheck>0 ) // interpolatePoints is a parallel operation so all processors must call it
	{
	  if( true ) 
	  {
	    // -- new way : call interpolatePoints to check all points at once ---  *wdh* 110627

	    IntegerArray interpolates;
	    if( numToCheck>0 )
	    {
	      interpolates.redim(numToCheck);
	    }

            interpolatePoints( cg,grid, numToCheck, ib, interpolates );
	    
	    for( int i=0; i<numToCheck; i++ )
	    {
	      for( int axis=0; axis<3; axis++ )
	      {
		jv[axis]=ib(i,axis);  // suspicious discretization point
		iv[axis]=ic(i,axis);  // corresponding hole point
	      }
	      

	      if( interpolates(i) )
	      {
		numberFixed++;
		if( info & 2 )
		{
		  fprintf(plogFile,"checkHoleCutting:INFO(1): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
			  "near the hole pt (%i,%i,%i) has been interpolated.\n",
			  j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
		  if( numberFixed<20 )
		    printf("checkHoleCutting:INFO(1): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
			   "near the hole pt (%i,%i,%i) has been interpolated.\n",
			   j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
		  else if( numberFixed==20 )
		    printf("checkHoleCutting:INFO(1): suspicious discr. pt: I am not printing any "
			   "more of these messages. See ogen.log\n");
			  
		}
	      }
	      else
	      {
		for( int axis=0; axis<numberOfDimensions; axis++ )
		  iv[axis]=ic(i,axis);

		if( debug & 1 && numberOfWarnings<maxNumberOfWarnings )
		{
		  printf("checkHoleCutting:WARNING: hole pt (%i,%i,%i) is next to an interp. pt and a discr. pt (%i,%i,%i) (<- orphan)"
			 " on grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,(const char*)g.getName());
		  if( numberOfWarnings==maxNumberOfWarnings-1 )
		    printf("***INFO*** See log file for further warnings of `hole pt next to interp pt..'\n");
		}
		numberOfWarnings++;
		if( debug & 1 )
		  fprintf(plogFile,"checkHoleCutting:WARNING: hole pt (%i,%i,%i) is next to an interp. pt "
			  "and a discr. pt (%i,%i,%i) (<- orphan) on grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,
			  (const char*)g.getName());


		if( numberOfOrphanPoints >= orphanPoint.getLength(0) )
		{
		  int num=numberOfOrphanPoints*2+10;
		  orphanPoint.resize(num,orphanPoint.getLength(1));
		}
		const int n=numberOfOrphanPoints;
		if( !isRectangular )
		{
		  for( int axis=0; axis<numberOfDimensions; axis++ )
		    orphanPoint(n,axis)=center(j1,j2,j3,axis);
		}
		else
		{
		  for( int axis=0; axis<numberOfDimensions; axis++ )
		    orphanPoint(n,axis)=XC(jv,axis);
		}
			  
		orphanPoint(n,numberOfDimensions)=grid;
		numberOfOrphanPoints++;


		ia(numberOfErrors,0)=i1;
		ia(numberOfErrors,1)=i2;
		ia(numberOfErrors,2)=i3;
		numberOfErrors++;
		if( numberOfErrors>=maximumNumberOfErrors )
		{
		  maximumNumberOfErrors*=2;
		  ia.resize(maximumNumberOfErrors,3);
		}
	      }

	    } // end for( i )


	  }
	  else
	  {
	    // old way : call one pt at a time

	    int infoLevel=0;
	    bool checkBoundaryPoint=false;
	    bool checkInterpolationCoords=false;
	    bool interpolatePoint=true;

	    for( int i=0; i<totalNumToCheck; i++ )
	    {
	      if( i<numToCheck )
	      {
		// check a point on this processor
		interpolatePoint=true;
		for( int axis=0; axis<3; axis++ )
		  jv[axis]=ib(i,axis);
	      
	      }
	      else
	      {
		// In parallel there may be more points to check on other processors but
		// we still need to call interpolateAPoint
		// Do this for now:
		interpolatePoint=false; // do not adjust this point
		if( numToCheck>0 )
		{ // just recheck the first point 
		  for( int axis=0; axis<3; axis++ )
		    jv[axis]=ib(0,axis);
		}
		else
		{
		  for( int axis=0; axis<numberOfDimensions; axis++ )
		    jv[axis]=0;
		}
	      }

	      // --- FIX ME -- interpolateAPoint should take an array of points to check ---- FIX ME 

	      // try to interpolate from another grid
	      bool ok=interpolateAPoint(cg, grid, jv, interpolatePoint, checkInterpolationCoords, 
					checkBoundaryPoint, infoLevel );

	      if( ok )
	      {
		numberFixed++;
		if( info & 2 )
		{
		  fprintf(plogFile,"checkHoleCutting:INFO(2): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
			  "near the hole pt (%i,%i,%i) has been interpolated\n",
			  j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
		  if( numberFixed<20 )
		    printf("checkHoleCutting:INFO(2): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
			   "near the hole pt (%i,%i,%i) has been interpolated\n",
			   j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
		  else if( numberFixed==20 )
		    printf("checkHoleCutting:INFO(2): suspicious discr. pt: I am not printing any "
			   "more of these messages. See ogen.log\n");
			  
		}
	      }
	      else
	      {
		if( debug & 1 && numberOfWarnings<maxNumberOfWarnings )
		{
		  printf("***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt and a discr. pt (%i,%i,%i)"
			 " grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,(const char*)g.getName());
		  if( numberOfWarnings==maxNumberOfWarnings-1 )
		    printf("***INFO*** See log file for further warnings of `hole pt next to interp pt..'\n");
		}
		numberOfWarnings++;
		if( debug & 1 )
		  fprintf(plogFile,"***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt "
			  "and a discr. pt (%i,%i,%i) grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,
			  (const char*)g.getName());

		if( numberOfOrphanPoints >= orphanPoint.getLength(0) )
		{
		  int num=numberOfOrphanPoints*2+10;
		  orphanPoint.resize(num,orphanPoint.getLength(1));
		}
		const int n=numberOfOrphanPoints;
		if( !isRectangular )
		{
		  for( int axis=0; axis<numberOfDimensions; axis++ )
		    orphanPoint(n,axis)=center(j1,j2,j3,axis);
		}
		else
		{
		  for( int axis=0; axis<numberOfDimensions; axis++ )
		    orphanPoint(n,axis)=XC(jv,axis);
		}
			  
		orphanPoint(n,numberOfDimensions)=grid;
		numberOfOrphanPoints++;


		ia(numberOfErrors,0)=i1;
		ia(numberOfErrors,1)=i2;
		ia(numberOfErrors,2)=i3;
		numberOfErrors++;
		if( numberOfErrors>=maximumNumberOfErrors )
		{
		  maximumNumberOfErrors*=2;
		  ia.resize(maximumNumberOfErrors,3);
		}
	      }

	    } // end for( i )
	    
	  }
	  
	} // end if( totalNumToCheck > 0 )
	
	
	int totalNumberFixed=ParallelUtility::getSum(numberFixed);

	if( totalNumberFixed==0 )
	{
	  cuttingIsValid=true;
	}
	else
	{
	  // if some points were fixed we may need to double check points again and recompute the invalid points
	  if (info & 2 )
	  {
	    printF("checkHoleCutting:: recheck hole cutting on grid %s...\n",(const char*)g.getName());
	    fprintf(plogFile,"checkHoleCutting:: recheck hole cutting on grid %s...\n",(const char*)g.getName());
	  }
	  numberOfErrors=numberOfErrorsAtStart;   // reset
	}
	
      } // end for( it ...
      if( it==maximumNumberOfIterations )
      {
	printF("checkHoleCutting::WARNING: maximumNumberOfIterations=%i exceeded for grid %s! Hole cutting may "
	       "be wrong\n",maximumNumberOfIterations,(const char*)g.getName());
      }
      
      if( info & 2 )
	fflush(plogFile);
		  
    }
    else
    {
      // -- old way --

      bool cuttingIsValid=false;
      const int maximumNumberOfIterations=10;  // what should this be?
      int numberOfWarnings=0;
      const int maxNumberOfWarnings=50;
      // --- We may have to iterate a few times to fix points -- fixing some point may then require new
      //     points to be fixed.
      int it;
      for( it=0; it<maximumNumberOfIterations && !cuttingIsValid; it++ ) 
      {
	int numberFixed=0;
	const int numberOfErrorsAtStart=numberOfErrors;

	// -- First make a list of suspicious points ---

	int numToCheck=0;
	if( ok )
	{
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  {
	    for( i2=I2.getBase(); i2<=I2Bound; i2++ )
	    {
	      for( i1=I1Base; i1<=I1Bound; i1++ )
	      {
		if( MASK(i1,i2,i3)==0 )
		{
		  j3=i3;
		  const int j1a=max(i1-1,id1a), j1b=min(i1+1,id1b); // *wdh* 031130
		  const int j2a=max(i2-1,id2a), j2b=min(i2+1,id2b);
		  if( numberOfDimensions==2 )
		  {
		    for( j2=j2a; j2<=j2b; j2++ )
		    {
		      int j2m=max(j2-1,id2a), j2p=min(j2+1,id2b); // *wdh* 040912
		      for( j1=j1a; j1<=j1b; j1++ )
		      {
			int j1m=max(j1-1,id1a), j1p=min(j1+1,id1b);  // *wdh* 040912
	      
			// ***NOTE*** we could still miss an error where holes points were marked
			//            but no interpolation points!  (see cicThin.cmd)
			if( MASK(j1,j2,j3)>0 &&(
			      MASK(j1m,j2m,j3)<0 || MASK(j1 ,j2m,j3)<0 || MASK(j1p,j2m,j3)<0 || 
			      MASK(j1m,j2 ,j3)<0 || MASK(j1 ,j2 ,j3)<0 || MASK(j1p,j2 ,j3)<0 || 
			      MASK(j1m,j2p,j3)<0 || MASK(j1 ,j2p,j3)<0 || MASK(j1p,j2p,j3)<0 ) )
			{
			  // Check here if point (j1,j2,j3) can interpolate from another grid.
			  // (There could be a finer grid in this area that is meant to interpolate. cf. cicThin.cmd)

			  // *wdh* 081028 -- only check pts that are also next to hole points
			  bool neighbouringHolePoints =
			    MASK(j1m,j2m,j3)<0 || MASK(j1 ,j2m,j3)<0 || MASK(j1p,j2m,j3)<0 || 
			    MASK(j1m,j2 ,j3)<0 || MASK(j1 ,j2 ,j3)<0 || MASK(j1p,j2 ,j3)<0 || 
			    MASK(j1m,j2p,j3)<0 || MASK(j1 ,j2p,j3)<0 || MASK(j1p,j2p,j3)<0 ;
			
			  if( !neighbouringHolePoints ) continue;

			  // try to interpolate from another grid
			  int infoLevel=0;
			  bool checkBoundaryPoint=false;
			  bool checkInterpolationCoords=false;
			  bool interpolatePoint=true;
			  bool ok=interpolateAPoint(cg, grid, jv, interpolatePoint, checkInterpolationCoords, 
						    checkBoundaryPoint, infoLevel );

			  if( ok )
			  {
			    numberFixed++;
			    if( info & 2 )
			    {
			      fprintf(plogFile,"checkHoleCutting:INFO(3): suspicious discr. pt (%i,%i) on grid %i (%s) "
				      "near the hole pt (%i,%i) has been interpolated\n",
				      j1,j2,grid,(const char*)g.getName(),i1,i2);
			      if( numberFixed<20 )
				printf("checkHoleCutting:INFO(3): suspicious discr. pt (%i,%i) on grid %i (%s) near the "
				       "hole pt (%i,%i) has been interpolated\n",j1,j2,grid,(const char*)g.getName(),i1,i2);
			      else if( numberFixed==20 )
				printf("checkHoleCutting:INFO(3): suspicious discr. pt: I am not printing any "
				       "more of these messages. See ogen.log\n");
			  
			    }
			
			  }
			  else
			  {
			    if( debug & 1 && numberOfWarnings<maxNumberOfWarnings )
			    {
			      printf("***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt and a discr. pt (%i,%i,%i)"
				     " grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,(const char*)g.getName());
			      if( numberOfWarnings==maxNumberOfWarnings-1 )
				printf("***INFO*** See log file for further warnings of `hole pt next to interp pt..'\n");
			    }
			    numberOfWarnings++;

			    fprintf(logFile,"***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt and a discr. pt (%i,%i,%i)"
				    " grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,(const char*)g.getName());

       
			    if( numberOfOrphanPoints >= orphanPoint.getLength(0) )
			    {
			      int num=numberOfOrphanPoints*2+10;
			      orphanPoint.resize(num,orphanPoint.getLength(1));
			    }
			    const int n=numberOfOrphanPoints;
			    if( !isRectangular )
			    {
			      for( int axis=0; axis<numberOfDimensions; axis++ )
				orphanPoint(n,axis)=center(j1,j2,j3,axis);
			    }
			    else
			    {
			      for( int axis=0; axis<numberOfDimensions; axis++ )
				orphanPoint(n,axis)=XC(jv,axis);
			    }
			
			    orphanPoint(n,numberOfDimensions)=grid;
			    numberOfOrphanPoints++;
		      

			    ia(numberOfErrors,0)=i1;
			    ia(numberOfErrors,1)=i2;
			    numberOfErrors++; 
			    if( numberOfErrors>=maximumNumberOfErrors )
			    {
			      maximumNumberOfErrors*=2;
			      ia.resize(maximumNumberOfErrors,3);
			    }
			    // mask(i1,i2,i3)=MappedGrid::ISdiscretizationPoint;
			  }
		    
			}
		      }
		    }
		  }
		  else
		  {
		    const int j3a=max(i3-1,id3a), j3b=min(i3+1,id3b);
		    for( j3=j3a; j3<=j3b; j3++ )
		    {
		      int j3m=max(j3-1,id3a), j3p=min(j3+1,id3b); // *wdh* 040912
		      for( j2=j2a; j2<=j2b; j2++ )
		      {
			int j2m=max(j2-1,id2a), j2p=min(j2+1,id2b); // *wdh* 040912
			for( j1=j1a; j1<=j1b; j1++ )
			{
			  int j1m=max(j1-1,id1a), j1p=min(j1+1,id1b); // *wdh* 040912

			  if( MASK(j1,j2,j3)>0 &&(
				MASK(j1m,j2m,j3m)<0 || MASK(j1 ,j2m,j3m)<0 || MASK(j1p,j2m,j3m)<0 || 
				MASK(j1m,j2 ,j3m)<0 || MASK(j1 ,j2 ,j3m)<0 || MASK(j1p,j2 ,j3m)<0 || 
				MASK(j1m,j2p,j3m)<0 || MASK(j1 ,j2p,j3m)<0 || MASK(j1p,j2p,j3m)<0 || 
				MASK(j1m,j2m,j3 )<0 || MASK(j1 ,j2m,j3 )<0 || MASK(j1p,j2m,j3 )<0 || 
				MASK(j1m,j2 ,j3 )<0 || MASK(j1 ,j2 ,j3 )<0 || MASK(j1p,j2 ,j3 )<0 || 
				MASK(j1m,j2p,j3 )<0 || MASK(j1 ,j2p,j3 )<0 || MASK(j1p,j2p,j3 )<0 || 
				MASK(j1m,j2m,j3p)<0 || MASK(j1 ,j2m,j3p)<0 || MASK(j1p,j2m,j3p)<0 || 
				MASK(j1m,j2 ,j3p)<0 || MASK(j1 ,j2 ,j3p)<0 || MASK(j1p,j2 ,j3p)<0 || 
				MASK(j1m,j2p,j3p)<0 || MASK(j1 ,j2p,j3p)<0 || MASK(j1p,j2p,j3p)<0 
				) )
			  {
			    // *wdh* 081028 -- only check pts that are also next to hole points
			    bool neighbouringHolePoints =
			      MASK(j1m,j2m,j3m)==0 || MASK(j1 ,j2m,j3m)==0 || MASK(j1p,j2m,j3m)==0 || 
			      MASK(j1m,j2 ,j3m)==0 || MASK(j1 ,j2 ,j3m)==0 || MASK(j1p,j2 ,j3m)==0 || 
			      MASK(j1m,j2p,j3m)==0 || MASK(j1 ,j2p,j3m)==0 || MASK(j1p,j2p,j3m)==0 || 
			      MASK(j1m,j2m,j3 )==0 || MASK(j1 ,j2m,j3 )==0 || MASK(j1p,j2m,j3 )==0 || 
			      MASK(j1m,j2 ,j3 )==0 || MASK(j1 ,j2 ,j3 )==0 || MASK(j1p,j2 ,j3 )==0 || 
			      MASK(j1m,j2p,j3 )==0 || MASK(j1 ,j2p,j3 )==0 || MASK(j1p,j2p,j3 )==0 || 
			      MASK(j1m,j2m,j3p)==0 || MASK(j1 ,j2m,j3p)==0 || MASK(j1p,j2m,j3p)==0 || 
			      MASK(j1m,j2 ,j3p)==0 || MASK(j1 ,j2 ,j3p)==0 || MASK(j1p,j2 ,j3p)==0 || 
			      MASK(j1m,j2p,j3p)==0 || MASK(j1 ,j2p,j3p)==0 || MASK(j1p,j2p,j3p)==0;
			
			    if( !neighbouringHolePoints ) continue;
			
			    // try to interpolate from another grid
			    int infoLevel=0;
			    bool checkBoundaryPoint=false;
			    bool checkInterpolationCoords=false;
			    bool interpolatePoint=true;
			    bool ok=interpolateAPoint(cg, grid, jv, interpolatePoint, checkInterpolationCoords, 
						      checkBoundaryPoint, infoLevel );

			    if( ok )
			    {
			      numberFixed++;
			      if( info & 2 )
			      {
				fprintf(plogFile,"checkHoleCutting:INFO(4): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
					"near the hole pt (%i,%i,%i) has been interpolated\n",
					j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
				if( numberFixed<20 )
				  printf("checkHoleCutting:INFO(4): suspicious discr. pt (%i,%i,%i) on grid %i (%s) "
					 "near the hole pt (%i,%i,%i) has been interpolated\n",
					 j1,j2,j3,grid,(const char*)g.getName(),i1,i2,i3);
				else if( numberFixed==20 )
				  printf("checkHoleCutting:INFO(4): suspicious discr. pt: I am not printing any "
					 "more of these messages. See ogen.log\n");
			  
			      }
			    }
			    else
			    {
			      if( debug & 1 && numberOfWarnings<maxNumberOfWarnings )
			      {
				printf("***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt and a discr. pt (%i,%i,%i)"
				       " grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,(const char*)g.getName());
				if( numberOfWarnings==maxNumberOfWarnings-1 )
				  printf("***INFO*** See log file for further warnings of `hole pt next to interp pt..'\n");
			      }
			      numberOfWarnings++;
			      if( debug & 1 )
				fprintf(logFile,"***WARNING*** hole pt (%i,%i,%i) is next to an interp. pt "
					"and a discr. pt (%i,%i,%i) grid %i : %s\n",i1,i2,i3,j1,j2,j3,grid,
					(const char*)g.getName());

			      if( numberOfOrphanPoints >= orphanPoint.getLength(0) )
			      {
				int num=numberOfOrphanPoints*2+10;
				orphanPoint.resize(num,orphanPoint.getLength(1));
			      }
			      const int n=numberOfOrphanPoints;
			      if( !isRectangular )
			      {
				for( int axis=0; axis<numberOfDimensions; axis++ )
				  orphanPoint(n,axis)=center(j1,j2,j3,axis);
			      }
			      else
			      {
				for( int axis=0; axis<numberOfDimensions; axis++ )
				  orphanPoint(n,axis)=XC(jv,axis);
			      }
			  
			      orphanPoint(n,numberOfDimensions)=grid;
			      numberOfOrphanPoints++;


			      ia(numberOfErrors,0)=i1;
			      ia(numberOfErrors,1)=i2;
			      ia(numberOfErrors,2)=i3;
			      numberOfErrors++;
			      if( numberOfErrors>=maximumNumberOfErrors )
			      {
				maximumNumberOfErrors*=2;
				ia.resize(maximumNumberOfErrors,3);
			      }
			    }
			    // mask(i1,i2,i3)=MappedGrid::ISdiscretizationPoint;
		      
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  } // end for i3
	} // end if( ok )
	
	if( numberFixed==0 )
	{
	  cuttingIsValid=true;
	}
	else
	{
	  // if some points were fixed we may need to double check points again and recompute the invalid points
	  if (info & 2 )
	    printf("checkHoleCutting:: recheck hole cutting on grid %s...\n",(const char*)g.getName());
	  numberOfErrors=numberOfErrorsAtStart;   // reset
	}
	
      } // end for( it ...
      if( it==maximumNumberOfIterations )
      {
	printf("checkHoleCutting::WARNING: maximumNumberOfIterations=%i exceeded for grid %s! Hole cutting may "
	       "be wrong\n",maximumNumberOfIterations,(const char*)g.getName());
      }
    } // end old way
    
  }  // end for grid
  
  
  if( numberOfErrors>0 )
  {
    if( numberOfCheckHoleCuttingWarnings<5 )
    {
      numberOfCheckHoleCuttingWarnings++;
      printf("Ogen:WARNING: check hole cutting: there were %i potential problem points found in the hole cutting\n"
	     "These points may cause failure of the algorithm. This warning may be generated if there is a portion\n"
	     "of physical boundary that extends outside the actual domain in which case the algorithm should work\n"
	     "Check the log file ogen.log for further info.\n",
	     numberOfErrors);
      printf("\nINFO: Any suspicious discretization points that are too close to hole points will be plotted\n"
	     "      as `orphan points' (large squares). These points should probably be interpolation points.\n");
    }
    else if( numberOfCheckHoleCuttingWarnings==5 )
    {
      numberOfCheckHoleCuttingWarnings++;
      printf("Ogen:INFO: too many check hole cutting warnings. I will not print anymore\n");
      
    }
    
//      fprintf(checkFile,"WARNING: check hole cutting: there were %i potential problem points found in the hole cutting\n",
//             numberOfErrors);
  }
  else
  {
    if( info & 2 )
      printF("INFO: check hole cutting: holes appear to be cut correctly\n");
  }
  real time=getCPU();
  timeCheckHoleCutting=time-time0;

  return 0;
}
#undef MASK


int Ogen::
cutHoles(CompositeGrid & cg)
// =======================================================================================================
//
// /Description:
// For each physical boundary of each grid:
//     Find all points on other grids that are outside the boundary.
//
// Note: for a cell-centred grid we still use the vertex boundary values to cut holes.
//
// =======================================================================================================
{
  real time0=getCPU();
//  info |= 4;

  // When we cut holes, for each cutter point we form a region on the cuttee grid of points
  // to check. The maximum with of this region is the maxiumHoleWidth.
  const int maximumHoleWidth=10;  // 5 

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  // We plot any suspicious discretization points that are too close to hole points as 'orphan' points
  numberOfOrphanPoints=0;
  plotOrphanPoints.redim(numberOfBaseGrids);
  plotOrphanPoints=2;  // colour orphan pts by grid number
  orphanPoint.redim(100,numberOfDimensions+1);

  if( numberOfBaseGrids==1 ) return 0;

  Range G(0,numberOfBaseGrids-1);
  const int maxNumberCutting= max(cg.mayCutHoles(G,G));
  if( maxNumberCutting==0 && numberOfManualHoles==0 )
  {
    return 0;
  }
  

  if( info & 4 ) printf("cutting holes with physical boundaries...\n");

  bool vectorize=TRUE;

  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R, R1, Rx(0,numberOfDimensions-1);
  realArray x,r,rr;
  realArray x2(1,3), r2(1,3);
  intArray ia,ia2;
  
  const real maxDistFactor=SQR(2.); // cut holes within 2*( cell diagonal length of cutee grid )


  int  iv[3], &i1 = iv[0],  &i2= iv[1], &i3 = iv[2];
  int  jv[3], &j1 = jv[0],  &j2= jv[1], &j3 = jv[2];
  int ipv[3], &i1p=ipv[0], &i2p=ipv[1], &i3p=ipv[2];
  int jpv[3], &j1p=jpv[0], &j2p=jpv[1], &j3p=jpv[2];

  int maxNumberOfHolePoints=10000*numberOfBaseGrids*numberOfDimensions;  // **** fix this 
  numberOfHolePoints=0;
  holePoint.redim(maxNumberOfHolePoints,numberOfDimensions+1);
  
  // const real boundaryAngleEps=.01;
  // const real boundaryNormEps=0.; // *wdh* 980126 ** not needed since we double check//  1.e-2;
  IntegerArray holeOffset(numberOfDimensions);
  IntegerArray holeMarker(3);
  real rv[3]={0.,0.,0.};

  const real biggerBoundaryEps = sqrt(boundaryEps);


  // *****************************************************************************
  //   cutShare[grid](i1,i2,i3) : when a physical boundary cuts a hole in another grid
  //         we keep track of the share value of the cutter grid so that we can prevent
  //         shared boundaries from cutting holes where they shouldn't. Normally shared
  //         boundaries do not cut holes so this flag is not needed. It is needed for
  //         fillet/collar type grids when the use has specified that share boundaries
  //         may cut holes.
  // *****************************************************************************

  int grid,dir; 
  intArray *cutShare= new intArray [numberOfBaseGrids];
  
  int maximumSharedBoundaryFlag=0;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    cutShare[grid].redim(cg[grid].mask());
    cutShare[grid]=0;
    maximumSharedBoundaryFlag=max(maximumSharedBoundaryFlag,max(cg[grid].sharedBoundaryFlag()));
  }
  
  // ********************************************************
  // *** Mark non-cutting portions of physical boundaries ***
  // ********************************************************
  int n;
  for( n=0; n<numberOfNonCuttingBoundaries; n++ )
  {
      int grid=nonCuttingBoundaryPoints(n,0);
      int i1a=nonCuttingBoundaryPoints(n,1);
      int i1b=nonCuttingBoundaryPoints(n,2);
      int i2a=nonCuttingBoundaryPoints(n,3);
      int i2b=nonCuttingBoundaryPoints(n,4);
      int i3a=nonCuttingBoundaryPoints(n,5);
      int i3b=nonCuttingBoundaryPoints(n,6);

      Index I1=Range(i1a,i1b);
      Index I2=Range(i2a,i2b);
      Index I3=Range(i3a,i3b);
      MappedGrid & mg = cg[grid];
      intArray & mask = mg.mask();
      where( mask(I1,I2,I3)!=0 )
      {
	mask(I1,I2,I3) |= ISnonCuttingBoundaryPoint;
      }
  }
  


  // *************************
  // *** Cut manual holes  ***
  // *************************
  if( numberOfManualHoles>0 )
  {
    for( int hole=0; hole<numberOfManualHoles; hole++ )
    {
      int grid=manualHole(hole,0);
      int i1a=manualHole(hole,1);
      int i1b=manualHole(hole,2);
      int i2a=manualHole(hole,3);
      int i2b=manualHole(hole,4);
      int i3a=manualHole(hole,5);
      int i3b=manualHole(hole,6);
      
      printf(" Cut the manual hole [%i,%i]x[%i,%i]x[%i,%i] in grid %s\n",
	     i1a,i1b,i2a,i2b,i3a,i3b, (const char*)cg[grid].getName() );

      assert( grid>=0 && grid<numberOfBaseGrids );
      
      MappedGrid & g = cg[grid];
      intArray & mask = g.mask();
      const realArray & vertex = g.vertex();

      const bool isRectangular = g.isRectangular();
      real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
      int iv0[3]={0,0,0}; //
      if( isRectangular )
      {
	g.getRectangularGridParameters( dvx, xab );
	for( int dir=0; dir<g.numberOfDimensions(); dir++ )
	  iv0[dir]=g.gridIndexRange(0,dir);
      }
      #define XV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

      I1=Range(i1a,i1b);
      I2=Range(i2a,i2b);
      I3=Range(i3a,i3b);

      mask(I1,I2,I3)=0;
      
      int numHoles=(i1b-i1a+1)*(i2b-i2a+1)*(i3b-i3a+1);
      if( numberOfHolePoints+numHoles>= maxNumberOfHolePoints )
      {
	maxNumberOfHolePoints*=2; 
        maxNumberOfHolePoints+=numHoles;
	holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
	printf(" ... increasing maxNumberOfHolePoints to %i\n",maxNumberOfHolePoints);
      }
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      for( i3=i3a; i3<=i3b; i3++ )
      {
	for( i2=i2a; i2<=i2b; i2++ )
	{
	  for( i1=i1a; i1<=i1b; i1++ )
	  {
            if( !isRectangular )
	    {
	      for( int axis=axis1; axis<numberOfDimensions; axis++ )
		holePoint(numberOfHolePoints,axis)=vertex(i1,i2,i3,axis);
	    }
	    else
	    {
	      for( int axis=axis1; axis<numberOfDimensions; axis++ )
		holePoint(numberOfHolePoints,axis)=XV(iv,axis);
	    }
	    
            holePoint(numberOfHolePoints,numberOfDimensions)=grid;
	    numberOfHolePoints++;
	  }
	}
      }
    }
  }  
  
  if( maxNumberCutting==0 )
  {
    return numberOfHolePoints;
  }

  // **********************************************************************
  //     cut holes with highest priority grids first since these will 
  //     provide the first interpolation points
  // **********************************************************************
  for( grid=numberOfBaseGrids-1; grid>=0; grid-- )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const realArray & vertex = g.vertex();
    const realArray & xr = g.vertexDerivative();
    intArray & inverseGrid1 = cg.inverseGrid[grid];
    const intArray & mask = g.mask();
    
    // shift this offset by epsilon to make sure we check the correct points in the k1,k2,k3 loop.
    const real cellCenterOffset= g.isAllCellCentered() ? .5-.5 : -.5;   // add -.5 to round to nearest point

    const bool isRectangular = g.isRectangular();
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      g.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<g.numberOfDimensions(); dir++ )
	iv0[dir]=g.gridIndexRange(0,dir);
    }
    #define XV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


    if( debug & 1 || info & 4 )
    {
      printf("cutting holes with grid: %s ...\n",(const char*)g.getName());
      fprintf(logFile,"cutting holes with grid: %s ...\n",(const char*)g.getName());
    }
    
    for( int axis=axis1; axis<numberOfDimensions; axis++ )
    {
      // axisp1 : must equal the most rapidly varying loop index of the triple (i1,i2,i3) loop below
      //          since we only save the holeWidth for the previous line.
      const int axisp1 = axis!=axis1 ? axis1 : axis2;  // we must make this axis1 if possible, otherwise axis2
      const int axisp2 = numberOfDimensions==2 ? axisp1 : (axis!=axis2 && axisp1!=axis2) ? axis2 : axis3;
      
      for( int side=Start; side<=End; side++ )
      {
        // Note: do not cut holes with singular sides
        if( g.boundaryCondition(side,axis) > 0 && 
            map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity ) 
	{
	  // this side is a physical boundary
	  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);   // note: use gridIndexRange

          real boundaryEpsilon=boundaryEps;
          if( debug & 2 )
            printf(" cg.maximumHoleCuttingDistance(%i,%i,%i)=%e\n",
                  side,axis,grid,cg.maximumHoleCuttingDistance(side,axis,grid));
	  
          const real maximumHoleCuttingDistanceSquared=SQR(cg.maximumHoleCuttingDistance(side,axis,grid));
          if( debug & 4 )
	    printf(" (side,axis,grid)=(%i,%i,%i) maximumHoleCuttingDistance=%e\n",
		   side,axis,grid,cg.maximumHoleCuttingDistance(side,axis,grid));

//           if( FALSE && g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
// 	  {
// 	    // choose boundaryEps to match tolerance on c-grid since we don't want to cut holes
//             //  inside the c-grid
// 	    for( int n=0; n<numberOfMixedBoundaries; n++ )
// 	    {
// 	      if( mixedBoundary(n,0)==grid && side==mixedBoundary(n,1) && axis==mixedBoundary(n,2))
// 	      {
// 		boundaryEpsilon=mixedBoundaryValue(n,0);
//                 assert( boundaryEpsilon>=0. );
// 	      }
// 	    }
// 	  }
	  

          Range R1(0,I1.length()*I2.length()*I3.length()-1);

	  // no: getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // note: use gridIndexRange
          bool firstTimeForThisBoundary=TRUE;

          // ************************************************************************************************
          // *** share : when a point on grid2 interp's from grid, we mark cutShare[grid2](i1,i2,i3)=share
          //             so we can prevent other boundaries with the same value for share from cutting
          // *** next line assume only positive share values ***
          // ************************************************************************************************
	  const int share=g.sharedBoundaryFlag(side,axis)>0 ? g.sharedBoundaryFlag(side,axis) : 
	    maximumSharedBoundaryFlag+grid+1 ; // a unique (bogus) value for this grid.
	  
          const RealDistributedArray & normal = g.vertexBoundaryNormal(side,axis); 


          // **********************************
          //     Cut Holes in other grids  
          // **********************************
          for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
	  {
            MappedGrid & g2 = cg[grid2];
	    if( grid2!=grid && 
                // *wdh* 990504 cg.mayCutHoles(grid,grid2) &&   // we need to check for interp points in this case
		(cg.mayCutHoles(grid,grid2) || cg.mayInterpolate(grid,grid2,0)) && // *wdh* 020127
                (isNew(grid) || isNew(grid2))
                &&  map.intersects( g2.mapping().getMapping(), side,axis,-1,-1,.1 ) )
	    {

              bool mayCutHoles = cg.mayCutHoles(grid,grid2);
              const bool phantomHoleCutting=cg.mayCutHoles(grid,grid2)==2;
	      
              if( debug & 4 )
		printf(" (side,axis,grid)=(%i,%i,%i) try to cut holes in grid2=%i (%s)\n",
		       side,axis,grid,grid2,(const char*)g2.getName());
		
              // printf(" (side,axis,grid)=(%i,%i,%i) mayCutHoles=%i grid2=%i sharedSidesMayCutHoles=%i\n",
	      //     side,axis,grid,mayCutHoles,grid2,cg.sharedSidesMayCutHoles(grid,grid2));
	      
              if( mayCutHoles && !cg.sharedSidesMayCutHoles(grid,grid2) && !phantomHoleCutting )
	      {
		// shared sides should not cut holes
                const int shareFlag=g.sharedBoundaryFlag(side,axis);
                // printf(" (side,axis,grid)=(%i,%i,%i) share=%i grid2=%i\n",
		//       side,axis,grid,shareFlag,grid2);
		
		if( shareFlag!=0 && min(abs(g2.sharedBoundaryFlag()-shareFlag))==0 )
		{
		  // grid2 has the same share flag as this face of grid.
                  mayCutHoles=FALSE;
                  if( debug & 2 )
  		    printf("*** hole cutting prevented for (side,axis,grid)=(%i,%i,%i) on grid2=%i since"
                           " grids share a boundary.\n",side,axis,grid,grid2);
		}
	      }
	      
              if( firstTimeForThisBoundary )
	      {
		firstTimeForThisBoundary=FALSE;
		r.redim(I1,I2,I3,Rx);
		rr.redim(I1.length()*I2.length()*I3.length(),Rx); rr=-1.;
		x.redim(I1.length()*I2.length()*I3.length(),Rx);
                ia.redim(I1.length()*I2.length()*I3.length(),7);  
	      }

              const bool isRectangular2 = g2.isRectangular();
	      real dvx2[3]={1.,1.,1.}, xab2[2][3]={{0.,0.,0.},{0.,0.,0.}};
              int iv20[3]={0,0,0}; //
	      if( isRectangular2 )
	      {
		g2.getRectangularGridParameters( dvx2, xab2 );
                for( int dir=0; dir<numberOfDimensions; dir++ )
		{
		  iv20[dir]=g2.gridIndexRange(0,dir);
                  if( g2.isAllCellCentered() )
		  {
                    xab2[0][dir]+=.5*dvx2[dir];  // offset for cell centered
		  }
		}
		
	      }
              #undef XC2
              #define XC2(iv,axis) (xab2[0][axis]+dvx2[axis]*(iv[axis]-iv20[axis]))

	      realArray & center2 = g2.center();
              intArray & mask2 = g2.mask();
              const IntegerArray & indexRange2 = g2.indexRange();
              intArray & inverseGrid = cg.inverseGrid[grid2];
	      realArray & rI = cg.inverseCoordinates[grid2];

              const IntegerArray & extendedIndexRange2 = g2.extendedIndexRange();
              // IntegerArray extendedIndexRange2 = g2.extendedRange();

              bool isPeriodic2[3]={false,false,false}; // function periodic for grid2
              for( dir=0; dir<numberOfDimensions; dir++ )
                isPeriodic2[dir]=g2.isPeriodic(dir)==Mapping::functionPeriodic;

	      intArray & cutShare2 = cutShare[grid2];
              intArray cut;
	      cut=cutShare2;   // ** make a copy ** why?
	      
	      
              // ***************************************************************************
              //   Make a list of points on grid in the bounding box of grid2
              // no need to cut holes with points that already interpolate from this grid!
              // ***************************************************************************

              RealArray boundingBox;
              boundingBox=g2.mapping().getMapping().getBoundingBox();    //   *** note: ghost lines not included
              real delta = .2*max( boundingBox(End,Rx)-boundingBox(Start,Rx) );
              for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		boundingBox(Start,dir)-=delta;
		boundingBox(End  ,dir)+=delta;
	      }

              intArray cutMask(I1,I2,I3);
              // cutMask(i1,i2,i3)==1 : if point (i1,i2,i3) is inside the bounding box of grid2
              if( !isRectangular )
	      {
		if( numberOfDimensions==2 )
		  cutMask=(vertex(I1,I2,I3,axis1)>boundingBox(0,axis1) && vertex(I1,I2,I3,axis1)<boundingBox(1,axis1)&&
			   vertex(I1,I2,I3,axis2)>boundingBox(0,axis2) && vertex(I1,I2,I3,axis2)<boundingBox(1,axis2));
		// && 			  inverseGrid1(I1,I2,I3)!=grid2);
		else
		  cutMask=(vertex(I1,I2,I3,axis1)>boundingBox(0,axis1) && vertex(I1,I2,I3,axis1)<boundingBox(1,axis1)&&
			   vertex(I1,I2,I3,axis2)>boundingBox(0,axis2) && vertex(I1,I2,I3,axis2)<boundingBox(1,axis2)&&
			   vertex(I1,I2,I3,axis3)>boundingBox(0,axis3) && vertex(I1,I2,I3,axis3)<boundingBox(1,axis3));
		// && inverseGrid1(I1,I2,I3)!=grid2);
	      }
	      else
	      {
		if( numberOfDimensions==2 )
		{
                  FOR_3D(i1,i2,i3,I1,I2,I3)
		  {
		    cutMask(i1,i2,i3)=(XV(iv,axis1)>boundingBox(0,axis1) && XV(iv,axis1)<boundingBox(1,axis1)&&
			               XV(iv,axis2)>boundingBox(0,axis2) && XV(iv,axis2)<boundingBox(1,axis2));
		  }
		}
		else
		{
                  FOR_3D(i1,i2,i3,I1,I2,I3)
		  {
		    cutMask(i1,i2,i3)=(XV(iv,axis1)>boundingBox(0,axis1) && XV(iv,axis1)<boundingBox(1,axis1)&&
				       XV(iv,axis2)>boundingBox(0,axis2) && XV(iv,axis2)<boundingBox(1,axis2)&&
				       XV(iv,axis3)>boundingBox(0,axis3) && XV(iv,axis3)<boundingBox(1,axis3));
		  }
		}
	      }
	      

	      if( map.getTopology(side,axis)==Mapping::topologyIsPartiallyPeriodic )
	      {
                printf("grid=%s (side,axis)=(%i,%i) is a c-grid side\n",(const char*)g.getName(),side,axis);
		
		cutMask=cutMask && map.topologyMask()(I1,I2,I3)==0;
	      }
//               if( FALSE && numberOfMixedBoundaries>0  ) // fix this ***************
// 	      {
//                 // do not cut holes with interpolation parts of mixed boundaries
// 		// **wdh*  990927 cutMask=cutMask && !(mask(I1,I2,I3) & MappedGrid::ISinteriorBoundaryPoint);
// 		cutMask=cutMask && !(mask(I1,I2,I3) & ISnonCuttingBoundaryPoint);
// 	      }
	      
              int i=0;
              const int I1Base=I1.getBase(), I1Bound=I1.getBound();
              const int I2Base=I2.getBase(), I2Bound=I2.getBound();
              const int I3Base=I3.getBase(), I3Bound=I3.getBound();
              if( true ) // **** 060318 **** do this 
	      {
		for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		{
		  for( i2=I2.getBase(); i2<=I2Bound; i2++ )
		  {
		    for( i1=I1Base; i1<=I1Bound; i1++ )
		    {
		      if( cutMask(i1,i2,i3) )
		      {
                        // ***** ia(i,.) : list of potential cutting points ********
			// for( dir=0; dir<numberOfDimensions; dir++ )
			//  x(i,dir)=vertex(i1,i2,i3,dir);
			ia(i,0)=i1;
			ia(i,1)=i2;
			ia(i,2)=i3;
			i++;
		      }
		    }
		  }
		}
	      }
	      else
	      {
                const intArray & cm = cutMask.indexMap();  // *** this has a leak I think ****
		i=cm.getLength(0);
		if( i>0 )
		{
		  R1=Range(0,i-1);
                  Range R2=cm.dimension(1);
		  ia(R1,R2)=cm;
                  if( R2.getBound()<1 ) ia(R1,1)=I2Base;
                  if( R2.getBound()<2 ) ia(R1,2)=I3Base;
		}
              }
              if( i==0 )
                continue;
	      int numberToCheck=i;
	      
              R1=Range(0,numberToCheck-1);

              if( !isRectangular )
	      {
		for( dir=0; dir<numberOfDimensions; dir++ )
		  x(R1,dir)=vertex(ia(R1,0),ia(R1,1),ia(R1,2),dir);
	      }
	      else
	      {
		for( int i=R1.getBase(); i<=R1.getBound(); i++ )
		{
		  iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
		  for( dir=0; dir<numberOfDimensions; dir++ )
		    x(i,dir)=XV(iv,dir);
		}
	      }
	      
              real time1=getCPU();
              // adjust boundary points on shared sides *** x is changed **
	      if( useBoundaryAdjustment )
		adjustBoundary(cg,grid,grid2,ia(R1,Rx),x(R1,Rx));   

              g2.mapping().getMapping().inverseMap(x(R1,Rx),rr);

              r=Mapping::bogus;
              for( dir=0; dir<numberOfDimensions; dir++ )
  	        r(ia(R1,0),ia(R1,1),ia(R1,2),dir)=rr(R1,dir);
	      
              real time2=getCPU();

              if( debug & 1 || info & 4 ) 
                   fprintf(logFile,"grid=%s: cut with (side,axis)=(%i,%i) :\n time for inverseMap grid2=%s, "
						 "is %e (total=%e) (number of pts=%i)\n",
                     (const char*)g.mapping().getName(Mapping::mappingName),
                     side,axis,(const char*)g2.mapping().getName(Mapping::mappingName),time2-time1,time2-totalTime,
                     numberToCheck);
	      
	      for( dir=numberOfDimensions; dir<3; dir++ )
	      {
		jv[dir]=indexRange2(Start,dir);   // give default values 
		jpv[dir]=jv[dir];
	      }
              // we need to save the old boxes along the first tangential direction, axisp1
              Range It=Range(Iv[axisp1].getBase()-1,Iv[axisp1].getBound());
              IntegerArray holeCenter(Range(0,2),It), holeWidth(Range(0,2),It);
              holeWidth=-1;
	      
  	      for( dir=0; dir<3; dir++ )
	        holeCenter(dir,It)=indexRange2(Start,dir)-100; // bogus value means not a valid hole.

              const int indexRange00 = numberOfDimensions==2 ? indexRange2(Start,axisp1) :
		min( indexRange2(Start,axisp1),indexRange2(Start,axisp2));

              int numberCut=0;
	      
              // ********************************************************************************************
              // Compute the holeMask:
              //     holeMask(i1,i2,i3) = 0 : point is outside and not invertible
              //                        = 1 : point is inside
              //                        = 2 : point is outside but invertible
              //
              //                      -------------------------
              //                      |                       |      
              //                      |        grid2          |      
              //    holeMask          |                       |      
              //      --0---0---2---2---1---1---1---1---1---1---2---2---2---0---0---- cutting curve, grid
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      |                       |      
              //                      -------------------------
              // ********************************************************************************************


              intArray holeMask(I1,I2,I3);
	      holeMask=0;
              real dr=0.;
              // loop over all points on the face that is cutting a hole
              for( i=0; i<numberToCheck; i++ )
	      {
		i1=ia(i,0);
		i2=ia(i,1);
		i3=ia(i,2);
		
		i1p=i1<I1Bound ? i1+1 : i1>I1Base ? i1-1 : i1;
		i2p=i2<I2Bound ? i2+1 : i2>I2Base ? i2-1 : i2;
		i3p=i3<I3Bound ? i3+1 : i3>I3Base ? i3-1 : i3;

		// ** int ib = iv[axisp1];  // tangential marching direction
		// We need to include as 'inside' points that are close to the boundary
		// This is so we catch 'corners' that are cut off. Base the distance we
		// need to check on the tangential distance between cutting points.
		//               X
		//         o--o-/--o
		//         |  /
		//         |/
		//        /o 
		//      X  | 
		// avoid bogus points,
                dr=0.;
		if( numberOfDimensions==2 )
		{
		  if( fabs(rr(i,axisp1)-.5)<.6 && fabs(r(i1p,i2p,i3p,axisp1)-.5)<.6 ) // watch out for periodic bndry's
		  {
		    dr=fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1));
                    dr=min(.2,1.-dr);  // this should handle the case when we cross a periodic boundary
		  }
		}
		else
		{
		  if( fabs(rr(i,axisp1)-.5)<.6 && fabs(r(i1p,i2p,i3p,axisp1)-.5)<.6  )
		  {
		    dr=max(fabs(r(i1p,i2p,i3p,axisp1)-rr(i,axisp1)),fabs(r(i1p,i2p,i3p,axisp2)-rr(i,axisp2)));
                    dr=min(.2,1.-dr);  // this should handle the case when we cross a periodic boundary
		  }
		  
		}
                //  if( dr>.5 )
		//  {
		//    printf("WARNING: i=%i, rr=(%e,%e,%e), rp=(%e,%e,%e) (i1,i2,i3)=(%4i,%4i,%4i) and dr=%e\n",
		//	   i,rr(i,0),rr(i,1),rr(i,2), r(i1p,i2p,i3p,0),r(i1p,i2p,i3p,1),r(i1p,i2p,i3p,2), i1,i2,i3,dr);
		//  }
		    
		if( rr(i,axis1)>rBound(Start,axis1,grid2)-dr && rr(i,axis1)<rBound(End,axis1,grid2)+dr &&
		    rr(i,axis2)>rBound(Start,axis2,grid2)-dr && rr(i,axis2)<rBound(End,axis2,grid2)+dr &&
		    ( numberOfDimensions<3 || 
		      (rr(i,axis3)>rBound(Start,axis3,grid2)-dr && rr(i,axis3)<rBound(End,axis3,grid2)+dr )
		      )
		  )
		{

		  // check for shared sides **** why is this needed ??
                  // *wdh* 990927 if( mask(i1,i2,i3) & MappedGrid::ISinteriorBoundaryPoint )
                  if( mask(i1,i2,i3) & ISnonCuttingBoundaryPoint ) // could be a mixed-boundary pt
		    holeMask(i1,i2,i3)=2;
		  else
		    holeMask(i1,i2,i3)=1;  // point is inside.

		}
		// else if( max(fabs(r(i1,i2,i3,Rx)))<3. )
		else if( rr(i,0)!=Mapping::bogus )
		{
		  holeMask(i1,i2,i3)=2;  // mark this point as "not inside" but invertible
		}

	      } // end for i
	      

              // for any point on grid that can be interpolated, find points nearby on grid2 that are outside the
              // boundary and mark them as unused.

              if( info & 4 )
		display(holeMask,"holeMask (on cutting face ) 1=inside of g2, 0=outside, 2=out (but invertible)",
                logFile,"%2i");
		

              // loop over all points on the face that is cutting a hole
              for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
              for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
              for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
                int ib = iv[axisp1];  // tangential marching direction
                int ib2 = numberOfDimensions>2 ? iv[axisp2] : 0;  // second tangential marching direction (3D)
                if( holeMask(i1,i2,i3)==1 )
		{
                  if( info & 4 ) 
                    fprintf(logFile,"------ cutHoles: process point ib=%i(ib2=%i) (%i,%i,%i) on (side,axis)=(%i,%i) holeMask==1 "
                           " r=(%6.2e,%6.2e,%6.2e)\n",ib,ib2,i1,i2,i3,side,axis,r(i1,i2,i3,0),r(i1,i2,i3,1),
                       numberOfDimensions==2 ? 0. : r(i1,i2,i3,2));
		  
                  // This point on the cutting curve is inside grid2
                
                  // Build a "box" of points on grid2 to check to see if they are inside or outside.
		  // jv : index of closest point to the interpolation point
                  //       (*NOTE* jv==(j1,j2,j3))
                  //             -----
                  //             |    |
                  //          -----1--------------------1------1
                  //             |    |
                  //             X-----
                  //           jv
		  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
                    // note: cellCenterOffset will round to the nearest point. 
                    jv[dir]=(int)floor( r(i1,i2,i3,dir)/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
                    holeMarker(dir)=jv[dir];
		  }
                  // 
                  // sequential boxes should overlap
                  //   o unless we cross a periodic boundary
                  //   o or unless we cross a boundary
                  // In 2D we just compare with the previous box.
                  // In 3D we also compare with the box for the point "below"
                  //          -----+----O----X---->
                  //               |    |    |
                  //          -----+----+----O----
		  
                  holeWidth(Rx,ib)=1; // by default check a stencil that extends by this amount in each direction.
                  int skipThisPoint=0;
		  int initialPoint=0;
		  
                  // m : In 3D we need to check 2 possible previous points.
                  for( int m=0; m<numberOfDimensions-1; m++ )
		  {
		    int axisT = m==0 ? axisp1 : axisp2;  // tangential direction.
		    int ibb =ib-1+m;   // i1-1 or i1

                    int ivp[3]={i1,i2,i3};   // holds next point.
                    ivp[axisT]++;            // increment tangential direction.

		    holeOffset=abs(holeMarker(Rx)-holeCenter(Rx,ibb));  // offset between this point and the last
		    if( max(holeOffset)==0 || max(holeOffset-holeWidth(Rx,ibb))<0 )
		    {
                      // *wdh* 040912 : only skip this pt if the next pt is there 
                      if(ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])==1)
		      {
			// skip this point, it is contained in the previous box (or boxes in 3D)
			skipThisPoint++;
			if( skipThisPoint>=numberOfDimensions-1 )
			{
			  if( info & 4 ) fprintf(logFile,"  skip point ib=%i(%i) (inside previous box)\n",ib,ib2);
			  break;
			}
			else
			  continue;
		      }
		    }
                    // ====================================================================================
                    // check the next point to see if it there
                    // If it is *NOT* there we must increase the size of this box.
                    if( ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=1 )
		    { // the next point is not in the grid or was not a cutting pt
		      if( info & 4 ) 
			fprintf(logFile,"  : m=%i, next point: r=(%6.2e,%6.2e) is not inside, holeMask(next)=%i\n",
			       m, r(ivp[0],ivp[1],ivp[2],0),r(ivp[0],ivp[1],ivp[2],1),holeMask(ivp[0],ivp[1],ivp[2]));
                      bool widthFound=FALSE;
                      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 )
		      {
                        // only use the next point if r does not change too much -- the next r value
                        // could be interpolating from another part of the grid
                        // projectToBoundary will find the closest intersection of the line segment
                        // with the rBound bounding box.
                        real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
                        if( dr<.3 && projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			{
                          widthFound=TRUE;
			  for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            // note: cellCenterOffset will round to the nearest point. 
			    jpv[dir]=(int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
			    holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir])+1);
			  }
                          if( info & 4 ) 
                            fprintf(logFile,"  : m=%i, next pt outside, intersection with bndy=(%6.2e,%6.2e,%6.2e) "
                                   "current holeWidth=(%i,%i,%i)\n",m,rv[0],rv[1],rv[2],
                                    holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
			}
		      }
                      else
		      {
			if( info & 4 ) 
			  fprintf(logFile,"  : holeMask=0 (far outside the grid) -- something is wrong here ? \n");
		      }
                      if( !widthFound ) // 990914 wdh
		      {
			// compute the distance in grid points to the nearest boundary. 
			int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
			for( dir=0; dir<numberOfDimensions; dir++ )
			{
			  bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
				    ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
			}
			if( info & 4 ) 
			  fprintf(logFile,"  : dist. to nearest boundary = %i grid points\n",bDist);
			for( dir=0; dir<numberOfDimensions; dir++ )
			{
			  holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
			  if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			  {
			    if( info & 1 ) 
			      printf("cutHoles:WARNING: Final point holeWidth is very large for this point, "
				     "holeWidth=%i, for point ib=%i(ib2=%i), along axis=%i \n"
				     "  case: next pt on cutting surface not in grid, dist to nearest boundary=%i \n",
				     holeWidth(dir,ib),ib,ib2,dir,bDist);
			    holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
			    if( info & 1 ) 
			      printf("        : something is wrong here. I am reducing the width to %i\n",
				     holeWidth(dir,ib));
			  }
			}
		      }
		    }

                    // ====================================================================================
                    // now check to see if the previous point is there
		    int ivm[3]={i1,i2,i3};   // holds previous point
		    ivm[axisT]--;
                    if( ivm[axisT] < Iv[axisT].getBase() || holeMask(ivm[0],ivm[1],ivm[2])!=1 )
		    {
                      // previous point is NOT inside this grid -- try to guess the box width in other ways.
                      if( ivm[axisT] < Iv[axisT].getBase() )
		      {
			// this is really the first point -- width=1 should do.
                        holeWidth(Rx,ib)=max(1,holeWidth(Rx,ib));
                        if( info & 4 ) 
			{
                          initialPoint++;
                          if( initialPoint>=numberOfDimensions-1 )
                            fprintf(logFile,"  : m=%i, previous pt is outside , this is an INITIAL point, x=(%e,%e,%e)\n",
				   m,x(i1,i2,i3,0),x(i1,i2,i3,1),numberOfDimensions==2 ? 0. : x(i1,i2,i3,2));
			}
		      }
		      else
		      {
                        // this is not the first point, the boundary must have entered this grid.
 	                real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivm[0],ivm[1],ivm[2],Rx)));

                        if( dr>.5 && dr<1.5 ) // *wdh* 060325
                          dr=fabs(1.-dr);  // this should handle the case when we cross a periodic boundary

                        if( ivm[axisT]>=Iv[axisT].getBase() && holeMask(ivm[0],ivm[1],ivm[2])==2 
                            && dr<.3 && projectToBoundary(cg,grid2,r,iv,ivm,rv)==0 )
			{
			  // previous point was invertible but must have been outside (or a non-cutting point).
                          // determine its location index space.

                          if( info & 4 ) 
                            fprintf(logFile,"  : m=%i, prev pt is outside but invert., intersect wth rBound =(%6.2e,%6.2e,%6.2e)\n",
			    m,rv[0],rv[1],rv[2]);
                          // only use the next point if r does not change too much -- the next r value
                          // could be interpolating from another part of the grid
			  for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            // note: cellCenterOffset will round to the nearest point. 
			    jpv[dir]=(int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-cellCenterOffset );
                            int jwidth=abs(jpv[dir]-jv[dir])+1;
			    if( isPeriodic2[dir] )
			    { // correct the case when jpv[dir] crosses a branch cut.
                              if( jwidth> (indexRange2(1,dir)-indexRange2(Start,dir))/2 )
			      {
				jwidth=max(1,abs(jwidth-(indexRange2(1,dir)-indexRange2(Start,dir)+1)));
			      }
			    }
			    
			    holeWidth(dir,ib)=max(holeWidth(dir,ib),jwidth); 

			    if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			    {
			      if( info & 2 ) 
                                printf("cutHoles:WARNING: holeWidth very large, holeWidth=%i,"
				     " grid=%i, grid2=%i, pt ib=%i(%i),m=%i, axis=%i ",holeWidth(dir,ib),grid,grid2,
                                       m,ib,ib2,dir);
			      holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
			      if( info & 2 ) 
                                printf(" -> Reducing width to %i\n", holeWidth(dir,ib));
			    }
			  }
			  if( info & 4 ) 
                            fprintf(logFile,"  : m=%i, previous pt is useable, current holeWidth=(%i,%i,%i)\n",
				 m,holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 1 : holeWidth(2,ib));
			}
                        else 
			{
                          // There is no previous point. Base the width on the next point (if it is there) AND
                          // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
                          if( ivp[axisT]<=Iv[axisT].getBound() && holeMask(ivp[0],ivp[1],ivp[2])!=0  )
			  {
                            real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
			    if( dr<.3 && projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			    { // the next point was invertible.
			      if( info & 4 ) 
				fprintf(logFile,"  : m=%i, next pt is useable, next intersection with "
                                       "r-bndry =(%6.2e,%6.2e,%6.2e) \n",m,rv[0],rv[1],rv[2]);

			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
                                // note: cellCenterOffset will round to the nearest point. 
				jpv[dir] = (int)floor( rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)- 
                                         cellCenterOffset );
				holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
			      }
			    }
			  }
                          // compute the distance in grid points to the nearest boundary. 
                          int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
                          for( dir=0; dir<numberOfDimensions; dir++ )
			  {
                            bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
				           ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
			  }
			  if( info & 4 ) 
                            fprintf(logFile,"  : (index) distance to nearest boundary = %i grid points\n",bDist);
                          for( dir=0; dir<numberOfDimensions; dir++ )
			  {
  			    holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
                            if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
			    {
			      if( info & 2 ) 
                                printf("cutHoles:WARNING: holeWidth very large, holeWidth=%i,"
				     " grid=%i, grid2=%i, pt ib=%i(%i),m=%i, axis=%i",holeWidth(dir,ib),grid,grid2,
                                       m,ib,ib2,dir);
                              holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
                              if( info & 2 ) 
                                printf(" -> reducing width to %i\n",holeWidth(dir,ib));
			    }
			  }
			}
		      }
		    }
		    else
		    {
		      real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivm[0],ivm[1],ivm[2],Rx)));
		      if( dr>.7 ) 
		      {
			// point jumps in r --- need to do something special
                        if( info & 4 ) 
                          fprintf(logFile,"cutHoles:There has been a jump in r, ib=%i(%i),im=%i,\n",ib,ib2,m);
                        for( dir=0; dir<numberOfDimensions; dir++ )
			{
			  if( fabs(r(i1,i2,i3,dir)-r(ivm[0],ivm[1],ivm[2],dir)) > .69 )
			  {
			    if( g2.isPeriodic(dir)==Mapping::functionPeriodic )
			    { // crossed a periodic boundary, 
                              //       +---+---++---+---+
                              //  ... n-2 n-1  n,0  1   2 .... 
                              //           <-  2   ->
                              // holeOffset = |a-(n-b)| = n-a-b : should be a+b
			      holeOffset(dir)=indexRange2(End,dir)-indexRange2(Start,dir)+1-holeOffset(dir);
                              assert( holeOffset(dir) >=0 );
			    }
                            else
			    {
                              if( info & 4 ) 
                                fprintf(logFile,"cutHoles:jump in r but not on a periodic boundary!\n");
			      //  Base the width on the next point (if it is there) AND
			      // extend the width to the nearest boundary. *** this could go wrong maybe ?? ***
			      int ivp[3]={i1,i2,i3};   // holds next point.
                              ivp[axisT]=min(ivp[axisT]+1,Iv[axisT].getBound()); // increment tangential direction.
			      real dr=max(fabs(r(i1,i2,i3,Rx)-r(ivp[0],ivp[1],ivp[2],Rx)));
			      if( holeMask(ivp[0],ivp[1],ivp[2])!=0 && dr<.3 && 
				  projectToBoundary(cg,grid2,r,iv,ivp,rv)==0 )
			      { // the next point was invertible.
				if( info & 4 ) 
				  fprintf(logFile,"        : jump at ib=%i, next intersection with rBound =(%6.2e,%6.2e,%6.2e)\n",
					 ib, rv[0],rv[1],rv[2]);

				for( dir=0; dir<numberOfDimensions; dir++ )
				{
				  jpv[dir]=(int)floor(rv[dir]/g2.gridSpacing(dir)+indexRange2(Start,dir)-
                                    cellCenterOffset);
				  holeWidth(dir,ib)=max(holeWidth(dir,ib),abs(jpv[dir]-jv[dir]));
				}
			      }

			      // compute the distance in grid points to the nearest boundary. 
			      int bDist=min(extendedIndexRange2(End,Rx)-extendedIndexRange2(Start,Rx));
			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
				bDist=min(bDist,abs(holeMarker(dir)-extendedIndexRange2(Start,dir))
					  ,abs(holeMarker(dir)-extendedIndexRange2(End  ,dir)));
			      }
			      if( info & 4 ) 
                                 fprintf(logFile,"cutHoles: Jump in r, ib=%i, distance to nearest boundary = %i grid points\n",
				        ib,bDist);
			      for( dir=0; dir<numberOfDimensions; dir++ )
			      {
				holeWidth(dir,ib)=max(holeWidth(dir,ib),bDist);
				if( holeWidth(dir,ib) > max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8) )
				{
				  if( info & 1 ) 
                                    printf("cutHoles:WARNING: holeWidth is very large for this point, holeWidth=%i,"
                                     " for point ib=%i(%i),m=%i, along axis=%i",holeWidth(dir,ib),ib,ib2,m,dir);
				  holeWidth(dir,ib)=max(maximumHoleWidth,(indexRange2(End,dir)-indexRange2(Start,dir))/8);
				  if( info & 1 ) 
                                    printf(" -> reducing to %i\n",holeWidth(dir,ib));
				}
			      }
			      holeOffset(0)=-1;  // no need to compute holeWidth below
			    }
			  }
			}
		      }
                      if( holeOffset(0) >=0 )
		      {
			if( numberOfDimensions==2 )
			{
			  if( holeOffset(0)==0 )
			  { // boxes lie one above each other of each other
			    holeWidth(1,ib)=max(holeWidth(1,ib),holeOffset(1)-1);
			  }
			  else if( holeOffset(1)==0 )
			  { // boxes are horizontal
			    holeWidth(0,ib)=max(holeWidth(0,ib),holeOffset(0)-1);
			  }
			  else
			  {
			    // holes are on a diagonal
                            if( info & 4 ) 
                              fprintf(logFile,"    : holes are on a diagonal\n");
			    holeWidth(Rx,ib)=max(holeWidth(Rx,ib),holeOffset(Rx)+1);
			  }
			}
			else
			{
			  // 3D:
			  if( holeOffset(1)==0 && holeOffset(2)==0 )
			  {// boxes are horizontal
			    holeWidth(0,ib)=max(holeWidth(0,ib),holeOffset(0)-1);
			  }
			  else if( holeOffset(2)==0 && holeOffset(0)==0 )
			  {// boxes are vertical  
			    holeWidth(1,ib)=max(holeWidth(1,ib),holeOffset(1)-1);
			  }
			  else if( holeOffset(0)==0 && holeOffset(1)==0 )
			  {
			    holeWidth(2,ib)=max(holeWidth(2,ib),holeOffset(2)-1);
			  }
			  else
			  {
			    // holes are on a diagonal
			    holeWidth(Rx,ib)=max(holeWidth(Rx,ib),holeOffset(Rx)+1);
			  }
			}
		      }
		    }
		  } // end for m
                  holeCenter(Rx,ib)=holeMarker(Rx);
		  if( skipThisPoint>=numberOfDimensions-1 )
                    continue;
		  if( info & 4 ) 
                    fprintf(logFile,"  *** : grid2=%i, point ib=%i(%i), r=(%6.2e,%6.2e,%6.2e), holeCenter=(%i,%i,%i), "
			   "width=(%i,%i,%i)\n",grid2,ib,ib2,
                         r(i1,i2,i3,0),r(i1,i2,i3,1),numberOfDimensions==2 ? 0. : r(i1,i2,i3,2),
                         holeCenter(0,ib),holeCenter(1,ib),numberOfDimensions==2 ? 0 : holeCenter(2,ib),
                         holeWidth(0,ib),holeWidth(1,ib),numberOfDimensions==2 ? 0 : holeWidth(2,ib));
		  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
                    
                    if( g2.isPeriodic(dir)!=Mapping::functionPeriodic ) // ************ 060325 *
		    {
		      jpv[dir]=min(jv[dir]+holeWidth(dir,ib),extendedIndexRange2(End,dir));
		      jv[dir] =max(jv[dir]-holeWidth(dir,ib),extendedIndexRange2(Start,dir));
		    }
		    else
		    {
		      jpv[dir]=jv[dir]+holeWidth(dir,ib); 
		      jv[dir] =jv[dir]-holeWidth(dir,ib); 
		    }
		    
		  }
		   // make a list of boundary points and their inverseMap images
                  // first make sure there is enough space:
                  int numberOfNewPoints=(jpv[0]-j1+1)*(jpv[1]-j2+1)*(jpv[2]-j3+1);  
		  if( ia.getLength(0)<=numberCut+numberOfNewPoints )
		    ia.resize((ia.getLength(0)+numberOfNewPoints)*2,7);


                  // ***********************************************************
		  // *** Fill-in the ia array with potential points to cut *****
                  // ***********************************************************
                  // At this stage we include points that are both inside and outside grid
                  int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
                  for( int k3a=j3; k3a<=jpv[2]; k3a++ )
 		  for( int k2a=j2; k2a<=jpv[1]; k2a++ )
                  for( int k1a=j1; k1a<=jpv[0]; k1a++ )
 		  {
                    
                    // ************ 060325 ***************
                    k1=k1a; k2=k2a; k3=k3a;
                    for( dir=0; dir<numberOfDimensions; dir++ )
		    {
		      if( isPeriodic2[dir] )
		      {
			if( kv[dir]<extendedIndexRange2(Start,dir) )
			{
			  kv[dir]+=indexRange2(1,dir)-indexRange2(0,dir)+1;
			}
			else if( kv[dir]>extendedIndexRange2(End,dir) )
			{
			  kv[dir]-=indexRange2(1,dir)-indexRange2(0,dir)+1;
			}
		      }
		    }
		    

		    if(
		      // *0 mask2(k1,k2,k3)!=0 &&           // this point already cut
		       cut(k1,k2,k3) >=0 &&  // this point not in the list yet
		        (mask2(k1,k2,k3)!=0 || cut(k1,k2,k3)==share) && 
		      inverseGrid(k1,k2,k3)!=grid &&  // *wdh* added 990426
		      !( mask2(k1,k2,k3) & ISnonCuttingBoundaryPoint) )  // is this correct?
		      // *wdh*  990927 !( mask2(k1,k2,k3) & MappedGrid::ISinteriorBoundaryPoint) )  
		    {
		      ia(numberCut,0)=k1;
		      ia(numberCut,1)=k2;
		      ia(numberCut,2)=k3;
		      ia(numberCut,3)=mask2(k1,k2,k3);

                      if( mayCutHoles )
		      {

			ia(numberCut,4)=i1;  // save these values for double checking points that cannot be inverted.
			ia(numberCut,5)=i2;
			ia(numberCut,6)=i3;

                        // 1. do not cut a hole at a pt that could already interp from inside a 
                        //    grid with the same share value:
                        // 2. Do not cut holes with a phantom hole cutter.
                        if( cut(k1,k2,k3)!=share && !phantomHoleCutting )
  		          mask2(k1,k2,k3)=0;   

                        // cutShare2(k1,k2,k3)=share;
			
		      }
   		      cut(k1,k2,k3)=-1;
		      numberCut++;

                      if( debug & 4 )
                        fprintf(logFile,"---- Cutting a hole on grid2=%i at (%i,%i,%i) share=%i \n",
                                grid2,k1,k2,k3,share);
		      
		    }
		  } // end for k1a, k2a,k3a
		}
                else 
		{
                  // *** this point is not inside.
                  // If the previous point was inside 
/* -----                  
                  for( int m=0; m<g.numberOfDimensions()-1; m++ )
		  {
		    int axisT = (axis +1+m) % numberOfDimensions;  // tangential direction.
		    int ibb =ib-1+m;   // i1-1 or i1
                    if( holeCenter(0,ibb)>=indexRange00 )
		    {
                      // previous 
		    }
		  }
---- */
		  holeCenter(0,ib)=indexRange00-1;  // mark this point as unused
		}
	      }  // for i1,i2,i3

              if( vectorize && numberCut > 0 )
	      {
                // =========================================================
		// ====== now double check the points we cut out  ==========
                // =========================================================

		R=Range(0,numberCut-1);
		x2.redim(R,Rx);
		r2.redim(R,Rx); r2=-2.;
		if( !isRectangular2 )
		{
		  for( dir=0; dir<numberOfDimensions; dir++ )
		    x2(R,dir)=center2(ia(R,0),ia(R,1),ia(R,2),dir);
		}
		else
		{
		  for( int i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);  // fill in iv[0..2]
		    for( dir=0; dir<numberOfDimensions; dir++ )
		    {
		      x2(i,dir)=XC2(iv,dir);
		    }
		  }
		}
		
                if( useBoundaryAdjustment )
                  adjustBoundary(cg,grid2,grid,ia(R,Rx),x2);  // adjust boundary points on shared sides 
		
		map.inverseMapC(x2,r2);
                // determine xB -- the closest pt on the boundary 
                realArray rB(R,Rx), xB(R,Rx);
                rB=(real)side; // project inverse pt to the boundary
                where( r2(R,0)!=Mapping::bogus )
		{
		  rB(R,axisp1)=r2(R,axisp1);
		  if( numberOfDimensions==3 )
  		    rB(R,axisp2)=r2(R,axisp2);
		}
		map.mapC(rB,xB);

//                 if( FALSE && debug & 4 )
// 		{
//                   char buff[80];
// 		  display(ia(R,Rx),sPrintF(buff,"Here is ia on grid=%i\n",grid),logFile," %3i");
// 		  display(x2(R,Rx),sPrintF(buff,"Here are the x2 coordinates on grid=%i\n",grid),logFile," %9.2e");
// 		  display(r2(R,Rx),sPrintF(buff,"Here are the r2 coordinates on grid=%i\n",grid),logFile," %9.2e");
// 		}
		
                if( debug & 4 )
		{
                  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    fprintf(logFile,"potential hole pt: grid2=%i ia=(%3i,%3i,%3i), r2=(%8.1e,%8.1e,%8.1e)\n",
			    grid2,ia(i,0),ia(i,1),ia(i,2),
			    r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)));
		  }
                  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    fprintf(logFile,"cutHoles: invert grid=%i, i=%i ia=(%i,%i,%i), x2=(%9.2e,%9.2e,%9.2e)"
			    "r2=(%5.2e,%5.2e,%5.2e) axis=%i\n",grid,i,
			    ia(i,0),ia(i,1),ia(i,2),
			    x2(i,0),x2(i,1),(numberOfDimensions==2? 0. : x2(i,2)),
			    r2(i,0),r2(i,1),(numberOfDimensions==2? 0. : r2(i,2)),axis);
		  }
		  for( i=R.getBase(); i<=R.getBound(); i++ )
		  {
		    if(  r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
			 r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) &&
                         ( numberOfDimensions==2 || 
                        (r2(i,axis3)>=rBound(Start,axis3,grid) && r2(i,axis3)<=rBound(End,axis3,grid)) ) )
		    {
		      fprintf(logFile,"cutHoles: un-cutting a hole: grid2=%i (%s), ia=(%i,%i,%i), "
			      "r=(%7.1e,%7.1e,%7.1e) axis=%i **can interpolate\n",grid2,
			      (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
			      r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)),axis);
		    }
		    else if(  r2(i,axis)!=Mapping::bogus &&       // point was invertible
			      ( fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon
				|| (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
				|| r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)
                                || r2(i,axisp2)<=rBound(Start,axisp2,grid) || r2(i,axisp2)>=rBound(End,axisp2,grid) ) )
		    {
		      fprintf(logFile,"cutHoles: un-cutting a hole: grid2=%i (%s), ia=(%i,%i,%i), "
			      "r=(%7.1e,%7.1e,%7.1e) grid=%i side=%i axis=%i **reset mask to %i\n",grid2,
			      (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
			      r2(i,0),r2(i,1),(numberOfDimensions==2 ? 0. : r2(i,2)),grid,side,axis,ia(i,3));
		    }
		  }
		}
		
                // *****************************************
                // ****** Mark interpolation points ********
                // *****************************************
                if( true )
		{
                  // ##################### optimise these scalar indexing loops. ##############
		  const int bound=R.getBound();
		  if( numberOfDimensions==2 )
		  {
                    for( int i=0; i<=bound; i++ )
		    {
                      i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
                      // r2(i,dir) : position of the potential hole point on the cutter grid.
		      if( r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
			  r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) )
		      { 
                        // we can interpolate: the potential hole point on grid2 is actually inside grid
                        // *wdh* 011027 only change interp if this is a higher priority
                        if( inverseGrid(i1,i2,i3)<grid || mask2(i1,i2,i3)!=MappedGrid::ISinterpolationPoint) 
			{
			  mask2(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
			  inverseGrid(i1,i2,i3)= grid;  
			  for( dir=0; dir<numberOfDimensions; dir++ )
			    rI(i1,i2,i3,dir)=r2(i,dir);
			}
		      }
		      else if( r2(i,axis)!=Mapping::bogus &&       // point was invertible
			       ( fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon
				 || (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
				 || r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)) )
		      {
                        // the potential hole point is not inside the cutter grid but it is
                        //     1) almost inside in the normal direction 
                        // OR  2) outside the opposite boundary in the normal direction
                        // OR  3) outside in the tangential directions of the cutter grid.
                        // (The case of a non-invertible point is treated later)

                        // *** we should mark this point as UNKNOWN status so we check it later *****
			mask2(i1,i2,i3)=ia(i,3); // reset these values
		      }
		    }
		    
		  }
		  else  // 3d
		  {
                    for( int i=0; i<=bound; i++ )
		    {
                      i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
		      if( r2(i,axis1)>=rBound(Start,axis1,grid) && r2(i,axis1)<=rBound(End,axis1,grid) &&
			  r2(i,axis2)>=rBound(Start,axis2,grid) && r2(i,axis2)<=rBound(End,axis2,grid) &&
			  r2(i,axis3)>=rBound(Start,axis3,grid) && r2(i,axis3)<=rBound(End,axis3,grid) )
		      {
                        // *wdh* 011027 only change interp if this is a higher priority
                        if( inverseGrid(i1,i2,i3)<grid || mask2(i1,i2,i3)!=MappedGrid::ISinterpolationPoint) 
			{
			  mask2(i1,i2,i3)=MappedGrid::ISinterpolationPoint; 
			  inverseGrid(i1,i2,i3)= grid;  
			  for( dir=0; dir<numberOfDimensions; dir++ )
			    rI(i1,i2,i3,dir)=r2(i,dir);
			}
		      }
		      else if( r2(i,axis)!=Mapping::bogus &&         // point was invertible
			       (fabs(r2(i,axis  )-.5) <= .5+boundaryEpsilon 
				|| (side==0 && r2(i,axis  )>=0. ) ||(side==1 && r2(i,axis  )<=1. )
				|| r2(i,axisp1)<=rBound(Start,axisp1,grid) || r2(i,axisp1)>=rBound(End,axisp1,grid)
				|| r2(i,axisp2)<=rBound(Start,axisp2,grid) || r2(i,axisp2)>=rBound(End,axisp2,grid)) )
		      {
			mask2(i1,i2,i3)=ia(i,3); // MappedGrid::ISdiscretizationPoint; // reset these values
		      }
		    }
		  }
		  
		}
		else  // if true 
		{ 
                  // ***************** this section not used ***********************
		  if( numberOfDimensions==2 )
		  {
		    // where( fabs(r2(R,axis1)-.5) <= .5+boundaryEps && fabs(r2(R,axis2)-.5) <= .5+boundaryEps  )
		    where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
			   r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) )
		    { // we can interpolate
		      mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
		      inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		      for( dir=0; dir<numberOfDimensions; dir++ )
			rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
		    }
		    elsewhere( r2(R,axis)!=Mapping::bogus &&       // point was invertible
			       ( fabs(r2(R,axis  )-.5) <= .5+boundaryEpsilon
				 || (side==0 && r2(R,axis  )>=0. ) ||(side==1 && r2(R,axis  )<=1. )
				 || r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)) )
		    {
		      mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
		    }
		  }
		  else  // 3d
		  {
		    where( r2(R,axis1)>=rBound(Start,axis1,grid) && r2(R,axis1)<=rBound(End,axis1,grid) &&
			   r2(R,axis2)>=rBound(Start,axis2,grid) && r2(R,axis2)<=rBound(End,axis2,grid) &&
			   r2(R,axis3)>=rBound(Start,axis3,grid) && r2(R,axis3)<=rBound(End,axis3,grid) )
		    {
		      mask2(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint; 
		      inverseGrid(ia(R,0),ia(R,1),ia(R,2))= grid;  
		      for( dir=0; dir<numberOfDimensions; dir++ )
			rI(ia(R,0),ia(R,1),ia(R,2),dir)=r2(R,dir);
		    }
		    elsewhere( r2(R,axis)!=Mapping::bogus &&         // point was invertible
			       (fabs(r2(R,axis  )-.5) <= .5+boundaryEpsilon 
				|| (side==0 && r2(R,axis  )>=0. ) ||(side==1 && r2(R,axis  )<=1. )
				|| r2(R,axisp1)<=rBound(Start,axisp1,grid) || r2(R,axisp1)>=rBound(End,axisp1,grid)
				|| r2(R,axisp2)<=rBound(Start,axisp2,grid) || r2(R,axisp2)>=rBound(End,axisp2,grid)) )
		    {
		      mask2(ia(R,0),ia(R,1),ia(R,2))=ia(R,3); // MappedGrid::ISdiscretizationPoint; // reset these values
		    }
		  }
		}
		

                if( !mayCutHoles )
                  numberCut=0;

                if( g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
		{
                  //  mixedBoundary -- don't cut holes where there is a non-cutting portion of the boundary
		  for( int i=0; i<numberCut; i++ )
		  {
		    if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 && r2(i,axis)!=Mapping::bogus )
		    {
                      bool okToCheck=TRUE;
                      j3=g.gridIndexRange(Start,axis3);
                      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
		      {
                        jv[dir]=int( rB(i,dir)/g.gridSpacing(dir)+g.gridIndexRange(Start,dir) );
                        okToCheck=okToCheck && (jv[dir]>=g.gridIndexRange(Start,dir) && 
						jv[dir]<=g.gridIndexRange(End  ,dir));
		      }
                      if( okToCheck && mask(j1,j2,j3) & ISnonCuttingBoundaryPoint )
		      {
                        // reset this point
                        if( debug & 4 )
                          fprintf(logFile,"Uncut point (%i,%i,%i) on grid %i on a mixed boundary\n",
                                  ia(i,0),ia(i,1),ia(i,2),grid2);
  		        mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3); // reset these values
		      }
		    }
		  }
		  
		}
	      
                for( int i=0; i<numberCut; i++ )
		{
                  if( mask2(ia(i,0),ia(i,1),ia(i,2))!=0 )
		  {
		    continue; // this point was not cut
		  }
                  else if( r2(i,axis)!=Mapping::bogus )
		  {
                    // point was invertible
		    if( fabs(r2(i,axis)-.5)<.5+biggerBoundaryEps &&
			( fabs( r2(i,axisp1)-.5) > .5 || (numberOfDimensions==3 && fabs( r2(i,axisp2)-.5) > .5 ) ) )
		    {
                      // *** wasn't this checked above ??  *****

		      // invertible, outside [0,1] in tangential direction and close enough in the normal
		      // We need this since points outside [0,1] may not be inverted as accurately by Newton.
                      if( debug & 2 )
		        fprintf(logFile,"++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. "
                                "r=(%7.1e,%7.1e,%7.1e)  "
				"This pt is close to the boundary and outside [0,1] in tangent directions\n",
				ia(i,0),ia(i,1),ia(i,2),grid2,grid,r2(i,0),r2(i,1),numberOfDimensions==2 ? 0. : r2(i,2));
		      mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
		    }
		    else
		    {
		      // estimate the distance to the boundary -- only cut points that are close to the cutting surface
                      // dx2 = square of the length of the diagonal of the cell on grid2
		      real distToBndry=0.,dx2=0.;
                      i3p=g2.dimension(End,axis3);
  		      // *wdh* 990918 use diagonal ipv[axis]=ia(i,axis);
                      ipv[axis]=ia(i,axis)+1;
                      if( ipv[axis] > g2.dimension(End,axis) )
                        ipv[axis]=ia(i,axis)-1;

                      ipv[axisp1]=ia(i,axisp1)+1; 
                      if( ipv[axisp1] > g2.dimension(End,axisp1) )
                        ipv[axisp1]=ia(i,axisp1)-1;

                      ipv[axisp2]=ia(i,axisp2)+1; 
                      if( ipv[axisp2] > g2.dimension(End,axisp2) )
                        ipv[axisp2]=ia(i,axisp2)-1;
		      int ax;		      
                      if( !isRectangular2 )
		      {
			for( ax=0; ax<numberOfDimensions; ax++ )
			{
			  distToBndry+=SQR(x2(i,ax)-xB(i,ax));
			  dx2+=SQR(center2(ia(i,0),ia(i,1),ia(i,2),ax)-center2(i1p,i2p,i3p,ax));
			}
		      }
		      else
		      {
                        i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);  // fill in iv[0..2]
			for( ax=0; ax<numberOfDimensions; ax++ )
			{
			  distToBndry+=SQR(x2(i,ax)-xB(i,ax));
			  dx2+=SQR(XC2(iv,ax)-XC2(ipv,ax));
			}
		      }
		      
                      if( distToBndry > maxDistFactor*dx2 || distToBndry>maximumHoleCuttingDistanceSquared )
		      {
			if( debug & 2 )
			{
			  fprintf(logFile,"++cutHoles: un-cutting a hole (%i,%i,%i) on grid=%i by grid=%i. "
				  " r=(%7.1e,%7.1e,%7.1e) pt is too far from boundary. \n"
                                  " distToBndry=%8.1e dx2=%8.1e x2=(%9.2e,%9.2e) xB=(%9.2e,%9.2e) \n",
				  ia(i,0),ia(i,1),ia(i,2),grid2,grid,r2(i,0),r2(i,1),
                                  numberOfDimensions==2 ? 0. : r2(i,2), sqrt(distToBndry),sqrt(dx2),
                                  x2(i,0),x2(i,1),xB(i,0),xB(i,1));
                          if( distToBndry>maximumHoleCuttingDistanceSquared )
			    fprintf(logFile,"since distToBndry=%7.2e >maximumHoleCuttingDistance=%7.2e\n",
				    SQRT(distToBndry),SQRT(maximumHoleCuttingDistanceSquared));
			}
			mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);  // reset this value
		      }
		    }
		  }
		  else if( r2(i,axis)==Mapping::bogus )  // NOT invertible
		  {
		    // if the point was NOT invertible then we double check to see it is outside of the boundary
                    // Estimate the r location for the point using
                    //        r = r(boundary) + dr
                    //        dr = [ dr/dx ] * dx       
                    //        dx = vector from boundary point jv to the point on the other grid, x2

                    // the point (j1,j2,j3) was used to cut this hole -- we need the closest bndry pt.
                    jv[0]=ia(i,4); jv[1]=ia(i,5); jv[2]=ia(i,6);
		    real distToBndry=REAL_MAX;
                    real xv[3]={0.,0.,0.};
                    int ax;
                    for( ax=0; ax<numberOfDimensions; ax++ )
  		      xv[ax]=x2(i,ax);
                    map.approximateGlobalInverse->binarySearchOverBoundary( xv,distToBndry,jv,side,axis ); 
                    jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
                    jpv[axisp1]=jv[axisp1]+1;   
                    if( jpv[axisp1] > g.dimension(End,axisp1) )
                      jpv[axisp1]=jv[axisp1]-1;
                
                    // compute the actual distance between x2 to the boundary segment
                    // jv ---> jpv
                    real xvj[3], xvp[3];
		    for( ax=0; ax<numberOfDimensions; ax++ )
		    {
		      xvj[ax]=!isRectangular ? vertex(j1,j2,j3,ax)    : XV(jv,ax);
		      xvp[ax]=!isRectangular ? vertex(j1p,j2p,j3p,ax) : XV(jpv,ax);
		    }
		      
                    if( numberOfDimensions==2 )
		    {
                      real dot = 0., norm=0.;
		      for( ax=0; ax<numberOfDimensions; ax++ )
		      {
			dot+=(x2(i,ax)-xvj[ax])*(xvp[ax]-xvj[ax]);
			norm+=SQR(xvp[ax]-xvj[ax]);
		      }
		      distToBndry-=dot*dot/max(REAL_MIN,norm);
		    }
//                    else
//		    {
//		      if( false ) // this correction doesn't seem to work very well
//		      {
// 			ipv[0]=j1; ipv[1]=j2; ipv[2]=j3;
// 			ipv[axisp2]=jv[axisp2]+1;  
// 			if( ipv[axisp2] > g.dimension(End,axisp2) )
// 			  ipv[axisp2]=jv[axisp2]-1;  
// 			real nn[3];  // normal to the plane
// 			nn[0]=(vertex(j1p,j2p,j3p,1)-vertex(j1,j2,j3,2))*(vertex(i1p,i2p,i3p,2)-vertex(j1,j2,j3,1));
// 			nn[1]=(vertex(j1p,j2p,j3p,2)-vertex(j1,j2,j3,0))*(vertex(i1p,i2p,i3p,0)-vertex(j1,j2,j3,2));
// 			nn[2]=(vertex(j1p,j2p,j3p,0)-vertex(j1,j2,j3,1))*(vertex(i1p,i2p,i3p,1)-vertex(j1,j2,j3,0));
// 			real dot = 0., norm=0.;
// 			for( ax=0; ax<numberOfDimensions; ax++ )
// 			{
// 			  dot+=(x2(i,ax)-vertex(j1,j2,j3,ax))*nn[ax];
// 			  norm+=SQR(nn[ax]);
// 			}
// 			distToBndry=dot*dot/max(REAL_MIN,norm); 
//		      }
//		      
//		    }
		    
                    real re[3], dx[3], det;
                    for( ax=0; ax<numberOfDimensions; ax++ )
                      dx[ax]=x2(i,ax)-xvj[ax];

// define XR(m,n) xr((j1),(j2),(j3),(m)+(n)*numberOfDimensions)
#define XR(m,n) xra[n][m]

                    int kv[3];
                    if( numberOfDimensions==2 )
		    {
                      real xra[2][2];
                      if( (j1+1) <= g.dimension(End,0) )
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
                          if( !isRectangular )
  			    xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
			  else
			  {
                            kv[0]=j1+1, kv[1]=j2, kv[2]=j3;
                            xra[0][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(0);
			  }
			}
		      }
                      else
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			 if( !isRectangular )
                            xra[0][ax]=(xvj[ax]-vertex(j1-1,j2,j3,ax))/g.gridSpacing(0);
			 else
			 {
                           kv[0]=j1-1, kv[1]=j2, kv[2]=j3;
			   xra[0][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(0);
			 }
			}
		      }
                      if( (j2+1) <= g.dimension(End,1) )
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
			  else
			  {
			    kv[0]=j1, kv[1]=j2+1, kv[2]=j3;
			    xra[1][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(1);
			  }
			}
		      }
                      else
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[1][ax]=(xvj[ax]-vertex(j1,j2-1,j3,ax))/g.gridSpacing(1);
			  else
			  {
			    kv[0]=j1, kv[1]=j2-1, kv[2]=j3;
			    xra[1][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(1);
			  }
			}
		      }
		      // assert( (j1+1) <= g.dimension(End,0) );
		      // assert( (j2+1) <= g.dimension(End,1) );
		      // for(ax=0; ax<numberOfDimensions; ax++ ) 
 		      // {
   		      //   xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
 		      //   xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
 		      // }
                      real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
                      if( det!=0. )
		      {
                        det=1./det;
    		        re[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
		        re[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
		      }
		      else
		      { // if the jacobian is singular
                        if( debug & 1 )
                          printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                        re[0]=re[1]=0.;
                        re[axis]=.1*(2*side-1); // move point outside the grid
		      }
                      re[2]=0.;
		    }
		    else
		    {
                      real xra[3][3];
		      if( (j1+1) <= g.dimension(End,0) )
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
                          else
			  {
			    kv[0]=j1+1, kv[1]=j2, kv[2]=j3;
			    xra[0][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(0);
			  }
			}
		      }
		      else
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[0][ax]=(xvj[ax]-vertex(j1-1,j2,j3,ax))/g.gridSpacing(0);
                          else
			  {
			    kv[0]=j1-1, kv[1]=j2, kv[2]=j3;
			    xra[0][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(0);
			  }
			}
		      }
		      if(  (j2+1) <= g.dimension(End,1) )
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
                          else
			  {
			    kv[0]=j1, kv[1]=j2+1, kv[2]=j3;
			    xra[1][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(1);
			  }
			}
		      }
		      else
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[1][ax]=(xvj[ax]-vertex(j1,j2-1,j3,ax))/g.gridSpacing(1);
                          else
			  {
			    kv[0]=j1, kv[1]=j2-1, kv[2]=j3;
			    xra[1][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(1);
			  }
			}
		      }
		      if( (j3+1) <= g.dimension(End,2) )
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[2][ax]=(vertex(j1,j2,j3+1,ax)-xvj[ax])/g.gridSpacing(2);
                          else
			  {
			    kv[0]=j1, kv[1]=j2, kv[2]=j3+1;
			    xra[2][ax]=(XV(kv,ax)-xvj[ax])/g.gridSpacing(2);
			  }
			}
		      }
		      else
		      {
			for(ax=0; ax<numberOfDimensions; ax++ ) 
			{
			  if( !isRectangular )
			    xra[2][ax]=(xvj[ax]-vertex(j1,j2,j3-1,ax))/g.gridSpacing(2);
                          else
			  {
			    kv[0]=j1, kv[1]=j2, kv[2]=j3-1;
			    xra[2][ax]=(xvj[ax]-XV(kv,ax))/g.gridSpacing(2);
			  }
			}
		      }
/* --------------------		      
		      assert( (j1+1) <= g.dimension(End,0) );
		      assert( (j2+1) <= g.dimension(End,1) );
		      assert( (j3+1) <= g.dimension(End,2) );

                      for(ax=0; ax<numberOfDimensions; ax++ ) 
		      {
			xra[0][ax]=(vertex(j1+1,j2,j3,ax)-xvj[ax])/g.gridSpacing(0);
			xra[1][ax]=(vertex(j1,j2+1,j3,ax)-xvj[ax])/g.gridSpacing(1);
			xra[2][ax]=(vertex(j1,j2,j3+1,ax)-xvj[ax])/g.gridSpacing(2);
		      }
----------------------------- */
		      det = (XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0))*XR(2,2) +
			    (XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1))*XR(2,0) +
			    (XR(0,2)*XR(1,0)-XR(0,0)*XR(1,2))*XR(2,1);
                      if( det!=0. )
		      {
                        det=1./det;
			re[0]=( (XR(1,1)*XR(2,2)-XR(2,1)*XR(1,2))*dx[0]+
				(XR(2,1)*XR(0,2)-XR(0,1)*XR(2,2))*dx[1]+
				(XR(0,1)*XR(1,2)-XR(1,1)*XR(0,2))*dx[2] )*det;
		      
			re[1]=( (XR(1,2)*XR(2,0)-XR(2,2)*XR(1,0))*dx[0]+
				(XR(2,2)*XR(0,0)-XR(0,2)*XR(2,0))*dx[1]+
				(XR(0,2)*XR(1,0)-XR(1,2)*XR(0,0))*dx[2] )*det;
		      
			re[2]=( (XR(1,0)*XR(2,1)-XR(2,0)*XR(1,1))*dx[0]+
				(XR(2,0)*XR(0,1)-XR(0,0)*XR(2,1))*dx[1]+
				(XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1))*dx[2] )*det;
			
		      }
		      else
		      { // if the jacobian is singular
                        if( debug & 1 )
                          printf("cutHoles:WARNING: non-invertible point and jacobian=0. for estimating location\n");
                        re[0]=re[1]=re[2]=0.;
                        re[axis]=.1*(2*side-1);
		      }
		      
		    }
#undef XR
                    for( ax=0; ax<numberOfDimensions; ax++ )
                      re[ax]+=jv[ax]*g.gridSpacing(ax);
		    
		    if( debug & 4 )
		    {
		      fprintf(logFile,"cutHoles: Non-invert pt: ia=(%i,%i,%i) est. r=(%7.2e,%7.2e,%7.2e) distToBndry=%8.2e is",
			      ia(i,0),ia(i,1),ia(i,2),re[0],re[1],re[2],distToBndry);
		      if(fabs(re[axis]-.5) <= .5+boundaryEpsilon ) 
			fprintf(logFile," inside in the normal direction (axis=%i)!\n",axis);
		      else
			fprintf(logFile," outside in the normal direction (axis=%i)!\n",axis);
		    }
		    
                    // do not cut a hole if the point is
                    //     1. inside in the normal direction
                    // or  2. on the wrong side in the normal direction
                    // or  3. outside in the tangential direction.
                    if( fabs(re[axis]-.5) <= .5+boundaryEpsilon 
			 || (side==0 && re[axis  ]>=0. ) ||(side==1 && re[axis  ]<=1. )
			 || re[axisp1]< -g.gridSpacing(axisp1) || re[axisp1]>1.+g.gridSpacing(axisp1)
			 || re[axisp2]< -g.gridSpacing(axisp2) || re[axisp2]>1.+g.gridSpacing(axisp2) )
		      // *wdh* 990702 || re[axisp1]<=0. || re[axisp1]>=1.
		      // *wdh* 990702 || re[axisp2]<=0. || re[axisp2]>=1. )
		      // *wdh* 990502 || re[axisp1]<=rBound(Start,axisp1,grid) || re[axisp1]>=rBound(End,axisp1,grid)
		      // *wdh* 990502 || re[axisp2]<=rBound(Start,axisp2,grid) || re[axisp2]>=rBound(End,axisp2,grid) )
		    {
                      if( debug & 4 )
		      {
			fprintf(logFile,"cutHoles: Non-invertible point: ia=(%i,%i,%i) est. r=(%7.2e,%7.2e,%7.2e) is",
			       ia(i,0),ia(i,1),ia(i,2),re[0],re[1],re[2]);
			if(fabs(re[axis]-.5) <= .5+boundaryEpsilon ) 
			  fprintf(logFile," inside in the normal direction (axis=%i)! No hole cut. ****** \n",axis);
			else
			  fprintf(logFile," outside in the tangential direction. No hole cut. \n");
			
		      }
		      mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
		    }
                    else
		    {
		      real cosAngle =0.;
		      jpv[0]=j1; jpv[1]=j2; jpv[2]=j3;
		      jpv[axis]=g.gridIndexRange(1-side,axis);  // opposite boundary
		      for( ax=0; ax<numberOfDimensions; ax++ )
			cosAngle += normal(j1,j2,j3,ax)*dx[ax]; //  (x2(i,ax)-vertex(j1,j2,j3,ax));

                      // in 3d we should probably take the minimum of all edge lengths or use
                      // the normal distance in both 2d and 3d
  		      ipv[axis]=ia(i,axis);
                      ipv[axisp1]=ia(i,axisp1)+1;
                      ipv[axisp2]=ia(i,axisp2); // ia(i,axisp2)+1; *wdh* 021230

                      real dx2=0.;
                      if( ipv[axis]<=g2.dimension(1,axis) )
		      {
                        if( !isRectangular2 )
			{
			  for( ax=0; ax<numberOfDimensions; ax++ )
			    dx2+=SQR(center2(ia(i,0),ia(i,1),ia(i,2),ax)-center2(i1p,i2p,i3p,ax));
			}
			else
			{
                          iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
			  for( ax=0; ax<numberOfDimensions; ax++ )
			    dx2+=SQR(XC2(iv,ax)-XC2(ipv,ax));
			}
			
		      }
                      cosAngle/= SQRT(max(REAL_MIN,distToBndry));
		      const real maxCosAngle=.0;  
		      // we do not cut a hole if the cosine of the angle between normals < maxCosAngle or if
		      // the distance to the potential hole point is greater than the distance to the opposite boundary
		      // 
		      if( debug & 4 )
		      {
			fprintf(logFile,"cutHoles: INFO non-invert pt (%i,%i,%i) on grid=%i by grid=%i "
				"distToBndry=%8.2e, dx2*maxDistFactor=%8.2e\n",
				ia(i,0),ia(i,1),ia(i,2),grid2,grid,distToBndry,dx2*maxDistFactor );
		      }
		      

		      if( cosAngle < maxCosAngle ||  // if cosine of the angle between normals > ?? 
                          distToBndry>dx2*maxDistFactor  || distToBndry>maximumHoleCuttingDistanceSquared ) 
		      {
			if( debug & 1 )
			{
			  fprintf(logFile,"cutHoles: no hole cut for non-invert pt (%i,%i,%i) on grid=%i by grid=%i ",
				 ia(i,0),ia(i,1),ia(i,2),grid2,grid);
			  if( cosAngle < maxCosAngle )
			    fprintf(logFile,"since n.n=%7.2e <%7.2e \n",cosAngle,maxCosAngle);
			  else if(distToBndry>maximumHoleCuttingDistanceSquared )
			    fprintf(logFile,"since distToBndry=%7.2e >maximumHoleCuttingDistance=%7.2e\n",
				    SQRT(distToBndry),SQRT(maximumHoleCuttingDistanceSquared));
                          else 
			    fprintf(logFile,"since distToBndry=%7.2e > dx*2.=%7.2e j=(%i,%i,%i) jp=(%i,%i,%i)\n",
                             SQRT(distToBndry),SQRT(dx2*maxDistFactor),j1,j2,j3,j1p,j2p,j3p);
			}
			mask2(ia(i,0),ia(i,1),ia(i,2))=ia(i,3);   // reset these values. Do not cut a hole.
		      }
		    }
		  }
                  if( debug & 16 && numberOfDimensions==3 )
		  {
		    fprintf(logFile,"cutHoles: grid=%s, grid2=%s pt=(%i,%i,%i) r2=(%6.2e,%6.2e,%6.2e) mask=%i\n",
                       (const char*)g.mapping().getName(Mapping::mappingName),
                       (const char*)g2.mapping().getName(Mapping::mappingName),ia(i,0),ia(i,1),ia(i,2),
			    r2(i,axis1),r2(i,axis2),r2(i,axis3),mask2(ia(i,0),ia(i,1),ia(i,2)));
                    fprintf(logFile,"          x=(%6.2e,%6.2e,%6.2e) boundary shift=(%6.2e,%6.2e,%6.2e) \n",
                            x2(i,axis1),x2(i,axis2),x2(i,axis3),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)),
                            fabs(x2(i,0)-center2(ia(i,0),ia(i,1),ia(i,2),0)) );
		  }
		  if( mask2(ia(i,0),ia(i,1),ia(i,2))==0 )
		  {
		    
		    for( dir=0; dir<numberOfDimensions; dir++ )
		    {
		      holePoint(numberOfHolePoints,dir) = x2(i,dir);
		    }
                    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
                    numberOfHolePoints++;
                    if( numberOfHolePoints >= maxNumberOfHolePoints )
		    {
                      maxNumberOfHolePoints*=2;
		      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		      printf(" ... increasing maxNumberOfHolePoints to %i\n",maxNumberOfHolePoints);
		    }
		  }
		}

		where( mask2(ia(R,0),ia(R,1),ia(R,2)) <= 0 )
		  cutShare2(ia(R,0),ia(R,1),ia(R,2))=share;
	      }
	    }
	  }
	}
      }
    }
  }

  // now double check the hole cutting
  checkHoleCutting(cg);


  // we need to set values in the inverseGrid array to -1 at all unused points *** can this be done else where ????
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    const intArray & mask = g.mask();

    g.mask().periodicUpdate();   // ****

    intArray & inverseGrid = cg.inverseGrid[grid];
    getIndex(extendedGridIndexRange(g),I1,I2,I3);  
    where( mask(I1,I2,I3)==0 )
      inverseGrid(I1,I2,I3)=-1;
  }
  
  if( debug & 1 && numberOfBaseGrids>2 )
  {
    // recompute the hole points for **plotting purposes only** since it is possible that some
    // hole points have been converted back into interpolation points
    numberOfHolePoints=0;
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      MappedGrid & g = cg[grid];
      const intArray & mask = g.mask();
      realArray & center = g.center();
      const bool isRectangular = g.isRectangular();
      real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
      int iv0[3]={0,0,0}; //
      if( isRectangular )
      {
	g.getRectangularGridParameters( dvx, xab );
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  iv0[dir]=g.gridIndexRange(0,dir);
	  if( g.isAllCellCentered() )
	    xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
	}
		
      }
      #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))




      getIndex(extendedGridIndexRange(g),I1,I2,I3);  
      
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
            if( mask(i1,i2,i3)==0 )
	    {
              if( !isRectangular )
	      {
		for( dir=0; dir<numberOfDimensions; dir++ )
		  holePoint(numberOfHolePoints,dir) = center(i1,i2,i3,dir);
	      }
	      else
	      {
		for( dir=0; dir<numberOfDimensions; dir++ )
		  holePoint(numberOfHolePoints,dir) = XC(iv,dir);
	      }
	      
	      holePoint(numberOfHolePoints,numberOfDimensions)=grid;
	      
	      numberOfHolePoints++;
	      assert( numberOfHolePoints < maxNumberOfHolePoints );
	    }
	  }
	}
      }
      // printf("*** number of hole points = %i \n",numberOfHolePoints);
      
    }
    
  }
  
  delete [] cutShare;
  
  real time=getCPU();
  if( info & 2 ) 
    printF(" time to cut holes........................................%e (total=%e)\n",time-time0,time-totalTime);
  timeCutHoles=time-time0;
  
  return numberOfHolePoints;
}



int Ogen::
removeExteriorPointsNew(CompositeGrid & cg, 
		     const bool boundariesHaveCutHoles /* = FALSE */ )
// ============================================================================================================
// /Description:
//   Once the hole boundary has been determined sweep out all remaining hole points.
//  This routine assumes that the boundary of the hole partitions the domain into
//  separate regions. Points inside should be bounded by a layer of interpolation points, mask(i1,i2,i3)<0,
//   and points outside should have a layer of holes, mask(i1,i2,i3)==0. Thus this routine
//  will look for places where interpolation points are next to hole points. This will signal
//  the start or end of the hole region.
//  
// ============================================================================================================
{
  real time0=getCPU();
  
  if( info & 4 ) printf("removing exterior points by sweeping...\n");

  const int np=max(1,Communication_Manager::numberOfProcessors());
  
  int grid;
  const int numberOfDimensions=cg.numberOfDimensions();
  const int numberOfBaseGrids=cg.numberOfBaseGrids();

  Index Iv[3];
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R,Rx(0,numberOfDimensions-1), I;
  // realArray x;
  // IntegerArray ia;

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int ivp[3];
  
  // *wdh* 981001 int maxNumberOfHolePoints=10000*numberOfBaseGrids*numberOfDimensions;
  // estimate the total number of hole points in terms of the total number of grid points.
  // guess that the number of holes points is proportional to the surface area of the grid boundaries.  
  int numberOfSurfacePoints=0;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
    numberOfSurfacePoints+=(int)pow((real)cg[grid].mask().elementCount(),
                               (numberOfDimensions-1.)/numberOfDimensions);
  
  int maxNumberOfHolePoints=numberOfSurfacePoints*numberOfDimensions*2;

  if( holePoint.getLength(0)<maxNumberOfHolePoints )
    holePoint.resize(maxNumberOfHolePoints,numberOfDimensions+1);
  else
    maxNumberOfHolePoints=holePoint.getLength(0);
  
  // For cell centred grids copy the mask from the last cell to the first ghost cell as
  // this info is used by the ray-tracing algorithm
  if( cg[0].isAllCellCentered() )
  {
    Index I1g,I2g,I3g;
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      MappedGrid & g = cg[grid];
      intArray & maskd = g.mask();
      GET_LOCAL(int,maskd,mask);

      if( g.isAllCellCentered() )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
          if( g.boundaryCondition()(End,axis)>0 )
	  {
	    getGhostIndex(g.gridIndexRange(),End,axis,I1 ,I2 ,I3 ,-1);  // last row of interior cells
	    getGhostIndex(g.gridIndexRange(),End,axis,I1g,I2g,I3g,0);   // first row of ghost cells

	    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
            ok = ok && ParallelUtility::getLocalArrayBounds(maskd,mask,I1g,I2g,I3g);
	    if( !ok ) continue;  // no pts on this processor

	    mask(I1g,I2g,I3g)=mask(I1,I2,I3);
	  }
	}
      }
    }
  }


  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    intArray & maskd = g.mask();
    // realArray & center = g.center();

    const bool isRectangular = g.isRectangular();

    GET_LOCAL(int,maskd,mask);
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.center(),center);
    #else
      const realSerialArray & center = g.center();
    #endif

    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      g.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=g.gridIndexRange(0,dir);
	if( g.isAllCellCentered() )
	{
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
	}
      }
		
    }
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

    if( info & 4 ) 
      printf("removing points on grid %s... \n",(const char*)g.mapping().getName(Mapping::mappingName));

    int axis;
    getIndex(extendedGridIndexRange(g),I1,I2,I3);  

    int includeGhost=1;  // we must include ghost pts so that holes can cross processor boundaries *wdh* 081003
    bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3,includeGhost);  
    
    bool manualHolesCut=false;
    if( ok )
    {
      if( numberOfManualHoles>0 )
      {
	manualHolesCut = min(abs(manualHole(Range(numberOfManualHoles),0)-grid))==0;
      }
      // printf(" *** grid=%i manualHolesCut=%i\n",grid,manualHolesCut);
    }
    
    // *wdh* 081003 : update added but is this needed? 
    maskd.updateGhostBoundaries();
    
    // In parallel we may need to sweep multiple times 
    const int maxSweeps = np; // this is the worst case 
    int numberOfSweepsNeeded=0;
    for( int sweep=0; sweep<maxSweeps; sweep++ )
    {
      const int numberOfHolePointsOld=numberOfHolePoints; // we check if new hole points were added.

      if( ok )
      {

	const int I1Base=I1.getBase(), I1Bound=I1.getBound();
	const int I2Base=I2.getBase(), I2Bound=I2.getBound();
	const int I3Base=I3.getBase(), I3Bound=I3.getBound();

	int mask0;
    
	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	{
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  {
	    for( i1=I1Base+1; i1<=I1Bound; i1++ )
	    {
	      mask0=mask(i1-1,i2,i3);
	      while( i1<I1Bound && mask(i1,i2,i3)==mask0 )
		i1++;
	      if( (mask(i1-1,i2,i3)<0 && mask(i1,i2,i3)==0) ||
		  (manualHolesCut &&  mask(i1-1,i2,i3)==0 && mask(i1,i2,i3)>=0) )
	      { // sweep to the right
		if( !manualHolesCut ) i1++;
		while( i1<=I1Bound && mask(i1,i2,i3)>=0 )
		{
		  if( mask(i1,i2,i3)>0 )
		  {
		    mask(i1,i2,i3)=0;
		    if( !isRectangular )
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
		    }
		    else
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = XC(iv,axis);
		    }
		
		    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		
		    numberOfHolePoints++;
		    if( numberOfHolePoints>=maxNumberOfHolePoints )
		    {
		      maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		    }
		  }
		  i1++;
		}
	      }
	      else if( (mask(i1-1,i2,i3)==0 && mask(i1,i2,i3)<=0) ||
		       (manualHolesCut &&  mask(i1-1,i2,i3)>=0 && mask(i1,i2,i3)==0) )
	      { // sweep to left
		// there may be initial points that we need to sweep out from right to left
		int i= i1-2;
		if( manualHolesCut ) i=i1-1;
		while( i>=I1Base && mask(i,i2,i3)>=0 )
		{
		  if( mask(i,i2,i3)>0 )
		  {
		    mask(i,i2,i3)=0;
		    if( !isRectangular )
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = center(i,i2,i3,axis);
		    }
		    else
		    {
		      ivp[0]=i, ivp[1]=i2, ivp[2]=i3;  
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = XC(ivp,axis);
		    }
		
		    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		    numberOfHolePoints++;
		    if( numberOfHolePoints>=maxNumberOfHolePoints )
		    {
		      maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
                      if( numberOfIncreasingNumberOfHoleWarnings<=5 )
		      {
			numberOfIncreasingNumberOfHoleWarnings++;
			printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			       maxNumberOfHolePoints);
                        if( numberOfIncreasingNumberOfHoleWarnings==5 )
			{
			  printf("CutHoles:Info: too many increasing the maximum number of holes points messages. I will not print anymore\n");
			}
		      }
		    }
		  }
		  i--;
		}
	      }
	    }
	  }
	}

	// sweep in the i2 direction
	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )   // sweep i2
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    for( i2=I2Base+1; i2<=I2Bound; i2++ )
	    {
	      mask0=mask(i1,i2-1,i3);
	      while( i2<I2Bound && mask(i1,i2,i3)==mask0 )
		i2++;
	      if( mask(i1,i2-1,i3)==0 && mask(i1,i2,i3)>0 )
	      { // sweep up
		while( i2<=I2Bound && mask(i1,i2,i3)>0 )
		{
		  mask(i1,i2,i3)=0;

		  if( !isRectangular )
		  {
		    for( axis=0; axis<numberOfDimensions; axis++ )
		      holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
		  }
		  else
		  {
		    for( axis=0; axis<numberOfDimensions; axis++ )
		      holePoint(numberOfHolePoints,axis) = XC(iv,axis);
		  }
	      
		  holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		  numberOfHolePoints++;
		  if( numberOfHolePoints>=maxNumberOfHolePoints )
		  {
		    maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		    holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		    if( numberOfIncreasingNumberOfHoleWarnings<=5 )
		    {
		      numberOfIncreasingNumberOfHoleWarnings++;
		      printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			     maxNumberOfHolePoints);
		      if( numberOfIncreasingNumberOfHoleWarnings==5 )
		      {
			printf("CutHoles:Info: too many increasing the maximum number of holes points messages. I will not print anymore\n");
		      }
		    }
		  }
		  i2++;
		}
	      }
	      else if( mask(i1,i2-1,i3)>0 && mask(i1,i2,i3)==0 )
	      {
		// sweep down
		int i=i2-1;
		while( i>=I2Base && mask(i1,i,i3)>0 )
		{
		  mask(i1,i,i3)=0;
		  if( !isRectangular )
		  {
		    for( axis=0; axis<numberOfDimensions; axis++ )
		      holePoint(numberOfHolePoints,axis) = center(i1,i,i3,axis);
		  }
		  else
		  {
		    ivp[0]=i1, ivp[1]=i, ivp[2]=i3;
		    for( axis=0; axis<numberOfDimensions; axis++ )
		      holePoint(numberOfHolePoints,axis) = XC(ivp,axis);
		  }
	      
		  holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		  numberOfHolePoints++;
		  if( numberOfHolePoints>=maxNumberOfHolePoints )
		  {
		    maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		    holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		    if( numberOfIncreasingNumberOfHoleWarnings<=5 )
		    {
		      numberOfIncreasingNumberOfHoleWarnings++;
		      printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			     maxNumberOfHolePoints);
		      if( numberOfIncreasingNumberOfHoleWarnings==5 )
		      {
			printf("CutHoles:Info: too many increasing the maximum number of holes points messages. I will not print anymore\n");
		      }
		    }
		  }
		  i--;
		}
	      }
	    }
	  }
	}
    
	// sweep in the i3 direction
	if( numberOfDimensions==3 )
	{
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )   // sweep i3
	  {
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      for( i3=I3Base+1; i3<=I3Bound; i3++ )
	      {
		mask0=mask(i1,i2,i3-1);
		while( i3<I3Bound && mask(i1,i2,i3)==mask0 )
		  i3++;
		if( mask(i1,i2,i3-1)==0 && mask(i1,i2,i3)>0 )
		{ // sweep up
		  while( i3<=I3Bound && mask(i1,i2,i3)>0 )
		  {
		    mask(i1,i2,i3)=0;
		    if( !isRectangular )
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
		    }
		    else
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = XC(iv,axis);
		    }
		
		    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		    numberOfHolePoints++;
		    if( numberOfHolePoints>=maxNumberOfHolePoints )
		    {
		      maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		      if( numberOfIncreasingNumberOfHoleWarnings<=5 )
		      {
			numberOfIncreasingNumberOfHoleWarnings++;
			printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			       maxNumberOfHolePoints);
			if( numberOfIncreasingNumberOfHoleWarnings==5 )
			{
			  printf("CutHoles:Info: too many increasing the maximum number of holes points messages. I will not print anymore\n");
			}
		      }
		    }
		    i3++;
		  }
		}
		else if( mask(i1,i2,i3-1)>0 && mask(i1,i2,i3)==0 )
		{
		  // sweep down
		  int i=i3-1;
		  while( i>=I3Base && mask(i1,i2,i)>0 )
		  {
		    mask(i1,i2,i)=0;

		    mask(i1,i2,i3)=0;
		    if( !isRectangular )
		    {
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = center(i1,i2,i,axis);
		    }
		    else
		    {
		      ivp[0]=i1, ivp[1]=i2, ivp[2]=i;
		      for( axis=0; axis<numberOfDimensions; axis++ )
			holePoint(numberOfHolePoints,axis) = XC(ivp,axis);
		    }
		
		    holePoint(numberOfHolePoints,numberOfDimensions)=grid;
		    numberOfHolePoints++;
		    if( numberOfHolePoints>=maxNumberOfHolePoints )
		    {
		      maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		      holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
		      if( numberOfIncreasingNumberOfHoleWarnings<=5 )
		      {
			numberOfIncreasingNumberOfHoleWarnings++;
			printf("CutHoles:Info: increasing the maximum number of holes points to %i\n",
			       maxNumberOfHolePoints);
			if( numberOfIncreasingNumberOfHoleWarnings==5 )
			{
			  printf("CutHoles:Info: too many increasing the maximum number of holes points messages. I will not print anymore\n");
			}
		      }
		    }
		    i--;
		  }
		}
	      }
	    }
	  }
	}
       
      } // if ok

      #ifdef USE_PPP
        
	maskd.updateGhostBoundaries();

        int numSwept = numberOfHolePoints - numberOfHolePointsOld;
	if( debug & 4 )
	{
	  fprintf(plogFile,"remove exterior points: sweep=%i, numSwept=%i\n",sweep,numSwept);
	}
        // One the first sweep it could be that NO new points are swept but we still need to sweep again: 
        if( numSwept==0 && sweep==0 ) numSwept = numberOfHolePoints;
        numSwept=ParallelUtility::getMaxValue(numSwept);
        if( numSwept==0 )
	{
          numberOfSweepsNeeded=sweep+1;
          break;
	}
     #endif
    } // end for sweep
    #ifdef USE_PPP
      assert( numberOfSweepsNeeded<=maxSweeps );
    #endif

    if( debug & 8 )
    {
      displayMask(maskd,sPrintF("mask after remove exterior points, grid=%i",grid),logFile);

      // intSerialArray mask; getLocalArrayWithGhostBoundaries(cg[grid].mask(),mask);
      // displayMask(mask,sPrintF("mask after cut holes, grid=%i",grid),plogFile);
    }

  } // for grid
  
  
  real time=getCPU();
  if( info & 2 ) 
    printF(" time to remove exterior points...........................%e (total=%e)\n",time-time0,time-totalTime);
  timeRemoveExteriorPoints=time-time0;
  

  return numberOfHolePoints;
}


int Ogen::
sweepOutHolePoints(CompositeGrid & cg )
// ============================================================================================================
// /Description:
//   Once the hole boundary has been determined sweep out more hole points.
//  This version incrementally expands the hole points to allow the user to 
//  see where any mistakes might occur.
//  
// ============================================================================================================
{
  real time0=getCPU();
  
  if( info & 4 ) printf("incrementally sweep out hole points...\n");
  assert( incrementalHoleSweep>0 );

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  Index Iv[3];
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  int grid,axis;
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];

  // *wdh* 981001 int maxNumberOfHolePoints=10000*numberOfBaseGrids*numberOfDimensions;
  // estimate the total number of hole points in terms of the total number of grid points.
  // guess that the number of holes points is proportional to the surface area of the grid boundaries.  
  int numberOfSurfacePoints=0;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
    numberOfSurfacePoints+=(int)pow((real)cg[grid].mask().elementCount(),
                               (numberOfDimensions-1.)/numberOfDimensions);
  
  int maxNumberOfHolePoints=numberOfSurfacePoints*numberOfDimensions*2;

  if( holePoint.getLength(0)<maxNumberOfHolePoints )
    holePoint.resize(maxNumberOfHolePoints,numberOfDimensions+1);
  else
    maxNumberOfHolePoints=holePoint.getLength(0);
  
  // For cell centred grids copy the mask from the last cell to the first ghost cell as
  // this info is used by the ray-tracing algorithm
  if( cg[0].isAllCellCentered() )
  {
    Index I1g,I2g,I3g;
    for( grid=0; grid<numberOfBaseGrids; grid++ )
    {
      MappedGrid & g = cg[grid];
      intArray & mask = g.mask();
      if( g.isAllCellCentered() )
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
          if( g.boundaryCondition()(End,axis)>0 )
	  {
	    getGhostIndex(g.gridIndexRange(),End,axis,I1 ,I2 ,I3 ,-1);  // last row of interior cells
	    getGhostIndex(g.gridIndexRange(),End,axis,I1g,I2g,I3g,0);   // first row of ghost cells
	    mask(I1g,I2g,I3g)=mask(I1,I2,I3);
	  }
	}
      }
    }
  }

  int oldNumberOfHolesPoints=numberOfHolePoints;
  
  IntegerArray numberSwept(numberOfBaseGrids);  // number swept on last iteration
  numberSwept=1;  // force a sweep the first time
  
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    if( numberSwept(grid)==0 ) continue;
    
    MappedGrid & g = cg[grid];
    intArray & mask = g.mask();
    realArray & center = g.center();
    const bool isRectangular = g.isRectangular();
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      g.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
	iv0[dir]=g.gridIndexRange(0,dir);
	if( g.isAllCellCentered() )
	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

//    displayMask(mask,"mask before incremental cut holes");

    if( info & 4 ) 
      printf("sweeping hole points on grid %s... \n",(const char*)g.mapping().getName(Mapping::mappingName));


//    getIndex(extendedGridIndexRange(g),I1,I2,I3);  // include too many points sometimes
//     getIndex(g.dimension(),I1,I2,I3,-1);   // this didn't work - includes too many points
//    getIndex(g.gridIndexRange(),I1,I2,I3);  // this is not correct either

    // *** this needs to be fixed ****
    getIndex(g.extendedIndexRange(),I1,I2,I3,1); // include an extra line of points
    intArray holeMask(I1,I2,I3);
	  
    holeMask=-1;  // set values on extra line

    getIndex(g.extendedIndexRange(),I1,I2,I3); // include ghost points on interp boundaries

    // we may not have enough ghost points on interpolation boundaries 
    // --> reduce Iv and treat boundaries as a special case.
//     for( axis=0; axis<numberOfDimensions; axis++ )
//     {
//       if( g.boundaryCondition(0,axis)<0 )
//       {
//       }
//     }
    


    int numberOfSweeps=incrementalHoleSweep;
    for( int sweep=0; sweep<numberOfSweeps; sweep++ )
    {
      holeMask(I1,I2,I3)=mask(I1,I2,I3);
      
      if( g.numberOfDimensions()==2 )
      {
	holeMask(I1,I2,I3) = holeMask(I1,I2,I3)>0 && 
	  (holeMask(I1-1,I2-1,I3)==0 || holeMask(I1  ,I2-1,I3)==0  || holeMask(I1+1,I2-1,I3)==0 ||
	   holeMask(I1-1,I2  ,I3)==0 ||                               holeMask(I1+1,I2  ,I3)==0 ||
	   holeMask(I1-1,I2+1,I3)==0 || holeMask(I1  ,I2+1,I3)==0  || holeMask(I1+1,I2+1,I3)==0 );
      }
      else if( g.numberOfDimensions()==3 )
      {
	holeMask(I1,I2,I3) = holeMask(I1,I2,I3)>0 && 
	  (holeMask(I1-1,I2-1,I3-1)==0 || holeMask(I1  ,I2-1,I3-1)==0  || holeMask(I1+1,I2-1,I3-1)==0 ||
	   holeMask(I1-1,I2  ,I3-1)==0 || holeMask(I1  ,I2  ,I3-1)==0  || holeMask(I1+1,I2  ,I3-1)==0 ||
	   holeMask(I1-1,I2+1,I3-1)==0 || holeMask(I1  ,I2+1,I3-1)==0  || holeMask(I1+1,I2+1,I3-1)==0 ||
	   holeMask(I1-1,I2-1,I3  )==0 || holeMask(I1  ,I2-1,I3  )==0  || holeMask(I1+1,I2-1,I3  )==0 ||
	   holeMask(I1-1,I2  ,I3  )==0 ||                                 holeMask(I1+1,I2  ,I3  )==0 ||
	   holeMask(I1-1,I2+1,I3  )==0 || holeMask(I1  ,I2+1,I3  )==0  || holeMask(I1+1,I2+1,I3  )==0 ||
	   holeMask(I1-1,I2-1,I3+1)==0 || holeMask(I1  ,I2-1,I3+1)==0  || holeMask(I1+1,I2-1,I3+1)==0 ||
	   holeMask(I1-1,I2  ,I3+1)==0 || holeMask(I1  ,I2  ,I3+1)==0  || holeMask(I1+1,I2  ,I3+1)==0 ||
	   holeMask(I1-1,I2+1,I3+1)==0 || holeMask(I1  ,I2+1,I3+1)==0  || holeMask(I1+1,I2+1,I3+1)==0 );
      }

//       if( grid==0 )
//       {
// 	displayMask(mask,"mask");
// 	display(holeMask,"holeMask","%2i");
//       }
      
      // bool done=FALSE;
	  
      const int I1Base=I1.getBase(), I1Bound=I1.getBound();
      const int I2Base=I2.getBase(), I2Bound=I2.getBound();
      const int I3Base=I3.getBase(), I3Bound=I3.getBound();

      int numSwept=0;
      for( i3=I3Base; i3<=I3Bound; i3++ )
      {
	for( i2=I2Base; i2<=I2Bound; i2++ )
	{
	  for( i1=I1Base; i1<=I1Bound; i1++ )
	  {
	    if( holeMask(i1,i2,i3)==1 )
	    {
              numSwept++;
	      mask(i1,i2,i3)=0;
              if( !isRectangular )
	      {
		for( axis=0; axis<numberOfDimensions; axis++ )
		  holePoint(numberOfHolePoints,axis) = center(i1,i2,i3,axis);
	      }
	      else
	      {
		for( axis=0; axis<numberOfDimensions; axis++ )
		  holePoint(numberOfHolePoints,axis) = XC(iv,axis);
	      }
              holePoint(numberOfHolePoints,numberOfDimensions)=grid;
	      numberOfHolePoints++;
	      if( numberOfHolePoints>=maxNumberOfHolePoints )
	      {
		maxNumberOfHolePoints=int(maxNumberOfHolePoints*1.5);
		holePoint.resize(maxNumberOfHolePoints,holePoint.getLength(1));
	      }
	    }
	  }
	}
      }
      numberSwept(grid)=numSwept;
      if( numSwept==0 )break;

    } // end for sweep

  }  // for grid
  

  real time=getCPU();
  if( info & 2 ) 
    printF(" time to sweep exterior points...........................%e (total=%e)\n",time-time0,time-totalTime);
  timeRemoveExteriorPoints+=(time-time0);
  

  return numberOfHolePoints-oldNumberOfHolesPoints;
}

