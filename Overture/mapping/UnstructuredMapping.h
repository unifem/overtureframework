#ifndef UNSTRUCTURED_MAPPING_H
#define UNSTRUCTURED_MAPPING_H 

#include "OvertureDefine.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <map>
#include <list>
#include <string>
#include <vector>
#else
#include <map.h>
#include <list.h>
#include <string>
#include <vector.h>
#endif

#include "Mapping.h"
#include "EntityTag.h"
#include "ArraySimple.h"
// #include "CompositeGrid.h"

class MappingProjectionParameters;
class CompositeSurface;
class GeometricADT3dInt;
class CompositeGrid;
class UnstructuredMappingIterator;
class UnstructuredMappingAdjacencyIterator;



//-------------------------------------------------------------
/// \brief Define a Mapping for unstructured surfaces and volumes.
//-------------------------------------------------------------
class UnstructuredMapping : public Mapping
{

public:

  enum EntityTypeEnum
  {
    Invalid=-1,
    Vertex=0,
    Edge,
    Face,
    Region,
    Mesh, // kkc put this here to enable "Mesh" tagging...
    NumberOfEntityTypes
  };

  /// an array usefull for diagnostics involving EntityTypeEnum
  static aString EntityTypeStrings[];

  enum ElementType 
  {
    triangle,
    quadrilateral,
    tetrahedron,
    pyramid,
    triPrism,
    septahedron,  
    hexahedron,
    other,
    boundary,
    NumberOfElementTypes
  };

  /// an array usefull for diagnostics involving ElementType
  static aString ElementTypeStrings[];

  /// \brief define an entity ID tuple
  class IDTuple 
  { 
  public:
    EntityTypeEnum et; int e; 

    inline IDTuple(EntityTypeEnum et_=Invalid, int e_=-1) : et(et_), e(e_) { }
    inline IDTuple( const IDTuple &id ) : et(id.et), e(id.e) { }
    inline ~IDTuple() {}
    
    inline bool operator< ( const IDTuple & id ) const 
    { return et<id.et ? true : ( et==id.et ?  e<id.e : false) ; }

    inline bool operator< ( const IDTuple & id ) 
    { return et<id.et ? true : ( et==id.et ?  e<id.e : false) ; }
    
    inline bool operator== ( const IDTuple & id ) const 
    { return (et==id.et && e==id.e); }

    inline bool operator== ( const IDTuple & id ) 
    { return (et==id.et && e==id.e); }

    inline bool operator!= ( const IDTuple & id ) const 
    { return !(*this==id); }

    inline bool operator!= ( const IDTuple & id ) 
    { return !(*this==id); }
    
  };
  
  struct TagError { }; // for exception handling
  
  UnstructuredMapping();
  
  UnstructuredMapping(int domainDimension_ /* =3 */, 
		      int rangeDimension_ /* =3 */, 
		      mappingSpace domainSpace_ /* =parameterSpace */,
		      mappingSpace rangeSpace_ /* =cartesianSpace */ );
  
  // Copy constructor is deep by default
  UnstructuredMapping( const UnstructuredMapping &, const CopyType copyType=DEEP );
  
  ~UnstructuredMapping();
  
  UnstructuredMapping & operator =( const UnstructuredMapping & X0 );
  
  void addGhostElements( bool trueOrFalse=true );
  //
  // overload of this method, it doesn't really make sense for UnstructuredMappings (?)
  // watch out, the base class method Mapping::getGrid actually calls Mapping::map, which
  // is private in UnstructuredMapping since it makes little sense.
  //
  virtual const realArray& getGrid(MappingParameters & params=Overture::nullMappingParameters(),
                                   bool includeGhost=false) { return node; }
  
  int getNumberOfNodes() const;
  
  int getNumberOfElements() const;
  
  int getNumberOfFaces() const;
  
  int getNumberOfEdges() const;

  int getMaxNumberOfNodesPerElement() const;
  
  int getMaxNumberOfFacesPerElement() const;
  
  int getMaxNumberOfNodesPerFace() const;
  
  int getNumberOfBoundaryFaces() const;

  inline int size( EntityTypeEnum t ) const { return t<Mesh ? entitySize[int(t)] : 1; }
  inline int capacity( EntityTypeEnum t ) const { return t<Mesh ? entityCapacity[int(t)] : 1; }
  int reserve( EntityTypeEnum, int);

  void setPreferTriangles( bool trueOrFalse=true ){ preferTriangles=trueOrFalse;}
  void setElementDensityTolerance(real tol);
  
  const realArray & getNodes() const;
  const intArray & getElements() const;
  const intArray & getFaces();
  const intArray & getFaceElements();
  const intArray & getEdges();
  const intArray & getTags();
  const intArray & getElementFaces();
  const intArray & getBoundaryFace();
  const intArray & getBoundaryFaceTags();

