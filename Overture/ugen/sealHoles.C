// #define BOUNDS_CHECK
#define OV_DEBUG
#include "Overture.h"
#include "Ugen.h"
#include "Geom.h"
#include "IntersectionMapping.h"
#include "ReductionMapping.h"
#include "ArraySimple.h"
#include "MappingProjectionParameters.h"
#include "AdvancingFront.h"
#include "CompositeSurface.h"

#include "PlotIt.h"

static bool newFaceOrientation(CompositeGrid &cg, 
			       const intArray &boundaryHoleVertices, 
			       int vert, 
			       const realArray &p1, const realArray &p2)
{

  // return true of p1 and p2 are oriented as a face pointing into the mesh

  Range AXES(p1.getLength(0));

  int grid = boundaryHoleVertices(vert, 0);
  int axis = boundaryHoleVertices(vert, 1);
  int side = boundaryHoleVertices(vert, 2);

  int i1 = boundaryHoleVertices(vert, 3);
  int i2 = boundaryHoleVertices(vert, 4);
  int i3 = boundaryHoleVertices(vert, 5);

  int ii[3];
  ii[0] = ii[1] = ii[2] = 0;
  ii[axis] = 1;
  
  realArray p3;
  if ( side== 0 ) 
    p3 = cg[grid].vertex() ( i1+ii[0],
			     i2+ii[1],
			     i3, AXES );
  else
    p3 = cg[grid].vertex() ( i1-ii[0],
			     i2-ii[1],
			     i3, AXES );
  
  p3.reshape(AXES);

  ArraySimple<real> pp1(3),pp2(3),pp3(3);
  for ( int a=0; a<p1.getLength(0); a++ )
    {
      pp1(a) = p1(a);
      pp2(a) = p2(a);
      pp3(a) = p3(a);
    }

  bool result = true;
  if ( cg[grid].rangeDimension()!=3 )
    result = triangleArea2D(pp1,pp2,pp3)>0.0;
  else
    {
      ArraySimple<real> n(3),e(3);
      ArraySimpleFixed<real,2,1,1,1> pt1,pt2,pt3;
      n = areaNormal3D(pp1,pp2,pp3);
      real nmag = sqrt(ASmag2(n));
      n[0] /= nmag;
      n[1] /= nmag;
      n[2] /= nmag;
      MappingProjectionParameters mp;
      mp.getRealArray(MappingProjectionParameters::r).redim(1,2);
      mp.getRealArray(MappingProjectionParameters::r) = -1;
      mp.getRealArray(MappingProjectionParameters::normal).redim(1,3);
      p3.reshape(1,3);
      cg[grid].mapping().mapPointer->project(p3,mp);
      
      realArray & sn = mp.getRealArray(MappingProjectionParameters::normal);
      result = cg[grid].mapping().mapPointer->getSignForJacobian()*(n[0]*sn(0,0) + n[1]*sn(0,1) + n[2]*sn(0,2))>0.0;

      //      cout<<"orientation test value "<<cg[grid].mapping().mapPointer->getSignForJacobian()*(n[0]*sn(0,0) + n[1]*sn(0,1) + n[2]*sn(0,2))<<endl;
    }


  return result;

}

static void accumulateBoundaryHoleVertices(const CompositeGrid &cg, 
					   intArray &boundaryHoleVertices)
{
  // gather the vertices that are on gaps in the advancing front boundary

  int grid; // we use this variable a lot

  // boundaryHoleVertices ( *, 0 ) - grid
  // boundaryHoleVertices ( *, 1 ) - axis
  // boundaryHoleVertices ( *, 2 ) - side
  // boundaryHoleVertices ( *, 3 ) - i1
  // boundaryHoleVertices ( *, 4 ) - i2
  // boundaryHoleVertices ( *, 5 ) - i3


  // first count the number of boundary hole vertices exist in cg
  int bdyVert = 0;
  int aa;
  for ( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const MappedGrid &mappedGrid = cg[grid];
      const intArray &mask = mappedGrid.mask();

      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

      // offset is used to compute the neighboring vertices of a 
      // vertex on boundary axis, side
      int offset[3];
      offset[0] = offset[1] = offset[2] = 0;

      int side, axis;
      for ( axis=0; axis<mappedGrid.domainDimension(); axis++ )
	{
	  for ( aa=0; aa<mappedGrid.domainDimension(); aa++ ) 
	    if ( aa!=axis ) offset[aa] = 1;

	  for ( side=0; side<2 && !(mappedGrid.isPeriodic(axis)); side++ ) 
	    {
	      getBoundaryIndex( mappedGrid.gridIndexRange(), side, axis, I1,I2,I3 );
	      
	      int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
	      for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      if ( mask(i1,i2,i3) != 0 && mappedGrid.boundaryFlag(side,axis)==MappedGrid::physicalBoundary)
			if (// log this vertex only if one of its neighbors is masked out	    
			    // immediate neighbors  
			    mask(i1+offset[0], i2, i3)==0 || mask(i1-offset[0], i2, i3)==0 ||
			    mask(i1, i2+offset[1], i3)==0 || mask(i1, i2-offset[1], i3)==0 ||
			    mask(i1, i2, i3+offset[2])==0 || mask(i1, i2, i3-offset[2])==0 ||
			    
			    // diagonal neighbors, i1,i2
			    mask(i1+offset[0], i2+offset[1], i3)==0 || 
			    mask(i1-offset[0], i2-offset[1], i3)==0 || 
			    mask(i1+offset[0], i2-offset[1], i3)==0 || 
			    mask(i1-offset[0], i2+offset[1], i3)==0 || 
			    // diagonal neighbors, i1,i3		    
			    mask(i1+offset[0], i2, i3+offset[2])==0 || 
			    mask(i1-offset[0], i2, i3-offset[2])==0 || 
			    mask(i1+offset[0], i2, i3-offset[2])==0 || 
			    mask(i1-offset[0], i2, i3+offset[2])==0 || 
			    // diagonal neighbors, i2,i3
			    mask(i1, i2+offset[1], i3+offset[2])==0 || 
			    mask(i1, i2-offset[1], i3-offset[2])==0 || 
			    mask(i1, i2-offset[1], i3+offset[2])==0 || 
			    mask(i1, i2+offset[1], i3-offset[2])==0  )
			  {
			    bdyVert++;
			  }
		      
		    } // i1, i2,i3
	    } // side

	  for ( aa=0; aa<cg.numberOfDimensions(); aa++ ) offset[aa] = 0;
		  
	} // axis
    } // grid

  int nBdyVertices = bdyVert;
  // redim boundaryHoleVertices know that the size is known
  if ( nBdyVertices>0) boundaryHoleVertices.redim(bdyVert, 6);
  bdyVert = 0;

  // now repeat the above loop, this time logging the boundary hole vertex information
  if ( nBdyVertices>0) 
  for ( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const MappedGrid &mappedGrid = cg[grid];
      const intArray &mask = mappedGrid.mask();

      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

      int offset[3];
      offset[0] = offset[1] = offset[2] = 0;

      int side, axis;
      for ( axis=0; axis<mappedGrid.domainDimension(); axis++ )
	{
	  for ( aa=0; aa<mappedGrid.domainDimension(); aa++ ) 
	    if ( aa!=axis ) offset[aa] = 1;

	  for ( side=0; side<2 && !(mappedGrid.isPeriodic(axis)); side++ ) 
	    {
	      getBoundaryIndex( mappedGrid.gridIndexRange(), side, axis, I1,I2,I3 );
	      
	      int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
	      for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      if ( mask(i1,i2,i3) != 0 && mappedGrid.boundaryFlag(side,axis)==MappedGrid::physicalBoundary)
			if (// log this vertex only if one of its neighbors is masked out	
			    // immediate neighbors  
			    mask(i1+offset[0], i2, i3)==0 || mask(i1-offset[0], i2, i3)==0 ||
			    mask(i1, i2+offset[1], i3)==0 || mask(i1, i2-offset[1], i3)==0 ||
			    mask(i1, i2, i3+offset[2])==0 || mask(i1, i2, i3-offset[2])==0 ||
			    
			    // diagonal neighbors, i1,i2
			    mask(i1+offset[0], i2+offset[1], i3)==0 || 
			    mask(i1-offset[0], i2-offset[1], i3)==0 || 
			    mask(i1+offset[0], i2-offset[1], i3)==0 || 
			    mask(i1-offset[0], i2+offset[1], i3)==0 || 
			    // diagonal neighbors, i1,i3		    
			    mask(i1+offset[0], i2, i3+offset[2])==0 || 
			    mask(i1-offset[0], i2, i3-offset[2])==0 || 
			    mask(i1+offset[0], i2, i3-offset[2])==0 || 
			    mask(i1-offset[0], i2, i3+offset[2])==0 || 
			    // diagonal neighbors, i2,i3
			    mask(i1, i2+offset[1], i3+offset[2])==0 || 
			    mask(i1, i2-offset[1], i3-offset[2])==0 || 
			    mask(i1, i2-offset[1], i3+offset[2])==0 || 
			    mask(i1, i2+offset[1], i3-offset[2])==0  )
			  {
			    // log vertex information
			    boundaryHoleVertices(bdyVert, 0) = grid;
			    boundaryHoleVertices(bdyVert, 1) = axis;
			    boundaryHoleVertices(bdyVert, 2) = side;
			    for ( int i=0; i<3; i++ )
			      boundaryHoleVertices(bdyVert, 3+i) = ii[i];
			    
			    bdyVert++;
			  }
		      
		    } // i1, i2,i3
	    } // side

	  for ( aa=0; aa<cg.numberOfDimensions(); aa++ ) offset[aa] = 0;
		  
	} // axis
    } // grid

}

