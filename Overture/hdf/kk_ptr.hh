#ifndef KK_PTR
#define KK_PTR

#include <iostream>
#include <typeinfo>

#ifdef KK_PTR_DEBUG

using std::cout;
using std::endl;

#define KK_PTR_OUT(x) cout<<x;

#ifndef KK_DEBUG
#define KK_DEBUG
#endif

#else

#define KK_PTR_OUT(x) 

#endif

#include "kk_defines.hh"

//#define KK_USE_VPTR

namespace KK {

  /// acquire a "global" id, this will be used interally by KK::sptr
  long int acquire_gid();

  /// release a "global" id allowing it to be used again
  void release_gid(long int gid);

  /// error class for sptr
  class sptr_Err : public KK::Err {};

#ifdef KK_USE_VPTR
  /// vptr is a base class that allows one to pass references to sptr's around (like a void sptr)
  class vptr 
  {
  public:
    virtual ~vptr(){};
    
    virtual operator bool() const=0;
    virtual long int GID() const=0;

  };
#endif

  /// MY smart pointer class (not yours!)
  /** KK::sptr is a smart pointer class implemented using reference
   *  counting (heap allocated int). The object pointed to is destructed
   *  when the reference count goes to zero.  A dynamic_cast mechanism
   *  is provided;  most of the methods throw sptr_Err in the event of
   *  an unrecoverable error.  Invalid casts return a null sptr 
   *  (count==0, data==0) 
   */
  template<typename T>
  class sptr 
#ifdef KK_USE_VPTR
: public vptr
#endif
  {
    template<typename TO, typename FROM> friend sptr<TO> sptr_cast( sptr<FROM> &p );
    template<typename TO, typename FROM> friend sptr<TO> sptr_static_cast( sptr<FROM> &p );
    template<typename TO, typename FROM> friend sptr<TO> sptr_dynamic_cast( sptr<FROM> &p );
  public:
    
    /// initialize the pointer using a regular pointer (or null if none is provided)
    inline sptr(T*d_=0) : data(d_), count(0), gid(-1)
    { 
      if (data) 
	{
	  gid=acquire_gid(); 
	  increment();
	}
      KK_PTR_OUT("built sptr for address "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);
    }
    
    /// make a new reference a pointer of the same type (be carefull if the pointer is assigned to itself)
    inline sptr( const sptr<T> &p ) : data(p.data), count(p.count), gid(p.gid)
    {
      if ( data ) increment();
      KK_PTR_OUT("built sptr for address "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);
    }

    /// decrement and destroy the pointer
    inline
    ~sptr()
    {
      KK_PTR_OUT("del sptr for address "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);
      if ( data )
      {
	if ( decrement()==0 )
	{
	  destroy();
	}
	else if ( (*count)<0 )
	{
	  throw sptr_Err();
	}
	else
	{
	}
      }

    }

    /// assignment operator, this will make the reference and increment the reference count
    inline sptr<T> & operator=( const sptr<T> & p )
    {
      if ( *this==p ) return *this;

      if ( data && decrement()==0 )
	destroy();

      data = p.data;
      count = p.count;
      gid = p.gid;

      if ( data ) increment();

      KK_PTR_OUT("opr= sptr for address "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);
      return *this;
    }

    /// boolean equals; two sptrs are the same if this->data==p.data && this->gid==p.gid
    /** boolean equals; two sptrs are the same if this->data==p.data && this->gid==p.gid. An error
     *  is thrown if (this->data==p.data && this->gid!=p.gid) or (this->data!=p.data && this->gid==p.gid)
     */
    inline bool operator==(const sptr<T> &p) const
    {
      KK::Assert<sptr_Err> ( (data==p.data && gid==p.gid) || (data!=p.data && gid!=p.gid), 
			     "ERROR: sptr_Err: sptr::== called but data and global id do not match!" );

      KK_PTR_OUT("this : "<<data<<"  "<<gid<<" ; p : "<<p.data<<"  "<<p.gid<<endl);
      return (data==p.data && gid==p.gid);
    }

    /// boolean not equals; two sptrs are the same if this->data==p.data && this->gid==p.gid
    /** boolean not equals; two sptrs are the same if this->data==p.data && this->gid==p.gid. An error
     *  is thrown if (this->data==p.data && this->gid!=p.gid) or (this->data!=p.data && this->gid==p.gid)
     */
    inline bool operator!=(const sptr<T> &p) const
    { return !(*this==p); }

    /// dereferencing operator for the pointer; an error is thrown if the pointer is null
    inline T* operator->()
    { 
      KK::Assert<sptr_Err> ( data, "ERROR: sptr_Err: sptr-> called for null pointer!" );
      return data; 
    }

    /// const dereferencing operator for the pointer; an error is thrown if the pointer is null
    inline const T* operator->() const
    { 
      KK::Assert<sptr_Err> ( data, "ERROR: sptr_Err: sptr-> called for null pointer!" );
      return data; 
    }

    /// dereferencing operator for the pointer; an error is thrown if the pointer is null
    inline T& operator*()
    { 
      KK::Assert<sptr_Err> ( data, "ERROR: sptr_Err: *sptr called for null pointer!" );
      return *data; 
    }

    /// const dereferencing operator for the pointer; an error is thrown if the pointer is null
    inline const T& operator*() const
    { 
      KK::Assert<sptr_Err> ( data, "ERROR: sptr_Err: *sptr called for null pointer!" );
      return *data; 
    }

    /// return the raw pointer; use it wisely!
    inline const T* raw() const 
    { return data; }

    /// conversion to bool; the result is true if data!=0 && gid>=-1
    inline operator bool() const
    { 
      KK::Assert<sptr_Err>( (data==0&&gid==-1)||(data!=0 && gid>-1) );
      return (data!=0 && gid>-1);
    }

    /// return the global id
    inline long int GID() const { return gid; }

    /// return the number of references
    inline int nRefs() const { return count ? *count : 0; }

  protected:

    /// a "special" constructor used by sptr_cast that takes a raw pointer and reference count
    sptr( T* rp, int * start_count, long int gid_) : data(rp), count(start_count), gid(gid_)
    {
      KK::Assert(start_count>0, "ERROR : protected sptr constructor : invalid initial count");

      if ( data ) increment();
    }

    /// increment the reference count after checking to see if the pointer makes sense
    inline int increment() 
    { 
      // data has to be here for the increment to be valid (a null sptr should never get incremented)
      KK::Assert<sptr_Err>( data, "ERROR: sptr_Err: sptr::increment called with no data!" );
      
      if ( !count ) 
	{
	  count = new int;
	  (*count) = 0;
	}

      KK_PTR_OUT("count is at address "<<count<<endl);
      KK_PTR_OUT("incrementing "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);

      return ++(*count);
    }

    /// decrement the reference count after checking to see if the pointer makes sense
    inline int decrement()
    {
      // data has to be here for the increment to be valid (a null sptr should never get decremented)
      KK::Assert<sptr_Err>( data, "ERROR: sptr_Err: sptr::decrement called with no data!" );
      // check to make sure the reference count makes at least a little sense (>0)
      KK::Assert<sptr_Err>( count && (*count)>0, 
			    "ERROR: sptr_Err: sptr::decrement called with invalid reference count!" );
      KK_PTR_OUT("decrementing "<<data<<", gid "<<gid<<", count "<<(count ? (*count) : -1)<<endl);

      return --(*count);
    }

    inline void destroy()
    {
      KK::Assert<sptr_Err>( (*count)==0, "ERROR: sptr_Err: sptr::destroy called with nonzero count!" );
      delete data;
      delete count;
      data  = 0;
      count = 0;
      release_gid(gid);
    }

  private:
    T *data;
    int *count;
    long int gid;
  };

  /// cast from one pointer type to another; return a null ptr if the dynamic cast is invalid
  template<typename TO, typename FROM>
  inline
  sptr<TO> sptr_cast( sptr<FROM> &p )
  {
    TO * to = static_cast<TO*>((FROM*)p.raw());

    if ( to )
      return sptr<TO>(to, p.count, p.gid);
    else
      return sptr<TO>(); // just return a null pointer
  }

  template<typename TO, typename FROM>
  inline
  sptr<TO> sptr_static_cast( sptr<FROM> &p )
  {
    return sptr_cast<TO>(p);
  }

  template<typename TO, typename FROM>
  inline
  sptr<TO> sptr_dynamic_cast( sptr<FROM> &p ) 
  {
    TO * to = dynamic_cast<TO*>((FROM*)p.raw());

    if ( to )
      return sptr<TO>(to, p.count, p.gid);
    else
      return sptr<TO>(); // just return a null pointer
  }

#ifdef KK_USE_VPTR
  template<typename TO>
  inline
  sptr<TO> vptr_cast( vptr &p )
  {
    sptr<TO> *sp = dynamic_cast< sptr<TO>* >(&p);
    if ( sp )
      return *sp;
    else 
      return sptr<TO>();
  }
#endif
}

#endif
