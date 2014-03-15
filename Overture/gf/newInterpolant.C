#include "Interpolant.h"
#include "Oges.h"

static IntegerArray nii;

Interpolant::RCData::
RCData()
{
  implicitInterpolant=NULL;
}

Interpolant::RCData::
~RCData()
{
  delete implicitInterpolant;
}


// deep copy of reference counted data 
Interpolant::RCData & Interpolant::RCData::
operator=( const Interpolant::RCData & rcdata )
{
  if( rcdata.implicitInterpolant !=NULL )
  {
    delete implicitInterpolant;
    *implicitInterpolant = *(rcdata.implicitInterpolant);      // deep copy, does this work?
  }
  return *this;
}




Interpolant::
Interpolant()
{
  initialize();
}

Interpolant::
Interpolant(CompositeGrid & cg0 )
{
  initialize();
  updateToMatchGrid( cg0 );
}

Interpolant::
Interpolant(GridCollection & )
{
 cout << "Interpolant:ERROR: Interpolant not implemented for a GridCollection \n";
}

// Copy constructor, deep copy by default
Interpolant::
Interpolant(const Interpolant & interpolant, const CopyType copyType )
{
  if( copyType==DEEP )  
  {
    initialize();
    (*this)=interpolant;
  }
  else
  {
    rcData=interpolant.rcData;
    rcData->incrementReferenceCount();
    reference( (Interpolant &) interpolant ); 
  }
}

void Interpolant::
initialize()
{
  rcData = new RCData;  
  rcData->incrementReferenceCount();
}


Interpolant::
~Interpolant()
{
  if( rcData->decrementReferenceCount() == 0 )
    delete rcData; 
}

// Assignment with = is a deep copy
Interpolant & Interpolant::
operator= ( const Interpolant & interpolant )
{
  *rcData=*interpolant.rcData;  // deep copy
  cg=interpolant.cg;
  coeff=interpolant.coeff;
  width=interpolant.width;
  return *this;
}

void Interpolant::
reference( const Interpolant & interpolant )
{
  if( this==&interpolant ) // no need to do anything if
    return;
  if( rcData->decrementReferenceCount() == 0 )
    delete rcData;   
  rcData=interpolant.rcData;
  rcData->incrementReferenceCount();
  cg.reference(interpolant.cg);
  coeff.reference(interpolant.coeff);
  width.reference(interpolant.width);
}


void Interpolant::
breakReference()
{
  // If there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    Interpolant interpolant = *this;  // makes a deep copy
    reference(interpolant);   // make a reference to this new copy
  }
}
  
//-------------------------------------------------------------------------------------------
//  Associate an interpolant with a grid and vice versa
//
//------------------------------------------------------------------------------------------
void Interpolant::
updateToMatchGrid(CompositeGrid & cg0 )
{
  if( cg0.rcData->interpolant == NULL )  // CompositeGrid does not have an interpolant yet
  {
    cg0.rcData->interpolant=this;        // here is an interpolant for the CompositeGrid
    cg.reference(cg0);
    bool implicitInterpolation=FALSE;
    for( int toGrid=0; toGrid<cg.numberOfComponentGrids; toGrid++ )
    for( int fromGrid=0; fromGrid<cg.numberOfComponentGrids; fromGrid++ )
      if( toGrid!=fromGrid )
        implicitInterpolation=implicitInterpolation || cg.interpolationIsImplicit(toGrid,fromGrid);
    
    if( implicitInterpolation ) 
    {
      cout << "Interpolant: initialize implicit interpolation...\n";
      delete rcData->implicitInterpolant;  // delete any existing one *****
      rcData->implicitInterpolant= new Oges( cg );  // Equation solver
      rcData->implicitInterpolant->setEquationType( Oges::Interpolation );
      rcData->implicitInterpolant->initialize( ); 
    }
    else
      initializeExplicitInterpolation();
  }
  else 
  {                                          // CompositeGrid already has an interpolant
    reference( *(cg0.rcData->interpolant) );  // reference this one to the existing one
  }
}  

int Interpolant::
interpolate( realMultigridCompositeGridFunction & u )
{
  int returnValue=0;
  for( int level=0; level<u.numberOfMultigridLevels; level++ )
    returnValue+=interpolate( u[level] );
  return returnValue;
}

