//#define BOUNDS_CHECK
//#define OV_DEBUG

#include "UnstructuredMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "MappingProjectionParameters.h"
#include "arrayGetIndex.h"
#include "DataFormats.h"
#include "display.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"
#include "TriangleWrapper.h"
#include "DataPointMapping.h"
#include "GeometricADT3dInt.h"

#include "entityConnectivityBuilder.h"
#include "uns_templates.h"

#include "Geom.h"
#include "ArraySimple.h"

#include <float.h>
#include <algorithm>
#include <string>
using namespace std;

bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

int 
getLineFromFile( FILE *file, char s[], int lim);


/* ---- These were moved to initStaticMappingVariables.C since they need to be in the static library *wdh* 030825
/// an array usefull for diagnostics involving EntityTypeEnum
aString UnstructuredMapping::EntityTypeStrings[] = { "Vertex",
						     "Edge",
						     "Face",
						     "Region",
						     "Mesh" };		

aString UnstructuredMapping::ElementTypeStrings[] = { "triangle",
						      "quadrilateral",
						      "tetrahedron",
						      "pyramid",
						      "triPrism",
						      "septahedron",  
						      "hexahedron",
						      "other",
						      "boundary" };
	      
------------------*/

namespace {
  // utility routine --may not be here in the future...

  void tagEntitiesFromVerts(UnstructuredMapping &umap, UnstructuredMapping::EntityTypeEnum type)
  {
    if ( type==UnstructuredMapping::Vertex )
      return;

    UnstructuredMappingIterator eit, eit_end;
    UnstructuredMappingAdjacencyIterator vit, vit_end;

    string bvtag = "boundary "+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Vertex];
    string betag = "boundary "+UnstructuredMapping::EntityTypeStrings[type];
    string bndtag = "boundary "+UnstructuredMapping::EntityTypeStrings[umap.getDomainDimension()-1];
    int ng=0;
    int nb=0;
    eit_end = umap.end(type);
    for ( eit=umap.begin(type); eit!=eit_end; eit++ )
      {
	bool isGhost = false; // is a ghost if at least 1 vert is a ghost
	bool isBdy   = true; // is a bdy if all verts are bdy 

	vit_end = umap.adjacency_end(eit,UnstructuredMapping::Vertex);
	for ( vit=umap.adjacency_begin(eit,UnstructuredMapping::Vertex);
	      vit!=vit_end && !isGhost;
	      vit++ )
	  {
	    isBdy = isBdy && umap.hasTag(UnstructuredMapping::Vertex, *vit,bvtag);
	    isGhost = vit.isGhost();
	  }

	if ( isGhost )
	  {
	    umap.setAsGhost(type, *eit);
	    ng++;
	  }
	else if ( isBdy && int(type)!=(umap.getDomainDimension()-1) )
	  {
	    UnstructuredMappingAdjacencyIterator ai;
	    ai = umap.adjacency_begin(eit, UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()-1));
	    
	    if ( umap.hasTag(ai.getType(), *ai, bndtag) && !umap.hasTag(type,*eit,betag) )
	      {
		umap.addTag(type, *eit, betag, (void *)0);
		nb++;
	      }
	  }
	else if ( isBdy )
	  {
	    int nadj = 0;
	    vit_end = umap.adjacency_end(eit, UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()), true);
	    for ( vit=umap.adjacency_begin(eit, UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()),true);
		  vit!=vit_end;
		  vit++ ) nadj++; 

	    if ( nadj==1 && !umap.hasTag(type,*eit,betag) ) 
	      {
		umap.addTag(type, *eit, betag, (void *)0);
		nb++;
	      }
	    
	  }
      }
#if 0
    cout<<"NUMBER OF (V)"<<umap.tagPrefix(type,UnstructuredMapping::GhostEntity)<<" : "<<ng<<endl;
    cout<<"NUMBER OF (V)"<<betag<<" : "<<nb<<endl;
#endif
  }

  void tagBoundaryEntities(UnstructuredMapping &umap)
  {
    // determine the highest dimensional entity that bounds the mesh
    UnstructuredMapping::EntityTypeEnum cellBdyType = umap.getDomainDimension()==2 ? 
      UnstructuredMapping::Edge : UnstructuredMapping::Face;

    // the next higher entity we will all the ``cell''
    UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(cellBdyType + 1);
    
    UnstructuredMappingIterator e_iter,e_iter_end;
    UnstructuredMappingAdjacencyIterator cellIter, vertIter;
    
    // iterate through the bounding entities and determine if they are on the boundary
    string gtag = string("Ghost ")+UnstructuredMapping::EntityTypeStrings[cellBdyType].c_str();
    string gcell = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[cellType].c_str();

    e_iter_end = umap.end(cellBdyType);


    umap.buildConnectivity(cellBdyType,cellType);
    umap.buildConnectivity(cellBdyType,UnstructuredMapping::Edge);
    
    for ( e_iter=umap.begin(cellBdyType); e_iter!=e_iter_end; e_iter++)
      {
        // an e_iter is on the boundary if it only has one neighboring non-ghost cell
        int nAdj=0;
        for ( cellIter=umap.adjacency_begin(e_iter, cellType); 
              cellIter!=umap.adjacency_end(e_iter, cellType); cellIter++ )
	  if ( !cellIter.isGhost() /*umap.hasTag(cellType, *cellIter, gcell)*/ ) nAdj++;

	if ( nAdj==0 )
	  {
	    // then this is a ghost (both adjacent cells are ghosts)
	    umap.setAsGhost(e_iter.getType(),*e_iter);
	  }
      }

    for ( e_iter=umap.begin(cellBdyType); e_iter!=e_iter_end; e_iter++)
      {
        // an e_iter is on the boundary if it only has one neighboring non-ghost cell
        int nAdj=0;
        for ( cellIter=umap.adjacency_begin(e_iter, cellType); 
              cellIter!=umap.adjacency_end(e_iter, cellType); cellIter++ )
	  if ( !cellIter.isGhost() /*umap.hasTag(cellType, *cellIter, gcell)*/ ) nAdj++;
	
	if ( nAdj==0 )
	  {
	    umap.setAsGhost(e_iter.getType(),*e_iter);
	  }
        else if ( nAdj==1 ) 
          {
	    std::string stag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[int(cellBdyType)].c_str();
	    std::string etag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Edge].c_str();
	    if ( !umap.hasTag(cellBdyType,*e_iter,stag) )
	      umap.addTag(cellBdyType,*e_iter,stag,((void *)*e_iter));
	    //	    break;
	    stag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[int(cellType)].c_str();
	    if ( !umap.hasTag(cellType,*umap.adjacency_begin(e_iter, cellType),stag) )
	      umap.addTag(cellType,*umap.adjacency_begin(e_iter, cellType),
			  stag,((void *)*umap.adjacency_begin(e_iter, cellType)));
	    
	    stag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Vertex)].c_str();

            // we are on a boundary, tag the vertices as such
            for ( vertIter=umap.adjacency_begin(e_iter, UnstructuredMapping::Vertex);
                  vertIter!=umap.adjacency_end(e_iter, UnstructuredMapping::Vertex);
                  vertIter++ )
	      if ( !umap.hasTag(UnstructuredMapping::Vertex, *vertIter, stag) )
		umap.addTag(UnstructuredMapping::Vertex, *vertIter, stag, ((void *)*vertIter));

	    if ( cellBdyType==UnstructuredMapping::Face )
	      {
		// then we need to tag the edges also! kkc 031213
		UnstructuredMappingAdjacencyIterator edgeIt, edgeIt_end;
		for ( edgeIt=umap.adjacency_begin(e_iter,UnstructuredMapping::Edge);
		      edgeIt!=umap.adjacency_end(e_iter,UnstructuredMapping::Edge);
		      edgeIt++ )
		  if ( !umap.hasTag(UnstructuredMapping::Edge, *edgeIt, etag) )
		    umap.addTag(UnstructuredMapping::Edge, *edgeIt, etag, ((void *)*edgeIt));
	      }
          }

	if ( e_iter.isGhost() )
	  {
	    int nAdj=0;
	    for ( cellIter=umap.adjacency_begin(e_iter, cellType); 
		  cellIter!=umap.adjacency_end(e_iter, cellType); cellIter++ ) nAdj++;

	    if (nAdj==1)
	      {
		std::string stag = std::string("Ghost boundary ") + UnstructuredMapping::EntityTypeStrings[int(cellBdyType)].c_str();
		std::string etag = std::string("Ghost boundary ") + UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Edge].c_str();
		std::string vtag = std::string("Ghost boundary ") + UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Vertex].c_str();

		if ( !umap.hasTag(cellBdyType,*e_iter,stag) )
		  umap.addTag(cellBdyType,*e_iter,stag,0);

		if ( cellBdyType==UnstructuredMapping::Face )
		  {
		    UnstructuredMappingAdjacencyIterator edgeIt;
		    for ( edgeIt=umap.adjacency_begin(e_iter,UnstructuredMapping::Edge);
		      edgeIt!=umap.adjacency_end(e_iter,UnstructuredMapping::Edge);
		      edgeIt++ )
		      if ( !umap.hasTag(edgeIt.getType(), *edgeIt, etag) )
			umap.addTag(edgeIt.getType(), *edgeIt, etag,0);
		  }

		for ( vertIter=umap.adjacency_begin(e_iter, UnstructuredMapping::Vertex);
		      vertIter!=umap.adjacency_end(e_iter, UnstructuredMapping::Vertex);
		      vertIter++ )
		  if ( !umap.hasTag(vertIter.getType(), *vertIter, vtag) )
		    umap.addTag(vertIter.getType(), *vertIter, vtag, 0);
	      }
	    
	  }
	
      }

    UnstructuredMapping::EntityTypeEnum bdybdyType = UnstructuredMapping::EntityTypeEnum(cellBdyType-1);
    UnstructuredMappingAdjacencyIterator biter,biter_end; // on second thought, biter_end is a pun
    std::string getag = string("Ghost ")+UnstructuredMapping::EntityTypeStrings[bdybdyType].c_str();
    std::string gbdytag = string("Ghost ")+UnstructuredMapping::EntityTypeStrings[cellBdyType].c_str();
    std::string bdytag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[bdybdyType].c_str();
    // loop through all the ghost cellBdyType, and bdybdyType that is not on the boundary is a ghost
#if 1
    for ( UnstructuredMapping::tag_entity_iterator git=umap.tag_entity_begin(gbdytag); 
	  git != umap.tag_entity_end(gbdytag); git++ )
      {
	biter_end = umap.adjacency_end(git->et, git->e, bdybdyType);
	for ( biter=umap.adjacency_begin(git->et, git->e, bdybdyType); biter!=biter_end; biter++ )
	  if ( !umap.hasTag( bdybdyType, *biter, bdytag ) && !biter.isGhost() )//umap.hasTag( bdybdyType, *biter, getag ) )
	    umap.setAsGhost(bdybdyType, *biter);
      }
#endif
#if 0
    std::string stag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[int(cellBdyType)].c_str();
    cout<<"NUMBER OF "<<stag<<" : "<<std::distance(umap.tag_entity_begin(stag),umap.tag_entity_end(stag))<<endl;
    cout<<"NUMBER OF "<<bdytag<<" : "<<std::distance(umap.tag_entity_begin(bdytag),umap.tag_entity_end(bdytag))<<endl;
    cout<<"NUMBER OF "<<gbdytag<<" : "<<std::distance(umap.tag_entity_begin(gbdytag),umap.tag_entity_end(gbdytag))<<endl;
    cout<<"NUMBER OF "<<getag<<" : "<<std::distance(umap.tag_entity_begin(getag),umap.tag_entity_end(getag))<<endl;
    cout<<"NUMBER OF "<<gcell<<" : "<<std::distance(umap.tag_entity_begin(gcell),umap.tag_entity_end(gcell))<<endl;
#endif
  }
  
  
}

UnstructuredMappingIterator::
UnstructuredMappingIterator() 
//===========================================================================
/// \details  Default Constructor
///  This class is used to iterate over the valid entities (excluding ghost entities)
/// \param uns (input):
/// \param entityType_ (input) :
/// \param position (input) : position=0 : start, position==1 : end.
//===========================================================================
{
  numberOfEntities=0; e=0; entityMask=NULL; /*includeGhostElements=0*/; entityType=UnstructuredMapping::Region;
  skipGhostEntities=false; skipMask=0;
}


UnstructuredMappingIterator::
UnstructuredMappingIterator(const UnstructuredMapping & uns, 
                            UnstructuredMapping::EntityTypeEnum entityType_,
			    int position, 
                            bool skipGhostEntities_ /* = false */ )
//===========================================================================
/// \details  Constructor for an iterator positioned to the start or end.
///  This class is used to iterate over the valid entities (excluding ghost entities)
/// \param uns (input): 
/// \param entityType_ (input) : Iterate over this entity.
/// \param position (input) : position=0 : start, position==1 : end.
/// \param includeGhostEntities (input) : if true include ghost entities in the iterator
//===========================================================================
{
  entityType=entityType_;
  //  includeGhostElements=includeGhostEntities && uns.includeGhostElements; // may be changed below.
  entityMask=NULL;

  skipGhostEntities = (skipGhostEntities_ /*kkc this should only be relevant to building from mappings && uns.includeGhostElements*/);
  skipMask = skipGhostEntities ? ( UnstructuredMapping::HoleInEntityData | UnstructuredMapping::GhostEntity ) : UnstructuredMapping::HoleInEntityData;

#if 1

  numberOfEntities = uns.size(entityType_);
  //  if ( !uns.entityMasks[entityType_] )
  //  includeGhostElements=false;
  //  else if ( includeGhostElements )
  if ( uns.entityMasks[entityType_] )
    entityMask=uns.entityMasks[entityType_]->getDataPointer();

  if ( position==0 )
    {
      e=0;
      //      if( includeGhostElements )
      //{
	  //kkc old interface	  while( entityMask[e]<=0 && e<numberOfEntities ) e++;  // look for the first non-ghost element
      //}
      //      int skipMask = includeGhostElements ? ( UnstructuredMapping::HoleInEntityData | UnstructuredMapping::GhostEntity ) : UnstructuredMapping::HoleInEntityData;
      while ( e<numberOfEntities && (entityMask[e]&skipMask) ) e++;
    }
  else
    {
      e = numberOfEntities;
    }

#else
  if( entityType==UnstructuredMapping::Region )
  {
    numberOfEntities=uns.getNumberOfElements();
    if( uns.elementMask==NULL )
      includeGhostElements=false;
    else if( includeGhostElements )
      entityMask=uns.elementMask->getDataPointer();
  }
  else if( entityType==UnstructuredMapping::Face )
  {
    numberOfEntities=uns.getNumberOfFaces();
    if( uns.faceMask==NULL )
      includeGhostElements=false;
    else if( includeGhostElements )
      entityMask=uns.faceMask->getDataPointer();
  }
  else if( entityType==UnstructuredMapping::Edge )
  {
    numberOfEntities=uns.getNumberOfEdges();
    if( uns.edgeMask==NULL )
      includeGhostElements=false;
    else if( includeGhostElements )
      entityMask=uns.edgeMask->getDataPointer();
  }
  else
  {
    numberOfEntities=uns.getNumberOfNodes();
   if( uns.nodeMask==NULL )
      includeGhostElements=false;
    else if( includeGhostElements )
      entityMask=uns.nodeMask->getDataPointer();
  }
  if( position==0 )
  {
    e=0;
    if( includeGhostElements )
    {
      while( entityMask[e]<=0 && e<numberOfEntities ) e++;  // look for the first non-ghost element
    }
  }
  else
  {
    e=numberOfEntities;  // marks the last element
//      e=numberOfEntities-1;
//      while( entityMask[e]<=0 && e<numberOfEntities) e--;
  }
#endif
}

void UnstructuredMappingIterator::
operator=(const UnstructuredMappingAdjacencyIterator & iter)
{
  entityType=iter.getType();
  entityMask=iter.entityMask;
  skipGhostEntities = iter.skipGhostEntities;
  skipMask = iter.skipMask;
  e=*iter;
  numberOfEntities=e+1;
}

void UnstructuredMappingIterator::
operator=(const UnstructuredMappingIterator & iter)
{
  entityType=iter.entityType;
  numberOfEntities=iter.numberOfEntities;
  entityMask=iter.entityMask;
  //  includeGhostElements=iter.includeGhostElements;
  skipGhostEntities = iter.skipGhostEntities;
  skipMask = iter.skipMask;
  e=iter.e;
}


UnstructuredMappingAdjacencyIterator::
UnstructuredMappingAdjacencyIterator() : entityType(UnstructuredMapping::Invalid), adjEntityType(UnstructuredMapping::Invalid), 
					 entityArray(0), orientationArray(0), numberOfEntities(0), stride(0), 
					 offset(0), e(0), skipGhostEntities(false), skipMask(0) { }

UnstructuredMappingAdjacencyIterator::
UnstructuredMappingAdjacencyIterator(const UnstructuredMapping & uns, UnstructuredMapping::EntityTypeEnum from, int adjTo,
				     UnstructuredMapping::EntityTypeEnum to, int position, bool skipGhostEntities_ /*= false*/ ) : 
  entityType(from), adjEntityType(to), entityArray(0), orientationArray(0), numberOfEntities(0), stride(0), offset(0), 
  e(0), entityMask(0), skipGhostEntities(skipGhostEntities_) 
{ 
  skipGhostEntities = (skipGhostEntities /*kkc this should only be relevant to building from mappings && !uns.includeGhostElements*/);
  skipMask = skipGhostEntities ? ( UnstructuredMapping::HoleInEntityData | UnstructuredMapping::GhostEntity ) : UnstructuredMapping::HoleInEntityData;

  
  if ( !uns.entityMasks[to]) skipGhostEntities=false;

  //  if ( skipGhostEntities )
  if (uns.entityMasks[to]) entityMask = uns.entityMasks[to]->getDataPointer();;
  orientationArray = uns.adjacencyOrientation[from][to];

  if ( from>to )
    {
      stride = uns.capacity(from);
      offset = adjTo;
      entityArray = uns.indexLists[from][to] ? uns.indexLists[from][to]->getDataPointer() : 0;

      switch(to) {
      case UnstructuredMapping::Vertex:
	{
	  if ( from==UnstructuredMapping::Edge )
	    {
	      entityArray = ((UnstructuredMapping &)uns).getEntities(UnstructuredMapping::Edge).getDataPointer();
	      numberOfEntities=2;
	    }
	  else
	    {	    
	      UnstructuredMapping::ElementType et = ((UnstructuredMapping &)uns).computeElementType(from,adjTo);
	      numberOfEntities = topoNVerts[et];
	      entityArray = ((UnstructuredMapping &)uns).getEntities(from).getDataPointer();
	    }
	  break;
	}
      case UnstructuredMapping::Edge:
	{
	  UnstructuredMapping::ElementType et = ((UnstructuredMapping &)uns).computeElementType(from,adjTo);
	  numberOfEntities = topoNEdges[et];
	  break;
	}
      case UnstructuredMapping::Face:
	{
	  UnstructuredMapping::ElementType et = ((UnstructuredMapping &)uns).computeElementType(from,adjTo);
	  numberOfEntities = topoNFaces[et];
	  break;
	}
      default:
	numberOfEntities=-1;
      }
    }
  else if ( from<to )
    {
      stride=1; //csr arrays are 1d
      if ( uns.indexLists[from][to] )
	{
	  offset = (*uns.upwardOffsets[from][to])(adjTo);
	  numberOfEntities = (*uns.upwardOffsets[from][to])(adjTo+1)-(*uns.upwardOffsets[from][to])(adjTo);
	  entityArray = uns.indexLists[from][to] ? uns.indexLists[from][to]->getDataPointer() : 0;
	}
    }
  else
    {
      stride=0; offset=0;
      //      entityArray = uns.entities[from] ? uns.entities[from]->getDataPointer() : 0;
      entityArray = & (this->e);
      numberOfEntities = adjTo+1;//uns.size(from);
    }

  if ( position==0 && entityMask )
    {
      if ( from!=to )
	{
	  e=0;
	  //      if ( skipGhostEntities )
	  while ( e<numberOfEntities && (entityMask[entityArray[offset + e*stride]]&skipMask) ) e++;
	}
      else
	{
	  e = adjTo;
	}
    }
  else
    e=numberOfEntities;
}

UnstructuredMappingAdjacencyIterator &
UnstructuredMappingAdjacencyIterator::operator=(const UnstructuredMappingAdjacencyIterator &i)
{
  entityType = i.entityType;
  orientationArray = i.orientationArray;
  adjEntityType = i.adjEntityType;
  entityArray = i.entityArray;
  numberOfEntities = i.numberOfEntities;
  stride = i.stride;
  offset = i.offset;
  e = i.e;
  entityMask = i.entityMask;
  skipGhostEntities = i.skipGhostEntities;
  skipMask = i.skipMask;

  return *this;
}
													     

