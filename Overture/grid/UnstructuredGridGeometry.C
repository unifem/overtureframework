//#define BOUNDS_CHECK
//#define KK_DEBUG

#include "Geom.h"
#include "MappedGrid.h"
#include "UnstructuredMapping.h"


namespace UnstructuredGeometry {

  void computeCellCenters(UnstructuredMapping &umap, realArray &c)
  {
    int domainDimension = umap.getDomainDimension();
    int rangeDimension = umap.getRangeDimension();
    
    Range AXES(rangeDimension);

    UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(domainDimension);
    UnstructuredMappingIterator cell;
    UnstructuredMappingAdjacencyIterator cellVert, cellVert_end;
    
    const realArray &vertices = umap.getNodes();
    
    //    c.redim(umap.size(cellType),1,1,umap.getRangeDimension());

    for ( cell=umap.begin(cellType); cell!=umap.end(cellType); cell++ )
      {
	c(*cell,0,0,AXES) = 0.;
	
	int nV=0;
	cellVert_end = umap.adjacency_end(cell, UnstructuredMapping::Vertex);
	for ( cellVert  = umap.adjacency_begin(cell, UnstructuredMapping::Vertex);
	      cellVert != cellVert_end;
	      cellVert++ )
	  {
	    for ( int a=0; a<rangeDimension; a++ )
	      c(*cell,0,0,a) += vertices(*cellVert,a);
	    
	    nV++;
	  }
	
	c(*cell,0,0,AXES) /= real(nV);
	
      }
  }

