#include "BodyDefinition.h"

//\begin{>BodyDefinitionInclude.tex}{\subsection{constructor}}
BodyDefinition::
BodyDefinition()
// ================================================================================================
// /Description:
//   Default constructor.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  initialize();
}


BodyDefinition::
~BodyDefinition()
{
}

BodyDefinition::
BodyDefinition( const BodyDefinition & bd )
// Copy constructor is deep by default
{
  *this=bd;
}

BodyDefinition & BodyDefinition::
operator =( const BodyDefinition & x0 )
{
  numberOfSurfaces=x0.numberOfSurfaces;
  maximumNumberOfFaces=x0.maximumNumberOfFaces;
  surfaceIdentifier=x0.surfaceIdentifier;
  numberOfFaces.redim(0);
  numberOfFaces=x0.numberOfFaces;
  boundaryFaces.redim(0);
  boundaryFaces=x0.boundaryFaces;
  
  return *this;
}


int BodyDefinition::
initialize()
{
  className="BodyDefinition";
  
  maximumNumberOfFaces=0;
  numberOfSurfaces=0;

  return 0;
}


//\begin{>>BodyDefinitionInclude.tex}{\subsection{defineSurface}}
int BodyDefinition::
defineSurface(const int & surfaceNumber, const int & numberOfFaces_, IntegerArray & boundary )
// ================================================================================================
// /Description:
//    Specify the sides of grids that define a "surface". A surface represents some subset of the
//  boundary of an entire domain. For example, for the sphere-in-a-box grid a surface could
//  represent the surface of the sphere. To define a surface you must supply:
//
// /surfaceNumber (input) : a surface identifier. This value must be bigger than or equal to zero. Normally surfaces
//  should be numbered starting from zero.
// /numberOfFaces (input) : the number of faces that make up the surface.
// /boundary (input): boundary(3,numberOfFaces) : (side,axis,grid)=boundary(0:2,i) i=0,1,...numberOfFaces-1.
//   To define a surface you must supply a list of sides of grids.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  if( surfaceNumber<0 )
  {
    printf("BodyDefinition::defineSurface:ERROR: The surfaceNumber=%i is less than zero.\n",surfaceNumber);
    Overture::abort("error");
  }
  // first try to find the surfaceNumber in the list of surfaceIdentifier's
  int surface=-1;
  int i;
  for( i=0; i<numberOfSurfaces; i++ )
  {
    if( surfaceIdentifier(i)==surfaceNumber )
    {
      surface=i;
      break;
    }
  }
  if( surface<0 )
  {
    surface=numberOfSurfaces;
    numberOfSurfaces++;   // this is a new surface

    if( numberOfSurfaces>surfaceIdentifier.getLength(0) )
    {
      const int oldDim=surfaceIdentifier.getLength(0);
      const int newDim=oldDim+10;
      
      surfaceIdentifier.resize(newDim);
      numberOfFaces.resize(newDim);

//      surfaceWeightsDefined.resize(newDim);
//      surfaceWeightsDefined(Range(oldDim,newDim-1))=FALSE;
    }
  }

  maximumNumberOfFaces=max(maximumNumberOfFaces,numberOfFaces_);
  
  if( maximumNumberOfFaces>boundaryFaces.getLength(1) || numberOfSurfaces>boundaryFaces.getLength(2) )
  {
    boundaryFaces.resize(3,maximumNumberOfFaces,numberOfSurfaces+10);

  }
  
  surfaceIdentifier(surface)=surfaceNumber;
  numberOfFaces(surface)=numberOfFaces_;
  Range R3(0,2), N(0,numberOfFaces_-1);
  boundaryFaces(R3,N,surface)=boundary(R3,N);
  
  // Check for duplicate faces -- this is an error
  for( int f=0; f<numberOfFaces(surface); f++ )
  { // compare face f to all higher numbered faces
    for( int f2=f+1; f2<numberOfFaces(surface); f2++ )
    {
      if( max(abs(boundary(R3,f)-boundary(R3,f2)))==0 )
      {
	printf("BodyDefinition::ERROR:defineSurface: The surface defintion has a duplicate face!\n"
               "   face %i (side,axis,grid)=(%i,%i,%i) is the same as face %i. \n",
               f,boundary(0,f),boundary(1,f),boundary(2,f),f2);
	Overture::abort("error");
      }
    }
  }

  return 0;
}

//\begin{>>BodyDefinitionInclude.tex}{\subsection{defineSurface}}
int BodyDefinition::
getSurface(const int & surfaceNumber, int & numberOfFaces_, IntegerArray & boundary ) const
// ================================================================================================
// /Description:
//    Return the faces that form a surface. A surface represents some subset of the
//  boundary of an entire domain. For example, for the sphere-in-a-box grid a surface could
//  represent the surface of the sphere. To define a surface you must supply:
//
// /surfaceNumber (input) : an existing surface identifier.
//  should be numbered starting from zero.
// /numberOfFaces (output) : the number of faces that make up the surface.
// /boundary (output): boundary(3,numberOfFaces) : (side,axis,grid)=boundary(0:2,i) i=0,1,...numberOfFaces.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  int surface = surfaceIndex(surfaceNumber);
  if( surface<0 )
  {
    printf("BodyDefinition::getSurface:ERROR: Unknown surfaceNumber=%i.\n",surfaceNumber);
    return 1;
  }
  numberOfFaces_=numberOfFaces(surface);
  boundary.redim(3,numberOfFaces_);
  Range R3(0,2), N(0,numberOfFaces_-1);
  boundary(R3,N)=boundaryFaces(R3,N,surface);

  return 0;
}


