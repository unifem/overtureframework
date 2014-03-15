#include "UnstructuredMapping.h"

#include "entityConnectivityBuilder.h"
extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

using namespace std;

int main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  string fileName="";
  if ( argc==2 )
    fileName = argv[1];
  else
    {
      cout<<"usage : "<<endl<<argv[0]<<" filename"<<endl;
      return 1;
    }

  UnstructuredMapping umap;
  umap.get(fileName);
  
  if ( umap.size(UnstructuredMapping::Vertex)==0 ) return 1;

  bool status = verifyUnstructuredConnectivity( umap, true );

  Overture::finish();

  return status ? 0 : 1;
}

