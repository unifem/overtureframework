#include "MatrixTransform.h"
#include "MappingInformation.h"
#include "MappingRC.h"

MatrixTransform::
MatrixTransform() : ComposeMapping()
//===========================================================================
/// \brief  Build a mapping for matrix transform.
//===========================================================================
{ 
  matrix = NULL;
  
  MatrixTransform::className="MatrixTransform";
  setName( Mapping::mappingName,"Transform");
  setGridDimensions( axis1,5 );
  setGridDimensions( axis2,5 );
  mappingHasChanged();
}

MatrixTransform::
MatrixTransform(Mapping & map) 
//===========================================================================
/// \brief  Build a Mapping for matrix transformation of another Mapping.
//===========================================================================
: ComposeMapping()
{ 
  matrix = new MatrixMapping();
  matrix->incrementReferenceCount();  // this indicates that the mapping was newed

  MatrixTransform::className="MatrixTransform";

  // set dimensions of matrix mapping to match
  matrix->setDomainDimension(map.getRangeDimension());  // set matrix domain and range dimensions
  matrix->setRangeDimension(map.getRangeDimension());
  // compose the mappings:
  setMappings(map,*matrix);

  setMappingProperties();

  setName(mappingName,aString("matrixTransform-")+map.getName(mappingName));
}


// Copy constructor is deep by default
MatrixTransform::
MatrixTransform( const MatrixTransform & map, const CopyType copyType )
{
  MatrixTransform::className="MatrixTransform";
  matrix=NULL;
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "MatrixTransform:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

MatrixTransform::
~MatrixTransform()
{ 
  if( debug & 4 )
    cout << " MatrixTransform::Destructor called" << endl;
  if( matrix!=NULL && matrix->decrementReferenceCount()==0 )
  {
    // printf(" MatrixTransform::delete the matrix\n");
    delete matrix;
  }
  
}

MatrixTransform & MatrixTransform::
operator=( const MatrixTransform & X )
{
  if( MatrixTransform::className != X.getClassName() )
  {
    cout << "MatrixTransform::operator= ERROR trying to set a MatrixTransform = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->ComposeMapping::operator=(X);            // call = for derivee class, deep copy
  // *wdh* what is this? matrix = (MatrixMapping*)map2.mapPointer; 

  if( matrix==NULL )
  {
    matrix = new MatrixMapping();
    matrix->incrementReferenceCount();  // this indicates that the mapping was newed
  }
  *matrix = *X.matrix;  // deep copy.

  map2.reference(*matrix);  // set mapping in the Compose base class *wdh* 040315 

  return *this;
}

void MatrixTransform::
reset()
//===========================================================================
/// \brief  Reset the transformation to the identity.
//===========================================================================
{
  assert(matrix!=0);
  matrix->reset();
  mappingHasChanged();
}

void MatrixTransform::
rotate( const int axis, const real theta )
//===========================================================================
/// \brief  Perform a rotation about a given axis.
/// \param axis (input) : axis to rotate about (0,1,2)
/// \param theta (input) : angle in radians to rotate by.
//===========================================================================
{
  assert(matrix!=0);
  matrix->rotate(axis,theta);
  mappingHasChanged();
}

void MatrixTransform::
rotate( const RealArray & rotate, bool incremental /* =false */ )
//===========================================================================
/// \brief  Perform a "rotation" using a $3\times3$ matrix. This does not really have to
///   be a rotation. 
/// \param rotate (input): If incremental=false then the upper $3\times3$ portion of the $4\times4$ transformation
///     matrix will be replaced by the matrix {\tt rotate(0:2,0:2)}. Otherwise this rotation matrix
///     will mutliply the existing transformation.
/// \param incremental (input) : if true apply this rotation to the existing transformation,
///     otherwise replace the existing rotation.
//===========================================================================
{
  assert(matrix!=0);
  matrix->rotate(rotate,incremental);
  mappingHasChanged();
}


void MatrixTransform::
scale( const real scalex /* =1. */,
       const real scaley /* =1. */, 
       const real scalez /* =1. */, 
       bool incremental  /* =true */  )
//===========================================================================
/// \brief  Perform a scaling
/// \param scalex, scaley, scalez (input): Scale factors along each axis.
/// \param incremental (input) : if true then incrementally transform the 
///        existing mapping, other transform the original mapping.
//===========================================================================
{
  assert(matrix!=0);
  matrix->scale(scalex,scaley,scalez,incremental);
  mappingHasChanged();
}


void MatrixTransform::
shift( const real shiftx /* =0. */ , 
       const real shifty /* =0. */ ,
       const real shiftz /* =0. */, 
       bool incremental  /* =true */  )
//===========================================================================
/// \brief  Perform a shift.
/// \param shitx, shity, shitz (input): shifts along each axis.
/// \param incremental (input) : if true then incrementally transform the 
///        existing mapping, other transform the original mapping.
//===========================================================================
{
  assert(matrix!=0);
  matrix->shift(shiftx,shifty,shiftz,incremental);
  mappingHasChanged();
}


int MatrixTransform::
setMappingProperties()
// We also need to set the following properties that are not set by the ComposeMapping setMappings function.
{
//    setCoordinateEvaluationType( cylindrical,getCoordinateEvaluationType(cylindrical) );
//    setCoordinateEvaluationType( spherical  ,getCoordinateEvaluationType(spherical  ) );
  setCoordinateEvaluationType( cylindrical,map1.getCoordinateEvaluationType(cylindrical) ); // *wdh* 030401
  setCoordinateEvaluationType( spherical  ,map1.getCoordinateEvaluationType(spherical  ) );
  for( int axis=0; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      setTypeOfCoordinateSingularity(side,axis,map1.getTypeOfCoordinateSingularity(side,axis));
    }
  }
  return 0;
}



void MatrixTransform::
display( const aString & label) const
{
  cout << "Here is a Matrix Transform Mapping\n";
  Mapping::display();
  cout << "Here is the matrix Mapping in the Matrix Transform:\n";
  assert(matrix!=0);
  matrix->display();
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int MatrixTransform::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering MatrixTransform::get" << endl;

  subDir->get( MatrixTransform::className,"className" ); 
  if( MatrixTransform::className != "MatrixTransform" )
  {
    cout << "MatrixTransform::get ERROR in className!" << endl;
  }
  ComposeMapping::get(*subDir,"ComposeMapping"); 
  matrix=(MatrixMapping*)map2.mapPointer;
  if( matrix!=NULL )
    matrix->incrementReferenceCount();  // 000402 
  mappingHasChanged();
  delete subDir;
  return 0;
}

int MatrixTransform::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( MatrixTransform::className,"className" );
  ComposeMapping::put( *subDir,"ComposeMapping" );
  delete subDir;
  return 0;
}

