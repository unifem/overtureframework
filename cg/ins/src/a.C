#include <iostream>

class P
{
public:
P() { std::cout << "P constructor\n";} //
virtual ~P(){ std::cout << "P destructor\n";} //
virtual void print(){ std::cout << "P print\n";} //
};

class Q : public P
{
public:
Q() { std::cout << "Q constructor\n";} //
virtual ~Q(){ std::cout << "Q destructor\n";} //
virtual void print(){ std::cout << "Q print\n";} //

};

class A
{
public:

A(P & p_) : p(p_){ std::cout << "A constructor\n";} //

virtual ~A(){ std::cout << "A destructor\n";} //

P & p;

};

class B : public A
{
  public:

  B() : A(*(new Q)){ std::cout << "B constructor\n";} // ;

  B(P & p_) : A(p_) { std::cout << "B constructor\n";} //
  ~B(){ delete &p; std::cout << "B destructor\n";} //
};




int
main(int argc, char *argv[])
{
//  using std;

  std::cout << "Running a...\n";

//  Q q;
  

//  A & a = *new B(q);
  A & a = *new B();
  
  a.p.print();
  
  delete &a;
  
  return 0;
}



