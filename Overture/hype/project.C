#include "HyperbolicMapping.h"
#include "DataPointMapping.h"
#include "TridiagonalSolver.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"
#include "CompositeTopology.h"
#include "ReparameterizationTransform.h"
#include "arrayGetIndex.h"

#include "MappingProjectionParameters.h"
#include "MatchingCurve.h"

int HyperbolicMapping::
project( const realArray & x, 
         const int & marchingDirection,
         realArray & xr, 
         const bool & setBoundaryConditions /* = true */,
         bool initialStep,  /* =false */
         int stepNumber /* = 0 */ )
//===========================================================================
/// \param Access: protected.
/// \details 
///    Project points onto the surface Mapping. 
/// 
/// \param x (input) : current front. NOTE: only the points correponding
///     to the indexRange are projected. You should call applyBoundaryCondtions after this routine.
/// \param marchingDirection (input) : -1, 0 or +1 
/// \param xr (output) : The surface normal is returned as xr(.,.,.,0:2,axis2). These are valid
///     up to and including the ghost points.
/// \param setBoundaryConditions (input) : if true apply boundary conditions to xr
/// \param initialStep (input) : if true this is an initial step -- we assume no initial guess for the
///    location of the points on the reference surface. 
/// 
//===========================================================================
{
  real time0 = getCPU();
  
  assert(surface!=NULL);

  const int i3 = x.getBase(2);
  
  if( Mapping::debug & 2 )
  {
    printf("\n=======================project called for grid line i3=%i ==================================\n",i3);
  }
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 
  // project ghost points on non-periodic edges 
  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    int xBase =Iv[axis].getBase();
    int xBound=Iv[axis].getBound();
    if( boundaryCondition(Start,axis)>= 0 && projectGhostPoints(Start,axis) )
      xBase=max(x.getBase(axis),xBase-1);
    if( boundaryCondition(End  ,axis)>= 0 && projectGhostPoints(End  ,axis) )
      xBound=min(x.getBound(axis),xBound+1);

    Iv[axis]=Range(xBase,xBound);
  }
  
  Range xAxes(0,rangeDimension-1);

  Range R = I1;
 
  if( Mapping::debug & 4 )
    ::display(x(I1,I2,i3,xAxes),"project: x before project");

  realArray xx(R,3);
  
  for( axis=0; axis<rangeDimension; axis++ )
    xx(R,axis)=x(R,0,i3,axis);

  // We keep 3 different MappingProjectionParameters, one for forward, backward and one for computing
  // the initial step.
  MappingProjectionParameters & mpParams = initialStep ? surfaceMappingProjectionParameters[2] :
    marchingDirection==-1 ? surfaceMappingProjectionParameters[0] :surfaceMappingProjectionParameters[1];

  typedef MappingProjectionParameters MPP;
  realArray & surfaceNormal= mpParams.getRealArray(MPP::normal);
  
  if( surfaceNormal.dimension(0)!=R ) 
  {
    // for a composite surface surfaceNormal holds the previous normal on input to project
    mpParams.reset();
    surfaceNormal.redim(R,3);
    surfaceNormal=0.;       

  }

  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  intArray & elementIndex = mpParams.getIntArray(MPP::elementIndex);
  realArray & xOld      = mpParams.getRealArray(MPP::x);
  if( initialStep ) 
  {
    // if we are starting from a corner we force the project function to start from no initial guess
    // so it does a more careful search
    if( debug & 1 ) printf("project: initialStep=true, set initial guess: subSurfaceIndex==-1\n");
    
    subSurfaceIndex=-1;  // no need to do both
    elementIndex=-1;
  }
  

  bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
  
  if( useTriangulation && isCompositeSurface  )
  {
    // use the triangulation associated with the reference surface.
    CompositeTopology *compositeTopology = ((CompositeSurface*)surface)->getCompositeTopology();
    if( compositeTopology!=NULL )
    {
      UnstructuredMapping *uns=compositeTopology->getTriangulation();
      if( uns!=NULL )
      {
        if( debug & 1 )
  	  printf("Project onto the unstructured surface. initialStep=%i marchingDirection=%i\n",initialStep,
               marchingDirection);
	
        // ***** project onto the triangulation *****
        mpParams.setOnlyChangePointsAdjustedForCornersWhenMarching(true);
	uns->project(xx,mpParams);
        mpParams.setOnlyChangePointsAdjustedForCornersWhenMarching(false);

	
        // *** We should not correct the initial curve if it is chosen from picked points!! since
        //  the correct assumes that the curve lies along edges of triangles (if there is a triangulation)
        // *wdh* 081102 -- we should not correct the initial curve if it is a curve built on the surface!
	if( initialStep && correctProjectionOfInitialCurves && 
	    initialCurveOption!=initialCurveFromCurveOnSurface ) // *wdh* 020913
	{
          correctProjectionOfInitialCurve(xx, xr, (CompositeSurface&)(*surface),marchingDirection,mpParams);
	}
	else if( initialStep )
	{
          // ************** check this ********************
          // we need to fill in xr for plotting the direction arrows
          const int xBase=xx.getBase(0), xBound=xx.getBound(0);
          Range I(xBase,xBound-1);
          for( int axis=0; axis<rangeDimension; axis++ )
	  {
  	    xr(I,0,0,axis,0)=xx(I+1,axis)-xx(I,axis);
	    xr(xBound,0,0,axis,0)=xx(xBound,axis)-xx(xBound-1,axis);
	  }
	  
	}
	

	if( projectOntoReferenceSurface )
	{
	  // project the points lying on the triangulation onto the reference surface
	  // ** finish this **
	  if( debug & 4 ) printf("Project points onto the reference CompositeSurface\n");

	  // get the original points **** shouldn't do this if we have gone around a corner ****
          if( false  )
	  {
	    for( axis=0; axis<rangeDimension; axis++ )
	      xx(R,axis)=x(R,0,i3,axis);
	  }

	  const intArray & elementSurface = uns->getTags();
	  const int numberOfElements = uns->getNumberOfElements();
          if( numberOfElements==0 )
	  {
	    printf("hype:project:WARNING: There are no triangles on the gloabl triangulation for the CompositeSurface!\n");
	    return 1;
	  }

	  CompositeSurface & cs = *((CompositeSurface*)surface);
	  const int numberOfSubSurfaces = cs.numberOfSubSurfaces();
	  const int rBound=R.getBound();
	  realArray x0(1,3), r0(1,2), xr0(1,3,2);
	  r0=-1;
	  for( int i=R.getBase(); i<=rBound; i++ )
	  {
	    // *** we should collect up all points that belong to a given surface

	    int e = elementIndex(i);  // this is the element we are in!
	    if( e<0 || e>numberOfElements ) //assert( e>=0 && e<numberOfElements );
            {
	      printf("hype:project:ERROR: element number e=%i is not valid to project onto a CompositeSurface\n");
	      printf(" i=%i, xx(i,.)=(%8.2e,%8.2e,%8.2e) \n",i,xx(i,0),xx(i,1),xx(i,2));
              return 1;
	      // Overture::abort();
	    }
	    
	    const int s=elementSurface(e);
            subSurfaceIndex(i)=s;  
	  
	    if( s>=0 && s<numberOfSubSurfaces )
	    {
	      // collect up all consecutive points that belong to this surface
	      int iStart=i, iEnd=i;
	      for( int j=i+1; j<=rBound; j++ )
	      {
		int e2=elementIndex(j);
		assert( e2>=0 && e2<numberOfElements );
		if( elementSurface(e2)==s )
		{
		  iEnd=j;
		  i++;
		}
                else
		{
		  break;
		}
	      }
	      if( debug & 4 ) printf("project points [%i,%i] onto surface %i (i=%i)\n",iStart,iEnd,s,i);
	    
	      Range J(iStart,iEnd), J0=iEnd-iStart+1;
	      x0.redim(J0,3);
	      r0.redim(J0,2);
	      xr0.redim(J0,3,2);

	      x0(J0,0)=xx(J,0); x0(J0,1)=xx(J,1); x0(J0,2)=xx(J,2); 

	      // cs[s].project(x0,mpParams2);
	    
	      r0=-1; // We could get an initial guess if we knew the r coordinates of the triangulation
              
              Mapping *mapPointer = &cs[s];
	      if( cs[s].getClassName()=="TrimmedMapping" )
		mapPointer = ((TrimmedMapping&)cs[s]).untrimmedSurface();

	      Mapping & subSurface = *mapPointer;

	      subSurface.inverseMap(x0,r0);
	      if( max(fabs(r0)) < 2. )
	      {

		subSurface.map(r0,x0,xr0);
                
		
		    // ::display(r0, "r0, before project");
		// ::display(x00, "x0, before project");
		// ::display(x0, "x0, after project");
		

		xx(J,0)=x0(J0,0); xx(J,1)=x0(J0,1); xx(J,2)=x0(J0,2); 

		// *** we need to compute normals too ****

		surfaceNormal(J,axis1)=xr0(J0,1,0)*xr0(J0,2,1)-xr0(J0,2,0)*xr0(J0,1,1);
		surfaceNormal(J,axis2)=xr0(J0,2,0)*xr0(J0,0,1)-xr0(J0,0,0)*xr0(J0,2,1);
		surfaceNormal(J,axis3)=xr0(J0,0,0)*xr0(J0,1,1)-xr0(J0,1,0)*xr0(J0,0,1);

		realArray norm;
		norm = SQRT( SQR(surfaceNormal(J,0))+SQR(surfaceNormal(J,1))+SQR(surfaceNormal(J,2))  ) ;

		norm=cs.getSignForNormal(s)/max(REAL_MIN,norm);
    
		int dir;
		for( dir=0; dir<rangeDimension; dir++ ) 
		  surfaceNormal(J,dir)*=norm;
	      }
	      else
	      {
		printf("***ERROR: inverting sub-surface %i\n",s);
	      }
	    }
	    else
	    {
	      printf("***ERROR: invalid sub-surface, s=%i, numberOfSubSurfaces=%i\n",s,numberOfSubSurfaces);
	    }
	  }
	}
      }
      else  // there is no unstructured grid to use
      {
	useTriangulation=false;
	surface->project(xx,mpParams);
      }
    }
    else
    {
      useTriangulation=false;
      surface->project(xx,mpParams);
    }
  }
  else
  {
    surface->project(xx,mpParams);
  }
  
  if( initialStep || surfaceNormal.dimension(0)!=R ) // **** fix this **** no need to recompute
  {
    // set the parameters for the reverse or forward marching equal to the initial curve
    if( marchingDirection==-1 )
      surfaceMappingProjectionParameters[0]=surfaceMappingProjectionParameters[2];
    else
      surfaceMappingProjectionParameters[1]=surfaceMappingProjectionParameters[2];
  }

