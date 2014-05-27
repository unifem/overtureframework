#include "DerivedFunctions.h"
#include "CompositeGridOperators.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


//\begin{>DerivedFunctionsInclude.tex}{\subsection{constructors}} 
DerivedFunctions::
DerivedFunctions()
// ==================================================================================
// /Description:
// Derive new functions from old ones. This class is used by plotStuff to
// create derived quantities such as vorticity, mach-number, derivatives etc.
//
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  initialize();
}

//\begin{>DerivedFunctionsInclude.tex}{\subsection{constructors}} 
DerivedFunctions::
DerivedFunctions(ShowFileReader & showFileReader_ )
// ==================================================================================
// /Description:
// Derive new functions from old ones. This class is used by plotStuff to
// create derived quantities such as vorticity, mach-number, derivatives etc.
//
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  initialize();
  showFileReader=&showFileReader_;
}

void DerivedFunctions::
initialize()
{
  showFileReader=NULL;
  numberOfDerivedFunctions=0;
  name=NULL;

  // Schlieren parameters
  exposure=1.;
  amplification=15.;

  for( int i=0; i<3; i++ )
  {
    velocityComponent[i]=-2;
    displacementComponent[i]=-2;
  }
  
  for( int i=0; i<9; i++ )
    stressComponent[i]=-2;

}


DerivedFunctions::
~DerivedFunctions()
{
  delete [] name;
}

// ==================================================================================
/// \brief Supply a ShowFileReader 
// ==================================================================================
void DerivedFunctions::
set( ShowFileReader & showFileReader_, GraphicsParameters *pgp /* =NULL */ )
{
 showFileReader=&showFileReader_; 

 // Note: the number of frames is zero if the showFile is really just a grid file with no solutions.
 if( showFileReader->getNumberOfFrames()>0 && displacementComponent[0]<0 )
 { // initialize any velocity and displacement components
   int v1c,v2c,v3c;
   getVelocityComponents(v1c,v2c,v3c);
   int u1c,u2c,u3c;
   getDisplacementComponents(u1c,u2c,u3c);
   printF("DerivedFunctions:: (v1c,v2c,v3c)=(%i,%i,%i) (u1c,u2c,u3c)=(%i,%i,%i)\n",v1c,v2c,v3c,u1c,u2c,u3c);
   if( pgp!=NULL )
   {
     pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,v1c);
     pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,v2c);
     pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,v3c);

     pgp->set(GI_DISPLACEMENT_U_COMPONENT,u1c);
     pgp->set(GI_DISPLACEMENT_V_COMPONENT,u2c);
     pgp->set(GI_DISPLACEMENT_W_COMPONENT,u3c);
   }
 }
 
}


//\begin{>>DerivedFunctionsInclude.tex}{\subsection{getASolution}} 
int DerivedFunctions:: 
getASolution(int & solutionNumber,
	     MappedGrid & cg,
	     realMappedGridFunction & u)
// ==================================================================================
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  printF("DerivedFunctions::getASolution: sorry: not implemented for realMappedGridFunction's\n");
  return getASolution(solutionNumber,cg,u);
}


//\begin{>>DerivedFunctionsInclude.tex}{\subsection{getASolution}} 
int DerivedFunctions::
getASolution(int & solutionNumber,
	     CompositeGrid & cg,
	     realCompositeGridFunction & u)
// ==================================================================================
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  assert( showFileReader!=NULL );
  int returnValue;

  // ** should allocate space for components plus extra derived functions first ***
  //  == Need to fix HDF_DB::get( array ) not to redim if the array already has enough space ===

  // int nc = numberOfComponents + numberOfDerivedFunctions;

  returnValue=showFileReader->getASolution(solutionNumber,cg,u);
  
  // compute additional derived grid functions
  computeDerivedFunctions( u );

  return returnValue;
}

//\begin{>>DerivedFunctionsInclude.tex}{\subsection{getComponent}} 
int DerivedFunctions::
getComponent( int & c, const aString & cName )
// ==================================================================================
// /Access: protected.
// /Description:
//    Look for a component number in the data base.
// /c (input): c==-2 first time through. If not found c==-1 on output.
// /Return value: 0=success, 1=not found.
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  if( c==-2 )
  {
    if( showFileReader->getGeneralParameter(cName,c) )
    {
      return 0; // found
    }
    else
    {
      // old way
      HDF_DataBase & db = *showFileReader->getFrame();
      db.get(c,cName);
      if( c==-2 )
	c=-1;
    }
    
  }
  return c<0;
}

// ========================================================================================
/// \brief Lookup the velocity components.
// ========================================================================================
int DerivedFunctions::
getVelocityComponents( int &v1c, int & v2c, int & v3c )
{
  if( velocityComponent[0]<0 )
  {
    //  get velocity components - these can have different names for solid and fluid mechanics
    getComponent(velocityComponent[0],"v1Component");
    if( velocityComponent[0]<0 )
    {
      velocityComponent[0]=-2;  // look again for a new name
      getComponent(velocityComponent[0],"uComponent");
    }
    
    getComponent(velocityComponent[1],"v2Component");
    if( velocityComponent[1]<0 )
    {
      velocityComponent[1]=-2;  // look again for a new name
      getComponent(velocityComponent[1],"vComponent");
    }
    
    getComponent(velocityComponent[2],"v3Component");
    if( velocityComponent[2]<0 )
    {
      velocityComponent[2]=-2; // look again for a new name
      getComponent(velocityComponent[2],"wComponent");
    }
    printF("DerivedFunctions:getVelocityComponents using (%i,%i,%i) as velocity components.\n",
	   velocityComponent[0],velocityComponent[1],velocityComponent[2]);
  }
  v1c=velocityComponent[0];
  v2c=velocityComponent[1];
  v3c=velocityComponent[2];

  return 0;
}


// ========================================================================================
/// \brief Lookup the displacement components.
// ========================================================================================
int DerivedFunctions::
getDisplacementComponents( int &u1c, int & u2c, int & u3c )
{	
  if( displacementComponent[0]<0 )
  {
    //  displacement components
    getComponent(displacementComponent[0],"u1Component");
//  if( numberOfDimensions>1 )
      getComponent(displacementComponent[1],"u2Component");
//     if( numberOfDimensions>2 )
      getComponent(displacementComponent[2],"u3Component");
    // old: 
    //  getComponent(uc,"uComponent");
    //  if( numberOfDimensions>1 )
    //    getComponent(vc,"vComponent");
    //  if( numberOfDimensions>2 )
    //    getComponent(wc,"wComponent");
  }
  u1c=displacementComponent[0];
  u2c=displacementComponent[1];
  u3c=displacementComponent[2];

  return 0;
}



// ========================================================================================
/// \brief Lookup the stress components.
// ========================================================================================
int DerivedFunctions::
getStressComponents( int & s11c, int & s12c, int & s13c,
                     int & s21c, int & s22c, int & s23c,
                     int & s31c, int & s32c, int & s33c )
{
  if( stressComponent[0]<0 )
  {
    //  get stress components
    getComponent(stressComponent[0],"s11Component");
    getComponent(stressComponent[1],"s12Component");
//     if( numberOfDimensions>2 )
      getComponent(stressComponent[2],"s13Component");
    getComponent(stressComponent[3],"s21Component");
    getComponent(stressComponent[4],"s22Component");
//     if( numberOfDimensions>2 )
      getComponent(stressComponent[5],"s23Component");
//     if( numberOfDimensions>2 )
    {
      getComponent(stressComponent[6],"s31Component");
      getComponent(stressComponent[7],"s32Component");
      getComponent(stressComponent[8],"s33Component");
    }
  }
  s11c=stressComponent[0];
  s12c=stressComponent[1];
  s13c=stressComponent[2];

  s21c=stressComponent[3];
  s22c=stressComponent[4];
  s23c=stressComponent[5];

  s31c=stressComponent[6];
  s32c=stressComponent[7];
  s33c=stressComponent[8];

  return 0;
}



//\begin{>>DerivedFunctionsInclude.tex}{\subsection{computeDerivedFunctions}} 
int DerivedFunctions::
computeDerivedFunctions( realCompositeGridFunction & u )
// ==================================================================================
// /Access: protected.
// /Description:
//    This function will compute the derived types and add these as new components to 
//  the end of the grid function u. This function is normally called after reading a 
// new solution from the show file. It used the showFileReader pointer to get info
// about the current frame.
// /u (input/output): On input u should contain valid values for the default components.
//   On output u will be redimensioned to hold th extra derived components.
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  if( numberOfDerivedFunctions==0 )
    return 0;

  // const int nc=showFileReader->
  const int numberOfComponents=u.getComponentDimension(0);

  HDF_DataBase & db = *showFileReader->getFrame();
  int uc=-2,vc=-2,wc=-2,pc=-2,rc=-2,tc=-2;
  int isAxisymmetric=0;
  real Rg,gamma;
  
