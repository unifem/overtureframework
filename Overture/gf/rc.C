#include "A++.h"

class RC
{
private:
  // Here is where we store the data
  class RCRep
  {
  public:
    int referenceCount;
    int data;
    floatArray array;
    RCRep()
    {
      referenceCount=1;
      data=0;
      array.redim(5); array=0.;
    };
    ~RCRep()
    {
    };
  };
  RCRep *rep;

  floatArray array;

  RC()
  {
    rep = new RCRep();
    array.reference(rep->array);
  }
  ~RC()
  {
    if( --rep->referenceCount==0 )
      delete rep;
  }
  RC( const RC & item )
  {
    rep=item.rep;
    array.reference(rep->array);
    
    rep->referenceCount++;
  }
  RC & operator=( const RC & item )
  {
    if( --(rep->referenceCount)== 0 )
      delete rep;
    rep=item.rep;
    array.reference(item.array);
    rep->referenceCount++;
    return *this;
  }
 public:
  inline int & data() 
  {
    cout << " int & data() called" << endl;
    return rep->data;
  }

  void reference( const RC & item )
  {
  }
  void unReference()  // un-reference entire object
  {
    if( rep->referenceCount == 1 ) // no one else referencing
      return;
    if( --(rep->referenceCount)== 0 ) // shouldn't happen
      delete rep;
    rep = new RCRep();
    array.reference(rep->array);

  }
  void unReferenceArray() // un-reference member array only
  {
    array.resize(array);  // does this work?
  }
  
  
};

  
void main()
{
  
  RC a;
  a.array=5.;
  a.data()=1;
  cout << "here is a.data = " << a.data() << endl;
  a.data()=6*a.data();
  cout << "here is a.data = " << a.data() << endl;
  RC b=a;
  b.array.display("Here is b:");
  b.array=6.;
  
  RC c;
  c=a;
  c.array.display("Here is c:");
}  
