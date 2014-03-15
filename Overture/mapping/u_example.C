#define OV_DEBUG
#define BOUNDS_CHECK
#include <string>
#include <iostream>

#include "UnstructuredMapping.h"
#include "Geom.h"
#include "OGPulseFunction.h"

namespace {

  void tagBoundaryEntities(UnstructuredMapping &umap);

  void computeGeometry(UnstructuredMapping &umap, 
		       realArray &faceAreaNormals, realArray &edgeAreaNormals, 
		       realArray &vertexVolumes, realArray &cellVolumes);

  void computeGradient(UnstructuredMapping &umap, 
		       UnstructuredMapping::EntityTypeEnum centering, UnstructuredMapping::EntityTypeEnum surfaceCentering,
		       UnstructuredMapping::EntityTypeEnum surfaceCentering,
		       realArray &u, realArray &surfaceNormals, realArray &volumes, 
		       realArray &gradu);

  void performCalculation(UnstructuredMapping &umap, 
			  realArray &faceAreaNormals, realArray &edgeAreaNormals, 
			  realArray &vertexVolumes, realArray &cellVolumes);
}

int main(int argc, char *argv[])
{
  UnstructuredMapping umap;

  if ( argc!=2 )
    exit(1);
  
  umap.get(argv[1]);
  
  UnstructuredMappingIterator face;
  for ( face = umap.begin(UnstructuredMapping::Face);
        face!=umap.end(UnstructuredMapping::Face);
	face++ )
     cout<<"Here is a Face with index "<<*face<<endl;

   
  for ( face =  umap.begin(UnstructuredMapping::Face);
        face != umap.end(UnstructuredMapping::Face);
	face++ )
    {
      cout<<"Face "<<*face<<" has vertices : ";

      UnstructuredMappingAdjacencyIterator faceVert;
      for ( faceVert=umap.adjacency_begin(face, UnstructuredMapping::Vertex);
	    faceVert!=umap.adjacency_end(face, UnstructuredMapping::Vertex);
	    faceVert++ )
	cout<<*faceVert<<" ";
      
      cout<<endl;
    }

  tagBoundaryEntities(umap);

  realArray cellSurfAreaNormals, edgeAreaNormals, vertexVolumes, cellVolumes;

  computeGeometry(umap, cellSurfAreaNormals, edgeAreaNormals, vertexVolumes, cellVolumes);

  performCalculation(umap, cellSurfAreaNormals, edgeAreaNormals, vertexVolumes, cellVolumes);

}

namespace {
  
  void tagBoundaryEntities(UnstructuredMapping &umap)
  {
    // determine the highest dimensional entity that bounds the mesh
    UnstructuredMapping::EntityTypeEnum cellBdyType = umap.getRangeDimension()==2 ? 
      UnstructuredMapping::Edge : UnstructuredMapping::Face;

    // the next higher entity we will all the ``cell''
    UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(cellBdyType + 1);

    UnstructuredMappingIterator e_iter;
    UnstructuredMappingAdjacencyIterator cellIter, vertIter;

    // iterate through the bounding entities and determine if they are on the boundary
    for ( e_iter=umap.begin(cellBdyType); e_iter!=umap.end(cellBdyType); e_iter++)
      {
        // an e_iter is on the boundary if it only has one neighboring cell
        int nAdj=0;
        for ( cellIter=umap.adjacency_begin(e_iter, cellType); 
              cellIter!=umap.adjacency_end(e_iter, cellType); cellIter++ )
           nAdj++;
	
        if ( nAdj==1 ) 
          {
	    if ( !umap.hasTag(cellBdyType,*e_iter,"boundary entity") )
	      umap.addTag(cellBdyType,*e_iter,"boundary entity",((void *)*e_iter));
	    
	    if ( !umap.hasTag(cellType,*umap.adjacency_begin(e_iter, cellType),"boundary entity") )
	      umap.addTag(cellType,*umap.adjacency_begin(e_iter, cellType),
			  "boundary entity",((void *)*umap.adjacency_begin(e_iter, cellType)));

            // we are on a boundary, tag the vertices as such
            for ( vertIter=umap.adjacency_begin(e_iter, UnstructuredMapping::Vertex);
                  vertIter!=umap.adjacency_end(e_iter, UnstructuredMapping::Vertex);
                  vertIter++ )
               if ( !umap.hasTag(UnstructuredMapping::Vertex, *vertIter, "boundary entity") )
                 umap.addTag(UnstructuredMapping::Vertex, *vertIter, "boundary entity", ((void *)*vertIter));
          }

      }
  }

