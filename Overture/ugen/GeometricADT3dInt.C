//kkc 081124 #include <iostream.h>
#include <iostream>

#include "GeometricADT3dInt.h"

using namespace std;

// *wdh* turn off assertions.
// define ASSERTIONS_ON
#undef ASSERTIONS_ON

const bool debug_GeometricADT3dInt = true;  // set to false to remove AssertException calls 

//\begin{>GeometricADT3dInt.tex}{\subsection{Constructor}}

GeometricADT3dInt::
GeometricADT3dInt(int rangeDimension_)
//======================================================
// /Purpose : construct a geometric search ADT for a given 6
// /rangeDimension\_ (input) : 6 of the physical search space
//\end{GeometciADT.tex}
//======================================================
{
  rangeDimension = rangeDimension_;
  initTree();
}

//\begin{>>GeometricADT3dInt.tex}{\subsection{Constructor}}

GeometricADT3dInt::
GeometricADT3dInt(int rangeDimension_, const real *boundingBox_)
//======================================================
// /Purpose : construct a geometric search ADT given a bounding box domain
// /boundingBox[2*rangeDimension\_]\_ (input) : bounding box for all subsequent insertions. 
//        *NOTE* change from previous version which had a bounding box for the ADT tree which
//        has twice as many entries. 
//\end{GeometciADT.tex}
//======================================================
{
  initTree(rangeDimension_,boundingBox_);
}


GeometricADT3dInt::
~GeometricADT3dInt() 
{
  if (adtRoot!=NULL) 
    {
      adtRoot->del(ADT_LEFT); // delete left
      adtRoot->del(ADT_RIGHT); // delete right
      delete adtRoot;
      adtRoot = NULL;
    }
}

//begin{>>GeometricADT3dInt.tex}{\subsection{initTree}}

void
GeometricADT3dInt::
initTree()
//=======================================================
// /Purpose : initialize the tree, sets up bounding boxes and does some assertion checks
// If a bounding box has not been provided, the bounding box is assumed to be the unit box
//\end{GeometricADT3dInt.tex}
//=======================================================
{
  
#if 1
  AssertException  (!debug_GeometricADT3dInt ||
		    (rangeDimension==1 || rangeDimension==2 || rangeDimension==3), InvalidADTDimension());
#else
  AssertException<InvalidADTDimension> (!debug_GeometricADT3dInt ||
					(rangeDimension==1 || rangeDimension==2 || rangeDimension==3));
#endif
  //assert(rangeDimension==1 || rangeDimension==2 || rangeDimension==3);
  
  //cout << "rangeDimension of GeometricADT3dInt is "<<rangeDimension<<endl;
  ADTDimension = 2*rangeDimension;
  // set the default bounding box.
  for( int axis=0; axis<12; axis+=2 )
  {
    boundingBox[axis]=0.;
    boundingBox[axis+1]=1.;
  }

  adtRoot = NULL;
  ADTdepth = 0;
  numberOfItems = 0;
  
}

//\begin{>>GeometricADT3dInt.tex}{\subsection{initTree}}

void 
GeometricADT3dInt::
initTree(int rangeDimension_, const real *boundingBox_) 
//=======================================================
// /Purpose : initialize the tree given a particular bounding box
// /rangeDimension_ (input) :
// /boundingBox[2*rangeDimension] (input) \_ : bounding box for all subsequent insertions. 
//        *NOTE* change from previous version
//\end{GeometricADT3dInt.tex}
//=======================================================
{
  rangeDimension = rangeDimension_;
  initTree();
  for( int axis=0; axis<6; axis+=2 )
  {
    // for now we use:  min(xMin)==min(xMax)
    boundingBox[2*axis  ] = boundingBox_[axis  ];   // min(xMin)
    boundingBox[2*axis+1] = boundingBox_[axis+1];   // max(xMin)
    boundingBox[2*axis+2] = boundingBox_[axis  ];   // min(xMax)
    boundingBox[2*axis+3] = boundingBox_[axis+1];   // max(xMax)
  }
  ADTDimension = 2*rangeDimension;
}