UnstructuredMapping::
UnstructuredMapping(int domainDimension_ /* =3 */, 
		    int rangeDimension_ /* =3 */, 
		    mappingSpace domainSpace_ /* =parameterSpace */,
		    mappingSpace rangeSpace_ /* =cartesianSpace */ ) : Mapping(domainDimension_,rangeDimension_,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Default Constructor
//
{
  UnstructuredMapping::className="UnstructuredMapping";
  setName( Mapping::mappingName,"unstructuredMapping"); 

  dumpTagsToHDF = true;//kkc 050124 false;
  preferTriangles=false;
  elementDensityTolerance=0.; // *wdh* 020727  0.05;
  stitchingTolerance=0.;
  absoluteStitchingTolerance=REAL_MAX;
  
  elementFaces=NULL;
  search=NULL;
  
  //  includeGhostElements=false; kkc 040714 changed default to know about ghosts
  includeGhostElements=true;
  ghostElements=NULL;  // optional info about ghost elements
  elementMask=NULL;
  faceMask=NULL;
  edgeMask=NULL;
  nodeMask=NULL;

  gridColour="blue"; // default colour

  initMapping();
}

UnstructuredMapping::
UnstructuredMapping() : Mapping(2,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Default Constructor
//===========================================================================
{ 
  dumpTagsToHDF = true;//kkc 050124 false;
  preferTriangles=false;
  elementDensityTolerance=0.; // *wdh* 020727  0.05;
  stitchingTolerance=0.;
  absoluteStitchingTolerance=REAL_MAX;
  elementFaces=NULL;
  search=NULL;

  UnstructuredMapping::className="UnstructuredMapping";
  setName( Mapping::mappingName,"unstructuredMapping"); 

  //  includeGhostElements=false; kkc 040714 changed default to know about ghosts
  includeGhostElements=true;
  ghostElements=NULL;  // optional info about ghost elements
  elementMask=NULL;
  faceMask=NULL;
  edgeMask=NULL;
  nodeMask=NULL;
  gridColour="blue"; // default colour

  initMapping();
}


// Copy constructor is deep by default
UnstructuredMapping::
UnstructuredMapping( const UnstructuredMapping & map, const CopyType copyType )
{
  UnstructuredMapping::className="UnstructuredMapping";

  initMapping();
  
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "UnstructuredMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

UnstructuredMapping::
~UnstructuredMapping()
{ 
  if( debug & 4 )
    cout << " UnstructuredMapping::Destructor called" << endl;
  
  // clean up tagging info
  maintainTagToEntityMap(false);
  
  for ( int eti=int(UnstructuredMapping::Vertex); 
	eti<(UnstructuredMapping::Mesh); eti++ )
    {
      
      UnstructuredMapping::EntityTypeEnum et=UnstructuredMapping::EntityTypeEnum(eti);
      UnstructuredMappingIterator it;
      
      if ( size((EntityTypeEnum)eti) )
	for ( it = begin(et); it!=end(et); it++ )
	  {
	    entity_tag_iterator tagit=entity_tag_begin(et,*it);
	    while ( tagit!=entity_tag_end(et,*it) )
	      {
		entity_tag_iterator tag2delete = tagit;
		deleteTag(et, *it, (*tag2delete)->getName());
		tagit = entity_tag_begin(et,*it);
	      }
	  }
    }
  
  entity_tag_iterator tagit=entity_tag_begin(Mesh,0);
  while ( tagit!=entity_tag_end(Mesh,0) )
    {
      entity_tag_iterator tag2delete = tagit;
      deleteTag(Mesh, 0, (*tag2delete)->getName());
      tagit = entity_tag_begin(Mesh,0);
    }
  
  entityTags.clear();
  
  dumpTagsToHDF = false;
  // *kkc* destroy all the connectivity
  deleteConnectivity();
  
  if ( elementFaces ) delete elementFaces; 
  if ( search ) delete search;
  if ( ghostElements ) delete ghostElements;
  if ( elementMask ) delete elementMask;
  if ( faceMask ) delete faceMask;
  if ( edgeMask ) delete edgeMask;
  if ( nodeMask ) delete nodeMask;
  
}

void UnstructuredMapping::
initMapping() 
{ 
// AP: initMapping gets called by both constructors, the copy constructor,
// both versions of setNodesAndConnectivity, setNodesElementsAndNeighbours, 
// and buildFromAMapping.

  int i;
  
  numberOfNodes=0;
  numberOfElements=0;
  numberOfFaces=0;
  numberOfBoundaryFaces=0;
  numberOfEdges=0;
  numberOfNodesPerElementIsConstant=false;  // set to true if all elements are the same
  
  setGridDimensions( 0,numberOfNodes );  
  for( int axis=1; axis<domainDimension; axis++ )
    setGridDimensions(axis,1);
  debugs=0;

  search=NULL;
  elementFaces=NULL; 
  ghostElements=NULL;
  elementMask=NULL;
  faceMask=NULL;
  edgeMask=NULL;
  nodeMask=NULL;

// initialize display lists
  for(i=0; i<numberOfDLProperties; i++ )
    dList[i] = 0;
  
  for(i=0; i<numberOfTimings; i++ )
    timing[i]=0.;

  maintainsTagEntities = false;

  FEZInitializeConnectivity();  // Finite Element Zoo connectivity

  // *kkc* initialize the arrays of connectivity to null
  int i_et=0;
  for ( EntityTypeEnum et=UnstructuredMapping::Vertex; et<UnstructuredMapping::Mesh; et = EntityTypeEnum(++i_et) )
    {
      entities[int(et)] = entityMasks[int(et)] = NULL;
      entitySize[int(et)] = entityCapacity[int(et)] = 0;
      int i_et2=0;
      for ( EntityTypeEnum et2=UnstructuredMapping::Vertex; et2<UnstructuredMapping::Mesh; et2=EntityTypeEnum(++i_et2) )
	{
	  indexLists[int(et)][int(et2)] = indexLists[int(et2)][int(et)] = 
	    upwardOffsets[int(et)][int(et2)] = upwardOffsets[int(et2)][int(et)] = NULL;
	  adjacencyOrientation[int(et)][int(et2)] = adjacencyOrientation[int(et2)][int(et)] = NULL;

	}
    }

  maintainTagToEntityMap(true);
}

UnstructuredMapping & UnstructuredMapping::
operator=( const UnstructuredMapping & x )
{
  if( UnstructuredMapping::className != x.getClassName() )
  {
    cout << "UnstructuredMapping::operator= ERROR trying to set a UnstructuredMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class

  preferTriangles=x.preferTriangles;
  elementDensityTolerance=x.elementDensityTolerance;

  // remove the search tree so that it will be rebuilt when needed.  -- is this what we want ? --
  if( search!=NULL )
    delete search;
  search=NULL;

  
  numberOfNodes          =x.numberOfNodes;
  numberOfElements       =x.numberOfElements;
  numberOfFaces          =x.numberOfFaces;
  numberOfEdges          =x.numberOfEdges;
  maxNumberOfNodesPerElement= x.maxNumberOfNodesPerElement;
  maxNumberOfNodesPerFace   = x.maxNumberOfNodesPerFace;
  maxNumberOfFacesPerElement= x.maxNumberOfFacesPerElement;
  numberOfInternalElements  = x.numberOfInternalElements;
  numberOfInternalFaces     = x.numberOfInternalFaces;
  numberOfBoundaryElements  = x.numberOfBoundaryElements;
  numberOfBoundaryFaces     = x.numberOfBoundaryFaces;
  
  node.redim(0);
  node             =x.node;
  element.redim(0);
  element          =x.element;
  face.redim(0);
  face             =x.face;
  bdyFace.redim(0);
  bdyFace          =x.bdyFace;
  bdyFaceTags.redim(0);
  bdyFaceTags      =x.bdyFaceTags;
  faceElements.redim(0);
  faceElements     =x.faceElements;
  edge.redim(0);
  edge             =x.edge;
  tags.redim(0);
  tags             =x.tags;

  stitchingTolerance=x.stitchingTolerance;
  absoluteStitchingTolerance=x.absoluteStitchingTolerance;

  // FEZ specific stuff
  numberOfFacesThisElementType.redim(0);
  numberOfFacesThisElementType = x.numberOfFacesThisElementType;
  numberOfNodesThisElementType.redim(0);  
  numberOfNodesThisElementType = x.numberOfNodesThisElementType;
  numberOfNodesThisElementFaceType.redim(0);
  numberOfNodesThisElementFaceType = x.numberOfNodesThisElementFaceType;
  elementMasterTemplate.redim(0);
  elementMasterTemplate = x.elementMasterTemplate;

  elementType.redim(0);
  elementType = x.elementType;
  faceZ1Offset.redim(0);
  faceZ1Offset = x.faceZ1Offset;
  
  if( x.elementFaces==NULL )
  {
    delete elementFaces;  elementFaces=NULL;
  }
  else if( elementFaces==NULL )
  {
    elementFaces = new intArray();
    *elementFaces=*(x.elementFaces);
  }
  else
  {
    elementFaces->redim(0);
    *elementFaces=*(x.elementFaces);
  }
  
  // NEW CONNECTIVITY
  int i_et=0;

  for ( EntityTypeEnum et=UnstructuredMapping::Vertex; et<UnstructuredMapping::Mesh; et=EntityTypeEnum(++i_et) )
    {
      reserve(et, x.entityCapacity[et]);

      if ( x.entities[et] )
	*entities[et] = *(x.entities[et]);

      if ( x.entityMasks[et] )
	*(entityMasks[et]) = *(x.entityMasks[et]);

      entitySize[et] = x.entitySize[et];

      entityDataStructureHoles[et] = x.entityDataStructureHoles[et];

      int i_et2 = 0;
      for ( EntityTypeEnum et2=UnstructuredMapping::Vertex; et2<UnstructuredMapping::Mesh; et2=EntityTypeEnum(++i_et2))
	{
	  if ( x.indexLists[et][et2] && et<et2 )
	    specifyConnectivity(et, et2, *x.indexLists[et][et2],  x.adjacencyOrientation[et][et2], *x.upwardOffsets[et][et2]);
	  else if ( x.indexLists[et][et2] && et>et2 )
	    specifyConnectivity(et, et2, *x.indexLists[et][et2], x.adjacencyOrientation[et][et2]);
	  else
	    {
	      deleteConnectivity(et,et2);
	      deleteConnectivity(et2,et);
	    }	      
	     
	}

    }

  node = x.node;

  //kkc why did I think this would work?  entityTags=x.entityTags;
  //kkc 050124 actually set new tags as specified by the copied mapping x
  for ( int eti=int(UnstructuredMapping::Vertex); 
	eti<(UnstructuredMapping::Mesh); eti++ )
    {
      
      UnstructuredMapping::EntityTypeEnum et=UnstructuredMapping::EntityTypeEnum(eti);
      UnstructuredMappingIterator it;
      
      if ( size((EntityTypeEnum)eti) )
	for ( it = begin(et); it!=end(et); it++ )
	  {
	    for ( const_entity_tag_iterator xtag=x.entity_tag_begin(et,*it); xtag!=x.entity_tag_end(et,*it); xtag++ )
	      {
		EntityTag &xt = **xtag;
		addTag(et,*it,xt.getName(), xt.getData(), xt.copiesData(), xt.getDataSize());
	      }
	  }
    }

  tagEntities = x.tagEntities;
  dumpTagsToHDF = x.dumpTagsToHDF;

  return *this;
}

void  UnstructuredMapping::
addGhostElements( bool trueOrFalse )
//===========================================================================
/// \details 
///    Specify whether to add ghost elements to the unstructured mapping.
///  
/// \param trueOrFalse (input): If true add ghost elements to the unstructured mapping.
//===========================================================================
{
  includeGhostElements=trueOrFalse;
}


const intArray &  UnstructuredMapping::
getBoundaryFace()
//===========================================================================
/// \details 
///    Return a list of boundary faces,
///  
/// \param boundaryFace (return value) : faces on the boundary.
//===========================================================================
{
  return bdyFace;
}

const intArray &  UnstructuredMapping::
getGhostElements() const
//===========================================================================
/// \details 
///    Return a list of ghost elements.
///  
/// \param boundaryFace (return value) : faces on the boundary.
//===========================================================================
{
  return ghostElements!=NULL ? *ghostElements : Overture::nullIntegerDistributedArray();
}

const intArray &  UnstructuredMapping::
getMask(EntityTypeEnum entityType) const
//===========================================================================
/// \details 
///    Return a the mask associated with the nodes, edges, faces or regions.
///  
/// \param boundaryFace (return value) : faces on the boundary.
//===========================================================================
{
  if( entityType == Region )
  {
    return elementMask!=NULL ? *elementMask : Overture::nullIntegerDistributedArray();
  }
  else if( entityType==Face )
  {
    return faceMask!=NULL ? *faceMask : Overture::nullIntegerDistributedArray();
  }
  else if( entityType==Edge )
  {
    return edgeMask!=NULL ? *edgeMask : Overture::nullIntegerDistributedArray();
  }
  else 
  {
    return nodeMask!=NULL ? *nodeMask : Overture::nullIntegerDistributedArray();
  }
  
}



const intArray &  UnstructuredMapping::
getBoundaryFaceTags()
//===========================================================================
/// \details 
///    Return a list of the tags on each boundary face, usefull for boundary conditions
///  
/// \param boundaryFaceTags (return value) : tags for faces on the boundary.
//===========================================================================
{
  return bdyFaceTags;
}

int UnstructuredMapping::
getNumberOfNodes() const
//===========================================================================
/// \details 
///    Return the number of nodes.
//===========================================================================
{
  return size(UnstructuredMapping::Vertex) ? size(UnstructuredMapping::Vertex) : numberOfNodes;
}

int UnstructuredMapping::
getMaxNumberOfNodesPerElement() const
//===========================================================================
/// \details 
///    Return the maximum number of nodes per element (max over all elements).
//===========================================================================
{
  return maxNumberOfNodesPerElement;
}

int UnstructuredMapping::
getMaxNumberOfFacesPerElement() const
//===========================================================================
/// \details 
///    Return the maximum number of faces per element (max over all elements).
//===========================================================================
{
  return maxNumberOfNodesPerElement;
}
int UnstructuredMapping::
getMaxNumberOfNodesPerFace() const
//===========================================================================
/// \details 
///    Return the maximum number of nodes per face (max over all faces).
//===========================================================================
{
  return maxNumberOfNodesPerFace;
}

int UnstructuredMapping::
getNumberOfElements() const
//===========================================================================
/// \details 
///    Return the number of elements (such as the number of triangles on a 2d grid or 3d surface or
///  the number of tetrahedra in a 3d grid).
//===========================================================================
{
  return size(EntityTypeEnum(domainDimension)) ? size(EntityTypeEnum(domainDimension)) : numberOfElements;
}

int UnstructuredMapping::
getNumberOfFaces() const
//===========================================================================
/// \details 
///    Return the number of faces.
//===========================================================================
{
  return size(EntityTypeEnum(domainDimension-1)) ? size(EntityTypeEnum(domainDimension-1)) : numberOfFaces;
}

int UnstructuredMapping::
getNumberOfBoundaryFaces() const
//===========================================================================
/// \details 
///    Return the number of faces.
//===========================================================================
{
  return numberOfBoundaryFaces;
}

int UnstructuredMapping::
getNumberOfEdges() const
//===========================================================================
/// \details 
///    Return the number of edges.
//===========================================================================
{
  return numberOfEdges;
}

const realArray & UnstructuredMapping::
getNodes() const
//===========================================================================
/// \details 
///    Return the list of nodes.
/// \param node (return value) : list of nodes, node(i,0:r-1) : (x,y) or (x,y,z) coordinates 
///    for each node, i=0,1,... r=rangeDimension
//===========================================================================
{
  return node;
}

const intArray & UnstructuredMapping::
getElements() const
//===========================================================================
/// \details 
///    Return the node information for each element.
///  
/// \param element (return value) : defines the nodes that make up each element (e.g. triangle),
///    element(i,n) index into the nodes array for the node n of element i,
///    for now n=0,1,2 for triangles. Thus element i will have nodes (element(i,0),element(i,1),...)
//===========================================================================
{
  return element;
}

const intArray & UnstructuredMapping::
getFaces()
//===========================================================================
/// \details 
///    Return the connectivity information for each face.
///  
/// \param face (return value) : defines the nodes that make up each face (e.g. triangle),
///    face(i,n) index into the nodes array for the node n of face i,

//===========================================================================
{
  if ( !face.getLength(0) ) buildConnectivityLists();
  return face;
}

const intArray & UnstructuredMapping::
getFaceElements()
//===========================================================================
/// \details 
///    Return the connectivity information containing the elements adjacent to each face.
///  
/// \param faceElements (return value) : defines the elements adjacent to each face,
///    faceElements(i,e) index into the elements array for the element e of face i,
///    for now e=0,1 since each face has two elements. 
///    For now, faces on a boundary return -1 for e=1.
//===========================================================================
{
  if ( !faceElements.getLength(0) ) buildConnectivityLists();
  return faceElements;
}

const intArray & UnstructuredMapping::
getEdges()
//===========================================================================
/// \details 
///    Return the connectivity information for each edge.
///  
/// \param edge (return value) : defines the 2 nodes that make up each edge.
///    face(i,n) index into the nodes array for the node n of face i,
///    for now n=0,1. Thus edge 0 will have end points with node numbers (edge(i,0),edge(i,1))
//===========================================================================
{
  if ( !edge.getLength(0) ) buildConnectivityLists();
  return edge;
}

const intArray &  UnstructuredMapping::
getElementFaces() 
//===========================================================================
/// \details 
///    Return the connectivety array describing the faces that belong to an element.
///  
/// \param elementFaces (return value) : defines the faces that belong to an element.
///     face=elementFaces(e,i) is the face for i=0,1,..
//===========================================================================
{
  // **** this only works for triangles and quads
  if( domainDimension!=2  )
  {
    printf("UnstructuredMapping::getElementFaces:ERROR: this routine has only been implemented for "
           "domainDimension==2\n");
  }

  if( elementFaces==NULL ) // should check if connectivity has changed.
  {
    if ( !faceElements.getLength(0) ) buildConnectivityLists();

    elementFaces= new intArray(numberOfElements,maxNumberOfNodesPerElement);
    *elementFaces=0;

    intArray & ef = *elementFaces;
  
    if( maxNumberOfNodesPerElement==3 )
    {
      // Triangles
      for( int f=0; f<numberOfFaces; f++ )
      {
	for( int i=0; i<=1; i++ ) // loop over two elements on this face.
	{
	  int e = faceElements(f,i);   // element on this face
	  if( e<0 )
	    continue;  // boundary face will only belong to one element.
	  int n0 = face(f,0);  // first node on this face.
	  int n1 = face(f,1);  // 2nd node
	  int faceOffset=-1;
	  if( element(e,0)==n0 )
	  {
	    if( element(e,1)==n1 )
	      faceOffset=0;
	    else
	      faceOffset=2;
	  }
	  else if( element(e,1)==n0 )
	  {
	    if( element(e,2)==n1 )
	      faceOffset=1;
	    else
	      faceOffset=0;
      
	  }
	  else if( element(e,2)==n0 )
	  {
	    if( element(e,0)==n1 )
	      faceOffset=2;
	    else
	      faceOffset=1;
	  }
	  if( faceOffset<0 )
	  {
            if( getIsPeriodic(0)==notPeriodic &&  getIsPeriodic(1)==notPeriodic )
	    {
              // this test will sometimes fail for periodic grids
	      printf("getElementFaces:ERROR: face f=%i, nodes=(%i,%i) is next to element e=%i, nodes=(%i,%i,%i)\n",
		     f,n0,n1,e,element(e,0),element(e,1),element(e,2));
	      printf(" *** The nodes do not match *** \n");
	    }
	    
	  }
	  else
	  {
	    ef(e,faceOffset)=f;
	  }
	}
      }
    }
    else 
    {
      int nn;
      for( int f=0; f<numberOfFaces; f++ )
      {
	for( int i=0; i<=1; i++ ) // loop over two elements on this face.
	{
	  const int e = faceElements(f,i);   // element on this face
	  if( e<0 )
	    continue;  // boundary face will only belong to one element.
	  const int n0 = face(f,0);  // first node on this face.
	  const int n1 = face(f,1);  // 2nd node
          const int numNodes=getNumberOfNodesThisElement(e);
          bool found=false;
	  for( nn=0; nn<numNodes; nn++ )
	  {
	    int na=element(e,nn), nb=element(e,(nn+1)%numNodes);
	    if( (na==n0 && nb==n1) || (na==n1 && nb==n0) )
	    {
	      ef(e,nn)=f;
              found=true;
	      break;
	    }
	  }
          if( !found )
	  {
            if( getIsPeriodic(0)==notPeriodic &&  getIsPeriodic(1)==notPeriodic )
	    {
              // this test will sometimes fail for periodic grids
	      printf("getElementFaces:ERROR: face f=%i, nodes=(%i,%i) is next to element e=%i, nodes=(",
		     f,n0,n1,e);
	      for( nn=0; nn<numNodes; nn++ )
		printf("%i,",element(e,nn));
	      printf(")\n");
	      printf(" *** The nodes do not match *** \n");
	    }
	  }
	}
      }
    }
    
    // ::display(ef,"elementFaces","%4i");
    
  }
  return *elementFaces;
}



const intArray & UnstructuredMapping::
getTags()
//===========================================================================
/// \details 
///    Return the element tagging information.
///  
/// \param tags (return value) : an integer tag for each element, defaults to 0 for
///  every element
//===========================================================================
{
  return tags;
}

void UnstructuredMapping::
setElementDensityTolerance(real tol)
//===========================================================================
/// \details 
///    Specify the tolerance for determining the triangle density when building from a mapping.
///   The smaller the tolerance the more triangles.
///    Choose a value of zero to use the default number of elements
///  
/// \param tol (input) : new tolerance.
//===========================================================================
{
  elementDensityTolerance=tol;
}



void UnstructuredMapping::
setTags( const intArray &new_tags )
//===========================================================================
/// \details 
///    Set the list of tags for each element;
/// \param tags (input) : an array the length of the number of elements containing an integer
///  tag for each element (eg like material region identifier)
// ========================================================================================
{
  assert(new_tags.getLength(0) == numberOfElements);

  tags = new_tags;
}

int UnstructuredMapping::
setNodesAndConnectivity( const realArray & nodes, 
                         const intArray & elements,
                         int domainDimension_ /* =-1 */,
			 bool buildConnectivity /* =true */)
//===========================================================================
/// \details 
///    Supply a list of nodes and a list of connectivity information.
/// \param nodes (input) : nodes(i,0:r-1) (x,y) or (x,y,z) coordinates for each node, i=0,1,... r=rangeDimension
/// \param elements (input) : defines the nodes that make up each element (e.g. triangle),
///    elements(i,n) index into the nodes array for the node n of element i,
///    for now n=0,1,2 for triangles. Thus element 0 will have nodes (elements(i,0),elements(i,1),...)
///    A value of elements(i,n)==-1 means no node is used. This option is used to specify elements
///     with different numbers of nodes per elements. For example if one has quadrilaterals and
///   triangles then set element(i,3)=-1 for triangles.
// ========================================================================================
{

  // kkc destroy any allocated connectivity arrays
  deleteConnectivity();


  if( domainDimension_==2 || domainDimension_==3 )
    domainDimension=domainDimension_;

  int nd=nodes.getLength(1);
  if( nd==2 || nd==3 )
  {
    rangeDimension=nd;
  }
  else
  {
    {throw "error";}
  }
  
  initMapping(); // AP: initMapping sets maxNumberOfNodesPerElement from domainDimension
  numberOfNodes=nodes.getLength(0);
  setGridDimensions( axis1,numberOfNodes );  

  node.redim(0);
  node=nodes;

  numberOfElements=elements.getLength(0);
  int inputNumberOfNodesPerElement=elements.getLength(1);

  maxNumberOfNodesPerElement = max(4,maxNumberOfNodesPerElement);
  element.redim(numberOfElements, maxNumberOfNodesPerElement);
  element = -1;

  Index IEnds(0,inputNumberOfNodesPerElement);
  Index IElements(0,numberOfElements);
  element(IElements, IEnds)=elements(IElements,IEnds);

  Range R(0,numberOfNodes-1);
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
  }

  if (buildConnectivity)
    buildConnectivityLists(); // buildConnectivityLists calls FEZComputeElementTypes
  else
    FEZComputeElementTypes();

  tags.redim(numberOfElements);
  tags = 0;

  // kkc set up the new connectivity
  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;

  if ( domainDimension==3 )
    specifyEntity(Region,element);
  else if ( domainDimension==2 )
    specifyEntity(Face,element);
      

  tagBoundaryEntities(*this);
  
  // remove the search tree so that it will be rebuilt when needed. 
  if( search!=NULL )
    delete search;
  search=NULL;

  return 0;
}

static int *faceNode; // used by boundaryFaceCompare for sorting boundary faces.

static int boundaryFaceCompare(const void *i0, const void *j0)
{
  int i = *(int*)i0;
  int j = *(int*)j0;
  
  if( faceNode[i] > faceNode[j] )
    return (1);
  if( faceNode[i] < faceNode[j] )
    return (-1);
  return (0);
  
}

int UnstructuredMapping::
setNodesElementsAndNeighbours(const realArray & nodes, 
			      const intArray & elements, 
			      const intArray & neighbours,
			      int numberOfFaces_ /* =-1 */,
			      int numberOfBoundaryFaces_ /* =-1 */,
			      int domainDimension_  /* =-1 */ )
//===========================================================================
/// \details 
///    Supply a list of nodes, elements and element neighbours. The element neighbours are used
///    in building the connectivity information. This should be faster than using setNodesAndConnectivity.
/// 
/// \param nodes (input) : nodes(i,0:r-1) (x,y) or (x,y,z) coordinates for each node, i=0,1,... r=rangeDimension
/// \param elements (input) : defines the nodes that make up each element (e.g. triangle),
///    elements(i,n) index into the nodes array for the node n of element i,
///    for now n=0,1,2 for triangles. Thus element 0 will have nodes (elements(i,0),elements(i,1),...)
///    A value of elements(i,n)==-1 means no node is used. This option is used to specify elements
///     with different numbers of nodes per elements. For example if one has quadrilaterals and
///   triangles then set element(i,3)=-1 for triangles.
/// \param neighbours (input) : a list of neighbours for each element.
///  / numberOfFaces\_ (input) : optionally supply the number of faces, if known. 
///  / numberOfBoundaryFaces\_ (input) : optionally supply the number of boundary faces, if known. 
// ========================================================================================
{

  real timeStart = getCPU();

  // kkc destroy any allocated connectivity arrays
  deleteConnectivity();

  if( domainDimension_==2 || domainDimension_==3 )
    domainDimension=domainDimension_;

  int nd=nodes.getLength(1);
  if( nd==2 || nd==3 )
  {
    rangeDimension=nd;
  }
  else
  {
    {throw "error";}
  }
  
  initMapping();
  numberOfNodes=nodes.getLength(0);
  setGridDimensions( axis1,numberOfNodes );  

  node.redim(0);
  node=nodes;

  numberOfElements=elements.getLength(0);
  int inputNumberOfNodesPerElement=elements.getLength(1);

  element.redim(numberOfElements, maxNumberOfNodesPerElement);
  element = -1;

  Index IEnds(0,inputNumberOfNodesPerElement);
  Index IElements(0,numberOfElements);
  element(IElements, IEnds)=elements(IElements,IEnds);

  Range R(0,numberOfNodes-1);
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
    // printf("*** axis=%i rangeBound=(%e,%e)\n",axis,(real)getRangeBound(Start,axis),(real)getRangeBound(End,axis));
  }

  // Now build the connectivity 

  if( elementFaces==NULL )
    elementFaces = new intArray;
      
  intArray & ef = *elementFaces;
  ef.redim(numberOfElements,3);
  
  ef=-1;

  numberOfFaces=numberOfFaces_;
  numberOfBoundaryFaces=numberOfBoundaryFaces_;

  int maxNumberOfFaces=numberOfFaces>0 ? numberOfFaces : numberOfElements*3;
  face.redim(maxNumberOfFaces,2);
  faceElements.redim(maxNumberOfFaces,2);
  faceElements=-1;
  
  int maxNumberOfBoundaryFaces=numberOfBoundaryFaces>0 ? numberOfBoundaryFaces : numberOfNodes;
  bdyFace.redim(maxNumberOfBoundaryFaces);

  int f=0, bf=0;
  for( int e=0; e<numberOfElements; e++ )
  {
    for( int m=0; m<3; m++ )  // check the three faces on this element.
    {
      if( ef(e,m)==-1 )
      {
        // this is a new face
        assert( f<maxNumberOfFaces );
	
        face(f,0)=element(e,m);
        face(f,1)=element(e,(m+1)%3);
	
        ef(e,m)=f;
        faceElements(f,0)=e;

        int en = neighbours(e,(m+2)%3);
        if( en>=0 )
	{
	  int mn = element(en,0)==face(f,1) ? 0 : element(en,1)==face(f,1) ? 1 : 2;
	
          assert( ef(en,mn)==-1 );
          assert( element(en,mn)==face(f,1) && element(en,(mn+1)%3)==face(f,0) );

	  ef(en,mn)=f;

	  faceElements(f,1)=en;
	}
	else
	{
	  // this is a boundary face ** these are sorted below ** 
          assert( bf<maxNumberOfBoundaryFaces );
          // printf(" Boundary face %i is face f=%i, nodes=(%i,%i) \n",bf,f,face(f,0),face(f,1));
          bdyFace(bf)=f;
	  bf++;
	}

	f++;
      }
    }
  }
  if( numberOfFaces<0 )
  {
    numberOfFaces=f;
    face.resize(numberOfFaces,2);
    faceElements.resize(numberOfFaces,2);
  }
  if( numberOfBoundaryFaces!=bf )
  {
    if( numberOfBoundaryFaces!=bf && bf>0)
      bdyFace.resize(bf);
    else if( bf==0 )
      bdyFace.redim(0);
    numberOfBoundaryFaces=bf;
  }
  // sort boundary faces by first node -- this will tend to order faces if the original boundary
  // nodes were ordered.
  //  const intArray & bn = face(bdyFace,0);
  // --> use qsort with a comparison based on face(i,0) > face(j,0)

  if( numberOfBoundaryFaces>0 )
  {
    faceNode = &face(0,0);  // used by the boundaryFaceCompare function, above.
    qsort( &bdyFace(0),numberOfBoundaryFaces,sizeof(int), boundaryFaceCompare );
  }

//   for( bf=0; bf<numberOfBoundaryFaces; bf++ )
//   {
//     int f = bdyFace(bf);
//     printf(" Sorted boundary face %i is face f=%i, nodes=(%i,%i) \n",bf,f,face(f,0),face(f,1));
//   }

  bdyFaceTags.redim(numberOfBoundaryFaces);
  bdyFaceTags = 0;
  tags.redim(numberOfElements);
  tags = 0;

  numberOfEdges=numberOfFaces;
  edge.reference(face);
  
  maxNumberOfNodesPerElement=3;
  maxNumberOfNodesPerFace=2;
  maxNumberOfFacesPerElement=3;

  // remove the search tree so that it will be rebuilt when needed. 
  if( search!=NULL )
    delete search;
  search=NULL;

  if( debug & 1 )
  {
    real timeEnd = getCPU();
    real connectivityGenTime = timeEnd - timeStart;
    printf("buildConnectivityLists : numberOfNodes = %i \n", numberOfNodes);
    printf("buildConnectivityLists : numberOfElements = %i \n", numberOfElements);
    printf("buildConnectivityLists : numberOfFaces = %i \n", numberOfFaces);
    printf("buildConnectivityLists : numberOfBoundaryFaces = %i \n", numberOfBoundaryFaces);
    printf("buildConnectivityLists : numberOfEdges = %i \n", numberOfEdges);
    printf("buildConnectivityLists : connectivityGenTime = %e \n", connectivityGenTime);
  }
  
  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;

  if ( domainDimension==3 )
    {
      specifyEntity(Region,element);
      //      specifyEntity(Face,face);
    }
  else if ( domainDimension==2 )
    {
      specifyEntity(Face,element);
      //      specifyEntity(Edge,face);
    }

  if( false )
    printConnectivity();
  

  return 0;
}

int UnstructuredMapping::
setNodesAndConnectivity( const realArray & nodes, 
			 const intArray & elements,
			 const intArray & faces,
			 const intArray & faceElements_,
			 const intArray & elementFaces_,
			 int numberOfFaces_ /* =-1 */,
			 int numberOfBoundaryFaces_ /* =-1 */,
			 int domainDimension_ /* =-1 */,
                         bool constantNumberOfNodesPerElement /* =false */ )
//===========================================================================
/// \details 
///    Supply a list of nodes, elements and element neighbours. The element neighbours are used
///    in building the connectivity information. This should be faster than using setNodesAndConnectivity.
/// 
/// \param nodes (input) : nodes(i,0:r-1) (x,y) or (x,y,z) coordinates for each node, i=0,1,... r=rangeDimension
/// \param elements (input) : defines the nodes that make up each element (e.g. triangle),
///    elements(i,n) index into the nodes array for the node n of element i,
///    for now n=0,1,2 for triangles. Thus element 0 will have nodes (elements(i,0),elements(i,1),...)
///    A value of elements(i,n)==-1 means no node is used. This option is used to specify elements
///     with different numbers of nodes per elements. For example if one has quadrilaterals and
///   triangles then set element(i,3)=-1 for triangles.
/// \param faces (input):
/// \param faceElements_ (input):
/// \param elementFaces_ (input):
///  / numberOfFaces\_ (input) : optionally supply the number of faces, if known. 
///  / numberOfBoundaryFaces\_ (input) : optionally supply the number of boundary faces, if known. 
// ========================================================================================
{
  real timeStart = getCPU();

  // kkc destroy any allocated connectivity arrays
  deleteConnectivity();

  if( domainDimension_==2 || domainDimension_==3 )
    domainDimension=domainDimension_;

  int nd=nodes.getLength(1);
  if( nd==2 || nd==3 )
  {
    rangeDimension=nd;
  }
  else
  {
    {throw "error";}
  }
  
  initMapping();

  maxNumberOfNodesPerElement=elements.getLength(1);
  maxNumberOfNodesPerFace=faces.getLength(1);
  maxNumberOfFacesPerElement=maxNumberOfNodesPerElement;

  numberOfNodesPerElementIsConstant=constantNumberOfNodesPerElement;
  
  numberOfNodes=nodes.getLength(0);
  setGridDimensions( axis1,numberOfNodes );  

  node.redim(0);
  node=nodes;
  Range R(0,numberOfNodes-1);
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
    // printf("*** axis=%i rangeBound=(%e,%e)\n",axis,(real)getRangeBound(Start,axis),(real)getRangeBound(End,axis));
  }

  numberOfElements=elements.getLength(0);
  int inputNumberOfNodesPerElement=elements.getLength(1);

  element.redim(0);
  element=elements;

  numberOfFaces=numberOfFaces_>=0 ? numberOfFaces_ : faces.getLength(0);
  Range F=numberOfFaces, all;
  face.redim(F,maxNumberOfNodesPerFace);
  face(F,all)=faces(F,all);

  if( elementFaces==NULL )
    elementFaces = new intArray;
      
  intArray & ef = *elementFaces;
  ef.redim(0);
  ef=elementFaces_;
    
  faceElements.redim(F,2);
  faceElements(F,all)=faceElements_(F,all);

  // assign boundary faces
  numberOfBoundaryFaces=numberOfBoundaryFaces_>=0 ? numberOfBoundaryFaces_ : numberOfFaces;
  bdyFace.redim(numberOfBoundaryFaces);
  int f, bf = 0;
  for( f=0; f<numberOfFaces; f++ ) 
  {
    if( faceElements(f,1)==-1 ) 
    {
      bdyFace(bf) = f;
      bf++;
    }
  }

  if( numberOfBoundaryFaces!=bf )
  {
    if( numberOfBoundaryFaces!=bf && bf>0)
      bdyFace.resize(bf);
    else if( bf==0 )
      bdyFace.redim(0);
    numberOfBoundaryFaces=bf;
  }

  bdyFaceTags.redim(numberOfBoundaryFaces);
  bdyFaceTags = 0;
  tags.redim(numberOfElements);
  tags = 0;

  numberOfEdges=numberOfFaces;
  edge.reference(face);
  
  // remove the search tree so that it will be rebuilt when needed. 
  if( search!=NULL )
    delete search;
  search=NULL;

  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;

  if ( domainDimension==3 )
    {
      specifyEntity(Region,element);
      //      specifyEntity(Face,face);
    }
  else if ( domainDimension==2 )
    {
      specifyEntity(Face,element);
      //      specifyEntity(Edge,face);
    }

  if( debug & 1 )
  {
    real timeEnd = getCPU();
    real connectivityGenTime = timeEnd - timeStart;
    printf("buildConnectivityLists : numberOfNodes = %i \n", numberOfNodes);
    printf("buildConnectivityLists : numberOfElements = %i \n", numberOfElements);
    printf("buildConnectivityLists : numberOfFaces = %i \n", numberOfFaces);
    printf("buildConnectivityLists : numberOfBoundaryFaces = %i \n", numberOfBoundaryFaces);
    printf("buildConnectivityLists : numberOfEdges = %i \n", numberOfEdges);
    printf("buildConnectivityLists : connectivityGenTime = %e \n", connectivityGenTime);
  }
  

  return 0;
}


int UnstructuredMapping::
intersects(Mapping & map2, 
	   const int & side1 /* =-1 */, 
	   const int & axis1 /* =-1 */,
	   const int & side2 /* =-1 */, 
	   const int & axis2 /* =-1 */,
	   const real & tol /* =0. */ ) const
// =============================================================================
// /Description:
//    Return false if the Mapping's do not intersect, true if they may.
//
// ============================================================================
{
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    real xa1=getRangeBound(Start,axis);
    real xb1=getRangeBound(End,axis);

    real xa2=map2.getRangeBound(Start,axis);
    real xb2=map2.getRangeBound(End,axis);
    if( xa1 > xb2+tol  || xb1 < xa2-tol )
      return false;
  }
  return true;
}


int UnstructuredMapping::
rotate( const int & axis, const real & theta )
//===========================================================================
/// \brief  Perform a rotation about a given axis. This rotation is applied
///    after any existing transformations. 
/// \param axis (input) : axis to rotate about (0,1,2)
/// \param theta (input) : angle in radians to rotate by.
//===========================================================================
{
  if( rangeDimension==1 )
    return 1;
  if( rangeDimension==2 && axis!=axis3 )
  {
    printf("UnstructuredMapping::rotate:ERROR: Can only rotate a rangeDimension==2 around axis==2\n");
    return 1;
  }
  const real ct = cos(theta); 
  const real st = sin(theta); 

  const int i1 = (axis+1) % 3;
  const int i2 = (axis+2) % 3;

  Range R(0,numberOfNodes-1);
  for( int i=0; i<numberOfNodes; i++ )
  {
    real temp = node(i,i1)*ct-node(i,i2)*st;
    node(i,i2)= node(i,i1)*st+node(i,i2)*ct;
    node(i,i1)=temp;
  }

  // -- recompute bounds --
  for( int dir=0; dir<rangeDimension; dir++ )
  {
    setRangeBound(Start,dir,min(node(R,dir)));
    setRangeBound(End  ,dir,max(node(R,dir)));
  }

  // mappingHasChanged();
  return 0;
}

int UnstructuredMapping::
shift(const real & shiftx /* =0. */, 
      const real & shifty /* =0. */, 
      const real & shiftz /* =0.*/ )
//===========================================================================
/// \brief  Shift the NURBS in space.
//===========================================================================
{
  const real shift[3]={shiftx,shifty,shiftz};
  Range R(0,numberOfNodes-1);

  for( int axis=0; axis<rangeDimension; axis++ )
  {
    node(R,axis)=node(R,axis)+shift[axis];
  }

  // -- recompute bounds --
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
  }

  // mappingHasChanged();
  return 0;
}

int UnstructuredMapping::
scale(const real & scalex /* =0. */, 
      const real & scaley /* =0. */, 
      const real & scalez /* =0.*/ )
//===========================================================================
/// \brief  Scale the NURBS in space.
//===========================================================================
{
  const real scale[3]={scalex,scaley,scalez};
  Range R(0,numberOfNodes-1);

  for( int axis=0; axis<rangeDimension; axis++ )
  {
    node(R,axis)=node(R,axis)*scale[axis];
  }

  // -- recompute bounds --
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(node(R,axis)));
    setRangeBound(End  ,axis,max(node(R,axis)));
  }

  // mappingHasChanged();
  
  return 0;
}


int UnstructuredMapping::
buildConnectivityLists()
//===========================================================================
// /Access: protected.
// /Description:
//   Compute the edge and face lists given the nodes and element list.
//\end{UnstructuredMappingImp.tex}
//===========================================================================
{

  real timeStart = getCPU();

  FEZComputeElementTypes();

  // Build a hash table that stores a list of Element faces keyed on the lowest
  // point id in each face. 
  // get an upper estimate on the number of Element faces in the mapping
  int maxNumberOfElementFaces = maxNumberOfFacesPerElement*numberOfElements;

  //  IntegerArray fhash(maxNumberOfNodesPerFace, maxNumberOfElementFaces);
  intArray fhash(maxNumberOfElementFaces, maxNumberOfNodesPerFace);
  // fhash_zfid maintains various additional information about the faces 
  // fhash_zfid(0,f) -- the Element the face belongs to
  // fhash_zfid(1,f) -- adjacent element face
  // fhash_zfid(2,f) -- offset of the face in the owning Element
  // fhash_zfid(3,f) -- number of nodes in this face
  intArray fhash_zfid(4,maxNumberOfElementFaces); 
  intArray fhash_index(2, numberOfNodes); // fhash_index(0,n) -- zero based index into fhash
                                              // fhash_index(1,n) -- number of faces for this key 
                                              
  fhash       = -1;
  fhash_zfid  = -1;
  fhash_index = 0;

  // generate fhash_index, a two step process
  // fhash_index 1:  see how many faces are in each key
  int z,f,n;
  for (z=0; z<numberOfElements; z++) 
  {
    for (f=0; f<getNumberOfFacesThisElement(z); f++)
    {
      int nmin = min(getElementFaceNodes(z,f));
      fhash_index(1, nmin)++;
    }
  }
   
  // fhash_index 2: sum results of step 1 to form offsets into fhash
  fhash_index(0,0) = 0;
  for (n=1; n<numberOfNodes; n++)
  {
    fhash_index(0,n) = fhash_index(0,n-1) + fhash_index(1,n-1);
  }
  
  // now generate fhash
  // zero out fhash_index(1,:) and use it as a counter, it will be rebuilt as fhash is built
  fhash_index(1,Index(0, numberOfNodes)) = 0;
  Index If(0,maxNumberOfNodesPerFace);
  IntegerArray smask(If);
  for (z=0; z<numberOfElements; z++)
  {
    for (f=0; f<getNumberOfFacesThisElement(z); f++)
    {
      // loop over all the faces in element z, adding them to the appropriate hash key and setting zfid values
      const IntegerArray & fpts = getElementFaceNodes(z,f);

      int nmin = min(fpts);

      for (int find=0; find<fpts.getLength(0); find++) fhash(fhash_index(0,nmin) + fhash_index(1,nmin),find) = fpts(find); 
      fhash_zfid(0, fhash_index(0,nmin) + fhash_index(1,nmin)) = z;
      fhash_zfid(2,fhash_index(0,nmin) + fhash_index(1,nmin)) = f;
      smask.redim(fpts.getLength(0));
      smask = 0;
      // count the number of points in this face
      where(fpts>=0) 
	{
	  smask = 1;
	}
      fhash_zfid(3,fhash_index(0,nmin) + fhash_index(1,nmin)) = sum(smask);//sum(fpts>=0);
      fhash_index(1,nmin)++;
    }
  }

  // now generate face connectivity:
  // Currently fhash contains lists of nearby Element faces. To see what a "Element face" is
  // consider the 2D diagram below :
  /*       +********+
           *        *  *  
           *        *     * 
           *  e1  f *f2 e2   +
           *        *     *
           *        *  *
           +********+

     e1 is a quadrilateral and e2 is a triangle.  f and f2 are neigboring element faces.  What 
     we are generating below will be a merging of f and f2 into a singe face data structure
     that has pointers to the adjacent Elements.

  */

  // procedure:
  // 1.  loop through fhash blocks using fhash_index
  // 2.  in each block, search remaining members for a match if one has not already been found
  // 3.  if a match is found, mark it as used, assign face id, go back to 2

  int interiorFaceIndexCounter = 0;
  int boundaryFaceIndexCounter = 0;
  IntegerArray mask(If);
  mask = 0;
  for (n=0; n<numberOfNodes; n++)
  {
    for (f=fhash_index(0,n); f<(fhash_index(0,n)+fhash_index(1,n)); f++)
    {
      bool match = false;
      if (fhash_zfid(1,f)==-1) // if this face has not been considered...
      {
	int z1 = fhash_zfid(0,f);
	const intArray & fpts = fhash(f,If);
	int fuse = -1;
	for (int f2=f+1; f2<(fhash_index(0,n)+fhash_index(1,n)); f2++)
	{ // search the remaining faces in key n
	  int z2 = fhash_zfid(0,f2);
	  // ignore faces in the same Element and faces with different numbers of nodes 
	  if (z1!=z2 && fhash_zfid(3,f)==fhash_zfid(3,f2)) 
	    {  // if f2 is not in the same zone and the number of nodes on f and f2 are the same
	      const intArray & f2pts = fhash(f2,If);
	      // compare points in the face, in the same order, they should all be the same...
	      int nfloc=0;
	      int nf2loc=0;
	      int numNodesCurrentFace = fhash_zfid(3,f);
	      // start face node comparison at the key node, so we need to find out where that is on each face
              int nf;
	      for (nf=0; nf<numNodesCurrentFace; nf++)
		{
		  if (fpts(f,nf) == n) nfloc = nf;
		  if (f2pts(f2,nf) == n) nf2loc = nf;
		}
	      for (nf=0; nf<numNodesCurrentFace; nf++)
		{
		  // starting with the lowest numbered point (n) on each face, loop through the
		  // the faces in the direction defined by the "f" face point list.  In other words,
		  // loop through the "f2" face's point list in a reverse order, starting with the 
		  // location of n.  This will ensure that the points are the same and are oriented consistently.
		  if (fpts(f,(nfloc+nf)%numNodesCurrentFace) != 
		      f2pts(f2,(nf2loc+numNodesCurrentFace-nf)%numNodesCurrentFace))
		    {
		      match = false;
		      break;
		    } else {
		      match = true;
		      fuse = f2;
		    }
		}
	
	    } // end check f2 candidate face for a match
	  if (match) break; // if a match has been found, stop the search
	} // end check other faces in this key
      
	// update information on face connectivity
	fhash_zfid(1,f) = interiorFaceIndexCounter;
	interiorFaceIndexCounter++;
	if (match) 
	  {
	    fhash_zfid(1,fuse) = fhash_zfid(1,f);
	  } else { // must be a boundary face
	    boundaryFaceIndexCounter++;
	  }
      } // end if a match has not been found for f
      
    } // end find a match for face f in this hash key
  } // end loop through keys in fhash

  // now that we have a nice list of faces we can create the faces field of this class...
  numberOfFaces = interiorFaceIndexCounter;
  numberOfBoundaryFaces = boundaryFaceIndexCounter;

  face.redim(numberOfFaces, maxNumberOfNodesPerFace);
  faceElements.redim(numberOfFaces, 2);
  face = 0;
  faceElements = -1;
  faceZ1Offset.redim(numberOfFaces);
  faceZ1Offset = -1;
  // go through fhash again, using fhash_index, and do the assignments into faces and faceElements
  IntegerArray checkNumberOfElementsOnFace(numberOfFaces);  // used in sanity
  checkNumberOfElementsOnFace = 0;

  for (n=0; n<numberOfNodes; n++)
  {
    for (f=fhash_index(0,n); f<(fhash_index(0,n)+fhash_index(1,n)); f++)
    {
      int fglob = fhash_zfid(1,f);
      if (faceElements(fglob,0)!=-1) 
      { // if this global face index has been stored already store the 
	// adjacency information
	faceElements(fglob,1) = fhash_zfid(0,f);
	checkNumberOfElementsOnFace(fglob)++;
      } else {
	// store new global face index
	faceElements(fglob,0) = fhash_zfid(0,f);
	faceZ1Offset(fglob) = fhash_zfid(2,f);
	face(fglob,If) = fhash(f,If);
	checkNumberOfElementsOnFace(fglob)++;
      }
     
    }
  }

  // perform a consistency check, make sure that nothing funny happened in the last loop
  // there should either be 1 or 2 Elements adjacent to a face:
  assert(max(checkNumberOfElementsOnFace)<3 && min(checkNumberOfElementsOnFace)>0);

  // log the boundary faces
  bdyFace.redim(numberOfBoundaryFaces);
  int bdyFcnt = 0;
  for (f=0; f<numberOfFaces; f++) 
  {
    if (faceElements(f,1)==-1) 
      {
	bdyFace(bdyFcnt) = f;
	bdyFcnt++;
	if( bdyFcnt>numberOfBoundaryFaces )
	{
	  printf("UnstructuredMapping:buildConnectivityLists:ERROR: too many boundary faces. something wrong here.\n"
                 " expected number : %i. This could be caused if there is an element with a collapsed edge. \n",
                 numberOfBoundaryFaces);
	  break;
	}
      }
  }
  bdyFaceTags.redim(numberOfBoundaryFaces);
  bdyFaceTags = 0;

  // edge connectivity
  // build ehash which is a hash table containing edges stored by thier smallest node index
  IntegerArray ehash_index(2,numberOfNodes);  // ehash_index(0,n) offset into ehash, ehash_index(1,n) number of edges this node
  IntegerArray ehash;

  ehash_index = 0;

  // loop through each face and count edges
  for (f=0; f<numberOfFaces; f++) 
  {
    int numn = getNumberOfNodesThisFace(f);
    Index If(0, numn);
    const intArray & nds = face(f, If);   // ***** could do better in parallel
    for (n=0; n<numn; n++)
    {
      int nmin = min(nds(f,n), nds(f,(n+1)%numn));
      ehash_index(1,nmin)++;
    }
  }

  // sum elements of ehash_index into offsets into ehash
  ehash_index(0,0) = 0;
  int nhashmax = 0;
  for (n=1; n<numberOfNodes; n++)
  {
    ehash_index(0,n) = ehash_index(0,n-1) + ehash_index(1,n-1);
    nhashmax = max(nhashmax, ehash_index(0,n)+ehash_index(1,n));
  }

  ehash.redim(2,nhashmax);  // ehash(0,n) - opposite node on edge, ehash(1,n) - edge found flag
  ehash=0;

  numberOfEdges=0;
  ehash_index(1, Index(0,numberOfNodes)) = 0;
  for (f=0; f<numberOfFaces; f++)
  {
    int numn = getNumberOfNodesThisFace(f);
    Index If(0, numn);
    const intArray & nds = face(f, If);
    for (n=0; n<numn; n++)
    {
      int nmin = min(nds(f,n), nds(f,(n+1)%numn));
      ehash(0, ehash_index(0,nmin)+ehash_index(1,nmin)) = max(nds(f,n), nds(f,(n+1)%numn));
      ehash_index(1,nmin)++;
    }
  }

  // count the number of actual edges to size this->edge 
  numberOfEdges = 0;
  for (n=0; n<numberOfNodes; n++) 
  {
    int nimax = ehash_index(0,n)+ehash_index(1,n);
    for (int ni=ehash_index(0,n); ni<nimax; ni++)
      { // loop through the edges in this hash key (n)
      if (ehash(1,ni)==0) // have not set an edge for this pair yet
      {
	ehash(1,ni) = 1;
	numberOfEdges++;
	// loop through remaining edges in the key to find matches
	for (int nii=ni+1; nii<nimax; nii++) 
	  if (ehash(0,nii)==ehash(0,ni)) ehash(1,nii)=1;
      }
    }
  }

  edge.redim(numberOfEdges,2);
  ehash(1,Index(0,nhashmax)) = 0;
  int numberOfEdges2 = 0;
  for (n=0; n<numberOfNodes; n++) 
  {
    int nimax = ehash_index(0,n)+ehash_index(1,n);
    for (int ni=ehash_index(0,n); ni<nimax; ni++)
    {
      if (ehash(1,ni)==0) // have not set an edge for this pair yet
      {
	ehash(1,ni) = 1;
	edge(numberOfEdges2,0) = n;
	edge(numberOfEdges2,1) = ehash(0,ni);
	numberOfEdges2++;
	// mark remaining matched edges as found
	for (int nii=ni+1; nii<(ehash_index(0,n)+ehash_index(1,n)); nii++) 
	  if (ehash(0,nii)==ehash(0,ni)) ehash(1,nii)=1;
	
      }
    }
  }
  assert(numberOfEdges == numberOfEdges2);

  real timeEnd = getCPU();

  real connectivityGenTime = timeEnd - timeStart;

  if ( Mapping::debug & 1 )
    {
      printf("buildConnectivityLists : numberOfNodes = %i \n", numberOfNodes);
      printf("buildConnectivityLists : numberOfElements = %i \n", numberOfElements);
      printf("buildConnectivityLists : numberOfFaces = %i \n", numberOfFaces);
      printf("buildConnectivityLists : numberOfBoundaryFaces = %i \n", numberOfBoundaryFaces);
      printf("buildConnectivityLists : numberOfEdges = %i \n", numberOfEdges);
      printf("buildConnectivityLists : connectivityGenTime = %e \n", connectivityGenTime);
    }

  if(domainDimension==2 && numberOfFaces!=numberOfEdges ) 
  {
    printf("***UnstructuredMapping::buildConnectivityLists:ERROR: numberOfFaces=%i != numberOfEdges=%i ***\n",
	   numberOfFaces,numberOfEdges);
    return 1;
  }
  // probably should perfrom geometry and connectivity checks now...

  if( elementFaces!=NULL )
  {
    // The elementfaces array is out of date, remove it so it will be rebuilt the next time it is needed.
    delete elementFaces;
    elementFaces=NULL;
  }

  assert(numberOfBoundaryFaces<=numberOfFaces);
  return 0;
}



// the static methods below are used to compartmentalize the building of unstructured elements
// from other mappings.

// when Overture supports namespaces these should probably be stuck into an unnammed namespace

static void setElement(const int &v0, const int &v1, const int &v2, const int &v3,
		       const intArray &elements, const int &elementID)
{
  
  elements(elementID, 0) = v0;
  elements(elementID, 1) = v1;
  elements(elementID, 2) = v2;
  elements(elementID, 3) = v3;  

}

static void setElement(const int &v0, const int &v1, const int &v2, const int &v3,
		       const int &v4, const int &v5, const int &v6, const int &v7,
		       const intArray &elements, const int &elementID)
{

  elements(elementID, 0) = v0;
  elements(elementID, 1) = v1;
  elements(elementID, 2) = v2;
  elements(elementID, 3) = v3;
  elements(elementID, 4) = v4;
  elements(elementID, 5) = v5;
  elements(elementID, 6) = v6;
  elements(elementID, 7) = v7;

}

static void assignTri(intArray &elements, int elementID, real sj, int v1, int v2, int v3)
{
  if ( sj>0 )
    {
      elements(elementID,0) = v1;
      elements(elementID,1) = v2;
      elements(elementID,2) = v3;
    }
  else
    {
      elements(elementID,2) = v1;
      elements(elementID,1) = v2;
      elements(elementID,0) = v3;
    }
}
  
