#ifndef OV_ENTITY_TAG_H
#define OV_ENTITY_TAG_H

#include <string>

class GenericDataBase;
class aString;

/** EntityTag can be used to manage generic tagging information for UnstructuredMappings.
 *  It will either shallow or deep copy the data given to it.  Also, it is usefull as a 
 *  base class for user defined tag types.
 */
class EntityTag {

public:
  /// the default constructor initialized the EntityTag to a nil tag
  EntityTag( const std::string name="", const void *data=0, const bool copy_data=false, const int data_size=0 )
    : tagName(""), tagData(0), copyData(false) , dataSize(0)
  { this->setData( name , data, copy_data, data_size ); }
  
  /// a simple copy constructor; note it will be deep or shallow depending on the value of et.copyData
  EntityTag( const EntityTag & et ) : tagName(""), tagData(0), copyData(false) , dataSize(0)
  { this->setData( et.tagName , et.tagData, et.copyData, et.dataSize ); }

  virtual ~EntityTag() 
  { destroy(); }

  /// return true if this tag has nothing stored in it
  inline bool is_nil() const { return tagName=="" && tagData==0; }

  /// set the data in the tag; note this will be a shallow or deep copy depending on the value of copy_data
  virtual int setData( const std::string name, const void *data, const bool copy_data=false, const int data_size=0 );
  /// obtain a pointer to the data and the name of the tag
  virtual int getData( void * &data, std::string &name );
  /// return a const pointer to the data
  virtual const void * getData() const { return tagData; }
  /// obtain the storage size of the data
  virtual int getDataSize() const { return dataSize; }
  /// copiesData returns true if the tag performs a deep copy of the data it stores
  bool copiesData() const { return copyData; }
  
  /// obtain the name of the tag, this name can be different for each instance
  std::string getName() const { return tagName; }
  /// obtain the classname of the tag; this name is the same for all instances
  virtual std::string getClassName() const { return className; }

  /// put to an Overture GenericDataBase
  virtual int get( const GenericDataBase & dir, const aString & name );    // get from a database file
  /// get from an Overture GenericDataBase
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file
  
protected:
  void destroy() 
  {
    if ( tagData && copyData )
      delete [] (char *)tagData;

    dataSize = 0;
    tagData = 0;
  }

private:
  
  static std::string className;
  std::string tagName;

  void *tagData;
  bool copyData;
  int dataSize;
  
};

#endif
