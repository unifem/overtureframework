#if !defined(SGI_FIX) || !defined(GeometricADT_C_H)
#define GeometricADT_C_H


//kkc 081124 #include <iostream.h>

#include <iostream>
#include "Overture.h"
#include "GeometricADT.h"

using namespace std;

const bool debug_GeometricADT = true;  // set to false to remove AssertException calls 

//\begin{>GeometricADT.tex}{\subsection{Constructor}}
template<class dataT>
GeometricADT<dataT>::
GeometricADT(int rangeDimension_)
//======================================================
// /Purpose : construct a geometric search ADT for a given dimension
// /rangeDimension\_ (input) : dimension of the physical search space
//\end{GeometciADT.tex}
//======================================================
{
  rangeDimension = rangeDimension_;
  initTree();
}

//\begin{>>GeometricADT.tex}{\subsection{Constructor}}
template<class dataT>
GeometricADT<dataT>::
//GeometricADT(const realArray & boundingBox_)
GeometricADT(int rangeDimension_, const ArraySimple<real> & boundingBox_)
//======================================================
// /Purpose : construct a geometric search ADT given a bounding box domain
// /boundingBox\_ (input) : bounding box of the ADT search space
//\end{GeometciADT.tex}
//======================================================
{
  initTree(rangeDimension_, boundingBox_);
  //boundingBox.display("GeometricADT boundingBox");
}

template<class dataT>
GeometricADT<dataT>::
~GeometricADT() 
{
  if (adtRoot!=NULL) 
    {
      adtRoot->del(ADT_LEFT); // delete left
      adtRoot->del(ADT_RIGHT); // delete right
      delete adtRoot;
      adtRoot = NULL;
    }
}

//begin{>>GeometricADT.tex}{\subsection{initTree}}
template<class dataT>
void
GeometricADT<dataT>::
initTree()
//=======================================================
// /Purpose : initialize the tree, sets up bounding boxes and does some assertion checks
// If a bounding box has not been provided, the bounding box is assumed to be the unit box
//\end{GeometricADT.tex}
//=======================================================
{
  
#if 1
  AssertException  (!debug_GeometricADT ||
		    (rangeDimension==1 || rangeDimension==2 || rangeDimension==3), InvalidADTDimension());
#else
  AssertException<InvalidADTDimension> (!debug_GeometricADT ||
					(rangeDimension==1 || rangeDimension==2 || rangeDimension==3));
#endif
  //assert(rangeDimension==1 || rangeDimension==2 || rangeDimension==3);
  
  //cout << "rangeDimension of GeometricADT is "<<rangeDimension<<endl;
  ADTDimension = 2*rangeDimension;
#if 0
  Range R(0,2*ADTDimension-2,2);
  
  boundingBox.redim(2*ADTDimension);
  boundingBox(R) = 0.0;
  boundingBox(R+1) = 1.0;
#else
  boundingBox.resize(2*ADTDimension);
  for ( int a=0; a<ADTDimension; a++ )
    {
      boundingBox(2*a) = 0.0;
      boundingBox(2*a+1) = 1.0;
    }
#endif

  adtRoot = NULL;
  ADTdepth = 0;
  numberOfItems = 0;
  
}

//\begin{>>GeometricADT.tex}{\subsection{initTree}}
template<class dataT>
void 
GeometricADT<dataT>::
//initTree(const realArray & boundingBox_) 
initTree(int rangeDimension_, const ArraySimple<real> & boundingBox_) 
//=======================================================
// /Purpose : initialize the tree given a particular bounding box
// /boundingBox[2*rangeDimension] (input) \_ : bounding box for the geometric search tree
//        *NOTE* change from previous version
//\end{GeometricADT.tex}
//=======================================================
{
#if 0
  rangeDimension = boundingBox_.getLength(0)/4;
  initTree();
  boundingBox = boundingBox_;
  ADTDimension = 2*rangeDimension;
  Range R(0,2*ADTDimension-1,2);
  AssertException (!debug_GeometricADT ||
		   (sum(boundingBox(R)<=boundingBox(R+1))==ADTDimension), OutOfBoundingBox());
#endif
  rangeDimension = rangeDimension_;
  initTree();
  for( int axis=0; axis<ADTDimension; axis+=2 )
  {
    // for now we use:  min(xMin)==min(xMax)
    boundingBox[2*axis  ] = boundingBox_[axis  ];   // min(xMin)
    boundingBox[2*axis+1] = boundingBox_[axis+1];   // max(xMin)
    boundingBox[2*axis+2] = boundingBox_[axis  ];   // min(xMax)
    boundingBox[2*axis+3] = boundingBox_[axis+1];   // max(xMax)
  }
  ADTDimension = 2*rangeDimension;
}

