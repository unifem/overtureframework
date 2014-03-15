//#define OV_DEBUG
//kkc 081124 #include <iostream.h>
#include <iostream>

#include "Overture.h"
#include "GeometricADT.h"

using namespace std;

// implementation of __GeometricADTtraversor class
template<class dataT>
__GeometricADTtraversor<dataT>::
//__GeometricADTtraversor(const GeometricADT<dataT> &gADT, realArray & target_) 
__GeometricADTtraversor(const GeometricADT<dataT> &gADT, ArraySimple<real> & target_) 
{
  thisADT = (GeometricADT<dataT> *) &gADT;
  //  target = evaluate(target_);
  target = target_;

  current = gADT.adtRoot;
  depth = 0;
  //leftSubTreeFinished.clear();
  leftSubTreeFinished.push_front(false);
  traversalFinished = false;

  //  a.redim(target.getLength(0));
  //b.redim(target.getLength(0));

  a.resize(target.size(0));
  a=0.;
  b = a;

  //Range R(0,a.getBound(0)-1,2);
  //Range R(0,a.getBound(0)-1,2);
#if 0
  a(R) = thisADT->boundingBox(2*R);
  a(R+1) = target(R);
  b(R) = target(R+1);
  b(R+1) = thisADT->boundingBox(2*(R+1)+1);
#else
  for ( int r=0; r<thisADT->ADTDimension; r+=2 )
    {
      a(r) = thisADT->boundingBox(2*r);
      a(r+1) = target(r);
      b(r) = target(r+1);
      //      b(r+1) = max(target(r), thisADT->boundingBox(2*(r+1)+1));
      b(r+1) = thisADT->boundingBox(2*r+3);
    }
#endif
  //a.display("a");
  //b.display("b");

  if (!(isCandidate(current->getData().coords))) ++(*this);
}

template<class dataT>
__GeometricADTtraversor<dataT>::
__GeometricADTtraversor(__GeometricADTtraversor &x) : target(x.target), thisADT(x.thisADT)
{
  depth = x.depth;
  current = x.current;
  leftSubTreeFinished = x.leftSubTreeFinished;
  traversalFinished = false;

  //  a= evaluate(x.a);
  //b= evaluate(x.b);

  a = x.a;
  b = x.b;

  if (!(isCandidate(current->getData().coords))) ++(*this);

}

template<class dataT>
__GeometricADTtraversor<dataT>::
~__GeometricADTtraversor()
{
  //leftSubTreeFinished.clear();
}

//\begin{>GeometricADTTraversor.tex}{\subsection{setTarget}}
template<class dataT>
void __GeometricADTtraversor<dataT>::
setTarget(const ArraySimple<real> &target_) 
{
  assert( thisADT!=NULL );
  int axis;
  target = target_;
  //for( axis=0; axis<thisADT->ADTDimension; axis++ )
    //  target[axis] = target_[axis];

  current = thisADT->adtRoot;
  depth = 0;

  leftSubTreeFinished.clear();
  leftSubTreeFinished.push_front(false);
  traversalFinished = false;

  int dim = thisADT->ADTDimension;
  //this->a = ArraySimple<real> (dim);
  //this->b = a;
  for( axis=0; axis<thisADT->ADTDimension; axis+=2 )
  {
    a[axis]=thisADT->boundingBox[2*axis];
    a[axis+1]=target[axis];
    b[axis]=target[axis+1];
    b[axis+1]=thisADT->boundingBox[2*axis+3];
  }
  
  if (!(isCandidate(current->getData().coords))) ++(*this);
}

template<class dataT>
bool
__GeometricADTtraversor<dataT>::
isFinished() const
{

  return traversalFinished;

}

// only change what the traversor is pointing to, not the iteration criteria (target) in operator=
template<class dataT>
__GeometricADTtraversor<dataT>  & 
__GeometricADTtraversor<dataT>::
operator=(__GeometricADTtraversor &i)
{
  
  this->current = i.current;
  //this->target  = target;
  this->depth   = i.depth;
  leftSubTreeFinished = i.leftSubTreeFinished;

  return *this;
}

template<class dataT>
__GeometricADTtraversor<dataT> & 
__GeometricADTtraversor<dataT>::
operator=(__ADTType &x)
{
  //this-> = &x;
  
  //this->thisADT = i.thisADT;
  this->current = &x;
  //this->target  = target;
  //  this->depth   = depth;
    return *this;
}

