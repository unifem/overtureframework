//#define OV_DEBUG

#include "ULink.h"

int unstructuredLink( UnstructuredMapping &umap,
		      ArraySimple< UnstructuredMappingAdjacencyIterator > &links,
		      const UnstructuredMappingIterator &linkFrom,
		      int nHops,
		      int minRefs, int pathDim /*=-1*/, bool useGhost /*=true*/ )
{

  if ( !links.size() )
    links.resize(5*nHops*umap.getDomainDimension()); // guess an initial number of adjacencies

  bool skipGhost = !useGhost;

  int nLinks = 0;
  UnstructuredMapping::EntityTypeEnum linkPathT = pathDim<0 || pathDim==int(linkFrom.getType()) ? 
    (linkFrom.getType()==UnstructuredMapping::Vertex ?
     UnstructuredMapping::Edge : UnstructuredMapping::EntityTypeEnum( int(linkFrom.getType())-1 ) ) :
    (UnstructuredMapping::EntityTypeEnum( pathDim ));

  UnstructuredMapping::EntityTypeEnum linkT = linkFrom.getType();

  UnstructuredMappingAdjacencyIterator path, link_cand, path_end, link_end;

  path_end = umap.adjacency_end(linkFrom, linkPathT);

  for ( path=umap.adjacency_begin(linkFrom, linkPathT);
	path!=path_end; path++ )
    {

      link_end = umap.adjacency_end(path, linkT, skipGhost);
      link_cand=umap.adjacency_begin(path, linkT, skipGhost);
      for ( ;//link_cand=umap.adjacency_begin(path, linkT, skipGhost); 
	    link_cand!=link_end; link_cand++ )
	{
	  if ( link_cand!=linkFrom )
	    {
	      bool foundDup=false;
	      for ( int l=0; l<nLinks && !foundDup; l++ )
		foundDup = (link_cand==links[l]);

	      if ( !foundDup )
		{
		  if ( links.size(0)<=nLinks )
		    links.resize(links.size(0)+10);

		  links[nLinks] = link_cand;
		  nLinks++;
		}
	    }
	}
      
    }

  int nh=1;
  int lastHop_s=0, lastHop_e=nLinks;

  while ( nHops>nh || minRefs>nLinks )
    {
      for ( int l=lastHop_s; l<lastHop_e && (nh<nHops || minRefs>nLinks); l++ )
	{
	  path_end = umap.adjacency_end(links(l), linkPathT);
	  for ( path=umap.adjacency_begin(links(l), linkPathT);
		path!=path_end /*&& minRefs>nLinks*/; path++ )
	    {
	      link_end = umap.adjacency_end(path, linkT,skipGhost);
	      for ( link_cand=umap.adjacency_begin(path, linkT,skipGhost); 
		    link_cand!=link_end /*&& minRefs>nLinks*/; link_cand++ )
		{
		  if ( link_cand!=linkFrom && link_cand!=links(l) )
		    {
		      bool foundDup=false;
		      for ( int lc=0; lc<nLinks && !foundDup; lc++ )
			{
			  foundDup = (*link_cand)==(*links[lc]);
			}

		      if ( !foundDup )
			{
			  if ( links.size(0)<=nLinks )
			    links.resize(links.size(0)+10);
			  
			  links[nLinks] = link_cand;
			  nLinks++;
			}
		    }
		}
	    }
	}
      lastHop_s=lastHop_e;
      lastHop_e=nLinks;
      
      nh++;
    }
  
  
  links.resize(nLinks);

  return nh;
}