static void assignQuad(intArray &elements, int elementID, real sj, int v1, int v2, int v3, int v4)
{
  if ( sj>0 )
    {
      elements(elementID,0) = v1;
      elements(elementID,1) = v2;
      elements(elementID,2) = v3;
      elements(elementID,3) = v4;
    }
  else
    {
      elements(elementID,3) = v1;
      elements(elementID,2) = v2;
      elements(elementID,1) = v3;
      elements(elementID,0) = v4;
    }
}

static int 
assembleElements2D(const Mapping & map, const intArray &mapVertexIDs, 
		   const intArray &mask, intArray &elements, intArray &elembdy, bool preferTriangles,int ngc )
{

  // assemble triangles and quadrilaterals from mapping map into the array elements
  
  int elementID = 0;
  int i3 = mask.getBase(2);

  int ib[2],ie[2],per[2];
  for (int a=0; a<2; a++) 
    {
      per[a] = (int)map.getIsPeriodic(a);
      ib[a] = 0;
      ie[a] = map.getGridDimensions(a)-2;
    }
  
  int xa0=ib[0],xb0=ie[0];  // index bounds without ghost points
  int xa1=ib[1], xb1=ie[1];

  int numberOfGhostCells = ngc;//1;
  for ( int a=0; a<2; a++ )
    {
      if ( per[a]!=Mapping::functionPeriodic ) // no need to add ghost cells on a branch cut.
	{
	  ib[a] -= numberOfGhostCells;
	  ie[a] += numberOfGhostCells;
	}
    }
#if 0
  int ie[2],ib[2];
    int ib[3],ie[3],per[3];
  for (int a=0; a<3; a++) 
    {
      per[a] = (int)map.getIsPeriodic(a);
      ib[a] = 0;
      ie[a] = map.getGridDimensions(a)-2;
    }
  
  int xa0=ib[0],xb0=ie[0];  // index bounds without ghost points
  int xa1=ib[1], xb1=ie[1];
  for (int a=0; a<2; a++) 
    {
      ie[a] = map.getGridDimensions(a)-1;
      ib[a] = 0;
    }

  int xa0=ib[0],xb0=ie[0];  // index bounds without ghost points
  int xa1=ib[1], xb1=ie[1];
#endif

  real sj = map.getSignForJacobian();

  for ( int i2=ib[1]; i2<=ie[1]; i2++ )
    {
      for ( int i1=ib[0]; i1<=ie[0]; i1++ )
	{
	  // check the various possibilities for a triangle

	  if ( mask(i1,i2,i3)!=0 && mask(i1+1,i2,i3)!=0 &&
	       mask(i1+1,i2+1,i3)!=0 && mask(i1,i2+1,i3)!=0 )
	    {

	      if ( i1<xa0 || i1>xb0 || i2<xa1 || i2>xb1 )
		elembdy(elementID) = 0x1; // this is a ghost element

	      if ( mapVertexIDs(i1, i2, i3) == mapVertexIDs(i1+1, i2, i3) )
		{ 
 		  assignTri(elements,elementID,sj,mapVertexIDs(i1,i2,i3),mapVertexIDs(i1+1, i2+1, i3),mapVertexIDs(i1, i2+1, i3));
// 		  elements(elementID, 0) = mapVertexIDs(i1,i2,i3);
// 		  elements(elementID, 1) = mapVertexIDs(i1+1, i2+1, i3);
// 		  elements(elementID, 2) = mapVertexIDs(i1, i2+1, i3);
		} 
	      else if ( mapVertexIDs(i1, i2, i3) == mapVertexIDs(i1, i2+1, i3) )
		{
		  assignTri(elements,elementID,sj,mapVertexIDs(i1,i2,i3),mapVertexIDs(i1+1, i2, i3),mapVertexIDs(i1+1, i2+1, i3));
// 		  elements(elementID, 0) = mapVertexIDs(i1,i2,i3);
// 		  elements(elementID, 1) = mapVertexIDs(i1+1, i2, i3);
// 		  elements(elementID, 2) = mapVertexIDs(i1+1, i2+1, i3);
		}
	      else if (mapVertexIDs(i1+1, i2, i3) == mapVertexIDs(i1+1, i2+1, i3) )
		{
		  assignTri(elements,elementID,sj,mapVertexIDs(i1+1,i2,i3),mapVertexIDs(i1, i2+1, i3),mapVertexIDs(i1, i2, i3));
// 		  elements(elementID, 0) = mapVertexIDs(i1+1,i2,i3);
// 		  elements(elementID, 1) = mapVertexIDs(i1, i2+1, i3);
// 		  elements(elementID, 2) = mapVertexIDs(i1, i2, i3);
		}
	      else if (mapVertexIDs(i1, i2+1, i3) == mapVertexIDs(i1+1, i2+1, i3) )
		{
		  assignTri(elements,elementID,sj,mapVertexIDs(i1,i2+1,i3),mapVertexIDs(i1, i2, i3),mapVertexIDs(i1+1, i2, i3));
// 		  elements(elementID, 0) = mapVertexIDs(i1,i2+1,i3);
// 		  elements(elementID, 1) = mapVertexIDs(i1, i2, i3);
// 		  elements(elementID, 2) = mapVertexIDs(i1+1, i2, i3);
		}
	      else // quadrilateral
		{
                  if( preferTriangles )
		  {
		    assignTri(elements,elementID,sj,mapVertexIDs(i1, i2, i3),mapVertexIDs(i1+1, i2, i3),mapVertexIDs(i1+1, i2+1, i3));
// 		    elements(elementID, 0) = mapVertexIDs(i1, i2, i3);
// 		    elements(elementID, 1) = mapVertexIDs(i1+1, i2, i3);
// 		    elements(elementID, 2) = mapVertexIDs(i1+1, i2+1, i3);
                    elementID++;
		    elembdy(elementID) = elembdy(elementID-1);
		    assignTri(elements,elementID,sj,mapVertexIDs(i1, i2, i3),mapVertexIDs(i1+1, i2+1, i3),mapVertexIDs(i1, i2+1, i3));
// 		    elements(elementID, 0) = mapVertexIDs(i1, i2, i3);
//                     elements(elementID, 1) = mapVertexIDs(i1+1, i2+1, i3);
// 		    elements(elementID, 2) = mapVertexIDs(i1, i2+1, i3);	      
		  }
		  else
		  {
		    assignQuad(elements,elementID,sj,mapVertexIDs(i1, i2, i3),mapVertexIDs(i1+1, i2, i3),
			       mapVertexIDs(i1+1, i2+1, i3),mapVertexIDs(i1, i2+1, i3));
// 		    elements(elementID, 0) = mapVertexIDs(i1, i2, i3);
// 		    elements(elementID, 1) = mapVertexIDs(i1+1, i2, i3);
// 		    elements(elementID, 2) = mapVertexIDs(i1+1, i2+1, i3);
// 		    elements(elementID, 3) = mapVertexIDs(i1, i2+1, i3);	      
		  }
		}
	      
	      elementID++;
	    } // if ( mask !=0 )
	} // i1
    } // i2
  //  elements.display();
  return elementID;
}

static int getNumberOfDuplicatedNodes( const intArray & element )
{

  int count = 0;
  for ( int n=0; n<element.getLength(0); n++ )
    {
      int current = element(n);
      for ( int n2=0; n2<element.getLength(0); n2++ )
	if ( n != n2 && current == element(n2) ) 
	  {
	    count++;
	    break;
	  }
    }

  return count;
}

static bool assembledHexahedron( intArray &tempElement, const intArray &elements, 
				 const int &elementID)
{

  // check to make sure none of the nodes are duplicated

  int maxNumberOfNodesPerElement = tempElement.getLength(0);

  for ( int node=0; node<maxNumberOfNodesPerElement; node++ )
    for (int node2=node+1; node2<maxNumberOfNodesPerElement; node2++ )
      if ( tempElement(node) == tempElement(node2) ) return false;

  elements(elementID, Range(0, maxNumberOfNodesPerElement-1)) = tempElement.reshape(1, maxNumberOfNodesPerElement);
  tempElement.reshape(maxNumberOfNodesPerElement);

  return true;

}

static bool assembledTriPrism( const intArray &tempElement, const intArray &elements, 
			       const int &elementID)
{
  bool result = true;

  // getNumberOfDuplicateNodes is compilation unit scoped and defined above
  int nDuplicated = getNumberOfDuplicatedNodes( tempElement );

  if ( nDuplicated != 4 ) return false;

  // there are twelve possible orientations, two for each hex face

  // face 0 is degnerate
  if ( tempElement(4) == tempElement(5) && tempElement(0) == tempElement(1) &&
       tempElement(4) != tempElement(0) )
    {
      setElement(tempElement(0), tempElement(2), tempElement(3), tempElement(4), 
		 tempElement(6), tempElement(7), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(4) == tempElement(0) && tempElement(5) == tempElement(1) &&
	    tempElement(0) != tempElement(1) )
    {
      setElement(tempElement(1), tempElement(6), tempElement(2), tempElement(0), 
		 tempElement(7), tempElement(3), -1            ,  -1           ,
		 elements, elementID);
    }
  // face 1 is degenerate
  else if ( tempElement(2) == tempElement(3) && tempElement(6) == tempElement(7) &&
	    tempElement(2) != tempElement(6) )
    {
      setElement(tempElement(0), tempElement(1), tempElement(2), tempElement(4), 
		 tempElement(5), tempElement(6), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(2) == tempElement(6) && tempElement(3) == tempElement(7) &&
	    tempElement(2) != tempElement(3) )
    {
      setElement(tempElement(5), tempElement(6), tempElement(1), tempElement(4), 
		 tempElement(7), tempElement(0), -1            ,  -1           ,
		 elements, elementID);
    }
  // face 2 is degnerate
  else if ( tempElement(4) == tempElement(7) && tempElement(5) == tempElement(6) &&
	    tempElement(4) != tempElement(5) )
    {
      setElement(tempElement(1), tempElement(5), tempElement(2), tempElement(0), 
		 tempElement(4), tempElement(3), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(4) == tempElement(5) && tempElement(7) == tempElement(6) &&
	    tempElement(4) != tempElement(7) )
    {
      setElement(tempElement(3), tempElement(2), tempElement(6), tempElement(0), 
		 tempElement(1), tempElement(5), -1            ,  -1           ,
		 elements, elementID);
    }
  // face 3 is degnerate
  else if ( tempElement(0) == tempElement(1) && tempElement(3) == tempElement(2) &&
	    tempElement(0) != tempElement(3) )
    {
      setElement(tempElement(0), tempElement(4), tempElement(5), tempElement(3), 
		 tempElement(7), tempElement(6), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(0) == tempElement(3) && tempElement(1) == tempElement(2) &&
	    tempElement(0) != tempElement(1) )
    {
      setElement(tempElement(0), tempElement(7), tempElement(4), tempElement(1), 
		 tempElement(6), tempElement(5), -1            ,  -1           ,
		 elements, elementID);
    }
  // face 4 is degnerate
  else if ( tempElement(1) == tempElement(5) && tempElement(2) == tempElement(6) &&
	    tempElement(1) != tempElement(2) )
    {
      setElement(tempElement(0), tempElement(4), tempElement(5), tempElement(3), 
		 tempElement(7), tempElement(6), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(1) == tempElement(2) && tempElement(5) == tempElement(6) &&
	    tempElement(1) != tempElement(5) )
    {
      setElement(tempElement(0), tempElement(1), tempElement(3), tempElement(4), 
		 tempElement(5), tempElement(7), -1            ,  -1           ,
		 elements, elementID);
    }
  // face 5 is degnerate
  else if ( tempElement(0) == tempElement(4) && tempElement(3) == tempElement(7) &&
       tempElement(0) != tempElement(3) )
    {
      setElement(tempElement(2), tempElement(6), tempElement(7), tempElement(1), 
		 tempElement(5), tempElement(4), -1            ,  -1           ,
		 elements, elementID);
    }
  else if ( tempElement(0) == tempElement(3) && tempElement(4) == tempElement(7) &&
	    tempElement(0) != tempElement(4) ) 
    {
      setElement(tempElement(0), tempElement(1), tempElement(2), tempElement(4), 
		 tempElement(5), tempElement(6), -1            ,  -1           ,
		 elements, elementID);
    }
  else
    result = false;

  return result;

}

static bool assembledPyramid( const intArray &tempElement, const intArray &elements, 
			      const int &elementID)
{
  bool result = true;

  // count the number of times a node is used
  int maxNumberOfNodesPerElement = tempElement.getLength(0);

  // getNumberOfDuplicateNodes is compilation unit scoped and defined above
  int nDuplicated = getNumberOfDuplicatedNodes( tempElement ); 

  if ( nDuplicated != 4 ) return false;

  if ( result )
    {
      // there are six possible orientations, one for each hex face
      if ( tempElement(0) == tempElement(3) && 
	   tempElement(0) == tempElement(7) && 
	   tempElement(0) == tempElement(4) )
	{
	  // axis=2, side=0 is degenerate
	  setElement(tempElement(5), tempElement(6), tempElement(2), tempElement(1), tempElement(0),
		     -1            , -1            , -1            , elements, elementID);
	}
      else if ( tempElement(1) == tempElement(2) && 
		tempElement(1) == tempElement(6) && 
		tempElement(1) == tempElement(5) )
	{
	  // axis=2, side=1 is degenerate
	  setElement(tempElement(0), tempElement(3), tempElement(7), tempElement(4), tempElement(1),
		     -1            , -1            , -1            , elements, elementID);
	}
      else if ( tempElement(0) == tempElement(1) && 
		tempElement(0) == tempElement(2) && 
		tempElement(0) == tempElement(3) )
	{
	  // axis=1, side=0 is degenerate
	  setElement(tempElement(4), tempElement(7), tempElement(6), tempElement(5), tempElement(0),
		     -1            , -1            , -1            , elements, elementID);
	}
      else if ( tempElement(4) == tempElement(5) && 
		tempElement(4) == tempElement(6) && 
		tempElement(4) == tempElement(7) )
	{
	  // axis=1, side=1 is degenerate
	  setElement(tempElement(0), tempElement(1), tempElement(2), tempElement(3), tempElement(4),
		     -1            , -1            , -1            , elements, elementID);
	}
      else if ( tempElement(0) == tempElement(1) && 
		tempElement(0) == tempElement(5) && 
		tempElement(0) == tempElement(4) )
	{
	  // axis=0, side=0 is degenerate
	  setElement(tempElement(2), tempElement(6), tempElement(7), tempElement(3), tempElement(0),
		     -1            , -1            , -1            , elements, elementID);
	}
      else if ( tempElement(3) == tempElement(2) && 
		tempElement(3) == tempElement(6) && 
		tempElement(3) == tempElement(7) )
	{
	  // axis=0, side=1 is degenerate
	  setElement(tempElement(0), tempElement(4), tempElement(5), tempElement(1), tempElement(3),
		     -1            , -1            , -1            , elements, elementID);
	}
      else
	{
	  result = false;
	}
    }

  return result;

}

static bool assembledTetrahedron( const intArray &tempElement, const intArray &elements, 
				  const int &elementID)
{
  bool result = true;

  // there are eight possible orientations, one for each hex node
  // just check each node to see if it is not degenerate, if it isn't, construct a tetrehedron

  if ( tempElement(0) != tempElement(1) &&
       tempElement(0) != tempElement(4) &&
       tempElement(0) != tempElement(3) )

    setElement(tempElement(1), tempElement(4), tempElement(3), tempElement(0), 
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);
  
  else if ( tempElement(1) != tempElement(0) &&
	    tempElement(1) != tempElement(2) &&
	    tempElement(1) != tempElement(5) )

    setElement(tempElement(0), tempElement(2), tempElement(5), tempElement(1),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);
  
  else if ( tempElement(2) != tempElement(1) &&
	    tempElement(2) != tempElement(3) &&
	    tempElement(2) != tempElement(6) )

    setElement(tempElement(1), tempElement(3), tempElement(6), tempElement(2),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);

  else if ( tempElement(3) != tempElement(2) &&
	    tempElement(3) != tempElement(0) &&
	    tempElement(3) != tempElement(7) )

    setElement(tempElement(2), tempElement(0), tempElement(7), tempElement(3),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);

  else if ( tempElement(4) != tempElement(7) &&
	    tempElement(4) != tempElement(5) &&
	    tempElement(4) != tempElement(0) )

    setElement(tempElement(0), tempElement(5), tempElement(7), tempElement(4),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);

  else if ( tempElement(5) != tempElement(4) &&
	    tempElement(5) != tempElement(6) &&
	    tempElement(5) != tempElement(1) )

    setElement(tempElement(4), tempElement(1), tempElement(6), tempElement(5),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);

  else if ( tempElement(6) != tempElement(5) &&
	    tempElement(6) != tempElement(7) &&
	    tempElement(6) != tempElement(2) )

    setElement(tempElement(2), tempElement(7), tempElement(5), tempElement(6),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);

  else if ( tempElement(7) != tempElement(6) &&
	    tempElement(7) != tempElement(4) &&
	    tempElement(7) != tempElement(3) )
    
    setElement(tempElement(3), tempElement(4), tempElement(6), tempElement(7),
	       -1            , -1            , -1            , -1            ,
	       elements, elementID);
  
  else
    {
      result = false;
    }

  return result;

}

static int assembleElements3D(const Mapping & map, const intArray &mapVertexIDs, 
			      const intArray &mask, intArray &elements, intArray &elembdy, 
			      intArray &periodic, intArray &elemMap, bool preferTriangles,int ngc)
{

  // assemble pyramids and hexahedra from mapping map into the array elements

  int maxNumberOfNodesPerElement = elements.getLength(1);

  int elementID = 0;
  int ib[3],ie[3],per[3];
  for (int a=0; a<3; a++) 
    {
      per[a] = (int)map.getIsPeriodic(a);
      ib[a] = 0;
      ie[a] = map.getGridDimensions(a)-2;
    }
  
  int xa0=ib[0],xb0=ie[0];  // index bounds without ghost points
  int xa1=ib[1], xb1=ie[1];
  int xa2=ib[2], xb2=ie[2];

  int numberOfGhostCells = ngc;//1;
  for ( int a=0; a<3; a++ )
    {
      if ( per[a]!=Mapping::functionPeriodic ) // no need to add ghost cells on a branch cut.
	{
	  ib[a] -= numberOfGhostCells;
	  ie[a] += numberOfGhostCells;
	}
    }

  const int nCells0 = ie[0] - ib[0] + 1;
  const int nCells1 = ie[1] - ib[1] + 1;
  const int nCells2 = ie[2] - ib[2] + 1;

  real sj = map.getSignForJacobian();

  for ( int i3=ib[2]; i3<=ie[2]; i3++ )
    {
      for ( int i2=ib[1]; i2<=ie[1]; i2++ )
	{
	  for ( int i1=ib[0]; i1<=ie[0]; i1++ )
	    {
	      if ( mask(i1,i2,i3)!=0 && mask(i1+1,i2,i3) !=0 && 
		   mask(i1+1,i2+1,i3)!=0 && mask(i1,i2+1,i3)!=0 &&
		   mask(i1,i2,i3+1)!=0 && mask(i1+1,i2,i3+1) !=0 && 
		   mask(i1+1,i2+1,i3+1)!=0 && mask(i1,i2+1,i3+1)!=0 )
		{
		  intArray tempHex(maxNumberOfNodesPerElement);
		  if ( sj>0. )
		    {
		      tempHex(0) = mapVertexIDs(i1  ,i2  ,i3  );
		      tempHex(1) = mapVertexIDs(i1+1,i2  ,i3  );
		      tempHex(2) = mapVertexIDs(i1+1,i2+1,i3  );
		      tempHex(3) = mapVertexIDs(i1  ,i2+1,i3  );
		      tempHex(4) = mapVertexIDs(i1  ,i2  ,i3+1);
		      tempHex(5) = mapVertexIDs(i1+1,i2  ,i3+1);
		      tempHex(6) = mapVertexIDs(i1+1,i2+1,i3+1);
		      tempHex(7) = mapVertexIDs(i1  ,i2+1,i3+1);
		    }
		  else
		    {
		      tempHex(3) = mapVertexIDs(i1  ,i2  ,i3  );
		      tempHex(2) = mapVertexIDs(i1+1,i2  ,i3  );
		      tempHex(1) = mapVertexIDs(i1+1,i2+1,i3  );
		      tempHex(0) = mapVertexIDs(i1  ,i2+1,i3  );

		      tempHex(7) = mapVertexIDs(i1  ,i2  ,i3+1);
		      tempHex(6) = mapVertexIDs(i1+1,i2  ,i3+1);
		      tempHex(5) = mapVertexIDs(i1+1,i2+1,i3+1);
		      tempHex(4) = mapVertexIDs(i1  ,i2+1,i3+1);
		    }

		  // the following "assembled*( ... )" methods are compilation unit scoped and are defined above
		  if ( ! assembledHexahedron(tempHex, elements, elementID) ) 
		    if ( ! assembledTriPrism(tempHex, elements, elementID) )
		      if ( ! assembledPyramid(tempHex, elements, elementID) )
			if ( ! assembledTetrahedron(tempHex, elements, elementID) )
			  throw "cannot recognize element type";

		  if ( i1<xa0 || i1>xb0 || i2<xa1 || i2>xb1 || i3<xa2 || i3>xb2 )
		    {

		      if ( per[0]==Mapping::derivativePeriodic ||
			   per[1]==Mapping::derivativePeriodic ||
			   per[2]==Mapping::derivativePeriodic )
			{
			  periodic(elementID,0) = i1;
			  periodic(elementID,1) = i2;
			  periodic(elementID,2) = i3;		      
			  //			  cout<<"PERIODIC MATCH "<<i1<<"  "<<i2<<"  "<<i3<<" to ";
			}
		      
		      elembdy(elementID) = 0x1; // this is a ghost element
		      // ah, but is it periodic?
		      bool isCorner = true;
		      if ( per[0] == Mapping::derivativePeriodic )
			{
			  if ( i1<xa0 )
			    periodic(elementID,0) = i1+nCells0-1-numberOfGhostCells;
			  else if ( i1>xb0 )
			    periodic(elementID,0) = i1-nCells0+1+numberOfGhostCells;
			  else 
			    isCorner = false;
			}
		      else 
			isCorner = false;
		      
		      if ( per[1] == Mapping::derivativePeriodic )
			{
			  if ( i2<xa1 )
			    periodic(elementID,1) = i2+nCells1-1-numberOfGhostCells;
			  else if ( i2>xb1 )
			    periodic(elementID,1) = i2-nCells1+1+numberOfGhostCells;
			  else 
			    isCorner = false;
			}
		      else 
			isCorner = false;

		      if ( per[2] == Mapping::derivativePeriodic )
			{
			  if ( i3<xa2 )
			    periodic(elementID,2) = i3+nCells2-1-numberOfGhostCells;
			  else if ( i3>xb2 )
			    periodic(elementID,2) = i3-nCells2+1+numberOfGhostCells;
			  else 
			    isCorner = false;
			}
		      else 
			isCorner = false;

// 		      if ( per[0]==Mapping::derivativePeriodic ||
// 			   per[1]==Mapping::derivativePeriodic ||
// 			   per[2]==Mapping::derivativePeriodic )
// 			cout<<periodic(elementID,0)<<"  "<<periodic(elementID,1)<<"  "<<periodic(elementID,2)<<endl;

		      if ( isCorner ) 
			cout<<"matched corner "<<i1<<" "<<i2<<" "<<i3<<" to "<<periodic(elementID,0)<<" "<<periodic(elementID,1)<<" "<<periodic(elementID,2)<<endl;
		    }

		  elemMap(i1,i2,i3) = elementID;
		  elementID++;
		} // if ( mask != 0)
	    } // i1
	} // i2
    } // i3
  return elementID;
}

intArray UnstructuredMapping::
buildFromAMapping( Mapping & map, intArray &maskin /* = Overture::nullIntArray() */ )
//===========================================================================
/// \details 
///     Builds an unstructured mapping from another mapping.  There are no duplicate nodes.
///  Degenerate elements occurring from coordinate singularities and periodic boundaries
///  are detected and the appropriate element ( hex, prism, pyramid, tet) is created
///  in the UnstructuredMapping.  For example, a spherical polar mesh will, in general, have
///  all four element types with pyramids at the spherical singularity, tetrahedron
///  connecting the pyramids to the polar axes, prisms along each polar axis and hexahedra everywhere
///  else.  A mask array can optionally be provided to exclude vertices/elements from the new
///  UnstructuredMapping. However, building a new UnstructuredMapping from a masked UnstructuredMapping
///  is NOT yet supported. The implementor is a bit lazy.
/// 
/// \param map (input) : Mapping to use.
/// \param maskin (input) : pointer to a vertex mask array to determine which nodes/elements to use
/// \param Returns : An IntegerArray mapping the vertices in the original Mapping to the vertices in the
///  new UnstructuredMapping.  If the value of the returned array is -1 at any vertex, then that vertex
///  was masked out of the original mapping.
/// \param Comments : Currently the code implements a rather complex algorithm to 
///  assign vertex id's to the boundary nodes.  The complexity of the coding is
///  due to the possibility of polar singularities ( with the possible occurance
///  of a spherical singularity ) as well as periodic boundaries.  These special cases
///  can occur on any side of any coordinate axis in 2 and 3d.  The approach became
///  more complicated than originally intended, there may be a more straightforward
///  way and any suggestions are welcome.
//===========================================================================
{
  intArray mapVertexIDs;
  bool includeGhostElements = this->includeGhostElements;
  int numberOfGhostCells = includeGhostElements ? 1 : 0;
  // beware of shallow copy if expression is false 
  // *wdh* make a reference
  const intArray & mask_ = (maskin.getDataPointer()==Overture::nullIntArray().getDataPointer()) ? intArray() : maskin;
  intArray & mask = (intArray&) mask_;
  
  //  Index::setBoundsCheck (On);
  if( map.getClassName()=="UnstructuredMapping" )
    {
      if ( mask.getLength(0)==0 )  throw "masked build from an UnstructuredMapping not implemented yet";
      *this=(UnstructuredMapping&)map;
      mapVertexIDs.redim(getNumberOfNodes());
      // set the vertex mapping to simply point to the existing vertices
      for ( int nn=0; nn<size(UnstructuredMapping::Vertex)/*getNumberOfNodes()*/; nn++ ) mapVertexIDs(nn) = nn;
      mapVertexIDs.reshape(getNumberOfNodes(), 1, 1);
    }
  else
    {

      // commonly used loop variables
      int axis, side;

      setDomainDimension(map.getDomainDimension());
      setRangeDimension(map.getRangeDimension());

      initMapping();

      Index I1,I2,I3;
      I1 = Range(-numberOfGhostCells,map.getGridDimensions(0)-1+numberOfGhostCells);
      I2 = Range(-numberOfGhostCells,map.getGridDimensions(1)-1+numberOfGhostCells);
      // if ( map.getRangeDimension()==3 ) // *wdh* 070414
      if ( map.getDomainDimension()==3 )
	I3 = Range(-numberOfGhostCells,map.getGridDimensions(2)-1+numberOfGhostCells);
      else
	I3 = Range(0,0);


      //      const realArray & mappingVertices = map.getGrid();
      realArray x;

      if( includeGhostElements )
	{
	  // If we include ghost elements we must re-evaluate the mapping
	  // ************* fix this -- only compute ghost values *************
	  real dr[3]={1.,1.,1.}; 
	  int axis;
	  for( axis=axis1; axis<domainDimension; axis++ )
	    {
	      dr[axis]=1./max(map.getGridDimensions(axis)-1,1);
	      cout<<"DR "<<dr[axis]<<endl;
	    }
	  
	  x.redim(I1,I2,I3,rangeDimension);
	  realArray r(I1,I2,I3,domainDimension);
	  int i1,i2,i3;
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		r(I1,i2,i3,0).seqAdd(dr[axis1]*I1.getBase(),dr[axis1]);
	      if( domainDimension>1 )
		{
		  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    r(i1,I2,i3,1).seqAdd(dr[axis2]*I2.getBase(),dr[axis2]);
		}
	    }
	  if( domainDimension>2 )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		  r(i1,i2,I3,2).seqAdd(dr[axis3]*I3.getBase(),dr[axis3]);
	    }
	  map.mapGrid(r,x);
	  
	}

      const realArray &mappingVertices = includeGhostElements ? x : map.getGrid();

      int boundaryConds[2][3];
      for ( axis=0; axis<rangeDimension; axis++ )
	for ( side=0; side<2; side++ )
	  boundaryConds[side][axis] = map.getBoundaryCondition(side,axis);

      // vertex and element ID counters
      int vertexID = 0;

      // a usefull Range to have
      Range AXES(0, domainDimension-1);

      // Get an upper estimate for the number of nodes in the UnstructuredMapping,;
      //   this will be adjusted if coordinate singularities or  periodic boundaries are present.
      int numberOfNodesMax = 1;
      for (axis=0; axis<domainDimension; axis++)
 	numberOfNodesMax *= mappingVertices.getLength(axis);

      node.redim(numberOfNodesMax, rangeDimension);
      node = 0.0;
      //IntegerArray mapVertexIDs;
#if 0
      mapVertexIDs.redim(mappingVertices.getLength(0), 
			 mappingVertices.getLength(1), 
			 mappingVertices.getLength(2));

      if ( mask.getLength(0) == 0 ) 
	{
	  mask.redim(mappingVertices.getLength(0), 
		     mappingVertices.getLength(1), 
		     mappingVertices.getLength(2));
	  mask = 1;
	}
#else
      mapVertexIDs.redim(I1,I2,I3);
      if ( mask.getLength(0) == 0 )
	{
	  mask.redim(I1,I2,I3);
	  mask = 1;
	}
#endif

      mapVertexIDs = -1;

      // determine the number of elements in the UnstructuredMapping
      numberOfElements = 1;
      for (axis=0; axis<domainDimension; axis++)
	numberOfElements *= mappingVertices.getLength(axis)-1;//map.getGridDimensions(axis)-1;

      if( preferTriangles )
	numberOfElements*= domainDimension==2 ? 2 : 12;   // ***wdh: fix this for 3D***

      cout<<"MAXNNODE "<<numberOfElements<<"  "<<maxNumberOfNodesPerElement<<endl;
      element.redim(numberOfElements, maxNumberOfNodesPerElement);
      element = -1;

      // first assign vertexID's to the vertices in the mapping, starting with the boundaries
      // and then do the interior

      // generate some loop bounds so we can generically loop over each side of each coordinate axis
      // in the mapping. is are the starting indices and ie are the ending indices
      IntegerArray is[3], &i1s=is[0], &i2s=is[1], &i3s=is[2];
      IntegerArray ie[3], &i1e=ie[0], &i2e=ie[1], &i3e=ie[2];
      int i1,i2,i3;

      for (axis=0; axis<3; axis++)
	{
	  is[axis].redim(domainDimension, 2);
	  ie[axis].redim(domainDimension, 2);
	  is[axis] = 0;
	  ie[axis] = 0;
	  if (axis<domainDimension) 
	    {
	      for (int ax2=0; ax2<domainDimension; ax2++)
		{
		  if (axis!=ax2)
		    {
		      if ( map.getIsPeriodic(ax2)==functionPeriodic )
			{
			  ie[axis](ax2, 0) = map.getGridDimensions(axis)-1;
			  ie[axis](ax2, 1) = map.getGridDimensions(axis)-1;
			}
		      else
			{
			  is[axis](ax2, 0) = -numberOfGhostCells;
			  is[axis](ax2, 1) = -numberOfGhostCells;
			  ie[axis](ax2, 0) = map.getGridDimensions(axis)-1+numberOfGhostCells;
			  ie[axis](ax2, 1) = map.getGridDimensions(axis)-1+numberOfGhostCells;
			}
		    }
		  else
		    {
		      if ( map.getIsPeriodic(axis)==functionPeriodic )
			{
			  is[axis](ax2, 1) = map.getGridDimensions(axis)-1;
			  ie[axis](ax2, 1) = map.getGridDimensions(axis)-1; 
			}
		      else
			{
			  is[axis](ax2,0) = -numberOfGhostCells;
			  ie[axis](ax2,0) = -numberOfGhostCells;
			  is[axis](ax2, 1) = map.getGridDimensions(axis)-1+numberOfGhostCells;
			  ie[axis](ax2, 1) = map.getGridDimensions(axis)-1+numberOfGhostCells; 
			}
		    }
		} // ax2
	    } 	  
	} // axis

      // // determine boundary vertex id's, checking for periodicity and coordinate singularities

      // perform this task by examining all the vertices on each side in each axis
      int itot=0, ising=0, iper=0, inorm=0;
      for (axis=0; axis<domainDimension; axis++)
	{
	  for (side=0; side<2; side++)
	    {

	      // check for a spherical singularity along a particular edge, if one exists, fix the node IDs to be the same
	      // this is only performed if side == 1 since side==0 will already have assigned the needed
	      // vertex id
	      if ( side==1 &&
		   map.getTypeOfCoordinateSingularity(0, axis) == Mapping::polarSingularity &&
		   map.getTypeOfCoordinateSingularity(1, axis) == Mapping::polarSingularity )
		{
		  // spherical type singularity, it would be nice to know which direction the coordinate are collapsed
		  if ( max(abs(mappingVertices(i1s(axis, 0), i2s(axis,0), i3s(axis,0),AXES)-mappingVertices(i1s(axis,1), i2s(axis,1), i3s(axis,1),AXES)))<10.*REAL_EPSILON )
		    {
		      // coordintate starts are singular
		      
		      // determine the non-singular direction (this is the one we keep constant)
		      if (max(abs(mappingVertices(i1s(axis,0), i2s(axis,0), i3s(axis,0),AXES)-mappingVertices(i1e(axis,0), i2s(axis,0), i3s(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i1 = i1s(axis,side);
			  for ( i3=i3s(axis,side); i3<=i3e(axis,side); i3++ )
			    for ( i2=i2s(axis,side); i2<=i2e(axis,side); i2++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1s(axis,0), i2s(axis,0), i3s(axis,0));
			}
		      else if (max(abs(mappingVertices(i1s(axis,0), i2s(axis,0), i3s(axis,0),AXES)-mappingVertices(i1s(axis,0), i2e(axis,0), i3s(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i2 = i2s(axis,side);
			  for ( i3=i3s(axis,side); i3<=i3e(axis,side); i3++ )
			    for ( i1=i1s(axis,side); i1<=i1e(axis,side); i1++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1s(axis,0), i2s(axis,0), i3s(axis,0));
			}
		      else if (max(abs(mappingVertices(i1s(axis,0), i2s(axis,0), i3s(axis,0),AXES)-mappingVertices(i1s(axis,0), i2s(axis,0), i3e(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i3 = i3s(axis,side);
			  for ( i2=i2s(axis,side); i2<=i2e(axis,side); i2++ )
			    for ( i1=i1s(axis,side); i1<=i1e(axis,side); i1++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1s(axis,0), i2s(axis,0), i3s(axis,0));
			}

		    }
		  else if ( max(abs(mappingVertices(i1e(axis,0), i2e(axis,0), i3e(axis,0),AXES)-mappingVertices(i1e(axis,1), i2e(axis,1), i3e(axis,1),AXES)))<10.*REAL_EPSILON )
		    {
		      // coordinate ends are singular
		      // determine the non-singular direction (this is the one we keep constant)
		      if (max(abs(mappingVertices(i1e(axis,0), i2e(axis,0), i3e(axis,0),AXES)-mappingVertices(i1s(axis,0), i2e(axis,0), i3e(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i1 = i1e(axis,side);
			  for ( i3=i3s(axis,side); i3<=i3e(axis,side); i3++ )
			    for ( i2=i2s(axis,side); i2<=i2e(axis,side); i2++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1e(axis,0), i2e(axis,0), i3e(axis,0));
			}
		      else if (max(abs(mappingVertices(i1e(axis,0), i2e(axis,0), i3e(axis,0),AXES)-mappingVertices(i1e(axis,0), i2s(axis,0), i3e(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i2 = i2e(axis,side);
			  for ( i3=i3s(axis,side); i3<=i3e(axis,side); i3++ )
			    for ( i1=i1s(axis,side); i1<=i1e(axis,side); i1++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1e(axis,0), i2e(axis,0), i3e(axis,0));
			}
		      else if (max(abs(mappingVertices(i1e(axis,0), i2e(axis,0), i3e(axis,0),AXES)-mappingVertices(i1e(axis,0), i2e(axis,0), i3s(axis,0),AXES)))>10.*REAL_EPSILON )
			{
			  i3 = i3e(axis,side);
			  for ( i2=i2s(axis,side); i2<=i2e(axis,side); i2++ )
			    for ( i1=i1s(axis,side); i1<=i1e(axis,side); i1++ )
			      mapVertexIDs(i1, i2, i3) = mapVertexIDs(i1e(axis,0), i2e(axis,0), i3e(axis,0));
			}
		    }
		}

	      // now check for the general polar singularity and periodic boundary cases
	      for (i3 = i3s(axis, side); i3<=i3e(axis,side); i3++)
		{
		  for (i2 = i2s(axis, side); i2<=i2e(axis,side); i2++)
		    {
		      for (i1 = i1s(axis, side); i1<=i1e(axis,side); i1++)
			{
			  if ( mapVertexIDs(i1,i2,i3)==-1 && mask(i1,i2,i3) != 0 ) // do not alter if already set
			    {
			      // Assignment to node(vertexID, AXES) is not performed outside the if statements
			      // since the if test controls which nodes are copied over.  For example,
			      // the periodicity test prevents the copy of nodes on the '1' side of a periodic boundary.

			      // off edge spherical singularity id assignments
			      // // we may no longer need these first two cases in the if statement
			      if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1s(axis,side), i2s(axis,side), i3s(axis, side), AXES)))<10.*REAL_EPSILON && 
					mapVertexIDs(i1s(axis,side), i2s(axis,side), i3s(axis,side))!=-1)
				{
				  mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1s(axis,side), i2s(axis,side), i3s(axis,side));
				}
			      else if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1e(axis,side), i2e(axis,side), i3e(axis, side), AXES)))<10.*REAL_EPSILON && 
					mapVertexIDs(i1e(axis,side), i2e(axis,side), i3e(axis,side))!=-1)
				{
				  mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1e(axis,side), i2e(axis,side), i3e(axis,side));
				}
			      else if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1e(axis,side%1), i2e(axis,side%1), i3e(axis, side%1), AXES)))<10.*REAL_EPSILON && 
					mapVertexIDs(i1e(axis,side%1), i2e(axis,side%1), i3e(axis,side%1))!=-1)
				{
				  mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1e(axis,side%1), i2e(axis,side%1), i3e(axis,side%1));
				}
			      // polar singularity node id assignments
			      else if ( map.getTypeOfCoordinateSingularity(side,axis) == Mapping::polarSingularity )
				{
				  // this will be different depending on the axis of the singularity, we need an option
				  // for each axis
				  if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1s(axis,side), i2, i3, AXES)))<10.*REAL_EPSILON && axis!=axis1) 
				    {
				      if ( mapVertexIDs(i1e(axis,side), i2, i3)!=-1 )
					mapVertexIDs(i1,i2,i3) =  mapVertexIDs(i1e(axis,side), i2, i3);
				      else if ( i1 == i1s(axis,side) && mapVertexIDs(i1s(side,axis), i2, i3)==-1)
					{
					  mapVertexIDs(i1,i2,i3) = vertexID;
					  for ( int a=0; a<rangeDimension; a++ ) 
					    node(vertexID, a) = mappingVertices(i1,i2,i3, a);
					  vertexID++;
					}
				      else
					mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1s(axis,side), i2, i3);
				      
				    }
				  else if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1, i2s(axis, side), i3, AXES)))<10.*REAL_EPSILON && axis!=axis2) 
				    {
				      if ( mapVertexIDs(i1, i2e(axis,side), i3)!=-1 )
					  mapVertexIDs(i1,i2,i3) =  mapVertexIDs(i1, i2e(axis,side), i3);
				      else if ( i2 == i2s(axis,side) && mapVertexIDs(i1, i2s(axis,side), i3)==-1 )
					{
					  mapVertexIDs(i1,i2,i3) = vertexID;
					  
					  for ( int a=0; a<rangeDimension; a++ ) 
					    node(vertexID, a) = mappingVertices(i1,i2,i3, a);
					  vertexID++;
					  					  
					}
				      else
					mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1, i2s(axis,side), i3);

				    }
				  else if ( max(abs(mappingVertices(i1,i2,i3,AXES)-mappingVertices(i1, i2, i3s(axis, side), AXES)))<10.*REAL_EPSILON && axis!=axis3)
				    {
				      if ( mapVertexIDs(i1, i2, i3e(axis,side) )!=-1 )
					  mapVertexIDs(i1,i2,i3) =  mapVertexIDs(i1, i2, i3e(axis,side));
				      else if ( i3 == i3s(axis,side) && mapVertexIDs(i1, i2, i3s(axis,side))==-1 )
					{
					  mapVertexIDs(i1,i2,i3) = vertexID;
					  for ( int a=0; a<rangeDimension; a++ ) 
					    node(vertexID, a) = mappingVertices(i1,i2,i3, a);
					  vertexID++;
					}
				      else
					mapVertexIDs(i1,i2,i3) = mapVertexIDs(i1, i2, i3s(axis,side));

				    }
				  else
				    {
				      throw "confused polar singularity";
				    }
				  ising++;
				}
                              // *wdh this next stuff only seems to work for functionPeriodic
			      // *wdh* 020517 else if ( map.getIsPeriodic(axis)!=Mapping::notPeriodic ) 
			      else if ( map.getIsPeriodic(axis)==Mapping::functionPeriodic ) 
				{
				  // periodic boundary node id assignment
				  if (side==0)
				    {
				      // if this boundary is periodic, set the vertex array for both ends
				      int axoff[3];
				      for ( int a1=0; a1<3; a1++) 
					if ( axis==a1 )
					  axoff[a1] = ie[a1](axis, 1);
					else
					  axoff[a1] = is[a1](axis, side);
				      
				      // check to make sure the other end has not been assigned by some other axis, side
				      if ( mapVertexIDs(axoff[0]+i1, axoff[1]+i2, axoff[2]+i3) != -1 )
					mapVertexIDs(i1,i2,i3) = mapVertexIDs(axoff[0]+i1, axoff[1]+i2, axoff[2]+i3);
				      else
					{ // no id existed at the other end of the periodic boundary, create a new ID
					  mapVertexIDs(i1,i2,i3) = vertexID;
					  mapVertexIDs(axoff[0]+i1, axoff[1]+i2, axoff[2]+i3) = vertexID;
					  for ( int a=0; a<rangeDimension; a++ ) node(vertexID, a) = mappingVertices(i1,i2,i3, a);
					  vertexID++;
					}
				      iper++;
				    }
				}
			      else 
				{
				  // otherwise we have a normal vertex, set and advance the vertexID
				  for ( int a=0; a<rangeDimension; a++ ) node(vertexID, a) = mappingVertices(i1,i2,i3, a);
				  mapVertexIDs(i1,i2,i3) = vertexID;
				  inorm++;
				  vertexID++;
				}
			    } // if (mapVertexIDs(i1,i2,i3) == -1

			  itot++;
			} // i1
		    } // i2
		} // i3
	    } // side
	} // axis

      int startOfNonBdyVerts = vertexID;

      // // set the vertex id's for the vertices in the middle of the mesh
      // // we *shouldn't* need to worry about singularities here ...
      int ibases[3];
      int ibounds[3];
      for (axis=0; axis<3; axis++)
	{
	  if (axis>=domainDimension)
	    {
	      ibases[axis] = 0;
	      ibounds[axis] = 0;
	    }
	  else
	    {
	      if ( map.getIsPeriodic(axis)==functionPeriodic )
		{
		  ibases[axis] = 1;
		  ibounds[axis] = map.getGridDimensions(axis)-2;
		}
	      else
		{
		  ibases[axis] = 1-numberOfGhostCells;
		  ibounds[axis] = map.getGridDimensions(axis)-2+numberOfGhostCells;
		}
	    }
	}

      for (i3=ibases[2]; i3<=ibounds[2]; i3++)
	{
	  for (i2=ibases[1]; i2<=ibounds[1]; i2++)
	    {
	      for (i1=ibases[0]; i1<=ibounds[0]; i1++)
		{
		  if ( mask(i1,i2,i3) != 0 )
		    { 
		      for ( int a=0; a<rangeDimension; a++ ) node(vertexID, a) = mappingVertices(i1,i2,i3, a);
		      mapVertexIDs(i1,i2,i3) = vertexID++;
		    }
		} // i1
	    } // i2
	} // i3

      // define all the elements, making triangles/tetrahedra for boundary elements that
      //    have coordinate singularities and quadrilaterals/hexahedra for the rest
      intArray elembdy(numberOfElements), periodic(numberOfElements,3);
      elembdy = 0;
      periodic = -1;
      // elembdy = 0x1 - ghost element
      // periodic >-1 are the matching periodic element indices
      // elemMap  maps the cell i1,i2,i3 to its corresponding element id (>-1)
      intArray elemMap(I1,I2,I3);
      elemMap = -1;
      //      elembdy = -1;

      if (domainDimension==2)
	numberOfElements = assembleElements2D(map, mapVertexIDs, mask, element, elembdy, preferTriangles,numberOfGhostCells); // this method is compilation unit scoped and is defined above
      else
	{
	  numberOfElements = assembleElements3D(map, mapVertexIDs, mask, element, elembdy, periodic, elemMap, preferTriangles,numberOfGhostCells); // this method is compilation unit scoped and is defined above
	}

      element.resize(numberOfElements, maxNumberOfNodesPerElement);

      numberOfNodes = vertexID;
      setGridDimensions( axis1,numberOfNodes );  

      node.resize(numberOfNodes, rangeDimension);      

      Range R(0,numberOfNodes-1);
      for( axis=0; axis<rangeDimension; axis++ )
	{
	  setRangeBound(Start,axis,min(node(R,axis)));
	  setRangeBound(End  ,axis,max(node(R,axis)));
	}

      //      buildConnectivityLists();
      // kkc set up the new connectivity
      if ( domainDimension==3 )
	{
	  specifyEntity(Region,element);
	}
      else if ( domainDimension==2 )
	specifyEntity(Face,element);

      UnstructuredMapping::EntityTypeEnum cellType = EntityTypeEnum(domainDimension);
      std::string ghostTag = string("Ghost ")+EntityTypeStrings[cellType].c_str();
      std::string perTag = string("periodic ")+EntityTypeStrings[cellType].c_str();
      for ( int e=0; e<numberOfElements; e++ )
	{
	  if ( elembdy(e) )
	    setAsGhost(cellType,e);
	  if ( periodic(e,0)>-1 )
	    addTag(cellType,e,perTag,(void*)elemMap(periodic(e,0),periodic(e,1),periodic(e,2)));

	}

      entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
      if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
      entityMasks[Vertex] = new intArray(capacity(Vertex));
      *entityMasks[Vertex] = 0;
      
      string bdyvTag = string("boundary ")+EntityTypeStrings[Vertex];
      
      if ( includeGhostElements )
      {
	for ( int v=0; v<startOfNonBdyVerts; v++ )
	{
	  if ( !isGhost(Vertex,v) )
	    setAsGhost(Vertex,v);
	}
      }
      else
      {
	for ( int v=0; v<startOfNonBdyVerts; v++ )
	{
	  if ( !hasTag(Vertex,v,bdyvTag) )
	    addTag(Vertex,v,bdyvTag,0);
	}
	
      }
      
      tagBoundaryEntities(*this);


    } // else if not building from UnstructuredMapping


  return mapVertexIDs;

}


