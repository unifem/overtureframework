// --------------------------------------------------------------------------------------------------------
// Version 1.0  : Summer 1996 - Eugene Sy
//         1.1  : 961129 - Bill Henshaw
//                o Changes for new Mapping format (neww array ordering in calles to map, inverseMap)
//         1.2  : 971030 - Bill Henshaw. 
//
// --------------------------------------------------------------------------------------------------------

#include "multigrid1.h"
#include "EllipticTransform5.h"
#include "TridiagonalSolver.h"
#include "ComposeMapping.h"
#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "Square.h"

 multigrid mulgrid;
//\begin{>EllipticTransformInclude.tex}{\subsubsection{Constructor}}
EllipticTransform::
EllipticTransform() 
: Mapping(2,2,parameterSpace,cartesianSpace) 
//===========================================================================
// /Purpose: 
//    Create a mapping that can be used to generate an elliptic grid
//   from an existing grid. This can be useful to smooth out an existing Mapping.
// 
//\end{EllipticTransformInclude.tex}
//===========================================================================
{ 
  EllipticTransform::className="EllipticTransform";
  setName( Mapping::mappingName,"ellipticTransform");
  userMap=NULL;
  compose=FALSE;
  dpm=NULL;
  ellipticGridDefined=FALSE;

  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );

  //setting defaults.
  project=TRUE;
  omega=0.5;
  lambda=0;
  maxIter=50;
  epsilon=REAL_EPSILON*100;  // relative error tolerance
  numDim=2;
  iDim=21;
  jDim=10;
  gridBc.redim(numDim,numDim);
  gridBc=1;
  solutionMethod=Jacobi;

  srcDefault=numOfILineSources
	    =numOfJLineSources
	    =numOfPointSources=0;

  powOfJLineSources.redim(srcDefault);
  difOfJLineSources.redim(srcDefault);
  locOfJLineSources.redim(srcDefault);

  powOfPointSources.redim(srcDefault);
  difOfPointSources.redim(srcDefault);
  locOfPointSources.redim(numDim,srcDefault);

  powOfILineSources.redim(srcDefault);
  difOfILineSources.redim(srcDefault);
  locOfILineSources.redim(srcDefault);

  dB.redim(numDim,numDim);
  dS.redim(numDim,numDim);
  mappingHasChanged();
}

// Copy constructor is deep by default
EllipticTransform::
EllipticTransform( const EllipticTransform& map, const CopyType copyType )
{
  EllipticTransform::className="EllipticTransform";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "EllipticTransform:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

EllipticTransform::
~EllipticTransform()
{ 
  if( debug & 4 )
    cout << " EllipticTransform::Destructor called" << endl;
}

EllipticTransform & EllipticTransform::
operator=( const EllipticTransform & X )
{
  if( EllipticTransform::className != X.getClassName() )
  {
    cout << "EllipticTransform::operator= ERROR trying to set a EllipticTransform= to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  // *********** FINISH THIS ****************

  userMap=X.userMap;     // **** we only copy the pointer here **** fix
  compose=X.compose;
  project=X.project;
  ellipticGridDefined=X.ellipticGridDefined;
  
  this->Mapping::operator=(X);            // call = for base class
  return *this;
}

//\begin{>>EllipticTransformInclude.tex}{\subsubsection{get}}
int EllipticTransform::
get( const GenericDataBase & dir, const aString & name)
//===========================================================================
// /Description:
//    Get a mapping from the database.
// /dir (input): get the Mapping from a sub-directory of this directory.
// /name (input) : name of the sub-directory to look for the Mapping in.
//\end{EllipticTransformInclude.tex}
//===========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering EllipticTransform::get" << endl;

  subDir.get( EllipticTransform::className,"className" ); 
  if( EllipticTransform::className != "EllipticTransform" )
  {
    cout << "EllipticTransform::get ERROR in className!" << endl;
  }
  subDir.get(ellipticGridDefined,"ellipticGridDefined");

  // ****** NOTE **** we should be saving all the other arrays that define the elliptic transformation *********

  subDir.get(project,"project");
  if( project )
  {
    if( compose==NULL )
      compose= new ComposeMapping;
    compose->get(subDir,"Compose Mapping");
    dpm=(DataPointMapping*)(& compose->map1.getMapping());  // set dpm equal to the one in the compose mapping.
    userMap=& compose->map2.getMapping();
  }
  else
  {
    aString userMapClassName;
    subDir.get(userMapClassName,"userMapClassName");  
    userMap = Mapping::makeMapping( userMapClassName ); // ***** this does a new -- who will delete? ***
    if( userMap==NULL )
    {
      cout << "EllipticTransform::get:ERROR: reading in the userMap with className=" << userMapClassName << endl;
      return 1;
    }
    userMap->get(subDir,"userMap");
    if( dpm==NULL )
      dpm = new DataPointMapping;
    dpm->get(subDir,"DataPointMapping");
  }
  

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;
  return 0;
}

//\begin{>>EllipticTransformInclude.tex}{\subsubsection{put}}
int EllipticTransform::
put( GenericDataBase & dir, const aString & name) const
//===========================================================================
// /Description:
//    Save a mapping into a database.
// /dir (input): put the Mapping into a sub-directory of this directory.
// /name (input) : name of the sub-directory to save the Mapping in.
//\end{EllipticTransformInclude.tex}
//===========================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  
  subDir.put( EllipticTransform::className,"className" );
  subDir.put(ellipticGridDefined,"ellipticGridDefined");
  subDir.put(project,"project");
  if( project )
  {
    compose->put(subDir,"Compose Mapping");
  }
  else
  {
    subDir.put(userMap->getClassName(),"userMapClassName");  // save the class name so we can "make" this type in get
    userMap->put(subDir,"userMap");
    dpm->put(subDir,"DataPointMapping");
  }
  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}


Mapping *EllipticTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==EllipticTransform::className )
    retval = new EllipticTransform();
  return retval;
}


void EllipticTransform::
map(const RealArray & r, 
    RealArray & x, 
    RealArray & xr /* = Overture::nullRealArray() */,
    MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  if( ellipticGridDefined )
  {
    if( project )
      compose->map(r,x,xr,params);
    else
      dpm->map(r,x,xr,params);
  }
  else if( userMap!=NULL ){
    userMap->map(r,x,xr,params);
  }
  else
  {
    cout << "EllipticTransform::map:ERROR: no mapping defined yet! \n";
  }
}

void EllipticTransform::
inverseMap(const RealArray & x, 
	   RealArray & r, 
	   RealArray & rx /* = Overture::nullRealArray() */,
	   MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  if( ellipticGridDefined )
  {
    if( project )
      compose->inverseMap(x,r,rx,params);
    else
      dpm->inverseMap(x,r,rx,params);
  }
  else if( userMap!=NULL )
    userMap->inverseMap(x,r,rx,params);
  else
  {
    cout << "EllipticTransform::inverseMap:ERROR: no mapping defined yet! \n";
  }
  
}