  const intArray & getEntities(EntityTypeEnum);
  const intArray & getEntityAdjacencyIndices(EntityTypeEnum from, EntityTypeEnum to, intArray &offsets);

  const intArray & getGhostElements() const;
  const intArray & getMask(EntityTypeEnum entityType) const;

  // dynamically created connectivities
  void createNodeElementList(intArray &nodeElementList);

  // iteration helpers
  inline int getNumberOfNodesThisElement(int element) const;
  inline int getNumberOfNodesThisFace(int face_) const;
  inline int getBoundaryFace(int bdyface_) const;
  inline int getBoundaryFaceTag(int bdyface_) const;
  inline int elementGlobalVertex(int element, int vertex) const;
  inline int faceGlobalVertex(int face, int vertex) const;
  inline int getNumberOfFacesThisElement(int element_) const;

  inline UnstructuredMappingIterator begin(EntityTypeEnum entityType_, bool skipGhostEntities=false) const;
  inline UnstructuredMappingIterator end(EntityTypeEnum entityType_, bool skipGhostEntities=false) const;

  inline UnstructuredMappingAdjacencyIterator adjacency_begin(EntityTypeEnum fromT, int fromE, EntityTypeEnum to, 
							      bool skipGhostEntities=false) const;

  inline UnstructuredMappingAdjacencyIterator adjacency_end(EntityTypeEnum fromT, int fromE, EntityTypeEnum to, 
							    bool skipGhostEntities=false) const;

  inline UnstructuredMappingAdjacencyIterator adjacency_begin(UnstructuredMappingIterator from, EntityTypeEnum to, 
							      bool skipGhostEntities=false) const;
  inline UnstructuredMappingAdjacencyIterator adjacency_end(UnstructuredMappingIterator from, EntityTypeEnum to, 
							    bool skipGhostEntities=false) const;

  inline UnstructuredMappingAdjacencyIterator adjacency_begin(UnstructuredMappingAdjacencyIterator from, EntityTypeEnum to, 
							      bool skipGhostEntities=false) const;
  inline UnstructuredMappingAdjacencyIterator adjacency_end(UnstructuredMappingAdjacencyIterator from, EntityTypeEnum to, 
							    bool skipGhostEntities=false) const;

  inline UnstructuredMappingAdjacencyIterator adjacency_begin(IDTuple from, EntityTypeEnum to, 
							      bool skipGhostEntities=false) const;
  inline UnstructuredMappingAdjacencyIterator adjacency_end(IDTuple from, EntityTypeEnum to, 
							    bool skipGhostEntities=false) const;



  // rotate about a given axis
  int rotate( const int & axis, const real & theta );

  // scale the mapping
  int scale(const real & scalex=1., 
	    const real & scaley=1., 
	    const real & scalez=1. );

  // shift in space
  int shift(const real & shiftx=0., 
	    const real & shifty=0., 
	    const real & shiftz=0. );

  int setNodesAndConnectivity( const realArray & nodes, 
                               const intArray & elements,
			       int domainDimension =-1,
			       bool buildConnectivity =true);

  int setNodesAndConnectivity( const realArray & nodes, 
                               const intArray & elements,
			       const intArray & faces,
                               const intArray & faceElements,
                               const intArray & elementFaces,
                               int numberOfFaces=-1,
                               int numberOfBoundaryFaces=-1,
                               int domainDimension =-1,
                               bool constantNumberOfNodesPerElement=false );


  int setNodesElementsAndNeighbours(const realArray & nodes, 
				    const intArray & elements, 
				    const intArray & neighbours,
                                    int numberOfFaces=-1,
                                    int numberOfBoundaryFaces=-1,
				    int domainDimension =-1 );

  void setTags(const intArray &new_tags);

  int splitElement( int e, int relativeEdge, real *x ); // add a new node that splits an edge.
  

  // determine if points are inside or outside a (closed) triangulation using ray tracing
  int insideOrOutside( realArray & x, IntegerArray & inside );

  virtual int intersects(Mapping & map2, 
			 const int & side1=-1, 
			 const int & axis1=-1,
			 const int & side2=-1, 
			 const int & axis2=-1,
			 const real & tol=0. ) const;
  
  // project points onto the surface
  int project( realArray & x, MappingProjectionParameters & mpParameters );
  
  int findClosestEntity( UnstructuredMapping::EntityTypeEnum etype, real x, real y, real z=0. );

  int printConnectivity( FILE *file =stdout );
  int printStatistics(FILE *file =stdout );

  int checkConnectivity( bool printResults=true, IntegerArray *pBadElements=NULL );
  
  void getNormal( int e, real *normalVector );  // determine the normal to an element 

  //IntegerArray buildFromAMapping( Mapping & map, int elementType=-1 );
  intArray buildFromAMapping( Mapping & map, intArray &maskin = Overture::nullIntegerDistributedArray() );