//   if( true )
//   {
//     intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
//     ::display(subSurfaceIndex,"subSurfaceIndex after");
//   }

  for( axis=0; axis<rangeDimension; axis++ )
  {
    xOld(R,axis)=xx(R,axis);  // save current value *wdh* 020930 Is this needed?

    x(R,0,i3,axis)=xx(R,axis);
    xr(R,0,0,axis,axis2)=surfaceNormal(R,axis);
  }


  if( setBoundaryConditions )
  {
    // boundary conditions in the periodic case
    int i1a = gridIndexRange(Start,axis1);
    int i1b = gridIndexRange(End,axis1);
    for( int side=Start; side<=End; side++ )
    {
      if( (bool)getIsPeriodic(axis1) )
      {
	xr(i1a-1,0,0,xAxes,1)=xr(i1b-1,0,0,xAxes,1);
	xr(i1b  ,0,0,xAxes,1)=xr(i1a  ,0,0,xAxes,1);
	xr(i1b+1,0,0,xAxes,1)=xr(i1a+1,0,0,xAxes,1);
      }
      else
      {
	xr(i1a-1,0,0,xAxes,1)=xr(i1a,0,0,xAxes,1);
	xr(i1b+1,0,0,xAxes,1)=xr(i1b,0,0,xAxes,1);
      }
      
    }
  }
  
  if( Mapping::debug & 4 )
    ::display(x(R,0,i3,xAxes),"project: x after project");

  if( Mapping::debug & 2 || Mapping::debug & 8 )
    ::display(surfaceNormal,sPrintF("project: surfaceNormal after project (step=%i)",stepNumber),debugFile,"%10.4e ");
  
  timing[timeForProject]+=getCPU()-time0;
  return 0;
}

