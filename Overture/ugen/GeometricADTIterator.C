//kkc 081124 #include <iostream.h>
#include <iostream>

#include "Overture.h"
#include "GeometricADT.h"

using namespace std;
// implementation of GeometricADT::iterator class

//\begin{>GeometricADTIterator.tex}{\subsection{Constructor}}
template<class dataT>
__GeometricADTiterator<dataT>::
//__GeometricADTiterator(const GeometricADT<dataT> &gADT, realArray & target_) 
__GeometricADTiterator(const GeometricADT<dataT> &gADT, ArraySimple<real> & target_) 
// /Purpose : initialize the iterator with a given {\tt GeometricADT} and target bounding box location
// /gADT (input) : the {\tt GeometricADT} through which to iterate
// /target\_ (input) : a {\tt realArray} containing the target bounding box location
//\end{GeometricADTIterator.tex}
{
  thisADT = (GeometricADT<dataT> *)&gADT;
  //  target = evaluate(target_);
  target = target_;
  current = gADT.adtRoot;
  depth = 0;


}

//\begin{>>GeometricADTIterator.tex}{\subsection{Copy Constructor}}
template<class dataT>
__GeometricADTiterator<dataT>::
__GeometricADTiterator(__GeometricADTiterator<dataT> &x) : target(x.target), thisADT(x.thisADT)
// /Purpose : copy the state of x into this iterator
// /x : iterator to copy
//\end{GeometricADTIterator.tex}
{
  depth = x.depth;
  current = x.current;
}

template<class dataT>
__GeometricADTiterator<dataT>::
~__GeometricADTiterator()
{
  // this does not need to do anything yet 
}

//\begin{>>GeometricADTIterator.tex}{\subsection{isTerminal}}
template<class dataT>
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
      (current->querry(ADT_LEFT) && target(splitAxis)<=splitLoc) ||
      (current->querry(ADT_RIGHT) && target(splitAxis)>splitLoc))
    return false;
  else
    return true;

}

//\begin{>>GeometricADTIterator.tex}{\subsection{Assignment to Another Iterator}}
template<class dataT>
__GeometricADTiterator<dataT> & 
__GeometricADTiterator<dataT>::
operator=(__GeometricADTiterator<dataT> &i)
// /Purpose : assign the iterator to the value of another, only change what the iterator is pointing to,not the iteration criteria (target)
// /i : iterator to assign to
//\end{GeometricADTIterator.tex}
{
// only change what the iterator is pointing to, not the iteration criteria (target) in operator=
  
  this->current = i.current;
  //this->target  = target;
  this->depth   = i.depth;
  

  return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Assignment to a Node in the Seach Tree}}
template<class dataT>
__GeometricADTiterator<dataT> &
__GeometricADTiterator<dataT>::
operator=(__ADTType &x)
// /Purpose : assign the iterator's current pointer to a particular node
// /x : node to assign current value to
//\end{GeometricADTIterator.tex}
{
  //this-> = &x;
  
  //this->thisADT = i.thisADT;
  this->current = &x;
  //this->target  = target;
  //  this->depth   = depth;
    return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Prefix Increment}}
template<class dataT>
__GeometricADTiterator<dataT> & __GeometricADTiterator<dataT>::
operator++()
// /Purpose : increment the iterator (advance the iteration) and then return the iterator
//\end{GeometricADTIterator.tex}
{
#ifdef ASSERTIONS_ON
#if 1
  AssertException (!isTerminal(),GeometricADTIteratorError()); 
#else
  AssertException<GeometricADTIteratorError> (!isTerminal());
#endif
#endif
  //assert(!isTerminal());

  int splitAxis = thisADT->getSplitAxis(depth);
  Real splitLoc = thisADT->getSplitLocation(splitAxis, current->getData().boundingBox);
  if (target(splitAxis)<=splitLoc) 
    current = &(current->getLeaf(ADT_LEFT));
  else
    current = &(current->getLeaf(ADT_RIGHT));

  depth++;

  return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Postfix Increment}}
template<class dataT>
__GeometricADTiterator<dataT> 
__GeometricADTiterator<dataT>::
operator++(int)
// /Purpose : increment the iterator (advance the iteration) and then return the old value
//\end{GeometricADTIterator.tex}
{
#ifdef ASSERTIONS_ON
#if 1
  AssertException (!isTerminal(),GeometricADTIteratorError() );
#else
  AssertException<GeometricADTIteratorError> (!isTerminal());
#endif
#endif
  //  assert(!isTerminal());

  int splitAxis = thisADT->getSplitAxis(depth);
  Real splitLoc = thisADT->getSplitLocation(splitAxis, current->getData().boundingBox);

  __GeometricADTiterator<dataT> tmp(*this);

  if (target(splitAxis)<=splitLoc) 
    current = &(current->getLeaf(ADT_LEFT));
  else
    current = &(current->getLeaf(ADT_RIGHT));

  depth++;

  return tmp;
}

#if 0
__GeometricADTiterator<dataT> & 
__GeometricADTiterator::
operator--()
{
  --(GeometricADT<dataT>::ADTType::selection_iterator(*this));
  return *this;
#if 0
  current = current->getTrunk();
  depth--;
#endif
}
#endif

//\begin{GeometricADTIterator.tex}{\subsection{Dereference operator*()}}
template<class dataT>
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
int
__GeometricADTiterator<dataT>::
getDepth() 
// /Purpose : returns the iterators depth in it's {\tt GeometricADT}
//\end{GeometricADT.tex}
{
  return depth;
}
