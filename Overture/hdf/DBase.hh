#ifndef __DBASE_HH__
#define __DBASE_HH__

#include <string>
#include <map>

#include "kk_defines.hh"
#include "kk_ptr.hh"

/// a simple variable database for a simple code
namespace DBase
{

  /// Exception class for the simple database
  class DBErr : public KK::Err
  {
  public:
    DBErr(std::string s="") : KK::Err(s) { }
  };

  /// traits for each database entry, currently just examples
  enum EntryTrait {
    persistent=0x1, /**< persistent data */
    distributed=persistent<<1 /**< distributed data */
  };

  /// a base class for all entries into the database
  class Entry
  {
  public:
    /// create a new entry with name name, no traits are assigned
    Entry() : traits(0x0) { }

    /// clean up the entry (nothing is done in the base class)
    virtual ~Entry() { }

    /// traits for a given entry
    unsigned int traits;
    
  };

  /// the actual container class that hold data in the database
  template<typename T>
  class SpecializedEntry : public Entry
  {
  public:
    /// create a new entry with data and a name
    /** \param e a COPY of this data will be stored in the database
     *  \param dd an optional callback to destroy the data upon deletion of this entry 
     */
    EXPLICIT SpecializedEntry(const T &e, void (*dd)(T &)=0) : 
      Entry(), entry(e), destroy_data(dd) { }

    EXPLICIT SpecializedEntry(void (*dd)(T &)) : Entry(), destroy_data(dd) { }

    EXPLICIT SpecializedEntry() : Entry(), destroy_data(0) { }

    /// clean up this instance, calling the entry's destructor callback if present
    virtual ~SpecializedEntry() 
    { if (destroy_data) destroy_data(entry); }

    /// return a reference to the data
    T & operator*() { return entry; }

    /// return a const reference to the data
    const T & operator*() const { return entry; }

  private:

    /// the copy constructor is currently meaningless/inaccessible
    SpecializedEntry(SpecializedEntry<T> &t) { }

    /// the data stored in this database node
    T entry;

    /// optional destructor callback for this entry
    void (*destroy_data)(T &);

  };

  typedef KK::sptr<Entry> EntryP;

  template<typename T> 
  bool can_cast_entry(Entry &e)
  { return dynamic_cast< SpecializedEntry< T > * >(&e); }

  template<typename T> 
  bool can_cast_entry(EntryP &e)
  { return KK::sptr_dynamic_cast< SpecializedEntry<T> >(e); }

  template<typename T> 
  T & cast_entry(Entry &e)
  { 
    KK::Assert<DBErr> ( can_cast_entry<T>(e), " cannot cast entry to mismatched type");
    SpecializedEntry<T> & p = dynamic_cast< SpecializedEntry< T > & > (e);
    return *p;
  }

  template<typename T> 
  T & cast_entry(EntryP &e)
  { 
    KK::Assert<DBErr> ( can_cast_entry<T>(e), " cannot cast entry to mismatched type");
    SpecializedEntry<T> & p = *KK::sptr_dynamic_cast< SpecializedEntry< T > > (e);
    return *p;
  }

  /// the actual database class, uses a stl map for actual data storage/retrieval
  class DataBase
  {
    /// the actual container class
    typedef std::map< std::string, DBase::EntryP > dbtype;
    //    typedef std::map< std::string, Entry *> dbtype;

  public:
    /// create an empty database
    DataBase() { }

    /// copy a database
    /** \param d database to be copied */
    DataBase(const DataBase &d) : db(d.db) { }

    /// destroy a database and its information
    virtual ~DataBase() 
    { 
      clear();
    }

    /// destroy a databae and its information
    virtual void clear()
    {

      // sptr now takes care of deletes
//       for ( dbtype::iterator i=this->db.begin(); i!=this->db.end(); i++ )
// 	//	if ( (*i).second !=NULL ) 
// 	if ( (*i).second  ) 
// 	  delete (*i).second;
// 	else
// 	  throw DBErr();

      this->db.clear();
    }

    /// put an item into the database, return a reference to the data
    /** \param name  the name of the entry in the database
     *  \param entry a COPY of this data will be stored in the database
     *  \param entry_destructor an optional callback to destroy the data
     *
     *  An entry containing a COPY of the data is
     *  placed into the database.  If entry is not provided, a new
     *  instance of the specified type is created using the type's default
     *  constructor.  An optional
     *  destructor callback for entry can be provided by passing 
     *  a pointer to a function returning void and taking one argument
     *  of type reference to T.
     *
     *  The method returns a reference to the data stored in the database.
     *
     *  sample usage:
     *  DBase::DataBase db;
     *  int &i = db.put("i",10); // create an int entry named "i" with value 10
     *  int &j = db.put<int>("j"); // create an uninitialized int at entry "j"
     *  // below, place a pointer to a Foo object and provide a destructor
     *  db.put("a foo object ptr", aFooObjectPtr, fooOpjectDestructor);
     */
    template<typename T>
    inline
    T &
    put(std::string name, const T &entry, void (*entry_destructor)(T&)=0) 
    {
      KK::Assert<DBErr> ( name!="", "cannot use empty name" );
      KK::Assert<DBErr> ( this->db.count(name)==0, "name "+name+" already in use!");
      KK::sptr< SpecializedEntry<T> > spece = new SpecializedEntry<T>(entry,entry_destructor); 
      this->db[name] = KK::sptr_dynamic_cast<Entry>( spece );
      //      this->db[name] = dynamic_cast<Entry *>( spece );

      return get<T>(name);
    }

