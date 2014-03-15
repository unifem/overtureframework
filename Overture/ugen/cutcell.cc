//#define BOUNDS_CHECK
//#define OV_DEBUG
#include "Geom.h"
#include "cutcell.hh"

namespace {

  inline int compute_i(real x0, real dx, ArraySimpleFixed<real,2,1,1,1> &p)
  {
    return int((p[0]-x0)/dx);
  }

  inline int compute_j(real y0, real dy, ArraySimpleFixed<real,2,1,1,1> &p)
  {
    return int((p[1]-y0)/dy);
  }

  inline int classifyNode(
			  int pp1, int pp2, ArraySimpleFixed<real,2,1,1,1> &ep1,
			  ArraySimpleFixed<real,2,1,1,1> &ep2,
			  ArraySimpleFixed<real,2,1,1,1> &p1, 
			  int oldVal)
  {

    //  return 1;
    real a1 = orient2d(ep1.ptr(),ep2.ptr(),p1.ptr());
    ArraySimpleFixed<real,2,1,1,1> ee,ee1,ee2;
    for ( int i=0; i<2; i++ )
      {
	ee1[i] = ep1[i] - p1[i];
	ee2[i] = ep2[i] - p1[i];
	ee[i] = ep2[i]-ep1[i];
      }
    
    real elen = ASmag2(ee);
    int rtval = oldVal;
    //            cout<<a1<<" "<<ASmag2(ee1)<<" "<<ASmag2(ee2)<<endl;
    //        cout<<ep1<<endl<<ep2<<endl<<p1<<endl;
    real tol = 1e-10;
    if ( ASmag2(ee1)/elen<tol )
      rtval = pp1;
    else if ( ASmag2(ee2)/elen<tol )
      rtval = pp2;
    else  if ( a1>0. && (oldVal==unclassifiedNode||oldVal==activeNode) )
      rtval = int(activeNode);
    else if ( a1<0. && oldVal<0 ) //&& oldVal<=0 )
      rtval = int(blankedNode);
    else 
      rtval = blankedNode;
    //  else if ( isBetweenOpenInterval2D(ep1,ep2,p1) && oldVal<0 )
    //    rtval = blankedNode;
    //  else if ( oldVal<0 )
    //    rtval = activeNode;

    return rtval;
  }
 