int HyperbolicMapping::
correctProjectionOfInitialCurve(realArray & x, 
                                realArray & xr,
                                CompositeSurface & cs,
                                const int & marchingDirection,
                                MappingProjectionParameters & mpParams)
//===========================================================================
/// \param Access: protected.
/// \brief  
///     After projecting the initial curve onto a corner we decide which side of the
///   corner we should actually be on depending on which way we are marching.
///  
/// \param xr (output) : the derivatives along the curve are returned. These are currently only used for 
///                  generating the direction arrows on the start curve.
///  Note: This function is also used when projecting a boundary of a volume grid
///       onto a matching mapping.
//===========================================================================
{
  // **** NOTE **** surfaceNormal is not set on input to this routine anymore -- no need to set it
  // **** NOTE: xr is not assigned when this routine is called from marching (but is assigned from plotting
  //               direction arrows.
  int debugc=debug;

//  assert( surface!=NULL );
  CompositeTopology *compositeTopology = cs.getCompositeTopology();
  if( compositeTopology==NULL )
  {
    printf("HyperbolicMapping::correctProjectionOfInitialCurve:WARNING: cs.getCompositeTopology()==NULL\n");
    return 1;
  }
  if( compositeTopology->getTriangulation()==NULL )
  {
    printf("HyperbolicMapping::correctProjectionOfInitialCurve:WARNING: compositeTopology->getTriangulation()==NULL\n");
    return 1;
  }
  
  UnstructuredMapping & uns=*compositeTopology->getTriangulation();
  
  typedef MappingProjectionParameters MPP;
  const intArray & elementIndex = mpParams.getIntArray(MPP::elementIndex);
  // const intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  realArray & surfaceNormal= mpParams.getRealArray(MPP::normal);

  const intArray & elementSurface = uns.getTags();
  const int numberOfElements = uns.getNumberOfElements();
  const int numberOfSubSurfaces = cs.numberOfSubSurfaces();

  // Here is the connectivity information

  const intArray & element = uns.getElements();
  const intArray & faceElements = uns.getFaceElements();
  const intArray & ef = uns.getElementFaces();
  const intArray & face = uns.getFaces();
  const realArray & node = uns.getNodes();
  
  real x0[3], x1[3], x2[3], xb[3], xt[3], r0,s0, nv[3];
  int intersectionFace,intersectionFace2;
  
  const int maximumNumberOfWarnings=10;
  int numberOfWarnings=0;

  const int xBase =x.getBase(0);
  const int xBound=x.getBound(0);
  for( int i=xBase; i<=xBound; i++ )
  {
    int e = elementIndex(i);  // this is the element we are in!
    const int eInitial=e; // remember this
    assert( e>=0 && e<numberOfElements );
    int s=elementSurface(e);
	  
    if( s>=0 && s<numberOfSubSurfaces )
    {
      
      // Determine if we are near a corner between two subsurfaces -- i.e. is there a
      // nearby edge (face) that separates two different subsurfaces and is nearly parallel
      // to the initial curve.

      int n0=element(e,0), n1=element(e,1), n2=element(e,2); // assumes triangles
      x0[0]=node(n0,0), x0[1]=node(n0,1), x0[2]=node(n0,2);
      x1[0]=node(n1,0), x1[1]=node(n1,1), x1[2]=node(n1,2);
      x2[0]=node(n2,0), x2[1]=node(n2,1), x2[2]=node(n2,2);

      xb[0]=x(i,0), xb[1]=x(i,1), xb[2]=x(i,2);   // we should use the original points

      // determine the nearest node
      real dist0=SQR(x0[0]-xb[0])+SQR(x0[1]-xb[1])+SQR(x0[2]-xb[2]);
      real dist1=SQR(x1[0]-xb[0])+SQR(x1[1]-xb[1])+SQR(x1[2]-xb[2]);
      real dist2=SQR(x2[0]-xb[0])+SQR(x2[1]-xb[1])+SQR(x2[2]-xb[2]);

      const int nearestNode = dist0<=min(dist1,dist2) ? n0 : dist1<=dist2 ? n1 : n2;

      if( debugc & 1 ) 
	printf("correctProject step: pt i=%i, e=%i subSurface =%i, nodes=[%i,%i,%i] nearest=%i \n",
             i,e,s,n0,n1,n2,nearestNode);


      // intersectionFace : face we are closest to
      //  intersectionFace2 : near a corner the next closest face
      bool inside;
      inside=UnstructuredMapping::projectOnTriangle(x0,x1,x2, xb,xb,xt,intersectionFace,intersectionFace2,r0,s0);
      if( inside )
      {
        // we need to define intersectionFace,intersectionFace2 in this case since we are inside.
        const real tol=.1;
	if( fabs(s0)<fabs(r0) )
	{
	  intersectionFace=0;
          if( fabs(r0)>.5 )
  	    intersectionFace2=1;
          else
  	    intersectionFace2=2;
	}
        else
	{
	  intersectionFace=2;
          if( fabs(s0)>.5 )
  	    intersectionFace2=1;
          else
  	    intersectionFace2=0;
	}
        // The two closest faces should connect to the nearestNode
        int f1=ef(e,intersectionFace), f2=ef(e,intersectionFace2);
        if( face(f1,0)!=nearestNode && face(f1,1)!=nearestNode )
	{
	  numberOfWarnings++;
	  if( numberOfWarnings<maximumNumberOfWarnings )
	  {
	    printF("correctProjectionOfInitialCurve:WARNING: i=%i intersectionFace=%i f1=%i nodes=(%i,%i) "
                   "does not have nearestNode=%i -- changing it\n",i,intersectionFace,f1,face(f1,0),face(f1,1),
                   nearestNode );
	  }
	  else if( numberOfWarnings==maximumNumberOfWarnings )
	  {
            printF("\n *** correctProjectionOfInitialCurve:INFO: too many warning messages. You may want to turn off\n"
                   " `correct projection of initial curve' if the initial curve is not correct ***\n\n");
	  }
	  
	  intersectionFace= (intersectionFace==0 ? (intersectionFace2==1 ? 2 : 1) :
                             intersectionFace==1 ? (intersectionFace2==2 ? 0 : 2) :
			     (intersectionFace2==0 ? 1 : 0) );
	}
        if( face(f2,0)!=nearestNode && face(f2,1)!=nearestNode )
	{
          numberOfWarnings++;
	  if( numberOfWarnings<maximumNumberOfWarnings )
	  {
            printF("WARNING: i=%i intersectionFace2=%i f2=%i nodes=(%i,%i) does not have nearestNode=%i -- "
		   "changing it\n",i, intersectionFace2,f2,face(f2,0),face(f2,1),nearestNode );
	  }
	  else if( numberOfWarnings==maximumNumberOfWarnings )
	  {
            printF("\n *** correctProjectionOfInitialCurve:INFO: too many warning messages. You may want to turn off\n"
                   " `correct projection of initial curve' if the initial curve is not correct ***\n\n");
	  }
	  
	  intersectionFace2= (intersectionFace2==0 ? (intersectionFace==1 ? 2 : 1) :
                             intersectionFace2==1 ? (intersectionFace==2 ? 0 : 2) :
			     (intersectionFace==0 ? 1 : 0) );
	}
      }
      
 
      real v[3], t[3];
      // t : holds the tangent to the initial curve
      // v : holds the vector along the edge of a triangle
      //     we are looking for an edge that is parallel to t.

      // Note: we want to choose a one-sided approx to the tangent so we more closely
      // match the edge of a nearby triangle -- otherwise there is trouble if the initial
      // curve has a corner in it.
      if( i<xBound  )
      {
	t[0]=x(i+1,0)-x(i,0);
	t[1]=x(i+1,1)-x(i,1);
	t[2]=x(i+1,2)-x(i,2);
      }
      else if( i>xBase )
      {
	t[0]=x(i,0)-x(i-1,0);
	t[1]=x(i,1)-x(i-1,1);
	t[2]=x(i,2)-x(i-1,2);
      }
      else
      {
        printF("correctProjectionOfInitialCurve:WARNING: this won't work when there is only 1 point projected!\n");
        t[0]=1.;  t[1]=0.;   t[2]=0.;   // this case shouldn't matter.
      }
      real tNorm=t[0]*t[0]+t[1]*t[1]+t[2]*t[2];

      xr(i,0,0,0,0)=t[0];
      xr(i,0,0,1,0)=t[1];
      xr(i,0,0,2,0)=t[2];

      if( debugc & 2 )
      {
        real norm=1./max(REAL_MIN*100.,sqrt(tNorm));
	printF("   ...curve tangent=(%8.2e,%8.2e,%8.2e)...point %i is near iface %i (or iface2=%i) (r0=%8.2e,s0=%8.2e)"
               " e=%i s=%i\n",
               t[0]*norm,t[1]*norm,t[2]*norm,i,intersectionFace,intersectionFace2,r0,s0,e,s);

      }

      bool edgeFound=false;  // becomes true when we find a nearby edge separating two subsurfaces
      // loops over all faces that are adjacent to the nearest node; starting with face intersectionFace
      //   
      //  For nf==0 : check face intersectionFace, then other faces in the direction of intersectionFace (e.g. f1)
      //  For nf==1 : check face intersectionFace2, then other faces in the direction of intersectionFace2 (f2)
      // 
      //                                        \      /
      //                        intersectionFace \ e  /intersectionFace2
      //                                     eNew \  /  eNew2
      //                              _____f1______\/_____f2____________  curve
      // 
      int f=-1;
      int faceIndex=-1;
      int eBackup=-1;
      real tDotVBackup=0.;
      for( int it=0; it<10 && !edgeFound; it++ )  // expect at most 10 extra faces to check for each intersectionFace
      {
	for( int nf=0; nf<=1 && !edgeFound; nf++ ) // first check  intersectionFace, then intersectionFace2
	{
	  if( it==0 )
	  {
	    faceIndex=nf==0 ? intersectionFace : intersectionFace2;
            f= ef(e,faceIndex);
	  }
	  else
	  {
	    // look for a new face that has nearestNode and shares and element with the current face, f
	    int eNew=-1, fNew=-1;
	    for( int n=0; n<=1 && fNew==-1; n++ ) // 2 elements next to current face
	    {
	      eNew=faceElements(f,n);  // this element borders this face
	      if( eNew!=e && eNew>=0 )
	      {
		// find a face on eNew with node nearestNode
		for( int m=0; m<=2; m++ )
		{
		  int ff= ef(eNew,m);
		  if( ff!=f && (face(ff,0)==nearestNode || face(ff,1)==nearestNode ) )
		  {
		    fNew=ff;
		    faceIndex=m;
		    break;
		  }
		}
	      }
	    }
	    if( fNew<0 )
	    {
	      break;  // no new face found! trouble if nf==1
	    }
	    e=eNew;                // pick a new closest element
	    f=fNew;                // pick a new closest face
	    s=elementSurface(e);   
	  }

          int e1=faceElements(f,0), e2=faceElements(f,1);
          if( e2<0 ) 
	  {
            printF("WARNING: e2=faceElements(f,1)<0 e2=%i f=%i\n",e2,f);
	    // ::display(faceElements,"faceElements","%3i ");
	    continue;  // no nearby element was found, keep looking
	  }
	  
          if( e1!=0 && e2!=0 && elementSurface(e1)==elementSurface(e2) )
	  {
	    // do not check this one since the face does not lie on a boundary between two sub-surfaces
            continue;
	  }
	  

//            int interFace=it==1 ? intersectionFace : intersectionFace2;
//            int f= ef(e,interFace);  // here is a nearby face
//            int e2=-1;
//  	  for( int n=0; n<=1; n++ )
//  	  {
//  	    e2=faceElements(f,n);  // this element borders this face
//  	    if( e2!=e && e2>=0 )
//  	    {
//  	      e=e2;   // pick a new closest element
//                assert( e>=0 && e<numberOfElements );
//                s=elementSurface(e);
//   	      break;
//  	    }
//  	  }
//            if( e2<0 ) continue;  // no nearby element was found, keep looking
          // printf("  ... ++++ try looking at edges of nearby element %i\n",e);

	  n0=element(e,0), n1=element(e,1), n2=element(e,2); // assumes triangles
	  x0[0]=node(n0,0), x0[1]=node(n0,1), x0[2]=node(n0,2);
	  x1[0]=node(n1,0), x1[1]=node(n1,1), x1[2]=node(n1,2);
	  x2[0]=node(n2,0), x2[1]=node(n2,1), x2[2]=node(n2,2);

	  //    int f=ef(e,m);  // here is a face on the element e;
	  int na=element(e,faceIndex);        // we have to get the orientation correct
	  int nb=element(e,(faceIndex+1)%3);
	  v[0]=node(nb,0)-node(na,0);
	  v[1]=node(nb,1)-node(na,1);
	  v[2]=node(nb,2)-node(na,2);
	
	  real vNorm=v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
	  real tDotV=t[0]*v[0]+t[1]*v[1]+t[2]*v[2];

	  tDotV*=marchingDirection==1 ? -1. : 1.;

	  // Here we compare the direction of the tangent to the inital curve, times +1 or -1 depending
	  // on the marching direction to the direction of the oriented edge 
	  // on the triangle. The normal to the surface is out of the page. We want t and v to
	  // be in the same direction.
	  //
	  //              x2
	  //           ^  |\
	  //           |  | \
	  //           | ||  \
	  //        t  | |v   \
	  //           | v|    \
	  //           |  |     \
	  //              +------o x1
	  //              x0
	  if( fabs(tDotV) > .75*sqrt(tNorm*vNorm) )
  	  {
	    if( debugc & 2 ) printf("   ... it=%i, curve parallel to face=%i(%i,%i) na,nb=(%i,%i) (e=%i,s=%i): tDotV=%e\n",
				    it,f,face(f,0),face(f,1),na,nb,e,s,tDotV/max(REAL_MIN*10.,sqrt(tNorm*vNorm)));

	    if( tDotV<0. )
	    {
	      // choose the other surface since the orientation is backward
              e= faceElements(f,0)!=e ? faceElements(f,0) : faceElements(f,1);
	      s=elementSurface(e);
	      if( debugc & 2 ) printf("   ... **** set closest element to e=%i, sub-surface s=%i\n",e,s);

	      elementIndex(i)=e;
	    }
	    else if( e!=eInitial )
	    {
	      // we have found an edge on an element that was not equal to the original
	      if( debugc & 1 ) printf("   ... **** set closest element to e=%i, sub-surface s=%i\n",e,s);

	      elementIndex(i)=e;
	    }

	    edgeFound=true;
	    break;
	  }
	  else
	  {
            // The tangent was not parallel to the edge.
            // keep a backup value in case we find none -- choose the best backup
            if( fabs(tDotV) > fabs(tDotVBackup) )
	    {
              if( tDotV<0. )
	      {
                eBackup= faceElements(f,0)!=e ? faceElements(f,0) : faceElements(f,1);
   	        tDotVBackup=tDotV;
	      }
	      else if( e!=eInitial )
	      {
                eBackup=e;
		tDotVBackup=tDotV;
	      }
	    }

	    if( debugc & 1 ) printF("   ...it=%i curve is NOT parallel to this face %i(%i,%i): (e=%i,s=%i) "
                       "tDotV=%e faceIndex=%i na,nb=%i,%i\n",it,f,face(f,0),face(f,1),e,s,
                tDotV/max(REAL_MIN*10.,sqrt(tNorm*vNorm)),
                     faceIndex,na,nb);
	  }
	
	} // end for nf
      } // for it
      
      if( !edgeFound )
      {
	numberOfWarnings++;
	if( numberOfWarnings<maximumNumberOfWarnings )
	{
	  printF("   ...WARNING: No edge found that was tangent to the initial curve at point i=%i\n",i);
	}
	else if( numberOfWarnings==maximumNumberOfWarnings )
	{
	  printF("\n *** correctProjectionOfInitialCurve:INFO: too many warning messages. You may want to turn off\n"
		 " `correct projection of initial curve' if the initial curve is not correct *** \n\n");
	}
        if( eBackup>=0 )
	{
          if( numberOfWarnings<maximumNumberOfWarnings )
            printF("   ...using the backup edge (element eBackup=%i) tDotVBackup=%8.2e\n",eBackup,tDotVBackup);
	  elementIndex(i)=eBackup;
	}
	
      }
    } // end if( s>=0 && 
  }  // end for( i
  

// if( debugc & 2 ) printf("   ... point %i -> face %i is next to surfaces %i and %i\n",i,f,sf[0],sf[1]);


  return 0;
}



