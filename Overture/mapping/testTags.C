#include "SquareMapping.h"
#include "UnstructuredMapping.h"
#include "GenericDataBase.h"
#include "HDF_DataBase.h"

struct SomeData {
  int i;
  int j;

  SomeData(int i_=10, int j_=20) : i(i_),j(j_) { }
  bool operator== ( const SomeData &sd ) const { return i==sd.i && j==sd.j; }
};

int main(int argc, char *argv[])
{
  SquareMapping square;
  UnstructuredMapping *umapp = new UnstructuredMapping;
  UnstructuredMapping & umap = *umapp;
  umap.buildFromARegularMapping(square);

  SomeData someDataOnStack;
  
  // now, add a bunch of tags to all the vertices that all refer to
  //      the same data
  UnstructuredMappingIterator nodeIt = umap.begin(UnstructuredMapping::Vertex), it;
  
  for ( ; nodeIt!=umap.end(UnstructuredMapping::Vertex); nodeIt++ )
    umap.addTag(UnstructuredMapping::Vertex, *nodeIt, "some data", (void *)&someDataOnStack);

  // now make sure we have the tags and they point to the correct thing!
  for ( nodeIt=umap.begin(UnstructuredMapping::Vertex); 
	nodeIt!=umap.end(UnstructuredMapping::Vertex); nodeIt++)
    {
      if ( !umap.hasTag(UnstructuredMapping::Vertex, *nodeIt, "some data") )
	abort();
      if ( !(umap.getTagData(UnstructuredMapping::Vertex, *nodeIt,"some data")==&someDataOnStack) )
	abort();

      // make sure we can delete the tag w/o hurting the stack data
      
      if ( umap.deleteTag(UnstructuredMapping::Vertex, *nodeIt,"some data")!=0 )
	abort();

      // there should be no more tags!
      if ( umap.entity_tag_begin(UnstructuredMapping::Vertex,*nodeIt)!=umap.entity_tag_end(UnstructuredMapping::Vertex,*nodeIt) )
	abort();
    }

  // now give each edge its own data
  for ( it=umap.begin(UnstructuredMapping::Edge); 
	it!=umap.end(UnstructuredMapping::Edge); it++)
    {
      SomeData sd(UnstructuredMapping::Edge, *it);
      int sz = sizeof(SomeData);
      umap.addTag(UnstructuredMapping::Edge, *it, "owned data", (void *)&sd, true, sz); 
    }

  // make sure it worked!
  for ( it=umap.begin(UnstructuredMapping::Edge); 
	it!=umap.end(UnstructuredMapping::Edge); it++)
    {
      SomeData  *sp = ((SomeData *) umap.getTagData(UnstructuredMapping::Edge, *it, "owned data"));
      if ( !sp ) abort();

      SomeData & sd = *sp;
      if ( sd.i!=UnstructuredMapping::Edge || sd.j!=*it )
	abort();
    }

  // write it to an hdf file
  HDF_DataBase db;
  db.mount("tagtest.hdf","I");
  umap.put(db,"umap");
  db.unmount();

  HDF_DataBase db2;
  db2.mount("tagtest.hdf","R");
  UnstructuredMapping umap2;
  umap.get(db2,"umap");
  db2.unmount();

  UnstructuredMappingIterator it2;
  for ( it2=umap2.begin(UnstructuredMapping::Edge); it2!=umap2.end(UnstructuredMapping::Edge); it2++ )
    {
      SomeData & sd = *((SomeData *) umap2.getTagData(UnstructuredMapping::Edge, *it2, "owned data"));
      if ( sd.i!=UnstructuredMapping::Edge || sd.j!=*it2 )
	abort();
    }

  UnstructuredMapping::tag_entity_iterator tagit;
  int n=0;
  for ( tagit=umap2.tag_entity_begin("owned data"); 
	tagit!=umap2.tag_entity_end("owned data"); tagit++, n++ ) ;
  
  if ( n!=umap2.getNumberOfEdges() )
    abort();

  // explicitly test the destructor
  delete umapp;

  cout<<"if it don't abort it ain't broke"<<endl;

  return 0;
}