static void sealShareHoles2D( CompositeGrid &cg,
			      intArray &boundaryHoleVertices,
			      intArray *gridIndexVertexMap, 
			      realArray &auxVertices, intArray &auxIDMap,
			      intArray &initialFaces, intArray &initialSurfaceMapping=Overture::nullIntegerDistributedArray() )
{
  // seal the front along shared boundaries

  Range AXES(0, cg.numberOfDimensions()-1);
  int GRID = 0;
  int AXIS = 1;
  int SIDE = 2;

  intArray newFacesWithExistingVertices(2*boundaryHoleVertices.getLength(0), 2);
  intArray newFaceSurface(2*boundaryHoleVertices.getLength(0));

  newFacesWithExistingVertices = -1;
  newFaceSurface = -1;

  // isSealed is a flag array that logs whether a boundary hole vertex has
  // been sealed or not.
  intArray isSealed(boundaryHoleVertices.getLength(0));
  isSealed = 0; // set to false

  // first join vertices on shared boundaries
  int vert;
  int newFace = 0;
  for ( vert=0; vert<(boundaryHoleVertices.getLength(0)-auxVertices.getLength(0))
	  && newFace<(2*boundaryHoleVertices.getLength(0)); vert++ )
    {
      int grid1 = boundaryHoleVertices(vert, GRID);
      int side1 = boundaryHoleVertices(vert, SIDE);
      int axis1 = boundaryHoleVertices(vert, AXIS);

      real dist = REAL_MAX;
      bool foundPair = false;
      int pair = -1;
      realArray vertex1;
      vertex1 = cg[grid1].vertex() ( boundaryHoleVertices(vert, 3),
				     boundaryHoleVertices(vert, 4),
				     boundaryHoleVertices(vert, 5), AXES );

      int v2;
      //      for ( v2=vert+1; v2<boundaryHoleVertices.getLength(0)+1; v2++ )
      for ( v2=vert+1; v2<boundaryHoleVertices.getLength(0) && isSealed(vert)==0 ; v2++ )
	{ // ok, this is N^2 on the number of boundary hole vertices,
	  // but usually this is very tiny in 2D

	  int vert2 = v2 ;//% boundaryHoleVertices.getLength(0);
	  int grid2 = boundaryHoleVertices(vert2, GRID);
	  int side2 = boundaryHoleVertices(vert2, SIDE);
	  int axis2 = boundaryHoleVertices(vert2, AXIS);

	  if ( grid2==-1 && auxVertices.getLength(0)>0 )
	    {
	      // check the auxiliary vertices (for example, from a surface-surface intersection)
	      realArray vertex2;
	      vertex2 = auxVertices(boundaryHoleVertices.getLength(0)-1-v2,AXES);
	      vertex2.reshape(1,1,1,AXES.getLength());
	      real dist_cand = sum(pow(vertex1-vertex2,2.0));
	      if ( dist_cand<dist )
		{
		  bool normsOK = true;
 		  if ( false && cg.numberOfDimensions()==3 ) // this is a surface, check the normals
 		    {
 		      MappingProjectionParameters mp;
 		      mp.getRealArray(MappingProjectionParameters::r).redim(1,2);
 		      mp.getRealArray(MappingProjectionParameters::r) = -1;
 		      mp.getRealArray(MappingProjectionParameters::normal).redim(1,3);
 		      vertex2.reshape(1,3);
 		      vertex1.reshape(1,3);
 		      cg[grid2].mapping().mapPointer->project(vertex2,mp);
 		      realArray & sn = mp.getRealArray(MappingProjectionParameters::normal);
		      
 		      mp.getRealArray(MappingProjectionParameters::r).redim(1,2);
 		      mp.getRealArray(MappingProjectionParameters::r) = -1;
 		      mp.getRealArray(MappingProjectionParameters::normal).redim(1,3);

 		      cg[grid1].mapping().mapPointer->project(vertex1,mp);
 		      realArray & norm = mp.getRealArray(MappingProjectionParameters::normal);
		      
 		      real magnorm = sqrt( norm(0,0)*norm(0,0)+norm(0,1)*norm(0,1)+norm(0,2)*norm(0,2) );
 		      real magsn = sqrt( sn(0,0)*sn(0,0)+sn(0,1)*sn(0,1)+sn(0,2)*sn(0,2) );
 		      normsOK = ((sn(0,0)*norm(0,0) + sn(0,1)*norm(0,1) + sn(0,2)*norm(0,2))/magnorm/magsn)>REAL_EPSILON;
 		      vertex1.reshape(1,1,1,3);
 		    }
		  if ( normsOK )
		    {
		      pair = vert2;
		      dist = dist_cand;
		      foundPair = true;
//               cout<<"NEW bdy PAIR IS "<<vert<<"  "<<vert2<<endl;
		    }
		}
	    }
	  else if ( isSealed(vert2)==0 && 
	       grid1!=grid2 &&
		    ( ( cg.numberOfDimensions()==2 && cg[grid1].sharedBoundaryFlag(side1,axis1)==cg[grid2].sharedBoundaryFlag(side2, axis2) )|| cg.numberOfDimensions()==3 ) )
	    {
	      // found a boundary hole vertex on a shared boundary
	      // should we make a new face ?
	      // just use the closest shared boundary vertex
	      // XXX ! this may cause problems on thin components
	      // with shared boundaries ( like the te of an airfoil )
	      realArray vertex2;
	      vertex2 = cg[grid2].vertex() ( boundaryHoleVertices(vert2, 3),
					     boundaryHoleVertices(vert2, 4),
					     boundaryHoleVertices(vert2, 5), AXES );
	      real disttol = 0;
	      for ( int aa=0; aa<cg[grid2].numberOfDimensions(); aa++ )
		{
		  disttol = max(disttol,.1*(cg[grid2].boundingBox(1,aa)-cg[grid2].boundingBox(0,aa)));
		}

	      disttol = disttol*disttol;
	      
	      real dist_cand = sum(pow(vertex1-vertex2,2.0));

	      if ( dist_cand < dist && dist_cand>100*REAL_MIN )//&& dist_cand<disttol )
		{

		  bool normsOK = true;
          for ( int v3=0; v3<boundaryHoleVertices.getLength(0) && normsOK; v3++ )
            {
	            int vert3 = v3 ;//% boundaryHoleVertices.getLength(0);
	            int grid3 = boundaryHoleVertices(vert3, GRID);
	            int side3 = boundaryHoleVertices(vert3, SIDE);
	            int axis3 = boundaryHoleVertices(vert3, AXIS);
                  
                if ( vert3!=vert && grid3==grid1) { 
	            realArray vertex3;
	            vertex3 = cg[grid3].vertex() ( boundaryHoleVertices(vert3, 3),
			     boundaryHoleVertices(vert3, 4),
			     boundaryHoleVertices(vert3, 5), AXES );
                normsOK = !( sum(pow(vertex2-vertex3,2))<100*REAL_MIN ) ;
                }
            } 

		  if ( cg.numberOfDimensions()==3 && normsOK ) // this is a surface, check the normals
		    {

		      MappingProjectionParameters mp;
		      mp.getRealArray(MappingProjectionParameters::r).redim(1,2);
		      mp.getRealArray(MappingProjectionParameters::r) = -1;
		      mp.getRealArray(MappingProjectionParameters::normal).redim(1,3);
		      vertex2.reshape(1,3);
		      vertex1.reshape(1,3);
		      cg[grid2].mapping().mapPointer->project(vertex2,mp);
		      realArray & sn = mp.getRealArray(MappingProjectionParameters::normal);
		      
              ArraySimpleFixed<real,3,1,1,1> edge;
              for ( int a=0; a<3; a++ ) edge[a] = vertex2(0,a)-vertex1(0,a);

		      cg[grid1].mapping().mapPointer->project(vertex1,mp);
		      realArray & norm = mp.getRealArray(MappingProjectionParameters::normal);
		      
		      real magnorm = sqrt( norm(0,0)*norm(0,0)+norm(0,1)*norm(0,1)+norm(0,2)*norm(0,2) );
		      real magsn = sqrt( sn(0,0)*sn(0,0)+sn(0,1)*sn(0,1)+sn(0,2)*sn(0,2) );
		      normsOK = ((sn(0,0)*norm(0,0) + sn(0,1)*norm(0,1) + sn(0,2)*norm(0,2))/magnorm/magsn)>0.5;
              // also make sure that edge and norm are not parallel
              normsOK = normsOK && 
                       (1.-fabs( norm(0,0)*edge[0] + norm(0,1)*edge[1]+ norm(0,2)*edge[2] )/magnorm/sqrt(ASmag2(edge)))>REAL_EPSILON;
		      vertex1.reshape(1,1,1,3);
		    }
		  if ( normsOK )
		    {
		      pair = vert2;
              // cout<<"NEW share PAIR IS "<<vert<<"  "<<vert2<<", "<<dist<<", "<<dist_cand<<endl;
		      dist = dist_cand;
		      foundPair = true;
		    }
		}
	      
	    } // if ( isSealed ... )
	} // v2

      if ( foundPair ) // make a new face
	{
	  // make sure the orientation is correct
	  // the face normal should point into the hole
	  realArray p1;
	  p1 = vertex1;
	  realArray p2;
	  if ( boundaryHoleVertices(pair,GRID)==-1 )
	    p2 = auxVertices(boundaryHoleVertices.getLength(0)-1-pair,AXES);
	  else
	    p2 = cg[boundaryHoleVertices(pair,GRID)].vertex() ( boundaryHoleVertices(pair, 3),
								boundaryHoleVertices(pair, 4),
								boundaryHoleVertices(pair, 5), AXES );

  
	  p1.reshape(AXES);
	  p2.reshape(AXES);

	  bool orientationCorrect = newFaceOrientation(cg, boundaryHoleVertices, vert, p1, p2);
	  
	  newFaceSurface(newFace) = grid1;
	  if ( orientationCorrect )
	    {
	      newFacesWithExistingVertices(newFace,0) = gridIndexVertexMap[grid1](boundaryHoleVertices(vert, 3),
										  boundaryHoleVertices(vert, 4),
										  boundaryHoleVertices(vert, 5));
	      if ( boundaryHoleVertices(pair,GRID)==-1 )
		{
		  newFacesWithExistingVertices(newFace,1) = auxIDMap(boundaryHoleVertices.getLength(0)-1-pair);
		 
// 		  newFace++;
// 		  newFacesWithExistingVertices(newFace,0) = newFacesWithExistingVertices(newFace-1,1);
// 		  newFacesWithExistingVertices(newFace,1) = newFacesWithExistingVertices(newFace-1,0);
		}
	      else
		newFacesWithExistingVertices(newFace,1) = 
		  gridIndexVertexMap[boundaryHoleVertices(pair,GRID)] ( boundaryHoleVertices(pair, 3),
								      boundaryHoleVertices(pair, 4),
								      boundaryHoleVertices(pair, 5) );
	    }
	  else 
	    {
	      newFacesWithExistingVertices(newFace,1) = gridIndexVertexMap[grid1](boundaryHoleVertices(vert, 3),
										  boundaryHoleVertices(vert, 4),
										  boundaryHoleVertices(vert, 5));

	      if ( boundaryHoleVertices(pair,GRID)==-1 )
		{
		  newFacesWithExistingVertices(newFace,0) = auxIDMap(boundaryHoleVertices.getLength(0)-1-pair);
// 		  newFace++;
// 		  newFacesWithExistingVertices(newFace,0) = newFacesWithExistingVertices(newFace-1,1);
// 		  newFacesWithExistingVertices(newFace,1) = newFacesWithExistingVertices(newFace-1,0);
		}
	      else
		newFacesWithExistingVertices(newFace,0) = 
		  gridIndexVertexMap[boundaryHoleVertices(pair,GRID)] ( boundaryHoleVertices(pair, 3),
									boundaryHoleVertices(pair, 4),
									boundaryHoleVertices(pair, 5) );
	    }

	  isSealed(vert) = 1;
	  isSealed(pair) = 1;

	  newFace++;

	}

    } // vert

  cout << "Boundary holes sealed with shared boundary vertices : "<<newFace<<endl;

  if ( newFace != 0 ) 
    {
      initialFaces.resize(initialFaces.getLength(0)+newFace, initialFaces.getLength(1));
      int oldLen =initialSurfaceMapping.getLength(0); 
      initialSurfaceMapping.resize(initialFaces.getLength(0));
      initialSurfaceMapping(Range(initialFaces.getLength(0)-newFace, initialFaces.getLength(0)-1)) = -1;
      int ff;
      Range FACEV(0,1);
      for ( ff=0; ff<newFace; ff++ )
	{
	  initialFaces(initialFaces.getLength(0)-newFace+ff, FACEV) = newFacesWithExistingVertices(ff, FACEV);
	}

      if ( &initialSurfaceMapping!=&Overture::nullIntegerDistributedArray() )
	for ( ff=0; ff<newFace; ff++ )
	  initialSurfaceMapping(initialFaces.getLength(0)-newFace+ff) = newFaceSurface(ff);

 //      newFaceSurface.display("new face surface");
//       initialSurfaceMapping.display("initial surface mapping");
      
    }
}