Mapping *MatrixTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==MatrixTransform::className )
    retval = new MatrixTransform();
  return retval;
}

//==============================================================
#define CROSS(dest,v1,v2){ \
dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
dest[2]=v1[0]*v2[1]-v1[1]*v2[0];}

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2){ \
dest[0]=v1[0]-v2[0]; \
dest[1]=v1[1]-v2[1]; \
dest[2]=v1[2]-v2[2];}


/*
* A function for creating a rotation matrix that rotates a vector called
* "from" into another vector called "to".
* Input : from[3], to[3] which both must be *normalized* non-zero vectors
* Output: mtx[3][3] -- a 3x3 matrix in colum-major form
* Author: Tomas Moller, 1999
* Ref: Moller and Hughes, Journal of Graphics tools, Vol 4, no. 4, pp 1-4, 1999.
*/
void getMatrixToRotateOneVectorIntoAnother(real *from, real *to, real *mtx9)
{
// *wdh* #define M(row,col) mtx9[col*4+row]
#define M(row,col) mtx9[(row)+3*(col)]

  const real EPSILON = REAL_EPSILON*10.;
  
  real v[3];
  real e,h;
  CROSS(v,from,to);
  e=DOT(from,to);
  if(e>1.0-EPSILON) /* "from" almost or equal to "to"-vector? */
  {
/* return identity */
    M(0, 0)=1.0; M(0, 1)=0.0; M(0, 2)=0.0;
    M(1, 0)=0.0; M(1, 1)=1.0; M(1, 2)=0.0;
    M(2, 0)=0.0; M(2, 1)=0.0; M(2, 2)=1.0;
  }
  else if(e<-1.0+EPSILON) /* "from" almost or equal to negated "to"? */
  {
    real up[3],left[3];
    real invlen;
    real fxx,fyy,fzz,fxy,fxz,fyz;
    real uxx,uyy,uzz,uxy,uxz,uyz;
    real lxx,lyy,lzz,lxy,lxz,lyz;
/* left=CROSS(from, (1,0,0)) */
    left[0]=0.0; left[1]=from[2]; left[2]=-from[1];
    if(DOT(left,left)<EPSILON) /* was left=CROSS(from,(1,0,0)) a good
				  choice? */
    {
/* here we now that left = CROSS(from, (1,0,0)) will be a good
   choice */
      left[0]=-from[2]; left[1]=0.0; left[2]=from[0];
    }
/* normalize "left" */
    invlen=1.0/sqrt(DOT(left,left));
    left[0]*=invlen;
    left[1]*=invlen;
    left[2]*=invlen;
    CROSS(up,left,from);
/* now we have a coordinate system, i.e., a basis; */
/* M=(from, up, left), and we want to rotate to: */
/* N=(-from, up, -left). This is done with the matrix:*/
/* N*M^T where M^T is the transpose of M */
    fxx=-from[0]*from[0]; fyy=-from[1]*from[1]; fzz=-from[2]*from[2];
    fxy=-from[0]*from[1]; fxz=-from[0]*from[2]; fyz=-from[1]*from[2];

    uxx=up[0]*up[0]; uyy=up[1]*up[1]; uzz=up[2]*up[2];
    uxy=up[0]*up[1]; uxz=up[0]*up[2]; uyz=up[1]*up[2];

    lxx=-left[0]*left[0]; lyy=-left[1]*left[1]; lzz=-left[2]*left[2];
    lxy=-left[0]*left[1]; lxz=-left[0]*left[2]; lyz=-left[1]*left[2];
/* symmetric matrix */
    M(0, 0)=fxx+uxx+lxx; M(0, 1)=fxy+uxy+lxy; M(0, 2)=fxz+uxz+lxz;
    M(1, 0)=M(0, 1); M(1, 1)=fyy+uyy+lyy; M(1, 2)=fyz+uyz+lyz;
    M(2, 0)=M(0, 2); M(2, 1)=M(1, 2); M(2, 2)=fzz+uzz+lzz;
  }
  else /* the most common case, unless "from"="to", or "from"=-"to" */
  {
#if 0
/* unoptimized version - a good compiler will optimize this. */
    h=(1.0-e)/DOT(v,v);
    M(0, 0)=e+h*v[0]*v[0]; M(0, 1)=h*v[0]*v[1]-v[2]; M(0,
						       2)=h*v[0]*v[2]+v[1];
    M(1, 0)=h*v[0]*v[1]+v[2]; M(1, 1)=e+h*v[1]*v[1]; M(1,
						       2)h*v[1]*v[2]-v[0];
    M(2, 0)=h*v[0]*v[2]-v[1]; M(2, 1)=h*v[1]*v[2]+v[0]; M(2,
							  2)=e+h*v[2]*v[2];
#else
/* ...otherwise use this hand optimized version (9 mults less) */
    real hvx,hvz,hvxy,hvxz,hvyz;
    h=(1.0-e)/DOT(v,v);
    hvx=h*v[0];
    hvz=h*v[2];
    hvxy=hvx*v[1];
    hvxz=hvx*v[2];
    hvyz=hvz*v[1];
    M(0, 0)=e+hvx*v[0]; M(0, 1)=hvxy-v[2]; M(0, 2)=hvxz+v[1];
    M(1, 0)=hvxy+v[2]; M(1, 1)=e+h*v[1]*v[1]; M(1, 2)=hvyz-v[0];
    M(2, 0)=hvxz-v[1]; M(2, 1)=hvyz+v[0]; M(2, 2)=e+hvz*v[2];
#endif
  }
#undef M
}

