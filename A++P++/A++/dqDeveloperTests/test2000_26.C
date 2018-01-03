/*
Test new and delete for objects derived from A++ objects

   This program tests the use of the delete operator.
   The derived object's delete operator or the global delete operator
   should be called instead of the base class's delete operator.

   In each case the base class new and delete operators are called.
   These both must be implemented to check the size of the class to
   determin if what is being newed/deleted is the base class or a
   possible derived class.
*/

#include <A++.h>

class A
   {
     public:
          int x;

          A()
             { x = 0; }
          virtual ~A(){};
          void* operator new(size_t s);
          void operator delete(void* p, size_t s);
   };

void* A::operator new(size_t s)
   {
     printf("A::new called\n");
     void *a = NULL;
     if (s != sizeof(A))
        {
          printf("A::new calling malloc (s=%d) != (sizeof(A)=%d) \n",s,sizeof(A));
          a = malloc(s);
        }
       else
        {
          printf("A::new calling pool mechanism (s=%d) == (sizeof(A)=%d) \n",s,sizeof(A));
          a = ::new A;
        }
     return a;
   }

void A::operator delete(void* p, size_t s)
   {
     printf("A::delete called\n");
     void *a = NULL;
     if (s != sizeof(A))
        {
          printf("A::delete calling global free mechanism (s=%d) != (sizeof(A)=%d) \n",s,sizeof(A));
          free(p);
        }
       else
        {
          printf("A::delete calling global delete mechanism (s=%d) == (sizeof(A)=%d) \n",s,sizeof(A));
          ::delete ((A*)p);
        }
   }

class B : public A
   {
     public:
          int y;

          B()
             { y = 0; }
          virtual ~B(){}
   };

class myArray : public intArray
   {
     public:
       // member data
          int myvariable;

       // member functions
         ~myArray ();
          myArray ();
   };

myArray::~myArray ()
   {
     printf ("Inside of myArray destructor! \n");
   }

myArray::myArray ()
   {
     printf ("Inside of myArray constructor! \n");
     myvariable = 0;
   }

int 
main()
   {
     Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

     printf ("Call 'Xptr = new myArray()' \n");
     myArray* Xptr = new myArray();

     APP_ASSERT (Xptr != NULL);

     printf ("Call 'delete Xptr' \n");
     delete Xptr;

  // Allocate the locally defined classes
     printf ("Call 'b = new B' \n");
     B *b = new B;

     printf ("Call 'delete b' \n");
     delete b;

     printf ("Program Terminated Normally! \n");

     return 0;
   }

