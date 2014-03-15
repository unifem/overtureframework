// #define BOUNDS_CHECK
#define OV_DEBUG

#include "uns_templates.h"
#include "UnstructuredMapping.h"
#include "Geom.h"

bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose )
{
  bool isOk = true;

  int numberOfEntities[] = { 0, 0, 0, 0 };
  int numberOfElements[] = { 0, 0, 0, 0, 0, 0, 0 };
  int numberOfGhostEntities[] = { 0, 0, 0, 0 };

  int maxUpAdjacency = 10000; // consider it an error if the maximum upward adjacency contains this many items

  int nOrphanEntities[] = { 0,0,0,0 };
  int nOrientationErrors[] = { 0,0,0,0 };
  int nErr = 0;
  int nWarn = 0;
  int nInvertedFaces=0;

  int nZeroLengthEdges = 0;
  real maxLen = 0;
  real minLen = REAL_MAX;
  real avgLen = 0;
  
  real maxVol=0;
  real maxArea=0;
  real minVol=REAL_MAX;
  real minArea=REAL_MAX;
  real avgVol=0;
  real avgArea=0;

  ArraySimple<int> vertexUseCount(umap.size(UnstructuredMapping::Vertex), int(UnstructuredMapping::NumberOfEntityTypes)-1);
  vertexUseCount = 0;

  // 1. make sure the available downward and upward adjacencies agree 
  //        a. make sure entities are contained in the downward adjacencies of higher dimensional entities which contain them
  //        b. make sure entities are contained in the upward adjacencies of lower dimensional constituent entites

  // 2. check orientations:
  //                               a. check the length of each edge to make sure it is nonzero (no duplicate vertices)
  //
  //        if rangeDimension==2,  b. check the areas of each "side" (edge-face combination) to look for inverted faces
  //                               c. compare the results from Face->Vertex and Face->Edge downward iterators
  //                               d. compare the results from Edge->Face upward iterator
  //
  //        if rangeDimension==3,  b. check the volumes of each "side" (edge-face-region combination) to look for inversions
  //                               c. compare the results of Region->Vertex, Region->Face and Region->Edge downward iterators
  //                               d. compare the results from {Edge->Face, Face->Region} and {Face->Region} upward iterators



  // 1. make sure the available downward and upward adjacencies agree 
  UnstructuredMappingIterator e_iter;
  UnstructuredMappingAdjacencyIterator u_iter, d_iter;

  int i_type=0;
  for ( UnstructuredMapping::EntityTypeEnum type = UnstructuredMapping::Vertex; type<=UnstructuredMapping::Region; type=UnstructuredMapping::EntityTypeEnum(++i_type) )
    {
      for ( e_iter=umap.begin(type); e_iter!=umap.end(type); e_iter++ )
	{
	  // 1.a make sure entities are contained in the downward adjacencies of higher dimensional entities which contain them
	  int i_tup=UnstructuredMapping::EntityTypeEnum(type+1);
	  for ( UnstructuredMapping::EntityTypeEnum t_up=UnstructuredMapping::EntityTypeEnum(type+1); 
		t_up<=UnstructuredMapping::Region; t_up=UnstructuredMapping::EntityTypeEnum(++i_tup) )
	    {
	      for ( u_iter=umap.adjacency_begin(e_iter, t_up); u_iter!=umap.adjacency_end(e_iter, t_up); u_iter++ )
		{
		  bool found = false;
		  for ( d_iter=umap.adjacency_begin(u_iter, type); !found && d_iter!=umap.adjacency_end(u_iter,type); d_iter++ )
		    found = *d_iter==*e_iter;
		  
		  if ( !found )
		    {
		      nErr++;
		      nOrphanEntities[type]++;
		      if ( verbose )
			cout<<"CONNECTIVITY ERROR : could not find "<<UnstructuredMapping::EntityTypeStrings[type]<<" "<<*e_iter<<
			  " in downward iteration of "<<UnstructuredMapping::EntityTypeStrings[t_up]<<" "<<*u_iter<<endl;
		    }
		}
	    }

	  // 1.b make sure entities are contained in the upward adjacencies of lower dimensional constituent entites (repeat of above)
	  int i_tdown = UnstructuredMapping::EntityTypeEnum(int(type)-1);
	  for ( UnstructuredMapping::EntityTypeEnum t_down=UnstructuredMapping::EntityTypeEnum(int(type)-1); 
		t_down>=UnstructuredMapping::Vertex; t_down=UnstructuredMapping::EntityTypeEnum(--i_tdown) )
	    {
	      for ( d_iter=umap.adjacency_begin(e_iter, t_down); d_iter!=umap.adjacency_end(e_iter,t_down); d_iter++ )
		{
		  bool found = false;
		  for ( u_iter=umap.adjacency_begin(d_iter, type); !found && u_iter!=umap.adjacency_end(d_iter, type); u_iter++ )
		    found = *u_iter==*e_iter;

		  if ( !found )
		    {
		      nErr++;
		      nOrphanEntities[type]++;
		      if ( verbose )
			cout<<"CONNECTIVITY ERROR : could not find "<<UnstructuredMapping::EntityTypeStrings[type]<<" "<<*e_iter<<
			  " in upward iteration of "<<UnstructuredMapping::EntityTypeStrings[t_down]<<" "<<*d_iter<<endl;
		    }
		}
	    }
	
	  numberOfEntities[type]++;
	  if ( e_iter.isGhost() ) numberOfGhostEntities[type]++;
	  if ( type>int(UnstructuredMapping::Edge) )
	    numberOfElements[ umap.computeElementType(type, *e_iter) ]++;
	}
    }


  // 2. check orientations:
  //                               a. check the length of each edge to make sure it is nonzero (no duplicate vertices)
  const realArray &vertices = umap.getNodes(); //XXX bad name for this interface!
  const intArray &edges = umap.getEntities(UnstructuredMapping::Edge);
  for ( e_iter = umap.begin(UnstructuredMapping::Edge); e_iter!=umap.end(UnstructuredMapping::Edge); e_iter++ )
    {
      real len = 0;
      for ( int a=0; a<umap.getRangeDimension(); a++ )
	len += (vertices(edges(*e_iter,1), a) - vertices(edges(*e_iter,0), a))*(vertices(edges(*e_iter,1), a) - vertices(edges(*e_iter,0), a));

      len = sqrt(len);
      maxLen = max(maxLen, len);
      minLen = min(minLen, len);
      avgLen += len;
      
      if ( len<10*REAL_MIN )
	{
	  nErr++;
	  nZeroLengthEdges++;
	  if ( verbose )
	    cout<<"Edge "<<*e_iter<<" has zero length with vertices "<<edges(*e_iter,0)<<" and "<<edges(*e_iter,1)<<endl;
    
	}
    }

  avgLen /= umap.size(UnstructuredMapping::Edge);

    if ( umap.getRangeDimension()==2 )
    {
      //        if rangeDimension==2,  b. check the areas of each "side" (edge-face combination) to look for inverted faces
      //                               c. compare the results from Face->Vertex and Face->Edge downward iterators
      //                               d. compare the results from Edge->Face upward iterator
      const intArray &faces = umap.getEntities(UnstructuredMapping::Face);

      ArraySimple< ArraySimpleFixed<real,2,1,1,1> > centers( umap.size(UnstructuredMapping::Face) );
      ArraySimple<bool> inverted(umap.size(UnstructuredMapping::Face));
      inverted = false;

      for ( e_iter=umap.begin(UnstructuredMapping::Face); e_iter!=umap.end(UnstructuredMapping::Face); e_iter++ )
	{
	  int nInvertedSides = 0;
	  real area = 0;
	  ArraySimpleFixed<real,2,1,1,1> &cent = centers[*e_iter];
	  ArraySimpleFixed<real,2,1,1,1> v1,v2;
	  cent = 0;

	  // first get the face centers for later calculations
	  int nv=0;
	  for ( d_iter=umap.adjacency_begin(e_iter, UnstructuredMapping::Vertex); 
		d_iter!=umap.adjacency_end(e_iter,UnstructuredMapping::Vertex); d_iter++, nv++ )
	    {
	      cent[0] += vertices(*d_iter,0);
	      cent[1] += vertices(*d_iter,1);
	    }
	  cent[0]/=nv;
	  cent[1]/=nv;

	  // b. check the areas of each "side" (edge-face combination) to look for inverted faces
	  // c. compare the results from Face->Vertex and Face->Edge downward iterators
	  d_iter = umap.adjacency_begin(e_iter, UnstructuredMapping::Edge);
	  for ( int v=0; v<nv; v++ )
	    {
	      for ( int a=0; a<2; a++ )
		{
		  v1[a] = vertices(faces(*e_iter,v),a);
		  v2[a] = vertices(faces(*e_iter,(v+1)%nv),a);
		}

	      real areaFromVerts = triangleArea2D(v1,v2,cent);

	      for ( int a=0; a<2; a++ )
		{
		  v1[a] = vertices(edges(*d_iter,0),a);
		  v2[a] = vertices(edges(*d_iter,1),a);
		}
	      
	      real areaFromEdge = double(d_iter.orientation())*triangleArea2D(v1,v2,cent);

	      if ( areaFromVerts<REAL_MIN ) 
		{
		  nWarn++;
		  nInvertedSides++;
		  if ( verbose )
		    cout<<"CONNECTIVITY WARNING : Face "<<*e_iter<<" has an inverted or collapsed side at index "<<v<<endl;
		}
	      
	      if ( areaFromEdge<REAL_MIN && areaFromVerts>REAL_MIN )
		{
		  nErr++;
		  nOrientationErrors[UnstructuredMapping::Edge]++;
		  if ( verbose )
		    cout<<"CONNECTIVITY ERROR : Face "<<*e_iter<<" has an inverted edge/side at edge "<<*d_iter<<endl;
		}

	      if ( fabs(areaFromVerts-areaFromEdge)>REAL_EPSILON )
		{
		  nErr++;
		  if ( verbose )
		    cout<<"CONNECTIVITY ERROR : Face "<<*e_iter<<" has an inconsistent edge at index "<<v<<" with area difference "
			<<fabs(areaFromVerts-areaFromEdge)
			<<" edge from verts "<<faces(*e_iter,v)<<"  "<<faces(*e_iter,(v+1)%nv)
			<<" edge from edge  "<<edges(*d_iter,0)<<"  "<<edges(*d_iter,1)
			<<" orientation "<<d_iter.orientation()<<endl;
		}

	      area += areaFromVerts;
	      d_iter++;
	    }

	  minArea=min(minArea,area);
	  maxArea=max(maxArea,area);
	  avgArea+=area;

	  if ( area<REAL_MIN )
	    {
	      nErr++;
	      nInvertedFaces++;
	      inverted(*e_iter) = true;
	      if ( verbose )
		cout<<"CONNECTIVITY ERROR : Face "<<*e_iter<<" is inverted"<<endl;
	    }
	}

      avgArea/=umap.size(UnstructuredMapping::Face);

      // d. compare the results from Edge->Face upward iterator
      for ( e_iter=umap.begin(UnstructuredMapping::Edge); e_iter!=umap.end(UnstructuredMapping::Edge); e_iter++ )
	{
	  for ( u_iter=umap.adjacency_begin(e_iter, UnstructuredMapping::Face); 
		u_iter!=umap.adjacency_end(e_iter, UnstructuredMapping::Face); u_iter++ )
	    {

	      int ne=0, nfv=0;
	      bool found = false;
	      for ( d_iter=umap.adjacency_begin(u_iter,UnstructuredMapping::Edge); 
		    d_iter!=umap.adjacency_end(u_iter,UnstructuredMapping::Edge); d_iter++ )
		
		{
		  if ( *d_iter!=*e_iter && !found ) 
		    ne++; 
		  else
		    found=true;

		  nfv++;
		}

	      ArraySimpleFixed<real,2,1,1,1> v1,v2,vf1,vf2;
	      for ( int a=0; a<2; a++ )
		{
		  v1[a] = vertices(edges(*e_iter,0),a);
		  v2[a] = vertices(edges(*e_iter,1),a);
		  vf1[a] = vertices(faces(*u_iter,ne),a);
		  vf2[a] = vertices(faces(*u_iter,(ne+1)%nfv),a);
		}

	      real areaFromVerts = triangleArea2D(vf1,vf2,centers[*u_iter]);
	      real areaFromEdgeUpward = u_iter.orientation()*triangleArea2D(v1,v2,centers[*u_iter]);

	      if ( (areaFromEdgeUpward<REAL_MIN && areaFromVerts>REAL_MIN) ||  (areaFromEdgeUpward>REAL_MIN && areaFromVerts<REAL_MIN) )
		{
		  nErr++;
		  nOrientationErrors[UnstructuredMapping::Face]++;
		  if ( verbose )
		    {
		      cout<<"CONNECTIVITY ERROR : Edge "<<*e_iter<<" has an inconsistent orientation for Face "<<*u_iter<<endl;
		      cout<<"                          "<<areaFromVerts<<"  "<<areaFromEdgeUpward<<endl;
		      cout<<"                          "<<edges(*e_iter,0)<<" "<<edges(*e_iter,1)<<endl;
		      cout<<"                          "<<faces(*u_iter,ne)<<" "<<faces(*u_iter,(ne+1)%nfv)<<endl;
		      cout<<"                          "<<u_iter.orientation()<<endl;
		      
		  
		    }
		}
	      
	    }
	}
    }
  else if ( umap.getRangeDimension()==3 )
    {
      //        if rangeDimension==3,  b. check the volumes of each "side" (edge-face-region combination) to look for inversions
      //                               c. compare the results of Region->Vertex, Region->Face and Region->Edge downward iterators
      //                               d. compare the results from {Edge->Face, Face->Region} and {Face->Region} upward iterators

      const intArray &regions = umap.getEntities(UnstructuredMapping::Region);
      const intArray &faces = umap.getEntities(UnstructuredMapping::Face);

      ArraySimple< ArraySimpleFixed<real,3,1,1,1> > centers( umap.size(UnstructuredMapping::Region) );
      ArraySimple<bool> inverted(umap.size(UnstructuredMapping::Region));
      inverted = false;

      for ( e_iter=umap.begin(UnstructuredMapping::Region); e_iter!=umap.end(UnstructuredMapping::Region); e_iter++ )
	{
	  int nInvertedSides = 0;
	  real vol = 0;
	  ArraySimpleFixed<real,3,1,1,1> &cent = centers[*e_iter];
	  ArraySimpleFixed<real,3,1,1,1> v1,v2,ve1,ve2,vfe1,vfe2,fc;
	  cent = 0;

	  // first get the region centers for later calculations
	  int nv=0;
	  for ( d_iter=umap.adjacency_begin(e_iter, UnstructuredMapping::Vertex); 
		d_iter!=umap.adjacency_end(e_iter,UnstructuredMapping::Vertex); d_iter++, nv++ )
	    {
	      cent[0] += vertices(*d_iter,0);
	      cent[1] += vertices(*d_iter,1);
	      cent[2] += vertices(*d_iter,2);
	    }
	  cent[0]/=nv;
	  cent[1]/=nv;
	  cent[2]/=nv;

	  // use the topology templates to get the side volumes from Region->Vertex
	  UnstructuredMapping::ElementType et = umap.computeElementType(UnstructuredMapping::Region, *e_iter);
	  
	  d_iter = umap.adjacency_begin(e_iter, UnstructuredMapping::Edge);

	  for ( int e=0; e<topoNEdges[et]; e++ )
	    {
	      for ( int a=0; a<3; a++ )
		{
		  ve1[a] = vertices(edges(*d_iter,0),a);
		  ve2[a] = vertices(edges(*d_iter,1),a);
		}

	      // loop through the faces adjacent to the edge
	      for ( u_iter=umap.adjacency_begin(d_iter, UnstructuredMapping::Face); 
		    u_iter!=umap.adjacency_end(d_iter, UnstructuredMapping::Face); u_iter++ )
		{

		  // find u_iter's face in the region->face adjacency
		  UnstructuredMappingAdjacencyIterator regFace = umap.adjacency_begin(e_iter,UnstructuredMapping::Face);
		  bool found= (*regFace==*u_iter);
		  int rf=0;
		  while ( !found && regFace!=umap.adjacency_end(e_iter,UnstructuredMapping::Face) )
		    {
		      found = (*regFace==*u_iter) ;
		      if ( !found ) { rf++; regFace++; }
		    }

		  if ( found )
		    { // this face is in both Region e_iter and Edge d_iter

		      assert(*regFace==*u_iter);
		      fc = 0;
		      int nvf = 0;
		      // get the face center
		      for ( UnstructuredMappingAdjacencyIterator fvi=umap.adjacency_begin(u_iter, UnstructuredMapping::Vertex);
			    fvi!=umap.adjacency_end(u_iter, UnstructuredMapping::Vertex); fvi++ )
			{
			  for ( int a=0; a<3; a++ )
			    fc[a] += vertices(*fvi,a);
			  nvf++;
			  
			}
		      
		      for ( int a=0; a<3; a++ )
			{
			  fc[a] /= nvf;
			}

		      // find d_iter's edge in Face->Edge adjacency
		      UnstructuredMappingAdjacencyIterator faceEdge = umap.adjacency_begin(regFace,UnstructuredMapping::Edge);
		      found = (*faceEdge==*d_iter);
		      int rfe=0;
		      while ( !found && faceEdge!=umap.adjacency_end(regFace,UnstructuredMapping::Edge) )
			{
			  found = ( *faceEdge==*d_iter );
			  if ( !found ) {  faceEdge++; }
			}

		      // now find d_iter's edge in the topo2FaceEdge mapping
		      int ee=0;
		      for ( ; ee<nvf; ee++ )
			{
			  if ( 
			      ( regions(*e_iter, topo2EdgeVert[et][topo2FaceEdge[et][rf][ee]][0])==edges(*d_iter,0) &&
				regions(*e_iter, topo2EdgeVert[et][topo2FaceEdge[et][rf][ee]][1])==edges(*d_iter,1) ) ||
			      ( regions(*e_iter, topo2EdgeVert[et][topo2FaceEdge[et][rf][ee]][0])==edges(*d_iter,1) &&
				regions(*e_iter, topo2EdgeVert[et][topo2FaceEdge[et][rf][ee]][1])==edges(*d_iter,0) ) )
			    break;
			}

		      if ( !found )
			{
			  nErr++;
			  if ( verbose )
			    cout<<"CONNECTIVITY ERROR : Could not find Edge "<<*d_iter<<" in Face "<<*regFace<<endl;
			}
		      
		      for ( int a=0; a<3; a++ )
			{
			  vfe1[a] = vertices(edges(*faceEdge,0),a);
			  vfe2[a] = vertices(edges(*faceEdge,1),a);

			  //			  if ( (edges(*faceEdge,0)==edges(*d_iter,0) || edges(*faceEdge,1)==edges(*d_iter,1)) )
			  //			    {
			  //			  ve1[a] = vertices(*e_iter, topo2Face[et]
			  //			      ve2[a] = vfe2[a];
			      //			    }

			  v1[a] = vertices(regions(*e_iter, topo2FaceVert[et][rf][ee]),a );
			  v2[a] = vertices(regions(*e_iter, topo2FaceVert[et][rf][(ee+1)%nvf]),a );
			}

		      real volFromVerts = tetVolume(fc,v2,v1,cent);
		      real volFromEdge = regFace.orientation()*tetVolume(fc,ve2,ve1,cent);
		      real volFromFaceEdge = regFace.orientation()*faceEdge.orientation()*tetVolume(fc,vfe2,vfe1,cent);

		      // b. check the volumes of each "side" (edge-face-region combination) to look for inversions
		      if ( volFromVerts<REAL_MIN ) 
			{
			  nWarn++;
			  nInvertedSides++;
			  if ( verbose )
			    cout<<"CONNECTIVITY WARNING : Region "<<*e_iter<<" has an inverted or collapsed side at index "<<e<<endl;
			}

#if 0
		      // c. compare the results of Region->Vertex, Region->Face and Region->Edge downward iterators
		      if ( volFromEdge<REAL_MIN && volFromVerts>REAL_MIN )
			{
			  nErr++;
			  nOrientationErrors[UnstructuredMapping::Edge]++;
			  if ( verbose )
			    cout<<"CONNECTIVITY ERROR : Region "<<*e_iter<<" has an inverted edge/side at edge "<<*d_iter<<endl;
			}
#endif

		      if ( (volFromFaceEdge<REAL_MIN && volFromVerts>REAL_MIN) || (volFromFaceEdge>REAL_MIN && volFromVerts<REAL_MIN) )
			{
			  nErr++;
			  nOrientationErrors[UnstructuredMapping::Edge]++;
			  if ( verbose )
			    cout<<"CONNECTIVITY ERROR : Region "<<*e_iter<<" has an inverted orientation for face "<<*regFace<<endl;
			}
#if 0
		      if ( fabs(volFromVerts-volFromEdge)>REAL_EPSILON )
			{
			  nErr;
			  if ( verbose )
			    cout<<"CONNECTIVITY ERROR : Region "<<*e_iter<<" has an inconsistent edge at index "<<
			      e<<" with volume difference "
				<<fabs(volFromVerts-volFromEdge)<<endl;
			}
#endif

		      if ( fabs(volFromVerts-volFromFaceEdge)>REAL_EPSILON )
			{
			  nErr++;
			  if ( verbose )
			    cout<<"CONNECTIVITY ERROR : Region "<<*e_iter<<" has an inconsistent face at index "<<
			      rf<<" with volume difference "
				<<fabs(volFromVerts-volFromFaceEdge)<<" volumes are "<<volFromVerts<<" "<<volFromFaceEdge<<endl;
			}

		      vol += volFromVerts;
		    }
		}

	      d_iter++;
	    }

	  minVol=min(minVol,vol);
	  maxVol=max(maxVol,vol);
	  avgVol+=vol;

	  if ( nInvertedSides || vol<REAL_MIN )
	    {
	      nErr++;
	      nInvertedFaces++;
	      inverted(*e_iter) = true;
	      if ( verbose )
		cout<<"CONNECTIVITY ERROR : Region "<<*e_iter<<" is inverted"<<endl;
	    }

	}

      avgVol/=umap.size(UnstructuredMapping::Region);

      // d. compare the results from {Edge->Face, Face->Region} and {Face->Region} upward iterators
      for ( e_iter=umap.begin(UnstructuredMapping::Edge); e_iter!=umap.end(UnstructuredMapping::Edge); e_iter++ )
	{
	  ArraySimpleFixed<real,3,1,1,1> v1,v2,fv1,fv2,fc;

	  for ( int a=0; a<3; a++ )
	    {
	      v1[a] = vertices(edges(*e_iter,0),a);
	      v2[a] = vertices(edges(*e_iter,1),a);
	    }

	  for ( UnstructuredMappingAdjacencyIterator edgeFace=umap.adjacency_begin(e_iter,UnstructuredMapping::Face);
		edgeFace!=umap.adjacency_end(e_iter,UnstructuredMapping::Face); edgeFace++ )
	    {
	      fc = 0;
	      int nfv=0;
	      UnstructuredMappingAdjacencyIterator faceEdge = umap.adjacency_begin(edgeFace,UnstructuredMapping::Edge);
	      int nfe=0; 
	      bool found = false;
	      for ( UnstructuredMappingAdjacencyIterator faceV=umap.adjacency_begin(edgeFace,UnstructuredMapping::Vertex);
		    faceV!=umap.adjacency_end(edgeFace,UnstructuredMapping::Vertex); faceV++ )
		{
		  for ( int a=0; a<3; a++ )
		    fc[a]+=vertices(*faceV,a);
		  nfv++;

		  if ( *faceEdge!=*e_iter && !found ) 
		    { nfe++; faceEdge++; }
		  else
		    found = true;
		}

	      if ( !found ) abort();
	      for ( int a=0; a<3; a++ )
		fc[a]/=nfv;

	      for ( int a=0; a<3; a++ )
		{
		  fv1[a] = vertices(faces(*edgeFace,nfe),a);
		  fv2[a] = vertices(faces(*edgeFace,(nfe+1)%nfv),a);
		}
		  
	      // and {Face->Region} upward iterator
	      for ( UnstructuredMappingAdjacencyIterator faceReg=umap.adjacency_begin(edgeFace,UnstructuredMapping::Region);
		    faceReg!=umap.adjacency_end(edgeFace,UnstructuredMapping::Region); faceReg++ )
		{
		  
		  // find the corresponding face index in the region
		  int rnf=0;
		  UnstructuredMappingAdjacencyIterator regFace=umap.adjacency_begin(faceReg, UnstructuredMapping::Face);
		  for ( ; regFace!=umap.adjacency_end(faceReg, UnstructuredMapping::Face); regFace++ )
		    if ( *regFace==*edgeFace ) 
		      break;
		    else
		      rnf++;
		  
		  if ( *regFace!=*edgeFace )
		    {
		      nErr++;
		      if ( verbose )
			cout<<"CONNECTIVITY ERROR : Cannot find face "<<*edgeFace<<" in region "<<*faceReg<<endl;
		    }

		  // compute the side volume directly from the regions
		  UnstructuredMapping::ElementType et = umap.computeElementType(UnstructuredMapping::Region, *faceReg);
		  ArraySimpleFixed<real,3,1,1,1> rv1, rv2;
		  for ( int a=0; a<3; a++ )
		    {
		      rv1[a] = vertices(regions(*faceReg, topo2FaceVert[et][rnf][nfe]),a );
		      rv2[a] = vertices(regions(*faceReg, topo2FaceVert[et][rnf][(nfe+1)%nfv]),a );
		    }

		  ArraySimpleFixed<real,3,1,1,1> &cent = centers[*faceReg];

		  real volFromRegion = tetVolume(fc,rv2,rv1,cent);
		  real volFromEdgeFaceReg = edgeFace.orientation()*faceReg.orientation()*tetVolume(fc,v2,v1,cent);
		  real volFromFaceReg = faceReg.orientation()*tetVolume(fc,fv2,fv1,cent);
		  
		  if ( (volFromRegion<REAL_MIN && volFromFaceReg>REAL_MIN) ||  (volFromRegion>REAL_MIN && volFromFaceReg<REAL_MIN))
		    {
		      nErr++;
		      nOrientationErrors[UnstructuredMapping::Region]++;
		      if ( verbose )
			cout<<"CONNECTIVITY ERROR : Face "<<*edgeFace<<
			  " has an inconsistent orientation with Region "<<*faceReg<<endl;
		    }
		  
		  if ( (volFromRegion<REAL_MIN && volFromEdgeFaceReg>REAL_MIN) || (volFromRegion>REAL_MIN && volFromEdgeFaceReg<REAL_MIN)  )
		    {
		      nErr++;
		      nOrientationErrors[UnstructuredMapping::Region]++;
		      if ( verbose )
			cout<<"CONNECTIVITY ERROR : Edge "<<*e_iter<<
			  " has an inconsistent orientation with Face "<<*edgeFace<<" and  Region "<<*faceReg<<endl;
		    }
		    
		}
	    }
	}
    }

  if ( verbose )
    {

      int i_type;
      cout<<"=== VERIFY CONNECTIVITY REPORT ================================"<<endl;
      cout<<"NUMBER OF ERRORS   : "<<nErr<<endl;
      cout<<"NUMBER OF WARNINGS : "<<nWarn<<endl; 
      cout<<"--- Entity Information ----------------------------------------"<<endl;

      i_type=UnstructuredMapping::Vertex;
      for ( UnstructuredMapping::EntityTypeEnum type = UnstructuredMapping::Vertex; 
	    type<=UnstructuredMapping::Region; type=UnstructuredMapping::EntityTypeEnum(++i_type) )
	{
	  cout<<UnstructuredMapping::EntityTypeStrings[type]<<" Count : "<<numberOfEntities[type]<<endl;
	  cout<<UnstructuredMapping::EntityTypeStrings[type]<<" Ghost : "<<numberOfGhostEntities[type]<<endl;
	}
      cout<<"--- Element Information ---------------------------------------"<<endl;

      i_type = UnstructuredMapping::triangle;
      for ( UnstructuredMapping::ElementType type = UnstructuredMapping::triangle; 
	    type<=UnstructuredMapping::hexahedron; type=UnstructuredMapping::ElementType(++i_type) )
	cout<<UnstructuredMapping::ElementTypeStrings[type]<<" Count : "<<numberOfElements[type]<<endl;
      cout<<"--- Geometric Information -------------------------------------"<<endl;
      cout<<"Min. Edge : "<<minLen<<endl<<"Max. Edge : "<<maxLen<<endl<<"Avg. Edge : "<<avgLen<<endl;
      if ( umap.getRangeDimension()==2 )
	cout<<"Min. Area : "<<minArea<<endl<<"Max. Area : "<<maxArea<<endl<<"Avg. Area : "<<avgArea<<endl;
      else
	cout<<"Min. Vol : "<<minVol<<endl<<"Max. Vol : "<<maxVol<<endl<<"Avg. Vol : "<<avgVol<<endl;
      cout<<"==============================================================="<<endl;
      
    }

  isOk = nErr==0;
  return isOk;
}
