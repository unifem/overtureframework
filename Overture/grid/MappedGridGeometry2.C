#include "MappedGrid.h"
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

Integer MappedGridData::
computeGeometryFromMapping(const Integer& what_,
			   const Integer& how) 
{
  Integer what=what_;

  Integer returnValue = 0;
  const real realSmall = REAL_MIN*100.;

  Range d0 = numberOfDimensions, one = 1, all;

  Range dr = rangeDimension();
  Range dd = domainDimension();

//
//      Compute the all geometric quantities directly from the mapping.
//

//  ------  ADD THIS 
//        ----------- Use the grid from the Mapping if possible ----------
//     ******* In parallel we need to consider the parition as well *********
  if( false )
  {
    // We may be able to use the grid in the mapping for the vertex/center array
    if( (what & THEvertex) || (isAllVertexCentered && (what & THEcenter)) )
    {
      Mapping & map = mapping.getMapping();
      if( map.gridIsValid() )
      {
	bool gridOk = true;

        bool includeGhost=true;
	const realArray & grid = map.getGrid(Overture::nullMappingParameters(),includeGhost);

        bool gridDimensionsOk=true, ghostLinesOk=true;
	for( int axis=0; axis<domainDimension(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  { // The grid in the mapping should have the correct number of grid points and ghost points
	    // If the mapping has more ghost points then we would need to change the number in the vertex or center

	    if( false )
	    {
	      printf(" vertex bounds = [%i,%i]  grid bounds=[%i,%i] dimension=[%i,%i]\n",
		     vertex->getBase(axis),vertex->getBound(axis),grid.getBase(axis),grid.getBound(axis),
		     dimension(0,axis),dimension(1,axis));
	  

	      printf(" gridIndexRange(side,axis)=%i map.gridIndexRange(side,axis)=%i\n"
		     " numberOfGhostPoints(side,axis)=%i map.numberOfGhostPoints(side,axis)=%i\n",
		     gridIndexRange(side,axis), map.getGridIndexRange(side,axis),
		     numberOfGhostPoints(side,axis),map.getNumberOfGhostPoints(side,axis));
	    }
	    
	    if( gridIndexRange(side,axis)!=map.getGridIndexRange(side,axis) )
	    {
	      gridDimensionsOk=false;
	    }
	    if( numberOfGhostPoints(side,axis) != map.getNumberOfGhostPoints(side,axis) )
	    {
              ghostLinesOk=false;
	    }
	    if( !gridDimensionsOk || !ghostLinesOk )
	      gridOk=false;
	  }
	}
	if( gridOk )
	{ // we can use the grid from the Mapping. 

	  printF(" MappedGrid::computeGeometry: I can use the grid from the mapping %s for the vertex or center array\n",
		 (const char*)map.getName(Mapping::mappingName));
	
	  // const realArray & grid = map.getGrid();
	  if( (what & THEvertex) )
	  {
	    realArray & x = *vertex;
	    x.reference(grid);       // Is this correct ?? -- we need to update references to the envelope
	    computedGeometry |= THEvertex;
            what ^= THEvertex;       // exclusive-or, we no longer need to compute the vertex
	  }
	  if( isAllVertexCentered && (what & THEcenter) )
	  {
	    realArray & x = *center;
	    x.reference(grid);                       // Is this correct ??
	    computedGeometry |= THEcenter;
            what ^= THEcenter;      // we no longer need to compute the vertex
	  }

	  // if( true ) return 0;  // *****************
	  
	}
	else
	{
	  printF(" MappedGrid::computeGeometry: I can NOT use the grid from mapping %s for the vertex or "
                 " center array, gridDimensions are %s, ghost lines are %s\n",
		 (const char*)map.getName(Mapping::mappingName),
                 (gridDimensionsOk ? "ok" : "not ok"),(ghostLinesOk ? "ok" : "not ok"));

	}
      
      }
    }
  }
  
//   ------ 
  

  Range dD[3], &d1D=dD[0], &d2D=dD[1], &d3D=dD[2]; Integer kd;
  for (kd=0; kd<3; kd++)
    dD[kd] = Range(dimension(0,kd), dimension(1,kd));

  
  // We compute the vertex derivative xr in the following cases: (e.g. we need xr to compute rx)
  bool vertexDerivativeIsNeeded = ( what & (THEvertexDerivative | THEvertexJacobian | THEinverseVertexDerivative) )
    || ( isAllVertexCentered && what & (THEcenterDerivative | THEcenterJacobian | THEinverseCenterDerivative |
                                        THEcenterNormal | THEcenterArea | THEcellVolume) );

  // No need to compute the vertex derivative if it is already there: 
  bool vertexDerivativeIsComputed = ( (computedGeometry & THEvertexDerivative) ||
				      (isAllVertexCentered && (computedGeometry & THEcenterDerivative)) );

  bool computeVertexDerivative = vertexDerivativeIsNeeded && !vertexDerivativeIsComputed;

  // printF("MappedGridGeometry2: xr : isNeeded=%i, isComputed=%i, need to compute=%i\n",
  // 	 (int)vertexDerivativeIsNeeded,(int)vertexDerivativeIsComputed,(int)computeVertexDerivative);
  


  if (what & (
    THEvertex                  |
    THEvertexDerivative        |
    THEvertexJacobian          |
    THEinverseVertexDerivative  ) || (
      isAllVertexCentered && what & (
	THEcenter                  |
	THEcenterDerivative        |
	THEcenterJacobian          |
	THEinverseCenterDerivative |
	THEcenterNormal            |
	THEcenterArea              |
	THEcellVolume              )) || (
          isAllCellCentered && what &
	  THEcorner                  )) {
//
//          Compute geometry at the vertices either because the vertex or center array is needed
//          or because a derived quantity (e.g. THEvertexJacobian) is needed.
//
//          xD might be needed for the boundingBox.
//
    realArray x;
    if( (what | computedGeometry) & THEvertex) 
      x.reference(*vertex);
    else if (isAllVertexCentered && (what | computedGeometry) & THEcenter) 
      x.reference(*center);
    else if (isAllCellCentered && (what | computedGeometry) & THEcorner) 
      x.reference(*corner);
    else 
    {
      x.partition(partition);
      x.redim(d1D,d2D,d3D,d0);
    } // end if

    Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
    Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
    IntegerArray dimensionL(2,3);
    Integer numberOfPoints = 1;
    for (kd=0; kd<3; kd++) 
    {
      dimensionL(0,kd) = x.getBase(kd),
	dimensionL(1,kd) = x.getBound(kd);
      i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
      numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
    } // end for
    // Range p = numberOfPoints;

    // if (numberOfPoints) x.reshape(p,d0);

    realArray xr;
    if ((what | computedGeometry) & THEvertexDerivative) 
    {
      xr.reference(*vertexDerivative);
      // *kkc --changed to accomodate surface grids      if (numberOfPoints) xr.reshape(d1,d2,d3,SQR(numberOfDimensions));
      if (numberOfPoints) xr.reshape(d1,d2,d3,domainDimension()*rangeDimension());
    } 
    else if (isAllVertexCentered && (what | computedGeometry) & THEcenterDerivative) 
    {
      xr.reference(*centerDerivative);
      if (numberOfPoints) xr.reshape(d1,d2,d3,domainDimension()*rangeDimension());
    }
    else if( numberOfPoints && computeVertexDerivative ) 
    {
      // ******  we seem to allocate space for xr even if we don't need it ?? ********************
      // printF("MappedGridGeometry2: ALLOCATE temp space for xr\n");
      xr.partition(partition);
      xr.redim(d1,d2,d3,domainDimension()*rangeDimension()); 
    } // end if

    if (((how & COMPUTEgeometry && what &
	  (THEvertex | THEvertexDerivative)) ||
	 !(computedGeometry & THEvertex) ||
	 !(computedGeometry & THEvertexDerivative)) ||
	(isAllVertexCentered &&
	 ((how & COMPUTEgeometry && what &
	   (THEcenter | THEcenterDerivative)) ||
	  !(computedGeometry & THEcenter) ||
	  !(computedGeometry & THEcenterDerivative))) ||
	(isAllCellCentered &&
	 ((how & COMPUTEgeometry && what & THEcorner) ||
	  !(computedGeometry & THEcorner)))) 
    {

      //  The vertex geometry either does not exist or must be recomputed.
      if( numberOfPoints ) 
      {
        // *kkc --changed to accommodate surface grids	realArray r(d1,d2,d3,d0);
	// *wdh* 060523: assign the local arrays to avoid communication. 
	realArray r; r.partition(partition); r.redim(d1,d2,d3,domainDimension());
        Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
        #ifdef USE_PPP
  	  realSerialArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
	  j1=i1; j2=i2; j3=i3;
          bool ok = ParallelUtility::getLocalArrayBounds(r,rLocal,j1,j2,j3,1); // include parallel ghost
          rLocal=0.;  // avoid UMR's in valgrind
	#else
          realSerialArray & rLocal = r;
	  j1=i1; j2=i2; j3=i3;
        #endif
	for( kd=0; kd<domainDimension(); kd++ ) 
        {
          Index jkd = jv[kd];
	  for( Integer k=rLocal.getBase(kd); k<=rLocal.getBound(kd); k++ ) 
          {
	    jv[kd] = k;
	    rLocal(j1,j2,j3,kd) = gridSpacing(kd) * (k - indexRange(0,kd));
	  } // end for
	  jv[kd] = jkd; // reset
	} // end for

// 	realArray r; r.partition(partition); r.redim(d1,d2,d3,domainDimension());
// 	// *kkc	for (kd=0; kd<numberOfDimensions; kd++) {
// 	for (kd=0; kd<domainDimension(); kd++) {
// 	  for (Integer k=dimensionL(0,kd);
// 	       k<=dimensionL(1,kd); k++) {
// 	    i[kd] = k;
// 	    r(i1,i2,i3,kd) = gridSpacing(kd) *
// 	      (k - indexRange(0,kd));
// 	  } // end for
// 	  i[kd] = d[kd];
// 	} // end for

	// printF("MappedGridGeometry2: evaluate mapping.mapGrid(r, x, xr)\n");

        if( computeVertexDerivative )
  	  mapping.mapGrid(r, x, xr);   
        else
  	  mapping.mapGrid(r, x);  

	// *kkc --changed to accommodate surface grids        
	//if (numberOfPoints) xr.reshape(d1,d2,d3,d0,d0);  // make 5D
        if (xr.getLength(0)>0) xr.reshape(d1,d2,d3,dd,dr);  // make 5D

        // Now set the grid in the mapping to use the vertex *wdh* 070318 
        // This will save space 
        if( (bool)shareGridWithMapping && ( (what & THEvertex) || (isAllVertexCentered && (what & THEcenter)) ) )
	{
          mapping.setGrid( x, gridIndexRange );
	}
	
      } // end if
      if (what & THEvertex)
	computedGeometry |= THEvertex;
      if (isAllVertexCentered && what & THEcenter)
	computedGeometry |= THEcenter;
      if (isAllCellCentered && what & THEcorner)
	computedGeometry |= THEcorner;
      if (what & THEvertexDerivative)
	computedGeometry |= THEvertexDerivative;
      if (isAllVertexCentered && what & THEcenterDerivative)
	computedGeometry |= THEcenterDerivative;
    } // end if
    else if(  xr.getLength(0)>0 )
    {
      xr.reshape(d1,d2,d3,d0,d0);  // *wdh* 011204 : xr may have already been computed
    }
    
    
    // ---------------------------
    // ---- THEvertexJacobian ----
    // ---------------------------

    realArray vj; // vertexJacobian 
    // *wdh* 100517: THEvertexJacobian is no longer needed for THEinverseVertexDerivative
    //          if (what & (
    //            THEvertexJacobian          |
    //            THEinverseVertexDerivative ) || (
    //      	isAllVertexCentered && what & (
    //      	  THEcenterJacobian          |
    //      	  THEinverseCenterDerivative |
    //      	  THEcellVolume              ))) 
    if (what & ( THEvertexJacobian ) || (
	  isAllVertexCentered && what & (
	    THEcenterJacobian          |
	    THEcellVolume              ))) 
    {
      if (numberOfPoints) 
      {
	if (what & THEvertexJacobian) 
        {
	  vj.reference(*vertexJacobian);
	} 
        else if (isAllVertexCentered && what & THEcenterJacobian) 
        {
	  vj.reference(*centerJacobian);
	} 
        else 
        {
          //  printF("MappedGridGeometry2: ALLOCATE temp space for the vertexJacobian.\n");
          vj.partition(partition);
	  vj.redim(d1,d2,d3);
	} // end if


        if( true ) // *wdh* 040204
	{
	  // parallel version here 
          realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);

	  real *xrp = xrLocal.Array_Descriptor.Array_View_Pointer3;
	  const int xrDim0=xrLocal.getRawDataSize(0);
	  const int xrDim1=xrLocal.getRawDataSize(1);
	  const int xrDim2=xrLocal.getRawDataSize(2);
	  const int xrDim3=numberOfDimensions;   // note
#undef XR
#define XR(i0,i1,i2,i3,i4) xrp[i0+xrDim0*(i1+xrDim1*(i2+xrDim2*(i3+xrDim3*(i4))))]

          realSerialArray vjLocal; getLocalArrayWithGhostBoundaries(vj,vjLocal);
	  real *vjp = vjLocal.Array_Descriptor.Array_View_Pointer2;
	  const int vjDim0=vjLocal.getRawDataSize(0);
	  const int vjDim1=vjLocal.getRawDataSize(1);
	  const int dj1=vjDim0, dj2=dj1*vjDim1; 
#define VJ(i0,i1,i2) vjp[(i0)+(i1)*dj1+(i2)*dj2]

          Index I1=d1, I2=d2, I3=d3;
	  bool ok = ParallelUtility::getLocalArrayBounds(xr,xrLocal,I1,I2,I3,1); // include parallel ghost
	  if( ok )
	  {
	    int i1,i2,i3;
	    if( numberOfDimensions == 1 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		VJ(i1,i2,i3) = XR(i1,i2,i3,0,0);
	      }
	    }
	    else if( numberOfDimensions == 2 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		VJ(i1,i2,i3) = XR(i1,i2,i3,0,0) * XR(i1,i2,i3,1,1) - XR(i1,i2,i3,0,1) * XR(i1,i2,i3,1,0);
	      }
	    }
	    else
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		VJ(i1,i2,i3) =
		  (XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,1)-XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,0))*XR(i1,i2,i3,2,2) +
		  (XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,2)-XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,1))*XR(i1,i2,i3,2,0) +
		  (XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,0)-XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,2))*XR(i1,i2,i3,2,1);
	      }
	    }
	  }
	  