int EllipticTransform::
setup()
{
  // set the domain and range for userMap 

  setName(mappingName,aString("elliptic-")+userMap->getName(mappingName));
  setDomainDimension(userMap->getDomainDimension());
  setRangeDimension(userMap->getRangeDimension());

  if( dpm==NULL )
    dpm = new DataPointMapping;

  // get grid resolution from userMap:
  int axis;
  for( axis=axis1; axis<domainDimension; axis++ )
    setGridDimensions(axis,userMap->getGridDimensions(axis));

  iDim=getGridDimensions(axis1);
  jDim=getGridDimensions(axis2);
  di=1.0/(iDim-1);	dj=1.0/(jDim-1);
  ib=iDim-1;		jb=jDim-1;

  //Initialize elliptic transform to the identity
  rTilde.resize(iDim,jDim,domainDimension);
  for (int i=0;i<iDim;i++)
    for (int j=0;j<jDim;j++)   
    {
	rTilde(i,j,0)=i*di;
	rTilde(i,j,1)=j*dj;
    }

  if( project )
  {
    dpm->setRangeSpace(parameterSpace);
    if( compose==NULL )
      compose=new ComposeMapping;
    assert( userMap );
    dpm->setDomainDimension(userMap->getDomainDimension());
    dpm->setRangeDimension(userMap->getDomainDimension());
    
    compose->setMappings(*dpm,*userMap);
    for( axis=0; axis<domainDimension; axis++)
    {
      compose->setGridDimensions(axis,userMap->getGridDimensions(axis));
      for( int side=Start; side<=End; side++ )
      {
	compose->setBoundaryCondition(side,axis,userMap->getBoundaryCondition(side,axis));
	compose->setShare(side,axis,userMap->getShare(side,axis));
      }		
      compose->setIsPeriodic(axis,userMap->getIsPeriodic(axis));
    }
   dpm->setDataPoints(rTilde,domainDimension,domainDimension);
  }
  else
  {
    dpm->setRangeSpace(parameterSpace);
    ellipticGridDefined=FALSE;
  }
  for( axis=0; axis<domainDimension; axis++)
  {
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,userMap->getBoundaryCondition(side,axis));
      setShare(side,axis,userMap->getShare(side,axis));
    }		
    setIsPeriodic(axis,userMap->getIsPeriodic(axis));
  }

  mappingHasChanged();
  return 0;
}