int Interpolant::
interpolate( realCompositeGridFunction & u )
{
  if( !rcData->implicitInterpolant )  // *** fix this ***
    return explicitInterpolate( u );
  else
  {
    // Interpolate each component separately
    if( u.positionOfComponent(0) < u.positionOfCoordinate(cg.numberOfDimensions-1) )
    {
      cout << "Interpolant:interpolate:ERROR unable to interpolate grid function " << u.getName() << endl;
      cout << "The component appears before the last coordinate direction\n";
      throw "Interpolant::interpolant: fatal error";
    }
    for( int c2=u.getComponentBase(2); c2<=u.getComponentBound(2); c2++ )
    for( int c1=u.getComponentBase(1); c1<=u.getComponentBound(1); c1++ )
    for( int c0=u.getComponentBase(0); c0<=u.getComponentBound(0); c0++ )
    {
      v.link(u,Range(c0,c0),Range(c1,c1),Range(c2,c2));         // link to a component
      for( int grid=0; grid< rcData->implicitInterpolant->numberOfComponentGrids; grid++ )
	where( rcData->implicitInterpolant->classify[grid]<0 )
	  v[grid]=0.;                         // zero out interpolation and periodic points

      rcData->implicitInterpolant->solve( v,v );      // solve the equations
    }
    return 0;  
  }
}


int Interpolant::
interpolate( realGridCollectionFunction &  )
{
  cout << "Interpolant::interpolate: sorry, don't know how to interpolate"
    " a GridCollectionFunction! \n";
  return 1;
}