int UnstructuredMapping::
printConnectivity( FILE *file /* stdout */ )
// ================================================================================================
/// \details 
///    
// ============================================================================================
{
  if( numberOfNodes==0 )
    return 0;
  
  const intArray & ef = getElementFaces();
  
  for( int e=0; e<numberOfElements; e++ )
  {
    int nn;
    const int numNodes=getNumberOfNodesThisElement(e);
    fprintf(file,"element %i: nodes=(",e);
    for( nn=0; nn<numNodes; nn++ )
      fprintf(file,"%i,",element(e,nn));
    fprintf(file,") faces=(");
    const int numFaces = getNumberOfFacesThisElement(e);
    int ff;
    for( ff=0; ff<numFaces; ff++ )
    {
      fprintf(file,"%i,",ef(e,ff));
    }
    fprintf(file,") adj elements=(");
    for( ff=0; ff<numFaces; ff++ )
    {
      const int f0=ef(e,ff);
      int ea=faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
      if( ea>=0 )
        fprintf(file,"%i,",ea);
    }
    fprintf(file,")\n");
    
  }
  
  for( int f=0; f<numberOfFaces; f++ )
  {
    fprintf(file,"face %i: nodes (%i,%i) next to elements %i and %i\n",f,face(f,0),face(f,1),
	    faceElements(f,0),faceElements(f,1));
  }
  

  return 0;
}

int UnstructuredMapping::
checkConnectivity( bool printResults /* =true */, 
                   IntegerArray *pBadElements  /* =NULL */ )
// ================================================================================================
/// \details 
///    Perform consistency checks on the connectivity.
/// \param printResults (input): output the results if true.
/// \param pBadElements (input/output) : If not null, return a list of the bad Elements.
/// \param return value: number of errors found.
// ============================================================================================
{
  if( numberOfNodes==0 )
    return 0;
  
  int numberOfErrors=0;
  int numberOfBadElements=0;
  IntegerArray & badElements = pBadElements!=0 ? *pBadElements : Overture::nullIntArray();
  const intArray & ef = getElementFaces();
  
  if( maxNumberOfNodesPerElement==3 )
  {
    // triangles
    for( int e=0; e<numberOfElements; e++ )
    {
      bool errorFound=false;
      const int n0=element(e,0), n1=element(e,1), n2=element(e,2);
      if( n0<0 || n0>=numberOfNodes || n1<0 || n1>=numberOfNodes || n2<0 || n2>=numberOfNodes )
      {
	printf("checkConnectivity:ERROR for element e=%i, nodes=(%i,%i,%i). "
	       "Node numbers out of range, numberOfNodes=%i\n",e,n0,n1,n2,numberOfNodes);
	numberOfErrors++; errorFound=true;
      }
      const int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);    // faces on this element
      if( f0<0 || f0>=numberOfFaces || f1<0 || f1>=numberOfFaces || f2<0 || f2>=numberOfFaces )
      {
	printf("checkConnectivity:ERROR for element e=%i, faces=(%i,%i,%i). "
	       "Face numbers out of range, numberOfFaces=%i\n",e,f0,f1,f2,numberOfNodes);
	numberOfErrors++; errorFound=true;
      }
      else if( !(
	(face(f0,0)==n0 || face(f0,0)==n1) && (face(f0,1)==n0 || face(f0,1)==n1) && face(f0,0)!=face(f0,1) &&
	(face(f1,0)==n1 || face(f1,0)==n2) && (face(f1,1)==n1 || face(f1,1)==n2) && face(f1,0)!=face(f1,1) &&
	(face(f2,0)==n2 || face(f2,0)==n0) && (face(f2,1)==n2 || face(f2,1)==n0) && face(f2,0)!=face(f2,1) ))
      {
	printf("checkConnectivity:ERROR for element e=%i, nodes=(%i,%i,%i) faces=(%i,%i,%i).\n"
	       "   Some faces have nodes that do not belong to the element\n"
	       "   face=%i:nodes=(%i,%i)  face=%i:nodes=(%i,%i) face=%i:nodes=(%i,%i)\n",
	       e,n0,n1,n2,f0,f1,f2,
	       f0,face(f0,0),face(f0,1),f1,face(f1,0),face(f1,1),f2,face(f2,0),face(f2,1));
	numberOfErrors++; errorFound=true;
      }
      if( errorFound && pBadElements!=0 )
      {
	if( numberOfBadElements>badElements.getBound(0) )
	  badElements.resize(max(numberOfBadElements+10,numberOfBadElements*2));
      
	badElements(numberOfBadElements)=e;
	numberOfBadElements++;
      }
    
    }
  }
  else
  {
    // general case
    int nn;
    for( int e=0; e<numberOfElements; e++ )
    {
      bool errorFound=false;
      const int numNodes=getNumberOfNodesThisElement(e);
      for( nn=0; nn<numNodes; nn++ )
      {
	const int n0=element(e,nn);
	if( n0<0 || n0>=numberOfNodes )
	{
	  printf("checkConnectivity:ERROR for element e=%i, node number %i is out of range."
		 " numberOfNodes=%i\n",e,n0,numberOfNodes);
	  numberOfErrors++; errorFound=true;
	}
      }
      const int numFaces=getNumberOfFacesThisElement(e);
      for( int ff=0; ff<numFaces; ff++ )
      {
	const int f0=ef(e,0);
	if( f0<0 || f0>=numberOfFaces )
	{
	  printf("checkConnectivity:ERROR for element e=%i, face %i is out of range. "
		 "numberOfFaces=%i\n",e,f0,numberOfNodes);
	  numberOfErrors++; errorFound=true;
	}
        // check that the nodes that make up each face are found on the element.
        const int numberNodesPerFace=getNumberOfNodesThisFace(f0);
	int numFound=0;
	for( int nf=0; nf<numberNodesPerFace; nf++ )
	{
	  int nf0=face(f0,nf);
	  for( nn=0; nn<numNodes; nn++ )
	  {
	    if( nf0==element(e,nn) ) numFound++;
	  }
	}
	if( numFound!=numberNodesPerFace )
	{
	  printf("checkConnectivity:ERROR for element e=%i, nodes=(",e);
	  for( nn=0; nn<numNodes; nn++ )
            printf("%i,",element(e,nn));
	  printf("). face=%i with nodes=(",f0);
	  for( nn=0; nn<numberNodesPerFace; nn++ )
            printf("%i,",face(f0,nn));
          printf(") does not match the nodes in the element!\n");
	  numberOfErrors++; errorFound=true;
	}
      }

      if( errorFound && pBadElements!=0 )
      {
	if( numberOfBadElements>badElements.getBound(0) )
	  badElements.resize(max(numberOfBadElements+10,numberOfBadElements*2));
      
	badElements(numberOfBadElements)=e;
	numberOfBadElements++;
      }
    
    }
    

  }
  

  for( int f=0; f<numberOfFaces; f++ )
  {
    for( int m=0; m<2; m++ )
    {
      int e=faceElements(f,m);

      if( e>=0 )
      {
	if( e>numberOfElements )
	{
          printf("checkConnectivity:ERROR in faceElements face f=%i, element=%i is too big, numberOfElements=%i\n",
		 f,e, numberOfElements);
          numberOfErrors++;
	}
	bool faceFound=false;
        const int nef=getNumberOfFacesThisElement(e);
	for( int ff=0; ff<nef; ff++ )
	{
	  if( ef(e,ff)==f )
	  {
	    faceFound=true;
	    break;
	  }
	}
	
        // if( !( ef(e,0)==f || ef(e,1)==f || ef(e,2)==f ) )
	if( !faceFound )
	{
          // periodic grids will not pass this test! *** fix this ***
          if( isPeriodic[0]==notPeriodic && isPeriodic[1]==notPeriodic )
	  {
	    printf("checkConnectivity:ERROR in faceElements for face f=%i, element=%i does not have a face f\n",
		   f,e);
	    numberOfErrors++;
	  }
	  
	}
      }
    }
  }
  if( printResults &&  numberOfErrors==0 )
  {
    printf("checkConnectivity:INFO: No errors found\n");
  }
  
  if( pBadElements!=0 )
  {
    if( numberOfBadElements>0 )
      badElements.resize(numberOfBadElements);
    else
      badElements.redim(numberOfBadElements);
  }

  return numberOfErrors;
}

int UnstructuredMapping::
printStatistics(FILE *file /* =stdout */ )
// =========================================================================================
/// \details  print some timing statistics.
// =========================================================================================
{
  fprintf(file,"  ========== Statistics for UnstructuredMapping, nodes=%i, elements=%i, faces=%i =================\n"
         "   Timings:                               seconds    sec/nodes     %%   \n",
           numberOfNodes,numberOfElements,numberOfFaces );

  timing[totalTime]=max(timing[0],REAL_MIN*10.);
  
  aString timingName[numberOfTimings]=
  {
    "total time",
    "time for building sub-surfaces",
    "time for connectivity",
    "time for project (global search)",
    "time for project (local search)",
    "time for stitching"
  };

  int nSpace=40;
  aString dots="............................................................................";
  int i;
  if( timing[totalTime]==0. )
    timing[totalTime]=REAL_MIN;
  for( i=0; i<numberOfTimings; i++ )
    if( timingName[i]!="" )    
      fprintf(file,"%s%s%10.2e  %10.2e  %7.3f\n",(const char*)timingName[i],
         (const char*)dots(0,max(0,int(nSpace-timingName[i].length()))),
	  timing[i],timing[i]/numberOfNodes,100.*timing[i]/timing[totalTime]);


  return 0;
}


int UnstructuredMapping::
buildUnstructuredGrid( Mapping & map, int numberOfGridPoints[2] )
// ===========================================================================================
// /Description:
//   Build an unstructured grid using a triangulation algorithm. use this routine if the
//   Mapping boundaries are poorly behaved so that the grid cells give poor quality triangles.
// ===========================================================================================
{

  TriangleWrapper triangleGridGenerator;

  TriangleWrapperParameters & triangleParameters = triangleGridGenerator.getParameters();
  triangleParameters.saveNeighbourList();
  triangleParameters.saveVoronoi(false);

  triangleParameters.setQuietMode(false);


  // In order to use the 2D triangulation function we convert the 3D grid points
  // x(r0,r1) into 2D arclength coordinates s(r0,r1)

  int nx=numberOfGridPoints[0], ny=numberOfGridPoints[1];
  realArray r(nx,ny,2), x(nx,ny,3), s(nx,ny,2);
  
  real averageArcLength[2];
  
  Range I1=nx, I2=ny;
  int i1,i2;
  for( i2=0; i2<ny; i2++ )
    r(I1,i2,1)=i2/real(ny-1);
  for( i1=0; i1<nx; i1++ )
    r(i1,I2,0)=i1/real(nx-1);
	
  map.mapGrid(r,x);
  
  // ::display(x,"x");
  
  // compute arclength positions (s0,s1) of each grid point.
  s=0.;
  for( i1=1; i1<nx; i1++ )
    s(i1,I2,0)=s(i1-1,I2,0)+ 
      SQRT( SQR(x(i1,I2,0)-x(i1-1,I2,0)) + SQR(x(i1,I2,1)-x(i1-1,I2,1)) + SQR(x(i1,I2,2)-x(i1-1,I2,2)) );
  for( i2=1; i2<ny; i2++ )
    s(I1,i2,1)=s(I1,i2-1,1)+
      SQRT( SQR(x(I1,i2,0)-x(I1,i2-1,0)) + SQR(x(I1,i2,1)-x(I1,i2-1,1)) + SQR(x(I1,i2,2)-x(I1,i2-1,2)) );

  // ::display(s,"s");

  averageArcLength[0]=sum(s(nx-1,I2,0))/ny;
  averageArcLength[1]=sum(s(I1,ny-1,1))/nx;

  // choose the max area for a triangle from the average area of a cell.
  real maximumArea = .5*averageArcLength[0]*averageArcLength[1]/(max((nx-1)*(ny-1),1));
  maximumArea=max(maximumArea,1.e-8);
  triangleParameters.setMaximumArea(maximumArea);
  
  printf("buildUnstructuredGrid: nx=%i ny=%i averageArcLength[0]=%e, averageArcLength[1]=%e, maximumArea=%e \n",
	 nx,ny,averageArcLength[0],averageArcLength[1],maximumArea);

  // Choose nodes and faces from the boundary points of the arclength array

  // First make a list of faces and vertices on the boundaries.

  numberOfNodes=2*(nx-1+ny-1), numberOfFaces=numberOfNodes;
  realArray xyz(numberOfNodes,1,2), xyz2;
  intArray faces(numberOfFaces,2);

  Range F=numberOfFaces;
  faces(F,0).seqAdd(0,1);
  faces(F,1).seqAdd(1,1);
  faces(numberOfFaces-1,1)=0;   // periodic
  
  xyz2.reference(xyz);
  xyz2.reshape(1,numberOfNodes,2);

  I1=Range(0,nx-2); // leave off the last point
  I2=Range(0,ny-2);
  
  Range R2=2, R;
  int ia=0, ib=nx-2;
  R=Range(ia,ib);
  xyz(R,0,R2)=s(I1,0,R2);

  ia=ib+1; ib+=ny-1;   R=Range(ia,ib);
  xyz2(0,R,R2)=s(nx-1,I2,R2);

  ia=ib+1; ib+=nx-1; 
  int i;
  for( i=0; i<nx-1; i++ )
    xyz(ia+i,0,R2)=s(nx-1-i,ny-1,R2);  // reverse order

  ia=ib+1; ib+=ny-1;  
  for( i=0; i<ny-1; i++ )
    xyz2(0,ia+i,R2)=s(0,ny-1-i,R2);    // reverse order
  
  xyz.reshape(numberOfNodes,2);

  // ::display(faces,"faces");
  // ::display(xyz,"xyz");

  triangleGridGenerator.initialize( faces,xyz );

  // Note that there may be new nodes introduced.
  triangleGridGenerator.generate();
      
  const intArray & elements = triangleGridGenerator.generateElementList();
  const realArray & sPoints = triangleGridGenerator.getPoints();
  const intArray & neighbours = triangleGridGenerator.getNeighbours();
  int numberOfTriangles=elements.getLength(0);
  if( false )
  {
    for( int i=0; i<numberOfTriangles; i++ )
    {
      printf(" triangle %i: nodes=(%i,%i,%i) neighbours=(%i,%i,%i)\n",
	     i,elements(i,0),elements(i,1),elements(i,2),neighbours(i,0),neighbours(i,1),neighbours(i,2));
    }
  }
  

  realArray nodes(sPoints.dimension(0),3);

  // Make a DataPointMapping of the arclenght positions
  DataPointMapping dpm;
  dpm.setDataPoints(s,2,2);
  
  r.redim(sPoints.dimension(0),2);
  r=-1.;
  dpm.inverseMap(sPoints,r );  // compute unit square coordinates for the arclength positions.

  map.map( r,nodes );   // compute 3d positions of triangle nodes.

  numberOfFaces=triangleGridGenerator.getNumberOfEdges();
  numberOfBoundaryFaces=triangleGridGenerator.getNumberOfBoundaryEdges();
  setNodesElementsAndNeighbours(nodes,elements,neighbours,
					      numberOfFaces,numberOfBoundaryFaces);




   return 0;
}

   