//\begin{>>EllipticTransformInclude.tex}{\subsubsection{generateGrid}}
void EllipticTransform::
generateGrid(GenericGraphicsInterface *gi /* = NULL */, 
             GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
//===========================================================================
// /Description:
//    This function performs the iterations to solve the elliptic grid equations.
// /gi (input) : supply a graphics interface if you want to see the grid as it
//    is being computed.
// /parameters (input) : optional parameters used by the graphics interface.
//\end{EllipticTransformInclude.tex}
//===========================================================================
{
  ellipticGridDefined=TRUE;

  int ipos,jpos,i,j,k,iteration,nd;
  real g11,s,oldError,errorsum,dMax,inv2di, inv2dj;
  real di2, dj2, inv4didj;
  int nPts=iDim*jDim;
  real xdis,ydis,dSMax,olddSMax;
  char buff[80];

  realArray Error(numDim);
  Range Rr(0,domainDimension-1);
  Range Rx(0,rangeDimension-1);

  SquareMapping square(0.,1.,0.,1.);
  square.setGridDimensions(axis1,userMap->getGridDimensions(axis1));
  square.setGridDimensions(axis2,userMap->getGridDimensions(axis2));
  MappedGrid mg(square);
  mg.update();
  Range all;

  realMappedGridFunction v, v0;
  realArray Src;
  realArray &u=v, &u0=v0;

  v0 = realMappedGridFunction(mg,all,all,all,rangeDimension);

  Src.redim(u0);

  //Get coordinate index including ghost points
  Index I1, I2, I3;
  ::getIndex(mg.dimension(),I1,I2,I3);

  //Get grid index (interior + boundary without ghost points)
  Index Ig, Jg, Kg;
  ::getIndex(mg.gridIndexRange(),Ig,Jg,Kg);

  // Initialize grid functions u0 to contain the x, y,
  // and z component of the initial grid. Also initialize
  // the source terms to 0

  if ((xe.getLength(0)<Ig.getBound())||(xe.getLength(1)<Jg.getBound())||
      (xe.getLength(2)<Kg.getBound())){
    xe.redim(Ig,Jg,Kg,Rr);
    xe=0.0;
    if (project)
      rTilde.redim(Ig,Jg,Kg,Rr);
  }

  if (max(fabs(xe))<0.00001)
  {
   RealArray r(I1,I2,I3,Rr);
   real a;

   //Define the domain of the mapping including ghost points
   //start with the x-values then the y-values
   
   a=1.0/(real(userMap->getGridDimensions(axis1)-1));
   for (k=I3.getBase();k<=I3.getBound(); k += I3.getStride())
     for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
       r(I1,j,k,0).seqAdd(-a,a);

   if (rangeDimension>1){
     a=1.0/(real(userMap->getGridDimensions(axis2)-1));
     for (k=I3.getBase();k<=I3.getBound();k+=I3.getStride())
       for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
         r(i,I2,k,1).seqAdd(-a,a);
   }

   if (rangeDimension>2){
     a=1.0/(real(userMap->getGridDimensions(axis3)-1));
     for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
       for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
	 r(i,j,I3,2).seqAdd(-a,a);
   }

   // Initialize the grid function u0 to the range (Image)
   // of the userMap
   userMap->mapGrid(r,u0);
  }
  else{
   if ((numDim==2)&&(xe.dimension(2) != u0.dimension(2)))
     xe.reshape(xe.dimension(0),xe.dimension(1),u0.dimension(2),
		xe.dimension(2));
   u0(Ig,Jg,Kg,Rr)=xe(Ig,Jg,Kg,Rr);
   //The ghost points
   if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
    u0(I1,jb+1,I3,Rr)=u0(I1,1,I3,Rr);
    u0(I1,-1,I3,Rr)=u0(I1,jb-1,I3,Rr);
   }
   else{
    u0(I1,jb+1,I3,Rr)=2.*u0(I1,jb,I3,Rr)-u0(I1,jb-1,I3,Rr);
    u0(I1,-1,I3,Rr)=2.*u0(I1,0,I3,Rr)-u0(I1,1,I3,Rr);
   }

   if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
    u0(ib+1,I2,I3,Rr)=u0(1,I2,I3,Rr);
    u0(-1,I2,I3,Rr)=u0(ib-1,I2,I3,Rr);
   }
   else{
    u0(ib+1,I2,I3,Rr)=2.*u0(ib,I2,I3,Rr)-u0(ib-1,I2,I3,Rr);
    u0(-1,I2,I3,Rr)=2.*u0(0,I2,I3,Rr)-u0(1,I2,I3,Rr);
   }
  }
  //xe.display("xe avant multigrid");


   u=u0;

  (mulgrid.gridBc).redim(gridBc);
  (mulgrid.dB).redim(dB);
  mulgrid.userMap = userMap;
  mulgrid.gridBc = gridBc;
  mulgrid.dB=dB;

  mulgrid.numberOfPeriods=numOfPeriods;
  mulgrid.numberOfPointattractions=numOfPointSources;
  mulgrid.numberOfIlineattractions=numOfILineSources;
  mulgrid.numberOfJlineattractions=numOfJLineSources;
  if (numOfPointSources>0){
   mulgrid.APointcoeff = new real[numOfPointSources];
   mulgrid.CPointcoeff = new real[numOfPointSources];
   if (rangeDimension==1)
     mulgrid.PointAttraction=mulgrid.IntArray2d(0,numOfPointSources-1,
						0,0);
   else if (rangeDimension==2)
     mulgrid.PointAttraction=mulgrid.IntArray2d(0,numOfPointSources-1,
						0,1);
   else if (rangeDimension==3)
     mulgrid.PointAttraction=mulgrid.IntArray2d(0,numOfPointSources-1,
						0,2);
   for (i=0;i<numOfPointSources;i++){
    mulgrid.APointcoeff[i]=powOfPointSources(i);
    mulgrid.CPointcoeff[i]=difOfPointSources(i);
    if (rangeDimension==1) 
     mulgrid.PointAttraction[i][0]=locOfPointSources(0,i);
    else if (rangeDimension==2){
     mulgrid.PointAttraction[i][0]=locOfPointSources(0,i);
     mulgrid.PointAttraction[i][1]=locOfPointSources(1,i);
    }
    else if (rangeDimension==3){
     mulgrid.PointAttraction[i][0]=locOfPointSources(0,i);
     mulgrid.PointAttraction[i][1]=locOfPointSources(1,i);
     mulgrid.PointAttraction[i][2]=locOfPointSources(2,i);
    }
   }
  }

  if (numOfILineSources>0){
   mulgrid.ILineAttraction = new int[numOfILineSources];
   mulgrid.AIlinecoeff = new real[numOfILineSources];
   mulgrid.CIlinecoeff = new real[numOfILineSources];
   for (i=0;i<numOfILineSources;i++){
    mulgrid.AIlinecoeff[i]=powOfILineSources(i);
    mulgrid.CIlinecoeff[i]=difOfILineSources(i);
    mulgrid.ILineAttraction[i]=locOfILineSources(i);
   }
  }
  if (numOfJLineSources>0){
   mulgrid.JLineAttraction = new int[numOfJLineSources];
   mulgrid.AJlinecoeff = new real[numOfJLineSources];
   mulgrid.CJlinecoeff = new real[numOfJLineSources];
   for (i=0;i<numOfJLineSources;i++){
    mulgrid.AJlinecoeff[i]=powOfJLineSources(i);
    mulgrid.CJlinecoeff[i]=difOfJLineSources(i);
    mulgrid.JLineAttraction[i]=locOfJLineSources(i);
   }
  }
 
  if (rangeDimension==1)
   mulgrid.setup(rangeDimension, //range dimension=ndim
	         4,              //Initial number of multigrid levels
	         u0(0,0,0,0),    //for a0x
	         u0(Ig.getBound(),0,0,0), //for b0x
	         userMap->getGridDimensions(axis1)); // for idim
  else if (rangeDimension==2)
   mulgrid.setup(rangeDimension, //range dimension=ndim
	         4,              //Initial number of multigrid levels
	         u0(0,0,0,0),    //for a0x
	         u0(Ig.getBound(),0,0,0), //for b0x
	         userMap->getGridDimensions(axis1), // for idim
	         u0(0,0,0,1),    //for a0y
	         u0(0,Jg.getBound(),0,1), //for b0y
                 userMap->getGridDimensions(axis2));// for jdim
  else if (rangeDimension==3)
   mulgrid.setup(rangeDimension, //range dimension=ndim
	         4,              //Initial number of multigrid levels
	         u0(0,0,0,0),    //for a0x
	         u0(Ig.getBound(),0,0,0), //for b0x
	         userMap->getGridDimensions(axis1), // for idim
	         u0(0,0,0,1),    //for a0y
	         u0(0,Jg.getBound(),0,1), //for b0y
	         userMap->getGridDimensions(axis2), // for jdim
	         u0(0,0,0,2),    //for a0z
	         u0(0,0,Kg.getBound(),2), //for b0z
	         userMap->getGridDimensions(axis3));// for kdim
  //mulgrid.applyMultigrid(u0,Ig,Jg,Kg,Rr);
  mulgrid.applyMultigrid(u0,I1,I2,I3,Rr);
  Index I11=Range(Ig.getBase()+1, Ig.getBound()-1);

  u(I1,I2,I3,Rr)=mulgrid.u[0](I1,I2,I3,Rr);

  u0(I1,I2,I3,Rr)=u(I1,I2,I3,Rr);
  xe(Ig,Jg,Kg,Rr)=u0(Ig,Jg,Kg,Rr);
  //xe.display("xe Apres multigrid");
   //Reset DataPointMapping and project the boundary, and plot every few iterations
  if (numDim==2) xe.reshape(xe.dimension(0),xe.dimension(1),xe.dimension(3));
  resetDataPointMapping(xe,Ig,Jg,Kg);
  //printf("Apres resetDatapointMapping\n");
    //for (int j11=Jg.getBase(); j11<=Jg.getBound(); j11++)
    //for (int i11=Ig.getBase(); i11<=Ig.getBound(); i11++)
     //printf("i=%i\t j=%i\t x=%g\t y=%g\n",i11,j11,xe(i11,j11,0,0),xe(i11,j11,0,1));
  //u0(Ig,Jg,Kg,Rx)=xe(Ig,Jg,Kg,Rx);
                  
  //parameters.set(GI_TOP_LABEL,sPrintF(buff,"iteration = %i",iteration));
  //xe(Ig,0,0,Range(0,1))=u(Ig,0,0,Range(0,1));
 
/**** DEBUG 
    {
     realArray X(Ig,2), Y(Ig,2), Xnorm(Ig), Ynorm(Ig);
     X(I11,0)=u0(I11+1,16,0,0)-u0(I11-1,16,0,0);
     Y(I11,0)=u0(I11,16,0,0)-u0(I11,15,0,0);
     X(I11,1)=u0(I11+1,16,0,1)-u0(I11-1,16,0,1);
     Y(I11,1)=u0(I11,16,0,1)-u0(I11,15,0,1);
     Xnorm(I11)=sqrt(X(I11,0)*X(I11,0)+
                          X(I11,1)*X(I11,1));
     Ynorm(I11)=sqrt(Y(I11,0)*Y(I11,0)+
                          Y(I11,1)*Y(I11,1));
     ((X(I11,0)*Y(I11,0)+X(I11,1)*Y(I11,1))/
     (Xnorm(I11)*Ynorm(I11))).display("Le dot product project");

       ((u0(Ig+1,0,0,0)-u0(Ig,0,0,0))*
        (u0(Ig-1,0,0,0)-u0(Ig,0,0,0))+
	(u0(Ig+1,0,0,1)-u0(Ig,0,0,1))*
	(u0(Ig-1,0,0,1)-u0(Ig,0,0,1))).display("VOYONS");
   }

     DEBUG  *******/
 

  gi->erase();
  fflush(stdout);
  printf("\n");
  PlotIt::plot(*gi,*this,parameters);   // *** recompute every time ?? ***
  gi->redraw(TRUE);   // force a redraw
 }
	