//===================================================================================
//  Explicit Interpolation
//===================================================================================
int Interpolant::
explicitInterpolate( realCompositeGridFunction & u ) const
{
  u.periodicUpdate();   // do this since we don't wrap the interpolation stencil
  if( cg.numberOfComponentGrids==1 ) 
    return 0;

  RealArray *ui[20];
  for( int grid=0; grid<cg.numberOfComponentGrids; grid++ )
    ui[grid]=&u[grid];

  for( grid=0; grid<cg.numberOfComponentGrids; grid++ )
  {
    IntegerArray & ip = cg.interpolationPoint[grid];    // use define?
    IntegerArray & il = cg.interpoleeLocation[grid];
    IntegerArray & ig = cg.interpoleeGrid[grid];
    
    if( Oges::debug & 8 )
    {
      ip.display("explicitInterpolate: Here is the ip array");
      il.display("explicitInterpolate: Here is the il array");
      ig.display("explicitInterpolate: Here is the ig array");
      coeff[grid].display("explicitInterpolate: Here is the coeff array");
    }    
    RealArray & ug = u[grid];
    RealArray & coeffg = coeff[grid];
    Index I(0,cg.numberOfInterpolationPoints(grid));

    // ********* fix this when there are more Index's *******************************
    int c2,c3;
    if( cg.numberOfDimensions==2 )
    {
      for( int grid2=0; grid2<cg.numberOfComponentGrids; grid2++ )
      {
        if( nii(grid,grid2,End)>=nii(grid,grid2,Start) )
	{
/* ---------
          if( useFortran )
	  {
            INTERP2D(cg[grid].dimension(0,0),cg[grid].dimension(1,0),
		     cg[grid].dimension(0,1),cg[grid].dimension(1,1),
                     u[grid].getComponentDimension(2),
                     u[grid].getComponentDimension(3),
		     u[grid].getDataPointer(),u[grid2].getDataPointer,
		     ip(nii(grid,grid2,Start),0),ip(nii(grid,grid2,Start),1),
		     il(nii(grid,grid2,Start),0),il(nii(grid,grid2,Start),1),
		     nii(grid,grid2,End)-nii(grid,grid2,Start)+1);
	    
	  }
------------- */

          RealArray & ui = u[grid2];
          Index I= Range(nii(grid,grid2,Start),nii(grid,grid2,End));
          if( width(axis1,grid)==3 && width(axis2,grid)==3 )
	  {
	    for( c3=u[grid].getBase(3); c3<=u[grid].getBound(3); c3++ )
	      for( c2=u[grid].getBase(2); c2<=u[grid].getBound(2); c2++ )
                ug(ip(I,axis1),ip(I,axis2),c2,c3)=
	          coeffg(I,0,0)*ui(il(I,axis1)  ,il(I,axis2)  ,c2,c3)
	         +coeffg(I,1,0)*ui(il(I,axis1)+1,il(I,axis2)  ,c2,c3)
	         +coeffg(I,2,0)*ui(il(I,axis1)+2,il(I,axis2)  ,c2,c3)
	         +coeffg(I,0,1)*ui(il(I,axis1)  ,il(I,axis2)+1,c2,c3)
                 +coeffg(I,1,1)*ui(il(I,axis1)+1,il(I,axis2)+1,c2,c3)
	         +coeffg(I,2,1)*ui(il(I,axis1)+2,il(I,axis2)+1,c2,c3)
	         +coeffg(I,0,2)*ui(il(I,axis1)  ,il(I,axis2)+2,c2,c3)
	         +coeffg(I,1,2)*ui(il(I,axis1)+1,il(I,axis2)+2,c2,c3)
	         +coeffg(I,2,2)*ui(il(I,axis1)+2,il(I,axis2)+2,c2,c3);
	    
	  }
	  else
	  {
	    
	    for( c3=u[grid].getBase(3); c3<=u[grid].getBound(3); c3++ )
	      for( c2=u[grid].getBase(2); c2<=u[grid].getBound(2); c2++ )
		ug(ip(I,axis1),ip(I,axis2),c2,c3)=0.;   

	    for( int w2=0; w2<width(axis2,grid); w2++ )
	    {
	      IntegerArray il2 = il(I,axis2)+w2;
	      for( int w1=0; w1<width(axis1,grid); w1++ )
	      {
		IntegerArray il1 = il(I,axis1)+w1;
		for( c3=u[grid].getBase(3); c3<=u[grid].getBound(3); c3++ )
		  for( c2=u[grid].getBase(2); c2<=u[grid].getBound(2); c2++ )
		    ug(ip(I,axis1),ip(I,axis2),c2,c3)+=coeffg(I,w1,w2)*ui(il1,il2,c2,c3);
	      }
	    }
	  }
	}
      }

/* -----------
      for( c3=u[grid].getBase(3); c3<=u[grid].getBound(3); c3++ )
	for( c2=u[grid].getBase(2); c2<=u[grid].getBound(2); c2++ )
	{
	  ug(ip(I,axis1),ip(I,axis2),c2,c3)=0.;   
	  for( int w2=0; w2<width(axis2,grid); w2++ )
	    for( int w1=0; w1<width(axis1,grid); w1++ )
 	      for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		ug(ip(i,axis1),ip(i,axis2),c2,c3)+=coeffg(i,w1,w2)*
		  (*ui[ig(i)])(il(i,axis1)+w1,il(i,axis2)+w2,c2,c3);
		  }
----------- */


/* ----
	  for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	    u[grid](ip(i,axis1),ip(i,axis2),c2,c3)=0.; 
	  for( int w2=0; w2<width(axis2,grid); w2++ )
	    for( int w1=0; w1<width(axis1,grid); w1++ )
	      for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		u[grid](ip(i,axis1),ip(i,axis2),c2,c3)+=coeff[grid](i,w1,w2)*
		  u[ig(i)](il(i,axis1)+w1,il(i,axis2)+w2,c2,c3);
---- */
	  // u[grid](ip(I,axis1),ip(I,axis2),c2,c3)=0.;    // ok
          // u[grid](ip(I,axis1),ip(I,axis2),c2,c3)+=coeff[grid](I,w1,w2)*
	  //	  u[ig(i)](il(I,axis1)+w1,il(I,axis2)+w2,c2,c3);   // ** not ok ** u[ig(i)] **
    }
    else
    {
      for( c3=u[grid].getBase(3); c3<=u[grid].getBound(3); c3++ )
      {
        for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
          u[grid](ip(i,axis1),ip(i,axis2),ip(i,axis3),c3)=0.;
	for( int w3=0; w3<width(axis3,grid); w3++ )
	  for( int w2=0; w2<width(axis2,grid); w2++ )
	    for( int w1=0; w1<width(axis1,grid); w1++ )
	      for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		u[grid](ip(i,axis1),ip(i,axis2),ip(i,axis3),c3)+=coeff[grid](i,w1,w2,w3)*
		  u[ig(i)](il(i,axis1)+w1,il(i,axis2)+w2,il(i,axis3)+w3,c3);
      }
    }
  }
  u.periodicUpdate();
  return 0;
}