#define LOCAL_NODE_NUMBER(i1,i2) ((i1-xBase0)+xDim0*(i2-xBase1))
int UnstructuredMapping::
buildFromARegularMapping( Mapping & map, ElementType elementTypePreferred /* =triangle */ )
// ================================================================================================
// /Description:
//     Optimised version to build an unstructured mapping from another mapping. The connectivity
//   information will also be built directly.
//
// /elementTypePreferred (input): Prefer these type of elements
//
// For triangles the connectivity will usually look like:
// \begin{verbatim}
//
//    12    13   14     15 
//    X-----X-----X-----X
//    |13 / |15 / |17 / |
//    | /12 | / 14| /16 |
//   8X-----X-----X-----X11
//    | 7 / | 9 / | 11/ |
//    | / 6 | / 8 | /10 |
//   4X-----X-----X-----X7
//    | 1 / | 3 / | 5 / |
//    | / 0 | / 2 | / 4 |
//    X-----X-----X-----X 
//    0     1     2     3
//
// \end{verbatim}
// For quadrilaterals the connectivity will usually look like:
// \begin{verbatim}
//
//    12    13   14     15 
//    X-----X-----X-----X
//    |     |     |     |
//    | 6   | 7   | 8   |
//   8X-----X-----X-----X11
//    |     |     |     |
//    | 3   | 4   | 5   |
//   4X-----X-----X-----X7
//    |     |     |     |
//    | 0   | 1   | 2   |
//    X-----X-----X-----X 
//    0     1     2     3
//
// \end{verbatim}
// 
//\end{UnstructuredMappingImp.tex}
// ============================================================================================
{
  real time0=getCPU();
  
  initMapping();

  rangeDimension=map.getRangeDimension();
  domainDimension=map.getDomainDimension();

  // for function periodic we should remove duplicate nodes too
  int periodic[3]={false,false,false}; //
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    periodic[axis]=(int)map.getIsPeriodic(axis);  // assumes notPeriodic==0
    if( periodic[axis] ) 
      printf(" buildFromARegularMapping: periodic[%i]=%i\n",axis,periodic[axis]);
    setIsPeriodic(axis,map.getIsPeriodic(axis));
  }
  
  assert( domainDimension==2 );

  int boundaryConds[2][2];
  
  int xBase0 = 0;
  int xBound0=map.getGridDimensions(0)-1;
  int xBase1 = 0;
  int xBound1=map.getGridDimensions(1)-1;

  int xa0=xBase0, xb0=xBound0;  // index bounds without ghost points
  int xa1=xBase1, xb1=xBound1;
  

  int numberOfGhostCells=0;
  if( includeGhostElements )
  {
    numberOfGhostCells=1;
    if( periodic[0]!=functionPeriodic ) // no need to add ghsot cells on a branch cut.
    {
      xBase0-=numberOfGhostCells;
      xBound0+=numberOfGhostCells;
    }
    if( periodic[1]!=functionPeriodic )
    {
      xBase1-=numberOfGhostCells;
      xBound1+=numberOfGhostCells;
    }
  }
  


  int xDim0=xBound0-xBase0+1;
  int xDim1=xBound1-xBase1+1;
  Range I1(xBase0,xBound0), I2(xBase1,xBound1), I3(0,0);

  const realArray & mappingGrid = map.getGrid();  // always do this to compute bounds.
  
  realArray xBound(2,3);
  real xScale=0.;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    xBound(Start,axis)=map.getRangeBound(Start,axis);
    xBound(End,  axis)=map.getRangeBound(End  ,axis);
    setRangeBound(Start,axis,xBound(Start,axis));
    setRangeBound(End  ,axis,xBound(End,  axis));
    xScale=max(xScale,xBound(End,  axis)-xBound(Start,axis));

  }
  // kkc 090407 moved this here from the above loop because it caused a memory overwrite for surface meshes (where domainDimension!=rangeDimension)
  for ( axis=0; axis<domainDimension; axis++ )
    for ( int side=0; side<2; side++ )
      boundaryConds[side][axis] = map.getBoundaryCondition(side,axis);

  // const realArray & x = map.getGrid();
  realArray x;
  if( includeGhostElements )
  {
    // If we include ghost elements we must re-evaluate the mapping
    // ************* fix this -- only compute ghost values *************
    real dr[3]={1.,1.,1.}; 
    int axis;
    for( axis=axis1; axis<domainDimension; axis++ )
      dr[axis]=1./max(map.getGridDimensions(axis)-1,1);

    x.redim(I1,I2,I3,rangeDimension);
    realArray r(I1,I2,I3,domainDimension);
    int i1,i2,i3;
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	r(I1,i2,i3,0).seqAdd(dr[axis1]*I1.getBase(),dr[axis1]);
      if( domainDimension>1 )
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  r(i1,I2,i3,1).seqAdd(dr[axis2]*I2.getBase(),dr[axis2]);
      }
    }
    if( domainDimension>2 )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  r(i1,i2,I3,2).seqAdd(dr[axis3]*I3.getBase(),dr[axis3]);
    }
    map.mapGrid(r,x);

  }
  else
  {
    x.reference(mappingGrid);
  }


  int i1,i2;
  int numberOfGridPoints[2], &numberOfGridPoints0=numberOfGridPoints[0], &numberOfGridPoints1=numberOfGridPoints[1];
  
  numberOfGridPoints0=xBound0-xBase0+1;
  numberOfGridPoints1=xBound1-xBase1+1;
  
  const real epsx = SQR(FLT_EPSILON*10.*xScale);
  const real sqrtEpsx=sqrt(epsx);


  numberOfNodesPerElementIsConstant=true;  // we build a mesh will all elements the same type
  
  if( elementTypePreferred==triangle )
  {
    // ***********************************
    // ***** Build a Triangular Mesh *****
    // ***********************************
    
    bool leftCollapsed=false, rightCollapsed=false, bottomCollapsed=false, topCollapsed=false;


  // ------  first define nodes -----

    if( elementDensityTolerance>0. && rangeDimension==3 )  // determineResolution only for 3d surfaces
    {
      // first determine an appropriate number of grid lines to use

      bool collapsedEdge[2][3];
      real averageArclength[2];
      collapsedEdge[0][0] = collapsedEdge[1][0] =
	collapsedEdge[0][1] = collapsedEdge[1][1] = 
	collapsedEdge[0][2] = collapsedEdge[1][2] = false;

      map.determineResolution(numberOfGridPoints,collapsedEdge,averageArclength,elementDensityTolerance );

      leftCollapsed  =collapsedEdge[0][0];
      rightCollapsed =collapsedEdge[1][0];
      bottomCollapsed=collapsedEdge[0][1];
      topCollapsed   =collapsedEdge[1][1];

      if( !leftCollapsed && !rightCollapsed && !bottomCollapsed && !topCollapsed )
      {
	// check the corners to see that they are nearly square -- if not build an unstructured mapping directly
	// since the boundary curves are probably poorly behaved near corners.
	int nx=xBound0+1, ny=xBound1+1;
	real v0[3], v1[3];
	v0[2] = v1[2] = 0;
	bool cuspFound=false;
	for( int m=0; m<4; m++ )
	{
	  i1=m==0 ? 0 : m==1 ? nx-1 : m==2 ? nx-1 : 0;
	  i2=m==0 ? 0 : m==1 ?    0 : m==2 ? ny-1 : ny-1;
	  int is1=m==0 ? 1 : m==1 ? -1 : m==2 ? -1 : 1;
	  int is2=m==0 ? 1 : m==1 ?  1 : m==2 ? -1 : -1;
	
	  if ( rangeDimension==3 )
	  {
	    v0[0]=x(i1+is1,i2,0,0)-x(i1,i2,0,0), v0[1]=x(i1+is1,i2,0,1)-x(i1,i2,0,1), v0[2]=x(i1+is1,i2,0,2)-x(i1,i2,0,2); 
	    v1[0]=x(i1,i2+is2,0,0)-x(i1,i2,0,0), v1[1]=x(i1,i2+is2,0,1)-x(i1,i2,0,1), v1[2]=x(i1,i2+is2,0,2)-x(i1,i2,0,2); 
	  }
	  else
	  {
	    v0[0]=x(i1+is1,i2,0,0)-x(i1,i2,0,0), v0[1]=x(i1+is1,i2,0,1)-x(i1,i2,0,1);
	    v1[0]=x(i1,i2+is2,0,0)-x(i1,i2,0,0), v1[1]=x(i1,i2+is2,0,1)-x(i1,i2,0,1);
	  }
	  real v0Norm=v0[0]*v0[0]+v0[1]*v0[1]+v0[2]*v0[2];
	  real v1Norm=v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2];
	
	  real dot = (v0[0]*v1[0]+v0[1]*v1[1]+v0[2]*v1[2])/max( REAL_MIN*100.,SQRT(v0Norm*v1Norm ));
	  // printf(" Check for cusp : v0=(%8.2e,%8.2e,%8.2e) v1=(%8.2e,%8.2e,%8.2e) (i1,i2)=(%i,%i) dot=%e\n",
	  //         v0[0],v0[1],v0[2], v1[0],v1[1],v1[2], i1,i2,dot);
	  if( fabs(dot) >.98 )
	  {
	    cuspFound=true;
	    printf(" cusp found at (i1,i2)=(%i,%i) dot=%e\n",i1,i2,dot);
	  }
	}
	if( cuspFound )
	{
	  buildUnstructuredGrid(map, numberOfGridPoints );
	  return 0;
	}
      
      }



      // now re-evaluate the grid points 


//        I1=numberOfGridPoints0;
//        I2=numberOfGridPoints1;
//        realArray r(I1,I2,domainDimension);
//        const real dr0=1./real(numberOfGridPoints0-1);
//        const real dr1=1./real(numberOfGridPoints1-1);
//        for( i1=0; i1<numberOfGridPoints0; i1++ )
//  	r(i1,I2,0)=i1*dr0;
//        for( i2=0; i2<numberOfGridPoints1; i2++ )
//  	r(I1,i2,1)=i2*dr1;
//        node.redim(I1,I2,rangeDimension);
//        map.mapGrid(r,node);

      xBase0 = 0;
      xBound0=numberOfGridPoints0-1;
      xBase1 = 0;
      xBound1=numberOfGridPoints1-1;
      xa0=xBase0, xb0=xBound0;
      xa1=xBase1, xb1=xBound1;

      if( includeGhostElements )
      {
	if( periodic[0]!=functionPeriodic ) // no need to add ghost cells on a branch cut.
	{
	  xBase0-=numberOfGhostCells;
	  xBound0+=numberOfGhostCells;
	}
	if( periodic[1]!=functionPeriodic )
	{
	  xBase1-=numberOfGhostCells;
	  xBound1+=numberOfGhostCells;
	}
      }
      xDim0=xBound0-xBase0+1;
      xDim1=xBound1-xBase1+1;

      I1=Range(xBase0,xBound0);
      I2=Range(xBase1,xBound1);
      real dr[3]={1./real(numberOfGridPoints0-1),1./real(numberOfGridPoints1-1),1.}; 

      node.redim(I1,I2,I3,rangeDimension);
      realArray r(I1,I2,I3,domainDimension);
      int i1,i2,i3;
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  r(I1,i2,i3,0).seqAdd(dr[axis1]*I1.getBase(),dr[axis1]);
	if( domainDimension>1 )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    r(i1,I2,i3,1).seqAdd(dr[axis2]*I2.getBase(),dr[axis2]);
	}
      }
      if( domainDimension>2 )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    r(i1,i2,I3,2).seqAdd(dr[axis3]*I3.getBase(),dr[axis3]);
      }
      map.mapGrid(r,node);



      numberOfNodes=I1.getLength()*I2.getLength();
      node.reshape(Range(numberOfNodes),rangeDimension);

    }
    else
    {
      // use existing nodes.
      node.redim(0);
      node.reference(x);
      numberOfNodes=x.getLength(0)*x.getLength(1)*x.getLength(2);
      node.reshape(Range(numberOfNodes),x.dimension(3));

      // check for collapsed edges  *** this fails for periodic directions.
      int n00=0, n10=numberOfGridPoints0-1, n11=numberOfGridPoints0*numberOfGridPoints1-1, 
	n01=n11-(numberOfGridPoints0-1);
  
      real distBottom,distTop,distLeft,distRight;
      if( rangeDimension==3 )
      {
	distBottom = SQR(node(n10,0)-node(n00,0))+SQR(node(n10,1)-node(n00,1))+SQR(node(n10,2)-node(n00,2));
	distTop    = SQR(node(n11,0)-node(n01,0))+SQR(node(n11,1)-node(n01,1))+SQR(node(n11,2)-node(n01,2));
	distLeft   = SQR(node(n01,0)-node(n00,0))+SQR(node(n01,1)-node(n00,1))+SQR(node(n01,2)-node(n00,2));
	distRight  = SQR(node(n11,0)-node(n10,0))+SQR(node(n11,1)-node(n10,1))+SQR(node(n11,2)-node(n10,2));
      }
      else
      {
	distBottom = SQR(node(n10,0)-node(n00,0))+SQR(node(n10,1)-node(n00,1));
	distTop    = SQR(node(n11,0)-node(n01,0))+SQR(node(n11,1)-node(n01,1));
	distLeft   = SQR(node(n01,0)-node(n00,0))+SQR(node(n01,1)-node(n00,1));
	distRight  = SQR(node(n11,0)-node(n10,0))+SQR(node(n11,1)-node(n10,1));
      }
  
      if( min(distBottom,distTop,distLeft,distRight) < epsx )
      {
	printf("***UnstructuredMapping::buildFromARegularMapping:WARNING: one side is collapsed."
	       " distBottom=%e ,distTop=%e,distLeft=%e,distRight=%e \n",distBottom,distTop,distLeft,distRight);
	if( distBottom< epsx )
	{
	  bottomCollapsed=true;
	  printf("***UnstructuredMapping::buildFromARegularMapping:WARNING: bottom is collapsed\n");
	}
	else if(   distTop<epsx )
	{
	  topCollapsed=true;
	}
	else if( distLeft<epsx )
	{
	  leftCollapsed=true;
	}
	else
	{
	  rightCollapsed=true;
	}
      }

    }
  
    bool collapsedEdge=bottomCollapsed || topCollapsed || leftCollapsed || rightCollapsed;
    if( bottomCollapsed )
    {
      xBase1+=1;
    }
    else if( topCollapsed )
    {
      xBound1-=1;
    }
    else if( leftCollapsed )
    {
      xBase0+=1;
    }
    else if( rightCollapsed )
    {
      xBound0-=1;
    }
    // xDim0=xBound0-xBase0+1;
    // xDim1=xBound1-xBase1+1;

  
    const int numberOfCellsDirection0 = xBound0-xBase0;
    const int numberOfCellsDirection1 = xBound1-xBase1;


    // --- define elements and faces ------
    maxNumberOfNodesPerElement=3;
  
    numberOfElements=2*numberOfCellsDirection0*numberOfCellsDirection1;
    numberOfFaces=3*numberOfCellsDirection0*numberOfCellsDirection1+(numberOfCellsDirection0+numberOfCellsDirection1);
  
    if( collapsedEdge )
    {
      numberOfElements+=(bottomCollapsed || topCollapsed) ? numberOfCellsDirection0 : numberOfCellsDirection1;
      numberOfFaces+=(bottomCollapsed || topCollapsed) ? numberOfCellsDirection0+1 : numberOfCellsDirection1+1;
    }
  

    //  element.redim(numberOfElements,rangeDimension);
    element.redim(numberOfElements,3); // kkc this routine does triangles only
    face.redim(numberOfFaces,2);
    faceElements.redim(numberOfFaces,2);
  

    int numberOfGhostElements=0;
    
    if( includeGhostElements )
    {
      if( elementMask==NULL )
        elementMask=new intArray ;
      elementMask->redim(numberOfElements);
      *elementMask=1; 
     
      numberOfGhostElements=4*(numberOfCellsDirection0+numberOfCellsDirection1-2);
      if( periodic[0]==functionPeriodic )
	numberOfGhostElements-=4*(numberOfCellsDirection1-1);
      if( periodic[1]==functionPeriodic )
	numberOfGhostElements-=4*(numberOfCellsDirection0-1);
      
      if( ghostElements==NULL )
        ghostElements=new intArray ;
      ghostElements->redim(numberOfGhostElements);

      if( faceMask==NULL )
        faceMask=new intArray ;
      faceMask->redim(numberOfFaces);
      *faceMask=1;

      if( edgeMask==NULL )
        edgeMask=new intArray ;
      edgeMask->reference(*faceMask);  // edges and faces are the same for rangeDimension==2

    }


    const int offset = 0;  // - LOCAL_NODE_NUMBER(xBase0,xBase1);

    int e0,e1;
    int e=0;   // counts elements
    int f=0;   // counts faces
    int eg=0;  // counts ghost elements
    
    for( i2=xBase1; i2<xBound1; i2++ )
    {
      int i2p= periodic[1]==functionPeriodic ? (i2+1) % numberOfCellsDirection1 : i2+1;  // wrap for periodic grids
      for( i1=xBase0; i1<xBound0; i1++ )
      {
	int i1p= periodic[0]==functionPeriodic ? (i1+1) % numberOfCellsDirection0 : i1+1; 
	// convert each quad into two triangles.
	//
	//         X-----------X
	//         |  e1     / |
	//      |  |       /   |
	//      f2 |     f2    |
	//      |  |   /       |
	//      v  | /    e0   |
	//         X-----------X
	//             - f0 ->
	e0=e;
	element(e0,0)=offset+LOCAL_NODE_NUMBER(i1,i2);
	element(e0,1)=offset+LOCAL_NODE_NUMBER(i1p,i2);
	element(e0,2)=offset+LOCAL_NODE_NUMBER(i1p,i2p);
	e++;
	e1=e;
	element(e1,0)=offset+LOCAL_NODE_NUMBER(i1,i2);
	element(e1,1)=offset+LOCAL_NODE_NUMBER(i1p,i2p);
	element(e1,2)=offset+LOCAL_NODE_NUMBER(i1,i2p);
	e++;

	if( includeGhostElements &&
	    (i1<xa0 || i1>=xb0 || i2<xa1 ||  i2>=xb1 ) )
	{
          if( includeGhostElements )
	  {
	    (*elementMask)(e0)=-1;            // this is a ghost element
            (*ghostElements)(eg,0)=e0;
            eg++;
	    (*elementMask)(e1)=-1;            // this is a ghost element
            (*ghostElements)(eg,0)=e1;
            eg++;
	    assert( eg<=numberOfGhostElements );

            // For periodic ghost elements we remember the periodic image element
            if( periodic[0]==Mapping::derivativePeriodic )
	    {
              if( i1<xa0 )
	      {
                (*elementMask)(e0)=-(e0  + 2*(numberOfCellsDirection0-2)) -2;  // subtract 2 so value is always < -1 
                (*elementMask)(e1)=-(e1  + 2*(numberOfCellsDirection0-2)) -2;  // subtract 2 so value is always < -1 
		// printf(" left: e0=%i ep=%i numberOfCellsDirection0=%i\n",e0,-(*elementMask)(e0)-2,numberOfCellsDirection0);
	      }
	      else if( i1>=xb0 )
	      {
                (*elementMask)(e0)=-(e0   - 2*(numberOfCellsDirection0-2)) -2;
                (*elementMask)(e1)=-(e1   - 2*(numberOfCellsDirection0-2)) -2;
	      }
	    }
	    if( periodic[1]==Mapping::derivativePeriodic )
	    {
	      if( i2<xa1 )
	      {
                (*elementMask)(e0)=-(e0  + 2*numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;
                (*elementMask)(e1)=-(e1  + 2*numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;
	      }
              else if( i2>=xb1 )
	      {
                (*elementMask)(e0)=-(e0  - 2*numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;
                (*elementMask)(e1)=-(e1  - 2*numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;
	      }
	      
              if( periodic[0]==Mapping::derivativePeriodic && (i2<xa1 || i2>=xb1) && (i1<xa0 || i1>=xb0) )
	      {
		// corner : and periodic in both directions
                if( i1<xa0 )
		{
                  (*elementMask)(e0)+= -2*(numberOfCellsDirection0-2);
                  (*elementMask)(e1)+= -2*(numberOfCellsDirection0-2);
                  // printf(" corner: e0=%i ep=%i\n",e0,-(*elementMask)(e0)+2);
		}
		else
		{
                  (*elementMask)(e0)+= +2*(numberOfCellsDirection0-2);
                  (*elementMask)(e1)+= +2*(numberOfCellsDirection0-2);
		}
		
	      }
	    }
	  }
	}


	face(f,0)=element(e0,0);   // face on bottom of e0
	face(f,1)=element(e0,1);
        if( includeGhostElements && (i1<xa0 || i1>=xb0 || i2<xa1  ) )
          (*faceMask)(f)=-1;

	faceElements(f,0)=e0;
	if( i2==xBase1 )
	{
	  faceElements(f,1)= periodic[1]==functionPeriodic ? e0+ 2*numberOfCellsDirection0*(numberOfCellsDirection1-1)+1 :
	    max(e0-2*numberOfCellsDirection0+1,-1);  // -1 == no neighbouring element
	  // faceElements(f,1)=max(e0-2*numberOfCellsDirection0+1,-1);  // -1 == no neighbouring element
	}
	else
	  faceElements(f,1)=e0-2*numberOfCellsDirection0+1;  // -1 == no neighbouring element
	f++;

	face(f,0)=element(e0,2);  // diagonal face
	face(f,1)=element(e0,0);
        if( includeGhostElements && (i1<xa0 || i1>=xb0 || i2<xa1 || i2>=xb1 ) )
          (*faceMask)(f)=-1;

	faceElements(f,0)=e0;
	faceElements(f,1)=e1;
	f++;

	face(f,0)=element(e1,2);    // face to the left of e1
	face(f,1)=element(e1,0);
        if( includeGhostElements && (i1<xa0 || i2<xa1 || i2>=xb1 ) )
          (*faceMask)(f)=-1;

	faceElements(f,0)=e1;
	if( i1==xBase0 )
	  faceElements(f,1)= periodic[0]==functionPeriodic ? e0+ 2*numberOfCellsDirection0-2 : -1;
	else
	  faceElements(f,1)= e1-3;
	f++;

      }
    }
  
    // ---- define faces on right and top
    const int rightFaceStart=f;
    e0=2*numberOfCellsDirection0-2;
    for( i2=xBase1; i2<xBound1; i2++ )
    {
      face(f,0)=element(e0,1);   // face to right of e0 (order NB)
      face(f,1)=element(e0,2);
      if( includeGhostElements && periodic[0]!=functionPeriodic )
	(*faceMask)(f)=-1;

      faceElements(f,0)=e0;
      faceElements(f,1)= periodic[0]==functionPeriodic ? e0-2*numberOfCellsDirection0+3: -1;
      f++;

      e0+=2*numberOfCellsDirection0;
    }
  
    const int topFaceStart=f;
    e0=2*numberOfCellsDirection0*(numberOfCellsDirection1-1)+1;
    for( i1=xBase0; i1<xBound0; i1++ )
    {
     
      face(f,0)=element(e0,1);   // face above e0    (order NB)    
      face(f,1)=element(e0,2);
      if( includeGhostElements  && periodic[1]!=functionPeriodic)
	(*faceMask)(f)=-1;

      faceElements(f,0)=e0;
      faceElements(f,1)= periodic[1]==functionPeriodic ? e0-2*numberOfCellsDirection0*(numberOfCellsDirection1-1)-1 :  -1;
      f++;

      e0+=2;
    }
    int topFaceEnd=f-1;

  // ----- add elements faces on the collapsed edge. ----
    int bFace1=-1, bFace2=-1;   // 2 extra boundary faces on the collapsed edge.
    if( collapsedEdge )
    {
      if( bottomCollapsed || topCollapsed ) 
      {
	int m1,m2,nc,fn,fs;
	if( bottomCollapsed )
	{
	  i2=xBase1;
	  m1=2, m2=1;
	  nc = offset+LOCAL_NODE_NUMBER(xBase0,i2-1);  // collapsed node.
	  fn=0; fs=3;                           // neighbouring face
	}
	else
	{
	  i2=xBound1;
	  m1=1, m2=2;
	  nc = offset+LOCAL_NODE_NUMBER(xBase0,i2+1);  // collapsed node.
	  fn=topFaceStart; fs=1;
	}
      
	bFace1=f;
	for( i1=xBase0; i1<xBound0; i1++ )
	{
	  e0=e;
	  element(e0, 0)=offset+LOCAL_NODE_NUMBER(i1,i2);
	  element(e0,m1)=offset+LOCAL_NODE_NUMBER(i1+1,i2);
	  element(e0,m2)=offset+nc;

	  e++;

	  if( topCollapsed && i1==xBase0 )
	  {
	    // reverse order of nodes on first element so that each boundary node will
	    // appear as the first entry in the face array : i.e.  bndry nodes are face(.,0)
	    face(f,0)=element(e0,m2);     // face to collapsed node m2==collapsed node
	    face(f,1)=element(e0,0);
	  }
	  else
	  {
	    face(f,0)=element(e0,0);     // face to collapsed node
	    face(f,1)=element(e0,m2);
	  }
	
	  faceElements(f,0)=e0;
	  faceElements(f,1)=i1==xBase0 ? -1 : e0-1;  // -1 == no neighbouring element
	  f++;

	  faceElements(fn,1)=e0;     fn+=fs;  // face adjacent to existing elements

	}
	bFace2=f;
	if( bottomCollapsed )
	{
	  face(f,0)=element(e0,m2);   
	  face(f,1)=element(e0,m1);
	}
	else
	{
	  face(f,0)=element(e0,m1);   
	  face(f,1)=element(e0,m2);
	}
      
	faceElements(f,0)=e0;
	faceElements(f,1)=-1;
	f++;

      
      }
      else
      { // left or right collapsed.

	int m1,m2,nc,fn,fs;
	if( leftCollapsed )
	{
	  i1=xBase0;
	  m1=1, m2=2;
	  nc = offset+LOCAL_NODE_NUMBER(i1-1,xBase1);  // collapsed node.
	  fn=2; fs=3*numberOfCellsDirection0;                   // neighbouring face
	}
	else
	{
	  i1=xBound0;
	  m1=2, m2=1;
	  nc = offset+LOCAL_NODE_NUMBER(i1+1,xBase1);  // collapsed node.
	  fn=rightFaceStart; fs=1;
	}
      
	bFace1=f;
	for( i2=xBase1; i2<xBound1; i2++ )
	{
	  e0=e;
	  element(e0, 0)=offset+LOCAL_NODE_NUMBER(i1,i2);
	  element(e0,m1)=offset+LOCAL_NODE_NUMBER(i1,i2+1);
	  element(e0,m2)=offset+nc;

	  e++;

	  if( leftCollapsed && i2==xBase1 )
	  {
	    // reverse order of nodes in first face (see explanation above)
	    face(f,0)=element(e0,m2);     // face to collapsed node
	    face(f,1)=element(e0,0);
	  }
	  else
	  {
	    face(f,0)=element(e0,0);     // face to collapsed node
	    face(f,1)=element(e0,m2);
	  }
	
	  faceElements(f,0)=e0;
	  faceElements(f,1)=i2==xBase1 ? -1 : e0-1;  // -1 == no neighbouring element
	  f++;

	  faceElements(fn,1)=e0;     fn+=fs;  // face adjacent to existing elements

	}
	bFace2=f;
	if( rightCollapsed )
	{
	  face(f,0)=element(e0,m2);   
	  face(f,1)=element(e0,m1);
	}
	else
	{
	  face(f,0)=element(e0,m1);   
	  face(f,1)=element(e0,m2);
	}
      
	faceElements(f,0)=e0;
	faceElements(f,1)=-1;
	f++;
      }
    
    }


    assert( e==numberOfElements );
    assert( numberOfFaces==f );


    // boundary faces 
    //  ** order the boundary faces in counter-clockwise order
    numberOfBoundaryFaces=2*(numberOfCellsDirection0+numberOfCellsDirection1);
    if( bottomCollapsed || topCollapsed )
      numberOfBoundaryFaces+=2-numberOfCellsDirection0;
    else if( leftCollapsed || rightCollapsed )
      numberOfBoundaryFaces+=2-numberOfCellsDirection1;
  
    if( periodic[0] )
      numberOfBoundaryFaces-=numberOfCellsDirection1*2;
    if( periodic[1] )
      numberOfBoundaryFaces-=numberOfCellsDirection0*2; 

    bdyFace.redim(numberOfBoundaryFaces);
    bdyFaceTags.redim(numberOfBoundaryFaces);

    int bdyFcnt = 0;
    if( !bottomCollapsed && !periodic[1] )
    {
      for( i1=xBase0; i1<xBound0; i1++ )
      {
	bdyFace(bdyFcnt) = 3*(i1-xBase0);
	bdyFaceTags(bdyFcnt) = boundaryConds[0][1];
	bdyFcnt++;
      }
    }
    else if( bottomCollapsed )
    {
      bdyFace(bdyFcnt)=bFace1; bdyFaceTags(bdyFcnt) = boundaryConds[0][0]; bdyFcnt++;
      bdyFace(bdyFcnt)=bFace2; bdyFaceTags(bdyFcnt) = boundaryConds[1][0]; bdyFcnt++;
    }
    if( !rightCollapsed && !periodic[0] )
    {
      for( i2=xBase1; i2<xBound1; i2++ )
      {
	bdyFace(bdyFcnt) = rightFaceStart+i2-xBase1;
	bdyFaceTags(bdyFcnt) = boundaryConds[1][0];
	bdyFcnt++;
      }
    }
    else if( rightCollapsed )
    {
      bdyFace(bdyFcnt)=bFace1; bdyFaceTags(bdyFcnt) = boundaryConds[0][1]; bdyFcnt++;
      bdyFace(bdyFcnt)=bFace2; bdyFaceTags(bdyFcnt) = boundaryConds[1][1]; bdyFcnt++;
    }
  
    if( !topCollapsed && !periodic[1] )
    {
      for( i1=xBase0; i1<xBound0; i1++ )
      {
	bdyFace(bdyFcnt) = topFaceEnd;
	bdyFaceTags(bdyFcnt) = boundaryConds[1][1];
	bdyFcnt++;  topFaceEnd--;
      }
    }
    else if( topCollapsed )
    {
      bdyFace(bdyFcnt)=bFace2; bdyFaceTags(bdyFcnt) = boundaryConds[0][0]; bdyFcnt++;
      bdyFace(bdyFcnt)=bFace1; bdyFaceTags(bdyFcnt) = boundaryConds[1][0]; bdyFcnt++;
    }
    if( !leftCollapsed && !periodic[0] )
    {
      int leftFaceEnd=3*numberOfCellsDirection0*(numberOfCellsDirection1-1)+3-1;
      for( i2=xBase1; i2<xBound1; i2++ )
      {
	bdyFace(bdyFcnt) = leftFaceEnd;
	bdyFaceTags(bdyFcnt) = boundaryConds[0][0];
	bdyFcnt++;
	leftFaceEnd-=3*numberOfCellsDirection0;
      }  
    }
    else if( leftCollapsed )
    {
      bdyFace(bdyFcnt)=bFace2; bdyFaceTags(bdyFcnt) = boundaryConds[0][1]; bdyFcnt++;
      bdyFace(bdyFcnt)=bFace1; bdyFaceTags(bdyFcnt) = boundaryConds[1][1]; bdyFcnt++;
    }
  
    // ::display(bdyFace,"bdyFace");
  

    assert( bdyFcnt==numberOfBoundaryFaces );

  
  }
  else if( elementTypePreferred==quadrilateral )
  {
    
    // ********************************
    // *** Build a mesh from quads ****
    // ********************************

    node.redim(0);
    //kkc 050809    node.reference(x);
    numberOfNodes=x.getLength(0)*x.getLength(1)*x.getLength(2);
    //kkc 050809    node.reshape(Range(numberOfNodes),x.dimension(3));
    node= x;
    node.reshape(Range(numberOfNodes),x.dimension(3));
    
    const int numberOfCellsDirection0 = xBound0-xBase0;
    const int numberOfCellsDirection1 = xBound1-xBase1;


    // --- define elements and faces ------
  
    numberOfElements=numberOfCellsDirection0*numberOfCellsDirection1;
    numberOfFaces=2*numberOfCellsDirection0*numberOfCellsDirection1+(numberOfCellsDirection0+numberOfCellsDirection1);
  

    maxNumberOfNodesPerElement=4;
    element.redim(numberOfElements,maxNumberOfNodesPerElement); 
    face.redim(numberOfFaces,2);
    faceElements.redim(numberOfFaces,2);
  
    numberOfBoundaryFaces=2*(numberOfCellsDirection0+numberOfCellsDirection1);
    if( periodic[0]==functionPeriodic ) numberOfBoundaryFaces-=2*numberOfCellsDirection1;
    if( periodic[1]==functionPeriodic ) numberOfBoundaryFaces-=2*numberOfCellsDirection0;
    
    bdyFace.redim(numberOfBoundaryFaces);
    bdyFaceTags.redim(numberOfBoundaryFaces);

    int bdyFcnt = 0;
    int numberOfGhostElements=0;
    if( includeGhostElements )
    {
      if( elementMask==NULL )
        elementMask=new intArray ;
      elementMask->redim(numberOfElements);
      *elementMask=1; 
     
      numberOfGhostElements=2*(numberOfCellsDirection0+numberOfCellsDirection1-2);
      if( periodic[0]==functionPeriodic )
        numberOfGhostElements-=2*(numberOfCellsDirection1-1);
      if( periodic[1]==functionPeriodic )
        numberOfGhostElements-=2*(numberOfCellsDirection0-1);
      
      
      if( ghostElements==NULL )
        ghostElements=new intArray ;
      ghostElements->redim(numberOfGhostElements);

      if( faceMask==NULL )
        faceMask=new intArray ;
      faceMask->redim(numberOfFaces);
      *faceMask=1;

      if( edgeMask==NULL )
        edgeMask=new intArray ;
      edgeMask->reference(*faceMask);  // edges and faces are the same for rangeDimension==2

    }
    

    const int offset = 0;  // - LOCAL_NODE_NUMBER(xBase0,xBase1);  

    int e0;
    int e=0;   // counts elements
    int f=0;   // counts faces
    int eg=0;  // counts ghost elements
    for( i2=xBase1; i2<xBound1; i2++ )
    {
      int i2p= periodic[1]==functionPeriodic ? (i2+1) % numberOfCellsDirection1 : i2+1;  // wrap for periodic grids
      for( i1=xBase0; i1<xBound0; i1++ )
      {
	int i1p= periodic[0]==functionPeriodic ? (i1+1) % numberOfCellsDirection0 : i1+1; 
	// convert each quad into a quad
	//
	//         X-----------X
	//         |           |
	//      |  |           |
	//      f2 |    e0     |
	//      |  |           |
	//      v  |           |
	//         X-----------X
	//             - f0 ->
	e0=e;
	element(e0,0)=offset+LOCAL_NODE_NUMBER(i1,i2);
	element(e0,1)=offset+LOCAL_NODE_NUMBER(i1p,i2);
	element(e0,2)=offset+LOCAL_NODE_NUMBER(i1p,i2p);
	element(e0,3)=offset+LOCAL_NODE_NUMBER(i1,i2p);
	e++;
	
	face(f,0)=element(e0,0);   // face on bottom of e0
	face(f,1)=element(e0,1);

	faceElements(f,0)=e0;
        if( includeGhostElements && (i1<xa0 || i1>=xb0 || i2<xa1  ) )
          (*faceMask)(f)=-1;

	if( includeGhostElements &&
	    (i1<xa0 || i1>=xb0 || i2<xa1 ||  i2>=xb1 ) )
	{
          if( includeGhostElements )
	  {
	    (*elementMask)(e0)=-1;            // this is a ghost element
            (*ghostElements)(eg,0)=e0;
            eg++;
	    

            // For periodic ghost elements we remember the periodic image element
            if( periodic[0]==derivativePeriodic )
	    {
              if( i1<xa0 )
	      {
                (*elementMask)(e0)=-(e0+ numberOfCellsDirection0-2) -2;  // subtract 2 so value is always < -1 
		// printf(" left: e0=%i ep=%i numberOfCellsDirection0=%i\n",e0,-(*elementMask)(e0)-2,numberOfCellsDirection0);
	      }
	      else if( i1>=xb0 )	      
                (*elementMask)(e0)=-(e0- numberOfCellsDirection0+2) -2;
	    }
	    if( periodic[1]==derivativePeriodic )
	    {
	      if( i2<xa1 )
                (*elementMask)(e0)=-(e0+ numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;
              else if( i2>=xb1 )
                (*elementMask)(e0)=-(e0- numberOfCellsDirection0*(numberOfCellsDirection1-2)) -2;

              if( periodic[0]==derivativePeriodic && (i2<xa1 ||  i2>=xb1) &&
                  (i1<xa0 || i1>=xb0 ) )
	      {
		// corner : and periodic in both directions
                if( i1<xa0 )
		{
                  (*elementMask)(e0)+= -(numberOfCellsDirection0-2);
                  // printf(" corner: e0=%i ep=%i\n",e0,-(*elementMask)(e0)+2);
		}
		else
                  (*elementMask)(e0)+= +(numberOfCellsDirection0-2);
	      }
	    }
	  }
	}
//          int ep= (*elementMask)(e0);
//  	ep=-ep-2;
//  	assert( ep<numberOfElements );
	

	if( i2==xBase1 )
	{
          if( periodic[1]==functionPeriodic )
	  {
	    faceElements(f,1)=  e0+ numberOfCellsDirection0*(numberOfCellsDirection1-1);
	  }
	  else
	  {
	    faceElements(f,1)=-1;  // -1 == no neighbouring element
            bdyFace(bdyFcnt)=f; bdyFcnt++;
	  }
	  
	}
	else
	  faceElements(f,1)=e0-numberOfCellsDirection0;  
	f++;

	//	face(f,0)=element(e0,3);    // face to the left of e0
	//	face(f,1)=element(e0,0);
	face(f,0)=element(e0,3);    // face to the left of e0
	face(f,1)=element(e0,0);
        if( includeGhostElements && (i1<xa0 || i2<xa1 ||  i2>=xb1 ) )
          (*faceMask)(f)=-1;


	faceElements(f,0)=e0;
	if( i1==xBase0 )
	{
          if( periodic[0]==functionPeriodic )
	    faceElements(f,1)= e0+ numberOfCellsDirection0-1;
          else
	  {
	    faceElements(f,1)= -1;
	    bdyFace(bdyFcnt)=f; bdyFcnt++;
	  }
	}
	else
	  faceElements(f,1)= e0-1;
	f++;

        if( i1==xBound0-1 )
	{
          // add a face on the far right side
	  face(f,0)=element(e0,1);  
	  face(f,1)=element(e0,2);
          if( includeGhostElements )
            (*faceMask)(f)=-1;

          faceElements(f,0)=e0;
          if( periodic[0]==functionPeriodic )
            faceElements(f,1)=e0-numberOfCellsDirection0+1;
          else
	  {
            faceElements(f,1)=-1;
	    bdyFace(bdyFcnt)=f; bdyFcnt++;
	  }
          f++;
	}

#if 1
        if( i2==xBound1-1 )
	{
	  // add a face on the top
	  face(f,0)=element(e0,2);  
	  face(f,1)=element(e0,3);
          if( includeGhostElements )
            (*faceMask)(f)=-1;

          faceElements(f,0)=e0;
          if( periodic[1]==functionPeriodic )
            faceElements(f,1)=e0-numberOfCellsDirection0*(numberOfCellsDirection1-1);
          else
	  {
            faceElements(f,1)=-1;
	    bdyFace(bdyFcnt)=f; bdyFcnt++;
	  }
          f++;
	}
#endif

      }
    }
//     for ( i1=xBase0; i1<xBound0; i1++ )
//       {
// 	// add a face on the top
// 	e0 = e;
// 	face(f,0)=element(e0,2);  
// 	face(f,1)=element(e0,3);
// 	if( includeGhostElements )
// 	  (*faceMask)(f)=-1;
	
// 	faceElements(f,0)=e0;
// 	if( periodic[1]==functionPeriodic )
// 	  faceElements(f,1)=e0-numberOfCellsDirection0*(numberOfCellsDirection1-1);
// 	else
// 	  {
//             faceElements(f,1)=-1;
// 	    bdyFace(bdyFcnt)=f; bdyFcnt++;
// 	  }
// 	f++;
//       }

    assert( bdyFcnt==numberOfBoundaryFaces );
    
  
  }  // end build a quad mesh
  else
  {
    printf("UnstructureMapping::buildFromAregularMapping:ERROR: unable to build a mesh with element type = %i\n",
      (int)elementTypePreferred);
    throw "error";
  }


  // Fill in a special negative number in the faceElements array for boundary nodes
  // This gives a mapping from faceElemenst to the bdyface array.
  for( int i=0; i<numberOfBoundaryFaces; i++ )
  {
    // printf(" bdyFace(%i)=%i\n",i,bdyFace(i));
    
    if( faceElements(bdyFace(i),1)==-1 )
    {
      faceElements(bdyFace(i),1)=-( i+2 );       // Use this value to go from the faceElements -> bdyFace
    }
    else
    {
      printf("ERROR: bdyFace(%i)=%i, faceElements(%i)=%i but should be -1 \n",i,bdyFace(i),
             bdyFace(i),faceElements(bdyFace(i),1) );
      // assert( faceElements(bdyFace(i),1)==-1 );
    }
    
    if( debug & 4 )
      printf("Bndy face %i is face=%i adjacent elements = (%i,%i) \n",i,bdyFace(i),
	     faceElements(bdyFace(i),0),faceElements(bdyFace(i),1));
  }


  numberOfEdges=numberOfFaces;
  edge.reference(face);


  if( elementFaces!=NULL )
  {
    // The elementfaces array is out of date, remove it so it will be rebuilt the next time it is needed.
    delete elementFaces;
    elementFaces=NULL;
  }


  intArray & ef = (intArray&) getElementFaces();
  

  
  // ***** look for triangles that are inside-out  ****
  if( elementTypePreferred==triangle && rangeDimension==3 )
  {
    for( int e=0; e<numberOfElements-1; e+=2 )
    {
      int e0=e;
      int e1=e+1;
      real nv1[3], nv2[3];
      getNormal( e0,nv1 );
      getNormal( e1,nv2 );
      real dot = nv1[0]*nv2[0]+nv1[1]*nv2[1]+nv1[2]*nv2[2];
      if( dot<0. )
      {
	printf("**WARNING** normals on elements %i and %i are in opposite directions\n",e0,e1);

        // try flipping the diagonal
	//        n3    f2    n2
        //         X-----------X
        //         |  e1     / |
        //         |       /   |
        //      f3 |     f     f1
        //         |   /       |
        //         | /    e0   |
        //      n0 X-----f0----X n1
        // 
	//        n3    f2    n2
        //         X-----------X
        //         |\          |
        //         |  \   e1   |
        //      f3 |    f      f1
        //         | e0   \    |
        //         |        \  |
        //      n0 X-----f0----X n1

        int n1=element(e0,1);
        int n3=element(e1,2);

	// element(e0,0)=n0;
	// element(e0,1)=n1; 
	element(e0,2)=n3;

	element(e1,0)=n1;
	// element(e1,1)=n2;
	// element(e1,2)=n3;

        int f=ef(e0,2);

        int f0=ef(e0,0);
        int f1=ef(e0,1);
        int f2=ef(e1,1);
        int f3=ef(e1,2);

	face(f,0)=n1;
	face(f,1)=n3;
	
        ef(e0,0)=f0;
        ef(e0,1)=f;
        ef(e0,2)=f3;

        ef(e1,0)=f1;
        ef(e1,1)=f2;
        ef(e1,2)=f;

        if( faceElements(f3,0)==e1 )
	  faceElements(f3,0)=e0;
        else
	  faceElements(f3,1)=e0;

        if( faceElements(f1,0)==e0 )
	  faceElements(f1,0)=e1;
        else
	  faceElements(f1,1)=e1;

	getNormal( e0,nv1 );
	getNormal( e1,nv2 );
	real dot = nv1[0]*nv2[0]+nv1[1]*nv2[1]+nv1[2]*nv2[2];
	if( dot<0. )
	{
	  printf("**ERROR** normals are STILL wrong after flipping the diagonal\n");
	}
	else
	{
	  printf("----> Normals are ok after flipping the diagonal\n");
	}
      
      }
    }
  }
  

  numberOfNodes=node.getLength(0);          // *wdh* 020515
  setGridDimensions( axis1,numberOfNodes );  

  if( debug & 1 )
  {
    real connectivityGenTime = getCPU() - time0;
    printf("buildFromARegularMapping : numberOfNodes = %i \n", numberOfNodes);
    printf("buildFromARegularMapping : numberOfElements = %i \n", numberOfElements);
    printf("buildFromARegularMapping : numberOfFaces = %i \n", numberOfFaces);
    printf("buildFromARegularMapping : numberOfBoundaryFaces = %i \n", numberOfBoundaryFaces);
    printf("buildFromARegularMapping : numberOfEdges = %i \n", numberOfEdges);
    printf("buildFromARegularMapping : connectivityGenTime = %e \n", connectivityGenTime);
  }

  tags.redim(numberOfElements);
  tags = 0;

  // kkc set up the new connectivity
  //     note that this code assumes that the implied orientations given above
  //     are the same as given by buildConnectivityLists.

  deleteConnectivity(); //delete old versions of the "new" connectity representation
  //  entitySize[Vertex] = entityCapacity[Vertex] = node.getLength(0);
  entityCapacity[Vertex] = entitySize[Vertex] = numberOfNodes;
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;
      

  if ( nodeMask )
    for ( int n=0; n<numberOfNodes; n++ )
      if ( (*nodeMask)(n)<0 )
	setAsGhost(Vertex,n);


  char *faceElementOrient = new char[2*numberOfFaces];
  char *elementFaceOrient = new char[elementFaces->getLength(0)*elementFaces->getLength(1)];

  intArray faceElemOffset(numberOfFaces+1);
  faceElemOffset(0) = 0;

  intArray faceElemCompressed(numberOfFaces*2);
  int nfc = 0;

  for ( int f=0; f<numberOfFaces; f++ )
    {

      faceElemCompressed(nfc) = faceElements(f,0);
      faceElementOrient[nfc] = 0x1;
      if ( faceElements(f,1)>-1 )
	{
	  faceElemCompressed(nfc+1) = faceElements(f,1);
	  faceElementOrient[nfc+1] = 0x0;
	  nfc+=2;
	}
      else
	nfc++;
      
      faceElemOffset(f+1) = nfc;
    }
  
  faceElemCompressed.resize(nfc);

  for ( int e=0; e<numberOfElements; e++ )
    {
      int fe=0;
      while ( fe<maxNumberOfNodesPerElement && (*elementFaces)(e,fe)>-1 )
	{
	  int f = (*elementFaces)(e,fe);
	  if ( face(f,0)==element(e,fe) )//faceElements(f,0)==e )
	    elementFaceOrient[e*maxNumberOfNodesPerElement + fe] = 0x1;
	  else
	    elementFaceOrient[e*maxNumberOfNodesPerElement + fe] = 0x0;

	  fe++;
	}
    }

  if ( domainDimension==3 )
    {
      specifyEntity(Region,element);
      specifyEntity(Face,face);
      specifyConnectivity(Face, Region, faceElemCompressed,faceElementOrient, faceElemOffset);
      specifyConnectivity(Region, Face, *elementFaces,elementFaceOrient);

      if ( includeGhostElements )
	{
	  for ( int f=0; f<numberOfFaces; f++ )
	    if ( (*faceMask)(f)<0 )
	      setAsGhost(Face, f);
	  
	  for ( int e=0; e<numberOfElements; e++ )
	    if ( (*elementMask)(e)<0 )
	      {
		setAsGhost(Region, e);
		int ep = -(*elementMask)(e)-2;
		if ( ep>=0 )
		  {
		    void *tagData = (void*)ep;
		    addTag(Region,e,std::string("periodic ")+EntityTypeStrings[int(Region)].c_str(),tagData);
		  }
	      }
	}
    }
  else if ( domainDimension==2 )
    {
      if ( element.getLength(1)<4 )
	{
	  intArray nElem(element.getLength(0),4);
	  nElem = -1;
	  Range R1(element.getLength(0)), R2(element.getLength(1));
	  nElem(R1,R2) = element(R1,R2);
	  specifyEntity(Face, nElem);
	}
      else
	specifyEntity(Face,element);

      specifyEntity(Edge,face);

      specifyConnectivity(Edge, Face, faceElemCompressed,faceElementOrient, faceElemOffset);
      specifyConnectivity(Face,Edge, *elementFaces,elementFaceOrient);

      if ( includeGhostElements )
	{
	  for ( int f=0; f<numberOfFaces; f++ )
	    if ( (*faceMask)(f)<0 )
	      setAsGhost(Edge, f);
	  
	  for ( int e=0; e<numberOfElements; e++ )
	    if ( (*elementMask)(e)<0 )
	      {
		setAsGhost(Face, e);
		int ep = -(*elementMask)(e)-2;
		if ( ep>=0 )
		  {
		    void *tagData = (void*)ep;
		    addTag(Face,e,std::string("periodic ")+EntityTypeStrings[int(Face)].c_str(),tagData);
		  }
	      }
	}

      // set up the edge->vertex orientation array
      adjacencyOrientation[ Edge ][ Vertex ] = new char[ 2*numberOfFaces ];

      for ( int f=0; f<numberOfFaces; f++ )
	{
	  adjacencyOrientation[ Edge ][ Vertex ] [ 2*f ] = 0x1;
	  adjacencyOrientation[ Edge ][ Vertex ] [ 2*f +1 ] = 0x0;
	}
    }

  // these were copied in setConnectivity
  delete [] faceElementOrient;
  delete [] elementFaceOrient;

  //kkc  tagBoundaryEntities(*this);

  entitySize(Vertex)=entityCapacity(Vertex)=numberOfNodes;
  entitySize(Edge  )=entityCapacity(Edge  )=domainDimension==2 ? numberOfFaces    : numberOfEdges;
  entitySize(Face  )=entityCapacity(Face  )=domainDimension==2 ? numberOfElements : numberOfFaces;
  entitySize(Region)=entityCapacity(Region)=domainDimension==2 ? 0                : numberOfElements;
  
#if 0
  entityMasks[Vertex]=nodeMask;
  entityMasks[Edge]  =domainDimension==2 ? faceMask    : edgeMask;
  entityMasks[Face]  =domainDimension==2 ? elementMask : faceMask;
  entityMasks[Region]=domainDimension==2 ? NULL        : elementMask;
#endif

  if( debug & 1 )
  {
    printConnectivity();
    checkConnectivity();
  }
  
  return 0;
}



int UnstructuredMapping::
get( const aString & fileName ) 
//===========================================================================
/// \details 
///    Read the unstructured grid from an ascii file.
/// \param fileName (input) : name of the file to save the results in.
//===========================================================================
{

  DataFormats::readIngrid(*this, fileName);
  
#if 0
  // kkc no longer needed here !

    // kkc set up the new connectivity
  if ( domainDimension==3 )
    specifyEntity(Region,element);
  else if ( domainDimension==2 )
    specifyEntity(Face,element);

  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;
#endif

  return 0;
}

int UnstructuredMapping::
put( const aString & fileName ) const
//===========================================================================
/// \details 
///    Save the unstructured grid to an ascii file.
/// \param fileName (input) : name of the file to save the results in.
//===========================================================================
{
  
  UnstructuredMapping & umap = *((UnstructuredMapping *)this);
  DataFormats::writeIngrid(umap, fileName);

  return 0;
}



void UnstructuredMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  cout << "UnstructuredMapping::map: Error: This function should not be called\n";
  {throw "error";}
}

//=================================================================================
// get a mapping from the database
//=================================================================================
int UnstructuredMapping::
get( const GenericDataBase & dir, const aString & name)
{
  FEZInitializeConnectivity();  // kkc this should be done for any mesh, it sets up the templates


  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  Mapping::get( subDir, "Mapping" ); //kkc 050309 domainDimension is needed for connectivity info

  if( debug & 4 )
    cout << "Entering UnstructuredMapping::get" << endl;

  subDir.get( UnstructuredMapping::className,"className" ); 
  if( UnstructuredMapping::className != "UnstructuredMapping" )
  {
    cout << "UnstructuredMapping::get ERROR in className!" << endl;
  }
  subDir.get(numberOfNodes,"numberOfNodes");
  subDir.get(numberOfElements,"numberOfElements");

  subDir.get(maxNumberOfNodesPerElement,"maxNumberOfNodesPerElement");
  subDir.get(maxNumberOfNodesPerFace,"maxNumberOfNodesPerFace");
  subDir.get(maxNumberOfFacesPerElement,"maxNumberOfFacesPerElement");
  subDir.get(numberOfInternalElements,"numberOfInternalElements");
  subDir.get(numberOfInternalFaces,"numberOfInternalFaces");
  subDir.get(numberOfBoundaryElements,"numberOfBoundaryElements");
  subDir.get(numberOfBoundaryFaces,"numberOfBoundaryFaces");
  subDir.get(numberOfEdges,"numberOfEdges");
  
  subDir.getDistributed(node,"node");
  subDir.getDistributed(element,"element");
  subDir.getDistributed(face,"face");
  subDir.getDistributed(edge,"edge");
  subDir.getDistributed(tags, "tags");
  subDir.get(preferTriangles,"preferTriangles");
  subDir.get(elementDensityTolerance,"elementDensityTolerance");

  numberOfFaces = face.getLength(0);

  subDir.getDistributed(bdyFace,"bdyFace");
  subDir.getDistributed(bdyFaceTags,"bdyFaceTags");
  subDir.getDistributed(faceElements,"faceElements");
  int elementFacesSaved=elementFaces!=NULL;
  subDir.get(elementFacesSaved,"elementFacesSaved");
  if( elementFacesSaved )
  {
    if( elementFaces==NULL )
      elementFaces = new intArray;
    subDir.getDistributed(*elementFaces,"elementFaces");
  }
  
  subDir.get(numberOfElementTypes,"numberOfElementTypes");
  subDir.getDistributed(elementType,"elementType");
  subDir.getDistributed(faceZ1Offset,"faceZ1Offset");

  // read the tagging info...

  // kkc get the new connectivity stuff

  aString highestEnt = EntityTypeStrings[domainDimension];
  if (entities[domainDimension]) delete entities[domainDimension];
  entities[domainDimension] = new intArray;
  aString nmv=highestEnt + "Vertices";
  aString nms=highestEnt + "Size";
  subDir.getDistributed( *entities[domainDimension], nmv);
  subDir.get( entitySize[domainDimension], nms);
  entityCapacity[domainDimension] = entities[domainDimension]->getLength(0);
  if ( entityMasks[domainDimension] ) delete entityMasks[domainDimension];
  entityMasks[domainDimension] = new intArray;
  aString nmm = highestEnt + "Mask";
  subDir.getDistributed( *entityMasks[domainDimension], nmm);

  //  entities[domainDimension]->display("entities read in");

  for ( int e=0; e<entitySize[domainDimension]; e++ )
    if ( (*entityMasks[domainDimension])(e) & GhostEntity )
      setAsGhost(EntityTypeEnum(domainDimension), e);

  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray;
  subDir.getDistributed( *entityMasks[Vertex],"VertexMask");
  subDir.get( entitySize[Vertex], "VertexSize");
  entityCapacity[Vertex] = node.getLength(0);
  for ( int e=0; e<entitySize[Vertex]; e++ )
    if ( (*entityMasks[Vertex])(e) & GhostEntity )
      setAsGhost(EntityTypeEnum(Vertex), e);

  subDir.get(dumpTagsToHDF,"dumpTagsToHDF");
  if ( dumpTagsToHDF ) {
    int numberOfTags=0;
  
    subDir.get(numberOfTags,"numberOfTags");
    if ( numberOfTags ) 
      {
	aString *tagNames = new aString[numberOfTags];
	
	subDir.get(tagNames,"tagNames", numberOfTags);
	intSerialArray tagdata;//(numberOfTags,3); // 0-type, 1-index, 2-data, 3-copiesData, 4-datasize
	subDir.get(tagdata,"tagData");
	assert(tagdata.getLength(0)==numberOfTags);
	//	tagdata.display("GET TAGDATA");
	for ( int i=0; i<numberOfTags; i++ )
	  {
	    std::string tmp = tagNames[i].c_str();
	    assert(tmp.length());
	    string::size_type sloc = tmp.find("__ws__");
	    //	    if ( sloc!=string::npos )
	    //	      tmp.replace(sloc,6," ");
	    while ( (sloc=tmp.find("__ws__"))!=string::npos )
	      tmp.replace(sloc,6," ");

	    string tagNm = tmp.c_str();
	    //	    cout<<"adding tag '"<<tagNm<<"' to "<<EntityTypeStrings[tagdata(i,0)]<<"  "<<int(tagdata(i,1))<<endl;
	    //	    cout<<tagdata(i,0)<<"  "<<tagdata(i,1)<<endl;
	    
	    addTag(EntityTypeEnum(tagdata(i,0)),int(tagdata(i,1)), tagNm,
		   (void *)tagdata(i,2), (bool)tagdata(i,3),
		   int(tagdata(i,4)));
	  }	  
	
	// #if 0
	// 	  GenericDataBase &tagDir = *subDir.virtualConstructor();
	// 	  subDir.find(tagDir,tagNames[i],"Tag");
	// 	  tagDir.get(tagNm,"tagName");
	// 	  tagDir.get(tagEntityType,"tagEntityType");
	// 	  tagDir.get(tagEntity,"tagEntity");
	// 	  EntityTag tag;
	// 	  tag.get(tagDir,tagNames[i]);//kkc	  tag.get(tagDir,tagNm);
	// 	  //	  std::string tmp = tagNm.c_str();
	// 	  //	  tmp.replace(tmp.find("__ws__"),6," ");
	// 	  //	  tagNm = tmp.c_str();
	// 	  addTag(UnstructuredMapping::EntityTypeEnum(tagEntityType),tagEntity,tagNm.c_str(),tag.getData(), tag.copiesData(), tag.getDataSize());
	// 	  delete &tagDir;  // is this needed here?
	// #endif
	delete [] tagNames;
      }
    
  }
  
  mappingHasChanged();

#if 0
  if ( domainDimension==3 )
    {
      specifyEntity(Region,element);
      //	  specifyEntity(Face,face);
    }
  else if ( domainDimension==2 )
    {
      specifyEntity(Face,element);
      //	  specifyEntity(Edge,face);
    }
  
  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);
  if ( entityMasks[Vertex] ) delete entityMasks[Vertex];
  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;
#endif
 
  if ( !dumpTagsToHDF ) 
    tagBoundaryEntities(*this);
  
  delete &subDir;

  // sync up the old connectivity
  if ( entities[domainDimension] )
    {
      element.redim(0);
      element = *entities[domainDimension];
      numberOfElements = size( EntityTypeEnum(domainDimension) );
    }

  if ( elementType.getLength(0)==0 )
    FEZComputeElementTypes();
  // *wdh* buildConnectivityLists(); 
  return 0;
}

int UnstructuredMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  Mapping::put( subDir, "Mapping" ); // kkc 050309 Mapping needs to be read first because the connectivity depends on the domainDim
  subDir.put( UnstructuredMapping::className,"className" );

  subDir.put(numberOfNodes,"numberOfNodes");
  subDir.put(numberOfElements,"numberOfElements");

  subDir.put(maxNumberOfNodesPerElement,"maxNumberOfNodesPerElement");
  subDir.put(maxNumberOfNodesPerFace,"maxNumberOfNodesPerFace");
  subDir.put(maxNumberOfFacesPerElement,"maxNumberOfFacesPerElement");
  subDir.put(numberOfInternalElements,"numberOfInternalElements");
  subDir.put(numberOfInternalFaces,"numberOfInternalFaces");
  subDir.put(numberOfBoundaryElements,"numberOfBoundaryElements");
  subDir.put(numberOfBoundaryFaces,"numberOfBoundaryFaces");
  subDir.put(numberOfEdges,"numberOfEdges");

  subDir.putDistributed(node,"node");

  subDir.putDistributed(element,"element");
  subDir.putDistributed(face,"face");
  subDir.putDistributed(edge,"edge");
  subDir.putDistributed(tags,"tags");
  subDir.put(preferTriangles,"preferTriangles");
  subDir.put(elementDensityTolerance,"elementDensityTolerance");

  subDir.putDistributed(bdyFace,"bdyFace");
  subDir.putDistributed(bdyFaceTags,"bdyFaceTags");
  subDir.putDistributed(faceElements,"faceElements");
  int elementFacesSaved=elementFaces!=NULL;
  subDir.put(elementFacesSaved,"elementFacesSaved");
  if( elementFacesSaved )
    subDir.putDistributed(*elementFaces,"elementFaces");

  subDir.put(numberOfElementTypes,"numberOfElementTypes");
  subDir.putDistributed(elementType,"elementType");
  subDir.putDistributed(faceZ1Offset,"faceZ1Offset");

  // write the new connectivity stuff, this should replace the old stuff
  // note that we currently write out a minimal amount of info and depend upon
  // the connectivity routines to reconstruct the other entities as requested
  aString highestEnt = EntityTypeStrings[domainDimension];
  subDir.putDistributed( *entities[domainDimension], highestEnt+"Vertices");
  subDir.put( entitySize[domainDimension], highestEnt+"Size");
  subDir.putDistributed( *entityMasks[domainDimension], highestEnt+"Mask");
  subDir.putDistributed( *entityMasks[Vertex],"VertexMask");
  subDir.put( entitySize[Vertex], "VertexSize");

  // write the tagging info...
  subDir.put(dumpTagsToHDF,"dumpTagsToHDF");
  if ( dumpTagsToHDF ) {
    int numberOfTags = 0;
    for ( std::map<IDTuple, std::list<EntityTag*>, std::less<IDTuple> >::const_iterator i=entityTags.begin();
	  i!=entityTags.end(); i++ )
      for ( std::list<EntityTag*>::const_iterator t=i->second.begin(); t!=i->second.end(); t++ ) numberOfTags++;

    subDir.put(numberOfTags,"numberOfTags");

    if ( numberOfTags ) 
      {
	aString *tagNames = new aString[numberOfTags];
	int tn=0;
	for ( std::map<IDTuple, std::list<EntityTag*>, std::less<IDTuple> >::const_iterator i=entityTags.begin();
	      i!=entityTags.end(); i++ )
	  for ( list<EntityTag*>::const_iterator t=i->second.begin(); t!=i->second.end(); t++,tn++ ) 
	    {
	      //	      sPrintF(tagNames[tn],"%s_%d_%d",(*t)->getName().c_str(),i->first.et,i->first.e);
	      tagNames[tn] = (*t)->getName();
	      std::string tmp = tagNames[tn].c_str();
	      assert(tmp.length());
	      string::size_type sloc = tmp.find(" ");
	      while ( (sloc=tmp.find(" "))!=string::npos )
		tmp.replace(sloc,1,"__ws__");

	      tagNames[tn] = tmp.c_str();
	    }

	subDir.put(tagNames,"tagNames",numberOfTags);

	intSerialArray tagdata(numberOfTags,5); // 0-type, 1-index, 2-data, 3-copiesData, 4-datasize
	numberOfTags=0;
	for ( std::map<IDTuple, std::list<EntityTag*>, std::less<IDTuple> >::const_iterator i=entityTags.begin();
	      i!=entityTags.end(); i++ )
	  for ( std::list<EntityTag*>::const_iterator t=i->second.begin(); t!=i->second.end(); t++ ) 
	    {
	      tagdata(numberOfTags,0) = i->first.et;
	      tagdata(numberOfTags,1) = i->first.e;
	      
              // *wdh* intptr_t is an integer type that can hold a pointer (in stdint.h)
	      tagdata(numberOfTags,2) = (intptr_t)(*t)->getData();  
	      tagdata(numberOfTags,3) = (*t)->copiesData();
	      tagdata(numberOfTags,4) = (*t)->getDataSize();
	      numberOfTags++;
	    }

	subDir.put(tagdata,"tagData");

#if 0
	tn=0;
	for ( std::map<IDTuple, std::list<EntityTag*>, std::less<IDTuple> >::const_iterator i=entityTags.begin();
	      i!=entityTags.end(); i++ )
	  for ( list<EntityTag*>::const_iterator t=i->second.begin(); t!=i->second.end(); t++,tn++ ) 
	    {
	      sPrintF(tagNames[tn],"%s_%d_%d",(*t)->getName().c_str(),i->first.et,i->first.e);
	      std::string tmp = tagNames[tn].c_str();
	      tmp.replace(tmp.find(" "),1,"__ws__");
	      tagNames[tn] = tmp.c_str();
	      GenericDataBase &tagDir = *subDir.virtualConstructor();
	      subDir.create(tagDir,tagNames[tn],"Tag");
	      tagDir.put((*t)->getName(),"tagName");
	      tagDir.put(i->first.et,"tagEntityType");
	      tagDir.put(i->first.e,"tagEntity");
	      (*t)->put(tagDir,tagNames[tn]);
	      delete &tagDir;
	    }
#endif

	delete [] tagNames;
      }
  }

  delete &subDir;
  return 0;
}