//   uc=1; db.get(uc,"uComponent");
//   vc=2; db.get(vc,"vComponent");
//   wc=3; db.get(wc,"wComponent");
//   pc=0; db.get(pc,"pComponent");
  

  // it looks like we need to make a copy of u -- maybe only grid by grid ?

  CompositeGrid & gc = *u.getCompositeGrid();
  const int numberOfDimensions = gc.numberOfDimensions();
  
  const int nc = numberOfComponents+numberOfDerivedFunctions;
  
  Range all;
  realCompositeGridFunction u0(gc,all,all,all,nc);
  
  Index I1,I2,I3;
  Range R=numberOfComponents;
  int grid;
  for( grid=0; grid<gc.numberOfGrids(); grid++ )
  {
    #ifdef USE_PPP
      realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0[grid],u0Local);
    #else
      const realSerialArray & uLocal = u[grid];
      realSerialArray & u0Local = u0[grid];
    #endif

    getIndex(gc[grid].dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3);
    if( ok )
    {
      u0Local(I1,I2,I3,R)=uLocal(I1,I2,I3,R);
    }
  }
  
  // *** u0 needs operators ****
  CompositeGridOperators cgop(gc);
  u0.setOperators(cgop);

  int i;
  for( i=0; i<numberOfComponents; i++ )
    u0.setName(u.getName(i),i);
  for( i=0; i<numberOfDerivedFunctions; i++ )
    u0.setName(name[i],numberOfComponents+i);


  bool interpolationRequired=false;
  
  for( grid=0; grid<gc.numberOfGrids(); grid++ )
  {
    realMappedGridFunction & vg = u0[grid];
    MappedGridOperators & op = cgop[grid];
    
    #ifdef USE_PPP
      realSerialArray v;  getLocalArrayWithGhostBoundaries(vg,v);
    #else
      const realSerialArray & v = vg;
    #endif


    // This next is needed if we take derivatives: *wdh* 2012/06/01
    vg.updateGhostBoundaries();


    getIndex(gc[grid].dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(vg,v,I1,I2,I3);
    if( !ok ) continue;

    for( i=0; i<numberOfDerivedFunctions; i++ )
    {
      int j=numberOfComponents+i;
      assert( derived(i,0)>=0 );

      if( derived(i,0)>=xDerivative && derived(i,0)<=laplaceDerivative )
      {
        interpolationRequired=true;
        realSerialArray vx(I1,I2,I3);
	vx=0.;
	
	int component=derived(i,1);
	assert( component>=0 && component<nc );
        switch( derived(i,0) )
	{

	case xDerivative:
          op.derivative(MappedGridOperators::xDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case yDerivative:
          op.derivative(MappedGridOperators::yDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case zDerivative:
          op.derivative(MappedGridOperators::zDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case xxDerivative:
          op.derivative(MappedGridOperators::xxDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case yyDerivative:
          op.derivative(MappedGridOperators::yyDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case zzDerivative:
          op.derivative(MappedGridOperators::zzDerivative,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
        case laplaceDerivative:
          op.derivative(MappedGridOperators::laplacianOperator,v,vx ,I1,I2,I3,component);
	  v(I1,I2,I3,j)=vx; 
          break;
	case gradientNorm:
          op.derivative(MappedGridOperators::xDerivative,v,vx ,I1,I2,I3,component);
          v(I1,I2,I3,j)=vx*vx;
	  if( numberOfDimensions>1 )
	  {
	    op.derivative(MappedGridOperators::yDerivative,v,vx ,I1,I2,I3,component);
	    v(I1,I2,I3,j)+=vx*vx;
	    if(numberOfDimensions>2 )
	    { 
	      op.derivative(MappedGridOperators::zDerivative,v,vx ,I1,I2,I3,component);
	      v(I1,I2,I3,j)+=vx*vx;
	    }
	  }
          v(I1,I2,I3,j)=sqrt(v(I1,I2,I3,j));
          break;
        default:
          printF("ERROR: unknown derivative\n");
	}
	
      }
      else if( derived(i,0)==logarithm )
      {
	int component=derived(i,1);
	assert( component>=0 && component<nc );

        v(I1,I2,I3,j)=log(v(I1,I2,I3,component));

      }
      else if( derived(i,0)==temperature )
      {
        bool foundRg = showFileReader->getGeneralParameter("Rg",Rg);

        if( !getComponent(tc,"temperatureComponent") || !foundRg )
	{
	  getComponent(rc,"densityComponent");
	  getComponent(pc,"pressureComponent");
	}
	  
	if( tc>= 0 )
	  v(I1,I2,I3,j)=v(I1,I2,I3,tc);          // ***** may have to compute ****
	else if( pc>=0 && rc>=0 )
	  v(I1,I2,I3,j)=v(I1,I2,I3,pc)/(v(I1,I2,I3,rc)*Rg);    
	else
	{
	  cout << "computeDerivedFunctions: unable to compute the temperature \n";
	  v(I1,I2,I3,j)=0.;
	}
      }
      else if( derived(i,0)==pressure )
      {
        bool foundRg = showFileReader->getGeneralParameter("Rg",Rg);

        if( !getComponent(pc,"pressureComponent") )
	{
	  getComponent(rc,"densityComponent");
	  getComponent(tc,"temperatureComponent");
	}
	  
	if( pc>= 0 )
	  v(I1,I2,I3,j)=v(I1,I2,I3,pc);          // ***** may have to compute ****
	else if( tc>=0 && rc>=0 && foundRg )
	  v(I1,I2,I3,j)=v(I1,I2,I3,tc)*v(I1,I2,I3,rc)*Rg;    
	else
	{
	  cout << "computeDerivedFunctions: unable to compute the pressure \n";
	  v(I1,I2,I3,j)=0.;
	}
      }
      else if( derived(i,0)==machNumber )
      {
	getVelocityComponents(uc,vc,wc);

	realSerialArray uSq;
        if( numberOfDimensions==1 ) 
          uSq=SQR(v(I1,I2,I3,uc));
	else if( numberOfDimensions==2 ) 
          uSq=SQR(v(I1,I2,I3,uc))+SQR(v(I1,I2,I3,vc));
	else
	  uSq=SQR(v(I1,I2,I3,uc))+SQR(v(I1,I2,I3,vc))+SQR(v(I1,I2,I3,wc));

        bool foundRg=showFileReader->getGeneralParameter("Rg",Rg);
	bool foundTemperature=getComponent(tc,"temperatureComponent");
        if( !foundTemperature || !foundRg )
	{
	  getComponent(rc,"densityComponent");
	  getComponent(pc,"pressureComponent");
	}
        gamma=-1.;
        showFileReader->getGeneralParameter("gamma",gamma);
        if( gamma<0. ) gamma=1.4;

	if( tc>= 0 )
	{
	  printF("DerivedFunctions: Using temperature, gamma=%9.3e and Rg=%9.3e to compute Mach Number\n",gamma,Rg);
	  v(I1,I2,I3,j)=sqrt(uSq/(gamma*Rg*v(I1,I2,I3,tc)));     
	}
	else if( pc>=0 && rc>=0 )
	{
          printF("DerivedFunctions: Using pressure, density, and gamma=%9.3e to compute Mach Number\n",gamma);
	  v(I1,I2,I3,j)=sqrt(uSq/(gamma*v(I1,I2,I3,pc)/v(I1,I2,I3,rc)));      // *wdh* 070511
	}
	else
	{
	  cout << "computeDerivedFunctions: unable to compute the Mach Number \n";
	  v(I1,I2,I3,j)=0.;
	}
      }
      else if( derived(i,0)==divergence )
      {
        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

	realSerialArray vx(I1,I2,I3);
        vx=0.;
	
	op.derivative(MappedGridOperators::xDerivative,v,vx ,I1,I2,I3,uc);
	v(I1,I2,I3,j)=vx;
	if( numberOfDimensions>1 )
	{
	  op.derivative(MappedGridOperators::yDerivative,v,vx ,I1,I2,I3,vc);
	  v(I1,I2,I3,j)+=vx;
	  if(numberOfDimensions>2 )
	  { 
	    op.derivative(MappedGridOperators::zDerivative,v,vx ,I1,I2,I3,wc);
	    v(I1,I2,I3,j)+=vx;
	  }
	}


        if( numberOfDimensions==2 )
	{ // add an axisymetric correction

          isAxisymmetric=0;
          // db.get(isAxisymmetric,"isAxisymmetric");
          showFileReader->getGeneralParameter("isAxisymmetric",isAxisymmetric);
	  if( isAxisymmetric==1 )
	  {
           #ifdef USE_PPP
            realSerialArray x; getLocalArrayWithGhostBoundaries(gc[grid].vertex(),x);
	   #else
            realSerialArray & x = gc[grid].vertex();
           #endif

            // div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
	    realSerialArray radiusInverse(I1,I2,I3);
            radiusInverse = 1./max(REAL_MIN,x(I1,I2,I3,axis2));
            Index Ib1,Ib2,Ib3;
            int axisymmetricBoundaryCondition=INT_MAX;
            // db.get(axisymmetricBoundaryCondition,"axisymmetricBoundaryCondition");
	    showFileReader->getGeneralParameter("axisymmetricBoundaryCondition",axisymmetricBoundaryCondition);
	    
            for( int axis=0; axis<numberOfDimensions; axis++ )
	    {
	      for( int side=0; side<=1; side++ )
	      {
		if( gc[grid].boundaryCondition(side,axis)==axisymmetricBoundaryCondition )
		{
		  getBoundaryIndex(gc[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  bool ok = ParallelUtility::getLocalArrayBounds(vg,v,Ib1,Ib2,Ib3);
		  if( !ok ) continue;

		  radiusInverse(Ib1,Ib2,Ib3)=0.;

       	          op.derivative(MappedGridOperators::yDerivative,v,vx ,Ib1,Ib2,Ib3,vc);
		  v(Ib1,Ib2,Ib3,i)+=vx;
		}
	      }
	    }
	    v(I1,I2,I3,j)+=v(I1,I2,I3,vc)*radiusInverse;
	  }
	}

      }
      else if( derived(i,0)==vorticity || derived(i,0)==zVorticity  )
      {
        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

        if( numberOfDimensions==1 )
          v(I1,I2,I3,j)=0.;
        else 
	{
	  realSerialArray vx(I1,I2,I3),uy(I1,I2,I3);
          vx=0.; 
          uy=0.;
	  op.derivative(MappedGridOperators::xDerivative,v,vx ,I1,I2,I3,vc);
	  op.derivative(MappedGridOperators::yDerivative,v,uy ,I1,I2,I3,uc);
  	  v(I1,I2,I3,j)=vx-uy;

  	  // v(I1,I2,I3,j)=v.x()(I1,I2,I3,vc)-v.y()(I1,I2,I3,uc); // flipped sign 030215
	}
	
      }
      else if( derived(i,0)==xVorticity )
      {
        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

	if( numberOfDimensions<3 )
	  v(I1,I2,I3,j)=0.;
	else
	{
	  realSerialArray vz(I1,I2,I3),wy(I1,I2,I3);
          vz=0.;
	  wy=0.;
	  op.derivative(MappedGridOperators::zDerivative,v,vz ,I1,I2,I3,vc);
	  op.derivative(MappedGridOperators::yDerivative,v,wy ,I1,I2,I3,wc);
  	  v(I1,I2,I3,j)=wy-vz;
	  // v(I1,I2,I3,j)=v.y()(I1,I2,I3,wc)-v.z()(I1,I2,I3,vc); // flipped sign 030215
	}
      }
      else if( derived(i,0)==yVorticity )
      {
        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

	if( numberOfDimensions<3 )
	  v(I1,I2,I3,j)=0.;
	else
	{
	  realSerialArray uz(I1,I2,I3),wx(I1,I2,I3);
          uz=0.;
	  wx=0.;
	  op.derivative(MappedGridOperators::zDerivative,v,uz ,I1,I2,I3,uc);
	  op.derivative(MappedGridOperators::xDerivative,v,wx ,I1,I2,I3,wc);
  	  v(I1,I2,I3,j)=uz-wx;

	  // v(I1,I2,I3,j)=v.z()(I1,I2,I3,uc)-v.x()(I1,I2,I3,wc); // flipped sign 030215
	}
      }
      else if( derived(i,0)==enstrophy  )
      {
        // Entrosphy : || vorticity || 

        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

	
        if( numberOfDimensions==1 )
          v(I1,I2,I3,j)=0.;
        else 
	{
          v(all,all,all,j)=0.; // *wdh* 2012/07/05

	  realSerialArray v1(I1,I2,I3),v2(I1,I2,I3);
          v1=0.;  // *wdh* 2012/07/05 -- this needed to avoid UMR's
	  v2=0.;
	  if( numberOfDimensions==2 )
	  {
	    op.derivative(MappedGridOperators::xDerivative,v,v1 ,I1,I2,I3,vc);
	    op.derivative(MappedGridOperators::yDerivative,v,v2 ,I1,I2,I3,uc);
	    v(I1,I2,I3,j) = fabs( v1-v2 ); // vx-uy 
	  }
	  else
	  {
	    op.derivative(MappedGridOperators::zDerivative,v,v1 ,I1,I2,I3,vc);
	    op.derivative(MappedGridOperators::yDerivative,v,v2 ,I1,I2,I3,wc);
	    v(I1,I2,I3,j) = SQR( v2-v1 );  // wy-vz 

	    op.derivative(MappedGridOperators::zDerivative,v,v1 ,I1,I2,I3,uc);
	    op.derivative(MappedGridOperators::xDerivative,v,v2 ,I1,I2,I3,wc);
	    v(I1,I2,I3,j) += SQR( v1-v2 );   // uz-wx 

	    op.derivative(MappedGridOperators::xDerivative,v,v1 ,I1,I2,I3,vc);
	    op.derivative(MappedGridOperators::yDerivative,v,v2 ,I1,I2,I3,uc);
	    v(I1,I2,I3,j) += SQR( v1-v2 ); // vx-uy 

	    v(I1,I2,I3,j) = sqrt(v(I1,I2,I3,j));
	  }
	  
  	  // v(I1,I2,I3,j)=v.x()(I1,I2,I3,vc)-v.y()(I1,I2,I3,uc); // flipped sign 030215
	}
	
      }
      else if( derived(i,0)==entropy )
      {
        // bool foundRg=db.get(Rg,"Rg");
        bool foundRg = showFileReader->getGeneralParameter("Rg",Rg);
	getComponent(rc,"densityComponent");
	bool foundTemperature=getComponent(tc,"temperatureComponent");
        if( !foundTemperature || !foundRg )
	{
	  getComponent(rc,"densityComponent");
	  getComponent(pc,"pressureComponent");
	}
        // db.get(gamma,"gamma");
        gamma=-1.;
        showFileReader->getGeneralParameter("gamma",gamma);
	if( gamma<0. ) gamma=1.4;
	
	if( tc>= 0 && rc>=0 )
	{
          if( grid==0 )
	    printF("DerivedFunctions: Using rho, temperature, gamma=%9.3e and Rg=%9.3e to compute entropy\n"
   //       "     really computing:  entropy/Cv = log( p/ rho^gamma ) +constant, with p=rho*Rg*T \n",gamma,Rg);
                   "     really computing:  p/rho^gamma  with p=rho*Rg*T \n",gamma,Rg);

	  // v(I1,I2,I3,j)=log( Rg*v(I1,I2,I3,tc)/pow(v(I1,I2,I3,rc),gamma-1.) );
	  v(I1,I2,I3,j)= Rg*v(I1,I2,I3,tc)/pow(v(I1,I2,I3,rc),gamma-1.);
	}
	else if( pc>=0 && rc>=0 )
	{
          if( grid==0 )
            printF("DerivedFunctions: Using pressure, density, and gamma=%9.3e to compute the entropy\n"
                   "     really computing:  entropy/Cv = log( p/ rho^gamma ) +constant \n",gamma);

	  v(I1,I2,I3,j)=log( v(I1,I2,I3,pc)/pow(v(I1,I2,I3,rc),gamma) );    
	}
	else
	{
	  cout << "computeDerivedFunctions: unable to compute the entropy \n";
	  v(I1,I2,I3,j)=0.;
	}
      }
      else if( derived(i,0)==speed )
      {
	getVelocityComponents(uc,vc,wc);
	
	if( numberOfDimensions==1 )
	  v(I1,I2,I3,j)=fabs(v(I1,I2,I3,uc));
	else if( numberOfDimensions==2 )
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,uc))+SQR(v(I1,I2,I3,vc)) );
	else
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,uc))+SQR(v(I1,I2,I3,vc))+SQR(v(I1,I2,I3,wc)) );
      }
      else if( derived(i,0)==displacementNorm )
      {
        int u1c,u2c,u3c;
        getDisplacementComponents( u1c,u2c,u3c );

	if( numberOfDimensions==1 )
	  v(I1,I2,I3,j)=fabs(v(I1,I2,I3,u1c));
	else if( numberOfDimensions==2 )
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,u1c))+SQR(v(I1,I2,I3,u2c)) );
	else
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,u1c))+SQR(v(I1,I2,I3,u2c))+SQR(v(I1,I2,I3,u3c)) );
      }
      else if( derived(i,0)==stressNorm )
      {
        // Compute the norm of the stress (Cauchy stress)

#ifdef USE_PPP
	  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(gc[grid].mask(),maskLocal);
	 
#else
	  const intSerialArray & maskLocal = gc[grid].mask();
#endif

	  const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	  const int maskDim0=maskLocal.getRawDataSize(0);
	  const int maskDim1=maskLocal.getRawDataSize(1);
	  const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

	  real *vp = v.Array_Descriptor.Array_View_Pointer3;
	  const int vDim0=v.getRawDataSize(0);
	  const int vDim1=v.getRawDataSize(1);
	  const int vDim2=v.getRawDataSize(2);
#undef V
#define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]

	real s11,s12,s13,s22,s23,s33;

	int s11c, s12c, s13c, s21c,s22c,s23c, s31c,s32c,s33c;
	getStressComponents( s11c, s12c, s13c,
			     s21c, s22c, s23c,
			     s31c, s32c, s33c );

//         // First look to see if the stress is already there
// 	int s11c=-1, s12c=-1, s13c=-1;
// 	int          s22c=-1, s23c=-1;
// 	int                   s33c=-1;
//         showFileReader->getGeneralParameter("s11Component",s11c);
//         showFileReader->getGeneralParameter("s12Component",s12c);
//         showFileReader->getGeneralParameter("s13Component",s13c);
//         showFileReader->getGeneralParameter("s22Component",s22c);
//         showFileReader->getGeneralParameter("s23Component",s23c);
//         showFileReader->getGeneralParameter("s33Component",s33c);

	printF("DerivedFunctions: s11c=%i, s12c=%i, s22c=%i\n",s11c,s12c,s22c);

	if( (gc.numberOfDimensions()==2 && s11c>=0 && s12c>=0 && s22c>=0 )  ||
            (gc.numberOfDimensions()==3 && s11c>=0 && s12c>=0 && s22c>=0 && s13c>=0 && s23c>=0 && s33c>=0 ) )
	{
	  // --- We can compute the stress norm directly from the stress components ---
	  printF("DerivedFunctions: stressNorm: using stress from showfile s11c=%i, s12c=%i, ...\n",s11c,s12c);

	  int i1,i2,i3;
	  if( gc.numberOfDimensions()==2 )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3) != 0 )
	      { 
		s11 = V(i1,i2,i3,s11c);
		s12 = V(i1,i2,i3,s12c);
		s22 = V(i1,i2,i3,s22c);
		V(i1,i2,i3,j)= sqrt( SQR(s11) + 2.*SQR(s12) + SQR(s22) );
	      }
	    }
	  }
	  else
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3) != 0 )
	      { 
		s11 = V(i1,i2,i3,s11c);
		s12 = V(i1,i2,i3,s12c);
		s13 = V(i1,i2,i3,s13c);
		s22 = V(i1,i2,i3,s22c);
		s23 = V(i1,i2,i3,s23c);
		s33 = V(i1,i2,i3,s33c);

		V(i1,i2,i3,j)=sqrt( SQR(s11)+ 2.*SQR(s12) + 2.*SQR(s13) + SQR(s22) + 2.*SQR(s23) + SQR(s33) );
	      }
	    }
	  }


	}
	else
	{
	  // ---We need to compute the stressNorm by first computing the stress from u ----

	  interpolationRequired=true;
	
	  int u1c,u2c,u3c;
	  getDisplacementComponents( u1c,u2c,u3c );
	
	  real lambda=-1., mu=-1.;
	  showFileReader->getGeneralParameter("lambda",lambda);
	  showFileReader->getGeneralParameter("mu",mu);
	  if( lambda<0. || mu<0. )
	  {
	    printF("DerivedFunctions: stresNorm:ERROR: lambda and/or mu not found in the show file!\n");
	  }
	
	  if( lambda<0. ) lambda=1.;
	  if( mu<0. ) mu=1.;

	  printF("DerivedFunctions: stressNorm: using displacement components [%i,%i,%i] lambda=%8.2e, mu=%8.2e\n",u1c,u2c,u3c,lambda,mu);

	  Range E = gc.numberOfDimensions()==2 ? Range(u1c,u2c) : Range(u1c,u3c);
	  realSerialArray ux(I1,I2,I3,E),uy(I1,I2,I3,E), uz;
          ux=0.;
	  uy=0.;

	  op.derivative(MappedGridOperators::xDerivative,v,ux,I1,I2,I3,E);
	  op.derivative(MappedGridOperators::yDerivative,v,uy,I1,I2,I3,E);

	  if( false )
	  {
	    printF("stressNorm:  lambda=%e mu=%e\n",lambda,mu);
	    ::display(ux,sPrintF(" ux on grid=%i\n",grid),"%9.2e ");
	    ::display(uy,sPrintF(" uy on grid=%i\n",grid),"%9.2e ");
	  }



	  real u1x,u1y,u1z, u2x,u2y,u2z, u3x,u3y,u3z,div;

	  int i1,i2,i3;
	  if( gc.numberOfDimensions()==2 )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3)>0 )
	      { 
		// Compute the stress from the displacement
		u1x = ux(i1,i2,i3,u1c), u1y=uy(i1,i2,i3,u1c);
		u2x = ux(i1,i2,i3,u2c), u2y=uy(i1,i2,i3,u2c);
	  
		div = u1x + u2y;
		s11 = lambda*div + 2.*mu*u1x;
		s12 = mu*( u1y+u2x );
		s22 = lambda*div + 2.*mu*u2y;
          
		V(i1,i2,i3,j)=sqrt( SQR(s11) + 2.*SQR(s12) + SQR(s22) );

	      }
	    }
	  }
	  else
	  {
	    uz.redim(I1,I2,I3,E);
            uz=0.;
	    op.derivative(MappedGridOperators::zDerivative,v,uz,I1,I2,I3,E);

	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3)>0 )
	      { 
		// Compute the stress from the displacement

		u1x = ux(i1,i2,i3,u1c), u1y=uy(i1,i2,i3,u1c), u1z=uz(i1,i2,i3,u1c);
		u2x = ux(i1,i2,i3,u2c), u2y=uy(i1,i2,i3,u2c), u2z=uz(i1,i2,i3,u2c);
		u3x = ux(i1,i2,i3,u3c), u3y=uy(i1,i2,i3,u3c), u3z=uz(i1,i2,i3,u3c);

		div = u1x+u2y+u3z;
		s11 = lambda*div + 2.*mu*u1x;
		s12 = mu*( u1y+u2x );
		s13 = mu*( u1z+u3x );
		s22 = lambda*div + 2.*mu*u2y;
		s23 = mu*( u2z + u3y );
		s33 = lambda*div + 2.*mu*u3z;

		V(i1,i2,i3,j)=sqrt( SQR(s11)+ 2.*SQR(s12) + 2.*SQR(s13) + SQR(s22) + 2.*SQR(s23) + SQR(s33) );

	      }
	    }
	  }
	}
	
      }
      else if( derived(i,0)==minimumScale )
      {
        #ifdef USE_PPP
	  Overture::abort("ERROR: finish me for parallel");
        #else

        // save the INVERSE of the scaled minimum scale: dx * sqrt( |Du| / nu ), j=1,2 or 3
        interpolationRequired=true;

        getVelocityComponents(uc,vc,wc);

        real nu=0.;
        // db.get(nu,"nu");
        showFileReader->getGeneralParameter("nu",nu);
	nu = max(REAL_MIN,nu);
      

        gc.update(MappedGrid::THEvertexDerivative);
// ** todo : optimised for rectangular **
//          for( int grid=0; grid<gc.numberOfGrids(); grid++ )
//  	  if( gc[grid].isRectangular() )
//    	    gc[grid].update(MappedGrid::THEvertexDerivative);

        const RealArray & dr = gc[grid].gridSpacing();
        const realMappedGridFunction & xr = gc[grid].vertexDerivative();

        if( numberOfDimensions==1 )
          v(I1,I2,I3,j)=sqrt(fabs(vg.x()(I1,I2,I3,uc))* ((1./(nu*(1./SQR(dr(0)))))) );
        else if( numberOfDimensions==2 )
	{
          v(I1,I2,I3,j)=sqrt( (SQR(xr(I1,I2,I3,0,0)) + SQR(xr(I1,I2,I3,1,0))) * dr(0)*dr(0) +
			      (SQR(xr(I1,I2,I3,0,1)) + SQR(xr(I1,I2,I3,1,1))) * dr(1)*dr(1) ) *
	    sqrt( sqrt(SQR(vg.x()(I1,I2,I3,uc)) + SQR(vg.y()(I1,I2,I3,uc)) +
		       SQR(vg.x()(I1,I2,I3,vc)) + SQR(vg.y()(I1,I2,I3,vc))) / nu);
	  
	}
        else 
	{
          v(I1,I2,I3,j)=sqrt( (SQR(xr(I1,I2,I3,0,0)) + SQR(xr(I1,I2,I3,1,0)) + SQR(xr(I1,I2,I3,2,0))) * dr(0)*dr(0) +
			      (SQR(xr(I1,I2,I3,0,1)) + SQR(xr(I1,I2,I3,1,1)) + SQR(xr(I1,I2,I3,2,1))) * dr(1)*dr(1) +
			      (SQR(xr(I1,I2,I3,0,2)) + SQR(xr(I1,I2,I3,1,2)) + SQR(xr(I1,I2,I3,2,2))) * dr(2)*dr(2) ) *
	    sqrt( sqrt(SQR(vg.x()(I1,I2,I3,uc)) + SQR(vg.y()(I1,I2,I3,uc)) + SQR(vg.z()(I1,I2,I3,uc)) +
		       SQR(vg.x()(I1,I2,I3,vc)) + SQR(vg.y()(I1,I2,I3,vc)) + SQR(vg.z()(I1,I2,I3,vc)) +
		       SQR(vg.x()(I1,I2,I3,wc)) + SQR(vg.y()(I1,I2,I3,wc)) + SQR(vg.z()(I1,I2,I3,wc))) / nu);

//            v(I1,I2,I3,j)=sqrt(
//  	    (fabs(vg.x()(I1,I2,I3,uc))+fabs(vg.y()(I1,I2,I3,uc))+fabs(vg.z()(I1,I2,I3,uc))+
//  	     fabs(vg.x()(I1,I2,I3,vc))+fabs(vg.y()(I1,I2,I3,vc))+fabs(vg.z()(I1,I2,I3,vc))+
//  	     fabs(vg.x()(I1,I2,I3,wc))+fabs(vg.y()(I1,I2,I3,wc))+fabs(vg.z()(I1,I2,I3,wc)))
//  	    *((1./(9./3.*nu*( 1./SQR(dr(0)) + 1./SQR(dr(1)) +1./SQR(dr(2)) )))) );
	}
        // printF(" **** grid=%i : max(min scale)=%e\n",grid,max(fabs(v(I1,I2,I3,j))));
      }
      else if( derived(i,0)==r1MinimumScale || 
	       derived(i,0)==r2MinimumScale ||
	       derived(i,0)==r3MinimumScale )
      {
        interpolationRequired=true;
        getVelocityComponents(uc,vc,wc);

        // save the INVERSE of the scaled minimum scale: dx_j * sqrt( |Du * e_j| / nu ), j=1,2 or 3
        real nu=0.;
        // db.get(nu,"nu");
        showFileReader->getGeneralParameter("nu",nu);
	nu = max(REAL_MIN,nu);

        gc[grid].update(MappedGrid::THEvertexDerivative);
// ** todo : optimised for rectangular **
//          for( int grid=0; grid<gc.numberOfGrids(); grid++ )
//  	  if( gc[grid].isRectangular() )
//    	    gc[grid].update(MappedGrid::THEvertexDerivative);
      
        const RealArray & dr = gc[grid].gridSpacing();
        const realMappedGridFunction & xr = gc[grid].vertexDerivative();
        const intArray & mask = gc[grid].mask();
	
        Index I1,I2,I3;

        if( numberOfDimensions==1 )
	{
          v(I1,I2,I3,j)=0.;
	}
        else if( numberOfDimensions==2 )
	{
          realArray xr1Norm, dur1, xr2Norm, dur2;
          
          if( derived(i,0)==r1MinimumScale )
	  {
	    xr1Norm =sqrt(sqrt(SQR(xr(I1,I2,I3,0,0))+SQR(xr(I1,I2,I3,1,0))));
	    
	    dur1 = sqrt( SQR(vg.x()(I1,I2,I3,uc)*xr(I1,I2,I3,0,0) + vg.y()(I1,I2,I3,uc)*xr(I1,I2,I3,1,0)) + 
			 SQR(vg.x()(I1,I2,I3,vc)*xr(I1,I2,I3,0,0) + vg.y()(I1,I2,I3,vc)*xr(I1,I2,I3,1,0)) );
	    
//            printF(" max(dur1)=%e, min(xr1Norm)=%e uNorm=%e\n",max(dur1),min(xr1Norm),min(uNorm));
	    
	    v(I1,I2,I3,j)= xr1Norm * dr(0) * dur1 / sqrt(nu);
	  }
	  else if( derived(i,0)==r2MinimumScale )
	  {
	    xr2Norm =sqrt(sqrt(SQR(xr(I1,I2,I3,0,1))+SQR(xr(I1,I2,I3,1,1))));
	    
	    dur2 = sqrt( SQR(vg.x()(I1,I2,I3,uc)*xr(I1,I2,I3,0,1) + vg.y()(I1,I2,I3,uc)*xr(I1,I2,I3,1,1)) + 
			 SQR(vg.x()(I1,I2,I3,vc)*xr(I1,I2,I3,0,1) + vg.y()(I1,I2,I3,vc)*xr(I1,I2,I3,1,1)) );
	    
	    v(I1,I2,I3,j)= xr2Norm * dr(1) * dur2 / sqrt(nu);

	  }

	  where( mask(I1,I2,I3)==0 )
	    v(I1,I2,I3,j)=0.;
	    
	  // display(v(I1,I2,I3,j),"q");
	  
	}
	else if ( numberOfDimensions == 3 )
	{
	  printF("Sorry, the 3-D case is not implemented yet\n");
	  v(I1,I2,I3,j)=0.;
	}
       #endif
      }
      else if( derived(i,0)==r1Velocity || derived(i,0)==r2Velocity || derived(i,0)==r3Velocity  )
      {
        // compute velocities that are parallel to the coordinate directions

        interpolationRequired=false;
        getVelocityComponents(uc,vc,wc);

        const int dir= derived(i,0)==r1Velocity ? 0 : derived(i,0)==r2Velocity ? 1 : 2;

        gc.update(MappedGrid::THEvertexDerivative);

// ** todo : optimised for rectangular **
//          for( int grid=0; grid<gc.numberOfGrids(); grid++ )
//  	  if( gc[grid].isRectangular() )
//    	    gc[grid].update(MappedGrid::THEvertexDerivative);


        #ifdef USE_PPP
  	  realSerialArray xr; getLocalArrayWithGhostBoundaries(gc[grid].vertexDerivative(),xr);
        #else
          realSerialArray & xr = gc[grid].vertexDerivative();
        #endif
        realSerialArray norm(I1,I2,I3);

	if( numberOfDimensions==1 )
	{
	  v(I1,I2,I3,j)=v(I1,I2,I3,uc);
	}
	else if( numberOfDimensions==2 )
	{
          norm=1./max( REAL_MIN*100., sqrt( SQR(xr(I1,I2,I3,0,dir))+SQR(xr(I1,I2,I3,1,dir)) ));
          v(I1,I2,I3,j)=(v(I1,I2,I3,uc)*xr(I1,I2,I3,0,dir)+ 
                         v(I1,I2,I3,vc)*xr(I1,I2,I3,1,dir))*norm;
	}
	else
	{
          norm=1./max( REAL_MIN*100., sqrt( SQR(xr(I1,I2,I3,0,dir))+SQR(xr(I1,I2,I3,1,dir))+SQR(xr(I1,I2,I3,2,dir)) ));
          v(I1,I2,I3,j)=(v(I1,I2,I3,uc)*xr(I1,I2,I3,0,dir)+
                         v(I1,I2,I3,vc)*xr(I1,I2,I3,1,dir)+
                         v(I1,I2,I3,wc)*xr(I1,I2,I3,2,dir))*norm;
	}
      }
      else if( derived(i,0)==cellVolume )
      {
        gc[grid].update(MappedGrid::THEcellVolume);
      
        #ifdef USE_PPP
  	  realSerialArray cellVolume; getLocalArrayWithGhostBoundaries(gc[grid].cellVolume(),cellVolume);
        #else
          realSerialArray & cellVolume = gc[grid].cellVolume();
        #endif

        real signForJacobian=gc[grid].mapping().getMapping().getSignForJacobian();
	
        if( signForJacobian>0. )
    	  v(I1,I2,I3,j)=cellVolume(I1,I2,I3);
	else
          v(I1,I2,I3,j)=-cellVolume(I1,I2,I3);
      }
      else if( derived(i,0)==minimumEdgeLength )
      {

        gc[grid].update(MappedGrid::THEvertex);
         
        
        #ifdef USE_PPP
  	  realSerialArray vertex; getLocalArrayWithGhostBoundaries(gc[grid].vertex(),vertex);
        #else
          realSerialArray & vertex = gc[grid].vertex();
        #endif


	const real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
	const int vertexDim0=vertex.getRawDataSize(0);
	const int vertexDim1=vertex.getRawDataSize(1);
	const int vertexDim2=vertex.getRawDataSize(2);
#define X(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]
	real *vp = v.Array_Descriptor.Array_View_Pointer3;
	const int vDim0=v.getRawDataSize(0);
	const int vDim1=v.getRawDataSize(1);
	const int vDim2=v.getRawDataSize(2);
#define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]


        v(I1,I2,I3,j)=0.;

	getIndex(gc[grid].dimension(),I1,I2,I3,-1);
	bool ok = ParallelUtility::getLocalArrayBounds(vg,v,I1,I2,I3);
	if( !ok ) continue;


        real ds1,ds2;
        int i1,i2,i3;
        // int I1Bound,I2Bound,I3Bound;
        if( numberOfDimensions==3 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    ds1 = SQR(X(i1+1,i2,i3,0)-X(i1,i2,i3,0))+SQR(X(i1+1,i2,i3,1)-X(i1,i2,i3,1))+
	          SQR(X(i1+1,i2,i3,2)-X(i1,i2,i3,2));
	    ds2 = SQR(X(i1-1,i2,i3,0)-X(i1,i2,i3,0))+SQR(X(i1-1,i2,i3,1)-X(i1,i2,i3,1))+
	          SQR(X(i1-1,i2,i3,2)-X(i1,i2,i3,2));
	    V(i1,i2,i3,j)=min(ds1,ds2);
	    ds1 = SQR(X(i1,i2+1,i3,0)-X(i1,i2,i3,0))+SQR(X(i1,i2+1,i3,1)-X(i1,i2,i3,1))+
	          SQR(X(i1,i2+1,i3,2)-X(i1,i2,i3,2));
	    ds2 = SQR(X(i1,i2-1,i3,0)-X(i1,i2,i3,0))+SQR(X(i1,i2-1,i3,1)-X(i1,i2,i3,1))+
	          SQR(X(i1,i2-1,i3,2)-X(i1,i2,i3,2));
	    V(i1,i2,i3,j)=min(V(i1,i2,i3,j),ds1,ds2);
	    ds1 = SQR(X(i1,i2,i3+1,0)-X(i1,i2,i3,0))+SQR(X(i1,i2,i3+1,1)-X(i1,i2,i3,1))+
	          SQR(X(i1,i2,i3+1,2)-X(i1,i2,i3,2));
	    ds2 = SQR(X(i1,i2,i3-1,0)-X(i1,i2,i3,0))+SQR(X(i1,i2,i3-1,1)-X(i1,i2,i3,1))+
	          SQR(X(i1,i2,i3-1,2)-X(i1,i2,i3,2));
	    V(i1,i2,i3,j)=min(V(i1,i2,i3,j),ds1,ds2);
            V(i1,i2,i3,j)=sqrt(V(i1,i2,i3,j));
	  }
	}
	else if( numberOfDimensions==2 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    ds1 = SQR(X(i1+1,i2,i3,0)-X(i1,i2,i3,0))+SQR(X(i1+1,i2,i3,1)-X(i1,i2,i3,1));
	    ds2 = SQR(X(i1-1,i2,i3,0)-X(i1,i2,i3,0))+SQR(X(i1-1,i2,i3,1)-X(i1,i2,i3,1));
	    V(i1,i2,i3,j)=min(ds1,ds2);
	    ds1 = SQR(X(i1,i2+1,i3,0)-X(i1,i2,i3,0))+SQR(X(i1,i2+1,i3,1)-X(i1,i2,i3,1));
	    ds2 = SQR(X(i1,i2-1,i3,0)-X(i1,i2,i3,0))+SQR(X(i1,i2-1,i3,1)-X(i1,i2,i3,1));
	    V(i1,i2,i3,j)=min(V(i1,i2,i3,j),ds1,ds2);
            V(i1,i2,i3,j)=sqrt(V(i1,i2,i3,j));
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    ds1 = fabs(X(i1+1,i2,i3,0)-X(i1,i2,i3,0));
	    ds2 = fabs(X(i1-1,i2,i3,0)-X(i1,i2,i3,0));
	    V(i1,i2,i3,j)=min(ds1,ds2);
	  }
	}

      }
      else if( derived(i,0)==schlieren || derived(i,0)==userDefined )
      {
        // this case is done below
      }
      else if( derived(i,0)==energyDensity )
      {
        int ex=0, ey=1, ez=2;
        int hx=3, hy=4, hz=5;
        getComponent(ex,"exFieldComponent");
        getComponent(ey,"eyFieldComponent");

	if( numberOfDimensions==2 )
	{
          hz=2; // set default
          getComponent(hz,"hzFieldComponent");

          // printF(" DerivedFunctions: energyDensity: ex=%i, ey=%i hz=%i\n",ex,ey,hz);

	  v(I1,I2,I3,j)=SQR(v(I1,I2,I3,ex))+SQR(v(I1,I2,I3,ey)) + SQR(v(I1,I2,I3,hz));
	}
	else
	{
          getComponent(ez,"ezFieldComponent");

	  getComponent(hx,"hxFieldComponent");
	  getComponent(hy,"hyFieldComponent");
          getComponent(hz,"hzFieldComponent");

	  v(I1,I2,I3,j)=( SQR(v(I1,I2,I3,ex))+SQR(v(I1,I2,I3,ey))+SQR(v(I1,I2,I3,ez)) +
			  SQR(v(I1,I2,I3,hx))+SQR(v(I1,I2,I3,hy))+SQR(v(I1,I2,I3,hz)) );
	}
      }
      else if( derived(i,0)==eFieldNorm )
      {
        int ex=0, ey=1, ez=2;
        getComponent(ex,"exFieldComponent");
        getComponent(ey,"eyFieldComponent");

	printF(" DerivedFunctions: eFieldNorm: ex=%i, ey=%i\n",ex,ey);
	
	if( numberOfDimensions==2 )
	{
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,ex))+SQR(v(I1,I2,I3,ey)) );
	}
	else
	{
          getComponent(ez,"ezFieldComponent");
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,ex))+SQR(v(I1,I2,I3,ey))+SQR(v(I1,I2,I3,ez)) );
	}
      }
      else if( derived(i,0)==hFieldNorm )
      {
        int hx=0, hy=1, hz=2;
        getComponent(hx,"hxFieldComponent");
        getComponent(hy,"hyFieldComponent");

	if( numberOfDimensions==2 )
	{
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,hx))+SQR(v(I1,I2,I3,hy)) );
	}
	else
	{
          getComponent(hz,"hzFieldComponent");
	  v(I1,I2,I3,j)=sqrt( SQR(v(I1,I2,I3,hx))+SQR(v(I1,I2,I3,hy))+SQR(v(I1,I2,I3,hz)) );
	}
      }
      else 
      {
	printF("DerivedFunctions::computeDerivedFunctions:ERROR: unknown value for i=%i, derived(i,0)=%i\n",
	       i,derived(i,0));
	throw "error";
      }
      
    }
  }

  // now process variables that require info about all grids, so we must
  // put the loop over grids inside the loop over numberOfDerivedFunctions
  for( i=0; i<numberOfDerivedFunctions; i++ )
  {
    int j=numberOfComponents+i;
    assert( derived(i,0)>=0 );
    if( derived(i,0)==userDefined )
    {
      getUserDefinedDerivedFunction(i,j,name[i],numberOfComponents,u,u0,interpolationRequired);
    }
    else if( derived(i,0)==schlieren )
    {
      interpolationRequired=true;
      getComponent(rc,"densityComponent");
      real sMin=REAL_MAX;
      real sMax=0.;
      
      for( grid=0; grid<gc.numberOfGrids(); grid++ )
      {
        realMappedGridFunction & vg = u0[grid];    
        MappedGridOperators & op = cgop[grid];

        #ifdef USE_PPP
	 realSerialArray v;  getLocalArrayWithGhostBoundaries(vg,v);
         intSerialArray mask; getLocalArrayWithGhostBoundaries(gc[grid].mask(),mask);
	 
        #else
	 realSerialArray & v = vg;
         const intSerialArray & mask = gc[grid].mask();
        #endif

	getIndex(gc[grid].dimension(),I1,I2,I3);
	bool ok = ParallelUtility::getLocalArrayBounds(vg,v,I1,I2,I3);
	if( ok )
	{
	  if( numberOfDimensions==1 )
	  {
	    realSerialArray rx(I1,I2,I3);
            rx=0.;
	    op.derivative(MappedGridOperators::xDerivative,v,rx,I1,I2,I3,rc);
	    v(I1,I2,I3,j)=fabs(rx);
	  }
	  else if( numberOfDimensions==2 )
	  {
	    realSerialArray rx(I1,I2,I3),ry(I1,I2,I3);
            rx=0.;
	    ry=0.;
	    op.derivative(MappedGridOperators::xDerivative,v,rx,I1,I2,I3,rc);
	    op.derivative(MappedGridOperators::yDerivative,v,ry,I1,I2,I3,rc);

	    v(I1,I2,I3,j)=sqrt(rx*rx+ry*ry);
	  }
	  else if( numberOfDimensions==3 )
	  {
	    realSerialArray rx(I1,I2,I3),ry(I1,I2,I3),rz(I1,I2,I3);
            rx=0.;
	    ry=0.;
	    rz=0.;
	    op.derivative(MappedGridOperators::xDerivative,v,rx,I1,I2,I3,rc);
	    op.derivative(MappedGridOperators::yDerivative,v,ry,I1,I2,I3,rc);
	    op.derivative(MappedGridOperators::zDerivative,v,rz,I1,I2,I3,rc);
	  
	    v(I1,I2,I3,j)=sqrt(rx*rx+ry*ry+rz*rz);
	  }
      
	  where( mask(I1,I2,I3)<=0 )
	    v(I1,I2,I3,j)=0.;

	  /* *jwb* 032207 */
	  Index J1,J2,J3;
	  getIndex(gc[grid].indexRange(),J1,J2,J3);
	  bool ok = ParallelUtility::getLocalArrayBounds(vg,v,J1,J2,J3);
	  if( ok ) 
	  {
	    sMin=min(sMin,min(v(J1,J2,J3,j)));
	    sMax=max(sMax,max(v(J1,J2,J3,j)));
	  }
	}
      } // end if ok 
      #ifdef USE_PPP
       sMin=ParallelUtility::getMinValue(sMin);
       sMax=ParallelUtility::getMaxValue(sMax);
      #endif

      if( fabs(sMax-sMin)>0. )
      {
	for( grid=0; grid<gc.numberOfGrids(); grid++ )
	{
	  realMappedGridFunction & vg = u0[grid];
          #ifdef USE_PPP
	   realSerialArray v;  getLocalArrayWithGhostBoundaries(vg,v);
          #else
  	   realSerialArray & v = vg;
          #endif

  	  getIndex(gc[grid].dimension(),I1,I2,I3);
	  bool ok = ParallelUtility::getLocalArrayBounds(vg,v,I1,I2,I3);
	  if( ok )
	  {
	    v(I1,I2,I3,j)=(v(I1,I2,I3,j)-sMin)*(1./(sMax-sMin));
	    v(I1,I2,I3,j)=exposure*exp(-amplification*v(I1,I2,I3,j));
	  }
	}
      }
    }
  }

  u.reference(u0);
  if( interpolationRequired )
  {
    if( gc.rcData->interpolant==NULL )
    {
      // we need to build an interpolant for this grid.  // *** but who deletes this ??

      // delete interpolant;  // *wdh* 081022 This is now done in the GridCollection
      Interpolant *interpolant = new Interpolant(gc);  // This constructor will set gc.rcData.interpolant
      assert( gc.rcData->interpolant!=NULL );
      // *wdh* 2012/07/05 gc.rcData->interpolant->incrementReferenceCount(); // *wdh* 081022 

      printF("DerivedFunctions: Creating a new interpolant. Reference count=%i.\n",
	     gc.rcData->interpolant->getReferenceCount());

      // *wdh* 020718
      interpolant->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      interpolant->setMaximumNumberOfIterations(15); // *wdh* 040820 

    }
    u.interpolate();
  }
  
  return 0;
}


