//#define BOUNDS_CHECK
//#define KK_DEBUG

#include "Geom.h"

#include "MappedGridOperators.h"
#include "UnstructuredOperators.h"
#include "UnstructuredMapping.h"
#include "UnstructuredGeometry.h"

namespace UNSTRUCTURED_OPS_FV2 {

  void divergence_uFV2(const UnstructuredMapping &umap,
		       const realArray &u, 
		       const UnstructuredMapping::EntityTypeEnum u_centering, 
		       const UnstructuredMapping::EntityTypeEnum surface_centering,
		       const UnstructuredMapping::EntityTypeEnum ux_centering,
		       const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
		       const realArray &scalar, const Index &C,
		       realArray &ux)
  {
    UnstructuredMappingIterator surf;
    UnstructuredMappingAdjacencyIterator u_cell, ux_cell, u_end, ux_end;

    int rDim = umap.getRangeDimension();

    int nComp = C.length();
    ArraySimple<real> uOnSurf(rDim*nComp);
    
    ux = 0;
    for ( surf  = umap.begin(surface_centering);
	  surf != umap.end(surface_centering);
	  surf++ )
      {
	int nC=0;
	uOnSurf=0.;
	u_end = umap.adjacency_end(surf, u_centering);
	for ( u_cell  = umap.adjacency_begin(surf, u_centering);
	      u_cell != u_end;
	      u_cell++ )
	  {
	    for ( int d=0; d<rDim; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++)
		uOnSurf[c+nComp*d] += u(*u_cell,0,0,c+nComp*d);
	    nC++;
	  }

	for ( int d=0; d<rDim; d++ )
	  for ( int c=C.getBase(); c<=C.getBound(); c++)
	    uOnSurf[c+nComp*d] /= real(nC); 
	
	ux_end = umap.adjacency_end(surf, ux_centering);
	for ( ux_cell  = umap.adjacency_begin(surf, ux_centering);
	      ux_cell != ux_end;
	      ux_cell++ )
	  {
	    real vol = volumes(*ux_cell);

	    for ( int d=0; d<rDim; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++ )
		ux(*ux_cell,0,0,c) += ux_cell.orientation()*uOnSurf[c+nComp*d]*surfaceAreas(*surf)*surfaceNormals(*surf,d)/vol;
	  }
      }
  }

  void xi_uFV2(int d, const UnstructuredMapping &umap,
	       const realArray &u, 
	       const UnstructuredMapping::EntityTypeEnum u_centering, 
	       const UnstructuredMapping::EntityTypeEnum surface_centering,
	       const UnstructuredMapping::EntityTypeEnum ux_centering,
	       const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
	       const realArray &scalar, const Index &C,
	       realArray &ux)
  {

    UnstructuredMappingIterator surf;
    UnstructuredMappingAdjacencyIterator u_cell, ux_cell, u_end, ux_end;

    int nComp = C.length();
    ArraySimple<real> uOnSurf(nComp);
    
    int rDim = umap.getRangeDimension();
    int dBase,dBound, offset;

    if ( d<0 )
      {
	// evaluate the full gradient
	dBase = 0;
	dBound = rDim-1;
	offset = nComp;
      }
    else if ( d<rDim )
      {
	// only evaluate the following derivative
	dBase = dBound = d;
	offset = 0;
      }
    else
      {
	cout<<"ERROR : xi_uFV2 : invalid derivative request using dimensions "<<d<<endl;
	return;
      }
    
    //    ux.redim(u); should already have been done?
    ux = 0;
    
    for ( surf  = umap.begin(surface_centering);
	  surf != umap.end(surface_centering);
	  surf++ )
      {
	int nC=0;
	uOnSurf=0.;
	u_end = umap.adjacency_end(surf, u_centering);
	for ( u_cell  = umap.adjacency_begin(surf, u_centering);
	      u_cell != u_end;
	      u_cell++ )
	  {
	    for ( int c=C.getBase(); c<=C.getBound(); c++)
	      uOnSurf[c] += u(*u_cell,0,0,c);
	    nC++;
	  }

	for ( int c=C.getBase(); c<=C.getBound(); c++)
	  uOnSurf[c] /= real(nC); 
	
	ux_end = umap.adjacency_end(surf, ux_centering);
	for ( ux_cell  = umap.adjacency_begin(surf, ux_centering);
	      ux_cell != ux_end;
	      ux_cell++ )
	  {
	    for ( int d=dBase; d<=dBound; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++)
		ux(*ux_cell,0,0,c+offset*d) += ux_cell.orientation()*surfaceNormals(*surf,d)*surfaceAreas(*surf)*uOnSurf[c]/volumes(*ux_cell);
	  }

      }
    //	ux.display();
  }