#undef CROSS
#undef DOT
#undef SUB


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int MatrixTransform::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!MatrixTransform",
      "transform which mapping?",
      "scale",
      "shift",
      "rotate",
      "rotate one vector into another",
      "general transform",
      "edit untransformed mapping",
      "reset",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "check",
      "check inverse",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Transform a Mapping by scaling, shifting and rotating",
      "         Note that transformations are cummulative ",
      "transform which mapping? : choose the mapping to transform",
      "scale              : scale the mapping",
      "shift              : shift in space",
      "rotate             : rotate in space",
      "rotate one vector into another : perform a transform that rotates on vector into another",
      "general transform  : supply a general transform as a 3x3 matrix"
      "edit untransformed mapping : edit the untransformed mapping",
      "reset              : reset the transformation to be the identity",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "check inverse       : check the inverse of the mapping",
      "check               : chech the mapping",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 
  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  bool mappingChosen= matrix!=NULL;
  Mapping *mapPointer;

  // By default transform the last mapping in the list (if this mapping is unitialized)
  if( !mappingChosen )
  {
    if( mapInfo.mappingList.getLength()>0 )
      mapPointer=mapInfo.mappingList[mapInfo.mappingList.getLength()-1].mapPointer;
    else
    {
      cout << "MatrixTransfrom:ERROR: no mappings to transform!! \n";
      return 1;
    }
  }
  gi.appendToTheDefaultPrompt("MatrixTransform>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 
    if( answer=="transform which mapping?" )
    { // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(mappingName);
//        menu2[i]=mapInfo.mappingList[i].getClassName()+" : "+mapInfo.mappingList[i].getName(mappingName);
      menu2[num]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      // Here is the mapping to be rotated/scaled etc.
      if( mapNumber<0 )
      {
        gi.outputString("Error: unknown mapping to transform!");
        gi.stopReadingCommandFile();
	continue;
      }
      else if( mapInfo.mappingList[mapNumber].mapPointer==this )
      {
	cout << "MatrixTransform::ERROR: you cannot transform this mapping, this would be recursive!\n";
        continue;
      }
      else 
        mapPointer=mapInfo.mappingList[mapNumber].mapPointer;
      mappingHasChanged();
    }
    
    if( !mappingChosen || answer=="transform which mapping?")
    {
      // set dimensions of matrix mapping to match
      if( matrix==NULL )
      {
        matrix = new MatrixMapping();
        matrix->incrementReferenceCount();  // this indicates that the mapping was newed
      }
      matrix->setDomainDimension(mapPointer->getRangeDimension());  // set matrix domain and range dimensions
      matrix->setRangeDimension(mapPointer->getRangeDimension());
      // compose the mappings:
      setMappings(*mapPointer,*matrix);
      if( getName(mappingName)!="Transform" )
        setName(mappingName,aString("matrixTransform-")+mapPointer->getName(mappingName));
      setMappingProperties();
      mappingChosen=TRUE;
      plotObject=TRUE;
      mappingHasChanged();
    }
    else if( answer=="scale" ) 
    {
      real xScale=1.; real yScale=1.; real zScale=1.;
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale (default=(%e,%e)): ",
            xScale,yScale));
        if( line!="" ) sScanF(line,"%e %e",&xScale,&yScale);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale, zScale (default=(%e,%e,%e)): ",
            xScale,yScale,zScale));
        if( line!="" ) sScanF(line,"%e %e %e",&xScale,&yScale,&zScale);
      }
      matrix->scale(xScale,yScale,zScale);
      mappingHasChanged();
    }
    else if( answer=="shift" ) 
    {
      real xShift=0., yShift=0., zShift=0.;
      if( rangeDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift (default=(%e,%e)): ",
				    xShift,yShift));
	if( line!="" ) sScanF(line,"%e %e",&xShift,&yShift);
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift, zShift (default=(%e,%e,%e)): ",
				    xShift,yShift,zShift));
	if( line!="" ) sScanF(line,"%e %e %e",&xShift,&yShift,&zShift);
      }
      matrix->shift(xShift,yShift,zShift);
      mappingHasChanged();
    }
    else if( answer=="rotate" ) 
    {
      int rotationAxis=2;
      real rotationAngle=45., centerOfRotation[3]={0.,0.,0.};
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter the rotation angle(degrees) (default=%e): ",
          rotationAngle));
        if( line!="" ) sScanF(line,"%e",&rotationAngle);
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e): ",
          centerOfRotation[0],centerOfRotation[1]));
        if( line!="" ) sScanF(line,"%e %e",&centerOfRotation[0],&centerOfRotation[1]);
      }        
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter rotation angle(degrees) and axis to rotate about(0,1, or 2)"
				    "(default=(%e,%i)): ",rotationAngle,rotationAxis));
        if( line!="" ) sScanF(line,"%e %i",&rotationAngle,&rotationAxis);
	if( rotationAxis<0 || rotationAxis>2 )
	{
	  cout << "Invalid rotation axis = " << rotationAxis << endl;
	  continue;
	}
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e,%e): ",
				    centerOfRotation[0],centerOfRotation[1],centerOfRotation[2]));
        if( line!="" ) sScanF(line,"%e %e %e",&centerOfRotation[0],&centerOfRotation[1],
                              &centerOfRotation[2]);
      }
      matrix->shift(-centerOfRotation[0],-centerOfRotation[1],-centerOfRotation[2]);
      matrix->rotate(rotationAxis,rotationAngle*Pi/180.);
      matrix->shift(+centerOfRotation[0],+centerOfRotation[1],+centerOfRotation[2]);
      mappingHasChanged();
    }
    else if( answer=="rotate one vector into another" )
    {
      printF("Create a matrix transform that will rotate vector v1=(v1x,v1y,v1z) into vector v2=(v2x,v2y,v2z)\n");
      printF("NOTE: this transformation will be incremental. Select 'reset' first to over-ride this\n");
      real v1[3]={1.,0.,0.}, v2[3]={0.,1.,0.};
      gi.inputString(line,sPrintF(buff,"Enter v1x,v1y,v1z, v2x,v2y,v2z"));
      sScanF(line,"%e %e %e %e %e %e",&v1[0],&v1[1],&v1[2], &v2[0],&v2[1],&v2[2] );

      RealArray m(3,3);
      m=0.; m(0,0)=m(1,1)=m(2,2)=1.;

      real norm=1./max(REAL_MIN*100.,sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]));
      v1[0]*=norm; v1[1]*=norm; v1[2]*=norm;
      norm=1./max(REAL_MIN*100.,sqrt(v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2]));
      v2[0]*=norm; v2[1]*=norm; v2[2]*=norm;
      

      getMatrixToRotateOneVectorIntoAnother(v1,v2,&m(0,0));
      
      bool incremental=true;
      matrix->rotate(m,incremental);
      mappingHasChanged();
    }
    else if( answer=="general transform" )
    {
      printF("Enter a general 3x3 matrix transform:\n");
      printF("          [ a11 a12 a13 ]\n");
      printF("      A = [ a21 a22 a23 ]\n");
      printF("          [ a31 a32 a33 ]\n");
      printF("NOTE: this transformation will be incremental. Select 'reset' first to over-ride this\n");
      RealArray m(3,3);
      m=0.; m(0,0)=m(1,1)=m(2,2)=1.;
      gi.inputString(line,sPrintF(buff,"Enter 9 values: a11,a12,a13, a21,a22,a23, a31,a32,a33"));
      sScanF(line,"%e %e %e %e %e %e %e %e %e",&m(0,0),&m(1,0),&m(2,0), &m(0,1),&m(1,1),&m(2,1), 
                                               &m(0,2),&m(1,2),&m(2,2));

      bool incremental=true;
      matrix->rotate(m,incremental);
      mappingHasChanged();
    }
    else if( answer=="edit untransformed mapping" )
    {
      map1.update(mapInfo);
    }
    else if( answer=="reset")
    {
      matrix->reset();
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
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
             answer=="periodicity"  ||
             answer=="check" ||
             answer=="check inverse" )
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
      gi.stopReadingCommandFile();
    }

    if( plotObject && mappingChosen )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