static void sealIntersectionHoles2D( CompositeGrid &cg,
				     intArray &boundaryHoleVertices, realArray &intersectionVertices, 
				     intArray &intersectionVertexData,
				     intArray *gridIndexVertexMap,
				     intArray &initialFaces, realArray &xyz_initial )
{

  // seal the front using intersections between the boundary mappings

  Range AXES(0, cg.numberOfDimensions()-1);
  int nIntersectionVertices = intersectionVertices.getLength(0);

  if ( intersectionVertices.getLength(0)>0 )
    {
      xyz_initial.resize(xyz_initial.getLength(0)+intersectionVertices.getLength(0), xyz_initial.getLength(1));
      for ( int v=0; v<nIntersectionVertices; v++ )
	xyz_initial( xyz_initial.getLength(0)-nIntersectionVertices+v, AXES ) = intersectionVertices(v,AXES);

      initialFaces.resize( initialFaces.getLength(0)+2*nIntersectionVertices, initialFaces.getLength(1) );
    } else {
      return ;
    }

  // for each intersectionVertex, find two boundaryHoleVertices that will complete the two faces
  // of the intersection

  int newFace = initialFaces.getLength(0)-nIntersectionVertices*2;
  for ( int intersect=intersectionVertices.getBase(0); intersect<=intersectionVertices.getBound(0); intersect++ )
    {

      int grid1 = intersectionVertexData(intersect, 0);
      int side1 = intersectionVertexData(intersect, 1);
      int axis1 = intersectionVertexData(intersect, 2);
      int grid2 = intersectionVertexData(intersect, 3);
      int side2 = intersectionVertexData(intersect, 4);
      int axis2 = intersectionVertexData(intersect, 5);

      int cand;
      real min1 = REAL_MAX;
      int min1Vert = -1;
      real min2 = REAL_MAX;
      int min2Vert = -1;
      for ( cand=0; cand<boundaryHoleVertices.getLength(0); cand++ )
	{
	  if ( grid1==boundaryHoleVertices(cand, 0) &&
	       axis1==boundaryHoleVertices(cand, 1) &&
	       side1==boundaryHoleVertices(cand, 2) )
	    {
	      int xyzInd = gridIndexVertexMap[boundaryHoleVertices(cand,0)] ( boundaryHoleVertices(cand, 3),
									      boundaryHoleVertices(cand, 4),
									      boundaryHoleVertices(cand, 5) );
	      real dist = sqrt(sum(pow(xyz_initial(xyzInd, AXES) - intersectionVertices(intersect,AXES), 2)));

	      if ( dist<min1 )
		{
		  min1 = dist;
		  min1Vert = cand;
		}
	    } else if ( grid2==boundaryHoleVertices(cand, 0) &&
			axis2==boundaryHoleVertices(cand, 1) &&
			side2==boundaryHoleVertices(cand, 2) ) {
	      
	      int xyzInd = gridIndexVertexMap[boundaryHoleVertices(cand,0)] ( boundaryHoleVertices(cand, 3),
									       boundaryHoleVertices(cand, 4),
									       boundaryHoleVertices(cand, 5) );
	      real dist = sqrt(sum(pow(xyz_initial(xyzInd, AXES) - intersectionVertices(intersect,AXES), 2)));

	      if ( dist<min2 )
		{
		  min2 = dist;
		  min2Vert = cand;
		}
	    }

	} // cand

      AssertException( min1Vert!=-1 && min2Vert!=-1, PreProcessingError() );

      realArray p1,p2;
      p1 = cg[boundaryHoleVertices(min1Vert,0)].vertex() ( boundaryHoleVertices(min1Vert, 3),
							   boundaryHoleVertices(min1Vert, 4),
							   boundaryHoleVertices(min1Vert, 5), AXES );

      p2 = intersectionVertices(intersect, AXES);
      p1.reshape(AXES);
      p2.reshape(AXES);

      if ( newFaceOrientation(cg, boundaryHoleVertices, min1Vert, p1, p2) )
      	{
	  initialFaces(newFace, 0) = 
	    gridIndexVertexMap[boundaryHoleVertices(min1Vert,0)] ( boundaryHoleVertices(min1Vert, 3),
								   boundaryHoleVertices(min1Vert, 4),
								   boundaryHoleVertices(min1Vert, 5) );
	  initialFaces(newFace, 1) = xyz_initial.getLength(0)-nIntersectionVertices+intersect;
	} else {
	  initialFaces(newFace, 1) = 
	    gridIndexVertexMap[boundaryHoleVertices(min1Vert,0)] ( boundaryHoleVertices(min1Vert, 3),
								   boundaryHoleVertices(min1Vert, 4),
								   boundaryHoleVertices(min1Vert, 5) );
	  initialFaces(newFace, 0) = xyz_initial.getLength(0)-nIntersectionVertices+intersect;
	}
      newFace++;

      p1.reshape(1,1,1,AXES);
      p1 = cg[boundaryHoleVertices(min2Vert,0)].vertex() ( boundaryHoleVertices(min2Vert, 3),
							   boundaryHoleVertices(min2Vert, 4),
							   boundaryHoleVertices(min2Vert, 5), AXES );

      p1.reshape(AXES);
      if ( newFaceOrientation(cg, boundaryHoleVertices, min2Vert, p1, p2) )
      	{
	  initialFaces(newFace, 0) = 
	    gridIndexVertexMap[boundaryHoleVertices(min2Vert,0)] ( boundaryHoleVertices(min2Vert, 3),
								   boundaryHoleVertices(min2Vert, 4),
								   boundaryHoleVertices(min2Vert, 5) );
	  initialFaces(newFace, 1) = xyz_initial.getLength(0)-nIntersectionVertices+intersect;
	} else {
	  initialFaces(newFace, 1) = 
	    gridIndexVertexMap[boundaryHoleVertices(min2Vert,0)] ( boundaryHoleVertices(min2Vert, 3),
								   boundaryHoleVertices(min2Vert, 4),
								   boundaryHoleVertices(min2Vert, 5) );
	  initialFaces(newFace, 0) = xyz_initial.getLength(0)-nIntersectionVertices+intersect;
	}

      newFace++;
            
    } // intersect

}