#undef VJ
#undef XR
	}
	else
	{  // old way

	  switch (numberOfDimensions) 
	  {
	  case 1:
	    vj = xr;
	    break;
	  case 2:
	    vj = xr(d1,d2,d3,0,0) * xr(d1,d2,d3,1,1) - xr(d1,d2,d3,0,1) * xr(d1,d2,d3,1,0);
	    break;
	  case 3:
	    vj =
	      (xr(d1,d2,d3,0,0)*xr(d1,d2,d3,1,1)-xr(d1,d2,d3,0,1)*xr(d1,d2,d3,1,0))*xr(d1,d2,d3,2,2) +
	      (xr(d1,d2,d3,0,1)*xr(d1,d2,d3,1,2)-xr(d1,d2,d3,0,2)*xr(d1,d2,d3,1,1))*xr(d1,d2,d3,2,0) +
	      (xr(d1,d2,d3,0,2)*xr(d1,d2,d3,1,0)-xr(d1,d2,d3,0,0)*xr(d1,d2,d3,1,2))*xr(d1,d2,d3,2,1);
	    break;
	  } // end switch
	}
	
	if (isAllVertexCentered && what & THEcellVolume) {
	  realArray cv;
	  cv.reference(*cellVolume);
	  // cv.reshape(p);
	  cv = (gridSpacing(0) * gridSpacing(1) * gridSpacing(2)) * vj;
	  Real minCellVolume = min(*cellVolume);
	  if (minCellVolume == (Real)0.) cerr
	    << "MappedGridData::computeGeometry():  WARNING:  "
	    << "Computed a zero cellVolume." << endl;
	  /* -- from pmb. wdh: are cell volumes negative for left handed coordinate systems?
	     else if (minCellVolume < (Real)0.) cerr
	     << "MappedGridData::computeGeometry():  WARNING:  "
	     << "Computed a negative cellVolume." << endl;
	  */
	} // end if
      } // end if
      if (what & THEvertexJacobian)
	computedGeometry |= THEvertexJacobian;
      if (isAllVertexCentered && what & THEcenterJacobian)
	computedGeometry |= THEcenterJacobian;
      if (isAllVertexCentered && what & THEcellVolume)
	computedGeometry |= THEcellVolume;
    } // end if

    // ---------------------------
    // ---- THEcenterNormal - ----
    // ---------------------------

    realArray vn; // vn holds centerNormal. 
    // *wdh* 100517: centerNormal is no longer needed for THEinverseVertexDerivative
    //      if (what &
    //      	THEinverseVertexDerivative || (
    //      	  isAllVertexCentered && what & (
    //      	    THEinverseCenterDerivative |
    //      	    THEcenterNormal            |
    //      	    THEcenterArea              ))) 
    if( isAllVertexCentered && what & (
	  THEcenterNormal            |
	  THEcenterArea              ))
    {
      if( numberOfPoints ) 
      {
	if( isAllVertexCentered && what & THEcenterNormal ) 
        {
	  vn.reference(*centerNormal);
	  vn.reshape(d1,d2,d3,d0,d0);
	} 
        else 
        {
         //  printF("MappedGridGeometry2: ALLOCATE temp space for the centerNormal.\n");

          vn.partition(partition);

	  vn.redim(d1,d2,d3,d0,d0);
	} // end if

        #ifdef USE_PPP
	  realSerialArray vnLocal; getLocalArrayWithGhostBoundaries(vn,vnLocal);
	  realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
	#else
          realSerialArray & vnLocal = vn;
          realSerialArray & xrLocal = xr;
        #endif
        Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
        j1=d1; j2=d2; j3=d3;
        bool ok = ParallelUtility::getLocalArrayBounds(vn,vnLocal,j1,j2,j3,1); // include parallel ghost

	if( ok )
	{
	  switch (numberOfDimensions) 
	  {
	  case 1:
	    vnLocal = (Real)1.;
	    break;
	  case 2: 
	  {
	    for (Integer i=0; i<numberOfDimensions; i++)
	      for (Integer l=0; l<numberOfDimensions; l++)
		vnLocal(j1,j2,j3,l,i) = (gridSpacing(1-i) * (1-2*l) * (1-2*i)) * xrLocal(j1,j2,j3,1-l,1-i);
	  } break;
	  case 3: 
	  {
	    for (Integer i=0; i<numberOfDimensions; i++)
	      for (Integer l=0; l<numberOfDimensions; l++) 
              {
		Integer j=(i+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
		vnLocal(j1,j2,j3,l,i) = (gridSpacing(j) * gridSpacing(k)) * 
		  (xrLocal(j1,j2,j3,m,j) * xrLocal(j1,j2,j3,n,k) - xrLocal(j1,j2,j3,m,k) * xrLocal(j1,j2,j3,n,j));
	      } // end for
	  } break;
	  } // end switch
	}
	
// 	switch (numberOfDimensions) 
//         {
// 	case 1:
// 	  vn = (Real)1.;
// 	  break;
// 	case 2: {
// 	  for (Integer i=0; i<numberOfDimensions; i++)
// 	    for (Integer l=0; l<numberOfDimensions; l++)
// 	      vn(d1,d2,d3,l,i) = (gridSpacing(1-i) * (1-2*l) * (1-2*i)) * xr(d1,d2,d3,1-l,1-i);
// 	} break;
// 	case 3: {
// 	  for (Integer i=0; i<numberOfDimensions; i++)
// 	    for (Integer l=0; l<numberOfDimensions; l++) {
// 	      Integer j=(i+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
// 	      vn(d1,d2,d3,l,i) = (gridSpacing(j) * gridSpacing(k)) * 
//                                  (xr(d1,d2,d3,m,j) * xr(d1,d2,d3,n,k) - xr(d1,d2,d3,m,k) * xr(d1,d2,d3,n,j));
// 	    } // end for
// 	} break;
// 	} // end switch


      } // end if
      if (isAllVertexCentered && what & THEcenterNormal)
	computedGeometry |= THEcenterNormal;
    } // end if

    if (isAllVertexCentered && what & THEcenterArea) 
    {
      if (numberOfPoints) 
      {
	realArray ca;
	ca.reference(*centerArea);
	ca.reshape(d1,d2,d3,one,d0);

	Index I1=d1, I2=d2, I3=d3;
#ifdef USE_PPP
	realSerialArray caLocal;
	getLocalArrayWithGhostBoundaries(ca,caLocal);
	realSerialArray vnLocal;
	getLocalArrayWithGhostBoundaries(vn,vnLocal);
#else
	realSerialArray & caLocal = ca;
	realSerialArray & vnLocal = vn;
#endif

	bool ok = ParallelUtility::getLocalArrayBounds(ca,caLocal, I1,I2,I3,1); //include parallel ghost

	if(ok)
	switch (numberOfDimensions) {
	case 1:
	  caLocal = (Real)1.;
	  break;
	case 2: {
	  for (Integer l=0; l<numberOfDimensions; l++)
	    caLocal(I1,I2,I3,0,l) = sqrt (vnLocal(I1,I2,I3,0,l) * vnLocal(I1,I2,I3,0,l) + vnLocal(I1,I2,I3,1,l) * vnLocal(I1,I2,I3,1,l));
	} break;
	case 3: {
	  for (Integer l=0; l<numberOfDimensions; l++)
	    caLocal(I1,I2,I3,0,l) = sqrt
	      ( vnLocal(I1,I2,I3,0,l) * vnLocal(I1,I2,I3,0,l)
		+ vnLocal(I1,I2,I3,1,l) * vnLocal(I1,I2,I3,1,l)
		+ vnLocal(I1,I2,I3,2,l) * vnLocal(I1,I2,I3,2,l) );
	} break;
	} // end switch
      } // end if
      computedGeometry |= THEcenterArea;
    } // end if

    // -------------------------------------------
    // ---  Compute THEinverseVertexDerivative ---
    // -------------------------------------------
    if (what &
	THEinverseVertexDerivative || (
	  isAllVertexCentered && what &
	  THEinverseCenterDerivative )) 
    {
      if( numberOfPoints ) 
      {
        realArray rx;
	if (what & THEinverseVertexDerivative)
	  rx.reference(*inverseVertexDerivative);
	else rx.reference(*inverseCenterDerivative);


	if( true ) // *wdh* 100517 -- new optimized version that doesn't use the centerNormal or centerJacobian array 
	{
	  
          realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
	  real *rxp = rxLocal.Array_Descriptor.Array_View_Pointer3;
	  const int rxDim0=rxLocal.getRawDataSize(0);
	  const int rxDim1=rxLocal.getRawDataSize(1);
	  const int rxDim2=rxLocal.getRawDataSize(2);
	  const int rxDim3=numberOfDimensions;   // note
          #undef RX
          #define RX(i0,i1,i2,i3,i4) rxp[i0+rxDim0*(i1+rxDim1*(i2+rxDim2*(i3+rxDim3*(i4))))]

          realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
	  real *xrp = xrLocal.Array_Descriptor.Array_View_Pointer3;
	  const int xrDim0=xrLocal.getRawDataSize(0);
	  const int xrDim1=xrLocal.getRawDataSize(1);
	  const int xrDim2=xrLocal.getRawDataSize(2);
	  const int xrDim3=numberOfDimensions;   // note
          #undef XR
          #define XR(i0,i1,i2,i3,i4) xrp[i0+xrDim0*(i1+xrDim1*(i2+xrDim2*(i3+xrDim3*(i4))))]

          Index I1=d1, I2=d2, I3=d3;
	  bool ok = ParallelUtility::getLocalArrayBounds(rx,rxLocal,I1,I2,I3,1); // include parallel ghost
	  if( ok )
	  {
	    int i1,i2,i3;
	    real det;
	    real dr0 = gridSpacing(0), dr1=gridSpacing(1), dr2=gridSpacing(2);
	    if( numberOfDimensions == 1 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( XR(i1,i2,i3,0,0) !=0  )
		{
		  RX(i1,i2,i3,0,0)=1./XR(i1,i2,i3,0,0);
		}
		else
		{
		  RX(i1,i2,i3,0,0)=1.;
		}
	      }
	    }
	    else if( numberOfDimensions == 2 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		det = XR(i1,i2,i3,0,0) * XR(i1,i2,i3,1,1) - XR(i1,i2,i3,0,1) * XR(i1,i2,i3,1,0);
		if( det !=0  )
		{
		  det=1./det;
		  RX(i1,i2,i3,0,0) = XR(i1,i2,i3,1,1) * det;
		  RX(i1,i2,i3,1,0) =-XR(i1,i2,i3,1,0) * det;
		  RX(i1,i2,i3,0,1) =-XR(i1,i2,i3,0,1) * det;
		  RX(i1,i2,i3,1,1) = XR(i1,i2,i3,0,0) * det;
		}
		else
		{
		  RX(i1,i2,i3,0,0)=1.;
		  RX(i1,i2,i3,1,0)=1.;
		  RX(i1,i2,i3,0,1)=1.;
		  RX(i1,i2,i3,1,1)=1.;
		}
	      }
	    }
	    else
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		det =( (XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,1)-XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,0))*XR(i1,i2,i3,2,2) +
		       (XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,2)-XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,1))*XR(i1,i2,i3,2,0) +
		       (XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,0)-XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,2))*XR(i1,i2,i3,2,1) );
		if( det !=0  )
		{
		  det=1./det;

		  RX(i1,i2,i3,0,0)=(XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,2)-XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,1))*det;
		  RX(i1,i2,i3,1,0)=(XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,0)-XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,2))*det;
		  RX(i1,i2,i3,2,0)=(XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,1)-XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,0))*det;
		  RX(i1,i2,i3,0,1)=(XR(i1,i2,i3,2,1)*XR(i1,i2,i3,0,2)-XR(i1,i2,i3,2,2)*XR(i1,i2,i3,0,1))*det;
		  RX(i1,i2,i3,1,1)=(XR(i1,i2,i3,2,2)*XR(i1,i2,i3,0,0)-XR(i1,i2,i3,2,0)*XR(i1,i2,i3,0,2))*det;
		  RX(i1,i2,i3,2,1)=(XR(i1,i2,i3,2,0)*XR(i1,i2,i3,0,1)-XR(i1,i2,i3,2,1)*XR(i1,i2,i3,0,0))*det;
		  RX(i1,i2,i3,0,2)=(XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,2)-XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,1))*det;
		  RX(i1,i2,i3,1,2)=(XR(i1,i2,i3,0,2)*XR(i1,i2,i3,1,0)-XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,2))*det;
		  RX(i1,i2,i3,2,2)=(XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,1)-XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,0))*det;

		}
		else
		{
		  RX(i1,i2,i3,0,0) = 1.;
		  RX(i1,i2,i3,1,0) = 1.;
		  RX(i1,i2,i3,2,0) = 1.;
		  RX(i1,i2,i3,0,1) = 1.;
		  RX(i1,i2,i3,1,1) = 1.;
		  RX(i1,i2,i3,2,1) = 1.;
		  RX(i1,i2,i3,0,2) = 1.;
		  RX(i1,i2,i3,1,2) = 1.;
		  RX(i1,i2,i3,2,2) = 1.;
		}
	      }
	    }
	  } // end if ok
	  