Mapping *UnstructuredMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==UnstructuredMapping::className )
    retval = new UnstructuredMapping();
  return retval;
}


#include "SplineMapping.h"
int UnstructuredMapping::
findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves )
// ===================================================================================================
/// \details 
///      Locate boundary curves on a 3D surface -- booth curve segments on the boundary.
/// \param numberOfBoundaryCurves (output) : number of boundary curves found.
/// \param boundaryCurves (output) : Boundary curves as spline mappings.
///    {\bf NOTE:} This routine will increment the reference count for you.
// ===================================================================================================
{
  if( getDomainDimension()!=2 || getRangeDimension()!=3 )
  {
    printf("findBoundaryCurves: ERROR: only implemented for surfaces in 3D\n");
    return 1;
  }
  
//   const int numberOfBoundaryFaces = getNumberOfBoundaryFaces();
//   const int numberOfNodes = getNumberOfNodes();
//   const int numberOfFaces = getNumberOfFaces();
  
  if( getNumberOfBoundaryFaces()==0 )
  {
    numberOfBoundaryCurves=0;
    return 0;
  }
  

  const intArray & bf = getBoundaryFace();
//   const intArray & face = getFaces();
//   const realArray & nodes = getNodes();
  

  // mark each boundary node with the two faces that touch it.
  //   nodeInfo(n,0:1) -> the two faces that attach to a boundary face.
  intArray nodeInfo(numberOfNodes,2);
  nodeInfo=-1;

  int f;
  for( int b=0; b<numberOfBoundaryFaces; b++ )
  {
    f=bf(b,0);  // face number for boundary face b
    assert( f>=0 );
    for( int m=0; m<=1; m++ )
    {
      int n0=face(f,m);
      assert( n0>=0  );
      if( nodeInfo(n0,0)==-1 )
	nodeInfo(n0,0)=f;
      else
	nodeInfo(n0,1)=f;
    }
  }

  Range Rx=getRangeDimension();
  const int maxNumberOfBoundaryNodes=numberOfBoundaryFaces*2+10;
  realArray x(maxNumberOfBoundaryNodes,getRangeDimension());
  // faceMask(face)=1 if we have processed this face
  intArray faceMask(numberOfFaces);
  faceMask=0;

  int maxNumberOfCurves=100;
  IntegerArray curveStart(maxNumberOfCurves), curveEnd(maxNumberOfCurves);
  curveStart=-1; curveEnd=-1;
  
  int c=0;  // curve number

  curveStart(c)=0;
  int i=0;
  f=bf(0);

  int n0;
  n0=face(f,0); x(i,Rx)=node(n0,Rx);  faceMask(f)=1; 
  i++;
  // printf("add node %i to curve %i\n",n0,c);

  IntegerArray firstNode(maxNumberOfCurves);
  firstNode=-1;
  firstNode(0)=n0; // first node for curve 0,  remember this for later

  n0=face(f,1); x(i,Rx)=node(n0,Rx); 
  i++;
  // printf("add node %i to curve %i\n",n0,c);
  

  // look for corners in curves where the tangent changes by more than :
  const real criticalAngle=30.;  // degrees
  const real cosCriticalAngle=cos( twoPi*criticalAngle/360.);

  bool done=false;
  int count=0;
  while( !done && count<=numberOfBoundaryFaces )
  {
    count++;   // for safety, count how many times we go through this loop
    assert( f>=0 );

    int fNext;
    if( nodeInfo(n0,0)==f )
      fNext=nodeInfo(n0,1);
    else
      fNext=nodeInfo(n0,0);

    assert( fNext>=0 && f!=fNext );
    f=fNext;

    if( faceMask(f)==0 )
    {
      // this is a new boundary face -- add a node to the curve

      assert( face(f,0)==n0 || face(f,1)==n0 );
      
      int previousNode=n0;
      n0 = face(f,0)==n0 ? face(f,1) : face(f,0);
      x(i,Rx)=node(n0,Rx);  faceMask(f)=1; 
      // printf("add node %i to curve %i\n",n0,c);
      
      i++;

      // check if the curve has a corner in which case we split the curve
      if( i>curveStart(c)+2 )
      {
	real t1[3],t2[3];
        // take the dot product of the tangents
	t1[0]=x(i-2,0)-x(i-3,0), t1[1]=x(i-2,1)-x(i-3,1), t1[2]=x(i-2,2)-x(i-3,2);
	t2[0]=x(i-1,0)-x(i-2,0), t2[1]=x(i-1,1)-x(i-2,1), t2[2]=x(i-1,2)-x(i-2,2);

        real dot=(t1[0]*t2[0]+t1[1]*t2[1]+t1[2]*t2[2])/
              max(REAL_MIN*10.,SQRT( (t1[0]*t1[0]+t1[1]*t1[1]+t1[2]*t1[2]) *
				     (t2[0]*t2[0]+t2[1]*t2[1]+t2[2]*t2[2]) ));
	
        // printf(" n0=%i, dot=%8.2e\n",n0,dot);
	
        if( dot < cosCriticalAngle )
	{
          // split the curve
           printf("split curve %i at i=%i node=%i\n",c,i,previousNode);
	  
	  assert( c<maxNumberOfCurves-1 );
	  curveEnd(c)=i-2; 
	  c++;
          curveStart(c)=i-2;    // note : corner point is on both curves
          firstNode(c)=previousNode;
	}
	
      }
      
    }
    else // old face found
    {
      // this is a face we have already seen, this curve is finished.
      if( c==0 || i>curveEnd(c-1)+1 )  // skip 'curves' with no points.
      {
	assert( c<maxNumberOfCurves-1 );
	curveEnd(c)=i-1; 

        bool curvesMatch = false;
        int c0=-1;
        // printf("Old face found - check for merging curve c=%i with node n0=%i\n",c,n0);
	
	for( int cc=0; cc<c; cc++ )
	{
	  if( firstNode(cc)==n0 )
	  {
	    c0=cc;
            break;
	  }
	}
        if( c0!=-1 )
	{
	  // this curve joins to the first pt  of curve c0 -- merge the two if there is not a corner.
          // printf("merge curves %i and %i \n",c,c0);
	  
	  n0 = face(f,0)==n0 ? face(f,1) : face(f,0);
	  x(i,Rx)=node(n0,Rx);

	  // check if the curve has a corner in which case we split the curve
          bool cornerFound=false;
   	  if( i>curveStart(c)+1 )  // we need at least 3 consecutive pts
	  {
	    real t1[3],t2[3];
	    // take the dot product of the tangents
	    t1[0]=x(i-1,0)-x(i-2,0), t1[1]=x(i-1,1)-x(i-2,1), t1[2]=x(i-1,2)-x(i-2,2);
	    t2[0]=x(i  ,0)-x(i-1,0), t2[1]=x(i  ,1)-x(i-1,1), t2[2]=x(i  ,2)-x(i-1,2);

	    real dot=(t1[0]*t2[0]+t1[1]*t2[1]+t1[2]*t2[2])/
              max(REAL_MIN*10.,SQRT( (t1[0]*t1[0]+t1[1]*t1[1]+t1[2]*t1[2]) *
				     (t2[0]*t2[0]+t2[1]*t2[1]+t2[2]*t2[2]) ));
	    if( dot < cosCriticalAngle )
	      cornerFound=true;
	  }
	  if( !cornerFound )
	  {
	    Range I(curveStart(c0)+1,curveEnd(c0));
	    x(I+curveEnd(c)-curveStart(c0),Rx)=x(I,Rx);
	    curveEnd(c)+=curveEnd(c0)-curveStart(c0);
	    Range C(c0,c-1);

	    curveStart(C)=curveStart(C+1);  // remove curve c0
	    curveEnd(C)  =curveEnd(C+1);

            firstNode(c0)=-1;  // do not merge again.
	  }
	  else
	    { // kkc 090408 start a new curve since we did not merge the first and last one
	      c++; // note that this need to be done so that curveStart(c)=i below does not mess up the last curve segment in a periodic multi-segment curve
	    }
	}
        else
	{
  	  c++;  // start a new boundary curve
	}
        curveStart(c)=i;
      }
      

      // --- look for a new starting node on a boundary face
      f=-1;
      for( int b=0; b<numberOfBoundaryFaces; b++ )
      {
	//kkc 090403 faceMask is an array over ALL the faces...	if( faceMask(b)==0 )
	if( faceMask(bf(b))==0 )
	{
	  f=bf(b);
	  break;
	}
      } 
      if( f==-1 )
	done=true;

      n0=face(f,0);
      firstNode(c)=n0;

    } // old face found
    
    
  } // end while
  

  numberOfBoundaryCurves=c;
  boundaryCurves = new Mapping * [numberOfBoundaryCurves];
  int numberRemoved=0;
  int cb=0;
  for( c=0; c<numberOfBoundaryCurves; c++ )
  {
    if( curveEnd(c)>curveStart(c))
    {
      SplineMapping & spline = *new SplineMapping;  spline.incrementReferenceCount();
      boundaryCurves[cb] = &spline;
      cb++;
      
      int c0=curveStart(c);
      assert( curveEnd(c)>c0 );
      Range I(c0,curveEnd(c));
//     ::display(x(I,0),"x(I,0)");
//     ::display(x(I,1),"x(I,1)");
//     ::display(x(I,2),"x(I,2)");

      spline.setShapePreserving(true);
      spline.setPoints( x(I,0),x(I,1),x(I,2) );

      if( x(c0,0)==x(curveEnd(c),0) && x(c0,1)==x(curveEnd(c),1) && x(c0,2)==x(curveEnd(c),2) )
      {
	spline.setIsPeriodic(0,Mapping::functionPeriodic);
	printf("Boundary curve %i is periodic \n",c);
      }
      else
      {
	printf("Boundary curve %i is not periodic diffs=(%e,%e,%e)\n",c,
	       x(c0,0)-x(curveEnd(c),0),x(c0,1)-x(curveEnd(c),1),x(c0,2)-x(curveEnd(c),2));
      }
      

      // spline.update(mapInfo);
    }
    else
    { 
      printf("**WARNING** there is a boundary curve with one point on it. I will remove it.\n");
      numberRemoved++;
    }
    
  }
  numberOfBoundaryCurves-=numberRemoved;
  
  return 0;
}


// int 
// buildCompositeTopology( CompositeSurface & cs, UnstructuredMapping *globalTriangulation );


int UnstructuredMapping::
FEZInitializeConnectivity()
{
  // initialize finite element zoo connecitivity

  assert(domainDimension==2||domainDimension==3);

  numberOfElementTypes = 9;

  if (domainDimension == 2) 
  {
    maxNumberOfNodesPerFace = 2;
    maxNumberOfNodesPerElement = 4;
    maxNumberOfFacesPerElement = 4;
  } else if (domainDimension == 3) {
    maxNumberOfNodesPerFace = 4;
    maxNumberOfNodesPerElement = 8;
    maxNumberOfFacesPerElement = 6;
  }
    
  numberOfFacesThisElementType.redim(numberOfElementTypes);
  numberOfFacesThisElementType(int(triangle     )) = 3;
  numberOfFacesThisElementType(int(quadrilateral)) = 4;
  numberOfFacesThisElementType(int(tetrahedron  )) = 4;
  numberOfFacesThisElementType(int(pyramid      )) = 5;
  numberOfFacesThisElementType(int(triPrism     )) = 5;
  numberOfFacesThisElementType(int(septahedron  )) = 6;
  numberOfFacesThisElementType(int(hexahedron   )) = 6;
  numberOfFacesThisElementType(int(other        )) = -1;
  numberOfFacesThisElementType(int(boundary     )) = maxNumberOfFacesPerElement;
  
  numberOfNodesThisElementType.redim(numberOfElementTypes);
  numberOfNodesThisElementType(int(triangle     )) = 3;
  numberOfNodesThisElementType(int(quadrilateral)) = 4;
  numberOfNodesThisElementType(int(tetrahedron  )) = 4;
  numberOfNodesThisElementType(int(pyramid      )) = 5;
  numberOfNodesThisElementType(int(triPrism     )) = 6;
  numberOfNodesThisElementType(int(septahedron  )) = 7;
  numberOfNodesThisElementType(int(hexahedron   )) = 8;
  numberOfNodesThisElementType(int(other        )) = -1;
  numberOfNodesThisElementType(int(boundary     )) = maxNumberOfNodesPerElement;

  elementMasterTemplate.redim(numberOfElementTypes,maxNumberOfFacesPerElement,maxNumberOfNodesPerFace);
  elementMasterTemplate = -1;
  numberOfNodesThisElementFaceType.redim(numberOfElementTypes,maxNumberOfFacesPerElement);
  numberOfNodesThisElementFaceType = -1;
  Index If(0,maxNumberOfNodesPerFace);
  Index Iff(0,maxNumberOfFacesPerElement);

  // assume ingrid zone arrangements, note that a septahedron is not part of ingrid...
  // also, only allocate and fill in templates appropriate for the zone types in the domainDimension, -1 indices everywhere else will help determine when something is screwed up...
  if (domainDimension == 2) 
  { // 2D element connectivity templates 
    elementMasterTemplate(int(triangle), 0, 0) = 0;
    elementMasterTemplate(int(triangle), 0, 1) = 1;
    elementMasterTemplate(int(triangle), 1, 0) = 1;
    elementMasterTemplate(int(triangle), 1, 1) = 2;
    elementMasterTemplate(int(triangle), 2, 0) = 2;
    elementMasterTemplate(int(triangle), 2, 1) = 0;

    elementMasterTemplate(int(quadrilateral), 0, 0) = 0 ;
    elementMasterTemplate(int(quadrilateral), 0, 1) = 1 ;
    elementMasterTemplate(int(quadrilateral), 1, 0) = 1 ;
    elementMasterTemplate(int(quadrilateral), 1, 1) = 2 ;
    elementMasterTemplate(int(quadrilateral), 2, 0) = 2 ;
    elementMasterTemplate(int(quadrilateral), 2, 1) = 3 ;
    elementMasterTemplate(int(quadrilateral), 3, 0) = 3 ;
    elementMasterTemplate(int(quadrilateral), 3, 1) = 0 ;

    numberOfNodesThisElementFaceType(int(triangle),Index(0,3)) = 2;
    numberOfNodesThisElementFaceType(int(triangle),3) = 0;
    numberOfNodesThisElementFaceType(int(quadrilateral),Index(0,4)) = 2;
    
  } else {
    // 3D connectivity templates
    {
      int i[] = { 0, 1, 3, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(tetrahedron), 0, ind) = i[ind];
    }
    {
      int i[] = { 0, 2, 1, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(tetrahedron), 1, ind) = i[ind];
    }
    {
      int i[] = { 0, 3, 2, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(tetrahedron), 2, ind) = i[ind];
    }
    {
      int i[] = { 1, 2, 3, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(tetrahedron), 3, ind) = i[ind];
    }

    {
      int i[] = { 0, 3, 2, 1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(pyramid), 0, ind) = i[ind];
    }
    {
      int i[] = { 0, 4, 3, -1 };    
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(pyramid), 1, ind) = i[ind];
    }
    {
      int i[] = { 0, 1, 4, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(pyramid), 2, ind) = i[ind];
    }
    {
      int i[] = { 1, 2, 4, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(pyramid), 3, ind) = i[ind];
    }
    {
      int i[] = { 2, 3, 4, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(pyramid), 4, ind) = i[ind];
    }

    {
      int i[] = { 0, 1, 4, 3 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(triPrism), 0, ind) = i[ind];
    }
    {
      int i[] = { 0, 3, 5, 2 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(triPrism), 1, ind) = i[ind];
    }
    {
      int i[] = { 1, 2, 5, 4 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(triPrism), 2, ind) = i[ind];
    }
    {
      int i[] = { 0, 2, 1, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(triPrism), 3, ind) = i[ind];
    }
    {
      int i[] = { 3, 4, 5, -1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(triPrism), 4, ind) = i[ind];
    }

    //    elementMasterTemplate(int(septahedron  ));
    
    {
      int i[] = { 0, 1, 5, 4 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 0, ind)= i[ind];
    }
    {
      int i[] = { 2, 3, 7, 6 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 1, ind)= i[ind];
    }
    {
      int i[] = { 4, 5, 6, 7 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 2, ind)= i[ind];
    }
    {
      int i[] = { 0, 3, 2, 1 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 3, ind)= i[ind];
    }
    {
      int i[] = { 1, 2, 6, 5 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 4, ind)= i[ind];
    }
    {
      int i[] = { 0, 4, 7, 3 };
      for (int ind=0; ind<maxNumberOfNodesPerFace; ind++) elementMasterTemplate(int(hexahedron), 5, ind)= i[ind];
    }

    int i1[] = { 3, 3, 3, 3, 0, 0 };
    int i2[] = { 4, 3, 3, 3, 3, 0 };
    int i3[] = { 4, 4, 4, 3, 3, 0 };    

    numberOfNodesThisElementFaceType(int(hexahedron), Iff) = 4;

    for (int iff=Iff.getBase(); iff<Iff.getBound(); iff++) {
      numberOfNodesThisElementFaceType(int(tetrahedron), iff) = i1[iff];
      numberOfNodesThisElementFaceType(int(pyramid), iff) = i2[iff];
      numberOfNodesThisElementFaceType(int(triPrism), iff) = i3[iff];
    }

    
  }
 
  return 0;
} 
  
int UnstructuredMapping::
FEZComputeElementTypes()
{
  Index I(0, maxNumberOfNodesPerElement);
  //  IntegerArray mask(1,I);
  IntegerArray nCount(numberOfElements);

  elementType.redim(0);
  elementType.redim(numberOfElements);
  elementType = int(other);
  for (int e=0; e<numberOfElements; e++) 
  {
    //    mask = 0;
    
    //    where (element(e,I)>=0) 
    //    {
    //      mask = 1;
    //    }
    
    //
    //    int nnod = sum(mask);
    int n=0;
    int nnod=0;
    for ( ; n<maxNumberOfNodesPerElement && element(e,n)>-1; n++ )
      nnod++;

    if( !(nnod>0 && nnod<=maxNumberOfNodesPerElement) )
    {
      if( nnod==0 )
        printf("UnstructuredMapping::ERROR: there is an element with zero nodes!\n");
      throw "error";
    }
    nCount(e) = nnod;
  }
  if (domainDimension==2) 
  {
    where (nCount == 3) 
      {
	elementType = int(triangle);
      }
    otherwise ()
      {
	elementType = int(quadrilateral);
      }
  } else {
    where(nCount == 4)
      {
	elementType = int(tetrahedron);
      }
    elsewhere(nCount == 5)
      {
	elementType = int(pyramid);
      }
    elsewhere(nCount == 6)
      {
	elementType = int(triPrism);
      }
    elsewhere(nCount == 8)
      {
	elementType = int(hexahedron);
      }
    otherwise()
      {
	elementType = int(other);
      }
  } 
  return 0;
}
    
// *** moved to ugen

// void UnstructuredMapping::
// buildFromACompositeGrid( CompositeGrid &cg )
// //===========================================================================
// // /Description: build an unstructured mapping from a composite grid
// // /cg (input) : a composite grid that may or may not be a hybrid grid
// // /Comments : The composite grid has no restrictions, it could be an overlapping
// // grid or hybrid mesh.  In the case of an overlapping grid, the UnstructuredMapping
// // essentially consists of overlapping sections and holes that have no connectivity
// // information.  A hybrid mesh becomes one consistent UnstructuredMapping.
// //===========================================================================
// {

//   int numberOfGrids = cg.numberOfComponentGrids();
//   int numberOfDimensions = cg.numberOfDimensions();


//   // set two basic fields of the class
//   domainDimension = rangeDimension = numberOfDimensions;

//   // initialize templates and constants.
//   initMapping();

//   // get the hybrid connectivity information
//   const CompositeGridHybridConnectivity & connectivity = cg.getHybridConnectivity();

//   intArray *gridVertexMappings = new intArray[numberOfGrids];
//   UnstructuredMapping *unstructuredMeshes = new UnstructuredMapping[numberOfGrids];
//   intArray *globalVertexIDMapping = new intArray[numberOfGrids];
//   intArray globalIndexMapping;
//   intArray *gridVertexBC       = new intArray[numberOfGrids];

//   int maxNumberOfVertices = 0;
//   numberOfElements = 0;
//   // common loop counters
//   int g;
//   globalIndexMapping = -1;

//   // convert all the grids to UnstructuredMappings, keeping track of the mapping between the
//   // original grid and the new UnstructuredMapping. Also tally the maximum number of vertices
//   // we will need in the new UnstructuredMapping we are building.
//   for ( g=0; g<numberOfGrids; g++ )
//     {
//       MappedGrid &mappedGrid = cg[g];
//       intArray &mask = mappedGrid.mask();
//       gridVertexMappings[g] = unstructuredMeshes[g].buildFromAMapping(cg[g].mapping().getMapping(), mask);
//       globalVertexIDMapping[g].redim(unstructuredMeshes[g].getNumberOfNodes());
//       globalVertexIDMapping[g]=-1;
//       maxNumberOfVertices += unstructuredMeshes[g].getNumberOfNodes();
//       numberOfElements += unstructuredMeshes[g].getNumberOfElements();
//       gridVertexBC[g].redim(unstructuredMeshes[g].getNumberOfNodes(),domainDimension);
//       gridVertexBC[g]=INT_MAX;

//       const intArray & gface = unstructuredMeshes[g].getFaces();
//       for ( int fb=0; fb<unstructuredMeshes[g].getNumberOfBoundaryFaces(); fb++ )
// 	{
// 	  int f = unstructuredMeshes[g].getBoundaryFace(fb);
// 	  int bcnum = unstructuredMeshes[g].getBoundaryFaceTag(fb);
// 	  for ( int fv=0; fv<unstructuredMeshes[g].getNumberOfNodesThisFace(f); fv++ )
// 	    {
// 	      int nn=0;
// 	      while ( nn<domainDimension && 
// 		      gridVertexBC[g](gface(f,fv),nn)!=INT_MAX &&
// 		      gridVertexBC[g](gface(f,fv),nn)!=bcnum ) 
// 		{
// 		  nn++;
// 		}
// 	      gridVertexBC[g](gface(f,fv),nn) = bcnum;
// 	    }
// 	}
//     }

//   node.redim(maxNumberOfVertices, numberOfDimensions);
//   element.redim(numberOfElements, maxNumberOfNodesPerElement);
//   tags.redim(numberOfElements);
//   globalIndexMapping.redim(maxNumberOfVertices,2);
//   globalIndexMapping = -1;

//   tags = -1;
//   element = -1;
//   Range AXES(0,numberOfDimensions-1);
//   int globalVertexIDCounter = 0;
//   // create the global vertexIDs for all the shared vertices ( using information obtained from the
//   // CompositeGrid's hybridConnectivity.
//   const intArray &uVertex2GridIndex = connectivity.getUVertex2GridIndex();
//   int unstructuredGrid = connectivity.getUnstructuredGridIndex();
  
//   for ( int v=0; v<uVertex2GridIndex.getLength(0); v++ )
//     {
//       int setGrid = uVertex2GridIndex(v,0);
//       int i1 = uVertex2GridIndex(v,1);
//       int i2 = uVertex2GridIndex(v,2);
//       int i3 = uVertex2GridIndex(v,3);
//       int setVertex = gridVertexMappings[setGrid](i1,i2,i3);
//       globalVertexIDMapping[unstructuredGrid](v) = globalVertexIDCounter;
//       globalVertexIDMapping[setGrid](setVertex) = globalVertexIDCounter;
//       globalIndexMapping(globalVertexIDCounter,0) = unstructuredGrid;
//       globalIndexMapping(globalVertexIDCounter,1) = v;
      
//       node(globalVertexIDCounter, AXES) = unstructuredMeshes[unstructuredGrid].getNodes()(v, AXES);
//       globalVertexIDCounter++;
      
//     }

//   // now loop through all the grids setting the globalVertexID for each vertex not already set
//   Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
//   for ( g=0; g<numberOfGrids; g++ )
//     {
//       ::getIndex(cg[g].gridIndexRange(), I1,I2,I3);

//       const realArray &uNodes = unstructuredMeshes[g].getNodes();

//       int i1,i2,i3;
//       for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
// 	{
// 	  for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
// 	    {
// 	      for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
// 		{
// 		  int vertex;
// 		  if ( (vertex = gridVertexMappings[g](i1,i2,i3)) != -1 )
// 		    { // if the vertex has not been blanked out give it a global id
// 		      if ( globalVertexIDMapping[g](vertex) == -1 )
// 			{ // but set a new id only if the vertex has not already been set
// 			  globalVertexIDMapping[g](vertex) = globalVertexIDCounter;
// 			  globalIndexMapping(globalVertexIDCounter,0) = g;
// 			  globalIndexMapping(globalVertexIDCounter,1) = vertex;
// 			  node(globalVertexIDCounter, AXES) = uNodes(vertex, AXES);
// 			  globalVertexIDCounter++;
// 			}
// 		    }
// 		} // i1
// 	    } // i2
// 	} // i3
      