static void sealHoles2D( CompositeGrid &cg,
			 intArray &boundaryHoleVertices, realArray &intersectionVertices, 
			 intArray &intersectionVertexData,
			 intArray *gridIndexVertexMap,
			 intArray &initialFaces, realArray &xyz_initial )
{

  // the following two are not needed in 2D geometries, pass them as empty arrays
  realArray auxVerts;
  intArray auxIDMap;

  sealShareHoles2D( cg,
		    boundaryHoleVertices,
		    gridIndexVertexMap,
		    auxVerts, auxIDMap,
		    initialFaces );

  sealIntersectionHoles2D( cg, boundaryHoleVertices,
			   intersectionVertices, intersectionVertexData, 
			   gridIndexVertexMap,
			   initialFaces, xyz_initial );

}


static void mergeDuplicateVertices(const realArray & xyz, intArray &initialFaces)
{

  intArray vertCount(xyz.getLength(0));
  vertCount = 0;

  for ( int f=0; f<initialFaces.getLength(0); f++ )
    {
      for ( int v=0; v<2; v++ )
	if ( initialFaces(f,v)!=-1 )
	  vertCount(initialFaces(f,v))++;
    }

  //  initialFaces.display();
  int nHanging = sum(vertCount==1);
  //  vertCount.display();
  cout<<"sealHoles : there were "<<nHanging<<" hanging vertices in the surface hole "<<endl;

  if ( nHanging )
    {
      intArray hangingVert(nHanging);
      intArray new_hangingVert(nHanging);
      
      int hv=0;
      for ( int v=0; v<xyz.getLength(0); v++ )
	{
	  if ( vertCount(v)==1 )
	    hangingVert(hv++) = v;
	}

      new_hangingVert = hangingVert;

      Range AXES(xyz.getLength(1));

      real eps = 100*REAL_MIN;

      for ( hv=0; hv<nHanging; hv++ )
	{

	  if ( vertCount(hv)==1 )
	    {
	      real minD = REAL_MAX;
	      // *wdh* 030909 real minV = -1;
              int minV = -1;
	      
	      for ( int hv1=hv+1; hv1<nHanging; hv1++ )
		{
		  real dist = sum(pow(xyz(hangingVert(hv),AXES)-xyz(hangingVert(hv1),AXES),2));

		  if ( dist<minD && dist<eps )
		    {
		      minD = dist;
		      minV = hv1;
		    }
		}
	      
	      if ( minV>-1 )
		{
		  vertCount(hv)++;
		  vertCount(minV)++;
		  
		  new_hangingVert(minV) = new_hangingVert(hv);
		}
	    }
	}

      for ( int f=0; f<initialFaces.getLength(0); f++ )
	{
	  for ( int v=0; v<2; v++ )
	    for ( int hv=0; hv<nHanging; hv++ )
	      {
		if ( initialFaces(f,v)==hangingVert(hv) )
		  {
		    initialFaces(f,v)=new_hangingVert(hv);
		    break;
		  }
	      }
	}
    }
}

static void sealSurfaceHoles ( CompositeGrid &cg,
			       intArray &boundaryHoleVertices,
			       intArray *gridIndexVertexMap,
			       intArray &initialFaces, realArray &xyz_initial, 
			       intArray & initialSurfaceMapping, AdvancingFront *advF=NULL )