#undef RX
#undef XR

	  
	}
        else if( true ) // *wdh* 040204
	{
	  // parallel version here 
          realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);

	  real *rxp = rxLocal.Array_Descriptor.Array_View_Pointer3;
	  const int rxDim0=rxLocal.getRawDataSize(0);
	  const int rxDim1=rxLocal.getRawDataSize(1);
	  const int rxDim2=rxLocal.getRawDataSize(2);
	  const int rxDim3=numberOfDimensions;   // note
          #undef RX
          #define RX(i0,i1,i2,i3,i4) rxp[i0+rxDim0*(i1+rxDim1*(i2+rxDim2*(i3+rxDim3*(i4))))]

          realSerialArray vjLocal; getLocalArrayWithGhostBoundaries(vj,vjLocal);
	  real *vjp = vjLocal.Array_Descriptor.Array_View_Pointer2;
	  const int vjDim0=vjLocal.getRawDataSize(0);
	  const int vjDim1=vjLocal.getRawDataSize(1);
	  const int dj1=vjDim0, dj2=dj1*vjDim1; 
          #define VJ(i0,i1,i2) vjp[(i0)+(i1)*dj1+(i2)*dj2]

          realSerialArray vnLocal; getLocalArrayWithGhostBoundaries(vn,vnLocal);
	  const real *vnp = vnLocal.Array_Descriptor.Array_View_Pointer4;
	  const int vnDim0=vnLocal.getRawDataSize(0);
	  const int vnDim1=vnLocal.getRawDataSize(1);
	  const int vnDim2=vnLocal.getRawDataSize(2);
	  const int vnDim3=vnLocal.getRawDataSize(3); // mg.numberOfDimensions();   // note
          #undef VN
          #define VN(i0,i1,i2,i3,i4) vnp[i0+vnDim0*(i1+vnDim1*(i2+vnDim2*(i3+vnDim3*(i4))))]

          Index I1=d1, I2=d2, I3=d3;
	  bool ok = ParallelUtility::getLocalArrayBounds(rx,rxLocal,I1,I2,I3,1); // include parallel ghost
	  if( ok )
	  {
	    int i1,i2,i3;
	    real det;
	    real dr0 = gridSpacing(0), dr1=gridSpacing(1), dr2=gridSpacing(2);
	    if( numberOfDimensions == 1 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( VJ(i1,i2,i3) !=0  )
		{
		  RX(i1,i2,i3,0,0)=1./VJ(i1,i2,i3);
		}
		else
		{
		  RX(i1,i2,i3,0,0)=1.;
		}
	      }
	    }
	    else if( numberOfDimensions == 2 )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		det=VJ(i1,i2,i3);
		if( det !=0  )
		{
		  det=1./det;
		  // rx(i1,i2,i3,i,l) = ((Real)1. / gridSpacing(1-i)) * vn(i1,i2,i3,l,i) * det;
		  RX(i1,i2,i3,0,0) = VN(i1,i2,i3,0,0) * det/dr1;
		  RX(i1,i2,i3,1,0) = VN(i1,i2,i3,0,1) * det/dr0;
		  RX(i1,i2,i3,0,1) = VN(i1,i2,i3,1,0) * det/dr1;
		  RX(i1,i2,i3,1,1) = VN(i1,i2,i3,1,1) * det/dr0;

		}
		else
		{
		  RX(i1,i2,i3,0,0)=1.;
		  RX(i1,i2,i3,1,0)=1.;
		  RX(i1,i2,i3,0,1)=1.;
		  RX(i1,i2,i3,1,1)=1.;
		}
	      }
	    }
	    else
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		det=VJ(i1,i2,i3);
		if( det !=0  )
		{
		  det=1./det;
		  // Integer j=(i+1)%3, k=(j+1)%3;
		  // rx(d1,d2,d3,i,l) = ((Real)1. / (gridSpacing(j) * gridSpacing(k))) * vn(d1,d2,d3,l,i) * det;

		  RX(i1,i2,i3,0,0) = VN(i1,i2,i3,0,0) * det/(dr1*dr2);
		  RX(i1,i2,i3,1,0) = VN(i1,i2,i3,0,1) * det/(dr2*dr0);
		  RX(i1,i2,i3,2,0) = VN(i1,i2,i3,0,2) * det/(dr0*dr1);
		  RX(i1,i2,i3,0,1) = VN(i1,i2,i3,1,0) * det/(dr1*dr2);
		  RX(i1,i2,i3,1,1) = VN(i1,i2,i3,1,1) * det/(dr2*dr0);
		  RX(i1,i2,i3,2,1) = VN(i1,i2,i3,1,2) * det/(dr0*dr1);
		  RX(i1,i2,i3,0,2) = VN(i1,i2,i3,2,0) * det/(dr1*dr2);
		  RX(i1,i2,i3,1,2) = VN(i1,i2,i3,2,1) * det/(dr2*dr0);
		  RX(i1,i2,i3,2,2) = VN(i1,i2,i3,2,2) * det/(dr0*dr1);

		}
		else
		{
		  RX(i1,i2,i3,0,0) = 1.;
		  RX(i1,i2,i3,1,0) = 1.;
		  RX(i1,i2,i3,2,0) = 1.;
		  RX(i1,i2,i3,0,1) = 1.;
		  RX(i1,i2,i3,1,1) = 1.;
		  RX(i1,i2,i3,2,1) = 1.;
		  RX(i1,i2,i3,0,2) = 1.;
		  RX(i1,i2,i3,1,2) = 1.;
		  RX(i1,i2,i3,2,2) = 1.;
		}
	      }
	    }
	  } // end if ok
	  
