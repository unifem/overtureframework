#include <list>
#include "kk_ptr.hh"

namespace {

  long int gid_counter=0;

  std::list<long int> available_gids;

}

/// acquire a "global" id, this will be used internally by KK::sptr
long int 
KK::acquire_gid()
{
  if ( !available_gids.size() )
    {
      return gid_counter++;
    } 
  else
    {
      long int gid = available_gids.back();
      available_gids.pop_back();
      return gid;
    }
}

/// release a "global" id allowing it to be used again
void 
KK::release_gid(long int gid)
{
  available_gids.push_back(gid);
}

#ifdef KK_PTR_TEST

// classes used for test of kkc's smart pointers

// utility that makes formatting gantlet output easier
#include "kk_synch.hh"

#include <string>
using namespace std;

enum TestEnum {
  fFromA,
  fFromB
};

class A {
public:
  TestEnum f() { return fFromA;}
  virtual void makepoly(){}
};

class B : public A {
public:
  TestEnum f() { return fFromB;}
};

class S { };

int main(int argc, char *argv[])
{
  {
    GANTLET_START(-1);

    GANTLET_COMMENT("KK::sptr Tests");

    std::list< KK::sptr<int> > l1,l2;

    int npush=3;
    for ( int i=0; i<npush; i++ )
      {
	//      std::cout<<"pushing "<<i<<std::endl;
	KK::sptr<int> ip = new int;
	*ip = i;
	l1.push_back(ip);
	l2.push_front(ip);
	GANTLET_ASSERT(*(l1.back())==i,"");
	GANTLET_ASSERT(*(l2.front())==i,"");
	GANTLET_ASSERT(ip.nRefs()==3,"");
	GANTLET_ASSERT(l1.back().nRefs()==3,"");
	GANTLET_ASSERT(l2.front().nRefs()==3,"");
      }

    int i=0;
    while( l1.size() )
      {
	//      std::cout<<"popping "<<*(l1.front())<<endl;
	GANTLET_ASSERT(*(l1.front())==i,"");
	GANTLET_ASSERT(*(l2.back())==i,"");
	l1.pop_front();
	l2.pop_back();

	i++;
      }

    KK::sptr<int> anInt = new int;
    *anInt = 100;
    KK::sptr<int> anotherInt;
    anotherInt = anInt;
    GANTLET_ASSERT(*anotherInt==100,"");

    *anotherInt = 200;
    GANTLET_ASSERT(*anInt==200,"");
    //  std::cout<<"*anInt = "<<*anInt<<"; *anotherInt = "<<*anotherInt<<std::endl;

    KK::sptr<int> yetAnotherInt = new int;
    *yetAnotherInt = 500;
    anotherInt = yetAnotherInt; // point to something new, break the old reference
    GANTLET_ASSERT(*anotherInt==500,"");
  
    //  cout<<"anotherInt is now "<<*anotherInt<<endl;

    // try to cause/catch some errors:

    // create a null sptr

    KK::sptr<int> aptr;
#ifdef KK_DEBUG
    try {
      GANTLET_BEGIN_TEST;
      *aptr=100;
      GANTLET_FAIL_TEST("did not catch assignment to null");
    }
    catch ( KK::sptr_Err &e ) {
      //    std::cout<<"caught "<<e.repr()<<std::endl;
      GANTLET_PASS_TEST("caught assignment to null");
    } 
    catch (...) {
      GANTLET_FAIL_TEST("unexpected error when trying to catch assignment to null exception");
    }    
#endif

    // try the bool thing
    GANTLET_ASSERT(!aptr,"null pointer boolean false check");
    //   if ( aptr )
    //     std::cout<<"should never see this line!"<<std::endl;
    //   else
    //     std::cout<<"should see this line, conversion to bool from null worked"<<std::endl;

    GANTLET_ASSERT(anInt,"allocated pointer boolean true check");
    //   if ( anInt )
    //     std::cout<<"should see this line, converstion to bool from not-null worked"<<std::endl;
    //   else
    //     std::cout<<"should never see this line!"<<std::endl;

    GANTLET_FINISH;
  }

  {  // try the dynamic casts
    GANTLET_START(-1);
    GANTLET_COMMENT("KK::sptr dynamic cast tests");

    KK::sptr<B> Bptr = new B;
    
    KK::sptr<A> Aptr = KK::sptr_cast<A>(Bptr);
    GANTLET_ASSERT(Aptr,"cast from B to A ");
    GANTLET_ASSERT(Aptr->f()==fFromA,"");

    // try the cast in the other direction
    KK::sptr<B> B2ptr;
    B2ptr = KK::sptr_cast<B>(Aptr);

    GANTLET_ASSERT(B2ptr,"cast from A to B ");
    GANTLET_ASSERT(B2ptr->f()==fFromB,"");

    // now try an invalid cast
    KK::sptr<S> Sptr;
    Sptr = KK::sptr_dynamic_cast<S>(Bptr);
    GANTLET_ASSERT(!Sptr,"prevent cast from B to S");

#ifdef KK_USE_VPTR
    KK::vptr &vp = Bptr;
    KK::sptr<int> null;
    KK::vptr &nvp = null;

    GANTLET_ASSERT(vp,"make sure that vptr::bool() is true for allocated pointer");
    GANTLET_ASSERT(!nvp,"make sure that vptr::bool() is false for null pointer");
    GANTLET_ASSERT(!KK::vptr_cast<S>(vp),"prevent vptr_cast from B to S ");

    GANTLET_ASSERT(KK::vptr_cast<B>(vp),"vptr_cast to B from vptr to B ");

    long int bgid = Bptr.GID();
    GANTLET_ASSERT(bgid==vp.GID(),"get correct GID via vptr::GID()");
#endif

    GANTLET_FINISH;
  }

  return 0;

}

#endif