realArray HyperbolicMapping::
normalize( const realArray & u )
// ================================================================================================
// /Description:
//   Normalize the vector u(I1,I2,I3,R) over the dimension(3)=R
// ================================================================================================
{
  Range all;
  const int dim = u.getLength(3);
  assert( dim==2 || dim==3 );
  
  realArray n(u);
  
  realArray normInverse;
  if( dim==2 )
    normInverse= 1./(max(REAL_MIN,SQRT( SQR(u(all,all,all,0))+SQR(u(all,all,all,1)) ) ));
  else
    normInverse=1./(max(REAL_MIN,SQRT( SQR(u(all,all,all,0))+SQR(u(all,all,all,1))+SQR(u(all,all,all,2)) ) ));

  for( int axis=0; axis<dim; axis++ )
    n(all,all,all,axis)=u(all,all,all,axis)*normInverse;

  return n;
}




int HyperbolicMapping::
getDistanceToStep(const int & i3Delta, realArray & ds, const int & growthDirection )
//===========================================================================
/// \param Access: protected.
/// \brief  
///    Return the current grid spacing in the normal direction. 
/// \param i3Delta (input) : abs(i3-i3Start)
/// \param ds(I1,I2) (output) : distance to march for each grid point.
//===========================================================================
{
  if( spacingType==constantSpacing )
  {
    if( initialSpacing>0. && spacingOption!=spacingFromDistanceAndLines )
      ds=initialSpacing;
    else
      ds=distance[growthDirection]/max(1.,linesToMarch[growthDirection]-1);
  }
  else if( spacingType==geometricSpacing )
  {
    int step = i3Delta;
    const bool growBothDirections = fabs(growthOption) > 1;
    if( growBothDirections && growthDirection==1 )
    {
      // Note: when we march in both directions and backwards we start at the middle grid line and decrease
      //  Fixed: *wdh* 110806.
      
      step = linesToMarch[growthDirection]-i3Delta;
    }
    
    const real scaling=pow(geometricFactor,double(step));
    if( debug & 2 )
      printF("Hype:getDistanceToStep:geometricSpacing: growthDirection=%i, i3Delta=%i step=%i scaling=%8.2e\n",
	     growthDirection,i3Delta,step,scaling);

    if( initialSpacing>0. && spacingOption!=spacingFromDistanceAndLines )
    {
      ds=initialSpacing*scaling;
    }
    else    
    {
      // ds=distance[growthDirection]*pow(geometricFactor,i3Delta)*geometricNormalization[growthDirection];
      // we now define the geometricNormalization to be the initial grid spacing
      ds=geometricNormalization[growthDirection]*scaling;
      // printf("getDistanceToStep: i3Delta=%i ds=%e\n",i3Delta,ds(0));
      
    }
  }
  else if( spacingType==oneDimensionalMappingSpacing )
  {
    assert( normalDistribution!=0 );
    realArray r(1,1),t(1,1),t2(1,1);
    r=(i3Delta-1)/max(1.,linesToMarch[growthDirection]-1.);
    normalDistribution->map(r,t);
    r=i3Delta/max(1.,linesToMarch[growthDirection]-1.);
    normalDistribution->map(r,t2);
    if( initialSpacing>0. )
      ds=(t2(0,0)-t(0,0))*initialSpacing*(linesToMarch[growthDirection]-1);
    else
      ds=(t2(0,0)-t(0,0))*distance[growthDirection];
  }
  else
  {
    printf("HyperbolicMapping::getNormalGridSpacing: ERROR: unknown spacing type!\n");
    {throw "error";}
  }
  if( Mapping::debug & 4 )
    ::display(ds,"Here is ds, the step length",debugFile);

  return 0;
}

