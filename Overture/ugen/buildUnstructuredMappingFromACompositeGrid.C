// #define BOUNDS_CHECK

// Put this function here to decouple the UnstructuredMapping from the CompositeGrid class.

#include "UnstructuredMapping.h"
#include "CompositeGrid.h"

//\begin{>>UnstructuredMappingImp.tex}{\subsection{Constructor}}
void UnstructuredMapping::
buildFromACompositeGrid( CompositeGrid &cg )
//===========================================================================
// /Description: build an unstructured mapping from a composite grid
// /cg (input) : a composite grid that may or may not be a hybrid grid
// /Comments : The composite grid has no restrictions, it could be an overlapping
// grid or hybrid mesh.  In the case of an overlapping grid, the UnstructuredMapping
// essentially consists of overlapping sections and holes that have no connectivity
// information.  A hybrid mesh becomes one consistent UnstructuredMapping.
//\end{UnstructuredMappingImp.tex}
//===========================================================================
{

  int numberOfGrids = cg.numberOfComponentGrids();
  int numberOfDimensions = cg.numberOfDimensions();


  // set two basic fields of the class
  domainDimension = rangeDimension = numberOfDimensions;

  // initialize templates and constants.
  initMapping();

  cg.update(MappedGrid::THEmask );

  // get the hybrid connectivity information
  const CompositeGridHybridConnectivity & connectivity = cg.getHybridConnectivity();

  intArray *gridVertexMappings = new intArray[numberOfGrids];
  UnstructuredMapping *unstructuredMeshes = new UnstructuredMapping[numberOfGrids];
  intArray *globalVertexIDMapping = new intArray[numberOfGrids];
  intArray globalIndexMapping;
  intArray *gridVertexBC       = new intArray[numberOfGrids];

  int maxNumberOfVertices = 0;
  numberOfElements = 0;
  // common loop counters
  int g;
  globalIndexMapping = -1;

  // convert all the grids to UnstructuredMappings, keeping track of the mapping between the
  // original grid and the new UnstructuredMapping. Also tally the maximum number of vertices
  // we will need in the new UnstructuredMapping we are building.
  for ( g=0; g<numberOfGrids; g++ )
    {
      MappedGrid &mappedGrid = cg[g];
      intArray &mask = mappedGrid.mask();
      unstructuredMeshes[g].addGhostElements(false);
      gridVertexMappings[g] = unstructuredMeshes[g].buildFromAMapping(cg[g].mapping().getMapping(), mask);
      globalVertexIDMapping[g].redim(unstructuredMeshes[g].getNumberOfNodes());
      globalVertexIDMapping[g]=-1;
      maxNumberOfVertices += unstructuredMeshes[g].getNumberOfNodes();
      numberOfElements += unstructuredMeshes[g].getNumberOfElements();
      gridVertexBC[g].redim(unstructuredMeshes[g].getNumberOfNodes(),domainDimension);
      gridVertexBC[g]=INT_MAX;

      const intArray & gface = unstructuredMeshes[g].getFaces();
#if 0
      for ( int fb=0; fb<unstructuredMeshes[g].getNumberOfBoundaryFaces(); fb++ )
	{
	  int f = unstructuredMeshes[g].getBoundaryFace(fb);
	  int bcnum = unstructuredMeshes[g].getBoundaryFaceTag(fb);
	  for ( int fv=0; fv<unstructuredMeshes[g].getNumberOfNodesThisFace(f); fv++ )
	    {
	      int nn=0;
	      while ( nn<domainDimension && 
		      gridVertexBC[g](gface(f,fv),nn)!=INT_MAX &&
		      gridVertexBC[g](gface(f,fv),nn)!=bcnum ) 
		{
		  nn++;
		}
	      if ( nn<domainDimension )
		gridVertexBC[g](gface(f,fv),nn) = bcnum;
	    }
	}
#endif
    }

  node.redim(maxNumberOfVertices, numberOfDimensions);
  element.redim(numberOfElements, maxNumberOfNodesPerElement);
  tags.redim(numberOfElements);
  globalIndexMapping.redim(maxNumberOfVertices,2);
  globalIndexMapping = -1;

  tags = -1;
  element = -1;
  Range AXES(0,numberOfDimensions-1);
  int globalVertexIDCounter = 0;
  // create the global vertexIDs for all the shared vertices ( using information obtained from the
  // CompositeGrid's hybridConnectivity.
  const intArray &uVertex2GridIndex = connectivity.getUVertex2GridIndex();
  int unstructuredGrid = connectivity.getUnstructuredGridIndex();
  
  for ( int v=0; v<uVertex2GridIndex.getLength(0); v++ )
    {
      int setGrid = uVertex2GridIndex(v,0);
      int i1 = uVertex2GridIndex(v,1);
      int i2 = uVertex2GridIndex(v,2);
      int i3 = uVertex2GridIndex(v,3);
      int setVertex = gridVertexMappings[setGrid](i1,i2,i3);
      globalVertexIDMapping[unstructuredGrid](v) = globalVertexIDCounter;
      globalVertexIDMapping[setGrid](setVertex) = globalVertexIDCounter;
      globalIndexMapping(globalVertexIDCounter,0) = unstructuredGrid;
      globalIndexMapping(globalVertexIDCounter,1) = v;
      
      node(globalVertexIDCounter, AXES) = unstructuredMeshes[unstructuredGrid].getNodes()(v, AXES);
      globalVertexIDCounter++;
      
    }

  // now loop through all the grids setting the globalVertexID for each vertex not already set
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  for ( g=0; g<numberOfGrids; g++ )
    {
      ::getIndex(cg[g].gridIndexRange(), I1,I2,I3);

      const realArray &uNodes = unstructuredMeshes[g].getNodes();

      int i1,i2,i3;
      for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	{
	  for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		{
		  int vertex;vertex = gridVertexMappings[g](i1,i2,i3);
		  if ( g==2 && ( vertex==1 || vertex==0 || vertex==90 || vertex==51) ){
		    cout<<"grid 2, vertex "<<vertex<<", isghost = "<<unstructuredMeshes[g].isGhost(UnstructuredMapping::Vertex,vertex)<<endl;
		  }
		  if ( ((vertex = gridVertexMappings[g](i1,i2,i3)) != -1) &&
		       !unstructuredMeshes[g].isGhost(UnstructuredMapping::Vertex,vertex) )
		    { // if the vertex has not been blanked out give it a global id
		      if ( globalVertexIDMapping[g](vertex) == -1 )
			{ // but set a new id only if the vertex has not already been set
			  globalVertexIDMapping[g](vertex) = globalVertexIDCounter;
			  globalIndexMapping(globalVertexIDCounter,0) = g;
			  globalIndexMapping(globalVertexIDCounter,1) = vertex;
			  node(globalVertexIDCounter, AXES) = uNodes(vertex, AXES);
			  globalVertexIDCounter++;
			}
		    }
		} // i1
	    } // i2
	} // i3
      
    } // g

  // with the vertex ID's set the elements in each unstructured mapping can be assembled into
  // the new one being constructed.
  int globalElementCounter = 0;
  for ( g=0; g<numberOfGrids; g++ )
    {
      UnstructuredMapping &uMap = unstructuredMeshes[g];
      const intArray &uElem = uMap.getElements();

      for ( int e=0; e<uMap.getNumberOfElements(); e++ )
	{
	  if ( !uMap.isGhost(UnstructuredMapping::EntityTypeEnum(uMap.getDomainDimension()),e) )
	    {
	      int nnode =0;
	      for ( int v=0; v<maxNumberOfNodesPerElement; v++ )
		{
		  if (uElem(e,v)>-1)
		    {
		      element(globalElementCounter, v) = globalVertexIDMapping[g](uElem(e, v));
		      nnode++;
		    }
		  else
		    element(globalElementCounter, v) = -1;
		  
		}
	      if ( globalElementCounter==1278 ){
		cout<<"grid = "<<g<<", element = "<<e<<endl;
		for ( int v=0; v<maxNumberOfNodesPerElement; v++ ) {
		  cout<<element(globalElementCounter, v)<<"  ";
		}
		cout<<endl;
		for ( int v=0; v<maxNumberOfNodesPerElement; v++ ) {
		  cout<<uElem(e,v)<<"  ";
		}
		cout<<"---";
		cout<<endl;
	      }
	      if ( !nnode ) cout<<"ERROR : element "<<e<<" of grid "<<g<<" has no nodes!"<<endl;
	      // tag the element by the grid number it came from
	      tags(globalElementCounter) = g;

	      globalElementCounter++;
	    }
// 	  if ( globalElementCounter>=3731 )
// 	    {
// 	      Range R(maxNumberOfNodesPerElement);
// 	      element(globalElementCounter-1,R).display("HERE IS THE BAD ONE");
// // 	      cout<<"grid is "<<g<<" ent is "<<e<<endl;
// // 	      cout<<"uElem is "<<uElem(e,0)<<" "<<uElem(e,1)<<" "<<uElem(e,2)<<" "<<uElem(e,3)<<endl;
// // 	      cout<<"and isGhost is "<<uMap.isGhost(UnstructuredMapping::EntityTypeEnum(uMap.getDomainDimension()),e)<<endl;
// 	    }

	}
    }

  numberOfNodes = globalVertexIDCounter;
  setGridDimensions( axis1,numberOfNodes );  
  
  node.resize(numberOfNodes, domainDimension);      
  numberOfElements = globalElementCounter;
  element.resize(globalElementCounter, maxNumberOfNodesPerElement);

  Range R(0,numberOfNodes-1);
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
  }
  
  buildConnectivityLists();

  // now figure out what the boundary conditions are
  for ( int fb=0; fb<getNumberOfBoundaryFaces(); fb++ )
    {
      int f=getBoundaryFace(fb);
      bool foundBC = false;
      for ( int testbcvert=0; testbcvert<domainDimension && !foundBC; testbcvert++ )
	{
	  int tbc = gridVertexBC[globalIndexMapping(face(f,0),0)](globalIndexMapping(face(f,0),1),testbcvert);
	  foundBC = true;
	  for ( int fn=1; foundBC && fn<getNumberOfNodesThisFace(f); fn++ )
	    {
	      foundBC = false;
	      for ( int tbcv=0; tbcv<domainDimension && !foundBC; tbcv++ )
		{
		  foundBC = 
		    tbc==gridVertexBC[globalIndexMapping(face(f,fn),0)](globalIndexMapping(face(f,fn),1),tbcv);
		}
	    }
	  if ( foundBC )
	    bdyFaceTags(fb) = tbc;
	}
    }

  delete [] gridVertexMappings;
  delete [] unstructuredMeshes;
  delete [] globalVertexIDMapping;
  delete [] gridVertexBC;
}