/* ----
int DerivedFunctions::
calculator()
{
  aString menu[]=
  {
    "u",
    "v",
    "+",
    "-",
    "*",
    ""
  };
  
  // sqrt(u*u+v*v) ->   u * u + v * v sqrt
  
}
-- */

//\begin{>>DerivedFunctionsInclude.tex}{\subsection{add}} 
int DerivedFunctions::
add(int derivative, const aString & name_,
    int n1 /* =0 */, 
    int n2 /* =0 */ )
// ============================================================================
// /Description:
//    Add a new entry to the list of derived functions.
// /derivative (input) : derived type code.
// /name\_ (input) : component name for this entry.
// /n1,n2 (input) : extra info to save in the derived array.
//\end{DerivedFunctionsInclude.tex}
// ============================================================================
{
  if( derived.getLength(0)<=numberOfDerivedFunctions )
  {
    // allocate more space

    int newNumber=numberOfDerivedFunctions+10;
    
    derived.resize(newNumber,3);
    derived(Range(derived.getBound(0)+1,newNumber-1),Range(0,2))=-1;
    aString *temp = new aString[newNumber];
    for( int i=0; i<numberOfDerivedFunctions; i++ )
      temp[i]=name[i];
    delete [] name;
    name = temp;
  }
  
  name[numberOfDerivedFunctions]=name_;
  
  derived(numberOfDerivedFunctions,0)=derivative;
  derived(numberOfDerivedFunctions,1)=n1;
  derived(numberOfDerivedFunctions,2)=n2;

  numberOfDerivedFunctions++;
  return numberOfDerivedFunctions;
}