int HyperbolicMapping::
adjustDistanceToMarch(const int & numberOfAdditionalSteps, const int & growthDirection )
//===========================================================================
/// \param Access: protected.
/// \brief  
///    Increase the distance[growthDirection] values if extra marching steps are taken.
///  linesToMarch[] should be the old value.
//===========================================================================
{
  if( spacingType==constantSpacing )
  {
    // just linearly scale
    distance[growthDirection]*=(linesToMarch[growthDirection]+numberOfAdditionalSteps-1.)/
              max(1,linesToMarch[growthDirection]-1.);
  }
  else if( spacingType==geometricSpacing )
  {
    real ds = initialSpacing>0. ? initialSpacing : geometricNormalization[growthDirection];
    distance[growthDirection]=ds*pow(geometricFactor,linesToMarch[growthDirection]+numberOfAdditionalSteps-1.);

  }
  else if( spacingType==oneDimensionalMappingSpacing )
  {
    // switch to constant spacing??
    printf("adjustDistanceToMarch:ERROR: I don't know how to take more steps\n");
    Overture::abort("error");
  }
  else
  {
    printf("HyperbolicMapping::getNormalGridSpacing: ERROR: unknown spacing type!\n");
    Overture::abort("error");
  }
  return 0;
}




int HyperbolicMapping::
computeNonlinearDiffussionCoefficient(const realArray & normXr, 
				      const realArray & normXs, 
				      const realArray & normXt, 
                                      const int & direction,
                                      int stepNumber )