  // an optimized build for domainDimension==2:
  int buildFromARegularMapping( Mapping & map, ElementType elementTypePreferred=triangle );  
  int buildUnstructuredGrid( Mapping & map, int numberOfGridPoints[2]);
  
  void buildFromACompositeGrid( CompositeGrid &cg );
  int buildFromACompositeSurface( CompositeSurface & mapping );

  int findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  // read an unstructured grid from an ascii file.
  virtual int get( const aString & fileName );

  // save the unstructured grid to an ascii file.
  virtual int put(const aString & fileName = nullString ) const;

  Mapping *make( const aString & mappingClassName );

  int update( MappingInformation & mapInfo ) ;

  inline ElementType getElementType(int e) const;

  virtual aString getClassName() const { return UnstructuredMapping::className; }

  // put this utility routine here so it can be used by other codes. It does not need to be
  // a member function. ** this could probably be done in a better way with a namespace **
  static bool projectOnTriangle( real *x0, real *x1, real *x2,  
				 real *xa, real *xb,  
				 real *xt, 
				 int & intersectionFace, int & intersectionFace2, 
				 real & r0, real & s0,
				 real *normal=NULL );
// set & get colour
  aString getColour( ) const;
  int setColour( const aString & colour);

// AP display lists...
  enum DisplayListProperty{nodeDL=0, edgeDL, boundaryEdgeDL, faceDL, faceNormalDL, numberOfDLProperties};

  int
  getDisplayList(DisplayListProperty d){if (d<numberOfDLProperties) return dList[d]; else return 0;};

  void
  setDisplayList(DisplayListProperty d, int l){if (d<numberOfDLProperties && l>0) dList[d]=l;};

  void
  eraseUnstructuredMapping(GenericGraphicsInterface &gi);

  // kkc tagging support added 0802
  inline bool dumpTags() const { return dumpTagsToHDF; }
  inline bool dumpTags( bool dt ) { dumpTagsToHDF = dt; return dumpTagsToHDF; }

  EntityTag & addTag( const EntityTypeEnum entityType, const int entityIndex, const std::string tagName,
		      const void *tagData, const bool copyTag=false, const int tagSize=0 );
  int deleteTag( const EntityTypeEnum entityType, const int entityIndex, const EntityTag &tagToDelete );
  int deleteTag( const EntityTypeEnum entityType, const int entityIndex, const std::string tagToDelete );
  
  bool hasTag( const EntityTypeEnum entityType, const int entityIndex, const std::string tag );
  EntityTag & getTag( const EntityTypeEnum entityType, const int entityIndex, const std::string tagName);
  void * getTagData( const EntityTypeEnum entityType, const int entityIndex, const std::string tag );

  int setTag( const EntityTypeEnum entityType, const int entityIndex, const EntityTag & newTag );
  int setTagData( const EntityTypeEnum entityType, const int entityIndex, const std::string tagName, 
		  const void *data, const bool copyData=false, const int tagSize=0 );
  
  void maintainTagToEntityMap( bool v );
  bool maintainsTagToEntityMap() const;

  /// iterator for going through the tags in a specific entity
  typedef std::list<EntityTag*>::iterator       entity_tag_iterator;
  typedef std::list<EntityTag*>::const_iterator       const_entity_tag_iterator;
  
  /// iterator for going throught the entities with a specific tag
  typedef std::list<IDTuple>::iterator          tag_entity_iterator;
  typedef std::list<IDTuple>::const_iterator          const_tag_entity_iterator;

  /// return the beginning of the tags for an entity specified with et and index
  inline entity_tag_iterator       entity_tag_begin(EntityTypeEnum et, int index) 
    { return entityTags[IDTuple(et,index)].begin(); }
  inline const_entity_tag_iterator entity_tag_begin(EntityTypeEnum et, int index) const
    { return entityTags[IDTuple(et,index)].begin(); }
  
  /// return the end of the tags for an entity specified with et and index
  inline entity_tag_iterator       entity_tag_end(EntityTypeEnum et, int index)
  { return entityTags[IDTuple(et,index)].end(); }
  inline const_entity_tag_iterator entity_tag_end(EntityTypeEnum et, int index) const
  { return entityTags[IDTuple(et,index)].end(); }
  
  /// return the beginning of the entities with the tag tagName; note this inverse mapping will be built if it does not already exist
  inline tag_entity_iterator      tag_entity_begin(std::string tagName)
  {
    // hey, you asked for it!
    maintainTagToEntityMap( true );
    return tagEntities[tagName].begin(); 
  }
  
  /// return the end of the entities with the tag tagName; note this inverse mapping will be built if it does not already exist
  inline tag_entity_iterator      tag_entity_end(std::string tagName)
  {
    // hey, you asked for it!
    maintainTagToEntityMap( true );
    return tagEntities[tagName].end(); 
  }