//===================================================================
//  Compute Interpolation coefficients for explicit interpolation
//===================================================================
void Interpolant::
initializeExplicitInterpolation()
{

#define Q11(x) (1.-(x))
#define Q21(x) (x)

#define Q12(x) .5*((x)-1.)*((x)-2.)
#define Q22(x) (x)*(2.-(x))
#define Q32(x) .5*(x)*((x)-1.)

  if( Oges::debug & 8 )
    cout << "Interpolant: initialize explicit interpolation...\n";

  if( cg.numberOfComponentGrids <= 1 )
    return;

  int grid,axis,i;
  Index I;
  Range R;
  RealArray q,px,qq;

  
  nii.redim(cg.numberOfComponentGrids,cg.numberOfComponentGrids,2);
  for( grid=0; grid<cg.numberOfComponentGrids; grid++ )
  {
    IntegerArray & ig = cg.interpoleeGrid[grid];
    for( int i=0; i<cg.numberOfInterpolationPoints(grid)-1; i++)
      if( ig(i+1)<ig(i) )
      {
	cout << "explicitInterpolate::ERROR: interpoleeGrid is not sorted! \n";
        exit(1);
      }
      	
    for( int grid2=0; grid2<cg.numberOfComponentGrids; grid2++ )
    {
      nii(grid,grid2,Start)=0;
      nii(grid,grid2,End  )=-1;
    }
      
    if( cg.numberOfInterpolationPoints(grid) > 0 )
    {
      nii(grid,ig(0),Start)=0;
      for( i=1; i<cg.numberOfInterpolationPoints(grid); i++)
      {
	if( ig(i)>ig(i-1) )
	{
	  nii(grid,ig(i-1),End)=i-1;
	  nii(grid,ig(i)  ,Start)=i;
	}
      }
      nii(grid,ig(cg.numberOfInterpolationPoints(grid)-1),End)=cg.numberOfInterpolationPoints(grid)-1;
      
    }
  }
  nii.display("Interpolant: Here is nii(grid,grid2,0:1)");


  // Allocate space for grid with largest number of interpolation points
  R=Range(0,max(cg.numberOfInterpolationPoints(Range(0,cg.numberOfComponentGrids-1)))-1);
  
  // for now we use only one width per grid
  width.redim(3,cg.numberOfComponentGrids); width=1;
  Range NG(0,cg.numberOfComponentGrids-1);
  for( grid=0; grid<cg.numberOfComponentGrids; grid++ )
  for( axis=axis1; axis<cg.numberOfDimensions; axis++ ) 
    width(axis,grid)=max(width(axis,grid),max(cg.interpolationWidth(axis,grid,NG)));

  px.redim(R);
  q.redim(R,3,max(width));    // q holds the interpolation weigths
  q=1.;

  while( coeff.getLength() > 0 )   // empty list
    coeff.deleteElement();  

  int m1,m2,m3;
  
  for( grid=0; grid<cg.numberOfComponentGrids; grid++ )
  {

    MappedGrid & cgrid = cg[grid];


    I=R=Range(0,cg.numberOfInterpolationPoints(grid)-1);

    //.........First form 1D interpolation coefficients
    int indexPosition,gridi;
    real relativeOffset;
    for( axis=axis1; axis<cg.numberOfDimensions; axis++ ) 
    {
      for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      {
	gridi = cg.interpoleeGrid[grid](i);
	MappedGrid & cgridi = cg[gridi];
	indexPosition=cg.interpoleeLocation[grid](i,axis);
	relativeOffset=cg.interpolationCoordinates[grid](i,axis)/cgridi.gridSpacing(axis)
    	               +cgridi.indexRange(Start,axis);
	px(i)= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5 
                                            : relativeOffset-indexPosition;
//	if( width(axis,grid) < interpWidth(axis,grid,gridi) )
//	{
//	  //......interpolation width less than maximum allowed
//	  if( px(i) > width(axis,grid)/2. )
//	  {
//	    int ipx=min(int(px(i)-(width(axis,grid)-2)/2.),interpWidth(axis,grid,gridi)-width(axis,grid));
//	    px(i)-=ipx;
//	  }
//	}
      }
      
      switch (width(axis,grid))
      {
      case 3:
	//........quadratic interpolation
	q(I,axis,0)=Q12(px(I));
	q(I,axis,1)=Q22(px(I));
	q(I,axis,2)=Q32(px(I));
	break;
      case 2:
	//.......linear interpolation
	q(I,axis,0)=Q11(px(I));
	q(I,axis,1)=Q21(px(I));
	break;
      default:
	// .....order >3 - compute lagrange interpolation
	for(m1=0; m1<width(axis,grid); m1++ ) 
	{
	  q(I,axis,m1)=1.;
	  for( m2=0; m2<width(axis,grid); m2++ )
	    if( m1 != m2  )
	      q(I,axis,m1)*=(px(I)-m2)/(m1-m2);
	}
      }
    }
    //.......Now form the interpolation coefficients
  
    coeff.addElement( *(new RealArray(R,width(axis1,grid),width(axis2,grid),width(axis3,grid))) );
    for( m3=0; m3< width(axis3,grid); m3++ ) 
    for( m2=0; m2< width(axis2,grid); m2++ ) 
    for( m1=0; m1< width(axis1,grid); m1++ ) 
      coeff[grid](I,m1,m2,m3)=q(I,axis1,m1)*q(I,axis2,m2)*q(I,axis3,m3);

    // coeff[grid].display("initializeExplicitInterpolation: Here is the coeff array");
  }
}