// ===================================================================================================
// /Description:
//     Find any curves of intersection between surface grids.
// ===================================================================================================
{

  intArray newFaces;
  realArray newVertices;
  intArray newSurfaceMapping;
  int oldNVerts = 0;
  int oldNFaces = 0;
  bool reparameterized;

  MappingProjectionParameters mp;

  Range AXES(3);

  bool useSpacing = ! (advF==NULL);

  realArray auxVertices;
  intArray auxIDMap;
  IntersectionMapping intersection;
  ArraySimpleFixed<real,3,1,1,1> sn1,sn2,ctan,scross;
  realArray xProject(1,3), rProject(1,1), rProjectCurve(1,1);
  
  int rangeDimension=cg.numberOfDimensions();
  for ( int grid=0; grid<cg.numberOfGrids()-1; grid++ )
    {

      Mapping *map1 = cg[grid].mapping().mapPointer;
      int share1=cg[grid].sharedBoundaryFlag(0,rangeDimension-1);
      
      for ( int grid2=grid+1; grid2<cg.numberOfGrids(); grid2++ )
	{
	  Mapping *map2 = cg[grid2].mapping().mapPointer;

          // wdh: On a surface grid, the extra axis in the normal direction holds a share flag for the whole surface:
	  int share2=cg[grid2].sharedBoundaryFlag(0,rangeDimension-1);
	  
          // printF("sealSurfaceHoles: grid=%i share=%i, grid2=%i share=%i\n",grid,share1,grid2,share2);
	  if( share1>0 && share1==share2 )
	  {
	    printF(" sealHoles: surface grids grid=%i and grid2=%i are on a shared surface, share=%i. "
                   " Do not check for an intersection\n",grid,grid2,share1);
	    continue;
	  }

	  if ( map1->intersects(*map2,-1,-1,-1,-1,-100*FLT_EPSILON) )
	    {
	      if ( intersection.intersect(*map1, *map2)==0 )
		{
		  cout<<"intersection found between grids "<<grid<<" and "<<grid2<<endl;

		  //		  intersection.setGridDimensions(0,200);
		  //		  PlotIt::plot(*Overture::getGraphicsInterface(),intersection);
	      
		  //(Overture::getGraphicsInterface())->plot(intersection,Overture::defaultGraphicsParameters());
		  //		  intersection.setGridDimensions(0,20);

		  // check the orientation of the curve relative to the surfaces
		  rProject = 0.;
		  intersection.map(rProject,xProject);

		  rProject.resize(1,2);
		  rProject = -1;

		  mp.getRealArray(MappingProjectionParameters::x).redim(0);
		  mp.getRealArray(MappingProjectionParameters::r).redim(0);
		  mp.getRealArray(MappingProjectionParameters::xr).redim(0);
		  mp.getRealArray(MappingProjectionParameters::normal).redim(0);

		  mp.getRealArray(MappingProjectionParameters::r) = rProject;
		  mp.getRealArray(MappingProjectionParameters::x) = xProject;
		  mp.getRealArray(MappingProjectionParameters::normal).redim(1,3);
		  mp.getIntArray(MappingProjectionParameters::subSurfaceIndex).redim(1);
		  mp.getIntArray(MappingProjectionParameters::subSurfaceIndex) = -1;

		  map1->project(xProject,mp);
		  real sj;
		  int a;

		  if ( map1->getClassName()=="CompositeSurface" )
		    {
		      int s = mp.getIntArray(MappingProjectionParameters::subSurfaceIndex)(0);
		      //sj = ((CompositeSurface *)map1)->getSignForJacobian();
		      sj  = ((CompositeSurface *)map1)->getSignForNormal(s);
		    }
		  else
		    sj = map1->getSignForJacobian();

		  //sj = 1;

		  for ( a=0; a<3; a++ )
		    sn1[a] = sj*mp.getRealArray(MappingProjectionParameters::normal)(0,a);

		  
		  map2->project(xProject,mp);
		  if ( map2->getClassName()=="CompositeSurface" )
		    {
		      int s = mp.getIntArray(MappingProjectionParameters::subSurfaceIndex)(0);
		      //sj = ((CompositeSurface *)map2)->getSignForJacobian();
		      sj  = ((CompositeSurface *)map2)->getSignForNormal(s);
		    }
		  else
		    sj = map2->getSignForJacobian();

		  //sj = 1;

		  for ( a=0; a<3; a++ )
		    sn2[a] = sj*mp.getRealArray(MappingProjectionParameters::normal)(0,a);

		  mp.getRealArray(MappingProjectionParameters::xr).redim(1,3,1);

		  intersection.project(xProject,mp);
		  for ( a=0; a<3; a++ )
		    ctan[a] = mp.getRealArray(MappingProjectionParameters::xr)(0,a,0);
		 
		  // sn1 x sn2
		  scross[0] = sn1[1]*sn2[2]-sn1[2]*sn2[1];
		  scross[1] = -(sn1[0]*sn2[2]-sn1[2]*sn2[0]);
		  scross[2] = sn1[0]*sn2[1]-sn1[1]*sn2[0];

		  reparameterized = false;
		  if ( ASdot(ctan, scross) < 0. )
		    {
		      reparameterized = true;
		      //intersection.reparameterize(1.,0.);
		      cout<<"    intersection curve reparameterized! "<<endl;
		    }

 		  bool surfPar = false;
 		  if ( fabs(ASdot(ctan,scross))<FLT_EPSILON ) 
 		    {
 		      cout<<" scross "<<scross<<endl;
 		      surfPar = true;
		    }

		  realArray verts;
		  // now adjust the spacing given the background mesh control (otherwise just guess 20 pts)
		  if ( useSpacing )
		    intersection.setGridDimensions(0,5);

		  verts = intersection.getGrid();

		  intersection.setGridDimensions(0,200); // reset for more accurate approximate inverse

		  bool someSplit = useSpacing;
		  int nv = verts.getLength(0);

		  ArraySimple<real> T(3,3);
		  ArraySimple<real> p1(3),p2(3),p1t(3),p2t(3),e(3);
		  realArray pc(3);
		  intArray split(20);

		  mp.getRealArray(MappingProjectionParameters::x).redim(0);
		  mp.getRealArray(MappingProjectionParameters::r).redim(0);
		  mp.getRealArray(MappingProjectionParameters::xr).redim(0);
		  mp.getRealArray(MappingProjectionParameters::normal).redim(0);

		  real normArcLength = 0;
		  while ( someSplit && !surfPar ) // this will only happen if useSpacing==true
		    {
		      int nvold = nv;
		  
		      int a,nsplit;
		      someSplit = false;
		      nsplit = 0;
		      split = 0;
		      normArcLength = 0;
		      for ( int v=0; v<nvold-1; v++ )
			{
			  for ( a=0; a<3; a++ )
			    {
			      p1[a] = verts(v,0,0,a);
			      p2[a] = verts(v+1,0,0,a);
			      pc(a) = 0.5*(p1[a]+p2[a]);
			    }

		      
			  advF->computeTransformationAtPoint(pc,T);

			  for ( int r=0; r<3; r++ )
			    {
			      p1t[r] = p2t[r] = 0.0;
			      for ( int r1=0; r1<3; r1++ )
				{
				  p1t[r] += T[r1*3+r]*p1(r1);
				  p2t[r] += T[r1*3+r]*p2(r1);
				}
			  
			      e[r] = p2t[r]-p1t[r];
			    }

			  normArcLength += sqrt(ASmag2(e));
			  if ( ASmag2(e)>(2.25) ) 
			    {
			      split(v) = 1;
			      someSplit = true;
			      nsplit++;
			    }
			}

		      if ( someSplit && normArcLength>1. )
			{
			  nv = nvold + nsplit;
			  realArray splitVerts(nsplit,3);
			  int vv=0;
			  int v;
			  for ( v=0; v<nvold-1; v++ )
			    if ( split(v)==1 )
			      {
				for ( a=0;a<3;a++ )
				  splitVerts(vv,a) = 0.5*(verts(v,0,0,a) + verts(v+1,0,0,a));
				vv++;
			      }
		      
			  mp.getRealArray(MappingProjectionParameters::r).resize(nsplit,1);
			  mp.getRealArray(MappingProjectionParameters::r) = -1;
			  mp.getRealArray(MappingProjectionParameters::x).redim(0);
			  mp.getRealArray(MappingProjectionParameters::x) = splitVerts;
			  
			  intersection.project(splitVerts,mp);
		      
			  verts.resize(nv,1,1,3);
			  realArray oldVerts;
			  oldVerts = verts;
			  int sv;
			  vv = sv = 0;
			  for ( v=0; v<nvold-1; v++ )
			    if ( split(v)==1 )
			      {
				for ( a=0; a<3; a++ )
				  verts(vv+1,0,0,a) = splitVerts(sv,a);

				vv++; sv++;

				for ( a=0; a<3; a++ )
				  verts(vv+1,0,0,a) = oldVerts(v+1,0,0,a);
				vv++;
			      }
			    else
			      {
				for ( a=0; a<3; a++ )
				  verts(vv+1,0,0,a) = oldVerts(v+1,0,0,a);
				vv++;
			      }
		      
			  if ( nv>split.getLength(0) ) split.resize(nv+10);

			}

		    }


		  // now stick the vertices to add into newVertices and create the new faces
		  if ( normArcLength>1. && !surfPar ) // try to ignore bogus intersections due to roundoff
		    {
		      oldNVerts = newVertices.getLength(0);
		      oldNFaces = newFaces.getLength(0);
		      
		      newVertices.resize(oldNVerts+verts.getLength(0),1,1,3);
		      newFaces.resize(oldNFaces+2*(verts.getLength(0)-1),2);
		      newSurfaceMapping.resize(oldNFaces+2*(verts.getLength(0)-1));
		      
		      for ( int v=0; v<verts.getLength(0); v++ )
			newVertices(oldNVerts+v,0,0,AXES) = verts(v,0,0,AXES);
		      
		      int oldNAux = auxVertices.getLength(0);
		      auxVertices.resize(oldNAux + 2,3);
		      auxIDMap.resize(oldNAux+2);
		      int nbdv = boundaryHoleVertices.getLength(0);
		      boundaryHoleVertices.resize(nbdv+2,6);
		      boundaryHoleVertices ( nbdv, 0 ) = -1;
		      boundaryHoleVertices ( nbdv, 1 ) = -1;
		      boundaryHoleVertices ( nbdv, 2 ) = -1;
		      boundaryHoleVertices ( nbdv, 3 ) = -1;
		      boundaryHoleVertices ( nbdv, 4 ) = -1;
		      boundaryHoleVertices ( nbdv, 5 ) = -1;
		      boundaryHoleVertices ( nbdv+1, 0 ) =-1;
		      boundaryHoleVertices ( nbdv+1, 1 ) = -1;
		      boundaryHoleVertices ( nbdv+1, 2 ) = -1;
		      boundaryHoleVertices ( nbdv+1, 3 ) = -1;
		      boundaryHoleVertices ( nbdv+1, 4 ) = -1;
		      boundaryHoleVertices ( nbdv+1, 5 ) = -1;
		      
		      for ( a=0; a<3; a++ )
			{
			  auxVertices(oldNAux, a) = verts(0,0,0,a);
			  auxVertices(oldNAux+1,a) = verts(verts.getLength(0)-1,0,0,a);
			}
		      
		      auxIDMap(oldNAux) = oldNVerts;
		      auxIDMap(oldNAux+1) = oldNVerts+verts.getLength(0)-1;
		      
		      for ( int f=0; f<verts.getLength(0)-1; f++ )
			{
			  newFaces(oldNFaces+2*f,1) = newFaces(oldNFaces + 2*f+1,0) = oldNVerts+f;
			  newFaces(oldNFaces+2*f,0) = newFaces(oldNFaces + 2*f+1,1) = oldNVerts+f+1;
			  
			  //newFaces(oldNFaces+2*f,0) = newFaces(oldNFaces + 2*f+1,0) = oldNVerts+f;
			  //newFaces(oldNFaces+2*f,1) = newFaces(oldNFaces + 2*f+1,1) = oldNVerts+f+1;
			  
			  if ( reparameterized )
			    {
			      newSurfaceMapping(oldNFaces+2*f) = grid;
			      newSurfaceMapping(oldNFaces+2*f+1) = grid2;
			    }
			  else
			    {
			      newSurfaceMapping(oldNFaces+2*f) = grid2;
			      newSurfaceMapping(oldNFaces+2*f+1) = grid;
			    }
			  
			}
		      int f=verts.getLength(0)-2;
		      if ( intersection.getIsPeriodic(0) )
			{
			  cout<<"the intersection was periodic"<<endl;
			  newFaces(oldNFaces+2*f,0) = newFaces(oldNFaces + 2*f+1,1) = oldNVerts;
			  newVertices.resize(newVertices.getLength(0)-1,1,1,newVertices.getLength(3));
			}

		      cout<<"SEALHOLES : added "<<verts.getLength(0)*2<<" new faces due to intersections"<<endl;
		    }
		}
	      else
		cout<<"no intersection between grids "<<grid<<" and "<<grid2<<endl;
	  
	    }
	}

    }

  //  newSurfaceMapping.display("new Surface Mapping");

  if ( newFaces.getLength(0) > 0 ) 
    {
      int newFace = newFaces.getLength(0);
      int newVert = newVertices.getLength(0);
      int nOldVert = xyz_initial.getLength(0);

      newVertices.reshape(newVertices.getLength(0), newVertices.getLength(3));

      initialFaces.resize(initialFaces.getLength(0)+newFace, initialFaces.getLength(1));
      initialSurfaceMapping.resize(initialSurfaceMapping.getLength(0)+newFace);

      int ff;
      Range FACEV(0,1);
      for ( ff=0; ff<newFace; ff++ )
      	{
	  initialFaces(initialFaces.getLength(0)-newFace+ff, FACEV) = newFaces(ff, FACEV) + nOldVert;
	  initialSurfaceMapping(initialSurfaceMapping.getLength(0)-newFace+ff) = newSurfaceMapping(ff);
	}
      auxIDMap += nOldVert;

      xyz_initial.resize(xyz_initial.getLength(0)+newVert, xyz_initial.getLength(1));
      for ( int vv=0; vv<newVert; vv++ )
	xyz_initial(nOldVert+vv,AXES) = newVertices(vv,AXES);

    }


  //  initialSurfaceMapping.display("initial surface mapping 1");

  sealShareHoles2D( cg,
		    boundaryHoleVertices,
		    gridIndexVertexMap, auxVertices, auxIDMap,
		    initialFaces, initialSurfaceMapping );

  mergeDuplicateVertices(xyz_initial, initialFaces);
  //  initialSurfaceMapping.display("initialSurfaceMapping");

  
}