  // =====================================================================================================
  // *kkc* new connectivity interface data structures
  //
 public:
  enum EntityInfoMask {
    NullEntityInfo = 0x0,
    HoleInEntityData = 0x1,
    GhostEntity = HoleInEntityData<<1,
    BCEntity = GhostEntity<<1
  };

 protected:
  /// entities maintains the node/vertex ID lists for each entity
  intArray *entities[int(NumberOfEntityTypes)-1]; // -1 because we don't include the "Mesh" entity
  ///  entity identification array that determines if an entity index refers to an internal, ghost or null entity
  intArray *entityMasks[int(NumberOfEntityTypes)-1]; 
  /// lists of holes in the entity arrays
  std::vector<int> entityDataStructureHoles[int(NumberOfEntityTypes)-1];

  /// entity adjacencies, the array is read [From][To]
  intArray *indexLists[int(NumberOfEntityTypes)-1][int(NumberOfEntityTypes)-1]; 
  /// entity adjacency orientations
  char *adjacencyOrientation[int(NumberOfEntityTypes)-1][int(NumberOfEntityTypes)-1];

  /// upwardOffsets allows random access into the upward adjacencies for a given entity
  intArray *upwardOffsets[int(NumberOfEntityTypes)-1][int(NumberOfEntityTypes)-1];

  /// entitySize maintains the current number of entities of a given type
  ArraySimpleFixed<int,int(NumberOfEntityTypes)-1,1,1,1> entitySize;
  /// entityCapacity maintains the current allocated size of the container arrays for a particular entity
  ArraySimpleFixed<int,int(NumberOfEntityTypes)-1,1,1,1> entityCapacity;

public:
  // *kkc* new connectivity interface utility methods
  //
  /// get the max number of vertices in a given entity type
  inline int maxVerticesInEntity(EntityTypeEnum type);

  inline ElementType computeElementType(EntityTypeEnum type, int e);

  /// get the number of vertices in a given entity
  inline int numberOfVertices(EntityTypeEnum, int);

  /// compare the vertices of an entity to a list of vertices, return true if the list specifies the entity
  bool entitiesAreEquivalent(EntityTypeEnum type, int entity, ArraySimple<int> &verticies);

  /// tagPrefix returns the prefix associated with builtin tag conventions
  std::string tagPrefix( EntityTypeEnum type, EntityInfoMask info );

  /// setAsGhost takes an entity and adjusts the data structures to make it a ghost 
  void setAsGhost(EntityTypeEnum type, int entity);

  /// isGhost returns true if a given entity is a ghost (it returns false if no ghost entity info exists)
  inline bool isGhost(EntityTypeEnum type, int entity) const 
  {
    return entityMasks[type] ? (*entityMasks[type])(entity) & GhostEntity : false;
  }

  /// setBC assigns a boundary condition number to a particular entity, if the entity==-1 then the the bc is removed
  void setBC(EntityTypeEnum type, int entity, int bc);

  /// hasBC returns true if the given {type,entity} has a boundary condition specified
  inline bool hasBC(EntityTypeEnum type, int entity)
  { return entityMasks[type] ? (*entityMasks[type])(entity) & BCEntity : false; }

  /// getBC returns the boundary condition number associated with {type,entity} or returns -1 if no bc is specified
  inline long getBC(EntityTypeEnum type, int entity)
  { return hasBC(type,entity) ? long(getTagData(type,entity,tagPrefix(type,BCEntity))) : -1; }

  /// provide an array containing the vertices in for this mapping
  bool specifyVertices(const realArray &verts);

  /// buildEntity directs the construction of the entity arrays stored in entities and entityMasks
  bool buildEntity(EntityTypeEnum type, bool rebuild=false, bool keepDownward=true, bool keepUpward=true);

  /// provide a list of verticies identifying each entity of a particular type
  bool specifyEntity(const EntityTypeEnum type, const intArray &entity);

  /// add a vertex to the mapping, return the index into the "node" array
  int addVertex(real x, real y, real z=0.);

  /// add a "type" entity to the mesh; specify the entity with the vertices in newEntVerts; return the new entity's index
  int addEntity(EntityTypeEnum type, ArraySimple<int> &newEntVerts);

  /// buildConnectivity directs the construction of the connectivity arrays, it returns true if successfull
  /*** buildConnectivity will allocate the space for and build the upward or downward connectivities requested.
   *   If rebuild=true, it will destroy any previously created connectivity and regenerate it.
   */
  bool buildConnectivity(EntityTypeEnum from, EntityTypeEnum to, bool rebuild=false);
  /// specifyConnectivity tells the mapping to use the given connectivity information rather than building it
  /*** specifyConnectivity tells the mapping to use the given connectivity information rather than building it.
   *   it returns false only if the given connectivity makes no sense.
   */
  bool specifyConnectivity(const EntityTypeEnum from, const EntityTypeEnum to, const intArray &index, const char *orientation, 
			   const intArray &offset=Overture::nullIntegerDistributedArray());

