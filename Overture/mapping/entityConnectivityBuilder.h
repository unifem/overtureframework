
#ifndef OV__ENTITY_CONNECTIVITY_BUILDER_HH__
#define OV__ENTITY_CONNECTIVITY_BUILDER_HH__

class intArray;

int constructEdgeEntityFromEntity(intArray &edges, intArray &downward, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient, intArray &regions, 
				  int nReg,
				  int maxVerts, int maxVertIDX, int dDim);

int constructFaceEntityFromRegion(intArray &faces, intArray &downward, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient, intArray &regions, 
				  int nReg,
				  int maxVertsPerFace, int maxVertIDX);

int constructRegion2EdgeFromFaces(intArray &region2Edge, char *&dOrient,
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
				  intArray &face2Edge, char *faceEdgeOrient, intArray &region2Face, 
				  intArray &faces, intArray &regions, 
				  int nReg, int maxVertIDX);

int constructFace2EdgeFromRegions(intArray &face2Edge, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
				  intArray &region2Edge, char *regionEdgeOrient, intArray &region2Face, 
				  intArray &faces, intArray &edges, intArray &regions, 
				  int nReg, int maxFaces, int maxVertIDX);

int constructUpwardAdjacenciesFromDownward(intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
					   const intArray &downward, const char *dOrient, int maxIDX);


#endif