//\begin{>>GeometricADT3dInt.tex}{\subsection{insert}}

int
GeometricADT3dInt::
insert(GeometricADT3dInt::iterator &insParent, int leaf, GeomADTTuple3dInt &data)
//=======================================================
// /Purpose : insert a geometric entity into the tree
// /insParent (input) : the position in the tree to insert the data
// /leaf (input) : leaf of insertParent to add the data
// /data (input) : data to add to the tree
//\end{GeometricADT3dInt.tex}
//=======================================================
{
#if 1
   int err = insParent.current->add(leaf, data);
   return err;
#else
  try {
    int err = insParent.current->add(leaf, data);
    return err;
  }
  catch (NTreeNodeError &e) {
    e.debug_print();
    throw TreeInsertionError();
  }
  catch (...) {
    // if it is not an error in NTreeNode2GeomADTTuple3dInt, then we don't know what it is, rethrow and pray (or abort)
    throw;
  }
#endif
}

//\begin{>>GeometricADT3dInt.tex}{\subsection{insert}}

int
GeometricADT3dInt::
insert(GeometricADT3dInt::iterator &insParent, int leaf)
//=======================================================
// /Purpose : insert an empty geometric entity into the tree
// /insParent (input) : the position in the tree to insert the data
// /leaf (input) : leaf of insertParent to add the data
//\end{GeometricADT3dInt.tex}
//=======================================================
{
   int err = insParent.current->add(leaf);
   return err;
}

//\begin{>>GeometricADT3dInt.tex}{\subsection{addElement}}