  /// return true if the requested adjacency information exists
  inline bool connectivityExists(EntityTypeEnum from, EntityTypeEnum to) const 
  {
    return to==Vertex ? entities[from]!=0 : indexLists[from][to]!=0;
  }

  /// delete specific connectivity information
  void deleteConnectivity(EntityTypeEnum from, EntityTypeEnum to);
  /// delete all connectivity referring to a particular entity type
  void deleteConnectivity(EntityTypeEnum type);

  /// delete ALL thet connectivity information
  void deleteConnectivity();

  /// expand the ghost boundary by a layer
  void expandGhostBoundary( int bc=-1 );

  // =====================================================================================================

protected:

  int dList[numberOfDLProperties]; // display list numbers for different properties
  
  int numberOfNodes, numberOfElements, numberOfFaces, numberOfEdges;
  int maxNumberOfNodesPerElement, maxNumberOfNodesPerFace, maxNumberOfFacesPerElement;
  bool numberOfNodesPerElementIsConstant; // *wdh* 020517
  
  int numberOfInternalElements, numberOfInternalFaces;
  int numberOfBoundaryElements, numberOfBoundaryFaces;
  bool preferTriangles;  // prefer building triangles or tets.
  real elementDensityTolerance; // for choosing the number of elements on a Mapping.
  real stitchingTolerance;      // relative tol for stitching surfaces together.
  real absoluteStitchingTolerance; // absolute tol for stitching surfaces together.
  int debugs;                   // debug for stitching

  GeometricADT3dInt *search;  // used to search for triangles nearby a point (Alternating Digital Tree)
  

  realArray node;
  intArray element, face, bdyFace, faceElements, edge;
  intArray bdyFaceTags;
  intArray *elementFaces;  // optionally holds elementFaces.

  bool includeGhostElements;
  intArray *elementMask, *faceMask, *edgeMask, *nodeMask; // optional info when ghost elements are included.
  intArray *ghostElements;  // optional info about ghost elements

  enum TimingsEnum
  {
    totalTime=0,
    timeForBuildingSubSurfaces,
    timeForConnectivity,
    timeForProjectGlobalSearch,
    timeForProjectLocalSearch,
    timeForStitch,
    timeForInsideOrOutside,
    numberOfTimings
  };
  real timing[numberOfTimings];

  int buildConnectivityLists();
  void initMapping();

  // FEZ means Finite Element Zoo
  // connectivity templates for a finite element zoo
  // the connectivity should be abstracted away in some nice way...
  // for now, keep it simple, allocate some extra arrays and just do the bookkeeping...

  // parameters
  int numberOfElementTypes;

  // zone based templates
  IntegerArray numberOfFacesThisElementType;
  IntegerArray numberOfNodesThisElementType;
  IntegerArray numberOfNodesThisElementFaceType;
  IntegerArray elementMasterTemplate;

  // auxillary connectivity arrays
  intArray elementType;
  intArray faceZ1Offset;

  // element tag array ( for denoting regions, etc.)
  intArray tags;

  // auxillary connectivity methods (used when building the connectivity)
  // most of the following methods abstract out the ugliness of the FEZ connectivity
  int FEZComputeElementTypes();
  inline IntegerArray getElementFaceNodes(int element_, int faceOffset) const;

  int FEZInitializeConnectivity();  // initialize the Finite Element Zoo Connectivity

  int addNodeToInterface( int s1, int & i1, int & j1, int & e1m, int & e1p, IntegerArray & connectionInfo1,
			  int s2, int & i2, int & j2, int & e2m, int & e2p, IntegerArray & connectionInfo2,
			  const intArray & elementface2, 
                          intArray * bNodep, IntegerArray & faceOffset, IntegerArray & elementOffset,
			  int maxNumberOfElements, int maxNumberOfFaces );

  int buildSearchTree();

  int computeConnection(int s, int s2, 
			intArray *bNodep,
			IntegerArray & numberOfBoundaryNodes,
			UnstructuredMapping *boundaryp,
			real epsx,
			IntegerArray & connectionInfo );
  
  bool isDuplicateNode(int i, int n, int e, int s, int s2, real & r0, real & r1,
		       realArray & x,
		       real epsDup,
		       intArray & bNode,
		       intArray & nodeInfo,
		       int & localEdge,           
		       real & dist0, real & dist1, real & dist2, int debugFlag);

  void replaceNode( int n, int n0, intArray & nodeInfo, intArray & ef );

  bool validStitch( int n, realArray & x0, realArray & x, intArray & nodeInfo, real tol, int debug);
  
  friend class UnstructuredMappingIterator;
  friend class UnstructuredMappingAdjacencyIterator;

