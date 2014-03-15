#include "OGFunction.h"
#include "ParallelUtility.h"

// ------------- These functions take an int for the component number -----  


//=================================================================================
//  Assign a grid Function with values defined from the OGFunction f
//
//=================================================================================
void OGFunction::
assignGridFunction(realCompositeGridFunction & cgf, const real t)
{
  GridCollection & og = *(cgf.gridCollection);
  Index I1,I2,I3;
  for( int grid=0; grid<og.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = og[grid];
    getIndex(mg.dimension(),I1,I2,I3);
    
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // *fix me*
    const realArray & x= mg.center();
    bool ok=true;
    #ifdef USE_PPP
      realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(cgf[grid],uLocal);
      const int includeGhost=1;
      ok = ParallelUtility::getLocalArrayBounds(cgf[grid],uLocal,I1,I2,I3,includeGhost); 
    #else
      const realSerialArray & xLocal = x;
      realSerialArray & uLocal = cgf[grid];
    #endif

    if( ok )
    {
      bool isRectangular=false;
      Range N(cgf[grid].getComponentBase(0),cgf[grid].getComponentBound(0));
      gd( uLocal,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,N,t);

//       // realSerialArray u0(I1,I2,I3);
//       for( int n=cgf[grid].getComponentBase(0); n<=cgf[grid].getComponentBound(0); n++ )
//       {
// 	// cgf[grid](I1,I2,I3,n)=operator()(og[grid],I1,I2,I3,n,t);
// // 	gd( u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,t);
// // 	uLocal(I1,I2,I3,n)=u0;
// 	gd( uLocal,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,t);
//       }

    }
    
  }
    
}
  

realMappedGridFunction& OGFunction::
gd( realMappedGridFunction & result,   // put result here
    const int & ntd, const int & nxd, const int & nyd, const int & nzd,
    const Index & I1_, const Index & I2_, 
    const Index & I3_, const Index & N_, 
    const real t /* =0. */, int option /* =0 */  )
// ==============================================================================================
//  Fill values into a grid function.
// ==============================================================================================
{
  MappedGrid & mg = *result.getMappedGrid();
  bool isRectangular=false;
 
  Index I1=I1_, I2=I2_, I3=I3_, N=N_;
  
  if( I1.getLength()<=0 ) I1=result.dimension(0);
  if( I2.getLength()<=0 ) I2=result.dimension(1);
  if( I3.getLength()<=0 ) I3=result.dimension(2);
  if(  N.getLength()<=0 )  N=result.dimension(3);
  
  const realArray & x= mg.center();
  bool ok=true;
#ifdef USE_PPP
  realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(result,uLocal);
  const int includeGhost=1;
  ok = ParallelUtility::getLocalArrayBounds(result,uLocal,I1,I2,I3,includeGhost); 
#else
  const realSerialArray & xLocal = x;
  realSerialArray & uLocal = result; 
#endif

  if( ok )
  {
    gd( uLocal,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,N,t);

  }

  return result;
}


realMappedGridFunction& OGFunction::
gd( realMappedGridFunction & result,   // put result here
    const int & ntd, const int & nxd, const int & nyd, const int & nzd,
    const Index & I1_, const Index & I2_, const Index & I3_, const Index & N_, 
    const Index & J1_, const Index & J2_, const Index & J3_, const Index & M_, 
    const real t /* =0. */, int option /* =0 */  )
// ==============================================================================================
//  Fill values into a grid function.
//        result(J1,J2,J3,M) = exact(I1,I2,I3,N)
// ==============================================================================================
{
  MappedGrid & mg = *result.getMappedGrid();
  bool isRectangular=false;
 
  Index I1=I1_, I2=I2_, I3=I3_, N=N_;
  
  if( I1.getLength()<=0 ) I1=result.dimension(0);
  if( I2.getLength()<=0 ) I2=result.dimension(1);
  if( I3.getLength()<=0 ) I3=result.dimension(2);
  if(  N.getLength()<=0 )  N=result.dimension(3);
  
  Index J1=J1_, J2=J2_, J3=J3_, M=M_;
  
  if( J1.getLength()<=0 ) J1=result.dimension(0);
  if( J2.getLength()<=0 ) J2=result.dimension(1);
  if( J3.getLength()<=0 ) J3=result.dimension(2);
  if(  M.getLength()<=0 )  M=result.dimension(3);
  
  const realArray & x= mg.center();
  bool ok=true;
#ifdef USE_PPP
  realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(result,uLocal);
  const int includeGhost=1;
  ok = ParallelUtility::getLocalArrayBounds(result,uLocal,I1,I2,I3,includeGhost); 
#else
  const realSerialArray & xLocal = x;
  realSerialArray & uLocal = result; 
#endif

  if( ok )
  {
    #ifdef USE_PPP
      ok = ParallelUtility::getLocalArrayBounds(result,uLocal,J1,J2,J3,includeGhost); 
      assert( ok );
    #endif
    // for now copy into a temporary -- could do better ---
    realSerialArray temp(I1,I2,I3,N);
    gd( temp,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,N,t);
    
    uLocal(J1,J2,J3,M)=temp(I1,I2,I3,N);
  }

  return result;
}