  void xixj_uFV2(int d1, int d2, const UnstructuredMapping &umap,
		 const realArray &u, 
		 const UnstructuredMapping::EntityTypeEnum u_centering, 
		 const UnstructuredMapping::EntityTypeEnum surface_centering,
		 const UnstructuredMapping::EntityTypeEnum ux_centering,
		 const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
		 const realArray &scalar,const Index &C, 
		 realArray &uxx)
  {

    UnstructuredMappingIterator cell; // edge/face/region 1/2/3D
    UnstructuredMappingAdjacencyIterator cellFace, cellFace_end, faceEdge, faceEdge_end, edgeVert, edgeVert_end;
    UnstructuredMappingAdjacencyIterator cellVert, cellVert_end, faceVert, faceVert_end;

    ArraySimpleFixed<real,3,1,1,1> faceNorm, subCellNorm, xc, areaNormal;
    
    int dDim = umap.getDomainDimension();
    int rDim = umap.getRangeDimension();

    int d1Base,d1Bound;
    int d2Base,d2Bound;
    if ( d1<0 || d2<0 )
      {
	// evaluate the full laplacian
	d1Base = d2Base = 0;
	d1Bound = d2Bound = rDim-1;
      }
    else if ( d1<rDim && d2<rDim )
      {
	// only evaluate the following mixed derivative
	d1Base = d1Bound = d1;
	d2Base = d2Bound = d2;
      }
    else
      {
	cout<<"ERROR : xixj_uFV2 : invalid derivative request using dimensions "<<d1<<"  "<<d2<<endl;
	return;
      }
    const realArray &vertices = umap.getNodes(); 

    int nComp = C.length();
    ArraySimple<real> uAvg(nComp);
    ArraySimple< ArraySimpleFixed<real,3,1,1,1> > der(nComp);


    uxx = 0.;

    // there are two ways to do this: either precompute the subcell geometry and do the following loop
    //   generically, with no conditionals on the dimension of the problem; or compute the geometry
    //   on the fly.  Right now the latter is chosen; the code is less readable and generic but may
    //   be more efficient on cache based machines.
    if ( u_centering==UnstructuredMapping::Vertex )
      {
	UnstructuredMapping::EntityTypeEnum der_centering = ( dDim==2 ? UnstructuredMapping::Face : UnstructuredMapping::Region );
	
	for ( cell=umap.begin(der_centering); 
	      cell!=umap.end(der_centering);
	      cell++ )
	  {
	    for ( int c=C.getBase(); c<=C.getBound(); c++ )
	      der[c] = 0.;

	    cellFace_end = umap.adjacency_end(cell,UnstructuredMapping::Face);

	    // compute the cell center
	    int nv=0;
	    xc=0.;
	    cellVert_end = umap.adjacency_end(cell,UnstructuredMapping::Vertex);
	    for ( cellVert=umap.adjacency_begin(cell,UnstructuredMapping::Vertex);
		  cellVert!=cellVert_end;
		  cellVert++ )
	      {
		for ( int a=0; a<rDim; a++ )
		  xc[a] += vertices(*cellVert,a);
		nv++;
	      }

	    for ( int a=0; a<rDim; a++ )
	      xc[a] /= real(nv);
	    
	    if ( rDim==2 )
	      {
		cellFace_end = umap.adjacency_end(cell,UnstructuredMapping::Edge);
		real area = 0;
		for ( cellFace=umap.adjacency_begin(cell,UnstructuredMapping::Edge);
		      cellFace!=cellFace_end;
		      cellFace++ )
		  {
		    // in 2D we connect the cell (face) center to the center of the edge to form the area normals
		    ArraySimpleFixed<real,2,1,1,1> edgeVertices[2], edgeCenter;
		    int v=0;
		    
		    for ( int a=0; a<rDim; a++ )
		      edgeCenter[a] = 0;
		    
		    edgeVert_end = umap.adjacency_end(cellFace,UnstructuredMapping::Vertex);
		    uAvg=0.;
		    int nAvg=0;

		    for ( edgeVert  = umap.adjacency_begin(cellFace,UnstructuredMapping::Vertex);
			  edgeVert != edgeVert_end;
			  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		      {
			for ( int a=0; a<rDim; a++ )
			  {
			    edgeVertices[v][a] = vertices(*edgeVert,a);
			    edgeCenter[a] += edgeVertices[v][a]/2.;
			  }
			
			for ( int c=C.getBase(); c<=C.getBound(); c++ )
			  uAvg[c] += u(*edgeVert,0,0,c);

			v++;
		      }

		    faceNorm[0] = cellFace.orientation()*(edgeVertices[1][1]-edgeVertices[0][1]);
		    faceNorm[1] = cellFace.orientation()*(edgeVertices[0][0]-edgeVertices[1][0]);

		    for ( int c=C.getBase(); c<=C.getBound(); c++ )
		      {
			uAvg[c] /= real(v);

			for ( int d=d1Base; d<=d1Bound; d++ )
			  der[c][d] += uAvg[c]*faceNorm[d];
		      }

		    area += cellFace.orientation()*triangleArea2D(edgeVertices[0], edgeVertices[1], xc);

		  }

		for ( int c=C.getBase(); c<=C.getBound(); c++ )
		  for ( int d=d1Base; d<=d1Bound; d++ )
		    der[c][d] /= area;
		
		for ( cellFace=umap.adjacency_begin(cell,UnstructuredMapping::Edge);
		      cellFace!=cellFace_end;
		      cellFace++ )
		  {
		    ArraySimpleFixed<real,2,1,1,1> edgeVertices[2], edgeCenter;
		    int v=0;
		    
		    for ( int a=0; a<rDim; a++ )
		      edgeCenter[a] = 0;
		    
		    edgeVert_end = umap.adjacency_end(cellFace,UnstructuredMapping::Vertex);
		    uAvg=0.;
		    int nAvg=0;
		    
		    for ( edgeVert  = umap.adjacency_begin(cellFace,UnstructuredMapping::Vertex);
			  edgeVert != edgeVert_end;
			  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		      {
			for ( int a=0; a<rDim; a++ )
			  {
			    edgeVertices[v][a] = vertices(*edgeVert,a);
			    edgeCenter[a] += edgeVertices[v][a]/2.;
			  }
			v++;
		      }

		    subCellNorm[0] = xc[1] - edgeCenter[1];
		    subCellNorm[1] = -xc[0] + edgeCenter[0];
		    
		    real orient = cellFace.orientation();

		    int fac=1;
		    for ( edgeVert  = umap.adjacency_begin(cellFace,UnstructuredMapping::Vertex);
			  edgeVert != edgeVert_end;
			  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		      {
			for ( int c=C.getBase(); c<=C.getBound(); c++ )
			  {
			    d1 = d1Base;
			    for ( int d2=d2Base; d2<=d2Bound; d2++ )
			      {
				uxx(*edgeVert,0,0,c) += fac*orient*(der[c][d1]*subCellNorm[d2])/volumes(*edgeVert,0,0);
				d1++;
			      }
			  }
			fac = -fac;
		      }
		  
		  }
		
	      }
	    else if ( rDim==3 )
	      {
		cellFace_end = umap.adjacency_end(cell,UnstructuredMapping::Face);
		real vol=0;
		for ( cellFace=umap.adjacency_begin(cell,UnstructuredMapping::Face);
		      cellFace!=cellFace_end;
		      cellFace++ )
		  {
		    // compute the face center, normal and the average of u on the face
		    ArraySimpleFixed<real,3,1,1,1> edgeVertices[2], edgeCenter,fc;
	    
		    faceVert_end = umap.adjacency_end(cellFace, UnstructuredMapping::Vertex);
		    uAvg = 0.;
		    int nv=0;
		    fc=0.;
		    for ( faceVert=umap.adjacency_begin(cellFace, UnstructuredMapping::Vertex);
			  faceVert!=faceVert_end;
			  faceVert++ )
		      {
			for ( int a=0; a<rDim; a++ )
			  fc[a] += vertices(*faceVert,a);
			for ( int c=C.getBase(); c<=C.getBound(); c++ )
			  uAvg[c] += u(*faceVert,0,0,c);

			nv++;
		      }

		    for ( int a=0; a<rDim; a++ )
		      fc[a] /= nv;

		    for ( int c=C.getBase(); c<=C.getBound(); c++ )
		      uAvg[c] /= nv;

		    faceEdge_end = umap.adjacency_end(cellFace, UnstructuredMapping::Edge);
		    for ( faceEdge=umap.adjacency_begin(cellFace, UnstructuredMapping::Edge);
			  faceEdge!=faceEdge_end;
			  faceEdge++ )
		      {
			edgeVert_end = umap.adjacency_end(faceEdge, UnstructuredMapping::Vertex);
			int v=0;
			edgeCenter = 0.;
			for ( edgeVert=umap.adjacency_begin(faceEdge, UnstructuredMapping::Vertex);
			      edgeVert!=edgeVert_end;
			      edgeVert++ )
			  {
			    for ( int a=0; a<rDim; a++ )
			      {
				edgeVertices[v][a] = vertices(*edgeVert,a);
				edgeCenter[a] += edgeVertices[v][a]/2.;
			      }
			    v++;
			  }

			ArraySimpleFixed<real,3,1,1,1> areaNormal = areaNormal3D(edgeVertices[0],edgeVertices[1],fc);
			for ( int a=0; a<rDim; a++ )
			  areaNormal[a] *= cellFace.orientation()*faceEdge.orientation();

			for ( int c=C.getBase(); c<=C.getBound(); c++ )
			  for ( int a=d1Base; a<=d1Bound; a++ )
			    der[c][a] += areaNormal[a]*uAvg[c];
			
			vol += 
			  faceEdge.orientation()*cellFace.orientation()*
			  tetVolume(edgeVertices[0],fc,edgeVertices[1],xc);
			
		      }

		  }

		for ( int c=C.getBase(); c<=C.getBound(); c++ )
		  for ( int a=d1Base; a<=d1Bound; a++ )
		    der[c][a] /= vol;
		
		for ( cellFace=umap.adjacency_begin(cell,UnstructuredMapping::Face);
		      cellFace!=cellFace_end;
		      cellFace++ )
		  {
		    // compute the face center, normal and the average of u on the face
		    ArraySimpleFixed<real,3,1,1,1> edgeVertices[2], edgeCenter,fc;
		    
		    UnstructuredMappingAdjacencyIterator faceVert_end = umap.adjacency_end(cellFace, UnstructuredMapping::Vertex);
		    int nv=0;
		    fc=0.;
		    for ( faceVert=umap.adjacency_begin(cellFace, UnstructuredMapping::Vertex);
			  faceVert!=faceVert_end;
			  faceVert++ )
		      {
			for ( int a=0; a<rDim; a++ )
			  fc[a] += vertices(*faceVert,a);
			nv++;
		      }


		    for ( int a=0; a<rDim; a++ )
		      fc[a] /= nv;

		    faceEdge_end = umap.adjacency_end(cellFace, UnstructuredMapping::Edge);
		    for ( faceEdge=umap.adjacency_begin(cellFace, UnstructuredMapping::Edge);
			  faceEdge!=faceEdge_end;
			  faceEdge++ )
		      {
			edgeVert_end = umap.adjacency_end(faceEdge, UnstructuredMapping::Vertex);
			int v=0;
			edgeCenter = 0.;
			for ( edgeVert=umap.adjacency_begin(faceEdge, UnstructuredMapping::Vertex);
			      edgeVert!=edgeVert_end;
			      edgeVert++ )
			  {
			    for ( int a=0; a<rDim; a++ )
			      {
				edgeVertices[v][a] = vertices(*edgeVert,a);
				edgeCenter[a] += edgeVertices[v][a]/2.;
			      }
			    v++;
			  }

			subCellNorm =  areaNormal3D(edgeCenter, xc, fc);
			for ( int a=0; a<rDim; a++ )
			  subCellNorm[a] *= cellFace.orientation()*faceEdge.orientation();

			real orient = 1; // the normal is already pointing in the direction of the edge
			
			int fac=1;
			for ( edgeVert  = umap.adjacency_begin(faceEdge,UnstructuredMapping::Vertex);
			      edgeVert != edgeVert_end;
			      edgeVert++ ) // we could optimize this loop by getting the edge entities directly
			  {
			    for ( int c=C.getBase(); c<=C.getBound(); c++ )
			      {
				d1 = d1Base;
				for ( int d2=d2Base; d2<=d2Bound; d2++ )
				  {
				    uxx(*edgeVert,0,0,c) += fac*orient*(der[c][d1]*subCellNorm[d2])/volumes(*edgeVert,0,0);
				    d1++;
				  }
			      }
			    fac = -fac;
			  }
		      
		      }
		  }
	      }
	    else
	      {
		cout<<"ERROR : laplacian_uFV2 not implemented for rangeDimension "<<rDim<<endl;
		return;
	      }
	  }
      }
    else
      { // isAllCellCentered 

	// what a pain. until we need it, keep this code short by doing div_{cell2vertex} grad_{vertex2cell} u

	realArray edgeNorms, edgeAreas, vertVols;

	edgeNorms.redim(umap.capacity(UnstructuredMapping::Edge),1,1,umap.getRangeDimension());
	edgeAreas.redim(umap.capacity(UnstructuredMapping::Edge),1,1);
	vertVols.redim(umap.capacity(UnstructuredMapping::Vertex),1,1);
	UnstructuredGeometry::computeGeometry((UnstructuredMapping &)umap, 0, &edgeNorms, &vertVols,0,0,0);

	Range AXES(rDim);

	for ( int f=0; f<edgeNorms.getLength(0); f++ )
	  {
	    real mag = sqrt(sum(pow(edgeNorms(f,0,0,AXES),2)));
	    
	    if ( fabs(mag)>100*REAL_MIN ) 
	      edgeNorms(f,0,0,AXES) /= mag;
	    
	    edgeAreas(f,0,0) = mag;
	  }

	UnstructuredMapping::EntityTypeEnum stmp_cent = UnstructuredMapping::Edge;
	UnstructuredMapping::EntityTypeEnum gtmp_cent = UnstructuredMapping::Vertex;

	edgeNorms.reshape(edgeNorms.getLength(0),edgeNorms.getLength(3));
	edgeAreas.reshape(edgeAreas.getLength(0),edgeAreas.getLength(3));
	vertVols.reshape(vertVols.getLength(0),vertVols.getLength(3));

	if ( d1Base==d1Bound )
	  {
	    realArray xd1(u.getLength(0),1,1,u.getLength(3));
	    xi_uFV2(d1,umap, u, u_centering, stmp_cent, gtmp_cent, edgeNorms, edgeAreas, vertVols, scalar, C, xd1);
	    xi_uFV2(d2,umap, xd1, gtmp_cent, surface_centering, ux_centering, surfaceNormals, surfaceAreas, volumes, scalar, C, uxx);
	  }
	else
	  {
	    realArray grad(umap.capacity(UnstructuredMapping::Vertex),1,1,u.getLength(3)*rDim);
	    xi_uFV2(d1,umap, u, u_centering, stmp_cent, gtmp_cent, edgeNorms, edgeAreas, vertVols, scalar, C, grad);
	    divergence_uFV2(umap, grad, gtmp_cent, surface_centering, ux_centering, surfaceNormals, surfaceAreas, volumes, scalar, C, uxx);
	  }
      }
  }
  
  void u_xi_uFV2(int d, const UnstructuredMapping &umap,
		 const realArray &u, 
		 const UnstructuredMapping::EntityTypeEnum u_centering, 
		 const UnstructuredMapping::EntityTypeEnum surface_centering,
		 const UnstructuredMapping::EntityTypeEnum ux_centering,
		 const realArray &surfaceNormals, const realArray &surfaceAreas, 
		 const realArray &scalar, const Index &C,
		 realArray &ux)
  {
    // an "undivided" difference of u, the unit normals are used and the integral is not divided by the volume

    UnstructuredMappingIterator surf,uxc;
    UnstructuredMappingAdjacencyIterator u_cell, ux_cell, u_end, ux_end;

    int nComp = C.length();
    ArraySimple<real> uOnSurf(nComp);
    
    ArraySimple<real> surfA(umap.size(ux_centering));
    surfA = 0.;

    int rDim = umap.getRangeDimension();
    int dBase,dBound, offset;

    if ( d<0 )
      {
	// evaluate the full gradient
	dBase = 0;
	dBound = rDim-1;
	offset = nComp;
      }
    else if ( d<rDim )
      {
	// only evaluate the following derivative
	dBase = dBound = d;
	offset = 0;
      }
    else
      {
	cout<<"ERROR : xi_uFV2 : invalid derivative request using dimensions "<<d<<endl;
	return;
      }
    
    //    ux.redim(u); should already have been done?
    ux = 0;
    
    for ( surf  = umap.begin(surface_centering);
	  surf != umap.end(surface_centering);
	  surf++ )
      {
	int nC=0;
	uOnSurf=0.;
	u_end = umap.adjacency_end(surf, u_centering);
	for ( u_cell  = umap.adjacency_begin(surf, u_centering);
	      u_cell != u_end;
	      u_cell++ )
	  {
	    for ( int c=C.getBase(); c<=C.getBound(); c++)
	      uOnSurf[c] += u(*u_cell,0,0,c);
	    nC++;
	  }

	for ( int c=C.getBase(); c<=C.getBound(); c++)
	  uOnSurf[c] /= real(nC); 
	
	ux_end = umap.adjacency_end(surf, ux_centering);
	for ( ux_cell  = umap.adjacency_begin(surf, ux_centering);
	      ux_cell != ux_end;
	      ux_cell++ )
	  {
	    for ( int d=dBase; d<=dBound; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++)
		ux(*ux_cell,0,0,c+offset*d) += ux_cell.orientation()*surfaceNormals(*surf,d)*surfaceAreas(*surf)*uOnSurf[c];

	    surfA(*ux_cell) += surfaceAreas(*surf);
	  }

      }

    for ( uxc=umap.begin(ux_centering); uxc!=umap.end(ux_centering); uxc++ )
      if ( surfA(*uxc)>REAL_EPSILON )
	for ( int d=dBase; d<=dBound; d++ )
	  for ( int c=C.getBase(); c<=C.getBound(); c++)
	    ux(*uxc,0,0,c+offset*d)/=surfA(*uxc);

    //	ux.display();
  }


  void u_xixj_uFV2(int d1, int d2, const UnstructuredMapping &umap,
		 const realArray &u, 
		 const UnstructuredMapping::EntityTypeEnum u_centering, 
		 const UnstructuredMapping::EntityTypeEnum surface_centering,
		 const UnstructuredMapping::EntityTypeEnum ux_centering,
		 const realArray &scalar,const Index &C, 
		 realArray &uxx)
  {

    // an "undivided" second difference, the value at the center is subtracted from the average of the neighbors

    UnstructuredMappingIterator cell; // edge/face/region 1/2/3D
    UnstructuredMappingIterator face;
    UnstructuredMappingAdjacencyIterator cellFace, cellFace_end, faceEdge, faceEdge_end, edgeVert, edgeVert_end;
    UnstructuredMappingAdjacencyIterator cellVert, cellVert_end, faceVert, faceVert_end, faceCell;

    ArraySimpleFixed<real,3,1,1,1> faceNorm, subCellNorm, xc, areaNormal;
    
    int dDim = umap.getDomainDimension();
    int rDim = umap.getRangeDimension();

    int d1Base,d1Bound;
    int d2Base,d2Bound;
    if ( d1<0 || d2<0 )
      {
	// evaluate the full laplacian
	d1Base = d2Base = 0;
	d1Bound = d2Bound = rDim-1;
      }
    else if ( d1<rDim && d2<rDim )
      {
	// only evaluate the following mixed derivative
	d1Base = d1Bound = d1;
	d2Base = d2Bound = d2;
      }
    else
      {
	cout<<"ERROR : xixj_uFV2 : invalid derivative request using dimensions "<<d1<<"  "<<d2<<endl;
	return;
      }
    const realArray &vertices = umap.getNodes(); 

    int nComp = C.length();
    ArraySimple<real> uAvg(nComp);
    ArraySimple< ArraySimpleFixed<real,2,1,1,1> > adjU(nComp);


    uxx = 0.;

    ArraySimple<real> cnt(umap.size(u_centering));
    cnt = 0.;

    for ( face=umap.begin(surface_centering);
	  face!=umap.end(surface_centering);
	  face++ )
      {
	uAvg = 0.;
	int nc=0;
	
	for ( faceCell=umap.adjacency_begin(face, u_centering);
	      faceCell!=umap.adjacency_end(face,u_centering);
	      faceCell++ )
	  {
	    for ( int c=C.getBase(); c<=C.getBound(); c++ )
	      adjU[c][nc] = u(*faceCell,0,0,c);
	    cnt[*faceCell]++;
	    nc++;
	  }

	int ncc=0;
	for ( faceCell=umap.adjacency_begin(face, u_centering);
	      faceCell!=umap.adjacency_end(face,u_centering);
	      faceCell++ )
	  {
	    for ( int c=C.getBase(); c<=C.getBound(); c++ )
	      {
		uxx(*faceCell,0,0,c) -= adjU[c][ncc];
		uxx(*faceCell,0,0,c) += adjU[c][(ncc+1)%nc];
	      }
	    
	    ncc++;
	  }
      }
    
  }    


  void u_divergence_uFV2(const UnstructuredMapping &umap,
		       const realArray &u, 
		       const UnstructuredMapping::EntityTypeEnum u_centering, 
		       const UnstructuredMapping::EntityTypeEnum surface_centering,
		       const UnstructuredMapping::EntityTypeEnum ux_centering,
		       const realArray &surfaceNormals, 
		       const realArray &scalar, const Index &C,
		       realArray &ux)
  {
    UnstructuredMappingIterator surf;
    UnstructuredMappingAdjacencyIterator u_cell, ux_cell, u_end, ux_end;

    int rDim = umap.getRangeDimension();

    int nComp = C.length();
    ArraySimple<real> uOnSurf(rDim*nComp);
    
    ux = 0;
    for ( surf  = umap.begin(surface_centering);
	  surf != umap.end(surface_centering);
	  surf++ )
      {
	int nC=0;
	uOnSurf=0.;
	u_end = umap.adjacency_end(surf, u_centering);
	for ( u_cell  = umap.adjacency_begin(surf, u_centering);
	      u_cell != u_end;
	      u_cell++ )
	  {
	    for ( int d=0; d<rDim; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++)
		uOnSurf[c+nComp*d] += u(*u_cell,0,0,c+nComp*d);

	    nC++;
	  }
	
	for ( int d=0; d<rDim; d++ )
	  for ( int c=C.getBase(); c<=C.getBound(); c++)
	    uOnSurf[c+nComp*d] /= real(nC); 
	
	ux_end = umap.adjacency_end(surf, ux_centering);
	for ( ux_cell  = umap.adjacency_begin(surf, ux_centering);
	      ux_cell != ux_end;
	      ux_cell++ )
	  {
	    for ( int d=0; d<rDim; d++ )
	      for ( int c=C.getBase(); c<=C.getBound(); c++ )
		ux(*ux_cell,0,0,c) += ux_cell.orientation()*uOnSurf[c+nComp*d]*surfaceNormals(*surf,d);
	  }
	
      }

  }
}

using namespace UNSTRUCTURED_OPS_FV2;

int
UnstructuredOperators::
derivative(const MappedGridOperators::derivativeTypes &derivativeType,
	   const realArray &u,
	   const realArray &scalar,
	   realArray &ux,
	   const Index & I1  /*= nullIndex*/, 
	   const Index & C  /*= nullIndex*/)
{
    // right now, because of the iterators and possible holes/ghosts in the entity arrays, indexing
    //   with I really does not make sense.  Now then, we could use I as an entity tag and only compute
    //   derivative on entities with that tag...
  if ( I1!=nullIndex )
    {      
      cout<<"ERROR : UnstructuredOperators::derivative ::xi_uFV2 : indexing not supported "<<endl;
      return 1;
    }

  const UnstructuredMapping &umap = *((UnstructuredMapping*)(mg->mapping().mapPointer));

  UnstructuredMapping::EntityTypeEnum u_centering,s_centering,ux_centering;

  // determine centering of the variables; for now we assume that u and ux are centered the same
  if ( mg->isAllVertexCentered() )
    {
      u_centering = ux_centering = UnstructuredMapping::Vertex;
      s_centering = UnstructuredMapping::Edge;
    }
  else
    {
      u_centering = ux_centering = UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension());
      s_centering = UnstructuredMapping::EntityTypeEnum( int(u_centering)-1 );
    }

  realArray &surfaceNormals = mg->faceNormal();
  realArray &surfaceAreas = mg->faceArea();
  realArray &volumes = mg->cellVolume();

  Index CC = C==nullIndex ? Index(0) : C;

  surfaceNormals.reshape(surfaceNormals.getLength(0),surfaceNormals.getLength(3));
  surfaceAreas.reshape(surfaceAreas.getLength(0),surfaceAreas.getLength(3));
  volumes.reshape(volumes.getLength(0),volumes.getLength(3));
#define UXI(dim) xi_uFV2(dim, umap, u, u_centering, s_centering, ux_centering, \
                         surfaceNormals, surfaceAreas, volumes, \
                         scalar, CC, ux)

#define UXIXJ(dim1,dim2) xixj_uFV2(dim1,dim2, umap, u, u_centering, s_centering, ux_centering, \
                         surfaceNormals, surfaceAreas, volumes, \
                         scalar, CC, ux)