  // kkc General tagging support added 0802
  // the user should *never* see these levely template types.  we only use them internally
  bool maintainsTagEntities;
  mutable std::map<IDTuple, std::list<EntityTag*> > entityTags; // always built when tags are used
  mutable std::map<std::string, std::list<IDTuple>, std::less<std::string> > tagEntities; // optionally built if requested.

  bool dumpTagsToHDF;

 private:

  aString className, gridColour;

  // the map function should not be called
  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((UnstructuredMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((UnstructuredMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new UnstructuredMapping(*this, ct); }

};

/// \brief This class is used to iterate over the valid entities (excluding ghost entities).
class UnstructuredMappingIterator
{
public:
 UnstructuredMappingIterator();
 UnstructuredMappingIterator(const UnstructuredMapping & uns, UnstructuredMapping::EntityTypeEnum entityType_,
                 int position, bool skipGhostEntites_ = false );
 ~UnstructuredMappingIterator(){}

 inline bool setLocation( int l )
  { 
    if ( l<numberOfEntities)
      {
	e = l;
	return true;
      }
    return false;
  }
    
  void operator++(int){ 
    e++;
    while ( e<numberOfEntities && (entityMask[e]&skipMask) ) e++;
    //    if ( includeGhostElements )
    //      { e++; while ( e<numberOfEntities && (entityMask[entityArray[offset+e*stride]] & UnstructuredMapping::HoleInEntityData) ) e++; }
    //    else
    //      { e++; while( e<numberOfEntities && entityMask[entityArray[offset + e*stride]] ) e++; }

    //    if( !includeGhostElements)
    //  { e++; }
    //else
    // { e++; while( entityMask[e]<=0 && e<numberOfEntities ){ e++; } }  
  }

  int operator *() const {return e;}   //

  bool operator==(const UnstructuredMappingIterator & iter) const { return iter.e==e; }  //
  bool operator!=(const UnstructuredMappingIterator & iter) const { return iter.e!=e; }  //

  void operator=(const UnstructuredMappingIterator & iter);
  void operator=(const UnstructuredMappingAdjacencyIterator & iter);
  inline UnstructuredMapping::EntityTypeEnum getType() const { return entityType; }
  
  bool isGhost() const { return entityMask[**this]&UnstructuredMapping::GhostEntity; }
  bool isBC() const { return entityMask[**this]&UnstructuredMapping::BCEntity; }

  friend class UnstructuredMappingAdjacencyIterator;
  friend class UnstructuredMapping;
private:
  UnstructuredMapping::EntityTypeEnum entityType;
  int numberOfEntities;
  int *entityMask;
  //bool includeGhostElements;
  bool skipGhostEntities;
  int skipMask;
  int e;
};

// ==================================================================================================
/// \brief class used to iterate over unstructured grids.
// ==================================================================================================
class UnstructuredMappingAdjacencyIterator
{
  friend class UnstructuredMappingIterator;
public:
  UnstructuredMappingAdjacencyIterator();
  UnstructuredMappingAdjacencyIterator(const UnstructuredMapping & uns, UnstructuredMapping::EntityTypeEnum from, int adjTo,
				       UnstructuredMapping::EntityTypeEnum to,
				       int position, bool skipGhostEntities_ = false );
  ~UnstructuredMappingAdjacencyIterator(){}

  inline void operator++(int)
  {
    e++;
    while ( e<numberOfEntities && (entityMask[entityArray[offset + e*stride]]&skipMask) ) e++;    //    if ( !skipGhostEntities )
    //      { e++; while ( e<numberOfEntities && (entityMask[entityArray[offset+e*stride]] & UnstructuredMapping::HoleInEntityData) ) e++; }
    //    else
    //      { e++; while( e<numberOfEntities && entityMask[entityArray[offset + e*stride]] ) e++; }
  }

  inline int operator*() const { return e<numberOfEntities ? abs(entityArray[offset + e*stride]) : -1; }

  inline bool operator==(const UnstructuredMappingAdjacencyIterator &i) const 
  { return entityArray ? ( (i.adjEntityType==adjEntityType) && ( (**this)==(*i) ) ) : entityArray==i.entityArray;  }
    //{ return entityArray ? (i.e==e) && i.adjEntityType==adjEntityType && offset==i.offset: entityArray==i.entityArray;  }
/*   { return (entityArray && i.entityArray) ?  */
/*       ( (i.adjEntityType==adjEntityType) && abs(entityArray[offset + e*stride])==abs(i.entityArray[i.offset + i.e*i.stride]) ) : */
/*     !(entityArray || i.entityArray); } */
        
  //{ return entityArray ? (i.e==e) && i.adjEntityType==adjEntityType : entityArray==i.entityArray;  } 
  
  inline bool operator==(const UnstructuredMappingIterator &i) const 
  { return entityArray ? (i.e==abs(entityArray[offset + e*stride])) && i.entityType==adjEntityType : false; }

  inline bool operator!=(const UnstructuredMappingAdjacencyIterator &i) const 
  { return ! (*this==i);}
    //  { return entityArray ? (i.adjEntityType==adjEntityType ? i.e!=e || offset!=i.offset : true) : entityArray!=i.entityArray; }
  //{ return entityArray ? (i.adjEntityType==adjEntityType ? i.e!=e : true) : entityArray!=i.entityArray; }

  inline bool operator!=(const UnstructuredMappingIterator &i) const 
  { return !(*this==i); }//entityArray ? (i.entityType==adjEntityType ? i.e!=abs(entityArray[offset + e*stride]) : true) : true; }

  UnstructuredMappingAdjacencyIterator & operator=(const UnstructuredMappingAdjacencyIterator &i);

  inline int orientation() const { return orientationArray ? (orientationArray[offset + e*stride] ? 1 : -1) : 1; }

  inline UnstructuredMapping::EntityTypeEnum getType() const { return adjEntityType; }
  bool isGhost() const { return entityMask[**this]&UnstructuredMapping::GhostEntity; }
  bool isBC() const { return entityMask[**this]&UnstructuredMapping::BCEntity; }

  inline int nAdjacent() const { return numberOfEntities; }

  friend class UnstructuredMapping;

private:
  UnstructuredMapping::EntityTypeEnum entityType;
  UnstructuredMapping::EntityTypeEnum adjEntityType;

  int *entityArray;
  char *orientationArray;
  int numberOfEntities, stride, offset;
  int e;
  int *entityMask;
  bool skipGhostEntities;
  int skipMask;
};



// most of the following inlined methods abstract out the ugliness of the FEZ connectivity
//\begin{>UnstructuredMappingInclude.tex}{\subsection{getElementFaceNodes}}
inline IntegerArray UnstructuredMapping::
getElementFaceNodes(int element_, int faceOffset) const
//===========================================================================
// /Description: get a list of the nodes on a particular face of an element
// /element\_ (input): the element in question
// /faceOffset (input): the face in the element whose nodes we want
// /Returns : IntegerArray of global node indices
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  assert(element_>-1 && element_<numberOfElements);
  int etype = elementType(element_);

  IntegerArray emslice;
  Index Iface(0,numberOfNodesThisElementFaceType(etype, faceOffset));
  IntegerArray retArray;

#if 0
  // this works but I am not sure it is any faster...
  emslice = elementMasterTemplate(etype, faceOffset, Iface);
  emslice.reshape(Iface);
  IntegerArray ia(Iface);
  ia = element_;
  retArray = element(ia, emslice);
  retArray.reshape(Iface);
#else
  retArray.resize(Iface);
  for (int i=0; i<Iface.getLength(); i++) 
    retArray(i) = element(element_, elementMasterTemplate(etype, faceOffset, i));
#endif
 
  return retArray;
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{getNumberOfFacesThisElement}}
inline int UnstructuredMapping::
getNumberOfFacesThisElement(int element_) const
//===========================================================================
// /Description: get the number of faces in a particular element
// /element\_ (input): the element in question
// /Returns : int containing the number of faces in element\_
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  assert(element_>-1);
  if( numberOfNodesPerElementIsConstant )
    return maxNumberOfNodesPerElement;
  else    
    return numberOfFacesThisElementType(elementType(element_));
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{getNumberOfNodesThisElement}}
inline int UnstructuredMapping::
getNumberOfNodesThisElement(int element_)  const
//===========================================================================
// /Description: get the number of nodes in a particular element
// /element\_ (input): the element in question
// /Returns : int containing the number of nodes in element\_
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  assert(element_>-1);
  if( numberOfNodesPerElementIsConstant || maxNumberOfNodesPerElement==3 )
    return maxNumberOfNodesPerElement;
  else
  return numberOfNodesThisElementType(elementType(element_));
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{getNumberOfNodesThisFace}}
inline int UnstructuredMapping::
getNumberOfNodesThisFace(int face_)  const
//===========================================================================
// /Description: get the number of nodes in a particular face
// /face\_ (input): the face in question
// /Returns : int containing the number of nodes in face\_
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  assert(face_>-1);
  if( domainDimension==2 || maxNumberOfNodesPerElement==3 )
    return 2;
  else
    return numberOfNodesThisElementFaceType(elementType(faceElements(face_,0)), faceZ1Offset(face_));
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{getElementType}}
inline UnstructuredMapping::ElementType UnstructuredMapping::
getElementType(int e) const
//===========================================================================
// /Description: get the type of a particular element
// /element\_ (input): the element in question
// /Returns : UnstructuredMapping::ElementType
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  if( maxNumberOfNodesPerElement==3 )
    return triangle;
  else
    return ElementType(elementType(e));
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{elementGlobalVertex}}
inline int UnstructuredMapping::
elementGlobalVertex(int elem, int vertex) const
//===========================================================================
// /Description: get the global vertex index for a node in an element
// /elem (input): the element in question
// /vertex (input): the node in the element whose global index is required
// /Returns : int, the global index of node vertex in element elem
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  return element(elem, vertex);
}