//===========================================================================
/// \param Access: protected.
/// \brief  
///    Compute the edge-centered uniform and upwind diffusion coefficient in a given direction.
/// \param direction (input): 
/// \param Xr, Xs, Xt (input) : required if  upwindDissipationCoefficient!=0
/// \param lambda (output) : the diffusion coefficient.
//===========================================================================
{
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 
   
  
  int is[2]={0,0}; //
  is[direction]=1;
  
  const real dissipationCoefficient=getDissipationCoefficient(stepNumber);
  
  lambda(I1,I2)=4.*dissipationCoefficient;


  if( upwindDissipationCoefficient!=0. )
  {
    if( direction==0 )
      lambda(I1,I2)+=upwindDissipationCoefficient*normXt(I1,I2)/normXr(I1,I2);
    else
      lambda(I1,I2)+=upwindDissipationCoefficient*normXt(I1,I2)/normXs(I1,I2);
  }
  
  if( FALSE && domainDimension==2 && rangeDimension==3 &&
     (arcLengthWeight!=0. || curvatureWeight!=0. || normalCurvatureWeight!=0. ) )
  {
    // add variable coefficient diffusion (and subtract off the constan one added above).
    lambda(I1,I2)+=4.*dissipationCoefficient*(gridDensityWeight(I1,I2)-1.);
  }

  Index Ib1,Ib2,Ib3, Ie1,Ie2,Ie3;
  getBoundaryIndex(indexRange,Start,direction,Ib1,Ib2,Ib3);
  getBoundaryIndex(indexRange,End  ,direction,Ie1,Ie2,Ie3);

  if( (bool)getIsPeriodic(direction) )
    lambda(Ie1+is[0],Ie2+is[1])=lambda(Ib1,Ib2);
  else
    lambda(Ie1+is[0],Ie2+is[1])=2.*lambda(Ie1,Ie2)-lambda(Ie1-is[0],Ie2-is[1]);

  lambda(I1,I2)=.5*(lambda(I1+is[0],I2+is[1])+lambda(I1,I2));   

  if( (bool)getIsPeriodic(direction) )
  {
    lambda(Ie1+is[0],Ie2+is[1])=lambda(Ib1,Ib2);
    lambda(Ib1-is[0],Ib2-is[1])=lambda(Ie1,Ie2);
  }
  else
  {
    lambda(Ie1+is[0],Ie2+is[1])=2.*lambda(Ie1,Ie2)-lambda(Ie1-is[0],Ie2-is[1]);
    lambda(Ib1-is[0],Ib2-is[1])=2.*lambda(Ib1,Ib2)-lambda(Ib1+is[0],Ib2+is[1]);
  }
  

  return 0;
}



int HyperbolicMapping::
getCurvatureDependentSpeed(realArray & ds, 
                           realArray & kappa,
			   const realArray & xrr, 
			   const realArray & normal, 
			   const realArray & normXr, 
			   const realArray & normXs)