  switch(derivativeType)
    {
    case MappedGridOperators::xDerivative:
      UXI(0);
      break;
    case MappedGridOperators::yDerivative:
      UXI(1);
      break;
    case MappedGridOperators::zDerivative:
      UXI(2);
      break;
    case MappedGridOperators::xxDerivative:
      UXIXJ(0,0);
      break;
    case MappedGridOperators::xyDerivative:
      UXIXJ(0,1);
      break;
    case MappedGridOperators::xzDerivative:
      UXIXJ(0,2);
      break;
      //    case MappedGridOperators::yxDerivative:
      //      UXIXJ(1,0);
      //      break;
    case MappedGridOperators::yyDerivative:
      UXIXJ(1,1);
      break;
    case MappedGridOperators::yzDerivative:
      UXIXJ(1,2);
      break;
      //    case MappedGridOperators::zxDerivative:
      //      xixj_uFV2(2,0,u,scalar,ux,CC);
      //      break;
      //    case MappedGridOperators::zyDerivative:
      //      xixj_uFV2(2,1,u,scalar,ux,CC);
      //      break;
    case MappedGridOperators::zzDerivative:
      UXIXJ(2,2);
      break;
    case MappedGridOperators::laplacianOperator:
      UXIXJ(-1,-1);
      //      laplacian_uFV2(u,scalar,ux,CC);
      break;
    case MappedGridOperators::gradient:
      UXI(-1);
      //      gradient_uFV2(u,scalar,ux,CC); 
      break;
    case MappedGridOperators::divergence:
      divergence_uFV2(umap,u,u_centering,s_centering,ux_centering,surfaceNormals, surfaceAreas, volumes, scalar,CC,ux);
      break;
    default:
      if ( undividedDerivative(derivativeType,u,scalar,ux,I1,C)==1 )
	{
	  cout<<"ERROR : UnstructuredOperators::derivative : operator "<<derivativeType<<" not implemented yet"<<endl;
	  return 1;
	}
    }

