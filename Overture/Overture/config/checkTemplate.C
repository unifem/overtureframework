#ifdef CHECKFORSTL
// code to check for STL availability
#include <vector>
#include <map>
#include <list>

// kkc this duplicate include fails on the SGI
//kkc 081124 gcc no longer allows iostream.h files #include <iostream.h>
#include <iostream>

using namespace std;

int 
main()
{
  istream &i=cin;
  vector<int> vi;
  return 0;
}
#endif

#ifdef CHECKFOROLDSTL
// see if the old style of include is used for STL
#include <vector.h>
#include <map.h>
#include <list.h>

int 
main()
{
  vector<int> vi;
  return 0;
}
#endif

#ifdef CHECKTEMPLARGS
// does the compiler understand default template arguments?
template< int a, int b=1 >
class foo
{

};

int 
main()
{
  foo<1> f;
  return 0;
}

#endif

#ifdef CHECKNAMESPACE
// wdh: SGI complains if we don't include something using std before 'using namespace std;'
#include <iostream>
#include <string>
using namespace std;

istream &getcin() { return cin; }

int
main(void)
{
  int foo=0;
  
  istream &i=cin;

  return foo;
}
#endif

#ifdef CHECKFOREXPLICIT

class foo
{
public:
  foo() { }
  ~foo() { }
  
  explicit foo(int i) { i=0; }
};

int 
main()
{
  foo f;
};

#endif

