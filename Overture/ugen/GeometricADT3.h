//
// GeometricADT3 : this version templated on the data-type but not the dimension
//                 For speed arrays are allocated on the stack. But this means
//                 the arrays are allocated for the maximum size (dimension==6 corresponding
//                 to bounding boxes in 3D).        
//
// Authors: Kyle Chand and Bill Henshaw.
//
#ifndef __KKC_GEOMETRIC3_SEARCH__
#define __KKC_GEOMETRIC3_SEARCH__

#define notProcessedWithDT

#ifndef processedWithDT
#define GeomADTTuple GeomADTTuple3
#define GeometricADT GeometricADT3
#define __GeometricADTtraversor __GeometricADTtraversor3
#define __GeometricADTiterator __GeometricADTiterator3
#define debug_GeometricADT debug_GeometricADT3


#define dimension2 (dimension*2)

#endif

//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#include "GeometricADTExceptions.h"
#include "NTreeNode.h"
#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
//using namespace std;
#else
#include <list.h>
#endif

#include "GeomADTTuple3.h"

#undef __ADTType
#define __ADTType NTreeNode<2,GeomADTTuple<dataT> >



// forward declare this guy
template<class dataT> class GeometricADT;

// GeometricADT iterator class
template<class dataT>
class __GeometricADTiterator
{
 public:
  enum LeftRightEnum
  {
    ADT_LEFT  = 0,
    ADT_RIGHT = 1
  };

  __GeometricADTiterator(int dimension_, const GeometricADT<dataT> &gADT, const real *target_);
  __GeometricADTiterator(__GeometricADTiterator &x);    
  ~__GeometricADTiterator();
  
  __GeometricADTiterator & operator= (__GeometricADTiterator<dataT> & i); 
  __GeometricADTiterator & operator= (NTreeNode<2,GeomADTTuple<dataT> > & i);
  __GeometricADTiterator & operator++();  // decend to the next node in the tree
  __GeometricADTiterator  operator++(int);  // decend to the next node in the tree

  inline GeomADTTuple<dataT> & operator*(); // dereference the iterator
  
  inline bool isTerminal(); // is this iterator at an end of the tree ?
  inline int getDepth();
  
  friend class GeometricADT<dataT>;
  
 protected:
  // these constructors make no sense
  __GeometricADTiterator();// {  } // what does this do ?
  __GeometricADTiterator(GeometricADT<dataT> &x);// {  }  // should probably be a standard tree iteration (eg preorder or postorder...)
  
 private:
  GeometricADT<dataT> *thisADT;
  __ADTType *current;
  int dimension;
//  real target[dimension];
//  real *target;
  real target[6];
  int depth;
};


// GeometricADT traversor class
template<class dataT>
class __GeometricADTtraversor  // this class should probably have a superclass in NTreeNode
{
 public:
  enum LeftRightEnum
  {
    ADT_LEFT  = 0,
    ADT_RIGHT = 1
  };

  __GeometricADTtraversor(int dimension, const GeometricADT<dataT> &gADT, const real *target_);
  __GeometricADTtraversor(int dimension, const GeometricADT<dataT> &gADT);
  __GeometricADTtraversor(__GeometricADTtraversor &x);    
  ~__GeometricADTtraversor();
  
  __GeometricADTtraversor & operator= (__GeometricADTtraversor<dataT> & i); 
  __GeometricADTtraversor & operator= (NTreeNode<2,GeomADTTuple<dataT> > & i);
  __GeometricADTtraversor & operator++();  // decend to the next node in the tree
  __GeometricADTtraversor  operator++(int);  // decend to the next node in the tree

  inline GeomADTTuple<dataT> & operator*(); // dereference the traversor
  
  void setTarget(const real *target_);
  
  inline bool isFinished() const; // is this traversor finished traversing the tree ?

  inline int getDepth();
  
  friend class GeometricADT<dataT>;

 protected:
  // these constructors make no sense
  __GeometricADTtraversor() {  } // what does this do ?
  // __GeometricADTtraversor(GeometricADT<dataT> &x) {  } // should probably be a standard tree iteration (eg preorder or postorder...)
  inline bool isOverlapping(int depth, const real *bBox);
  inline bool isCandidate(const real *candidate);
  
 private:
  int depth;
  bool traversalFinished;

  list<bool> leftSubTreeFinished;  // keeps a stack of the traversal

  GeometricADT<dataT> *thisADT;
  __ADTType *current;
  int dimension;
  real target[6], a[6], b[6];
//  real *target, *a, *b;
//  real target[dimension];
//  real a[dimension];
//  real b[dimension];
};

// Actual GeometricADT class
template<class dataT>
class GeometricADT 
{
 public:
  
  enum LeftRightEnum
  {
    ADT_LEFT  = 0,
    ADT_RIGHT = 1
  };

  GeometricADT(int rangeDimension_=2); 
  GeometricADT(int rangeDimension_, const real *boundingBox_); 

  typedef __ADTType ADTType;

  ~GeometricADT();

  typedef __GeometricADTiterator<dataT>  iterator;
  typedef __GeometricADTtraversor<dataT> traversor;
  
  void initTree();
  void initTree(int rangeDimension_, const real *boundingBox_);