//\begin{>>DerivedFunctionsInclude.tex}{\subsection{remove}} 
int DerivedFunctions::
remove( int i )
// ==================================================================================
// /Description:
//   remove an entry from the list of derived types.
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{
  if( i>=0 && i<numberOfDerivedFunctions )
  {
    Range I(i,numberOfDerivedFunctions-2), P(0,2);
    derived(I,P)=derived(I+1,P);

  numberOfDerivedFunctions--;
  derived(numberOfDerivedFunctions,P)=-1;
  }
  else
  {
    printF("DerivedFunctions::remove:ERROR: i=%i should be in the range [%i,%i]\n",i,0,numberOfDerivedFunctions);
  }
  
  return numberOfDerivedFunctions;
}


//\begin{>>DerivedFunctionsInclude.tex}{\subsection{update}} 
int DerivedFunctions::
update(GenericGraphicsInterface & gi, 
       int numberOfComponents, 
       aString *componentNames,
       GraphicsParameters *pgp /* =NULL */ )
// ==================================================================================
// /Description:
// update current list of derived grid functions
//
// /numberOfComponents, componentNames : supply names of components
// /pgp : optionally provide a GraphicsParameters object which is updated with
//    the velocity components, displacmenet components etc. of they are set
//\end{DerivedFunctionsInclude.tex}
// ==================================================================================
{

  aString menu[]=
  {
    "!Derived Functions",
    ">cfd variables",
      "vorticity",
      "xVorticity",
      "yVorticity",
      "zVorticity",
      "enstrophy",
      "divergence",
      "mach Number",
      "pressure",
      "temperature",
      "speed",
      "entropy",
      "schlieren",
      "r1 velocity",
      "r2 velocity",
      "r3 velocity",
      "minimumScale",
      "r1MinimumScale",
      "r2MinimumScale",
      "r3MinimumScale",
    "<>other",
      "logarithm",
    "<>E&M variables",
      "energy density",
      "E field norm",
      "H field norm",
    "<>Solid Mechanics variables",
      "speed",
      "displacementNorm",
      "stressNorm",
    "<user defined",
    ">derivatives",
      "x",
      "y",
      "z",
      "xx",
      "yy",
      "zz",
      "laplacian",
      "gradient norm",
    "<>geometry",
      "cell volume",
      "minimum edge length",    
    "<>parameters",
      "schlieren parameters",
      "specify displacement components",
      "specify velocity components",
      "specify stress components",
    "<calculator",
    "remove",
    "exit",
    ""
  };
  aString answer;
  
  for( ;; )
  {
    int choice = gi.getMenuItem(menu,answer,"toggle items");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="x" || answer=="y" || answer=="z" || answer=="laplacian" || answer=="gradient norm" )
    {
      aString *cNames = new aString [numberOfComponents+3];
      aString answer2;
      IntegerArray componentIsOn(numberOfComponents);
      componentIsOn=0;
      
      for( ;; )
      {
        int n;
	for( n=0; n<numberOfComponents; n++ )
	{
	  cNames[n]=componentNames[n] + (componentIsOn(n) ? "  (on)" : "  (off)");
	}
	n=numberOfComponents;
	cNames[n]="all";  n++;
	cNames[n]="done"; n++;
	cNames[n]="";

        int c = gi.getMenuItem(cNames,answer2,"choose components");
        if( answer2=="done" || answer=="exit" )
	{
	  break;
	}
	else if( c>=0 && c<numberOfComponents )
	{
	  componentIsOn(c)=!componentIsOn(c);
	}
        else if( answer2=="all" )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    componentIsOn(n)=!componentIsOn(n);
	  }
          break;
	}
	else
	{
	  cout << "Unknown response: [" << answer << "]\n";
	  gi.stopReadingCommandFile();
	}
      }
      const int unknownDerivative=1001;
      int derivative = answer=="x" ? xDerivative : answer=="y" ? yDerivative : answer=="z" ? zDerivative : 
                       answer=="xx" ? xxDerivative : answer=="yy" ? yyDerivative : answer=="zz" ? zzDerivative :
                       answer=="laplacian" ? laplaceDerivative : 
                       answer=="gradient norm" ? gradientNorm : unknownDerivative;
      
      if( derivative==unknownDerivative )
      {
	printF("ERROR:unknown derivative: [%s]\n",(const char*)answer);
      }
      else
      {
	for( int n=0; n<numberOfComponents; n++ )
	{
	  if( componentIsOn(n) )
	    add( derivative,componentNames[n]+"."+answer,n );
	}
      }
      delete [] cNames;
    }
    else if( answer=="vorticity" )
    {
      add( vorticity,"vorticity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="xVorticity" )
    {
      add( xVorticity,"xVorticity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="yVorticity" )
    {
      add( yVorticity,"yVorticity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="zVorticity" )
    {
      add( zVorticity,"zVorticity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="enstrophy" )
    {
      add( enstrophy,"enstrophy" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
      printF("The enstrophy is the L2 norm of the vorticity.\n");
    }
    else if( answer=="divergence" )
    {
      add( divergence,"divergence" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="mach Number" )
    {
      add( machNumber,"machNumber" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="pressure" )
    {
      add( pressure,"pressure" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="temperature" )
    {
      add( temperature,"temperature" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="speed" )
    {
      add( speed,"speed" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
//       if( velocityComponent[0]==-1 )
//       {
//         // look for the components to use for the velocity
// 	int uc=-2, vc=-2, wc=-2; // NOTE: use -2 
// 	getComponent(uc,"v1Component");
// 	if( uc>=0 ) // this means found
// 	{
// 	  getComponent(vc,"v2Component");
// 	  getComponent(wc,"v3Component");
// 	}
// 	else
// 	{
// 	  // fluid velocity
// 	  getComponent(uc,"uComponent");
// 	  getComponent(vc,"vComponent");
// 	  getComponent(wc,"wComponent");
// 	}
// 	velocityComponent[0]=uc;
// 	velocityComponent[1]=vc;
// 	velocityComponent[2]=wc;
//       }
      
//       printF("INFO: I will use uc=%i, vc=%i, wc=%i for the components of the velocity.\n",
//               velocityComponent[0],velocityComponent[1],velocityComponent[2]);
      printF("INFO: Use the option `specify velocity components to over-ride the default choice\n");
    }
    else if( answer=="entropy" )
    {
      add( entropy,"entropy" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="schlieren" )
    {
      add( schlieren,"schlieren" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r1 velocity" )
    {
      add( r1Velocity,"r1Velocity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r2 velocity" )
    {
      add( r2Velocity,"r2Velocity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r3 velocity" )
    {
      add( r3Velocity,"r3Velocity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="minimumScale" )
    {
      add( minimumScale,"minimumScale" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r1MinimumScale" )
    {
      add( r1MinimumScale,"r1MinimumScale" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r2MinimumScale" )
    {
      add( r2MinimumScale,"r2MinimumScale" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="r3MinimumScale" )
    {
      add( r3MinimumScale,"r3MinimumScale" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="cell volume" )
    {
      add( cellVolume,"cellVolume" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="minimum edge length" )
    {
      add( minimumEdgeLength,"minimumEdgeLength" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="user defined" )
    {
      setupUserDefinedDerivedFunction(gi,numberOfComponents,componentNames );
    }
    else if( answer=="energy density" )
    {
      add( energyDensity,"energyDensity" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="E field norm" )
    {
      add( eFieldNorm,"eFieldNorm" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="H field norm" )
    {
      add( hFieldNorm,"hFieldNorm" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
    }
    else if( answer=="displacementNorm" )
    {
      add( displacementNorm,"displacementNorm" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
      printF(" The displacement norm is the square root of the sum of the squares of the displacement components\n");
    }
    else if( answer=="stressNorm" )
    {
      add( stressNorm,"stressNorm" );
      printF("%s was chosen.\n",(const char*)menu[choice]);
      printF(" The stress norm is the square root of the sum of the squares of the stress components\n");
    }

    else if( answer=="remove" )
    {
      aString *cNames = new aString [numberOfDerivedFunctions+3];
      aString answer2;
      
      for( ;; )
      {
        int n;
	for( n=0; n<numberOfDerivedFunctions; n++ )
	{
	  cNames[n]=name[n];
	}
	n=numberOfDerivedFunctions;
	cNames[n]="all";  n++;
	cNames[n]="done"; n++;
	cNames[n]="";

        int c = gi.getMenuItem(cNames,answer2,"remove components");
        if( answer2=="done" || answer=="exit" )
	{
	  break;
	}
	else if( c>=0 && c<numberOfComponents )
	{
	  remove(c);
	}
        else if( answer2=="all" )
	{
	  for(  n=numberOfDerivedFunctions; n>=0; n-- )
	  {
	    remove(n);
	  }
          break;
	}
	else
	{
	  printF("Unknown response: [%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}
      }
      delete [] cNames;
    }
    else if( answer=="schlieren parameters" )
    {
      printF(" The schlieren function is exposure*exp(-amplification*R)) \n"
             "  R = (|grad rho| - min{|grad rho|}) /( max{|grad rho|}- min{|grad rho|} )\n"
             "  Current values: exposure=%g, amplification=%g\n",exposure,amplification);
      aString answer2;
      gi.inputString(answer2,"Enter exposure,amplification");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e",&exposure,&amplification);
	printF(" Using exposure=%9.3e and amplification=%9.3e\n",exposure,amplification);
      }
    }
    else if( answer=="specify displacement components" )
    {
      printF("Specify the components to use for the displacement. Here are the available components:\n");
      for( int n=0; n<numberOfComponents; n++ )
      {
	printF("  component %i : %s\n",n,(const char*)componentNames[n]);
      }
      aString answer2;
      int uc=displacementComponent[0], vc=displacementComponent[1], wc=displacementComponent[2];
      if( uc<0 ) getDisplacementComponents(uc,vc,wc);
      gi.inputString(answer2,sPrintF("Enter the 3 components uc,vc,wc (default= %i,%i,%i)",uc,vc,wc));
      if( answer2!="" )
      {
	sScanF(answer2,"%i %i %i",&uc,&vc,&wc);
	if( uc>=0 && uc<numberOfComponents && vc>=0 && vc<numberOfComponents && wc>=0 && wc<numberOfComponents )
	{
          displacementComponent[0]=uc;
          displacementComponent[1]=vc;
          displacementComponent[2]=wc;
	  if( pgp!=NULL )
	  {
            pgp->set(GI_DISPLACEMENT_U_COMPONENT,uc);
            pgp->set(GI_DISPLACEMENT_V_COMPONENT,vc);
            pgp->set(GI_DISPLACEMENT_W_COMPONENT,wc);
	  }
	  
	}
	else
	{
	  printF("ERROR: invalid value for one of uc=%i, vc=%i, wc=%i (numberOfComponents=%i). \n"
                 "   All components should be in the range [0,%i]. I am not changing the components.\n",uc,vc,wc,numberOfComponents-1);
	}
	printF(" Using displacement components %i, %i, %i\n",displacementComponent[0],displacementComponent[1],displacementComponent[2]);
	
      }
    }
    else if( answer=="specify velocity components" )
    {
      printF("Specify the components to use for the velocity. Here are the available components:\n");
      for( int n=0; n<numberOfComponents; n++ )
      {
	printF("  component %i : %s\n",n,(const char*)componentNames[n]);
      }
      aString answer2;
      int uc=velocityComponent[0], vc=velocityComponent[1], wc=velocityComponent[2];
      gi.inputString(answer2,sPrintF("Enter the 3 components uc,vc,wc (default= %i,%i,%i)",uc,vc,wc));
      if( answer2!="" )
      {
	sScanF(answer2,"%i %i %i",&uc,&vc,&wc);
	if( uc>=0 && uc<numberOfComponents && vc>=0 && vc<numberOfComponents && wc>=0 && wc<numberOfComponents )
	{
          velocityComponent[0]=uc;
          velocityComponent[1]=vc;
          velocityComponent[2]=wc;
	  if( pgp!=NULL )
	  {
            pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,uc);
            pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,vc);
            pgp->set(GI_U_COMPONENT_FOR_STREAM_LINES,wc);
	  }
	}
	else
	{
	  printF("ERROR: invalid value for one of uc=%i, vc=%i, wc=%i (numberOfComponents=%i). \n"
                 "   All components should be in the range [0,%i]. I am not changing the components.\n",uc,vc,wc,numberOfComponents-1);
	}
	printF(" Using velocity components %i, %i, %i\n",velocityComponent[0],velocityComponent[1],velocityComponent[2]);
	
      }
    }
    else if( answer=="specify stress components" )
    {
      printF("Specify the components to use for the stress. Here are the available components:\n");
      for( int n=0; n<numberOfComponents; n++ )
      {
	printF("  component %i : %s\n",n,(const char*)componentNames[n]);
      }
      aString answer2;
      int s11c=stressComponent[0], s12c=stressComponent[1], s13c=stressComponent[2];
      int s21c=stressComponent[3], s22c=stressComponent[4], s23c=stressComponent[5];
      int s31c=stressComponent[6], s32c=stressComponent[7], s33c=stressComponent[8];

      gi.inputString(answer2,sPrintF("Enter the 9 components: s11,s12,s13, s21,s22,s23, s31,s32,s33"));
      if( answer2!="" )
      {
	sScanF(answer2,"%i %i %i %i %i %i %i %i %i",
	       &stressComponent[0],&stressComponent[1],&stressComponent[2],
	       &stressComponent[3],&stressComponent[4],&stressComponent[5],
	       &stressComponent[6],&stressComponent[7],&stressComponent[8]);
	
	printF(" Using stress components s11=%i s12=%i s13=%i, s21=%i s22=%i s23=%i s31=%i s32=%i s33=%i.\n",
	       stressComponent[0],stressComponent[1],stressComponent[2],
	       stressComponent[3],stressComponent[4],stressComponent[5],
	       stressComponent[6],stressComponent[7],stressComponent[8]);
	
      }
    }
    else if( answer=="logarithm" )
    { // from pmb 080419 
      aString *cNames = new aString [numberOfComponents+3];
      aString answer2;
      IntegerArray componentIsOn(numberOfComponents);
      componentIsOn=0;
      
      for( ;; )
      {
	int n;
	for( n=0; n<numberOfComponents; n++ )
	{
	  cNames[n]=componentNames[n] + (componentIsOn(n) ? "  (on)" : "  (off)");
	}
	n=numberOfComponents;
	cNames[n]="all";  n++;
	cNames[n]="done"; n++;
	cNames[n]="";

	int c = gi.getMenuItem(cNames,answer2,"choose components");
	if( answer2=="done" || answer=="exit" )
	{
	  break;
	}
	else if( c>=0 && c<numberOfComponents )
	{
	  componentIsOn(c)=!componentIsOn(c);
	}
	else if( answer2=="all" )
	{
	  for( n=0; n<numberOfComponents; n++ )
	  {
	    componentIsOn(n)=!componentIsOn(n);
	  }
	  break;
	}
	else
	{
	  cout << "Unknown response: [" << answer << "]\n";
	  gi.stopReadingCommandFile();
	}
      }

      for( int n=0; n<numberOfComponents; n++ )
      {
	if( componentIsOn(n) )
	  add( logarithm,"ln "+componentNames[n],n );
      }
      delete [] cNames;

    }

    else if( answer=="calculator" )
    {
      printF("Sorry: calculator not implemented yet\n");
    }
    else
    {
      printF("Unknown response: [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }

  return 0;

}