template<class dataT>
__GeometricADTtraversor<dataT> & 
__GeometricADTtraversor<dataT>::
operator++()
{

  bool candFound = false;

//   if (traversalFinished) 
//     cout<<"traversalFinished true"<<endl;
//   else
//     cout<<"traversalFinished false"<<endl;
  
  while (!candFound && !traversalFinished) 
    {
      
      if ((!leftSubTreeFinished.front()) &&  
	  current->querry(ADT_LEFT) && 
	  isOverlapping(depth, (current->getLeaf(ADT_LEFT)).getData().boundingBox)) {
	// proceed left
	//cout<<"proceeding left"<<endl;
	//cout <<"traversor id "<<current->getData().id<<endl;
	current = &(current->getLeaf(ADT_LEFT));
	leftSubTreeFinished.push_front(false);
	depth++;
	candFound = isCandidate((current->getData()).coords);
      } else if (current->querry(ADT_RIGHT) && 
		 isOverlapping(depth,(current->getLeaf(ADT_RIGHT).getData().boundingBox))) {
	// proceed right
	//cout<<"proceeding right"<<endl;
	//cout <<"traversor id "<<current->getData().id<<endl;
	current = &(current->getLeaf(ADT_RIGHT));
	leftSubTreeFinished.pop_front();
	leftSubTreeFinished.push_front(true);
	leftSubTreeFinished.push_front(false);
	depth++;
	candFound = isCandidate((current->getData()).coords);
      } else {
	// back up
	//cout<<"backing up..."<<endl;
	//cout <<"traversor id "<<current->getData().id<<endl;

	if (depth!=0) 
	  {
	    leftSubTreeFinished.pop_front();
	    current = &(current->getTrunk());
	    depth--;
	    
	    //cout <<"backing up 1"<<endl;
	    while (leftSubTreeFinished.front()&&depth>0)
	      {
		//cout <<"backing up"<<endl;
		leftSubTreeFinished.pop_front();
		current = &(current->getTrunk());
		depth--;
	      }
	    
	  } 
	if (!leftSubTreeFinished.front()) 
	  {
	    leftSubTreeFinished.pop_front();
	    leftSubTreeFinished.push_front(true);
	  } else {
	    traversalFinished = true;
	  }
      }

    }
  
  return *this;

}

template<class dataT>
__GeometricADTtraversor<dataT>
__GeometricADTtraversor<dataT>::
operator++(int)
{

  __GeometricADTtraversor tmp(*this);

  ++(*this);

  return tmp;

}

#if 0
__GeometricADTtraversor & 
__GeometricADTtraversor<dataT>::
operator--()
{
  --(GeometricADT<dataT>::ADTType::selection_traversor(*this));
  return *this;
#if 0
  current = current->getTrunk();
  depth--;
#endif
}
#endif

template<class dataT>
GeomADTTuple<dataT> &
__GeometricADTtraversor<dataT>::
operator*()
{
  return current->getData();
}

template<class dataT>
int
__GeometricADTtraversor<dataT>::
getDepth() 
{
  return depth;
}

template<class dataT>
bool 
__GeometricADTtraversor<dataT>::
//isOverlapping(int theDepth, const realArray &bBox)
isOverlapping(int theDepth, const ArraySimple<real> &bBox)
{
  
  // this may need to be reworked to make it more general...
  //bBox.display("bBox");

  int axis = thisADT->getSplitAxis(theDepth);
  //cout<<"axis is "<<axis<<endl;
  //cout<<a(axis)<<" "<<bBox(2*axis+1)<<" "<<bBox(2*axis)<<" "<<b(axis)<<endl;
  return (a(axis)<=bBox(2*axis+1) && bBox(2*axis)<=b(axis));

}

template<class dataT>
bool 
__GeometricADTtraversor<dataT>::
//isCandidate(const realArray &candidate) 
isCandidate(const ArraySimple<real> &candidate) 
{

  bool res;
  res = true;
  for (int axis=0; axis<thisADT->ADTDimension; axis++) 
    {
      res = res && (a(axis)<=candidate(axis) && candidate(axis)<=b(axis));
      //cout << " axis "<<axis<<" a "<<a(axis)<<" xk "<<candidate(axis)<<" b "<<b(axis)<<endl;
    }

  return res;
}