RealDistributedArray OGFunction::operator()(const MappedGrid & c, const Index & I1, const Index & I2, 
		      const Index & I3)
{ return this->operator()(c,I1,I2,I3,Range(0,0),0.); }
RealDistributedArray OGFunction::operator()(const MappedGrid & c, const Index & I1, const Index & I2, 
		      const Index & I3, const int n)
{ return this->operator()(c,I1,I2,I3,Range(n,n),0.); }
RealDistributedArray OGFunction::operator()(const MappedGrid & c, const Index & I1, const Index & I2, 
		      const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->operator()(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::t(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->t(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::x(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->x(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::y(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->y(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::z(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->z(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::xx(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->xx(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::yy(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->yy(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::zz(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->zz(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::xy(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->xy(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::xz(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->xz(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::yz(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->yz(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::laplacian(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->laplacian(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::xxx(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->xxx(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::xxxx(const MappedGrid & c, const Index & I1, const Index & I2, 
            const Index & I3, const int n, const real t,const GridFunctionParameters::GridFunctionType & centering)
{ return this->xxxx(c,I1,I2,I3,Range(n,n),t,centering); }

RealDistributedArray OGFunction::gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd,
                                    const MappedGrid & c, const Index & I1, const Index & I2, 
				    const Index & I3, const int n, const real t,
				    const GridFunctionParameters::GridFunctionType & centering)
{ return this->gd(ntd,nxd,nyd,nzd,c,I1,I2,I3,Range(n,n),t,centering); }




// These versions take a CompositeGrid, a component Range and a time
// and return a realCompositeGridFunction

realCompositeGridFunction OGFunction::   
operator()(CompositeGrid & cg, const Index & N, const real t, const GridFunctionParameters::GridFunctionType & centering)   
{   
  Range all; Index I1,I2,I3;   
  realCompositeGridFunction cgf(cg,all,all,all,N);   
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )   
  {   
    getIndex(cg[grid].dimension(),I1,I2,I3);   
    cgf[grid](I1,I2,I3,N)= this->operator()(cg[grid],I1,I2,I3,N,t,centering);   
  }   
  return cgf;   
}
realCompositeGridFunction OGFunction::   
operator()(CompositeGrid & cg, const Index & N)
{   
  return operator()(cg,N,0.);
}
realCompositeGridFunction OGFunction::   
operator()(CompositeGrid & cg)
{   
  return operator()(cg,0,0.);
}

#undef U_FUNCTION
#define U_FUNCTION(u)   \
realCompositeGridFunction OGFunction::   \
u(CompositeGrid & cg, const Index & N, const real t, const GridFunctionParameters::GridFunctionType & centering)   \
{   \
  Range all; Index I1,I2,I3;   \
  realCompositeGridFunction cgf(cg,all,all,all,N);   \
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )   \
  {   \
    getIndex(cg[grid].dimension(),I1,I2,I3);   \
    cgf[grid](I1,I2,I3,N)= this->u(cg[grid],I1,I2,I3,N,t,centering);   \
  }   \
  return cgf;   \
}

// instantiate the macro
U_FUNCTION(t)
U_FUNCTION(x)
U_FUNCTION(y)
U_FUNCTION(z)
U_FUNCTION(xx)
U_FUNCTION(xy)
U_FUNCTION(xz)
U_FUNCTION(yy)
U_FUNCTION(yz)
U_FUNCTION(zz)
U_FUNCTION(laplacian)
U_FUNCTION(xxx)
U_FUNCTION(xxxx)


realCompositeGridFunction OGFunction::   
gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd,
   CompositeGrid & cg, const Index & N, const real t, 
   const GridFunctionParameters::GridFunctionType & centering)   
{   
  Range all; Index I1,I2,I3;   
  realCompositeGridFunction cgf(cg,all,all,all,N);   
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )   
  {   
    getIndex(cg[grid].dimension(),I1,I2,I3);   
    cgf[grid](I1,I2,I3,N)= this->gd(ntd,nxd,nyd,nzd,cg[grid],I1,I2,I3,N,t,centering);   
  }   
  return cgf;   
}

//\begin{>>OGFunctionInclude.tex}{\subsection{getCoordinates}} 
void OGFunction::
getCoordinates(const GridFunctionParameters::GridFunctionType & centering, 
	       const int & numberOfDimensions,
               const MappedGrid & c, 
	       const Index & I1, const Index & I2, const Index & I3,
               RealDistributedArray & x, 
	       RealDistributedArray & y, 
	       RealDistributedArray & z )
// =============================================================================================
// /Description:
//  Define the coordinate arrays (x,y,z) depending on the value of centering to be the
//  vertices or cell centers or face centered positions. 
//
// /centering (input): Indicates the positions of the coordinates, one of 
//   \begin{description} 
//    \item[defaultCentering] use the {\tt c.center()} array (vertices for a vertex centered grid
//       and cell centers for a cell-centered grid).
//    \item[vertexCentered] grid vertices, {\tt c.vertex()}.
//    \item[cellCentered] {\tt c.center()} for a cell-centered grid or else {\tt c.corner()} for
//        a vertex centered grid (the cell centers).
//    \item[faceCenteredAxis1] use the center of the cell face in the axis1 direction (defined
//       by averaging the {\tt c.vertex() values} for the y,z coordinates).
//    \item[faceCenteredAxis2] use the center of the cell face in the axis2 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,z coordinates).
//    \item[faceCenteredAxis3] use the center of the cell face in the axis3 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,y coordinates).
//  \end{description}
//  /numberOfDimensions (input): 1,2, or 3
//  /c (input) : MappedGrid
//  /I1,I2,I3 (input): evaluate at these points
//  /x,y,z : output 
//\end{OGFunctionInclude.tex}
// =============================================================================================
{

//  enum GridFunctionType
//  {
//    defaultCentering=-1,
//    general,
//    vertexCentered,
//    cellCentered,
//    faceCenteredAll,
//    faceCenteredAxis1,
//    faceCenteredAxis2,
//    faceCenteredAxis3
//    };
  int axis;
  switch (centering) 
  {
  case GridFunctionParameters::defaultCentering:
    x.reference(c.center()(I1,I2,I3,axis1));
    if( numberOfDimensions>1 )
      y.reference(c.center()(I1,I2,I3,axis2));
    if( numberOfDimensions>2 )
      z.reference(c.center()(I1,I2,I3,axis3));
    break;
  case GridFunctionParameters::vertexCentered:
    x.reference(c.vertex()(I1,I2,I3,axis1));
    if( numberOfDimensions>1 )
      y.reference(c.vertex()(I1,I2,I3,axis2));
    if( numberOfDimensions>2 )
      z.reference(c.vertex()(I1,I2,I3,axis3));
    break;
  case GridFunctionParameters::cellCentered:
    if( c.isAllCellCentered() )
    {
      x.reference(c.center()(I1,I2,I3,axis1));
      if( numberOfDimensions>1 )
	y.reference(c.center()(I1,I2,I3,axis2));
      if( numberOfDimensions>2 )
	z.reference(c.center()(I1,I2,I3,axis3));
    }
    else
    { // *** assumes the corner array has been created
      x.reference(c.corner()(I1,I2,I3,axis1));
      if( numberOfDimensions>1 )
	y.reference(c.corner()(I1,I2,I3,axis2));
      if( numberOfDimensions>2 )
	z.reference(c.corner()(I1,I2,I3,axis3));
    }
    break;
  case GridFunctionParameters::faceCenteredAxis1:
    if( numberOfDimensions==1 )
      x=c.vertex()(I1,I2,I3,axis1);
    else if( numberOfDimensions==2 )
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	realArray & xyz = axis==0 ? x : y;
        xyz=.5*(c.vertex()(I1,I2  ,I3,axis)+c.vertex()(I1,I2+1,I3,axis));
      }
    }
    else
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	realArray & xyz = axis==0 ? x : axis==1 ? y : z;
        xyz=.25*(c.vertex()(I1,I2  ,I3  ,axis)+c.vertex()(I1,I2+1,I3  ,axis)+
                 c.vertex()(I1,I2  ,I3+1,axis)+c.vertex()(I1,I2+1,I3+1,axis));
      }
    }
    break;
  case GridFunctionParameters::faceCenteredAxis2:
    if( numberOfDimensions==1 )
      x=.5*(c.vertex()(I1,I2,I3,axis1)+c.vertex()(I1+1,I2,I3,axis1));
    else if( numberOfDimensions==2 )
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	realArray & xyz = axis==0 ? x : y;
        xyz=.5*(c.vertex()(I1,I2,I3,axis)+c.vertex()(I1+1,I2,I3,axis));
      }
    }
    else
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	realArray & xyz = axis==0 ? x : axis==1 ? y : z;
        xyz=.25*(c.vertex()(I1  ,I2,I3  ,axis)+c.vertex()(I1+1,I2,I3  ,axis)+
                 c.vertex()(I1  ,I2,I3+1,axis)+c.vertex()(I1+1,I2,I3+1,axis));
      }
    }
    break;
  case GridFunctionParameters::faceCenteredAxis3:
    if( numberOfDimensions==1 )
      x=.5*(c.vertex()(I1,I2,I3,axis1)+c.vertex()(I1+1,I2,I3,axis1));
    else 
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	realArray & xyz = axis==0 ? x : axis==1 ? y : z;
        xyz=.25*(c.vertex()(I1  ,I2  ,I3,axis)+c.vertex()(I1+1,I2  ,I3,axis)+
                 c.vertex()(I1  ,I2+1,I3,axis)+c.vertex()(I1+1,I2+1,I3,axis));
      }
    }
    break;
  default:
    cout << "OGFunction::getCoordinates:ERROR: unknown vaule for centering = " << centering << endl;
    Overture::abort("error");
  }
}