int BodyDefinition::
getSurfaceNumber( const int surface ) const
// ================================================================================================
// /Description:
//    Return the surfaceNumber (surface ID) that corresponds to the surface numbered "surface". 
// /Return value: the surfaceNumber (if found) otherwise -1
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  int surfaceNumber=-1;
  for( int s=surfaceIdentifier.getBase(0); s<surfaceIdentifier.getBound(0); s++ )
  {
    if( surfaceIdentifier(s)==surface )
    {
      surfaceNumber=s;
      break;
    }
  }
  return surfaceNumber;
}



//\begin{>>BodyDefinitionInclude.tex}{\subsection{numberOfFacesOnASurface}}
int BodyDefinition::
numberOfFacesOnASurface(int surfaceNumber) const
// ================================================================================================
// /Description:
//    Return the number of faces that form a given surface
// /Return value: number of faces for surface.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  if( surfaceNumber>=0 && surfaceNumber<numberOfSurfaces )
    return numberOfFaces(surfaceNumber);
  else
  {
    printf("BodyDefinition::numberOfFacesOnASurface:ERROR: invalid surfaceNumber=%i\n",surfaceNumber);
    return -1;
  }
}

//\begin{>>BodyDefinitionInclude.tex}{\subsection{getFace}}
int BodyDefinition::
totalNumberOfSurfaces() const
// ================================================================================================
// /Description:
//    Return the number of surfaces that have been defined.
// /Return value: total number of surfaces defined.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  return numberOfSurfaces;
}



//\begin{>>BodyDefinitionInclude.tex}{\subsection{getFace}}
int BodyDefinition::
getFace(int surfaceNumber,int face, int & side, int & axis, int & grid) const
// ================================================================================================
// /Description:
//    Return the data for a particular face of a surface.
// /side,axis,grid (output): this face corresponds to these values.
// /Return value: 0 for success.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  if( surfaceNumber>=0 && surfaceNumber<numberOfSurfaces )
  {
    if( face>=0 && face<numberOfFaces(surfaceNumber) )
    {
      side = boundaryFaces(0,face,surfaceNumber);
      axis = boundaryFaces(1,face,surfaceNumber);
      grid = boundaryFaces(2,face,surfaceNumber);    
      return 0;
    }
    else
    {
      printf("BodyDefinition::getFace:ERROR: invalid face=%i for surfaceNumber=%i\n",face,surfaceNumber);
      return -1;
    }
  }
  else
  {
    printf("BodyDefinition::getFace:ERROR: invalid surfaceNumber=%i\n",surfaceNumber);
    return -1;
  }
}


//\begin{>>BodyDefinitionInclude.tex}{\subsection{surfaceIndex}}
int BodyDefinition::
surfaceIndex( int surfaceNumber ) const
// ================================================================================================
// /Access level: protected.
// /Description:
//   For a given surfaceNumber determine the surfaceIndex such that
//   surfaceIdentifier(surfaceIndex)==surfaceNumber. Return -1 if no match is found.
// /surfaceNumber (input) : the surface ID for a user defined surface.
// /Return value: the index into the surfaceIdentifier array, or -1 if no match exists.
//\end{BodyDefinitionInclude.tex}
// ================================================================================================
{
  int i, surfaceIndex=-1;
  for( i=0; i<numberOfSurfaces; i++ )
  {
    if( surfaceIdentifier(i)==surfaceNumber )
    {
      surfaceIndex=i;
      break;
    }
  }
  return surfaceIndex;
}






// =====================================================================================
/// \brief Get the Integrate object from the directory "name" of the data base.
// =====================================================================================
int BodyDefinition::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  // subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( className,"className" ); 

  subDir.get( numberOfSurfaces,"numberOfSurfaces" ); 
  subDir.get( maximumNumberOfFaces,"maximumNumberOfFaces" ); 

  subDir.get( surfaceIdentifier,"surfaceIdentifier" ); 
  subDir.get( numberOfFaces,"numberOfFaces" ); 
  subDir.get( boundaryFaces,"boundaryFaces" ); 
  
  delete & subDir;
  return true;
}


// =====================================================================================
/// \brief Put this Integrate object in a sub-directory called "name" of the data base
// =====================================================================================
int BodyDefinition::
put( GenericDataBase & dir, const aString & name) const
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  // subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( className,"className" );

  subDir.put( numberOfSurfaces,"numberOfSurfaces" ); 
  subDir.put( maximumNumberOfFaces,"maximumNumberOfFaces" ); 

  subDir.put( surfaceIdentifier,"surfaceIdentifier" ); 
  subDir.put( numberOfFaces,"numberOfFaces" ); 
  subDir.put( boundaryFaces,"boundaryFaces" ); 

  delete &subDir;
  return true;
}