  void
  findAndClassifyBoundaryEdges(intArray &faces,
			       realArray &xyz,
			       real x0, real y0, 
			       real dx, real dy, int nx, int ny,
			       intArray &mask, 
			       ArraySimple<bool> &isABoundaryEdge)
  {
    int iz1,jz1,iz2=-1,jz2=-1;

    ArraySimpleFixed<real,2,1,1,1> ep1,ep2,p1,p2,p3,p4;
    ArraySimple<int> usedVertex(xyz.getLength(0),2);
    usedVertex=-1;

    for ( int ep=0; ep<faces.getLength(0); ep++ )
      {
	int pp1 = faces(ep,0);
	int pp2 = faces(ep,1);

	ep1[0] = xyz(pp1,0);
	ep1[1] = xyz(pp1,1);
	ep2[0] = xyz(pp2,0);
	ep2[1] = xyz(pp2,1);

	iz1 = min(nx-2,max(0,compute_i(x0,dx,ep1)));
	jz1 = min(ny-2,max(0,compute_j(y0,dy,ep1)));
	iz2 = min(nx-2,max(0,compute_i(x0,dx,ep2)));
	jz2 = min(ny-2,max(0,compute_j(y0,dy,ep2)));
	//      iz2 = compute_i(x0,dx,ep2);
	//      jz2 = compute_j(y0,dy,ep2);
	//cout<<"iz1, iz2, jz1, jz2 "<<iz1<<" "<<iz2<<" "<<jz1<<" "<<jz2<<endl;
	bool finished = false;
	bool isVert = fabs(xyz(pp1,0)-xyz(pp2,0))<dx*1e-10;
	bool isHorz = fabs(xyz(pp1,1)-xyz(pp2,1))<dy*1e-10;

	while ( !finished )
	  {
	    // classify the 4 nodes in cell iz1,jz1
 
	    finished = finished || (iz1==iz2 && jz1==jz2);

	    p1[0] = x0+iz1*dx;
	    p2[0] = x0+(iz1+1)*dx;
	    p3[0] = p2[0];
	    p4[0] = p1[0];
	  
	    p1[1] = y0+jz1*dy;
	    p2[1] = p1[1];
	    p3[1] = y0+(jz1+1)*dy;
	    p4[1] = p3[1];

	    int m[4];
	    m[0] = classifyNode(pp1,pp2,ep1,ep2,
				p1,mask(iz1,jz1));

	    if ( (m[0]==pp1 || m[0]==pp2 ) && usedVertex(m[0],0)==-1) 
	      {
		usedVertex(m[0],0) = iz1;
		usedVertex(m[0],1) = jz1;
	      }

	    m[1] = classifyNode(pp1,pp2,ep1,ep2,
				p2,mask(iz1+1,jz1));

	    if ( ( m[1]==pp1||m[1]==pp2) && usedVertex(m[1],0)==-1 )
	      {
		usedVertex(m[1],0) = iz1+1;
		usedVertex(m[1],1) = jz1;
	      }

	    m[2] = classifyNode(pp1,pp2,ep1,ep2,
				p3,mask(iz1+1,jz1+1));

	    if ( (m[2]==pp1||m[2]==pp2) && usedVertex(m[2],0)==-1 )
	      {
		usedVertex(m[2],0) = iz1+1;
		usedVertex(m[2],1) = jz1+1;
	      }

	    m[3] = classifyNode(pp1,pp2,ep1,ep2,
				p4,mask(iz1,jz1+1));

	    if ( (m[3]==pp1||m[3]==pp1) && usedVertex(m[3],0)==-1)
	      {
		usedVertex(m[3],0) = iz1;
		usedVertex(m[3],1) = jz1+1;
	      }

	    bool isAGridEdge = false;
	    if ( m[0]==pp1 )
	      {
		mask(iz1,jz1) = m[0];

		if ( m[1]==pp2 || ( isHorz && xyz(pp1,0)<xyz(pp2,0)) )
		  {
		    if ( m[1]==pp2 )
		      mask(iz1+1,jz1) = m[1];
		    if ( mask(iz1,jz1+1)==unclassifiedNode )
  		      mask(iz1,jz1+1) = activeNode;
  		    if ( mask(iz1+1,jz1+1)==unclassifiedNode )
  		      mask(iz1+1,jz1+1) = activeNode;
		    if ( jz1>0 && mask(iz1,jz1-1)==unclassifiedNode )
		      mask(iz1,jz1-1) = blankedNode;
		    if ( jz1>0 && mask(iz1+1,jz1-1)==unclassifiedNode )
		      mask(iz1+1,jz1-1) = blankedNode;
		    isAGridEdge = true;
		  }
		else if ( m[3]==pp2 || ( isVert && xyz(pp1,1)<xyz(pp2,1)) )
		  {
		    mask(iz1,jz1) = m[0];
		    if ( m[3]==pp2 )
		      mask(iz1,jz1+1) = m[3];
		    if ( mask(iz1+1,jz1)==unclassifiedNode )
		      mask(iz1+1,jz1) = blankedNode;
		    if ( mask(iz1+1,jz1+1)==unclassifiedNode )
		      mask(iz1+1,jz1+1) = blankedNode;
		      if ( iz1>0 && mask(iz1-1,jz1+1)==unclassifiedNode )
  		      mask(iz1-1,jz1+1) = activeNode;
  		    if ( iz1>0 && mask(iz1-1,jz1)==unclassifiedNode )
  		      mask(iz1-1,jz1) = activeNode;
		    isAGridEdge = true;
		  }
	      }
	    else if ( m[0]==pp2 )
	      {
		mask(iz1,jz1) = m[0];

		if ( m[1]==pp1 || ( isHorz && xyz(pp1,0)>xyz(pp2,0)) )
		  {
		    mask(iz1,jz1) = m[0];
		    if ( m[1]==pp1 )
		      mask(iz1+1,jz1) = m[1];
		    if ( mask(iz1,jz1+1)==unclassifiedNode )
		      mask(iz1,jz1+1) = blankedNode;
		    if ( mask(iz1+1,jz1+1)==unclassifiedNode )
		      mask(iz1+1,jz1+1) = blankedNode;
		      if ( jz1>0 && mask(iz1,jz1-1)==unclassifiedNode )
  		      mask(iz1,jz1-1) = activeNode;
  		    if ( jz1>0 && mask(iz1+1,jz1-1)==unclassifiedNode )
  		      mask(iz1+1,jz1-1) = activeNode;
		    isAGridEdge = true;
		  }
		else if ( m[3]==pp1 || ( isVert && xyz(pp1,1)>xyz(pp2,1)))
		  {
		    //		    mask(iz1,jz1) = m[0];
		    if ( m[3]==pp1 )
		      mask(iz1,jz1+1) = m[3];
		    if ( mask(iz1+1,jz1)==unclassifiedNode )
  		      mask(iz1+1,jz1) = activeNode;
  		    if ( mask(iz1+1,jz1+1)==unclassifiedNode )
  		      mask(iz1+1,jz1+1) = activeNode;
		    if ( iz1>0 && mask(iz1-1,jz1+1)==unclassifiedNode )
		      mask(iz1-1,jz1+1) = blankedNode;
		    if ( iz1>0 && mask(iz1-1,jz1)==unclassifiedNode )
		      mask(iz1-1,jz1) = blankedNode;
		    isAGridEdge = true;
		  }
	      }
	    else if ( m[2]==pp1 )
	      {
		mask(iz1+1,jz1+1) = m[2];

		if ( m[3]==pp2 || ( isHorz && xyz(pp1,0)>xyz(pp2,0) ) )
		  {
		    if ( m[3]==pp2 )
		      mask(iz1,jz1+1) = m[3];
		    if ( mask(iz1,jz1)==unclassifiedNode )
  		      mask(iz1,jz1) = activeNode;
  		    if ( mask(iz1+1,jz1)==unclassifiedNode )
  		      mask(iz1+1,jz1) = activeNode;
		    if ( jz1<(ny-2) && mask(iz1,jz1+2)==unclassifiedNode )
		      mask(iz1,jz1+2) = blankedNode;
		    if ( jz1<(ny-2) && mask(iz1+1,jz1+2)==unclassifiedNode )
		      mask(iz1+1,jz1+2) = blankedNode;
		    isAGridEdge = true;
		  }
		else if ( m[1]==pp2 || ( isVert && xyz(pp1,1)>xyz(pp2,1)) )
		  {
		    if ( m[1]==pp2 )
		      mask(iz1+1,jz1) = m[1];
		    if ( mask(iz1,jz1)==unclassifiedNode )
		      mask(iz1,jz1) = blankedNode;
		    if ( mask(iz1,jz1+1)==unclassifiedNode )
		      mask(iz1,jz1+1) = blankedNode;
		    if ( iz1<(nx-2) && mask(iz1+2,jz1)==unclassifiedNode )
  		      mask(iz1+2,jz1) = activeNode;
  		    if ( iz1<(nx-2) && mask(iz1+2,jz1+1)==unclassifiedNode )
  		      mask(iz1+2,jz1+1) = activeNode;
		    isAGridEdge = true;
		  }
	      }
	    else if ( m[2]==pp2 )
	      {
		mask(iz1+1,jz1+1) = m[2];

		if ( m[3]==pp1 || ( isHorz && xyz(pp1,0)<xyz(pp2,0) ))
		  {
		    if ( m[3]==pp1 )
		      mask(iz1,jz1+1) = m[3];
		    if ( mask(iz1,jz1)==unclassifiedNode )
		      mask(iz1,jz1) = blankedNode;
		    if ( mask(iz1+1,jz1)==unclassifiedNode )
		      mask(iz1+1,jz1) = blankedNode;
		      if ( jz1<(ny-2) && mask(iz1,jz1+2)==unclassifiedNode )
  		      mask(iz1,jz1+2) = activeNode;
  		    if ( jz1<(ny-2) && mask(iz1+1,jz1+2)==unclassifiedNode )
  		      mask(iz1+1,jz1+2) = activeNode;
		    isAGridEdge = true;
		  }
		else if ( m[1]==pp1 || ( isVert && xyz(pp1,1)<xyz(pp2,1)))
		  {
		    //		    mask(iz1+1,jz1+1) = m[2];
		    if ( m[1]==pp1 )
		      mask(iz1+1,jz1) = m[1];
		    if ( mask(iz1,jz1)==unclassifiedNode )
  		      mask(iz1,jz1) = activeNode;
  		    if ( mask(iz1,jz1+1)==unclassifiedNode )
  		      mask(iz1,jz1+1) = activeNode;
		    if ( iz1<(nx-2) && mask(iz1+2,jz1)==unclassifiedNode )
		      mask(iz1+2,jz1) = blankedNode;
		    if ( iz1<(nx-2) && mask(iz1+2,jz1+1)==unclassifiedNode )
		      mask(iz1+2,jz1+1) = blankedNode;
		    isAGridEdge = true;
		  }
	      }

	 //     cout<<"ms ";
//  	    for ( int ii=0; ii<4; ii++ ) cout<<m[ii]<<" ";
//  	    cout<<endl;

	    //	    	    mask.display("bdy mask in calc");
	    bool isp;

	    if ( isAGridEdge )
	      isABoundaryEdge(ep) = finished = true;
	    else if ( intersect2D(ep1,ep2,p1,p2,isp) && jz2<jz1 )
	      jz1--;
	    else if ( intersect2D(ep1,ep2,p3,p4,isp) && jz2>jz1 )
	      jz1++;
	    else if ( intersect2D(ep1,ep2,p2,p3,isp) && iz2>iz1 )
	      iz1++;
	    else if ( intersect2D(ep1,ep2,p1,p4,isp) && iz2<iz1 )
	      iz1--;
	    else //if ( isAGridEdge )
	      {
		if ( iz1!=iz2 || jz1!=jz2 )
		  {
		    if ( iz1!=iz2 )
		      iz1 = iz2>iz1 ? iz1+1 : iz1-1;
		    if ( jz1!=jz2 )
		      jz1 = jz2>jz1 ? jz1+1 : jz1-1;
		  }
		else
		  finished = true;
	      }

	  }
      }

    //    int nx, int ny,
    //      intArray &mask, 
    //    mask.display("before adjusting boundaries");
    for ( int ep=0; ep<faces.getLength(0); ep++ )
      {
	int pp1 = faces(ep,0);
	int pp2 = faces(ep,1);
	//	cout<<pp1<<" "<<pp2;
	if ( usedVertex(pp1,0)>-1 && usedVertex(pp2,0)==-1 )
	  {
	    //	    cout<<" "<<usedVertex(pp1,0)<<" "<<usedVertex(pp1,1)<<" "<<usedVertex(pp2,0)<<" "<<usedVertex(pp2,1);

	    if ( fabs(xyz(pp1,0)-xyz(pp2,0))<dx*1e-10 ||
		 fabs(xyz(pp1,1)-xyz(pp2,1))<dy*1e-10 )
	      {
		mask(usedVertex(pp1,0),usedVertex(pp1,1)) =blankedNode;
		isABoundaryEdge(ep) = true;
	      }
	    //	    else 
	    //	      cout<<" "<<fabs(xyz(pp1,0)-xyz(pp2,0))<<" "<<fabs(xyz(pp1,1)-xyz(pp2,1))<<endl;
	  }
	else  if ( usedVertex(pp2,0)>-1 && usedVertex(pp1,0)==-1 )
	  {
	    //	    cout<<" "<<usedVertex(pp1,0)<<" "<<usedVertex(pp1,1)<<" "<<usedVertex(pp2,0)<<" "<<usedVertex(pp2,1);

	    if ( fabs(xyz(pp1,0)-xyz(pp2,0))<dx*1e-10 ||
		 fabs(xyz(pp1,1)-xyz(pp2,1))<dy*1e-10 )
	      {
		mask(usedVertex(pp2,0),usedVertex(pp2,1)) =blankedNode;
		//		cout<<"IN HERE "<<usedVertex(pp2,0)<< " "<<usedVertex(pp2,1)<<endl;
		isABoundaryEdge(ep) = true;
	      }
	    //	    else 
	    //	      cout<<" "<<fabs(xyz(pp1,0)-xyz(pp2,0))<<" "<<fabs(xyz(pp1,1)-xyz(pp2,1))<<endl;
	  }
      }

    //    cout<<"used vertex "<<usedVertex<<endl;

    
  }
}