//\begin{>>UnstructuredMappingInclude.tex}{\subsection{faceGlobalVertex}}
inline int UnstructuredMapping::
faceGlobalVertex(int f, int v) const 
//===========================================================================
// /Description: get the global vertex index for a node in a face
// /f (input): the face in question
// /v (input): the node in the face whose global index is required
// /Returns : int, the global index of node v in face f
// /Throws : nothing
//\end{UnstructuredMappingInclude.tex}
//===========================================================================
{
  return element(face(f,0), elementMasterTemplate(elementType(face(f,0)), faceZ1Offset(f), v));
}

inline int UnstructuredMapping::
getBoundaryFace(int bdyface_) const
{
  return bdyFace(bdyface_);
}

inline int UnstructuredMapping::
getBoundaryFaceTag(int bdyface_) const
{
  return bdyFaceTags(bdyface_);
}

inline UnstructuredMappingIterator UnstructuredMapping::
begin(EntityTypeEnum entityType, bool skipGhostEntities /* =false */ ) const
{
  if ( !entities[entityType] ) ((UnstructuredMapping *)this)->buildEntity(entityType);
  return UnstructuredMappingIterator(*this,entityType,0,skipGhostEntities);
}

inline UnstructuredMappingIterator UnstructuredMapping:: 
end(EntityTypeEnum entityType, bool skipGhostEntities /* =false */) const
{
  if ( !entities[entityType] ) ((UnstructuredMapping *)this)->buildEntity(entityType);
  return UnstructuredMappingIterator(*this,entityType,1,skipGhostEntities); 
} 

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_begin(EntityTypeEnum fromT, int fromE, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(fromT,to) ) ((UnstructuredMapping *)this)->buildConnectivity(fromT,to);
  return UnstructuredMappingAdjacencyIterator(*this, fromT, fromE, to, 0, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_end(EntityTypeEnum fromT, int fromE, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(fromT,to) ) ((UnstructuredMapping *)this)->buildConnectivity(fromT,to);
  return UnstructuredMappingAdjacencyIterator(*this, fromT, fromE, to, 1, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_begin(UnstructuredMappingIterator from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.entityType,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.entityType,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.entityType, *from, to, 0, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_end(UnstructuredMappingIterator from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.entityType,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.entityType,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.entityType, *from, to, 1, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_begin(UnstructuredMappingAdjacencyIterator from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.adjEntityType,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.adjEntityType,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.adjEntityType, *from, to, 0, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_end(UnstructuredMappingAdjacencyIterator from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.adjEntityType,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.adjEntityType,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.adjEntityType, *from, to, 1, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_begin(IDTuple from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.et,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.et,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.et, from.e, to, 0, skipGhostEntities);
}

inline UnstructuredMappingAdjacencyIterator 
UnstructuredMapping::
adjacency_end(IDTuple from, EntityTypeEnum to, bool skipGhostEntities) const
{
  if ( !connectivityExists(from.et,to) ) ((UnstructuredMapping *)this)->buildConnectivity(from.et,to);
  return UnstructuredMappingAdjacencyIterator(*this, from.et, from.e, to, 1, skipGhostEntities);
}

/// get the max number of vertices in a given entity type
inline int 
UnstructuredMapping::
maxVerticesInEntity(EntityTypeEnum type)
{
  switch(type) {
  case Region:
    return 8;
  case Face:
    return 4;
  case Edge:
    return 2;
  case Vertex:
    return 1;
  default:
    return 0;
  }
}

/// get the number of vertices in a given entity
inline int 
UnstructuredMapping::
numberOfVertices(EntityTypeEnum type, int entity)
{
  const intArray & earray = getEntities(type);
  switch(type) {
  case Region:
    {
      for ( int i=7; i>=3; i-- )
	if ( earray(entity,i)!=-1 )
	  return i+1;

      break;
    }
  case Face:
    {
      for ( int i=3; i>1; i-- )
	if ( earray(entity,i)!=-1 )
	  return i+1;

      break;
    }
  case Edge:
    return 2;
  case Vertex:
    return 1;
  default:
    return 0;
  }
  return 0;
}

inline UnstructuredMapping::ElementType 
UnstructuredMapping::
computeElementType(EntityTypeEnum type, int e)
{ 
  int nv = numberOfVertices(type,e);
  if ( type==Face )
    return nv==3 ? triangle : ( nv==4 ? quadrilateral : other );
  else
    return nv==8 ? hexahedron : ( nv==7 ? septahedron : ( nv==6 ? triPrism : ( nv==5 ? pyramid : ( nv==4 ? tetrahedron : other))));
}


#endif  