//     } // g

//   // with the vertex ID's set the elements in each unstructured mapping can be assembled into
//   // the new one being constructed.
//   int globalElementCounter = 0;
//   for ( g=0; g<numberOfGrids; g++ )
//     {
//       UnstructuredMapping &uMap = unstructuredMeshes[g];
//       const intArray &uElem = uMap.getElements();

//       for ( int e=0; e<uMap.getNumberOfElements(); e++ )
// 	{
// 	  for ( int v=0; v<maxNumberOfNodesPerElement; v++ )
// 	    {
// 	      if (uElem(e,v)!=-1)
// 		element(globalElementCounter, v) = globalVertexIDMapping[g](uElem(e, v));
// 	      else
// 		element(globalElementCounter, v) = -1;

// 	    }
// 	  // tag the element by the grid number it came from
// 	  tags(globalElementCounter) = g;
// 	  globalElementCounter++;
// 	}
//     }

//   numberOfNodes = globalVertexIDCounter;
//   setGridDimensions( axis1,numberOfNodes );  
  
//   node.resize(numberOfNodes, domainDimension);      
  
//   Range R(0,numberOfNodes-1);
//   for( int axis=0; axis<rangeDimension; axis++ )
//   {
//     setRangeBound(Start,axis,min(node(R,axis)));
//     setRangeBound(End  ,axis,max(node(R,axis)));
//   }
  
//   buildConnectivityLists();

//   // now figure out what the boundary conditions are
//   for ( int fb=0; fb<getNumberOfBoundaryFaces(); fb++ )
//     {
//       int f=getBoundaryFace(fb);
//       bool foundBC = false;
//       for ( int testbcvert=0; testbcvert<domainDimension && !foundBC; testbcvert++ )
// 	{
// 	  int tbc = gridVertexBC[globalIndexMapping(face(f,0),0)](globalIndexMapping(face(f,0),1),testbcvert);
// 	  foundBC = true;
// 	  for ( int fn=1; foundBC && fn<getNumberOfNodesThisFace(f); fn++ )
// 	    {
// 	      foundBC = false;
// 	      for ( int tbcv=0; tbcv<domainDimension && !foundBC; tbcv++ )
// 		{
// 		  foundBC = 
// 		    tbc==gridVertexBC[globalIndexMapping(face(f,fn),0)](globalIndexMapping(face(f,fn),1),tbcv);
// 		}
// 	    }
// 	  if ( foundBC )
// 	    bdyFaceTags(fb) = tbc;
// 	}
//     }

//   delete [] gridVertexMappings;
//   delete [] unstructuredMeshes;
//   delete [] globalVertexIDMapping;
//   delete [] gridVertexBC;
// }


void 
UnstructuredMapping::
createNodeElementList(intArray &nodeElementList)
{

  intArray nodeCounter(numberOfNodes);
  nodeCounter = 0;

  int e,n;
  for ( e=0; e<numberOfElements; e++ )
    for ( n=0; n<getNumberOfNodesThisElement(e); n++ )
      nodeCounter(element(e,n))++;

  int lenNodeElementList = numberOfNodes + nodeCounter(0);
  intArray nodeOffset(numberOfNodes);
  //  nodeOffset = 1;
  nodeOffset = 0;

  for ( n=1; n<numberOfNodes; n++ )
    {
      lenNodeElementList += nodeCounter(n);
      nodeOffset(n) = nodeOffset(n-1)+nodeCounter(n-1)+1;
    }

  nodeElementList.redim(lenNodeElementList, 2); // n,0 -> element; n,1 -> node's local element id
  nodeElementList = -1;
  nodeCounter = 0;

  for ( e=0; e<numberOfElements; e++ )
    for ( n=0; n<getNumberOfNodesThisElement(e); n++ )
      {
	int nn = nodeOffset(element(e,n))+nodeCounter(element(e,n));
	
	nodeElementList(nn,0) = e;
	nodeElementList(nn,1) = n;
	nodeCounter(element(e,n))++;
      }

  //nodeElementList.display("nodeElementList");

}
      
aString UnstructuredMapping::
getColour( ) const
//===========================================================================
/// \brief  
///     Get the colour of the grid.
/// \param Return value : the name of the colour.
//===========================================================================
{
  return gridColour;
}

int UnstructuredMapping::
setColour( const aString & colour )
//===========================================================================
/// \brief  
///     Set the colour for the grid.
/// \param colour (input) : the name of the colour such as "red", "green",...
//===========================================================================
{
  gridColour=colour;
  return 0;
}

void UnstructuredMapping::
eraseUnstructuredMapping(GenericGraphicsInterface &gi)
// ==============================================================================
/// \details 
///    purge all display lists for the unstructured mapping
// ==============================================================================
{
  if( gi.isGraphicsWindowOpen() )
  {
    gi.deleteList(dList[nodeDL]);
    gi.deleteList(dList[edgeDL]);
    gi.deleteList(dList[boundaryEdgeDL]);
    gi.deleteList(dList[faceDL]);
    gi.deleteList(dList[faceNormalDL]);

    dList[nodeDL] = 0;
    dList[edgeDL] = 0;
    dList[boundaryEdgeDL] = 0;
    dList[faceDL] = 0;
    dList[faceNormalDL] = 0;

    gi.redraw(); // redraw all display lists
  }
  
};
//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int UnstructuredMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!UnstructuredMapping",
      "set nodes",
      "define elements",
      "save data in a file",
      "read data from a file",
      "read avs file",
      "read stl file",
      "read ply file",
      "rotate",
      "scale",
      "shift",
      "wire frame (toggle)",
      "build from a mapping",
      "prefer triangles (toggle)",
      "do include ghost elements",
      "do not include ghost elements",
      "project a point",
      "query an element",
      "element density tolerance",
      "stitching tolerance",
      "absolute stitching tolerance",
      "find boundary curves",
      "plot sub-surface triangulations",
      "stitch debug",
      "expand ghost boundary",
//      "build topology",
      " ",
//      "lines",
//       "boundary conditions",
//       "share",
      "mappingName",
//       "periodicity",
      "check",
      "show parameters",
      "show connectivity",
      "show boundary edges (toggle)",
      "show edges (toggle)",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "set nodes",
      "define elements",
      "save data in a file : save the unstructured grid in an ascii file",
      "read data from a file : read  the unstructured grid from an ascii file",
      "build from a mapping  : build an unstructured mapping from another mapping",
      "prefer triangles (toggle): prefer building triangles and tetrahedra",
      "include ghost elements : add ghost elements at boundaries",
      "rotate : rotate the current unstructured mapping",
      "scale  : scale  the current unstructured mapping",
      "shift  : shift  the current unstructured mapping",
//       "lines              : specify number of grid lines",
//       "boundary conditions: specify boundary conditions",
//       "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
//       "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=numberOfElements > 0;


// NOTE: These parameters are local and changing them won't affect the calling program
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  int plotWireFrame;
  parameters.get(GI_PLOT_WIRE_FRAME,plotWireFrame);

  parameters.set(GI_PLOT_UNS_EDGES,true);
  parameters.set(GI_PLOT_UNS_BOUNDARY_EDGES,true);
  parameters.set(GI_PLOT_BLOCK_BOUNDARIES,false);

  if( rangeDimension==2 )
  {
    parameters.set(GI_PLOT_UNS_FACES,true);
    parameters.set(GI_PLOT_UNS_EDGES,true);
    parameters.set(GI_PLOT_LINES_ON_GRID_BOUNDARIES,true);
    parameters.set(GI_PLOT_GRID_LINES,true);
  }

  gi.appendToTheDefaultPrompt("Unstructured>"); // set the default prompt
  int axis;

  Mapping *referenceMapping=NULL;

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set nodes" )
    { 
      gi.inputString(line,sPrintF(buff,"Enter the domain and range dimensions (2 or 3) (default=%i,%i): ",
				  domainDimension,rangeDimension));
      if( line!="" )
      {
        int dd=2, rd=2;
        sScanF(line,"%i %i",&dd, &rd);
        if( rd!=2 && rd!=3 )
	{
	  printf("Invalid value %i for the range dimension! \n",rd);
	  continue;
	}
        if( dd!=2 && dd!=3 )
	{
	  printf("Invalid value %i for the domain dimension! \n",dd);
	  continue;
	}
	domainDimension=dd;
	rangeDimension=rd;
      }
      
      RealArray values;
      int numberSpecified = gi.getValues("Enter nodes x1 y1 x2 y2 ...",values);
      if( numberSpecified>0 )
      {
	numberOfNodes=numberSpecified/rangeDimension;
        setGridDimensions( axis1,numberOfNodes );  

	values.resize(numberSpecified);
	values.reshape(rangeDimension,numberOfNodes);
	node.redim(numberOfNodes,rangeDimension);
	node = 0.0;
	for( axis=0; axis<rangeDimension; axis++ )
	  for( int i=0; i<numberOfNodes; i++ )
	    node(i,axis)=values(axis,i);

	Range R(0,numberOfNodes-1), Rx(0,rangeDimension-1);
	for( axis=0; axis<rangeDimension; axis++ )
	{
	  setRangeBound(Start,axis,min(node(R,axis)));
	  setRangeBound(End  ,axis,max(node(R,axis)));
	}
        plotObject=TRUE;

      }
    }
    else if( answer=="define elements" ) 
    {

      FEZInitializeConnectivity();

      // *wdh* added back to Kyle's changes. Is this ok?? 
      gi.inputString(line,sPrintF(buff,
           "Enter the number of nodes per element (max over all elements) (default=%i): ",
				  maxNumberOfNodesPerElement));
      if( line!="" )
        sScanF(line,"%i",&maxNumberOfNodesPerElement);


      IntegerArray values;
      // 2D: 4 number per element
      // 3D:
      int numberSpecified = 
        gi.getValues("Enter elements, use -1 for unused element points:  q1 q2 q3 q4, t1 t2 t3 -1,...",values);

      if( numberSpecified>0 )
      {
	numberOfElements=numberSpecified/maxNumberOfNodesPerElement;
	values.resize(numberSpecified);

	values.reshape(maxNumberOfNodesPerElement,numberOfElements);

	element.redim(1,numberOfElements,maxNumberOfNodesPerElement);
        Range R=numberOfElements;
        for( int n=0; n<maxNumberOfNodesPerElement; n++ )
	{
#ifndef USE_PPP
          element(0,R,n)=values(n,R);
#else
	  for( int i=R.getBase(); i<=R.getBound(); i++ )
            element(0,i,n)=values(n,i);
#endif
	}
        element.reshape(R,maxNumberOfNodesPerElement);
	// element.display("element");
	//setNodesAndConnectivity(node, element, domainDimension);
	buildConnectivityLists();
	
      }
    }
    else if( answer=="save data in a file" )
    {
      gi.inputString(line,"Enter the name of the file to save");
      if( line!="" )
      {
        printf("The format of the file is\n"
              "domainDimension rangeDimension numberOfNodes numberOfElements maxNumberOfNodesPerElement\n"
	       "x0 y0 z0  (or just x0 y0 in 2D \n"
	       "x1 y1 z1 \n"
               " ...     \n"
	       "xn yn zn \n"
               "m0 n0 l0  (node numbers for element 0)\n"
               "m1 n1 l1 \n"
               "  ...    \n");
	put(line);
      }
      continue;
    }
    else if( answer=="read data from a file" )
    {
      gi.inputString(line,"Enter the name of the file to read");
      if( line!="" )
      {
        if( get(line)==0 )
          plotObject=TRUE;
      }
    }
    else if( answer=="read avs file" )
    {
      aString fileName;
      gi.inputString(fileName,"Enter avs file name");

      FILE *fp=NULL;
      fp=fopen((const char*)fileName,"r");

      const int buffSize=100;
      char buff[buffSize];
      getLineFromFile( fp,buff,buffSize );

      int dum1,dum2,dum3;
      sScanF(buff,"%i %i %i %i %i",&numberOfNodes,&numberOfElements,&dum1,&dum2,&dum3);
      

      // assume a 3D surface of triangles:
      domainDimension=2;
      rangeDimension=3;
      maxNumberOfNodesPerElement=3;  // we could read file to find 'tri' or ??
      
      realArray xyz;
      xyz.redim(numberOfNodes,rangeDimension);

      printF("Reading numberOfNodes=%i, numberOfElements=%i from an avs file...\n",numberOfNodes,numberOfElements);
      real time=getCPU();
      int n;
      real x,y,z;
      for( int m=0; m<numberOfNodes; m++ )
      {
	getLineFromFile( fp,buff,buffSize );
	sScanF(buff,"%i %e %e %e",&n,&x,&y,&z);
        // fscanf(fp,"%i %e %e %e",&n,&x,&y,&z);

	// printF(" node: n=%i (x,y,z)=(%g,%g,%g)\n",n,x,y,z);

	n--;
        assert( n>=0 && n<numberOfNodes );
	xyz(n,0)=x; xyz(n,1)=y; xyz(n,2)=z;
      }
      
      
      // FEZInitializeConnectivity();

      intArray elems;
      elems.redim(numberOfElements,maxNumberOfNodesPerElement);

      // 2 1 tri  2 526 1  
      char elementName[4]; 
      int n1,n2,n3;
      for( int m=0; m<numberOfElements; m++ )
      {
	getLineFromFile( fp,buff,buffSize );
      
	sScanF(buff,"%i %i %s %i %i %i",&n,&dum1,elementName,&n1,&n2,&n3);
	// printF(" element: n=%i name=%s (n1,n2,n3)=(%i,%i,%i)\n",n,elementName,n1,n2,n3);
	n--; n1--; n2--; n3--;
        assert( n>=0 && n<numberOfElements );
        assert( n1>=0 && n1<numberOfNodes );
        assert( n2>=0 && n2<numberOfNodes );
        assert( n3>=0 && n3<numberOfNodes );

	elems(n,0)=n1; elems(n,1)=n2; elems(n,2)=n3;
      }

      fclose(fp);
      time=getCPU()-time;
      printF("... done reading avs file. Time to read = %8.2e (s)\n",time);

      // buildConnectivityLists();
      setNodesAndConnectivity(xyz, elems, domainDimension );

      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }
    else if( answer=="read stl file" )
    {
      DataFormats::readSTL(*this);

      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }
    else if( answer== "read ply file" )
    {
      DataFormats::readPly(*this);
      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }

    else if( answer=="scale" ) 
    {
      real xScale=1.; real yScale=1.; real zScale=1.;
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale (default=(%e,%e)): ",
            xScale,yScale));
        if( line!="" ) sScanF(line,"%e %e",&xScale,&yScale);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale, zScale (default=(%e,%e,%e)): ",
            xScale,yScale,zScale));
        if( line!="" ) sScanF(line,"%e %e %e",&xScale,&yScale,&zScale);
      }
      scale(xScale,yScale,zScale);

      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }
    else if( answer=="shift" ) 
    {
      real xShift=0., yShift=0., zShift=0.;
      if( rangeDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift (default=(%e,%e)): ",
				    xShift,yShift));
	if( line!="" ) sScanF(line,"%e %e",&xShift,&yShift);
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift, zShift (default=(%e,%e,%e)): ",
				    xShift,yShift,zShift));
	if( line!="" ) sScanF(line,"%e %e %e",&xShift,&yShift,&zShift);
      }
      shift(xShift,yShift,zShift);

      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }
    else if( answer=="rotate" ) 
    {
      int rotationAxis=2;
      real rotationAngle=45., centerOfRotation[3]={0.,0.,0.};
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter the rotation angle(degrees) (default=%e): ",
          rotationAngle));
        if( line!="" ) sScanF(line,"%e",&rotationAngle);
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e): ",
          centerOfRotation[0],centerOfRotation[1]));
        if( line!="" ) sScanF(line,"%e %e",&centerOfRotation[0],&centerOfRotation[1]);
      }        
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter rotation angle(degrees) and axis to rotate about(0,1, or 2)"
				    "(default=(%e,%i)): ",rotationAngle,rotationAxis));
        if( line!="" ) sScanF(line,"%e %i",&rotationAngle,&rotationAxis);
	if( rotationAxis<0 || rotationAxis>2 )
	{
	  cout << "Invalid rotation axis = " << rotationAxis << endl;
	  continue;
	}
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e,%e): ",
				    centerOfRotation[0],centerOfRotation[1],centerOfRotation[2]));
        if( line!="" ) sScanF(line,"%e %e %e",&centerOfRotation[0],&centerOfRotation[1],
                              &centerOfRotation[2]);
      }
      shift(-centerOfRotation[0],-centerOfRotation[1],-centerOfRotation[2]);
      rotate(rotationAxis,rotationAngle*Pi/180.);
      shift(+centerOfRotation[0],+centerOfRotation[1],+centerOfRotation[2]);

      eraseUnstructuredMapping(gi); // this erases old display lists for plotting
      plotObject=true;
    }

    else if( answer=="prefer triangles (toggle)" )
    {
      preferTriangles=!preferTriangles;
      printF("preferTriangles=%i \n",preferTriangles);
    }
    else if( answer=="do include ghost elements" )
    {
      includeGhostElements=true;
      printF(" includeGhostElements=%i\n",includeGhostElements);
    }
    else if( answer=="do not include ghost elements" )
    {
      includeGhostElements=false;
      printF(" includeGhostElements=%i\n",includeGhostElements);
    }
    else if( answer=="include ghost elements" )
    { // for backward compatibility
      includeGhostElements=!includeGhostElements;
      printF(" includeGhostElements=%i\n",includeGhostElements);
      
    }
    else if( answer=="element density tolerance" )
    {
      printf("Enter the tolerance for determining the triangle density when building from a mapping\n");
      printf(" Choose a value of zero to use the default number of elements\n");
      gi.inputString(line,sPrintF(buff,"Enter tolerance (current=%e)",elementDensityTolerance));
      if( line!="" )
      {
	sScanF(line,"%e",&elementDensityTolerance);
	printf("element density tolerance = %e\n",elementDensityTolerance);
      }
    }
    else if( answer=="stitching tolerance" )
    {
      printf("Enter the tolerance for stitching surfaces together on a CompositeSurface\n");
      printf(" Choose a value of zero to use the default.\n");
      real defaultTolerance = stitchingTolerance>0. ? stitchingTolerance : elementDensityTolerance*.1;
      gi.inputString(line,sPrintF(buff,"Enter tolerance (current=%e)",defaultTolerance));
      if( line!="" )
      {
	sScanF(line,"%e",&stitchingTolerance);
	printf("stitching tolerance = %e\n",stitchingTolerance);
      }
    }
    else if( answer=="absolute stitching tolerance" )
    {
      printf("Enter the absolute tolerance for stitching surfaces together on a CompositeSurface\n");
      gi.inputString(line,sPrintF(buff,"Enter tolerance (current=%e)",absoluteStitchingTolerance));
      if( line!="" )
      {
	sScanF(line,"%e",&absoluteStitchingTolerance);
	printf("absolute stitching tolerance = %e\n",absoluteStitchingTolerance);
      }
    }
    else if ( answer=="check" )
      {
	verifyUnstructuredConnectivity( *this, true );
      }
    else if( answer=="build from a mapping" )
    {
      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(mappingName);
      menu2[num]="";   // null string terminates the menu
      for( ;; )
      {
	int mapNumber = gi.getMenuItem(menu2,line);
        if( mapNumber<0 )
	{
	  printf("UnstructuredMapping::ERROR:unknown mapping to turn into a UnstructuredMapping!\n");
	  gi.stopReadingCommandFile();
	}
	else if( mapInfo.mappingList[mapNumber].mapPointer==this )
	{
	  cout << "UnstructuredMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
	  continue;
	}
	else
	{
          Mapping & map = mapInfo.mappingList[mapNumber].getMapping();
	  referenceMapping=&map;
	  
          if( map.getClassName()=="TrimmedMapping" )
	  {
//             if( false )
// 	    {
// 	      // get the mask for a trimmed mapping.
// 	      MappingParameters mapParams;
// 	      map.getGrid(mapParams);
// 	      mapParams.mask.reshape(map.getGridDimensions(0),map.getGridDimensions(1));
// 	      // ::display(mapParams.mask,"mask from trimmed mapping","%3i");
// 	      buildFromAMapping( map,mapParams.mask );
// 	    }
// 	    else
	    {
	      TrimmedMapping & trim = (TrimmedMapping&)map;
	      *this =  trim.getTriangulation();
	    }
	    
	  }
          else if( map.getClassName()=="CompositeSurface" )
	  {
	    buildFromACompositeSurface( (CompositeSurface &)map );
	  }
	  else if( map.getDomainDimension()==2 && ( map.getRangeDimension()==2 || preferTriangles) )
	  {
            // elementDensityTolerance=0.;
	    

            buildFromARegularMapping( map, (preferTriangles? triangle : quadrilateral) );


            // For debugging:
            UnstructuredMappingIterator iter;
            int ne=0;
            printf(" *begin(Region)=%i, *end(Region)=%i\n",*begin(Region),*end(Region));
	    
            for( iter=begin(Region); iter!=end(Region); iter++ )
 	    {
 	      printf(" Element %i is valid \n",*iter);
              ne++;
 	    }
            printf("Using iterators: there are %i regions\n",ne);
            for( iter=begin(Face); iter!=end(Face); iter++ )
              ne++;
            printf("Using iterators: there are %i faces\n",ne);

            bool includeGhost=true;
            ne=0;
            for( iter=begin(Region,includeGhost); iter!=end(Region,includeGhost); iter++ )
 	    {
 	      printf(" Include ghost: Element %i is valid \n",*iter);
              ne++;
 	    }
            printf("Using iterators: there are %i regions with ghost elements\n",ne);
            ne=0;
            for( iter=begin(Face,includeGhost); iter!=end(Face,includeGhost); iter++ )
 	    {
 	      // printf(" Include ghost: Face %i is valid \n",*iter);
              ne++;
 	    }
            printf("Using iterators: there are %i faces with ghost elements\n",ne);
	    
	    if ( ghostElements )
	      {
		const int numberOfGhostElements=ghostElements->getLength(0);
		int n,m,e,ep;
		for( n=0; n<numberOfGhostElements; n++ )
		  {
		    e=(*ghostElements)(n);
		    m = (*elementMask)(e);
		    ep = -m-2;
		    if( ep>=0 )
		      {
			// this is a periodic point with periodic element 
			printf(" Element e=%i has periodic image ep=%i\n",e,ep);
		      }
		  }
	      }
	    

	  }
	  else
	  {
	    buildFromAMapping( map );
	  }
	  
          plotObject=true;
          mappingHasChanged();

	  if( domainDimension==2 ) // && rangeDimension==3 )
	    parameters.set(GI_PLOT_UNS_EDGES,true);
          if( rangeDimension==2 )
	    parameters.set(GI_PLOT_UNS_FACES,true);

          break;
	}
      }
      delete [] menu2;
    }
//     else if( answer=="build topology" )
//     {
//       // Make a menu with the Mapping names
//       int num=mapInfo.mappingList.getLength();
//       aString *menu2 = new aString[num+1];
//       for( int i=0; i<num; i++ )
//         menu2[i]=mapInfo.mappingList[i].getName(mappingName);
//       menu2[num]="";   // null string terminates the menu

//       int mappingListStart=0;
//       int mappingListEnd=num-1;

//       gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

//       for( ;; )
//       {
// 	int mapNumber = gi.getMenuItem(menu2,line);
//         gi.indexInCascadingMenu( mapNumber,mappingListStart,mappingListEnd);

//         if( mapNumber<0 )
// 	{
// 	  printf("UnstructuredMapping::ERROR:unknown mapping to turn into a UnstructuredMapping!\n");
// 	  gi.stopReadingCommandFile();
// 	}
// 	else if( mapInfo.mappingList[mapNumber].mapPointer==this )
// 	{
// 	  cout << "UnstructuredMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
// 	  continue;
// 	}
// 	else
// 	{
//           Mapping & map = mapInfo.mappingList[mapNumber].getMapping();
// 	  referenceMapping=&map;
	  
//           if( map.getClassName()=="CompositeSurface" )
// 	  {
// 	    buildCompositeTopology( (CompositeSurface &)map, this );
// 	  }
// 	  else
// 	  {
//             printf("Sorry: this is not a composite surface");
// 	  }
	  
//           plotObject=TRUE;
//           mappingHasChanged();

// 	  if( domainDimension==2 && rangeDimension==3 )
// 	    parameters.set(GI_PLOT_UNS_EDGES,true);

//           break;
// 	}
//       }
//       delete [] menu2;
//     }
    else if( answer=="project a point" )
    {
      MappingProjectionParameters mpParams;
      intArray & subSurfaceIndex = mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);

      realArray x(1,3), x0(1,3), x2(2,3);
      // parameters.set(GI_USE_PLOT_BOUNDS,TRUE);
      parameters.set(GI_POINT_SIZE,(real)4.);
      Range Rx=rangeDimension;
      
      for( ;; )
      {
        gi.inputString(answer,"Enter a point to project");
	if( answer=="" )
          break;
        else
	{
	  sScanF(answer,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
	  

          x0(0,Rx)=x(0,Rx);
//	  subSurfaceIndex=-1;
	  
          project(x,mpParams);

          x2(0,Rx)=x0(0,Rx);
	  x2(1,Rx)=x(0,Rx);
	  gi.plotPoints(x2,parameters);

          printf("Pt (%e,%e,%e) was projected to (%e,%e,%e) \n",x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2));

	}

      }
      
    }
    else if( answer=="query an element" )
    {
      SelectionInfo select; select.nSelect=0;
//      PickInfo3D pick;  pick.active=0;
      MappingProjectionParameters mpParams;
      intArray & elementIndex = mpParams.getIntArray(MappingProjectionParameters::elementIndex);

      realArray x(1,3), x0(1,3), x2(2,3);
      parameters.set(GI_POINT_SIZE,(real)4.);
      Range Rx=rangeDimension;
      const intArray & ef = getElementFaces();

// setup a new gui
      GUIState interface;

// *this doesn't work for some reason* *wdh* 030825
//        aString windowButtons[][2] = {{"Done"},
//  				    {""}};
//        interface.setUserButtons(windowButtons);

      interface.setWindowTitle("Query An Element");
      interface.addInfoLabel("Pick a point.");
      interface.setExitCommand("done","done");

      gi.pushGUI(interface);
      
      for( ;; )
      {
	gi.getAnswer(answer,"pick a point", select);
        if( answer=="exit" || answer=="done" )
          break;
	if (select.active == 1)
	{
	  printf("A point was picked!\n");
	  printf("Window coordinates: %e, %e\n", select.r[0], select.r[1]);
	  if (select.nSelect)
	  {
	    printf("World coordinates: %e, %e, %e\n", select.x[0], select.x[1], select.x[2]);

            x(0,0)=select.x[0], x(0,1)=select.x[1], x(0,2)=select.x[2];
	    
	    x0(0,Rx)=x(0,Rx);
	    elementIndex=-1;
	  
	    project(x,mpParams);

	    x2(0,Rx)=x0(0,Rx);
	    x2(1,Rx)=x(0,Rx);
	    // gi.plotPoints(x2,parameters);

	    printf("Pt (%e,%e,%e) was projected to (%e,%e,%e) \n",x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2));
            int e=elementIndex(0);
            if( e>=0 && e<numberOfElements )
	    {
              int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);
	      int ae0 = faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
	      int ae1 = faceElements(f1,0)==e ? faceElements(f1,1) : faceElements(f1,0);
	      int ae2 = faceElements(f2,0)==e ? faceElements(f2,1) : faceElements(f2,0);
              printf("Element e=%i, nodes=(%i,%i,%i), faces=(%i,%i,%i) adj elements=(%i,%i,%i) sub-surface=%i \n",
                     e,element(e,0),element(e,1),element(e,2),
                     f0,f1,f2,ae0,ae1,ae2,tags(e));
              printf(" face f0=%i nodes=(%i,%i) e=(%i,%i), f1=%i nodes=(%i,%i) e=(%i,%i), f2=%i nodes=(%i,%i) e=(%i,%i)\n",
		     f0,face(f0,0),face(f0,1),faceElements(f0,0),faceElements(f0,1),
                     f1,face(f1,0),face(f1,1),faceElements(f1,0),faceElements(f1,1),
                     f2,face(f2,0),face(f2,1),faceElements(f2,0),faceElements(f2,1));
	    }
	    else
	    {
	      printf("Invalid element=%i\n",e);
	    }
	  }
	}
      }
      gi.popGUI();
    }
    else if( answer=="find boundary curves" )
    {
      int numberOfBoundaryCurves=0;
      Mapping **boundaryCurves;

      findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
      printf(" ** %i boundary curves found \n",numberOfBoundaryCurves);
      
      real oldCurveLineWidth;
      parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      parameters.set(GI_MAPPING_COLOUR,"green");

      int b;
      for( b=0; b<numberOfBoundaryCurves; b++ )
      {
	aString colour = gi.getColourName(b);
        parameters.set(GI_MAPPING_COLOUR,colour);
        parameters.set(GraphicsParameters::curveLineWidth,4.);
        // gi.erase();
        // boundaryCurves[b]->update(mapInfo);
	
        if( b==numberOfBoundaryCurves-1 )
	  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);          
	PlotIt::plot(gi,*boundaryCurves[b],parameters);
      }
      parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      parameters.set(GI_MAPPING_COLOUR,"red");
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      for( b=0; b<numberOfBoundaryCurves; b++ )
      {
        if( boundaryCurves[b]->decrementReferenceCount()==0 )
          delete boundaryCurves[b];
      }
      delete [] boundaryCurves;

    }
    else if( answer=="stitch debug" )
    {
      gi.inputString(answer,"Enter the debug for stitching");
      sScanF(answer,"%i",&debugs);
      printf(" debugs=%i\n",debugs);
    }
    else if (answer=="expand ghost boundary")
      {
	expandGhostBoundary();
	eraseUnstructuredMapping(gi);
	plotObject =true;
      }
    else if( answer=="show parameters" )
    {
      printf(" numberOfNodes=%i, numberOfElements=%i maxNumberOfNodesPerElement=%i\n",numberOfNodes,numberOfElements,
               maxNumberOfNodesPerElement);
      continue;
    }
    else if( answer=="show connectivity" )
    {
      printConnectivity();
    }
    else if( answer=="show boundary edges (toggle)" )
    {
      int plotBoundaryEdges;
      parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES,plotBoundaryEdges);
      parameters.set(GI_PLOT_UNS_BOUNDARY_EDGES,!plotBoundaryEdges);
    }
    else if( answer=="show edges (toggle)" )
    {
      int plotEdges;
      parameters.get(GI_PLOT_UNS_EDGES,plotEdges);
      parameters.set(GI_PLOT_UNS_EDGES,!plotEdges);
    }
    else if( answer=="wire frame (toggle)" )
    {
      plotWireFrame=!plotWireFrame;
      parameters.set(GI_PLOT_WIRE_FRAME,plotWireFrame);
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="plot sub-surface triangulations" )
    {
      if( referenceMapping==NULL || referenceMapping->getClassName()!="CompositeSurface" )
      {
        printf("There is no CompositeSurface as a reference Mapping\n");
	continue;
      }

      CompositeSurface & cs = *((CompositeSurface*)referenceMapping);
      

      gi.erase();
      aString menu[]=
      {
	"choose a sub-surface",
        "exit",
        ""
      };
      for( ;; )
      {
        int s=-1;
	gi.getMenuItem(menu,answer,"");
	if( answer=="choose a sub-surface" )
	{
	  gi.inputString(answer,sPrintF(line,"Enter a sub-surface s in [%i,%i]",0,cs.numberOfSubSurfaces()-1));
	  if( answer=="" )
	    break;
	  else
	  {
	    sScanF(answer,"%i",&s);
	  }
	}
	else if( answer=="exit" )
	  break;
	
	if( s>=0 && s<cs.numberOfSubSurfaces() )
	{
	  printf("plot sub-surface s=%i \n",s);
	  
	  Mapping & map = cs[s];
	  if( map.getClassName()=="TrimmedMapping" )
	  {
	    printf("Subsurface %i is a TrimmedMapping\n",s);
	  }

	  if( cs[s].getClassName()=="TrimmedMapping" )
	  {
	    TrimmedMapping & trim = (TrimmedMapping&)cs[s];
	    Mapping & subSurface = trim.getTriangulation();
            PlotIt::plot(gi,subSurface,parameters);
	  }
	  else
	  {
	    UnstructuredMapping subSurface;
	    subSurface.setPreferTriangles();
	    subSurface.setElementDensityTolerance(elementDensityTolerance);
	    subSurface.buildFromARegularMapping(cs[s]);
            PlotIt::plot(gi,subSurface,parameters); 
	  }

	}
      }
      
    }
    
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="mappingName" )
        continue;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }


    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

EntityTag & 
UnstructuredMapping::
addTag( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, const std::string tagName,
	const void *tagData, const bool copyTag, const int tagSize )
//===========================================================================
/// \brief  
///     add an EntityTag to a specific entity in the mesh
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tagName    (input): name to give the tag instance
/// \param tagData    (input): data stored by the tag
/// \param copyTag    (input): deep copy tagData if copyTag==true, shallow copy if false
/// \param tagSize    (input): if copyTag==true, this is the size of the tagData
/// \param Returns : a reference to the added EntityTag 
//===========================================================================
{
  assert(tagName.length());
  IDTuple ID(entityType, entityIndex);
  EntityTag *newEt = new EntityTag( tagName, tagData, copyTag, tagSize );

  entityTags[ID].push_back( newEt );

  if ( maintainsTagEntities )
    {
      tagEntities[tagName].push_back(ID);
      //      cout<<"adding a "<<tagName<<", size = "<<tagEntities[tagName].size()<<endl;
    }

  return *newEt;
}

int 
UnstructuredMapping::
deleteTag( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, 
	   const EntityTag &tagToDelete )
//===========================================================================
/// \brief  
///     delete an EntityTag from the mesh
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tagToDelete    (input): a reference to a tag specifying the deletion
/// \param Returns : 0 if successfull
//===========================================================================
{
  std::string tagName = tagToDelete.getName();
  return deleteTag(entityType, entityIndex, tagName);
}

int 
UnstructuredMapping::
deleteTag( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, 
	   const std::string tagToDelete )
//===========================================================================
/// \brief  
///     delete an EntityTag from the mesh
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tagToDelete    (input): a string specifying the name of the tag to delete
/// \param Returns : 0 if successfull
//===========================================================================
{

  //  for ( entity_tag_iterator i=entity_tag_begin(entityType, entityIndex); 
  //	i!=entity_tag_end(entityType, entityIndex); i++ )
  
  entity_tag_iterator i=entity_tag_begin(entityType, entityIndex);
  while ( i!=entity_tag_end(entityType,entityIndex) )
    {
      if ( (*i)->getName() == tagToDelete )
	{
	  IDTuple ID(entityType, entityIndex);
	  if ( maintainsTagEntities && tagEntities[tagToDelete].size()>0 )
	    tagEntities[tagToDelete].erase(std::find(tagEntities[tagToDelete].begin(), tagEntities[tagToDelete].end(),ID));

	  delete *i;
	  *i=NULL;
	  entityTags[ID].erase(i);
	  return 0;
	}
      i++;
    }
  // error, could not find the tag
  return 1;
}

bool 
UnstructuredMapping::
hasTag( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, const std::string tag )
//===========================================================================
/// \brief  
///     check to see if an entity has a particular tag
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tag    (input): a string specifying the name of the tag in question
/// \param Returns : true if the tag exists on the entity
//===========================================================================
{
  // kkc what was I thinking?  return bool(entityTags[IDTuple(entityType,entityIndex)].size());
  for ( entity_tag_iterator i=entity_tag_begin(entityType, entityIndex);
	i != entity_tag_end(entityType, entityIndex); i++ )
    if ( (*i)->getName()== tag )
      return true;

  return false;
}

EntityTag &
UnstructuredMapping::
getTag( const UnstructuredMapping::EntityTypeEnum entityType, 
	const int entityIndex, const std::string tagName)
//===========================================================================
/// \brief  
///     obtain a reference to a tag on a specific entity
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tagName    (input): a string specifying the name of the tag in question
/// \param Returns : the tag requested
/// \param Throws : TagError if the tag is not found
//===========================================================================
{
  for ( entity_tag_iterator i=entity_tag_begin(entityType, entityIndex);
	i != entity_tag_end(entityType, entityIndex); i++ )
    if ( (*i)->getName()== tagName )
      return **i;
  
  // could not find the tag
  throw TagError();
}

void * 
UnstructuredMapping::
getTagData( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, 
	    const std::string tag )
//===========================================================================
/// \brief  
///     obtain the the data in a tag
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tag    (input): a string specifying the name of the tag in question
/// \param Returns : NULL if the tag did not exist
//===========================================================================
{
  void *retTag=0;
  retTag = (void *)getTag(entityType, entityIndex, tag).getData();
  return retTag;
  //  return (void *)getTag(entityType, entityIndex, tag).getData();
}

int 
UnstructuredMapping::
setTagData( const UnstructuredMapping::EntityTypeEnum entityType, const int entityIndex, 
	    const std::string tagName, 
	    const void *data, const bool copyData, const int tagSize )
//===========================================================================
/// \brief  
///     set the data in an existing tag
/// \param entityType (input) : the EntityTypeEnum of the entity
/// \param entityIndex (input): the index of the entity
/// \param tagName    (input): a string specifying the name of the tag in question
/// \param data    (input): data stored by the tag
/// \param copyTag    (input): deep copy tagData if copyTag==true, shallow copy if false
/// \param tagSize    (input): if copyTag==true, this is the size of the tagData
/// \param Returns : 0 if successfull
//===========================================================================
{
  try {
    getTag(entityType, entityIndex, tagName).setData(tagName, data, copyData, tagSize);
  } catch ( TagError &e ) {
    return 1;
  }

  return 0;
}

void 
UnstructuredMapping::
maintainTagToEntityMap( bool v )
//===========================================================================
/// \brief  
///     turn on/off maintainance of the mapping from tags to thier entities
/// \param v (input) : if true turn on the tag to entity mapping, if false turn it off
/// \param Note: If v==true, this method will build the mapping.  If false, it will destroy the mapping
//===========================================================================
{
  if ( v )
    {
      if ( !maintainsTagEntities )
	{
	  // build the mapping from tags to thier entites
	  for ( std::map<IDTuple, std::list<EntityTag*>, std::less<IDTuple> >::iterator i=entityTags.begin();
		i!=entityTags.end(); i++ )
	    for ( entity_tag_iterator tag=i->second.begin(); tag!=i->second.end(); tag++ )
	      tagEntities[(*tag)->getName()].push_back(i->first);
	}
    }
  else
    if ( maintainsTagEntities ) // destroy the mapping
      tagEntities.clear();

  maintainsTagEntities = v;
}