void cutcell(SquareMapping &square, real dx_, real dy_,
	     intArray &faces, realArray &xyz,
	     intArray &mask)
{

  square.getGrid();
  int nx=square.getGridDimensions(0);
  int ny=square.getGridDimensions(1);
    real dx = 
      real(square.getRangeBound(1,0)-square.getRangeBound(0,0))/real(nx-1);
    real dy = 
      real(square.getRangeBound(1,1)-square.getRangeBound(0,1))/real(ny-1);

//    real x0,y0,xn,yn;
//    square.getVertices(x0,xn,y0,yn);
    real x0 = square.getRangeBound(0,0);
    real y0 = square.getRangeBound(0,1);

  mask.redim(nx,ny);
  mask = int(unclassifiedNode);

  ArraySimpleFixed<real,2,1,1,1> ep1,ep2,p1,p2,p3,p4;
  ArraySimple<bool> isABoundaryEdge(faces.getLength(0));
  isABoundaryEdge = false;

  findAndClassifyBoundaryEdges(faces,xyz,x0,y0,
			       dx,dy,nx,ny,mask,isABoundaryEdge);

  ArraySimple<int> cutCounter(nx,ny);
  cutCounter = 0;

  //        mask.display("after bdy");

  int iz1=-1,jz1=-1,iz2=-1,jz2=-1;
  real tol=1e-10;//real(0);

  //      cout<<x0<<" "<<y0<<" dx, dy "<<dx<<" "<<dy<<endl;
  for ( int ep=0; ep<faces.getLength(0); ep++ )
    {

      if ( !isABoundaryEdge(ep) )
	{
	  int pp1 = faces(ep,0);
	  int pp2 = faces(ep,1);
	  
	  ep1[0] = xyz(pp1,0);
	  ep1[1] = xyz(pp1,1);
	  ep2[0] = xyz(pp2,0);
	  ep2[1] = xyz(pp2,1);

	  iz1 = min(nx-2,max(0,compute_i(x0,dx,ep1)));
	  jz1 = min(ny-2,max(0,compute_j(y0,dy,ep1)));
	  //	  assert ( (iz2==-1 && jz2==-1) || (iz2==iz1 && jz2==jz1) );
	  iz2 = min(nx-2,max(0,compute_i(x0,dx,ep2)));
	  jz2 = min(ny-2,max(0,compute_j(y0,dy,ep2)));


	  //	  	  cout<<"iz1,jz1,iz2,jz2 "<<iz1<<" "<<jz1<<" "<<iz2<<" "<<jz2<<endl;

	  //	  if (iz1==iz2 && jz1==jz2 ) 
	  //	    cout<<ep<<ep1<<endl<<ep2<<endl;

	  bool finished = false;
	  while ( !finished )
	    {
	      // classify the 4 nodes in cell iz1,jz1 
	      finished = finished || (iz1==iz2 && jz1==jz2);
	      //	      	      cout<<iz1<<" "<<jz1<<endl;
	      p1[0] = x0+iz1*dx;
	      p2[0] = x0+(iz1+1)*dx;
	      p3[0] = p2[0];
	      p4[0] = p1[0];
	      
	      p1[1] = y0+jz1*dy;
	      p2[1] = p1[1];
	      p3[1] = y0+(jz1+1)*dy;
	      p4[1] = p3[1];
	      
	      bool isp1,isp2,isp3,isp4;

	      real a1 = orient2d(ep1.ptr(),ep2.ptr(),p1.ptr());
	      real a2 = orient2d(ep1.ptr(),ep2.ptr(),p2.ptr());
	      real a3 = orient2d(ep1.ptr(),ep2.ptr(),p3.ptr());
	      real a4 = orient2d(ep1.ptr(),ep2.ptr(),p4.ptr());

	      //	      	      cout<<"areas "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<endl;

	      if ( intersect2D(ep1,ep2,p1,p2,isp1) && jz2<jz1 )
		{
		//    if ( a1>real(0) && jz1<jz2 )
//  		    {
//  		      if ( mask(iz1,jz1)<0 ) 
//  			mask(iz1,jz1)=activeNode;
		      
//  		      if ( mask(iz1+1,jz1)<0 ) 
//  			mask(iz1+1,jz1)=blankedNode;	

//  		      jz1++;
//  		    }
//  		  else
		  if ( a1<real(0) && jz1>jz2 )
		    {
		      if ( mask(iz1,jz1)<0 ) 
			mask(iz1,jz1)=blankedNode;
			
		      if ( mask(iz1+1,jz1)<0 ) 
			mask(iz1+1,jz1)=activeNode;
		      jz1--;
		    }
		  else if (!finished)
		    jz1--;
 //		    abort();
		  
		  //		  jz1--;
		}
	      else if ( intersect2D(ep1,ep2,p3,p4,isp3) && jz2>jz1 )
		{
		//    if ( a3>real(0) && jz2<jz1 )
//  		    {
//  		      if ( mask(iz1+1,jz1+1)<0 ) 
//  			mask(iz1+1,jz1+1) = activeNode;
			
//  		      if ( mask(iz1,jz1+1)<0 ) 
//  			mask(iz1,jz1+1) = blankedNode;
//  		      jz1--;
//  		    }
//  		  else
		  if ( a3<real(0) && jz2>jz1 )
		    {
		      if ( mask(iz1+1,jz1+1)<0 ) 
			mask(iz1+1,jz1+1) = blankedNode;
			
		      if ( mask(iz1,jz1+1)<0 ) 
			mask(iz1,jz1+1) = activeNode;
		      jz1++;
		    }
 		  else if (!finished)
		    jz1++;
		    //		    abort();

		  //		  jz1++;
		}
	      else if ( intersect2D(ep1,ep2,p2,p3,isp2) && iz2>iz1 )
		{
		  if ( a3>real(0) && iz1<iz2 )
		    {
		      if ( mask(iz1+1,jz1+1)<0 ) 
			mask(iz1+1,jz1+1) = activeNode;

		      if ( mask(iz1+1,jz1)<0 ) 
			mask(iz1+1,jz1) = blankedNode;
		      iz1++;
		    }
		 //   else if ( a3<real(0) && iz2<iz1 )
//  		    {
//  		      if ( mask(iz1+1,jz1+1)<0 ) 
//  			mask(iz1+1,jz1+1) = blankedNode;
		      
//  		      if ( mask(iz1+1,jz1)<0 ) 
//  			mask(iz1+1,jz1) = activeNode;
//  		      iz1--;
//  		    }
		  else if (!finished)
		    iz1++;
		    //		    abort();

		  //		  iz1++;
		}
	      else if ( intersect2D(ep1,ep2,p1,p4,isp4) && iz2<iz1 )
		{
		  if ( a1>real(0) && iz2<iz1 )
		    {
		      if ( mask(iz1,jz1)<0 ) 
			mask(iz1,jz1) = activeNode;

		      if ( mask(iz1,jz1+1)<0 ) 
			mask(iz1,jz1+1) = blankedNode;
		      iz1--;
		    }
		 //   else if ( a1<real(0) && iz2>iz1 )
//  		    {
//  		      if ( mask(iz1,jz1)<0 ) 
//  			mask(iz1,jz1) = blankedNode;
		      
//  		      if ( mask(iz1,jz1+1)<0 ) 
//  			mask(iz1,jz1+1) = activeNode;
//  		      iz1++;
//  		    }
		  else if (!finished)
		    iz1--;
		    //		    abort();
		  
		  //		  iz1--;
		}
	      else if ( (fabs(a1))<tol )
		{
		  if ( mask(iz1+1,jz1)<0 ) 
		    mask(iz1+1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p2,mask(iz1+1,jz1));
		  if ( mask(iz1+1,jz1+1)<0) 
		    mask(iz1+1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p3,mask(iz1+1,jz1+1));
		  if ( mask(iz1,jz1+1)<0) 
		    mask(iz1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p4,mask(iz1,jz1+1));

		  if ( isp1 )
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		    }
		  else if ( isp4 )
		    {
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		  else
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		}
	      else if ( (fabs(a2))<tol )
		{
		  if ( mask(iz1,jz1)<0 ) 
		    mask(iz1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p1,mask(iz1,jz1));
		  if ( mask(iz1+1,jz1+1)<0) 
		    mask(iz1+1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p3,mask(iz1+1,jz1+1));
		  if ( mask(iz1,jz1+1)<0) 
		    mask(iz1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p4,mask(iz1,jz1+1));
		  if ( isp1 )
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		    }
		  else if ( isp2 )
		    {
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		  else
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		}
	      else if ( (fabs(a3))<tol )
		{
		  if ( mask(iz1,jz1)<0 ) 
		    mask(iz1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p1,mask(iz1,jz1));
		  if ( mask(iz1+1,jz1)<0) 
		    mask(iz1+1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p2,mask(iz1+1,jz1));
		  if ( mask(iz1,jz1+1)<0) 
		    mask(iz1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p4,mask(iz1,jz1+1));
		  if ( isp3 )
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		    }
		  else if ( isp2 )
		    {
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		  else
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		}
	      else if ( (fabs(a4))<tol )
		{
		  if ( mask(iz1,jz1)<0 ) 
		    mask(iz1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p1,mask(iz1,jz1));
		  if ( mask(iz1+1,jz1)<0) 
		    mask(iz1+1,jz1) = classifyNode(pp1,pp2,ep1,ep2,p2,mask(iz1+1,jz1));
		  if ( mask(iz1+1,jz1+1)<0) 
		    mask(iz1+1,jz1+1) = classifyNode(pp1,pp2,ep1,ep2,p3,mask(iz1+1,jz1+1));
		  if ( isp3 )
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		    }
		  else if ( isp4 )
		    {
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		  else
		    {
		      if ( iz1>iz2 ) 
			iz1--;
		      else if ( iz1<iz2 )
			iz1++;
		      if ( jz1>jz2 )
			jz1--;
		      else if ( jz1<jz2 )
			jz1++;
		    }
		}
	      else if ( fabs(orient2d(p1.ptr(),p2.ptr(),ep1.ptr()))<tol && jz1!=jz2 )//==real(0) )
		{
		  if ( a1<=0 && mask(iz1,jz1)<0 ) 
		    mask(iz1,jz1)=blankedNode; 
		  else if ( a1>0 && mask(iz1,jz1)<0 ) mask(iz1,jz1)=activeNode;
		    
		  if ( a2<=0 && mask(iz1+1,jz1)<0 ) mask(iz1+1,jz1)=blankedNode;
		  else if ( a2>0 && mask(iz1+1,jz1)<0 ) mask(iz1+1,jz1)=activeNode;

		  if ( jz1<jz2 )
		    jz1++;
		  else if ( jz1>jz2 )
		    jz1--;
		}
	      else if ( fabs(orient2d(p2.ptr(),p3.ptr(),ep1.ptr()))<tol && iz1!=iz2 )//==real(0) )
		{
		  if ( a2<=0 && mask(iz1+1,jz1)<0 ) mask(iz1+1,jz1)=blankedNode;
		  else if ( a2>0 && mask(iz1+1,jz1)<0 ) mask(iz1+1,jz1)=activeNode;
		    
		  if ( a3<=0 && mask(iz1+1,jz1+1)<0 ) mask(iz1+1,jz1+1)=blankedNode;
		  else if ( a3>0 && mask(iz1+1,jz1+1)<0 ) mask(iz1+1,jz1+1)=activeNode;

		  if ( iz1<iz2 )
		    iz1++;
		  else if ( iz1>iz2 )
		    iz1--;
		}
	      else if ( fabs(orient2d(p3.ptr(),p4.ptr(),ep1.ptr()))<tol && jz1!=jz2 )//==real(0) )
		{
		  if ( a3<=0 && mask(iz1+1,jz1+1)<0 )mask(iz1+1,jz1+1)=blankedNode;
		  else if ( a3>0 && mask(iz1+1,jz1+1)<0 )mask(iz1+1,jz1+1)=activeNode;
		  
		  if ( a4<=0 && mask(iz1,jz1+1)<0) mask(iz1,jz1+1)=blankedNode;
		  else if ( a4>0 && mask(iz1,jz1+1)<0 )
		    mask(iz1,jz1+1)=activeNode;

		  if ( jz1<jz2 )
		    jz1++;
		  else if ( jz1>jz2 )
		    jz1--;
		}
	      else if ( fabs(orient2d(p4.ptr(),p1.ptr(),ep1.ptr()))<tol && iz1!=iz2 )//==real(0) )
		{
		  if ( a4<=0 && mask(iz1,jz1+1)<0 ) mask(iz1,jz1+1)=blankedNode;
		  else if ( a4>0 && mask(iz1,jz1+1)<0 ) mask(iz1,jz1+1)=activeNode;
		    
		  if ( a1<=0 && mask(iz1,jz1)<0 ) mask(iz1,jz1)=blankedNode;
		  else if ( a1>0 && mask(iz1,jz1)<0 ) mask(iz1,jz1)=activeNode;

		  if ( iz1<iz2 )
		    iz1++;
		  else if ( iz1>iz2 )
		    iz1--;
		}
	      else
		{
		  //		  finished=true;
//  		  cout<<100.*REAL_EPSILON<<endl;
//    		  cout<<"aux areas 1"<<orient2d(p1.ptr(),p2.ptr(),ep1.ptr())<<
//    		    " "<<orient2d(p2.ptr(),p3.ptr(),ep1.ptr())<<" "<<
//    		    orient2d(p3.ptr(),p4.ptr(),ep1.ptr())<<" "<<
//    		    orient2d(p4.ptr(),p1.ptr(),ep1.ptr())<<endl;
//    		  cout<<"aux areas 2"<<orient2d(p1.ptr(),p2.ptr(),ep2.ptr())<<
//    		    " "<<orient2d(p2.ptr(),p3.ptr(),ep2.ptr())<<" "<<
//    		    orient2d(p3.ptr(),p4.ptr(),ep2.ptr())<<" "<<
//    		    orient2d(p4.ptr(),p1.ptr(),ep2.ptr())<<endl;
		  //		  assert(finished);
		  if ( !finished )
		    {
		      cout<<"cutcell WARNING : recovering from roundoff confusion"<<endl;
		      if ( iz1<iz2 )
			iz1++;
		      else if ( iz1>iz2 ) 
			iz1--;

		      if ( jz1<jz2 )
			jz1++;
		      else if ( jz1>jz2 ) 
			jz1--;

		      if ( mask(iz1,jz1)<0 )
			if ( a1<=0. )
			  mask(iz1,jz1) = blankedNode;
			else
			  mask(iz1,jz1) = activeNode;

		      if ( mask(iz1+1,jz1)<0 )
			if ( a2<=0. )
			  mask(iz1+1,jz1) = blankedNode;
			else
			  mask(iz1+1,jz1) = activeNode;

		      if ( mask(iz1+1,jz1+1)<0 )
			if ( a3<=0. )
			  mask(iz1+1,jz1+1) = blankedNode;
			else
			  mask(iz1+1,jz1+1) = activeNode;

		      if ( mask(iz1,jz1+1)<0 )
			if ( a4<=0. )
			  mask(iz1,jz1+1) = blankedNode;
			else
			  mask(iz1,jz1+1) = activeNode;
		    }
//  		  cout<<"FINISHED?"<<endl;
		  //		  abort();
		}
	    
	      //mask.display("IN EDGE");
	      //	      if ( finished ) cout<<"FINISHED "<<endl;
	      
	    }
	  //  mask.display("END OF EDGE");
	}
    }

  //      mask.display("mask before 0");
#if 0
    for ( int i=0; i<nx-1; i++ )
      for ( int j=0; j<ny; j++ )
	if ( mask(i,j)>activeNode && mask(i+1,j)>activeNode && 
	     abs(mask(i,j)-mask(i+1,j))!=1 )
	  mask(i,j) = blankedNode;
    
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny-1; j++ )
	if ( mask(i,j)>activeNode && mask(i,j+1)>activeNode && 
	     abs(mask(i,j)-mask(i,j+1))!=1 )
	  mask(i,j) = blankedNode;
#endif

    //          mask.display("mask before");
  for ( int i=0; i<nx; i++ )
    {
      bool active = mask(i,0)>blankedNode;
      if ( !active ) 
	mask(i,0) = blankedNode;
      //      else if (mask(i,0)==activeNode)
      //	mask(i,0) = activeNode;

      for ( int j=1; j<ny; j++ )
	{
	  if (mask(i,j)==unclassifiedNode) 
	    {
	      if ( active ) 
		mask(i,j) = activeNode;
	      else
		mask(i,j) = blankedNode;
	    }
	  else if ( mask(i,j)==activeNode )
	    {
	      if ( !active )
		active = true;
	      mask(i,j) = activeNode;
	    }
	  else if ( mask(i,j)>int(activeNode) )
	    {
	      active = !active;
	    }
	  else if ( mask(i,j)==blankedNode )
	    {
	      if ( active )
		active=false;
	      mask(i,j) = blankedNode;
	    }
	  else
	    abort();
	}
    }
  //        mask.display("here is the mask");
}