  void computeGeometry( UnstructuredMapping &umap, 
			realArray *cellNorm, realArray *edgeNorm, realArray *vertVol, realArray *cellVol, 
			realArray *subCellNorms, realArray *subCellVols )
  {

    if ( !( cellNorm || edgeNorm || vertVol || cellVol || subCellNorms || subCellVols ) )
      return;

    UnstructuredMapping::EntityTypeEnum cellType = 
      umap.getRangeDimension()==2 ? UnstructuredMapping::Face : UnstructuredMapping::Region;

    UnstructuredMappingIterator cell,region,face,edge;
    UnstructuredMappingAdjacencyIterator cellVert,faceVert,edgeFace,faceCell,edgeVert,edgeVert_end,edgeCell, cellEdge, 
      cellEdge_end, cellFace, cellFace_end, faceEdge, faceEdge_end;

    realArray cellSurfAreaNormals, edgeAreaNormals, vertexVolumes, cellVolumes, subCellNormals, subCellVolumes;

    int rDim = umap.getRangeDimension();
    int dDim = umap.getDomainDimension();

    const realArray &vertices = umap.getNodes(); // legacy method

    umap.buildEntity(UnstructuredMapping::Edge); // make sure edges are built!

    if ( cellNorm )
      {
	cellSurfAreaNormals.reference(*cellNorm);
	//	cellSurfAreaNormals.redim( umap.capacity(UnstructuredMapping::EntityTypeEnum( int(cellType)-1)), 1,1, rDim);
	cellSurfAreaNormals = 0;
      }
    if ( edgeNorm )
      {
	edgeAreaNormals.reference(*edgeNorm);
	//	edgeAreaNormals.redim(umap.capacity(UnstructuredMapping::Edge), 1,1, rDim);
	edgeAreaNormals = 0;
      }
    if ( vertVol )
      {
	vertexVolumes.reference(*vertVol);
	//	vertexVolumes.redim(umap.capacity(UnstructuredMapping::Vertex), 1,1);
	vertexVolumes = 0;
      }
    if ( cellVol )
      {
	cellVolumes.reference(*cellVol);
	//	cellVolumes.redim(umap.capacity(cellType), 1,1);
	cellVolumes = 0;
      }
    if ( subCellNorms || subCellVols )
      {
	UnstructuredMappingAdjacencyIterator cellEdge, cellEdge_end;
	// first compute the number of subcell things
	int nSubC = 0;
	for ( cell=umap.begin(cellType); cell!=umap.end(cellType); cell++ )
	  {
	    cellEdge_end = umap.adjacency_end(cell, UnstructuredMapping::Edge); 

	    for ( cellEdge = umap.adjacency_begin(cell, UnstructuredMapping::Edge); 
		  cellEdge != cellEdge_end;
		  cellEdge++ )
	      nSubC += ( dDim==2 ? 1 : 2 );
	  }

	if ( subCellNorms )
	  {
	    //	    subCellNormals.reference(*subCellNorms);
	    subCellNormals.redim(nSubC,rDim);
	    subCellNormals = 0.;
	  }

	if ( subCellVols )
	  {
	    subCellVolumes.reference(*subCellVols);
	    //	    subCellVolumes.redim(nSubC);
	    subCellVolumes = 0.;
	  }
      }

    ArraySimple< ArraySimpleFixed<real,3,1,1,1> >  cellCenters(umap.size(cellType));

    ArraySimple< ArraySimpleFixed<real,3,1,1,1> > faceCenters;
    if ( rDim==3 )
      faceCenters.resize(umap.size(UnstructuredMapping::Face));

    // first compute the cell centers (used in later computations)
    for ( cell=umap.begin(cellType); cell!=umap.end(cellType); cell++ )
      {
	cellCenters[*cell] = 0.;

	int nV=0;
	for ( cellVert  = umap.adjacency_begin(cell, UnstructuredMapping::Vertex);
	      cellVert != umap.adjacency_end(cell, UnstructuredMapping::Vertex);
	      cellVert++ )
	  {
	    for ( int a=0; a<rDim; a++ )
	      cellCenters[*cell][a] += vertices(*cellVert,a);

	    nV++;
	  }

	for ( int a=0; a<rDim; a++ )
	  cellCenters[*cell][a] /= real(nV);

      }

    // if we have a 3D mapping, compute the face centers as well
    for ( face=umap.begin(UnstructuredMapping::Face); rDim==3 && face!=umap.end(UnstructuredMapping::Face); face++ )
      {
	for ( int a=0; a<rDim; a++ )
	  faceCenters[*face][a] = 0.;

	int nV=0;
	for ( faceVert  = umap.adjacency_begin(face, UnstructuredMapping::Vertex);
	      faceVert != umap.adjacency_end(face, UnstructuredMapping::Vertex);
	      faceVert++ )
	  {
	    for ( int a=0; a<rDim; a++ )
	      faceCenters[*face][a] += vertices(*faceVert,a);

	    nV++;
	  }

	for ( int a=0; a<rDim; a++ )
	  faceCenters[*face][a] /= real(nV);

      }
    
    if ( cellType==UnstructuredMapping::Face )
      {
	for ( edge  = umap.begin(UnstructuredMapping::Edge);
	      edge != umap.end(UnstructuredMapping::Edge);
	      edge++ )
	  {
	    // in 2D we connect the cell (face) center to the center of the edge to form the area normals
	    ArraySimpleFixed<real,2,1,1,1> edgeVertices[2], edgeCenter;
	    int v=0;

	    for ( int a=0; a<rDim; a++ )
	      edgeCenter[a] = 0;

	    edgeVert_end = umap.adjacency_end(edge,UnstructuredMapping::Vertex);
	    for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
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

	    if ( cellNorm )
	      {
		cellSurfAreaNormals(*edge,0,0,0) = edgeVertices[1][1]-edgeVertices[0][1];
		cellSurfAreaNormals(*edge,0,0,1) = edgeVertices[0][0]-edgeVertices[1][0];
	      }

	    // iterate through the adjacent Faces to compute the area normal and the edges volume contributions
	    for ( edgeCell  = umap.adjacency_begin(edge, cellType );
		  edgeCell != umap.adjacency_end(edge, cellType ) && ( vertVol || cellVol || edgeNorm );
		  edgeCell++ )
	      {
		// the "area normal" is the vector pointing in the + direction of the edge that is normal
		//   to the line connecting the cell center and the edge center

		// we multiply by the orientation to get the sign right
		if ( edgeNorm )
		  {
		    edgeAreaNormals(*edge,0,0,0) += edgeCell.orientation()*(cellCenters[*edgeCell][1] - edgeCenter[1]);
		    edgeAreaNormals(*edge,0,0,1) -= edgeCell.orientation()*(cellCenters[*edgeCell][0] - edgeCenter[0]);
		  }

		real area = edgeCell.orientation()*triangleArea2D(edgeVertices[0], edgeVertices[1], cellCenters[*edgeCell]);

		assert(area>0.);

		if ( cellVol )
		  cellVolumes(*edgeCell,0,0) += area;
		
		if ( !edgeNorm )
		  for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
			edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex) && vertVol;
			edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		    vertexVolumes(*edgeVert,0,0) += .5*area;
	      }
	    
	    for ( edgeCell  = umap.adjacency_begin(edge, cellType );
		  edgeCell != umap.adjacency_end(edge, cellType ) && vertVol && edgeNorm ;
		  edgeCell++ )
	      {
		for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
		      edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex);
		      edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		  {
		    vertexVolumes(*edgeVert,0,0) += .5*real(edgeVert.orientation())*edgeAreaNormals(*edge,0,0,0)*cellCenters[*edgeCell][0];
		  }
	      }
	  }
	