  void computeGeometry(UnstructuredMapping &umap, 
		       realArray &cellSurfAreaNormals, realArray &edgeAreaNormals, 
		       realArray &vertexVolumes, realArray &cellVolumes)
  {
    UnstructuredMapping::EntityTypeEnum cellType = 
      umap.getRangeDimension()==2 ? UnstructuredMapping::Face : UnstructuredMapping::Region;

    int rDim = umap.getRangeDimension();

    const realArray &vertices = umap.getNodes(); // legacy method

    cellSurfAreaNormals.redim( umap.size(UnstructuredMapping::EntityTypeEnum( cellType-1)), rDim);

    umap.buildEntity(UnstructuredMapping::Edge); // make sure edges are built!

    edgeAreaNormals.redim(umap.size(UnstructuredMapping::Edge), rDim);
    vertexVolumes.redim(umap.size(UnstructuredMapping::Vertex));
    cellVolumes.redim(umap.size(cellType));

    ArraySimple< ArraySimpleFixed<real,3,1,1,1> >  cellCenters(umap.size(cellType));

    ArraySimple< ArraySimpleFixed<real,3,1,1,1> > faceCenters;
    if ( rDim==3 )
      faceCenters.resize(umap.size(UnstructuredMapping::Face));

    cellSurfAreaNormals = 0;
    edgeAreaNormals = 0;
    vertexVolumes = 0;
    cellVolumes = 0;

    UnstructuredMappingIterator cell,region,face,edge;
    UnstructuredMappingAdjacencyIterator cellVert,faceVert,edgeFace,faceCell,edgeVert,edgeCell;

    // first compute the cell centers (used in later computations)
    for ( cell=umap.begin(cellType); cell!=umap.end(cellType); cell++ )
      {
	for ( int a=0; a<rDim; a++ )
	  cellCenters[*cell][a] = 0.;

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

	    cellSurfAreaNormals(*edge,0) = edgeVertices[1][1]-edgeVertices[0][1];
	    cellSurfAreaNormals(*edge,1) = edgeVertices[0][0]-edgeVertices[1][0];

	    // iterate through the adjacent Faces to compute the area normal and the edges volume contributions
	    for ( edgeCell  = umap.adjacency_begin(edge, cellType );
		  edgeCell != umap.adjacency_end(edge, cellType );
		  edgeCell++ )
	      {
		// the "area normal" is the vector pointing in the + direction of the edge that is normal
		//   to the line connecting the cell center and the edge center

		// we multiply by the orientation to get the sign right
		edgeAreaNormals(*edge,0) += edgeCell.orientation()*(cellCenters[*edgeCell][1] - edgeCenter[1]);
		edgeAreaNormals(*edge,1) -= edgeCell.orientation()*(cellCenters[*edgeCell][0] - edgeCenter[0]);

		real area = edgeCell.orientation()*triangleArea2D(edgeVertices[0], edgeVertices[1], cellCenters[*edgeCell]);

		cellVolumes(*edgeCell) += area;

		for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
		      edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex);
		      edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		  vertexVolumes(*edgeVert) += .5*area;
	      }
	  }
      }
    else
      {
	for ( edge  = umap.begin(UnstructuredMapping::Edge);
	      edge != umap.end(UnstructuredMapping::Edge);
	      edge++ )
	  {
	    // in 2D we connect the cell (face) center to the center of the edge to form the area normals
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
		for ( int a=0; a<rDim; a++ )
		  cellSurfAreaNormals(*edgeFace,a) += edgeFace.orientation()*faceNorm[a];

		for ( faceCell  = umap.adjacency_begin(edgeFace, cellType);
		      faceCell != umap.adjacency_end(edgeFace, cellType);
		      faceCell++ ) // we could optimize this loop by getting the face->cell adjacency directly
		  {
		    // multiply by the orientations to get the sign right
		    ArraySimpleFixed<real,3,1,1,1> areaNormal = areaNormal3D(edgeCenter, cellCenters[*faceCell], faceCenters[*edgeFace]);

		    for ( int a=0; a<rDim; a++ )
		      edgeAreaNormals(*edge,a) += faceCell.orientation()*edgeFace.orientation()*areaNormal[a];

		    real wedgeVol = 
		      edgeFace.orientation()*faceCell.orientation()*
		      tetVolume(edgeVertices[0],faceCenters[*edgeFace],edgeVertices[1],cellCenters[*faceCell]);

		    cellVolumes(*faceCell) += wedgeVol;

		    for ( edgeVert  = umap.adjacency_begin(edge,UnstructuredMapping::Vertex);
			  edgeVert != umap.adjacency_end(edge,UnstructuredMapping::Vertex);
			  edgeVert++ ) // we could optimize this loop by getting the edge entities directly
		      vertexVolumes(*edgeVert) += .5*wedgeVol;
		    
		  }
	      }
	  }
      }
  }

  void computeGradient(UnstructuredMapping &umap, 
		       UnstructuredMapping::EntityTypeEnum centering, UnstructuredMapping::EntityTypeEnum surfaceCentering,
		       UnstructuredMapping::EntityTypeEnum gradientCentering,
		       realArray &u, realArray &surfaceNormals, realArray &volumes, 
		       realArray &gradu)
  {

    UnstructuredMappingIterator surf;
    UnstructuredMappingAdjacencyIterator u_cell, g_cell;

    gradu.redim(umap.size(gradientCentering), umap.getRangeDimension());
    gradu = 0;

    for ( surf  = umap.begin(surfaceCentering);
	  surf != umap.end(surfaceCentering);
	  surf++ )
      {
	int nC=0;
	real uOnSurf=0;
	for ( u_cell  = umap.adjacency_begin(surf, centering);
	      u_cell != umap.adjacency_end(surf, centering);
	      u_cell++ )
	  {
	    uOnSurf += u(*u_cell);
	    nC++;
	  }

	uOnSurf /= real(nC); 

	for ( g_cell  = umap.adjacency_begin(surf, gradientCentering);
	      g_cell != umap.adjacency_end(surf, gradientCentering);
	      g_cell++ )
	  for ( int a=0; a<umap.getRangeDimension(); a++ )
	    gradu(*g_cell,a) += g_cell.orientation()*surfaceNormals(*surf,a)*uOnSurf/volumes(*g_cell);
      }
  }

  void performCalculation(UnstructuredMapping &umap, 
			  realArray &cellSurfNormals, realArray &edgeAreaNormals, 
			  realArray &vertexVolumes, realArray &cellVolumes)
  {
    UnstructuredMapping::EntityTypeEnum cellType = umap.getRangeDimension()==2 ? 
      UnstructuredMapping::Face : UnstructuredMapping::Region;
    
    realArray vertexCenteredScalar( umap.size(UnstructuredMapping::Vertex) );
    realArray cellCenteredScalar( umap.size(cellType) );
    
    realArray vertexCenteredGradient, cellCenteredGradient, cellCenteredGradientFromNodeU;
    
    OGPulseFunction u_exact;

    realArray &vertices = umap.getNodes(); // relic method, needs to be changed

    UnstructuredMappingIterator vert, cell;
    UnstructuredMappingAdjacencyIterator cellVert;

    for ( vert  = umap.begin(UnstructuredMapping::Vertex);
	  vert != umap.end(UnstructuredMapping::Vertex);
	  vert++ )
      {
	if ( umap.getRangeDimension()==2 )
	  vertexCenteredScalar(*vert) = u_exact( vertices(*vert,0), vertices(*vert,1), 0.0 );
	else
	  vertexCenteredScalar(*vert) = u_exact( vertices(*vert,0), vertices(*vert,1), vertices(*vert,2) );
      }

    for ( cell  = umap.begin(cellType);
	  cell != umap.end(cellType);
	  cell++ )
      {
	ArraySimpleFixed<real,3,1,1,1> center;
	center = 0;

	int nV=0;
	for ( cellVert  = umap.adjacency_begin(cell, UnstructuredMapping::Vertex);
	      cellVert != umap.adjacency_end(cell, UnstructuredMapping::Vertex);
	      cellVert++ )
	  {
	    for ( int a=0; a<umap.getRangeDimension(); a++ )
	      center[a] += vertices(*cellVert,a);

	    nV++;
	  }
	
	for ( int a=0; a<umap.getRangeDimension(); a++ )
	  center[a] /= real(nV);

	if ( umap.getRangeDimension()==2 )
	  cellCenteredScalar(*cell) = u_exact( center[0], center[1], 0.0 );
	else
	  cellCenteredScalar(*cell) = u_exact( center[0], center[1], center[2] );
      }

//     // vertex centered u and gradu
    computeGradient(umap, 
		    UnstructuredMapping::Vertex, UnstructuredMapping::Edge,
		    UnstructuredMapping::Vertex,
		    vertexCenteredScalar, edgeAreaNormals, vertexVolumes, 
		    vertexCenteredGradient);

    // cell centered u and gradu
    computeGradient(umap, 
		    cellType, UnstructuredMapping::EntityTypeEnum( cellType-1 ),
		    cellType,
		    cellCenteredScalar, cellSurfNormals, cellVolumes, 
		    cellCenteredGradient);

    // vertex centered u and cell centered gradu
    computeGradient(umap,
		    UnstructuredMapping::Vertex, UnstructuredMapping::EntityTypeEnum( cellType-1 ),
		    cellType,
		    vertexCenteredScalar, cellSurfNormals, cellVolumes,
		    cellCenteredGradientFromNodeU);

    ArraySimpleFixed<real, 3,1,1,1> vertNorms,cellNorms, cellFromUNorms;

    vertNorms = cellNorms = cellFromUNorms = 0;

    real tVertVol = 0;
    real tCellVol = 0;

    for ( vert  = umap.begin(UnstructuredMapping::Vertex);
	  vert != umap.end(UnstructuredMapping::Vertex);
	  vert++ )
      {
	if ( !umap.hasTag(UnstructuredMapping::Vertex, *vert,"boundary entity") )
	  {
	    ArraySimpleFixed<real,3,1,1,1> err;
	    err = 0;
	    if ( umap.getRangeDimension()==2 )
	      {
		err[0] = vertexCenteredGradient(*vert,0) - u_exact.x( vertices(*vert,0), vertices(*vert,1), 0.0 );
		err[1] = vertexCenteredGradient(*vert,1) - u_exact.y( vertices(*vert,0), vertices(*vert,1), 0.0 );
	      }
	    else
	      {
		err[0] = vertexCenteredGradient(*vert,0) - u_exact.x( vertices(*vert,0), vertices(*vert,1), vertices(*vert,2) );
		err[1] = vertexCenteredGradient(*vert,1) - u_exact.y( vertices(*vert,0), vertices(*vert,1), vertices(*vert,2) );
		err[2] = vertexCenteredGradient(*vert,2) - u_exact.z( vertices(*vert,0), vertices(*vert,1), vertices(*vert,2) );
	      }
	    real errmag2 = ASmag2(err);
	    vertNorms[0] = max(sqrt(errmag2),vertNorms[0]);
	    vertNorms[1] += sqrt(errmag2)*vertexVolumes(*vert);
	    vertNorms[2] += errmag2*vertexVolumes(*vert);
	    
	    tVertVol += vertexVolumes(*vert);
	  }
      }

    for ( cell = umap.begin(cellType);
	  cell != umap.end(cellType);
	  cell++ )
      {

	if  ( !umap.hasTag(cellType,*cell,"boundary entity") )
	  {
	    ArraySimpleFixed<real,3,1,1,1> err, err2;
	    err = err2 = 0;

	    ArraySimpleFixed<real,3,1,1,1> center;
	    center = 0;
	    
	    int nV=0;
	    for ( cellVert  = umap.adjacency_begin(cell, UnstructuredMapping::Vertex);
		  cellVert != umap.adjacency_end(cell, UnstructuredMapping::Vertex);
		  cellVert++ )
	      {
		for ( int a=0; a<umap.getRangeDimension(); a++ )
		  center[a] += vertices(*cellVert,a);
		
		nV++;
	      }

	    for ( int a=0; a<umap.getRangeDimension(); a++ )
	      center[a] /= real(nV);

	    if ( umap.getRangeDimension()==2 )
	      {
		err[0] = cellCenteredGradient(*cell,0) - u_exact.x( center[0], center[1], 0.0 );
		err[1] = cellCenteredGradient(*cell,1) - u_exact.y( center[0], center[1], 0.0 );

		err2[0] = cellCenteredGradientFromNodeU(*cell,0) - u_exact.x( center[0], center[1], 0.0 );
		err2[1] = cellCenteredGradientFromNodeU(*cell,1) - u_exact.y( center[0], center[1], 0.0 );

	      }
	    else
	      {
		err[0] = cellCenteredGradient(*cell,0) - u_exact.x( center[0], center[1], center[2] );
		err[1] = cellCenteredGradient(*cell,1) - u_exact.y( center[0], center[1], center[2] );
		err[2] = cellCenteredGradient(*cell,2) - u_exact.z( center[0], center[1], center[2] );

		err2[0] = cellCenteredGradientFromNodeU(*cell,0) - u_exact.x( center[0], center[1], center[2] );
		err2[1] = cellCenteredGradientFromNodeU(*cell,1) - u_exact.y( center[0], center[1], center[2] );
		err2[2] = cellCenteredGradientFromNodeU(*cell,2) - u_exact.z( center[0], center[1], center[2] );
	      }
	    
	    real errmag2 = ASmag2(err);
	    real err2mag2 = ASmag2(err2);

	    cellNorms[0] = max(sqrt(errmag2),cellNorms[0]);
	    cellNorms[1] += sqrt(errmag2)*cellVolumes(*cell);
	    cellNorms[2] += errmag2*cellVolumes(*cell);

	    cellFromUNorms[0] = max(sqrt(err2mag2),cellFromUNorms[0]);
	    cellFromUNorms[1] += sqrt(err2mag2)*cellVolumes(*cell);
	    cellFromUNorms[2] += err2mag2*cellVolumes(*cell);
	    
	    tCellVol += cellVolumes(*cell);
	  }
      }

    vertNorms[1] /= tVertVol;
    cellNorms[1] /= tCellVol;
    cellFromUNorms[1] /= tCellVol;

    vertNorms[2] = sqrt(vertNorms[2]/tVertVol);
    cellNorms[2] = sqrt(cellNorms[2]/tCellVol);
    cellFromUNorms[2] = sqrt(cellFromUNorms[2]/tCellVol);

    cout<<"total vert vol "<<tVertVol<<endl;
    cout<<"total cell vol "<<tCellVol<<endl;
    cout<<"Vertex Norms "<<vertNorms<<endl;
    cout<<"Cell Norms "<<cellNorms<<endl;
    cout<<"Cell From Node U Norms "<<cellFromUNorms<<endl;

  }

}