#undef VJ
#undef VN
#undef RX
	}
	else
	{  // old way
  	  realArray det; det.partition(partition); det.redim(d1,d2,d3);

	  rx.reshape(d1,d2,d3,d0,d0);

	  where (vj == (Real)0.) det = (Real)1.;
	  otherwise() det = (Real)1. / vj;
	  switch (numberOfDimensions) {
	  case 1:
	    rx = det;
	    break;
	  case 2: {
	    for (Integer i=0; i<2; i++)
	      for (Integer l=0; l<2; l++)
		rx(d1,d2,d3,i,l) = ((Real)1. / gridSpacing(1-i)) * vn(d1,d2,d3,l,i) * det;
	  } break;
	  case 3: {
	    for (Integer i=0; i<numberOfDimensions; i++)
	      for (Integer l=0; l<numberOfDimensions; l++) {
		Integer j=(i+1)%3, k=(j+1)%3;
		rx(d1,d2,d3,i,l) = ((Real)1. / (gridSpacing(j) * gridSpacing(k))) * vn(d1,d2,d3,l,i) * det;
	      } // end for
	  } break;
	  } // end switch
	}
	
      } // end if
      if (what & THEinverseVertexDerivative)
	computedGeometry |= THEinverseVertexDerivative;
      if (isAllVertexCentered && what & THEinverseCenterDerivative)
	computedGeometry |= THEinverseCenterDerivative;
    } // end if
  
    vn.redim(0); // Save some space.

 //    if (what & THEminMaxEdgeLength) 
//     {
//       // Compute the min and max edge-length's -- the edge length along each axis is the
//       // distance between grid points in that direction.
//       if( true )
//       {
// 	// new way that works in parallel too
	
//         Index iD[3], &i1D=iD[0], &i2D=iD[1], &i3D=iD[2];
//         for (kd=0; kd<3; kd++) iD[kd] = dD[kd];
//         int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

//         // const realSerialArray & xLocal = x.getLocalArray();
//         realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);

// 	for (kd=0; kd<numberOfDimensions; kd++) 
// 	{
// 	  iD[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
      
//           real eMin=REAL_MAX, eMax=0.;

//           bool ok = ParallelUtility::getLocalArrayBounds(x,xLocal,i1D,i2D,i3D,0);
// 	  if( ok )
// 	  {
// 	    is1=is2=is3=0;
// 	    isv[kd]=1;
//             int i1,i2,i3;
// 	    if( numberOfDimensions==1 )
// 	    {
// 	      FOR_3D(i1,i2,i3,i1D,i2D,i3D)
// 	      {
// 		real ds =SQR(xLocal(i1+is1,i2,i3,0)-xLocal(i1,i2,i3,0));
// 		eMin = min(eMin,ds);
// 		eMax = max(eMax,ds);
// 	      }
// 	    }
// 	    else if( numberOfDimensions==2 ) 
// 	    {
// 	      FOR_3D(i1,i2,i3,i1D,i2D,i3D)
// 	      {
// 		real ds =(SQR(xLocal(i1+is1,i2+is2,i3,0)-xLocal(i1,i2,i3,0))+
// 		          SQR(xLocal(i1+is1,i2+is2,i3,1)-xLocal(i1,i2,i3,1)));
// 		eMin = min(eMin,ds);
// 		eMax = max(eMax,ds);
// 	      }
// 	    }
// 	    else
// 	    {
// 	      FOR_3D(i1,i2,i3,i1D,i2D,i3D)
// 	      {
// 		real ds =(SQR(xLocal(i1+is1,i2+is2,i3+is3,0)-xLocal(i1,i2,i3,0))+
// 			  SQR(xLocal(i1+is1,i2+is2,i3+is3,1)-xLocal(i1,i2,i3,1))+
// 			  SQR(xLocal(i1+is1,i2+is2,i3+is3,2)-xLocal(i1,i2,i3,2)));
// 		eMin = min(eMin,ds);
// 		eMax = max(eMax,ds);
// 	      }
// 	    }
// 	  }
	  
// 	  minimumEdgeLength(kd) = ParallelUtility::getMinValue(sqrt(eMin));
// 	  maximumEdgeLength(kd) = ParallelUtility::getMaxValue(sqrt(eMax));
          
// 	  iD[kd] =dD[kd]; // reset
//  	} 

// 	for (kd=numberOfDimensions; kd<3; kd++) 
// 	{
// 	  minimumEdgeLength(kd) = (Real)1.;
// 	  maximumEdgeLength(kd) = (Real)1.;
// 	}
// 	/*
//         printF("minMaxEdgeLength: NEW way : min=[%10.4e,%10.4e,%10.4e] max=[%10.4e,%10.4e,%10.4e]\n",
// 	       minimumEdgeLength(0),minimumEdgeLength(1),minimumEdgeLength(2),
//                maximumEdgeLength(0),maximumEdgeLength(1),maximumEdgeLength(2));
// 	*/
//       }
 
 //     if( false )
//       { // old way
//         Index iD[3], &i1D=iD[0], &i2D=iD[1], &i3D=iD[2], jD[3], &j1D=jD[0], &j2D=jD[1], &j3D=jD[2];
//         for (kd=0; kd<3; kd++) iD[kd] = jD[kd] = dD[kd];

// 	RealDistributedArray y; y.partition(partition);
// 	y.redim(d1D,d2D,d3D,d0);
// 	for (kd=0; kd<3; kd++) if (kd < numberOfDimensions) 
// 	{
// 	  iD[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
// 	  jD[kd] = Range(dimension(0,kd)+1, dimension(1,kd));
// 	  y(i1D,i2D,i3D,d0) = x(j1D,j2D,j3D,d0) - x(i1D,i2D,i3D,d0);
// 	  y(i1D,i2D,i3D,d0) = y (i1D,i2D,i3D,d0) * y (i1D,i2D,i3D,d0);
// 	  RealDistributedArray z; z.partition(partition);
// 	  z.redim(i1D,i2D,i3D); z = (Real)0.;
// 	  for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
// 	    z += y(i1D,i2D,i3D,kd1);
// 	  minimumEdgeLength(kd) = Sqrt(min(z));
// 	  maximumEdgeLength(kd) = Sqrt(max(z));
// 	  iD[kd] = jD[kd] = dD[kd];
// 	} 
// 	else 
// 	{
// 	  minimumEdgeLength(kd) = (Real)1.;
// 	  maximumEdgeLength(kd) = (Real)1.;
// 	} // end if, end for

//         printF("minMaxEdgeLength: OLD way : min=[%10.4e,%10.4e,%10.4e] max=[%10.4e,%10.4e,%10.4e]\n",
// 	       minimumEdgeLength(0),minimumEdgeLength(1),minimumEdgeLength(2),
//                maximumEdgeLength(0),maximumEdgeLength(1),maximumEdgeLength(2));

//       }
//       computedGeometry |= THEminMaxEdgeLength;
//    } // end if

  }// end if

  if (what & THEboundingBox) 
  { // get the bounding box from the Mapping (*new* wdh 070320)
    // boundingBox = mapping.getMapping().getBoundingBox();
    
    // *wdh* 2013/08/20 -- use extendedGridIndexRange for bounding box derivative-periodic grids 

    // The bounding box covers the extendedIndexRange -- this will include ghost lines
    // on interpolation boundaries. Thus points that can interpolate from this grid will
    // be inside this bounding box.

    IntegerArray extendedGridIndexRange(2,3); extendedGridIndexRange=0;
    extendedGridIndexRange=gridIndexRange;
    for( int side=Start; side<=End; side++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
  	if( boundaryCondition(side,axis)==0 )
  	{
  	  extendedGridIndexRange(side,axis)=extendedIndexRange(side,axis);
  	}
        else if( boundaryCondition(side,axis)<0 && mapping.getIsPeriodic(axis)==Mapping::derivativePeriodic )
	{
          // include a ghost point on derivative periodic sides *wdh* 2013/08/20
	  extendedGridIndexRange(side,axis)+= 2*side-1;
	}
	
      }
    }

    // mapping.getMapping().getBoundingBox(extendedIndexRange,gridIndexRange,boundingBox);
    mapping.getMapping().getBoundingBox(extendedGridIndexRange,gridIndexRange,boundingBox);  // *wdh* 2013/08/20 

    // Now compute the local bounding box for the part of the grid on this processor
    // -- base this on the extendedGridIndexRange (include periodic pts and ghost-pts on interp boundaries)
    #ifdef USE_PPP
      Index I1,I2,I3;
      getIndex(extendedGridIndexRange,I1,I2,I3);
      assert( mask!=NULL );
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(*mask,maskLocal);
      const int includeGhost=1;  // is this correct ? 
      bool ok = ParallelUtility::getLocalArrayBounds(*mask,maskLocal,I1,I2,I3,includeGhost);
      IntegerArray localegir(2,3); localegir=0;
      if( ok )
      {
        localegir(0,0)=I1.getBase(); localegir(1,0)=I1.getBound(); 
        localegir(0,1)=I2.getBase(); localegir(1,1)=I2.getBound(); 
        localegir(0,2)=I3.getBase(); localegir(1,2)=I3.getBound(); 
      
        // *NOTE* we assume there is no communication in getBoundingBox or getGridMinAndMax when 
        // looking for local bounds  *wdh* 091130

	bool local=true;
	int returnValue=mapping.getMapping().getBoundingBox(localegir,gridIndexRange,localBoundingBox,local);
	// printF("MappedGridGeometry2: returnValue=%i\n",returnValue);
	if( returnValue!=0 )
	{
	  // unable to compute local bounds with the Mapping getBoundingBox since the grid in the Mapping
	  // does not have the same distribution -- use the vertex array instead
	  if( vertex==NULL )
	  {
	    printF("MappedGridGeometry2:ERROR: computing local bounding box. The vertex array is NULL\n");
	    Overture::abort("error");
	  }
	  realArray & x = *vertex;
	  Range R[3];
	  real xMin[3]={0.,0.,0.},  xMax[3]={0.,0.,0.};
	  for( int axis=0; axis<3; axis++ )
	  {
	    R[axis]=Range(extendedGridIndexRange(0,axis),extendedGridIndexRange(1,axis));
	  }
	  mapping.getMapping().getGridMinAndMax(x,R[0],R[1],R[2],xMin,xMax,local );
	  for( int axis=0; axis<3; axis++ )
	  {
	    localBoundingBox(0,axis)=xMin[axis];
	    localBoundingBox(1,axis)=xMax[axis];
	  }
	  if( false )
	  {
	    printF("MappedGridGeometry2:INFO: local bounding box computed from the vertex: "
		   "localBoundingBox=[%g,%g][%g,%g][%g,%g]\n",
		   localBoundingBox(0,0),localBoundingBox(1,0),localBoundingBox(0,1),localBoundingBox(1,1),
		   localBoundingBox(0,2),localBoundingBox(1,2));
	  }
	}
      }
      else
      {
        // no points on this processor -- local bounding box is null
        localegir(0,0)=0; localegir(1,0)=-1; 
        localegir(0,1)=0; localegir(1,1)=-1; 
        localegir(0,2)=0; localegir(1,2)=-1; 
        // *wdh* 091130 
        for( int axis=0; axis<3; axis++ )
        {
  	  localBoundingBox(0,axis)= FLT_MAX;
  	  localBoundingBox(1,axis)=-FLT_MAX;
        }
       
      }
    
    #else
      localBoundingBox=boundingBox;
    #endif