bool
UnstructuredMapping::
maintainsTagToEntityMap( ) const
//===========================================================================
/// \brief  
///     return true if the Mapping maintains the list of entities with a given tag
/// \param Return value : true if the Mapping maintains the list of entities with a given tag.
//===========================================================================
{
  return maintainsTagEntities;
}

bool 
UnstructuredMapping::
entitiesAreEquivalent(EntityTypeEnum type, int entity, ArraySimple<int> &vertices)
// =======================================================================================================
/// \brief  
///  compare the vertices of an entity to a list of vertices, return true if the list specifies the entity
/// \param Return value : return true if the list specifies the entity.
// =======================================================================================================
{
  // FALSE : no entities of this type created yet!
  if ( !entities[type] ) return false;
  // FALSE : invalid entity id given! 
  if ( entity>=size(type) ) return false;
  // FALSE : the number of vertices do not match in each entity
  int nv2=0;
  for ( ; nv2<vertices.size(0) && vertices(nv2)>-1; nv2++ ) { }
  // FALSE : number of vertices do not match!
  if ( numberOfVertices(type,entity)!=nv2 ) return false;

  intArray &entIdx = *entities[type];

  // two entities are the same if thier vertices are the same, note that the ordering can
  //  be reversed.
  
  int nV = numberOfVertices(type,entity);
  int minV1 = entIdx(entity,0);
  int minV1Idx = 0;
  int minV2 = vertices(0);
  int minV2Idx = 0;

  // first find the starting point for each entity
  // the starting point is the lowest vertex id
  for ( int v=1; v<nV; v++ )
    {
      if ( entIdx(entity,v)<minV1 )
	{
	  minV1 = entIdx(entity,v);
	  minV1Idx = v;
	}

      if ( vertices(v)<minV2 )
	{
	  minV2 = vertices(v);
	  minV2Idx = v;
	}
    }
   
  // FALSE : minimum vertex index does not match
  if ( minV1!=minV2 ) return false;

  // now check the vertices in the current order
  bool matches = true;
  for ( int v=0; v<nV && matches; v++ )
    matches = entIdx(entity, (minV1Idx+v)%nV)==vertices((minV2Idx+v)%nV);

  // now check in the opposite direction (only the previous did not work!)
  if ( !matches )
    {
      matches = true;
      for ( int v=0; v<nV && matches; v++ )
	matches = entIdx(entity, (minV1Idx+v)%nV)==vertices((nV+minV2Idx-v)%nV);
    }

  return matches;
}

std::string
UnstructuredMapping::
tagPrefix(EntityTypeEnum type, EntityInfoMask info)
{
  string s="";
  switch(info) {
  case GhostEntity:
    s=string("Ghost ")+EntityTypeStrings[int(type)];
    break;
  case BCEntity:
    s=string("__bcnum ")+EntityTypeStrings[int(type)];
    break;
  default:
    break;
  }
  return s;
  
}

/// setAsGhost takes an entity and adjusts the data structures to make it a ghost 
void 
UnstructuredMapping::
setAsGhost(EntityTypeEnum type, int entity)
{
  // if the entity mask array is there (if not build it?) set the mask
  if ( entityMasks[type] )
    (*entityMasks[type])(entity) |= GhostEntity;

  // now add the info as a tag
  // note this is a simple tag; the only data is the index "entity"
  string ghostStr = string("Ghost ")+EntityTypeStrings[type].c_str();
  if ( !hasTag(type,entity,ghostStr) )
    addTag(type,entity, ghostStr, (void *)entity);
}

/// setBC assigns a boundary condition number to a particular entity, if the entity==-1 the the bc is removed
void 
UnstructuredMapping::
setBC(EntityTypeEnum type, int entity, int bc)
{
  // if the entity mask array is there (if not build it?) set the mask
  if ( bc!=-1 && entityMasks[type] )
    (*entityMasks[type])(entity) |= BCEntity;

  // now add the info as a tag
  // note this is a simple tag; the only data is the index "entity"
  string bcs = tagPrefix(type, BCEntity);

  if ( !hasTag(type,entity,bcs) && bc!=-1 )
    addTag(type, entity, bcs, (void *)bc);
  else if ( hasTag(type,entity,bcs) && bc!=-1 )
    setTagData(type,entity,bcs,(void *)bc);
  else if ( hasTag(type,entity,bcs) && bc==-1 )
    {
      deleteTag(type,entity,bcs);
      if ( entityMasks[type] )
	(*entityMasks[type])(entity) &= BCEntity;
    }
}

bool
UnstructuredMapping::
specifyVertices(const realArray &verts)
{
  node.redim(0);
  node = verts;
  entityCapacity[Vertex] = entitySize[Vertex] = node.getLength(0);

  entityMasks[Vertex] = new intArray(capacity(Vertex));
  *entityMasks[Vertex] = 0;

  return true;
}

const intArray &
UnstructuredMapping::
getEntities(EntityTypeEnum type)
{
  if (!entities[type] ) buildEntity(type);

  return *entities[type];
}

const intArray & 
UnstructuredMapping::
getEntityAdjacencyIndices(EntityTypeEnum from, EntityTypeEnum to, intArray &offsets)
{
  if ( !indexLists[from][to] ) buildConnectivity(from,to);

  if ( offsets.getLength(0) )
    offsets.redim(0);

  if ( from<to )
    offsets = *upwardOffsets[from][to];

  if ( to==Vertex )
    return *entities[to];
  else
    return *indexLists[from][to];
}

bool 
UnstructuredMapping::
buildEntity(EntityTypeEnum type, bool rebuild /*=false*/, bool keepDownward /*=true*/, bool keepUpward /*=true*/)
{
  if ( entities[type] && !rebuild ) 
    return true;

  intArray downward, upwardIDX, upwardOffset;

  if ( type!=Vertex && !entityMasks[Vertex] ) buildEntity(Vertex,true);

  switch(type) {
  case Vertex:
    {
      break;
    }
  case Edge:
    {
      // later we may allow construction using connectivity info
      if ( (!entities[Face]) && (!entities[Region]) )
	return false;
      
      deleteConnectivity(Edge);
      entities[Edge] = new intArray;

      int nedges=0;
      if ( entities[Region] )
	{
	  int mv = maxVerticesInEntity(Region);
	  nedges = constructEdgeEntityFromEntity(*entities[Edge], downward, adjacencyOrientation[Region][Edge], 
						 upwardIDX, upwardOffset, adjacencyOrientation[Vertex][Edge],
						 *entities[Region], size(Region), mv, size(Vertex)-1, int(Region));

	  if ( keepDownward )
	    {
	      indexLists[Region][Edge] = new intArray;;
	      indexLists[Region][Edge]->reference(downward);
	    }
	  else
	    {
	      delete [] adjacencyOrientation[Region][Edge];
	      adjacencyOrientation[Region][Edge] = 0;
	    }

	  if ( keepUpward )
	    {
	      indexLists[Vertex][Edge] = new intArray;
	      upwardOffsets[Vertex][Edge] = new intArray;
	      
	      indexLists[Vertex][Edge]->reference(upwardIDX);
	      upwardOffsets[Vertex][Edge]->reference(upwardOffset);
	    }
	  else
	    {
	      delete [] adjacencyOrientation[Vertex][Edge];
	      adjacencyOrientation[Vertex][Edge] = 0;
	    }

	}
      else
	{
	  int mv = maxVerticesInEntity(Face);
	  nedges = constructEdgeEntityFromEntity(*entities[Edge], downward, adjacencyOrientation[Face][Edge], 
						 upwardIDX, upwardOffset, adjacencyOrientation[Vertex][Edge],
						 *entities[Face], size(Face), mv, size(Vertex)-1,int(Face));

	  if ( keepDownward )
	    {
	      indexLists[Face][Edge] = new intArray;;
	      indexLists[Face][Edge]->reference(downward);
	    }
	  else
	    {
	      delete [] adjacencyOrientation[Face][Edge];
	      adjacencyOrientation[Face][Edge] = 0;
	    }

	  if ( keepUpward )
	    {
	      indexLists[Vertex][Edge] = new intArray;
	      upwardOffsets[Vertex][Edge] = new intArray;
	      
	      indexLists[Vertex][Edge]->reference(upwardIDX);
	      upwardOffsets[Vertex][Edge]->reference(upwardOffset);
	    }
	  else
	    {
	      delete [] adjacencyOrientation[Vertex][Edge];
	      adjacencyOrientation[Vertex][Edge] = 0;
	    }

	}

   
      entityCapacity[Edge] = entitySize[Edge] = nedges;
      entityCapacity[Edge] = entities[Edge]->getLength(0);

      // vertices do have an "orientation" relative to thier edges, the lowest vertex index is +ive
      if ( adjacencyOrientation[Edge][Vertex]!=0 )
	delete [] adjacencyOrientation[Edge][Vertex];
      
      adjacencyOrientation[Edge][Vertex] = new char[2*capacity(Edge)];
      for ( int e=0; e<size(Edge); e++ )
	{
	  if ( (*entities[Edge])(e,0)<(*entities[Edge])(e,1) )
	    {
	      (adjacencyOrientation[Edge][Vertex])[e]=0x1;
	      (adjacencyOrientation[Edge][Vertex])[e+size(Edge)]=0x0;
	    }
	  else
	    {
	      (adjacencyOrientation[Edge][Vertex])[e]=0x0;
	      (adjacencyOrientation[Edge][Vertex])[e+size(Edge)]=0x1;
	    }
	}
	  

      // link to the old connectivity
      if ( !edge.getLength(0) )
	edge.reference(*entities[Edge]);

      // *wdh* 040221 --- This next line was causing problems but is now fixed in setNodesAndConnectivity
      if ( !face.getLength(0) )
	if ( domainDimension==2 )
	  face.reference(edge);

      break;
    }
  case Face:
    {
      // later we may allow construction using connectivity info
      if ( !entities[Region] ) return false;

      deleteConnectivity(Face);
      entities[Face] = new intArray;

      int nfaces = constructFaceEntityFromRegion(*entities[Face], downward, adjacencyOrientation[Region][Face], 
						 upwardIDX, upwardOffset, adjacencyOrientation[Face][Region],
						 *entities[Region], size(Region), maxNumberOfNodesPerFace, size(Vertex)-1);

      entityCapacity[Face] = entitySize[Face] = nfaces;
      entityCapacity[Face] = entities[Face]->getLength(0);

      if ( keepDownward )
	{
	  indexLists[Region][Face] = new intArray;
	  indexLists[Region][Face]->reference(downward);
	}
      else
	{
	  delete [] adjacencyOrientation[Region][Face];
	  adjacencyOrientation[Region][Face] = 0;
	}

      if ( keepUpward )
	{
	  indexLists[Face][Region] = new intArray;
	  upwardOffsets[Face][Region] = new intArray;
	  
	  indexLists[Face][Region]->reference(upwardIDX);
	  upwardOffsets[Face][Region]->reference(upwardOffset);
	}
      else
	{
	  delete [] adjacencyOrientation[Face][Region];
	  adjacencyOrientation[Face][Region] = 0;
	}

      // link to the old connectivity
      if ( !face.getLength(0) ) 
	face.reference(*entities[Face]);

      break;
    }
  case Region:
    {
      break;
    }
  default:
    return false;
  }

  if ( (!entityMasks[type] && type<=domainDimension) ) // *wdh* 030108 : there are no regions in 2D
  {
    if ( entityMasks[type] )
      entityMasks[type]->resize(capacity(type));
    else
      {
	entityMasks[type] = new intArray(capacity(type));
      }
    *entityMasks[type] = 0;
  }

  if ( domainDimension>int(type) ) tagEntitiesFromVerts( *this, type );


  return true;
}

bool 
UnstructuredMapping::
specifyEntity(const EntityTypeEnum type, const intArray &entity)
{
  deleteConnectivity(type);

  entities[type] = new intArray;

  *entities[type] = entity;

  if ( type==Face && entity.getLength(1)!=4 )
    {
      Range R(entity.getLength(0));
      Range E(entity.getLength(1));

      entities[type]->resize(R,4);
      *entities[type] = -1;
      (*entities[type])(R,E) = entity;
    }

  indexLists[type][Vertex] = new intArray;
  indexLists[type][Vertex]->reference(*entities[type]);

  entityCapacity[type]=entitySize[type]=entity.getLength(0);

  entityMasks[type] = new intArray(capacity(type));
  *(entityMasks[type]) = 0;

  if ( domainDimension>int(type) ) tagEntitiesFromVerts( *this, type );

  return true;
}

/// connectivityBuilder directs the construction of the connectivity arrays, it returns true if successfull
/*** connectivityBuilder will allocate the space for and build the upward or downward connectivities requested.
 *   If rebuild=true, it will destroy any previously created connectivity and regenerate it.
 */
bool 
UnstructuredMapping::
buildConnectivity(EntityTypeEnum from, EntityTypeEnum to, bool rebuild)
{
  if ( !rebuild && connectivityExists(from,to) )
    return true;

  deleteConnectivity(from,to);

  if ( !size(to) ) buildEntity(to);
  if ( !size(from) ) buildEntity(from);

  if ( from<to )
    {

      if ( !entities[to] ) return false; // we don't have enough information

      if ( !indexLists[to][from] )
	buildConnectivity(to,from,true);

      indexLists[from][to] = new intArray;
      upwardOffsets[from][to] = new intArray;

      intArray &downward = from==Vertex ? *entities[to] : *indexLists[to][from];

      constructUpwardAdjacenciesFromDownward(*indexLists[from][to], *upwardOffsets[from][to], adjacencyOrientation[from][to], 
					     downward, adjacencyOrientation[to][from], size(from));

      if ( false && from==Edge )
	{
	  UnstructuredMappingIterator e,e_end;
	  UnstructuredMappingAdjacencyIterator ef,ef_end,fe,fe_end;
	  e_end = end(Edge);
	  for ( e=begin(Edge); e!=e_end; e++ )
	    {
	      cout<<"Edge : "<<*e<<endl;
	      ef_end = adjacency_end(e,Face);
	      for ( ef=adjacency_begin(e,Face); ef!=ef_end; ef++ )
		{
		  cout<<"  "<<*ef<<"  orient = "<<ef.orientation()<<endl;
		  cout<<"  reverse : ";
		  fe_end = adjacency_end(ef,Edge);
		  for ( fe=adjacency_begin(ef,Edge); fe!=fe_end; fe++ )
		    if ( *fe==*e ) cout<<"  "<<*fe<<" orient="<<fe.orientation();
		  cout<<endl;
		}
	    }
	}

      return true;
    }

  switch(from) {
  case Vertex:
    {
      // there is no downward from here!
      break;
    }
  case Edge:
    {
      if ( !entities[Edge] || rebuild ) buildEntity(Edge, rebuild, true);
      break;
    }
  case Face:
    {
      if ( !entities[Face] || rebuild ) buildEntity(Face, rebuild, true);
      
      if ( to==Edge )
	{
	  if ( entities[Region] )
	    {
	      intArray dumA, dumA2;
	      char *dumC=0;

	      indexLists[from][to] = new intArray;
	      int ok =constructFace2EdgeFromRegions(*indexLists[from][to], adjacencyOrientation[from][to],
						    dumA, dumA2, dumC, *indexLists[Region][Edge], adjacencyOrientation[Region][Edge],
						    *indexLists[Region][Face],
						    *entities[Face], *entities[Edge], *entities[Region], size(Region), size(Face), size(Vertex));
	      assert(ok==0);
	      if ( dumC ) delete [] dumC;

	    }
	  else
	    buildEntity(Edge, true, true, true);
	}
      // XXX else add generic downward builder here!
      break;
    }
  case Region:
    {
      if ( !entities[Region] )
	return false;

      switch(to) {
      case Vertex:
	{
	  /// we always have this if there are Regions
	  break;
	}
      case Edge:
	{
	  if ( !entities[Edge] || rebuild ) buildEntity(Edge, rebuild, true);

	  // XXX else add generic downward builder here!
	  break;
	}
      case Face:
	{
	  if ( !entities[Face] || rebuild ) buildEntity(Face, rebuild, true);
	  // XXX else add generic downward builder here!
	  break;
	}
      default:
	return false;
      }

      break;
    }
  default:
    return false;
  }

  return true;
}

/// specifyConnectivity tells the mapping to use the given connectivity information rather than building it
/*** specifyConnectivity tells the mapping to use the given connectivity information rather than building it.
 *   it returns false only if the given connectivity makes no sense.
 */
bool 
UnstructuredMapping::
specifyConnectivity(const EntityTypeEnum from, const EntityTypeEnum to, const intArray &index, 
		    const char *orientation, const intArray &offset)
{
  deleteConnectivity(from,to);

  if ( orientation )
    {
      adjacencyOrientation[from][to] = new char[index.getLength(0)*index.getLength(1)];
      
      char *op = adjacencyOrientation[from][to];
      for ( int i=0; i<index.getLength(0)*index.getLength(1); i++ ) op[i] = orientation[i];
    }

  if ( from>to )
    {
      indexLists[from][to] = new intArray;
      *indexLists[from][to] = index;
    }
  else
    {
      if ( offset.getDataPointer()==Overture::nullIntArray().getDataPointer() ) return false;

      indexLists[from][to] = new intArray;
      upwardOffsets[from][to] = new intArray;
      
      *indexLists[from][to] = index;
      *upwardOffsets[from][to] = offset;
    }
  return true;
}

/// delete specific connectivity information
void 
UnstructuredMapping::
deleteConnectivity(EntityTypeEnum from, EntityTypeEnum to)
{
  if ( from>to )
    {
      if (indexLists[from][to]) delete indexLists[from][to];
      indexLists[from][to] = 0;
    }
  else
    {
      if (indexLists[from][to]) delete indexLists[from][to];
      if (upwardOffsets[from][to]) delete upwardOffsets[from][to];
      indexLists[from][to] = upwardOffsets[from][to] = 0;
    }

  if ( adjacencyOrientation[from][to] ) delete [] adjacencyOrientation[from][to];
  adjacencyOrientation[from][to] = 0;
}

/// delete all connectivity information for a specific entity type
void 
UnstructuredMapping::
deleteConnectivity(EntityTypeEnum type)
{
  int i_et = 0;
  for ( EntityTypeEnum et=Vertex; et<=Region; et=EntityTypeEnum(++i_et))
    {
      if ( et<type )
	{
	  if (indexLists[type][et]) delete indexLists[type][et];
	  indexLists[type][et] = 0;
	}
      else
	{
	  if (indexLists[type][et]) delete indexLists[type][et];
	  if (upwardOffsets[type][et]) delete upwardOffsets[type][et];
	  
	  indexLists[type][et] = upwardOffsets[type][et] = 0;
	}

      if ( adjacencyOrientation[type][et] ) delete [] adjacencyOrientation[type][et];
      adjacencyOrientation[type][et] = 0;
    }

  if ( entities[type] ) 
    {
      for ( int e=0; e<size(type); e++ )
	{
	  entity_tag_iterator tg=entity_tag_begin(type,e);
	  while( tg!=entity_tag_end(type,e) )
	    {
	      deleteTag(type,e,**tg);
	      tg = entity_tag_begin(type,e);
	    }
	}

      delete entities[type];
      entities[type] = 0;
    }

  if ( entityMasks[type] ) 
    {
      delete entityMasks[type];
      entityMasks[type] = 0;
    }
  
  entitySize[type] = entityCapacity[type] = 0;
}

/// delete ALL the connectivity information
void 
UnstructuredMapping::
deleteConnectivity()
{
  int i_et=0;
  for ( EntityTypeEnum et=Vertex; et<=Region; et=EntityTypeEnum(++i_et) )
    deleteConnectivity(et);

  // clean up the tagging
  maintainTagToEntityMap(false);
  
  for ( int eti=int(UnstructuredMapping::Vertex); 
	eti<(UnstructuredMapping::Mesh); eti++ )
    {
      
      UnstructuredMapping::EntityTypeEnum et=UnstructuredMapping::EntityTypeEnum(eti);
      UnstructuredMappingIterator it;
      
     if ( size((EntityTypeEnum)eti) )
       for ( it = begin(et); it!=end(et); it++ )
	 {
	   entity_tag_iterator tagit=entity_tag_begin(et,*it);
	   while ( tagit!=entity_tag_end(et,*it) )
	     {
	       entity_tag_iterator tag2delete = tagit;
	       deleteTag(et, *it, (*tag2delete)->getName());
	       tagit = entity_tag_begin(et,*it);
	     }
	 }
    }
  
  entity_tag_iterator tagit=entity_tag_begin(Mesh,0);
  while ( tagit!=entity_tag_end(Mesh,0) )
    {
     entity_tag_iterator tag2delete = tagit;
     deleteTag(Mesh, 0, (*tag2delete)->getName());
     tagit = entity_tag_begin(Mesh,0);
    }

 entityTags.clear();

}

/// expand the ghost boundary by a layer
void 
UnstructuredMapping::
expandGhostBoundary( int bc /*=-1*/ )
{
  EntityTypeEnum boundingEntityType = EntityTypeEnum(domainDimension-1);

  if ( !size(boundingEntityType) ) buildEntity(boundingEntityType);
					     
  //    (*entities[domainDimension]).display("entities before ghost");

  maintainTagToEntityMap(true);

  if ( !hasTag(Mesh,0,"number of ghost layers") )
    addTag(Mesh,0,"number of ghost layers",0);

  int nGhostLayers = (intptr_t)getTagData(Mesh,0,"number of ghost layers");
  //cout<<"there are "<<nGhostLayers<<endl;
  std::string bdyEntTag;
  std::string bdyVertTag;
  std::string entTag;
  std::string tagPrefix = "boundary ";
  for ( int g=0; g<nGhostLayers; g++ )
    tagPrefix = "Ghost "+tagPrefix;

  bdyEntTag = tagPrefix + EntityTypeStrings[int(boundingEntityType)].c_str();
  bdyVertTag = tagPrefix + EntityTypeStrings[int(Vertex)].c_str();
  entTag = tagPrefix + EntityTypeStrings[domainDimension];

  cout<<"BDY ENT TAG = ("<<bdyEntTag<<")"<<endl;
  string newBdyEntTag  = "Ghost "+bdyEntTag;
  string newBdyVertTag = "Ghost "+bdyVertTag;

  //  cout<<bdyEntTag<<endl<<bdyVertTag<<endl<<entTag<<endl;

  int nNewCells = tagEntities[bdyEntTag].size();
  int nNewVerts = tagEntities[bdyVertTag].size();
  cout<<"adding "<<nNewCells<<" new cells and "<<nNewVerts<<" new verts"<<endl;
  
  if ( !nNewCells )
    {
      cout<<"ERROR : UnstructuredMapping::expandGhostBoundaries : there are no "<<
	EntityTypeStrings[int(boundingEntityType)]<<" entities bounding the mesh "<<endl;
      return;
    }
  
  std::string extrudedVertexTag = "extruded vertex";  // we will use this to keep track of new vertex ids
  reserve(Vertex,size(Vertex)+nNewVerts); // reserve some space, keep some extra for later use.
  reserve(EntityTypeEnum(domainDimension), size(EntityTypeEnum(domainDimension))+nNewCells);

  ArraySimpleFixed<real,3,1,1,1> normal;
  ArraySimple< ArraySimpleFixed<real,3,1,1,1> > fv(maxVerticesInEntity(Face));

  void *IDForVertex;
  IDForVertex=(void*)-1;
  int newVertID = size(Vertex);

  UnstructuredMappingIterator cell, cell_end, cellBound;
  UnstructuredMappingAdjacencyIterator edge, vert, vert_end, adj,adj_end;

  ArraySimple<int> newEnt(maxVerticesInEntity(EntityTypeEnum(domainDimension)));

  bool filterbc = bc > -1;

  tag_entity_iterator tag_bdy_end = tag_entity_end(bdyEntTag);
  int nadded = 0;
  for ( tag_entity_iterator bdy=tag_entity_begin(bdyEntTag);
	bdy!=tag_bdy_end; bdy++ )
    {
      bool okToExtrude = true;
      if ( bc != -1 )
	{
	  vert_end = adjacency_end(bdy->et, bdy->e, Vertex);
	  for ( vert=adjacency_begin(bdy->et, bdy->e, Vertex); okToExtrude && vert!=vert_end; vert++ )
	    okToExtrude = getBC(Vertex,*vert)==bc;
	}

      // first compute the normal and average edge length

      // get the sign for the normal from the adjacency information
      if ( okToExtrude )
	{
	  UnstructuredMappingAdjacencyIterator adj = adjacency_begin(bdy->et, bdy->e, EntityTypeEnum(domainDimension));
	  int signForNormal = adj.orientation();

	  normal = 0;
	  real step = 1;
	  int nv=0;
	  real fac=.5;
	  if ( rangeDimension==2 )
	    {
	      const intArray &edg = getEntities(Edge);
	      normal[0] = (node(edg(bdy->e,1),1)-node(edg(bdy->e,0),1));
	      normal[1] = -(node(edg(bdy->e,1),0)-node(edg(bdy->e,0),0));
	      step = fac*sqrt(ASmag2(normal));
	      nv=2;
	    }
	  else if ( domainDimension==3 )
	    {
	      fac=.15;
	      vert_end = adjacency_end(bdy->et, bdy->e, Vertex);
	      ArraySimpleFixed<real,3,1,1,1> xc;
	      xc = 0;
	      nv = 0;
	      for ( vert=adjacency_begin(bdy->et, bdy->e, Vertex); vert!=vert_end; vert++ )
		{
		  for ( int a=0; a<rangeDimension; a++ )
		    fv[nv][a] = node(*vert,a);
		  nv++;
		}

	      if ( nv==3 )
		{
		  normal = areaNormal3D(fv[0],fv[1],fv[2]);
		}
	      else
		{
		  normal = areaNormal3D(fv[0],fv[1],fv[2]);

		  ArraySimpleFixed<real,3,1,1,1> n2 = areaNormal3D(fv[2],fv[3],fv[0]);
		  for ( int a=0; a<rangeDimension; a++ )
		    normal[a] = n2[a] + normal[a];
		}
	  
	      step = fac*sqrt(sqrt(ASmag2(normal))/real(nv));
	    }
	  else if ( rangeDimension==3 && domainDimension==2 )
	    {
	      cout<<"ERROR : UnstructuredMapping::expandGhostBoundary : does not work for surfaces "<<endl;
	    }

	  real nmag = sqrt(ASmag2(normal));
	  
	  for ( int a=0; a<rangeDimension; a++ )
	    normal[a] *= signForNormal*step/nmag;

	  // make the entity and vertices 
	  newEnt=-1;
	  vert_end = adjacency_end(bdy->et, bdy->e, Vertex);
	  int n=0;
	  for ( vert=adjacency_begin(bdy->et, bdy->e, Vertex); vert!=vert_end; vert++ )
	    {
	      if ( hasTag(Vertex,*vert,extrudedVertexTag) )
		{
		  IDForVertex = getTagData(Vertex,*vert,extrudedVertexTag);
		  for ( int a=0; a<rangeDimension; a++ )
		    node(intptr_t(IDForVertex),a) = node(intptr_t(IDForVertex),a) + normal[a];

		  if ( bc!=-1 )
		    setBC( Vertex, intptr_t(IDForVertex), bc );
		}
	      else
		{
		  if (rangeDimension==2)
		    IDForVertex = (void*)addVertex(node(*vert,0)+normal[0],node(*vert,1)+normal[1]);
		  else
		    IDForVertex = (void*)addVertex(node(*vert,0)+normal[0],node(*vert,1)+normal[1],node(*vert,2)+normal[2]);

		  //		  cout<<"ADDED VERTEX "<<(int)IDForVertex<<endl;
		  addTag(Vertex,*vert,extrudedVertexTag,IDForVertex);
		  setAsGhost(Vertex,(intptr_t)IDForVertex);
		  addTag(Vertex,(intptr_t)IDForVertex,newBdyVertTag,0);
		  //	      addTag(Vertex,(int)IDForVertex,"ghost "+bdyVertTag,0);
		}

	      //	  newEnt(n) = *vert;
	      if ( signForNormal<0 )
		{
		  if ( domainDimension==2 )
		    {
		      newEnt(n) = *vert;
		      newEnt(2*nv-1-n) = intptr_t(IDForVertex);
		    }
		  else
		    {
		      newEnt(n) = intptr_t(IDForVertex);
		      newEnt(n+nv) = *vert;
		      //		      newEnt((nv-n)%nv) = *vert;
		      //		      newEnt(nv+(nv-n)%nv) = int(IDForVertex);
		    }
		}
	      else
		{
	      
		  if ( domainDimension==2 )
		    {
		      newEnt((nv-n+1)%nv) = *vert;
		      newEnt(2*nv-1-(nv-n+1)%nv) = intptr_t(IDForVertex);
		    }
		  else
		    {
		      newEnt(n) = *vert;
		      newEnt(n+nv) = intptr_t(IDForVertex);
		    }
		}
	  
	      n++;
	    }

	  int newEntIndex = addEntity(EntityTypeEnum(domainDimension), newEnt);
	  //cout<<"new entity has id "<<newEntIndex<<endl<<newEnt<<endl;
	  nadded++;
	  setAsGhost(EntityTypeEnum(domainDimension), newEntIndex);

	  //      addTag(EntityTypeEnum(domainDimension), newEntIndex, std::string("ghost ")+entTag,0);
	  
	  //      cout<<"size of entity storage "<<(*entities[domainDimension]).getLength(0)<<endl;;
      
	}
    }
  //    (*entities[domainDimension]).display("entities");
  cout<<"NENTS ADDED = "<<nadded<<endl;
  //  if ( tags.getLength(0) )
    {
      int oldSize = tags.getLength(0);
      if ( oldSize )
	{
	  tags.resize( oldSize + nadded ); 
	  Range R(oldSize,oldSize+nadded-1);
	  tags(R) = -1;
	}
      else
	{
	  // *wdh* 060529 tags.resize( size(domainDimension) );
	  tags.resize( size(EntityTypeEnum(domainDimension)) );
	  tags = -1;
	}
    }

  // for now, force a rebuild of the connectivity when needed next (so, what happens to the tags...)
  //kkc 040430 tagBoundaryEntities will take care of all entities as long as
  //           the ghost tags are correct for ( int e=domainDimension-1; e>0; e-- )
  for ( int e=domainDimension; e>0; e-- )
    {
      std::string tag = std::string("boundary ")+EntityTypeStrings[e].c_str();
      tag_entity_iterator ei = tag_entity_begin(tag);

      while( (ei=tag_entity_begin(tag)) != tag_entity_end(tag) )
	if ( deleteTag(ei->et,ei->e,tag) ) abort();
      //      deleteConnectivity(EntityTypeEnum(e));
    }

  for ( int e=domainDimension-1; e>0; e-- )
    {
      deleteConnectivity(EntityTypeEnum(e));
      deleteConnectivity(Vertex, EntityTypeEnum(e));
    }

  // make old and new connectivities consistent
  if ( this->edge.getLength(0) )
    {
      buildEntity(Edge);
      this->edge.redim(0);
      this->edge = getEntities(Edge);
    }

  // this will retag the boundaries in the new connectivity
  tagBoundaryEntities(*this);

  // loop through the new ghost entities and add any new, lowerD entities.
#if 0
  tag_entity_iterator bdy_end = tag_entity_end("ghost "+tagPrefix+EntityTypeStrings[domainDimension].c_str());
  int nbdy=0;
  for ( tag_entity_iterator bdy=tag_entity_begin("ghost "+tagPrefix+EntityTypeStrings[domainDimension].c_str());
	bdy!=bdy_end; bdy++ )
    {
      for ( int d=domainDimension-1; d>0; d-- )
	{
	  adj_end = adjacency_end(bdy->et, bdy->e,EntityTypeEnum(d));
	  for ( adj=adjacency_begin(bdy->et, bdy->e,EntityTypeEnum(d));
		adj!=adj_end;
		adj++ )
	    {
	      bool isBdy=true;
	      bool isGhost=false;
	      vert_end = adjacency_end(EntityTypeEnum(d), *adj,Vertex);
	      for ( vert=adjacency_begin(EntityTypeEnum(d), *adj,Vertex);
		    vert!=vert_end;
		    vert++ )
		if ( hasTag(Vertex, *vert, "ghost "+bdyVertTag) )
		  isGhost = true;
		else
		  isBdy = false;

	      if ( isGhost && !hasTag(EntityTypeEnum(d), *adj, std::string("Ghost ")+EntityTypeStrings[EntityTypeEnum(d)].c_str()) )
		setAsGhost(EntityTypeEnum(d),*adj);
	      
	      if ( isBdy )
		{	
		  std::string tag = "ghost "+tagPrefix+EntityTypeStrings[d].c_str();
		  if ( !hasTag(EntityTypeEnum(d), *adj, tag) )
		    addTag(EntityTypeEnum(d), *adj, tag,0);
		  nbdy++;
		}

	      
	    }
	} 
    } 
#endif

  setTagData(Mesh,0,"number of ghost layers",(void *)++nGhostLayers); 

  tag_entity_iterator ei;

  while( (ei=tag_entity_begin(extrudedVertexTag)) != tag_entity_end(extrudedVertexTag) )
    if ( deleteTag(ei->et,ei->e,extrudedVertexTag) ) abort();  

  int nbdyv = 0;
  int nbdye = 0;
  for ( ei=tag_entity_begin(newBdyVertTag); ei!=tag_entity_end(newBdyVertTag); ei++ )
    {
      nbdyv++;
      UnstructuredMappingAdjacencyIterator bit,bit_end;
      bit_end=adjacency_end(ei->et, ei->e, boundingEntityType);
      for ( bit=adjacency_begin(ei->et, ei->e, boundingEntityType); bit!=bit_end; bit++ )
	{
	  UnstructuredMappingAdjacencyIterator tmpi;
	  tmpi = adjacency_begin(bit, EntityTypeEnum(domainDimension));
	  int nadj = tmpi.nAdjacent();

// 	  cout<<"nadj = "<<nadj<<endl;
// 	  while (tmpi!=adjacency_end(bit,EntityTypeEnum(domainDimension)))
// 	    {
// 	      cout<<"edge("<<*bit<<") face is "<<*tmpi<<endl;
// 	      tmpi++;
// 	    }
	  if ( !hasTag(boundingEntityType, *bit, newBdyEntTag) && nadj==1 )
	    {
	      addTag(boundingEntityType,*bit,newBdyEntTag,0);
	      //	      cout<<"shoul dhave added a "<<newBdyEntTag<<" at "<<*bit<<endl;
	      nbdye++;
	    }
	}
    }
  cout<<"NEW BDY VERTS("<<newBdyVertTag<<") = "<<nbdyv<<endl;
  cout<<"NEW BDY ENTS("<<newBdyEntTag<<") = "<<nbdye<<" umap says "<<tagEntities[newBdyEntTag].size()<<endl;

  // fix up some old FEZ stuff so older code still works
  numberOfElements = size(EntityTypeEnum(domainDimension));
  element.redim(0);
  element = *entities[EntityTypeEnum(domainDimension)];
  FEZComputeElementTypes();
  
  Range R(0,size(Vertex)-1);
  for( int axis=0; axis<rangeDimension; axis++ )
    {
      setRangeBound(Start,axis,min(node(R,axis)));
      setRangeBound(End  ,axis,max(node(R,axis)));
    }

}

int 
UnstructuredMapping::
reserve(EntityTypeEnum type, int amt)
{

  //  cout<<"before reserve, capacity is  "<<capacity(type)<<endl;
  if ( capacity(type)>=amt ) 
    return capacity(type);

  if ( type==Vertex )
    {
      node.resize(amt, rangeDimension);
      entityCapacity[int(type)] = node.getLength(0);
      Range all(rangeDimension);
      node(Range(size(type),capacity(type)-1),all) = 0;
    }
  else
    {
      if ( !entities[type] )
	entities[type] = new intArray;

      entities[int(type)]->resize(amt, maxVerticesInEntity(type));
      entityCapacity[int(type)] = entities[int(type)]->getLength(0);
      Range all(maxVerticesInEntity(type));
      
      (*entities[int(type)])(Range(size(type),capacity(type)-1),all) = -1;
    }

  if ( !entityMasks[type] )
    entityMasks[type] = new intArray;

  entityMasks[int(type)]->resize(capacity(type));
  (*entityMasks[int(type)])(Range(size(type),capacity(type)-1)) = int(NullEntityInfo);

  // what about the connectivity arrays ... ???

  //  cout<<"reserve to new size of "<<capacity(type)<<endl;

  return capacity(type);
}

int 
UnstructuredMapping::
addVertex(real x, real y, real z /*=0.*/)
{
  if ( size(Vertex)==capacity(Vertex) )
    reserve(Vertex,size(Vertex)+100);

  node(size(Vertex),0) = x;
  node(size(Vertex),1) = y;
  if ( rangeDimension==3 )
    node(size(Vertex),2) = z;
  
  entitySize[int(Vertex)]++;
  numberOfNodes++;

  return size(Vertex)-1;
}

int
UnstructuredMapping::
addEntity(EntityTypeEnum type, ArraySimple<int> &newEntVerts)
{
  if ( size(type)==capacity(type) )
    reserve(type,size(type)+100);

  if ( newEntVerts.size(0) > maxVerticesInEntity(type) )
    {
      cout<<"ERROR : UnstructuredMapping::addEntity : too many vertices specified for new "<<EntityTypeStrings[int(type)]<<endl;
      return -1;
    }

  int id=size(type);

  // ok, here we could use the data structure holes but lets make it simple for now...
  for ( int v=0; v<maxVerticesInEntity(type); v++ )
    (*entities[int(type)])(id,v) = newEntVerts(v);

  if ( entityMasks[type] )
    (*entityMasks[type])(id) = 0;

  entitySize[int(type)]++;
  if ( int(type)==domainDimension ) numberOfElements++;

  // what about the connectivity arrays ... ???

  return id;
}