void EllipticTransform::
resetDataPointMapping( realArray & x, Index Ig, Index Jg, Index Kg )
// Protected function.
// Change the data point mapping to be consistent with this x
// Project the boundary back onto the original mapping
{
  //x.display("C'EST x");
  //printf("iDim=%i\t jDim=%i\n",iDim,jDim);
  if( project )
  {
    x.reshape(iDim*jDim,rangeDimension);
    rTilde.reshape(iDim*jDim,domainDimension);

    userMap->inverseMap(x,rTilde);

    rTilde.reshape(iDim,jDim,domainDimension);
    // project boundaries of rTilde

//  for( int axis=axis1; axis<domainDimension; axis++ )
//    for( int side=Start; side<=End; side++ )

    Range I(0,iDim-1);
    Range J(0,jDim-1);
    rTilde(I , 0,axis2)=0.;
    rTilde(I ,jb,axis2)=1.;
    rTilde(0 ,J ,axis1)=0.;
    rTilde(ib,J ,axis1)=1.;
  
    dpm->setDataPoints(rTilde,domainDimension,domainDimension);

    rTilde.reshape(iDim*jDim,domainDimension);
    userMap->map(rTilde,x);

    int klength=Kg.getBound()-Kg.getBase()+1;
    rTilde.reshape(iDim,jDim,domainDimension);
    x.reshape(iDim,jDim,klength,rangeDimension);
  }
  else
  {
    // no projection so we just make a dpm for the given grid points
    dpm->setDataPoints(x,domainDimension,domainDimension);
  }
  mappingHasChanged();
}


real EllipticTransform::
vDot(realArray a,realArray b,int dim)
{
  real output=0.0;
  for (int i=0;i<dim;i++)
    output += a(i)*b(i);
  return(output);
}


void EllipticTransform::
initialize()
{

  //we must make sure that all the sources are zero at start.

  powOfILineSources=0.0;
  difOfILineSources=0.0;
  locOfILineSources=0;

  powOfJLineSources=0.0;
  difOfJLineSources=0.0;
  locOfJLineSources=0;

  powOfPointSources=0.0;
  difOfPointSources=0.0;
  locOfPointSources=0;

  //acknowledge periodicity, else guess Dirichlet.

  int axisnum=userMap->getDomainDimension();
  for (int axis=0;axis<axisnum;axis++)	{
    if (userMap->getIsPeriodic(axis) != notPeriodic)
      gridBc(0,axis)=gridBc(1,axis)=-1;
    else
      gridBc(0,axis)=gridBc(1,axis)=1;
  }

  //set the correct ib,jb for boundaries.

  ib=iDim-1;
  jb=jDim-1;

  di=1.0/ib;
  dj=1.0/jb;

	//set the dB boundary layer thickness to default.

   realArray x;
   x=userMap->getGrid();
   Range I(0,ib), J(0,jb);
   dS(0,0) = max(sqrt((x(1,J,0,0)-x(0,J,0,0))*(x(1,J,0,0)-x(0,J,0,0))+
		      (x(1,J,0,1)-x(0,J,0,1))*(x(1,J,0,1)-x(0,J,0,1))));
   dS(1,0) = max(sqrt((x(ib,J,0,0)-x(ib-1,J,0,0))*(x(ib,J,0,0)-x(ib-1,J,0,0))+
		      (x(ib,J,0,1)-x(ib-1,J,0,1))*(x(ib,J,0,1)-x(ib-1,J,0,1))));
   dS(0,1) = max(sqrt((x(I,1,0,0)-x(I,0,0,0))*(x(I,1,0,0)-x(I,0,0,0))+
		      (x(I,1,0,1)-x(I,0,0,1))*(x(I,1,0,1)-x(I,0,0,1))));
   dS(1,1) = max(sqrt((x(I,jb,0,0)-x(I,jb-1,0,0))*(x(I,jb,0,0)-x(I,jb-1,0,0))+
		      (x(I,jb,0,1)-x(I,jb-1,0,1))*(x(I,jb,0,1)-x(I,jb-1,0,1))));
  dB=dS;


  //set number of periods to default 1.

  numOfPeriods=1;
}


real EllipticTransform::
Signf(real x)
{
  if (x<0) return (-1.0);
  if (x>0) return (1.0);
  return(0.0);
}


