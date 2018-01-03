
#include <A++.h>

class A
   {
     public:
          A(){}
          virtual ~A(){};
          void* operator new(size_t s)
             {
               printf("A::new called\n"); 
               A *a;
               a = ::new A;

               ::delete a;
               a = NULL;
               return a;
             }
          void operator delete(void* p, size_t s)
             {
               printf("A::delete called\n");
             }
   };

class B : public A
   {
     public:
          B(){}
          virtual ~B(){}
   };


int 
main()
   {
     B *b = new B;
  
     delete b;

     return 0;
   }



