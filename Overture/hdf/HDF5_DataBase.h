#ifndef HDF5_DATABASE_H 
#define HDF5_DATABASE_H "HDF_DataBase.h"

#include "GenericDataBase.h"



//--------------------------------------------------------------------------
//
//  HDF_DataBase: This is a class to support access to and from a data-base.
//    This class knows how to get and put the types
//       o int, float, double, aString
//    as well as A++ arrays
//       o intArray, floatArray, doubleArray
//    as well as
//       o "c" arrays of Strings.
//
//  This implementation acts as a front end to the HDF Library from NCSA
//
//--------------------------------------------------------------------------


class DataBaseBuffer;

class HDF_DataBase : public GenericDataBase
{
public:
  HDF_DataBase();
  HDF_DataBase(const HDF_DataBase &);
  HDF_DataBase(const GenericDataBase &);
  virtual GenericDataBase* virtualConstructor() const;
  ~HDF_DataBase();
  HDF_DataBase & operator=(const HDF_DataBase & );
  GenericDataBase & operator=(const GenericDataBase & );

  // open a data-base file
  int mount(const aString & fileName, const aString & flags = "I");
  // flush data, close the file
  int unmount();
  // flush the data to the file 
  int flush();
  
  virtual int getID() const;  // get the identifier for this directory

  // build a directory with the given ID
  virtual int build(GenericDataBase & db, int id);

  int isNull() const;

  // create a sub-directory
  int create(GenericDataBase & db, const aString & name, const aString & dirClassName );
  // link a new directory to an existing directory
  int link(GenericDataBase & db, const HDF_DataBase & parent, const aString & name);
  // find a sub-directory (crash if not found)
  int find(GenericDataBase & db, const aString & name, const aString & dirClassName="directory") const;
  // locate a sub-directory
  int locate(GenericDataBase & db, const aString & name, const aString & dirClassName="directory") const;

  // find the names of all objects with a given class-name
  virtual int find(aString *name, const aString & dirClassName, 
                      const int & maxNumber, int & actualNumber) const;
  // find all sub-directories (and their names) with a given class-name
  virtual int find(GenericDataBase *db, aString *name, const aString & dirClassName, 
                      const int & maxNumber, int & actualNumber) const;

  virtual int turnOnWarnings();
  virtual int turnOffWarnings();

  // put a float/int/double/aString
  int put( const float & x, const aString & name );
  int put( const double & x, const aString & name );
  int put( const int & x, const aString & name );
  int put( const aString & x, const aString & name );
#ifdef  OV_BOOL_DEFINED
  int put( const bool & x, const aString & name );
#endif

  // get a float/int/double/aString
  int get( float & x, const aString & name ) const;
  int get( double & x, const aString & name ) const;
  int get( int & x, const aString & name ) const;
  int get( aString & x, const aString & name ) const;
#ifdef  OV_BOOL_DEFINED
  int get( bool & x, const aString & name ) const;
#endif

  // put a float/int/double A++ array
  virtual int put( const floatSerialArray & x, const aString & name );
  virtual int put( const doubleSerialArray & x, const aString & name );
  virtual int put( const intSerialArray & x, const aString & name );

  virtual int putDistributed( const floatArray & x, const aString & name );
  virtual int putDistributed( const doubleArray & x, const aString & name );
  virtual int putDistributed( const intArray & x, const aString & name );

  // get a float/int/double A++ array
  virtual int get( floatSerialArray & x, const aString & name, Index *Iv=NULL ) const;
  virtual int get( doubleSerialArray & x, const aString & name, Index *Iv=NULL ) const;
  virtual int get( intSerialArray & x, const aString & name, Index *Iv=NULL ) const;

  virtual int getDistributed( floatArray & x, const aString & name ) const;
  virtual int getDistributed( doubleArray & x, const aString & name ) const;
  virtual int getDistributed( intArray & x, const aString & name ) const;

  // put/get a "c" array of float/int/double/Strings 
  int put( const int    x[],const aString & name, const int number ); 
  int put( const float  x[],const aString & name, const int number ); 
  int put( const double x[],const aString & name, const int number ); 
  int put( const aString x[],const aString & name, const int number ); 

  int get( int    x[], const aString & name, const int number ) const;
  int get( float  x[], const aString & name, const int number ) const;
  int get( double x[], const aString & name, const int number ) const;
  int get( aString x[], const aString & name, const int number ) const;

  // output statistics on the file such as number of vgroups, vdatas
  void printStatistics() const;

  virtual void setMode(const InputOutputMode & mode=normalMode);

  int putSM( const floatArray & x, const aString & name );
  int getSM( floatArray & x, const aString & name );

	
static int debug;


protected:

  enum AcessModeEnum
  {
    none,
    write,
    read
  } accessMode;


  // close the directory, flush data
  int close();
  void reference( const HDF_DataBase & db );
  void destroy();

  bool bufferWasCreatedInThisDirectory; // indicates when stream buffers were really opened

  void closeStream() const;  // close the stream buffers, if they are open (not really const!)

  int closeLocalFile(int p, AcessModeEnum accessMode ) const;

  int getFileGroup(int p, AcessModeEnum accessMode ) const;

  aString getSerialArrayName( const aString & fullGroupPath, const aString & name, const int p ) const;

  aString getSerialFileName( const int p, AcessModeEnum accessMode ) const ;

  HDF_DataBase* openLocalFile(int p, AcessModeEnum accessMode ) const;

  int    fileID;  	 // file identifier
  aString fileName;      // name of the file 
  aString fullGroupPath; // full path from root directory to current file name
  int processorForWriting;  // only one processor writes scalars to the file.

  DataBaseBuffer *dataBaseBuffer;
  mutable HDF_DataBase **serialDataBase;  // a separate data-base file for local arrays (multipleFileIO option)
};

#endif