void EllipticTransform::
findSourceTerms(realArray &Src, const int rangedimension, Index I, Index J, Index K)
{

  int k, m, i, j, indexstart[3], indexend[3], indexstride[3], numPeriod;
  real distFromILine,distMagnitude,iLineSourceValue=0.,
       iPointSourceValue=0.,distFromIPoint,distFromJPoint;
  real pBndSrc,rtemp1, rtemp2;   // rtemp are temporary storage variables
  realArray IJKSrc(I,J,K,2); // To store the i, j and k coordinate of points
  realArray ptmp(I,J,K,4);   // To store temporary results
  realArray pInterpolate(I,J,K,2); // contains the interpolated values of 
				 // boundary points of pSrc (linear interp
				 // along i + linear interpolation along j).
				 // Similar to pBndSrc from old code

  ptmp=0.0;
  pInterpolate=0.0;
  realArray JReal;
  indexstart[0]=I.getBase(), indexend[0]=I.getBound(), indexstride[0]=I.getStride();
  indexstart[1]=J.getBase(), indexend[1]=J.getBound(), indexstride[1]=J.getStride();
  indexstart[2]=K.getBase(), indexend[2]=K.getBound(), indexstride[2]=K.getStride();


  JReal.resize(1,J);
  JReal.seqAdd(indexstart[1],indexstride[1]);
  for (k=indexstart[2];k<=indexend[2];k+=indexstride[2])
   for (i=indexstart[0];i<=indexend[0];i+=indexstride[0])
    for (m=0;m<rangedimension;m++)
     pInterpolate(i,J,k,m)=(JReal*exp(-JReal*dj*lambda)*Src(i,jb,k,m)+
                           (jb-JReal)*exp(-dj*lambda*(jb-JReal))*Src(i,0,k,m))/jb+
		           (i*exp(-di*lambda*real(i))*Src(ib,J,k,m)+
                           (ib-i)*exp(-di*lambda*real(ib-i))*Src(0,J,k,m))/ib;
     /*pInterpolate(i,J,k,m)=(JReal*Src(i,jb,k,m)+
                           (jb-JReal)*Src(i,0,k,m))/jb+
		           (i*Src(ib,J,k,m)+
                           (ib-i)*Src(0,J,k,m))/ib;*/

  if ((fabs(powOfILineSources(0))>1.0e-5)||
      (fabs(powOfPointSources(0))>1.0e-5))  {

      for (k=indexstart[2];k<=indexend[2];k+=indexstride[2]){
	for (j=indexstart[1];j<=indexend[1];j+=indexstride[1])
           IJKSrc(I,j,k,0).seqAdd(indexstart[0],indexstride[0]);
        for (i=indexstart[0];i<=indexend[0];i+=indexstride[0])
	   IJKSrc(i,J,k,1).seqAdd(indexstart[1],indexstride[1]);
     }

     if ((gridBc(0,0)==-1)||(gridBc(1,0)==-1)) numPeriod=numOfPeriods;
			//Periodicity in the xi direction
			//for periodic boundary, do three sheets -1, 0, 1.
			//remember that numPeriods=1;
     else numPeriod=0; 
     for (m=0;m<numOfILineSources;m++){
       if (fabs(powOfILineSources(m))>1.0e-5){
         for (k=-numPeriod;k<=numPeriod;k++){
             ptmp(I,J,K,0)=(IJKSrc(I,J,K,0)-(locOfILineSources(m)+k*ib))*di; // ptmp stores ksi - ksi_i + ...
	     ptmp(I,J,K,1)=sign(ptmp(I,J,K,0),1);
	     if ((locOfILineSources(m)>=indexstart[0])&&
		 (numPeriod==0)) ptmp(locOfILineSources(m),J,K,1)=0.0;
	     ptmp(I,J,K,2) -= powOfILineSources(m)*ptmp(I,J,K,1)*
			      exp(-difOfILineSources(m)*fabs(ptmp(I,J,K,0)));
         }
       }
     }

     for (m=0;m<numOfPointSources;m++){
       if (fabs(powOfPointSources(m))>1.0e-5){
         for (k=-numPeriod;k<=numPeriod;k++){
	     ptmp(I,J,K,0)=(IJKSrc(I,J,K,0)-(locOfPointSources(0,m)+k*ib))*di;
	     ptmp(I,J,K,1)=(IJKSrc(I,J,K,1)-locOfPointSources(1,m))*dj;
	     ptmp(I,J,K,3)=sign(ptmp(I,J,K,0),1);
             if ((locOfPointSources(0,m)>=indexstart[0])&&
		  (numPeriod==0)) ptmp(locOfPointSources(0,m),J,K,3)=0.0;
	     ptmp(I,J,K,2) -= powOfPointSources(m)*ptmp(I,J,K,3)*
		              exp(-difOfPointSources(m)*sqrt(ptmp(I,J,K,0)*ptmp(I,J,K,0)+
							     ptmp(I,J,K,1)*ptmp(I,J,K,1)));
         }
       }
     }
   }
  ptmp(I,J,K,2) += pInterpolate(I,J,K,0);
  Src(I,J,K,0) = ptmp(I,J,K,2);

  ptmp = 0.0;
  if ((fabs(powOfJLineSources(0))>1.0e-5)||
      (fabs(powOfPointSources(0))>1.0e-5))  {

      for (k=indexstart[2];k<=indexend[2];k+=indexstride[2]){
	for (j=indexstart[1];j<=indexend[1];j+=indexstride[1])
           IJKSrc(I,j,k,0).seqAdd(indexstart[0],indexstride[0]);
        for (i=indexstart[0];i<=indexend[0];i+=indexstride[0])
	   IJKSrc(i,J,k,1).seqAdd(indexstart[1],indexstride[1]);
     }

      if ((gridBc(0,1)==-1)||(grid(1,1)==-1)) numPeriod=numOfPeriods;
      else numPeriod=0;
      for (m=0;m<numOfJLineSources;m++){
	if (fabs(powOfJLineSources(m))>1.0e-5){
          for (k=-numPeriod;k<=numPeriod;k++){
	    ptmp(I,J,K,0) = (IJKSrc(I,J,K,1)-(locOfJLineSources(m)+k*jb))*dj;
	    ptmp(I,J,K,1) = sign(ptmp(I,J,K,0),1);
	    if ((locOfJLineSources(m)>=indexstart[1]) && (numPeriod==0))
		    ptmp(I,locOfJLineSources(m),K,1)=0.0;
	    ptmp(I,J,K,2) -= powOfJLineSources(m)*ptmp(I,J,K,1)*
		             exp(-difOfJLineSources(m)*fabs(ptmp(I,J,K,0)));
          }
        }
      }

      for (m=0;m<numOfPointSources;m++){
	if (fabs(powOfPointSources(m))>1.0e-5){
          for (k=-numPeriod;k<=numPeriod;k++){
	    ptmp(I,J,K,0)=(IJKSrc(I,J,K,0)-locOfPointSources(0,m))*di;
	    ptmp(I,J,K,1)=(IJKSrc(I,J,K,1)-(locOfPointSources(1,m)+k*jb))*dj;
	    ptmp(I,J,K,3)=sign(ptmp(I,J,K),1);
	    if ((locOfJLineSources(m)>=indexstart[1])&&(numPeriod==0))
		   ptmp(I,locOfPointSources(1,m),K,3)=0.0;
	    ptmp(I,J,K,2) -= powOfPointSources(m)*ptmp(I,J,K,3)*
		          exp(-difOfPointSources(m)*sqrt(ptmp(I,J,K,0)*ptmp(I,J,K,0)+
							 ptmp(I,J,K,1)*ptmp(I,J,K,1)));
          }
        }
      }
  }
 ptmp(I,J,K,2) += pInterpolate(I,J,K,1);
 Src(I,J,K,1) = ptmp(I,J,K,2);
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int EllipticTransform::
update( MappingInformation & mapInfo ) 
{

  Mapping *previousMapPointer;
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "transform which mapping?",
      "elliptic smoothing",
      "change resolution for elliptic grid",
      "set Poisson i-line sources",
      "set Poisson j-line sources",
      "set Poisson point sources",
      "set GRID boundary conditions",
      "set source interpolation coefficient",
      "set number of periods (for sourced problems)",
      "project onto original mapping (toggle)",
      "reset elliptic transform",
      "set order of interpolation",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Transform a Mapping by Elliptic Grid Generation",
      "transform which mapping? : 		choose the mapping to transform",
      "elliptic smoothing : 			smooth out grid with elliptic transform",
      "change resolution for elliptic grid:	change iDim,jDim for elliptic solver",
      "set Poisson i-line sources: 		set line sources for constant i",
      "set Poisson j-line sources: 		set line sources for constant j",
      "set Poisson point sources: 		set point sources in field",
      "set GRID boundary conditions: 		set b.c's for elliptic solver",
      "set source interpolation coefficient     set lambda for interpolation of B. sources terms",
      "set number of periods: 			make sources periodic",
      "project onto original mapping (toggle)",
      "reset elliptic transform                 start iterations from scratch",
      "set order of interpolation               order of interpolation for data point mapping",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 
  bool plotObject=FALSE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  bool mappingChosen= userMap!=NULL;		
  int k;

  // By default transform the last mapping in the list (if this mapping is uninitialized, mappingChosen==FALSE)
  if( !mappingChosen )
  {
    mappingHasChanged();
    int number= mapInfo.mappingList.getLength();
    for( int i=number-2; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( (mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==2) ||
          (mapPointer->getDomainDimension()==3 && mapPointer->getRangeDimension()==3) )
      {
        userMap=mapPointer;   // use this one
	previousMapPointer=mapPointer;
        mappingChosen=TRUE;
        setup();
  	initialize();		
        mappingHasChanged();
        plotObject=TRUE;
	break; 
      }
    }
  }
  if( !mappingChosen )
  {
    cout << "EllipticTransfrom:ERROR: no mappings to transform!! \n";
    return 1;
  }
mulgrid.ndimension=rangeDimension;
mulgrid.Maxiter=10;
mulgrid.smoothingMethod=1;
if (numDim==1) mulgrid.omega=2.0/3.0;
else if (numDim==2) mulgrid.omega=4.0/5.0;
else mulgrid.omega=0.5;
mulgrid.omega1=0.1;
mulgrid.lambda=0.0;
mulgrid.useBlockTridiag=0;
mulgrid.nlevel=0;
int idimtmp, jdimtmp, nlevelmax;
idimtmp=(userMap->getGridDimensions(axis1))-1;
iDim=idimtmp+1;
jdimtmp=(userMap->getGridDimensions(axis2))-1;
jDim=jdimtmp+1;
while (((idimtmp%2)==0)&&((jdimtmp%2)==0)) {
   (mulgrid.nlevel)++;
   idimtmp /= 2, jdimtmp /=2;
}
if ((idimtmp==1)||(jdimtmp==1)) (mulgrid.nlevel)--;
if (idimtmp>jdimtmp) idimtmp=jdimtmp;
nlevelmax=mulgrid.nlevel;
mulgrid.uprev=new realArray[nlevelmax];
for (int itmp=0;itmp<nlevelmax;itmp++){
 (mulgrid.uprev)[itmp].redim(Range(-1,(iDim/(int(pow(2,itmp))))+1),
		Range(-1,(jDim/(int(pow(2,itmp))))+1),
		Range(0,0), numDim);
 (mulgrid.uprev)[itmp]=0.0;
}

{
 realArray r;
 real a;
 Index I1, I2, I3;
 int i, j, k;

 I1=Range(-1, iDim+1);
 I2=Range(-1, jDim+1);
 I3=Range(0,0);
 r.redim(I1,I2,I3,numDim);

 a=1.0/(real(userMap->getGridDimensions(axis1)-1));
 for (k=I3.getBase();k<=I3.getBound(); k += I3.getStride())
   for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
     r(I1,j,k,0).seqAdd(-a,a);
 
   if (rangeDimension>1){
     a=1.0/(real(userMap->getGridDimensions(axis2)-1));
     for (k=I3.getBase();k<=I3.getBound();k+=I3.getStride())
       for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
         r(i,I2,k,1).seqAdd(-a,a);
   }
 
   if (rangeDimension>2){
     a=1.0/(real(userMap->getGridDimensions(axis3)-1));
     for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
       for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
         r(i,j,I3,2).seqAdd(-a,a);
   }
 
   // Initialize the grid function u0 to the range (Image)
   // of the userMap
   userMap->mapGrid(r,(mulgrid.uprev)[0]);
}


//This section is the part dealing with menu choices.
  gi.appendToTheDefaultPrompt("EllipticTransform>"); // set the default prompt

  for( int it=0;; it++ )
  {

    if( it==0 && plotObject )
      answer="plotObject";  // plot first time through
    else
      gi.getMenuItem(menu,answer);
    
 
    if( answer=="transform which mapping?" )
    { 
      // make a list of all potential Mappings:
    
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
        if( ((map.getDomainDimension()==2 && map.getRangeDimension()==2) ||
             (map.getDomainDimension()==3 && map.getRangeDimension()==3)) &&
             map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;

      if( mapNumber<0 )
        gi.outputString("Error: unknown mapping to transform!");
      else
      {
        mapNumber=subListNumbering(mapNumber);
	if( mapInfo.mappingList[mapNumber].mapPointer==this )
	{
	  cout << "EllipticTransform::ERROR: you cannot transform this mapping, "
	    "this would be recursive!\n";
	  continue;
      	}
      
	// define the mappings to be smoothed:

	userMap = mapInfo.mappingList[mapNumber].mapPointer;
        previousMapPointer=userMap;
        setup();
	
        mappingHasChanged();
	mappingChosen=TRUE;
        plotObject=TRUE;
      }
    }
    else if (it ==0) previousMapPointer=userMap;
    else if( answer=="elliptic smoothing" ) 
    {
      if (!mappingChosen) 
	 gi.outputString("Error: Mapping not yet chosen.");
      else 
      {
	aString menuSmooth[] =
	  {
	   "maximum number of V-cycles",
	   ">smoothing method",
	    "Jacobi",
	    "Red Black",
	    "Line Solver",
	    "Zebra",
	   "<number of multigrid levels",
	   "Jacobi under-relax coeff",
	   "use block tridiagonal (toggle)",
	   "source under-relax coeff",
	   "source interpolation power",
	   "start smoothing",
	   "help",
	   "exit",
	   ""
          };
        aString helpSmooth[] =
	  {
	   "     Smooth a mapping using Elliptic ",
	   "     Grid Generation and multigrid   ",
	   "maximum number of V-cycles:    Total number of multigrid iterations",
	   "smoothing method:              Choose a smoothing strategy",
	   "number of multigrid levels:    Choose the depth of a multigrid V-cycle",
	   "Jacobi under-relax coeff:  The underelaxation coefficient for the jacobi method",
	   "use block tridiagonal (toggle): Select the block tridiagonal solver\n",
	   "source under-relax coeff:      set the under relaxation coefficient\n                               for source terms when there are combined\n                               boundary conditions",
           "source interpolation power:    set the exponential coefficient to dump\n                               the boundary values",
	   "start smoothing:               call multigrid to do the smoothing",
	   "help:                          To display this help message",
	   "exit:                          To leave this menu",
	   ""
	  };
    
        gi.appendToTheDefaultPrompt("Multigrid>");
	for(int ism=0;;ism++){
	 gi.getMenuItem(menuSmooth,answer);
	 if (answer=="maximum number of V-cycles"){
	  gi.inputString(line,sPrintF(buff,"Enter the max # of V-cycl (default=%i): ",mulgrid.Maxiter));
          if (line != ""){
	   sscanf(line,"%d",&(mulgrid.Maxiter));
	  }
	 }
	 else if (answer == "number of multigrid levels"){
	  gi.inputString(line,sPrintF(buff,"Enter the # of levels (default=%i): ",mulgrid.nlevel));
	  if (line != ""){
	   sscanf(line,"%d",&(mulgrid.nlevel));
	   if ((mulgrid.nlevel)>nlevelmax){
	    gi.outputString("Error:: Too big number. Using the maximum");
	    (mulgrid.nlevel)=nlevelmax;
	   }
          }
         }
	 else if ((answer == "Jacobi")||(answer == "Red Black")||
		  (answer == "Line Solver")||(answer=="Zebra")){
	  if ((answer=="Zebra")||(answer=="Red Black")){
	   if (((mulgrid.nlevel)==nlevelmax)&&(idimtmp != 1)){
	    gi.outputString("Cannot use Zebra or RedBlack.");
	    gi.outputString("Using Jacobi method.");
	    answer="Jacobi", (mulgrid.omega)=4.0/5.0;
	   }
	  }
	  if (answer!="Jacobi") (mulgrid.omega)=1.0;
	  else (mulgrid.omega)=4./5.;
	  if ((answer=="Jacobi")||(answer=="Red Black")||
	      (answer=="Line Solver")||(answer=="Zebra")){
	      if (answer=="Jacobi") (mulgrid.smoothingMethod)=1;
	      if (answer=="Red Black") (mulgrid.smoothingMethod)=2;
	      if (answer=="Line Solver") (mulgrid.smoothingMethod)=3;
	      if (answer=="Zebra") (mulgrid.smoothingMethod)=4;
          }
         }
	 else if (answer=="Jacobi under-relax coeff"){
	   gi.inputString(line,sPrintF(buff,"Enter Jacobi under-relax coeff (default=%f): ",mulgrid.omega));
	   if (line != "") sScanF(line,"%f", &(mulgrid.omega));
	  }
	 else if (answer=="use block tridiagonal (toggle)"){
	   if ((mulgrid.useBlockTridiag)==0){
	     mulgrid.useBlockTridiag=1;
	     if ((mulgrid.smoothingMethod==3)||(mulgrid.smoothingMethod==4))
		   printf(" Will use Block Tridiagonal solver\n");
             }
	     else {
		 mulgrid.useBlockTridiag=0;
		 if ((mulgrid.smoothingMethod==3)||
		     (mulgrid.smoothingMethod==4))
		   printf(" Will not use Block Tridiagonal solver\n");
	     }
	 }
	 else if (answer == "source under-relax coeff"){
	    gi.inputString(line,sPrintF(buff,"Enter under-relax coeff for source terms (default=%f): ",mulgrid.omega1));
	    if (line != ""){
	     sScanF(line,"%f", &(mulgrid.omega1));
            }
	 }
	 else if (answer == "source interpolation power"){
	    gi.inputString(line,sPrintF(buff,"Enter interpolation exponent (default=%f): ",mulgrid.lambda));
	    if (line != "")
	     sScanF(line,"%f", &(mulgrid.lambda));
	 }
	 else if (answer == "start smoothing"){
	  generateGrid(&gi,parameters);
	  gi.unAppendTheDefaultPrompt();
  //printf("\n Apres GENERATEGRID iDim=%i\t jDim=%i\n\n",iDim,jDim);
          mappingChosen=TRUE;
          mappingHasChanged();
          plotObject=TRUE;
	  break;
         }
	 else if (answer=="exit"){
	  gi.unAppendTheDefaultPrompt();
          plotObject=TRUE;
	  break;
	 }
	 else if (answer=="help"){
          for( int i=0; helpSmooth[i]!=""; i++ )
            gi.outputString(helpSmooth[i]);
	 }
	}
      }
      mappingHasChanged();
    }
    else if( answer=="change resolution for elliptic grid")
    {
      /**********
      answer="lines";
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo);
      mapInfo.commandOption=MappingInformation::interactive;
      ************/
      gi.inputString(line,sPrintF(buff,"Enter i-dimension,j-dimension (default=(%d,%d)): ",
		     iDim,jDim));
      if (line!="")	
      {
      	sscanf(line,"%d %d",&iDim,&jDim);
	userMap->setGridDimensions(axis1, iDim);
	userMap->setGridDimensions(axis2, jDim);
        idimtmp=iDim-1;
        jdimtmp=jDim-1;
        while (((idimtmp%2)==0)&&((jdimtmp%2)==0)) {
           (mulgrid.nlevel)++;
           idimtmp /= 2, jdimtmp /=2;
        }
        if ((idimtmp==1)||(jdimtmp==1)) (mulgrid.nlevel)--;
        if (idimtmp>jdimtmp) idimtmp=jdimtmp;
        nlevelmax=mulgrid.nlevel;
        
        delete[] mulgrid.uprev;

        mulgrid.uprev = new realArray[nlevelmax];
        for (int itmp=0;itmp<nlevelmax;itmp++){
         (mulgrid.uprev)[itmp].redim(Range(-1,((iDim-1)/(int(pow(2,itmp))))+1),
		Range(-1,((jDim-1)/(int(pow(2,itmp))))+1),
		Range(0,0), numDim);
         (mulgrid.uprev)[itmp]=0.0;
        }

        {
         realArray r;
         real a;
         Index I1, I2, I3;
         int i, j, k;

         I1=Range(-1, iDim);
         I2=Range(-1, jDim);
         I3=Range(0,0);

         r.redim(I1,I2,I3,numDim);
         a=1.0/(real(userMap->getGridDimensions(axis1)-1));
         for (k=I3.getBase();k<=I3.getBound(); k += I3.getStride())
           for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
             r(I1,j,k,0).seqAdd(-a,a);
 
           if (rangeDimension>1){
             a=1.0/(real(userMap->getGridDimensions(axis2)-1));
             for (k=I3.getBase();k<=I3.getBound();k+=I3.getStride())
               for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
                 r(i,I2,k,1).seqAdd(-a,a);
           }
 
           if (rangeDimension>2){
             a=1.0/(real(userMap->getGridDimensions(axis3)-1));
             for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
               for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
                 r(i,j,I3,2).seqAdd(-a,a);
           }
 
           // Initialize the grid function u0 to the range (Image)
           // of the userMap
           userMap->mapGrid(r,(mulgrid.uprev)[0]);
        }


	setup();
	mappingHasChanged();
	mappingChosen=TRUE;
	plotObject=TRUE;
      }
    }
    else if( answer=="set GRID boundary conditions")
    {
      gi.outputString("Enter Boundary conditions:  (bc==1-->Dirichlet, bc==2-->Normal");
      gi.outputString("                             bc==3-->Combined, bc==-1-->Periodic");
      gi.inputString(line,sPrintF(buff,"Enter bc(0,0), bc(1,0), bc(0,1), bc(1,1)"
		     "(default = (%d,%d,%d,%d)): ",gridBc(0,0),gridBc(1,0),gridBc(0,1),gridBc(1,1)));
      if( line!="" ) 
	sscanf( line,"%d %d %d %d",&gridBc(0,0),&gridBc(1,0),&gridBc(0,1),&gridBc(1,1));
      for (int i=0;i<numDim;i++)
       for (int j=0;j<numDim;j++)
	if (gridBc(i,j)==3)	{
	  gi.inputString(line,sPrintF(buff,"Enter thickness of boundary layer dS(%d,%d) "
     			 "[default = %e] :",i,j,dS(i,j)));
	  if ( line!="" ){
	    sScanF( line,"%e",&dS(i,j));

	    if (j==0)
	    	dB(i,j)=dS(i,j)/di;
	    else
		dB(i,j)=dS(i,j)/dj;
	  }
        }
    }
    else if ( answer == "set source interpolation coefficient"){
      gi.inputString(line, sPrintF(buff,"Enter the value of lambda  (default = %g)",lambda));
      if ( line != "")
        sScanF( line,"%f",&lambda);
    }
    else if( answer=="set Poisson i-line sources")
    {
      gi.inputString(line,sPrintF(buff,"Enter number of source LINES (default=(%d)): ",
		     srcDefault));
      if (line != "")
      {
      
	sscanf(line,"%d",&numOfILineSources);
	powOfILineSources.redim(numOfILineSources);
	difOfILineSources.redim(numOfILineSources);
	locOfILineSources.redim(numOfILineSources);

	for (int n=0;n<numOfILineSources;n++)	       
	{
	  gi.inputString(line,sPrintF(buff,"Enter POWER of source %d",n));
	  if (line!="")	
	  {
	    sScanF(line,"%e",&powOfILineSources(n));
	    gi.inputString(line,sPrintF(buff,"Enter DIFFUSIVITY of source %d",n));
	    if (line!="")		
	    {
	      sScanF(line,"%e",&difOfILineSources(n));
	      gi.inputString(line,sPrintF(buff,"Enter i-LOCATION of Line Source %d",n));
	      if (line!="")               
	      {
		sscanf(line,"%d",&locOfILineSources(n));
	      }
	    }
	  }
	}
      }
    }    
    else if( answer=="set Poisson j-line sources")
    {
      gi.inputString(line,sPrintF(buff,"Enter number of source LINES (default=(%d)): ",
		     srcDefault));
      if (line != "")
      {
      
	sscanf(line,"%d",&numOfJLineSources);
	powOfJLineSources.redim(numOfJLineSources);
	difOfJLineSources.redim(numOfJLineSources);
	locOfJLineSources.redim(numOfJLineSources);

	for (int n=0;n<numOfJLineSources;n++)		
	{
	  gi.inputString(line,sPrintF(buff,"Enter POWER of source %d",n));
	  if (line!="")		
	  {
	    sScanF(line,"%e",&powOfJLineSources(n));
	    gi.inputString(line,sPrintF(buff,"Enter DIFFUSIVITY of source %d",n));
	    if (line!="")		
	    {
	      sScanF(line,"%e",&difOfJLineSources(n));
	      gi.inputString(line,sPrintF(buff,"Enter j-LOCATION of Line Source %d",n));
	      if (line!="")            
	      {
		sscanf(line,"%d",&locOfJLineSources(n));
	      }
	    }
	  }
	}
      }
    }
    else if( answer=="set Poisson point sources")
    {
      gi.inputString(line,sPrintF(buff,"Enter number of POINT sources (default=(%d)): ",
		     srcDefault));
      if (line != "")
      {
	sscanf(line,"%d",&numOfPointSources);
	powOfPointSources.redim(numOfPointSources);
	difOfPointSources.redim(numOfPointSources);
	locOfPointSources.redim(2,numOfPointSources);
	for (int n=0;n<numOfPointSources;n++)		
	{
	  gi.inputString(line,sPrintF(buff,"Enter POWER of source %d",n));
	  if (line!="")		
	  {
	    sScanF(line,"%e",&powOfPointSources(n));
	    gi.inputString(line,sPrintF(buff,"Enter DIFFUSIVITY of source %d",n));
	    if (line!="")		
	    {
	      sScanF(line,"%e",&difOfPointSources(n));
	      gi.inputString(line,sPrintF(buff,"Enter i-LOCATION and j-LOCATION of Point Source %d",n));
	      if (line!="")               
	      {
		sscanf(line,"%d %d",&locOfPointSources(0,n),&locOfPointSources(1,n));
	      }
	    }
	  }
	}
      }
    }
    else if( answer=="set number of periods (for sourced problems)")
    {
      gi.inputString(line,sPrintF(buff,"Enter number of periods for resolving"
		     " periodic problem with source (default==%d):",numOfPeriods));
      if (line!="")
	 sscanf(line,"%d",&numOfPeriods);
      if (numOfPeriods%2==0) 
	 numOfPeriods=numOfPeriods+1;
    }
    else if( answer=="project onto original mapping (toggle)" )
    {
      project=!project;
      if( project )
      {
	dpm->setRangeSpace(parameterSpace);
        if( compose==NULL )
          compose = new ComposeMapping;
        compose->setMappings(*dpm,*userMap);
	gi.outputString("the elliptic transform will be projected onto the original mapping");
      }
      else    
      {
	dpm->setRangeSpace(cartesianSpace);
        gi.outputString("the elliptic transform will NOT be projected onto the original mapping");
      }
    }
    else if( answer=="reset elliptic transform" )
    {
      xe.redim(0);   // this will reset the iterations to scratch
         userMap=previousMapPointer;
         //printf("iDim=%i\t thisiiDim=%i\n",iDim,userMap->getGridDimensions(axis1));
	 setup();
	
         //mappingHasChanged();
      mappingChosen=TRUE;
      mappingHasChanged();
      plotObject=TRUE;
      //ellipticGridDefined=TRUE;
    }
    else if( answer=="set order of interpolation" )
    {
      aString menu2[] = { "2nd order",
                         "4th order",
                         "no change", 
                         "" };
      int response=gi.getMenuItem(menu2,answer2);
      if( response>=0 && response<=1 )
      { 
	dpm->setOrderOfInterpolation(2+response*2);   // 2 or 4
      }
    }
    else if( answer=="show parameters" )
    {
      //don't run this until the thing is set!!
      printf("(i-dimension,j-dimension)=(%d,%d)\n",iDim,jDim);
      printf(" MaxIterations = %d \n",maxIter);
      for (k=0;k<numOfILineSources;k++)
	printf(" i-line source %d:\t%7.3e\t%7.5e\t%d\n",k,powOfILineSources(k),
		difOfILineSources(k),locOfILineSources(k));
      for (k=0;k<numOfILineSources;k++)
	printf(" j-line source %d:\t%7.3e\t%7.5e\t%d\n",k,powOfJLineSources(k),
		difOfJLineSources(k),locOfJLineSources(k));
      for (k=0;k<numOfPointSources;k++)
	printf(" point source %d:\t%7.3e\t%7.5e\t%d\t%d\n",k,powOfPointSources(k),
		difOfPointSources(k),locOfPointSources(0,k),locOfPointSources(1,k));
      printf(" GRID boundary conditions:\t%d\t%d\t%d\t%d\n",gridBc(0,0),gridBc(1,0),
		gridBc(0,1),gridBc(1,1));
      printf(" number of periods:\t%d\n",numOfPeriods);
      display();
    }
    else if( answer=="plot" )
    {
      if( !mappingChosen )
      {
	gi.outputString("you must first choose a mapping to transform");
	continue;
      }
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo);
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject && mappingChosen )
    {
      gi.erase();
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      PlotIt::plot(gi,*this,parameters);  
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