  surfaceNormals.reshape(surfaceNormals.getLength(0),1,1,surfaceNormals.getLength(1));
  surfaceAreas.reshape(surfaceAreas.getLength(0),1,1,surfaceAreas.getLength(1));
  volumes.reshape(volumes.getLength(0),1,1,volumes.getLength(1));

#undef UXI
#undef UXIXJ
  
  return 0;
}

int 
UnstructuredOperators::
undividedDerivative(const MappedGridOperators::derivativeTypes &derivativeType,
		    const realArray &u,
		    const realArray &scalar,
		    realArray &ux,
		    const Index & I1, 
		    const Index & C )
{
    // right now, because of the iterators and possible holes/ghosts in the entity arrays, indexing
    //   with I really does not make sense.  Now then, we could use I as an entity tag and only compute
    //   derivative on entities with that tag...
  if ( I1!=nullIndex )
    {      
      cout<<"ERROR : UnstructuredOperators::derivative ::xi_uFV2 : indexing not supported "<<endl;
      return 1;
    }

  const UnstructuredMapping &umap = *((UnstructuredMapping*)(mg->mapping().mapPointer));

  UnstructuredMapping::EntityTypeEnum u_centering,s_centering,ux_centering;

  // determine centering of the variables; for now we assume that u and ux are centered the same
  if ( mg->isAllVertexCentered() )
    {
      u_centering = ux_centering = UnstructuredMapping::Vertex;
      s_centering = UnstructuredMapping::Edge;
    }
  else
    {
      u_centering = ux_centering = UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension());
      s_centering = UnstructuredMapping::EntityTypeEnum( int(u_centering)-1 );
    }

  realArray &surfaceNormals = mg->faceNormal();
  realArray &surfaceAreas = mg->faceArea();

  Index CC = C==nullIndex ? Index(0) : C;

  surfaceNormals.reshape(surfaceNormals.getLength(0),surfaceNormals.getLength(3));  
  surfaceAreas.reshape(surfaceAreas.getLength(0),surfaceAreas.getLength(3));