  int addElement(const real *bBox, dataT &data);
  int delElement(GeometricADT<dataT>::iterator & delItem);
  void verifyTree();


  friend class __GeometricADTiterator<dataT>;  
  friend class __GeometricADTtraversor<dataT>; 

 protected:
  inline int getSplitAxis(int depth) const;
  inline Real getSplitLocation(int axis, const real *box) const;
  void shiftTreeUp(GeometricADT<dataT>::ADTType *node, int depth);  // used to rebuild the search tree
  void verifyNode(GeometricADT<dataT>::ADTType &node, int depth);
  int insert(GeometricADT<dataT>::iterator &insParent, int leaf, GeomADTTuple<dataT> &data);
  int insert(GeometricADT<dataT>::iterator &insParent, int leaf);

 private:

  int rangeDimension;
  int dimension;
  int ADTdepth;
  int numberOfItems;

  // real boundingBox[dimension2];
  // real *boundingBox;
  real boundingBox[12];
  
  GeometricADT<dataT>::ADTType *adtRoot;

};


//
// implementation of inlined GeometricADT methods
//


template<class dataT>
inline 
int
GeometricADT<dataT>::
getSplitAxis(int depth) const 
{
  //AssertException<GeometricADT::InvalidDepth> (depth<=ADTdepth);
#if 0
  assert(depth<=ADTdepth);
#endif
  return depth%dimension;
}

template<class dataT>
inline 
real 
GeometricADT<dataT>::
getSplitLocation(int axis, const real *box) const
{
  //AssertException<InvalidADTDimension> (axis<ADTDimension && box.getLength(0)==2*ADTDimension);
  // assert(axis<ADTDimension && box.getLength(0)==2*ADTDimension);
  // assert(axis<ADTDimension && box.getLength(0)==2*ADTDimension);
  return (box[2*axis+1] + box[2*axis])/2.0;
}

// include "../GridGenerator/GeometricADT.C"
// include "../GridGenerator/GeometricADTIterator.C"
// include "../GridGenerator/GeometricADTTraversor.C"

//\begin{GeometricADTIterator.tex}{\subsection{Dereference operator*()}}
template<class dataT>
inline
GeomADTTuple<dataT> &
__GeometricADTiterator<dataT>::
operator*()
// /Purpose : returns the data at the current node of the iterators {\tt GeometricADT}
//\end{GeometricADT.tex}
{
  return current->getData();
}

//\begin{GeometricADTIterator.tex}{\subsection{getDepth}}
template<class dataT>
inline
int
__GeometricADTiterator<dataT>::
getDepth() 
// /Purpose : returns the iterators depth in it's {\tt GeometricADT}
//\end{GeometricADT.tex}
{
  return depth;
}

template<class dataT>
inline
bool
__GeometricADTtraversor<dataT>::
isFinished() const
{

  return traversalFinished;

}

template<class dataT>
inline
GeomADTTuple<dataT> &
__GeometricADTtraversor<dataT>::
operator*()
{
  return current->getData();
}

template<class dataT>
inline
int
__GeometricADTtraversor<dataT>::
getDepth() 
{
  return depth;
}


template<class dataT>
inline
bool 
__GeometricADTtraversor<dataT>::
isOverlapping(int theDepth, const real *bBox)
{
  
  // this may need to be reworked to make it more general...
  //bBox.display("bBox");
  int axis = thisADT->getSplitAxis(theDepth);
  return (a[axis]<=bBox[2*axis+1] && bBox[2*axis]<=b[axis]);

}

template<class dataT>
inline
bool 
__GeometricADTtraversor<dataT>::
isCandidate(const real *candidate) 
{
  for (int axis=0; axis<thisADT->dimension; axis++) 
  {
    // res = res && (a[axis]<=candidate[axis] && candidate[axis]<=b[axis]);
    if( a[axis]>candidate[axis] || candidate[axis]>b[axis] )
      return false;
    //cout << " axis "<<axis<<" a "<<a(axis)<<" xk "<<candidate(axis)<<" b "<<b(axis)<<endl;
  }
  return true;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{isTerminal}}
template<class dataT>
inline
bool
__GeometricADTiterator<dataT>::
isTerminal()
// /Purpose : return true if the iterator is a terminal leaf (ie it cannot descend any further)
//\end{GeometricADTIterator.tex}
{

  if (current==NULL) return true;

  int splitAxis = thisADT->getSplitAxis(depth);
  Real splitLoc = thisADT->getSplitLocation(splitAxis, (current->getData()).boundingBox);

  if ((current->querry(ADT_LEFT) && 
      current->querry(ADT_RIGHT)) ||
      (current->querry(ADT_LEFT) && target[splitAxis]<=splitLoc) ||
      (current->querry(ADT_RIGHT) && target[splitAxis]>splitLoc))
    return false;
  else
    return true;

}


#ifndef processedWithDT

#include "GeometricADT3.C"

#undef GeomADTTuple
#undef GeometricADT
#undef __GeometricADTtraversor 
#undef __GeometricADTiterator
#undef GeometricADTError
#undef GeometricADTIteratorError 
#undef GeometricADTTraversorError
#undef debug_GeometricADT 
#undef UnknownError 
#undef VerificationError
#undef InvalidDepth
#undef TreeInsertionError
#undef InvalidADTDimension 
#undef dimension2

#endif

#endif