//\begin{>>GeometricADT.tex}{\subsection{insert}}
template<class dataT>
int
GeometricADT<dataT>::
insert(typename GeometricADT<dataT>::iterator &insParent, int leaf, GeomADTTuple<dataT> &data)
//=======================================================
// /Purpose : insert a geometric entity into the tree
// /insParent (input) : the position in the tree to insert the data
// /leaf (input) : leaf of insertParent to add the data
// /data (input) : data to add to the tree
//\end{GeometricADT.tex}
//=======================================================
{
  try {
    int err = insParent.current->add(leaf, data);
    return err;
  }
  catch (NTreeNodeError &e) {
    e.debug_print();
    throw TreeInsertionError();
  }
  catch (...) {
    // if it is not an error in NTreeNode, then we don't know what it is, rethrow and pray (or abort)
    throw;
  }
}

//\begin{>>GeometricADT.tex}{\subsection{addElement}}
template<class dataT>
int
GeometricADT<dataT>::
//addElement(realArray &coords, dataT &data)
addElement(ArraySimple<real> &coords, dataT &data)
//=======================================================
// /Purpose : insert id into the search tree if id give the bounding box coordinates coords
// /coords (input) : bounding box coordinates for id (x1min, x1max, x2min, x2max,..., xnmin, xnmax)
// /id (input) : id to store at the given location (probably should be a templatized type
//\end{GeometricADT.tex}
//=======================================================
{

  int err=0;
	
  //const realArray & bb = boundingBox;
  const ArraySimple<real> & bb = boundingBox;

  for (int axis = 0; axis<ADTDimension; axis++)
    AssertException (!debug_GeometricADT ||
		     (bb(2*axis)<=coords(axis) && coords(axis)<=bb(2*axis+1)), OutOfBoundingBox());

  if (numberOfItems == 0)  // this is the first element to be added 
    {
      AssertException ((!debug_GeometricADT || (adtRoot == NULL)), NULLADTRoot());

      GeomADTTuple<dataT> gt(boundingBox, coords, data);
      adtRoot = new ADTType(gt);
      // adtRoot->getData().setData(boundingBox, coords, data);

      numberOfItems = 1;
      ADTdepth = 0;
      err = 0;
    } else {

      // iterate until we find the location to stick this thing
      typename GeometricADT<dataT>::iterator iter(*this,coords);
      
      //for (iter=this->Begin(); !iter.isTerminal(); ++iter) ;
      for ( ; !iter.isTerminal(); ++iter) {
	//cout << iter.getDepth()<<endl;
      }
      
      //ADTType & parent = *iter;
      
      //const realArray & parKey = (*iter).boundingBox; //parent.getKey();
      const ArraySimple<real> & parKey = (*iter).boundingBox; //parent.getKey();
      
      const int parentDepth = iter.getDepth();
      //cout<<"parent depth is"<<endl;
      // there are three possibilities:
      // 1. the left and right leaves are both empty -- we must choose one
      // 2. the left tree is the one we want, the right is occupied but excludes coords
      // 3. the right tree is the one we want, the left is occupied but excludes coords
      int splitAxis = getSplitAxis(parentDepth);
      real splitLoc = getSplitLocation(splitAxis, parKey);
      
      // create new boundingBox
      //GeomADTTuple *newTuple = new GeomADTTyple;
      //realArray *newBoxPtr = new realArray(2*ADTDimension);
      //newTuple->boundingBox.redim(2*ADTDimension);
      //realArray & newBox = newTuple->boundingBox;
      //      realArray newBox(2*ADTDimension);
      ArraySimple<real> newBox(2*ADTDimension);
      newBox = parKey;
      //newTuple->id = id;
      if (coords(splitAxis)<=splitLoc) // go left 
	{
	  //assert(!parent.querryLeaf(ADT_LEFT)); // if this fails the iterator screwed up
	  //GeometricADT<dataT>::iterator last = iter;
	  //assert(iter.isTerminal());
	  newBox(2*splitAxis + 1) = splitLoc;
	  GeomADTTuple<dataT> gt(newBox, coords, data);
	  err = insert(iter, ADT_LEFT, gt);
	  //err = parent.addLeaf(ADT_LEFT, newBox, id);
	} else { // go right
	  //assert(coords(splitAxis)>splitLoc); // if this fails the iterator screwed up
	  //assert(!parent.querryLeaf(ADT_RIGHT));// if this fails the iterator screwed up
	  //GeometricADT<dataT>::iterator last = iter;
	  //assert(iter.isTerminal());
	  newBox(2*splitAxis) = splitLoc;
	  GeomADTTuple<dataT> gt(newBox, coords, data);
	  err = insert(iter, ADT_RIGHT, gt);
	  //err = parent.addLeaf(ADT_RIGHT, newBox, id);
	}
    
      if (err == 0) 
	{
	  numberOfItems++;
	  if ((parentDepth+1)>ADTdepth) 
	    ADTdepth+=1;
	}
    }    
  //cout << "depth is "<<ADTdepth<<" numberOfItems is "<<numberOfItems<<endl;
  return err;
}