#if 0
void
Ugen::
sealHoles3D( CompositeGrid &cg, intArray *gridIndexVertexMap, intArray &boundaryHoleVertices, 
	     intArray &initialFaces, realArray &xyz_initial )
{
  int grid;
  int maxShare = 0;
  int side,axis;

  ArraySimple<bool> shareSidesUsed(cg.numberOfGrids(), cg.numberOfDimensions(), 2);
  ArraySimple<bool> bcSidesUsed(cg.numberOfGrids(), cg.numberOfDimensions(), 2);

  intArray * indexMappings = new intArray[ 2*cg.numberOfGrids() ];

  ArraySimple<int> gridID(cg.numberOfGrids());

  for ( int i=0; i<cg.numberOfGrids(); i++ )
    for ( int a=0; a<cg.numberOfDimensions(); a++ )
      {
	shareSidesUsed(i,a,0) = shareSidesUsed(i,a,1) = false;
	bcSidesUsed(i,a,0) = bcSidesUsed(i,a,0) = false;
      }

  int currshare, currbc, gid;
  for ( grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];

      //      CompositeGrid * cgsurfp = new CompositeGrid;
      //      CompositeGrid & cgsurf = *cgsurfp;
      intArray & givMap1 = gridIndexVertexMap[grid];

      for ( int a1=0; a1<mg.domainDimension(); a1++ )
	for ( int s1=0; s1<2; s1++ )
	  {
	    gid=0;
	    currshare = -1;
	    if ( mg.sharedBoundaryFlag(s1,a1)>0 )
	      currshare = mg.sharedBoundaryFlag(s1,a1);

	    currbc = -1;
	    if ( mg.boundaryFlag(s1,a1)==MappedGrid::physicalBoundary )
	      currbc =  mg.boundaryFlag(s1,a1);

	    CompositeGrid cgsurf;

	    if ( currshare!=-1 )
	      {
		for ( int grid2=grid+1;  grid2<cg.numberOfGrids(); grid2++ )
		  {
		    MappedGrid &mg2 = cg[grid2];
		    intArray & givMap2 = gridIndexVertexMap[grid2];
		    
		    for ( int a2=0; a2<mg2.domainDimension(); a2++ )
		      for ( int s2=0; s2<2; s2++ )
			{
			  if ( mg2.sharedBoundaryFlag(s2,a2)==currshare )
			    {
			      // if the sides share, create new grids from ReductionMappings and
			      // add to the composite grid for the surface
			      if ( !shareSidesUsed(grid, a1,s1) )
				{
				  mg.mapping().incrementReferenceCount();
				  ReductionMapping *redMap = 
				    new ReductionMapping(*(mg.mapping().mapPointer), a1, real(s1));
				  redMap->incrementReferenceCount();
				  real sj = mg.mapping().mapPointer->getSignForJacobian();;
				  if ( a1==1 ) sj *= -1;
				  if ( s1==1 ) sj *= -1;
				  
				  redMap->setSignForJacobian(sj);
				  
				  cgsurf.add(*redMap);
				  cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | MappedGrid::THEboundingBox);
				  //			      cout<<"reduction mapping g,s,a, sign for jac, sj  "<<grid<<" "<<s1<<" "<<a1<<" "<<mg.mapping().mapPointer->getSignForJacobian()<<"  "<<sj<<endl;
				  shareSidesUsed(grid,a1,s1) = true;
				  intArray &mask = cgsurf[cgsurf.numberOfGrids()-1].mask();
				  intArray &mgmask = mg.mask();
				  Index Ib1, Ib2, Ib3;
				  int ie[3]; ie[0] = ie[1] = ie[2] = 1;
				  ie[a1] = 0;
				  getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
				  mask = -1;
				  if ( a1==axis1 )
				    {
				      indexMappings[gid].redim(Ib2,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(Ib1.getBase(),i,j);
					    if ( i>Ib2.getBase() && i< Ib2.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap1(Ib1.getBase(),i,j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else if ( a1==axis2 )
				    {
				      indexMappings[gid].redim(Ib1,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(i,Ib2.getBase(),j);
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap1(i,Ib2.getBase(),j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else
				    {
				      indexMappings[gid].redim(Ib1,Ib2);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(i,j,Ib3.getBase());
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib2.getBase() && j< Ib2.getBound() )
					      im(i,j) = givMap1(i,j,Ib3.getBase());
					    else
					      im(i,j) = -1;
					  }
				    }
				  
				  if ( (redMap->decrementReferenceCount()) == 0 ) delete redMap; 
				  gid++;
				  
				}

			      if ( !shareSidesUsed(grid2, a2,s2) )
				{
				  mg2.mapping().incrementReferenceCount();
				  ReductionMapping *redMap = 
				    new ReductionMapping(*(mg2.mapping().mapPointer), a2,real(s2));
				  
				  redMap->incrementReferenceCount();
				  real sj = mg2.mapping().mapPointer->getSignForJacobian();
				  if ( a2==1 ) sj *= -1;
				  if ( s2==1 ) sj *= -1;
				  
				  redMap->setSignForJacobian(sj);
				  
				  cgsurf.add(*redMap);
				  cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | 
									  MappedGridData::THEboundingBox);
				  //			      cout<<"reduction mapping g, s,a, sign for jac, sj "<<grid2<<" "<<s2<<" "<<a2<<" "<<mg2.mapping().mapPointer->getSignForJacobian()<<" "<<sj<<endl;
				  shareSidesUsed(grid2,a2,s2) = true;
				  intArray &mask = cgsurf[cgsurf.numberOfGrids()-1].mask();
				  intArray &mg2mask = mg2.mask();
				  Index Ib1, Ib2, Ib3;
				  int ie[3]; ie[0] = ie[1] = ie[2] = 1;
				  ie[a2] = 0;
				  getBoundaryIndex(mg2.gridIndexRange(),s2,a2,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
				  mask = -1;
				  if ( a2==axis1 )
				    {
				      indexMappings[gid].redim(Ib2,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mg2mask(Ib1.getBase(),i,j);
					    if ( i>Ib2.getBase() && i< Ib2.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap2(Ib1.getBase(),i,j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else if ( a2==axis2 )
				    {
				      indexMappings[gid].redim(Ib1,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mg2mask(i,Ib2.getBase(),j);
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap2(i,Ib2.getBase(),j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else
				    {
				      indexMappings[gid].redim(Ib1,Ib2);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
					  {
					    mask(i,j,0) = mg2mask(i,j,Ib3.getBase());
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib2.getBase() && j< Ib2.getBound() )
					      im(i,j) = givMap2(i,j,Ib3.getBase());
					    else
					      im(i,j) = -1;
					  }
				    }
				  
				  if ( (redMap->decrementReferenceCount()) == 0 ) delete redMap; 
				  gid++;
				}
			    }
			}
		  }
	      }
	    else if (currbc==MappedGrid::physicalBoundary)
	      {
		// check intersection of mapping with other mappings
		for ( int grid2=grid+1;  grid2<cg.numberOfGrids(); grid2++ )
		  {
		    MappedGrid &mg2 = cg[grid2];
		    intArray & givMap2 = gridIndexVertexMap[grid2];
		    
		    IntersectionMapping intersect;

		    for ( int a2=0; a2<mg2.domainDimension(); a2++ )
		      for ( int s2=0; s2<2; s2++ )
			{
			  if ( !bcSidesUsed(grid,a1,s1) && !bcSidesUsed(grid,a2,s2 ) )
			    {
			      mg.mapping().incrementReferenceCount();
			      ReductionMapping *redMap = 
				new ReductionMapping(*(mg.mapping().mapPointer), a1, real(s1));
			      redMap->incrementReferenceCount();
			      real sj = mg.mapping().mapPointer->getSignForJacobian();;
			      if ( a1==1 ) sj *= -1;
			      if ( s1==1 ) sj *= -1;
			      
			      redMap->setSignForJacobian(sj);
				
			      mg2.mapping().incrementReferenceCount();
			      ReductionMapping *redMap2 = 
				new ReductionMapping(*(mg2.mapping().mapPointer), a2,real(s2));
			      
			      redMap2->incrementReferenceCount();
			      sj = mg2.mapping().mapPointer->getSignForJacobian();
			      if ( a2==1 ) sj *= -1;
			      if ( s2==1 ) sj *= -1;
			      
			      redMap->setSignForJacobian(sj);
			      
			      if (intersect.intersect(*redMap, *redMap)==0)
				{
				  cgsurf.add(*redMap);
				  cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | MappedGrid::THEboundingBox);
				  
				  bcSidesUsed(grid,a1,s1) = true;
				  intArray &mask = cgsurf[cgsurf.numberOfGrids()-1].mask();
				  intArray &mgmask = mg.mask();
				  Index Ib1, Ib2, Ib3;
				  int ie[3]; ie[0] = ie[1] = ie[2] = 1;
				  ie[a1] = 0;
				  getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
				  mask = -1;
				  if ( a1==axis1 )
				    {
				      indexMappings[gid].redim(Ib2,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(Ib1.getBase(),i,j);
					    if ( i>Ib2.getBase() && i< Ib2.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap1(Ib1.getBase(),i,j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else if ( a1==axis2 )
				    {
				      indexMappings[gid].redim(Ib1,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(i,Ib2.getBase(),j);
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap1(i,Ib2.getBase(),j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else
				    {
				      indexMappings[gid].redim(Ib1,Ib2);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
					  {
					    mask(i,j,0) = mgmask(i,j,Ib3.getBase());
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib2.getBase() && j< Ib2.getBound() )
					      im(i,j) = givMap1(i,j,Ib3.getBase());
					    else
					      im(i,j) = -1;
					  }
				    }

				  gid++;
				  
				  cgsurf.add(*redMap2);
				  cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | 
									  MappedGridData::THEboundingBox);
				  bcSidesUsed(grid2,a2,s2) = true;
				  intArray &mask2 = cgsurf[cgsurf.numberOfGrids()-1].mask();
				  intArray &mg2mask = mg2.mask();
				  ie[0] = ie[1] = ie[2] = 1;
				  ie[a2] = 0;
				  getBoundaryIndex(mg2.gridIndexRange(),s2,a2,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
				  mask2 = -1;
				  if ( a2==axis1 )
				    {
				      indexMappings[gid].redim(Ib2,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask2(i,j,0) = mg2mask(Ib1.getBase(),i,j);
					    if ( i>Ib2.getBase() && i< Ib2.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap2(Ib1.getBase(),i,j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else if ( a2==axis2 )
				    {
				      indexMappings[gid].redim(Ib1,Ib3);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
					  {
					    mask2(i,j,0) = mg2mask(i,Ib2.getBase(),j);
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib3.getBase() && j< Ib3.getBound() )
					      im(i,j) = givMap2(i,Ib2.getBase(),j);
					    else
					      im(i,j) = -1;
					  }
				    }
				  else
				    {
				      indexMappings[gid].redim(Ib1,Ib2);
				      intArray &im = indexMappings[gid];
				      for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
					for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
					  {
					    mask2(i,j,0) = mg2mask(i,j,Ib3.getBase());
					    if ( i>Ib1.getBase() && i< Ib1.getBound() &&
						 j>Ib2.getBase() && j< Ib2.getBound() )
					      im(i,j) = givMap2(i,j,Ib3.getBase());
					    else
					      im(i,j) = -1;
					  }
				    }
				  
				  if ( redMap->decrementReferenceCount()==0 ) delete redMap;
				  if ( redMap2->decrementReferenceCount()==0 ) delete redMap2;
				  
				  gid++;
				}
			    }
			}
		  }
	      }
	    

	    if ( cgsurf.numberOfGrids()>0 )
	      {
		// construct the hybrid connectivity for the surface and then add the new
		//  triangles to the initial face list.

		//(Overture::getGraphicsInterface())->plot(cgsurf,Overture::defaultGraphicsParameters());
		updateHybrid(cgsurf);
 		const CompositeGridHybridConnectivity & connect = cgsurf.getHybridConnectivity();
		const intArray & uVertex2Grid = connect.getUVertex2GridIndex();
		int ugrid = connect.getUnstructuredGridIndex();
		if ( ugrid!=-1 )
		  {
		    UnstructuredMapping & umap = (UnstructuredMapping &) (*cgsurf[ugrid].mapping().mapPointer) ;
		    const realArray &umapXYZ = umap.getNodes();
		    const intArray &newFaces = umap.getEntities(UnstructuredMapping::Face);//umap.getElements();
		    
		    int nOldVertices = uVertex2Grid.getLength(0);
		    
		    int nInitialVertices = xyz_initial.getLength(0);
		    int nInitialFaces   = initialFaces.getLength(0);
		    int newNumberOfVertices = nInitialVertices + umap.getNumberOfNodes()-nOldVertices;
		    int newNumberOfFaces    = umap.getNumberOfElements() + initialFaces.getLength(0);
		    
		    xyz_initial.resize(newNumberOfVertices, 3);
		    initialFaces.resize(newNumberOfFaces, initialFaces.getLength(1));
		    
		    for ( int p=0; p<(umap.getNumberOfNodes()-nOldVertices); p++ )
		      for ( int a=0; a<3; a++ )
			xyz_initial(p+nInitialVertices,a) = umapXYZ(nOldVertices+p,a);
		    
		    for ( int ff=0; ff<umap.getNumberOfElements(); ff++ )
		      {
			for ( int a=0; a<3; a++ )
			  {
			    int p = newFaces(ff,a);
			    int pid = p<nOldVertices ? indexMappings[uVertex2Grid(p,0)](uVertex2Grid(p,1),uVertex2Grid(p,2)) :
			      p-nOldVertices + nInitialVertices;
			    
			    //cout<<p<<","<<pid<<" ";
			    if ( pid<0 ) 
			      {
				cout<<"PID<0! "<<p<<" "<<pid<<endl;
				if (p<nOldVertices) 
				  {
				    cout<<uVertex2Grid(p,0)<<" "<<uVertex2Grid(p,1)<<" "<<uVertex2Grid(p,2)<<endl;
				    indexMappings[uVertex2Grid(p,0)].display("indexMappings");
				  }
				throw PreProcessingError();
			      }
			    //			    initialFaces(nInitialFaces+ff, 2-a) = pid;
			    initialFaces(nInitialFaces+ff, a) = pid;
			  }
			//cout<<endl;
			initialFaces(nInitialFaces+ff,3) = -1;
		      }
		    
		  }
		currshare = -1;
		//delete cgsurfp;
	      }
	  }
      
    }
  
  delete indexMappings;
}

#else
void
Ugen::
sealHoles3D( CompositeGrid &cg, intArray *gridIndexVertexMap, intArray &boundaryHoleVertices, 
	     intArray &initialFaces, realArray &xyz_initial )
{
  int grid;
  int maxShare = 0;
  int side,axis;

  ArraySimple<bool> shareSidesUsed(cg.numberOfGrids(), cg.numberOfDimensions(), 2);
  ArraySimple<bool> bcSidesUsed(cg.numberOfGrids(), cg.numberOfDimensions(), 2);

  intArray * indexMappings = new intArray[ 6*cg.numberOfGrids() ];

  ArraySimple<int> gridID(cg.numberOfGrids());

  for ( int i=0; i<cg.numberOfGrids(); i++ )
    for ( int a=0; a<cg.numberOfDimensions(); a++ )
      {
	shareSidesUsed(i,a,0) = shareSidesUsed(i,a,1) = false;
	bcSidesUsed(i,a,0) = bcSidesUsed(i,a,0) = false;
      }

  int currshare, currbc, gid;

  CompositeGrid cgsurf;
  gid=0;
  for ( grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];

      //      CompositeGrid * cgsurfp = new CompositeGrid;
      //      CompositeGrid & cgsurf = *cgsurfp;
      intArray & givMap1 = gridIndexVertexMap[grid];
      real sj = 1;

      for ( int a1=0; a1<mg.domainDimension(); a1++ )
	for ( int s1=0; s1<2; s1++ )
	  {
	    currshare = -1;
	    if ( mg.sharedBoundaryFlag(s1,a1)>0 )
	      currshare = mg.sharedBoundaryFlag(s1,a1);

	    currbc = -1;
	    if ( (mg.boundaryFlag(s1,a1)==MappedGrid::physicalBoundary ||
		 mg.boundaryFlag(s1,a1)==MappedGrid::mixedPhysicalInterpolationBoundary) &&
		 mg.sharedBoundaryFlag(s1,a1)!=-20)
	      {	    
		//mg.mapping().incrementReferenceCount();
		ReductionMapping *redMap = 
		  new ReductionMapping(*(mg.mapping().mapPointer), a1, real(s1));
		redMap->incrementReferenceCount();
		sj = mg.mapping().mapPointer->getSignForJacobian();
		if ( a1==1 ) sj *= -1;
		if ( s1==1 ) sj *= -1;
		
		cgsurf.add(*redMap);
		// *wdh* 070415 -- build the vertex too so the boundingBox can be computed in the new way
                // cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | MappedGrid::THEboundingBox);
		cgsurf[cgsurf.numberOfGrids()-1].update(MappedGridData::THEmask | 
                                                        MappedGrid::THEvertex | MappedGrid::THEboundingBox);
		shareSidesUsed(grid,a1,s1) = true;
		intArray &mask = cgsurf[cgsurf.numberOfGrids()-1].mask();
		intArray &mgmask = mg.mask();
		Index Ib1, Ib2, Ib3;
		int ie[3]; ie[0] = ie[1] = ie[2] = 1;
		ie[a1] = 0;
		getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ib1,Ib2,Ib3,ie[0],ie[1],ie[2]);
		mask = -1;
		if ( a1==axis1 )
		  {
		    indexMappings[gid].redim(Ib2,Ib3);
		    intArray &im = indexMappings[gid];
		    for ( int i=Ib2.getBase(); i<=Ib2.getBound(); i++ )
		      for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
			{
			  mask(i,j,0) = mgmask(Ib1.getBase(),i,j);
			  if ( i>Ib2.getBase() && i< Ib2.getBound() &&
			       j>Ib3.getBase() && j< Ib3.getBound() )
			    im(i,j) = givMap1(Ib1.getBase(),i,j);
			  else
			    im(i,j) = -1;
			}
		  }
		else if ( a1==axis2 )
		  {
		    indexMappings[gid].redim(Ib1,Ib3);
		    intArray &im = indexMappings[gid];
		    for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
		      for ( int j=Ib3.getBase(); j<=Ib3.getBound(); j++ )
			{
			  mask(i,j,0) = mgmask(i,Ib2.getBase(),j);
			  if ( i>Ib1.getBase() && i< Ib1.getBound() &&
			       j>Ib3.getBase() && j< Ib3.getBound() )
			    im(i,j) = givMap1(i,Ib2.getBase(),j);
			  else
			    im(i,j) = -1;
			}
		  }
		else
		  {
		    indexMappings[gid].redim(Ib1,Ib2);
		    intArray &im = indexMappings[gid];
		    for ( int i=Ib1.getBase(); i<=Ib1.getBound(); i++ )
		      for ( int j=Ib2.getBase(); j<=Ib2.getBound(); j++ )
			{
			  mask(i,j,0) = mgmask(i,j,Ib3.getBase());
			  if ( i>Ib1.getBase() && i< Ib1.getBound() &&
			       j>Ib2.getBase() && j< Ib2.getBound() )
			    im(i,j) = givMap1(i,j,Ib3.getBase());
			  else
			    im(i,j) = -1;
			}
		  }
		
		cgsurf[cgsurf.numberOfGrids()-1].mapping().mapPointer->approximateGlobalInverse->setParameter(MappingParameters::THEboundingBoxExtensionFactor, 0.);
		cgsurf[cgsurf.numberOfGrids()-1].mapping().mapPointer->approximateGlobalInverse->setParameter(MappingParameters::THEstencilWalkBoundingBoxExtensionFactor, 0.);
		cgsurf[cgsurf.numberOfGrids()-1].mapping().mapPointer->approximateGlobalInverse->initialize();
		cgsurf[cgsurf.numberOfGrids()-1].mapping().mapPointer->setSignForJacobian(sj);
		

		//		cout<<"REDUCTION MAPPING "<<gid<<" BBOX "<<endl;
		//cgsurf[cgsurf.numberOfGrids()-1].mapping().mapPointer->approximateGlobalInverse->getBoundingBox().display();

		if ( (redMap->decrementReferenceCount()) == 0 ) delete redMap; 
		gid++;
		
	      }
	  }
    }
  
  if ( cgsurf.numberOfGrids()>0 )
    {
      // construct the hybrid connectivity for the surface and then add the new
      //  triangles to the initial face list.
      
      //(Overture::getGraphicsInterface())->plot(cgsurf,Overture::defaultGraphicsParameters());
      MappingInformation mapInfo;
      mapInfo.graphXInterface = Overture::getGraphicsInterface();
      updateHybrid(cgsurf,mapInfo);
      const CompositeGridHybridConnectivity & connect = cgsurf.getHybridConnectivity();
      const intArray & uVertex2Grid = connect.getUVertex2GridIndex();
      int ugrid = connect.getUnstructuredGridIndex();
      if ( ugrid!=-1 )
	{
	  UnstructuredMapping & umap = (UnstructuredMapping &) (*cgsurf[ugrid].mapping().mapPointer) ;
	  const realArray &umapXYZ = umap.getNodes();
	  const intArray &newFaces = umap.getElements();
	  
	  int nOldVertices = uVertex2Grid.getLength(0);
		    
	  int nInitialVertices = xyz_initial.getLength(0);
	  int nInitialFaces   = initialFaces.getLength(0);
	  int newNumberOfVertices = nInitialVertices + umap.getNumberOfNodes()-nOldVertices;
	  int newNumberOfFaces    = umap.getNumberOfElements() + initialFaces.getLength(0);
	  
	  xyz_initial.resize(newNumberOfVertices, 3);
	  initialFaces.resize(newNumberOfFaces, initialFaces.getLength(1));
	  
	  for ( int p=0; p<(umap.getNumberOfNodes()-nOldVertices); p++ )
	    for ( int a=0; a<3; a++ )
	      xyz_initial(p+nInitialVertices,a) = umapXYZ(nOldVertices+p,a);
	  
	  for ( int ff=0; ff<umap.getNumberOfElements(); ff++ )
	    {
	      for ( int a=0; a<3; a++ )
		{
		  int p = newFaces(ff,a);
		  int pid = p<nOldVertices ? indexMappings[uVertex2Grid(p,0)](uVertex2Grid(p,1),uVertex2Grid(p,2)) :
		    p-nOldVertices + nInitialVertices;
		  
		  //cout<<p<<","<<pid<<" ";
		  if ( pid<0 ) 
		    {
		      cout<<"PID<0! "<<p<<" "<<pid<<endl;
		      if (p<nOldVertices) 
			{
			  cout<<uVertex2Grid(p,0)<<" "<<uVertex2Grid(p,1)<<" "<<uVertex2Grid(p,2)<<endl;
				    indexMappings[uVertex2Grid(p,0)].display("indexMappings");
			}
		      throw PreProcessingError();
		    }
		  //			    initialFaces(nInitialFaces+ff, 2-a) = pid;
		  initialFaces(nInitialFaces+ff, a) = pid;
		}
	      //cout<<endl;
	      initialFaces(nInitialFaces+ff,3) = -1;
	    }
	  
	}
      currshare = -1;
      //delete cgsurfp;
    }
  
  delete indexMappings;
}

#endif

void
Ugen::
sealHoles( CompositeGrid &cg, intArray *gridIndexVertexMap, intArray &initialFaces, realArray &xyz_initial, intArray &initialSurfaceMapping)
{
  // seal holes in the front making a connected/water-tight boundary for the advancing front

  int grid;

  
  intArray boundaryHoleVertices;
  // boundaryHoleVertices will be filled as follows, where v is a boundary hole vertex
  // boundaryHoleVertices ( v, 0 ) - grid
  // boundaryHoleVertices ( v, 1 ) - axis
  // boundaryHoleVertices ( v, 2 ) - side
  // boundaryHoleVertices ( v, 3 ) - i1
  // boundaryHoleVertices ( v, 4 ) - i2
  // boundaryHoleVertices ( v, 5 ) - i3

  // first count and log the vertices on the boundaries :  
  accumulateBoundaryHoleVertices(cg, boundaryHoleVertices);

  // get any intersection vertices
  // estimate the number of intersections
  Range AXES(0, cg.numberOfDimensions()-1);

  cout<<"CompositeGrid information:: nd, ng "<<cg.numberOfDimensions()<<" "<<cg.numberOfComponentGrids()<<endl;

  realArray intersectionVertices(cg.numberOfComponentGrids()*cg.numberOfDimensions()*2, cg.numberOfDimensions()); 
  intArray intersectionVertexData(cg.numberOfComponentGrids()*cg.numberOfDimensions()*2, 6);

  int nIntersections = 0;

  realArray rmap1, rmap2;
  for ( grid=0; cg[0].rangeDimension()==2 && grid<cg.numberOfComponentGrids()-1; grid++ )
    {
      Mapping &map1 = cg[grid].mapping().getMapping();
      for ( int grid2=grid+1; grid2<cg.numberOfComponentGrids(); grid2++ )
	{
	  Mapping &map2 = cg[grid2].mapping().getMapping();
	  
	  for ( int axis1=0; axis1<cg[grid].domainDimension(); axis1++ )
	    for ( int side1=0; side1<2; side1++ )
	      {
		ReductionMapping reducedMap1(map1, axis1, real(side1));
		
		for ( int axis2=0; axis2<cg[grid].domainDimension(); axis2++ )
		  for ( int side2=0; side2<2; side2++ )
		    {

		      if ( ((cg[grid].sharedBoundaryFlag(side1,axis1)<=0 && cg[grid2].sharedBoundaryFlag(side2,axis2)<=0) ||
			    (cg[grid].sharedBoundaryFlag(side1,axis1) != cg[grid2].sharedBoundaryFlag(side2,axis2))) && 
			   ( ( cg[grid].boundaryFlag(side1,axis1)==MappedGrid::physicalBoundary ||
			       cg[grid].boundaryFlag(side1,axis1)==MappedGrid::mixedPhysicalInterpolationBoundary ) &&
			     ( cg[grid2].boundaryFlag(side2,axis2)==MappedGrid::physicalBoundary ||
			       cg[grid2].boundaryFlag(side2,axis2)==MappedGrid::mixedPhysicalInterpolationBoundary ) ) &&
			   map1.intersects(map2, side1, axis1, side2, axis2, -0.0001) )
			{
			  ReductionMapping reducedMap2(map2, axis2, real(side2));
			  IntersectionMapping intersection;
			  realArray localIntersection;
			  int numberOfIntersectionPoints=0;
			  // XXX 2D ! the following is for 2D only
			  intersection.intersectCurves(reducedMap1, reducedMap2, numberOfIntersectionPoints, rmap1, rmap2, localIntersection);

			  if ((nIntersections+numberOfIntersectionPoints)>intersectionVertices.getLength(0))
			    {
			      intersectionVertices.resize(nIntersections+numberOfIntersectionPoints, AXES);
			      intersectionVertexData.resize(nIntersections+numberOfIntersectionPoints, 6);
			    }
			  //if (numberOfIntersectionPoints>0) localIntersection.display("localIntersection");
			  for ( int i=0; i<numberOfIntersectionPoints; i++ )
			    {
			      for ( int ax=0; ax<AXES.getLength(); ax++ )
				intersectionVertices(nIntersections, ax) = localIntersection(ax, i);
			      
			      intersectionVertexData(nIntersections, 0) = grid;
			      intersectionVertexData(nIntersections, 1) = side1;
			      intersectionVertexData(nIntersections, 2) = axis1;
			      intersectionVertexData(nIntersections, 3) = grid2;
			      intersectionVertexData(nIntersections, 4) = side2;
			      intersectionVertexData(nIntersections, 5) = axis2;
			      
			      nIntersections++;
			    }
			}
		    } // axis2, side2
	      } // axis1, side1
	} // grid2
    } // grid1
    
  cout <<"number of intersections "<<nIntersections<<endl;
  if (nIntersections!=0) 
    intersectionVertices.resize(nIntersections, AXES);
  else
    intersectionVertices.redim(0);

#if 0
  // plot the vertices on the gap in the hole boundaries
  if ( ps!=NULL )
    {
      PlotStuffParameters psp;
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
      ps->plot(cg, psp);
      
      realArray bdynodes( boundaryHoleVertices.getLength(0)+intersectionVertices.getLength(0),
			  cg.numberOfDimensions() );

      for ( int nn=0; nn<boundaryHoleVertices.getLength(0); nn++ )
	{
	  int grid = boundaryHoleVertices(nn,0);

	  realArray & vertices = cg[grid].vertex();

	  for ( int aa=0; aa<cg.numberOfDimensions(); aa++ )
	    bdynodes(nn ,aa) = vertices( boundaryHoleVertices(nn, 3),
					 boundaryHoleVertices(nn, 4),
					 boundaryHoleVertices(nn, 5), aa );
	}

      for ( int ni=0; ni<nIntersections; ni++ )
	{
	  bdynodes(boundaryHoleVertices.getLength(0)+ni, AXES) = intersectionVertices(ni,AXES);
	}

      if ( boundaryHoleVertices.getLength(0)!=0 )
	{
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, FALSE);
	  ps->plotPoints(bdynodes, psp);
	}
    }
#endif				       
  
  if ( cg[0].domainDimension() == 2 && cg[0].rangeDimension()==2 )
    sealHoles2D( cg, boundaryHoleVertices, intersectionVertices, intersectionVertexData,
		 gridIndexVertexMap, initialFaces, xyz_initial );
  else if ( cg[0].domainDimension()==2 )
    {
      if ( advancingFront.getParameters().usingControlFunction() )
	sealSurfaceHoles( cg, boundaryHoleVertices, 
			  gridIndexVertexMap, initialFaces, xyz_initial, initialSurfaceMapping, &advancingFront );
      else
	sealSurfaceHoles( cg, boundaryHoleVertices, 
			  gridIndexVertexMap, initialFaces, xyz_initial, initialSurfaceMapping );
    }
  else
    sealHoles3D(cg, gridIndexVertexMap, boundaryHoleVertices, 
		initialFaces, xyz_initial );
  
    

}