    /// This version of put does not initialize the entry
    template<typename T>
    inline
    T &
    put(std::string name, void (*entry_destructor)(T&)=0) 
    {
      KK::Assert<DBErr> ( name!="", "cannot use empty name" );
      KK::Assert<DBErr> ( this->db.count(name)==0, "name "+name+" already in use!");
      KK::sptr< SpecializedEntry<T> > spece = new SpecializedEntry<T>(entry_destructor); 
      this->db[name] = KK::sptr_dynamic_cast<Entry>( spece );
      //      this->db[name] = dynamic_cast<Entry *>( spece );

      return get<T>(name);
    }

    /// get a reference to something in the database
    template<typename T>
    inline
    T &
    get(std::string name)
    {
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );

      KK::sptr< SpecializedEntry<T> > p = KK::sptr_dynamic_cast< SpecializedEntry< T >  > (this->db[name]);
      //      SpecializedEntry<T> * p = dynamic_cast< SpecializedEntry< T > * > (this->db[name]);

      KK::Assert<DBErr> ( p , "type mismatch for variable "+name );
      return **p;
    }

    /// get a const reference to something in the database
    template<typename T>
    inline
    const T &
    get(std::string name) const
    {
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );

      KK::sptr< SpecializedEntry<T> > p = KK::sptr_dynamic_cast< SpecializedEntry< T >  > (this->db[name]);
      //      SpecializedEntry<T> * p = dynamic_cast< SpecializedEntry< T > * > (this->db[name]);

      KK::Assert<DBErr> ( p , "type mismatch for variable "+name );

      return **p;
    }

    inline
    const KK::sptr< Entry >
    getEntry(std::string name) const
    {
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );
      return this->db[name];
    }

    /// remove an item from the database
    void
    remove(std::string name) 
    {
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );
      
      //sptr takes care of this      if ( this->db[name]!=NULL ) delete this->db[name];
      
      this->db.erase(name);
    }

    /// toggle a trait for a particular entry, default state is off
    void toggleTrait(std::string name, int t)
    { 
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );
      
      if ( this->db[name]->traits & t )
	this->db[name]->traits ^= t;
      else
	this->db[name]->traits |= t;

    }

    /// return true if the trait t is active for entry name
    bool traitActive(std::string name, int t)
    {
      KK::Assert<DBErr> ( this->db.count(name)==1, "name "+name+" not in database!" );
      
      return this->db[name]->traits & t;
    }

    /// return true if the database has an entry with this name
    bool has_key(std::string name) const { return this->db.count(name)!=0; }

    /// return the number of entries in the database
    size_t size() const { return db.size(); }

    /// typedef for function to apply to all elements
    typedef void (*EntryProcessingFunction)(std::string , Entry &);

    /// process all entries with the function func
    void processEntries(EntryProcessingFunction func)
    {
      for ( dbtype::iterator i=this->db.begin(); i!=this->db.end(); i++ )
	func((*i).first, *((*i).second));
    }

    /// process all entries with a function class
    template<class F>
    inline
    void processEntries(F &func)
    {
      for ( dbtype::iterator i=this->db.begin(); i!=this->db.end(); i++ )
	func((*i).first, *((*i).second));
    }

    /// process all entries with a function class
    template<class F>
    inline
    void processEntries(const F &func)
    {
      for ( dbtype::iterator i=this->db.begin(); i!=this->db.end(); i++ )
	func((*i).first, *((*i).second));
    }

    /// merge the contents of a database into this one; if there is a name conflict throw an error
    void merge( DataBase &db_in ) throw(DBErr)
    {
      for ( iterator n=db_in.begin(); n!=db_in.end(); n++ )
	{
	  if ( !has_key( n->first ) )
	    db.insert(*n);
	  else
	    throw DBErr("cannot merge databases with duplicate entries");
	}
    }

    /// link an entry of one database to an entry in the current database with a new name.
    /** link allows the manual merge of two databases when there are possible name conflicts. The
     *  user can provide a new name to use in the current instance.
     **/
    void link( DataBase &from, std::string from_nm, std::string to_nm )
    {
      KK::Assert<DBErr> ( !has_key(to_nm), "name "+to_nm+" already in use!");
      KK::Assert<DBErr> ( from.has_key(from_nm), "name "+to_nm+" not in link database!");

      db[to_nm] = from.db[from_nm];
    }

    /// link an entry pointer to an entry in the current database with a new name.
    /** link allows the manual merge of two databases when there are possible name conflicts. The
     *  user can provide a new name to use in the current instance.
     **/
    void link( DBase::EntryP &from, std::string to_nm )
    {
      KK::Assert<DBErr> ( !has_key(to_nm), "name "+to_nm+" already in use!");

      db[to_nm] = from;
    }

    typedef dbtype::iterator iterator;
    typedef dbtype::const_iterator const_iterator;

    iterator begin() { return db.begin(); }
    const_iterator begin() const { return db.begin(); }
    iterator end() { return db.end(); }
    const_iterator end() const { return db.end(); }

  private:
    mutable dbtype db;
  };
}

#endif