int
GeometricADT3dInt::
addElement(const real *coords, int &data)
//=======================================================
// /Purpose : insert id into the search tree if id give the bounding box coordinates coords
// /coords (input) : bounding box coordinates for id (x1min, x1max, x2min, x2max,..., xnmin, xnmax)
// /id (input) : id to store at the given location (probably should be a templatized type
//\end{GeometricADT3dInt.tex}
//=======================================================
{

  int err=0;
	
//   const realArray & bb = boundingBox;
//   const real *coords = coords_.getDataPointer();
//   const real *bbp = boundingBox.getDataPointer();

  for (int axis = 0; axis<ADTDimension; axis++)
    AssertException (!debug_GeometricADT3dInt ||
		     (boundingBox[2*axis]<=coords[axis] && coords[axis]<=boundingBox[2*axis+1]), OutOfBoundingBox());

  if (numberOfItems == 0)  // this is the first element to be added 
    {
      AssertException ((!debug_GeometricADT3dInt || (adtRoot == NULL)), NULLADTRoot());

      adtRoot = new ADTType();
      adtRoot->getData().setData(boundingBox, coords, data);

      numberOfItems = 1;
      ADTdepth = 0;
      err = 0;
    } else {

      // iterate until we find the location to stick this thing
      GeometricADT3dInt::iterator iter(*this,coords);    // ** can we avoid building an iterator?
      
      for ( ; !iter.isTerminal(); ++iter) {
	//cout << iter.getDepth()<<endl;
      }
      
      const real *parKey = (*iter).boundingBox; 
      
      const int parentDepth = iter.getDepth();
      //cout<<"parent depth is"<<endl;
      // there are three possibilities:
      // 1. the left and right leaves are both empty -- we must choose one
      // 2. the left tree is the one we want, the right is occupied but excludes coords
      // 3. the right tree is the one we want, the left is occupied but excludes coords
      int splitAxis = getSplitAxis(parentDepth);
      real splitLoc = getSplitLocation(splitAxis, parKey);
      
      // create new boundingBox
      real newBox[12];
      for( int axis=0; axis<12; axis++ )
        newBox[axis] = parKey[axis];

      if (coords[splitAxis]<=splitLoc) // go left 
	{
	  newBox[2*splitAxis + 1] = splitLoc;

	  err = insert(iter, ADT_LEFT);
          iter.current->getLeaf(ADT_LEFT).getData().setData(newBox, coords, data);

	} else { // go right
	  newBox[2*splitAxis] = splitLoc;

	  err = insert(iter, ADT_RIGHT);
          iter.current->getLeaf(ADT_RIGHT).getData().setData(newBox, coords, data);

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

//\begin{>>GeometricADT3dInt.tex}{\subsection{delElement}}

int 
GeometricADT3dInt::
delElement(GeometricADT3dInt::iterator &delItem)
//=======================================================
// /Purpose : delete an element, at location delItem, from the tree
// /delItem : iterator pointing to the location to delete from the tree
//\end{GeometricADT3dInt.tex} 
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

//\begin{>>GeometricADT3dInt.tex}{\subsection{verifyTree}}

void
GeometricADT3dInt::
verifyTree()
//=======================================================
// /Purpose : verify the structure and logic of the tree, usually done after deletions
// This method will throw a VerificationError exception if the data structure has
// been corrupted.  Use of this method aught to be surrounded by a try block.
//\end{GeometricADT3dInt.tex}
//=======================================================
{
  
  if (adtRoot!=NULL) verifyNode(*adtRoot, 0);
}



void
GeometricADT3dInt::
verifyNode(GeometricADT3dInt::ADTType &node, int depth)
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
      const real *nKey = (node.getData()).boundingBox;
      const real *tKey = (node.getTrunk().getData()).boundingBox;
      int splitAxis0 = getSplitAxis(depth-1);
      real splitLoc0  = getSplitLocation(splitAxis0, tKey);
      // if the following assertions fail then something really rotten has happened, verification fails

      if (&node == &((node.getTrunk()).getLeaf(ADT_LEFT)))
	AssertException (splitLoc0 == nKey[2*splitAxis0+1], VerificationError());
      else
	AssertException (splitLoc0 == nKey[2*splitAxis0], VerificationError());

    }  
}


void
GeometricADT3dInt::
shiftTreeUp(GeometricADT3dInt::ADTType *node, int depth)
{

  if (node->querry(ADT_LEFT))
    {
      (node->getData().data) = (node->getLeaf(ADT_LEFT)).getData().data;
      real *coord1=node->getData().coords, *coord2=node->getLeaf(ADT_LEFT).getData().coords;
      for( int axis=0; axis<6; axis++ )
        coord1[axis]=coord2[axis];
      shiftTreeUp(&(node->getLeaf(ADT_LEFT)), depth + 1);
    }
  else if(node->querry(ADT_RIGHT))
    {
      (node->getData().data) = (node->getLeaf(ADT_RIGHT)).getData().data;
      real *coord1=node->getData().coords, *coord2=node->getLeaf(ADT_RIGHT).getData().coords;
      for( int axis=0; axis<6; axis++ )
        coord1[axis]=coord2[axis];
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
	  AssertException (!debug_GeometricADT3dInt || ((numberOfItems - 1)==0), UnknownError());
	  AssertException (!debug_GeometricADT3dInt || (node == adtRoot), UnknownError());
	  delete adtRoot;
	  adtRoot = NULL;
	} 

      node = NULL;
      //if (depth==ADTdepth) ADTdepth = depth-1;
      numberOfItems--;
    }

} 


// ====================================================================================================



// implementation of GeometricADT3dInt::iterator class

//\begin{>GeometricADTIterator.tex}{\subsection{Constructor}}

GeometricADTIterator3dInt::
GeometricADTIterator3dInt(const GeometricADT3dInt &gADT, const real *target_) 
// ====================================================================================================
// /Purpose : initialize the iterator with a given {\tt GeometricADT3dInt} and target bounding box location
// /gADT (input) : the {\tt GeometricADT3dInt} through which to iterate
// /target\_ (input) : a {\tt realArray} containing the target bounding box location
//\end{GeometricADTIterator.tex}
// ====================================================================================================
{
  thisADT = (GeometricADT3dInt *)&gADT;
  for( int axis=0; axis<6; axis++ )
    target[axis] = target_[axis];
  current = gADT.adtRoot;
  depth = 0;


}