//     if( false )
//     {
//        Index I1,I2,I3;
//        getIndex(extendedGridIndexRange,I1,I2,I3);
//        const int includeGhost=1;  // is this correct ? 
//        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(*mask,maskLocal);
//        assert( mask!=NULL );
//        bool ok = ParallelUtility::getLocalArrayBounds(*mask,maskLocal,I1,I2,I3,includeGhost);
//        IntegerArray localegir(2,3); localegir=0;
//        if( ok )
//        {
//          localegir(0,0)=I1.getBase(); localegir(1,0)=I1.getBound(); 
//          localegir(0,1)=I2.getBase(); localegir(1,1)=I2.getBound(); 
//          localegir(0,2)=I3.getBase(); localegir(1,2)=I3.getBound(); 
//       }

//       const RealArray & bb = mapping.getMapping().getBoundingBox(); 
//       printf("*** MGG2: grid=(%s) boundingBox=[%g,%g][%g,%g][%g,%g] localBoundingBox=[%g,%g][%g,%g][%g,%g] "
//              "bb=[%g,%g][%g,%g][%g,%g] \n"
//              " egir=[%i,%i][%i,%i][%i,%i] legir=[%i,%i][%i,%i][%i,%i]\n",
// 	     (const char *)mapping.getName(Mapping::mappingName),
// 	     boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),boundingBox(0,2),boundingBox(1,2),
// 	     localBoundingBox(0,0),localBoundingBox(1,0),localBoundingBox(0,1),localBoundingBox(1,1),
//              localBoundingBox(0,2),localBoundingBox(1,2),
// 	     bb(0,0),bb(1,0),bb(0,1),bb(1,1), bb(0,2),bb(1,2),
//  	     extendedGridIndexRange(0,0),extendedGridIndexRange(1,0),
//  	     extendedGridIndexRange(0,1),extendedGridIndexRange(1,1),
//  	     extendedGridIndexRange(0,2),extendedGridIndexRange(1,2),
//  	     localegir(0,0),localegir(1,0),
//  	     localegir(0,1),localegir(1,1),
//  	     localegir(0,2),localegir(1,2)
//              );
//    }
    
    computedGeometry |= THEboundingBox;
  } 

  if (!isAllVertexCentered && what & (
    THEcenter                  |
    THEcenterDerivative        |
    THEcenterJacobian          |
    THEinverseCenterDerivative |
    THEcellVolume              |
    THEcenterNormal            |
    THEcenterArea              )) {
//
//          Compute geometry at the centers.
//
    RealDistributedArray x;
    if ((what | computedGeometry) & THEcenter) {
      x.reference(*center);
    } else {
      x.partition(partition);
      x.redim(d1D,d2D,d3D,d0);
    } // end if

    Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
    Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
    IntegerArray dimensionL(2,3);
    Integer numberOfPoints = 1;
    for (kd=0; kd<3; kd++) {
      dimensionL(0,kd) = x.getBase(kd),
	dimensionL(1,kd) = x.getBound(kd);
      i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
      numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
    } // end for
    // Range p = numberOfPoints;
    // if (numberOfPoints) x.reshape(p,d0);

    realArray xr;
    if ((what | computedGeometry) & THEcenterDerivative) 
    {
      xr.reference(*centerDerivative);
      if( xr.getLength(0)>0 ) xr.reshape(d1,d2,d3,SQR(numberOfDimensions));
    } 
    else if (numberOfPoints) 
    {
      // I am not sure if this covers all cases:
      if( what & (THEcenterJacobian | THEinverseCenterDerivative | THEcellVolume ) ) // *wdh* 011102 
      {
        xr.partition(partition);	
	xr.redim(d1,d2,d3,SQR(numberOfDimensions)); // no need to build if not used
      }
    } // end if

    if ((how & COMPUTEgeometry && what &
	 (THEcenter | THEcenterDerivative)) ||
	!(computedGeometry & THEcenter) ||
	!(computedGeometry & THEcenterDerivative)) {
//              The center geometry either does not exist or must be recomputed.
      if (numberOfPoints) 
      {
	// *wdh* 060523: assign the local arrays to avoid communication. 
	realArray r; r.partition(partition); r.redim(d1,d2,d3,domainDimension());
        Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
        #ifdef USE_PPP
  	  realSerialArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
	  j1=i1; j2=i2; j3=i3;
          bool ok = ParallelUtility::getLocalArrayBounds(r,rLocal,j1,j2,j3,1); // include parallel ghost
	#else
          realSerialArray & rLocal = r;
	  j1=i1; j2=i2; j3=i3;
        #endif
	for( kd=0; kd<domainDimension(); kd++ ) 
        {
          Index jkd = jv[kd];
	  for( Integer k=rLocal.getBase(kd); k<=rLocal.getBound(kd); k++ ) 
          {
	    jv[kd] = k;
	    rLocal(j1,j2,j3,kd)=gridSpacing(kd)*( k-indexRange(0,kd)+(isCellCentered(kd) ? (Real).5 : (Real)0.) );
	  } // end for
	  jv[kd] = jkd; // reset
	} // end for
// 	realArray r; r.partition(partition); r.redim(d1,d2,d3,d0);
// 	for (kd=0; kd<numberOfDimensions; kd++) {
// 	  for (Integer k=dimensionL(0,kd);
// 	       k<=dimensionL(1,kd); k++) {
// 	    i[kd] = k;
// 	    r(i1,i2,i3,kd) = gridSpacing(kd) * ( k - indexRange(0,kd) + (isCellCentered(kd) ? (Real).5 : (Real)0.) );
// 	  } // end for
// 	  i[kd] = d[kd];
// 	} // end for

	mapping.mapGrid(r, x, xr);

        if( xr.getLength(0)>0 ) xr.reshape(d1,d2,d3,d0,d0);  // make 5D

      } // end if
      if (what & THEcenter)
	computedGeometry |= THEcenter;
      if (what & THEcenterDerivative)
	computedGeometry |= THEcenterDerivative;
    } // end if

    realArray cj; // This might be needed for inverseCenterDerivative.
    if (what & (
      THEcenterJacobian          |
      THEinverseCenterDerivative |
      THEcellVolume              )) {
      if (numberOfPoints) {
	if (what & THEcenterJacobian) {
	  cj.reference(*centerJacobian);
	  // cj.reshape(p);
	} else {
          cj.partition(partition);	
	  cj.redim(d1,d2,d3);
	} // end if
	switch (numberOfDimensions) {
	case 1:
	  cj = xr;
	  break;
	case 2:
	  cj = xr(d1,d2,d3,0,0) * xr(d1,d2,d3,1,1) - xr(d1,d2,d3,0,1) * xr(d1,d2,d3,1,0);
	  break;
	case 3:
	  cj =
	    (xr(d1,d2,d3,0,0)*xr(d1,d2,d3,1,1)-xr(d1,d2,d3,0,1)*xr(d1,d2,d3,1,0))*xr(d1,d2,d3,2,2) +
	    (xr(d1,d2,d3,0,1)*xr(d1,d2,d3,1,2)-xr(d1,d2,d3,0,2)*xr(d1,d2,d3,1,1))*xr(d1,d2,d3,2,0) +
	    (xr(d1,d2,d3,0,2)*xr(d1,d2,d3,1,0)-xr(d1,d2,d3,0,0)*xr(d1,d2,d3,1,2))*xr(d1,d2,d3,2,1);
	  break;
	} // end switch
	if (what & THEcellVolume) {
	  realArray cv;
	  cv.reference(*cellVolume);
	  // cv.reshape(p);
	  cv = (gridSpacing(0) * gridSpacing(1) * gridSpacing(2)) * cj;
	  Real minCellVolume = min(*cellVolume);
	  if (minCellVolume == (Real)0.) cerr
	    << "MappedGridData::computeGeometry():  WARNING:  "
	    << "Computed a zero cellVolume." << endl;
	  /*
	  else if (minCellVolume < (Real)0.) cerr
	    << "MappedGridData::computeGeometry():  WARNING:  "
	    << "Computed a negative cellVolume." << endl;
	  */
	} // end if
      } // end if
      if (what & THEcenterJacobian)
	computedGeometry |= THEcenterJacobian;
      if (what & THEcellVolume)
	computedGeometry |= THEcellVolume;
    } // end if

    realArray cn; // This might be needed for inverseCenterDerivative.
    if (what & (
      THEinverseCenterDerivative |
      THEcenterNormal            |
      THEcenterArea              )) {
      if (numberOfPoints) {
	if (what & THEcenterNormal) {
	  cn.reference(*centerNormal);
	  cn.reshape(d1,d2,d3,d0,d0);
	} else {
          cn.partition(partition);	
	  cn.redim(d1,d2,d3,d0,d0);
          
	} // end if
	switch (numberOfDimensions) {
	case 1:
	  cn = (Real)1.;
	  break;
	case 2: {
	  for (Integer i=0; i<numberOfDimensions; i++)
	    for (Integer l=0; l<numberOfDimensions; l++)
	      cn(d1,d2,d3,l,i) = (gridSpacing(1-i) * (1-2*l) * (1-2*i)) * xr(d1,d2,d3,1-l,1-i);
	} break;
	case 3: {
	  for (Integer i=0; i<numberOfDimensions; i++)
	    for (Integer l=0; l<numberOfDimensions; l++) {
	      Integer j=(i+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
	      cn(d1,d2,d3,l,i) = (gridSpacing(j) * gridSpacing(k)) *
		(xr(d1,d2,d3,m,j) * xr(d1,d2,d3,n,k) - xr(d1,d2,d3,m,k) * xr(d1,d2,d3,n,j));
	    } // end for
	} break;
	} // end switch
      } // end if
      if (what & THEcenterNormal) computedGeometry |= THEcenterNormal;
      if (what & THEcenterArea) {
	if (numberOfPoints) {
	  realArray ca;
	  ca.reference(*centerArea);
	  ca.reshape(d1,d2,d3,one,d0);
	  switch (numberOfDimensions) {
	  case 1:
	    ca = (Real)1.;
	    break;
	  case 2: {
	    for (Integer l=0; l<numberOfDimensions; l++)
	      ca(d1,d2,d3,0,l) = sqrt
		(cn(d1,d2,d3,0,l) * cn(d1,d2,d3,0,l) + cn(d1,d2,d3,1,l) * cn(d1,d2,d3,1,l));
	  } break;
	  case 3: {
	    for (Integer l=0; l<numberOfDimensions; l++)
	      ca(d1,d2,d3,0,l) = sqrt
		( cn(d1,d2,d3,0,l) * cn(d1,d2,d3,0,l)
		  + cn(d1,d2,d3,1,l) * cn(d1,d2,d3,1,l)
		  + cn(d1,d2,d3,2,l) * cn(d1,d2,d3,2,l) );
	  } break;
	  } // end switch
	} // end if
	computedGeometry |= THEcenterArea;
      } // end if
    } // end if

    if (what & THEinverseCenterDerivative) {
      if (numberOfPoints) {
	realArray det, rx; det.partition(partition); det.redim(d1,d2,d3);
	rx.reference(*inverseCenterDerivative);
	rx.reshape(d1,d2,d3,d0,d0);
	where (cj == (Real)0.) det = (Real)1.;
	otherwise() det = (Real)1. / cj;
	switch (numberOfDimensions) {
	case 1:
	  rx = det;
	  break;
	case 2: {
	  for (Integer i=0; i<2; i++)
	    for (Integer l=0; l<2; l++)
	      rx(d1,d2,d3,i,l) = ((Real)1. / gridSpacing(1-i)) * cn(d1,d2,d3,l,i) * det;
	} break;
	case 3: {
	  for (Integer i=0; i<numberOfDimensions; i++)
	    for (Integer l=0; l<numberOfDimensions; l++) {
	      Integer j=(i+1)%3, k=(j+1)%3;
	      rx(d1,d2,d3,i,l) = ((Real)1. / (gridSpacing(j) * gridSpacing(k))) * cn(d1,d2,d3,l,i) * det;
	    } // end for
	} break;
	} // end switch
      } // end if
      computedGeometry |= THEinverseCenterDerivative;
    } // end if
  } // end if

  if (!isAllCellCentered && what & THEcorner) {
//
//          Compute geometry at the corners.
//
    realArray x;
    x.reference(*corner);

    Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
    Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
    IntegerArray dimensionL(2,3);
    Integer numberOfPoints = 1;
    for (kd=0; kd<3; kd++) {
      dimensionL(0,kd) = x.getBase(kd),
	dimensionL(1,kd) = x.getBound(kd);
      i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
      numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
    } // end for
    // Range p = numberOfPoints;

    if (numberOfPoints) 
    {
      // x.reshape(p,d0);
      // *wdh* 060523: assign the local arrays to avoid communication. 
      realArray r; r.partition(partition); r.redim(d1,d2,d3,domainDimension());
      Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
      #ifdef USE_PPP
        realSerialArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
        j1=i1; j2=i2; j3=i3;
        bool ok = ParallelUtility::getLocalArrayBounds(r,rLocal,j1,j2,j3,1); // include parallel ghost
      #else
        realSerialArray & rLocal = r;
        j1=i1; j2=i2; j3=i3;
      #endif
      for( kd=0; kd<domainDimension(); kd++ ) 
      {
        Index jkd = jv[kd];
	for( Integer k=rLocal.getBase(kd); k<=rLocal.getBound(kd); k++ ) 
	{
	  jv[kd] = k;
	  rLocal(j1,j2,j3,kd)=gridSpacing(kd)*(k-indexRange(0,kd)-(isCellCentered(kd) ? (Real)0. : (Real).5) );
	} // end for
	jv[kd] = jkd; // reset
      } // end for

//       realArray r; r.partition(partition); r.redim(d1,d2,d3,d0);  
//       for (kd=0; kd<numberOfDimensions; kd++) {
// 	for (Integer k=dimensionL(0,kd);
// 	     k<=dimensionL(1,kd); k++) {
// 	  i[kd] = k;
// 	  r(i1,i2,i3,kd) = gridSpacing(kd) *
// 	    ( k - indexRange(0,kd)
// 	      - (isCellCentered(kd) ? (Real)0. : (Real).5) );
// 	} // end for
// 	i[kd] = d[kd];
//       } // end for

      mapping.mapGrid(r, x);
    } // end if
    computedGeometry |= THEcorner;
  } // end if

  if (what & (
	      THEfaceNormal |
	      THEfaceArea   )) {
//
//          Compute geometry at the cell faces.
//
    RealDistributedArray x;
    x.partition(partition); x.redim(d1D,d2D,d3D,d0);
    Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
    Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
    IntegerArray dimensionL(2,3);
    Integer numberOfPoints = 1;
    for (kd=0; kd<3; kd++) {
      dimensionL(0,kd) = x.getBase(kd),
	dimensionL(1,kd) = x.getBound(kd);
      i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
      numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
    } // end for
    // Range p = numberOfPoints;

    realArray fn;
    if (what & THEfaceNormal) {
      fn.reference(*faceNormal);
      fn.reshape(d1,d2,d3,d0,d0);
    } else if (numberOfPoints) {
      fn.partition(partition);	
      fn.redim(d1,d2,d3,d0,d0);
    } // end if

    bool ok;
    if (numberOfPoints)
    {
      for (kd=0; kd<numberOfDimensions; kd++) 
      {
        realArray r; r.partition(partition); r.redim(d1,d2,d3,domainDimension());
        Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
        #ifdef USE_PPP
          realSerialArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
          j1=i1; j2=i2; j3=i3;
          ok = ParallelUtility::getLocalArrayBounds(r,rLocal,j1,j2,j3,1); // include parallel ghost
        #else
          realSerialArray & rLocal = r;
          j1=i1; j2=i2; j3=i3;
        #endif
        for( int kd1=0; kd1<domainDimension(); kd1++ ) 
        {
          Index jkd1 = jv[kd1];
	  for( Integer k=rLocal.getBase(kd1); k<=rLocal.getBound(kd1); k++ ) 
	  {
	    jv[kd1] = k;
	    rLocal(j1,j2,j3,kd)=gridSpacing(kd1)*( k - indexRange(0,kd1)
		+ (kd1 == kd           ? (Real)0. : (Real).5)
		- (isCellCentered(kd1) ? (Real)0. : (Real).5) );
	  } // end for
	  jv[kd1] = jkd1; // reset
	} // end for

//       realArray r; r.partition(partition); r.redim(d1,d2,d3,d0);  
//       for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
// 	for (Integer k=dimensionL(0,kd1); k<=dimensionL(1,kd1); k++) {
// 	  i[kd1] = k;
// 	  r(i1,i2,i3,kd1) = gridSpacing(kd1) *
// 	    ( k - indexRange(0,kd1)
// 	      + (kd1 == kd           ? (Real)0. : (Real).5)
// 	      - (isCellCentered(kd1) ? (Real)0. : (Real).5) );
// 	} // end for
// 	i[kd1] = d[kd1];
//       } // end for

        realArray xr; xr.partition(partition); xr.redim(d1,d2,d3,SQR(numberOfDimensions));

	mapping.mapGrid(r, x, xr);

	xr.reshape(d1,d2,d3,d0,d0);

	Index I1=d1, I2=d2, I3=d3;
        #ifdef USE_PPP
          realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
          realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(fn,fnLocal);
	#else
          realSerialArray & xrLocal=xr;
          realSerialArray & fnLocal=fn;
        #endif
	ok = ParallelUtility::getLocalArrayBounds(xr,xrLocal,I1,I2,I3,1); // include parallel ghost
	if( ok )
	{
	  switch (numberOfDimensions) 
	  {
	  case 1:
	    fnLocal = (Real)1.;
	    break;
	  case 2: 
	  {
	    for (Integer l=0; l<numberOfDimensions; l++)
	      fnLocal(I1,I2,I3,l,kd) = (gridSpacing(1-kd) * (1-2*l) * (1-2*kd)) * xrLocal(I1,I2,I3,1-l,1-kd);
	  }
	  break;
	  case 3: 
	  {
	    for (Integer l=0; l<numberOfDimensions; l++) 
	    {
	      Integer j=(kd+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
	      fnLocal(I1,I2,I3,l,kd) = (gridSpacing(j) * gridSpacing(k)) *
		(xrLocal(I1,I2,I3,m,j) * xrLocal(I1,I2,I3,n,k) - xrLocal(I1,I2,I3,m,k) * xrLocal(I1,I2,I3,n,j));
	    } // end for
	  } break;
	  } // end switch
	}
	
      } // end for
    } // end if
    
    if (what & THEfaceNormal) computedGeometry |= THEfaceNormal;
    x.redim(0); // Save some space.

    if (what & THEfaceArea) {
      if (numberOfPoints) {
	realArray fa;
	fa.reference(*faceArea);
	fa.reshape(d1,d2,d3,one,d0);
	Index I1=d1, I2=d2, I3=d3;
#ifdef USE_PPP
	realSerialArray faLocal;
	getLocalArrayWithGhostBoundaries(fa,faLocal);
	realSerialArray fnLocal;
	getLocalArrayWithGhostBoundaries(fn,fnLocal);
#else
	realSerialArray & faLocal=fa;
	realSerialArray & fnLocal=fn;
#endif
	bool ok = ParallelUtility::getLocalArrayBounds(fa,faLocal,I1,I2,I3,1); //include parallel ghost
	 
	if(ok)
	  switch (numberOfDimensions) {
	  case 1:
	    faLocal = (Real)1.;
	    break;
	  case 2: {
	    for (Integer l=0; l<numberOfDimensions; l++)
	      faLocal(I1,I2,I3,0,l) = sqrt (fnLocal(I1,I2,I3,0,l) * fnLocal(I1,I2,I3,0,l) + fnLocal(I1,I2,I3,1,l) * fnLocal(I1,I2,I3,1,l));
	  } break;
	  case 3: {
	    for (Integer l=0; l<numberOfDimensions; l++)
	      faLocal(I1,I2,I3,0,l) = sqrt
		( fnLocal(I1,I2,I3,0,l) * fnLocal(I1,I2,I3,0,l)
		  + fnLocal(I1,I2,I3,1,l) * fnLocal(I1,I2,I3,1,l)
		  + fnLocal(I1,I2,I3,2,l) * fnLocal(I1,I2,I3,2,l) );
	  } break;
	  } // end switch
      } // end if
      computedGeometry |= THEfaceArea;
    } // end if
  } // end if

  if (what &
      THEvertexBoundaryNormal || (
	isAllVertexCentered && (what &
				THEcenterBoundaryNormal || (
				  numberOfDimensions > 1 && what &
				  THEcenterBoundaryTangent)))) {
//
//          Compute geometry at the boundary vertices.
//
    #ifdef USE_PPP
     // ***** parallel version *****

//       printf("MappedGrid::computeGeometryFromMapping:ERROR: the vertexBoundaryNormal cannot currently "
//              "  be computed for parallel computations. Use the inverseVertexDerivative instead for e.g. \n");
//       Overture::abort("error");
    
    Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2];
    Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
    for (kd=0; kd<numberOfDimensions; kd++)
    {
      for (Integer ks=0; ks<2; ks++) 
      {
	IntegerArray dimensionL(2,3);
	realSerialArray fn;
	if( what & THEvertexBoundaryNormal )
	  fn.reference(*pVertexBoundaryNormal[kd][ks]);
	else if( what & THEcenterBoundaryNormal )
	  fn.reference(*pCenterBoundaryNormal[kd][ks]);
	else 
          fn.reference(*pCenterBoundaryTangent[kd][ks]);
	Integer numberOfPoints = 1;
	for (Integer kd1=0; kd1<3; kd1++) 
        {
	  dimensionL(0,kd1) = fn.getBase(kd1);
	  dimensionL(1,kd1) = fn.getBound(kd1);
	  i[kd1] = d[kd1] = Range(dimensionL(0,kd1), dimensionL(1,kd1));
	  numberOfPoints *= dimensionL(1,kd1) - dimensionL(0,kd1) + 1;
	} // end for

        realSerialArray xr; 
	if( numberOfPoints>0 ) 
        {
	  realSerialArray r;
          r.redim(d1,d2,d3,d0);  
          Integer kd1;
	  for (kd1=0; kd1<numberOfDimensions; kd1++)
	  {
	    if (kd1==kd) 
            {
	      r(i1,i2,i3,kd1) = (Real)ks;
	    }
            else 
            {
	      for (Integer k=dimensionL(0,kd1); k<=dimensionL(1,kd1); k++) 
              {
		i[kd1] = k;
		r(i1,i2,i3,kd1) = gridSpacing(kd1) * (k - indexRange(0,kd1));
	      } // end for
	      i[kd1] = d[kd1];
	    } // end if
	  }// end for
	  
          xr.redim(d1,d2,d3,SQR(numberOfDimensions));
          // *wdh* at singularities we use the normal from a point near the end. Otherwise we could have NaN's
          // Also the normal would turn inside out at the ghost points
	  for (kd1=0; kd1<numberOfDimensions; kd1++)
	  {
            if( kd1!=kd )
	    {
	      if( mapping.getTypeOfCoordinateSingularity(Start,kd1)==Mapping::polarSingularity )
  	        r(d1,d2,d3,kd1)=max(.001,r(d1,d2,d3,kd1));
              if( mapping.getTypeOfCoordinateSingularity(End  ,kd1)==Mapping::polarSingularity )
	        r(d1,d2,d3,kd1)=min(.999,r(d1,d2,d3,kd1));
	    }
	  }
	  mapping.getMapping().mapGridS(r, Overture::nullRealArray(), xr);
	  r.redim(0); 
          xr.reshape(d1,d2,d3,d0,d0);
	}
	else // numberOfPoints==0
	{
          // If the map function is distributed we need to call it on all processors *wdh* 110821
	  mapping.getMapping().mapGridS(Overture::nullRealArray(), Overture::nullRealArray(), Overture::nullRealArray());
	} // end if( numberOfpoints )
	

	if( what & THEvertexBoundaryNormal || (isAllVertexCentered && what & THEcenterBoundaryNormal) )
	{
	  //  Get the sign of the jacobian 
	  Real jac = mapping.getSignForJacobian()*(2*ks-1);  // for outward normal
	  if( numberOfPoints>0 ) 
	  {
	    realSerialArray fnn;
            fnn.redim(d1,d2,d3); fn.reshape(d1,d2,d3,d0);
	    switch (numberOfDimensions) 
            {
	    case 1:
	      fn = jac;
	      break;
	    case 2: 
            {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++)
		fn(d1,d2,d3,l) = (Real)((1-2*l)*(1-2*kd)) * xr(d1,d2,d3,1-l,1-kd);
              // normalize to have unit length:
	      fnn(d1,d2,d3) = jac / max(realSmall, sqrt( SQR(fn(d1,d2,d3,0)) + SQR(fn(d1,d2,d3,1)) ) );
	      for (l=0; l<numberOfDimensions; l++) fn(d1,d2,d3,l) *= fnn(d1,d2,d3);
	    } break;
	    case 3: 
            {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++) {
		Integer j=(kd+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
		fn(d1,d2,d3,l) = xr(d1,d2,d3,m,j)*xr(d1,d2,d3,n,k) - xr(d1,d2,d3,m,k)*xr(d1,d2,d3,n,j);
	      } // end for
              // normalize to have unit length:
	      fnn(d1,d2,d3) = jac / 
                    max(realSmall, sqrt(SQR(fn(d1,d2,d3,0)) + SQR(fn(d1,d2,d3,1)) + SQR(fn(d1,d2,d3,2))) );
	      for (l=0; l<numberOfDimensions; l++) fn(d1,d2,d3,l) *= fnn(d1,d2,d3);
	    } break;
	    } // end switch
	  } // end if( numberOfPoints>0 )
	} //end if
	if( numberOfPoints>0 ) 
	{
	  if( numberOfDimensions > 1 && what & THEcenterBoundaryTangent ) 
	  {
	    fn.reference(*pCenterBoundaryTangent[kd][ks]);
	    const Range d0m = numberOfDimensions - 1; Integer kd1;
	    realSerialArray fnn;
	    fnn.redim(d1,d2,d3,one,d0m); fn.reshape(d1,d2,d3,d0,d0m);
	    for (kd1=0; kd1<numberOfDimensions-1; kd1++) 
	    {
	      Integer kd2 = (kd + kd1 + 1) % numberOfDimensions;
	      fn(d1,d2,d3,d0,kd1) = xr(d1,d2,d3,d0,kd2);
	    } // end for
	    switch (numberOfDimensions) {
	    case 2:
	      // normalize to have unit length:
	      fnn(d1,d2,d3,0,d0m) = 1./max(realSmall, sqrt( SQR(fn(d1,d2,d3,0,d0m)) + SQR(fn(d1,d2,d3,1,d0m)) ) );
	      break;
	    case 3:
	      // normalize to have unit length:
	      fnn(d1,d2,d3,0,d0m)=1./max(realSmall,
					 sqrt( SQR(fn(d1,d2,d3,0,d0m))+SQR(fn(d1,d2,d3,1,d0m))+SQR(fn(d1,d2,d3,2,d0m)) ) );
	      break;
	    } // end switch
	    for (kd1=0; kd1<numberOfDimensions; kd1++)
	      fn(d1,d2,d3,kd1,d0m) *= fnn(d1,d2,d3,0,d0m);

	  } // end if
	}  // end if( numberOfPoints>0 )
	
      } // end for ks
    } // end for kd 

    #else
      // ***** serial version *****


    Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2];
    Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
    for (kd=0; kd<numberOfDimensions; kd++)
    {
      for (Integer ks=0; ks<2; ks++) 
      {
	IntegerArray dimensionL(2,3);
	realArray fn;
	if (what & THEvertexBoundaryNormal)
	  fn.reference(*vertexBoundaryNormal[kd][ks]);
	else if (what & THEcenterBoundaryNormal)
	  fn.reference(*centerBoundaryNormal[kd][ks]);
	else fn.reference(*centerBoundaryTangent[kd][ks]);
	Integer numberOfPoints = 1;
	for (Integer kd1=0; kd1<3; kd1++) 
        {
	  dimensionL(0,kd1) = fn.getBase(kd1);
	  dimensionL(1,kd1) = fn.getBound(kd1);
	  i[kd1] = d[kd1] = Range(dimensionL(0,kd1), dimensionL(1,kd1));
	  numberOfPoints *= dimensionL(1,kd1) - dimensionL(0,kd1) + 1;
	} // end for
	if (numberOfPoints) 
        {
	  // Range p = numberOfPoints;
	  realArray r; // r.partition(partition);  // we don't use the normal partition on the boundary
          r.redim(d1,d2,d3,d0);  
          Integer kd1;
	  for (kd1=0; kd1<numberOfDimensions; kd1++)
	    if (kd1==kd) {
	      r(i1,i2,i3,kd1) = (Real)ks;
	    } else {
	      for (Integer k=dimensionL(0,kd1);
		   k<=dimensionL(1,kd1); k++) {
		i[kd1] = k;
		r(i1,i2,i3,kd1) = gridSpacing(kd1) *
		  (k - indexRange(0,kd1));
	      } // end for
	      i[kd1] = d[kd1];
	    } // end if, end for
	  // *wdh*  r.reshape(p,d0); RealArray x(p,d0), xr(p,d0,d0);
	  // r.reshape(p,d0); 
          realArray xr; // xr.partition(partition); // we don't use the normal partition on the boundary
          xr.redim(d1,d2,d3,SQR(numberOfDimensions));
          // *wdh* at singularities we use the normal from a point near the end. Otherwise we could have NaN's
          // Also the normal would turn inside out at the ghost points
	  for (kd1=0; kd1<numberOfDimensions; kd1++)
	  {
            if( kd1!=kd )
	    {
	      if( mapping.getTypeOfCoordinateSingularity(Start,kd1)==Mapping::polarSingularity )
  	        r(d1,d2,d3,kd1)=max(.001,r(d1,d2,d3,kd1));
              if( mapping.getTypeOfCoordinateSingularity(End  ,kd1)==Mapping::polarSingularity )
	        r(d1,d2,d3,kd1)=min(.999,r(d1,d2,d3,kd1));
	    }
	  }
	  mapping.mapGrid(r, Overture::nullRealDistributedArray(), xr);
	  r.redim(0); // *wdh* x.redim(0);
          xr.reshape(d1,d2,d3,d0,d0);

	  if (what &
	      THEvertexBoundaryNormal ||
	      (isAllVertexCentered && what &
	       THEcenterBoundaryNormal)) {
//                      Get the sign of the jacobian at the center of the grid.
//	    Integer q = numberOfPoints / 2;
            Real jac = ks ? 1. : -1.;
            const int q1=(d1.getBase()+d1.getBound())/2;
            const int q2=(d2.getBase()+d2.getBound())/2;
            const int q3=(d3.getBase()+d3.getBound())/2;
	    
	    switch (numberOfDimensions) {
	    case 1:
	      jac *= xr(q1,q2,q3,0,0);
	      break;
	    case 2:
	      jac *= xr(q1,q2,q3,0,0) * xr(q1,q2,q3,1,1) - xr(q1,q2,q3,0,1) * xr(q1,q2,q3,1,0);
	      break;
	    case 3:
	      jac *=
		(xr(q1,q2,q3,0,0)*xr(q1,q2,q3,1,1)-xr(q1,q2,q3,0,1)*xr(q1,q2,q3,1,0))*xr(q1,q2,q3,2,2) +
		(xr(q1,q2,q3,0,1)*xr(q1,q2,q3,1,2)-xr(q1,q2,q3,0,2)*xr(q1,q2,q3,1,1))*xr(q1,q2,q3,2,0) +
		(xr(q1,q2,q3,0,2)*xr(q1,q2,q3,1,0)-xr(q1,q2,q3,0,0)*xr(q1,q2,q3,1,2))*xr(q1,q2,q3,2,1);
	      break;
	    } // end switch
	    jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
	    realArray fnn; // fnn.partition(partition); // we don't use the normal partition on the boundary
            fnn.redim(d1,d2,d3); fn.reshape(d1,d2,d3,d0);
	    switch (numberOfDimensions) {
	    case 1:
	      fn = jac;
	      break;
	    case 2: {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++)
		fn(d1,d2,d3,l) = (Real)((1-2*l)*(1-2*kd)) * xr(d1,d2,d3,1-l,1-kd);
	      fnn(d1,d2,d3) = jac / max(realSmall, sqrt(fn(d1,d2,d3,0) * fn(d1,d2,d3,0) +
                            fn(d1,d2,d3,1) * fn(d1,d2,d3,1)) );
	      for (l=0; l<numberOfDimensions; l++) fn(d1,d2,d3,l) *= fnn(d1,d2,d3);
	    } break;
	    case 3: {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++) {
		Integer j=(kd+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
		fn(d1,d2,d3,l) = xr(d1,d2,d3,m,j)*xr(d1,d2,d3,n,k) - xr(d1,d2,d3,m,k)*xr(d1,d2,d3,n,j);
	      } // end for
	      fnn(d1,d2,d3) = jac / max(realSmall, sqrt(fn(d1,d2,d3,0) * fn(d1,d2,d3,0) + 
                       fn(d1,d2,d3,1) * fn(d1,d2,d3,1) + fn(d1,d2,d3,2) * fn(d1,d2,d3,2)) );
	      for (l=0; l<numberOfDimensions; l++) fn(d1,d2,d3,l) *= fnn(d1,d2,d3);
	    } break;
	    } // end switch
	  } //end if
	  if (numberOfDimensions > 1 &&
	      what & THEcenterBoundaryTangent) {

	    fn.reference(*centerBoundaryTangent[kd][ks]);
	    const Range d0m = numberOfDimensions - 1; Integer kd1;
	    realArray fnn; // fnn.partition(partition); 
            fnn.redim(d1,d2,d3,one,d0m); fn.reshape(d1,d2,d3,d0,d0m);
	    for (kd1=0; kd1<numberOfDimensions-1; kd1++) {
	      Integer kd2 = (kd + kd1 + 1) % numberOfDimensions;
	      fn(d1,d2,d3,d0,kd1) = xr(d1,d2,d3,d0,kd2);
	    } // end for
	    switch (numberOfDimensions) {
	    case 2:
	      fnn(d1,d2,d3,0,d0m) = (Real)1. / max(realSmall, sqrt( fn(d1,d2,d3,0,d0m)*fn(d1,d2,d3,0,d0m)+
                               fn(d1,d2,d3,1,d0m)*fn(d1,d2,d3,1,d0m) ) );
	      break;
	    case 3:
	      fnn(d1,d2,d3,0,d0m)=1./max(realSmall,
                sqrt( fn(d1,d2,d3,0,d0m)*fn(d1,d2,d3,0,d0m)+fn(d1,d2,d3,1,d0m)*fn(d1,d2,d3,1,d0m)+
                   fn(d1,d2,d3,2,d0m)*fn(d1,d2,d3,2,d0m) ) );
	      break;
	    } // end switch
	    for (kd1=0; kd1<numberOfDimensions; kd1++)
	      fn(d1,d2,d3,kd1,d0m) *= fnn(d1,d2,d3,0,d0m);
	  } // end if
	} // end if
      } // end for ks
    } // end for kd 
    

    // **** end serial version *****
    #endif

    if (what & THEvertexBoundaryNormal)
      computedGeometry |= THEvertexBoundaryNormal;
    if (isAllVertexCentered && what & THEcenterBoundaryNormal)
      computedGeometry |= THEcenterBoundaryNormal;
    if (isAllVertexCentered && numberOfDimensions > 1 &&
	what & THEcenterBoundaryTangent)
      computedGeometry |= THEcenterBoundaryTangent;
  } // end if

  if (!isAllVertexCentered && (what &
			       THEcenterBoundaryNormal || (
				 numberOfDimensions > 1 && what &
				 THEcenterBoundaryTangent))) {
//
//          Compute geometry at the boundary centers.
//
    Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2];
    Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
    for (kd=0; kd<numberOfDimensions; kd++)
      for (Integer ks=0; ks<2; ks++) {
	IntegerArray dimensionL(2,3);
	realArray fn;
	if (what & THEcenterBoundaryNormal)
	  fn.reference(*centerBoundaryNormal[kd][ks]);
	else fn.reference(*centerBoundaryTangent[kd][ks]);
	Integer numberOfPoints = 1;
	for (Integer kd1=0; kd1<3; kd1++) {
	  dimensionL(0,kd1) = fn.getBase(kd1);
	  dimensionL(1,kd1) = fn.getBound(kd1);
	  i[kd1] = d[kd1] = 
	    Range(dimensionL(0,kd1), dimensionL(1,kd1));
	  numberOfPoints *=
	    dimensionL(1,kd1) - dimensionL(0,kd1) + 1;
	} // end for
	if (numberOfPoints) {
	  // Range p = numberOfPoints;
	  realArray r(d1,d2,d3,d0);
	  for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
	    if (kd1==kd) {
	      r(i1,i2,i3,kd1) = (Real)ks;
	    } else {
	      for (Integer k=dimensionL(0,kd1);
		   k<=dimensionL(1,kd1); k++) {
		i[kd1] = k;
		r(i1,i2,i3,kd1) = gridSpacing(kd1) *
		  ( k - indexRange(0,kd1)
		    + (isCellCentered(kd1) ? (Real).5 : (Real)0.) );
	      } // end for
	      i[kd1] = d[kd1];
	    } // end if, end for
	  // r.reshape(p,d0); 
          realArray x(d1,d2,d3,d0), xr(d1,d2,d3,SQR(numberOfDimensions));
	  mapping.mapGrid(r, x, xr);
	  r.redim(0); x.redim(0);
	  xr.reshape(d1,d2,d3,d0,d0);
	  
	  if (what & THEcenterBoundaryNormal) {
//                      Get the sign of the jacobian at the center of the grid.
//	    Integer q = numberOfPoints / 2;
            const int q1=(d1.getBase()+d1.getBound())/2;
            const int q2=(d2.getBase()+d2.getBound())/2;
            const int q3=(d3.getBase()+d3.getBound())/2;

	    Real jac = ks ? 1. : -1.;
	    switch (numberOfDimensions) {
	    case 1:
	      jac *= xr(q1,q2,q3,0,0);
	      break;
	    case 2:
	      jac *= xr(q1,q2,q3,0,0)*xr(q1,q2,q3,1,1) - xr(q1,q2,q3,0,1)*xr(q1,q2,q3,1,0);
	      break;
	    case 3:
	      jac *= (xr(q1,q2,q3,0,0)*xr(q1,q2,q3,1,1) - xr(q1,q2,q3,0,1)*xr(q1,q2,q3,1,0)) *
		xr(q1,q2,q3,2,2) +
		(xr(q1,q2,q3,0,1)*xr(q1,q2,q3,1,2) - xr(q1,q2,q3,0,2)*xr(q1,q2,q3,1,1)) *
		xr(q1,q2,q3,2,0) +
		(xr(q1,q2,q3,0,2)*xr(q1,q2,q3,1,0) - xr(q1,q2,q3,0,0)*xr(q1,q2,q3,1,2)) *
		xr(q1,q2,q3,2,1);
	      break;
	    } // end switch
	    jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
	    realArray fnn(d1,d2,d3); fn.reshape(d1,d2,d3,d0);
	    switch (numberOfDimensions) {
	    case 1:
	      fn = jac;
	      break;
	    case 2: {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++)
		fn(d1,d2,d3,l,0) =
		  (Real)((1-2*l)*(1-2*kd)) * xr(d1,d2,d3,1-l,1-kd);
	      fnn(d1,d2,d3) = jac / max(realSmall, sqrt
		(fn(d1,d2,d3,0) * fn(d1,d2,d3,0) + fn(d1,d2,d3,1) * fn(d1,d2,d3,1)) );
	      for (l=0; l<numberOfDimensions; l++)
		fn(d1,d2,d3,l) = fn(d1,d2,d3,l) * fnn(d1,d2,d3);
	    } break;
	    case 3: {
              Integer l;
	      for (l=0; l<numberOfDimensions; l++) {
		Integer j=(kd+1)%3, k=(j+1)%3,
		  m=(l +1)%3, n=(m+1)%3;
		fn(d1,d2,d3,l) =   xr(d1,d2,d3,m,j) * xr(d1,d2,d3,n,k) - xr(d1,d2,d3,m,k) * xr(d1,d2,d3,n,j);
	      } // end for
	      fnn(d1,d2,d3) = jac / max(realSmall, sqrt(fn(d1,d2,d3,0) * fn(d1,d2,d3,0) +
				  fn(d1,d2,d3,1) * fn(d1,d2,d3,1) + fn(d1,d2,d3,2) * fn(d1,d2,d3,2)) );
	      for (l=0; l<numberOfDimensions; l++)
		fn(d1,d2,d3,l) *= fnn(d1,d2,d3);
	    } break;
	    } // end switch
	  } // end if
	  if (numberOfDimensions > 1 &&
	      what & THEcenterBoundaryTangent) {
	    fn.reference(*centerBoundaryTangent[kd][ks]);
	    const Range d0m = numberOfDimensions - 1; Integer kd1;
	    realArray fnn(d1,d2,d3,one,d0m); fn.reshape(d1,d2,d3,d0,d0m);
	    for (kd1=0; kd1<numberOfDimensions-1; kd1++) {
	      Integer kd2 = (kd + kd1 + 1) % numberOfDimensions;
	      fn(d1,d2,d3,d0,kd1) = xr(d1,d2,d3,d0,kd2);
	    } // end for
	    switch (numberOfDimensions) {
	    case 2:
	      fnn(d1,d2,d3,0,d0m) = (Real)1. / max(realSmall, sqrt
		( fn(d1,d2,d3,0,d0m) * fn(d1,d2,d3,0,d0m) +
		  fn(d1,d2,d3,1,d0m) * fn(d1,d2,d3,1,d0m) ) );
	      break;
	    case 3:
	      fnn(d1,d2,d3,0,d0m) = (Real)1. / max(realSmall, sqrt
		( fn(d1,d2,d3,0,d0m) * fn(d1,d2,d3,0,d0m) +
		  fn(d1,d2,d3,1,d0m) * fn(d1,d2,d3,1,d0m) +
		  fn(d1,d2,d3,2,d0m) * fn(d1,d2,d3,2,d0m) ) );
	      break;
	    } // end switch
	    for (kd1=0; kd1<numberOfDimensions; kd1++)
	      fn(d1,d2,d3,kd1,d0m) *= fnn(d1,d2,d3,0,d0m);
	  } // end if
	} // end if
      } // end for, end for
    if (what & THEcenterBoundaryNormal)
      computedGeometry |= THEcenterBoundaryNormal;
    if (numberOfDimensions > 1 && what & THEcenterBoundaryTangent)
      computedGeometry |= THEcenterBoundaryTangent;
  } // end if

  return returnValue;
}