//\begin{>>GeometricADT.tex}{\subsection{delElement}}
template<class dataT>
int 
GeometricADT<dataT>::
delElement(typename GeometricADT<dataT>::iterator &delItem)
//=======================================================
// /Purpose : delete an element, at location delItem, from the tree
// /delItem : iterator pointing to the location to delete from the tree
//\end{GeometricADT.tex} 
//=======================================================
{
  // structure of the three allows us to just shift elements up, deleting only at the leaves
  ADTType *delNode = delItem.current;
  int depth = delItem.getDepth();

  shiftTreeUp(delNode, depth);

  // force this iterator to be invalid
  delItem.current = NULL;

  return 0;
}

//\begin{>>GeometricADT.tex}{\subsection{delElement}}
template<class dataT>
int 
GeometricADT<dataT>::
delElement(typename GeometricADT<dataT>::traversor &delItem)
//=======================================================
// /Purpose : delete an element, at location delItem, from the tree
// /delItem : iterator pointing to the location to delete from the tree
//\end{GeometricADT.tex} 
//=======================================================
{
  // structure of the three allows us to just shift elements up, deleting only at the leaves
  ADTType *delNode = delItem.current;
  int depth = delItem.getDepth();

  shiftTreeUp(delNode, depth);

  // force this iterator to be invalid
  delItem.current = NULL;

  return 0;
}

//\begin{>>GeometricADT.tex}{\subsection{verifyTree}}
template<class dataT>
void
GeometricADT<dataT>::
verifyTree()
//=======================================================
// /Purpose : verify the structure and logic of the tree, usually done after deletions
// This method will throw a VerificationError exception if the data structure has
// been corrupted.  Use of this method aught to be surrounded by a try block.
//\end{GeometricADT.tex}
//=======================================================
{
  
  if (adtRoot!=NULL) verifyNode(*adtRoot, 0);
}


template<class dataT>
void
GeometricADT<dataT>::
verifyNode(typename GeometricADT<dataT>::ADTType &node, int depth)
{

  // a preorder traversal of the tree checking the bisections at each level
  // note that this ensures only that the tree is reasonable, not that the data 
  // stored at each node is... (we could add an optional argument that takes a function to check this as well)

  //assert(node!=NULL);

  if (node.querry(ADT_LEFT))
    verifyNode(node.getLeaf(ADT_LEFT), depth+1);
  if (node.querry(ADT_RIGHT))
    verifyNode(node.getLeaf(ADT_RIGHT), depth+1);

  if (node.querry()) // i.e. not at the root
    {
      //      const realArray &nKey = (node.getData()).boundingBox;
      //const realArray & tKey = (node.getTrunk().getData()).boundingBox;
      const ArraySimple<real> &nKey = (node.getData()).boundingBox;
      const ArraySimple<real> & tKey = (node.getTrunk().getData()).boundingBox;
      int splitAxis0 = getSplitAxis(depth-1);
      real splitLoc0  = getSplitLocation(splitAxis0, tKey);
      // if the following assertions fail then something really rotten has happened, verification fails

      if (&node == &((node.getTrunk()).getLeaf(ADT_LEFT)))
	AssertException (splitLoc0 == nKey(2*splitAxis0+1), VerificationError());
      else
	AssertException (splitLoc0 == nKey(2*splitAxis0), VerificationError());

    }  
}

template<class dataT>
void
GeometricADT<dataT>::
shiftTreeUp(typename GeometricADT<dataT>::ADTType *node, int depth)
{

  if (node->querry(ADT_LEFT))
    {
      (node->getData().data) = (node->getLeaf(ADT_LEFT)).getData().data;
      (node->getData().coords) = (node->getLeaf(ADT_LEFT)).getData().coords;
      shiftTreeUp(&(node->getLeaf(ADT_LEFT)), depth + 1);
    }
  else if(node->querry(ADT_RIGHT))
    {
      (node->getData().data) = (node->getLeaf(ADT_RIGHT)).getData().data;
      (node->getData().coords) = (node->getLeaf(ADT_RIGHT)).getData().coords;
      shiftTreeUp(&(node->getLeaf(ADT_RIGHT)), depth + 1);
    }
  else
    {
      
      if (node->querry())
	if (node == &((node->getTrunk()).getLeaf(ADT_LEFT)))
	  {
	    //cout<<"deleting trunk's left"<<endl;
	    (node->getTrunk()).del(ADT_LEFT);
	    //(node->getTrunk()).change(ADT_LEFT, NULL);
	  }
	else if (node == &((node->getTrunk()).getLeaf(ADT_RIGHT)))
	  {
	    //cout <<"deleting trunk's right"<<endl;
	    (node->getTrunk()).del(ADT_RIGHT);
	    //(node->getTrunk()).change(ADT_RIGHT, NULL);
	  }
	else
	  ;
      else
	{
	  // looks like there is only one node left ..., make sure of that with exceptions
	  AssertException (!debug_GeometricADT || ((numberOfItems - 1)==0), UnknownError());
	  AssertException (!debug_GeometricADT || (node == adtRoot), UnknownError());
	  delete adtRoot;
	  adtRoot = NULL;
	} 

      node = NULL;
      //if (depth==ADTdepth) ADTdepth = depth-1;
      numberOfItems--;
    }

} 

#endif