//\begin{>>GeometricADTIterator.tex}{\subsection{Copy Constructor}}

GeometricADTIterator3dInt::
GeometricADTIterator3dInt(GeometricADTIterator3dInt &x) : thisADT(x.thisADT)
// ====================================================================================================
// /Purpose : copy the state of x into this iterator
// /x : iterator to copy
//\end{GeometricADTIterator.tex}
// ====================================================================================================
{
  for( int axis=0; axis<6; axis++ )
    target[axis] = x.target[axis];
  depth = x.depth;
  current = x.current;
}


GeometricADTIterator3dInt::
~GeometricADTIterator3dInt()
{
  // this does not need to do anything yet 
}


//\begin{>>GeometricADTIterator.tex}{\subsection{Assignment to Another Iterator}}

GeometricADTIterator3dInt & 
GeometricADTIterator3dInt::
operator=(GeometricADTIterator3dInt &i)
// ====================================================================================================
// /Purpose : assign the iterator to the value of another, only change what the iterator is pointing to,not the iteration criteria (target)
// /i : iterator to assign to
//\end{GeometricADTIterator.tex}
// ====================================================================================================
{
// only change what the iterator is pointing to, not the iteration criteria (target) in operator=
  
  this->current = i.current;
  //this->target  = target;
  this->depth   = i.depth;
  

  return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Assignment to a Node in the Seach Tree}}

GeometricADTIterator3dInt &
GeometricADTIterator3dInt::
operator=(__ADTType &x)
// ====================================================================================================
// /Purpose : assign the iterator's current pointer to a particular node
// /x : node to assign current value to
//\end{GeometricADTIterator.tex}
// ====================================================================================================
{
  //this-> = &x;
  
  //this->thisADT = i.thisADT;
  this->current = &x;
  //this->target  = target;
  //  this->depth   = depth;
    return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Prefix Increment}}

GeometricADTIterator3dInt & GeometricADTIterator3dInt::
operator++()
// ====================================================================================================
// /Purpose : increment the iterator (advance the iteration) and then return the iterator
//\end{GeometricADTIterator.tex}
// ====================================================================================================
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
  if (target[splitAxis]<=splitLoc) 
    current = &(current->getLeaf(ADT_LEFT));
  else
    current = &(current->getLeaf(ADT_RIGHT));

  depth++;

  return *this;
}

//\begin{>>GeometricADTIterator.tex}{\subsection{Postfix Increment}}

GeometricADTIterator3dInt 
GeometricADTIterator3dInt::
operator++(int)
// ====================================================================================================
// /Purpose : increment the iterator (advance the iteration) and then return the old value
//\end{GeometricADTIterator.tex}
// ====================================================================================================
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

  GeometricADTIterator3dInt tmp(*this);

  if (target[splitAxis]<=splitLoc) 
    current = &(current->getLeaf(ADT_LEFT));
  else
    current = &(current->getLeaf(ADT_RIGHT));

  depth++;

  return tmp;
}

#if 0
GeometricADTIterator3dInt & 
GeometricADTIterator3dInt::
operator--()
{
  --(GeometricADT3dInt::ADTType::selection_iterator(*this));
  return *this;
#if 0
  current = current->getTrunk();
  depth--;
#endif
}
#endif


// ====================================================================================================


// implementation of GeometricADTTraversor3dInt class


//\begin{>GeometricADTTraversor.tex}{\subsection{Constructor}}