//===========================================================================
/// \param Access: protected.
/// \brief  
///      Change the step length to depend on the curvature.
//===========================================================================
{
  // try to choose the smoothing where there is a large "negative" curvature.
  // curvature is x_r X x_rr / |x_r|^3
  if( curvatureSpeedCoefficient==0. )
    return 0;
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 

  // compute the curvature kappa
  // in 3D take the maximum of the curvatures in the 2 parameter directions.
  real dr1 = 1./max(1,(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1)));
  if( rangeDimension==2 )
  {
    // kappa(I1,I2)=(xrr(I1,I2,0,0)*normal(I1,I2,0,0)+xrr(I1,I2,0,1)*normal(I1,I2,0,1))*(1./dr1); *wdh* 010406
    kappa(I1,I2)=(xrr(I1,I2,0,0)*normal(I1,I2,0,0)+xrr(I1,I2,0,1)*normal(I1,I2,0,1))*(1./dr1)/normXr(I1,I2);
  }
  else
  {
    kappa(I1,I2)=(xrr(I1,I2,0,0,0)*normal(I1,I2,0,0)+
		  xrr(I1,I2,0,1,0)*normal(I1,I2,0,1)+
		  xrr(I1,I2,0,2,0)*normal(I1,I2,0,2))*(1./dr1)/normXr(I1,I2);
  }
  
  if( domainDimension==3 )
  {
    real dr2 = 1./max(1,(gridIndexRange(End,axis2)-gridIndexRange(Start,axis2)));

    kappa(I1,I2)=max(kappa(I1,I2),
		     (xrr(I1,I2,0,0,1)*normal(I1,I2,0,0)+
		      xrr(I1,I2,0,1,1)*normal(I1,I2,0,1)+
		      xrr(I1,I2,0,2,1)*normal(I1,I2,0,2))*(1./dr2)/normXs(I1,I2));
  }
      
  where( kappa(I1,I2)>0. )
  {
    kappa(I1,I2)*=4.;
  }
  otherwise()
  {
    kappa(I1,I2)*=.25;
  }
  kappa(I1,I2)=max(kappa(I1,I2),-.8/curvatureSpeedCoefficient);

  jacobiSmooth( kappa,2 );

  printf("max kappa = %e \n",max(kappa));
  ds(I1,I2)*=1.+curvatureSpeedCoefficient*kappa(I1,I2);
      
  // ::display(kappa,sPrintF("*** Here is kappa at step %i",i3));
  return 0;
}

int HyperbolicMapping::
implicitSolve(realArray & xTri, 
              const int & i3Mod2,
              realArray & xr,
              realArray & xt,
	      realArray & normal,
	      realArray & normXr,
	      realArray & normXs,
	      realArray & normXt,
              TridiagonalSolver & tri,
              int stepNumber )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Solve the implicit time stepping equations.
/// 
/// \param xt (input) : 
/// \param xTri (input/output) : on input the RHS to the implicit solve.
//===========================================================================
{
#ifndef USE_PPP
  real time0=getCPU();

  // **** if implicit coefficient==0 then we do not need to create the c matrix.
  // **** if lambda==const then we only need to factor the matrix once ****

  formCMatrix(xr,xt,i3Mod2,normal,normXr,axis1);

  computeNonlinearDiffussionCoefficient(normXr,normXs,normXt,axis1,stepNumber);  // compute lambda

  formBlockTridiagonalSystem(axis1,xTri);
      
  TridiagonalSolver::SystemType systemType = 
    (bool)getIsPeriodic(axis1) ? TridiagonalSolver::periodic : TridiagonalSolver::normal;
  real time1=getCPU();
  tri.factor(at,bt,ct,systemType,axis1,rangeDimension);
     
  // ::display(xTri,"RHS xTri before step 1");
  tri.solve(xTri);
  timing[timeForTridiagonalSolve]+=getCPU()-time1;
  // ::display(xTri,"solution xTri after step 1");

  if( domainDimension==3 )
  {
    computeNonlinearDiffussionCoefficient(normXr,normXs,normXt,axis2,stepNumber); // compute lambda
    // ::display(lambda,"lambda after step 2");
    formCMatrix(xr,xt,i3Mod2,normal,normXr,axis2);

    formBlockTridiagonalSystem(axis2,xTri);
    systemType=(bool)getIsPeriodic(axis2) ? TridiagonalSolver::periodic : TridiagonalSolver::normal;

    real time1=getCPU();
    tri.factor(at,bt,ct,systemType,axis2,rangeDimension);
    tri.solve(xTri);
    timing[timeForTridiagonalSolve]+=getCPU()-time1;
    // ::display(xTri,"solution xTri after step 2");

  }

  timing[timeForImplicitSolve]+=getCPU()-time0;
#else
  Overture::abort("HyperbolicMapping:ERROR:finish me for parallel Bill!");  // Tridiagonal:: factor/solve
#endif
  
  return 0;
}

int HyperbolicMapping::
equidistributeAndStretch( const int & i3, const realArray & x, const real & weight, 
                          const int marchingDirection, bool stretchGrid /*= true */  )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Redistribute the points by equidistributing a weight function based