	// compute subcell information in an order that looks like an iteration over face-edge
	int nSubC = 0;
	for ( cell=umap.begin(UnstructuredMapping::Face);
	      cell!=umap.end(UnstructuredMapping::Face) && ( subCellNorms || subCellVols );
	      cell++ )
	  {
	    cellEdge_end = umap.adjacency_end(cell, UnstructuredMapping::Edge);
	    for ( cellEdge = umap.adjacency_begin(cell, UnstructuredMapping::Edge);
		  cellEdge!=cellEdge_end;
		  cellEdge++ )
	      {
		ArraySimpleFixed<real,2,1,1,1> edgeVertices[2], edgeCenter;
		int v=0;
		
		for ( int a=0; a<rDim; a++ )
		  edgeCenter[a] = 0;
		
		for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
		      edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex);
		      edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		  {
		    for ( int a=0; a<rDim; a++ )
		      {
			edgeVertices[v][a] = vertices(*edgeVert,a);
			edgeCenter[a] += edgeVertices[v][a]/2.;
		      }
		    v++;
		  }

		real area = cellEdge.orientation()*triangleArea2D(edgeVertices[0], edgeVertices[1], cellCenters[*edgeCell]);

		if ( subCellNorms )
		  {
		    subCellNormals(nSubC,0) = (cellCenters[*cell][1] - edgeCenter[1]);
		    subCellNormals(nSubC,1) = -(cellCenters[*cell][0] - edgeCenter[0]);
		  }

		if ( subCellVols )
		  subCellVolumes(nSubC) = area;

		nSubC++;
	      }
	  }
      }
    else
      {
	for ( edge  = umap.begin(UnstructuredMapping::Edge);
	      edge != umap.end(UnstructuredMapping::Edge);
	      edge++ )
	  {
	    ArraySimpleFixed<real,3,1,1,1> edgeVertices[2], edgeCenter;
	    int v=0;

	    for ( int a=0; a<rDim; a++ )
	      edgeCenter[a] = 0;

	    for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
		  edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex);
		  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
	      {
		for ( int a=0; a<rDim; a++ )
		  {
		    edgeVertices[v][a] = vertices(*edgeVert,a);
		    edgeCenter[a] += edgeVertices[v][a]/2.;
		  }
		v++;
	      }

	    // iterate through the edges faces to compute volumes and areas
	    for ( edgeFace  = umap.adjacency_begin(edge,UnstructuredMapping::Face);
		  edgeFace != umap.adjacency_end(edge,UnstructuredMapping::Face);
		  edgeFace++ )
	      {

		ArraySimpleFixed<real,3,1,1,1> faceNorm = areaNormal3D(edgeVertices[0],edgeVertices[1],faceCenters[*edgeFace]);
		for ( int a=0; a<rDim && cellNorm; a++ )
		  cellSurfAreaNormals(*edgeFace,0,0,a) += edgeFace.orientation()*faceNorm[a];

		for ( faceCell  = umap.adjacency_begin(edgeFace, cellType);
		      faceCell != umap.adjacency_end(edgeFace, cellType);
		      faceCell++ ) // we could optimize this loop by getting the face->cell adjacency directly
		  {
		    // multiply by the orientations to get the sign right
		    ArraySimpleFixed<real,3,1,1,1> areaNormal = areaNormal3D(edgeCenter, cellCenters[*faceCell], faceCenters[*edgeFace]);

		    real dirCheck = 0.;
		    for ( int a=0; a<rDim && edgeNorm; a++ )
		      {
			areaNormal[a] *= faceCell.orientation()*edgeFace.orientation();
			edgeAreaNormals(*edge,0,0,a) += areaNormal[a];
			dirCheck+= areaNormal[a]*(edgeVertices[1][a]-edgeVertices[0][a]);
		      }

		    assert( !edgeNorm || (edgeNorm && dirCheck >0. ) );

		    real wedgeVol = 
		      edgeFace.orientation()*faceCell.orientation()*
		      tetVolume(edgeVertices[0],faceCenters[*edgeFace],edgeVertices[1],cellCenters[*faceCell]);

		    assert(wedgeVol>0.);

		    if ( cellVol )
		      cellVolumes(*faceCell,0,0) += wedgeVol;

		    for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
			  edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex) && vertVol;
			  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		      vertexVolumes(*edgeVert,0,0) += .5*wedgeVol;
		    
		  }
	      }
	  }

	int nSubC=0;
	for ( cell=umap.begin(UnstructuredMapping::Region);
	      cell!=umap.end(UnstructuredMapping::Region) && ( subCellNorms || subCellVols );
	      cell++ )
	  {
	    cellFace_end = umap.adjacency_end(cell,UnstructuredMapping::Face);

	    for ( cellFace = umap.adjacency_begin(cell,UnstructuredMapping::Face);
		  cellFace !=cellFace_end;
		  cellFace++ )
	      {

		faceEdge_end = umap.adjacency_end(cellFace,UnstructuredMapping::Edge);
		for ( faceEdge=umap.adjacency_begin(cellFace,UnstructuredMapping::Edge);
		      faceEdge!=faceEdge_end;
		      faceEdge++ )
		  {
		    ArraySimpleFixed<real,3,1,1,1> edgeVertices[2], edgeCenter;
		    int v=0;
		    
		    for ( int a=0; a<rDim; a++ )
		      edgeCenter[a] = 0;
		    
		    edgeVert_end = umap.adjacency_end(edge,UnstructuredMapping::Vertex);
		    for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
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
		    ArraySimpleFixed<real,3,1,1,1> areaNormal =  areaNormal3D(edgeCenter, cellCenters[*cellFace], faceCenters[*faceEdge]);


		    for ( int a=0; a<rDim && edgeNorm; a++ )
		      areaNormal[a] *= cellFace.orientation()*faceEdge.orientation();

		    
		    if ( subCellNorms )
		      for ( int a=0; a<rDim && edgeNorm; a++ )
			subCellNormals(nSubC,a) = areaNormal[a];

		    if ( subCellVols )
		      {
			real wedgeVol =  
			  faceEdge.orientation()*cellFace.orientation()*
			  tetVolume(edgeVertices[0],faceCenters[*faceEdge],edgeVertices[1],cellCenters[*cellFace]);
			
			subCellVolumes(nSubC) = wedgeVol;
		      }
		  }
	      }
		
	  }
      }
  }
  
}