GeometricADTTraversor3dInt::
GeometricADTTraversor3dInt(const GeometricADT3dInt &gADT, const real *target_) 
// ====================================================================================================
// /Purpose : initialize the iterator with a given {\tt GeometricADT3dInt} and target bounding box location
// /gADT (input) : the {\tt GeometricADT3dInt} through which to iterate
// /target[2*rangeDimension]\_ (input) : an array containing the target bounding box location
//\end{GeometricADTIterator.tex}
// ====================================================================================================
{
  thisADT = (GeometricADT3dInt *) &gADT;
  int axis;
  for( axis=0; axis<6; axis++ )
    target[axis] = target_[axis];

  current = gADT.adtRoot;
  depth = 0;
  //leftSubTreeFinished.clear();
  leftSubTreeFinished.push_front(false);
  traversalFinished = false;

  for( axis=0; axis<6; axis+=2 )
  {
    a[axis]=thisADT->boundingBox[2*axis];
    a[axis+1]=target[axis];
    b[axis]=target[axis+1];
    b[axis+1]=thisADT->boundingBox[2*axis+3];
  }
  
  if (!(isCandidate(current->getData().coords))) ++(*this);
}


GeometricADTTraversor3dInt::
GeometricADTTraversor3dInt(const GeometricADT3dInt &gADT )
{
  thisADT = (GeometricADT3dInt *) &gADT;
  // should we init other data??
}


GeometricADTTraversor3dInt::
GeometricADTTraversor3dInt(GeometricADTTraversor3dInt &x) : thisADT(x.thisADT)
{
  depth = x.depth;
  current = x.current;
  leftSubTreeFinished = x.leftSubTreeFinished;
  traversalFinished = false;

  for( int axis=0; axis<6; axis++ )
  {
    target[axis]=x.target[axis];
    a[axis]= x.a[axis];
    b[axis]= x.b[axis];
  }
  
  if (!(isCandidate(current->getData().coords))) ++(*this);

}


GeometricADTTraversor3dInt::
~GeometricADTTraversor3dInt()
{
  //leftSubTreeFinished.clear();
}

//\begin{>GeometricADTTraversor.tex}{\subsection{setTarget}}

void GeometricADTTraversor3dInt::
setTarget(const real *target_) 
{
  assert( thisADT!=NULL );
  int axis;
  for( axis=0; axis<6; axis++ )
    target[axis] = target_[axis];

  current = thisADT->adtRoot;
  depth = 0;

  leftSubTreeFinished.clear();
  leftSubTreeFinished.push_front(false);
  traversalFinished = false;

  for( axis=0; axis<6; axis+=2 )
  {
    a[axis]=thisADT->boundingBox[2*axis];
    a[axis+1]=target[axis];
    b[axis]=target[axis+1];
    b[axis+1]=thisADT->boundingBox[2*axis+3];
  }
  
  if (!(isCandidate(current->getData().coords))) ++(*this);
}




// only change what the traversor is pointing to, not the iteration criteria (target) in operator=

GeometricADTTraversor3dInt  & 
GeometricADTTraversor3dInt::
operator=(GeometricADTTraversor3dInt &i)
{
  
  this->current = i.current;
  //this->target  = target;
  this->depth   = i.depth;
  leftSubTreeFinished = i.leftSubTreeFinished;

  return *this;
}


GeometricADTTraversor3dInt & 
GeometricADTTraversor3dInt::
operator=(__ADTType &x)
{
  //this-> = &x;
  
  //this->thisADT = i.thisADT;
  this->current = &x;
  //this->target  = target;
  //  this->depth   = depth;
    return *this;
}


GeometricADTTraversor3dInt & 
GeometricADTTraversor3dInt::
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


GeometricADTTraversor3dInt
GeometricADTTraversor3dInt::
operator++(int)
{

  GeometricADTTraversor3dInt tmp(*this);

  ++(*this);

  return tmp;

}

#if 0
GeometricADTTraversor3dInt & 
GeometricADTTraversor3dInt::
operator--()
{
  --(GeometricADT3dInt::ADTType::selection_traversor(*this));
  return *this;
#if 0
  current = current->getTrunk();
  depth--;
#endif
}
#endif