#define UXI(dim) u_xi_uFV2(dim, umap, u, u_centering, s_centering, ux_centering, \
                         surfaceNormals, surfaceAreas,\
                         scalar, CC, ux)

#define UXIXJ(dim1,dim2) u_xixj_uFV2(dim1,dim2, umap, u, u_centering, s_centering, ux_centering, \
                         scalar, CC, ux)
  switch(derivativeType)
    {
    case MappedGridOperators::r1Derivative:
      UXI(0);
      break;
    case MappedGridOperators::r2Derivative:
      UXI(1);
      break;
    case MappedGridOperators::r3Derivative:
      UXI(2);
      break;
    case MappedGridOperators::r1r1Derivative:
      UXIXJ(0,0);
      break;
    case MappedGridOperators::r1r2Derivative:
      UXIXJ(0,1);
      break;
    case MappedGridOperators::r1r3Derivative:
      UXIXJ(0,2);
      break;
      //    case MappedGridOperators::yxDerivative:
      //      UXIXJ(1,0);
      //      break;
    case MappedGridOperators::r2r2Derivative:
      UXIXJ(1,1);
      break;
    case MappedGridOperators::r2r3Derivative:
      UXIXJ(1,2);
      break;
      //    case MappedGridOperators::zxDerivative:
      //      xixj_uFV2(2,0,u,scalar,ux,CC);
      //      break;
      //    case MappedGridOperators::zyDerivative:
      //      xixj_uFV2(2,1,u,scalar,ux,CC);
      //      break;
    case MappedGridOperators::r3r3Derivative:
      UXIXJ(2,2);
      break;
    default:
      cout<<"ERROR : UnstructuredOperators::derivative : operator "<<derivativeType<<" not implemented yet"<<endl;
      return 1;
    }

  surfaceNormals.reshape(surfaceNormals.getLength(0),1,1,surfaceNormals.getLength(1));
  surfaceAreas.reshape(surfaceAreas.getLength(0),1,1,surfaceAreas.getLength(1));

#undef UXI
#undef UXIXJ

  return 0;
}

int
UnstructuredOperators::
assignCoefficients(const MappedGridOperators::derivativeTypes &derivativeType,
		   realArray &coeff,
		   const realArray &scalar,
		   const Index & I1  /*= nullIndex*/, 
		   const Index & E   /*= nullIndex*/,   
		   const Index & C  /*= nullIndex*/)
{

  return 0;
}
  