//\begin{>>GridInclude.tex}{\subsubsection{computeGeometryForUnstructuredMapping}}
Integer MappedGridData::
computeUnstructuredGeometry(const Integer& what,
			    const Integer& how) 
// ==================================================================================
//  /Description:
//    Determine geometry arrays for an unstructured grid.
//\end{GridInclude.tex} 
// ==================================================================================
{
  Integer returnValue = 0;
  const real realSmall = REAL_MIN*100.;
  
  Range d0 = numberOfDimensions, one = 1, all;

  UnstructuredMapping & map = (UnstructuredMapping &) mapping.getMapping();
  int domainDimension = map.getDomainDimension();

  UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(domainDimension);

  const realArray &vertices = map.getNodes();

  if( what & THEvertex )
  {
    // *vertex.reference(map.getNodes());
    realArray & x = *vertex;

    Range R(map.size(UnstructuredMapping::Vertex));
    Range A(map.getNodes().getLength(1));
    x.redim(R, A);
    x(R,A)=map.getNodes()(R,A);   // make a copy for now.
    x.reshape(x.getLength(0),1,1,x.getLength(1)); // reshape vertices to look like structured case.

    computedGeometry |= THEvertex;
  }
  if( what & THEcenter ) // *wdh* 020515 && !isAllVertexCentered )
  {
    realArray & x = *center;
	
    if ( isAllVertexCentered )
      {
	if( vertex!=NULL )
	  {
	    x.reference(*vertex);
	  }
	else
	  {
	    x.redim(map.getNodes());
	    x=map.getNodes();   // make a copy for now.
	    x.reshape(x.getLength(0),1,1,x.getLength(1));
	  }
      }
    else
      {
	// compute "cell" (Edge|Face|Region) centers as average of defining vertices
	UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(map.getDomainDimension());
	
	x.redim(map.size(cellType),1,1,map.getRangeDimension());

	UnstructuredGeometry::computeCellCenters(map,x);
      }

    computedGeometry |= THEcenter;
  }
  
  if( what & THEcorner ) 
  {
    assert( corner!=NULL );
    realArray & xc = *corner;

    if ( isAllVertexCentered )
      UnstructuredGeometry::computeCellCenters(map,xc);
    else
      {
	if( vertex!=NULL )
	  {
	    xc.reference(*vertex);
	  }
	else
	  {
	    xc.redim(map.getNodes());
	    xc=map.getNodes();   // make a copy for now.
	    xc.reshape(xc.dimension(0),1,1,xc.dimension(1));
	  }
      }
    
    computedGeometry |= THEcorner;
  }

  map.buildEntity(cellType);
  map.buildEntity(UnstructuredMapping::EntityTypeEnum(cellType-1));
  map.buildEntity(UnstructuredMapping::Edge);

  if (what & THEfaceNormal)
    {
      if ( !faceNormal ) 
	faceNormal = ::new RealMappedGridFunction;
      if ( isAllVertexCentered ) 
	faceNormal->redim(map.size(UnstructuredMapping::Edge),1,1,d0);
      else
	faceNormal->redim(map.size(UnstructuredMapping::EntityTypeEnum( int(cellType)-1 )),1,1,d0);
    }

  if (what & THEfaceArea)
    {
      if (!faceArea) 
	faceArea = ::new RealMappedGridFunction;
      if ( isAllVertexCentered ) 
	faceArea->redim(map.size(UnstructuredMapping::Edge),1,1);
      else
	faceArea->redim(map.size(UnstructuredMapping::EntityTypeEnum( int(cellType)-1 )),1,1);
    }

  if (what & THEcellVolume) 
    {
      if (!cellVolume) cellVolume = ::new RealMappedGridFunction;
      if ( isAllVertexCentered )
	cellVolume->redim(map.size(UnstructuredMapping::Vertex),1,1);
      else
	cellVolume->redim(map.size(cellType),1,1);
    }

  if ( what & THEcenterNormal )
    {
      if ( !centerNormal ) 
	centerNormal = ::new RealMappedGridFunction;
      if ( isAllVertexCentered ) 
	centerNormal->redim(map.size(UnstructuredMapping::EntityTypeEnum( int(cellType)-1 )),1,1,d0);
      else
	centerNormal->redim(map.size(UnstructuredMapping::Edge),1,1,d0);
    }

  if (what & THEcenterArea)
    {
      if (!centerArea) 
	centerArea = ::new RealMappedGridFunction;
      if ( isAllVertexCentered ) 
	centerArea->redim(map.size(UnstructuredMapping::EntityTypeEnum( int(cellType)-1 )),1,1);
      else
	centerArea->redim(map.size(UnstructuredMapping::Edge),1,1);
    }

  realArray *cellNorm=0, *edgeNorm=0, *vertVol=0, *cellVol=0;

  if ( isAllVertexCentered )
    {
      if ( what&THEcellVolume) vertVol = cellVolume;
      if ( what&THEfaceNormal) edgeNorm = faceNormal;
      //      if ( what & THEfaceArea )
      //	(*faceArea).redim(map.capacity(UnstructuredMapping::Edge),1,1);
      if ( what&THEcenterNormal )
	cellNorm = centerNormal;
    }
  else
    {
      if ( what&THEcellVolume) cellVol = cellVolume;
      if ( what&THEfaceNormal) cellNorm = faceNormal;
//      if ( what & THEfaceArea )
//	(*faceArea).redim(map.capacity(UnstructuredMapping::EntityTypeEnum(domainDimension-1)),1,1);

      if ( what&THEcenterNormal )
	edgeNorm = centerNormal;
    }

  UnstructuredGeometry::computeGeometry(map, cellNorm, edgeNorm, vertVol, cellVol, 0, 0);

  if ( (what&THEfaceNormal) || (what &THEfaceArea) )
    {
      realArray &fn = isAllVertexCentered ? *edgeNorm : *cellNorm;

      for ( int f=0; f<fn.getLength(0); f++ )
	{
	  real mag = sqrt(sum(pow(fn(f,0,0,d0),2)));
	  
	  if ( fabs(mag)>100*REAL_MIN ) 
	    fn(f,0,0,d0) /= mag;

	  if ( what & THEfaceArea )
	    (*faceArea)(f,0,0) = mag;
	}

    }

  if ( (what&THEcenterNormal) || (what &THEcenterArea) )
    {
      realArray &fn = isAllVertexCentered ? *cellNorm : *edgeNorm;

      for ( int f=0; f<fn.getLength(0); f++ )
	{
	  real mag = sqrt(sum(pow(fn(f,0,0,d0),2)));
	  
	  if ( fabs(mag)>100*REAL_MIN ) 
	    fn(f,0,0,d0) /= mag;
	  
	  if ( what & THEcenterArea )
	    (*centerArea)(f,0,0) = mag;
	}
    }

  if ( what & THEcellVolume )
    computedGeometry |= THEcellVolume;
  if ( what & THEfaceNormal )
    computedGeometry |= THEfaceNormal;
  if ( what & THEfaceArea ) 
    computedGeometry |= THEfaceArea;
  if ( what & THEcenterNormal )
    computedGeometry |= THEcenterNormal;
  if ( what & THEcenterArea   )
    computedGeometry |= THEcenterArea;

//   if ( what & THEminMaxEdgeLength )
//     {
//       UnstructuredMappingIterator edge, edgesEnd;
//       map.buildEntity(UnstructuredMapping::Edge); // make sure these are built
//       edgesEnd = map.end(UnstructuredMapping::Edge);

//       const intArray &edges = map.getEntities(UnstructuredMapping::Edge);

//       maximumEdgeLength = 1;
//       minimumEdgeLength = 1;
//       maximumEdgeLength(0) = -REAL_MAX;
//       minimumEdgeLength(0) =  REAL_MAX;
//       for ( edge=map.begin(UnstructuredMapping::Edge); edge!=edgesEnd; edge++ )
// 	{
// 	  int e = *edge;
// 	  real len = sum(pow(vertices(edges(e,1),d0)-vertices(edges(e,0),d0),2));
// 	  if ( len>maximumEdgeLength(0) )
// 	    maximumEdgeLength(0) = len;
// 	  if ( len<minimumEdgeLength(0) )
// 	    minimumEdgeLength(0) = len;
// 	}

//       maximumEdgeLength(0) = sqrt(maximumEdgeLength(0));
//       minimumEdgeLength(0) = sqrt(minimumEdgeLength(0));

//       computedGeometry |= THEminMaxEdgeLength;
//     }

  if ( what &THEboundingBox )
    {
      boundingBox = 0;
      boundingBox(0,d0) = REAL_MAX;
      boundingBox(1,d0) = -REAL_MAX;

      // there may be holes in the data structure (possibly with garbage?) so iterate through the vertices skipping holes
      UnstructuredMappingIterator vert, vertsEnd;
      vertsEnd = map.end(UnstructuredMapping::Vertex);

      const int rangeDim=rangeDimension();
      for ( vert=map.begin(UnstructuredMapping::Vertex); vert!=vertsEnd; vert++ )
	{
	  int v=*vert;
          for( int axis=0; axis<rangeDim; axis++ ) // *wdh* 030915
	  {
	    boundingBox(0,axis) = min(boundingBox(0,axis),vertices(v,axis));
	    boundingBox(1,axis) = max(boundingBox(1,axis),vertices(v,axis));
	  }
	  
	}
       
      computedGeometry |= THEboundingBox;
    }

  if ( what & THEmask )
    {
      // mask out all ghost boundaries (not really sure if we should do this) !!XXX NO WE SHOULDNT, changed 050120
      UnstructuredMapping::EntityTypeEnum cellType = isAllVertexCentered ? UnstructuredMapping::Vertex : 
	UnstructuredMapping::EntityTypeEnum( map.getDomainDimension() );

      if (mask->getLength(0)==0)
	{
	  mask->redim(map.size(cellType),1,1);
	  (*mask) = MappedGrid::ISdiscretizationPoint;
	}
      else if ( mask->getLength(0)<map.size(cellType) )
	{
	  int oldM = mask->getLength(0);
	  mask->resize(map.size(cellType),1,1);
	  Range R(oldM, mask->getLength(0)-1);
	  (*mask)(R,0,0) = MappedGrid::ISdiscretizationPoint;
	}

#if 0
      std::string gTag = "Ghost "+string(UnstructuredMapping::EntityTypeStrings[cellType].c_str());
      UnstructuredMapping::tag_entity_iterator git;
      for ( git=map.tag_entity_begin(gTag); git!=map.tag_entity_end(gTag); git++ )
	{
	  (*mask)(git->e,0,0) = 0;
	}

      computedGeometry |= THEmask;
      cout<<"computed unstructured grid mask from ghost boundaries!"<<endl;
#endif

    }

  return returnValue;
}
