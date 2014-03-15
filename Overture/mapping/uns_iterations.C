#define TESTDOWN
#define TESTUP
#define TESTGHOST

#include <iostream>
#include <string>

#include "aString.H"
#include "OvertureDefine.h"
#include "OvertureTypes.h"
#include "OvertureInit.h"
#include "UnstructuredMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

using namespace std;

#define SQUARE

int main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  bool useSquare=true;
  bool useBox = false;
  string fileName="";
  if ( argc==2 )
    {
      useSquare = string(argv[1])=="square";
      useBox = string(argv[1])=="box";
      fileName = argv[1];
    }


  UnstructuredMapping umap;
  umap.setPreferTriangles(true);

  if ( useSquare )
    {
      SquareMapping omap(0,1,0,1);
      omap.setGridDimensions(0,3);
      omap.setGridDimensions(1,3);
      umap.buildFromAMapping(omap);
    }
  else if ( useBox )
    {
      BoxMapping omap(0,1,0,1,0,1);
      omap.setGridDimensions(0,3);
      omap.setGridDimensions(1,3);
      omap.setGridDimensions(2,2);
      umap.buildFromAMapping(omap);
    }
  else
    {
      umap.get(fileName);
    }

  
  if ( umap.size(UnstructuredMapping::Vertex)==0 ) return false;

  UnstructuredMappingIterator e_iter;

#if (defined TESTUP || defined TESTDOWN)
  UnstructuredMappingAdjacencyIterator a_iter;
#endif

  // loop through all the entities of a particular type
  for ( UnstructuredMapping::EntityTypeEnum t=UnstructuredMapping::Vertex;
	t<UnstructuredMapping::Mesh; 
	((int)t)++ )
    for ( e_iter=umap.begin(t); e_iter!=umap.end(t); e_iter++ )
      cout<<UnstructuredMapping::EntityTypeStrings[int(t)]<<" entity with index "<<*e_iter<<endl;

  umap.getNodes().display("Nodes");
  umap.getEntities(UnstructuredMapping::Edge).display("Edges");
  umap.getEntities(UnstructuredMapping::Face).display("Faces");
  if ( umap.getDomainDimension()==3 ) umap.getEntities(UnstructuredMapping::Region).display("Regions");
#ifdef TESTDOWN

  cout<<" =================== TESTING DOWNWARD ADJACENCIES ====================="<<endl;
  // loop through all downward adjacencies for each entity
  for ( UnstructuredMapping::EntityTypeEnum t=UnstructuredMapping::Vertex;
	t<UnstructuredMapping::Mesh; 
	((int) t)++ )
    for ( UnstructuredMapping::EntityTypeEnum td=UnstructuredMapping::EntityTypeEnum(((int)t)-1);
	td>=UnstructuredMapping::Vertex; 
	((int)td)-- )
      for ( e_iter=umap.begin(t); e_iter!=umap.end(t); e_iter++ )
	{
	  cout<<UnstructuredMapping::EntityTypeStrings[int(td)]<<" Entities adjacent to "<<
	    UnstructuredMapping::EntityTypeStrings[int(t)]<<" "<<*e_iter<<" : ";

	  for ( a_iter=umap.adjacency_begin(e_iter, td); 
		a_iter!=umap.adjacency_end(e_iter, td);
		a_iter++ )
	    cout<< (a_iter.orientation()>0 ? "+":"-") <<(*a_iter)<<" ";

	  cout<<endl;
	}	  
#endif

#ifdef TESTUP

  cout<<" ==================== TESTING UPWARD ADJACENCIES ======================"<<endl;

  // loop through all upward adjacencies for each entity
  for ( UnstructuredMapping::EntityTypeEnum t=UnstructuredMapping::Vertex;
	t<UnstructuredMapping::Mesh; 
	((int)t)++ )
    for ( UnstructuredMapping::EntityTypeEnum td=UnstructuredMapping::EntityTypeEnum(((int)t)+1);
	td<UnstructuredMapping::Mesh; 
	((int) td)++ )
      for ( e_iter=umap.begin(t); e_iter!=umap.end(t) && umap.size(td); e_iter++ )
	{
	  cout<<UnstructuredMapping::EntityTypeStrings[int(td)]<<" Entities adjacent to "<<
	    UnstructuredMapping::EntityTypeStrings[int(t)]<<" "<<*e_iter<<" : ";

	  for ( a_iter=umap.adjacency_begin(e_iter, td); 
		a_iter!=umap.adjacency_end(e_iter, td);
		a_iter++ )
	    cout<< (a_iter.orientation()>0 ? "+":"-") <<(*a_iter)<<" ";

	  cout<<endl;
	}
	  
#endif

#ifdef TESTGHOST
  cout<<" ==================== TESTING GHOST ENTITIES ==========================="<<endl;

  // loop through all the "ghost" or boundary entities (do this with tags?)
  for ( UnstructuredMapping::EntityTypeEnum t=UnstructuredMapping::Vertex;
	t<UnstructuredMapping::Mesh; 
	((int)t)++ )
    {
      string tagname = string("ghost ")+UnstructuredMapping::EntityTypeStrings[int(t)].c_str();
      cout<<"looping through : "<<tagname<<endl;
      for ( UnstructuredMapping::tag_entity_iterator iter=umap.tag_entity_begin(tagname);
	    iter!=umap.tag_entity_end(tagname);
	    iter++ )
	cout<<tagname<<" "<<iter->e<<endl;
    }
#endif

  assert(verifyUnstructuredConnectivity(umap, true));

  Overture::finish();

  return 0;
}