///   on arclength and curvature. Choose the new points by
///  \begin{verbatim}
///        x(I1,I2,i3,xAxes) = (1-weight) x(I1,I2,i3,xAxes) + weight xe(I1,I2,0,xAxes)
///  \end{verbatim}
/// \param i3 (input) :
/// \param x (input/output) :
/// \param weight (input) : 
/// \param marchingDirection : 0=start-curve (special case) 
//===========================================================================
{
  if( domainDimension!=2 )
    return 0;


  DataPointMapping dp;
//  dp.setIsPeriodic(axis1,getIsPeriodic(axis1));
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(gridIndexRange,I1,I2,I3); 
  Range xAxes(0,rangeDimension-1);

  // ::display(x(I1,0,i3,xAxes),"equidistribute: here is x(I1,0,i3,xAxes)");
  // we need to apply this BC
  if( (bool)getIsPeriodic(axis1) )
    x(I1.getBound(),0,i3,xAxes)=x(I1.getBase(),0,i3,xAxes);
    
  realArray xe(I1,xAxes);

  // *** stretching ***
  realArray r(I1,1);
  realArray rs;
  if( stretchGrid && startCurveStretchMapping!=NULL ) 
  {
    const real dr = 1./max(1,I1.getBound()-I1.getBase());
    r.seqAdd(0.,dr);
    rs.redim(I1,1);
    startCurveStretchMapping->map(r,rs);  // rs = stretched grid locations
  }

  const int numberOfMatchingCurves=matchingCurves.size();
  if( numberOfMatchingCurves>0 &&
      marchingDirection!=0  ) // we do NOT do this for the start curve since the gridLine positions need to be computed
  {
    // If we matching to an interior curve we equidistrubute on each sub-interval separately
    //   +---+---+---+---+--------------------+

    int gridLineOld=I1.getBase();
    int gridLine,curveDirection;
    for( int i=0; i<numberOfMatchingCurves+1; i++ )  // watch out for periodic case -- we could startor end on the cut
    {
      if( i<numberOfMatchingCurves )
      {
	MatchingCurve & match = matchingCurves[i];
	gridLine=match.gridLine+boundaryOffset[0][0];  
        assert( gridLine>=0 );
	curveDirection=match.curveDirection;
        // If there is a matching curve on the periodic branch cut then we force it to be at r=0
        if( i==0 && (bool)getIsPeriodic(axis1) && gridLine==0 )
	{
	  gridLineOld=gridLine;   
	  continue;
	}
      }
      else
      {
	gridLine=I1.getBound();
      }
      if( marchingDirection==0 ||   // treats case of the initial curve
          curveDirection==marchingDirection ) 
      {
        Range J1(gridLineOld,gridLine);
	dp.setDataPoints( x(J1,0,i3,xAxes),3,domainDimension-1);
	ReparameterizationTransform rt(dp,ReparameterizationTransform::equidistribution);
	for( int axis=0; axis<domainDimension-1; axis++ )
	  rt.setIsPeriodic(axis,getIsPeriodic(axis));
  
	rt.setEquidistributionParameters(arcLengthWeight,curvatureWeight);

        printf("Equidistribute with interior match curve: equidistribute the interval J1=[%i,%i]\n",
	       J1.getBase(),J1.getBound());
	
	realArray r(J1,1);
	if( stretchGrid && startCurveStretchMapping!=NULL ) // add stretching here
	{
          real ra=rs(J1.getBase()), rb=rs(J1.getBound());
          real rba=max(rb-ra,REAL_MIN);
	  r(J1)=(rs(J1)-ra)/rba; // scale stretched grid to [0,1]
	  
	}
	else
	{
	  const real dr = 1./max(1,J1.getBound()-J1.getBase());
	  r.seqAdd(0.,dr);
	}
	rt.mapC(r,xe(J1,xAxes));
	
      }
      gridLineOld=gridLine;
    }
  }
  else
  {
    dp.setDataPoints( x(I1,0,i3,xAxes),3,domainDimension-1);
    ReparameterizationTransform rt(dp,ReparameterizationTransform::equidistribution);
    for( int axis=0; axis<domainDimension-1; axis++ )
      rt.setIsPeriodic(axis,getIsPeriodic(axis));
  
    rt.setEquidistributionParameters(arcLengthWeight,curvatureWeight);

    if( stretchGrid && startCurveStretchMapping!=NULL ) // add stretching here
    {
      rt.map(rs,xe);
    }
    else
    {
      const real dr = 1./max(1,I1.getBound()-I1.getBase());
      r.seqAdd(0.,dr);

      rt.map(r,xe);
    }
  }
  
  xe.reshape(I1,1,1,xAxes);

  // ::display(xe,"Equidistributed x");
  
  x(I1,I2,i3,xAxes)=(1.-weight)*x(I1,I2,i3,xAxes) + weight*xe(I1,I2,0,xAxes);
  
  return 0;
} 

int HyperbolicMapping::
computeCellVolumes(const realArray & xt, const int & i3Mod2,
                   real & minCellVolume, real & maxCellVolume, 
                   const real & dSign )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Compute the minimum and maximum `cell volumes'. 
/// \param xt (input) : step length vector
///  / minCellVolume, maxCellVolume : minimum and maximum (signed) volumes.
//===========================================================================
{
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 

  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
    Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

  Range xAxes(0,rangeDimension-1);

  realArray volume(I1,I2);
  // *** this is not exactly the cell volume but close enough **
  // For dir==0 we compute the `forward cell volume' and for dir==1 the reverse cell volume.
  // if the grid turns inside out then one of these should change sign.
  for( int dir=0; dir<=1; dir++ )
  {
    const int i3m = (i3Mod2+dir) %2;
    if( rangeDimension==2 )
      volume=(normalCC(I1,I2,0,0)*xt(I1,I2,i3m,0)+  
	      normalCC(I1,I2,0,1)*xt(I1,I2,i3m,1));
    else
      volume=(normalCC(I1,I2,0,0)*xt(I1,I2,i3m,0)+
	      normalCC(I1,I2,0,1)*xt(I1,I2,i3m,1)+
	      normalCC(I1,I2,0,2)*xt(I1,I2,i3m,2));

    // ::display(volume,"volume","%9.2e");

    if( dir==0 )
    {
      minCellVolume=min(volume);
      maxCellVolume=max(volume);
    }
    else
    {
      minCellVolume=min(minCellVolume,min(volume));
      maxCellVolume=max(maxCellVolume,max(volume));
    }

  }
  

  real minCell=minCellVolume;
  minCellVolume=min(dSign*minCellVolume,dSign*maxCellVolume);
  maxCellVolume=max(dSign*minCell      ,dSign*maxCellVolume);
  
  return 0;
}



/* -------------
int HyperbolicMapping::
inspectInitialSurface( realArray & xSurface, realArray & normal )
//===========================================================================
/// \brief  
///       Inspect the initial surface for corners etc.
//===========================================================================
{

  // find the min and max angles between normals to adjacent cells.

  realArray cosAngle;
  cosAngle = normal(I1,I2,0,axis1)*normal(I1+1,I2,0,axis1);
  minCosAngle=min(cosAngle);
  //
  // cosAngle == 0 : 90 degree corner
  //           < 0  : 
  //
  //         ^
   //        |                          |
  //   -------------                    |
  //               |             ^   <--|
  //               |             |      |
  //               | -->   -------------|   
  //               |

  return 0;
}
------------ */


